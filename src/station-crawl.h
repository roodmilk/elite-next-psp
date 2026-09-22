/* MacVenture-style station: lush illustrated rooms + verb/hotspot point-and-click.
 * Inspired by Shadowgate / Deja Vu / Uninvited — not a grid maze.
 * Small interior graph. Soft-FB pixel scenes. Talk/shop/gift/taxi preserved. */
enum {
 SC_R_ARRIVALS=0, SC_R_SHOP, SC_R_CANTEEN, SC_R_CARGO, SC_R_GUILD, SC_R_CLINIC, SC_R_CUSTOMS, SC_R_COUNT
};
enum { SC_V_LOOK=0, SC_V_SPEAK, SC_V_GO, SC_V_TAKE, SC_V_COUNT };
enum { SC_H_NONE=0, SC_H_PERSON, SC_H_EXIT, SC_H_PROP, SC_H_FEATURE };
enum { SC_MENU_NONE=0, SC_MENU_SHOP, SC_MENU_TALK };
/* Compatibility aliases so older open-walk / tests that touch sc_x still compile. */
enum { SC_W=1, SC_H=1, SC_N=0, SC_E=1, SC_S=2, SC_WDIR=3 };
enum { SC_EXIT_SHIP=-1 }; /* special GO target: return to command deck / ship */
static int sc_room=SC_R_ARRIVALS, sc_verb=SC_V_LOOK, sc_hot=0, sc_menu=0, sc_shop_row=0, sc_talk_row=0, sc_talk_who=0, sc_built_for=-1;
static int sc_x=0, sc_y=0, sc_face=SC_S; /* legacy stubs */
static unsigned char sc_map[1][1], sc_door_n[1][1], sc_door_e[1][1];
typedef struct { const char *name; int role; int act; int shop_item; int gift_bit; int quest_pay; int taxi_pay; const char *line; const char *offer; } ScNpc;
enum { SC_ACT_TALK=0, SC_ACT_SHOP, SC_ACT_GIFT, SC_ACT_QUEST, SC_ACT_TAXI, SC_ACT_BOARD };
typedef struct { int kind; int id; int x,y,w,h; const char *label; const char *look; } ScHot;
/* Stand positions inside the MAIN window (relative to VX,VY). */
static void sc_person_pos(int room,int i,int *ox,int *oy){
 /* Spread people across the scene so they feel placed, not lined up. */
 static const int pos[SC_R_COUNT][3][2]={
  {{48,92},{210,88},{140,100}},   /* arrivals: rail / board / mid */
  {{100,96},{230,90},{160,100}},  /* shop: counter L / shelves R */
  {{80,98},{240,96},{150,90}},    /* canteen: bar left / booth right */
  {{60,100},{200,92},{140,96}},   /* cargo */
  {{90,98},{220,94},{150,100}},   /* guild */
  {{110,96},{230,92},{160,100}},  /* clinic */
  {{100,98},{220,94},{150,96}}    /* customs */
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
  "Berth lights wash the arrivals deck. Traffic boards flicker. Someone always waits by the rail.",
  "Crates, clamps and off-ledger stock. The chandler keeps a ledger the main board never sees.",
  "Warm air, cold drinks. Rumours travel faster than freighters in here.",
  "Loaders shout over pallet buzz. A tip job usually sits on the nearest crate.",
  "Guild teal and quiet screens. Kei keeps a chair warm when she is in-system.",
  "Sterile light, soft beeps. One free medkit, if you have not claimed it.",
  "Law glass and warrant screens. Restricted goods still raise heat."
 };
 return r>=0&&r<SC_R_COUNT?n[r]:"";
}
static int sc_fill_npcs(int room,ScNpc *out,int maxn){
 int n=0; unsigned h=(unsigned)(game.system*131u+room*41u);
 #define SC_PUSH(N,R,A,SI,GB,QP,TP,L,O) do{if(n<maxn){out[n].name=(N);out[n].role=(R);out[n].act=(A);out[n].shop_item=(SI);out[n].gift_bit=(GB);out[n].quest_pay=(QP);out[n].taxi_pay=(TP);out[n].line=(L);out[n].offer=(O);n++;}}while(0)
 if(room==SC_R_ARRIVALS){
  SC_PUSH("VENN",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Berth six stays warm if you keep coming home.","Ask about traffic");
  SC_PUSH("DOCKHAND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Watch the freighter lane. Pads are tight.","Ask about berths");
 }else if(room==SC_R_SHOP){
  SC_PUSH("CHANDLER",TRADERS,SC_ACT_SHOP,-1,-1,0,0,"I stock what the main board won't list.","Browse exclusive stock");
  if((h&3)==0)SC_PUSH("MECHANIC",TRADERS,SC_ACT_GIFT,-1,0,0,0,"Take a spare clamp. Once. Don't ask twice.","Accept free clamp");
 }else if(room==SC_R_CANTEEN){
  SC_PUSH("BARTEND",TRADERS,SC_ACT_TALK,-1,-1,0,0,"Meridian tips well. Truth tips better.","Ask for rumours");
  SC_PUSH("TRAVELER",EXPLORERS,SC_ACT_TAXI,-1,-1,0,1800,"Need a lift to another hub. One tonne seat.","Offer taxi berth");
 }else if(room==SC_R_CARGO){
  SC_PUSH("LOADER",TRADERS,SC_ACT_QUEST,-1,-1,400,0,"Haul a crate mark to the board for me.","Take 40 U tip job");
 }else if(room==SC_R_CLINIC){
  SC_PUSH("MEDIC",LAW,SC_ACT_GIFT,-1,1,0,0,"Medkit for the next scrape. One issue only.","Take free medkit");
 }else if(room==SC_R_GUILD){
  SC_PUSH("KEI",EXPLORERS,SC_ACT_TALK,-1,-1,0,0,"When you are ready, open Tracked Mission.","Ask about Ryn");
  SC_PUSH("SURVEYOR",EXPLORERS,SC_ACT_QUEST,-1,-1,600,0,"Fresh surface sample pays. Log it honest.","Accept survey tip");
 }else if(room==SC_R_CUSTOMS){
  SC_PUSH("CUSTOMS",LAW,SC_ACT_TALK,-1,-1,0,0,"Restricted goods still raise warrants.","Ask about law");
 }
 #undef SC_PUSH
 return n;
}
/* Exit lists per room — small station graph. */
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
/* ---- hotspot list: people in place, doors, props, ship return ---- */
static int sc_hotspots(ScHot *out,int maxn){
 int n=0;
 #define SC_HOT(K,ID,X,Y,W,H,L,LOOK) do{if(n<maxn){out[n].kind=(K);out[n].id=(ID);out[n].x=(X);out[n].y=(Y);out[n].w=(W);out[n].h=(H);out[n].label=(L);out[n].look=(LOOK);n++;}}while(0)
 const int VX=72,VY=24;
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 for(int i=0;i<pn;i++){
  int px,py;sc_person_pos(sc_room,i,&px,&py);
  SC_HOT(SC_H_PERSON,i,VX+px,VY+py,52,64,people[i].name,people[i].line);
 }
 /* Room doors painted along the top of the scene */
 int ex[4],en=sc_exits(sc_room,ex,4);
 for(int i=0;i<en&&i<4;i++){
  int dx=VX+8+i*78,dy=VY+4;
  SC_HOT(SC_H_EXIT,ex[i],dx,dy,72,22,sc_room_short(ex[i]),"Station door. GO + X walks through, or pick it in EXITS.");
 }
 /* Always-available return to ship — every room, amber plaque + EXITS list + TRI. */
 SC_HOT(SC_H_EXIT,SC_EXIT_SHIP,VX+248,VY+132,68,24,"YOUR SHIP","Board your ship. Also TRI, or EXITS > SHIP.");
 /* Extra clickables per room — MacVenture density (props + scenery you can LOOK at). */
 if(sc_room==SC_R_ARRIVALS){
  SC_HOT(SC_H_FEATURE,0,VX+210,VY+78,86,50,"TRAFFIC BOARD","Arrivals, delays, and berth heat. Nothing classified.");
  SC_HOT(SC_H_PROP,0,VX+40,VY+120,200,16,"RAIL","Cold alloy rail over the freighter lane. LOOK only.");
  SC_HOT(SC_H_FEATURE,1,VX+20,VY+10,280,50,"VIEWPORT","Stars and freighter traffic through the outer glass.");
  SC_HOT(SC_H_PROP,9,VX+40,VY+80,36,48,"SIDE DOOR","Leads toward the chandlery corridor.");
  SC_HOT(SC_H_FEATURE,7,VX+140,VY+66,50,8,"LAMP ROW","Warm berth lamps. They never quite go out.");
  SC_HOT(SC_H_PROP,10,VX+100,VY+130,40,20,"BENCH","Waiting seat. Someone left a coffee ring.");
 }else if(sc_room==SC_R_SHOP){
  SC_HOT(SC_H_PROP,1,VX+180,VY+100,70,40,"STOCK CRATE","Sealed clamps and off-board spares.");
  SC_HOT(SC_H_FEATURE,1,VX+120,VY+40,56,40,"LEDGER","Hand ink. Prices the main board never prints.");
  SC_HOT(SC_H_PROP,5,VX+12,VY+20,64,90,"SHELF","Tools, clamps, and mystery tins.");
  SC_HOT(SC_H_PROP,11,VX+244,VY+20,64,90,"SPARE RACK","Off-ledger stock the board never lists.");
  SC_HOT(SC_H_FEATURE,8,VX+90,VY+100,140,20,"COUNTER","Scratch marks from years of deals.");
  SC_HOT(SC_H_PROP,12,VX+148,VY+8,40,28,"BACK DOOR","Cargo bay access if the chandler trusts you.");
 }else if(sc_room==SC_R_CANTEEN){
  SC_HOT(SC_H_PROP,2,VX+40,VY+110,240,28,"BAR TOP","Sticky rings and tip jars.");
  SC_HOT(SC_H_FEATURE,2,VX+12,VY+78,40,48,"JUKE","Neon Transit loops forever.");
  SC_HOT(SC_H_PROP,6,VX+28,VY+18,260,40,"BOTTLES","Backlit glass. Pretty, not free.");
  SC_HOT(SC_H_PROP,13,VX+16,VY+100,50,28,"LEFT BOOTH","Gossip booth. Travellers leave tip notes.");
  SC_HOT(SC_H_PROP,14,VX+254,VY+100,50,28,"RIGHT BOOTH","Warm seat, cold drink, louder rumours.");
  SC_HOT(SC_H_FEATURE,9,VX+40,VY+64,240,6,"NEON STRIP","Pink light that never sleeps.");
 }else if(sc_room==SC_R_CARGO){
  SC_HOT(SC_H_PROP,3,VX+130,VY+70,50,42,"TIP CRATE","Marked for the board. SPEAK to the loader for pay.");
  SC_HOT(SC_H_FEATURE,5,VX+12,VY+8,296,20,"CRANE","Overhead hoist. Not for climbing.");
  SC_HOT(SC_H_PROP,15,VX+16,VY+70,50,42,"PALLET A","Sealed freight. Manifest says 'misc'.");
  SC_HOT(SC_H_PROP,16,VX+246,VY+70,50,42,"PALLET B","Hazard tape and a polite warning.");
  SC_HOT(SC_H_FEATURE,10,VX+4,VY+120,312,8,"HAZARD LINE","Do not cross while loaders shout.");
  SC_HOT(SC_H_PROP,17,VX+70,VY+40,40,24,"CLIPBOARD","Today's tip jobs. Loader keeps the pen.");
 }else if(sc_room==SC_R_GUILD){
  SC_HOT(SC_H_FEATURE,3,VX+90,VY+14,140,58,"TRACKED SCREEN","Mission slate. Kei points here when you stall.");
  SC_HOT(SC_H_PROP,7,VX+50,VY+110,220,28,"GUILD DESK","Quiet teal and paperwork.");
  SC_HOT(SC_H_PROP,18,VX+16,VY+80,26,40,"PLANTER","A stubborn green thing thrives under UV.");
  SC_HOT(SC_H_PROP,19,VX+30,VY+120,36,28,"GUEST CHAIR","Kei keeps it warm for late arrivals.");
  SC_HOT(SC_H_FEATURE,11,VX+120,VY+70,80,20,"CARPET","Guild teal runner. Soft steps only.");
  SC_HOT(SC_H_PROP,20,VX+254,VY+120,36,28,"SECOND CHAIR","Surveyors leave star charts on the arm.");
 }else if(sc_room==SC_R_CLINIC){
  SC_HOT(SC_H_PROP,4,VX+240,VY+40,60,80,"MEDKIT LOCKER","One issue only. TAKE after SPEAK with the medic.");
  SC_HOT(SC_H_PROP,8,VX+70,VY+100,180,36,"BAY BED","Clean sheets. You hope you never need them.");
  SC_HOT(SC_H_FEATURE,12,VX+24,VY+50,50,36,"VITALS SCREEN","Soft beeps. Green is good.");
  SC_HOT(SC_H_PROP,21,VX+80,VY+70,44,20,"PILLOW","Sterile fluff. Still somehow comforting.");
  SC_HOT(SC_H_FEATURE,13,VX+50,VY+10,40,20,"CEILING LAMP","Soft clinic light. No shadows, on purpose.");
  SC_HOT(SC_H_PROP,22,VX+200,VY+100,36,40,"TRAY","Bandages and a cup of cold tea.");
 }else if(sc_room==SC_R_CUSTOMS){
  SC_HOT(SC_H_FEATURE,4,VX+100,VY+18,120,60,"WARRANT GLASS","Your legal heat, rendered in unfriendly cyan.");
  SC_HOT(SC_H_FEATURE,6,VX+24,VY+50,22,80,"SCANNER","Walk through only if you like questions.");
  SC_HOT(SC_H_FEATURE,14,VX+274,VY+50,22,80,"RIGHT ARCH","Mirror scanner. Same questions, other side.");
  SC_HOT(SC_H_PROP,23,VX+80,VY+110,160,28,"CUSTOMS DESK","Stamps, forms, and a tired officer.");
  SC_HOT(SC_H_FEATURE,15,VX+60,VY+54,200,8,"SCAN BEAMS","Red lines. Restricted goods light up.");
  SC_HOT(SC_H_PROP,24,VX+140,VY+80,40,24,"STAMP PAD","Ink still wet. Do not touch.");
 }
 #undef SC_HOT
 return n;
}
/* ---- lush room illustration (main window) ---- */
static void sc_win(int x,int y,int w,int h,const char *title,unsigned edge){
 rect(x,y,w,h,RGB(10,14,20));
 rect(x,y,w,12,RGB(28,36,48));
 rect(x,y,w,1,edge);rect(x,y+h-1,w,1,edge);rect(x,y,1,h,edge);rect(x+w-1,y,1,h,edge);
 text((x+6)/8,(y+2)/8,WHITE,"%.14s",title);
}
static void sc_scene_sky(int x,int y,int w,int h,unsigned hi,unsigned lo){
 for(int row=0;row<h;row++){
  float t=row/(float)(h>1?h-1:1);
  rect(x,y+row,w,1,mix_rgb(hi,lo,t));
 }
}
static void sc_draw_person_sprite(int x,int y,const ScNpc *p,int selected){
 unsigned ink=faction_colors[p->role%FACTION_COUNT];
 if(selected){rect(x-2,y-2,52,66,GOLD);rect(x-1,y-1,50,64,RGB(8,12,18));}
 rect(x+8,y+48,12,12,mix_rgb(ink,RGB(10,12,16),.5f));
 rect(x+22,y+48,12,12,mix_rgb(ink,RGB(10,12,16),.5f));
 rect(x+10,y+28,28,24,mix_rgb(ink,RGB(24,32,40),.4f));
 rect(x+6,y+28,6,20,mix_rgb(ink,RGB(24,32,40),.4f));
 rect(x+36,y+28,6,20,mix_rgb(ink,RGB(24,32,40),.4f));
 rect(x+8,y+2,32,28,RGB(6,12,20));
 if(!strcmp(p->name,"KEI"))draw_kei(x+8,y+2,28,0);
 else draw_portrait(x+8,y+2,28,28,game.system*37+p->role*17+(int)(p->name[0]*3),p->role);
 rect(x+8,y+28,32,1,ink);
 {int nw=(int)strlen(p->name);if(nw>8)nw=8;text((x+4)/8,(y+58)/8,selected?GOLD:WHITE,"%.8s",p->name);}
}
static void sc_illust_arrivals(int x,int y,int w,int h){
 /* McQuarrie soft industrial: warm cream panels, blue-grey metal, light well. */
 sc_scene_sky(x,y,w,h,RGB(72,78,86),RGB(28,32,40));
 /* Stars / lane through big viewport */
 rect(x+18,y+10,w-36,54,RGB(8,14,26));
 rect(x+18,y+10,w-36,2,RGB(180,160,120));
 for(int i=0;i<28;i++){unsigned n=art_hash((unsigned)(i*97+game.system));pixel(x+30+(n% (w-60)),y+16+((n>>8)%40),((n>>16)&1)?WHITE:RGB(160,180,200));}
 /* Freighter silhouette in viewport */
 rect(x+w/2-50,y+28,70,14,RGB(90,100,110));rect(x+w/2-30,y+22,30,8,RGB(70,80,90));
 pixel(x+w/2+10,y+30,GOLD);pixel(x+w/2-40,y+34,RGB(200,120,80));
 /* Ceiling beams + warm hanging lamps */
 for(int i=0;i<5;i++){rect(x+10+i*62,y+66,50,4,RGB(90,96,104));rect(x+28+i*62,y+70,14,6,RGB(255,210,150));}
 /* Side bulkheads — cream / ochre panels */
 for(int row=0;row<8;row++){
  rect(x+4,y+78+row*8,28,7,((row)&1)?RGB(140,120,90):RGB(110,96,72));
  rect(x+w-32,y+78+row*8,28,7,((row)&1)?RGB(140,120,90):RGB(110,96,72));
 }
 /* Floor tiles — countable deck */
 for(int row=0;row<7;row++)for(int col=0;col<8;col++){
  int fx=x+34+col*32,fy=y+h-64+row*8;
  unsigned c=((col+row)&1)?RGB(96,100,108):RGB(72,76,84);
  rect(fx,fy,30,7,c);rect(fx,fy,30,1,RGB(160,150,130));
 }
 /* Traffic board (right) */
 rect(x+w-100,y+78,86,58,RGB(36,40,48));
 rect(x+w-96,y+82,78,14,RGB(50,110,120));
 rect(x+w-96,y+100,78,10,RGB(50,120,80));
 rect(x+w-96,y+114,78,10,AMBER);
 text((x+w-90)/8,(y+84)/8,CYAN,"BERTHS");
 /* Observation rail */
 rect(x+36,y+h-72,w-72,5,RGB(150,140,120));
 rect(x+36,y+h-68,w-72,3,RGB(70,66,58));
 /* Round-ish hatch frames (MacVenture door feel) */
 rect(x+40,y+80,36,48,RGB(50,48,44));rect(x+44,y+86,28,38,RGB(18,20,26));
 rect(x+w-76,y+80,36,48,RGB(50,48,44));rect(x+w-72,y+86,28,38,RGB(18,20,26));
}
static void sc_illust_shop(int x,int y,int w,int h){
 /* Warm ochre chandlery — McQuarrie crate bay, not neon. */
 sc_scene_sky(x,y,w,h,RGB(120,96,64),RGB(42,32,20));
 for(int row=0;row<6;row++)for(int col=0;col<9;col++)
  rect(x+6+col*34,y+h-56+row*8,32,7,((col+row)&1)?RGB(130,104,70):RGB(100,80,52));
 for(int s=0;s<4;s++){
  rect(x+8,y+12+s*26,64,22,RGB(140,112,70));
  rect(x+12,y+16+s*26,16,12,GOLD);rect(x+32,y+16+s*26,16,12,RGB(90,130,140));rect(x+52,y+16+s*26,14,12,RGB(170,100,60));
  rect(x+w-72,y+12+s*26,64,22,RGB(140,112,70));
  rect(x+w-68,y+16+s*26,16,12,RGB(80,140,150));rect(x+w-48,y+16+s*26,16,12,RGB(190,150,80));
 }
 rect(x+78,y+h-78,w-156,28,RGB(80,60,36));
 rect(x+78,y+h-86,w-156,10,RGB(160,120,70));
 for(int i=0;i<5;i++)rect(x+90+i*28,y+h-74,18,10,RGB(50,38,22));
 rect(x+w/2-28,y+40,56,40,RGB(60,44,26));
 rect(x+w/2-22,y+46,44,20,RGB(220,200,160));
 text((x+w/2-18)/8,(y+52)/8,AMBER,"LEDGER");
 rect(x+w/2-20,y+8,40,28,RGB(40,30,18));rect(x+w/2-14,y+12,28,20,RGB(16,12,8));
}
static void sc_illust_canteen(int x,int y,int w,int h){
 sc_scene_sky(x,y,w,h,RGB(90,36,78),RGB(28,10,26));
 for(int row=0;row<5;row++)rect(x,y+h-48+row*8,w,8,mix_rgb(RGB(70,30,58),RGB(30,12,28),row/5.f));
 /* Backlit bottle wall */
 rect(x+20,y+12,w-40,50,RGB(40,16,40));
 for(int i=0;i<12;i++){
  unsigned c=RGB(120+i*8,50+(i%4)*20,140);
  rect(x+28+i*22,y+18,12,36,c);pixel(x+32+i*22,y+22,RGB(255,220,240));
 }
 /* Neon strip */
 rect(x+20,y+64,w-40,3,RGB(255,80,180));
 /* Bar top */
 rect(x+24,y+h-70,w-48,26,RGB(90,44,70));
 rect(x+24,y+h-78,w-48,10,RGB(140,70,110));
 for(int i=0;i<7;i++){rect(x+40+i*36,y+h-92,10,14,mix_rgb(RGB(220,180,140),GOLD,.35f));pixel(x+43+i*36,y+h-94,GOLD);}
 /* Booth seats */
 rect(x+16,y+h-110,50,28,RGB(60,24,50));rect(x+w-66,y+h-110,50,28,RGB(60,24,50));
 /* Juke */
 rect(x+12,y+78,40,48,RGB(36,36,52));rect(x+16,y+82,32,22,RGB(160,50,180));
 for(int i=0;i<4;i++)pixel(x+20+i*6,y+110,RGB(80,220,255));
 text((x+16)/8,(y+116)/8,CYAN,"JUKE");
}
static void sc_illust_cargo(int x,int y,int w,int h){
 sc_scene_sky(x,y,w,h,RGB(70,62,34),RGB(22,18,10));
 for(int row=0;row<6;row++)for(int col=0;col<9;col++)
  rect(x+4+col*35,y+h-58+row*8,33,7,((col+row)&1)?RGB(90,78,42):RGB(68,58,32));
 /* Crane */
 rect(x+12,y+8,w-24,5,RGB(120,120,90));rect(x+w/2-3,y+8,6,50,RGB(90,90,60));
 rect(x+w/2-14,y+54,28,8,RGB(70,70,40));
 /* Crate stacks */
 for(int i=0;i<5;i++){
  unsigned c=RGB(120,100,48);
  rect(x+16+i*58,y+h-108,50,42,c);
  rect(x+16+i*58,y+h-108,50,5,RGB(180,160,90));
  rect(x+30+i*58,y+h-92,20,8,RGB(40,32,16));
  if(i==2){rect(x+20+i*58,y+h-120,42,14,GOLD);text((x+24+i*58)/8,(y+h-118)/8,RGB(20,16,8),"TIP");}
 }
 /* Hazard stripe */
 rect(x+4,y+h-64,w-8,4,RGB(200,160,40));
 for(int i=0;i<(w-8)/12;i++)if(i&1)rect(x+4+i*12,y+h-64,12,4,RGB(20,16,8));
}
static void sc_illust_guild(int x,int y,int w,int h){
 sc_scene_sky(x,y,w,h,RGB(28,78,64),RGB(8,28,22));
 for(int row=0;row<5;row++)rect(x,y+h-46+row*8,w,8,mix_rgb(RGB(36,70,58),RGB(14,30,24),row/5.f));
 /* Carpet runner */
 rect(x+w/2-40,y+70,80,h-120,RGB(20,60,48));
 /* Big tracked screen */
 rect(x+w/2-70,y+14,140,58,RGB(10,36,30));
 rect(x+w/2-64,y+20,128,36,RGB(40,120,96));
 text((x+w/2-48)/8,(y+30)/8,CYAN,"TRACKED MISSION");
 rect(x+w/2-50,y+56,100,8,GOLD);
 /* Desk */
 rect(x+50,y+h-78,w-100,32,RGB(24,70,56));
 rect(x+50,y+h-86,w-100,10,RGB(60,140,110));
 /* Twin chairs */
 rect(x+30,y+h-100,36,28,RGB(18,50,40));rect(x+w-66,y+h-100,36,28,RGB(18,50,40));
 /* Plant / accent */
 rect(x+16,y+80,18,40,RGB(20,80,40));rect(x+12,y+70,26,14,RGB(40,140,70));
}
static void sc_illust_clinic(int x,int y,int w,int h){
 sc_scene_sky(x,y,w,h,RGB(56,96,112),RGB(16,32,42));
 for(int row=0;row<5;row++)rect(x,y+h-46+row*8,w,8,mix_rgb(RGB(70,100,112),RGB(28,48,58),row/5.f));
 /* Soft ceiling lights */
 for(int i=0;i<4;i++){circle(x+50+i*70,y+16,8,RGB(180,230,240));rect(x+46+i*70,y+24,16,3,RGB(120,180,190));}
 /* Bed */
 rect(x+70,y+h-86,w-140,36,RGB(210,230,236));
 rect(x+70,y+h-94,w-140,10,RGB(140,200,210));
 rect(x+80,y+h-110,44,20,WHITE);
 rect(x+90,y+h-106,24,8,RGB(200,220,230));
 /* Med locker */
 rect(x+w-78,y+40,60,80,RGB(60,100,112));
 rect(x+w-70,y+50,44,30,RGB(140,220,230));
 text((x+w-64)/8,(y+58)/8,RGB(10,40,50),"MEDKIT");
 rect(x+w-70,y+90,44,20,RGB(40,70,80));
 /* Monitor */
 rect(x+24,y+50,50,36,RGB(30,50,60));rect(x+28,y+54,42,24,RGB(80,200,180));
}
static void sc_illust_customs(int x,int y,int w,int h){
 sc_scene_sky(x,y,w,h,RGB(96,40,40),RGB(28,10,10));
 for(int row=0;row<5;row++)rect(x,y+h-46+row*8,w,8,mix_rgb(RGB(80,36,36),RGB(30,12,12),row/5.f));
 /* Scanner arch */
 rect(x+24,y+50,22,h-100,RGB(120,60,60));
 rect(x+w-46,y+50,22,h-100,RGB(120,60,60));
 rect(x+24,y+50,w-48,8,RGB(180,80,80));
 for(int i=0;i<6;i++)rect(x+50+i*36,y+54,4,h-110,mix_rgb(RED,RGB(40,10,10),.4f));
 /* Desk */
 rect(x+60,y+h-78,w-120,30,RGB(90,42,42));
 rect(x+60,y+h-86,w-120,10,RGB(150,70,70));
 /* Warrant glass */
 rect(x+w/2-60,y+18,120,60,RGB(50,18,18));
 rect(x+w/2-54,y+24,108,22,RED);
 text((x+w/2-40)/8,(y+30)/8,WHITE,"WARRANT");
 rect(x+w/2-54,y+50,108,20,RGB(24,24,32));
 text((x+w/2-36)/8,(y+54)/8,CYAN,"SCANNING");
}
static void sc_draw_main_scene(void){
 const int VX=72,VY=24,VW=320,VH=160;
 sc_win(VX-2,VY-14,VW+4,VH+18,"MAIN",CYAN);
 text((VX+6)/8,(VY-12)/8,CYAN,"%.16s",sc_room_title(sc_room));
 if(sc_room==SC_R_ARRIVALS)sc_illust_arrivals(VX,VY,VW,VH);
 else if(sc_room==SC_R_SHOP)sc_illust_shop(VX,VY,VW,VH);
 else if(sc_room==SC_R_CANTEEN)sc_illust_canteen(VX,VY,VW,VH);
 else if(sc_room==SC_R_CARGO)sc_illust_cargo(VX,VY,VW,VH);
 else if(sc_room==SC_R_GUILD)sc_illust_guild(VX,VY,VW,VH);
 else if(sc_room==SC_R_CLINIC)sc_illust_clinic(VX,VY,VW,VH);
 else sc_illust_customs(VX,VY,VW,VH);
 ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 /* People stand at room-specific spots */
 for(int i=0;i<pn;i++){
  int px,py;sc_person_pos(sc_room,i,&px,&py);
  sc_draw_person_sprite(VX+px,VY+py,&people[i],sc_hot<hn&&hot[sc_hot].kind==SC_H_PERSON&&hot[sc_hot].id==i);
 }
 /* Highlight selected hotspot */
 if(sc_hot>=0&&sc_hot<hn){
  ScHot *h=&hot[sc_hot];
  unsigned c=GOLD;
  rect(h->x-2,h->y-2,h->w+4,1,c);rect(h->x-2,h->y+h->h+1,h->w+4,1,c);
  rect(h->x-2,h->y-2,1,h->h+4,c);rect(h->x+h->w+1,h->y-2,1,h->h+4,c);
 }
 /* Door plaques show destination; ship plaque always amber and obvious */
 for(int i=0;i<hn;i++)if(hot[i].kind==SC_H_EXIT){
  unsigned c=(sc_hot==i)?GOLD:(hot[i].id==SC_EXIT_SHIP?AMBER:CYAN);
  rect(hot[i].x,hot[i].y,hot[i].w,hot[i].h,RGB(8,14,22));
  rect(hot[i].x,hot[i].y,hot[i].w,1,c);
  rect(hot[i].x,hot[i].y+hot[i].h-1,hot[i].w,1,c);
  if(hot[i].id==SC_EXIT_SHIP)text((hot[i].x+4)/8,(hot[i].y+6)/8,c,">> SHIP");
  else text((hot[i].x+4)/8,(hot[i].y+6)/8,c,"%.7s",hot[i].label);
 }
}
static void sc_draw_commands(void){
 static const char *v[]={"LOOK","SPEAK","GO","TAKE"};
 sc_win(4,2,472,18,"COMMANDS",GOLD);
 for(int i=0;i<SC_V_COUNT;i++){
  int x=16+i*90;
  if(i==sc_verb){rect(x-2,6,80,12,RGB(40,70,50));rect(x-2,6,80,1,GOLD);}
  text(x/8,1,i==sc_verb?GOLD:DIM,"%s",v[i]);
 }
 text(48,1,AMBER,"L/R");text(52,1,DIM,"verb");text(57,1,AMBER,"TRI");text(61,1,GOLD,"SHIP");
}
static void sc_draw_inventory(void){
 sc_win(4,24,64,160,"PACK",AMBER);
 text(1,5,DIM,"HOLD");
 int y=7;
 if(game.gift_flags&1u){text(1,y,CYAN,"CLAMP");y+=2;}
 if(game.gift_flags&2u){text(1,y,CYAN,"MEDKIT");y+=2;}
 if(!(game.gift_flags&3u)){text(1,y,DIM,"(empty)");y+=2;}
 if(game.passenger_dest>=0){text(1,y,GOLD,"PAX");y+=2;text(1,y,WHITE,"%.8s",game.systems[game.passenger_dest].name);y+=2;}
 text(1,15,DIM,"HERE");
 text(1,17,CYAN,"%.8s",sc_room_title(sc_room));
 text(1,19,AMBER,"TRI");text(1,20,GOLD,"SHIP");
 text(1,22,DIM,"O=DECK");
}
static void sc_draw_exits(void){
 sc_win(400,24,76,160,"EXITS",CYAN);
 int y=5;
 /* Ship always first and obvious */
 {int sel=0;ScHot hot[24];int hn=sc_hotspots(hot,24);
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT&&hot[h].id==SC_EXIT_SHIP&&h==sc_hot)sel=1;
  if(sel)rect(404,y*8-2,68,14,RGB(60,50,20));
  text(51,y,sel?GOLD:AMBER,">>SHIP");y+=2;}
 int ex[4],en=sc_exits(sc_room,ex,4);
 text(51,y,DIM,"doors");y+=2;
 for(int i=0;i<en;i++){
  int sel=0;ScHot hot[24];int hn=sc_hotspots(hot,24);
  for(int h=0;h<hn;h++)if(hot[h].kind==SC_H_EXIT&&hot[h].id==ex[i]&&h==sc_hot)sel=1;
  if(sel)rect(404,y*8-2,68,14,RGB(30,60,70));
  text(51,y,sel?GOLD:WHITE,"%.8s",sc_room_short(ex[i]));y+=2;
 }
 text(51,20,DIM,"GO+X");
 text(51,22,AMBER,"TRI ship");
}
static int sc_talk_choices(const ScNpc *p,const char **out,int maxn){
 int n=0;
 #define SC_CH(S) do{if(n<maxn)out[n++]=(S);}while(0)
 SC_CH("Ask about them");
 if(p->act==SC_ACT_SHOP)SC_CH("Browse their stock");
 else if(p->act==SC_ACT_GIFT)SC_CH(p->offer);
 else if(p->act==SC_ACT_QUEST)SC_CH(p->offer);
 else if(p->act==SC_ACT_TAXI)SC_CH(p->offer);
 else SC_CH(p->offer&&p->offer[0]?p->offer:"Ask for a tip");
 SC_CH("Never mind");
 #undef SC_CH
 return n;
}
static void sc_draw_text_box(void){
 sc_win(4,190,472,78,"TEXT",CYAN);
 ScHot hot[24]; int hn=sc_hotspots(hot,24);
 if(sc_menu==SC_MENU_SHOP){
  int list[8],ln=sc_exclusive_catalog(list,8);
  text(1,25,GOLD,"CHANDLER STOCK");
  if(!ln)text(1,27,DIM,"Sold out today.");
  else {int idx=list[sc_shop_row%ln];text(1,27,WHITE,"%.14s  %.1fU",equipment_list_names[idx],equipment_costs[idx]*.1f);
   text(1,29,DIM,"UP/DOWN  X buy  TRI back");}
  return;
 }
 if(sc_menu==SC_MENU_TALK){
  ScNpc people[3]; int pn=sc_fill_npcs(sc_room,people,3);
  if(sc_talk_who<0||sc_talk_who>=pn){sc_menu=SC_MENU_NONE;return;}
  ScNpc *p=&people[sc_talk_who];
  text(1,25,GOLD,"%.10s",p->name);
  text_wrap(14,25,42,1,WHITE,p->line,0);
  const char *ch[4]; int cn=sc_talk_choices(p,ch,4);
  if(sc_talk_row<0)sc_talk_row=0;if(sc_talk_row>=cn)sc_talk_row=cn-1;
  for(int i=0;i<cn;i++){
   int y=27+i;
   text(1,y,i==sc_talk_row?AMBER:DIM,i==sc_talk_row?">":" ");
   text_wrap(3,y,54,1,i==sc_talk_row?WHITE:DIM,ch[i],0);
  }
  return;
 }
 if(hn<=0){text_wrap(1,25,56,3,WHITE,sc_room_blurb(sc_room),0);return;}
 if(sc_hot<0)sc_hot=0;if(sc_hot>=hn)sc_hot=hn-1;
 ScHot *h=&hot[sc_hot];
 text(1,25,GOLD,"%.14s",h->label);
 text_wrap(1,27,56,2,WHITE,h->look?h->look:sc_room_blurb(sc_room),0);
 {static const char *v[]={"LOOK","SPEAK","GO","TAKE"};
  text(1,30,AMBER,"%s > %.12s",v[sc_verb],h->label);}
 text(28,30,DIM,"U/D pick  TRI ship  O deck");
}
static void sc_draw_ui(void){
 sc_build_map();
 rect(0,0,W,H,RGB(8,12,20));
 for(int i=0;i<H;i+=3)rect(0,i,W,1,RGB(10,14,22));
 sc_draw_commands();
 sc_draw_inventory();
 sc_draw_main_scene();
 sc_draw_exits();
 sc_draw_text_box();
}
static void sc_board_ship(void){
 /* Walk input returns 0 so Circle can leave; Triangle boards immediately. */
 page=HOME;sc_menu=0;message(&game,"Back aboard. Command deck ready.");game.cue=SFX_UI;
}
static void sc_do_npc_choice(ScNpc *p,int choice){
 /* choice 0 = ask/flavour, 1 = accept offer (or tip), last = never mind */
 const char *ch[4]; int cn=sc_talk_choices(p,ch,4);
 if(choice<0||choice>=cn)return;
 if(choice==cn-1){sc_menu=SC_MENU_NONE;message(&game,"You nod and step back.");game.cue=SFX_UI;return;}
 speak(&game,p->role==LAW?VOICE_LAW:p->role==EXPLORERS?VOICE_KEI:!strcmp(p->name,"VENN")?VOICE_VENN:VOICE_DOCK,p->line);
 if(choice==0){
  message(&game,p->line);sc_menu=SC_MENU_NONE;game.cue=SFX_UI;return;
 }
 /* Accept offer / do act */
 sc_menu=SC_MENU_NONE;
 if(p->act==SC_ACT_TALK){message(&game,p->offer);game.cue=SFX_UI;}
 else if(p->act==SC_ACT_SHOP){sc_menu=SC_MENU_SHOP;sc_shop_row=0;message(&game,"Chandler stock - exclusive to this deck.");}
 else if(p->act==SC_ACT_GIFT){
  int bit=p->gift_bit>=0?p->gift_bit:0;
  if(game.gift_flags&(1u<<bit)){message(&game,"Already took that gift.");return;}
  game.gift_flags|=1u<<bit;
  if(bit==0){if(!(game.upgrades&8)){game.upgrades|=8;message(&game,"Free cargo clamp fitted (+8t).");}else {game.credits+=200;message(&game,"Clamp spare sold for 20 U.");}}
  else {game.energy=100;message(&game,"Medkit used - energy restored.");}
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
  message(&game,"GO needs a door or YOUR SHIP.");game.cue=SFX_UI;return;
 }
 if(sc_verb==SC_V_SPEAK){
  if(h->kind!=SC_H_PERSON){message(&game,"Nobody there to speak with.");game.cue=SFX_UI;return;}
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
  if(h->kind==SC_H_PROP){message(&game,"Bolted down. SPEAK to the person who owns it.");game.cue=SFX_UI;return;}
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
 if(pressed&PSP_CTRL_LEFT){sc_verb=(sc_verb+SC_V_COUNT-1)%SC_V_COUNT;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_RIGHT){sc_verb=(sc_verb+1)%SC_V_COUNT;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_UP){sc_hot=(sc_hot+hn-1)%hn;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_DOWN){sc_hot=(sc_hot+1)%hn;game.cue=SFX_SELECT;}
 if(pressed&PSP_CTRL_CROSS)sc_apply();
 return 0;
}
