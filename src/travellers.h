/* Named travellers — session registry + promote hooks (include from game.c only).
 * See docs/LIVING-GALAXY-ALIVENESS.md.
 */
#ifndef TRAVELLERS_H
#define TRAVELLERS_H
#include "travellers-data.h"

static int traveller_pick_dest(const Game *g,int from,int salt){
 int best=from,tries=0;float best_d=1e9f;
 for(int i=0;i<256&&tries<48;i++){
  int cand=(from*17+salt*31+i*13)&255;if(cand==from)continue;
  float d=distance_ly(g,from,cand);if(d<.5f||d>7.f)continue;
  tries++;if(d<best_d){best_d=d;best=cand;}
 }
 return best==from?(from+17+(salt&15))&255:best;
}

static void travellers_seed(Game *g){
 for(int i=0;i<TRAVELLER_COUNT;i++){
  const TravellerDef *d=&traveller_defs[i];
  g->travellers[i].sys=(uint8_t)(d->home&255);
  g->travellers[i].dest=(uint8_t)traveller_pick_dest(g,g->travellers[i].sys,i*19+3);
  g->travellers[i].slot=-1;g->travellers[i].flags=0;
 }
 for(int i=3;i<TRAVELLER_COUNT;i++){
  int hop=(i*47+11)&255;g->travellers[i].sys=(uint8_t)hop;
  g->travellers[i].dest=(uint8_t)traveller_pick_dest(g,hop,i*7);
 }
}

static void travellers_clear_slots(Game *g){
 for(int i=0;i<TRAVELLER_COUNT;i++)g->travellers[i].slot=-1;
 for(int i=0;i<NPC_COUNT;i++)g->npc[i].traveller=-1;
}

static void travellers_advance(Game *g,int arrived_sys){
 for(int i=0;i<TRAVELLER_COUNT;i++){
  if(g->travellers[i].sys==arrived_sys)continue;
  if(g->travellers[i].sys==g->travellers[i].dest){g->travellers[i].dest=(uint8_t)traveller_pick_dest(g,g->travellers[i].sys,i*23+(int)g->kills);continue;}
  float d=distance_ly(g,g->travellers[i].sys,g->travellers[i].dest);
  if(d<=7.f){g->travellers[i].sys=g->travellers[i].dest;g->travellers[i].dest=(uint8_t)traveller_pick_dest(g,g->travellers[i].sys,i*29+arrived_sys);}
  else {
   int step=(g->travellers[i].sys*3+g->travellers[i].dest*5+i*11+arrived_sys)&255;
   if(distance_ly(g,g->travellers[i].sys,step)<distance_ly(g,g->travellers[i].sys,g->travellers[i].dest)+2.f)g->travellers[i].sys=(uint8_t)step;
  }
 }
}

static int travellers_find_slot(Game *g,int role){
 for(int i=0;i<36;i++){
  if(i%12==8)continue;
  NPC *n=&g->npc[i];
  if(!n->alive&&n->traveller<0)return i;
 }
 for(int i=0;i<36;i++){
  if(i%12==8)continue;
  NPC *n=&g->npc[i];
  if(n->alive&&n->role==role&&!n->freighter&&n->traveller<0)return i;
 }
 (void)role;return -1;
}

#endif
