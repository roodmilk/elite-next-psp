from pathlib import Path
p=Path('src/discovery-detail.h');s=p.read_text();s=s.replace('{{0,2,3,1},{4,5,7,6},{0,4,6,2},{1,3,7,5},{0,1,5,4},{2,6,7,3}}','{{2,3,1,0},{7,6,4,5},{6,2,0,4},{3,7,5,1},{0,1,5,4},{6,7,3,2}}');p.write_text(s)
p=Path('src/encounter-custom.h');s=p.read_text();at=s.index('static int encounter_custom(');s=s[:at]+'''/* Faceted pressure tank: curved silhouette carries its own mapped skin. */
static void encounter_tank(Vec3 c,Vec3 p,float radius,float half,float yaw,unsigned paint,int family){
 for(int k=0;k<8;k++){float a=k*.78539816f,b=(k+1)*.78539816f;Vec3 v[4]={{cosf(a)*radius,sinf(a)*radius,-half},{cosf(b)*radius,sinf(b)*radius,-half},{cosf(b)*radius,sinf(b)*radius,half},{cosf(a)*radius,sinf(a)*radius,half}};
  for(int i=0;i<4;i++)v[i]=add(v[i],p);discovery_design_quad(c,v[0],v[1],v[2],v[3],yaw,paint,ENCOUNTER_TILE(family,0),0);
  Vec3 front=add(p,(Vec3){0,0,-half}),rear=add(p,(Vec3){0,0,half});discovery_design_quad(c,front,v[1],v[0],front,yaw,paint,ENCOUNTER_TILE(family,2),0);discovery_design_quad(c,rear,v[3],v[2],rear,yaw,paint,ENCOUNTER_TILE(family,3),0);
 }
}
''' +s[at:]
s=s.replace('encounter_box(c,(Vec3){side*440,-60,k*790},(Vec3){320,330,320},yaw,paint,family,0);','encounter_tank(c,(Vec3){side*440,-60,k*790},290,330,yaw,paint,family);')
s=s.replace('encounter_box(c,(Vec3){cosf(a)*660,sinf(a)*660,100},(Vec3){210,165,200},yaw,paint,family,0);','encounter_tank(c,(Vec3){cosf(a)*660,sinf(a)*660,100},180,240,yaw,paint,family);')
s=s.replace('encounter_box(c,(Vec3){0,60,980},(Vec3){580,320,250},yaw,paint,family,2);','encounter_box(c,(Vec3){0,60,980},(Vec3){580,320,250},yaw,paint,family,2);for(int side=-1;side<=1;side+=2)encounter_tank(c,(Vec3){side*600,-160,460},150,340,yaw,paint,family);')
p.write_text(s)
