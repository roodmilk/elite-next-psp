#ifndef ARCELITE_PLANET_PROFILE_H
#define ARCELITE_PLANET_PROFILE_H
#include <stdio.h>
#include <math.h>
/* Stable identity shared by the renderer, interactions and discovery archive. */
static inline unsigned field_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static inline unsigned field_seed(int sys,int body){return field_hash((sys+1)*911u+body*65537u);}
static inline int field_type(int sys,int body){
 if(body<0||body>=BODY_COUNT||!system_body_active_slot(sys,body))return SUN;
 return system_body_type_slot(sys,body);
}
#include "planet-regions.h"
typedef struct {unsigned seed;int biome,culture,weather,geology,job;} FieldProfile;
static inline FieldProfile field_profile(const Game *g,int sys,int body){
 unsigned h=field_seed(sys,body);int type=field_type(sys,body);const System *s=&g->systems[sys];
 FieldProfile p={h,type==GAS?5:type==OCEAN?0:1+(int)(h%4),s->government<2?3:s->tech>=10?2:s->economy>=4?1:0,(int)((h>>12)%4),(int)((h>>20)%4),(int)((h+s->economy+s->government)%3)};
 if(opening_sol(g)&&sys==7&&body==1){p.seed=g->bodies[1].seed;p.biome=4;p.culture=1;p.weather=0;p.geology=0;p.job=0;return p;}
 p.biome=world_legacy_biome(field_world(g,sys,body)->type);
 if(sys==7&&body==1){p.job=0;p.biome=4;}
 if(sys==7&&body==2)p.weather=2;
 if(sys==7&&body==3)p.weather=1;
 if(sys==7&&body==4)p.weather=1;
 return p;
}
static inline const char *field_culture(int c){static const char *s[]={"INDUSTRIAL OUTPOST","AGRICULTURAL CO-OP","RESEARCH ENCLAVE","FRONTIER CAMP"};return s[c&3];}
static inline const char *field_lave_region(int body){static const char *s[]={"","ISLAND / OBSERVATORY RIDGE","MOSSWOOD EXPEDITION","CLOUD SEA / SKYPORT","GLACIAL SURVEY RANGE"};return s[body>0&&body<5?body:0];}
static inline const char *field_weather(int w){static const char *s[]={"CLEAR / LONG SHADOWS","HIGHEST CLOUDS","DRIFTING SPORES","ELECTRIC HAZE"};return s[w&3];}
static inline int field_species_kind(int sys,int body,int slot){unsigned h=field_hash(field_seed(sys,body)+slot*131u);return field_type(sys,body)==OCEAN?(slot%5==0?LIFE_MINERAL:(slot&1?LIFE_FLORA:LIFE_FAUNA)):(int)(h%3);}
static inline unsigned field_species_seed(int sys,int body,int slot){return field_hash(field_seed(sys,body)^((slot+1)*0x9e3779b9u));}
static inline void field_species_name(int sys,int body,int slot,char *out,int cap){
 static const char *prefix[]={"AMBER","GLASS","SILVER","VELVET","GLOW","CROWN","RIBBON","DUSK","CORAL","COPPER","IVORY","VIOLET"};
 static const char *noun[3][8]={{"FERN","BLOOM","MOSS","VINE","FAN","SPORE","REED","LANTERN"},{"MOTH","STRIDER","HOPPER","RAY","CRAWLER","GLIDER","GRAZER","SKIPPER"},{"QUARTZ","BASALT","GEODE","SILICA","NICKEL","LATTICE","CRYSTAL","ORE"}};
 unsigned h=field_species_seed(sys,body,slot);snprintf(out,cap,"%s %s",prefix[h%12],noun[field_species_kind(sys,body,slot)][(h>>8)%8]);
}
static inline const char *field_species_trait(int sys,int body,int slot){
 static const char *plant[]={"Folds its fronds at dusk.","Stores dew in hollow stems.","Pulses softly before a storm.","Spreads through windborne spores."};
 static const char *animal[]={"Forages in a slow circling gait.","Flashes its crest to communicate.","Hops between mineral-rich patches.","Glides on warm updrafts."};
 static const char *mineral[]={"Layered crystals trap ancient dust.","Conductive veins carry faint charge.","Its facets glow under ultraviolet.","Dense grains preserve magnetic traces."};
 int kind=field_species_kind(sys,body,slot),i=(field_species_seed(sys,body,slot)>>16)&3;
 if(kind==LIFE_FAUNA){int family=(field_species_seed(sys,body,slot)>>8)%8;return family==0||family==3||family==5?"Rests between flights; startled by nearby movement.":family==2||family==7?"Forages in short hops; retreats from running visitors.":"Pauses to feed, wanders locally, and flees approaching rovers.";}
 if(sys==7&&body==3&&kind==LIFE_FLORA)return "Cultivated in a skyport planter above the cloud sea.";
 if(sys==7&&body==4&&kind==LIFE_FLORA)return "Shelters low against the snow; holds moisture through the long cold.";
 return kind==LIFE_FLORA?plant[i]:mineral[i];
}
enum { FIELD_CACHE,FIELD_RUINS,FIELD_DISH,FIELD_RESCUE,FIELD_GARDEN,FIELD_FOSSIL,FIELD_VENT,FIELD_WRECK,FIELD_MIGRATION,FIELD_CRYSTAL,FIELD_ARCHIVE,FIELD_WEATHER };
static inline int field_site_kind(const Game *g,int sys,int body,int id){
 if(sys==7&&body==1&&id==2)return FIELD_DISH;
 FieldProfile p=field_profile(g,sys,body);if(id==0)return -1;if(id==1)return FIELD_WEATHER;
 int rank=id<6?id-2:id-3;return (int)((p.seed%12+p.culture*3+rank*5)%12);
}
/* Shared deterministic vegetation lattice. Rendering and EVA collision use
 * these exact cells, so Lave's large trees cannot be walked through. */
