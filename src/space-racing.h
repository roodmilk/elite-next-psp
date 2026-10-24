/* Bounded winding arcade courses, actual directional aperture crossings and racers.
 * ERL checkpoints persist the complete event; legacy exhibition jobs keep their grid restart. */
static void race_arcade_init(Game *g);
static Vec3 race_cross(Vec3 a,Vec3 b){return (Vec3){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
#include "erl-neon-course.h"
#include "erl-neon-rings-data.h"
float race_neon_scale(const SpaceRace *r){return r->top_speed*46.f/NEON_SOURCE_LENGTH;}
float race_neon_clock(float arc){return neon_time(arc);}
int race_job(const Game *g){for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==MISSION_RACE&&g->jobs[i].dest==g->system)return i;return expedition_race_job(g);}
int race_ready(const Game *g){return (race_job(g)>=0||g->erl.event_kind||(g->race.phase>=RACE_LOST&&g->race.job>=0))&&g->race.phase!=RACE_NONE&&g->race.system==g->system;}
int race_is_active(const Game *g){return race_ready(g)&&(g->race.phase==RACE_COUNTDOWN||g->race.phase==RACE_RUNNING);}
int race_boost_visible(const SpaceRace *r,int section){return section>=0&&section<RACE_GATES&&!r->gates[section].boost_used&&!((r->arcade.branch_boost[0]|r->arcade.branch_repair[0])&(1u<<section));}
float race_crossing(Vec3 from,Vec3 to,Vec3 center,Vec3 normal,float radius){
 float a=dot(sub(from,center),normal),b=dot(sub(to,center),normal);
 if(a>0||b<=0||b-a<.0001f)return -1;
 float t=-a/(b-a);Vec3 hit=add(from,mul(sub(to,from),t));
 return length(sub(hit,center))<=radius?t:-1;
}
static float race_segment_distance(Vec3 a,Vec3 b,Vec3 p){Vec3 d=sub(b,a);float t=fmaxf(0,fminf(1,dot(sub(p,a),d)/fmaxf(1,dot(d,d))));return length(sub(p,add(a,mul(d,t))));}
/* Freeze the performance class when the grid is built. Modules/pips affect
 * this class; they cannot bypass its reserve or make the course a short sprint. */
static float race_top_speed(const Game *g){(void)g;return 8000.f;}
static float race_hull_radius(int mesh){
 const Mesh *m=&meshes[mesh];float radius=0;
 for(int k=0;k<m->vertices;k++)radius=fmaxf(radius,length(m->v[k]));
 return radius*RACE_SHIP_SCALE;
}
static void race_reserve_tick(float *reserve,float *delay,int boosting,float dt){
 if(boosting){*reserve=fmaxf(0,*reserve-dt*(100.f/RACE_BOOST_SECONDS));*delay=.55f;}
 else {float charging=fmaxf(0,dt-*delay);*delay=fmaxf(0,*delay-dt);*reserve=fminf(100,*reserve+charging*4.f);} /* 25 seconds empty-to-full; no recharge while spending. */
}
int race_course_clear(const Game *g,const SpaceRace *r){
 Vec3 prev=add(r->start,mul(r->dir,-1600));
 for(int k=0;k<RACE_GATES;k++){
  const RaceGate *gate=&r->gates[k];
  for(int i=0;i<BODY_COUNT;i++)if(body_active(&g->bodies[i])&&race_segment_distance(prev,gate->pos,g->bodies[i].pos)<g->bodies[i].radius*2.8f+4000)return 0;
  for(int h=0;h<HUB_COUNT;h++)if(race_segment_distance(prev,gate->pos,hub_position(g,h))<7000)return 0;
  prev=gate->pos;
 }
 return 1;
}
int race_course_build(const Game *g,int hub,SpaceRace *r){
 memset(r,0,sizeof(*r));r->system=g->system;r->hub=hub;r->phase=RACE_GRID;
 r->seed=sector_hash((g->system+1)*12347u+(hub+1)*811u);r->rivals=5+(r->seed%6);r->place=1;
 r->top_speed=g->erl.event_kind?8000.f:race_top_speed(g);r->reserve=100;
 r->discipline=g->erl.event_kind?g->erl.event_type:ERL_SPRINT;r->layout=g->erl.event_layout;
 r->gate_count=32;r->laps=r->discipline==ERL_CIRCUIT?2:1;r->limit=120;r->target_score=5;r->health=100;
 int job=race_job(g);
 RaceRules entry=g->erl.event_kind?g->erl_local.event_rules:job>=0&&g->jobs[job].type==MISSION_RACE?race_contract_rules(g,&g->jobs[job],r->seed):erl_catalogue_rules(r->seed,g->erl.licence,(int)(r->seed%18));
 if(!g->erl.event_kind&&job>=0&&g->jobs[job].type==MISSION_RACE)r->discipline=ERL_CIRCUIT;
 if(ra_rules_valid(&entry,g->erl.licence)){r->rivals=entry.objective==RA_TAKEDOWNS?6:entry.field-1;r->laps=entry.laps;r->limit=entry.seconds;}
 else if(g->erl.event_kind)r->rivals=g->erl.event_kind==ERL_INTRO?1:6;
 /* New courses use a distinct saved layout ID. Existing accepted races keep
  * their original geometry and resume semantics. */
 if(g->erl.event_kind&&g->erl.event_kind!=ERL_INTRO&&g->erl.event_layout==0&&r->discipline==ERL_CIRCUIT)r->layout=5;
 float angle=(r->seed%6283)*.001f;r->dir=norm((Vec3){sinf(angle),.23f,cosf(angle)});
 r->right=norm(race_cross((Vec3){0,1,0},r->dir));r->up=norm(race_cross(r->dir,r->right));
 for(int attempt=0;attempt<8;attempt++){
  r->start=add(hub_position(g,hub),(Vec3){5000,80000+attempt*100000.f,8000});Vec3 prev=r->start;r->length=0;
  for(int k=0;k<RACE_GATES;k++){
   RaceGate *q=&r->gates[k];
   /* Heading changes, rather than tiny offsets on a straight line: alternating
    * S bends, climbing turns and descending chicanes. First leg is aligned. */
   static const float turns[32]={0,.18f,.48f,.92f,1.15f,.62f,-.12f,-.78f,-1.02f,-.5f,.22f,.85f,1.22f,.65f,-.20f,-.86f,-1.18f,-.56f,.18f,.72f,1.02f,.4f,-.34f,-.95f,-1.15f,-.55f,.16f,.78f,1.1f,.58f,.08f,0};
   float heading=turns[(k+(r->layout%4)*8)%32]-turns[(r->layout%4)*8];if(!k)heading=0;
   float climb=k?.34f*sinf(k*.91f):0,span=r->top_speed*102.f/RACE_GATES*(.94f+(sector_hash(r->seed+k*397u)%1200)*.0001f);
   Vec3 course=norm(add(mul(r->dir,cosf(heading)*cosf(climb)),add(mul(r->right,sinf(heading)*cosf(climb)),mul(r->up,sinf(climb)))));
   if(g->erl.event_kind==ERL_INTRO)span*=.62f;
   if(r->layout==5){
    r->dir=norm((Vec3){sinf(angle),0,cosf(angle)});r->right=norm(race_cross((Vec3){0,1,0},r->dir));r->up=(Vec3){0,1,0};
    RaceTrackFrame f=neon_frame(r,k,1);q->pos=f.centre;course=f.ahead;span=length(sub(q->pos,prev));
   }else if(r->layout==4){
    int per=RACE_GATES/r->laps;float theta=6.283185307f*(k%per+1)/per;
    float radius=r->top_speed*100.f/(6.283185307f*r->laps);
    r->dir=norm((Vec3){sinf(angle),0,cosf(angle)});r->right=norm(race_cross((Vec3){0,1,0},r->dir));r->up=(Vec3){0,1,0};
    q->pos=(k%per==per-1)?r->start:add(r->start,add(mul(r->right,radius*(1-cosf(theta))),mul(r->dir,radius*sinf(theta))));
    course=norm(sub(q->pos,prev));span=length(sub(q->pos,prev));
   }else if(r->discipline==ERL_CIRCUIT||r->discipline>=ERL_RUSH){
    /* Authored closed figure with a broad hairpin, chicane and altitude change.
       Repeated coordinates are intentional laps, not new random gates. */
    static const float path[17][3]={{0,0,0},{0,.12f,1},{.2f,.25f,2},{.8f,.4f,2.8f},{1.8f,.35f,3.1f},{2.8f,.18f,2.8f},{3.1f,0,1.8f},{2.7f,-.22f,.8f},{2,-.3f,.2f},{1.3f,-.1f,.7f},{1.5f,.16f,1.5f},{.9f,.28f,1.9f},{.35f,.15f,1.4f},{-.4f,0,.8f},{-.8f,-.1f,.1f},{-.5f,-.06f,-.6f},{0,0,0}};
    static const float grand[17][3]={{0,0,0},{0,0,1},{-.4f,.1f,1.9f},{-1.2f,.35f,2.5f},{-2.2f,.6f,2.4f},{-3.0f,.4f,1.7f},{-3.2f,.1f,.7f},{-2.6f,-.2f,0},{-1.7f,-.3f,-.1f},{-1.3f,-.1f,.7f},{-.6f,.15f,1.4f},{.3f,.25f,1.5f},{1.1f,.1f,.9f},{1.4f,-.2f,0},{1,-.3f,-.8f},{.3f,-.1f,-.7f},{0,0,0}};
    int per_lap=RACE_GATES/r->laps;if(per_lap<4)per_lap=4;float sample=(k%per_lap+1)*16.f/per_lap;int n=(int)sample;if(n>16)n=16;int next=n<16?n+1:16;float fraction=sample-n;const float (*shape)[3]=r->layout==3?grand:path;float v[3];for(int axis=0;axis<3;axis++)v[axis]=shape[n][axis]+(shape[next][axis]-shape[n][axis])*fraction;float scale=r->top_speed*(r->layout==3?3.f:3.2f)*(entry.objective==RA_ENDURANCE?1.5f:1.f),mirror=(r->seed&128)?-1.f:1.f;
    q->pos=add(r->start,add(mul(r->right,v[0]*scale*mirror),add(mul(r->up,v[1]*scale),mul(r->dir,v[2]*scale))));course=norm(sub(q->pos,prev));span=length(sub(q->pos,prev));
   }else q->pos=add(prev,mul(course,span));r->length+=span;
   q->normal=course;q->right=norm(race_cross((Vec3){0,1,0},q->normal));q->up=norm(race_cross(q->normal,q->right));q->radius=k%8>=5?1800:2500;
   if(r->layout==5){RaceTrackFrame f=neon_frame(r,k,1);q->right=f.right;q->up=f.up;q->radius=3200;}
   q->booster=add(mul(add(prev,q->pos),.5f),mul(q->right,(k&2)?-450.f:450.f));prev=q->pos;
  }
  if(race_course_clear(g,r)){for(int k=0;k<RACE_GATES;k++)r->gates[k].booster=race_pad_point(r,k,.5f,r->layout==5?race_neon_pad_lane(r,k):0);return 1;}
 }
 memset(r,0,sizeof(*r));return 0;
}
static void race_grid_ships(Game *g){
 /* The ERL grid uses real selectable, sleek player hulls. Every entry has a
  * verified race silhouette and engine placement; NPC-era utility hulls do
  * not leak into the championship field. */
 SpaceRace *r=&g->race;const char *hulls[]={"FER DE LANCE","KRAIT","OPHIDIAN","MORAY","GECKO","COBRA MK 1","COBRA MK 3"};
 race_arcade_init(g);
 float max=r->top_speed;
 for(int i=0;i<r->rivals;i++){
  RaceRival *p=&r->pilot[i];memset(p,0,sizeof(*p));p->mesh=mesh_id(hulls[(i+r->seed)%7]);p->finished=-1;p->health=100;p->boost_gate=-1;
  p->pace=max*(r->layout==5?.66f+((sector_hash(r->seed+i*7919u)>>8)%5)*.01f:.82f+((sector_hash(r->seed+i*7919u)>>8)%7)*.02f);p->reserve=100;p->radius=240;
  int lane=i%5-2,row=i/5;p->pos=add(r->start,add(mul(r->dir,-300-row*280),add(mul(r->right,lane*320.f),mul(r->up,75+row*140.f))));p->dir=r->dir;
  if(r->arcade.rules.objective==RA_TAKEDOWNS){p->mesh=mesh_id("MONITOR");p->pos=add(r->start,add(mul(r->dir,3500+i*1100),mul(r->right,((i%3)-1)*900.f)));p->pace=max*.55f;}
 }
}
void race_prepare(Game *g){
 int j=race_job(g);SpaceRace *r=&g->race;
 if(g->erl.event_kind&&g->erl.event_system!=g->system)return;
 if(j<0&&!g->erl.event_kind){if(r->phase<RACE_LOST)r->phase=RACE_NONE;return;}
 const ExpeditionStage *xp=j>=0?expedition_stage(g,j):0;int hub=g->erl.event_kind?g->erl.event_hub:xp?xp->arg:g->jobs[j].item-1;if(hub<0||hub>=HUB_COUNT)hub=0;
 if(r->phase==RACE_NONE||r->system!=g->system||r->hub!=hub||((r->phase==RACE_GRID||r->phase==RACE_LOST)&&!g->erl.event_kind&&fabsf(r->top_speed-race_top_speed(g))>.5f)){
  if(!race_course_build(g,hub,r))return;race_grid_ships(g);
 }
 r->job=j;
}
void race_restart(Game *g){
 race_prepare(g);if(!race_ready(g))return;SpaceRace *r=&g->race;
 r->phase=RACE_GRID;race_prepare(g); /* Retry also refreshes changed engine pips/modules. */
 r->phase=RACE_COUNTDOWN;r->countdown=7;r->elapsed=r->boost_time=r->flash=r->result_age=0;r->gate=0;r->place=1;r->arcade.entry=0;r->arcade.launch_seen=0;
 if(!g->erl.event_kind&&r->job>=0&&g->jobs[r->job].type==MISSION_RACE)g->erl_local.progress.board_until[g->system][r->hub]=(social_now(g)/1200+1)*1200;
 r->reserve=100;r->boost_delay=0;r->boost_lock=0;
 r->off_course=r->repair=r->handover=0;r->health=100;r->protection=g->erl.event_kind?2:0;
 if(g->erl.event_kind){g->erl.resume=1;g->erl.dirty=1;}
 r->crash_time=r->crash_cooldown=0;r->crash_id=-1;r->takedowns=r->crash_manual_boost=r->crash_awards=0;
 for(int k=0;k<RACE_GATES;k++)r->gates[k].boost_used=0;race_grid_ships(g);
 g->pos=add(r->start,mul(r->dir,-1400));g->yaw=atan2f(r->dir.x,r->dir.z);g->pitch=asinf(r->dir.y);g->roll=0;
 if(r->arcade.rules.objective==RA_HUNTER)r->pilot[0].pos=add(r->start,mul(r->dir,4500));
 if(r->arcade.rules.objective==RA_ESCAPE)g->pos=add(r->start,mul(r->dir,4500));
 RaceTrackFrame sf=race_track_frame(r,0,0);g->pos=add(g->pos,mul(sf.up,RACE_TRACK_FLOOR+race_floor_support(g,0,sf)+70));
 if(r->layout==5){
  /* The authored road begins at section zero. Place the grid on that actual
   * surface, rather than suspending its ships behind the first floor panel. */
  static unsigned grid_rotation;int slot=(random_u(g)+grid_rotation++)%(r->rivals+1);r->place=slot+1;
  sf=race_track_frame(r,0,.20f-(slot/3)*.045f);g->yaw=atan2f(sf.ahead.x,sf.ahead.z);g->pitch=asinf(sf.ahead.y);
  {Vec3 br={cosf(g->yaw),0,-sinf(g->yaw)},bu=norm(race_cross(sf.ahead,br));g->roll=atan2f(-dot(sf.up,br),dot(sf.up,bu));}
  g->pos=add(sf.centre,add(mul(sf.right,(slot%3-1)*650),mul(sf.up,RACE_TRACK_FLOOR+race_floor_support(g,0,sf)+35)));
  for(int i=0;i<r->rivals;i++){int grid=i>=slot?i+1:i;RaceTrackFrame f=race_track_frame(r,0,.20f-(grid/3)*.045f);r->pilot[i].dir=f.ahead;r->pilot[i].pos=add(f.centre,add(mul(f.right,(grid%3-1)*650),mul(f.up,RACE_TRACK_FLOOR+race_floor_support(g,i+1,f)+35)));}
 }
 if(r->layout!=5)for(int i=0;i<r->rivals;i++)r->pilot[i].pos=add(r->pilot[i].pos,mul(sf.up,RACE_TRACK_FLOOR+race_floor_support(g,i+1,sf)+70));
 g->speed=0;g->boost=0;g->shot=0;g->cue=SFX_ALERT;
}
Vec3 race_waypoint(const Game *g){const SpaceRace *r=&g->race;return r->phase==RACE_RUNNING?r->gates[r->gate<RACE_GATES?r->gate:RACE_GATES-1].pos:r->start;}
/* READY and simulation must agree before creating a resume checkpoint. */
const char *race_start_blocker(const Game *g){
 if(g->docked)return "Launch, then contact the start beacon.";
 if(g->dead)return "Recover your ship before entering the race.";
 if(g->dock_stage)return "Finish or cancel docking guidance before READY.";
 if(g->planet>=0)return "Leave the surface before entering the race.";
 if(g->jump>0)return "Complete your jump before entering the race.";
 if(g->police_stop)return "Resolve the police stop before entering the race.";
 return 0;
}
int race_preflight(Game *g,float dt){
 race_prepare(g);SpaceRace *r=&g->race;
 if(!race_ready(g))return 0;
 if(race_start_blocker(g)){if(race_is_active(g)||r->phase==RACE_LOST){r->phase=RACE_GRID;r->boost_time=r->crash_time=0;race_grid_ships(g);}return 0;}
 /* Start requires Triangle at the beacon and an explicit READY action. */
 if(r->phase==RACE_LOST||r->phase==RACE_WON)return 1;
 if(r->phase!=RACE_COUNTDOWN)return 0;
 int before=(int)ceilf(r->countdown);r->countdown-=dt;g->speed=0;g->boost=0;
 if((int)ceilf(r->countdown)!=before&&r->countdown<3)g->cue=SFX_UI;
 if(r->countdown<=0){r->countdown=0;r->phase=RACE_RUNNING;r->arcade.entry=0;if(r->arcade.launch_seen)g->speed=r->top_speed*.35f;g->cue=SFX_SPEED_SURGE;}
 return 1;
}
static void race_crash_award_flush(Game *g){SpaceRace *r=&g->race;if(r->layout==5&&r->crash_awards){r->reserve=fminf(100,r->reserve+r->crash_awards*25.f);r->crash_awards=0;r->flash=.45f;}}
void race_crash_present_tick(Game *g,float dt){SpaceRace *r=&g->race;if(r->layout!=5)return;float before=r->crash_time;r->crash_time=fmaxf(0,r->crash_time-dt);if(before>2.2f&&r->crash_time<=2.2f)g->cue=SFX_DEATH;if(before>0&&r->crash_time==0)r->arcade.actor[0].require_release=1;if(r->crash_awards){r->crash_cooldown=fmaxf(0,r->crash_cooldown-dt);if(!r->crash_cooldown)race_crash_award_flush(g);}}
void race_flight_control(Game *g,float *dt,float *turn,float *pitch,int *throttle){
 SpaceRace *r=&g->race;if(!race_is_active(g)||r->phase!=RACE_RUNNING)return;
 if(r->layout==5&&(r->crash_time>0||r->repair>0)){
  /* The live field keeps its normal clock. Only the presentation slows history. */
  race_crash_present_tick(g,*dt);r->handover=0;
  if(r->repair>0){r->repair=fmaxf(0,r->repair-*dt);*throttle=0;g->speed=g->boost=0;*turn=*pitch=0;
   if(!r->repair){if(r->arcade.actor[0].health<=0){r->health=100;r->reserve=25;ra_respawn(&r->arcade.actor[0]);}r->protection=1;r->arcade.actor[0].protection=1;g->speed=r->top_speed*.25f;}return;}
  if(!r->manual_input)r->crash_manual_boost=0;
  g->boost=r->crash_manual_boost&&r->manual_input;erl_drive_reserve(&r->reserve,r->manual_input,0,*dt);
  RaceTrackFrame f=race_track_nearest(r,r->gate,g->pos);int surface=race_track_surface(r,f.section,f.t);*throttle=1;
  if(surface==2){*turn=*pitch=0;return;}
  float lane=dot(sub(g->pos,f.centre),f.right),edge=(surface==1?RACE_NEON_EDGE:190)*2.5f*race_neon_scale(r);
  lane=fmaxf(-edge*.60f,fminf(edge*.60f,lane));float t=f.t+.08f;int k=f.section;if(t>1){k++;t-=1;}RaceTrackFrame next=race_track_frame(r,k,t);
  Vec3 aim=add(next.centre,add(mul(next.right,lane),mul(next.up,RACE_TRACK_FLOOR+race_floor_support(g,0,next)+35))),desired=norm(sub(aim,g->pos));
  Vec3 right=norm(race_cross(f.up,forward(g)));*turn=fmaxf(-1,fminf(1,dot(desired,right)*6));*pitch=0;return;
 }
 if(r->layout==4){RaceTrackFrame frame=race_track_nearest(r,r->gate,g->pos);float height=dot(sub(g->pos,frame.centre),frame.up);int per=RACE_GATES/r->laps,launch=(r->gate/per)*per+(int)(per*.421875f);if(race_track_surface(r,frame.section,frame.t)==1||((r->arcade.branch_boost[0]&(1u<<launch))&&r->gate<=launch+2&&height>RACE_TRACK_FLOOR+race_floor_support(g,0,frame)+125))*pitch=0;}
 if(r->layout==5){RaceTrackFrame frame=race_track_nearest(r,r->gate,g->pos);if(race_track_surface(r,frame.section,frame.t)==1)*pitch=0;}
 /* Actual simulation slows for every racer, not just the exploding victim.
  * Wall-clock shot timing stays independent, with one full second of safe
  * guidance after the return swing. No teleporting or skipped checkpoints. */
 if(r->crash_time>0||r->handover>0||r->repair>0){
  int held_throttle=*throttle;float age=5.8f-r->crash_time;r->crash_time=fmaxf(0,r->crash_time-*dt);r->handover=fmaxf(0,r->handover-*dt);
  if(r->repair>0){r->repair=fmaxf(0,r->repair-*dt);if(!r->repair)r->health=100;*throttle=0;g->speed=0;*turn=*pitch=0;return;}
  if(age<1.2f)*dt*=.22f;else if(age<1.8f)*dt*=.22f+.78f*(age-1.2f)/.6f;
  RaceGate *q=&r->gates[r->gate];Vec3 aim=add(q->pos,mul(q->normal,300));if(r->layout==5){RaceTrackFrame f=race_track_nearest(r,r->gate,g->pos);if(race_track_surface(r,f.section,f.t)==2){*turn=*pitch=0;*throttle=1;return;}float t=f.t+.08f;int section=f.section;if(t>1){section++;t-=1;}f=race_track_frame(r,section,t);aim=add(f.centre,mul(f.up,RACE_TRACK_FLOOR+race_floor_support(g,0,f)+35));}Vec3 desired=norm(sub(aim,g->pos));
  float sy=sinf(g->yaw),cy=cosf(g->yaw),sp=sinf(g->pitch),cp=cosf(g->pitch),sr=0,cr=1;
  Vec3 right=add(mul((Vec3){cy,0,-sy},cr),mul((Vec3){-sy*sp,cp,-cy*sp},sr));
  Vec3 up=add(mul((Vec3){cy,0,-sy},-sr),mul((Vec3){-sy*sp,cp,-cy*sp},cr));
  *turn=fmaxf(-1,fminf(1,dot(desired,right)*6));*pitch=fmaxf(-1,fminf(1,dot(desired,up)*6));*throttle=1;
  r->boost_time=0;g->boost=r->crash_time<=0&&r->crash_manual_boost&&held_throttle>0;
 }
 if(r->layout==5&&r->crash_time<=0&&r->repair<=0){
  int held=r->manual_input||g->boost||r->boost_lock,spending=g->boost&&*throttle>0&&!r->boost_lock&&r->reserve>0;
  erl_drive_reserve(&r->reserve,held,spending,*dt);r->boost_delay=0;
  if(spending&&r->reserve<=0)r->boost_lock=1;
  g->boost=spending&&!r->boost_lock;
  if(*throttle>=0&&(r->boost_time>0||r->arcade.actor[0].rocket>0))*throttle=1;
  return;
 }
 if(r->arcade.ready){RaceItemActor *a=&r->arcade.actor[0];if(*throttle<0)a->rocket=0;if(a->spin>0){g->yaw+=sinf(a->spin*16)*(*dt)*3;*throttle=0;}if(a->rocket>0&&*throttle>=0){g->boost=0;*throttle=1;return;}}
 /* Empty means release R before another burst. Keeping R held cannot silently
  * recharge and restart the drive. Autopilot safety never spends the reserve. */
 if(r->crash_time<=0){
  if(r->boost_time>0&&*throttle>=0){g->boost=1;*throttle=1;return;}
  if(*throttle<=0)r->boost_lock=0;
  if(r->boost_lock||r->reserve<=0||*throttle<=0)g->boost=0;
  int spending=g->boost;
  race_reserve_tick(&r->reserve,&r->boost_delay,g->boost,*dt);
  if(spending&&r->reserve<=0){r->boost_lock=1;g->boost=0;}
 }
}
#include "erl-course-physics.h"
static void race_burst(Game *g,int id,int cinematic){
 SpaceRace *r=&g->race;RaceRival *p=&r->pilot[id];if(p->disabled)return;
 p->impact=p->pos;Vec3 impact;
 if(race_scenery_hit(r,p->gate,p->pos,add(p->pos,mul(p->dir,1800)),p->radius,0,&impact))p->impact=impact;
 p->disabled=1;p->knockouts++;p->boosting=0;p->burst=.001f;p->finished=-1;g->cue=SFX_DEATH;
 if(cinematic){r->takedowns++;r->last_score=r->elapsed;r->feed_code=2;r->feed_id=id;r->feed_age=3;r->flash=.7f;if(r->crash_cooldown<=0&&r->crash_time<=0){r->crash_id=id;r->crash_time=5.8f;r->crash_cooldown=7;r->crash_manual_boost=g->boost;}}
 if(cinematic&&r->layout==5){if(r->crash_time==5.8f){r->crash_time=3;r->crash_cooldown=.8f;r->crash_pos=p->impact;r->crash_dir=p->dir;r->protection=fmaxf(r->protection,3);g->cue=SFX_HIT;}r->crash_awards++;if(r->crash_cooldown<=0)r->crash_cooldown=.001f;}
}
#include "erl-arcade-combat.h"
#include "erl-holo-track.h"
#include "erl-arcade-course.h"
/* Position constraints remain active during impact cooldown. Compute room
 * inside the actual cylindrical corridor/floor before sharing penetration;
 * a pinned craft yields space through its neighbour, never through the wall. */
static float __attribute__((noinline,optimize("O2"))) race_neon_contact_room(const Game *g,int id,Vec3 direction){
 const SpaceRace *r=&g->race;Vec3 pos=ra_position(g,id);int gate=id?r->pilot[id-1].gate:r->gate;
 RaceTrackFrame f=race_track_nearest(r,gate,pos);int kind=race_track_surface(r,f.section,f.t);if(kind==2)return 100000;
 float body=id?r->pilot[id-1].radius+8:race_neon_body_radius(g)+8,edge=(kind==1?RACE_NEON_EDGE:190)*2.5f*race_neon_scale(r)-body;
 Vec3 d=sub(pos,f.centre);float x=dot(d,f.right),y=fmaxf(0,dot(d,f.up)-RACE_TRACK_FLOOR),sx=dot(direction,f.right),sy=dot(direction,f.up),a=sx*sx+sy*sy,b=x*sx+y*sy,c=x*x+y*y-edge*edge;
 float room=a>.00001f?fmaxf(0,(-b+sqrtf(fmaxf(0,b*b-a*c)))/a):100000;
 if(sy<-.0001f){float floor=RACE_TRACK_FLOOR+race_floor_support(g,id,f)+35;room=fminf(room,fmaxf(0,(dot(d,f.up)-floor)/-sy));}return room;
}
static void __attribute__((noinline,optimize("O2"))) race_neon_separate(Game *g,int a,int b){
 SpaceRace *r=&g->race;Vec3 *pa=a?&r->pilot[a-1].pos:&g->pos,*pb=b?&r->pilot[b-1].pos:&g->pos;
 float touch=(a?r->pilot[a-1].radius:race_neon_body_radius(g))+(b?r->pilot[b-1].radius:race_neon_body_radius(g));
 Vec3 delta=sub(*pa,*pb);float distance=length(delta);if(distance>=touch)return;
 RaceTrackFrame f=race_track_nearest(r,a?r->pilot[a-1].gate:r->gate,*pa);Vec3 side=distance>1?mul(delta,1/distance):f.right;
 float need=touch-distance+2,room_a=race_neon_contact_room(g,a,side),room_b=race_neon_contact_room(g,b,mul(side,-1));
 if(room_a+room_b<need){side=mul(f.ahead,dot(delta,f.ahead)<0?-1:1);need=touch+2;room_a=race_neon_contact_room(g,a,side);room_b=race_neon_contact_room(g,b,mul(side,-1));}
 float move_a=fminf(need*.5f,room_a),move_b=fminf(need-move_a,room_b);move_a=fminf(room_a,need-move_b);
 *pa=add(*pa,mul(side,move_a));*pb=sub(*pb,mul(side,move_b));
}
static float race_progress(const SpaceRace *r,int gate,Vec3 pos){
 if(gate>=RACE_GATES)return (float)RACE_GATES+1;
 if(r->layout==5){RaceTrackFrame f=race_track_nearest(r,gate,pos);return f.section+f.t;}
 Vec3 a=gate?r->gates[gate-1].pos:r->start,b=r->gates[gate].pos,axis=sub(b,a);
 return gate+fmaxf(0,fminf(.999f,dot(sub(pos,a),axis)/fmaxf(1,dot(axis,axis))));
}
float race_gold_delta(const Game *g){
 const SpaceRace *r=&g->race;if(r->layout==5)return r->elapsed-r->arcade.rules.gold*fminf(1,race_progress(r,r->gate,g->pos)/RACE_GATES);float covered=0,total=0;Vec3 start=r->start;
 for(int k=0;k<RACE_GATES;k++){Vec3 axis=sub(r->gates[k].pos,start);float span=length(axis);total+=span;if(k<r->gate)covered+=span;else if(k==r->gate)covered+=span*fmaxf(0,fminf(1,dot(sub(g->pos,start),axis)/fmaxf(1,dot(axis,axis))));start=r->gates[k].pos;}
 return r->elapsed-r->arcade.rules.gold*fminf(1,covered/fmaxf(1,total));
}
#include "erl-neon-traffic.h"
#include "erl-arcade-objectives.h"
void race_withdraw(Game *g){if(race_is_active(g))ra_complete(g,g->race.rivals+1,1);}
void race_contract_resume(Game *g){
 if(g->erl.event_kind||!race_is_active(g))return;
 ErlCareer *e=&g->erl;g->pos=e->resume_pos;g->yaw=e->resume_yaw;g->pitch=e->resume_pitch;g->roll=e->resume_roll;g->speed=e->resume_speed;g->rng=e->resume_rng;
 g->docked=0;g->dock_stage=0;g->planet=-1;g->surface=0;e->paused=1;e->resume=0;
}
void race_tick(Game *g,Vec3 previous,float dt){
 SpaceRace *r=&g->race;if(r->phase==RACE_WON)return;
 if(!race_ready(g)||r->phase!=RACE_RUNNING)return;
 int previous_place=r->place;float frame_start=r->elapsed;r->elapsed+=dt;r->boost_time=fmaxf(0,r->boost_time-dt);r->flash=fmaxf(0,r->flash-dt);if(r->layout!=5)r->crash_cooldown=fmaxf(0,r->crash_cooldown-dt);
 r->protection=fmaxf(0,r->protection-dt);r->feed_age=fmaxf(0,r->feed_age-dt);
 r->off_course=0; /* Energy walls replace the old off-course countdown. */
 unsigned rubbing=0;race_neon_traffic_prepare(g);
 /* Rivals steer their actual ships toward apertures. No clock-based checkpoints. */
 for(int i=0;i<r->rivals;i++){
  RaceRival *p=&r->pilot[i];if(r->arcade.actor[i+1].eliminated)continue;p->bump=fmaxf(0,p->bump-dt);p->protection=fmaxf(0,p->protection-dt);
  if(p->disabled){p->burst+=dt;if(p->burst<.35f)p->pos=add(p->pos,mul(sub(p->impact,p->pos),fminf(1,dt/fmaxf(.01f,.35f-p->burst))));
   if(p->burst>=3){RaceGate *q=&r->gates[p->gate<RACE_GATES?p->gate:RACE_GATES-1];Vec3 last=p->gate?r->gates[p->gate-1].pos:r->start;p->pos=add(last,mul(q->normal,300));p->dir=q->normal;if(r->layout==5){RaceTrackFrame f=race_track_frame(r,p->gate,0);p->dir=f.ahead;p->pos=add(f.centre,add(mul(f.up,RACE_TRACK_FLOOR+race_floor_support(g,i+1,f)+35),mul(f.ahead,150)));}p->disabled=0;p->speed=r->top_speed*.3f;p->health=100;p->protection=3;p->bump=2;if(r->arcade.rules.objective==RA_TAKEDOWNS){p->gate=r->gate;p->pos=add(g->pos,add(mul(forward(g),4500+i*600),mul(r->right,((i%3)-1)*900.f)));p->dir=forward(g);}r->feed_code=3;r->feed_id=i;r->feed_age=2;}
   continue;}
  if(r->arcade.actor[i+1].eliminated)continue;
  if(r->arcade.rules.objective==RA_TAKEDOWNS&&length(sub(p->pos,g->pos))>r->top_speed*6){p->gate=r->gate;p->pos=add(g->pos,add(mul(forward(g),5500+i*700),mul(r->right,((i%3)-1)*900.f)));p->dir=forward(g);}
  if(p->gate>=RACE_GATES)continue;RaceGate *q=&r->gates[p->gate];
  Vec3 lane=add(mul(q->right,((i%5)-2)*230.f),mul(q->up,((i/5)*2-1)*140.f));
  RaceTrackFrame endpoint=race_track_frame(r,p->gate,1);Vec3 aim=add(add(q->pos,lane),add(mul(q->normal,250),mul(endpoint.up,RACE_TRACK_FLOOR+race_floor_support(g,i+1,endpoint)+70)));int seeking_repair=0;
  if(r->layout!=4&&p->gate%4==2&&r->arcade.actor[i+1].health<RA_HEALTH*.55f&&!(r->arcade.branch_repair[i+1]&(1u<<p->gate))&&!(r->arcade.branch_boost[i+1]&(1u<<p->gate))){RaceTrackFrame service=race_repair_frame(r,p->gate);if(dot(sub(p->pos,service.centre),service.ahead)<RA_REPAIR_HALF_LENGTH){aim=add(service.centre,mul(service.ahead,RA_REPAIR_HALF_LENGTH+150));seeking_repair=1;}}
  if(!seeking_repair&&p->reserve<80&&!((r->arcade.branch_boost[i+1]|r->arcade.branch_repair[i+1])&(1u<<p->gate))&&dot(sub(p->pos,race_pad_point(r,p->gate,.5f,0)),q->normal)<0)
   aim=add(race_pad_point(r,p->gate,.5f,0),add(mul(q->up,race_floor_support(g,i+1,race_track_frame(r,p->gate,.5f))+45),mul(lane,.4f)));
  if(g->erl.event_kind&&dot(sub(p->pos,q->pos),q->normal)>q->radius)aim=add(q->pos,mul(q->normal,-3500));
  /* Some rivals race clean; others try a nearby shoulder check. They still
   * have to fly the same physical course. A bump costs speed, not hull. */
  if(!seeking_repair&&i%3==1&&p->gate==r->gate&&length(sub(p->pos,g->pos))<1800&&dot(p->dir,forward(g))>.7f&&p->bump<=0)aim=add(g->pos,mul(forward(g),550));
  if(!seeking_repair&&r->arcade.rules.objective!=RA_TAKEDOWNS&&r->arcade.rules.mode==RA_COMBAT&&p->bump<=0){
   float nearest=r->repair?1e9f:length(sub(g->pos,p->pos));Vec3 prey=g->pos,velocity=forward(g);
   for(int j=0;j<r->rivals;j++)if(i!=j&&!r->pilot[j].disabled&&r->pilot[j].protection<=0){float d=length(sub(p->pos,r->pilot[j].pos));if(d<nearest){nearest=d;prey=r->pilot[j].pos;velocity=r->pilot[j].dir;}}
   if(nearest<1800&&dot(p->dir,norm(sub(prey,p->pos)))>.8f)aim=add(prey,mul(velocity,250));
  }
  int neon_airborne=0;
  float traffic_limit=r->top_speed*ERL_MANUAL_SURGE;
  if(r->layout==5){RaceTrackFrame f=race_track_nearest(r,p->gate,p->pos);neon_airborne=race_track_surface(r,f.section,f.t)==2;aim=race_neon_traffic_aim(g,i+1,f,dt,&traffic_limit);}
  Vec3 desired=neon_airborne?p->dir:norm(sub(aim,p->pos));float alignment=dot(p->dir,desired);
  float remaining=dot(sub(q->pos,p->pos),q->normal),turn_exit=p->gate+1<RACE_GATES?dot(q->normal,r->gates[p->gate+1].normal):1;
  for(int ahead=1;g->erl.event_kind&&ahead<=3&&p->gate+ahead<RACE_GATES;ahead++)turn_exit=fminf(turn_exit,dot(q->normal,r->gates[p->gate+ahead].normal)+ahead*.06f);
  int straight=alignment>.985f&&(remaining>r->top_speed*1.25f||turn_exit>.93f);
  if(p->boosting&&(!straight||p->reserve<=0))p->boosting=0;
  else if(!p->boosting&&straight&&p->reserve>=48)p->boosting=1;
  if(r->layout==5){if(traffic_limit<p->speed-1)p->boosting=0;erl_drive_reserve(&p->reserve,p->boosting,p->boosting,dt);p->boost_delay=0;}
  else race_reserve_tick(&p->reserve,&p->boost_delay,p->boosting,dt);
  float handling=1.6f;
  float angle=acosf(fmaxf(-1,fminf(1,alignment))),step=fminf(angle,dt*handling);
  Vec3 axis=race_cross(p->dir,desired);
  if(length(axis)>.00001f)p->dir=norm(flight_rotate(p->dir,norm(axis),cosf(step),sinf(step)));
  float target=r->arcade.actor[i+1].contact>.35f?r->top_speed*1.33f:p->boosting?r->top_speed:p->pace;
  /* Mild bounded catch-up keeps the pack in play without overriding a clean
   * line or manufacturing position swaps. One checkpoint is about 2.5%. */
  if(r->layout!=5&&r->arcade.rules.objective!=RA_TAKEDOWNS){int gap=r->gate-p->gate;
   if(gap>0)target*=1.f+fminf(.075f,gap*.025f);
   else if(gap<0)target*=1.f-fminf(.03f,-gap*.012f);
  }
  if(r->layout==5){
   float gap=(race_progress(r,r->gate,g->pos)-race_progress(r,p->gate,p->pos))*r->length/RACE_GATES/fmaxf(1,r->top_speed*ERL_CRUISE);
   float pace=fminf(ERL_CRUISE,p->pace/r->top_speed*(1+erl_trailing_gain(gap)));
   target=erl_drive_target(r->top_speed,pace,p->boosting,r->arcade.actor[i+1].respawn>0||r->arcade.actor[i+1].rocket>0,0);
   target=fminf(target,traffic_limit);
  }
  if(alignment<.94f||(remaining<r->top_speed*.65f&&turn_exit<.8f))target*=.78f;
  p->speed+=fmaxf(-r->top_speed*1.6f*dt,fminf(target-p->speed,r->top_speed*.8f*1.f*dt));Vec3 old=p->pos;p->pos=add(old,mul(p->dir,p->speed*dt));
  ra_boundary(g,i+1,old,dt);ra_service_crossing(g,i+1,old,dt);
  float t=race_crossing(old,p->pos,q->pos,q->normal,ra_section_radius(r,q));
  if(t>=0&&r->layout==5){RaceTrackFrame at=race_track_nearest(r,p->gate,old);if(at.section<p->gate||at.t<.80f)t=-1;}
  if(t>=0){p->gate++;int finish=r->arcade.rules.objective==RA_ELIMINATOR&&r->arcade.rules.required?r->arcade.rules.required:RACE_GATES;if(p->gate>=finish){if(r->arcade.rules.objective==RA_TAKEDOWNS){p->gate=0;p->boost_gate=-1;r->arcade.branch_boost[i+1]=r->arcade.branch_repair[i+1]=0;}else p->finished=frame_start+t*dt;}}
  Vec3 relative_from=sub(previous,old),relative_to=sub(g->pos,p->pos);
  /* One real swept overtake, outside contact radius: sound only, no item,
   * boost, points or damage. Crossing direction prevents per-frame repeats. */
  float miss=race_segment_distance(relative_from,relative_to,(Vec3){0,0,0}),touch=p->radius+(r->layout==5?race_neon_body_radius(g):65);
  if(r->layout==5&&miss<touch+35)rubbing|=1u|(1u<<(i+1));
  if(!g->cue&&p->gate<RACE_GATES&&p->bump<=0&&p->protection<=0&&r->protection<=0&&r->repair<=0&&r->crash_time<=0&&r->handover<=0&&miss>touch&&miss<touch+195&&dot(relative_from,forward(g))<0&&dot(relative_to,forward(g))>=0&&length(sub(relative_to,relative_from))>20)g->cue=SFX_SPEED_SURGE;
  if(p->gate<RACE_GATES&&p->bump<=0&&p->protection<=0&&r->protection<=0&&r->repair<=0&&r->crash_time<=0&&r->handover<=0&&miss<touch){
   float impact=length(sub(mul(forward(g),g->speed),mul(p->dir,p->speed)));
   int heavy=impact>r->top_speed*.18f;
   if(r->arcade.rules.mode==RA_COMBAT&&heavy&&r->layout!=5){
    int player_attack=dot(forward(g),norm(sub(p->pos,g->pos)))>.25f;
    int damage=impact>r->top_speed*.65f||g->boost||r->arcade.actor[0].rocket>0?RA_HEALTH:RA_MISSILE_DAMAGE;
    ra_hit(g,i+1,player_attack?damage:600,1,0);ra_hit(g,0,player_attack?600:damage,1,i+1);
   }
   if(r->layout==5&&r->arcade.rules.mode==RA_COMBAT&&impact>r->top_speed*.45f&&dot(forward(g),p->dir)>.3f){
    int attack=dot(forward(g),norm(sub(p->pos,previous)))>.5f;
    if(attack&&(g->boost||r->boost_time>0||r->arcade.actor[0].rocket>0))ra_hit(g,i+1,RA_HEALTH,1,0);
    else if(!attack&&(p->boosting||r->arcade.actor[i+1].respawn>0||r->arcade.actor[i+1].rocket>0))ra_hit(g,0,RA_HEALTH,1,i+1);
   }
   Vec3 side=norm(sub(p->pos,g->pos));if(length(side)<.1f)side=q->right;
   if(r->layout!=5){p->pos=add(p->pos,mul(side,240));g->pos=add(g->pos,mul(side,-90));}p->speed*=.72f;g->speed*=.86f;p->bump=.9f;r->flash=.2f;
  }
  if(r->layout==5&&!p->disabled&&p->gate<RACE_GATES&&r->protection<=0&&r->repair<=0&&r->crash_time<=0)race_neon_separate(g,0,i+1);
 }
 race_arcade_tick(g,previous,dt);
 /* Rival-to-rival contact is also physical. Keep the pair budget fixed. */
 for(int i=0;i<r->rivals;i++)for(int j=i+1;j<r->rivals;j++){
  RaceRival *a=&r->pilot[i],*b=&r->pilot[j];if(r->arcade.actor[i+1].eliminated||r->arcade.actor[j+1].eliminated||a->disabled||b->disabled||a->gate>=RACE_GATES||b->gate>=RACE_GATES)continue;
  Vec3 delta=sub(a->pos,b->pos);float d=length(delta),touch=a->radius+b->radius;if(r->layout==5&&d<touch+35)rubbing|=(1u<<(i+1))|(1u<<(j+1));if(d>=touch)continue;if(r->layout==5)race_neon_separate(g,i+1,j+1);if(a->bump>0||b->bump>0)continue;
  Vec3 side=d>1?mul(delta,1/d):r->right;float separation=(touch-d)*.5f+2;if(r->layout!=5){a->pos=add(a->pos,mul(side,separation));b->pos=add(b->pos,mul(side,-separation));}float impact=length(sub(mul(a->dir,a->speed),mul(b->dir,b->speed)));
  if(r->layout!=5&&r->arcade.rules.mode==RA_COMBAT&&impact>r->top_speed*.2f){int winner=a->speed>b->speed?i:j,loser=winner==i?j:i;int damage=impact>r->top_speed*.65f?RA_HEALTH:RA_MISSILE_DAMAGE;ra_hit(g,loser+1,damage,1,winner+1);ra_hit(g,winner+1,600,1,loser+1);}
  a->speed*=.86f;b->speed*=.86f;a->bump=b->bump=.6f;
 }
 if(r->layout==5)for(int id=0;id<=r->rivals;id++)if(ra_eligible(g,id))ra_neon_grind(g,id,1,!!(rubbing&(1u<<id)),dt);
 if(r->protection<=0&&r->repair<=0&&r->crash_time<=0&&r->handover<=0){
  if(race_scenery_hit(r,r->gate,previous,g->pos,85,dt,0))ra_hit(g,0,g->speed>r->top_speed*.7f?RA_HEALTH:600,0,-1);
  else if(!g->cue&&r->arcade.cue_age<=0&&race_scenery_near(r,r->gate,previous,g->pos)){r->arcade.last_hit=5;r->arcade.cue_age=.25f;g->cue=SFX_SPEED_SURGE;}
 }
 for(int i=0;i<r->rivals;i++){RaceRival *p=&r->pilot[i];if(!p->disabled&&p->protection<=0&&p->gate<RACE_GATES&&race_scenery_hit(r,p->gate,sub(p->pos,mul(p->dir,p->speed*dt)),p->pos,p->radius,dt,0))ra_hit(g,i+1,p->speed>r->top_speed*.7f?RA_HEALTH:600,0,-1);}
 RaceGate *q=&r->gates[r->gate];
 if(r->repair>0)return;
 ra_boundary(g,0,previous,dt);ra_service_crossing(g,0,previous,dt);
 float t=race_crossing(previous,g->pos,q->pos,q->normal,ra_section_radius(r,q));
 if(t>=0&&r->layout==5){RaceTrackFrame at=race_track_nearest(r,r->gate,previous);if(at.section<r->gate||at.t<.80f)t=-1;}
 if(t>=0){r->gate++;r->flash=.45f;r->arcade.last_hit=4;r->arcade.cue_age=.45f;g->cue=SFX_SCAN;
  if(r->gate%(RACE_GATES/r->laps)==0&&r->gate<RACE_GATES){r->feed_code=5;r->feed_age=3;}
  if(r->gate==RACE_GATES&&r->arcade.rules.objective==RA_TAKEDOWNS){r->gate=0;r->arcade.branch_boost[0]=r->arcade.branch_repair[0]=0;for(int k=0;k<RACE_GATES;k++)r->gates[k].boost_used=0;}
 
  int finish=r->arcade.rules.objective==RA_ELIMINATOR&&r->arcade.rules.required?r->arcade.rules.required:RACE_GATES;
  if(r->gate>=finish){
   float finished=frame_start+t*dt;r->place=1;for(int i=0;i<r->rivals;i++)if(!r->arcade.actor[i+1].eliminated&&r->pilot[i].finished>=0&&r->pilot[i].finished<=finished)r->place++;
   r->elapsed=finished;ra_complete(g,r->place,0);
   return;
  }
 }
 r->place=1;float own=race_progress(r,r->gate,g->pos);for(int i=0;i<r->rivals;i++)if(!r->arcade.actor[i+1].eliminated&&race_progress(r,r->pilot[i].gate,r->pilot[i].pos)>own)r->place++;
 if(r->place<previous_place&&r->feed_age<=0){r->feed_code=4;r->feed_id=r->place;r->feed_age=2;}
 if(r->elapsed>=r->limit-30&&frame_start<r->limit-30&&r->discipline>=ERL_RUSH){r->feed_code=6;r->feed_age=2;}
 ra_objectives_tick(g,dt);
}
