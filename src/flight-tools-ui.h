/* One mapping table makes the four assignments cheap to revise. */
static const unsigned tool_buttons[]={PSP_CTRL_UP,PSP_CTRL_DOWN,PSP_CTRL_LEFT,PSP_CTRL_RIGHT};
static const char *tool_names[]={"MISSILES","FLARES","TRACTOR","HEAT SINK","LAW SCAN"};
static int tools_selected=0,tools_open=0,tools_armed=0,tools_consumed=0,tools_chosen=0;
static int tools_wait_release=0;
static int tools_tractor=-1;static float tools_tractor_timeout=0;
static void tools_cancel_tractor(void){if(tools_tractor>=0)autoaim=0;tools_tractor=-1;tools_tractor_timeout=0;}
static float tools_hold=0;static unsigned tools_dirs=0;
static void tools_reset_gesture(void){tools_open=tools_armed=tools_consumed=tools_chosen=0;tools_hold=0;tools_dirs=0;}
static void tools_block_until_release(void){tools_reset_gesture();tools_cancel_tractor();tools_wait_release=1;}
static int tools_context(void){return page==FLIGHT&&!game.docked&&!game.dead&&game.planet<0&&game.jump<=0&&!game.dock_stage&&game.approach<0&&!game.police_stop&&!paused&&!comms_quick;}
static int tools_recoverable(int id){
 if(!IS_DEBRIS_ID(id))return 0;
 Debris *d=&game.debris[id-DEBRIS_ID_MIN];
 return d->alive&&!d->rock&&length(sub(d->pos,game.pos))<=500&&!occluded(d->pos);
}
static void tools_start_tractor(void){
 if(game.tractor_time>0||tools_tractor>=0){message(&game,"Tractor already engaged.");return;}
 int id=selected_target;
 if(!tools_recoverable(id)){
  id=-1;float best=501;
  for(int i=0;i<DEBRIS_COUNT;i++)if(tools_recoverable(DEBRIS_ID_MIN+i)){
   float distance=length(sub(game.debris[i].pos,game.pos));
   if(distance<best){best=distance;id=DEBRIS_ID_MIN+i;}
  }
 }
 if(id<0){message(&game,"No recoverable cargo within 500 m.");return;}
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
static int tools_input(unsigned pressed,unsigned held,float dt){
 if(!tools_context()){tools_reset_gesture();return 0;}
 if(tools_wait_release){tools_reset_gesture();if(!(held&PSP_CTRL_CIRCLE))tools_wait_release=0;return (held&PSP_CTRL_CIRCLE)!=0;}
 unsigned directions=held&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT);
 if(held&PSP_CTRL_CIRCLE){
  if(pressed&PSP_CTRL_CIRCLE){tools_reset_gesture();tools_armed=1;}
  if(!tools_armed)return 1; /* A held key from a menu cannot become a fresh shot. */
  tools_hold+=fmaxf(0,dt);
  if(!tools_chosen&&(pressed&PSP_CTRL_RTRIGGER)){
   tools_consumed=1;tools_chosen=1;
   if(fit_find(&game,LAW_SCANNER_ITEM)>=0){tools_selected=4;message(&game,"Law Scanner selected. Tap Circle to scan.");}
   else message(&game,"Fit a Law Scanner in a UTIL slot first.");
  }
  if(!tools_chosen&&(pressed&PSP_CTRL_TRIANGLE)){
   tools_selected=3;tools_chosen=1;tools_consumed=1;message(&game,"HEAT SINK selected.");
  }
  unsigned fresh=pressed&directions&~tools_dirs;tools_dirs=directions;
  if(directions)tools_consumed=1;
  if(!tools_chosen&&fresh&&!(directions&(directions-1))){
   for(int action=0;action<4;action++)if(fresh&tool_buttons[action]){
    tools_chosen=1;tools_consumed=1;
    if(action==3){
     int slot=fit_cycle_weapon(&game);char note[80];
     if(slot>=0)snprintf(note,sizeof(note),"Weapon bank %d armed: %s.",slot/6+1,fit_weapon_short_name(game.fit[slot]));
     else if(slot==-2)snprintf(note,sizeof(note),"Only one WPN module fitted.");
     else snprintf(note,sizeof(note),"No WPN module fitted.");
     message(&game,note);
    }else{
     tools_selected=action;char note[64];snprintf(note,sizeof(note),"%s selected.",tool_names[action]);message(&game,note);
    }
   }
  }
  if(tools_hold>=.20f)tools_consumed=1;
  tools_open=tools_hold>=.20f&&!tools_chosen;
  return 1;
 }
 if(tools_armed){
  int fire=!tools_consumed&&tools_hold<.20f&&!directions;
  tools_reset_gesture();if(fire)tools_activate(tools_selected);return 1;
 }
 tools_open=0;return 0;
}
static void tools_panel(void){
 if(!tools_context()||!tools_open)return;
 /* Compact ship-tools cross: weapons and recovery share one selector. */
 unsigned edge=RGB(177,92,58),ink=RGB(245,190,123),dim=RGB(157,167,178);
 rect(104,48,272,120,RGB(12,18,25));rect(104,48,272,1,edge);rect(104,167,272,1,edge);
 line(104,48,112,48,ink);line(104,48,104,56,ink);line(375,159,375,167,ink);
 rect(227,89,26,38,UI_RAISED);rect(216,100,48,16,UI_RAISED);
 button_icon(235,102,'O',ink);
 button_icon(235,80,'U',ink);button_icon(235,132,'D',ink);
 button_icon(210,104,'L',ink);button_icon(260,104,'R',ink);
 text_px(196,58,ink,"MISSILES %d",game.missiles);
 text_px(196,150,ink,"FLARES %d/%d",game.flare_charges,flare_capacity(&game));
 text_px(116,94,ink,"TRACTOR");text_px(284,90,ink,"NEXT WPN");
 text_px(116,110,dim,game.tractor_time>0?"PULLING":tools_tractor>=0?"ALIGNING":"500 M");
 {int wc=fit_weapon_count(&game),wi=fit_weapon_item(&game);text_px(284,106,dim,"B%d/%d %.7s",game.active_weapon/6+1,wc,fit_weapon_short_name(wi));}
 if(game.flare_cd>0)text_px(112,150,dim,"%ds",(int)ceilf(game.flare_cd));
 button_icon(284,126,'T',ink);text_px(302,130,ink,"HEAT SINK");
 if(fit_find(&game,LAW_SCANNER_ITEM)>=0){button_icon(284,145,'r',ink);text_px(302,150,ink,"LAW SCAN");}
}
static void tools_flare_effect(void){
 if(game.flare_fx<=0||game.planet>=0)return;
 Vec3 v=camera(&game,game.flare_pos);if(v.z<15)return;Point p=project(v);
 int x=(int)p.x,y=(int)p.y;if(x<12||x>W-12||y<view_top()+12||y>view_bot()-12)return;
 int r=(int)fmaxf(2,fminf(9,800/v.z));circle(x,y,r,RGB(255,180,75));circle(x,y,r/2,WHITE);
}
