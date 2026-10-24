"""Nine encounter paintings, composed per surface, not per storage tile.
Each named object has its own facade layouts and wraparound hull artwork.
Original hand-positioned indexed art; no random greeble/noise generation.
"""
from pathlib import Path
from PIL import Image,ImageDraw
import collections,json,re
root=Path(__file__).resolve().parents[1];out=root/'assets/generated/nine-encounter-reworks';out.mkdir(exist_ok=True,parents=True)
radio=json.loads((root/'assets/generated/radio-shack-rework/surface-map.json').read_text());BASE=radio['base']+radio['tile_count']
PAL=[(8,14,18),(35,46,53),(130,148,153),(158,176,179),(187,201,197),(174,126,64),(20,31,35),(15,45,65),(15,85,105),(180,220,240),(45,220,240),(45,145,240),(70,49,39),(175,185,183),(165,183,178),(81,97,105)]
tiles=[];ids={};manifest=[]
def recess(d,b):
 x,y,a,z=b;d.rectangle(b,fill=0);d.rectangle((x+1,y+1,a-1,z-1),fill=1);d.line((x,z,a,z),fill=3)
def glass(d,b):
 recess(d,b);x,y,a,z=b;d.rectangle((x+2,y+2,a-2,z-2),fill=7);d.polygon([(x+2,y+2),(a-2,y+2),(x+2,z-2)],fill=8);d.line((x+3,y+3,a-4,y+3),fill=9)
def pipe(d,p):d.line(p,fill=0,width=3);d.line([(x-1,y-1) for x,y in p],fill=3,width=1)
def vents(d,b):
 recess(d,b);x,y,a,z=b
 for yy in range(y+3,z-1,4):d.line((x+3,yy,a-3,yy),fill=3)
def hatch(d,b,window=True):
 recess(d,b);x,y,a,z=b;d.rectangle((x+2,y+2,a-2,z-2),fill=3)
 if window:glass(d,(x+4,y+4,a-4,y+13))
 d.line((x+4,z-5,a-4,z-5),fill=1);d.rectangle((a-7,z-14,a-3,z-12),fill=5)
 for xx,yy in ((x+2,y+2),(a-2,y+2),(x+2,z-2),(a-2,z-2)):d.point((xx,yy),fill=4)
def band(d,y,x=2,end=93):
 d.line((x,y,end,y),fill=1);d.line((x,y+1,end,y+1),fill=3)
def wear(d,pts):
 for x,y,n in pts:d.line((x,y,x+n,y),fill=12);d.point((x,y-1),fill=3)
def foliage(d,x,y):
 d.line((x,y,x,y+8),fill=14)
 d.polygon([(x,y+4),(x-4,y),(x-3,y+5),(x,y+7),(x+4,y+1),(x+2,y)],fill=14);d.point((x-1,y+2),fill=4)
def motor(d,x,y):
 d.rectangle((x-7,y-6,x+7,y+6),fill=12);d.ellipse((x-5,y-5,x+5,y+5),fill=3);d.ellipse((x-3,y-3,x+3,y+3),fill=1);d.line((x-8,y+8,x+8,y+8),fill=4)
def cup(d,x,y):
 d.rectangle((x,y,x+5,y+5),fill=14);d.arc((x+3,y,x+9,y+4),270,90,fill=14);d.line((x-1,y+7,x+8,y+7),fill=5)
def sheet(obj,name,w,h,paint):
 im=Image.new('P',(w,h),2);d=ImageDraw.Draw(im);paint(d,w,h)
 key=obj+'_'+name;ids[key]=BASE+len(tiles);start=ids[key]
 for y in range(0,h,32):
  for x in range(0,w,32):tiles.append(im.crop((x,y,x+32,y+32)))
 folder=out/obj.lower();folder.mkdir(exist_ok=True);im.putpalette(sum((list(c) for c in PAL),[])+[0]*720);im.save(folder/(name.lower()+'.png'))
 manifest.append(dict(object=obj,surface=name,width=w,height=h,first_tile=start,count=len(tiles)-(start-BASE)))

