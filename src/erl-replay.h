/* Presentation-only recent action. 65 samples at 10 Hz, never frame video or
 * a Game copy. The replay cannot execute physics, saves or reward callbacks. */
#define ERL_REPLAY_SAMPLES 65
#define ERL_REPLAY_SHOTS 8
/* Preserve all 65 history samples. Store only presentation fields, not AI,
 * reserve/accounting and results data that replay is forbidden to execute. */
typedef struct {Vec3 pos,dir,impact;float speed,bump,burst,radius,protection;uint16_t health24,mesh;uint8_t gate,disabled,boosting;} ErlReplayPilot;
/* HULL already comes from integer actor health /24. Store that original
 * integer, retaining its exact displayed fraction without a redundant float. */
static ErlReplayPilot erl_replay_pilot_pack(const RaceRival *p){return (ErlReplayPilot){.pos=p->pos,.dir=p->dir,.impact=p->impact,.speed=p->speed,.bump=p->bump,.burst=p->burst,.radius=p->radius,.protection=p->protection,.health24=(uint16_t)fmaxf(0,fminf(RA_HEALTH,p->health*24.f+.5f)),.mesh=p->mesh,.gate=p->gate,.disabled=p->disabled,.boosting=p->boosting};}
static void erl_replay_pilot_unpack(RaceRival *p,const ErlReplayPilot *s){p->pos=s->pos;p->dir=s->dir;p->impact=s->impact;p->speed=s->speed;p->bump=s->bump;p->burst=s->burst;p->radius=s->radius;p->protection=s->protection;p->health=s->health24/24.f;p->gate=s->gate;p->mesh=s->mesh;p->disabled=s->disabled;p->boosting=s->boosting;}

/* Replay never runs projectile physics. Retain all eight visible rounds,
 * their exact vectors/type/live state; omit collision/AI timers and owners. */
