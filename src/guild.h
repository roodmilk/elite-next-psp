/* Opening playable Guild objectives. Kept separate from the optional coach.
 * Progress uses the three formerly reserved version-7 storypack integers.
 * Chapter increments and reward commit together, so repeated claims do nothing.
 */
enum { GUILD_LAUNCH=1,GUILD_DOCK=2,GUILD_DELIVERY=4,GUILD_SCAN=8,GUILD_RESCUE=16 };
static inline int guild_required_contract(const Game *g){return g->guild_chapter==1?MISSION_DELIVERY:g->guild_chapter==3?MISSION_RESCUE:-1;}
/* Reproduce the board's deterministic offer slots for any system without
 * mutating or copying the live game. This guarantees that a routed board
 * still contains the promised contract when the player arrives. */
static inline int guild_offer_index_at(const Game *g,int system,int type){
 int available=0,max=1+prosperity(g,system);for(int i=0;i<256&&available<max;i++)if(i!=system&&distance_ly(g,system,i)<=10.0f)available++;
 for(int offer=0;offer<available;offer++)if((system+offer)%MISSION_TYPES==type)return offer;
 return -1;
}
static inline int guild_contract_station(const Game *g){
 int type=guild_required_contract(g);if(type<0)return g->system;if(guild_offer_index_at(g,g->system,type)>=0)return g->system;
 int best=-1;float range=1e9f;for(int system=0;system<256;system++){if(system==g->system||guild_offer_index_at(g,system,type)<0)continue;float d=distance_ly(g,g->system,system);if(d<range){range=d;best=system;}}
 return best>=0?best:g->system;
}
static inline int guild_ready(const Game *g){
 const int need[]={GUILD_LAUNCH|GUILD_DOCK,GUILD_DELIVERY,GUILD_SCAN,GUILD_RESCUE};
 return g->guild_chapter>=0&&g->guild_chapter<4&&
        (g->guild_flags&need[g->guild_chapter])==need[g->guild_chapter];
}
static inline void guild_event(Game *g,int event){
 if(g->guild_chapter<0||g->guild_chapter>=4)return;
 if(g->guild_chapter==0&&(event==GUILD_LAUNCH||event==GUILD_DOCK)){
  if(event!=GUILD_DOCK||(g->guild_flags&GUILD_LAUNCH))g->guild_flags|=event;
 }else if((g->guild_chapter==1&&event==GUILD_DELIVERY)||
          (g->guild_chapter==2&&event==GUILD_SCAN)||
          (g->guild_chapter==3&&event==GUILD_RESCUE))g->guild_flags|=event;
}
static inline const char *guild_title(const Game *g){
 const char *titles[]={"A SHIP WITH YOUR NAME ON IT","SOMETHING WORTH CARRYING","THE QUIET BETWEEN PINGS","ALL HANDS ACCOUNTED FOR","THE CHANNEL STAYS OPEN"};
 int i=g->guild_chapter;if(i<0)i=0;if(i>4)i=4;return titles[i];
}
static inline const char *guild_objective(const Game *g){
 if(guild_ready(g))return g->docked?"Assignment complete. Collect your reward.":"Return to a station to collect your reward.";
 static char routed[96];int type=guild_required_contract(g);if(type>=0){for(int i=0;i<g->job_n;i++)if(g->jobs[i].type==type)return type==MISSION_DELIVERY?"Complete your active food delivery.":"Complete your active pilot rescue.";int station=guild_contract_station(g);snprintf(routed,sizeof(routed),"Take a %s mission at %s Hub.",type==MISSION_DELIVERY?"food delivery":"pilot rescue",g->systems[station].name);return routed;}
 const char *tasks[]={"Launch and return safely to a station.","Complete a food delivery from the board.","Find and scan an uncharted anomaly.","Rescue a pilot and return them to a hub.","Opening assignments complete. Keep exploring."};
 int i=g->guild_chapter;if(i<0)i=0;if(i>4)i=4;return tasks[i];
}
static inline const char *guild_line(const Game *g,int line){
 /* Expanded Guild dialogue from manuscript Vol III — echo Open Channel verbs without spoiling Act I. */
 static const char *lines[][4]={
  {"KEI: Before I send you on a survey, I want to know that you can leave a station and return safely.","Launch from the berth, fly clear of the station, then approach the hub and press Circle to dock. Return to this assignment for your reward.","VENN: Prefer docking soft enough that I do not file you.","A safe return is a good first story."},
  {"TAMSIN: A settlement is waiting for food, and we need a pilot who can get the supplies there intact.","Take the food-delivery contract named below. Keep its food aboard, travel to the destination and dock to complete the delivery.","KEI: Take a food job from the board.","Keep its crate aboard until you dock."},
  {"ORU: Our charts have a gap that I would like you to investigate. Look for an unscanned anomaly in the local system.","Select the signal in your target list, approach it and press Circle to scan. When the scan is recorded, dock at a station to collect your reward.","Get close, then press Circle to scan.","KEI: Ryn taught me to listen twice."},
  {"KEI: A pilot has sent a rescue beacon, and the Guild needs someone who can bring them back safely.","Accept a pilot-rescue contract from the mission board named below. Follow its instructions to pick up the survivor, then return them to a station.","Hail them nearby, then bring them home.","Nobody should become a missing number."},
  {"KEI: You have completed all four of our opening assignments. You brought supplies, information and people back where they were needed.","Thank you for seeing them through. Choose another mission in the log whenever you are ready; our search for Ryn is a separate entry.","Her trail goes beyond these assignments.","For now, the galaxy is yours to explore."}
 };
 int i=g->guild_chapter;if(i<0)i=0;if(i>4)i=4;
 return line>=0&&line<4?lines[i][line]:"";
}