def salvage_front(d,w,h):
 band(d,6);hatch(d,(5,13,29,59));glass(d,(37,13,89,37))
 for x in (53,71):d.line((x,15,x,35),fill=3)
 d.rectangle((40,30,85,34),fill=12)
 for x in (44,61,77):d.rectangle((x,26,x+6,30),fill=3);d.point((x+3,27),fill=10)
 pipe(d,[(33,9),(33,51),(89,51)]);vents(d,(42,42,71,48));wear(d,[(3,45,3),(77,58,9),(37,8,5)])
def salvage_rear(d,w,h):
 band(d,8);hatch(d,(68,17,91,58),False)
 for x in (5,27,48):vents(d,(x,14,x+15,43))
 pipe(d,[(9,48),(59,48),(59,57),(93,57)]);pipe(d,[(12,52),(53,52),(53,60)])
 d.rectangle((30,7,47,10),fill=12);wear(d,[(6,56,15),(72,11,9)])
def salvage_left(d,w,h):
 glass(d,(5,10,27,28));hatch(d,(34,16,58,59),False)
 d.polygon([(3,38),(27,35),(29,55),(6,58)],fill=12);d.line((6,39,25,37),fill=3)
 pipe(d,[(30,5),(30,48),(4,48)]);band(d,61,1,62)
def salvage_right(d,w,h):
 vents(d,(4,8,23,40));glass(d,(33,11,58,30));d.line((45,13,45,28),fill=3)
 pipe(d,[(29,3),(29,43),(55,43),(55,58)]);hatch(d,(5,45,22,60),False);wear(d,[(34,54,13)])
def salvage_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1);hatch(d,(7,8,30,31),False)
 for x in (39,61):vents(d,(x,7,x+15,33))
 pipe(d,[(4,45),(85,45),(85,7)]);d.polygon([(12,51),(48,48),(56,60),(16,62)],fill=12)
 d.line((13,52,47,49),fill=3);d.rectangle((61,48,88,56),fill=3)
def breaker_front(d,w,h):
 glass(d,(4,10,90,32));
 for x in (24,46,68):d.line((x,12,x,30),fill=3)
 for x in (10,34,58):d.rectangle((x,25,x+13,29),fill=1);d.line((x+2,25,x+10,25),fill=10)
 hatch(d,(5,38,26,59),False);vents(d,(33,40,69,58));d.rectangle((77,42,89,54),fill=12);band(d,5)
def breaker_rear(d,w,h):
 for x in (5,31,58):hatch(d,(x,11,x+21,45),False)
 pipe(d,[(9,53),(83,53),(83,8)]);pipe(d,[(13,57),(89,57),(89,9)]);wear(d,[(4,5,19),(59,48,7)])
def breaker_left(d,w,h):
 vents(d,(4,8,27,43));hatch(d,(35,16,59,58),False);pipe(d,[(7,51),(30,51),(30,4)])
 for x in range(4,29,6):d.line((x,58,x+3,61),fill=5)
def breaker_right(d,w,h):
 glass(d,(5,8,59,29));d.line((30,10,30,27),fill=3);vents(d,(6,38,36,57));hatch(d,(43,37,59,58),False)
def breaker_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1);vents(d,(6,9,38,42));hatch(d,(48,8,75,35),False);pipe(d,[(4,51),(86,51),(86,6)])
 for x in (7,22,54,70):d.rectangle((x,56,x+8,60),fill=3)
def greenhouse_front(d,w,h):
 glass(d,(3,7,33,41));glass(d,(64,7,92,41));hatch(d,(37,9,59,59))
 for x in (11,24,72,85):foliage(d,x,22)
 pipe(d,[(4,48),(29,48),(29,57)]);pipe(d,[(67,49),(89,49),(89,58)]);band(d,3)
def greenhouse_rear(d,w,h):
 for x in (5,35,65):
  recess(d,(x,8,x+24,39));d.ellipse((x+5,13,x+19,31),fill=3);d.line((x+7,17,x+17,17),fill=1)
 pipe(d,[(7,46),(80,46),(80,58)]);pipe(d,[(12,51),(86,51),(86,60)]);hatch(d,(35,54,59,63),False)
