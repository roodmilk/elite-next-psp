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
 /* Keep people off side hatches and free the hero focal zone. */
 static const int pos[SC_R_COUNT][3][2]={
  {{70,94},{200,90},{0,0}},
  {{110,98},{210,92},{0,0}},
  {{90,96},{210,94},{0,0}},
  {{56,98},{0,0},{0,0}},
  {{86,96},{210,94},{0,0}},
  {{120,96},{0,0},{0,0}},
  {{160,96},{0,0},{0,0}}
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
  "One bright berth window. A freighter slips the lane. Board waits when you look.",
  "Cream walls, ochre counter. One ledger — deals happen here.",
  "Warm bar top under a single practical. Rumours sit in the booth.",
  "Three crates, one tip mark, cool hoist above. Keep the lane clear.",
  "Tracked slate owns the room. Desk paper stays secondary.",
  "One treatment bay, one cool lamp, one medkit locker.",
  "Scanner gate reads first. Warrant glass keeps your heat honest."
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
/* Focal anchors only (art handoff: 3–5 interactables + people/doors/ship).
 * Doors sit as side hatches — never over the hero focal object. */
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
 /* Side hatch layout: primary left, secondary right, overflow mid-bottom (clear of SHIP). */
 static const int door_xy[4][2]={{8,52},{258,52},{78,128},{136,128}};
 for(int i=0;i<en&&i<4;i++){
  int dx=VX+door_xy[i][0],dy=VY+door_xy[i][1];
  int dw=i<2?50:42,dh=i<2?60:26;
  SC_HOT(SC_H_EXIT,ex[i],dx,dy,dw,dh,sc_room_short(ex[i]),"Door. GO + X walks through. Also listed under EXITS.");
 }
 /* Ship return — every room, amber, bottom-right of MAIN (clear of hero + overflow doors). */
 SC_HOT(SC_H_EXIT,SC_EXIT_SHIP,VX+244,VY+128,68,28,"YOUR SHIP","Board your ship now. TRI also boards. EXITS > SHIP.");
 if(sc_room==SC_R_ARRIVALS){
  SC_HOT(SC_H_FEATURE,1,VX+72,VY+10,176,70,"BERTH WINDOW","Warm spill from the lane. One freighter silhouette.");
  SC_HOT(SC_H_FEATURE,0,VX+196,VY+84,90,40,"TRAFFIC BOARD","Berths and delays. Cool cyan signal only.");
  SC_HOT(SC_H_PROP,0,VX+72,VY+118,160,12,"RAIL","Handrail over the freighter lane.");
 }else if(sc_room==SC_R_SHOP){
  SC_HOT(SC_H_FEATURE,8,VX+88,VY+96,150,36,"COUNTER","Ochre counter. Deals happen here.");
  SC_HOT(SC_H_FEATURE,1,VX+118,VY+36,70,42,"LEDGER","Hand ink. Prices the main board never prints.");
  SC_HOT(SC_H_PROP,1,VX+196,VY+100,60,36,"STOCK CRATE","Exclusive clamps and spares. SPEAK to buy.");
 }else if(sc_room==SC_R_CANTEEN){
  SC_HOT(SC_H_PROP,2,VX+64,VY+100,200,30,"BAR TOP","One warm practical. Tip jar at the end.");
  SC_HOT(SC_H_FEATURE,2,VX+16,VY+72,40,48,"JUKE","Soft Transit loop. Not a nightclub.");
  SC_HOT(SC_H_PROP,13,VX+20,VY+108,48,28,"RUMOUR BOOTH","Travellers leave tip notes in the seat.");
 }else if(sc_room==SC_R_CARGO){
  SC_HOT(SC_H_PROP,3,VX+128,VY+72,64,48,"TIP CRATE","Marked for the board. SPEAK to the loader.");
  SC_HOT(SC_H_FEATURE,5,VX+48,VY+8,224,16,"CARGO LIFT","Overhead hoist. Industrial cool light.");
  SC_HOT(SC_H_PROP,17,VX+72,VY+40,48,24,"MANIFEST","Today's tip jobs. Loader keeps the pen.");
 }else if(sc_room==SC_R_GUILD){
  SC_HOT(SC_H_FEATURE,3,VX+96,VY+16,128,60,"TRACKED SCREEN","Mission slate. Kei points here when you stall.");
  SC_HOT(SC_H_PROP,7,VX+64,VY+108,192,28,"GUILD DESK","Sage survey gear and cream paper strips.");
 }else if(sc_room==SC_R_CLINIC){
  SC_HOT(SC_H_PROP,8,VX+80,VY+96,160,40,"TREATMENT BAY","Cool slate bench. Rest if you need it.");
  SC_HOT(SC_H_PROP,4,VX+244,VY+48,52,72,"MEDKIT LOCKER","One issue. TAKE after SPEAK with the medic.");
  SC_HOT(SC_H_FEATURE,12,VX+72,VY+40,48,36,"DIAG PANEL","Soft beeps. Shape beats hue for readability.");
 }else if(sc_room==SC_R_CUSTOMS){
  SC_HOT(SC_H_FEATURE,6,VX+72,VY+40,40,90,"SCANNER GATE","Walk through only if you like questions.");
  SC_HOT(SC_H_FEATURE,4,VX+140,VY+16,120,52,"WARRANT GLASS","Your legal heat on navy glass.");
  SC_HOT(SC_H_PROP,23,VX+120,VY+112,140,28,"INSPECT DESK","Stamps, forms, a tired officer.");
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
 if(sc_verb==SC_V_GO){
  /* Prefer a room hatch first; fall back to YOUR SHIP so GO always lands on travel. */
  int i=sc_find_hot(SC_H_EXIT,-1);
  ScHot hot[24]; int hn=sc_hotspots(hot,24);
  int ship=-1,door=-1;
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT){if(hot[h].id==SC_EXIT_SHIP)ship=h;else if(door<0)door=h;}
  if(door>=0)sc_hot=door;else if(ship>=0)sc_hot=ship;else if(i>=0)sc_hot=i;
 }else if(sc_verb==SC_V_SPEAK){int i=sc_find_hot(SC_H_PERSON,-1);if(i>=0)sc_hot=i;}
 else if(sc_verb==SC_V_TAKE){
  int i=sc_find_hot(SC_H_PERSON,-1);
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  for(int p=0;p<pn;p++)if(people[p].act==SC_ACT_GIFT||people[p].act==SC_ACT_SHOP||people[p].act==SC_ACT_QUEST||people[p].act==SC_ACT_TAXI){int j=sc_find_hot(SC_H_PERSON,p);if(j>=0){i=j;break;}}
  if(i>=0)sc_hot=i;
 }
}
/* ---- drawing helpers (A++ cinematic: hero focus, 3 planes, warm staging) ---- */
static void sc_win(int x,int y,int w,int h,const char *title,unsigned edge){
 /* Thin cream/cyan instruments — chrome recedes so MAIN owns the eye. */
 rect(x,y,w,h,mix_rgb(SC_VOID,SC_CHAR,.55f));
 rect(x,y,w,10,mix_rgb(SC_CHAR,SC_CREAM,.12f));
 rect(x,y,w,1,mix_rgb(edge,SC_CREAM,.35f));
 rect(x,y+h-1,w,1,mix_rgb(edge,SC_VOID,.4f));
 rect(x,y,1,h,mix_rgb(edge,SC_CREAM,.25f));
 rect(x+w-1,y,1,h,mix_rgb(edge,SC_VOID,.35f));
 text((x+6)/8,(y+1)/8,mix_rgb(SC_CREAM,edge,.2f),"%.14s",title);
}
static void sc_scene_sky(int x,int y,int w,int h,unsigned hi,unsigned lo){
 for(int row=0;row<h;row++){
  float t=row/(float)(h>1?h-1:1);
  rect(x,y+row,w,1,mix_rgb(hi,lo,t));
 }
}
/* Three floor value masses — no checker texture that dies at 1×. */
static void sc_floor_planes(int x,int y,int w,int h,unsigned nearc,unsigned midc,unsigned farc){
 int band=h/5;if(band<10)band=10;
 rect(x,y+h-band*3,w,band,farc);
 rect(x,y+h-band*2,w,band,midc);
 rect(x,y+h-band,w,band,nearc);
}
static void sc_warm_key(int cx,int cy,unsigned lamp){
 rect(cx-10,cy,20,3,mix_rgb(lamp,SC_CREAM,.4f));
 rect(cx-4,cy+3,8,2,lamp);
 pixel(cx,cy-2,SC_CREAM);
}
static void sc_hatch(int x,int y,int w,int h,unsigned frame,unsigned aperture,int hero){
 rect(x,y,w,h,frame);
 rect(x+3,y+4,w-6,h-8,aperture);
 if(hero){
  rect(x+1,y+1,w-2,2,SC_CREAM);
  rect(x+w/2-2,y+h/2-6,4,12,mix_rgb(SC_AMBER,frame,.4f));
 }else{
  rect(x+w/2-1,y+h/2-4,2,8,mix_rgb(SC_CYAN,frame,.35f));
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
 /* Contact shadow — keep silhouette friendly, two-tone accents only. */
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
 /* Hero: berth window at upper third. Layers: void lane / cream hall / rail. */
 sc_scene_sky(x,y,w,h,mix_rgb(SC_OCHRE,st->wall,.45f),mix_rgb(st->wall2,SC_RUST,.25f));
 sc_floor_planes(x,y,w,h,mix_rgb(SC_CREAM,st->wall,.35f),mix_rgb(st->wall,st->wall2,.4f),st->wall2);
 /* Distant coral haze through the aperture */
 rect(x+72,y+10,176,70,SC_VOID);
 rect(x+72,y+10,176,3,SC_CREAM);
 rect(x+72,y+10,3,70,mix_rgb(SC_CREAM,st->trim,.4f));
 rect(x+245,y+10,3,70,mix_rgb(SC_CREAM,st->trim,.4f));
 /* Soft warm spill */
 for(int row=0;row<28;row++)rect(x+78,y+16+row,164,1,mix_rgb(SC_VOID,SC_OCHRE,row/40.f));
 /* One freighter silhouette — scale cue, not a fleet */
 rect(x+130,y+40,54,10,st->wall);
 rect(x+140,y+34,28,8,mix_rgb(st->wall,SC_CREAM,.25f));
 rect(x+178,y+42,16,6,st->wall2);
 pixel(x+120,y+30,SC_CREAM);pixel(x+200,y+26,SC_LAV);pixel(x+160,y+22,st->lamp);
 sc_warm_key(x+160,y+84,st->lamp);
 /* Mid: traffic board — cyan signal only */
 rect(x+196,y+84,90,40,st->wall2);
 rect(x+200,y+88,82,12,mix_rgb(st->accent,st->wall,.4f));
 rect(x+200,y+104,82,8,mix_rgb(SC_OLIVE,st->accent,.25f));
 rect(x+200,y+116,40,4,st->lamp);
 text((x+204)/8,(y+90)/8,st->accent,"BERTHS");
 /* Fore: rail */
 rect(x+72,y+118,160,4,st->trim);
 rect(x+72,y+122,160,3,SC_OLIVE);
}
static void sc_illust_shop(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: ochre counter. Cream walls, two shelf masses only. */
 sc_scene_sky(x,y,w,h,mix_rgb(SC_CREAM,st->wall,.4f),st->wall2);
 sc_floor_planes(x,y,w,h,mix_rgb(SC_RUST,st->trim,.45f),mix_rgb(st->trim,st->wall,.35f),st->wall2);
 /* Back shelves — two blocks, not a grid of parts */
 rect(x+64,y+12,70,48,mix_rgb(st->wall,st->trim,.3f));
 rect(x+70,y+18,18,14,st->lamp);rect(x+94,y+18,18,14,st->accent);rect(x+118,y+18,10,14,SC_RUST);
 rect(x+186,y+12,70,48,mix_rgb(st->wall,st->trim,.3f));
 rect(x+192,y+18,18,14,SC_LAV);rect(x+216,y+18,18,14,st->trim);
 /* Mid: ledger */
 rect(x+118,y+36,70,42,mix_rgb(st->wall,SC_RUST,.25f));
 rect(x+124,y+42,58,22,SC_CREAM);
 text((x+128)/8,(y+48)/8,st->lamp,"LEDGER");
 /* Hero counter */
 rect(x+88,y+96,150,36,SC_RUST);
 rect(x+88,y+90,150,8,st->trim);
 rect(x+96,y+104,40,16,st->wall2);
 rect(x+196,y+100,60,28,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+200,y+104,52,6,SC_CREAM);
 sc_warm_key(x+160,y+82,st->lamp);
}
static void sc_illust_canteen(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: bar top. Four bottles, one booth, one juke. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,SC_OCHRE,.3f),st->wall2);
 sc_floor_planes(x,y,w,h,mix_rgb(st->trim,st->wall,.4f),st->wall,st->wall2);
 /* Back bar wall + few bottles */
 rect(x+64,y+12,192,44,mix_rgb(st->wall,st->trim,.2f));
 for(int i=0;i<4;i++){
  unsigned c=mix_rgb(SC_CREAM,st->trim,(i%3)*.2f);
  rect(x+88+i*36,y+18,10,28,c);pixel(x+91+i*36,y+22,st->lamp);
 }
 rect(x+64,y+56,192,3,st->accent);
 /* Hero bar */
 rect(x+64,y+100,200,30,mix_rgb(st->trim,st->wall,.35f));
 rect(x+64,y+94,200,8,st->lamp);
 rect(x+240,y+88,12,12,mix_rgb(SC_CREAM,st->lamp,.35f));
 /* Juke + booth — secondary */
 rect(x+16,y+72,40,48,SC_SLATE);
 rect(x+20,y+76,32,18,mix_rgb(st->lamp,st->wall,.4f));
 text((x+18)/8,(y+108)/8,SC_CREAM,"JUKE");
 rect(x+20,y+108,48,28,mix_rgb(st->trim,st->wall2,.35f));
 sc_warm_key(x+160,y+70,st->lamp);
}
static void sc_illust_cargo(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: tip crate centre. Three crates, one cool hoist. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->trim,.15f),st->wall2);
 sc_floor_planes(x,y,w,h,mix_rgb(SC_OLIVE,st->wall,.4f),st->wall,st->wall2);
 /* Cool lift signal */
 rect(x+48,y+8,224,4,SC_LAV);
 rect(x+w/2-3,y+8,6,40,st->trim);
 rect(x+w/2-14,y+44,28,8,st->wall);
 /* Three crates — centre is tip hero */
 for(int i=0;i<3;i++){
  int cx=x+72+i*72;
  rect(cx,y+h-88,56,40,st->trim);
  rect(cx,y+h-88,56,5,SC_CREAM);
  rect(cx+16,y+h-72,22,8,st->wall2);
  if(i==1){rect(cx+6,y+h-100,44,14,st->lamp);text((cx+10)/8,(y+h-98)/8,st->wall2,"TIP");}
 }
 /* Manifest secondary */
 rect(x+72,y+40,48,24,mix_rgb(st->wall,SC_CREAM,.25f));
 rect(x+76,y+44,40,6,st->accent);
 rect(x+48,y+h-52,w-96,4,st->lamp);
}
static void sc_illust_guild(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: tracked screen. Desk secondary. Olive/cream calm. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->wall2,.45f),st->wall2);
 sc_floor_planes(x,y,w,h,mix_rgb(st->wall,SC_CREAM,.2f),st->wall,st->wall2);
 rect(x+96,y+16,128,60,st->wall2);
 rect(x+102,y+22,116,36,mix_rgb(st->wall,st->accent,.35f));
 text((x+108)/8,(y+30)/8,st->accent,"TRACKED MISSION");
 rect(x+108,y+58,100,8,st->lamp);
 rect(x+64,y+108,192,28,mix_rgb(st->wall,st->wall2,.4f));
 rect(x+64,y+102,192,8,mix_rgb(st->trim,st->wall,.4f));
 rect(x+72,y+114,40,12,SC_CREAM);
 sc_warm_key(x+160,y+80,st->lamp);
}
static void sc_illust_clinic(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: treatment bay. One cool lamp, one locker. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->trim,st->wall,.3f),st->wall2);
 sc_floor_planes(x,y,w,h,mix_rgb(st->trim,st->wall2,.4f),st->wall,st->wall2);
 /* One practical cool key — not a string of discs */
 circle(x+160,y+18,8,mix_rgb(st->lamp,st->accent,.45f));
 rect(x+154,y+26,12,3,st->trim);
 /* Diag panel secondary */
 rect(x+72,y+40,48,36,st->wall2);
 rect(x+76,y+44,40,20,mix_rgb(st->lamp,SC_OLIVE,.35f));
 /* Hero bay */
 rect(x+80,y+96,160,40,mix_rgb(st->trim,st->accent,.45f));
 rect(x+80,y+90,160,8,mix_rgb(st->lamp,st->wall,.4f));
 rect(x+90,y+108,40,16,st->accent);
 /* Medkit locker */
 rect(x+244,y+48,52,72,st->wall);
 rect(x+250,y+56,40,28,mix_rgb(st->lamp,st->accent,.35f));
 text((x+252)/8,(y+64)/8,st->wall2,"MED");
 rect(x+250,y+92,40,16,st->wall2);
}
static void sc_illust_customs(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: scanner gate aperture. Warrant secondary. Cobalt/navy calm. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->trim,.12f),st->wall2);
 sc_floor_planes(x,y,w,h,mix_rgb(st->wall,SC_CREAM,.15f),st->wall,st->wall2);
 /* Gate posts + warm scan aperture */
 rect(x+72,y+40,12,90,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+100,y+40,12,90,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+72,y+40,40,6,st->trim);
 for(int i=0;i<4;i++)rect(x+78,y+52+i*18,28,2,mix_rgb(st->accent,SC_VOID,.5f));
 /* Warrant glass */
 rect(x+140,y+16,120,52,st->wall2);
 rect(x+146,y+22,108,18,st->trim);
 text((x+156)/8,(y+26)/8,SC_CREAM,"WARRANT");
 rect(x+146,y+44,108,16,SC_VOID);
 text((x+160)/8,(y+46)/8,st->accent,"SCAN");
 /* Inspect desk */
 rect(x+120,y+112,140,28,st->wall);
 rect(x+120,y+108,140,6,mix_rgb(SC_CREAM,st->wall,.35f));
 sc_warm_key(x+190,y+96,st->lamp);
}
static void sc_draw_main_scene(void){
 const int VX=72,VY=24,VW=320,VH=160;
 sc_win(VX-2,VY-14,VW+4,VH+18,"MAIN",SC_CYAN);
 text((VX+6)/8,(VY-12)/8,mix_rgb(SC_CREAM,SC_CYAN,.3f),"%.16s",sc_room_title(sc_room));
 if(sc_room==SC_R_ARRIVALS)sc_illust_arrivals(VX,VY,VW,VH);
 else if(sc_room==SC_R_SHOP)sc_illust_shop(VX,VY,VW,VH);
 else if(sc_room==SC_R_CANTEEN)sc_illust_canteen(VX,VY,VW,VH);
 else if(sc_room==SC_R_CARGO)sc_illust_cargo(VX,VY,VW,VH);
 else if(sc_room==SC_R_GUILD)sc_illust_guild(VX,VY,VW,VH);
 else if(sc_room==SC_R_CLINIC)sc_illust_clinic(VX,VY,VW,VH);
 else sc_illust_customs(VX,VY,VW,VH);
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 /* Side hatches first — primary exit reads as cream hero door, others cyan. */
 {
  int exit_ord=0;
  for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_EXIT&&hot[i].id!=SC_EXIT_SHIP){
   int hero=exit_ord==0;
   unsigned frame=(hero||sc_hot==i)?mix_rgb(SC_CREAM,SC_OCHRE,.35f):mix_rgb(SC_SLATE,SC_CREAM,.2f);
   unsigned aperture=(hero||sc_hot==i)?mix_rgb(SC_VOID,SC_OCHRE,.2f):SC_VOID;
   sc_hatch(hot[i].x,hot[i].y,hot[i].w,hot[i].h,frame,aperture,hero||sc_hot==i);
   text((hot[i].x+4)/8,(hot[i].y+hot[i].h-12)/8,sc_hot==i?SC_AMBER:(hero?SC_CREAM:SC_CYAN),"%.7s",hot[i].label);
   exit_ord++;
  }
 }
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
 for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_EXIT&&hot[i].id==SC_EXIT_SHIP){
  unsigned c=(sc_hot==i)?SC_AMBER:SC_AMBER;
  rect(hot[i].x,hot[i].y,hot[i].w,hot[i].h,mix_rgb(SC_CHAR,SC_OCHRE,.2f));
  rect(hot[i].x,hot[i].y,hot[i].w,1,c);
  rect(hot[i].x,hot[i].y+hot[i].h-1,hot[i].w,1,c);
  rect(hot[i].x+1,hot[i].y+1,hot[i].w-2,hot[i].h-2,mix_rgb(SC_AMBER,SC_CHAR,.22f));
  text((hot[i].x+6)/8,(hot[i].y+8)/8,c,">> SHIP");
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
  if(i==sc_verb){rect(x-2,6,78,12,mix_rgb(SC_OCHRE,SC_CHAR,.3f));rect(x-2,6,78,1,SC_AMBER);}
  text(x/8,1,i==sc_verb?SC_AMBER:mix_rgb(SC_LAV,SC_CREAM,.35f),"%s",v[i]);
 }
 text(48,1,SC_AMBER,"TRI");text(52,1,SC_CREAM,"BOARD");
}
static void sc_draw_inventory(void){
 sc_win(4,24,64,160,"PACK",mix_rgb(SC_CREAM,SC_AMBER,.3f));
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
 sc_win(400,24,76,160,"EXITS",mix_rgb(SC_CREAM,SC_CYAN,.35f));
 int y=4;
 {int sel=0;ScHot hot[24];int hn=sc_hotspots(hot,24);
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT&&hot[h].id==SC_EXIT_SHIP&&h==sc_hot)sel=1;
  rect(404,y*8-2,68,16,sel?mix_rgb(SC_AMBER,SC_CHAR,.35f):mix_rgb(SC_OCHRE,SC_CHAR,.15f));
  text(51,y,SC_AMBER,">>SHIP");y+=2;}
 text(51,y,SC_LAV,"doors");y+=2;
 int ex[4],en=sc_exits(sc_room,ex,4);
 for(int i=0;i<en;i++){
  int sel=0;ScHot hot[24];int hn=sc_hotspots(hot,24);
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT&&hot[h].id==ex[i]&&h==sc_hot)sel=1;
  if(sel)rect(404,y*8-2,68,14,mix_rgb(SC_CYAN,SC_CHAR,.25f));
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
 /* Warm void wash — not flat black dashboard chrome. */
 rect(0,0,W,H,mix_rgb(SC_VOID,SC_OCHRE,.08f));
 for(int i=0;i<H;i+=6)rect(0,i,W,1,mix_rgb(SC_CHAR,SC_VOID,.5f));
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
   if(h->id>=0&&h->id<pn&&(people[h->id].act==SC_ACT_QUEST||people[h->id].act==SC_ACT_TAXI)){
    sc_menu=SC_MENU_TALK;sc_talk_who=h->id;sc_talk_row=1;message(&game,"Offer sits in SPEAK — pick the deal.");game.cue=SFX_UI;return;
   }
  }
  /* Props with an owner: route TAKE into the fun SPEAK deal instead of a dead end. */
  if(h->kind==SC_H_PROP||h->kind==SC_H_FEATURE){
   int owner=sc_find_hot(SC_H_PERSON,-1);
   if(owner>=0&&(!strcmp(h->label,"TIP CRATE")||!strcmp(h->label,"STOCK CRATE")||!strcmp(h->label,"MEDKIT LOCKER")||!strcmp(h->label,"COUNTER")||!strcmp(h->label,"BAR TOP")||!strcmp(h->label,"GUILD DESK"))){
    ScHot hot2[24]; int hn2=sc_hotspots(hot2,24);
    if(owner<hn2&&hot2[owner].kind==SC_H_PERSON){
     sc_hot=owner;sc_verb=SC_V_SPEAK;sc_menu=SC_MENU_TALK;sc_talk_who=hot2[owner].id;sc_talk_row=1;
     message(&game,"Ask them — TAKE opens their deal.");game.cue=SFX_UI;return;
    }
   }
   if(h->kind==SC_H_PROP){message(&game,"Bolted down. SPEAK to whoever owns it.");game.cue=SFX_UI;return;}
  }
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
