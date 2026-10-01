"""Arrange actual native PSP framebuffer captures, never concept art."""
from pathlib import Path
import sys
from PIL import Image, ImageDraw
folder=Path(sys.argv[1])
for path in folder.glob('world-*.bmp'):
    with Image.open(path) as im: im.save(path.with_suffix('.png'))
sheet=Image.new('RGB',(960,3*296),'#101820');draw=ImageDraw.Draw(sheet)
for body,name in [(2,'LAVE II / MOSSWOOD'),(3,'LAVE III / CLOUD SKYPORT'),(4,'LAVE IV / GLACIAL RANGE')]:
    for col,view in enumerate([8,6]):
        with Image.open(folder/f'world-{body}-view-{view}.png') as im: sheet.paste(im,(col*480,(body-2)*296+24))
        draw.text((col*480+8,(body-2)*296+6),name+' / NATIVE CAPTURE',fill='white')
    frames=[Image.open(p).convert('RGB') for p in sorted(folder.glob(f'world-{body}-motion-*.png'))]
    if frames: frames[0].save(folder/f'world-{body}-motion.gif',save_all=True,append_images=frames[1:],duration=167,loop=0)
sheet.save(folder/'lave-worlds.png')
