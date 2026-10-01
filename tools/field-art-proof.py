"""Arrange unmodified native framebuffer captures, not concept images."""
from pathlib import Path
import sys
from PIL import Image, ImageDraw

folder = Path(sys.argv[1])
for path in folder.glob('*.bmp'):
    with Image.open(path) as image:
        image.save(path.with_suffix('.png'))
names = ['SUPPLY CACHE', 'RUINS', 'OBSERVATORY', 'RESCUE CAMP', 'SEED GARDEN',
         'FOSSIL EXCAVATION', 'THERMAL STACKS', 'WRECK', 'MIGRATION LOOKOUT',
         'CRYSTALS', 'ARCHIVE', 'WEATHER ARRAY']
sheet = Image.new('RGB', (960, 6 * 296), '#101820')
draw = ImageDraw.Draw(sheet)
for i, name in enumerate(names):
    with Image.open(folder / f'poi-{i:02}-1.png') as image:
        x, y = (i % 2) * 480, (i // 2) * 296
        sheet.paste(image, (x, y + 24))
        draw.text((x + 8, y + 6), name + ' / NATIVE GAME CAPTURE', fill='white')
sheet.save(folder / 'poi-gallery.png')
sheet = Image.new('RGB', (960, 272), '#101820')
for i, name in enumerate(['species-flora', 'species-fauna']):
    with Image.open(folder / f'{name}.png') as image:
        sheet.paste(image, (i * 480, 0))
sheet.save(folder / 'wildlife-gallery.png')
frames = [Image.open(p).convert('RGB').resize((960, 544), Image.Resampling.NEAREST)
          for p in sorted(folder.glob('tree-leaves-*.png'))]
if frames:
    frames[0].save(folder / 'tree-leaves.gif', save_all=True, append_images=frames[1:],
                   duration=250, loop=0)
print(folder / 'poi-gallery.png')
