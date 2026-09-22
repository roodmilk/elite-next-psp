static unsigned sector_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
int prosperity(const Game *g,int system){const int wealth[8]={5,4,2,3,3,5,4,2};return wealth[g->systems[system].economy];}
void system_bodies(Game *g){
 /* Four orbital templates so neighbouring systems do not share the same skyline. */
 const Vec3 templates[4][BODY_COUNT]={
  {{18000,7000,42000},{-5000,2500,14000},{8500,-2000,23000},{-22000,-4000,38000},{12000,5000,-24000}},
  {{22000,-3000,28000},{6000,4000,9000},{-14000,1000,18000},{8000,-6000,32000},{-18000,2000,-16000}},
  {{12000,9000,36000},{-9000,-1500,11000},{16000,500,19000},{-6000,3500,44000},{4000,-4500,-28000}},
  {{25000,2000,30000},{3000,5500,16000},{-11000,-3500,25000},{19000,1000,12000},{-8000,6000,-22000}}
 };
 const float radii[BODY_COUNT]={4200,1700,1100,3600,1400};
 /* World-type permutations so each system cycles ocean/rocky/gas differently. */
 const int world_perm[6][4]={{OCEAN,ROCKY,GAS,ROCKY},{ROCKY,GAS,OCEAN,ROCKY},{GAS,OCEAN,ROCKY,ROCKY},{ROCKY,OCEAN,ROCKY,GAS},{OCEAN,GAS,ROCKY,ROCKY},{ROCKY,ROCKY,OCEAN,GAS}};
 const unsigned suns[]={0x80dfff,0x66c8ff,0x526eff,0xf4f4ff,0xffc88a,0xc88cff,0x9ee8ff,0xd8e8ff,0xffa060,0xb0ffe0};
 const unsigned worlds[]={0xc97535,0x91b45c,0x8763b5,0x7ebfc4,0xb87775,0xadc2ce,0xd4a574,0x5a8f6a,0x6b5b95,0xc45c5c,0x3d7a5a,0xd0a040,0x5a90c0,0xa05070,0x708050};
 unsigned sys=sector_hash((g->system+1)*0x9e3779b9u);float system_scale=.68f+((sys>>8)%70)*.01f;float system_tilt=((int)((sys>>20)%3200)-1600)*.00014f;
 int tmpl=(int)((sys>>2)%4),perm=(int)((sys>>6)%6);
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&g->bodies[i];unsigned h=sector_hash((g->system+1)*911u+i*65537u);b->seed=h;
  b->pos=templates[tmpl][i];
  float angle=(h%6283)*.001f+(sys%2800)*.001f+i*.27f+g->system*.11f;
  float radial=system_scale*(.78f+((h>>12)%48)*.01f);
  b->pos.x*=radial;b->pos.z*=radial;
  float c=cosf(angle),s=sinf(angle);
  b->pos=(Vec3){b->pos.x*c+b->pos.z*s,b->pos.y*radial+(int)((h>>18)%11000)-5500+system_tilt*b->pos.z,-b->pos.x*s+b->pos.z*c};
  b->radius=radii[i]*(g->system==7?1:.62f+((h>>12)%90)*.01f);
  b->type=i==0?SUN:world_perm[perm][(i-1)&3];
  b->color=i==0?suns[h%10]:worlds[(h>>8)%15];
  b->accent=worlds[(h>>16)%15];
  if(g->system==7&&i==1){b->type=OCEAN;b->color=0xc35f23;b->accent=0x4b9137;}
  snprintf(b->name,sizeof(b->name),"%s %s",g->systems[g->system].name,i==0?"SUN":i==1?"I":i==2?"II":i==3?"III":"IV");
 }
 /* Lave keeps the familiar sun / ocean / rocky / gas / rocky set for the opening chapter. */
 if(g->system==7){g->bodies[1].type=OCEAN;g->bodies[1].color=0xc35f23;g->bodies[1].accent=0x4b9137;g->bodies[2].type=ROCKY;g->bodies[3].type=GAS;g->bodies[4].type=ROCKY;}
 /* Keep the hub corridor clear so traffic and station approaches stay readable. */
 {Vec3 hub={0,0,STATION_Z};for(int i=1;i<BODY_COUNT;i++){Body *b=&g->bodies[i];float d=length(sub(b->pos,hub)),need=b->radius+9000.f;if(d<1)b->pos=(Vec3){need,0,STATION_Z};else if(d<need)b->pos=add(hub,mul(norm(sub(b->pos,hub)),need));}}
}
int mission_destination(const Game *g,int offer){int n=0;for(int i=0;i<256;i++)if(i!=g->system&&distance_ly(g,g->system,i)<=10.0f){if(n++==offer)return i;}return -1;}
int mission_count(const Game *g){int max=1+prosperity(g,g->system),n=0;while(n<max&&mission_destination(g,n)>=0)n++;return n;}
int jobs_active(const Game *g){return g->job_n;}
int mission_type_for_offer(const Game *g,int offer){return (g->system+offer)%MISSION_TYPES;}
const char *mission_name(int type){static const char *names[]={"Food delivery","Pirate hunt","Exploration scan","Pilot rescue","Covert delivery"};return type>=0&&type<MISSION_TYPES?names[type]:"Unknown mission";}
const char *mission_brief(const Game *g,int offer){
 /* Authored banks from manuscript Vol II — flavour-linked singles (Hungry Pad / Listen Twice / Boring Lies).
  * Open Channel flags tint copy without spoiling chapter reveals. */
 static char out[96];
 int type=mission_type_for_offer(g,offer),risk=danger_rating(g,g->system),wealth=prosperity(g,g->system);
 int dest_id=mission_destination(g,offer);const char *dest=dest_id>=0?g->systems[dest_id].name:"nearby space";
 int pick=(g->system*7+offer*3)&3;int flags=g->campaign_stage>=6?g->saga_flags:0;
 if(type==MISSION_DELIVERY){
  if(flags&2&&pick==0){snprintf(out,sizeof(out),"Clinic softpacks to %s — settlements still prepping loud.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),"Protein crates to %s; kitchen ran out of polite excuses.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"School meal packs to %s dock. Quiet in the good way.",dest);
  else if(pick==2)snprintf(out,sizeof(out),wealth>=4?"Greenhouse starters for %s — hope needs fertiliser.":"Water filters to %s; thirst makes bad navigators.",dest);
  else snprintf(out,sizeof(out),wealth>=4?"Fresh cargo to %s; market is hungry.":"Essential cargo to %s; margins are thin.",dest);
 }else if(type==MISSION_BOUNTY){
  if(flags&1&&pick==0){snprintf(out,sizeof(out),"Clear a raider shaking quiet couriers near %s.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),risk>=4?"Raiders active near %s; bounty is live.":"Track one wanted hull beyond %s.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Marked hull past %s. Clear it. Do not become the next bulletin.",dest);
  else snprintf(out,sizeof(out),"Pirate taking tolls on the %s approach — remove the toll.",dest);
 }else if(type==MISSION_EXPLORATION){
  if(flags&4&&pick==0){snprintf(out,sizeof(out),"Listen twice near %s — markers, not ownership claims.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),"Untitled anomaly near %s. Scan before you invent a god.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Migration whisper off %s. Observe; do not herd.",dest);
  else snprintf(out,sizeof(out),"Survey the unusual worlds around %s.",dest);
 }else if(type==MISSION_RESCUE){
  if(pick==0)snprintf(out,sizeof(out),risk>=4?"Distress beacon in hostile lanes near %s.":"A civilian beacon is waiting near %s.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Beacon in the same sector as %s — stamp before shrug.",dest);
  else snprintf(out,sizeof(out),"Freighter tender crew in suits near %s — air thin, time thinner.",dest);
 }else{
  if(flags&1&&pick==0){snprintf(out,sizeof(out),"Sealed favour to %s — agricultural sensors on the label.",dest);return out;}
  if(pick==0)snprintf(out,sizeof(out),"Quiet courier to %s; boring lie if asked. Local law is watching.",dest);
  else if(pick==1)snprintf(out,sizeof(out),"Sealed crate to %s. Do not open for curiosity.",dest);
  else snprintf(out,sizeof(out),"Quiet courier run to %s; local law is watching.",dest);
 }
 return out;
}
int mission_risk(const Game *g,int offer){int id=mission_destination(g,offer);if(id<0)return 1;int r=danger_rating(g,id);int type=mission_type_for_offer(g,offer);if(type==MISSION_BOUNTY||type==MISSION_SMUGGLING)r++;return r>5?5:r<1?1:r;}
int mission_reward(const Game *g,int offer){int id=mission_destination(g,offer),type=mission_type_for_offer(g,offer);if(id<0)return 0;const int bonus[]={0,700,500,900,1300};int risk=mission_risk(g,offer);return 1000+(int)(distance_ly(g,g->system,id)*200)+prosperity(g,g->system)*150+bonus[type]+(risk-1)*125;}
static int job_marked(const Job *j,const Game *g,int id){if(j->dest!=g->system)return 0;if(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING)return id==0;if(j->type==MISSION_EXPLORATION)return id==j->item+1;if(j->type==MISSION_RESCUE&&j->stage)return id==0;if((j->type==MISSION_BOUNTY||j->type==MISSION_RESCUE)&&j->target>=0)return id==BODY_COUNT+1+j->target;return 0;}
static void jobs_sync(Game *g){if(g->job_n<=0){g->job_n=0;g->job_sel=0;g->contract=-1;g->mission_target=-1;g->mission_stage=0;g->contract_time=0;g->contract_reward=0;return;}if(g->job_sel<0||g->job_sel>=g->job_n)g->job_sel=0;Job *j=&g->jobs[g->job_sel];g->contract=j->dest;g->mission_type=j->type;g->mission_stage=j->stage;g->mission_target=j->target;g->mission_item=j->item;g->mission_origin=j->origin;g->contract_reward=j->reward;g->contract_time=j->time;}
static void jobs_from_legacy(Game *g){if(g->job_n>0||g->contract<0)return;g->jobs[0].dest=g->contract;g->jobs[0].type=g->mission_type;g->jobs[0].stage=g->mission_stage;g->jobs[0].target=g->mission_target;g->jobs[0].item=g->mission_item;g->jobs[0].origin=g->mission_origin;g->jobs[0].reward=g->contract_reward;g->jobs[0].time=g->contract_time>0?g->contract_time:300;g->job_n=1;g->job_sel=0;}
static void job_remove(Game *g,int i){if(i<0||i>=g->job_n)return;for(int k=i;k<g->job_n-1;k++)g->jobs[k]=g->jobs[k+1];g->job_n--;if(g->job_sel>=g->job_n)g->job_sel=g->job_n-1;if(g->job_sel<0)g->job_sel=0;jobs_sync(g);}
static void mission_finish_slot(Game *g,int i,const char *notice){if(i<0||i>=g->job_n)return;guild_event(g,g->jobs[i].type==MISSION_DELIVERY?GUILD_DELIVERY:g->jobs[i].type==MISSION_RESCUE?GUILD_RESCUE:0);g->credits+=g->jobs[i].reward;g->mission_result=1;g->last_mission_type=g->jobs[i].type;g->last_mission_system=g->jobs[i].dest;job_remove(g,i);message(g,notice);}
void mission_timers(Game *g,float dt){if(g->docked||g->dead||g->police_stop||g->approach>=0||g->dock_stage||g->jump>0)return;jobs_from_legacy(g);for(int i=0;i<g->job_n;){g->jobs[i].time-=dt;if(g->jobs[i].time<=0){g->mission_result=-1;g->last_mission_type=g->jobs[i].type;g->last_mission_system=g->jobs[i].dest;char note[96];snprintf(note,sizeof(note),"Mission expired: %s.",mission_name(g->jobs[i].type));int crate=g->jobs[i].type==MISSION_DELIVERY?0:g->jobs[i].type==MISSION_SMUGGLING?6:-1;if(crate>=0&&g->cargo[crate]>0)g->cargo[crate]--;job_remove(g,i);message(g,note);}else i++;}jobs_sync(g);}
const char *mission_objective_at(const Game *g,int slot){static char out[96];if(slot<0||slot>=g->job_n)return "No active mission";const Job *j=&g->jobs[slot];if(g->system!=j->dest){snprintf(out,sizeof(out),"Jump to %s",g->systems[j->dest].name);return out;}if(j->type==MISSION_BOUNTY){snprintf(out,sizeof(out),"Destroy the marked pirate");return out;}if(j->type==MISSION_EXPLORATION){snprintf(out,sizeof(out),"Approach and scan %s",g->bodies[j->item].name);return out;}if(j->type==MISSION_RESCUE){snprintf(out,sizeof(out),j->stage?"Return the rescued pilot to the station":"Fly within 600 m, then press O");return out;}if(j->type==MISSION_DELIVERY){snprintf(out,sizeof(out),"Dock with the food crate still aboard");return out;}if(j->type==MISSION_SMUGGLING){snprintf(out,sizeof(out),"Dock. Keep the narcotics in the hold");return out;}snprintf(out,sizeof(out),"Dock at %s",station_name(g));return out;}
const char *mission_objective(const Game *g){return mission_objective_at(g,g->job_sel);}
int is_mission_target(const Game *g,int id){for(int i=0;i<g->job_n;i++)if(job_marked(&g->jobs[i],g,id))return 1;if(g->job_n==0&&g->contract>=0){Job tmp={g->contract,g->mission_type,g->mission_stage,g->mission_target,g->mission_item,g->mission_origin,g->contract_reward,g->contract_time};return job_marked(&tmp,g,id);}return 0;}
int mission_interact(Game *g,int id){int n=id-BODY_COUNT-1;if(n<0||n>=NPC_COUNT)return 0;for(int i=0;i<g->job_n;i++){Job *j=&g->jobs[i];if(j->type!=MISSION_RESCUE||j->stage||!job_marked(j,g,id))continue;if(length(sub(g->pos,g->npc[n].pos))>600){message(g,"Move within 600 m of the rescue target.");return 0;}j->stage=1;g->job_sel=i;jobs_sync(g);g->cue=SFX_SCAN;message(g,"Pilot aboard. Return to the local station.");return 1;}return 0;}
int mission_offer_active(const Game *g,int offer){int dest=mission_destination(g,offer),type=mission_type_for_offer(g,offer);if(dest<0)return 0;for(int i=0;i<g->job_n;i++)if(g->jobs[i].dest==dest&&g->jobs[i].type==type)return 1;return 0;}
int abandon_mission(Game *g,int slot){jobs_from_legacy(g);if(slot<0||slot>=g->job_n)return 0;g->mission_result=-1;g->last_mission_type=g->jobs[slot].type;g->last_mission_system=g->jobs[slot].dest;int crate=g->jobs[slot].type==MISSION_DELIVERY?0:g->jobs[slot].type==MISSION_SMUGGLING?6:-1;if(crate>=0&&g->cargo[crate]>0)g->cargo[crate]--;g->credits+=100;char note[96];snprintf(note,sizeof(note),"Abandoned: %s. Crate/deposit returned.",mission_name(g->jobs[slot].type));job_remove(g,slot);message(g,note);g->cue=SFX_UI;return 1;}
int accept_mission(Game *g,int offer){if(!g->docked){message(g,"Dock to accept mission work.");return 0;}jobs_from_legacy(g);if(g->job_n>=MISSION_SLOTS){message(g,"Mission log full (5/5). Complete a job first.");return 0;}int destination=mission_destination(g,offer);if(offer<0||offer>=mission_count(g)||destination<0)return 0;int type=mission_type_for_offer(g,offer);if(mission_offer_active(g,offer)){message(g,"Mission in progress.");return 0;}if(g->credits<100){message(g,"Contract deposit: 10 units.");return 0;}if((type==MISSION_DELIVERY||type==MISSION_SMUGGLING)&&cargo_used(g)>=cargo_capacity(g)){message(g,"Hold is full. Free a tonne first.");return 0;}g->credits-=100;Job *j=&g->jobs[g->job_n];
 /* Prefer a landable world at the destination; type permutations can make body 1 a gas giant. */
 int item=1;{unsigned sys=sector_hash((destination+1)*0x9e3779b9u);int perm=(int)((sys>>6)%6);const int world_perm[6][4]={{OCEAN,ROCKY,GAS,ROCKY},{ROCKY,GAS,OCEAN,ROCKY},{GAS,OCEAN,ROCKY,ROCKY},{ROCKY,OCEAN,ROCKY,GAS},{OCEAN,GAS,ROCKY,ROCKY},{ROCKY,ROCKY,OCEAN,GAS}};const int prefer[]={1,2,4,3};
  for(int i=0;i<4;i++){int b=prefer[(destination+i)%4];if(destination==7&&b==1){item=1;break;}if(world_perm[perm][(b-1)&3]!=GAS){item=b;break;}}}
 j->dest=destination;j->type=type;j->origin=g->system;j->stage=0;j->target=-1;j->item=item;j->time=300;j->reward=mission_reward(g,offer);g->job_sel=g->job_n;g->job_n++;g->destination=destination;if(type==MISSION_DELIVERY)g->cargo[0]++;else if(type==MISSION_SMUGGLING)g->cargo[6]++;jobs_sync(g);char note[96];if(type==MISSION_SMUGGLING)snprintf(note,sizeof(note),"Smuggle accepted. Narcotics aboard — Law scans holds.");else snprintf(note,sizeof(note),"%s accepted. Risk %d/5. Log %d/%d.",mission_name(j->type),mission_risk(g,offer),g->job_n,MISSION_SLOTS);message(g,note);g->cue=SFX_SELECT;return 1;}
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