typedef struct {Vec3 pos,dir;unsigned char kind,live;} ErlReplayShot;
static ErlReplayShot erl_replay_shot_pack(const RaceProjectile *p){return (ErlReplayShot){p->pos,p->dir,p->kind,p->live};}
static void erl_replay_shot_unpack(RaceProjectile *p,const ErlReplayShot *s){p->pos=s->pos;p->dir=s->dir;p->kind=s->kind;p->live=s->live;}
_Static_assert(sizeof(ErlReplayShot)==28,"Replay shot stores only exact presentation fields");
typedef struct {
 Vec3 pos;float yaw,pitch,roll,speed,time,elapsed;int gate;uint32_t boost,repair;
 ErlReplayPilot pilot[RACE_RIVALS];ErlReplayShot shot[ERL_REPLAY_SHOTS];
 int health[RA_ACTORS];float shield[RA_ACTORS],protection[RA_ACTORS];
} ErlReplayFrame;
static struct {ErlReplayFrame sample[ERL_REPLAY_SAMPLES];unsigned serial,played;int next,count,active;float clock,age;int crash_pinned,crash_start,crash_count;SpaceRace course;int hull;unsigned paint;} erl_replay;
static void erl_replay_capture(float dt){
 if(!game.erl.event_kind)return;
 if(erl_replay.serial!=game.erl.serial){erl_replay.serial=game.erl.serial;erl_replay.count=erl_replay.next=erl_replay.active=erl_replay.crash_pinned=0;erl_replay.clock=0;erl_replay.course=game.race;erl_replay.hull=game.ship;erl_replay.paint=ship_paint[game.ship];}
 if(game.race.layout==5&&game.race.crash_time>0&&!erl_replay.crash_pinned){erl_replay.crash_pinned=1;erl_replay.crash_count=erl_replay.count<21?erl_replay.count:21;erl_replay.crash_start=(erl_replay.next+ERL_REPLAY_SAMPLES-erl_replay.crash_count)%ERL_REPLAY_SAMPLES;}
 if(game.race.crash_time<=0)erl_replay.crash_pinned=0;
 if(game.race.phase!=RACE_RUNNING&&!game.erl.result)return;
 erl_replay.clock+=dt;if(erl_replay.clock<.1f&&!game.erl.result)return;erl_replay.clock=fmodf(erl_replay.clock,.1f);
 ErlReplayFrame *f=&erl_replay.sample[erl_replay.next];f->pos=game.pos;f->yaw=game.yaw;f->pitch=game.pitch;f->roll=game.roll;f->speed=game.speed;f->time=game.time;f->elapsed=game.race.elapsed;f->gate=game.race.gate;f->boost=game.race.arcade.branch_boost[0];f->repair=game.race.arcade.branch_repair[0];
 for(int i=0;i<RACE_RIVALS;i++)f->pilot[i]=erl_replay_pilot_pack(&game.race.pilot[i]);memset(f->shot,0,sizeof(f->shot));int n=0;for(int i=0;i<RA_PROJECTILES&&n<ERL_REPLAY_SHOTS;i++)if(game.race.arcade.shot[i].live)f->shot[n++]=erl_replay_shot_pack(&game.race.arcade.shot[i]);
 for(int i=0;i<RA_ACTORS;i++){f->health[i]=game.race.arcade.actor[i].health;f->shield[i]=game.race.arcade.actor[i].shield;f->protection[i]=game.race.arcade.actor[i].protection;}
 erl_replay.next=(erl_replay.next+1)%ERL_REPLAY_SAMPLES;if(erl_replay.count<ERL_REPLAY_SAMPLES)erl_replay.count++;
}
static int erl_replay_tick(unsigned pressed,float dt){
 if(erl_replay.played==game.erl.serial)return 0;
 if(erl_replay.serial!=game.erl.serial||erl_replay.count<2){erl_replay.played=game.erl.serial;return 0;}
 if(!erl_replay.active){erl_replay.active=1;erl_replay.age=0;}
 erl_replay.age=fminf(6,erl_replay.age+dt);
 if((pressed&PSP_CTRL_CROSS)||erl_replay.age>=6){erl_replay.active=0;erl_replay.played=game.erl.serial;}
 return 1; /* The skip press never operates the results underneath. */
}
static Vec3 erl_replay_lerp(Vec3 a,Vec3 b,float t){return add(a,mul(sub(b,a),t));}
static float erl_replay_angle(float a,float b,float t){float d=fmodf(b-a+9.42477796f,6.2831853f)-3.14159265f;return a+d*t;}
static void erl_replay_render(int television){
 int old_x0=clipx0,old_x1=clipx1,old_y0=clipy0,old_y1=clipy1,old_ox=proj_ox,old_oy=proj_oy;
 int start=(erl_replay.next+ERL_REPLAY_SAMPLES-erl_replay.count)%ERL_REPLAY_SAMPLES;float sample=erl_replay.age/6*(erl_replay.count-1);int at=(int)sample;float fraction=sample-at;
 ErlReplayFrame *a=&erl_replay.sample[(start+at)%ERL_REPLAY_SAMPLES],*b=&erl_replay.sample[(start+(at+1<erl_replay.count?at+1:at))%ERL_REPLAY_SAMPLES];
 Vec3 position=game.pos;float yaw=game.yaw,pitch=game.pitch,roll=game.roll,speed=game.speed,time=game.time,elapsed=game.race.elapsed;int gate=game.race.gate,phase=game.race.phase,person=third_person;
 uint32_t saved_boost=game.race.arcade.branch_boost[0],saved_repair=game.race.arcade.branch_repair[0];RaceRival pilots[RACE_RIVALS];RaceProjectile shots[RA_PROJECTILES];RaceItemActor actors[RA_ACTORS];memcpy(pilots,game.race.pilot,sizeof(pilots));memcpy(shots,game.race.arcade.shot,sizeof(shots));memcpy(actors,game.race.arcade.actor,sizeof(actors));
 game.pos=erl_replay_lerp(a->pos,b->pos,fraction);game.yaw=erl_replay_angle(a->yaw,b->yaw,fraction);game.pitch=erl_replay_angle(a->pitch,b->pitch,fraction);game.roll=erl_replay_angle(a->roll,b->roll,fraction);game.speed=a->speed+(b->speed-a->speed)*fraction;game.time=a->time+(b->time-a->time)*fraction;game.race.elapsed=a->elapsed+(b->elapsed-a->elapsed)*fraction;game.race.gate=a->gate;game.race.phase=RACE_RUNNING;game.race.arcade.branch_boost[0]=a->boost;game.race.arcade.branch_repair[0]=a->repair;
 for(int i=0;i<RACE_RIVALS;i++)erl_replay_pilot_unpack(&game.race.pilot[i],&a->pilot[i]);for(int i=0;i<game.race.rivals;i++){game.race.pilot[i].pos=erl_replay_lerp(a->pilot[i].pos,b->pilot[i].pos,fraction);game.race.pilot[i].dir=norm(erl_replay_lerp(a->pilot[i].dir,b->pilot[i].dir,fraction));}
 memset(game.race.arcade.shot,0,sizeof(shots));for(int i=0;i<ERL_REPLAY_SHOTS;i++)erl_replay_shot_unpack(&game.race.arcade.shot[i],&a->shot[i]);for(int i=0;i<RA_ACTORS;i++){game.race.arcade.actor[i].health=a->health[i];game.race.arcade.actor[i].shield=a->shield[i];game.race.arcade.actor[i].protection=a->protection[i];}
 Vec3 dir=forward(&game),right=norm(race_ui_cross((Vec3){0,1,0},dir)),eye,focus=add(game.pos,mul(dir,150));int cut=(int)(erl_replay.age/2.f);
 if(cut==0)eye=add(game.pos,add(mul(dir,-950),add(mul(right,650),(Vec3){0,320,0})));
 else if(cut==1){int anchor=(start+erl_replay.count/3)%ERL_REPLAY_SAMPLES;eye=add(erl_replay.sample[anchor].pos,add(mul(right,1500),(Vec3){0,650,0}));}
 else eye=add(game.pos,add(mul(dir,-430),add(mul(right,-290),(Vec3){0,160,0})));
 Vec3 look=norm(sub(focus,eye));camera_override_set(eye,atan2f(look.x,look.z),asinf(fmaxf(-1,fminf(1,look.y))),0);if(television)preview_clip(258,97,166,45,350,149);else preview_reset();
 int min=surface_y_min,max=surface_y_max,depth=surface_depth_on,encoding=mega_depth_encoding,material=surface_material;float light=surface_light,focal=proj_focal;proj_focal=cut==1?fminf(8000,fmaxf(400,length(sub(eye,game.pos))*.6f)):cut==0?460:340;
 if(television){rect(166,45,184,104,EB);proj_focal*=184.f/480;}else rect(0,0,W,H,EB);surface_y_min=television?45:20;surface_y_max=television?148:H-23;surface_depth_on=1;mega_depth_encoding=1;surface_material=0;surface_light=1;memset(surface_depth,255,sizeof(surface_depth));race_starfield();race_world_draw();flush_solid_meshes();race_holo_track_draw();race_pad_art();race_boundary_glass();race_ghost_art();race_projectile_art();race_dressing_art();mega_depth_valid=1;third_person=1;flight_camera_ship();
 surface_y_min=min;surface_y_max=max;surface_depth_on=depth;mega_depth_encoding=encoding;surface_material=material;surface_light=light;proj_focal=focal;
 if(!television){rect(0,0,W,20,EB);rect(0,H-23,W,23,EB);erl_text(12,6,ER,"ERL / ACTUAL RACE REPLAY",1);erl_text(354,H-16,ET,"X SKIP",1);for(int i=0;i<3;i++)rect(12+i*16,H-15,12,3,i==cut?ER:EP);}
 game.pos=position;game.yaw=yaw;game.pitch=pitch;game.roll=roll;game.speed=speed;game.time=time;game.race.elapsed=elapsed;game.race.gate=gate;game.race.phase=phase;third_person=person;game.race.arcade.branch_boost[0]=saved_boost;game.race.arcade.branch_repair[0]=saved_repair;memcpy(game.race.pilot,pilots,sizeof(pilots));memcpy(game.race.arcade.shot,shots,sizeof(shots));memcpy(game.race.arcade.actor,actors,sizeof(actors));camera_override_clear();clipx0=old_x0;clipx1=old_x1;clipy0=old_y0;clipy1=old_y1;proj_ox=old_ox;proj_oy=old_oy;
}

