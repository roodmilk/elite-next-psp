/* Guild Handler Kei's campaign. Menus unlock as the commander learns the ship. */
#include <stdio.h>
enum {
 STORY_BRIEF=0, STORY_LAUNCH, STORY_SIGHT, STORY_RETURN, STORY_LOCAL, STORY_NET,
 STORY_WORK, STORY_HOLD, STORY_MAP, STORY_POWER, STORY_WORLD, STORY_ATLAS, STORY_FREE
};
enum { STORY_EV_HELP=1, STORY_EV_LAUNCH=2, STORY_EV_TARGET=4, STORY_EV_DOCK=8, STORY_EV_PAGE=16, STORY_EV_WORLD=32, STORY_EV_SCAN=64, STORY_EV_PIP=128 };
static void story_complete(Game *g){g->story=STORY_FREE;g->story_flags|=0xffff;}
static int story_menu_ok(const Game *g,int home_index){(void)g;(void)home_index;return 1;}
static inline const char *story_title(const Game *g){
 static const char *t[]={"HER CHANNEL","FIRST FLIGHT","EYES OPEN","BERTH","THE ROOM","LAST PING","PAID WORK","THE HOLD","A STAR","THREE EDGES","HER WORLD","FIRST LIGHT","STILL HERE"};
 int s=g->story;if(s<0)s=0;if(s>STORY_FREE)s=STORY_FREE;return t[s];
}
static inline const char *story_line(const Game *g,int line){
 static const char *copy[][4]={
  {"Kei. Ryn was my surveyor. My friend.","Then her last ping went quiet.","This ship's yours. Open CONTROLS.","I won't lose anyone else."},
  {"Your ship is ready at the hub.","Hit Launch. D-pad steers you.","Hold R for speed. L to slow.","I need you on this channel."},
  {"Don't fly blind. Ryn did, once.","Tap Square for the target list.","Hold Square, Left or Right to tab.","Find her ping, then come home."},
  {"Bring the ship in. Not a ghost.","Face the hub and press O to dock.","Or open Comms. Venn will guide you.","He kept Ryn's berth warm."},
  {"Learn the sky she used to fly.","Open Details, or Factions.","See who shares Lave with you.","Someone sold her route."},
  {"This is where the rumours live.","I posted the last thing she sent.","A Meridian echo. Read it like a letter.","She wanted someone to look."},
  {"Money won't find her.","Work will keep you flying.","Open Missions. Take one job.","Read first. Job clocks pause in menus."},
  {"A hold is a promise to come back.","Open Cargo. I packed you food.","Eat. Stay clean if you can.","Dockhand_77 still teases a dirty bay."},
  {"The map is a promise to a star.","Open Galaxy Map. Pick one.","You don't have to jump yet.","Her echo isn't in Lave."},
  {"Power is how you get home.","Start pauses SYS, ENG and WEP.","I taught Ryn those three banks.","Outfitting sells the rest."},
  {"She loved dirt more than orbit.","Tab to Planets, then press O.","O to approach, X to go in.","Land on the cyan cross, then walk."},
  {"The Codex is how we keep her.","O on an echo. Square on foot.","Then open Codex. Log first light.","If it's hers, I'll know."},
  {"Every door is yours now.","Ryn's still out there. So am I.","I'm still on this channel.","Fly like you mean to come home."}
 };
 int s=g->story;if(s<0)s=0;if(s>STORY_FREE)s=STORY_FREE;if(line<0||line>3)return "";
 return copy[s][line];
}
static const char *story_radio(int stage){
 static const char *r[]={
  "Kei: Open Controls. I'm right here.",
  "Kei: Launch when you're ready.",
  "Kei: Tap Square. Don't fly blind.",
  "Kei: Press O. Venn will bring you in.",
  "Kei: Open Details. Learn her sky.",
  "Kei: Her last ping is on the wire.",
  "Kei: Take a job. Stay flying.",
  "Kei: I packed you food. Eat.",
  "Kei: Open the map. Pick a star.",
  "Kei: Hit Start. Three power banks.",
  "Kei: Walk her world. The pad's cyan.",
  "Kei: Log the echo. That's how we keep her.",
  "Kei: You made it. I'm still here."
 };
 if(stage<0)stage=0;
 if(stage>STORY_FREE)stage=STORY_FREE;
 return r[stage];
}
static const char *story_task(const Game *g){
 static const char *t[]={
  "Open CONTROLS, then X to launch.",
  "Launch, then fly. Hold R.",
  "Tap Square, or Square and Left.",
  "Press O to dock, or open Comms.",
  "Open Details, or Factions.",
  "Open GalacticNet. Read her ping.",
  "Open Missions. Take one job.",
  "Open Cargo. I packed you food.",
  "Open Galaxy Map. Pick a star.",
  "Open Outfitting, or hit Start.",
  "Tab Planets, then O to go in.",
  "Scan, then open the Codex.",
  "Every door is yours. Come home."
 };
 int s=g->story;if(s<0)s=0;if(s>STORY_FREE)s=STORY_FREE;return t[s];
}
static inline int story_home_row(const Game *g){
 static const int r[]={6,0,0,10,11,14,12,1,2,4,8,15,0};
 int s=g->story;if(s<0)s=0;if(s>STORY_FREE)s=STORY_FREE;return r[s];
}
static __attribute__((unused)) const char *story_hint(const Game *g){
 if(g->planet>=0){
  if(g->surface==2)return "R walks. 2xR jumps. Square scans.";
  if(g->surface==1)return "O to walk. Triangle to take off.";
  return "Land on the cyan pad, then press O.";
 }
 if(g->approach>=0)return "X to go in. O to turn back.";
 if(g->dock_stage)return "Match the cyan slot to dock.";
 return story_task(g);
}
static void story_advance(Game *g,int to){
 if(g->story>=to||to>STORY_FREE)return;
 g->story=to;
 if(to==STORY_FREE){g->credits+=2500;speak(g,VOICE_KEI,"You made it. I'm still here.");message(g,"Ryn's channel stays open.");}
 else speak(g,VOICE_KEI,story_radio(to)+5);
 g->cue=SFX_COMM;
}
static void story_try(Game *g){
 if(g->story>=STORY_FREE)return;
 if(g->story==STORY_BRIEF&&(g->story_flags&STORY_EV_HELP))story_advance(g,STORY_LAUNCH);
 if(g->story==STORY_LAUNCH&&(g->story_flags&STORY_EV_LAUNCH))story_advance(g,STORY_SIGHT);
 if(g->story==STORY_SIGHT&&(g->story_flags&STORY_EV_TARGET))story_advance(g,STORY_RETURN);
 if(g->story==STORY_RETURN&&(g->story_flags&STORY_EV_DOCK)&&g->docked)story_advance(g,STORY_LOCAL);
 if(g->story==STORY_LOCAL&&(g->story_flags&STORY_EV_PAGE))story_advance(g,STORY_NET);
 if(g->story==STORY_NET&&(g->story_flags&(STORY_EV_PAGE<<8)))story_advance(g,STORY_WORK);
 if(g->story==STORY_WORK&&(g->story_flags&(STORY_EV_PAGE<<9)))story_advance(g,STORY_HOLD);
 if(g->story==STORY_HOLD&&(g->story_flags&(STORY_EV_PAGE<<10)))story_advance(g,STORY_MAP);
 if(g->story==STORY_MAP&&(g->story_flags&(STORY_EV_PAGE<<11)))story_advance(g,STORY_POWER);
 if(g->story==STORY_POWER&&(g->story_flags&(STORY_EV_PIP| (STORY_EV_PAGE<<12))))story_advance(g,STORY_WORLD);
 if(g->story==STORY_WORLD&&(g->story_flags&STORY_EV_WORLD))story_advance(g,STORY_ATLAS);
 if(g->story==STORY_ATLAS&&(g->story_flags&(STORY_EV_SCAN|(STORY_EV_PAGE<<13))))story_advance(g,STORY_FREE);
}
static void story_event(Game *g,int ev){g->story_flags|=ev;story_try(g);}
static void story_skip(Game *g){g->story=STORY_FREE;g->story_flags|=0xffff;speak(g,VOICE_KEI,"I'll still be here. Go fly.");message(g,"Flight coaching skipped. Guild jobs remain.");g->cue=SFX_COMM;}
static void story_on_open(Game *g,int home_index){
 if(home_index==6)story_event(g,STORY_EV_HELP);
 if(home_index==11||home_index==7)story_event(g,STORY_EV_PAGE);
 if(home_index==14)g->story_flags|=(STORY_EV_PAGE<<8),story_try(g);
 if(home_index==12||home_index==13)g->story_flags|=(STORY_EV_PAGE<<9),story_try(g);
 if(home_index==1)g->story_flags|=(STORY_EV_PAGE<<10),story_try(g);
 if(home_index==2)g->story_flags|=(STORY_EV_PAGE<<11),story_try(g);
 if(home_index==4)g->story_flags|=(STORY_EV_PAGE<<12),story_try(g);
 if(home_index==15)g->story_flags|=(STORY_EV_PAGE<<13),story_try(g);
}
static int pip_shift(Game *g,int dst){
 int *p[3]={&g->pip_sys,&g->pip_eng,&g->pip_wep};
 if(dst<0||dst>2||*p[dst]>=4)return 0;
 int src=-1,best=0;
 for(int i=0;i<3;i++)if(i!=dst&&*p[i]>best){best=*p[i];src=i;}
 if(src<0||*p[src]<1)return 0;
 (*p[src])--;(*p[dst])++;
 g->story_flags|=STORY_EV_PIP;story_try(g);
 return 1;
}
static int pip_selected_move(Game *g,int sel,int dir){
 if(dir>0)return pip_shift(g,sel);
 int src=sel,dst=-1,best=5;
 int *p[3]={&g->pip_sys,&g->pip_eng,&g->pip_wep};
 if(*p[src]<1)return 0;
 for(int i=0;i<3;i++)if(i!=src&&*p[i]<best){best=*p[i];dst=i;}
 if(dst<0||*p[dst]>=4)return 0;
 (*p[src])--;(*p[dst])++;
 g->story_flags|=STORY_EV_PIP;story_try(g);
 return 1;
}
