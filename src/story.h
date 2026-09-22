/* Guild Handler Kei's campaign. Menus unlock as the commander learns the ship. */
#include <stdio.h>
enum {
 STORY_BRIEF=0, STORY_LAUNCH, STORY_SIGHT, STORY_RETURN, STORY_LOCAL, STORY_NET,
 STORY_WORK, STORY_HOLD, STORY_MAP, STORY_POWER, STORY_WORLD, STORY_ATLAS, STORY_FREE
};
enum { STORY_EV_HELP=1, STORY_EV_LAUNCH=2, STORY_EV_TARGET=4, STORY_EV_DOCK=8, STORY_EV_PAGE=16, STORY_EV_WORLD=32, STORY_EV_SCAN=64, STORY_EV_PIP=128, STORY_EV_LANDING_TECH=256 };
static void story_complete(Game *g){g->story=STORY_FREE;g->story_flags|=0xffff;}
static int story_menu_ok(const Game *g,int home_index){(void)g;(void)home_index;return 1;}
static inline const char *story_title(const Game *g){
 static const char *t[]={"HER CHANNEL","FIRST FLIGHT","EYES OPEN","BERTH","THE ROOM","LAST PING","PAID WORK","THE HOLD","A STAR","THREE EDGES","HER WORLD","FIRST LIGHT","STILL HERE"};
 int s=g->story;if(s<0)s=0;if(s>STORY_FREE)s=STORY_FREE;return t[s];
}
static inline const char *story_line(const Game *g,int line){
 static const char *copy[][4]={
  {"Kei. Ryn was my surveyor, and my friend, until her last ping went quiet.","This ship is yours while we look. Open CONTROLS and learn it properly.","I will not lose anyone else to a silence I could have challenged sooner.","Stay on this channel — panic makes me over-explain, and today I am scared."},
  {"Your ship is ready at the hub. Hit Launch when your hands mean it.","D-pad steers; hold R for speed and L to slow without turning courage into wreckage.","I need you on this channel for the return, not only the departure.","Clear the pad cleanly. Venn has buried enough clever pilots already."},
  {"Do not fly blind. Ryn did that once, and the silence afterward was worse than the mistake.","Tap Square for the target list; hold Square with Left or Right to tab without turning.","Find her ping if it still answers, then come home before you invent a second silence.","Eyes open is not optional out here — it is how people keep their names on the board."},
  {"Bring the ship in as a living hull, not a ghost story for the tower to file.","Face the hub and press O to dock, or open Comms and let Venn guide a boring approach.","He kept Ryn's berth warm through procedure, which is how he survives grief.","A clean dock is the first promise you make to everyone still waiting on the pad."},
  {"Learn the sky she used to fly — open Details or Factions and see who shares Lave with you.","Someone sold her route as if a chart could be private weather.","Names and uniforms matter here; do not let a seed choose your friends for you.","Understanding the room is how you notice when the room starts lying."},
  {"This is where the rumours live. I posted the last thing she sent like a letter, not a trophy.","A Meridian echo sits on the wire. Read it carefully before you decide what it means.","She wanted someone to look, not someone to own the looking.","GalacticNet is noisy; her silence is still the loudest line on it."},
  {"Money will not find her. Work will keep you flying long enough to keep looking.","Open Missions and take one job you can finish without turning the hold into a shrine.","Read the brief first. Job clocks pause in menus so reading is not a trap.","Paid work is how independent pilots stay available when the next call goes wrong."},
  {"A hold is a promise to come back with what you said you would carry.","Open Cargo. I packed you food because hunger makes heroes into statistics.","Eat. Stay clean if you can — Dockhand_77 still teases a dirty bay like it is a moral failing.","What you carry tells people whether your word survives a jump."},
  {"The map is a promise to a star, not a dare written in empty fuel.","Open Galaxy Map and pick one destination you can explain to yourself.","You do not have to jump yet. Her echo is not waiting politely in Lave.","Plan the road before the road plans you."},
  {"Power is how you get home when the sky stops being generous.","Start pauses the three banks — SYS, ENG and WEP — the same lesson I taught Ryn.","Outfitting sells the rest; judgement decides whether you needed it.","Balance the pips like someone who intends to dock again."},
  {"She loved dirt more than orbit, which is why the quiet worlds still feel like hers.","Venn found Ryn's atmospheric landing kit. It is yours now — use it carefully.","Tab to Planets, press O to approach, then X to enter when you mean it.","Bring back what you find without pretending the ground owes you a story."},
  {"The Codex is how we keep her — not as a relic, but as a record that can be checked.","O on an echo; Square on foot; then open Codex and log first light properly.","If it is hers, I will know. If it is not, we still owed the sky an honest entry.","Evidence without ownership. That is the Guild lesson I am still learning."},
  {"Every door is yours now. Ryn is still out there, and so am I.","I am still on this channel when you need a voice that talks too much while caring.","Fly like you mean to come home — home is rarer than jump fuel.","The Open Channel stays open. Use it."}
 };
 int s=g->story;if(s<0)s=0;if(s>STORY_FREE)s=STORY_FREE;if(line<0||line>3)return "";
 return copy[s][line];
}
static const char *story_radio(int stage){
 static const char *r[]={
  "Kei: Open Controls. I am right here on the channel.",
  "Kei: Launch when you are ready — and mean the return.",
  "Kei: Tap Square. Do not fly blind the way she once did.",
  "Kei: Press O to dock, or let Venn bring you in boring and alive.",
  "Kei: Open Details. Learn the sky she used to share with you.",
  "Kei: Her last ping is still on the wire. Read it like a letter.",
  "Kei: Take a job. Stay flying long enough to keep looking.",
  "Kei: I packed you food. Eat before heroics invent themselves.",
  "Kei: Open the map. Pick a star you can explain to yourself.",
  "Kei: Hit Start. Three power banks — the lesson that gets you home.",
  "Kei: Walk her world. The pad is cyan for a reason.",
  "Kei: Log the echo. That is how we keep her without owning her.",
  "Kei: You made it. I am still here."
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
  if(g->surface==2)return "D-pad moves. Hold R lifts. Triangle faces ship.";
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
 if(to==STORY_WORLD){g->story_flags|=STORY_EV_LANDING_TECH;speak(g,VOICE_KEI,"Venn found Ryn's atmospheric landing kit in the locker. It is yours now. Take the ground seriously.");message(g,"Landing kit received. Planetary landing is now available.");}
 else if(to==STORY_FREE){g->credits+=2500;speak(g,VOICE_KEI,"You made it. I am still here.");message(g,"Ryn's channel stays open.");}
 else speak(g,VOICE_KEI,story_radio(to)+5);
 g->cue=SFX_COMM;
}
static inline int story_landing_ready(const Game *g){return g->story>=STORY_WORLD||(g->story_flags&STORY_EV_LANDING_TECH);}
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
static void story_skip(Game *g){g->story=STORY_FREE;g->story_flags|=0xffff;speak(g,VOICE_KEI,"I will still be here. Go fly — and mean the return.");message(g,"Flight coaching skipped. Guild jobs remain.");g->cue=SFX_COMM;}
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
