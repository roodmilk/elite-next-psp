static unsigned sector_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
#include "erl-contracts.h"
/* Rendering and mineable bodies use the same world-space tilted annulus. */
float planet_ring_inner(const Body *b){return b->radius*(1.4f+((b->seed>>5)&3)*.05f);}
float planet_ring_outer(const Body *b){return b->radius*(2.3f+((b->seed>>7)&3)*.1f);}
Vec3 planet_ring_point(const Body *b,float angle,float radius){
 float tilt=.18f+((b->seed>>11)&7)*.045f;
 return add(b->pos,(Vec3){cosf(angle)*radius,sinf(angle)*radius*sinf(tilt),sinf(angle)*radius*cosf(tilt)});
}
int prosperity(const Game *g,int system){const int wealth[8]={5,4,2,3,3,5,4,2};return wealth[g->systems[system].economy];}
void system_bodies(Game *g){
 opening_world_apply(g);
 /* Ten bounded slots support sparse, crowded and multiple-star systems. A
  * radius of zero is an inactive slot and is ignored by maps, sensors and
  * physics. The layout remains deterministic for saves and mission routes. */
 const unsigned suns[]={0x80dfff,0x66c8ff,0x526eff,0xc8edff,0x80c8ff,0x609cff,0x9ee8ff,0xb8e8ff,0x60a0ff,0x90cfff};
 const unsigned worlds[]={0xc97535,0x91b45c,0x8763b5,0x7ebfc4,0xb87775,0xadc2ce,0xd4a574,0x5a8f6a,0x6b5b95,0xc45c5c,0x3d7a5a,0xd0a040,0x5a90c0,0xa05070,0x708050};
 unsigned sys=sector_hash((g->system+1)*0x9e3779b9u);float system_scale=.90f+((sys>>8)%26)*.01f;int archetype=system_archetype(g->system),active=system_body_slots(g->system);
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&g->bodies[i];unsigned h=sector_hash((g->system+1)*911u+i*65537u);memset(b,0,sizeof(*b));b->seed=h;b->pos=(Vec3){900000+i*1000.f,900000,900000};
  if(i>=active)continue;
  b->type=system_body_type_slot(g->system,i);
  float angle=(sys%6283)*.001f+i*6.2831853f/fmaxf(1.f,(float)active)+((int)(h%401)-200)*.0014f;
  float distance=(archetype==SYS_HABITABLE?32000.f:archetype==SYS_DEAD?50000.f:40000.f)+i*(archetype==SYS_HABITABLE?6500.f:10500.f)+((h>>10)%8000);
  b->pos=(Vec3){sinf(angle)*distance,((int)((h>>18)%22000)-11000),STATION_Z+cosf(angle)*distance};
  if(i==0&&archetype!=SYS_ROGUE)b->pos=(Vec3){0,0,STATION_Z-125000.f-(h%22000)};
  if((archetype==SYS_BINARY&&i==1)||(archetype==SYS_TRINARY&&i>0&&i<3)){float sa=(i-1)*3.14159265f+(sys%1000)*.001f;b->pos=(Vec3){cosf(sa)*62000.f,(i-1)*9000.f,STATION_Z-98000.f+sinf(sa)*18000.f};}
  if(archetype==SYS_MOON_SWARM&&i>=2){Body *giant=&g->bodies[1];float ma=(i-2)*6.2831853f/8.f+((h>>8)&255)*.002f,mr=giant->radius+4300.f+(i-2)*1500.f;b->pos=add(giant->pos,(Vec3){cosf(ma)*mr,((i&1)?1:-1)*(500+(i%3)*350),sinf(ma)*mr});}
  float base=b->type==SUN?4200.f:b->type==GAS?3300.f:b->type==OCEAN?1500.f:1250.f;
  if(archetype==SYS_ASTEROID&&i>0)base=220.f+(h%430);
  if(archetype==SYS_MOON_SWARM&&i>=2)base=260.f+(h%420);
  if(archetype==SYS_ROGUE)base*=.72f;
  b->radius=base*(g->system==7?1.f:.70f+((h>>12)%75)*.01f);
  b->color=i==0?suns[h%10]:worlds[(h>>8)%15];
  b->accent=worlds[(h>>16)%15];
  if(archetype==SYS_RED_DWARF&&i==0){b->color=0x4050d8;b->radius=2800;}
  if(archetype==SYS_RED_DWARF&&i>0){b->color=i&1?0xb8d6df:0x91b8cc;b->accent=0xe5edf2;}
  if(archetype==SYS_DEAD&&i>0){b->color=i&1?0x423b35:0x59432e;b->accent=0x755642;}
  if(b->type==SUN)snprintf(b->name,sizeof(b->name),active>1&&((archetype==SYS_BINARY)||(archetype==SYS_TRINARY))?"%s SUN %c":"%s SUN",g->systems[g->system].name,'A'+i);
  else if(archetype==SYS_MOON_SWARM&&i>=2)snprintf(b->name,sizeof(b->name),"%s MOON %d",g->systems[g->system].name,i-1);
  else if(archetype==SYS_ASTEROID)snprintf(b->name,sizeof(b->name),"%s PLANETOID %d",g->systems[g->system].name,i);
  else snprintf(b->name,sizeof(b->name),"%s %d",g->systems[g->system].name,i+(archetype==SYS_ROGUE?1:0));
 }
 /* Reserve a clear solar sightline from the primary departure aperture. */
 for(int i=0;i<BODY_COUNT;i++){Body *b=&g->bodies[i];if(body_active(b)&&b->pos.z<STATION_Z&&sqrtf(b->pos.x*b->pos.x+b->pos.y*b->pos.y)<b->radius+1500)b->pos.x=b->radius+2000;}
 /* Lave keeps the familiar sun / ocean / rocky / gas / rocky set for the opening chapter. */
 if(g->system==7){g->bodies[1].type=OCEAN;g->bodies[1].color=0xc35f23;g->bodies[1].accent=0x4b9137;g->bodies[2].type=ROCKY;g->bodies[3].type=GAS;g->bodies[4].type=ROCKY;}
 if(opening_sol(g)&&g->system==7){
  static const char *names[]={"SOL","EARTH","MARS","JUPITER","NEPTUNE"};
  static const int types[]={SUN,OCEAN,ROCKY,GAS,GAS};
  static const unsigned colors[]={0x80dfff,0xc87535,0x416faf,0x94abc9,0xca7745};
  static const unsigned accents[]={0x80dfff,0x4b9137,0x548ab0,0x7094b2,0xac6835};
  /* Compressed playable distances. The authored Sol chart holds all eight
   * planets; these four physical bodies are the bounded flight subset. */
  static const Vec3 positions[]={{0,0,-130000},{-14000,0,8000},{24000,1800,18000},{-44000,-2200,-32000},{58000,3200,-42000}};
  for(int i=0;i<LEGACY_BODY_COUNT;i++){Body *b=&g->bodies[i];snprintf(b->name,sizeof(b->name),"%s",names[i]);b->type=types[i];b->color=colors[i];b->accent=accents[i];b->pos=positions[i];b->seed=sector_hash(2050u+i*65537u);}
 }
 /* Expand free-space geography without touching the authored opening world. */
 if(navigation_enabled(g)){Vec3 hub={0,0,STATION_Z};for(int i=0;i<BODY_COUNT;i++)if(body_active(&g->bodies[i]))g->bodies[i].pos=add(hub,mul(sub(g->bodies[i].pos,hub),1.25f));}
 /* Keep the hub corridor clear so traffic and station approaches stay readable. */
 {Vec3 hub={0,0,STATION_Z};for(int i=0;i<BODY_COUNT;i++){Body *b=&g->bodies[i];if(!body_active(b))continue;float d=length(sub(b->pos,hub)),need=b->radius+9000.f;if(d<1)b->pos=(Vec3){need,0,STATION_Z};else if(d<need)b->pos=add(hub,mul(norm(sub(b->pos,hub)),need));}}
}
/* Enter inside the wide system, on a full-circle bearing. Heading follows a
 * separate route seed, never an automatic look-at-station camera. */
