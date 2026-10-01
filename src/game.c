/* Native PSP adaptation. Galaxy/market algorithms and market values follow
 * Elite-A TT24, TT20, QQ23; geometry is generated from its blueprint macros.
 * Combat and flight here are new approximation code, not translated ArcElite AI.
 */
#include "game.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "equipment-fit.h"
#include "rift-catalog.h"
#include "social-events.h"
const Good goods[GOODS]={
 {"Food",19,-2,6,1,'t'},{"Textiles",20,-1,10,3,'t'},
 {"Radioactives",65,-3,2,7,'t'},{"Slaves",40,-5,226,31,'t'},
 {"Liquor/Wines",83,-5,251,15,'t'},{"Luxuries",196,8,54,3,'t'},
 {"Narcotics",235,29,8,120,'t'},{"Computers",154,14,56,3,'t'},
 {"Machinery",117,6,40,7,'t'},{"Alloys",78,1,17,31,'t'},
 {"Firearms",124,13,29,7,'t'},{"Furs",176,-9,220,63,'t'},
 {"Minerals",32,-1,53,3,'t'},{"Gold",97,-1,66,7,'k'},
 {"Platinum",171,-2,55,31,'k'},{"Gem-Stones",45,-1,250,15,'g'},
 {"Alien items",53,15,192,7,'t'} };
/* Early-build shipyard balance; not the complete Elite-A new_details table. */
const PlayerShip player_ships[]={
 {"ADDER",8,0,360,60},{"GECKO",9,25000,450,70},
 {"MORAY",11,65000,380,80},{"COBRA MK 1",15,120000,390,70},
 {"COBRA MK 3",20,250000,420,70},{"PYTHON",60,800000,300,90},
 {"ANACONDA",100,1500000,240,100},
 {"FER DE LANCE",28,420000,520,76},
 {"KRAIT",36,680000,470,84},
 {"OPHIDIAN",48,980000,410,92}};
const int player_ship_count=sizeof(player_ships)/sizeof(*player_ships);
int mission_landable_body(const Game *g,int system,int body){if(!world_planet_available(g,system,body))return 0;Game probe=*g;probe.system=system;system_bodies(&probe);return probe.bodies[body].type!=SUN&&probe.bodies[body].type!=GAS;}
int mission_offer_valid(const Game *g,int offer,int *destination_out){if(!g||offer<0)return 0;int dest=mission_destination(g,offer);if(dest<0||distance_ly(g,g->system,dest)>fminf(10.f,player_ships[g->ship].range*.1f)+.001f)return 0;int type=mission_type_for_offer(g,offer);if(type==MISSION_EXPLORATION){int ok=0;for(int b=1;b<BODY_COUNT;b++)if(mission_landable_body(g,dest,b)){ok=1;break;}if(!ok)return 0;}if(destination_out)*destination_out=dest;return world_station_available(g,dest,0);}
Vec3 add(Vec3 a,Vec3 b){return (Vec3){a.x+b.x,a.y+b.y,a.z+b.z};}
Vec3 sub(Vec3 a,Vec3 b){return (Vec3){a.x-b.x,a.y-b.y,a.z-b.z};}
Vec3 mul(Vec3 a,float s){return (Vec3){a.x*s,a.y*s,a.z*s};}
float dot(Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
float length(Vec3 a){return sqrtf(dot(a,a));}
Vec3 norm(Vec3 a){float l=length(a);return l>0.001f?mul(a,1/l):(Vec3){0,0,1};}
Vec3 forward(const Game *g){return (Vec3){sinf(g->yaw)*cosf(g->pitch),sinf(g->pitch),cosf(g->yaw)*cosf(g->pitch)};}
Vec3 camera(const Game *g,Vec3 p){
 static const Game *owner=0;static float yaw,pitch,roll;static Vec3 right,up,ahead;
 if(owner!=g||yaw!=g->yaw||pitch!=g->pitch||roll!=g->roll){
  float sy=sinf(g->yaw),cy=cosf(g->yaw),sp=sinf(g->pitch),cp=cosf(g->pitch),sr=sinf(g->roll),cr=cosf(g->roll);
  Vec3 r={cy,0,-sy},u={-sy*sp,cp,-cy*sp};right=add(mul(r,cr),mul(u,sr));up=add(mul(r,-sr),mul(u,cr));ahead=(Vec3){sy*cp,sp,cy*cp};
  owner=g;yaw=g->yaw;pitch=g->pitch;roll=g->roll;
 }
 Vec3 d=sub(p,g->pos);return (Vec3){dot(d,right),dot(d,up),dot(d,ahead)};
}
/* Integrate about cockpit axes, then recover Euler storage. Rotating the full
 * frame preserves screen-relative input through banks and vertical loops.
 * atan2(y,horizontal) avoids asin precision loss at the poles. */
static Vec3 flight_cross(Vec3 a,Vec3 b){return (Vec3){a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
static Vec3 flight_rotate(Vec3 v,Vec3 axis,float c,float s){
 return add(add(mul(v,c),mul(flight_cross(axis,v),s)),mul(axis,dot(axis,v)*(1-c)));
}
static void flight_steer(Game *g,float turn,float pitch,float step){
 float amount=sqrtf(turn*turn+pitch*pitch);
 if(amount<.000001f||step<=0)return;
 float sy=sinf(g->yaw),cy=cosf(g->yaw),sp=sinf(g->pitch),cp=cosf(g->pitch);
 float sr=sinf(g->roll),cr=cosf(g->roll);
 Vec3 r={cy,0,-sy},u={-sy*sp,cp,-cy*sp};
 Vec3 right=add(mul(r,cr),mul(u,sr)),up=add(mul(r,-sr),mul(u,cr));
 Vec3 axis=mul(sub(mul(up,turn),mul(right,pitch)),1/amount);
 float c=cosf(amount*step),s=sinf(amount*step);
 Vec3 ahead=norm(flight_rotate(forward(g),axis,c,s));
 right=norm(flight_rotate(right,axis,c,s));
 float horizontal=sqrtf(ahead.x*ahead.x+ahead.z*ahead.z);
 if(horizontal>.000001f)g->yaw=atan2f(ahead.x,ahead.z);
 g->pitch=atan2f(ahead.y,horizontal);
 sy=sinf(g->yaw);cy=cosf(g->yaw);sp=sinf(g->pitch);cp=cosf(g->pitch);
 r=(Vec3){cy,0,-sy};u=(Vec3){-sy*sp,cp,-cy*sp};
 g->roll=atan2f(dot(right,u),dot(right,r));
}
int mesh_id(const char *name){for(int i=0;i<mesh_count;i++)if(!strcmp(meshes[i].name,name))return i;return 4;}
static uint32_t random_u(Game *g){uint32_t x=g->rng;x^=x<<13;x^=x>>17;x^=x<<5;return g->rng=x;}
static float random_f(Game *g){return (random_u(g)&65535)/65535.0f;}
void message(Game *g,const char *s){snprintf(g->message,sizeof(g->message),"%s",s);g->message_time=5;}
int encounter_requires_reply(const Game *g){
 if(!g||g->encounter_kind==ENCOUNTER_NONE||g->encounter<=0)return 0;
 if(g->encounter_kind==ENCOUNTER_POLICE)return g->legal>0;
 switch(g->encounter_kind){
  case ENCOUNTER_DISTRESS:case ENCOUNTER_CONVOY:case ENCOUNTER_PIRATE:
  case ENCOUNTER_BOUNTY:case ENCOUNTER_ESCAPE_POD:case ENCOUNTER_SMUGGLER:return 1;
  default:return 0;
 }
}
void speak(Game *g,int who,const char *s){
 g->voice_who=who<VOICE_KEI||who>VOICE_CONTACT?VOICE_COMP:who;
 if(g->voice_who==VOICE_LAW&&g->legal<=0&&!g->police_stop&&!g->attacked&&!g->incoming_missile){g->voice[0]=0;g->voice_time=0;return;}
 if(g->voice_who==VOICE_COMP){g->voice[0]=0;g->voice_time=0;if(!g->cue)g->cue=SFX_COMM;return;}
 snprintf(g->voice,sizeof(g->voice),"%s",s);g->voice_time=6.5f;g->message_time=0;if(!g->cue)g->cue=(g->attacked>0||g->incoming_missile>0||g->encounter>0)?SFX_TALK:SFX_COMM;
}
void encounter_ignore(Game *g){
 int kind=g->encounter_kind;g->encounter_kind=ENCOUNTER_NONE;g->encounter_npc=-1;g->encounter_payload=-1;g->encounter=0;g->voice_time=0;g->voice[0]=0;
 if(kind==ENCOUNTER_POLICE&&g->legal>0){police_begin(g,0);return;}
 message(g,kind==ENCOUNTER_PIRATE?"Pirate channel ignored. Weapons hot.":"Channel ignored.");
}
void encounter_respond(Game *g){
 int kind=g->encounter_kind,n=g->encounter_npc;g->encounter=4;
 if(kind==ENCOUNTER_POLICE){if(g->legal>0){police_begin(g,0);return;}speak(g,VOICE_LAW,"Routine scan complete. Safe travels, Commander.");return;}
 if(kind==ENCOUNTER_PIRATE){if(n>=0&&n<NPC_COUNT&&g->npc[n].alive)g->npc[n].target=-2;g->attacked=4;speak(g,VOICE_CONTACT,"You chose poorly. Dump cargo or defend yourself.");g->voice_role=PIRATES;return;}
 if(kind==ENCOUNTER_DISTRESS||kind==ENCOUNTER_CONVOY){g->credits+=350;speak(g,VOICE_CONTACT,"You saved our convoy, Commander. The gratitude is real.");g->voice_role=TRADERS;message(g,"RESCUE REWARD: +350 CR");return;}
 if(kind==ENCOUNTER_BOUNTY){if(n>=0&&n<NPC_COUNT&&g->npc[n].alive){g->npc[n].target=-2;g->attacked=4;message(g,"BOUNTY TARGET MARKED. Engage when ready.");}return;}
 if(kind==ENCOUNTER_CARGO||kind==ENCOUNTER_WRECKAGE){message(g,"Scanner data uploaded. Approach the marked cargo and press Triangle to collect it.");return;}
 if(kind==ENCOUNTER_ESCAPE_POD){if(g->passenger_dest<0){g->passenger_dest=g->system;g->passenger_kind=1;g->passenger_pay=450;speak(g,VOICE_CONTACT,"Thank you, Commander. Get me to the nearest station.");message(g,"SURVIVOR RESCUED: deliver them to a station.");}return;}
 if(kind==ENCOUNTER_SMUGGLER){if(n>=0&&n<NPC_COUNT&&g->npc[n].alive)trader_offer_hail(g,n);else speak(g,VOICE_CONTACT,"Private cargo is available if you know where to look.");return;}
 if(kind==ENCOUNTER_DERELICT){message(g,"Derelict logs recovered. A navigation marker was added to your chart.");g->discoveries++;return;}
 if(kind==ENCOUNTER_MYSTERY||kind==ENCOUNTER_UNKNOWN){speak(g,VOICE_COMP,"Signal archived. No source identified.");return;}
 speak(g,VOICE_CONTACT,"Safe flight, Commander. Keep your scanner open.");
}
static void mark_visited(Game *g){g->visited[g->system>>3]|=(uint8_t)(1u<<(g->system&7));}
static const char *arrival_government(int government){static const char *names[]={"ANARCHY","FEUDAL","MULTI-GOVERNMENT","DICTATORSHIP","COMMUNIST","CONFEDERACY","DEMOCRACY","CORPORATE STATE"};return names[government&7];}
static void first_arrival_brief(const Game *g,char *out,int cap){const System *s=&g->systems[g->system];snprintf(out,cap,"First arrival at %s. %s station. Government %s. Local danger %d of 5. Four planets and the local station are now logged in your Discovery Codex.",s->name,station_class_name(station_class(g)),arrival_government(s->government),danger_rating(g,g->system));}
int systems_visited(const Game *g){int n=0;for(int i=0;i<32;i++)for(int b=0;b<8;b++)n+=(g->visited[i]>>b)&1;return n;}
static void twist(uint16_t s[3]){uint16_t t=(uint16_t)(s[0]+s[1]+s[2]);s[0]=s[1];s[1]=s[2];s[2]=t;}
void galaxy(System out[256]){
 uint16_t s[3]={0x5a4a,0x0248,0xb753};
 const char pairs[]="..LEXEGEZACEBISOUSESARMAINDIREA.ERATENBERALAVETIEDORQUANTEISRION";
 for(int i=0;i<256;i++){
  System *p=&out[i];p->x=s[1]>>8;p->y=s[0]>>8;
  p->government=(s[1]>>3)&7;p->economy=(s[0]>>8)&7;
  if(p->government<2)p->economy|=2;
  p->tech=((s[1]>>8)&3)+(p->economy^7)+(p->government>>1)+(p->government&1);
  int longname=s[0]&64,n=0;
  for(int k=0;k<4;k++){int j=2*((s[2]>>8)&31);if(k<3||longname){for(int a=0;a<2;a++)if(pairs[j+a]!='.'&&pairs[j+a]!=' ')p->name[n++]=pairs[j+a];}twist(s);}
  p->name[n]=0;
 }
}
float distance_ly(const Game *g,int a,int b){float dx=g->systems[a].x-g->systems[b].x,dy=(g->systems[a].y-g->systems[b].y)*0.5f;return floorf(sqrtf(dx*dx+dy*dy))*0.4f;}
/* Fuel is a ship resource, never a tonne in the hold. Keeping it separate
 * avoids confusing cargo capacity with the tank gauge. */
int fuel_cargo_units(const Game *g){(void)g;return 0;}
int cargo_used(const Game *g){int n=fuel_cargo_units(g);for(int i=0;i<GOODS;i++)if(goods[i].unit=='t')n+=g->cargo[i];if(g->passenger_dest>=0)n+=1;return n;}
int cargo_capacity(const Game *g){int c=player_ships[g->ship].capacity;int fitted=fit_hold_bonus(g);if(fitted)c+=fitted;else if(g->upgrades&64)c+=16;else if(g->upgrades&8)c+=8;return c;}
int refuel_full(Game *g){if(!g)return 0;if(g->fuel>=player_ships[g->ship].range-.001f)return 1;g->fuel=(float)player_ships[g->ship].range;return 1;}
int galactic_price(int item){if(item<0||item>=GOODS)return 0;const Good *p=&goods[item];int avg=4*((p->base+(p->mask>>1)+3*p->factor)&255);return avg<4?4:avg;}
void market(Game *g){int e=g->systems[g->system].economy,fluct=random_u(g)&255;
 for(int i=0;i<GOODS;i++){const Good *p=&goods[i];int f=fluct&p->mask;int q=(p->quantity+f-e*p->factor)&255;g->stock[i]=((q&128)?0:(q&63))+prosperity(g,g->system)*4;g->price[i]=4*((p->base+f+e*p->factor)&255);}g->stock[16]=0;
}
#include "guild.h"
#include "campaign.h"
#include "campaign-runtime.h"
#include "saga.h"
#include "sectors.h"
#include "story.h"
#include "tutorial.h"
int danger_rating(const Game *g,int system){return system==7?1:1+(7-g->systems[system].government)*4/7;}
int system_is_lawful(const Game *g,int system){return system==7||g->systems[system].government>=4;}
int system_rock_belt(int sys){return sys==7||(sys%3)!=2;}
int system_ice_belt(int sys){return sys!=7&&((sys%5)==1||(sys%7)==3);}
int system_whales(int sys){return sys!=7&&((sys%6)==2||(sys%11)==0);}
int system_comet(int sys){return sys!=7&&(sys%13)==4;}
int traffic_budget(const Game *g){
 int seed=(g->system*37+g->systems[g->system].government*11+g->systems[g->system].economy*17)%7;
 int n=2+prosperity(g,g->system)/2+seed;
 if(g->system==7)n=10;
 if(danger_rating(g,g->system)>=4)n+=4;
 if(n>18)n=18;
 return n;
}
Vec3 hub_position(const Game *g,int hub){int s=g->system;if(hub<=0)return (Vec3){0,0,STATION_Z};if(hub==1)return (Vec3){-24000.f+(s%5)*900.f,2200.f,-22000.f-(s%7)*650.f};return (Vec3){26000.f-(s%6)*850.f,-1800.f,25000.f+(s%9)*700.f};}
int nearest_hub(const Game *g){Vec3 p=g->pos;int best=0;float bd=length(sub(p,hub_position(g,0)));for(int i=1;i<HUB_COUNT;i++){float d=length(sub(p,hub_position(g,i)));if(d<bd){bd=d;best=i;}}return best;}
static int npc_role_for(const Game *g,int i){
 int risk=danger_rating(g,g->system);
 int lawful=system_is_lawful(g,g->system);
 if(i<4)return !lawful&&i==LAW?PIRATES:i;
 if(i==8||i==20||i==32)return TRADERS;
 if(i==7||i==11||i==15)return EXPLORERS;
 if(i==6||i==10||i==14)return lawful?LAW:PIRATES;
 if(risk>=4&&(i==5||i==9||i==12||i==13||i==16||i==17))return PIRATES;
 if(risk>=2&&(i==5||i==9||i==12))return PIRATES;
 return TRADERS;
}
int wanted_level(const Game *g){return g->legal>0?(g->legal>=25?5:1+(g->legal-1)/5):0;}
void record_crime(Game *g,int points,unsigned reason){
 if(points<=0)return;
 unsigned record=g->legal>0?g->crime_record[g->system]:0;
 if(g->legal>0&&!record)record=CRIME_UNKNOWN;
 int cargo=record>>8,old=g->legal,overflow=old+points-1000;
 g->legal=old+points;if(g->legal>1000)g->legal=1000;
 if(reason==CRIME_CARGO)cargo+=g->legal-old;
 else if(overflow>0){cargo-=overflow;if(cargo<0)cargo=0;}
 g->crime_record[g->system]=((record|reason)&255)|((unsigned)cargo<<8);
 g->police_cargo_heat=cargo;g->wanted[g->system]=g->legal;
}
void add_crime(Game *g,int points){record_crime(g,points,CRIME_UNKNOWN);}
const char *police_accusation(const Game *g){
 unsigned charges=g->crime_record[g->system]&255;
 if(charges&CRIME_LAW_DESTRUCTION)return "Commander, you destroyed a patrol vessel. Cut your engines. You are being detained for that attack.";
 if(charges&CRIME_DESTRUCTION)return "Commander, a vessel was destroyed by your weapons. This is a criminal stop, not a cargo inspection. Power down.";
 if(charges&CRIME_LAW_ASSAULT)return "Commander, you fired on a law-enforcement vessel. Cease fire and cut your engines. You must answer for that attack.";
 if(charges&CRIME_ASSAULT)return "Commander, your weapons struck a vessel that was not a lawful target. Stop firing and power down. An assault has been recorded.";
 if(charges&CRIME_ESCAPE)return "Commander, you fled a lawful stop. Your warrant is still active. Cut your engines; do not attempt to run again.";
 if(charges&CRIME_REFUSAL)return "Commander, you refused a lawful inspection. A warrant has been issued. Power down while we settle this.";
 if(charges&CRIME_CARGO)return "Commander, the scan confirmed restricted goods in your hold. Keep your engines off while we resolve the cargo charge.";
 return "Commander, an outstanding local warrant is attached to your vessel. The original incident details are unavailable. Power down while we settle the record.";
}
void police_charge_details(const Game *g,char *out,int cap){
 static const char *labels[]={"assault on a vessel","attack on Local Law","destruction of a vessel","destruction of a patrol","restricted cargo","inspection refusal","fleeing a stop","earlier warrant"};
 if(cap<=0)return;out[0]=0;unsigned charges=g->crime_record[g->system]&255;
 if(g->legal>0&&!charges)charges=CRIME_UNKNOWN;
 snprintf(out,cap,"Recorded offences: ");
 for(int i=0;i<8;i++)if(charges&(1u<<i)){
  size_t n=strlen(out);snprintf(out+n,cap-(int)n,"%s%s",labels[i],(charges&~((1u<<(i+1))-1))?"; ":". ");
 }
 size_t n=strlen(out);int cargo=cargo_contraband(g);
 if(cargo>0){snprintf(out+n,cap-(int)n,"Restricted goods aboard: %d t. ",cargo);n=strlen(out);}
 snprintf(out+n,cap-(int)n,"Fine: %.1f units. Payment or custody settles this local warrant.",police_fine(g)*.1f);
}
int bounty_target_index(const Game *g,int poster){(void)g;return poster>=0&&poster<BOUNTY_POSTER_COUNT?BOUNTY_NPC_FIRST+poster:-1;}
int bounty_target_taken(const Game *g,int poster){return g&&poster>=0&&poster<BOUNTY_POSTER_COUNT&&(g->bounty_claimed[g->system]&(1u<<poster))!=0;}
int bounty_target_reward(const Game *g,int poster){int risk=g?danger_rating(g,g->system):1;return 300+fmaxf(0,risk-1)*125+fmaxf(0,poster)*25;}
void bounty_target_label(const Game *g,int poster,char *out,int cap){if(!out||cap<1)return;int seed=(g?g->system:0)*17+poster*29;snprintf(out,cap,"RAIDER %c-%02d",'A'+seed%26,(seed/26)%100);}
int goods_restricted(int item){return item==3||item==6||item==10;}
int cargo_contraband(const Game *g){int n=0;for(int i=0;i<GOODS;i++)if(goods_restricted(i))n+=g->cargo[i];return n;}
static int police_seize_contraband(Game *g){
 int seized=0,heat=0;
 for(int i=0;i<GOODS;i++)if(goods_restricted(i)&&g->cargo[i]>0){
  heat+=2+g->cargo[i];seized+=g->cargo[i];g->cargo[i]=0;
 }
 if(heat){if(heat>25)heat=25;record_crime(g,heat,CRIME_CARGO);}
 return seized;
}
int police_fine(const Game *g){int wl=wanted_level(g);return (wl>0?wl:1)*500;}
/* Phases: 0 warrant, 1 scan; 2/3 seized custody, 4/5 paid custody;
 * 6/7 release receipts, 8 voluntary cargo surrender. Charges persist per system in V20 saves. */
static void police_stand_down(Game *g){
 g->legal=0;g->wanted[g->system]=0;g->crime_record[g->system]=0;g->police_cargo_heat=0;g->police_warning=0;g->police_warned=0;
 g->police_grace=15;g->attacked=0;g->speed=0;g->boost=0;
 for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].role==LAW)g->npc[i].target=-1;
 if(g->incoming_source>=0&&g->incoming_source<NPC_COUNT&&g->npc[g->incoming_source].role==LAW){g->incoming_missile=0;g->incoming_source=-1;}
 if(g->encounter_kind==ENCOUNTER_POLICE){g->encounter_kind=ENCOUNTER_NONE;g->encounter=0;}
}
int police_surrender_cargo(Game *g){
 if(!g->police_stop||g->police_phase>1||cargo_contraband(g)<=0)return 0;
 for(int i=0;i<GOODS;i++)if(goods_restricted(i))g->cargo[i]=0;
 g->legal-=g->police_cargo_heat;if(g->legal<0)g->legal=0;
 g->wanted[g->system]=g->legal;g->police_cargo_heat=0;g->crime_record[g->system]&=255u&~CRIME_CARGO;
 if(g->legal>0){g->police_phase=0;message(g,"Cargo surrendered. Other offences still require settlement.");speak(g,VOICE_LAW,"Cargo received. Your other offences remain on the warrant. Pay the fine or accept custody.");return 1;}
 police_stand_down(g);g->police_phase=8;g->police_timer=0;g->cue=SFX_UI;
 message(g,"Restricted cargo surrendered. Local warrant cleared.");
 speak(g,VOICE_LAW,"Cargo transfer confirmed. Your cooperation settles the local warrant. Patrols are standing down.");return 1;
}
int police_acknowledge(Game *g){
 if(!g->police_stop||g->police_phase<6||g->police_phase>8)return 0;
 g->police_stop=0;g->police_phase=0;g->police_timer=0;g->voice_time=0;g->message_time=0;return 1;
}
static void police_jail_release(Game *g,int seized){
 if(seized){
  g->credits=0;for(int i=0;i<GOODS;i++)g->cargo[i]=0;
  g->ship=0;g->upgrades=0;g->laser=0;g->missiles=0;fit_clear_all(g);
  g->fuel=(float)player_ships[g->ship].range;g->hull=100;g->damaged=0;g->damage_fx=0;
 }else for(int i=0;i<GOODS;i++)if(goods_restricted(i))g->cargo[i]=0;
 g->energy=100;g->heat=0;police_stand_down(g);
 g->police_phase=seized?6:7;g->police_timer=0;g->docked=1;g->pos=(Vec3){0,0,3200};
 g->dock_stage=0;g->approach=-1;g->cue=SFX_DOCK;
 message(g,"Release authorised. Read your custody record before returning.");
}
static void police_jail_tick(Game *g,float dt){
 if(!g->police_stop||g->police_phase<2||g->police_phase>5)return;
 g->police_timer-=dt;if(g->police_timer>0)return;
 if(g->police_phase==2||g->police_phase==4){g->police_phase++;g->police_timer=3.5f;g->cue=SFX_TALK;}
 else police_jail_release(g,g->police_phase==3);
}
void police_begin(Game *g,int phase){
 g->police_cargo_heat=g->crime_record[g->system]>>8;g->police_stop=1;g->police_phase=phase?1:0;g->police_warning=0;g->police_warned=0;g->speed=0;g->boost=0;g->approach=-1;g->cue=SFX_ALERT;
 if(g->police_phase==1){message(g,"Local Law: hold inspection. Submit, refuse, or run.");speak(g,VOICE_LAW,"Inspection. Submit your hold, refuse, or run.");}
 else {message(g,"Local Law: stop and settle your warrant.");speak(g,VOICE_LAW,police_accusation(g));}
}
int police_scan_submit(Game *g){
 if(!g->police_stop||g->police_phase!=1)return 0;
 int seized=cargo_contraband(g);
 if(!seized&&g->legal>0){g->police_phase=0;message(g,"Hold clear, but your existing warrant remains.");speak(g,VOICE_LAW,police_accusation(g));return 1;}
 if(!seized){g->police_stop=0;g->police_phase=0;g->police_grace=5.f;g->cue=SFX_UI;message(g,"Hold clear. Free to proceed.");speak(g,VOICE_LAW,"Clean hold. Proceed, Commander.");return 1;}
 if(!(g->crime_record[g->system]&CRIME_CARGO))record_crime(g,seized>23?25:seized+2,CRIME_CARGO);g->police_phase=0;g->cue=SFX_ALERT;message(g,"Contraband confirmed. Surrender cargo, pay, or accept custody.");speak(g,VOICE_LAW,"Restricted cargo confirmed. Hand it over voluntarily and we will waive the cargo charge. Other offences still apply.");return 1;
}
int police_scan_refuse(Game *g){
 if(!g->police_stop||g->police_phase!=1)return 0;
 int seized=police_seize_contraband(g);record_crime(g,seized?6:4,CRIME_REFUSAL);
 g->police_phase=0;g->cue=SFX_ALERT;message(g,"Scan forced. Warrant filed. Settle or run.");speak(g,VOICE_LAW,"Refusal noted. Hold opened under warrant.");return 1;
}
int police_resolve(Game *g,int jail){
 if(!g->police_stop||g->police_phase!=0)return 0;
 int cost=jail?police_fine(g)/2:police_fine(g);
 if(!jail&&g->credits<cost){message(g,"Not enough units. Choose station custody.");return 0;}
 if(jail){
  social_emit(g,SB_ARREST);int seized=g->credits<=0;if(cost>g->credits)cost=g->credits;g->credits-=cost;
  g->police_phase=seized?2:4;g->police_timer=2.8f;g->speed=0;g->boost=0;g->cue=SFX_ALERT;
  message(g,seized?"Custody transfer. No funds: property seizure pending.":"Release fee reserved. Custody shuttle approaching.");
  return 1;
 }
 social_emit(g,SB_FINE);g->credits-=cost;int had_cargo=cargo_contraband(g)>0;
 for(int i=0;i<GOODS;i++)if(goods_restricted(i))g->cargo[i]=0;
 police_stand_down(g);g->police_stop=0;g->police_phase=0;g->cue=SFX_UI;
 message(g,had_cargo?"Fine paid. Restricted goods seized. Local warrant cleared.":"Fine paid. Local warrant cleared. You may proceed.");
 return 1;
}
int police_escape(Game *g){
 if(!g->police_stop||g->police_phase>1)return 0;
 social_emit(g,SB_FLEE);int scan=g->police_phase==1;g->police_stop=0;g->police_phase=0;g->police_warning=0;g->police_warned=0;record_crime(g,scan?10:8,CRIME_ESCAPE);g->police_grace=7;g->attacked=7;
 g->speed=fmaxf(g->speed,player_ships[g->ship].speed*.8f);g->boost=0;
 for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive&&g->npc[i].role==LAW)g->npc[i].target=-2;
 g->cue=SFX_ALERT;message(g,"You ran. Local warrant raised; patrols are pursuing.");speak(g,VOICE_LAW,"Suspect fleeing. All patrols intercept.");return 1;
}
int police_pay_desk(Game *g){
 if(!g->docked||g->legal<=0||g->police_stop)return 0;
 int cost=police_fine(g);if(g->credits<cost){message(g,"Not enough units to clear this warrant.");return 0;}
 g->credits-=cost;g->legal=0;g->wanted[g->system]=0;g->crime_record[g->system]=0;g->police_cargo_heat=0;g->cue=SFX_UI;
 message(g,"Local fine paid at the station desk. Warrant cleared.");return 1;
}
int approach_planet(Game *g,int body){if(g->dead||g->docked||g->dock_stage||g->police_stop||g->jump>0||g->planet>=0||body<1||body>=BODY_COUNT)return 0;Body *b=&g->bodies[body];if(dot(forward(g),norm(sub(b->pos,g->pos)))<.7f){message(g,"Turn to face the planet.");return 0;}if(length(sub(g->pos,b->pos))>b->radius+1000+fmaxf(0,g->speed)*.1f){message(g,"Move within 1,000 m of the surface first.");return 0;}g->approach=body;g->boost=0;jobs_from_legacy(g);for(int i=0;i<g->job_n;){if(g->jobs[i].dest==g->system&&g->jobs[i].type==MISSION_EXPLORATION&&g->jobs[i].item==body){g->job_sel=i;mission_finish_slot(g,i,"Exploration scan complete. Payment received.");}else i++;}story_event(g,STORY_EV_WORLD);return 1;}
void turn_back(Game *g){if(g->approach>=1&&g->approach<BODY_COUNT){Body *body=&g->bodies[g->approach];Vec3 delta=sub(g->pos,body->pos);float distance=length(delta);Vec3 away=distance>1?mul(delta,1/distance):mul(forward(g),-1);if(distance<body->radius+100)g->pos=add(body->pos,mul(away,body->radius+100));g->yaw=atan2f(away.x,away.z);g->pitch=asinf(fmaxf(-1,fminf(1,away.y)));}else {g->yaw+=3.14159265f;g->pitch=-g->pitch;}g->speed=100;g->boost=0;g->approach=-1;}
static float wrap_range(float v,float half){float w=half*2;v=fmodf(v+half,w);if(v<0)v+=w;return v-half;}
static void site_xz(const Game *g,int i,float *x,float *z){
 static unsigned seeds[3]={~0u,~0u,~0u};static float xs[3],zs[3];unsigned seed=g->bodies[g->planet].seed;
 if(i>=0&&i<3){if(seeds[i]!=seed){unsigned h=sector_hash(seed+(unsigned)(i+1)*9973u);float a=(h%6283)*.001f,d=480.f+(h%900);xs[i]=cosf(a)*d;zs[i]=sinf(a)*d;seeds[i]=seed;}*x=xs[i];*z=zs[i];return;}
 unsigned h=sector_hash(seed+(unsigned)(i+1)*9973u);float a=(h%6283)*.001f,d=480.f+(h%900);*x=cosf(a)*d;*z=sinf(a)*d;
}
static float terrain_smooth(float x){x=fmaxf(0,fminf(1,x));return x*x*(3-2*x);}
static float terrain_noise(const Game *g,float x,float z,float scale,unsigned salt){
 float gx=x/scale,gz=z/scale;int ix=(int)floorf(gx),iz=(int)floorf(gz);float u=terrain_smooth(gx-ix),v=terrain_smooth(gz-iz);unsigned seed=g->bodies[g->planet].seed^salt;
 float a=(sector_hash(seed^(unsigned)ix*73856093u^(unsigned)iz*19349663u)&65535)/65535.f;
 float b=(sector_hash(seed^(unsigned)(ix+1)*73856093u^(unsigned)iz*19349663u)&65535)/65535.f;
 float c=(sector_hash(seed^(unsigned)ix*73856093u^(unsigned)(iz+1)*19349663u)&65535)/65535.f;
 float d=(sector_hash(seed^(unsigned)(ix+1)*73856093u^(unsigned)(iz+1)*19349663u)&65535)/65535.f;
 return (a+(b-a)*u)+((c+(d-c)*u)-(a+(b-a)*u))*v;
}
float terrain_relief_scale(const Game *g){
 if(g->planet<1||g->planet>=BODY_COUNT||g->bodies[g->planet].type==GAS)return 0;
 FieldProfile p=field_profile(g,g->system,g->planet);static const float relief[]={260,310,470,650,360,0};
 float r=relief[p.biome<0||p.biome>5?1:p.biome];r*=.82f+p.geology*.18f;
 if(g->system==7&&g->planet==1)r=360;return r;
}
static int terrain_has_river(const Game *g){
 if(g->planet<1||g->planet>=BODY_COUNT||g->bodies[g->planet].type==GAS)return 0;
 FieldProfile p=field_profile(g,g->system,g->planet);
 return p.biome==0||p.biome==2||p.biome==4||p.weather==2||((p.seed>>5)&3)!=0;
}
/* u follows the river valley; v crosses it. The seeded rotation prevents all
 * worlds from sharing a north/south river. */
