"""Convert actual native framebuffer captures; never synthesize gameplay proof."""
from pathlib import Path
import sys
from PIL import Image
folder = Path(sys.argv[1])
frames = []
for path in sorted(folder.glob('animal-walk-*.bmp')):
    with Image.open(path) as im:
        rgb = im.convert('RGB')
        rgb.save(path.with_suffix('.png'))
        frames.append(rgb.copy())
if frames:
    frames[0].save(folder / 'animal-walk.gif', save_all=True,
                   append_images=frames[1:], duration=50, loop=0)
with Image.open(folder / 'animal-feeding.bmp') as im:
    im.save(folder / 'animal-feeding.png')
print(folder / 'animal-walk.gif')
