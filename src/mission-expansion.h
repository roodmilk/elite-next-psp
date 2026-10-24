/* Real, event-driven contracts. One tracked physical encounter at a time; all
 * five jobs retain independent provenance, stages, choices and unread calls. */
#include "mission-expansion-catalog.h"
const ExpeditionDef *expedition_def(int type){return type>=EXPEDITION_FIRST&&type<EXPEDITION_FIRST+EXPEDITION_COUNT?&expedition_defs[type-EXPEDITION_FIRST]:0;}
const char *expedition_sender(int type){const ExpeditionDef *d=expedition_def(type);return d?d->sender:"MISSION DESK";}
const ExpeditionStage *expedition_stage(const Game *g,int slot){if(!g||slot<0||slot>=g->job_n)return 0;const ExpeditionDef *d=expedition_def(g->jobs[slot].type);return d&&g->expeditions[slot].stage<(unsigned)d->count?&d->stages[g->expeditions[slot].stage]:0;}
static int expedition_system(const Game *g,int slot){const ExpeditionStage *s=expedition_stage(g,slot);if(!s)return -1;return g->expeditions[slot].systems[s->loc==2?2:s->loc==0?0:1];}
int expedition_world(const Game *g,int slot){const ExpeditionStage *s=expedition_stage(g,slot);return s?(int)g->expeditions[slot].worlds[s->loc==4]:-1;}
static int expedition_find_world(const Game *g,int sys,int type,int other){
 for(int b=1;b<system_body_slots(sys);b++){
  int bt=system_body_type_slot(sys,b);if(b==other||bt==SUN||(type==31?bt!=GAS:bt==GAS))continue;
  int wanted=-9;if(type==27)wanted=FIELD_DISH;else if(type==28)wanted=FIELD_WRECK;else if(type==30)wanted=FIELD_ARCHIVE;else if(type==25)wanted=FIELD_MIGRATION;
  int found=wanted<0;for(int id=2;id<10;id++)if(id!=6&&field_site_kind(g,sys,b,id)==wanted)found=1;
  const ExpeditionDef *d=&expedition_defs[type];
  for(int k=0;k<d->count;k++)if(d->stages[k].kind==XP_SITE&&d->stages[k].arg>=0){int present=0;for(int id=2;id<10;id++)if(id!=6&&field_site_kind(g,sys,b,id)==d->stages[k].arg)present=1;if(!present)found=0;}
  if(type==24){int plants=0;for(int k=0;k<LIFE_COUNT;k++)plants+=field_species_kind(sys,b,k)==LIFE_FLORA;if(plants<2)found=0;}
  if(found)return b;
 }return -1;
}
int expedition_destination(const Game *g,int type){
 const ExpeditionDef *d=expedition_def(type);if(!d)return -1;int index=type-EXPEDITION_FIRST;
 /* Search only the ship's reachable neighbourhood, using canonical planets
  * and existing site kinds. Never promise a nonexistent world or whale pod. */
 for(int pass=0;pass<2;pass++)for(int n=0;n<256;n++){
  int sys=pass==0?g->system:(g->system+n*73)%256;if(pass==0&&n)break;
  if(distance_ly(g,g->system,sys)>fminf(10.f,player_ships[g->ship].range*.1f)+.001f||!world_station_available(g,sys,0))continue;
  if(index==33&&!system_whales(sys))continue;
  if(index==32&&!(sys<=2||sys==7))continue;
  if(index>=24&&index<=31){int w=expedition_find_world(g,sys,index,-1);if(w<0)continue;int ok=1;for(int k=0;k<d->count;k++)if(d->stages[k].kind==XP_SITE&&d->stages[k].arg>=0){int found=0;for(int id=2;id<10;id++)if(id!=6&&field_site_kind(g,sys,w,id)==d->stages[k].arg)found=1;if(!found)ok=0;}if(index==24){int plants=0;for(int k=0;k<LIFE_COUNT;k++)plants+=field_species_kind(sys,w,k)==LIFE_FLORA;if(plants<2)ok=0;}if(!ok)continue;if(index==26&&expedition_find_world(g,sys,index,w)<0)continue;}
  return sys;
 }return -1;
}
static int expedition_second(const Game *g,int sys){
 for(int n=1;n<256;n++){int candidate=(sys+n*73)%256;if(candidate!=g->system&&candidate!=sys&&world_station_available(g,candidate,0)&&distance_ly(g,sys,candidate)<=player_ships[g->ship].range*.1f+.001f)return candidate;}return sys;
}
int expedition_can_accept(const Game *g,int type,int destination){
 const ExpeditionDef *d=expedition_def(type);if(!d||destination<0)return 0;
 if(type==EXPEDITION_FIRST+39)for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==MISSION_RACE||g->jobs[i].type==type){message((Game*)g,"Finish or abandon your current race entry first.");return 0;}
 if(d->gear==2&&(g->upgrades&131072)){message((Game*)g,"Store your refinery first; this order needs unrefined minerals.");return 0;}
 if(d->gear&&!g->laser){message((Game*)g,"Fit a weapon before accepting this contract.");return 0;}
 if(d->gear==3&&!(g->upgrades&131072)){message((Game*)g,"Fit an onboard refinery before accepting this contract.");return 0;}
 int need=0;for(int i=0;i<d->count;i++)if(d->stages[i].kind==XP_RECOVER||d->stages[i].kind==XP_MINE||d->stages[i].kind==XP_REFINE||d->stages[i].kind==XP_BUY)need=d->stages[i].need>need?d->stages[i].need:need;
 if(cargo_capacity(g)-cargo_used(g)<need){message((Game*)g,"Free the cargo space required by this contract first.");return 0;}return 1;
}
int expedition_start(Game *g,int type,int destination,int reward){
 const ExpeditionDef *d=expedition_def(type);if(!d||g->job_n>=MISSION_SLOTS||!expedition_can_accept(g,type,destination))return 0;
 int slot=g->job_n++;Job *j=&g->jobs[slot];memset(j,0,sizeof(*j));j->type=type;j->origin=g->system;j->dest=destination;j->reward=reward;j->target=-1;j->item=1;j->time=300;
 ExpeditionState *e=&g->expeditions[slot];memset(e,0,sizeof(*e));e->seed=sector_hash(type*7919u+destination*31337u+g->system*73u+(unsigned)g->world_clock);e->systems[0]=g->system;e->systems[1]=destination;e->systems[2]=expedition_second(g,destination);
 int index=type-EXPEDITION_FIRST,w=expedition_find_world(g,destination,index,-1),w2=expedition_find_world(g,destination,index,w);e->worlds[0]=w<0?1:w;e->worlds[1]=w2<0?e->worlds[0]:w2;e->unread=1;e->shots_at_start=g->shots;j->dest=expedition_system(g,slot);
 for(int k=0;k<6;k++)e->health[k]=100;
 if(index==21){e->advance=2000;g->credits+=2000;}g->job_sel=slot;jobs_sync(g);g->destination=j->dest;g->cue=SFX_COMM;return 1;
}
static void expedition_cleanup(Game *g);
void expedition_remove(Game *g,int slot){expedition_cleanup(g);if(slot<0||slot>=MISSION_SLOTS)return;for(int i=slot;i<MISSION_SLOTS-1;i++)g->expeditions[i]=g->expeditions[i+1];memset(&g->expeditions[MISSION_SLOTS-1],0,sizeof(ExpeditionState));g->expedition_runtime.built=0;}
static void expedition_cleanup(Game *g){ExpeditionRuntime *r=&g->expedition_runtime;if(r->built&&r->system==g->system){for(int i=0;i<6;i++)if(r->npc[i]>=0&&r->npc[i]<NPC_COUNT)g->npc[r->npc[i]].alive=0;for(int i=0;i<8;i++)if(r->pod[i]>=0&&r->pod[i]<DEBRIS_COUNT)g->debris[r->pod[i]].alive=0;}r->built=0;}
void expedition_spawn_reset(Game *g){memset(&g->expedition_runtime,0,sizeof(g->expedition_runtime));g->expedition_runtime.job=-1;memset(g->debris_mined,0,sizeof(g->debris_mined));}
static void expedition_advance(Game *g,int slot){
 if(!expedition_stage(g,slot))return;ExpeditionState *e=&g->expeditions[slot];e->history[e->stage]=e->progress;e->done|=1u<<e->stage;e->stage++;e->progress=e->seen=e->destroyed=e->recovered=e->rival_count=0;e->elapsed=e->dwell=e->escort_distance=0;
 if(slot==g->expedition_runtime.job)expedition_cleanup(g);
 const ExpeditionDef *d=expedition_def(g->jobs[slot].type);
 if(e->stage>=(unsigned)d->count){
  int reward=g->jobs[slot].reward+(int)e->bonus-(int)e->advance;if(reward<0)reward=0;g->jobs[slot].reward=reward;
  mission_finish_slot(g,slot,"Contract complete. Your agreed payment has arrived.");return;
 }
 g->jobs[slot].dest=expedition_system(g,slot);e->unread=1;jobs_sync(g);g->cue=SFX_COMM;
}
const char *expedition_objective(const Game *g,int slot){static char out[192];const ExpeditionStage *s=expedition_stage(g,slot);if(!s)return "No active expedition";const ExpeditionState *e=&g->expeditions[slot];if(g->system!=expedition_system(g,slot)){snprintf(out,sizeof(out),"Travel to %s. %s",g->systems[expedition_system(g,slot)].name,s->objective);return out;}snprintf(out,sizeof(out),"%s%s",s->objective,e->progress?"":"");return out;}
int expedition_reserved(const Game *g,int item){if(item<0||item>=GOODS)return 0;int n=0;for(int i=0;i<g->job_n;i++)if(expedition_stage(g,i))n+=g->expeditions[i].held[item];return n;}
int expedition_card_slot(const Game *g){for(int i=0;i<g->job_n;i++)if(expedition_stage(g,i)&&g->expeditions[i].unread)return i;return -1;}
void expedition_ack(Game *g,int slot){if(expedition_stage(g,slot))g->expeditions[slot].unread=0;}
int expedition_site_id(const Game *g,int slot){const ExpeditionStage *s=expedition_stage(g,slot);if(!s||s->kind!=XP_SITE)return -1;if(s->arg==-3)return 0;if(s->arg==-1)return 1;if(s->arg==-2)return 4;for(int id=2;id<10;id++)if(id!=6&&field_site_kind(g,expedition_system(g,slot),expedition_world(g,slot),id)==s->arg)return id;return -1;}
int expedition_ready(const Game *g){return g->job_sel>=0&&g->job_sel<g->job_n&&expedition_stage(g,g->job_sel)&&expedition_system(g,g->job_sel)==g->system;}
Vec3 expedition_waypoint(const Game *g){return g->expedition_runtime.waypoint;}
int expedition_target(const Game *g,int slot){
 const ExpeditionStage *s=expedition_stage(g,slot);if(!s||expedition_system(g,slot)!=g->system)return -1;
 if(s->kind==XP_DOCK||s->kind==XP_BUY)return 0;
 if(s->loc>=3&&g->planet<0)return expedition_world(g,slot)+1;
 if(s->kind==XP_WHALE)return WHALE_TARGET_ID;
 if(s->kind==XP_DISCOVERY&&g->expedition_runtime.built&&g->expedition_runtime.job==slot&&g->expedition_runtime.external_id>=0)return g->expedition_runtime.external_id;
 if(s->kind==XP_ANOMALY)return ANOMALY_ID_MIN+s->arg;
 if(s->kind==XP_RACE)return RACE_TARGET_ID;
 if(s->loc>=3&&g->planet==expedition_world(g,slot))return -1;
 return EXPEDITION_TARGET_ID;
}
static Vec3 expedition_anchor(const Game *g,const ExpeditionState *e){
 float a=(e->seed%6283)*.001f;Vec3 p=add(hub_position(g,0),(Vec3){cosf(a)*16000,10000,sinf(a)*16000});
 for(int attempt=0;attempt<10;attempt++){int clear=1;for(int b=0;b<BODY_COUNT;b++)if(body_active(&g->bodies[b])&&length(sub(p,g->bodies[b].pos))<g->bodies[b].radius*2.8f+9000)clear=0;if(clear)return p;p.y+=17000;}return p;
}
static int expedition_slot_available(const Game *g,int id){
 if(expedition_npc(g,id)>=0)return 0;
 for(int j=0;j<g->job_n;j++)if(g->jobs[j].dest==g->system&&g->jobs[j].target==id&&(mission_base_type(g->jobs[j].type)==MISSION_BOUNTY||mission_base_type(g->jobs[j].type)==MISSION_RESCUE))return 0;
 const NPC *n=&g->npc[id];if(!n->alive)return 1;
 return n->role!=LAW&&n->bounty_slot<0&&n->traveller<0&&g->encounter_npc!=id&&(!g->trader_offer_active||g->trader_offer_npc!=id)&&length(sub(n->pos,g->pos))>20000;
}
static int expedition_actor(Game *g,int number,int role,Vec3 pos,int capital){
 ExpeditionRuntime *r=&g->expedition_runtime;for(int i=0;i<36;i++)if(i!=8&&i!=20&&i!=32&&expedition_slot_available(g,i)){NPC *n=&g->npc[i];memset(n,0,sizeof(*n));n->alive=1;n->role=role;n->pos=pos;n->dir=(Vec3){0,0,1};n->mesh=mesh_id(capital?"ANACONDA":role==PIRATES?"KRAIT":"COBRA MK 3");n->scale=capital?9:1;n->radius=capital?900:75;n->health=capital?900:100;n->shield=capital?300:35;n->target=-1;n->traveller=n->bounty_slot=-1;n->mega_hull=-1;n->cruise=130;n->freighter=capital;n->freight_style=1;int identity=number;const ExpeditionStage *s=expedition_stage(g,r->job);int index=g->jobs[r->job].type-EXPEDITION_FIRST;if(s&&s->kind==XP_COMBAT&&number==0&&(index==8||index==9||index==10||index==14))identity=1;n->name_known=s&&s->kind==XP_COMBAT&&identity==1;n->vessel_seed=sector_hash(g->expeditions[r->job].seed+identity*391u);r->npc[number]=i;return i;}return -1;
}
static int expedition_pod(Game *g,int number,Vec3 pos,int item,int rock){
 for(int i=0;i<DEBRIS_COUNT;i++)if(!g->debris[i].alive||(g->debris[i].rock&&length(sub(g->debris[i].pos,g->pos))>20000)){int owned=0;for(int k=0;k<8;k++)if(g->expedition_runtime.pod[k]==i)owned=1;if(owned)continue;Debris *p=&g->debris[i];memset(p,0,sizeof(*p));p->alive=1;p->mega_hull=-1;p->pos=pos;p->radius=rock?180:24;p->rock=rock;p->health=15;p->good=item;p->qty=1;p->life=3600;g->debris_mined[i]=0;g->expedition_runtime.pod[number]=i;return i;}return -1;
}
static void expedition_build(Game *g){
 if(!expedition_ready(g))return;int slot=g->job_sel;ExpeditionState *e=&g->expeditions[slot];const ExpeditionStage *s=expedition_stage(g,slot);ExpeditionRuntime *r=&g->expedition_runtime;uint32_t key=e->seed^(e->stage*0x9e3779b9u);
 if(r->built&&r->key==key&&r->system==g->system&&r->job==slot)return;
 expedition_cleanup(g);memset(r,0,sizeof(*r));for(int i=0;i<6;i++)r->npc[i]=-1;for(int i=0;i<8;i++)r->pod[i]=-1;r->job=slot;r->system=g->system;r->key=key;r->built=1;r->external_id=-1;r->anchor=expedition_anchor(g,e);r->waypoint=r->anchor;r->previous=g->pos;
 int kind=s->kind,index=g->jobs[slot].type-EXPEDITION_FIRST;
 if(s->loc>=3){if(g->planet==expedition_world(g,slot)){r->site=expedition_site_id(g,slot);if(r->site>=0)r->waypoint=surface_poi(g,r->site);else for(int i=0;i<LIFE_COUNT;i++)if(g->life[i].alive&&((kind==XP_FLORA&&g->life[i].kind==LIFE_FLORA)||(kind==XP_BEHAVIOUR&&g->life[i].kind==LIFE_FAUNA)||(kind==XP_SAMPLE&&g->life[i].kind==LIFE_MINERAL))){r->waypoint=g->life[i].pos;break;}}return;}
 if(kind==XP_HAIL||kind==XP_PATROL){int count=s->arg==1||kind==XP_PATROL?3:1;for(int i=0;i<count;i++)expedition_actor(g,i,TRADERS,kind==XP_PATROL?add(r->anchor,(Vec3){sinf(i*1.7f+(e->seed%6283)*.001f)*9000,cosf(i*1.7f+(e->seed%6283)*.001f)*2250,cosf(i*1.7f+(e->seed%6283)*.001f)*9000}):add(r->anchor,(Vec3){(i-1)*900.f,0,i*500.f}),index==1||index==2||index==3||index==12||index==16||index==36);r->waypoint=count>1?r->anchor:g->npc[r->npc[0]>=0?r->npc[0]:0].pos;}
 if(kind==XP_COMBAT){for(int i=0;i<s->need;i++)if(!(e->destroyed&(1u<<i)))expedition_actor(g,i,PIRATES,add(r->anchor,(Vec3){(i-1)*600.f,100,1200}),0);if(index<8){int count=index==1?1:3;for(int a=0;a<count;a++)if(e->health[a]>0){int n=expedition_actor(g,3+a,TRADERS,add(r->anchor,(Vec3){a*280.f,0,400+a*180.f}),index==1);if(n>=0)g->npc[n].health=e->health[a]*(index==1?9:1);}}}
 if(kind==XP_RECOVER||kind==XP_COMPETE||kind==XP_MINE||kind==XP_REFINE){int count=kind==XP_COMPETE?5:s->need;for(int i=0;i<count;i++)if(!(e->recovered&(1u<<i)))expedition_pod(g,i,add(r->anchor,(Vec3){(i-1)*650.f,i*130.f,i*200.f}),s->arg>0?s->arg:8,kind==XP_MINE||kind==XP_REFINE);if(kind==XP_COMPETE)expedition_actor(g,0,TRADERS,add(r->anchor,(Vec3){-1800,0,-1200}),0);}
 if(kind==XP_ESCORT){for(int i=0;i<(index==1?1:3);i++){if(e->health[i]<=0)continue;int n=expedition_actor(g,i,TRADERS,add(r->anchor,(Vec3){i*280.f,0,i*180.f}),index==1);if(n>=0){g->npc[n].health=fmaxf(1,e->health[i]*(index==1?9:1));g->npc[n].pos.z+=e->escort_distance;}}expedition_actor(g,4,PIRATES,add(r->anchor,(Vec3){-900,200,1700}),0);expedition_actor(g,5,PIRATES,add(r->anchor,(Vec3){900,-200,1700}),0);}
 if(kind==XP_MAP){expedition_actor(g,0,TRADERS,r->anchor,1);r->waypoint=add(r->anchor,(Vec3){0,0,2200});}
 if(kind==XP_CHASE){expedition_actor(g,0,PIRATES,add(r->anchor,(Vec3){0,0,-1500}),0);expedition_actor(g,1,PIRATES,add(r->anchor,(Vec3){-400,100,-1700}),0);}
 if(kind==XP_ANOMALY)r->waypoint=g->anomaly[s->arg].pos;
 if(kind==XP_MAP&&r->npc[0]>=0)for(int k=0;k<3;k++)expedition_pod(g,k,add(r->anchor,(Vec3){1800+k*230.f,300-k*170.f,-900+k*550.f}),12,0);
 /* A saturated ambient pool must never create phantom objectives or fail an
  * escort with no ships. Keep the navigation area and retry as traffic clears. */
 int missing=0,actors=kind==XP_HAIL?(s->arg==1?3:1):kind==XP_PATROL?3:kind==XP_COMBAT?s->need:kind==XP_ESCORT?(index==1?1:3):kind==XP_CHASE?2:kind==XP_MAP||kind==XP_COMPETE?1:0;
 for(int i=0;i<actors;i++)if(!(kind==XP_COMBAT&&(e->destroyed&(1u<<i)))&&!(kind==XP_ESCORT&&e->health[i]<=0)&&r->npc[i]<0)missing=1;
 if(kind==XP_COMBAT&&index<8)for(int a=0;a<(index==1?1:3);a++)if(e->health[a]>0&&r->npc[3+a]<0)missing=1;
 int pods=kind==XP_COMPETE?5:kind==XP_RECOVER||kind==XP_MINE||kind==XP_REFINE?s->need:0;for(int i=0;i<pods;i++)if(!(e->recovered&(1u<<i))&&r->pod[i]<0)missing=1;
 if(missing)expedition_cleanup(g);
}
int expedition_npc(const Game *g,int id){const ExpeditionRuntime *r=&g->expedition_runtime;if(!r->built||r->system!=g->system)return -1;for(int k=0;k<6;k++)if(r->npc[k]==id)return k;return -1;}
const char *expedition_npc_name(const Game *g,int id){
 int actor=expedition_npc(g,id);if(actor<0)return "UNKNOWN";int slot=g->expedition_runtime.job,index=g->jobs[slot].type-EXPEDITION_FIRST;const ExpeditionStage *s=expedition_stage(g,slot);
 if(s&&s->kind==XP_HAIL&&s->arg==1)return actor==1?(index==8?"SABLE VOSS":"ASH KITE"):"CIVILIAN PILOT";
 if(s&&s->kind==XP_COMBAT&&actor==0&&(index==8||index==9||index==10||index==14))return index==8?"SABLE VOSS":"ASH KITE";
 if(g->npc[id].role==PIRATES)return "MISSION RAIDER";
 if(s&&s->kind==XP_PATROL)return actor==0?"TRANSPORT JUNO":actor==1?"COURIER TWELVE":"MERCHANT HESTIA";
 return expedition_sender(g->jobs[slot].type);
}
const char *expedition_pod_name(const Game *g,int id){
 const ExpeditionRuntime *r=&g->expedition_runtime;if(!r->built||r->system!=g->system)return 0;int own=0;for(int k=0;k<8;k++)own|=r->pod[k]==id;if(!own)return 0;const ExpeditionStage *s=expedition_stage(g,r->job);if(!s)return 0;int index=g->jobs[r->job].type-EXPEDITION_FIRST;
 if(g->debris[id].rock)return "MISSION ASTEROID";
 if(s->kind==XP_MINE||s->kind==XP_REFINE)return "EXTRACTED ORE";
 if(index==2||index==3)return "ESCAPE POD";
 if(index==11)return "POD RECORDER";
 if(index==12||index==35)return "FLIGHT RECORDER";
 if(index==14)return "SUPPLY MANIFEST";
 if(index==16)return "NAVIGATION CORE";
 if(index==36)return s->arg==13?"SALVAGER'S CACHE":"ROUTE RECORDER";
 return "CONTRACT CONTAINER";
}
int expedition_npc_control(Game *g,int id,float dt){
 int actor=expedition_npc(g,id);if(actor<0)return 0;ExpeditionRuntime *r=&g->expedition_runtime;const ExpeditionStage *s=expedition_stage(g,r->job);if(!s)return 0;ExpeditionState *e=&g->expeditions[r->job];NPC *n=&g->npc[id];if(!n->alive||e->unread)return 1;n->flash=fmaxf(0,n->flash-dt);n->cooldown-=dt;
 n->player_tag=fmaxf(0,n->player_tag-dt);
 if(n->role==PIRATES){int victim=-2;Vec3 aim=g->pos;if(n->player_tag<=0||length(sub(n->pos,g->pos))>1800){int begin=s->kind==XP_ESCORT?0:3,end=s->kind==XP_ESCORT?3:6;if(s->kind==XP_COMBAT||s->kind==XP_ESCORT)for(int a=begin;a<end;a++)if(r->npc[a]>=0&&g->npc[r->npc[a]].alive){victim=r->npc[a];aim=g->npc[victim].pos;break;}}n->target=victim;n->dir=norm(add(mul(n->dir,.9f),mul(norm(sub(aim,n->pos)),.1f)));float distance=length(sub(aim,n->pos));if(distance>450)n->pos=add(n->pos,mul(n->dir,dt*220));if(distance<1800&&dot(n->dir,norm(sub(aim,n->pos)))>.75f&&n->cooldown<=0){n->cooldown=1.6f;n->flash=.12f;if(victim==-2){player_damage(g,4,"Mission raiders are firing at your shields.");g->attacked=3;}else hit(g,victim,6,0);}return 1;}
 n->target=-1;
 if(s->kind==XP_COMBAT&&actor>=3)e->health[actor-3]=n->health/(n->freighter?9:1);
 if(s->kind==XP_ESCORT&&actor<3){Vec3 end=add(r->anchor,(Vec3){(actor-1)*300.f,0,12500});Vec3 aim=sub(end,n->pos);float d=length(aim);n->dir=norm(aim);if(d>70)n->pos=add(n->pos,mul(n->dir,fminf(d,dt*135)));e->health[actor]=n->health/(n->freighter?9:1);e->escort_distance=fmaxf(e->escort_distance,fminf(12500,n->pos.z-r->anchor.z));}
 if(s->kind==XP_COMPETE&&actor==0){int best=-1;float distance=1e9f;for(int k=0;k<5;k++){int pod=r->pod[k];if(pod<0||!g->debris[pod].alive)continue;float d=length(sub(g->debris[pod].pos,n->pos));if(d<distance){distance=d;best=k;}}if(best>=0){Vec3 aim=g->debris[r->pod[best]].pos;n->dir=norm(sub(aim,n->pos));n->pos=add(n->pos,mul(n->dir,dt*95));if(distance<45){g->debris[r->pod[best]].alive=0;e->recovered|=1u<<best;e->rival_count++;}}}
 return 1;
}
void expedition_killed(Game *g,int id,int player){int actor=expedition_npc(g,id);if(actor<0)return;int slot=g->expedition_runtime.job;const ExpeditionStage *s=expedition_stage(g,slot);if(!s)return;ExpeditionState *e=&g->expeditions[slot];if(s->kind==XP_ESCORT&&actor<3)e->health[actor]=0;else if(s->kind==XP_COMBAT&&actor>=3)e->health[actor-3]=0;if(s->kind==XP_COMBAT&&actor<s->need){e->destroyed|=1u<<actor;e->progress++;if(e->progress>=(unsigned)s->need)expedition_advance(g,slot);}else if((s->kind==XP_ESCORT&&actor<3)||(s->kind==XP_COMBAT&&actor>=3)){e->destroyed|=1u<<actor;e->unread=1;if(s->kind==XP_COMBAT&&e->health[0]<=0&&(g->jobs[slot].type==EXPEDITION_FIRST+1||e->health[1]<=0&&e->health[2]<=0)){abandon_mission(g,slot);message(g,"The protected ships were destroyed. Contract failed.");}}(void)player;}
int expedition_hail(Game *g,int id){
 int actor=IS_NPC_ID(id)?expedition_npc(g,id-NPC_ID_MIN):-1;if(actor<0)return 0;int slot=g->expedition_runtime.job;const ExpeditionStage *s=expedition_stage(g,slot);if(!s)return 0;
 if(length(sub(g->pos,g->npc[id-NPC_ID_MIN].pos))>600){message(g,"Move within 600 m of the mission contact.");return 1;}
 g->npc[id-NPC_ID_MIN].name_known=1;ExpeditionState *e=&g->expeditions[slot];if(s->kind==XP_HAIL){if(s->arg==1&&actor!=1){message(g,"Identity checked: civilian pilot. This is not the suspect.");return 1;}
  int index=g->jobs[slot].type-EXPEDITION_FIRST;
  if(index==5){if(e->held[0]<2||g->cargo[0]<2){message(g,"Bring the two purchased tonnes of food for transfer.");return 1;}g->cargo[0]-=2;e->held[0]-=2;}
  if(s->arg==2){if(cargo_used(g)>=cargo_capacity(g)){message(g,"Free a tonne of hold space for your passenger.");return 1;}e->passengers=1;g->cargo[1]++;e->held[1]++;}
  if(index==6||index==23||index==38){if(cargo_used(g)>=cargo_capacity(g)){message(g,"Free 1T for the sealed shipment.");return 1;}g->cargo[8]++;e->held[8]++;}
  expedition_advance(g,slot);return 1;
 }
 if(s->kind==XP_PATROL){if(actor!=(int)e->progress){message(g,"Identity already checked. Follow the next patrol marker.");return 1;}unsigned bit=1u<<actor;if(!(e->seen&bit)){e->seen|=bit;e->progress++;if(e->progress>=(unsigned)s->need)expedition_advance(g,slot);}return 1;}
 message(g,"Mission channel confirmed. Follow the current objective in your mission log.");return 1;
}
void expedition_event(Game *g,int kind,int id,int arg,int quantity){
 if(quantity<=0)return;
 for(int slot=0;slot<g->job_n;slot++){
  const ExpeditionStage *s=expedition_stage(g,slot);if(!s||s->kind!=kind||expedition_system(g,slot)!=g->system)continue;ExpeditionState *e=&g->expeditions[slot];
  if(s->loc>=3&&g->planet!=expedition_world(g,slot))continue;
  if(kind==XP_BUY){if(arg!=s->arg)continue;e->held[arg]+=quantity;e->spent+=g->price[arg]*quantity;}
  else if(kind==XP_FLORA){if(arg!=LIFE_FLORA||id<0||id>=LIFE_COUNT||(e->seen&(1u<<id)))continue;e->seen|=1u<<id;}
  else if(kind==XP_SAMPLE){if(id<0||id>=LIFE_COUNT||arg!=12&&arg!=9)continue;e->held[arg]+=quantity;}
  else if(kind==XP_SITE){if(id!=expedition_site_id(g,slot))continue;}
  else if(kind==XP_ANOMALY){if(id!=ANOMALY_ID_MIN+s->arg)continue;}
  else if(kind==XP_DISCOVERY){if(id!=g->expedition_runtime.external_id)continue;}
  e->progress+=quantity;if(e->progress>=(unsigned)s->need)expedition_advance(g,slot);return;
 }
}
void expedition_collected(Game *g,int pod,int item,int quantity,int mined){
 if(!expedition_ready(g)||quantity<=0)return;ExpeditionRuntime *r=&g->expedition_runtime;int slot=r->job;const ExpeditionStage *s=expedition_stage(g,slot);if(!s)return;ExpeditionState *e=&g->expeditions[slot];int own=-1;for(int i=0;i<8;i++)if(r->pod[i]==pod)own=i;
 if((s->kind==XP_RECOVER||s->kind==XP_COMPETE)&&own>=0&&!(e->recovered&(1u<<own))){e->recovered|=1u<<own;e->held[item]+=quantity;e->progress++;if(s->kind==XP_RECOVER&&e->progress>=(unsigned)s->need)expedition_advance(g,slot);}
 else if((s->kind==XP_MINE&&item==12||s->kind==XP_REFINE&&item==9)&&mined){e->held[item]+=quantity;e->progress+=quantity;if(e->progress>=(unsigned)s->need)expedition_advance(g,slot);}
}
static int expedition_cargo_intact(const Game *g,const ExpeditionState *e){for(int item=0;item<GOODS;item++)if(e->held[item]>(unsigned)g->cargo[item])return 0;return 1;}
static void expedition_deliver(Game *g,ExpeditionState *e){for(int item=0;item<GOODS;item++){g->cargo[item]-=e->held[item];e->held[item]=0;}e->passengers=0;}
void expedition_dock(Game *g){
 for(int slot=0;slot<g->job_n;){const ExpeditionStage *s=expedition_stage(g,slot);
  if(s&&g->jobs[slot].type==EXPEDITION_FIRST+3&&s->kind==XP_RECOVER&&g->system==(int)g->expeditions[slot].systems[0]&&g->station_variant==0&&g->expeditions[slot].progress&&expedition_cargo_intact(g,&g->expeditions[slot])){g->jobs[slot].reward=g->jobs[slot].reward*g->expeditions[slot].progress/3;expedition_advance(g,slot);s=expedition_stage(g,slot);}
  if(!s||s->kind!=XP_DOCK||g->system!=expedition_system(g,slot)||g->station_variant!=0){slot++;continue;}ExpeditionState *e=&g->expeditions[slot];if(!expedition_cargo_intact(g,e)){message(g,"Required mission cargo is missing. Recover it or abandon the contract.");slot++;continue;}
  int index=g->jobs[slot].type-EXPEDITION_FIRST;if(s->seconds&&e->elapsed>s->seconds){abandon_mission(g,slot);message(g,"The courier deadline passed. Contract failed.");continue;}
  if(index==22&&e->stage==1&&cargo_used(g)-(int)e->held[8]+2>cargo_capacity(g)){message(g,"Free 2T before exchanging the shipment.");slot++;continue;}if(!(index==37&&e->stage==1))expedition_deliver(g,e);if(index==22&&e->stage==1){g->cargo[0]+=2;e->held[0]=2;}
  int n=g->job_n;expedition_advance(g,slot);if(g->job_n==n)slot++;
 }
}
int expedition_choice(Game *g,int slot,int choice){
 const ExpeditionStage *s=expedition_stage(g,slot);if(!s||s->kind!=XP_CHOICE||choice<0||choice>1)return 0;ExpeditionState *e=&g->expeditions[slot];int arg=s->arg;e->choice|=(choice+1)<<(e->stage*2);
 if(arg==4&&choice){abandon_mission(g,slot);message(g,"Championship entry withdrawn. No championship prize awarded.");return 1;}
 if(arg==1&&choice){e->stage=expedition_def(g->jobs[slot].type)->count-1;expedition_advance(g,slot);return 1;}
 if(arg==2&&choice){g->jobs[slot].reward=500;e->bonus=e->advance=0;e->systems[2]=e->systems[0];expedition_advance(g,slot);return 1;}
 if((arg==1||arg==2||arg==3)&&!choice)e->bonus+=arg==1?3000:2500;
 if(arg==3&&choice)e->systems[2]=e->systems[0];
 if(arg==0){if(!choice){g->saga_trust[LAW]+=2;e->bonus+=300;}else g->saga_trust[EXPLORERS]+=2;}
 expedition_advance(g,slot);return 1;
}
int expedition_race_job(const Game *g){for(int slot=0;slot<g->job_n;slot++){const ExpeditionStage *s=expedition_stage(g,slot);if(s&&s->kind==XP_RACE&&expedition_system(g,slot)==g->system)return slot;}return -1;}
int expedition_race_win(Game *g,int slot){const ExpeditionStage *s=expedition_stage(g,slot);if(!s||s->kind!=XP_RACE)return 0;g->race.phase=RACE_NONE;g->expeditions[slot].progress=1;expedition_advance(g,slot);return 1;}
void expedition_external(Game *g,int id,Vec3 position){if(!expedition_ready(g))return;const ExpeditionStage *s=expedition_stage(g,g->job_sel);if(s&&(s->kind==XP_DISCOVERY||s->kind==XP_WHALE||s->kind==XP_OBSERVE)){g->expedition_runtime.external_id=id;g->expedition_runtime.anchor=position;g->expedition_runtime.waypoint=position;}}
void expedition_tick(Game *g,float dt){
 if(g->dead||g->dock_stage||g->jump>0||g->police_stop||opening_active(g))return;
 for(int i=0;i<g->job_n;i++){const ExpeditionStage *s=expedition_stage(g,i);if(s&&s->seconds&&s->kind==XP_DOCK)g->expeditions[i].elapsed+=dt;}
 if(!expedition_ready(g)){expedition_cleanup(g);return;}expedition_build(g);if(!g->expedition_runtime.built)return;int slot=g->job_sel;const ExpeditionStage *s=expedition_stage(g,slot);if(!s)return;ExpeditionState *e=&g->expeditions[slot];ExpeditionRuntime *r=&g->expedition_runtime;if(g->docked||e->unread)return;if(s->kind!=XP_DOCK)e->elapsed+=dt;
 if(s->loc>=3){if(g->planet!=expedition_world(g,slot))return;if(s->kind==XP_SITE){r->site=expedition_site_id(g,slot);if(r->site>=0)r->waypoint=surface_poi(g,r->site);}else if(s->kind==XP_BEHAVIOUR){for(int i=0;i<LIFE_COUNT;i++){Lifeform *l=&g->life[i];if(!l->alive||l->kind!=LIFE_FAUNA||length(sub(l->pos,g->pos))>110)continue;r->waypoint=l->pos;unsigned bit=l->behaviour==FAUNA_FEED?1u:l->behaviour==FAUNA_ALERT||l->behaviour==FAUNA_FLEE?2u:0;e->seen|=bit;if(bit){l->scanned=1;g->surface_progress[g->system][g->planet]|=1u<<(8+i);}}e->progress=(e->seen&1)+((e->seen>>1)&1);if(e->progress>=2)expedition_advance(g,slot);}return;}
 if(g->planet>=0)return;
 if(s->kind==XP_WAYPOINT||s->kind==XP_CHASE||s->kind==XP_PATROL||s->kind==XP_OBSERVE||s->kind==XP_MAP){
  float angle=e->progress*1.7f+(e->seed%6283)*.001f;float radius=s->kind==XP_MAP?2200:s->kind==XP_OBSERVE?1700:9000;
  r->waypoint=add(r->anchor,(Vec3){sinf(angle)*radius,cosf(angle)*radius*.25f,cosf(angle)*radius});
  if(s->kind!=XP_PATROL){float range=s->kind==XP_MAP?600:s->kind==XP_OBSERVE?500:800;if(length(sub(g->pos,r->waypoint))<range||(!s->seconds&&segment_distance(r->previous,g->pos,r->waypoint)<range)){if(!s->seconds||g->speed<150){e->dwell+=dt;if(e->dwell>=fmaxf(.15f,s->seconds)){e->progress++;e->dwell=0;g->cue=SFX_SCAN;if(e->progress>=(unsigned)s->need){if(s->kind==XP_OBSERVE&&g->shots==e->shots_at_start)e->bonus+=1000;expedition_advance(g,slot);}}}}else e->dwell=0;}
 }
 if(s->kind==XP_ESCORT){int live=0,arrived=0;for(int a=0;a<3;a++){int id=r->npc[a];if(id>=0&&g->npc[id].alive){live++;if(length(sub(g->npc[id].pos,add(r->anchor,(Vec3){(a-1)*300.f,0,12500})))<100)arrived++;}}if(!live){abandon_mission(g,slot);message(g,"No convoy ships survived. Contract failed.");return;}r->waypoint=g->npc[r->npc[0]>=0&&g->npc[r->npc[0]].alive?r->npc[0]:r->npc[1]>=0&&g->npc[r->npc[1]].alive?r->npc[1]:r->npc[2]].pos;e->progress=arrived;if(arrived==live&&length(sub(g->pos,r->waypoint))<2500){if(live==(g->jobs[slot].type-EXPEDITION_FIRST==1?1:3))e->bonus+=1000;expedition_advance(g,slot);}}
 else if(s->kind==XP_COMPETE&&e->progress+e->rival_count>=5){if(!e->progress){abandon_mission(g,slot);message(g,"The rival recovered every container. Contract failed.");return;}g->jobs[slot].reward=g->jobs[slot].reward*e->progress/5;expedition_advance(g,slot);}
 else if(s->kind==XP_RECOVER||s->kind==XP_COMPETE||s->kind==XP_MINE||s->kind==XP_REFINE){for(int i=0;i<8;i++)if(r->pod[i]>=0&&g->debris[r->pod[i]].alive){r->waypoint=g->debris[r->pod[i]].pos;break;}}
 r->previous=g->pos;
 if(s->kind==XP_HAIL||s->kind==XP_COMBAT){int best=-1;float distance=1e9f;for(int i=0;i<6;i++)if(r->npc[i]>=0&&g->npc[r->npc[i]].alive){float d=length(sub(g->npc[r->npc[i]].pos,g->pos));if(d<distance){distance=d;best=r->npc[i];}}if(best>=0)r->waypoint=g->npc[best].pos;}
}
