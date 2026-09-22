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
 text(3,y,row==index?WHITE:DIM,"%s %.52s",row==index?">":" ",label);
}
static void narrative_reply_choice(int index,int y,const char *label){
 int active=row==index,py=y*8-3;unsigned edge=active?RGB(245,157,62):RGB(119,71,38),fill=active?RGB(62,36,24):RGB(28,24,23);
 rect(16,py,448,14,fill);rect(16,py,448,1,edge);rect(16,py+13,448,1,edge);rect(16,py,2,14,edge);rect(462,py,2,14,edge);
 /* The right-hand tail marks this as the commander's side of the exchange. */
 line(464,py+4,470,py+7,edge);line(470,py+7,464,py+10,edge);
 text(3,y,active?GOLD:AMBDIM,"%s YOU: %.45s",active?">":" ",label);
}
static void kei_speech_bubble(int y,const char *line1,const char *line2,int expression){
 const int px=16,size=48,bx=76,bw=388,bh=62;unsigned edge=RGB(76,181,190),fill=RGB(14,29,39);
 draw_kei(px,y+7,size,expression);
 rect(bx,y,bw,bh,fill);rect(bx,y,bw,2,edge);rect(bx,y+bh-2,bw,2,RGB(30,78,86));rect(bx+bw-2,y,2,bh,edge);
 /* A compact pixel tail physically links these words to Kei's portrait. */
 line(bx,y+20,bx-12,y+28,edge);line(bx-12,y+28,bx,y+36,edge);rect(bx-3,y+22,4,13,fill);
 text(11,y/8+1,CYAN,"KEI SAYS");
 text(11,y/8+3,WHITE,"%.43s",line1);
 if(line2&&line2[0])text(11,y/8+5,WHITE,"%.43s",line2);
}
static void narrative_footer(void){footer("UP/DOWN CHOOSE   X SELECT   O BACK");}
