#ifndef ELITE_PLANET_REGIONS_H
#define ELITE_PLANET_REGIONS_H
/* Generation v1: IDs and salts are persistent world geography. Never renumber.
 * Derived from existing body seeds; no new destinations or save fields.
 * Ground biome ID = type*6+local; gas IDs start at 108 (four per family). */
#define WORLD_GENERATION_VERSION 1
enum { WORLD_TEMPERATE,WORLD_OVERGROWN,WORLD_OCEAN,WORLD_ARCHIPELAGO,
 WORLD_DESERT,WORLD_SALT,WORLD_FROZEN,WORLD_TUNDRA,WORLD_BARREN,WORLD_SCORCHED,
 WORLD_VOLCANIC,WORLD_HELL,WORLD_TOXIC,WORLD_FUNGAL,WORLD_SWAMP,WORLD_CRYSTAL,
 WORLD_CARBON,WORLD_METAL,WORLD_BANDED,WORLD_STORM,WORLD_ICE_GIANT,WORLD_HOT_GIANT,
 WORLD_TYPE_COUNT };
#define WORLD_RGB(r,g,b) ((unsigned)(r)|((unsigned)(g)<<8)|((unsigned)(b)<<16))
typedef struct {const char *name;int weight,kit;unsigned ground,accent,water;float relief;int density,trees;} WorldType;
static const WorldType world_types[WORLD_TYPE_COUNT]={
 {"TEMPERATE",140,4,WORLD_RGB(100,145,65),WORLD_RGB(45,98,50),WORLD_RGB(44,123,165),360,85,35},
 {"OVERGROWN",50,4,WORLD_RGB(54,100,58),WORLD_RGB(111,147,57),WORLD_RGB(56,106,83),420,95,72},
 {"OCEAN",80,3,WORLD_RGB(185,175,110),WORLD_RGB(65,133,94),WORLD_RGB(31,140,185),210,55,18},
 {"ARCHIPELAGO",60,3,WORLD_RGB(201,186,127),WORLD_RGB(61,141,91),WORLD_RGB(37,157,174),270,80,32},
 {"DESERT",110,0,WORLD_RGB(202,150,83),WORLD_RGB(149,85,52),WORLD_RGB(69,146,132),310,36,6},
 {"SALT",30,0,WORLD_RGB(217,207,182),WORLD_RGB(178,153,185),WORLD_RGB(118,171,161),160,22,0},
 {"FROZEN",100,1,WORLD_RGB(194,222,232),WORLD_RGB(91,159,194),WORLD_RGB(57,111,154),500,40,0},
 {"TUNDRA",70,1,WORLD_RGB(136,154,119),WORLD_RGB(196,216,212),WORLD_RGB(64,126,151),330,57,23},
 {"BARREN",130,2,WORLD_RGB(137,126,113),WORLD_RGB(85,82,83),WORLD_RGB(80,91,106),420,35,0},
 {"SCORCHED",50,0,WORLD_RGB(155,102,68),WORLD_RGB(66,52,50),WORLD_RGB(98,77,63),370,25,0},
 {"VOLCANIC",65,2,WORLD_RGB(91,74,72),WORLD_RGB(172,78,47),WORLD_RGB(153,89,41),650,45,9},
 {"HELL",15,2,WORLD_RGB(62,48,55),WORLD_RGB(199,79,34),WORLD_RGB(169,103,43),780,30,5},
 {"TOXIC",35,4,WORLD_RGB(123,146,68),WORLD_RGB(151,92,162),WORLD_RGB(129,175,60),270,60,10},
 {"FUNGAL",20,4,WORLD_RGB(114,88,139),WORLD_RGB(81,157,159),WORLD_RGB(93,103,154),290,90,25},
 {"SWAMP",40,4,WORLD_RGB(77,107,73),WORLD_RGB(142,129,81),WORLD_RGB(74,112,95),160,90,55},
 {"CRYSTAL",8,1,WORLD_RGB(115,142,181),WORLD_RGB(193,136,213),WORLD_RGB(75,157,182),550,46,0},
 {"CARBON",10,2,WORLD_RGB(70,74,83),WORLD_RGB(118,103,123),WORLD_RGB(58,81,94),450,29,0},
 {"METAL-RICH",25,0,WORLD_RGB(168,100,68),WORLD_RGB(100,154,141),WORLD_RGB(81,119,113),590,39,0},
 {"BANDED GIANT",0,5,WORLD_RGB(149,172,187),WORLD_RGB(199,176,133),WORLD_RGB(121,145,182),0,0,0},
 {"STORM GIANT",0,5,WORLD_RGB(118,139,175),WORLD_RGB(175,153,193),WORLD_RGB(85,110,150),0,0,0},
 {"ICE GIANT",0,5,WORLD_RGB(127,194,206),WORLD_RGB(191,216,226),WORLD_RGB(78,141,175),0,0,0},
 {"HOT GIANT",0,5,WORLD_RGB(184,119,91),WORLD_RGB(226,176,106),WORLD_RGB(139,88,100),0,0,0}
};
static const char *const world_biomes[124]={
 "MEADOWS","DECIDUOUS FOREST","WETLANDS","MOUNTAIN RANGE","COASTAL CLIFFS","RIVER VALLEY",
 "GIANT FOREST","ROOT LABYRINTH","FERN BASIN","CANOPY CLEARING","TANGLED WETLAND","VINE CLIFFS",
 "VOLCANIC ISLAND","CORAL SHORE","TIDAL FLATS","ROCKY ISLETS","STORM COAST","FLOATING BEDS",
 "ISLAND FOREST","SHELTERED LAGOON","MANGROVES","WHITE BEACH","ISLAND HIGHLANDS","EXPOSED REEF",
 "DUNE SEA","ROCKY MESA","DRY RIVERBED","OASIS VALLEY","STONE ARCH REGION","BURIED BASIN",
 "SALT FLATS","BRINE LAKES","MINERAL TERRACES","SALT SPIRE REGION","CRUST CAVERN REGION","CRYSTAL SHORE",
 "GLACIER","FROZEN OCEAN","ICE CANYON","SNOWFIELD","BLUE-ICE CAVE REGION","GEOTHERMAL POCKET",
 "MOSS PLAINS","SPARSE CONIFERS","PERMAFROST HILLS","GLACIAL VALLEY","THAW LAKES","SNOWY MOUNTAINS",
 "CRATER FIELD","DUST PLAINS","BOULDER FIELD","IMPACT MOUNTAINS","MINERAL RIDGE","LAVA TUBE REGION",
 "BAKED PLAINS","GLASS FIELD","FRACTURED CRUST","BLACKENED MOUNTAINS","IMPACT BASIN","SHADOW VALLEY",
 "LAVA FIELD","BASALT CLIFFS","ASH PLAINS","SULPHUR SPRINGS","CALDERA","COOLED LAVA FOREST",
 "BURNING WASTES","CHASM REGION","FURNACE CRATER","OBSIDIAN FOREST","SULPHUR MARSH","EMBER DUNES",
 "ACID MARSH","CHEMICAL LAKES","CORROSIVE FLATS","POISONOUS SCRUB","VENT FIELD","MINERAL CLIFFS",
 "MUSHROOM FOREST","SPORE MEADOW","MYCELIUM PLAINS","GLOWING GROVE","FUNGAL WETLAND","HOLLOW GROWTH REGION",
 "FLOODED FOREST","REED MARSH","PEAT ISLANDS","MUDDY CHANNELS","MIST BASIN","GIANT-ROOT WETLAND",
 "CRYSTAL FOREST REGION","GEODE VALLEY","GLASS DUNES","MINERAL TERRACES","RESONANT CAVE REGION","SHATTERED CRYSTALS",
 "GRAPHITE PLAINS","SOOT DUNES","DARK CRAGS","HYDROCARBON POOLS","DIAMOND RIDGE","FRACTURED CAVERN REGION",
 "IRON MOUNTAINS","COPPER BASIN","MAGNETIC SPIRE REGION","OXIDISED PLAINS","METALLIC SCREE","ORE CANYON",
 "CALM CLOUD SHELF","TURBULENT BELT","CLOUD TOWERS","ATMOSPHERIC GAP",
 "LIGHTNING FRONT","CYCLONE EYE","STORM WALL","CALM POCKET",
 "METHANE HAZE","CRYSTAL CLOUDS","DARK STORM BAND","THIN UPPER AIR",
 "GLOWING CLOUD BANK","THERMAL UPDRAFT","ASH HAZE","CONVECTION FRONT"
};
/* Terrain, cover and water recipes for six compatible local biome IDs.
 * Caves/arches/roots are region identities only until specialist geometry is added. */
