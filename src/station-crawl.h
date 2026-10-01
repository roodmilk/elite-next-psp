/* MacVenture-style station: illustrated rooms + verb/hotspot point-and-click.
 * Shadowgate / Deja Vu grammar. Soft-FB. Talk/shop/gift/taxi preserved.
 * Soft look from ART DIRECTOR bake kit (PR #11 handoff + PR #12 station-art-kit.h).
 * Palette+styles via station-art-kit.h; this file owns geometry/input/state. */
#include "station-art-kit.h"
#include "native-art-scenes.h"
#include "station-bar-art.h"
#include "station-identity.h"
enum {
 SC_R_ARRIVALS=0, SC_R_SHOP, SC_R_CANTEEN, SC_R_CARGO, SC_R_GUILD, SC_R_CLINIC, SC_R_CUSTOMS, SC_R_COUNT
};
enum { SC_V_LOOK=0, SC_V_SPEAK, SC_V_GO, SC_V_TAKE, SC_V_COUNT };
enum { SC_H_NONE=0, SC_H_HERE, SC_H_PERSON, SC_H_EXIT, SC_H_PROP, SC_H_FEATURE };
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
#include "station-shell.h"
#include "station-authored-art.h"
#include "station-arrivals-authored-art.h"
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
static StationIdentity sc_identity(void){return station_identity_for(&game);}
/* Activities are keyed to the canonical system/hub, never a shared gift bit. */
static uint32_t *sc_progress(void){return &game.station_progress[game.system][game.station_variant];}
static int sc_lave(void){return game.system==7&&game.station_variant==0&&!tutorial_active(&game);}
static int sc_lave_stage(void){return (game.station_progress[7][0]>>2)&15;}
static void sc_lave_set(int stage){game.station_progress[7][0]=(game.station_progress[7][0]&~60u)|((unsigned)stage<<2);}
static int sc_manifest_state(void){return *sc_progress()&3;}
static unsigned sc_manifest_key(void){StationIdentity s=sc_identity();return s.seed&0xffffu;}
static void sc_manifest_set(int state){*sc_progress()=(*sc_progress()&~3u)|(state&3);}
static int sc_manifest_reward(void){StationIdentity s=sc_identity();return station_identity_manifest_reward(&s);}
static void sc_manifest_prepare(void){
 unsigned stored=(unsigned)game.gift_flags>>10&0xffffu;
 if(stored==sc_manifest_key()&&(*sc_progress()&3)==0){
  unsigned legacy=((unsigned)game.gift_flags>>8)&3;
  if(legacy<=2)*sc_progress()|=legacy;
  game.gift_flags&=0xff;
 }
}
static const char *sc_lave_objective(void){
 static const char *steps[]={
 "Lave Hub: meet Mara, the Cargo Bay loader.",
 "Find the loose MANIFEST in the Cargo Bay.",
 "Ask Niko in the Canteen who moved the medical crate.",
 "Get a replacement seal from Ada in the Chandlery.",
 "Ask Dr Sen in the Clinic to verify the medicine list.",
 "Ask Officer Orin in Customs to clear the corrected manifest.",
 "Return the cleared manifest to Mara in the Cargo Bay.",
 "Meet Iona at the Guild Desk about Lave I survey work.",
 "Scan life or minerals on Lave I; return to Iona at Lave Hub.",
 "Berth Six complete. Lave's medicine shipment is moving."
 };int stage=sc_lave_stage();return steps[stage>=0&&stage<=9?stage:0];
}

