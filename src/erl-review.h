/* Review-only: real domain actions, byte-exact saves, native renderer and inputs. */
static void erl_review_fixture(int hq){
 erl_motion_reset();game_init(&game);story_complete(&game);game.opening.stage=0;game.tutorial_step=0;mission_card_reset();game.job_n=0;game.credits=1000000;game.docked=1;game.dock_stage=0;game.planet=game.approach=-1;page=ERL_PAGE;game.voice_time=game.message_time=0;erl_screen=ERL_HOME;erl_choice=erl_confirm=0;erl_notice[0]=0;erl_anim=2;erl_result_serial=0;
 if(hq)for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_MEGA){game.system=s;system_bodies(&game);game_spawn(&game);break;}
 erl_ship_capture(&game,RGB(245,70,55));
}
static void erl_review_capture(const char *name){erl_motion_entry_age=erl_motion.focus=erl_motion.portrait=erl_motion.pulse=10;rect(0,0,W,H,EB);drawcount=0;if(game.erl.result)erl_result_draw();else if(page==ERL_PAGE)erl_console_draw();else {space();erl_pause_draw();}dump_native_bmp(name);}
static int erl_review_icon_matches(int kind,int x,int y){unsigned colors[]={0,ET,EC,ER,RGB(242,184,66),RGB(16,20,28)};for(int yy=0;yy<24;yy++)for(int xx=0;xx<32;xx++){unsigned p=erl_card_icons[kind][yy*32+xx];if(p&&fb[(y+yy)*STRIDE+x+xx]!=colors[p])return 0;}return 1;}
static int erl_review(void){
 FILE *flag=fopen("erl-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("erl-review.txt","w");if(!out)return 1;
 unsigned *oldfb=fb,*mem=calloc(STRIDE*H+32,sizeof(unsigned));Game *copy=malloc(sizeof(Game));if(!mem||!copy){fprintf(out,"RESULT 1 failures\n");fclose(out);free(mem);free(copy);return 1;}fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0x123cabfe;int failures=0;
 #define ECHK(c,label) do{int pass=!!(c);fprintf(out,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(out);}while(0)
 erl_review_fixture(0);ECHK(!erl_hq(&game),"ordinary shipyard does not become a racing headquarters");
 ECHK(erl_begin(&game,ERL_INTRO,0,ERL_SPRINT,0)&&game.race.rivals==1,"Lave mechanic test creates one actual opponent");
 int credits=game.credits;erl_finish(&game,2,0);ECHK(!game.erl.referral&&game.credits==credits,"losing the mechanic test gives no referral or units");erl_return_hq();
 ECHK(erl_begin(&game,ERL_INTRO,0,ERL_SPRINT,0),"mechanic test can be retried after a loss");erl_finish(&game,1,0);ECHK(game.erl.referral&&game.erl.ref_system>=0&&station_class_for_system(&game,game.erl.ref_system)==STATION_MEGA,"winning supplies a real Mega Capitol referral");
 credits=game.credits;erl_finish(&game,1,0);ECHK(game.credits==credits,"replayed result cannot pay the mechanic reward twice");
 erl_reveal=9.8f;erl_result_serial=game.erl.serial;erl_choice=0;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 ECHK(page==ERL_PAGE&&erl_screen==ERL_TALK&&game.system==7&&!game.erl.event_kind&&tracked_mission==TRACK_ERL,"winning Torr's test returns to his congratulations, never the league dashboard");erl_review_capture("erl-test-congratulations.bmp");
 ECHK(erl_log_active()&&!strcmp(mission_dossier_name(mission_log_count()-1),"INTRODUCTION TO THE ERL"),"referral preserves the introduction title and keeps the mission active");int referred=game.erl.ref_system;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 ECHK(page==CHART&&game.route_goal==referred,"Torr's referral action plots the real nearest reachable Mega Capitol");game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 game.system=referred;game.station_variant=0;system_bodies(&game);game.docked=1;page=HOME;row=31;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 ECHK(page==ERL_PAGE&&erl_screen==ERL_WELCOME&&!game.erl.event_kind,"first ERL HQ visit opens the organiser's welcome before showing the league desk");erl_review_capture("erl-first-hq-welcome.bmp");
 int welcome_pages=dialogue_pages;for(int n=0;n<welcome_pages;n++){erl_review_capture("erl-first-hq-welcome-page.bmp");erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);}
 ECHK(erl_screen==ERL_HOME&&(game.erl.inbox&8)&&!game.erl.event_kind,"reading the welcome reveals the rookie qualifier without starting a race");erl_review_capture("erl-first-hq-rookie.bmp");
 ECHK(load_game(&game,commander_save_path(&game))&&(game.erl.inbox&8),"completed organiser introduction persists in the existing V35 save");erl_open_hq();ECHK(erl_screen==ERL_HOME,"later headquarters visits do not repeat the first welcome");
 erl_choice=0;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_BRIEF&&!game.erl.event_kind,"rookie card opens its event brief only after the welcome");
 erl_review_fixture(0);page=MISSIONS;row=0;preview_time=.8f;rect(0,0,W,H,BG);mission_board();unsigned shine_before=fb[45*STRIDE+45];dump_native_bmp("erl-featured-board.bmp");preview_time=2.5f;mission_board();
 ECHK(mission_board_offer(0)==5&&mission_board_offer(1)==0&&mission_board_offer(5)==4,"Lave display promotes its featured introduction while retaining all six original offer IDs");
 int offers_seen[6]={0};for(int n=0;n<6;n++)offers_seen[mission_board_offer(n)]++;int unique=1;for(int n=0;n<6;n++)unique&=offers_seen[n]==1;ECHK(unique,"featured sorting cannot duplicate or hide a normal contract");
 ECHK(shine_before!=fb[45*STRIDE+45],"featured card's rainbow sheen visibly moves with time");erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(page==ERL_PAGE&&erl_screen==ERL_TALK&&!game.job_n,"top featured card opens Torr rather than accepting a different mission");
 erl_review_fixture(1);
 const int exits[]={MISSIONLOG,MISSIONS,CHART,DETAILS};int exits_ok=1;
 for(int i=0;i<4;i++){change_page(CAMPAIGN);change_page(MISSIONLOG);change_page(exits[i]);game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);exits_ok&=page==HOME&&nav_depth==0;}
 ECHK(exits_ok,"Circle exits mission log, board, galaxy map and system details straight to the dashboard and clears shortcut history");
 change_page(CHART);chart_search=1;game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);ECHK(page==HOME&&!chart_search,"Circle also leaves galaxy search without trapping the commander in shortcut history");
 erl_review_fixture(1);game.erl.referral=1;erl_begin(&game,ERL_QUALIFIER,0,ERL_CIRCUIT,1);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;page=FLIGHT;
 game.race.elapsed=12;game.race.gate=3;game.erl.save_age=5;erl_checkpoint_test_delay=600000;
 unsigned long long queued=sceKernelGetSystemTimeWide();erl_ui_tick(.016f);queued=sceKernelGetSystemTimeWide()-queued;
 ECHK(erl_checkpoint_thread>=0&&queued<40000,"five-second checkpoint queues without waiting for an injected 600ms storage delay");
 ECHK(!erl_checkpoint_queue(),"only one immutable snapshot and writer may be in flight");
 game.race.elapsed=19;game.race.gate=5;game.credits+=100;
 ECHK(erl_checkpoint_copy&&erl_checkpoint_copy->race.elapsed==12&&erl_checkpoint_copy->race.gate==3,"live racing cannot mutate the checkpoint being written");
 int joined=erl_checkpoint_collect(1);erl_checkpoint_test_delay=0;*copy=game;
 fprintf(out,"INFO checkpoint worker unused stack after save/validation: %d bytes\n",erl_checkpoint_stack_free);
 ECHK(erl_checkpoint_stack_free>=64*1024,"checkpoint save and full validation retain at least 64 KiB of worker stack");
 ECHK(joined==1&&load_game(&game,commander_save_path(copy))&&game.race.elapsed==12&&game.race.gate==3&&game.erl.paused,"background checkpoint loads with its exact snapshot and paused safe recovery");game=*copy;
 game.erl.dirty=1;erl_commit();ECHK(load_game(&game,commander_save_path(copy))&&game.race.elapsed==19&&game.race.gate==5&&game.credits==copy->credits,"later final save supersedes the older checkpoint without losing race progress or money");
 erl_review_fixture(1);ECHK(erl_hq(&game)&&deck_service_visible(31),"WORK exposes ERL only at actual Mega Capitol stations");erl_screen=ERL_TALK;erl_review_capture("erl-before-referral.bmp");erl_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);ECHK(dialogue_page==1,"R reads the next page of a long organiser message");erl_review_capture("erl-before-referral-page2.bmp");erl_input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);ECHK(dialogue_page==0,"L returns to the previous speech page without activating a reply");erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(page==MISSIONLOG&&tracked_mission==TRACK_ERL&&!game.erl.event_kind,"ineligible HQ guides the player to Torr without starting an unavailable race");page=ERL_PAGE;game.erl.referral=1;ECHK(erl_next_qualifier(&game)==1,"referral unlocks the rookie qualifier");
 ECHK(erl_begin(&game,ERL_QUALIFIER,0,ERL_CIRCUIT,1),"rookie qualifier creates a regulated circuit");ECHK(game.race.rivals==6&&game.race.laps==2&&game.race.top_speed==8000,"seven racers share a fixed performance class");erl_finish(&game,1,0);ECHK(game.erl.licence==1,"qualifier victory grants the rookie licence");ECHK(!erl_log_active(),"rookie qualification completes the introduction mission");erl_return_hq();ECHK(tracked_mission!=TRACK_ERL,"completed introduction cannot remain the tracked objective");
 snprintf(game.commander_name,sizeof(game.commander_name),"ALEXANDRA STARLIGHT");erl_review_capture("erl-home.bmp");erl_screen=ERL_EVENTS;erl_review_capture("erl-events.bmp");erl_screen=ERL_DRIVER;erl_review_capture("erl-driver.bmp");erl_screen=ERL_RIVALS;erl_review_capture("erl-rivals.bmp");erl_screen=ERL_TALK;erl_review_capture("erl-organiser.bmp");
 ECHK(erl_season_start(&game),"licenced driver enters a four-round championship");erl_screen=ERL_SEASON;erl_choice=1;erl_review_capture("erl-season-withdraw-selection.bmp");ECHK(fb[252*STRIDE+260]==ER,"season navigation visibly highlights the selected Withdraw button");erl_choice=0;erl_review_capture("erl-season.bmp");char why[96];ECHK(!erl_swap(&game,0,0,why,sizeof(why)),"championship locks the active ship");
 for(int round=0;round<4;round++){
  ECHK(erl_begin(&game,ERL_OFFICIAL,round,erl_discipline(round),0),"next official round starts in its registered headquarters");
  if(round==0){erl_screen=ERL_BRIEF;erl_review_capture("erl-brief.bmp");erl_screen=ERL_GARAGE;erl_review_capture("erl-garage-locked.bmp");game.docked=0;race_restart(&game);game.race.countdown=0;game.race.phase=RACE_RUNNING;game.race.gate=8;game.race.elapsed=27.25f;game.race.reserve=43.5f;game.race.pilot[0].gate=3;game.race.pilot[0].reserve=29;game.pos=add(game.race.gates[8].pos,mul(game.race.gates[8].normal,-11500));game.yaw=atan2f(game.race.gates[8].normal.x,game.race.gates[8].normal.z);game.pitch=asinf(game.race.gates[8].normal.y);page=FLIGHT;erl_review_capture("erl-race-hud.bmp");
   *copy=game;ECHK(save_game(&game,"erl-mid.sav"),"mid-race save writes an atomic V34 checkpoint");game_init(&game);ECHK(load_game(&game,"erl-mid.sav"),"V34 mid-race checkpoint loads");ECHK(game.erl.paused&&game.race.gate==8&&game.race.elapsed==27.25f&&game.race.reserve==43.5f&&game.race.pilot[0].gate==3,"recovery preserves clock, gates, boost and rivals and starts paused");erl_review_capture("erl-resume.bmp");
   float elapsed=game.race.elapsed;erl_input(0,0,1,0,0);ECHK(game.race.elapsed==elapsed,"paused recovery does not advance simulation");erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(!game.erl.paused&&game.race.handover==3,"resume grants a protected three-second handover");game=*copy;
  }
  erl_finish(&game,1,0);int paid=game.credits,xp=game.erl.xp;erl_finish(&game,1,0);ECHK(game.credits==paid&&game.erl.xp==xp,"official result pays and scores once");
  if(round==3){ECHK(game.erl.fleet_n==2&&game.erl.fleet[1].hull==erl_prizes[0]&&game.erl.trophies==1,"first championship permanently awards its registered Needlefish");ECHK(game.erl.points[0]==40&&!game.erl.championship,"four wins finalise standings and unlock the ship");
   erl_reveal=0.8f;erl_review_capture("erl-victory-launch.bmp");erl_reveal=2.6f;erl_review_capture("erl-victory-impact.bmp");erl_reveal=4.5f;erl_review_capture("erl-victory-rewards.bmp");erl_reveal=9.8f;erl_review_capture("erl-victory-ready.bmp");
  }erl_return_hq();
 }
 erl_screen=ERL_STANDINGS;erl_review_capture("erl-standings.bmp");erl_screen=ERL_GARAGE;erl_garage_row=1;erl_review_capture("erl-garage.bmp");
 int stored=module_store_total(),reward_slot=db_owned_free(&game);DockRoll expected_reward=game.erl_local.pending[0];ECHK(erl_claim_module(&game,module_store_capacity())&&module_store_total()==stored+1&&reward_slot>=0&&!memcmp(&game.dockbay.owned[reward_slot],&expected_reward,sizeof(DockRoll)),"championship enhanced module transfers all exact persisted stats into Item Storage");
 ECHK(!erl_claim_module(&game,module_store_capacity()),"a claimed module cannot be claimed twice");
 unsigned id=game.erl.fleet[0].id;float oldfuel=game.fuel;ECHK(erl_swap(&game,1,RGB(245,70,55),why,sizeof(why))&&game.ship==erl_prizes[0],"Garage swaps to the actual prize ship");ECHK(erl_swap(&game,0,RGB(60,130,230),why,sizeof(why))&&game.erl.fleet[0].id==id&&game.fuel==oldfuel&&game.erl.fleet[0].paint==RGB(245,70,55),"returning to the original identity restores condition, fuel and paint");
 *copy=game;game.cargo[0]=100;ECHK(!erl_swap(&game,1,0,why,sizeof(why))&&game.ship==copy->ship&&game.erl.active_ship==copy->erl.active_ship,"cargo overflow blocks swapping without modifying either ship");game=*copy;
 ECHK(erl_next_qualifier(&game)==2&&game.erl.licence==1,"XP unlocks the next qualifier without granting automatic promotion");
 credits=game.credits;int xp=game.erl.xp;ECHK(erl_begin(&game,ERL_PRACTICE,0,ERL_CIRCUIT,0),"practice accepts a licenced pilot");erl_finish(&game,1,0);ECHK(game.credits==credits&&game.erl.xp==xp,"practice awards neither money nor league XP");erl_return_hq();
 ECHK(erl_season_start(&game)&&erl_begin(&game,ERL_OFFICIAL,0,ERL_CIRCUIT,0),"a new championship can follow the first");game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.reserve=2;game.boost=1;for(int i=0;i<10;i++)game_tick(&game,.05f,0,0,1,0);ECHK(game.race.boost_lock&&!game.boost,"continuous boost exhausts reserve and requires releasing R");
 game.race.phase=RACE_RUNNING;game.race.crash_time=game.race.handover=game.race.repair=0;game.race.protection=0;game.pos=add(game.race.start,(Vec3){0,1000000,0});for(int i=0;i<31&&!game.erl.result;i++)race_tick(&game,game.pos,1);ECHK(!game.erl.result&&game.erl.round==0,"missing the route never invents an automatic off-course DNF");erl_withdraw(&game);ECHK(game.erl.result==2&&game.erl.round==1&&game.erl.places[0]==8,"explicit withdrawal records the official DNF and advances only that round");erl_reveal=9.8f;erl_review_capture("erl-loss.bmp");
 ECHK(save_game(&game,"erl-result.sav"),"pending result saves before presentation");int balance=game.credits;game_init(&game);ECHK(load_game(&game,"erl-result.sav")&&game.credits==balance&&game.erl.round==1&&game.erl.result==2,"loading a pending result preserves payment and round progression");
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;int drive_ok=1,rivals_ok=1,hold_wins=0;float fastest=999,slowest=0;
 for(int hull=0;hull<player_ship_count;hull++){
  game.ship=hull;fit_clear_all(&game);game.fit[FIT_WPN]=1;fit_rebuild(&game);game.docked=1;erl_ship_capture(&game,RGB(245,70,55));erl_begin(&game,ERL_PRACTICE,0,ERL_CIRCUIT,0);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;
  race_review_drive(0);int finished=game.race.gate==32;drive_ok&=finished;fastest=fminf(fastest,game.race.elapsed);slowest=fmaxf(slowest,game.race.elapsed);fprintf(out,"INFO hull %02d managed %.2fs gate %d place %d\n",hull,game.race.elapsed,game.race.gate,game.race.place);if(hull==0){int progressed=0;for(int i=0;i<game.race.rivals;i++)progressed+=game.race.pilot[i].gate>25;rivals_ok=progressed>=4;}
  erl_return_hq();
 }
 ECHK(drive_ok&&fastest>90&&slowest<180,"every playable hull can complete the regulated circuit in roughly two minutes");ECHK(rivals_ok,"the majority of the field physically reaches the final quarter of the course");
 for(int layout=0;layout<4;layout++){game.docked=1;erl_begin(&game,ERL_PRACTICE,layout,erl_discipline(layout),0);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;race_review_drive(1);hold_wins+=game.erl.result_place==1;fprintf(out,"INFO held boost layout %d %.2fs gate %d place %d\n",layout,game.race.elapsed,game.race.gate,game.erl.result_place);if(!game.erl.result)erl_withdraw(&game);erl_return_hq();}
 ECHK(hold_wins<4,"holding boost throughout does not guarantee every event win");

 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl_local.event_rules=ra_rules_make(RA_COMBAT,RA_TAKEDOWNS,RA_POOL_ALL,19);game.erl_local.rules_pending=1;erl_begin(&game,ERL_TRIAL,2,ERL_TARGET,0);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;
 for(int i=0;i<5;i++){RaceRival *r=&game.race.pilot[i];r->pos=add(game.race.start,mul(game.race.dir,2000+i*500.f));r->dir=game.race.dir;r->speed=1000;r->bump=r->protection=0;game.race.crash_time=game.race.handover=game.race.protection=0;game.pos=add(r->pos,mul(r->dir,-50));game.yaw=atan2f(r->dir.x,r->dir.z);game.pitch=asinf(r->dir.y);game.race.arcade.actor[0].health=RA_HEALTH;game.race.arcade.actor[0].protection=game.race.arcade.actor[i+1].protection=0;game.race.arcade.actor[i+1].health=RA_HEALTH;game.boost=1;game.speed=8000;race_tick(&game,add(r->pos,mul(r->dir,-900)),.001f);}
 ECHK(game.race.takedowns==5&&game.erl.result_place==1,"five actual ram collisions complete the timed Takedown Trial");erl_return_hq();
 game.erl_local.event_rules=ra_rules_make(RA_COMBAT,RA_TAKEDOWNS,RA_POOL_ALL,19);game.erl_local.rules_pending=1;erl_begin(&game,ERL_PRACTICE,2,ERL_RUSH,0);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.elapsed=119.95f;game.race.takedowns=3;for(int i=0;i<6;i++)game.race.pilot[i].score=2;race_tick(&game,game.pos,.1f);
 ECHK(game.erl.result==2&&game.erl.result_takedowns==3&&game.race.elapsed>=120,"timed drone trial fails honestly when its required fifth takedown is not reached");erl_return_hq();
 erl_begin(&game,ERL_PRACTICE,1,ERL_CIRCUIT,0);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.off_course=12;game.race.protection=0;race_tick(&game,game.pos,.01f);ECHK(game.race.off_course==0,"returning to the route resets the consecutive off-course timer");
 int shots=game.shots;erl_input(PSP_CTRL_SQUARE|PSP_CTRL_TRIANGLE|PSP_CTRL_CROSS,PSP_CTRL_SQUARE|PSP_CTRL_TRIANGLE|PSP_CTRL_CROSS,.016f,0,0);ECHK(page==FLIGHT&&!square_held&&game.shots==shots&&!game.shot,"targeting, comms and weapons cannot steal racing controls");
 erl_input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);float frozen=game.race.elapsed;erl_input(0,PSP_CTRL_RTRIGGER,1,1,1);ECHK(game.erl.paused&&game.race.elapsed==frozen,"Start pauses all racing simulation");erl_review_capture("erl-pause.bmp");erl_input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);ECHK(!game.erl.paused&&game.race.handover==3,"Start resumes with protected guidance");
 game.race.handover=game.race.protection=0;game.race.arcade.actor[0].protection=0;game.race.gate=6;RaceTrackFrame repair_frame=race_repair_frame(&game.race,6);RaceGate repaired=game.race.gates[6];repaired.normal=repair_frame.ahead;RaceGate *wall=&repaired;Vec3 service=repair_frame.centre;game.race.arcade.actor[0].health=600;game.pos=add(service,mul(wall->normal,3100));race_tick(&game,add(service,mul(wall->normal,-3100)),.01f);erl_review_capture("erl-repair.bmp");ECHK(game.race.arcade.actor[0].health==RA_HEALTH&&(game.race.arcade.branch_repair[0]&(1u<<6))&&!game.dead,"actual rectangular repair traversal restores temporary hull without permanent damage");
 game.race.gate=8;RaceGate *shot=&game.race.gates[8];game.pos=add(shot->pos,add(mul(shot->normal,-11500),mul(shot->up,400)));game.yaw=atan2f(shot->normal.x,shot->normal.z);game.pitch=asinf(shot->normal.y);game.race.pilot[0].pos=add(game.pos,add(mul(shot->normal,3000),mul(shot->right,-900)));game.race.pilot[0].dir=shot->normal;game.race.pilot[0].disabled=0;game.race.pilot[0].radius=150;third_person=0;erl_review_capture("erl-broadcast-flight.bmp");third_person=1;flight_camera_seen=-1;erl_review_capture("erl-broadcast-chase.bmp");third_person=0;
 for(int i=0;i<6;i++){game.race.pilot[i].disabled=0;game.race.pilot[i].dir=shot->normal;game.race.pilot[i].pos=add(game.pos,add(mul(shot->normal,900+(i/3)*1100),add(mul(shot->right,(i%3-1)*330.f),mul(shot->up,250))));}game.race.pilot[0].disabled=1;game.race.pilot[0].burst=1;game.race.pilot[0].impact=game.race.pilot[0].pos;game.race.repair=1;erl_review_capture("erl-full-field-effects.bmp");*copy=game;uint64_t begin=sceKernelGetSystemTimeWide();for(int i=0;i<90;i++){drawcount=0;space();}uint64_t duration=sceKernelGetSystemTimeWide()-begin;fprintf(out,"INFO native populated course render %.3f ms/frame in PPSSPP (hardware pending)\n",duration/90000.0);game=*copy;
 erl_withdraw(&game);erl_return_hq();
 int brands=1;char seen[256][32];for(int sys=0;sys<256;sys++){snprintf(seen[sys],32,"%s",erl_organiser(&game,sys));for(int j=0;j<sys;j++)brands&=strcmp(seen[sys],seen[j])!=0;}ECHK(brands,"all 256 systems have deterministic distinct organisers");
 erl_review_fixture(0);erl_screen=ERL_TALK;erl_review_capture("erl-mechanic.bmp");erl_screen=ERL_MECHANIC;erl_review_capture("erl-mechanic-brief.bmp");
 int bounds=1;for(int i=0;i<16;i++)bounds&=mem[i]==0x123cabfe&&mem[STRIDE*H+16+i]==0x123cabfe;ECHK(bounds,"all native screens respect framebuffer bounds");
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;
 int exclusive=1;for(int system=0;system<256;system++){game.system=system;exclusive&=!equipment_in_stock(TECH_ERL_VECTOR_GOVERNOR)&&!equipment_in_stock(TECH_ERL_COOLANT_MATRIX);}ECHK(exclusive,"exclusive modules stay out of every ordinary shop, including Lave");erl_review_fixture(1);game.erl.referral=game.erl.licence=1;
 int ladder=1,ladder_home=game.system;for(int division=0;division<4;division++){game.system=ladder_home;game.docked=1;system_bodies(&game);game_spawn(&game);erl_local_sync(&game);
  if(division){game.erl.xp=erl_thresholds[division];ladder&=erl_begin(&game,ERL_QUALIFIER,0,ERL_CIRCUIT,division+1);erl_finish(&game,1,0);erl_return_hq();}
  ladder&=erl_season_start(&game);int hull=game.ship;ladder&=!buy_ship(&game,(hull+1)%player_ship_count)&&game.ship==hull;
  for(int round=0;round<4;round++){game.system=ladder_home;game.docked=1;system_bodies(&game);game_spawn(&game);ladder&=erl_begin(&game,ERL_OFFICIAL,round,erl_discipline(round),0);erl_finish(&game,1,0);erl_return_hq();}game.system=ladder_home;game.docked=1;system_bodies(&game);game_spawn(&game);erl_local_sync(&game);
  ladder&=game.erl.fleet_n==division+2&&game.erl.fleet[division+1].hull==erl_prizes[division]&&game.erl.licence==division+1;
 }
 ECHK(ladder&&game.erl.fleet_n==5&&game.erl.prize_mask==15,"all four qualifier/season tiers award permanent stronger ships without overflowing the Garage");
 int enhanced_claims=0;while(game.erl_local.pending_count&&erl_claim_module(&game,module_store_capacity()))enhanced_claims++;ECHK(enhanced_claims==4,"all four exact enhanced championship modules can be claimed after their seasons");game.erl.module_pending=1;unsigned pending=game.erl.module_pending;ECHK(!erl_claim_module(&game,0)&&game.erl.module_pending==pending,"full Item Storage defers every exclusive module without losing its claim");
 ECHK(save_game(&game,"erl-fleet.sav"),"complete five-identity fleet writes a valid V34 save");game_init(&game);ECHK(load_game(&game,"erl-fleet.sav")&&game.erl.fleet_n==5&&game.erl.prize_mask==15&&game.erl.module_pending==pending,"all prize identities and a legacy deferred Vector Governor survive reload");
 ECHK(erl_claim_module(&game,module_store_capacity()),"a deferred legacy championship claim succeeds after storage becomes available");module_inventory_row=module_bank=0;int legacy_list[EQUIPMENT_COUNT],legacy_count=module_owned_list(legacy_list,EQUIPMENT_COUNT);for(int i=0;i<legacy_count;i++)if(legacy_list[i]==TECH_ERL_VECTOR_GOVERNOR)module_inventory_row=i;ECHK(module_swap_selected()&&fit_find(&game,TECH_ERL_VECTOR_GOVERNOR)>=0&&!game.module_store[TECH_ERL_VECTOR_GOVERNOR],"legacy exclusive prize installs through actual Item Storage without duplication");ECHK(tech_turn_multiplier(&game,1.f)>1.1f,"fitted legacy Vector Governor retains real normal-flight handling");
 int swaps=1;for(int i=1;i<5;i++){swaps&=erl_swap(&game,i,ship_paint[game.ship],why,sizeof(why));swaps&=game.ship==erl_prizes[i-1];}ECHK(swaps,"each earned prize is a usable playable ship at any headquarters");
 page=ERL_PAGE;erl_screen=ERL_RIVALS;erl_choice=0;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_RIVAL_TALK&&erl_rival_id==1,"Rivals profile action opens the selected persistent pilot's conversation");erl_review_capture("erl-rival-conversation.bmp");erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_RIVALS,"rival conversation returns to the correct paddock");

 /* Hyper Circuit uses the real renderer, real career states and unchanged controls. */
 erl_review_fixture(1);page=ERL_PAGE;game.erl.referral=game.erl.licence=1;game.erl.xp=640;snprintf(game.commander_name,25,"JAMESON");erl_screen=ERL_HOME;erl_choice=0;erl_review_capture("erl-hyper-home.bmp");
 ECHK(erl_xp_goal()==900&&erl_xp_remaining()==260&&erl_xp_fill(100)==71,"HQ XP bar represents actual earned XP and remaining XP to the next qualifier");
 ECHK(fb[186*STRIDE+257]==ET&&fb[186*STRIDE+455]!=ET,"native HQ bar lights earned segments and keeps remaining segments dim");
 unsigned builds=erl_preview_builds;uint64_t menu_begin=sceKernelGetSystemTimeWide();for(int i=0;i<90;i++)erl_console_draw();uint64_t menu_time=sceKernelGetSystemTimeWide()-menu_begin;
 ECHK(erl_preview_builds==builds,"animated HQ reuses the native race artwork rather than rebuilding the 3D world");fprintf(out,"INFO Hyper Circuit cached HQ %.3f ms/frame in PPSSPP (hardware pending)\n",menu_time/90000.0);
 game.erl.xp=899;ECHK(erl_xp_remaining()==1,"one XP short shows one remaining");game.erl.xp=900;ECHK(!erl_xp_remaining()&&erl_next_qualifier(&game)==2&&game.erl.licence==1,"full XP unlocks a qualifier without automatically promoting the driver");erl_review_capture("erl-hyper-qualifier-ready.bmp");
 game.erl.licence=4;ECHK(!erl_xp_remaining()&&erl_xp_fill(100)==100,"Elite caps the bar without inventing another level");erl_review_capture("erl-hyper-elite.bmp");game.erl.licence=1;game.erl.xp=640;
 const int screens[]={ERL_EVENTS,ERL_BRIEF,ERL_DRIVER,ERL_RIVALS,ERL_GARAGE,ERL_SEASON,ERL_STANDINGS,ERL_MECHANIC,ERL_TALK,ERL_WELCOME,ERL_RIVAL_TALK};
 const char *captures[]={"events","brief","driver","rivals","garage","season","standings","mechanic-brief","torr","welcome","rival-talk"};
 for(int i=0;i<11;i++){erl_screen=screens[i];erl_choice=0;erl_requested_kind=ERL_QUALIFIER;erl_requested_type=ERL_CIRCUIT;erl_rival_id=1;char file[80];snprintf(file,sizeof(file),"erl-hyper-%s.bmp",captures[i]);erl_review_capture(file);}
 erl_screen=ERL_SEASON;game.erl.championship=1;game.erl.round=1;erl_review_capture("erl-hyper-season-circles.bmp");
 ECHK(fb[172*STRIDE+166]==ER&&fb[134*STRIDE+166]==ET&&fb[157*STRIDE+143]==ET&&fb[134*STRIDE+143]!=ER&&fb[134*STRIDE+143]!=ET,"championship round markers are true 47px circles with a pink active round and cyan rim, not square tiles");
 int number_ok=1;const char *digits="02";for(int ch=0;ch<2;ch++)for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[digits[ch]-32][yy]&(128>>xx))for(int dy=0;dy<2;dy++)for(int dx=0;dx<2;dx++)number_ok&=fb[(150+yy*2+dy)*STRIDE+150+ch*16+xx*2+dx]==EB;
 ECHK(number_ok,"active round uses the exact game-font 02 at double size inside its circle");erl_screen=ERL_HOME;erl_review_capture("erl-hyper-home-round02.bmp");erl_screen=ERL_SEASON;
 erl_confirm=1;erl_confirm_yes=0;erl_review_capture("erl-hyper-confirm.bmp");erl_confirm=0;game.erl.championship=0;game.erl.round=0;
 erl_screen=ERL_GARAGE;game.erl.championship=1;erl_review_capture("erl-hyper-garage-locked.bmp");game.erl.championship=0;
 int nav_ok=1;const int destinations[]={ERL_EVENTS,ERL_SEASON,ERL_DRIVER,ERL_RIVALS,ERL_GARAGE};
 for(int i=0;i<5;i++){erl_screen=ERL_HOME;erl_choice=i;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);nav_ok&=erl_screen==destinations[i];erl_input(PSP_CTRL_CIRCLE,0,.016f,0,0);nav_ok&=erl_screen==ERL_HOME;}
 ECHK(nav_ok,"all five Hyper Circuit navigation buttons open the right page and Circle returns to HQ");erl_screen=ERL_EVENTS;erl_choice=0;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_EVENTS&&!game.erl.event_kind,"locked qualifier cannot start from the redesigned event control");
 unsigned saved_paint=ship_paint[game.ship];
 int brand_ok=1;unsigned backdrop_hash=0;const int paints[]={13,5,8};
 for(int p=0;p<3;p++){
  ship_paint[game.ship]=decorator_finishes[paints[p]];erl_backdrop();unsigned bh=0;for(int y=0;y<H;y++)for(int x=0;x<W;x++)bh=bh*33u+fb[y*STRIDE+x];if(p)brand_ok&=bh==backdrop_hash;else backdrop_hash=bh;
  for(int system=0;system<256;system++){int oldsystem=game.system;game.system=system;brand_ok&=EP==RGB(39,26,72)&&ET==RGB(123,255,240)&&ER==RGB(255,96,206)&&dialogue_edge()==ET;game.system=oldsystem;}
  for(int i=0;i<11;i++){erl_screen=screens[i];erl_choice=0;erl_review_capture("erl407-brand-audit.bmp");brand_ok&=EP==RGB(39,26,72)&&ET==RGB(123,255,240)&&ER==RGB(255,96,206);}
  erl_screen=ERL_HOME;char name[64];snprintf(name,sizeof(name),"erl407-hq-paint-%d.bmp",paints[p]);erl_review_capture(name);brand_ok&=fb[7*STRIDE+8]==ER;
  erl_screen=ERL_WELCOME;snprintf(name,sizeof(name),"erl407-welcome-paint-%d.bmp",paints[p]);erl_review_capture(name);brand_ok&=dialogue_edge()==ET;
 }
 ECHK(brand_ok,"all ERL screens keep magenta purple cyan branding across ship paints and all 256 systems");
 rect(0,0,100,20,0);text_px(0,0,ET,"A");int text_ok=1;for(int y=0;y<8;y++)for(int x=0;x<8;x++)if(font8['A'-32][y]&(128>>x))text_ok&=fb[y*STRIDE+x]==ET;
 ECHK(text_ok,"shared ERL dialogue text bypasses Pimp-my-Ship palette recolouring");
 erl_screen=ERL_GARAGE;erl_garage_row=game.erl.active_ship;unsigned fleetpaint=game.erl.fleet[erl_garage_row].paint;unsigned hashes[2]={0};
 for(int p=0;p<2;p++){game.erl.fleet[erl_garage_row].paint=decorator_finishes[p?5:13];char name[64];snprintf(name,sizeof(name),"erl407-garage-paint-%d.bmp",p?5:13);erl_review_capture(name);for(int y=66;y<166;y++)for(int x=96;x<384;x++)hashes[p]=hashes[p]*33u+fb[y*STRIDE+x];}
 ECHK(hashes[0]!=hashes[1],"Garage ship preview visibly retains each actual stored ship paint while ERL branding stays fixed");
 game.erl.fleet[erl_garage_row].paint=fleetpaint;ship_paint[game.ship]=saved_paint;erl_screen=ERL_HOME;
 snprintf(game.commander_name,25,"ALEXANDRA STARFARER LONG");game.erl.xp=123456;erl_review_capture("erl-hyper-long-name.bmp");
 int hyper_bounds=1;for(int i=0;i<16;i++)hyper_bounds&=mem[i]==0x123cabfe&&mem[STRIDE*H+16+i]==0x123cabfe;ECHK(hyper_bounds,"all Hyper Circuit native screens preserve framebuffer guard pixels");

 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl.championship=1;game.erl.round=1;game.erl.xp=640;snprintf(game.commander_name,25,"JAMESON");erl_screen=ERL_HOME;erl_review_capture("erl-cards-headquarters.bmp");
 ECHK(fb[148*STRIDE+20]==EP&&fb[148*STRIDE+254]==RGB(16,20,28)&&fb[252*STRIDE+24]==ER,"approved HQ separates commander and XP panels and places navigation at y240");
 int icons_ok=1;for(int kind=0;kind<9;kind++){int pixels=0;for(int i=0;i<768;i++)pixels+=erl_card_icons[kind][i]!=0;icons_ok&=pixels>30;}ECHK(icons_ok,"all nine approved icons contain actual masked pixel art");
 erl_screen=ERL_BRIEF;game.erl.championship=0;erl_requested_kind=ERL_QUALIFIER;erl_requested_layout=0;erl_requested_type=ERL_CIRCUIT;erl_review_capture("erl-cards-qualifier-brief.bmp");
 ECHK(fb[152*STRIDE+308]==RGB(16,20,28)&&fb[251*STRIDE+24]==ER,"approved brief places rewards in the right card and Start Race at y240 without a control legend");
 int subtitle_ok=1;const char *subtitle="ROOKIE QUALIFIER";for(int ch=0;subtitle[ch];ch++)for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8[subtitle[ch]-32][yy]&(128>>xx))subtitle_ok&=fb[(132+yy)*STRIDE+20+ch*8+xx]==ET;ECHK(subtitle_ok,"qualifier label is directly beneath the large course title in the left banner");
 const int brief_types[]={ERL_SPRINT,ERL_CIRCUIT,ERL_RUSH,ERL_TARGET};const char *brief_names[]={"erl-cards-sprint.bmp","erl-cards-circuit.bmp","erl-cards-rush.bmp","erl-cards-target.bmp"};for(int i=0;i<4;i++){erl_requested_kind=i==3?ERL_TRIAL:ERL_PRACTICE;erl_requested_type=brief_types[i];game.erl.event_kind=erl_requested_kind;game.race.arcade.rules=ra_rules_make(i>=2?RA_COMBAT:RA_CLEAN,i==3?RA_TAKEDOWNS:i==2?RA_ENDURANCE:RA_FINISH,i>=2?RA_POOL_ALL:RA_POOL_CLEAN,19);erl_review_capture(brief_names[i]);if(i==2)ECHK(erl_review_icon_matches(ERLI_CLOCK,112,157),"minutes metric renders its own clock rather than a lap arrow");if(i==3)ECHK(erl_review_icon_matches(ERLI_TARGET,112,157),"targets metric renders its own target rather than a lap arrow");}
 game.erl.event_kind=0;game.erl.xp=900;erl_requested_kind=0;erl_requested_type=ERL_CIRCUIT;erl_review_capture("erl-cards-club-qualifier.bmp");ECHK(erl_next_qualifier(&game)==2,"later qualifier card uses the unlocked league without changing the current licence");game.erl.xp=640;
 game.erl.module_pending=1;erl_screen=ERL_HOME;erl_review_capture("erl-cards-pending-module.bmp");ECHK(game.erl.module_pending&&module_store_total()==0,"displaying the claim action never consumes a pending module");game.erl.module_pending=0;
 int cards_bounds=1;for(int i=0;i<16;i++)cards_bounds&=mem[i]==0x123cabfe&&mem[STRIDE*H+16+i]==0x123cabfe;ECHK(cards_bounds,"all approved race-card variants stay within the native framebuffer");

 erl_review_fixture(1);game.erl.referral=1;game.erl.licence=1;game.erl.xp=640;erl_screen=ERL_EVENTS;erl_choice=0;
 erl_review_capture("erl404-events.bmp");
 ECHK(fb[213*STRIDE+12]==ER&&fb[213*STRIDE+170]==EB&&fb[213*STRIDE+328]==EB,"events use three horizontal reference cards with the locked qualifier still visibly selected");
 ECHK(erl_xp_remaining()==260&&fb[175*STRIDE+18]==ET&&fb[175*STRIDE+163]!=ET,"large XP readout and progress bar represent 640 of 900 earned XP");
 int bigxp=1;for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)if(font8['2'-32][yy]&(128>>xx))bigxp&=fb[(145+yy*2)*STRIDE+15+xx*2]==EC;
 ECHK(bigxp,"remaining XP is rendered as actual double-size font pixels");
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_EVENTS&&!game.erl.event_kind,"locked qualifier cannot start a race");
 erl_input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);erl_review_capture("erl404-events-practice.bmp");ECHK(erl_choice==1&&fb[213*STRIDE+170]==ER,"right selects the middle practice card");
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_BRIEF&&erl_requested_kind==ERL_PRACTICE,"practice opens its working race brief");
 erl_screen=ERL_EVENTS;erl_choice=0;game.erl.xp=900;erl_review_capture("erl404-events-ready.bmp");ECHK(erl_next_qualifier(&game)==2,"ready event correctly reflects the next real qualifier");
 game.erl.licence=4;erl_review_capture("erl404-events-elite.bmp");ECHK(!erl_next_qualifier(&game)&&!erl_xp_remaining(),"Elite events do not invent another XP unlock");
 game.erl.licence=1;game.erl.xp=640;erl_screen=ERL_RIVALS;int grid_ok=1,identity_ok=1;
 for(int i=0;i<6;i++){erl_choice=i;char path[48];snprintf(path,sizeof(path),"erl404-rival-%d.bmp",i+1);erl_review_capture(path);grid_ok&=fb[(202+(i/3)*25)*STRIDE+12+(i%3)*158]==ER;erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);identity_ok&=erl_screen==ERL_RIVAL_TALK&&erl_rival_id==i+1;erl_screen=ERL_RIVALS;}
 ECHK(grid_ok,"all six reference-grid rivals show the correct selected card");ECHK(identity_ok,"every rival card opens the same actual pilot identity");
 erl_choice=0;erl_input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);ECHK(erl_choice==3,"down moves one row in the three-column paddock");erl_input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);ECHK(erl_choice==0,"up returns to the same column");
 erl_review_capture("erl404-rivals.bmp");ECHK(fb[37*STRIDE+367]==ET&&fb[132*STRIDE+462]==ET&&fb[160*STRIDE+280]==ER,"reference portrait is 96px with cyan rim and separate contact action");
 int picker_bounds=1;for(int i=0;i<16;i++)picker_bounds&=mem[i]==0x123cabfe&&mem[STRIDE*H+16+i]==0x123cabfe;ECHK(picker_bounds,"all event and six rival layouts retain framebuffer guards");
 /* Exercise live phases separately from the settled-layout captures above. */
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl.xp=640;erl_screen=ERL_EVENTS;erl_choice=0;erl_motion_sync();
 ECHK(erl_motion_entry_age==0,"entering ERL starts a bounded Slipstream transition");
 erl_console_draw();dump_native_bmp("erl405-events-entry.bmp");ECHK(fb[80*STRIDE+450]==EB,"incoming transition covers only the page body with the native background");
 erl_input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);ECHK(erl_choice==1&&erl_motion.focus==0,"selection changes immediately during a page transition");
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);ECHK(erl_screen==ERL_BRIEF&&erl_requested_kind==ERL_PRACTICE,"X opens the selected event while its animation is still active");
 erl_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);ECHK(erl_screen==ERL_HOME,"Circle interrupts an incoming transition without waiting");
 erl_motion_tick(.06f);erl_console_draw();dump_native_bmp("erl405-hq-wipe.bmp");
 erl_motion_tick(.2f);ECHK(erl_motion_entry_age>=.22f,"page wipe ends after 220ms without a queue");
 erl_motion_entry_age=10;erl_motion.focus=10;erl_motion.pulse=10;erl_motion.clock=1.5f;erl_screen=ERL_EVENTS;erl_choice=0;erl_motion_sync();erl_motion_entry_age=10;erl_motion.focus=10;
 erl_input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);erl_motion_tick(.06f);erl_console_draw();dump_native_bmp("erl405-events-focus.bmp");
 ECHK(erl_motion.focus>.05f&&erl_motion.focus<.12f&&erl_motion.to.x==165,"Broadcast Neon rim targets the middle event card at its halfway phase");
 erl_motion_tick(.08f);ECHK(erl_motion.focus>=.12f,"focus animation completes in 120ms");
 erl_screen=ERL_RIVALS;erl_choice=0;erl_motion_sync();erl_motion_entry_age=10;erl_motion.clock=1.5f;erl_motion.portrait=.04f;erl_console_draw();dump_native_bmp("erl405-portrait-acquiring.bmp");
 ECHK(fb[110*STRIDE+400]==RGB(16,20,28)&&fb[37*STRIDE+367]==ET,"Paddock Signal bands stay inside the aligned portrait and retain its frame");
 unsigned dna=erl_portrait(1);erl_motion.portrait=10;erl_motion.focus=10;erl_motion.pulse=10;erl_motion.clock=1.5f;erl_console_draw_raw();unsigned portrait[96*96];for(int y=0;y<96;y++)memcpy(portrait+y*96,fb+(37+y)*STRIDE+367,96*sizeof(unsigned));erl_console_draw();int portrait_exact=1;for(int y=0;y<96;y++)portrait_exact&=!memcmp(portrait+y*96,fb+(37+y)*STRIDE+367,96*sizeof(unsigned));
 ECHK(portrait_exact&&dna==erl_portrait(1),"completed portrait reveal restores every original portrait pixel and preserves DNA");dump_native_bmp("erl405-portrait-ready.bmp");
 int identities=1;for(int i=0;i<6;i++){erl_screen=ERL_RIVALS;erl_choice=i;erl_motion_sync();erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);identities&=erl_rival_id==i+1&&erl_screen==ERL_RIVAL_TALK;erl_motion_tick(.1f);erl_console_draw();char path[64];snprintf(path,sizeof(path),"erl405-contact-%d.bmp",i+1);dump_native_bmp(path);}
 ECHK(identities,"all six animated portraits contact the actual selected rival immediately");
 ECHK(erl_motion.to.x==12&&erl_motion.to.y==154&&erl_motion.to.w==456,"conversation focus uses the actual freshly laid-out single reply");
 erl_confirm=1;erl_confirm_yes=1;ErlMotionRect yes=erl_motion_rect(101,1);erl_confirm_yes=0;ErlMotionRect no=erl_motion_rect(101,1);ECHK(yes.x==80&&no.x==244,"paused-race confirmation light follows Yes and No rather than the previous menu choice");erl_confirm=0;
 const int moving[]={ERL_HOME,ERL_EVENTS,ERL_RIVALS,ERL_SEASON,ERL_GARAGE,ERL_DRIVER,ERL_BRIEF,ERL_STANDINGS,ERL_TALK,ERL_WELCOME,ERL_RIVAL_TALK};int phases_ok=1;
 for(int i=0;i<11;i++){erl_screen=moving[i];erl_choice=0;erl_motion_reset();erl_motion_sync();for(int frame=0;frame<35;frame++){if(i<3&&frame==23)erl_input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,1.f/60,0,0);erl_motion_tick(frame?1.f/60:0);erl_console_draw();if(i<3){char film[64];snprintf(film,sizeof(film),"erl405-film-%d-%02d.bmp",i,frame);dump_native_bmp(film);}for(int j=0;j<16;j++)phases_ok&=mem[j]==0x123cabfe&&mem[STRIDE*H+16+j]==0x123cabfe;}char path[64];snprintf(path,sizeof(path),"erl405-motion-page-%02d.bmp",i);dump_native_bmp(path);}
 ECHK(phases_ok,"all eleven console pages retain framebuffer guards through every live phase");
 erl_screen=ERL_SEASON;erl_motion_sync();erl_motion.clock=-.01f;erl_console_draw();erl_motion_tick(0);ECHK(erl_motion.clock>=0,"Season route handles a negative first-frame timestamp without an invalid node index");
 int xp_before=game.erl.xp;erl_screen=ERL_DRIVER;erl_motion_sync();erl_motion_entry_age=.05f;erl_console_draw();unsigned fill_early=fb[156*STRIDE+100];erl_motion_entry_age=.5f;erl_console_draw();ECHK(game.erl.xp==xp_before&&fill_early!=fb[156*STRIDE+100],"XP reveal animates earned segments without changing saved XP");
 erl_screen=ERL_HOME;erl_motion_sync();erl_motion_entry_age=10;erl_motion.focus=10;erl_console_draw();unsigned motion_builds=erl_preview_builds;
 for(int i=0;i<90;i++){erl_input(i%2?PSP_CTRL_LEFT:PSP_CTRL_RIGHT,0,.016f,0,0);erl_motion_tick(1.f/60);erl_console_draw();}
 ECHK(erl_preview_builds==motion_builds,"rapid selection and motion reuse the cached native course instead of rebuilding it");
 double worst_overlay=0;for(int i=0;i<11;i++){erl_screen=moving[i];erl_choice=0;erl_motion_sync();erl_console_draw_raw();for(int k=0;k<20;k++){erl_motion_entry_age=(k%14)/60.f;erl_motion.focus=(k%8)/60.f;erl_motion.portrait=(k%10)/60.f;uint64_t started=sceKernelGetSystemTimeWide();erl_motion_draw();double elapsed=sceKernelGetSystemTimeWide()-started;if(elapsed>worst_overlay)worst_overlay=elapsed;}}
 fprintf(out,"INFO motion overlay worst %.3f ms across all live console phases (emulator CPU timer)\n",worst_overlay/1000);ECHK(worst_overlay<16000,"bounded animation overlay stays below one 60Hz CPU frame in the emulator");
 erl_motion_reset();
 remove("erl-mid.sav");remove("erl-mid.sav.bak");remove("erl-result.sav");remove("erl-result.sav.bak");remove("erl-fleet.sav");remove("erl-fleet.sav.bak");fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(mem);free(copy);return 1;
 #undef ECHK
}
