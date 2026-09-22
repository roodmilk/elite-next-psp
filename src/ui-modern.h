static int cargo_item(int selection){if(game.docked)return selection;int n=0;for(int i=0;i<GOODS;i++)if(game.cargo[i]>0){if(n++==selection)return i;}return -1;}
static int cargo_rows(void){if(game.docked)return GOODS;int n=0;for(int i=0;i<GOODS;i++)n+=game.cargo[i]>0;return n?n:1;}
static int contact_ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],contact_count;
static void contacts_refresh(void){contact_count=0;for(int i=0;i<=ANOMALY_ID_MAX;i++)if(valid_target(i))contact_ids[contact_count++]=i;}
/* Instrument panel — soft charcoal fill, cream/slate rules. No gold corner brackets (debug look). */
static void panel(int x,int y,int w,int h){rect(x,y,w,h,RGB(21,28,39));rect(x,y,w,1,RGB(229,210,163));rect(x,y+h-1,w,1,RGB(41,54,70));rect(x,y,1,h,RGB(90,96,76));rect(x+w-1,y,1,h,RGB(41,54,70));rect(x+1,y+1,w-2,1,RGB(41,54,70));}
#include "deck-ui.h"
static void market_screen(void){
 header(game.docked?"CARGO / STATION MARKET":"CARGO / INVENTORY");panel(8,32,220,156);panel(236,32,236,156);
 int count=cargo_rows(),first=(row/7)*7,item=cargo_item(row);
 text(3,5,CYAN,game.docked?"COMMODITIES":"YOUR CARGO");page_number_at(21,5,row/7+1,(count+6)/7);text(25,5,CYAN,"HOLD");
 for(int j=0;j<7&&first+j<count;j++){int index=cargo_item(first+j);int y=8+j*2;if(first+j==row)rect(10,y*8-2,216,13,RGB(25,65,77));if(index>=0)text(3,y,WHITE,"%-17s %d%c",goods[index].name,game.cargo[index],goods[index].unit);}
 if(item<0){text(31,8,DIM,"Hold is empty.");text(31,10,WHITE,"Dock to buy cargo.");}
 else {
  int avg=galactic_price(item),local=game.price[item],diff=local-avg;draw_next_art(next_items[item],32,32,430,65,32,32);
  text(31,5,GOLD,"%.22s",goods[item].name);
  text(31,7,WHITE,"%d %c in hold",game.cargo[item],goods[item].unit);if(mission_cargo_reserved(&game,item))text(31,8,GOLD,"%d reserved for jobs",mission_cargo_reserved(&game,item));
  if(game.docked){
   text(31,9,WHITE,"Here %.1f",local*.1f);
   text(31,10,DIM,"Gal  %.1f",avg*.1f);
   text(31,12,diff>0?RED:diff<0?CYAN:WHITE,diff>0?"^ ABOVE AVG":diff<0?"v BELOW AVG":"= GALACTIC AVG");
   text(31,13,diff? (diff>0?RED:CYAN):DIM,diff>0?"Sellers win here.":diff<0?"Buy cheap, sell away.":"Fair local price.");
   text(31,15,DIM,"Stock %d",game.stock[item]);
   if(item==3||item==6||item==10)text(31,17,RED,"Restricted cargo.");
  }else {int wx=252,wy=116;line(wx,wy+12,wx+7,wy,AMBER);line(wx+7,wy,wx+14,wy+12,AMBER);line(wx+14,wy+12,wx,wy+12,AMBER);text(32,15,AMBER,"! DOCK TO BUY OR SELL");}
 }
 int used=cargo_used(&game),cap=cargo_capacity(&game);if(cap<1)cap=1;if(used>cap)used=cap;rect(14,176,210,5,RGB(30,45,60));rect(14,176,210*used/cap,5,CYAN);text(31,22,WHITE,"%.1f U  %d/%d T",game.credits*.1f,cargo_used(&game),cargo_capacity(&game));footer(game.docked?"UP/DOWN   RIGHT BUY   LEFT SELL   O BACK":"UP/DOWN   O BACK");
}
static int target_filter=0,target_ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],target_count=0,target_details=0;
static int target_matches(int id){if(target_filter==0)return 1;if(target_filter==1)return IS_NPC_ID(id);if(target_filter==2)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].target==-2;if(target_filter==3)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==LAW;if(target_filter==4)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==TRADERS;if(target_filter==5)return is_mission_target(&game,id);if(target_filter==6)return id>0&&id<=BODY_COUNT;if(target_filter==7)return id==0;if(target_filter==8)return IS_DEBRIS_ID(id);if(target_filter==9)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].freighter;return IS_ANOMALY_ID(id);}
static void target_refresh(void){target_count=0;for(int id=0;id<=ANOMALY_ID_MAX;id++)if(valid_target(id)&&target_matches(id))target_ids[target_count++]=id;if(row>=target_count)row=0;}
static unsigned contact_color(int id){if(is_mission_target(&game,id))return WHITE;if(IS_NPC_ID(id))return faction_colors[game.npc[id-BODY_COUNT-1].role];if(IS_ANOMALY_ID(id))return GOLD;if(IS_DEBRIS_ID(id))return DIM;return id==0?CYAN:GOLD;}
static const char *target_status(int id){if(is_mission_target(&game,id))return "MISSION TARGET";if(IS_ANOMALY_ID(id))return game.anomaly[id-ANOMALY_ID_MIN].scanned?"CODEX ENTRY":"UNSCANNED ANOMALY";if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].rock?"LASER-MINEABLE ROCK":game.debris[id-DEBRIS_ID_MIN].wreck?"SALVAGE WRECK":"COLLECTABLE CARGO";if(id==0)return "DOCKABLE";if(id<=BODY_COUNT){Body *b=&game.bodies[id-1];return b->type==SUN?"STAR / NO LANDING":b->type==GAS?"NO LANDING":"APPROACHABLE";}NPC *n=&game.npc[id-BODY_COUNT-1];if(n->target==-2)return "HOSTILE";if(n->role==PIRATES)return "WANTED";if(n->role==LAW)return "LAW";if(n->role==TRADERS)return n->freighter?freight_status(n):"TRADER";return "NEUTRAL";}
static void targeting_screen(void){
 target_count=collect_scan_ids(target_ids,scan_cat);if(row>=target_count)row=0;
 header("TARGETING COMPUTER");page_number_at(52,4,target_count?row/7+1:1,target_count?(target_count+6)/7:1);
 for(int i=0;i<5;i++){int col=1+i*11;if(i==scan_cat)rect(col*8-2,30,84,14,RGB(25,65,77));text(col,4,i==scan_cat?GOLD:DIM,"%s",scan_cat_names[i]);}
 panel(8,47,464,140);int first=row/7*7;for(int j=0;j<7&&first+j<target_count;j++){int i=first+j,id=target_ids[i],y=7+j*2;Vec3 p=camera(&game,target_position(id));if(i==row)selected(y);text(3,y,contact_color(id),"%s%-20.20s %6d M %-6s",is_mission_target(&game,id)?"[M] ":"    ",scanner_known(id)?target_name(id):"UNKNOWN CONTACT",(int)length(sub(target_position(id),game.pos)),p.z>=0?"AHEAD":"BEHIND");}if(!target_count)text(3,10,DIM,"Nothing in this band. L/R changes category.");
 if(target_count){int id=target_ids[row];text(3,21,GOLD,"%s",target_status(id));if(target_details){if(IS_NPC_ID(id)){NPC *n=&game.npc[id-BODY_COUNT-1];if(scanner_known(id))text(3,22,WHITE,"%s  hull %d  %d m",faction_names[n->role],(int)n->health,(int)length(sub(n->pos,game.pos)));else text(3,22,WHITE,"Close in, or fit a long-range scanner.");}else if(IS_ANOMALY_ID(id))text(3,22,WHITE,"%s",game.anomaly[id-ANOMALY_ID_MIN].scanned?"Logged in Codex":"Close in. Press O.");else if(IS_DEBRIS_ID(id))text(3,22,WHITE,"%s",game.debris[id-DEBRIS_ID_MIN].rock?"Fire to fracture; Circle collects loose ore.":"Circle: collect within 500 m");else text(3,22,WHITE,"%s",id==0?station_name(&game):game.bodies[id-1].name);}else text(3,22,DIM,"Triangle for details.");}
 footer("L/R CATEGORY   X LOCK   TRI DETAILS   O BACK");}
