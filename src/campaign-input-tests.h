{
 TEST_INIT();change_page(HOME);row=17;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN,"campaign UI: visible Story menu opens the next step");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&!game.campaign_stage,"campaign UI: Circle stays locked until the conversation ends");
 input(PSP_CTRL_SELECT,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&!game.campaign_stage,"campaign UI: Select stays locked until the conversation ends");
 for(int i=0;i<PROLOGUE_BRIEF_BEATS-1;i++){
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  INPUT_CHECK(!game.campaign_stage&&page==CAMPAIGN,"campaign UI: early Cross advances dialogue without accepting");
 }
 INPUT_CHECK(game.campaign_choice==2&&prologue_brief_beat==PROLOGUE_BRIEF_BEATS-1,"campaign UI: linear beats record Ryn context before accept");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.campaign_stage==1&&page==CAMPAIGN,"campaign UI: final Cross accepts the first flight");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page!=CAMPAIGN||!prologue_brief_locked(),"campaign UI: Circle unlocks after accept");
 change_page(CAMPAIGN);
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&game.docked&&game.shots==0,"campaign UI: accepted mission briefing has no hidden launch action");
 change_page(CAMPAIGN);float flight=game.campaign_distance;
 input(0,0,.05f,0,0);
 INPUT_CHECK(game.campaign_distance==flight,"campaign UI: reading does not advance flight distance");
 game.dead=1;change_page(FLIGHT);int cash=game.credits;
 input(PSP_CTRL_START,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&!game.dead&&game.docked&&game.credits==cash,"campaign UI: Start recovers training instead of deleting commander");
 TEST_INIT();change_page(CAMPAIGN);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
 INPUT_CHECK(!game.campaign_stage&&page==CAMPAIGN,"story: unadvertised Triangle does not accept a mission");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(prologue_brief_beat==1&&!game.campaign_stage,"story: Cross advances the locked prologue beat");
 game.campaign_stage=5;game.docked=1;row=0;cash=game.credits;
 INPUT_CHECK(narrative_action(CAMPAIGN)==NA_REWARD,"story: ready report offers a reward");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.campaign_stage==6&&game.credits==cash+1000&&narrative_action(CAMPAIGN)==NA_ASSIGNMENTS,"story: collecting once replaces the reward action");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&game.credits==cash+1000,"story: completed chapter has no hidden Guild-menu action or duplicate repayment");
 /* Open Channel brief: six locked beats, Select/Circle blocked, then accept reinforces objective. */
 saga_brief_beat=0;saga_brief_chapter=game.saga_chapter;row=0;
 INPUT_CHECK(saga_brief_locked()&&!game.saga_step,"saga brief: chapter opens locked before accept");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&!game.saga_step,"saga brief: Circle blocked mid-conversation");
 input(PSP_CTRL_SELECT,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&!game.saga_step,"saga brief: Select blocked mid-conversation");
 for(int i=0;i<SAGA_BRIEF_BEATS-1;i++){input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(!game.saga_step,"saga brief: Cross walks beats without starting the objective");}
 INPUT_CHECK(saga_brief_beat==SAGA_BRIEF_BEATS-1,"saga brief: final beat is the reinforce / accept step");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.saga_step==1&&!saga_brief_locked(),"saga brief: accept sets the next mission step and unlocks exit");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.saga_step==1&&page==CHART,"saga brief: after accept, Cross sets course instead of replaying dialogue");
 change_page(CAMPAIGN);game.guild_chapter=0;game.guild_flags=0;row=0;
 INPUT_CHECK(narrative_action(GUILD)==NA_FLY,"assignments: unfinished flight offers Launch instead of Claim");
 game.guild_flags=GUILD_LAUNCH|GUILD_DOCK;
 INPUT_CHECK(narrative_action(GUILD)==NA_REWARD,"assignments: completed docked work offers Claim");
 game.docked=0;INPUT_CHECK(narrative_action(GUILD)==NA_DOCK,"assignments: earned reward in flight directs player to dock");
 game.docked=1;game.guild_chapter=4;tracked_mission=1;
 INPUT_CHECK(narrative_action(GUILD)==NA_DECK,"assignments: completed set never advertises another reward");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN,"assignments: completed tracked briefing has no hidden menu action");
 TEST_INIT();change_page(CAMPAIGN);game.system=0;row=0;
 INPUT_CHECK(narrative_action(CAMPAIGN)==NA_ROUTE,"story: new commander outside Lave gets navigation rather than an unusable acceptance");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==CHART&&!game.campaign_stage,"story: route action opens map without starting remote training");
 TEST_INIT();game.guild_chapter=1;game.guild_flags=0;game.credits=20000;
 change_page(GUILD);INPUT_CHECK(narrative_action(GUILD)==NA_BOARD,"assignments: food delivery opens the matching board offer");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==MISSIONS&&mission_type_for_offer(&game,row)==MISSION_DELIVERY,"assignments: board highlights the required delivery");
 TEST_INIT();game.guild_chapter=1;game.guild_flags=0;game.docked=1;int no_food=-1;for(int s=0;s<256;s++)if(guild_offer_index_at(&game,s,MISSION_DELIVERY)<0){no_food=s;break;}game.system=no_food;int food_hub=guild_contract_station(&game);INPUT_CHECK(no_food>=0&&food_hub!=no_food&&guild_offer_index_at(&game,food_hub,MISSION_DELIVERY)>=0,"assignments: missing local food job resolves to a station that really offers one");change_page(GUILD);INPUT_CHECK(narrative_action(GUILD)==NA_MAP&&strstr(guild_objective(&game),game.systems[food_hub].name),"assignments: next step names the verified food-contract station");input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==CHART&&game.destination>=0,"assignments: route action charts the verified contract station");
 TEST_INIT();game.guild_chapter=1;game.guild_flags=0;game.credits=20000;int local_food=assignment_offer();INPUT_CHECK(local_food>=0&&accept_mission(&game,local_food),"assignments: verified local food offer can be accepted");change_page(GUILD);INPUT_CHECK(narrative_action(GUILD)==NA_LOG,"assignments: accepted delivery changes next step to Track");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==MISSIONLOG&&game.jobs[row].type==MISSION_DELIVERY,"assignments: Track opens the existing contract");
 TEST_INIT();change_page(STORY);int lesson=story_home_row(&game);input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&row==lesson,"guide: next lesson focuses the correct visible service");
 char icon=0;INPUT_CHECK(!footer_token("NEXT",4,&icon)&&!footer_token("STORY",5,&icon)&&footer_token("X",1,&icon)&&icon=='X'&&footer_token("SQ",2,&icon)&&icon=='S',"footer: complete button tokens only, including Square abbreviation");
 int variants=0,consistent=1,sun_fams=0;
 for(int sys=0;sys<256;sys++){game.system=sys;system_bodies(&game);
  {unsigned seed=body_art_seed(sys,0);if(seed!=game.bodies[0].seed)consistent=0;sun_fams|=1<<sun_family(seed);}
  for(int b=1;b<BODY_COUNT;b++){unsigned seed=art_hash((sys+1)*911u+b*65537u);if(seed!=game.bodies[b].seed)consistent=0;
   int id=planet_sprite_index(seed,game.bodies[b].type);if(id<0||id>=8)consistent=0;else variants|=1<<id;
  }
 }
 INPUT_CHECK(consistent&&variants==255,"planet sprites: all world seeds match chart identity and cover eight art families");
 INPUT_CHECK(sun_fams==255,"sun sprites: every system sun family appears across the galaxy");
 TEST_INIT();
}
