static int cargo_item(int selection){if(game.docked)return selection;int n=0;for(int i=0;i<GOODS;i++)if(game.cargo[i]>0){if(n++==selection)return i;}return -1;}
static int cargo_rows(void){if(game.docked)return GOODS;int n=0;for(int i=0;i<GOODS;i++)n+=game.cargo[i]>0;return n?n:1;}
static int contact_ids[1+BODY_COUNT+NPC_COUNT+DEBRIS_COUNT+ANOMALY_COUNT],contact_count;
static void contacts_refresh(void){contact_count=0;for(int i=0;i<=ANOMALY_ID_MAX;i++)if(valid_target(i))contact_ids[contact_count++]=i;}
static void panel(int x,int y,int w,int h){rect(x,y,w,h,RGB(7,14,22));rect(x,y,w,1,CYAN);rect(x,y+h-1,w,1,RGB(22,44,58));rect(x,y,1,h,CYAN);rect(x+w-1,y,1,h,RGB(18,36,48));rect(x,y,7,1,GOLD);rect(x,y,1,7,GOLD);rect(x+w-7,y,7,1,GOLD);rect(x+w-1,y,1,7,GOLD);}
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
static int target_matches(int id){if(target_filter==0)return 1;if(target_filter==1)return IS_NPC_ID(id);if(target_filter==2)return IS_NPC_ID(id)&&(game.npc[id-BODY_COUNT-1].role==PIRATES||game.npc[id-BODY_COUNT-1].target==-2);if(target_filter==3)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==LAW;if(target_filter==4)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].role==TRADERS;if(target_filter==5)return is_mission_target(&game,id);if(target_filter==6)return id>0&&id<=BODY_COUNT;if(target_filter==7)return id==0;if(target_filter==8)return IS_DEBRIS_ID(id);if(target_filter==9)return IS_NPC_ID(id)&&game.npc[id-BODY_COUNT-1].freighter;return IS_ANOMALY_ID(id);}
static void target_refresh(void){target_count=0;for(int id=0;id<=ANOMALY_ID_MAX;id++)if(valid_target(id)&&target_matches(id))target_ids[target_count++]=id;if(row>=target_count)row=0;}
static unsigned contact_color(int id){if(is_mission_target(&game,id))return WHITE;if(IS_NPC_ID(id))return faction_colors[game.npc[id-BODY_COUNT-1].role];if(IS_ANOMALY_ID(id))return GOLD;if(IS_DEBRIS_ID(id))return DIM;return id==0?CYAN:GOLD;}
static const char *target_status(int id){if(is_mission_target(&game,id))return "MISSION TARGET";if(IS_ANOMALY_ID(id))return game.anomaly[id-ANOMALY_ID_MIN].scanned?"CODEX ENTRY":"UNSCANNED ANOMALY";if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].rock?"LASER-MINEABLE ROCK":game.debris[id-DEBRIS_ID_MIN].wreck?"SALVAGE WRECK":"COLLECTABLE CARGO";if(id==0)return "DOCKABLE";if(id<=BODY_COUNT){Body *b=&game.bodies[id-1];return b->type==SUN?"STAR / NO LANDING":b->type==GAS?"NO LANDING":"APPROACHABLE";}NPC *n=&game.npc[id-BODY_COUNT-1];if(n->target==-2)return "HOSTILE";if(n->role==PIRATES)return "WANTED";if(n->role==LAW)return "LAW";if(n->role==TRADERS)return n->freighter?freight_status(n):"TRADER";return "NEUTRAL";}
static void targeting_screen(void){
 target_count=collect_scan_ids(target_ids,scan_cat);if(row>=target_count)row=0;
 header("TARGETING COMPUTER");page_number_at(52,4,target_count?row/7+1:1,target_count?(target_count+6)/7:1);
 for(int i=0;i<4;i++){int col=1+i*14;if(i==scan_cat)rect(col*8-2,30,108,14,RGB(25,65,77));text(col,4,i==scan_cat?GOLD:DIM,"%s",scan_cat_names[i]);}
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
 const int types[5]={SUN,OCEAN,ROCKY,GAS,ROCKY};
 const unsigned suns[]={0x80dfff,0xffd8ac,0x5088ff,0xc4f4ff};
 const unsigned worlds[]={0xc97535,0x91b45c,0x8763b5,0x7ebfc4,0xb87775,0xadc2ce};
 unsigned h=art_hash((sys+1)*911u+i*65537u);
 *type=types[i];*col=i==0?suns[h%4]:worlds[(h>>8)%6];*acc=worlds[(h>>16)%6];
 if(sys==7&&i==1){*col=0xc35f23;*acc=0x4b9137;}
}
static void chart_system_preview(int dest){
 int xs[5]={352,394,436,373,415},ys[5]={58,52,58,92,96},rs[5]={16,12,10,14,11};
 for(int i=0;i<BODY_COUNT;i++){unsigned col,acc;int type;body_tint(dest,i,&col,&acc,&type);draw_planet_disc(xs[i],ys[i],rs[i],col,acc,art_hash((dest+1)*911u+i*65537u),type);}
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
static const char *codex_tabs[]={"PLANETS","FLORA","FAUNA","MINERALS","ECHOES"};
static int vis_count(void){int n=1;for(int s=0;s<256;s++)if(s!=game.system&&(game.visited[s>>3]&(1<<(s&7))))n++;return n;}
static int vis_sys(int idx){if(idx<=0)return game.system;int n=1;for(int s=0;s<256;s++)if(s!=game.system&&(game.visited[s>>3]&(1<<(s&7)))){if(n==idx)return s;n++;}return game.system;}
static int codex_kind_count(int tab){if(tab==0)return vis_count();if(tab==1)return game.scanned_flora;if(tab==2)return game.scanned_fauna;if(tab==3)return game.scanned_minerals;return game.scanned_anomalies;}
static int codex_rows(void){int n=codex_kind_count(codex_tab);return n>0?n:1;}
static void codex_life_label(int tab,int i,char *name,int nn,char *where,int wn){
 const char *flora[]={"GLOW VINE","GLASS FERN","SPORE TREE","NIGHT MOSS","KELP FAN","IRON MOSS"};
 const char *fauna[]={"GLASS MOTH","DUST RUNNER","SKY RAY","BURROWER","SAND HOPPER","DRIFT EEL"};
 const char *ore[]={"RED ORE","ICE CRYSTAL","BASALT VEIN","SILICA","NICKEL SEAM","CARBON LACE"};
 const char *echo[]={"MERIDIAN ECHO","STELLAR RIFT","GHOST PING","QUASAR HUM"};
 const char **src=tab==1?flora:tab==2?fauna:tab==3?ore:echo;int names=tab==4?4:6;
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
  for(int b=0;b<BODY_COUNT;b++){body_tint(sys,b,&col,&acc,&type);draw_planet_disc(270+b*38,78,b==0?16:12,col,acc,art_hash((sys+1)*911u+b*65537u),type);}
  text(32,14,CYAN,"%.22s",game.systems[sys].name);
  text(32,16,WHITE,"Sun + 4 worlds");
  if(sys==game.system)text(32,18,GOLD,"Here: flora %d fauna %d",game.scanned_flora,game.scanned_fauna);
  else text(32,18,DIM,"Charted system");
 }else if(n<=0){text(3,10,DIM,"Nothing logged yet.");text(32,10,WHITE,"Scan on foot. Square.");}
 else {
  for(int j=0;j<7&&first+j<count;j++){int i=first+j,y=7+j*2;char name[24],where[24];codex_life_label(codex_tab,i,name,sizeof(name),where,sizeof(where));if(i==row)rect(10,y*8-2,220,13,RGB(25,65,77));text(3,y,i==row?WHITE:DIM,"%.22s",name);}
  char name[24],where[24];codex_life_label(codex_tab,row,name,sizeof(name),where,sizeof(where));
  if(codex_tab==1)draw_flora_icon(270,60,row+1);else if(codex_tab==2)draw_fauna_icon(270,60,row+1);else if(codex_tab==3)draw_mineral_icon(270,60,row+1);else draw_anomaly_icon(268,58,row&1);
  text(32,14,GOLD,"%.22s",name);
  text(32,16,WHITE,"%.22s",where);
  text(32,18,DIM,codex_tab==4?"O on an echo in space.":"Square on foot to log.");
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
static const char *equipment_names[]={"REFUEL TANK","BEAM LASER","DOCKING COMPUTER","SHIELD BOOSTER","LASER COOLING","EXPANDED CARGO BAY","LONG-RANGE SCANNER","FUEL SCOOP","MISSILE RESTOCK"};
static const char *equipment_list_names[]={"REFUEL TANK","BEAM LASER","DOCK COMPUTER","SHIELD BOOST","LASER COOLING","CARGO BAY +8T","LONG SCANNER","FUEL SCOOP","MISSILE +1"};
static const char *equipment_details[]={"Fill hyperspace tank.","Twice laser damage.","Docking from 8,000 m.","Twice shield recharge.","Laser cools faster.","Adds eight tonnes.","IDs distant contacts.","Skim fuel near a sun.","Load one missile."};
static const char *equipment_effects[]={"Tank: now -> ship max","Laser 18 -> 36","Dock 2500 -> 8000 m","Shield 1.5 -> 3.0 /s","Cool 22 -> 38 /s","Hold +8 tonnes","Scan 3 -> 6 fields","Fuel +0.5 /s at sun","Missiles +1"};
static const int equipment_costs[]={0,4000,2500,6000,4500,3500,3000,7500,1000};
static const int equipment_tech[]={0,4,5,7,6,4,5,9,3};
static const int equip_show[]={0,7,1,8,3,4,2,6,5};
static const char *equip_cat[]={"FUEL","FUEL","WPN","WPN","DEF","DEF","NAV","NAV","HOLD"};
static int equipment_owned(int i){if(i==0)return game.fuel>=player_ships[game.ship].range;if(i==1)return game.laser;if(i==8)return game.missiles>=4;return (game.upgrades&(1<<(i-2)))!=0;}
static void buy_equipment(int i){if(!game.docked){message(&game,"Dock to buy equipment.");return;}if(i<0||i>=9)return;if(i==0){int cost=(int)ceilf(player_ships[game.ship].range-game.fuel)*2;if(cost<=0){message(&game,"Tank is already full.");return;}if(game.credits<cost){message(&game,"Not enough units.");return;}game.credits-=cost;game.fuel=player_ships[game.ship].range;game.cue=SFX_UI;message(&game,"Tank full.");return;}if(equipment_owned(i)){message(&game,i==8?"Missile rack full.":"Already installed.");return;}if(game.systems[game.system].tech+1<equipment_tech[i]){message(&game,"This hub does not have the required technology.");return;}if(game.credits<equipment_costs[i]){message(&game,"Not enough units.");return;}game.credits-=equipment_costs[i];game.cue=SFX_UI;if(i==1)game.laser=1;else if(i==8){game.missiles++;message(&game,"Missile loaded.");return;}else game.upgrades|=1<<(i-2);message(&game,i==6?"Long-range scanner online.":"Upgrade installed.");}
static void equipment(void){
 header("OUTFITTING");if(!game.docked){text(3,8,DIM,"Dock to view equipment and fuel.");footer("O BACK");return;}
 panel(8,32,225,156);panel(241,32,231,156);
 int first=row/6*6,fuelcost=(int)ceilf(player_ships[game.ship].range-game.fuel)*2;
 text(2,5,CYAN,"CATALOG");page_number_at(18,5,row/6+1,2);for(int j=0;j<6&&first+j<9;j++){int disp=first+j,i=equip_show[disp],y=7+j*3;if(disp==row)rect(10,y*8-3,220,15,RGB(25,65,77));text(2,y,equipment_owned(i)?CYAN:disp==row?WHITE:DIM,"%-4s %-14s%s",equip_cat[disp],equipment_list_names[i],equipment_owned(i)?" *":"");}
 int i=equip_show[row];const int art_item[]={19,10,18,17,19,9,18,19,10};draw_next_art(next_items[art_item[i]],32,32,430,55,32,32);
 text(31,5,GOLD,"%.22s",equipment_names[i]);
 text(31,7,CYAN,"%.22s",equip_cat[row]);
 text(31,9,WHITE,"%.21s",equipment_details[i]);
 text(31,12,CYAN,"%.27s",equipment_effects[i]);
 text(31,15,DIM,"Tech %d / hub %d",equipment_tech[i],game.systems[game.system].tech+1);
 text(31,18,equipment_owned(i)?CYAN:WHITE,equipment_owned(i)?i==8?"Rack full":i==0?"Tank full":"Installed":"%.1f units",row>=0&&i==0?fuelcost*.1f:equipment_costs[i]*.1f);
 text(31,21,DIM,"Balance %.1f",game.credits*.1f);
 footer("UP/DOWN   X BUY / REFUEL   O BACK");
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
 if(row==0){int traders=0,law=0,pirates=0,explorers=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive){if(game.npc[i].role==TRADERS)traders++;else if(game.npc[i].role==LAW)law++;else if(game.npc[i].role==PIRATES)pirates++;else explorers++;}draw_station_badge(270,52);text(32,12,CYAN,"%.22s",station_name(&game));text(32,14,WHITE,"Docking hub / live traffic");text(32,16,CYAN,"T%d  L%d  P%d  E%d",traders,law,pirates,explorers);text(32,18,danger_rating(&game,game.system)>=4?RED:AMBER,"Danger %d/5  Prosperity %d/5",danger_rating(&game,game.system),prosperity(&game,game.system));text(32,20,DIM,"%s",pirates>law?"Pirate activity elevated.":law>0?"Patrol cover active.":"Quiet local traffic.");}
 else {Body *b=&game.bodies[row-1];draw_planet_disc(360,78,28,b->color,b->accent,b->seed,b->type);text(32,14,CYAN,"%.22s",b->name);text(32,16,WHITE,"%.22s",kinds[row-1]);text(32,18,DIM,b->type==SUN||b->type==GAS?"No landing":"Approachable");}
 char lore[48];lore_line(game.system,0,lore,sizeof(lore));text(32,row==0?22:20,WHITE,"%.22s",lore);
 text(2,22,DIM,"%.22s  T%d",economies[game.systems[game.system].economy],game.systems[game.system].tech+1);
 footer("UP/DOWN   X LOCK   TRI ALIGN   O BACK");
}
static void factions(void){
 header("FACTIONS");panel(8,32,232,156);panel(248,32,224,156);
 const char *blurb[]={"Trade between hubs.","Hunt pirates.","Raid passing traders.","Survey nearby worlds."};
 const char *d0[]={"Convoy lanes feed the hub.","Patrols stop wanted ships.","Pirate bounty: 15 units.","Guild wings survey a world."};
 const char *d1[]={"Hail them for market talk.","Pay a fine or take custody.","Missiles lock hostiles only.","They will not fire first."};
 for(int i=0;i<FACTION_COUNT;i++){int count=0;for(int j=0;j<NPC_COUNT;j++)if(game.npc[j].alive&&game.npc[j].role==i)count++;int y=5+i*4;if(i==row)rect(10,y*8-2,220,28,RGB(25,65,77));draw_portrait(14,y*8-1,28,24,game.system+i*17,i);text(7,y,faction_colors[i],"%.9s",faction_names[i]);text(17,y,i==row?GOLD:WHITE,"SHIPS HERE: %d",count);text(7,y+2,i==row?WHITE:DIM,"%s",blurb[i]);}
 draw_portrait(270,48,86,72,game.system+row*17,row);
 text(32,16,faction_colors[row],"%.22s",faction_names[row]);
 text(32,18,WHITE,"%s",d0[row]);
 text(32,20,WHITE,"%s",d1[row]);
 footer("UP/DOWN   O BACK");
}
static void mission_board(void){header("MISSION BOARD");panel(8,32,464,156);if(!game.docked){text(3,8,WHITE,"Dock to take work.");text(3,10,DIM,"You can hold five jobs.");footer("O BACK");return;}int count=mission_count(&game);text(2,5,game.job_n>=MISSION_SLOTS?RED:GOLD,"Log %d/5%s",game.job_n,game.job_n>=MISSION_SLOTS?"  full":"   X accept  |  10 units  |  5:00");text(5,7,CYAN,"%-16s %-8s %s","JOB","DEST","REWARD");for(int i=0;i<count&&i<6;i++){int id=mission_destination(&game,i),type=mission_type_for_offer(&game,i),y=9+i*2,active=mission_offer_active(&game,i),risk=mission_risk(&game,i);if(i==row)selected(y);draw_icon(12,y*8-1,12+type,i==row);text(5,y,active?GOLD:risk>=4?RED:WHITE,"%-16.16s %-8.8s %6.1f",mission_name(type),game.systems[id].name,mission_reward(&game,i)*.1f);text(38,y,risk>=4?RED:AMBER,"%d/5",risk);if(active)text(43,y,GOLD,"LIVE");}if(count){text(3,21,DIM,"%.45s",mission_brief(&game,row<count?row:0));text(34,23,mission_risk(&game,row<count?row:0)>=4?RED:CYAN,"RISK %d/5",mission_risk(&game,row<count?row:0));}footer(game.job_n>=MISSION_SLOTS?"LOG FULL   SELECT > LOG   O BACK":"UP/DOWN   X ACCEPT   SELECT > LOG");}
static void mission_log(void){header("MISSION LOG / CHOOSE TRACKED");panel(8,32,464,156);text(2,5,CYAN,"  MISSION                         STATUS");int total=2+game.job_n;for(int i=0;i<total;i++){int y=7+i*2;if(i==row)selected(y);unsigned ink=i==tracked_mission?GOLD:i==row?WHITE:DIM;if(i==0)text(2,y,ink,"%s MAIN STORY / KEI + RYN        %s",i==tracked_mission?"*":" ",game.saga_chapter>=SAGA_COUNT?"DONE":"ACTIVE");else if(i==1)text(2,y,ink,"%s EXPLORERS GUILD ASSIGNMENTS   %s",i==tracked_mission?"*":" ",game.guild_chapter>=4?"DONE":"OPTIONAL");else {Job *j=&game.jobs[i-2];text(2,y,ink,"%s %-18.18s -> %-8.8s %3.0fs",i==tracked_mission?"*":" ",mission_name(j->type),game.systems[j->dest].name,j->time);}}rect(8,165,464,23,RGB(15,31,39));const char *objective=row==0?(game.campaign_stage>=6&&game.saga_chapter<SAGA_COUNT?saga_beats[game.saga_chapter].objective:campaign_task(&game)):row==1?guild_objective(&game):mission_objective_at(&game,row-2);text(2,21,abandon_confirm?RED:GOLD,"%s %.43s",abandon_confirm?"WARNING:":"NEXT:",abandon_confirm?"ABANDON? X CONFIRM / O CANCEL":objective);footer("X TRACK   SELECT NEXT STEP   TRI ABANDON   O BACK");}
static void navigate_job(int index){if(index<0||index>=game.job_n)return;Job *j=&game.jobs[index];game.job_sel=index;route_set_goal(&game,j->dest);game.destination=j->dest;if(game.system!=j->dest){int jumps=0,hop=route_next_hop(&game,j->dest,&jumps);if(hop>=0){game.destination=hop;change_page(CHART);char note[96];snprintf(note,sizeof(note),"Route: %d jump%s. Refuel at intermediate hubs.",jumps,jumps==1?"":"s");message(&game,note);}else message(&game,"No fuel-safe route. Refuel at the hub first.");}else {route_clear(&game);int id=mission_target_id(&game,index);if(id>=0&&valid_target(id)){selected_target=id;scan_cat=target_category(id);autoaim=1;message(&game,"Mission target locked. Auto-align active.");if(!game.docked)change_page(FLIGHT);}else if(game.docked)message(&game,"Launch to continue this objective in the system.");else message(&game,"Objective unavailable. Re-enter this system.");}}
static int galnet_tab=0;
static int galnet_rows(void){return galnet_tab==3?7:5;}
static int active_role(int role){int n=0;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&game.npc[i].role==role)n++;return n;}
static void galnet_post(int i,char *author,int alen,char *body,int blen){
 int risk=danger_rating(&game,game.system),wealth=prosperity(&game,game.system),item=(game.system*3+i*5)%GOODS;
 if(galnet_tab==0){const char *a[]={"SYSTEM DESK","LAW BULLETIN","TRAFFIC CONTROL","ECONOMY WIRE","EXPLORER GUILD"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"%s risk is %d/5: %s traffic expected.",game.systems[game.system].name,risk,risk>=4?"heavy combat":"routine");else if(i==1)snprintf(body,blen,"%d Law craft and %d pirates currently on scanner.",active_role(LAW),active_role(PIRATES));else if(i==2)snprintf(body,blen,"%s reports normal docking operations.",station_name(&game));else if(i==3)snprintf(body,blen,"Local prosperity %d/5; technology level %d.",wealth,game.systems[game.system].tech+1);else snprintf(body,blen,system_whales(game.system)?"Migrating whales reported beyond the gas giant.":system_ice_belt(game.system)?"Ice-belt survey teams are charting the outer ring.":system_rock_belt(game.system)?"A rock belt is visible off the inner worlds.":"Survey teams are charting all four local worlds.");}
 else if(galnet_tab==1){snprintf(author,alen,"MARKET TIP / %s",goods[item].name);if(i==0)snprintf(body,blen,"Local stock strength: %d/5. Dock for live prices.",wealth);else if(i==1)snprintf(body,blen,"Industrial hubs favour machinery and computers.");else if(i==2)snprintf(body,blen,"Agricultural worlds often export food cheaply.");else if(i==3)snprintf(body,blen,"Restricted goods can raise your local wanted level.");else snprintf(body,blen,"Cargo space: %d/%d tonnes used.",cargo_used(&game),cargo_capacity(&game));}
 else if(galnet_tab==2){const char *a[]={"BOUNTY DESK","LOCAL LAW","PILOT WARNING","PATROL WATCH","SECURITY FEED"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"Pirate contacts active: %d. Standard bounty 15.0.",active_role(PIRATES));else if(i==1)snprintf(body,blen,"Your local wanted record: [%s].",stars(wanted_level(&game)));else if(i==2)snprintf(body,blen,risk>=4?"Travel in groups. Hostile activity is elevated.":"No major raid warning at this time.");else if(i==3)snprintf(body,blen,"Law patrols active: %d.",active_role(LAW));else snprintf(body,blen,"Wanted records remain inside the offending system.");}
 else if(galnet_tab==3){const char *a[]={"Mira / Trader","Marshal Iona Renn","Dockhand_77","Kei / Explorer","Freighter Crew","DefinitelyNotAPirate","Lave Spotters"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"%s market looks %s today.",game.systems[game.system].name,wealth>=4?"well stocked":"a little thin");else if(i==1)snprintf(body,blen,risk>=4?"Pirate sightings up. Keep scanners active.":"Patrol lanes are calm. Fly safely, commanders.");else if(i==2)snprintf(body,blen,"Stop boosting near %s, you maniacs.",station_name(&game));else if(i==3)snprintf(body,blen,game.story<STORY_FREE?"Ryn's last ping is still on this wire.":"The colour of %s is unreal from orbit.",game.bodies[1].name);else if(i==4)snprintf(body,blen,"Slow convoy crossing the system. Give us room.");else if(i==5)snprintf(body,blen,"Free cargo inspection behind the gas giant. Honest.");else if(game.mission_result)snprintf(body,blen,"Commander mission report: %s %s.",mission_name(game.last_mission_type),game.mission_result>0?"complete":"expired");else snprintf(body,blen,"Saw Law chasing raiders beyond the trade lane.");}
 else if(galnet_tab==4){snprintf(author,alen,"MISSION NETWORK");if(i==0&&game.job_n>0)snprintf(body,blen,"Log %d/5. Focus %s to %s; %.0fs.",game.job_n,mission_name(game.jobs[game.job_sel].type),game.systems[game.jobs[game.job_sel].dest].name,game.jobs[game.job_sel].time);else if(i==0&&game.mission_result)snprintf(body,blen,"Last job: %s %s at %s.",mission_name(game.last_mission_type),game.mission_result>0?"complete":"expired",game.systems[game.last_mission_system].name);else if(i==0)snprintf(body,blen,"No active missions. %d local offers available.",mission_count(&game));else if(i==1)snprintf(body,blen,"Five jobs. Clocks pause in menus, docking and dialogue.");else if(i==2)snprintf(body,blen,"Delivery, hunt, scan, rescue and covert jobs online.");else if(i==3)snprintf(body,blen,"Mission contacts carry white [M] scanner markers.");else snprintf(body,blen,game.job_n>=MISSION_SLOTS?"Log full. Complete a job before taking more work.":"Dock to use the board and accept a contract.");}
 else {const char *a[]={"SHIP COMPUTER","GALACTICNET","NAV COMPUTER","STATION LINK","SYSTEM NOTICE"};snprintf(author,alen,"%s",a[i]);if(i==0)snprintf(body,blen,"Welcome, Commander. Network link is online.");else if(i==1)snprintf(body,blen,"%d unread local posts.",2+wealth);else if(i==2&&game.contract>=0)snprintf(body,blen,"Mission route set for %s.",game.systems[game.contract].name);else if(i==2)snprintf(body,blen,"No mission route currently assigned.");else if(i==3)snprintf(body,blen,"Docking channel: %s.",game.docked?"connected":"standby");else snprintf(body,blen,"System %s / risk [%s].",game.systems[game.system].name,stars(risk));}
}
static void galnet_avatar(int x,int y,int size,int i){
 int role=TRADERS,seed=100+i;
 if(galnet_tab==3){static const int roles[]={TRADERS,LAW,TRADERS,EXPLORERS,TRADERS,PIRATES,EXPLORERS};role=roles[i%7];if(i==3){draw_kei(x,y,size,0);return;}if(i==1)seed=VOICE_LAW*37;if(i==2)seed=VOICE_DOCK*37;}
 else if(galnet_tab==2)role=LAW;else if(galnet_tab==0)role=i==1?LAW:i==4?EXPLORERS:TRADERS;
 else if(galnet_tab>=4)role=EXPLORERS;
 draw_portrait(x,y,size,size,seed,role);
}
#include "spacebook.h"
static void news_screen(void){unsigned paper=RGB(194,184,145),ink=RGB(27,31,31),rule=RGB(91,80,58);rect(6,43,468,205,paper);rect(9,45,462,2,ink);text(16,6,ink,"THE GALACTIC GAZETTE");text(3,8,rule,"SYSTEM: %.15s          LOCAL EDITION",game.systems[game.system].name);rect(14,75,452,2,ink);int first=(row/2)*2;for(int j=0;j<2&&first+j<5;j++){int i=first+j,y=88+j*72;char author[40],body[96];galnet_post(i,author,sizeof(author),body,sizeof(body));if(i==row){rect(12,y-3,4,62,RGB(130,67,42));text(49,y/8,RGB(130,67,42),"SELECTED");}text(3,y/8,ink,"%.28s",author);rect(22,y+13,438,1,rule);text(3,(y+22)/8,ink,"%.53s",body);text(3,(y+38)/8,ink,"%.53s",i==0?"Pilots are advised to check routes before launch.":i==1?"Scanner reports update throughout the local day.":i==2?"Dock control asks commanders to approach at safe speed.":i==3?"Prices remain available only at a docked terminal.":"Guild observers invite verified field reports.");text(3,(y+54)/8,rule,"CONTINUED ON GALACTICNET");}text(48,29,ink,"%d/3",row/2+1);footer("L/R SECTION   UP/DOWN SCROLL   O BACK");}
static void galnet_market(void){rect(0,42,W,206,RGB(5,16,18));text(2,6,CYAN,"MARKET EXCHANGE / DELAYED PRICES");text(2,8,DIM,"SYMBOL       LOCAL      GAL AVG      TREND");for(int i=0;i<5;i++){int item=(game.system*3+i*5)%GOODS,y=82+i*29,diff=game.price[item]-galactic_price(item);if(i==row)rect(8,y-5,464,24,RGB(15,47,47));text(2,y/8,i==row?WHITE:DIM,"%-12.12s %7.1f %10.1f",goods[item].name,game.price[item]*.1f,galactic_price(item)*.1f);unsigned c=diff>0?RED:CYAN;int x=360,base=y+8;for(int k=0;k<7;k++){int a=((item*13+k*17)%11)-5,b=((item*13+(k+1)*17)%11)-5;line(x+k*14,base-a,x+(k+1)*14,base-b,c);}text(56,y/8,c,diff>0?"UP":"DOWN");}text(2,29,GOLD,game.docked?"LIVE TERMINAL: LEFT SELL / RIGHT BUY":"DOCK FOR LIVE TRADING");footer("L/R SECTION   UP/DOWN TICKER   O BACK");}
static void galnet_bounties(void){rect(0,42,W,206,RGB(26,19,16));int first=row/3*3;for(int j=0;j<3&&first+j<5;j++){int i=first+j,x=10+j*157;unsigned paper=i==row?RGB(215,185,125):RGB(166,147,108);rect(x,50,146,188,paper);rect(x+4,54,138,3,RGB(74,43,32));text(x/8+2,8,RGB(70,35,27),"WANTED");draw_portrait(x+38,78,70,60,game.system*31+i*19,PIRATES);char name[32],body[96];galnet_post(i,name,sizeof(name),body,sizeof(body));text(x/8+2,18,RGB(70,35,27),"RAIDER %c-%02d",'A'+(i*7+game.system)%26,(i*31+game.system)%100);text(x/8+2,20,RGB(70,35,27),"BOUNTY %d.0 U",15+i*5);text(x/8+2,22,RGB(70,35,27),"RISK %d/5",danger_rating(&game,game.system));text(x/8+2,25,RGB(55,42,32),"LAST SEEN");text(x/8+2,27,RGB(55,42,32),"%.13s",game.systems[game.system].name);}footer("L/R SECTION   UP/DOWN POSTER   O BACK");}
static void galnet_chrome(void){header("GALACTICNET // LIVE NETWORK");rect(0,22,W,20,RGB(11,25,35));const char *shorts[]={"NEWS","MARKET","WANTED","SPACEBOOK","JOBS","MESSAGES"};for(int i=0;i<6;i++){int x=i*80;if(i==galnet_tab){rect(x,22,80,20,RGB(25,65,77));rect(x,40,80,2,GOLD);}text(x/8+1,3,i==galnet_tab?WHITE:DIM,"%.9s",shorts[i]);}text(0,3,CYAN,"L");text(59,3,CYAN,"R");}
static void galnet_screen(void){if(galnet_tab==3||galnet_tab==5)spacebook_screen(galnet_tab==5);else if(galnet_tab==0)news_screen();else if(galnet_tab==1)galnet_market();else if(galnet_tab==2)galnet_bounties();else {int count=galnet_rows(),pages=(count+2)/3,first=row/3*3;rect(0,42,W,206,BG);text(2,6,CYAN,galnet_tab==4?"MISSION FEED":"MESSAGE FEED");page_number_at(20,6,row/3+1,pages);for(int j=0;j<3&&first+j<count;j++){int i=first+j,y=8+j*5;char author[40],body[96];galnet_post(i,author,sizeof(author),body,sizeof(body));panel(8,y*8-3,464,34);if(i==row)rect(8,y*8-3,3,34,GOLD);galnet_avatar(14,y*8-1,28,i);text(7,y,i==row?GOLD:CYAN,"%s",author);text(7,y+2,WHITE,"%.50s",body);}footer("L/R SECTION   UP/DOWN   O BACK");}galnet_chrome();}
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


