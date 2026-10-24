/* Shared gameplay domain. UI, shortcuts, results and saves call these actions. */
#include "tutorial.h"
#include "erl-local-career.h"
#include "erl-event-catalogue.h"
#include "erl-progress.h"
#include "erl-venues.h"
static const int erl_points[7]={10,7,5,3,2,1,0};
int erl_sponsor_module(const RaceRules *q){return q->sponsor&&!(q->seed&3);}
int erl_sponsor_room(const Game *g){int reserve=g->erl.championship&&g->erl_local.season[g->erl.champ_system].prize_state==1;return g->erl_local.pending_count<(unsigned)(32-reserve);}
DockRoll erl_sponsor_offer(const RaceRules *q){
 static const int bases[]={1,2,6,10,12,24,27,32,38,TECH_BOOST_INJECTORS,TECH_EFFICIENT_BOOSTER,TECH_ADVANCED_HEAT_BUFFER,TECH_EFFICIENT_HYPERDRIVE,TECH_REPAIR_GANTRY};
 DockRoll roll={0};unsigned seed=sector_hash(q->seed^0x5a0f50u);int base=bases[seed%(sizeof(bases)/sizeof(*bases))];if(equip_slot_for(base)<0)base=1;dockbay_roll_generate(&roll,seed,base,DB_MODDED,0);return roll;
}
/* Caller owns its exact-once completion guard and adds the returned cash.
 * Both board and league events queue the same persistent equipment instance. */
