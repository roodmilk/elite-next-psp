"""Purpose-built 32px panel art. Each role has a different physical job.
No external texture photographs or generated art are used.
"""
from PIL import Image, ImageDraw

ROLES = [
 ['receiver electronics', 'waveguide radiator', 'dish segments', 'power distribution'],
 ['pressure hatch', 'thermal radiator', 'habitation windows', 'cargo service door'],
 ['sealed habitat cylinder', 'structural restraint', 'inspection hatch', 'life support pump'],
 ['greenhouse glass', 'thermal fins', 'beacon controller', 'water and oxygen plant'],
 ['film poster', 'screen support', 'marquee lamps', 'projector housing'],
 ['shuttered guest rooms', 'balcony service deck', 'observation wheel', 'closed atrium entrance'],
 ['repair tools and lockers', 'crane hydraulics', 'workshop windows', 'repair deck'],
 ['fractured basalt', 'mineral seams', 'weathered strata', 'cut foundation rock'],
 ['reclaimed cabin with airlock', 'sorted salvage rack', 'cut hull and exposed conduits', 'salvage winch and cable drum'],
 ['cut ship armour and oxidised welds', 'gantry rail and hydraulic carriage', 'dismantling bay and tool sockets', 'control gallery and hazard interlock'],
 ['glazed crop trays and irrigation', 'water recovery and nutrient pumps', 'pressure vestibule and service hatch', 'ribbed heat exchanger and solar cells'],
 ['riveted pressure shell and welded patch', 'fuel pump and pressure gauge', 'loading boom rotary coupling', 'operators cabin and illuminated price board'],
 ['ferry cabin windows and deck stripe', 'engine housing and exhaust throat', 'boarding lock and mooring clamps', 'bridge glazing and navigation console'],
 ['radio cabin and broadcast display', 'mast insulators and coax connectors', 'receiver dish radial waveguides', 'transmitter cooling and patch leads'],
 ['tea-room windows with booths', 'galley and crockery lockers', 'barge drive and service access', 'painted cup sign and canopy fabric'],
 ['mineral hopper and secured ore', 'crusher drive and auger coupling', 'conveyor guards and inspection doors', 'weighbridge controls and shared crew room'],
 ['parts shop shutters and displayed engines', 'stock lockers and component silhouettes', 'market pressure entrance and windows', 'pitched canopy and illuminated shop signs'],
 ['beacon lens and segmented reflector', 'beacon fins and broken service panel', 'repair tug cab and airlock', 'robot gripper hydraulics and tool reel'],
]

