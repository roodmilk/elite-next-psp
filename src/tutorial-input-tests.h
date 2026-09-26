/* Run real input handlers against each authored completion condition. */
{
 int smoke_before=smoke;smoke=1;
 tutorial_start();
 int vis[6],only_fly=deck_fill(0,vis)==1&&vis[0]==0;
 for(int g=1;g<DECK_GROUPS;g++)only_fly&=deck_fill(g,vis)==0;
 INPUT_CHECK(only_fly,"tutorial: only FLY is visible at the start");
 int introduced=0;for(int i=0;i<TUTORIAL_COUNT;i++)if(tutorial_beats[i].unlock>=0)introduced|=1<<tutorial_beats[i].unlock;
 INPUT_CHECK(introduced==((1<<DECK_ITEMS)-1),"tutorial: script introduces every command-deck service");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.docked&&game.tutorial_step==1,"tutorial: briefing confirmation never launches");
 row=23;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&game.tutorial_step==1,"tutorial: hidden decorator shortcut is gated");
 change_page(CHART);
 INPUT_CHECK(page==HOME,"tutorial: locked pages reject indirect navigation");
 row=0;
 int walkthrough=1;
 for(int step=1;step<=TUTORIAL_COUNT;step++){
  if(game.tutorial_step!=step){walkthrough=0;break;}
  const TutorialBeat *b=tutorial_beat(&game);int kind=b->event,arg=b->arg;
  tutorial_prepare();input(PSP_CTRL_CROSS,0,.016f,0,0);
  game.credits=50000;game.heat=0;game.energy=100;game.hull=100;game.dead=0;
  switch(kind){
   case TU_LAUNCH:game.docked=1;page=HOME;row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);break;
   case TU_YAW:page=FLIGHT;for(int i=0;i<70&&game.tutorial_step==step;i++)input(0,PSP_CTRL_RIGHT,.016f,1,0);break;
   case TU_PITCH:page=FLIGHT;for(int i=0;i<70&&game.tutorial_step==step;i++)input(0,PSP_CTRL_UP,.016f,0,1);break;
   case TU_ROLL:page=FLIGHT;for(int i=0;i<70&&game.tutorial_step==step;i++)input(0,PSP_CTRL_LTRIGGER|PSP_CTRL_RIGHT,.016f,0,0);break;
   case TU_THROTTLE:page=FLIGHT;game.speed=155;input(0,PSP_CTRL_RTRIGGER,.016f,0,0);break;
   case TU_BRAKE:page=FLIGHT;game.speed=20;input(0,PSP_CTRL_LTRIGGER,.016f,0,0);break;
   case TU_BOOST:page=FLIGHT;input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);input(0,0,.016f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);break;
   case TU_PIP:page=FLIGHT;pip_sel=0;input(PSP_CTRL_UP,PSP_CTRL_START,.016f,0,0);input(0,0,.016f,0,0);break;
   case TU_LOCK:page=FLIGHT;selected_target=0;input(PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);break;
   case TU_DOCK:
    page=FLIGHT;game.docked=0;game.planet=-1;game.surface=0;game.pos=(Vec3){0,0,2000};game.speed=0;game.yaw=game.pitch=game.roll=0;
    selected_target=0;autoaim=0;dock(&game);
    for(int i=0;i<1600&&game.tutorial_step==step;i++)input(0,0,.016f,0,0);
    break;
   case TU_VIEW:
    page=HOME;row=arg;input(PSP_CTRL_CROSS,0,.016f,0,0);
    input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_NET:
    page=HOME;row=14;input(PSP_CTRL_CROSS,0,.016f,0,0);
    for(int i=0;i<6&&galnet_tab!=arg;i++)input(PSP_CTRL_RTRIGGER,0,.016f,0,0);
    input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_SAVE:page=STATUS;input(PSP_CTRL_CROSS,0,.016f,0,0);break;
   case TU_SELL:page=MARKET;row=0;input(PSP_CTRL_LEFT,0,.016f,0,0);break;
   case TU_BUY:page=MARKET;row=0;input(PSP_CTRL_RIGHT,0,.016f,0,0);break;
   case TU_FIT:{
    page=EQUIP;int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);row=-1;
    for(int i=0;i<n;i++)if(list[i]==1)row=i;
    INPUT_CHECK(row>=0,"tutorial: starting hub stocks the required Pulse Laser");
    if(row>=0){input(PSP_CTRL_CROSS,0,.016f,0,0);}break;}
   case TU_TALK:
    page=WALK;walk_kind=0;sc_build_map();sc_room=arg;sc_menu=SC_MENU_NONE;
    sc_hot=sc_find_hot(SC_H_PERSON,0);input(PSP_CTRL_CROSS,0,.016f,0,0);
    sc_talk_row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);break;
   case TU_SHOP:
    page=WALK;walk_kind=0;sc_room=SC_R_SHOP;sc_menu=SC_MENU_NONE;
    sc_hot=sc_find_hot(SC_H_PERSON,0);input(PSP_CTRL_CROSS,0,.016f,0,0);
    sc_talk_row=1;input(PSP_CTRL_CROSS,0,.016f,0,0);break;
   case TU_GUILD:page=HOME;row=18;input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_FIRE:page=FLIGHT;game.yaw=3.14f;game.pitch=0;input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);break;
   case TU_SALVAGE:
    page=FLIGHT;input(PSP_CTRL_CIRCLE,0,.016f,0,0);
    for(int i=0;i<90&&game.tutorial_step==step;i++){input(0,0,.016f,0,0);}break;
   case TU_SCAN:page=FLIGHT;input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_FUEL:page=EQUIP;row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);break;
   case TU_JUMP:{
    page=HOME;row=2;input(PSP_CTRL_CROSS,0,.016f,0,0);chart_mode=0;
    input(PSP_CTRL_CROSS,0,.016f,0,0);
    for(int i=0;i<650&&game.tutorial_step==step;i++){input(0,0,.016f,0,0);}break;}
   case TU_ATMOSPHERE:
    launch(&game);page=FLIGHT;game.approach=1;game.bodies[1].type=OCEAN;input(PSP_CTRL_CROSS,0,.016f,0,0);break;
   case TU_LAND:game.pos=add(surface_site(&game,1),(Vec3){0,20,0});game.speed=12;page=FLIGHT;input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_EVA:page=FLIGHT;input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_SURVEY:game.life[0].alive=1;game.life[0].scanned=0;game.life[0].pos=game.pos;page=FLIGHT;input(PSP_CTRL_SQUARE,0,.016f,0,0);break;
   case TU_BOARD:game.pos=game.ship_pos;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;page=FLIGHT;input(PSP_CTRL_CIRCLE,0,.016f,0,0);break;
   case TU_ORBIT:page=FLIGHT;game.voice_time=game.message_time=0;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);game.voice_time=game.message_time=0;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);break;
   case TU_FINISH:break; /* the briefing acknowledgement completes this beat */
  }
  char label[120];snprintf(label,sizeof(label),"tutorial: lesson %02d completes through input (%s)",step,b->title);
  INPUT_CHECK(game.tutorial_step==step+1,label);
  if(game.tutorial_step!=step+1){walkthrough=0;break;}
 }
 INPUT_CHECK(walkthrough&&!tutorial_active(&game)&&game.campaign_stage==6,"tutorial: complete authored route awards harbour licence and reaches unrestricted play");
 tutorial_start();game.tutorial_seen=1;game.tutorial_step=1;
 INPUT_CHECK(!tutorial_event(&game,TU_DOCK,0)&&game.tutorial_step==1,"tutorial: out-of-order actions cannot skip a lesson");
 game.tutorial_seen=0;
 INPUT_CHECK(!tutorial_event(&game,TU_LAUNCH,0),"tutorial: actions before briefing acknowledgement are ignored");
 game.tutorial_step=TUTORIAL_COUNT+1;game.tutorial_seen=0;
 INPUT_CHECK(tutorial_service(&game,24)&&tutorial_service(&game,9),"tutorial: completion unlocks all services");
 game.tutorial_step=15;game.tutorial_seen=15;game.credits=3210;
 INPUT_CHECK(save_game(&game,"tutorial.sav"),"tutorial: continuation fixture saved");
 game.credits=9;page=INTRO;intro_choice=1;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&game.tutorial_step==15&&game.tutorial_seen==0&&game.credits==3210,"tutorial: Continue restores checkpoint and repeats its briefing");
 INPUT_CHECK(!strcmp(commander_save_path(&game),"tutorial.sav"),"tutorial: tutorial saves cannot target regular commander");
 game.dead=1;page=FLIGHT;input(PSP_CTRL_START,0,.016f,0,0);
 INPUT_CHECK(!game.dead&&game.docked&&game.tutorial_step==15,"tutorial: death recovery returns to the saved lesson");
 tutorial_start();input(PSP_CTRL_TRIANGLE,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(tutorial_active(&game),"tutorial: cancelling exit keeps training active");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(!tutorial_active(&game)&&game.campaign_stage==0,"tutorial: confirmed exit opens services without awarding a licence");
 smoke=smoke_before;TEST_INIT();page=HOME;tutorial_reset_frontend();
}
