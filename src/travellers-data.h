/* Traveller presentation + defs. Sim helpers live in travellers.h (game.c only). */
#ifndef TRAVELLERS_DATA_H
#define TRAVELLERS_DATA_H

#define TRAVELLER_COUNT 12

typedef struct {
 const char *callsign;
 const char *speech[4];
 int role;
 int home;
} TravellerDef;

static const TravellerDef traveller_defs[TRAVELLER_COUNT]={
 {"MIRA VANE",{
  "Mira Vane, independent medical hauler. I carry clinic supplies between the smaller stations. Keep clear of my wake and we will get along.",
  "You again, Commander. The clinic run is late, but intact. That counts as a good day in this trade.",
  "I started hauling medicine after a station charged my sister more for delivery than for treatment. I cannot fix the market, but I can keep one route honest.",
  "That is all I have time for. Three clinics are waiting on this hold. Clear skies, Commander."
 },TRADERS,7},
 {"IONA REED",{
  "Iona Reed, Explorers Guild field recorder. If you report a discovery to me, include a timestamp and coordinates. A good story is not evidence.",
  "Hello again. I checked your last transponder record before answering. Habit, not suspicion.",
  "My first survey partner fabricated a whole moon to hide a navigation mistake. Beautiful report. Terrible moon. I have insisted on raw records ever since.",
  "I have given you everything cleared for this channel. Bring me evidence and we can speak again."
 },EXPLORERS,7},
 {"TAMSIN KOR",{
  "Tamsin Kor, convoy lead. I shepherd civilian hulls through bad lanes. If you fly near my formation, mark your debris and answer before changing course.",
  "Commander. I remember your transponder. My crews prefer familiar traffic, even when they pretend otherwise.",
  "Every captain in my convoy thinks theirs is the ship I worry about most. They are all correct. That is the job.",
  "Nothing further while I am responsible for this formation. Hold your lane and let my crews work."
 },TRADERS,129},
 {"SABLE QUINN",{
  "Sable Quinn. I arrange passage through lanes respectable pilots call unsafe. Do not mistake an introduction for friendship; passage has a price.",
  "I know that ship now. You listened the first time, which puts you ahead of most commanders.",
  "People call me a pirate because customs dislikes my invoices. Customs calls everyone a pirate eventually; I merely saved them the paperwork.",
  "We have said enough on an open frequency. If I need you, I will make contact."
 },PIRATES,0},
 {"ORU VENN",{
  "Oru Venn, long-range survey wing. We map quiet systems and try to leave them quiet. Keep your weapons cold near our instruments.",
  "Venn here. I recognise your drive note now. It carries farther than you think.",
  "The Guild publishes discoveries and forgets the months between them. Most exploration is checking empty coordinates carefully enough to trust the emptiness.",
  "No new survey data to share. Let the channel rest until one of us finds something worth interrupting the silence."
 },EXPLORERS,7},
 {"DOCKHAND NYX",{
  "Nyx, relief-fleet loadmaster. I coordinate food, medicine and evacuation berths. Relief hulls take priority, and every loose buoy gets filed.",
  "Commander, good to hear you again. We cleared the last backlog; another one arrived before the deck cooled.",
  "I used to fly these supplies myself. Then I learned one careful loadmaster can keep six reckless pilots alive. I miss flying less than I expected.",
  "That is the whole manifest and all the gossip I can spare. I have relief ships waiting."
 },TRADERS,7},
 {"KEEL RAIDER",{
  "They call me Keel. I work the lanes your chart marks in red. That scanner of yours is charming, but this is the wrong neighbourhood for curiosity.",
  "Still flying, Commander? Either you learn quickly or fortune has developed poor judgement.",
  "I was a station tug pilot once. Same work, really: take hold of something valuable and ignore the owner shouting over comms.",
  "Conversation is over. Leave with your cargo while that remains an option."
 },PIRATES,42},
 {"PATROL RISS",{
  "Patrol Officer Riss, local enforcement. Your transponder is recorded and the warrant board is live. Keep your weapons safe and your cargo declared.",
  "Commander, Riss again. Your identity check is already clear; do not make me repeat the rest.",
  "Most patrol work is not pursuit. It is explaining the same regulation until somebody decides surviving is less embarrassing than listening.",
  "No further questions on this channel. Maintain course and let the patrol continue."
 },LAW,7},
 {"FREIGHT JAY",{
  "Freight Jay, independent peer-runner. I move awkward cargo between stations too small for the capital haulers. Give the paint a little room.",
  "There is that familiar transponder. You kept off my hull last time, so I am answering politely.",
  "My ship lists two centimetres to port when the hold is empty. Every mechanic offers to fix it. After twelve years, flying straight would feel broken.",
  "That is enough talk. I have a berth time to miss by the smallest possible margin."
 },TRADERS,33},
 {"GUILD LEN",{
  "Len, Explorers Guild route cartographer. I chart the quiet lanes between famous places. The loud routes already have enough names attached to them.",
  "Commander. I remember your course trace. You favour direct lines; the galaxy rarely does.",
  "My best chart contains almost nothing: no fuel, no traffic and no rescue coverage. Knowing where emptiness begins has saved more pilots than any landmark.",
  "I have no fresh route data for you. Ask again after the map has had time to become wrong."
 },EXPLORERS,91},
 {"BRIG HALE",{
  "Brig Hale, independent operator. Independent of governments, guild dues and, when necessary, manners. State your business briefly.",
  "I remember you. That is neither a threat nor a compliment yet.",
  "I spent six years flying legal freight. The contracts were longer, the pay was worse and everyone lied with official seals. At least out here the lies are concise.",
  "We are finished for now. Keep hailing and I may decide you are volunteering cargo."
 },PIRATES,18},
 {"WATCHER CAL",{
  "Watch Officer Cal, intersystem law liaison. This channel is logged. Identify your purpose and keep your flight computer transmitting.",
  "Cal here. Your identity remains on file, Commander. What has changed since our last contact?",
  "I joined the watch to stop raiders. Most days I mediate docking arguments and missing forms. Order is less dramatic than the recruitment film suggested.",
  "There is nothing further I can release. If your circumstances change, contact the nearest patrol."
 },LAW,55},
};

