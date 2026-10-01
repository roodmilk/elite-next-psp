/* Native, allocation-free faction dossiers. Lore is separate from local traffic. */
static const char *faction_short[]={"TRADERS","LAW","PIRATES","EXPLORERS"};
static const char *faction_motto[]={"KEEP THE LANES FED","ORDER IN THE VOID","NO FLAG. NO PROMISE.","LEAVE A BETTER CHART"};
static const char *faction_dossier[4][3]={
 {
 "Traders are the working bloodstream of settled space. Independent haulers and heavy freighters carry food, machinery and the things nobody puts on a passenger manifest. They disagree on almost everything except this: an empty cargo hold is a wasted journey.",
 "Traders run between the station and world ports, pausing to transfer cargo. Heavy freighters keep their own berth schedules. Hail crews for business; check System Operations for routes. Outer lanes have fewer patrols, but expose haulers to raiders.",
 "Freight crews measure a route in meals missed, docking fees and mugs of cold tea. Their channels carry practical gossip: which berth is blocked, which clinic needs supplies, and who last borrowed the good spanner."
 },
 {
 "Law is the blue light at the end of a bad decision. Patrol crews guard the lanes, challenge wanted pilots and inspect suspicious cargo. A badge does not make space safe, but the people behind it still expect everyone to get home.",
 "Law patrols main routes and dispatches from the station. A fitted Law Scanner reveals nearby inspection reach, but its echoes age. Cargo surrender clears cargo offences, not violence. Help finish a wanted target you recently hit and its bounty is still yours.",
 "The official channel is all clearances and incident numbers. Between reports, tired officers argue about station coffee and whose turn it is to inspect a leaking hold. Their favourite kind of shift is one with nothing worth writing down."
 },
 {
 "Pirates are independent raiders, not one tidy empire. Some crews dress extortion up as a toll; others do not bother with the speech. Their ships haunt trade routes, hunt vulnerable haulers and turn a quiet crossing into somebody else's emergency.",
 "Raiders hunt outer-lane traffic and flee nearby Law. Wanted posters track real ships, including escaping targets. Police cannot clear unengaged posters for you. Higher-risk targets carry stronger weapons and tougher hulls. Player takedowns stay recorded on the board.",
 "Raider channels trade boasts, warnings and suspiciously cheap salvage. Every captain claims to be the reasonable one. The old joke says a pirate's promise is worth its weight in vacuum; experienced traders still check the scanner before laughing."
 },
 {
 "The Explorers Guild belongs to pilots who cannot leave an unanswered signal alone. Survey wings cross the lanes while field crews catalogue unfamiliar worlds. A discovery matters, but a clear record that another traveller can use matters more.",
 "Follow Guild work through your Mission Log and tracked mission. Land on planets to scan flora, fauna and minerals into your records; analyse anomalies from space. Guild ships explore in formation and avoid raiders. They are peaceful, but will defend themselves.",
 "Guild channels mix careful observations with very bad naming suggestions. Somewhere, a committee is still deciding whether a new plant is a fern. Their unwritten rule is simple: bring back the measurements, and bring back the person who took them."
 }
};
static const char *faction_aside[4][2]={
 {"A crate is a promise with thrusters. The invoice is how you know they meant it.","Try: Market, Mission Board and Triangle hails. An offer is not an accepted mission until you agree to it."},
 {"Patrol handbook: stay calm, log the facts, and never trust an unlabelled cargo canister.","Try: Wanted posters and your legal status. Cargo surrender is offered only when you actually carry contraband."},
 {"Free cargo inspection. No appointment necessary. Satisfaction not guaranteed.","Try: GalacticNet / Wanted. Clearing those targets earns units; attacking innocent ships is still a crime."},
 {"Listen twice. Measure twice. Name the rock after lunch.","Try: planetary survey, Codex, Field Guide and the tracked Guild story. Unknown does not mean hostile."}
};
static int faction_local_count(int role){
 int count=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==role)count++;return count;
}
static const char *faction_note(int role,int card){
 if(card<2)return faction_aside[role][card];
 const char *channel=saga_faction_channel(&game,role,1);
 return channel?channel:faction_lore_tag(role);
}
static void faction_badge(int x,int y,int role,unsigned c){
 circle(x,y,11,c);
 if(role==TRADERS){rect(x-6,y-4,12,9,c);line(x-6,y-7,x+6,y-7,c);line(x,y-7,x,y+5,UI_PANEL);}
 else if(role==LAW){line(x-6,y-6,x+6,y-6,c);line(x-6,y-6,x-5,y+3,c);line(x+6,y-6,x+5,y+3,c);line(x-5,y+3,x,y+8,c);line(x+5,y+3,x,y+8,c);}
 else if(role==PIRATES){line(x-6,y-6,x+6,y+6,c);line(x+6,y-6,x-6,y+6,c);rect(x-3,y-4,6,6,c);}
 else {line(x,y-8,x,y+8,c);line(x-8,y,x+8,y,c);circle(x,y,4,c);}
}
static void factions(void){
 if(row<0||row>=FACTION_COUNT)row=0;
 if(faction_lore_card<0||faction_lore_card>2)faction_lore_card=0;
 int card=faction_lore_card;unsigned accent=faction_colors[row];
 header("FACTIONS / PILOT'S ALMANAC");
 panel(8,32,120,208);panel(136,32,336,208);
 for(int i=0;i<4;i++){
  int y=40+i*43;unsigned c=faction_colors[i];
  if(row==i){rect(12,y,112,39,UI_RAISED);rect(12,y,3,39,c);}
  faction_badge(29,y+18,i,c);text_px(46,y+9,row==i?WHITE:DIM,"%s",faction_short[i]);
  text_px(46,y+23,c,"DOSSIER %02d",i+1);
 }
 text_px(16,220,DIM,"4 FACTIONS");
 rect(144,40,3,34,accent);
 text_px(152,42,accent,"%s",faction_names[row]);
 text_px(152,58,DIM,"%.30s",faction_motto[row]);
 draw_portrait(416,38,48,40,game.system+row*17,row);
 const char *tabs[]={"IDENTITY","IN PLAY","CHANNEL"};
 for(int i=0;i<3;i++){int x=144+i*106;rect(x,84,102,16,i==card?accent:UI_RAISED);text_px(x+6,88,i==card?RGB(9,15,22):DIM,"%s",tabs[i]);}
 text_wrap(18,14,39,8,WHITE,faction_dossier[row][card],0);
 text_px(144,184,accent,card==0?"CREW SAYING":card==1?"COMMANDER'S NOTES":"STORY CHANNEL");
 text_wrap(18,25,39,4,DIM,faction_note(row,card),0);
 text_px(16,240,DIM,"LOCAL SNAPSHOT: %d %s SHIPS / %.12s",faction_local_count(row),faction_short[row],game.systems[game.system].name);
 footer("UP/DOWN FACTION   X PAGE   TRI CONTACT   O BACK");
}