static void terrain_river_uv(const Game *g,float x,float z,float *u,float *v,float *centre,float *width){
 float px,pz;site_xz(g,1,&px,&pz);float angle=((g->bodies[g->planet].seed>>8)%6283)*.001f,c=cosf(angle),s=sinf(angle),dx=x-px,dz=z-pz;
 *u=dx*c+dz*s;*v=-dx*s+dz*c;*centre=sinf(*u/1180.f+(g->bodies[g->planet].seed&255)*.017f)*310.f+sinf(*u/430.f)*85.f;
 FieldProfile p=field_profile(g,g->system,g->planet);*width=(p.biome==0?105.f:p.biome==2?72.f:58.f)+p.weather*9.f;
}
static int terrain_bridge_at(const Game *g,float u,float v,float centre,float width){
 static const float crossings[6]={-7600.f,-5000.f,-2400.f,2400.f,5000.f,7600.f};
 (void)g;(void)width;for(int i=0;i<6;i++)if(fabsf(u-crossings[i])<220.f&&fabsf(v-centre)<260.f)return 1;return 0;
}
int terrain_bridge_position(const Game *g,int index,Vec3 *position,Vec3 *across){
 if(index<0||index>=6||!terrain_has_river(g))return 0;
 static const float crossings[6]={-7600.f,-5000.f,-2400.f,2400.f,5000.f,7600.f};
 float px,pz;site_xz(g,1,&px,&pz);float angle=((g->bodies[g->planet].seed>>8)%6283)*.001f,c=cosf(angle),s=sinf(angle),u=crossings[index];
 float centre=sinf(u/1180.f+(g->bodies[g->planet].seed&255)*.017f)*310.f+sinf(u/430.f)*85.f;
 float x=px+u*c-centre*s,z=pz+u*s+centre*c;if((x-px)*(x-px)+(z-pz)*(z-pz)>EVA_FIELD_RADIUS*EVA_FIELD_RADIUS)return 0;
 if(position)*position=(Vec3){x,terrain_height(g,x,z)+.7f,z};if(across)*across=(Vec3){-s,0,c};return 1;
}
/* One fixed lattice and diagonal for collision and the rendered ground mesh. */
static float terrain_vertex_raw(const Game *g,int ix,int iz){
 float px,pz;site_xz(g,1,&px,&pz);float x=ix*(float)SURFACE_CELL,z=iz*(float)SURFACE_CELL,dx=x-px,dz=z-pz;
 float distance=sqrtf(dx*dx+dz*dz),port_distance=fmaxf(fabsf(dx),fabsf(dz));
 if(g->bodies[g->planet].type==GAS)return 24;
 FieldProfile profile=field_profile(g,g->system,g->planet);float relief=terrain_relief_scale(g);
 float broad=terrain_noise(g,x,z,2600.f,0x51f15e5u),ridge=1.f-fabsf(terrain_noise(g,x,z,980.f,0xa12b31u)*2.f-1.f),detail=terrain_noise(g,x,z,330.f,0x933d7u)-.5f;
 ridge*=ridge;float height=24+(broad-.48f)*relief*.78f+(ridge-.32f)*relief*.48f+detail*relief*.14f;
 /* Seeded landmark ranges create recognisable peaks and basins. Geological
  * worlds get taller, tighter mountains; soft ocean worlds get broad hills. */
 for(int peak=0;peak<5;peak++){
  unsigned h=sector_hash(g->bodies[g->planet].seed+peak*13007u);float a=(h%6283)*.001f,r=3300.f+((h>>8)%5600),cx=px+cosf(a)*r,cz=pz+sinf(a)*r;
  float radius=profile.biome==3?1250.f:1800.f+((h>>19)%700),ddx=x-cx,ddz=z-cz,t=1.f-(ddx*ddx+ddz*ddz)/(radius*radius);
  if(t>0){t=terrain_smooth(t);height+=t*relief*(profile.biome==3?1.05f:.62f+(h&31)*.008f);}
 }
 if(g->system==7&&g->planet==1){float ox=dx,oz=dz-FIELD_OBSERVATORY_DISTANCE,t=1.f-(ox*ox+oz*oz)/(1150.f*1150.f);if(t>0)height+=terrain_smooth(t)*250.f;}
 if(terrain_has_river(g)){
  float u,v,centre,width;terrain_river_uv(g,x,z,&u,&v,&centre,&width);float bank=fabsf(v-centre);
  if(!terrain_bridge_at(g,u,v,centre,width)&&bank<width*5.f){float cut=1.f-bank/(width*5.f);height-=terrain_smooth(cut)*(profile.biome==1||profile.biome==3?relief*.34f:relief*.22f);}
 }
 if(g->bodies[g->planet].type==OCEAN&&distance>8800.f){float coast=terrain_smooth((distance-8800.f)/1000.f);height=height*(1-coast)+(-55.f)*coast;}
 /* A gentle blend protects the port and rover garage from mountain seams. */
 if(port_distance<field_port_flat(g)+520.f){float blend=terrain_smooth((port_distance-field_port_flat(g))/520.f);height=24+(height-24)*blend;}
 return height;
}
/* Exact coordinate tags keep the procedural terrain cheap on PSP while the
 * camera revisits neighbouring vertices. */
static float terrain_vertex(const Game *g,int ix,int iz){
 static struct {int x,z;unsigned seed;float height;int valid;} cache[8192];
 unsigned seed=g->bodies[g->planet].seed,index=(sector_hash(seed^(unsigned)ix*73856093u^(unsigned)iz*19349663u))&8191u;
 if(!cache[index].valid||cache[index].seed!=seed||cache[index].x!=ix||cache[index].z!=iz){cache[index].x=ix;cache[index].z=iz;cache[index].seed=seed;cache[index].height=terrain_vertex_raw(g,ix,iz);cache[index].valid=1;}
 return cache[index].height;
}
float terrain_height(const Game *g,float x,float z){
 if(g->planet<1||g->planet>=BODY_COUNT)return 0;
 int step=4;
 float gx=x/(SURFACE_CELL*step),gz=z/(SURFACE_CELL*step);int ix=(int)floorf(gx),iz=(int)floorf(gz);
 float u=gx-ix,v=gz-iz;ix*=step;iz*=step;float a=terrain_vertex(g,ix,iz);
 if(u==0&&v==0)return a;
 float c=terrain_vertex(g,ix+step,iz+step);
 if(u>=v){float b=terrain_vertex(g,ix+step,iz);return a+(b-a)*u+(c-b)*v;}
 float d=terrain_vertex(g,ix,iz+step);return a+(c-d)*u+(d-a)*v;
}
Vec3 surface_site(const Game *g,int i){float x=0,z=0;if(g->planet>=1)site_xz(g,i,&x,&z);return (Vec3){x,terrain_height(g,x,z)+22,z};}
#include "cloud-platforms.h"
int terrain_is_water(const Game *g,float x,float z){
 if(g->planet<1||g->planet>=BODY_COUNT)return 0;
 if(g->bodies[g->planet].type==GAS)return !surface_cloud_deck(g,x,z);
 /* Whole water tiles: collision, rendering and map coastlines stay identical. */
 float px,pz;site_xz(g,1,&px,&pz);
 float tx=(floorf(x/SURFACE_CELL)+.5f)*SURFACE_CELL,tz=(floorf(z/SURFACE_CELL)+.5f)*SURFACE_CELL,dx=tx-px,dz=tz-pz;
 if(fmaxf(fabsf(dx),fabsf(dz))<field_port_flat(g)+420.f)return 0;
 if(g->bodies[g->planet].type==OCEAN&&dx*dx+dz*dz>=9800.f*9800.f)return 1;
 if(terrain_has_river(g)){float u,v,centre,width;terrain_river_uv(g,tx,tz,&u,&v,&centre,&width);if(fabsf(v-centre)<width&&!terrain_bridge_at(g,u,v,centre,width))return 1;}
 return 0;
}
static int eva_position_allowed(const Game *g,float x,float z);
#include "surface-activities.h"
void surface_ship_bounds(const Game *g,float *hw,float *hd){*hw=*hd=8;if(g->ship<0||g->ship>=player_ship_count)return;const Mesh *m=&meshes[mesh_id(player_ships[g->ship].name)];for(int i=0;i<m->vertices;i++){*hw=fmaxf(*hw,fabsf(m->v[i].x)*.72f+8);*hd=fmaxf(*hd,fabsf(m->v[i].z)*.72f+8);}}
int eva_can_board(const Game *g){
 if(g->planet<1||g->planet>=BODY_COUNT||g->system<0||g->system>=256||g->ship<0||g->ship>=player_ship_count||g->surface!=2||g->dead||g->rover_driving)return 0;
 float dx=g->pos.x-g->ship_pos.x,dz=g->pos.z-g->ship_pos.z;
 float hw,hd;surface_ship_bounds(g,&hw,&hd);return fabsf(dx)<=hw+30&&fabsf(dz)<=hd+30&&fabsf(g->pos.y-(terrain_height(g,g->pos.x,g->pos.z)+22))<=2;
}
static int eva_position_allowed(const Game *g,float x,float z){
 if(g->planet<1||g->planet>=BODY_COUNT||g->system<0||g->system>=256||g->ship<0||g->ship>=player_ship_count||!isfinite(x)||!isfinite(z))return 0;
 if(g->bodies[g->planet].type==GAS){float margin=g->rover_driving?18:10;if(!surface_cloud_deck(g,x-margin,z)||!surface_cloud_deck(g,x+margin,z)||!surface_cloud_deck(g,x,z-margin)||!surface_cloud_deck(g,x,z+margin))return 0;}
 Vec3 pad=surface_site(g,1);float dx=x-pad.x,dz=z-pad.z;Vec3 sites[10];FieldBuilding shapes[10];for(int i=1;i<10;i++){if(i==6)continue;sites[i]=surface_poi(g,i);shapes[i]=field_site_building(g,i);}
 if(g->system==7&&g->planet==3)for(int i=0;i<12;i++){float px=pad.x-250+(i%6)*95,pz=pad.z+(i<6?-235:235);if(fabsf(x-px)<13&&fabsf(z-pz)<13)return 0;}
 for(int i=0;i<FIELD_PORT_BUILDINGS;i++){FieldBuilding structure=field_port_building(g,i);const FieldBuilding *b=&structure;if(fabsf(dx-b->x)<b->w+8&&fabsf(dz-b->z)<b->d+8)return 0;}
 float hw,hd;surface_ship_bounds(g,&hw,&hd);if(fabsf(x-g->ship_pos.x)<hw&&fabsf(z-g->ship_pos.z)<hd)return 0;
 /* Reserved traffic pads keep walkers clear of descending hulls. */
 for(int i=0;i<2;i++)if(fabsf(dx-(i?FIELD_TRAFFIC_X:-FIELD_TRAFFIC_X))<FIELD_TRAFFIC_PAD+8&&fabsf(dz-FIELD_TRAFFIC_Z)<FIELD_TRAFFIC_PAD+8)return 0;
 for(int i=0;i<3;i++){const FieldBuilding *b=&field_garage_walls[i];float margin=g->rover_driving?14:6;if(fabsf(dx-b->x)<b->w+margin&&fabsf(dz-b->z)<b->d+margin)return 0;}
 if(!g->rover_driving&&fabsf(x-g->rover_pos.x)<19&&fabsf(z-g->rover_pos.z)<24)return 0;
 if(g->system==7&&g->planet==1){
  int cx=(int)floorf(x/70.f),cz=(int)floorf(z/70.f);
  for(int iz=-1;iz<=1;iz++)for(int ix=-1;ix<=1;ix++){
   float tx,tz;unsigned h;if(!field_lave_tree_cell(g->bodies[g->planet].seed,cx+ix,cz+iz,&tx,&tz,&h))continue;
   if(fabsf(tx-pad.x)<field_port_clear(g)&&fabsf(tz-pad.z)<field_port_clear(g))continue;
   float t=(tz-pad.z)/FIELD_OBSERVATORY_DISTANCE,side=g->bodies[g->planet].seed&1?1.f:-1.f;
   if(t>0&&t<1&&fabsf(tx-(pad.x-side*420*t*(1-t)))<38)continue;
   int reserved=0;for(int i=1;i<10;i++){if(i==6)continue;if(fabsf(tx-sites[i].x)<shapes[i].w+30&&fabsf(tz-sites[i].z)<shapes[i].d+30){reserved=1;break;}}
   if(reserved||terrain_is_water(g,tx,tz))continue;
   float trunk=g->rover_driving?30.f:22.f;if((x-tx)*(x-tx)+(z-tz)*(z-tz)<trunk*trunk)return 0;
  }
 }
 if(g->bodies[g->planet].type==ROCKY){
  int tile=g->system==7?70:90,cx=(int)floorf(x/tile),cz=(int)floorf(z/tile);
  for(int iz=-1;iz<=1;iz++)for(int ix=-1;ix<=1;ix++){
   unsigned h;float tx,tz;int kind=field_prop_cell(g->system,g->planet,g->bodies[g->planet].seed,cx+ix,cz+iz,&tx,&tz,&h);
   if(fabsf(tx-pad.x)<field_port_clear(g)&&fabsf(tz-pad.z)<field_port_clear(g))continue;
   int reserved=0;for(int i=1;i<10;i++){if(i==6)continue;if(fabsf(tx-sites[i].x)<shapes[i].w+30&&fabsf(tz-sites[i].z)<shapes[i].d+30){reserved=1;break;}}if(reserved)continue;
   int biome=field_profile(g,g->system,g->planet).biome;if(g->system!=7&&biome!=4&&(h&3)==0)continue;
   if(biome==1||biome==3)kind=kind==0?2:kind;
   float radius=kind==0?(g->rover_driving?30:22):g->system==7&&kind==2?(g->rover_driving?22:14):0;
   if(radius>0&&(x-tx)*(x-tx)+(z-tz)*(z-tz)<radius*radius)return 0;
  }
 }
 for(int i=1;i<10;i++){if(i==6)continue;Vec3 p=sites[i];FieldBuilding b=shapes[i];float margin=g->rover_driving?18:8;if(fabsf(x-p.x)<b.w+margin&&fabsf(z-p.z)<b.d+margin)return 0;}
 return dx*dx+dz*dz<=EVA_FIELD_RADIUS*EVA_FIELD_RADIUS&&!terrain_is_water(g,x,z);
}
#include "fauna-behaviour.h"
#include "fauna-tests.h"
#include "lave-world-tests.h"
#include "starport-layout-tests.h"
int enter_planet(Game *g){
 if(g->dead||g->docked||g->dock_stage||g->police_stop||g->jump>0||g->planet>=0)return 0;
 int body=g->approach;if(body<1||body>=BODY_COUNT)return 0;if(g->bodies[body].type==SUN){message(g,"No solid landing surface. Circle turns back.");return 0;}
 g->orbit_pos=g->pos;g->orbit_yaw=g->yaw;g->orbit_pitch=g->pitch;g->orbit_roll=g->roll;g->orbit_speed=g->speed>40?g->speed:80;
 g->planet=body;g->approach=-1;g->surface=0;g->boost=0;g->rover_driving=0;g->rover_pos=surface_site(g,1);g->rover_pos.x+=FIELD_GARAGE_X;g->rover_pos.z-=10;g->rover_pos.y=terrain_height(g,g->rover_pos.x,g->rover_pos.z)+22;
 float px,pz;site_xz(g,1,&px,&pz);g->pos=(Vec3){px-220,0,pz-60};g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+170;
 g->yaw=atan2f(px-g->pos.x,pz-g->pos.z);g->pitch=-.22f;g->roll=0;g->speed=48;g->hazard=0;g->cue=SFX_LAND;
 for(int i=0;i<LIFE_COUNT;i++){Lifeform *l=&g->life[i];l->alive=1;l->scanned=(g->surface_progress[g->system][body]>>(8+i))&1;unsigned h=sector_hash(g->bodies[body].seed+i*131u);l->kind=field_species_kind(g->system,body,i);float a=i*.95f+(h%7)*.1f;float rad=100.f+(h%90)+i*18;l->pos=(Vec3){px+cosf(a)*rad,0,pz+sinf(a)*rad};if(terrain_is_water(g,l->pos.x,l->pos.z)){l->pos.x=px+cosf(a)*140;l->pos.z=pz+sinf(a)*140;}l->pos.y=terrain_height(g,l->pos.x,l->pos.z)+(l->kind==LIFE_FAUNA?16:7);}
 for(int i=0;i<LIFE_COUNT;i++){Lifeform *l=&g->life[i];if(l->kind==LIFE_FAUNA)fauna_init(g,i);else {l->pos.x=px+(i&1?-1:1)*(170+(i%4)*22);l->pos.z=pz-248;l->pos.y=terrain_height(g,l->pos.x,l->pos.z)+7;}}
 message(g,"Atmosphere. Guided landing is available at the spaceport.");
 {
  const Body *wb=&g->bodies[body];
  const char *line="Rocky surface. Cyan pad in the scrub ahead.";
  if(g->system==7&&body==1)line="Lave I. Green island country, a bright coast and the observatory ridge.";
  else if(g->system==7&&body==2)line="Lave II. Mosswood expedition country: wooded hills, research outposts and old survey sites.";
  else if(g->system==7&&body==3)line="Lave III. Skyport above the cloud sea. Stay on the linked decks; the safety rails mark the edge.";
  else if(g->system==7&&body==4)line="Lave IV. Glacial survey range. Snow-covered conifers, pale ice and sheltered research stations.";
  else if(wb->type==GAS)line="Gas giant: a floating research platform above the cloud sea.";else if(wb->type==OCEAN)line="Ocean world. Island pad ahead.";
  else {int art=(int)(wb->seed%4);if(art==0)line="Arid flats. Cyan pad in the dunes.";else if(art==1)line="Ice field. Cyan pad on the shelf.";else if(art==2)line="Volcanic scrub. Cyan pad ahead.";else line="Forest rise. Cyan pad in the trees.";}
  speak(g,VOICE_COMP,line);
 }
 story_event(g,STORY_EV_WORLD);return 1;
}
void leave_planet(Game *g){
 if(g->planet<0)return;
 if(g->surface==2){message(g,"Board the ship before leaving the planet.");return;}
 if(g->surface==1){message(g,"Take off before returning to orbit.");return;}
 g->planet_sequence=0;int body=g->planet;g->planet=-1;g->surface=0;
 g->pos=g->orbit_pos;g->yaw=g->orbit_yaw;g->pitch=g->orbit_pitch;g->roll=g->orbit_roll;g->speed=g->orbit_speed;
 Vec3 away=norm(sub(g->pos,g->bodies[body].pos));g->yaw=atan2f(away.x,away.z);g->pitch=asinf(fmaxf(-1,fminf(1,away.y)));g->boost=0;
 message(g,"Returned to orbit.");
}
int land_planet(Game *g){
 if(g->planet<0||g->surface||g->dead)return 0;
 if(!story_landing_ready(g)){message(g,"Landing kit required. Return to the hub and follow Kei's briefing.");return 0;}
 float ground=terrain_height(g,g->pos.x,g->pos.z),alt=g->pos.y-ground;
 float px,pz;site_xz(g,1,&px,&pz);float pad=sqrtf((g->pos.x-px)*(g->pos.x-px)+(g->pos.z-pz)*(g->pos.z-pz));
 if(g->speed>48){message(g,"Too fast to land. Slow down.");return 0;}
 if(alt>48){message(g,"Too high. Descend toward the pad.");return 0;}
 if(terrain_is_water(g,g->pos.x,g->pos.z)){message(g,"Cannot land on water. Reach the cyan pad.");return 0;}
 if(pad>160){message(g,"Land on the cyan settlement pad ahead.");return 0;}
 g->surface=1;g->speed=0;g->boost=0;g->pitch=0;g->roll=0;g->pos=(Vec3){px,terrain_height(g,px,pz)+18,pz};g->yaw=0;g->ship_pos=g->pos;g->cue=SFX_LAND;
 int bit=1<<(g->planet-1),first_landing=!(g->landed_planets[g->system]&bit);g->landed_planets[g->system]|=(uint8_t)bit;social_emit(g,SB_LAND);
 message(g,"Landing secured. Preparing the airlock.");if(first_landing){char brief[160];snprintf(brief,sizeof(brief),"Planet %s added to your Discovery Codex. Pad locked. Preparing the airlock.",g->bodies[g->planet].name);speak(g,VOICE_COMP,brief);}else speak(g,VOICE_COMP,"Pad locked. Preparing the airlock.");return 1;
}
int takeoff_planet(Game *g){
 if(g->planet<0||g->surface!=1){if(g->surface==2)message(g,"Board the ship before takeoff.");return 0;}
 g->surface=0;g->speed=48;g->pitch=.2f;g->pos.y+=28;g->cue=SFX_BOOST;message(g,"Departure confirmed. Climbing to orbit.");return 1;
}
int disembark_planet(Game *g){
 if(g->planet<1||g->planet>=BODY_COUNT||g->ship<0||g->ship>=player_ship_count||g->surface!=1||g->dead)return 0;
 /* Pick a validated exit before committing the on-foot state. */
 g->ship_pos=g->pos;float hw,hd;surface_ship_bounds(g,&hw,&hd);Vec3 exit=g->pos;int safe=0;
 for(int i=0;i<16;i++){
  float a=(i%8)*.785398f;
  float x=g->ship_pos.x+cosf(a)*(hw+18+(i/8)*8),z=g->ship_pos.z+sinf(a)*(hd+18+(i/8)*8);
  if(eva_position_allowed(g,x,z)){exit=(Vec3){x,terrain_height(g,x,z)+22,z};safe=1;break;}
 }
 if(!safe||!isfinite(exit.x)||!isfinite(exit.y)||!isfinite(exit.z)){message(g,"Airlock blocked. Remain aboard; launch is available.");return 0;}
 g->planet_sequence=0;g->pos=exit;g->surface=2;g->rover_driving=0;
 g->yaw=atan2f(g->ship_pos.x-exit.x,g->ship_pos.z-exit.z);g->pitch=g->roll=0;g->speed=0;g->boost=0;g->jetpack=0;
 g->eva_run_arm=g->eva_running=g->eva_jump_held=0;g->eva_run_hold=0;
 g->voice_time=0;g->voice[0]=0;message(g,"On foot. Explore the planet; board beside your ship.");
 return 1;
}
int board_planet(Game *g){
 if(!eva_can_board(g)){message(g,"Stand beside your parked ship to board.");return 0;}
 g->planet_sequence=0;g->pos=g->ship_pos;g->surface=1;g->speed=0;g->yaw=0;g->pitch=g->roll=g->jetpack=0;g->boost=0;
 g->eva_run_arm=g->eva_running=g->eva_jump_held=0;g->eva_run_hold=0;
 g->voice_time=0;g->voice[0]=0;message(g,"Aboard. Choose launch or step outside.");return 1;
}
/* Model compatibility for saved-game/tests; frontend uses explicit actions. */
int eva_toggle(Game *g){return g->surface==1?disembark_planet(g):board_planet(g);}

