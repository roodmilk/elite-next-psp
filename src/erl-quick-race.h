/* Candidate title shortcut. Real course/controls, disposable practice career.
 * Keep one bounded heap snapshot; never add Game/save fields or a static Game. */
static Game *erl_quick_original;
static unsigned erl_quick_paint;
static int erl_quick_ship,erl_quick_camera;
static __attribute__((noinline,optimize("Os"))) void erl_quick_race_leave(void){
 if(!erl_quick_race)return;
 /* The no-write guard remains set until all quick state is discarded. */
 erl_checkpoint_collect(1);memset(&erl_replay,0,sizeof(erl_replay));apex_fx.valid=0;
 ship_paint[erl_quick_ship]=erl_quick_paint;third_person=erl_quick_camera;
 if(erl_quick_original){game=*erl_quick_original;free(erl_quick_original);erl_quick_original=NULL;}
 opening_frontend_reset();erl_choice=erl_confirm=erl_board_brief=0;erl_result_serial=0;
 erl_quick_race=erl_quick_race_release=0;flight_camera_seen=-1;
 change_page(INTRO);
}
static __attribute__((noinline,optimize("Os"))) int erl_quick_race_start(void){
 if(!erl_quick_race){
  /* Finish legitimate writes before replacing the live commander. */
  erl_checkpoint_collect(1);erl_quick_original=malloc(sizeof(Game));
  if(!erl_quick_original){message(&game,"Race 1 needs more free memory. Please retry.");return 0;}
  *erl_quick_original=game;erl_quick_camera=third_person;erl_quick_ship=0;
  for(int i=0;i<player_ship_count;i++)if(!strcmp(player_ships[i].name,"FER DE LANCE")){erl_quick_ship=i;break;}
  erl_quick_paint=ship_paint[erl_quick_ship];erl_quick_race=1;
 }
 game_init(&game);story_complete(&game);game.opening.stage=0;game.tutorial_step=0;game.job_n=0;
 opening_frontend_reset();night_close();game.ship=erl_quick_ship;erl_racer_select(&game,erl_quick_racer_choice);
 fit_clear_all(&game);game.fit[FIT_WPN]=1;fit_rebuild(&game);
 ship_paint[game.ship]=RGB(76,169,193);
 for(int system=0;system<256;system++)if(station_class_for_system(&game,system)==STATION_MEGA){game.system=system;system_bodies(&game);game_spawn(&game);break;}
 game.docked=1;game.dock_stage=0;game.planet=game.approach=-1;game.erl.referral=game.erl.licence=1;
 erl_ship_capture(&game,ship_paint[game.ship]);
 game.erl_local.event_rules=ra_rules_make(RA_COMBAT,RA_FINISH,RA_POOL_ALL,station_profile_hash(game.system,813));
 game.erl_local.event_rules.field=15;game.erl_local.rules_pending=1;
 if(!erl_begin(&game,ERL_PRACTICE,0,ERL_CIRCUIT,0)){erl_quick_race_leave();return 0;}
 game.docked=0;race_restart(&game);memset(&erl_replay,0,sizeof(erl_replay));apex_fx.valid=0;
 erl_choice=erl_confirm=erl_board_brief=0;erl_result_serial=0;erl_reveal=0;erl_rtap=10;
 erl_notice[0]=0;game.voice_time=game.message_time=0;third_person=1;flight_camera_seen=-1;
 selected_target=RACE_TARGET_ID;tracked_mission=0;paused=0;erl_quick_race_release=1;
 change_page(FLIGHT);return 1;
}
