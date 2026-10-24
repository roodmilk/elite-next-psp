static int contact_ids[TARGET_CAPACITY],contact_count;
static void contacts_refresh(void){contact_count=0;for(int i=0;i<=FLIGHT_TARGET_MAX;i++)if(valid_target(i))contact_ids[contact_count++]=i;}
/* Instrument panel — soft charcoal fill, cream/slate rules. No gold corner brackets (debug look). */
static void panel(int x,int y,int w,int h){rect(x,y,w,h,UI_PANEL);rect(x,y,w,1,UI_TEXT);rect(x,y+h-1,w,1,UI_RAISED);rect(x,y,1,h,RGB(90,96,76));rect(x+w-1,y,1,h,UI_RAISED);rect(x+1,y+1,w-2,1,UI_RAISED);}

#include "deck-ui.h"
/* Shared native-resolution adventure chrome. Text and art keep their own
 * reserved regions; solid fills remain readable over any scene. */
static void adventure_panel(int x,int y,int w,int h,int reply){
 unsigned edge=reply?RGB(211,141,76):RGB(94,157,174);
 rect(x,y,w,h,RGB(9,17,25));
 rect(x,y,w,1,edge);rect(x,y+h-1,w,1,RGB(43,64,77));
 rect(x,y,1,h,edge);rect(x+w-1,y,1,h,RGB(43,64,77));
 rect(x+2,y+2,w-4,1,RGB(28,43,55));
 rect(x+4,y+5,2,h-10,edge);
}
#include "opening-art.h"
#include "cargo-console.h"
static int target_filter=0,target_ids[TARGET_CAPACITY],target_count=0,target_details=0;
static int target_matches(int id){if(id==PLAYER_TARGET_ID)return target_filter==0||target_filter==1;if(target_filter==0)return 1;if(target_filter==1)return IS_NPC_ID(id);if(target_filter==2)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].target==-2;if(target_filter==3)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==LAW;if(target_filter==4)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==TRADERS;if(target_filter==5)return is_mission_target(&game,id);if(target_filter==6)return id>0&&id<=BODY_COUNT;if(target_filter==7)return IS_STATION_ID(id);if(target_filter==8)return IS_DEBRIS_ID(id);if(target_filter==9)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].freighter;return IS_ANOMALY_ID(id);}
static void target_refresh(void){target_count=0;for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(valid_target(id)&&target_matches(id))target_ids[target_count++]=id;if(row>=target_count)row=0;}
static unsigned contact_color(int id){if(id==PLAYER_TARGET_ID)return CYAN;if(id==ROUTE_TARGET_ID)return CYAN;if(is_mission_target(&game,id))return WHITE;if(IS_NPC_ID(id))return faction_colors[game.npc[id-BODY_COUNT-1].role];if(IS_ANOMALY_ID(id))return UI_GOLD;if(IS_DEBRIS_ID(id))return DIM;return IS_STATION_ID(id)?UI_CYAN:UI_GOLD;}
static const char *target_status(int id){if(id==RUMOUR_TARGET_ID)return "SCAN LOCALLY";if(id==WHALE_TARGET_ID)return scanner_known(id)?"OBSERVED POD":"TRI TO OBSERVE";if(IS_DISCOVERY_ID(id))return "DISCOVERY";if(id==PLAYER_TARGET_ID)return wifi_contact_status();if(id==ROUTE_TARGET_ID)return "PLOTTED HYPERSPACE VECTOR";if(is_mission_target(&game,id))return "MISSION TARGET";if(IS_ANOMALY_ID(id))return game.anomaly[id-ANOMALY_ID_MIN].scanned?"CODEX ENTRY":"UNSCANNED ANOMALY";if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].rock?"LASER-MINEABLE ROCK":game.debris[id-DEBRIS_ID_MIN].wreck?"SALVAGE WRECK":"COLLECTABLE CARGO";if(IS_STATION_ID(id))return station_target_hub(id)==1?"OUTER RELAY / DOCKABLE":station_target_hub(id)==2?"OUTPOST / DOCKABLE":station_class_name(station_class(&game));if(id<=BODY_COUNT){Body *b=&game.bodies[id-1];return b->type==SUN?"STAR / NO LANDING":b->type==GAS?"FLOATING SKYPORT":"APPROACHABLE";}NPC *n=&game.npc[id-BODY_COUNT-1];if(n->target==-2)return "HOSTILE";if(n->role==PIRATES)return "WANTED";if(n->role==LAW)return "LAW";if(n->role==TRADERS)return n->freighter?freight_status(n):"TRADER";return "NEUTRAL";}
/* Faint CRT glass under targeting list — scanlines + sparse fuzz. Drawn before glyphs so text stays sharp. */
static void targeting_crt_glass(int x,int y,int w,int h){
 unsigned base=UI_PANEL,scan=ui_paint_surface(RGB(17,23,31)),fleck_lo=ui_paint_surface(RGB(14,19,26)),fleck_hi=ui_paint_surface(RGB(28,36,48));
 unsigned seed=((unsigned)(game.time*29.f)^0xA71Cu)*1664525u+1013904223u;
 for(int yy=y+2;yy<y+h-2;yy+=2){
  for(int xx=x+2;xx<x+w-2;xx++)if(fb[yy*STRIDE+xx]==base)fb[yy*STRIDE+xx]=scan;
 }
 int flecks=22;
 for(int i=0;i<flecks;i++){
  seed=seed*1664525u+1013904223u;int px=x+2+(int)((seed>>8)%(unsigned)(w-4));
  seed=seed*1664525u+1013904223u;int py=y+2+(int)((seed>>8)%(unsigned)(h-4));
  unsigned cur=fb[py*STRIDE+px];
  if(cur!=base&&cur!=scan)continue;
 fb[py*STRIDE+px]=(seed&7)==0?fleck_hi:fleck_lo;
 }
}
static void targeting_bezel_stickers(void){
 /* Tiny crew decals live on the physical lower bezel, never in the scan area. */
 rect(18,181,18,7,RGB(48,70,74));circle(27,184,3,RGB(120,224,194));line(27,181,27,187,RGB(120,224,194));line(24,184,30,184,RGB(120,224,194));
 rect(45,181,28,7,RGB(74,46,57));pixel(51,183,RGB(250,138,158));pixel(55,183,RGB(250,138,158));rect(50,184,7,2,RGB(250,138,158));pixel(51,186,RGB(250,138,158));pixel(56,186,RGB(250,138,158));
 rect(409,181,35,7,RGB(63,53,37));pixel(416,184,UI_GOLD);pixel(418,182,UI_GOLD);pixel(420,184,UI_GOLD);pixel(418,186,UI_GOLD);
 }
static void targeting_scope_stamp(int x,int y,int id){
 unsigned frame=RGB(90,96,76),glass=RGB(10,20,29),dim=RGB(55,101,111),ink=valid_target(id)?contact_color(id):DIM;
 rect(x,y,50,30,glass);rect(x,y,50,1,frame);rect(x+49,y,1,30,frame);rect(x,y+29,50,1,UI_RAISED);
 line(x+24,y+4,x+24,y+25,dim);line(x+12,y+14,x+37,y+14,dim);line(x+19,y+9,x+29,y+9,dim);line(x+19,y+19,x+29,y+19,dim);
 pixel(x+24,y+14,ink);line(x+22,y+12,x+26,y+12,ink);line(x+22,y+16,x+26,y+16,ink);
 int sweep=3+(int)(preview_time*9.f)%42;rect(x+sweep,y+2,1,26,RGB(28,66,77));
 if(valid_target(id)){int pulse=(int)(preview_time*6.f)&1;pixel(x+24-pulse,y+14,ink);pixel(x+24+pulse,y+14,ink);}
}
static void targeting_screen(void){
 target_count=collect_scan_ids(target_ids,scan_cat);if(row>=target_count)row=0;
 header("TARGET COMPUTER");
 /* Native 480x272 monitor layout: a readable six-row scan list and a
  * dedicated 184px readout.  Keep every label inside the 8px safe grid so
  * the targeting page never collides with the bezel or footer. */
 panel(8,32,272,156);panel(288,32,184,156);
 text(3,5,UI_CYAN,"CONTACTS");
 for(int i=0;i<5;i++)text(11+i*5,5,i==scan_cat?UI_GOLD:DIM,"%.4s",scan_cat_names[i]);
 if(!target_count){text(3,9,DIM,"NO CONTACTS IN THIS BAND");text(3,12,WHITE,"L/R CHANGE SCAN BAND");}
 else {
  int first=(row/6)*6;
  for(int i=0;i<6&&first+i<target_count;i++){
   int idx=first+i,id=target_ids[idx],yy=7+i*2;
   if(idx==row)rect(10,yy*8-2,264,13,RGB(25,65,77));
   unsigned ink=idx==row?WHITE:contact_color(id);
   const char *label=scanner_known(id)?target_name(id):"UNKNOWN CONTACT";
   text(3,yy,ink,"%c %-18.18s",idx==row?'>':' ',label);
   char range[8];target_range_label(length(sub(target_position(id),game.pos)),range,sizeof(range));text(26,yy,DIM,"%5s",range);
  }
  if(target_count>6){int pages=(target_count+5)/6;text(3,21,DIM,"PAGE %d/%d",row/6+1,pages);}
  selected_target=target_ids[row];
  int id=selected_target;
  text(37,5,contact_color(id),"%.18s",id==RUMOUR_TARGET_ID?"RUMOUR AREA":scanner_known(id)?target_name(id):"UNKNOWN CONTACT");
  text(37,7,DIM,"%.22s",target_status(id));
  targeting_scope_stamp(414,42,id);
  if(id==PLAYER_TARGET_ID&&!wifi_contact_local())text(37,11,WHITE,"FOLLOW SYSTEM ROUTE");else {char range[12];target_range_label(length(sub(target_position(id),game.pos)),range,sizeof(range));text(37,11,WHITE,"RANGE %s",range);}
  if(IS_NPC_ID(id)&&scanner_known(id)){
   NPC *n=&game.npc[id-BODY_COUNT-1];
   text(37,13,DIM,"FACTION %s",n->role==LAW?"LAW":n->role==PIRATES?"PIRATES":n->role==TRADERS?"TRADERS":n->role==EXPLORERS?"EXPLORERS":"NEUTRAL");
   target_condition(296,128,id);
   text_px(296,160,UI_CYAN,"%.21s",traffic_activity(n));
  } else text(37,13,DIM,"CLASS %s",id==RUMOUR_TARGET_ID?"SEARCH":id==WHALE_TARGET_ID?"FAUNA":IS_DISCOVERY_ID(id)?"DISCOVERY":id==PLAYER_TARGET_ID?"FRIEND":id==ROUTE_TARGET_ID?"NAV STAR":IS_STATION_ID(id)?"STATION":id<=BODY_COUNT?"CELESTIAL":IS_DEBRIS_ID(id)?"DEBRIS":"ANOMALY");
  if(target_details){
   text(37,17,UI_GOLD,"DETAILS");
   text(37,19,WHITE,IS_NPC_ID(id)?"TRI SCAN / IDENTIFY":"TRI INSPECT TARGET");
  }
 }
 targeting_bezel_stickers();
 footer("UP/DOWN SELECT   L/R BAND   X LOCK   TRI DETAILS   O BACK");}
static void local_system(void){
 contacts_refresh();if(row>=contact_count)row=0;header("SYSTEM / ALL CONTACTS");int first=row/8*8;panel(8,32,464,156);text(3,5,UI_CYAN,"NAME                            DISTANCE");
  for(int j=0;j<8&&first+j<contact_count;j++){int id=contact_ids[first+j],y=7+j*2;if(first+j==row)selected(y);const char *kind=IS_NPC_ID(id)?"SHIP":IS_ANOMALY_ID(id)?"ECHO":IS_DEBRIS_ID(id)?"LOOT":"NAV ";text(3,y,contact_color(id),"%-24s %s %7d M",scanner_known(id)?target_name(id):"UNKNOWN CONTACT",kind,(int)length(sub(target_position(id),game.pos)));}
 page_number_at(52,5,row/8+1,(contact_count+7)/8);footer("UP/DOWN   X LOCK   TRI AUTO-AIM   O BACK");
}
static void debug_screen(void){header((game.debug_flags&DEBUG_MODIFIED)?"DEBUG TOOLS / MODIFIED":"DEBUG TOOLS");const char *items[]={"Add 10,000 units","Add 100,000 units","Refill fuel and shields","Clear local wanted","Add local wanted","Move to station approach","Move near targeted planet","Return to station","Enter planet atmosphere","Install pulse laser","Reveal local Codex","Cool ship and clear heat","Install Heat Buffer","Force Thargoid encounters","Unlimited fuel","Unlimited jump range"};for(int i=0;i<16;i++){const char *state=i==13?(debug_force_thargoid?": ON":": OFF"):i==14?((game.debug_flags&DEBUG_UNLIMITED_FUEL)?": ON":": OFF"):i==15?((game.debug_flags&DEBUG_UNLIMITED_RANGE)?": ON":": OFF"):"";if(row==i)selected(3+i);text(3,3+i,row==i?WHITE:DIM,"%s%s",items[i],state);}footer("UP/DOWN   X APPLY   O BACK");}
static void debug_action(void){
 game.debug_flags|=DEBUG_MODIFIED;
 if(row==0){debug_add_units(&game,10000);game.cue=SFX_UI;}
 if(row==1){debug_add_units(&game,100000);game.cue=SFX_UI;}
 if(row==2){game.fuel=player_ships[game.ship].range;game.energy=100;message(&game,"Fuel and shields full.");}
 if(row==3){game.legal=game.wanted[game.system]=0;game.crime_record[game.system]=0;game.police_cargo_heat=0;message(&game,"Local warrant cleared.");}
 if(row==4){add_crime(&game,5);message(&game,"Local wanted level raised.");}
 if(row==5){game.docked=0;game.pos=(Vec3){0,0,3000};game.speed=0;game.yaw=game.pitch=game.roll=0;selected_target=0;autoaim=0;change_page(FLIGHT);}
 if(row==6){if(selected_target<2||selected_target>BODY_COUNT){message(&game,"Select a planet in Contacts first.");return;}Body *b=&game.bodies[selected_target-1];game.docked=0;game.pos=add(b->pos,(Vec3){0,0,-b->radius-800});game.speed=0;game.yaw=game.pitch=game.roll=0;autoaim=0;change_page(FLIGHT);}
 if(row==7){game.planet=game.approach=-1;game.surface=0;game.docked=1;game.speed=0;game.boost=0;game.dock_stage=game.dock_phase=0;game.dock_timer=game.dock_duration=0;game.yaw=game.pitch=game.roll=0;autoaim=0;hard_brake=0;game.pos=(Vec3){0,0,3200};change_page(HOME);message(&game,"Docked at the local station.");}
 if(row==8){if(selected_target<2||selected_target>BODY_COUNT){message(&game,"Select a planet in Contacts first.");return;}Body *b=&game.bodies[selected_target-1];if(b->type==SUN){message(&game,"Suns have no atmosphere flight.");return;}game.docked=0;game.pos=add(b->pos,(Vec3){0,0,-b->radius-800});game.yaw=game.pitch=game.roll=0;game.approach=selected_target-1;if(enter_planet(&game)){autoaim=0;change_page(FLIGHT);}else message(&game,"Could not enter atmosphere.");}
 if(row==9){game.laser=1;game.upgrades|=4;message(&game,"Pulse laser installed in WPN slot.");}
 if(row==10){game.discoveries=256;message(&game,"Local Codex records revealed.");}
 if(row==11){game.heat=0;game.energy=100;message(&game,"Heat dumped and shields restored.");}
 if(row==12){game.fit[FIT_UTIL]=24;fit_rebuild(&game);message(&game,"Heat Buffer installed in UTIL slot.");}
 if(row==13){debug_force_thargoid=!debug_force_thargoid;game.cue=SFX_UI;message(&game,debug_force_thargoid?"Forced Thargoid encounters enabled. Every jump will be intercepted.":"Forced Thargoid encounters disabled. Normal encounter chance restored.");}
 if(row==14){game.debug_flags^=DEBUG_UNLIMITED_FUEL;if(game.debug_flags&DEBUG_UNLIMITED_FUEL)game.fuel=(float)player_ships[game.ship].range;game.cue=SFX_UI;message(&game,(game.debug_flags&DEBUG_UNLIMITED_FUEL)?"Unlimited fuel enabled. Boost and jumps use no fuel.":"Unlimited fuel disabled. Normal fuel use restored.");}
 if(row==15){game.debug_flags^=DEBUG_UNLIMITED_RANGE;game.cue=SFX_UI;message(&game,(game.debug_flags&DEBUG_UNLIMITED_RANGE)?"Unlimited jump range enabled. Any system can be one jump away.":"Unlimited jump range disabled. Fitted drive range restored.");}
}
static void body_tint(int sys,int i,unsigned *col,unsigned *acc,int *type){
 /* Keep in lockstep with sectors.h sun/world palettes so every view matches flight. */
 const unsigned suns[]={0x80dfff,0x66c8ff,0x526eff,0xf4f4ff,0xffc88a,0xc88cff,0x9ee8ff,0xd8e8ff,0xffa060,0xb0ffe0};
 const unsigned worlds[]={0xc97535,0x91b45c,0x8763b5,0x7ebfc4,0xb87775,0xadc2ce,0xd4a574,0x5a8f6a,0x6b5b95,0xc45c5c,0x3d7a5a,0xd0a040,0x5a90c0,0xa05070,0x708050};
 unsigned h=body_art_seed(sys,i);*type=system_body_type_slot(sys,i);*col=*type==SUN?suns[h%10]:worlds[(h>>8)%15];*acc=worlds[(h>>16)%15];
 if(sys==7&&i==1){*type=OCEAN;*col=0xc35f23;*acc=0x4b9137;}
}
static const char *codex_body_name(int sys,int body){
 static char name[32];
 if(sys==game.system&&body>=0&&body<BODY_COUNT)return game.bodies[body].name;
 if(system_body_type_slot(sys,body)==SUN)snprintf(name,sizeof(name),"%s SUN %c",game.systems[sys].name,'A'+body);
 else snprintf(name,sizeof(name),"%s %d",game.systems[sys].name,body+1);
 return name;
}
static void chart_system_preview(int dest){
 int count=system_body_slots(dest);for(int i=0;i<count;i++){unsigned col,acc;int type;body_tint(dest,i,&col,&acc,&type);int x=344+(i%5)*24,y=56+(i/5)*39,r=type==SUN?10:type==GAS?8:5;draw_planet_disc(x,y,r,col,acc,body_art_seed(dest,i),type);}
}
static void galaxy_xy(int id,int *x,int *y){chart_project(id,x,y,0);}
static void chart_ellipse(int cx,int cy,int rx,int ry,unsigned ink){
 int lx=cx+rx,ly=cy;for(int i=1;i<=48;i++){float a=i*6.2831853f/48.f;int x=cx+(int)(cosf(a)*rx),y=cy+(int)(sinf(a)*ry);line(lx,ly,x,y,ink);lx=x;ly=y;}
}
#include "deep-chart-art.h"
/* A single cached breadth-first traversal supplies both the preview route and
 * the selected system's jump count. No full Game copies on the PSP stack. */
