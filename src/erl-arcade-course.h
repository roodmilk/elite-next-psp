/* One bounded player contact flash. Item/manual boost never creates it. */
static RaceBoostVisual ra_boost_visual;static const Game *ra_boost_owner;
const RaceBoostVisual *race_boost_visual(const Game *g){
 return ra_boost_owner==g&&g->race.layout==5&&ra_boost_visual.serial==g->erl.serial&&ra_boost_visual.seed==g->race.seed&&ra_boost_visual.until>g->race.elapsed&&ra_boost_visual.until<=g->race.elapsed+.31f?&ra_boost_visual:0;
}
static void ra_boost_flash(const Game *g,Vec3 pos,RaceTrackFrame f,int ring){
 ra_boost_owner=g;ra_boost_visual=(RaceBoostVisual){pos,f.right,f.up,g->race.elapsed+.30f,g->erl.serial,g->race.seed,ring};
}
/* Corridor and branch rules share authored sections with renderer. */
static float ra_section_radius(const SpaceRace *r,const RaceGate *q){return r->layout==5?q->radius:11000.f;}
/* Invert the generated arc-length lookup to find an authored clock station.
 * The lip and landing therefore move with editable geometry, not lap time. */
static RaceTrackFrame ra_neon_clock_frame(const SpaceRace *r,int gate,float clock){
 int n=0;while(n<127&&neon_arc_time[n+1]<clock)n++;
 float fraction=(clock-neon_arc_time[n])/(neon_arc_time[n+1]-neon_arc_time[n]);
 int per=RACE_GATES/r->laps;float q=(n+fraction)*(per/128.f);int section=(int)q;
 return race_track_frame(r,(gate/per)*per+section,q-section);
}
/* Structural wear bypasses the optional item shield, while preserving spawn
 * and cinematic protection. Brief brushes are forgiven, tiny gaps do not farm
 * fresh grace, and fractional damage is independent of frame rate. */
