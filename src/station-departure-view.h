static void weapon_fx_fire(void);
/* Bounded camera-local port scenery: no allocations, RNG consumption or
 * simulation NPCs. The launch lane stays clear and the eight-second path stays
 * exactly the same. Relay berths retain their simpler industrial character. */
static Vec3 port_view_origin,port_view_x={1,0,0},port_view_y={0,1,0},port_view_z={0,0,1};
static int port_view_active;static int port_queue_peak;
static void departure_view_prepare(float travelled){
 port_view_active=game.dock_stage==4;if(!port_view_active)return;
 Vec3 f=game.dock_to,r=norm((Vec3){f.z,0,-f.x}),u={-f.x*f.y,f.x*f.x+f.z*f.z,-f.z*f.y};u=norm(u);Vec3 axis=add(game.dock_from,mul(f,travelled));
 port_view_origin=camera(&game,axis);port_view_x=mul(sub(camera(&game,add(axis,mul(r,100))),port_view_origin),.01f);port_view_y=mul(sub(camera(&game,add(axis,mul(u,100))),port_view_origin),.01f);port_view_z=mul(sub(camera(&game,add(axis,mul(f,100))),port_view_origin),.01f);
}
static Vec3 departure_view_point(Vec3 p){return port_view_active?add(port_view_origin,add(add(mul(port_view_x,p.x),mul(port_view_y,p.y)),mul(port_view_z,p.z))):p;}
static Point departure_project(Vec3 p){return project(departure_view_point(p));}
static int departure_grade(void){return game.station_variant?STATION_POOR:station_class(&game);}
static void departure_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){a=departure_view_point(a);b=departure_view_point(b);c=departure_view_point(c);d=departure_view_point(d);queue_triangle(a,b,c,ink);queue_triangle(a,c,d,ink);if(drawcount>port_queue_peak)port_queue_peak=drawcount;}
static void departure_box(float x,float y,float z,float w,float h,float depth,unsigned ink){
 if(departure_view_point((Vec3){x,y,z+depth*.5f}).z+w+h+depth<8)return;
 Vec3 a={x-w,y-h,z},b={x+w,y-h,z},c={x+w,y+h,z},d={x-w,y+h,z};
 Vec3 aa=a,bb=b,cc=c,dd=d;aa.z=bb.z=cc.z=dd.z=z+depth;
 departure_quad(a,b,c,d,ink);departure_quad(d,c,cc,dd,livery_tint(ink,22));
 departure_quad(a,aa,bb,b,livery_tint(ink,-25));
 if(x<0)departure_quad(b,bb,cc,c,livery_tint(ink,-12));else departure_quad(a,d,dd,aa,livery_tint(ink,-38));
}
/* Lamps share their buildings' painter queue: no windows drawn through hulls. */
static void departure_lamp(float x,float y,float z,float w,float h,unsigned ink){
 if(departure_view_point((Vec3){x,y,z}).z+w+h<8)return;
 departure_quad((Vec3){x-w,y-h,z},(Vec3){x+w,y-h,z},(Vec3){x+w,y+h,z},(Vec3){x-w,y+h,z},ink);
}
static int departure_facade_visible(float x,float y,float z,float w,float h){
 if(z<8)return 0;
 Point a=departure_project((Vec3){x-w,y+h,z}),b=departure_project((Vec3){x+w,y-h,z});
 return b.x>=clipx0&&a.x<clipx1&&b.y>=clipy0&&a.y<clipy1&&b.x-a.x>12;
}
static void departure_side_detail(float x,float y,float z,float bw,float bh,int side,unsigned trim){
 if(z+100<8)return;
 float xx=x-side*(bw+.7f),near=fmaxf(8,z),far=z+100;
 Point a=departure_project((Vec3){xx,y+bh,near}),b=departure_project((Vec3){xx,y-bh,far});
 if(fmaxf(a.x,b.x)<clipx0||fminf(a.x,b.x)>=clipx1||b.y<clipy0||a.y>=clipy1)return;
 for(int row=0;row<9;row++)for(int bay=0;bay<3;bay++){
  float yy=y-bh+13+row*(bh*2-26)/8,zz=z+12+bay*31;
  if(zz<8)continue;
  unsigned ink=(row+bay)%4?RGB(140,194,184):RGB(240,184,100);
  departure_quad((Vec3){xx,yy-1.7f,zz},(Vec3){xx,yy+1.7f,zz},(Vec3){xx,yy+1.7f,zz+9},(Vec3){xx,yy-1.7f,zz+9},ink);
 }
 for(int bay=0;bay<3;bay++){
  float zz=z+4+bay*38;if(zz<8)continue;
  departure_quad((Vec3){xx,y-bh,zz},(Vec3){xx,y+bh,zz},(Vec3){xx,y+bh,zz+1.5f},(Vec3){xx,y-bh,zz+1.5f},trim);
 }
}
#include "port-advert-art.h"
static unsigned departure_board_ink(int brand){return port_ad_paper(brand,brand);}
/* Review/legacy helper: stencil lettering only onto its own billboard paper,
 * never over foreground station geometry. */