static int chart_parents[256],chart_hops[256],chart_path[256],chart_path_n;
static void chart_routes_prepare(void){
 static int source=-1,hull=-1,unlimited=-1;static float range=-1;
 float fitted=(float)player_ships[game.ship].range*.1f;
 int debug_unlimited=(game.debug_flags&DEBUG_UNLIMITED_RANGE)!=0;
 if(source==game.system&&hull==game.ship&&range==fitted&&unlimited==debug_unlimited)return;
 int queue[256],head=0,tail=0;
 for(int i=0;i<256;i++){chart_parents[i]=-1;chart_hops[i]=-1;}
 chart_parents[game.system]=game.system;chart_hops[game.system]=0;queue[tail++]=game.system;
 if(debug_unlimited){for(int i=0;i<256;i++)if(i!=game.system){chart_parents[i]=game.system;chart_hops[i]=1;}source=game.system;hull=game.ship;range=fitted;unlimited=debug_unlimited;return;}
 while(head<tail){
  int from=queue[head++];
  for(int i=0;i<256;i++)if(chart_parents[i]<0&&distance_ly(&game,from,i)<=fitted+.001f){
   chart_parents[i]=from;chart_hops[i]=chart_hops[from]+1;queue[tail++]=i;
  }
 }
 source=game.system;hull=game.ship;range=fitted;unlimited=debug_unlimited;
}
static int chart_route_contains(int system){
 chart_routes_prepare();int goal=game.route_goal;
 if(goal<0||goal>=256||chart_hops[goal]<0)return 0;
 for(int at=goal,n=0;at>=0&&n<256;n++,at=chart_parents[at]){
  if(at==system)return 1;if(at==game.system)break;
 }
 return 0;
}
static int galaxy_route_draw(int goal,unsigned ink){
 chart_routes_prepare();chart_path_n=0;
 if(goal<0||goal>=256||chart_hops[goal]<0)return -1;
 for(int at=goal;chart_path_n<256;at=chart_parents[at]){chart_path[chart_path_n++]=at;if(at==game.system)break;}
 for(int i=1;i<chart_path_n;i++){
  int ax,ay,bx,by;galaxy_xy(chart_path[i-1],&ax,&ay);galaxy_xy(chart_path[i],&bx,&by);
  unsigned edge=chart_lerp(RGB(2,8,17),ink,38);
  line(ax,ay-1,bx,by-1,edge);line(ax,ay+1,bx,by+1,edge);line(ax,ay,bx,by,ink);
 }
 for(int i=0;i<chart_path_n;i++){int x,y;galaxy_xy(chart_path[i],&x,&y);chart_halo(x,y,6,ink);rect(x-1,y-1,3,3,RGB(182,245,255));pixel(x,y,WHITE);}
 return chart_hops[goal];
}
static void chart_search_overlay(void){
 static const char keys[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 chart_glass(30,35,420,190,UI_SIGNAL);chart_text(46,44,UI_SIGNAL,"SYSTEM SEARCH");
 rect(46,59,184,21,UI_PANEL);rect(46,59,184,1,UI_EDGE);chart_text(54,65,UI_TEXT,"%s",chart_query[0]?chart_query:"TYPE A PREFIX...");
 chart_text(46,85,UI_MUTED,"ON-SCREEN INPUT");
 for(int i=0;i<28;i++){
  int x=46+(i%7)*25,y=100+(i/7)*23,sel=i==chart_search_key;
  if(sel){rect(x-2,y-3,23,19,UI_RAISED);rect(x-2,y-3,2,19,UI_SIGNAL);}
  if(i<26)chart_text(x+5,y,sel?UI_TEXT:UI_MUTED,"%c",keys[i]);
  else chart_text(x+(i==26?0:3),y,sel?WHITE:UI_TEXT,i==26?"DEL":"GO");
 }
 rect(238,51,1,156,UI_EDGE);chart_text(254,44,UI_ACCENT,"PREFIX MATCHES");
 int first=chart_search_match-2;if(first<0)first=0;if(first+6>chart_match_n)first=chart_match_n-6;if(first<0)first=0;
 if(!chart_match_n)chart_text(254,69,RED,"NO MATCHING SYSTEMS");
 for(int j=0;j<6&&first+j<chart_match_n;j++){
  int index=first+j,id=chart_matches[index],y=66+j*23;
  if(index==chart_search_match){rect(250,y-4,184,19,UI_RAISED);rect(250,y-4,2,19,UI_SIGNAL);}
  chart_text(258,y,index==chart_search_match?UI_TEXT:UI_MUTED,"%.11s",game.systems[id].name);
  chart_text(356,y,index==chart_search_match?UI_ACCENT:UI_MUTED,"%4.1f LY",distance_ly(&game,game.system,id));
 }
 chart_text(254,205,DIM,"L/R CHANGE MATCH");
 chart_glass(2,248,476,22,UI_EDGE);
 rect(13,254,3,11,UI_TEXT);rect(9,258,11,3,UI_TEXT);chart_text(26,256,UI_TEXT,"TYPE");
 button_icon(78,254,'X',RGB(97,190,255));chart_text(94,256,UI_TEXT,"KEY");
 button_icon(140,254,'S',RGB(255,114,213));chart_text(156,256,UI_TEXT,"DEL");
 button_icon(196,254,'l',UI_TEXT);button_icon(213,254,'r',UI_TEXT);chart_text(230,256,UI_TEXT,"MATCH");
 button_icon(284,254,'T',RGB(105,230,174));chart_text(300,256,UI_TEXT,"SELECT");
 button_icon(386,254,'O',RGB(255,125,151));chart_text(402,256,UI_TEXT,"CLOSE");
}
static void galaxy_overview(void){
 const unsigned cyan=UI_SIGNAL,pink=UI_ACCENT,gold=UI_GOLD;
 static const char *filters[]={"ALL","VISITED","UNVISITED","RICH","POOR","MEGA","JOBS","IN RANGE","ROUTE"};
 preview_reset();rect(0,0,W,H,RGB(2,5,11));chart_galaxy_background();
 clipx0=2;clipx1=478;clipy0=25;clipy1=246;
 int gcx=250+(int)chart_pan_x,gcy=144+(int)chart_pan_y;
 /* Faint continuous survey ellipses read as structure, never isolated dots. */
 chart_ellipse(gcx,gcy,(int)(199*chart_zoom),(int)(74*chart_zoom),chart_lerp(UI_PANEL,UI_SIGNAL,68));
 chart_ellipse(gcx,gcy,(int)(135*chart_zoom),(int)(50*chart_zoom),chart_lerp(UI_PANEL,UI_SIGNAL,40));
 int shown=0,matching=0;
 for(int i=0;i<256;i++){
  int match=chart_filter_matches(i);matching+=match;
  if(!match&&i!=game.system&&i!=chart_cursor&&i!=game.route_goal)continue;
  int x,y;float depth;chart_project(i,&x,&y,&depth);
  if(x<4||x>475||y<28||y>240)continue;
  if((x<100&&y<120)||(x>342&&y<124))continue;
  shown++;
  int kind=station_class_for_system(&game,i),visited=(game.visited[i>>3]&(1<<(i&7)))!=0;
  unsigned ink=kind==STATION_MEGA?RGB(206,119,201):kind==STATION_POOR?RGB(107,146,176):RGB(111,198,236);
  if(visited)ink=chart_lerp(ink,WHITE,105);
  if(i!=chart_cursor&&i!=game.system){
   chart_halo(x,y,kind==STATION_MEGA?4:3,chart_lerp(RGB(0,0,0),ink,80));
   rect(x,y,2,2,ink);
  }
 }
 /* Retain a subdued committed route while previewing the highlighted star. */
 if(chart_filter!=CHART_ROUTE&&game.route_goal>=0&&game.route_goal!=chart_cursor)galaxy_route_draw(game.route_goal,chart_lerp(UI_PANEL,UI_SIGNAL,104));
 galaxy_route_draw(chart_filter==CHART_ROUTE?game.route_goal:chart_cursor,cyan);
 int jumps=chart_hops[chart_cursor];
 int x,y;galaxy_xy(game.system,&x,&y);chart_glow(x,y,gold,0);
 if(chart_cursor!=game.system){galaxy_xy(chart_cursor,&x,&y);chart_glow(x,y,pink,1);
  line(x-7,y-7,x-4,y-7,pink);line(x-7,y-7,x-7,y-4,pink);
  line(x+7,y+7,x+4,y+7,pink);line(x+7,y+7,x+7,y+4,pink);}
 chart_label_n=0;chart_label(chart_cursor,chart_cursor==game.system?gold:pink);
 if(chart_cursor!=game.system)chart_label(game.system,gold);
 /* One intermediate name gives the route context without filling the view. */
 if(chart_path_n>2)chart_label(chart_path[chart_path_n/2],UI_TEXT);
 preview_reset();
 chart_glass(2,2,476,22,UI_EDGE);
 chart_type(11,5,UI_TEXT,2,"GALAXY.MAPS");
 chart_glass(8,31,90,86,UI_EDGE);
 int first_filter=chart_filter_first();
 for(int row=0;row<5;row++){
  int i=first_filter+row,y=36+row*16;
  if(row)line(11,y-4,89,y-4,UI_EDGE);
  if(i==chart_filter){
   rect(11,y-3,79,15,UI_RAISED);line(11,y-3,89,y-3,cyan);line(11,y+11,89,y+11,cyan);
   rect(11,y-3,2,15,cyan);rect(88,y-3,2,15,cyan);
  }
  chart_text(16,y,i==chart_filter?cyan:UI_TEXT,"%s",filters[i]);
 }
 rect(93,35,2,77,UI_RAISED);rect(93,35+first_filter*35/(CHART_FILTERS-5),2,42,cyan);
 chart_glass(345,31,127,90,cyan);
 chart_text(353,41,chart_cursor==game.system?gold:pink,"%s",game.systems[chart_cursor].name);
 line(347,54,469,54,UI_EDGE);
 int kind=station_class_for_system(&game,chart_cursor);
 chart_text(354,62,UI_TEXT,"%s",kind==STATION_MEGA?"MEGA":kind==STATION_POOR?"POOR":"RICH");
 chart_text(354,78,UI_TEXT,"%.1f LY",distance_ly(&game,game.system,chart_cursor));
 if(chart_cursor==game.system)chart_text(354,101,gold,"CURRENT SYSTEM");
 else if(jumps>=0)chart_text(354,101,UI_TEXT,"%d JUMP%s",jumps,jumps==1?"":"S");
 else chart_text(354,101,RGB(239,150,134),"OUT OF RANGE");
 chart_glow(452,79,chart_cursor==game.system?gold:pink,1);
 /* Quiet bottom corners provide useful context without a banner. */
 rect(8,231,122,12,chart_lerp(UI_PANEL,RGB(2,5,11),90));
 if(!matching)chart_text(12,233,UI_MUTED,"%s",chart_filter==CHART_ROUTE?"NO ROUTE":chart_filter==CHART_JOBS?"NO JOBS":"NO MATCHES");
 else chart_text(12,233,UI_MUTED,"%d IN VIEW",shown);
 chart_text(389,233,UI_MUTED,"ZOOM %.1fX",chart_zoom);
 chart_glass(2,248,476,22,UI_EDGE);
 button_icon(10,254,'X',RGB(97,190,255));chart_text(26,256,RGB(218,232,242),"PLOT");
 button_icon(78,254,'S',pink);chart_text(94,256,RGB(218,232,242),"FILTER");
 button_icon(160,254,'l',RGB(155,193,218));button_icon(180,254,'r',RGB(155,193,218));chart_text(199,256,RGB(218,232,242),"ZOOM");
 circle(254,260,5,RGB(111,154,179));rect(252,258,5,5,RGB(151,187,207));chart_text(265,256,RGB(218,232,242),"PAN");
 button_icon(309,254,'T',RGB(105,230,174));chart_text(325,256,RGB(218,232,242),"SEARCH");
 button_icon(402,254,'O',RGB(255,125,151));chart_text(418,256,RGB(218,232,242),"BACK");
 if(chart_search)chart_search_overlay();
}
static void chart(void){
 galaxy_overview();
}
static int codex_tab=0,codex_scope=0,codex_system=-1,codex_body=0;
static const char *lore_categories[]={"ORIGINS","FRONTIER","TECHNOLOGY","PEOPLES","POWER","MYSTERIES"};
static const char *milky_way_topics[]={
 "THE MILKY WAY","SOL AND EARTH","THE FIRST LAUNCHES",
 "THE FRONTIER","COLONIAL WORLDS","PILOTS OF THE VOID",
 "HYPERSPACE","SHIPCRAFT","NAVIGATION AND FUEL",
 "ALIEN CIVILISATIONS","LANGUAGES OF THE STARS","LIFE ON STRANGE WORLDS",
 "TRADE AND POWER","LAW AT THE EDGE","THE GREAT HOUSES",
 "ANCIENT SIGNALS","THE UNKNOWN","THE FUTURE GALAXY"
};
static const char *milky_way_notes[]={
 "The Milky Way is a barred spiral of countless suns. Human charts show only a fraction of its arms; whole regions remain guesses, rumours and beautiful blank space.",
 "Sol is humanity's old home: birthplace, archive, political prize and warning about what crowded worlds can become. Its light still sits at the centre of every old story.",
 "The first jump routes were short, dangerous and celebrated like moon landings. Each new beacon turned a frightening distance into a place where someone might build a home.",
 "Beyond the prosperous core, maps become suggestions. Frontier pilots fly through thin law, old wreckage and sudden opportunity, where a good repair can matter more than a good speech.",
 "Colonies began as mines, outposts and research stations. Generations later they are cultures in their own right, carrying local customs, grudges and recipes between the stars.",
 "Couriers, miners, scouts, escorts, traders and explorers make the map feel close. Their small decisions carry news between systems and quietly turn into galactic history.",
 "Hyperspace turns distance into a route, never a free shortcut. Drive range, fuel, navigation data and safe arrival windows decide which worlds a pilot can reach alive.",
 "A ship is part machine, part promise. Reactors, shields, engines and weapons are tuned by crews who know that every bright warning light has a story behind it.",
 "Navigation is a discipline of margins. A commander watches the tank, the star, the route and the clock, then commits to a blue tunnel that has no room for doubt.",
 "Human records describe intelligences that do not fit familiar shapes. Some civilisations trade, some hide, and some leave only a pattern in a signal bank.",
 "Every world invents words for weather, distance and danger. Translators can carry a message across light-years, but they cannot always carry its meaning.",
 "Life adapts to salt seas, crystal caves, gas giant skies and airless rock. Surveyors catalogue it with care because the first name in a field guide tends to last.",
 "Markets bind distant systems together. Food, machines, metals, medicine and restricted cargo move along the same lanes, making prosperity and danger travel side by side.",
 "A station's charter, a governor's reach and a commander's reputation can matter more than any distant statute. Law travels unevenly, like radio through a storm.",
 "Dynasties, corporations, democracies and emergency regimes all claim to know what the galaxy needs. Their borders look tidy on a chart and blurry in real space.",
 "Ancient beacons, impossible echoes and ruins with no builders keep the old questions alive. The safest answer is often the one that leaves room for another expedition.",
 "The unknown is not empty. It is a signal not yet decoded, a world not yet landed on, a light that moves the wrong way, and a story waiting for a witness.",
 "The galaxy has no single ending written for it. Its future is made by ordinary crews choosing who to help, what to carry and which quiet signal deserves another look."
};
static int vis_count(void){int n=1;for(int s=0;s<256;s++)if(s!=game.system&&(game.visited[s>>3]&(1<<(s&7))))n++;return n;}
static int vis_sys(int idx){if(idx<=0)return game.system;int n=1;for(int s=0;s<256;s++)if(s!=game.system&&(game.visited[s>>3]&(1<<(s&7)))){if(n==idx)return s;n++;}return game.system;}
/* Planet records are created only by a completed landing. */
static int planet_log_count(void){int n=0;for(int s=0;s<256;s++)for(int b=1;b<BODY_COUNT;b++)if(game.landed_planets[s]&(1u<<(b-1)))n++;return n;}
static void planet_log_at(int idx,int *sys_out,int *body_out){
 int seen=0;for(int s=0;s<256;s++)for(int b=1;b<BODY_COUNT;b++)if(game.landed_planets[s]&(1u<<(b-1))){if(seen++==idx){*sys_out=s;*body_out=b;return;}}
 *sys_out=game.system;*body_out=1;
}
static int codex_system_star_count(int sys){int n=0;for(int b=0;b<system_body_slots(sys);b++)n+=system_body_type_slot(sys,b)==SUN;return n;}
static int codex_system_signal_row(int sys){return 1+codex_system_star_count(sys);}
/* Station + every charted stellar body. Landing controls the information
 * revealed by a planet, not whether its navigation record exists. */
static int codex_system_body_count(int sys){return 1+system_body_slots(sys);}
static int codex_system_body_at(int sys,int r){
 if(r==0)return 0;int stars=codex_system_star_count(sys),signal=1+stars;if(r==signal)return -1;
 int wanted=r<=stars?r-1:r-signal-1,seen=0,star=r<=stars;
 for(int b=0;b<system_body_slots(sys);b++)if((system_body_type_slot(sys,b)==SUN)==star)if(seen++==wanted)return b+1;
 return -1;
}
static int codex_system_row_for_body(int sys,int body){if(body==0)return 0;int total=codex_system_body_count(sys)+1;for(int r=1;r<total;r++)if(codex_system_body_at(sys,r)==body)return r;return 0;}
static int codex_system_row(void);
static int codex_kind_count(int tab){if(tab==0)return vis_count();if(tab==1)return planet_log_count();if(tab==2)return game.scanned_minerals;return game.scanned_anomalies;}
static int codex_rows(void);
static void lore_cover(int x,int y,int w,int h,int seed,int active){
 unsigned paper=active?RGB(235,208,147):RGB(182,161,116),ink=RGB(34,27,52),violet=RGB(72,34,112),blue=RGB(28,78,137),pink=RGB(190,55,116),orange=RGB(238,128,44);
 rect(x,y,w,h,ink);rect(x+3,y+3,w-6,h-6,paper);rect(x+7,y+7,w-14,h-14,RGB(12,18,48));
 for(int i=0;i<18;i++){int sx=x+10+(seed*13+i*19)%(w-20),sy=y+12+(seed*7+i*17)%(h-30);pixel(sx,sy,i&1?RGB(221,222,193):RGB(95,184,215));}
 fill_disc(x+w/2,y+48,27,blue);fill_disc(x+w/2+12,y+42,17,violet);circle(x+w/2,y+48,27,active?orange:pink);circle(x+w/2,y+48,22,RGB(23,45,91));
 if(seed%3==0){line(x+18,y+82,x+w-17,y+34,pink);line(x+14,y+91,x+w-10,y+43,orange);rect(x+17,y+79,8,5,orange);}
 else if(seed%3==1){line(x+16,y+76,x+w-14,y+76,orange);line(x+w/2,y+28,x+w/2,y+99,pink);circle(x+w/2,y+76,18,active?orange:pink);}
 else {rect(x+20,y+66,w-40,24,RGB(20,32,70));line(x+20,y+78,x+w-20,y+78,orange);line(x+20,y+66,x+w/2,y+45,pink);line(x+w/2,y+45,x+w-20,y+66,pink);}
 rect(x+9,y+h-29,w-18,1,active?orange:RGB(137,108,78));text((x/8)+1,(y+h-25)/8,active?RGB(250,226,161):RGB(215,195,148),"STAR ARCHIVE");
}
static void galactic_lore_screen(void){
 int topic=row,category=topic/3;unsigned bg=RGB(5,9,27),neb=RGB(20,18,58),ink=RGB(225,233,228),muted=RGB(133,158,181),accent=RGB(95,222,218),hot=RGB(246,156,65);
 rect(0,22,W,226,bg);rect(0,30,W,2,RGB(35,49,102));rect(0,226,W,2,RGB(86,48,106));
 for(int i=0;i<48;i++){int x=(i*83+topic*17)%W,y=34+(i*47+topic*9)%188;pixel(x,y,i%7==0?hot:(i%3==0?accent:RGB(74,102,154)));}
 rect(0,68,W,66,neb);rect(0,135,W,44,RGB(16,12,47));
 header("GALACTIC LORE / COSMIC ARCHIVE");panel(8,32,120,190);panel(136,32,336,190);
 text(2,5,hot,"ARCHIVE SECTIONS");
 for(int i=0;i<6;i++){int y=8+i*3;if(i==category){rect(10,y*8-2,116,17,RGB(40,42,92));rect(10,y*8-2,3,17,hot);}text(3,y,i==category?RGB(250,226,161):muted,"%s",lore_categories[i]);text(3,y+1,i==category?accent:RGB(72,96,130),"%s","SECTOR");}
 text(3,28,muted,"L/R CATEGORY");
 lore_cover(148,48,92,138,topic+category*11,1);
 text(31,5,hot,"%s",lore_categories[category]);text(31,7,ink,"%.27s",milky_way_topics[topic]);
 rect(248,68,207,1,RGB(99,69,121));
 text_wrap(31,10,25,8,ink,milky_way_notes[topic],0);
 text(31,21,accent,"FILE %02d / %02d",topic+1,(int)(sizeof(milky_way_topics)/sizeof(*milky_way_topics)));
 /* Each archive section contains three files; show progress within the
  * current section so the reader can see the local three-page depth. */
 {int local=topic%3,rail_y=72,rail_h=112,thumb_h=36;rect(462,rail_y,3,rail_h,RGB(48,62,86));rect(462,rail_y+local*(rail_h-thumb_h)/2,3,thumb_h,accent);}
 text(31,23,muted,"A field guide to the inhabited galaxy.");
 footer("UP/DOWN TOPIC   L/R CATEGORY   O BACK");
}
#include "discovery-atlas.h"
static int module_store_capacity_for_ship(int ship){
 int banks=0;
 for(int c=0;c<6;c++)banks+=fit_capacity(ship,c);
 int cap=banks*2;
 if(cap<12)cap=12;
 if(cap>MODULE_STORE_CAPACITY)cap=MODULE_STORE_CAPACITY;
 return cap;
}
/* Catalogue order matches player_ships; prose describes real speed/hold/range trade-offs. */
static const char *shipyard_description[]={
 "The Adder is a first command, not a last resort. Its tiny eight-tonne hold rewards careful buying; a compact hull and modest range suit a pilot learning the local lanes.",
 "The Gecko lives between departure boards. Fast enough to make courier runs feel brisk, it trades hold space for pace. Nine tonnes means choosing your cargo before choosing your customers.",
 "The Moray is the patient scout's travelling companion. Eleven tonnes leave room for discoveries, while its eight-light-year reach opens routes the starter hull cannot manage.",
 "The Cobra Mk1 is a working pilot's old friend: fifteen tonnes, useful pace and no grand promises. It carries the next shipment while you decide what sort of commander to become.",
 "The Cobra Mk3 turns small-time trading into a proper living. Twenty tonnes and a lively cruising speed make this a versatile step up, without dropping to bulk-hauler cruising speeds.",
 "The Python brings the warehouse with you. Sixty tonnes and a long jump range reward planned trading circuits. Its slower cruise is the price of carrying enough stock to matter.",
 "The Anaconda thinks in consignments, not parcels. A hundred-tonne hold and ten-light-year reach suit ambitious trade routes. Plan your departure: this is the slowest hull in the shop.",
 "The Fer de Lance arrives before its reputation. The fastest ship on this catalogue pairs pursuit-friendly speed with a useful 28-tonne hold. Buy it for the journey as much as the delivery.",
 "The Krait splits the difference between courier and merchant. A brisk cruise, 36 tonnes and extended jump reach let an independent pilot chase distant opportunities without packing light.",
 "The Ophidian is for the trader who keeps one eye on the next horizon. Forty-eight tonnes and a 9.2-light-year reach offer serious carrying power without dropping to heavy-hauler speeds.",
 "The Manta Ray is a swift long-range command ship with a broad navigation suite. Its flowing hull favours explorers who want capital-grade systems without accepting freighter handling.",
 "The Atlas Hauler is a moving warehouse. Eight hold banks and immense cargo space make it a trade-route specialist; deliberate acceleration and wide turns demand an organised captain.",
 "The Hammerhead is a hardened patrol vessel built around weapons, defence and decisive braking. It sacrifices freight space for the ability to enter a dangerous lane and control it.",
 "The Leviathan carries bulk freight behind a fortress of hull and shields. It is slow to answer the helm, but few independent ships can move this much cargo through hostile space.",
 "The Iron Cathedral is an armoured citadel with eight weapon and defence banks. Its monumental protection comes with low speed, high heat demands and the handling of a true capital hull.",
 "The Wayfarer Ark is a self-contained expedition ship. Huge hold, fuel and utility capacity support long deployments, research work and rescue operations far beyond ordinary station routes.",
 "The Solar Clipper turns stellar energy into speed and range. Fast, elegant and lightly protected for its price, it rewards pilots who plan routes carefully and avoid prolonged slugging matches.",
 "The Vigilance Carrier coordinates fighters, patrol systems and heavy defensive equipment. It excels as a mobile security platform while requiring patience during acceleration and docking.",
 "The Nautilus is a deep-space laboratory wrapped in formidable shields. Its fuel and utility banks support remote surveys, anomaly work and extended voyages without regular station support.",
 "The Mirage is a fast electronic-warfare hull with strong navigation and utility capacity. Modest cargo space is exchanged for speed, evasive handling and adaptable specialist systems.",
 "The Crown of Zaonce combines diplomatic presence with serious cargo and defensive capacity. It is a balanced capital yacht for commanders who want comfort, authority and practical range.",
 "The Anvil is a siege ship: eight weapon banks, eight defence banks and exceptional hull strength. It moves slowly, runs hot and exists to survive confrontations smaller ships must avoid.",
 "Needlefish is an ERL-exclusive asymmetric interceptor. Its long port blade and offset starboard engine make a distinctive silhouette. Fast acceleration and sharp handling reward precision; light armour and a small hold demand care."
};
#include "shipyard-console.h"
static void yard(void){shipyard_console_draw();}
/* Expanded outfitting: only list items this hub actually stocks. */
enum { EQUIP_COUNT = EQUIPMENT_COUNT };
static const char *equipment_names[EQUIP_COUNT]={
 "REFUEL TANK","PULSE LASER","BEAM LASER","MISSILE RESTOCK","DOCKING COMPUTER","NAV BEACON",
 "SHIELD BOOSTER","MILITARY SHIELD","LASER COOLING","HEAT SINK","CARGO BAY +8T","FREIGHT RACK +16T",
 "LONG-RANGE SCANNER","PLANET SCANNER","FUEL SCOOP","AGRI SCOOP","ECM SUITE","CHAFF DISPENSER",
 "ESCAPE POD","AUTO-REPAIR KIT","MINING LASER","REFINERY UNIT","PASSENGER CABIN","EXCLUSIVE CLAMP","HEAT BUFFER","HEAVY LASER",
 "RAPID PULSE",
 "PRECISION LANCE",
 "SCATTER ARRAY",
 "ION PROJECTOR",
 "PLASMA CUTTER",
 "DISRUPTOR",
 "REACTIVE ARMOUR",
 "RECHARGE RELAY",
 "PRISMATIC SHIELD",
 "THERMAL LINER",
 "IMPACT DAMPER",
 "MISSILE BULKHEAD",
 "DEEP SCANNER",
 "SURVEY ARRAY",
 "RANGE OPTICS",
 "TARGETING LENS",
 "SALVAGE ANALYSER",
 "COMPRESSED BAY",
 "BULK CONTAINER",
 "UTILITY LOCKER",
 "INDUSTRIAL SCOOP",
 "CORONA SCOOP",
 "BOOST ECONOMISER",
 "SOLAR BAFFLE",
 "FIREPOWER AMP",
 "WEAPON OVERDRIVE",
 "CYCLING SERVO",
 "COLD FIRING COIL",
 "REPAIR DRONES",
 "RADIATOR FINS",
 "LAW SCANNER",
 "RED BEAM LASER","GREEN PULSE LASER","BLUE ION LANCE","VIOLET ARC LASER","RAINBOW PRISM LASER","AMBER BURST LASER","CYAN RIPPLE LASER","WHITE RAIL LASER","ORANGE FLARE ARRAY","PINK PHASE LASER","LIME SHARD ARRAY","BLACKSTAR DISRUPTOR","MiniBolt MiningGun Lite",
 "TWIN PULSE COUPLER","SUSTAINED BEAM REGULATOR","ARMOUR-PIERCING CAPACITOR","MINING RESONATOR","POINT-DEFENCE TURRET","ABLATIVE HULL PLATING","KINETIC SCREEN","OVERCHARGED SHIELD GRID","FAST-CYCLE SHIELD RELAY","THERMAL BARRIER","EMERGENCY BULKHEADS","VECTORING JETS","PRECISION MANOEUVRING JETS","INERTIAL STABILISER","FLIGHT-ASSIST BYPASS","DRIFT VECTOR CONTROLLER","HYPERDRIVE CHARGE ACCELERATOR","MILITARY CHARGE ACCELERATOR","JUMP STABILISER","MASS-LOCK COMPENSATOR","APPROACH GOVERNOR","MISSION CARGO LOCKER","SECURE CARGO VAULT","REFRIGERATED HOLD","ORE BINS","LUXURY PASSENGER SUITE","HEAVY TRACTOR ARRAY","CRUISE THRUSTERS","COMBAT THRUSTERS","BOOST INJECTORS","EFFICIENT BOOSTER","EMERGENCY BOOSTER","DEEP-SPACE DRIVE","EFFICIENT HYPERDRIVE","MILITARY HYPERDRIVE","THERMAL CRUISE SHUNT","ADVANCED HEAT BUFFER","FIELD REPAIR GANTRY","SALVAGE DRONE RACK","SIGNAL DECRYPTER","SIGNATURE BAFFLER","EMERGENCY ENERGY ROUTER","SURVEY DATA CORE",
 "TWIN PULSE COUPLER","SUSTAINED BEAM REGULATOR","ARMOUR-PIERCING CAPACITOR","MINING RESONATOR","POINT-DEFENCE TURRET","ABLATIVE HULL PLATING","KINETIC SCREEN","OVERCHARGED SHIELD GRID","FAST-CYCLE SHIELD RELAY","THERMAL BARRIER","EMERGENCY BULKHEADS","VECTORING JETS","PRECISION MANOEUVRING JETS","INERTIAL STABILISER","FLIGHT-ASSIST BYPASS","DRIFT VECTOR CONTROLLER","HYPERDRIVE CHARGE ACCELERATOR","MILITARY CHARGE ACCELERATOR","JUMP STABILISER","MASS-LOCK COMPENSATOR","APPROACH GOVERNOR","MISSION CARGO LOCKER","SECURE CARGO VAULT","REFRIGERATED HOLD","ORE BINS","LUXURY PASSENGER SUITE","HEAVY TRACTOR ARRAY","CRUISE THRUSTERS","COMBAT THRUSTERS","BOOST INJECTORS","EFFICIENT BOOSTER","EMERGENCY BOOSTER","DEEP-SPACE DRIVE","EFFICIENT HYPERDRIVE","MILITARY HYPERDRIVE","THERMAL CRUISE SHUNT","ADVANCED HEAT BUFFER","FIELD REPAIR GANTRY","SALVAGE DRONE RACK","SIGNAL DECRYPTER","SIGNATURE BAFFLER","EMERGENCY ENERGY ROUTER","SURVEY DATA CORE",
 "ERL VECTOR GOV", "ERL COOLANT",
 [115]="LITTLE UFO", [116]="LITTLE ALIEN", [117]="HAMBURGER", [118]="LITTLE DUCKY",
 [119]="COFFEE CUP", [120]="COLA CUP", [121]="TINY CACTUS", [122]="LEAFY PLANT", [123]="BONSAI", [124]="ALIEN BOBBLEHEAD", [125]="LUCKY DUCK", [126]="SPACE BURGER", [127]="LAVA LAMP", [128]="PLANET GLOBE", [129]="MINI ARCADE", [130]="SERVICE ROBOT", [131]="ROCKET MODEL", [132]="NOODLE CUP", [133]="LUCKY CAT", [134]="CRYSTAL CLUSTER"
};
static const char *equipment_list_names[EQUIP_COUNT]={
 "REFUEL","PULSE LASER","BEAM LASER","MISSILE +1","DOCK COMP","NAV BEACON",
 "SHIELD BOOST","MIL SHIELD","LASER COOL","HEAT SINK","CARGO +8T","FREIGHT +16T",
 "LONG SCAN","PLANET SCAN","FUEL SCOOP","AGRI SCOOP","ECM SUITE","CHAFF",
 "ESCAPE POD","AUTO-REPAIR","MINING LASER","REFINERY","PAX CABIN","EXCL CLAMP","HEAT BUFFER","HEAVY LASER",
 "RAPID PULSE",
 "LANCE",
 "SCATTER",
 "ION PROJECTOR",
 "PLASMA CUTTER",
 "DISRUPTOR",
 "REACT ARMOUR",
 "RECHARGE RELAY",
 "PRISM SHIELD",
 "THERMAL LINER",
 "IMPACT DAMPER",
 "MISSILE BULKHD",
 "DEEP SCANNER",
 "SURVEY ARRAY",
 "RANGE OPTICS",
 "TARGET LENS",
 "SALVAGE ANALYSR",
 "COMPRESS BAY",
 "BULK CONTAINER",
 "UTILITY LOCKER",
 "IND SCOOP",
 "CORONA SCOOP",
 "BOOST ECON",
 "SOLAR BAFFLE",
 "FIREPOWER +10%",
 "WPN OVERDRIVE",
 "CYCLING SERVO",
 "COLD COIL",
 "REPAIR DRONES",
 "RADIATOR FINS",
 "LAW SCANNER",
 "RED BEAM","GREEN PULSE","BLUE ION","VIOLET ARC","RAINBOW PRISM","AMBER BURST","CYAN RIPPLE","WHITE RAIL","ORANGE FLARE","PINK PHASE","LIME SHARD","BLACKSTAR","MINIBOLT LITE",
 "TWIN PULSE","BEAM REGULATOR","ARMOUR CAP","MINING RESONATOR","POINT DEFENCE","ABLATIVE PLATING","KINETIC SCREEN","OVERCHARGE GRID","FAST SHIELD","THERMAL BARRIER","EMERG BULKHEADS","VECTOR JETS","PRECISION JETS","STABILISER","ASSIST BYPASS","DRIFT CONTROL","CHARGE ACCEL","MIL CHARGE","JUMP STABILISER","MASS-LOCK COMP","APPROACH GOV","MISSION LOCKER","SECURE VAULT","REFRIG HOLD","ORE BINS","LUXURY SUITE","HEAVY TRACTOR","CRUISE THRUST","COMBAT THRUST","BOOST INJECT","EFFIC BOOST","EMERG BOOST","DEEP-SPACE DRIVE","EFFIC HYPER","MIL HYPER","THERMAL SHUNT","ADV HEAT BUFFER","REPAIR GANTRY","SALVAGE DRONES","SIGNAL DECRYPT","SIG BAFFLER","ENERGY ROUTER","SURVEY CORE",
 "TWIN PULSE","BEAM REGULATOR","ARMOUR CAP","MINING RESONATOR","POINT DEFENCE","ABLATIVE PLATING","KINETIC SCREEN","OVERCHARGE GRID","FAST SHIELD","THERMAL BARRIER","EMERG BULKHEADS","VECTOR JETS","PRECISION JETS","STABILISER","ASSIST BYPASS","DRIFT CONTROL","CHARGE ACCEL","MIL CHARGE","JUMP STABILISER","MASS-LOCK COMP","APPROACH GOV","MISSION LOCKER","SECURE VAULT","REFRIG HOLD","ORE BINS","LUXURY SUITE","HEAVY TRACTOR","CRUISE THRUST","COMBAT THRUST","BOOST INJECT","EFFIC BOOST","EMERG BOOST","DEEP-SPACE DRIVE","EFFIC HYPER","MIL HYPER","THERMAL SHUNT","ADV HEAT BUFFER","REPAIR GANTRY","SALVAGE DRONES","SIGNAL DECRYPT","SIG BAFFLER","ENERGY ROUTER","SURVEY CORE",
 "ERL Vector Governor", "ERL Coolant Matrix",
 [115]="UFO CHARM", [116]="ALIEN CHARM", [117]="BURGER CHARM", [118]="DUCK CHARM",
 [119]="COFFEE CUP", [120]="COLA CUP", [121]="TINY CACTUS", [122]="LEAFY PLANT", [123]="BONSAI", [124]="ALIEN BOBBLEHEAD", [125]="LUCKY DUCK", [126]="SPACE BURGER", [127]="LAVA LAMP", [128]="PLANET GLOBE", [129]="MINI ARCADE", [130]="SERVICE ROBOT", [131]="ROCKET MODEL", [132]="NOODLE CUP", [133]="LUCKY CAT", [134]="CRYSTAL CLUSTER"
};
static const char *equipment_details[EQUIP_COUNT]={
 "Refill jump fuel to this ship's tank limit. Fuel is separate from cargo.",
 "24 base damage per shot. X fires in space. Weapon pips modify damage.",
 "36 damage per shot. Arm a fitted laser with Square on Ship Loadout.",
 "Add one missile, up to four. Circle+L selects; tap Circle fires at a locked ship.",
 "Guidance range 8,000 m. Triangle opens Comms; select REQUEST AUTO-DOCK.",
 "Marks your plotted next hop on the nearby jump list. Plot a route first.",
 "Shield recovery becomes 3.0/s before SYS-pip scaling. Best fitted rate wins.",
 "Shield recovery becomes 4.5/s before SYS-pip scaling. Best fitted rate wins.",
 "Passive cooling rises to 38/s. Boosting and nearby suns still generate heat.",
 "Circle+Down selects. Tap Circle dumps 40 heat; 30s cooldown.",
 "Adds eight tonnes. Stacks with different hold expansions and a cabin.",
 "Adds sixteen tonnes. Stacks with Cargo +8T and the exclusive clamp.",
 "Identify unknown ships within 5,000 m instead of 2,500 m. Uses NAV slot.",
 "Extends surface survey scans from 380 to 560 m. Aim at life or minerals.",
 "Passive fuel collection within 4,000 m of a sun's surface. Watch heat.",
 "Agri-market scoop: collects fuel at a sun, not a planet. Watch heat.",
 "Auto pulse vs close missiles: 18 shield energy, 18s cooldown. DEF.",
 "Six decoys; recharge10s. Circle+Down selects; tap Circle deploys. UTIL.",
 "Consumed on fatal damage or thermal runaway. Emergency tow to the hub.",
 "Repairs hull at 2/s out of combat and below 80 heat. Restores damage at 100.",
 "54 base damage to rocks; 18 to ships. Fire at rocks, then collect the ore.",
 "Collected mineral cargo becomes alloys. Passive; does not refine old stock.",
 "One passenger berth for station taxi offers. Deliver before removing it.",
 "Chandler-only clamp adds 8 tonnes alongside other hold modules.",
 "Reduces speed and boost heat generation by 70%; not sun or weapon heat.",
 "60 damage per shot; slower 0.36s cycle and 22 heat. Square arms in Loadout.",
 "14 damage; 0.08s cycle, 5 heat. Short 1800m rapid-fire streams.",
 "90 damage; 0.70s cycle, 28 heat. Narrow aim; reaches 4200m.",
 "Wide shot cone; up to 48 damage, falling with distance. 900m range.",
 "12 damage plus up to 25 shield drain. 0.25s cycle, 9 heat.",
 "70 damage; 25% bypasses shields. 0.50s cycle, 26 heat. 1600m.",
 "22 damage; delays struck ship's next shot by at least 1.2s. 2000m.",
 "Reduces damage reaching your hull by 20%. Shields are unchanged.",
 "Adds 1 shield energy/s before SYS pips. Works with other shields.",
 "Shield recovery 6/s before SYS pips. Strongest shield tier wins.",
 "Reduces shield and hull damage from solar heat and runaway by 30%.",
 "Reduces collision damage by 25%. Does not reduce weapons fire.",
 "Reduces damage from incoming missile impacts by 35%.",
 "Identifies unknown ships within 7500m. Strongest ID scanner wins.",
 "Extends planetary life and mineral scans to 760m.",
 "Extends the active primary weapon's reach by 20%.",
 "Widens primary shot tolerance by 20%; no automatic steering.",
 "Adds 20% cash from wreck recovery and automatic full-hold sales.",
 "Adds 24 tonnes. Stacks with different hold modules.",
 "Adds 32 tonnes. Stacks with different hold modules.",
 "Adds 4 tonnes of general cargo. Does not hide illegal goods.",
 "Collects 1 fuel/s near the sun. Strongest scoop wins. Watch heat.",
 "Collects 1.25 fuel/s near the sun. Strongest scoop wins.",
 "Reduces boost fuel consumption by 20%. Jump fuel is unchanged.",
 "Reduces heat gained near suns by 25%. Does not prevent all damage.",
 "Adds 10% primary weapon damage. Works with weapon power pips.",
 "Adds 20% primary damage, but raises weapon heat by 15%.",
 "Shortens primary shot intervals by 15%. Weapon heat rises 10%.",
 "Reduces heat per primary shot by 20%. Does not cool the sun.",
 "Repairs hull at 3.5/s out of combat below 80 heat. Best rate wins.",
 "Adds 8 heat/s to passive cooling, away from suns and boost.",
 "Circle+Up selects. Circle scans 20 km. Radar echoes last 12s; patrols keep moving.",
 "42 damage red beam. Reliable mid-range pressure with a warm red twin trace.","20 damage rapid green pulses. Forgiving aim and low heat at close range.","58 damage blue lance. Drains 18 shields on impact; long, focused blue fire.","30 damage violet arcs. Wide aim cone and a jagged electric visual.","34 damage prism bursts. Gold, pink and cyan pulse colours cycle each shot.","52 damage amber burst. Heavy two-shot-looking fire with 18 percent armour bypass.","26 damage cyan ripple. A cool zig-zag wave that interrupts hostile firing.","110 damage white rail. Very hot, narrow, slow long-range precision shot.","46 damage orange flare spread. Devastating close-range fan of heated shards.","38 damage pink phase pulse. 35 percent bypasses shields into hull integrity.","32 damage lime shard spread. Very wide, fast anti-fighter pattern.","76 damage Blackstar disruptor. Slows hostile firing for two seconds.","Dad\'s starter mining tool. 54 damage to rocks; only 2 to ships. Low heat.",
 "Primary damage +15%; weapon heat +12%.","Beam cycle x0.82; damage per shot x0.90.","20% damage bypasses shields; cycle x1.15.","Rock damage +45%; ship damage x0.70.","Automatically engages one close incoming missile.","Hull damage x0.72; acceleration x0.92.","Collision and asteroid damage x0.55; shield recovery x0.90.","Maximum shield +35%; recharge heat +25%.","Shield recovery +2.0/s; maximum shield x0.85.","Sun, boost-overheat and plasma damage x0.60.","Once per launch, fatal damage leaves 10% hull; speed x0.96.","Pitch, yaw and roll +18%; hard-turn heat +8%.","Low-speed rotation +30%; high-speed rotation -8%.","Roll settles 25% faster; drift retention -20%.","Momentum retention 72%; braking x0.70.","Momentum retention 60%; braking x0.85.","Charge duration x0.75; charge heat +20%.","Charge duration x0.58; heat +35%; jump fuel +10%.","Interruption threshold +50%; charge duration x1.08.","Nearby-object charge penalty reduced 45%.","Safer auto-approach braking; maximum speed x0.96.","Eight tonnes reserved for mission cargo only.","General hold +12t; reduced illegal-goods scan chance.","General hold +12t; delivery decay timers x0.65.","Twenty tonnes reserved for mined materials.","Two premium passenger berths.","Recovery range +65%; tractor time x0.70.","Maximum speed +12%; normal fuel use +10%.","Acceleration +22%; braking +18%; speed +5%.","Boost acceleration +30%; boost speed +12%.","Boost fuel x0.68; heat x0.82; acceleration x0.90.","Below 30% hull, boost acceleration +45%.","Jump range +22%; fuel tank +15%; acceleration x0.90.","Jump fuel x0.78; charge duration x1.18.","Jump range +15%; charge x0.72; fuel +18%.","Cruise heat x0.65; weapon heat +8%.","Speed and boost heat x0.18.","Out-of-combat repair +2.5/s; speed x0.96.","Wreck cash +30%; recovery range +25%.","Unknown discovery ID range +50%; scan time x0.65.","Hostile detection -20%; sensor range -15%.","Once per 45s, restore 15 shield for 25 heat.","New exploration and discovery payouts +20%.",
 "Primary damage +15%; weapon heat +12%.","Beam cycle x0.82; damage per shot x0.90.","20% damage bypasses shields; cycle x1.15.","Rock damage +45%; ship damage x0.70.","Automatically engages one close incoming missile.","Hull damage x0.72; acceleration x0.92.","Collision damage x0.55; shield recovery x0.90.","Maximum shield +35%; recharge heat +25%.","Shield recovery +2.0/s; maximum shield x0.85.","Sun, boost-overheat and plasma damage x0.60.","Once per launch, fatal damage leaves 10% hull; speed x0.96.","Pitch, yaw and roll +18%; hard-turn heat +8%.","Low-speed rotation +30%; high-speed rotation -8%.","Roll settles 25% faster; drift retention -20%.","Momentum retention 72%; braking x0.70.","Momentum retention 60%; braking x0.85.","Charge duration x0.75; charge heat +20%.","Charge duration x0.58; heat +35%; jump fuel +10%.","Interruption threshold +50%; charge duration x1.08.","Nearby-object charge penalty reduced 45%.","Safer auto-approach braking; maximum speed x0.96.","Eight tonnes reserved for mission cargo only.","General hold +12t; reduced illegal-goods scan chance.","General hold +12t; delivery decay timers x0.65.","Twenty tonnes reserved for mined materials.","Two premium passenger berths.","Recovery range +65%; tractor time x0.70.","Maximum speed +12%; normal fuel use +10%.","Acceleration +22%; braking +18%; speed +5%.","Boost acceleration +30%; boost speed +12%.","Boost fuel x0.68; heat x0.82; acceleration x0.90.","Below 30% hull, boost acceleration +45%.","Jump range +22%; fuel tank +15%; acceleration x0.90.","Jump fuel x0.78; charge duration x1.18.","Jump range +15%; charge x0.72; fuel +18%.","Cruise heat x0.65; weapon heat +8%.","Speed and boost heat x0.18.","Out-of-combat repair +2.5/s; speed x0.96.","Wreck cash +30%; recovery range +25%.","Unknown discovery ID range +50%; scan time x0.65.","Hostile detection -20%; sensor range -15%.","Once per 45s, restore 15 shield for 25 heat.","New exploration and discovery payouts +20%.",
 "A championship-only manoeuvring governor. Authored flight profiles improve rotation by twelve percent without changing the hull.", "A sealed championship coolant matrix. Reduces engine and boost heat by fifteen percent in normal flight.",
 [115]="Supplied pixel-art hanging UFO. One charm per ship.", [116]="Supplied pixel-art alien companion. One charm per ship.", [117]="A tiny hanging hamburger. One charm per ship.", [118]="A tiny hanging duck. One charm per ship.",
 [119]="Supplied dashboard pixel art. Static. One per ship.", [120]="Supplied dashboard pixel art. Static. One per ship.", [121]="Supplied dashboard pixel art. Static. One per ship.", [122]="Supplied dashboard pixel art. Static. One per ship.", [123]="Supplied dashboard pixel art. Static. One per ship.", [124]="Supplied dashboard pixel art. Static. One per ship.", [125]="Supplied dashboard pixel art. Static. One per ship.", [126]="Supplied dashboard pixel art. Static. One per ship.", [127]="Supplied dashboard pixel art. Static. One per ship.", [128]="Supplied dashboard pixel art. Static. One per ship.", [129]="Supplied dashboard pixel art. Static. One per ship.", [130]="Supplied dashboard pixel art. Static. One per ship.", [131]="Supplied dashboard pixel art. Static. One per ship.", [132]="Supplied dashboard pixel art. Static. One per ship.", [133]="Supplied dashboard pixel art. Static. One per ship.", [134]="Supplied dashboard pixel art. Static. One per ship."
};
static const char *equipment_effects[EQUIP_COUNT]={
 "Refill to tank maximum",
 "24 base damage / shot",
 "36 base damage / shot",
 "One missile; maximum 4",
 "Guided docking: 8000 m",
 "Jump-list next-hop marker",
 "Shield regen: 3.0/s",
 "Shield regen: 4.5/s",
 "Cooling: 38 heat/s",
 "Dump 40 heat / 30s cooldown",
 "Base hold +8 tonnes",
 "Base hold +16 tonnes",
 "Unknown ship IDs: 5000 m",
 "Surface survey: 560 m",
 "Sun scoop: 0.5 fuel/s",
 "Sun scoop: 0.75 fuel/s",
 "Automatic missile defence",
 "6 decoys / 10s recharge",
 "One-use emergency tow",
 "Hull repair: 2/s",
 "Rocks 54 / ships 18",
 "New minerals become alloys",
 "One passenger berth",
 "Base hold +8 tonnes",
 "Speed/boost heat x0.30","60 damage / 0.36s / 22 heat",
 "Fast fire / short range",
 "Long range / precise aim",
 "Close spread / 18 heat",
 "Shield breaker / 2200m",
 "Armour pressure / hot",
 "Fire suppression / 14 heat",
 "Hull damage x0.80",
 "Shield recovery +1/s",
 "Shield recovery 6/s",
 "Thermal damage x0.70",
 "Collision damage x0.75",
 "Missile damage x0.65",
 "Ship ID range 7500m",
 "Surface scan range 760m",
 "Weapon range +20%",
 "Aim tolerance +20%",
 "Cash salvage +20%",
 "Cargo capacity +24t",
 "Cargo capacity +32t",
 "Cargo capacity +4t",
 "Sun scoop 1 fuel/s",
 "Sun scoop 1.25 fuel/s",
 "Boost fuel use x0.80",
 "Solar heat gain x0.75",
 "Primary damage +10%",
 "Damage +20% / heat +15%",
 "Cycle x0.85 / heat +10%",
 "Weapon heat x0.80",
 "Automatic repair 3.5/s",
 "Passive cooling +8/s",
 "Law radar / 20 km / UTIL",
 "42 damage / 2800m","20 damage / 1600m","58 + shield drain / 3200m","Arc cone / 1900m","Prism pulse / 2500m","52 + armour bypass / 2600m","Arc interrupt / 2100m","110 rail / 4800m","Close flare spread / 1200m","Phase bypass / 2400m","Wide shard fan / 1500m","76 + 2s disrupt / 3000m","MINING TOOL / 54 ROCK / 2 SHIP",
 "Primary damage +15%; weapon heat +12%.","Beam cycle x0.82; damage per shot x0.90.","20% damage bypasses shields; cycle x1.15.","Rock damage +45%; ship damage x0.70.","Automatically engages one close incoming missile.","Hull damage x0.72; acceleration x0.92.","Collision and asteroid damage x0.55; shield recovery x0.90.","Maximum shield +35%; recharge heat +25%.","Shield recovery +2.0/s; maximum shield x0.85.","Sun, boost-overheat and plasma damage x0.60.","Once per launch, fatal damage leaves 10% hull; speed x0.96.","Pitch, yaw and roll +18%; hard-turn heat +8%.","Low-speed rotation +30%; high-speed rotation -8%.","Roll settles 25% faster; drift retention -20%.","Momentum retention 72%; braking x0.70.","Momentum retention 60%; braking x0.85.","Charge duration x0.75; charge heat +20%.","Charge duration x0.58; heat +35%; jump fuel +10%.","Interruption threshold +50%; charge duration x1.08.","Nearby-object charge penalty reduced 45%.","Safer auto-approach braking; maximum speed x0.96.","Eight tonnes reserved for mission cargo only.","General hold +12t; reduced illegal-goods scan chance.","General hold +12t; delivery decay timers x0.65.","Twenty tonnes reserved for mined materials.","Two premium passenger berths.","Recovery range +65%; tractor time x0.70.","Maximum speed +12%; normal fuel use +10%.","Acceleration +22%; braking +18%; speed +5%.","Boost acceleration +30%; boost speed +12%.","Boost fuel x0.68; heat x0.82; acceleration x0.90.","Below 30% hull, boost acceleration +45%.","Jump range +22%; fuel tank +15%; acceleration x0.90.","Jump fuel x0.78; charge duration x1.18.","Jump range +15%; charge x0.72; fuel +18%.","Cruise heat x0.65; weapon heat +8%.","Speed and boost heat x0.18.","Out-of-combat repair +2.5/s; speed x0.96.","Wreck cash +30%; recovery range +25%.","Unknown discovery ID range +50%; scan time x0.65.","Hostile detection -20%; sensor range -15%.","Once per 45s, restore 15 shield for 25 heat.","New exploration and discovery payouts +20%.",
 "Primary damage +15%; weapon heat +12%.","Beam cycle x0.82; damage per shot x0.90.","20% damage bypasses shields; cycle x1.15.","Rock damage +45%; ship damage x0.70.","Automatically engages one close incoming missile.","Hull damage x0.72; acceleration x0.92.","Collision damage x0.55; shield recovery x0.90.","Maximum shield +35%; recharge heat +25%.","Shield recovery +2.0/s; maximum shield x0.85.","Sun, boost-overheat and plasma damage x0.60.","Once per launch, fatal damage leaves 10% hull; speed x0.96.","Pitch, yaw and roll +18%; hard-turn heat +8%.","Low-speed rotation +30%; high-speed rotation -8%.","Roll settles 25% faster; drift retention -20%.","Momentum retention 72%; braking x0.70.","Momentum retention 60%; braking x0.85.","Charge duration x0.75; charge heat +20%.","Charge duration x0.58; heat +35%; jump fuel +10%.","Interruption threshold +50%; charge duration x1.08.","Nearby-object charge penalty reduced 45%.","Safer auto-approach braking; maximum speed x0.96.","Eight tonnes reserved for mission cargo only.","General hold +12t; reduced illegal-goods scan chance.","General hold +12t; delivery decay timers x0.65.","Twenty tonnes reserved for mined materials.","Two premium passenger berths.","Recovery range +65%; tractor time x0.70.","Maximum speed +12%; normal fuel use +10%.","Acceleration +22%; braking +18%; speed +5%.","Boost acceleration +30%; boost speed +12%.","Boost fuel x0.68; heat x0.82; acceleration x0.90.","Below 30% hull, boost acceleration +45%.","Jump range +22%; fuel tank +15%; acceleration x0.90.","Jump fuel x0.78; charge duration x1.18.","Jump range +15%; charge x0.72; fuel +18%.","Cruise heat x0.65; weapon heat +8%.","Speed and boost heat x0.18.","Out-of-combat repair +2.5/s; speed x0.96.","Wreck cash +30%; recovery range +25%.","Unknown discovery ID range +50%; scan time x0.65.","Hostile detection -20%; sensor range -15%.","Once per 45s, restore 15 shield for 25 heat.","New exploration and discovery payouts +20%.",
 "Pitch and yaw +12%.", "Engine and boost heat -15%.",
 [115]="COSMETIC MODULE. Uses one Utility slot. No stat bonuses.", [116]="COSMETIC MODULE. Uses one Utility slot. No stat bonuses.", [117]="COSMETIC MODULE. Uses one Utility slot. No stat bonuses.", [118]="COSMETIC MODULE. Uses one Utility slot. No stat bonuses.",
 [119]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [120]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [121]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [122]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [123]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [124]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [125]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [126]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [127]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [128]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [129]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [130]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [131]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [132]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [133]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses.", [134]="DASHBOARD COSMETIC MODULE. Hold/Cabin slot. No stat bonuses."
};
static const int equipment_costs[EQUIP_COUNT]={0,2200,4000,1000,2500,1800,6000,9000,4500,3200,3500,7000,3000,2800,7500,5000,5500,2000,4000,3600,4200,4800,2500,1500,6500,14000,
 6800,
 24000,
 9500,
 12500,
 18500,
 16500,
 7500,
 8800,
 21000,
 6900,
 5400,
 13000,
 11500,
 9800,
 10500,
 8200,
 7800,
 14500,
 23000,
 1200,
 11000,
 19000,
 6400,
 8500,
 9000,
 17500,
 11200,
 9700,
 15500,
 5600,
 4800,
 11800,8600,16400,14200,19500,17600,15100,29000,13300,16800,12100,22400,1800,
 14500,16000,19000,12500,17500,18500,14500,24000,17000,15500,27500,12500,13500,11000,15000,22500,21000,34000,17000,25000,11500,10000,19000,14000,16500,26000,17500,15000,18500,21000,17000,15500,26500,20000,36000,14500,26000,22500,20000,16500,22000,24500,17500,
 14500,16000,19000,12500,17500,18500,14500,24000,17000,15500,27500,12500,13500,11000,15000,22500,21000,34000,17000,25000,11500,10000,19000,14000,16500,26000,17500,15000,18500,21000,17000,15500,26500,20000,36000,14500,26000,22500,20000,16500,22000,24500,17500,
 0,0,
 [115]=250, [116]=250, [117]=250, [118]=250,
 [119]=250, [120]=250, [121]=250, [122]=250, [123]=250, [124]=250, [125]=250, [126]=250, [127]=250, [128]=250, [129]=250, [130]=250, [131]=250, [132]=250, [133]=250, [134]=250
};
/* Minimum displayed tech (systems[].tech+1). 0 = always if economy allows. */
static const int equipment_tech[EQUIP_COUNT]={0,2,4,2,5,3,6,8,5,4,3,6,4,3,7,4,6,3,4,5,4,5,3,2,4,10,
 6,
 11,
 7,
 9,
 10,
 10,
 6,
 7,
 11,
 6,
 5,
 9,
 8,
 7,
 8,
 6,
 6,
 8,
 10,
 2,
 8,
 11,
 5,
 7,
 7,
 10,
 8,
 7,
 9,
 5,
 3,
 7,5,8,8,10,9,8,11,7,9,6,10,1,
 7,8,9,1,8,1,6,10,8,7,11,6,6,1,7,10,9,12,8,11,1,1,8,1,1,1,8,6,8,8,7,7,10,9,12,7,11,1,1,8,10,10,1,
 7,8,9,1,8,1,6,10,8,7,11,6,6,1,7,10,9,12,8,11,1,1,8,1,1,1,8,6,8,8,7,7,10,9,12,7,11,1,1,8,10,10,1,
 1,1,
 [115]=1, [116]=1, [117]=1, [118]=1,
 [119]=1, [120]=1, [121]=1, [122]=1, [123]=1, [124]=1, [125]=1, [126]=1, [127]=1, [128]=1, [129]=1, [130]=1, [131]=1, [132]=1, [133]=1, [134]=1
};
/* Economy bands that stock the item: bit0 rich industrial ... bit7 poor agricultural. 0xff = all. */
static const unsigned equipment_econ[EQUIP_COUNT]={
 0xff,0xff,0x0f,0xff,0xff,0xf0,0x1f,0x07,0x0f,0x1f,0xff,0x0e,0xff,0xf0,0x0f,0xf0,0x0f,0xff,0xff,0x1f,0x0e,0x0e,0xff,0x00,0xff,0x07,
 15,
 7,
 31,
 15,
 7,
 15,
 31,
 255,
 7,
 255,
 255,
 15,
 255,
 240,
 31,
 255,
 14,
 31,
 14,
 255,
 15,
 7,
 255,
 240,
 255,
 15,
 15,
 255,
 31,
 255,
 255,
 255,255,255,255,255,255,255,255,255,255,255,255,255u,
 255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,
 255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,255u,
 0,0,
 [115]=255, [116]=255, [117]=255, [118]=255,
 [119]=255, [120]=255, [121]=255, [122]=255, [123]=255, [124]=255, [125]=255, [126]=255, [127]=255, [128]=255, [129]=255, [130]=255, [131]=255, [132]=255, [133]=255, [134]=255
};
static const char *equip_cat_name(int i){
 int slot=equip_slot_for(i);static const char *slots[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
 return slot>=0?slots[slot]:i==0?"FUEL":"AMMO";
}
static int equipment_owned(int i){if(cosmetic_item(i))return 0;
 if(i<=0)return game.fuel>=player_ships[game.ship].range;
 if(i==3)return game.missiles>=4;
 /* Fitted catalog index wins — peers in the same slot are not "owned". */
 for(int s=0;s<FIT_SLOTS;s++)if(game.fit[s]==(uint8_t)i)return 1;
 return 0;
}
static const char *equipment_label(int i){return i>=0&&i<EQUIP_COUNT?equipment_list_names[i]:"INVALID";}
static const int equipment_trade[EQUIP_COUNT]={1,1,2,1,2,1,2,4,2,2,1,3,2,1,3,1,3,1,2,2,2,3,1,1,2,4,
 2,
 4,
 2,
 3,
 4,
 3,
 2,
 3,
 4,
 2,
 2,
 3,
 3,
 2,
 3,
 2,
 2,
 3,
 4,
 1,
 3,
 4,
 2,
 2,
 3,
 4,
 3,
 3,
 3,
 2,
 1,
 2,2,3,3,4,3,3,4,2,3,2,4,1,
 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
 1,1,
 [115]=1, [116]=1, [117]=1, [118]=1,
 [119]=1, [120]=1, [121]=1, [122]=1, [123]=1, [124]=1, [125]=1, [126]=1, [127]=1, [128]=1, [129]=1, [130]=1, [131]=1, [132]=1, [133]=1, [134]=1
};
static int equipment_in_stock(int i){if(i>=113&&!cosmetic_item(i))return 0;
 if(i<0||i>=EQUIP_COUNT)return 0;
 if(i==0)return 0;
 /* Development certification hub: Lave exposes the complete catalogue so
    every module can be bought and tested without chasing regional stock. */
 if(game.system==7)return 1;
 if(equipment_econ[i]==0)return 0; /* exclusive — chandler only */
 int have=game.systems[game.system].tech+1;
 if(game.station_variant>0&&have>1)have--;
 if(have<equipment_tech[i]||prosperity(&game,game.system)<equipment_trade[i])return 0;
 if(game.station_variant>0&&i>5&&((game.system*13+game.station_variant*7+i)%4)==0)return 0;
 unsigned mask=1u<<(game.systems[game.system].economy&7);
 return (equipment_econ[i]&mask)!=0;
}
static int equipment_stock_list(int *out,int maxn){
 int n=0; for(int i=0;i<EQUIP_COUNT&&n<maxn;i++)if(equipment_in_stock(i))out[n++]=i;
 return n;
}
static int sc_exclusive_catalog(int *out,int maxn){
 int n=0; unsigned h=game.system*17u;
 for(int i=0;i<EQUIP_COUNT&&n<maxn;i++)if(i<113&&equipment_econ[i]==0&&!equipment_owned(i))out[n++]=i;
 int extras[]={7,11,16,19,22}; for(int k=0;k<5&&n<maxn;k++){int i=extras[(h+k)%5];if(!equipment_owned(i)&&game.systems[game.system].tech+1>=equipment_tech[i]){int dupe=0;for(int j=0;j<n;j++)if(out[j]==i)dupe=1;if(!dupe)out[n++]=i;}}
 return n;
}
static int equip_install_slot(int i){
 int cat=equip_slot_for(i);if(cat<0)return -1;
 int owned=fit_find(&game,i);if(owned>=0)return owned;
 if(equip_target>=0&&equip_target%6==cat&&fit_slot_open(&game,equip_target))return equip_target;
 int empty=fit_empty_slot(&game,cat);return empty>=0?empty:cat;
}
static int equip_sell_price(int i){if(i<=0||i>=EQUIP_COUNT)return 0;return equipment_costs[i]/2;}

static int module_item_base(int token){return db_token_base(&game,token);}
static int module_item_count(int token){return token>=EQUIPMENT_COUNT?db_instance_stored(&game,token-EQUIPMENT_COUNT):token>0&&token<EQUIPMENT_COUNT?game.module_store[token]:0;}
static const char *module_item_name(int token){if(cosmetic_item(module_item_base(token)))return equipment_names[module_item_base(token)];static char names[4][80];static int next;int b=module_item_base(token);if(b<0||b>=EQUIPMENT_COUNT)return "NO MODULE";if(token<EQUIPMENT_COUNT)return equipment_names[b];char *out=names[(next++)&3];snprintf(out,80,"%s %s",db_quality(game.dockbay.owned[token-EQUIPMENT_COUNT].quality),equipment_names[b]);return out;}
static const char *module_item_short(int token){return token<EQUIPMENT_COUNT?equipment_list_names[token]:module_item_name(token);}
static const char *module_roll_effect(const DockRoll *r){if(cosmetic_item(r->base))return equipment_effects[r->base];static char text[4][128];static int next;char *out=text[(next++)&3];float p=r->power*.001f,q=r->secondary*.001f;int b=r->base;if(!r->value&&r->quality==DB_NORMAL)return equipment_effects[b];
 if(fit_is_active_weapon(b))snprintf(out,128,"DAMAGE %.1f / CYCLE %.3fs. OUTPUT %.0f%% OF NORMAL.",laser_item_damage(b)*p,weapon_item_cycle(b)/q,p*q*100);
 else if(b==10)snprintf(out,128,"CARGO +%dT (NORMAL +8T). PERMANENT %s.",(int)floorf(8*p+.5f),db_quality(r->quality));
 else if(b==38)snprintf(out,128,"SCAN RANGE %.0fm (NORMAL 7500m).",7500*p);
 else if(b==6)snprintf(out,128,"SHIELD REGEN %.2f/s (NORMAL 3.00/s).",1.5f+1.5f*p);
 else if(b==32)snprintf(out,128,"HULL DAMAGE TAKEN %.1f%% (NORMAL 80.0%%).",80/p);
 else if(b==TECH_EFFICIENT_BOOSTER)snprintf(out,128,"BOOST FUEL %.1f%% / HEAT %.1f%% OF NORMAL.",100/p,100/q);
 else if(b==TECH_BOOST_INJECTORS)snprintf(out,128,"BOOST SPEED BONUS +%.1f%% (NORMAL +12.0%%).",12*p);
 else if(b==TECH_EFFICIENT_HYPERDRIVE)snprintf(out,128,"JUMP FUEL %.1f%% (NORMAL 80.0%%).",80/p);
 else snprintf(out,128,"ENGINE HEAT %.1f%% OF NORMAL. PERMANENT %s.",100/p,db_quality(r->quality));return out;
}
static const char *module_item_effect(int token){return token>=EQUIPMENT_COUNT?module_roll_effect(&game.dockbay.owned[token-EQUIPMENT_COUNT]):equipment_effects[token];}
static int module_slot_token(int slot){return db_slot_roll(&game,slot)?EQUIPMENT_COUNT+game.dockbay.fitted[slot]-1:game.fit[slot];}
static int module_item_resale(int token){return token>=EQUIPMENT_COUNT?(game.dockbay.owned[token-EQUIPMENT_COUNT].value?game.dockbay.owned[token-EQUIPMENT_COUNT].value/2:equip_sell_price(module_item_base(token))):equip_sell_price(token);}

#define CHARM_LILAC RGB(207,166,244)
static unsigned cosmetic_ink(int base,unsigned fallback){return cosmetic_item(base)?CHARM_LILAC:fallback;}
static unsigned tech_icon_ink(int cat);
static void module_rare_shine(int x,int y,int w,int h,int token){if(token<EQUIPMENT_COUNT)return;unsigned quality=game.dockbay.owned[token-EQUIPMENT_COUNT].quality;if(quality!=DB_NORMAL){int ax=x+w-4,ay=y+h-4,up=quality!=DB_WORN;unsigned c=up?RGB(78,224,144):RGB(230,102,82);line(ax,ay-2,ax,ay+2,c);line(ax-2,ay+(up?0:0),ax,ay+(up?-2:2),c);line(ax+2,ay,ax,ay+(up?-2:2),c);}if(quality!=DB_RARE)return;unsigned color=dim_rgb(UI_CYAN,1,2);int phase=(int)(preview_time*12)%(w+40);for(int xx=0;xx<w;xx++)if(abs(xx-phase)<2)for(int yy=0;yy<h;yy++){unsigned v=fb[(y+yy)*STRIDE+x+xx],ink=tech_icon_ink(equip_slot_for(module_item_base(token)));if(v==ink||v==dim_rgb(ink,1,2))pixel(x+xx,y+yy,color);}}
static int module_store_total(void){return dockbay_storage_used(&game);}
static int module_store_capacity(void){return module_store_capacity_for_ship(game.ship);}
static int module_store_add(int item){if(cosmetic_item(item)){if(module_store_total()>=module_store_capacity())return 0;int f=db_owned_free(&game);if(f<0)return 0;game.dockbay.owned[f]=(DockRoll){++game.dockbay.serial,item,DB_NORMAL,1,0,1000,1000,0};game.dockbay.dirty=1;return 1;}if(item<=0||item>=EQUIPMENT_COUNT||equip_slot_for(item)<0||module_store_total()>=module_store_capacity())return 0;game.module_store[item]++;return 1;}
/* Storage contains spare hardware only. Fitted modules belong exclusively to
   Ship Loadout; a second copy appears here only when it is genuinely stored. */
static int module_owned_list(int *out,int maxn){int n=0;for(int i=1;i<EQUIPMENT_COUNT&&n<maxn;i++)if(equip_slot_for(i)>=0&&game.module_store[i])out[n++]=i;unsigned char used[DB_ITEMS];db_owned_map(&game,used);for(int i=0;i<DB_ITEMS&&n<maxn;i++)if(game.dockbay.owned[i].id&&!used[i])out[n++]=EQUIPMENT_COUNT+i;return n;}
static int module_buy_to_store(int item){
 if(!game.docked){message(&game,"Dock to buy spare modules.");return 0;}
 if(item<=0||item>=EQUIPMENT_COUNT||equip_slot_for(item)<0){message(&game,"That service cannot be stored.");return 0;}
 if(!equipment_in_stock(item)){message(&game,"Not stocked at this hub.");return 0;}
 if(module_store_total()>=module_store_capacity()){char note[72];snprintf(note,sizeof(note),"Item Storage is full (%d/%d).",module_store_total(),module_store_capacity());message(&game,note);return 0;}
 if(game.credits<equipment_costs[item]){message(&game,"Not enough units.");return 0;}
 if(!module_store_add(item)){message(&game,"Equipment records are full.");return 0;}game.credits-=equipment_costs[item];game.cue=SFX_UI;message(&game,"Module purchased and placed in Item Storage.");return 1;
}
static int unequip_slot(int slot,int refund){
 if(slot<0||slot>=FIT_SLOTS)return 0;
 if(!game.docked){message(&game,"Dock to change fitted modules.");return 0;}
 int old=game.fit[slot];if(old==FIT_EMPTY||!fit_value_valid(slot,old))return 0;
 if(old==22&&game.passenger_dest>=0){message(&game,"Deliver your passenger before removing the cabin.");return 0;}
 game.fit[slot]=(uint8_t)FIT_EMPTY;fit_rebuild(&game);
 if(cargo_used(&game)>cargo_capacity(&game)){
  game.fit[slot]=(uint8_t)old;fit_rebuild(&game);
  message(&game,"Unload cargo before removing the hold.");return 0;
 }
 if(refund&&fit_value_valid(slot,old)){int back=equip_sell_price(old);game.credits+=back;char note[72];snprintf(note,sizeof(note),"Sold %s for %.1f U.",equipment_list_names[old],back*.1f);message(&game,note);}
 else message(&game,"Module removed.");
 game.cue=SFX_UI;return 1;
}
static char loadout_store_note[128];
static float loadout_store_until=-1;
static int store_fitted_module(int slot){
 if(slot<0||slot>=FIT_SLOTS||!fit_slot_open(&game,slot)){message(&game,"Select an available module bank.");return 0;}
 if(!game.docked){message(&game,"Dock before moving fitted equipment into Item Storage.");return 0;}
 int item=game.fit[slot];
 if(item==FIT_EMPTY||!fit_value_valid(slot,item)){message(&game,"That module bank is already empty.");return 0;}
 if(module_store_total()>=module_store_capacity()){message(&game,"Item Storage is full. Sell a spare before removing this module.");return 0;}
 if(item==22&&game.passenger_dest>=0){message(&game,"Deliver your passenger before storing the cabin.");return 0;}
 if(db_slot_roll(&game,slot)){unsigned ref=game.dockbay.fitted[slot];int active=game.active_weapon;game.fit[slot]=FIT_EMPTY;game.dockbay.fitted[slot]=0;fit_rebuild(&game);if(cargo_used(&game)>cargo_capacity(&game)){game.fit[slot]=item;game.dockbay.fitted[slot]=ref;game.active_weapon=active;fit_rebuild(&game);message(&game,"Unload cargo before storing that hold module.");return 0;}game.dockbay.dirty=1;game.cue=SFX_UI;message(&game,"Your exact module moved to Item Storage.");return 1;}
 game.fit[slot]=(uint8_t)FIT_EMPTY;fit_rebuild(&game);
 if(cargo_used(&game)>cargo_capacity(&game)){
  game.fit[slot]=(uint8_t)item;fit_rebuild(&game);
  message(&game,"Unload cargo before storing that hold module.");return 0;
 }
 if(!module_store_add(item)){
  game.fit[slot]=(uint8_t)item;fit_rebuild(&game);
  message(&game,"Item Storage could not accept that module.");return 0;
 }
 fit_rebuild(&game);game.cue=SFX_UI;
 {char note[88];snprintf(note,sizeof(note),"%s moved to Item Storage.",equipment_list_names[item]);message(&game,note);}
 return 1;
}
static void buy_equipment(int i,int chandler){if(cosmetic_item(i)){module_buy_to_store(i);return;}
 if(!game.docked){message(&game,"Dock to buy equipment.");return;}
 if(i<0||i>=EQUIP_COUNT)return;
 if(chandler){int offers[8],n=sc_exclusive_catalog(offers,8),found=0;for(int k=0;k<n;k++)if(offers[k]==i)found=1;if(!found){message(&game,"Not offered by this chandler.");return;}}
 else if(!equipment_in_stock(i)){message(&game,"Not stocked at this hub.");return;}

 if(i==3){if(game.missiles>=4){message(&game,"Missile rack full.");return;}if(game.credits<equipment_costs[i]){message(&game,"Not enough units.");return;}game.credits-=equipment_costs[i];game.missiles++;game.cue=SFX_UI;message(&game,"Missile loaded.");return;}
 if(equipment_owned(i)){message(&game,"Already fitted.");return;}

 int slot=equip_install_slot(i);if(slot<0){message(&game,"Cannot fit that.");return;}
 int cost=equipment_costs[i],old=game.fit[slot];
 if(old==22&&game.passenger_dest>=0){message(&game,"Deliver your passenger before replacing the cabin.");return;}
 if(old!=FIT_EMPTY&&module_store_total()>=module_store_capacity()){message(&game,"Item Storage is full. Sell a spare before replacing this module.");return;}
 if(game.credits<cost){message(&game,"Not enough units.");return;}
 {uint8_t prev=game.fit[slot];game.fit[slot]=(uint8_t)i;fit_rebuild(&game);
  if(cargo_used(&game)>cargo_capacity(&game)){game.fit[slot]=prev;fit_rebuild(&game);message(&game,"Cargo will not fit that hold.");return;}}
 game.credits-=cost;if(old!=FIT_EMPTY)module_store_add(old);game.cue=SFX_UI;
 if(old!=FIT_EMPTY){char note[96];snprintf(note,sizeof(note),"Fitted %s. The old module is now in Item Storage.",equipment_list_names[i]);message(&game,note);}
 else message(&game,"Module fitted.");
}
static void sell_equipment_row(int i){
 if(!game.docked){message(&game,"Dock to sell equipment.");return;}
 if(i<=0||i==3||i>=EQUIP_COUNT){message(&game,"That is not a fitted module.");return;}
 if(!equipment_owned(i)){message(&game,"Not fitted.");return;}
 int slot=fit_find(&game,i);if(slot<0||game.fit[slot]!=(uint8_t)i){message(&game,"Not fitted.");return;}
 unequip_slot(slot,1);
}
static int equip_row_count(void){int list[EQUIP_COUNT];return equipment_stock_list(list,EQUIP_COUNT);}
static int equipment_net_cost(int i){
 if(i==0)return (int)ceilf(fmaxf(0,player_ships[game.ship].range-game.fuel))*2;
 if(i<0||i>=EQUIP_COUNT||equipment_owned(i))return 0;
 int slot=equip_install_slot(i),old=slot>=0?game.fit[slot]:FIT_EMPTY;
 return equipment_costs[i];
}
static const char *equipment_stat_change(int i){
 static char out[80];int slot=equip_slot_for(i),target=equip_install_slot(i),old=target>=0?game.fit[target]:FIT_EMPTY;
 int retained=0;for(int k=0;k<FIT_SLOTS;k++)if(k!=target&&game.fit[k]!=FIT_EMPTY)retained|=equip_mask_for(game.fit[k]);
 int delta=module_hold_bonus(i)-module_hold_bonus(old);
 if(i>=26&&slot!=FIT_HOLD){snprintf(out,sizeof(out),"%s",equipment_effects[i]);return out;}
 if(slot==FIT_HOLD)snprintf(out,sizeof(out),"Hold %d > %d tonnes",cargo_capacity(&game),cargo_capacity(&game)+delta);
 else if(slot==FIT_WPN)snprintf(out,sizeof(out),"Stored laser: %d damage/shot",i==25?60:i==2?36:i==1?24:18);
 else if(slot==FIT_DEF)snprintf(out,sizeof(out),"Shield %.1f > %.1f /s",shield_regen_rate(&game),((fit_find(&game,34)>=0&&fit_find(&game,34)!=target)?6.0:(retained&128)||i==7?4.5:(retained&2)||i==6?3.0:1.5)+((fit_find(&game,33)>=0&&fit_find(&game,33)!=target)?1.0:0.0));
 else if(slot==FIT_FUEL)snprintf(out,sizeof(out),"Scoop %.2f > %.2f fuel/s",fuel_scoop_rate(&game),(fit_find(&game,47)>=0&&fit_find(&game,47)!=target)?1.25:(fit_find(&game,46)>=0&&fit_find(&game,46)!=target)?1.0:(retained&8192)||i==15?.75:.5);
 else if(i==0)snprintf(out,sizeof(out),"Fuel %.1f > %.1f LY",game.fuel*.1f,player_ships[game.ship].range*.1f);
 else if(i==3)snprintf(out,sizeof(out),"Missiles %d > %d",game.missiles,game.missiles<4?game.missiles+1:4);
 else snprintf(out,sizeof(out),"%s",equipment_effects[i]);
 return out;
}
static void equipment_buy_action(int i,int chandler){if(cosmetic_item(i)){module_buy_to_store(i);return;}
 int cat=equip_slot_for(i);
 /* Never silently choose the first occupied weapon bay. A full WPN bank
    enters a small in-place picker; the player selects the exact trade-in
    slot with Left/Right before the normal confirmation applies. */
 if(cat==FIT_WPN&&!equipment_owned(i)&&fit_empty_slot(&game,FIT_WPN)<0&&equip_confirm_item!=i){
  equip_target=FIT_WPN;equip_confirm_item=i;
  message(&game,"Weapon bank full. Left/Right selects the WPN slot to replace.");return;
 }
 int slot=equip_install_slot(i);
 if(slot>=0&&!equipment_owned(i)&&game.fit[slot]!=FIT_EMPTY&&equip_confirm_item!=i){
  equip_confirm_item=i;message(&game,"Replace fitted module? X confirms the shown trade-in.");return;
 }
 equip_confirm_item=-1;buy_equipment(i,chandler);
}
enum { SHOP_GENERAL,SHOP_WEAPONS,SHOP_DEFENCE,SHOP_NAV,SHOP_HOLD,SHOP_FUEL,SHOP_UTIL };
static int equipment_shop_kind(void){
 int count[6]={0},total=0,best=0;
 for(int i=1;i<EQUIP_COUNT;i++)if(equipment_in_stock(i)){
  int cat=equip_slot_for(i);if(cat>=0){count[cat]++;total++;if(count[cat]>count[best])best=cat;}
 }
 if(game.system==7)return SHOP_GENERAL;
 if(!total)return SHOP_GENERAL;
 for(int cat=0;cat<6;cat++)if(cat!=best&&count[cat]==count[best])return SHOP_GENERAL;
 return best+1;
}
static const char *equipment_shop_name(void){
 static const char *general[]={"STATION OUTFITTERS","THE MODULE EXCHANGE","DECKSIDE TECH & FIT","ALL SYSTEMS OUTFIT"};
 static const char *weapons[]={"MAJOR LAZOR'S ARMAMENTS","REDFLARE ORDNANCE","STARFALL WEAPONS","HARDPOINT HEAVEN"};
 static const char *defence[]={"AEGIS DEFENCE WORKS","BULWARK SHIELDWORKS","SAFE HAVEN SYSTEMS","FORTIFY & FIT"};
 static const char *nav[]={"FARPOINT NAVIGATION","ORBITAL INSTRUMENTS","TRUE NORTH AVIONICS","THE LONG RANGE SHOP"};
 static const char *hold[]={"BIG BELLY CARGO FITTINGS","PORTSIDE HOLDWORKS","CRATE & CARRIAGE","EXPANDABLE SPACES"};
 static const char *fuel[]={"HELIOS PROPULSION","CORONA SCOOPS & FUEL","THE FUEL EFFICIENT","SUNWARD ENGINEERING"};
 static const char *util[]={"ODD JOBS UTILITY BAY","THE USEFUL BIT","DOCKSIDE GADGETS","SPARE PARTS & SENSE"};
 static const char **names[]={general,weapons,defence,nav,hold,fuel,util};
 int kind=equipment_shop_kind();unsigned h=station_profile_for(&game,game.station_variant).seed;
 return names[kind][(h>>7)%4];
}
static const char *equipment_shop_line(void){
 static const char *line[]={"GENERAL OUTFITTING","WEAPONS + ORDNANCE","DEFENCE + SURVIVAL","NAVIGATION SYSTEMS","CARGO + CABINS","FUEL + PROPULSION","UTILITY + UPGRADES"};
 return line[equipment_shop_kind()];
}
/* Technology icons are authored for their actual 14x14 list footprint.  The
 * category owns the colour; seven silhouettes keep adjacent modules distinct
 * without relying on unreadable interior labels. */
static unsigned tech_icon_ink(int cat){static const unsigned c[]={RGB(225,78,70),RGB(86,151,235),RGB(69,202,190),RGB(224,174,73),RGB(235,125,54),RGB(170,103,218)};return cat>=0&&cat<6?c[cat]:UI_TEXT;}
static void tech_module_icon(int x,int y,int item){
 if(dashboard_item(item)){rect(x+2,y+11,11,2,CHARM_LILAC);rect(x+4,y+4,7,6,CHARM_LILAC);rect(x+6,y+2,3,2,CHARM_LILAC);return;}
 if(charm_item(item)){line(x+7,y,x+7,y+5,CHARM_LILAC);circle(x+7,y+9,4,CHARM_LILAC);pixel(x+6,y+8,WHITE);return;}
 int cat=equip_slot_for(item),shape=item%7;unsigned ink=cat>=0?tech_icon_ink(cat):item==3?UI_GOLD:UI_CYAN,dim=dim_rgb(ink,1,2);
 rect(x,y,14,14,RGB(12,20,29));rect(x,y,14,1,dim);rect(x,y,1,14,dim);
 if(shape==0){line(x+2,y+10,x+11,y+3,ink);line(x+4,y+11,x+13,y+4,ink);}
 else if(shape==1){rect(x+3,y+3,8,2,ink);rect(x+5,y+6,4,6,ink);pixel(x+3,y+10,ink);pixel(x+10,y+10,ink);}
 else if(shape==2){circle(x+7,y+7,5,ink);circle(x+7,y+7,2,dim);pixel(x+7,y+7,WHITE);}
 else if(shape==3){line(x+2,y+11,x+7,y+2,ink);line(x+7,y+2,x+12,y+11,ink);line(x+4,y+8,x+10,y+8,dim);}
 else if(shape==4){rect(x+2,y+5,10,5,ink);rect(x+5,y+2,4,10,dim);pixel(x+7,y+7,WHITE);}
 else if(shape==5){line(x+2,y+7,x+12,y+7,ink);line(x+7,y+2,x+7,y+12,ink);circle(x+7,y+7,3,dim);}
 else {line(x+2,y+3,x+11,y+3,ink);line(x+11,y+3,x+8,y+12,ink);line(x+8,y+12,x+3,y+8,ink);line(x+3,y+8,x+2,y+3,ink);}
}
static void equipment(void){
 header("OUTFITTING");
 if(!game.docked){text(3,8,DIM,"Dock to view equipment.");footer("O BACK");return;}
 rect(8,31,464,13,UI_PANEL);rect(8,31,464,1,UI_GOLD);rect(8,43,464,1,UI_RAISED);
 text_px(16,33,UI_GOLD,"WELCOME TO %.29s",equipment_shop_name());
 text_px(286,33,UI_CYAN,"%.22s",equipment_shop_line());
 panel(8,48,186,182);panel(200,48,272,182);
 int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);
 if(!n){text_px(24,72,DIM,"NO STOCK");text_wrap(26,9,31,4,WHITE,"No modules are available at this hub. You can still sell stored equipment.",0);footer("SQUARE SELL MENU   O BACK");return;}
 if(row<0||row>=n)row=0;int i=list[row],slot=equip_install_slot(i),old=slot>=0?game.fit[slot]:FIT_EMPTY;
 text_px(24,56,UI_CYAN,"LOCAL STOCK");text_px(128,56,DIM,"%02d/%02d",row+1,n);
 int first=row/7*7,shown=n<7?n:7,thumb=140*shown/n;
 rect(12,77,3,140,UI_RAISED);rect(12,77+(140-thumb)*(first<n-shown?first:n-shown)/(n>shown?n-shown:1),3,thumb,UI_ACCENT);
 for(int j=0;j<7&&first+j<n;j++){
  int id=list[first+j],y=78+j*20,active=first+j==row;
  if(active){rect(20,y-3,168,16,UI_RAISED);rect(20,y-3,3,16,UI_GOLD);}
  tech_module_icon(25,y-3,id);text_px(44,y,cosmetic_ink(id,active?WHITE:equipment_owned(id)?UI_CYAN:DIM),"%.15s",equipment_list_names[id]);
  if(equipment_owned(id))text_px(176,y,UI_CYAN,"*");
 }
 text_px(208,56,cosmetic_ink(i,WHITE),"%.27s",equipment_names[i]);tech_module_icon(450,54,i);if(cosmetic_item(i))text_px(208,72,CHARM_LILAC,"%s",cosmetic_label(i));else text_px(208,72,UI_CYAN,"%s / %s",equip_cat_name(i),slot<0?"SERVICE":"MODULE");
 const char *stat=equipment_stat_change(i);text_px(208,88,cosmetic_ink(i,UI_GOLD),cosmetic_item(i)?"APPEARANCE:":"STATS:");text_wrap(26,13,31,2,WHITE,stat,0);
 if(strcmp(stat,equipment_details[i]))text_wrap(26,16,31,3,DIM,equipment_details[i],0);
 if(slot<0)text_px(208,156,UI_CYAN,"STATION SERVICE");
 int net=equipment_net_cost(i);
 text_px(208,200,net>game.credits?RED:UI_CYAN,equipment_owned(i)?"NO PURCHASE NEEDED":net<0?"RETURN %.1f U":"COST: %.1f U",fabsf(net)*.1f);
 text_px(208,216,UI_GOLD,"BALANCE %.1f U",game.credits*.1f);
 if(game.message_time>0)text_wrap(2,29,56,2,UI_GOLD,game.message,0);
 footer("UP/DOWN ITEM   X BUY   SQUARE SELL MENU   O BACK");
}
/* Ship loadout — real fitted slots from fit[]. */
/* Native PSP ship inventory: the pixel silhouette uses the same mesh identity as
 * the 3D ship preview, so every hull stays visually consistent with the yard. */
static void ship_loadout_art(int mesh,int cx,int cy){
 unsigned hull=RGB(154,166,171),edge=RGB(224,181,105),shadow=RGB(43,58,72),glow=RGB(89,184,194);
 int style=mesh%5;
 rect(cx-58,cy-1,116,3,shadow);rect(cx-42,cy+2,84,3,hull);rect(cx-26,cy+5,52,3,hull);
 if(style==0){rect(cx-12,cy-25,24,50,hull);rect(cx-30,cy-9,60,18,hull);rect(cx-48,cy-3,96,6,edge);rect(cx-8,cy-34,16,9,shadow);}
 else if(style==1){rect(cx-8,cy-32,16,64,hull);rect(cx-40,cy-7,80,14,hull);rect(cx-54,cy-2,108,4,edge);rect(cx-18,cy-18,36,36,shadow);}
 else if(style==2){rect(cx-18,cy-22,36,44,hull);rect(cx-48,cy-8,96,16,hull);rect(cx-62,cy-2,124,5,edge);rect(cx-9,cy-34,18,10,shadow);}
 else if(style==3){rect(cx-10,cy-31,20,62,hull);rect(cx-34,cy-14,68,28,hull);rect(cx-58,cy-5,116,10,edge);rect(cx-5,cy-38,10,7,shadow);}
 else {rect(cx-22,cy-18,44,36,hull);rect(cx-54,cy-6,108,12,hull);rect(cx-68,cy-2,136,5,edge);rect(cx-10,cy-29,20,7,shadow);}
 rect(cx-5,cy-4,10,8,glow);rect(cx-2,cy-1,4,3,RGB(205,231,210));
 line(cx-50,cy+13,cx-34,cy+13,shadow);line(cx+34,cy+13,cx+50,cy+13,shadow);
}
static int module_fit_token(int token,int slot);
static int loadout_picker_list(int*out,int cap){int all[EQUIPMENT_COUNT+DB_ITEMS],n=module_owned_list(all,EQUIPMENT_COUNT+DB_ITEMS),count=0;if(loadout_picker_slot<0||loadout_picker_slot>=FIT_SLOTS)return 0;for(int i=0;i<n&&count<cap;i++)if(equip_slot_for(module_item_base(all[i]))==loadout_picker_slot%6)out[count++]=all[i];return count;}
#include "loadout-console.h"
static void inventory_screen(void){loadout_console_draw();}
static int module_selected_item(void){int owned[EQUIPMENT_COUNT],n=module_owned_list(owned,EQUIPMENT_COUNT);if(!n)return -1;if(module_inventory_row<0)module_inventory_row=0;if(module_inventory_row>=n)module_inventory_row=n-1;return owned[module_inventory_row];}
static int module_fit_token(int token,int slot){
 if(token<0||slot<0||slot>=FIT_SLOTS||!fit_slot_open(&game,slot)||equip_slot_for(module_item_base(token))!=slot%6)return 0;
 int item=module_item_base(token);if(cosmetic_item(item)&&cosmetic_installed(&game,item)>=0){message(&game,dashboard_item(item)?"Remove the installed dashboard cosmetic first.":"Remove the installed charm to Item Storage first.");return 0;}if(token<0||!game.docked||!module_item_count(token))return 0;
 int cat=slot%6;
 int old=game.fit[slot],active=game.active_weapon;unsigned oldref=game.dockbay.fitted[slot],ref=token>=EQUIPMENT_COUNT?token-EQUIPMENT_COUNT+1:0;
 if(old==22&&game.passenger_dest>=0){message(&game,"Deliver your passenger before replacing the cabin.");return 0;}
 if(!ref)game.module_store[item]--;game.fit[slot]=item;game.dockbay.fitted[slot]=ref;fit_rebuild(&game);
 if(game.fit[slot]!=item||cargo_used(&game)>cargo_capacity(&game)){game.fit[slot]=old;game.dockbay.fitted[slot]=oldref;game.active_weapon=active;if(!ref)game.module_store[item]++;fit_rebuild(&game);message(&game,"Cannot fit this bank: duplicate module or cargo overflow.");return 0;}
 if(old!=FIT_EMPTY&&!oldref)game.module_store[old]++;if(cat==FIT_WPN)game.active_weapon=slot;fit_rebuild(&game);loadout_store_until=-1;game.dockbay.dirty=1;game.cue=SFX_UI;message(&game,"Module installed. Previous equipment returned to storage.");return 1;
}
static int module_swap_selected(void){int token=module_selected_item();if(token<0)return 0;int cat=equip_slot_for(module_item_base(token)),cap=fit_capacity(game.ship,cat);if(module_bank<0)module_bank=0;if(module_bank>=cap)module_bank=cap-1;return module_fit_token(token,cat+module_bank*6);}

static int module_sell_selected(void){
 int token=module_selected_item();if(token<0||!module_item_count(token)||!game.docked)return 0;
 if(module_sell_item!=token){module_sell_item=token;char note[96];snprintf(note,sizeof(note),"Sell %.45s for %.1f U? X confirms.",module_item_name(token),module_item_resale(token)*.1f);message(&game,note);return 0;}
 int price=module_item_resale(token);if(game.credits>100000000-price)return 0;module_sell_item=-1;if(token>=EQUIPMENT_COUNT)memset(&game.dockbay.owned[token-EQUIPMENT_COUNT],0,sizeof(DockRoll));else game.module_store[token]--;game.credits+=price;game.dockbay.dirty=1;game.cue=SFX_UI;message(&game,"Stored module sold.");return 1;
}
static void storage_screen(void){
 if(!storage_sell_mode){storage_console_draw();return;}
 header(storage_sell_mode?"OUTFITTING / SELL STORED MODULES":"SHIP TECH BOARD / ITEM STORAGE");
 static const char *cats[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
 int owned[EQUIPMENT_COUNT],n=module_owned_list(owned,EQUIPMENT_COUNT),item=module_selected_item();
 int cat=item>=0?equip_slot_for(module_item_base(item)):(row>=0?row%6:FIT_WPN),cap=fit_capacity(game.ship,cat);if(cap<1)cap=1;if(module_bank>=cap)module_bank=cap-1;
 int fitted=0,total=0;for(int c=0;c<6;c++)total+=fit_capacity(game.ship,c);for(int s=0;s<FIT_SLOTS;s++)fitted+=fit_slot_open(&game,s)&&game.fit[s]!=FIT_EMPTY;
 text_px(16,29,UI_CYAN,"%.22s",player_ships[game.ship].name);text_px(294,29,UI_MUTED,"FITTED %d/%d",fitted,total);text_px(400,29,UI_GOLD,"%.1f U",game.credits*.1f);
 panel(8,46,190,196);panel(206,46,266,196);text_px(16,53,UI_CYAN,"%s BANKS",cats[cat]);text_px(216,53,UI_GOLD,"ITEM STORAGE");text_px(392,53,UI_MUTED,"%d/%d",module_store_total(),module_store_capacity());
 for(int b=0;b<cap;b++){
  int y=68+b*20,slot=cat+b*6,selected=b==module_bank,mod=game.fit[slot];
  if(selected&&!storage_sell_mode&&storage_bank_focus){rect(16,y-2,174,18,UI_RAISED);rect(16,y-2,3,18,UI_GOLD);}text_px(24,y,selected&&!storage_sell_mode&&storage_bank_focus?UI_GOLD:UI_CYAN,"%s B%d",cats[cat],b+1);
  if(mod==FIT_EMPTY)text_px(88,y,UI_MUTED,"EMPTY");else{text_px(88,y,selected&&!storage_sell_mode&&storage_bank_focus?UI_TEXT:UI_MUTED,"%.12s",equipment_list_names[mod]);if(cat==FIT_WPN&&slot==game.active_weapon)text_px(176,y,UI_SIGNAL,"A");}
 }
 if(!n){text_px(224,84,UI_MUTED,"NO STORED MODULES");text_wrap(28,13,28,5,UI_TEXT,storage_sell_mode?"You have no stored modules for this outfitter to buy.":"Buy modules through Outfitting, then fit them here.",0);}
 else {
  int visible=6,first=module_inventory_row<visible?0:module_inventory_row-visible+1;if(first>n-visible)first=n-visible;if(first<0)first=0;
  rect(210,68,3,117,UI_RAISED);int thumb=n<=visible?117:117*visible/n;if(thumb<8)thumb=8;int sy=n>visible?68+(117-thumb)*first/(n-visible):68;rect(210,sy,3,thumb,UI_CYAN);
  for(int j=0;j<visible&&first+j<n;j++){int idx=first+j,id=owned[idx],y=68+j*20,active=idx==module_inventory_row,stored=module_item_count(id);if(active){rect(216,y-2,242,18,UI_RAISED);rect(216,y-2,3,18,storage_bank_focus&&!storage_sell_mode?UI_CYAN:UI_GOLD);}tech_module_icon(222,y-2,module_item_base(id));module_rare_shine(222,y-2,16,16,id);text_px(241,y,cosmetic_ink(module_item_base(id),active?(storage_bank_focus&&!storage_sell_mode?UI_CYAN:UI_GOLD):UI_TEXT),"%.13s",module_item_short(id));if(storage_sell_mode)text_px(354,y,UI_MUTED,"%.1f U",module_item_resale(id)*.1f);else text_px(354,y,UI_MUTED,"X%d STORED",stored);text_px(432,y,active?UI_CYAN:UI_MUTED,"%s",equip_cat_name(module_item_base(id)));}
  line(216,190,458,190,UI_EDGE);text_px(216,198,cosmetic_ink(module_item_base(item),UI_GOLD),"%.28s",module_item_name(item));if(cosmetic_item(module_item_base(item)))text_px(216,182,CHARM_LILAC,"%s",cosmetic_label(module_item_base(item)));
  if(storage_sell_mode){text_px(216,211,UI_TEXT,"STATION BUYS FOR %.1f U",module_item_resale(item)*.1f);text_px(216,224,UI_SIGNAL,"%d STORED",module_item_count(item));}
  else if(module_item_count(item)&&storage_bank_focus){int slot=cat+module_bank*6,old=game.fit[slot];text_px(216,211,UI_TEXT,"CHOOSE %s BANK %d",cats[cat],module_bank+1);if(old==FIT_EMPTY)text_px(216,224,UI_SIGNAL,"EMPTY BANK / X TO FIT");else text_px(216,224,UI_SIGNAL,"REPLACE %.18s",equipment_list_names[old]);}
  else if(module_item_count(item)){text_px(216,211,UI_TEXT,"X SELECT THIS MODULE");text_px(216,224,UI_MUTED,"THEN CHOOSE AN AVAILABLE BANK");}
  else text_px(216,211,UI_MUTED,"NO STORED COPY");
 }
 footer(storage_sell_mode?(module_sell_item>=0?"X CONFIRM SALE   O CANCEL":"UP/DOWN ITEM   X SELL   O BACK"):(storage_bank_focus?"UP/DOWN BANK   X FIT   O ITEMS":"UP/DOWN ITEM   X SELECT   O BACK"));
}
static int engineer_fuel_cost(void){return (int)ceilf(fmaxf(0,player_ships[game.ship].range-game.fuel))*2;}
static void engineer_refuel(void){
 if(!game.docked){message(&game,"Dock to refuel.");return;}
 int cost=engineer_fuel_cost();
 if(!cost){message(&game,"Tank is already full.");return;}
 if(game.credits<cost){message(&game,"Not enough units to refuel.");return;}
 game.credits-=cost;refuel_full(&game);game.cue=SFX_UI;message(&game,"Fuel tank filled.");
}
static void engineer_status_box(int x,int y,int w,const char *label,const char *value,unsigned edge,unsigned value_ink){
 rect(x,y,w,25,RGB(4,10,16));rect(x,y,w,1,edge);rect(x,y+24,w,1,edge);rect(x,y,1,25,edge);rect(x+w-1,y,1,25,edge);
 text_px(x+7,y+5,WHITE,"%s",label);text_px(x+w-7-(int)strlen(value)*8,y+14,value_ink,"%s",value);
}
static void engineer_service_button(int x,int y,int w,char icon,const char *label,const char *price,int active){
 unsigned edge=active?UI_GOLD:UI_EDGE,fill=active?RGB(57,43,25):UI_PANEL,ink=active?WHITE:UI_MUTED;
 rect(x,y,w,42,fill);rect(x,y,w,1,edge);rect(x,y+41,w,1,edge);rect(x,y,1,42,edge);rect(x+w-1,y,1,42,edge);
 rect(x+4,y+4,35,34,RGB(4,10,16));rect(x+4,y+4,35,1,edge);rect(x+4,y+37,35,1,edge);rect(x+4,y+4,1,34,edge);rect(x+38,y+4,1,34,edge);
 button_icon(x+16,y+15,icon,edge);text_px(x+46,y+7,ink,"%s",label);text_px(x+46,y+22,active?UI_CYAN:UI_MUTED,"%s",price);
}
static void repair_screen(void){
 header("ENGINEER / DIAGNOSTIC");
 int hull=(int)game.hull,shield=(int)game.energy,repair=ship_repair_cost(&game),fuel_cost=engineer_fuel_cost();
 int max_fuel=player_ships[game.ship].range,fuel_pct=max_fuel>0?(int)(game.fuel*100.f/max_fuel):0;if(fuel_pct>100)fuel_pct=100;
 char hull_value[12],shield_value[12],fuel_value[20],sink_value[12],repair_price[18],fuel_price[18];
 snprintf(hull_value,sizeof(hull_value),"%d%%",hull);snprintf(shield_value,sizeof(shield_value),"%d%%",shield);
 snprintf(fuel_value,sizeof(fuel_value),"%.1f/%.1f",game.fuel*.1f,max_fuel*.1f);snprintf(sink_value,sizeof(sink_value),"%s",repair>0?"SERVICE":"NOMINAL");
 snprintf(repair_price,sizeof(repair_price),repair>0?"%.1f U":"NO WORK",repair*.1f);snprintf(fuel_price,sizeof(fuel_price),fuel_cost>0?"%.1f U":"FULL",fuel_cost*.1f);
/* Left: the current hull and livery, using the Shipyard's actual model.
  * The aperture reserves the surrounding diagnostic labels at native size. */
 panel(8,31,278,161);text_px(16,39,UI_CYAN,"%.19s / SERVICE SCAN",player_ships[game.ship].name);
 for(int x=18;x<278;x+=16)line(x,55,x,183,RGB(16,42,52));for(int y=55;y<184;y+=16)line(18,y,277,y,RGB(16,42,52));
 line(96,112,61,87,UI_GOLD);line(196,112,231,87,UI_CYAN);line(118,151,76,170,UI_SIGNAL);line(175,151,215,170,AMBER);
 yc_ship(game.ship,24,91,246,64);
 engineer_status_box(18,64,83,"HULL",hull_value,UI_GOLD,hull<35?RED:UI_GOLD);
 engineer_status_box(191,64,86,"SHIELDS",shield_value,UI_CYAN,UI_CYAN);
 engineer_status_box(18,158,91,"FUEL",fuel_value,UI_SIGNAL,UI_SIGNAL);
 engineer_status_box(187,158,90,"HEAT SINK",sink_value,AMBER,repair>0?UI_GOLD:UI_SIGNAL);
 /* Right: the same live values in quickly readable bars. */
 panel(293,31,179,161);text_px(304,39,UI_CYAN,"LIVE DIAGNOSTICS");
 text_px(304,60,DIM,"HULL");text_px(424,60,hull<35?RED:UI_GOLD,"%d%%",hull);pip_bar(304,72,152,6,hull,hull<35?RED:UI_GOLD);
 text_px(304,91,DIM,"SHIELDS");text_px(424,91,UI_CYAN,"%d%%",shield);pip_bar(304,103,152,6,shield,UI_CYAN);
 text_px(304,122,DIM,"FUEL");text_px(392,122,UI_SIGNAL,"%.1f / %.1f",game.fuel*.1f,max_fuel*.1f);pip_bar(304,134,152,6,fuel_pct,UI_SIGNAL);
 text_px(304,153,DIM,"HEAT SINKS");text_px(416,153,repair>0?UI_GOLD:UI_SIGNAL,"%s",repair>0?"SERVICE":"OK");
 text_px(304,172,DIM,"BALANCE");text_px(376,172,WHITE,"%.1f U",game.credits*.1f);
 engineer_service_button(8,199,228,'X',"REPAIR SHIP",repair_price,game.docked&&repair>0);
 engineer_service_button(244,199,228,'T',"REFUEL",fuel_price,game.docked&&fuel_cost>0);
 if(game.message_time>0){rect(96,181,288,14,RGB(6,12,22));rect(96,181,288,1,UI_GOLD);text_px(104,184,UI_GOLD,"%.34s",game.message);}
 footer(game.docked?"X REPAIR   TRI REFUEL   O BACK":"DOCK AT A STATION   O BACK");
}

static void status(void);
static void communications(void){
 header("COMMS / STATION CHANNEL");panel(8,32,232,156);panel(248,32,224,156);
 draw_station_badge(18,48);draw_portrait(70,48,70,62,VOICE_VENN*37,TRADERS);
 int hub=docking_target_hub();float distance=length(sub(game.pos,hub_position(&game,hub))),range=station_comms_range(&game,hub);
 int cheap=0,dear=0,traders=0,cops=0,raiders=0;
 for(int i=1;i<GOODS;i++){if(game.price[i]-galactic_price(i)<game.price[cheap]-galactic_price(cheap))cheap=i;if(game.price[i]-galactic_price(i)>game.price[dear]-galactic_price(dear))dear=i;}
 for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive){if(game.npc[i].role==TRADERS)traders++;else if(game.npc[i].role==LAW)cops++;else if(game.npc[i].role==PIRATES)raiders++;}
 text(2,15,UI_CYAN,"%.22s",station_name_for(&game,docking_target_hub()));
 if(comms_rescue_confirm)text(2,17,UI_GOLD,"Fee: up to 50 units");else text(2,17,WHITE,"%d m / %d m",(int)distance,(int)range);
 text(2,19,UI_GOLD,comms_rescue_confirm?"X confirm recovery":game.docked?"Already docked.":distance<=range?"X request docking":"Close in to hail.");
 text(32,5,UI_CYAN,"DOCKMASTER VENN");
 text(32,7,WHITE,game.docked?"Buy  %.12s":"Dock to view the market.",goods[cheap].name);
 if(game.docked)text(32,8,UI_CYAN,"v cheap vs galaxy");
 if(game.docked)text(32,10,WHITE,"Sell %.12s",goods[dear].name);
 if(game.docked)text(32,11,RED,"^ dear vs galaxy");
 text(32,13,WHITE,"Traffic T%d L%d P%d",traders,cops,raiders);
 text(32,15,game.legal?RED:UI_CYAN,"Warrant [%s]",stars(wanted_level(&game)));
 text(32,17,UI_GOLD,"Board jobs %d",mission_count(&game));
 if(valid_target(selected_target)&&selected_target>0)text(32,19,WHITE,"Lock %.14s",target_name(selected_target));
 else text(32,19,DIM,"No ship locked.");
 footer("X DOCK   TRI RESCUE   SQUARE RADIO   O BACK");
}
static int system_ops_bits(unsigned value){int n=0;while(value){n+=(int)(value&1u);value>>=1;}return n;}
static int system_ops_species(int sys,int body){int n=0;for(int i=0;i<LIFE_COUNT;i++)n+=field_species_logged(&game,sys,body,i);return n;}
static int system_ops_sites(int sys,int body){int n=0;for(int id=1;id<10;id++){unsigned bit=field_site_bit(id);n+=bit&&(game.surface_progress[sys][body]&bit)!=0;}return n;}
static int system_ops_local_jobs(int sys){int n=0;for(int i=0;i<game.job_n;i++)n+=game.jobs[i].dest==sys;return n;}
static int system_ops_done(int sys){
 int done=system_ops_bits(game.landed_planets[sys]&0x0fffu)+system_ops_bits(game.rift_logged[sys]&15u)+system_ops_bits(game.bounty_claimed[sys]&31u);
 for(int body=1;body<BODY_COUNT;body++)if(system_body_active_slot(sys,body)&&system_body_type_slot(sys,body)!=SUN)done+=system_ops_species(sys,body)+system_ops_sites(sys,body);
 done+=(game.station_progress[sys][0]&3u)==2u;return done;
}
static const char *system_ops_body_kind(int type){return type==SUN?"PRIMARY STAR":type==OCEAN?"OCEAN WORLD":type==GAS?"GAS GIANT / SKYPORT":"ROCKY WORLD";}
static const char *system_ops_station_family(int family){static const char *names[]={"HABITAT WHEEL","TWIN SAUCER","GLOBE CITADEL","ROCKET CATHEDRAL","ORBITAL LINER","DOUBLE WHEEL","CRYSTAL PALACE","ORBITAL PALACE"};return names[family%8];}
static const char *system_ops_next(void){
 static char note[64];int sys=game.system,station=(int)(game.station_progress[sys][0]&3u);
 for(int i=0;i<game.job_n;i++)if(game.jobs[i].dest==sys){snprintf(note,sizeof(note),"MISSION HERE: %s / OPEN MISSION LOG",mission_name(game.jobs[i].type));return note;}
 if(wanted_level(&game)){snprintf(note,sizeof(note),"LOCAL WARRANT: PAY %.1f U AT LAW DESK",police_fine(&game)*.1f);return note;}
 if(station==1){snprintf(note,sizeof(note),"STATION ACTIVITY: CONTINUE THE CARGO MANIFEST LEAD");return note;}
 int wanted=BOUNTY_POSTER_COUNT-system_ops_bits(game.bounty_claimed[sys]&31u);if(wanted){snprintf(note,sizeof(note),"BOUNTY BOARD: %d LOCAL TARGET%s REMAIN",wanted,wanted==1?"":"S");return note;}
 for(int body=1;body<BODY_COUNT;body++)if(system_body_active_slot(sys,body)&&system_body_type_slot(sys,body)!=SUN&&!(game.landed_planets[sys]&(1u<<(body-1)))){snprintf(note,sizeof(note),"SURVEY NEXT: %.25s",game.bodies[body].name);return note;}
 for(int body=1;body<BODY_COUNT;body++)if(system_body_active_slot(sys,body)&&system_body_type_slot(sys,body)!=SUN){int life=system_ops_species(sys,body),sites=system_ops_sites(sys,body);if(life<LIFE_COUNT||sites<8){snprintf(note,sizeof(note),"FIELDWORK: %.20s / LIFE %d/8 SITES %d/8",game.bodies[body].name,life,sites);return note;}}
 int rifts=system_ops_bits(anomaly_slots_mask(&game,sys)&~game.rift_logged[sys]);if(rifts){snprintf(note,sizeof(note),"DEEP SCAN: %d NATURAL SIGNAL%s UNLOGGED",rifts,rifts==1?"":"S");return note;}
 if(station==0){snprintf(note,sizeof(note),"STATION ACTIVITY: ASK ABOUT THE MISSING MANIFEST");return note;}
 snprintf(note,sizeof(note),"SYSTEM RECORD COMPLETE / CHOOSE A NEW ROUTE");return note;
}
static void system_ops_meter(int x,int y,int value,int total,unsigned ink){
 if(total<1)total=1;if(value<0)value=0;if(value>total)value=total;rect(x,y,96,5,RGB(20,31,40));rect(x,y,96*value/total,5,ink);rect(x+96,y,1,5,UI_RAISED);
}
static int system_route_view=0;
static void system_route_map(void){
 header("SYSTEM / TRAFFIC ROUTES");panel(8,30,464,180);
 float extent=1;for(int r=0;r<4;r++)for(int k=0;k<3;k++){Vec3 p=game.traffic_nodes[r][k];extent=fmaxf(extent,fmaxf(fabsf(p.x),fabsf(p.z-3500)));}
 float scale=52/extent; /* Keep every route marker above the target caption. */
 for(int r=0;r<4;r++){
  unsigned ink=row==r+2?UI_GOLD:r<2?UI_CYAN:AMBER;
  for(int k=0;k<2;k++){Vec3 a=game.traffic_nodes[r][k],b=game.traffic_nodes[r][k+1];line(164+a.x*scale,119-(a.z-3500)*scale,164+b.x*scale,119-(b.z-3500)*scale,ink);}
  Vec3 p=game.traffic_nodes[r][2];circle(164+p.x*scale,119-(p.z-3500)*scale,3,ink);text_px(170+p.x*scale,113-(p.z-3500)*scale,ink,"%d",r+1);
  text_px(286,47+r*34,ink,"%d %.20s",r+1,game.bodies[r+1].name);
  text_px(286,61+r*34,UI_MUTED,r<2?"MAIN / PATROLLED":"OUTER / EXPOSED");
 }
 rect(161,116,6,6,WHITE);text_px(104,126,WHITE,"STATION");
 if(fabsf(game.pos.x)*scale<140&&fabsf(game.pos.z-3500)*scale<78){int x=164+game.pos.x*scale,y=119-(game.pos.z-3500)*scale;line(x-3,y,x+3,y,UI_GOLD);line(x,y-3,x,y+3,UI_GOLD);}
 text_px(20,178,UI_GOLD,"TARGET: %.28s",row==0?station_name_for(&game,0):game.bodies[row-1].name);
 text_px(20,193,UI_MUTED,"TOP-DOWN / HEIGHT OMITTED / GOLD CROSS = YOU");
 text_px(16,219,UI_CYAN,"LAW SCANNER: CIRCLE + R SELECT / CIRCLE SCAN");
 text_px(16,235,UI_MUTED,"PATROLS MOVE. OUTER ROUTES ARE NOT GUARANTEED SAFE.");
 footer("SQUARE OVERVIEW   UP/DOWN TARGET   X LOCK   O BACK");
}
static void almanac_ellipse(int cx,int cy,int rx,int ry,unsigned ink,int offset){
 for(int i=0;i<128;i++){if(((i+offset)&3)!=0)continue;float a=i*6.2831853f/128.f;int x=cx+(int)(cosf(a)*rx),y=cy+(int)(sinf(a)*ry);if(x>10&&x<306&&y>31&&y<218)pixel(x,y,ink);}
}
static void almanac_brackets(int x,int y,int r,unsigned ink){
 line(x-r,y-r,x-r+5,y-r,ink);line(x-r,y-r,x-r,y-r+5,ink);line(x+r,y-r,x+r-5,y-r,ink);line(x+r,y-r,x+r,y-r+5,ink);
 line(x-r,y+r,x-r+5,y+r,ink);line(x-r,y+r,x-r,y+r-5,ink);line(x+r,y+r,x+r-5,y+r,ink);line(x+r,y+r,x+r,y+r-5,ink);
}
static void almanac_label_glass(int x,int y,int w,int h){
 /* A selected world can sit directly over its bright primary. Blend a dark
  * navy plate at 70% so the map remains visible but its name always reads. */
 const unsigned ink=RGB(10,18,29);const int alpha=179,inv=255-alpha;
 int x0=x<0?0:x,y0=y<0?0:y,x1=x+w>W?W:x+w,y1=y+h>H?H:y+h;
 for(int yy=y0;yy<y1;yy++)for(int xx=x0;xx<x1;xx++){
  unsigned old=fb[yy*STRIDE+xx];
  int r=((old&255)*inv+(ink&255)*alpha)/255;
  int g=(((old>>8)&255)*inv+((ink>>8)&255)*alpha)/255;
  int b=(((old>>16)&255)*inv+((ink>>16)&255)*alpha)/255;
  fb[yy*STRIDE+xx]=RGB(r,g,b);
 }
}
static void almanac_callout(int object_x,int object_y,const char *name){
 /* One full-width instrument label leaves enough room for long generated
  * names and station classes. Its fixed position also prevents a selected
  * body's callout from covering another world in crowded systems. */
 const int box_x=18,box_y=38,width=280,height=22;
 int edge_x=object_x<box_x?box_x:object_x>box_x+width?box_x+width:object_x;
 int edge_y=box_y+height;
 line(object_x,object_y,edge_x,edge_y,UI_CYAN);
 line(object_x,object_y-1,edge_x,edge_y-1,UI_CYAN);
 almanac_label_glass(box_x+1,box_y+1,width-1,height-1);
 line(box_x,box_y,box_x+width,box_y,UI_CYAN);
 line(box_x,box_y+height,box_x+width,box_y+height,UI_CYAN);
 line(box_x,box_y,box_x,box_y+height,UI_CYAN);line(box_x+width,box_y,box_x+width,box_y+height,UI_CYAN);
 text_px(box_x+7,box_y+7,WHITE,"%.42s",name);
}
static void almanac_station_art(int cx,int cy){
 StationProfile p=station_profile_for(&game,0);int kind=p.station_class;
 if(kind==STATION_POOR)draw_next_art(almanac_station_poor,ALMANAC_POOR_W,ALMANAC_POOR_H,cx-ALMANAC_POOR_W/2,cy-ALMANAC_POOR_H/2,ALMANAC_POOR_W,ALMANAC_POOR_H);
 else if(kind==STATION_RICH)draw_next_art(almanac_station_rich,ALMANAC_RICH_W,ALMANAC_RICH_H,cx-ALMANAC_RICH_W/2,cy-ALMANAC_RICH_H/2,ALMANAC_RICH_W,ALMANAC_RICH_H);
 else draw_next_art(almanac_station_mega,ALMANAC_MEGA_W,ALMANAC_MEGA_H,cx-ALMANAC_MEGA_W/2,cy-ALMANAC_MEGA_H/2,ALMANAC_MEGA_W,ALMANAC_MEGA_H);
 /* Authored hulls stay fixed; only practical lights pulse so silhouettes remain crisp. */
 if(((int)(preview_time*3)+(p.seed&7))&3){pixel(cx-13,cy+5,p.light);pixel(cx+14,cy+5,p.light);}
}
static const char *almanac_government(int sys){
 static const char *law[]={"LAWFUL COOPERATIVE","LAWFUL CHARTER","LAWFUL FEDERATION"};
 static const char *pirate[]={"PIRATES / CLANS","PIRATES / FREEHOLD","PIRATES / SYNDICATE"};
 unsigned h=station_profile_hash(sys,9);return system_is_lawful(&game,sys)?law[h%3]:pirate[h%3];
}
static void almanac_description(int sys,char *out,int cap){
 if(opening_sol(&game)){snprintf(out,cap,"Sol, 2050. Earth is home. Eight planets orbit the Sun. Earth Orbital handles local trade and family flights.");return;}
 int kind=station_class_for_system(&game,sys),lawful=system_is_lawful(&game,sys);
 if(sys==7){snprintf(out,cap,"Lave is a bright lawful crossroads. Its orbital markets welcome traders while old survey routes lead toward four distant worlds.");return;}
 static const char *forms[]={"a varied planetary system","a solitary-star beacon with no planets","a giant attended by eight small moons","a realm of three gas giants","an asteroid-dominated planetoid belt","a binary-star system","a rare triple-star system","a red dwarf with frozen outer worlds","a dead system of scorched rocks","a crowded chain of habitable worlds","a sunless cluster of rogue planets","a turbulent gas-giant nursery"};
 snprintf(out,cap,"%s is %s under %s control. Its %s station supports traffic across this unusual local geography.",game.systems[sys].name,forms[system_archetype(sys)],lawful?"lawful":"pirate-held",kind==STATION_MEGA?"capital":kind==STATION_POOR?"frontier":"trade");
}
static void system_details(void){
 int active=system_body_slots(game.system);if(row<0||row>active)row=0;
 int sys=game.system,offset=(int)(preview_time*4);char description[192];
 int px[BODY_COUNT]={0},py[BODY_COUNT]={0},pr[BODY_COUNT]={0};
 header("SYSTEM ALMANAC");panel(8,29,300,199);panel(314,29,158,199);
 draw_next_art(almanac_backdrop,ALMANAC_BACKDROP_W,ALMANAC_BACKDROP_H,11,32,ALMANAC_BACKDROP_W,ALMANAC_BACKDROP_H);
 for(int i=0;i<18;i++){unsigned h=station_profile_hash(sys,i+23);int x=14+(h%288),y=35+((h>>9)%185),twinkle=((int)(preview_time*2)+(h>>18))%7;if(twinkle<2)pixel(x,y,twinkle?WHITE:UI_CYAN);}
 for(int i=1;i<active&&i<5;i++)almanac_ellipse(72,128,40+i*28,13+i*6,RGB(96,96,82),offset+i*7);
 for(int i=0;i<active;i++){
  Body *b=&game.bodies[i];unsigned h=b->seed;px[i]=31+(i%5)*56+(int)(sinf(preview_time*.10f+i*1.7f)*2);py[i]=i<5?94:148;py[i]+=(int)(cosf(preview_time*.12f+i)*2);
  pr[i]=b->type==SUN?18:b->type==GAS?15:b->type==OCEAN?12:10;
  draw_planet_disc(px[i],py[i],pr[i],b->color,b->accent,h,b->type);
 }
 almanac_station_art(145,193);
 {unsigned h=station_profile_hash(sys,77);int sx=86+(int)fmodf(preview_time*19+(h%190),208),sy=54+((h>>9)%126);line(sx-10,sy,sx-3,sy,RGB(40,93,110));line(sx-5,sy-1,sx,sy,UI_CYAN);pixel(sx+1,sy,WHITE);}
 if(row==0)almanac_brackets(145,193,station_class(&game)==STATION_MEGA?32:station_class(&game)==STATION_RICH?29:25,UI_GOLD);
 else almanac_brackets(px[row-1],py[row-1],pr[row-1]+5,UI_GOLD);
 if(row==0)almanac_callout(145,193,station_name_for(&game,0));
 else almanac_callout(px[row-1],py[row-1],game.bodies[row-1].name);
 text_px(324,40,UI_GOLD,"%.20s",game.systems[sys].name);line(324,55,462,55,UI_RAISED);
 almanac_description(sys,description,sizeof(description));text_wrap(41,8,17,7,WHITE,description,0);
 line(324,127,462,127,UI_RAISED);
 text_px(324,137,UI_CYAN,"GOVERNMENT");text_px(324,150,system_is_lawful(&game,sys)?WHITE:RED,"%.22s",almanac_government(sys));
 text_px(324,171,UI_CYAN,"ECONOMY");text_px(324,184,UI_GOLD,"%s",station_class_name(station_class(&game)));
 {int worlds=0,stars=0;for(int i=0;i<active;i++)if(game.bodies[i].type==SUN)stars++;else worlds++;text_px(324,205,UI_CYAN,"STARS / WORLDS");text_px(324,217,WHITE,"%d / %d",stars,opening_sol(&game)?8:worlds);}
 if(opening_sol(&game)){static const char *extra[]={"MERCURY","VENUS","SATURN","URANUS"};for(int i=0;i<4;i++){int x=35+i*68;draw_planet_sprite(x,208,4+i,23+i, i<2?ROCKY:GAS,11,32,305,225);text_px(x-18,217,DIM,"%s",extra[i]);}}
 footer("UP/DOWN WORLD   X TARGET + RETURN   O BACK");
}
#include "gazette-lore.h"
#include "faction-dossier.h"
static void mission_upper(char *out,int cap,const char *in){int i=0;for(;i<cap-1&&in[i];i++){char c=in[i];out[i]=c>='a'&&c<='z'?c-32:c;}out[i]=0;}
static int mission_long_family(int type){return type>=EXPEDITION_FIRST|| type==MISSION_CONVOY||type==MISSION_ARCHAEOLOGY||type==MISSION_DISTRESS||type==MISSION_MULTI_STOP||type==MISSION_FREIGHTER_DEFENCE||type==MISSION_DERELICT;}
static const char *mission_bonus_copy(int type){
 if(type==MISSION_CONVOY)return "ALL SHIPS SURVIVE";if(type==MISSION_PATROL)return "SCAN ALL CONTACTS";
 if(type==MISSION_WILDLIFE)return "NO WEAPONS FIRED";if(type==MISSION_DELIVERY)return "CARGO INTACT";
 if(type==MISSION_BOUNTY||type==MISSION_POLICE_ASSIST)return "ID TARGET FIRST";
 if(type==MISSION_REPAIR)return "PREVENT 2ND FAILURE";return "NONE";
}
static const char *mission_card_name(int type){if(type>=EXPEDITION_FIRST)return mission_name(type);
 static const char *short_name[MISSION_TYPES]={
  "CARGO DELIVERY","BOUNTY HUNT","EXPLORATION SURVEY","PILOT RESCUE","COVERT DELIVERY","CONVOY ESCORT","BEACON REPAIR","SALVAGE RECOVERY","WILDLIFE SURVEY","ANOMALY INQUIRY","MINING CONTRACT","DISTRESS RESPONSE","SYSTEM PATROL","CARGO RECOVERY","MISSING PERSON","TRADE PROCUREMENT","MOBILE MECHANIC","BLACK-BOX RECOVERY","PLANET SAMPLES","LISTENING POST","POLICE ASSISTANCE","PASSENGER RUN","MEMORIAL DELIVERY","MULTI-STOP LOGISTICS","FREIGHTER DEFENCE","DERELICT SURVEY","SPACE RACE"};
 return type>=0&&type<MISSION_TYPES?short_name[type]:"UNKNOWN CONTRACT";
}
static void mission_board_objective(char *out,int cap,int type,const char *dest){
 int base=mission_base_type(type);
 if(type>=EXPEDITION_FIRST)snprintf(out,cap,"%s",expedition_def(type)->stages[0].objective);
 else if(base==MISSION_RACE)snprintf(out,cap,"Meet the start grid. Fly all 32 rings in order. Finish first to win.");
 else if(base==MISSION_BOUNTY)snprintf(out,cap,"Find and destroy the marked hostile near %s.",dest);
 else if(base==MISSION_EXPLORATION)snprintf(out,cap,"Fly to the marked world near %s and scan it.",dest);
 else if(base==MISSION_RESCUE)snprintf(out,cap,"Find the marked contact near %s. Hail within 600 m, then dock.",dest);
 else if(base==MISSION_SMUGGLING)snprintf(out,cap,"Deliver the sealed cargo to %s Hub without losing it.",dest);
 else snprintf(out,cap,"Deliver the reserved mission cargo to %s Hub.",dest);
}
static void mission_card_icon(int x,int y,int type,unsigned ink){
 int base=mission_base_type(type);
 if(base==MISSION_RACE){line(x-5,y-7,x-5,y+7,ink);for(int a=0;a<4;a++)for(int b=0;b<3;b++)rect(x-4+a*3,y-7+b*3,3,3,(a+b)&1?ink:WHITE);}
 else if(base==MISSION_BOUNTY){circle(x,y,5,ink);line(x-7,y,x+7,y,ink);line(x,y-7,x,y+7,ink);}
 else if(base==MISSION_EXPLORATION){circle(x,y,6,ink);line(x-4,y+5,x+5,y-4,ink);pixel(x+3,y-3,WHITE);}
 else if(base==MISSION_RESCUE){rect(x-5,y-5,10,10,ink);rect(x-1,y-7,2,14,WHITE);rect(x-7,y-1,14,2,WHITE);}
 else if(base==MISSION_SMUGGLING){line(x-6,y+4,x+5,y-5,ink);rect(x+3,y-6,4,4,ink);rect(x-7,y+3,4,4,ink);}
 else {rect(x-6,y-4,12,9,ink);rect(x-3,y-7,6,3,ink);pixel(x-3,y+1,WHITE);pixel(x+3,y+1,WHITE);}
}
/* Native 8px glyphs, centred inside the 172px confirmation card. */
static void mission_confirm_text(int y,unsigned ink,const char *s){
 while(*s){
  int len=(int)strlen(s),cut=len<18?len:18;
  if(len>18){for(int k=cut;k>0;k--)if(s[k]==' '){cut=k;break;}}
  text_px(386-cut*4,y,ink,"%.*s",cut,s);
  s+=cut;while(*s==' ')s++;y+=10;
 }
}
static void mission_confirm_card(int type,int dest,unsigned frame,unsigned rule){
 mission_confirm_text(55,UI_GOLD,"ARE YOU SURE?");rect(316,71,140,1,rule);
 char prompt[64],place[40];snprintf(prompt,sizeof(prompt),"ACCEPT %s?",mission_card_name(type));
 mission_confirm_text(80,WHITE,prompt);
 snprintf(place,sizeof(place),"IN %s",game.systems[dest].name);mission_confirm_text(112,UI_CYAN,place);
 int pressed=ui_press_anim>0&&ui_press_source==MISSIONS&&ui_press_row==row;
 unsigned pulse=pressed?RGB(94,235,167):UI_GOLD;
 unsigned yes=mission_accept_yes?pulse:frame,no=mission_accept_yes?frame:pulse;
 rect(316,132,62,30,yes);rect(318,134,58,26,RGB(18,28,38));text_px(347-3*4,143,mission_accept_yes?UI_GOLD:DIM,"YES");
 rect(394,132,62,30,no);rect(396,134,58,26,RGB(18,28,38));text_px(425-2*4,143,!mission_accept_yes?UI_GOLD:DIM,"NO");
 mission_confirm_text(181,DIM,"CHOOSE BEFORE ADDING THIS JOB TO YOUR LOG.");
}
/* Display order is independent of the stable contract IDs used in saves. */
static int mission_board_offer(int index){return game.system==7?(index==0?5:index-1):index;}
static void mission_feature_shine(int x,int y,int w,int h){
 const unsigned rainbow[]={RGB(245,91,104),RGB(255,180,77),RGB(245,229,107),RGB(102,235,147),RGB(84,205,246),RGB(176,135,247)};
 float cycle=fmodf(preview_time,5.f);int sweep=(int)(cycle*110)-55;
 for(int row=1;row<h-1;row++){int left=sweep-row/2;
  for(int band=0;band<6;band++){int a=left+band*6,b=a+6;if(a<x+1)a=x+1;if(b>x+w-1)b=x+w-1;if(b>a)rect(a,y+row,b-a,1,planet_rgb_mix(RGB(24,34,45),rainbow[band],48));}
 }
 for(int k=0;k<6;k++)rect(x+k*w/6,y,w/6+1,1,rainbow[k]);
}
static void mission_board(void){
 header("MISSION BOARD");text_px(386,9,UI_CYAN,"%.14s HUB",game.systems[game.system].name);
 if(!game.docked){panel(8,29,464,211);text_px(24,52,UI_GOLD,"MISSION NETWORK OFFLINE");text_px(24,72,DIM,"DOCK AT A STATION TO VIEW LOCAL CONTRACTS.");footer("O BACK");return;}
 mission_queries_begin(&game);
 int count=mission_count(&game);if(row<0||row>=count)row=0;
 unsigned frame=RGB(63,102,121),card=RGB(13,21,30),selected=RGB(24,34,45),rule=RGB(165,113,54);
 rect(8,28,284,212,frame);rect(9,29,282,210,RGB(8,15,22));
 rect(300,28,172,212,frame);rect(301,29,170,210,RGB(13,21,30));
 int first=row>4?row-4:0;
 if(count>5){rect(9,32,2,191,frame);rect(9,32+first*32,2,159,UI_GOLD);}
 for(int shown=0;shown<5&&first+shown<count;shown++){int i=first+shown,offer=mission_board_offer(i);
  int y=32+shown*39+(ui_press_anim>0&&ui_press_source==MISSIONS&&ui_press_row==i),dest=mission_destination(&game,offer),type=mission_type_for_offer(&game,offer),risk=mission_risk(&game,offer),active=mission_offer_active(&game,offer);
  unsigned riskink=risk>=4?RED:risk>=3?UI_GOLD:RGB(74,218,178),accent=i==row?UI_GOLD:UI_CYAN;
  rect(13,y,274,35,i==row?selected:card);rect(13,y,274,1,i==row?UI_GOLD:frame);rect(13,y+34,274,1,frame);if(i==row)rect(13,y,3,35,UI_GOLD);
  if(game.system==7&&i==0)mission_feature_shine(13,y,274,35);
  mission_card_icon(27,y+17,type,active?UI_GOLD:accent);
  char destline[32];snprintf(destline,sizeof(destline),"IN %.18s",game.systems[(game.system==7&&i==0)?7:dest].name);
  text_px(42,y+(active?2:5),i==row?UI_GOLD:WHITE,game.system==7&&i==0?"%.23s":"%.19s",game.system==7&&i==0?"INTRODUCTION TO THE ERL":mission_card_name(type));
  text_px(42,y+(active?13:20),active?RGB(74,218,178):UI_CYAN,"%.24s",destline);
  if(game.system==7&&i==0)text_px(207,y+20,game.erl.licence?RGB(74,218,178):riskink,"%s",game.erl.licence?"COMPLETE":"FEATURED");else text_px(207,y+(active?2:5),riskink,"%.10s",mission_difficulty_name(risk));
  if(active){unsigned live=RGB(74,218,178);circle(99,y+28,3,live);text_px(106,y+24,live,"IN PROGRESS");}
 }
 if(count){
  int type=mission_type_for_offer(&game,mission_board_offer(row)),dest=mission_destination(&game,mission_board_offer(row)),risk=mission_risk(&game,mission_board_offer(row)),active=mission_offer_active(&game,mission_board_offer(row));char plain[96],objective[96];mission_board_objective(plain,sizeof(plain),type,game.systems[dest].name);if(game.system==7&&row==0)snprintf(plain,sizeof(plain),"%s",game.erl.licence?"Introduction complete! Visit ERL headquarters to enter championships.":game.erl.referral?"Visit ERL headquarters at the referred Mega Capitol. Win the rookie qualifier.":"Meet mechanic Torr Bell. Win his test race for an ERL referral.");mission_upper(objective,sizeof(objective),plain);
  if(mission_accept_confirm==row&&!active){
   mission_confirm_card(type,dest,frame,rule);
  }else{
   text_px(308,35,UI_CYAN,"CONTRACT BRIEF");rect(308,47,156,1,rule);
   if(game.system==7&&row==0){text_px(308,53,UI_GOLD,"INTRODUCTION TO");text_px(308,62,UI_GOLD,"THE ERL");}else text_px(308,55,UI_GOLD,"%.19s",mission_card_name(type));text_px(308,70,WHITE,game.system==7&&row==0?"LAVE HUB":type==MISSION_RACE?"LOCAL SPRINT":"TO %.15s",game.systems[dest].name);
   if(mission_long_family(type))text_px(308,84,RED,"LONG CONTRACT");
   rect(304,91,164,13,RGB(38,52,66));text_px(308,93,UI_CYAN,"OBJECTIVE");
   text_wrap(38,13,19,6,WHITE,objective,0);
   rect(304,153,164,13,RGB(38,52,66));text_px(308,155,UI_CYAN,"REQUIREMENT");
   text_wrap(38,21,19,3,WHITE,game.system==7&&row==0?"NONE / FREE ENTRY":mission_requirement(type),0);
   text_px(308,194,DIM,game.system==7&&row==0?"ERL INTRODUCTION":type==MISSION_RACE?"BOOST GATES":"OPTIONAL BONUS");text_px(308,207,WHITE,"%.19s",game.system==7&&row==0?(game.erl.referral?"RECURRING CONTACT":"WIN FOR A REFERRAL"):type==MISSION_RACE?"GREEN RINGS = BOOST":mission_bonus_copy(type));
   rect(308,221,156,1,rule);text_px(308,227,DIM,"REWARD");if(game.system==7&&row==0&&game.erl.referral)text_px(400,227,UI_GOLD,"CONTACT");else text_px(400,227,UI_GOLD,"%.0f U",game.system==7&&row==0?500.f:mission_reward(&game,mission_board_offer(row))*.1f);
   if(game.job_n>=MISSION_SLOTS&&!active)text_px(155,231,RED,"MISSION LOG FULL");
  }
 }
 if(mission_accept_confirm==row)footer("LEFT/RIGHT CHOOSE   X CONFIRM   O CANCEL");
 else if(mission_offer_active(&game,mission_board_offer(row)))footer("UP/DOWN   X GO TO MISSION LOG   O BACK");
 else footer(game.system==7&&row==0?"UP/DOWN   X TALK TO TORR   O BACK":"UP/DOWN   X ACCEPT   O BACK");
 mission_queries_end();
}
static void mission_dossier_icon(int x,int y,int kind,unsigned ink){
 if(kind==0){line(x,y-7,x+6,y,ink);line(x+6,y,x,y+7,ink);line(x,y+7,x-6,y,ink);line(x-6,y,x,y-7,ink);rect(x-1,y-3,3,7,ink);}
 else if(kind==1){circle(x,y,6,ink);line(x-5,y+5,x+5,y-5,ink);line(x-3,y+7,x+7,y-3,ink);}
 else mission_card_icon(x,y,kind,ink);
}
static void mission_dossier_title(int x,int y,int cols,const char *s,unsigned ink){
 for(int lines=0;lines<2&&s&&*s;lines++){
  int len=(int)strlen(s),cut=len<cols?len:cols;if(len>cols)for(int k=cut;k>cols/2;k--)if(s[k]==' '){cut=k;break;}
  text_px(x,y+lines*10,ink,"%.*s",cut,s);s+=cut;while(*s==' ')s++;
 }
}
static const char *mission_dossier_name(int index){
 int track=mission_track_at(index);
 if(index==0)return game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT?saga_beats[game.saga_chapter].title:"KEI + RYN / OPEN CHANNEL";
 if(index==1)return guild_title(&game);
 if(track==TRACK_ERL)return "INTRODUCTION TO THE ERL";
 if(track==TRACK_LAVE)return "LAVE / BERTH SIX";
 if(track==TRACK_STATION_TOUR)return "STATION WELCOME / SECOND SHIFT";
 if(index>=2&&index<2+game.job_n&&trade_job_is(&game.jobs[index-2])){static char trade_name[72];char trader[40];trade_contact_name_at(game.jobs[index-2].dest,game.jobs[index-2].target,trader,sizeof(trader));snprintf(trade_name,sizeof(trade_name),"CARGO EXCHANGE / %s",trader);return trade_name;}
 return mission_card_name(game.jobs[index-2].type);
}
static int mission_dossier_destination(int index){
 int track=mission_track_at(index);
 if(index==0)return game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT?game.saga_dest:7;
 if(index==1){int type=guild_required_contract(&game);for(int i=0;i<game.job_n;i++)if(game.jobs[i].type==type)return game.jobs[i].dest;return type>=0?guild_contract_station(&game):game.system;}
 if(track==TRACK_ERL)return erl_destination();
 if(track==TRACK_LAVE)return 7;
 if(track==TRACK_STATION_TOUR)return STATION_TOUR_DEST;
 return index>=2&&index<2+game.job_n?game.jobs[index-2].dest:game.system;
}
static const char *mission_dossier_heading(int index){return mission_track_at(index)==tracked_mission?"TRACKED MISSION":"MISSION DETAILS";}
/* Pause at both ends and pan at 24 native pixels/sec. Only the focused title
 * animates; switching selection, reopening the log or changing its title resets. */
static int mission_title_offset(int index,const char *name,int width,int selected_row){
 if(!selected_row)return 0;
 unsigned hash=5381;for(const unsigned char *p=(const unsigned char *)name;*p;p++)hash=hash*33u+*p;
 if(mission_title_focus!=index||mission_title_hash!=hash){mission_title_focus=index;mission_title_hash=hash;mission_title_start=preview_time;}
 int excess=(int)strlen(name)*8-width;if(excess<=0)return 0;
 float travel=excess/24.f,elapsed=fmaxf(0,preview_time-mission_title_start),phase=fmodf(elapsed,2.f+travel*2.f);
 if(phase<=1.f)return 0;
 if(phase<1.f+travel)return (int)((phase-1.f)*24.f);
 if(phase<=2.f+travel)return excess;
 return (int)fmaxf(0,excess-(phase-2.f-travel)*24.f);
}
/* text_px draws whole glyphs without a local clip. Clip each native font pixel
 * here so partial ticker glyphs never touch the tracking marker or panel edge. */
static void mission_title_pixels(int x,int y,int width,const char *name,unsigned ink,int offset){
 ink=ui_paint_text(ink);
 int len=(int)strlen(name),first=offset/8,last=(offset+width+7)/8;if(last>len)last=len;
 for(int i=first;i<last;i++){
  unsigned ch=(unsigned char)name[i];if(ch<32||ch>126)ch='?';
  for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[ch-32][yy]&(1u<<(7-xx))){int px=x+i*8-offset+xx;if(px>=x&&px<x+width)pixel(px,y+yy,ink);}
 }
}
static void mission_dossier_list_row(int index,int x,int y,int w,int selected_row){
 int track=mission_track_at(index),tracked=track==tracked_mission,kind=index==0?0:index==1?1:index>=2&&index<2+game.job_n?game.jobs[index-2].type:MISSION_DELIVERY;
 unsigned accent=index==0?UI_GOLD:index==1?UI_CYAN:index>=2&&index<2+game.job_n?mission_base_type(kind)==MISSION_BOUNTY?RED:mission_base_type(kind)==MISSION_RESCUE?UI_CYAN:UI_SIGNAL:UI_GOLD;
 if(selected_row){rect(x,y,w,26,RGB(27,53,64));rect(x,y,w,1,UI_GOLD);rect(x,y+25,w,1,UI_GOLD);rect(x,y,3,26,UI_GOLD);}
 else rect(x,y+25,w,1,UI_EDGE);
 const char *name=mission_dossier_name(index);char short_name[128];int title_width=w-42,offset=mission_title_offset(index,name,title_width,selected_row);
 if(!selected_row&&(int)strlen(name)*8>title_width){snprintf(short_name,sizeof(short_name),"%.*s...",title_width/8-3,name);name=short_name;}
 mission_dossier_icon(x+14,y+13,index<2?index:kind,accent);mission_title_pixels(x+30,y+3,title_width,name,selected_row?WHITE:DIM,offset);
 int dest=mission_dossier_destination(index);text_px(x+30,y+15,accent,"IN %.13s",game.systems[dest].name);if(tracked)text_px(x+w-8,y+3,UI_GOLD,"*");
}
static void mission_dossier_route(int index,int dest){
 int track=mission_track_at(index),local=dest==game.system,jumps=0,hop=local?dest:route_next_hop(&game,dest,&jumps);
 int x0=254,x1=350,x2=438,y=207;unsigned mid=UI_CYAN;
 if(!local){
  text_px(238,188,DIM,"%d JUMP%s",jumps,jumps==1?"":"S");line(x0,y,x2,y,UI_EDGE);circle(x0,y,4,UI_SIGNAL);circle(x1,y,4,DIM);circle(x2,y,4,UI_GOLD);
  text_px(238,217,UI_SIGNAL,"%.8s",game.systems[game.system].name);if(hop>=0&&hop!=dest)text_px(318,217,mid,"%.9s",game.systems[hop].name);else text_px(334,217,DIM,jumps>1?"+%d":"NEXT",jumps>1?jumps-1:0);text_px(414,217,UI_GOLD,"%.6s",game.systems[dest].name);return;
 }
 const char *target="DOCK";int endpoint=0;
 if(index>=2&&index<2+game.job_n){Job *j=&game.jobs[index-2];int base=mission_base_type(j->type);if(trade_job_is(j)){target="HAIL";endpoint=2;}else if(base==MISSION_RACE){target="RACE";endpoint=4;}else if(base==MISSION_EXPLORATION){target="LAND";endpoint=1;}else if(base==MISSION_BOUNTY||base==MISSION_RESCUE){target="FIND";endpoint=2;}}
 else if(index==0&&game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT){int kind=saga_beats[game.saga_chapter].kind;if(kind==SAGA_SCAN){target="SCAN";endpoint=3;}else if(kind==SAGA_HUNT){target="FIND";endpoint=2;}}
 else if(index==1&&game.guild_chapter==2){target="SCAN";endpoint=3;}
 line(x0,200,424,200,UI_EDGE);circle(x0,200,4,UI_SIGNAL);
 if(endpoint==1){circle(344,200,11,UI_CYAN);line(337,195,351,202,UI_SIGNAL);line(338,206,348,193,UI_GOLD);}
 else if(endpoint==2){circle(344,200,7,UI_CYAN);line(334,200,354,200,UI_CYAN);line(344,190,344,210,UI_CYAN);}
 else if(endpoint==4){circle(344,200,10,UI_SIGNAL);mission_card_icon(344,200,MISSION_RACE,UI_GOLD);}
 else if(endpoint==3){line(336,192,352,208,UI_CYAN);line(352,192,336,208,UI_CYAN);circle(344,200,9,UI_GOLD);}
 else {rect(336,193,16,14,UI_CYAN);rect(341,197,6,6,RGB(4,10,16));}
 line(420,196,428,200,UI_GOLD);line(428,200,420,204,UI_GOLD);
 text_px(238,215,UI_SIGNAL,"%.8s",game.systems[game.system].name);
 if(endpoint==1&&index>=2&&index<2+game.job_n){int b=game.jobs[index-2].item;text_px(310,215,UI_CYAN,"%.10s",b>=1&&b<BODY_COUNT?game.bodies[b].name:"PLANET");}
 else text_px(312,215,UI_CYAN,endpoint==4?"START GRID":endpoint==2?"CONTACT":endpoint==3?"SIGNAL":"LOCAL HUB");text_px(408,215,UI_GOLD,"%s",target);
 (void)track;
}
static void mission_log(void){
 header("MISSION LOG / DOSSIER");
 int total=mission_log_count();if(row<0)row=0;if(row>=total)row=total-1;
 panel(8,31,211,211);panel(226,31,246,211);text_px(18,39,UI_GOLD,"MAIN MISSIONS");
 mission_dossier_list_row(0,17,52,192,row==0);mission_dossier_list_row(1,17,79,192,row==1);
 text_px(18,112,UI_CYAN,"SIDE MISSIONS");rect(18,124,191,1,UI_EDGE);
 int sides=total-2,visible=4,side_sel=row-2,first=side_sel>=visible?side_sel-visible+1:0;if(first>sides-visible)first=sides-visible;if(first<0)first=0;
 for(int j=0;j<visible&&first+j<sides;j++){int index=2+first+j;mission_dossier_list_row(index,17,128+j*27,192,row==index);}
 /* The rail is always visible; its thumb scales and moves with the side list. */
 rect(10,128,3,104,UI_EDGE);int thumb=sides>visible?104*visible/sides:104;if(thumb<14)thumb=14;int sy=sides>visible?128+(104-thumb)*first/(sides-visible):128;rect(9,sy,5,thumb,UI_CYAN);text_px(8,116,UI_CYAN,"^");text_px(8,233,UI_CYAN,"V");
 int track=mission_track_at(row),dest=mission_dossier_destination(row),kind=row==0?0:row==1?1:row>=2&&row<2+game.job_n?game.jobs[row-2].type:MISSION_DELIVERY;
 char erl_objective_text[192];const char *objective=track==TRACK_ERL?erl_objective(&game,erl_objective_text,sizeof(erl_objective_text)):track==TRACK_LAVE?sc_lave_objective():track==TRACK_STATION_TOUR?station_tour_objective():row==0?(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT?saga_beats[game.saga_chapter].objective:campaign_task(&game)):row==1?guild_objective(&game):mission_objective_at(&game,row-2);
 text_px(238,39,track==tracked_mission?UI_SIGNAL:UI_CYAN,"%s",mission_dossier_heading(row));mission_dossier_icon(251,68,row<2?row:kind,row==0?UI_GOLD:row==1?UI_CYAN:UI_SIGNAL);
 text_px(270,54,UI_GOLD,row<2?"MAIN MISSION":"SIDE CONTRACT");if(row>=2&&row<2+game.job_n){if(trade_job_is(&game.jobs[row-2]))text_px(404,54,UI_SIGNAL,"BARTER");else text_px(394,54,UI_SIGNAL,"%6.1f U",game.jobs[row-2].reward*.1f);}mission_dossier_title(270,66,23,mission_dossier_name(row),WHITE);
 rect(238,91,222,25,UI_RAISED);rect(238,91,222,1,UI_EDGE);text_px(246,94,DIM,"DESTINATION");int dest_len=(int)strlen(game.systems[dest].name);if(dest_len>14)dest_len=14;text_px(450-dest_len*8,105,UI_CYAN,"%.*s",dest_len,game.systems[dest].name);
 text_px(238,119,UI_GOLD,"CURRENT OBJECTIVE");rect(238,135,222,45,RGB(4,10,16));rect(238,135,222,1,UI_EDGE);rect(238,179,222,1,UI_EDGE);rect(238,135,1,45,UI_EDGE);rect(459,135,1,45,UI_EDGE);
 if(abandon_confirm){text_px(246,143,RED,"ABANDON THIS CONTRACT?");text_px(246,158,WHITE,"X CONFIRM / O CANCEL");}
 else text_wrap(31,18,26,3,WHITE,objective,0);
 mission_dossier_route(row,dest);
 footer(row>=2&&row<2+game.job_n?"X TRACK   SQUARE NEXT STEP   TRI ABANDON   O BACK":"X TRACK   SQUARE NEXT STEP   O BACK");
}
static void navigate_job(int index){if(index<0||index>=game.job_n)return;Job *j=&game.jobs[index];game.job_sel=index;const ExpeditionStage *xp=expedition_stage(&game,index);if(xp&&game.system==j->dest){if(xp->kind==XP_DOCK&&game.docked&&game.station_variant==0){expedition_dock(&game);return;}if(game.planet>=0&&game.planet==expedition_world(&game,index)){int site=expedition_site_id(&game,index);if(site>=0)surface_target_lock=site+1;change_page(FLIGHT);message(&game,expedition_objective(&game,index));return;}if(!game.docked)expedition_tick(&game,.001f);}route_set_goal(&game,j->dest);game.destination=j->dest;if(game.system!=j->dest){int jumps=0,hop=route_next_hop(&game,j->dest,&jumps);if(hop>=0){game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),"Route: %d jump%s. Refuel at intermediate hubs.",jumps,jumps==1?"":"s");message(&game,note);}else message(&game,"No fuel-safe route. Refuel at the hub first.");}else {route_clear(&game);if(game.docked){message(&game,"Launch, then navigate from Tracked Mission.");change_page(HOME);return;}int id=mission_target_id(&game,index);if(id>=0&&valid_target(id)){selected_target=id;scan_cat=target_category(id);autoaim=1;message(&game,"Mission target locked. Auto-align active.");if(!game.docked)change_page(FLIGHT);}else if(game.docked)message(&game,"Launch to continue this objective in the system.");else message(&game,"Objective unavailable. Re-enter this system.");}}
static int galnet_tab=0;
#include "social-feed.h"
#include "story-inbox.h"
#include "newspaper.h"
static int galnet_rows(void){return galnet_tab==0?(int)newspaper_local_identity()->page_count:galnet_tab==3?social_rows():galnet_tab==4?story_inbox_count():5;}
static int active_role(int role){int n=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==role)n++;return n;}
static const char *opening_net_author(int i);
static const char *opening_net_body(int i);
static int opening_net_count(void);
static void galnet_post(int i,char *author,int alen,char *body,int blen){
 if(galnet_tab==4){char subject[128];story_inbox_text(i,author,alen,subject,sizeof(subject),body,blen);return;}
 if(opening_sol(&game)&&galnet_tab!=3&&galnet_tab!=0&&i>=0&&i<4){snprintf(author,alen,"%s",opening_net_author(i));snprintf(body,blen,"%s",opening_net_body(i));return;}
 if(galnet_tab==3){unsigned code=social_code(i);if(code){int at=social_event_row(i);social_post_text(code,(game.social.origin[at]&256)!=0,author,alen,body,blen);}else{snprintf(author,alen,"Spacebook");snprintf(body,blen,"Your flights, discoveries and questionable decisions will appear here. Save your commander to keep them.");}return;}
 int risk=danger_rating(&game,game.system),wealth=prosperity(&game,game.system),item=(game.system*3+i*5)%GOODS;
 if(galnet_tab==0){int n=newspaper_local_identity()->page_count;if(i<0||i>=n)i=0;const NewspaperPage *p=&newspaper_pages[newspaper_local_offset()+i];const NewspaperBlock *v=&newspaper_blocks[p->first];snprintf(author,alen,"%s",newspaper_strings[v->title]);snprintf(body,blen,"%s",newspaper_strings[v->body]);}
 else if(galnet_tab==1){snprintf(author,alen,"MARKET TIP / %s",goods[item].name);if(i==0)snprintf(body,blen,"Local stock strength: %d/5. Dock for live prices.",wealth);else if(i==1)snprintf(body,blen,"Industrial hubs favour machinery and computers.");else if(i==2)snprintf(body,blen,"Agricultural worlds often export food cheaply.");else if(i==3)snprintf(body,blen,"Restricted goods matter when Law scans your hold.");else snprintf(body,blen,"Cargo space: %d/%d tonnes used.",cargo_used(&game),cargo_capacity(&game));}
 else if(galnet_tab==2){const char *a[]={"BOUNTY DESK","LOCAL LAW","PILOT WARNING","PATROL WATCH","SECURITY FEED"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"Pirate contacts active: %d. Standard bounty 15.0.",active_role(PIRATES));else if(i==1)snprintf(body,blen,"Your local wanted record: [%s].",stars(wanted_level(&game)));else if(i==2)snprintf(body,blen,risk>=4?"Travel in groups. Hostile activity is elevated.":"No major raid warning at this time.");else if(i==3)snprintf(body,blen,"Law patrols active: %d.",active_role(LAW));else snprintf(body,blen,"Wanted records remain inside the offending system.");}
 else if(galnet_tab==3){const char *a[]={"Mira / Trader","Marshal Iona Renn","Dockhand_77","Kei / Explorer","Freighter Crew","DefinitelyNotAPirate","Local Spotters"};snprintf(author,alen,"%s",a[i]);if(i==0){const char *mira=saga_galnet_mira(&game);if(mira)snprintf(body,blen,"%s",mira);else snprintf(body,blen,"%s market looks %s today.",game.systems[game.system].name,wealth>=4?"well stocked":"a little thin");}else if(i==1){const char *iona=saga_galnet_iona(&game);if(iona)snprintf(body,blen,"%s",iona);else snprintf(body,blen,risk>=4?"Pirate sightings up. Keep scanners active.":"Patrol lanes are calm. Fly safely, commanders.");}else if(i==2)snprintf(body,blen,"Stop boosting near %s, you maniacs.",station_name(&game));else if(i==3){const char *kei=saga_galnet_kei(&game);if(kei)snprintf(body,blen,"%s",kei);else snprintf(body,blen,game.story<STORY_FREE?"Ryn's last ping is still on this wire.":"The colour of %s is unreal from orbit.",game.bodies[1].name);}else if(i==4){const char *fr=saga_galnet_freighter(&game);if(fr)snprintf(body,blen,"%s",fr);else snprintf(body,blen,"Slow convoy crossing the system. Give us room.");}else if(i==5){const char *sable=saga_galnet_sable(&game);if(sable)snprintf(body,blen,"%s",sable);else snprintf(body,blen,"Free cargo inspection behind the gas giant. Honest.");}else if(game.mission_result&&game.last_mission_system==game.system)snprintf(body,blen,"Commander mission report: %s %s.",mission_name(game.last_mission_type),game.mission_result>0?"complete":"closed");else snprintf(body,blen,"%s",travellers_galnet_spotter(&game));}
 else if(galnet_tab==5){snprintf(author,alen,"MISSION NETWORK");if(i==0&&game.job_n>0)snprintf(body,blen,"Log %d/5. Focus %s to %s.",game.job_n,mission_name(game.jobs[game.job_sel].type),game.systems[game.jobs[game.job_sel].dest].name);else if(i==0&&game.mission_result)snprintf(body,blen,"Last job: %s %s at %s.",mission_name(game.last_mission_type),game.mission_result>0?"complete":"closed",game.systems[game.last_mission_system].name);else if(i==0)snprintf(body,blen,"No active missions. %d local offers available.",mission_count(&game));else if(i==1){const char *net=saga_galnet_network(&game);if(net)snprintf(body,blen,"%s",net);else snprintf(body,blen,"Five jobs. Track one in the Log, then follow its next step.");}else if(i==2)snprintf(body,blen,"Delivery, hunt, scan, rescue and covert jobs online.");else if(i==3)snprintf(body,blen,"Mission contacts carry white [M] scanner markers.");else snprintf(body,blen,game.job_n>=MISSION_SLOTS?"Log full. Complete a job before taking more work.":"Dock to use the board and accept a contract.");}
 else {const char *a[]={"SHIP COMPUTER","GALACTICNET","NAV COMPUTER","STATION LINK","SYSTEM NOTICE"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"Welcome, Commander. Network link is online.");else if(i==1)snprintf(body,blen,"%d unread local posts.",2+wealth);else if(i==2&&game.contract>=0)snprintf(body,blen,"Mission route set for %s.",game.systems[game.contract].name);else if(i==2)snprintf(body,blen,"No mission route currently assigned.");else if(i==3)snprintf(body,blen,"Docking channel: %s.",game.docked?"connected":"standby");else snprintf(body,blen,"System %s / risk [%s].",game.systems[game.system].name,stars(risk));}
}
static void galnet_avatar(int x,int y,int size,int i){
 int role=TRADERS,seed=100+i;
 if(galnet_tab==3){static const int roles[]={TRADERS,LAW,TRADERS,EXPLORERS,TRADERS,PIRATES,EXPLORERS};role=roles[i%7];if(i==3){draw_kei(x,y,size,0);return;}if(i==1)seed=VOICE_LAW*37;if(i==2)seed=VOICE_DOCK*37;}
 else if(galnet_tab==2)role=LAW;else if(galnet_tab==0)role=i==1?LAW:i==4?EXPLORERS:TRADERS;
 else if(galnet_tab==5){role=EXPLORERS;if(i==0){portrait_draw(x,y,size,size,portrait_ship_computer(),EXPLORERS);return;}}
 draw_portrait(x,y,size,size,seed,role);
}
#include "spacebook.h"
/* A small neon shop sign, authored as 5x7 tubes at its displayed 2px scale.
 * Brief deterministic dropouts/scanline slips do not consume gameplay RNG. */
