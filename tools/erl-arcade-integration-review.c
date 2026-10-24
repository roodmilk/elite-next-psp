#include <pspkernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/game.h"
#include "../src/story.h"
#include "../src/erl-event-catalogue.h"
PSP_MODULE_INFO("ERL arcade integration",0,1,0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_MAIN_THREAD_STACK_SIZE_KB(1024);
PSP_HEAP_SIZE_KB(2048);
static Game g,loaded,before;
static int same_roll(const DockRoll *a,const DockRoll *b){return a->id==b->id&&a->base==b->base&&a->quality==b->quality&&a->generation==b->generation&&a->seed==b->seed&&a->power==b->power&&a->secondary==b->secondary&&a->value==b->value;}
static int objective_fixture(int mode,int objective){
 game_init(&g);g.opening.stage=0;g.tutorial_step=0;g.story=STORY_FREE;g.erl.referral=1;g.erl.licence=4;g.docked=1;
 for(int sys=0;sys<256;sys++)if(station_class_for_system(&g,sys)==STATION_MEGA){g.system=sys;system_bodies(&g);game_spawn(&g);break;}
 erl_ship_capture(&g,0x005a41cd);RaceRules q=ra_rules_make(mode,objective,mode==RA_TIME_TRIAL?1u<<RA_BOOST:mode==RA_COMBAT?RA_POOL_ALL:RA_POOL_CLEAN,19);q.seconds=objective==RA_ENDURANCE?360:120;g.erl_local.event_rules=q;g.erl_local.rules_pending=1;
 if(!erl_begin(&g,ERL_TRIAL,0,objective==RA_TAKEDOWNS?ERL_TARGET:ERL_CIRCUIT,0))return 0;
 g.docked=0;race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;g.race.protection=0;return 1;
}
static void board_sponsor_fixture(void){
 game_init(&g);g.opening.stage=0;g.story=STORY_FREE;g.tutorial_step=0;g.erl.referral=1;g.erl.licence=1;g.docked=0;g.job_n=1;
 g.jobs[0]=(Job){.type=MISSION_RACE,.dest=g.system,.origin=g.system,.item=1,.target=40,.reward=6000,.time=300};race_prepare(&g);race_restart(&g);
 g.race.phase=RACE_RUNNING;g.race.countdown=0;g.race.gate=31;g.race.elapsed=150;g.race.arcade.rules.sponsor=1;g.race.arcade.rules.seed=4;g.race.arcade.sponsor_done=1;
}
static void board_sponsor_finish(void){RaceGate *q=&g.race.gates[31];Vec3 previous=add(q->pos,mul(q->normal,-120));g.pos=add(q->pos,mul(q->normal,120));race_tick(&g,previous,.01f);}
int main(void){FILE *f=fopen("erl-arcade-integration-review.txt","w");if(!f)return 1;int failures=0,checks=0;
#define CHECK(c,t) do{int ok=!!(c);checks++;failures+=!ok;fprintf(f,"%s %s\n",ok?"PASS":"FAIL",t);fflush(f);}while(0)
CHECK(load_game(&g,"legacy-v35.sav"),"actual released V35 commander loads into the expanded game");
int credits=g.credits;unsigned portrait=g.commander_portrait;ErlCareer career=g.erl;
CHECK(!g.race.arcade.ready,"legacy save does not invent arcade state");
CHECK(save_game(&g,"migrated-v36.sav")&&load_game(&loaded,"migrated-v36.sav")&&loaded.credits==credits&&loaded.commander_portrait==portrait&&loaded.erl.prize_mask==career.prize_mask&&loaded.erl.fleet_n==career.fleet_n,"V35 to V36 migration preserves credits, commander and earned fleet");
game_init(&g);g.opening.stage=0;g.story=STORY_FREE;g.tutorial_step=0;g.job_n=0;g.credits=1000000;g.docked=1;g.erl.referral=g.erl.licence=1;
for(int s=0;s<256;s++)if(station_class_for_system(&g,s)==STATION_MEGA){g.system=s;system_bodies(&g);game_spawn(&g);break;}
erl_ship_capture(&g,0x005a41cd);
g.erl_local.selection=1;CHECK(erl_begin(&g,ERL_PRACTICE,0,ERL_RUSH,0),"actual ERL entry initializes a combat attempt");
race_restart(&g);g.docked=0;g.race.phase=RACE_RUNNING;g.race.countdown=0;
RaceArcade *a=&g.race.arcade;CHECK(a->ready&&a->rules.mode==RA_COMBAT,"real race creates the shared temporary combat state");
a->actor[0].item=RA_MINIGUN;a->actor[0].ammo=17;a->actor[0].shield=1.25f;a->actor[0].rocket=.75f;
a->shot[0]=(RaceProjectile){.pos=g.pos,.dir={0,0,1},.kind=RA_MISSILE,.owner=1,.target=0,.live=1,.generation=a->generation[0],.age=.5f,.life=3.5f};
g.race.reserve=37;g.erl.resume=1;before=g;
CHECK(save_game(&g,"arcade-live.sav"),"full commander save accepts a live missile and partly spent item");
CHECK(load_game(&loaded,"arcade-live.sav")&&loaded.race.arcade.actor[0].ammo==17&&loaded.race.arcade.actor[0].shield==1.25f&&loaded.race.arcade.actor[0].rocket==.75f&&loaded.race.arcade.shot[0].generation==before.race.arcade.shot[0].generation&&loaded.race.reserve==37,"full load resumes exact item, effect, target-generation and reserve state");
g=before;g.race.arcade.shot[0].live=0;g.race.arcade.shot[0].life=0;
CHECK(save_game(&g,"arcade-expired.sav")&&load_game(&loaded,"arcade-expired.sav"),"an expired projectile does not prevent subsequent atomic saves");
g=before;g.race.arcade.actor[0].ammo=99;
CHECK(!save_game(&g,"arcade-live.sav"),"atomic save validation rejects impossible arcade state");
CHECK(load_game(&loaded,"arcade-live.sav")&&loaded.race.arcade.actor[0].ammo==17,"failed save retains the previous valid commander checkpoint");
/* System-local championship regression: two independent Mega Capitol desks. */
game_init(&g);g.opening.stage=0;g.story=STORY_FREE;g.tutorial_step=0;g.job_n=0;g.docked=1;g.erl.referral=g.erl.licence=1;
int system_a=-1,system_b=-1;for(int sys=0;sys<256;sys++)if(station_class_for_system(&g,sys)==STATION_MEGA){if(system_a<0)system_a=sys;else {system_b=sys;break;}}
g.system=system_a;system_bodies(&g);game_spawn(&g);erl_ship_capture(&g,0x005a41cd);erl_local_sync(&g);
CHECK(system_b>=0&&erl_season_start(&g),"system A creates its own season");
for(int round=0;round<2;round++){CHECK(erl_begin(&g,ERL_OFFICIAL,round,ERL_CIRCUIT,0),"system A round starts");erl_finish(&g,1,0);g.erl.event_kind=0;g.erl.result=0;g.erl.resume=0;g.race.phase=RACE_NONE;}
int points_a=g.erl.points[0];g.system=system_b;system_bodies(&g);game_spawn(&g);g.docked=1;erl_local_sync(&g);
CHECK(!g.erl.championship&&g.erl.round==0&&g.erl.points[0]==0,"system B does not inherit system A's two completed rounds");
CHECK(erl_season_start(&g)&&erl_begin(&g,ERL_OFFICIAL,0,ERL_CIRCUIT,0),"system B starts a separate first round");erl_finish(&g,3,0);g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;
CHECK(g.erl.round==1&&g.erl.points[0]==5,"system B records only its own result");
g.system=system_a;system_bodies(&g);game_spawn(&g);g.docked=1;erl_local_sync(&g);
CHECK(g.erl.championship&&g.erl.round==2&&g.erl.points[0]==points_a,"returning to system A restores its exact two rounds and standings");
CHECK(save_game(&g,"local-seasons.sav")&&load_game(&loaded,"local-seasons.sav")&&loaded.erl.round==2&&loaded.erl_local.season[system_b].round==1&&loaded.erl_local.season[system_b].points[0]==5,"both local championships survive an atomic save and reload");
g=loaded;g.system=system_b;system_bodies(&g);game_spawn(&g);g.docked=1;erl_local_sync(&g);
CHECK(g.erl.championship&&g.erl.round==1&&g.erl.points[0]==5,"reloaded second headquarters resumes only its own progress");
/* Broad walls never damage or break a shield; repair is an actual pipe exit. */
g.erl.event_kind=0;g.race.phase=RACE_NONE;g.erl_local.selection=1;CHECK(erl_begin(&g,ERL_PRACTICE,0,ERL_RUSH,0),"nonseason combat remains available while a local season is pending");g.docked=0;race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;g.race.gate=6;
RaceGate *q=&g.race.gates[6];Vec3 prev=g.race.gates[5].pos,mid=mul(add(prev,q->pos),.5f);g.pos=add(mid,mul(q->right,13000));g.race.arcade.actor[0].shield=2;int health=g.race.arcade.actor[0].health;float shield=g.race.arcade.actor[0].shield;Vec3 before_wall=g.pos;g.speed=4000;race_tick(&g,g.pos,1.f/60);
CHECK(length(sub(g.pos,before_wall))>100&&g.race.arcade.actor[0].health==health&&g.race.arcade.actor[0].shield>shield-.02f,"energy boundary redirects at rocket scale without hull damage or shield break");
g.race.arcade.actor[0].shield=0;g.race.arcade.actor[0].health=600;RaceTrackFrame service=race_repair_frame(&g.race,6);Vec3 pipe=service.centre;Vec3 entrance=add(pipe,mul(service.ahead,-3100));g.pos=add(pipe,mul(service.ahead,3100));race_tick(&g,entrance,1.f/60);
CHECK(g.race.arcade.actor[0].health==RA_HEALTH&&(g.race.arcade.branch_repair[0]&(1u<<6)),"traversing the physical rectangular repair tunnel restores temporary health");
g.race.arcade.branch_repair[0]=0;g.race.arcade.actor[0].decision=0;g.race.arcade.actor[0].health=600;g.pos=add(pipe,mul(q->normal,3100));race_tick(&g,add(pipe,mul(q->normal,2900)),1.f/60);
CHECK(g.race.arcade.actor[0].health==600&&!g.race.arcade.branch_repair[0],"crossing only the exit from the side cannot collect a repair");
Vec3 corner=add(pipe,add(mul(q->right,850),mul(q->up,850)));g.pos=add(corner,mul(q->normal,3100));race_tick(&g,add(corner,mul(q->normal,-3100)),1.f/60);
CHECK(g.race.arcade.actor[0].health==RA_HEALTH&&g.race.arcade.branch_repair[0],"rectangular opening accepts its actual corners rather than obsolete circular geometry");
float reserve=g.race.reserve;g.pos=add(q->booster,mul(q->normal,120));race_tick(&g,add(q->booster,mul(q->normal,-120)),1.f/60);
CHECK(!race_boost_visible(&g.race,6),"repair selection hides its mutually exclusive boost ring in the player's view");
CHECK(g.race.reserve==reserve&&!(g.race.arcade.branch_boost[0]&(1u<<6)),"one pass cannot collect repair and boost from the same section");
g.race.gate=7;q=&g.race.gates[7];g.race.arcade.branch_boost[1]|=1u<<7;
CHECK(race_boost_visible(&g.race,7),"a rival collecting a boost ring cannot remove the player's available ring");
g.race.reserve=20;RaceTrackFrame padf=race_track_frame(&g.race,7,.5f);float padsupport=race_floor_support(&g,0,padf)+35;Vec3 pad=race_pad_point(&g.race,7,.47f,0);g.pos=add(race_pad_point(&g.race,7,.62f,0),mul(padf.up,padsupport));race_tick(&g,add(pad,mul(padf.up,padsupport)),1.f/60);
CHECK(!race_boost_visible(&g.race,7)&&(g.race.arcade.branch_boost[0]&(1u<<7))&&g.race.reserve==20&&g.race.boost_time>0,"actual low pad collection kicks speed without reserve refill and consumes its own passage");
CHECK(save_game(&g,"branches.sav")&&load_game(&loaded,"branches.sav")&&loaded.race.arcade.branch_repair[0]==g.race.arcade.branch_repair[0],"branch choice survives reload without a second reward");
/* Generate every catalogue slot at each licence; build and validate real courses. */
int catalogue_ok=1,geometry_ok=1;for(int licence=1;licence<=4;licence++)for(int slot=0;slot<180;slot++){
 RaceRules rules=erl_catalogue_rules(813,licence,slot);catalogue_ok&=ra_rules_valid(&rules,licence);
 g.erl.licence=licence;g.erl.event_kind=0;g.erl.result=0;g.race.phase=RACE_NONE;g.docked=1;g.erl_local.selection=slot;
 if(!erl_begin(&g,ERL_TRIAL,slot%4,slot%4,0)){geometry_ok=0;continue;}
 Vec3 prev=g.race.start;for(int gate=0;gate<RACE_GATES;gate++){RaceGate *q=&g.race.gates[gate];geometry_ok&=length(sub(q->pos,prev))>100&&length(q->normal)>.99f&&length(q->right)>.99f&&length(q->up)>.99f;prev=q->pos;}
}
CHECK(catalogue_ok,"all 720 licence/catalogue combinations satisfy approved ingredient rules");
CHECK(geometry_ok,"all generated courses have nonzero legs and orthonormal checkpoint frames");
/* Full season receives the exact advertised instance, including secondary stats. */
g.erl.event_kind=0;g.race.phase=RACE_NONE;g.docked=1;g.erl.licence=1;g.erl.championship=0;g.erl.prize_mask=0;g.erl.fleet_n=1;
g.erl_local.season[g.system].active=0;g.erl_local.season[g.system].prize_state=0;g.erl_local.pending_count=0;
CHECK(erl_season_start(&g),"new championship advertises its permanent enhanced equipment offer");
DockRoll offered=g.erl_local.season[g.system].prize;int exact=offered.id!=0;
for(int round=0;round<4;round++){
 exact&=erl_begin(&g,ERL_OFFICIAL,round,ERL_CIRCUIT,0);g.race.elapsed=90;erl_finish(&g,1,0);g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;
}
fprintf(f,"INFO offer %u %u %u %u %u %u %u %u pending %u %u %u %u %u %u %u %u exact=%d place=%d\n",offered.id,offered.base,offered.quality,offered.generation,offered.seed,offered.power,offered.secondary,offered.value,g.erl_local.pending[0].id,g.erl_local.pending[0].base,g.erl_local.pending[0].quality,g.erl_local.pending[0].generation,g.erl_local.pending[0].seed,g.erl_local.pending[0].power,g.erl_local.pending[0].secondary,g.erl_local.pending[0].value,exact,g.erl.season_place);
CHECK(exact&&g.erl.season_place==1&&g.erl_local.pending_count==1&&same_roll(&offered,&g.erl_local.pending[0]),"championship awards exactly the pre-entry enhanced offer without rerolling");
CHECK(g.erl.fleet_n==2&&g.erl.fleet[1].hull==22&&!strcmp(player_ships[22].name,"NEEDLEFISH")&&mesh_id("NEEDLEFISH")!=mesh_id("ADDER"),"first new championship awards the authored playable Needlefish hull");
CHECK(!erl_claim_module(&g,0)&&g.erl_local.pending_count==1,"full storage keeps exact module prize pending");
CHECK(save_game(&g,"pending-prize.sav")&&load_game(&loaded,"pending-prize.sav")&&same_roll(&offered,&loaded.erl_local.pending[0]),"unclaimed prize persists exact identity and both performance values");
g=loaded;int balance=g.credits;CHECK(erl_claim_module(&g,40)&&!g.erl_local.pending_count,"claim transfers the exact prize into real Item Storage");
int found=0;for(int i=0;i<DB_ITEMS;i++)found+=same_roll(&offered,&g.dockbay.owned[i]);
CHECK(found==1&&!erl_claim_module(&g,40)&&g.credits==balance,"module claim is exactly once and never charges units");
/* Every placing receives consolation, without the exclusive champion prize. */
int placings_ok=1;for(int place=2;place<=7;place++){
 g=loaded;g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;g.docked=1;g.erl.championship=0;g.erl_local.season[g.system].active=0;
 erl_season_start(&g);unsigned trophies=g.erl.trophies,prizes=g.erl_local.pending_count;int fleet=g.erl.fleet_n,xp=g.erl.xp,cash=g.credits;
 for(int round=0;round<4;round++){erl_begin(&g,ERL_OFFICIAL,round,ERL_CIRCUIT,0);g.race.elapsed=220;erl_finish(&g,place,0);g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;}
 placings_ok&=!g.erl.championship&&g.erl.xp>xp&&g.credits>cash&&g.erl.trophies==trophies&&g.erl.fleet_n==fleet&&g.erl_local.pending_count==prizes;
}
CHECK(placings_ok,"every nonchampion placing earns units and XP without champion-only prizes");
g=loaded;g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;g.docked=1;g.erl.championship=0;g.erl_local.season[g.system].active=0;erl_season_start(&g);
g.erl.active_ship=1;CHECK(!erl_begin(&g,ERL_OFFICIAL,0,ERL_CIRCUIT,0),"official entry refuses a ship other than the season's registered ship");g.erl.active_ship=0;
erl_begin(&g,ERL_PRACTICE,0,ERL_CIRCUIT,0);g.docked=0;race_restart(&g);Vec3 grid=g.pos;game_tick(&g,2,0,0,0,0);
CHECK(g.race.phase==RACE_COUNTDOWN&&g.race.countdown>4&&g.race.elapsed==0&&length(sub(g.pos,grid))<.01f,"pregrid introduction freezes actual poses and the race clock");
g.race.countdown=.05f;g.race.arcade.entry=0;game_tick(&g,.1f,0,0,1,0);CHECK(g.race.phase==RACE_RUNNING&&g.race.elapsed==0&&g.speed>0,"fresh throttle during final launch window grants a real perfect launch");
race_restart(&g);game_tick(&g,.05f,0,0,1,0);g.race.countdown=.02f;game_tick(&g,.03f,0,0,1,0);CHECK(g.race.phase==RACE_RUNNING&&g.speed==0,"holding throttle through countdown cannot farm perfect launches");
/* First playtest: complete the real two-lap combat circuit with steering only. */
g=loaded;g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;g.docked=1;g.erl.licence=1;g.erl.active_ship=0;g.erl_local.selection=1;
Vec3 accepted_pos=g.pos;
CHECK(erl_begin(&g,ERL_TRIAL,0,ERL_RUSH,0)&&g.docked&&length(sub(g.pos,accepted_pos))<.01f,"combat acceptance keeps the player at the station until actual departure");
CHECK(g.race.arcade.rules.mode==RA_COMBAT&&g.race.arcade.rules.field==7&&g.race.arcade.rules.laps==2&&g.race.arcade.rules.pool==RA_POOL_ALL,"first arcade event is a seven-racer two-lap combat circuit with all items");
g.docked=0;race_restart(&g);int frames=0,uses=0;float travelled=0;
for(;frames<12000&&g.race.phase<RACE_LOST;frames++){
 RaceGate *gate=&g.race.gates[g.race.gate];Vec3 desired=norm(sub(add(gate->pos,mul(gate->normal,300)),g.pos));
 float sy=sinf(g.yaw),cy=cosf(g.yaw),sp=sinf(g.pitch),cp=cosf(g.pitch);
 float turn=fmaxf(-1,fminf(1,dot(desired,(Vec3){cy,0,-sy})*6));
 float pitch=fmaxf(-1,fminf(1,dot(desired,(Vec3){-sy*sp,cp,-cy*sp})*6));
 Vec3 old=g.pos;g.boost=0;int fire=g.race.arcade.actor[0].item!=RA_EMPTY&&frames%2==0;uses+=fire;
 race_item_input(&g,fire);game_tick(&g,.05f,turn,pitch,1,0);travelled+=length(sub(g.pos,old));
}
fprintf(f,"INFO live combat frames=%d elapsed=%.2f phase=%d lap=%d gate=%d place=%d travel=%.0f item_inputs=%d\n",frames,g.race.elapsed,g.race.phase,g.race.laps,g.race.gate,g.race.place,travelled,uses);fflush(f);
CHECK(g.race.phase>=RACE_LOST&&g.race.elapsed>60&&g.race.elapsed<500&&travelled>100000,"real combat circuit finishes through actual steering and ordered checkpoints without teleporting");
CHECK(save_game(&g,"combat-finished.sav")&&load_game(&before,"combat-finished.sav")&&before.erl.result==g.erl.result,"completed combat result survives save and reload");
/* Contract predicates must test the advertised goal, never background rank. */
game_init(&g);g.opening.stage=0;g.story=STORY_FREE;g.tutorial_step=0;g.erl.event_kind=0;g.erl.referral=1;g.job_n=1;
g.jobs[0]=(Job){.type=MISSION_RACE,.dest=g.system,.origin=g.system,.item=1,.target=40,.reward=6000,.time=300};g.race.phase=RACE_NONE;race_prepare(&g);
CHECK(g.race.arcade.rules.objective==RA_FINISH&&g.race.rivals==6&&g.race.laps==2,"board contracts build a full seven-racer course rather than incompatible solo objectives");
g.race.gate=RACE_GATES;g.race.elapsed=160;
CHECK(race_contract_won(&g,&g.jobs[0],3,0)&&!race_contract_won(&g,&g.jobs[0],4,0),"top-three contract accepts third and rejects fourth");
g.jobs[0].target=41;g.race.pilot[0].finished=161;
CHECK(race_contract_won(&g,&g.jobs[0],5,0),"named-rival contract accepts fifth when Rafe finishes behind");g.race.pilot[0].finished=159;
CHECK(!race_contract_won(&g,&g.jobs[0],1,0),"named-rival predicate rejects a result behind Rafe regardless of supplied rank");
g.jobs[0].target=42;g.race.elapsed=165;int at_limit=race_contract_won(&g,&g.jobs[0],7,0);g.race.elapsed=165.01f;
CHECK(at_limit&&!race_contract_won(&g,&g.jobs[0],1,0),"timed contract uses exact inclusive 165-second finish boundary");
g.jobs[0].target=43;g.race.takedowns=2;
CHECK(race_contract_won(&g,&g.jobs[0],7,0)&&!race_contract_won(&g,&g.jobs[0],1,1),"finish-and-takedown contract accepts sufficient hits but never a withdrawal");
g.race.gate=31;CHECK(!race_contract_won(&g,&g.jobs[0],1,0),"takedowns alone cannot pay a contract before all checkpoints");
g.jobs[0].target=-1;g.race.gate=RACE_GATES;CHECK(race_contract_won(&g,&g.jobs[0],1,0)&&!race_contract_won(&g,&g.jobs[0],2,0),"legacy contracts keep their first-place promise without hidden top-three payout");
g.jobs[0].target=43;g.race.phase=RACE_NONE;race_prepare(&g);
CHECK(g.race.arcade.rules.mode==RA_COMBAT&&save_game(&g,"board-goal.sav")&&load_game(&loaded,"board-goal.sav")&&loaded.jobs[0].target==43,"combat takedown contract identity persists in a real commander save");
/* Board checkpoints resume the actual one-attempt race, never a fresh grid. */
race_restart(&g);g.docked=0;g.race.phase=RACE_RUNNING;g.race.countdown=0;g.race.elapsed=37.25f;g.race.gate=3;g.race.reserve=28;g.race.arcade.actor[0].item=RA_MINIGUN;g.race.arcade.actor[0].ammo=17;g.pos=g.race.gates[2].pos;Vec3 board_position=g.pos;unsigned board_rng=g.rng;
int board_saved=save_game(&g,"board-running.sav"),board_loaded=load_game(&loaded,"board-running.sav");fprintf(f,"INFO board save=%d load=%d source_top=%.2f source_phase=%d source_system=%d system=%d loaded_phase=%d job_n=%d elapsed=%.2f gate=%d reserve=%.2f ammo=%d paused=%d\n",board_saved,board_loaded,g.race.top_speed,g.race.phase,g.race.system,g.system,loaded.race.phase,loaded.job_n,loaded.race.elapsed,loaded.race.gate,loaded.race.reserve,loaded.race.arcade.actor[0].ammo,loaded.erl.paused);fflush(f);
CHECK(board_saved&&board_loaded&&loaded.job_n==1&&loaded.race.phase==RACE_RUNNING&&loaded.race.elapsed==37.25f&&loaded.race.gate==3&&loaded.race.reserve==28&&loaded.race.arcade.actor[0].ammo==17,"board reload preserves elapsed progress, reserve and partly spent ammunition instead of granting a retry");
CHECK(!loaded.docked&&loaded.erl.paused&&length(sub(loaded.pos,board_position))<.01f&&loaded.rng==board_rng&&!loaded.erl.event_kind,"board reload restores its exact pose and RNG into an operable paused race");
g=loaded;g.race.rivals=50;CHECK(!save_game(&g,"board-running.sav")&&load_game(&loaded,"board-running.sav")&&loaded.race.rivals<=10,"invalid board geometry cannot overwrite its previous valid checkpoint");
g=loaded;int board_balance=g.credits;race_withdraw(&g);
CHECK(!g.job_n&&g.race.phase==RACE_LOST&&g.credits==board_balance+100,"board withdrawal fails its one attempt and returns only the normal 10-unit deposit, never the prize");
/* Every board variant uses the same validated ingredients as league events. */
int board_valid=1,board_formats=0,board_expert=0;
for(int licence=1;licence<=3;licence++)for(int target=40;target<=47;target++)for(unsigned seed=0;seed<32;seed++)for(int origin=0;origin<2;origin++){
 g.erl.licence=licence;Job entry={.target=target,.origin=origin};RaceRules q=race_contract_rules(&g,&entry,seed);
 board_valid&=ra_rules_valid(&q,licence)&&q.context==2;board_formats|=1u<<q.objective;board_expert|=q.expert;
 if(licence<2&&q.expert)board_valid=0;
 if(target==44&&(q.pool&((1u<<RA_MISSILE)|(1u<<RA_MINIGUN)))==0)board_valid=0;
}
CHECK(board_valid&&board_formats==127&&board_expert,"1536 board variants cover every objective with legal formats, weapons and expert eligibility");
game_init(&g);g.opening.stage=0;g.story=STORY_FREE;g.tutorial_step=0;g.erl.referral=1;g.job_n=1;g.docked=0;
g.jobs[0]=(Job){.type=MISSION_RACE,.dest=g.system,.origin=g.system,.item=1,.target=44,.reward=6300,.time=300};race_prepare(&g);race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;
CHECK(g.race.arcade.rules.objective==RA_TAKEDOWNS&&g.race.arcade.rules.field==1&&g.race.rivals==6&&g.race.pilot[0].mesh==mesh_id("MONITOR")&&dot(sub(g.race.pilot[0].pos,g.race.start),g.race.dir)>0,"board drone trial spawns forward moving targets through the actual grid path");
int drone_cash=g.credits;g.race.takedowns=4;race_tick(&g,g.pos,0);CHECK(g.job_n==1&&g.credits==drone_cash,"four drones cannot pay the five-drone board offer");g.race.takedowns=5;race_tick(&g,g.pos,0);
CHECK(!g.job_n&&g.credits==drone_cash+6300&&g.race.phase==RACE_WON,"fifth drone pays the exact contract once without demanding a lap finish");race_tick(&g,g.pos,0);CHECK(g.credits==drone_cash+6300,"board drone completion cannot repeat its payment");
for(int target=45;target<=47;target++){
 game_init(&g);g.opening.stage=0;g.story=STORY_FREE;g.tutorial_step=0;g.erl.referral=1;g.job_n=1;g.docked=0;
 g.jobs[0]=(Job){.type=MISSION_RACE,.dest=g.system,.origin=0,.item=1,.target=target,.reward=6400,.time=300};race_prepare(&g);race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;
 if(target==45){g.race.arcade.rules.required=3;g.race.gate=2;}
 else if(target==46){g.race.gate=1;g.pos=g.race.gates[0].pos;g.race.pilot[0].gate=0;g.race.pilot[0].pos=g.race.start;g.race.pilot[0].speed=g.race.pilot[0].pace=0;g.race.arcade.actor[0].capture=3.95f;}
 else g.race.gate=31;
 int contract_cash=g.credits;
 if(target==46)race_tick(&g,g.pos,.1f);
 else {RaceGate *end=&g.race.gates[g.race.gate];Vec3 start=add(end->pos,mul(end->normal,-120));g.pos=add(end->pos,mul(end->normal,120));race_tick(&g,start,.01f);}
 CHECK(!g.job_n&&g.credits==contract_cash+6400&&g.race.phase==RACE_WON,"actual eliminator, hunter or escape completion pays its promised board reward");
}
g.jobs[0]=(Job){.target=47,.origin=1};g.race.gate=32;
CHECK(race_contract_rules(&g,&g.jobs[0],0).objective==RA_ENDURANCE&&strstr(race_contract_goal(&g.jobs[0]),"ENDURANCE")&&race_contract_won(&g,&g.jobs[0],1,0)&&!race_contract_won(&g,&g.jobs[0],2,0),"endurance contract has a distinct saved venue identity and first-place goal");
/* Elimination is authoritative simulation, including its saved finish gate. */
g=before;g.erl.event_kind=ERL_PRACTICE;g.erl.result=0;g.erl_local.event_rules=ra_rules_make(RA_COMBAT,RA_ELIMINATOR,RA_POOL_ALL,19);g.erl_local.event_rules.laps=4;g.erl_local.event_rules.seconds=300;g.race.phase=RACE_NONE;race_prepare(&g);g.docked=0;race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;g.race.elapsed=100;g.race.gate=12;g.pos=g.race.gates[11].pos;
for(int i=0;i<g.race.rivals;i++){g.race.pilot[i].gate=10;g.race.pilot[i].pos=g.race.gates[9].pos;g.race.arcade.actor[i+1].eliminated=i<4;}
g.race.arcade.shot[0]=(RaceProjectile){.live=1,.owner=6,.kind=RA_MISSILE,.life=3,.dir={0,0,1},.pos=g.pos,.target=0,.generation=g.race.arcade.generation[0]};
race_tick(&g,g.pos,0);int remaining=0;for(int i=0;i<=g.race.rivals;i++)remaining+=!g.race.arcade.actor[i].eliminated;
CHECK(remaining==2&&g.race.arcade.rules.required==15,"timed eliminator leaves two racers and creates a three-gate final sprint");
int eliminated_owner=g.race.arcade.actor[6].eliminated?6:5;g.race.arcade.shot[1]=(RaceProjectile){.live=1,.owner=eliminated_owner,.kind=RA_MINIGUN,.life=.5f,.dir={0,0,1}};race_tick(&g,g.pos,0);
CHECK(!g.race.arcade.shot[1].live,"eliminated racers cannot keep attacking with existing projectiles");
g.erl.resume=1;CHECK(save_game(&g,"eliminator-final.sav")&&load_game(&loaded,"eliminator-final.sav")&&loaded.race.arcade.rules.required==15,"final sprint finish survives checkpoint reload without restarting elimination");
/* Exact payments run through the real finish crossing, not just a predicate. */
game_init(&g);g.opening.stage=0;g.tutorial_step=0;g.story=STORY_FREE;g.erl.referral=1;g.job_n=1;g.docked=0;
g.jobs[0]=(Job){.type=MISSION_RACE,.dest=g.system,.origin=g.system,.item=1,.target=40,.reward=6000,.time=300};race_prepare(&g);race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;g.race.elapsed=130;g.race.gate=31;g.race.pilot[0].gate=g.race.pilot[1].gate=32;g.race.pilot[0].finished=100;g.race.pilot[1].finished=101;
RaceGate *last=&g.race.gates[31];Vec3 from=add(last->pos,mul(last->normal,-120));g.pos=add(last->pos,mul(last->normal,120));int old_credits=g.credits;race_tick(&g,from,.01f);
CHECK(g.job_n==0&&g.race.place==3&&g.race.phase==RACE_WON&&g.credits==old_credits+6000,"actual third-place crossing fulfils a podium contract and pays exactly its displayed reward");old_credits=g.credits;race_tick(&g,from,.01f);CHECK(g.credits==old_credits,"completed board contract cannot pay twice on a later frame");
game_init(&g);g.opening.stage=0;g.tutorial_step=0;g.story=STORY_FREE;g.erl.referral=1;g.docked=1;g.credits=10000;
CHECK(accept_mission(&g,5),"a referred commander can accept the current race contract");race_restart(&g);uint32_t refresh=g.erl_local.progress.board_until[g.system][g.race.hub];abandon_mission(&g,0);g.race.phase=RACE_NONE;
CHECK(refresh&&!accept_mission(&g,5),"starting an attempt prevents immediate reacceptance after abandoning it");
CHECK(save_game(&g,"board-refresh.sav")&&load_game(&loaded,"board-refresh.sav")&&loaded.erl_local.progress.board_until[g.system][0]==refresh&&!accept_mission(&loaded,5),"board refresh restriction persists through save and reload");
/* Career records and sponsor instances share the existing payment serial. */
game_init(&g);g.opening.stage=0;g.tutorial_step=0;g.story=STORY_FREE;g.erl.referral=g.erl.licence=1;g.docked=1;
for(int sys=0;sys<256;sys++)if(station_class_for_system(&g,sys)==STATION_MEGA){g.system=sys;system_bodies(&g);game_spawn(&g);break;}
erl_ship_capture(&g,0x005a41cd);g.erl_local.selection=1;CHECK(erl_begin(&g,ERL_TRIAL,0,ERL_CIRCUIT,0),"paid exhibition prepares a career-record attempt");g.race.gate=32;g.race.elapsed=140;g.race.arcade.rules.sponsor=1;g.race.arcade.rules.seed=4;g.race.arcade.sponsor_done=1;DockRoll sponsor=erl_sponsor_offer(&g.race.arcade.rules);old_credits=g.credits;erl_finish(&g,1,0);
CHECK(g.erl_local.progress.records==1&&g.erl_local.progress.best[RA_COMBAT][RA_FINISH]==140&&g.erl_local.progress.latest_units==(unsigned)(g.credits-old_credits),"career stores a completed best time and exact awarded units");
CHECK(g.erl_local.pending_count==1&&g.erl_local.pending[0].base==sponsor.base&&g.erl_local.pending[0].power==sponsor.power&&g.erl_local.pending[0].secondary==sponsor.secondary,"sponsor grants the exact deterministic enhanced module shown by its offer");
unsigned posts=g.social.count;old_credits=g.credits;erl_finish(&g,1,0);
CHECK(g.erl_local.progress.records==1&&g.erl_local.pending_count==1&&g.social.count==posts&&g.credits==old_credits,"repeated finish cannot duplicate career records, public milestones, sponsor equipment or cash");
CHECK(g.erl_local.progress.milestones&1,"first victory grants a persistent career title milestone");
int career_saved=save_game(&g,"career-record.sav"),career_loaded=load_game(&loaded,"career-record.sav");
CHECK(career_saved&&career_loaded,"V37 career checkpoint writes and loads successfully");
CHECK(loaded.erl_local.progress.best[RA_COMBAT][RA_FINISH]==140,"V37 preserves the exact earned best time");
CHECK(loaded.erl_local.progress.milestones==g.erl_local.progress.milestones&&loaded.erl_local.progress.latest_units==g.erl_local.progress.latest_units,"V37 preserves earned titles and factual broadcast rewards");
CHECK(!memcmp(loaded.erl_local.progress.beaten,g.erl_local.progress.beaten,sizeof(g.erl_local.progress.beaten)),"V37 preserves every system's defeated-rival identities");
g=loaded;g.erl_local.progress.best[0][0]=NAN;CHECK(!save_game(&g,"career-record.sav")&&load_game(&loaded,"career-record.sav")&&loaded.erl_local.progress.best[RA_COMBAT][RA_FINISH]==140,"invalid career data cannot replace the previous valid commander save");
/* Queue capacity includes the championship's already-promised module. */
g=loaded;g.erl.championship=1;g.erl.champ_system=g.system;g.erl_local.season[g.system].prize_state=1;g.erl_local.pending_count=30;
CHECK(erl_sponsor_room(&g),"sponsor can use the penultimate free reward slot");g.erl_local.pending_count=31;
CHECK(!erl_sponsor_room(&g),"sponsor cannot consume the slot reserved for the championship prize");g.erl.championship=0;
CHECK(erl_sponsor_room(&g),"non-championship sponsor may use the final free reward slot");g.erl_local.pending_count=32;
CHECK(!erl_sponsor_room(&g),"full pending queue uses cash instead of losing equipment");
/* Construct an exact V36 payload by removing only the appended V37 suffix. */
g=loaded;g.docked=1;g.erl.event_kind=g.erl.result=g.erl.resume=0;g.race.phase=RACE_NONE;g.erl.wins=12;g.erl.takedowns=51;g.erl.licence=2;g.erl.prize_mask=1;g.erl.fleet_n=2;g.erl.fleet[1]=g.erl.fleet[0];g.erl.fleet[1].id=g.erl.ship_id++;g.erl_local.progress.milestones=0;
int legacy_ok=save_game(&g,"v36-source.sav");fprintf(f,"INFO V36 source saved=%d message=%s pending=%u best=%f\n",legacy_ok,g.message,g.erl_local.pending_count,g.erl_local.progress.best[0][0]);FILE *source=fopen("v36-source.sav","rb");unsigned char *bytes=0;long payload=0;
if(source){fseek(source,0,SEEK_END);long n=ftell(source);payload=n-4-ERL_PROGRESS_BYTES;rewind(source);bytes=malloc(n);legacy_ok=legacy_ok&&bytes&&fread(bytes,1,n,source)==(size_t)n;fclose(source);}else legacy_ok=0;
if(legacy_ok){bytes[4]=36;bytes[5]=bytes[6]=bytes[7]=0;uint32_t crc=0xffffffffu;for(long i=0;i<payload;i++){crc^=bytes[i];for(int b=0;b<8;b++)crc=(crc>>1)^((crc&1)?0xedb88320u:0);}crc^=0xffffffffu;for(int b=0;b<4;b++)bytes[payload+b]=(unsigned char)(crc>>(b*8));FILE *old=fopen("legacy-v36.sav","wb");if(old){legacy_ok=fwrite(bytes,1,payload+4,old)==(size_t)payload+4;fclose(old);}else legacy_ok=0;}free(bytes);
CHECK(legacy_ok&&load_game(&loaded,"legacy-v36.sav")&&loaded.credits==g.credits&&loaded.erl.wins==12&&loaded.erl.fleet_n==g.erl.fleet_n,"exact V36 payload migrates without changing credits or earned fleet");
CHECK((loaded.erl_local.progress.milestones&783)==783&&!loaded.erl_local.progress.records&&!loaded.erl_local.progress.best[RA_COMBAT][RA_FINISH],"legacy career seeds earned milestone flags without fabricated race records");
/* A home-owned season can stage a real course in a neighbouring system. */
game_init(&g);g.opening.stage=0;g.tutorial_step=0;g.story=STORY_FREE;g.erl.referral=1;g.erl.licence=2;g.docked=1;
for(int sys=0;sys<256;sys++)if(station_class_for_system(&g,sys)==STATION_MEGA){g.system=sys;system_bodies(&g);game_spawn(&g);if(erl_round_venue(&g,2)!=sys)break;}
erl_ship_capture(&g,0x005a41cd);int home=g.system,venue=erl_round_venue(&g,2);Vec3 station_pose=g.pos;
CHECK(venue!=home&&world_station_available(&g,venue,0)&&distance_ly(&g,home,venue)<=fminf(7,player_ships[g.ship].range*.1f)+.001f,"later championship venue is a real refuellable port within a reachable jump");
CHECK(erl_season_start(&g),"later league season can be registered at its home capital");g.erl.round=2;g.erl_local.season[home].round=2;
CHECK(erl_begin(&g,ERL_OFFICIAL,2,ERL_RUSH,0)&&g.system==home&&g.docked&&length(sub(g.pos,station_pose))<.01f&&g.erl.event_system==venue&&g.race.system==venue,"accepting an away round stages its real course without moving the commander");
SpaceRace away_course=g.race;race_prepare(&g);race_restart(&g);
CHECK(g.race.phase==RACE_GRID&&!race_is_active(&g)&&g.race.seed==away_course.seed&&g.system==home,"READY cannot start a foreign course and transit cannot rebuild it locally");
CHECK(save_game(&g,"away-pending.sav")&&load_game(&loaded,"away-pending.sav")&&loaded.system==home&&loaded.erl.event_system==venue&&loaded.race.phase==RACE_GRID&&!loaded.erl.paused&&!loaded.erl.resume,"away selection survives a home save without a false resume warning");
g=loaded;g.system=venue;system_bodies(&g);game_spawn(&g);g.docked=0;race_prepare(&g);
CHECK(g.race.seed==away_course.seed&&length(sub(g.race.start,away_course.start))<.01f&&race_course_clear(&g,&g.race),"arrival uses the same accepted course with valid local scenery clearance");
race_restart(&g);CHECK(g.race.phase==RACE_COUNTDOWN&&race_is_active(&g),"actual READY at the destination starts the away grid");g.race.elapsed=145;g.race.gate=32;erl_finish(&g,1,0);
CHECK(g.erl.round==3&&g.erl.champ_system==home&&g.erl_local.season[home].round==3&&!g.erl_local.season[venue].started&&g.erl_local.season[home].points[0]>0,"away finish advances only its home championship, never the venue's season");
/* Live objective acceptance, with actual tick ordering and completion actions. */
CHECK(objective_fixture(RA_CLEAN,RA_HUNTER),"clean hunter starts with its real two-ship head-start format");g.race.gate=1;g.pos=g.race.gates[0].pos;g.race.pilot[0].pace=g.race.pilot[0].speed=0;
for(int i=0;i<20;i++)race_tick(&g,g.pos,.1f);CHECK(g.race.phase==RACE_RUNNING&&g.race.arcade.actor[0].capture>1.9f,"hunter does not finish after only two seconds in the lead");
g.race.pilot[0].gate=2;g.race.pilot[0].pos=g.race.gates[1].pos;race_tick(&g,g.pos,.1f);CHECK(g.race.arcade.actor[0].capture==0,"losing the lead resets the continuous hunter capture");g.race.pilot[0].gate=0;g.race.pilot[0].pos=g.race.start;
for(int i=0;i<41&&g.race.phase==RACE_RUNNING;i++)race_tick(&g,g.pos,.1f);CHECK(g.race.phase==RACE_WON&&g.erl.result_place==1,"four continuous seconds produces an actual hunter victory");
CHECK(objective_fixture(RA_COMBAT,RA_HUNTER),"armed hunter starts through the same rules path");g.race.pilot[0].disabled=1;g.race.arcade.actor[1].health=0;race_tick(&g,g.pos,.05f);CHECK(g.race.phase==RACE_RUNNING,"destroying the hunter target does not automatically win the pursuit");
CHECK(objective_fixture(RA_CLEAN,RA_ESCAPE),"escape starts with its real head start");g.race.gate=0;g.pos=add(g.race.start,mul(g.race.dir,100));g.race.pilot[0].gate=1;g.race.pilot[0].pos=g.race.gates[0].pos;g.race.pilot[0].pace=g.race.pilot[0].speed=0;
for(int i=0;i<41&&g.race.phase==RACE_RUNNING;i++)race_tick(&g,g.pos,.1f);CHECK(g.race.phase==RACE_LOST&&g.erl.result==2,"four continuous seconds behind the pursuer records an escape loss");
CHECK(objective_fixture(RA_COMBAT,RA_TAKEDOWNS),"solo drone mode starts with forward moving drones");g.race.takedowns=4;race_tick(&g,g.pos,0);CHECK(g.race.phase==RACE_RUNNING,"four takedowns cannot satisfy the five-drone goal");g.race.takedowns=5;race_tick(&g,g.pos,0);CHECK(g.race.phase==RACE_WON&&g.erl.result_takedowns==5,"fifth takedown completes the live drone event and records actual score");
CHECK(objective_fixture(RA_COMBAT,RA_TAKEDOWNS),"second drone attempt starts independently");g.race.elapsed=g.race.limit;race_tick(&g,g.pos,0);CHECK(g.race.phase==RACE_LOST&&g.erl.result==2,"time expiry without required drones records a failure");
CHECK(objective_fixture(RA_COMBAT,RA_ENDURANCE),"endurance starts with a long race clock and temporary hull");g.race.arcade.actor[0].health=800;float own_hull=g.erl.real_hull;race_tick(&g,g.pos,.1f);CHECK(g.race.arcade.actor[0].health==800&&g.erl.real_hull==own_hull,"endurance does not automatically refill race health or consume the real hull");
CHECK(objective_fixture(RA_TIME_TRIAL,RA_MEDALS),"solo medal event has no imaginary race opponents");g.race.gate=31;last=&g.race.gates[31];from=add(last->pos,mul(last->normal,-100));g.pos=add(last->pos,mul(last->normal,100));g.race.elapsed=137;race_tick(&g,from,.01f);CHECK(g.race.rivals==0&&g.race.phase==RACE_LOST&&g.erl.result_place==2&&g.erl.result==1,"silver time finishes normally rather than inventing first place or a failed attempt");
CHECK(objective_fixture(RA_COMBAT,RA_FINISH),"moving-hazard collision test starts a normal combat event");Vec3 hazard,half;RaceGate *hazard_gate=&g.race.gates[3];race_hazard(&g.race,3,g.race.elapsed,&hazard,&half);g.race.gate=3;g.pos=add(hazard,mul(hazard_gate->normal,2000));g.speed=g.race.top_speed;g.race.arcade.actor[0].protection=0;from=add(hazard,mul(hazard_gate->normal,-2000));race_tick(&g,from,0);
CHECK(g.race.knockouts==1&&g.race.repair>0&&g.race.arcade.actor[0].health==0,"swept crossing of an animated hazard causes actual temporary race destruction");
CHECK(objective_fixture(RA_CLEAN,RA_FINISH),"scenery near-pass test starts an actual clean race");
g.race.elapsed=43;g.race.gate=0;RaceGate *close_gate=&g.race.gates[0];Vec3 close_mid=mul(add(g.race.start,close_gate->pos),.5f),rock=add(close_mid,add(mul(close_gate->right,5400),mul(close_gate->normal,-3000)));
Vec3 near_centre=add(rock,mul(close_gate->right,1300));from=add(near_centre,mul(close_gate->normal,-400));g.pos=add(near_centre,mul(close_gate->normal,400));
CHECK(race_scenery_near(&g.race,0,from,g.pos)&&!race_scenery_near(&g.race,0,add(rock,mul(close_gate->normal,-400)),add(rock,mul(close_gate->normal,400))),"genuine close scenery pass is detected while a direct collision is excluded");
CHECK(!race_scenery_near(&g.race,0,from,add(from,mul(close_gate->normal,100))),"travelling alongside a rock cannot repeatedly trigger its closest-approach cue");
float close_reserve=g.race.reserve;int close_health=g.race.arcade.actor[0].health,cash_near=g.credits;g.cue=0;race_tick(&g,from,0);
CHECK(g.race.arcade.last_hit==5&&g.cue==SFX_SPEED_SURGE&&g.race.reserve==close_reserve&&g.race.arcade.actor[0].health==close_health&&g.credits==cash_near,"live near miss produces only cosmetic feedback without free boost, health or units");
g.erl.resume=1;CHECK(save_game(&g,"near-miss.sav")&&load_game(&loaded,"near-miss.sav")&&loaded.race.arcade.last_hit==5,"new cosmetic cue preserves the existing V37 save layout and resumes exactly");
CHECK(objective_fixture(RA_TIME_TRIAL,RA_MEDALS),"gold-pace test starts an actual solo event");for(int i=0;i<RACE_GATES;i++)g.race.gates[i].pos=add(g.race.start,(Vec3){0,0,1000+i*100.f});g.race.gate=1;g.pos=g.race.gates[0].pos;g.race.elapsed=10;g.race.arcade.rules.gold=100;
CHECK(fabsf(race_gold_delta(&g)-(10-100*1000.f/4100))<.01f,"gold pace weights actual unequal leg distances instead of counting checkpoints uniformly");
/* Real steering with held normal boost must face physically competitive rivals. */
int held_wins=0,competitive=0;
for(int layout=0;layout<4;layout++){
 CHECK(objective_fixture(RA_CLEAN,RA_FINISH),"competitive clean race begins through the actual event entry");
 g.erl.event_kind=0;g.erl.result=0;g.race.phase=RACE_NONE;g.docked=1;g.erl_local.event_rules=ra_rules_make(RA_CLEAN,RA_FINISH,RA_POOL_CLEAN,19);g.erl_local.rules_pending=1;
 CHECK(erl_begin(&g,ERL_PRACTICE,layout,ERL_CIRCUIT,0),"physical clean course is built for this venue layout");g.docked=0;race_restart(&g);g.race.phase=RACE_RUNNING;g.race.countdown=0;
 for(int frame=0;frame<5600&&g.race.phase==RACE_RUNNING;frame++){
  RaceGate *gate=&g.race.gates[g.race.gate];Vec3 aim=add(gate->pos,mul(gate->normal,180));if(!gate->boost_used&&dot(sub(g.pos,gate->booster),gate->normal)<0)aim=add(gate->booster,mul(gate->normal,100));Vec3 desired=norm(sub(aim,g.pos));
  float sy=sinf(g.yaw),cy=cosf(g.yaw),sp=sinf(g.pitch),cp=cosf(g.pitch);float turn=fmaxf(-1,fminf(1,dot(desired,(Vec3){cy,0,-sy})*7)),pitch=fmaxf(-1,fminf(1,dot(desired,(Vec3){-sy*sp,cp,-cy*sp})*7));g.boost=1;game_tick(&g,.05f,turn,pitch,1,0);
 }
 int advanced=0;for(int i=0;i<g.race.rivals;i++)advanced+=g.race.pilot[i].gate>25;competitive+=advanced>=4;held_wins+=g.erl.result_place==1;
 fprintf(f,"INFO held course %d time %.2f place %d rival-final-quarter %d/%d\n",layout,g.race.elapsed,g.erl.result_place,advanced,g.race.rivals);
 for(int i=0;i<g.race.rivals;i++)fprintf(f,"INFO rival %d gate %d reserve %.1f pace %.0f\n",i,g.race.pilot[i].gate,g.race.pilot[i].reserve,g.race.pilot[i].pace);
}
CHECK(competitive>=3,"most rivals physically reach the final quarter across the clean-course layouts");
CHECK(held_wins<4,"holding normal boost throughout cannot guarantee every clean-course win");
/* The same sponsor reward path serves board and league events. */
{
 board_sponsor_fixture();DockRoll expected=erl_sponsor_offer(&g.race.arcade.rules);int cash=g.credits;unsigned xp=g.erl.xp,serial=g.dockbay.serial;
 expected.id=serial+1;board_sponsor_finish();CHECK(!g.job_n&&g.race.phase==RACE_WON&&g.credits==cash+10000,"board victory pays agreed agreed 600U contract and 400U sponsor exactly once");
 CHECK(g.erl_local.pending_count==1&&same_roll(&expected,&g.erl_local.pending[0])&&g.erl_local.pending[0].id==serial+1,"board sponsor queues the exact advertised enhanced module instead of a cosmetic promise");
 CHECK(g.erl.xp==xp&&!g.erl.championship&&!g.erl.round,"board sponsor cannot advance a local championship or league XP");
 expected=g.erl_local.pending[0];cash=g.credits;race_tick(&g,g.pos,.1f);race_withdraw(&g);CHECK(g.credits==cash&&g.erl_local.pending_count==1,"completed board race cannot repeat sponsor cash or equipment");
 CHECK(save_game(&g,"board-sponsor.sav")&&load_game(&loaded,"board-sponsor.sav")&&loaded.credits==cash&&loaded.erl_local.pending_count==1&&same_roll(&expected,&loaded.erl_local.pending[0]),"board sponsor exact instance and paid balance survive save and reload");
 g=loaded;g.docked=1;for(int sys=0;sys<256;sys++)if(station_class_for_system(&g,sys)==STATION_MEGA){g.system=sys;system_bodies(&g);break;}CHECK(!erl_claim_module(&g,0)&&g.erl_local.pending_count==1&&same_roll(&expected,&g.erl_local.pending[0]),"full Item Storage preserves an unclaimed board sponsor module");
 CHECK(erl_claim_module(&g,40)&&!g.erl_local.pending_count,"board sponsor equipment can be deliberately claimed into real storage");
 int matches=0;for(int i=0;i<DB_ITEMS;i++)matches+=same_roll(&expected,&g.dockbay.owned[i]);CHECK(matches==1&&!erl_claim_module(&g,40)&&g.credits==cash,"claimed board sponsor keeps exact stats and cannot duplicate");
 board_sponsor_fixture();cash=g.credits;for(int i=0;i<4;i++){g.race.pilot[i].gate=RACE_GATES;g.race.pilot[i].finished=100+i;}board_sponsor_finish();CHECK(!g.job_n&&g.race.phase==RACE_LOST&&g.credits==cash+4100&&g.erl_local.pending_count==1,"completed optional sponsor pays independently when the main top-three contract is lost");
 board_sponsor_fixture();cash=g.credits;race_withdraw(&g);CHECK(!g.job_n&&g.credits==cash+100&&!g.erl_local.pending_count,"withdrawal returns its ordinary deposit without farming a completed sponsor");
 board_sponsor_fixture();cash=g.credits;g.erl_local.pending_count=32;for(int i=0;i<32;i++){g.erl_local.pending[i]=erl_sponsor_offer(&g.race.arcade.rules);g.erl_local.pending[i].id=++g.dockbay.serial;}serial=g.dockbay.serial;board_sponsor_finish();CHECK(g.credits==cash+14000&&g.erl_local.pending_count==32&&g.dockbay.serial==serial,"full reward queue pays disclosed 800U sponsor alternative without deleting or rerolling equipment");
 board_sponsor_fixture();cash=g.credits;g.race.arcade.sponsor_done=0;g.race.arcade.rules.sponsor=2;board_sponsor_finish();CHECK(g.credits==cash+6000&&!g.erl_local.pending_count,"unfinished challenge neither pays nor interferes with a successful main contract");
}
fprintf(f,"INFO %d checks\nRESULT %d failures\n",checks,failures);fclose(f);sceKernelExitGame();return 0;}

