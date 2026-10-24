{
 TEST_INIT();commander_active_slot=0;change_page(STATUS);
 {unsigned a=portrait_seeded(77,TRADERS,-1),b=portrait_seeded(77,TRADERS,-1),c=portrait_seeded(78,TRADERS,-1);INPUT_CHECK(a==b&&a!=c,"portraits: identity seed is deterministic and varied");INPUT_CHECK(portrait_valid(a)&&portrait_version(a)==PORTRAIT_DNA_VERSION,"portraits: DNA is self-versioned for future extensions");unsigned computer=portrait_ship_computer();INPUT_CHECK(portrait_species(computer)==PORTRAIT_ROBOT,"portraits: ship computer has a persistent robot identity");}
 int fixed_slot=profile_slot;profile_input(PSP_CTRL_RIGHT);INPUT_CHECK(profile_slot==fixed_slot,"profile: in-game screen cannot change save slot");
 row=1;profile_input(PSP_CTRL_CROSS);snprintf(profile_draft,25,"NOVA REED");profile_input(PSP_CTRL_START);
 INPUT_CHECK(!strcmp(game.commander_name,"NOVA REED")&&!profile_mode,"profile: controller name editor applies commander name");
 row=1;profile_input(PSP_CTRL_CROSS);profile_draft[0]=0;profile_input(PSP_CTRL_START);INPUT_CHECK(profile_mode&&!strcmp(game.commander_name,"NOVA REED"),"profile: blank names cannot replace identity");profile_input(PSP_CTRL_CIRCLE);
 game.credits=2345;game.bounty_claimed[7]=3;row=0;profile_input(PSP_CTRL_CROSS);if(profile_confirm)profile_input(PSP_CTRL_CROSS);
 INPUT_CHECK(profile_info[0].state==1&&!strcmp(profile_info[0].name,"NOVA REED"),"profile: active record stores the named commander");
 profile_slot=1;profile_input(PSP_CTRL_LEFT);INPUT_CHECK(profile_slot==commander_active_slot,"profile: direction input cannot silently switch records");profile_slot=0;
 change_page(INTRO);intro_menu_age=1;intro_wait_release=0;intro_choice=2;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(page==STATUS&&profile_from_intro,"profile: title-screen load keeps the three save cards");profile_slot=0;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(profile_confirm==2,"profile: title load requires confirmation");input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&game.credits==2345&&!strcmp(game.commander_name,"NOVA REED")&&game.bounty_claimed[7]==3,"profile: title load restores identity, money and progress");
 change_page(STATUS);game.opening.stage=OP_CREATE;row=3;profile_input(PSP_CTRL_CROSS);INPUT_CHECK(profile_portrait_edit,"profile: new-commander flow retains portrait creation");profile_input(PSP_CTRL_CIRCLE);game.opening.stage=OP_FREE;
 profile_open_load();profile_slot=0;input(PSP_CTRL_SQUARE,0,.016f,0,0);INPUT_CHECK(profile_confirm==3,"profile: title-screen delete remains confirmed");input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 profile_slot=0;profile_delete();commander_active_slot=0;profile_from_intro=profile_confirm=profile_mode=profile_portrait_edit=0;TEST_INIT();
}
