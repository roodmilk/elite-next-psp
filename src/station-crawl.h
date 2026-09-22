/* MacVenture-style station: illustrated rooms + verb/hotspot point-and-click.
 * Shadowgate / Deja Vu grammar. Soft-FB. Talk/shop/gift/taxi preserved.
 * Soft look from ART DIRECTOR bake kit (PR #11 handoff + PR #12 station-art-kit.h).
 * Palette+styles via station-art-kit.h; this file owns geometry/input/state. */
#include "station-art-kit.h"
enum {
 SC_R_ARRIVALS=0, SC_R_SHOP, SC_R_CANTEEN, SC_R_CARGO, SC_R_GUILD, SC_R_CLINIC, SC_R_CUSTOMS, SC_R_COUNT
};
enum { SC_V_LOOK=0, SC_V_SPEAK, SC_V_GO, SC_V_TAKE, SC_V_COUNT };
enum { SC_H_NONE=0, SC_H_PERSON, SC_H_EXIT, SC_H_PROP, SC_H_FEATURE };
enum { SC_MENU_NONE=0, SC_MENU_SHOP, SC_MENU_TALK };
enum { SC_W=1, SC_H=1, SC_N=0, SC_E=1, SC_S=2, SC_WDIR=3 };
enum { SC_EXIT_SHIP=-1 };
/* Kit aliases — single palette source (do not diverge from ART_*). */
#define SC_VOID ART_VOID
#define SC_CHAR ART_CHARCOAL
#define SC_SLATE ART_SLATE
#define SC_OLIVE ART_OLIVE
#define SC_RUST ART_RUST
#define SC_OCHRE ART_OCHRE
#define SC_CREAM ART_CREAM
#define SC_LAV ART_LAVENDER
#define SC_CYAN ART_CYAN
#define SC_AMBER ART_AMBER
#define SC_DANGER ART_DANGER
/* MacVenture room ids → art kit (bake mapper still tracks old grid enums). */
static int sc_art_id(int room){
 static const int map[SC_R_COUNT]={
  ART_ROOM_HUB, ART_ROOM_SHOP, ART_ROOM_BAR, ART_ROOM_BAY,
  ART_ROOM_GUILD, ART_ROOM_CLINIC, ART_ROOM_CUSTOMS
 };
 return room>=0&&room<SC_R_COUNT?map[room]:ART_ROOM_HUB;
}
static int sc_room=SC_R_ARRIVALS, sc_verb=SC_V_LOOK, sc_hot=0, sc_menu=0, sc_shop_row=0, sc_talk_row=0, sc_talk_who=0, sc_built_for=-1;
static int sc_x=0, sc_y=0, sc_face=SC_S;
static unsigned char sc_map[1][1], sc_door_n[1][1], sc_door_e[1][1];
static const ArtRoomStyle *sc_style(void){return art_room_style(sc_art_id(sc_room));}
typedef struct { const char *name; int role; int act; int shop_item; int gift_bit; int quest_pay; int taxi_pay; const char *line; const char *offer; } ScNpc;
enum { SC_ACT_TALK=0, SC_ACT_SHOP, SC_ACT_GIFT, SC_ACT_QUEST, SC_ACT_TAXI, SC_ACT_BOARD };
typedef struct { int kind; int id; int x,y,w,h; const char *label; const char *look; } ScHot;
static void sc_person_pos(int room,int i,int *ox,int *oy){
 /* Keep people off the door row; leave centre free for focal props. */
 static const int pos[SC_R_COUNT][3][2]={
  {{40,94},{218,90},{0,0}},
  {{96,98},{232,92},{0,0}},
  {{72,96},{236,94},{0,0}},
  {{48,98},{0,0},{0,0}},
  {{78,96},{228,94},{0,0}},
  {{100,96},{0,0},{0,0}},
  {{108,96},{0,0},{0,0}}
 };
 if(room<0||room>=SC_R_COUNT)room=0;if(i<0)i=0;if(i>2)i=2;
 *ox=pos[room][i][0];*oy=pos[room][i][1];
}
static const char *sc_room_title(int r){
 static const char *n[]={"ARRIVALS HALL","CHANDLERY","CANTEEN","CARGO BAY","GUILD DESK","MED CLINIC","CUSTOMS LOCK"};
 return r>=0&&r<SC_R_COUNT?n[r]:"DECK";
}
static const char *sc_room_short(int r){
 static const char *n[]={"ARRIVAL","SHOP","CANTEEN","CARGO","GUILD","CLINIC","CUSTOMS"};
 return r>=0&&r<SC_R_COUNT?n[r]:"DECK";
}
static const char *sc_room_blurb(int r){
 static const char *n[]={
  "Berth window and traffic board. Warm lamps. Your ship waits when you are done.",
  "Ochre crates and a frank ledger. Exclusive stock lives behind this counter.",
  "Warm counter light, worn booths. Rumours and taxi seats share the air.",
  "Pallet lane and hazard stripes. Loader keeps the tip clipboard.",
  "Quiet survey desk. Tracked Mission sits on the green terminal.",
  "Cool task light, one medkit issue. Rest if you need it.",
  "Navy glass and a red scanner gate. Restricted goods light the beams."
 };
 return r>=0&&r<SC_R_COUNT?n[r]:"";
}
static int sc_fill_npcs(int room,ScNpc *out,int maxn){
 int n=0; unsigned h=(unsigned)(game.system*131u+room*41u);
 #define SC_PUSH(N,R,A,SI,GB,QP,TP,L,O) do{if(n<maxn){out[n].name=(N);out[n].role=(R);out[n].act=(A);out[n].shop_item=(SI);out[n].gift_bit=(GB);out[n].quest_pay=(QP);out[n].taxi_pay=(TP);out[n].line=(L);out[n].offer=(O);n++;}}while(0)
 if(room==SC_R_ARRIVALS){
  SC_PUSH("VENN",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Berth six stays warm if you keep coming home.","Ask about local traffic");
  SC_PUSH("DOCKHAND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Watch the freighter lane. Pads are tight.","Ask about berths and fuel");
 }else if(room==SC_R_SHOP){
  SC_PUSH("CHANDLER",TRADERS,SC_ACT_SHOP,-1,-1,0,0,"I stock what the main board won't list.","Browse exclusive stock");
  if((h&3)==0)SC_PUSH("MECHANIC",TRADERS,SC_ACT_GIFT,-1,0,0,0,"Take a spare clamp. Once. Don't ask twice.","Accept free clamp");
 }else if(room==SC_R_CANTEEN){
  SC_PUSH("BARTEND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Meridian tips well. Truth tips better.","Ask for a rumour");
  SC_PUSH("TRAVELER",EXPLORERS,SC_ACT_TAXI,-1,-1,0,1800,"Need a lift to another hub. One tonne seat.","Book taxi berth");
 }else if(room==SC_R_CARGO){
  SC_PUSH("LOADER",TRADERS,SC_ACT_QUEST,-1,-1,400,0,"Haul a crate mark to the board for me.","Take 40 U tip job");
 }else if(room==SC_R_CLINIC){
  SC_PUSH("MEDIC",LAW,SC_ACT_GIFT,-1,1,0,0,"Medkit for the next scrape. One issue only.","Take free medkit");
 }else if(room==SC_R_GUILD){
  SC_PUSH("KEI",EXPLORERS,SC_ACT_TALK,-1,-1,0,0,"When you are ready, open Tracked Mission.","Ask about Ryn / next step");
  SC_PUSH("SURVEYOR",EXPLORERS,SC_ACT_QUEST,-1,-1,600,0,"Fresh surface sample pays. Log it honest.","Accept survey tip");
 }else if(room==SC_R_CUSTOMS){
  SC_PUSH("CUSTOMS",LAW,SC_ACT_TALK,-1,-1,0,0,"Restricted goods still raise warrants.","Ask about your heat");
 }
 #undef SC_PUSH
 return n;
}
static int sc_exits(int room,int *out,int maxn){
 int n=0;
 #define SC_EX(R) do{if(n<maxn)out[n++]=(R);}while(0)
 if(room==SC_R_ARRIVALS){SC_EX(SC_R_SHOP);SC_EX(SC_R_CANTEEN);SC_EX(SC_R_GUILD);SC_EX(SC_R_CUSTOMS);}
 else if(room==SC_R_SHOP){SC_EX(SC_R_ARRIVALS);SC_EX(SC_R_CARGO);}
 else if(room==SC_R_CANTEEN){SC_EX(SC_R_ARRIVALS);SC_EX(SC_R_CARGO);}
 else if(room==SC_R_CARGO){SC_EX(SC_R_SHOP);SC_EX(SC_R_CANTEEN);}
 else if(room==SC_R_GUILD){SC_EX(SC_R_ARRIVALS);SC_EX(SC_R_CLINIC);}
 else if(room==SC_R_CLINIC){SC_EX(SC_R_GUILD);SC_EX(SC_R_CUSTOMS);}
 else if(room==SC_R_CUSTOMS){SC_EX(SC_R_ARRIVALS);SC_EX(SC_R_CLINIC);}
 #undef SC_EX
 return n;
}
static void sc_build_map(void){
 if(sc_built_for==game.system)return;
 sc_built_for=game.system; sc_room=SC_R_ARRIVALS; sc_verb=SC_V_LOOK; sc_hot=0; sc_menu=0; sc_shop_row=0; sc_talk_row=0; sc_talk_who=0;
 sc_x=0;sc_y=0;sc_face=SC_S;sc_map[0][0]=SC_R_ARRIVALS;sc_door_n[0][0]=1;sc_door_e[0][0]=1;
}
static int sc_door_dir(int dir){(void)dir;return 1;}
static int sc_door_ahead(void){return 1;}
/* Focal anchors only (art handoff: 3–5 interactables + people/doors/ship). */
static int sc_hotspots(ScHot *out,int maxn){
 int n=0;
 #define SC_HOT(K,ID,X,Y,W,H,L,LOOK) do{if(n<maxn){out[n].kind=(K);out[n].id=(ID);out[n].x=(X);out[n].y=(Y);out[n].w=(W);out[n].h=(H);out[n].label=(L);out[n].look=(LOOK);n++;}}while(0)
 const int VX=72,VY=24;
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 for(int i=0;i<pn;i++){
  int px,py;sc_person_pos(sc_room,i,&px,&py);
  if(px==0&&py==0)continue;
  SC_HOT(SC_H_PERSON,i,VX+px,VY+py,52,64,people[i].name,people[i].line);
 }
 int ex[4],en=sc_exits(sc_room,ex,4);
 for(int i=0;i<en&&i<4;i++){
  int dx=VX+10+i*76,dy=VY+4;
  SC_HOT(SC_H_EXIT,ex[i],dx,dy,70,22,sc_room_short(ex[i]),"Door. GO + X walks through. Also listed under EXITS.");
 }
 /* Ship return — every room, amber, bottom-right of MAIN. */
 SC_HOT(SC_H_EXIT,SC_EXIT_SHIP,VX+236,VY+128,76,28,"YOUR SHIP","Board your ship now. TRI also boards. EXITS > SHIP.");
 if(sc_room==SC_R_ARRIVALS){
  SC_HOT(SC_H_FEATURE,0,VX+208,VY+78,90,52,"TRAFFIC BOARD","Berths, delays, heat. Safe to LOOK.");
  SC_HOT(SC_H_FEATURE,1,VX+24,VY+12,270,48,"BERTH WINDOW","Cool spill from the lane. Tiny freighter silhouette.");
  SC_HOT(SC_H_PROP,0,VX+36,VY+122,200,14,"RAIL","Handrail over the freighter lane.");
 }else if(sc_room==SC_R_SHOP){
  SC_HOT(SC_H_FEATURE,1,VX+118,VY+38,60,42,"LEDGER","Hand ink. Prices the main board never prints.");
  SC_HOT(SC_H_PROP,1,VX+176,VY+98,72,40,"STOCK CRATE","Exclusive clamps and spares. SPEAK to buy.");
  SC_HOT(SC_H_FEATURE,8,VX+88,VY+100,150,22,"COUNTER","Warm key light. Deals happen here.");
 }else if(sc_room==SC_R_CANTEEN){
  SC_HOT(SC_H_PROP,2,VX+40,VY+112,240,26,"BAR TOP","Warm amber pools. Tip jars and rings.");
  SC_HOT(SC_H_FEATURE,2,VX+12,VY+78,42,48,"JUKE","Soft Transit loop. Not a nightclub.");
  SC_HOT(SC_H_PROP,13,VX+16,VY+100,50,28,"RUMOUR BOOTH","Travellers leave tip notes in the seat.");
 }else if(sc_room==SC_R_CARGO){
  SC_HOT(SC_H_PROP,3,VX+128,VY+72,54,44,"TIP CRATE","Marked for the board. SPEAK to the loader.");
  SC_HOT(SC_H_FEATURE,5,VX+12,VY+8,296,18,"CARGO LIFT","Overhead hoist. Industrial cool light.");
  SC_HOT(SC_H_PROP,17,VX+68,VY+42,44,24,"MANIFEST","Today's tip jobs. Loader keeps the pen.");
 }else if(sc_room==SC_R_GUILD){
  SC_HOT(SC_H_FEATURE,3,VX+88,VY+14,144,56,"TRACKED SCREEN","Mission slate. Kei points here when you stall.");
  SC_HOT(SC_H_PROP,7,VX+50,VY+110,220,28,"GUILD DESK","Sage survey gear and cream paper strips.");
 }else if(sc_room==SC_R_CLINIC){
  SC_HOT(SC_H_PROP,4,VX+240,VY+40,60,80,"MEDKIT LOCKER","One issue. TAKE after SPEAK with the medic.");
  SC_HOT(SC_H_PROP,8,VX+70,VY+100,180,36,"TREATMENT BAY","Cool slate bench. Rest if you need it.");
  SC_HOT(SC_H_FEATURE,12,VX+24,VY+50,50,36,"DIAG PANEL","Soft beeps. Shape beats hue for readability.");
 }else if(sc_room==SC_R_CUSTOMS){
  SC_HOT(SC_H_FEATURE,4,VX+100,VY+18,120,56,"WARRANT GLASS","Your legal heat on navy glass.");
  SC_HOT(SC_H_FEATURE,6,VX+24,VY+50,26,80,"SCANNER GATE","Walk through only if you like questions.");
  SC_HOT(SC_H_PROP,23,VX+80,VY+110,160,28,"INSPECT DESK","Stamps, forms, a tired officer.");
 }
 #undef SC_HOT
 return n;
}
static int sc_find_hot(int kind,int id){
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 for(int i=0;i<hn;i++)if(hot[i].kind==kind&&(id<0||hot[i].id==id))return i;
 return -1;
}
static void sc_snap_verb_hot(void){
 if(sc_verb==SC_V_GO){int i=sc_find_hot(SC_H_EXIT,-1);if(i>=0)sc_hot=i;}
 else if(sc_verb==SC_V_SPEAK){int i=sc_find_hot(SC_H_PERSON,-1);if(i>=0)sc_hot=i;}
 else if(sc_verb==SC_V_TAKE){
  int i=sc_find_hot(SC_H_PERSON,-1);
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  for(int p=0;p<pn;p++)if(people[p].act==SC_ACT_GIFT||people[p].act==SC_ACT_SHOP){int j=sc_find_hot(SC_H_PERSON,p);if(j>=0){i=j;break;}}
  if(i>=0)sc_hot=i;
 }
}
/* ---- drawing helpers ---- */
static void sc_win(int x,int y,int w,int h,const char *title,unsigned edge){
 rect(x,y,w,h,SC_CHAR);
 rect(x,y,w,12,SC_SLATE);
 rect(x,y,w,1,edge);rect(x,y+h-1,w,1,edge);rect(x,y,1,h,edge);rect(x+w-1,y,1,h,edge);
 text((x+6)/8,(y+2)/8,SC_CREAM,"%.14s",title);
}
static void sc_scene_sky(int x,int y,int w,int h,unsigned hi,unsigned lo){
 for(int row=0;row<h;row++){
  float t=row/(float)(h>1?h-1:1);
  rect(x,y+row,w,1,mix_rgb(hi,lo,t));
 }
}
static void sc_anchor_tick(int x,int y,int w,int h,unsigned c){
 /* Tiny edge cue so every hotspot reads without a full gold box. */
 rect(x,y,6,1,c);rect(x,y,1,6,c);
 rect(x+w-6,y,6,1,c);rect(x+w-1,y,1,6,c);
 rect(x,y+h-1,6,1,c);rect(x,y+h-6,1,6,c);
 rect(x+w-6,y+h-1,6,1,c);rect(x+w-1,y+h-6,1,6,c);
}
static void sc_draw_person_sprite(int x,int y,const ScNpc *p,int selected){
 unsigned ink=faction_colors[p->role%FACTION_COUNT];
 /* Contact shadow */
 rect(x+10,y+60,32,4,mix_rgb(SC_VOID,SC_CHAR,.4f));
 if(selected){rect(x-2,y-2,52,66,SC_AMBER);rect(x-1,y-1,50,64,SC_CHAR);}
 else sc_anchor_tick(x,y,52,64,mix_rgb(ink,SC_CREAM,.5f));
 rect(x+8,y+48,12,12,mix_rgb(ink,SC_SLATE,.5f));
 rect(x+22,y+48,12,12,mix_rgb(ink,SC_SLATE,.5f));
 rect(x+10,y+28,28,24,mix_rgb(ink,SC_OLIVE,.35f));
 rect(x+6,y+28,6,20,mix_rgb(ink,SC_OLIVE,.35f));
 rect(x+36,y+28,6,20,mix_rgb(ink,SC_OLIVE,.35f));
 rect(x+8,y+2,32,28,SC_VOID);
 if(!strcmp(p->name,"KEI"))draw_kei(x+8,y+2,28,0);
 else draw_portrait(x+8,y+2,28,28,game.system*37+p->role*17+(int)(p->name[0]*3),p->role);
 rect(x+8,y+28,32,1,ink);
 text((x+4)/8,(y+58)/8,selected?SC_AMBER:SC_CREAM,"%.8s",p->name);
}
static void sc_illust_arrivals(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Kit HUB: board + berth window; warm lamp + cyan berth cue. */
 sc_scene_sky(x,y,w,h,st->wall,st->wall2);
 rect(x+18,y+10,w-36,52,SC_VOID);
 rect(x+18,y+10,w-36,2,st->trim);
 for(int i=0;i<22;i++){unsigned n=art_hash((unsigned)(i*97+game.system));pixel(x+30+(n%(w-60)),y+16+((n>>8)%36),((n>>16)&1)?SC_CREAM:SC_LAV);}
 rect(x+w/2-48,y+28,64,12,st->wall);rect(x+w/2-28,y+22,28,8,mix_rgb(st->wall,st->wall2,.5f));
 pixel(x+w/2+8,y+30,st->lamp);pixel(x+w/2-36,y+34,st->trim);
 for(int i=0;i<5;i++){rect(x+12+i*60,y+64,48,3,SC_OLIVE);rect(x+28+i*60,y+68,12,5,st->lamp);}
 for(int row=0;row<8;row++){
  rect(x+4,y+76+row*8,26,7,((row)&1)?st->trim:mix_rgb(st->trim,SC_OLIVE,.35f));
  rect(x+w-30,y+76+row*8,26,7,((row)&1)?st->trim:mix_rgb(st->trim,SC_OLIVE,.35f));
 }
 for(int row=0;row<6;row++)for(int col=0;col<8;col++){
  int fx=x+34+col*32,fy=y+h-58+row*8;
  unsigned c=((col+row)&1)?st->wall:mix_rgb(st->wall,st->wall2,.4f);
  rect(fx,fy,30,7,c);rect(fx,fy,30,1,mix_rgb(SC_CREAM,st->wall,.55f));
 }
 rect(x+w-100,y+76,88,56,st->wall2);
 rect(x+w-96,y+80,80,14,mix_rgb(st->accent,st->wall,.45f));
 rect(x+w-96,y+98,80,10,mix_rgb(SC_OLIVE,st->accent,.3f));
 rect(x+w-96,y+112,80,10,st->lamp);
 text((x+w-90)/8,(y+82)/8,st->accent,"BERTHS");
 rect(x+36,y+h-68,w-72,4,st->trim);
 rect(x+36,y+h-64,w-72,3,SC_OLIVE);
 rect(x+42,y+78,34,46,st->wall);rect(x+46,y+84,26,36,SC_VOID);
 rect(x+w-76,y+78,34,46,st->wall);rect(x+w-72,y+84,26,36,SC_VOID);
}
static void sc_illust_shop(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 sc_scene_sky(x,y,w,h,mix_rgb(st->trim,st->wall,.4f),st->wall2);
 for(int row=0;row<6;row++)for(int col=0;col<9;col++)
  rect(x+6+col*34,y+h-54+row*8,32,7,((col+row)&1)?st->trim:mix_rgb(SC_RUST,st->trim,.4f));
 for(int s=0;s<4;s++){
  rect(x+8,y+12+s*26,64,22,mix_rgb(st->wall,st->trim,.3f));
  rect(x+12,y+16+s*26,14,12,st->lamp);rect(x+30,y+16+s*26,14,12,st->accent);rect(x+48,y+16+s*26,14,12,SC_RUST);
  rect(x+w-72,y+12+s*26,64,22,mix_rgb(st->wall,st->trim,.3f));
  rect(x+w-68,y+16+s*26,14,12,SC_LAV);rect(x+w-50,y+16+s*26,14,12,st->trim);
 }
 rect(x+78,y+h-76,w-156,26,SC_RUST);
 rect(x+78,y+h-84,w-156,10,st->trim);
 for(int i=0;i<5;i++)rect(x+90+i*28,y+h-72,18,10,st->wall2);
 rect(x+w/2-28,y+38,56,40,mix_rgb(st->wall,SC_RUST,.3f));
 rect(x+w/2-22,y+44,44,20,SC_CREAM);
 text((x+w/2-18)/8,(y+50)/8,st->lamp,"LEDGER");
 rect(x+w/2-18,y+8,36,26,st->wall);rect(x+w/2-12,y+12,24,18,SC_VOID);
}
static void sc_illust_canteen(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Kit BAR: warm practical key, no neon. */
 sc_scene_sky(x,y,w,h,st->wall,st->wall2);
 for(int row=0;row<5;row++)rect(x,y+h-46+row*8,w,8,mix_rgb(st->wall,st->wall2,row/5.f));
 rect(x+20,y+12,w-40,48,mix_rgb(st->wall,st->trim,.25f));
 for(int i=0;i<10;i++){
  unsigned c=mix_rgb(SC_CREAM,st->trim,(i%4)*.2f);
  rect(x+28+i*24,y+18,10,32,c);pixel(x+31+i*24,y+22,st->lamp);
 }
 rect(x+20,y+62,w-40,3,st->accent);
 rect(x+24,y+h-68,w-48,24,mix_rgb(st->trim,st->wall,.4f));
 rect(x+24,y+h-76,w-48,10,st->lamp);
 for(int i=0;i<6;i++){rect(x+44+i*40,y+h-90,10,12,mix_rgb(SC_CREAM,st->lamp,.3f));pixel(x+47+i*40,y+h-92,st->lamp);}
 rect(x+16,y+h-108,48,26,mix_rgb(st->trim,st->wall2,.35f));
 rect(x+w-64,y+h-108,48,26,mix_rgb(st->trim,st->wall2,.35f));
 rect(x+12,y+78,40,46,SC_SLATE);rect(x+16,y+82,32,20,mix_rgb(st->lamp,st->wall,.4f));
 for(int i=0;i<4;i++)pixel(x+20+i*6,y+110,st->accent);
 text((x+16)/8,(y+116)/8,SC_CREAM,"JUKE");
}
static void sc_illust_cargo(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->trim,.2f),st->wall2);
 for(int row=0;row<6;row++)for(int col=0;col<9;col++)
  rect(x+4+col*35,y+h-56+row*8,33,7,((col+row)&1)?st->wall:mix_rgb(SC_OLIVE,st->wall,.4f));
 rect(x+12,y+8,w-24,4,SC_LAV);rect(x+w/2-3,y+8,6,48,st->trim);
 rect(x+w/2-14,y+52,28,8,st->wall);
 for(int i=0;i<5;i++){
  rect(x+16+i*58,y+h-104,50,40,st->trim);
  rect(x+16+i*58,y+h-104,50,5,SC_CREAM);
  rect(x+30+i*58,y+h-88,20,8,st->wall2);
  if(i==2){rect(x+20+i*58,y+h-116,42,14,st->lamp);text((x+24+i*58)/8,(y+h-114)/8,st->wall2,"TIP");}
 }
 rect(x+4,y+h-62,w-8,4,st->lamp);
 for(int i=0;i<(w-8)/12;i++)if(i&1)rect(x+4+i*12,y+h-62,12,4,st->wall2);
}
static void sc_illust_guild(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->wall2,.5f),st->wall2);
 for(int row=0;row<5;row++)rect(x,y+h-44+row*8,w,8,mix_rgb(st->wall,st->wall2,row/5.f));
 rect(x+w/2-40,y+68,80,h-116,mix_rgb(st->wall,st->wall2,.3f));
 rect(x+w/2-70,y+14,140,56,st->wall2);
 rect(x+w/2-64,y+20,128,34,mix_rgb(st->wall,st->accent,.35f));
 text((x+w/2-48)/8,(y+28)/8,st->accent,"TRACKED MISSION");
 rect(x+w/2-50,y+54,100,8,st->lamp);
 rect(x+50,y+h-76,w-100,30,mix_rgb(st->wall,st->wall2,.4f));
 rect(x+50,y+h-84,w-100,10,mix_rgb(st->trim,st->wall,.4f));
 rect(x+30,y+h-98,36,26,SC_SLATE);rect(x+w-66,y+h-98,36,26,SC_SLATE);
 rect(x+16,y+78,18,38,mix_rgb(st->wall,st->accent,.2f));rect(x+12,y+70,26,12,mix_rgb(st->wall,st->lamp,.3f));
}
static void sc_illust_clinic(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 sc_scene_sky(x,y,w,h,mix_rgb(st->trim,st->wall,.35f),st->wall2);
 for(int row=0;row<5;row++)rect(x,y+h-44+row*8,w,8,mix_rgb(st->trim,st->wall2,row/5.f));
 for(int i=0;i<4;i++){circle(x+50+i*70,y+16,7,mix_rgb(st->lamp,st->accent,.4f));rect(x+46+i*70,y+22,14,3,st->trim);}
 rect(x+70,y+h-84,w-140,34,mix_rgb(st->trim,st->accent,.5f));
 rect(x+70,y+h-92,w-140,10,mix_rgb(st->lamp,st->wall,.4f));
 rect(x+80,y+h-108,42,18,st->accent);
 rect(x+w-78,y+40,60,78,st->wall);
 rect(x+w-70,y+50,44,28,mix_rgb(st->lamp,st->accent,.35f));
 text((x+w-64)/8,(y+58)/8,st->wall2,"MEDKIT");
 rect(x+w-70,y+88,44,18,st->wall2);
 rect(x+24,y+50,48,34,st->wall2);rect(x+28,y+54,40,22,mix_rgb(st->lamp,SC_OLIVE,.4f));
}
static void sc_illust_customs(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->trim,.15f),st->wall2);
 for(int row=0;row<5;row++)rect(x,y+h-44+row*8,w,8,mix_rgb(st->wall,st->wall2,row/5.f));
 rect(x+24,y+48,22,h-98,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+w-46,y+48,22,h-98,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+24,y+48,w-48,8,st->trim);
 for(int i=0;i<6;i++)rect(x+50+i*36,y+52,3,h-108,mix_rgb(st->trim,SC_VOID,.45f));
 rect(x+60,y+h-76,w-120,28,st->wall);
 rect(x+60,y+h-84,w-120,10,mix_rgb(SC_CREAM,st->wall,.4f));
 rect(x+w/2-60,y+16,120,56,st->wall2);
 rect(x+w/2-54,y+22,108,20,st->trim);
 text((x+w/2-40)/8,(y+28)/8,SC_CREAM,"WARRANT");
 rect(x+w/2-54,y+46,108,18,SC_VOID);
 text((x+w/2-36)/8,(y+50)/8,st->accent,"SCANNING");
}
static void sc_draw_main_scene(void){
 const int VX=72,VY=24,VW=320,VH=160;
 sc_win(VX-2,VY-14,VW+4,VH+18,"MAIN",SC_CYAN);
 text((VX+6)/8,(VY-12)/8,SC_CYAN,"%.16s",sc_room_title(sc_room));
 if(sc_room==SC_R_ARRIVALS)sc_illust_arrivals(VX,VY,VW,VH);
 else if(sc_room==SC_R_SHOP)sc_illust_shop(VX,VY,VW,VH);
 else if(sc_room==SC_R_CANTEEN)sc_illust_canteen(VX,VY,VW,VH);
 else if(sc_room==SC_R_CARGO)sc_illust_cargo(VX,VY,VW,VH);
 else if(sc_room==SC_R_GUILD)sc_illust_guild(VX,VY,VW,VH);
 else if(sc_room==SC_R_CLINIC)sc_illust_clinic(VX,VY,VW,VH);
 else sc_illust_customs(VX,VY,VW,VH);
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 for(int i=0;i<pn;i++){
  int px,py;sc_person_pos(sc_room,i,&px,&py);
  if(px==0&&py==0)continue;
  sc_draw_person_sprite(VX+px,VY+py,&people[i],sc_hot<hn&&hot[sc_hot].kind==SC_H_PERSON&&hot[sc_hot].id==i);
 }
 /* Cue ticks on props/features; full gold on selection. */
 for(int i=0;i<hn;i++){
  if(hot[i].kind==SC_H_PERSON||hot[i].kind==SC_H_EXIT)continue;
  if(sc_hot==i){
   rect(hot[i].x-2,hot[i].y-2,hot[i].w+4,1,SC_AMBER);rect(hot[i].x-2,hot[i].y+hot[i].h+1,hot[i].w+4,1,SC_AMBER);
   rect(hot[i].x-2,hot[i].y-2,1,hot[i].h+4,SC_AMBER);rect(hot[i].x+hot[i].w+1,hot[i].y-2,1,hot[i].h+4,SC_AMBER);
  }else sc_anchor_tick(hot[i].x,hot[i].y,hot[i].w,hot[i].h,SC_CREAM);
 }
 for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_EXIT){
  int ship=hot[i].id==SC_EXIT_SHIP;
  unsigned c=(sc_hot==i)?SC_AMBER:(ship?SC_AMBER:SC_CYAN);
  rect(hot[i].x,hot[i].y,hot[i].w,hot[i].h,SC_CHAR);
  rect(hot[i].x,hot[i].y,hot[i].w,1,c);
  rect(hot[i].x,hot[i].y+hot[i].h-1,hot[i].w,1,c);
  if(ship){
   rect(hot[i].x+1,hot[i].y+1,hot[i].w-2,hot[i].h-2,mix_rgb(SC_AMBER,SC_CHAR,.25f));
   text((hot[i].x+6)/8,(hot[i].y+8)/8,c,">> SHIP");
  }else text((hot[i].x+4)/8,(hot[i].y+6)/8,c,"%.7s",hot[i].label);
 }
 if(sc_hot>=0&&sc_hot<hn&&hot[sc_hot].kind==SC_H_EXIT){
  ScHot *h=&hot[sc_hot];
  rect(h->x-2,h->y-2,h->w+4,1,SC_AMBER);rect(h->x-2,h->y+h->h+1,h->w+4,1,SC_AMBER);
 }
}
static void sc_draw_commands(void){
 static const char *v[]={"LOOK","SPEAK","GO","TAKE"};
 sc_win(4,2,472,18,"COMMANDS",SC_AMBER);
 for(int i=0;i<SC_V_COUNT;i++){
  int x=14+i*86;
  if(i==sc_verb){rect(x-2,6,78,12,mix_rgb(SC_OLIVE,SC_CHAR,.4f));rect(x-2,6,78,1,SC_AMBER);}
  text(x/8,1,i==sc_verb?SC_AMBER:SC_LAV,"%s",v[i]);
 }
 text(48,1,SC_AMBER,"TRI");text(52,1,SC_CREAM,"BOARD");
}
static void sc_draw_inventory(void){
 sc_win(4,24,64,160,"PACK",SC_AMBER);
 text(1,5,SC_LAV,"HOLD");
 int y=7;
 if(game.gift_flags&1u){text(1,y,SC_CYAN,"CLAMP");y+=2;}
 if(game.gift_flags&2u){text(1,y,SC_CYAN,"MEDKIT");y+=2;}
 if(!(game.gift_flags&3u)){text(1,y,SC_LAV,"(empty)");y+=2;}
 if(game.passenger_dest>=0){text(1,y,SC_AMBER,"PAX");y+=2;text(1,y,SC_CREAM,"%.8s",game.systems[game.passenger_dest].name);y+=2;}
 text(1,14,SC_LAV,"HERE");
 text(1,16,SC_CYAN,"%.8s",sc_room_short(sc_room));
 text(1,18,SC_AMBER,"TRI");text(1,19,SC_CREAM,"BOARD");
 text(1,21,SC_LAV,"O");text(1,22,SC_CREAM,"DECK");
}
static void sc_draw_exits(void){
 sc_win(400,24,76,160,"EXITS",SC_CYAN);
 int y=4;
 {int sel=0;ScHot hot[24];int hn=sc_hotspots(hot,24);
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT&&hot[h].id==SC_EXIT_SHIP&&h==sc_hot)sel=1;
  rect(404,y*8-2,68,16,sel?mix_rgb(SC_AMBER,SC_CHAR,.35f):mix_rgb(SC_OCHRE,SC_CHAR,.2f));
  text(51,y,sel?SC_AMBER:SC_AMBER,">>SHIP");y+=2;}
 text(51,y,SC_LAV,"doors");y+=2;
 int ex[4],en=sc_exits(sc_room,ex,4);
 for(int i=0;i<en;i++){
  int sel=0;ScHot hot[24];int hn=sc_hotspots(hot,24);
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT&&hot[h].id==ex[i]&&h==sc_hot)sel=1;
  if(sel)rect(404,y*8-2,68,14,mix_rgb(SC_CYAN,SC_CHAR,.3f));
  text(51,y,sel?SC_AMBER:SC_CREAM,"%.8s",sc_room_short(ex[i]));y+=2;
 }
 text(51,19,SC_LAV,"GO+X");
 text(51,21,SC_AMBER,"TRI");text(51,22,SC_CREAM,"board");
}
static int sc_talk_choices(const ScNpc *p,const char **out,int maxn){
 int n=0;
 #define SC_CH(S) do{if(n<maxn)out[n++]=(S);}while(0)
 SC_CH("Hear them out");
 if(p->act==SC_ACT_SHOP)SC_CH("Browse exclusive stock");
 else if(p->act==SC_ACT_GIFT)SC_CH(p->offer);
 else if(p->act==SC_ACT_QUEST)SC_CH(p->offer);
 else if(p->act==SC_ACT_TAXI)SC_CH(p->offer);
 else SC_CH(p->offer&&p->offer[0]?p->offer:"Ask for a useful tip");
 SC_CH("Never mind");
 #undef SC_CH
 return n;
}
static void sc_talk_tip(const ScNpc *p){
 System *s=&game.systems[game.system];
 char note[96];
 if(!strcmp(p->name,"VENN")){
  snprintf(note,sizeof(note),"Traffic: %s runs %s. Watch heat if you haul restricted.",
   s->name,s->economy<4?"industrial lanes":"farm freighters");
  message(&game,note);
 }else if(!strcmp(p->name,"DOCKHAND")){
  snprintf(note,sizeof(note),"Pads tight. Tech %d hub — fuel and repairs hold if you dock clean.",s->tech+1);
  message(&game,note);
 }else if(!strcmp(p->name,"BARTEND")){
  int d=danger_rating(&game,game.system);
  snprintf(note,sizeof(note),"Rumour: %s feels %s. Keep a clean hold past Customs.",
   s->name,d>=3?"rough":"quiet enough");
  message(&game,note);
 }else if(!strcmp(p->name,"KEI")){
  if(game.campaign_stage<6)message(&game,"Kei: finish Open Channel harbour training, then Tracked Mission.");
  else if(game.saga_chapter<SAGA_COUNT)message(&game,"Kei: open Tracked Mission on the command deck when you are ready.");
  else message(&game,"Kei: the board is quiet. Fly honest and check Guild tips.");
 }else if(!strcmp(p->name,"CUSTOMS")){
  snprintf(note,sizeof(note),"Warrant heat %d/5 here. Restricted goods raise it fast.",wanted_level(&game));
  message(&game,note);
 }else message(&game,p->offer?p->offer:p->line);
}
static void sc_draw_text_box(void){
 sc_win(4,190,472,78,"TEXT",SC_CYAN);
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  text(1,25,SC_AMBER,"CHANDLER STOCK");
  if(!ln)text(1,27,SC_LAV,"Sold out today.");
  else {int idx=list[sc_shop_row%ln];text(1,27,SC_CREAM,"%.14s  %.1fU",equipment_list_names[idx],equipment_costs[idx]*.1f);
   text(1,29,SC_LAV,"UP/DOWN  X buy  TRI back");}
  return;
 }
 if(sc_menu==SC_MENU_TALK){
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  if(sc_talk_who<0||sc_talk_who>=pn){sc_menu=SC_MENU_NONE;return;}
  ScNpc *p=&people[sc_talk_who];
  text(1,25,SC_AMBER,"%.10s",p->name);
  text_wrap(14,25,42,1,SC_CREAM,p->line,0);
  const char *ch[4]; int cn=sc_talk_choices(p,ch,4);
  if(sc_talk_row<0)sc_talk_row=0;if(sc_talk_row>=cn)sc_talk_row=cn-1;
  for(int i=0;i<cn;i++){
   int y=27+i;
   text(1,y,i==sc_talk_row?SC_AMBER:SC_LAV,i==sc_talk_row?">":" ");
   text_wrap(3,y,54,1,i==sc_talk_row?SC_CREAM:SC_LAV,ch[i],0);
  }
  return;
 }
 if(hn<=0){text_wrap(1,25,56,3,SC_CREAM,sc_room_blurb(sc_room),0);return;}
 if(sc_hot<0)sc_hot=0;if(sc_hot>=hn)sc_hot=hn-1;
 ScHot *h=&hot[sc_hot];
 text(1,25,SC_AMBER,"%.14s",h->label);
 {const ArtRoomStyle *st=sc_style();text(20,25,SC_LAV,"%.18s",st->keywords);}
 text_wrap(1,27,56,2,SC_CREAM,h->look?h->look:sc_room_blurb(sc_room),0);
 {static const char *v[]={"LOOK","SPEAK","GO","TAKE"};
  text(1,30,SC_AMBER,"%s > %.12s",v[sc_verb],h->label);}
 text(26,30,SC_LAV,"U/D  TRI board  O deck");
}
static void sc_draw_ui(void){
 sc_build_map();
 rect(0,0,W,H,SC_VOID);
 for(int i=0;i<H;i+=4)rect(0,i,W,1,SC_CHAR);
 sc_draw_commands();
 sc_draw_inventory();
 sc_draw_main_scene();
 sc_draw_exits();
 sc_draw_text_box();
}
static void sc_board_ship(void){
 page=HOME;sc_menu=0;message(&game,"Back aboard. Command deck ready.");game.cue=SFX_UI;
}
static void sc_do_npc_choice(ScNpc *p,int choice){
 const char *ch[4]; int cn=sc_talk_choices(p,ch,4);
 if(choice<0||choice>=cn)return;
 if(choice==cn-1){sc_menu=SC_MENU_NONE;message(&game,"You nod and step back.");game.cue=SFX_UI;return;}
 speak(&game,p->role==LAW?VOICE_LAW:p->role==EXPLORERS?VOICE_KEI:!strcmp(p->name,"VENN")?VOICE_VENN:VOICE_DOCK,p->line);
 if(choice==0){
  message(&game,p->line);sc_menu=SC_MENU_NONE;game.cue=SFX_UI;return;
 }
 sc_menu=SC_MENU_NONE;
 if(p->act==SC_ACT_TALK){sc_talk_tip(p);game.cue=SFX_UI;}
 else if(p->act==SC_ACT_SHOP){sc_menu=SC_MENU_SHOP;sc_shop_row=0;message(&game,"Chandler stock — exclusive to this deck.");}
 else if(p->act==SC_ACT_GIFT){
  int bit=p->gift_bit>=0?p->gift_bit:0;
  if(game.gift_flags&(1u<<bit)){message(&game,"Already took that gift.");return;}
  game.gift_flags|=1u<<bit;
  if(bit==0){if(!(game.upgrades&8)){game.upgrades|=8;message(&game,"Free cargo clamp fitted (+8t).");}else {game.credits+=200;message(&game,"Clamp spare sold for 20 U.");}}
  else {game.energy=100;message(&game,"Medkit used — energy restored.");}
  game.cue=SFX_UI;
 }else if(p->act==SC_ACT_QUEST){
  game.credits+=p->quest_pay;game.discoveries++;
  {char note[64];snprintf(note,sizeof(note),"Side tip paid %.1f U.",p->quest_pay*.1f);message(&game,note);}
  game.cue=SFX_SELECT;
 }else if(p->act==SC_ACT_TAXI){
  if(game.passenger_dest>=0){message(&game,"Cabin already holds a passenger.");return;}
  if(cargo_used(&game)>=cargo_capacity(&game)){message(&game,"Need 1t free for a passenger.");return;}
  int dest=(game.system*17+sc_room*3+11)&255;if(dest==game.system)dest=(dest+5)&255;
  game.passenger_dest=dest;game.passenger_kind=1;game.passenger_pay=p->taxi_pay;
  {char note[80];snprintf(note,sizeof(note),"Passenger booked for %s.",game.systems[dest].name);message(&game,note);}
  speak(&game,VOICE_CONTACT,"Thanks. I'll talk your ear off.");game.cue=SFX_UI;
 }
}
static void sc_apply(void){
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  if(!ln){message(&game,"Chandler is empty.");return;}
  if(sc_shop_row<0)sc_shop_row=0;if(sc_shop_row>=ln)sc_shop_row=ln-1;
  buy_equipment(list[sc_shop_row]);return;
 }
 if(sc_menu==SC_MENU_TALK){
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  if(sc_talk_who>=0&&sc_talk_who<pn)sc_do_npc_choice(&people[sc_talk_who],sc_talk_row);
  return;
 }
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 if(hn<=0){message(&game,sc_room_blurb(sc_room));return;}
 if(sc_hot<0)sc_hot=0;if(sc_hot>=hn)sc_hot=hn-1;
 ScHot *h=&hot[sc_hot];
 if(sc_verb==SC_V_LOOK){
  message(&game,h->look?h->look:sc_room_blurb(sc_room));game.cue=SFX_UI;return;
 }
 if(sc_verb==SC_V_GO){
  if(h->kind==SC_H_EXIT){
   if(h->id==SC_EXIT_SHIP){sc_board_ship();return;}
   sc_room=h->id;sc_hot=0;sc_verb=SC_V_LOOK;message(&game,sc_room_blurb(sc_room));game.cue=SFX_SELECT;return;
  }
  message(&game,"GO needs a door or YOUR SHIP (also TRI).");game.cue=SFX_UI;return;
 }
 if(sc_verb==SC_V_SPEAK){
  if(h->kind!=SC_H_PERSON){message(&game,"Nobody there. Pick a person, then SPEAK.");game.cue=SFX_UI;return;}
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  if(h->id<0||h->id>=pn){message(&game,"They stepped away.");return;}
  sc_menu=SC_MENU_TALK;sc_talk_who=h->id;sc_talk_row=0;game.cue=SFX_UI;return;
 }
 if(sc_verb==SC_V_TAKE){
  if(h->kind==SC_H_PERSON){
   ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
   if(h->id>=0&&h->id<pn&&people[h->id].act==SC_ACT_GIFT){
    sc_menu=SC_MENU_TALK;sc_talk_who=h->id;sc_talk_row=1;game.cue=SFX_UI;return;
   }
   if(h->id>=0&&h->id<pn&&people[h->id].act==SC_ACT_SHOP){sc_menu=SC_MENU_SHOP;sc_shop_row=0;return;}
  }
  if(h->kind==SC_H_PROP){message(&game,"Bolted down. SPEAK to whoever owns it.");game.cue=SFX_UI;return;}
  message(&game,"You cannot take that.");game.cue=SFX_UI;
 }
}
static int sc_input(unsigned pressed){
 sc_build_map();
 if(pressed&PSP_CTRL_TRIANGLE && sc_menu==SC_MENU_NONE){sc_board_ship();return 1;}
 if(sc_menu==SC_MENU_SHOP){
  if(pressed&PSP_CTRL_UP){sc_shop_row--;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_DOWN){sc_shop_row++;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_TRIANGLE){sc_menu=SC_MENU_NONE;return 1;}
  if(pressed&PSP_CTRL_CROSS){sc_apply();return 1;}
  if(pressed&PSP_CTRL_CIRCLE){sc_menu=SC_MENU_NONE;return 1;}
  return 1;
 }
 if(sc_menu==SC_MENU_TALK){
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  const char *ch[4]; int cn=0;
  if(sc_talk_who>=0&&sc_talk_who<pn)cn=sc_talk_choices(&people[sc_talk_who],ch,4);
  if(cn<1)cn=1;
  if(pressed&PSP_CTRL_UP){sc_talk_row=(sc_talk_row+cn-1)%cn;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_DOWN){sc_talk_row=(sc_talk_row+1)%cn;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_CROSS){sc_apply();return 1;}
  if(pressed&PSP_CTRL_CIRCLE||pressed&PSP_CTRL_TRIANGLE){sc_menu=SC_MENU_NONE;return 1;}
  return 1;
 }
 ScHot hot[24]; int hn=sc_hotspots(hot,24);if(hn<1)hn=1;
 if(pressed&PSP_CTRL_LEFT){sc_verb=(sc_verb+SC_V_COUNT-1)%SC_V_COUNT;sc_snap_verb_hot();game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_RIGHT){sc_verb=(sc_verb+1)%SC_V_COUNT;sc_snap_verb_hot();game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_UP){sc_hot=(sc_hot+hn-1)%hn;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_DOWN){sc_hot=(sc_hot+1)%hn;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_CROSS)sc_apply();
 return 0;
}
