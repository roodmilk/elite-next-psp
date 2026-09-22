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
static void narrative_choice(int index,int y,const char *label){
 if(row==index)selected_span(y,464);
 text(3,y,row==index?WHITE:DIM,"%s %s",row==index?">":" ",label);
}
static void narrative_reply_choice(int index,int y,const char *label){
 int active=row==index,py=y*8-3;unsigned edge=active?RGB(245,157,62):RGB(119,71,38),fill=active?RGB(62,36,24):RGB(28,24,23);
 rect(16,py,448,14,fill);rect(16,py,448,1,edge);rect(16,py+13,448,1,edge);rect(16,py,2,14,edge);rect(462,py,2,14,edge);
 /* The right-hand tail marks this as the commander's side of the exchange. */
 line(464,py+4,470,py+7,edge);line(470,py+7,464,py+10,edge);
 text(3,y,active?GOLD:AMBDIM,"%s YOU: %s",active?">":" ",label);
}
static int saga_speaker_role(const SagaBeat *b){
 if(!b)return EXPLORERS;
 if(b->role==3)return LAW;
 if(b->role==2)return TRADERS;
 return EXPLORERS;
}
static unsigned saga_speaker_color(const SagaBeat *b){
 int role=saga_speaker_role(b);
 if(b&&(!strcmp(b->speaker,"KEI")||!strcmp(b->speaker,"RYN")))return RGB(76,181,190);
 return faction_colors[role];
}
static void saga_speaker_face(int x,int y,int size,const SagaBeat *b){
 if(!b){draw_kei(x,y,size,0);return;}
 if(!strcmp(b->speaker,"KEI")||!strcmp(b->speaker,"RYN")){draw_kei(x,y,size,!strcmp(b->speaker,"RYN")?1:0);return;}
 int role=saga_speaker_role(b);unsigned seed=0;
 for(const char *p=b->speaker;*p;p++)seed=seed*131u+(unsigned char)*p;
 draw_portrait(x,y,size,size,(int)(seed%2000)+role*37,role);
}
static void kei_speech_bubble(int y,const char *line1,const char *line2,int expression){
 const int px=16,size=48,bx=76,bw=388,bh=62;unsigned edge=RGB(76,181,190),fill=RGB(14,29,39);
 draw_kei(px,y+7,size,expression);
 rect(bx,y,bw,bh,fill);rect(bx,y,bw,2,edge);rect(bx,y+bh-2,bw,2,RGB(30,78,86));rect(bx+bw-2,y,2,bh,edge);
 /* A compact pixel tail physically links these words to Kei's portrait. */
 line(bx,y+20,bx-12,y+28,edge);line(bx-12,y+28,bx,y+36,edge);rect(bx-3,y+22,4,13,fill);
 speaker_name_tag(11,y/8+1,"KEI",edge);
 /* Wrap into the bubble width — never hard-truncate mid-sentence. */
 int col=11,cap=((bx+bw-8)/8)-col,row=y/8+3;
 const char *left=0;
 if(line1&&line1[0])row+=text_wrap(col,row,cap,2,WHITE,line1,&left);
 if(line2&&line2[0]&&row<=y/8+5)text_wrap(col,row,cap,y/8+6-row,WHITE,line2,0);
}
static void narrative_footer(void){footer("UP/DOWN CHOOSE   X SELECT   O BACK");}
enum { PROLOGUE_BRIEF_BEATS = 6 };
static int saga_brief_beat=0,saga_brief_chapter=-1,prologue_brief_beat=0;
static void saga_brief_reset(int chapter){if(saga_brief_chapter!=chapter){saga_brief_chapter=chapter;saga_brief_beat=0;}}
/* Locked until the player finishes every beat and accepts the next step. */
static int saga_brief_locked(void){return game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT&&!game.saga_step;}
static int prologue_brief_locked(void){return tracked_mission==0&&game.campaign_stage==0&&game.system==7&&game.docked;}
static int story_brief_locked(void){return prologue_brief_locked()||saga_brief_locked();}
static const char *saga_brief_line(const SagaBeat *b,int beat){
 if(!b)return "";
 switch(beat){
 case 0:return b->line;
 case 1:return b->talk2;
 case 2:return b->talk3;
 case 3:return b->talk4;
 case 4:return b->talk5;
 default:return b->talk6;
 }
}
static const char *saga_brief_reply(int beat){
 /* Reply is the commander's next ask/ack — never the answer to the line on screen. */
 static const char *r[SAGA_BRIEF_BEATS]={"What happened?","Go on","I understand","What do you need?","Confirm the next step","Accept next step"};
 return r[beat>=0&&beat<SAGA_BRIEF_BEATS?beat:SAGA_BRIEF_BEATS-1];
}
static const char *prologue_brief_line1(int beat){
 static const char *a[PROLOGUE_BRIEF_BEATS]={
  "Ryn is missing. Help me find her.","The catch is simple: come back alive.","Ryn flew a ship like this one.","Analog or D-pad - pick what feels true.","Launch, clear the station, then dock again.","First flight: launch, fly, return to Lave Hub."};
 return a[beat>=0&&beat<PROLOGUE_BRIEF_BEATS?beat:PROLOGUE_BRIEF_BEATS-1];
}
static const char *prologue_brief_line2(int beat){
 static const char *b[PROLOGUE_BRIEF_BEATS]={
  "She missed three calls. That is not like her.","I will not send you into combat yet.","Borrow it. Learn its habits. Bring it home.","Select opens the deck when you need air.","That proves you can carry the next job.","Accept when you are ready to begin."};
 return b[beat>=0&&beat<PROLOGUE_BRIEF_BEATS?beat:PROLOGUE_BRIEF_BEATS-1];
}
static const char *prologue_brief_reply(int beat){
 /* Ask first, then hear the answer on the next beat. Final beat is accept. */
 static const char *r[PROLOGUE_BRIEF_BEATS]={"What is the catch?","Tell me about Ryn's ship.","How do the controls work?","Confirm first flight","I'm ready to accept","Accept first flight"};
 return r[beat>=0&&beat<PROLOGUE_BRIEF_BEATS?beat:PROLOGUE_BRIEF_BEATS-1];
}
