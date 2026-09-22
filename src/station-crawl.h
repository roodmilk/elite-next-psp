/* First-person NES dungeon-crawler station concourse + NPC services. */
enum { SC_W=5, SC_H=4, SC_N=0, SC_E=1, SC_S=2, SC_WDIR=3 };
enum { SC_ROOM_EMPTY=0, SC_ROOM_HUB, SC_ROOM_SHOP, SC_ROOM_BAR, SC_ROOM_BAY, SC_ROOM_CLINIC, SC_ROOM_GUILD, SC_ROOM_LOCK };
enum { SC_ACT_TALK=0, SC_ACT_SHOP, SC_ACT_GIFT, SC_ACT_QUEST, SC_ACT_TAXI, SC_ACT_BOARD };
enum { SC_MENU_NONE=0, SC_MENU_PERSON, SC_MENU_SHOP };
static int sc_x=2,sc_y=0,sc_face=SC_N,sc_menu=0,sc_choice=0,sc_shop_row=0,sc_built_for=-1,sc_bob=0;
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
static int sc_door_dir(int dir){
 if(dir==SC_N)return sc_y>0&&sc_door_n[sc_y][sc_x];
 if(dir==SC_S)return sc_y+1<SC_H&&sc_door_n[sc_y+1][sc_x];
 if(dir==SC_E)return sc_x+1<SC_W&&sc_door_e[sc_y][sc_x];
 return sc_x>0&&sc_door_e[sc_y][sc_x-1];
}
static int sc_door_ahead(void){return sc_door_dir(sc_face);}
static int sc_nbr(int dir,int *ox,int *oy){
 int x=sc_x,y=sc_y;
 if(dir==SC_N){if(y<=0)return -1;y--;}
 else if(dir==SC_S){if(y+1>=SC_H)return -1;y++;}
 else if(dir==SC_E){if(x+1>=SC_W)return -1;x++;}
 else {if(x<=0)return -1;x--;}
 if(ox)*ox=x;if(oy)*oy=y;return sc_map[y][x];
}
static unsigned sc_room_tint(int room){
 if(room==SC_ROOM_SHOP)return RGB(52,42,28);
 if(room==SC_ROOM_BAR)return RGB(48,28,52);
 if(room==SC_ROOM_GUILD)return RGB(24,52,44);
 if(room==SC_ROOM_CLINIC)return RGB(32,52,64);
 if(room==SC_ROOM_LOCK)return RGB(56,32,32);
 if(room==SC_ROOM_BAY)return RGB(44,40,28);
 if(room==SC_ROOM_HUB)return RGB(28,44,54);
 return RGB(36,48,58);
}
static unsigned sc_room_tint2(int room){
 if(room==SC_ROOM_SHOP)return RGB(40,32,20);
 if(room==SC_ROOM_BAR)return RGB(34,18,40);
 if(room==SC_ROOM_GUILD)return RGB(16,36,30);
 if(room==SC_ROOM_CLINIC)return RGB(22,38,50);
 if(room==SC_ROOM_LOCK)return RGB(40,22,22);
 if(room==SC_ROOM_BAY)return RGB(30,28,18);
 if(room==SC_ROOM_HUB)return RGB(18,32,40);
 return RGB(28,38,48);
}
static unsigned sc_room_trim(int room){
 if(room==SC_ROOM_SHOP)return RGB(120,90,40);
 if(room==SC_ROOM_BAR)return RGB(120,70,130);
 if(room==SC_ROOM_GUILD)return RGB(60,140,110);
 if(room==SC_ROOM_CLINIC)return RGB(90,160,180);
 if(room==SC_ROOM_LOCK)return RGB(140,70,70);
 if(room==SC_ROOM_BAY)return RGB(110,100,50);
 if(room==SC_ROOM_HUB)return RGB(70,130,150);
 return RGB(70,92,104);
}
static void sc_step(int dir){ /* dir +1 forward, -1 back */
 int f=sc_face; if(dir<0)f=(f+2)&3;
 int nx=sc_x,ny=sc_y;
 if(f==SC_N){if(sc_y>0&&sc_door_n[sc_y][sc_x])ny--;else return;}
 else if(f==SC_S){if(sc_y+1<SC_H&&sc_door_n[sc_y+1][sc_x])ny++;else return;}
 else if(f==SC_E){if(sc_x+1<SC_W&&sc_door_e[sc_y][sc_x])nx++;else return;}
 else {if(sc_x>0&&sc_door_e[sc_y][sc_x-1])nx--;else return;}
 sc_x=nx;sc_y=ny;sc_menu=0;sc_bob=6;game.cue=SFX_UI;
}
/* OpenEnroth-inspired corridor frame: depth layers, side portals, tile floor.
 * Grid-step crawler (PSP-safe) — no enemies, station interactions only. */