static void erl_replay_draw(void){erl_replay_render(0);}
static int erl_tv_has_highlights(void){
 const ErlProgress *p=&game.erl_local.progress;
 return !local_tv_program&&local_tv_beat==2&&p->records&&erl_replay.count>=2&&erl_replay.serial==game.erl.paid&&fabsf(erl_replay.sample[(erl_replay.next+ERL_REPLAY_SAMPLES-1)%ERL_REPLAY_SAMPLES].elapsed-p->latest_time)<=.2f;
}
static void erl_tv_highlights(void){
 if(!erl_tv_has_highlights())return;
 SpaceRace saved=game.race;int kind=game.erl.event_kind,system=game.system,hull=game.ship;float age=erl_replay.age;unsigned paint=ship_paint[erl_replay.hull];
 game.race=erl_replay.course;game.system=game.race.system;game.erl.event_kind=ERL_TRIAL;game.ship=erl_replay.hull;ship_paint[game.ship]=erl_replay.paint;erl_replay.age=fmodf(local_tv_clock,6);
 erl_replay_render(1);
 game.race=saved;game.erl.event_kind=kind;game.system=system;game.ship=hull;ship_paint[erl_replay.hull]=paint;erl_replay.age=age;
 line(166,44,349,44,ET);line(166,149,349,149,ER);text_px(171,48,ET,"ERL / REPLAY");
}

/* A three-second cutaway pins at most21 existing samples. At10Hz, only30
 * new live samples arrive before return: 21+30 fits the unchanged65-slot ring.
 * Rendering saves/restores transforms; no physics, awards or saves run here. */