static void galnet_live_terminal(void){
 static const char letters[]="LIVETRMNA";
 static const unsigned char glyphs[][7]={
  {16,16,16,16,16,16,31},{14,4,4,4,4,4,14},{17,17,17,17,17,10,4},
  {31,16,16,30,16,16,31},{31,4,4,4,4,4,4},{30,17,17,30,20,18,17},
  {17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,31,17,17,17}
 };
 const char *label="LIVE TERMINAL";int phase=(int)(preview_time*30.f)%173,glitch=phase==7||phase==8||phase==91;
 unsigned glow=RGB(65,28,67),tube=mix_rgb(RGB(226,85,166),RGB(255,143,213),.5f+.5f*sinf(preview_time*1.8f));
 rect(12,222,176,23,RGB(6,12,21));rect(12,222,176,1,RGB(37,50,62));rect(12,244,176,1,RGB(37,50,62));
 for(int pass=0;pass<2;pass++)for(int i=0;label[i];i++){
  const char *found=strchr(letters,label[i]);if(!found)continue;int index=(int)(found-letters);
  int dim=glitch&&i==((int)(preview_time/5.f)*3+3)%13;
  for(int gy=0;gy<7;gy++)for(int gx=0;gx<5;gx++)if(glyphs[index][gy]&(16u>>gx)){
   int x=22+i*12+gx*2+(phase==8&&gy==3?1:0),y=226+gy*2;
   if(!pass)rect(x-1,y-1,4,4,glow);
   else {rect(x,y,2,2,dim?RGB(112,46,91):tube);if(!dim)pixel(x,y,RGB(255,218,239));}
  }
 }
 if(phase==8)line(24,233,32,233,UI_CYAN);
}
static void galnet_market(void){dockbay_site_draw();}
#include "wanted-crime-watch.h"
static void galnet_chrome(void){
 header("GALACTICNET // LIVE NETWORK");rect(0,22,W,20,ui_paint_surface(RGB(11,25,35)));
 /* Six equal tabs between L/R pads — short labels so SPACEBOOK/MESSAGES never collide. */
 rect(0,22,28,20,ui_paint_surface(RGB(8,18,28)));rect(452,22,28,20,ui_paint_surface(RGB(8,18,28)));
 rect(2,24,24,16,ui_paint_surface(RGB(25,65,77)));rect(454,24,24,16,ui_paint_surface(RGB(25,65,77)));
 text(1,3,UI_CYAN,"<L");text(57,3,UI_CYAN,"R>");
 if(galnet_tab==5){rect(0,22,28,20,ui_paint_surface(RGB(8,18,28)));rect(452,22,28,20,ui_paint_surface(RGB(8,18,28)));button_icon(8,27,'l',UI_TEXT);button_icon(460,27,'r',UI_TEXT);}
 const char *shorts[]={"NEWS","MARKET","WANTED","BOOK","INBOX","RUMOURS"};
 int sol=opening_sol(&game),slots=sol?5:6,k=0;
 for(int i=0;i<6;i++){
  if(sol&&i==2)continue;int x=30+k++*(420/slots),tw=420/slots-2;
  if(i==galnet_tab){rect(x,22,tw,20,ui_paint_surface(RGB(25,65,77)));rect(x,40,tw,2,UI_GOLD);}
  int len=(int)strlen(shorts[i]);int col=(x+(tw-len*8)/2)/8;
  text(col,3,i==galnet_tab?WHITE:DIM,"%s",shorts[i]);
 }
}
#include "rumours-ui.h"
#include "opening-content.h"
static void galnet_screen(void){if(galnet_tab==5){rumours_screen();galnet_chrome();return;}if(opening_net_screen())return;if(galnet_tab==3||galnet_tab==4)spacebook_screen(galnet_tab==4);else if(galnet_tab==0)news_screen();else if(galnet_tab==1)galnet_market();else if(galnet_tab==2)galnet_bounties();else {int count=galnet_rows(),pages=(count+2)/3,first=row/3*3;rect(0,42,W,206,BG);text(2,6,UI_CYAN,"MISSION FEED");page_number_at(20,6,row/3+1,pages);for(int j=0;j<3&&first+j<count;j++){int i=first+j,y=8+j*5;char author[40],body[96];galnet_post(i,author,sizeof(author),body,sizeof(body));panel(14,y*8-3,458,34);if(i==row)rect(14,y*8-3,3,34,UI_GOLD);galnet_avatar(20,y*8-1,28,i);text(7,y,i==row?UI_GOLD:UI_CYAN,"%s",author);text(7,y+2,WHITE,"%.50s",body);}galnet_scrollbar(8,61,114,first,3,count,UI_RAISED,UI_GOLD);footer("L/R SECTION   UP/DOWN   O BACK");}galnet_chrome();}
static void story_screen(void){
 header("FLIGHT GUIDE / OPTIONAL");panel(8,32,464,190);
 draw_kei(16,40,48,0);text(10,5,UI_CYAN,"KEI / FLIGHT COACH");
 text(10,7,DIM,"Help with controls; separate from the story.");
 text(3,12,WHITE,"Optional help with your ship's controls.");
 text(3,14,WHITE,"Show the next lesson to highlight its menu entry.");
 text(3,16,DIM,"You can end this guide and use Controls at any time.");
 text(3,20,UI_GOLD,"%s",game.story<STORY_FREE?story_task(&game):"Guide finished. Fly whenever you are ready.");
 if(row==0)selected_span(23,464);
 text(3,23,row==0?WHITE:DIM,"%s %s",row==0?">":" ",game.story<STORY_FREE?"Show the next lesson on the menu":"Return to the main menu");
 if(game.story<STORY_FREE){if(row==1)selected_span(26,464);text(3,26,row==1?WHITE:DIM,"%s End the optional guide",row==1?">":" ");}
 footer("UP/DOWN CHOOSE   X SELECT   O BACK");
}


