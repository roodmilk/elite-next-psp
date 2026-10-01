{
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};game.speed=0;speech_ok();night_wait=480;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 NPC *n=&game.npc[0];n->alive=1;n->role=PIRATES;n->freighter=0;n->pos=add(game.pos,(Vec3){0,0,1500});n->dir=(Vec3){0,0,1};n->health=1000;n->shield=0;n->cooldown=100;
 selected_target=NPC_ID_MIN;game.missiles=4;
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 INPUT_CHECK(!tools_open&&game.missiles==4,"weapon computer: initial Circle press waits to distinguish tap from hold");
 for(int i=0;i<15;i++)input(0,PSP_CTRL_CIRCLE,.016f,0,0);
 INPUT_CHECK(tools_open&&game.missiles==4,"weapon computer: only holding opens the selector without firing");
 float yaw=game.yaw,pitch=game.pitch;
 input(PSP_CTRL_UP,PSP_CTRL_CIRCLE|PSP_CTRL_UP,.016f,1,1);
 INPUT_CHECK(tools_selected==0&&!tools_open&&game.missiles==4&&game.yaw==yaw&&game.pitch==pitch,"weapon computer: direction equips and closes, without firing or steering");
 for(int i=0;i<20;i++)input(PSP_CTRL_UP,PSP_CTRL_CIRCLE|PSP_CTRL_UP,.016f,1,1);
 input(0,0,.016f,0,0);
 INPUT_CHECK(!tools_open&&game.missiles==4,"weapon computer: held repeat and selection release never fire");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.missiles==3&&!tools_open,"weapon computer: fresh Circle tap fires selected missile once");
 for(int action=1;action<3;action++){
  input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
  input(tool_buttons[action],PSP_CTRL_CIRCLE|tool_buttons[action],.016f,0,0);
  input(0,0,.016f,0,0);
  INPUT_CHECK(tools_selected==action&&!tools_open,"weapon computer: each direction selects its tool and disappears on release");
 }
 game.ship=6;fit_clear_all(&game);game.fit[0]=1;game.fit[6]=2;game.fit[12]=25;fit_rebuild(&game);game.active_weapon=0;tools_selected=2;
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.active_weapon==6&&fit_weapon_item(&game)==2&&tools_selected==2&&!tools_open,"weapon computer: Circle+Right arms the next fitted WPN without changing the selected tool");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.active_weapon==12&&fit_weapon_item(&game)==25,"weapon computer: repeated Circle+Right skips empty banks and continues cycling");
 game.fit[6]=game.fit[12]=FIT_EMPTY;game.active_weapon=0;fit_rebuild(&game);
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.active_weapon==0&&strstr(game.message,"Only one WPN"),"weapon computer: one fitted weapon stays active with clear feedback");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(tools_selected==3&&!tools_open,"weapon computer: Circle+Triangle preserves the heat-sink shortcut");
 game.upgrades|=2048;game.heat=90;game.heat_sink_cd=0;
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.heat<51&&game.heat_sink_cd>29,"weapon computer: tap activates selected heat sink");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);for(int i=0;i<20;i++)input(0,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(!tools_open,"weapon computer: unselected long hold closes immediately on release");
 tools_selected=0;game.missile_time=0;change_page(HOME);
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 int count=game.missiles;
 for(int i=0;i<25;i++)input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 input(0,0,.016f,0,0);
 INPUT_CHECK(page==FLIGHT&&!tools_open&&game.missiles==count,"weapon computer: menu Back and repeated held Circle cannot open or discharge");
 int back_safe=1;const int menus[]={HELP,COMMS_PANEL,RADIO,GALNET};
 for(int m=0;m<4;m++){
  change_page(FLIGHT);input(0,0,.016f,0,0);comms_return=FLIGHT;change_page(menus[m]);
  input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
  for(int i=0;i<20;i++)input(0,PSP_CTRL_CIRCLE,.016f,0,0);
  input(0,0,.016f,0,0);
  back_safe&=!tools_open&&game.missiles==count;
  if(page==HOME){input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);for(int i=0;i<20;i++)input(0,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);}
  back_safe&=page==FLIGHT&&!tools_open&&game.missiles==count;
 }
 INPUT_CHECK(back_safe,"weapon computer: Controls, Comms, Radio and GalacticNet Back do not leak Circle into flight");
 int visible[6],has_exit=0;deck_fill(0,visible);INPUT_CHECK(!deck_service_visible(20),"Fly: Disembark is hidden away from stations");
 game.docked=1;has_exit=deck_service_visible(20);INPUT_CHECK(has_exit,"Fly: Disembark is available when docked");
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,2700};game.speed=0;speech_ok();selected_target=0;night_wait=480;
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(!game.dock_stage&&!game.docked,"station: Circle near a station cannot request auto-dock");
 speech_ok();input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(page==COMMS_PANEL&&row==3&&!game.dock_stage,"station: Triangle opens Comms with REQUEST AUTO-DOCK selected");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(page==FLIGHT&&game.dock_stage==1,"station: only explicit request starts guided docking");
 TEST_INIT();launch(&game);page=FLIGHT;game.speed=0;speech_ok();selected_target=2;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-800});game.yaw=game.pitch=0;
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.approach==1&&game.planet<0&&!game.planet_sequence,"Triangle planet hail: requests confirmation without starting landing");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 INPUT_CHECK(game.approach<0&&game.planet<0,"planet confirmation: Circle cancels rather than firing");
 TEST_INIT();
}
{
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};game.speed=0;game.yaw=game.pitch=0;speech_ok();night_wait=480;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;
 Debris *d=&game.debris[0];memset(d,0,sizeof(*d));d->alive=1;d->pos=add(game.pos,(Vec3){300,0,0});d->life=100;d->qty=1;d->good=0;selected_target=0;
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_LEFT,PSP_CTRL_CIRCLE|PSP_CTRL_LEFT,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(tools_selected==2&&!strcmp(tool_names[2],"TRACTOR")&&tools_tractor<0&&d->alive,"ship tools: Circle+Left equips tractor without firing");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(selected_target==DEBRIS_ID_MIN&&tools_tractor>=0&&game.yaw==0&&game.tractor_time==0,"tractor: tap finds nearby cargo and waits for smooth alignment");
 input(0,0,.016f,0,0);
 INPUT_CHECK(game.yaw>0&&game.yaw<.04f&&d->alive,"tractor: initial turn is smooth, not an instant snap");
 int before=game.cargo[0];for(int i=0;i<200;i++)input(0,0,.016f,0,0);
 INPUT_CHECK(!d->alive&&game.cargo[0]==before+1&&tools_tractor<0,"tractor: alignment completes then animation recovers exactly one item");
 d->alive=1;d->life=100;d->pos=add(game.pos,(Vec3){600,0,0});tools_activate(2);
 INPUT_CHECK(tools_tractor<0&&game.tractor_time<=0,"tractor: cannot collect distant cargo");
 d->pos=add(game.pos,(Vec3){200,0,0});d->rock=1;tools_activate(2);
 INPUT_CHECK(tools_tractor<0,"tractor: intact asteroids are not salvage");
 d->rock=0;game.yaw=0;tools_activate(2);input(0,0,.016f,.7f,0);
 INPUT_CHECK(tools_tractor<0&&d->alive,"tractor: manual steering cancels pending pickup");
 tools_activate(2);police_begin(&game,0);input(0,0,.016f,0,0);
 INPUT_CHECK(tools_tractor<0&&d->alive,"tractor: police stop cancels alignment safely");
 game.police_stop=0;tools_activate(2);change_page(HOME);input(0,0,.016f,0,0);
 INPUT_CHECK(tools_tractor<0&&d->alive,"tractor: opening a menu cancels alignment safely");
 TEST_INIT();
}