def greenhouse_left(d,w,h):
 glass(d,(5,8,58,38));d.line((31,10,31,36),fill=3)
 for x in (14,26,43,53):foliage(d,x,23)
 vents(d,(5,46,25,59));pipe(d,[(32,44),(32,55),(58,55)])
def greenhouse_right(d,w,h):
 hatch(d,(4,9,27,55));vents(d,(36,9,59,34));pipe(d,[(32,4),(32,45),(56,45),(56,60)]);d.point((39,39),fill=10)
def greenhouse_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1)
 for x in (5,26,47,68):
  d.rectangle((x,5,x+16,25),fill=7)
  for yy in (10,16,22):d.line((x+2,yy,x+14,yy),fill=8)
 hatch(d,(6,35,30,57),False);pipe(d,[(38,33),(87,33),(87,58)]);vents(d,(46,41,75,58))
def fuel_front(d,w,h):
 glass(d,(5,7,91,31));
 for x in (28,54,76):d.line((x,9,x,29),fill=3)
 hatch(d,(6,36,30,59));recess(d,(39,38,88,55))
 for x in (44,56,68):d.line((x,43,x+6,43),fill=5);d.line((x+6,43,x+6,50),fill=5);d.line((x,50,x+6,50),fill=5)
 pipe(d,[(2,3),(93,3)]);band(d,61)
def fuel_rear(d,w,h):
 for x in (6,36,65):
  vents(d,(x,8,x+23,35));recess(d,(x+2,41,x+21,58));d.ellipse((x+5,43,x+18,55),fill=3)
 pipe(d,[(3,39),(91,39)]);wear(d,[(13,4,18),(40,60,28)])
def fuel_left(d,w,h):
 hatch(d,(5,10,28,58));pipe(d,[(34,5),(34,44),(58,44)])
 for x in (39,51):
  d.ellipse((x-4,12,x+4,21),fill=3);d.line((x,16,x+2,14),fill=5)
 vents(d,(38,49,58,60))
def fuel_right(d,w,h):
 vents(d,(4,8,28,31));hatch(d,(34,15,59,58),False);pipe(d,[(8,42),(27,42),(27,57),(4,57)])
def fuel_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1);hatch(d,(5,7,30,36),False);vents(d,(41,8,67,34));pipe(d,[(5,48),(87,48),(87,5)])
 d.rectangle((38,55,75,60),fill=12);d.line((41,55,71,55),fill=3)
def ferry_front(d,w,h):
 glass(d,(4,8,91,36));
 for x in (22,45,69):d.line((x,10,x,34),fill=3)
 d.rectangle((8,28,87,33),fill=1)
 for x in (13,36,60,78):d.line((x,28,x+8,28),fill=10)
 vents(d,(8,44,38,57));hatch(d,(49,42,77,60),False);band(d,4)
def ferry_rear(d,w,h):
 hatch(d,(36,9,60,58));vents(d,(5,15,27,39));vents(d,(69,15,91,39));pipe(d,[(4,49),(29,49),(29,57)]);pipe(d,[(67,57),(67,49),(92,49)])
def ferry_left(d,w,h):
 glass(d,(5,10,58,32));d.line((30,12,30,30),fill=3);hatch(d,(5,38,27,59),False);vents(d,(35,39,58,57));band(d,4,2,61)
def ferry_right(d,w,h):
 glass(d,(4,8,59,30));d.line((20,10,20,28),fill=3);d.line((43,10,43,28),fill=3);vents(d,(5,39,32,59));pipe(d,[(39,35),(39,52),(58,52)])
def ferry_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1)
 for x in (7,31,55,77):glass(d,(x,6,x+13,27))
 pipe(d,[(4,35),(89,35)]);hatch(d,(9,41,32,59),False);vents(d,(45,43,86,58))
