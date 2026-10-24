"""Embed selected authored pixel backgrounds and portraits; no runtime PNG decoder."""
from pathlib import Path
from PIL import Image
import shutil

ROOT=Path(__file__).resolve().parents[1]
SOURCE=Path('C:/Users/skarm/.codex/generated_images/01a100f9-ff86-7d11-bcc4-0efe02a64d4e')
ART={
 'bedroom':'exec-31f86bdc-d3d4-4d36-b756-30a62dbf7e5f.png',
 'city':'exec-3aa82600-40bc-45ec-9a1e-8cb472cc1635.png',
 'dealer':'exec-680a19b7-9e2e-41ba-a865-8a298e6418bd.png',
 'field':'exec-5f6a4ce5-dcdc-4215-a767-4565cfb72726.png',
 'cockpit':'exec-d888bd82-cd8a-4f6d-a240-982ec3ed20f3.png',
 'orbit':'exec-77a0b63b-9842-4935-b509-e8e0843152dc.png',
 'portraits':'exec-580aafa1-c9a6-4180-820e-c52d948993c9.png',
 'ships':'exec-bf6e57a8-bf58-4707-bdf6-97e0d5c2d23f.png',
 'party':'exec-53e0b30b-7969-4ab4-8908-ad0d43e269ab.png',
}
out=ROOT/'assets'/'story';out.mkdir(parents=True,exist_ok=True)
chunks=['/* Generated from assets/story by tools/compile-opening-art.py. */',
        '#ifndef ELITE_OPENING_BITMAPS_H','#define ELITE_OPENING_BITMAPS_H',
        'typedef struct {int w,h;const unsigned *palette;const unsigned char *pixels;} OpeningBitmap;']
def embed(name,img,size,transparent=False):
    rgba=img.convert('RGBA').resize(size,Image.Resampling.BOX)
    img=rgba.convert('RGB').quantize(colors=255 if transparent else 256,dither=Image.Dither.NONE)
    pal=img.getpalette();colors=[0] if transparent else []
    for i in range(255 if transparent else 256):
        r,g,b=pal[i*3:i*3+3];colors.append(0xff000000|b<<16|g<<8|r)
    data=img.tobytes()
    if transparent:
        alpha=rgba.getchannel('A').tobytes()
        data=bytes(index+1 if a>=128 else 0 for index,a in zip(data,alpha))
        preview=Image.new('P',size);preview.putpalette([v for color in colors for v in (color&255,color>>8&255,color>>16&255)])
        preview.frombytes(data);preview.info['transparency']=0;preview.save(out/(name+'-native.png'))
    else:img.save(out/(name+'-native.png'))
    chunks.append('static const unsigned opening_'+name+'_palette[256]={'+','.join(hex(v)+'u' for v in colors)+'};')
    rows=[','.join(str(b) for b in data[n:n+64]) for n in range(0,len(data),64)]
    chunks.append('static const unsigned char opening_'+name+'_pixels['+str(len(data))+']={\n'+',\n'.join(rows)+'\n};')
    chunks.append('static const OpeningBitmap opening_'+name+'_bitmap={'+str(size[0])+','+str(size[1])+',opening_'+name+'_palette,opening_'+name+'_pixels};')
for name,file in ART.items():
    dst=out/(name+'.png')
    if not dst.exists():shutil.copyfile(SOURCE/file,dst)
    img=Image.open(dst)
    if name=='ships':
        for i in range(3):
            w,h=img.size;cell=img.crop((i*w//3,0,(i+1)*w//3,h)).convert('RGBA')
            box=cell.getchannel('A').point(lambda a:255 if a>=128 else 0).getbbox()
            cell=cell.crop(box)
            scale=min(220/cell.width,126/cell.height)
            scaled=cell.resize((round(cell.width*scale),round(cell.height*scale)),Image.Resampling.BOX)
            canvas=Image.new('RGBA',(220,126));canvas.alpha_composite(scaled,((220-scaled.width)//2,126-scaled.height))
            embed('ship'+str(i),canvas,(220,126),True)
    elif name=='party':embed(name,img,(128,72))
    elif name=='portraits':
        for i in range(8):
            x=i%4;y=i//4;w,h=img.size
            embed('portrait'+str(i),img.crop((x*w//4,y*h//2,(x+1)*w//4,(y+1)*h//2)),(64,64))
    else:embed(name,img,(480,272))
chunks+=['#endif','']
(ROOT/'src'/'opening-bitmaps.h').write_text('\n'.join(chunks),encoding='ascii')
print('Embedded six scenes, eight portraits, three transparent ship sprites and a party photograph.')
