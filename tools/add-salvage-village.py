"""Install the first catalogue continuation without changing encounter placement."""
from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'tools/encounter_panel_design.py'
s=p.read_text().replace(" ['fractured basalt', 'mineral seams', 'weathered strata', 'cut foundation rock'],", " ['fractured basalt', 'mineral seams', 'weathered strata', 'cut foundation rock'],\n ['reclaimed cabin with airlock', 'sorted salvage rack', 'cut hull and exposed conduits', 'salvage winch and cable drum'],")
s=s.replace(' if f==0:', ''' if f==8:
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
 elif f==0:''')
p.write_text(s)
p=root/'tools/generate-encounter-upgrades.py';s=p.read_text().replace('range(8):','range(9):').replace('(32*4,32*8)','(32*4,32*9)').replace('(512,1024)','(512,1152)').replace("'Asteroid foundation']", "'Asteroid foundation','Salvage village']");p.write_text(s)
p=root/'src/encounter-custom.h';s=p.read_text();marker='static int encounter_custom(Vec3 c,int entry,float yaw){'
model='''/* Salvage village: inhabited reclaimed hulls anchored to a worked asteroid.
 * Open sorting courtyard, pressure corridors and a moving recovery winch. */
static void encounter_salvage_village(Vec3 c,float yaw){
 unsigned hull=RGB(139,145,132),rust=RGB(154,112,76);int f=8;
 encounter_asteroid_foundation(c,yaw);
 encounter_box(c,(Vec3){0,-380,0},(Vec3){860,60,920},yaw,rust,f,2);
 /* An offset bridge connects two salvaged pressure vessels. */
 encounter_tank(c,(Vec3){-610,-10,60},250,680,yaw,hull,f);
 encounter_box(c,(Vec3){580,-70,360},(Vec3){280,240,410},yaw,hull,f,0);
 encounter_box(c,(Vec3){0,-120,590},(Vec3){450,110,95},yaw,hull,f,0);
 encounter_box(c,(Vec3){-610,250,290},(Vec3){250,75,260},yaw,hull,f,0);
 /* Sorted scrap has a clear frame, alternating bundles and open approach. */
 for(int k=0;k<3;k++){
  encounter_box(c,(Vec3){580,-200,-650+k*220},(Vec3){260,125,85},yaw,rust,f,1);
 }
 /* Broken ship bow, angled ribs and intact panel fragments in the yard. */
 encounter_box(c,(Vec3){-80,-180,-580},(Vec3){190,120,260},yaw+.26f,hull,f,2);
 encounter_arm(c,(Vec3){-250,-300,-790},(Vec3){-150,60,-610},24,yaw,rust);
 encounter_arm(c,(Vec3){170,-300,-780},(Vec3){100,20,-580},24,yaw,rust);
 /* Recovery gantry and its slowly translating suspended magnet. */
 encounter_box(c,(Vec3){-790,80,-750},(Vec3){65,440,65},yaw,rust,f,3);
 encounter_box(c,(Vec3){-340,480,-750},(Vec3){500,55,65},yaw,rust,f,3);
 float x=-260+sinf(game.time*.18f)*170;
 encounter_arm(c,(Vec3){x,430,-750},(Vec3){x,70,-750},9,yaw,hull);
 encounter_box(c,(Vec3){x,40,-750},(Vec3){85,35,85},yaw,rust,f,3);
 for(int k=0;k<4;k++)encounter_lamp(c,(Vec3){-230+k*155,-285,-935},yaw,RGB(224,166,72));
 encounter_lamp(c,(Vec3){-610,355,290},yaw,((int)(game.time*2)&1)?RGB(80,201,181):RGB(39,91,89));
}
'''
s=s.replace(marker,model+marker+'\n if(entry==1){encounter_salvage_village(c,yaw);return 1;}');p.write_text(s)
# A diagnostic override is private-flag-only; ordinary catalogue selection stays identical.
p=root/'src/space-discoveries.h';s=p.read_text().replace('static int discovery_entry(int slot){','static int discovery_capture_entry=-1;\nstatic int discovery_entry(int slot){\n if(discovery_capture_entry>=0&&slot==4)return discovery_capture_entry;');p.write_text(s)
p=root/'src/encounter-flight-capture.h';s=p.read_text().replace('"cinema B"};','"cinema B","salvage village"};').replace('scene>7','scene>8').replace('encounter_capture_scene=scene;','if(scene==8)discovery_capture_entry=1;\n encounter_capture_scene=scene;').replace('slot=scene<2?-scene-1:scene,','slot=scene==8?4:scene<2?-scene-1:scene,');p.write_text(s)
print('Salvage village geometry/materials and isolated native capture installed')
