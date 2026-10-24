"""Bake manually traced 64px scalp contours, never colour-threshold polygons.

Coordinates refer to the exact compiled atlas tiles 0 and 76. Inclusive row
spans follow curls, forehead openings, sideburns and the visible ear cutout.
Source alpha clips the outer silhouette; black ink remains black at rendering.
"""
from pathlib import Path
import re
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
HUMAN = {y: [(0, 63)] for y in range(13)}
HUMAN.update({
    13: [(0, 38), (43, 63)], 14: [(0, 38), (44, 63)],
    15: [(0, 38), (48, 63)], 16: [(0, 37), (48, 63)],
    17: [(0, 37), (48, 63)], 18: [(0, 32), (35, 36), (48, 63)],
    19: [(0, 31), (49, 63)], 20: [(0, 31)], 21: [(0, 31)],
    22: [(0, 31)], 23: [(0, 30)], 24: [(0, 30)],
    25: [(0, 29)], 26: [(0, 29)], 27: [(0, 29)],
    28: [(0, 21), (26, 29)], 29: [(0, 20), (27, 29)],
    30: [(0, 19), (28, 29)], 31: [(0, 19), (28, 28)],
    **{y: [(0, 20)] for y in range(32, 39)},
    39: [(0, 25)], 40: [(0, 25)], 41: [(0, 24)],
    42: [(0, 23)], 43: [(0, 22)],
})
ANDROID = {y: [(0, 63)] for y in range(11)}
ANDROID.update({
    11: [(0, 30), (38, 63)], 12: [(0, 27), (44, 63)],
    13: [(0, 27), (47, 63)], 14: [(0, 27), (49, 63)],
    15: [(0, 27), (50, 63)], 16: [(0, 26)],
    17: [(0, 25)], 18: [(0, 24)], 19: [(0, 24)],
    20: [(0, 25)], 21: [(0, 24)], 22: [(0, 24)],
    23: [(0, 23)], 24: [(0, 15), (22, 23)],
    25: [(0, 15), (23, 23)], 26: [(0, 14), (23, 23)],
    27: [(0, 13), (23, 23)], 28: [(0, 13), (23, 23)],
})
rows = []
for spans in (HUMAN, ANDROID):
    rows.append([sum(1 << x for a, b in spans.get(y, []) for x in range(a, b + 1)) for y in range(64)])
lines = ['/* Generated from the reviewed per-row pixel contours in tools/bake-portrait-hair-masks.py. */',
         'static const uint64_t portrait_hair_rows[2][64]={']
for row in rows:
    lines += [' {'] + ['  ' + ','.join(f'0x{v:016x}ULL' for v in row[i:i+4]) + ',' for i in range(0, 64, 4)] + [' },']
lines += ['};', '']
(ROOT / 'src/portrait-hair-masks.h').write_text('\n'.join(lines), encoding='ascii')

# Diagnostic source and material layers: these are not runtime portrait assets.
values = [int(x, 16) for x in re.findall(r'0x([0-9A-Fa-f]{4})', (ROOT / 'src/portrait-atlas.h').read_text())]
out = ROOT / 'outputs/hair363'
out.mkdir(parents=True, exist_ok=True)
for rig, tile in enumerate((0, 76)):
    data = values[tile*4096:(tile+1)*4096]
    for choice, target in [('brown', (105,57,31)), ('blonde', (246,213,115)), ('blue', (65,133,245))]:
        pixels = []
        for at, value in enumerate(data):
            x, y = at % 64, at // 64
            rgb = (((value>>10)&31)*255//31, ((value>>5)&31)*255//31, (value&31)*255//31)
            if not value & 32768:
                rgb = (15,24,34)
            elif rows[rig][y] >> x & 1:
                light = (rgb[0]*3+rgb[1]*5+rgb[2]*2)//10
                if light >= 10:
                    light = min(255,144+light*3//5)
                    rgb = tuple(c*light//255 for c in target)
            pixels.append(rgb)
        im = Image.new('RGB', (64,64)); im.putdata(pixels)
        im.resize((384,384), Image.Resampling.NEAREST).save(out/f'traced-{tile}-{choice}.png')
print('Baked two 64px scalp masks and source-pixel diagnostics')