def tea_front(d,w,h):
 glass(d,(4,10,91,45))
 for x in (25,49,73):d.line((x,12,x,43),fill=3)
 for x in (11,35,59,80):
  d.rectangle((x-2,28,x+9,42),fill=12);d.line((x-3,34,x+11,34),fill=4);cup(d,x,27)
 band(d,6);d.rectangle((6,49,89,57),fill=3);wear(d,[(2,59,15),(75,60,13)])
def tea_rear(d,w,h):
 hatch(d,(5,10,29,59));glass(d,(40,11,89,32));
 for x in (44,59,75):cup(d,x,18)
 vents(d,(37,42,70,57));pipe(d,[(76,39),(89,39),(89,58)]);band(d,5)
def tea_left(d,w,h):
 glass(d,(5,11,58,37));d.line((30,13,30,35),fill=3)
 for x in (13,40):cup(d,x,26)
 d.line((6,45,57,45),fill=3);vents(d,(9,49,50,59))
def tea_right(d,w,h):
 hatch(d,(5,12,29,58));glass(d,(37,10,59,34));cup(d,43,23);vents(d,(36,43,59,58));pipe(d,[(2,4),(60,4)])
def tea_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1)
 for x in (6,29,52):glass(d,(x,8,x+17,34))
 vents(d,(75,7,90,41));pipe(d,[(7,44),(85,44),(85,58)]);hatch(d,(8,49,32,61),False)
def ore_front(d,w,h):
 glass(d,(4,8,91,31));
 for x in (25,49,72):d.line((x,10,x,29),fill=3)
 for x in (11,39,67):d.rectangle((x,25,x+14,29),fill=1);d.line((x+2,25,x+11,25),fill=10)
 hatch(d,(5,37,28,60));vents(d,(38,41,88,57));band(d,4)
def ore_rear(d,w,h):
 for x in (5,35,65):
  hatch(d,(x,9,x+24,37),False);vents(d,(x,43,x+24,58))
 pipe(d,[(3,39),(93,39)]);wear(d,[(15,5,19),(60,60,21)])
def ore_left(d,w,h):
 vents(d,(5,8,30,39));hatch(d,(37,14,59,58),False);pipe(d,[(6,47),(29,47),(29,59)]);band(d,4,2,61)
def ore_right(d,w,h):
 hatch(d,(4,10,29,59));recess(d,(37,13,59,40));d.ellipse((41,18,55,33),fill=3);d.line((47,25,52,21),fill=5);pipe(d,[(35,49),(59,49)])
def ore_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1);hatch(d,(7,9,33,38),False);vents(d,(47,7,87,38));pipe(d,[(4,47),(88,47),(88,59)])
 for x in (16,51,73):d.rectangle((x,54,x+9,60),fill=12)
def parts_front(d,w,h):
 recess(d,(4,10,91,54));d.rectangle((7,13,88,49),fill=0)
 for y in (13,16,19):d.line((7,y,88,y),fill=3)
 for x in (21,48,75):motor(d,x,36)
 d.line((7,49,88,49),fill=4);d.rectangle((9,57,31,61),fill=5);band(d,5)
def parts_rear(d,w,h):
 hatch(d,(5,10,29,58),False)
 for x in (37,62):
  for y in (12,35):recess(d,(x,y,x+21,y+19));d.rectangle((x+5,y+5,x+15,y+13),fill=12);d.line((x+4,y+4,x+17,y+4),fill=3)
 pipe(d,[(32,5),(32,60),(92,60)])
def parts_left(d,w,h):
 glass(d,(5,10,59,35));motor(d,20,24);motor(d,45,24);vents(d,(7,45,35,59));hatch(d,(42,43,59,60),False)
def parts_right(d,w,h):
 hatch(d,(5,10,29,58));vents(d,(37,9,59,31));pipe(d,[(35,37),(57,37),(57,56),(35,56)]);d.rectangle((39,44,49,49),fill=12)
