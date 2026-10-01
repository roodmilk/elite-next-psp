"""Export native emulator framebuffers for visual QA; no invented UI pixels."""
from pathlib import Path
from PIL import Image, ImageDraw
import sys
root=Path(__file__).resolve().parents[1]
run=Path(sys.argv[1])
out=root/'proof'
out.mkdir(exist_ok=True)
for channel in (8,12,19):
    Image.open(run/f'tv-channel-{channel}.bmp').save(out/f'channel-{channel}.png')
frames=[Image.open(p).convert('RGB') for p in sorted(run.glob('tv-motion-*.bmp'))]
frames[0].save(out/'local-tv-animation.gif',save_all=True,append_images=frames[1:],duration=160,loop=0)
source=Image.open(root/'assets/preview/local-tv/reference-native.png').convert('RGB')
actual=Image.open(out/'channel-8.png').convert('RGB')
sheet=Image.new('RGB',(984,316),(6,12,22))
d=ImageDraw.Draw(sheet)
d.text((8,8),'APPROVED REFERENCE AT PSP RESOLUTION',fill='white')
d.text((496,8),'ACTUAL PSP FRAMEBUFFER / LIVE UI',fill='white')
sheet.paste(source,(8,32));sheet.paste(actual,(496,32))
sheet.save(out/'reference-vs-game.png')
Image.open(run/'tv-discover-menu.bmp').save(out/'discover-menu.png')
print(out)