static const char *traveller_name(int id){
 if(id<0||id>=TRAVELLER_COUNT)return "TRAVELLER";
 return traveller_defs[id].callsign;
}

static const char *traveller_hail(const Game *g,int id){
 if(id<0||id>=TRAVELLER_COUNT)return "Keep your distance.";
 unsigned flags=g?g->travellers[id].flags:0;
 if(!(flags&1))return traveller_defs[id].speech[0];
 unsigned stage=(flags>>1)&3;
 return traveller_defs[id].speech[stage<1?1:stage];
}

static void traveller_note_hail(Game *g,int id){
 if(!g||id<0||id>=TRAVELLER_COUNT)return;
 unsigned flags=g->travellers[id].flags,stage=(flags>>1)&3;
 if(!(flags&1)){flags|=1;stage=1;}
 else if(stage<3)stage++;
 g->travellers[id].flags=(uint8_t)((flags&~6u)|(stage<<1));
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
 int id=-1;for(int i=0;i<12;i++)if(g->travellers[i].sys==g->system){id=i;break;}
 if(id<0){snprintf(line,sizeof(line),"Spotted nobody from the long-haul list here today. Quiet watch.");return line;}
 if(g->travellers[id].sys==g->system){
  snprintf(line,sizeof(line),"%.12s lingering near %.8s.",traveller_defs[id].callsign,g->systems[g->system].name);
 }else{
  snprintf(line,sizeof(line),"Spotted %.12s in %.8s heading %.8s.",
   traveller_defs[id].callsign,g->systems[g->travellers[id].sys].name,g->systems[g->travellers[id].dest].name);
 }
 return line;
}

#endif
