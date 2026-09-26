/* Labels and handlers share these action IDs, so hints cannot drift from input. */
enum { NA_BEGIN,NA_FLY,NA_DOCK,NA_ROUTE,NA_REWARD,NA_ASSIGNMENTS,NA_BOARD,NA_LOG,NA_CONTACTS,NA_MAP,NA_DECK };
static int assignment_job(void){int type=game.guild_chapter==3?MISSION_RESCUE:MISSION_DELIVERY;for(int i=0;i<game.job_n;i++)if(game.jobs[i].type==type)return i;return -1;}
static int assignment_offer(void){int type=guild_required_contract(&game);for(int i=0;i<mission_count(&game);i++)if(mission_type_for_offer(&game,i)==type)return i;return -1;}
static int narrative_action(int screen){
 if(screen==CAMPAIGN){
  if(game.campaign_stage==6)return NA_ASSIGNMENTS;
  if(game.system!=7)return NA_ROUTE;
  if(game.campaign_stage==0)return game.docked?NA_BEGIN:NA_DOCK;
  if(game.campaign_stage==5)return game.docked?NA_REWARD:NA_DOCK;
  return !game.docked&&(game.campaign_stage==4||(game.campaign_flags&CP_FLEW))?NA_DOCK:NA_FLY;
 }
 if(game.guild_chapter>=4)return NA_DECK;
 if(guild_ready(&game))return game.docked?NA_REWARD:NA_DOCK;
 if(game.guild_chapter==0)return game.docked?NA_FLY:NA_DOCK;
 if(game.guild_chapter==2){for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive&&!game.anomaly[i].scanned)return game.docked?NA_FLY:NA_CONTACTS;return NA_MAP;}
 if(assignment_job()>=0)return NA_LOG;
 if(!game.docked)return NA_DOCK;
 return assignment_offer()>=0?NA_BOARD:NA_MAP;
}
static const char *narrative_label(int action){
 switch(action){
 case NA_BEGIN:return "Accept first flight";
 case NA_FLY:return game.docked?"Launch ship":"Resume flight";
 case NA_DOCK:return "Open docking guidance";
 case NA_ROUTE:return "Plan route to Lave";
 case NA_REWARD:return "Collect reward";
 case NA_ASSIGNMENTS:return "View optional assignments";
 case NA_BOARD:return "Choose the required contract";
 case NA_LOG:return "Track your active contract";
 case NA_CONTACTS:return "Find an unscanned signal";
 case NA_MAP:{static char route[40];int type=guild_required_contract(&game);if(type>=0&&assignment_job()<0){int station=guild_contract_station(&game);snprintf(route,sizeof(route),"Route to %.18s Hub",game.systems[station].name);return route;}return "Choose another system";}
 default:return "Return to the main menu";
 }
}
static void narrative_do(int screen){
 int action=narrative_action(screen);
 if(action==NA_BEGIN){campaign_accept(&game);row=0;}
 else if(action==NA_REWARD){if(screen==CAMPAIGN)campaign_claim(&game);else guild_claim(&game);row=0;}
 else if(action==NA_FLY){analog_ready=0;launch(&game);selected_target=0;autoaim=0;scan_cat=0;if(screen==CAMPAIGN)campaign_event(&game,CP_LOCK);change_page(FLIGHT);}
 else if(action==NA_DOCK)change_page(COMMS);
 else if(action==NA_ROUTE){int hops=0,hop=route_next_hop(&game,7,&hops);game.destination=hop>=0?hop:7;change_page(CHART);message(&game,"Choose a jump toward Lave. Check fuel first.");}
 else if(action==NA_ASSIGNMENTS)change_page(GUILD);
 else if(action==NA_BOARD){int offer=assignment_offer();change_page(MISSIONS);if(offer>=0)row=offer;}
 else if(action==NA_LOG){int job=assignment_job();if(job>=0)game.job_sel=job;change_page(MISSIONLOG);}
 else if(action==NA_CONTACTS){change_page(LOCAL);contacts_refresh();for(int i=0;i<contact_count;i++)if(IS_ANOMALY_ID(contact_ids[i])&&!game.anomaly[contact_ids[i]-ANOMALY_ID_MIN].scanned){row=i;break;}}
 else if(action==NA_MAP){int type=guild_required_contract(&game);if(type>=0&&assignment_job()<0){int station=guild_contract_station(&game),hops=0,hop=route_next_hop(&game,station,&hops);game.destination=hop>=0?hop:station;change_page(CHART);char note[96];snprintf(note,sizeof(note),"Route to %s Hub. Required contract confirmed there.",game.systems[station].name);message(&game,note);}else change_page(CHART);}
 else change_page(HOME);
}
static int saga_speaker_role(const SagaBeat *b){
 if(!b)return EXPLORERS;
 if(b->role==3)return LAW;
 if(b->role==2)return TRADERS;
 return EXPLORERS;
}
static void narrative_footer(void){footer(page==CAMPAIGN?"UP/DOWN   X CHOOSE   O BACK   SELECT LOG":"UP/DOWN   X CHOOSE   O BACK");}
enum { PROLOGUE_BRIEF_BEATS = 6 };
static int saga_brief_beat=0,saga_brief_chapter=-1,prologue_brief_beat=0;
/* echo=1: player just asked; next Cross reveals the NPC answer (never answer-before-ask). */
static int prologue_brief_echo=0,saga_brief_echo=0;
static void saga_brief_reset(int chapter){if(saga_brief_chapter!=chapter){saga_brief_chapter=chapter;saga_brief_beat=0;saga_brief_echo=0;}}
static int saga_coda_locked(void){return saga_coda_pending>=0;}
/* Locked until the player finishes every beat and accepts the next step. */
static int saga_brief_locked(void){return game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&!game.saga_step&&saga_coda_pending<0;}
static int prologue_brief_locked(void){return tracked_mission==0&&game.campaign_stage==0&&game.system==7&&game.docked;}
static int story_brief_locked(void){return prologue_brief_locked()||saga_brief_locked()||saga_coda_locked();}
static const char *saga_brief_line(const SagaBeat *b,int beat){
 if(!b)return "";
 switch(beat){
 case 0:return b->line;
 case 1:return b->talk2;
 case 2:return b->talk3;
 case 3:return b->talk4;
 case 4:return b->talk5;
 case 5:return b->talk6;
 case 6:return b->talk7;
 default:return b->talk8;
 }
}
static const char *saga_brief_reply(const SagaBeat *b,int beat){
 /* Chapter-authored asks — never answer a question the NPC line already covered. */
 if(!b)return "Continue.";
 switch(beat){
 case 0:return b->ask1;
 case 1:return b->ask2;
 case 2:return b->ask3;
 case 3:return b->ask4;
 case 4:return b->ask5;
 case 5:return b->ask6;
 case 6:return b->ask7;
 default:return b->ask8;
 }
}
static const char *prologue_brief_line1(int beat){
 /* Beat 0 = hook. Beats 1..4 answer the previous ask. Beat 5 reinforces accept. */
 static const char *a[PROLOGUE_BRIEF_BEATS]={
  "Ryn Vale is missing, and I need a pilot who can leave this berth and come back.",
  "The catch is simple: come back alive, with the ship still answering the tower.",
  "Ryn flew a ship like this one — patched, honest, and too willing to chase a quiet ping.",
  "Analog stick or D-pad, pick what feels true in your hands; the ship will forgive either.",
  "Launch, clear the station, fly out six hundred metres, then dock again on the same vector.",
  "First flight is only that checklist. Accept when you mean the return as much as the launch."};
 return a[beat>=0&&beat<PROLOGUE_BRIEF_BEATS?beat:PROLOGUE_BRIEF_BEATS-1];
}
static const char *prologue_brief_line2(int beat){
 static const char *b[PROLOGUE_BRIEF_BEATS]={
  "She missed three scheduled calls. Ryn skips meals and permits — never a call.",
  "I will not send you into combat yet. Throttle is distance over time, not courage.",
  "Borrow it. Learn its habits. Bring it home before we ask you for distances that matter.",
  "Select opens the deck when you need air. Venn would rather tow a boring pilot than a clever wreck.",
  "That return is the only skill that scales. Harbour badge is a receipt, not a medal.",
  "When you accept, we start looking for the sealed case Ryn left with Mara."};
 return b[beat>=0&&beat<PROLOGUE_BRIEF_BEATS?beat:PROLOGUE_BRIEF_BEATS-1];
}
static const char *prologue_brief_reply(int beat){
 /* Ask is chosen on this beat; NPC answer arrives only after the echo beat. */
 static const char *r[PROLOGUE_BRIEF_BEATS]={
  "What is the catch?",
  "Tell me about Ryn's ship.",
  "How do the controls work?",
  "Walk me through first flight.",
  "I am ready when you are.",
  "Accept first flight"};
 return r[beat>=0&&beat<PROLOGUE_BRIEF_BEATS?beat:PROLOGUE_BRIEF_BEATS-1];
}
static int prologue_brief_needs_echo(int beat){return beat>=0&&beat<PROLOGUE_BRIEF_BEATS-1;}
static int saga_brief_needs_echo(int beat){return beat>=0&&beat<SAGA_BRIEF_BEATS-1;}
