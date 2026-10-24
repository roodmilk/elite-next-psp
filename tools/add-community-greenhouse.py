"""A pressure greenhouse with curved glazed growing bays and life support."""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'tools/encounter_panel_design.py';s=p.read_text().replace(" ['cut ship armour and oxidised welds', 'gantry rail and hydraulic carriage', 'dismantling bay and tool sockets', 'control gallery and hazard interlock'],", " ['cut ship armour and oxidised welds', 'gantry rail and hydraulic carriage', 'dismantling bay and tool sockets', 'control gallery and hazard interlock'],\n ['glazed crop trays and irrigation', 'water recovery and nutrient pumps', 'pressure vestibule and service hatch', 'ribbed heat exchanger and solar cells'],")
s=s.replace(' if f==9:', ''' if f==10:
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
 elif f==9:''');p.write_text(s)
p=root/'tools/generate-encounter-upgrades.py';s=p.read_text().replace('range(10):','range(11):').replace('(32*4,32*10)','(32*4,32*11)').replace('(512,1280)','(512,1408)').replace("'Shipbreakers yard']", "'Shipbreakers yard','Community greenhouse']");p.write_text(s)
p=root/'src/encounter-custom.h';s=p.read_text();mark='static int encounter_custom(Vec3 c,int entry,float yaw){'
model='''/* Three vaulted pressure conservatories: framed glazing carries crop art,
 * opaque insulated floor and a real vestibule close the inhabited volume. */
static void encounter_greenhouse(Vec3 c,float yaw){
 int f=10;unsigned ivory=RGB(194,214,186),green=RGB(137,196,161);
 encounter_salvage_box(c,(Vec3){0,-230,0},(Vec3){1060,95,870},yaw,ivory,f,1);
 for(int bay=-1;bay<=1;bay++){
  float x=bay*690;
  for(int k=0;k<6;k++){
   float a=k*.52359878f,b=(k+1)*.52359878f;
   Vec3 aa={x+cosf(a)*300,-120+sinf(a)*430,-700},bb={x+cosf(b)*300,-120+sinf(b)*430,-700};
   Vec3 cc=bb,dd=aa;cc.z=700;dd.z=700;
   discovery_design_quad(c,aa,bb,cc,dd,yaw,green,ENCOUNTER_TILE(f,0),0);
   Vec3 front={x,-120,-700},back={x,-120,700};
   discovery_design_quad(c,front,bb,aa,front,yaw,ivory,ENCOUNTER_TILE(f,2),0);
   discovery_design_quad(c,back,dd,cc,back,yaw,ivory,ENCOUNTER_TILE(f,2),0);
  }
  encounter_salvage_box(c,(Vec3){x,-30,-810},(Vec3){130,160,120},yaw,ivory,f,2);
 }
 /* The shaded underside holds water recovery and a paired radiator bank. */
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*650,-450,0},(Vec3){260,150,430},yaw,ivory,f,1);
  encounter_salvage_box(c,(Vec3){side*1170,-260,0},(Vec3){110,30,680},yaw,ivory,f,3);
 }
 encounter_salvage_box(c,(Vec3){0,-470,820},(Vec3){240,160,180},yaw,ivory,f,2);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*690,340,0},yaw,((int)(game.time*.7f)+k)%3?RGB(73,142,98):RGB(136,225,174));
}
'''
s=s.replace(mark,model+mark+'\n if(entry==3){encounter_greenhouse(c,yaw);return 1;}');p.write_text(s)
p=root/'src/encounter-flight-capture.h';s=p.read_text().replace('"shipbreakers yard"};','"shipbreakers yard","community greenhouse"};').replace('scene>9','scene>10').replace('if(scene==9)discovery_capture_entry=15;','if(scene==9)discovery_capture_entry=15;\n if(scene==10)discovery_capture_entry=3;');p.write_text(s)
print('Community greenhouse geometry, crop plates, life support and private capture installed')
