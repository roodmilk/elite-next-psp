from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'tools/encounter_panel_design.py';s=p.read_text().replace(" ['glazed crop trays and irrigation', 'water recovery and nutrient pumps', 'pressure vestibule and service hatch', 'ribbed heat exchanger and solar cells'],", " ['glazed crop trays and irrigation', 'water recovery and nutrient pumps', 'pressure vestibule and service hatch', 'ribbed heat exchanger and solar cells'],\n ['riveted pressure shell and welded patch', 'fuel pump and pressure gauge', 'loading boom rotary coupling', 'operators cabin and illuminated price board'],")
s=s.replace(' if f==10:', ''' if f==11:
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
 elif f==10:''');p.write_text(s)
p=root/'tools/generate-encounter-upgrades.py';s=p.read_text().replace('range(11):','range(12):').replace('(32*4,32*11)','(32*4,32*12)').replace('(512,1408)','(512,1536)').replace("'Community greenhouse']", "'Community greenhouse','Patchwork fuel stop']");p.write_text(s)
p=root/'src/encounter-custom.h';s=p.read_text();mark='static int encounter_custom(Vec3 c,int entry,float yaw){'
model='''/* Repaired fuel tender: two pressure vessels on a structural spine,
 * isolated operator cabin, pump skids and an articulated loading coupling. */
static void encounter_fuel_stop(Vec3 c,float yaw){
 int f=11;unsigned paint=RGB(187,169,125),patch=RGB(146,177,185);
 encounter_salvage_box(c,(Vec3){0,-340,0},(Vec3){940,95,1070},yaw,paint,f,0);
 for(int side=-1;side<=1;side+=2){
  encounter_tank(c,(Vec3){side*570,0,210},320,810,yaw,side>0?patch:paint,f);
  for(int k=-1;k<=1;k++)encounter_salvage_box(c,(Vec3){side*570,-320,k*660+200},(Vec3){360,70,65},yaw,paint,f,2);
  encounter_salvage_box(c,(Vec3){side*260,-30,-760},(Vec3){170,210,170},yaw,paint,f,1);
 }
 encounter_salvage_box(c,(Vec3){0,60,1000},(Vec3){350,270,240},yaw,patch,f,3);
 encounter_salvage_box(c,(Vec3){0,410,1030},(Vec3){260,80,85},yaw,paint,f,3);
 /* Rigid articulated hose boom; a slow swivel suggests a serviced berth. */
 float sway=sinf(game.time*.15f)*100;Vec3 shoulder={-900,-100,-700},joint={-1130,380,-760},nozzle={-620+sway,540,-1050};
 encounter_arm(c,shoulder,joint,45,yaw,paint);encounter_arm(c,joint,nozzle,33,yaw,patch);
 encounter_salvage_box(c,nozzle,(Vec3){95,85,70},yaw,paint,f,2);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-410+k*270,-220,-1100},yaw,((int)(game.time*2)+k)%4==0?RGB(101,225,199):RGB(51,111,103));
 encounter_lamp(c,(Vec3){0,525,1030},yaw,RGB(237,173,69));
}
'''
s=s.replace(mark,model+mark+'\n if(entry==0){encounter_fuel_stop(c,yaw);return 1;}');p.write_text(s)
p=root/'src/encounter-flight-capture.h';s=p.read_text().replace('"community greenhouse"};','"community greenhouse","patchwork fuel stop"};').replace('scene>10','scene>11').replace('if(scene==10)discovery_capture_entry=3;','if(scene==10)discovery_capture_entry=3;\n if(scene==11)discovery_capture_entry=0;');p.write_text(s)
print('Patchwork fuel stop: unique pressure-shell, pump, coupling and cabin materials installed')
