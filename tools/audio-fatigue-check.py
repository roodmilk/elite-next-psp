#!/usr/bin/env python3
"""Report simple repeat-listening risk measures for boost review WAVs."""
import json
import wave
from pathlib import Path

import numpy as np

root = Path(__file__).resolve().parents[1] / "audio"
files = list((root / "review" / "boost-signatures").glob("*.wav"))
files += list((root / "rejected" / "2026-10-12-grating-boost").glob("*.wav"))
report = {}
for path in files:
    with wave.open(str(path), "rb") as stream:
        rate = stream.getframerate()
        channels = stream.getnchannels()
        width = stream.getsampwidth()
        raw = stream.readframes(stream.getnframes())
    if width == 2:
        pcm = np.frombuffer(raw, dtype="<i2").astype(np.float32) / 32768.0
    elif width == 3:
        triples = np.frombuffer(raw, dtype=np.uint8).reshape(-1, 3)
        values = (triples[:, 0].astype(np.int32) |
                  triples[:, 1].astype(np.int32) << 8 |
                  triples[:, 2].astype(np.int32) << 16)
        pcm = np.where(values & 0x800000, values - 0x1000000, values).astype(np.float32) / 8388608.0
    else:
        raise ValueError(f"unsupported sample width {width}: {path}")
    signal = pcm.reshape(-1, channels).mean(axis=1)
    window = signal[:min(len(signal), rate * 8)]
    spectrum = np.abs(np.fft.rfft(window)) ** 2
    frequencies = np.fft.rfftfreq(len(window), 1 / rate)
    rms = float(np.sqrt(np.mean(signal * signal)))
    peak = float(np.max(np.abs(signal)))
    report[path.name] = {
        "peak": round(peak, 3),
        "rms": round(rms, 3),
        "crest_db": round(float(20 * np.log10(peak / (rms + 1e-9))), 1),
        "energy_above_5khz_pct": round(float(100 * spectrum[frequencies > 5000].sum() / spectrum.sum()), 2),
    }
print(json.dumps(report, indent=2))