def parts_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1);glass(d,(6,7,41,31));hatch(d,(54,8,82,35),False);pipe(d,[(5,43),(89,43),(89,58)]);vents(d,(10,49,45,59))
def beacon_front(d,w,h):
 glass(d,(4,8,91,34));
 for x in (29,62):d.line((x,10,x,32),fill=3)
 d.rectangle((9,27,86,31),fill=1);d.line((13,27,35,27),fill=10);d.line((51,27,81,27),fill=5)
 hatch(d,(6,40,30,60));vents(d,(42,43,87,58));band(d,4)
def beacon_rear(d,w,h):
 for x in (6,36,65):motor(d,x+11,25);vents(d,(x,42,x+22,58))
 pipe(d,[(3,7),(93,7)]);wear(d,[(8,35,19),(65,36,23)])
def beacon_left(d,w,h):
 hatch(d,(5,9,29,59));vents(d,(37,10,59,31));pipe(d,[(35,39),(53,39),(53,56),(35,56)])
def beacon_right(d,w,h):
 recess(d,(4,8,59,55));d.rectangle((8,12,26,49),fill=12)
 for y in (16,27,38):d.line((12,y,22,y),fill=3)
 d.ellipse((34,16,54,38),fill=3);d.ellipse((38,20,50,34),fill=1);pipe(d,[(31,48),(55,48)])
def beacon_roof(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1);glass(d,(6,6,43,29));hatch(d,(55,8,84,35),False);pipe(d,[(5,42),(89,42),(89,59)]);vents(d,(11,49,43,59))

FACADES={
 'SALVAGE':(salvage_front,salvage_rear,salvage_left,salvage_right,salvage_roof),
 'BREAKER':(breaker_front,breaker_rear,breaker_left,breaker_right,breaker_roof),
 'GARDEN':(greenhouse_front,greenhouse_rear,greenhouse_left,greenhouse_right,greenhouse_roof),
 'FUEL':(fuel_front,fuel_rear,fuel_left,fuel_right,fuel_roof),
 'FERRY':(ferry_front,ferry_rear,ferry_left,ferry_right,ferry_roof),
 'TEA':(tea_front,tea_rear,tea_left,tea_right,tea_roof),
 'ORE':(ore_front,ore_rear,ore_left,ore_right,ore_roof),
 'PARTS':(parts_front,parts_rear,parts_left,parts_right,parts_roof),
 'BEACON':(beacon_front,beacon_rear,beacon_left,beacon_right,beacon_roof)}
for obj,paintings in FACADES.items():
 for name,paint in zip(['FRONT','REAR','LEFT','RIGHT','ROOF'],paintings):sheet(obj,name,64 if name in ('LEFT','RIGHT') else 96,64,paint)

def wrap(d,w,h,kind):
 # The eight circumferential columns have different physical functions.
 d.rectangle((0,0,w-1,h-1),fill=2)
 for y in (8,39,77):band(d,y,0,255)
 for k in range(8):
  x=k*32
  if k in (0,3,4):
   if kind=='FERRY':
    for y in (14,45):
     for xx in (x+4,x+18):glass(d,(xx,y,xx+10,y+18));d.line((xx+3,y+14,xx+7,y+14),fill=5)
    d.line((x,69,x+31,69),fill=5)
   elif kind=='TEA':
    glass(d,(x+3,12,x+28,67));d.line((x+4,39,x+27,39),fill=3);cup(d,x+10,27);cup(d,x+10,52)
   elif kind=='SALVAGE':
    glass(d,(x+3,12,x+27,38));hatch(d,(x+5,46,x+26,86));wear(d,[(x+2,70,2),(x+28,25,3)])
   elif kind=='BEACON':
    hatch(d,(x+5,13,x+27,55));vents(d,(x+5,63,x+27,86));d.rectangle((x+2,89,x+29,92),fill=5)
   else:
    recess(d,(x+4,13,x+28,61));pipe(d,[(x+8,18),(x+8,54),(x+24,54)])
    d.polygon([(x+2,67),(x+25,64),(x+28,86),(x+4,90)],fill=12)
  elif k in (1,2):
   if kind in ('FERRY','TEA'):glass(d,(x+5,18,x+27,62));d.line((x+16,20,x+16,60),fill=3)
   else:vents(d,(x+5,15,x+26,55));hatch(d,(x+7,65,x+25,87),False)
   pipe(d,[(x+3,7),(x+3,86)])
  else:
   vents(d,(x+6,14,x+26,41));pipe(d,[(x+4,52),(x+4,84),(x+28,84)])
   recess(d,(x+11,59,x+26,76));d.rectangle((x+15,63,x+22,71),fill=12)
  # Non-tile-aligned repairs join sections rather than stamping32px borders.
  if k in (3,6):wear(d,[(x+6,34,16),(x+18,72,9)])
