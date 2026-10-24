"""Three individually authored civilian encounters; no procedural art variants."""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'tools/encounter_panel_design.py';s=p.read_text().replace(" ['riveted pressure shell and welded patch', 'fuel pump and pressure gauge', 'loading boom rotary coupling', 'operators cabin and illuminated price board'],", " ['riveted pressure shell and welded patch', 'fuel pump and pressure gauge', 'loading boom rotary coupling', 'operators cabin and illuminated price board'],\n ['ferry cabin windows and deck stripe', 'engine housing and exhaust throat', 'boarding lock and mooring clamps', 'bridge glazing and navigation console'],\n ['radio cabin and broadcast display', 'mast insulators and coax connectors', 'receiver dish radial waveguides', 'transmitter cooling and patch leads'],\n ['tea-room windows with booths', 'galley and crockery lockers', 'barge drive and service access', 'painted cup sign and canopy fabric'],")
s=s.replace(' if f==11:', ''' if f==12:
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
 elif f==11:''');p.write_text(s)
p=root/'tools/generate-encounter-upgrades.py';s=p.read_text().replace('range(12):','range(15):').replace('(32*4,32*12)','(32*4,32*15)').replace('(512,1536)','(512,1920)').replace("'Patchwork fuel stop']", "'Patchwork fuel stop','Old passenger ferry','Local radio shack','Miners tea barge']");p.write_text(s)
p=root/'src/encounter-custom.h';s=p.read_text();mark='static int encounter_custom(Vec3 c,int entry,float yaw){'
model='''/* Civilian ferry: tapered eight-sided pressure hull, layered cabin decks,
 * boarding collar, separate engine pods and raised forward bridge. */
static void encounter_passenger_ferry(Vec3 c,float yaw){
 int f=12;unsigned ivory=RGB(192,201,180),blue=RGB(116,155,178);
 const float z[4]={-1350,-800,820,1190},r[4]={.18f,1,1,.64f};
 for(int section=0;section<3;section++)for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;
  Vec3 aa={cosf(a)*540*r[section],sinf(a)*300*r[section],z[section]},bb={cosf(b)*540*r[section],sinf(b)*300*r[section],z[section]},cc={cosf(b)*540*r[section+1],sinf(b)*300*r[section+1],z[section+1]},dd={cosf(a)*540*r[section+1],sinf(a)*300*r[section+1],z[section+1]};
  discovery_design_quad(c,aa,bb,cc,dd,yaw,section==1?ivory:blue,ENCOUNTER_TILE(f,section==1?0:2),0);
 }
 encounter_salvage_box(c,(Vec3){0,340,-740},(Vec3){300,140,220},yaw,ivory,f,3);
 encounter_salvage_box(c,(Vec3){0,0,1190},(Vec3){346,193,35},yaw,blue,f,1);
 encounter_salvage_box(c,(Vec3){0,0,-1320},(Vec3){95,55,35},yaw,blue,f,3);
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*620,-180,650},(Vec3){130,150,460},yaw,blue,f,1);
  encounter_salvage_box(c,(Vec3){side*550,-40,-240},(Vec3){80,190,190},yaw,ivory,f,2);
  encounter_lamp(c,(Vec3){side*770,-170,1120},yaw,RGB(68,142,180));
 }
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*150,495,-740},yaw,RGB(231,187,104));
}
/* Independent neighbourhood broadcaster: compact occupied shack, receiver
 * dish, coax-fed mast, separate transmitter cooling and off-axis solar wing. */
static void encounter_radio_shack(Vec3 c,float yaw){
 int f=13;unsigned paint=RGB(180,194,174);
 encounter_salvage_box(c,(Vec3){0,-130,0},(Vec3){640,280,480},yaw,paint,f,0);
 encounter_salvage_box(c,(Vec3){-450,-440,90},(Vec3){210,80,620},yaw,paint,f,3);
 encounter_salvage_box(c,(Vec3){-380,590,170},(Vec3){40,480,40},yaw,paint,f,1);
 for(int k=0;k<3;k++)encounter_salvage_box(c,(Vec3){-380,480+k*210,170},(Vec3){300-k*65,20,28},yaw,paint,f,1);
 encounter_salvage_box(c,(Vec3){770,-180,150},(Vec3){100,30,650},yaw,paint,f,3);
 /* A concave dish is formed by tapered sectors, with an actual feed horn. */
 Vec3 centre={280,300,0};float turn=.15f*sinf(game.time*.13f);
 for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;
  Vec3 aa=add(centre,(Vec3){cosf(a)*90,sinf(a)*90,0}),bb=add(centre,(Vec3){cosf(a)*440,sinf(a)*440,-130}),cc=add(centre,(Vec3){cosf(b)*440,sinf(b)*440,-130}),dd=add(centre,(Vec3){cosf(b)*90,sinf(b)*90,0});
  discovery_design_quad(c,aa,dd,cc,bb,yaw+turn,paint,ENCOUNTER_TILE(f,2),0);
  discovery_design_quad(c,bb,cc,dd,aa,yaw+turn,paint,ENCOUNTER_TILE(f,2),0);
 }
 encounter_arm(c,(Vec3){280,300,0},(Vec3){280,300,-250},20,yaw+turn,paint);
 encounter_lamp(c,(Vec3){-380,1110,170},yaw,((int)(game.time*2)&1)?RGB(230,119,71):RGB(92,49,32));
}
/* Tea barge: wide glazed mess room, sheltered boarding deck, roof cup sign,
 * galley cupboards and service engines instead of a generic cargo crate. */
static void encounter_tea_barge(Vec3 c,float yaw){
 int f=14;unsigned cream=RGB(202,179,132),teal=RGB(115,164,155);
 encounter_salvage_box(c,(Vec3){0,-320,0},(Vec3){960,110,1100},yaw,teal,f,2);
 encounter_salvage_box(c,(Vec3){0,30,200},(Vec3){750,300,660},yaw,cream,f,0);
 encounter_salvage_box(c,(Vec3){0,370,120},(Vec3){850,45,800},yaw,teal,f,3);
 encounter_salvage_box(c,(Vec3){0,-60,-810},(Vec3){650,170,180},yaw,cream,f,1);
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*710,-230,900},(Vec3){150,120,300},yaw,teal,f,2);
  encounter_salvage_box(c,(Vec3){side*750,90,-810},(Vec3){35,300,35},yaw,cream,f,1);
 }
 /* A small illuminated cup-shaped advertising fixture sits on a real post. */
 encounter_salvage_box(c,(Vec3){0,540,100},(Vec3){25,130,25},yaw,cream,f,1);
 encounter_salvage_box(c,(Vec3){0,760,100},(Vec3){200,115,55},yaw,cream,f,3);
 encounter_arm(c,(Vec3){200,830,100},(Vec3){310,830,100},25,yaw,cream);
 encounter_arm(c,(Vec3){310,830,100},(Vec3){310,710,100},25,yaw,cream);
 encounter_arm(c,(Vec3){310,710,100},(Vec3){200,710,100},25,yaw,cream);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-600+k*400,420,-700},yaw,RGB(235,189,111));
 encounter_lamp(c,(Vec3){-60,920,90},yaw,((int)(game.time*1.4f)&1)?RGB(181,225,218):RGB(94,136,129));
}
'''
s=s.replace(mark,model+mark+'\n if(entry==4){encounter_passenger_ferry(c,yaw);return 1;}\n if(entry==14){encounter_radio_shack(c,yaw);return 1;}\n if(entry==6){encounter_tea_barge(c,yaw);return 1;}');p.write_text(s)
p=root/'src/encounter-flight-capture.h';s=p.read_text().replace('"patchwork fuel stop"};','"patchwork fuel stop","old passenger ferry","local radio shack","miners tea barge"};').replace('scene>11','scene>14').replace('if(scene==11)discovery_capture_entry=0;','if(scene==11)discovery_capture_entry=0;\n if(scene==12)discovery_capture_entry=4;\n if(scene==13)discovery_capture_entry=14;\n if(scene==14)discovery_capture_entry=6;');p.write_text(s)
print('Old passenger ferry, local radio shack and miners tea barge installed with individual geometry and twelve dedicated plates')
