"""Individually authored mining, exchange and maintenance encounters."""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'tools/encounter_panel_design.py';s=p.read_text().replace(" ['tea-room windows with booths', 'galley and crockery lockers', 'barge drive and service access', 'painted cup sign and canopy fabric'],", " ['tea-room windows with booths', 'galley and crockery lockers', 'barge drive and service access', 'painted cup sign and canopy fabric'],\n ['mineral hopper and secured ore', 'crusher drive and auger coupling', 'conveyor guards and inspection doors', 'weighbridge controls and shared crew room'],\n ['parts shop shutters and displayed engines', 'stock lockers and component silhouettes', 'market pressure entrance and windows', 'pitched canopy and illuminated shop signs'],\n ['beacon lens and segmented reflector', 'beacon fins and broken service panel', 'repair tug cab and airlock', 'robot gripper hydraulics and tool reel'],")
s=s.replace(' if f==12:', ''' if f==15:
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
 elif f==12:''');p.write_text(s)
p=root/'tools/generate-encounter-upgrades.py';s=p.read_text().replace('range(15):','range(18):').replace('(32*4,32*15)','(32*4,32*18)').replace('(512,1920)','(512,2304)').replace("'Miners tea barge']", "'Miners tea barge','Cooperative ore depot','Parts exchange','Beacon repair crew']");p.write_text(s)
p=root/'src/encounter-custom.h';s=p.read_text();mark='static int encounter_custom(Vec3 c,int entry,float yaw){'
model='''/* Shared ore equipment anchored to rock. Secured external mineral bays
 * feed enclosed augers; the design does not rely on gravity in open space. */
static void encounter_ore_depot(Vec3 c,float yaw){
 int f=15;unsigned steel=RGB(180,157,113);encounter_salvage_rock(c,yaw);
 encounter_salvage_box(c,(Vec3){0,-350,0},(Vec3){970,75,1000},yaw,steel,f,2);
 for(int k=0;k<3;k++){
  float x=k==2?0:(k?590:-590),z=k==2?600:-450;
  encounter_salvage_box(c,(Vec3){x,-120,z},(Vec3){330,170,350},yaw,steel,f,0);
  for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){x+side*330,90,z},(Vec3){30,80,370},yaw,steel,f,2);
  encounter_salvage_box(c,(Vec3){x,140,z},(Vec3){290,65,310},yaw,steel,f,0);
  encounter_salvage_box(c,(Vec3){x,-230,z-400},(Vec3){125,85,120},yaw,steel,f,1);
 }
 encounter_salvage_box(c,(Vec3){-570,290,760},(Vec3){300,210,240},yaw,steel,f,3);
 encounter_salvage_box(c,(Vec3){0,-180,-1070},(Vec3){780,70,120},yaw,steel,f,2);
 float turn=game.time*.16f;
 discovery_design_ring(add(c,discovery_yaw((Vec3){0,210,-1000},yaw)),280,65,yaw,steel,ENCOUNTER_TILE(f,1),8);
 encounter_arm(c,(Vec3){cosf(turn)*190,210+sinf(turn)*190,-1080},(Vec3){-cosf(turn)*190,210-sinf(turn)*190,-1080},25,yaw,steel);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){-440+k*440,-85,-1210},yaw,RGB(231,174,69));
}
/* A three-front cooperative market. The covered promenade joins real pressure
 * shops, with open display shutters, component racks and roof-mounted signs. */
static void encounter_parts_exchange(Vec3 c,float yaw){
 int f=16;unsigned paint=RGB(177,181,161),trim=RGB(148,118,89);
 encounter_salvage_box(c,(Vec3){0,-330,0},(Vec3){1120,100,950},yaw,trim,f,1);
 for(int k=-1;k<=1;k++){
  float x=k*690,h=k?210:340;
  encounter_salvage_box(c,(Vec3){x,h-170,150},(Vec3){300,h,560},yaw,paint,f,0);
  encounter_salvage_box(c,(Vec3){x,h*2-140,150},(Vec3){330,45,600},yaw,trim,f,3);
  encounter_salvage_box(c,(Vec3){x,h*2-10,-360},(Vec3){200,75,45},yaw,paint,f,3);
  encounter_salvage_box(c,(Vec3){x,-170,-640},(Vec3){250,120,180},yaw,paint,f,2);
 }
 for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){side*350,-50,0},(Vec3){80,100,230},yaw,paint,f,2);
 encounter_salvage_box(c,(Vec3){0,-70,820},(Vec3){600,160,150},yaw,trim,f,1);
 for(int k=0;k<5;k++)encounter_lamp(c,(Vec3){-800+k*400,-175,-860},yaw,((int)(game.time*2)+k)%5==0?RGB(226,176,91):RGB(111,171,168));
}
/* The crew's tug physically grips the failed beacon. Separate service robots
 * carry tool reels; a slow boom movement is an actual mechanical joint. */
static void encounter_beacon_crew(Vec3 c,float yaw){
 int f=17;unsigned paint=RGB(189,193,161),blue=RGB(119,157,177);
 encounter_salvage_box(c,(Vec3){-590,-140,80},(Vec3){360,230,780},yaw,paint,f,2);
 encounter_salvage_box(c,(Vec3){-590,150,-420},(Vec3){310,150,220},yaw,paint,f,2);
 for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){-590+side*390,-200,590},(Vec3){110,140,280},yaw,blue,f,3);
 encounter_salvage_box(c,(Vec3){460,60,0},(Vec3){150,580,150},yaw,blue,f,1);
 for(int side=-1;side<=1;side+=2)encounter_salvage_box(c,(Vec3){460+side*240,150,0},(Vec3){130,320,50},yaw,blue,f,1);
 encounter_salvage_box(c,(Vec3){460,690,0},(Vec3){210,130,210},yaw,paint,f,0);
 Vec3 elbow={-120,340,-230},tool={260,260+sinf(game.time*.18f)*80,-150};
 encounter_arm(c,(Vec3){-260,80,-350},elbow,42,yaw,paint);encounter_arm(c,elbow,tool,32,yaw,paint);
 encounter_salvage_box(c,tool,(Vec3){75,65,75},yaw,paint,f,3);
 for(int k=0;k<2;k++){
  Vec3 p={820,100+k*310,160+k*180};encounter_salvage_box(c,p,(Vec3){95,70,130},yaw,paint,f,3);
  encounter_arm(c,add(p,(Vec3){-80,0,-70}),add(p,(Vec3){-210,40,-150}),14,yaw,paint);
  encounter_lamp(c,add(p,(Vec3){0,0,-140}),yaw,RGB(127,211,224));
 }
 encounter_lamp(c,(Vec3){460,840,0},yaw,((int)(game.time*.7f)&3)==0?RGB(213,153,72):RGB(61,87,82));
}
'''
s=s.replace(mark,model+mark+'\n if(entry==8){encounter_ore_depot(c,yaw);return 1;}\n if(entry==9){encounter_parts_exchange(c,yaw);return 1;}\n if(entry==10){encounter_beacon_crew(c,yaw);return 1;}');p.write_text(s)
p=root/'src/encounter-flight-capture.h';s=p.read_text().replace('"miners tea barge"};','"miners tea barge","cooperative ore depot","parts exchange","beacon repair crew"};').replace('scene>14','scene>17').replace('if(scene==14)discovery_capture_entry=6;','if(scene==14)discovery_capture_entry=6;\n if(scene==15)discovery_capture_entry=8;\n if(scene==16)discovery_capture_entry=9;\n if(scene==17)discovery_capture_entry=10;');p.write_text(s)
print('Ore depot, parts exchange and beacon repair crew installed with twelve dedicated plates')
