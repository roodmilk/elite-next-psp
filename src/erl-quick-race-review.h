/* Review only. Native title/input/countdown/pause/result regression for the
 * disposable Race 1 shortcut. This stages result actions with erl_finish;
 * it does not claim a completed lap or the tutorial/HQ/beacon player route.
 * The isolated runner must hash seeded commander/tutorial/paint files and
 * their backup/temp variants before/after.
 * No save_game, paint_save or commander-file creation is called here. */
#if defined(ELITE_ENABLE_REVIEW) || defined(ELITE_ENCOUNTER_REVIEW)
static void erl_quick_review_capture(const char *path){
 rect(0,0,W,H,BG);drawcount=0;
 if(page==INTRO)intro_screen();else {space();erl_result_draw();erl_pause_draw();}
 dump_native_bmp(path);
}
static int erl_quick_review_enter(void){
 /* A real release clears the title's X latch after returning from a race. */
 input(0,0,.016f,0,0);
 for(int i=0;i<4&&intro_choice!=3;i++)input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);
 if(page!=INTRO||intro_choice!=3)return 0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);if(!erl_racer_picker)return 0;input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);return erl_quick_race&&page==FLIGHT;
}
static void erl_quick_review_run(void){
 input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 for(int i=0;i<300&&erl_quick_race&&!game.erl.result;i++){input(0,0,1.f/60,0,0);erl_ui_tick(1.f/60);}
}
static void erl_quick_review_loss(void){
 /* Domain result fixture: native result controls remain untouched. */
 erl_finish(&game,2,0);input(0,0,.016f,0,0);
}
static int erl_quick_race_review(void){
 FILE *flag=fopen("erl-quick-race-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("erl-quick-race-review.txt","w");if(!out)return 1;
 int failures=0,checks=0;
 #define QRCHK(condition,label) do{int ok=!!(condition);checks++;failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);fflush(out);}while(0)
 #define QRRESTORED(label) do { \
  QRCHK(!memcmp(&game,original,sizeof(Game)),label " restores original Game bytes"); \
  QRCHK(!memcmp(ship_paint,baseline_paint,sizeof(baseline_paint)),label " restores all ship paint values"); \
  QRCHK(third_person==baseline_camera&&commander_active_slot==baseline_slot,label " restores camera and keeps active commander slot"); \
  QRCHK(page==INTRO&&!erl_quick_race&&!game.erl.event_kind&&!game.erl.result&&!race_is_active(&game),label " leaves a clean title without a retained quick event"); \
 }while(0)
 unsigned *oldfb=fb,*pixels=calloc(STRIDE*H,sizeof(unsigned));Game *original=malloc(sizeof(Game));
 unsigned old_paint[sizeof(ship_paint)/sizeof(ship_paint[0])],baseline_paint[sizeof(ship_paint)/sizeof(ship_paint[0])];
 memcpy(old_paint,ship_paint,sizeof(old_paint));int old_camera=third_person,old_slot=commander_active_slot;
 if(!pixels||!original){fprintf(out,"RESULT 1 failures / review allocation\n");fclose(out);free(pixels);free(original);return 1;}
 fb=pixels;
 QRCHK(erl_checkpoint_thread<0&&!erl_quick_race,"review begins without a live checkpoint writer or existing quick session");
 if(erl_checkpoint_thread>=0||erl_quick_race)goto finish;
 /* A nontrivial in-memory career snapshot, with no active racing event.
  * Title lifecycle transients are already zero before snapshot comparison. */
 game_init(&game);story_complete(&game);game.opening.stage=0;game.tutorial_step=0;game.job_n=0;
 opening_frontend_reset();game.credits=732510;game.cargo[0]=3;game.fuel=2.25f;game.hull=67;game.energy=31;game.heat=18;
 game.erl.referral=1;game.erl.licence=2;game.erl.xp=137;game.erl.event_kind=game.erl.result=game.erl.paused=game.erl.resume=0;
 game.race.phase=RACE_NONE;game.boost=0;game.cue=SFX_NONE;game.voice_time=game.message_time=0;
 snprintf(game.commander_name,sizeof(game.commander_name),"CAREER SNAPSHOT");
 int fer_de_lance=-1;for(int i=0;i<player_ship_count;i++)if(!strcmp(player_ships[i].name,"FER DE LANCE"))fer_de_lance=i;
 QRCHK(fer_de_lance>=0,"the selectable Fer-de-Lance is registered");if(fer_de_lance<0)goto finish;
 ship_paint[fer_de_lance]=RGB(37,111,153);third_person=0;commander_active_slot=2;
 change_page(INTRO);intro_time=40;intro_menu_age=1;intro_wait_release=0;intro_choice=0;
 /* The four-row route requires a valid saved profile. The approved no-save
  * menu now starts at New Commander and deliberately hides Continue. */
 intro_profile_ready=1;memset(profile_info,0,sizeof(profile_info));profile_info[0].state=1;snprintf(profile_info[0].name,sizeof(profile_info[0].name),"CAREER SNAPSHOT");profile_info[0].ship=0;
 unsigned queued=erl_checkpoint_queued,completed=erl_checkpoint_completed;
 int baseline_camera=third_person,baseline_slot=commander_active_slot;
 memcpy(baseline_paint,ship_paint,sizeof(baseline_paint));
 input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);QRCHK(page==INTRO&&intro_choice==3,"title Up wraps from row 0 to Extras row 3");
 input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);QRCHK(page==INTRO&&intro_choice==0,"title Down wraps from Extras to row 0");
 int seen=1;for(int i=0;i<4;i++){input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);if(intro_choice>=0&&intro_choice<4)seen|=1<<intro_choice;}
 QRCHK(seen==15&&intro_choice==0&&page==INTRO,"real title navigation visits all four main rows exactly");
 input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);QRCHK(intro_extras&&intro_extras_choice==0,"Extras opens on First Light Tutorial");input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);erl_quick_review_capture("quick-title-race1.bmp");
 game.cue=SFX_NONE;game.voice_time=game.message_time=0;game.boost=0;*original=game;
 QRCHK(intro_extras&&intro_extras_choice==1,"real Extras input selects the Race 1 test before launch");if(!intro_extras||intro_extras_choice!=1)goto finish;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(erl_racer_picker&&page==INTRO&&!erl_quick_race,"title X opens the racer selection before launching");
 input(0,0,.016f,0,0);input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);int wraps=erl_racer_cursor==11;
 input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);wraps&=erl_racer_cursor==0;
 input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);wraps&=erl_racer_cursor==1;input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);wraps&=erl_racer_cursor==0;
 QRCHK(wraps,"all four D-pad directions select racers and wrap across the twelve-entry roster");
 input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(erl_quick_race&&page==FLIGHT&&game.race.phase==RACE_COUNTDOWN&&game.race.countdown>4,"confirmed racer enters actual flight with the full READY countdown");
 QRCHK(!erl_racer_picker&&erl_racer_id(&game,0)==11,"chosen Razorback is the actual race vehicle");
 if(!erl_quick_race)goto finish;
 QRCHK(game.ship==fer_de_lance&&third_person==1&&!game.docked&&!race_start_blocker(&game),"quick entry fits the sleek selectable Fer-de-Lance and enters legal undocked chase view");
 QRCHK(game.race.layout==5&&game.erl.event_kind==ERL_PRACTICE,"quick entry uses the real editable Race 1 as disposable practice");
 QRCHK(game.race.rivals==14&&game.race.laps==2&&game.race.arcade.rules.field==15&&game.race.arcade.rules.laps==2,"quick practice has fifteen racers and two laps");
 QRCHK(game.race.arcade.rules.mode==RA_COMBAT&&game.race.arcade.rules.objective==RA_FINISH&&game.race.arcade.rules.pool==RA_POOL_ALL&&ra_rules_valid(&game.race.arcade.rules,game.erl.licence),"Race 1 practice uses valid combat finish rules and all four pickups");
 QRCHK(game.fit[FIT_WPN]==1&&commander_active_slot==baseline_slot,"quick Fer-de-Lance has the intended fitting and does not switch commander slots");
 erl_quick_review_capture("quick-ready.bmp");
 input(0,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(game.race.phase==RACE_COUNTDOWN&&game.race.countdown>4,"holding the title launch X cannot skip the READY stage");
 input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(game.race.phase==RACE_COUNTDOWN&&fabsf(game.race.countdown-4)<.001f,"a fresh released X advances READY to the real four-second countdown");
 erl_quick_review_capture("quick-countdown-4.bmp");
 game.erl.save_age=6;game.erl.dirty=1;erl_ui_tick(.05f);
 QRCHK(erl_checkpoint_queued==queued&&erl_checkpoint_completed==completed&&erl_checkpoint_thread<0,"save_age 6 cannot queue or complete a quick-race autosave");
 QRCHK(!erl_checkpoint_queue()&&erl_checkpoint_thread<0&&erl_checkpoint_queued==queued,"explicit async checkpoint requests are blocked inside quick practice");
 QRCHK(erl_commit()&&erl_checkpoint_thread<0&&erl_checkpoint_queued==queued&&erl_checkpoint_completed==completed,"quick commit returns without checkpoint activity; external save hashes are required");
 int digits=1<<4;
 for(int i=0;i<300&&erl_quick_race&&!game.erl.result;i++){
  input(0,0,1.f/60,0,0);erl_ui_tick(1.f/60);
  if(game.race.phase==RACE_COUNTDOWN){int digit=(int)ceilf(game.race.countdown);
   if(digit>=1&&digit<=4&&!(digits&(1<<digit))){char name[48];snprintf(name,sizeof(name),"quick-countdown-%d.bmp",digit);erl_quick_review_capture(name);digits|=1<<digit;}}
 }
 QRCHK(digits==30,"native diagnostic observes every 4 3 2 1 countdown stage");
 QRCHK(game.race.phase==RACE_RUNNING&&game.race.elapsed>0&&!game.erl.result,"300 real input ticks advance countdown into running gameplay");
 QRCHK(erl_checkpoint_queued==queued&&erl_checkpoint_completed==completed&&erl_checkpoint_thread<0,"running input and UI ticks retain the no-checkpoint guard");
 erl_quick_review_capture("quick-running.bmp");
 float paused_elapsed=game.race.elapsed;int paused_gate=game.race.gate;
 input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);
 QRCHK(game.erl.paused&&page==FLIGHT,"Start opens the actual quick-race pause menu");erl_quick_review_capture("quick-pause.bmp");
 for(int i=0;i<20;i++){input(0,0,.05f,0,0);erl_ui_tick(.05f);}
 QRCHK(game.erl.paused&&game.race.elapsed==paused_elapsed&&game.race.gate==paused_gate,"pause input ticks preserve exact clock and checkpoint progress");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 QRCHK(!game.erl.paused&&game.race.handover==0,"Circle resumes quick racing with the direct control and protection");
 input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(!game.erl.paused&&erl_quick_race,"Resume X keeps the quick session active");
 input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);
 QRCHK(game.erl.paused&&erl_choice==1,"real Right input selects Main Menu in quick pause");erl_quick_review_capture("quick-pause-main-menu.bmp");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRRESTORED("Pause Main Menu");
 if(erl_quick_race||page!=INTRO||memcmp(&game,original,sizeof(Game)))goto finish;
 QRCHK(!erl_replay.active&&!apex_fx.valid,"leaving quick practice clears transient replay and engine history");

 QRCHK(erl_quick_review_enter(),"the title button can launch a second independent quick attempt");if(!erl_quick_race)goto finish;
 erl_quick_review_run();erl_quick_review_loss();
 QRCHK(game.erl.result&&game.erl.result_place==2&&game.race.phase==RACE_LOST,"domain-staged loss opens the real practice result state");
 QRCHK(!erl_replay.active&&erl_replay.played==game.erl.serial&&erl_reveal<.1f,"race finish skips the automatic replay and immediately starts the position flyby");
 erl_reveal=3.2f;erl_quick_review_capture("quick-result-place-2-flyby.bmp");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(erl_quick_race&&game.erl.result&&erl_reveal>=SW_READY,"early result X reveals actions without immediately retrying");erl_quick_review_capture("quick-result-loss.bmp");
 input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 QRCHK(erl_quick_race&&!game.erl.result&&page==FLIGHT&&game.race.phase==RACE_COUNTDOWN&&game.race.countdown>4,"result Race Again starts a fresh real countdown within the same disposable session");
 erl_quick_review_capture("quick-retry-ready.bmp");
 erl_quick_review_run();erl_quick_review_loss();input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);
 QRCHK(erl_quick_race&&game.erl.result&&erl_choice==1,"real result Right selects Main Menu after the reveal gate");erl_quick_review_capture("quick-result-main-menu.bmp");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);QRRESTORED("Result Main Menu after retry");
 if(erl_quick_race||page!=INTRO||memcmp(&game,original,sizeof(Game)))goto finish;

 QRCHK(erl_quick_review_enter(),"a third title entry remains available after a result and retry");if(!erl_quick_race)goto finish;
 erl_quick_review_run();erl_quick_review_loss();input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 QRRESTORED("Result Circle");
 if(erl_quick_race||page!=INTRO||memcmp(&game,original,sizeof(Game)))goto finish;
 int repeated=1;
 for(int i=0;i<3;i++){
  if(!erl_quick_review_enter()){repeated=0;break;}
  input(0,0,.016f,0,0);input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
  repeated&=page==INTRO&&!erl_quick_race&&!memcmp(&game,original,sizeof(Game))&&!memcmp(ship_paint,baseline_paint,sizeof(baseline_paint))&&third_person==baseline_camera&&commander_active_slot==baseline_slot&&!game.erl.event_kind;
  if(!repeated)break;
 }
 QRCHK(repeated,"three more real title/pause exits repeatedly restore the same original commander and presentation state");
 input(0,0,.016f,0,0);erl_ui_tick(6);
 QRCHK(page==INTRO&&!erl_quick_race&&!game.erl.event_kind&&!game.erl.result,"title release and UI ticks cannot reactivate the discarded quick event");
 QRCHK(erl_checkpoint_queued==queued&&erl_checkpoint_completed==completed&&erl_checkpoint_thread<0,"all quick entry retry result and exit paths leave checkpoint counters unchanged");
 erl_quick_review_capture("quick-title-restored.bmp");
 fprintf(out,"INFO fixture=automated_native_input result_losses=domain_staged capture=480x272 no_FPS_claim\n");
 fprintf(out,"EXTERNAL_SAVE_GUARD compare all seeded commander/tutorial/paint primary backup and temp hashes before/after\n");
finish:
 if(erl_quick_race)erl_quick_race_leave();
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,failures);fclose(out);
 fb=oldfb;memcpy(ship_paint,old_paint,sizeof(old_paint));third_person=old_camera;commander_active_slot=old_slot;
 free(pixels);free(original);
 #undef QRRESTORED
 #undef QRCHK
 return 1;
}
#endif