typedef struct { const char *name; int role; int act; int shop_item; int gift_bit; int quest_pay; int taxi_pay; const char *line; const char *offer; } ScNpc;
#include "station-bar-preview.h"
enum { SC_ACT_TALK=0, SC_ACT_SHOP, SC_ACT_GIFT, SC_ACT_QUEST, SC_ACT_TAXI, SC_ACT_BOARD, SC_ACT_MANIFEST };
#include "station-room-kit.h"
typedef struct { int kind; int id; int x,y,w,h; const char *label; const char *look; } ScHot;
static void sc_person_pos(int room,int i,int *ox,int *oy){
 if(room==SC_R_CANTEEN&&bar_preview_at()){second_shift_person_pos(i,ox,oy);return;}
 if(sc_lave()&&room==SC_R_ARRIVALS&&i==0){*ox=145;*oy=77;return;}
 if(sc_raster_room()&&room>=0&&room<7&&i>=0&&i<2){*ox=sc_recipes[room].people[i].x;*oy=sc_recipes[room].people[i].y;return;}
 /* Keep people inside the narrower MAIN (options list owns the right). */
 static const int pos[SC_R_COUNT][3][2]={
  {{90,88},{210,84},{0,0}},
  {{100,90},{220,86},{0,0}},
  {{90,88},{220,86},{0,0}},
  {{80,90},{0,0},{0,0}},
  {{90,88},{210,86},{0,0}},
  {{120,88},{0,0},{0,0}},
  {{150,88},{0,0},{0,0}}
 };
 if(room<0||room>=SC_R_COUNT)room=0;if(i<0)i=0;if(i>2)i=2;
 *ox=pos[room][i][0];*oy=pos[room][i][1];
}
static const char *sc_room_title(int r){
 if(r==SC_R_ARRIVALS&&station_authored_arrivals_at())return "REORTE TRANSIT";
 if(r==SC_R_CANTEEN&&bar_preview_at())return "THE SECOND SHIFT";
 static const char *n[]={"ARRIVALS HALL","CHANDLERY","CANTEEN","CARGO BAY","GUILD DESK","MED CLINIC","CUSTOMS LOCK"};
 return r>=0&&r<SC_R_COUNT?n[r]:"DECK";
}
static const char *sc_room_short(int r){
 if(r==SC_R_CANTEEN&&bar_preview_at())return "SECOND SHIFT";
 static const char *n[]={"ARRIVAL","SHOP","CANTEEN","CARGO","GUILD","CLINIC","CUSTOMS"};
 return r>=0&&r<SC_R_COUNT?n[r]:"DECK";
}
static const char *sc_room_blurb(int r){
 if(r==SC_R_CANTEEN&&bar_preview_at())return "The Second Shift is warm enough that you notice yourself relaxing as the door shuts. Light from the bar catches the edges of glasses and the worn surfaces of the booths. Outside the window, traffic continues its slow procession around Reorte. Lysa Kest, Pell Sorn and Dax Neral have settled into their usual corners; none of them looks in a hurry to leave.";
 static const char *n[]={
 "Beyond the arrivals window, a freighter hangs against the light of the planet. Its bulk makes the ships passing beneath it look impossibly small. The hall smells faintly of warm machinery and the cleaning fluid used on the deck. Worn seats face the berth board, where crews pause to check their next departure. Corridors on either side lead deeper into the station. For a moment, nobody is asking anything of you.",
 "The chandlery is packed with the things a ship needs after the glamour of flying it has worn off: seals, cables, spare fittings and tools polished by years of handling. A clear strip of counter separates the working stock from the pieces still waiting to be checked. The shelves look crowded, but the staff seem to know exactly where everything belongs. You can speak to the Chandler about deck stock or use the outfitting terminal to inspect fitted modules.",
 "The canteen is warmer than the corridor outside. Crockery knocks softly against the counter, and the jukebox sits within reach of the booths. From here the view of the planet almost makes you forget how much machinery is between your feet and open space. This is where crews stop being callsigns for a while. Some come for a meal; others stay because there is no particular reason to go back to an empty cabin yet.",
 "The cargo bay is full of restrained weight: stacked containers, loaded pallets and a lifting rig suspended above the work area. Scuffed paint marks the routes taken by a thousand previous deliveries. Near the rail, loose paperwork waits beside the cargo records. Every crate has somewhere it is meant to go, but a berth can come to a halt over something as small as a missing signature. The loader keeps an eye on you without abandoning the next job.",
 "The Guild office is quieter than the public concourse. A large star chart fills the rear wall, while specimens and well-used reference books compete for space around the desk. The displays make distant places look orderly; the notes beside them suggest the people who visited had a less straightforward experience. This is a place for comparing observations, asking careful questions and turning a pilot's travels into records that someone else can use.",
 "The clinic is clean without feeling untouched. Supplies are arranged within easy reach of the treatment couch, and a diagnostic display casts a cool light across the room. A trolley stands ready for the next patient. The doctor has left a little space between the equipment and the doorway, enough for someone to come in without feeling they are interrupting an emergency. After the noise of the cargo decks, even the ordinary hum of the ventilation seems reassuring.",
 "The customs scanner divides the room into a before and an after. On one side sit containers still waiting to be cleared; on the other, a desk carries the records that will allow them to move. The officer works through each entry with the patience of someone who has heard every explanation twice. Through the window you can see ships that have already finished this part of their journey. Yours is not the only crew hoping the next form will be the last."
 };return r>=0&&r<SC_R_COUNT?n[r]:"";
}
#include "station-reading.h"
#include "station-lave.h"
static int sc_fill_npcs(int room,ScNpc *out,int maxn){
 int n=0; unsigned h=(unsigned)(game.system*131u+room*41u);
 #define SC_PUSH(N,R,A,SI,GB,QP,TP,L,O) do{if(n<maxn){out[n].name=(N);out[n].role=(R);out[n].act=(A);out[n].shop_item=(SI);out[n].gift_bit=(GB);out[n].quest_pay=(QP);out[n].taxi_pay=(TP);out[n].line=(L);out[n].offer=(O);n++;}}while(0)
 if(sc_lave()){
  SC_PUSH(lave_names[room],room==SC_R_CUSTOMS||room==SC_R_CLINIC?LAW:room==SC_R_GUILD?EXPLORERS:TRADERS,40+room,-1,-1,0,0,lave_line(room),lave_offer(room));
  if(room==SC_R_SHOP)SC_PUSH("CHANDLER",TRADERS,SC_ACT_SHOP,-1,-1,0,0,"Supplies, fitted modules and a fair trade-in. Keep the lanes fed.","Browse deck stock");
  return n;
 }
 if(room==SC_R_ARRIVALS){
  SC_PUSH("VENN",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Berth six stays warm if you keep coming home.","Ask about local traffic");
  SC_PUSH("DOCKHAND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Watch the freighter lane. Pads are tight.","Ask about berths and fuel");
 }else if(room==SC_R_SHOP){
  SC_PUSH("CHANDLER",TRADERS,SC_ACT_SHOP,-1,-1,0,0,"I stock what the main board won't list.","Browse exclusive stock");
  if((h&3)==0)SC_PUSH("MECHANIC",TRADERS,SC_ACT_GIFT,-1,0,0,0,"Take a spare clamp. Once. Don't ask twice.","Accept free clamp");
 }else if(room==SC_R_CANTEEN){
  if(bar_preview_at()){
   SC_PUSH("LYSA KEST",TRADERS,SC_ACT_BAR_LYSA,-1,-1,0,0,bar_preview_notes[0],"Ask about REORTE I");
   SC_PUSH("PELL SORN",EXPLORERS,SC_ACT_BAR_PELL,-1,-1,0,0,bar_preview_notes[1],"Target REORTE I");
   SC_PUSH("DAX NERAL",TRADERS,SC_ACT_BAR_DAX,-1,-1,0,0,bar_preview_notes[2],"Free dice practice");
  }else{
  SC_PUSH("BARTEND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Meridian tips well. Truth tips better.","Ask for a rumour");
  SC_PUSH("TRAVELER",EXPLORERS,SC_ACT_TAXI,-1,-1,0,1800,"Need a lift to another hub. One tonne seat.","Book taxi berth");
  }
 }else if(room==SC_R_CARGO){
  sc_manifest_prepare();
  SC_PUSH("LOADER",TRADERS,SC_ACT_MANIFEST,-1,-1,sc_manifest_reward(),0,
   sc_manifest_state()==0?"Berth six has a crate with no manifest. Find the loose sheet, then bring it back.":
   sc_manifest_state()==1?"You found it. Hand over the manifest before the next lift cycle.":
   "Manifest filed. The berth can move again.",
   sc_manifest_state()==0?"Search for the missing manifest":
   sc_manifest_state()==1?"Return the missing manifest":"Manifest already filed");
 }else if(room==SC_R_CLINIC){
  SC_PUSH("MEDIC",LAW,SC_ACT_GIFT,-1,1,0,0,"Medkit for the next scrape. One issue only.","Take free medkit");
 }else if(room==SC_R_GUILD){
  SC_PUSH("KEI",EXPLORERS,SC_ACT_TALK,-1,-1,0,0,"When you are ready, open Tracked Mission.","Ask about Ryn / next step");
  SC_PUSH("SURVEYOR",EXPLORERS,SC_ACT_QUEST,-1,-1,600,0,"Fresh surface sample pays. Log it honest.","Accept survey tip");
 }else if(room==SC_R_CUSTOMS){
  SC_PUSH("CUSTOMS",LAW,SC_ACT_TALK,-1,-1,0,0,"Restricted goods matter when Law scans.","Ask about your heat");
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
 int identity=game.system*HUB_COUNT+game.station_variant;
 if(sc_built_for==identity)return;
 sc_manifest_prepare();
 sc_built_for=identity; sc_room=SC_R_ARRIVALS; sc_verb=SC_V_LOOK; sc_hot=0; sc_menu=0; sc_shop_row=0; sc_talk_row=0; sc_talk_who=0;
 sc_x=0;sc_y=0;sc_face=SC_S;sc_map[0][0]=SC_R_ARRIVALS;sc_door_n[0][0]=1;sc_door_e[0][0]=1;sc_read_room();
}
static int sc_door_dir(int dir){(void)dir;return 1;}
static int sc_door_ahead(void){return 1;}
/* Shared MAIN + right options list — no verb row, no PACK chrome. */
enum { SC_VX=NATIVE_ART_STATION_X, SC_VY=NATIVE_ART_STATION_Y, SC_VW=NATIVE_ART_STATION_W, SC_VH=NATIVE_ART_STATION_H, SC_LX=354, SC_LY=20, SC_LW=120, SC_LH=168 };
/* Focal anchors only (art handoff: 3–5 interactables + people/doors/ship).
 * Doors sit as side hatches — never over the hero focal object. */
static int sc_hotspots(ScHot *out,int maxn){
 int n=0;
 #define SC_HOT(K,ID,X,Y,W,H,L,LOOK) do{if(n<maxn){out[n].kind=(K);out[n].id=(ID);out[n].x=(X);out[n].y=(Y);out[n].w=(W);out[n].h=(H);out[n].label=(L);out[n].look=(LOOK);n++;}}while(0)
 const int VX=SC_VX,VY=SC_VY;
 /* HERE is always first: it restores the room's arrival prose and treats the
  * whole illustration as the selected place rather than inventing a prop. */
 SC_HOT(SC_H_HERE,sc_room,VX,VY,SC_VW,SC_VH,"HERE",sc_room_blurb(sc_room));
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 for(int i=0;i<pn;i++){
  int px,py;sc_person_pos(sc_room,i,&px,&py);
  if(px==0&&py==0)continue;
  int pw=sc_baked_lave_arrivals()?34:(sc_raster_room()?32:52),ph=sc_baked_lave_arrivals()?76:64;
  SC_HOT(SC_H_PERSON,i,VX+px,VY+py,pw,ph,people[i].name,people[i].line);
 }
 int ex[4],en=sc_exits(sc_room,ex,4);
 /* Side hatch layout on the illustrated MAIN (left of options list). */
 static const int door_xy[4][2]={{4,36},{286,36},{70,120},{180,120}};
 for(int i=0;i<en&&i<4;i++){
  int dx=VX+door_xy[i][0],dy=VY+door_xy[i][1];
  int dw=i<2?50:42,dh=i<2?60:26;
  if(sc_raster_room()){dx=VX+(i%2?317:2);dy=VY+45+(i/2)*36;dw=20;dh=en>2?32:65;}
  SC_HOT(SC_H_EXIT,ex[i],dx,dy,dw,dh,sc_room_short(ex[i]),"Door. GO + X walks through.");
 }
 /* Ship return — bottom-right of MAIN. */
 SC_HOT(SC_H_EXIT,SC_EXIT_SHIP,VX+(sc_raster_room()?317:SC_VW-72),VY+(sc_raster_room()?45:SC_VH-30),sc_raster_room()?20:68,sc_raster_room()?65:26,"YOUR SHIP","Board your ship now. TRI also boards.");
 if(sc_raster_room()){
  ScAnchor a=sc_recipes[sc_room].hero,b=sc_recipes[sc_room].service;
  static const char *landmarks[]={"BERTH WINDOW","COUNTER","BAR TOP","CARGO LIFT","STAR CHART","TREATMENT BAY","SCANNER GATE"};
  SC_HOT(SC_H_FEATURE,50+sc_room,VX+a.x,VY+a.y,a.w,a.h,landmarks[sc_room],sc_room_blurb(sc_room));
  if(sc_room==SC_R_CARGO)SC_HOT(SC_H_PROP,17,VX+b.x,VY+b.y,b.w,b.h,"MANIFEST","A loose manifest hangs beside the cargo rail. Inspect the sheet.");
  for(int slot=0;slot<2;slot++){int id=sc_module_id(sc_room,slot);SC_HOT(SC_H_PROP,60+slot,VX+(slot?268:36),VY+128,32,32,sc_module_names[id],sc_module_notes[id]);}
 }else if(sc_room==SC_R_ARRIVALS){
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
 if(sc_lave()){
  static const int services[]={100,101,102,-1,103,104,105};
  static const char *labels[]={"LOCAL JOBS","OUTFITTING","JUKEBOX",0,"BERTH SIX LOG","TREATMENT","LEGAL STATUS"};
  ScAnchor a=sc_recipes[sc_room].service;
  if(services[sc_room]>=0)SC_HOT(SC_H_FEATURE,services[sc_room],VX+a.x,VY+a.y,a.w,a.h,labels[sc_room],"Use this station service.");
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
  for(int p=0;p<pn;p++)if(people[p].act==SC_ACT_GIFT||people[p].act==SC_ACT_SHOP||people[p].act==SC_ACT_QUEST||people[p].act==SC_ACT_MANIFEST||people[p].act==SC_ACT_TAXI){int j=sc_find_hot(SC_H_PERSON,p);if(j>=0){i=j;break;}}
  if(i>=0)sc_hot=i;
 }
}
/* ---- drawing helpers (A++ cinematic: hero focus, 3 planes, warm staging) ---- */
static void sc_scene_sky(int x,int y,int w,int h,unsigned hi,unsigned lo){
 for(int row=0;row<h;row++){
  float t=row/(float)(h>1?h-1:1);
  rect(x,y+row,w,1,mix_rgb(hi,lo,t));
 }
}
/* Built plates: vertical seams + sparse rivets — “built, not grown”. */
static void sc_wall_plates(int x,int y,int w,int h,unsigned plate,unsigned seam){
 int cols=4,cw=w/cols;if(cw<28){cols=3;cw=w/cols;}
 for(int c=0;c<cols;c++){
  int px=x+c*cw;
  rect(px,y,cw-(c==cols-1?0:1),h,mix_rgb(plate,SC_CHAR,(c&1)*.08f));
  if(c)rect(px-1,y,1,h,seam);
  for(int r=0;r<3;r++){pixel(px+4,y+10+r*22,seam);pixel(px+cw-6,y+10+r*22,seam);}
 }
}
/* Three floor value masses — no checker texture that dies at 1×. */
static void sc_floor_planes(int x,int y,int w,int h,unsigned nearc,unsigned midc,unsigned farc){
 int band=h/5;if(band<10)band=10;
 rect(x,y+h-band*3,w,band,farc);
 rect(x,y+h-band*2,w,band,midc);
 rect(x,y+h-band,w,band,nearc);
 /* Floor seam lines for countable depth without a game-grid checker. */
 for(int i=1;i<4;i++)rect(x,y+h-band*i,w,1,mix_rgb(farc,SC_CREAM,.12f));
}
static void sc_warm_key(int cx,int cy,unsigned lamp){
 rect(cx-10,cy,20,3,mix_rgb(lamp,SC_CREAM,.4f));
 rect(cx-4,cy+3,8,2,lamp);
 pixel(cx,cy-2,SC_CREAM);
}
/* Sparse pinprick stars inside a berth/void aperture — no glitter masks. */
static void sc_void_stars(int x,int y,int w,int h,unsigned seed){
 unsigned s=seed^0xA11Cu;
 for(int i=0;i<10;i++){
  s=s*1664525u+1013904223u;
  int px=x+2+(int)((s>>8)%(unsigned)(w>4?w-4:1));
  s=s*1664525u+1013904223u;
  int py=y+2+(int)((s>>8)%(unsigned)(h>4?h-4:1));
  unsigned c=(i%5==0)?mix_rgb(SC_VOID,SC_CREAM,.45f):(i%3==0)?mix_rgb(SC_VOID,SC_LAV,.35f):mix_rgb(SC_VOID,SC_CREAM,.22f);
  pixel(px,py,c);
 }
}
/* Tiny freighter silhouette that drifts across a berth window (presentation only). */
static void sc_traffic_silhouette(int x0,int y0,int w,int h,unsigned hull,unsigned lamp){
 float phase=fmodf(game.time*.18f+(game.system&7)*.07f,1.f);
 int fx=x0+(int)(phase*(w-56));
 int fy=y0+h/2-6+(int)(sinf(game.time*.9f)*3);
 if(fx<x0+2)fx=x0+2;if(fx>x0+w-58)fx=x0+w-58;
 rect(fx,fy,48,8,hull);
 rect(fx+8,fy-5,22,6,mix_rgb(hull,SC_CREAM,.2f));
 rect(fx+40,fy+1,12,5,mix_rgb(hull,SC_VOID,.35f));
 pixel(fx+2,fy+2,lamp);
 if(((int)(game.time*6))&1)pixel(fx+46,fy+3,SC_CYAN);
 space_anim_draw(SPACE_ANIM_PLUME,fx-2,fy+4,((int)(game.time*10))&3,mix_rgb(SC_OCHRE,lamp,.4f));
}
/* Soft blink for practical lamps / board slots. */
static int sc_lamp_on(int slot,float rate){
 return ((int)(game.time*rate)+slot)&3;
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
/* Native 8×8 prop markers — authored at 1×, no resample (ART-KIT handoff). */
static void sc_prop8(int x,int y,int id,unsigned ink){
 unsigned fill=mix_rgb(SC_CHAR,ink,.35f),hi=mix_rgb(ink,SC_CREAM,.4f);
 rect(x,y,8,8,SC_VOID);
 if(id==0||id==1||id==3||id==17){ /* crate / stock / tip / manifest */
  rect(x+1,y+2,6,5,fill);rect(x+1,y+2,6,1,hi);pixel(x+4,y+4,hi);
 }else if(id==2||id==13){ /* bar / booth */
  rect(x+1,y+5,6,2,fill);rect(x+2,y+2,4,3,hi);
 }else if(id==4||id==8){ /* medkit / bay */
  rect(x+2,y+1,4,6,fill);rect(x+3,y+3,2,1,hi);rect(x+3,y+2,1,3,hi);
 }else if(id==7||id==23){ /* desk */
  rect(x+1,y+4,6,3,fill);rect(x+2,y+2,4,2,hi);
 }else if(id==5){ /* lift/hoist */
  rect(x+3,y+1,2,6,fill);rect(x+1,y+2,6,1,hi);
 }else if(id==6){ /* scanner gate */
  rect(x+1,y+1,2,6,fill);rect(x+5,y+1,2,6,fill);rect(x+3,y+3,2,2,hi);
 }else if(id==12){ /* diag panel */
  rect(x+1,y+1,6,6,fill);pixel(x+3,y+3,hi);pixel(x+5,y+4,ink);
 }else{ /* board / window / ledger / glass */
  rect(x+1,y+1,6,6,fill);rect(x+2,y+2,4,3,hi);pixel(x+6,y+1,ink);
 }
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
 sc_wall_plates(x,y,w,h-48,mix_rgb(st->wall,SC_CREAM,.08f),mix_rgb(SC_OLIVE,SC_RUST,.35f));
 sc_floor_planes(x,y,w,h,mix_rgb(SC_CREAM,st->wall,.35f),mix_rgb(st->wall,st->wall2,.4f),st->wall2);
 /* Overhead conduit strip — lamps blink gently */
 rect(x+8,y+2,w-16,4,mix_rgb(st->trim,SC_OLIVE,.4f));
 {
  int lx[]={24,w/2-8,w-40};int lw[]={12,16,12};
  for(int i=0;i<3;i++){
   unsigned lamp=sc_lamp_on(i,2.2f)<3?st->lamp:mix_rgb(st->lamp,SC_VOID,.45f);
   rect(x+lx[i],y+6,lw[i],2,lamp);
   if(sc_lamp_on(i,2.2f)==0)pixel(x+lx[i]+lw[i]/2,y+5,SC_CREAM);
  }
 }
 /* Distant coral haze through the aperture */
 rect(x+72,y+10,176,70,SC_VOID);
 rect(x+72,y+10,176,3,SC_CREAM);
 rect(x+72,y+10,3,70,mix_rgb(SC_CREAM,st->trim,.4f));
 rect(x+245,y+10,3,70,mix_rgb(SC_CREAM,st->trim,.4f));
 rect(x+72,y+77,176,3,mix_rgb(SC_CREAM,st->wall,.3f));
 sc_void_stars(x+78,y+16,164,58,game.bodies[0].seed^(unsigned)game.system*17u);
 /* Soft warm spill */
 for(int row=0;row<28;row++)rect(x+78,y+16+row,164,1,mix_rgb(SC_VOID,SC_OCHRE,row/40.f));
 /* Animated freighter crossing the berth — one ship, readable at 1× */
 sc_traffic_silhouette(x+78,y+18,164,52,st->wall,st->lamp);
 /* Soft berth lamp — cream pixel pulse, not a four-point glitter mask. */
 if(sc_lamp_on(0,2.5f)<2){pixel(x+118,y+28,mix_rgb(st->lamp,SC_CREAM,.35f));pixel(x+119,y+28,mix_rgb(st->lamp,SC_VOID,.4f));}
 sc_warm_key(x+160,y+84,st->lamp);
 /* Mid: traffic board — berth slots blink occupancy */
 rect(x+196,y+84,90,40,st->wall2);
 rect(x+196,y+84,90,2,SC_CREAM);
 rect(x+200,y+88,82,12,mix_rgb(st->accent,st->wall,.4f));
 text((x+204)/8,(y+90)/8,st->accent,"BERTHS");
 for(int i=0;i<6;i++){
  int on=sc_lamp_on(i+(game.system&3),1.6f)<2;
  unsigned slot=on?st->lamp:mix_rgb(SC_OLIVE,st->wall2,.5f);
  rect(x+202+i*13,y+104,10,6,slot);
  if(on&&(i&1)==0)pixel(x+206+i*13,y+106,SC_CREAM);
 }
 rect(x+200,y+116,40,4,sc_lamp_on(0,3.f)<2?st->lamp:mix_rgb(st->lamp,SC_VOID,.4f));
 /* Cargo-loader cycle under the board — ART DIRECTOR HUB production target. */
 {
  int bob=(int)(sinf(game.time*2.2f)*3);
  rect(x+248,y+100+bob,18,28,mix_rgb(st->wall,SC_OLIVE,.4f));
  rect(x+252,y+92+bob,10,10,st->trim);
  rect(x+250,y+128+bob,14,4,SC_RUST);
  if(sc_lamp_on(2,3.f)<2)pixel(x+256,y+96+bob,SC_CREAM);
 }
 /* Fore: rail + contact shadow */
 rect(x+72,y+118,160,4,st->trim);
 rect(x+72,y+122,160,3,SC_OLIVE);
 rect(x+80,y+126,144,2,mix_rgb(SC_VOID,SC_CHAR,.5f));
}
static void sc_illust_shop(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: ochre counter. Cream walls, two shelf masses only. */
 sc_scene_sky(x,y,w,h,mix_rgb(SC_CREAM,st->wall,.4f),st->wall2);
 sc_wall_plates(x,y,w,h-52,mix_rgb(st->wall,SC_CREAM,.1f),mix_rgb(SC_RUST,st->trim,.3f));
 sc_floor_planes(x,y,w,h,mix_rgb(SC_RUST,st->trim,.45f),mix_rgb(st->trim,st->wall,.35f),st->wall2);
 /* Back shelves — two blocks; one part lamp pulses as "exclusive" cue */
 rect(x+64,y+12,70,48,mix_rgb(st->wall,st->trim,.3f));
 rect(x+70,y+18,18,14,st->lamp);rect(x+94,y+18,18,14,st->accent);rect(x+118,y+18,10,14,SC_RUST);
 rect(x+186,y+12,70,48,mix_rgb(st->wall,st->trim,.3f));
 rect(x+192,y+18,18,14,SC_LAV);rect(x+216,y+18,18,14,st->trim);
 if(sc_lamp_on(1,3.5f)==0){rect(x+72,y+20,14,10,SC_CREAM);pixel(x+79,y+25,mix_rgb(st->lamp,SC_CREAM,.4f));}
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
 /* Clamp crate — scale cue + soft label blink */
 rect(x+24,y+h-44,36,28,mix_rgb(st->trim,SC_OCHRE,.4f));
 rect(x+24,y+h-44,36,4,SC_CREAM);
 if(sc_lamp_on(4,2.2f)<2)rect(x+28,y+h-36,12,4,st->accent);
 /* Hanging tool rack — left alcove personality */
 rect(x+12,y+40,20,48,mix_rgb(st->wall,SC_SLATE,.3f));
 rect(x+16,y+48,4,28,st->trim);rect(x+24,y+52,4,20,SC_RUST);
}
static void sc_illust_canteen(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: bar top. Four bottles, one booth, one juke. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,SC_OCHRE,.3f),st->wall2);
 sc_wall_plates(x,y,w,h-52,mix_rgb(st->wall,SC_OLIVE,.15f),mix_rgb(SC_RUST,st->trim,.35f));
 sc_floor_planes(x,y,w,h,mix_rgb(st->trim,st->wall,.4f),st->wall,st->wall2);
 /* Back bar wall + few bottles with glints */
 rect(x+64,y+12,192,44,mix_rgb(st->wall,st->trim,.2f));
 for(int i=0;i<4;i++){
  unsigned c=mix_rgb(SC_CREAM,st->trim,(i%3)*.2f);
  rect(x+88+i*36,y+18,10,28,c);
  if(sc_lamp_on(i,2.8f)<2)pixel(x+91+i*36,y+22,st->lamp);
 }
 /* Lave identity is foreground signage: keep it after the back-wall paint so
  * the native text remains readable in both normal and contrast palettes. */
 if(game.system==7){
  rect(x+94,y+2,132,4,mix_rgb(SC_RUST,SC_OCHRE,.35f));
  line(x+94,y+6,x+84,y+30,SC_OCHRE);line(x+226,y+6,x+236,y+30,SC_OCHRE);
  rect(x+104,y+8,112,16,SC_SLATE);rect(x+108,y+10,104,1,SC_CREAM);
  text((x+116)/8,(y+12)/8,SC_CREAM,"LAVE PUBLIC");
  circle(x+238,y+18,5,SC_OCHRE);pixel(x+238,y+18,SC_CREAM);
 }
 rect(x+64,y+56,192,3,st->accent);
 /* Hero bar */
 rect(x+64,y+100,200,30,mix_rgb(st->trim,st->wall,.35f));
 rect(x+64,y+94,200,8,st->lamp);
 rect(x+240,y+88,12,12,mix_rgb(SC_CREAM,st->lamp,.35f));
 /* Juke pulse + booth */
 rect(x+16,y+72,40,48,SC_SLATE);
 {
  unsigned juke=sc_lamp_on(2,4.f)<2?mix_rgb(st->lamp,SC_OCHRE,.35f):mix_rgb(st->lamp,st->wall,.4f);
  rect(x+20,y+76,32,18,juke);
  if(sc_lamp_on(2,4.f)==0)pixel(x+36,y+84,mix_rgb(SC_CREAM,st->lamp,.35f));
 }
 text((x+18)/8,(y+108)/8,SC_CREAM,"JUKE");
 rect(x+20,y+108,48,28,mix_rgb(st->trim,st->wall2,.35f));
 /* Steam wisps + cup on the bar — lived-in cue */
 if(!high_contrast)for(int i=0;i<3;i++){
  int sx=x+100+i*40+(int)(sinf(game.time*1.5f+i)*2);
  int sy=y+88-(int)fmodf(game.time*18+i*11,14);
  pixel(sx,sy,mix_rgb(SC_CREAM,st->wall,.4f));
 }
 rect(x+148,y+96,8,6,SC_CREAM);rect(x+150,y+92,4,4,mix_rgb(SC_CREAM,st->lamp,.3f));
 sc_warm_key(x+160,y+70,st->lamp);
 /* Lave showcase: a small authored window and lived-in foreground cues make
  * the public canteen read as a place, while staying native 1x and keeping
  * the existing person/door hotspot geometry untouched. */
 if(game.system==7){
  rect(x+118,y+28,122,46,SC_SLATE);rect(x+122,y+32,114,38,SC_VOID);
  for(int i=0;i<13;i++){
   int sx=x+126+((i*29+(int)(game.time*3))%104),sy=y+35+((i*17)%30);
   pixel(sx,sy,(i&3)==0?SC_CREAM:SC_LAV);
  }
  circle(x+202,y+51,14,SC_OCHRE);circle(x+207,y+47,13,SC_VOID);
  rect(x+124,y+66,110,2,SC_CYAN);
  text((x+126)/8,(y+34)/8,SC_CREAM,"LAVE // PUBLIC");
  /* Pendant lights and a booth silhouette create a clear foreground layer. */
  line(x+84,y+28,x+84,y+52,SC_SLATE);circle(x+84,y+56,5,st->lamp);
  line(x+270,y+24,x+270,y+50,SC_SLATE);circle(x+270,y+54,5,st->lamp);
  rect(x+274,y+82,44,42,mix_rgb(st->wall2,SC_SLATE,.35f));
  rect(x+280,y+76,32,8,st->trim);rect(x+286,y+88,4,26,SC_RUST);
  rect(x+276,y+118,42,5,SC_OCHRE);
  for(int i=0;i<4;i++)rect(x+92+i*34,y+132,20,3,(i&1)?SC_OCHRE:SC_RUST);
 }
}
static void sc_illust_cargo(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: tip crate centre. Three crates, one cool hoist. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->trim,.15f),st->wall2);
 sc_wall_plates(x,y,w,h-56,st->wall,mix_rgb(SC_OLIVE,st->trim,.3f));
 sc_floor_planes(x,y,w,h,mix_rgb(SC_OLIVE,st->wall,.4f),st->wall,st->wall2);
 /* Cool lift — cable bob + amber tip cue */
 rect(x+48,y+8,224,4,SC_LAV);
 {
  int bob=(int)(sinf(game.time*2.4f)*4);
  rect(x+w/2-3,y+8,6,40+bob,st->trim);
  rect(x+w/2-14,y+44+bob,28,8,st->wall);
  if(sc_lamp_on(0,2.f)<2)pixel(x+w/2,y+48+bob,st->lamp);
 }
 /* Hazard stripe on floor lane */
 for(int i=0;i<8;i++)rect(x+56+i*28,y+h-56,14,4,(i&1)?SC_OCHRE:st->wall2);
 /* Three crates — centre is tip hero */
 for(int i=0;i<3;i++){
  int cx=x+72+i*72;
  rect(cx,y+h-88,56,40,st->trim);
  rect(cx,y+h-88,56,5,SC_CREAM);
  rect(cx+16,y+h-72,22,8,st->wall2);
  if(i==1){
   unsigned tip=sc_lamp_on(3,2.5f)<3?st->lamp:mix_rgb(st->lamp,SC_VOID,.35f);
   rect(cx+6,y+h-100,44,14,tip);text((cx+10)/8,(y+h-98)/8,st->wall2,"TIP");
  }
 }
 /* Manifest secondary */
 rect(x+72,y+40,48,24,mix_rgb(st->wall,SC_CREAM,.25f));
 rect(x+76,y+44,40,6,st->accent);
 rect(x+48,y+h-52,w-96,4,st->lamp);
 /* Loader silhouette + rolling tip pallet glimmer */
 rect(x+20,y+h-100,28,40,mix_rgb(st->wall,SC_OLIVE,.35f));
 rect(x+24,y+h-108,20,10,st->trim);
 {
  int roll=(int)fmodf(game.time*22,40);
  rect(x+w-70+roll/4,y+h-70,22,12,mix_rgb(st->trim,SC_OCHRE,.35f));
  rect(x+w-68+roll/4,y+h-66,8,4,SC_CREAM);
 }
}
static void sc_illust_guild(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: tracked screen. Desk secondary. Olive/cream calm. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->wall,st->wall2,.45f),st->wall2);
 sc_wall_plates(x,y,w,h-52,mix_rgb(st->wall,SC_OLIVE,.2f),mix_rgb(SC_CREAM,st->trim,.25f));
 sc_floor_planes(x,y,w,h,mix_rgb(st->wall,SC_CREAM,.2f),st->wall,st->wall2);
 /* Evidence / survey pin board — left secondary */
 rect(x+16,y+16,56,64,mix_rgb(st->wall2,SC_OLIVE,.3f));
 rect(x+20,y+20,20,14,SC_CREAM);rect(x+44,y+20,20,14,st->lamp);
 rect(x+20,y+40,44,8,mix_rgb(st->accent,st->wall,.4f));
 rect(x+20,y+52,28,6,SC_RUST);
 /* Amber mission pin blink on evidence board */
 if(sc_lamp_on(5,2.8f)<2){rect(x+52,y+54,6,6,SC_AMBER);pixel(x+54,y+56,SC_CREAM);}
 /* Cream paper strips */
 rect(x+22,y+64,16,8,SC_CREAM);rect(x+42,y+66,18,6,mix_rgb(SC_CREAM,st->trim,.25f));
 /* Tracked screen — soft scanline flicker */
 rect(x+96,y+16,128,60,st->wall2);
 rect(x+102,y+22,116,36,mix_rgb(st->wall,st->accent,.35f));
 text((x+108)/8,(y+30)/8,st->accent,"TRACKED MISSION");
 {
  int scan=((int)(game.time*18))%28;
  rect(x+104,y+24+scan,112,1,mix_rgb(st->accent,SC_CREAM,.35f));
 }
 rect(x+108,y+58,100,8,sc_lamp_on(1,2.f)<2?st->lamp:mix_rgb(st->lamp,SC_VOID,.4f));
 rect(x+64,y+108,192,28,mix_rgb(st->wall,st->wall2,.4f));
 rect(x+64,y+102,192,8,mix_rgb(st->trim,st->wall,.4f));
 rect(x+72,y+114,40,12,SC_CREAM);
 sc_warm_key(x+160,y+80,st->lamp);
}
static void sc_illust_clinic(int x,int y,int w,int h){
 const ArtRoomStyle *st=sc_style();
 /* Hero: treatment bay. One cool lamp, one locker. */
 sc_scene_sky(x,y,w,h,mix_rgb(st->trim,st->wall,.3f),st->wall2);
 sc_wall_plates(x,y,w,h-52,mix_rgb(st->wall,SC_LAV,.12f),mix_rgb(st->trim,st->wall2,.35f));
 sc_floor_planes(x,y,w,h,mix_rgb(st->trim,st->wall2,.4f),st->wall,st->wall2);
 /* Cool key lamp pulse */
 {
  unsigned cool=sc_lamp_on(0,3.2f)<3?mix_rgb(st->lamp,st->accent,.45f):mix_rgb(st->lamp,SC_VOID,.35f);
  circle(x+160,y+18,8,cool);
  rect(x+154,y+26,12,3,st->trim);
 }
 /* Diag panel secondary */
 rect(x+72,y+40,48,36,st->wall2);
 rect(x+76,y+44,40,20,mix_rgb(st->lamp,SC_OLIVE,.35f));
 if(sc_lamp_on(2,2.5f)==0)rect(x+80,y+48,32,4,st->accent);
 /* Hero bay */
 rect(x+80,y+96,160,40,mix_rgb(st->trim,st->accent,.45f));
 rect(x+80,y+90,160,8,mix_rgb(st->lamp,st->wall,.4f));
 rect(x+90,y+108,40,16,st->accent);
 /* Privacy screen — clinic silhouette */
 rect(x+48,y+72,16,70,mix_rgb(st->wall,SC_LAV,.25f));
 rect(x+50,y+76,12,8,mix_rgb(st->accent,st->wall,.4f));
 /* Vitals blip on diag panel */
 if(sc_lamp_on(1,5.f)==0)pixel(x+96,y+52,SC_CREAM);
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
 sc_wall_plates(x,y,w,h-52,st->wall,mix_rgb(SC_DANGER,st->trim,.2f));
 sc_floor_planes(x,y,w,h,mix_rgb(st->wall,SC_CREAM,.15f),st->wall,st->wall2);
 /* Gate posts + animated scan sweep */
 rect(x+72,y+40,12,90,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+100,y+40,12,90,mix_rgb(st->trim,SC_RUST,.4f));
 rect(x+72,y+40,40,6,st->trim);
 {
  int sweep=((int)(game.time*28))%72;
  for(int i=0;i<4;i++){
   int yy=y+52+i*18;
   unsigned bar=(abs((i*18)-sweep)<10)?mix_rgb(st->accent,SC_CREAM,.4f):mix_rgb(st->accent,SC_VOID,.5f);
   rect(x+78,yy,28,2,bar);
  }
 }
 space_anim_draw(SPACE_ANIM_BEACON,x+92,y+70,((int)(game.time*5))&3,mix_rgb(SC_DANGER,SC_AMBER,.4f));
 /* Warrant glass */
 rect(x+140,y+16,120,52,st->wall2);
 rect(x+146,y+22,108,18,st->trim);
 text((x+156)/8,(y+26)/8,SC_CREAM,"WARRANT");
 rect(x+146,y+44,108,16,SC_VOID);
 text((x+160)/8,(y+46)/8,st->accent,"SCAN");
 /* Restricted placard */
 rect(x+268,y+48,40,28,mix_rgb(st->wall,SC_DANGER,.25f));
 rect(x+272,y+52,32,8,SC_DANGER);
 text((x+274)/8,(y+54)/8,SC_CREAM,"REST");
 /* Inspect desk */
 rect(x+120,y+112,140,28,st->wall);
 rect(x+120,y+108,140,6,mix_rgb(SC_CREAM,st->wall,.35f));
 /* Warrant stamp pulse */
 if(sc_lamp_on(3,1.8f)<2)rect(x+230,y+118,20,12,mix_rgb(SC_DANGER,SC_AMBER,.35f));
 sc_warm_key(x+190,y+96,st->lamp);
}
/* Focus follows the selector, independent of the latched reading panel. */
static void sc_focus_edge(int x,int y,int w,int h,unsigned ink){
 int l=x<SC_VX?SC_VX:x,t=y<SC_VY?SC_VY:y;
 int r=x+w-1>=SC_VX+SC_VW?SC_VX+SC_VW-1:x+w-1;
 int b=y+h-1>=SC_VY+SC_VH?SC_VY+SC_VH-1:y+h-1;
 if(r<l||b<t)return;
 line(l,t,r,t,ink);line(l,b,r,b,ink);line(l,t,l,b,ink);line(r,t,r,b,ink);
}
static void sc_focus_glow(const ScHot *h){
 float pulse=.5f+.5f*sinf(preview_time*3.f);
 unsigned core=high_contrast?SC_CREAM:mix_rgb(SC_CYAN,SC_CREAM,.35f+.4f*pulse);
 for(int spread=4;spread>=1;spread--){
  unsigned halo=mix_rgb(SC_CHAR,SC_CYAN,(5-spread)*(.07f+.035f*pulse));
  sc_focus_edge(h->x-spread,h->y-spread,h->w+spread*2,h->h+spread*2,halo);
 }
 sc_focus_edge(h->x,h->y,h->w,h->h,core);
 /* Brighter corner ticks stay readable even when a room contains cyan lights. */
 int l=h->x,t=h->y,r=l+h->w-1,b=t+h->h-1;
 if(l>=SC_VX&&r<SC_VX+SC_VW&&t>=SC_VY&&b<SC_VY+SC_VH){
  line(l,t,l+5,t,SC_CREAM);line(l,t,l,t+5,SC_CREAM);
  line(r-5,t,r,t,SC_CREAM);line(r,t,r,t+5,SC_CREAM);
  line(l,b,l+5,b,SC_CREAM);line(l,b-5,l,b,SC_CREAM);
  line(r-5,b,r,b,SC_CREAM);line(r,b-5,r,b,SC_CREAM);
 }
}
static void sc_draw_main_scene(void){
 const int VX=SC_VX,VY=SC_VY,VW=SC_VW,VH=SC_VH;
 int second_shift=sc_room==SC_R_CANTEEN&&bar_preview_at();
 /* Illustrated room only — options list is the selector chrome. */
 rect(VX,VY,VW,VH,SC_VOID);
 rect(VX,VY,VW,1,SC_OCHRE);
 rect(VX,VY+VH-1,VW,1,SC_SLATE);
 if(sc_raster_room())sc_raster_draw(VX,VY,sc_room);
 else if(sc_room==SC_R_ARRIVALS){
  if(station_authored_arrivals_at())station_arrivals_authored_draw(VX,VY,VW,VH);
  else sc_illust_arrivals(VX,VY,VW,VH);
 }
 else if(sc_room==SC_R_SHOP)sc_illust_shop(VX,VY,VW,VH);
 else if(sc_room==SC_R_CANTEEN){if(second_shift)sc_illust_second_shift(VX,VY,VW,VH);else sc_illust_canteen(VX,VY,VW,VH);}
 else if(sc_room==SC_R_CARGO)sc_illust_cargo(VX,VY,VW,VH);
 else if(sc_room==SC_R_GUILD)sc_illust_guild(VX,VY,VW,VH);
 else if(sc_room==SC_R_CLINIC)sc_illust_clinic(VX,VY,VW,VH);
 else sc_illust_customs(VX,VY,VW,VH);
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 /* Hatches — silhouette only; names live in the right options list. */
 {
  int exit_ord=0;
  for(int i=0;i<hn;i++)if(!second_shift&&!sc_raster_room()&&hot[i].kind==SC_H_EXIT&&hot[i].id!=SC_EXIT_SHIP){
   int hero=exit_ord==0;
   unsigned frame=(hero||sc_hot==i)?mix_rgb(SC_CREAM,SC_OCHRE,.35f):mix_rgb(SC_SLATE,SC_CREAM,.2f);
   unsigned aperture=(hero||sc_hot==i)?mix_rgb(SC_VOID,SC_OCHRE,.2f):SC_VOID;
   sc_hatch(hot[i].x,hot[i].y,hot[i].w,hot[i].h,frame,aperture,hero||sc_hot==i);
   exit_ord++;
  }
 }
 for(int i=0;i<pn&&!second_shift;i++){
  int px,py;sc_person_pos(sc_room,i,&px,&py);
  if(px==0&&py==0)continue;
  int focus=sc_hot<hn&&hot[sc_hot].kind==SC_H_PERSON&&hot[sc_hot].id==i;
  /* Lave's hero plate already contains Venn at this authored anchor.  Never
   * stamp the legacy atlas card over the baked figure; only the live focus
   * rectangle is drawn below. */
  int baked_venn=sc_room==SC_R_ARRIVALS&&px==145&&py==77;
  if(!baked_venn){
   if(sc_raster_room())sc_raster_crew(VX+px,VY+py,i,focus);
   else sc_draw_person_sprite(VX+px,VY+py,&people[i],focus);
  }
 }
 if(sc_hot>=0&&sc_hot<hn)sc_focus_glow(&hot[sc_hot]);
 for(int i=0;i<hn;i++)if(!sc_raster_room()&&hot[i].kind==SC_H_EXIT&&hot[i].id==SC_EXIT_SHIP){
  unsigned c=sc_hot==i?SC_AMBER:SC_OCHRE;
  rect(hot[i].x,hot[i].y,hot[i].w,hot[i].h,mix_rgb(SC_CHAR,SC_OCHRE,.2f));
  rect(hot[i].x,hot[i].y,hot[i].w,1,c);
  rect(hot[i].x,hot[i].y+hot[i].h-1,hot[i].w,1,c);
  text((hot[i].x+8)/8,(hot[i].y+8)/8,c,"SHIP");
 }
}
/* Room identity and live balance; no verb buttons or PACK hold cue. */
static void sc_draw_header(void){
 StationShellStyle shell=station_shell_style();
 StationIdentity identity=sc_identity();
 rect(0,0,W,SC_VY-2,SC_CHAR);
 rect(0,SC_VY-3,W,1,shell.rule);
 text(1,1,shell.warm,"%.18s",sc_room_title(sc_room));
 char station_name[32];snprintf(station_name,sizeof(station_name),"%s STATION",game.systems[game.system].name);
 text(24,1,shell.cyan,"%.20s",station_name);
 char balance[32];snprintf(balance,sizeof(balance),"%d.%d U",game.credits/10,game.credits%10);
 int balance_x=W-8-(int)strlen(balance)*8;
 rect(balance_x-5,4,W-balance_x+1,16,SC_VOID);
 text_px(balance_x,8,shell.warm,"%s",balance);
 (void)identity;

}
/* Right-side people / options list — the only selector. */
static int sc_talk_choices(const ScNpc *p,const char **out,int maxn);
static void sc_draw_options(void){
 StationShellStyle shell=station_shell_style();
 rect(SC_LX,SC_LY,SC_LW,SC_LH,mix_rgb(SC_CHAR,SC_VOID,.35f));
 station_shell_frame(SC_LX,SC_LY,SC_LW,SC_LH,0);
 if(sc_menu==SC_MENU_SHOP){
  text_px(SC_LX+6,SC_LY+4,shell.cyan,"DECK STOCK");
  int list[8],n=sc_exclusive_catalog(list,8);
  for(int i=0;i<n;i++){int y=SC_LY+16+i*12,chosen=i==(sc_shop_row%n+n)%n;
   if(chosen)rect(SC_LX+2,y-1,SC_LW-4,11,SC_AMBER);
   text_px(SC_LX+6,y,chosen?SC_VOID:SC_CREAM,"%.14s",equipment_list_names[list[i]]);
  }return;
 }
 if(sc_menu==SC_MENU_TALK){
  text_px(SC_LX+6,SC_LY+4,shell.cyan,"REPLIES");
  ScNpc people[3];int n=sc_fill_npcs(sc_room,people,3);if(sc_talk_who<0||sc_talk_who>=n)return;
  const char *choices[4];int count=sc_talk_choices(&people[sc_talk_who],choices,4),y=40;
  for(int i=0;i<count;i++){
   int lines=0;const char *p=choices[i];do{lines++;p=sc_read_advance(p,1,14);}while(*p&&lines<4);
   if(i==sc_talk_row)rect(SC_LX+2,y-1,SC_LW-4,lines*8+2,SC_AMBER);
   text_wrap(45,y/8,14,4,i==sc_talk_row?SC_VOID:SC_CREAM,choices[i],0);y+=lines*8+8;
  }return;
 }
 text_px(SC_LX+6,SC_LY+4,shell.cyan,"EXPLORE");
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 if(sc_hot<0)sc_hot=0;if(hn>0&&sc_hot>=hn)sc_hot=hn-1;
 int rows=6,row_h=22;
 int first=0;if(hn>rows){first=sc_hot-(rows/2);if(first<0)first=0;if(first>hn-rows)first=hn-rows;}
 for(int j=0;j<rows&&first+j<hn;j++){
  int i=first+j,y=SC_LY+25+j*row_h;
  int chosen=i==sc_hot;unsigned ink=chosen?shell.ink:shell.cream;
  rect(SC_LX+2,y-3,SC_LW-4,20,mix_rgb(SC_CHAR,SC_VOID,.18f));
  rect(SC_LX+2,y-3,SC_LW-4,1,chosen?shell.cream:SC_SLATE);
  rect(SC_LX+2,y+16,SC_LW-4,1,chosen?shell.cream:SC_SLATE);
  rect(SC_LX+2,y-3,1,20,chosen?shell.cream:SC_SLATE);
  rect(SC_LX+SC_LW-3,y-3,1,20,chosen?shell.cream:SC_SLATE);
  if(chosen){
   rect(SC_LX+3,y-2,SC_LW-6,18,shell.warm);
   text_px(SC_LX+5,y,ink,">");
  }
  char line[20];
  if(hot[i].kind==SC_H_HERE)snprintf(line,sizeof(line),"HERE");
  else if(hot[i].kind==SC_H_PERSON)snprintf(line,sizeof(line),"%.14s",hot[i].label);
  else if(hot[i].kind==SC_H_EXIT&&hot[i].id==SC_EXIT_SHIP)snprintf(line,sizeof(line),"YOUR SHIP");
  else if(hot[i].kind==SC_H_EXIT)snprintf(line,sizeof(line),"-> %.11s",hot[i].label);
  else snprintf(line,sizeof(line),"%.14s",hot[i].label);
  text_px(SC_LX+(chosen?15:6),y,ink,"%s",line);
 }
}
/* Verb chrome retired — options list owns LOOK/SPEAK/GO/TAKE payoffs. */
static int sc_talk_choices(const ScNpc *p,const char **out,int maxn){
 if(bar_preview_action(p->act)&&maxn>=3){
  out[0]=p->act==SC_ACT_BAR_DAX?"LOW 2-6 / 7 draw - FREE":"Hear them out";
  out[1]=p->act==SC_ACT_BAR_DAX?"HIGH 8-12 / 7 draw - FREE":"Target REORTE I";
  out[2]="Back";return 3;
 }
 int n=0;
 #define SC_CH(S) do{if(n<maxn)out[n++]=(S);}while(0)
 SC_CH("Hear them out");
 if(p->act==SC_ACT_SHOP)SC_CH("Browse exclusive stock");
 else if(p->act==SC_ACT_GIFT)SC_CH(p->offer);
 else if(p->act==SC_ACT_MANIFEST)SC_CH(p->offer);
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
  if(game.legal>0){if(police_pay_desk(&game))return;snprintf(note,sizeof(note),"Outstanding warrant %d/5. Need %.1f U at the desk.",wanted_level(&game),police_fine(&game)*.1f);message(&game,note);}
  else if(cargo_contraband(&game)>0){snprintf(note,sizeof(note),"Hold shows %d t restricted. Space Law will scan if they catch you.",cargo_contraband(&game));message(&game,note);}
  else message(&game,"Hold looks clean. Restricted goods only matter when Law scans.");
 }else message(&game,p->offer?p->offer:p->line);
}
static void sc_draw_text_box(void){
 rect(0,190,W,82,SC_CHAR);station_shell_rule(0,190,W,SC_CYAN);
 if(sc_menu==SC_MENU_SHOP){
  int list[8],n=sc_exclusive_catalog(list,8);text(1,24,SC_AMBER,"CHANDLER STOCK");
  if(n){int k=list[(sc_shop_row%n+n)%n];text(1,25,SC_CREAM,"%.30s  %.1f U",equipment_list_names[k],equipment_net_cost(k)*.1f);}
  text_wrap(1,27,58,4,SC_CREAM,game.message_time>0?game.message:"Choose a module to inspect its price. Replacement purchases need confirmation.",0);
 }else{
  int pages=sc_read_pages();if(sc_read_page>=pages)sc_read_page=pages-1;if(sc_read_page<0)sc_read_page=0;
  const char *p=sc_read_text;for(int i=0;i<sc_read_page;i++)p=sc_read_advance(p,6,58);
  text(1,24,SC_AMBER,"%.43s",sc_read_title);if(pages>1)text(52,24,SC_LAV,"%d/%d",sc_read_page+1,pages);
  text_wrap(1,25,58,6,SC_CREAM,p,0);
 }
 rect(0,256,W,16,SC_VOID);rect(0,256,W,1,SC_SLATE);
 button_icon(8,259,'U',SC_CREAM);button_icon(20,259,'D',SC_CREAM);text_px(36,260,SC_LAV,sc_menu==SC_MENU_TALK?"REPLY":"SELECT");
 button_icon(100,259,'X',SC_CREAM);text_px(116,260,SC_LAV,sc_menu==SC_MENU_SHOP?(equip_confirm_item>=0?"CONFIRM":"BUY"):"CHOOSE");
 if(sc_menu!=SC_MENU_SHOP){button_icon(180,259,'L',SC_CREAM);button_icon(192,259,'R',SC_CREAM);text_px(208,260,SC_LAV,"READ");}
 button_icon(256,259,'O',SC_CREAM);text_px(272,260,SC_LAV,"BACK");
 button_icon(320,259,'T',SC_CREAM);text_px(336,260,SC_LAV,sc_menu?"BACK":"SHIP");
 if(sc_lave()){button_icon(396,259,'E',SC_CREAM);text_px(416,260,SC_LAV,"LOG");}
}
static void sc_draw_ui(void){
 sc_build_map();preview_reset();*sc_progress()|=1u<<(8+sc_room);
 rect(0,0,W,H,mix_rgb(SC_VOID,SC_OCHRE,.05f));
 sc_draw_header();
 sc_draw_main_scene();
 sc_draw_options();
 sc_draw_text_box();
}
static void sc_board_ship(void){
 page=HOME;sc_menu=0;message(&game,"Back aboard. Command deck ready.");game.cue=SFX_UI;
}
static void sc_do_npc_choice(ScNpc *p,int choice){
 const char *ch[4]; int cn=sc_talk_choices(p,ch,4);
 if(choice<0||choice>=cn)return;
 if(choice==cn-1){sc_menu=SC_MENU_NONE;message(&game,"You nod and step back.");game.cue=SFX_UI;return;}
 if(bar_preview_action(p->act)){bar_preview_choose(p->act,choice);sc_menu=SC_MENU_NONE;return;}
 speak(&game,lave_npc_action(p->act)?VOICE_CONTACT:p->role==LAW?VOICE_LAW:p->role==EXPLORERS?VOICE_KEI:!strcmp(p->name,"VENN")?VOICE_VENN:VOICE_DOCK,p->line);
 if(lave_npc_action(p->act)&&choice==1){sc_menu=SC_MENU_NONE;lave_action(p->act-40);return;}
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
  if(bit==0){int clamp_slot=fit_empty_slot(&game,FIT_HOLD);if(clamp_slot>=0&&fit_find(&game,23)<0){game.fit[clamp_slot]=23;fit_rebuild(&game);message(&game,"Free cargo clamp fitted (+8t).");}else {game.credits+=200;message(&game,"Clamp spare sold for 20 U.");}}
  else {game.energy=100;message(&game,"Medkit used — energy restored.");}
  game.cue=SFX_UI;
 }else if(p->act==SC_ACT_MANIFEST){
  sc_manifest_prepare();
  if(sc_manifest_state()==1){
   sc_manifest_set(2);game.credits+=sc_manifest_reward();
   {char note[80];snprintf(note,sizeof(note),"Manifest filed. Tip paid %.1f U.",sc_manifest_reward()*.1f);message(&game,note);}
   game.cue=SFX_SELECT;
  }else if(sc_manifest_state()==0){
   message(&game,"The loose sheet is somewhere in the cargo bay. Inspect MANIFEST first.");game.cue=SFX_UI;
  }else {message(&game,"The loader has already filed that manifest.");game.cue=SFX_UI;}
 }else if(p->act==SC_ACT_QUEST){
  if(*sc_progress()&(1u<<16)){message(&game,"Survey tip already paid at this hub.");return;}
  int surveyed=0;for(int b=1;b<BODY_COUNT;b++)surveyed|=(game.surface_progress[game.system][b]&0x0fff00u)!=0;
  if(!surveyed){message(&game,"Scan surface life or minerals in this system, then bring back the record.");return;}
  *sc_progress()|=1u<<16;game.credits+=p->quest_pay;
  {char note[64];snprintf(note,sizeof(note),"Side tip paid %.1f U.",p->quest_pay*.1f);message(&game,note);}
  game.cue=SFX_SELECT;
 }else if(p->act==SC_ACT_TAXI){
  if(!(game.upgrades&512)){message(&game,"Need a passenger cabin fitted.");return;}
  if(game.passenger_dest>=0){message(&game,"Cabin already holds a passenger.");return;}
  if(cargo_used(&game)>=cargo_capacity(&game)){message(&game,"Need 1t free for a passenger.");return;}
  int dest=(game.system*17+sc_room*3+11)&255;if(dest==game.system)dest=(dest+5)&255;
  game.passenger_dest=dest;game.passenger_kind=1;game.passenger_pay=p->taxi_pay;
  {char note[80];snprintf(note,sizeof(note),"Passenger booked for %s.",game.systems[dest].name);message(&game,note);}
  speak(&game,VOICE_CONTACT,"Thanks. I'll talk your ear off.");game.cue=SFX_UI;
 }
}
static void sc_apply_action(void){
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  if(!ln){message(&game,"Chandler is empty.");return;}
  if(sc_shop_row<0)sc_shop_row=0;if(sc_shop_row>=ln)sc_shop_row=ln-1;
  equipment_buy_action(list[sc_shop_row],1);return;
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
 /* Natural action from the selected options-list entry — no verb row. */
 if(h->kind==SC_H_HERE){message(&game,sc_room_blurb(sc_room));game.cue=SFX_UI;return;}
 if(h->kind==SC_H_EXIT){
  if(h->id==SC_EXIT_SHIP){sc_board_ship();return;}
  sc_room=h->id;sc_hot=0;message(&game,sc_room_blurb(sc_room));game.cue=SFX_SELECT;return;
 }
 if(h->kind==SC_H_PERSON){
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  if(h->id<0||h->id>=pn){message(&game,"They stepped away.");return;}
  if(people[h->id].act==SC_ACT_BAR_DAX)bar_preview_token=bar_preview_practice.round_id+1;
  sc_menu=SC_MENU_TALK;sc_talk_who=h->id;sc_talk_row=0;game.cue=SFX_UI;return;
 }
 /* Props that own a deal open talk with that person; otherwise LOOK. */
 if(h->kind==SC_H_PROP||h->kind==SC_H_FEATURE){
  if(sc_lave()&&h->id>=100&&h->kind==SC_H_FEATURE){
   if(h->id==100){change_page(MISSIONS);return;}
   if(h->id==101){change_page(EQUIP);return;}
   if(h->id==102){audio_station_juke=!audio_station_juke;message(&game,audio_station_juke?"Canteen jukebox on. Music volume applies.":"Canteen jukebox off.");return;}
   if(h->id==103){tracked_mission=TRACK_LAVE;change_page(CAMPAIGN);return;}
   if(h->id==104){if(!(*sc_progress()&(1u<<17))){*sc_progress()|=1u<<17;game.energy=100;game.hazard=0;message(&game,"Dr Sen: complimentary treatment. Shields recharged and exposure cleared.");}else message(&game,"You are already cleared for duty. Ship damage needs the repair dock.");return;}
   if(h->id==105){change_page(STATUS);return;}
  }
  if(sc_room==SC_R_CARGO&&!strcmp(h->label,"MANIFEST")){
   if(sc_lave()){if(sc_lave_stage()==1){sc_lave_set(2);sc_manifest_set(1);message(&game,"Manifest recovered: medical crate, canteen shift. Ask Niko in the Canteen.");}else message(&game,sc_lave_objective());game.cue=SFX_SCAN;return;}
   sc_manifest_prepare();
   if(sc_manifest_state()==0){sc_manifest_set(1);message(&game,"You find the missing manifest under the rail. Take it to the LOADER.");}
   else if(sc_manifest_state()==1)message(&game,"The missing manifest is in your hand. The LOADER can file it.");
   else message(&game,"The cargo manifest is filed and the berth is moving again.");
   game.cue=SFX_UI;return;
  }
  int owner=sc_find_hot(SC_H_PERSON,-1);
  if(owner>=0&&(!strcmp(h->label,"TIP CRATE")||!strcmp(h->label,"STOCK CRATE")||!strcmp(h->label,"MEDKIT LOCKER")||!strcmp(h->label,"COUNTER")||!strcmp(h->label,"BAR TOP")||!strcmp(h->label,"GUILD DESK"))){
   ScHot hot2[24]; int hn2=sc_hotspots(hot2,24);
   if(owner<hn2&&hot2[owner].kind==SC_H_PERSON){
    sc_hot=owner;sc_menu=SC_MENU_TALK;sc_talk_who=hot2[owner].id;sc_talk_row=1;
    message(&game,"Ask them — their deal is open.");game.cue=SFX_UI;return;
   }
  }
  message(&game,h->look?h->look:sc_room_blurb(sc_room));game.cue=SFX_UI;return;
 }
 message(&game,h->look?h->look:sc_room_blurb(sc_room));game.cue=SFX_UI;
}
static void sc_apply(void){
 int oldroom=sc_room,oldmenu=sc_menu,oldstage=sc_lave_stage(),oldpage=page;ScNpc person={0};ScHot picked={0};
 if(oldmenu==SC_MENU_TALK){ScNpc p[3];int n=sc_fill_npcs(sc_room,p,3);if(sc_talk_who>=0&&sc_talk_who<n)person=p[sc_talk_who];}
 else if(!oldmenu){ScHot h[24];int n=sc_hotspots(h,24);if(sc_hot>=0&&sc_hot<n)picked=h[sc_hot];}
 int choice=sc_talk_row;sc_apply_action();
 if(page!=oldpage)return; /* Returning from a service preserves the last paragraph. */
 if(oldroom!=sc_room){sc_read_room();return;}
 if(sc_menu==SC_MENU_TALK){ScNpc p[3];int n=sc_fill_npcs(sc_room,p,3);if(sc_talk_who>=0&&sc_talk_who<n)sc_read_contact(&p[sc_talk_who]);return;}
 if(oldmenu==SC_MENU_TALK&&person.name){
  if(choice==0&&!bar_preview_action(person.act))sc_read_set(person.name,sc_contact_detail(&person));
  else if(lave_npc_action(person.act)&&choice==1)sc_read_pair(person.name,lave_response(person.act-40,oldstage),sc_lave_objective());
  else sc_read_set(person.name,game.message);
 }else if(picked.label){
  if(picked.kind==SC_H_HERE)sc_read_room();
  else if(picked.kind==SC_H_FEATURE&&picked.id>=50&&picked.id<57)sc_read_set(picked.label,sc_landmark_detail(sc_room));
  else if(picked.kind==SC_H_PROP&&picked.id==17)sc_read_pair(picked.label,"The manifest records the shipment rather than the crate itself: a small, easily misplaced piece of paperwork standing between the cargo and its next destination. You check the entries carefully. A change of handler, a damaged seal or a missing clearance matters here, even when the container has never left the station.",game.message);
  else if(picked.kind==SC_H_PROP&&picked.id>=60&&picked.id<62)sc_read_set(picked.label,picked.look);
  else sc_read_set(picked.label,game.message);
 }else sc_read_set("STATION",game.message);
}
static int sc_input(unsigned pressed){
 sc_build_map();
 if(sc_menu!=SC_MENU_SHOP&&(pressed&PSP_CTRL_LEFT)){
  if(sc_read_page>0)sc_read_page--;game.cue=SFX_SELECT;return 1;
 }
 if(sc_menu!=SC_MENU_SHOP&&(pressed&PSP_CTRL_RIGHT)){
  int pages=sc_read_pages();if(sc_read_page+1<pages)sc_read_page++;game.cue=SFX_SELECT;return 1;
 }
 if((pressed&PSP_CTRL_SELECT)&&sc_lave()){tracked_mission=TRACK_LAVE;change_page(CAMPAIGN);return 1;}
 if(pressed&PSP_CTRL_TRIANGLE && sc_menu==SC_MENU_NONE){sc_board_ship();return 1;}
 if(sc_menu==SC_MENU_SHOP){
  int list[8],n=sc_exclusive_catalog(list,8);if(n<1)n=1;
  sc_shop_row=(sc_shop_row%n+n)%n;
  if(pressed&PSP_CTRL_UP){sc_shop_row=(sc_shop_row+n-1)%n;game.cue=SFX_SELECT;}
  if(pressed&PSP_CTRL_DOWN){sc_shop_row=(sc_shop_row+1)%n;game.cue=SFX_SELECT;}
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
 if(pressed&PSP_CTRL_UP){sc_hot=(sc_hot+hn-1)%hn;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_DOWN){sc_hot=(sc_hot+1)%hn;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_CROSS)sc_apply();
 return 0;
}
