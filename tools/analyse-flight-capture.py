#!/usr/bin/env python3
"""Measure an actual PPSSPP stereo capture without altering it."""
from __future__ import annotations

import argparse
import json
import wave
from pathlib import Path

import numpy as np


def analyse(path: Path) -> dict:
    with wave.open(str(path), "rb") as stream:
        channels, width, rate, frames = (stream.getnchannels(), stream.getsampwidth(),
                                         stream.getframerate(), stream.getnframes())
        if channels != 2 or width != 2:
            raise ValueError("expected 16-bit stereo PPSSPP capture")
        pcm = np.frombuffer(stream.readframes(frames), dtype="<i2").reshape(-1, 2).astype(np.float64)
    normal = pcm / 32768.0
    mono = normal.mean(axis=1)
    spectrum = np.fft.rfft(mono)
    frequency = np.fft.rfftfreq(len(mono), 1.0 / rate)
    energy = np.abs(spectrum) ** 2
    return {
        "file": str(path.resolve()),
        "rate_hz": rate,
        "duration_seconds": round(frames / rate, 3),
        "peak": round(float(np.max(np.abs(normal))), 5),
        "rms": round(float(np.sqrt(np.mean(normal ** 2))), 5),
        "stereo_difference_rms": round(float(np.sqrt(np.mean((normal[:, 0] - normal[:, 1]) ** 2))), 7),
        "dc": [round(float(v), 7) for v in normal.mean(axis=0)],
        "clipped_samples": int(np.count_nonzero(np.abs(pcm) >= 32767)),
        "largest_adjacent_step": round(float(np.max(np.abs(np.diff(normal, axis=0)))), 5),
        "energy_above_5khz_pct": round(float(100 * energy[frequency >= 5000].sum() / max(1.0, energy.sum())), 3),
        "silent_frames_pct": round(float(100 * np.count_nonzero(np.max(np.abs(pcm), axis=1) <= 1) / frames), 3),
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("capture", type=Path)
    args = parser.parse_args()
    result = analyse(args.capture)
    # A signal gate does not prove sound quality, absence of crackles, or runtime
    # stability. Those require listening and separate performance evidence.
    result["check_scope"] = "captured signal levels only; no listening-quality claim"
    result["result"] = "PASS" if result["clipped_samples"] == 0 and result["peak"] < .98 else "FAIL"
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
