#ifndef ERL_EVENT_CATALOGUE_H
#define ERL_EVENT_CATALOGUE_H
/* Curated windows into the ingredient space; generation never emits invalid
 * solo elimination, unarmed takedown or rookie expert ring combat. */
static inline RaceRules erl_catalogue_rules(unsigned seed,int licence,int slot){
 unsigned h=seed+(unsigned)slot*2654435761u;h^=h>>16;h*=2246822519u;h^=h>>13;
 unsigned mode=slot%3,objective=RA_FINISH,pool=mode==RA_COMBAT?RA_POOL_ALL:mode==RA_TIME_TRIAL?1u<<RA_BOOST:RA_POOL_CLEAN;
 if(mode==RA_TIME_TRIAL)objective=RA_MEDALS;
 else if(mode==RA_COMBAT){static const unsigned goals[]={RA_FINISH,RA_TAKEDOWNS,RA_ELIMINATOR,RA_HUNTER,RA_ESCAPE,RA_ENDURANCE};objective=goals[(slot/3)%6];}
 else if(slot>=6){static const unsigned goals[]={RA_FINISH,RA_ELIMINATOR,RA_HUNTER,RA_ESCAPE,RA_ENDURANCE};objective=goals[(slot/3)%5];}
 if(mode==RA_COMBAT&&((h>>8)&3)==0)pool=1u<<RA_MINIGUN;
 else if(mode==RA_COMBAT&&((h>>8)&3)==1)pool=(1u<<RA_MISSILE)|(1u<<RA_SHIELD);
 RaceRules q=ra_rules_make(mode,objective,pool,h);q.laps=objective==RA_ENDURANCE?4:((h>>16)&1)+1;q.seconds=objective==RA_ENDURANCE?360:120;
 if(q.format==RA_PACK)q.field=7+(h>>24)%5;
 /* Leave enough physical track for the timed field reductions. The final
  * two racers receive a nearby sprint finish rather than four forced laps. */
 if(objective==RA_ELIMINATOR){q.laps=4;q.seconds=300;}
 q.gold=120;q.silver=138;q.bronze=156;
 if(licence>=2&&mode==RA_COMBAT&&objective==RA_FINISH&&((h>>20)&7)==0){q.course=RA_RINGS;q.expert=1;}
 q.sponsor=(h>>22)%5;q.context=1;
 if(q.sponsor==2&&mode!=RA_COMBAT)q.sponsor=3; /* Never ask an unarmed racer to score takedowns. */
 if(mode==RA_TIME_TRIAL)q.sponsor=0;
 /* First playtest's direct exhibition: all four items, seven real racers,
  * two laps and broad combat sections. Other curated variants remain data. */
 if(slot==1){q=ra_rules_make(RA_COMBAT,RA_FINISH,RA_POOL_ALL,h);q.field=7;q.laps=2;q.seconds=300;q.context=1;q.sponsor=0;}
 return q;
}
static inline const char *erl_rule_name(const RaceRules *q){
 static const char *goals[]={"CLEAN RACE","TIME TRIAL","DRONE TAKEDOWN","ELIMINATOR","HUNTER","ESCAPE","ENDURANCE"};
 if(q->objective==RA_FINISH)return q->mode==RA_COMBAT?q->expert?"EXPERT RING COMBAT":"COMBAT RACE":"CLEAN RACE";
 return goals[q->objective];
}
#endif
