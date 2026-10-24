from pathlib import Path
import ctypes as c
from PIL import Image
root=Path(__file__).resolve().parents[1]
lib=c.CDLL(str(root/'outputs/portrait379-host/portrait379-pre.dll'))
for n in ('ph_modern','ph_adjust'):getattr(lib,n).restype=c.c_uint
lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int]
lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
class Model(c.Structure):
    _fields_=[('ink',c.c_ubyte*4096),('owner',c.c_ubyte*4096),('pixels',c.c_uint16*4096)]
lib.ph_model.argtypes=[c.c_uint,c.POINTER(Model)]
for sp in (0,7):
    d=lib.ph_adjust(lib.ph_modern(sp),11,1);m=Model();lib.ph_model(d,c.byref(m))
    a=(c.c_uint*4096)();lib.ph_render(d,64,a)
    im=Image.new('RGB',(64,64));im.putdata([(v&255,(v>>8)&255,(v>>16)&255) for v in a])
    im.resize((384,384),Image.Resampling.NEAREST).save(root/f'outputs/profile379-audit/female-rig-{sp}.png')
    print('SPECIES',sp)
    for y in range(4,48):print(f'{y:02}', ''.join({0:'.',5:'s',8:'H',12:'u',15:'e',16:'L'}.get(m.ink[y*64+x],'?') for x in range(12,56)))