static struct {Vec3 pos;float yaw,pitch,roll,speed,time,elapsed;int gate,person;RaceRival pilot[RACE_RIVALS];} erl_crash_live;
static int erl_crash_visual_begin(void){
 if(game.race.layout!=5||game.race.crash_time<=0||erl_crash_visual_active)return 0;
 erl_crash_live.pos=game.pos;erl_crash_live.yaw=game.yaw;erl_crash_live.pitch=game.pitch;erl_crash_live.roll=game.roll;erl_crash_live.speed=game.speed;erl_crash_live.time=game.time;erl_crash_live.elapsed=game.race.elapsed;erl_crash_live.gate=game.race.gate;erl_crash_live.person=third_person;memcpy(erl_crash_live.pilot,game.race.pilot,sizeof(erl_crash_live.pilot));
 erl_crash_visual_active=1;erl_crash_visual_age=3-game.race.crash_time;third_person=1;
 if(erl_replay.crash_pinned&&erl_replay.crash_count>1){
  float sample=fminf(1,erl_crash_visual_age/.8f)*(erl_replay.crash_count-1);int n=(int)sample;float mix=sample-n;
  ErlReplayFrame *a=&erl_replay.sample[(erl_replay.crash_start+n)%ERL_REPLAY_SAMPLES],*b=&erl_replay.sample[(erl_replay.crash_start+(n+1<erl_replay.crash_count?n+1:n))%ERL_REPLAY_SAMPLES];
  game.pos=erl_replay_lerp(a->pos,b->pos,mix);game.yaw=erl_replay_angle(a->yaw,b->yaw,mix);game.pitch=erl_replay_angle(a->pitch,b->pitch,mix);game.roll=erl_replay_angle(a->roll,b->roll,mix);game.speed=a->speed;game.time=a->time;game.race.elapsed=a->elapsed;game.race.gate=a->gate;
  for(int i=0;i<game.race.rivals;i++){erl_replay_pilot_unpack(&game.race.pilot[i],&a->pilot[i]);game.race.pilot[i].pos=erl_replay_lerp(a->pilot[i].pos,b->pilot[i].pos,mix);game.race.pilot[i].dir=norm(erl_replay_lerp(a->pilot[i].dir,b->pilot[i].dir,mix));}
 }
 if((!erl_replay.crash_pinned||erl_replay.crash_count<2)&&game.race.repair>0){game.pos=game.race.crash_pos;game.yaw=atan2f(game.race.crash_dir.x,game.race.crash_dir.z);game.pitch=asinf(fmaxf(-1,fminf(1,game.race.crash_dir.y)));}
 if(erl_crash_visual_age>=.8f){
  if(game.race.repair>0){game.pos=game.race.crash_pos;game.yaw=atan2f(game.race.crash_dir.x,game.race.crash_dir.z);game.pitch=asinf(fmaxf(-1,fminf(1,game.race.crash_dir.y)));}
  else if(game.race.crash_id>=0){RaceRival *p=&game.race.pilot[game.race.crash_id];p->pos=game.race.crash_pos;p->dir=game.race.crash_dir;p->disabled=1;p->burst=(erl_crash_visual_age-.8f)*.65f;}
 }
 return 1;
}
static void erl_crash_visual_end(void){
 if(!erl_crash_visual_active)return;
 game.pos=erl_crash_live.pos;game.yaw=erl_crash_live.yaw;game.pitch=erl_crash_live.pitch;game.roll=erl_crash_live.roll;game.speed=erl_crash_live.speed;game.time=erl_crash_live.time;game.race.elapsed=erl_crash_live.elapsed;game.race.gate=erl_crash_live.gate;third_person=erl_crash_live.person;memcpy(game.race.pilot,erl_crash_live.pilot,sizeof(erl_crash_live.pilot));erl_crash_visual_active=0;
}
static void erl_crash_live_camera(Vec3 *eye,Vec3 *ahead,Vec3 *up){
 Vec3 pos=game.pos;float yaw=game.yaw,pitch=game.pitch,roll=game.roll,shot=game.race.crash_time;int person=third_person;
 if(erl_crash_visual_active){game.pos=erl_crash_live.pos;game.yaw=erl_crash_live.yaw;game.pitch=erl_crash_live.pitch;game.roll=erl_crash_live.roll;third_person=erl_crash_live.person;}
 game.race.crash_time=0;flight_camera_prepare();*eye=camera_origin(&game);Vec3 right;camera_axes(&game,&right,up,ahead);
 game.pos=pos;game.yaw=yaw;game.pitch=pitch;game.roll=roll;game.race.crash_time=shot;third_person=person;
}
