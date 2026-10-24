"""Bake approved native 164x100 Space Secrets artwork into bounded indexed sprites."""
from pathlib import Path
import hashlib
from PIL import Image
root=Path(__file__).resolve().parents[1]
out=['/* Native 164x100 approved Space Secrets mockup artwork. No scaling. */']
for name in ('rocks','wreck','whales','beacon'):
    path=root/'assets/source/rumours'/f'art-{name}-164x100.png'
    im=Image.open(path).convert('RGB');assert im.size==(164,100)
    q=im.quantize(colors=256,dither=Image.Dither.NONE)
    palette=q.getpalette();palette+=([0]*(768-len(palette)));pixels=q.tobytes()
    out.append(f'/* {path.name}: {hashlib.sha256(path.read_bytes()).hexdigest()} */')
    out.append(f'static const unsigned rumour_{name}_pal[256]={{'+','.join(f'0xff{palette[i+2]:02x}{palette[i+1]:02x}{palette[i]:02x}u' for i in range(0,768,3))+'};')
    out.append(f'static const unsigned char rumour_{name}_pixels[16400]={{')
    out.extend(','.join(str(v) for v in pixels[i:i+164])+',' for i in range(0,len(pixels),164))
    out.append('};')
(root/'src/generated/rumours-pixels.h').write_text('\n'.join(out)+'\n',encoding='utf-8')
print('Four native sprites baked; 65,600 indexed pixels plus 4,096 palette bytes.')