for obj in ('SALVAGE','BREAKER','FUEL','FERRY','TEA','BEACON'):sheet(obj,'SKIN',256,96,lambda d,w,h,obj=obj:wrap(d,w,h,obj))
def rock(d,w,h,mineral=False):
 d.rectangle((0,0,w-1,h-1),fill=2)
 d.polygon([(0,5),(78,14),(139,5),(216,18),(301,9),(383,16),(383,24),(302,17),(213,27),(139,14),(79,22),(0,14)],fill=3)
 d.polygon([(0,44),(70,37),(141,48),(220,35),(310,44),(383,36),(383,51),(312,55),(219,46),(141,59),(68,48),(0,56)],fill=1)
 for pts in ([(25,0),(39,19),(31,38),(45,63)],[(118,0),(112,22),(130,39),(124,63)],[(246,0),(237,25),(259,45),(249,63)],[(342,0),(350,20),(333,42),(347,63)]):
  d.line(pts,fill=0);d.line([(x+1,y) for x,y in pts],fill=3)
 if mineral:
  for x,y in ((61,24),(180,42),(289,19)):
   d.polygon([(x,y),(x+18,y-5),(x+26,y+7),(x+8,y+10)],fill=12);d.line((x+1,y,x+18,y-4),fill=5)
sheet('SALVAGE','ROCK',384,64,lambda d,w,h:rock(d,w,h));sheet('ORE','ROCK',384,64,lambda d,w,h:rock(d,w,h,True))
def deck(d,w,h,obj):
 d.rectangle((0,0,w-1,h-1),fill=1)
 for x in range(3,125,10):d.line((x,4,x,27),fill=2)
 d.line((2,28,125,28),fill=3)
 if obj in ('FUEL','BREAKER','ORE'):
  for x in range(5,125,9):d.line((x,3,x+4,7),fill=5)
 elif obj=='TEA':
  for x in (9,48,88):cup(d,x,12)
 else:pipe(d,[(3,9),(121,9),(121,23)])
for obj in ('SALVAGE','BREAKER','GARDEN','FUEL','TEA','ORE','PARTS','BEACON'):sheet(obj,'DECK',128,32,lambda d,w,h,obj=obj:deck(d,w,h,obj))
def toolskin(d,w,h,kind):
 for x in range(0,w,32):
  if kind=='BREAKER':
   recess(d,(x+3,5,x+28,27));d.ellipse((x+6,7,x+25,25),fill=3)
   for xx in (x+11,x+15,x+19):d.line((xx,10,xx,22),fill=1)
  elif kind=='SALVAGE':
   recess(d,(x+3,4,x+28,28));d.rectangle((x+7,7,x+12,24),fill=12);d.ellipse((x+15,7,x+25,23),fill=3)
   for xx in (x+18,x+21,x+24):d.line((xx,9,xx,21),fill=1)
  else:
   recess(d,(x+3,3,x+28,28));d.ellipse((x+6,6,x+25,25),fill=3);d.ellipse((x+10,10,x+21,21),fill=1)
   d.rectangle((x+3,26,x+27,28),fill=5)
for obj in ('SALVAGE','BREAKER','FUEL','BEACON'):sheet(obj,'TOOLS',64,32,lambda d,w,h,obj=obj:toolskin(d,w,h,obj))
def cut(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=0)
 for x in (5,22,45):
  pipe(d,[(x,3),(x,22),(x+7,30),(x+7,60)])
 for y in (14,43):
  d.rectangle((4,y,59,y+7),fill=12);d.line((5,y,58,y),fill=3)
 for x,y in ((10,25),(35,10),(48,54)):motor(d,x,y)
