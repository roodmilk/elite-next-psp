/* One mapping table makes the four assignments cheap to revise. */
static const unsigned tool_buttons[]={PSP_CTRL_UP,PSP_CTRL_DOWN,PSP_CTRL_LEFT,PSP_CTRL_RIGHT};
static const char *tool_names[]={"MISSILES","FLARES","TRACTOR","HEAT SINK","LAW SCAN"};
static int tools_selected=0,tools_open=0,tools_armed=0,tools_consumed=0,tools_chosen=0;
static int tools_wait_release=0;
static int tools_tractor=-1;static float tools_tractor_timeout=0;
static float tools_confirm_time=0;static int tools_confirm_action=-1,tools_weapon_result=0;
static char tools_weapon_old[24],tools_weapon_new[24];
static void tools_cancel_tractor(void){if(tools_tractor>=0)autoaim=0;tools_tractor=-1;tools_tractor_timeout=0;}
static float tools_hold=0;static unsigned tools_dirs=0;
static void tools_reset_gesture(void){tools_open=tools_armed=tools_consumed=tools_chosen=0;tools_hold=0;tools_dirs=0;}
static void tools_block_until_release(void){tools_reset_gesture();tools_cancel_tractor();tools_wait_release=1;}
static int tools_context(void){return page==FLIGHT&&!game.docked&&!game.dead&&game.planet<0&&game.jump<=0&&!game.dock_stage&&game.approach<0&&!game.police_stop&&!paused&&!comms_quick;}
static int tools_recoverable(int id){
 if(!IS_DEBRIS_ID(id))return 0;
 Debris *d=&game.debris[id-DEBRIS_ID_MIN];
 return d->alive&&!d->rock&&length(sub(d->pos,game.pos))<=tech_tractor_range(&game)&&!occluded(d->pos);
}
static void tools_no_cargo(void){
 static const char *line[]={
  "Um, there's no cargo nearby! At least... I can't see any!",
  "Nothing loose within the fitted tractor range. Fly closer and try again.",
  "Cargo bay is ready, but I can't see anything recoverable nearby.",
  "No salvage in reach. Move inside the fitted tractor range.",
  "I checked twice. No loose cargo close enough to recover.",
  "Nothing to pull in, Commander. Try again inside the fitted tractor range.",
  "That was optimistic. There is no cargo on the local scope.",
  "No canisters nearby. Fly closer - check the fitted tractor range."
 };
 unsigned pick=(unsigned)(game.time*17.f)+game.rng+(unsigned)game.system*13u;
 message(&game,line[pick%(sizeof(line)/sizeof(line[0]))]);
}
static void tools_start_tractor(void){
 if(game.tractor_time>0||tools_tractor>=0){message(&game,"Tractor already engaged.");return;}
 int id=selected_target;
 if(!tools_recoverable(id)){
  id=-1;float best=tech_tractor_range(&game)+1;
  for(int i=0;i<DEBRIS_COUNT;i++)if(tools_recoverable(DEBRIS_ID_MIN+i)){
   float distance=length(sub(game.debris[i].pos,game.pos));
   if(distance<best){best=distance;id=DEBRIS_ID_MIN+i;}
  }
 }
 if(id<0){tools_no_cargo();return;}
 selected_target=id;autoaim=1;tools_tractor=id;tools_tractor_timeout=5;
 game.speed=0;game.boost=0;message(&game,"Tractor aligning. Manual steering cancels.");
}
static void tools_tractor_tick(float dt){
 if(tools_tractor<0)return;
 tools_tractor_timeout-=dt;
 if(!tools_context()||!autoaim||selected_target!=tools_tractor||!tools_recoverable(tools_tractor)||tools_tractor_timeout<=0){
  tools_cancel_tractor();message(&game,"Tractor alignment cancelled.");return;
 }
 game.speed=0;game.boost=0;
 Vec3 delta=sub(game.debris[tools_tractor-DEBRIS_ID_MIN].pos,game.pos);
 if(length(delta)<1||dot(forward(&game),norm(delta))>=.998f){
  int id=tools_tractor;tools_cancel_tractor();salvage(&game,id);
 }
}
static void tools_activate(int action){
 if(action==0)fire_missile(&game,selected_target);else if(action==1)deploy_flare(&game);else if(action==2)tools_start_tractor();else if(action==4)law_scan(&game);else dump_heat_sink(&game);
}
#include "navigation-flight.h"
#include "cockpit-computer-controls.h"
static int tools_input(unsigned pressed,unsigned held,float dt){
 tools_confirm_time=fmaxf(0,tools_confirm_time-fmaxf(0,dt));
 if(!tools_context()){tools_reset_gesture();tools_confirm_time=0;return 0;}
 if(held&PSP_CTRL_START){tools_reset_gesture();tools_wait_release=1;return 0;}
 if(tools_wait_release){tools_reset_gesture();if(!(held&PSP_CTRL_CIRCLE))tools_wait_release=0;return (held&PSP_CTRL_CIRCLE)!=0;}
 if(held&PSP_CTRL_CIRCLE){
  if(pressed&PSP_CTRL_CIRCLE){tools_reset_gesture();tools_armed=1;}
  if(!tools_armed)return 1;
  tools_hold+=fmaxf(0,dt);unsigned action=pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_LTRIGGER|PSP_CTRL_TRIANGLE);
  if(action){
   tools_consumed=1;tools_chosen=1;tools_open=1;tools_confirm_time=.36f;tools_weapon_result=0;
   if(action&PSP_CTRL_RIGHT){
    int old=fit_weapon_item(&game);snprintf(tools_weapon_old,sizeof(tools_weapon_old),"%s",fit_weapon_short_name(old));
    int result=fit_cycle_weapon(&game);snprintf(tools_weapon_new,sizeof(tools_weapon_new),"%s",fit_weapon_short_name(fit_weapon_item(&game)));
    tools_confirm_action=3;tools_weapon_result=result>=0?1:result==-2?2:-1;
   }else{
    int chosen=action&PSP_CTRL_LTRIGGER?0:action&PSP_CTRL_TRIANGLE?1:action&PSP_CTRL_LEFT?2:action&PSP_CTRL_DOWN?3:4;
    tools_selected=chosen;tools_confirm_action=chosen;
   }
   game.cue=SFX_SELECT;
  }
  if(tools_hold>=.20f){tools_consumed=1;tools_open=1;}
  return 1;
 }
 if(tools_armed){int fire=!tools_consumed&&tools_hold<.20f;tools_reset_gesture();if(fire)tools_activate(tools_selected);return 1;}
 tools_open=0;return 0;
}
static int tools_flash_twice(void){if(tools_confirm_time<=0)return 0;int phase=(int)((.64f-fminf(.64f,tools_confirm_time))/.16f);return phase==0||phase==2;}
static int tools_direction_flash(void){
 if(tools_confirm_time<=0||tools_confirm_action<0||tools_confirm_action>3)return 0;
 float total=tools_confirm_action==3&&tools_weapon_result>0?1.20f:.64f;
 int phase=(int)((total-fminf(total,tools_confirm_time))/.16f);
 return phase==0||phase==2;
}
static void tools_direction_box(int x,int y,int w,int h,int active){
 /* Confirm the physical direction, rather than recolouring only its caption. */
 unsigned edge=active?RED:RGB(74,52,46),fill=active?RGB(72,18,26):RGB(17,23,31);
 rect(x,y,w,h,edge);rect(x+2,y+2,w-4,h-4,fill);
 if(active){line(x+4,y+3,x+w-5,y+3,RGB(255,112,104));line(x+3,y+4,x+3,y+h-5,RGB(255,112,104));}
}
static void tools_ticker_clipped(int x,int y,int left,int right,unsigned ink,const char *s){
 for(int i=0;s[i];i++){int px=x+i*6;if(px>=left&&px+5<right)text_px(px,y,ink,"%c",s[i]);}
}
#include "cockpit-computer-art.h"
static void tools_flare_effect(void){
 if(game.flare_fx<=0||game.planet>=0)return;
 Vec3 v=camera(&game,game.flare_pos);if(v.z<15)return;Point p=project(v);
 int x=(int)p.x,y=(int)p.y;if(x<12||x>W-12||y<view_top()+12||y>view_bot()-12)return;
 int r=(int)fmaxf(2,fminf(9,800/v.z));circle(x,y,r,RGB(255,180,75));circle(x,y,r/2,WHITE);
}
