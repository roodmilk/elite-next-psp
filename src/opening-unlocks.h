/* Earned command-deck services. Stable IDs, no added save fields: the saved
 * opening stage already identifies every completed teaching milestone. */
#ifndef ELITE_OPENING_UNLOCKS_H
#define ELITE_OPENING_UNLOCKS_H
#define OPENING_ALL_SERVICES ((UINT32_C(1)<<30)-1)
static inline uint32_t opening_stage_service_mask(int stage){
 if(stage<=0||stage>=OP_FREE)return OPENING_ALL_SERVICES;
 static const struct {int stage,id;} earned[]={
  {OP_FIRST_SAVE,5},{OP_RADIO,16},{OP_LAUNCH,0},{OP_SOL_MAP,11},
  {OP_EARTH_NET,14},{OP_LOADOUT,21},{OP_LOG,13},{OP_TRACK,17},
  {OP_MINE,8},{OP_MUM_RING,10},{OP_REPAIR,24},{OP_YARD,3},
  {OP_OUTFIT,4},{OP_FOOD,1},{OP_GALAXY,2},{OP_APP,23},{OP_NEWS,26}
 };
 uint32_t mask=0;
 for(unsigned i=0;i<sizeof(earned)/sizeof(earned[0]);i++)if(stage>=earned[i].stage)mask|=UINT32_C(1)<<earned[i].id;
 return mask;
}
static inline uint32_t opening_unlocked_services(const Game *g){
 if(!g||!opening_exists(g))return OPENING_ALL_SERVICES;
 uint32_t mask=opening_stage_service_mask(g->opening.stage);
 if(!(g->opening.flags&OF_FACTIONS))mask&=~(UINT32_C(1)<<7);
 if(!(g->opening.flags&OF_LORE))mask&=~(UINT32_C(1)<<22);
 /* Discovery opens with local evidence or when the guided opening ends. */
 if(opening_done(g)||g->discoveries>0||(g->opening.flags&OF_LORE))mask|=UINT32_C(1)<<15;
 if(!(g->opening.flags&OF_APP))mask&=~(UINT32_C(1)<<23);
 return mask;
}
static inline int opening_menu_available(const Game *g,int id){
 if(id<0||id>=30)return 0;
 return (opening_unlocked_services(g)&(UINT32_C(1)<<id))!=0;
}
#endif
