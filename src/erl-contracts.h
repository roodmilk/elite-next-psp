/* Contract goals are stored in the existing race Job target word. Legacy
 * target=-1 entries retain their advertised first-place goal. Values 40-47
 * remain valid in V6+ Job streams and are never ordinary NPC targets. */
enum {RC_PODIUM=40,RC_RIVAL,RC_TIME,RC_TAKEDOWNS,RC_DRONES,RC_ELIMINATOR,RC_HUNTER,RC_DISTANCE};
/* The final slot has two venue-curated variants without changing Job streams. */
static int race_contract_endurance(const Job *job){return job->target==RC_DISTANCE&&(job->origin&1);}
static int race_contract_target(const Game *g,int hub){
 return RC_PODIUM+sector_hash((g->system+1)*12347u+(hub+1)*811u+social_now(g)/1200)%8;
}
const char *race_contract_goal(const Job *job){
 switch(job->target){
 case RC_PODIUM:return "FINISH IN THE TOP THREE";
 case RC_RIVAL:return "FINISH AHEAD OF RAFE VOSS";
 case RC_TIME:return "FINISH WITHIN 165 SECONDS";
 case RC_TAKEDOWNS:return "FINISH WITH AT LEAST 2 TAKEDOWNS";
 case RC_DRONES:return "DESTROY 5 DRONES WITHIN 2 MINUTES";
 case RC_ELIMINATOR:return "SURVIVE. WIN THE FINAL SPRINT";
 case RC_HUNTER:return "OVERTAKE. LEAD FOR 4 SEC.";
 case RC_DISTANCE:return race_contract_endurance(job)?"WIN FOUR-LAP ENDURANCE":"ESCAPE. AVOID 4-SECOND CAPTURE";
 default:return "FINISH FIRST";
 }
}
RaceRules race_contract_rules(const Game *g,const Job *job,unsigned seed){
 int combat=job->target==RC_TAKEDOWNS||job->target==RC_DRONES||(seed&1);
 unsigned objective=job->target==RC_DRONES?RA_TAKEDOWNS:job->target==RC_ELIMINATOR?RA_ELIMINATOR:job->target==RC_HUNTER?RA_HUNTER:job->target==RC_DISTANCE?race_contract_endurance(job)?RA_ENDURANCE:RA_ESCAPE:RA_FINISH;
 unsigned pool=combat?RA_POOL_ALL:RA_POOL_CLEAN;
 /* Fixed/restricted pools are honest variants of the same event ingredients. */
 if(combat&&(seed&6)==2)pool=1u<<RA_MINIGUN;
 else if(combat&&(seed&6)==4)pool=(1u<<RA_MISSILE)|(1u<<RA_SHIELD);
 else if(!combat&&(seed&6)==2)pool=1u<<RA_BOOST;
 RaceRules q=ra_rules_make(combat?RA_COMBAT:RA_CLEAN,objective,pool,seed);
 if(job->target==RC_TIME){q=ra_rules_make(RA_TIME_TRIAL,RA_MEDALS,1u<<RA_BOOST,seed);q.gold=165;q.silver=190;q.bronze=215;}
 else if(combat&&objective!=RA_TAKEDOWNS&&g->erl.licence>=2&&(seed&24)==24){q.course=RA_RINGS;q.expert=1;}
 q.laps=objective==RA_ENDURANCE||objective==RA_ELIMINATOR?4:2;
 q.seconds=objective==RA_TAKEDOWNS?120:objective==RA_ENDURANCE?360:300;q.context=2;
 return q;
}
int race_contract_won(const Game *g,const Job *job,int place,int failed){
 const SpaceRace *r=&g->race;if(failed)return 0;
 if(job->target==RC_DRONES)return r->takedowns>=5&&r->elapsed<=120;
 if(job->target==RC_HUNTER)return place==1&&r->arcade.actor[0].capture>=4;
 if(job->target==RC_ELIMINATOR)return place==1&&r->arcade.rules.required>0&&r->gate>=(int)r->arcade.rules.required;
 if(r->gate<RACE_GATES)return 0;
 switch(job->target){
 case RC_PODIUM:return place<=3;
 case RC_RIVAL:return r->rivals>0&&(r->pilot[0].finished<0||r->elapsed<r->pilot[0].finished);
 case RC_TIME:return r->elapsed<=165.f;
 case RC_TAKEDOWNS:return r->takedowns>=2;
 default:return place==1;
 }
}
