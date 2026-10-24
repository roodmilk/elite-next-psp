/* Opt-in integration check: the briefing must lead to an obtainable contract,
 * never a galaxy-map route back to the system the commander is already in. */
static int guild_navigation_review(void){
 FILE *flag=fopen("guild-navigation-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("guild-navigation-review.txt","w");if(!report)return 1;
 int failures=0;unsigned *saved_fb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
 if(!pixels){fprintf(report,"RESULT 1 failures\n");fclose(report);return 1;}fb=pixels;
#define GCHECK(ok,label) do{int pass=!!(ok);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(report);}while(0)
 game_init(&game);story_complete(&game);mission_queries_end();
 int boards=0,bad=0;
 for(int chapter=1;chapter<=3;chapter+=2){game.guild_chapter=chapter;
  for(int system=0;system<256;system++)for(int hub=0;hub<HUB_COUNT;hub++){
   game.system=system;game.station_variant=hub;int offer=guild_offer_index_at(&game,system,guild_required_contract(&game));
   int valid=offer>=0&&offer<MISSION_OFFER_SLOTS&&mission_type_for_offer(&game,offer)==guild_required_contract(&game)&&guild_contract_station(&game)==system;
   for(int i=0;i<MISSION_OFFER_SLOTS;i++)for(int j=0;j<i;j++)valid&=mission_type_for_offer(&game,i)!=mission_type_for_offer(&game,j);
   bad+=!valid;boards++;
  }
 }
 GCHECK(boards==256*HUB_COUNT*2&&!bad,"Guild food/rescue promises match five distinct actual offers at every system and hub");
 for(int chapter=1;chapter<=3;chapter+=2)for(int hub=0;hub<HUB_COUNT;hub++){
  game_init(&game);story_complete(&game);deck_reset();game.guild_chapter=chapter;game.guild_flags=0;game.system=7;game.station_variant=hub;game.docked=1;game.credits=20000;tracked_mission=1;change_page(CAMPAIGN);
  int offer=assignment_offer();GCHECK(offer>=0&&mission_offer_valid(&game,offer,0)&&narrative_action(GUILD)==NA_BOARD,"docked Lave Guild next step offers the validated local board, not Galaxy.Maps");
  if(chapter==1&&hub==0){rect(0,0,W,H,BG);campaign_screen();dump_native_bmp("guild-food-next-step.bmp");}
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  GCHECK(page==MISSIONS&&row==offer&&mission_type_for_offer(&game,row)==guild_required_contract(&game),"tracked briefing X opens and highlights the promised local contract");
  if(chapter==1&&hub==0){rect(0,0,W,H,BG);mission_board();dump_native_bmp("guild-food-local-board.bmp");
   input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_LEFT,0,.016f,0,0);input(PSP_CTRL_CROSS,0,.016f,0,0);
   GCHECK(game.job_n==1&&game.jobs[0].type==MISSION_DELIVERY&&game.cargo[0]>0,"highlighted food delivery accepts through confirmation and reserves real Food cargo");
   tracked_mission=1;change_page(CAMPAIGN);input(PSP_CTRL_CROSS,0,.016f,0,0);
   GCHECK(page==MISSIONLOG&&row==2&&game.job_sel==0,"accepted food job leads to its existing mission-log entry");
  }
 }
 game_init(&game);story_complete(&game);game.guild_chapter=1;game.system=7;game.docked=0;tracked_mission=1;change_page(CAMPAIGN);
 GCHECK(narrative_action(GUILD)==NA_DOCK,"in-system flight guidance requests docking instead of a same-system jump");
 input(PSP_CTRL_CROSS,0,.016f,0,0);GCHECK(page==FLIGHT&&selected_target==0&&!autoaim,"flight next step selects the local hub without opening the galaxy map or steering");
 game_init(&game);story_complete(&game);game.system=7;game.docked=1;game.guild_chapter=1;int before=assignment_offer();
 int persisted=save_game(&game,"guild-navigation-test.sav");game.guild_chapter=0;
 persisted=persisted&&load_game(&game,"guild-navigation-test.sav")&&game.guild_chapter==1&&assignment_offer()==before;remove("guild-navigation-test.sav");
 GCHECK(persisted,"existing-save Guild progress restores the same obtainable local food offer");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=saved_fb;free(pixels);
#undef GCHECK
 return 1;
}
