"""Native framebuffer proof. No concept images or synthetic scenery."""
from pathlib import Path
from PIL import Image, ImageDraw
import sys
p=Path(sys.argv[1])
for f in p.glob('*.bmp'):
    with Image.open(f) as im: im.save(f.with_suffix('.png'))
for phase,title in [(1,'arrival'),(2,'touchdown'),(3,'departure')]:
    frames=[Image.open(f).convert('RGB') for f in sorted(p.glob(f'pilot-1-phase-{phase}-*.png'))]
    if frames: frames[0].save(p/f'{title}.gif',save_all=True,append_images=frames[1:],duration=333,loop=0)
items=[('Approach / actual Lave I terrain','pilot-1-phase-1-090.png'),('Spaceport / braking','pilot-1-phase-1-210.png'),('Parked / live first-person view','pilot-1-parked.png'),('Departure / forward reveal','pilot-1-phase-3-100.png'),('Departure / climb','pilot-1-phase-3-170.png'),('Roamer / stepping into cabin','roamer-seat-04.png')]
sheet=Image.new('RGB',(960,3*296),'#101820');draw=ImageDraw.Draw(sheet)
for i,(title,file) in enumerate(items):
    x=(i%2)*480;y=(i//2)*296
    draw.text((x+8,y+6),title,fill='white')
    with Image.open(p/file) as im: sheet.paste(im,(x,y+24))
sheet.save(p/'cinematic-port.png')
