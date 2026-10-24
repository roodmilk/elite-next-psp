static void ra_sponsor_tick(Game *g,int finishing,int place,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;a->sponsor_age=fmaxf(0,a->sponsor_age-dt);if(a->sponsor_done||!a->rules.sponsor)return;
 unsigned rings=a->branch_boost[0];int count=0;while(rings){count+=rings&1;rings>>=1;}
 int done=a->rules.sponsor==1?finishing&&a->actor[0].health>=RA_HEALTH*3/4:a->rules.sponsor==2?r->takedowns>=2:a->rules.sponsor==3?count>=3:finishing&&place==1&&!r->knockouts;
 if(done){a->sponsor_done=1;a->sponsor_age=1.2f;g->cue=SFX_SCAN;}
}
static void ra_complete(Game *g,int place,int failed){
 SpaceRace *r=&g->race;
 if(r->arcade.rules.mode==RA_TIME_TRIAL&&!failed){if(g->erl.event_kind==ERL_OFFICIAL){place=1;for(int i=0;i<6;i++)place+=g->erl_local.season[g->erl.champ_system].trial_times[i]<r->elapsed;}else {int points=ra_medal_points(r->elapsed,&r->arcade.rules);place=points==10?1:points==7?2:points==5?3:4;}}
 ra_sponsor_tick(g,!failed,place,0);r->place=place;r->phase=!failed&&place==1?RACE_WON:RACE_LOST;r->result_age=0;r->boost_time=0;if(r->layout!=5)r->crash_time=0;g->boost=0;
 if(g->erl.event_kind){erl_finish(g,place,failed);return;}
 int job=race_job(g);if(job<0)return;
 /* One attempt. Failure consumes the accepted contract; ordinary board
  * cycling supplies the next offer instead of a free immediate retry. */
 int contract=g->jobs[job].type==MISSION_RACE,won=contract?race_contract_won(g,&g->jobs[job],place,failed):!failed&&place==1;
 if(contract)r->phase=won?RACE_WON:RACE_LOST;
 /* A completed optional challenge is independent of the main finish goal.
  * Withdrawal/expiry cannot farm a challenge; the consumed Job guards repeats. */
 g->erl.dirty=1;
 if(!failed)g->credits+=erl_sponsor_award(g);
 if(won){
  if(!expedition_race_win(g,job))mission_finish_slot(g,job,"Race contract complete. Agreed prize paid.");
 }else {abandon_mission(g,job);message(g,"Race attempt complete. Contract not won.");}
}
static int ra_objectives_tick(Game *g,float dt){
 SpaceRace *r=&g->race;RaceArcade *a=&r->arcade;RaceRules *q=&a->rules;ra_sponsor_tick(g,0,r->place,dt);
 if(q->objective==RA_TAKEDOWNS){
  if(r->takedowns>=(int)q->required){ra_complete(g,1,0);return 1;}
  if(r->elapsed>=q->seconds){ra_complete(g,2,1);return 1;}return 0;
 }
 if(q->objective==RA_HUNTER||q->objective==RA_ESCAPE){
  float own=race_progress(r,r->gate,g->pos),other=race_progress(r,r->pilot[0].gate,r->pilot[0].pos);
  a->actor[0].capture=own>other?fminf(4,a->actor[0].capture+dt):0;
  a->actor[1].capture=other>own?fminf(4,a->actor[1].capture+dt):0;
  if(q->objective==RA_HUNTER&&a->actor[0].capture>=4){ra_complete(g,1,0);return 1;}
  if(q->objective==RA_ESCAPE&&a->actor[1].capture>=4){ra_complete(g,2,1);return 1;}
 }
 if(q->objective==RA_ELIMINATOR){
  if(q->required){for(int i=0;i<r->rivals;i++)if(!a->actor[i+1].eliminated&&r->pilot[i].finished>=0){ra_complete(g,2,0);return 1;}if(r->elapsed>=q->seconds){ra_complete(g,2,1);return 1;}return 0;}
  int count=0;for(int i=0;i<=r->rivals;i++)count+=a->actor[i].eliminated;
  int due=(int)(r->elapsed/20);if(due>r->rivals-1)due=r->rivals-1;
  if(count<due){int last=-1;float progress=1.e9f;
   for(int id=0;id<=r->rivals;id++)if(!a->actor[id].eliminated){float p=race_progress(r,id?r->pilot[id-1].gate:r->gate,ra_position(g,id));if(p<progress){last=id;progress=p;}}
   if(last>=0){a->actor[last].eliminated=1;a->generation[last]++;count++;for(int k=0;k<RA_PROJECTILES;k++)if(a->shot[k].owner==last)a->shot[k].live=0;if(last)r->pilot[last-1].boosting=0;else {ra_complete(g,r->rivals+2-count,0);return 1;}}
  }
  if(count==r->rivals-1){int lead=r->gate;for(int i=0;i<r->rivals;i++)if(!a->actor[i+1].eliminated&&r->pilot[i].gate>lead)lead=r->pilot[i].gate;q->required=lead+3;if(q->required>RACE_GATES)q->required=RACE_GATES;g->cue=SFX_ALERT;}
 }
 if(r->elapsed>=fmaxf(300,q->seconds)){ra_complete(g,r->rivals+1,1);return 1;}return 0;
}