sheet('BREAKER','CUT',64,64,cut)
def glaze(d,w,h,b=False):
 d.rectangle((0,0,w-1,h-1),fill=7)
 for y in (5,33):
  d.rectangle((2,y,189,y+23),fill=6);d.line((3,y+22,188,y+22),fill=3)
  for x in range(10,187,13 if b else 16):foliage(d,x,y+8)
  d.line((4,y+2,187,y+2),fill=9)
 for x in (4,37,76,113,151,188):d.line((x,2,x,61),fill=3)
 d.line((1,30,190,30),fill=1);d.line((1,62,190,62),fill=3)
sheet('GARDEN','GLAZE',192,64,lambda d,w,h:glaze(d,w,h));sheet('GARDEN','GLAZE_B',192,64,lambda d,w,h:glaze(d,w,h,True))
def garden_end(d,w,h):
 glass(d,(5,5,58,58));hatch(d,(21,21,42,59));
 for x in (11,51):foliage(d,x,32)
 d.line((5,19,58,19),fill=3);d.line((5,43,18,43),fill=3);d.line((45,43,58,43),fill=3)
sheet('GARDEN','END',64,64,garden_end)
def tank(d,w,h,patch=False,water=False):
 d.rectangle((0,0,w-1,h-1),fill=2)
 for y in (6,56):band(d,y,0,255)
 for x in (6,68,130,194):
  d.line((x,9,x,54),fill=1);d.line((x+1,9,x+1,54),fill=3)
 for x in (19,92,163,228):
  recess(d,(x,18,x+18,39));d.line((x+3,21,x+15,21),fill=3)
  if water:d.rectangle((x+4,27,x+13,35),fill=8)
  else:d.rectangle((x+4,27,x+13,35),fill=5)
 pipe(d,[(1,46),(119,46),(127,41),(255,41)])
 if patch:
  for x in (38,151):
   d.polygon([(x,10),(x+27,13),(x+25,35),(x+3,32)],fill=12);d.line((x+2,11,x+26,14),fill=3)
sheet('FUEL','TANK_A',256,64,lambda d,w,h:tank(d,w,h));sheet('FUEL','TANK_B',256,64,lambda d,w,h:tank(d,w,h,True))
sheet('GARDEN','TANK',256,64,lambda d,w,h:tank(d,w,h,False,True));sheet('ORE','SILO',256,64,lambda d,w,h:tank(d,w,h,True))
def engines(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=1)
 for x in (4,35):
  d.ellipse((x,7,x+24,48),fill=3);d.ellipse((x+3,10,x+21,45),fill=12);d.ellipse((x+7,15,x+17,39),fill=0)
  for xx in (x+9,x+13):d.line((xx,20,xx,35),fill=8)
 d.line((3,55,60,55),fill=5)
sheet('FERRY','ENGINE',64,64,engines)
def tea_sign(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=7);d.ellipse((8,8,55,55),fill=1)
 d.polygon([(16,23),(43,23),(39,46),(20,46)],fill=14);d.arc((36,23,52,40),270,90,fill=14,width=3);d.line((15,49,45,49),fill=5)
 d.line([(24,19),(22,15),(25,11)],fill=9);d.line([(35,19),(33,14),(36,9)],fill=9)
sheet('TEA','SIGN',64,64,tea_sign)
def awning(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=7)
 for x in range(0,w,16):d.rectangle((x,0,x+7,29),fill=3)
 d.line((0,29,w-1,29),fill=5)
sheet('TEA','AWNING',128,32,awning)
def hopper(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=2)
 for x in (5,36,68,99):
  d.polygon([(x,5),(x+23,5),(x+21,56),(x+3,56)],fill=1)
  d.line((x+2,7,x+21,7),fill=3);d.line((x+4,52,x+19,52),fill=5)
 pipe(d,[(2,60),(125,60)])
