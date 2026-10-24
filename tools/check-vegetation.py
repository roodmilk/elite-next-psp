"""Validate authored sprite ownership and the exact PSP compilation products."""
import json
import re
from pathlib import Path
from PIL import Image
from biome_prop_frames import load_props

root = Path(__file__).resolve().parents[1]
layout = json.loads((root / 'assets/source/lave-landscape/vegetation-frames.json').read_text())
source = Image.open(root / 'assets/source/lave-landscape/vegetation-atlas.png').convert('RGBA')
assert source.size == (layout['width'], layout['height'])
frames = [f['bounds'] for f in layout['frames']]
assert len(frames) == 8

# Each connected source subject must belong wholly to ONE frame. The former
# uniform columns split three tree canopies and one fern into adjacent cells.
alpha = source.getchannel('A')
for row, (top, bottom) in enumerate(((80, 640), (640, source.height))):
    remaining = {(x, y) for y in range(top, bottom) for x in range(source.width)
                 if alpha.getpixel((x, y)) > 160}
    subjects = []
    while remaining:
        seed = remaining.pop()
        todo, component = [seed], [seed]
        while todo:
            x, y = todo.pop()
            for dx, dy in ((-1,0),(1,0),(0,-1),(0,1),(-1,-1),(1,-1),(-1,1),(1,1)):
                p = (x+dx, y+dy)
                if p in remaining:
                    remaining.remove(p)
                    todo.append(p)
                    component.append(p)
        if len(component) > 1000:
            subjects.append(component)
    assert len(subjects) == 4, 'Source artwork changed: review individual frame ownership'
    owners = set()
    for subject in subjects:
        owned = [i for i in range(row*4, row*4+4) if all(
            frames[i][0] <= x < frames[i][2] and frames[i][1] <= y < frames[i][3]
            for x, y in subject)]
        assert len(owned) == 1, 'A complete plant crosses a frame boundary'
        owners.add(owned[0])
    assert len(owners) == 4, 'Multiple plants were assigned to the same sprite'
print('PASS all eight complete source plants belong to separate authored frames')

header = (root / 'src/generated/lave-landscape-pixels.h').read_text()
palette = [tuple(map(int, p)) for p in re.findall(r'RGB\((\d+),(\d+),(\d+)\)', header)]
pixels = list(map(int, re.findall(r'\d+', header.split('lave_vegetation_pixels[8][3072]={')[1])))
assert len(palette) == 32 and len(pixels) == 8*48*64
sheet = Image.open(root / 'assets/generated/lave-landscape/vegetation-atlas.png').convert('RGBA')
for cell, (left, top, right, bottom) in enumerate(frames):
    width, height = right-left, bottom-top
    scale = min(46/width, 63/height)
    dw, dh = round(width*scale), round(height*scale)
    ox, oy = (48-dw)//2, 64-dh
    for y in range(64):
        for x in range(48):
            actual = pixels[cell*3072+y*48+x]
            expected = 255
            if ox <= x < ox+dw and oy <= y < oy+dh:
                sx = left + min(width-1, int((x-ox+.5)*width/dw))
                sy = top + min(height-1, int((y-oy+.5)*height/dh))
                r, g, b, a = source.getpixel((sx, sy))
                if a > 160:
                    expected = min(range(32), key=lambda i:
                        2*(r-palette[i][0])**2 + 3*(g-palette[i][1])**2 + (b-palette[i][2])**2)
            assert actual == expected, (cell, x, y, 'stale or incorrectly cropped PSP sprite')
            preview = sheet.getpixel((cell%4*48+x, cell//4*64+y))
            assert (preview[3] == 0) if actual == 255 else preview == (*palette[actual], 255)
            if x in (0, 47):
                assert actual == 255, 'Missing transparent side gutter'
print('PASS all 24,576 PSP texels match their own source frame and native preview')
print('PASS transparent side gutters on all eight sprites')
cells = load_props(root)
header = (root / 'src/generated/biome-props-pixels.h').read_text()
palette = [tuple(map(int, p)) for p in re.findall(r'RGB\((\d+),(\d+),(\d+)\)', header)]
pixels = list(map(int, re.findall(r'\d+', header.split('biome_prop_pixels[24][3072]={')[1])))
assert len(palette) == 128 and len(pixels) == 24*48*64
preview = Image.open(root / 'assets/generated/biome-props/kit.png').convert('RGBA')
for i, cell in enumerate(cells):
    for y in range(64):
        for x in range(48):
            index = pixels[i*3072+y*48+x]
            rgba = cell.getpixel((x, y))
            assert (index != 255) == (rgba[3] > 160), (i, x, y, 'Foreign or missing silhouette pixel')
            shown = preview.getpixel((i%4*48+x, i//4*64+y))
            assert shown[3] == 0 if index == 255 else shown == (*palette[index], 255)
            if x in (0,47):
                assert index == 255, 'Biome sprite missing transparent side gutter'
print('PASS all 24 planetary props have distinct source ownership and exact native silhouettes')
print('PASS all 73,728 biome texels match the PSP preview, with transparent side gutters')
print('RESULT 0 failures')
