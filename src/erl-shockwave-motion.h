/* Presentation coordinates only. No Game mutation, RNG or reward callbacks. */
#ifndef ERL_SHOCKWAVE_MOTION_H
#define ERL_SHOCKWAVE_MOTION_H
#define SW_IMPACT 3.81f
#define SW_READY 7.36f
typedef struct {Vec3 eye,forward,right,up,ship;float bank,time;} ErlShockPose;
static float sw_clamp(float v){return fmaxf(0,fminf(1,v));}
static float sw_ease(float v){v=sw_clamp(v);return v*v*(3-2*v);}
static float sw_mix(float a,float b,float t){return a+(b-a)*t;}
static Vec3 sw_cross(Vec3 a,Vec3 b){return (Vec3){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
static float sw_time(float wall){
 float t=wall+1.1f;if(t<=SW_IMPACT)return t;float a=t-SW_IMPACT;
 if(a<=.4f)return SW_IMPACT+a*.22f;
 if(a<=.9f){float q=a-.4f;return SW_IMPACT+.088f+.22f*q+.78f*(q*q*q/.25f-.5f*q*q*q*q/.125f);}
 return t-.507f;
}
static ErlShockPose sw_pose(float wall){
 ErlShockPose p={0};float t=sw_time(wall);p.time=t;
 if(t<2.95f){
  float travel=sw_clamp((t-1.35f)/1.6f),launch=travel*travel,zoom=sw_ease(t/1.35f),phase=sw_clamp((t-2.15f)/.8f);
  float turn=phase*phase*phase*(phase*(phase*6-15)+10),base=-5100;
  p.ship=(Vec3){0,0,base+launch*2400};Vec3 initial={sw_mix(0,28,zoom),sw_mix(8,55,zoom),base+sw_mix(-145,-240,zoom)};
  Vec3 target={0,0,base+fminf(launch*2400,700)};
  Vec3 dir=norm(t<2.15f?sub(target,initial):(Vec3){-28,-55,840});
  float yaw=sw_mix(atan2f(-dir.x,dir.z),1.570796327f,turn),pitch=sw_mix(asinf(-dir.y),atan2f(62,230),turn);
  p.forward=(Vec3){-sinf(yaw)*cosf(pitch),-sinf(pitch),cosf(yaw)*cosf(pitch)};
  p.right=norm(sw_cross((Vec3){0,1,0},p.forward));p.up=sw_cross(p.forward,p.right);
  Vec3 ir=norm(sw_cross((Vec3){0,1,0},dir)),iu=sw_cross(dir,ir),relative=sub(p.ship,initial);
  float depth=sw_mix(dot(relative,dir),sqrtf(230*230+62*62),turn),horizontal=sw_mix(dot(relative,ir),p.ship.z,turn*turn*turn),vertical=sw_mix(dot(relative,iu),0,turn);
  p.eye=sub(p.ship,add(add(mul(p.forward,depth),mul(p.right,horizontal)),mul(p.up,vertical)));
  p.bank=sinf(t*2)*.018f-.18f*turn;
 }else{
  float speed=5400/1.7f,age=fmaxf(0,t-SW_IMPACT),arc=sw_ease(age/1.85f),impactz=-2700+(SW_IMPACT-2.95f)*speed;
  p.ship=(Vec3){0,0,-2700+(t-2.95f)*speed};p.bank=sw_mix(-.18f,-.13f,arc)+sinf(t*2)*.035f*arc;
  float yaw=sw_mix(1.570796327f,3.141592654f-.4f,arc),pitch=sw_mix(atan2f(62,230),atan2f(105,245),arc);
  p.forward=(Vec3){-sinf(yaw)*cosf(pitch),-sinf(pitch),cosf(yaw)*cosf(pitch)};
  p.right=norm(sw_cross((Vec3){0,1,0},p.forward));p.up=sw_cross(p.forward,p.right);
  if(t<SW_IMPACT)p.eye=(Vec3){230,62,0};
  else {float depth=sw_mix(sqrtf(230*230+62*62),sqrtf(245*245+105*105),sw_ease(age/1.2f));
   float horizontal=impactz*expf(-age*4)+(speed+4*impactz)*age*expf(-age*12);
   p.eye=sub(p.ship,add(mul(p.forward,depth),mul(p.right,horizontal)));
  }
 }
 /* The entire choreography travels through ONE seeded, finite world. */
 float cruise=24000*(t-1.1f);p.eye.z+=cruise;p.ship.z+=cruise;return p;
}
#endif
