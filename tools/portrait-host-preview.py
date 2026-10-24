"""Actual production C renders, not a Python reimplementation."""
from pathlib import Path
import ctypes as c,os
from PIL import Image,ImageDraw
root=Path(__file__).resolve().parents[1];out=root/'outputs/portrait364-host'
lib=c.CDLL(str(out/os.environ.get('PORTRAIT_QA_LIBRARY','portrait.dll')))
for name in ('ph_dna','ph_adjust'):
 getattr(lib,name).restype=c.c_uint
lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int]
lib.ph_count.argtypes=[c.c_uint,c.c_int]
lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
def dna(sp,choices={}):
 d=lib.ph_dna(sp)
 for p,v in choices.items():
  for n in range(v):d=lib.ph_adjust(d,p,1)
 return d
def render(d,side=64):
 data=(c.c_uint*(side*side))();lib.ph_render(d,side,data)
 im=Image.new('RGB',(side,side));im.putdata([(v&255,(v>>8)&255,(v>>16)&255) for v in data]);return im
names=['HUMAN','ORRITH','AMPHIBIAN','INSECTOID','SAURAN','AVIAN','ROBOT','ANDROID','MASKED','CRYSTAL']
for sp in range(10):
 base=dna(sp,{3:1,5:1})
 cols=12;rows=sum(lib.ph_count(base,p)>1 for p in range(1,10))
 sheet=Image.new('RGB',(cols*80,rows*100+28),(13,23,34));draw=ImageDraw.Draw(sheet);draw.text((4,4),names[sp],fill='white')
 row=0
 for p in range(1,10):
  count=lib.ph_count(base,p)
  if count<2:continue
  d=base
  # Start each control at zero, retaining the other selected features.
  for n in range(lib.ph_value(d,p)):d=lib.ph_adjust(d,p,-1)
  for v in range(count):
   x=v*80;y=row*100+28;sheet.paste(render(d),(x,y));sheet.paste(render(d,20),(x+58,y+66));draw.text((x,y+68),f'{p}:{v}',fill='white');d=lib.ph_adjust(d,p,1)
  row+=1
 sheet.save(out/f'choices-{sp}.png')
 for choices,label in [({3:3,5:1},'blonde-teal'),({3:5,5:2},'blue-red'),({2:2,3:9,5:4,4:2,6:2,8:2,9:2,1:1},'mixed')]:
  d=dna(sp,{p:v for p,v in choices.items() if v<lib.ph_count(lib.ph_dna(sp),p)})
  render(d,96).resize((384,384),Image.Resampling.NEAREST).save(out/f'{sp}-{label}.png')
print('Actual C previews: ten complete control sheets plus mixed editor-size portraits.')
