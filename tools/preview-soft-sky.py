"""Assemble unretouched PSP framebuffer captures for visual review."""
from pathlib import Path
import sys
from PIL import Image, ImageDraw

root = Path(sys.argv[1])
systems = [7, 31, 63, 127, 173, 255]
sheet = Image.new('RGB', (960, 3 * 296), '#080b12')
draw = ImageDraw.Draw(sheet)
for i, system in enumerate(systems):
    source = root / f'system-{system}' / f'soft-sky-{system:03d}-view-1.png'
    image = Image.open(source).convert('RGB')
    x, y = (i % 2) * 480, (i // 2) * 296
    draw.text((x + 8, y + 6), f'System {system} - native PSP capture', fill='#c5d3de')
    sheet.paste(image, (x, y + 24))
sheet.save(root / 'soft-sky-gallery.png')
frames = [Image.open(path).convert('RGB') for path in sorted((root / 'system-7').glob('soft-sky-motion-*.png'))]
if frames:
    palette_source = Image.new('RGB', (480, 272 * len(frames)))
    for i, frame in enumerate(frames):
        palette_source.paste(frame, (0, i * 272))
    palette = palette_source.quantize(colors=256)
    indexed = [frame.quantize(palette=palette, dither=Image.Dither.NONE) for frame in frames]
    loop = indexed + indexed[-2:0:-1]
    loop[0].save(root / 'soft-sky-pitch-sweep.gif', save_all=True, append_images=loop[1:], duration=40, loop=0, optimize=False)
print(root / 'soft-sky-gallery.png')
print(root / 'soft-sky-pitch-sweep.gif')
