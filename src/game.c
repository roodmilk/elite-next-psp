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
#include "planet-profile.h"
#include "planet-noise.h"
#include "planet-pack.h"
#include "station-profile.h"
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
 {"ANACONDA",100,1500000,240,100}};
const int player_ship_count=sizeof(player_ships)/sizeof(*player_ships);
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
int mesh_id(const char *name){for(int i=0;i<mesh_count;i++)if(!strcmp(meshes[i].name,name))return i;return 4;}
static uint32_t random_u(Game *g){uint32_t x=g->rng;x^=x<<13;x^=x>>17;x^=x<<5;return g->rng=x;}
static float random_f(Game *g){return (random_u(g)&65535)/65535.0f;}
void message(Game *g,const char *s){snprintf(g->message,sizeof(g->message),"%s",s);g->message_time=5;}
void speak(Game *g,int who,const char *s){g->voice_who=who<VOICE_KEI||who>VOICE_CONTACT?VOICE_COMP:who;snprintf(g->voice,sizeof(g->voice),"%s",s);g->voice_time=6.5f;g->message_time=0;if(!g->cue)g->cue=(g->attacked>0||g->incoming_missile>0||g->encounter>0)?SFX_TALK:SFX_COMM;}
static void mark_visited(Game *g){g->visited[g->system>>3]|=(uint8_t)(1u<<(g->system&7));}
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
int cargo_used(const Game *g){int n=0;for(int i=0;i<GOODS;i++)if(goods[i].unit=='t')n+=g->cargo[i];if(g->passenger_dest>=0)n+=1;return n;}
int cargo_capacity(const Game *g){int c=player_ships[g->ship].capacity;if(g->upgrades&64)c+=16;else if(g->upgrades&8)c+=8;return c;}
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
int danger_rating(const Game *g,int system){return system==7?1:1+(7-g->systems[system].government)*4/7;}
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
Vec3 hub_position(const Game *g,int hub){int s=g->system;if(hub<=0)return (Vec3){0,0,STATION_Z};if(hub==1)return (Vec3){-5200.f+(s%5)*420.f,700.f,-6200.f-(s%7)*260.f};return (Vec3){6100.f-(s%6)*360.f,-500.f,8200.f+(s%9)*300.f};}
int nearest_hub(const Game *g){Vec3 p=g->pos;int best=0;float bd=length(sub(p,hub_position(g,0)));for(int i=1;i<HUB_COUNT;i++){float d=length(sub(p,hub_position(g,i)));if(d<bd){bd=d;best=i;}}return best;}
static int npc_role_for(const Game *g,int i){
 int risk=danger_rating(g,g->system);
 if(i<4)return i;
 if(i==8||i==20||i==32)return TRADERS;
 if(i==7||i==11||i==15)return EXPLORERS;
 if(i==6||i==10||i==14)return LAW;
 if(risk>=4&&(i==5||i==9||i==12||i==13||i==16||i==17))return PIRATES;
 if(risk>=2&&(i==5||i==9||i==12))return PIRATES;
 return TRADERS;
}
int wanted_level(const Game *g){return g->legal>0?(g->legal>=25?5:1+(g->legal-1)/5):0;}
void add_crime(Game *g,int points){g->legal+=points;if(g->legal>1000)g->legal=1000;g->wanted[g->system]=g->legal;}
int goods_restricted(int item){return item==3||item==6||item==10;}
int cargo_contraband(const Game *g){int n=0;for(int i=0;i<GOODS;i++)if(goods_restricted(i))n+=g->cargo[i];return n;}
static int police_seize_contraband(Game *g){
 int seized=0,heat=0;
 for(int i=0;i<GOODS;i++)if(goods_restricted(i)&&g->cargo[i]>0){
  heat+=2+g->cargo[i];seized+=g->cargo[i];g->cargo[i]=0;
 }
 if(heat){if(heat>25)heat=25;add_crime(g,heat);}
 return seized;
}
int police_fine(const Game *g){int wl=wanted_level(g);return (wl>0?wl:1)*500;}
void police_begin(Game *g,int phase){
 g->police_stop=1;g->police_phase=phase?1:0;g->speed=0;g->boost=0;g->approach=-1;g->cue=SFX_ALERT;
 if(g->police_phase==1){message(g,"Local Law: hold inspection. Submit, refuse, or run.");speak(g,VOICE_LAW,"Inspection. Submit your hold, refuse, or run.");}
 else {message(g,"Local Law: stop and settle your warrant.");speak(g,VOICE_LAW,"Commander, your vessel is under local arrest.");}
}
int police_scan_submit(Game *g){
 if(!g->police_stop||g->police_phase!=1)return 0;
 int seized=police_seize_contraband(g);
 if(!seized){g->police_stop=0;g->police_phase=0;g->police_grace=5.f;g->cue=SFX_UI;message(g,"Hold clear. Free to proceed.");speak(g,VOICE_LAW,"Clean hold. Proceed, Commander.");return 1;}
 g->police_phase=0;g->cue=SFX_ALERT;message(g,"Contraband seized. Settle the new warrant.");speak(g,VOICE_LAW,"Contraband confirmed. Settle now.");return 1;
}
int police_scan_refuse(Game *g){
 if(!g->police_stop||g->police_phase!=1)return 0;
 int seized=police_seize_contraband(g);add_crime(g,seized?6:4);
 g->police_phase=0;g->cue=SFX_ALERT;message(g,"Scan forced. Warrant filed. Settle or run.");speak(g,VOICE_LAW,"Refusal noted. Hold opened under warrant.");return 1;
}
int police_resolve(Game *g,int jail){
 if(!g->police_stop||g->police_phase!=0)return 0;
 int cost=jail?police_fine(g)/2:police_fine(g);
 if(!jail&&g->credits<cost){message(g,"Not enough units. Choose station custody.");return 0;}
 if(cost>g->credits)cost=g->credits;g->credits-=cost;g->legal=0;g->wanted[g->system]=0;g->police_stop=0;g->police_phase=0;g->attacked=0;g->speed=0;g->boost=0;g->cue=SFX_UI;
 if(jail){g->docked=1;g->pos=(Vec3){0,0,3200};g->energy=100;message(g,"Station custody complete. Release fee paid.");}
 else message(g,"Fine paid. Local warrant cleared.");
 return 1;
}
int police_escape(Game *g){
 if(!g->police_stop)return 0;
 int scan=g->police_phase==1;g->police_stop=0;g->police_phase=0;add_crime(g,scan?10:8);g->police_grace=7;g->attacked=7;
 g->speed=fmaxf(g->speed,player_ships[g->ship].speed*.8f);g->boost=0;
 for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive&&g->npc[i].role==LAW)g->npc[i].target=-2;
 g->cue=SFX_ALERT;message(g,"You ran. Local warrant raised; patrols are pursuing.");speak(g,VOICE_LAW,"Suspect fleeing. All patrols intercept.");return 1;
}
int police_pay_desk(Game *g){
 if(!g->docked||g->legal<=0||g->police_stop)return 0;
 int cost=police_fine(g);if(g->credits<cost){message(g,"Not enough units to clear this warrant.");return 0;}
 g->credits-=cost;g->legal=0;g->wanted[g->system]=0;g->cue=SFX_UI;
 message(g,"Local fine paid at the station desk. Warrant cleared.");return 1;
}
int approach_planet(Game *g,int body){if(g->dead||g->docked||g->jump>0||g->planet>=0||body<1||body>=BODY_COUNT)return 0;Body *b=&g->bodies[body];if(dot(forward(g),norm(sub(b->pos,g->pos)))<.7f){message(g,"Turn to face the planet, then press O.");return 0;}if(b->type==GAS){message(g,"Gas giants have no landing approach.");return 0;}if(length(sub(g->pos,b->pos))>b->radius+1000){message(g,"Move within 1,000 m of the surface first.");return 0;}g->approach=body;g->speed=0;g->boost=0;jobs_from_legacy(g);for(int i=0;i<g->job_n;){if(g->jobs[i].dest==g->system&&g->jobs[i].type==MISSION_EXPLORATION&&g->jobs[i].item==body){g->job_sel=i;mission_finish_slot(g,i,"Exploration scan complete. Payment received.");}else i++;}story_event(g,STORY_EV_WORLD);return 1;}
void turn_back(Game *g){if(g->approach>=1&&g->approach<BODY_COUNT){Body *body=&g->bodies[g->approach];Vec3 delta=sub(g->pos,body->pos);float distance=length(delta);Vec3 away=distance>1?mul(delta,1/distance):mul(forward(g),-1);if(distance<body->radius+100)g->pos=add(body->pos,mul(away,body->radius+100));g->yaw=atan2f(away.x,away.z);g->pitch=asinf(fmaxf(-1,fminf(1,away.y)));}else {g->yaw+=3.14159265f;g->pitch=-g->pitch;}g->speed=100;g->boost=0;g->approach=-1;}
static float wrap_range(float v,float half){float w=half*2;v=fmodf(v+half,w);if(v<0)v+=w;return v-half;}
static void site_xz(const Game *g,int i,float *x,float *z){
 static unsigned seeds[3]={~0u,~0u,~0u};static float xs[3],zs[3];unsigned seed=g->bodies[g->planet].seed;
 if(i>=0&&i<3){if(seeds[i]!=seed){unsigned h=sector_hash(seed+(unsigned)(i+1)*9973u);float a=(h%6283)*.001f,d=480.f+(h%900);xs[i]=cosf(a)*d;zs[i]=sinf(a)*d;seeds[i]=seed;}*x=xs[i];*z=zs[i];return;}
 unsigned h=sector_hash(seed+(unsigned)(i+1)*9973u);float a=(h%6283)*.001f,d=480.f+(h%900);*x=cosf(a)*d;*z=sinf(a)*d;
}
float terrain_height(const Game *g,float x,float z){
 if(g->planet<1||g->planet>=BODY_COUNT)return 0;
 float px,pz;site_xz(g,1,&px,&pz);
 float dx=x-px,dz=z-pz,d2=dx*dx+dz*dz;
 float h=24.f;
 if(d2>100.f*100.f){
  float n=planet_noise_sample(g->bodies[g->planet].seed,x,z);
  float edge=1.f-100.f/sqrtf(d2+1.f);if(edge<0)edge=0;if(edge>1)edge=1;
  int rocky=g->bodies[g->planet].type!=OCEAN;
  unsigned art=rocky?(g->bodies[g->planet].seed%4):0;
  h+=n*(rocky&&art==2?14.f:rocky&&art==1?6.f:10.f)*edge; /* existing terrain contract */
 }
 return h;
}
Vec3 surface_site(const Game *g,int i){float x=0,z=0;if(g->planet>=1)site_xz(g,i,&x,&z);return (Vec3){x,terrain_height(g,x,z)+22,z};}
int terrain_is_water(const Game *g,float x,float z){
 if(g->planet<1||g->bodies[g->planet].type!=OCEAN)return 0;
 float px,pz;site_xz(g,1,&px,&pz);float dx=x-px,dz=z-pz;
 return dx*dx+dz*dz>=420.f*420.f;
}
int enter_planet(Game *g){
 if(g->dead||g->docked||g->jump>0||g->planet>=0)return 0;
 int body=g->approach;if(body<1||body>=BODY_COUNT)return 0;if(g->bodies[body].type==GAS||g->bodies[body].type==SUN)return 0;
 g->orbit_pos=g->pos;g->orbit_yaw=g->yaw;g->orbit_pitch=g->pitch;g->orbit_roll=g->roll;g->orbit_speed=g->speed>40?g->speed:80;
 g->planet=body;g->approach=-1;g->surface=0;g->boost=0;
 float px,pz;site_xz(g,1,&px,&pz);g->pos=(Vec3){px-220,0,pz-60};g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+170;
 g->yaw=atan2f(px-g->pos.x,pz-g->pos.z);g->pitch=-.22f;g->roll=0;g->speed=48;g->hazard=0;g->cue=SFX_LAND;
 for(int i=0;i<LIFE_COUNT;i++){Lifeform *l=&g->life[i];l->alive=1;l->scanned=0;unsigned h=sector_hash(g->bodies[body].seed+i*131u);l->kind=g->bodies[body].type==OCEAN?(i%5==0?LIFE_MINERAL:(i&1?LIFE_FLORA:LIFE_FAUNA)):(h%3);float a=i*.95f+(h%7)*.1f;float rad=100.f+(h%90)+i*18;l->pos=(Vec3){px+cosf(a)*rad,0,pz+sinf(a)*rad};if(terrain_is_water(g,l->pos.x,l->pos.z)){l->pos.x=px+cosf(a)*140;l->pos.z=pz+sinf(a)*140;}l->pos.y=terrain_height(g,l->pos.x,l->pos.z)+(l->kind==LIFE_FAUNA?16:7);}
 message(g,"Atmosphere. Fly to the cyan pad. Triangle returns to orbit.");
 {
  const Body *wb=&g->bodies[body];
  const char *line="Rocky surface. Cyan pad in the scrub ahead.";
  if(wb->type==OCEAN)line="Ocean world. Island pad ahead.";
  else {int art=(int)(wb->seed%4);if(art==0)line="Arid flats. Cyan pad in the dunes.";else if(art==1)line="Ice field. Cyan pad on the shelf.";else if(art==2)line="Volcanic scrub. Cyan pad ahead.";else line="Forest rise. Cyan pad in the trees.";}
  speak(g,VOICE_COMP,line);
 }
 story_event(g,STORY_EV_WORLD);return 1;
}
void leave_planet(Game *g){
 if(g->planet<0)return;
 if(g->surface==2){message(g,"Board the ship before leaving the planet.");return;}
 if(g->surface==1){message(g,"Take off before returning to orbit.");return;}
 int body=g->planet;g->planet=-1;g->surface=0;
 g->pos=g->orbit_pos;g->yaw=g->orbit_yaw;g->pitch=g->orbit_pitch;g->roll=g->orbit_roll;g->speed=g->orbit_speed;
 Vec3 away=norm(sub(g->pos,g->bodies[body].pos));g->yaw=atan2f(away.x,away.z);g->pitch=asinf(fmaxf(-1,fminf(1,away.y)));g->boost=0;
 message(g,"Returned to orbit.");
}
int land_planet(Game *g){
 if(g->planet<0||g->surface||g->dead)return 0;
 float ground=terrain_height(g,g->pos.x,g->pos.z),alt=g->pos.y-ground;
 float px,pz;site_xz(g,1,&px,&pz);float pad=sqrtf((g->pos.x-px)*(g->pos.x-px)+(g->pos.z-pz)*(g->pos.z-pz));
 if(g->speed>48){message(g,"Too fast to land. Slow down.");return 0;}
 if(alt>48){message(g,"Too high. Descend toward the pad.");return 0;}
 if(terrain_is_water(g,g->pos.x,g->pos.z)){message(g,"Cannot land on water. Reach the cyan pad.");return 0;}
 if(pad>160){message(g,"Land on the cyan settlement pad ahead.");return 0;}
 g->surface=1;g->speed=0;g->boost=0;g->pitch=0;g->roll=0;g->pos.y=ground+18;g->ship_pos=g->pos;g->cue=SFX_LAND;
 message(g,"Landed. O to walk. Triangle to take off.");speak(g,VOICE_COMP,"Pad locked. Circle to walk.");return 1;
}
int takeoff_planet(Game *g){
 if(g->planet<0||g->surface!=1){if(g->surface==2)message(g,"Board the ship before takeoff.");return 0;}
 g->surface=0;g->speed=48;g->pitch=.2f;g->pos.y+=28;g->cue=SFX_BOOST;message(g,"Takeoff. Triangle returns to orbit.");return 1;
}
int eva_toggle(Game *g){
 if(g->planet<0||g->dead)return 0;
 if(g->surface==1){g->surface=2;g->ship_pos=g->pos;g->pos.x+=55;g->pos.z+=40;g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+22;g->yaw=atan2f(g->ship_pos.x-g->pos.x,g->ship_pos.z-g->pos.z);g->pitch=0;g->speed=0;g->boost=0;g->jetpack=0;message(g,"On foot. Nub look, D-pad move, Square scans, Circle boards.");speak(g,VOICE_COMP,"On foot. Survey flora and fauna around the pad.");return 1;}
 if(g->surface!=2)return 0;
 float dx=g->pos.x-g->ship_pos.x,dz=g->pos.z-g->ship_pos.z;if(dx*dx+dz*dz>3600){message(g,"Return to the parked ship to board.");return 0;}
 g->pos=g->ship_pos;g->surface=1;g->speed=0;message(g,"Boarded. Triangle takes off.");return 1;
}
static void planet_tick(Game *g,float dt,float turn,float pitch,int throttle){
 g->heat=fmaxf(0,g->heat-dt*22);g->shot=fmaxf(0,g->shot-dt);g->energy=fminf(100,g->energy+dt*1.5f);
 if(g->surface==2){
  /* Nub look (turn) + forward/back from pitch stick — same mental model as station walk. */
  g->yaw+=turn*dt*2.2f;g->pitch=0;
  float walk=0;if(pitch>.18f)walk=62.f;else if(pitch<-.18f)walk=-28.f;
  if(throttle>0)walk=62.f;else if(throttle<0)walk=-28.f;
  Vec3 dir=forward(g);g->pos.x+=dir.x*walk*dt;g->pos.z+=dir.z*walk*dt;
  if(g->boost){int lift=g->jetpack<=0;g->jetpack=fminf(80,g->jetpack+220*dt);if(lift)g->cue=SFX_BOOST;}else g->jetpack=fmaxf(-100,g->jetpack-140*dt);
  g->pos.y+=g->jetpack*dt;
  for(int i=0;i<LIFE_COUNT;i++)if(g->life[i].alive&&g->life[i].kind==LIFE_FAUNA){g->life[i].pos.x+=sinf(g->time*1.4f+i)*18*dt;g->life[i].pos.z+=cosf(g->time*1.1f+i)*14*dt;g->life[i].pos.y=terrain_height(g,g->life[i].pos.x,g->life[i].pos.z)+16;}
  g->pos.x=wrap_range(g->pos.x,4200);g->pos.z=wrap_range(g->pos.z,4200);
  float floor=terrain_height(g,g->pos.x,g->pos.z)+22;
  if(g->pos.y<floor){g->pos.y=floor;if(g->jetpack<0)g->jetpack=0;}
  float px,pz;site_xz(g,1,&px,&pz);float pd=(g->pos.x-px)*(g->pos.x-px)+(g->pos.z-pz)*(g->pos.z-pz);
  if(pd>140*140){g->hazard=fminf(100,g->hazard+dt*(g->bodies[g->planet].type==OCEAN?9:14));if(g->hazard>=100)g->energy-=dt*10;}
  else g->hazard=fmaxf(0,g->hazard-dt*22);
  g->speed=fabsf(walk);
  if(g->energy<=0){g->energy=0;g->dead=1;g->jump=0;g->explosion=0;g->cue=SFX_DEATH;message(g,"Ship destroyed. START for a new commander.");}
  return;
 }
 if(g->surface==1){
  g->yaw+=turn*dt*1.2f;g->pitch+=pitch*dt;if(g->pitch>.5f)g->pitch=.5f;if(g->pitch<-.2f)g->pitch=-.2f;
  g->speed=0;g->boost=0;g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+18;return;
 }
 float localturn=turn*cosf(g->roll)-pitch*sinf(g->roll),localpitch=turn*sinf(g->roll)+pitch*cosf(g->roll);
 g->yaw+=localturn*dt*1.4f;g->pitch+=localpitch*dt*1.4f;if(g->pitch>1.2f)g->pitch=1.2f;if(g->pitch<-1.2f)g->pitch=-1.2f;
 g->speed+=throttle*dt*(g->boost?700:110);if(g->speed<0)g->speed=0;float maxspeed=160*(g->boost?2.2f:1.f);if(g->speed>maxspeed)g->speed=maxspeed;
 g->pos=add(g->pos,mul(forward(g),g->speed*dt));
 if(!g->boost)g->pos.y-=32*dt;
 if(throttle>0&&g->pitch>0.08f)g->pos.y+=throttle*dt*55;
 g->pos.x=wrap_range(g->pos.x,4200);g->pos.z=wrap_range(g->pos.z,4200);
 float ground=terrain_height(g,g->pos.x,g->pos.z)+16;
 if(g->pos.y<ground){
  int water=terrain_is_water(g,g->pos.x,g->pos.z);g->pos.y=ground;
  if(g->speed>48||water){g->energy-=water?18:12;g->collision=2;snprintf(g->collide,sizeof(g->collide),water?"OCEAN SURFACE":"PLANET TERRAIN");message(g,water?"Collision: ocean surface. Shields damaged.":"Collision: terrain. Shields damaged.");}
  g->speed*=.32f;if(g->pitch<0)g->pitch=0;g->boost=0;
 }
 if(g->pos.y>780){leave_planet(g);if(g->planet<0)message(g,"Climbed out of the atmosphere. Orbit restored.");return;}
 if(g->energy<=0){g->dead=1;g->jump=0;g->energy=0;g->explosion=0;g->cue=SFX_DEATH;message(g,"Ship destroyed. START for a new commander.");}
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
static void place_civilian(Game *g,NPC *n,int i){
 Vec3 stn={0,0,3500};
 /* Spin, stretch and lift traffic per system so each star feels differently occupied. */
 unsigned layout=sector_hash((g->system+1)*0x85ebca6bu+i*97u);
 float spin=g->system*.41f+(layout&255)*.004f,spread=0.72f+((g->system*7+layout)%55)*.012f;
 float lift=((int)((layout>>8)%900)-450)*.08f;
 n->target=-1;n->flash=0;n->dir=(Vec3){0,0,1};
 if(n->freighter){n->alive=0;return;}
 if(n->role==EXPLORERS){Body *b=&g->bodies[1+(g->system%3)];int wing=0;for(int j=0;j<i;j++)if(g->npc[j].alive&&g->npc[j].role==EXPLORERS)wing++;float a=.35f*wing+spin;float ring=b->radius+1800*spread+((layout>>12)%900);n->waypoint=1;n->pos=add(b->pos,(Vec3){cosf(a)*ring+wing*210.f,280+lift+((g->system+wing)%5)*40.f,sinf(a)*ring});n->dir=norm((Vec3){-sinf(a),0,cosf(a)});return;}
 if(n->role==PIRATES){int world=1+(g->system+i)%3;if(world>=BODY_COUNT)world=2;n->waypoint=world;if(i==2){float a=spin+1.1f;float ring=1600.f+spread*900.f;n->pos=(Vec3){cosf(a)*ring,180+lift,3500+sinf(a)*ring};n->dir=norm(sub(stn,n->pos));return;}Body *b=&g->bodies[world];n->pos=add(b->pos,(Vec3){b->radius+1600*spread+(i%3)*220,160+lift+((g->system+i)%4)*70.f,((layout>>4)%800)-400});n->dir=(Vec3){0,0,-1};return;}
 if(n->role==LAW){float a=spin+(i%4)*.7f;float ring=(700.f+((layout>>6)%500))*spread;n->waypoint=0;n->pos=(Vec3){cosf(a)*ring,90+lift+(i%3)*40.f,3500+sinf(a)*ring};n->dir=norm(sub(stn,n->pos));return;}
 if(i==0||i==4){float a=spin+(i?1.2f:-.4f);float ring=1100.f+spread*600.f;n->waypoint=0;n->pos=(Vec3){cosf(a)*ring,60+lift,3500+sinf(a)*ring};n->dir=norm(sub(stn,n->pos));return;}
 float a=i*1.31f+g->system*.19f+(layout&127)*.01f;float ring=(11000.f+((layout>>10)%8000))*spread;n->waypoint=0;n->pos=(Vec3){cosf(a)*ring,180.f+lift+((g->system+i)%6)*80.f,3500+sinf(a)*ring}; n->dir=norm(sub(stn,n->pos));
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
void game_spawn(Game *g){
 jobs_from_legacy(g);
 system_bodies(g);g->dock_stage=0;g->station_variant=0;g->approach=-1;g->planet=-1;g->surface=0;g->boost=0;g->attacked=0;g->encounter=0;g->police_stop=0;g->police_phase=0;g->missile_time=0;g->missile_target=-1;g->incoming_missile=0;g->incoming_source=-1;
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
 for(int i=0;i<ac;i++){Anomaly *a=&g->anomaly[i];a->alive=1;a->scanned=0;a->kind=i&1;unsigned h=sector_hash(g->system*401u+i*9973u);float ang=i*2.15f+g->system*.27f+(h%400)*.001f;float ring=3200.f+((h>>8)%4200);a->pos=(Vec3){cosf(ang)*ring,((int)((h>>16)%1600)-800),sinf(ang)*ring*.75f+(int)((h>>20)%900)-200};}
 mark_visited(g);
 int budget=traffic_budget(g);
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&g->npc[i];memset(n,0,sizeof(*n));n->traveller=-1;
  n->role=i>=36?LAW:npc_role_for(g,i);n->alive=0;n->waypoint=0;n->target=-1;n->cooldown=0;n->flash=0;
  npc_blueprint(g,n,i);n->health=n->freighter?900:n->role==LAW?110:80;n->shield=n->freighter?100:n->role==LAW?60:40;
  if(i>=36){n->role=LAW;n->mesh=mesh_id("VIPER");n->alive=0;n->radius=80;n->scale=1;n->cruise=650;n->freighter=0;n->dir=(Vec3){0,0,1};continue;}
  n->alive=i<budget;if(!n->alive){n->dir=(Vec3){0,0,1};n->pos=(Vec3){0,0,8000};continue;}
  place_civilian(g,n,i);
 }
 /* Keep explorer wings aligned at spawn so formation reads clearly. */
 {int lead=-1;for(int i=0;i<36;i++)if(g->npc[i].alive&&g->npc[i].role==EXPLORERS){if(lead<0){lead=i;continue;}g->npc[i].dir=g->npc[lead].dir;g->npc[i].waypoint=g->npc[lead].waypoint;}}
 g->freight_next=freight_interval(g,sector_hash(g->system*13u));g->freight_gap=45;
 int initial_freight=freight_capacity(g);if(g->system!=7&&initial_freight>0)initial_freight=(g->system+1)%(initial_freight+1);
 for(int i=8;i<36;i+=12){NPC *n=&g->npc[i];n->alive=0;n->freight_state=FREIGHT_ABSENT;n->freight_timer=0;if(initial_freight>0&&freight_begin(g,n,i,1))initial_freight--;}
 int pirate=-1,cop=-1;
 for(int i=0;i<36;i++)if(g->npc[i].alive&&g->npc[i].role==PIRATES&&pirate<0)pirate=i;
 for(int i=0;i<36;i++)if(g->npc[i].alive&&g->npc[i].role==LAW&&cop<0)cop=i;
 if(pirate>=0&&cop>=0){
  float a=g->system*.41f+1.1f;g->npc[pirate].pos=(Vec3){cosf(a)*1700.f,120.f,3500.f+sinf(a)*1700.f};
  g->npc[cop].pos=add(g->npc[pirate].pos,(Vec3){80.f,30.f,-260.f});
  g->npc[cop].dir=norm(sub(g->npc[pirate].pos,g->npc[cop].pos));g->npc[pirate].dir=norm(sub(g->npc[cop].pos,g->npc[pirate].pos));
  g->npc[pirate].target=cop;g->npc[cop].target=pirate;g->npc[pirate].waypoint=0;g->npc[cop].waypoint=0;
 }
 int claimed[NPC_COUNT]={0};for(int s=0;s<g->job_n;s++){Job *j=&g->jobs[s];if(j->dest!=g->system||j->stage||(j->type!=MISSION_BOUNTY&&j->type!=MISSION_RESCUE))continue;int role=j->type==MISSION_BOUNTY?PIRATES:EXPLORERS;j->target=-1;for(int i=0;i<36;i++)if(g->npc[i].alive&&g->npc[i].role==role&&!g->npc[i].freighter&&!claimed[i]){j->target=i;claimed[i]=1;break;}}
 for(int s=0;s<g->job_n;s++){
  Job *j=&g->jobs[s];if(j->dest!=g->system||j->stage||(j->type!=MISSION_BOUNTY&&j->type!=MISSION_RESCUE))continue;
  if(j->target>=0&&j->target<NPC_COUNT&&g->npc[j->target].alive)continue;
  int role=j->type==MISSION_BOUNTY?PIRATES:EXPLORERS,slot=-1;
  for(int i=0;i<36;i++)if(!g->npc[i].alive&&i%12!=8){slot=i;break;}
  if(slot<0)for(int i=0;i<36;i++)if(!g->npc[i].freighter){slot=i;break;}
  if(slot<0)continue;
  NPC *n=&g->npc[slot];n->role=role;n->freighter=0;n->scale=1;n->cruise=650;n->mesh=mesh_id(role==PIRATES?"MAMBA":"ADDER");n->radius=30;
  for(int v=0;v<meshes[n->mesh].vertices;v++)n->radius=fmaxf(n->radius,length(meshes[n->mesh].v[v])*n->scale);
  n->alive=1;n->health=80;n->shield=40;n->target=-1;n->cooldown=0;n->flash=0;n->waypoint=role==PIRATES?2:1;
  if(role==PIRATES){n->pos=(Vec3){1600,120,3800};n->dir=(Vec3){0,0,-1};}
  else {Body *b=&g->bodies[1];n->pos=add(b->pos,(Vec3){b->radius+900.f,80,0});n->dir=norm(sub((Vec3){0,0,3500},n->pos));}
  j->target=slot;
 }
 travellers_promote(g);
 jobs_sync(g);
}
void game_init(Game *g){memset(g,0,sizeof(*g));g->rng=0x19841991;g->ai_phase=-1;galaxy(g->systems);g->system=7;g->destination=129;g->route_goal=-1;g->passenger_dest=-1;g->credits=1000;g->fuel=60;g->energy=100;g->docked=1;g->contract=-1;g->mission_target=-1;g->missile_target=-1;g->incoming_source=-1;g->approach=-1;g->planet=-1;g->missiles=1;g->pip_sys=2;g->pip_eng=2;g->pip_wep=4;fit_clear_all(g);travellers_seed(g);market(g);game_spawn(g);g->cargo[0]=2;message(g,"X opens the deck.");speak(g,VOICE_KEI,"Kei Aven. Ryn is missing — and this berth is yours until we find her.");}
void launch(Game *g){if(!g->docked)return;guild_event(g,GUILD_LAUNCH);g->docked=0;g->pos=(Vec3){0,0,0};g->yaw=g->pitch=0;g->speed=100;game_spawn(g);int before=g->story;story_event(g,STORY_EV_LAUNCH);if(g->story==before)message(g,"Station ahead. Select opens the deck.");if(g->story==STORY_SIGHT||g->story==STORY_RETURN)speak(g,VOICE_VENN,"Tower. Cleared. Soft launch — come home in one piece.");if(!g->cue)g->cue=SFX_DOCK;campaign_event(g,CP_LAUNCH);}
#include "docking.h"
#include "journey.h"
int trade(Game *g,int i,int buy){if(!g->docked||i<0||i>=GOODS)return 0;
 if(buy){if(g->credits<g->price[i]||g->stock[i]<1||(goods[i].unit=='t'&&cargo_used(g)>=cargo_capacity(g))){message(g,"Check units, stock and cargo space.");return 0;}g->credits-=g->price[i];g->stock[i]--;g->cargo[i]++;if(goods_restricted(i))message(g,"Restricted goods loaded. Law will scan your hold.");}
 else {if(g->cargo[i]<=mission_cargo_reserved(g,i)){message(g,g->cargo[i]?"Reserved for a mission. Abandon it to release cargo.":"Nothing to sell.");return 0;}g->cargo[i]--;g->stock[i]++;g->credits+=g->price[i];}g->cue=SFX_UI;
 if(!(buy&&goods_restricted(i))){char note[64];snprintf(note,sizeof(note),buy?"Bought %s.":"Sold %s.",goods[i].name);message(g,note);}
 return 1;
}
int buy_ship(Game *g,int i){if(!g->docked||i<0||i>=player_ship_count)return 0;if(i==g->ship){message(g,"Already on this ship.");return 0;}
 int cost=player_ships[i].price-player_ships[g->ship].price*3/4;
 if(g->credits<cost||cargo_used(g)>player_ships[i].capacity+((g->upgrades&64)?16:((g->upgrades&8)?8:0))){message(g,"Not enough units, or cargo will not fit.");return 0;}
 g->credits-=cost;g->ship=i;if(g->fuel>player_ships[i].range)g->fuel=(float)player_ships[i].range;g->cue=SFX_UI;message(g,"Ship exchanged. Cargo transferred.");return 1;
}
int jump_start(Game *g){if(g->planet>=0){message(g,"Return to orbit before engaging the hyperdrive.");return 0;}if(g->docked){message(g,"Launch before engaging the hyperdrive.");return 0;}
 if(g->jump>0){message(g,"Hyperspace already engaged.");return 0;}
 if(g->destination==g->system){message(g,"Already in that system.");return 0;}
 float cost=distance_ly(g,g->system,g->destination)*10;
 if(cost>g->fuel+0.01f){message(g,"Not enough fuel. Refuel at a hub or scoop a sun.");return 0;}
 g->jump=5;g->speed=0;g->boost=0;g->cue=SFX_WARP;message(g,"Hyperspace countdown: 5 seconds. Jump spends fuel.");return 1;
}
int contract_accept(Game *g){if(!g->docked||g->contract>=0)return 0;
 if(g->destination==g->system||distance_ly(g,g->system,g->destination)>10.0f){message(g,"Select a destination within 10 LY.");return 0;}
 if(g->credits<100){message(g,"Contract deposit: 10 units.");return 0;}
 g->credits-=100;g->contract=g->destination;g->contract_time=300;g->contract_reward=1000+(int)(distance_ly(g,g->system,g->destination)*200);message(g,"Courier contract accepted. Five minutes.");return 1;
}
static float npc_max_health(const NPC *n){return n->freighter?900:n->role==LAW?110:80;}
static float npc_max_shield(const NPC *n){return n->freighter?100:n->role==LAW?60:40;}
static void hit(Game *g,int i,float damage,int player);
static void ram_contact(Game *g,int i){NPC *n=&g->npc[i];int rescue=0;for(int s=0;s<g->job_n;s++)if(g->jobs[s].dest==g->system&&g->jobs[s].target==i&&g->jobs[s].type==MISSION_RESCUE)rescue=1;if(rescue){n->health=1;return;}hit(g,i,0,1);}
static void hit(Game *g,int i,float damage,int player){NPC *n=&g->npc[i];if(!n->alive)return;if(n->shield>0){float absorbed=fminf(n->shield,damage);n->shield-=absorbed;damage-=absorbed;}n->health-=damage;n->flash=.12f;if(player)g->cue=SFX_HIT;
 if(player&&n->role!=PIRATES){add_crime(g,n->freighter?8:5);message(g,n->freighter?"Freighter returns fire. Heavy police alert.":"Assault reported. Police alert.");}
 if(player&&(n->freighter||n->role==TRADERS||n->role==EXPLORERS||n->role==LAW)){n->target=-2;if(n->freighter)n->cooldown=0;}
 jobs_from_legacy(g);int protect=0;for(int s=0;s<g->job_n;s++){Job *j=&g->jobs[s];if(j->dest!=g->system||j->target!=i)continue;if(j->type==MISSION_RESCUE)protect=1;if(!player&&(j->type==MISSION_BOUNTY||j->type==MISSION_RESCUE))protect=1;}
 if(n->health<=0&&protect){n->health=1;if(player)message(g,"That's a rescue beacon. Don't fire.");return;}
 if(n->health<=0){n->alive=0;if(n->freighter){n->freight_state=FREIGHT_ABSENT;n->freight_timer=240;}wreck_from_npc(g,i);if(player){g->kills++;if(n->role!=PIRATES)add_crime(g,n->freighter?25:10);if(n->role==PIRATES){g->credits+=150;message(g,"Pirate destroyed. Bounty 15 units. Cargo released.");}else message(g,n->freighter?"Freighter destroyed. Heavy warrant filed. Cargo released.":"Contact destroyed. Cargo canisters released.");for(int s=0;s<g->job_n;){if(g->jobs[s].dest==g->system&&g->jobs[s].type==MISSION_BOUNTY&&g->jobs[s].target==i){g->job_sel=s;mission_finish_slot(g,s,"Pirate-hunt complete. Payment received.");}else s++;}}else g->npc_kills++;}
}
int salvage(Game *g,int id){
 if(g->docked||g->dead||g->jump>0||g->planet>=0||!IS_DEBRIS_ID(id))return 0;
 int i=id-DEBRIS_ID_MIN;Debris *d=&g->debris[i];if(!d->alive)return 0;
 if(d->rock){message(g,"Blast the asteroid; then collect its ore.");return 0;}
 if(length(sub(d->pos,g->pos))>500){message(g,"Close within 500 m to salvage.");return 0;}
 char note[96];
 if(d->wreck){g->credits+=d->qty;snprintf(note,sizeof(note),"HULL SALVAGE RECOVERED: %.1f UNITS.",d->qty*.1f);}
  else {int item=d->good;if(item<0||item>=GOODS)item=12;
  if(item==12&&(g->upgrades&131072))item=9; /* refinery: minerals → alloys */
  if(goods[item].unit=='t'&&cargo_used(g)+d->qty>cargo_capacity(g)){int payout=g->price[item]*d->qty/2;if(payout<10)payout=10;g->credits+=payout;snprintf(note,sizeof(note),"HOLD FULL. SOLD %s SALVAGE FOR %.1f UNITS.",goods[item].name,payout*.1f);}
  else {g->cargo[item]+=d->qty;if(goods_restricted(item))snprintf(note,sizeof(note),"SALVAGED RESTRICTED %s. HIDE IT FROM LAW SCANS.",goods[item].name);else snprintf(note,sizeof(note),"SALVAGED %d%c %s.",d->qty,goods[item].unit,goods[item].name);}
 }
 d->alive=0;message(g,note);g->cue=SFX_SCAN;return 1;
}
int analysis_scan(Game *g,int id){
 if(g->dead||g->docked||!IS_ANOMALY_ID(id))return 0;
 int i=id-ANOMALY_ID_MIN;Anomaly *a=&g->anomaly[i];if(!a->alive)return 0;
 if(length(sub(a->pos,g->pos))>1100){message(g,"Move within 1,100 m to analyse the anomaly.");return 0;}
 if(a->scanned){message(g,"Anomaly already logged in the Codex.");return 0;}
 a->scanned=1;g->scanned_anomalies++;g->discoveries++;g->credits+=280;g->cue=SFX_SCAN;
 message(g,a->kind?"Meridian echo logged. Research units awarded.":"Stellar anomaly catalogued. Research units awarded.");story_event(g,STORY_EV_SCAN);guild_event(g,GUILD_SCAN);return 1;
}
int survey_scan(Game *g){
 if(g->planet<0||g->surface!=2){message(g,"Exit the ship to survey surface life.");return 0;}
 float range=(g->upgrades&4096)?560.f:380.f;int best=-1;for(int i=0;i<LIFE_COUNT;i++)if(g->life[i].alive&&!g->life[i].scanned){float d=length(sub(g->life[i].pos,g->pos));if(d<range){range=d;best=i;}}
 if(best<0){message(g,"No unscanned life or minerals in visor range.");return 0;}
 Lifeform *l=&g->life[best];l->scanned=1;g->discoveries++;g->credits+=120;g->cue=SFX_SCAN;
 if(l->kind==LIFE_FLORA){g->scanned_flora++;message(g,"Flora discovered. Sample uploaded to Codex.");}
 else if(l->kind==LIFE_FAUNA){g->scanned_fauna++;message(g,"Fauna discovered. Behaviour logged.");}
 else {g->scanned_minerals++;if(cargo_used(g)>=cargo_capacity(g)){g->credits+=40;message(g,"Mineral logged. Hold full; ore sold for 4.0 units.");}else {if((g->upgrades&131072)){g->cargo[9]++;message(g,"Mineral refined to alloys onboard.");}else {g->cargo[12]++;message(g,"Mineral deposit scanned. 1t minerals collected.");}}}
 story_event(g,STORY_EV_SCAN);guild_event(g,GUILD_SCAN);return 1;
}
int mine_rock(Game *g,int id){
 if(g->planet>=0||g->docked||g->dead||g->jump>0||!IS_DEBRIS_ID(id))return 0;
 Debris *d=&g->debris[id-DEBRIS_ID_MIN];if(!d->alive||!d->rock)return 0;
 if(length(sub(d->pos,g->pos))-d->radius>2200)return 0;
 d->health-=mine_shot_damage(g)*(0.50f+0.25f*g->pip_wep);d->flash=.22f;g->cue=SFX_MINE;
 if(d->health<=0){
  /* Reuse the fractured rock's slot: ore cannot be lost to a full debris pool. */
  d->rock=0;d->wreck=0;d->good=12;d->radius=24;d->life=240;d->flash=.65f;
  message(g,"Ore released. Close within 500 m and press Circle.");
 }
 return 1;
}
int fire_missile(Game *g,int id){if(g->docked||g->dead)return 0;if(g->jump>0){message(g,"Missiles are locked during hyperspace.");return 0;}if(g->planet>=0){message(g,"Missiles cannot launch in atmosphere.");return 0;}if(g->missile_time>0)return 0;if(g->missiles<=0){message(g,"No missiles remaining.");return 0;}if(!IS_NPC_ID(id)){message(g,"Missile needs a ship target.");return 0;}int n=id-BODY_COUNT-1;if(!g->npc[n].alive||(g->npc[n].role!=PIRATES&&g->npc[n].target!=-2)){message(g,"Missile lock needs a hostile target.");return 0;}if(length(sub(g->npc[n].pos,g->pos))>12000){message(g,"Target outside 12,000 m missile range.");return 0;}g->missiles--;g->missile_target=n;g->missile_pos=g->pos;g->missile_time=8;g->cue=SFX_MISSILE;message(g,"Missile launched.");return 1;}
static float segment_distance(Vec3 a,Vec3 b,Vec3 p){Vec3 d=sub(b,a);float l=dot(d,d);float t=l>0?fmaxf(0,fminf(1,dot(sub(p,a),d)/l)):0;return length(sub(p,add(a,mul(d,t))));}
static void note_collision(Game *g,const char *what){
 snprintf(g->collide,sizeof(g->collide),"%s",what);
 char note[96];snprintf(note,sizeof(note),"Collision: %s. Shields damaged.",what);
 message(g,note);g->cue=SFX_HIT;
}
static void world_collision(Game *g,Vec3 previous){
 if(station_collision(g,previous))return;
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&g->bodies[i];float radius=b->radius+60;
  if(segment_distance(previous,g->pos,b->pos)>=radius)continue;
  if(b->type!=SUN){
   Vec3 away=norm(sub(previous,b->pos));if(length(away)<.01f)away=(Vec3){0,0,-1};
   g->pos=add(b->pos,mul(away,b->radius+900));g->speed=0;g->boost=0;g->approach=i;
   message(g,b->type==GAS?"Gas giant ahead. X to scan; Circle to reverse.":"Planet ahead. X to land; Circle to reverse.");
   g->cue=SFX_UI;
  }else {g->pos=add(b->pos,mul(norm(sub(previous,b->pos)),radius+10));g->speed=0;g->boost=0;g->collision=2;g->energy-=10;note_collision(g,b->name);}
 }
}
void game_tick(Game *g,float dt,float turn,float pitch,int throttle,int fire){
 if(dt<=0||dt>.1f)dt=1.0f/60;
 mission_timers(g,dt);
 if(g->police_grace>0)g->police_grace=fmaxf(0,g->police_grace-dt);
 if(g->police_stop)return;
 g->wanted[g->system]=g->legal;if(g->dead)g->explosion+=dt;
 g->time+=dt;g->message_time-=dt;if(g->message_time<0)g->message_time=0;g->attacked=fmaxf(0,g->attacked-dt);g->collision=fmaxf(0,g->collision-dt);g->heat_sink_cd=fmaxf(0,g->heat_sink_cd-dt);
 if(g->incoming_missile>0){
  if(g->boost&&g->speed>player_ships[g->ship].speed*4){g->incoming_missile=0;g->incoming_source=-1;message(g,"Incoming missile evaded.");}
  else if((g->upgrades&256)&&g->incoming_missile>1.7f&&g->incoming_missile-dt<=1.7f&&random_f(g)<.5f){g->incoming_missile=0;g->incoming_source=-1;message(g,(g->fit[FIT_UTIL]==17)?"Chaff broke the lock.":"ECM broke missile lock.");}
  else {g->incoming_missile-=dt;if(g->incoming_missile<=0){g->energy-=25;g->attacked=3;g->incoming_source=-1;g->cue=SFX_HIT;message(g,"Missile impact. Shields damaged.");}}
 }
 if(g->dock_stage){docking_tick(g,dt);return;}
 if(g->dead||g->docked||g->approach>=0)return;
 if(g->planet>=0){planet_tick(g,dt,turn,pitch,throttle);return;}
 float localturn=turn*cosf(g->roll)-pitch*sinf(g->roll),localpitch=turn*sinf(g->roll)+pitch*cosf(g->roll);g->yaw+=localturn*dt*1.5f;g->pitch+=localpitch*dt*1.5f;if(g->pitch>1.5f)g->pitch=1.5f;if(g->pitch<-1.5f)g->pitch=-1.5f;
 g->speed+=throttle*dt*(g->boost?4500:180);if(g->speed<0)g->speed=0;float maxspeed=player_ships[g->ship].speed*(g->boost?20.f:1.f)*(0.70f+0.15f*g->pip_eng);if(g->speed>maxspeed)g->speed=maxspeed;
 if(g->boost&&g->planet<0&&g->jump<=0){g->fuel=fmaxf(0,g->fuel-dt*.35f);if(g->fuel<=0){g->fuel=0;g->boost=0;if(g->message_time<=0)message(g,"Fuel empty. Boost cut.");}}
 Vec3 previous_pos=g->pos;g->pos=add(g->pos,mul(forward(g),g->speed*dt));world_collision(g,previous_pos);if(g->dead||g->dock_stage)return;campaign_flight(g,previous_pos);
 if(g->missile_time>0){g->missile_time-=dt;int i=g->missile_target;if(i<0||i>=NPC_COUNT||!g->npc[i].alive)g->missile_time=0;else {Vec3 d=norm(sub(g->npc[i].pos,g->missile_pos));g->missile_pos=add(g->missile_pos,mul(d,dt*2600));if(length(sub(g->npc[i].pos,g->missile_pos))<g->npc[i].radius+80){int mission_hit=0;for(int s=0;s<g->job_n;s++)if(g->jobs[s].dest==g->system&&g->jobs[s].type==MISSION_BOUNTY&&g->jobs[s].target==i)mission_hit=1;hit(g,i,140,1);g->missile_time=0;if(!mission_hit)message(g,"Missile hit confirmed.");}}}
 /* Heat builds from speed, boost and sun; cools when not boosting and clear of the star. */
 {
  float sun_d=length(sub(g->pos,g->bodies[0].pos)),safe=g->bodies[0].radius+5200.f;
  int near_sun=sun_d<safe;float speed_ratio=g->speed/fmaxf(1.f,player_ships[g->ship].speed);
  if(speed_ratio>1.15f)g->heat=fminf(100,g->heat+dt*(speed_ratio-1.f)*10.f);
  if(g->boost)g->heat=fminf(100,g->heat+dt*14.f);
  if(near_sun)g->heat=fminf(100,g->heat+dt*(6.f+(safe-sun_d)/safe*10.f));
  if(!g->boost&&!near_sun&&speed_ratio<=1.15f)g->heat=fmaxf(0,g->heat-dt*((g->upgrades&4)?38:22));
  else if(g->boost&&!near_sun)g->heat=fmaxf(0,g->heat-dt*4.f);
 if(g->heat>=100){
  g->heat=100;
  if(g->upgrades&16384){
   for(int s=0;s<FIT_SLOTS;s++)if(g->fit[s]==18)g->fit[s]=(uint8_t)FIT_EMPTY;
   fit_rebuild(g);g->heat=0;g->energy=40;g->dead=0;g->jump=0;g->boost=0;g->explosion=0;
   docking_complete(g);g->fuel=(float)player_ships[g->ship].range;
   message(g,"Escape pod fired. Recovered to hub — pod spent.");speak(g,VOICE_COMP,"Escape pod recovered. Module consumed.");
  }else {g->energy=0;g->dead=1;g->jump=0;g->boost=0;g->explosion=0;g->cue=SFX_DEATH;message(g,"Hull overheat. Ship destroyed. START to recover.");}
 }
  else if(g->heat>=90){g->boost=0;if(g->message_time<=0)message(g,"CRITICAL HEAT — boost locked. Break off and cool.");}
 }
 g->shot=fmaxf(0,g->shot-dt);g->energy=fminf(100,g->energy+dt*shield_regen_rate(g)*(0.50f+0.25f*g->pip_sys));
 if((g->upgrades&32768)&&g->attacked<=0)g->energy=fminf(100,g->energy+dt*2.f);
 if(g->passenger_dest>=0&&!g->docked&&g->message_time<=0&&((int)(g->time*2)&63)==0){static const char *chatter[]={"Passenger: Ever notice how Lave still smells like GalCop paint?","Passenger: Meridian sold us a 'clear lane' once. Cost a tender.","Passenger: If the animals stop singing, burn a different chart.","Passenger: Guild folks tip. Corporate folks invoice."};speak(g,VOICE_CONTACT,chatter[((int)g->time+(unsigned)g->passenger_dest)%4]);message(g,chatter[((int)g->time+(unsigned)g->passenger_dest)%4]);}
 {float scoop=fuel_scoop_rate(g);if(scoop>0&&length(sub(g->pos,g->bodies[0].pos))<g->bodies[0].radius+4000){g->fuel=fminf((float)player_ships[g->ship].range,g->fuel+dt*scoop);if(g->message_time<=0)message(g,g->heat>80?"Fuel scoop overheating. Break off.":"Fuel scoop filling the tank.");}}
 if(fire&&g->heat>=80&&(g->upgrades&2048)&&g->heat_sink_cd<=0){g->heat=fmaxf(0,g->heat-40);g->heat_sink_cd=30;message(g,"Heat sink dumped.");g->cue=SFX_UI;}
 if(fire&&g->heat>=80&&g->shot<=.001f&&g->message_time<=0)message(g,"Lasers overheated. Wait for the HEAT bar.");
 if(fire&&g->shot<=.001f&&g->heat<80){g->shot=.18f;g->heat+=12;g->shots++;g->cue=SFX_LASER;float closest=2200;int target=-1;
  Vec3 beam_end=add(g->pos,mul(forward(g),2200));
  for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive){
   NPC *n=&g->npc[i];Vec3 p=camera(g,n->pos);float distance=p.z;
   int onbeam=p.z>20&&sqrtf(p.x*p.x+p.y*p.y)<fmaxf(n->radius*.65f,p.z*.035f);
   if(n->freighter){float t=freight_intersection(n,g->pos,beam_end,4);onbeam=t>=0;distance=t*2200;}
   if(onbeam&&distance<closest){target=i;closest=distance;}
  }
  int rock=-1;
  for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&g->debris[i];if(!d->alive||!d->rock)continue;
   Vec3 p=camera(g,d->pos);float lateral=p.x*p.x+p.y*p.y,r=d->radius;
   if(p.z<=0||lateral>r*r)continue;
   float near=fmaxf(0,p.z-sqrtf(r*r-lateral));if(near<closest){rock=i;closest=near;}
  }
  if(rock>=0)mine_rock(g,DEBRIS_ID_MIN+rock);
  else if(target>=0)hit(g,target,(int)(laser_shot_damage(g)*(0.50f+0.25f*g->pip_wep)),1);
 }
 freight_update(g,dt);
 /* Capital hulls use the same oriented dimensions for rendering and collision.
  * Test every frame, including boosted movement across an entire hull. */
 for(int i=8;i<36;i+=12){NPC *n=&g->npc[i];if(!n->alive||!n->freighter)continue;
  float t=freight_intersection(n,previous_pos,g->pos,20);
  if(t>=0){Vec3 movement=sub(g->pos,previous_pos);
   if(t>0)g->pos=add(previous_pos,mul(movement,fmaxf(0,t-.01f)));
   else {Vec3 p=freight_local(n,g->pos),e=freight_extent(n);p.x=(p.x<0?-1:1)*(e.x+30);g->pos=freight_world(n,p);}
   g->speed=0;g->boost=0;if(g->collision<=0){g->energy-=5;n->health-=10;g->collision=2;note_collision(g,"FREIGHTER HULL");if(n->health<=0)ram_contact(g,i);}
  }
 }
 int ai_phase=g->ai_phase++;float frame_dt=dt;
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&g->npc[i];if(n->freighter)continue;if(ai_phase>=0&&((i+ai_phase)&1))continue;float dt=ai_phase<0?frame_dt:frame_dt*2;
  if(!n->alive){if(i>=36){if(i<36+wanted_level(g)*2){n->cooldown-=dt;if(n->cooldown<=0){n->alive=1;n->health=85;n->shield=60;n->pos=add(g->pos,(Vec3){(i%2?1:-1)*1200,150,(i%3-1)*900});n->target=-2;n->cooldown=2;}}continue;}n->cooldown-=dt;if(i<traffic_budget(g)&&n->cooldown<-18){n->health=npc_max_health(n);n->shield=npc_max_shield(n);n->alive=1;n->cooldown=1;n->flash=0;place_civilian(g,n,i);}continue;}n->cooldown-=dt;n->flash=fmaxf(0,n->flash-dt);if(n->flash<=0)n->shield=fminf(npc_max_shield(n),n->shield+dt*.8f);
  float best=1000+danger_rating(g,g->system)*700;int target=-1;Vec3 stn={0,0,3500},aim=stn;
  if(n->role==EXPLORERS){
   int lead=-1,wing=0;for(int j=0;j<36;j++)if(g->npc[j].alive&&g->npc[j].role==EXPLORERS){if(lead<0)lead=j;if(j<i)wing++;}
   if(lead>=0&&lead!=i){NPC *L=&g->npc[lead];Vec3 right=norm((Vec3){L->dir.z,0,-L->dir.x});aim=add(L->pos,add(mul(right,220.f*wing),mul(L->dir,-160.f*wing)));n->waypoint=L->waypoint;}
   else {int planet=n->waypoint;if(planet<1||planet>=BODY_COUNT)planet=1;Body *b=&g->bodies[planet];float a=g->time*.1f;aim=add(b->pos,(Vec3){cosf(a)*(b->radius+2300),350,sinf(a)*(b->radius+2300)});if(length(sub(n->pos,aim))<380)n->waypoint=planet%4+1;}
  }else if(n->role==PIRATES){Body *b=&g->bodies[n->waypoint>=1&&n->waypoint<BODY_COUNT?n->waypoint:2];float a=g->time*.07f+i;aim=add(b->pos,(Vec3){cosf(a)*(b->radius+2000),200,sinf(a)*(b->radius+2000)});}
  else if(n->role==LAW){float a=g->time*.12f+i;aim=(Vec3){cosf(a)*900,80,3200+sinf(a)*700};}
  else if(n->waypoint>=8){aim=add(n->pos,mul(n->dir,4000));if(length(n->pos)>24000){n->alive=0;n->cooldown=12+i;continue;}}
  else {aim=stn;if(length(sub(n->pos,stn))<520){n->waypoint=8;n->dir=norm((Vec3){n->pos.x,0,n->pos.z-3500});}}
  if(n->role==LAW||n->role==PIRATES)for(int j=0;j<NPC_COUNT;j++)if(i!=j&&g->npc[j].alive){NPC *o=&g->npc[j];if((n->role==LAW&&o->role==PIRATES)||(n->role==PIRATES&&(o->role==TRADERS||o->role==LAW))){float d=length(sub(o->pos,n->pos));if(d<best){target=j;best=d;aim=o->pos;}}}
  float pd=length(sub(g->pos,n->pos));if(((n->role==PIRATES&&g->system!=7)||(n->role==LAW&&g->legal>0)||((n->role==TRADERS||n->role==EXPLORERS)&&n->health<npc_max_health(n)-.5f))&&pd<best){target=-2;best=pd;aim=g->pos;}
  if(n->role==LAW&&pd<650&&g->jump<=0&&g->police_grace<=0){
   int dirty=cargo_contraband(g);
   if(g->legal>0){n->target=-2;police_begin(g,0);return;}
   if(dirty>0){n->target=-2;police_begin(g,1);return;}
  }
  if((n->role==TRADERS&&!n->freighter)||n->role==EXPLORERS){float danger=1100;int threat=-1;for(int j=0;j<NPC_COUNT;j++)if(g->npc[j].alive&&g->npc[j].role==PIRATES){float d=length(sub(n->pos,g->npc[j].pos));if(d<danger){danger=d;threat=j;}}if(threat>=0)aim=add(n->pos,mul(norm(sub(n->pos,g->npc[threat].pos)),2000));}
  n->target=target;Vec3 desired=norm(sub(aim,n->pos));for(int b=0;b<BODY_COUNT;b++){Vec3 toward=sub(g->bodies[b].pos,n->pos);float along=dot(toward,desired),radius=g->bodies[b].radius+n->radius+300;if(along>0&&along<radius+1800&&length(sub(toward,mul(desired,along)))<radius){Vec3 outward=norm(mul(toward,-1));desired=norm(add(desired,add(mul(outward,2),(Vec3){.15f,.4f,0})));}}for(int cap=8;cap<36;cap+=12){NPC *c=&g->npc[cap];if(!c->alive||!c->freighter)continue;Vec3 toward=sub(c->pos,n->pos);float along=dot(toward,desired),clear=c->radius+n->radius+220;if(along>0&&along<clear+1000&&length(sub(toward,mul(desired,along)))<clear)desired=norm(add(desired,add(mul(norm(mul(toward,-1)),2),(Vec3){.15f,.6f,0})));}
  n->dir=norm(add(mul(n->dir,1-dt*1.8f),mul(desired,dt*1.8f)));
  float speed=n->freighter?90:target==-1?(n->role==EXPLORERS?180:n->cruise):240;if(n->waypoint>=8)speed=n->cruise;if(target!=-1&&best<200)speed=40;
  n->pos=add(n->pos,mul(n->dir,dt*speed));
  if(length(sub(n->pos,stn))<190)n->pos=add(stn,mul(norm(sub(n->pos,stn)),200));
  if(target!=-1&&best<(n->freighter?2400:1300)&&dot(n->dir,desired)>.8f&&n->cooldown<=0){n->cooldown=1.5f-danger_rating(g,g->system)*.18f+random_f(g)*.5f;n->flash=.1f;
    if(target==-2){g->energy-=4;g->attacked=3.5f;g->cue=SFX_HIT;if(n->role==PIRATES&&best>1200&&g->incoming_missile<=0&&random_f(g)<.18f){g->incoming_missile=3.5f;g->incoming_source=i;g->cue=SFX_ALERT;}}else hit(g,target,n->freighter?20:8,0);
  }
  if(segment_distance(previous_pos,g->pos,n->pos)<n->radius+15){g->pos=add(n->pos,mul(norm(sub(previous_pos,n->pos)),n->radius+20));g->speed=0;g->boost=0;g->energy-=5;n->health-=10;g->collision=2;note_collision(g,meshes[n->mesh].name);if(n->health<=0)ram_contact(g,i);}
  for(int b=0;b<BODY_COUNT;b++)if(length(sub(n->pos,g->bodies[b].pos))<g->bodies[b].radius+n->radius+100)n->pos=add(g->bodies[b].pos,mul(norm(sub(n->pos,g->bodies[b].pos)),g->bodies[b].radius+n->radius+120));
 }
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
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&g->debris[i];if(!d->alive)continue;d->flash=fmaxf(0,d->flash-dt);d->life-=dt;if(d->life<=0){d->alive=0;continue;}d->pos=add(d->pos,mul(d->vel,dt));Vec3 stn={0,0,3500};if(length(sub(d->pos,stn))<200)d->pos=add(stn,mul(norm(sub(d->pos,stn)),210));}
 if(g->energy<=0){
  if(g->upgrades&16384){
   for(int s=0;s<FIT_SLOTS;s++)if(g->fit[s]==18)g->fit[s]=(uint8_t)FIT_EMPTY;
   fit_rebuild(g);g->energy=40;g->dead=0;g->jump=0;g->boost=0;g->explosion=0;
   docking_complete(g);g->fuel=(float)player_ships[g->ship].range;
   message(g,"Escape pod fired. Recovered to hub — pod spent.");speak(g,VOICE_COMP,"Escape pod recovered. Module consumed.");
  }else {g->dead=1;g->jump=0;g->cue=SFX_DEATH;message(g,"Ship destroyed. START for a new commander.");}
 }
 if(g->jump>0){g->jump-=dt;if(g->jump<=0){float spent=distance_ly(g,g->system,g->destination)*10;g->fuel-=spent;if(g->fuel<0)g->fuel=0;g->wanted[g->system]=g->legal;g->system=g->destination;g->legal=g->wanted[g->system];
  /* Arrive well short of the hub, from a system-unique bearing — always farther than a normal launch. */
  {unsigned h=sector_hash((g->system+1)*0xc2b2ae35u);float ang=g->system*1.918f+0.55f+((h&1023)*.001f);float dist=11000.f+(g->system%17)*780.f+((h>>10)%900);
   g->pos=(Vec3){sinf(ang)*dist*.62f,((int)((h>>18)%11)-5)*420.f,-dist*.78f};g->yaw=atan2f(-g->pos.x,STATION_Z-g->pos.z);g->pitch=0;g->speed=100;}
  travellers_advance(g,g->system);
  market(g);game_spawn(g);route_refresh_destination(g);g->cue=SFX_WARP;char note[80];snprintf(note,sizeof(note),"Hyperspace complete. Fuel %.1f LY left.",g->fuel*.1f);message(g,note);speak(g,VOICE_COMP,"Hyperspace complete. Station ahead.");}}
}
/* Versioned commander file. Load into a temporary struct; reject before mutation. */
typedef struct {uint32_t magic,version;int system,destination,credits,kills,legal,ship,laser,missiles;float fuel;int cargo[GOODS],stock[GOODS],price[GOODS];int contract,reward;float remaining;} Save;
#include "save-integrity.h"
#include "save-safety.h"
int save_game(Game *g,const char *path){if(!g->docked)return 0;
 Save s;memset(&s,0,sizeof(s));s.magic=0x41455053;s.version=13;s.system=g->system;s.destination=g->destination;s.credits=g->credits;s.kills=g->kills;s.legal=g->legal;s.ship=g->ship;s.laser=g->laser;s.missiles=g->missiles;s.fuel=g->fuel;s.contract=g->contract;s.reward=g->contract_reward;s.remaining=g->contract_time;
 memcpy(s.cargo,g->cargo,sizeof(s.cargo));memcpy(s.stock,g->stock,sizeof(s.stock));memcpy(s.price,g->price,sizeof(s.price));
 char tmp[256],bak[256];if(!save_side_path(tmp,sizeof(tmp),path,".tmp")||!save_side_path(bak,sizeof(bak),path,".bak")){message(g,"Save path is too long.");return 0;}FILE *f=fopen(tmp,"wb");if(!f){message(g,"Cannot create save file.");return 0;}int mission[8]={g->mission_type,g->mission_stage,g->mission_target,g->mission_item,g->mission_origin,g->mission_result,g->last_mission_type,g->last_mission_system};g->wanted[g->system]=g->legal;int extra[5]={g->discoveries,g->scanned_flora,g->scanned_fauna,g->scanned_minerals,g->scanned_anomalies};int jobhead[2]={g->job_n,g->job_sel};int ok=fwrite(&s,1,sizeof(s),f)==sizeof(s);if(ok)ok=fwrite(g->wanted,1,sizeof(g->wanted),f)==sizeof(g->wanted);if(ok)ok=fwrite(&g->upgrades,1,sizeof(g->upgrades),f)==sizeof(g->upgrades);if(ok)ok=fwrite(mission,1,sizeof(mission),f)==sizeof(mission);if(ok)ok=fwrite(extra,1,sizeof(extra),f)==sizeof(extra);if(ok)ok=fwrite(g->visited,1,sizeof(g->visited),f)==sizeof(g->visited);if(ok)ok=fwrite(jobhead,1,sizeof(jobhead),f)==sizeof(jobhead);if(ok)ok=fwrite(g->jobs,1,sizeof(g->jobs),f)==sizeof(g->jobs);int storypack[8]={g->story,g->story_flags,g->pip_sys,g->pip_eng,g->pip_wep,g->guild_chapter,g->guild_flags,g->guild_choice};if(ok)ok=fwrite(storypack,1,sizeof(storypack),f)==sizeof(storypack);uint32_t cp[5]={(uint32_t)g->campaign_stage,(uint32_t)g->campaign_choice,(uint32_t)g->campaign_flags,0,0};memcpy(&cp[3],&g->campaign_distance,4);memcpy(&cp[4],&g->campaign_fuel,4);for(int i=0;ok&&i<5;i++)ok=save_u32(f,cp[i]);uint32_t saga[10]={(uint32_t)g->saga_chapter,(uint32_t)g->saga_step,(uint32_t)g->saga_flags,(uint32_t)g->saga_choice,(uint32_t)g->saga_dest,(uint32_t)g->saga_start,(uint32_t)g->saga_trust[0],(uint32_t)g->saga_trust[1],(uint32_t)g->saga_trust[2],(uint32_t)g->saga_trust[3]};for(int i=0;ok&&i<10;i++)ok=save_u32(f,saga[i]);if(ok)ok=save_u32(f,(uint32_t)(int32_t)g->route_goal);if(ok)ok=save_u32(f,(uint32_t)(int32_t)g->passenger_dest);if(ok)ok=save_u32(f,(uint32_t)g->passenger_kind);if(ok)ok=save_u32(f,(uint32_t)g->passenger_pay);if(ok)ok=save_u32(f,(uint32_t)g->gift_flags);for(int ti=0;ok&&ti<12;ti++){uint32_t tp=(uint32_t)g->travellers[ti].sys|((uint32_t)g->travellers[ti].dest<<8)|((uint32_t)g->travellers[ti].flags<<16);ok=save_u32(f,tp);}if(ok){unsigned char fitpad[8];for(int s=0;s<6;s++)fitpad[s]=g->fit[s];fitpad[6]=fitpad[7]=0;ok=fwrite(fitpad,1,8,f)==8;}if(fflush(f)!=0)ok=0;if(fclose(f)!=0)ok=0;if(ok)ok=save_seal(tmp);if(ok)ok=save_commit(path,tmp,bak,rename);if(ok)g->cue=SFX_SELECT;message(g,ok?"Commander saved.":"Save failed.");return ok;
}
static int load_game_file(Game *g,const char *path){Save s;memset(&s,0,sizeof(s));FILE *f=fopen(path,"rb");if(!f)return 0;int warrants[256]={0},upgrades=0,mission[8]={MISSION_DELIVERY,0,-1,1,7,0,MISSION_DELIVERY,7}; int extra[5]={0,0,0,0,0};uint8_t visited[32]={0};int jobhead[2]={0,0};Job packed[MISSION_SLOTS];memset(packed,0,sizeof(packed));int storypack[8]={STORY_FREE,0xffff,2,2,4,0,0,0};uint32_t cp[5]={0},saga[10]={0},route_goal_u=0xffffffffu;float cp_distance=0,cp_fuel=0;int route_goal=-1;int ok=fread(&s,1,sizeof(s),f)==sizeof(s);if(ok&&s.version>=2)ok=fread(warrants,1,sizeof(warrants),f)==sizeof(warrants);if(ok&&s.version>=3)ok=fread(&upgrades,1,sizeof(upgrades),f)==sizeof(upgrades);if(ok&&s.version>=4)ok=fread(mission,1,sizeof(mission),f)==sizeof(mission);if(ok&&s.version>=5){ok=fread(extra,1,sizeof(extra),f)==sizeof(extra);if(ok)ok=fread(visited,1,sizeof(visited),f)==sizeof(visited);}if(ok&&s.version>=6){ok=fread(jobhead,1,sizeof(jobhead),f)==sizeof(jobhead);if(ok)ok=fread(packed,1,sizeof(packed),f)==sizeof(packed);}if(ok&&s.version>=7)ok=fread(storypack,1,sizeof(storypack),f)==sizeof(storypack);if(ok&&s.version>=8){for(int i=0;ok&&i<5;i++)ok=load_u32(f,&cp[i]);memcpy(&cp_distance,&cp[3],4);memcpy(&cp_fuel,&cp[4],4);if(cp[0]>6||cp[1]>2||cp[2]>7||!isfinite(cp_distance)||cp_distance<0||cp_distance>600||!isfinite(cp_fuel)||cp_fuel<0||cp_fuel>255)ok=0;}if(ok&&s.version>=9){for(int i=0;ok&&i<10;i++)ok=load_u32(f,&saga[i]);if(saga[0]>SAGA_COUNT||saga[1]>1||saga[3]>3||saga[4]>255)ok=0;}if(ok&&s.version>=10){ok=load_u32(f,&route_goal_u);if(ok){if(route_goal_u==0xffffffffu)route_goal=-1;else if(route_goal_u<=255)route_goal=(int)route_goal_u;else ok=0;}}uint32_t pax_dest_u=0xffffffffu,pax_kind=0,pax_pay=0,gift=0,trav_pack[12]={0};int pax_dest=-1,have_trav=0;if(ok&&s.version>=11){ok=load_u32(f,&pax_dest_u);if(ok)ok=load_u32(f,&pax_kind);if(ok)ok=load_u32(f,&pax_pay);if(ok)ok=load_u32(f,&gift);if(ok){if(pax_dest_u==0xffffffffu)pax_dest=-1;else if(pax_dest_u<=255)pax_dest=(int)pax_dest_u;else ok=0;}}if(ok&&s.version>=12){for(int ti=0;ok&&ti<12;ti++)ok=load_u32(f,&trav_pack[ti]);if(ok)have_trav=1;}unsigned char fitpad[8]={0};int have_fit=0;if(ok&&s.version>=13){ok=fread(fitpad,1,8,f)==8;if(ok){have_fit=1;for(int s=0;s<6;s++){int m=fitpad[s];if(m==0xff)continue;if(m<=0||m>=24||equip_slot_for(m)!=s){have_fit=0;break;}}}}if(ok)ok=save_verify_end(f,s.version);fclose(f);for(int i=0;i<256;i++)if(warrants[i]<0||warrants[i]>1000)ok=0;
 if(ok&&s.version>=6&&(jobhead[0]<0||jobhead[0]>MISSION_SLOTS||jobhead[1]<0||jobhead[1]>=MISSION_SLOTS))ok=0;
 if(ok&&s.version>=6)for(int i=0;i<jobhead[0];i++){Job *j=&packed[i];if(j->dest<0||j->dest>255||j->type<0||j->type>=MISSION_TYPES||j->stage<0||j->stage>1||j->target< -1||j->target>=NPC_COUNT||j->item<1||j->item>=BODY_COUNT||j->origin<0||j->origin>255||j->reward<0||j->reward>100000||!isfinite(j->time)||j->time<=0||j->time>3600)ok=0;}
 if(!ok||s.magic!=0x41455053||(s.version<1||s.version>13)||upgrades<0||upgrades>262143||(s.contract>=0&&(mission[0]<0||mission[0]>=MISSION_TYPES||mission[1]<0||mission[1]>1||mission[2]<-1||mission[2]>=NPC_COUNT||mission[3]<1||mission[3]>=BODY_COUNT||mission[4]<0||mission[4]>255))||(s.version>=4&&(mission[5]<-1||mission[5]>1||mission[6]<0||mission[6]>=MISSION_TYPES||mission[7]<0||mission[7]>255))||s.system<0||s.system>255||s.destination<0||s.destination>255||s.ship<0||s.ship>=player_ship_count||s.credits<0||s.credits>100000000||s.kills<0||s.legal<0||s.contract< -1||s.contract>255||!isfinite(s.fuel)||s.fuel<0||s.fuel>player_ships[s.ship].range||!isfinite(s.remaining)||s.remaining<0||s.reward<0||s.reward>100000||s.laser<0||s.laser>1||jobhead[0]<0||jobhead[0]>MISSION_SLOTS||jobhead[1]<0||jobhead[1]>=MISSION_SLOTS)return 0;
 int cargo=0;for(int i=0;i<GOODS;i++){if(s.cargo[i]<0||s.cargo[i]>100000||s.stock[i]<0||s.stock[i]>100000||s.price[i]<0||s.price[i]>1020)return 0;if(goods[i].unit=='t')cargo+=s.cargo[i];}if(cargo>player_ships[s.ship].capacity+((upgrades&64)?16:((upgrades&8)?8:0)))return 0;
 if(!g){return 1;}
 game_init(g);if(s.version>=9){g->saga_chapter=(int)saga[0];g->saga_step=(int)saga[1];g->saga_flags=(int)saga[2];g->saga_choice=(int)saga[3];g->saga_dest=(int)saga[4];g->saga_start=(int)saga[5];for(int i=0;i<4;i++)g->saga_trust[i]=(int)saga[6+i];}g->campaign_stage=(int)cp[0];g->campaign_choice=(int)cp[1];g->campaign_flags=(int)cp[2];g->campaign_distance=cp_distance;g->campaign_fuel=cp_fuel;g->system=s.system;g->destination=s.destination;g->route_goal=s.version>=10?route_goal:-1;if(s.version>=11){g->passenger_dest=pax_dest;g->passenger_kind=(int)pax_kind;g->passenger_pay=(int)pax_pay;g->gift_flags=(int)gift;}else {g->passenger_dest=-1;g->passenger_kind=0;g->passenger_pay=0;g->gift_flags=0;}if(have_trav){for(int ti=0;ti<12;ti++){g->travellers[ti].sys=(uint8_t)(trav_pack[ti]&255);g->travellers[ti].dest=(uint8_t)((trav_pack[ti]>>8)&255);g->travellers[ti].flags=(uint8_t)((trav_pack[ti]>>16)&255);g->travellers[ti].slot=-1;}}g->credits=s.credits;g->kills=s.kills;g->legal=s.legal;memcpy(g->wanted,warrants,sizeof(warrants));g->wanted[g->system]=s.legal;g->ship=s.ship;g->laser=s.laser;g->missiles=s.missiles;g->fuel=s.fuel;g->contract=s.contract;g->contract_reward=s.reward;g->contract_time=s.remaining;g->upgrades=upgrades;if(have_fit){int any=0;for(int s=0;s<6;s++){g->fit[s]=fitpad[s];if(fitpad[s]!=0xff)any=1;}if(any)fit_rebuild(g);else fit_synthesize(g);}else fit_synthesize(g);g->mission_type=mission[0];g->mission_stage=mission[1];g->mission_target=mission[2];g->mission_item=mission[3];g->mission_origin=mission[4];g->mission_result=mission[5];g->last_mission_type=mission[6];g->last_mission_system=mission[7];
 memcpy(g->cargo,s.cargo,sizeof(s.cargo));memcpy(g->stock,s.stock,sizeof(s.stock));memcpy(g->price,s.price,sizeof(s.price));g->discoveries=extra[0];g->scanned_flora=extra[1];g->scanned_fauna=extra[2];g->scanned_minerals=extra[3];g->scanned_anomalies=extra[4];memcpy(g->visited,visited,sizeof(visited));if(s.version>=6){g->job_n=jobhead[0];g->job_sel=jobhead[1];memcpy(g->jobs,packed,sizeof(g->jobs));jobs_sync(g);}if(s.version>=7){g->story=storypack[0];g->story_flags=storypack[1];g->pip_sys=storypack[2];g->pip_eng=storypack[3];g->pip_wep=storypack[4];g->guild_chapter=storypack[5]>=0&&storypack[5]<=4?storypack[5]:0;g->guild_flags=storypack[6]&31;g->guild_choice=storypack[7];if(g->story<0||g->story>STORY_FREE)g->story=STORY_FREE;if(g->pip_sys<0||g->pip_sys>4||g->pip_eng<0||g->pip_eng>4||g->pip_wep<0||g->pip_wep>4||g->pip_sys+g->pip_eng+g->pip_wep!=8){g->pip_sys=2;g->pip_eng=2;g->pip_wep=4;}}else story_complete(g);game_spawn(g);message(g,"Commander loaded.");g->voice_time=0;g->voice_who=0;g->voice[0]=0;if(g->cue==SFX_COMM)g->cue=SFX_SELECT;return 1;
}
#define CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);if(!ok)fails++;}while(0)
#include "reliability-tests.h"
#include "journey-tests.h"
#include "campaign-tests.h"
int game_tests(const char *path){FILE *f=fopen(path,"w");if(!f)return 1;int fails=0;Game g;game_init(&g);
 reliability_tests(f,&fails);
 journey_tests(f,&fails);campaign_tests(f,&fails);
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
 CHECK(g.npc[PIRATES].target==TRADERS||g.npc[PIRATES].target==LAW,"pirates target traffic and patrols");
 CHECK(g.npc[TRADERS].target==-1&&g.npc[EXPLORERS].target==-1,"civilians never select attack targets");
 g.npc[PIRATES].alive=0;g.legal=5;g.pos=g.npc[LAW].pos;game_tick(&g,.016f,0,0,0,0);game_tick(&g,.016f,0,0,0,0);
 CHECK(g.npc[LAW].target==-2,"Law pursues wanted player");
 game_init(&g);
 int credits=g.credits,stock=g.stock[0];CHECK(trade(&g,0,1)&&trade(&g,0,0)&&g.credits==credits&&g.stock[0]==stock,"market round trip conserves credits/stock");
 g.credits=0;CHECK(!trade(&g,0,1),"cannot buy without credits");
 g.credits=100000;g.cargo[0]=8;CHECK(!trade(&g,0,1),"cargo capacity enforced");g.cargo[0]=0;
 CHECK(!jump_start(&g),"cannot jump while docked");launch(&g);CHECK(!dock(&g),"cannot dock remotely");g.pos=(Vec3){0,0,2800};g.speed=300;CHECK(dock(&g)&&!g.docked,"nearby request starts guided approach without teleporting");for(int i=0;i<1200;i++){game_tick(&g,1.f/60,0,0,0,0);}CHECK(g.docked,"guided docking completes before services open");
 CHECK(save_game(&g,"test-commander.sav"),"save commander");Game loaded;CHECK(load_game(&loaded,"test-commander.sav")&&loaded.credits==g.credits,"load commander round trip");remove("test-commander.sav");
 game_init(&g);launch(&g);
 {int p=-1,c=-1;for(int i=0;i<36;i++)if(g.npc[i].alive&&g.npc[i].role==PIRATES&&p<0)p=i;for(int i=0;i<36;i++)if(g.npc[i].alive&&g.npc[i].role==LAW&&c<0)c=i;
  if(p>=0&&c>=0){g.npc[p].pos=(Vec3){400,80,2800};g.npc[c].pos=(Vec3){400,80,2500};g.npc[p].dir=(Vec3){0,0,-1};g.npc[c].dir=(Vec3){0,0,1};g.npc[p].target=c;g.npc[c].target=p;g.npc[p].health=20;g.npc[p].shield=0;g.npc[c].health=20;g.npc[c].shield=0;}}
 for(int i=0;i<6000;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.npc_kills>0,"independent NPC combat produces kills");
 game_init(&g);g.system=0;g.systems[g.system].government=7;game_spawn(&g);int safe=0;for(int i=0;i<NPC_COUNT;i++)safe+=g.npc[i].role==PIRATES;
 g.systems[g.system].government=0;game_spawn(&g);int dangerous=0;for(int i=0;i<NPC_COUNT;i++)dangerous+=g.npc[i].role==PIRATES;
 CHECK(danger_rating(&g,g.system)==5&&dangerous>safe,"higher danger spawns more pirates");
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
 g.boost=0;game_tick(&g,.016f,0,0,0,0);CHECK(g.speed<=player_ships[g.ship].speed,"boost release brakes to normal speed");
 game_init(&g);launch(&g);g.boost=0;g.heat=0;g.pip_eng=4;g.pip_sys=2;g.pip_wep=2;
 g.pos=add(g.bodies[0].pos,(Vec3){0,0,-(g.bodies[0].radius+20000.f)});
 g.speed=player_ships[g.ship].speed*(0.70f+0.15f*g.pip_eng);
 for(int i=0;i<180;i++)game_tick(&g,1.f/60,0,0,1,0);CHECK(g.heat>8,"over-speed cruise builds hull heat");
 float hot=g.heat;g.speed=0;g.boost=0;for(int i=0;i<120;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.heat<hot,"idle clear of the star vents heat");
 g.heat=0;g.pos=add(g.bodies[0].pos,(Vec3){0,0,g.bodies[0].radius+800});g.speed=0;
 for(int i=0;i<180;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.heat>10,"sun proximity cooks the hull");
 g.heat=99.5f;g.boost=1;game_tick(&g,.05f,0,0,0,0);CHECK(g.dead&&g.heat>=100&&!g.boost,"critical overheat destroys the ship and cuts boost");
 g.dead=0;g.energy=100;g.heat=92;g.boost=1;game_tick(&g,.016f,0,0,0,0);CHECK(!g.boost,"heat above ninety locks boost");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 Vec3 before={80,0,3100};g.pos=(Vec3){80,0,3900};g.energy=100;g.speed=400;g.roll=1.2f;world_collision(&g,before);CHECK(g.pos.z<3340&&g.energy<100,"swept collision blocks station tunnelling");
 CHECK(g.dead&&g.energy==0,"station impact destroys the player ship");g.dead=0;g.energy=100;
 Body *b=&g.bodies[1];before=add(b->pos,(Vec3){0,0,-b->radius-200});g.pos=add(b->pos,(Vec3){0,0,b->radius+200});float energy_before=g.energy;world_collision(&g,before);CHECK(length(sub(g.pos,b->pos))>b->radius&&g.approach==1,"planet boundary offers approach without impact");CHECK(g.energy==energy_before&&g.collision==0,"planet approach causes no collision damage");
 g.approach=-1;g.pos=add(b->pos,(Vec3){0,0,-b->radius-400});g.yaw=g.pitch=0;CHECK(approach_planet(&g,1)&&g.speed==0,"explicit planet approach stops ship and opens choice");
 Vec3 facing=forward(&g);turn_back(&g);CHECK(dot(facing,forward(&g))<-.999f&&g.approach==-1,"turn back reverses heading 180 degrees");
 game_init(&g);g.system=0;launch(&g);g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[2].alive=1;g.npc[2].pos=(Vec3){0,0,300};g.npc[2].dir=(Vec3){0,0,-1};g.npc[2].cooldown=0;game_tick(&g,.016f,0,0,0,0);CHECK(g.attacked>0,"incoming fire raises attack warning");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;int dest=-1;for(int i=0;i<256;i++)if(i!=g.system&&distance_ly(&g,g.system,i)*10<=g.fuel){dest=i;break;}g.destination=dest;float fuel=g.fuel,cost=distance_ly(&g,g.system,dest)*10;CHECK(jump_start(&g),"reachable warp starts");for(int i=0;i<310;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.system==dest&&fabsf(g.fuel-(fuel-cost))<.01f,"warp arrives in selected system and consumes fuel once");
 game_init(&g);add_crime(&g,15);CHECK(wanted_level(&g)==3&&g.wanted[g.system]==15,"crime raises the current system wanted level");g.credits=2000;g.police_stop=1;g.police_phase=0;int fine=police_fine(&g);CHECK(police_resolve(&g,0)&&g.credits==2000-fine&&g.legal==0,"paying police deducts exact fine and clears local warrant");
 add_crime(&g,10);g.credits=20;g.police_stop=1;g.police_phase=0;CHECK(!police_resolve(&g,0)&&g.police_stop,"unaffordable fine keeps police choice open");CHECK(police_resolve(&g,1)&&g.docked&&g.credits==0&&g.legal==0,"custody returns to station and takes affordable release bribe");
 game_init(&g);launch(&g);add_crime(&g,10);g.police_stop=1;g.police_phase=0;int before_run=g.legal;CHECK(police_escape(&g)&&!g.police_stop&&g.legal>before_run&&g.police_grace>0&&g.attacked>0,"running from law resumes flight, escalates warrant and starts pursuit");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,-20000};g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;add_crime(&g,10);game_tick(&g,.016f,0,0,0,0);int cops=0;for(int i=36;i<NPC_COUNT;i++)cops+=g.npc[i].alive;CHECK(cops==4,"wanted level two dispatches four extra police");
 g.npc[36].pos=add(g.pos,(Vec3){0,0,300});game_tick(&g,.016f,0,0,0,0);CHECK(g.police_stop&&g.police_phase==0,"approaching police opens warrant interception");Vec3 stopped=g.npc[36].pos;float frozen=g.time;game_tick(&g,.016f,1,1,1,1);CHECK(g.time==frozen&&length(sub(stopped,g.npc[36].pos))==0,"police encounter freezes world simulation");
 g.police_stop=0;g.police_phase=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;int origin=g.system;g.destination=dest;g.fuel=100;g.jump=.001f;game_tick(&g,.016f,0,0,0,0);CHECK(g.legal==0&&g.wanted[origin]==10,"warp leaves warrant behind in origin system");g.destination=origin;g.jump=.001f;game_tick(&g,.016f,0,0,0,0);CHECK(g.legal==10,"returning to original system restores its warrant");
 g.docked=1;g.fuel=10;CHECK(save_game(&g,"test-wanted.sav")&&load_game(&loaded,"test-wanted.sav")&&loaded.wanted[origin]==10,"wanted records survive save and load");remove("test-wanted.sav");
 /* Law rewrite: no heat on acquire; scan discovers contraband; desk clears when docked. */
 game_init(&g);g.credits=5000;int legal0=g.legal;CHECK(trade(&g,6,1)&&g.cargo[6]==1&&g.legal==legal0,"buying narcotics does not raise warrant until scanned");
 launch(&g);g.pos=(Vec3){0,0,-20000};g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[36].alive=1;g.npc[36].role=LAW;g.npc[36].pos=add(g.pos,(Vec3){0,0,300});g.police_grace=0;game_tick(&g,.016f,0,0,0,0);
 CHECK(g.police_stop&&g.police_phase==1,"dirty hold near Law opens a cargo scan");
 CHECK(police_scan_submit(&g)&&g.cargo[6]==0&&g.legal>0&&g.police_phase==0&&g.police_stop,"submit scan seizes narcotics and opens settle");
 g.credits=5000;CHECK(police_resolve(&g,0)&&g.legal==0&&!g.police_stop,"paying after a scan clears the warrant");
 game_init(&g);g.credits=5000;g.cargo[3]=2;launch(&g);g.pos=(Vec3){0,0,-5000};for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[6].alive=1;g.npc[6].role=LAW;g.npc[6].pos=add(g.pos,(Vec3){0,0,200});g.police_grace=0;game_tick(&g,.016f,0,0,0,0);
 CHECK(g.police_phase==1&&police_scan_refuse(&g)&&g.cargo[3]==0&&g.legal>=6&&g.police_phase==0,"refuse forces seizure and warrant");
 game_init(&g);g.docked=1;add_crime(&g,10);g.credits=5000;int desk=police_fine(&g);CHECK(police_pay_desk(&g)&&g.legal==0&&g.credits==5000-desk,"docked status desk pays and clears local warrant");
 game_init(&g);g.docked=1;g.credits=5000;g.cargo[0]=0;{int before=g.legal;/* smuggle accept via cargo only */g.cargo[6]=1;CHECK(g.legal==before&&cargo_contraband(&g)==1,"contraband cargo alone does not create a warrant");}
 game_init(&g);g.docked=1;g.fit[FIT_DEF]=7;fit_rebuild(&g);CHECK((g.upgrades&128)&&shield_regen_rate(&g)==4.5f,"military shield raises recharge to 4.5 per second");
 g.fit[FIT_WPN]=1;fit_rebuild(&g);CHECK(laser_shot_damage(&g)==24.f,"pulse laser deals 24 damage");g.fit[FIT_WPN]=2;fit_rebuild(&g);CHECK(laser_shot_damage(&g)==36.f,"beam laser deals 36 damage");
 g.fit[FIT_UTIL]=9;fit_rebuild(&g);launch(&g);g.heat=90;g.heat_sink_cd=0;game_tick(&g,.016f,0,0,0,1);CHECK(g.heat_sink_cd>0&&g.heat<80,"heat sink dumps when lasers fire while overheated");
 game_init(&g);g.fit[FIT_DEF]=16;fit_rebuild(&g);CHECK((g.upgrades&256)&&g.fit[FIT_DEF]==16,"ECM suite fits DEF and arms missile soft-kill");
 game_init(&g);g.docked=1;g.fit[FIT_HOLD]=11;g.fit[FIT_NAV]=4;fit_rebuild(&g);CHECK(save_game(&g,"test-fit.sav")&&load_game(&loaded,"test-fit.sav")&&loaded.fit[FIT_HOLD]==11&&loaded.fit[FIT_NAV]==4&&(loaded.upgrades&64)&&(loaded.upgrades&1),"save V13 persists fitted HOLD and NAV modules");remove("test-fit.sav");remove("test-fit.sav.bak");
 game_init(&g);g.upgrades=8|64;fit_synthesize(&g);CHECK(g.fit[FIT_HOLD]==11&&cargo_capacity(&g)==player_ships[g.ship].capacity+16,"V12 upgrades synthesize into fitted freight rack");
 CHECK(fit_value_valid(FIT_HOLD,23)&&fit_value_valid(FIT_UTIL,FIT_EMPTY),"V13 accepts valid catalog and empty slot values");CHECK(!fit_value_valid(FIT_WPN,23)&&!fit_value_valid(FIT_DEF,1),"V13 rejects invalid slot values");
 game_init(&g);g.roll=1.5707963f;Vec3 rolled=camera(&g,(Vec3){100,0,100});CHECK(fabsf(rolled.x)<.01f&&rolled.y< -99,"roll rotates camera and compass coordinates");
 Save legacy;memset(&legacy,0,sizeof(legacy));legacy.magic=0x41455053;legacy.version=1;legacy.system=7;legacy.destination=129;legacy.credits=1000;legacy.fuel=10;legacy.contract=-1;legacy.legal=5;FILE *legacyfile=fopen("test-legacy.sav","wb");if(legacyfile){fwrite(&legacy,1,sizeof(legacy),legacyfile);fclose(legacyfile);}CHECK(load_game(&loaded,"test-legacy.sav")&&loaded.credits==1000&&loaded.wanted[7]==5,"previous build saves import with a local warrant");remove("test-legacy.sav");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.pos=(Vec3){0,0,3200};g.speed=100;g.roll=station_angle(&g);for(int i=0;i<120&&!g.dock_stage;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.dock_stage==2&&!g.dead&&g.energy==100,"manual entry passes through actual station opening without damage");int sawWelcome=0;for(int i=0;i<400;i++){game_tick(&g,1.f/60,0,0,0,0);sawWelcome|=g.dock_stage==3;}CHECK(sawWelcome&&g.docked,"manual entry shows arrival then welcome before services");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,3400};g.roll=1.5707963f;g.speed=100;world_collision(&g,(Vec3){0,0,3300});CHECK(g.dead,"incorrect roll collides with rotating entrance rim");
 game_init(&g);launch(&g);g.time=62.831853f;g.roll=station_angle(&g);g.pos=(Vec3){0,40,3400};g.speed=100;world_collision(&g,(Vec3){0,40,3300});CHECK(g.dock_stage==2&&!g.dead,"rotated opening matches rotated collision coordinates");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,4300};CHECK(dock(&g),"communicator accepts request from behind station");int routeSafe=1;for(int i=0;i<1400&&!g.docked;i++){Vec3 prior=g.pos,hit;int stage=g.dock_stage;game_tick(&g,1.f/60,0,0,0,0);if(stage==1&&g.dock_stage==1&&station_intersection(&g,prior,g.pos,&hit))routeSafe=0;}CHECK(routeSafe&&g.docked,"guided route flies around hull before entering");
 game_init(&g);launch(&g);g.pos=hub_position(&g,1);CHECK(dock(&g)&&g.docked&&g.station_variant==1,"secondary relay accepts easy communicator docking");
 game_init(&g);launch(&g);g.pos=add(g.bodies[1].pos,(Vec3){0,0,-g.bodies[1].radius-800});g.yaw=g.pitch=0;CHECK(approach_planet(&g,1),"nearby facing planet can be approached");Vec3 nearPlanet=g.pos;turn_back(&g);CHECK(length(sub(g.pos,nearPlanet))==0&&dot(forward(&g),norm(sub(g.bodies[1].pos,g.pos)))<-.99f,"planet turn-back keeps position and faces away");CHECK(!approach_planet(&g,1),"planet cannot immediately re-prompt while facing away");
 game_init(&g);launch(&g);g.speed=0;for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[2].alive=1;g.npc[2].pos=(Vec3){0,0,300};g.npc[2].dir=(Vec3){0,0,-1};g.npc[2].cooldown=0;game_tick(&g,.016f,0,0,0,0);CHECK(danger_rating(&g,7)==1&&g.npc[2].target!=-2&&g.attacked==0,"Lave is peaceful and pirates never target player");
 game_init(&g);Body original=g.bodies[1];PlanetProfile profile=planet_profile_for_body(&original);system_bodies(&g);PlanetProfile profile_again=planet_profile_for_body(&g.bodies[1]);CHECK(original.seed==g.bodies[1].seed&&length(sub(original.pos,g.bodies[1].pos))==0,"system generation repeats deterministically");CHECK(planet_profile_valid(&profile)&&profile.seed==profile_again.seed&&profile.family==profile_again.family&&profile.terrain_style==profile_again.terrain_style&&profile.palette==profile_again.palette&&profile.prop_density==profile_again.prop_density&&profile.activity_density==profile_again.activity_density,"planet profile is deterministic and bounded");{int familyParity=1;for(int i=1;i<BODY_COUNT;i++){PlanetProfile bodyProfile=planet_profile_for_body(&g.bodies[i]);if(g.bodies[i].type==OCEAN)familyParity&=bodyProfile.family==PLANET_FAMILY_OCEAN;else if(g.bodies[i].type!=GAS)familyParity&=bodyProfile.family==(uint8_t)(PLANET_FAMILY_DESERT+(g.bodies[i].seed%4));}CHECK(familyParity,"planet profile preserves orbit-to-ground art family identity");}g.system=0;system_bodies(&g);CHECK(original.seed!=g.bodies[1].seed&&original.radius!=g.bodies[1].radius,"other systems have distinct planets");
 {float n0=planet_noise_sample(g.bodies[1].seed,137.f,-241.f),n1=planet_noise_sample(g.bodies[1].seed,137.f,-241.f);CHECK(planet_noise_bounded(n0)&&n0==n1,"planet noise is bounded and deterministic");}
 {StationProfile s0=station_profile_for(&g.systems[0],0,0),s0_again=station_profile_for(&g.systems[0],0,0),s1=station_profile_for(&g.systems[0],0,1),s2=station_profile_for(&g.systems[0],0,2);CHECK(station_profile_valid(&s0)&&s0.seed==s0_again.seed&&s0.architecture==s0_again.architecture&&s0.arrangement==s0_again.arrangement&&s0.landmark==s0_again.landmark&&s0.windows==s0_again.windows,"station profile is deterministic and bounded");CHECK(s1.hub==1&&s2.hub==2&&s1.seed!=s0.seed&&s2.seed!=s0.seed,"station profile distinguishes secondary hubs");CHECK(s0.seed==1178420178u&&s0.architecture==0&&s0.arrangement==2&&s0.landmark==7&&s1.seed==3093628131u&&s1.architecture==3&&s1.arrangement==4&&s1.landmark==1&&s2.seed==599002885u&&s2.architecture==1&&s2.arrangement==0&&s2.landmark==6,"station profile matches Node authoring selector fixture");}
 {const PlanetPackRecord *packed=planet_pack_find(0,1);int matches=!planet_pack_loaded;if(packed){PlanetProfile live=planet_profile_for_body(&g.bodies[1]);matches=packed->seed==live.seed&&packed->family==live.family&&packed->terrain_style==live.terrain_style&&packed->palette==live.palette&&packed->sea_level==live.sea_level&&packed->prop_density==live.prop_density&&packed->activity_density==live.activity_density;}CHECK(matches,"planet sidecar matches live deterministic profile when present");}
 {Body a=g.bodies[1];g.system=19;system_bodies(&g);Body b=g.bodies[1];CHECK(a.type!=b.type||a.color!=b.color||length(sub(a.pos,b.pos))>800,"distant systems diverge in planet type, colour or orbit");}
 game_init(&g);g.system=0;game_spawn(&g);Vec3 traffic0=g.npc[0].alive?g.npc[0].pos:(Vec3){99999,0,0};g.system=15;game_spawn(&g);CHECK(g.npc[0].alive&&length(sub(traffic0,g.npc[0].pos))>400,"ship traffic occupies a different layout in another system");
 game_init(&g);launch(&g);{int dest=-1;for(int i=0;i<256;i++)if(i!=g.system&&distance_ly(&g,g.system,i)*10<=g.fuel){dest=i;break;}g.destination=dest;CHECK(jump_start(&g),"warp starts for arrival-distance check");for(int i=0;i<310;i++)game_tick(&g,1.f/60,0,0,0,0);float hub=length(sub(g.pos,(Vec3){0,0,STATION_Z}));CHECK(g.system==dest&&hub>8500.f,"hyperspace drops the ship well outside the local hub");}
 game_init(&g);int large=0,models[64]={0},unique=0;for(int i=0;i<36;i++){large+=g.npc[i].freighter;models[g.npc[i].mesh]=1;}for(int i=0;i<64;i++)unique+=models[i];CHECK(large>=3&&unique>=9,"system traffic includes capital freighters and varied ship models");CHECK(g.npc[8].cruise<g.npc[0].cruise&&freight_extent(&g.npc[8]).z>g.npc[0].radius*2&&fabsf(g.npc[8].radius-length(freight_extent(&g.npc[8])))<.1f,"freighters use large shared hull dimensions and lower cruise speeds");
 g.system=0;g.systems[0].economy=0;int rich=mission_count(&g);g.systems[0].economy=2;CHECK(rich>mission_count(&g),"prosperous systems offer more mission jobs");
 game_init(&g);int types=0;for(int i=0;i<5;i++)types|=1<<mission_type_for_offer(&g,i);CHECK(types==31,"mission board rotates through five job types");CHECK(accept_mission(&g,0)&&g.contract>=0&&g.mission_type==MISSION_EXPLORATION,"exploration mission acceptance records objective type");
 int filled=1;for(int i=1;i<5;i++)filled+=accept_mission(&g,i)!=0;CHECK(filled==5&&g.job_n==5&&!accept_mission(&g,0),"mission log holds five jobs and rejects a sixth");
 g.job_sel=0;jobs_sync(&g);g.system=g.contract;g.docked=0;game_spawn(&g);Body *scan=&g.bodies[g.mission_item];g.pos=add(scan->pos,(Vec3){0,0,-scan->radius-500});g.yaw=g.pitch=0;int scan_cash=g.credits;CHECK(approach_planet(&g,g.mission_item)&&g.job_n==4&&g.credits>scan_cash,"approaching marked planet completes exploration scan");
 game_init(&g);g.contract=g.system;g.contract_time=300;g.mission_type=MISSION_BOUNTY;g.mission_stage=0;game_spawn(&g);int mark=g.mission_target,bounty_cash=g.credits;CHECK(mark>=0&&is_mission_target(&g,BODY_COUNT+1+mark),"pirate hunt assigns a marked hostile");hit(&g,mark,2000,1);CHECK(g.contract<0&&g.credits>bounty_cash&&g.mission_result==1&&g.last_mission_type==MISSION_BOUNTY,"destroying marked pirate completes hunt and records network result");
 game_init(&g);g.contract=g.system;g.mission_type=MISSION_RESCUE;g.mission_stage=0;g.contract_reward=2500;game_spawn(&g);mark=g.mission_target;g.pos=g.npc[mark].pos;CHECK(mission_interact(&g,BODY_COUNT+1+mark)&&g.mission_stage==1&&is_mission_target(&g,0),"rescue pickup moves mission marker to station");int rescue_cash=g.credits;docking_complete(&g);CHECK(g.contract<0&&g.credits==rescue_cash+2500,"returning rescued pilot completes mission");
 game_init(&g);g.system=0;launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[0].alive=1;g.npc[0].role=PIRATES;g.npc[0].pos=(Vec3){0,0,1200};g.npc[0].dir=(Vec3){0,0,1};g.npc[0].radius=30;g.npc[0].health=80;g.npc[0].shield=40;CHECK(fire_missile(&g,BODY_COUNT+1)&&g.missiles==0,"locked hostile accepts player missile launch");for(int i=0;i<100&&g.missile_time>0;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(!g.npc[0].alive&&g.missile_time<=0,"homing missile reaches and destroys hostile");
 game_init(&g);launch(&g);g.incoming_missile=3;g.incoming_source=2;g.boost=1;g.speed=player_ships[g.ship].speed*5;game_tick(&g,.016f,0,0,0,0);CHECK(g.incoming_missile<=0,"high-speed boost evades incoming missile");
 game_init(&g);launch(&g);g.contract=g.system;g.contract_time=.001f;g.mission_type=MISSION_RESCUE;game_tick(&g,.016f,0,0,0,0);CHECK(g.contract<0&&g.mission_result==-1&&g.last_mission_type==MISSION_RESCUE,"expired mission records failure for network feeds");
 game_init(&g);g.docked=1;CHECK(accept_mission(&g,0)&&accept_mission(&g,1)&&g.job_n==2, "two missions can be held at once");g.jobs[0].time=.001f;g.jobs[1].time=120;g.docked=0;g.pos=(Vec3){0,0,-8000};game_tick(&g,.016f,0,0,0,0);CHECK(g.job_n==1&&g.jobs[0].time>100,"mission timers expire independently");
 game_init(&g);g.docked=1;g.credits=20000;CHECK(accept_mission(&g,0)&&mission_offer_active(&g,0)&&!accept_mission(&g,0)&&g.job_n==1,"board marks accepted work as in progress and blocks a duplicate");CHECK(abandon_mission(&g,0)&&g.job_n==0,"abandon removes the focused in-progress job");
 game_init(&g);g.upgrades=8;CHECK(cargo_capacity(&g)==player_ships[g.ship].capacity+8,"expanded bay adds eight tonnes of cargo capacity");g.upgrades=43;fit_synthesize(&g);g.docked=1;g.contract=mission_destination(&g,0);g.mission_type=MISSION_SMUGGLING;g.mission_item=1;g.mission_origin=g.system;CHECK(save_game(&g,"test-upgrades.sav")&&load_game(&loaded,"test-upgrades.sav")&&loaded.upgrades==43&&loaded.mission_type==MISSION_SMUGGLING,"upgrades and mission survive version-four save and load");remove("test-upgrades.sav");
 game_init(&g);g.docked=1;g.discoveries=6;g.scanned_flora=2;g.scanned_anomalies=1;g.visited[2]|=4;CHECK(save_game(&g,"test-codex.sav")&&load_game(&loaded,"test-codex.sav")&&loaded.discoveries==6&&loaded.scanned_flora==2&&(loaded.visited[2]&4),"codex and visited systems survive version-six save");remove("test-codex.sav");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,-3000};CHECK(!dock(&g),"standard docking communicator has limited range");g.upgrades|=1;CHECK(dock(&g),"docking computer extends guided docking range");
 game_init(&g);launch(&g);int floating=0;for(int i=0;i<DEBRIS_COUNT;i++)floating+=g.debris[i].alive;CHECK(floating>=8,"systems spawn cargo canisters and wreckage");
 int loot=-1;for(int i=0;i<DEBRIS_COUNT;i++)if(g.debris[i].alive&&!g.debris[i].wreck&&!g.debris[i].rock){loot=i;break;}CHECK(loot>=0,"cargo filter has at least one canister");g.pos=g.debris[loot].pos;int hold=g.cargo[g.debris[loot].good];CHECK(salvage(&g,DEBRIS_ID_MIN+loot)&&!g.debris[loot].alive&&g.cargo[g.debris[loot].good]==hold+1,"close salvage collects a cargo canister");
 game_init(&g);g.system=0;launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.npc[0].alive=1;g.npc[0].role=TRADERS;g.npc[0].freighter=0;g.npc[0].pos=(Vec3){0,0,800};g.npc[0].dir=(Vec3){0,0,1};g.npc[0].health=10;g.npc[0].shield=0;int debris_before=0;for(int i=0;i<DEBRIS_COUNT;i++)debris_before+=g.debris[i].alive;hit(&g,0,200,1);int after=0,pods=0;for(int i=0;i<DEBRIS_COUNT;i++){after+=g.debris[i].alive;pods+=g.debris[i].alive&&!g.debris[i].wreck;}CHECK(!g.npc[0].alive&&after>debris_before&&pods>0,"destroyed ships release cargo and wreckage");
 g.cargo[0]=cargo_capacity(&g);int cash=g.credits,sold=-1;for(int i=0;i<DEBRIS_COUNT;i++)if(g.debris[i].alive&&!g.debris[i].wreck&&!g.debris[i].rock){sold=i;g.pos=g.debris[i].pos;break;}CHECK(sold>=0&&salvage(&g,DEBRIS_ID_MIN+sold)&&g.credits>cash,"full hold sells salvage for credits");
 CHECK(!fire_missile(&g,DEBRIS_ID_MIN),"missiles cannot lock cargo or wreckage");
 game_init(&g);launch(&g);g.pos=add(g.bodies[1].pos,(Vec3){0,0,-g.bodies[1].radius-500});g.yaw=g.pitch=0;Vec3 parked=g.pos;CHECK(approach_planet(&g,1)&&enter_planet(&g)&&g.planet==1&&g.approach<0,"approach X-path enters atmosphere flight");
 CHECK(g.planet==1&&g.pos.y>terrain_height(&g,g.pos.x,g.pos.z)+100,"atmosphere spawn sits above generated terrain");
 CHECK(!land_planet(&g),"cannot land while high and fast");
 Vec3 pad=surface_site(&g,1);g.pos=add(pad,(Vec3){0,20,0});g.speed=12;g.energy=100;CHECK(land_planet(&g)&&g.surface==1,"slow pad approach lands the ship");
 CHECK(eva_toggle(&g)&&g.surface==2,"commander can leave the landed ship");
 {int life=0;for(int i=0;i<LIFE_COUNT;i++)life+=g.life[i].alive;CHECK(life==LIFE_COUNT,"landed worlds spawn a full set of surface lifeforms");}
 {float farh=terrain_height(&g,pad.x+700,pad.z+700),nearh=terrain_height(&g,pad.x,pad.z);CHECK(nearh>23.f&&nearh<25.f&&fabsf(farh-nearh)>0.5f,"surface terrain stays flat on the pad and rises away from it");}
 CHECK(g.pos.y-terrain_height(&g,g.pos.x,g.pos.z)>=21.9f,"EVA camera remains above terrain at eye height");
 CHECK(survey_scan(&g)&&g.discoveries>0,"visor scan logs nearby surface life");
 float eva_floor=terrain_height(&g,g.pos.x,g.pos.z)+22;g.boost=1;game_tick(&g,.1f,0,0,0,0);float airborne=g.pos.y;g.boost=0;game_tick(&g,.016f,0,0,0,0);CHECK(airborne>eva_floor&&g.pos.y>eva_floor,"jetpack release transitions into a smooth fall");
 for(int i=0;i<180;i++)game_tick(&g,1.f/60,0,0,0,0);
 CHECK(fabsf(g.pos.y-(terrain_height(&g,g.pos.x,g.pos.z)+22))<.1f,"EVA landing restores safe eye clearance");
 g.pos.x+=200;CHECK(!eva_toggle(&g)&&g.surface==2,"cannot board from far away");
 g.pos=g.ship_pos;g.pos.y=terrain_height(&g,g.pos.x,g.pos.z)+22;CHECK(eva_toggle(&g)&&g.surface==1,"nearby commander boards the parked ship");
 CHECK(takeoff_planet(&g)&&g.surface==0,"takeoff returns to atmosphere flight");
 float ground=terrain_height(&g,g.pos.x,g.pos.z);g.pos.y=ground-20;g.speed=80;g.energy=100;game_tick(&g,.016f,0,0,0,0);CHECK(g.planet==1&&g.pos.y>=terrain_height(&g,g.pos.x,g.pos.z)+15,"surface collision keeps the ship above terrain");
 g.pos=(Vec3){2200,0,1800};g.pos.y=terrain_height(&g,g.pos.x,g.pos.z)+20;g.speed=10;
 int wet=terrain_is_water(&g,g.pos.x,g.pos.z);
 if(!wet){for(int i=0;i<24&&!wet;i++){float x=-2400+i*190,z=1600-i*70;if(terrain_is_water(&g,x,z)){g.pos=(Vec3){x,terrain_height(&g,x,z)+20,z};wet=1;}}}
 CHECK(wet&&!land_planet(&g),"cannot land on open water");
 CHECK(!fire_missile(&g,BODY_COUNT+1)&&g.planet==1,"missiles cannot launch in atmosphere");
 CHECK(!jump_start(&g)&&g.planet==1,"cannot warp from inside an atmosphere");
 leave_planet(&g);CHECK(g.planet<0&&length(sub(g.pos,parked))<1,"leaving atmosphere restores the parked orbit position");
 CHECK(!approach_planet(&g,1),"orbit restore faces away so the prompt does not reopen");
 game_init(&g);launch(&g);g.approach=3;CHECK(!enter_planet(&g)&&g.planet<0,"gas giants reject atmosphere entry");
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
 CHECK(g.story==STORY_WORLD,"moving pips advances the dirt chapter");
 {Game p=g;CHECK(pip_selected_move(&p,0,-1)&&p.pip_sys==2&&p.pip_eng==3,"taking a pip from SYS feeds the weakest bank");}
 story_event(&g,STORY_EV_WORLD);CHECK(g.story==STORY_ATLAS,"planet approach unlocks the Codex");
 story_on_open(&g,15);CHECK(g.story==STORY_FREE&&g.credits==3500,"codex completes the campaign with a Guild payout");
 CHECK(story_menu_ok(&g,3)&&story_menu_ok(&g,9),"free commander opens shipyard and debug");
 g.docked=1;CHECK(save_game(&g,"test-story.sav"),"save campaign state");
 Game campaign;CHECK(load_game(&campaign,"test-story.sav")&&campaign.story==STORY_FREE&&campaign.pip_sys==3&&campaign.pip_wep==3&&campaign.voice_time==0,"campaign and pips survive version-seven save");remove("test-story.sav");
 Save oldsix;memset(&oldsix,0,sizeof(oldsix));oldsix.magic=0x41455053;oldsix.version=6;oldsix.system=7;oldsix.destination=129;oldsix.credits=1000;oldsix.fuel=60;oldsix.contract=-1;FILE *six=fopen("test-v6.sav","wb");if(six){int zero_up=0,mission8[8]={0},extra5[5]={0};uint8_t vis[32]={0};int jobh[2]={0,0};Job emptyj[MISSION_SLOTS];memset(emptyj,0,sizeof(emptyj));fwrite(&oldsix,1,sizeof(oldsix),six);int warr[256]={0};fwrite(warr,1,sizeof(warr),six);fwrite(&zero_up,1,sizeof(zero_up),six);fwrite(mission8,1,sizeof(mission8),six);fwrite(extra5,1,sizeof(extra5),six);fwrite(vis,1,sizeof(vis),six);fwrite(jobh,1,sizeof(jobh),six);fwrite(emptyj,1,sizeof(emptyj),six);fclose(six);}CHECK(load_game(&campaign,"test-v6.sav")&&campaign.story==STORY_FREE&&campaign.pip_sys==2&&campaign.pip_wep==4,"version-six commanders import with the campaign complete");remove("test-v6.sav");
#include "freight-tests.h"
 fprintf(f,"RESULT %d failures\n",fails);fclose(f);return fails;
}