static void sc_wall_column(int x0,int x1,int y0,int y1,unsigned wall,unsigned wall2,unsigned trim,int seam){
 if(x1<=x0||y1<=y0)return;
 rect(x0,y0,x1-x0,y1-y0,seam?wall2:wall);
 if(seam){rect(x0,y0+12,2,y1-y0-24,trim);rect(x1-2,y0+12,2,y1-y0-24,trim);}
 /* Panel rivets */
 for(int y=y0+18;y<y1-18;y+=22){pixel(x0+3,y,trim);pixel(x1-4,y,trim);}
}
static void sc_draw_side_portal(int left,int depth,int has_door,int nbr_room,unsigned wall,unsigned trim){
 /* depth 0=near, 1=mid — portals open into neighbour tint like Enroth doorways. */
 int inset=18+depth*42,top=48+depth*28,bot=232-depth*28,w=34-depth*8;
 if(w<16)w=16;
 int x=left?inset:W-inset-w;
 unsigned dark=RGB(8,12,18),inner=has_door?sc_room_tint2(nbr_room):wall;
 rect(x,top,w,bot-top,dark);
 if(has_door){
  rect(x+3,top+8,w-6,bot-top-16,inner);
  rect(x+6,top+14,w-12,bot-top-28,mix_rgb(inner,RGB(4,8,12),.35f));
  rect(x+(left?w-5:2),top+20,3,bot-top-40,trim);
  /* Floor lip into the side room */
  rect(x+4,bot-18,w-8,6,mix_rgb(inner,RGB(20,24,28),.4f));
 }else{
  for(int k=0;k<4;k++)rect(x+4,top+16+k*28,w-8,2,mix_rgb(wall,RGB(10,14,18),.3f));
 }
}
static void sc_draw_fp(void){
 rect(0,0,W,H,RGB(4,8,14));
 int mid=120+sc_bob/2,room=sc_map[sc_y][sc_x];
 unsigned wall=sc_room_tint(room),wall2=sc_room_tint2(room),trim=sc_room_trim(room);
 int left_dir=(sc_face+3)&3,right_dir=(sc_face+1)&3;
 int door_l=sc_door_dir(left_dir),door_r=sc_door_dir(right_dir),door_f=sc_door_ahead();
 int nlx,nly,nrx,nry,nfx,nfy;
 int nbr_l=door_l?sc_nbr(left_dir,&nlx,&nly):-1;
 int nbr_r=door_r?sc_nbr(right_dir,&nrx,&nry):-1;
 int nbr_f=door_f?sc_nbr(sc_face,&nfx,&nfy):-1;
 /* Ceiling vault with cross-beams (indoor BLV feel, flat raster) */
 for(int y=28;y<mid;y++){
  float t=(y-28)/(float)(mid-28);int inset=(int)(t*108);
  unsigned c=mix_rgb(RGB(54,64,78),RGB(10,14,20),t);
  rect(inset,y,W-2*inset,1,c);
  if(((y*2)&15)==0)rect(inset+6,y,W-2*inset-12,1,mix_rgb(c,trim,.25f));
 }
 /* Beam ribs converging to vanishing point */
 for(int i=0;i<5;i++){int x=40+i*100;line(x,32,240,mid-2,mix_rgb(trim,RGB(20,24,30),.55f));}
 /* Floor: foreshortened tile grid */
 for(int y=mid;y<248;y++){
  float t=(y-mid)/(float)(248-mid);int inset=(int)((1.f-t)*108);
  unsigned c=mix_rgb(RGB(22,30,40),RGB(6,8,12),t*.85f);
  if(((y/3+sc_x)&1)==0)c=mix_rgb(c,trim,.08f);
  rect(inset,y,W-2*inset,1,c);
 }
 for(int i=0;i<=6;i++){
  float t=i/6.f;int inset=(int)((1.f-t)*108);int y=mid+(int)(t*(248-mid));
  line(inset,y,W-inset,y,RGB(12,16,22));
  int span=(W-2*inset)/4;for(int k=1;k<4;k++){int x=inset+k*span;line(x,y,240+(x-240)/4,mid,RGB(10,14,18));}
 }
 /* Near wall columns (depth 0) — heavy Enroth panel framing */
 for(int i=0;i<5;i++){
  int x0=i*10,x1=(i+1)*10;int y0=30+i*10,y1=246-i*10;
  sc_wall_column(x0,x1,y0,y1,wall,wall2,trim,(i%2)==0);
  sc_wall_column(W-x1,W-x0,y0,y1,wall,wall2,trim,(i%2)==0);
 }
 /* Mid wall columns (depth 1) */
 for(int i=5;i<9;i++){
  int x0=i*12,x1=(i+1)*12;int y0=36+i*8,y1=240-i*8;
  sc_wall_column(x0,x1,y0,y1,wall2,wall,trim,(i%3)==0);
  sc_wall_column(W-x1,W-x0,y0,y1,wall2,wall,trim,(i%3)==0);
 }
 /* Facing-relative side portals (fixed: left/right track turn facing) */
 sc_draw_side_portal(1,0,door_l,nbr_l,wall,trim);
 sc_draw_side_portal(0,0,door_r,nbr_r,wall,trim);
 sc_draw_side_portal(1,1,door_l,nbr_l,wall,trim);
 sc_draw_side_portal(0,1,door_r,nbr_r,wall,trim);
 /* Far wall / passage — portal into next cell tint */
 if(door_f){
  unsigned far=sc_room_tint(nbr_f),far2=sc_room_tint2(nbr_f),ftrim=sc_room_trim(nbr_f);
  rect(148,56,184,148,RGB(2,6,10));
  rect(158,64,164,132,far2);
  rect(170,76,140,112,far);
  rect(182,88,116,92,mix_rgb(far,RGB(4,8,12),.4f));
  /* Arch jambs */
  rect(158,64,10,132,trim);rect(312,64,10,132,trim);
  rect(158,64,164,8,trim);rect(158,188,164,8,trim);
  /* Door plate + latch */
  rect(222,120,36,56,mix_rgb(ftrim,GOLD,.35f));
  rect(234,138,8,8,RGB(40,30,10));
  rect(168,196,144,8,mix_rgb(far2,RGB(30,34,40),.3f));
  text(23,9,CYAN,"PASSAGE");
  /* Tiny prop glimpse from neighbour type */
  if(nbr_f==SC_ROOM_SHOP){rect(200,150,20,16,RGB(70,50,28));rect(260,150,20,16,RGB(70,50,28));}
  if(nbr_f==SC_ROOM_BAR){rect(210,160,60,12,RGB(60,30,50));}
 }else{
  rect(150,52,180,156,wall);
  for(int k=0;k<7;k++)rect(162,66+k*18,156,2,wall2);
  rect(198,96,84,64,RGB(14,20,26));
  rect(210,108,60,40,RGB(10,14,18));
  text(22,9,DIM,"BULKHEAD");
 }
 /* Ceiling lamp / glow */
 circle(240,44-sc_bob/3,6,trim);rect(237,36-sc_bob/3,6,8,GOLD);
 rect(232,50-sc_bob/3,16,2,mix_rgb(trim,RGB(255,220,120),.4f));
 /* Room props — denser furniture silhouettes */
 if(room==SC_ROOM_SHOP){
  rect(64,148,56,44,RGB(60,45,25));rect(70,140,44,10,RGB(90,70,40));
  rect(360,148,56,44,RGB(60,45,25));rect(366,140,44,10,RGB(90,70,40));
  rect(200,170,80,20,RGB(50,38,22));
 }else if(room==SC_ROOM_BAR){
  rect(90,168,300,28,RGB(50,30,40));rect(100,156,36,12,RGB(90,60,50));
  rect(340,156,36,12,RGB(90,60,50));rect(200,150,80,14,RGB(70,40,55));
 }else if(room==SC_ROOM_HUB){
  rect(204,136,72,56,RGB(30,50,60));rect(216,124,48,12,CYAN);
  rect(220,148,40,8,RGB(50,90,100));
 }else if(room==SC_ROOM_GUILD){
  rect(190,140,100,40,RGB(20,60,48));rect(210,128,60,12,RGB(60,140,110));
 }else if(room==SC_ROOM_CLINIC){
  rect(180,150,120,36,RGB(40,70,80));rect(200,140,80,10,RGB(120,200,210));
 }else if(room==SC_ROOM_BAY){
  rect(70,160,50,36,RGB(70,60,30));rect(360,160,50,36,RGB(70,60,30));
  rect(160,180,160,16,RGB(50,45,25));
 }else if(room==SC_ROOM_LOCK){
  rect(200,140,80,50,RGB(70,35,35));rect(220,150,40,20,RGB(20,20,24));
 }
 /* NPC silhouettes mid-corridor */
 ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
 for(int i=0;i<pn;i++){
  int sx=186+i*52,sy=158; unsigned ink=faction_colors[people[i].role];
  rect(sx,sy-52,34,52,RGB(12,18,26));
  rect(sx+3,sy-50,28,48,RGB(20,30,40));
  rect(sx+8,sy-50,18,16,ink);
  pixel(sx+12,sy-44,WHITE);pixel(sx+18,sy-44,WHITE);
  rect(sx+10,sy-30,14,22,ink);
  rect(sx+6,sy-8,22,8,mix_rgb(ink,RGB(10,14,18),.4f));
  if(sc_menu==SC_MENU_PERSON&&i==sc_choice)rect(sx-2,sy-54,38,2,GOLD);
 }
 if(sc_bob>0)sc_bob--;
}
static void sc_draw_minimap(void){
 int ox=368,oy=36,cs=14;
 rect(ox-4,oy-4,SC_W*cs+8,SC_H*cs+8,RGB(6,14,20));
 rect(ox-4,oy-4,SC_W*cs+8,1,CYAN); text(46,3,CYAN,"MAP");
 /* Door links first so rooms sit on top */
 for(int y=0;y<SC_H;y++)for(int x=0;x<SC_W;x++){
  int px=ox+x*cs+cs/2-1,py=oy+y*cs+cs/2-1;
  if(x+1<SC_W&&sc_door_e[y][x])rect(px,py,cs,2,RGB(40,60,70));
  if(y>0&&sc_door_n[y][x])rect(px,py-cs,2,cs,RGB(40,60,70));
 }
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
 text(28,0,CYAN,"%.12s",sc_room_name(sc_map[sc_y][sc_x]));
 {const char *d[]={"N","E","S","W"};text(42,0,WHITE,"[%s]",d[sc_face&3]);
  if(sc_door_ahead())text(46,0,CYAN,"DOOR");else text(46,0,DIM,"WALL");}
 text(1,2,DIM,"LEFT/RIGHT TURN   UP MOVE   X ACT   O DECK");
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
  text(1,31,GOLD,"%.10s",p->name); text_wrap(14,31,44,1,WHITE,p->line,0);
  text(1,32,AMBER,">"); text_wrap(3,32,55,1,AMBER,p->offer,0);
 }else if(pn>0){
  text(1,31,GOLD,"%d here",pn); text_wrap(12,31,46,1,WHITE,people[0].line,0);
  text(1,32,DIM,"X talk / trade   face a door to walk");
 }else text(1,31,DIM,"Empty deck. Face a door and press UP.");
 if(game.passenger_dest>=0)text(50,2,CYAN,"PAX");
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
 else if(p->act==SC_ACT_SHOP){sc_menu=SC_MENU_SHOP;sc_shop_row=0;message(&game,"Chandler stock - exclusive to this deck.");}
 else if(p->act==SC_ACT_GIFT){
  int bit=p->gift_bit>=0?p->gift_bit:0;
  if(game.gift_flags&(1u<<bit)){message(&game,"Already took that gift.");return;}
  game.gift_flags|=1u<<bit;
  if(bit==0){if(!(game.upgrades&8)){game.upgrades|=8;message(&game,"Free cargo clamp fitted (+8t).");}else {game.credits+=200;message(&game,"Clamp spare sold for 20 U.");}}
  else {game.energy=100;message(&game,"Medkit used - energy restored.");}
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
