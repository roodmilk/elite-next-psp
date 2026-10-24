/* Fixed 15-craft snapshot: every planner sees the same beginning-of-tick
 * traffic. Intent persists briefly; safety is rechecked before every merge.
 * These are steering targets and speed limits, never position/checkpoint edits. */
typedef struct {Vec3 pos,dir;float speed,radius;int gate,live;} ErlTrafficBody;
typedef struct {float lane,hold;int state;} ErlTrafficIntent;
static struct {ErlTrafficBody body[RA_ACTORS];ErlTrafficIntent intent[RA_ACTORS];const Game *owner;unsigned serial,seed;float age;} erl_traffic;
static void race_neon_traffic_prepare(const Game *g){
 const SpaceRace *r=&g->race;if(r->layout!=5)return;
 if(erl_traffic.owner!=g||erl_traffic.serial!=g->erl.serial||erl_traffic.seed!=r->seed||r->elapsed<erl_traffic.age){
  memset(&erl_traffic,0,sizeof(erl_traffic));erl_traffic.owner=g;erl_traffic.serial=g->erl.serial;erl_traffic.seed=r->seed;
  for(int id=1;id<=r->rivals;id++){RaceTrackFrame f=race_track_nearest(r,r->pilot[id-1].gate,r->pilot[id-1].pos);erl_traffic.intent[id].lane=dot(sub(r->pilot[id-1].pos,f.centre),f.right);}
 }
 erl_traffic.age=r->elapsed;
 for(int id=0;id<RA_ACTORS;id++){
  ErlTrafficBody *b=&erl_traffic.body[id];b->live=id<=r->rivals&&ra_eligible(g,id);
  if(!b->live)continue;b->pos=ra_position(g,id);b->dir=ra_direction(g,id);b->speed=ra_speed(g,id);b->gate=id?r->pilot[id-1].gate:r->gate;b->radius=id?r->pilot[id-1].radius:race_neon_body_radius(g);
 }
}
/* Predict closest relative contact over a short horizon in the actual banked
 * local frame. Nearby turns/helix levels are filtered by real world distance. */
