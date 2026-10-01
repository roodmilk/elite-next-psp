/* Shared authored raster kit and deterministic room recipes. No RNG mutation.
 * Scene anchors are the authority for artwork overlays AND interaction boxes.
 * Seven plates are shared by every hub; overrides keep Reorte's authored bar. */
#ifndef STATION_ROOM_KIT_H
#define STATION_ROOM_KIT_H
#include "generated/lave-room-pixels.h"
#include "generated/lave-props-pixels.h"
#include "generated/lave-crew-pixels.h"
typedef struct {unsigned char x,y,w,h;} ScAnchor;
typedef struct {ScAnchor hero,service,people[2];} ScRoomRecipe;
static const ScRoomRecipe sc_recipes[7]={
 {{40,8,211,67},{252,27,41,32},{{99,94,32,64},{207,94,32,64}}},
 {{88,83,175,31},{137,69,24,16},{{99,94,32,64},{207,94,32,64}}},
 {{38,78,96,21},{136,74,23,44},{{99,94,32,64},{207,94,32,64}}},
 {{132,30,62,25},{51,34,21,27},{{99,94,32,64},{207,94,32,64}}},
 {{100,16,143,67},{150,99,51,18},{{99,94,32,64},{207,94,32,64}}},
 {{102,70,113,46},{141,27,66,49},{{73,94,32,64},{207,94,32,64}}},
 {{94,35,75,86},{231,77,29,22},{{180,94,32,64},{207,94,32,64}}}
};
static int sc_raster_room(void){return !(sc_room==SC_R_CANTEEN&&bar_preview_at())&&!(sc_room==SC_R_ARRIVALS&&station_authored_arrivals_at());}
static int sc_baked_lave_arrivals(void){return sc_lave()&&sc_room==SC_R_ARRIVALS;}
static unsigned sc_room_seed(int room){StationIdentity s=sc_identity();return s.seed^((unsigned)(room+1)*0x45d9f3bu);}
static int sc_module_id(int room,int slot){
 StationIdentity s=sc_identity();unsigned seed=sc_room_seed(room);
 static const int fixed[]={1,6,2,0,5,4,1};
 if(slot==0)return fixed[room];
 if(room==5)return 7;
 int id=s.economy>=5?((seed&1)?2:0):s.economy<=1?((seed&1)?6:3):((seed&1)?7:5);
 return id==fixed[room]?3:id;
}
static const char *sc_module_names[]={"CARGO STACK","DECK TERMINAL","ALIEN PLANT","MAINTENANCE BOT","MEDICAL TROLLEY","MINERAL SAMPLE","CABLE CART","WORK LAMP"};
static const char *sc_module_notes[]={
 "The stack is secured for handling, with scuffed corners and old marks beneath the current labels. Whatever is inside has already travelled farther than the condition of the packaging would suggest. These are working supplies, not abandoned salvage. If you need to know where a particular shipment belongs, the loader is the person to ask.",
 "The terminal's casing has been rubbed smooth around the controls. Its screen casts a small pool of cool light over the deck, one more piece of equipment that remains awake while the shifts change around it. It belongs to the station rather than to any one crew. Your usual flight and ship-service menus are still available when you return aboard.",
 "Broad leaves spread above a heavy planter, their uneven edges catching the overhead light. Someone has made room for a living thing where another container could easily have been stacked instead. A little soil has escaped onto the rim. For all the equipment keeping this place habitable, the plant is the thing that makes the room feel as though people live here.",
 "The maintenance robot waits with its tools folded close to its body. Scratches in the casing have been painted over more than once, and its small indicator continues to blink with patient regularity. It looks less like a machine awaiting orders than a colleague enjoying a quiet minute. Whatever name the crew has given it, nobody has bothered to write it on the shell.",
 "The trolley carries sealed supplies in carefully arranged compartments. Handles and latches are positioned so that someone in a hurry can find what they need without searching. Unlike the cargo stacks, nothing here appears to have been left wherever it would fit. You keep your hands clear; if there is something you need, it is better to ask the medical staff.",
 "A mineral specimen rests inside a protective container. The plain housing gives very little away about the journey that brought it here: the landing, the search and the decision that this particular sample was worth keeping. It is a reference for visitors, not a fresh discovery for your own log. Records from your travels belong in the surface Codex.",
 "A thick cable is wound around the cart's reel, ready to be paid out when the next job needs it. The outer covering bears the pale marks left by dragging and repeated handling. Up close, it is easy to appreciate how much ordinary maintenance lies behind the station's clean departure notices. Nobody sends a freighter out on the strength of a tidy sign alone.",
 "The work lamp throws a steady amber light across the nearby deck. Its stand is broad enough to resist a careless knock, and the casing has the battered look of equipment that is moved whenever somebody needs both hands free. It is a practical little island of warmth in the room, illuminating the floor rather than competing with the distant lights outside."
};
static void sc_raster_sprite(const unsigned char *src,int x,int y,int w,int h){
 for(int yy=0;yy<h;yy++)for(int xx=0;xx<w;xx++){unsigned c=src[yy*w+xx];if(c<32)pixel(x+xx,y+yy,lave_room_palette[c]);}
}
static void sc_raster_crew(int x,int y,int i,int selected){
 int sprite=i==0?sc_room:7;
 sc_raster_sprite(lave_crew_pixels[sprite],x,y,32,64);
 if(selected){line(x,y+62,x+31,y+62,SC_AMBER);pixel(x,y+60,SC_AMBER);pixel(x+31,y+60,SC_AMBER);}
}
static void sc_raster_draw(int x,int y,int room){
 unsigned pal[32];StationIdentity s=sc_identity();
 unsigned accent=s.economy>=5?RGB(111,132,75):s.economy<=1?RGB(109,138,153):RGB(138,114,151);
 for(int i=0;i<32;i++){
  pal[i]=lave_room_palette[i];
  if(!sc_lave()&&i>=8&&i<14)pal[i]=mix_rgb(pal[i],accent,.22f+(s.seed%3)*.07f);
  if(high_contrast&&i<8)pal[i]=mix_rgb(pal[i],RGB(0,5,13),.25f);
 }
 const unsigned char *src=sc_baked_lave_arrivals()?lave_arrivals_hero_pixels:lave_room_pixels[room];
 for(int yy=0;yy<168;yy++)for(int xx=0;xx<340;xx++)pixel(x+xx,y+yy,pal[src[yy*340+xx]]);
 if(room==0){
  int tx=x+164+(int)fmodf(preview_time*4.f+(s.seed%31),52.f),ty=y+37;
  line(tx,ty,tx+5,ty,RGB(169,177,170));pixel(tx-1,ty,RGB(85,190,194));pixel(tx+2,ty-1,RGB(82,112,128));
 }
 if(room==2){int sy=(int)(preview_time*4)%5;pixel(x+118,y+81-sy,RGB(120,144,154));}
 /* Tiny screen activity anchored to the painted service, not random walls. */
 for(int slot=0;slot<2&&!sc_baked_lave_arrivals();slot++){
  int id=sc_module_id(room,slot),px=slot?268:36,py=128;
  sc_raster_sprite(lave_props_pixels[id],x+px,y+py,32,32);
  if(id==3&&((int)(preview_time*3)&1))pixel(x+px+16,y+py+12,RGB(160,238,238));
 }
 ScAnchor a=sc_recipes[room].service;
 if(room!=3&&((int)(preview_time*2)+(int)(s.seed&7))%4==0)pixel(x+a.x+3,y+a.y+3,RGB(160,238,238));
}
#endif
