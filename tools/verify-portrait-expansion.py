"""Exercise the production C renderer; inspect foreground, never background hashes.
The PNGs use the actual game font and actual C portraits at native sizes.
"""
from pathlib import Path
import ctypes as c, json, re, sys, os
import numpy as np
from PIL import Image

root=Path(__file__).resolve().parents[1];out=root/os.environ.get('PORTRAIT_QA_OUTPUT','outputs/portrait376-host')
out.mkdir(parents=True,exist_ok=True)
lib=c.CDLL(str(out/os.environ.get('PORTRAIT_QA_LIBRARY','portrait376.dll')))
for n in ('ph_dna','ph_modern','ph_expand','ph_random','ph_adjust'):
    getattr(lib,n).restype=c.c_uint
lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int]
lib.ph_expand.argtypes=lib.ph_random.argtypes=[c.c_uint]
lib.ph_count.argtypes=lib.ph_value.argtypes=[c.c_uint,c.c_int]
lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
class Model(c.Structure):
    _fields_=[('ink',c.c_ubyte*4096),('owner',c.c_ubyte*4096),('pixels',c.c_uint16*4096)]
lib.ph_model.argtypes=[c.c_uint,c.POINTER(Model)]
def select(d,p,v):
    for _ in range(16):
        if lib.ph_value(d,p)==v:return d
        d=lib.ph_adjust(d,p,1)
    raise AssertionError((hex(d),p,v))
def render(d,s):
    a=(c.c_uint*(s*s))();lib.ph_render(d,s,a)
    return np.ctypeslib.as_array(a).copy().reshape(s,s)
def picture(d,s):
    a=render(d,s);return Image.fromarray(np.stack([a&255,(a>>8)&255,(a>>16)&255],axis=-1).astype('uint8'))
def model(d):
    m=Model();lib.ph_model(d,c.byref(m));return m
glyphs=[int(v,16) for v in re.findall(r'0x([\dA-Fa-f]+)u',(root/'src/font8.h').read_text())]
def text(im,x,y,s,colour=(225,225,207)):
    for letter in s.upper():
        if 32<=ord(letter)<=126:
            for yy,bits in enumerate(glyphs[(ord(letter)-32)*8:(ord(letter)-31)*8]):
                for xx in range(8):
                    if bits&(128>>xx) and 0<=x+xx<im.width and 0<=y+yy<im.height:im.putpixel((x+xx,y+yy),colour)
        x+=8
def base(sp=0,gender=0):
    d=select(lib.ph_modern(sp),11,gender) if sp in (0,7) else lib.ph_modern(sp)
    return select(select(d,3,1 if lib.ph_count(d,3)>1 else 0),5,1)