static void local_system(void){
 contacts_refresh();if(row>=contact_count)row=0;header("SYSTEM / ALL CONTACTS");int first=row/8*8;panel(8,32,464,156);text(3,5,CYAN,"NAME                            DISTANCE");
  for(int j=0;j<8&&first+j<contact_count;j++){int id=contact_ids[first+j],y=7+j*2;if(first+j==row)selected(y);const char *kind=IS_NPC_ID(id)?"SHIP":IS_ANOMALY_ID(id)?"ECHO":IS_DEBRIS_ID(id)?"LOOT":"NAV ";text(3,y,contact_color(id),"%-24s %s %7d M",scanner_known(id)?target_name(id):"UNKNOWN CONTACT",kind,(int)length(sub(target_position(id),game.pos)));}
 page_number_at(52,5,row/8+1,(contact_count+7)/8);footer("UP/DOWN   X LOCK   TRI AUTO-AIM   O BACK");
}
static void debug_screen(void){header("DEBUG");const char *items[]={"Add 1,000 units","Refill fuel and shields","Clear local wanted","Add local wanted","Move to station approach","Move near targeted planet","Return to station","Enter planet atmosphere"};for(int i=0;i<8;i++){if(row==i)selected(5+i*2);text(3,5+i*2,row==i?WHITE:DIM,"%s",items[i]);}footer("UP/DOWN   X APPLY   O BACK");}
static void debug_action(void){
 if(row==0){game.credits+=10000;if(game.credits>100000000)game.credits=100000000;game.cue=SFX_UI;message(&game,"Added 1,000 units.");}
 if(row==1){game.fuel=player_ships[game.ship].range;game.energy=100;message(&game,"Fuel and shields full.");}
 if(row==2){game.legal=game.wanted[game.system]=0;message(&game,"Local warrant cleared.");}
 if(row==3){add_crime(&game,5);message(&game,"Local wanted level raised.");}
 if(row==4){game.docked=0;game.pos=(Vec3){0,0,3000};game.speed=0;game.yaw=game.pitch=game.roll=0;selected_target=0;autoaim=0;change_page(FLIGHT);}
 if(row==5){if(selected_target<2||selected_target>BODY_COUNT){message(&game,"Select a planet in Contacts first.");return;}Body *b=&game.bodies[selected_target-1];game.docked=0;game.pos=add(b->pos,(Vec3){0,0,-b->radius-800});game.speed=0;game.yaw=game.pitch=game.roll=0;autoaim=0;change_page(FLIGHT);}
 if(row==6){game.planet=-1;game.surface=0;game.docked=1;game.speed=0;game.pos=(Vec3){0,0,3200};change_page(HOME);message(&game,"Docked at the local station.");}
 if(row==7){if(selected_target<2||selected_target>BODY_COUNT){message(&game,"Select a planet in Contacts first.");return;}Body *b=&game.bodies[selected_target-1];if(b->type==GAS||b->type==SUN){message(&game,"Gas giants and suns have no atmosphere flight.");return;}game.docked=0;game.pos=add(b->pos,(Vec3){0,0,-b->radius-800});game.yaw=game.pitch=game.roll=0;game.approach=selected_target-1;if(enter_planet(&game)){autoaim=0;change_page(FLIGHT);}else message(&game,"Could not enter atmosphere.");}
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
static void chart_system_preview(int dest){
 int xs[5]={352,394,436,373,415},ys[5]={58,52,58,92,96},rs[5]={16,12,10,14,11};
 for(int i=0;i<BODY_COUNT;i++){unsigned col,acc;int type;body_tint(dest,i,&col,&acc,&type);draw_planet_disc(xs[i],ys[i],rs[i],col,acc,body_art_seed(dest,i),type);}
}
static void galaxy_xy(int id,int *x,int *y){
 int cx=chart_zoom==1?128:game.systems[chart_cursor].x,cy=chart_zoom==1?128:game.systems[chart_cursor].y;
 *x=158+(game.systems[id].x-cx)*chart_zoom*284/256;*y=110+(game.systems[id].y-cy)*chart_zoom*142/256;
}
static int galaxy_route_draw(int goal,unsigned ink){
 static int cached_from=-1,cached_goal=-1,cached_ship=-1,path[65],path_n=0;
 if(cached_from!=game.system||cached_goal!=goal||cached_ship!=game.ship){Game route=game;route.fuel=(float)player_ships[game.ship].range;int from=route.system;path_n=0;path[path_n++]=from;for(int guard=0;guard<64&&from!=goal;guard++){int jumps=0,next=route_next_hop(&route,goal,&jumps);if(next<0){path_n=0;break;}path[path_n++]=next;route.system=next;route.fuel=(float)player_ships[route.ship].range;from=next;}if(!path_n||path[path_n-1]!=goal)path_n=0;cached_from=game.system;cached_goal=goal;cached_ship=game.ship;}
 for(int i=1;i<path_n;i++){int ax,ay,bx,by;galaxy_xy(path[i-1],&ax,&ay);galaxy_xy(path[i],&bx,&by);line(ax,ay,bx,by,ink);}
 return path_n?path_n-1:-1;
}
static void galaxy_overview(void){
 int mission=tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT?game.saga_dest:-1;
 int planned=mission>=0?mission:game.route_goal;
 header("GALAXY MAP / ROUTE PLANNER");panel(8,32,300,156);panel(316,32,156,156);
 clipx0=10;clipx1=307;clipy0=34;clipy1=187;
 int goal=planned>=0?planned:chart_cursor,jumps=galaxy_route_draw(goal,GOLD);
 for(int i=0;i<256;i++){int x,y;galaxy_xy(i,&x,&y);if(x<10||x>306||y<34||y>186)continue;unsigned c=i==game.system?CYAN:i==mission?GOLD:i==game.route_goal?AMBER:(game.visited[i>>3]&(1<<(i&7)))?WHITE:RGB(62,83,96);pixel(x,y,c);if(i==game.system)rect(x-2,y-2,5,5,c);}
 int x,y;galaxy_xy(chart_cursor,&x,&y);line(x-4,y-4,x+4,y-4,WHITE);line(x-4,y+4,x+4,y+4,WHITE);line(x-4,y-4,x-4,y+4,WHITE);line(x+4,y-4,x+4,y+4,WHITE);
 clipy0=-1;
 text(40,5,CYAN,"CURSOR");text(40,7,WHITE,"%.17s",game.systems[chart_cursor].name);text(40,9,DIM,"%.1f LY away",distance_ly(&game,game.system,chart_cursor));
 text(40,11,CYAN,mission>=0?"TRACKED DESTINATION":"ROUTE GOAL");text(40,13,planned>=0?(mission>=0?GOLD:AMBER):DIM,"%.17s",planned>=0?game.systems[planned].name:"None");
 text(40,15,CYAN,"ROUTE");text(40,17,jumps>=0?WHITE:RED,jumps>=0?"%d jump%s":"No drive route",jumps,jumps==1?"":"s");text(40,19,DIM,"Zoom %dx / 4x",chart_zoom);text(40,21,DIM,"Cyan: here");text(40,22,mission>=0?GOLD:AMBER,mission>=0?"Gold: mission route":"Amber: saved route");
 footer("D-PAD MOVE   L/R ZOOM   X PLOT   TRI NEARBY");
}
static void chart(void){
 if(chart_mode){galaxy_overview();return;}
 header("GALAXY MAP / JUMP");panel(8,32,300,156);panel(316,32,156,156);
 text(3,5,CYAN,"DESTINATION        RANGE  RISK");
 int first=row/8*8;for(int j=0;j<8&&first+j<near_count;j++){int i=first+j,id=nearby[i],y=7+j*2;float d=distance_ly(&game,game.system,id);if(i==row)selected_span(y,284);int seen=game.visited[id>>3]&(1<<(id&7));text(3,y,d*10<=game.fuel?WHITE:DIM,"%s%-12s %4.1f  %s",seen?"* ":"  ",game.systems[id].name,d,d*10<=game.fuel?"RDY":"LOW");}
 int dest=near_count?nearby[row]:game.system;float d=distance_ly(&game,game.system,dest),tank=game.fuel*.1f,need=d;
 chart_system_preview(dest);
 if(tracked_mission==0&&game.campaign_stage>=6&&game.saga_step&&game.saga_chapter<SAGA_COUNT&&game.system!=game.saga_dest)text(41,14,DIM,"FINAL: %.12s",game.systems[game.saga_dest].name);
 else if(game.route_goal>=0&&game.system!=game.route_goal)text(41,14,DIM,"FINAL: %.12s",game.systems[game.route_goal].name);
 text(41,16,CYAN,"%.12s",game.systems[dest].name);
 text(41,17,GOLD,"%.12s",race_name(dest));
 text(41,19,WHITE,"Tank %.1f LY",tank);
 text(41,20,need*10<=game.fuel?CYAN:RED,"Jump %.1f LY",need);
 text(41,21,DIM,need*10<=game.fuel?"X spends that fuel":"Refuel or scoop");
 page_number_at(37,5,row/8+1,(near_count+7)/8);footer(game.docked?"UP/DOWN X JUMP   TRI GALAXY   O BACK":"UP/DOWN X JUMP   TRI GALAXY   O BACK");
}
static int codex_tab=0;
static const char *codex_tabs[]={"SYSTEMS","PLANETS","FLORA","FAUNA","MINERALS","ECHOES"};
static int vis_count(void){int n=1;for(int s=0;s<256;s++)if(s!=game.system&&(game.visited[s>>3]&(1<<(s&7))))n++;return n;}
static int vis_sys(int idx){if(idx<=0)return game.system;int n=1;for(int s=0;s<256;s++)if(s!=game.system&&(game.visited[s>>3]&(1<<(s&7)))){if(n==idx)return s;n++;}return game.system;}
/* Visiting a system discovers its four worlds (I–IV). Count is systems × 4. */
static int planet_log_count(void){int n=systems_visited(&game);return n>0?n*4:4;}
static void planet_log_at(int idx,int *sys_out,int *body_out){
 int n=vis_count(),si=idx/4,bi=(idx%4)+1;if(si<0)si=0;if(si>=n)si=n-1;if(bi<1)bi=1;if(bi>4)bi=4;
 *sys_out=vis_sys(si);*body_out=bi;
}
static int codex_kind_count(int tab){if(tab==0)return vis_count();if(tab==1)return planet_log_count();if(tab==2)return game.scanned_flora;if(tab==3)return game.scanned_fauna;if(tab==4)return game.scanned_minerals;return game.scanned_anomalies;}
static int codex_rows(void){int n=codex_kind_count(codex_tab);return n>0?n:1;}
static void codex_life_label(int tab,int i,char *name,int nn,char *where,int wn){
 const char *flora[]={"GLOW VINE","GLASS FERN","SPORE TREE","NIGHT MOSS","KELP FAN","IRON MOSS"};
 const char *fauna[]={"GLASS MOTH","DUST RUNNER","SKY RAY","BURROWER","SAND HOPPER","DRIFT EEL"};
 const char *ore[]={"RED ORE","ICE CRYSTAL","BASALT VEIN","SILICA","NICKEL SEAM","CARBON LACE"};
 const char *echo[]={"MERIDIAN ECHO","STELLAR RIFT","GHOST PING","QUASAR HUM"};
 const char **src=tab==2?flora:tab==3?fauna:tab==4?ore:echo;int names=tab==5?4:6;
 snprintf(name,nn,"%s",src[i%names]);
 int sys=vis_sys(i%vis_count());const char *rom[]={"I","II","IV"};
 snprintf(where,wn,"%s %s",game.systems[sys].name,rom[i%3]);
}
static void codex_screen(void){
 int count=codex_rows(),first=row/7*7;header("DISCOVERY CODEX");page_number_at(48,4,row/7+1,(count+6)/7);
 text(2,4,CYAN,"< L  %s  R >",codex_tabs[codex_tab]);
 panel(8,47,232,140);panel(248,47,224,140);
 int n=codex_kind_count(codex_tab);
 if(codex_tab==0){
  for(int j=0;j<7&&first+j<count;j++){int i=first+j,y=7+j*2,sys=vis_sys(i);if(i==row)rect(10,y*8-2,220,13,RGB(25,65,77));text(3,y,i==row?WHITE:DIM,"%s%-12s",sys==game.system?"* ":"  ",game.systems[sys].name);}
  int sys=vis_sys(row);unsigned col,acc;int type;
  for(int b=0;b<BODY_COUNT;b++){body_tint(sys,b,&col,&acc,&type);draw_planet_disc(270+b*38,78,b==0?16:12,col,acc,body_art_seed(sys,b),type);}
  text(32,14,CYAN,"%.22s",game.systems[sys].name);
  text(32,16,WHITE,"Sun + 4 worlds");
  if(sys==game.system)text(32,18,GOLD,"Here: flora %d fauna %d",game.scanned_flora,game.scanned_fauna);
  else text(32,18,DIM,"Visited system");
 }else if(codex_tab==1){
  for(int j=0;j<7&&first+j<count;j++){
   int i=first+j,y=7+j*2,sys,body;planet_log_at(i,&sys,&body);const char *rom[]={"","I","II","III","IV"};
   if(i==row)rect(10,y*8-2,220,13,RGB(25,65,77));
   text(3,y,i==row?WHITE:DIM,"%.10s %s",game.systems[sys].name,rom[body]);
  }
  int sys,body;planet_log_at(row,&sys,&body);unsigned col,acc;int type;body_tint(sys,body,&col,&acc,&type);
  draw_planet_disc(318,86,34,col,acc,body_art_seed(sys,body),type);
  const char *rom[]={"","I","II","III","IV"};
  text(32,16,CYAN,"%.12s %s",game.systems[sys].name,rom[body]);
  {const char *kindname[]={"STAR","OCEAN","ROCKY","GAS"};text(32,18,WHITE,"%s world",kindname[type>=0&&type<=GAS?type:ROCKY]);}
  text(32,20,DIM,sys==game.system?"In this system":"Discovered on visit");
 }else if(n<=0){text(3,10,DIM,"Nothing logged yet.");text(32,10,WHITE,"Scan on foot. Square.");}
 else {
  for(int j=0;j<7&&first+j<count;j++){int i=first+j,y=7+j*2;char name[24],where[24];codex_life_label(codex_tab,i,name,sizeof(name),where,sizeof(where));if(i==row)rect(10,y*8-2,220,13,RGB(25,65,77));text(3,y,i==row?WHITE:DIM,"%.22s",name);}
  char name[24],where[24];codex_life_label(codex_tab,row,name,sizeof(name),where,sizeof(where));
  if(codex_tab==2)draw_flora_icon(270,60,row+1);else if(codex_tab==3)draw_fauna_icon(270,60,row+1);else if(codex_tab==4)draw_mineral_icon(270,60,row+1);else draw_anomaly_icon(268,58,row&1);
  text(32,14,GOLD,"%.22s",name);
  text(32,16,WHITE,"%.22s",where);
  text(32,18,DIM,codex_tab==5?"O on an echo in space.":"Square on foot to log.");
 }
 text(32,21,CYAN,"Logged %d",game.discoveries);
 footer("L/R TYPE   UP/DOWN   O BACK");
}
static void yard(void){
 header("SHIPYARD / EXCHANGE");if(!game.docked){text(3,8,DIM,"Dock to view ships for sale.");footer("O BACK");return;}
 panel(8,32,218,156);panel(234,32,238,156);
 for(int i=0;i<player_ship_count;i++){int y=5+i*2;if(i==row)rect(10,y*8-2,214,14,RGB(25,65,77));text(3,y,i==row?WHITE:DIM,"%-12.12s%s",player_ships[i].name,i==game.ship?" [OWNED]":"");}
 const PlayerShip *p=&player_ships[row];
 preview_clip(353,84,242,40,464,128);
 fitted_ship_preview(mesh_id(p->name),preview_time*.5f,preview_time*.22f);
 flush_meshes();preview_reset();
 text(31,17,CYAN,"%.22s",p->name);
 text(31,18,WHITE,"Hold %d t  Spd %d",p->capacity,p->speed);
 text(31,19,WHITE,"Range %.1f LY",p->range*.1f);
 text(31,21,GOLD,"%.1f units",(p->price-player_ships[game.ship].price*3/4)*.1f);
 footer("UP/DOWN   X EXCHANGE   O BACK");
}
/* Expanded outfitting: only list items this hub actually stocks. */
enum { EQUIP_COUNT = 24 };
static const char *equipment_names[EQUIP_COUNT]={
 "REFUEL TANK","PULSE LASER","BEAM LASER","MISSILE RESTOCK","DOCKING COMPUTER","NAV BEACON",
 "SHIELD BOOSTER","MILITARY SHIELD","LASER COOLING","HEAT SINK","CARGO BAY +8T","FREIGHT RACK +16T",
 "LONG-RANGE SCANNER","PLANET SCANNER","FUEL SCOOP","AGRI SCOOP","ECM SUITE","CHAFF DISPENSER",
 "ESCAPE POD","AUTO-REPAIR KIT","MINING LASER","REFINERY UNIT","PASSENGER CABIN","EXCLUSIVE CLAMP"
};
static const char *equipment_list_names[EQUIP_COUNT]={
 "REFUEL","PULSE LASER","BEAM LASER","MISSILE +1","DOCK COMP","NAV BEACON",
 "SHIELD BOOST","MIL SHIELD","LASER COOL","HEAT SINK","CARGO +8T","FREIGHT +16T",
 "LONG SCAN","PLANET SCAN","FUEL SCOOP","AGRI SCOOP","ECM SUITE","CHAFF",
 "ESCAPE POD","AUTO-REPAIR","MINING LASER","REFINERY","PAX CABIN","EXCL CLAMP"
};
static const char *equipment_details[EQUIP_COUNT]={
 "Fill hyperspace tank.","Solid starter pulse.","Twice laser damage.","Load one missile.","Dock from 8,000 m.","Clearer next-hop marks.",
 "Twice shield recharge.","Even faster shields.","Laser cools faster.","Dump heat in a hurry.","Adds eight tonnes.","Adds sixteen tonnes.",
 "IDs distant contacts.","Surface scan assist.","Skim fuel near a sun.","Scoop near agri belts.","Break missile locks.","Decoy flare burst.",
 "One free emergency tow.","Slow hull patching.","Faster rock mining.","Ore→alloys onboard.","+1 passenger berth.","Chandler deck clamp."
};
static const char *equipment_effects[EQUIP_COUNT]={
 "Tank: now -> ship max","Laser fitted","Laser 18 -> 36","Missiles +1","Dock 2500 -> 8000 m","Chart clarity+",
 "Shield 1.5 -> 3.0 /s","Shield 3.0 -> 4.5 /s","Cool 22 -> 38 /s","Heat dump ready","Hold +8 tonnes","Hold +16 tonnes",
 "Scan 3 -> 6 fields","Planet scan boost","Fuel +0.5 /s at sun","Scoop near agri","ECM online","Chaff ready",
 "Rescue once free","Repair tick","Mine faster","Refine minerals","Passenger OK","Cargo clamp+"
};
static const int equipment_costs[EQUIP_COUNT]={0,2200,4000,1000,2500,1800,6000,9000,4500,3200,3500,7000,3000,2800,7500,5000,5500,2000,4000,3600,4200,4800,2500,1500};
/* Minimum displayed tech (systems[].tech+1). 0 = always if economy allows. */
static const int equipment_tech[EQUIP_COUNT]={0,2,4,2,5,3,6,8,5,4,3,6,4,3,7,4,6,3,4,5,4,5,3,2};
/* Economy bands that stock the item: bit0 poor ind … bit7 poor agri. 0xff = all. */
static const unsigned equipment_econ[EQUIP_COUNT]={
 0xff,0xff,0x0f,0xff,0xff,0xf0,0x1f,0x07,0x0f,0x1f,0xff,0x0e,0xff,0xf0,0x0f,0xf0,0x0f,0xff,0xff,0x1f,0x0e,0x0e,0xff,0x00
};
static const char *equip_cat_name(int i){
 static const char *c[]={"FUEL","WPN","WPN","WPN","NAV","NAV","DEF","DEF","DEF","DEF","HOLD","HOLD","NAV","NAV","FUEL","FUEL","DEF","DEF","UTIL","UTIL","UTIL","UTIL","HOLD","HOLD"};
 return i>=0&&i<EQUIP_COUNT?c[i]:"UTIL";
}
static int equipment_owned(int i){
 if(i<=0)return game.fuel>=player_ships[game.ship].range;
 if(i==1||i==2)return game.laser;
 if(i==3)return game.missiles>=4;
 if(i==4)return (game.upgrades&1)!=0;      /* dock */
 if(i==5)return (game.upgrades&1024)!=0;   /* nav beacon */
 if(i==6)return (game.upgrades&2)!=0&&!(game.upgrades&128);
 if(i==7)return (game.upgrades&128)!=0;    /* mil shield */
 if(i==8)return (game.upgrades&4)!=0;      /* cooling */
 if(i==9)return (game.upgrades&2048)!=0;   /* heat sink */
 if(i==10)return (game.upgrades&8)!=0&&!(game.upgrades&64);
 if(i==11)return (game.upgrades&64)!=0;    /* +16 */
 if(i==12)return (game.upgrades&16)!=0;    /* long scan */
 if(i==13)return (game.upgrades&4096)!=0;
 if(i==14)return (game.upgrades&32)!=0&&!(game.upgrades&8192);
 if(i==15)return (game.upgrades&8192)!=0;
 if(i==16||i==17)return (game.upgrades&256)!=0;
 if(i==18)return (game.upgrades&16384)!=0;
 if(i==19)return (game.upgrades&32768)!=0;
 if(i==20)return (game.upgrades&65536)!=0;
 if(i==21)return (game.upgrades&131072)!=0;
 if(i==22)return (game.upgrades&512)!=0;   /* pax cabin */
 if(i==23)return (game.upgrades&8)!=0;
 return 0;
}
static int equipment_in_stock(int i){
 if(i<0||i>=EQUIP_COUNT)return 0;
 if(i==0)return 1;
 if(equipment_econ[i]==0)return 0; /* exclusive — chandler only */
 int have=game.systems[game.system].tech+1;
 if(have<equipment_tech[i])return 0;
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
static void buy_equipment(int i){
 if(!game.docked){message(&game,"Dock to buy equipment.");return;}
 if(i<0||i>=EQUIP_COUNT)return;
 if(i==0){int cost=(int)ceilf(player_ships[game.ship].range-game.fuel)*2;if(cost<=0){message(&game,"Tank is already full.");return;}if(game.credits<cost){message(&game,"Not enough units.");return;}game.credits-=cost;game.fuel=player_ships[game.ship].range;game.cue=SFX_UI;message(&game,"Tank full.");return;}
 if(equipment_owned(i)){message(&game,i==3?"Missile rack full.":"Already installed.");return;}
 if(!equipment_in_stock(i)&&equipment_econ[i]!=0){message(&game,"Not stocked at this hub.");return;}
 if(game.credits<equipment_costs[i]){message(&game,"Not enough units.");return;}
 game.credits-=equipment_costs[i];game.cue=SFX_UI;
 if(i==1||i==2){game.laser=1;message(&game,"Laser fitted.");return;}
 if(i==3){game.missiles++;message(&game,"Missile loaded.");return;}
 if(i==4)game.upgrades|=1;
 else if(i==5)game.upgrades|=1024;
 else if(i==6)game.upgrades|=2;
 else if(i==7)game.upgrades|=2|128;
 else if(i==8)game.upgrades|=4;
 else if(i==9)game.upgrades|=2048;
 else if(i==10)game.upgrades|=8;
 else if(i==11)game.upgrades|=8|64;
 else if(i==12)game.upgrades|=16;
 else if(i==13)game.upgrades|=4096;
 else if(i==14)game.upgrades|=32;
 else if(i==15)game.upgrades|=32|8192;
 else if(i==16||i==17)game.upgrades|=256;
 else if(i==18)game.upgrades|=16384;
 else if(i==19)game.upgrades|=32768;
 else if(i==20)game.upgrades|=65536;
 else if(i==21)game.upgrades|=131072;
 else if(i==22)game.upgrades|=512;
 else if(i==23)game.upgrades|=8;
 message(&game,"Upgrade installed.");
}
static int equip_row_count(void){int list[EQUIP_COUNT];return equipment_stock_list(list,EQUIP_COUNT);}
static void equipment(void){
 header("OUTFITTING");if(!game.docked){text(3,8,DIM,"Dock to view equipment and fuel.");footer("O BACK");return;}
 panel(8,32,225,156);panel(241,32,231,156);
 int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);
 if(n<1){text(2,8,DIM,"No stock today.");footer("O BACK");return;}
 if(row<0)row=0; if(row>=n)row=n-1;
 int first=row/6*6,fuelcost=(int)ceilf(player_ships[game.ship].range-game.fuel)*2;
 text(2,5,CYAN,"IN STOCK");page_number_at(18,5,row/6+1,(n+5)/6);
 for(int j=0;j<6&&first+j<n;j++){int disp=first+j,i=list[disp],y=7+j*3;if(disp==row)rect(10,y*8-3,220,15,RGB(25,65,77));text(2,y,equipment_owned(i)?CYAN:disp==row?WHITE:DIM,"%-4s %-14s%s",equip_cat_name(i),equipment_list_names[i],equipment_owned(i)?" *":"");}
 int i=list[row];
 text(31,5,GOLD,"%.22s",equipment_names[i]);
 text(31,7,CYAN,"%.22s",equip_cat_name(i));
 text(31,9,WHITE,"%.28s",equipment_details[i]);
 text(31,12,CYAN,"%.28s",equipment_effects[i]);
 if(equipment_owned(i))text(31,15,CYAN,i==3?"Missile rack full":i==0?"Fuel tank full":"Already fitted");
 else text(31,15,CYAN,"In stock at this hub");
 text(31,17,DIM,"Hub tech %d",game.systems[game.system].tech+1);
 text(31,19,equipment_owned(i)?DIM:WHITE,equipment_owned(i)?"--":"%.1f units",i==0?fuelcost*.1f:equipment_costs[i]*.1f);
 text(31,21,DIM,"Balance %.1f",game.credits*.1f);
 footer("UP/DOWN   X BUY / REFUEL   O BACK");
}
/* Ship loadout / inventory slots — what is fitted right now. */
static void inventory_screen(void){
 header("SHIP LOADOUT / HOLD");panel(8,32,232,180);panel(248,32,224,180);
 text(2,5,CYAN,"EQUIP SLOTS");
 const char *slot[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
 const char *fit[6];
 fit[0]=game.laser?"BEAM/PULSE LASER":"NONE";
 fit[1]=(game.upgrades&128)?"MILITARY SHIELD":(game.upgrades&2)?"SHIELD BOOST":"NONE";
 fit[2]=(game.upgrades&16)?"LONG SCANNER":(game.upgrades&1)?"DOCK COMPUTER":"NONE";
 fit[3]=(game.upgrades&64)?"FREIGHT +16T":(game.upgrades&8)?"CARGO +8T":"BASE HOLD";
 fit[4]=(game.upgrades&32)?"FUEL SCOOP":"TANK ONLY";
 fit[5]=(game.upgrades&512)?"PAX CABIN":(game.upgrades&256)?"ECM/CHAFF":"NONE";
 for(int i=0;i<6;i++){int y=7+i*2;if(i==row)selected(y);text(2,y,i==row?GOLD:WHITE,"%-4s %.18s",slot[i],fit[i]);}
 text(2,20,CYAN,"MISSILES %d",game.missiles);
 text(2,22,WHITE,"HOLD %d / %d T",cargo_used(&game),cargo_capacity(&game));
 text(32,5,CYAN,"CARGO MANIFEST");
 int line=7; for(int g=0;g<GOODS&&line<20;g++)if(game.cargo[g]>0){text(32,line,WHITE,"%-12.12s %d%c",goods[g].name,game.cargo[g],goods[g].unit);line++;}
 if(game.passenger_dest>=0){text(32,line,GOLD,"PASSENGER");line++;text(32,line,CYAN,"-> %.12s",game.systems[game.passenger_dest].name);}
 if(line==7)text(32,7,DIM,"Hold empty.");
 footer("UP/DOWN   O BACK");
}

static void status(void){header("COMMANDER");panel(8,32,464,156);draw_portrait(14,40,92,78,game.system,EXPLORERS);draw_world_card(118,40,92,78,&game.bodies[1],game.bodies[1].seed);text(29,5,CYAN,"%.18s",player_ships[game.ship].name);text(29,8,WHITE,"%.1f units  Kills %d  Ms %d",game.credits*.1f,game.kills,game.missiles);text(29,11,game.legal?RED:CYAN,"Wanted [%s] %.10s",stars(wanted_level(&game)),game.systems[game.system].name);text(29,13,GOLD,"SYS %d  ENG %d  WEP %d",game.pip_sys,game.pip_eng,game.pip_wep);text(3,17,WHITE,"Codex %d    Charted %d",game.discoveries,systems_visited(&game));if(game.job_n>0)text(3,19,GOLD,"Jobs %d/5  %s -> %s",game.job_n,mission_name(game.jobs[game.job_sel].type),game.systems[game.jobs[game.job_sel].dest].name);else text(3,19,DIM,"No active missions.");text(3,21,WHITE,game.docked?"X save     Triangle load":"Dock to save or load.");footer("O BACK");}
static void communications(void){
 header("COMMS / STATION CHANNEL");panel(8,32,232,156);panel(248,32,224,156);
 draw_station_badge(18,48);draw_portrait(70,48,70,62,VOICE_VENN*37,TRADERS);
 float distance=length(sub(game.pos,(Vec3){0,0,3500})),range=(game.upgrades&1)?8000:2500;
 int cheap=0,dear=0,traders=0,cops=0,raiders=0;
 for(int i=1;i<GOODS;i++){if(game.price[i]-galactic_price(i)<game.price[cheap]-galactic_price(cheap))cheap=i;if(game.price[i]-galactic_price(i)>game.price[dear]-galactic_price(dear))dear=i;}
 for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive){if(game.npc[i].role==TRADERS)traders++;else if(game.npc[i].role==LAW)cops++;else if(game.npc[i].role==PIRATES)raiders++;}
 text(2,15,CYAN,"%.22s",station_name(&game));
 if(comms_rescue_confirm)text(2,17,GOLD,"Fee: up to 50 units");else text(2,17,WHITE,"%d m / %d m",(int)distance,(int)range);
 text(2,19,GOLD,comms_rescue_confirm?"X confirm recovery":game.docked?"Already docked.":distance<=range?"X request docking":"Close in to hail.");
 text(32,5,CYAN,"DOCKMASTER VENN");
 text(32,7,WHITE,game.docked?"Buy  %.12s":"Dock to view the market.",goods[cheap].name);
 if(game.docked)text(32,8,CYAN,"v cheap vs galaxy");
 if(game.docked)text(32,10,WHITE,"Sell %.12s",goods[dear].name);
 if(game.docked)text(32,11,RED,"^ dear vs galaxy");
 text(32,13,WHITE,"Traffic T%d L%d P%d",traders,cops,raiders);
 text(32,15,game.legal?RED:CYAN,"Warrant [%s]",stars(wanted_level(&game)));
 text(32,17,GOLD,"Board jobs %d",mission_count(&game));
 if(valid_target(selected_target)&&selected_target>0)text(32,19,WHITE,"Lock %.14s",target_name(selected_target));
 else text(32,19,DIM,"No ship locked.");
 footer("X DOCK   TRI RESCUE   SQUARE RADIO   O BACK");
}
static void system_details(void){
 header("SYSTEM / BODIES");const char *economies[]={"Rich industrial","Average industrial","Poor industrial","Mainly industrial","Mainly agricultural","Rich agricultural","Average agricultural","Poor agricultural"};
 const char *kinds[]={"STAR","OCEAN WORLD","ROCKY WORLD","GAS GIANT","ROCKY WORLD"};
 panel(8,32,232,156);panel(248,32,224,156);
 for(int i=0;i<1+BODY_COUNT;i++){int y=5+i*2;if(i==row)rect(10,y*8-2,220,14,RGB(25,65,77));if(i==0)text(2,y,i==row?GOLD:WHITE,"HUB  %.16s",station_name(&game));else text(2,y,i==row?GOLD:WHITE,"%-4s %.16s",i==1?"SUN":i==2?"I":i==3?"II":i==4?"III":"IV",game.bodies[i-1].name);}
 if(row==0){int traders=0,law=0,pirates=0,explorers=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive){if(game.npc[i].role==TRADERS)traders++;else if(game.npc[i].role==LAW)law++;else if(game.npc[i].role==PIRATES)pirates++;else explorers++;}draw_station_badge(270,52);text(32,12,CYAN,"%.22s",station_name(&game));text(32,14,WHITE,"Docking hub / live traffic");text(32,16,CYAN,"T%d  L%d  P%d  E%d",traders,law,pirates,explorers);text(32,18,danger_rating(&game,game.system)>=4?RED:AMBER,"Danger %d/5  Prosperity %d/5",danger_rating(&game,game.system),prosperity(&game,game.system));text(32,20,DIM,"%.40s",travellers_brief_line(&game));}
 else {Body *b=&game.bodies[row-1];draw_planet_disc(360,78,28,b->color,b->accent,b->seed,b->type);text(32,14,CYAN,"%.22s",b->name);text(32,16,WHITE,"%.22s",kinds[row-1]);text(32,18,DIM,b->type==SUN||b->type==GAS?"No landing":"Approachable");}
 char lore[48];lore_line(game.system,0,lore,sizeof(lore));text(32,row==0?22:20,WHITE,"%.22s",lore);
 text(2,22,DIM,"%.22s  T%d",economies[game.systems[game.system].economy],game.systems[game.system].tech+1);
 footer("UP/DOWN   X LOCK   TRI ALIGN   O BACK");
}
#include "gazette-lore.h"
static void factions(void){
 header("FACTIONS");panel(8,32,232,156);panel(248,32,224,156);
 int rot=(game.system*3+game.discoveries)&3;
 for(int i=0;i<FACTION_COUNT;i++){int count=0;for(int j=0;j<NPC_COUNT;j++)if(game.npc[j].alive&&game.npc[j].role==i)count++;int y=5+i*4;if(i==row)rect(10,y*8-2,220,28,RGB(25,65,77));draw_portrait(14,y*8-1,28,24,game.system+i*17,i);text(7,y,faction_colors[i],"%.9s",faction_names[i]);text(17,y,i==row?GOLD:WHITE,"SHIPS HERE: %d",count);text(7,y+2,i==row?WHITE:DIM,"%s",faction_lore_tag(i));}
 draw_portrait(270,48,86,72,game.system+row*17,row);
 text(32,16,faction_colors[row],"%.22s",faction_names[row]);
 text(32,18,WHITE,"%s",faction_lore_line(row,0,rot));
 {const char *channel=saga_faction_channel(&game,row,faction_lore_card);text(32,20,WHITE,"%.40s",channel?channel:faction_lore_line(row,1,rot^2));}
 text(32,22,DIM,faction_lore_card?"CHANNEL %d/2":"OPS  X for channel",faction_lore_card);
 footer("UP/DOWN   X LORE   TRI SCAN   O BACK");
}
static void mission_board(void){header("MISSION BOARD");panel(8,32,464,156);if(!game.docked){text(3,8,WHITE,"Dock to take work.");text(3,10,DIM,"You can hold five jobs.");footer("O BACK");return;}int count=mission_count(&game);text(2,5,game.job_n>=MISSION_SLOTS?RED:GOLD,"Log %d/5%s",game.job_n,game.job_n>=MISSION_SLOTS?"  full":"   X accept  |  10 units  |  5:00");text(5,7,CYAN,"%-16s %-8s %s","JOB","DEST","REWARD");for(int i=0;i<count&&i<6;i++){int id=mission_destination(&game,i),type=mission_type_for_offer(&game,i),y=9+i*2,active=mission_offer_active(&game,i),risk=mission_risk(&game,i);if(i==row)selected(y);draw_icon(12,y*8-1,12+type,i==row);text(5,y,active?GOLD:risk>=4?RED:WHITE,"%-16.16s %-8.8s %6.1f",mission_name(type),game.systems[id].name,mission_reward(&game,i)*.1f);text(38,y,risk>=4?RED:AMBER,"%d/5",risk);if(active)text(43,y,GOLD,"LIVE");}if(count){text_wrap(3,21,40,1,DIM,mission_brief(&game,row<count?row:0),0);text(34,23,mission_risk(&game,row<count?row:0)>=4?RED:CYAN,"RISK %d/5",mission_risk(&game,row<count?row:0));}footer(game.job_n>=MISSION_SLOTS?"LOG FULL   SELECT > LOG   O BACK":"UP/DOWN   X ACCEPT   SELECT > LOG");}
static void mission_log(void){header("MISSION LOG / CHOOSE TRACKED");panel(8,32,464,156);text(2,5,CYAN,"  MISSION                         STATUS");int total=2+game.job_n;for(int i=0;i<total;i++){int y=7+i*2;if(i==row)selected(y);unsigned ink=i==tracked_mission?GOLD:i==row?WHITE:DIM;if(i==0)text(2,y,ink,"%s MAIN STORY / KEI + RYN        %s",i==tracked_mission?"*":" ",game.saga_chapter>=SAGA_COUNT?"DONE":"ACTIVE");else if(i==1)text(2,y,ink,"%s EXPLORERS GUILD ASSIGNMENTS   %s",i==tracked_mission?"*":" ",game.guild_chapter>=4?"DONE":"OPTIONAL");else {Job *j=&game.jobs[i-2];text(2,y,ink,"%s %-18.18s -> %-8.8s %3.0fs",i==tracked_mission?"*":" ",mission_name(j->type),game.systems[j->dest].name,j->time);}}rect(8,165,464,23,RGB(15,31,39));const char *objective=row==0?(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT?saga_beats[game.saga_chapter].objective:campaign_task(&game)):row==1?guild_objective(&game):mission_objective_at(&game,row-2);if(abandon_confirm)text(2,21,RED,"WARNING: ABANDON? X CONFIRM / O CANCEL");else{text(2,21,GOLD,"NEXT:");text_wrap(8,21,48,1,GOLD,objective,0);}footer("X TRACK   SELECT NEXT STEP   TRI ABANDON   O BACK");}
static void navigate_job(int index){if(index<0||index>=game.job_n)return;Job *j=&game.jobs[index];game.job_sel=index;route_set_goal(&game,j->dest);game.destination=j->dest;if(game.system!=j->dest){int jumps=0,hop=route_next_hop(&game,j->dest,&jumps);if(hop>=0){game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),"Route: %d jump%s. Refuel at intermediate hubs.",jumps,jumps==1?"":"s");message(&game,note);}else message(&game,"No fuel-safe route. Refuel at the hub first.");}else {route_clear(&game);int id=mission_target_id(&game,index);if(id>=0&&valid_target(id)){selected_target=id;scan_cat=target_category(id);autoaim=1;message(&game,"Mission target locked. Auto-align active.");if(!game.docked)change_page(FLIGHT);}else if(game.docked)message(&game,"Launch to continue this objective in the system.");else message(&game,"Objective unavailable. Re-enter this system.");}}
static int galnet_tab=0;
static int galnet_rows(void){return galnet_tab==3?7:5;}
static int active_role(int role){int n=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==role)n++;return n;}
static void galnet_post(int i,char *author,int alen,char *body,int blen){
 int risk=danger_rating(&game,game.system),wealth=prosperity(&game,game.system),item=(game.system*3+i*5)%GOODS;
 if(galnet_tab==0){
  if(gazette_wants_tabloid(game.system,i)){
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
 else if(galnet_tab==1){snprintf(author,alen,"MARKET TIP / %s",goods[item].name);if(i==0)snprintf(body,blen,"Local stock strength: %d/5. Dock for live prices.",wealth);else if(i==1)snprintf(body,blen,"Industrial hubs favour machinery and computers.");else if(i==2)snprintf(body,blen,"Agricultural worlds often export food cheaply.");else if(i==3)snprintf(body,blen,"Restricted goods can raise your local wanted level.");else snprintf(body,blen,"Cargo space: %d/%d tonnes used.",cargo_used(&game),cargo_capacity(&game));}
 else if(galnet_tab==2){const char *a[]={"BOUNTY DESK","LOCAL LAW","PILOT WARNING","PATROL WATCH","SECURITY FEED"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"Pirate contacts active: %d. Standard bounty 15.0.",active_role(PIRATES));else if(i==1)snprintf(body,blen,"Your local wanted record: [%s].",stars(wanted_level(&game)));else if(i==2)snprintf(body,blen,risk>=4?"Travel in groups. Hostile activity is elevated.":"No major raid warning at this time.");else if(i==3)snprintf(body,blen,"Law patrols active: %d.",active_role(LAW));else snprintf(body,blen,"Wanted records remain inside the offending system.");}
 else if(galnet_tab==3){const char *a[]={"Mira / Trader","Marshal Iona Renn","Dockhand_77","Kei / Explorer","Freighter Crew","DefinitelyNotAPirate","Lave Spotters"};snprintf(author,alen,"%s",a[i]);if(i==0){const char *mira=saga_galnet_mira(&game);if(mira)snprintf(body,blen,"%s",mira);else snprintf(body,blen,"%s market looks %s today.",game.systems[game.system].name,wealth>=4?"well stocked":"a little thin");}else if(i==1){const char *iona=saga_galnet_iona(&game);if(iona)snprintf(body,blen,"%s",iona);else snprintf(body,blen,risk>=4?"Pirate sightings up. Keep scanners active.":"Patrol lanes are calm. Fly safely, commanders.");}else if(i==2)snprintf(body,blen,"Stop boosting near %s, you maniacs.",station_name(&game));else if(i==3){const char *kei=saga_galnet_kei(&game);if(kei)snprintf(body,blen,"%s",kei);else snprintf(body,blen,game.story<STORY_FREE?"Ryn's last ping is still on this wire.":"The colour of %s is unreal from orbit.",game.bodies[1].name);}else if(i==4){const char *fr=saga_galnet_freighter(&game);if(fr)snprintf(body,blen,"%s",fr);else snprintf(body,blen,"Slow convoy crossing the system. Give us room.");}else if(i==5){const char *sable=saga_galnet_sable(&game);if(sable)snprintf(body,blen,"%s",sable);else snprintf(body,blen,"Free cargo inspection behind the gas giant. Honest.");}else if(game.mission_result)snprintf(body,blen,"Commander mission report: %s %s.",mission_name(game.last_mission_type),game.mission_result>0?"complete":"expired");else snprintf(body,blen,"%s",travellers_galnet_spotter(&game));}
 else if(galnet_tab==5){snprintf(author,alen,"MISSION NETWORK");if(i==0&&game.job_n>0)snprintf(body,blen,"Log %d/5. Focus %s to %s; %.0fs.",game.job_n,mission_name(game.jobs[game.job_sel].type),game.systems[game.jobs[game.job_sel].dest].name,game.jobs[game.job_sel].time);else if(i==0&&game.mission_result)snprintf(body,blen,"Last job: %s %s at %s.",mission_name(game.last_mission_type),game.mission_result>0?"complete":"expired",game.systems[game.last_mission_system].name);else if(i==0)snprintf(body,blen,"No active missions. %d local offers available.",mission_count(&game));else if(i==1){const char *net=saga_galnet_network(&game);if(net)snprintf(body,blen,"%s",net);else snprintf(body,blen,"Five jobs. Clocks pause in menus, docking and dialogue.");}else if(i==2)snprintf(body,blen,"Delivery, hunt, scan, rescue and covert jobs online.");else if(i==3)snprintf(body,blen,"Mission contacts carry white [M] scanner markers.");else snprintf(body,blen,game.job_n>=MISSION_SLOTS?"Log full. Complete a job before taking more work.":"Dock to use the board and accept a contract.");}
 else {const char *a[]={"SHIP COMPUTER","GALACTICNET","NAV COMPUTER","STATION LINK","SYSTEM NOTICE"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"Welcome, Commander. Network link is online.");else if(i==1)snprintf(body,blen,"%d unread local posts.",2+wealth);else if(i==2&&game.contract>=0)snprintf(body,blen,"Mission route set for %s.",game.systems[game.contract].name);else if(i==2)snprintf(body,blen,"No mission route currently assigned.");else if(i==3)snprintf(body,blen,"Docking channel: %s.",game.docked?"connected":"standby");else snprintf(body,blen,"System %s / risk [%s].",game.systems[game.system].name,stars(risk));}
}
static void galnet_avatar(int x,int y,int size,int i){
 int role=TRADERS,seed=100+i;
 if(galnet_tab==3){static const int roles[]={TRADERS,LAW,TRADERS,EXPLORERS,TRADERS,PIRATES,EXPLORERS};role=roles[i%7];if(i==3){draw_kei(x,y,size,0);return;}if(i==1)seed=VOICE_LAW*37;if(i==2)seed=VOICE_DOCK*37;}
 else if(galnet_tab==2)role=LAW;else if(galnet_tab==0)role=i==1?LAW:i==4?EXPLORERS:TRADERS;
 else if(galnet_tab==5)role=EXPLORERS;
 draw_portrait(x,y,size,size,seed,role);
}
#include "spacebook.h"
static void news_screen(void){unsigned paper=RGB(194,184,145),ink=RGB(27,31,31),rule=RGB(91,80,58);rect(6,43,468,205,paper);rect(9,45,462,2,ink);text(16,6,ink,"THE GALACTIC GAZETTE");text(3,8,rule,"SYSTEM: %.15s          LOCAL EDITION",game.systems[game.system].name);rect(14,75,452,2,ink);int first=(row/2)*2;for(int j=0;j<2&&first+j<5;j++){int i=first+j,y=88+j*72;char author[40],body[96];galnet_post(i,author,sizeof(author),body,sizeof(body));if(i==row){rect(12,y-3,4,62,RGB(130,67,42));text(49,y/8,RGB(130,67,42),"SELECTED");}text(3,y/8,ink,"%.28s",author);rect(22,y+13,438,1,rule);text(3,(y+22)/8,ink,"%.53s",body);text(3,(y+38)/8,ink,"%.53s",gazette_dek(game.system,i));text(3,(y+54)/8,rule,"CONTINUED ON GALACTICNET");}text(48,29,ink,"%d/3",row/2+1);footer("L/R SECTION   UP/DOWN SCROLL   O BACK");}
static void galnet_market(void){rect(0,42,W,206,RGB(5,16,18));text(2,6,CYAN,"MARKET EXCHANGE / DELAYED PRICES");text(2,8,DIM,"SYMBOL       LOCAL      GAL AVG      TREND");for(int i=0;i<5;i++){int item=(game.system*3+i*5)%GOODS,y=82+i*29,diff=game.price[item]-galactic_price(item);if(i==row)rect(8,y-5,464,24,RGB(15,47,47));text(2,y/8,i==row?WHITE:DIM,"%-12.12s %7.1f %10.1f",goods[item].name,game.price[item]*.1f,galactic_price(item)*.1f);unsigned c=diff>0?RED:CYAN;int x=360,base=y+8;for(int k=0;k<7;k++){int a=((item*13+k*17)%11)-5,b=((item*13+(k+1)*17)%11)-5;line(x+k*14,base-a,x+(k+1)*14,base-b,c);}text(56,y/8,c,diff>0?"UP":"DOWN");}text(2,29,GOLD,game.docked?"LIVE TERMINAL: LEFT SELL / RIGHT BUY":"DOCK FOR LIVE TRADING");footer("L/R SECTION   UP/DOWN TICKER   O BACK");}
static void galnet_bounties(void){
 rect(0,42,W,206,RGB(26,19,16));
 int posters=5,pages=(posters+2)/3,page=row/3,first=page*3,more=posters-first-3;if(more<0)more=0;
 /* Unmistakable page cue: gold banner on page 1, board-complete on page 2. */
 if(pages>1){
  if(page==0){rect(6,44,468,14,RGB(92,52,18));rect(6,44,468,2,GOLD);text(2,6,GOLD,"WANTED  PAGE 1/%d",pages);text(22,6,WHITE,">> PAGE 2: %d MORE  DOWN",more>0?more:2);}
  else {rect(6,44,468,14,RGB(40,28,22));rect(6,44,468,2,CYAN);text(2,6,CYAN,"WANTED  PAGE %d/%d",page+1,pages);text(24,6,DIM,"<< UP FOR PAGE 1");}
 }else text(2,6,GOLD,"WANTED BOARD");
 for(int j=0;j<3&&first+j<posters;j++){
  int i=first+j,x=10+j*157,y0=62;
  unsigned paper=i==row?RGB(215,185,125):RGB(166,147,108);
  unsigned bg=RGB(26,19,16),ink=RGB(70,35,27),mute=RGB(55,42,32);
  int style=i%5; /* 0 clean 1 corner rip 2 left fray 3 bottom bite 4 hole+crease */
  if(style==0){rect(x,y0,146,168,paper);rect(x+4,y0+4,138,3,RGB(74,43,32));rect(x+70,y0-2,6,6,RGB(90,90,90));}
  else if(style==1){ /* top-right corner ripped off */
   rect(x,y0+4,146,164,paper);rect(x+118,y0,28,18,bg);rect(x+110,y0+14,16,8,bg);rect(x+4,y0+8,130,3,RGB(74,43,32));
   rect(x+8,y0+2,10,4,RGB(120,90,50));rect(x+100,y0+2,10,4,RGB(120,90,50));
  }else if(style==2){ /* left edge frayed + peel */
   rect(x+4,y0,142,168,paper);rect(x,y0,6,48,bg);rect(x,y0+90,8,40,bg);rect(x+138,y0+120,10,48,bg);
   rect(x+8,y0+4,132,3,RGB(74,43,32));rect(x+12,y0+2,10,4,RGB(120,90,50));rect(x+128,y0+2,10,4,RGB(120,90,50));
  }else if(style==3){ /* bottom-left bite + dog-ear */
   rect(x,y0,146,160,paper);rect(x,y0+148,36,20,bg);rect(x+28,y0+156,18,12,bg);rect(x+130,y0,16,14,bg);
   rect(x+4,y0+4,138,3,RGB(74,43,32));rect(x+70,y0-2,6,6,RGB(90,90,90));
  }else{ /* bullet hole + crease streak */
   rect(x,y0,146,168,paper);rect(x+4,y0+4,138,3,RGB(74,43,32));
   rect(x+64,y0+70,14,12,bg);rect(x+66,y0+72,10,8,RGB(40,28,22));
   rect(x+20,y0+40,100,2,RGB(120,95,70));rect(x+8,y0+2,10,4,RGB(120,90,50));rect(x+128,y0+2,10,4,RGB(120,90,50));
  }
  text(x/8+2,9,ink,style?"WANTED!":"WANTED");
  draw_portrait(x+38,y0+28,70,56,game.system*31+i*19+(style*97),PIRATES);
  text(x/8+2,19,ink,"RAIDER %c-%02d",'A'+(i*7+game.system)%26,(i*31+game.system)%100);
  text(x/8+2,21,ink,"BOUNTY %d.0 U",15+i*5);
  text(x/8+2,23,ink,"RISK %d/5",danger_rating(&game,game.system));
  static const char *wear[]={"LAST SEEN","TORN COPY","WEATHERED","DOG-EARED","HOLED"};
  text(x/8+2,26,mute,"%s",wear[style]);
  text(x/8+2,28,mute,"%.13s",game.systems[game.system].name);
 }
 if(pages>1&&page==0){rect(8,234,464,10,RGB(92,52,18));text(8,29,GOLD,"DOWN  >>  PAGE 2 OF %d",pages);}
 else if(pages>1){rect(8,234,464,10,RGB(40,28,22));text(8,29,CYAN,"PAGE %d/%d  BOARD COMPLETE",page+1,pages);}
 footer("L/R SECTION   UP/DOWN POSTER   O BACK");
}
static void galnet_chrome(void){
 header("GALACTICNET // LIVE NETWORK");rect(0,22,W,20,RGB(11,25,35));
 /* Six equal tabs between L/R pads — short labels so SPACEBOOK/MESSAGES never collide. */
 rect(0,22,28,20,RGB(8,18,28));rect(452,22,28,20,RGB(8,18,28));
 rect(2,24,24,16,RGB(25,65,77));rect(454,24,24,16,RGB(25,65,77));
 text(1,3,CYAN,"<L");text(57,3,CYAN,"R>");
 const char *shorts[]={"NEWS","MARKET","WANTED","BOOK","INBOX","JOBS"};
 for(int i=0;i<6;i++){
  int x=30+i*70,tw=68;
  if(i==galnet_tab){rect(x,22,tw,20,RGB(25,65,77));rect(x,40,tw,2,GOLD);}
  int len=(int)strlen(shorts[i]);int col=(x+(tw-len*8)/2)/8;
  text(col,3,i==galnet_tab?WHITE:DIM,"%s",shorts[i]);
 }
}
static void galnet_screen(void){if(galnet_tab==3||galnet_tab==4)spacebook_screen(galnet_tab==4);else if(galnet_tab==0)news_screen();else if(galnet_tab==1)galnet_market();else if(galnet_tab==2)galnet_bounties();else {int count=galnet_rows(),pages=(count+2)/3,first=row/3*3;rect(0,42,W,206,BG);text(2,6,CYAN,"MISSION FEED");page_number_at(20,6,row/3+1,pages);for(int j=0;j<3&&first+j<count;j++){int i=first+j,y=8+j*5;char author[40],body[96];galnet_post(i,author,sizeof(author),body,sizeof(body));panel(8,y*8-3,464,34);if(i==row)rect(8,y*8-3,3,34,GOLD);galnet_avatar(14,y*8-1,28,i);text(7,y,i==row?GOLD:CYAN,"%s",author);text(7,y+2,WHITE,"%.50s",body);}footer("L/R SECTION   UP/DOWN   O BACK");}galnet_chrome();}
static void story_screen(void){
 header("FLIGHT GUIDE / OPTIONAL");panel(8,32,464,190);
 draw_kei(16,40,48,0);text(10,5,CYAN,"KEI / FLIGHT COACH");
 text(10,7,DIM,"Help with controls; separate from the story.");
 text(3,12,WHITE,"Optional help with your ship's controls.");
 text(3,14,WHITE,"Show the next lesson to highlight its menu entry.");
 text(3,16,DIM,"You can end this guide and use Controls at any time.");
 text(3,20,GOLD,"%s",game.story<STORY_FREE?story_task(&game):"Guide finished. Fly whenever you are ready.");
 if(row==0)selected_span(23,464);
 text(3,23,row==0?WHITE:DIM,"%s %s",row==0?">":" ",game.story<STORY_FREE?"Show the next lesson on the menu":"Return to the main menu");
 if(game.story<STORY_FREE){if(row==1)selected_span(26,464);text(3,26,row==1?WHITE:DIM,"%s End the optional guide",row==1?">":" ");}
 footer("UP/DOWN CHOOSE   X SELECT   O BACK");
}


