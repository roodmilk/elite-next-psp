#ifndef ELITE_MEGA_HULL_GEOMETRY_H
#define ELITE_MEGA_HULL_GEOMETRY_H
/* Twelve capital-hull silhouettes for native PSP previews.
 *
 * The kit deliberately uses a small fixed set of solid cuboids.  This keeps
 * every portrait shaded and readable while avoiding heap allocations or a
 * large vertex table.  Each class has authored extents; preview framing is
 * consequently stable through a complete rotation and never depends on an
 * approximate ordinary-ship mesh. */
typedef struct { float x,y,z,hx,hy,hz; unsigned shade; } MegaHullPart;
typedef struct { const char *name; float hx,hy,hz; const MegaHullPart *part; int parts; } MegaHullArt;

#define MB(x,y,z,hx,hy,hz,s) {(x),(y),(z),(hx),(hy),(hz),(s)}
static const MegaHullPart mega_manta[]={MB(0,0,0,42,5,28,0),MB(-34,0,-15,25,3,28,1),MB(34,0,-15,25,3,28,1),MB(0,-7,6,12,7,19,2),MB(-42,0,22,7,6,12,3),MB(42,0,22,7,6,12,3)};
static const MegaHullPart mega_atlas[]={MB(0,0,0,7,7,58,0),MB(0,8,-48,13,10,12,2),MB(-23,0,-20,14,12,20,1),MB(23,0,-20,14,12,20,1),MB(-23,0,24,14,12,20,1),MB(23,0,24,14,12,20,1),MB(-15,-1,52,9,9,13,3),MB(15,-1,52,9,9,13,3)};
static const MegaHullPart mega_hammer[]={MB(0,0,-34,48,7,15,2),MB(0,0,0,8,8,34,0),MB(-27,0,27,9,9,27,1),MB(27,0,27,9,9,27,1),MB(-27,0,53,13,12,10,3),MB(27,0,53,13,12,10,3)};
static const MegaHullPart mega_leviathan[]={MB(-10,0,0,15,14,48,0),MB(22,0,-22,19,19,14,2),MB(22,0,15,23,5,18,1),MB(32,0,40,11,12,19,3),MB(-31,-8,15,10,9,25,1),MB(-31,10,-10,8,8,20,2)};
static const MegaHullPart mega_cathedral[]={MB(0,0,0,12,22,58,0),MB(-25,-4,-12,11,17,35,1),MB(25,-4,-12,11,17,35,1),MB(-18,18,12,8,15,20,2),MB(18,25,2,7,20,18,2),MB(-25,0,50,13,13,12,3),MB(25,0,50,13,13,12,3)};
static const MegaHullPart mega_ark[]={MB(0,0,0,15,15,61,0),MB(-29,0,-22,6,30,7,1),MB(29,0,-22,6,30,7,1),MB(0,-29,-22,30,6,7,1),MB(0,29,-22,30,6,7,1),MB(-25,0,28,6,26,7,2),MB(25,0,28,6,26,7,2),MB(0,8,-60,10,9,10,3)};
static const MegaHullPart mega_clipper[]={MB(0,0,0,6,7,62,0),MB(-26,15,-4,3,29,35,1),MB(26,15,-4,3,29,35,1),MB(-34,-4,43,9,8,13,3),MB(34,-4,43,9,8,13,3),MB(-12,-4,51,7,7,12,3),MB(12,-4,51,7,7,12,3)};
static const MegaHullPart mega_vigilance[]={MB(0,0,0,38,5,57,0),MB(0,5,0,5,3,50,2),MB(22,12,-2,10,12,19,1),MB(-31,-3,39,10,11,15,3),MB(-10,-3,46,10,11,15,3),MB(10,-3,46,10,11,15,3),MB(31,-3,39,10,11,15,3)};
static const MegaHullPart mega_nautilus[]={MB(0,0,0,18,18,18,2),MB(-24,0,0,9,28,9,1),MB(0,25,0,28,8,9,1),MB(25,0,0,8,25,9,1),MB(0,-24,0,24,8,9,1),MB(0,0,35,14,14,31,0),MB(-16,0,57,8,9,13,3),MB(16,0,57,8,9,13,3)};
static const MegaHullPart mega_mirage[]={MB(-29,0,-7,14,7,44,0),MB(29,0,-7,14,7,44,0),MB(-15,0,-36,19,8,12,1),MB(15,0,-36,19,8,12,1),MB(-34,0,39,11,10,18,3),MB(34,0,39,11,10,18,3),MB(0,-5,12,7,5,22,2)};
static const MegaHullPart mega_crown[]={MB(0,0,0,16,12,16,2),MB(-32,0,0,28,8,10,0),MB(32,0,0,28,8,10,0),MB(0,0,-32,10,8,28,1),MB(0,0,32,10,8,28,1),MB(-24,0,-24,16,7,16,1),MB(24,0,-24,16,7,16,1),MB(-24,0,24,16,7,16,1),MB(24,0,24,16,7,16,1)};
static const MegaHullPart mega_anvil[]={MB(-28,0,-24,24,19,32,1),MB(28,0,-24,24,19,32,1),MB(0,0,5,12,10,25,2),MB(0,0,40,31,18,21,0),MB(-20,0,57,12,12,13,3),MB(20,0,57,12,12,13,3)};

static const MegaHullArt mega_hull_art[12]={
 {"MANTA RAY",60,14,50,mega_manta,sizeof(mega_manta)/sizeof(*mega_manta)},
 {"ATLAS HAULER",40,20,66,mega_atlas,sizeof(mega_atlas)/sizeof(*mega_atlas)},
 {"HAMMERHEAD",50,14,66,mega_hammer,sizeof(mega_hammer)/sizeof(*mega_hammer)},
 {"LEVIATHAN",47,25,61,mega_leviathan,sizeof(mega_leviathan)/sizeof(*mega_leviathan)},
 {"IRON CATHEDRAL",38,45,71,mega_cathedral,sizeof(mega_cathedral)/sizeof(*mega_cathedral)},
 {"WAYFARER ARK",38,38,71,mega_ark,sizeof(mega_ark)/sizeof(*mega_ark)},
 {"SOLAR CLIPPER",45,47,74,mega_clipper,sizeof(mega_clipper)/sizeof(*mega_clipper)},
 {"VIGILANCE CARRIER",48,24,72,mega_vigilance,sizeof(mega_vigilance)/sizeof(*mega_vigilance)},
 {"NAUTILUS",38,36,70,mega_nautilus,sizeof(mega_nautilus)/sizeof(*mega_nautilus)},
 {"MIRAGE",48,17,59,mega_mirage,sizeof(mega_mirage)/sizeof(*mega_mirage)},
 {"CROWN OF ZAONCE",60,20,60,mega_crown,sizeof(mega_crown)/sizeof(*mega_crown)},
 {"ANVIL",54,22,72,mega_anvil,sizeof(mega_anvil)/sizeof(*mega_anvil)}
};

static int mega_hull_art_id(const char *name){for(int i=0;i<12;i++)if(!strcmp(name,mega_hull_art[i].name))return i;return -1;}

#undef MB
/* Exactly the tapered vertices submitted by mega_art_block, in flight axes. */
static Vec3 mega_hull_vertex(const MegaHullArt *a,unsigned index){const MegaHullPart *b=&a->part[(index/8)%a->parts];int i=index%8;float nose=(i&4)?.62f:1.f;return (Vec3){-(b->x+((i&1)?b->hx:-b->hx)*nose),b->y+((i&2)?b->hy:-b->hy)*(.78f+.22f*nose),-(b->z+((i&4)?b->hz:-b->hz))};}
#endif
