{
 const char *notices[]={"Move within 1000m.","The sun has no landing approach.","Not enough fuel.","Dock first.","Target outside missile range."};
 for(int i=0;i<5;i++){
  TEST_INIT();launch(&game);page=FLIGHT;game.encounter_kind=ENCOUNTER_PIRATE;game.encounter=8;speak(&game,VOICE_CONTACT,"Old transmission.");message(&game,notices[i]);
  INPUT_CHECK(!incoming_reply_ready(),"computer: status notice never offers conversation over stale encounter");
  input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
  INPUT_CHECK(page==FLIGHT&&!comms_quick&&game.message_time<=0,"computer: Triangle dismisses notice without reply choices");
 }
 TEST_INIT();launch(&game);page=FLIGHT;game.encounter=0;game.voice_time=0;game.message_time=0;
 game.npc[0].alive=1;game.npc[0].role=TRADERS;game.npc[0].freighter=0;game.npc[0].target=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,1200});
 night_npc=-1;night_wait=480;night_seen=0;night_tick(479);
 INPUT_CHECK(night_npc<0,"quiet watch: no early unsolicited conversation");
 game.attacked=4;night_tick(2);INPUT_CHECK(night_npc<0,"quiet watch: combat suppresses social hails");game.attacked=0;night_tick(2);
 INPUT_CHECK(night_ready(),"quiet watch: peaceful flight eventually offers nearby civilian conversation");night_close();
 for(int story=0;story<3;story++){
  game.message_time=0;night_begin(0,story);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
  INPUT_CHECK(night_open&&page==COMMS_PANEL,"quiet watch: Triangle opens actual caller dialogue");
  row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(night_stage==1,"quiet watch: invitation leads to authored story");
  input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(night_stage==2,"quiet watch: contextual response leads to authored answer");
  input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==FLIGHT&&night_npc<0&&night_wait>=600,"quiet watch: signoff returns to flight with long cooldown");
 }
 game.message_time=0;night_begin(0,0);night_open=1;change_page(COMMS_PANEL);
 int credits_before=game.credits;night_reply(1);
 INPUT_CHECK(night_stage==3&&game.trader_offer_active&&game.credits==credits_before,"quiet watch: asking about trade only quotes an offer");
 int need=game.trader_offer_need,reward=game.trader_offer_reward,qty=game.trader_offer_qty;memset(game.cargo,0,sizeof(game.cargo));
 night_reply(0);INPUT_CHECK(game.trader_offer_active&&game.cargo[reward]==0,"quiet watch: missing cargo cannot produce free goods");
 game.cargo[need]=qty;night_reply(0);INPUT_CHECK(!game.trader_offer_active&&game.cargo[reward]==qty&&night_stage==4,"quiet watch: explicit agreement exchanges cargo once");
 night_reply(0);INPUT_CHECK(page==FLIGHT&&game.cargo[reward]==qty,"quiet watch: farewell cannot duplicate reward");
 night_npc=-1;night_open=0;night_wait=480;night_seen=0;TEST_INIT();
}
