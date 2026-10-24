"""Check GalacticNet Market captures, allowing only JOBS -> RUMOURS glyphs.

Usage: python tools/check-rumours-isolation.py outputs/newspapers-isolation-...
Capture pairs are produced by the existing review-news-isolation.ps1 tool.
Its strict whole-image hashes intentionally differ when a shared tab is renamed.
"""
from pathlib import Path
import sys
from PIL import Image, ImageChops

root = Path(sys.argv[1])
names = [f"market-neon-{i}.bmp" for i in range(4)] + ["market-offline.bmp"]
failures = 0
for name in names:
    before = Image.open(root / "baseline" / name).convert("RGB")
    after = Image.open(root / "candidate" / name).convert("RGB")
    assert before.size == after.size == (480, 272)
    diff = ImageChops.difference(before, after)
    bounds = diff.getbbox()
    # Exact native glyph area of the sixth tab; not the entire header or body.
    diff.paste((0, 0, 0), (384, 24, 440, 32))
    ok = diff.getbbox() is None
    failures += not ok
    print(f"{'PASS' if ok else 'FAIL'} {name}: only approved tab glyphs may differ; actual bounds {bounds}")
print(f"RESULT {failures} failures")
raise SystemExit(bool(failures))
