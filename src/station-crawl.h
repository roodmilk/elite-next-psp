/* First-person NES dungeon-crawler station concourse + NPC services. */
enum { SC_W=5, SC_H=4, SC_N=0, SC_E=1, SC_S=2, SC_WDIR=3 };
enum { SC_ROOM_EMPTY=0, SC_ROOM_HUB, SC_ROOM_SHOP, SC_ROOM_BAR, SC_ROOM_BAY, SC_ROOM_CLINIC, SC_ROOM_GUILD, SC_ROOM_LOCK };
enum { SC_ACT_TALK=0, SC_ACT_SHOP, SC_ACT_GIFT, SC_ACT_QUEST, SC_ACT_TAXI, SC_ACT_BOARD };
enum { SC_MENU_NONE=0, SC_MENU_PERSON, SC_MENU_SHOP };
static int sc_x=2,sc_y=0,sc_face=SC_N,sc_menu=0,sc_choice=0,sc_shop_row=0,sc_built_for=-1;
static unsigned char sc_map[SC_H][SC_W];
static unsigned char sc_door_n[SC_H][SC_W],sc_door_e[SC_H][SC_W]; /* doors to N and E neighbours */
typedef struct { const char *name; int role; int act; int shop_item; int gift_bit; int quest_pay; int taxi_pay; const char *line; const char *offer; } ScNpc;
static int sc_fill_npcs(int x,int y,ScNpc *out,int maxn){
 int n=0; unsigned h=(unsigned)(game.system*131u+x*17u+y*41u);
 int room=sc_map[y][x];
 #define SC_PUSH(N,R,A,SI,GB,QP,TP,L,O) do{if(n<maxn){out[n].name=(N);out[n].role=(R);out[n].act=(A);out[n].shop_item=(SI);out[n].gift_bit=(GB);out[n].quest_pay=(QP);out[n].taxi_pay=(TP);out[n].line=(L);out[n].offer=(O);n++;}}while(0)
 if(room==SC_ROOM_HUB){
  SC_PUSH("VENN",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Berth six stays warm if you keep coming home.","Ask about traffic");
  SC_PUSH("DOCKHAND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Watch the freighter lane. Pads are tight.","Ask about berths");
 }else if(room==SC_ROOM_SHOP){
  SC_PUSH("CHANDLER",TRADERS,SC_ACT_SHOP,-1,-1,0,0,"I stock what the main board won't list.","Browse exclusive stock");
  if((h&3)==0)SC_PUSH("MECHANIC",TRADERS,SC_ACT_GIFT,-1,0,0,0,"Take a spare clamp. Once. Don't ask twice.","Accept free clamp");
 }else if(room==SC_ROOM_BAR){
  SC_PUSH("BARTENDER",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Meridian tips well. Truth tips better.","Ask for rumours");
  SC_PUSH("TRAVELER",EXPLORERS,SC_ACT_TAXI,-1,-1,0,1800,"Need a lift to another hub. One tonne seat.","Offer taxi berth");
 }else if(room==SC_ROOM_BAY){
  SC_PUSH("LOADER",TRADERS,SC_ACT_QUEST,-1,-1,400,0,"Haul a crate mark to the board for me.","Take 40 U tip job");
 }else if(room==SC_ROOM_CLINIC){
  SC_PUSH("MEDIC",LAW,SC_ACT_GIFT,-1,1,0,0,"Medkit for the next scrape. One issue only.","Take free medkit");
 }else if(room==SC_ROOM_GUILD){
  SC_PUSH("KEI",EXPLORERS,SC_ACT_TALK,-1,-1,0,0,"When you are ready, open Tracked Mission.","Ask about Ryn");
  SC_PUSH("SURVEYOR",EXPLORERS,SC_ACT_QUEST,-1,-1,600,0,"Fresh surface sample pays. Log it honest.","Accept survey tip");
 }else if(room==SC_ROOM_LOCK){
  SC_PUSH("CUSTOMS",LAW,SC_ACT_TALK,-1,-1,0,0,"Restricted goods still raise warrants.","Ask about law");
 }
 #undef SC_PUSH
 return n;
}
static const char *sc_room_name(int t){
 static const char *n[]={"CORRIDOR","ARRIVALS","CHANDLERY","CANTEEN","CARGO BAY","CLINIC","GUILD DESK","CUSTOMS"};
 return t>=0&&t<8?n[t]:"ROOM";
}
static void sc_build_map(void){
 if(sc_built_for==game.system)return;
 sc_built_for=game.system; memset(sc_map,0,sizeof(sc_map)); memset(sc_door_n,0,sizeof(sc_door_n)); memset(sc_door_e,0,sizeof(sc_door_e));
 /* Fixed layout with economy flavour on shop/clinic placement. */
 static const unsigned char layout[SC_H][SC_W]={
  {SC_ROOM_LOCK,SC_ROOM_HUB,SC_ROOM_SHOP,SC_ROOM_BAR,SC_ROOM_BAY},
  {SC_ROOM_EMPTY,SC_ROOM_HUB,SC_ROOM_EMPTY,SC_ROOM_EMPTY,SC_ROOM_CLINIC},
  {SC_ROOM_GUILD,SC_ROOM_EMPTY,SC_ROOM_SHOP,SC_ROOM_BAR,SC_ROOM_EMPTY},
  {SC_ROOM_EMPTY,SC_ROOM_BAY,SC_ROOM_EMPTY,SC_ROOM_LOCK,SC_ROOM_HUB}
 };
 memcpy(sc_map,layout,sizeof(layout));
 /* Open a connected grid: every cell links to east/south when both exist. */
 for(int y=0;y<SC_H;y++)for(int x=0;x<SC_W;x++){
  if(x+1<SC_W)sc_door_e[y][x]=1;
  if(y>0)sc_door_n[y][x]=1;
 }
 /* Agricultural economies emphasise bay/bar; industrial emphasise shop. */
 if(game.systems[game.system].economy>=4){sc_map[0][3]=SC_ROOM_BAR;sc_map[2][3]=SC_ROOM_BAY;}
 else {sc_map[0][2]=SC_ROOM_SHOP;sc_map[2][2]=SC_ROOM_SHOP;}
 sc_x=1;sc_y=0;sc_face=SC_S;sc_menu=0;sc_choice=0;
}
static int sc_door_ahead(void){
 if(sc_face==SC_N)return sc_y>0&&sc_door_n[sc_y][sc_x];
 if(sc_face==SC_S)return sc_y+1<SC_H&&sc_door_n[sc_y+1][sc_x];
 if(sc_face==SC_E)return sc_x+1<SC_W&&sc_door_e[sc_y][sc_x];
 return sc_x>0&&sc_door_e[sc_y][sc_x-1];
}
static void sc_step(int dir){ /* dir +1 forward, -1 back */
 int f=sc_face; if(dir<0)f=(f+2)&3;
 int nx=sc_x,ny=sc_y;
 if(f==SC_N){if(sc_y>0&&sc_door_n[sc_y][sc_x])ny--;else return;}
 else if(f==SC_S){if(sc_y+1<SC_H&&sc_door_n[sc_y+1][sc_x])ny++;else return;}
 else if(f==SC_E){if(sc_x+1<SC_W&&sc_door_e[sc_y][sc_x])nx++;else return;}
 else {if(sc_x>0&&sc_door_e[sc_y][sc_x-1])nx--;else return;}
 sc_x=nx;sc_y=ny;sc_menu=0;game.cue=SFX_UI;
}
static void sc_draw_fp(void){
 /* NES-style corridor: left/right walls taper, floor/ceiling, door or wall ahead. */
 rect(0,0,W,H,RGB(8,12,18));
 int mid=118;
 /* ceiling */
 for(int y=28;y<mid;y++){float t=(y-28)/(float)(mid-28);int inset=(int)(t*90);rect(inset,y,W-2*inset,1,mix_rgb(RGB(40,52,64),RGB(14,20,28),t));}
 /* floor */
 for(int y=mid;y<248;y++){float t=(y-mid)/(float)(248-mid);int inset=(int)((1.f-t)*90);rect(inset,y,W-2*inset,1,mix_rgb(RGB(22,30,40),RGB(10,14,20),t));}
 /* left wall */
 for(int i=0;i<10;i++){int x0=i*9,x1=(i+1)*9;int y0=28+i*9,y1=248-i*9;rect(x0,y0,x1-x0+2,y1-y0,RGB(28+i*2,38+i,48+i));}
 /* right wall */
 for(int i=0;i<10;i++){int x0=W-(i+1)*9,x1=W-i*9;int y0=28+i*9,y1=248-i*9;rect(x0,y0,x1-x0+2,y1-y0,RGB(28+i*2,38+i,48+i));}
 int door=sc_door_ahead();
 if(door){
  rect(170,70,140,120,RGB(6,10,16));
  rect(178,78,124,104,RGB(18,36,44));
  rect(230,110,20,48,GOLD);
  text(24,12,CYAN,"DOOR");
 }else{
  rect(160,60,160,140,RGB(32,44,56));
  text(23,12,DIM,"BULKHEAD");
 }
 /* NPC silhouettes in room */
 ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
 for(int i=0;i<pn;i++){
  int sx=200+i*40,sy=150; unsigned ink=faction_colors[people[i].role];
  rect(sx,sy-40,28,40,RGB(20,30,40)); rect(sx+6,sy-40,16,14,ink); pixel(sx+14,sy-34,WHITE);
 }
}
static void sc_draw_minimap(void){
 int ox=368,oy=36,cs=14;
 rect(ox-4,oy-4,SC_W*cs+8,SC_H*cs+8,RGB(6,14,20));
 rect(ox-4,oy-4,SC_W*cs+8,1,CYAN); text(46,3,CYAN,"MAP");
 for(int y=0;y<SC_H;y++)for(int x=0;x<SC_W;x++){
  int px=ox+x*cs,py=oy+y*cs; unsigned c=RGB(16,24,32);
  if(sc_map[y][x]==SC_ROOM_HUB)c=RGB(40,90,100);
  else if(sc_map[y][x]==SC_ROOM_SHOP)c=RGB(90,70,30);
  else if(sc_map[y][x]==SC_ROOM_BAR)c=RGB(70,40,80);
  else if(sc_map[y][x]==SC_ROOM_GUILD)c=RGB(30,80,60);
  else if(sc_map[y][x]==SC_ROOM_CLINIC)c=RGB(40,70,90);
  else if(sc_map[y][x]==SC_ROOM_BAY)c=RGB(60,50,30);
  else if(sc_map[y][x]==SC_ROOM_LOCK)c=RGB(80,40,40);
  rect(px,py,cs-2,cs-2,c);
  if(x==sc_x&&y==sc_y){rect(px+2,py+2,cs-6,cs-6,GOLD);
   /* facing tick */
   if(sc_face==SC_N)rect(px+cs/2-1,py+1,2,3,WHITE);
   if(sc_face==SC_S)rect(px+cs/2-1,py+cs-5,2,3,WHITE);
   if(sc_face==SC_E)rect(px+cs-5,py+cs/2-1,3,2,WHITE);
   if(sc_face==SC_WDIR)rect(px+1,py+cs/2-1,3,2,WHITE);
  }
 }
}
static void sc_draw_ui(void){
 sc_build_map();
 sc_draw_fp();
 rect(0,0,W,28,RGB(8,19,28)); rect(0,26,W,2,GOLD);
 text(1,0,GOLD,"STATION / %.16s",station_name(&game));
 text(1,2,DIM,"D-PAD TURN/MOVE  X ACT  O DECK");
 text(28,2,CYAN,"%.12s",sc_room_name(sc_map[sc_y][sc_x]));
 sc_draw_minimap();
 rect(0,248,W,24,RGB(8,19,28)); rect(0,248,W,2,CYAN);
 ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  text(1,31,GOLD,"CHANDLER STOCK");
  if(!ln)text(18,31,DIM,"Sold out today.");
  else {int idx=list[sc_shop_row%ln]; text(18,31,WHITE,"%.14s  %.1fU",equipment_list_names[idx],equipment_costs[idx]*.1f);}
  text(1,32,DIM,"UP/DOWN pick   X buy   TRI back");
 }else if(sc_menu==SC_MENU_PERSON&&pn>0){
  if(sc_choice<0)sc_choice=0; if(sc_choice>=pn)sc_choice=pn-1;
  ScNpc *p=&people[sc_choice];
  text(1,31,GOLD,"%.10s",p->name); text(14,31,WHITE,"%.36s",p->line);
  text(1,32,AMBER,"> %.40s",p->offer);
 }else if(pn>0){
  text(1,31,GOLD,"%d here",pn); text(12,31,WHITE,"%.40s",people[0].line);
  text(1,32,DIM,"X talk / trade   face a door to walk");
 }else text(1,31,DIM,"Empty deck. Face a door and press UP.");
 if(game.passenger_dest>=0)text(40,0,CYAN,"PAX");
}
static void sc_do_act(void){
 ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  if(!ln){message(&game,"Chandler is empty.");return;}
  if(sc_shop_row<0)sc_shop_row=0; if(sc_shop_row>=ln)sc_shop_row=ln-1;
  buy_equipment(list[sc_shop_row]); return;
 }
 if(!pn){message(&game,"Nobody here. Move to another room.");return;}
 if(sc_menu!=SC_MENU_PERSON){sc_menu=SC_MENU_PERSON;sc_choice=0;return;}
 if(sc_choice<0||sc_choice>=pn)sc_choice=0;
 ScNpc *p=&people[sc_choice];
 speak(&game,p->role==LAW?VOICE_LAW:p->role==EXPLORERS?VOICE_KEI:!strcmp(p->name,"VENN")?VOICE_VENN:VOICE_DOCK,p->line);
 if(p->act==SC_ACT_TALK){message(&game,p->line);game.cue=SFX_UI;}
 else if(p->act==SC_ACT_SHOP){sc_menu=SC_MENU_SHOP;sc_shop_row=0;message(&game,"Chandler stock — exclusive to this deck.");}
 else if(p->act==SC_ACT_GIFT){
  int bit=p->gift_bit>=0?p->gift_bit:0;
  if(game.gift_flags&(1u<<bit)){message(&game,"Already took that gift.");return;}
  game.gift_flags|=1u<<bit;
  if(bit==0){if(!(game.upgrades&8)){game.upgrades|=8;message(&game,"Free cargo clamp fitted (+8t).");}else {game.credits+=200;message(&game,"Clamp spare sold for 20 U.");}}
  else {game.energy=100;message(&game,"Medkit used — energy restored.");}
  game.cue=SFX_UI;
 }else if(p->act==SC_ACT_QUEST){
  game.credits+=p->quest_pay; game.discoveries++; {char note[64];snprintf(note,sizeof(note),"Side tip paid %.1f U.",p->quest_pay*.1f);message(&game,note);} game.cue=SFX_SELECT;
 }else if(p->act==SC_ACT_TAXI){
  if(game.passenger_dest>=0){message(&game,"Cabin already holds a passenger.");return;}
  if(cargo_used(&game)>=cargo_capacity(&game)){message(&game,"Need 1t free for a passenger.");return;}
  int dest=(game.system*17+sc_x*3+11)&255; if(dest==game.system)dest=(dest+5)&255;
  game.passenger_dest=dest; game.passenger_kind=1; game.passenger_pay=p->taxi_pay;
  {char note[80];snprintf(note,sizeof(note),"Passenger booked for %s.",game.systems[dest].name);message(&game,note);}
  speak(&game,VOICE_CONTACT,"Thanks. I'll talk your ear off."); game.cue=SFX_UI;
 }
}
static int sc_input(unsigned pressed){
 sc_build_map();
 if(sc_menu==SC_MENU_SHOP){
  if(pressed&PSP_CTRL_UP){sc_shop_row--;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_DOWN){sc_shop_row++;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_TRIANGLE){sc_menu=SC_MENU_PERSON;return 1;}
  if(pressed&PSP_CTRL_CROSS){sc_do_act();return 1;}
  if(pressed&PSP_CTRL_CIRCLE){sc_menu=0;return 1;}
  return 1;
 }
 if(sc_menu==SC_MENU_PERSON){
  ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
  if(pressed&PSP_CTRL_UP){sc_choice=(sc_choice+pn-1)%(pn?pn:1);game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_DOWN){sc_choice=(sc_choice+1)%(pn?pn:1);game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_CROSS){sc_do_act();return 1;}
  if(pressed&PSP_CTRL_CIRCLE||pressed&PSP_CTRL_TRIANGLE){sc_menu=0;return 1;}
  return 1;
 }
 if(pressed&PSP_CTRL_LEFT){sc_face=(sc_face+3)&3;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_RIGHT){sc_face=(sc_face+1)&3;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_UP)sc_step(1);
 if(pressed&PSP_CTRL_DOWN)sc_step(-1);
 if(pressed&PSP_CTRL_CROSS)sc_do_act();
 return 0;
}
