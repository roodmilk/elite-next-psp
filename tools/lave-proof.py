"""Package unaltered native framebuffer captures for review."""
from pathlib import Path
import sys
from PIL import Image, ImageDraw

folder = Path(sys.argv[1])
for path in folder.glob('*.bmp'):
    with Image.open(path) as image:
        image.save(path.with_suffix('.png'))
views = sorted(folder.glob('lave-view-*.png'))
sheet = Image.new('RGB', (960, ((len(views) + 1) // 2) * 296), '#101820')
draw = ImageDraw.Draw(sheet)
for i, path in enumerate(views):
    with Image.open(path) as image:
        x, y = (i % 2) * 480, (i // 2) * 296
        sheet.paste(image, (x, y + 24))
        draw.text((x + 8, y + 6), path.stem, fill='white')
sheet.save(folder / 'views.png')
frames = [Image.open(p).convert('RGB').resize((960,544), Image.Resampling.NEAREST) for p in sorted(folder.glob('lave-motion-*.png'))]
if frames:
    frames[0].save(folder / 'walking.gif', save_all=True, append_images=frames[1:], duration=100, loop=0)
print(folder / 'views.png')