static void departure_ad_text(int x,int y,unsigned ink,unsigned paper,const char *copy){
 for(int n=0;copy[n];n++)for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[(unsigned char)copy[n]-32][yy]&(128>>xx)){int px=x+n*8+xx,py=y+yy;if(px>=0&&px<W&&py>=0&&py<H&&fb[py*STRIDE+px]==paper)pixel(px,py,ink);}
}
static void departure_ad_graphic(float x,float y,float z,float w,int brand,unsigned accent){(void)x;(void)y;(void)z;(void)w;(void)brand;(void)accent;/* Shared picture texture now covers all distances. */}
static void departure_ad(float x,float y,float z,float w,int brand,unsigned accent){(void)accent;float h=w*.5f;int id=brand;/* Legacy renderer-review entry point. */port_ad_map((Vec3){x-w,y+h,z},(Vec3){x+w,y+h,z},(Vec3){x+w,y-h,z},(Vec3){x-w,y-h,z},brand,id);}
static void departure_wall_pictures(float travelled,int paint){
 if(departure_grade()==STATION_POOR)return;
 for(int side=-1;side<=1;side+=2){int id=side<0?16:17,brand=game.system==7?(side<0?3:2):1+(station_profile_hash(game.system,2)>>(side<0?1:9))%5;float x=side*69.5f,near=-35-travelled,far=25-travelled;
  Vec3 a={x,15,side<0?near:far},b={x,15,side<0?far:near},c={x,-15,b.z},d={x,-15,a.z};
  if(paint)port_ad_map(a,b,c,d,brand,id);else departure_quad(a,b,c,d,port_ad_paper(id,brand));
 }
}
static void departure_district(float travelled,int grade){
 departure_view_prepare(travelled);
 StationProfile p=station_profile_for(&game,0);float mouth=p.half-travelled;
 unsigned hull=mix_rgb(RGB(34,48,65),RGB(p.hull&255,(p.hull>>8)&255,(p.hull>>16)&255),.22f);
 unsigned trim=RGB(p.trim&255,(p.trim>>8)&255,(p.trim>>16)&255);
 int tiers=grade==STATION_POOR?0:grade==STATION_MEGA?4:2;
 for(int tier=tiers-1;tier>=0;tier--)for(int side=-1;side<=1;side+=2){
  unsigned seed=station_profile_hash(game.system,tier+4);
  /* All outboard structures must pass behind/offscreen before eight-second
   * handoff, including the deepest capital profile (half-length 920m). */
  float z=mouth+(grade==STATION_MEGA?260:280)+tier*(grade==STATION_MEGA?125:180);
  float x=side*(155+tier*96),y=(tier&1)?66:-70;
  float bw=grade==STATION_MEGA?46:34,bh=grade==STATION_MEGA?138:86;
  departure_box(x,y,z,bw,bh,105+tier*24,hull);
  departure_box(x+side*(bw+15),y+20,z+36,16,bh*.66f,160,livery_tint(hull,-12));
  int columns=grade==STATION_MEGA?3:4;
  if(departure_facade_visible(x,y,z,bw,bh))for(int row=0;row<(grade==STATION_MEGA?10:7);row++)for(int col=0;col<columns;col++){
   int lit=((seed>>(col+row%7))&1)||(row+col)%3==0;
   unsigned light=lit?((row+col)%4?RGB(158,195,184):RGB(244,190,104)):RGB(43,60,65);
   departure_lamp(x-bw+9+col*(bw*2-18)/(columns-1),y-bh+10+row*(grade==STATION_MEGA?22:15),z-.5f,3,1.6f,light);
  }
  departure_lamp(x,y+bh-7,z-1,bw-5,2,trim);
  departure_side_detail(x,y,z,bw,bh,side,trim);
  if(grade==STATION_MEGA){
   departure_box(x+side*55,y+bh+25,z+55,8,38,70,livery_tint(hull,14));
   departure_box(side*270,155,z+120,152,12,58,hull);
   departure_lamp(side*270,150,z+119,143,1.8f,trim);
   departure_box(side*(340+tier*95),-125,z+130,48,95+(seed%55),150,livery_tint(hull,-10));
  }
  unsigned accent=(tier+(side>0))%3==0?RGB(255,110,173):(tier&1)?RGB(255,194,95):RGB(92,229,222);
  /* Face the outbound lane at eye level, rather than mounting every sign
   * above the canopy where it becomes unreadable just as it grows close. */
  float ay=y+bh*((tier&1)?-.35f:.40f);
  int brand=(tier*2+(side>0)+(p.seed%6))%6;
  float boardw=bw+10,boardh=boardw*.5f;int boardid=tier*2+(side>0);
  departure_box(x,ay,z-5,boardw,boardh,8,port_ad_paper(boardid,brand));
  departure_lamp(x,ay+boardh-1,z-6,boardw,1.3f,accent);departure_lamp(x,ay-boardh+1,z-6,boardw,1.3f,accent);
  departure_ad_graphic(x,ay,z-7,bw,brand,accent);
 }
 /* Actual solid ship meshes cross elevated lanes, never the launch axis. */
 int ships=grade==STATION_POOR?1:grade==STATION_MEGA?5:3;
 for(int i=0;i<ships;i++){
  int mode=(i+p.seed%3)%3;float sign=(i&1)?-1:1,x,y=35+i*9,z,yaw;
  if(mode==0){x=sign*(-510+game.dock_timer*(105+i*13));z=mouth+420+i*80;yaw=sign*1.5707963f;}
  else if(mode==1){x=sign*(195+i*55);z=mouth+100+game.dock_timer*(45+i*5);yaw=0;}
  else{x=sign*(235+i*48);z=mouth+1350-game.dock_timer*(95+i*8);yaw=3.14159265f;}
  Vec3 q=departure_view_point((Vec3){x,y,z});if(q.z<25)continue;
  const char*name=mode==0?"SHUTTLE":mode==1?"ADDER":"COBRA MK III";float base=atan2f(game.dock_to.x,game.dock_to.z),delta=game.yaw-base;
  shipmesh(mesh_id(name),q,yaw-delta,.035f*sinf(game.dock_timer+i)-game.roll,.55f+(i%3)*.12f,RGB(126+(i%3)*26,164+(i%2)*20,188),1);
  departure_lamp(x-sign*12,y,z-1,1.4f,1,mode==1?RGB(255,160,86):RGB(117,216,246));
 }
 if(drawcount>port_queue_peak)port_queue_peak=drawcount;flush_meshes();
 for(int tier=tiers-1;tier>=0;tier--)for(int side=-1;side<=1;side+=2){
  float z=mouth+(grade==STATION_MEGA?260:280)+tier*(grade==STATION_MEGA?125:180),x=side*(155+tier*96),y=(tier&1)?66:-70;
  float bw=grade==STATION_MEGA?46:34,bh=grade==STATION_MEGA?138:86;
  unsigned accent=(tier+(side>0))%3==0?RGB(255,110,173):(tier&1)?RGB(255,194,95):RGB(92,229,222);
  float ay=y+bh*((tier&1)?-.35f:.40f),w=bw+10,h=w*.5f;int brand=(tier*2+(side>0)+(p.seed%6))%6,id=tier*2+(side>0);
  port_ad_map((Vec3){x-w,ay+h,z-5},(Vec3){x+w,ay+h,z-5},(Vec3){x+w,ay-h,z-5},(Vec3){x-w,ay-h,z-5},brand,id);
 }
}
/* Real space is drawn through the hangar mouth, then revealed as it passes
 * the canopy. Masks cover stars/flare outside the opening while still inside. */
