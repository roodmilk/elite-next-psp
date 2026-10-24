"""Independent source pixels and before/after production C renders, 480x272."""
from pathlib import Path
import ctypes as c, json, re, sys
import numpy as np
from PIL import Image
root=Path(__file__).resolve().parents[1];out=root/'outputs/profile379-audit'
new=c.CDLL(str(root/'outputs/portrait379-host/portrait379.dll'))
old=c.CDLL(str(root/'outputs/portrait376-host/portrait376.dll'))
for lib in (old,new):
    for name in ('ph_modern','ph_adjust'):getattr(lib,name).restype=c.c_uint
    lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int]
    lib.ph_value.argtypes=[c.c_uint,c.c_int]
    lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
new.ph_legacy_dna.restype=c.c_uint
class Model(c.Structure):
    _fields_=[('ink',c.c_ubyte*4096),('owner',c.c_ubyte*4096),('pixels',c.c_uint16*4096)]
new.ph_model.argtypes=[c.c_uint,c.POINTER(Model)]
def select(d,p,value):
    for _ in range(16):
        if new.ph_value(d,p)==value:return d
        d=new.ph_adjust(d,p,1)
    raise AssertionError('unreachable feature')
def render(lib,d,s):
    a=(c.c_uint*(s*s))();lib.ph_render(d,s,a)
    return np.ctypeslib.as_array(a).copy().reshape(s,s)
def image(a):return Image.fromarray(np.stack([a&255,(a>>8)&255,(a>>16)&255],axis=-1).astype('uint8'))
glyphs=[int(v,16) for v in re.findall(r'0x([\dA-Fa-f]+)u',(root/'src/font8.h').read_text())]
def text(im,x,y,s,colour=(225,225,207)):
    for letter in s.upper():
        for yy,bits in enumerate(glyphs[(ord(letter)-32)*8:(ord(letter)-31)*8]):
            for xx in range(8):
                if bits&(128>>xx):im.putpixel((x+xx,y+yy),colour)
        x+=8
def comparison(d,label,file):
    im=Image.new('RGB',(480,272),(13,23,34));text(im,8,8,label,(234,185,89))
    for x,lib,title in ((24,old,'BEFORE / 2.5.378'),(264,new,'AFTER / 2.5.379')):
        text(im,x,30,title);im.paste(image(render(lib,d,96)),(x,48))
        im.paste(image(render(lib,d,20)),(x+112,48));im.paste(image(render(lib,d,32)),(x+112,80))
    text(im,24,160,'96PX EDITOR / 20PX + 32PX DIALOGUE',(117,169,178))
    text(im,24,192,'SAME SAVED DNA. CORRECTED ART.')
    im.save(out/file)
failures=[];checks=0
def check(ok,message):
    global checks
    checks+=1
    if not ok:failures.append(message)
# These gutter rows were reviewed against the original source images.
sheets=[('pulp-humans-16.png',(0,292,551,809,1086)),
 ('pulp-orrith-amphibian-16.png',(0,326,615,849,1086)),
 ('pulp-insect-sauran-16.png',(0,310,571,813,1086)),
 ('pulp-avian-masked-16.png',(0,323,592,843,1086)),
 ('pulp-robots-12.png',(0,375,725,1086)),
 ('pulp-androids-12.png',(0,369,727,1086)),
 ('pulp-crystal-fungal-8.png',(0,451,887))]
packed_text=(root/'src/portrait-legacy-atlas.h').read_text()
packed=[int(v,16) for v in re.findall(r'0x([0-9A-Fa-f]{4})',packed_text)]
offsets=[int(v) for v in re.search(r'portrait_legacy_offsets\[97\]=\{([^}]+)',packed_text)[1].split(',')]
encoded=np.zeros((96,64,64),dtype='uint16')
for face in range(96):
    at=offsets[face]
    for y in range(64):
        start,count=packed[at:at+2];at+=2
        assert 0<=start<=64 and 0<=count<=64-start
        encoded[face,y,start:start+count]=packed[at:at+count];at+=count
    assert at==offsets[face+1]