static inline int field_prop_cell(int sys,int body,unsigned seed,int cx,int cz,float *x,float *z,unsigned *hash){
 unsigned h=field_hash(seed^(unsigned)cx*73856093u^(unsigned)cz*19349663u);int lush=sys==7&&(body==2||body==4);
 /* Convert before adding unsigned hash jitter: negative lattice cells must
  * not wrap to 4 billion metres. Drawing and collision share these positions. */
 int tile=lush?70:90,spread=lush?42:60;*x=cx*(float)tile+15+(h%spread);*z=cz*(float)tile+15+((h>>8)%spread);*hash=h;
 float px,pz;world_pad(seed,&px,&pz);const WorldProfile *w=world_profile(seed,field_type(sys,body));return world_prop_kind(w,world_sample(w,*x-px,*z-pz),h);
}
static inline int field_lave_tree_cell(unsigned seed,int cx,int cz,float *x,float *z,unsigned *hash){
 unsigned h=field_hash(seed^(unsigned)cx*73856093u^(unsigned)cz*19349663u);
 if(hash)*hash=h;
 if(x)*x=cx*70.f+8.f+(h%54);
 if(z)*z=cz*70.f+8.f+((h>>8)%54);
 if(seed==field_hash(2050u+65537u))return (h&15)<4;
 float px,pz;world_pad(seed,&px,&pz);float tx=cx*70.f+8.f+(h%54),tz=cz*70.f+8.f+((h>>8)%54);const WorldProfile *w=world_profile(seed,OCEAN);WorldRegion r=world_sample(w,tx-px,tz-pz);
 int chance=r.a%6==1?55:r.a%6==5?22:r.a%6==4?9:5;return (h&255)<chance*256/100;
}
static inline float field_lave_tree_height(unsigned h){return 52.f+((h>>9)%85);}
static inline float field_tree_trunk_radius(unsigned h,int rover){return 5.f+(h%4)+(rover?14.f:5.f);}
/* Low open terraces are traversable floor, not invisible full-box walls. */
static inline int field_port_solid(int id){return id<2||id>4;}
static inline float field_traffic_lift(const Game *g,int lane){
 float t=fmodf(g->world_clock+lane*63+(g->bodies[g->planet].seed%40),150);
 return t<40?(40-t)*(40-t)*.8f:t<90?0:(t-90)*(t-90)*.8f;
}
static inline unsigned field_site_bit(int id){return id>=1&&id<=5?1u<<id:id>=7&&id<=9?1u<<(id+9):0;}
static inline int field_job_reward(const Game *g,int sys,int body){return sys==7&&body==1?900:600+(field_profile(g,sys,body).seed%9)*100;}
static inline int field_species_logged(const Game *g,int sys,int body,int slot){return (g->surface_progress[sys][body]>>(8+slot))&1;}
static inline float field_local_hour(const Game *g,int sys,int body){if(opening_sol(g)&&sys==7&&body==1)return 10.f+fmodf(g->world_clock,1200.f)*.002f;unsigned h=field_seed(sys,body);static const int days[]={600,900,1200,1800};float hour=g->world_clock*24.f/days[(h>>24)&3]+(sys==7&&body==1?10.f:(h%2400)*.01f);while(hour>=24)hour-=24;return hour;}
/* One geometry source for visible port structures and their collision boxes. */
typedef struct {float x,z,w,d,height;} FieldBuilding;
/* Shared airport dimensions: renderer, collision, foliage, cloud decks and maps. */
#define FIELD_OBSERVATORY_DISTANCE 4200.f
#define FIELD_PORT_HALF 420.f
#define FIELD_PORT_CLEAR 460.f
#define FIELD_PORT_FLAT 600.f
#define FIELD_PLAYER_PAD 125.f
#define FIELD_PLAYER_PAD_DEPTH 220.f
#define FIELD_TRAFFIC_X 335.f
#define FIELD_TRAFFIC_Z 30.f
#define FIELD_TRAFFIC_PAD 85.f
#define FIELD_GARAGE_X (-192.f)
static const FieldBuilding field_port_buildings[]={
 {420,-500,210,110,100},{630,-200,60,65,300},{-460,-500,230,150,175},
 {460,500,230,150,195},{-460,500,230,150,195},
 {-630,-250,55,70,85},{70,670,95,35,90}
};
enum { FIELD_PORT_BUILDINGS=7 };
static inline float field_port_half(const Game *g){return FIELD_PORT_HALF;}
static inline float field_port_clear(const Game *g){return field_port_half(g)+40;}
/* The apron must include the neighbouring 160m interpolation vertices. */
static inline float field_port_flat(const Game *g){return FIELD_PORT_FLAT;}
static inline FieldBuilding field_port_building(const Game *g,int id){
 FieldBuilding b=field_port_buildings[id];
 float scale=g->system==7&&g->planet==1?.50f:.48f+(field_seed(g->system,g->planet)%4)*.015f;
 b.x*=scale;b.z*=scale;b.w*=scale;b.d*=scale;
 if(id==0)b.x+=36.f;
 /* Keep cargo handling beside the terraces, never across the ridge trail. */
 if(id==6)b.x=-360.f*scale;
 if(id>=2&&id<=4){b.x=(b.x<0?-500.f:500.f)*scale;b.height=.6f;b.w*=.80f;b.d*=.76f;}
 else if(id==0)b.height=122.f+(field_seed(g->system,g->planet)%4)*12.f;
 else if(id==1){b.w*=.75f;b.d*=.75f;b.height*=.72f*scale;}
 else if(id!=0)b.height*=.68f*scale;
 return b;
}
static inline int field_cave_wall(float dx,float dz,FieldBuilding b,float margin){
 return (fabsf(dx)>b.w*.70f-margin&&fabsf(dx)<b.w+margin&&fabsf(dz)<b.d+margin)||
        (fabsf(dx)<b.w+margin&&dz>b.d*.70f-margin&&dz<b.d+margin);
}
static inline int field_nav_number(int id){return id==6?0:id<6?id+1:id;}
/* Open-front rover garage: walls and roof share these exact bounds. */
static const FieldBuilding field_garage_walls[]={{FIELD_GARAGE_X-43,-20,3,44,48},{FIELD_GARAGE_X+43,-20,3,44,48},{FIELD_GARAGE_X,-61,40,3,48}};
static inline FieldBuilding field_site_building(const Game *g,int id){
 int k=field_site_kind(g,g->system,g->planet,id);
 static const FieldBuilding shape[]={
  {0,0,58,44,68},   /* supply cache and loading canopy */
  {0,0,70,62,190},  /* monumental ruins */
  {0,0,72,72,215},  /* observatory tower and crown dome */
  {0,0,68,54,92},   /* rescue landing site */
  {0,0,110,110,175},/* seed-garden biodome */
  {0,0,82,60,30},   /* fossil excavation */
  {0,0,70,64,126},  /* thermal stacks */
  {0,0,92,68,52},   /* crashed survey drone */
  {0,0,58,58,152},  /* migration lookout */
  {0,0,74,74,158},  /* crystal spires */
  {0,0,88,66,62},   /* buried archive vault */
  {0,0,104,70,178}  /* weather/electrical array */
 };
 FieldBuilding b=shape[k<0||k>FIELD_WEATHER?FIELD_WEATHER:k];
 if(g->system==7&&g->planet==1&&id==2)b=(FieldBuilding){0,0,125,105,210};
 if(id==1){
  int job=field_profile(g,g->system,g->planet).job;
  if(job==0)b=(FieldBuilding){0,0,88,58,150};
  else if(job==1)b=(FieldBuilding){0,0,68,68,105};
  else b=(FieldBuilding){0,0,72,54,116};
 }
 return b;
}
static inline const char *field_site_sign(const Game *g,int id){
 static const char *s[]={"SUPPLIES","RUINS","OBSERVATORY","RESCUE","GARDEN","FOSSILS","THERMAL","SALVAGE","MIGRATION","CRYSTALS","ARCHIVE","WEATHER"};
 if(id==1){static const char *jobs[]={"RELAY","BIOLOGY","UPLINK"};return jobs[field_profile(g,g->system,g->planet).job];}
 return s[field_site_kind(g,g->system,g->planet,id)];
}
#endif
