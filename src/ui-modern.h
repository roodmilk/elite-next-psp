static int cargo_item(int selection){int n=0;if(game.docked){for(int i=0;i<GOODS;i++)if(station_market_item_visible(&game,i)){if(n++==selection)return i;}return -1;}for(int i=0;i<GOODS;i++)if(game.cargo[i]>0){if(n++==selection)return i;}return -1;}
static int cargo_rows(void){int n=0;if(game.docked){for(int i=0;i<GOODS;i++)n+=station_market_item_visible(&game,i);return n?n:1;}for(int i=0;i<GOODS;i++)n+=game.cargo[i]>0;return n?n:1;}
static int contact_ids[TARGET_CAPACITY],contact_count;
static void contacts_refresh(void){contact_count=0;for(int i=0;i<=FLIGHT_TARGET_MAX;i++)if(valid_target(i))contact_ids[contact_count++]=i;}
/* Instrument panel — soft charcoal fill, cream/slate rules. No gold corner brackets (debug look). */
static void panel(int x,int y,int w,int h){rect(x,y,w,h,UI_PANEL);rect(x,y,w,1,UI_TEXT);rect(x,y+h-1,w,1,UI_RAISED);rect(x,y,1,h,RGB(90,96,76));rect(x+w-1,y,1,h,UI_RAISED);rect(x+1,y+1,w-2,1,UI_RAISED);}
#include "deck-ui.h"
static void market_screen(void){
 char market_title[48];snprintf(market_title,sizeof(market_title),game.docked?"CARGO / %s MARKET":"CARGO / INVENTORY",station_class_name(station_class(&game)));
 header(market_title);panel(8,32,220,156);panel(236,32,236,156);
 int count=cargo_rows(),first=(row/7)*7,item=cargo_item(row);
 text(3,5,UI_CYAN,game.docked?"COMMODITIES":"YOUR CARGO");page_number_at(21,5,row/7+1,(count+6)/7);text(25,5,UI_CYAN,"HOLD");
 for(int j=0;j<7&&first+j<count;j++){int index=cargo_item(first+j);int y=8+j*2;if(first+j==row)rect(10,y*8-2,216,13,RGB(25,65,77));if(index>=0)text(3,y,WHITE,"%-17s %d%c",goods[index].name,game.cargo[index],goods[index].unit);}
 if(item<0){text(31,8,DIM,"Hold is empty.");text(31,10,WHITE,"Dock to buy cargo.");}
 else {
  int avg=galactic_price(item),local=game.price[item],diff=local-avg;draw_next_art(next_items[item],32,32,430,65,32,32);
  text(31,5,UI_GOLD,"%.22s",goods[item].name);
  text(31,7,WHITE,"%d %c in hold",game.cargo[item],goods[item].unit);if(mission_cargo_reserved(&game,item))text(31,8,UI_GOLD,"%d reserved for jobs",mission_cargo_reserved(&game,item));
  if(game.docked){
   text(31,9,WHITE,"Here %.1f",local*.1f);
   text(31,10,DIM,"Gal  %.1f",avg*.1f);
   text(31,12,diff>0?RED:diff<0?UI_CYAN:WHITE,diff>0?"^ ABOVE AVG":diff<0?"v BELOW AVG":"= GALACTIC AVG");
   text(31,15,DIM,"Stock %d",game.stock[item]);
   if(item==3||item==6||item==10)text(31,17,RED,"RESTRICTED GOODS!");
  }else {int wx=252,wy=116;line(wx,wy+12,wx+7,wy,AMBER);line(wx+7,wy,wx+14,wy+12,AMBER);line(wx+14,wy+12,wx,wy+12,AMBER);text(32,15,AMBER,"! DOCK TO BUY OR SELL");}
 }
 int used=cargo_used(&game),cap=cargo_capacity(&game);if(cap<1)cap=1;if(used>cap)used=cap;rect(14,176,210,5,RGB(30,45,60));rect(14,176,210*used/cap,5,UI_CYAN);text(31,22,WHITE,"%.1f U  %d/%d T",game.credits*.1f,cargo_used(&game),cargo_capacity(&game));footer(game.docked?"UP/DOWN   RIGHT BUY   LEFT SELL   O BACK":"UP/DOWN   O BACK");
}
static int target_filter=0,target_ids[TARGET_CAPACITY],target_count=0,target_details=0;
static int target_matches(int id){if(target_filter==0)return 1;if(target_filter==1)return IS_NPC_ID(id);if(target_filter==2)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].target==-2;if(target_filter==3)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==LAW;if(target_filter==4)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==TRADERS;if(target_filter==5)return is_mission_target(&game,id);if(target_filter==6)return id>0&&id<=BODY_COUNT;if(target_filter==7)return IS_STATION_ID(id);if(target_filter==8)return IS_DEBRIS_ID(id);if(target_filter==9)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].freighter;return IS_ANOMALY_ID(id);}
static void target_refresh(void){target_count=0;for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(valid_target(id)&&target_matches(id))target_ids[target_count++]=id;if(row>=target_count)row=0;}
static unsigned contact_color(int id){if(id==ROUTE_TARGET_ID)return CYAN;if(is_mission_target(&game,id))return WHITE;if(IS_NPC_ID(id))return faction_colors[game.npc[id-BODY_COUNT-1].role];if(IS_ANOMALY_ID(id))return UI_GOLD;if(IS_DEBRIS_ID(id))return DIM;return IS_STATION_ID(id)?UI_CYAN:UI_GOLD;}
static const char *target_status(int id){if(id==ROUTE_TARGET_ID)return "PLOTTED HYPERSPACE VECTOR";if(is_mission_target(&game,id))return "MISSION TARGET";if(IS_ANOMALY_ID(id))return game.anomaly[id-ANOMALY_ID_MIN].scanned?"CODEX ENTRY":"UNSCANNED ANOMALY";if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].rock?"LASER-MINEABLE ROCK":game.debris[id-DEBRIS_ID_MIN].wreck?"SALVAGE WRECK":"COLLECTABLE CARGO";if(IS_STATION_ID(id))return station_target_hub(id)==1?"OUTER RELAY / DOCKABLE":station_target_hub(id)==2?"OUTPOST / DOCKABLE":station_class_name(station_class(&game));if(id<=BODY_COUNT){Body *b=&game.bodies[id-1];return b->type==SUN?"STAR / NO LANDING":b->type==GAS?"FLOATING SKYPORT":"APPROACHABLE";}NPC *n=&game.npc[id-BODY_COUNT-1];if(n->target==-2)return "HOSTILE";if(n->role==PIRATES)return "WANTED";if(n->role==LAW)return "LAW";if(n->role==TRADERS)return n->freighter?freight_status(n):"TRADER";return "NEUTRAL";}
/* Faint CRT glass under targeting list — scanlines + sparse fuzz. Drawn before glyphs so text stays sharp. */
static void targeting_crt_glass(int x,int y,int w,int h){
 unsigned base=UI_PANEL,scan=RGB(17,23,31),fleck_lo=RGB(14,19,26),fleck_hi=RGB(28,36,48);
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
  text(37,5,contact_color(id),"%.18s",scanner_known(id)?target_name(id):"UNKNOWN CONTACT");
  text(37,7,DIM,"%s",target_status(id));
  targeting_scope_stamp(414,42,id);
  {char range[12];target_range_label(length(sub(target_position(id),game.pos)),range,sizeof(range));text(37,11,WHITE,"RANGE %s",range);}
  if(IS_NPC_ID(id)&&scanner_known(id)){
   NPC *n=&game.npc[id-BODY_COUNT-1];
   text(37,13,DIM,"FACTION %s",n->role==LAW?"LAW":n->role==PIRATES?"PIRATES":n->role==TRADERS?"TRADERS":n->role==EXPLORERS?"EXPLORERS":"NEUTRAL");
   target_condition(296,128,id);
   text_px(296,160,UI_CYAN,"%.21s",traffic_activity(n));
  } else text(37,13,DIM,"CLASS %s",id==ROUTE_TARGET_ID?"NAV STAR":IS_STATION_ID(id)?"STATION":id<=BODY_COUNT?"CELESTIAL":IS_DEBRIS_ID(id)?"DEBRIS":"ANOMALY");
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
static void debug_screen(void){header((game.debug_flags&DEBUG_MODIFIED)?"DEBUG TOOLS / MODIFIED":"DEBUG TOOLS");const char *items[]={"Add 1,000 units","Refill fuel and shields","Clear local wanted","Add local wanted","Move to station approach","Move near targeted planet","Return to station","Enter planet atmosphere","Install pulse laser","Reveal local Codex","Cool ship and clear heat","Install Heat Buffer","Force Thargoid encounters","Unlimited fuel","Unlimited jump range"};for(int i=0;i<15;i++){const char *state=i==12?(debug_force_thargoid?": ON":": OFF"):i==13?((game.debug_flags&DEBUG_UNLIMITED_FUEL)?": ON":": OFF"):i==14?((game.debug_flags&DEBUG_UNLIMITED_RANGE)?": ON":": OFF"):"";if(row==i)selected(3+i);text(3,3+i,row==i?WHITE:DIM,"%s%s",items[i],state);}footer("UP/DOWN   X APPLY   O BACK");}
static void debug_action(void){
 game.debug_flags|=DEBUG_MODIFIED;
 if(row==0){game.credits+=10000;if(game.credits>100000000)game.credits=100000000;game.cue=SFX_UI;message(&game,"Added 1,000 units.");}
 if(row==1){game.fuel=player_ships[game.ship].range;game.energy=100;message(&game,"Fuel and shields full.");}
 if(row==2){game.legal=game.wanted[game.system]=0;game.crime_record[game.system]=0;game.police_cargo_heat=0;message(&game,"Local warrant cleared.");}
 if(row==3){add_crime(&game,5);message(&game,"Local wanted level raised.");}
 if(row==4){game.docked=0;game.pos=(Vec3){0,0,3000};game.speed=0;game.yaw=game.pitch=game.roll=0;selected_target=0;autoaim=0;change_page(FLIGHT);}
 if(row==5){if(selected_target<2||selected_target>BODY_COUNT){message(&game,"Select a planet in Contacts first.");return;}Body *b=&game.bodies[selected_target-1];game.docked=0;game.pos=add(b->pos,(Vec3){0,0,-b->radius-800});game.speed=0;game.yaw=game.pitch=game.roll=0;autoaim=0;change_page(FLIGHT);}
 if(row==6){game.planet=game.approach=-1;game.surface=0;game.docked=1;game.speed=0;game.boost=0;game.dock_stage=game.dock_phase=0;game.dock_timer=game.dock_duration=0;game.yaw=game.pitch=game.roll=0;autoaim=0;hard_brake=0;game.pos=(Vec3){0,0,3200};change_page(HOME);message(&game,"Docked at the local station.");}
 if(row==7){if(selected_target<2||selected_target>BODY_COUNT){message(&game,"Select a planet in Contacts first.");return;}Body *b=&game.bodies[selected_target-1];if(b->type==SUN){message(&game,"Suns have no atmosphere flight.");return;}game.docked=0;game.pos=add(b->pos,(Vec3){0,0,-b->radius-800});game.yaw=game.pitch=game.roll=0;game.approach=selected_target-1;if(enter_planet(&game)){autoaim=0;change_page(FLIGHT);}else message(&game,"Could not enter atmosphere.");}
 if(row==8){game.laser=1;game.upgrades|=4;message(&game,"Pulse laser installed in WPN slot.");}
 if(row==9){game.discoveries=256;message(&game,"Local Codex records revealed.");}
 if(row==10){game.heat=0;game.energy=100;message(&game,"Heat dumped and shields restored.");}
 if(row==11){game.fit[FIT_UTIL]=24;fit_rebuild(&game);message(&game,"Heat Buffer installed in UTIL slot.");}
 if(row==12){debug_force_thargoid=!debug_force_thargoid;game.cue=SFX_UI;message(&game,debug_force_thargoid?"Forced Thargoid encounters enabled. Every jump will be intercepted.":"Forced Thargoid encounters disabled. Normal encounter chance restored.");}
 if(row==13){game.debug_flags^=DEBUG_UNLIMITED_FUEL;if(game.debug_flags&DEBUG_UNLIMITED_FUEL)game.fuel=(float)player_ships[game.ship].range;game.cue=SFX_UI;message(&game,(game.debug_flags&DEBUG_UNLIMITED_FUEL)?"Unlimited fuel enabled. Boost and jumps use no fuel.":"Unlimited fuel disabled. Normal fuel use restored.");}
 if(row==14){game.debug_flags^=DEBUG_UNLIMITED_RANGE;game.cue=SFX_UI;message(&game,(game.debug_flags&DEBUG_UNLIMITED_RANGE)?"Unlimited jump range enabled. Any system can be one jump away.":"Unlimited jump range disabled. Fitted drive range restored.");}
}
static void body_tint(int sys,int i,unsigned *col,unsigned *acc,int *type){
 const int world_perm[6][4]={{OCEAN,ROCKY,GAS,ROCKY},{ROCKY,GAS,OCEAN,ROCKY},{GAS,OCEAN,ROCKY,ROCKY},{ROCKY,OCEAN,ROCKY,GAS},{OCEAN,GAS,ROCKY,ROCKY},{ROCKY,ROCKY,OCEAN,GAS}};
 /* Keep in lockstep with sectors.h sun/world palettes so every view matches flight. */
 const unsigned suns[]={0x80dfff,0x66c8ff,0x526eff,0xf4f4ff,0xffc88a,0xc88cff,0x9ee8ff,0xd8e8ff,0xffa060,0xb0ffe0};
 const unsigned worlds[]={0xc97535,0x91b45c,0x8763b5,0x7ebfc4,0xb87775,0xadc2ce,0xd4a574,0x5a8f6a,0x6b5b95,0xc45c5c,0x3d7a5a,0xd0a040,0x5a90c0,0xa05070,0x708050};
 unsigned h=body_art_seed(sys,i);int perm=(int)((sun_hash((unsigned)(sys+1)*0x9e3779b9u)>>6)%6);
 *type=i==0?SUN:world_perm[perm][(i-1)&3];*col=i==0?suns[h%10]:worlds[(h>>8)%15];*acc=worlds[(h>>16)%15];
 if(sys==7&&i==1){*type=OCEAN;*col=0xc35f23;*acc=0x4b9137;}
}
static const char *codex_body_name(int sys,int body){
 static char name[32];
 if(sys==game.system&&body>=0&&body<BODY_COUNT)return game.bodies[body].name;
 snprintf(name,sizeof(name),"%s %s",game.systems[sys].name,body==0?"SUN":body==1?"1":body==2?"2":body==3?"3":"4");
 return name;
}
static void chart_system_preview(int dest){
 int xs[5]={352,394,436,373,415},ys[5]={58,52,58,92,96},rs[5]={16,12,10,14,11};
 for(int i=0;i<BODY_COUNT;i++){unsigned col,acc;int type;body_tint(dest,i,&col,&acc,&type);draw_planet_disc(xs[i],ys[i],rs[i],col,acc,body_art_seed(dest,i),type);}
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
 chart_glass(30,35,420,190,UI_CYAN);chart_text(46,44,UI_CYAN,"SYSTEM SEARCH");
 rect(46,59,184,21,RGB(8,20,31));rect(46,59,184,1,UI_EDGE);chart_text(54,65,WHITE,"%s",chart_query[0]?chart_query:"TYPE A PREFIX...");
 chart_text(46,85,DIM,"ON-SCREEN INPUT");
 for(int i=0;i<28;i++){
  int x=46+(i%7)*25,y=100+(i/7)*23,sel=i==chart_search_key;
  if(sel){rect(x-2,y-3,23,19,RGB(30,72,86));rect(x-2,y-3,2,19,UI_CYAN);}
  if(i<26)chart_text(x+5,y,sel?WHITE:UI_TEXT,"%c",keys[i]);
  else chart_text(x+(i==26?0:3),y,sel?WHITE:UI_TEXT,i==26?"DEL":"GO");
 }
 rect(238,51,1,156,RGB(30,94,120));chart_text(254,44,UI_GOLD,"PREFIX MATCHES");
 int first=chart_search_match-2;if(first<0)first=0;if(first+6>chart_match_n)first=chart_match_n-6;if(first<0)first=0;
 if(!chart_match_n)chart_text(254,69,RED,"NO MATCHING SYSTEMS");
 for(int j=0;j<6&&first+j<chart_match_n;j++){
  int index=first+j,id=chart_matches[index],y=66+j*23;
  if(index==chart_search_match){rect(250,y-4,184,19,RGB(28,60,76));rect(250,y-4,2,19,UI_CYAN);}
  chart_text(258,y,index==chart_search_match?WHITE:UI_TEXT,"%.11s",game.systems[id].name);
  chart_text(356,y,index==chart_search_match?UI_GOLD:DIM,"%4.1f LY",distance_ly(&game,game.system,id));
 }
 chart_text(254,205,DIM,"L/R CHANGE MATCH");
 chart_glass(2,248,476,22,RGB(56,125,156));
 rect(13,254,3,11,UI_TEXT);rect(9,258,11,3,UI_TEXT);chart_text(26,256,UI_TEXT,"TYPE");
 button_icon(78,254,'X',RGB(97,190,255));chart_text(94,256,UI_TEXT,"KEY");
 button_icon(140,254,'S',RGB(255,114,213));chart_text(156,256,UI_TEXT,"DEL");
 button_icon(196,254,'l',UI_TEXT);button_icon(213,254,'r',UI_TEXT);chart_text(230,256,UI_TEXT,"MATCH");
 button_icon(284,254,'T',RGB(105,230,174));chart_text(300,256,UI_TEXT,"SELECT");
 button_icon(386,254,'O',RGB(255,125,151));chart_text(402,256,UI_TEXT,"CLOSE");
}
static void galaxy_overview(void){
 const unsigned cyan=RGB(42,228,243),pink=RGB(255,114,213),gold=RGB(255,221,113);
 static const char *filters[]={"ALL","VISITED","UNVISITED","RICH","POOR","MEGA","JOBS","IN RANGE","ROUTE"};
 preview_reset();rect(0,0,W,H,RGB(2,5,11));chart_galaxy_background();
 clipx0=2;clipx1=478;clipy0=25;clipy1=246;
 int gcx=250+(int)chart_pan_x,gcy=144+(int)chart_pan_y;
 /* Faint continuous survey ellipses read as structure, never isolated dots. */
 chart_ellipse(gcx,gcy,(int)(199*chart_zoom),(int)(74*chart_zoom),RGB(12,58,77));
 chart_ellipse(gcx,gcy,(int)(135*chart_zoom),(int)(50*chart_zoom),RGB(11,42,60));
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
 if(chart_filter!=CHART_ROUTE&&game.route_goal>=0&&game.route_goal!=chart_cursor)galaxy_route_draw(game.route_goal,RGB(40,105,135));
 galaxy_route_draw(chart_filter==CHART_ROUTE?game.route_goal:chart_cursor,cyan);
 int jumps=chart_hops[chart_cursor];
 int x,y;galaxy_xy(game.system,&x,&y);chart_glow(x,y,gold,0);
 if(chart_cursor!=game.system){galaxy_xy(chart_cursor,&x,&y);chart_glow(x,y,pink,1);
  line(x-7,y-7,x-4,y-7,pink);line(x-7,y-7,x-7,y-4,pink);
  line(x+7,y+7,x+4,y+7,pink);line(x+7,y+7,x+7,y+4,pink);}
 chart_label_n=0;chart_label(chart_cursor,chart_cursor==game.system?gold:pink);
 if(chart_cursor!=game.system)chart_label(game.system,gold);
 /* One intermediate name gives the route context without filling the view. */
 if(chart_path_n>2)chart_label(chart_path[chart_path_n/2],RGB(202,235,247));
 preview_reset();
 chart_glass(2,2,476,22,RGB(56,125,156));
 chart_type(11,5,WHITE,2,"GALAXY");chart_type(92,5,cyan,2,"/ DEEP CHART");
 chart_text(407,9,RGB(58,110,137),"ARCELITE");
 chart_glass(8,31,90,86,RGB(39,126,158));
 int first_filter=chart_filter_first();
 for(int row=0;row<5;row++){
  int i=first_filter+row,y=36+row*16;
  if(row)line(11,y-4,89,y-4,RGB(23,57,76));
  if(i==chart_filter){
   rect(11,y-3,79,15,RGB(4,41,54));line(11,y-3,89,y-3,cyan);line(11,y+11,89,y+11,cyan);
   rect(11,y-3,2,15,cyan);rect(88,y-3,2,15,cyan);
  }
  chart_text(16,y,i==chart_filter?cyan:RGB(184,207,222),"%s",filters[i]);
 }
 rect(93,35,2,77,RGB(20,58,76));rect(93,35+first_filter*35/(CHART_FILTERS-5),2,42,cyan);
 chart_glass(345,31,127,90,cyan);
 chart_text(353,41,chart_cursor==game.system?gold:pink,"%s",game.systems[chart_cursor].name);
 line(347,54,469,54,RGB(30,94,120));
 int kind=station_class_for_system(&game,chart_cursor);
 chart_text(354,62,RGB(206,225,238),"%s",kind==STATION_MEGA?"MEGA":kind==STATION_POOR?"POOR":"RICH");
 chart_text(354,78,WHITE,"%.1f LY",distance_ly(&game,game.system,chart_cursor));
 if(chart_cursor==game.system)chart_text(354,101,gold,"CURRENT SYSTEM");
 else if(jumps>=0)chart_text(354,101,RGB(206,225,238),"%d JUMP%s",jumps,jumps==1?"":"S");
 else chart_text(354,101,RGB(239,150,134),"OUT OF RANGE");
 chart_glow(452,79,chart_cursor==game.system?gold:pink,1);
 /* Quiet bottom corners provide useful context without a banner. */
 rect(8,231,122,12,RGB(3,12,22));
 if(!matching)chart_text(12,233,RGB(110,160,184),"%s",chart_filter==CHART_ROUTE?"NO ROUTE":chart_filter==CHART_JOBS?"NO JOBS":"NO MATCHES");
 else chart_text(12,233,RGB(110,160,184),"%d IN VIEW",shown);
 chart_text(389,233,RGB(110,160,184),"ZOOM %.1fX",chart_zoom);
 chart_glass(2,248,476,22,RGB(56,125,156));
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
static int codex_system_body_count(int sys){int n=2;for(int b=1;b<BODY_COUNT;b++)if(game.landed_planets[sys]&(1u<<(b-1)))n++;return n;}
static int codex_system_body_at(int sys,int r){if(r==0)return 0;if(r==1)return 1;int seen=0;for(int b=1;b<BODY_COUNT;b++)if(game.landed_planets[sys]&(1u<<(b-1)))if(seen++==r-2)return b+1;return 1;}
static int codex_system_row_for_body(int sys,int body){if(body<2)return body;int r=2;for(int b=1;b<BODY_COUNT;b++)if(game.landed_planets[sys]&(1u<<(b-1))){if(b+1==body)return r;r++;}return 1;}
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
 "The Ophidian is for the trader who keeps one eye on the next horizon. Forty-eight tonnes and a 9.2-light-year reach offer serious carrying power without dropping to heavy-hauler speeds."
};
static void yard(void){
 int mega=station_class(&game)==STATION_MEGA;header(mega?"MEGA SHIPYARD / CAPITAL EXCHANGE":"SHIPYARD / EXCHANGE");if(!game.docked){text(3,8,DIM,"Dock to view ships for sale.");footer("O BACK");return;}
 panel(8,32,218,156);panel(234,32,238,156);
 /* Keep the catalogue inside its panel.  The left rail is deliberately
  * reserved for the scroll thumb so distance and price columns never clash. */
 int visible=7,first=row-visible/2;if(first<0)first=0;if(first>player_ship_count-visible)first=player_ship_count-visible;if(first<0)first=0;
 for(int j=0;j<visible&&first+j<player_ship_count;j++){int i=first+j,y=5+j*2;if(i==row)rect(18,y*8-2,196,14,RGB(25,65,77));text(3,y,i==row?WHITE:DIM,"%-12.12s%s",player_ships[i].name,i==game.ship?" [OWNED]":"");}
 if(player_ship_count>visible){int rail_y=40,rail_h=112;rect(12,rail_y,3,rail_h,RGB(45,58,70));int thumb_h=fmaxf(12,(float)rail_h*visible/player_ship_count);int thumb_y=rail_y+(rail_h-thumb_h)*first/(player_ship_count-visible);rect(12,thumb_y,3,thumb_h,AMBER);}
 const PlayerShip *p=&player_ships[row];
 preview_clip(353,84,242,40,464,128);
 fitted_ship_preview(mesh_id(p->name),preview_time*.5f,preview_time*.22f);
 flush_meshes();preview_reset();
 text(31,17,UI_CYAN,"%.22s",p->name);
 text(31,18,WHITE,"Hold %d t  Spd %d",p->capacity,p->speed);
 text(31,19,WHITE,"Range %.1f LY",p->range*.1f);
 text_px(248,160,UI_CYAN,"W%d D%d N%d H%d F%d U%d",fit_capacity(row,0),fit_capacity(row,1),fit_capacity(row,2),fit_capacity(row,3),fit_capacity(row,4),fit_capacity(row,5));
 text(31,21,UI_GOLD,"%.1f units",(p->price-player_ships[game.ship].price*3/4)*.1f);
 /* Full-width copy beneath catalogue and preview; credits retain their own strip. */
 text(3,24,UI_GOLD,mega?"MEGA SHIPYARD / CAPITAL HULLS COMING SOON":"SHIP DESCRIPTION");
 text_wrap(3,26,54,4,DIM,shipyard_description[row],0);
 text_px(280,192,UI_GOLD,"BALANCE %.1f U",game.credits*.1f);
 footer("UP/DOWN   X EXCHANGE   O BACK");
}
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
 "RED BEAM LASER","GREEN PULSE LASER","BLUE ION LANCE","VIOLET ARC LASER","RAINBOW PRISM LASER","AMBER BURST LASER","CYAN RIPPLE LASER","WHITE RAIL LASER","ORANGE FLARE ARRAY","PINK PHASE LASER","LIME SHARD ARRAY","BLACKSTAR DISRUPTOR"
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
 "RED BEAM","GREEN PULSE","BLUE ION","VIOLET ARC","RAINBOW PRISM","AMBER BURST","CYAN RIPPLE","WHITE RAIL","ORANGE FLARE","PINK PHASE","LIME SHARD","BLACKSTAR"
};
static const char *equipment_details[EQUIP_COUNT]={
 "Refill jump fuel to this ship's tank limit. Fuel is separate from cargo.",
 "24 base damage per shot. X fires in space. Weapon pips modify damage.",
 "36 damage per shot. Arm a fitted laser with Square on Ship Loadout.",
 "Add one missile, up to four. Circle+Up selects; tap Circle fires at a locked ship.",
 "Guidance range 8,000 m. Triangle opens Comms; select REQUEST AUTO-DOCK.",
 "Marks your plotted next hop on the nearby jump list. Plot a route first.",
 "Shield recovery becomes 3.0/s before SYS-pip scaling. Best fitted rate wins.",
 "Shield recovery becomes 4.5/s before SYS-pip scaling. Best fitted rate wins.",
 "Passive cooling rises to 38/s. Boosting and nearby suns still generate heat.",
 "Circle+Right selects. Tap Circle dumps 40 heat; 30s cooldown.",
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
 "Circle+R selects. Circle scans 20 km. Radar echoes last 12s; patrols keep moving.",
 "42 damage red beam. Reliable mid-range pressure with a warm red twin trace.","20 damage rapid green pulses. Forgiving aim and low heat at close range.","58 damage blue lance. Drains 18 shields on impact; long, focused blue fire.","30 damage violet arcs. Wide aim cone and a jagged electric visual.","34 damage prism bursts. Gold, pink and cyan pulse colours cycle each shot.","52 damage amber burst. Heavy two-shot-looking fire with 18 percent armour bypass.","26 damage cyan ripple. A cool zig-zag wave that interrupts hostile firing.","110 damage white rail. Very hot, narrow, slow long-range precision shot.","46 damage orange flare spread. Devastating close-range fan of heated shards.","38 damage pink phase pulse. 35 percent bypasses shields into hull integrity.","32 damage lime shard spread. Very wide, fast anti-fighter pattern.","76 damage Blackstar disruptor. Slows hostile firing for two seconds."
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
 "42 damage / 2800m","20 damage / 1600m","58 + shield drain / 3200m","Arc cone / 1900m","Prism pulse / 2500m","52 + armour bypass / 2600m","Arc interrupt / 2100m","110 rail / 4800m","Close flare spread / 1200m","Phase bypass / 2400m","Wide shard fan / 1500m","76 + 2s disrupt / 3000m"
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
 11800,8600,16400,14200,19500,17600,15100,29000,13300,16800,12100,22400
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
 7,5,8,8,10,9,8,11,7,9,6,10
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
 255,255,255,255,255,255,255,255,255,255,255,255
};
static const char *equip_cat_name(int i){
 int slot=equip_slot_for(i);static const char *slots[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
 return slot>=0?slots[slot]:i==0?"FUEL":"AMMO";
}
static int equipment_owned(int i){
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
 2,2,3,3,4,3,3,4,2,3,2,4
};
static int equipment_in_stock(int i){
 if(i<0||i>=EQUIP_COUNT)return 0;
 if(i==0)return 0;
 /* Lave is the first dedicated weapon house: its catalogue is deliberately
    broad, while every other hub continues to use tech/economy-based stock. */
 if(game.system==7)return equip_slot_for(i)==FIT_WPN||i==3;
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
 for(int i=0;i<EQUIP_COUNT&&n<maxn;i++)if(equipment_econ[i]==0&&!equipment_owned(i))out[n++]=i;
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
static void buy_equipment(int i,int chandler){
 if(!game.docked){message(&game,"Dock to buy equipment.");return;}
 if(i<0||i>=EQUIP_COUNT)return;
 if(chandler){int offers[8],n=sc_exclusive_catalog(offers,8),found=0;for(int k=0;k<n;k++)if(offers[k]==i)found=1;if(!found){message(&game,"Not offered by this chandler.");return;}}
 else if(!equipment_in_stock(i)){message(&game,"Not stocked at this hub.");return;}

 if(i==3){if(game.missiles>=4){message(&game,"Missile rack full.");return;}if(game.credits<equipment_costs[i]){message(&game,"Not enough units.");return;}game.credits-=equipment_costs[i];game.missiles++;game.cue=SFX_UI;message(&game,"Missile loaded.");return;}
 if(equipment_owned(i)){message(&game,"Already fitted.");return;}

 int slot=equip_install_slot(i);if(slot<0){message(&game,"Cannot fit that.");return;}
 int cost=equipment_costs[i],refund=0,old=game.fit[slot];
 if(old==22&&game.passenger_dest>=0){message(&game,"Deliver your passenger before replacing the cabin.");return;}
 if(old!=FIT_EMPTY){refund=equip_sell_price(old);cost-=refund;}
 if(game.credits<cost){message(&game,"Not enough units.");return;}
 {uint8_t prev=game.fit[slot];game.fit[slot]=(uint8_t)i;fit_rebuild(&game);
  if(cargo_used(&game)>cargo_capacity(&game)){game.fit[slot]=prev;fit_rebuild(&game);message(&game,"Cargo will not fit that hold.");return;}}
 game.credits-=cost;game.cue=SFX_UI;
 if(old!=FIT_EMPTY){char note[80];snprintf(note,sizeof(note),"Fitted %s (traded in %.1f).",equipment_list_names[i],refund*.1f);message(&game,note);}
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
 return equipment_costs[i]-(slot>=0&&fit_value_valid(slot,old)&&old!=FIT_EMPTY?equip_sell_price(old):0);
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
static void equipment_buy_action(int i,int chandler){
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
 if(game.system==7)return SHOP_WEAPONS;
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
static void equipment(void){
 header("OUTFITTING");
 if(!game.docked){text(3,8,DIM,"Dock to view equipment.");footer("O BACK");return;}
 rect(8,31,464,13,UI_PANEL);rect(8,31,464,1,UI_GOLD);rect(8,43,464,1,UI_RAISED);
 text_px(16,33,UI_GOLD,"WELCOME TO %.29s",equipment_shop_name());
 text_px(286,33,UI_CYAN,"%.22s",equipment_shop_line());
 panel(8,48,186,182);panel(200,48,272,182);
 int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);
 if(!n){text_px(24,72,DIM,"NO STOCK");text_wrap(26,9,31,4,WHITE,"No modules are available at this hub. Visit a higher-tech station.",0);footer("TRI LOADOUT   O BACK");return;}
 if(row<0||row>=n)row=0;int i=list[row],slot=equip_install_slot(i),old=slot>=0?game.fit[slot]:FIT_EMPTY;
 text_px(24,56,UI_CYAN,"LOCAL STOCK");text_px(128,56,DIM,"%02d/%02d",row+1,n);
 int first=row/7*7,shown=n<7?n:7,thumb=140*shown/n;
 rect(12,77,3,140,UI_RAISED);rect(12,77+(140-thumb)*(first<n-shown?first:n-shown)/(n>shown?n-shown:1),3,thumb,UI_ACCENT);
 for(int j=0;j<7&&first+j<n;j++){
  int id=list[first+j],y=78+j*20,active=first+j==row;
  if(active){rect(20,y-3,168,16,UI_RAISED);rect(20,y-3,3,16,UI_GOLD);}
  text_px(26,y,active?WHITE:equipment_owned(id)?UI_CYAN:DIM,"%.17s",equipment_list_names[id]);
  if(equipment_owned(id))text_px(176,y,UI_CYAN,"*");
 }
 text_px(208,56,WHITE,"%.31s",equipment_names[i]);text_px(208,72,UI_CYAN,"%s / %s",equip_cat_name(i),slot<0?"SERVICE":"MODULE");
 text_px(208,88,UI_GOLD,"STATS:");text_wrap(26,13,31,2,WHITE,equipment_stat_change(i),0);
 text_wrap(26,16,31,3,DIM,equipment_details[i],0);
 text_px(208,156,UI_CYAN,slot<0?"NO SLOT REQUIRED":equipment_owned(i)?"CURRENTLY FITTED":equip_confirm_item==i&&equip_slot_for(i)==FIT_WPN?"CHOOSE WPN SLOT":"REPLACES");
 if(slot>=0){if(old==FIT_EMPTY)text_px(208,168,WHITE,"EMPTY %s SLOT %d",equip_cat_name(i),slot/6+1);else text_px(208,168,WHITE,"%.17s / %s %d",equipment_list_names[old],equip_cat_name(i),slot/6+1);}
 int net=equipment_net_cost(i);
 text_px(208,184,net>game.credits?RED:UI_GOLD,equipment_owned(i)?"NO PURCHASE NEEDED":net<0?"RETURN %.1f U":"COSTS: %.1f U",fabsf(net)*.1f);
 text_px(208,200,DIM,slot>=0&&old!=FIT_EMPTY?"Trade-in %.1f U":"",slot>=0&&old!=FIT_EMPTY?equip_sell_price(old)*.1f:0);
 text_px(208,216,UI_GOLD,"BALANCE %.1f U",game.credits*.1f);
 if(game.message_time>0)text_wrap(2,29,56,2,UI_GOLD,game.message,0);
 else text_px(16,234,DIM,"* FITTED/FULL   Ship Loadout shows all fitted modules");
 footer(equip_confirm_item<-1?"SQUARE CONFIRM SALE   O CANCEL":equip_confirm_item==i&&equip_slot_for(i)==FIT_WPN&&fit_empty_slot(&game,FIT_WPN)<0?"L/R WPN SLOT   X CONFIRM   O CANCEL":equip_confirm_item>=0?"X CONFIRM REPLACEMENT   O CANCEL":equipment_owned(i)&&slot>=0?"UP/DOWN   SQUARE SELL   TRI LOADOUT   O BACK":"UP/DOWN   X BUY   TRI LOADOUT   O BACK");
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
static void inventory_screen(void){
 header("SHIP TECH BOARD");
 panel(8,32,252,190);panel(268,32,204,190);
 const char *cats[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
 const char *chip[EQUIP_COUNT]={"FUE","PLS","BEM","MIS","DCK","NAV","SHD","MIL","COL","HSK","BAY","FRG","LRG","PLN","SCP","AGR","ECM","CHF","ESC","REP","MIN","REF","PAX","CLP","BUF","HVY","RPD","LNC","SCT","ION","PLS","DIS","ARM","RLY","PRI","THM","IMP","MBH","DEP","SUR","OPT","AIM","SAL","C24","C32","C04","IND","COR","ECO","BAF","AMP","OVR","SRV","CLD","DRN","RAD","LAW","RBE","GPL","ION","ARC","PRM","ABR","RPL","RAIL","OFR","PHS","LMS","BST"};
 if(row<0||row>=FIT_SLOTS||!fit_slot_open(&game,row))row=0;
 int count=0,total=0;for(int c=0;c<6;c++)total+=fit_capacity(game.ship,c);
 for(int i=0;i<FIT_SLOTS;i++)count+=fit_slot_open(&game,i)&&game.fit[i]!=FIT_EMPTY;
 text_px(16,40,UI_CYAN,"%.22s",player_ships[game.ship].name);
 text_px(16,54,DIM,"MODULES %d/%d",count,total);
 for(int c=0;c<6;c++){
  int y=76+c*23;text_px(16,y+4,c==row%6?UI_GOLD:UI_CYAN,"%s",cats[c]);
  /* A bank is made from native 44px module cells.  Do not stretch one- or
     two-slot categories across the whole board: their compact width tells
     the player at a glance that this hull has fewer available bays. */
  int cap=fit_capacity(game.ship,c),gap=4,w=44;
  for(int b=0;b<cap;b++){
   int slot=c+b*6,x=64+b*(w+gap),item=game.fit[slot];
   unsigned edge=slot==row?UI_GOLD:UI_EDGE;
   rect(x,y,w,19,edge);rect(x+1,y+1,w-2,17,slot==row?UI_RAISED:UI_PANEL);
   if(item==FIT_EMPTY)text_px(x+4,y+5,UI_TEXT,"%d +",b+1);else text_px(x+4,y+5,UI_TEXT,"%d %s",b+1,item<EQUIP_COUNT?chip[item]:"ERR");
   if(c==FIT_WPN&&slot==game.active_weapon&&item!=FIT_EMPTY)rect(x+4,y+16,35,2,UI_SIGNAL);
  }
 }
 int mod=game.fit[row];
 text_px(280,40,UI_GOLD,"%s SLOT %d/%d",cats[row%6],row/6+1,fit_capacity(game.ship,row%6));
 text_px(280,56,UI_CYAN,mod==FIT_EMPTY?"EMPTY":row==game.active_weapon?"ACTIVE WEAPON":"INSTALLED");
 text_px(16,66,UI_MUTED,"AVAILABLE SLOTS ONLY");
 if(mod==FIT_EMPTY)text_wrap(35,10,23,4,DIM,"Available installation slot. Purchase and fit compatible equipment through Outfitting.",0);
 else {
  text_px(280,80,WHITE,"%.23s",equipment_names[mod]);
  text_wrap(35,12,23,5,DIM,equipment_details[mod],0);
  text_wrap(35,18,23,3,UI_CYAN,equipment_effects[mod],0);
 }
 text_px(16,224,UI_GOLD,"HOLD %d/%dT   HULL %d",cargo_used(&game),cargo_capacity(&game),(int)game.hull);
 if(sell_confirm_slot==row)text_px(16,236,UI_GOLD,"Sell module for %.1f U? X confirms.",equip_sell_price(mod)*.1f);
 else if(game.message_time>0)text_px(16,236,UI_GOLD,"%.56s",game.message);
 footer(sell_confirm_slot==row?"X CONFIRM SALE   O CANCEL":row%6==FIT_WPN?(game.docked?"D-PAD SLOT   X SELL   SQUARE ARM   O BACK":"D-PAD SLOT   SQUARE ARM WEAPON   O BACK"):(game.docked?"D-PAD SLOT   X SELL   O BACK":"D-PAD SLOT   O BACK"));
}

static int engineer_fuel_cost(void){return (int)ceilf(fmaxf(0,player_ships[game.ship].range-game.fuel))*2;}
static void engineer_refuel(void){
 if(!game.docked){message(&game,"Dock to refuel.");return;}
 int cost=engineer_fuel_cost();
 if(!cost){message(&game,"Tank is already full.");return;}
 if(game.credits<cost){message(&game,"Not enough units to refuel.");return;}
 game.credits-=cost;refuel_full(&game);game.cue=SFX_UI;message(&game,"Fuel tank filled.");
}
static void repair_screen(void){
 header("STATION / MECHANICS");panel(8,32,464,156);
 text(3,5,UI_CYAN,"HULL SERVICE");text(3,7,WHITE,"%.25s",player_ships[game.ship].name);
 text(3,10,DIM,"HULL");pip_bar(48,80,270,6,(int)game.hull,game.hull<35?RED:UI_CYAN);
 text(3,14,DIM,"SHLD");pip_bar(48,112,270,6,(int)game.energy,game.energy<35?RED:UI_CYAN);
 int cost=ship_repair_cost(&game);
 if(cost>0){text(3,19,UI_GOLD,"MECHANIC FEE");text(22,19,WHITE,"%.1f units",cost*.1f);text(3,21,DIM,"Restores hull, shields and heat sinks.");}
 else {text(3,19,UI_CYAN,"SHIP CONDITION: NOMINAL");text(3,21,DIM,"No engineering work is required.");}
 text_px(24,200,UI_CYAN,"FUEL %.1f / %.1f LY",game.fuel*.1f,player_ships[game.ship].range*.1f);
 text_px(24,216,UI_GOLD,"REFUEL COST: %.1f U",engineer_fuel_cost()*.1f);
 if(game.message_time>0)text_px(24,234,UI_GOLD,"%.54s",game.message);
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
 int done=system_ops_bits(game.landed_planets[sys]&15u)+system_ops_bits(game.rift_logged[sys]&15u)+system_ops_bits(game.bounty_claimed[sys]&31u);
 for(int body=1;body<BODY_COUNT;body++)done+=system_ops_species(sys,body)+system_ops_sites(sys,body);
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
 for(int body=1;body<BODY_COUNT;body++)if(!(game.landed_planets[sys]&(1u<<(body-1)))){snprintf(note,sizeof(note),"SURVEY NEXT: %.25s",game.bodies[body].name);return note;}
 for(int body=1;body<BODY_COUNT;body++){int life=system_ops_species(sys,body),sites=system_ops_sites(sys,body);if(life<LIFE_COUNT||sites<8){snprintf(note,sizeof(note),"FIELDWORK: %.20s / LIFE %d/8 SITES %d/8",game.bodies[body].name,life,sites);return note;}}
 int rifts=ANOMALY_COUNT-system_ops_bits(game.rift_logged[sys]&15u);if(rifts){snprintf(note,sizeof(note),"DEEP SCAN: %d STELLAR RIFT%s UNLOGGED",rifts,rifts==1?"":"S");return note;}
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
 int kind=station_class_for_system(&game,sys),lawful=system_is_lawful(&game,sys);
 if(sys==7){snprintf(out,cap,"Lave is a bright lawful crossroads. Its orbital markets welcome traders while old survey routes lead toward four distant worlds.");return;}
 if(kind==STATION_MEGA)snprintf(out,cap,"%s is a vast %s capital. Its orbital city watches four busy worlds circle a brilliant primary.",game.systems[sys].name,lawful?"lawful":"pirate-held");
 else if(kind==STATION_POOR)snprintf(out,cap,"%s is a hard-bitten %s frontier. A compact port serves four remote worlds beneath a restless sun.",game.systems[sys].name,lawful?"lawful":"pirate-held");
 else snprintf(out,cap,"%s is a prosperous %s crossroads. Its citadel welcomes traffic bound for four varied worlds.",game.systems[sys].name,lawful?"lawful":"pirate-held");
}
static void system_details(void){
 int sys=game.system,star_x=19,star_y=121,offset=(int)(preview_time*4);char description[192];
 static const int bx[BODY_COUNT]={19,116,161,215,272},by[BODY_COUNT]={121,121,111,129,141};
 int px[BODY_COUNT]={star_x,0,0,0,0},py[BODY_COUNT]={star_y,0,0,0,0},pr[BODY_COUNT]={72,0,0,0,0};
 header("SYSTEM ALMANAC");panel(8,29,300,199);panel(314,29,158,199);
 draw_next_art(almanac_backdrop,ALMANAC_BACKDROP_W,ALMANAC_BACKDROP_H,11,32,ALMANAC_BACKDROP_W,ALMANAC_BACKDROP_H);
 for(int i=0;i<18;i++){unsigned h=station_profile_hash(sys,i+23);int x=14+(h%288),y=35+((h>>9)%185),twinkle=((int)(preview_time*2)+(h>>18))%7;if(twinkle<2)pixel(x,y,twinkle?WHITE:UI_CYAN);}
 for(int i=1;i<BODY_COUNT;i++)almanac_ellipse(star_x,star_y,56+i*45,18+i*9,RGB(96,96,82),offset+i*7);
 draw_next_art(almanac_sun,ALMANAC_SUN_W,ALMANAC_SUN_H,star_x-ALMANAC_SUN_W/2,star_y-ALMANAC_SUN_H/2,ALMANAC_SUN_W,ALMANAC_SUN_H);
 for(int i=1;i<BODY_COUNT;i++){
  unsigned h=game.bodies[i].seed;px[i]=bx[i]+(int)(sinf(preview_time*.10f+i*1.7f)*2);py[i]=by[i]+(int)(cosf(preview_time*.12f+i)*1);
  pr[i]=7+i*3+(game.bodies[i].type==GAS?2:game.bodies[i].type==OCEAN?1:0);
  draw_planet_sprite(px[i],py[i],pr[i],h,game.bodies[i].type,11,32,305,225);
 }
 almanac_station_art(145,176);
 {unsigned h=station_profile_hash(sys,77);int sx=86+(int)fmodf(preview_time*19+(h%190),208),sy=54+((h>>9)%126);line(sx-10,sy,sx-3,sy,RGB(40,93,110));line(sx-5,sy-1,sx,sy,UI_CYAN);pixel(sx+1,sy,WHITE);}
 if(row==0)almanac_brackets(145,176,station_class(&game)==STATION_MEGA?47:station_class(&game)==STATION_RICH?39:30,UI_GOLD);
 else almanac_brackets(px[row-1],py[row-1],pr[row-1]+5,UI_GOLD);
 text_px(17,38,UI_GOLD,"%.18s",game.systems[sys].name);
 text_px(324,40,UI_GOLD,"%.20s",game.systems[sys].name);line(324,55,462,55,UI_RAISED);
 almanac_description(sys,description,sizeof(description));text_wrap(41,8,17,7,WHITE,description,0);
 line(324,127,462,127,UI_RAISED);
 text_px(324,137,UI_CYAN,"GOVERNMENT");text_px(324,150,system_is_lawful(&game,sys)?WHITE:RED,"%.22s",almanac_government(sys));
 text_px(324,171,UI_CYAN,"ECONOMY");text_px(324,184,UI_GOLD,"%s",station_class_name(station_class(&game)));
 text_px(324,205,UI_CYAN,"KNOWN WORLDS");text_px(424,205,WHITE,"4");
 footer("UP/DOWN WORLD   X TARGET + RETURN   O BACK");
}
#include "gazette-lore.h"
#include "faction-dossier.h"
static void mission_board(void){
 header("MISSION BOARD / LOCAL CONTRACTS");
 if(!game.docked){text(3,8,DIM,"Dock at a station to take work.");footer("O BACK");return;}
 int count=mission_count(&game);if(row<0||row>=count)row=0;
 text_px(16,32,UI_CYAN,"%.18s / %d OPEN JOBS",station_name(&game),count);
 text_px(344,32,game.job_n>=MISSION_SLOTS?RED:UI_GOLD,"LOG %d/%d",game.job_n,MISSION_SLOTS);
 panel(8,48,186,180);panel(200,48,272,180);
 for(int i=0;i<count;i++){
  int y=57+i*33,dest=mission_destination(&game,i),type=mission_type_for_offer(&game,i),active=mission_offer_active(&game,i);
  unsigned accent=active?UI_GOLD:faction_colors[type==MISSION_BOUNTY?LAW:type==MISSION_EXPLORATION?EXPLORERS:TRADERS];
  if(i==row){rect(14,y-3,174,29,UI_RAISED);rect(14,y-3,3,29,accent);}
  text_px(24,y,i==row?WHITE:DIM,"%.19s",mission_name(type));
  text_px(24,y+12,accent,"%s %.14s",active?"*":">",game.systems[dest].name);
 }
 if(count){
  int type=mission_type_for_offer(&game,row),dest=mission_destination(&game,row),risk=mission_risk(&game,row),active=mission_offer_active(&game,row);
  const char *issuer[]={"DOCK COOPERATIVE","LOCAL BOUNTY DESK","SURVEY OFFICE","RESCUE NETWORK","QUIET COURIERS"};
  const char *requirements[]={"One tonne of food supplied. Dock at the destination with it aboard.","Destroy the marked pirate. Fit a weapon before setting out.","Approach the marked planet; press Triangle within 1000 m of its surface.","Hail the marked ship within 600 m, then dock at that system's station.","One tonne of narcotics supplied. Deliver it intact; Law may scan you."};
  text_px(208,56,UI_GOLD,"%s",issuer[type]);
  draw_portrait(208,76,40,40,game.system*17+row,type==MISSION_BOUNTY?LAW:type==MISSION_EXPLORATION?EXPLORERS:TRADERS);
  text_px(264,78,WHITE,"TO %.22s",game.systems[dest].name);
  text_px(264,94,UI_GOLD,"PAY %.1f U",mission_reward(&game,row)*.1f);
  text_px(264,110,risk>=4?RED:UI_CYAN,"RISK %d/5",risk);
  text_wrap(26,16,31,4,WHITE,mission_brief(&game,row),0);
  text_wrap(26,21,31,3,DIM,requirements[type],0);
  text_px(208,208,active?UI_GOLD:UI_CYAN,active?"ACCEPTED / X NEXT STEP":"DEPOSIT 10 U / X ACCEPT");
 }
 if(game.message_time>0)text_wrap(2,29,56,2,UI_GOLD,game.message,0);
 else text_px(16,234,DIM,"Accept > Mission Log > Tracked Mission > Objective");
 footer("UP/DOWN   X ACCEPT / VIEW   SELECT LOG   O BACK");
}
static void mission_log(void){
 header("MISSION LOG / CHOOSE TRACKED");panel(8,32,464,154);
 text(2,5,UI_CYAN,"MISSIONS");text(43,5,DIM,"* TRACKED");
 int total=mission_log_count();
 if(row<0)row=0;if(row>=total)row=total-1;
 int first=row>6?row-6:0;
 for(int i=first;i<total&&i<first+7;i++){
  int y=7+(i-first)*2;int track=mission_track_at(i);
  if(i==row)selected(y);
  unsigned ink=track==tracked_mission?UI_GOLD:i==row?WHITE:DIM;
  if(i==0)text(2,y,ink,"%s KEI + RYN / OPEN CHANNEL",i==tracked_mission?"*":" ");
  else if(i==1)text(2,y,ink,"%s EXPLORERS GUILD",i==tracked_mission?"*":" ");
  else if(track==TRACK_LAVE)text(2,y,ink,"%s LAVE / BERTH SIX",tracked_mission==track?"*":" ");
  else if(track==TRACK_STATION_TOUR)text(2,y,ink,"%s STATION WELCOME / REORTE",tracked_mission==track?"*":" ");
  else {
   Job *j=&game.jobs[i-2];
   text(2,y,ink,"%s %.24s",i==tracked_mission?"*":" ",mission_name(j->type));
   text(33,y,ink,"%.24s",game.systems[j->dest].name);
  }
 }

 rect(8,190,464,42,RGB(15,31,39));
 const char *objective=mission_track_at(row)==TRACK_LAVE?sc_lave_objective():mission_track_at(row)==TRACK_STATION_TOUR?station_tour_objective():row==0?(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT?saga_beats[game.saga_chapter].objective:campaign_task(&game)):row==1?guild_objective(&game):mission_objective_at(&game,row-2);
 if(abandon_confirm){text(2,24,RED,"ABANDON THIS CONTRACT?");text(2,26,WHITE,"X CONFIRM / O CANCEL");}
 else {text(2,24,UI_GOLD,"NEXT:");text_wrap(8,24,49,3,UI_GOLD,objective,0);}
 footer(row>=2&&row<2+game.job_n?"X TRACK   SELECT NEXT STEP   TRI ABANDON   O BACK":"X TRACK   SELECT NEXT STEP   O BACK");
}
static void navigate_job(int index){if(index<0||index>=game.job_n)return;Job *j=&game.jobs[index];game.job_sel=index;route_set_goal(&game,j->dest);game.destination=j->dest;if(game.system!=j->dest){int jumps=0,hop=route_next_hop(&game,j->dest,&jumps);if(hop>=0){game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),"Route: %d jump%s. Refuel at intermediate hubs.",jumps,jumps==1?"":"s");message(&game,note);}else message(&game,"No fuel-safe route. Refuel at the hub first.");}else {route_clear(&game);if(game.docked){message(&game,"Launch, then navigate from Tracked Mission.");change_page(HOME);return;}int id=mission_target_id(&game,index);if(id>=0&&valid_target(id)){selected_target=id;scan_cat=target_category(id);autoaim=1;message(&game,"Mission target locked. Auto-align active.");if(!game.docked)change_page(FLIGHT);}else if(game.docked)message(&game,"Launch to continue this objective in the system.");else message(&game,"Objective unavailable. Re-enter this system.");}}
static int galnet_tab=0;
#include "social-feed.h"
static int galnet_rows(void){return galnet_tab==0?16:galnet_tab==3?social_rows():5;}
static int active_role(int role){int n=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==role)n++;return n;}
static void galnet_post(int i,char *author,int alen,char *body,int blen){
 if(tutorial_active(&game)&&galnet_tab==3&&i==0){snprintf(author,alen,"Kei / pinned evidence");snprintf(body,blen,"Ryn's receipt: 'If I miss supper, ask who kept the light on.' Lave. Two tonnes of food.");return;}
 if(tutorial_active(&game)&&galnet_tab==4&&i==0){snprintf(author,alen,"Venn / private");snprintf(body,blen,"Kei kept the berth. Loader kept the receipt. Check the station people, not just the screens.");return;}
 if(galnet_tab==3){unsigned code=social_code(i);if(code){int at=social_event_row(i);social_post_text(code,(game.social.origin[at]&256)!=0,author,alen,body,blen);}else{snprintf(author,alen,"Spacebook");snprintf(body,blen,"Your flights, discoveries and questionable decisions will appear here. Save your commander to keep them.");}return;}
 int risk=danger_rating(&game,game.system),wealth=prosperity(&game,game.system),item=(game.system*3+i*5)%GOODS;
 if(galnet_tab==0){
  if(i==5){snprintf(author,alen,"ORBITAL PUZZLE");snprintf(body,blen,"Orbit Sudoku: fill four sectors. No repeated signals.");}
  else if(i==6){snprintf(author,alen,"JOKE WIRE");snprintf(body,blen,"Why did the pilot cross the jump lane? The other side had better prices.");}
  else if(i==7){snprintf(author,alen,"CAPTAIN'S COLUMN");snprintf(body,blen,"A quiet lane is not an empty lane. Leave room for the story.");}
  else if(gazette_wants_tabloid(game.system,i)){
   snprintf(author,alen,"%s",gazette_tabloid_author(game.system,i));
   snprintf(body,blen,"%s",gazette_tabloid_body(game.system,i));
  }else{
   const char *a[]={"SYSTEM DESK","LAW BULLETIN","TRAFFIC CONTROL","ECONOMY WIRE","EXPLORER GUILD"};
   snprintf(author,alen,"%s",a[i]);
   if(i==0)snprintf(body,blen,"%s risk is %d/5: %s traffic expected.",game.systems[game.system].name,risk,risk>=4?"heavy combat":"routine");
   else if(i==1)snprintf(body,blen,"%d Law craft and %d pirates currently on scanner.",active_role(LAW),active_role(PIRATES));
   else if(i==2)snprintf(body,blen,"%s",travellers_galnet_traffic(&game));
   else if(i==3)snprintf(body,blen,"Local prosperity %d/5; technology level %d.",wealth,game.systems[game.system].tech+1);
   else{const char *echo=saga_galnet_desk(&game);if(echo)snprintf(body,blen,"%s",echo);else snprintf(body,blen,system_whales(game.system)?"Migrating whales reported beyond the gas giant.":system_ice_belt(game.system)?"Ice-belt survey teams are charting the outer ring.":system_rock_belt(game.system)?"A rock belt is visible off the inner worlds.":"Survey teams are charting all four local worlds.");}
  }
 }
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
static void gazette_orbit_puzzle(int x,int y,int seed){
 unsigned ink=RGB(45,65,60);rect(x,y,65,65,RGB(221,215,176));
 for(int r=0;r<4;r++)for(int c=0;c<4;c++){
  int value=(r*2+r/2+c+(seed&3))%4+1;
  if(((r*7+c*3+seed)%5)<3)text_px(x+c*16+4,y+r*16+4,ink,"%d",value);
 }
 for(int i=0;i<=4;i++){rect(x+i*16,y,i%2?1:2,65,ink);rect(x,y+i*16,65,i%2?1:2,ink);}
}
static const char *news_extra_title[]={
 "THE SHIP THAT CAME HOME","LETTERS: LOST AND FOUND","JOKES FROM THE OUTER RIM","THE LAST CUP",
 "SMALL ADS / BIG DREAMS","SCIENCE: A MOON THAT HUMS","DOCKSIDE DINNER","EDITOR'S NIGHT WATCH"
};
static const char *news_extra_body[]={
 "The freighter arrived eleven years late. Its pilot apologised for the delay and asked whether her soup was still warm. Dock control found the original order in an archive. The canteen refused to charge interest. By midnight, three generations of loaders were sharing dinner beneath her hull.",
 "To whoever returned my missing maintenance drone: thank you. To whoever taught it to whistle whenever I reverse: we need to talk. It now insists on being called Captain. The registry says this is technically allowed. Yours, a increasingly junior engineer.",
 "Why do pirates avoid libraries? Too many overdue warrants. A mechanic offered to fix my jump drive while I waited. I am now waiting last Tuesday. My copilot says I take everything literally. So I took the cargo labelled EVERYTHING. Customs were magnificent about it.",
 "There was one cup of coffee left on the rescue tug. The captain gave it to the survivor, who held it for an hour without drinking. 'My brother made this mug,' she said. They turned the tug around. The next beacon was faint, but it was still transmitting.",
 "FOR SALE: gravity boots, one careful ceiling. WANTED: pianist for a zero-gravity lounge; must bring own straps. ROOM TO LET: excellent view, occasional eclipse. SWAP: slightly haunted scanner for ordinary kettle. The scanner knows when you are thirsty.",
 "Surveyors traced a moon's mysterious hum to resonating caverns beneath its ice. The sound changes as its parent planet rises. A composer has requested landing rights. The survey team asks visitors to stop applauding: the microphones are sensitive.",
 "Loader Sen cooks for fourteen people in a kitchen designed for two. Her rule is simple: whoever complains washes up. Last week a visiting admiral asked for the recipe. Sen handed him a sponge. He returned the next evening with his own apron.",
 "At closing time the station looks almost still. A tug light moves across the glass. Someone calls home. Someone decides to stay another day. Tomorrow's edition will have prices, warnings and arguments. Tonight we leave a little space for everyone still on their way."
};
/* Tiny print adverts: deliberately drawn at native resolution, no texture cost. */
static const char *gazette_ad[]={
 "ORBIT COFFEE. Wake up in the correct century. Lid included.",
 "MOON BOOTS. For the person who has almost everything underfoot.",
 "NEBULA NOODLES. Hot in three minutes. Even in deep space.",
 "SECOND SUN LAMPS. Bring a little dawn to the night shift.",
 "TUG & TOW. Missed your berth? We specialise in awkward arrivals.",
 "ASTRO SOAP. Removes engine grease. Memories sold separately.",
 "VOID INSURANCE. Read the small print before entering a void.",
 "COMET COURIERS. Your parcel deserves its own adventure."
};
static const char *gazette_small[]={
 "Night-shift navigator seeks company for quiet orbits. Box 14.",
 "FOUND: one left glove. Right glove still wanted. Dock office.",
 "RIDDLE: What has rings but no fingers? A planet. No refunds.",
 "Dear editor: stop calling my shuttle vintage. It can hear you.",
 "SWAP: telescope for curtains. The neighbours wave back.",
 "CLUB: cloud-watchers meet at sunset. Bring your own planet.",
 "FOR RENT: cosy bunk. Sea view on selected flight paths only.",
 "NOTE TO SELF: buy milk BEFORE leaving this star system."
};
static void gazette_supplement(int article,unsigned ink,unsigned rule){
 int ad=(article+game.system)%8,small=(article*3+game.system)%8;
 rect(24,190,440,1,rule);rect(264,194,1,48,rule);
 text(3,24,ink,article&1?"SPONSORED / SMALL PRINT":"DOCKSIDE / COMMERCIAL");
 text(34,24,ink,article%3==0?"PERSONAL / NOTICES":article%3==1?"LETTERS / FOUND":"BACK-PAGE ODDITIES");
 /* Eight chunky silhouettes: mug, boots, bowl, lamp, tug, soap, shield, parcel. */
 static const unsigned char icons[8][8]={
  {0x28,0x14,0x00,0x7c,0x46,0x46,0x7c,0x38},
  {0x24,0x24,0x24,0x24,0x36,0x7e,0x00,0x7e},
  {0x28,0x14,0x28,0x00,0x7e,0x3c,0x18,0x3c},
  {0x18,0x3c,0x7e,0x7e,0x18,0x18,0x18,0x7e},
  {0x00,0x18,0x7c,0xff,0x7c,0x18,0x24,0x42},
  {0x04,0x20,0x00,0x3c,0x7e,0x5e,0x7e,0x3c},
  {0x18,0x7e,0x7e,0x5a,0x66,0x3c,0x18,0x00},
  {0x00,0x7e,0x5a,0x7e,0x5a,0x5a,0x7e,0x00}
 };
 for(int y=0;y<8;y++)for(int x=0;x<8;x++)if(icons[ad][y]&(1u<<x))rect(28+x*4,208+y*4,4,4,ink);
 text_wrap(9,26,23,4,ink,gazette_ad[ad],0);
 text_wrap(34,26,23,4,ink,gazette_small[small],0);
 if(article%4==3)line(275,241,447,239,rule); /* reader's pencil underline */
}
static int news_screen(void){
 int fits=1;
 unsigned paper=RGB(212,202,165),ink=RGB(32,36,34),rule=RGB(114,97,70);
 rect(6,43,468,205,paper);rect(20,47,445,2,ink);
 text_px(24,53,ink,"THE GALACTIC GAZETTE");
 text_px(24,67,rule,"%.15s / LOCAL EDITION / %02d OF 16",game.systems[game.system].name,row+1);
 rect(24,80,440,2,ink);
 /* One article per stop: the rail represents the entire edition. */
 text_px(7,84,ink,"^");rect(9,97,3,130,rule);rect(8,97+row*118/15,5,12,ink);text_px(7,235,ink,"v");
 char author[64],body[768];int i=row;
 if(i<8){galnet_post(i,author,sizeof(author),body,sizeof(body));
  size_t used=strlen(body);
  if(i!=5)snprintf(body+used,sizeof(body)-used," %s %s",gazette_dek(game.system,i),
   i==0?"Dock dispatch asks arriving pilots to leave the freight lane clear. The night shift reports steady arrivals, a repaired beacon and a queue at the canteen. Local crews say the best news is an uneventful trip home.":
   i==1?"Warrants name specific ships. Check the Wanted board before opening fire; ordinary traffic is protected. Patrol officers remind hunters that rewards follow confirmed takedowns, even when a target was never selected on the board.":
   i==6?"A pilot ordered a light meal. The waiter switched off artificial gravity. A station installed a new doorbell: it rings a little differently in every atmosphere.":
   "The Gazette will follow the story as local crews return. Letters, eyewitness accounts and corrections are welcome at the station desk.");
 }else{snprintf(author,sizeof(author),"%s",news_extra_title[i-8]);snprintf(body,sizeof(body),"%s",news_extra_body[i-8]);}
 text_px(24,88,ink,"%.54s",author);rect(24,100,440,1,rule);
 if(i==5){
  text_wrap(3,14,39,9,ink,"Orbit Sudoku: complete the four sectors. Each row, column and 2x2 sector must contain signals 1, 2, 3 and 4 exactly once. Pencil your answer on a spare manifest. Please do not submit it to customs as a cargo declaration.",0);
  gazette_orbit_puzzle(384,120,game.system);
 }else {
  const char *left;int used=text_wrap(3,14,54,9,ink,body,&left);fits=!*left;
  if(used<=7)text_px(24,176,rule,"NOTICE: LOST MOONS MUST BE CLAIMED BY THEIR PLANET.");
 }
 gazette_supplement(i,ink,rule);
 footer("L/R SECTION   UP/DOWN READ   O BACK");return fits;
}
static void galnet_market(void){rect(0,42,W,206,RGB(5,16,18));text(2,6,UI_CYAN,"MARKET EXCHANGE / DELAYED PRICES");text(2,8,DIM,"SYMBOL       LOCAL      GAL AVG      TREND");for(int i=0;i<5;i++){int item=(game.system*3+i*5)%GOODS,y=82+i*29,diff=game.price[item]-galactic_price(item);if(i==row)rect(8,y-5,464,24,RGB(15,47,47));text(2,y/8,i==row?WHITE:DIM,"%-12.12s %7.1f %10.1f",goods[item].name,game.price[item]*.1f,galactic_price(item)*.1f);unsigned c=diff>0?RED:UI_CYAN;int x=360,base=y+8;for(int k=0;k<7;k++){int a=((item*13+k*17)%11)-5,b=((item*13+(k+1)*17)%11)-5;line(x+k*14,base-a,x+(k+1)*14,base-b,c);}text(56,y/8,c,diff>0?"UP":"DOWN");}text(2,29,UI_GOLD,game.docked?"LIVE TERMINAL: LEFT SELL / RIGHT BUY":"DOCK FOR LIVE TRADING");footer("L/R SECTION   UP/DOWN TICKER   O BACK");}
static void galnet_bounties(void){
 unsigned board=RGB(30,26,24),ink=RGB(57,33,27);rect(0,42,W,206,board);
 text_px(16,46,UI_GOLD,"LOCAL WARRANTS");text_px(272,46,DIM,"%.22s",game.systems[game.system].name);
 int first=row/3*3;
 /* Selection thumb tracks all warrants, including entries on the second sheet. */
 rect(2,64,3,172,RGB(89,77,60));
 rect(1,64+(BOUNTY_POSTER_COUNT>1?row*152/(BOUNTY_POSTER_COUNT-1):0),5,20,UI_GOLD);
 for(int j=0;j<3&&first+j<BOUNTY_POSTER_COUNT;j++){
  int i=first+j,x=10+j*157,y=62,wear=(game.system*7+i*5)%6;
  unsigned paper=i==row?RGB(231,204,153):RGB(190,170,132);
  rect(x+3,y+3,146,176,RGB(13,12,12));rect(x,y,146,176,paper);
  for(int k=0;k<28;k++){int px=x+4+(k*37+i*11)%137,py=y+5+(k*23+game.system)%166;pixel(px,py,RGB(165,144,110));}
  if(wear==1||wear==4){line(x+2,y+85,x+142,y+89,RGB(162,140,105));line(x+2,y+86,x+142,y+90,RGB(216,194,149));}
  if(wear==2||wear==5)for(int k=0;k<9;k++){rect(x,y+k,9-k,1,board);rect(x+138+k,y+175-k,8-k,1,board);}
  if(wear==3||wear==5){circle(x+130,y+145,7,RGB(141,115,82));circle(x+130,y+145,5,RGB(160,132,92));rect(x+6,y+152,3,4,board);}
  rect(x+67,y-2,12,5,RGB(105,100,89));pixel(x+70,y-1,WHITE);
  text_px(x+25,y+10,ink,"W A N T E D");rect(x+9,y+23,128,2,ink);
  rect(x+33,y+30,80,62,ink);draw_portrait(x+37,y+33,72,56,game.system*31+i*19,PIRATES);
  char label[32];bounty_target_label(&game,i,label,sizeof(label));
  text_px(x+9,y+98,ink,"%.16s",label);
  text_px(x+9,y+112,ink,"BOUNTY %.1f U",bounty_target_reward(&game,i)*.1f);
  text_px(x+9,y+126,ink,"RISK %d/5",danger_rating(&game,game.system));
  rect(x+9,y+140,128,1,RGB(143,110,74));
  if(bounty_target_taken(&game,i)){rect(x+5,y+147,136,21,RGB(49,77,59));text_px(x+9,y+150,UI_GOLD,"TARGET TAKEN");text_px(x+53,y+159,UI_GOLD,"DOWN");}
  else{text_px(x+9,y+148,ink,i==row?"X TRACK TARGET":"LIVE WARRANT");text_px(x+9,y+161,ink,"LOCAL LAW");}
  if(i==row){rect(x-2,y-2,150,2,UI_GOLD);rect(x-2,y+176,150,2,UI_GOLD);rect(x-2,y,2,176,UI_GOLD);rect(x+146,y,2,176,UI_GOLD);}
 }
 footer("L/R SECTION   UP/DOWN POSTER   X TRACK   O BACK");
}
static void galnet_chrome(void){
 header("GALACTICNET // LIVE NETWORK");rect(0,22,W,20,RGB(11,25,35));
 /* Six equal tabs between L/R pads — short labels so SPACEBOOK/MESSAGES never collide. */
 rect(0,22,28,20,RGB(8,18,28));rect(452,22,28,20,RGB(8,18,28));
 rect(2,24,24,16,RGB(25,65,77));rect(454,24,24,16,RGB(25,65,77));
 text(1,3,UI_CYAN,"<L");text(57,3,UI_CYAN,"R>");
 const char *shorts[]={"NEWS","MARKET","WANTED","BOOK","INBOX","JOBS"};
 for(int i=0;i<6;i++){
  int x=30+i*70,tw=68;
  if(i==galnet_tab){rect(x,22,tw,20,RGB(25,65,77));rect(x,40,tw,2,UI_GOLD);}
  int len=(int)strlen(shorts[i]);int col=(x+(tw-len*8)/2)/8;
  text(col,3,i==galnet_tab?WHITE:DIM,"%s",shorts[i]);
 }
}
static void galnet_screen(void){if(galnet_tab==3||galnet_tab==4)spacebook_screen(galnet_tab==4);else if(galnet_tab==0)news_screen();else if(galnet_tab==1)galnet_market();else if(galnet_tab==2)galnet_bounties();else {int count=galnet_rows(),pages=(count+2)/3,first=row/3*3;rect(0,42,W,206,BG);text(2,6,UI_CYAN,"MISSION FEED");page_number_at(20,6,row/3+1,pages);for(int j=0;j<3&&first+j<count;j++){int i=first+j,y=8+j*5;char author[40],body[96];galnet_post(i,author,sizeof(author),body,sizeof(body));panel(14,y*8-3,458,34);if(i==row)rect(14,y*8-3,3,34,UI_GOLD);galnet_avatar(20,y*8-1,28,i);text(7,y,i==row?UI_GOLD:UI_CYAN,"%s",author);text(7,y+2,WHITE,"%.50s",body);}galnet_scrollbar(8,61,114,first,3,count,UI_RAISED,UI_GOLD);footer("L/R SECTION   UP/DOWN   O BACK");}galnet_chrome();}
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