def panel(f, r):
 im=Image.new('P',(32,32),2); d=ImageDraw.Draw(im)
 if f==7:
  # Broad rock planes and connected fractures, no industrial frames or lamps.
  d.polygon([(0,0),(25,0),(18,10),(0,16)],fill=3)
  d.polygon([(0,25),(14,15),(31,19),(31,31),(0,31)],fill=1)
  for points in ([(0,18),(9,14),(15,18),(23,8),(31,5)],[(7,0),(10,8),(8,14)],[(15,18),(17,26),(26,31)]):
   d.line(points,fill=0,width=1)
   d.line([(x,y+1) for x,y in points],fill=2,width=1)
  for x,y in ((5,5),(19,7),(25,24),(9,25)):
   d.line((x,y,x+3,y-1),fill=3 if r&1 else 2)
  if r==3:d.line((0,9,31,12),fill=1);d.line((0,12,31,15),fill=3)
  return im
 # Quiet metal field with a single recessed seam and sparse fasteners.
 d.rectangle((0,0,31,31),outline=0)
 d.line((1,1,30,1),fill=3);d.line((1,30,30,30),fill=1)
 for x,y in ((2,2),(29,2),(2,29),(29,29)):d.point((x,y),fill=3)
 def recess(box):
  x,y,a,b=box;d.rectangle(box,fill=0);d.rectangle((x+1,y+1,a-1,b-1),fill=1);d.line((x,b,a,b),fill=3)
 def vents(x,y,w,h):
  recess((x,y,x+w,y+h))
  for yy in range(y+2,y+h,3):d.line((x+2,yy,x+w-2,yy),fill=3)
 def glass(box,lit=False):
  recess(box);x,y,a,b=box;d.rectangle((x+2,y+2,a-2,b-2),fill=8 if lit else 7)
  d.line((x+2,y+2,a-3,y+2),fill=9 if lit else 3)
 def pipe(points):
  d.line(points,fill=0,width=3);d.line(points,fill=3,width=1)
 def hazard(y):
  d.rectangle((4,y,27,y+2),fill=0)
  for x in range(5,27,5):d.line((x,y,x+2,y+2),fill=5)
 if f==15:
  if r==0:
   recess((3,3,28,28))
   for points in ([(5,23),(7,13),(13,10),(18,15),(16,25)],[(15,23),(18,7),(25,5),(27,15),(24,26)],[(5,11),(8,5),(14,6),(12,13)]):
    d.polygon(points,fill=12);d.line(points[:3],fill=3,width=1)
   for y in (7,24):d.line((3,y,28,y),fill=5)
   for x in (4,27):d.line((x,4,x,27),fill=13)
  elif r==1:
   recess((3,3,28,28));d.ellipse((5,5,26,26),fill=3)
   d.ellipse((8,8,23,23),fill=1)
   for k in range(5):d.line((10+k*2,11,10+k*2,21),fill=12 if k&1 else 4)
   for x,y in ((6,6),(24,6),(6,24),(24,24)):d.point((x,y),fill=5)
  elif r==2:
   for y in (4,16):
    recess((3,y,28,y+10));d.rectangle((6,y+3,24,y+7),fill=1)
    for x in range(7,24,4):d.line((x,y+3,x+2,y+7),fill=3)
   hazard(2);hazard(29)
  else:
   glass((3,4,28,14),True);d.line((15,6,15,12),fill=3)
   recess((4,18,19,28));d.rectangle((6,20,17,24),fill=7)
   for x in (7,10,13):d.line((x,21,x,23),fill=10)
   d.rectangle((23,20,27,26),fill=12);d.point((25,22),fill=5)
 elif f==16:
  if r==0:
   recess((3,3,28,28));d.rectangle((5,5,26,24),fill=0)
   for y in (5,8,11):d.line((5,y,26,y),fill=3)
   # Display engines on the open shelf: casing, central bore, mounting feet.
   for x in (8,20):
    d.rectangle((x-3,16,x+3,22),fill=12);d.ellipse((x-2,17,x+2,21),fill=3)
    d.point((x,19),fill=0);d.line((x-4,24,x+4,24),fill=4)
   d.rectangle((6,26,13,28),fill=5)
  elif r==1:
   for y in (4,15):
    for x in (4,16):
     recess((x,y,x+10,y+10));d.line((x+2,y+3,x+8,y+3),fill=4)
     d.rectangle((x+3,y+5,x+7,y+8),fill=12);d.point((x+7,y+6),fill=5)
  elif r==2:
   recess((4,3,27,28));glass((7,6,24,14),True)
   d.line((15,6,15,25),fill=3);d.line((7,25,24,25),fill=4)
   d.rectangle((19,18,23,20),fill=5);d.point((28,16),fill=10)
  else:
   for x in (3,11,19,27):d.line((x,3,x,28),fill=3)
   d.rectangle((4,5,27,14),fill=0)
   for x in (7,15,23):
    d.ellipse((x-2,7,x+2,11),fill=5);d.point((x,9),fill=0)
   d.line((2,24,29,24),fill=4);d.line((3,27,28,27),fill=12)
 elif f==17:
  if r==0:
   recess((3,3,28,28));d.ellipse((5,5,26,26),fill=3)
   d.ellipse((8,8,23,23),fill=8);d.ellipse((11,11,20,20),fill=9)
   d.line((14,9,14,21),fill=4);d.line((10,14,21,14),fill=4)
  elif r==1:
   for x in (5,13,21):
    d.rectangle((x,3,x+5,28),fill=1);d.line((x+1,5,x+1,26),fill=3)
   d.polygon([(15,14),(28,19),(26,28),(14,26)],fill=0)
   pipe([(18,17),(22,18),(22,24)]);d.point((25,20),fill=5)
  elif r==2:
   glass((3,3,28,13),True);d.line((16,5,16,11),fill=3)
   recess((5,18,17,28));d.line((11,20,11,26),fill=3)
   vents(21,18,7,10);d.point((26,15),fill=10)
  else:
   recess((3,3,28,28));d.rectangle((6,5,12,26),fill=12)
   d.rectangle((8,7,10,22),fill=4);d.ellipse((15,8,26,22),fill=3)
   for x in (18,21,24):d.line((x,11,x,19),fill=1)
   hazard(27)
 elif f==12:
  if r==0:
   for y in (5,17):
    for x in (4,13,22):
     glass((x,y,x+6,y+7),True)
     d.line((x+2,y+5,x+4,y+5),fill=5)
   d.line((2,14,29,14),fill=5);d.line((2,29,29,29),fill=12)
  elif r==1:
   recess((3,3,28,28));d.ellipse((6,5,25,26),fill=3)
   d.ellipse((8,7,23,24),fill=1);d.ellipse((11,10,20,21),fill=0)
   for x in (12,15,18):d.line((x,12,x,19),fill=8)
   for y in (5,25):d.line((2,y,7,y),fill=5)
  elif r==2:
   recess((6,3,25,28));d.rectangle((9,5,22,26),fill=3)
   d.rectangle((12,8,19,12),fill=7);d.line((14,16,19,16),fill=1)
   d.line((16,17,16,23),fill=1);hazard(27)
   for y in (7,22):d.rectangle((3,y,6,y+3),fill=12)
  else:
   glass((3,4,28,17),True)
   for x in (10,21):d.line((x,6,x,15),fill=3)
   d.rectangle((4,22,27,27),fill=1)
   for x in range(6,27,4):d.point((x,24),fill=10 if x%3 else 5)
 elif f==13:
  if r==0:
   glass((3,3,28,12),True);recess((4,17,27,28))
   for x,h in ((6,3),(10,7),(14,5),(18,8),(22,4)):
    d.rectangle((x,26-h,x+1,26),fill=10 if x%3 else 5)
   d.line((15,5,15,10),fill=3)
  elif r==1:
   d.rectangle((13,3,18,28),fill=12)
   for y in (5,11,17,23):d.rectangle((9,y,22,y+2),fill=3);d.line((10,y,21,y),fill=4)
   pipe([(25,3),(25,25),(19,25)])
  elif r==2:
   d.polygon([(3,28),(10,3),(21,3),(28,28)],fill=3)
   for y in (8,15,23):d.line((7,y,24,y),fill=1)
   d.line((15,4,15,27),fill=4);d.rectangle((12,20,19,26),fill=8)
  else:
   vents(3,3,13,24);recess((20,4,28,17));d.point((24,8),fill=10)
   pipe([(20,21),(25,21),(25,28),(17,28)])
 elif f==14:
  if r==0:
   glass((3,3,28,27),True)
   for x in (7,20):
    d.rectangle((x,16,x+4,23),fill=12)
    d.line((x+1,16,x+3,16),fill=5)
    d.line((x-1,20,x+5,20),fill=4)
    d.rectangle((x+1,17,x+2,18),fill=14)
   d.line((15,5,15,25),fill=3)
  elif r==1:
   for y in (4,15):
    recess((3,y,28,y+9))
    for x in (6,15,23):
     d.rectangle((x,y+2,x+3,y+5),fill=14);d.point((x+4,y+3),fill=3)
   vents(4,26,23,3)
  elif r==2:
   recess((3,3,28,28));d.ellipse((7,5,24,23),fill=1)
   for x in (11,15,19):d.line((x,9,x,19),fill=8)
   d.line((5,27,26,27),fill=5);pipe([(3,4),(5,4),(5,22)])
  else:
   d.rectangle((3,3,28,28),fill=7)
   for x in (5,14,23):d.rectangle((x,4,x+3,27),fill=3)
   d.rectangle((8,12,20,22),fill=14);d.arc((17,12,26,20),270,90,fill=14,width=2)
   d.line((7,24,23,24),fill=5)
   for x in (11,17):d.line((x,6,x-1,10),fill=9)
 elif f==11:
  if r==0:
   for y in (7,23):
    d.line((2,y,29,y),fill=0);d.line((2,y+1,29,y+1),fill=4)
   d.rectangle((17,11,28,21),fill=12);d.rectangle((19,12,27,20),fill=3)
   for x in (4,14):
    for y in (4,27):d.point((x,y),fill=4)
   d.rectangle((5,11,12,19),fill=1);d.line((7,13,10,13),fill=5)
   pipe([(2,28),(10,28),(10,25),(28,25)])
  elif r==1:
   recess((3,3,28,28));d.ellipse((6,5,18,17),fill=3)
   d.ellipse((8,7,16,15),fill=0);d.line((12,11,15,9),fill=5)
   d.rectangle((21,5,25,18),fill=1);d.line((22,7,22,15),fill=10)
   pipe([(8,21),(8,26),(24,26),(24,20)])
   d.rectangle((11,19,18,24),fill=12);d.point((15,21),fill=4)
  elif r==2:
   recess((3,3,28,28));d.ellipse((5,5,26,26),fill=3)
   d.ellipse((9,9,22,22),fill=1);d.ellipse((12,12,19,19),fill=0)
   for x,y in ((9,8),(23,10),(8,22),(22,23)):d.rectangle((x,y,x+2,y+2),fill=4)
   hazard(2);hazard(28)
  else:
   glass((3,4,28,13),True);d.line((16,6,16,11),fill=1)
   recess((4,18,20,27))
   # Deliberate pixel digit shapes, not illegible generated text.
   for x in (6,12,17):
    d.line((x,20,x+3,20),fill=5);d.line((x+3,20,x+3,24),fill=5)
    d.line((x,22,x+3,22),fill=5);d.line((x,24,x+3,24),fill=5)
   d.rectangle((24,19,27,26),fill=1);d.point((25,21),fill=10)
 elif f==10:
  if r==0:
   # Readable plant silhouettes behind pressure glazing, three growth tiers.
   d.rectangle((2,2,29,29),fill=7)
   for y in (4,13,22):
    d.rectangle((4,y,27,y+6),fill=6)
    d.line((4,y+6,27,y+6),fill=3)
    for x in (7,14,22):
     d.line((x,y+1,x,y+5),fill=14)
     d.polygon([(x,y+3),(x-3,y+1),(x-2,y+4),(x,y+5),(x+3,y+2),(x+1,y+1)],fill=14)
     d.point((x,y+2),fill=4)
    d.line((5,y,26,y),fill=9)
   d.line((15,3,15,28),fill=3)
   d.line((3,3,3,27),fill=4);d.line((4,4,11,4),fill=9)
  elif r==1:
   for x in (5,19):
    recess((x,4,x+8,22));d.ellipse((x+1,5,x+7,11),fill=3)
    d.rectangle((x+2,9,x+6,19),fill=2);d.line((x+2,10,x+2,18),fill=4)
   pipe([(9,23),(9,27),(23,27),(23,23)]);d.rectangle((12,10,16,19),fill=0)
   for y in (12,16):d.point((14,y),fill=10)
  elif r==2:
   recess((5,3,26,28));d.rectangle((8,5,23,26),fill=3)
   d.rectangle((11,7,20,13),fill=7);d.line((12,8,19,8),fill=9)
   d.line((16,15,16,24),fill=1);d.rectangle((19,18,22,20),fill=5)
   for y in (6,22):d.rectangle((4,y,7,y+3),fill=12)
   d.point((27,15),fill=10)
  else:
   recess((3,3,28,28))
   for y in range(5,28,4):
    for x in (5,17):
     d.rectangle((x,y,x+9,y+2),fill=8);d.line((x+1,y,x+8,y),fill=3)
   d.line((15,4,15,27),fill=4)
 elif f==9:
  if r==0:
   d.rectangle((2,3,29,28),fill=3)
   for y in (4,15):
    recess((3,y,28,y+10));d.rectangle((5,y+2,25,y+7),fill=2)
    d.line((7,y+3,21,y+3),fill=4);d.line((6,y+8,25,y+8),fill=12)
   d.polygon([(20,0),(31,0),(31,31),(25,31),(26,22),(22,18),(26,11)],fill=0)
   for y in (7,19,26):d.line((23,y,29,y+1),fill=5)
  elif r==1:
   recess((3,3,28,28))
   for x in (6,24):d.rectangle((x,4,x+2,27),fill=13)
   for y in range(6,28,4):d.line((10,y,21,y),fill=3)
   d.rectangle((4,12,27,19),fill=2);d.line((5,12,26,12),fill=4)
   d.rectangle((13,13,19,17),fill=5);d.point((15,15),fill=0)
  elif r==2:
   recess((4,4,27,27));d.rectangle((7,7,24,24),fill=0)
   for x in (9,17):
    for y in (9,17):d.ellipse((x,y,x+5,y+5),fill=12);d.point((x+2,y+2),fill=3)
   hazard(3);hazard(27);pipe([(3,10),(5,10),(5,23),(3,23)])
  else:
   glass((3,4,28,13),True)
   for x in (11,20):d.line((x,6,x,11),fill=3)
   recess((4,18,16,27));d.rectangle((6,20,13,24),fill=2)
   for x in (20,24,28):d.point((x,20),fill=5);d.point((x,25),fill=10)
 elif f==8:
  if r==0:
   # Asymmetric reclaimed pressure cabin: airlock, glazing, oxidised repairs.
   recess((3,4,14,26));d.rectangle((5,6,12,24),fill=2)
   d.rectangle((7,9,10,13),fill=7);d.line((7,9,10,9),fill=9)
   d.line((10,18,12,18),fill=5);hazard(27)
   for y in (5,12):glass((18,y,28,y+5),True)
   vents(18,21,10,5);pipe([(15,3),(15,18),(28,18)])
   for x,y in ((3,16),(17,28),(25,2)):d.line((x,y,x+3,y),fill=5)
  elif r==1:
   # Three distinct storage bays, secured by straps rather than random marks.
   for y in (4,13,22):
    recess((3,y,28,y+7))
    for x in (5,13,21):
     d.rectangle((x,y+2,x+5,y+5),fill=12)
     d.line((x,y+2,x+4,y+2),fill=13)
     d.line((x+2,y+1,x+2,y+6),fill=5)
  elif r==2:
   d.polygon([(3,3),(27,3),(27,12),(22,16),(28,23),(27,28),(3,28)],fill=3)
   d.polygon([(4,5),(24,5),(24,11),(18,15),(24,22),(24,26),(4,26)],fill=1)
   for y in (8,13,21):pipe([(5,y),(12,y),(15,y+3)])
   d.rectangle((5,23,12,25),fill=12)
   for y in (5,26):d.line((3,y,20,y),fill=4)
  else:
   recess((3,5,28,26));d.ellipse((6,8,24,24),fill=0)
   d.ellipse((8,9,22,23),fill=3);d.ellipse((11,12,19,20),fill=1)
   for x in (11,14,17):d.line((x,12,x,20),fill=12)
   d.line((19,16,28,16),fill=4);hazard(3);hazard(27)
 elif f==0:
  if r==0:
   for y in (5,14):
    recess((5,y,24,y+7));d.rectangle((7,y+2,15,y+4),fill=7)
    d.line((8,y+2,14,y+2),fill=8);d.point((21,y+3),fill=10)
   pipe([(6,25),(18,25),(18,23),(27,23)]);d.rectangle((26,6,28,19),fill=1)
  elif r==1:
   vents(4,4,23,23);d.line((15,6,15,25),fill=0)
  elif r==2:
   # Radial ribs follow the dish's sector UV rather than a generic box.
   d.polygon([(2,29),(11,3),(20,3),(29,29)],fill=3)
   d.polygon([(5,27),(12,4),(19,4),(26,27)],fill=2)
   for y in (10,18,25):d.line((8,y,24,y),fill=1)
   d.line((15,4,15,28),fill=3)
  else:
   vents(4,4,10,12);recess((18,4,27,13));d.point((24,7),fill=10)
   pipe([(8,19),(8,25),(25,25),(25,18)]);d.rectangle((15,18,20,21),fill=5)
 elif f==1:
  if r==0:
   recess((7,4,24,27));d.rectangle((9,6,22,25),fill=2)
   d.line((16,7,16,25),fill=0);d.line((19,14,21,14),fill=5)
   d.rectangle((3,10,5,18),fill=1);d.point((4,12),fill=10);hazard(27)
  elif r==1:vents(4,4,23,23)
  elif r==2:
   for y in (6,18):
    for x in (4,14,24):glass((x,y,min(x+5,29),y+6),x==14)
   d.line((4,15,28,15),fill=1)
  else:
   recess((4,5,27,25))
   for y in range(7,25,3):d.line((6,y,25,y),fill=3)
   d.rectangle((22,10,25,15),fill=1);hazard(26)
 elif f==2:
  if r==0:
   # Cylinder seam, clamp bands, dark inspection porthole and breathing plant.
   for x in (4,26):d.rectangle((x,2,x+1,29),fill=3)
   glass((9,8,22,19));d.line((15,10,15,17),fill=1)
   vents(10,23,11,5);d.rectangle((12,3,19,4),fill=5)
  elif r==1:
   d.rectangle((12,2,19,29),fill=1);d.line((13,2,13,29),fill=3)
   for y in (7,23):d.rectangle((7,y,24,y+2),fill=3);d.point((9,y+1),fill=0);d.point((22,y+1),fill=0)
  elif r==2:
   recess((6,5,25,26));d.rectangle((8,7,23,24),fill=2)
   glass((11,10,20,17));d.line((11,21,19,21),fill=5)
  else:
   vents(4,5,10,17);pipe([(19,4),(19,25),(27,25)])
   for y in (8,19):d.ellipse((17,y,25,y+6),fill=1,outline=3);d.point((21,y+3),fill=5)
 elif f==3:
  if r==0:
   glass((4,4,27,27));d.line((15,6,15,25),fill=3);d.line((6,16,25,16),fill=3)
   for x,y in ((8,10),(20,10),(8,21),(20,21)):
    d.rectangle((x-1,y+1,x+2,y+3),fill=1);d.line((x,y,x,y-3),fill=11);d.line((x-2,y-2,x+2,y-2),fill=11)
  elif r==1:vents(4,4,23,23)
  elif r==2:
   glass((6,5,25,18),True);d.line((9,14,12,10,15,13,20,8),fill=10)
   for x in (8,15,22):d.rectangle((x,23,x+2,25),fill=5)
  else:
   pipe([(5,6),(5,24),(25,24),(25,8)])
   for x in (9,18):d.rectangle((x,6,x+5,20),fill=1,outline=3);d.line((x+2,8,x+2,17),fill=8)
 elif f==4:
  if r==0:
   d.rectangle((3,3,28,28),fill=0);d.rectangle((5,5,26,26),fill=7)
   d.ellipse((7,7,14,14),fill=5);d.polygon([(8,25),(18,12),(24,25)],fill=8)
   d.ellipse((15,13,21,19),fill=9);d.rectangle((16,16,20,17),fill=0)
   d.rectangle((15,20,22,25),fill=3)
   for x,y in ((20,7),(24,11),(8,18)):d.point((x,y),fill=9)
  elif r==1:
   d.rectangle((12,2,19,29),fill=1);d.line((13,3,13,28),fill=3)
   for y in (7,23):d.rectangle((5,y,26,y+2),fill=3)
  elif r==2:
   recess((3,9,28,22))
   for x in range(5,28,4):d.rectangle((x,11,x+1,12),fill=5);d.rectangle((x,19,x+1,20),fill=9)
   d.line((6,16,25,16),fill=3)
  else:
   vents(4,5,10,21);glass((18,9,27,20),True);d.rectangle((21,12,25,17),fill=0)
 elif f==5:
  if r==0:
   for x in (5,18):
    recess((x,5,x+8,24))
    for y in range(7,23,3):d.line((x+2,y,x+6,y),fill=3)
    d.line((x+1,25,x+9,25),fill=1)
   d.line((3,28,29,28),fill=3)
  elif r==1:
   d.line((3,10,28,10),fill=3);d.line((3,22,28,22),fill=1)
   for x in range(5,29,6):d.line((x,11,x,21),fill=1)
   vents(8,3,16,4)
  elif r==2:
   glass((4,6,27,25));d.line((15,8,15,23),fill=3)
   d.line((6,15,25,15),fill=1);d.rectangle((7,21,12,23),fill=2)
  else:
   recess((5,4,26,27));d.line((15,6,15,25),fill=3)
   for y in range(8,25,4):d.line((7,y,24,y),fill=1)
   d.rectangle((9,13,22,18),fill=0);d.line((11,15,20,15),fill=5)
 else:
  if r==0:
   recess((4,4,18,26))
   for y in (7,13,19):d.rectangle((6,y,16,y+4),fill=2);d.line((11,y+2,14,y+2),fill=5)
   pipe([(23,5),(23,20),(27,20)]);d.rectangle((21,23,27,26),fill=1)
  elif r==1:
   d.rectangle((11,3,20,28),fill=1);d.rectangle((14,5,17,23),fill=3)
   for y in (4,24):d.rectangle((8,y,23,y+3),fill=2);d.line((10,y,21,y),fill=5)
  elif r==2:
   glass((4,5,27,16),True);d.line((15,7,15,14),fill=1)
   vents(4,21,14,6);d.rectangle((23,21,27,26),fill=1);d.point((25,23),fill=10)
  else:
   d.rectangle((5,5,26,26),fill=1)
   for y in (10,21):d.line((7,y,24,y),fill=3)
   for x in (7,24):d.line((x,7,x,24),fill=2)
   hazard(4);hazard(26)
 return im