static const float world_relief[18][6]={
 {.40f,.8f,.3f,1.9f,1.5f,.6f},{1.1f,.85f,.45f,.35f,.25f,1.6f},
 {1.2f,.3f,.15f,.95f,1.4f,.1f},{.85f,.25f,.3f,.2f,1.6f,.5f},
 {.7f,1.6f,.3f,.45f,1.5f,.3f},{.15f,.2f,.85f,1.3f,.65f,.5f},
 {1.1f,.2f,1.8f,.35f,1.2f,.5f},{.35f,.8f,1.3f,.65f,.3f,1.9f},
 {1.1f,.25f,.65f,1.8f,1.4f,.8f},{.25f,.45f,1.2f,1.9f,.5f,.7f},
 {.45f,1.7f,.3f,.45f,1.25f,.8f},{.7f,1.8f,1.1f,1.3f,.3f,.55f},
 {.25f,.2f,.3f,.65f,.7f,1.7f},{1.1f,.4f,.2f,.65f,.25f,1.2f},
 {.4f,.2f,.6f,.3f,.25f,.6f},{1.4f,.65f,.4f,.8f,1.1f,.65f},
 {.3f,.65f,1.7f,.25f,1.4f,1.2f},{1.9f,.5f,1.6f,.35f,1.1f,1.8f}
};
static const unsigned char world_cover[18][6]={
 {48,100,70,22,35,65},{100,85,80,40,90,55},{45,42,22,18,30,30},{100,30,95,35,65,15},
 {20,28,16,100,18,30},{10,10,24,35,12,30},{25,12,25,10,30,65},{65,80,45,50,60,25},
 {20,15,90,28,55,35},{20,35,55,35,15,22},{42,55,25,55,28,65},{25,35,32,65,40,22},
 {70,20,25,100,70,38},{100,70,40,95,85,55},{100,90,65,85,70,95},{100,60,25,65,45,85},
 {20,25,65,20,50,40},{55,35,60,25,80,45}
};
static const unsigned char world_wet[18][6]={
 {5,12,88,0,15,68},{22,15,45,8,90,12},{12,50,85,20,65,95},{18,95,90,35,3,70},
 {0,0,0,68,0,0},{0,75,10,0,0,35},{0,0,0,0,0,35},{10,8,0,35,82,0},
 {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,60,0,0},{0,0,0,0,60,0},
 {85,95,35,25,15,10},{20,15,8,12,85,10},{90,92,25,88,65,78},{0,15,0,10,0,0},
 {0,0,0,75,0,0},{0,20,0,0,0,0}
};
typedef struct {unsigned seed;int broad,type,count,biome[6];float x[6],z[6],scale;unsigned ground[6];} WorldProfile;
typedef struct {int a,b;float weight,relief,density,wet;unsigned ground;} WorldRegion;
static inline void world_pad(unsigned seed,float *x,float *z){
 /* Prop cells share a world origin; do not repeat its trigonometry for every
  * tree/collision query. Full seed tags preserve identity on cache aliasing. */
 static struct {unsigned seed;float x,z;int valid;} cache[16];int at=seed&15;
 if(!cache[at].valid||cache[at].seed!=seed){unsigned h=field_hash(seed+2u*9973u);float a=(h%6283)*.001f,d=480.f+(h%900);cache[at].seed=seed;cache[at].x=cosf(a)*d;cache[at].z=sinf(a)*d;cache[at].valid=1;}
 *x=cache[at].x;*z=cache[at].z;
}
static inline int world_type_seed(unsigned seed,int broad){
 if(broad==GAS){if(seed==field_seed(7,3))return WORLD_BANDED;return WORLD_BANDED+(field_hash(seed^0x41c64e6du)%4);}
 if(seed==field_seed(7,1)||seed==field_hash(2050u+65537u))return WORLD_TEMPERATE;
 if(seed==field_seed(7,2))return WORLD_OVERGROWN;if(seed==field_seed(7,4))return WORLD_FROZEN;
 unsigned pick=field_hash(seed^0x358da721u);int total=0;
 for(int i=0;i<18;i++)if(broad==OCEAN?(i==WORLD_TEMPERATE||i==WORLD_OCEAN||i==WORLD_ARCHIPELAGO||i==WORLD_SWAMP):(i!=WORLD_OCEAN&&i!=WORLD_ARCHIPELAGO))total+=world_types[i].weight;
 int at=pick%total;for(int i=0;i<18;i++)if(broad==OCEAN?(i==WORLD_TEMPERATE||i==WORLD_OCEAN||i==WORLD_ARCHIPELAGO||i==WORLD_SWAMP):(i!=WORLD_OCEAN&&i!=WORLD_ARCHIPELAGO)){at-=world_types[i].weight;if(at<0)return i;}
 return WORLD_BARREN;
}
static inline unsigned world_mix(unsigned a,unsigned b,float t){
 t=fmaxf(0,fminf(1,t));return WORLD_RGB((int)((a&255)+((int)(b&255)-(int)(a&255))*t),(int)(((a>>8)&255)+((int)((b>>8)&255)-(int)((a>>8)&255))*t),(int)(((a>>16)&255)+((int)((b>>16)&255)-(int)((a>>16)&255))*t));
}
static inline unsigned world_biome_ground(const WorldProfile *p,int id);
static inline const WorldProfile *world_profile(unsigned seed,int broad){
 static WorldProfile cache[16];static unsigned char valid[16];int at=(seed^(unsigned)broad)&15;WorldProfile *p=&cache[at];
 if(valid[at]&&p->seed==seed&&p->broad==broad)return p;
 *p=(WorldProfile){.seed=seed,.broad=broad,.type=world_type_seed(seed,broad),.scale=1650.f};
 unsigned h=field_hash(seed^0x1b873593u);int pool=broad==GAS?4:6,roll=h%100;
 p->count=roll<20?1:roll<70?2+((h>>8)&1):roll<95?4+((h>>9)&1):6;if(p->count>pool)p->count=pool;
 int used=0;for(int i=0;i<p->count;i++){int k=(field_hash(h+i*997u)>>8)%pool;while(used&(1<<k))k=(k+1)%pool;used|=1<<k;p->biome[i]=p->type<18?p->type*6+k:108+(p->type-18)*4+k;}
 if(seed==field_seed(7,1)){p->count=4;p->biome[0]=0;p->biome[1]=1;p->biome[2]=5;p->biome[3]=4;p->scale=1150;}
 if(seed==field_seed(7,2)){p->count=3;p->biome[0]=6;p->biome[1]=8;p->biome[2]=10;p->scale=1150;}
 if(seed==field_seed(7,4)){p->count=4;p->biome[0]=39;p->biome[1]=36;p->biome[2]=38;p->biome[3]=41;p->scale=1150;}
 p->x[0]=p->z[0]=0;
 for(int i=1;i<p->count;i++){unsigned k=field_hash(h+i*15331u);float angle=(k%6283)*.001f+i*1.7f,r=p->scale*(1.15f+((k>>16)%100)*.012f);p->x[i]=cosf(angle)*r;p->z[i]=sinf(angle)*r;}
 if(seed==field_seed(7,1)){p->x[1]=-1550;p->z[1]=1050;float a=((seed>>8)%6283)*.001f,u=2000,v=sinf(u/1180.f+(seed&255)*.017f)*310+sinf(u/430.f)*85+100;p->x[2]=u*cosf(a)-v*sinf(a);p->z[2]=u*sinf(a)+v*cosf(a);p->x[3]=0;p->z[3]=8800;}
 for(int i=0;i<p->count;i++)p->ground[i]=world_biome_ground(p,p->biome[i]);
 valid[at]=1;return p;
}
static inline unsigned world_biome_ground(const WorldProfile *p,int id){
 const WorldType *t=&world_types[p->type];int local=p->type<18?id%6:(id-108)%4;
 unsigned shade=local==0?WORLD_RGB(191,172,116):local==1?WORLD_RGB(70,110,103):local==2?WORLD_RGB(152,129,108):local==3?WORLD_RGB(208,192,151):local==4?t->accent:WORLD_RGB(83,126,114);
 return world_mix(t->ground,shade,p->type==WORLD_TEMPERATE?.34f:.19f);
}
static inline WorldRegion world_sample(const WorldProfile *p,float x,float z){
 int ai=0,bi=0;float da=1e30f,db=1e30f;
 for(int i=0;i<p->count;i++){float dx=x-p->x[i],dz=z-p->z[i],d=dx*dx+dz*dz;if(!i)d*=.70f;if(d<da){db=da;bi=ai;da=d;ai=i;}else if(d<db){db=d;bi=i;}}
 float w=p->count==1?1:fminf(1,.5f+(db-da)/(p->scale*p->scale*1.2f));w=w*w*(3-2*w);
 int a=p->biome[ai],b=p->biome[bi];WorldRegion r={.a=a,.b=b,.weight=w,.ground=world_mix(p->ground[bi],p->ground[ai],w)};
 if(p->type<18){int al=a%6,bl=b%6;const WorldType *t=&world_types[p->type];r.relief=t->relief*(world_relief[p->type][bl]+(world_relief[p->type][al]-world_relief[p->type][bl])*w);r.density=t->density*(world_cover[p->type][bl]+(world_cover[p->type][al]-world_cover[p->type][bl])*w)*.01f;r.wet=(world_wet[p->type][bl]+(world_wet[p->type][al]-world_wet[p->type][bl])*w)*.01f;}
 return r;
}
static inline const WorldProfile *field_world(const Game *g,int sys,int body){int sol=opening_sol(g)&&sys==g->system&&body>0&&body<BODY_COUNT;return world_profile(sol?g->bodies[body].seed:field_seed(sys,body),sol?g->bodies[body].type:field_type(sys,body));}
static inline int world_river_type(int t){return t==WORLD_TEMPERATE||t==WORLD_OVERGROWN||t==WORLD_OCEAN||t==WORLD_ARCHIPELAGO||t==WORLD_TUNDRA||t==WORLD_SWAMP||t==WORLD_TOXIC||t==WORLD_FUNGAL;}
static inline int world_legacy_biome(int type){return type>=18?5:type==WORLD_OCEAN||type==WORLD_ARCHIPELAGO?0:type==WORLD_DESERT||type==WORLD_SALT||type==WORLD_SCORCHED||type==WORLD_METAL?1:type==WORLD_FROZEN||type==WORLD_TUNDRA||type==WORLD_CRYSTAL?2:type==WORLD_BARREN||type==WORLD_VOLCANIC||type==WORLD_HELL||type==WORLD_CARBON?3:4;}
/* Same deterministic prop choice is consumed by drawing and collision. */
static inline int world_prop_kind(const WorldProfile *p,WorldRegion r,unsigned h){
 if((h>>20)%100>=(unsigned)r.density)return -1;
 if((h>>10)%100<(unsigned)world_types[p->type].trees)return 0;
 if(!world_types[p->type].trees&&(p->type==WORLD_BARREN||p->type==WORLD_SCORCHED||p->type==WORLD_CRYSTAL||p->type==WORLD_CARBON||p->type==WORLD_METAL||p->type==WORLD_FROZEN||p->type==WORLD_SALT))return 2;
 return 1+(h>>16)%3;
}
#endif
