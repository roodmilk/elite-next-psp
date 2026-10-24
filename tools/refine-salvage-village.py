from pathlib import Path
root=Path(__file__).resolve().parents[1]
p=root/'src/encounter-custom.h';s=p.read_text()
start=s.index('static void encounter_salvage_village(');end=s.index('static int encounter_custom(',start)
part=s[start:end].replace('RGB(139,145,132),rust=RGB(154,112,76)','RGB(198,209,194),rust=RGB(196,149,102)')
part=part.replace('encounter_asteroid_foundation(c,yaw);','encounter_salvage_rock(c,yaw);').replace('encounter_box(c,','encounter_salvage_box(c,')
extra='''static void encounter_salvage_box(Vec3 c,Vec3 p,Vec3 e,float yaw,unsigned paint,int family,int role){
 discovery_design_box(c,p,e,yaw,paint,ENCOUNTER_TILE(family,role),ENCOUNTER_TILE(family,2),ENCOUNTER_TILE(family,role),0);
}
static void encounter_salvage_rock(Vec3 c,float yaw){
 const float radius[10]={1,.83f,1.1f,.9f,1.04f,.88f,1.08f,.86f,1.03f,.94f};
 Vec3 rim[10],bottom[10],core={120,-410,-90};unsigned rock=RGB(132,121,108);
 for(int k=0;k<10;k++){float a=k*.62831853f;rim[k]=(Vec3){cosf(a)*1280*radius[k],-450-(k%3)*85,sinf(a)*1390*radius[k]};bottom[k]=(Vec3){cosf(a+.17f)*820,-1180-(k%4)*100,sinf(a+.17f)*930};}
 for(int k=0;k<10;k++){int n=(k+1)%10;
  discovery_design_quad(c,core,rim[n],rim[k],core,yaw,rock,ENCOUNTER_TILE(7,k&3),0);
  discovery_design_quad(c,rim[k],rim[n],bottom[n],bottom[k],yaw,rock,ENCOUNTER_TILE(7,(k+1)&3),0);
  Vec3 tip={-80,-1640,70};discovery_design_quad(c,tip,bottom[k],bottom[n],tip,yaw,rock,ENCOUNTER_TILE(7,2),0);
 }
}
'''
s=s[:start]+extra+part+s[end:]
# The reused pressure vessel has inhabited end caps, not winch/cut-hull plates.
s=s.replace('ENCOUNTER_TILE(family,2),0);discovery_design_quad(c,rear','ENCOUNTER_TILE(family,family==8?0:2),0);discovery_design_quad(c,rear').replace('ENCOUNTER_TILE(family,3),0);\n }\n}', 'ENCOUNTER_TILE(family,family==8?0:3),0);\n }\n}')
p.write_text(s)
p=root/'src/main.c';s=p.read_text().replace('(frames==90||frames==290||frames==490)','(frames==30||frames==60||frames==90||frames==130||frames==170||frames==290||frames==490)');p.write_text(s)
print('Revised village: textured irregular rock crown, brighter materials, role-correct cabin end caps, seven orbit captures')