for sp in (0,7):
    for gender in range(2):
        im=Image.new('RGB',(480,272),(13,23,34))
        text(im,8,8,('HUMAN' if sp==0 else 'ANDROID')+' / '+('FEMALE' if gender else 'MALE')+' HAIR STYLES',(234,185,89))
        for style,name in enumerate(('CLASSIC','CROPPED','HIGH SWEEP','SIDE PART')):
            x=8+style*118;d=select(base(sp,gender),10,style)
            im.paste(picture(d,96),(x,32));im.paste(picture(d,20),(x,138));im.paste(picture(d,32),(x+28,134));text(im,x,174,name)
            d=select(d,3,3);im.paste(picture(d,64),(x,194));d=select(d,3,5);im.paste(picture(d,32),(x+70,210))
        im.save(out/f'styles-{sp}-{gender}.png')
        im=Image.new('RGB',(480,272),(13,23,34));text(im,8,8,'EYE STYLES / '+('FEMALE' if gender else 'MALE'),(234,185,89))
        for v,name in enumerate(('NATURAL','NARROW','LUMINOUS','WIDE','ANGLED')):
            x=8+(v%3)*156;y=30+(v//3)*118;d=select(base(sp,gender),4,v)
            im.paste(picture(d,96),(x,y));im.paste(picture(d,20),(x+100,y));im.paste(picture(d,32),(x+100,y+28));text(im,x,y+100,name)
        im.save(out/f'eyes-{sp}-{gender}.png')

failures=[];checks=0;cases=0
def check(ok,detail):
    global checks
    checks+=1
    if not ok:failures.append(detail)
for sp in range(10):
    for gender in range(2 if sp in (0,7) else 1):
        for context in range(2):
            d=base(sp,gender)
            for p in (1,2,3,4,5,6,8,9,10):d=select(d,p,context*(lib.ph_count(d,p)-1))
            for p in range(1,12):
                count=lib.ph_count(d,p)
                if count<2:continue
                ids=[select(d,p,v) for v in range(count)];models=[model(i) for i in ids]
                for i in ids:
                    check(lib.ph_valid(i) and all(lib.ph_value(i,q)==lib.ph_value(d,q) for q in range(12) if q!=p),dict(test='field preservation',sp=sp,gender=gender,part=p,dna=hex(i)))
                for size in (20,32,96):
                    images=[render(i,size)[1:-1,1:-1] for i in ids];a=np.arange(1,size-1)*64//size;indices=a[:,None]*64+a[None,:]
                    cases+=count
                    for v in range(count):
                        for prior in range(v):
                            ia=np.ctypeslib.as_array(models[v].ink)[indices];ib=np.ctypeslib.as_array(models[prior].ink)[indices]
                            oa=np.ctypeslib.as_array(models[v].owner)[indices];ob=np.ctypeslib.as_array(models[prior].owner)[indices]
                            changed=images[v]!=images[prior];fg=(ia!=0)|(ib!=0);allowed=(oa==p+1)|(ob==p+1)
                            if p==1:allowed=(ia!=12)&(ib!=12)
                            if p==2:allowed=(ia==5)|(ib==5)
                            if p in (3,10):allowed=(ia==8)|(ib==8)
                            if p==5:allowed=(ia==12)|(ib==12)
                            if p==11:allowed=np.ones_like(changed)
                            n=int(np.count_nonzero(changed&fg));bad=int(np.count_nonzero(changed&~allowed));bg=int(np.count_nonzero(changed&~fg))
                            check(n>=2 and bad==0 and bg==0,dict(test='foreground/ownership',sp=sp,gender=gender,context=context,part=p,size=size,choices=[prior,v],foreground=n,unrelated=bad,background=bg))
# Pixel-identical conversion of both existing parts versions, including inactive fields.
for sp in range(10):
    for seed in range(30):
        d=lib.ph_dna(sp)
        for p in range(1,10):d=select(d,p,(seed*(p+3))%lib.ph_count(d,p))
        new=lib.ph_expand(d)
        check(all(np.array_equal(render(d,s),render(new,s)) for s in (20,32,96)),dict(test='migration pixels',sp=sp,seed=seed))
# Every hair/style/gender/skin pair, six eyes and maximum local parts in mixed sets.
for sp in (0,7):
    for gender in range(2):
        for style in range(4):
            for skin in range(12):
                for hair in range(12):
                    d=select(select(select(base(sp,gender),10,style),2,skin),3,hair)
                    for p in (1,4,5,6,8,9):d=select(d,p,(skin+hair+style+gender+p)%lib.ph_count(d,p))
                    check(lib.ph_valid(d),dict(test='mixed validity',dna=hex(d)))
                    m=model(d);check(all(v<18 for v in m.ink) and all(v<=12 for v in m.owner) and all(not m.ink[a] or m.pixels[a]&32768 for a in range(4096)),dict(test='material integrity',dna=hex(d)))
                    for s in (20,32,96):render(d,s);cases+=1
for seed in range(1000):
    d=lib.ph_random(seed);check(lib.ph_valid(d) and all(lib.ph_value(d,p)<lib.ph_count(d,p) for p in range(12)),dict(test='new look',seed=seed))
# Independent landmarks traced against the original female atlas, rather than
# accepting whatever anatomy the implementation itself claims to have drawn.
for sp,neck,face,hair in [(0,[(37,51),(38,53),(39,49)],[(34,19),(39,35),(47,35)],[(31,4),(24,12),(45,20)]),
                          (7,[(39,47),(40,49),(41,51)],[(34,18),(38,34),(48,32)],[(32,4),(23,10),(48,15)])]:
    d=base(sp,1);m=model(d)
    for x,y in neck+face:check(m.ink[y*64+x]==5,dict(test='independent female skin/neck',sp=sp,x=x,y=y,material=m.ink[y*64+x]))
    for x,y in hair:check(m.ink[y*64+x]==8,dict(test='independent female scalp',sp=sp,x=x,y=y,material=m.ink[y*64+x]))
    for style in range(4):
        d=select(base(sp,1),10,style);original=render(d,64)
        for colour in range(12):
            im=render(select(d,3,colour),64)
            check(all(im[y,x]==original[y,x] for x,y in neck+face),dict(test='hair leaves actual female skin unchanged',sp=sp,style=style,colour=colour))
        for outfit in range(6):
            im=render(select(d,5,outfit),64)
            check(all(im[y,x]==original[y,x] for x,y in neck),dict(test='female neck never takes outfit colour',sp=sp,style=style,outfit=outfit))
check(lib.ph_guards(),dict(test='framebuffer guards'))
report=dict(checks=checks,render_cases=cases,failures=failures)
(out/'audit.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
print(json.dumps(dict(checks=checks,render_cases=cases,failures=len(failures),examples=failures[:40]),indent=2))
sys.exit(bool(failures))
