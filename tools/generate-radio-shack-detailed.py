"""Continuous, individually painted facades; tiles are only a PSP storage unit.
No generic panel frame is applied at tile boundaries. Original indexed artwork.
"""
from pathlib import Path
from PIL import Image,ImageDraw
import collections,json,re
from encounter_panel_design import ROLES
root=Path(__file__).resolve().parents[1];out=root/'assets/generated/radio-shack-rework';out.mkdir(parents=True,exist_ok=True)
BASE=int(re.search(r"ENCOUNTER_TILE_BASE (\d+)",(root/'src/encounter-art-ids.h').read_text()).group(1))+len(ROLES)*4
DISPLAY_PALETTE=[(8,14,18),(35,46,53),(130,148,153),(158,176,179),(187,201,197),(174,126,64),(20,31,35),(15,45,65),(15,85,105),(180,220,240),(45,220,240),(45,145,240),(70,49,39),(175,185,183),(165,183,178),(81,97,105)]
tiles=[];ids={};purposes=[]
def sheet(name,w,h,paint):
 im=Image.new('P',(w,h),2);d=ImageDraw.Draw(im);paint(d,w,h)
 ids[name]=BASE+len(tiles);purposes.append(dict(name=name,width=w,height=h,first_tile=ids[name],tiles=w*h//1024))
 for y in range(0,h,32):
  for x in range(0,w,32):tiles.append(im.crop((x,y,x+32,y+32)))
 im.putpalette(sum((list(c) for c in DISPLAY_PALETTE),[])+[0]*720);im.save(out/(name.lower()+'.png'));return im
def recess(d,b):
 x,y,a,z=b;d.rectangle(b,fill=0);d.rectangle((x+1,y+1,a-1,z-1),fill=1);d.line((x,z,a,z),fill=3)
def pipe(d,pts):
 d.line(pts,fill=0,width=3);d.line([(x-1,y-1) for x,y in pts],fill=3,width=1)
def glass(d,b):
 recess(d,b);x,y,a,z=b;d.rectangle((x+2,y+2,a-2,z-2),fill=7)
 d.polygon([(x+2,y+2),(a-2,y+2),(x+2,z-2)],fill=8)
 d.line((x+3,y+3,a-4,y+3),fill=9)
def bolts(d,pts):
 for x,y in pts:d.point((x,y),fill=4);d.point((x,y+1),fill=1)
def front(d,w,h):
 # A patched airlock on the left; occupied broadcast room on the right.
 d.rectangle((0,0,95,5),fill=1);d.line((0,6,95,6),fill=3)
 recess(d,(6,13,30,58));d.rectangle((9,16,27,55),fill=3)
 glass(d,(12,19,24,30));d.rectangle((23,38,26,42),fill=5)
 d.line((15,48,24,48),fill=1);bolts(d,[(10,17),(26,17),(10,54),(26,54)])
 glass(d,(38,14,89,39))
 for x in (54,72):d.line((x,16,x,37),fill=3)
 # Mixer desks and silhouettes are painted behind the glazing.
 for x in (43,59,77):
  d.rectangle((x,31,x+9,35),fill=1);d.line((x+1,31,x+8,31),fill=10)
  d.ellipse((x+3,25,x+6,28),fill=1);d.rectangle((x+2,29,x+7,31),fill=12)
 recess(d,(40,45,88,55));d.line((43,48,83,48),fill=3)
 for x in range(43,84,6):d.rectangle((x,50,x+2,52),fill=12)
 # A modest physical station number, not repeated text on every patch.
 d.rectangle((6,7,30,11),fill=1)
 for x in (10,19):d.line((x,8,x+5,8),fill=5);d.line((x+5,8,x+5,10),fill=5)
 pipe(d,[(33,7),(33,55),(39,55)])
 for x,y in ((4,42),(36,58),(80,60),(91,8)):d.line((x,y,x+3,y),fill=12)
 d.line((0,62,95,62),fill=1)
def rear(d,w,h):
 # Service wall has no shop-window motif: rack bays and a real cable trunk.
 d.rectangle((0,0,95,6),fill=1);d.line((0,7,95,7),fill=3)
 for x in (5,28,51):
  recess(d,(x,14,x+18,48))
  for y in range(18,45,5):d.line((x+3,y,x+15,y),fill=3)
  bolts(d,[(x+2,16),(x+16,46)])
 recess(d,(75,15,90,39));d.rectangle((79,18,87,34),fill=12)
 pipe(d,[(9,54),(80,54),(80,43),(89,43)])
 pipe(d,[(13,57),(84,57),(84,47),(92,47)])
 d.rectangle((34,10,43,12),fill=5);d.point((82,19),fill=10)
 d.polygon([(2,49),(17,50),(18,59),(2,61)],fill=12)
 d.line((3,50,16,51),fill=3);d.line((0,62,95,62),fill=1)
def left(d,w,h):
 # Power conditioning, lower egress hatch and one narrow observation window.
 glass(d,(7,12,26,28));d.line((17,14,17,26),fill=3)
 recess(d,(35,10,57,36))
 for y in range(14,34,4):d.line((39,y,53,y),fill=3)
 recess(d,(8,38,28,57));d.rectangle((11,41,25,54),fill=3)
 d.rectangle((21,46,25,48),fill=5);d.line((16,44,16,51),fill=1)
 pipe(d,[(32,5),(32,53),(56,53)])
 pipe(d,[(61,9),(61,45),(50,45)])
 bolts(d,[(5,5),(58,6),(5,59),(58,59)])
 d.line((1,61,62,61),fill=1)
def right(d,w,h):
 # Receiver equipment, insulated waveguide and two welded repairs.
 recess(d,(5,9,39,43))
 for x in (10,20,30):
  d.rectangle((x,13,x+5,36),fill=12);d.line((x+1,15,x+1,33),fill=3)
 pipe(d,[(45,5),(45,27),(56,27),(56,52),(36,52)])
 d.rectangle((6,48,29,57),fill=3);d.rectangle((9,49,26,55),fill=12)
 bolts(d,[(7,49),(28,56),(4,5),(59,59)])
 d.line((1,61,62,61),fill=1);d.point((37,15),fill=10)
def roof(d,w,h):
 # Antenna cable route, hatch, folded thermal blanket and offset solar strip.
 d.rectangle((0,0,w-1,h-1),fill=1)
 for y in (6,25,49):d.line((3,y,92,y),fill=2)
 for x in (4,25,46,67):
  d.rectangle((x,6,x+16,24),fill=7)
  for xx in range(x+2,x+15,4):d.line((xx,8,xx,22),fill=8)
  d.line((x,6,x+16,6),fill=3)
 recess(d,(8,35,32,55));d.line((12,40,28,40),fill=3);d.rectangle((25,45,29,47),fill=5)
 pipe(d,[(90,4),(90,33),(43,33),(43,58)])
 d.polygon([(51,39),(76,37),(82,53),(56,57)],fill=12)
 d.line((53,40,74,39),fill=3);d.line((58,52,79,50),fill=1)
 bolts(d,[(3,3),(92,3),(3,60),(92,60)])
sheet('FRONT',96,64,front);sheet('REAR',96,64,rear);sheet('LEFT',64,64,left);sheet('RIGHT',64,64,right);sheet('ROOF',96,64,roof)
def sphere(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=2)
 for y in (12,58,116,179,213):
  d.line((0,y,w-1,y),fill=1);d.line((0,y+1,w-1,y+1),fill=3)
 for k in range(16):
  x=k*32+15
  d.line([(x,0),(x-3,58),(x+1,116),(x-2,179),(x,223)],fill=1)
  for y in (16,183):bolts(d,[(x-2,y),(x+2,y+1)])
  if k in (0,3,7,10,14):
   # Small oval maintenance covers, not large square windows on the ball.
   xx=k*32+9;d.ellipse((xx,86,xx+13,106),fill=1)
   d.ellipse((xx+1,87,xx+12,105),fill=3)
   d.line((xx+3,92,xx+10,92),fill=1);d.rectangle((xx+8,98,xx+11,100),fill=5)
  if k in (1,5,8,12):
   xx=k*32;d.polygon([(xx+5,22),(xx+24,25),(xx+22,44),(xx+6,42)],fill=12)
   d.line((xx+6,23,xx+23,26),fill=3)
  if k in (2,6,11,15):pipe(d,[(k*32+6,140),(k*32+6,164),(k*32+24,164)])
 pipe(d,[(1,52),(131,52),(139,46),(276,46),(284,52),(511,52)])
 for x in (42,164,285,421):
  d.rectangle((x,124,x+12,129),fill=12);d.line((x+2,125,x+10,125),fill=3)
sheet('SPHERE',512,224,sphere)
def deck(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1)
 for k in range(12):
  x=k*32;d.line((x+4,4,x+27,28),fill=2)
  d.line((x+8,4,x+30,25),fill=2)
  if k in (0,1,11):d.line((x+5,25,x+25,25),fill=5)
  else:recess(d,(x+6,10,x+22,20));d.line((x+8,12,x+20,12),fill=3)
sheet('DECK',384,32,deck)
def rim(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=2);d.line((0,4,w-1,4),fill=4);d.line((0,28,w-1,28),fill=1)
 for k in range(12):
  x=k*32;recess(d,(x+4,10,x+27,23))
  for xx in (x+8,x+15,x+22):d.rectangle((xx,13,xx+2,19),fill=1)
  bolts(d,[(x+2,7),(x+29,25)])
  if k in (0,3,6,9):d.rectangle((x+12,6,x+18,8),fill=9)
sheet('RIM',384,32,rim)
def dish(d,w,h,back=False):
 d.rectangle((0,0,w-1,h-1),fill=2)
 for k in range(8):
  x=k*32
  if back:
   d.polygon([(x+3,30),(x+13,2),(x+18,2),(x+29,30)],fill=1)
   d.line((x+15,3,x+15,28),fill=3)
   for y in (11,22):d.line((x+6,y,x+25,y),fill=3)
  else:
   d.polygon([(x+3,29),(x+11,3),(x+20,3),(x+28,29)],fill=3)
   d.line((x+15,3,x+15,28),fill=4)
   for y in (10,19,27):d.line((x+8,y,x+24,y),fill=2)
sheet('DISH_FRONT',256,32,lambda d,w,h:dish(d,w,h));sheet('DISH_BACK',256,32,lambda d,w,h:dish(d,w,h,True))
def equipment(d,w,h):
 for k in range(8):
  x=k*32;recess(d,(x+2,3,x+29,28))
  if k<3:
   for y in range(7,26,4):d.line((x+6,y,x+25,y),fill=3)
   if k==1:d.rectangle((x+20,6,x+26,11),fill=5)
  elif k<5:
   d.ellipse((x+7,6,x+24,23),fill=3);d.ellipse((x+10,9,x+21,20),fill=1)
   pipe(d,[(x+3,27),(x+3,18),(x+11,18)])
  else:
   d.rectangle((x+6,6,x+25,24),fill=12);d.line((x+9,9,x+22,9),fill=4)
   bolts(d,[(x+5,5),(x+26,26)])
sheet('EQUIPMENT',256,32,equipment)
packed=[]
for im in tiles:
 arr=list(im.getdata());data=[]
 for size in (32,16,8):
  step=32//size;small=[]
  for y in range(size):
   for x in range(size):small.append(collections.Counter(arr[(y*step+dy)*32+x*step+dx] for dy in range(step) for dx in range(step)).most_common(1)[0][0])
  data.extend(small[i]|small[i+1]<<4 for i in range(0,len(small),2))
 packed.append('{'+','.join(map(str,data))+'}')
(root/'src/radio-shack-tiles.h').write_text(',\n'.join(packed)+'\n')
(root/'src/radio-shack-art.h').write_text('\n'.join(f'#define RADIO_{k} {v}' for k,v in ids.items())+'\n')
(out/'surface-map.json').write_text(json.dumps(dict(tile_count=len(tiles),base=BASE,surfaces=purposes),indent=2))
print('Radio shack:',len(tiles),'individual tiles from continuous, side-specific artwork')
