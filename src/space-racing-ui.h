static int erl_commit(void);
/* Native flight visuals. Fixed local scenery/ring/hull budgets share world
 * depth sorting; no new framebuffer, texture or unbounded particle pool. */
static Vec3 race_ui_cross(Vec3 a,Vec3 b){return (Vec3){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
static float race_view_focal(float base){
 static unsigned seed;static float last,blend;SpaceRace *r=&game.race;
 if(seed!=r->seed||r->elapsed<last){seed=r->seed;last=r->elapsed;blend=0;}
 float dt=fmaxf(0,fminf(.1f,r->elapsed-last));last=r->elapsed;
 float speed=fmaxf(0,game.speed/fmaxf(1,r->top_speed)-.55f),target=r->phase==RACE_RUNNING?fminf(.31f,.055f+speed*.12f+(game.boost||r->boost_time>0||r->arcade.actor[0].rocket>0?.12f:0)):0;
 blend+=(target-blend)*(1-expf(-dt*7));return base*(1-blend);
}
/* Infinite, camera-anchored stars: turning moves the sky, accelerating never
 * drags a flat wallpaper behind the track. Fixed storage and work per frame. */
static void race_starfield(void){
 static Vec3 directions[2400];static unsigned char brightness[2400];static int ready;
 if(!ready){unsigned seed=0xE710397u;for(int i=0;i<2400;i++){float v[3];for(int k=0;k<3;k++){seed=seed*1664525u+1013904223u;v[k]=((seed>>8)&65535)/32767.5f-1;}directions[i]=norm((Vec3){v[0],v[1],v[2]});brightness[i]=(seed>>24)&255;}ready=1;}
 Vec3 r,u,a;camera_axes(&game,&r,&u,&a);int x0=clipx0,x1=clipx1,y0=clipy0>=0?clipy0:view_top(),y1=clipy1>=0?clipy1:view_bot()+1;
 for(int i=0;i<2400;i++){if(brightness[i]<224)continue;Vec3 d=directions[i];float z=dot(d,a);if(z<.2f)continue;int x=proj_ox+dot(d,r)*proj_focal/z,y=proj_oy-dot(d,u)*proj_focal/z;if(x<x0+2||x>=x1-2||y<y0+2||y>=y1-2)continue;
  int b=brightness[i];float shimmer=b>250?.85f+.15f*sinf(game.race.elapsed*1.3f+i*.71f):1;unsigned c=b<252?RGB((int)((35+b/5)*shimmer),(int)((48+b/5)*shimmer),(int)((65+b/5)*shimmer)):RGB(182,209,221);pixel(x,y,c);
  if(b>253){unsigned edge=RGB(48,91,111);pixel(x-1,y,edge);pixel(x+1,y,edge);pixel(x,y-1,edge);pixel(x,y+1,edge);}
 }
 /* The round course orbits an actual local world.  A single native planet
  * sprite provides the large, readable landmark used by the approved
  * studies without adding another 3-D mesh or render target. */
 if(game.race.layout==4){
  /* A full menu-quality planet renderer costs several milliseconds in live
   * flight.  This bounded 95-scanline landmark keeps its atmospheric rim,
   * night side and seeded cloud bands for a small, stable cost. */
  const int cx=72,cy=82,pr=47;unsigned seed=game.race.seed^((unsigned)game.system*2654435761u);
  for(int py=-45;py<=45;py+=6){int half=(int)sqrtf((float)(pr*pr-py*py)),shade=25+(py+pr)*18/(pr*2),terminator=cx+half/3;unsigned sea=RGB(15+shade/4,38+shade/2,51+shade/2);rect(cx-half,cy+py,half*2+1,6,sea);rect(terminator,cy+py,cx+half-terminator+1,6,RGB(8+shade/7,18+shade/5,28+shade/4));if(((py+(int)(seed&15))/6)%3==0){int wave=(int)(sinf(py*.31f+(seed&255)*.01f)*7),left=cx-half/2+wave,right=cx+half/3+wave;if(right>left)rect(left,cy+py+2,right-left,2,RGB(51,91,103));}}
 }
}
static int race_crash_camera(void){
 if(game.race.layout==5&&game.race.crash_time>0){
  SpaceRace *r=&game.race;float age=3-r->crash_time;int own=r->repair>0;Vec3 focus=own?game.pos:r->pilot[r->crash_id>=0?r->crash_id:0].pos,dir=own?forward(&game):r->crash_dir;
  RaceTrackFrame f=race_track_nearest(r,r->gate,focus);Vec3 right=norm(race_ui_cross(f.up,dir)),up=norm(race_ui_cross(dir,right));
  Vec3 eye=add(focus,add(mul(dir,-720),add(mul(right,430+130*sinf(age*.8f)),mul(up,240))));
  if(age>2.75f){float t=fminf(1,(age-2.75f)*4);t=t*t*(3-2*t);Vec3 home,ca,cu;erl_crash_live_camera(&home,&ca,&cu);eye=add(mul(eye,1-t),mul(home,t));focus=add(mul(focus,1-t),mul(add(home,mul(ca,1800)),t));}
  Vec3 look=norm(sub(focus,eye));camera_override_set(eye,atan2f(look.x,look.z),asinf(fmaxf(-1,fminf(1,look.y))),0);return 1;
 }
 SpaceRace *r=&game.race;if(!race_is_active(&game)||r->crash_time<=3.f||r->crash_id<0)return 0;
 RaceRival *p=&r->pilot[r->crash_id];float age=5.8f-r->crash_time;
 Vec3 right=norm(race_ui_cross((Vec3){0,1,0},p->dir)),up=norm(race_ui_cross(p->dir,right));
 float orbit=age*.7f;Vec3 eye=add(p->pos,add(mul(p->dir,-600*cosf(orbit)),add(mul(right,420+180*sinf(orbit)),mul(up,230))));
 Vec3 focus=p->pos,return_up={0,1,0};float blend=0;
 if(age>1.2f){
  float t=fminf(1,(age-1.2f)/.6f);t=t*t*(3-2*t);blend=t;
  /* Sample the real selected first/chase/cinematic mount, then blend back
   * to it. Temporarily bypass only this override, never the simulation. */
  float shot=r->crash_time;r->crash_time=0;flight_camera_prepare();Vec3 home=camera_origin(&game),cr,ca;camera_axes(&game,&cr,&return_up,&ca);r->crash_time=shot;
  eye=add(mul(eye,1-t),mul(home,t));focus=add(mul(focus,1-t),mul(add(home,mul(ca,1800)),t));
 }
 Vec3 look=norm(sub(focus,eye));float yaw=atan2f(look.x,look.z),pitch=atan2f(look.y,sqrtf(look.x*look.x+look.z*look.z));
 Vec3 cr={cosf(yaw),0,-sinf(yaw)},cu={-sinf(yaw)*sinf(pitch),cosf(pitch),-cosf(yaw)*sinf(pitch)};
 float roll=blend*atan2f(-dot(return_up,cr),dot(return_up,cu));camera_override_set(eye,yaw,pitch,roll);return 1;
}
static void race_crash_overlay(void){
 if(game.race.layout==5&&game.race.crash_time>0){rect(132,0,216,18,RGB(4,8,15));text_px(160,6,GOLD,game.race.repair>0?"HULL LOST / RECOVERY":"RIVAL TAKEDOWN!");text_px(156,195,RGB(111,238,176),game.race.repair>0?"3 SECOND RACE PENALTY":"LIVE RACE CONTINUES");return;}
 if(game.race.crash_time<=3.f)return;
 rect(0,0,W,18,RGB(4,8,15));rect(0,H-22,W,22,RGB(4,8,15));
 text_px(180,6,GOLD,"RIVAL TAKEDOWN!");text_px(168,H-14,RGB(111,238,176),"AUTOPILOT ENGAGED");
}
static void race_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){
 a=camera(&game,a);b=camera(&game,b);c=camera(&game,c);d=camera(&game,d);
 /* Course faces use an inward winding. Reject the hidden half before clip,
  * sort or raster work; this also removes luminous backs of solid panels. */
 if(dot(race_ui_cross(sub(b,a),sub(c,a)),a)<=0)return;
 queue_triangle(a,b,c,ink);queue_triangle(a,c,d,ink);
}
static Vec3 race_ring_vertex(Vec3 center,Vec3 right,Vec3 up,Vec3 normal,float angle,float radius,float depth){
 return add(center,add(mul(right,cosf(angle)*radius),add(mul(up,sinf(angle)*radius),mul(normal,depth))));
}
static void race_ring_draw(const RaceGate *q,Vec3 center,float radius,int gold,int finish){
 float distance=length(sub(center,game.pos));if(distance>53000||!render_sphere_visible(center,radius+100))return;
 int segments=distance>18000?12:24;float thickness=fmaxf(gold?32:66,distance*(gold?2.f:2.8f)/240.f),depth=gold?36:90;
 for(int s=0;s<segments;s++){
  float a=s*6.2831853f/segments,b=(s+1)*6.2831853f/segments;
  unsigned face=gold?RGB(82,255,113):finish?((s/2)&1?WHITE:RGB(35,48,60)):s%6==0?WHITE:RGB(62,230,206);
  Vec3 outer[4],inner[4];
  outer[0]=race_ring_vertex(center,q->right,q->up,q->normal,a,radius+thickness,-depth);
  outer[1]=race_ring_vertex(center,q->right,q->up,q->normal,b,radius+thickness,-depth);
  outer[2]=race_ring_vertex(center,q->right,q->up,q->normal,b,radius+thickness,depth);
  outer[3]=race_ring_vertex(center,q->right,q->up,q->normal,a,radius+thickness,depth);
  inner[0]=race_ring_vertex(center,q->right,q->up,q->normal,a,radius,-depth);
  inner[1]=race_ring_vertex(center,q->right,q->up,q->normal,b,radius,-depth);
  inner[2]=race_ring_vertex(center,q->right,q->up,q->normal,b,radius,depth);
  inner[3]=race_ring_vertex(center,q->right,q->up,q->normal,a,radius,depth);
  race_quad(outer[0],outer[1],inner[1],inner[0],face);
  race_quad(outer[3],inner[3],inner[2],outer[2],livery_tint(face,-50));
  race_quad(outer[0],outer[3],outer[2],outer[1],livery_tint(face,-90));
  race_quad(inner[0],inner[1],inner[2],inner[3],livery_tint(face,-24));
 }
}
static void race_flag_panel(Vec3 center,Vec3 right,Vec3 up,Vec3 normal,float cell){
 for(int y=0;y<3;y++)for(int x=0;x<8;x++){
  Vec3 a=add(center,add(mul(right,(x-4)*cell),mul(up,(y-1.5f)*cell)));
  race_quad(a,add(a,mul(right,cell)),add(a,add(mul(right,cell),mul(up,cell))),add(a,mul(up,cell)),(x+y)&1?RGB(235,232,211):RGB(22,28,36));
 }
}
static Vec3 race_ship_vertex(const RaceRival *p,Vec3 v,Vec3 right,Vec3 up){return add(p->pos,add(mul(right,v.x*RACE_SHIP_SCALE),add(mul(up,v.y*RACE_SHIP_SCALE),mul(p->dir,v.z*RACE_SHIP_SCALE))));}
static void race_fragments(const RaceRival *p,const Mesh *m,Vec3 right,Vec3 up,unsigned color){
 float age=p->burst;if(age>4)return;
 for(int f=0;f<m->triangles&&f<32;f++){
  const MeshTri *t=&m->t[f];Vec3 local[3]={m->v[t->a],m->v[t->b],m->v[t->c]};
  Vec3 centre=mul(add(add(local[0],local[1]),local[2]),1.f/3),direction=norm(add(centre,(Vec3){(f%3-1)*17.f,((f*7)%5-2)*11.f,13}));
  Vec3 drift=mul(direction,age*(220+f%7*55));Vec3 v[3];
  for(int k=0;k<3;k++){Vec3 rotated=rotate(sub(local[k],centre),age*(f%3+1),age*(f%2?1:-1));v[k]=camera(&game,race_ship_vertex(p,add(add(centre,rotated),drift),right,up));}
  queue_triangle(v[0],v[1],v[2],age<.045f?WHITE:age<.13f?RGB(255,168,64):livery_tint(color,-15));
 }
 for(int k=0;k<12;k++){
  float a=k*2.39996f;Vec3 drift=add(mul(right,cosf(a)*age*220),add(mul(up,sinf(a)*age*220),mul(p->dir,(k%3-1)*age*130)));
  Vec3 c=add(p->pos,drift);float size=(1-fminf(1,age/3.8f))*(10+k%3*5);unsigned ink=age<.13f?RGB(255,227,154):age<1.4f?RGB(252,118,42):RGB(85,65,75);
  queue_triangle(camera(&game,add(c,mul(right,-size))),camera(&game,add(c,mul(up,size))),camera(&game,add(c,mul(right,size))),ink);
 }
 if(age<1.f)for(int s=0;s<16;s++){
  float a=s*.392699f,b=(s+1)*.392699f,radius=80+age*480;
  Vec3 a0=race_ring_vertex(p->pos,right,up,p->dir,a,radius,0),a1=race_ring_vertex(p->pos,right,up,p->dir,b,radius,0);
  Vec3 b0=race_ring_vertex(p->pos,right,up,p->dir,a,radius+8,0),b1=race_ring_vertex(p->pos,right,up,p->dir,b,radius+8,0);
  race_quad(a0,a1,b1,b0,s&1?RGB(245,151,74):RGB(239,205,155));
 }
}
static void race_ship_draw(const RaceRival *p,int i){
 if((p->disabled&&game.race.arcade.actor[i+1].health>0)||game.race.arcade.actor[i+1].eliminated||length(sub(p->pos,camera_origin(&game)))>22000||!render_sphere_visible(p->pos,550))return;
 Vec3 right=norm((Vec3){p->dir.z,0,-p->dir.x}),up=norm(race_ui_cross(p->dir,right));
 if(game.race.layout==5){RaceTrackFrame f=race_track_nearest(&game.race,p->gate,p->pos);Vec3 n=sub(f.up,mul(p->dir,dot(f.up,p->dir)));if(dot(n,n)>.00001f){up=norm(n);right=norm(race_ui_cross(up,p->dir));up=norm(race_ui_cross(p->dir,right));}}
 erl_racer_draw(erl_racer_id(&game,i+1),p->pos,right,up,p->dir,RACE_SHIP_SCALE,p->disabled?p->burst:0,game.race.arcade.actor[i+1].protection>0);
}
static void race_block(const RaceGate *q,Vec3 centre,float sx,float sy,float sz,unsigned ink){
 Vec3 v[8];for(int i=0;i<8;i++){float bevel=i&4?.82f:1;v[i]=camera(&game,add(centre,add(mul(q->right,((i&1)?sx:-sx)*bevel),add(mul(q->up,((i&2)?sy:-sy)*bevel),mul(q->normal,(i&4)?sz:-sz)))));}
 static const int sides[6][4]={{0,1,3,2},{4,6,7,5},{0,4,5,1},{2,3,7,6},{0,2,6,4},{1,5,7,3}};
 /* Shared corners are transformed once, instead of once per adjoining face. */
 for(int s=0;s<6;s++){Vec3 a=v[sides[s][0]],b=v[sides[s][1]],c=v[sides[s][2]],d=v[sides[s][3]];if(dot(race_ui_cross(sub(b,a),sub(c,a)),a)<=0)continue;unsigned shade=livery_tint(ink,s*9-24);queue_triangle(a,b,c,shade);queue_triangle(a,c,d,shade);}
}
static void race_drone(const RaceGate *q,Vec3 ship,float age){
 Vec3 p=add(ship,add(mul(q->right,170+30*sinf(age*5)),add(mul(q->up,110),mul(q->normal,440))));
 race_block(q,p,48,22,34,RGB(188,206,208));race_block(q,add(p,mul(q->normal,-36)),25,9,6,RGB(82,236,202));
 for(int side=-1;side<=1;side+=2){Vec3 arm=add(p,mul(q->right,side*67));race_block(q,arm,22,8,13,RGB(69,109,125));race_block(q,add(arm,mul(q->normal,14+sinf(age*9)*8)),8,8,28,RGB(254,191,65));}
 Vec3 a=camera(&game,p),b=camera(&game,ship);if(a.z>15&&b.z>15){Point x=project(a),y=project(b);line(x.x,x.y,y.x,y.y,RGB(68,230,206));}
}
static void race_scenery(void){
 SpaceRace *r=&game.race;int start=r->gate>0?r->gate-1:0;
 if(r->layout==4){
  /* The opening round course gets its own authored skyline instead of the
   * asteroid/freighter themes used by later free-flight layouts. Monumental
   * portals provide scale and speed rhythm while every solid stays well
   * outside the legal road and jump envelope. */
  for(int k=start;k<RACE_GATES&&k<start+2;k++){
   RaceTrackFrame f=race_track_frame(r,k,.72f);int kind=race_track_surface(r,k,.72f);if(kind==2||length(sub(f.centre,camera_origin(&game)))>52000||!render_sphere_visible(f.centre,15000))continue;
   RaceGate q={.pos=f.centre,.normal=f.ahead,.right=f.right,.up=f.up};float road=kind?3900:8500,clear=road+1450;
   if(!(k&1)){
    unsigned metal=kind?RGB(55,62,82):RGB(54,72,88),light=kind?RGB(255,103,207):RGB(92,224,224);
    /* Twin illuminated pylons frame the road. A former full-width crown
     * projected as several detached horizontal faces on tight bends, so the
     * landmark now stays vertical and cannot cut across the road horizon. */
    for(int side=-1;side<=1;side+=2){Vec3 pillar=add(f.centre,add(mul(f.right,side*clear),mul(f.up,RACE_TRACK_FLOOR+2450)));race_block(&q,pillar,kind?310:430,2450,300,metal);race_block(&q,add(pillar,mul(f.right,-side*(kind?335:455))),70,1900,330,light);}
   }else{
    /* One strong arcology silhouette and one inset strip per side read more
     * cleanly at 480x272 than stacks of small blocks, and halve the face
     * cost of the earlier pass. */
    for(int side=-1;side<=1;side+=2){Vec3 tower=add(f.centre,add(mul(f.right,side*(clear+2600)),mul(f.up,RACE_TRACK_FLOOR+1750)));race_block(&q,tower,1150,1750,720,RGB(42,57,75));race_block(&q,add(tower,mul(f.right,-side*1170)),35,1020,500,side>0?RGB(78,176,190):RGB(239,91,183));}
   }
  }
  return;
 }
 for(int k=start;k<RACE_GATES&&k<start+4;k++){
  RaceGate *q=&r->gates[k];Vec3 prev=k?r->gates[k-1].pos:r->start,mid=mul(add(prev,q->pos),.5f);
  if(length(sub(mid,camera_origin(&game)))>44000||!render_sphere_visible(mid,11000))continue;
  int theme=(k/8)%4;
  Vec3 hazard,half;int moving=race_hazard(r,k,r->elapsed,&hazard,&half);
  if(moving&&render_sphere_visible(hazard,length(half)+1000)){
   unsigned ink=moving==3?RGB(188,61,140):RGB(71,94,111);race_block(q,hazard,half.x,half.y,half.z,ink);
   race_block(q,add(hazard,mul(q->up,half.y+18)),half.x,18,half.z,RGB(255,119,183));
   race_block(q,add(hazard,mul(q->normal,-half.z-18)),half.x*.7f,35,18,RGB(93,236,226));
   Vec3 warning=camera(&game,add(hazard,mul(q->up,half.y+520)));if(warning.z>15&&length(sub(hazard,camera_origin(&game)))<18000){Point p=project(warning);unsigned lamp=((int)(r->elapsed*3)&1)?RGB(255,206,96):RGB(178,118,61);line(p.x,p.y-7,p.x-7,p.y+6,lamp);line(p.x-7,p.y+6,p.x+7,p.y+6,lamp);line(p.x+7,p.y+6,p.x,p.y-7,lamp);line(p.x,p.y-2,p.x,p.y+1,lamp);pixel(p.x,p.y+4,lamp);}
  }
  if(theme==0||theme==3){
   for(int n=0;n<5;n++){
    float side=n&1?1:-1;Vec3 p=add(mid,add(mul(q->right,side*(4500+n%3*900)),add(mul(q->up,(n%3-1)*3300.f),mul(q->normal,(n-2)*3000.f))));
    if(!render_sphere_visible(p,1700))continue;
    shipmesh(mesh_id("BOULDER"),p,k*.37f+game.time*.02f,n*.6f,(850+n%3*300)/render_mesh_radius(mesh_id("BOULDER")),theme?RGB(82,141,174):RGB(112,85,70),0);
   }
  }else if(theme==1){
   /* Split freighter carcasses: wide physical passage, torn cargo ribs and
    * orange warning lamps. Every solid component is outside the racing lane. */
   for(int side=-1;side<=1;side+=2){
    Vec3 p=add(mid,mul(q->right,side*4700.f));race_block(q,p,900,1700,6500,RGB(62,79,92));
    for(int rib=-1;rib<=1;rib++){Vec3 c=add(p,mul(q->normal,rib*4300.f));race_block(q,c,1150,2000,160,RGB(95,98,103));race_block(q,add(c,mul(q->right,-side*1170.f)),35,950,70,RGB(249,145,60));}
    /* Recessed cooling ribs and teal lamps face the open racing lane. */
    for(int rib=0;rib<7;rib++){Vec3 c=add(p,add(mul(q->normal,(rib-3)*1450.f),mul(q->right,-side*920.f)));race_block(q,c,24,1150,55,RGB(29,45,57));race_block(q,add(c,mul(q->up,280)),28,560,26,rib%3?RGB(53,95,108):RGB(72,202,194));}
   }
   race_block(q,add(mid,mul(q->up,5000)),5800,450,5700,RGB(50,67,83));
  }else{
   /* Floodlit port fly-through: grandstands, luminous pylons and chevron
    * advertising panels; geometric art remains legible at native resolution. */
   for(int side=-1;side<=1;side+=2){
    Vec3 p=add(mid,mul(q->right,side*6500.f));race_block(q,p,1700,1100,6000,RGB(51,57,91));
    for(int n=-1;n<=1;n++){Vec3 c=add(p,mul(q->normal,n*4300.f));race_block(q,add(c,mul(q->up,1800)),400,900,400,RGB(65,177,179));race_block(q,add(c,mul(q->right,-side*1740.f)),50,700,1300,n&1?RGB(239,99,173):RGB(76,210,237));}
    for(int rib=0;rib<9;rib++){Vec3 c=add(p,add(mul(q->normal,(rib-4)*1300.f),mul(q->right,-side*1725.f)));race_block(q,c,26,800,80,RGB(89,111,125));}
   }
  }
  /* Trail lights suggest the turn, with a clearly open lane down the middle. */
  for(int n=1;n<4;n++)for(int side=-1;side<=1;side+=2){Vec3 p=add(add(prev,mul(sub(q->pos,prev),n*.25f)),mul(q->right,side*3200.f));race_block(q,p,60,150,110,theme==3?RGB(144,141,249):RGB(90,217,196));}
 }
}
#include "erl-arcade-world.h"
#include "erl-apex-effects.h"
#include "erl-course-dressing.h"
static void race_gantry_art(void){
 SpaceRace *r=&game.race;if(r->phase==RACE_RUNNING&&(r->elapsed>.8f||r->gate))return;
 if(r->phase!=RACE_GRID&&r->phase!=RACE_COUNTDOWN&&r->phase!=RACE_RUNNING)return;
 Vec3 centre=add(r->start,mul(r->dir,1800));RaceGate q={.pos=centre,.normal=r->dir,.right=r->right,.up=r->up};
 for(int side=-1;side<=1;side+=2)race_block(&q,add(centre,mul(r->right,side*1250)),65,600,75,RGB(49,52,77));
 race_block(&q,add(centre,mul(r->up,600)),1300,100,90,RGB(44,43,69));
 for(int i=0;i<3;i++){int on=r->phase==RACE_RUNNING||r->phase==RACE_COUNTDOWN&&r->countdown<3-i;unsigned ink=on?(r->phase==RACE_RUNNING?RGB(92,255,151):RGB(255,63,92)):RGB(51,34,45);Vec3 p=add(centre,add(mul(r->right,(i-1)*340),mul(r->up,600)));race_block(&q,add(p,mul(r->dir,-110)),115,65,20,ink);}
 if(r->phase==RACE_RUNNING){const char *word="GO";for(int i=0;i<2;i++)for(int y=0;y<8;y++)for(int x=0;x<8;x++)if(font8[word[i]-32][y]&(128>>x)){
  Vec3 p=add(centre,add(mul(r->right,(i*8+x-8)*32),add(mul(r->up,300-y*32),mul(r->dir,60))));race_block(&q,p,13,13,2,RGB(112,255,182));}}
}
static void race_world_draw(void){
 SpaceRace *r=&game.race;if(!race_ready(&game)||game.docked||game.dock_stage||game.planet>=0)return;
 if(r->phase==RACE_GRID||r->phase==RACE_COUNTDOWN){
  RaceGate start={.pos=r->start,.normal=r->dir,.right=r->right,.up=r->up};race_ring_draw(&start,r->start,1100,0,1);
  if(length(sub(r->start,game.pos))<20000){Vec3 panel=add(r->start,mul(r->up,650));race_flag_panel(panel,r->right,r->up,r->dir,85);}
 }
 race_gantry_art();
 race_scenery();race_corridor_art();race_service_art();race_pickup_art();
 for(int i=0;i<r->rivals;i++){RaceRival *p=&r->pilot[i];race_ship_draw(p,i);if(p->disabled&&p->burst>.8f)race_drone(&r->gates[p->gate<RACE_GATES?p->gate:RACE_GATES-1],p->pos,p->burst);}
 if(r->repair>0)race_drone(&r->gates[r->gate<RACE_GATES?r->gate:RACE_GATES-1],game.pos,3-r->repair);

}
static void race_count_digit(int x,int y,int number,unsigned ink){
 const int bits[4]={0,6,91,79};int mask=bits[number<1||number>3?0:number];
 const int seg[7][4]={{4,0,24,5},{28,4,5,24},{28,31,5,24},{4,55,24,5},{0,31,5,24},{0,4,5,24},{4,27,24,5}};
 for(int s=0;s<7;s++)if(mask&(1<<s))rect(x+seg[s][0],y+seg[s][1],seg[s][2],seg[s][3],ink);
}
#include "erl-vector-hud.h"
/* Three seconds of a render-only trackside flyby. Actual grid poses, clock,
 * pickups and RNG remain untouched; X skips only this introduction. */
static int race_grid_intro_draw(void){
 SpaceRace *r=&game.race;if(r->layout==5||r->phase!=RACE_COUNTDOWN||r->countdown<=4)return 0;
 float age=7-r->countdown;int rival=r->rivals?1:0;RaceRival p=rival?r->pilot[0]:(RaceRival){.pos=game.pos,.dir=r->dir,.mesh=mesh_id(player_ships[game.ship].name),.radius=180};
 p.pos=add(r->start,add(mul(r->dir,(age-1.5f)*1600),mul(r->right,-250)));p.dir=r->dir;p.disabled=0;
 Vec3 eye=add(r->start,add(mul(r->right,-850),mul(r->up,190))),look=norm(sub(p.pos,eye));camera_override_set(eye,atan2f(look.x,look.z),asinf(look.y),0);
 int min=surface_y_min,max=surface_y_max,depth=surface_depth_on,encoding=mega_depth_encoding;float focal=proj_focal;surface_y_min=20;surface_y_max=H-24;surface_depth_on=1;mega_depth_encoding=1;proj_focal=300;
 rect(0,0,W,H,RGB(8,9,24));memset(surface_depth,255,sizeof(surface_depth));race_starfield();race_scenery();race_ship_draw(&p,0);flush_solid_meshes();race_holo_track_draw();
 surface_y_min=min;surface_y_max=max;surface_depth_on=depth;mega_depth_encoding=encoding;proj_focal=focal;
 rect(8,165,254,78,RGB(39,26,72));line(8,165,261,165,RGB(255,96,206));portrait_draw(16,173,62,62,erl_portrait(rival),TRADERS);text_px(88,177,RGB(123,255,240),rival?"ON THE GRID":"SOLO SESSION");text_px(88,195,RGB(255,240,203),"%s",erl_driver_name(rival));text_px(88,222,RGB(255,96,206),"ERL LIVE");text_px(380,255,RGB(123,255,240),"X SKIP");return 1;
}
static int race_loss_release;static float race_loss_age;
static int race_ui_input(unsigned pressed,unsigned held,float dt){
 if(game.erl.event_kind||!race_ready(&game)||game.race.phase<RACE_LOST||page!=FLIGHT){race_loss_age=0;return 0;}
 race_loss_age+=dt;if(!(held&PSP_CTRL_CROSS))race_loss_release=1;
 if(race_loss_age>.5f&&race_loss_release&&(pressed&(PSP_CTRL_CROSS|PSP_CTRL_CIRCLE))){if(!erl_commit()){message(&game,"Save failed. Keep this session open and press X to retry.");return 1;}game.race.phase=RACE_NONE;game.race.job=-1;selected_target=0;race_loss_release=0;race_loss_age=0;game.cue=SFX_UI;}return 1;
}
static void race_loss_draw(void){
 if(game.erl.event_kind||page!=FLIGHT||!race_ready(&game)||game.race.phase<RACE_LOST)return;
 rect(80,75,320,112,RGB(12,23,34));rect(80,75,320,2,GOLD);
 text_px(104,89,GOLD,game.race.phase==RACE_WON?"RACE CONTRACT / FULFILLED":"RACE CONTRACT / NOT WON");
 if(game.race.arcade.rules.mode==RA_TIME_TRIAL)text_px(104,112,WHITE,"%s   %02d:%05.2f",game.race.place==1?"GOLD":game.race.place==2?"SILVER":game.race.place==3?"BRONZE":"NO MEDAL",(int)game.race.elapsed/60,fmodf(game.race.elapsed,60));
 else if(game.race.arcade.rules.objective==RA_TAKEDOWNS)text_px(104,112,WHITE,"DRONES %d / %u   %02d:%05.2f",game.race.takedowns,game.race.arcade.rules.required,(int)game.race.elapsed/60,fmodf(game.race.elapsed,60));
 else text_px(104,112,WHITE,"FINISHED %d / %d   %02d:%05.2f",game.race.place,game.race.rivals+1,(int)game.race.elapsed/60,fmodf(game.race.elapsed,60));
 text_px(104,135,DIM,"ONE ATTEMPT / RESULT RECORDED");text_px(192,164,CYAN,"X CONTINUE");
}
static void race_ui_sync(void){
 race_broadcast_tick();
 audio_title_active=page==INTRO;
 radio_race_on=page!=INTRO&&race_is_active(&game)&&!game.dead?(game.erl.event_kind?2:1):0;
 if(race_is_active(&game)){autoaim=0;square_held=0;selected_target=RACE_TARGET_ID;}
}
