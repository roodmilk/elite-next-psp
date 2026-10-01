#ifndef ELITE_STATION_PROFILE_H
#define ELITE_STATION_PROFILE_H
/* Deterministic exterior identity. The port aperture remains constant while
 * the inhabited hull varies by more than 2x in diameter and depth. */
typedef struct {
 unsigned seed, hull, trim, light;
 float radius, half, spin;
 int family, bands, pods, station_class;
} StationProfile;
enum { STATION_POOR=0,STATION_RICH=1,STATION_MEGA=2 };
static inline unsigned station_profile_hash(int system,int variant){
 unsigned h=(unsigned)(system+1)*0x9e3779b9u^(unsigned)(variant+11)*0x85ebca6bu;
 h^=h>>16;h*=0x7feb352du;h^=h>>15;h*=0x846ca68bu;return h^(h>>16);
}
/* Public-facing station identity.  The legacy economy still drives prices
 * behind the scenes, but players only need this clear three-class model. */
static inline int station_class_for_system(const Game *g,int system){
 unsigned h=station_profile_hash(system,0);int economy=g->systems[system].economy&7;
 if(system!=7&&(h%17u)==0)return STATION_MEGA;
 return economy==2||economy==6?STATION_POOR:STATION_RICH;
}
static inline int station_class(const Game *g){return station_class_for_system(g,g->system);}
static inline const char *station_class_name(int kind){return kind==STATION_MEGA?"MEGA CAPITAL":kind==STATION_POOR?"POOR":"RICH";}
static inline int station_market_item_visible(const Game *g,int item){
 if(station_class(g)!=STATION_POOR)return 1;
 /* Nine of seventeen commodities, stable per system; hidden goods are
    neither bought nor sold at this under-supplied port. */
 return ((item+g->system*3)&1)==0;
}
static inline StationProfile station_profile_for(const Game *g,int variant){
 unsigned h=station_profile_hash(g->system,variant);int economy=g->systems[g->system].economy;
 static const unsigned hulls[]={0xb7c6cc,0x9e8871,0x788e9f,0xa7a2ad,0x758c78,0xa68b80,0x6f7f94,0x9b967b};
 static const unsigned trims[]={0x55d4d4,0xe0ad62,0xbc747c,0x75b58b,0x8ea8df,0xc798d1,0xd4c77a,0x78c5b6};
 StationProfile p;p.station_class=station_class(g);
 p.seed=h;p.family=(int)(h%8);p.radius=150.f+(float)((h>>5)%211);p.half=145.f+(float)((h>>14)%236);
 if(p.station_class==STATION_POOR){p.radius=145.f+(float)((h>>5)%76);p.half=140.f+(float)((h>>14)%91);}
 else if(p.station_class==STATION_RICH&&!variant){p.family=h%16;p.radius=460.f+(float)((h>>5)%161);p.half=350.f+(float)((h>>14)%151);}
 else if(p.station_class==STATION_MEGA){p.radius=940.f+(float)((h>>5)%281);p.half=680.f+(float)((h>>14)%241);}
 if(variant){p.radius*=.58f+.08f*variant;p.half*=.62f+.06f*variant;p.family=(p.family+variant*2)%8;}
 p.spin=.012f+(float)((h>>22)%21)*.0011f;p.bands=1+(int)((h>>9)%3);p.pods=3+(int)((h>>17)%6);
 if(p.station_class==STATION_RICH&&!variant)p.spin*=.38f;
 if(p.station_class==STATION_MEGA){p.spin=0;p.bands=5;p.pods=10;}
 p.hull=hulls[(economy+(h>>27))&7];p.trim=trims[(g->systems[g->system].government+(h>>24))&7];p.light=trims[(h>>20)&7];
 return p;
}
static inline int station_port_count_for(const Game *g,int variant){return !variant&&station_class(g)==STATION_MEGA?5:1;}
static inline Vec3 station_port_offset_for(const Game *g,int variant,int port){
 StationProfile p=station_profile_for(g,variant);if(port<=0||station_port_count_for(g,variant)==1)return (Vec3){0,0,0};
 float x=p.radius*.47f,y=p.radius*.34f;
 return port==1?(Vec3){-x,0,0}:port==2?(Vec3){x,0,0}:port==3?(Vec3){0,-y,0}:(Vec3){0,y,0};
}
static inline float station_half_for(const Game *g,int variant){return station_profile_for(g,variant).half;}
static inline float station_entry_z_for(const Game *g,int variant){return STATION_Z-station_half_for(g,variant);}
static inline Vec3 station_port_corner_for(const Game *g,int variant,int i){
 return (Vec3){(i==0||i==3)?-STATION_PORT_HALF_W:STATION_PORT_HALF_W,
               i<2?-STATION_PORT_HALF_H:STATION_PORT_HALF_H,-station_half_for(g,variant)};
}
#endif