int erl_sponsor_award(Game *g){
 RaceArcade *a=&g->race.arcade;if(!a->rules.sponsor||!a->sponsor_done)return 0;
 int units=4000;
 if(erl_sponsor_module(&a->rules)){
  if(erl_sponsor_room(g)){DockRoll roll=erl_sponsor_offer(&a->rules);roll.id=++g->dockbay.serial;g->erl_local.pending[g->erl_local.pending_count++]=roll;}
  else units+=4000;
 }
 g->erl.dirty=1;return units;
}
int erl_hq(const Game *g){return g&&g->docked&&!tutorial_active(g)&&!opening_sol(g)&&station_class(g)==STATION_MEGA;}
const char *erl_organiser(const Game *g,int system){
 static const char *first[]={"JUNO","ADA","ELI","NIA","IVO","MARA","ORIN","KEZ"};
 static const char *last[]={"MERCER","SERRIN","TORR","CALDER","SOL","QUILL","VALE","ARDEN","BELL","QUINN","KEST","AVEN","NORTH","REED","MARLOW","DALE","WREN","SABLE","KEEN","FROST","ASH","LENNOX","PARK","DRAKE","VESPER","CAIRN","HOLT","SHAW","BLAKE","MOSS","STARR","LYNN"};
 static char names[4][32];static int cursor;int hash=(system*173+61)&255;cursor=(cursor+1)&3;(void)g;snprintf(names[cursor],32,"%s %s",first[hash&7],last[hash>>3]);return names[cursor];
}
const char *erl_course_at(const Game *g,int system,int layout){
 static const char *names[4][4]={{"ASTEROID SLALOM","CITADEL RUN","WRECKER'S YARD","CROWN GRAND"},{"ICE NEEDLE","RELAY RUSH","SCRAPSTORM","AURORA GRAND"},{"DUST DEVIL","NEON PASS","IRON RUMBLE","SUNSET GRAND"},{"RIBBON RUN","HARBOUR DASH","QUARRY CLASH","STARFALL GRAND"}};
 (void)g;return names[sector_hash((system+1)*193u)%4][layout&3];
}
const char *erl_course_name(const Game *g,int layout){return erl_course_at(g,g->system,layout);}
static void erl_initialise(Game *g){
 ErlCareer *e=&g->erl;if(e->fleet_n)return;e->fleet_n=1;e->ship_id=2;e->fleet[0].id=1;e->fleet[0].hull=g->ship;e->ref_system=-1;
}
void erl_ship_capture(Game *g,unsigned paint){
 erl_initialise(g);ErlShip *s=&g->erl.fleet[g->erl.active_ship];s->hull=g->ship;s->paint=paint;s->missiles=g->missiles;s->damaged=g->damaged;
 memcpy(s->fit,g->fit,48);memcpy(g->dockbay.fleet[g->erl.active_ship],g->dockbay.fitted,sizeof(g->dockbay.fitted));s->weapon=g->active_weapon;s->fuel=g->fuel;s->health=g->hull;s->shield=g->energy;s->heat=g->heat;
}
int erl_next_qualifier(const Game *g){int level=g->erl.licence;return level<4&&g->erl.xp>=erl_thresholds[level]?level+1:0;}
void erl_season_offer(Game *g){
 erl_local_sync(g);if(!erl_hq(g)||!g->erl.licence)return;ErlLocalSeason *s=&g->erl_local.season[g->system];if(s->active||s->prize_state==1)return;
 dockbay_init(g);unsigned seed=sector_hash(g->system*7919u+(s->number+1)*131u+g->erl.licence*919u);
 static const int bases[]={1,2,6,10,12,24,27,32,38,TECH_BOOST_INJECTORS,TECH_EFFICIENT_BOOSTER,TECH_ADVANCED_HEAT_BUFFER,TECH_EFFICIENT_HYPERDRIVE,TECH_REPAIR_GANTRY};
 int base=bases[seed%(sizeof(bases)/sizeof(*bases))];if(base>=EQUIPMENT_COUNT||equip_slot_for(base)<0)base=bases[seed%9];
 memset(&s->prize,0,sizeof(s->prize));s->prize.id=++g->dockbay.serial;dockbay_roll_generate(&s->prize,seed,base,DB_RARE,0);s->prize_state=1;g->erl.dirty=1;
}
int erl_season_start(Game *g){
 erl_local_sync(g);ErlCareer *e=&g->erl;if(!erl_hq(g)||!e->licence||e->championship||e->event_kind)return 0;
 if(g->erl_local.pending_count>=32)return 0;erl_season_offer(g);erl_initialise(g);e->championship=1;e->champ_system=g->system;e->champ_hub=g->station_variant;e->division=e->licence-1;e->season++;e->round=0;e->season_place=0;
 memset(e->points,0,sizeof(e->points));memset(e->round_wins,0,sizeof(e->round_wins));memset(e->places,0,sizeof(e->places));ErlLocalSeason *local=&g->erl_local.season[g->system];memset(local->results,0,sizeof(local->results));local->ship_id=e->fleet[e->active_ship].id;local->seed=sector_hash(g->system*131u+e->season*919u);erl_local_store(g);e->dirty=1;return 1;
}
RaceRules erl_entry_rules(const Game *g,int kind,int layout,int discipline){
 unsigned seed=sector_hash(g->system*131u+(g->erl.serial+1)*919u);
 RaceRules q=ra_rules_make(discipline>=ERL_RUSH?RA_COMBAT:RA_CLEAN,discipline==ERL_TARGET?RA_TAKEDOWNS:RA_FINISH,discipline>=ERL_RUSH?RA_POOL_ALL:RA_POOL_CLEAN,seed);
 q.laps=discipline==ERL_CIRCUIT?2:1;
 if(kind==ERL_PRACTICE||kind==ERL_TRIAL)q=erl_catalogue_rules(station_profile_hash(g->system,813),g->erl.licence,g->erl_local.selection);
 if(kind==ERL_INTRO){q.field=2;q.sponsor=0;}
 if(kind==ERL_OFFICIAL){q.seed=g->erl_local.season[g->system].seed^(unsigned)layout*7919u;q.context=3;q.field=7;q.sponsor=1+(q.seed%4);if(layout==1){q=ra_rules_make(RA_TIME_TRIAL,RA_MEDALS,1u<<RA_BOOST,q.seed);q.context=3;q.sponsor=1;}else if(layout==3&&g->erl.division>=2){q.objective=RA_ENDURANCE;q.laps=4;q.seconds=360;}}
 if(kind==ERL_PRACTICE||kind==ERL_QUALIFIER)q.sponsor=0;
 if(q.mode==RA_TIME_TRIAL)q.sponsor=0;
 if(q.sponsor==2&&q.mode!=RA_COMBAT)q.sponsor=3;
 return q;
}
int erl_begin(Game *g,int kind,int layout,int discipline,int qualifier){
 erl_local_sync(g);ErlCareer *e=&g->erl;if(!g->docked||g->dead||g->police_stop||e->event_kind||race_job(g)>=0||opening_sol(g))return 0;
 if(kind<ERL_INTRO||kind>ERL_TRIAL||discipline<ERL_SPRINT||discipline>ERL_TARGET)return 0;
 if(kind==ERL_OFFICIAL&&g->erl_local.pending_count>=32){message(g,"Claim a pending module before the next championship race.");return 0;}
 if(kind==ERL_INTRO&&(g->system!=7||e->referral))return 0;
 if(kind!=ERL_INTRO&&!erl_hq(g))return 0;
 if(kind==ERL_QUALIFIER&&(!e->referral||qualifier!=erl_next_qualifier(g)))return 0;
 if(kind==ERL_OFFICIAL&&(!e->championship||e->champ_system!=g->system||e->champ_hub!=g->station_variant||layout!=e->round))return 0;
 if(kind==ERL_OFFICIAL){unsigned registered=g->erl_local.season[g->system].ship_id;if(registered&&registered!=e->fleet[e->active_ship].id)return 0;}
 if((kind==ERL_PRACTICE||kind==ERL_TRIAL)&&!e->licence)return 0;
 ErlCareer before=*e;SpaceRace previous=g->race;RaceRules chosen=g->erl_local.rules_pending?g->erl_local.event_rules:erl_entry_rules(g,kind,layout,discipline);
 erl_initialise(g);e->real_hull=g->hull;e->real_shield=g->energy;e->real_heat=g->heat;e->real_fuel=g->fuel;e->real_damaged=g->damaged;
 e->event_kind=kind;e->event_layout=layout&3;e->event_type=discipline;e->event_system=kind==ERL_OFFICIAL?erl_round_venue(g,layout):g->system;e->event_hub=e->event_system==g->system?g->station_variant:0;e->qualifier=qualifier;
 e->serial++;e->result=0;e->result_units=e->result_xp=e->result_points=e->result_takedowns=0;e->track=1;e->dirty=1;e->resume=0;
 g->erl_local.event_rules=chosen;g->erl_local.rules_pending=0;
 memset(&g->race,0,sizeof(g->race));int venue_ok=erl_prepare_venue(g,e->event_system);if(kind==ERL_OFFICIAL&&g->race.arcade.rules.mode==RA_TIME_TRIAL){ErlLocalSeason *local=&g->erl_local.season[g->system];for(int i=0;i<6;i++)local->trial_times[i]=ra_rival_trial_time(&g->race.arcade.rules,i,e->division);}
 if(!venue_ok||g->race.phase==RACE_NONE){*e=before;g->race=previous;return 0;}return 1;
}
static void erl_referral(Game *g){
 ErlCareer *e=&g->erl;float route[256];uint8_t done[256]={0};for(int i=0;i<256;i++)route[i]=1e9f;route[7]=0;
 /* Dijkstra through refuellable ports, not a fictional station name or a
    straight-line nearest Capitol on the other side of an unreachable gap. */
 int found=-1;float range=fminf(7.f,player_ships[g->ship].range*.1f);
 for(int step=0;step<256;step++){int best=-1;for(int s=0;s<256;s++)if(!done[s]&&(best<0||route[s]<route[best]))best=s;
  if(best<0||route[best]>=1e9f)break;done[best]=1;
  if(best!=7&&station_class_for_system(g,best)==STATION_MEGA){found=best;break;}
  for(int s=0;s<256;s++)if(!done[s]&&world_station_available(g,s,0)){float d=distance_ly(g,best,s);if(d<=range+.001f&&route[best]+d<route[s])route[s]=route[best]+d;}
 }
 e->ref_system=found;e->ref_hub=0;e->referral=1;e->inbox|=1;e->track=1;
}
static int erl_season_position(const Game *g){int place=1;for(int i=1;i<7;i++)if(erl_standing_better(g,i,0))place++;return place;}
static void erl_season_rewards(Game *g){
 ErlCareer *e=&g->erl;e->championship=0;e->season_place=erl_season_position(g);e->inbox|=4;
 /* All classified seasons pay a placing bonus; exclusive prizes stay first. */
 static const int cash[7]={60000,40000,20000,12000,8000,5000,3000},xp[7]={300,220,160,110,80,60,40};
 int index=e->season_place-1;if(index<0)index=6;if(index>6)index=6;
 int bonus=cash[index]*(e->division+1),experience=xp[index]*(e->division+1);
 g->credits+=bonus;e->result_units+=bonus;e->xp+=experience;e->result_xp+=experience;
 if(e->season_place==1){
  e->trophies++;ErlLocalSeason *local=&g->erl_local.season[e->champ_system];if(local->prize_state==1&&g->erl_local.pending_count<32){g->erl_local.pending[g->erl_local.pending_count++]=local->prize;local->prize_state=3;}unsigned bit=1u<<e->division;
  if(!(e->prize_mask&bit)){
   ErlShip *s=&e->fleet[e->fleet_n++];memset(s,0,sizeof(*s));s->id=e->ship_id++;s->hull=erl_prizes[e->division];s->paint=0xfffaf2ebu;s->health=s->shield=100;s->fuel=player_ships[s->hull].range;
   memset(s->fit,FIT_EMPTY,48);s->fit[0]=1;e->prize_mask|=bit;
  }else{g->credits+=30000*(e->division+1);e->result_units+=30000*(e->division+1);}
 }
}
void erl_finish(Game *g,int place,int withdrawn){
 ErlCareer *e=&g->erl;if(!e->event_kind||e->paid==e->serial)return;
 e->paid=e->serial;e->result=withdrawn?2:1;e->result_place=place;e->result_takedowns=g->race.takedowns;
 int official=e->event_kind==ERL_OFFICIAL,eligible=e->event_kind!=ERL_PRACTICE&&!withdrawn;
 int xp=eligible?(place==1?180+e->division*50:fmaxf(20,70-place*7)):0;
 int units=eligible&&place==1?(e->event_kind==ERL_INTRO?5000:e->event_kind==ERL_QUALIFIER?8000:12000*(e->division+1)):0;
 if(eligible)units+=erl_sponsor_award(g);
 e->result_xp=xp;e->result_units=units;e->xp+=xp;g->credits+=units;
 if(eligible){e->takedowns+=g->race.takedowns;e->wins+=place==1;e->podiums+=place<=3;}
 if(e->event_kind==ERL_INTRO&&place==1&&!withdrawn&&!e->referral)erl_referral(g);
 if(e->event_kind==ERL_QUALIFIER&&place==1&&!withdrawn){e->licence=e->qualifier;e->inbox|=2;}
 if(official){
  int order[7]={0,1,2,3,4,5,6};
  for(int i=1;i<7;i++)for(int j=i+1;j<7;j++){
   RaceRival *a=&g->race.pilot[order[i]-1],*b=&g->race.pilot[order[j]-1];
   float ap=race_progress(&g->race,a->gate,a->pos),bp=race_progress(&g->race,b->gate,b->pos);
   int better=g->race.arcade.rules.mode==RA_TIME_TRIAL?g->erl_local.season[e->champ_system].trial_times[order[j]-1]<g->erl_local.season[e->champ_system].trial_times[order[i]-1]:g->race.arcade.rules.objective==RA_TAKEDOWNS?(b->score>a->score||(b->score==a->score&&(b->knockouts<a->knockouts||(b->knockouts==a->knockouts&&b->score_time<a->score_time)))):bp>ap;
   if(better){int t=order[i];order[i]=order[j];order[j]=t;}
  }
  e->result_points=withdrawn?0:g->race.arcade.rules.mode==RA_TIME_TRIAL?ra_medal_points(g->race.elapsed,&g->race.arcade.rules):erl_points[place<1?6:place>7?6:place-1];e->points[0]+=e->result_points;e->round_wins[0]+=place==1&&!withdrawn;e->places[e->round]=withdrawn?8:place;ErlLocalSeason *local=&g->erl_local.season[e->champ_system];local->results[e->round][0]=withdrawn?8:place;
  int pos=1;for(int i=1;i<7;i++){if(pos==place)pos++;int id=order[i];e->points[id]+=g->race.arcade.rules.mode==RA_TIME_TRIAL?ra_medal_points(local->trial_times[id-1],&g->race.arcade.rules):erl_points[pos-1];e->round_wins[id]+=pos==1;local->results[e->round][id]=pos;pos++;}
  if(++e->round==4){erl_season_rewards(g);local->history_count++;local->history_wins+=e->season_place==1;}
  local->history_units+=e->result_units;local->history_xp+=e->result_xp;erl_local_store(g);
 }
 erl_progress_result(g,place,withdrawn);
 g->hull=e->real_hull;g->energy=e->real_shield;g->heat=e->real_heat;g->fuel=e->real_fuel;g->damaged=e->real_damaged;g->dead=0;g->boost=0;
 e->resume=1;e->dirty=1;e->paused=0;g->race.phase=place==1&&!withdrawn?RACE_WON:RACE_LOST;g->race.result_age=0;
}
void erl_withdraw(Game *g){if(!g->erl.event_kind)return;erl_finish(g,7,1);}
int erl_swap(Game *g,int index,unsigned paint,char *reason,int size){
 ErlCareer *e=&g->erl;const char *why=0;
 if(!erl_hq(g))why="Visit an ERL headquarters.";
 else if(e->event_kind)why="Finish or withdraw from the event before swapping.";
 else if(index<0||index>=e->fleet_n)why="No owned ship selected.";
 else if(index==e->active_ship)why="This is your active ship.";
 if(why){snprintf(reason,size,"%s",why);return 0;}
 ErlShip *in=&e->fleet[index];int cap=player_ships[in->hull].capacity;
 for(int b=0;b<8;b++){int item=in->fit[b*6+FIT_HOLD];unsigned ref=g->dockbay.fleet[index][b*6+FIT_HOLD];float power=ref&&ref<=DB_ITEMS?g->dockbay.owned[ref-1].power*.001f:1;cap+=(int)floorf(module_hold_bonus(item)*power+.5f);if(item==TECH_SECURE_VAULT||item==TECH_REFRIGERATED_HOLD)cap+=12;}
 if(cargo_used(g)>cap){snprintf(reason,size,"Clear cargo: %dT will not fit in %dT.",cargo_used(g),cap);return 0;}
 int stored=dockbay_storage_used(g);
 int banks=0;for(int c=0;c<6;c++)banks+=fit_capacity(in->hull,c);int storage=banks*2;if(storage<12)storage=12;if(storage>40)storage=40;
 if(stored>storage){snprintf(reason,size,"Clear Item Storage: %d items, %d places.",stored,storage);return 0;}
 erl_ship_capture(g,paint);e->active_ship=index;g->ship=in->hull;g->missiles=in->missiles;g->damaged=in->damaged;memcpy(g->fit,in->fit,48);memcpy(g->dockbay.fitted,g->dockbay.fleet[index],sizeof(g->dockbay.fitted));g->active_weapon=in->weapon;fit_rebuild(g);
 g->fuel=in->fuel;g->hull=in->health;g->energy=in->shield;g->heat=in->heat;e->dirty=1;snprintf(reason,size,"Previous ship parked safely.");return 1;
}
int erl_claim_module(Game *g,int capacity){
 ErlCareer *e=&g->erl;if(!erl_hq(g))return 0;
 if(g->erl_local.pending_count){if(dockbay_storage_used(g)>=capacity)return 0;int free=db_owned_free(g);if(free<0)return 0;g->dockbay.owned[free]=g->erl_local.pending[0];for(unsigned i=1;i<g->erl_local.pending_count;i++)g->erl_local.pending[i-1]=g->erl_local.pending[i];memset(&g->erl_local.pending[--g->erl_local.pending_count],0,sizeof(DockRoll));e->dirty=1;return 1;}
 if(!e->module_pending)return 0;int total=dockbay_storage_used(g);if(total>=capacity)return 0;
 int bit=0;while(!(e->module_pending&(1u<<bit)))bit++;g->module_store[TECH_ERL_VECTOR_GOVERNOR+(bit&1)]++;e->module_pending&=~(1u<<bit);e->dirty=1;return 1;
}
const char *erl_objective(const Game *g,char *out,int size){
 const ErlCareer *e=&g->erl;
 if(e->event_kind){const RaceRules *q=&g->erl_local.event_rules;const char *goal=q->objective==RA_TAKEDOWNS?"Score the required drone takedowns.":q->objective==RA_MEDALS?"Beat the medal target times.":q->objective==RA_HUNTER?"Take the lead for four continuous seconds.":q->objective==RA_ESCAPE?"Escape without being captured.":q->objective==RA_ELIMINATOR?"Survive elimination, then win the final sprint.":q->course==RA_CORRIDOR?"Follow the course and finish first.":"Pass every checkpoint and finish first.";snprintf(out,size,"Reach the ERL start grid in %s. %s",g->systems[e->event_system].name,goal);}
 else if(!e->referral)snprintf(out,size,"Meet mechanic TORR BELL at Lave Hub. Win his test race for an ERL referral.");
 else if(!e->licence&&e->ref_system>=0)snprintf(out,size,"Visit %s Mega Capital. Ask %s at ERL headquarters for your qualifier.",g->systems[e->ref_system].name,erl_organiser(g,e->ref_system));
 else if(e->championship)snprintf(out,size,"Return to the ERL headquarters in %s for round %d of four.",g->systems[e->champ_system].name,e->round+1);
 else snprintf(out,size,"Visit any Mega Capitol's ERL headquarters. Enter a season or try a higher qualifier.");
 return out;
}

void erl_resume(Game *g){
 ErlCareer *e=&g->erl;
 /* Optional film history is discarded on reload; a persisted earned receipt
  * settles once before presentation ownership is released. */
 if(g->race.layout==5){race_crash_award_flush(g);g->race.crash_time=0;g->race.crash_cooldown=0;}
 /* Accepted contracts are ordinary flight, not a suspended race. Repair old
  * pre-start pause flags before any saved race camera/position is restored. */
 if(!e->result&&!race_is_active(g)){e->paused=e->resume=0;return;}
 if(!e->event_kind||!e->resume)return;
 g->docked=0;g->dock_stage=0;g->planet=g->approach=-1;g->surface=0;g->pos=e->resume_pos;g->yaw=e->resume_yaw;g->pitch=e->resume_pitch;g->roll=e->resume_roll;g->speed=e->resume_speed;g->rng=e->resume_rng;g->boost=0;
 if(g->race.phase==RACE_RUNNING){g->race.crash_time=0;g->race.handover=0;g->race.protection=fmaxf(3,g->race.protection);}
 e->paused=!e->result&&race_is_active(g);
}
