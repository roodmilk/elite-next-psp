/* Small branded lettering; stable baseline wobble, no flashing or extra assets. */
static void decorator_menu_label(int x,int y){
 const char *label="PIMP-MY-SHIP.NET";
 for(int i=0;label[i];i++){
  unsigned ink=high_contrast?UI_TEXT:i<4?RGB(116,225,235):i<12?RGB(246,165,212):RGB(250,210,124);
  text_px(x+i*8,y+(!high_contrast&&i%3==1?1:0),ink,"%c",label[i]);
 }
}
#include "command-deck-console.h"
static void home(void){dc_home();}
#include "controls-handbook.h"
static void decorator_screen(void){
 int finish=row<0?0:row>=DECORATOR_COUNT?DECORATOR_COUNT-1:row,first=finish/DECORATOR_PAGE*DECORATOR_PAGE;
 decorator_monitor_draw();
 /* The artwork contains its own screen frame, browser bar, icons and ad.
  * Dynamic UI is kept within the two intentionally empty screen windows. */
 text_px(118,29,RGB(210,235,229),"PIMP-MY-SHIP.NET");
 text_px(115,93,UI_SIGNAL,"PAINT FINISHES");
 for(int i=first;i<first+DECORATOR_PAGE;i++){
  int y=(13+i-first)*8;
  if(i==finish){rect(111,y-1,130,10,UI_RAISED);rect(111,y-1,2,10,UI_ACCENT);}
  if(ship_paint[game.ship]==decorator_finishes[i]){rect(114,y-1,12,10,UI_ACCENT);rect(116,y+1,8,6,decorator_finishes[i]);pixel(109,y+2,UI_ACCENT);pixel(108,y+3,UI_ACCENT);pixel(109,y+4,UI_ACCENT);}
  else rect(116,y+1,8,6,decorator_finishes[i]);
  text(16,13+i-first,i==finish?WHITE:RGB(171,194,198),"%s",decorator_names[i]);
 }
 preview_clip(329,116,252,82,408,155);
 {int art=mega_hull_art_id(player_ships[game.ship].name);float yaw=preview_time*.5f+.55f,roll=preview_time*.22f;if(art>=0)mega_hull_preview(art,yaw,roll,decorator_finishes[finish]);else{int pm=mesh_id(player_ships[game.ship].name);Vec3 pc;float ps;ship_preview_layout(pm,&pc,&ps);ps*=1.8f;Vec3 pp=sub((Vec3){0,0,320},mul(rotate(pc,yaw,roll),ps));shipmesh(pm,pp,yaw,roll,ps,decorator_finishes[finish],1);shipmesh_preview_edges(pm,pp,yaw,roll,ps,RGB(174,196,193));}flush_meshes();preview_reset();}
 text_px(115,179,RGB(171,194,198),"FINISH %02d / %02d",finish+1,DECORATOR_COUNT);
 for(int i=first;i<first+DECORATOR_PAGE;i++){int x=249+(i-first)*20;rect(x,171,17,10,decorator_finishes[i]);if(i==finish){rect(x,168,17,2,WHITE);rect(x,182,17,2,WHITE);}}
 rect(247,194,166,27,RGB(18,31,44));rect(247,194,166,1,UI_SIGNAL);
 text(32,25,WHITE,"%.19s",player_ships[game.ship].name);
 text(32,26,ship_paint[game.ship]==decorator_finishes[finish]?UI_CYAN:UI_GOLD,
      ship_paint[game.ship]==decorator_finishes[finish]?"EQUIPPED":"%d U TO PAINT",decorator_fees[finish]);
 if(decorator_feedback&&game.message_time>0&&game.message[0]){rect(108,200,132,21,RGB(18,31,44));text_wrap(14,25,16,2,UI_ACCENT,game.message,0);}
 rect(84,240,226,17,RGB(39,50,59));text_px(100,244,WHITE,game.docked?"UP/DOWN X PAINT O BACK":"DOCK TO PAINT O BACK");
 rect(318,240,116,17,RGB(39,50,59));text_px(330,243,UI_GOLD,"%.1f U",game.credits*.1f);
}
/* Action feedback gets its own reserved band, not a talking-character card. */
static void menu_notice(void){
 if(page==HOME)return; /* The deck owns its bounded right-hand notice area. */
 if(page==LOCALTV)return; /* Broadcast owns its caption and lower-third bands. */
 if(page==INVENTORY)return; /* Loadout owns one reserved feedback line. */
 if(page==WALK&&walk_kind==0&&game.docked)return; /* Station owns its dialogue/feedback band. */
 if(page==DETAILS)return; /* Almanac owns the full illustrated spread. */
 if(game.message_time<=0||!game.message[0])return;
 int narrative=page==CAMPAIGN||page==GUILD||page==STORY;
 int y=page==HOME?216:narrative?224:192;int h=narrative?24:32;
 /* Charcoal plate + ochre rail — action feedback, not a gold debug card. */
 rect(8,y,464,h,UI_PANEL);rect(8,y,464,1,UI_EDGE);rect(8,y,3,h,UI_EDGE);
 text_wrap(2,(y+2)/8,56,narrative?2:3,UI_TEXT,game.message,0);
}
