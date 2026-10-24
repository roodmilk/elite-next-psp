/* One shared, allocation-free, time-based motion state. No save-layout fields. */
#include "charm-assets.h"
typedef struct {float angle,angular_velocity,depth,depth_velocity;} CharmMotion;
static const float charm_spring[]={29,20,24,36},charm_damping[]={2.8f,2.1f,2.4f,3.7f},charm_length[]={31,32,30,33};
static CharmMotion charm_motion;
static float charm_last_time=-1,charm_last_energy,charm_last_hull,charm_last_speed;
static float charm_last_yaw,charm_last_roll;
static Vec3 charm_last_velocity,charm_last_position;
static int charm_last_item=-1,charm_last_ship=-1,charm_last_system=-1;
static unsigned charm_draw_calls;static uint64_t charm_draw_us;
static float charm_clamp(float x,float lo,float hi){return fmaxf(lo,fminf(hi,x));}
static void charm_step(CharmMotion*m,float dt,float lateral,float longitudinal,float spring,float damping){
 dt=charm_clamp(dt,0,.10f);int n=(int)ceilf(dt*120);if(n<1)n=1;float h=dt/n;
 lateral=charm_clamp(lateral,-1,1);longitudinal=charm_clamp(longitudinal,-1,1);
 for(int i=0;i<n;i++){
  m->angular_velocity+=(-spring*m->angle-damping*m->angular_velocity+lateral*12)*h;m->angle+=m->angular_velocity*h;
  m->depth_velocity+=(-spring*m->depth-damping*m->depth_velocity+longitudinal*9)*h;m->depth+=m->depth_velocity*h;
  if(fabsf(m->angle)>.52f){m->angle=charm_clamp(m->angle,-.52f,.52f);if(m->angle*m->angular_velocity>0)m->angular_velocity=0;}
  if(fabsf(m->depth)>.35f){m->depth=charm_clamp(m->depth,-.35f,.35f);if(m->depth*m->depth_velocity>0)m->depth_velocity=0;}
 }
}
static void charm_tick(void){
 int slot=charm_installed(&game),item=slot<0?-1:game.fit[slot];float dt=game.time-charm_last_time;Vec3 f=forward(&game),v=mul(f,game.speed);
 int reset=item!=charm_last_item||game.ship!=charm_last_ship||game.system!=charm_last_system||dt<0||dt>.15f||length(sub(game.pos,charm_last_position))>fmaxf(1000,fabsf(game.speed)*fmaxf(0,dt)*5);
 if(page!=FLIGHT||game.docked||game.planet>=0||game.jump>0)reset=1;
 if(reset){memset(&charm_motion,0,sizeof(charm_motion));}
 else if(item>=CHARM_UFO&&dt>0&&!paused){
  Vec3 acceleration=mul(sub(v,charm_last_velocity),1.f/dt),right={cosf(game.yaw),0,-sinf(game.yaw)};
  float lateral=-dot(acceleration,right)/300.f-sinf(game.yaw-charm_last_yaw)/dt*.25f-sinf(game.roll-charm_last_roll)/dt*.18f,longitudinal=-dot(acceleration,f)/500.f;
  float hit=fmaxf(0,charm_last_hull-game.hull)+fmaxf(0,charm_last_energy-game.energy);
  if(hit>.1f){float kick=fminf(1.6f,.3f+hit*.035f);charm_motion.angular_velocity+=(lateral<0?-kick:kick);charm_motion.depth_velocity+=kick*.7f;}
  charm_step(&charm_motion,dt,lateral,longitudinal,charm_spring[item-CHARM_UFO],charm_damping[item-CHARM_UFO]);
 }
 charm_last_yaw=game.yaw;charm_last_roll=game.roll;charm_last_time=game.time;charm_last_velocity=v;charm_last_position=game.pos;charm_last_energy=game.energy;charm_last_hull=game.hull;charm_last_speed=game.speed;charm_last_item=item;charm_last_ship=game.ship;charm_last_system=game.system;
}
static void charm_draw(void){
 int slot=charm_installed(&game);if(slot<0||!hc_presentation_context()||third_person||!hc_full_console_visible())return;int item=game.fit[slot]-CHARM_UFO;if(item<0||item>3)return;
 uint64_t start=sceKernelGetSystemTimeWide();float angle=charm_motion.angle,roll=-angle*(item==3?.85f:.65f),cs=cosf(roll),sn=sinf(roll),ax=item==3?19:16,ay=8;
 float ex=71+sinf(angle)*charm_length[item],ey=33+cosf(angle)*charm_length[item]+charm_motion.depth*13;
 line(71,33,(int)lroundf(ex),(int)lroundf(ey),RGB(152,174,176));pixel(71,33,RGB(216,225,222));
 /* Inverse nearest-neighbour sampling retains holes, outlines and every RGBA colour.
    Rotation is around the supplied attachment point, never the sprite centre. */
 float loX=1e9f,hiX=-1e9f,loY=1e9f,hiY=-1e9f;for(int k=0;k<4;k++){float u=((k&1)?31:0)-ax,v=((k&2)?31:0)-ay;float x=ex+cs*u-sn*v,y=ey+sn*u+cs*v;loX=fminf(loX,x);hiX=fmaxf(hiX,x);loY=fminf(loY,y);hiY=fmaxf(hiY,y);}
 for(int y=(int)floorf(loY)-1;y<=(int)ceilf(hiY)+1;y++)for(int x=(int)floorf(loX)-1;x<=(int)ceilf(hiX)+1;x++){
  if(x<0||x>=W||y<34||y>=164)continue;float dx=x-ex,dy=y-ey;
  int sx=(int)floorf(cs*dx+sn*dy+ax+.5f),sy=(int)floorf(-sn*dx+cs*dy+ay+.5f);if(sx<0||sx>=32||sy<0||sy>=32)continue;
  unsigned c=charm_pixels[item][sy*32+sx],a=c>>24;if(!a)continue;if(a==255){pixel(x,y,c);continue;}
  unsigned b=fb[y*STRIDE+x],out=0xff000000u;for(int k=0;k<24;k+=8)out|=((((c>>k)&255)*a+((b>>k)&255)*(255-a)+127)/255)<<k;pixel(x,y,out);
 }
 charm_draw_calls++;charm_draw_us+=sceKernelGetSystemTimeWide()-start;
}
