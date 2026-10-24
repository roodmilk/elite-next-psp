/* ERL-only vehicle class. Original designs follow reference sheet positions
 * left-to-right, top-to-bottom. Shared stats keep this first roster balanced.
 * Ordinary PlayerShip IDs/fleet ownership remain unchanged.
 * inbox bits 16..19 are the persisted racer index; bits 0..3 retain notices.
 * Old saves therefore select racer 0. No stream/version/length changes. */
#define ERL_RACER_COUNT 12
typedef struct {const char *name;unsigned hull,trim;short nose,tail,width,pod,fin,style;} ErlRacer;
extern const ErlRacer erl_racers[ERL_RACER_COUNT];
int erl_racer_id(const Game *g,int actor);
int erl_racer_select(Game *g,int id);
Vec3 erl_racer_engine(int id,int side);

#ifdef ERL_RACERS_IMPLEMENTATION
#define RC(r,g,b) (0xff000000u|(r)|((g)<<8)|((b)<<16))
const ErlRacer erl_racers[12]={
 {"KESTREL",RC(49,160,172),RC(237,202,91),142,76,29,39,17,0},
 {"REDLINE",RC(181,44,42),RC(227,221,196),128,72,24,46,26,1},
 {"SOLSTICE",RC(249,178,37),RC(234,245,229),110,70,38,48,10,2},
 {"TWINSTREAM",RC(36,131,196),RC(232,238,223),147,74,19,48,15,3},
 {"MANTIS",RC(74,185,69),RC(211,231,201),104,78,24,56,30,4},
 {"VIOLET ARC",RC(99,54,142),RC(233,224,218),151,65,24,39,18,5},
 {"ROSEFIN",RC(203,57,115),RC(239,235,226),129,77,31,51,32,6},
 {"GOLD RUSH",RC(179,161,48),RC(243,84,143),119,82,35,54,12,7},
 {"YELLOWJACKET",RC(246,191,37),RC(54,75,85),148,70,25,43,24,8},
 {"POLARIS",RC(226,236,241),RC(51,115,214),136,80,22,53,16,9},
 {"SUNLANCE",RC(194,38,42),RC(247,197,45),144,67,33,39,27,10},
 {"RAZORBACK",RC(102,121,71),RC(192,203,157),155,76,19,49,9,11}
};
int __attribute__((noinline,optimize("Os"))) erl_racer_id(const Game *g,int actor){
 int selected=((unsigned)g->erl.inbox>>16)&15;if(selected>=12)selected=0;
 if(actor<=0)return selected;
 /* Eleven-slot permutation excludes the player's craft and cannot duplicate
  * any of the ten supported opponents. Stable across saves/restarts. */
 int n=(g->race.seed%11+(actor-1)*5)%11;return n>=selected?n+1:n;
}
int erl_racer_select(Game *g,int id){
 if(id<0||id>=12||g->erl.event_kind||g->erl.championship||race_is_active(g))return 0;
 g->erl.inbox=(g->erl.inbox&~(15<<16))|(id<<16);g->erl.dirty=1;return 1;
}
Vec3 erl_racer_engine(int id,int side){
 const ErlRacer *s=&erl_racers[id>=0&&id<12?id:0];return (Vec3){side*s->pod,-1,-s->tail-1};
}
#undef RC
#endif