static int race_neon_lane_clear(int id,RaceTrackFrame f,float lane,float height,float horizon){
 const ErlTrafficBody *a=&erl_traffic.body[id];float here=dot(sub(a->pos,f.centre),f.right),rise=dot(sub(a->pos,f.centre),f.up);
 Vec3 move=add(mul(a->dir,a->speed*horizon),add(mul(f.right,lane-here),mul(f.up,height-rise)));
 for(int other=0;other<RA_ACTORS;other++){
  const ErlTrafficBody *b=&erl_traffic.body[other];if(other==id||!b->live||abs(b->gate-a->gate)>1)continue;
  Vec3 delta=sub(b->pos,a->pos);if(dot(delta,delta)>16000.f*16000.f)continue;
  float radius=a->radius+b->radius+80;
  Vec3 v=sub(mul(b->dir,b->speed*horizon),move);
  Vec3 start={dot(delta,f.right)/radius,dot(delta,f.up)/radius,dot(delta,f.ahead)/(radius*1.4f)};
  Vec3 step={dot(v,f.right)/radius,dot(v,f.up)/radius,dot(v,f.ahead)/(radius*1.4f)};
  float t=fmaxf(0,fminf(1,-dot(start,step)/fmaxf(.0001f,dot(step,step))));Vec3 closest=add(start,mul(step,t));
  if(dot(closest,closest)<1)return 0;
 }
 return 1;
}
static Vec3 race_neon_traffic_aim(Game *g,int id,RaceTrackFrame now,float dt,float *limit){
 SpaceRace *r=&g->race;RaceRival *p=&r->pilot[id-1];ErlTrafficIntent *intent=&erl_traffic.intent[id];
 float scale=race_neon_scale(r),at=now.t+.08f;int section=now.section;if(at>1){section++;at-=1;}
 RaceTrackFrame ahead=race_track_frame(r,section,at);int kind=race_track_surface(r,ahead.section,ahead.t);
 float edge=(kind==1?RACE_NEON_EDGE:190)*2.5f*scale-p->radius-100;
 float lane=dot(sub(p->pos,now.centre),now.right),height=dot(sub(p->pos,now.centre),now.up);
 float lift=kind==0?(90+70*sinf(r->elapsed*.32f+id-1))*scale:0;
 float target_height=RACE_TRACK_FLOOR+race_floor_support(g,id,ahead)+35+lift;
 float preferred=sinf((now.section+now.t)*.7f+id*1.7f)*fminf(1400,edge*.55f),pad_t,pad_lane;
 if(race_neon_pad(r,now.section,&pad_t,&pad_lane)&&now.t>pad_t-.30f&&now.t<pad_t+.06f&&!(r->arcade.branch_boost[id]&(1u<<now.section))){
  preferred=pad_lane;target_height=RACE_TRACK_FLOOR+race_floor_support(g,id,ahead)+45;
 }
 float clock=neon_time(((now.section%(RACE_GATES/r->laps))+now.t)/(RACE_GATES/r->laps));
 if(erl_service_clock(clock)&&r->arcade.actor[id].health<RA_HEALTH*.45f){preferred=190*2.5f*scale*ERL_SERVICE_LANE;target_height=RACE_TRACK_FLOOR+race_floor_support(g,id,ahead)+35;}
 preferred=fmaxf(-edge,fminf(edge,preferred));intent->hold=fmaxf(0,intent->hold-dt);
 int alongside=0,blocked=0;float front=1e9f,front_speed=*limit;
 for(int other=0;other<=r->rivals;other++){
  ErlTrafficBody *b=&erl_traffic.body[other];if(other==id||!b->live||abs(b->gate-p->gate)>1)continue;
  Vec3 delta=sub(b->pos,p->pos);float x=dot(delta,now.right),y=dot(delta,now.up),z=dot(delta,now.ahead),radius=p->radius+b->radius+100;
  if(fabsf(y)>radius)continue;
  if(fabsf(z)<radius*1.4f&&fabsf(x)<radius*2.5f)alongside=1;
  if(z>0&&z<front&&fabsf(x)<radius&&z<radius*1.5f+fmaxf(0,p->speed-b->speed)*.8f){front=z;front_speed=b->speed;blocked=1;}
 }
 float chosen=intent->hold>0?intent->lane:preferred;chosen=fmaxf(-edge,fminf(edge,chosen));
 int clear=race_neon_lane_clear(id,now,chosen,height+(target_height-height)*.45f,.65f);
 if(alongside){chosen=lane;intent->state=2;intent->hold=.35f;}
 else if(!clear||blocked){
  float best=1e9f;int found=0;
  for(int n=-2;n<=2;n++){
   float candidate=n*edge*.42f;
   if(!race_neon_lane_clear(id,now,candidate,height+(target_height-height)*.45f,.65f))continue;
   float cost=fabsf(candidate-lane)+fabsf(candidate-preferred)*.25f;
   if(cost<best){best=cost;chosen=candidate;found=1;}
  }
  intent->state=found?1:3;intent->hold=.7f;
  if(!found){chosen=lane;*limit=fminf(*limit,fmaxf(r->top_speed*.25f,front_speed-200));}
 }else if(intent->hold<=0)intent->state=0;
 intent->lane=chosen;
 /* Leave a physical closing gap while the lateral pass is still developing. */
 if(blocked&&fabsf(chosen-lane)<p->radius*2)*limit=fminf(*limit,fmaxf(r->top_speed*.25f,front_speed));
 float reach=r->top_speed*.18f*.65f;chosen=lane+fmaxf(-reach,fminf(reach,chosen-lane));
 float roof=sqrtf(fmaxf(0,(edge+100)*(edge+100)-chosen*chosen))+RACE_TRACK_FLOOR;
 target_height=fminf(target_height,roof-50);
 return add(ahead.centre,add(mul(ahead.right,chosen),mul(ahead.up,target_height)));
}

