/* Collision shapes match the solid near-field scenery, leaving the same open
 * corridor used by the renderer. Fixed five-section / nine-shape work budget. */
static int race_slab(Vec3 a,Vec3 b,Vec3 half,float radius){
 float t0=0,t1=1,av[3]={a.x,a.y,a.z},bv[3]={b.x,b.y,b.z},hv[3]={half.x+radius,half.y+radius,half.z+radius};
 for(int k=0;k<3;k++){float d=bv[k]-av[k];if(fabsf(d)<.001f){if(fabsf(av[k])>hv[k])return 0;continue;}float lo=(-hv[k]-av[k])/d,hi=(hv[k]-av[k])/d;if(lo>hi){float tmp=lo;lo=hi;hi=tmp;}t0=fmaxf(t0,lo);t1=fminf(t1,hi);if(t0>t1)return 0;}return 1;
}
#include "erl-moving-hazards.h"
/* One collision description also drives cosmetic close passes. */
static int race_scenery_shape(const RaceGate *q,Vec3 mid,int theme,int n,Vec3 *centre,Vec3 *half){
 int sphere=theme==0||theme==3;
 if(sphere){float side=n&1?1:-1;*centre=add(mid,add(mul(q->right,side*(4500+n%3*900)),add(mul(q->up,(n%3-1)*3300.f),mul(q->normal,(n-2)*3000.f))));*half=(Vec3){850+n%3*300,0,0};}
 else if(theme==1){if(n==8){*centre=add(mid,mul(q->up,5000));*half=(Vec3){5800,450,5700};}else {int side=n<4?-1:1,index=n%4;*centre=add(mid,mul(q->right,side*4700.f));*half=(Vec3){900,1700,6500};if(index){*centre=add(*centre,mul(q->normal,(index-2)*4300.f));*half=(Vec3){1150,2000,160};}}}
 else {int side=n<4?-1:1,index=n%4;*centre=add(mid,mul(q->right,side*6500.f));*half=(Vec3){1700,1100,6000};if(index){*centre=add(*centre,add(mul(q->normal,(index-2)*4300.f),mul(q->up,1800)));*half=(Vec3){400,900,400};}}
 return sphere;
}
static int race_shape_cross(const RaceGate *q,Vec3 from,Vec3 to,Vec3 centre,Vec3 half,int sphere,float radius){
 if(sphere)return race_segment_distance(from,to,centre)<half.x+radius;
 Vec3 a=sub(from,centre),b=sub(to,centre);a=(Vec3){dot(a,q->right),dot(a,q->up),dot(a,q->normal)};b=(Vec3){dot(b,q->right),dot(b,q->up),dot(b,q->normal)};return race_slab(a,b,half,radius);
}
static int race_scenery_hit(const SpaceRace *r,int gate,Vec3 from,Vec3 to,float radius,float dt,Vec3 *hit){
 if(r->layout==4||r->layout==5)return 0;
 if(race_moving_hazard_hit(r,gate,from,to,radius,dt,hit))return 1;
 int start=gate>1?gate-2:0,end=start+5;if(end>RACE_GATES)end=RACE_GATES;
 for(int k=start;k<end;k++){const RaceGate *q=&r->gates[k];Vec3 prev=k?r->gates[k-1].pos:r->start,mid=mul(add(prev,q->pos),.5f);if(race_segment_distance(from,to,mid)>14000)continue;int theme=k/8%4;
  for(int n=0;n<(theme==0||theme==3?5:theme==1?9:8);n++){
   Vec3 centre,half;int sphere=race_scenery_shape(q,mid,theme,n,&centre,&half);
   int collides=race_shape_cross(q,from,to,centre,half,sphere,radius);
   if(collides){if(hit)*hit=to;return 1;}
  }
 }return 0;
}
int race_scenery_near(const SpaceRace *r,int gate,Vec3 from,Vec3 to){
 Vec3 travel=sub(to,from);if(dot(travel,travel)<400)return 0;
 int start=gate>1?gate-2:0,end=start+5;if(end>RACE_GATES)end=RACE_GATES;
 for(int k=start;k<end;k++){const RaceGate *q=&r->gates[k];Vec3 prev=k?r->gates[k-1].pos:r->start,mid=mul(add(prev,q->pos),.5f);if(race_segment_distance(from,to,mid)>14000)continue;int theme=k/8%4;
  for(int n=0;n<(theme==0||theme==3?5:theme==1?9:8);n++){
   Vec3 centre,half;int sphere=race_scenery_shape(q,mid,theme,n,&centre,&half);
   /* Trigger at closest approach, never continuously while alongside a wall. */
   if(dot(sub(from,centre),travel)>=0||dot(sub(to,centre),travel)<0)continue;
   if(race_shape_cross(q,from,to,centre,half,sphere,265)&&!race_shape_cross(q,from,to,centre,half,sphere,85))return 1;
  }
 }return 0;
}
static Vec3 race_rejoin_point(const SpaceRace *r,int gate){const RaceGate *q=&r->gates[gate<RACE_GATES?gate:RACE_GATES-1];return add(gate?r->gates[gate-1].pos:r->start,mul(q->normal,300));}
static void race_player_repair(Game *g){
 SpaceRace *r=&g->race;r->knockouts++;r->repair=3;r->health=0;r->feed_code=4;r->feed_age=3;g->speed=0;g->boost=0;
 if(r->layout==5){
  Vec3 wreck=g->pos,heading=forward(g);
  /* The existing repair timer is the elapsed-time penalty. Rejoin the last
   * confirmed section on its real floor; retain every ordered checkpoint. */
  int gate=r->gate<RACE_GATES?r->gate:RACE_GATES-1;RaceTrackFrame f=race_track_frame(r,gate,0);
  g->yaw=atan2f(f.ahead.x,f.ahead.z);g->pitch=asinf(fmaxf(-1,fminf(1,f.ahead.y)));
  Vec3 right={cosf(g->yaw),0,-sinf(g->yaw)},up=norm(race_cross(f.ahead,right));g->roll=atan2f(-dot(f.up,right),dot(f.up,up));
  g->pos=add(f.centre,add(mul(f.up,RACE_TRACK_FLOOR+race_floor_support(g,0,f)+35),mul(f.ahead,150)));
  r->arcade.actor[0].health=0;r->arcade.actor[0].rocket=0;r->boost_time=r->handover=0;race_crash_award_flush(g);r->crash_time=3;r->crash_id=-1;r->crash_pos=wreck;r->crash_dir=heading;
  r->protection=5;g->cue=SFX_DEATH;return;
 }
 {RaceGate *q=&r->gates[r->gate<RACE_GATES?r->gate:RACE_GATES-1];Vec3 a=r->gate?r->gates[r->gate-1].pos:r->start,axis=sub(q->pos,a);float t=fmaxf(0,fminf(.98f,dot(sub(g->pos,a),axis)/fmaxf(1,dot(axis,axis)))-.025f);g->pos=add(a,mul(axis,t));}g->yaw=atan2f(r->gates[r->gate].normal.x,r->gates[r->gate].normal.z);g->pitch=asinf(r->gates[r->gate].normal.y);g->roll=0;r->protection=5;g->cue=SFX_DEATH;
}
