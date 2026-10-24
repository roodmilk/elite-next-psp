/* Selected authored artwork, embedded at the PSP's native resolution. */
#include "opening-bitmaps.h"
static void opening_bitmap_draw(const OpeningBitmap *b,int x,int y,int w,int h){
 if(!b||w<=0||h<=0)return;
 int x0=x<0?0:x,y0=y<0?0:y,x1=x+w>W?W:x+w,y1=y+h>H?H:y+h;
 for(int yy=y0;yy<y1;yy++){int sy=(yy-y)*b->h/h;const unsigned char *p=b->pixels+sy*b->w;
  for(int xx=x0;xx<x1;xx++){unsigned ink=b->palette[p[(xx-x)*b->w/w]];if(ink>>24)fb[yy*STRIDE+xx]=ink;}
 }
}
/* Canonical ship-computer face: opening, cockpit and dialogue share this asset. */
static void ship_computer_portrait(int x,int y,int w,int h){opening_bitmap_draw(&opening_portrait6_bitmap,x,y,w,h);}
static void opening_portrait(const char *name,int x,int y,int size){
 int id=0;
 if(strstr(name,"PLAYER")||strstr(name,"YOU")){portrait_draw(x,y,size,size,game.commander_portrait,EXPLORERS);return;}
 if(strstr(name,"MUM")||strstr(name,"Mum"))id=1;
 else if(strstr(name,"DEALER")||strstr(name,"SALE"))id=2;
 else if(strstr(name,"PRESENTER"))id=3;
 else if(strstr(name,"NEWS"))id=4;
 else if(strstr(name,"DOCKWORKER"))id=5;
 else if(strstr(name,"COMPUTER")||strstr(name,"Computer")||strstr(name,"GAME"))id=6;
 else if(strstr(name,"THARGOID"))id=7;
 else if(strstr(name,"Kat")||strstr(name,"Scott")){portrait_draw(x,y,size,size,portrait_seeded(strstr(name,"Kat")?0x4b4154u:0x53434fu,TRADERS,PORTRAIT_HUMAN),TRADERS);return;}
 static const OpeningBitmap *portraits[]={&opening_portrait0_bitmap,&opening_portrait1_bitmap,&opening_portrait2_bitmap,&opening_portrait3_bitmap,&opening_portrait4_bitmap,&opening_portrait5_bitmap,&opening_portrait6_bitmap,&opening_portrait7_bitmap};
 if(id==6){ship_computer_portrait(x,y,size,size);return;}
 opening_bitmap_draw(portraits[id],x,y,size,size);
}
static void opening_scene_title(const char *place,const char *detail){
 rect(0,0,W,24,RGB(10,21,32));rect(0,23,W,1,RGB(216,166,82));
 text_px(12,8,UI_GOLD,"%s",place);if(detail)text_px(320,8,UI_CYAN,"%s",detail);
}
static void opening_scene_draw(int kind){
 const OpeningBitmap *b=kind==0?&opening_bedroom_bitmap:kind==1?&opening_city_bitmap:kind==2?&opening_dealer_bitmap:kind==3?&opening_field_bitmap:kind==5?&opening_cockpit_bitmap:&opening_orbit_bitmap;
 opening_bitmap_draw(b,0,0,W,H);
 opening_scene_title(kind==0?"HOME / THE BIG DAY":kind==1?"EARTH / THE SHIP DISTRICT":kind==2?"SOL SHIP CO. / SHOWROOM":kind==3?"EARTH / ALPINE WATERFALL":kind==5?"CABIN / SIGNAL LOST":"SOL / HOMEWARD",kind==5?"UNKNOWN":"EARTH 2050");
}
static void opening_showroom_mesh(int ship){
 int id=mesh_id(player_ships[ship].name);Vec3 c;float scale;ship_preview_layout(id,&c,&scale);
 float yaw=.55f+preview_time*.22f,roll=.05f*sinf(preview_time*.35f);scale*=2.05f;
 preview_clip(242,114,108,32,371,184);
 Vec3 p=sub((Vec3){0,0,320},mul(rotate(c,yaw,roll),scale));
 shipmesh(id,p,yaw,roll,scale,ship_paint[ship],1);flush_meshes();
 shipmesh_preview_edges(id,p,yaw,roll,scale,RGB(185,203,205));preview_reset();
}
static void opening_showroom_illustration(int ship){
 static const OpeningBitmap *ships[]={&opening_ship0_bitmap,&opening_ship1_bitmap,&opening_ship2_bitmap};
 opening_bitmap_draw(ships[ship%3],133,48,220,126);
}
static void opening_alien_contacts(void){
 int id=mesh_id("THARGOID");Vec3 c;float size;ship_preview_layout(id,&c,&size);
 for(int i=0;i<3;i++){
  float yaw=.4f+sinf(preview_time*.25f+i)*.08f,roll=.15f+sinf(preview_time*.3f+i)*.06f,scale=size*(i==1?1.5f:.75f);
  preview_clip(120+i*120,i==1?83:114,0,24,W,184);
  Vec3 p=sub((Vec3){0,0,320},mul(rotate(c,yaw,roll),scale));
  shipmesh(id,p,yaw,roll,scale,RGB(91,210,124),1);flush_meshes();preview_reset();
 }
 opening_scene_title("UNIDENTIFIED CONTACTS / SOL","SIGNAL FAULT");
}
static void opening_burst_overlay(float t){
 if(t<=0)return;
 int level=(int)(fminf(1,t<1.2f?t/1.2f:(t-1.2f)/1.3f)*256);unsigned target=t<1.2f?RGB(208,243,232):RGB(2,5,12);
 for(int y=24;y<H;y++)for(int x=0;x<W;x++){
  unsigned a=fb[y*STRIDE+x];int r=((a&255)*(256-level)+(target&255)*level)>>8;
  int g=(((a>>8)&255)*(256-level)+((target>>8)&255)*level)>>8;
  int b=(((a>>16)&255)*(256-level)+((target>>16)&255)*level)>>8;fb[y*STRIDE+x]=RGB(r,g,b);
 }
}
static void opening_dealer_card(int ship){
 static const char *roles[]={"THE STEADY START","QUICK AND NIMBLE","ROOM TO GROW"};
 static const char *copy[]={"A forgiving little ship. Easy to park, cheap to keep. Dad's sensible choice.","Quick off the mark. Light on cargo space; made for pilots who like to move.","A bigger hold and a longer range. Good for a first trading or exploring job."};
 rect(8,36,130,44,RGB(10,24,35));rect(8,36,2,44,UI_GOLD);text_px(16,43,UI_GOLD,"%s",player_ships[ship].name);text_px(16,59,UI_CYAN,"%02d / 03",ship+1);
 rect(346,88,126,78,RGB(10,24,35));rect(346,88,126,1,UI_GOLD);
 text_px(354,98,UI_CYAN,"HOLD   %3dt",player_ships[ship].capacity);
 text_px(354,118,WHITE,"SPEED  %3d",(int)player_ships[ship].speed);
 text_px(354,138,WHITE,"RANGE  %3d",(int)player_ships[ship].range);
 rect(0,184,W,88,RGB(9,20,31));rect(0,184,W,2,UI_GOLD);
 text_px(16,194,UI_GOLD,"%s",roles[ship]);text_wrap(2,27,55,3,WHITE,copy[ship],0);
 text_px(12,253,UI_CYAN,"L/R BROWSE  SQUARE HULL  X CHOOSE");text_px(368,253,UI_GOLD,"DAD'S TREAT");
}

/* Live Earth sky/terrain is rendered by earth-meadow-scene.h. */