sheet('ORE','HOPPER',128,64,hopper)
def pod(d,w,h,second=False):
 d.rectangle((0,0,w-1,h-1),fill=2)
 for k in range(8):
  x=k*32
  if k in (0,3,4):
   glass(d,(x+3,8,x+28,45))
   motor(d,x+15,29 if not second else 22)
   if second:recess(d,(x+5,39,x+26,54));d.line((x+8,44,x+23,44),fill=5)
  elif k in (1,2):vents(d,(x+5,10,x+26,46))
  else:hatch(d,(x+5,12,x+27,54),False)
  band(d,58,x,x+31)
sheet('PARTS','POD_A',256,64,lambda d,w,h:pod(d,w,h));sheet('PARTS','POD_B',256,64,lambda d,w,h:pod(d,w,h,True))
def lens(d,w,h):
 d.rectangle((0,0,w-1,h-1),fill=2)
 for x in (0,32,64,96):
  glass(d,(x+3,5,x+28,56));d.line((x+5,20,x+26,20),fill=3);d.line((x+5,41,x+26,41),fill=3)
  d.rectangle((x+10,24,x+21,37),fill=9);d.line((x+15,8,x+15,53),fill=4)
sheet('BEACON','LENS',128,64,lens)
def robot(d,w,h):
 for x in (0,32,64,96):
  recess(d,(x+3,3,x+28,28));glass(d,(x+7,6,x+24,17));d.point((x+16,11),fill=10);pipe(d,[(x+6,23),(x+25,23)])
sheet('BEACON','ROBOT',128,32,robot)

# Hull end paintings are fitted inside the octagonal pressure bulkheads.
# Fronts have access/approach hardware; rears have distinct engine/service banks.
def hull_end(d,w,h,obj,rear):
 d.rectangle((0,0,63,31),fill=2);band(d,2,0,63);band(d,29,0,63)
 if rear:
  if obj in ('FERRY','BEACON','TEA'):
   for x in (12,49):
    d.ellipse((x-9,6,x+9,25),fill=0);d.ellipse((x-7,8,x+7,23),fill=15);d.ellipse((x-4,11,x+4,20),fill=7);d.line((x-3,14,x+3,14),fill=8)
   vents(d,(26,7,36,24))
  else:
   vents(d,(4,6,23,25));hatch(d,(29,5,47,27),False);pipe(d,[(53,5),(53,25),(61,25)])
 else:
  hatch(d,(24,4,40,28),False)
  if obj in ('FERRY','BEACON'):
   glass(d,(3,7,19,17));glass(d,(45,7,61,17))
  else:
   vents(d,(3,7,18,24));recess(d,(46,8,60,23));d.line((49,12,57,12),fill=5)
 wear(d,[(2,27,9),(44,3,7)])
for obj in ('SALVAGE','BREAKER','FUEL','FERRY','TEA','BEACON'):
 for rear in (False,True):
  sheet(obj,'CAP_BACK' if rear else 'CAP_FRONT',64,32,lambda d,w,h,o=obj,r=rear:hull_end(d,w,h,o,r))

packed=[]
for im in tiles:
 arr=list(im.getdata());data=[]
 for size in (32,16,8):
  step=32//size;small=[]
  for y in range(size):
   for x in range(size):small.append(collections.Counter(arr[(y*step+dy)*32+x*step+dx] for dy in range(step) for dx in range(step)).most_common(1)[0][0])
  data.extend(small[i]|small[i+1]<<4 for i in range(0,len(small),2))
 packed.append('{'+','.join(map(str,data))+'}')
(root/'src/nine-encounter-tiles.h').write_text(',\n'.join(packed)+'\n')
(root/'src/nine-encounter-art.h').write_text('\n'.join(f'#define N_{key} {value}' for key,value in ids.items())+'\n')
(out/'surface-map.json').write_text(json.dumps(dict(base=BASE,tile_count=len(tiles),surfaces=manifest),indent=2))
print('Nine reworks:',len(tiles),'storage tiles from',len(manifest),'continuous authored surfaces; atlas base',BASE)