static void player_damage_kind(Game *g,float amount,const char *note,int attack){
 if(amount<=0||g->dead||g->docked)return;
 if(g->hull<=0&&!g->damaged)g->hull=100;
 if(g->energy>0){float absorbed=fminf(g->energy,amount);g->energy-=absorbed;amount-=absorbed;}
 if(g->energy<=0){g->energy=0;g->damaged=1;}
 if(amount>0){g->damaged=1;g->hull-=fmaxf(.5f,amount*.45f)*(fit_find(g,32)>=0?.8f:1.f);}
 g->damage_fx=fmaxf(g->damage_fx,1.2f);if(attack)g->attacked=fmaxf(g->attacked,1.5f);g->cue=SFX_HIT;
 if(note&&g->message_time<=0)message(g,note);
 if(g->hull<=0){g->hull=0;g->energy=0;g->dead=1;g->boost=0;g->jump=0;g->explosion=0;g->cue=SFX_DEATH;message(g,"Hull integrity lost. START to recover.");}
}
static void player_damage(Game *g,float amount,const char *note){player_damage_kind(g,amount,note,1);}
/* Thermal stress retains shake and shield/hull damage, not a combat alert. */
static void collision_damage(Game *g,float amount,const char *note){player_damage_kind(g,amount*(fit_find(g,36)>=0?.75f:1.f),note,1);}
static void thermal_damage(Game *g,float amount,const char *note){player_damage_kind(g,amount*(fit_find(g,35)>=0?.7f:1.f),note,0);}
int ship_repair_cost(const Game *g){
 float hull=g->hull<=0&&!g->damaged?100:g->hull;
 if(!g->damaged&&hull>=99.9f&&g->energy>=99.9f&&g->heat<=.1f)return 0;
 int cost=250+(int)((100.f-fmaxf(0,fminf(100,hull)))*35.f)+(int)((100.f-fmaxf(0,fminf(100,g->energy)))*8.f);
 return cost<100?100:cost;
}
int repair_ship(Game *g){
 if(!g->docked){message(g,"Dock at a station to call the engineers.");return 0;}
 int cost=ship_repair_cost(g);
 if(cost<=0){message(g,"Engineers report no hull damage.");return 0;}
 if(g->credits<cost){char note[96];snprintf(note,sizeof(note),"Engineers need %.1f units for the repair.",cost*.1f);message(g,note);return 0;}
 g->credits-=cost;g->energy=100;g->hull=100;g->damaged=0;g->damage_fx=0;g->heat=0;g->cue=SFX_DOCK;
 message(g,"Engineers repaired the hull, shields and heat sinks.");speak(g,VOICE_DOCK,"The ship is sound again, Commander.");return 1;
}
#include "roamer-physics.h"
static void planet_tick(Game *g,float dt,float turn,float pitch,int throttle,float strafe){
 g->heat=fmaxf(0,g->heat-dt*22);g->shot=fmaxf(0,g->shot-dt);g->energy=fminf(100,g->energy+dt*1.5f);
 if(g->surface==2){
  if(g->rover_driving){rover_step(g,dt,turn,pitch,throttle);return;}
  int jump_press=g->boost&&!g->eva_jump_held;g->eva_jump_held=g->boost!=0;
  g->yaw+=turn*dt*2.2f;g->pitch=fmaxf(-.75f,fminf(.75f,g->pitch+pitch*dt*1.6f));g->roll=0;
  float maxpace=g->rover_driving?190.f:g->eva_running?100.f:62.f;float walk=throttle>0?maxpace:throttle<0?-maxpace*.45f:0,side=strafe*maxpace*.72f;
  float pace=sqrtf(walk*walk+side*side);if(pace>maxpace){walk*=maxpace/pace;side*=maxpace/pace;}if(g->rover_driving){g->boost=0;g->jetpack=0;}
  Vec3 before=g->pos;
  float sy=sinf(g->yaw),cy=cosf(g->yaw);
  int steps=pace>0?(int)ceilf(fmaxf(1,pace*dt/4)):0;for(int step=0;step<steps;step++){float nx=g->pos.x+(sy*walk+cy*side)*dt/steps,nz=g->pos.z+(cy*walk-sy*side)*dt/steps;
  if(eva_position_allowed(g,nx,nz)){g->pos.x=nx;g->pos.z=nz;}
  else {
   /* Slide along a shore/field edge, never wrap to the far side of the map. */
   if(eva_position_allowed(g,nx,g->pos.z))g->pos.x=nx;
   if(eva_position_allowed(g,g->pos.x,nz))g->pos.z=nz;
   if(g->message_time<=0)message(g,g->system==7&&g->planet==3?"Safety rail. Follow the connected decks.":terrain_is_water(g,nx,nz)?"Shoreline. Find a route along the coast.":"Field edge or obstacle. Find a clear route.");
  }
  }if(g->rover_driving)g->rover_pos=g->pos;
  float oldfloor=terrain_height(g,before.x,before.z)+22;
  float floor=terrain_height(g,g->pos.x,g->pos.z)+22;
  int grounded=g->pos.y<=oldfloor+.5f&&g->jetpack<=0;
  if(grounded)g->pos.y=floor;
  /* One short grounded impulse. Holding R cannot hover/retrigger, and
   * another press in mid-air cannot add velocity or queue a landing hop. */
  if(jump_press&&grounded&&!g->rover_driving){g->jetpack=62;g->cue=SFX_BOOST;}
  else g->jetpack=fmaxf(-100,g->jetpack-140*dt);
  g->boost=0;
  g->pos.y+=g->jetpack*dt;
  fauna_tick(g,dt);
  Vec3 garage=surface_site(g,1);if(fabsf(g->pos.x-(garage.x+FIELD_GARAGE_X))<40&&g->pos.z>garage.z-60&&g->pos.z<garage.z+24&&g->pos.y>floor+22){g->pos.y=floor+22;g->jetpack=0;}
  if(g->pos.y>floor+120){g->pos.y=floor+120;g->jetpack=0;}
  if(g->pos.y<floor){g->pos.y=floor;if(g->jetpack<0)g->jetpack=0;}
  float px,pz;site_xz(g,1,&px,&pz);float pd=(g->pos.x-px)*(g->pos.x-px)+(g->pos.z-pz)*(g->pos.z-pz);
  if(pd>280*280&&!g->rover_driving){g->hazard=fminf(100,g->hazard+dt*(g->bodies[g->planet].type==OCEAN?.45f:.8f));if(g->hazard>=100)player_damage(g,dt*10,"Surface hazard is burning through the shields.");}
  else g->hazard=fmaxf(0,g->hazard-dt*22);
  g->speed=sqrtf((g->pos.x-before.x)*(g->pos.x-before.x)+(g->pos.z-before.z)*(g->pos.z-before.z))/dt;
  if(g->hull<=0){g->dead=1;g->jump=0;g->explosion=0;g->cue=SFX_DEATH;message(g,"Hull integrity lost. START to recover.");}
  return;
 }
 if(g->surface==1){
  g->yaw+=turn*dt*1.2f;g->pitch+=pitch*dt;if(g->pitch>.5f)g->pitch=.5f;if(g->pitch<-.2f)g->pitch=-.2f;
  g->speed=0;g->boost=0;g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+18;return;
 }
 flight_steer(g,turn,pitch,dt*1.4f);
 g->speed+=throttle*dt*(g->boost?700:110);if(g->speed<0)g->speed=0;float maxspeed=160*(g->boost?2.2f:1.f);if(g->speed>maxspeed)g->speed=maxspeed;
 g->pos=add(g->pos,mul(forward(g),g->speed*dt));
 if(!g->boost)g->pos.y-=32*dt;
 if(throttle>0&&g->pitch>0.08f)g->pos.y+=throttle*dt*55;
 g->pos.x=wrap_range(g->pos.x,4200);g->pos.z=wrap_range(g->pos.z,4200);
 float ground=terrain_height(g,g->pos.x,g->pos.z)+16;
 if(g->pos.y<ground){
  int water=terrain_is_water(g,g->pos.x,g->pos.z);g->pos.y=ground;
  if(g->speed>48||water){collision_damage(g,water?18:12,water?"Collision: ocean surface. Shields damaged.":"Collision: terrain. Shields damaged.");g->collision=2;snprintf(g->collide,sizeof(g->collide),water?"OCEAN SURFACE":"PLANET TERRAIN");}
  g->speed*=.32f;if(g->pitch<0)g->pitch=0;g->boost=0;
 }
 if(g->pos.y>780){leave_planet(g);if(g->planet<0)message(g,"Climbed out of the atmosphere. Orbit restored.");return;}
 if(g->hull<=0){g->dead=1;g->jump=0;g->energy=0;g->explosion=0;g->cue=SFX_DEATH;message(g,"Hull integrity lost. START to recover.");}
}
static int spawn_debris(Game *g,Vec3 pos,Vec3 vel,int good,int qty,int wreck){
 for(int i=0;i<DEBRIS_COUNT;i++)if(!g->debris[i].alive){Debris *d=&g->debris[i];memset(d,0,sizeof(*d));d->radius=24;d->alive=1;d->pos=pos;d->vel=vel;d->good=good;d->qty=qty>0?qty:1;d->wreck=wreck;d->life=wreck?90:240;return i;}
 return -1;
}
static int loot_good(Game *g,const NPC *n){
 if(n->role==PIRATES){const int items[]={6,9,10};return items[random_u(g)%3];}
 if(n->role==EXPLORERS)return (random_u(g)&1)?12:16;
 if(n->role==LAW)return 9;
 {const int items[]={0,1,7,8,12};return items[random_u(g)%5];}
}
static void wreck_from_npc(Game *g,int i){
 NPC *n=&g->npc[i];Vec3 drift=mul(n->dir,18);spawn_debris(g,n->pos,drift,-1,80+n->role*40,1);
 int pods=n->freighter?(n->freight_qty>1?2:n->freight_qty):1;
 for(int p=0;p<pods;p++){Vec3 offset={(p?1:-1)*40.f,12.f+p*8,p*25.f};int qty=n->freighter?(n->freight_qty/pods+(p<n->freight_qty%pods)):1;
  spawn_debris(g,add(n->pos,offset),add(drift,(Vec3){offset.x*.2f,4,offset.z*.1f}),n->freighter?n->freight_good:loot_good(g,n),qty,0);
 }
}
#include "freight.h"
static float npc_max_health(const Game *g,const NPC *n);
static float npc_max_shield(const Game *g,const NPC *n);
static void place_civilian(Game *g,NPC *n,int i){
 Vec3 stn={0,0,3500};
 /* Spin, stretch and lift traffic per system so each star feels differently occupied. */
 unsigned layout=sector_hash((g->system+1)*0x85ebca6bu+i*97u);
 float spin=g->system*.41f+(layout&255)*.004f,spread=0.72f+((g->system*7+layout)%55)*.012f;
 float lift=((int)((layout>>8)%900)-450)*.08f;
 n->target=-1;n->flash=0;n->dir=(Vec3){0,0,1};
 if(n->freighter){n->alive=0;return;}
 if(n->role==EXPLORERS){Body *b=&g->bodies[1+(g->system%3)];int wing=0;for(int j=0;j<i;j++)if(g->npc[j].alive&&g->npc[j].role==EXPLORERS)wing++;float a=.35f*wing+spin;float ring=b->radius+1800*spread+((layout>>12)%900);n->waypoint=1;n->pos=add(b->pos,(Vec3){cosf(a)*ring+wing*210.f,280+lift+((g->system+wing)%5)*40.f,sinf(a)*ring});n->dir=norm((Vec3){-sinf(a),0,cosf(a)});return;}
 if(n->role==PIRATES){int world=1+(g->system+i)%3;if(world>=BODY_COUNT)world=2;n->waypoint=world;if(i==2){float a=spin+1.1f;float ring=1600.f+spread*900.f;n->pos=(Vec3){cosf(a)*ring,180+lift,3500+sinf(a)*ring};n->dir=norm(sub(stn,n->pos));return;}Body *b=&g->bodies[world];n->pos=add(b->pos,(Vec3){b->radius+1600*spread+(i%3)*220,160+lift+((g->system+i)%4)*70.f,(int)((layout>>4)%800)-400});n->dir=(Vec3){0,0,-1};return;}
 if(n->role==LAW){float a=spin+(i%4)*.7f;float ring=(700.f+((layout>>6)%500))*spread;n->waypoint=0;n->pos=(Vec3){cosf(a)*ring,90+lift+(i%3)*40.f,3500+sinf(a)*ring};n->dir=norm(sub(stn,n->pos));return;}
 if(i==0||i==4){float a=spin+(i?1.2f:-.4f);float ring=1100.f+spread*600.f;n->waypoint=0;n->pos=(Vec3){cosf(a)*ring,60+lift,3500+sinf(a)*ring};n->dir=norm(sub(stn,n->pos));return;}
 int body=1+(i+g->system)%4;Vec3 end=traffic_world_point(g,body);
 n->pos=add(stn,mul(sub(end,stn),.18f+(layout%60)*.01f));n->pos.y+=lift;
 n->waypoint=body;n->dir=norm(sub(end,n->pos));
}
#include "travellers.h"
static void travellers_promote(Game *g){
 travellers_clear_slots(g);
 int promoted=0;
 for(int i=0;i<TRAVELLER_COUNT&&promoted<3;i++){
  if(g->travellers[i].sys!=g->system)continue;
  const TravellerDef *d=&traveller_defs[i];
  int slot=travellers_find_slot(g,d->role);if(slot<0)continue;
  NPC *n=&g->npc[slot];
  n->role=d->role;n->freighter=0;n->alive=1;n->traveller=(int8_t)i;n->target=-1;n->cooldown=0;n->flash=0;
  n->scale=1;n->cruise=d->role==LAW?700:650;
  n->mesh=mesh_id(d->role==LAW?"VIPER":d->role==PIRATES?"MAMBA":d->role==EXPLORERS?"ADDER":"COBRA MK 3");
  n->radius=30;for(int v=0;v<meshes[n->mesh].vertices;v++)n->radius=fmaxf(n->radius,length(meshes[n->mesh].v[v])*n->scale);
  n->health=d->role==LAW?110:80;n->shield=d->role==LAW?60:40;
  place_civilian(g,n,slot);
  n->pos=add(n->pos,(Vec3){(i%3-1)*120.f,40.f+(i%2)*30.f,(i&1)?-80.f:90.f});
  g->travellers[i].slot=(int8_t)slot;promoted++;
 }
}
#include "traffic-routes.h"
void game_spawn(Game *g){
 if(g->hull<=0&&!g->damaged)g->hull=100;
 jobs_from_legacy(g);
 g->rift_report=0;g->departure_glow=0;memset(g->fire_bearing_time,0,sizeof(g->fire_bearing_time));g->missile_trail_n=0;system_bodies(g);g->dock_stage=0;g->station_variant=0;g->approach=-1;g->planet=-1;g->surface=0;g->boost=0;g->attacked=0;g->encounter=0;g->police_stop=0;g->police_phase=0;g->police_warning=0;g->police_warned=0;g->police_timer=0;g->missile_time=0;g->missile_target=-1;g->incoming_missile=0;g->incoming_source=-1;
 for(int i=0;i<DEBRIS_COUNT;i++)g->debris[i].alive=0;
 /* Belt geometry and mineable objects share one bounded pool. */
 for(int band=0;band<2;band++){
  int count=band?(system_ice_belt(g->system)?10+(g->system%5):0):(system_rock_belt(g->system)?18+(g->system%11):3);
  Body *b=&g->bodies[band?3:2];
  for(int i=0;i<count;i++){
   unsigned h=sector_hash(g->system*911u+i*379u+band*12347u);float a=(h%6283)*.001f,r=b->radius+2100+((h>>8)%1800);
   Vec3 p=add(b->pos,(Vec3){cosf(a)*r,(int)((h>>19)%1400)-700,sinf(a)*r});
   int slot=spawn_debris(g,p,(Vec3){-sinf(a)*1.2f,0,cosf(a)*1.2f},12,1,0);
   if(slot>=0){Debris *d=&g->debris[slot];d->rock=band?2:1;d->radius=45+(h%66);d->health=36+(h%3)*18;d->life=20000;d->qty=1+(h%3);}
  }
 }
 spawn_debris(g,(Vec3){180+((int)g->system%9)*90.f,50+((int)g->system%5)*20.f,1800.f+(g->system%11)*140.f},(Vec3){6,1,-4},9,1,1);
 spawn_debris(g,(Vec3){-420-((int)g->system%7)*70.f,80-((int)g->system%4)*15.f,1600.f+(g->system%13)*110.f},(Vec3){-3,1,5},0,1,0);
 spawn_debris(g,(Vec3){520-((int)g->system%6)*55.f,-40+((int)g->system%3)*25.f,2200.f+(g->system%9)*160.f},(Vec3){4,0,-2},8,1,0);
 for(int i=0;i<ANOMALY_COUNT;i++)g->anomaly[i].alive=0;
 int ac=0;if(g->system==7||g->system%11==0)ac=1;if(danger_rating(g,g->system)>=4)ac++;if(g->system%23==0)ac=2;if(ac>ANOMALY_COUNT)ac=ANOMALY_COUNT;
 for(int i=0;i<ac;i++){Anomaly *a=&g->anomaly[i];a->alive=1;a->scanned=(g->rift_logged[g->system]>>i)&1;a->kind=i&1;unsigned h=sector_hash(g->system*401u+i*9973u);float ang=i*2.15f+g->system*.27f+(h%400)*.001f;float ring=3200.f+((h>>8)%4200);a->pos=(Vec3){cosf(ang)*ring,((int)((h>>16)%1600)-800),sinf(ang)*ring*.75f+(int)((h>>20)%900)-200};}
 mark_visited(g);
 int budget=traffic_budget(g);
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&g->npc[i];memset(n,0,sizeof(*n));n->traveller=-1;n->bounty_slot=-1;
  n->role=i>=36?LAW:npc_role_for(g,i);n->alive=0;n->waypoint=0;n->target=-1;n->cooldown=0;n->flash=0;
  npc_blueprint(g,n,i);n->health=n->freighter?900:n->role==LAW?110:80;n->shield=n->freighter?100:n->role==LAW?60:40;
  if(i>=36){n->role=LAW;n->mesh=mesh_id("VIPER");n->alive=0;n->radius=80;n->scale=1;n->cruise=650;n->freighter=0;n->dir=(Vec3){0,0,1};continue;}
  n->alive=i<budget;if(!n->alive){n->dir=(Vec3){0,0,1};n->pos=(Vec3){0,0,8000};continue;}
  place_civilian(g,n,i);
 }
 /* Every local system has a complete five-poster board. These are persistent
  * local targets, so traffic respawning cannot replace a claimed poster. */
 for(int poster=0;poster<BOUNTY_POSTER_COUNT;poster++){
  int i=BOUNTY_NPC_FIRST+poster;NPC *n=&g->npc[i];
  n->bounty_slot=poster;n->role=PIRATES;n->freighter=0;n->traveller=-1;n->scale=1;n->cruise=650;n->mesh=mesh_id("MAMBA");n->radius=30;
  for(int v=0;v<meshes[n->mesh].vertices;v++)n->radius=fmaxf(n->radius,length(meshes[n->mesh].v[v])*n->scale);
  n->health=npc_max_health(g,n);n->shield=npc_max_shield(g,n);n->target=-1;n->cooldown=0;n->flash=0;n->alive=!bounty_target_taken(g,poster);
  if(n->alive){place_civilian(g,n,i);n->waypoint=1+(poster%3);n->dir=norm(sub((Vec3){0,0,3500},n->pos));}
 }
 /* Keep explorer wings aligned at spawn so formation reads clearly. */
 {int lead=-1;for(int i=0;i<36;i++)if(g->npc[i].alive&&g->npc[i].role==EXPLORERS){if(lead<0){lead=i;continue;}g->npc[i].dir=g->npc[lead].dir;g->npc[i].waypoint=g->npc[lead].waypoint;}}
 g->freight_next=freight_interval(g,sector_hash(g->system*13u));g->freight_gap=45;
 int initial_freight=freight_capacity(g);if(g->system!=7&&initial_freight>0)initial_freight=(g->system+1)%(initial_freight+1);
 for(int i=8;i<36;i+=12){NPC *n=&g->npc[i];n->alive=0;n->freight_state=FREIGHT_ABSENT;n->freight_timer=0;if(initial_freight>0&&freight_begin(g,n,i,1))initial_freight--;}
 int claimed[NPC_COUNT]={0};for(int s=0;s<g->job_n;s++){Job *j=&g->jobs[s];if(j->dest!=g->system||j->stage||(j->type!=MISSION_BOUNTY&&j->type!=MISSION_RESCUE))continue;int role=j->type==MISSION_BOUNTY?PIRATES:EXPLORERS;j->target=-1;for(int i=0;i<36;i++)if(g->npc[i].alive&&g->npc[i].role==role&&!g->npc[i].freighter&&!claimed[i]){j->target=i;claimed[i]=1;break;}}
 for(int s=0;s<g->job_n;s++){
  Job *j=&g->jobs[s];if(j->dest!=g->system||j->stage||(j->type!=MISSION_BOUNTY&&j->type!=MISSION_RESCUE))continue;
  if(j->target>=0&&j->target<NPC_COUNT&&g->npc[j->target].alive)continue;
  int role=j->type==MISSION_BOUNTY?PIRATES:EXPLORERS,slot=-1;
  for(int i=0;i<36;i++)if(!g->npc[i].alive&&i%12!=8&&g->npc[i].bounty_slot<0){slot=i;break;}
  if(slot<0)for(int i=0;i<36;i++)if(!g->npc[i].freighter&&g->npc[i].bounty_slot<0){slot=i;break;}
  if(slot<0)continue;
  NPC *n=&g->npc[slot];n->role=role;n->freighter=0;n->scale=1;n->cruise=650;n->mesh=mesh_id(role==PIRATES?"MAMBA":"ADDER");n->radius=30;
  for(int v=0;v<meshes[n->mesh].vertices;v++)n->radius=fmaxf(n->radius,length(meshes[n->mesh].v[v])*n->scale);
  n->alive=1;n->health=80;n->shield=40;n->target=-1;n->cooldown=0;n->flash=0;n->waypoint=role==PIRATES?2:1;
  if(role==PIRATES){n->pos=(Vec3){1600,120,3800};n->dir=(Vec3){0,0,-1};}
  else {Body *b=&g->bodies[1];n->pos=add(b->pos,(Vec3){b->radius+900.f,80,0});n->dir=norm(sub((Vec3){0,0,3500},n->pos));}
  j->target=slot;
 }
 travellers_promote(g);
 traffic_network(g);for(int i=0;i<NPC_COUNT;i++)traffic_assign(g,&g->npc[i],i,1);
 jobs_sync(g);
}
void game_init(Game *g){memset(g,0,sizeof(*g));snprintf(g->commander_name,sizeof(g->commander_name),"JAMESON");g->rng=0x19841991;g->ai_phase=-1;g->flare_charges=3;galaxy(g->systems);g->system=7;g->social.clock=1;g->social.seen[7]=social_now(g);g->destination=129;g->route_goal=-1;g->passenger_dest=-1;g->trader_offer_system=-1;g->trader_offer_npc=-1;g->encounter_npc=-1;g->encounter_payload=-1;g->tractor_target=-1;g->credits=1000;g->fuel=60;g->energy=100;g->docked=1;g->contract=-1;g->mission_target=-1;g->missile_target=-1;g->incoming_source=-1;g->approach=-1;g->planet=-1;g->missiles=1;g->pip_sys=2;g->pip_eng=2;g->pip_wep=4;fit_clear_all(g);travellers_seed(g);market(g);game_spawn(g);g->cargo[0]=2;message(g,"X opens the deck.");speak(g,VOICE_KEI,"Kei Aven. Ryn is missing — and this berth is yours until we find her.");}
/* Raw launch initialises live space; player-facing routes use launch_departure. */
void launch(Game *g){if(!g->docked)return;g->flare_charges=flare_capacity(g);g->flare_reload=g->flare_cd=g->ecm_cd=g->flare_fx=0;guild_event(g,GUILD_LAUNCH);g->docked=0;g->pos=(Vec3){0,0,0};g->yaw=g->pitch=g->roll=0;g->dock_phase=0;g->dock_timer=g->dock_duration=0;g->speed=100;game_spawn(g);int before=g->story;story_event(g,STORY_EV_LAUNCH);if(g->story==before)message(g,"Station ahead. Select opens the deck.");if(g->story==STORY_SIGHT||g->story==STORY_RETURN)speak(g,VOICE_VENN,"Tower. Cleared. Soft launch — come home in one piece.");if(!g->cue)g->cue=SFX_DOCK;campaign_event(g,CP_LAUNCH);}
#include "docking.h"
#include "mega-city-tests.h"
#include "pulp-station-tests.h"
#include "journey.h"
int trade(Game *g,int i,int buy){if(!g->docked||i<0||i>=GOODS)return 0;
 if(buy){if(g->credits<g->price[i]||g->stock[i]<1||(goods[i].unit=='t'&&cargo_used(g)>=cargo_capacity(g))){message(g,"Check units, stock and cargo space.");return 0;}g->credits-=g->price[i];g->stock[i]--;g->cargo[i]++;if(goods_restricted(i))message(g,"Restricted goods loaded. Law will scan your hold.");}
 else {if(g->cargo[i]<=mission_cargo_reserved(g,i)){message(g,g->cargo[i]?"Reserved for a mission. Abandon it to release cargo.":"Nothing to sell.");return 0;}g->cargo[i]--;g->stock[i]++;g->credits+=g->price[i];}g->cue=SFX_UI;
 if(!(buy&&goods_restricted(i))){char note[64];snprintf(note,sizeof(note),buy?"Bought %s.":"Sold %s.",goods[i].name);message(g,note);}
 return 1;
}
int trader_offer_hail(Game *g,int npc_id){
 if(!g||npc_id<0||npc_id>=NPC_COUNT||!g->npc[npc_id].alive||g->npc[npc_id].role!=TRADERS||g->npc[npc_id].freighter)return 0;
 if(g->trader_offer_active&&g->trader_offer_system==g->system&&g->trader_offer_npc==npc_id){
  int need=g->trader_offer_need,reward=g->trader_offer_reward,qty=g->trader_offer_qty;
  if(need<0||need>=GOODS||reward<0||reward>=GOODS||qty<1||qty>3){g->trader_offer_active=0;return 0;}
  if(g->cargo[need]<qty){char line[160];snprintf(line,sizeof(line),"Bring %d%c %s and hail me again.",qty,goods[need].unit,goods[need].name);speak(g,VOICE_CONTACT,line);return 1;}
  if(goods[reward].unit=='t'&&cargo_used(g)-((goods[need].unit=='t')?qty:0)+qty>cargo_capacity(g)){speak(g,VOICE_CONTACT,"Your hold is too full for the exchange.");return 1;}
  g->cargo[need]-=qty;g->cargo[reward]+=qty;g->trader_offer_active=0;g->cue=SFX_TALK;
  {char line[160];snprintf(line,sizeof(line),"Deal done. %d%c %s for %d%c %s.",qty,goods[reward].unit,goods[reward].name,qty,goods[need].unit,goods[need].name);speak(g,VOICE_CONTACT,line);message(g,"Trader exchange complete.");}
  return 1;
 }
 if(g->trader_offer_active){speak(g,VOICE_CONTACT,"I have already made an offer. Find that trader again when you have the cargo.");return 1;}
 {static const int pool[]={0,1,2,4,5,7,8,9,10,11,12,16};int seed=(g->system*13+npc_id*7+g->systems[g->system].economy)&255;int need=pool[seed%(int)(sizeof(pool)/sizeof(pool[0]))];int reward=pool[(seed+3)%(int)(sizeof(pool)/sizeof(pool[0]))];
  g->trader_offer_active=1;g->trader_offer_system=g->system;g->trader_offer_npc=npc_id;g->trader_offer_need=need;g->trader_offer_reward=reward;g->trader_offer_qty=1+(seed&1);
  {char line[160];snprintf(line,sizeof(line),"I can swap %d%c %s for %d%c %s. Buy the %s, then find me again.",g->trader_offer_qty,goods[reward].unit,goods[reward].name,g->trader_offer_qty,goods[need].unit,goods[need].name,goods[need].name);speak(g,VOICE_CONTACT,line);message(g,"Trader offer recorded. Scan ships to find this trader again.");}
 }
 return 1;
}
int buy_ship(Game *g,int i){if(!g->docked||i<0||i>=player_ship_count)return 0;if(i==g->ship){message(g,"Already on this ship.");return 0;}
 for(int cat=0;cat<6;cat++){int n=0;for(int b=0;b<4;b++)n+=g->fit[cat+b*6]!=FIT_EMPTY;if(n>fit_capacity(i,cat)){message(g,"New hull has fewer module slots. Sell excess modules first.");return 0;}}
 int cost=player_ships[i].price-player_ships[g->ship].price*3/4;
 if(g->credits<cost||cargo_used(g)>player_ships[i].capacity+fit_hold_bonus(g)){message(g,"Not enough units, or cargo will not fit.");return 0;}
 int active=fit_weapon_item(g);
 for(int cat=0;cat<6;cat++){int n=0;uint8_t packed[4];for(int b=0;b<4;b++)if(g->fit[cat+b*6]!=FIT_EMPTY)packed[n++]=g->fit[cat+b*6];for(int b=0;b<4;b++)g->fit[cat+b*6]=b<n?packed[b]:FIT_EMPTY;}
 g->credits-=cost;g->ship=i;int active_slot=fit_find(g,active);g->active_weapon=active_slot>=0?active_slot:0;fit_rebuild(g);social_emit(g,SB_SHIP);if(g->fuel>player_ships[i].range)g->fuel=(float)player_ships[i].range;g->cue=SFX_UI;message(g,"Ship exchanged. Cargo transferred.");return 1;
}
int jump_start(Game *g){if(g->dock_stage==4){g->dock_phase=1;return 1;}if(g->planet>=0){message(g,"Return to orbit before engaging the hyperdrive.");return 0;}if(g->docked){message(g,"Launch before engaging the hyperdrive.");return 0;}
 if(g->jump>0){message(g,"Hyperspace already engaged.");return 0;}
 if(g->destination==g->system){message(g,"Already in that system.");return 0;}
 float cost=distance_ly(g,g->system,g->destination)*10;
 if(cost>g->fuel+0.01f&&!(g->debug_flags&DEBUG_UNLIMITED_RANGE)){message(g,"Not enough fuel. Refuel at a hub or scoop a sun.");return 0;}
 g->jump=8;g->speed=0;g->boost=0;g->cue=SFX_WARP;message(g,"Hyperdrive charging. Hold steady — jump spends fuel.");return 1;
}
int contract_accept(Game *g){if(!g->docked||g->contract>=0)return 0;
 if(g->destination==g->system||distance_ly(g,g->system,g->destination)>10.0f){message(g,"Select a destination within 10 LY.");return 0;}
 if(g->credits<100){message(g,"Contract deposit: 10 units.");return 0;}
 g->credits-=100;g->contract=g->destination;g->contract_time=300;g->contract_reward=1000+(int)(distance_ly(g,g->system,g->destination)*200);message(g,"Courier contract accepted. Track it from the mission log.");return 1;
}
static float npc_max_health(const Game *g,const NPC *n){if(n->bounty_slot>=0)return 110.f+danger_rating(g,g->system)*35.f;return n->freighter?900:n->role==LAW?110:80;}
static float npc_max_shield(const Game *g,const NPC *n){if(n->bounty_slot>=0)return 55.f+danger_rating(g,g->system)*15.f;return n->freighter?100:n->role==LAW?60:40;}
static void hit(Game *g,int i,float damage,int player);
static void ram_contact(Game *g,int i){NPC *n=&g->npc[i];int rescue=0;for(int s=0;s<g->job_n;s++)if(g->jobs[s].dest==g->system&&g->jobs[s].target==i&&g->jobs[s].type==MISSION_RESCUE)rescue=1;if(rescue){n->health=1;return;}hit(g,i,0,1);}
static void hit(Game *g,int i,float damage,int player){NPC *n=&g->npc[i];if(!n->alive)return;
 if(player&&damage>0)n->player_tag=90;
 /* Police assistance cannot steal a recently engaged poster target. */
 if(!player&&n->bounty_slot>=0&&n->player_tag>0)player=1;
 if(n->shield>0){float absorbed=fminf(n->shield,damage);n->shield-=absorbed;damage-=absorbed;}n->health-=damage;n->flash=.12f;if(player)g->cue=SFX_HIT;
 if(!player&&n->bounty_slot>=0&&n->health<35){n->health=35;n->escape_time=20;n->target=-1;}
 if(player&&n->role!=PIRATES){record_crime(g,n->freighter?8:5,n->role==LAW?CRIME_LAW_ASSAULT:CRIME_ASSAULT);message(g,n->freighter?"Freighter returns fire. Heavy police alert.":"Assault reported. Police alert.");}
 if(player&&(n->freighter||n->role==TRADERS||n->role==EXPLORERS||n->role==LAW)){n->target=-2;if(n->freighter)n->cooldown=0;}
 jobs_from_legacy(g);int protect=0;for(int s=0;s<g->job_n;s++){Job *j=&g->jobs[s];if(j->dest!=g->system||j->target!=i)continue;if(j->type==MISSION_RESCUE)protect=1;if(!player&&(j->type==MISSION_BOUNTY||j->type==MISSION_RESCUE))protect=1;}
 if(n->health<=0&&protect){n->health=1;if(player)message(g,"That's a rescue beacon. Don't fire.");return;}
 if(n->health<=0){n->alive=0;if(n->bounty_slot>=0)g->bounty_claimed[g->system]|=(uint8_t)(1u<<n->bounty_slot);if(n->freighter){n->freight_state=FREIGHT_ABSENT;n->freight_timer=240;}wreck_from_npc(g,i);if(player){g->kills++;social_emit(g,n->bounty_slot>=0||n->role==PIRATES?SB_BOUNTY:SB_DESTROY);if(n->role!=PIRATES)record_crime(g,n->freighter?25:10,n->role==LAW?CRIME_LAW_DESTRUCTION:CRIME_DESTRUCTION);if(n->bounty_slot>=0){int reward=bounty_target_reward(g,n->bounty_slot);g->credits+=reward;message(g,"WANTED TARGET TAKEN DOWN. Local Law bounty paid.");}else if(n->role==PIRATES){g->credits+=150;message(g,"Pirate destroyed. Bounty 15 units. Cargo released.");}else message(g,n->freighter?"Freighter destroyed. Heavy warrant filed. Cargo released.":"Contact destroyed. Cargo canisters released.");for(int s=0;s<g->job_n;){if(g->jobs[s].dest==g->system&&g->jobs[s].type==MISSION_BOUNTY&&g->jobs[s].target==i){g->job_sel=s;mission_finish_slot(g,s,"Pirate-hunt complete. Payment received.");}else s++;}}else g->npc_kills++;}
}
static void primary_weapon_hit(Game *g,int target,float distance){
 if(target<0||target>=NPC_COUNT||!g->npc[target].alive)return;
   int w=fit_weapon_item(g);NPC *n=&g->npc[target];float damage=laser_shot_damage(g)*weapon_output_multiplier(g);
   if(w==28)damage*=1.f-.5f*fminf(1.f,distance/weapon_range(g));
   if(w==29)n->shield=fmaxf(0,n->shield-25*weapon_output_multiplier(g));
   if(w==30){n->health-=damage*.25f;damage*=.75f;}
   if(w==59)n->shield=fmaxf(0,n->shield-18*weapon_output_multiplier(g));
   if(w==62){n->health-=damage*.18f;damage*=.82f;}
   if(w==66){n->health-=damage*.35f;damage*=.65f;}
   hit(g,target,damage,1);
   if((w==31||w==60||w==68)&&n->alive)n->cooldown=fmaxf(n->cooldown,w==68?2.f:1.2f);
}
static int salvage_collect(Game *g,int id){
 if(g->docked||g->dead||g->jump>0||g->planet>=0||!IS_DEBRIS_ID(id))return 0;
 int i=id-DEBRIS_ID_MIN;Debris *d=&g->debris[i];if(!d->alive)return 0;
 if(d->rock){message(g,"Blast the asteroid; then collect its ore.");return 0;}
 if(length(sub(d->pos,g->pos))>500){message(g,"Close within 500 m to salvage.");return 0;}
 char note[96];
 if(d->wreck){int cash=d->qty*(fit_find(g,42)>=0?120:100)/100;g->credits+=cash;snprintf(note,sizeof(note),"HULL SALVAGE RECOVERED: %.1f UNITS.",cash*.1f);}
  else {int item=d->good;if(item<0||item>=GOODS)item=12;
  if(item==12&&(g->upgrades&131072))item=9; /* refinery: minerals → alloys */
  if(goods[item].unit=='t'&&cargo_used(g)+d->qty>cargo_capacity(g)){int payout=g->price[item]*d->qty/2;if(payout<10)payout=10;if(fit_find(g,42)>=0)payout=payout*120/100;g->credits+=payout;snprintf(note,sizeof(note),"HOLD FULL. SOLD %s SALVAGE FOR %.1f UNITS.",goods[item].name,payout*.1f);}
  else {g->cargo[item]+=d->qty;if(goods_restricted(item))snprintf(note,sizeof(note),"SALVAGED RESTRICTED %s. HIDE IT FROM LAW SCANS.",goods[item].name);else snprintf(note,sizeof(note),"SALVAGED %d%c %s.",d->qty,goods[item].unit,goods[item].name);}
 }
 d->alive=0;social_emit(g,SB_SALVAGE);message(g,note);g->cue=SFX_SCAN;return 1;
}
int salvage(Game *g,int id){
 if(g->docked||g->dead||g->jump>0||g->planet>=0||!IS_DEBRIS_ID(id))return 0;
 int i=id-DEBRIS_ID_MIN;Debris *d=&g->debris[i];if(!d->alive)return 0;
 if(d->rock){message(g,"Blast the asteroid; then collect its ore.");return 0;}
 if(length(sub(d->pos,g->pos))>500){message(g,"Close within 500 m to salvage.");return 0;}
 if(g->tractor_time>0){message(g,"Tractor beam already engaged.");return 0;}
 g->tractor_target=id;g->tractor_time=.75f;g->speed=0;g->boost=0;g->cue=SFX_SCAN;message(g,"TRACTOR BEAM ENGAGED. HOLDING POSITION.");return 1;
}
int survey_scan(Game *g){return survey_scan_target(g,-1);}
int analysis_scan(Game *g,int id){
 if(g->dead||g->docked||g->police_stop||g->dock_stage||g->jump>0||g->planet>=0||!IS_ANOMALY_ID(id))return 0;
 int i=id-ANOMALY_ID_MIN;Anomaly *a=&g->anomaly[i];if(!a->alive)return 0;
 if(length(sub(a->pos,g->pos))>1100){message(g,"Move within 1,100 m to analyse the anomaly.");return 0;}
 g->rift_report=i+1;g->voice_who=VOICE_COMP;snprintf(g->voice,sizeof(g->voice),"%s",rift_reports[rift_type(g->system,i)]);g->voice_time=12;g->cue=SFX_TALK;
 if(a->scanned||(g->rift_logged[g->system]&(1u<<i))){a->scanned=1;saga_story_scan(g,id);message(g,"Archive report reopened. Already logged.");return 0;}
 saga_story_scan(g,id);a->scanned=1;g->rift_logged[g->system]|=1u<<i;g->scanned_anomalies++;social_emit(g,SB_RIFT);g->discoveries++;g->credits+=280;g->cue=SFX_TALK;
 message(g,a->kind?"Meridian echo logged. Research units awarded.":"Stellar anomaly catalogued. Research units awarded.");story_event(g,STORY_EV_SCAN);guild_event(g,GUILD_SCAN);return 1;
}
int survey_scan_target(Game *g,int requested){
 if(g->planet<0||g->surface!=2){message(g,"Exit the ship to survey surface life.");return 0;}
 float range=module_survey_range(g);int best=-1;for(int i=0;i<LIFE_COUNT;i++)if((requested<0||requested==i)&&g->life[i].alive&&!g->life[i].scanned){float d=length(sub(g->life[i].pos,g->pos));if(d<range){range=d;best=i;}}
 if(best<0){message(g,"No unscanned life or minerals in visor range.");return 0;}
 Lifeform *l=&g->life[best];l->scanned=1;g->surface_progress[g->system][g->planet]|=1u<<(8+best);g->discoveries++;g->credits+=120;g->cue=SFX_SCAN;
 if(l->kind==LIFE_FLORA){g->scanned_flora++;social_emit(g,SB_FLORA);message(g,"Flora discovered. Sample uploaded to Codex.");}
 else if(l->kind==LIFE_FAUNA){g->scanned_fauna++;social_emit(g,SB_FAUNA);message(g,"Fauna discovered. Behaviour logged.");}
 else {g->scanned_minerals++;if(cargo_used(g)>=cargo_capacity(g)){g->credits+=40;message(g,"Mineral logged. Hold full; ore sold for 4.0 units.");}else {if((g->upgrades&131072)){g->cargo[9]++;message(g,"Mineral refined to alloys onboard.");}else {g->cargo[12]++;message(g,"Mineral deposit scanned. 1t minerals collected.");}}}
 {char name[40],note[96];field_species_name(g->system,g->planet,best,name,sizeof(name));snprintf(note,sizeof(note),"%s logged. Planet Codex updated. +12 units.",name);message(g,note);}
 story_event(g,STORY_EV_SCAN);guild_event(g,GUILD_SCAN);return 1;
}
int mine_rock(Game *g,int id){
 if(g->planet>=0||g->docked||g->dead||g->jump>0||!IS_DEBRIS_ID(id))return 0;
 Debris *d=&g->debris[id-DEBRIS_ID_MIN];if(!d->alive||!d->rock)return 0;
 if(length(sub(d->pos,g->pos))-d->radius>weapon_range(g))return 0;
 d->health-=mine_shot_damage(g)*weapon_output_multiplier(g);d->flash=.22f;g->cue=SFX_MINE;
 if(d->health<=0){
  /* Reuse the fractured rock's slot: ore cannot be lost to a full debris pool. */
  d->rock=0;d->wreck=0;d->good=12;d->radius=24;d->life=240;d->flash=.65f;
  message(g,"Ore released. Close within 500 m and press Triangle.");
 }
 return 1;
}
int fire_missile(Game *g,int id){if(g->docked||g->dead)return 0;if(g->jump>0){message(g,"Missiles are locked during hyperspace.");return 0;}if(g->planet>=0){message(g,"Missiles cannot launch in atmosphere.");return 0;}if(g->missile_time>0)return 0;if(g->missiles<=0){message(g,"No missiles remaining.");return 0;}if(!IS_NPC_ID(id)){message(g,"Missile needs a ship target.");return 0;}int n=id-BODY_COUNT-1;if(!g->npc[n].alive){message(g,"Missile target is no longer present.");return 0;}if(length(sub(g->npc[n].pos,g->pos))>12000){message(g,"Target outside 12,000 m missile range.");return 0;}g->missiles--;g->missile_target=n;g->missile_dir=forward(g);Vec3 right={cosf(g->yaw),0,-sinf(g->yaw)};g->missile_pos=add(add(g->pos,mul(g->missile_dir,45)),mul(right,14));g->missile_pos.y-=6;g->missile_speed=fmaxf(2600,g->speed+1200);g->missile_trail_n=1;g->missile_trail[0]=g->missile_pos;g->missile_time=8;g->cue=SFX_MISSILE;message(g,"Missile launched.");return 1;}
static float segment_distance(Vec3 a,Vec3 b,Vec3 p){Vec3 d=sub(b,a);float l=dot(d,d);float t=l>0?fmaxf(0,fminf(1,dot(sub(p,a),d)/l)):0;return length(sub(p,add(a,mul(d,t))));}
static void note_collision(Game *g,const char *what){
 snprintf(g->collide,sizeof(g->collide),"%s",what);
 char note[96];snprintf(note,sizeof(note),"Collision: %s. Shields damaged.",what);
 message(g,note);g->cue=SFX_HIT;
}
#include "npc-steering.h"
#include "flight-tools.h"
static void world_collision(Game *g,Vec3 previous){
 if(station_collision(g,previous))return;
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&g->bodies[i];float radius=b->radius+60;
  if(segment_distance(previous,g->pos,b->pos)>=radius)continue;
  if(b->type!=SUN){
   Vec3 away=norm(sub(previous,b->pos));if(length(away)<.01f)away=(Vec3){0,0,-1};
   g->pos=add(b->pos,mul(away,b->radius+900));g->boost=0;g->approach=i;
   message(g,b->type==GAS?"Gas giant: X for the floating skyport; Circle to turn back.":"Planet ahead. X for surface flight; Circle to turn back.");
   g->cue=SFX_UI;return;
  }else {g->pos=add(b->pos,mul(norm(sub(previous,b->pos)),radius+10));g->speed=0;g->boost=0;g->collision=2;collision_damage(g,10,"Planet collision. Shields damaged.");note_collision(g,b->name);}
 }
}
static int escape_pod_recover(Game *g){
 if(!(g->upgrades&16384)||g->docked)return 0;
 for(int s=0;s<FIT_SLOTS;s++)if(g->fit[s]==18)g->fit[s]=FIT_EMPTY;
 fit_rebuild(g);g->dead=0;g->jump=0;g->boost=0;g->explosion=0;g->attacked=0;g->incoming_missile=0;
 g->hull=fmaxf(25,g->hull);g->damaged=g->hull<100;g->planet=-1;g->surface=0;g->approach=-1;g->planet_sequence=0;g->station_variant=0;
 docking_complete(g);refuel_full(g);message(g,"Escape pod spent. Emergency tow to hub; check hull repairs.");
 speak(g,VOICE_COMP,"Recovery complete. Escape pod consumed.");return 1;
}
#include "station-departure.h"
static void game_step(Game *g,float dt,float turn,float pitch,int throttle,int fire,float strafe){
 if(g->dead&&escape_pod_recover(g))return;
 if(dt<=0||dt>.1f)dt=1.0f/60;
 if(g->hull<=0&&!g->damaged)g->hull=100;
 /* An approach choice pauses threats and timers, like the input modal. */
 if(g->approach>=0&&!g->dead)return;
 mission_timers(g,dt);
 if(fire||g->heat>=80)saga_observation_interrupt(g);
 if(g->police_grace>0)g->police_grace=fmaxf(0,g->police_grace-dt);
 if(g->police_warning>0)g->police_warning=fmaxf(0,g->police_warning-dt);
 police_jail_tick(g,dt);
 if(g->police_stop)return;
 g->wanted[g->system]=g->legal;if(g->dead)g->explosion+=dt;
 for(int i=0;i<NPC_COUNT;i++)g->fire_bearing_time[i]=g->npc[i].alive?fmaxf(0,g->fire_bearing_time[i]-dt):0;
 social_tick(g,dt);g->time+=dt;g->world_clock=fmodf(g->world_clock+dt,86400.f);g->message_time-=dt;if(g->message_time<0)g->message_time=0;g->attacked=fmaxf(0,g->attacked-dt);g->collision=fmaxf(0,g->collision-dt);g->heat_sink_cd=fmaxf(0,g->heat_sink_cd-dt);g->damage_fx=fmaxf(0,g->damage_fx-dt);
 g->departure_glow=fmaxf(0,g->departure_glow-dt);flight_tools_tick(g,dt);
 if(g->incoming_missile>0){
  if(g->boost&&g->speed>player_ships[g->ship].speed*4){g->incoming_missile=0;g->incoming_source=-1;message(g,"Incoming missile evaded.");}
  else {g->incoming_missile-=dt;if(g->incoming_missile<=0){if(g->incoming_source>=0&&g->incoming_source<NPC_COUNT)g->fire_bearing_time[g->incoming_source]=2.5f;player_damage(g,25*(fit_find(g,37)>=0?.65f:1.f),"Missile impact. Shields damaged.");g->attacked=3;g->incoming_source=-1;}}
 }
 if(g->dock_stage==4){departure_tick(g,dt);return;}
 if(g->dock_stage){docking_tick(g,dt);return;}
 if(g->tractor_time>0){g->tractor_time-=dt;g->speed=0;g->boost=0;if(g->tractor_time<=0){int id=g->tractor_target;g->tractor_target=-1;g->tractor_time=0;salvage_collect(g,id);}return;}
 if(g->dead||g->docked||g->approach>=0)return;
 if(g->planet>=0){planet_tick(g,dt,turn,pitch,throttle,strafe);return;}
 flight_steer(g,turn,pitch,dt*1.5f);
 float damage_factor=g->damaged?fmaxf(.45f,g->hull/100.f):1.f;
 float cruise_speed=player_ships[g->ship].speed*(0.70f+0.15f*g->pip_eng)*damage_factor;
 g->speed+=throttle*dt*(g->boost?4500:180)*damage_factor;if(g->speed<0)g->speed=0;
 if(g->boost){float boost_speed=cruise_speed*20.f;if(g->speed>boost_speed)g->speed=boost_speed;}
 else if(g->speed>cruise_speed){
  /* Leaving boost should read as the drive winding down, not a one-frame
   * speed clamp. Ease only the excess velocity, settling in about a second. */
  float excess=g->speed-cruise_speed;
  g->speed-=excess*fminf(1.f,dt*4.5f);
  if(g->speed<cruise_speed+1.f)g->speed=cruise_speed;
 }
 if(g->boost&&g->planet<0&&g->jump<=0&&!(g->debug_flags&DEBUG_UNLIMITED_FUEL)){g->fuel=fmaxf(0,g->fuel-dt*.35f*(fit_find(g,48)>=0?.8f:1.f));if(g->fuel<=0){g->fuel=0;g->boost=0;if(g->message_time<=0)message(g,"Fuel empty. Boost cut.");}}
 Vec3 previous_pos=g->pos;g->pos=add(g->pos,mul(forward(g),g->speed*dt));world_collision(g,previous_pos);if(g->dead||g->dock_stage||g->approach>=0)return;campaign_flight(g,previous_pos);
 if(g->missile_time>0){g->missile_time-=dt;int i=g->missile_target;if(i<0||i>=NPC_COUNT||!g->npc[i].alive)g->missile_time=0;else {Vec3 previous=g->missile_pos;float age=8-g->missile_time;
 if(age>.18f)g->missile_dir=npc_turn_toward(g->missile_dir,norm(sub(g->npc[i].pos,g->missile_pos)),dt*3);
 float speed=g->missile_speed*fminf(1,.45f+age*2);
 g->missile_pos=add(g->missile_pos,mul(g->missile_dir,dt*speed));
 if(g->missile_trail_n<12)g->missile_trail_n++;
 for(int t=g->missile_trail_n-1;t>0;t--)g->missile_trail[t]=g->missile_trail[t-1];
 g->missile_trail[0]=g->missile_pos;
 if(segment_distance(previous,g->missile_pos,g->npc[i].pos)<g->npc[i].radius+80){int mission_hit=0;for(int s=0;s<g->job_n;s++)if(g->jobs[s].dest==g->system&&g->jobs[s].type==MISSION_BOUNTY&&g->jobs[s].target==i)mission_hit=1;hit(g,i,140,1);g->missile_time=0;if(!mission_hit)message(g,"Missile hit confirmed.");}}}
 /* Heat builds from speed, boost and sun; cools when not boosting and clear of the star. */
 {
  float sun_d=length(sub(g->pos,g->bodies[0].pos)),safe=g->bodies[0].radius+5200.f;
  int near_sun=sun_d<safe;float speed_ratio=g->speed/fmaxf(1.f,player_ships[g->ship].speed);float heat_mult=(g->upgrades&262144)?.30f:1.f;
  /* Residual velocity after boost is passive coast-down, not continued
   * engine output, so it must not create an artificial heat spike. */
  if(speed_ratio>1.15f)g->heat=fminf(100,g->heat+dt*(speed_ratio-1.f)*(g->boost?.35f:throttle>0?10.f:.08f)*heat_mult);
  /* Engine pips stretch boost endurance: more ENG means less heat per second
   * and faster recovery while the boost is held. */
  if(g->boost){float boost_heat=fmaxf(6.f,10.f-g->pip_eng*.75f);g->heat=fminf(100,g->heat+dt*boost_heat*heat_mult);}
  if(near_sun){float exposure=fmaxf(0,fminf(1,(safe-sun_d)/5200.f));
   g->heat=fminf(100,g->heat+dt*(6.f+80.f*exposure*exposure)*(fit_find(g,49)>=0?.75f:1.f));
   if(g->heat>=85)thermal_damage(g,dt*(3.f+30.f*exposure*exposure),"Solar radiation is melting the ship. Pull away!");
  }
  /* Once boost is released the heat sinks must work even while the ship is
   * still travelling fast. High speed cools a little more slowly, but it
   * must never leave a full heat bar permanently latched at 100. */
  if(!g->boost&&!near_sun&&(speed_ratio<=1.15f||g->heat>=90.f)){float cool=module_cooling(g);if(speed_ratio>1.15f)cool*=.55f;g->heat=fmaxf(0,g->heat-dt*cool);}
  else if(g->boost&&!near_sun)g->heat=fmaxf(0,g->heat-dt*(4.f+g->pip_eng*1.5f));
  /* Critical boost is still available, but the overheated drive now draws
   * directly on shields before the hard runaway threshold is reached. */
  if(g->boost&&g->heat>=90)thermal_damage(g,dt*(3.f+(g->heat-90.f)*.5f),"Boost heat is draining the shields.");
 if(g->heat>=100){
  g->heat=100;
  if(escape_pod_recover(g))return;
  else {thermal_damage(g,dt*8,"Thermal runaway. Shields are failing.");}
 }
  else if(g->heat>=90){/* Heat and shield bars carry this warning without a caption. */}
 }
 g->shot=fmaxf(0,g->shot-dt);g->energy=fminf(100,g->energy+dt*shield_regen_rate(g)*(0.50f+0.25f*g->pip_sys));
 if(g->damaged&&g->energy>25)g->energy=25;
 if(module_repair_rate(g)>0&&g->attacked<=0&&g->heat<80&&!g->dead){
  g->hull=fminf(100,g->hull+dt*module_repair_rate(g));if(g->hull>=100){g->damaged=0;g->damage_fx=0;}
 }
 if(g->passenger_dest>=0&&!g->docked&&g->message_time<=0&&((int)(g->time*2)&63)==0){static const char *chatter[]={"Passenger: Ever notice how Lave still smells like GalCop paint?","Passenger: Meridian sold us a 'clear lane' once. Cost a tender.","Passenger: If the animals stop singing, burn a different chart.","Passenger: Guild folks tip. Corporate folks invoice."};speak(g,VOICE_CONTACT,chatter[((int)g->time+(unsigned)g->passenger_dest)%4]);message(g,chatter[((int)g->time+(unsigned)g->passenger_dest)%4]);}
 {float scoop=fuel_scoop_rate(g);if(scoop>0&&length(sub(g->pos,g->bodies[0].pos))<g->bodies[0].radius+4000){g->fuel=fminf((float)player_ships[g->ship].range,g->fuel+dt*scoop);if(g->message_time<=0)message(g,g->heat>80?"Fuel scoop overheating. Break off.":"Fuel scoop filling the tank.");}}
 if(fire&&!g->laser){if(g->message_time<=0)message(g,"No weapon fitted. Visit Outfitting to install one.");fire=0;}
 if(fire&&g->heat>=80&&g->shot<=.001f){/* The heat bar is the persistent warning. */}
 if(fire&&g->shot<=.001f&&g->heat<80){g->shot=weapon_cycle(g);g->heat+=weapon_heat(g);g->shots++;g->cue=SFX_LASER;float shot_range=weapon_range(g),closest=shot_range;int target=-1;
  Vec3 beam_end=add(g->pos,mul(forward(g),shot_range));
  for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive){
   NPC *n=&g->npc[i];Vec3 p=camera(g,n->pos);float distance=p.z;
   int onbeam=p.z>20&&sqrtf(p.x*p.x+p.y*p.y)<fmaxf(n->radius*.65f,p.z*weapon_cone(g));
   if(n->freighter){float t=freight_intersection(n,g->pos,beam_end,4);onbeam=t>=0;distance=t*shot_range;}
   if(onbeam&&distance<closest){target=i;closest=distance;}
  }
  int rock=-1;
  for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&g->debris[i];if(!d->alive||!d->rock)continue;
   Vec3 p=camera(g,d->pos);float lateral=p.x*p.x+p.y*p.y,r=d->radius;
   if(p.z<=0||lateral>r*r)continue;
   float near=fmaxf(0,p.z-sqrtf(r*r-lateral));if(near<closest){rock=i;closest=near;}
  }
  if(rock>=0)mine_rock(g,DEBRIS_ID_MIN+rock);
  else if(target>=0){
   primary_weapon_hit(g,target,closest);
  }
 }
 freight_update(g,dt);
 /* Capital hulls use the same oriented dimensions for rendering and collision.
  * Test every frame, including boosted movement across an entire hull. */
 for(int i=8;i<36;i+=12){NPC *n=&g->npc[i];if(!n->alive||!n->freighter)continue;
  float t=freight_intersection(n,previous_pos,g->pos,20);
  if(t>=0){Vec3 movement=sub(g->pos,previous_pos);
   if(t>0)g->pos=add(previous_pos,mul(movement,fmaxf(0,t-.01f)));
   else {Vec3 p=freight_local(n,g->pos),e=freight_extent(n);p.x=(p.x<0?-1:1)*(e.x+30);g->pos=freight_world(n,p);}
   g->speed=0;g->boost=0;if(g->collision<=0){collision_damage(g,5,"Freighter collision. Shields damaged.");n->health-=10;g->collision=2;note_collision(g,"FREIGHTER HULL");if(n->health<=0)ram_contact(g,i);}
  }
 }
 int ai_phase=g->ai_phase++;float frame_dt=dt;
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&g->npc[i];if(n->freighter)continue;if(ai_phase>=0&&((i+ai_phase)&1))continue;float dt=ai_phase<0?frame_dt:frame_dt*2;
  if(!n->alive){if(n->bounty_slot>=0)continue;if(i>=36){if(system_is_lawful(g,g->system)&&i<36+wanted_level(g)*2){n->cooldown-=dt;if(n->cooldown<=0){n->alive=1;n->health=85;n->shield=60;traffic_assign(g,n,i,0);n->cooldown=2;}}continue;}n->cooldown-=dt;if(i<traffic_budget(g)&&n->cooldown<-18){n->health=npc_max_health(g,n);n->shield=npc_max_shield(g,n);n->alive=1;n->cooldown=1;n->flash=0;place_civilian(g,n,i);traffic_assign(g,n,i,0);}continue;}n->cooldown-=dt;n->flash=fmaxf(0,n->flash-dt);if(n->flash<=0)n->shield=fminf(npc_max_shield(g,n),n->shield+dt*.8f);
  float best=n->role==LAW?1800:2400;int target=-1;Vec3 aim=traffic_aim(g,n,dt);
  if(n->role==EXPLORERS){
   int lead=-1,wing=0;for(int j=0;j<36;j++)if(g->npc[j].alive&&g->npc[j].role==EXPLORERS){if(lead<0)lead=j;if(j<i)wing++;}
   if(lead>=0&&lead!=i){NPC *L=&g->npc[lead];Vec3 right=norm((Vec3){L->dir.z,0,-L->dir.x});aim=add(L->pos,add(mul(right,220.f*wing),mul(L->dir,-160.f*wing)));n->waypoint=L->waypoint;}
   else {int planet=n->waypoint;if(planet<1||planet>=BODY_COUNT)planet=1;aim=traffic_world_point(g,planet);if(length(sub(n->pos,aim))<380)n->waypoint=planet%4+1;}
  }
  /* Patrols stay near their assigned lane; pirates do not seek fights with Law. */
  int on_patrol=traffic_in_lane(g,n);
  if((n->role==LAW&&on_patrol)||n->role==PIRATES)for(int j=0;j<NPC_COUNT;j++)if(i!=j&&g->npc[j].alive){NPC *o=&g->npc[j];if((n->role==LAW&&o->role==PIRATES)||(n->role==PIRATES&&o->role==TRADERS)){float d=length(sub(o->pos,n->pos));if(d<best){target=j;best=d;aim=o->pos;}}}
  float pd=length(sub(g->pos,n->pos));if(((n->role==PIRATES&&g->system!=7)||(n->role==LAW&&g->legal>0&&on_patrol)||((n->role==TRADERS||n->role==EXPLORERS)&&n->player_tag>0))&&pd<best){target=-2;best=pd;aim=g->pos;}
  if(n->role==PIRATES){for(int j=0;j<NPC_COUNT;j++)if(g->npc[j].alive&&g->npc[j].role==LAW&&length(sub(n->pos,g->npc[j].pos))<2200)n->escape_time=fmaxf(n->escape_time,12);
   if(n->escape_time>0){target=-1;aim=traffic_refuge(g,n);n->route_wait=0;n->route_task=TRAFFIC_ESCAPE;}}
  if(n->role==LAW&&pd<650&&g->jump<=0&&g->police_grace<=0){
   int dirty=cargo_contraband(g);
   if(g->legal>0){n->target=-2;if(!g->police_warned){g->police_warned=1;g->police_warning=3.0f;g->cue=SFX_ALERT;message(g,"LOCAL LAW WARNING: cease fire and power down. Next pass is arrest.");speak(g,VOICE_LAW,"Warning, Commander. Cease fire and power down, or we will arrest you.");return;}if(g->police_warning<=0){police_begin(g,0);return;}return;}
   if(dirty>0){n->target=-2;police_begin(g,1);return;}
  }
  if((n->role==TRADERS&&!n->freighter)||n->role==EXPLORERS){float danger=1100;int threat=-1;for(int j=0;j<NPC_COUNT;j++)if(g->npc[j].alive&&g->npc[j].role==PIRATES){float d=length(sub(n->pos,g->npc[j].pos));if(d<danger){danger=d;threat=j;}}if(threat>=0)aim=add(n->pos,mul(norm(sub(n->pos,g->npc[threat].pos)),2000));}
  n->target=target;Vec3 attack_direction=norm(sub(aim,n->pos)),desired=attack_direction;
  /* Fly an escape leg instead of braking into the enemy hull. */
  if(target!=-1&&best<450){Vec3 side={n->dir.z,.25f,-n->dir.x};desired=norm(add(mul(attack_direction,-1.6f),side));}
  for(int b=0;b<BODY_COUNT;b++){Vec3 toward=sub(g->bodies[b].pos,n->pos);float along=dot(toward,desired),radius=g->bodies[b].radius+n->radius+300;if(along>0&&along<radius+1800&&length(sub(toward,mul(desired,along)))<radius){Vec3 outward=norm(mul(toward,-1));desired=norm(add(desired,add(mul(outward,2),(Vec3){.15f,.4f,0})));}}for(int cap=8;cap<36;cap+=12){NPC *c=&g->npc[cap];if(!c->alive||!c->freighter)continue;Vec3 toward=sub(c->pos,n->pos);float along=dot(toward,desired),clear=c->radius+n->radius+220;if(along>0&&along<clear+1000&&length(sub(toward,mul(desired,along)))<clear)desired=norm(add(desired,add(mul(norm(mul(toward,-1)),2),(Vec3){.15f,.6f,0})));}
  float speed=traffic_speed(n);
  desired=npc_avoid_traffic(g,i,desired,speed);n->dir=npc_turn_toward(n->dir,desired,dt);
  if(target!=-1&&best<(n->freighter?2400:1300)&&dot(n->dir,attack_direction)>.8f&&n->cooldown<=0){n->cooldown=1.5f-danger_rating(g,g->system)*.18f+random_f(g)*.5f;n->flash=.1f;
    if(target==-2){player_damage(g,n->bounty_slot>=0?4+danger_rating(g,g->system):4,"Weapons fire is striking your shields.");g->attacked=3.5f;g->fire_bearing_time[i]=2.5f;if(n->role==PIRATES&&best>1200&&g->incoming_missile<=0&&random_f(g)<(n->bounty_slot>=0?.24f:.18f)){g->incoming_missile=3.5f;g->incoming_source=i;g->cue=SFX_ALERT;}}else hit(g,target,n->freighter?20:8,0);
  }
 }
 /* Decisions stay staggered, but visible movement/collisions advance every frame. */
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&g->npc[i];if(!n->alive||n->freighter)continue;
  Vec3 stn={0,0,3500};float speed=traffic_speed(n);
  Vec3 city_previous=n->pos;n->pos=add(n->pos,mul(n->dir,frame_dt*speed));mega_npc_clear(g,n,city_previous);
  if(length(sub(n->pos,stn))<190)n->pos=add(stn,mul(norm(sub(n->pos,stn)),200));
  if(segment_distance(previous_pos,g->pos,n->pos)<n->radius+15){g->pos=add(n->pos,mul(norm(sub(previous_pos,n->pos)),n->radius+20));g->speed=0;g->boost=0;collision_damage(g,5,"Ship collision. Shields damaged.");n->health-=10;g->collision=2;note_collision(g,meshes[n->mesh].name);if(n->health<=0)ram_contact(g,i);}
  for(int b=0;b<BODY_COUNT;b++)if(length(sub(n->pos,g->bodies[b].pos))<g->bodies[b].radius+n->radius+100)n->pos=add(g->bodies[b].pos,mul(norm(sub(n->pos,g->bodies[b].pos)),g->bodies[b].radius+n->radius+120));
 }
 npc_separate_traffic(g);
 /* The scanner occasionally calls out battles away from the player. This
  * turns the faction simulation into a readable world event without adding
  * another HUD panel or interrupting ordinary flight. Battle talk SFX rides
  * with the radio voice so combat chatter feels live on the channel. */
 g->encounter=fmaxf(0,g->encounter-dt);
 if(g->encounter<=0&&g->message_time<=0&&g->voice_time<=0){int pirate=-1,law=-1,trader=-1;float pd=999999,ld=999999,td=999999;
  for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive){NPC *n=&g->npc[i];float d=length(sub(n->pos,g->pos));if(n->role==PIRATES&&d<pd){pirate=i;pd=d;}else if(n->role==LAW&&d<ld){law=i;ld=d;}else if(n->role==TRADERS&&d<td){trader=i;td=d;}}
  if(pirate>=0&&law>=0&&pd<5200&&ld<5200&&length(sub(g->npc[pirate].pos,g->npc[law].pos))<2600){g->encounter=9;g->cue=SFX_TALK;speak(g,VOICE_LAW,"Law channel: engagement in progress. Stay clear.");}
  else if(pirate>=0&&trader>=0&&pd<5200&&td<5200&&length(sub(g->npc[pirate].pos,g->npc[trader].pos))<2200){g->encounter=9;g->cue=SFX_TALK;speak(g,VOICE_CONTACT,"Mayday - convoy under pirate attack. Need cover.");g->voice_role=TRADERS;}
 }
 /* Close-range battle talk when someone is painting the canopy. */
 if(g->attacked>0&&g->voice_time<=0&&g->message_time<=0&&g->encounter<=0&&((int)(g->time*3)&31)==0){
  static const char *taunt[]={"Pirate band: Drop cargo or burn.","Hostile: Shields won't save you.","Open channel: Break off or we finish this.","Law band: Cease fire and identify."};
  int who=VOICE_CONTACT,role=PIRATES;for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive&&g->npc[i].target==-2){role=g->npc[i].role;who=role==LAW?VOICE_LAW:VOICE_CONTACT;break;}
  g->encounter=5;g->cue=SFX_TALK;speak(g,who,taunt[((int)g->time+g->system)%4]);if(who==VOICE_CONTACT)g->voice_role=role;
 }
 /* Reusable ambient encounter deck: mostly harmless traffic and scanner
  * colour, with a smaller tail for danger, rescue and mystery. */
 if(g->encounter_kind==ENCOUNTER_NONE&&g->encounter<=0&&g->message_time<=0&&g->voice_time<=0&&g->time>12){
  float station_d=length(sub(g->pos,hub_position(g,0))),planet_d=length(sub(g->pos,g->bodies[1].pos));
  int interval=45-g->systems[g->system].government*3;if(station_d<9000)interval-=10;if(planet_d<9000)interval-=5;if(interval<15)interval=15;if(interval>50)interval=50;
  int now=(int)g->time;
  if(now%interval==0){
   int trader=-1,pirate=-1,law=-1;for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive){if(trader<0&&g->npc[i].role==TRADERS)trader=i;if(pirate<0&&g->npc[i].role==PIRATES)pirate=i;if(law<0&&g->npc[i].role==LAW)law=i;}
   int roll=(int)(random_u(g)%100),kind=ENCOUNTER_TRADER,n=-1;
   if(roll<34){kind=ENCOUNTER_TRADER;n=trader;}else if(roll<47&&system_is_lawful(g,g->system)&&law>=0){kind=ENCOUNTER_POLICE;n=law;}else if(roll<59)kind=ENCOUNTER_CARGO;else if(roll<69){kind=(trader>=0&&pirate>=0)?ENCOUNTER_DISTRESS:ENCOUNTER_WRECKAGE;n=trader;}else if(roll<78||!system_is_lawful(g,g->system)){kind=ENCOUNTER_PIRATE;n=pirate;}else if(roll<86)kind=ENCOUNTER_WRECKAGE;else if(roll<91){kind=ENCOUNTER_SMUGGLER;n=trader;}else if(roll<96)kind=ENCOUNTER_MYSTERY;else if(roll<99){kind=ENCOUNTER_BOUNTY;n=pirate;}else kind=ENCOUNTER_UNKNOWN;
   g->encounter_kind=kind;g->encounter_npc=n;g->encounter_payload=-1;g->encounter=8;
   if(kind==ENCOUNTER_CARGO||kind==ENCOUNTER_WRECKAGE){Vec3 p=add(g->pos,mul(forward(g),1400+(random_u(g)%1100)));int slots=kind==ENCOUNTER_WRECKAGE?1+(random_u(g)%4):1;for(int s=0;s<slots;s++){int id=spawn_debris(g,add(p,(Vec3){(float)(s*90),0,(float)(s*55)}),(Vec3){0,0,0},random_u(g)%13,1,0);if(s==0&&id>=0)g->encounter_payload=DEBRIS_ID_MIN+id;}}
   if(kind==ENCOUNTER_DISTRESS)speak(g,VOICE_CONTACT,"Mayday! Trader under fire. Any ship nearby, please respond.");else if(kind==ENCOUNTER_PIRATE)speak(g,VOICE_CONTACT,"Cut engines and dump two tonnes of cargo.");else if(kind==ENCOUNTER_SMUGGLER)speak(g,VOICE_CONTACT,"Interested in something the station does not advertise?");else if(kind==ENCOUNTER_BOUNTY)speak(g,VOICE_CONTACT,"Wanted pilot in your lane. Bounty confirmed on scanner.");
   if(kind==ENCOUNTER_POLICE)g->voice_role=LAW;else g->voice_role=TRADERS;
  }
 }
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&g->debris[i];if(!d->alive)continue;d->flash=fmaxf(0,d->flash-dt);d->life-=dt;if(d->life<=0){d->alive=0;continue;}d->pos=add(d->pos,mul(d->vel,dt));Vec3 stn={0,0,3500};if(length(sub(d->pos,stn))<200)d->pos=add(stn,mul(norm(sub(d->pos,stn)),210));}
 if(g->hull<=0){
  if(escape_pod_recover(g))return;
  else {g->dead=1;g->jump=0;g->cue=SFX_DEATH;message(g,"Ship destroyed. START for a new commander.");}
 }
 if(g->jump>0){g->jump-=dt;if(g->jump<=0){float spent=distance_ly(g,g->system,g->destination)*10;if(!(g->debug_flags&DEBUG_UNLIMITED_FUEL))g->fuel-=spent;if(g->fuel<0)g->fuel=0;g->wanted[g->system]=g->legal;int origin=g->system,arriving=g->destination,first_arrival=!(g->visited[arriving>>3]&(1u<<(arriving&7)));social_emit(g,SB_LEAVE);g->social.seen[g->system]=social_now(g);g->system=arriving;social_arrive(g);g->legal=g->wanted[g->system];g->police_cargo_heat=g->crime_record[g->system]>>8;
  system_arrival(g,origin);
  travellers_advance(g,g->system);
  market(g);game_spawn(g);saga_observation_reenter(g);route_refresh_destination(g);g->cue=SFX_WARP;char note[80];snprintf(note,sizeof(note),"Arrival: hold Square to find contacts. Fuel %.1f LY.",g->fuel*.1f);message(g,note);if(first_arrival){char brief[160];first_arrival_brief(g,brief,sizeof(brief));message(g,"FIRST ARRIVAL: System data added to the Discovery Codex.");speak(g,VOICE_COMP,brief);}else speak(g,VOICE_COMP,"Hyperspace complete. Use the targeting computer to find local contacts.");}}
}
void game_tick(Game *g,float dt,float turn,float pitch,int throttle,int fire){game_step(g,dt,turn,pitch,throttle,fire,0);}
void game_eva_tick(Game *g,float dt,float turn,float pitch,int walk,float strafe,int jet){
 if(g->planet<1||g->planet>=BODY_COUNT||g->surface!=2||g->dead||g->docked||g->police_stop||g->approach>=0||g->dock_stage||g->jump>0)return;
 g->boost=jet!=0;game_step(g,dt,turn,pitch,walk,0,fmaxf(-1,fminf(1,strafe)));
}
void game_rover_tick(Game *g,float dt,float steer,float look,unsigned controls){
 if(!g->rover_driving||!isfinite(steer)||!isfinite(look)||!isfinite(dt))return;
 g->rover_brake=(controls&ROAM_BRAKE)!=0;g->rover_drift=(controls&ROAM_DRIFT)!=0;g->rover_boost=(controls&ROAM_BOOST)!=0;
 g->rover_last_controls=controls;
 game_eva_tick(g,dt,steer,look,(controls&ROAM_ACCEL)!=0,0,0);
 g->rover_brake=g->rover_drift=g->rover_boost=0;
}
/* Versioned commander file. Load into a temporary struct; reject before mutation. */
typedef struct {uint32_t magic,version;int system,destination,credits,kills,legal,ship,laser,missiles;float fuel;int cargo[GOODS],stock[GOODS],price[GOODS];int contract,reward;float remaining;} Save;
#include "save-integrity.h"
#include "social-save.h"
#include "save-safety.h"
int save_game(Game *g,const char *path){if(!g->docked)return 0;
 Save s;memset(&s,0,sizeof(s));s.magic=0x41455053;s.version=13;s.system=g->system;s.destination=g->destination;s.credits=g->credits;s.kills=g->kills;s.legal=g->legal;s.ship=g->ship;s.laser=g->laser;s.missiles=g->missiles;s.fuel=g->fuel;s.contract=g->contract;s.reward=g->contract_reward;s.remaining=g->contract_time;
 s.version=26;
 memcpy(s.cargo,g->cargo,sizeof(s.cargo));memcpy(s.stock,g->stock,sizeof(s.stock));memcpy(s.price,g->price,sizeof(s.price));
 char tmp[256],bak[256];if(!save_side_path(tmp,sizeof(tmp),path,".tmp")||!save_side_path(bak,sizeof(bak),path,".bak")){message(g,"Save path is too long.");return 0;}FILE *f=fopen(tmp,"wb");if(!f){message(g,"Cannot create save file.");return 0;}int mission[8]={g->mission_type,g->mission_stage,g->mission_target,g->mission_item,g->mission_origin,g->mission_result,g->last_mission_type,g->last_mission_system};g->wanted[g->system]=g->legal;int extra[5]={g->discoveries,g->scanned_flora,g->scanned_fauna,g->scanned_minerals,g->scanned_anomalies};int jobhead[2]={g->job_n,g->job_sel};int ok=fwrite(&s,1,sizeof(s),f)==sizeof(s);if(ok)ok=fwrite(g->wanted,1,sizeof(g->wanted),f)==sizeof(g->wanted);if(ok)ok=fwrite(&g->upgrades,1,sizeof(g->upgrades),f)==sizeof(g->upgrades);if(ok)ok=fwrite(mission,1,sizeof(mission),f)==sizeof(mission);if(ok)ok=fwrite(extra,1,sizeof(extra),f)==sizeof(extra);if(ok)ok=fwrite(g->visited,1,sizeof(g->visited),f)==sizeof(g->visited);if(ok)ok=fwrite(jobhead,1,sizeof(jobhead),f)==sizeof(jobhead);if(ok)ok=fwrite(g->jobs,1,sizeof(g->jobs),f)==sizeof(g->jobs);int storypack[8]={g->story,g->story_flags,g->pip_sys,g->pip_eng,g->pip_wep,g->guild_chapter,g->guild_flags,g->guild_choice};if(ok)ok=fwrite(storypack,1,sizeof(storypack),f)==sizeof(storypack);uint32_t cp[5]={(uint32_t)g->campaign_stage,(uint32_t)g->campaign_choice,(uint32_t)g->campaign_flags,0,0};memcpy(&cp[3],&g->campaign_distance,4);memcpy(&cp[4],&g->campaign_fuel,4);for(int i=0;ok&&i<5;i++)ok=save_u32(f,cp[i]);uint32_t saga[10]={(uint32_t)g->saga_chapter,(uint32_t)g->saga_step,(uint32_t)g->saga_flags,(uint32_t)g->saga_choice,(uint32_t)g->saga_dest,(uint32_t)g->saga_start,(uint32_t)g->saga_trust[0],(uint32_t)g->saga_trust[1],(uint32_t)g->saga_trust[2],(uint32_t)g->saga_trust[3]};for(int i=0;ok&&i<10;i++)ok=save_u32(f,saga[i]);if(ok)ok=save_u32(f,(uint32_t)(int32_t)g->route_goal);if(ok)ok=save_u32(f,(uint32_t)(int32_t)g->passenger_dest);if(ok)ok=save_u32(f,(uint32_t)g->passenger_kind);if(ok)ok=save_u32(f,(uint32_t)g->passenger_pay);if(ok)ok=save_u32(f,(uint32_t)g->gift_flags);for(int ti=0;ok&&ti<12;ti++){uint32_t tp=(uint32_t)g->travellers[ti].sys|((uint32_t)g->travellers[ti].dest<<8)|((uint32_t)g->travellers[ti].flags<<16);ok=save_u32(f,tp);}if(ok){unsigned char fitpad[8];for(int s=0;s<6;s++)fitpad[s]=g->fit[s];fitpad[6]=fitpad[7]=0;ok=fwrite(fitpad,1,8,f)==8;}if(ok)ok=fwrite(g->landed_planets,1,sizeof(g->landed_planets),f)==sizeof(g->landed_planets);if(ok)ok=save_u32(f,(uint32_t)g->tutorial_step);if(ok)ok=save_u32(f,(uint32_t)g->tutorial_seen);if(ok)ok=fwrite(g->commander_name,1,25,f)==25;if(ok)ok=save_u32(f,g->commander_portrait);if(ok)ok=fwrite(g->bounty_claimed,1,256,f)==256;for(int sys=0;ok&&sys<256;sys++)for(int body=0;ok&&body<BODY_COUNT;body++)ok=save_u32(f,g->surface_progress[sys][body]);uint32_t clock_bits;memcpy(&clock_bits,&g->world_clock,4);if(ok)ok=save_u32(f,clock_bits);for(int sys=0;ok&&sys<256;sys++)for(int hub=0;ok&&hub<HUB_COUNT;hub++)ok=save_u32(f,g->station_progress[sys][hub]);for(int sys=0;ok&&sys<256;sys++){uint32_t record=g->wanted[sys]>0?g->crime_record[sys]:0;if(g->wanted[sys]>0&&!record)record=CRIME_UNKNOWN;ok=save_u32(f,record);}if(ok)ok=fwrite(g->rift_logged,1,256,f)==256;if(ok){g->social.seen[g->system]=social_now(g);ok=social_write(f,&g->social);}if(ok){unsigned char bank[FIT_SAVE_BYTES];memcpy(bank,g->fit+6,18);bank[18]=g->active_weapon;bank[19]=0;ok=fwrite(bank,1,sizeof(bank),f)==sizeof(bank);}if(fflush(f)!=0)ok=0;if(fclose(f)!=0)ok=0;if(ok)ok=save_seal(tmp);if(ok)ok=save_commit(path,tmp,bak,rename);if(ok)g->cue=SFX_SELECT;message(g,ok?"Commander saved.":"Save failed.");return ok;
}
static int load_game_file(Game *g,const char *path){Save s;memset(&s,0,sizeof(s));FILE *f=fopen(path,"rb");if(!f)return 0;int warrants[256]={0},upgrades=0,mission[8]={MISSION_DELIVERY,0,-1,1,7,0,MISSION_DELIVERY,7}; int extra[5]={0,0,0,0,0};uint8_t visited[32]={0};int jobhead[2]={0,0};Job packed[MISSION_SLOTS];memset(packed,0,sizeof(packed));int storypack[8]={STORY_FREE,0xffff,2,2,4,0,0,0};uint32_t cp[5]={0},saga[10]={0},route_goal_u=0xffffffffu;float cp_distance=0,cp_fuel=0;int route_goal=-1;int ok=fread(&s,1,sizeof(s),f)==sizeof(s);if(ok&&s.version>=2)ok=fread(warrants,1,sizeof(warrants),f)==sizeof(warrants);if(ok&&s.version>=3)ok=fread(&upgrades,1,sizeof(upgrades),f)==sizeof(upgrades);if(ok&&s.version>=4)ok=fread(mission,1,sizeof(mission),f)==sizeof(mission);if(ok&&s.version>=5){ok=fread(extra,1,sizeof(extra),f)==sizeof(extra);if(ok)ok=fread(visited,1,sizeof(visited),f)==sizeof(visited);}if(ok&&s.version>=6){ok=fread(jobhead,1,sizeof(jobhead),f)==sizeof(jobhead);if(ok)ok=fread(packed,1,sizeof(packed),f)==sizeof(packed);}if(ok&&s.version>=7)ok=fread(storypack,1,sizeof(storypack),f)==sizeof(storypack);if(ok&&s.version>=8){for(int i=0;ok&&i<5;i++)ok=load_u32(f,&cp[i]);memcpy(&cp_distance,&cp[3],4);memcpy(&cp_fuel,&cp[4],4);if(cp[0]>6||cp[1]>2||cp[2]>7||!isfinite(cp_distance)||cp_distance<0||cp_distance>600||!isfinite(cp_fuel)||cp_fuel<0||cp_fuel>255)ok=0;}if(ok&&s.version>=9){for(int i=0;ok&&i<10;i++)ok=load_u32(f,&saga[i]);if(saga[0]>SAGA_COUNT||saga[1]>1||saga[3]>3||saga[4]>255)ok=0;}if(ok&&s.version>=10){ok=load_u32(f,&route_goal_u);if(ok){if(route_goal_u==0xffffffffu)route_goal=-1;else if(route_goal_u<=255)route_goal=(int)route_goal_u;else ok=0;}}uint32_t pax_dest_u=0xffffffffu,pax_kind=0,pax_pay=0,gift=0,trav_pack[12]={0};int pax_dest=-1,have_trav=0;if(ok&&s.version>=11){ok=load_u32(f,&pax_dest_u);if(ok)ok=load_u32(f,&pax_kind);if(ok)ok=load_u32(f,&pax_pay);if(ok)ok=load_u32(f,&gift);if(ok){if(pax_dest_u==0xffffffffu)pax_dest=-1;else if(pax_dest_u<=255)pax_dest=(int)pax_dest_u;else ok=0;}}if(ok&&s.version>=12){for(int ti=0;ok&&ti<12;ti++)ok=load_u32(f,&trav_pack[ti]);if(ok)have_trav=1;}unsigned char fitpad[8]={0};int have_fit=0;uint8_t landed_planets[256]={0};int have_landed=0;if(ok&&s.version>=13){ok=fread(fitpad,1,8,f)==8;if(ok){have_fit=1;for(int s=0;s<6;s++){int m=fitpad[s];if(m==0xff)continue;if(m<=0||m>=EQUIPMENT_COUNT||equip_slot_for(m)!=s){have_fit=0;break;}}}}if(ok&&s.version>=14){ok=fread(landed_planets,1,sizeof(landed_planets),f)==sizeof(landed_planets);if(ok)have_landed=1;}uint32_t tut_step=0,tut_seen=0;if(ok&&s.version>=15){ok=load_u32(f,&tut_step)&&load_u32(f,&tut_seen);if(tut_step>TUTORIAL_COUNT+1||tut_seen>tut_step)ok=0;}char profile_name[25]="JAMESON";uint32_t profile_portrait=0;uint8_t bounty_done[256]={0};if(ok&&s.version>=16){ok=fread(profile_name,1,25,f)==25&&load_u32(f,&profile_portrait)&&fread(bounty_done,1,256,f)==256;if(profile_name[24]!=0||!profile_name[0]||!(profile_portrait<8||((profile_portrait&0x80000000u)&&!(profile_portrait&0x7f000000u)&&(profile_portrait&15u)<10u)))ok=0;int nonspace=0;for(int i=0;i<24&&profile_name[i];i++){if((unsigned char)profile_name[i]<32||(unsigned char)profile_name[i]>126)ok=0;if(profile_name[i]!=' ')nonspace=1;}if(!nonspace)ok=0;for(int i=0;i<256;i++)if(bounty_done[i]&~31u)ok=0;}uint32_t surface_saved[256][BODY_COUNT]={{0}};if(ok&&s.version>=17)for(int sys=0;ok&&sys<256;sys++)for(int body=0;ok&&body<BODY_COUNT;body++){ok=load_u32(f,&surface_saved[sys][body]);if(surface_saved[sys][body]&~(s.version>=26?SURFACE_V26_MASK:0x0fff7fu))ok=0;}float saved_clock=0;uint32_t clock_bits=0;if(ok&&s.version>=18){ok=load_u32(f,&clock_bits);memcpy(&saved_clock,&clock_bits,4);if(!isfinite(saved_clock)||saved_clock<0||saved_clock>=86400)ok=0;}uint32_t station_saved[256][HUB_COUNT]={{0}};if(ok&&s.version>=19)for(int sys=0;ok&&sys<256;sys++)for(int hub=0;ok&&hub<HUB_COUNT;hub++){ok=load_u32(f,&station_saved[sys][hub]);if(station_saved[sys][hub]&~0x7ffffu)ok=0;if((station_saved[sys][hub]&3)>2||((station_saved[sys][hub]>>2)&15)>9)ok=0;}uint32_t crime_saved[256]={0};if(ok&&s.version>=20)for(int sys=0;ok&&sys<256;sys++){ok=load_u32(f,&crime_saved[sys]);unsigned heat=crime_saved[sys]>>8;if(heat>1000||heat>(unsigned)warrants[sys]||(heat&&!(crime_saved[sys]&CRIME_CARGO))||(!warrants[sys]&&crime_saved[sys]))ok=0;}else if(s.version<20)for(int sys=0;sys<256;sys++)if(warrants[sys]>0)crime_saved[sys]=CRIME_UNKNOWN;uint8_t rift_saved[256]={0};if(ok&&s.version>=21){ok=fread(rift_saved,1,256,f)==256;for(int sys=0;sys<256;sys++)if(rift_saved[sys]&~15u)ok=0;}SocialState social_saved;memset(&social_saved,0,sizeof(social_saved));if(ok&&s.version>=22)ok=social_read(f,&social_saved,s.version);unsigned char bank[FIT_SAVE_BYTES];memset(bank,FIT_EMPTY,sizeof(bank));bank[18]=bank[19]=0;
 if(ok&&s.version>=24){
  ok=fread(bank,1,sizeof(bank),f)==sizeof(bank);
  if(!have_fit||bank[19]||bank[18]>=24||bank[18]%6)ok=0;
  unsigned char seen[EQUIPMENT_COUNT]={0};int weapons=0;for(int slot=0;slot<24;slot++){int item=slot<6?fitpad[slot]:bank[slot-6];if(item==FIT_EMPTY)continue;if(!fit_value_valid(slot,item)||slot/6>=fit_capacity(s.ship,slot%6))ok=0;else {if(seen[item])ok=0;seen[item]=1;if(slot%6==FIT_WPN)weapons++;}}
  int active=bank[18]>=24?FIT_EMPTY:bank[18]<6?fitpad[bank[18]]:bank[bank[18]-6];
  if(bank[18]>=24||bank[18]%6|| (active==FIT_EMPTY&&weapons))ok=0;
 }
 if(ok)ok=save_verify_end(f,s.version);fclose(f);for(int i=0;i<256;i++)if(warrants[i]<0||warrants[i]>1000)ok=0;
 if(ok&&s.version>=6&&(jobhead[0]<0||jobhead[0]>MISSION_SLOTS||jobhead[1]<0||jobhead[1]>=MISSION_SLOTS))ok=0;
 if(ok&&s.version>=6)for(int i=0;i<jobhead[0];i++){Job *j=&packed[i];if(j->dest<0||j->dest>255||j->type<0||j->type>=MISSION_TYPES||j->stage<0||j->stage>1||j->target< -1||j->target>=NPC_COUNT||j->item<1||j->item>=BODY_COUNT||j->origin<0||j->origin>255||j->reward<0||j->reward>100000||!isfinite(j->time)||j->time<=0||j->time>3600)ok=0;}
 if(ok&&s.version>=26){static Game validation;galaxy(validation.systems);for(int sys=0;ok&&sys<256;sys++)for(int body=0;ok&&body<BODY_COUNT;body++)if(!observatory_flags_valid(&validation,sys,body,surface_saved[sys][body]))ok=0;}
 if(!ok||s.magic!=0x41455053||(s.version<1||s.version>26)||upgrades<0||upgrades>524287||(s.contract>=0&&(mission[0]<0||mission[0]>=MISSION_TYPES||mission[1]<0||mission[1]>1||mission[2]<-1||mission[2]>=NPC_COUNT||mission[3]<1||mission[3]>=BODY_COUNT||mission[4]<0||mission[4]>255))||(s.version>=4&&(mission[5]<-1||mission[5]>1||mission[6]<0||mission[6]>=MISSION_TYPES||mission[7]<0||mission[7]>255))||s.system<0||s.system>255||s.destination<0||s.destination>255||s.ship<0||s.ship>=player_ship_count||s.credits<0||s.credits>100000000||s.kills<0||s.legal<0||s.contract< -1||s.contract>255||!isfinite(s.fuel)||s.fuel<0||s.fuel>player_ships[s.ship].range||!isfinite(s.remaining)||s.remaining<0||s.reward<0||s.reward>100000||s.laser<0||s.laser>1||jobhead[0]<0||jobhead[0]>MISSION_SLOTS||jobhead[1]<0||jobhead[1]>=MISSION_SLOTS)return 0;
 int cargo=0;for(int i=0;i<GOODS;i++){if(s.cargo[i]<0||s.cargo[i]>100000||s.stock[i]<0||s.stock[i]>100000||s.price[i]<0||s.price[i]>1020)return 0;if(goods[i].unit=='t')cargo+=s.cargo[i];}if(cargo>player_ships[s.ship].capacity+((upgrades&64)?16:((upgrades&8)?8:0)))return 0;
 if(!g){return 1;}
 game_init(g);g->social=social_saved;memcpy(g->rift_logged,rift_saved,256);memcpy(g->crime_record,crime_saved,sizeof(crime_saved));memcpy(g->station_progress,station_saved,sizeof(station_saved));g->world_clock=saved_clock;memcpy(g->surface_progress,surface_saved,sizeof(surface_saved));memcpy(g->commander_name,profile_name,25);g->commander_portrait=profile_portrait;memcpy(g->bounty_claimed,bounty_done,256);g->tutorial_step=(int)tut_step;g->tutorial_seen=(int)tut_seen;if(s.version>=9){g->saga_chapter=(int)saga[0];g->saga_step=(int)saga[1];g->saga_flags=(int)saga[2];g->saga_choice=(int)saga[3];g->saga_dest=(int)saga[4];g->saga_start=(int)saga[5];for(int i=0;i<4;i++)g->saga_trust[i]=(int)saga[6+i];}g->campaign_stage=(int)cp[0];g->campaign_choice=(int)cp[1];g->campaign_flags=(int)cp[2];g->campaign_distance=cp_distance;g->campaign_fuel=cp_fuel;g->system=s.system;g->destination=s.destination;g->route_goal=s.version>=10?route_goal:-1;if(s.version>=11){g->passenger_dest=pax_dest;g->passenger_kind=(int)pax_kind;g->passenger_pay=(int)pax_pay;g->gift_flags=(int)gift;}else {g->passenger_dest=-1;g->passenger_kind=0;g->passenger_pay=0;g->gift_flags=0;}if(have_trav){for(int ti=0;ti<12;ti++){g->travellers[ti].sys=(uint8_t)(trav_pack[ti]&255);g->travellers[ti].dest=(uint8_t)((trav_pack[ti]>>8)&255);g->travellers[ti].flags=(uint8_t)((trav_pack[ti]>>16)&255);g->travellers[ti].slot=-1;}}g->credits=s.credits;g->kills=s.kills;g->legal=s.legal;memcpy(g->wanted,warrants,sizeof(warrants));g->wanted[g->system]=s.legal;if(s.legal&&!g->crime_record[g->system])g->crime_record[g->system]=CRIME_UNKNOWN;g->police_cargo_heat=g->crime_record[g->system]>>8;g->ship=s.ship;g->laser=s.laser;g->missiles=s.missiles;g->fuel=s.fuel;g->contract=s.contract;g->contract_reward=s.reward;g->contract_time=s.remaining;g->upgrades=upgrades;if(s.version>=24){memcpy(g->fit,fitpad,6);memcpy(g->fit+6,bank,18);g->active_weapon=bank[18];fit_rebuild(g);}else if(have_fit){int any=0;for(int s=0;s<6;s++){g->fit[s]=fitpad[s];if(fitpad[s]!=0xff)any=1;}if(any)fit_rebuild(g);else fit_synthesize(g);}else fit_synthesize(g);g->mission_type=mission[0];g->mission_stage=mission[1];g->mission_target=mission[2];g->mission_item=mission[3];g->mission_origin=mission[4];g->mission_result=mission[5];g->last_mission_type=mission[6];g->last_mission_system=mission[7];
 memcpy(g->cargo,s.cargo,sizeof(s.cargo));memcpy(g->stock,s.stock,sizeof(s.stock));memcpy(g->price,s.price,sizeof(s.price));g->discoveries=extra[0];g->scanned_flora=extra[1];g->scanned_fauna=extra[2];g->scanned_minerals=extra[3];g->scanned_anomalies=extra[4];memcpy(g->visited,visited,sizeof(visited));if(have_landed)memcpy(g->landed_planets,landed_planets,sizeof(g->landed_planets));if(s.version>=6){g->job_n=jobhead[0];g->job_sel=jobhead[1];memcpy(g->jobs,packed,sizeof(g->jobs));jobs_sync(g);}if(s.version>=7){g->story=storypack[0];g->story_flags=storypack[1];g->pip_sys=storypack[2];g->pip_eng=storypack[3];g->pip_wep=storypack[4];g->guild_chapter=storypack[5]>=0&&storypack[5]<=4?storypack[5]:0;g->guild_flags=storypack[6]&31;g->guild_choice=storypack[7];if(g->story<0||g->story>STORY_FREE)g->story=STORY_FREE;if(g->pip_sys<0||g->pip_sys>4||g->pip_eng<0||g->pip_eng>4||g->pip_wep<0||g->pip_wep>4||g->pip_sys+g->pip_eng+g->pip_wep!=8){g->pip_sys=2;g->pip_eng=2;g->pip_wep=4;}}else story_complete(g);game_spawn(g);if(social_elapsed(social_now(g),g->social.seen[g->system],86400))social_arrive(g);message(g,"Commander loaded.");g->voice_time=0;g->voice_who=0;g->voice[0]=0;if(g->cue==SFX_COMM)g->cue=SFX_SELECT;return 1;
}
#define CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);if(!ok)fails++;}while(0)
#include "reliability-tests.h"
#include "journey-tests.h"
#include "campaign-tests.h"
int game_tests(const char *path){FILE *f=fopen(path,"w");if(!f)return 1;int fails=0;Game g;game_init(&g);
 #include "traffic-route-tests.h"
 #include "station-save-tests.h"
 #include "station-exterior-tests.h"
 #include "crime-record-tests.h"
 #include "rift-tests.h"
 #include "social-tests.h"
 {int local=1,identity=1;
  for(int sys=0;sys<256;sys++){
   g.system=sys;game_spawn(&g);
   for(int p=0;p<BOUNTY_POSTER_COUNT;p++){NPC *n=&g.npc[BOUNTY_NPC_FIRST+p];
    local&=isfinite(length(n->pos))&&length(n->pos)<200000;
    identity&=n->alive&&n->bounty_slot==p&&n->role==PIRATES&&n->traveller<0;
   }
  }
  CHECK(local,"Wanted: all 1280 targets spawn within local space, without unsigned coordinate wrap");
  CHECK(identity,"Wanted: every system retains all five pirate identities after traffic promotion");
  game_init(&g);
 }
 reliability_tests(f,&fails);
 journey_tests(f,&fails);campaign_tests(f,&fails);
 #include "debug-tools-tests.h"
 CHECK(!strcmp(g.systems[7].name,"LAVE"),"galaxy index 7 is Lave");CHECK(g.systems[7].x==20&&g.systems[7].y==173,"Lave coordinates");
 CHECK(g.systems[7].economy==5&&g.systems[7].tech==4,"Lave economy/tech");
 int factions[FACTION_COUNT]={0};for(int i=0;i<NPC_COUNT;i++)factions[g.npc[i].role]++;
 CHECK(factions[0]&&factions[1]&&factions[2]&&factions[3],"all four factions spawn");
 CHECK(g.bodies[0].type==SUN&&g.bodies[3].type==GAS&&g.bodies[1].type==OCEAN,"system includes sun, gas giant and ocean world");
 {int mira=-1;for(int i=0;i<NPC_COUNT;i++)if(g.npc[i].alive&&g.npc[i].traveller==0)mira=i;
  CHECK(g.travellers[0].sys==7&&mira>=0&&!strcmp(traveller_name(g.npc[mira].traveller),"MIRA VANE"),"Lave hosts named traveller Mira Vane");
  g.travellers[0].sys=7;g.travellers[0].dest=129;g.system=129;game_spawn(&g);
  int mira_away=-1;for(int i=0;i<NPC_COUNT;i++)if(g.npc[i].alive&&g.npc[i].traveller==0)mira_away=i;
  CHECK(mira_away<0,"Mira stays abstract at Lave while commander is elsewhere");
  g.system=7;game_spawn(&g);mira=-1;for(int i=0;i<NPC_COUNT;i++)if(g.npc[i].alive&&g.npc[i].traveller==0)mira=i;
  CHECK(mira>=0&&!strcmp(traveller_name(0),"MIRA VANE"),"returning to Lave restores the same Mira callsign");}
 {game_init(&g);g.travellers[0].sys=42;g.travellers[0].dest=99;g.travellers[0].flags=1;g.travellers[3].sys=18;g.travellers[3].dest=55;g.travellers[3].flags=3;
  CHECK(save_game(&g,"test-travellers.sav"),"save V12 persists traveller routes");
  Game tload;CHECK(load_game(&tload,"test-travellers.sav"),"load V12 commander with travellers");
  CHECK(tload.travellers[0].sys==42&&tload.travellers[0].dest==99&&(tload.travellers[0].flags&1),"Mira route and met flag survive save");
  CHECK(tload.travellers[3].sys==18&&tload.travellers[3].dest==55&&tload.travellers[3].flags==3,"second traveller route survives save");
  remove("test-travellers.sav");remove("test-travellers.sav.bak");}
 launch(&g);g.speed=0;g.pos=(Vec3){0,0,-10000};for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 g.npc[0].alive=g.npc[1].alive=g.npc[2].alive=g.npc[3].alive=1;
 for(int i=0;i<4;i++)g.npc[i].pos=(Vec3){i*100.f,0,2000};
 game_tick(&g,.016f,0,0,0,0);
 CHECK(g.npc[LAW].target==PIRATES,"Law targets pirates");
 CHECK(g.npc[PIRATES].target==-1&&g.npc[PIRATES].escape_time>0,"pirates evade nearby Law instead of duelling patrols");
 CHECK(g.npc[TRADERS].target==-1&&g.npc[EXPLORERS].target==-1,"civilians never select attack targets");
 g.npc[PIRATES].alive=0;g.legal=5;g.pos=g.npc[LAW].pos;game_tick(&g,.016f,0,0,0,0);game_tick(&g,.016f,0,0,0,0);
 CHECK(g.npc[LAW].target==-2,"Law pursues wanted player");
 game_init(&g);
 int credits=g.credits,stock=g.stock[0];CHECK(trade(&g,0,1)&&trade(&g,0,0)&&g.credits==credits&&g.stock[0]==stock,"market round trip conserves credits/stock");
 g.credits=0;CHECK(!trade(&g,0,1),"cannot buy without credits");
 g.credits=100000;g.cargo[0]=8;CHECK(!trade(&g,0,1),"cargo capacity enforced");g.cargo[0]=0;
 CHECK(!jump_start(&g),"cannot jump while docked");launch(&g);g.pos=(Vec3){0,0,STATION_Z-station_comms_range(&g,0)-100};CHECK(!dock(&g),"cannot dock remotely");g.pos=(Vec3){0,0,station_entry_z_for(&g,0)-300};g.speed=300;CHECK(dock(&g)&&!g.docked,"nearby request starts guided approach without teleporting");for(int i=0;i<1200;i++){game_tick(&g,1.f/60,0,0,0,0);}CHECK(g.docked,"guided docking completes before services open");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,STATION_Z+station_half_for(&g,0)*7+200};g.speed=0;CHECK(dock(&g)&&g.dock_stage==1,"station docking request starts guidance from behind");for(int i=0;i<4000&&!g.docked;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.docked,"station guidance reaches the third-person arrival sequence");
 CHECK(save_game(&g,"test-commander.sav"),"save commander");Game loaded;CHECK(load_game(&loaded,"test-commander.sav")&&loaded.credits==g.credits,"load commander round trip");remove("test-commander.sav");
 game_init(&g);launch(&g);
 {int p=-1,c=-1;for(int i=0;i<36;i++)if(g.npc[i].alive&&g.npc[i].role==PIRATES&&p<0)p=i;for(int i=0;i<36;i++)if(g.npc[i].alive&&g.npc[i].role==LAW&&c<0)c=i;
  if(p>=0&&c>=0){g.npc[p].pos=(Vec3){400,80,2800};g.npc[c].pos=(Vec3){400,80,2500};g.npc[p].dir=(Vec3){0,0,-1};g.npc[c].dir=(Vec3){0,0,1};g.npc[p].target=c;g.npc[c].target=p;g.npc[p].health=20;g.npc[p].shield=0;g.npc[c].health=20;g.npc[c].shield=0;}}
 for(int i=0;i<6000;i++)game_tick(&g,1.f/60,0,0,0,0);
 {int available=0;for(int p=0;p<BOUNTY_POSTER_COUNT;p++)available+=g.npc[BOUNTY_NPC_FIRST+p].alive&&!bounty_target_taken(&g,p);
 CHECK(available==BOUNTY_POSTER_COUNT,"ambient pursuit preserves all unengaged Wanted targets over 100 seconds");}
 game_init(&g);g.system=0;g.systems[g.system].government=7;game_spawn(&g);int safe=0,lawful_patrols=0;for(int i=0;i<NPC_COUNT;i++){safe+=g.npc[i].role==PIRATES;lawful_patrols+=g.npc[i].alive&&g.npc[i].role==LAW;}
 g.systems[g.system].government=0;game_spawn(&g);int dangerous=0,pirate_law=0;for(int i=0;i<NPC_COUNT;i++){dangerous+=g.npc[i].role==PIRATES;pirate_law+=g.npc[i].alive&&g.npc[i].role==LAW;}
 CHECK(danger_rating(&g,g.system)==5&&dangerous>safe,"higher danger spawns more pirates");
 CHECK(lawful_patrols>0&&pirate_law==0,"government identity: lawful systems field patrols and pirate systems field no Law ships");
 int remote=0,hub=0,alive=0;for(int i=0;i<36;i++)if(g.npc[i].alive){alive++;float d=length(g.npc[i].pos);if(d>8000)remote++;if(d<5000)hub++;}
 CHECK(alive<=18&&remote>=2&&hub>=1,"traffic stays sparse, with ships at the hub and out among the worlds");
 int e0=-1,e1=-1;for(int i=0;i<36;i++)if(g.npc[i].alive&&g.npc[i].role==EXPLORERS){if(e0<0)e0=i;else if(e1<0)e1=i;}
 if(e0<0||e1<0){for(int i=0;i<36&&(e0<0||e1<0);i++)if(!g.npc[i].alive||g.npc[i].role!=EXPLORERS){NPC *n=&g.npc[i];n->role=EXPLORERS;n->freighter=0;n->alive=1;n->health=80;n->shield=40;n->waypoint=1;n->mesh=mesh_id("ADDER");n->radius=30;n->dir=(Vec3){0,0,1};n->pos=(Vec3){(e0<0?-1:1)*400.f,200.f,12000.f};if(e0<0)e0=i;else e1=i;}}
 g.npc[e1].dir=g.npc[e0].dir;g.npc[e1].waypoint=g.npc[e0].waypoint;
 CHECK(e0>=0&&e1>=0&&g.npc[e0].waypoint==g.npc[e1].waypoint&&dot(g.npc[e0].dir,g.npc[e1].dir)>.7f,"explorers survey in formation");
 int belts=0,fauna=0;for(int s=0;s<64;s++){belts+=system_rock_belt(s)+system_ice_belt(s);fauna+=system_whales(s)+system_comet(s);}
 CHECK(system_rock_belt(7)&&!system_whales(7)&&belts>=20&&fauna>=4&&fauna<=24,"belts and fauna appear in some systems, not all");
 launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.pos=(Vec3){0,0,-20000};g.speed=0;g.boost=1;
 for(int i=0;i<60;i++){game_tick(&g,1.f/60,0,0,1,0);}CHECK(g.speed>3000,"boost accelerates beyond normal speed");
 float release_speed=g.speed,release_cruise=player_ships[g.ship].speed*(.70f+.15f*g.pip_eng);g.boost=0;game_tick(&g,.016f,0,0,0,0);
 CHECK(g.speed<release_speed&&g.speed>release_cruise,"boost release begins a visible coast-down instead of snapping to cruise");
 for(int i=0;i<120;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.speed<=release_cruise+1,"boost coast-down settles back to normal cruise speed");
 g.heat=100;g.boost=0;g.speed=player_ships[g.ship].speed*8.f;float heat_before=g.heat;for(int i=0;i<30;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.heat<heat_before,"heat cools after boost release at high speed");
 game_init(&g);launch(&g);g.speed=0;g.pitch=0;
 for(int i=0;i<262;i++)game_tick(&g,.016f,0,1,0,0);
 CHECK(fabsf(g.pitch)<.08f&&forward(&g).z>.99f,"flight pitch wraps through a full loop");
 game_init(&g);launch(&g);g.speed=0;g.pitch=1.8f;g.yaw=0;
 game_tick(&g,.1f,1,0,0,0);
 CHECK(g.yaw>.1f,"horizontal steering stays consistent past the vertical pole");
 game_init(&g);launch(&g);g.boost=0;g.heat=0;g.pip_eng=4;g.pip_sys=2;g.pip_wep=2;
 g.pos=add(g.bodies[0].pos,(Vec3){0,0,-(g.bodies[0].radius+20000.f)});
 g.speed=player_ships[g.ship].speed*(0.70f+0.15f*g.pip_eng);
 for(int i=0;i<180;i++)game_tick(&g,1.f/60,0,0,1,0);CHECK(g.heat>8,"over-speed cruise builds hull heat");
 float hot=g.heat;g.speed=0;g.boost=0;for(int i=0;i<120;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.heat<hot,"idle clear of the star vents heat");
 g.heat=0;g.pos=add(g.bodies[0].pos,(Vec3){0,0,g.bodies[0].radius+800});g.speed=0;
 for(int i=0;i<180;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.heat>10,"sun proximity cooks the hull");
 g.heat=99.5f;g.boost=1;game_tick(&g,.05f,0,0,0,0);CHECK(!g.dead&&g.heat>=100&&g.boost&&g.energy<100,"critical heat drains shields but does not cut boost");
 g.dead=0;g.energy=100;g.heat=92;g.boost=1;game_tick(&g,.016f,0,0,0,0);CHECK(g.boost&&g.energy<100,"hot boost drains shields without cutting boost");
 game_init(&g);launch(&g);g.pip_sys=4;g.pip_eng=0;g.pip_wep=4;g.boost=1;for(int i=0;i<240;i++)game_tick(&g,1.f/60,0,0,0,0);float low_eng_heat=g.heat;game_init(&g);launch(&g);g.pip_sys=2;g.pip_eng=4;g.pip_wep=2;g.boost=1;for(int i=0;i<240;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.heat<low_eng_heat,"extra ENG pips extend boost heat endurance");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 float hull_front=station_entry_z_for(&g,0);
 Vec3 before={80,0,hull_front-400};g.pos=(Vec3){80,0,STATION_Z+station_half_for(&g,0)+400};g.energy=100;g.speed=400;g.roll=1.2f;world_collision(&g,before);CHECK(g.pos.z<hull_front&&g.energy<100,"swept collision blocks station tunnelling at the generated front plane");
 CHECK(g.dead&&g.energy==0,"station impact destroys the player ship");g.dead=0;g.energy=100;
 Body *b=&g.bodies[1];before=add(b->pos,(Vec3){0,0,-b->radius-200});g.pos=add(b->pos,(Vec3){0,0,b->radius+200});float energy_before=g.energy;world_collision(&g,before);CHECK(length(sub(g.pos,b->pos))>b->radius&&g.approach==1,"planet boundary offers approach without impact");CHECK(g.energy==energy_before&&g.collision==0,"planet approach causes no collision damage");
 g.approach=-1;g.pos=add(b->pos,(Vec3){0,0,-b->radius-400});g.yaw=g.pitch=0;CHECK(approach_planet(&g,1)&&g.speed==0,"explicit planet approach stops ship and opens choice");
 Vec3 facing=forward(&g);turn_back(&g);CHECK(dot(facing,forward(&g))<-.999f&&g.approach==-1,"turn back reverses heading 180 degrees");
 game_init(&g);g.system=0;launch(&g);g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[2].alive=1;g.npc[2].pos=(Vec3){0,0,300};g.npc[2].dir=(Vec3){0,0,-1};g.npc[2].cooldown=0;game_tick(&g,.016f,0,0,0,0);CHECK(g.attacked>0,"incoming fire raises attack warning");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;int dest=-1;for(int i=0;i<256;i++)if(i!=g.system&&distance_ly(&g,g.system,i)*10<=g.fuel){dest=i;break;}g.destination=dest;float fuel=g.fuel,cost=distance_ly(&g,g.system,dest)*10;CHECK(jump_start(&g),"reachable warp starts");for(int i=0;i<500;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.system==dest&&fabsf(g.fuel-(fuel-cost))<.01f,"warp arrives in selected system and consumes fuel once");
 game_init(&g);add_crime(&g,15);CHECK(wanted_level(&g)==3&&g.wanted[g.system]==15,"crime raises the current system wanted level");g.credits=2000;g.police_stop=1;g.police_phase=0;int fine=police_fine(&g);CHECK(police_resolve(&g,0)&&g.credits==2000-fine&&g.legal==0,"paying police deducts exact fine and clears local warrant");
 add_crime(&g,10);g.credits=20;g.police_stop=1;g.police_phase=0;CHECK(!police_resolve(&g,0)&&g.police_stop,"unaffordable fine keeps police choice open");CHECK(police_resolve(&g,1)&&g.police_phase==4&&g.credits==0,"paid custody starts animated transfer and deducts the affordable fee");
 for(int i=0;i<400;i++)game_tick(&g,1.f/60,0,0,0,0);
 CHECK(g.police_stop&&g.police_phase==7&&g.docked&&g.legal==0,"paid custody waits on release papers");
 CHECK(police_acknowledge(&g)&&!g.police_stop,"release papers require acknowledgement");
 game_init(&g);g.ship=5;g.credits=0;g.cargo[0]=3;g.upgrades=128;g.laser=1;fit_synthesize(&g);g.police_stop=1;g.police_phase=0;add_crime(&g,10);CHECK(police_resolve(&g,1)&&g.police_phase==2,"zero-unit custody starts the jail transfer");for(int i=0;i<400&&g.police_stop;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.police_stop&&g.police_phase==6&&g.docked&&g.ship==0&&g.credits==0&&cargo_used(&g)==0&&g.upgrades==0&&g.legal==0,"jail release confiscates property and returns the basic ship");CHECK(police_acknowledge(&g)&&!g.police_stop,"seized custody release acknowledged");
 game_init(&g);launch(&g);add_crime(&g,10);g.police_stop=1;g.police_phase=0;int before_run=g.legal;CHECK(police_escape(&g)&&!g.police_stop&&g.legal>before_run&&g.police_grace>0&&g.attacked>0,"running from law resumes flight, escalates warrant and starts pursuit");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,-20000};g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;add_crime(&g,10);game_tick(&g,.016f,0,0,0,0);int cops=0;for(int i=36;i<NPC_COUNT;i++)cops+=g.npc[i].alive;CHECK(cops==4,"wanted level two dispatches four extra police");
 g.npc[36].pos=add(g.pos,(Vec3){0,0,300});game_tick(&g,.016f,0,0,0,0);CHECK(!g.police_stop&&g.police_warning>0,"approaching police issues a warning before arrest");for(int i=0;i<240&&!g.police_stop;i++)game_tick(&g,.016f,0,0,0,0);CHECK(g.police_stop&&g.police_phase==0,"warning escalates to warrant interception");Vec3 stopped=g.npc[36].pos;float frozen=g.time;game_tick(&g,.016f,1,1,1,1);CHECK(g.time==frozen&&length(sub(stopped,g.npc[36].pos))==0,"police encounter freezes world simulation");
 g.police_stop=0;g.police_phase=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;int origin=g.system;g.destination=dest;g.fuel=100;g.jump=.001f;game_tick(&g,.016f,0,0,0,0);CHECK(g.legal==0&&g.wanted[origin]==10,"warp leaves warrant behind in origin system");g.destination=origin;g.jump=.001f;game_tick(&g,.016f,0,0,0,0);CHECK(g.legal==10,"returning to original system restores its warrant");
 g.docked=1;g.fuel=10;CHECK(save_game(&g,"test-wanted.sav")&&load_game(&loaded,"test-wanted.sav")&&loaded.wanted[origin]==10,"wanted records survive save and load");remove("test-wanted.sav");
 /* Law rewrite: no heat on acquire; scan discovers contraband; desk clears when docked. */
 game_init(&g);g.credits=5000;int legal0=g.legal;CHECK(trade(&g,6,1)&&g.cargo[6]==1&&g.legal==legal0,"buying narcotics does not raise warrant until scanned");
 launch(&g);g.pos=(Vec3){0,0,-20000};g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[36].alive=1;g.npc[36].role=LAW;g.npc[36].pos=add(g.pos,(Vec3){0,0,300});g.police_grace=0;game_tick(&g,.016f,0,0,0,0);
 CHECK(g.police_stop&&g.police_phase==1,"dirty hold near Law opens a cargo scan");
 CHECK(police_scan_submit(&g)&&g.cargo[6]>0&&g.legal>0&&g.police_phase==0&&g.police_stop,"scan identifies cargo and allows voluntary surrender");
 g.credits=5000;CHECK(police_resolve(&g,0)&&g.legal==0&&!g.police_stop,"paying after a scan clears the warrant");
 game_init(&g);g.credits=5000;g.cargo[3]=2;launch(&g);g.pos=(Vec3){0,0,-5000};for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[6].alive=1;g.npc[6].role=LAW;g.npc[6].pos=add(g.pos,(Vec3){0,0,200});g.police_grace=0;game_tick(&g,.016f,0,0,0,0);
 CHECK(g.police_phase==1&&police_scan_refuse(&g)&&g.cargo[3]==0&&g.legal>=6&&g.police_phase==0,"refuse forces seizure and warrant");
 game_init(&g);launch(&g);g.cargo[6]=2;g.cargo[0]=3;police_begin(&g,1);
 CHECK(police_scan_submit(&g)&&g.police_cargo_heat==4,"scan attributes only the new cargo charge");
 CHECK(police_surrender_cargo(&g)&&g.legal==0&&g.cargo[6]==0&&g.cargo[0]==3&&g.police_phase==8,"cargo-only surrender clears cargo charge and preserves legal goods");
 CHECK(!police_surrender_cargo(&g)&&police_acknowledge(&g),"empty hold cannot surrender twice");
 game_init(&g);launch(&g);add_crime(&g,18);g.cargo[6]=2;police_begin(&g,1);police_scan_submit(&g);
 CHECK(police_surrender_cargo(&g)&&g.legal==18&&g.wanted[g.system]==18&&g.police_stop&&g.police_phase==0,"cargo surrender never wipes existing violent or other offences");
 g.cargo[6]=1;add_crime(&g,10);CHECK(police_surrender_cargo(&g)&&g.legal==28,"repeated surrender cannot reduce unrelated warrant");
 game_init(&g);launch(&g);g.cargo[6]=2;police_begin(&g,1);police_scan_submit(&g);add_crime(&g,1000);
 CHECK(police_surrender_cargo(&g)&&g.legal==1000,"surrender cannot discount violent offences at warrant cap");
 game_init(&g);g.docked=1;add_crime(&g,10);g.credits=5000;int desk=police_fine(&g);CHECK(police_pay_desk(&g)&&g.legal==0&&g.credits==5000-desk,"docked status desk pays and clears local warrant");
 game_init(&g);g.docked=1;g.credits=5000;g.cargo[0]=0;{int before=g.legal;/* smuggle accept via cargo only */g.cargo[6]=1;CHECK(g.legal==before&&cargo_contraband(&g)==1,"contraband cargo alone does not create a warrant");}
 game_init(&g);g.docked=1;g.fit[FIT_DEF]=7;fit_rebuild(&g);CHECK((g.upgrades&128)&&shield_regen_rate(&g)==4.5f,"military shield raises recharge to 4.5 per second");
 g.fit[FIT_WPN]=1;fit_rebuild(&g);CHECK(laser_shot_damage(&g)==24.f,"pulse laser deals 24 damage");g.fit[FIT_WPN]=2;fit_rebuild(&g);CHECK(laser_shot_damage(&g)==36.f,"beam laser deals 36 damage");
 game_init(&g);launch(&g);g.fit[FIT_WPN]=2;fit_rebuild(&g);g.pip_wep=0;float wep_low=weapon_output_multiplier(&g);g.pip_wep=4;float wep_high=weapon_output_multiplier(&g);CHECK(wep_low==.5f&&wep_high==1.5f&&wep_high>wep_low,"WEP pips scale weapon output");
 g.pip_sys=0;float sys_low=shield_regen_rate(&g)*(.50f+.25f*g.pip_sys);g.pip_sys=4;float sys_high=shield_regen_rate(&g)*(.50f+.25f*g.pip_sys);CHECK(sys_high>sys_low,"SYS pips scale shield recharge");
 g.pip_eng=0;float eng_low=.70f+.15f*g.pip_eng;g.pip_eng=4;float eng_high=.70f+.15f*g.pip_eng;CHECK(eng_high>eng_low,"ENG pips scale engine speed limit");
 g.fit[FIT_UTIL]=9;fit_rebuild(&g);launch(&g);g.heat=90;g.heat_sink_cd=0;dump_heat_sink(&g);CHECK(g.heat_sink_cd>0&&g.heat==50,"manual heat sink dumps 40 heat and starts cooldown");
 game_init(&g);g.fit[FIT_DEF]=16;fit_rebuild(&g);CHECK((g.upgrades&256)&&g.fit[FIT_DEF]==16,"ECM suite fits DEF and arms missile soft-kill");
 game_init(&g);g.docked=1;g.fit[FIT_HOLD]=11;g.fit[FIT_NAV]=4;fit_rebuild(&g);CHECK(save_game(&g,"test-fit.sav")&&load_game(&loaded,"test-fit.sav")&&loaded.fit[FIT_HOLD]==11&&loaded.fit[FIT_NAV]==4&&(loaded.upgrades&64)&&(loaded.upgrades&1),"save V13 persists fitted HOLD and NAV modules");remove("test-fit.sav");remove("test-fit.sav.bak");
 game_init(&g);g.upgrades=8|64;fit_synthesize(&g);CHECK(g.fit[FIT_HOLD]==11&&cargo_capacity(&g)==player_ships[g.ship].capacity+16,"V12 upgrades synthesize into fitted freight rack");
 game_init(&g);CHECK(fuel_cargo_units(&g)==0&&cargo_used(&g)==g.cargo[0],"fuel is tracked separately from cargo");memset(g.cargo,0,sizeof(g.cargo));g.fuel=0;CHECK(refuel_full(&g)&&g.fuel==player_ships[g.ship].range&&cargo_used(&g)==0,"empty tank refuels without consuming cargo");g.fuel=0;g.cargo[0]=cargo_capacity(&g);CHECK(refuel_full(&g)&&g.fuel==player_ships[g.ship].range,"full cargo hold does not block refuelling");
 CHECK(fit_value_valid(FIT_HOLD,23)&&fit_value_valid(FIT_UTIL,FIT_EMPTY),"V13 accepts valid catalog and empty slot values");CHECK(!fit_value_valid(FIT_WPN,23)&&!fit_value_valid(FIT_DEF,1),"V13 rejects invalid slot values");
 game_init(&g);g.roll=1.5707963f;Vec3 rolled=camera(&g,(Vec3){100,0,100});CHECK(fabsf(rolled.x)<.01f&&rolled.y< -99,"roll rotates camera and compass coordinates");
 Save legacy;memset(&legacy,0,sizeof(legacy));legacy.magic=0x41455053;legacy.version=1;legacy.system=7;legacy.destination=129;legacy.credits=1000;legacy.fuel=10;legacy.contract=-1;legacy.legal=5;FILE *legacyfile=fopen("test-legacy.sav","wb");if(legacyfile){fwrite(&legacy,1,sizeof(legacy),legacyfile);fclose(legacyfile);}CHECK(load_game(&loaded,"test-legacy.sav")&&loaded.credits==1000&&loaded.wanted[7]==5,"previous build saves import with a local warrant");remove("test-legacy.sav");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.pos=(Vec3){0,0,station_entry_z_for(&g,0)-20};g.speed=100;g.roll=station_angle(&g);for(int i=0;i<120&&!g.dock_stage;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.dock_stage==2&&!g.dead&&g.energy==100,"manual entry passes through actual station opening without damage");int sawWelcome=0;for(int i=0;i<400;i++){game_tick(&g,1.f/60,0,0,0,0);sawWelcome|=g.dock_stage==3;}CHECK(sawWelcome&&g.docked,"manual entry shows arrival then welcome before services");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,station_entry_z_for(&g,0)+5};g.roll=1.5707963f;g.speed=100;world_collision(&g,(Vec3){0,0,station_entry_z_for(&g,0)-10});CHECK(g.dead,"incorrect roll collides with rotating entrance rim");
 game_init(&g);launch(&g);g.time=62.831853f;g.roll=station_angle(&g);g.pos=add(station_port_world(&g,0,-station_half_for(&g,0)+5),station_arch_rotate((Vec3){40,0,0},station_angle(&g)));g.speed=100;world_collision(&g,add(station_port_world(&g,0,-station_half_for(&g,0)-10),station_arch_rotate((Vec3){40,0,0},station_angle(&g))));CHECK(g.dock_stage==2&&!g.dead,"rotated opening matches rotated collision coordinates");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,STATION_Z+station_half_for(&g,0)*7+200};CHECK(dock(&g),"communicator accepts request from behind station");int routeSafe=1;for(int i=0;i<4000&&!g.docked;i++){Vec3 prior=g.pos,hit;int stage=g.dock_stage;game_tick(&g,1.f/60,0,0,0,0);if(stage==1&&g.dock_stage==1&&station_intersection(&g,prior,g.pos,&hit))routeSafe=0;}CHECK(routeSafe&&g.docked,"guided route flies around hull before entering");
 game_init(&g);launch(&g);g.pos=hub_position(&g,1);CHECK(dock(&g)&&!g.docked&&g.dock_stage==2&&g.station_variant==1,"secondary relay starts the visible approach sequence");for(int i=0;i<400&&!g.docked;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.docked,"secondary relay approach reaches services");
 game_init(&g);launch(&g);g.pos=add(g.bodies[1].pos,(Vec3){0,0,-g.bodies[1].radius-800});g.yaw=g.pitch=0;CHECK(approach_planet(&g,1),"nearby facing planet can be approached");Vec3 nearPlanet=g.pos;turn_back(&g);CHECK(length(sub(g.pos,nearPlanet))==0&&dot(forward(&g),norm(sub(g.bodies[1].pos,g.pos)))<-.99f,"planet turn-back keeps position and faces away");CHECK(!approach_planet(&g,1),"planet cannot immediately re-prompt while facing away");
 game_init(&g);launch(&g);g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[2].alive=1;g.npc[2].pos=(Vec3){0,0,300};g.npc[2].dir=(Vec3){0,0,-1};g.npc[2].cooldown=0;game_tick(&g,.016f,0,0,0,0);CHECK(danger_rating(&g,7)==1&&g.npc[2].target!=-2&&g.attacked==0,"Lave is peaceful and pirates never target player");
 WorldState lw;world_state_build(&g,7,&lw);CHECK(lw.danger==1&&lw.station_count==HUB_COUNT&&lw.planet_count==4,"Lave certification: canonical peaceful world state");CHECK(lw.faction[TRADERS]+lw.faction[LAW]+lw.faction[PIRATES]+lw.faction[EXPLORERS]==lw.npc_count,"Lave certification: faction counts reconcile with traffic");
 g.docked=1;g.credits=20000;int valid_offers=0;for(int oi=0;oi<mission_count(&g);oi++)valid_offers+=mission_offer_valid(&g,oi,0);CHECK(valid_offers==mission_count(&g),"Lave certification: every displayed mission validates");CHECK(mission_landable_body(&g,7,1),"Lave certification: planet identity is landable and playable");
 int rep=world_reputation(&g,EXPLORERS);g.guild_chapter=2;CHECK(world_reputation(&g,EXPLORERS)>rep,"Lave certification: faction reputation changes with guild progress");
 game_init(&g);Body original=g.bodies[1];system_bodies(&g);CHECK(original.seed==g.bodies[1].seed&&length(sub(original.pos,g.bodies[1].pos))==0,"system generation repeats deterministically");g.system=0;system_bodies(&g);CHECK(original.seed!=g.bodies[1].seed&&original.radius!=g.bodies[1].radius,"other systems have distinct planets");
 {Body a=g.bodies[1];g.system=19;system_bodies(&g);Body b=g.bodies[1];CHECK(a.type!=b.type||a.color!=b.color||length(sub(a.pos,b.pos))>800,"distant systems diverge in planet type, colour or orbit");}
 game_init(&g);g.system=0;game_spawn(&g);Vec3 traffic0=g.npc[0].alive?g.npc[0].pos:(Vec3){99999,0,0};g.system=15;game_spawn(&g);CHECK(g.npc[0].alive&&length(sub(traffic0,g.npc[0].pos))>400,"ship traffic occupies a different layout in another system");
 game_init(&g);launch(&g);{int dest=-1;for(int i=0;i<256;i++)if(i!=g.system&&distance_ly(&g,g.system,i)*10<=g.fuel){dest=i;break;}g.destination=dest;CHECK(jump_start(&g),"warp starts for arrival-distance check");for(int i=0;i<500;i++)game_tick(&g,1.f/60,0,0,0,0);float hub=length(sub(g.pos,(Vec3){0,0,STATION_Z}));CHECK(g.system==dest&&hub>8500.f,"hyperspace drops the ship well outside the local hub");}
 game_init(&g);int large=0,models[64]={0},unique=0;for(int i=0;i<36;i++){large+=g.npc[i].freighter;models[g.npc[i].mesh]=1;}for(int i=0;i<64;i++)unique+=models[i];CHECK(large>=3&&unique>=9,"system traffic includes capital freighters and varied ship models");CHECK(g.npc[8].cruise<g.npc[0].cruise&&freight_extent(&g.npc[8]).z>g.npc[0].radius*2&&fabsf(g.npc[8].radius-length(freight_extent(&g.npc[8])))<.1f,"freighters use large shared hull dimensions and lower cruise speeds");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[0].alive=1;g.npc[0].role=TRADERS;g.npc[0].freighter=0;CHECK(trader_offer_hail(&g,0)&&g.trader_offer_active&&g.trader_offer_system==g.system&&g.trader_offer_npc==0,"trader hail creates a system-local cargo offer");{int need=g.trader_offer_need,reward=g.trader_offer_reward,qty=g.trader_offer_qty;for(int i=0;i<GOODS;i++)g.cargo[i]=0;g.cargo[need]=qty;CHECK(trader_offer_hail(&g,0)&&!g.trader_offer_active&&g.cargo[need]==0&&g.cargo[reward]==qty,"trader offer exchanges the requested commodity on a later hail");}
 g.system=0;g.systems[0].economy=0;int rich=mission_count(&g);g.systems[0].economy=2;CHECK(rich==5&&mission_count(&g)==5,"rich and poor systems both offer all five mission types");
 game_init(&g);int types=0;for(int i=0;i<5;i++)types|=1<<mission_type_for_offer(&g,i);CHECK(types==31,"mission board rotates through five job types");CHECK(accept_mission(&g,0)&&g.contract>=0&&g.mission_type==MISSION_EXPLORATION,"exploration mission acceptance records objective type");
 int filled=1;for(int i=1;i<5;i++)filled+=accept_mission(&g,i)!=0;CHECK(filled==5&&g.job_n==5&&!accept_mission(&g,0),"mission log holds five jobs and rejects a sixth");
 g.job_sel=0;jobs_sync(&g);g.system=g.contract;g.docked=0;game_spawn(&g);Body *scan=&g.bodies[g.mission_item];g.pos=add(scan->pos,(Vec3){0,0,-scan->radius-500});g.yaw=g.pitch=0;int scan_cash=g.credits;CHECK(approach_planet(&g,g.mission_item)&&g.job_n==4&&g.credits>scan_cash,"approaching marked planet completes exploration scan");
 game_init(&g);g.contract=g.system;g.contract_time=300;g.mission_type=MISSION_BOUNTY;g.mission_stage=0;game_spawn(&g);int mark=g.mission_target,bounty_cash=g.credits;CHECK(mark>=0&&is_mission_target(&g,BODY_COUNT+1+mark),"pirate hunt assigns a marked hostile");hit(&g,mark,2000,1);CHECK(g.contract<0&&g.credits>bounty_cash&&g.mission_result==1&&g.last_mission_type==MISSION_BOUNTY,"destroying marked pirate completes hunt and records network result");
 game_init(&g);g.system=0;game_spawn(&g);int board_ok=1;for(int p=0;p<BOUNTY_POSTER_COUNT;p++){int bi=bounty_target_index(&g,p);board_ok&=bi==BOUNTY_NPC_FIRST+p&&g.npc[bi].alive&&g.npc[bi].bounty_slot==p&&!bounty_target_taken(&g,p);}CHECK(board_ok,"every system spawns five live bounty-board targets");int bounty_before=g.credits;hit(&g,BOUNTY_NPC_FIRST,100000,1);CHECK(bounty_target_taken(&g,0)&&g.credits>bounty_before,"destroying a wanted target records the poster and pays its bounty");
 game_init(&g);g.contract=g.system;g.mission_type=MISSION_RESCUE;g.mission_stage=0;g.contract_reward=2500;game_spawn(&g);mark=g.mission_target;g.pos=g.npc[mark].pos;CHECK(mission_interact(&g,BODY_COUNT+1+mark)&&g.mission_stage==1&&is_mission_target(&g,0),"rescue pickup moves mission marker to station");int rescue_cash=g.credits;docking_complete(&g);CHECK(g.contract<0&&g.credits==rescue_cash+2500,"returning rescued pilot completes mission");
 game_init(&g);g.system=0;launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[0].alive=1;g.npc[0].role=PIRATES;g.npc[0].pos=(Vec3){0,0,1200};g.npc[0].dir=(Vec3){0,0,1};g.npc[0].radius=30;g.npc[0].health=80;g.npc[0].shield=40;CHECK(fire_missile(&g,BODY_COUNT+1)&&g.missiles==0,"locked hostile accepts player missile launch");for(int i=0;i<100&&g.missile_time>0;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(!g.npc[0].alive&&g.missile_time<=0,"homing missile reaches and destroys hostile");
 game_init(&g);launch(&g);g.incoming_missile=3;g.incoming_source=2;g.boost=1;g.speed=player_ships[g.ship].speed*5;game_tick(&g,.016f,0,0,0,0);CHECK(g.incoming_missile<=0,"high-speed boost evades incoming missile");
 game_init(&g);launch(&g);g.contract=g.system;g.contract_time=.001f;g.mission_type=MISSION_RESCUE;game_tick(&g,.016f,0,0,0,0);CHECK(g.contract==g.system&&g.job_n==1&&g.mission_result!=-1,"legacy short-duration mission no longer expires");
 game_init(&g);g.docked=1;CHECK(accept_mission(&g,0)&&accept_mission(&g,1)&&g.job_n==2, "two missions can be held at once");g.jobs[0].time=.001f;g.jobs[1].time=120;g.docked=0;g.pos=(Vec3){0,0,-8000};game_tick(&g,.016f,0,0,0,0);CHECK(g.job_n==2&&g.jobs[0].time==.001f&&g.jobs[1].time==120,"all board missions remain active without countdowns");
 game_init(&g);g.docked=1;g.credits=20000;CHECK(accept_mission(&g,0)&&mission_offer_active(&g,0)&&!accept_mission(&g,0)&&g.job_n==1,"board marks accepted work as in progress and blocks a duplicate");CHECK(abandon_mission(&g,0)&&g.job_n==0,"abandon removes the focused in-progress job");
 game_init(&g);g.upgrades=8;CHECK(cargo_capacity(&g)==player_ships[g.ship].capacity+8,"expanded bay adds eight tonnes of cargo capacity");g.upgrades=43;fit_synthesize(&g);g.docked=1;g.contract=mission_destination(&g,0);g.mission_type=MISSION_SMUGGLING;g.mission_item=1;g.mission_origin=g.system;CHECK(save_game(&g,"test-upgrades.sav")&&load_game(&loaded,"test-upgrades.sav")&&loaded.upgrades==43&&loaded.mission_type==MISSION_SMUGGLING,"upgrades and mission survive version-four save and load");remove("test-upgrades.sav");
 game_init(&g);g.docked=1;g.discoveries=6;g.scanned_flora=2;g.scanned_anomalies=1;g.visited[2]|=4;CHECK(save_game(&g,"test-codex.sav")&&load_game(&loaded,"test-codex.sav")&&loaded.discoveries==6&&loaded.scanned_flora==2&&(loaded.visited[2]&4),"codex and visited systems survive version-six save");remove("test-codex.sav");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,STATION_Z-station_comms_range(&g,0)-1000};CHECK(!dock(&g),"standard docking communicator has limited range");g.upgrades|=1;CHECK(dock(&g),"docking computer extends guided docking range");
 game_init(&g);launch(&g);int floating=0;for(int i=0;i<DEBRIS_COUNT;i++)floating+=g.debris[i].alive;CHECK(floating>=8,"systems spawn cargo canisters and wreckage");
 int loot=-1;for(int i=0;i<DEBRIS_COUNT;i++)if(g.debris[i].alive&&!g.debris[i].wreck&&!g.debris[i].rock){loot=i;break;}CHECK(loot>=0,"cargo filter has at least one canister");g.pos=g.debris[loot].pos;int hold=g.cargo[g.debris[loot].good];CHECK(salvage(&g,DEBRIS_ID_MIN+loot)&&g.tractor_time>0,"close salvage starts the tractor beam");for(int i=0;i<50;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(!g.debris[loot].alive&&g.cargo[g.debris[loot].good]==hold+1,"tractor beam collects a cargo canister");
 game_init(&g);g.system=0;launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[0].alive=1;g.npc[0].role=TRADERS;g.npc[0].freighter=0;g.npc[0].pos=(Vec3){0,0,800};g.npc[0].dir=(Vec3){0,0,1};g.npc[0].health=10;g.npc[0].shield=0;int debris_before=0;for(int i=0;i<DEBRIS_COUNT;i++)debris_before+=g.debris[i].alive;hit(&g,0,200,1);int after=0,pods=0;for(int i=0;i<DEBRIS_COUNT;i++){after+=g.debris[i].alive;pods+=g.debris[i].alive&&!g.debris[i].wreck;}CHECK(!g.npc[0].alive&&after>debris_before&&pods>0,"destroyed ships release cargo and wreckage");
 g.cargo[0]=cargo_capacity(&g);int cash=g.credits,sold=-1;for(int i=0;i<DEBRIS_COUNT;i++)if(g.debris[i].alive&&!g.debris[i].wreck&&!g.debris[i].rock){sold=i;g.pos=g.debris[i].pos;break;}CHECK(sold>=0&&salvage(&g,DEBRIS_ID_MIN+sold)&&g.tractor_time>0,"full hold salvage starts the tractor beam");for(int i=0;i<50;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.credits>cash,"full hold sells salvage for credits");
 CHECK(!fire_missile(&g,DEBRIS_ID_MIN),"missiles cannot lock cargo or wreckage");
 game_init(&g);launch(&g);g.pos=add(g.bodies[1].pos,(Vec3){0,0,-g.bodies[1].radius-500});g.yaw=g.pitch=0;Vec3 parked=g.pos;CHECK(approach_planet(&g,1)&&enter_planet(&g)&&g.planet==1&&g.approach<0,"approach X-path enters atmosphere flight");
 CHECK(g.planet==1&&g.pos.y>terrain_height(&g,g.pos.x,g.pos.z)+100,"atmosphere spawn sits above generated terrain");
 CHECK(!land_planet(&g),"cannot land while high and fast");
 Vec3 pad=surface_site(&g,1);g.pos=add(pad,(Vec3){0,20,0});g.speed=12;g.energy=100;CHECK(!land_planet(&g),"landing stays locked until the story awards the landing kit");
 g.story_flags|=STORY_EV_LANDING_TECH;CHECK(land_planet(&g)&&g.surface==1,"story-awarded landing kit enables a slow pad approach");
 CHECK(eva_toggle(&g)&&g.surface==2,"commander can leave the landed ship");
 {int life=0;for(int i=0;i<LIFE_COUNT;i++)life+=g.life[i].alive;CHECK(life==LIFE_COUNT,"landed worlds spawn a full set of surface lifeforms");}
 {float farh=terrain_height(&g,pad.x+3300,pad.z+3300),nearh=terrain_height(&g,pad.x,pad.z),coast=terrain_height(&g,pad.x+9700,pad.z);
  CHECK(nearh==24.f&&isfinite(farh)&&isfinite(coast)&&fabsf(farh-nearh)>4.f&&coast<farh,"ocean terrain: level port opens into varied uplands and a distant coast");}
 CHECK(g.pos.y-terrain_height(&g,g.pos.x,g.pos.z)>=21.9f,"EVA camera remains above terrain at eye height");
 CHECK(survey_scan(&g)&&g.discoveries>0,"visor scan logs nearby surface life");
 float eva_floor=terrain_height(&g,g.pos.x,g.pos.z)+22;g.boost=1;game_tick(&g,.1f,0,0,0,0);float airborne=g.pos.y;g.boost=0;game_tick(&g,.016f,0,0,0,0);CHECK(airborne>eva_floor&&g.pos.y>eva_floor,"jetpack release transitions into a smooth fall");
 for(int i=0;i<180;i++)game_tick(&g,1.f/60,0,0,0,0);
 CHECK(fabsf(g.pos.y-(terrain_height(&g,g.pos.x,g.pos.z)+22))<.1f,"EVA landing restores safe eye clearance");
 g.pos.x+=200;CHECK(!eva_toggle(&g)&&g.surface==2,"cannot board from far away");
 g.pos=g.ship_pos;g.pos.y=terrain_height(&g,g.pos.x,g.pos.z)+22;CHECK(eva_toggle(&g)&&g.surface==1,"nearby commander boards the parked ship");
 CHECK(takeoff_planet(&g)&&g.surface==0,"takeoff returns to atmosphere flight");
 float ground=terrain_height(&g,g.pos.x,g.pos.z);g.pos.y=ground-20;g.speed=80;g.energy=100;game_tick(&g,.016f,0,0,0,0);CHECK(g.planet==1&&g.pos.y>=terrain_height(&g,g.pos.x,g.pos.z)+15,"surface collision keeps the ship above terrain");
 g.pos=(Vec3){pad.x+10000,pad.y,pad.z};g.pos.y=terrain_height(&g,g.pos.x,g.pos.z)+20;g.speed=10;
 int wet=terrain_is_water(&g,g.pos.x,g.pos.z);
 CHECK(wet&&!land_planet(&g),"cannot land on open water");
 CHECK(!fire_missile(&g,BODY_COUNT+1)&&g.planet==1,"missiles cannot launch in atmosphere");
 CHECK(!jump_start(&g)&&g.planet==1,"cannot warp from inside an atmosphere");
 leave_planet(&g);CHECK(g.planet<0&&length(sub(g.pos,parked))<1,"leaving atmosphere restores the parked orbit position");
 CHECK(!approach_planet(&g,1),"orbit restore faces away so the prompt does not reopen");
 game_init(&g);launch(&g);g.approach=3;CHECK(enter_planet(&g)&&g.planet==3,"gas giants allow a floating platform visit");
 #include "planet-approach-tests.h"
 #include "planet-eva-tests.h"
 #include "surface-world-tests.h"
 int rares=0;for(int i=0;i<ANOMALY_COUNT;i++)rares+=g.anomaly[i].alive;CHECK(rares>=1,"quiet Lave still contains a rare anomaly");
 g.pos=g.anomaly[0].pos;int disc=g.discoveries;CHECK(analysis_scan(&g,ANOMALY_ID_MIN)&&g.discoveries==disc+1,"close analysis scan catalogues an anomaly");
 CHECK(systems_visited(&g)>=1,"visited systems are recorded for the Codex");
 game_init(&g);
 CHECK(g.story==STORY_BRIEF&&story_menu_ok(&g,0)&&story_menu_ok(&g,6)&&story_menu_ok(&g,3)&&story_menu_ok(&g,12),"new commander has every service available while coaching remains optional");
 CHECK(g.cargo[0]==2&&story_hint(&g)[0]&&story_home_row(&g)==6,"new commander starts with food and a Controls lesson");
 CHECK(story_line(&g,0)[0]&&strstr(story_task(&g),"CONTROLS")&&story_title(&g)[0],"guild briefing names the controls lesson");
 CHECK(strstr(story_line(&g,0),"Ryn")!=0,"guild briefing names the missing surveyor");
 Game skipped;game_init(&skipped);story_skip(&skipped);CHECK(skipped.story==STORY_FREE&&story_menu_ok(&skipped,3),"skipping the campaign unlocks every deck");
 story_event(&g,STORY_EV_HELP);CHECK(g.story==STORY_LAUNCH&&g.voice_who==VOICE_KEI&&g.voice_time>0,"opening controls advances the briefing");
 launch(&g);CHECK(g.story==STORY_SIGHT,"first launch advances the campaign");
 story_event(&g,STORY_EV_TARGET);CHECK(g.story==STORY_RETURN,"locking a target advances the campaign");
 g.docked=1;story_event(&g,STORY_EV_DOCK);CHECK(g.story==STORY_LOCAL,"docking after the first flight advances the campaign");
 story_on_open(&g,11);CHECK(g.story==STORY_NET,"system details unlock GalacticNet");
 story_on_open(&g,14);CHECK(g.story==STORY_WORK,"GalacticNet unlocks the mission board");
 story_on_open(&g,12);CHECK(g.story==STORY_HOLD,"the mission board unlocks cargo");
 story_on_open(&g,1);CHECK(g.story==STORY_MAP,"cargo unlocks the galaxy map");
 story_on_open(&g,2);CHECK(g.story==STORY_POWER,"the map unlocks outfitting and power pips");
 CHECK(g.pip_sys==2&&g.pip_eng==2&&g.pip_wep==4,"default pips are two shields, two engines, four weapons");
 CHECK(pip_shift(&g,0)&&g.pip_sys==3&&g.pip_wep==3&&g.pip_sys+g.pip_eng+g.pip_wep==8,"pips steal from the strongest bank and keep eight assigned");
 CHECK(g.story==STORY_WORLD&&(g.story_flags&STORY_EV_LANDING_TECH),"moving pips advances the dirt chapter and awards the landing kit");
 {Game p=g;CHECK(pip_selected_move(&p,0,-1)&&p.pip_sys==2&&p.pip_eng==3,"taking a pip from SYS feeds the weakest bank");}
 story_event(&g,STORY_EV_WORLD);CHECK(g.story==STORY_ATLAS,"planet approach unlocks the Codex");
 story_on_open(&g,15);CHECK(g.story==STORY_FREE&&g.credits==3500,"codex completes the campaign with a Guild payout");
 CHECK(story_menu_ok(&g,3)&&story_menu_ok(&g,9),"free commander opens shipyard and debug");
 g.docked=1;CHECK(save_game(&g,"test-story.sav"),"save campaign state");
 Game campaign;CHECK(load_game(&campaign,"test-story.sav")&&campaign.story==STORY_FREE&&campaign.pip_sys==3&&campaign.pip_wep==3&&campaign.voice_time==0,"campaign and pips survive version-seven save");remove("test-story.sav");
 Save oldsix;memset(&oldsix,0,sizeof(oldsix));oldsix.magic=0x41455053;oldsix.version=6;oldsix.system=7;oldsix.destination=129;oldsix.credits=1000;oldsix.fuel=60;oldsix.contract=-1;FILE *six=fopen("test-v6.sav","wb");if(six){int zero_up=0,mission8[8]={0},extra5[5]={0};uint8_t vis[32]={0};int jobh[2]={0,0};Job emptyj[MISSION_SLOTS];memset(emptyj,0,sizeof(emptyj));fwrite(&oldsix,1,sizeof(oldsix),six);int warr[256]={0};fwrite(warr,1,sizeof(warr),six);fwrite(&zero_up,1,sizeof(zero_up),six);fwrite(mission8,1,sizeof(mission8),six);fwrite(extra5,1,sizeof(extra5),six);fwrite(vis,1,sizeof(vis),six);fwrite(jobh,1,sizeof(jobh),six);fwrite(emptyj,1,sizeof(emptyj),six);fclose(six);}CHECK(load_game(&campaign,"test-v6.sav")&&campaign.story==STORY_FREE&&campaign.pip_sys==2&&campaign.pip_wep==4,"version-six commanders import with the campaign complete");remove("test-v6.sav");
#include "flight-tools-tests.h"
 #include "missile-feedback-tests.h"
#include "npc-steering-tests.h"
#include "freight-tests.h"
 #include "outfitting-effect-tests.h"
#include "multislot-tests.h"
#include "observatory-tests.h"
#include "expanded-equipment-tests.h"
 #include "mission-network-tests.h"
 #include "system-spread-tests.h"
 #include "tutorial-save-tests.h"
 fprintf(f,"RESULT %d failures\n",fails);fclose(f);return fails;
}



