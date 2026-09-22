{
 TEST_INIT();change_page(HOME);row=17;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN,"campaign UI: visible Story menu opens the next step");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&!game.campaign_stage,"campaign UI: Circle stays locked until the conversation ends");
 input(PSP_CTRL_SELECT,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&!game.campaign_stage,"campaign UI: Select stays locked until the conversation ends");
 /* Ask-then-answer: each non-final beat needs Cross to speak, then Cross to hear Kei. */
 for(int i=0;i<PROLOGUE_BRIEF_BEATS-1;i++){
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  INPUT_CHECK(prologue_brief_echo==1&&prologue_brief_beat==i&&!game.campaign_stage,"campaign UI: Cross speaks the ask before Kei answers");
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  INPUT_CHECK(prologue_brief_echo==0&&prologue_brief_beat==i+1&&!game.campaign_stage&&page==CAMPAIGN,"campaign UI: second Cross reveals Kei's answer on the next beat");
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
 INPUT_CHECK(prologue_brief_echo==1&&prologue_brief_beat==0&&!game.campaign_stage,"story: Cross speaks the first ask without advancing Kei's answer");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(prologue_brief_beat==1&&!prologue_brief_echo&&!game.campaign_stage,"story: second Cross advances to Kei's answer beat");
 game.campaign_stage=5;game.docked=1;row=0;cash=game.credits;
 INPUT_CHECK(narrative_action(CAMPAIGN)==NA_REWARD,"story: ready report offers a reward");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.campaign_stage==6&&game.credits==cash+1000&&narrative_action(CAMPAIGN)==NA_ASSIGNMENTS,"story: collecting once replaces the reward action");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==CAMPAIGN&&game.credits==cash+1000,"story: completed chapter has no hidden Guild-menu action or duplicate repayment");
 /* Open Channel brief: ask echo then answer for each beat, Select/Circle blocked, then accept. */
 saga_brief_beat=0;saga_brief_echo=0;saga_brief_chapter=game.saga_chapter;row=0;
 INPUT_CHECK(saga_brief_locked()&&!game.saga_step,"saga brief: chapter opens locked before accept");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&!game.saga_step,"saga brief: Circle blocked mid-conversation");
 input(PSP_CTRL_SELECT,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&!game.saga_step,"saga brief: Select blocked mid-conversation");
 for(int i=0;i<SAGA_BRIEF_BEATS-1;i++){
  input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(saga_brief_echo==1&&!game.saga_step,"saga brief: Cross speaks before the next NPC line");
  input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(saga_brief_echo==0&&saga_brief_beat==i+1&&!game.saga_step,"saga brief: second Cross walks to the next NPC beat");
 }
 INPUT_CHECK(saga_brief_beat==SAGA_BRIEF_BEATS-1,"saga brief: final beat is the reinforce / accept step");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.saga_step==1&&!saga_brief_locked(),"saga brief: accept sets the next mission step and unlocks exit");
 INPUT_CHECK(SAGA_BRIEF_BEATS==8&&strstr(saga_beats[0].ask5,"Mara")&&strstr(saga_beats[0].talk6,"sealed"),"saga brief: Act I eight-beat page script answers the Mara ask");
 INPUT_CHECK(strstr(saga_beats[0].talk7,"flying")&&strstr(saga_beats[0].ask8,"Accept"),"saga brief: Act I page script ends on accept after depth beats");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(game.saga_step==1&&page==CHART,"saga brief: after accept, Cross sets course instead of replaying dialogue");
 /* Act I completion coda locks Story until dismissed. */
 change_page(CAMPAIGN);game.docked=1;int saga_port=game.saga_dest;game.system=saga_port;saga_dock_event(&game);game.system=7;game.saga_dest=7;row=0;
 INPUT_CHECK(saga_ready(&game),"saga coda: delivery chapter is ready at destination");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(saga_coda_pending==0&&game.saga_chapter==1&&saga_coda_locked(),"saga coda: Act I chapter complete opens a locked coda page");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==CAMPAIGN&&saga_coda_pending==0,"saga coda: Circle stays locked on the coda page");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(saga_coda_pending<0&&!saga_coda_locked()&&saga_brief_locked(),"saga coda: Cross dismisses coda and unlocks the next brief");
 INPUT_CHECK(strstr(saga_choice_blurb(5,0),"pirates")||strstr(saga_choice_blurb(5,0),"Pirates")||strstr(saga_choice_blurb(5,0),"Loud"),"saga choice: consequence blurbs stay on the decision screen");
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
 /* Explicit ask-before-answer content check: catch question never shares the screen with its answer. */
 TEST_INIT();change_page(CAMPAIGN);
 INPUT_CHECK(!strstr(prologue_brief_line1(0),"catch")&&strstr(prologue_brief_reply(0),"catch"),"chat flow: beat 0 asks about the catch before Kei answers it");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(prologue_brief_echo&&strstr(prologue_brief_reply(0),"catch"),"chat flow: first Cross shows the commander ask about the catch");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(!prologue_brief_echo&&prologue_brief_beat==1&&strstr(prologue_brief_line1(1),"catch"),"chat flow: Kei's catch answer arrives only after the ask");
 /* Prose quality: chapter asks are authored, and first beats are full sentences. */
 INPUT_CHECK(strstr(saga_beats[0].ask1,"holding")&&strlen(saga_beats[0].line)>60,"script: chapter 02 opens with a long Kei sentence and a holding ask");
 INPUT_CHECK(strstr(saga_beats[0].talk2,"you")&&strstr(saga_beats[0].ask1,"holding"),"script: chapter 02 beat 1 answers who holds the case");
 INPUT_CHECK(strstr(saga_beats[10].ask1,"alive")&&strstr(saga_beats[10].line,"alive"),"script: Ryn reunion keeps living voice on both sides");
 INPUT_CHECK(strstr(saga_beats[10].talk2,"Yes")||strstr(saga_beats[10].talk2,"yes")||strstr(saga_beats[10].talk2,"geometry"),"script: Ryn beat 1 answers the alive ask");
 INPUT_CHECK((strstr(saga_choice_reaction(5,0),"loud")||strstr(saga_choice_reaction(5,0),"Loud")||strstr(saga_choice_reaction(5,0),"Brave"))&&strstr(saga_close_line(0),"Case"),"script: choice reactions and chapter closers stay character-voiced");
 INPUT_CHECK(strstr(saga_choice_blurb(5,0),"pirates")&&strstr(saga_choice_blurb(5,2),"die"),"script: Silence blurbs name pirate knives and delay cost");
 INPUT_CHECK(strstr(prologue_brief_reply(3),"Walk me through")&&strstr(prologue_brief_reply(4),"ready when you are"),"script: prologue asks stay spoken, not menu chrome");
 INPUT_CHECK(strstr(saga_coda_line1(0),"evidence")&&strstr(saga_coda_line2(1),"sky"),"script: Act I codas keep screenplay aftermath voice");
 INPUT_CHECK(strstr(saga_beats[5].line,"flinch")&&strstr(saga_beats[6].line,"Clinic"),"script: Act I Sable staging and Act II Mara clinic open from screenplay");
 INPUT_CHECK(strstr(saga_beats[10].ask2,"silent")&&strstr(saga_beats[10].talk3,"ghost")&&saga_has_coda(11),"script: Ryn reunion asks why she vanished; Act II coda range reaches Carry Home");
 INPUT_CHECK(strstr(saga_choice_blurb(11,0),"Broadcast")||strstr(saga_choice_blurb(11,0),"board")||strstr(saga_choice_blurb(11,0),"Clinics"),"script: Carry Home blurbs name the broadcast stakes");
 INPUT_CHECK(strstr(saga_beats[12].line,"Three copies")&&strstr(saga_beats[13].line,"does not need your map")&&saga_has_coda(17),"script: Act III map/Federal open from screenplay; codas reach No Easy Flag");
 INPUT_CHECK(strstr(saga_beats[16].line,"curiosity without munitions")&&strstr(saga_choice_blurb(17,2),"Watchers"),"script: Coldest Signal and Flag blurbs keep screenplay stakes");
 INPUT_CHECK(strstr(saga_beats[18].talk2,"Pale Meridian")&&strstr(saga_beats[21].talk4,"No pursuit")&&strstr(saga_beats[22].talk5,"Tone"),"script: Act IV ask-then-answer pairs answer the prior ask");
 INPUT_CHECK(!strstr(saga_beats[10].ask3,"silent"),"script: reunion asks stay ordered — coords after silence");
 INPUT_CHECK(strstr(saga_beats[16].talk7,"Weapons stay cold")||strstr(saga_beats[16].talk7,"weapons stay cold"),"script: Coldest confirm ask gets a direct yes");
 INPUT_CHECK(strstr(saga_beats[20].talk7,"Crews first")||strstr(saga_beats[20].talk7,"crews first"),"script: Open Channel confirm ask gets crews-first yes");
 INPUT_CHECK(strstr(saga_choice_blurb(22,0),"inspection")&&strstr(saga_choice_reaction(22,1),"method"),"script: Who Keeps Light blurbs/reactions match screenplay custody stakes");
 INPUT_CHECK(strstr(prologue_brief_line1(0),"Ryn")&&strstr(prologue_brief_line2(0),"three"),"script: prologue hook still names Ryn and the three missed calls");
 {
  Game echo;game_init(&echo);echo.campaign_stage=6;echo.saga_trust[0]=2;echo.saga_flags=4;echo.saga_chapter=19;
  INPUT_CHECK(strstr(saga_epilogue_line(&echo),"noisy")&&strstr(saga_trust_helper(&echo),"Tamsin"),"script: epilogue and helpers speak berth-six decision echoes");
  INPUT_CHECK(saga_galnet_desk(&echo)&&strstr(saga_galnet_desk(&echo),"Coalition")&&saga_galnet_kei(&echo)&&strstr(saga_galnet_kei(&echo),"Lane first"),"script: GalNet desk/Kei posts colour from Open Channel flags");
  INPUT_CHECK(saga_galnet_iona(&echo)&&strstr(saga_galnet_iona(&echo),"Suspend")&&saga_galnet_freighter(&echo)&&strstr(saga_galnet_freighter(&echo),"Tenders"),"script: GalNet Iona/freighter posts colour from Flag trust");
  echo.saga_flags=1;echo.saga_trust[0]=0;echo.saga_trust[3]=1;
  INPUT_CHECK(saga_galnet_mira(&echo)&&strstr(saga_galnet_mira(&echo),"chart dump")&&saga_galnet_sable(&echo)&&strstr(saga_galnet_sable(&echo),"corridor"),"script: GalNet Mira/Sable posts colour from Silence + Independent");
  echo.system=0;INPUT_CHECK(mission_type_for_offer(&echo,0)==MISSION_DELIVERY&&strstr(mission_brief(&echo,0),"Protein"),"script: mission briefs use authored Vol II Hungry Pad bank");
  INPUT_CHECK(!!strstr(guild_line(&echo,0),"Borrowed hulls"),"script: Guild opening line keeps manuscript expanded dialogue");
  INPUT_CHECK(!!strstr(faction_lore_tag(TRADERS),"Margins")&&!!strstr(faction_lore_line(LAW,0,0),"Patrols"),"script: faction lore tags stay distinct voices without YOU chrome");
  INPUT_CHECK(gazette_wants_tabloid(0,0)&&!!strstr(gazette_tabloid_author(0,0),"CITY")&&!!strstr(gazette_dek(0,0),"Sources"),"script: Gazette tabloid masthead and dek banks stay authored");
 }
 TEST_INIT();
}
