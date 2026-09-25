#ifndef ELITE_NEXT_WORLD_STATE_H
#define ELITE_NEXT_WORLD_STATE_H
#include <string.h>
/* Canonical, read-only world snapshot used by mission generation, HUDs and
 * certification. It is derived from the live Game so there is one source of
 * truth for economy, danger, government, traffic and active events. */
typedef struct {
 int system,economy,government,tech,danger;
 int faction[FACTION_COUNT],npc_count,station_count,planet_count;
 unsigned active_events;
} WorldState;
static inline int world_reputation(const Game *g,int role){
 if(!g||role<0||role>=FACTION_COUNT)return 0;
 if(role==EXPLORERS)return g->guild_chapter*5+(g->guild_flags&31);
 if(role==LAW)return g->legal?-(g->legal):0;
 if(role==TRADERS)return g->kills<0?0:g->kills/4;
 return g->wanted[g->system]>0?-g->wanted[g->system]:0;
}
static inline void world_state_build(const Game *g,int system,WorldState *w){
 if(!w)return;memset(w,0,sizeof(*w));if(!g)return;
 if(system<0||system>255)system=g->system;w->system=system;w->economy=g->systems[system].economy;w->government=g->systems[system].government;w->tech=g->systems[system].tech;w->danger=danger_rating(g,system);w->station_count=HUB_COUNT;w->planet_count=BODY_COUNT-1;
 if(system==g->system){for(int i=0;i<NPC_COUNT;i++)if(g->npc[i].alive){w->npc_count++;if(g->npc[i].role>=0&&g->npc[i].role<FACTION_COUNT)w->faction[g->npc[i].role]++;}w->active_events=(g->attacked>0?1u:0u)|(g->police_stop?2u:0u)|(g->freight_gap<=0?4u:0u);}
}
static inline int world_station_available(const Game *g,int system,int hub){WorldState w;world_state_build(g,system,&w);return hub>=0&&hub<w.station_count;}
static inline int world_planet_available(const Game *g,int system,int body){return g&&system>=0&&system<256&&body>=1&&body<BODY_COUNT;}
#endif
