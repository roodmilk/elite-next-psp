/* Frontend adapter: observes completed actions, never advances from button intent alone. */
static int tutorial_modal=0,tutorial_confirm=0,tutorial_prepared=0;
static float tutorial_practice=0;
static int tutorial_page_for(int id){
 static const int pages[]={FLIGHT,MARKET,CHART,YARD,EQUIP,STATUS,HELP,FACTIONS,TARGETING,DEBUG,COMMS,DETAILS,MISSIONS,MISSIONLOG,GALNET,CODEX,RADIO,CAMPAIGN,GUILD,COMFORT,WALK,INVENTORY,CODEX,DECORATOR,REPAIR};
 return id>=0&&id<25?pages[id]:HOME;
}
static void tutorial_checkpoint(void){
 if(game.docked&&!smoke&&!save_game(&game,"tutorial.sav"))message(&game,"Tutorial checkpoint failed. Save at the next berth.");
}
static void tutorial_reset_frontend(void){
 tutorial_modal=0;tutorial_confirm=0;tutorial_prepared=0;tutorial_practice=0;
 paused=0;autoaim=0;selected_target=0;look_target=-1;comms_quick=0;
 triangle_arm=0;fire_blocked=1;analog_ready=0;station_tour_stage=STATION_TOUR_OFF;
 deck_reset();sc_built_for=-1;nav_depth=0;
}
static void tutorial_start(void){
 game_init(&game);story_complete(&game);game.tutorial_step=1;game.tutorial_seen=0;
 tutorial_reset_frontend();change_page(HOME);game.voice_time=game.message_time=0;
 tutorial_checkpoint();
}
static void tutorial_prepare(void){
 const TutorialBeat *b=tutorial_beat(&game);
 if(!b||tutorial_prepared==game.tutorial_step)return;
 if(game.docked&&(b->event==TU_SALVAGE||b->event==TU_SCAN))return;
 tutorial_prepared=game.tutorial_step;tutorial_practice=0;tutorial_confirm=0;
 game.voice_time=0;game.message_time=0;
 /* Practice props are finite, local fixtures. They are recreated only on a new
  * lesson or a checkpoint reload, not every frame or every briefing replay. */
 if(b->event==TU_SALVAGE&&!game.docked){
  int i=DEBRIS_COUNT-1;Debris *d=&game.debris[i];memset(d,0,sizeof(*d));
  d->alive=1;d->good=0;d->qty=1;d->radius=12;d->health=20;d->life=600;
  d->pos=add(game.pos,mul(forward(&game),180));selected_target=DEBRIS_ID_MIN+i;
  scan_cat=3;autoaim=0;game.speed=0;
 }
 if(b->event==TU_SCAN&&!game.docked){
  int i=ANOMALY_COUNT-1;Anomaly *a=&game.anomaly[i];
  a->alive=1;a->kind=0;a->scanned=0;a->pos=add(game.pos,mul(forward(&game),300));
  selected_target=ANOMALY_ID_MIN+i;autoaim=0;game.speed=0;
 }
}
static int tutorial_emit(int event,int arg){
 if(!tutorial_event(&game,event,arg))return 0;
 /* One-time allowance is coupled to a completed first docking transition. */
 if(game.tutorial_step==12)game.credits+=5000;
 tutorial_prepared=0;tutorial_practice=0;
 game.boost=0;game.voice_time=0;
 if(!tutorial_active(&game)){
  game.campaign_stage=6;game.campaign_flags|=CP_LOCKED|CP_FLEW|CP_GUIDED;game.campaign_distance=600;
  story_complete(&game);message(&game,"First Light complete. Every service is available.");
  change_page(HOME);
 }
 tutorial_checkpoint();return 1;
}
static void tutorial_resume(void){
 if(load_game(&game,"tutorial.sav")&&game.tutorial_step>0){
  tutorial_reset_frontend();game.tutorial_seen=0;change_page(HOME);
 }else message(&game,"No tutorial checkpoint found. Choose START TUTORIAL.");
}
static void tutorial_draw(void){
 if(!tutorial_active(&game))return;
 const TutorialBeat *b=tutorial_beat(&game);
 if(tutorial_modal||game.tutorial_seen!=game.tutorial_step){
  rect(0,0,W,H,BG);header("FIRST LIGHT / FLIGHT LICENCE");
  text(3,5,CYAN,"LESSON %02d / %02d",game.tutorial_step,TUTORIAL_COUNT);
  rect(24,54,352,3,RGB(30,48,58));rect(24,54,352*(game.tutorial_step-1)/TUTORIAL_COUNT,3,GOLD);
  if(!strcmp(b->speaker,"KEI"))draw_kei(400,28,48,0);else draw_portrait(400,28,48,48,VOICE_VENN*37,TRADERS);
  text(3,8,GOLD,"%s",b->title);text(3,10,CYAN,"%s",b->speaker);
  text_wrap(3,12,53,9,WHITE,b->copy,0);
  rect(16,181,448,49,RGB(21,38,49));text(3,23,GOLD,"YOUR NEXT STEP");
  text_wrap(3,25,53,3,WHITE,b->task,0);
  if(tutorial_confirm==1)footer("END TRAINING? X YES   O CANCEL");
  else if(tutorial_confirm==2)footer("RELOAD CHECKPOINT? X YES   O CANCEL");
  else footer("X CONTINUE   SQUARE CHECKPOINT   TRI END");
 }else if(page==HOME){
  rect(8,216,464,38,RGB(16,31,43));
  text_wrap(2,27,56,2,GOLD,b->task,0);
  text(2,30,CYAN,"TRI LESSON / RECOVERY    %02d/%02d",game.tutorial_step,TUTORIAL_COUNT);
 }else if(page==FLIGHT&&!game.dock_stage&&!game.police_stop&&!paused){
  /* Two narrow lines at the upper edge leave the reticle and instruments clear. */
  rect(8,0,464,20,RGB(10,24,35));text_wrap(2,0,56,2,CYAN,b->task,0);
 }
}
static void input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 if(page==INTRO){
  intro_time+=dt;
  if(pressed&PSP_CTRL_UP)intro_choice=(intro_choice+3)%4;
  if(pressed&PSP_CTRL_DOWN)intro_choice=(intro_choice+1)%4;
  if(pressed&PSP_CTRL_CROSS){
   if(intro_choice==0)tutorial_start();
   else if(intro_choice==1)tutorial_resume();
   else if(intro_choice==2){game_init(&game);story_complete(&game);tutorial_reset_frontend();change_page(CAMPAIGN);}
   else if(load_game(&game,"commander.sav")){tutorial_reset_frontend();change_page(HOME);}
   else message(&game,"No saved commander found.");
  }
  return;
 }
 if(!tutorial_active(&game)){game_input(pressed,held,dt,ax,ay);return;}
 tutorial_prepare();
 if(game.dead){
  if(pressed&PSP_CTRL_START)tutorial_resume();
  else game_input(0,0,dt,0,0);
  return;
 }
 if(page==HOME&&!tutorial_modal&&game.tutorial_seen==game.tutorial_step&&(pressed&PSP_CTRL_TRIANGLE)){tutorial_modal=1;return;}
 if(tutorial_modal||game.tutorial_seen!=game.tutorial_step){
  game.boost=0;
  if(tutorial_confirm){
   if(pressed&PSP_CTRL_CIRCLE)tutorial_confirm=0;
   else if(pressed&PSP_CTRL_CROSS){
    if(tutorial_confirm==2)tutorial_resume();
    else {game.tutorial_step=TUTORIAL_COUNT+1;game.tutorial_seen=0;tutorial_modal=tutorial_confirm=0;story_complete(&game);change_page(HOME);tutorial_checkpoint();}
   }
   return;
  }
  if(pressed&PSP_CTRL_TRIANGLE){tutorial_confirm=1;return;}
  if(pressed&PSP_CTRL_SQUARE){tutorial_confirm=2;return;}
  if(pressed&PSP_CTRL_CROSS){
   game.tutorial_seen=game.tutorial_step;tutorial_modal=0;fire_blocked=1;
   if(tutorial_beat(&game)->event==TU_FINISH)tutorial_emit(TU_FINISH,0);
  }
  return;
 }
 const TutorialBeat *b=tutorial_beat(&game);
 if(page==HOME&&(pressed&PSP_CTRL_CROSS)&&!tutorial_service(&game,row)){message(&game,b->task);return;}
 if(page==HOME&&row==10&&(pressed&PSP_CTRL_CROSS)){comms_return=HOME;change_page(COMMS_PANEL);return;}
 if(page==COMMS_PANEL&&row==9&&(pressed&PSP_CTRL_CROSS)&&!tutorial_service(&game,20)){message(&game,b->task);return;}
 if(page==STATUS&&game.docked&&(pressed&PSP_CTRL_CROSS)){
  if(save_game(&game,"tutorial.sav"))tutorial_emit(TU_SAVE,0);
  return;
 }
 if(page==STATUS&&(pressed&PSP_CTRL_TRIANGLE)){tutorial_resume();return;}
 /* Capture only fields needed by this adapter. Copying the entire Game each
  * frame would consume too much PSP stack and memory bandwidth. */
 int oldpage=page,oldrow=row,oldtab=galnet_tab,oldroom=sc_room,oldmenu=sc_menu;
 int oldtalk=sc_talk_row,oldwho=sc_talk_who,oldsystem=game.system,olddock=game.docked;
 int oldfood=game.cargo[0],oldshots=game.shots,oldscan=game.scanned_anomalies,olddiscoveries=game.discoveries;
 int oldsurface=game.surface,oldplanet=game.planet,oldtractor=game.tractor_time>0;
 int oldpip=game.pip_sys+game.pip_eng*8+game.pip_wep*64;
 float oldyaw=game.yaw,oldpitch=game.pitch,oldroll=game.roll;
 int oldstep=game.tutorial_step;
 game_input(pressed,held,dt,ax,ay);
 if(game.tutorial_step!=oldstep||game.dead)return;
 int event=-1,arg=0;
 switch(b->event){
 case TU_LAUNCH:if(olddock&&!game.docked)event=TU_LAUNCH;break;
 case TU_YAW:if(page==FLIGHT&&!paused&&!(held&(PSP_CTRL_SQUARE|PSP_CTRL_LTRIGGER))&&fabsf(game.yaw-oldyaw)>.0001f)tutorial_practice+=dt;if(tutorial_practice>=1)event=TU_YAW;break;
 case TU_PITCH:if(page==FLIGHT&&!paused&&!(held&PSP_CTRL_SQUARE)&&fabsf(game.pitch-oldpitch)>.0001f)tutorial_practice+=dt;if(tutorial_practice>=1)event=TU_PITCH;break;
 case TU_ROLL:if(page==FLIGHT&&(held&PSP_CTRL_LTRIGGER)&&fabsf(game.roll-oldroll)>.0001f)tutorial_practice+=dt;if(tutorial_practice>=1)event=TU_ROLL;break;
 case TU_THROTTLE:if(page==FLIGHT&&game.speed>=150&&(held&PSP_CTRL_RTRIGGER))event=TU_THROTTLE;break;
 case TU_BRAKE:if(page==FLIGHT&&game.speed<40&&(held&PSP_CTRL_LTRIGGER))event=TU_BRAKE;break;
 case TU_BOOST:if(page==FLIGHT&&game.boost)event=TU_BOOST;break;
 case TU_PIP:if(oldpip!=game.pip_sys+game.pip_eng*8+game.pip_wep*64)event=TU_PIP;break;
 case TU_LOCK:if(page==FLIGHT&&selected_target==0&&autoaim)event=TU_LOCK;break;
 case TU_DOCK:if(!olddock&&game.docked)event=TU_DOCK;break;
 case TU_VIEW:if((oldpage==tutorial_page_for(b->arg)||(b->arg==10&&oldpage==COMMS_PANEL))&&(pressed&PSP_CTRL_CIRCLE)&&page!=oldpage){event=TU_VIEW;arg=b->arg;}break;
 case TU_NET:if(oldpage==GALNET&&oldtab==b->arg&&(pressed&PSP_CTRL_CIRCLE)&&page!=GALNET){event=TU_NET;arg=oldtab;}break;
 case TU_SELL:if(oldpage==MARKET&&oldfood>game.cargo[0])event=TU_SELL;break;
 case TU_BUY:if(oldpage==MARKET&&oldfood<game.cargo[0])event=TU_BUY;break;
 case TU_FIT:if(oldpage==EQUIP&&game.fit[FIT_WPN]==b->arg&&(pressed&PSP_CTRL_CROSS)){event=TU_FIT;arg=b->arg;}break;
 case TU_TALK:if(oldpage==WALK&&oldroom==b->arg&&oldmenu==SC_MENU_TALK&&oldtalk==0&&oldwho==0&&(pressed&PSP_CTRL_CROSS)&&sc_menu!=SC_MENU_TALK){event=TU_TALK;arg=b->arg;}break;
 case TU_SHOP:if(page==WALK&&sc_menu==SC_MENU_SHOP)event=TU_SHOP;break;
 case TU_GUILD:if((oldpage==GUILD&&(pressed&PSP_CTRL_CIRCLE)&&page!=GUILD)||(oldpage==MISSIONLOG&&oldrow==1&&(pressed&PSP_CTRL_CROSS)&&tracked_mission==1))event=TU_GUILD;break;
 case TU_FIRE:if(game.shots>oldshots)event=TU_FIRE;break;
 case TU_SALVAGE:if(oldtractor&&game.tractor_time<=0&&!game.debris[DEBRIS_COUNT-1].alive)event=TU_SALVAGE;break;
 case TU_SCAN:if(game.scanned_anomalies>oldscan)event=TU_SCAN;break;
 case TU_FUEL:if(oldpage==EQUIP&&oldrow==0&&(pressed&PSP_CTRL_CROSS)&&game.fuel>=player_ships[game.ship].range)event=TU_FUEL;break;
 case TU_JUMP:if(oldsystem!=game.system)event=TU_JUMP;break;
 case TU_ATMOSPHERE:if(oldplanet<0&&game.planet>=0)event=TU_ATMOSPHERE;break;
 case TU_LAND:if(oldsurface==0&&game.surface==1)event=TU_LAND;break;
 case TU_EVA:if(oldsurface==1&&game.surface==2)event=TU_EVA;break;
 case TU_SURVEY:if(game.surface==2&&game.discoveries>olddiscoveries)event=TU_SURVEY;break;
 case TU_BOARD:if(oldsurface==2&&game.surface==1)event=TU_BOARD;break;
 case TU_ORBIT:if(oldplanet>=0&&game.planet<0)event=TU_ORBIT;break;
 }
 if(event>=0)tutorial_emit(event,arg);
}
