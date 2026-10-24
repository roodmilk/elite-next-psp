/* Regression: an accepted race is ordinary flight until READY is confirmed. */
static int erl_flight_state_review(void){
 FILE *flag=fopen("erl-flight-state-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("erl-flight-state-review.txt","w");if(!out)return 1;int failures=0,checks=0;
 #define FCHK(c,s) do{int ok=!!(c);checks++;failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);fflush(out);}while(0)
 unsigned *old=fb,*memory=calloc(STRIDE*H,sizeof(unsigned));Game *copy=malloc(sizeof(Game));
 if(!memory||!copy){fprintf(out,"RESULT 1 failures\n");fclose(out);free(memory);free(copy);return 1;}fb=memory;
 /* Completion and skipping retain a nonzero save marker, not an active lesson. */
 erl_checkpoint_collect(1);erl_review_fixture(1);game.erl.referral=game.erl.licence=1;
 int active_blocked=station_class(&game)==STATION_MEGA;
 for(int step=1;step<=TUTORIAL_COUNT;step++){
  game.tutorial_step=step;game.tutorial_seen=step;
  active_blocked&=!erl_hq(&game)&&!erl_begin(&game,ERL_PRACTICE,0,ERL_CIRCUIT,0)&&!erl_season_start(&game)&&game.tutorial_step==step&&!game.erl.event_kind&&!game.erl.championship;
 }
 FCHK(active_blocked,"every active First Light lesson still blocks HQ, practice and championship entry at a real Mega Capitol");
 for(int skipped=0;skipped<2;skipped++){
  erl_review_fixture(1);game.erl.referral=game.erl.licence=1;int ended=0;
  if(skipped){
   tutorial_reset_frontend();game.tutorial_step=game.tutorial_seen=1;tutorial_prepare();tutorial_modal=tutorial_confirm=1;page=HOME;
   input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ended=!tutorial_active(&game);
  }else {game.tutorial_step=game.tutorial_seen=TUTORIAL_COUNT;ended=tutorial_event(&game,TU_FINISH,0);}
  FCHK(ended&&game.tutorial_step==TUTORIAL_COUNT+1&&erl_hq(&game),skipped?"confirmed Skip Tutorial retains its completed marker and unlocks real headquarters":"final tutorial event retains its completed marker and unlocks real headquarters");
  FCHK(erl_begin(&game,ERL_PRACTICE,0,ERL_CIRCUIT,0)&&game.race.phase==RACE_GRID&&game.race.layout==5&&game.tutorial_step==TUTORIAL_COUNT+1,skipped?"skipped tutorial can accept actual Race 1 practice without clearing its save marker":"completed tutorial can accept actual Race 1 practice without clearing its save marker");
  game.erl.event_kind=0;game.race.phase=RACE_NONE;
  FCHK(erl_season_start(&game)&&erl_begin(&game,ERL_OFFICIAL,0,ERL_CIRCUIT,0)&&game.race.phase==RACE_GRID&&game.race.layout==5&&game.race.laps==2&&game.race.rivals==6&&game.tutorial_step==TUTORIAL_COUNT+1,skipped?"skipped tutorial can enter League 1 round one with its original marker and seven racers":"completed tutorial can enter League 1 round one with its original marker and seven racers");
 }
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;erl_start_event(ERL_PRACTICE,0,ERL_CIRCUIT,0);
 FCHK(game.race.phase==RACE_GRID&&!race_is_active(&game)&&page==HOME,"accepting event keeps race pending at station");
 unsigned pending_serial=game.erl.serial,pending_seed=game.race.seed;int pending_kind=game.erl.event_kind;
 page=HOME;row=31;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 FCHK(page==ERL_PAGE&&erl_screen==ERL_HOME,"Command Deck X reopens HQ with an accepted pending race");
 FCHK(erl_motion_view()==ERL_HOME,"pending-race HQ retains menu selection animations");
 erl_review_capture("erl412-pending-headquarters.bmp");
 int menu_ok=1;const int views[]={ERL_SEASON,ERL_DRIVER,ERL_RIVALS,ERL_GARAGE};
 for(int i=0;i<4;i++){erl_screen=ERL_HOME;erl_choice=i+1;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);menu_ok&=page==ERL_PAGE&&erl_screen==views[i];input(PSP_CTRL_CIRCLE,0,.016f,0,0);menu_ok&=page==ERL_PAGE&&erl_screen==ERL_HOME;}
 FCHK(menu_ok,"accepted race does not swallow Season Driver Rivals Garage or Back inputs");
 erl_screen=ERL_BRIEF;erl_choice=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 FCHK(page==HOME&&game.docked&&game.race.phase==RACE_GRID&&selected_target==RACE_TARGET_ID,"HQ pending brief sets beacon without starting or teleporting from station");
 int repeated=1;for(int i=0;i<5;i++){row=31;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);repeated&=page==ERL_PAGE&&erl_screen==ERL_HOME;input(PSP_CTRL_CIRCLE,0,.016f,0,0);repeated&=page==HOME&&row==31;}
 FCHK(repeated,"repeated X and Circle reopen the selected League button reliably");
 FCHK(game.erl.serial==pending_serial&&game.race.seed==pending_seed&&game.erl.event_kind==pending_kind&&game.race.phase==RACE_GRID,"HQ browsing preserves the exact accepted event and course");
 game.docked=0;game.dock_stage=0;page=FLIGHT;game.pos=add(game.race.start,mul(game.race.dir,-9000));Vec3 before=game.pos;
 for(int i=0;i<STRIDE*H;i++)fb[i]=0x12345678;race_hud();int unchanged=1;for(int i=0;i<STRIDE*H;i++)unchanged&=fb[i]==0x12345678;
 FCHK(unchanged,"pending race draws zero race HUD pixels over normal flight");
 erl_review_capture("erl410-pending-flight.bmp");race_ui_sync();FCHK(!radio_race_on,"pending race retains ordinary radio selection");
 int credits=game.credits,xp=game.erl.xp,round=game.erl.round;
 erl_sleep_pause(1);erl_sleep_pause(0);
 FCHK(!game.erl.paused&&!game.erl.resume&&length(sub(before,game.pos))<.001f,"pre-start suspend/resume leaves normal flight and position intact");
 FCHK(!erl_input(PSP_CTRL_CROSS,0,.016f,0,0),"pre-start flight input is not captured by ERL pause controls");
 game.erl.paused=game.erl.resume=1;erl_confirm=1;erl_prestart_recover();
 FCHK(!game.erl.paused&&!game.erl.resume&&!erl_confirm&&game.erl.event_kind,"stale pre-start softlock flags clear without cancelling event");
 game.erl.resume=1;game.erl.paused=1;game.erl.resume_pos=(Vec3){1,2,3};erl_resume(&game);
 FCHK(!game.erl.paused&&!game.erl.resume&&length(sub(before,game.pos))<.001f,"legacy pre-start recovery never restores race camera or teleports player");
 game.erl.resume=1;FCHK(save_game(&game,"erl410-pending.sav")&&load_game(copy,"erl410-pending.sav")&&!copy->erl.paused&&!copy->erl.resume&&copy->race.phase==RACE_GRID&&copy->erl.event_kind,"saved affected pre-start event loads without pause dialog and retains contract");
 /* Exercise READY through real input, then through the simulation guard. */
 *copy=game;
 for(int blocked=0;blocked<5;blocked++){
  game=*copy;game.pos=game.race.start;page=FLIGHT;
  if(blocked==0)game.dock_stage=1;
  if(blocked==1)game.jump=1;
  if(blocked==2)game.planet=0;
  if(blocked==3)game.dead=1;
  if(blocked==4)game.police_stop=1;
  input(PSP_CTRL_TRIANGLE,0,.016f,0,0);erl_choice=0;
  input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
  fprintf(out,"OBSERVE blocker=%d phase_after_READY=%d ",blocked,game.race.phase);
  race_preflight(&game,.016f);fprintf(out,"phase_after_tick=%d\n",game.race.phase);
  FCHK(!game.erl.resume&&game.race.phase==RACE_GRID,"blocked READY never creates an active countdown or resume checkpoint");
  FCHK(erl_notice[0]&&page==ERL_PAGE,"blocked READY displays an actionable reason in the brief");
  if(blocked==0)erl_review_capture("entry-blocked-docking.bmp");
  game.dock_stage=game.dead=game.police_stop=0;game.jump=0;game.planet=-1;
  input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);race_preflight(&game,.016f);
  FCHK(game.race.phase==RACE_COUNTDOWN&&page==FLIGHT,"fresh READY succeeds after the blocking state clears");
 }
 game=*copy;page=FLIGHT;
 game.pos=game.race.start;FCHK(erl_input(PSP_CTRL_TRIANGLE,0,.016f,0,0)&&page==ERL_PAGE&&erl_screen==ERL_BRIEF,"Triangle at beacon still opens READY brief");
 erl_choice=0;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 FCHK(race_is_active(&game)&&game.race.phase==RACE_COUNTDOWN&&page==FLIGHT,"READY explicitly starts countdown and race UI");
 for(int tick=0;tick<160&&game.race.phase==RACE_COUNTDOWN;tick++)input(0,0,.05f,0,0);
 FCHK(game.race.phase==RACE_RUNNING,"READY countdown advances through real input ticks into RUNNING");
 erl_review_capture("entry-ready-running.bmp");race_restart(&game);
 erl_sleep_pause(1);float countdown=game.race.countdown;
 FCHK(game.erl.paused&&save_game(&game,"erl410-countdown.sav")&&load_game(copy,"erl410-countdown.sav")&&copy->erl.paused&&copy->race.countdown==countdown,"active countdown persists and resumes paused safely");
 erl_review_capture("erl410-countdown-paused.bmp");erl_input(PSP_CTRL_CROSS,0,.016f,0,0);
 FCHK(!game.erl.paused&&game.race.countdown==countdown&&game.race.handover==0,"Resume X consumes input without skipping countdown or softlocking");
 game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.elapsed=12.5f;game.race.gate=3;erl_sleep_pause(1);erl_sleep_pause(0);
 FCHK(game.erl.paused&&game.race.elapsed==12.5f&&game.race.gate==3,"active running sleep pauses exact race progress");
 erl_review_capture("erl410-running-paused.bmp");erl_input(PSP_CTRL_CIRCLE,0,.016f,0,0);FCHK(!game.erl.paused,"Circle resumes active race");
 race_ui_sync();FCHK(radio_race_on,"started race selects race radio");
 erl_input(PSP_CTRL_START,0,.016f,0,0);erl_input(PSP_CTRL_RIGHT,0,.016f,0,0);erl_input(PSP_CTRL_CROSS,0,.016f,0,0);
 FCHK(game.erl.paused&&erl_confirm,"Withdraw opens confirmation during active race");
 erl_confirm_yes=0;erl_input(PSP_CTRL_CROSS,0,.016f,0,0);FCHK(game.erl.paused&&!erl_confirm,"declining withdraw keeps active pause operable");
 erl_choice=1;erl_input(PSP_CTRL_CROSS,0,.016f,0,0);erl_confirm_yes=1;erl_input(PSP_CTRL_CROSS,0,.016f,0,0);
 FCHK(game.erl.result&&!game.erl.paused,"confirmed Withdraw exits pause into existing results");
 FCHK(game.credits==credits&&game.erl.xp==xp&&game.erl.round==round,"sleep and recovery never grant rewards or advance practice progression");
 erl_return_hq();FCHK(!game.erl.event_kind&&game.docked,"withdrawn event can return to headquarters without softlock");
 /* Mission Board races share pause/withdraw without owning a league round. */
 erl_review_fixture(1);game.erl.referral=1;game.erl.event_kind=game.erl.result=0;game.job_n=1;game.jobs[0]=(Job){.type=MISSION_RACE,.dest=game.system,.origin=game.system,.item=1,.target=40,.reward=6000,.time=300};game.race.phase=RACE_NONE;race_prepare(&game);page=FLIGHT;game.docked=0;
 erl_sleep_pause(0);FCHK(!game.erl.paused&&game.race.phase==RACE_GRID,"sleep before a board race never creates an active-race warning");race_restart(&game);
 float board_count=game.race.countdown;erl_input(PSP_CTRL_START,0,.016f,0,0);FCHK(game.erl.paused,"Start opens the shared pause screen during a board countdown");erl_input(PSP_CTRL_CROSS,0,.016f,0,0);
 FCHK(!game.erl.paused&&game.race.countdown==board_count,"board Resume consumes X without skipping the countdown");game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.elapsed=27;erl_sleep_pause(1);
 FCHK(game.erl.paused&&save_game(&game,"board-pause.sav")&&load_game(copy,"board-pause.sav")&&copy->erl.paused&&copy->race.elapsed==27,"active board sleep checkpoint reloads paused with exact race progress");erl_review_capture("board-paused.bmp");int board_cash=game.credits;
 erl_input(PSP_CTRL_RIGHT,0,.016f,0,0);erl_input(PSP_CTRL_CROSS,0,.016f,0,0);FCHK(erl_confirm&&game.job_n==1,"board Withdraw asks for confirmation before failing the contract");erl_review_capture("board-withdraw-confirm.bmp");erl_confirm_yes=0;erl_input(PSP_CTRL_CROSS,0,.016f,0,0);
 FCHK(!erl_confirm&&game.erl.paused&&game.job_n==1,"declining board withdrawal preserves the paused attempt");erl_input(PSP_CTRL_CROSS,0,.016f,0,0);erl_confirm_yes=1;erl_input(PSP_CTRL_CROSS,0,.016f,0,0);
 FCHK(!game.job_n&&!game.erl.paused&&game.race.phase==RACE_LOST&&game.credits==board_cash+100,"confirmed board withdrawal returns only the normal deposit, with no race prize or softlock");
 /* Finish while an older immutable checkpoint is still being written. */
 erl_checkpoint_collect(1);erl_review_fixture(1);game.erl.event_kind=game.erl.result=0;game.erl.referral=game.erl.licence=1;game.docked=0;game.opening.stage=0;game.job_n=1;
 game.jobs[0]=(Job){.type=MISSION_RACE,.dest=game.system,.origin=game.system,.item=1,.target=40,.reward=6000,.time=300};game.race.phase=RACE_NONE;race_prepare(&game);race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.gate=31;game.race.elapsed=150;game.race.arcade.rules.sponsor=1;game.race.arcade.rules.seed=4;game.race.arcade.sponsor_done=1;game.erl.paused=game.erl.resume=0;page=FLIGHT;
 int finish_cash=game.credits;erl_checkpoint_test_delay=250000;FCHK(erl_checkpoint_queue(),"board completion regression queues an older mid-race snapshot");
 RaceGate *last_gate=&game.race.gates[31];Vec3 finish_previous=add(last_gate->pos,mul(last_gate->normal,-120));game.pos=add(last_gate->pos,mul(last_gate->normal,120));race_tick(&game,finish_previous,.01f);
 FCHK(game.race.phase==RACE_WON&&!game.job_n&&game.erl.dirty,"board completion marks its paid rewards dirty without a league result screen");
 erl_ui_tick(.016f);FCHK(game.erl.dirty,"older in-flight snapshot cannot clear newer board completion state");
 race_loss_age=1;race_loss_release=1;FCHK(race_ui_input(PSP_CTRL_CROSS,0,.016f)&&game.race.phase==RACE_NONE&&game.race.job==-1,"Continue waits for the old writer and commits the final board result before leaving");erl_checkpoint_test_delay=0;
 FCHK(load_game(copy,commander_save_path(&game))&&!copy->job_n&&copy->credits==finish_cash+10000&&copy->erl_local.pending_count==1,"last committed commander contains final contract cash and exact sponsor equipment");
 FCHK(copy->erl_local.pending[0].id==game.erl_local.pending[0].id&&copy->erl_local.pending[0].power==game.erl_local.pending[0].power&&copy->erl_local.pending[0].seed==game.erl_local.pending[0].seed,"board result checkpoint preserves the awarded equipment instance and stats");
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,failures);fclose(out);fb=old;free(memory);free(copy);
 #undef FCHK
 return 1;
}
