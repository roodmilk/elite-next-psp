#!/usr/bin/env python3
"""Build review masters and PSP-resident PCM from licensed field recordings.

The script deliberately has no codec or DSP package dependency beyond NumPy.  It
accepts PCM and IEEE-float RIFF/WAVE sources, performs all work in float32, and
writes deterministic files so source hashes and shipped assets can be audited.
"""

from __future__ import annotations

import argparse
import json
import math
import struct
import wave
from pathlib import Path

import numpy as np


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "audio" / "sources" / "opengameart"
REVIEW = ROOT / "audio" / "review" / "engine-signatures"
BOOST_REVIEW = ROOT / "audio" / "review" / "boost-signatures"
RUNTIME = ROOT / "audio" / "runtime"
GENERATED = ROOT / "src" / "generated"
MASTER_RATE = 48000
PSP_RATE = 22050
BOOST_RATE = 8000


def riff_chunks(data: bytes):
    if data[:4] != b"RIFF" or data[8:12] != b"WAVE":
        raise ValueError("not a RIFF/WAVE file")
    pos = 12
    while pos + 8 <= len(data):
        tag, size = struct.unpack_from("<4sI", data, pos)
        pos += 8
        yield tag, data[pos : pos + size]
        pos += size + (size & 1)


def read_wav(path: Path) -> tuple[np.ndarray, int]:
    chunks = list(riff_chunks(path.read_bytes()))
    fmt = next(body for tag, body in chunks if tag == b"fmt ")
    raw = next(body for tag, body in chunks if tag == b"data")
    form, channels, rate, _, _, bits = struct.unpack_from("<HHIIHH", fmt)
    if form == 0xFFFE and len(fmt) >= 40:
        form = struct.unpack_from("<H", fmt, 24)[0]
    if form == 3 and bits == 32:
        samples = np.frombuffer(raw, dtype="<f4").astype(np.float32)
    elif form == 1 and bits == 16:
        samples = np.frombuffer(raw, dtype="<i2").astype(np.float32) / 32768.0
    elif form == 1 and bits == 24:
        triples = np.frombuffer(raw, dtype=np.uint8).reshape(-1, 3)
        values = (triples[:, 0].astype(np.int32) |
                  (triples[:, 1].astype(np.int32) << 8) |
                  (triples[:, 2].astype(np.int32) << 16))
        values = np.where(values & 0x800000, values - 0x1000000, values)
        samples = values.astype(np.float32) / 8388608.0
    else:
        raise ValueError(f"unsupported WAVE format={form} bits={bits}: {path}")
    samples = samples[: len(samples) - (len(samples) % channels)]
    samples = samples.reshape(-1, channels).mean(axis=1)
    samples = np.nan_to_num(samples, copy=False)
    return np.clip(samples, -1.0, 1.0), rate


def resample(x: np.ndarray, source_rate: int, target_rate: int) -> np.ndarray:
    if source_rate == target_rate:
        return x.copy()
    count = max(1, int(round(len(x) * target_rate / source_rate)))
    old = np.linspace(0.0, 1.0, len(x), endpoint=False)
    new = np.linspace(0.0, 1.0, count, endpoint=False)
    return np.interp(new, old, x).astype(np.float32)


def normalise(x: np.ndarray, peak: float = 0.82) -> np.ndarray:
    x = x - float(np.mean(x))
    current = float(np.max(np.abs(x))) if len(x) else 0.0
    return x if current < 1e-8 else (x * (peak / current)).astype(np.float32)


def level_match(x: np.ndarray, target_rms: float = .16, peak: float = .76) -> np.ndarray:
    x = x - float(np.mean(x))
    rms = float(np.sqrt(np.mean(x.astype(np.float64) ** 2)))
    maximum = float(np.max(np.abs(x)))
    if rms < 1e-8 or maximum < 1e-8:
        return x.astype(np.float32)
    scale = min(target_rms / rms, peak / maximum)
    return (x * scale).astype(np.float32)


