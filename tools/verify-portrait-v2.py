"""Render every old selectable v2 combination with the corrected anatomy."""
from pathlib import Path
import ctypes as c,json,time,os
import numpy as np
root=Path(__file__).resolve().parents[1];out=root/'outputs/portrait364-host'
lib=c.CDLL(str(out/os.environ.get('PORTRAIT_QA_LIBRARY','portrait.dll')));lib.ph_exhaustive_v2.argtypes=[c.c_char_p]
start=time.monotonic();failure=lib.ph_exhaustive_v2(str(out/'v2-combinations.bin').encode())
records=np.fromfile(out/'v2-combinations.bin',dtype=[('dna','<u4'),('h20','<u4'),('h32','<u4'),('h96','<u4')])
report=dict(combinations=len(records),renders=len(records)*3,structural_failures=failure,seconds=round(time.monotonic()-start,1))
(out/'v2-audit.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
raise SystemExit(bool(failure))
