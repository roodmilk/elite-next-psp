/* V36 additive ERL arcade extension. V1-V35 streams remain byte-for-byte
 * readable; no struct padding or transient rendering buffers are serialized. */
static int ra_stream(FILE *f,RaceArcade *a,int reading){
 RaceRules *q=&a->rules;
 if(!erl_word(f,&q->mode,reading))return 0;
 if(!erl_word(f,&q->format,reading))return 0;
 if(!erl_word(f,&q->course,reading))return 0;
 if(!erl_word(f,&q->objective,reading))return 0;
 if(!erl_word(f,&q->pool,reading))return 0;
 if(!erl_word(f,&q->field,reading))return 0;
 if(!erl_word(f,&q->laps,reading))return 0;
 if(!erl_word(f,&q->seconds,reading))return 0;
 if(!erl_word(f,&q->required,reading))return 0;
 if(!erl_word(f,&q->expert,reading))return 0;
 if(!erl_word(f,&q->seed,reading))return 0;
 if(!erl_word(f,&q->sponsor,reading))return 0;
 if(!erl_word(f,&q->context,reading))return 0;
 if(!erl_float(f,&q->gold,reading))return 0;
 if(!erl_float(f,&q->silver,reading))return 0;
 if(!erl_float(f,&q->bronze,reading))return 0;
 for(int i=0;i<(reading&&erl_save_read_version<38?11:RA_ACTORS);i++){RaceItemActor *p=&a->actor[i];
 if(!erl_word(f,&p->health,reading))return 0;
 if(!erl_word(f,&p->item,reading))return 0;
 if(!erl_word(f,&p->pending,reading))return 0;
 if(!erl_word(f,&p->ammo,reading))return 0;
 if(!erl_word(f,&p->require_release,reading))return 0;
 if(!erl_word(f,&p->previous_x,reading))return 0;
 if(!erl_word(f,&p->eliminated,reading))return 0;
 if(!erl_float(f,&p->shuffle,reading))return 0;
 if(!erl_float(f,&p->shield,reading))return 0;
 if(!erl_float(f,&p->rocket,reading))return 0;
 if(!erl_float(f,&p->protection,reading))return 0;
 if(!erl_float(f,&p->spin,reading))return 0;
 if(!erl_float(f,&p->fire_clock,reading))return 0;
 if(!erl_float(f,&p->shield_break,reading))return 0;
 if(!erl_float(f,&p->flash,reading))return 0;
 if(!erl_float(f,&p->capture,reading))return 0;
 if(!erl_float(f,&p->contact,reading))return 0;
 if(!erl_float(f,&p->decision,reading))return 0;
 if(!erl_float(f,&p->respawn,reading))return 0;
 }
 for(int i=0;i<(reading&&erl_save_read_version<38?11:RA_ACTORS);i++)if(!erl_word(f,&a->generation[i],reading))return 0;
 if(!erl_word(f,&a->rng,reading))return 0;
 for(int i=0;i<(reading&&erl_save_read_version<38?11:RA_ACTORS);i++)if(!erl_word(f,&a->branch_boost[i],reading)||!erl_word(f,&a->branch_repair[i],reading))return 0;
 for(int i=0;i<RA_PROJECTILES;i++){RaceProjectile *p=&a->shot[i];
 if(!erl_float(f,&p->pos.x,reading))return 0;
 if(!erl_float(f,&p->pos.y,reading))return 0;
 if(!erl_float(f,&p->pos.z,reading))return 0;
 if(!erl_float(f,&p->dir.x,reading))return 0;
 if(!erl_float(f,&p->dir.y,reading))return 0;
 if(!erl_float(f,&p->dir.z,reading))return 0;
 if(!erl_word(f,&p->kind,reading))return 0;
 if(!erl_word(f,&p->owner,reading))return 0;
 if(!erl_word(f,&p->target,reading))return 0;
 if(!erl_word(f,&p->live,reading))return 0;
 if(!erl_word(f,&p->generation,reading))return 0;
 if(!erl_float(f,&p->age,reading))return 0;
 if(!erl_float(f,&p->life,reading))return 0;
 }
 for(int i=0;i<RA_PICKUPS;i++){RacePickup *p=&a->pickup[i];
 if(!erl_float(f,&p->pos.x,reading))return 0;
 if(!erl_float(f,&p->pos.y,reading))return 0;
 if(!erl_float(f,&p->pos.z,reading))return 0;
 if(!erl_float(f,&p->cooldown,reading))return 0;
 if(!erl_word(f,&p->section,reading))return 0;
 if(!erl_word(f,&p->lane,reading))return 0;
 }
 if(!erl_word(f,&a->ready,reading))return 0;
 if(!erl_word(f,&a->input_x,reading))return 0;
 if(!erl_word(f,&a->lock,reading))return 0;
 if(!erl_word(f,&a->warning,reading))return 0;
 if(!erl_word(f,&a->warning_side,reading))return 0;
 if(!erl_word(f,&a->last_hit,reading))return 0;
 if(!erl_word(f,&a->launch_seen,reading)||!erl_word(f,&a->sponsor_done,reading)||!erl_float(f,&a->sponsor_age,reading))return 0;
 if(!erl_float(f,&a->cue_age,reading))return 0;
 if(!erl_float(f,&a->lock_age,reading))return 0;
 if(!erl_float(f,&a->warning_gap,reading))return 0;
 if(!erl_float(f,&a->entry,reading))return 0;
 if(!erl_float(f,&a->wall_ripple,reading))return 0;
 if(!erl_float(f,&a->launch_age,reading))return 0;
 if(!erl_float(f,&a->wall_impact.x,reading))return 0;
 if(!erl_float(f,&a->wall_impact.y,reading))return 0;
 if(!erl_float(f,&a->wall_impact.z,reading))return 0;
 return 1;
}
static int ra_save_timer(float v,float max){return isfinite(v)&&v>=0&&v<=max;}
static int ra_save_vector(Vec3 v){return isfinite(v.x)&&isfinite(v.y)&&isfinite(v.z)&&fabsf(v.x)<=1.e8f&&fabsf(v.y)<=1.e8f&&fabsf(v.z)<=1.e8f;}
static int ra_save_valid(const RaceArcade *a){
 for(int i=0;i<RA_ACTORS;i++)if(a->branch_boost[i]&a->branch_repair[i])return 0;
 if(a->sponsor_done<0||a->sponsor_done>1||!ra_save_timer(a->sponsor_age,2.01f))return 0;
 if(a->ready<0||a->ready>1)return 0;if(!a->ready)return 1;
 if(!ra_rules_valid(&a->rules,4)||a->input_x<0||a->input_x>1||a->lock< -1||a->lock>=RA_ACTORS||a->warning< -1||a->warning>=RA_PROJECTILES||a->warning_side< -1||a->warning_side>1||a->last_hit<0||a->last_hit>5||a->launch_seen<0||a->launch_seen>1||a->cue_age<0||a->cue_age>5||a->lock_age<0||a->entry<0)return 0;
 for(int i=0;i<RA_ACTORS;i++){const RaceItemActor *p=&a->actor[i];if(p->health<0||p->health>RA_HEALTH||p->item<0||p->item>=RA_ITEM_COUNT||p->pending<0||p->pending>=RA_ITEM_COUNT||p->ammo<0||p->ammo>48||p->require_release<0||p->require_release>1||p->previous_x<0||p->previous_x>1||p->eliminated<0||p->eliminated>1||p->shuffle<0||p->shuffle>.301f||p->shield<0||p->shield>3.001f||p->rocket<0||p->rocket>3.001f||p->protection<0||p->protection>8||p->spin<0||p->spin>1.201f||p->fire_clock<0||p->fire_clock>1.01f)return 0;}
 for(int i=0;i<RA_ACTORS;i++){const RaceItemActor *p=&a->actor[i];const float values[]={p->shuffle,p->shield,p->rocket,p->protection,p->spin,p->fire_clock,p->shield_break,p->flash,p->capture,p->contact,p->decision,p->respawn};for(int k=0;k<12;k++)if(!ra_save_timer(values[k],k==8?4.01f:8.01f))return 0;if(p->ammo&&p->item!=RA_MINIGUN)return 0;}
 const float timers[]={a->cue_age,a->lock_age,a->warning_gap,a->entry,a->wall_ripple,a->launch_age};for(int k=0;k<6;k++)if(!ra_save_timer(timers[k],1.e8f))return 0;if(!ra_save_vector(a->wall_impact))return 0;
 for(int i=0;i<RA_PROJECTILES;i++){const RaceProjectile *p=&a->shot[i];if(p->live<0||p->live>1||p->owner<0||p->owner>=RA_ACTORS||p->target< -1||p->target>=RA_ACTORS||!ra_save_timer(p->age,4.1f)||!ra_save_timer(p->life,4.01f)||!ra_save_vector(p->pos)||!ra_save_vector(p->dir))return 0;if(p->live&&((p->kind!=RA_MISSILE&&p->kind!=RA_MINIGUN)||p->life<=0||fabsf(length(p->dir)-1)>.02f))return 0;}
 for(int i=0;i<RA_PICKUPS;i++){const RacePickup *p=&a->pickup[i];if(p->section<0||p->section>=RACE_GATES||p->lane< -1||p->lane>1||!ra_save_timer(p->cooldown,5.01f)||!ra_save_vector(p->pos))return 0;}
 return 1;
}