static int ra_neon_grind(Game *g,int id,int channel,int touching,float dt){
 static const Game *owner;static unsigned serial,seed;static float age,wear[RA_ACTORS][2],clear[RA_ACTORS][2],fraction[RA_ACTORS][2];
 SpaceRace *r=&g->race;RaceItemActor *a=&r->arcade.actor[id];
 if(owner!=g||serial!=g->erl.serial||seed!=r->seed||r->elapsed<age){owner=g;serial=g->erl.serial;seed=r->seed;memset(wear,0,sizeof(wear));memset(clear,0,sizeof(clear));memset(fraction,0,sizeof(fraction));}age=r->elapsed;
 if(!touching){clear[id][channel]+=dt;if(clear[id][channel]>=.3f)wear[id][channel]=fraction[id][channel]=0;return 0;}
 clear[id][channel]=0;float before=wear[id][channel];wear[id][channel]+=dt;
 float protection=id?r->pilot[id-1].protection:r->protection;
 if(a->health<=0||protection>0||a->protection>0)return 0;
 float harmful=fmaxf(0,wear[id][channel]-.25f)-fmaxf(0,before-.25f);
 fraction[id][channel]+=harmful*RA_HEALTH*(channel?.03f:.06f);int damage=(int)fraction[id][channel];fraction[id][channel]-=damage;
 if(!damage)return 0;a->health=a->health>damage?a->health-damage:0;ra_health_sync(g,id);
 if(!a->health){if(id)race_burst(g,id-1,0);else race_player_repair(g);return 1;}return 0;
}
static void ra_boundary(Game *g,int id,Vec3 from,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;if(!ra_eligible(g,id))return;
 int gate=id?r->pilot[id-1].gate:r->gate;if(gate>=RACE_GATES)return;
 Vec3 *pos=id?&r->pilot[id-1].pos:&g->pos;float *speed=id?&r->pilot[id-1].speed:&g->speed;
 RaceTrackFrame frame=race_track_nearest(r,gate,*pos);Vec3 centre=frame.centre,right=frame.right,up=frame.up;
 float height=dot(sub(*pos,centre),up),support=race_floor_support(g,id,frame),clearance=RACE_TRACK_FLOOR+support+35;
 if(r->layout==5){
  /* Derived attachment state only; loading/restarting recaptures safely from
   * the saved physical position, without changing the save payload. */
  static const Game *owner;static unsigned attached,jumping,seed,serial;static float age;
  if(owner!=g||seed!=r->seed||serial!=g->erl.serial||r->elapsed<age){owner=g;seed=r->seed;serial=g->erl.serial;attached=jumping=0;}age=r->elapsed;
  unsigned contact_bit=1u<<id;
  Vec3 direction=id?r->pilot[id-1].dir:forward(g);float lane=dot(sub(*pos,centre),right);int surface=race_track_surface(r,frame.section,frame.t);
  int per=RACE_GATES/r->laps;float clock=neon_time(((frame.section%per)+frame.t)/per);
  float scale=race_neon_scale(r),body=id?r->pilot[id-1].radius+8:race_neon_body_radius(g);
  float edge=(surface==1?RACE_NEON_EDGE:190.f)*2.5f*scale;
  float containment=fmaxf(1,edge-body);
  /* The phase field is one cylindrical tunnel above the road, rather than
   * separate square side and roof planes. Project the whole hull onto the
   * same faceted semicircle the renderer draws. The authored jump stays open. */
  float tunnel=fmaxf(1,edge-body),above=fmaxf(0,height-RACE_TRACK_FLOOR),radial=sqrtf(lane*lane+above*above);
  if(ra_neon_grind(g,id,0,surface!=2&&radial>tunnel-.5f&&*speed>r->top_speed*.12f,dt))return;
  if(surface!=2&&radial>tunnel){float factor=tunnel/radial,newlane=lane*factor,newabove=above*factor;
   *pos=add(*pos,add(mul(right,newlane-lane),mul(up,newabove-above)));lane=newlane;height=RACE_TRACK_FLOOR+newabove;
   Vec3 normal=norm(add(mul(right,newlane),mul(up,newabove)));float outward=dot(direction,normal);if(outward>0)direction=norm(sub(direction,mul(normal,outward)));
   if(!id){a->wall_impact=add(*pos,mul(normal,body));a->wall_ripple=.3f;}
  }
  if(!(jumping&contact_bit)&&surface==2&&clock<51){
   RaceTrackFrame lip=ra_neon_clock_frame(r,gate,50);
   Vec3 lip_floor=add(lip.centre,mul(lip.up,RACE_TRACK_FLOOR+race_floor_support(g,id,lip)+35));
   float beyond=dot(sub(*pos,lip_floor),lip.ahead);
   if(race_crossing(from,*pos,lip_floor,lip.ahead,containment+300)>=0||
      ((attached&contact_bit)&&beyond>=0&&beyond<*speed*dt*1.5f+50)){
    RaceTrackFrame landing=ra_neon_clock_frame(r,gate,53);
    float across=fmaxf(-containment*.9f,fminf(containment*.9f,dot(sub(*pos,lip.centre),lip.right)));
    Vec3 target=add(landing.centre,add(mul(landing.up,RACE_TRACK_FLOOR+race_floor_support(g,id,landing)+80),mul(landing.right,across)));
    Vec3 delta=sub(target,*pos),route=norm(add(mul(lip.ahead,.68f),mul(landing.ahead,.32f)));
    float launch=fminf(r->top_speed*1.08f,fmaxf(r->top_speed*.62f,*speed));
    float span=fmaxf(1,dot(delta,route)),travel=fmaxf(.55f,fminf(3.2f,span/launch));
    float side=fmaxf(-launch*.20f,fminf(launch*.20f,dot(delta,lip.right)/travel));
    float lift=fmaxf(launch*.10f,fminf(launch*.28f,dot(delta,lip.up)/travel+350*travel));
    Vec3 velocity=add(mul(route,launch),add(mul(lip.right,side),mul(lip.up,lift)));
    if(dot(velocity,lip.ahead)<launch*.65f)velocity=add(mul(lip.ahead,launch*.65f),mul(lip.up,lift));
    direction=norm(velocity);*speed=fminf(length(velocity),r->top_speed*1.12f);jumping|=contact_bit;attached&=~contact_bit;
    a->actor[id].contact=.35f;if(!id){r->flash=.35f;g->cue=SFX_SPEED_SURGE;}
   }
  }
  /* Magnet capture starts above the floor, with a small forgiving overlap.
   * Falling well underneath the landing must never pull a hull through it. */
  if((surface!=2&&height<clearance-250)||(surface==2&&height<clearance-1500)||
     ((jumping&contact_bit)&&surface==1&&clock>=53&&height>=clearance+2400)){
   attached&=~contact_bit;jumping&=~contact_bit;
   if(id){RaceRival *p=&r->pilot[id-1];RaceTrackFrame safe=race_track_frame(r,gate,0);p->pos=add(safe.centre,mul(safe.up,RACE_TRACK_FLOOR+race_floor_support(g,id,safe)+35));p->disabled=1;p->burst=.001f;p->speed=0;p->boosting=0;return;}
   RaceItemActor held=a->actor[0];float reserve=r->reserve,boost=r->boost_time;
   race_player_repair(g);a->actor[0]=held;r->reserve=reserve;r->boost_time=boost;r->health=held.health/24.f;r->knockouts--; /* Existing serialized three-second race penalty. */
   RaceTrackFrame rejoin=race_track_frame(r,gate,0);
   g->yaw=atan2f(rejoin.ahead.x,rejoin.ahead.z);g->pitch=asinf(fmaxf(-1,fminf(1,rejoin.ahead.y)));
   Vec3 br={cosf(g->yaw),0,-sinf(g->yaw)},bu=norm(race_cross(rejoin.ahead,br));g->roll=atan2f(-dot(rejoin.up,br),dot(rejoin.up,bu));
   g->pos=add(rejoin.centre,add(mul(rejoin.up,RACE_TRACK_FLOOR+race_floor_support(g,0,rejoin)+35),mul(rejoin.ahead,150)));
   g->race.crash_time=0;
   message(g,"Missed landing. Race reset: 3 second penalty.");return;
  }
  if(surface!=1||height>=clearance+2400||height<clearance-250)attached&=~contact_bit;
  else if(fabsf(clearance-height)<120)attached|=contact_bit;
  if(surface==1&&height<clearance+2400&&height>=clearance-250){float error=clearance-height;*pos=add(*pos,mul(up,error*(1-expf(-dt*6))));if(fabsf(error)<120)attached|=contact_bit;direction=norm(sub(direction,mul(up,dot(direction,up))));if(clock>=53)jumping&=~contact_bit;}
  else if(surface!=2&&height<clearance)*pos=add(*pos,mul(up,clearance-height));
  if(!id&&surface==0){
   RaceTrackFrame old=race_track_nearest(r,gate,from);
   if(race_track_surface(r,old.section,old.t)==0){float held=dot(sub(from,old.centre),old.up);float current=dot(sub(*pos,centre),up);
    float roof=RACE_TRACK_FLOOR+sqrtf(fmaxf(0,tunnel*tunnel-lane*lane));held=fmaxf(clearance,fminf(roof,held));*pos=add(*pos,mul(up,held-current));}
   direction=norm(sub(direction,mul(up,dot(direction,up))));
  }
  if(surface==2){
   /* A forgiving magnetic-air guide keeps high-speed launches aligned with
    * the authored landing while gravity still supplies the visible arc. */
   RaceTrackFrame landing=ra_neon_clock_frame(r,gate,53);float across=fmaxf(-containment*.9f,fminf(containment*.9f,lane));
   Vec3 target=add(landing.centre,add(mul(landing.up,RACE_TRACK_FLOOR+race_floor_support(g,id,landing)+80),mul(landing.right,across)));
   Vec3 to_landing=sub(target,*pos);Vec3 guided=dot(to_landing,landing.ahead)>0?norm(to_landing):landing.ahead;float guide=fminf(1,dt*2.4f);direction=norm(add(mul(direction,1-guide),mul(guided,guide)));
   Vec3 velocity=sub(mul(direction,*speed),mul(landing.up,700*dt));direction=norm(velocity);*speed=fminf(length(velocity),r->top_speed*1.12f);
  }
  if(id)r->pilot[id-1].dir=direction;else {g->yaw=atan2f(direction.x,direction.z);g->pitch=asinf(fmaxf(-1,fminf(1,direction.y)));}
  /* Solve clearance after the new orientation and boundary deflection: hull
   * support changes during a bank, even when the centre's height is stable. */
  if(attached&contact_bit)for(int solve=0;solve<3;solve++){RaceTrackFrame f=race_track_nearest(r,gate,*pos);float target=RACE_TRACK_FLOOR+race_floor_support(g,id,f)+35,now=dot(sub(*pos,f.centre),f.up),error=target-now;if(fabsf(error)<2400)*pos=add(*pos,mul(f.up,error));
   float across=dot(sub(*pos,f.centre),f.right),rise=fmaxf(0,dot(sub(*pos,f.centre),f.up)-RACE_TRACK_FLOOR),radius=sqrtf(across*across+rise*rise);if(radius>containment){float factor=containment/radius;*pos=add(*pos,add(mul(f.right,across*factor-across),mul(f.up,rise*factor-rise)));}
  }
  return;
 }
 if(r->layout==4){
  int per=RACE_GATES/r->laps,launch=(gate/per)*per+(int)(per*.421875f);float lt=per*.421875f-(int)(per*.421875f);
  RaceTrackFrame lip=race_track_frame(r,launch,lt);uint32_t bit=1u<<launch;
  Vec3 direction=id?r->pilot[id-1].dir:forward(g);float lane=dot(sub(*pos,centre),right);
  if(gate==launch&&!(a->branch_boost[id]&bit)&&race_crossing(from,*pos,add(lip.centre,mul(lip.up,clearance)),lip.ahead,6000)>=0&&fabsf(lane)<7500){
   a->branch_boost[id]|=bit;direction=norm(add(lip.ahead,mul(lip.up,.16f)));*speed=fmaxf(*speed,r->top_speed*1.18f);a->actor[id].contact=.9f;
   if(!id){r->boost_time=.9f;r->flash=.6f;g->cue=SFX_BOOST;}
  }
  int surface=race_track_surface(r,frame.section,frame.t);
  if(surface==1&&fabsf(lane)>6500){*pos=sub(*pos,mul(right,lane-copysignf(6500,lane)));a->actor[id].contact=fmaxf(a->actor[id].contact,.2f);}
  int airborne=(a->branch_boost[id]&bit)&&(surface==2||(gate<=launch+2&&height>clearance+90));
  if(airborne){Vec3 velocity=sub(mul(direction,*speed),mul((Vec3){0,1,0},1150*dt));*speed=length(velocity);direction=norm(velocity);}
  else if(surface!=2&&fabsf(lane)<8500){
   if(height<clearance||surface==1){*pos=add(*pos,mul(up,clearance-height));direction=norm(sub(direction,mul(up,dot(direction,up))));if(height>clearance+90)a->actor[id].contact=.35f;}
  }
  if(id)r->pilot[id-1].dir=direction;else {g->yaw=atan2f(direction.x,direction.z);g->pitch=asinf(fmaxf(-1,fminf(1,direction.y)));}
  goto road_contact_done;
 }
 float kick=id?a->actor[id].contact:r->boost_time;
 if(kick>0&&kick<=.9f&&height<clearance+230&&fabsf(dot(sub(*pos,centre),right))<RACE_TRACK_WIDTH){float settle=sinf(fminf(1,(.9f-kick)/.9f)*3.14159265f);float target=clearance-27*settle;*pos=add(*pos,mul(up,(target-height)*fminf(1,dt*18)));height=dot(sub(*pos,centre),up);clearance=target;}
 if(height<clearance&&fabsf(dot(sub(*pos,centre),right))<RACE_TRACK_WIDTH){
  *pos=add(*pos,mul(up,clearance-height));
  if(a->actor[id].contact<=0)a->actor[id].contact=.2f;
  Vec3 direction=id?r->pilot[id-1].dir:forward(g);float downward=dot(direction,up);
  if(downward<0){direction=norm(sub(direction,mul(up,downward*fminf(1,dt*5))));
   if(id)r->pilot[id-1].dir=direction;else {g->yaw=atan2f(direction.x,direction.z);g->pitch=asinf(fmaxf(-1,fminf(1,direction.y)));}}
 }
 road_contact_done:
 if(a->rules.course!=RA_CORRIDOR)return;
 Vec3 d=sub(*pos,centre);float x=dot(d,right),y=dot(d,up),ellipse=sqrtf(x*x/(8900.f*8900.f)+y*y/(6400.f*6400.f));
 if(ellipse<=1)return;
 /* Sweep endpoint is projected to the inside. Removing outward displacement
  * gives a smooth slide even with a 2.5x rocket; no health/shield mutation. */
 Vec3 hit=add(centre,add(mul(right,x/ellipse),mul(up,y/ellipse)));
 Vec3 inward=norm(sub(centre,hit));*pos=add(hit,mul(inward,25));
 if(a->actor[id].contact<=0){*speed*=.78f;a->actor[id].contact=.35f;if(!id){a->wall_impact=hit;a->wall_ripple=.4f;}}
 if(id){RaceRival *p=&r->pilot[id-1];p->dir=norm(add(p->dir,mul(inward,dt*2)));}
 (void)from;
}
static int ra_repair_plane(Vec3 from,Vec3 to,Vec3 centre,const RaceGate *q,float z){
 Vec3 plane=add(centre,mul(q->normal,z));float a=dot(sub(from,plane),q->normal),b=dot(sub(to,plane),q->normal);if(a>=0||b<0||b-a<.0001f)return 0;
 Vec3 hit=sub(add(from,mul(sub(to,from),-a/(b-a))),centre);return fabsf(dot(hit,q->right))<1000&&fabsf(dot(hit,q->up))<1000;
}
/* Moving service strip: no checkpoint, reserve, item or protection reward.
 * Fractional HULL is cosmetic-independent and fixed-sized for all 15 actors. */
