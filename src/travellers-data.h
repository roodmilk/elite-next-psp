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

/* Prefer a traveller not in the current system so GalNet reads as elsewhere. */
static int travellers_elsewhere_id(const Game *g,int salt){
 int best=-1;
 for(int i=0;i<TRAVELLER_COUNT;i++){
  int id=(i+salt)%TRAVELLER_COUNT;
  if(g->travellers[id].sys!=g->system){best=id;break;}
 }
 if(best>=0)return best;
 return salt%TRAVELLER_COUNT;
}

static const char *travellers_galnet_traffic(const Game *g){
 static char line[80];
 int id=travellers_elsewhere_id(g,g->system*3+2);
 snprintf(line,sizeof(line),"%.12s cleared %.8s bound for %.8s.",
  traveller_defs[id].callsign,g->systems[g->travellers[id].sys].name,g->systems[g->travellers[id].dest].name);
 return line;
}

static const char *travellers_galnet_spotter(const Game *g){
 static char line[80];
 int id=travellers_elsewhere_id(g,g->system*5+6);
 if(g->travellers[id].sys==g->system){
  snprintf(line,sizeof(line),"%.12s lingering near %.8s.",traveller_defs[id].callsign,g->systems[g->system].name);
 }else{
  snprintf(line,sizeof(line),"Spotted %.12s in %.8s heading %.8s.",
   traveller_defs[id].callsign,g->systems[g->travellers[id].sys].name,g->systems[g->travellers[id].dest].name);
 }
 return line;
}

#endif
