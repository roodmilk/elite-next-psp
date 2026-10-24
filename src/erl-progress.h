/* Called only after the career serial has passed its exact-once guard. */
static void erl_progress_result(Game *g,int place,int withdrawn){
 ErlCareer *e=&g->erl;ErlProgress *p=&g->erl_local.progress;SpaceRace *r=&g->race;
 if(withdrawn||e->event_kind==ERL_PRACTICE)return;
 p->records++;p->latest_system=g->system;p->latest_mode=r->arcade.rules.mode;p->latest_objective=r->arcade.rules.objective;p->latest_place=place;
 p->latest_units=e->result_units;p->latest_xp=e->result_xp;p->latest_sponsor=r->arcade.sponsor_done;p->latest_time=r->elapsed;
 int completed=r->gate>=RACE_GATES||(r->arcade.rules.objective==RA_TAKEDOWNS&&r->takedowns>=(int)r->arcade.rules.required)||(r->arcade.rules.objective==RA_HUNTER&&place==1)||(r->arcade.rules.objective==RA_ELIMINATOR&&r->arcade.rules.required&&r->gate>=(int)r->arcade.rules.required);
 float *best=&p->best[p->latest_mode][p->latest_objective];if(completed&&r->elapsed>0&&(!*best||r->elapsed<*best))*best=r->elapsed;
 if(r->gate>=RACE_GATES&&(r->arcade.rules.objective==RA_FINISH||r->arcade.rules.objective==RA_MEDALS)){float *course=&p->course_best[g->system][r->layout&3];if(r->elapsed>0&&(!*course||r->elapsed<*course))*course=r->elapsed;}
 int roster=e->event_kind==ERL_OFFICIAL?e->champ_system:g->system;
 if(r->arcade.rules.mode==RA_TIME_TRIAL&&e->event_kind==ERL_OFFICIAL){for(int i=0;i<6;i++)if(r->elapsed<g->erl_local.season[e->champ_system].trial_times[i])p->beaten[roster]|=1u<<i;}
 else if(completed)for(int i=0;i<r->rivals;i++)if(r->pilot[i].finished<0||r->pilot[i].finished>r->elapsed)p->beaten[roster]|=1u<<i;
 p->rivals_beaten=0;int rafe=0;for(int s=0;s<256;s++){rafe|=p->beaten[s]&1;for(int i=1;i<10;i++)p->rivals_beaten+=!!(p->beaten[s]&(1u<<i));}p->rivals_beaten+=!!rafe;
 unsigned bit=1;if(e->wins&&!(p->milestones&bit)){p->milestones|=bit;social_emit(g,SB_ERL_WIN);}
 bit=2;if(e->wins>=10&&!(p->milestones&bit)){p->milestones|=bit;social_emit(g,SB_ERL_MILESTONE);}
 bit=4;if(e->takedowns>=50&&!(p->milestones&bit)){p->milestones|=bit;social_emit(g,SB_ERL_MILESTONE);}
 if(e->event_kind==ERL_QUALIFIER&&place==1){bit=1u<<(7+e->licence);if(!(p->milestones&bit)){p->milestones|=bit;social_emit(g,SB_ERL_LICENCE);}}
 if(e->event_kind==ERL_OFFICIAL&&e->round==4&&e->season_place==1){bit=1u<<(3+e->division);if(!(p->milestones&bit)){p->milestones|=bit;social_emit(g,SB_ERL_TROPHY);}}
}