original=np.array([int(v,16) for v in re.findall(r'0x([0-9A-Fa-f]{4})',(root/'src/portrait-atlas.h').read_text())],dtype='uint16').reshape(96,64,64)
rigs=np.array([int(v,16) for v in re.findall(r'0x([0-9A-Fa-f]{4})',(root/'src/portrait-rig-atlas.h').read_text())],dtype='uint16').reshape(12,64,64)
for i,source_index in enumerate((0,16,24,32,40,48,64,76,56,88,1,77)):
    check(np.array_equal(rigs[i],original[source_index]),f'compact rig {i}: original alignment and pixels unchanged')
base=0;old_contaminated=0;old_colour_failures=0
for name,rows in sheets:
    source=Image.open(root/'assets/source/portraits'/name).convert('RGBA')
    for row in range(len(rows)-1):
        for col in range(4):
            cell=source.crop((col*source.width//4,rows[row],(col+1)*source.width//4,rows[row+1]))
            expected=np.zeros((64,64),dtype='uint16');rgba=np.array(cell.resize((60,60),Image.Resampling.NEAREST),dtype='uint16')
            expected[2:62,2:62]=np.where(rgba[:,:,3]>=64,32768|((rgba[:,:,0]>>3)<<10)|((rgba[:,:,1]>>3)<<5)|(rgba[:,:,2]>>3),0)
            check(np.array_equal(encoded[base],expected),f'face {base}: exact reviewed source crop')
            check(not expected[:2].any(),f'face {base}: clean top gutter')
            d=new.ph_legacy_dna(base);check(bool(d),f'face {base}: saved identity reachable')
            for s in (20,32,80,96):
                a=np.arange(s)*64//s;x,y=np.meshgrid(a,a);p=expected[y,x]
                valid=(p&32768)!=0;valid[0,:]=valid[-1,:]=valid[:,0]=valid[:,-1]=False
                xx,yy=np.meshgrid(np.arange(s),np.arange(s))
                valid&=~((xx>=50*s//64)&(xx<58*s//64)&(yy>=52*s//64)&(yy<60*s//64))
                rgb=((p.astype('uint32')>>10)&31)*255//31|((((p.astype('uint32')>>5)&31)*255//31)<<8)|(((p.astype('uint32')&31)*255//31)<<16)
                actual=render(new,d,s);before=render(old,d,s)
                check(np.array_equal(actual[valid],rgb[valid]),f'face {base} at {s}px: exact source colours')
                old_colour_failures+=int(np.count_nonzero(before[valid]!=rgb[valid]))
            base+=1
check(base==96 and old_colour_failures>1000,'negative control: shipped legacy renderer fails corrected source-pixel comparisons')
for sp in (0,7):
    d=select(select(select(new.ph_modern(sp),11,1),3,1),5,1)
    comparison(select(d,10,1),f'AUTHORED CROP / {"HUMAN" if sp==0 else "ANDROID"}',f'crop-before-after-{sp}.png')
    comparison(select(d,8,2),f'AUTHORED SMIRK / {"HUMAN" if sp==0 else "ANDROID"}',f'smirk-before-after-{sp}.png')
    for ear in (1,2):
        cropped=select(select(select(d,10,1),3,5),9,ear)
        comparison(cropped,f'AUTHORED EARS / {"HUMAN" if sp==0 else "ANDROID"}',f'ears-before-after-{sp}-{ear}.png')
        m=Model();new.ph_model(cropped,c.byref(m))
        check(not any(v==8 for v in m.ink[30*64:]),f'female {sp} ear {ear}: cropped hairstyle never restores bob hair through ear transform')
    for style in (0,1):
        before=render(new,select(d,10,style),64)
        for skin in range(12):
            for hair in range(12):
                dna=select(select(select(select(d,10,style),2,skin),3,hair),8,2)
                check(bool(new.ph_valid(dna)),f'mixed female part validity: {sp}/{style}/{skin}/{hair}')
comparison(new.ph_legacy_dna(4),'LEGACY PHOTO / ROW-BOUNDARY REPAIR','profile-before-after.png')
report=dict(checks=checks,legacy_faces=base,source_sizes=[20,32,80,96],portrait_asset_bytes=len(packed)*2+388+8192+98304,old_mismatched_pixels=old_colour_failures,failures=failures)
(out/'source-checks.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
print(json.dumps(report,indent=2));sys.exit(bool(failures))
