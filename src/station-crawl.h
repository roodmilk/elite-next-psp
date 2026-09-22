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
  SC_PUSH("BARTEND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Meridian tips well. Truth tips better.","Ask for rumours");
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
/* MM6-style 3-depth station crawl: riveted panels, labeled doors, portrait NPCs.
 * Soft-FB only — squeeze clarity from rects/pixels. No enemies; talk/shop/gift/taxi. */
static void sc_panel(int x0,int y0,int x1,int y1,unsigned wall,unsigned wall2,unsigned trim,int lit){
 if(x1<=x0+2||y1<=y0+2)return;
 int w=x1-x0,h=y1-y0;
 rect(x0,y0,w,h,wall);
 rect(x0,y0,w,1,mix_rgb(wall,RGB(90,110,130),.35f));
 rect(x0,y1-1,w,1,mix_rgb(wall,RGB(4,6,10),.45f));
 rect(x0,y0,1,h,mix_rgb(wall,RGB(70,90,110),.25f));
 rect(x1-1,y0,1,h,mix_rgb(wall,RGB(4,6,10),.4f));
 /* Horizontal plate seams */
 for(int y=y0+10;y<y1-8;y+=14)rect(x0+2,y,w-4,1,wall2);
 /* Rivets */
 for(int y=y0+8;y<y1-6;y+=16){pixel(x0+3,y,trim);pixel(x1-4,y,trim);}
 if(lit){rect(x0+(w>20?w/2-1:2),y0+6,2,h-12,trim);pixel(x0+(w>20?w/2:3),y0+4,GOLD);}
}
static const char *sc_room_short(int t){
 static const char *n[]={"HALL","HUB","SHOP","BAR","BAY","MED","GUILD","LOCK"};
 return t>=0&&t<8?n[t]:"ROOM";
}
static void sc_door_label(int x,int y,int w,unsigned ink,const char *label){
 /* Pixel-backed strip so side labels stay crisp and never fight the far door text. */
 if(w<20||y<32||y>220)return;
 int len=0;while(label[len]&&len<5)len++;
 int tw=len*8;if(tw>w-6){len=(w-6)/8;if(len<1)return;tw=len*8;}
 int lx=x+(w-tw)/2;
 /* Keep text columns inside the safe 1..54 band so right alcoves never clip. */
 if(lx<8)lx=8;if(lx+tw>W-16)lx=W-16-tw;if(lx<x+2)lx=x+2;
 rect(lx-2,y,tw+4,10,RGB(4,10,16));
 rect(lx-2,y,tw+4,1,ink);
 text(lx/8,y/8,CYAN,"%.*s",len,label);
}
static void sc_draw_side_door(int left,int depth,int has_door,int nbr_room,unsigned wall,unsigned wall2,unsigned trim){
 /* depth 0 near, 1 mid — classic MM side alcove / doorway. */
 int inset=14+depth*46,top=42+depth*26,bot=236-depth*26,w=40-depth*10;
 if(w<18)w=18;
 int x=left?inset:W-inset-w;
 unsigned inner=has_door?sc_room_tint(nbr_room):wall2;
 /* Outer frame */
 rect(x-2,top-4,w+4,bot-top+8,RGB(6,10,16));
 sc_panel(x-2,top-4,x+w+2,bot+4,wall,wall2,trim,0);
 if(has_door){
  unsigned far2=sc_room_tint2(nbr_room),ftrim=sc_room_trim(nbr_room);
  rect(x,top,w,bot-top,RGB(4,8,12));
  rect(x+3,top+6,w-6,bot-top-14,inner);
  rect(x+6,top+12,w-12,bot-top-28,far2);
  /* Door plate */
  rect(x+w/2-4,top+28,8,bot-top-56,mix_rgb(ftrim,GOLD,.3f));
  rect(x+(left?w-7:3),top+36,3,10,GOLD);
  /* Threshold lip */
  rect(x+2,bot-12,w-4,6,mix_rgb(inner,RGB(40,48,56),.35f));
  /* Label only the near alcove — mid depth is visual only */
  if(depth==0)sc_door_label(x,top+2,w,ftrim,sc_room_short(nbr_room));
 }else{
  sc_panel(x,top,x+w,bot,wall2,wall,trim,1);
  for(int k=0;k<3;k++)rect(x+6,top+18+k*36,w-12,3,mix_rgb(wall,RGB(8,12,16),.35f));
 }
}
static void sc_draw_far_door(int nbr_room){
 unsigned far=sc_room_tint(nbr_room),far2=sc_room_tint2(nbr_room),ftrim=sc_room_trim(nbr_room);
 int x0=152,y0=58,ww=176,hh=140;
 /* Recessed arch */
 rect(x0-6,y0-8,ww+12,hh+16,RGB(2,6,12));
 rect(x0-4,y0-6,ww+8,hh+12,mix_rgb(ftrim,RGB(12,18,24),.55f));
 rect(x0,y0,ww,hh,RGB(4,8,14));
 /* Depth layers into neighbour tint */
 rect(x0+10,y0+10,ww-20,hh-24,far2);
 rect(x0+22,y0+22,ww-44,hh-48,far);
 rect(x0+34,y0+34,ww-68,hh-72,mix_rgb(far,RGB(4,8,12),.4f));
 /* Heavy jambs */
 rect(x0,y0,12,hh,ftrim);rect(x0+ww-12,y0,12,hh,ftrim);
 rect(x0,y0,ww,10,ftrim);rect(x0,y0+hh-10,ww,10,ftrim);
 rect(x0+12,y0+10,4,hh-20,mix_rgb(ftrim,GOLD,.25f));
 rect(x0+ww-16,y0+10,4,hh-20,mix_rgb(ftrim,GOLD,.25f));
 /* Latch plate */
 rect(x0+ww/2-14,y0+hh/2-22,28,48,mix_rgb(ftrim,GOLD,.4f));
 rect(x0+ww/2-4,y0+hh/2-4,8,8,RGB(30,24,12));
 rect(x0+ww/2+6,y0+hh/2+2,4,6,GOLD);
 /* Threshold */
 rect(x0+16,y0+hh-16,ww-32,8,mix_rgb(far2,RGB(50,58,68),.35f));
 /* Big destination label */
 rect(x0+36,y0+6,ww-72,12,RGB(6,14,22));
 rect(x0+36,y0+6,ww-72,1,CYAN);
 {char tip[24];snprintf(tip,sizeof(tip),"TO %s",sc_room_short(nbr_room));
  int len=(int)strlen(tip);int tw=len*8;int lx=x0+(ww-tw)/2;if(lx<x0+40)lx=x0+40;
  text(lx/8,(y0+7)/8,CYAN,"%s",tip);}
 /* Neighbour prop silhouette */
 if(nbr_room==SC_ROOM_SHOP){rect(x0+40,y0+90,22,18,RGB(80,58,30));rect(x0+ww-62,y0+90,22,18,RGB(80,58,30));}
 else if(nbr_room==SC_ROOM_BAR){rect(x0+50,y0+100,76,12,RGB(70,36,58));}
 else if(nbr_room==SC_ROOM_HUB){rect(x0+ww/2-16,y0+70,32,28,CYAN);}
 else if(nbr_room==SC_ROOM_CLINIC){rect(x0+ww/2-20,y0+80,40,16,RGB(100,180,190));}
 else if(nbr_room==SC_ROOM_GUILD){rect(x0+ww/2-24,y0+78,48,20,RGB(40,110,90));}
 else if(nbr_room==SC_ROOM_BAY){rect(x0+36,y0+96,28,20,RGB(90,78,40));rect(x0+ww-64,y0+96,28,20,RGB(90,78,40));}
 else if(nbr_room==SC_ROOM_LOCK){rect(x0+ww/2-18,y0+74,36,30,RGB(90,40,40));}
}
static void sc_draw_bulkhead(unsigned wall,unsigned wall2,unsigned trim){
 int x0=148,y0=52,ww=184,hh=156;
 sc_panel(x0,y0,x0+ww,y0+hh,wall,wall2,trim,0);
 for(int k=0;k<6;k++)rect(x0+14,y0+18+k*22,ww-28,3,wall2);
 /* View slit / sealed hatch */
 rect(x0+48,y0+48,ww-96,52,RGB(8,12,18));
 rect(x0+56,y0+56,ww-112,36,RGB(4,8,12));
 rect(x0+ww/2-10,y0+70,20,12,trim);
 rect(x0+40,y0+4,ww-80,12,RGB(8,14,20));
 text(22,7,DIM,"BULKHEAD");
}
static void sc_draw_props(int room,unsigned trim){
 if(room==SC_ROOM_SHOP){
  /* Stock crates + counter */
  rect(52,150,58,46,RGB(72,52,28));rect(58,142,46,10,RGB(110,84,44));
  rect(62,156,14,12,GOLD);rect(82,160,18,10,CYAN);
  rect(370,150,58,46,RGB(72,52,28));rect(376,142,46,10,RGB(110,84,44));
  rect(190,172,100,22,RGB(56,42,24));rect(198,166,84,8,trim);
  rect(210,176,16,10,RGB(40,30,18));rect(250,176,16,10,RGB(40,30,18));
 }else if(room==SC_ROOM_BAR){
  rect(80,170,320,30,RGB(58,32,46));rect(84,166,312,6,RGB(90,50,70));
  rect(96,152,40,16,RGB(100,68,54));rect(344,152,40,16,RGB(100,68,54));
  rect(200,148,80,16,RGB(80,44,60));rect(210,142,60,8,trim);
  for(int i=0;i<5;i++){rect(110+i*52,176,8,14,mix_rgb(trim,RGB(200,180,120),.35f));pixel(112+i*52,174,GOLD);}
 }else if(room==SC_ROOM_HUB){
  rect(198,128,84,62,RGB(28,54,66));rect(206,118,68,12,CYAN);
  rect(214,140,52,10,RGB(50,100,110));rect(222,156,36,20,RGB(18,36,44));
  rect(230,122,20,4,GOLD);rect(210,188,60,6,mix_rgb(trim,RGB(40,60,70),.4f));
 }else if(room==SC_ROOM_GUILD){
  rect(178,136,124,48,RGB(18,58,46));rect(198,124,84,14,RGB(50,130,100));
  rect(210,148,60,8,GOLD);rect(220,160,40,12,RGB(12,40,32));
  rect(186,128,8,8,CYAN);rect(286,128,8,8,CYAN);
 }else if(room==SC_ROOM_CLINIC){
  rect(170,148,140,40,RGB(36,68,78));rect(190,136,100,14,RGB(110,190,200));
  rect(200,156,30,18,WHITE);rect(250,156,30,18,RGB(180,220,230));
  rect(220,142,40,6,CYAN);
 }else if(room==SC_ROOM_BAY){
  rect(58,158,56,40,RGB(78,66,32));rect(66,150,40,10,RGB(110,96,48));
  rect(366,158,56,40,RGB(78,66,32));rect(374,150,40,10,RGB(110,96,48));
  rect(150,178,180,18,RGB(54,48,28));rect(160,172,160,8,trim);
  for(int i=0;i<4;i++)rect(170+i*40,184,24,8,RGB(40,36,20));
 }else if(room==SC_ROOM_LOCK){
  rect(190,132,100,58,RGB(78,36,36));rect(210,144,60,28,RGB(18,18,24));
  rect(230,152,20,12,RED);rect(200,124,80,10,RGB(100,50,50));
  rect(214,188,52,6,trim);
 }else{
  /* Corridor conduit boxes */
  rect(70,168,36,28,RGB(40,52,62));rect(374,168,36,28,RGB(40,52,62));
  rect(78,174,20,8,trim);rect(382,174,20,8,trim);
 }
}
static void sc_draw_npc(int i,const ScNpc *p,int selected){
 int feet=196,cx=168+i*72,bw=44,bh=58;
 unsigned ink=faction_colors[p->role%FACTION_COUNT];
 unsigned cloth=mix_rgb(ink,RGB(16,24,32),.45f),boot=mix_rgb(ink,RGB(8,10,14),.55f);
 int bob=(sc_bob>0&&i==0)?sc_bob/3:0;
 int top=feet-bh-bob;
 /* Selection halo */
 if(selected){rect(cx-bw/2-3,top-4,bw+6,2,GOLD);rect(cx-bw/2-3,feet+2,bw+6,2,GOLD);}
 /* Boots + legs */
 rect(cx-12,feet-10,10,10,boot);rect(cx+2,feet-10,10,10,boot);
 rect(cx-10,feet-28,8,18,cloth);rect(cx+2,feet-28,8,18,cloth);
 /* Torso */
 rect(cx-14,feet-48,28,22,cloth);
 rect(cx-14,feet-48,28,3,mix_rgb(ink,GOLD,.25f));
 /* Arms */
 rect(cx-18,feet-46,5,18,cloth);rect(cx+13,feet-46,5,18,cloth);
 /* Portrait head (pixel art) */
 {int hx=cx-16,hy=top+2,hs=32;
  rect(hx-1,hy-1,hs+2,hs+2,RGB(6,12,20));
  if(!strcmp(p->name,"KEI"))draw_kei(hx,hy,hs,0);
  else draw_portrait(hx,hy,hs,hs,game.system*37+i*91+p->role*13,p->role);
  rect(hx,hy+hs-1,hs,1,ink);
 }
 /* Name plate under feet */
 {int nw=(int)strlen(p->name);if(nw>9)nw=9;int px=cx-(nw*4);if(px<4)px=4;if(px+nw*8>W-4)px=W-4-nw*8;
  int py=feet+3;if(py>236)py=236;
  rect(px-2,py,nw*8+4,10,RGB(6,12,20));
  rect(px-2,py,nw*8+4,1,ink);
  text(px/8,py/8,selected?GOLD:WHITE,"%.9s",p->name);}
}
static void sc_draw_fp(void){
 rect(0,0,W,H,RGB(4,8,14));
 int mid=122+sc_bob/2,room=sc_map[sc_y][sc_x];
 unsigned wall=sc_room_tint(room),wall2=sc_room_tint2(room),trim=sc_room_trim(room);
 int left_dir=(sc_face+3)&3,right_dir=(sc_face+1)&3;
 int door_l=sc_door_dir(left_dir),door_r=sc_door_dir(right_dir),door_f=sc_door_ahead();
 int nlx,nly,nrx,nry,nfx,nfy;
 int nbr_l=door_l?sc_nbr(left_dir,&nlx,&nly):-1;
 int nbr_r=door_r?sc_nbr(right_dir,&nrx,&nry):-1;
 int nbr_f=door_f?sc_nbr(sc_face,&nfx,&nfy):-1;
 /* Ceiling vault — brighter near lamp, darker to walls */
 for(int y=28;y<mid;y++){
  float t=(y-28)/(float)(mid-28);int inset=(int)(t*118);
  unsigned c=mix_rgb(RGB(62,74,90),RGB(8,12,18),t);
  if(((y+sc_y)&7)==0)c=mix_rgb(c,trim,.18f);
  rect(inset,y,W-2*inset,1,c);
 }
 /* Converging beam ribs */
 for(int i=0;i<6;i++){int x=24+i*86;line(x,30,240,mid-1,mix_rgb(trim,RGB(16,20,28),.5f));}
 /* Ceiling pipes */
 rect(60,34,360,3,RGB(28,36,46));rect(100,40,280,2,RGB(22,30,40));
 /* Floor: high-contrast checker foreshortened (MM dungeon tile read) */
 for(int y=mid;y<248;y++){
  float t=(y-mid)/(float)(248-mid);int inset=(int)((1.f-t)*118);
  int span=W-2*inset;if(span<8)span=8;
  int cells=4+(int)(t*4);if(cells<4)cells=4;if(cells>8)cells=8;
  int cw=span/cells;if(cw<4)cw=4;
  for(int k=0;k<cells;k++){
   int x=inset+k*cw;int ww=(k==cells-1)?(W-inset-x):cw;
   int check=((k+(y/8)+sc_x+sc_y)&1);
   unsigned c=check?mix_rgb(RGB(42,54,68),trim,.22f):RGB(16,22,30);
   c=mix_rgb(c,RGB(6,8,12),t*.55f);
   rect(x,y,ww,1,c);
  }
 }
 /* Tile edge lines */
 for(int i=0;i<=7;i++){
  float t=i/7.f;int inset=(int)((1.f-t)*118);int y=mid+(int)(t*(248-mid));
  line(inset,y,W-inset,y,RGB(18,24,32));
 }
 /* Near wall faces (depth 0) — thick MM-style side panels */
 for(int i=0;i<4;i++){
  int x0=i*12,x1=(i+1)*12;int y0=28+i*12,y1=248-i*12;
  sc_panel(x0,y0,x1,y1,wall,wall2,trim,(i%2)==0);
  sc_panel(W-x1,y0,W-x0,y1,wall,wall2,trim,(i%2)==0);
 }
 /* Mid wall faces (depth 1) */
 for(int i=4;i<8;i++){
  int x0=48+(i-4)*14,x1=x0+14;int y0=40+(i-4)*10,y1=236-(i-4)*10;
  sc_panel(x0,y0,x1,y1,wall2,wall,trim,(i%3)==0);
  sc_panel(W-x1,y0,W-x0,y1,wall2,wall,trim,(i%3)==0);
 }
 /* Side doors track facing (left / right relative) */
 sc_draw_side_door(1,0,door_l,nbr_l,wall,wall2,trim);
 sc_draw_side_door(0,0,door_r,nbr_r,wall,wall2,trim);
 sc_draw_side_door(1,1,door_l,nbr_l,wall,wall2,trim);
 sc_draw_side_door(0,1,door_r,nbr_r,wall,wall2,trim);
 /* Far passage or sealed bulkhead */
 if(door_f)sc_draw_far_door(nbr_f);else sc_draw_bulkhead(wall,wall2,trim);
 /* Hanging lamp + soft glow cone */
 {int ly=40-sc_bob/3;circle(240,ly,7,trim);rect(236,ly-10,8,10,GOLD);
  rect(232,ly+8,16,2,mix_rgb(trim,RGB(255,220,120),.45f));
  for(int g=0;g<5;g++)rect(220-g*6,ly+12+g*8,40+g*12,1,mix_rgb(trim,RGB(8,12,16),.08f+g*.02f));}
 /* Room furniture props */
 sc_draw_props(room,trim);
 /* Portrait NPCs standing in the deck */
 ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
 for(int i=0;i<pn;i++)sc_draw_npc(i,&people[i],sc_menu==SC_MENU_PERSON&&i==sc_choice);
 if(sc_bob>0)sc_bob--;
}
static void sc_draw_minimap(void){
 int ox=372,oy=34,cs=13;
 rect(ox-5,oy-12,SC_W*cs+10,SC_H*cs+18,RGB(4,10,16));
 rect(ox-5,oy-12,SC_W*cs+10,1,CYAN);
 text(47,3,CYAN,"MAP");
 for(int y=0;y<SC_H;y++)for(int x=0;x<SC_W;x++){
  int px=ox+x*cs+cs/2-1,py=oy+y*cs+cs/2-1;
  if(x+1<SC_W&&sc_door_e[y][x])rect(px,py,cs,2,RGB(48,72,84));
  if(y>0&&sc_door_n[y][x])rect(px,py-cs,2,cs,RGB(48,72,84));
 }
 for(int y=0;y<SC_H;y++)for(int x=0;x<SC_W;x++){
  int px=ox+x*cs,py=oy+y*cs; unsigned c=RGB(14,22,30);
  if(sc_map[y][x]==SC_ROOM_HUB)c=RGB(40,100,112);
  else if(sc_map[y][x]==SC_ROOM_SHOP)c=RGB(100,78,32);
  else if(sc_map[y][x]==SC_ROOM_BAR)c=RGB(84,42,92);
  else if(sc_map[y][x]==SC_ROOM_GUILD)c=RGB(28,92,68);
  else if(sc_map[y][x]==SC_ROOM_CLINIC)c=RGB(42,88,108);
  else if(sc_map[y][x]==SC_ROOM_BAY)c=RGB(72,60,32);
  else if(sc_map[y][x]==SC_ROOM_LOCK)c=RGB(96,40,40);
  rect(px,py,cs-2,cs-2,c);
  if(x==sc_x&&y==sc_y){rect(px+1,py+1,cs-4,cs-4,GOLD);
   if(sc_face==SC_N)rect(px+cs/2-1,py+1,2,4,WHITE);
   if(sc_face==SC_S)rect(px+cs/2-1,py+cs-6,2,4,WHITE);
   if(sc_face==SC_E)rect(px+cs-6,py+cs/2-1,4,2,WHITE);
   if(sc_face==SC_WDIR)rect(px+1,py+cs/2-1,4,2,WHITE);
  }
 }
}
static void sc_draw_ui(void){
 sc_build_map();
 sc_draw_fp();
 /* Header chrome — room name owns the center so it never fights station title */
 rect(0,0,W,28,RGB(6,16,24)); rect(0,26,W,2,GOLD);
 text(1,0,GOLD,"STATION");
 text(10,0,WHITE,"%.14s",station_name(&game));
 {const char *rn=sc_room_name(sc_map[sc_y][sc_x]);unsigned tc=sc_room_trim(sc_map[sc_y][sc_x]);
  rect(200,4,120,18,RGB(8,20,30));rect(200,4,120,1,tc);text(26,0,CYAN,"%.12s",rn);}
 {const char *d[]={"N","E","S","W"};text(42,0,WHITE,"[%s]",d[sc_face&3]);
  if(sc_door_ahead())text(46,0,CYAN,"DOOR");else text(46,0,DIM,"WALL");}
 text(1,2,DIM,"L/R TURN  UP WALK  X ACT  O DECK");
 sc_draw_minimap();
 rect(0,248,W,24,RGB(6,16,24)); rect(0,248,W,2,CYAN);
 ScNpc people[3]; int pn=sc_fill_npcs(sc_x,sc_y,people,3);
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  text(1,31,GOLD,"CHANDLER");
  if(!ln)text(12,31,DIM,"Sold out today.");
  else {int idx=list[sc_shop_row%ln]; text(12,31,WHITE,"%.14s  %.1fU",equipment_list_names[idx],equipment_costs[idx]*.1f);}
  text(1,32,DIM,"UP/DOWN pick   X buy   TRI back");
 }else if(sc_menu==SC_MENU_PERSON&&pn>0){
  if(sc_choice<0)sc_choice=0; if(sc_choice>=pn)sc_choice=pn-1;
  ScNpc *p=&people[sc_choice];
  text(1,31,GOLD,"%.10s",p->name); text_wrap(14,31,44,1,WHITE,p->line,0);
  text(1,32,AMBER,">"); text_wrap(3,32,55,1,AMBER,p->offer,0);
 }else if(pn>0){
  text(1,31,GOLD,"%d HERE",pn); text_wrap(12,31,46,1,WHITE,people[0].line,0);
  text(1,32,DIM,"X talk/trade   face DOOR + UP to walk");
 }else text(1,31,DIM,"Empty deck. Face a DOOR and press UP.");
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