static void departure_view(void){
 float travelled=dot(sub(game.pos,game.dock_from),game.dock_to);
 float distance=station_half_for(&game,game.station_variant)-travelled;
 int top=view_top(),bot=view_bot(),cy=(top+bot)/2,grade=departure_grade();
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cx1=clipx1,cy0=clipy0,cy1=clipy1;
 /* Cull against the visible aperture before submitting any port geometry.
  * Also clip planets/flares: the opaque walls would hide those pixels too. */
 preview_clip(ox,oy,0,top,W,bot);
 if(distance>15){int hw=(int)(70*280/distance),hh=(int)(32*280/distance);
  preview_clip(ox,oy,(int)fmaxf(0,240-hw),(int)fmaxf(top,cy-hh),(int)fminf(W,240+hw),(int)fminf(bot,cy+hh));}
 sector_background();space_fx_nebula();starfield();celestial_rings(0);celestial_rims();draw_bodies();celestial_rings(1);lens_flares();
 preview_clip(240,cy,clipx0,clipy0,clipx1,clipy1);
 departure_district(travelled,grade);preview_clip(240,cy,0,top,W,bot);
 if(distance>15){
  int hw=(int)(70*280/distance),hh=(int)(32*280/distance);
  int l=240-hw,r=240+hw,t=cy-hh,b=cy+hh;
  if(l<0)l=0;
  if(r>W)r=W;
  if(t<top)t=top;
  if(b>bot)b=bot;
  rect(0,top,W,t-top,RGB(16,20,27));rect(0,b,W,bot-b,RGB(23,25,30));
  rect(0,t,l,b-t,RGB(20,27,35));rect(r,t,W-r,b-t,RGB(20,27,35));
  departure_wall_pictures(travelled,0);flush_meshes();
  unsigned rim=RGB(237,177,83),rail=RGB(63,95,111);
  line(l,t,r,t,rim);line(l,b,r,b,rim);line(l,t,l,b,UI_CYAN);line(r,t,r,b,UI_CYAN);
  line(0,top,l,t,rail);line(W-1,top,r,t,rail);line(0,bot,l,b,rail);line(W-1,bot,r,b,rail);
  for(int i=1;i<=4;i++){float k=i*.2f;int xl=l*(1-k),xr=r+(W-r)*k,yt=t+(top-t)*k,yb=b+(bot-b)*k;
   line(xl,yt,xr,yt,rail);line(xl,yb,xr,yb,rail);
   rect(xl,yt,4,5,UI_CYAN);rect(xr-4,yt,4,5,UI_CYAN);
   if(grade>STATION_POOR){
    rect(xl,yt,xr-xl,3,RGB(54,66,75));rect(xl,yb-3,xr-xl,3,RGB(42,49,57));
    int lampw=(xr-xl)/5;rect(xl+lampw,yt+3,lampw,2,RGB(116,194,179));
    rect(xr-lampw*2,yt+3,lampw,2,RGB(116,194,179));line(xl,yb-4,xr,yb-4,RGB(219,158,70));
    if(grade==STATION_MEGA){rect(xl,yt,5,yb-yt,RGB(65,77,89));rect(xr-5,yt,5,yb-yt,RGB(65,77,89));}
   }
  }

  departure_wall_pictures(travelled,1);
 }else speed_lines();
 preview_clip(ox,oy,cx0,cy0,cx1,cy1);
 weapon_fx_fire();
 if(hc_full_console_visible())hc_flight_overlay();
 autopilot_annunciator(40,game.dock_timer);
}