def seamless(x: np.ndarray, rate: int, seconds: float = 1.5) -> np.ndarray:
    needed = int(rate * seconds)
    if len(x) < needed:
        x = np.tile(x, int(math.ceil(needed / len(x))))
    x = x[:needed].copy()
    fade = min(int(rate * 0.08), needed // 4)
    if fade:
        t = np.linspace(0.0, 1.0, fade, endpoint=False, dtype=np.float32)
        head, tail = x[:fade].copy(), x[-fade:].copy()
        blend = tail * (1.0 - t) + head * t
        x[:fade] = blend
        x[-fade:] = blend
    return normalise(x)


def lowpass(x: np.ndarray, taps: int) -> np.ndarray:
    taps = max(1, taps)
    kernel = np.ones(taps, dtype=np.float32) / taps
    return np.convolve(x, kernel, mode="same").astype(np.float32)


def sustained_machine(parts, seconds, rate=PSP_RATE):
    """Resident recorded-engine bed without the rejected defect-fan impulses.

    Keep the existing sample budget. Circular RMS levelling restrains recurrent
    amplitude knocks without generating noise or replacing the recorded texture.
    This is a listening candidate, not a numerical guarantee of good sound.
    """
    count = int(rate * seconds)
    overlap = int(rate * .06)
    out = np.zeros(count + overlap, dtype=np.float32)
    def cyclic(x, fade):
        blend = np.linspace(0, 1, fade, endpoint=False)
        return np.r_[x[fade:-fade], x[-fade:]*(1-blend)+x[:fade]*blend]
    for index, (recording, gain, speed) in enumerate(parts):
        src = cyclic(lowpass(recording, 31), min(2880, len(recording)//4))
        positions = (np.arange(len(out)) * MASTER_RATE / rate * speed + index * 10331) % len(src)
        out += np.interp(positions, np.arange(len(src)+1), np.r_[src, src[0]]) * gain
    out = cyclic(out, overlap)
    # Circular filtering avoids introducing an edge dip at the loop boundary.
    taps = int(47*rate/PSP_RATE)|1
    out = np.convolve(np.r_[out[-taps:], out, out[:taps]], np.ones(taps)/taps, mode='same')[taps:-taps]
    window = int(rate*.06)|1
    energy = np.convolve(np.r_[out[-window:]**2, out*out, out[:window]**2], np.ones(window)/window, mode='same')[window:-window]
    target = np.sqrt(np.mean(out*out))
    correction = np.clip(target/np.sqrt(np.maximum(energy, 1e-8)), .5, 2.0)
    out *= correction ** .75
    return normalise(out.astype(np.float32))


def layer(parts: list[tuple[np.ndarray, float]], count: int) -> np.ndarray:
    out = np.zeros(count, dtype=np.float32)
    for source, gain in parts:
        out += np.resize(source, count) * gain
    return normalise(np.tanh(out * 1.25).astype(np.float32), 0.78)


def write_wav16(path: Path, x: np.ndarray, rate: int, stereo=False):
    path.parent.mkdir(parents=True, exist_ok=True)
    y = np.clip(x, -0.999, 0.999)
    if stereo:
        delay = max(1, int(rate * 0.007))
        right = np.roll(y, delay) * 0.92
        y = np.column_stack((y, right)).reshape(-1)
    pcm = np.round(y * 32767.0).astype("<i2")
    with wave.open(str(path), "wb") as stream:
        stream.setnchannels(2 if stereo else 1)
        stream.setsampwidth(2)
        stream.setframerate(rate)
        stream.writeframes(pcm.tobytes())


def write_wav24(path: Path, x: np.ndarray, rate: int, stereo=False):
    path.parent.mkdir(parents=True, exist_ok=True)
    y = np.clip(x, -0.999, 0.999)
    if stereo:
        delay = max(1, int(rate * 0.007))
        right = np.roll(y, delay) * 0.92
        y = np.column_stack((y, right)).reshape(-1)
    values = np.round(y * 8388607.0).astype(np.int32)
    packed = np.empty((len(values), 3), dtype=np.uint8)
    packed[:, 0] = values & 0xFF
    packed[:, 1] = (values >> 8) & 0xFF
    packed[:, 2] = (values >> 16) & 0xFF
    with wave.open(str(path), "wb") as stream:
        stream.setnchannels(2 if stereo else 1)
        stream.setsampwidth(3)
        stream.setframerate(rate)
        stream.writeframes(packed.tobytes())


def crossfade_segments(segments: list[np.ndarray], rate: int, overlap=0.16):
    fade = int(rate * overlap)
    out = segments[0].copy()
    for segment in segments[1:]:
        t = np.linspace(0.0, 1.0, fade, endpoint=False, dtype=np.float32)
        out[-fade:] = out[-fade:] * (1.0 - t) + segment[:fade] * t
        out = np.concatenate((out, segment[fade:]))
    return out


def fit_length(x: np.ndarray, count: int) -> np.ndarray:
    if len(x) == count:
        return x.copy()
    return resample(x, len(x), count)


def soft_fade(x: np.ndarray, rate: int, attack: float, release: float) -> np.ndarray:
    y = x.copy()
    a, r = min(len(y), int(rate * attack)), min(len(y), int(rate * release))
    if a:
        y[:a] *= np.sin(np.linspace(0, math.pi / 2, a, dtype=np.float32)) ** 2
    if r:
        y[-r:] *= np.cos(np.linspace(0, math.pi / 2, r, dtype=np.float32)) ** 2
    return y


def place(out: np.ndarray, source: np.ndarray, start: float, gain: float, rate: int):
    """Mix a source at a time without resizing/repeating its transient."""
    offset = int(start * rate)
    count = min(len(source), len(out) - offset)
    if count > 0:
        out[offset : offset + count] += source[:count] * gain


def boost_signature(engine: np.ndarray, startup: np.ndarray, impact: np.ndarray,
                    resonance: np.ndarray, character: str) -> np.ndarray:
    """Recorded-source boost: stored energy, ignition, loaded engine and tail.

    The upper band is deliberately a short accent.  Sustained level comes from
    the approved warm engine, so repeated boosts do not turn into broadband hiss.
    """
    duration = 3.25
    n = int(duration * MASTER_RATE)
    out = np.zeros(n, dtype=np.float32)
    t = np.arange(n, dtype=np.float32) / MASTER_RATE

    # Give the existing ship a physical acceleration curve. Chunked resampling
    # keeps the source recognisable while pitch rises through the push.
    bed = np.zeros(n, dtype=np.float32)
    cursor = int(.14 * MASTER_RATE)
    for step in range(24):
        seconds = .13
        ratio = .78 + step / 23 * ({"warm": .48, "deep": .40, "pulse": .55}[character])
        chunk_n = int(seconds * MASTER_RATE)
        shifted = resample(engine, MASTER_RATE, int(MASTER_RATE * ratio))
        chunk = np.resize(shifted, chunk_n)
        end = min(n, cursor + chunk_n)
        bed[cursor:end] = chunk[:end-cursor]
        cursor = end
    bed_env = np.clip((t - .14) / .32, 0, 1) * np.clip((duration - t) / .72, 0, 1)
    out += lowpass(bed, 5) * bed_env * ({"warm": .48, "deep": .53, "pulse": .44}[character])

    # The starter is treated as a compact contactor/turbine catch, never a full
    # foreground car start. Removing its bright residual keeps the onset soft.
    catch = startup[int(.05 * MASTER_RATE):int(.72 * MASTER_RATE)]
    catch = lowpass(catch, {"warm": 13, "deep": 21, "pulse": 9}[character])
    catch = soft_fade(normalise(catch, .62), MASTER_RATE, .025, .24)
    place(out, catch, .08 if character != "deep" else .15,
          {"warm": .23, "deep": .19, "pulse": .25}[character], MASTER_RATE)

    # A very short recorded hull impulse supplies weight. Its low-passed copy
    # carries the body; a quieter original edge preserves definition.
    body = soft_fade(normalise(lowpass(impact, 25), .72), MASTER_RATE, .004, .26)
    edge = soft_fade(normalise(impact, .52), MASTER_RATE, .002, .08)
    place(out, body, .28 if character != "pulse" else .22,
          {"warm": .42, "deep": .52, "pulse": .35}[character], MASTER_RATE)
    place(out, edge, .29 if character != "pulse" else .23, .07, MASTER_RATE)

    # One brief ship-specific mechanical answer after ignition. It stays below
    # the engine and is absent from the sustained tail.
    ring = resonance[:int(.70 * MASTER_RATE)]
    ring = soft_fade(normalise(lowpass(ring, 31), .55), MASTER_RATE, .006, .34)
    place(out, ring, {"warm": .56, "deep": .68, "pulse": .47}[character],
          {"warm": .10, "deep": .13, "pulse": .16}[character], MASTER_RATE)

    # A quiet sub-bass displacement conveys scale on headphones and speakers
    # without needing a noisy whoosh or imitating any reference recording.
    sub_hz = {"warm": 46.0, "deep": 38.0, "pulse": 52.0}[character]
    sub_phase = 2 * math.pi * (sub_hz * t + 9.0 * np.minimum(t, .7) ** 2)
    sub_env = np.clip((t - .20) / .055, 0, 1) * np.exp(-np.maximum(t - .27, 0) * 1.22)
    out += np.sin(sub_phase).astype(np.float32) * sub_env * ({"warm": .13, "deep": .17, "pulse": .11}[character])
    return level_match(np.tanh(out * 1.12).astype(np.float32), .145, .78)


def boost_review_journey(signature: np.ndarray, engine: np.ndarray) -> np.ndarray:
    """Context, boost, recovery, then a second trigger for fatigue checking."""
    n = int(9.0 * MASTER_RATE)
    out = np.zeros(n, dtype=np.float32)
    idle = lowpass(np.resize(engine, n), 5) * .16
    fade = np.ones(n, dtype=np.float32)
    fade[:int(.25 * MASTER_RATE)] = np.linspace(0, 1, int(.25 * MASTER_RATE))
    fade[-int(.5 * MASTER_RATE):] = np.linspace(1, 0, int(.5 * MASTER_RATE))
    out += idle * fade
    place(out, signature, 1.15, 1.0, MASTER_RATE)
    place(out, signature, 5.35, .96, MASTER_RATE)
    return level_match(out, .13, .82)


def c_array(name: str, x: np.ndarray) -> str:
    pcm = np.round(np.clip(x, -0.999, 0.999) * 32767).astype(np.int16)
    rows = []
    for offset in range(0, len(pcm), 16):
        rows.append("    " + ", ".join(str(int(v)) for v in pcm[offset : offset + 16]))
    return (f"static const short {name}[{len(pcm)}] __attribute__((aligned(64))) = {{\n" +
            ",\n".join(rows) + "\n};\n")


def c_array8(name: str, x: np.ndarray) -> str:
    pcm = np.round(np.clip(x, -.999, .999) * 127).astype(np.int8)
    rows = []
    for offset in range(0, len(pcm), 20):
        rows.append("    " + ", ".join(str(int(v)) for v in pcm[offset : offset + 20]))
    return (f"static const signed char {name}[{len(pcm)}] __attribute__((aligned(64))) = {{\n" +
            ",\n".join(rows) + "\n};\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--inspect", action="store_true")
    args = parser.parse_args()
    names = ["racing-loop-0.wav", "racing-loop-2.wav", "racing-loop-4.wav",
             "fan-interval.wav", "metal-bong1.wav", "metal-thud2.wav", "metal-thud3.wav",
             "engine_start_up_01.wav",
             "air-move.wav", "jet-engine-takeoff.wav", "steamboat-engine.wav"]
    decoded = {}
    audit = {}
    for name in names:
        samples, rate = read_wav(SOURCE / name)
        decoded[name] = resample(samples, rate, MASTER_RATE)
        audit[name] = {"sample_rate": rate, "frames": len(samples),
                       "duration_seconds": round(len(samples) / rate, 4)}
    if args.inspect:
        print(json.dumps(audit, indent=2))
        return

    loops = {name: seamless(samples, MASTER_RATE) for name, samples in decoded.items()}
    count = int(1.5 * MASTER_RATE)
    fan = loops["fan-interval.wav"]
    low_fan = lowpass(fan, 17)
    high_fan = fan - lowpass(fan, 31)
    variants = {
        "a-industrial-weight": layer([(loops["racing-loop-0.wav"], .58), (low_fan, .32),
                                       (loops["racing-loop-2.wav"], .18)], count),
        "b-turbine-precision": layer([(loops["racing-loop-2.wav"], .46), (high_fan, .34),
                                      (loops["racing-loop-4.wav"], .28)], count),
        "c-mechanical-pulse": layer([(loops["racing-loop-0.wav"], .42), (fan, .44),
                                      (loops["racing-loop-4.wav"], .14)], count),
    }
    # A short identical flight gesture makes tonal and response differences easy to compare.
    envelope_sections = [(0.34, 1.00), (0.55, 1.00), (0.76, 1.20), (1.00, 1.10), (0.52, 0.85)]
    for name, signature in variants.items():
        sections = []
        for speed, duration in envelope_sections:
            n = int(duration * MASTER_RATE)
            shifted = resample(signature, MASTER_RATE, max(8000, int(MASTER_RATE * (0.82 + speed * .42))))
            bed = np.resize(shifted, n)
            ramp = np.linspace(.88, 1.0, n, dtype=np.float32)
            sections.append(normalise(bed * ramp, peak=.72))
        journey = crossfade_segments(sections, MASTER_RATE)
        write_wav24(REVIEW / f"{name}.wav", journey, MASTER_RATE, stereo=True)
        write_wav24(REVIEW / f"{name}-loop.wav", signature, MASTER_RATE, stereo=False)

    boosts = {
        "a-warm-gravity": boost_signature(variants["a-industrial-weight"],
            decoded["engine_start_up_01.wav"], decoded["metal-thud2.wav"],
            decoded["metal-bong1.wav"], "warm"),
        "b-deep-displacement": boost_signature(variants["a-industrial-weight"],
            decoded["engine_start_up_01.wav"], decoded["metal-thud3.wav"],
            decoded["metal-bong1.wav"], "deep"),
        "c-mechanical-catch": boost_signature(variants["a-industrial-weight"],
            decoded["engine_start_up_01.wav"], decoded["metal-thud2.wav"],
            decoded["metal-bong1.wav"], "pulse"),
    }
    for name, signature in boosts.items():
        write_wav24(BOOST_REVIEW / f"{name}.wav",
                    boost_review_journey(signature, variants["a-industrial-weight"]),
                    MASTER_RATE, stereo=True)


    # Preferred A is compacted to PSP rate and embedded; source mastering remains external.
    # The retail build enforces 3 MiB headroom before codec/thread overhead.
    # These different prime-ish loop lengths stay below that budget and avoid
    # an obvious shared repetition cycle while the full masters remain intact.
    # A 0.52-second recorded cycle remains long enough for the continuously
    # changing runtime pitch heads, and releases 5.3 KiB for environmental air.
    engine_parts = [(decoded["racing-loop-0.wav"], .58, .91),
                    (decoded["racing-loop-2.wav"], .28, .73),
                    (decoded["racing-loop-4.wav"], .14, .57)]
    cockpit_parts = [(decoded["racing-loop-2.wav"], .55, .58),
                     (decoded["racing-loop-4.wav"], .45, .71)]
    preferred = sustained_machine(engine_parts, .52)
    cockpit = sustained_machine(cockpit_parts, .39)
    for name, samples in (("engine", preferred), ("cockpit", cockpit)):
        integers = np.round(samples*32767).astype(np.int32)
        differences = np.abs(np.diff(integers))
        seam = abs(int(integers[-1])-int(integers[0]))
        if seam>max(512, int(np.percentile(differences, 99))*2):
            raise ValueError(f"Refusing discontinuous {name} loop: boundary step {seam}")
    master_dir = ROOT / "audio/masters/repaired-continuous-beds"
    write_wav24(master_dir / "engine-bed.wav", sustained_machine(engine_parts, .52, MASTER_RATE), MASTER_RATE)
    write_wav24(master_dir / "cockpit-bed.wav", sustained_machine(cockpit_parts, .39, MASTER_RATE), MASTER_RATE)
    boost_pcm = resample(boosts["a-warm-gravity"][:int(.55 * MASTER_RATE)], MASTER_RATE, BOOST_RATE)
    # One-shot boundaries must return to zero.  The worker can therefore stop
    # this voice or retrigger it without exposing an 8-bit step as a click.
    boost_pcm = soft_fade(normalise(boost_pcm, .78), BOOST_RATE, .008, .075)
    dock_pcm = resample(lowpass(decoded["metal-thud3.wav"][:int(.22 * MASTER_RATE)], 31), MASTER_RATE, BOOST_RATE)
    dock_pcm = soft_fade(normalise(dock_pcm, .62), BOOST_RATE, .006, .085)
    write_wav16(RUNTIME / "engine-industrial-weight-22050.wav", preferred, PSP_RATE)
    write_wav16(RUNTIME / "cockpit-machinery-22050.wav", cockpit, PSP_RATE)
    GENERATED.mkdir(parents=True, exist_ok=True)
    header = """/* Generated by tools/build-flight-audio.py; do not hand-edit. */
#ifndef ELITE_FLIGHT_AUDIO_PCM_H
#define ELITE_FLIGHT_AUDIO_PCM_H
#define FLIGHT_AUDIO_RATE 22050
#define FLIGHT_BOOST_RATE 8000
"""
    header += c_array("flight_engine_industrial_pcm", preferred)
    header += c_array("flight_cockpit_machinery_pcm", cockpit)
    header += c_array8("flight_boost_warm_gravity_pcm", boost_pcm)
    header += c_array8("flight_dock_clamp_pcm", dock_pcm)
    header += "#endif\n"
    (GENERATED / "audio-flight-pcm.h").write_text(header, encoding="ascii", newline="\n")
    (REVIEW / "build-report.json").write_text(json.dumps({
        "sources": audit,
        "review_rate": MASTER_RATE,
        "psp_rate": PSP_RATE,
        "preferred": "a-industrial-weight",
        "preferred_boost": "a-warm-gravity (approved 2026-10-13)",
        "runtime_pcm_bytes": int((len(preferred) + len(cockpit)) * 2 + len(boost_pcm) + len(dock_pcm)),
    }, indent=2) + "\n", encoding="utf-8")
    print(f"Built {len(variants)} engine and {len(boosts)} boost comparisons; resident PCM is {(len(preferred) + len(cockpit)) * 2 + len(boost_pcm) + len(dock_pcm)} bytes")


if __name__ == "__main__":
    main()
