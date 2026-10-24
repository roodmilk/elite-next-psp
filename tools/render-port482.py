from pathlib import Path
p=Path('src/station-departure-view.h');s=p.read_text();needle='static int departure_grade(void)'
i=s.index(needle)
transform='''static Vec3 port_view_origin,port_view_x={1,0,0},port_view_y={0,1,0},port_view_z={0,0,1};
static int port_view_active;
static void departure_view_prepare(float travelled){
 port_view_active=game.dock_stage==4;if(!port_view_active)return;
 Vec3 f=game.dock_to,r=norm((Vec3){f.z,0,-f.x}),u={-f.x*f.y,f.x*f.x+f.z*f.z,-f.z*f.y};u=norm(u);Vec3 axis=add(game.dock_from,mul(f,travelled));
 port_view_origin=camera(&game,axis);port_view_x=mul(sub(camera(&game,add(axis,mul(r,100))),port_view_origin),.01f);port_view_y=mul(sub(camera(&game,add(axis,mul(u,100))),port_view_origin),.01f);port_view_z=mul(sub(camera(&game,add(axis,mul(f,100))),port_view_origin),.01f);
}
static Vec3 departure_view_point(Vec3 p){return port_view_active?add(port_view_origin,add(add(mul(port_view_x,p.x),mul(port_view_y,p.y)),mul(port_view_z,p.z))):p;}
static Point departure_project(Vec3 p){return project(departure_view_point(p));}
'''
s=s[:i]+transform+s[i:];s=s.replace('static void departure_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){queue_triangle(a,b,c,ink);queue_triangle(a,c,d,ink);}','static void departure_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){a=departure_view_point(a);b=departure_view_point(b);c=departure_view_point(c);d=departure_view_point(d);queue_triangle(a,b,c,ink);queue_triangle(a,c,d,ink);}')
a=s.index('static unsigned departure_board_ink');b=s.index('static void departure_district',a)
s=s[:a]+'''#include "port-advert-art.h"
static unsigned departure_board_ink(int brand){return port_ad_paper(brand,brand);}
static void departure_ad_graphic(float x,float y,float z,float w,int brand,unsigned accent){(void)x;(void)y;(void)z;(void)w;(void)brand;(void)accent;/* Shared picture texture now covers all distances. */}
static void departure_ad(float x,float y,float z,float w,int brand,unsigned accent){(void)accent;float h=w*.5f;int id=0;/* Legacy renderer-review entry point. */port_ad_map((Vec3){x-w,y+h,z},(Vec3){x+w,y+h,z},(Vec3){x+w,y-h,z},(Vec3){x-w,y-h,z},brand,id);}
static void departure_wall_pictures(float travelled,int paint){
 if(departure_grade()==STATION_POOR)return;
 for(int side=-1;side<=1;side+=2){int id=side<0?16:17,brand=side<0?3:2;float x=side*69.5f,near=-35-travelled,far=25-travelled;
  Vec3 a={x,15,side<0?near:far},b={x,15,side<0?far:near},c={x,-15,b.z},d={x,-15,a.z};
  if(paint)port_ad_map(a,b,c,d,brand,id);else departure_quad(a,b,c,d,port_ad_paper(id,brand));
 }
}
'''+s[b:]
s=s.replace(' if(grade==STATION_POOR)return;\n StationProfile',' departure_view_prepare(travelled);\n StationProfile')
s=s.replace('int tiers=grade==STATION_MEGA?4:2;','int tiers=grade==STATION_POOR?0:grade==STATION_MEGA?4:2;')
s=s.replace('departure_box(x,ay,z-5,bw+2,27,8,departure_board_ink(brand));','float boardw=bw+10,boardh=boardw*.5f;int boardid=tier*2+(side>0);\n  departure_box(x,ay,z-5,boardw,boardh,8,port_ad_paper(boardid,brand));')
s=s.replace('departure_lamp(x,ay+24,z-6,bw,1.3f,accent);departure_lamp(x,ay-24,z-6,bw,1.3f,accent);','departure_lamp(x,ay+boardh-1,z-6,boardw,1.3f,accent);departure_lamp(x,ay-boardh+1,z-6,boardw,1.3f,accent);')
a=s.index(' int ships=grade==STATION_MEGA?4:1;');b=s.index(' flush_meshes();',a)
s=s[:a]+''' int ships=grade==STATION_POOR?1:grade==STATION_MEGA?5:3;
 for(int i=0;i<ships;i++){
  int mode=(i+p.seed%3)%3;float sign=(i&1)?-1:1,x,y=35+i*9,z,yaw;
  if(mode==0){x=sign*(-510+game.dock_timer*(105+i*13));z=mouth+590+i*150;yaw=sign*1.5707963f;}
  else if(mode==1){x=sign*(195+i*55);z=mouth+160+game.dock_timer*(75+i*12);yaw=0;}
  else{x=sign*(235+i*48);z=mouth+1350-game.dock_timer*(95+i*8);yaw=3.14159265f;}
  Vec3 q=departure_view_point((Vec3){x,y,z});if(q.z<25)continue;
  const char*name=mode==0?"SHUTTLE":mode==1?"ADDER":"COBRA MK III";float base=atan2f(game.dock_to.x,game.dock_to.z),delta=game.yaw-base;
  shipmesh(mesh_id(name),q,yaw-delta,.035f*sinf(game.dock_timer+i)-game.roll,.55f+(i%3)*.12f,RGB(126+(i%3)*26,164+(i%2)*20,188),1);
  departure_lamp(x-sign*12,y,z-1,1.4f,1,mode==1?RGB(255,160,86):RGB(117,216,246));
 }
'''+s[b:]
old='  departure_ad(x,y+bh*((tier&1)?-.35f:.40f),z-7,bw,(tier*2+(side>0)+(p.seed%6))%6,accent);'
new='''  float ay=y+bh*((tier&1)?-.35f:.40f),w=bw+10,h=w*.5f;int brand=(tier*2+(side>0)+(p.seed%6))%6,id=tier*2+(side>0);
  port_ad_map((Vec3){x-w,ay+h,z-5},(Vec3){x+w,ay+h,z-5},(Vec3){x+w,ay-h,z-5},(Vec3){x-w,ay-h,z-5},brand,id);'''
assert old in s;s=s.replace(old,new)
# All visibility checks must follow the actual cleared-hull bend.
s=s.replace('Point a=project(','Point a=departure_project(').replace('),b=project(', '),b=departure_project(')
a=s.index('  if(grade>STATION_POOR&&l>85&&b-t>22){');b=s.index('\n }else speed_lines();',a)
s=s[:a]+s[b:]
# Draw wall-mounted picture planes into wall masks, then rails can occlude them.
s=s.replace('  unsigned rim=RGB(237,177,83),rail=', '  departure_wall_pictures(travelled,0);flush_meshes();\n  unsigned rim=RGB(237,177,83),rail=')
s=s.replace('\n }else speed_lines();','\n  departure_wall_pictures(travelled,1);\n }else speed_lines();')
s=s.replace('five-second path','eight-second path').replace('before five-second','before eight-second')
p.write_text(s)
