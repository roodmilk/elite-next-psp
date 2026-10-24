/* Race-only combat, contested pickup rows and stable participant IDs.
 * Fixed pools, swept segments, no allocation/file I/O in the simulation. */
static Vec3 ra_position(const Game *g,int id){return id?g->race.pilot[id-1].pos:g->pos;}
static Vec3 ra_direction(const Game *g,int id){return id?g->race.pilot[id-1].dir:forward(g);}
static float ra_speed(const Game *g,int id){return id?g->race.pilot[id-1].speed:g->speed;}
static int ra_eligible(const Game *g,int id){const SpaceRace *r=&g->race;return id>=0&&id<=r->rivals&&!r->arcade.actor[id].eliminated&&(id?!r->pilot[id-1].disabled&&r->pilot[id-1].gate<RACE_GATES:r->gate<RACE_GATES&&r->repair<=0);}
static unsigned ra_random(RaceArcade *a){unsigned x=a->rng?a->rng:1;x^=x<<13;x^=x>>17;x^=x<<5;return a->rng=x;}
static void race_arcade_init(Game *g){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;memset(a,0,sizeof(*a));
 a->rules=ra_rules_make(r->discipline>=ERL_RUSH?RA_COMBAT:RA_CLEAN,RA_FINISH,r->discipline>=ERL_RUSH?RA_POOL_ALL:RA_POOL_CLEAN,r->seed);
 if(g->erl.event_kind&&ra_rules_valid(&g->erl_local.event_rules,g->erl.licence))a->rules=g->erl_local.event_rules;
 else if(race_job(g)>=0){int job=race_job(g);a->rules=g->jobs[job].type==MISSION_RACE?race_contract_rules(g,&g->jobs[job],r->seed):erl_catalogue_rules(r->seed,g->erl.licence,(int)(r->seed%18));a->rules.context=2;}
 r->rivals=a->rules.objective==RA_TAKEDOWNS?6:a->rules.field-1;r->laps=a->rules.laps;r->limit=a->rules.seconds;
 a->rng=r->seed^0xa7cadedu;a->ready=1;a->lock=a->warning=-1;
 for(int i=0;i<RA_ACTORS;i++){ra_actor_reset(&a->actor[i]);a->generation[i]=1;}
 for(int i=0;i<RA_PICKUPS;i++){RacePickup *p=&a->pickup[i];p->section=2+(i/3)*4;p->lane=i%3-1;RaceGate *q=&r->gates[p->section];Vec3 prev=p->section?r->gates[p->section-1].pos:r->start;p->pos=add(mul(add(prev,q->pos),.5f),mul(q->right,p->lane*800.f));if(r->layout==4){RaceTrackFrame f=race_track_frame(r,p->section,.5f);p->pos=add(f.centre,add(mul(f.right,p->lane*800.f),mul(f.up,RACE_TRACK_FLOOR+170)));}
  else if(r->layout==5){float q=((i/2)+.75f)*(RACE_GATES/r->laps)/12.f;int section=(int)q;if(race_track_surface(r,section,q-section)==2)q+=.6f;section=(int)q;p->section=section;p->lane=i%2?1:-1;RaceTrackFrame f=race_track_frame(r,section,q-section);float lane=p->lane*75*2.5f*race_neon_scale(r)*.65f;p->pos=add(f.centre,add(mul(f.right,lane),mul(f.up,RACE_TRACK_FLOOR+170)));}
 }
}
void race_item_input(Game *g,int held){if(!race_ready(g))return;RaceArcade *a=&g->race.arcade;if(!a->ready)race_arcade_init(g);a->input_x=!!held;if(g->race.phase!=RACE_RUNNING){a->actor[0].previous_x=!!held;a->actor[0].require_release=!!held;}}
static int ra_lock_target(const Game *g,int owner){
 const SpaceRace *r=&g->race;Vec3 origin=ra_position(g,owner),dir=ra_direction(g,owner);int best=-1;float nearest=18000;
 for(int id=0;id<=r->rivals;id++)if(id!=owner&&ra_eligible(g,id)&&r->arcade.actor[id].protection<=0){Vec3 d=sub(ra_position(g,id),origin);float dist=length(d);if(dist>100&&dist<nearest&&dot(mul(d,1/dist),dir)>.91f){best=id;nearest=dist;}}return best;
}
static void ra_health_sync(Game *g,int id){float health=g->race.arcade.actor[id].health/24.f;if(id)g->race.pilot[id-1].health=health;else g->race.health=health;}
static int ra_hit(Game *g,int victim,int damage,int heavy,int owner){
 SpaceRace *r=&g->race;RaceArcade *arc=&r->arcade;if(!ra_eligible(g,victim))return RA_DAMAGE_NONE;
 RaceItemActor *a=&arc->actor[victim];float protection=victim?r->pilot[victim-1].protection:r->protection;if(protection>a->protection)a->protection=protection;
 int result=ra_damage(a,damage,heavy);ra_health_sync(g,victim);
 if(result==RA_DAMAGE_TAKEDOWN){arc->generation[victim]++;if(victim){race_burst(g,victim-1,owner==0);if(owner>0){r->pilot[owner-1].score++;r->pilot[owner-1].score_time=r->elapsed;}}else {if(owner>0){r->pilot[owner-1].score++;r->pilot[owner-1].score_time=r->elapsed;}race_player_repair(g);}}
 if(result==RA_DAMAGE_SHIELD_BREAK){arc->last_hit=2;arc->cue_age=.6f;if(!victim)g->cue=SFX_SCAN;}
 else if(r->layout!=5&&result==RA_DAMAGE_HIT&&heavy&&damage==RA_MISSILE_DAMAGE){a->spin=1.2f;a->protection=.55f;if(victim)r->pilot[victim-1].speed*=.28f;else g->speed*=.28f;}
 return result;
}
static void ra_fire(Game *g,int owner,int kind,int target){
 RaceArcade *a=&g->race.arcade;RaceProjectile *p=0;
 for(int i=0;i<RA_PROJECTILES;i++)if(!a->shot[i].live){p=&a->shot[i];break;}if(!p)return;
 memset(p,0,sizeof(*p));p->live=1;p->kind=kind;p->owner=owner;p->target=kind==RA_MISSILE?target:-1;p->generation=target>=0?a->generation[target]:0;
 p->dir=ra_direction(g,owner);p->pos=add(ra_position(g,owner),mul(p->dir,180));p->life=kind==RA_MISSILE?4:.65f;
 /* An unlocked rear rocket uses the existing serialized missile shape.
  * UINT32_MAX is an explicit straight-shot tag, never a target generation. */
 if(!owner&&g->race.layout==5&&g->race.rear_input&&kind==RA_MISSILE){p->dir=mul(p->dir,-1);p->pos=add(g->pos,mul(p->dir,350));p->target=-1;p->generation=UINT32_MAX;p->life=1.75f;}
 if(!owner){a->launch_age=.12f;g->cue=kind==RA_MISSILE?SFX_MISSILE:SFX_LASER;}
}
static void ra_pickup_tick(Game *g,Vec3 previous,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;
 for(int k=0;k<RA_PICKUPS;k++){RacePickup *p=&a->pickup[k];p->cooldown=fmaxf(0,p->cooldown-dt);if(p->cooldown>0)continue;
  /* Resolve the earliest swept contact, rather than always preferring player. */
  int winner=-1;float first=2;
  for(int id=0;id<=r->rivals;id++)if(ra_eligible(g,id)&&a->actor[id].protection<=0){Vec3 to=ra_position(g,id),from=id?sub(to,mul(ra_direction(g,id),ra_speed(g,id)*dt)):previous;Vec3 d=sub(to,from);float t=fmaxf(0,fminf(1,dot(sub(p->pos,from),d)/fmaxf(1,dot(d,d))));if(length(sub(add(from,mul(d,t)),p->pos))<580&&t<first){winner=id;first=t;}}
  if(winner>=0){int place=winner?2:r->place;int item=ra_roll(ra_random(a),a->rules.pool,place,r->rivals+1);if(a->rules.mode==RA_TIME_TRIAL)item=RA_BOOST;ra_collect(&a->actor[winner],item,winner?0:a->input_x);p->cooldown=5;if(!winner)g->cue=SFX_SCAN;}
 }
}
static void ra_projectile_tick(Game *g,Vec3 previous,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;a->warning=-1;float closest=1.15f;
 for(int k=0;k<RA_PROJECTILES;k++){RaceProjectile *p=&a->shot[k];if(!p->live)continue;if(a->actor[p->owner].eliminated){p->live=0;continue;}p->age+=dt;p->life=fmaxf(0,p->life-dt);if(p->life<=0){p->live=0;continue;}
  if(p->kind==RA_MISSILE&&p->target>=0){int id=p->target;if(!ra_eligible(g,id)||p->generation!=a->generation[id]){p->target=-1;p->life=fminf(p->life,.5f);}else {Vec3 desired=norm(sub(ra_position(g,id),p->pos));float alignment=dot(p->dir,desired);if(alignment<-.1f){p->target=-1;p->life=fminf(p->life,.5f);}else {Vec3 axis=race_cross(p->dir,desired);float angle=acosf(fmaxf(-1,fminf(1,alignment))),step=fminf(angle,dt*1.35f);if(length(axis)>.0001f)p->dir=norm(flight_rotate(p->dir,norm(axis),cosf(step),sinf(step)));};}}
  Vec3 from=p->pos;float speed=p->kind==RA_MISSILE?r->top_speed*(p->generation==UINT32_MAX&&p->target<0?1.35f:1.85f):r->top_speed*3.6f;p->pos=add(from,mul(p->dir,speed*dt));
  if(race_scenery_hit(r,r->gate,from,p->pos,12,dt,0)){p->live=0;continue;}
  /* A real forward tracer can destroy an intersected missile. */
  if(p->kind==RA_MINIGUN)for(int j=0;j<RA_PROJECTILES;j++){RaceProjectile *m=&a->shot[j];if(m->live&&m->kind==RA_MISSILE&&m->owner!=p->owner&&race_segment_distance(from,p->pos,m->pos)<65){m->live=p->live=0;break;}}
  if(!p->live)continue;
  for(int id=0;id<=r->rivals;id++)if(id!=p->owner&&ra_eligible(g,id)){
   Vec3 to=ra_position(g,id),old=id?sub(to,mul(ra_direction(g,id),ra_speed(g,id)*dt)):previous;
   float radius=id?r->pilot[id-1].radius:85;
   if(race_segment_distance(sub(from,old),sub(p->pos,to),(Vec3){0,0,0})<radius+25){
    int damage=p->kind==RA_MISSILE?RA_MISSILE_DAMAGE:RA_BULLET_DAMAGE;if(r->layout==5)damage=p->kind==RA_MISSILE?600:24;
    int result=ra_hit(g,id,damage,p->kind==RA_MISSILE,p->owner);
    if(r->layout==5&&p->kind==RA_MISSILE&&result==RA_DAMAGE_HIT){a->actor[id].spin=.3f;if(id)r->pilot[id-1].speed*=.88f;else g->speed*=.88f;}
    p->live=0;break;}
  }
  if(p->live&&p->kind==RA_MISSILE&&p->target==0){float t=length(sub(p->pos,g->pos))/fmaxf(1,speed);if(t<closest){closest=t;a->warning=k;a->warning_side=dot(norm(sub(p->pos,g->pos)),forward(g))<0?-1:1;}}
 }
 if(a->warning>=0&&a->warning_gap<=0){g->cue=SFX_ALERT;a->warning_gap=.65f;}a->warning_gap=fmaxf(0,a->warning_gap-dt);
}
static void race_arcade_tick(Game *g,Vec3 previous,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;if(!a->ready)race_arcade_init(g);
 a->entry+=dt;a->wall_ripple=fmaxf(0,a->wall_ripple-dt);a->cue_age=fmaxf(0,a->cue_age-dt);a->launch_age=fmaxf(0,a->launch_age-dt);
 for(int id=0;id<=r->rivals;id++){
  RaceItemActor *p=&a->actor[id];ra_actor_tick(p,dt);
  /* Legacy recovery timer is the crash delay; protection starts on rejoin. */
  if(id&&!r->pilot[id-1].disabled&&p->health<=0){ra_respawn(p);r->pilot[id-1].protection=3;}
  if(!id&&!r->repair&&p->health<=0){ra_respawn(p);r->protection=3;g->speed=r->top_speed*.3f;}
  if(!ra_eligible(g,id))continue;
  int held=id?0:a->input_x,target=ra_lock_target(g,id),available=p->protection<=0&&p->spin<=0&&(id||(r->crash_time<=0&&r->handover<=0&&r->repair<=0));
  if(id){RaceRival *pilot=&r->pilot[id-1];int gate=pilot->gate<RACE_GATES?pilot->gate:RACE_GATES-1;Vec3 desired=norm(sub(r->gates[gate].pos,pilot->pos));
   if(p->item==RA_BOOST)held=dot(pilot->dir,desired)>.985f&&length(sub(r->gates[gate].pos,pilot->pos))>r->top_speed*2;
   else if(p->item==RA_SHIELD){for(int j=0;j<RA_PROJECTILES;j++){RaceProjectile *m=&a->shot[j];if(m->live&&m->kind==RA_MISSILE&&m->target==id&&length(sub(m->pos,pilot->pos))<6500&&((ra_random(a)>>8)&7)!=0){held=1;break;}}}
   else if(p->item==RA_MISSILE)held=target>=0;
   else if(p->item==RA_MINIGUN)held=target>=0&&dot(pilot->dir,norm(sub(ra_position(g,target),pilot->pos)))>.975f;
  }else {if(a->lock!=target)a->lock_age=0;else a->lock_age+=dt;a->lock=target;}
  int rounds=0,action=ra_use(p,held,available,target>=0||(!id&&r->layout==5&&r->rear_input),dt,&rounds);
  if(action==RA_MISSILE)ra_fire(g,id,action,target);else if(action==RA_MINIGUN)for(int n=0;n<rounds;n++)ra_fire(g,id,action,-1);
  else if(action&&!id)g->cue=action==RA_BOOST?SFX_BOOST:SFX_SCAN;
  if(id&&r->layout!=5&&p->rocket>0)r->pilot[id-1].speed=fminf(r->top_speed*2.5f,r->pilot[id-1].speed+r->top_speed*dt*2);
  if(id&&r->layout!=5&&p->spin>0)r->pilot[id-1].speed=fminf(r->pilot[id-1].speed,r->top_speed*.3f);
  ra_health_sync(g,id);
 }
 ra_pickup_tick(g,previous,dt);ra_projectile_tick(g,previous,dt);
}
