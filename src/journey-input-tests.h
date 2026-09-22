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
/* Generic PR-01 rescue: exercise the real buttons, then arrival and Guild claim.
 * Travel/encounter placement is deterministic; this is not a flight soak. */
{
 TEST_INIT();game.guild_chapter=3;game.guild_flags=0;page=MISSIONS;
 int offer=-1;for(int i=0;i<mission_count(&game);i++)if(mission_type_for_offer(&game,i)==MISSION_RESCUE){offer=i;break;}
 INPUT_CHECK(offer>=0,"rescue: initial board contains a real pilot rescue");
 if(offer>=0){
  int start=game.credits;row=offer;input(PSP_CTRL_CROSS,0,0,0,0);
  INPUT_CHECK(game.job_n==1&&game.jobs[0].type==MISSION_RESCUE&&game.credits==start-100&&game.jobs[0].time==300,"rescue: board acceptance preserves deposit and clock");
  int dest=game.jobs[0].dest,reward=game.jobs[0].reward,hops=0,next=route_next_hop(&game,dest,&hops);
  change_page(MISSIONLOG);row=2;input(PSP_CTRL_CROSS,0,0,0,0);input(PSP_CTRL_SELECT,0,0,0,0);input(PSP_CTRL_CROSS,0,0,0,0);
  INPUT_CHECK(tracked_mission==2&&page==CHART&&next>=0&&game.destination==next&&game.jobs[0].dest==dest,"rescue: track and navigate retain job and plot a fuel-safe hop");
  /* Arrive at the real contract destination, then isolate input from traffic. */
  launch(&game);game.system=dest;game_spawn(&game);page=FLIGHT;
  int mark=game.jobs[0].target,id=BODY_COUNT+1+mark;
  INPUT_CHECK(mark>=0&&mark<NPC_COUNT,"rescue: destination spawns a marked pilot");
  if(mark>=0&&mark<NPC_COUNT){
   for(int i=0;i<NPC_COUNT;i++)if(i!=mark)game.npc[i].alive=0;
   game.pos=(Vec3){0,0,-20000};game.yaw=game.pitch=game.roll=0;game.speed=0;
   /* Freeze the contact for exact radius checks; zero-dt input still ticks. */
   game.npc[mark].waypoint=8;game.npc[mark].cruise=0;
   game.npc[mark].pos=add(game.pos,(Vec3){0,0,601});selected_target=id;autoaim=0;
   INPUT_CHECK(strstr(mission_objective_at(&game,0),"Triangle")&&!strstr(mission_objective_at(&game,0),"press O"),"rescue: objective names the live hail control");
   /* Optional native evidence uses the production renderer and private pixels. */
   FILE *capture=fopen("rescue-capture.flag","r");if(capture){fclose(capture);unsigned *saved=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));INPUT_CHECK(pixels!=0,"rescue: native capture buffer allocated");if(pixels){fb=pixels;rect(0,0,W,H,BG);int oldpage=page;page=CAMPAIGN;row=0;campaign_screen();dump_native_bmp("rescue-objective.bmp");page=oldpage;fb=saved;free(pixels);}}
   speech_ok();input(PSP_CTRL_CIRCLE,0,0,0,0);
   INPUT_CHECK(selected_target==id&&!game.jobs[0].stage,"rescue: Circle locks the pilot without pickup");
   /* Triangle first acknowledges the lock notice, then a fresh tap hails. */
   input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,0,0,0);input(0,0,0,0,0);
   INPUT_CHECK(!game.jobs[0].stage&&!speech_active(),"rescue: first Triangle acknowledges the lock notice");
   input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,0,0,0);input(0,0,0,0,0);
   INPUT_CHECK(!game.jobs[0].stage&&game.credits==start-100,"rescue: hail beyond 600 m cannot pick up or pay");
   /* A different visible NPC at the same distance is not the rescue target. */
   int other=(mark+1)%NPC_COUNT;game.npc[other]=game.npc[mark];game.npc[other].alive=1;game.npc[other].pos=add(game.pos,(Vec3){0,0,500});game.npc[mark].pos=add(game.pos,(Vec3){2000,0,600});selected_target=BODY_COUNT+1+other;
   speech_ok();input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,0,0,0);input(0,0,0,0,0);
   INPUT_CHECK(!game.jobs[0].stage,"rescue: hailing unrelated ship cannot recover the pilot");game.npc[other].alive=0;
   game.npc[mark].pos=add(game.pos,(Vec3){0,0,600});selected_target=id;speech_ok();
   input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,0,0,0);input(0,0,0,0,0);
   INPUT_CHECK(game.jobs[0].stage==1&&game.credits==start-100&&!guild_ready(&game),"rescue: exact 600 m hail picks up but does not pay or complete Guild");
   INPUT_CHECK(strstr(mission_objective_at(&game,0),"Return")&&strstr(game.message,"Pilot aboard"),"rescue: pickup explains the station return");
   speech_ok();input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,0,0,0);input(0,0,0,0,0);
   INPUT_CHECK(game.job_n==1&&game.jobs[0].stage==1&&game.credits==start-100,"rescue: repeated hail cannot duplicate pickup or payout");
   for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
   game.pos=(Vec3){0,0,2700};game.yaw=game.pitch=game.roll=0;selected_target=0;game.speed=0;speech_ok();
   input(PSP_CTRL_CIRCLE,0,0,0,0);for(int t=0;t<400&&!game.docked&&!game.dead;t++)input(0,0,.05f,0,0);
   INPUT_CHECK(game.docked&&!game.dead&&game.job_n==0&&game.credits==start-100+reward&&guild_ready(&game),"rescue: guided system-hub arrival pays once and marks Guild ready");
   INPUT_CHECK(guild_claim(&game)&&game.guild_chapter==4&&game.credits==start-100+reward+3000,"rescue: Guild claim pays once and advances");
   INPUT_CHECK(!guild_claim(&game)&&game.credits==start-100+reward+3000,"rescue: repeated Guild claim cannot pay twice");
   launch(&game);for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;page=FLIGHT;game.pos=(Vec3){0,0,2700};game.yaw=game.pitch=game.roll=0;game.speed=0;selected_target=0;speech_ok();
   input(PSP_CTRL_CIRCLE,0,0,0,0);for(int t=0;t<400&&!game.docked&&!game.dead;t++)input(0,0,.05f,0,0);
   INPUT_CHECK(game.docked&&game.credits==start-100+reward+3000&&game.guild_chapter==4,"rescue: redocking cannot repeat contract or Guild reward");
  }
  TEST_INIT();game.guild_chapter=3;game.guild_flags=0;page=MISSIONS;row=offer;input(PSP_CTRL_CROSS,0,0,0,0);int paid=game.credits;launch(&game);page=FLIGHT;game.pos=(Vec3){0,0,-20000};game.speed=0;game.jobs[0].time=.001f;speech_ok();input(0,0,.016f,0,0);
  INPUT_CHECK(!game.job_n&&game.mission_result==-1&&game.credits==paid&&!guild_ready(&game)&&strstr(game.message,"Mission expired"),"rescue: expiry reports failure without payout or Guild completion");
  game.docked=1;page=MISSIONS;row=offer;input(PSP_CTRL_CROSS,0,0,0,0);
  INPUT_CHECK(game.job_n==1&&!game.jobs[0].stage&&game.credits==paid-100,"rescue: expired offer can be accepted afresh");
 }
}
