{
 TEST_INIT();launch(&game);selected_target=NPC_ID_MIN;game.npc[0].alive=1;game.npc[0].pos=add(game.pos,(Vec3){100,20,1000});game.yaw=game.pitch=0;autoaim=1;
 float prior_error=atan2f(100,1000);int eased=1;
 for(int frame=0;frame<60;frame++){align_target(1.f/60,0,0);float error=atan2f(100,1000)-game.yaw;eased&=error>=-.00001f&&error<=prior_error;prior_error=error;}
 INPUT_CHECK(eased&&prior_error<.0001f,"target lock: eases to bearing without overshoot or snapping");
 align_target(.016f,.5f,0);INPUT_CHECK(!autoaim,"target lock: manual steering still releases smooth alignment");
 TEST_INIT();deck_reset();change_page(HOME);int seen=0,unique=1;
 for(int g=0;g<5;g++)for(int i=0;i<deck_sizes[g];i++){int id=deck_rows[g][i];if(id<0||id>=DECK_ITEMS||(seen&(1<<id)))unique=0;else seen|=1<<id;}
  INPUT_CHECK(unique&&seen==(((1<<DECK_ITEMS)-1)&~((1<<18)|(1<<8)|(1<<19))),"deck: every active service appears once without duplicate Guild or standalone targeting entries");
 {int fly_vis[6],fly_n=deck_fill(0,fly_vis),targeting_visible=0;for(int i=0;i<fly_n;i++)if(fly_vis[i]==8)targeting_visible=1;INPUT_CHECK(!targeting_visible,"deck: standalone Targeting computer is hidden from Fly");}
 row=10;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==COMMS_PANEL,"deck: Fly Comms opens the canonical held-Triangle panel");input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==HOME&&row==10,"deck: canonical Comms returns to the Fly menu");
 row=0;
 input(PSP_CTRL_RIGHT,0,.016f,0,0);INPUT_CHECK(row==1&&deck_group(row)==1,"deck: Right moves from Fly to Ship");
 input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(row==3,"deck: Down follows the visible Ship list");
 input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&row==3,"deck: Back restores selected service and category");
 input(PSP_CTRL_RIGHT,0,.016f,0,0);input(PSP_CTRL_LEFT,0,.016f,0,0);
 INPUT_CHECK(row==3,"deck: each category remembers its focus");
 row=10;input(PSP_CTRL_CROSS,0,.016f,0,0);row=10;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==RADIO,"comms settings: audio is reachable through visible menus");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==COMMS_PANEL&&row==10,"comms settings: Radio returns to its owning screen and row");
 row=7;int previous_hud=hud_mode;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(hud_mode==(previous_hud+1)%3&&hud_hidden==(hud_mode==2),"comms settings: HUD setting changes actual flight layout");
 row=9;int previous_contrast=high_contrast;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(high_contrast!=previous_contrast,"comms settings: High contrast focus toggles visibly");
 row=12;int previous_view=third_person;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(third_person!=previous_view,"comms settings: third-person view toggles actual camera preference");
 input(PSP_CTRL_DOWN,0,.016f,0,0);INPUT_CHECK(row==0,"comms settings: final option wraps to first");
 input(PSP_CTRL_UP,0,.016f,0,0);INPUT_CHECK(row==12,"comms settings: first option wraps to final");
 row=11;input(PSP_CTRL_CROSS,0,.016f,0,0);help_tab=0;
 input(PSP_CTRL_LEFT,0,.016f,0,0);INPUT_CHECK(help_tab==4,"controls: pages wrap and remain bounded");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==COMMS_PANEL&&row==11,"controls: Back returns to Comms instead of losing location");
 TEST_INIT();launch(&game);change_page(HOME);
 {int ship_vis[6],ship_n=deck_fill(1,ship_vis),work_vis[6],work_n=deck_fill(2,work_vis),hidden=0;
  for(int i=0;i<ship_n;i++)if(ship_vis[i]==3||ship_vis[i]==4)hidden=1;
  for(int i=0;i<work_n;i++)if(work_vis[i]==12)hidden=1;
 INPUT_CHECK(!hidden&&ship_n==2&&work_n==3,"deck: Shipyard, Outfitting and Mission board hide while undocked");}
 row=3;deck_clamp_row();INPUT_CHECK(row!=3&&deck_service_visible(row),"deck: undocked focus clamps off hidden station services");
 int distinct=1;for(unsigned seed=0;seed<256;seed++)for(int role=0;role<FACTION_COUNT;role++){
  int id=faction_portrait_index(seed,role);if(id<0||id>=8||id%4!=role)distinct=0;
 }
 INPUT_CHECK(distinct,"portraits: faction always selects the correct uniform family");
 TEST_INIT();change_page(FACTIONS);row=0;faction_lore_card=0;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(faction_lore_card==1,"factions: X opens the next lore channel card");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==FACTIONS||page==FLIGHT,"factions: Triangle scans for a contact of that colour");
 TEST_INIT();launch(&game);page=FLIGHT;selected_target=BODY_COUNT+1;
 contact_speak(EXPLORERS,"Survey channel.");INPUT_CHECK(game.voice_who==VOICE_CONTACT&&game.voice_role==EXPLORERS,"identity: ordinary explorers are not Kei");
 TEST_INIT();hud_mode=hud_hidden=0;deck_reset();
}
