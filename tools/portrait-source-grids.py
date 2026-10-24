from pathlib import Path
from PIL import Image, ImageDraw
import re
root=Path(__file__).resolve().parents[1]
out=root/'outputs/portrait364-host';out.mkdir(exist_ok=True)
v=[int(x,16) for x in re.findall(r'0x([0-9a-fA-F]{4})',(root/'src/portrait-atlas.h').read_text())]
for sp,tile in enumerate((0,16,24,32,40,48,64,76,56,88)):
 im=Image.new('RGB',(64,64));im.putdata([(((c>>10)&31)*255//31,((c>>5)&31)*255//31,(c&31)*255//31) if c&32768 else (15,24,34) for c in v[tile*4096:(tile+1)*4096]])
 im.save(out/f'source-{sp}.png')
 grid=Image.new('RGB',(600,600),'white');grid.paste(im.resize((576,576),Image.Resampling.NEAREST),(24,24));draw=ImageDraw.Draw(grid)
 for n in range(0,64,2):
  draw.text((24+n*9,3),str(n),fill='black');draw.text((1,24+n*9),str(n),fill='black')
 for n in range(65):
  draw.line((24+n*9,24,24+n*9,600),fill=(48,48,48));draw.line((24,24+n*9,600,24+n*9),fill=(48,48,48))
 grid.save(out/f'grid-{sp}.png')
print('10 exact source tile grids')