static void system_arrival(Game *g,int origin){
 unsigned h=sector_hash((g->system+1)*0xc2b2ae35u+(origin+1)*7919u);
 float a=(h%6283)*.001f,dist=(navigation_enabled(g)?45000.f:15000.f)+((h>>10)%(navigation_enabled(g)?10000:5000));
 g->pos=(Vec3){sinf(a)*dist,((int)((h>>20)%6000)-3000),STATION_Z+cosf(a)*dist};
 unsigned heading=sector_hash(h^0x94d049bbu);g->yaw=(heading%6283)*.001f;g->pitch=((int)((heading>>16)%301)-150)*.001f;g->roll=0;g->speed=100;
}
static Vec3 traffic_world_point(const Game *g,int body){
 Vec3 hub=hub_position(g,0);const Body *b=&g->bodies[body];
 return add(b->pos,mul(norm(sub(hub,b->pos)),b->radius+5000));
}

/* A render-scoped snapshot: all cards share one neighbourhood search. End
 * the scope before input/save/load so mutations can never leave stale offers. */
static const Game *mission_query_owner;
static int mission_legacy_review;
static int mission_query_type[MISSION_OFFER_SLOTS];
static int mission_query_dest[MISSION_OFFER_SLOTS];
void mission_queries_end(void){mission_query_owner=0;}
void mission_queries_begin(const Game *g){
 mission_queries_end();if(!g)return;
 int nearby[256],landable[256],n=0,l=0;float range=fminf(10.f,player_ships[g->ship].range*.1f);
 for(int i=0;i<256;i++)if(i!=g->system&&distance_ly(g,g->system,i)<=range+.001f&&world_station_available(g,i,0)){
  nearby[n++]=i;for(int b=1;b<BODY_COUNT;b++)if(mission_landable_body(g,i,b)){landable[l++]=i;break;}
 }
 for(int offer=0;offer<MISSION_OFFER_SLOTS;offer++){
  mission_query_type[offer]=mission_type_for_offer(g,offer);
  if(mission_type_for_offer(g,offer)>=EXPEDITION_FIRST){mission_query_dest[offer]=expedition_destination(g,mission_type_for_offer(g,offer));continue;}
  if(mission_type_for_offer(g,offer)==MISSION_RACE){mission_query_dest[offer]=g->system;continue;}
  int explore=mission_base_type(mission_type_for_offer(g,offer))==MISSION_EXPLORATION,count=explore?l:n;
  mission_query_dest[offer]=count?(explore?landable:nearby)[(g->system*13+offer*7)%count]:g->system;
 }
 mission_query_owner=g;
}
int mission_destination(const Game *g,int offer){
 if(!g||offer<0||offer>=MISSION_OFFER_SLOTS)return -1;
 if(mission_type_for_offer(g,offer)==MISSION_RACE)return g->system;
 if(mission_query_owner==g)return mission_query_dest[offer];
 if(mission_type_for_offer(g,offer)>=EXPEDITION_FIRST)return expedition_destination(g,mission_type_for_offer(g,offer));
 int nearby[256],n=0,type=mission_type_for_offer(g,offer);float range=fminf(10.f,player_ships[g->ship].range*.1f);
 for(int i=0;i<256;i++)if(i!=g->system&&distance_ly(g,g->system,i)<=range+.001f&&world_station_available(g,i,0)){
  int playable=1;if(mission_base_type(type)==MISSION_EXPLORATION){playable=0;for(int b=1;b<BODY_COUNT;b++)if(mission_landable_body(g,i,b)){playable=1;break;}}
  if(playable)nearby[n++]=i;
 }
 return n?nearby[(g->system*13+offer*7)%n]:g->system;
}
int mission_count(const Game *g){int n=0;while(n<MISSION_OFFER_SLOTS&&mission_offer_valid(g,n,0))n++;return n;}
int jobs_active(const Game *g){return g->job_n;}
int mission_type_at(const Game *g,int system,int hub,int offer){
 if(!g||system<0||system>=256||hub<0||hub>=HUB_COUNT||offer<0||offer>=MISSION_OFFER_SLOTS)return -1;
 if(offer==5)return g->erl.referral?MISSION_RACE:MISSION_DELIVERY;
 unsigned seed=sector_hash((unsigned)(system+1)*0x9e3779b9u+(unsigned)(hub+1)*7919u);
 if(mission_legacy_review)return (int)((seed+(unsigned)offer*7u)%MISSION_RACE);
 /* Five stable distinct, eligible families. Cache only immutable board inputs,
  * so native UI drawing never repeats world/site searches every text row. */
 static const Game *owner;static int cached_system=-1,cached_hub,cached_ship,cached_guild,cached_sol,types[5];
 int required=guild_required_contract(g),sol=opening_sol(g);
 if(owner!=g||cached_system!=system||cached_hub!=hub||cached_ship!=g->ship||cached_guild!=required||cached_sol!=sol){
  for(int slot=0;slot<5;slot++){
   int raw=(int)((seed+(unsigned)slot*7u)%(MISSION_TYPES-1));
   for(int attempt=0;attempt<MISSION_TYPES-1;attempt++){
    int draw=(raw+attempt*7)%(MISSION_TYPES-1),type=draw>=MISSION_RACE?draw+1:draw,duplicate=0;
    for(int k=0;k<slot;k++)duplicate|=types[k]==type;
    if(duplicate||type>=EXPEDITION_FIRST&&expedition_destination(g,type)<0)continue;
    types[slot]=type;break;
   }
  }
  if(required>=0){int found=0;for(int slot=0;slot<5;slot++)found|=types[slot]==required;if(!found)types[4]=required;}
  owner=g;cached_system=system;cached_hub=hub;cached_ship=g->ship;cached_guild=required;cached_sol=sol;
 }
 return types[offer];
}
int mission_type_for_offer(const Game *g,int offer){return g&&offer>=0&&offer<MISSION_OFFER_SLOTS?(mission_query_owner==g?mission_query_type[offer]:mission_type_at(g,g->system,g->station_variant,offer)):-1;}
const char *mission_name(int type){const ExpeditionDef *xp=expedition_def(type);if(xp)return xp->name;static const char *names[MISSION_TYPES]={
 "Cargo delivery","Bounty hunt","Exploration survey","Pilot rescue","Covert delivery",
 "Convoy escort","Beacon repair","Salvage recovery","Wildlife conservation","Anomaly investigation",
 "Mining contract","Distress response","System patrol","Cargo recovery","Missing-person search",
 "Trade procurement","Mobile mechanic","Black-box recovery","Planetary samples","Listening-post inquiry",
 "Police assistance","Passenger transport","Memorial delivery","Multi-stop logistics","Freighter defence","Derelict survey","Space race"};
 return type>=0&&type<MISSION_TYPES?names[type]:"Unknown mission";
}
const char *mission_requirement(int type){const ExpeditionDef *xp=expedition_def(type);if(xp)return xp->gear==3?"WEAPON / REFINERY / 3T":xp->gear?"WEAPON / FREE HOLD":"FREE HOLD / FIELD WORK";
 static const char *req[MISSION_TYPES]={
  "1T FREE CARGO SPACE","WEAPON REQUIRED","PLANET SCANNER REQUIRED","PASSENGER BERTH RESERVED","ILLEGAL CARGO / LAW RISK",
  "RENDEZVOUS AFTER LAUNCH","SCANNER REQUIRED","1T FREE CARGO SPACE","DO NOT HARM WILDLIFE","PLANET SCANNER REQUIRED",
  "MINING LASER REQUIRED","PASSENGER BERTH MAY BE NEEDED","SCANNER REQUIRED","1T FREE CARGO SPACE","COMMS REQUIRED",
  "1T FREE CARGO SPACE","COMMS REQUIRED","1T FREE CARGO SPACE","PLANET SCANNER REQUIRED","SCANNER REQUIRED",
  "WEAPON REQUIRED","PASSENGER BERTH RESERVED","1T FREE CARGO SPACE","1T FREE CARGO SPACE","WEAPON REQUIRED","PLANET SCANNER REQUIRED","ERL REFERRAL / ONE ATTEMPT"};
 return type>=0&&type<MISSION_TYPES?req[type]:"VALIDATED EQUIPMENT";
}
const char *mission_difficulty_name(int risk){return risk>=5?"EXTREME":risk>=4?"DANGEROUS":risk>=3?"RISKY":"LOW RISK";}
const char *mission_brief(const Game *g,int offer){
 /* Authored banks from manuscript Vol II; flavour-linked singles (Hungry Pad / Listen Twice / Boring Lies).
  * Open Channel flags tint copy without spoiling chapter reveals. */
 static char out[96];
 int type=mission_type_for_offer(g,offer),base=mission_base_type(type),risk=danger_rating(g,g->system),wealth=prosperity(g,g->system);
 int dest_id=mission_destination(g,offer);const char *dest=dest_id>=0?g->systems[dest_id].name:"nearby space";
 int pick=(g->system*7+offer*3)&3;int flags=g->campaign_stage>=6?g->saga_flags:0;
 if(type>=EXPEDITION_FIRST){snprintf(out,sizeof(out),"%s",expedition_def(type)->stages[0].objective);return out;}
 if(type==MISSION_RACE){Job entry={0};entry.origin=g->system;entry.target=race_contract_target(g,g->station_variant);snprintf(out,sizeof(out),"%s. One attempt. Meet the grid in %s.",race_contract_goal(&entry),dest);return out;}
 if(type>=5){
  static const char *brief[MISSION_TYPES-5]={
   "Meet the outbound convoy near %s and see its flagship safely through.",
   "Survey a damaged relay near %s and deliver the repair package.",
   "Recover the marked component from a wreck near %s and return it intact.",
   "Observe protected fauna near %s without opening fire.",
   "Trace an anomalous signal around %s and record the planetary evidence.",
   "Extract the contracted mineral near %s and return the sealed sample.",
   "Investigate a distress call near %s. Its cause is not yet known.",
   "Scan the assigned patrol points around %s and identify any threat.",
   "Recover the network-marked containers near %s and deliver them intact.",
   "Follow the last verified contact to %s and identify the missing pilot.",
   "Acquire the requested trade lot and deliver it to %s Hub.",
   "Meet a mobile mechanic near %s and assist with their disabled client.",
   "Locate the named wreck near %s and recover its flight recorder.",
   "Land on a surveyed world near %s and collect the requested samples.",
   "Inspect the listening post near %s and recover its stored transmission.",
   "Help local Law near %s identify and stop a genuinely wanted vessel.",
   "Carry a named passenger safely to %s Hub.",
   "Deliver a private memorial package to its recipient at %s Hub.",
   "Complete the first verified logistics leg to %s Hub.",
   "Defend a scheduled heavy freighter arrival near %s.",
   "Survey the exterior of a derelict capital ship near %s."};
  snprintf(out,sizeof(out),brief[type-5],dest);return out;
 }
 if(base==MISSION_DELIVERY){
  if(flags&2&&pick==0){snprintf(out,sizeof(out),"Clinic meal packs to %s; settlements still prepping loud.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),"Protein crates to %s; kitchen ran out of polite excuses.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"School meal packs to %s dock. Quiet in the good way.",dest);
  else if(pick==2)snprintf(out,sizeof(out),wealth>=4?"Fresh produce for %s; the harvest festival ran short.":"Ration packs to %s; hungry pilots make bad navigators.",dest);
  else snprintf(out,sizeof(out),wealth>=4?"Fresh cargo to %s; market is hungry.":"Essential cargo to %s; margins are thin.",dest);
 }else if(base==MISSION_BOUNTY){
  if(flags&1&&pick==0){snprintf(out,sizeof(out),"Clear a raider shaking quiet couriers near %s.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),risk>=4?"Raiders active near %s; bounty is live.":"Track one wanted hull beyond %s.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Marked hull past %s. Clear it. Do not become the next bulletin.",dest);
  else snprintf(out,sizeof(out),"Pirate taking tolls on the %s approach; remove the toll.",dest);
 }else if(base==MISSION_EXPLORATION){
  if(flags&4&&pick==0){snprintf(out,sizeof(out),"Listen twice near %s; markers, not ownership claims.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),"Map a world near %s. Approach its surface and request a scan.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Planetary survey at %s. Observe the world from close orbit.",dest);
  else snprintf(out,sizeof(out),"Survey the unusual worlds around %s.",dest);
 }else if(base==MISSION_RESCUE){
  if(pick==0)snprintf(out,sizeof(out),risk>=4?"Distress beacon in hostile lanes near %s.":"A civilian beacon is waiting near %s.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Beacon in the same sector as %s; stamp before shrug.",dest);
  else snprintf(out,sizeof(out),"A tender crew near %s needs a lift back to its station.",dest);
 }else{
  if(flags&1&&pick==0){snprintf(out,sizeof(out),"Sealed favour to %s; agricultural sensors on the label.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),"Quiet courier to %s; boring lie if asked. Local law is watching.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Sealed crate to %s. Do not open for curiosity.",dest);
  else snprintf(out,sizeof(out),"Quiet courier run to %s; local law is watching.",dest);
 }
 return out;
}
int mission_risk(const Game *g,int offer){if(mission_type_for_offer(g,offer)==MISSION_RACE)return 2;int id=mission_destination(g,offer);if(id<0)return 1;int type=mission_type_for_offer(g,offer),base=mission_base_type(type),r=danger_rating(g,id);if(base==MISSION_BOUNTY||base==MISSION_SMUGGLING)r++;if(type==MISSION_FREIGHTER_DEFENCE||type==MISSION_DERELICT)r++;return r>5?5:r<1?1:r;}
int mission_reward(const Game *g,int offer){int id=mission_destination(g,offer),type=mission_type_for_offer(g,offer),base=mission_base_type(type);if(id<0)return 0;if(type>=EXPEDITION_FIRST)return 5000+expedition_def(type)->count*1200+prosperity(g,g->system)*300;if(type==MISSION_RACE){int reward=5000+prosperity(g,g->system)*500;return race_contract_target(g,g->station_variant)==RC_DISTANCE&&(g->system&1)?reward*2:reward;}const int bonus[]={0,700,500,900,1300};int risk=mission_risk(g,offer);return 1000+(int)(distance_ly(g,g->system,id)*200)+prosperity(g,g->system)*150+bonus[base]+(type>=5?500:0)+(risk-1)*125;}
int trade_job_is(const Job *j);
static int job_marked(const Job *j,const Game *g,int id){int base=mission_base_type(j->type);if(j->dest!=g->system)return 0;if(trade_job_is(j))return id==BODY_COUNT+1+j->target;if(base==MISSION_RACE)return id==RACE_TARGET_ID;if(base==MISSION_DELIVERY||base==MISSION_SMUGGLING)return id==0;if(base==MISSION_EXPLORATION)return id==j->item+1;if(base==MISSION_RESCUE&&j->stage)return id==0;if((base==MISSION_BOUNTY||base==MISSION_RESCUE)&&j->target>=0)return id==BODY_COUNT+1+j->target;return 0;}
static void jobs_sync(Game *g){if(g->job_n<=0){g->job_n=0;g->job_sel=0;g->contract=-1;g->mission_target=-1;g->mission_stage=0;g->contract_time=0;g->contract_reward=0;return;}if(g->job_sel<0||g->job_sel>=g->job_n)g->job_sel=0;Job *j=&g->jobs[g->job_sel];g->contract=j->dest;g->mission_type=j->type;g->mission_stage=j->stage;g->mission_target=j->target;g->mission_item=j->item;g->mission_origin=j->origin;g->contract_reward=j->reward;g->contract_time=j->time;}
static void jobs_from_legacy(Game *g){if(g->job_n>0||g->contract<0)return;g->jobs[0].dest=g->contract;g->jobs[0].type=g->mission_type;g->jobs[0].stage=g->mission_stage;g->jobs[0].target=g->mission_target;g->jobs[0].item=g->mission_item;g->jobs[0].origin=g->mission_origin;g->jobs[0].reward=g->contract_reward;g->jobs[0].time=g->contract_time>0?g->contract_time:300;g->job_n=1;g->job_sel=0;}
static void job_remove(Game *g,int i){if(i<0||i>=g->job_n)return;expedition_remove(g,i);for(int k=i;k<g->job_n-1;k++)g->jobs[k]=g->jobs[k+1];g->job_n--;if(g->job_sel>=g->job_n)g->job_sel=g->job_n-1;if(g->job_sel<0)g->job_sel=0;jobs_sync(g);}
/* Ambient cargo exchanges use the existing saved Job record. Stage one is
 * never used by ordinary procurement deliveries, so it safely identifies a
 * personal trader appointment without changing the V37 save layout. */
int trade_job_is(const Job *j){return j&&j->type==MISSION_PROCUREMENT&&j->stage==1&&j->target>=0&&j->target<NPC_COUNT;}
static int trade_job_qty(const Job *j){return trade_job_is(j)?j->reward&3:0;}
static int trade_job_need(const Job *j){return trade_job_is(j)?j->item-1:-1;}
static int trade_job_reward(const Job *j){return trade_job_is(j)?(j->reward>>2)-1:-1;}
static int trade_job_pack(int reward,int qty){return ((reward+1)<<2)|(qty&3);}
static int trade_job_slot(const Game *g,int system,int npc){for(int i=0;i<g->job_n;i++)if(trade_job_is(&g->jobs[i])&&g->jobs[i].dest==system&&g->jobs[i].target==npc)return i;return -1;}
void trade_contact_name_at(int system,int npc,char *out,int cap){
 static const char *given[]={"MIRA","ORIN","TALA","JUNO","SEN","KITO","VARA","PELL","NARA","CORIN","ZIA","ROOK"};
 static const char *call[]={"CANDLE","WAYFARER","COPPER FOX","BLUE LANTERN","MARIGOLD","NIGHT JAR","STARLING","PATCHWORK"};
 unsigned seed=(unsigned)(system*37+npc*19+11);snprintf(out,cap,"%s / %s",given[seed%12],call[(seed/7)%8]);
}
void trade_contact_name(const Game *g,int npc,char *out,int cap){trade_contact_name_at(g?g->system:0,npc,out,cap);}
int accept_trader_offer(Game *g){
 if(!g||!g->trader_offer_active||g->trader_offer_system!=g->system)return 0;
 int npc=g->trader_offer_npc,need=g->trader_offer_need,reward=g->trader_offer_reward,qty=g->trader_offer_qty;
 if(npc<0||npc>=NPC_COUNT||need<0||need>=GOODS||reward<0||reward>=GOODS||qty<1||qty>3){g->trader_offer_active=0;message(g,"The trader's manifest is invalid. No appointment was added.");return 0;}
 jobs_from_legacy(g);int existing=trade_job_slot(g,g->system,npc);
 if(existing>=0){g->job_sel=existing;jobs_sync(g);g->trader_offer_active=0;speak(g,VOICE_CONTACT,"Our exchange is already in your log. Bring the agreed cargo and hail me here when you're ready.");g->voice_role=TRADERS;return 1;}
 if(g->job_n>=MISSION_SLOTS){speak(g,VOICE_CONTACT,"Your mission log is full. I cannot register this exchange until you clear another commitment.");g->voice_role=TRADERS;message(g,"MISSION LOG FULL: cargo exchange was not accepted.");return 0;}
 Job *j=&g->jobs[g->job_n];*j=(Job){g->system,MISSION_PROCUREMENT,1,npc,need+1,g->system,trade_job_pack(reward,qty),300};
 g->job_sel=g->job_n++;g->destination=g->system;jobs_sync(g);g->trader_offer_active=0;g->npc[npc].name_known=1;
 char trader[40],title[64],note[112];trade_contact_name(g,npc,trader,sizeof(trader));
 snprintf(title,sizeof(title),"%d%c %s FOR %d%c %s",qty,goods[need].unit,goods[need].name,qty,goods[reward].unit,goods[reward].name);
 mission_acceptance_record(g,MISSION_PROCUREMENT,0,g->system,title,trader,TRADERS,g->system*NPC_COUNT+NPC_ID_MIN+npc);
 snprintf(note,sizeof(note),"EXCHANGE ACCEPTED: meet %s in %s.",trader,g->systems[g->system].name);message(g,note);
 speak(g,VOICE_CONTACT,"Good. I've registered the exchange. Your log has my name, this system, and the exact cargo. Hail me again when you're ready.");g->voice_role=TRADERS;g->cue=SFX_SELECT;return 1;
}
static int finish_trade_job(Game *g,int slot){
 if(!g||slot<0||slot>=g->job_n||!trade_job_is(&g->jobs[slot]))return 0;Job *j=&g->jobs[slot];
 int need=trade_job_need(j),reward=trade_job_reward(j),qty=trade_job_qty(j);if(need<0||need>=GOODS||reward<0||reward>=GOODS||qty<1||qty>3)return 0;
 if(g->cargo[need]<qty){char line[160];snprintf(line,sizeof(line),"Not yet, Commander. We agreed on %d%c of %s; your hold has %d. The appointment stays open.",qty,goods[need].unit,goods[need].name,g->cargo[need]);speak(g,VOICE_CONTACT,line);g->voice_role=TRADERS;return 1;}
 int used=cargo_used(g)-(goods[need].unit=='t'?qty:0)+(goods[reward].unit=='t'?qty:0);
 if(used>cargo_capacity_for(g,reward)){speak(g,VOICE_CONTACT,"I have your cargo ready, but it will not fit in your hold. Make room and hail me again; I will wait.");g->voice_role=TRADERS;return 1;}
 char trader[40],title[64],line[176],note[112];trade_contact_name_at(j->dest,j->target,trader,sizeof(trader));
 g->cargo[need]-=qty;g->cargo[reward]+=qty;
 snprintf(title,sizeof(title),"%d%c %s FOR %d%c %s",qty,goods[need].unit,goods[need].name,qty,goods[reward].unit,goods[reward].name);
 mission_receipt_record(g,RECEIPT_BOARD,MISSION_PROCUREMENT,0,j->origin,title,trader);
 g->mission_result=1;g->last_mission_type=MISSION_PROCUREMENT;g->last_mission_system=j->dest;job_remove(g,slot);
 snprintf(line,sizeof(line),"Done. I have your %d%c of %s, and you have my %d%c of %s. Clean trade, Commander.",qty,goods[need].unit,goods[need].name,qty,goods[reward].unit,goods[reward].name);speak(g,VOICE_CONTACT,line);g->voice_role=TRADERS;
 snprintf(note,sizeof(note),"EXCHANGE COMPLETE: -%d %s / +%d %s",qty,goods[need].name,qty,goods[reward].name);message(g,note);g->cue=SFX_TALK;return 1;
}
static void fail_trade_job(Game *g,int slot,const char *title,const char *notice){
 if(!g||slot<0||slot>=g->job_n||!trade_job_is(&g->jobs[slot]))return;Job *j=&g->jobs[slot];char trader[40];trade_contact_name_at(j->dest,j->target,trader,sizeof(trader));
 mission_receipt_record(g,RECEIPT_FAILED,MISSION_PROCUREMENT,0,j->origin,title?title:"EXCHANGE CANCELLED",trader);g->mission_result=-1;g->last_mission_type=MISSION_PROCUREMENT;g->last_mission_system=j->dest;job_remove(g,slot);message(g,notice?notice:"Cargo exchange cancelled. No goods changed hands.");g->cue=SFX_ALERT;
}
static void distress_contact_name(const Game *g,int npc,char *out,int cap){
 if(!out||cap<=0)return;if(!g||npc<0||npc>=NPC_COUNT){snprintf(out,cap,"DISTRESS VESSEL");return;}
 const NPC *n=&g->npc[npc];
 int serial=((BODY_COUNT+1+npc)*17+g->system*7)&255;
 if(n->freighter)snprintf(out,cap,"CAPITAL MERCHANT %02d",serial%100);else snprintf(out,cap,"MERCHANT %c-%02d",'A'+serial%26,serial%100);
}
int accept_distress_call(Game *g,int caller){
 if(!g||caller<0||caller>=NPC_COUNT||!g->npc[caller].alive)return 0;
 jobs_from_legacy(g);
 if(g->job_n>=MISSION_SLOTS){speak(g,VOICE_CONTACT,"Your mission log is full. I understand, but please relay our beacon if you can.");g->voice_role=TRADERS;message(g,"MISSION LOG FULL: distress call was not accepted.");return 0;}
 for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==MISSION_DISTRESS&&g->jobs[i].dest==g->system&&g->jobs[i].item==caller+1){speak(g,VOICE_CONTACT,"You already have our beacon in your log. The marked attacker is still on your scanner.");g->voice_role=TRADERS;return 1;}
 int attacker=-1;float nearest=1e30f;for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive&&g->npc[i].role==PIRATES){float d=length(sub(g->npc[i].pos,g->npc[caller].pos));if(d<nearest){nearest=d;attacker=i;}}
 if(attacker<0){speak(g,VOICE_CONTACT,"The attackers have broken away. Thank you for answering; we can make it from here.");g->voice_role=TRADERS;message(g,"Distress vessel is clear. No mission added.");return 0;}
 int risk=danger_rating(g,g->system),reward=1000+risk*250+prosperity(g,g->system)*100;
 Job *j=&g->jobs[g->job_n];*j=(Job){g->system,MISSION_DISTRESS,0,attacker,caller+1,g->system,reward,300};
 g->job_sel=g->job_n++;g->destination=g->system;jobs_sync(g);
 g->npc[attacker].target=-2;g->attacked=4;g->npc[caller].name_known=1;
 char caller_name[32];distress_contact_name(g,caller,caller_name,sizeof(caller_name));
 mission_acceptance_record(g,MISSION_DISTRESS,reward,g->system,"DISTRESS RESPONSE",caller_name,TRADERS,g->system*NPC_COUNT+NPC_ID_MIN+caller);
 speak(g,VOICE_CONTACT,"Thank you, Commander. The attacker is marked. Drive them off and give us room to escape.");g->voice_role=TRADERS;
 char note[96];snprintf(note,sizeof(note),"MISSION ACCEPTED: protect %s. Reward %.1f U.",caller_name,reward*.1f);message(g,note);g->cue=SFX_SELECT;return 1;
}
static void mission_finish_slot(Game *g,int i,const char *notice){
 if(i<0||i>=g->job_n)return;Job *j=&g->jobs[i];int base=mission_base_type(j->type),amount=j->reward,type=j->type,origin=j->origin,dest=j->dest;
 char distress_sender[32];const char *sender=type==MISSION_RACE?"RACE CONTROL":expedition_sender(type);
 if(type==MISSION_DISTRESS){distress_contact_name(g,j->item-1,distress_sender,sizeof(distress_sender));sender=distress_sender;}
 guild_event(g,base==MISSION_DELIVERY?GUILD_DELIVERY:base==MISSION_RESCUE?GUILD_RESCUE:0);if(base!=MISSION_SMUGGLING)social_emit(g,SB_JOB);g->credits+=amount;
 mission_receipt_record(g,RECEIPT_BOARD,type,amount,origin,mission_name(type),sender);g->mission_result=1;g->last_mission_type=type;g->last_mission_system=dest;job_remove(g,i);message(g,notice);
}
static void mission_fail_slot(Game *g,int i,const char *title,const char *notice){
 if(i<0||i>=g->job_n)return;Job *j=&g->jobs[i];int type=j->type,origin=j->origin,dest=j->dest;char caller[32];const char *sender=expedition_sender(type);
 if(type==MISSION_DISTRESS){distress_contact_name(g,j->item-1,caller,sizeof(caller));sender=caller;}
 mission_receipt_record(g,RECEIPT_FAILED,type,0,origin,title?title:"MISSION FAILED",sender);g->mission_result=-1;g->last_mission_type=type;g->last_mission_system=dest;job_remove(g,i);message(g,notice?notice:"Mission failed.");g->cue=SFX_ALERT;
}
static int fail_distress_contracts(Game *g,int system,const char *title,const char *notice){int failed=0;for(int i=0;i<g->job_n;)if(g->jobs[i].type==MISSION_DISTRESS&&g->jobs[i].dest==system){mission_fail_slot(g,i,title,notice);failed++;}else i++;return failed;}
/* Board contracts are untimed. Keep the legacy duration field for save compatibility. */
void mission_timers(Game *g,float dt){(void)dt;jobs_from_legacy(g);jobs_sync(g);}
const char *mission_objective_at(const Game *g,int slot){static char out[160];if(slot<0||slot>=g->job_n)return "No active mission";const Job *j=&g->jobs[slot];if(expedition_stage(g,slot))return expedition_objective(g,slot);int base=mission_base_type(j->type);if(trade_job_is(j)){int need=trade_job_need(j),reward=trade_job_reward(j),qty=trade_job_qty(j);char trader[40];trade_contact_name_at(j->dest,j->target,trader,sizeof(trader));if(g->system!=j->dest)snprintf(out,sizeof(out),"Return to %s and hail %s",g->systems[j->dest].name,trader);else snprintf(out,sizeof(out),"Hail %s with %d%c %s; receive %d%c %s",trader,qty,goods[need].unit,goods[need].name,qty,goods[reward].unit,goods[reward].name);return out;}if(g->system!=j->dest){snprintf(out,sizeof(out),"Jump to %s",g->systems[j->dest].name);return out;}if(j->type==MISSION_DISTRESS){char caller[32];distress_contact_name(g,j->item-1,caller,sizeof(caller));snprintf(out,sizeof(out),"Protect %s; stop the marked attacker",caller);return out;}if(base==MISSION_RACE){snprintf(out,sizeof(out),race_is_active(g)?g->race.arcade.rules.course==RA_RINGS?"%s. Follow the ordered rings.":"%s. Follow the course.":"%s. Fly to the start grid.",race_contract_goal(j));return out;}if(base==MISSION_BOUNTY){snprintf(out,sizeof(out),"Identify and stop the marked hostile");return out;}if(base==MISSION_EXPLORATION){snprintf(out,sizeof(out),"Approach and scan %s",g->bodies[j->item].name);return out;}if(base==MISSION_RESCUE){snprintf(out,sizeof(out),j->stage?"Return your passenger to the station":"Hail the marked contact within 600 m");return out;}if(base==MISSION_DELIVERY){snprintf(out,sizeof(out),"Dock with the reserved mission cargo aboard");return out;}if(base==MISSION_SMUGGLING){snprintf(out,sizeof(out),"Dock with the sealed cargo; avoid Law scans");return out;}snprintf(out,sizeof(out),"Dock at %s",station_name(g));return out;}
const char *mission_objective(const Game *g){return mission_objective_at(g,g->job_sel);}
int is_mission_target(const Game *g,int id){for(int i=0;i<g->job_n;i++)if(job_marked(&g->jobs[i],g,id))return 1;if(g->job_n==0&&g->contract>=0){Job tmp={g->contract,g->mission_type,g->mission_stage,g->mission_target,g->mission_item,g->mission_origin,g->contract_reward,g->contract_time};return job_marked(&tmp,g,id);}return 0;}
int mission_interact(Game *g,int id){if(expedition_hail(g,id))return 1;int n=id-BODY_COUNT-1;if(n<0||n>=NPC_COUNT)return 0;for(int i=0;i<g->job_n;i++){Job *j=&g->jobs[i];if(j->type==MISSION_DISTRESS||mission_base_type(j->type)!=MISSION_RESCUE||j->stage||!job_marked(j,g,id))continue;if(length(sub(g->pos,g->npc[n].pos))>600){message(g,"Move within 600 m of the mission contact.");return 0;}j->stage=1;g->job_sel=i;jobs_sync(g);g->cue=SFX_SCAN;message(g,j->type==MISSION_MECHANIC?"Mechanic linked up. Return to the local station.":"Contact aboard. Return to the local station.");return 1;}return 0;}
int mission_offer_active(const Game *g,int offer){int dest=mission_destination(g,offer),type=mission_type_for_offer(g,offer);if(dest<0||!mission_offer_valid(g,offer,0))return 0;for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==type&&(g->jobs[i].dest==dest||expedition_stage(g,i)&&g->expeditions[i].systems[1]==(unsigned)dest))return 1;return 0;}
int abandon_mission(Game *g,int slot){jobs_from_legacy(g);if(slot<0||slot>=g->job_n)return 0;if(trade_job_is(&g->jobs[slot])){fail_trade_job(g,slot,"EXCHANGE CANCELLED","Cargo exchange abandoned. No goods changed hands.");return 1;}if(g->jobs[slot].type==MISSION_DISTRESS){mission_fail_slot(g,slot,"DISTRESS CALL ABANDONED","Distress response abandoned. No payment issued.");return 1;}g->mission_result=-1;g->last_mission_type=g->jobs[slot].type;g->last_mission_system=g->jobs[slot].dest;int base=mission_base_type(g->jobs[slot].type),crate=base==MISSION_DELIVERY?0:base==MISSION_SMUGGLING?6:-1;if(crate>=0&&g->cargo[crate]>0)g->cargo[crate]--;if(expedition_stage(g,slot)){ExpeditionState *e=&g->expeditions[slot];int repay=(int)e->advance-(int)e->spent;if(repay>0){if(repay>g->credits)repay=g->credits;g->credits-=repay;}}g->credits+=100;char note[96];snprintf(note,sizeof(note),"Abandoned: %s. Resources cleared.",mission_name(g->jobs[slot].type));job_remove(g,slot);message(g,note);g->cue=SFX_UI;return 1;}
int accept_mission(Game *g,int offer){if(!g->docked){message(g,"Dock to accept mission work.");return 0;}jobs_from_legacy(g);if(g->job_n>=MISSION_SLOTS){message(g,"Mission log full (5/5). Complete a job first.");return 0;}int destination=-1;if(offer<0||offer>=mission_count(g)||!mission_offer_valid(g,offer,&destination)||destination<0)return 0;int type=mission_type_for_offer(g,offer),base=mission_base_type(type);if(mission_offer_active(g,offer)){message(g,"Mission in progress.");return 0;}if(g->credits<100){message(g,"Contract deposit: 10 units.");return 0;}if((base==MISSION_DELIVERY||base==MISSION_SMUGGLING)&&cargo_used(g)>=cargo_capacity(g)){message(g,"1T free cargo space required.");return 0;}if(type==MISSION_RACE){if(!g->erl.referral){message(g,"Win Torr Bell's ERL test first.");return 0;}if(g->erl_local.progress.board_until[g->system][g->station_variant]>social_now(g)){message(g,"Race entry used. Next board refresh within 20 minutes.");return 0;}for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==EXPEDITION_FIRST+39){message(g,"Finish or abandon your championship entry first.");return 0;}}if(type>=EXPEDITION_FIRST){if(!expedition_can_accept(g,type,destination))return 0;g->credits-=100;return expedition_start(g,type,destination,mission_reward(g,offer));}g->credits-=100;Job *j=&g->jobs[g->job_n];
 /* Mission validation already proved that a landable destination exists. Pick
  * from that same canonical layout so contracts never name an inactive slot. */
 int item=1;for(int step=0;step<BODY_COUNT;step++){int b=1+(destination+step)%(BODY_COUNT-1);if(mission_landable_body(g,destination,b)){item=b;break;}}
 j->dest=destination;j->type=type;j->origin=g->system;j->stage=0;j->target=-1;j->item=base==MISSION_RACE?g->station_variant+1:item;j->time=300;j->reward=mission_reward(g,offer);if(type==MISSION_RACE)j->target=race_contract_target(g,g->station_variant);g->job_sel=g->job_n;g->job_n++;g->destination=destination;if(base==MISSION_DELIVERY)g->cargo[0]++;else if(base==MISSION_SMUGGLING)g->cargo[6]++;jobs_sync(g);char note[96];if(base==MISSION_SMUGGLING)snprintf(note,sizeof(note),"Covert contract accepted. Law may scan the sealed cargo.");else snprintf(note,sizeof(note),"%s accepted. %s. Log %d/%d.",mission_name(j->type),mission_difficulty_name(mission_risk(g,offer)),g->job_n,MISSION_SLOTS);if(base==MISSION_RACE)race_prepare(g);message(g,note);g->cue=SFX_SELECT;return 1;}
static void npc_blueprint(Game *g,NPC *n,int i){
 const char *traders[]={"COBRA MK 3","PYTHON","BOA","TRANSPORTER","SHUTTLE","MORAY"};
 const char *pirates[]={"MAMBA","KRAIT","ASP MK 2","FER DE LANCE","SIDEWINDER","BUSHMASTER"};
 const char *explorers[]={"ADDER","GECKO","OPHIDIAN","CHAMELEON","COBRA MK 1","IGUANA"};
 int variant=(i*7+g->system)%6; n->freighter=i<36&&i%12==8;n->scale=1;n->freight_style=(g->system+i/12)%3;n->cruise=n->freighter?60+(sector_hash(g->system*71u+i)%41):650;n->health=n->freighter?900:n->role==LAW?110:80;
 if(n->freighter){n->role=TRADERS;n->mesh=mesh_id(i==20?"ANACONDA":"BOA");}
 else n->mesh=mesh_id(n->role==TRADERS?traders[variant]:n->role==PIRATES?pirates[variant]:n->role==LAW?"VIPER":explorers[variant]);
 n->radius=30;for(int j=0;j<meshes[n->mesh].vertices;j++)n->radius=fmaxf(n->radius,length(meshes[n->mesh].v[j])*n->scale);
 if(n->freighter)n->radius=length(freight_extent(n));
}
