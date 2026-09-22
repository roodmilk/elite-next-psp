/* Traveller presentation + defs. Sim helpers live in travellers.h (game.c only). */
#ifndef TRAVELLERS_DATA_H
#define TRAVELLERS_DATA_H

#define TRAVELLER_COUNT 12

typedef struct {
 const char *callsign;
 const char *hail;
 int role;
 int home;
} TravellerDef;

static const TravellerDef traveller_defs[TRAVELLER_COUNT]={
 {"MIRA VANE","Hauling clinics. Stay off my wake.",TRADERS,7},
 {"IONA REED","Timestamps or it did not happen.",EXPLORERS,7},
 {"TAMSIN KOR","Convoy lead. Mark your debris.",TRADERS,129},
 {"SABLE QUINN","Passage has a price. Always.",PIRATES,0},
 {"ORU VENN","Survey wing. Weapons stay cold.",EXPLORERS,7},
 {"DOCKHAND NYX","Relief hulls first. File your buoys.",TRADERS,7},
 {"KEEL RAIDER","Cute scanner. Wrong neighbourhood.",PIRATES,42},
 {"PATROL RISS","Warrant board is live. Behave.",LAW,7},
 {"FREIGHT JAY","Peer run — do not scrape the paint.",TRADERS,33},
 {"GUILD LEN","Chart the quiet lanes, not the loud ones.",EXPLORERS,91},
 {"BRIG HALE","Independent. Independent of manners.",PIRATES,18},
 {"WATCHER CAL","Law channel. State your business.",LAW,55},
};

static const char *traveller_name(int id){
 if(id<0||id>=TRAVELLER_COUNT)return "TRAVELLER";
 return traveller_defs[id].callsign;
}

static const char *traveller_hail(int id){
 if(id<0||id>=TRAVELLER_COUNT)return "Keep your distance.";
 return traveller_defs[id].hail;
}

static const char *travellers_brief_line(const Game *g){
 static char line[48];
 for(int i=0;i<TRAVELLER_COUNT;i++)if(g->travellers[i].sys==g->system){
  snprintf(line,sizeof(line),"%.12s here -> %.8s",traveller_defs[i].callsign,g->systems[g->travellers[i].dest].name);
  return line;
 }
 for(int i=0;i<TRAVELLER_COUNT;i++)if(g->travellers[i].dest==g->system){
  snprintf(line,sizeof(line),"%.12s inbound soon",traveller_defs[i].callsign);
  return line;
 }
 return "No named traffic on board.";
}

#endif
