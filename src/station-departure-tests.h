{
 TEST_INIT();page=HOME;row=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(page==FLIGHT&&game.dock_stage==4&&!game.docked&&forward(&game).z<-.99f,"departure: real Launch menu starts outward first-person sequence");
 float fuel=game.fuel,hull=game.hull;int shots=game.shots;
 rect(0,0,W,H,BG);space();dump_native_bmp("departure-berth.bmp");
 for(int i=0;i<100;i++)input(PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SELECT|PSP_CTRL_CROSS,PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SELECT|PSP_CTRL_CROSS,.016f,1,1);
 INPUT_CHECK(page==FLIGHT&&game.dock_stage==4&&game.shots==shots&&game.yaw>3,"departure: button mashing cannot skip, steer, fire or open menus");
 float fast=0;
 for(int i=0;i<100;i++){input(0,0,.016f,0,0);fast=fmaxf(fast,game.speed);}
 rect(0,0,W,H,BG);space();dump_native_bmp("departure-solar.bmp");
 for(int i=0;i<140;i++)input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 INPUT_CHECK(game.dock_stage==0&&departure_release&&fast>850&&game.speed<=100.1f,"departure: acceleration eases back to cruise and awaits button release");
 INPUT_CHECK(game.fuel==fuel&&game.hull==hull&&!game.dead&&game.pos.z<station_entry_z_for(&game,0)-1000,"departure: clear of station without boost fuel or collision damage");
 INPUT_CHECK(dot(forward(&game),norm(sub(game.bodies[0].pos,game.pos)))>.999f,"departure: real sun lies ahead at handoff");
 input(0,0,.016f,0,0);INPUT_CHECK(!departure_release&&!tools_open,"departure: releasing controls completes clean handoff");
 tools_selected=1;int flares=game.flare_charges;input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 INPUT_CHECK(game.flare_charges==flares-1,"departure: first fresh Circle tap works after release gate");
 float yaw=game.yaw;input(0,0,.016f,.5f,0);INPUT_CHECK(game.yaw!=yaw,"departure: fresh steering works after handoff");
 for(int hub=1;hub<HUB_COUNT;hub++){
  TEST_INIT();game.station_variant=hub;launch_departure(&game);Vec3 center=hub_position(&game,hub);
  INPUT_CHECK(game.station_variant==hub&&length(sub(game.pos,center))<111,"departure: relay departure begins in its own berth");
  for(int i=0;i<320;i++)game_tick(&game,.016f,0,0,0,0);
  INPUT_CHECK(!game.dock_stage&&length(sub(game.pos,center))>1700,"departure: relay exits safely toward local sun");
 }
 TEST_INIT();launch_departure(&game);int dest=-1;for(int i=0;i<256;i++)if(i!=game.system&&distance_ly(&game,game.system,i)*10<=game.fuel){dest=i;break;}
 game.destination=dest;INPUT_CHECK(jump_start(&game)&&game.jump==0&&game.dock_phase==1,"departure: docked chart request queues warp until clear");
 for(int i=0;i<313;i++)game_tick(&game,.016f,0,0,0,0);
 INPUT_CHECK(!game.dock_stage&&game.jump>0,"departure: queued warp starts only after launch sequence");
 TEST_INIT();page=GALNET;lock_wanted_poster(0);
 for(int i=0;i<320;i++)input(0,0,.016f,0,0);
 INPUT_CHECK(!game.dock_stage&&autoaim&&selected_target==NPC_ID_MIN+BOUNTY_NPC_FIRST,"departure: wanted pursuit resumes its selected lock after safe exit");
 TEST_INIT();page=HOME;
}
