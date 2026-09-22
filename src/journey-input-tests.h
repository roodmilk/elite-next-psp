{
 TEST_INIT();accept_mission(&game,0);launch(&game);page=HOME;
 float remain=game.jobs[0].time;input(0,0,.05f,0,0);
 INPUT_CHECK(game.jobs[0].time==remain,"journey: reading command deck pauses jobs");
 page=FLIGHT;game.police_stop=1;input(0,0,.05f,0,0);
 INPUT_CHECK(game.jobs[0].time==remain,"journey: reading police choices pauses jobs");
 game.police_stop=0;game.approach=1;input(0,0,.05f,0,0);
 INPUT_CHECK(game.jobs[0].time==remain,"journey: reading planet choices pauses jobs");
 TEST_INIT();launch(&game);game.pos=(Vec3){0,0,2700};dock(&game);page=FLIGHT;
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 INPUT_CHECK(!game.dock_stage&&!game.docked&&!game.dead&&game.speed==0,"journey: Circle safely cancels exterior docking guidance");
 TEST_INIT();change_page(HOME);row=13;input(PSP_CTRL_CROSS,0,.016f,0,0);row=1;input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_SELECT,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&tracked_mission==1,"guild: Mission Log tracks the Guild assignment and opens its next step");
 TEST_INIT();accept_mission(&game,0);int dest=game.jobs[0].dest,hops=0;int next=route_next_hop(&game,dest,&hops);page=MISSIONLOG;row=2;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_SELECT,0,.016f,0,0);input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==CHART&&game.destination==next&&game.jobs[0].dest==dest,"journey: tracked contract next step charts a fuel-safe hop without changing the objective");
 TEST_INIT();launch(&game);page=COMMS;game.pos=(Vec3){0,0,-20000};game.credits=0;
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
 INPUT_CHECK(comms_rescue_confirm&&!game.docked,"journey: rescue requires explicit in-game confirmation");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&game.docked&&game.fuel>0,"journey: confirmed rescue returns safely to services");
 TEST_INIT();launch(&game);change_page(HOME);row=0;game.shots=0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(0,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(game.shots==0,"journey: holding menu confirm does not fire after resuming");
 input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(game.shots>0,"journey: laser rearms after confirm is released");
}
