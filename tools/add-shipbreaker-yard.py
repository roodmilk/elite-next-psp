"""Add an industrial dismantling dock; keep all existing catalogue selection."""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'tools/encounter_panel_design.py';s=p.read_text().replace(" ['reclaimed cabin with airlock', 'sorted salvage rack', 'cut hull and exposed conduits', 'salvage winch and cable drum'],", " ['reclaimed cabin with airlock', 'sorted salvage rack', 'cut hull and exposed conduits', 'salvage winch and cable drum'],\n ['cut ship armour and oxidised welds', 'gantry rail and hydraulic carriage', 'dismantling bay and tool sockets', 'control gallery and hazard interlock'],")
s=s.replace(' if f==8:', ''' if f==9:
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
 elif f==8:''');p.write_text(s)
p=root/'tools/generate-encounter-upgrades.py';s=p.read_text().replace('range(9):','range(10):').replace('(32*4,32*9)','(32*4,32*10)').replace('(512,1152)','(512,1280)').replace("'Salvage village']", "'Salvage village','Shipbreakers yard']");p.write_text(s)
p=root/'src/encounter-custom.h';s=p.read_text();mark='static int encounter_custom(Vec3 c,int entry,float yaw){'
model='''/* Open-ended dry dock: ship hull fragments are supported by the cradle,
 * rather than indistinguishable free-floating containers. */
static void encounter_shipbreaker(Vec3 c,float yaw){
 int f=9;unsigned steel=RGB(185,177,156),hull=RGB(146,167,183);
 for(int side=-1;side<=1;side+=2){
  encounter_salvage_box(c,(Vec3){side*900,-340,0},(Vec3){80,110,1640},yaw,steel,f,1);
  for(int k=-1;k<=1;k++){
   encounter_salvage_box(c,(Vec3){side*900,160,k*1100},(Vec3){75,450,70},yaw,steel,f,1);
   encounter_salvage_box(c,(Vec3){side*600,650,k*1100},(Vec3){370,60,80},yaw,steel,f,1);
  }
 }
 /* Two uneven separated shell sections, each with an open centre. */
 for(int k=0;k<2;k++){
  float z=-650+k*1250;
  encounter_salvage_box(c,(Vec3){0,-160,z},(Vec3){590,70,380},yaw,hull,f,0);
  for(int side=-1;side<=1;side+=2){
   encounter_salvage_box(c,(Vec3){side*540,70,z},(Vec3){65,230,380},yaw+side*.06f,hull,f,0);
   encounter_arm(c,(Vec3){side*490,-80,z-290},(Vec3){side*370,370,z-290},32,yaw,steel);
  }
 }
 /* Overhead travelling cutting head. Pulsed blue lamps mark active equipment. */
 float z=sinf(game.time*.16f)*820;
 encounter_salvage_box(c,(Vec3){0,650,z},(Vec3){500,65,75},yaw,steel,f,1);
 encounter_arm(c,(Vec3){0,590,z},(Vec3){170,330,z},30,yaw,steel);
 encounter_salvage_box(c,(Vec3){170,300,z},(Vec3){60,65,60},yaw,steel,f,2);
 encounter_lamp(c,(Vec3){170,215,z},yaw,((int)(game.time*7)&1)?RGB(104,223,239):RGB(51,110,140));
 /* Occupied control gallery, isolated from the dismantling volume. */
 encounter_salvage_box(c,(Vec3){1120,170,900},(Vec3){210,220,390},yaw,steel,f,3);
 encounter_salvage_box(c,(Vec3){1080,-240,-780},(Vec3){160,100,540},yaw,steel,f,2);
 for(int k=0;k<3;k++)encounter_lamp(c,(Vec3){(k-1)*360,-220,-1580},yaw,RGB(235,171,65));
}
'''
s=s.replace(mark,model+mark+'\n if(entry==15){encounter_shipbreaker(c,yaw);return 1;}');p.write_text(s)
p=root/'src/encounter-flight-capture.h';s=p.read_text().replace('"salvage village"};','"salvage village","shipbreakers yard"};').replace('scene>8','scene>9').replace('if(scene==8)discovery_capture_entry=1;','if(scene==8)discovery_capture_entry=1;\n if(scene==9)discovery_capture_entry=15;').replace('slot=scene==8?4:','slot=scene>=8?4:');p.write_text(s)
print('Shipbreaker geometry, materials and private capture scene installed')