#ifdef ELITE_ENABLE_REVIEW
unsigned race_polish_traffic_review(Game *g){
 SpaceRace saved=g->race;Vec3 pos=g->pos;unsigned serial=g->erl.serial,flags=0;SpaceRace *r=&g->race;
 r->rivals=2;r->gate=0;r->phase=RACE_RUNNING;r->elapsed=10;g->erl.serial++;
 RaceTrackFrame f=race_track_frame(r,0,.45f);float h=RACE_TRACK_FLOOR+80;
 g->pos=add(f.centre,mul(f.ahead,-30000));r->arcade.actor[0].eliminated=1;
 for(int i=0;i<RACE_RIVALS;i++){memset(&r->pilot[i],0,sizeof(r->pilot[i]));r->pilot[i].radius=240;r->pilot[i].dir=f.ahead;r->pilot[i].pos=add(f.centre,mul(f.up,h));r->pilot[i].speed=i?4400:6000;ra_actor_reset(&r->arcade.actor[i+1]);}
 r->pilot[1].pos=add(r->pilot[1].pos,mul(f.ahead,900));race_neon_traffic_prepare(g);float limit=r->top_speed*ERL_MANUAL_SURGE;
 race_neon_traffic_aim(g,1,f,.033334f,&limit);if(erl_traffic.intent[1].state==1&&fabsf(erl_traffic.intent[1].lane)>500)flags|=1;
 r->rivals=6;float edge=190*2.5f*race_neon_scale(r)-340;
 for(int i=1;i<=5;i++){r->pilot[i].pos=add(r->pilot[0].pos,add(mul(f.ahead,900),mul(f.right,(i-3)*edge*.42f)));r->pilot[i].speed=4400;}
 g->erl.serial++;race_neon_traffic_prepare(g);limit=r->top_speed*ERL_MANUAL_SURGE;race_neon_traffic_aim(g,1,f,.033334f,&limit);
 if(erl_traffic.intent[1].state==3&&limit<=4400)flags|=2;
 r->rivals=2;r->pilot[1].pos=add(r->pilot[0].pos,mul(f.right,900));r->pilot[1].speed=6000;
 g->erl.serial++;race_neon_traffic_prepare(g);limit=r->top_speed*ERL_MANUAL_SURGE;race_neon_traffic_aim(g,1,f,.033334f,&limit);
 if(erl_traffic.intent[1].state==2&&fabsf(erl_traffic.intent[1].lane)<1)flags|=4;
 if(!race_neon_lane_clear(1,f,900,h,.65f))flags|=8;
 /* Another helix level cannot block a nearby lane merely by sharing a gate. */
 r->pilot[1].pos=add(r->pilot[0].pos,mul(f.up,6000));g->erl.serial++;race_neon_traffic_prepare(g);
 if(race_neon_lane_clear(1,f,900,h,.65f))flags|=16;
 r->pilot[0].pos=add(f.centre,mul(f.up,h));r->pilot[1].pos=add(r->pilot[0].pos,mul(f.right,100));r->pilot[0].bump=r->pilot[1].bump=.5f;r->pilot[0].speed=r->pilot[1].speed=2000;r->pilot[0].pace=r->pilot[1].pace=2000;
 r->arcade.actor[0].eliminated=1;r->protection=0;g->erl.serial++;race_tick(g,g->pos,.001f);
 if(length(sub(r->pilot[0].pos,r->pilot[1].pos))>=479.9f&&r->pilot[0].speed>1990&&r->pilot[1].speed>1990)flags|=32;
 g->race=saved;g->pos=pos;g->erl.serial=serial;return flags;
}
#endif