static void ra_neon_repair(Game *g,int id,Vec3 from,Vec3 to,RaceTrackFrame f,float dt){
 static const Game *owner;static unsigned serial,seed;static float age,fraction[RA_ACTORS];
 SpaceRace *r=&g->race;RaceItemActor *a=&r->arcade.actor[id];
 if(owner!=g||serial!=g->erl.serial||seed!=r->seed||r->elapsed<age){owner=g;serial=g->erl.serial;seed=r->seed;memset(fraction,0,sizeof(fraction));}age=r->elapsed;
 float clock=neon_time(((f.section%(RACE_GATES/r->laps))+f.t)/(RACE_GATES/r->laps));
 float edge=190*2.5f*race_neon_scale(r),lane=dot(sub(to,f.centre),f.right),height=dot(sub(to,f.centre),f.up)-RACE_TRACK_FLOOR-race_floor_support(g,id,f);
 if(!erl_service_clock(clock)||fabsf(lane-edge*ERL_SERVICE_LANE)>edge*.12f||height<10||height>110||dot(sub(to,from),f.ahead)<r->top_speed*.12f*dt||a->health<=0||a->health>=RA_HEALTH){fraction[id]=0;return;}
 fraction[id]+=RA_HEALTH*.15f*dt;int heal=(int)fraction[id];fraction[id]-=heal;a->health=a->health+heal>RA_HEALTH?RA_HEALTH:a->health+heal;ra_health_sync(g,id);int entering=a->decision<=0;a->decision=.25f;
 if(!id&&entering)g->cue=SFX_SCAN;
}
static void ra_service_crossing(Game *g,int id,Vec3 from,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;int gate=id?r->pilot[id-1].gate:r->gate;if(gate>=RACE_GATES||!ra_eligible(g,id))return;
 RaceGate *q=&r->gates[gate];uint32_t bit=1u<<gate;
 Vec3 to=ra_position(g,id);RaceTrackFrame pf=race_track_nearest(r,gate,to);float support=race_floor_support(g,id,pf);
 /* Authored beds may cross a checkpoint. Attribute collection to the pad's
  * anchor section, including its tail after the actor earns the next gate. */
 if(r->layout==5){
  ra_neon_repair(g,id,from,to,pf,dt);
  /* Rings and floor arrows deliberately share the same short, audible kick.
   * Ring history is transient physical contact state, reset for every race. */
  static const Game *ring_owner;static unsigned ring_serial,ring_seed,ring_seen[RA_ACTORS];static float ring_age;
  if(ring_owner!=g||ring_serial!=g->erl.serial||ring_seed!=r->seed||r->elapsed<ring_age){ring_owner=g;ring_serial=g->erl.serial;ring_seed=r->seed;memset(ring_seen,0,sizeof(ring_seen));}ring_age=r->elapsed;
  int per=RACE_GATES/r->laps;float scale=race_neon_scale(r),body=id?r->pilot[id-1].radius:race_neon_body_radius(g);
  for(int lap=0;lap<r->laps;lap++)for(int ring=0;ring<NEON_RING_COUNT;ring++){int key=lap*NEON_RING_COUNT+ring;
   float arc=neon_sky_rings[ring][0]*per;int section=lap*per+(int)arc;if(section<gate-1||section>gate+1)continue;RaceTrackFrame rf=race_track_frame(r,section,arc-(int)arc);
   Vec3 centre=add(rf.centre,mul(rf.up,RACE_TRACK_FLOOR+neon_sky_rings[ring][1]*scale));
   if(race_crossing(from,to,centre,rf.ahead,ERL_BOOST_RING_RADIUS*scale+body*.30f)<0)continue;
   if(ring_seen[id]&(1u<<key))return;
   /* On the two-lap Race 1, spare non-pad section bits retain ring
    * consumption across save/reload without changing the v38 payload. */
   int stamp=-1,unused=0;for(int k=0;k<per;k++)if(!race_neon_pad(r,lap*per+k,0,0)){if(unused++==ring){stamp=lap*per+k;break;}}
   if(stamp>=0&&(a->branch_repair[id]&(1u<<stamp)))return;
   if(stamp>=0)a->branch_repair[id]|=1u<<stamp;
   ring_seen[id]|=1u<<key;a->actor[id].respawn=.65f;a->actor[id].contact=.65f;if(id)r->pilot[id-1].boost_gate=section;else {r->boost_time=.65f;r->flash=.55f;g->cue=SFX_BOOST;ra_boost_flash(g,centre,rf,1);}return;
  }
  for(int anchor=gate>0?gate-1:0;anchor<=gate;anchor++){
   uint32_t pad_bit=1u<<anchor;if((a->branch_boost[id]|a->branch_repair[id])&pad_bit)continue;
   if(!race_pad_crossing(r,anchor,from,to,support))continue;
   a->branch_boost[id]|=pad_bit;a->actor[id].respawn=.65f;a->actor[id].contact=.65f;
   if(id)r->pilot[id-1].boost_gate=anchor;else {r->boost_time=.65f;r->flash=.55f;g->cue=SFX_BOOST;
    float t=.5f,lane=0;race_neon_pad(r,anchor,&t,&lane);RaceTrackFrame flash=race_track_frame(r,anchor,t);ra_boost_flash(g,add(flash.centre,add(mul(flash.right,lane),mul(flash.up,RACE_TRACK_FLOOR+8*scale))),flash,0);
   }return;
  }return;}
 if((a->branch_boost[id]|a->branch_repair[id])&bit)return;
 if(race_pad_crossing(r,gate,from,to,support)){a->branch_boost[id]|=bit;if(id){RaceRival *p=&r->pilot[id-1];p->boost_gate=gate;a->actor[id].contact=.9f;}
  else {r->boost_time=.9f;a->actor[0].contact=.9f;r->flash=.8f;g->cue=SFX_BOOST;}return;}
 if(r->layout==4||r->layout==5)return;
 if(gate%4!=2){a->actor[id].decision=0;return;}
 RaceTrackFrame service=race_repair_frame(r,gate);RaceGate aligned=*q;aligned.normal=service.ahead;aligned.right=service.right;aligned.up=service.up;q=&aligned;
 Vec3 c=service.centre,offset=sub(to,c);
 if(ra_repair_plane(from,to,c,q,-RA_REPAIR_HALF_LENGTH))a->actor[id].decision=8;
 if(fabsf(dot(offset,q->right))>1000||fabsf(dot(offset,q->up))>1000||dot(offset,q->normal)<-RA_REPAIR_HALF_LENGTH-25)a->actor[id].decision=0;
 if(!a->actor[id].decision||!ra_repair_plane(from,to,c,q,RA_REPAIR_HALF_LENGTH))return;
 /* Forward traversal of the rectangular tunnel is the only repair trigger. One mutually
  * exclusive choice per section, retained through checkpoint reloads. */
 a->branch_repair[id]|=bit;a->actor[id].health=RA_HEALTH;a->actor[id].decision=1;ra_health_sync(g,id);if(!id){a->last_hit=3;a->cue_age=.6f;g->cue=SFX_SCAN;}
}
