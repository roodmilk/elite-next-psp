"""Exhaustive production-C portrait audit and independent anatomy fixtures.
Compile tools/portrait-host.c as outputs/portrait364-host/portrait.dll first.
No background-only hash can establish that a feature works.
"""
from pathlib import Path
import ctypes as c, json, sys, time,os
import numpy as np
root=Path(__file__).resolve().parents[1];out=root/'outputs/portrait364-host'
lib=c.CDLL(str(out/os.environ.get('PORTRAIT_QA_LIBRARY','portrait.dll')))
run=out/os.environ.get('PORTRAIT_QA_RUN','audit-final');run.mkdir(exist_ok=True)
for name in ('ph_dna','ph_adjust'):getattr(lib,name).restype=c.c_uint
lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int]
lib.ph_count.argtypes=lib.ph_value.argtypes=[c.c_uint,c.c_int]
lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
class Model(c.Structure):
 _fields_=[('ink',c.c_ubyte*4096),('owner',c.c_ubyte*4096),('pixels',c.c_uint16*4096)]
lib.ph_model.argtypes=[c.c_uint,c.POINTER(Model)]
lib.ph_exhaustive.argtypes=[c.c_char_p]
def select(d,p,v):
 while lib.ph_value(d,p)!=v:d=lib.ph_adjust(d,p,1)
 return d
def render(d,side):
 a=(c.c_uint*(side*side))();lib.ph_render(d,side,a);return np.ctypeslib.as_array(a).copy().reshape(side,side)
def model(d):
 m=Model();lib.ph_model(d,c.byref(m));return m
failures=[];checks=0
def check(ok,detail):
 global checks
 checks+=1
 if not ok:failures.append(detail)
for sp in range(10):
 for context in range(2):
  base=lib.ph_dna(sp)
  for p in (1,2,3,4,5,6,8,9):base=select(base,p,(context*(p+2))%lib.ph_count(base,p))
  for p in range(1,10):
   count=lib.ph_count(base,p)
   if count<2:continue
   ids=[select(base,p,v) for v in range(count)];models=[model(d) for d in ids]
   for size in (20,32,96):
    images=[render(d,size)[1:-1,1:-1] for d in ids];a=(np.arange(1,size-1)*64//size);indices=a[:,None]*64+a[None,:]
    for v in range(count):
     for prior in range(v):
      ma,mb=models[v],models[prior]
      ia=np.ctypeslib.as_array(ma.ink)[indices];ib=np.ctypeslib.as_array(mb.ink)[indices]
      oa=np.ctypeslib.as_array(ma.owner)[indices];ob=np.ctypeslib.as_array(mb.owner)[indices]
      changed=images[v]!=images[prior];foreground=(ia!=0)|(ib!=0)
      allowed=(oa==p+1)|(ob==p+1)
      if p==1:allowed=(ia!=12)&(ib!=12)
      if p==2:allowed=(ia==5)|(ib==5)
      if p==3:allowed=(ia==8)|(ib==8)
      if p==5:allowed=(ia==12)|(ib==12)
      n=int(np.count_nonzero(changed&foreground));bad=int(np.count_nonzero(changed&~allowed));bg=int(np.count_nonzero(changed&~foreground))
      delta=sum(int(np.abs(((images[v]>>shift)&255).astype('i4')-((images[prior]>>shift)&255).astype('i4'))[changed&foreground].sum()) for shift in (0,8,16))
      readable=n>=2 and (p!=3 or (n>=3 and delta>=n*54))
      check(readable and bad==0 and bg==0,dict(sp=sp,context=context,part=p,size=size,choices=[prior,v],foreground=n,unrelated=bad,background=bg,colour_delta=delta))
# Independent source-art landmarks: these are not derived from the masks.
for sp,neck,coat in [(0,[(29,49),(30,51),(32,53),(26,46)],[(22,51),(25,57),(40,56),(46,52)]),
                     (7,[(21,43),(23,46),(29,47),(33,49)],[(16,48),(23,52),(31,56),(43,55)])]:
 for skin in range(12):
  for hair in range(12):
   d=select(select(lib.ph_dna(sp),2,skin),3,hair);baseline=render(d,64)
   m=model(d)
   for x,y in neck:check(m.ink[y*64+x]==5,dict(fixture='neck material',sp=sp,x=x,y=y,material=m.ink[y*64+x]))
   for x,y in coat:check(m.ink[y*64+x]==12,dict(fixture='clothing material',sp=sp,x=x,y=y,material=m.ink[y*64+x]))
   for suit in range(1,6):
    im=render(select(d,5,suit),64)
    check(all(im[y,x]==baseline[y,x] for x,y in neck),dict(fixture='outfit never recolours neck',sp=sp,skin=skin,hair=hair,suit=suit))
print(f'All-pairs feature/anatomy checks: {checks}; failures: {len(failures)}',flush=True)
(run/'feature-audit.json').write_text(json.dumps(dict(checks=checks,failures=failures),indent=2))
if failures:
 print(json.dumps(failures[:35],indent=2));sys.exit(1)
start=time.monotonic();result=lib.ph_exhaustive(str(run/'combinations.bin').encode())
records=np.fromfile(run/'combinations.bin',dtype=[('dna','<u4'),('h20','<u4'),('h32','<u4'),('h96','<u4')])
# A hash collision is a candidate to inspect, never proof of a visual defect.
collisions={}
for key in ('h20','h32','h96'):
 order=np.argsort(records[key]);values=records[key][order];duplicate=values[1:]==values[:-1]
 pairs=np.stack((records['dna'][order[:-1]][duplicate],records['dna'][order[1:]][duplicate]),axis=1)
 equal=[]
 for a,b in pairs:
  if np.array_equal(render(int(a),int(key[1:])),render(int(b),int(key[1:]))):equal.append([int(a),int(b)])
 collisions[key]=dict(hash_candidates=len(pairs),identical_portraits=len(equal),examples=equal[:12])
report=dict(feature_checks=checks,combinations=len(records),renders=len(records)*3,structural_failures=result,collisions=collisions,seconds=round(time.monotonic()-start,1))
(run/'exhaustive-audit.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2),flush=True)
if result or any(v['identical_portraits'] for v in collisions.values()):sys.exit(1)
