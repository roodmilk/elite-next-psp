#!/usr/bin/env python3
"""Fail on clipping or exposed discontinuities in generated flight audio."""
import json
import re
import sys
from pathlib import Path

import numpy as np

root = Path(__file__).resolve().parents[1]
header = (root / "src/generated/audio-flight-pcm.h").read_text(encoding="ascii")
report = {}
failed = False
for name in ("flight_boost_warm_gravity_pcm", "flight_dock_clamp_pcm"):
    match = re.search(name + r"\[\d+\].*?= \{(.*?)\};", header, re.S)
    if not match:
        raise SystemExit(f"FAIL missing resident PCM: {name}")
    pcm = np.asarray([int(value) for value in re.findall(r"-?\d+", match.group(1))], dtype=np.int16)
    difference = np.abs(np.diff(pcm.astype(np.int32)))
    clipped = int(np.count_nonzero(np.abs(pcm) >= 127))
    large_steps = int(np.count_nonzero(difference > 64))
    item_failed = clipped or large_steps or abs(int(pcm[0])) > 1 or abs(int(pcm[-1])) > 1
    report[name] = {
        "samples": int(len(pcm)), "start": int(pcm[0]), "end": int(pcm[-1]),
        "peak": int(np.max(np.abs(pcm))), "clipped_samples": clipped,
        "largest_adjacent_step": int(difference.max(initial=0)),
        "steps_over_half_scale": large_steps, "result": "FAIL" if item_failed else "PASS",
    }
    failed |= item_failed
for name in ("flight_engine_industrial_pcm", "flight_cockpit_machinery_pcm"):
    match = re.search(name + r"\[\d+\].*?= \{(.*?)\};", header, re.S)
    if not match:
        raise SystemExit(f"FAIL missing resident loop: {name}")
    pcm = np.asarray([int(value) for value in re.findall(r"-?\d+", match.group(1))], dtype=np.int32)
    difference = np.abs(np.diff(pcm))
    seam = abs(int(pcm[-1])-int(pcm[0]))
    threshold = max(512, int(np.percentile(difference, 99))*2)
    clipped = int(np.count_nonzero(np.abs(pcm)>=32767))
    item_failed = clipped or seam>threshold
    report[name] = {"samples":int(len(pcm)), "loop_seam_step":seam,
        "seam_step_limit":threshold, "largest_internal_step":int(difference.max()),
        "clipped_samples":clipped, "result":"FAIL" if item_failed else "PASS"}
    failed |= item_failed
report["check_scope"] = "PCM clipping and exposed loop/event discontinuities; listening remains required"
report["result"] = "FAIL" if failed else "PASS"
print(json.dumps(report, indent=2))
sys.exit(1 if failed else 0)
