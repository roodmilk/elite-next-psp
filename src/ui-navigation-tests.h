{
 TEST_INIT();deck_reset();change_page(HOME);int seen=0,unique=1;
 for(int g=0;g<5;g++)for(int i=0;i<deck_sizes[g];i++){int id=deck_rows[g][i];if(id<0||id>=DECK_ITEMS||(seen&(1<<id)))unique=0;else seen|=1<<id;}
 INPUT_CHECK(unique&&seen==((1<<DECK_ITEMS)-1-(1<<18)-(1<<10)),"deck: every visible service appears once; Guild work lives in Mission Log and docking lives in Comms");
 row=8;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==TARGETING&&target_count>0,"deck: Targeting computer opens the same target HUD used in flight");input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME&&row==8,"deck: leaving the targeting computer restores its Fly selection");row=0;
 input(PSP_CTRL_RIGHT,0,.016f,0,0);INPUT_CHECK(row==1&&deck_group(row)==1,"deck: Right moves from Fly to Ship");
 input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(row==3,"deck: Down follows the visible Ship list");
 input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&row==3,"deck: Back restores selected service and category");
 input(PSP_CTRL_RIGHT,0,.016f,0,0);input(PSP_CTRL_LEFT,0,.016f,0,0);
 INPUT_CHECK(row==3,"deck: each category remembers its focus");
 row=19;input(PSP_CTRL_CROSS,0,.016f,0,0);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==RADIO,"comfort: audio is reachable through visible menus");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==COMFORT&&row==3,"comfort: Radio returns to its owning screen and row");
 row=0;int previous_hud=hud_mode;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(hud_mode==(previous_hud+1)%3&&hud_hidden==(hud_mode==2),"comfort: HUD setting changes actual flight layout");
 row=2;int previous_contrast=high_contrast;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(high_contrast!=previous_contrast,"comfort: High contrast focus toggles visibly");
 row=4;input(PSP_CTRL_CROSS,0,.016f,0,0);help_tab=0;
 input(PSP_CTRL_LEFT,0,.016f,0,0);INPUT_CHECK(help_tab==3,"controls: pages wrap and remain bounded");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==COMFORT&&row==4,"controls: Back returns to comfort instead of losing location");
 TEST_INIT();launch(&game);change_page(HOME);row=3;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&row==3&&strstr(game.message,"Dock"),"deck: locked station service explains why without moving focus");
 int distinct=1;for(unsigned seed=0;seed<256;seed++)for(int role=0;role<FACTION_COUNT;role++){
  int id=faction_portrait_index(seed,role);if(id<0||id>=8||id%4!=role)distinct=0;
 }
 INPUT_CHECK(distinct,"portraits: faction always selects the correct uniform family");
 TEST_INIT();launch(&game);page=FLIGHT;selected_target=BODY_COUNT+1;
 contact_speak(EXPLORERS,"Survey channel.");INPUT_CHECK(game.voice_who==VOICE_CONTACT&&game.voice_role==EXPLORERS,"identity: ordinary explorers are not Kei");
 TEST_INIT();hud_mode=hud_hidden=0;deck_reset();
}
