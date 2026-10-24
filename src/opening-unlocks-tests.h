/* Include inside input_tests(), where INPUT_CHECK is available. */
{
 static Game unlock_saved;unlock_saved=game;int unlock_row=row;
 game_init(&game);game.opening.stage=OP_HOME;game.docked=1;
 int groups[DECK_GROUPS],vis[9];
 INPUT_CHECK(opening_unlocked_services(&game)==0&&deck_visible_groups(groups)==0,"Home scene has no premature services or empty category tabs");
 game.opening.stage=OP_FIRST_SAVE;
 INPUT_CHECK(opening_unlocked_services(&game)==(UINT32_C(1)<<5)&&deck_visible_groups(groups)==1&&groups[0]==4,"First save reveals only Commander and Save status");
 game.opening.stage=OP_RADIO;
 INPUT_CHECK(opening_menu_available(&game,5)&&opening_menu_available(&game,16)&&!opening_menu_available(&game,0)&&!opening_menu_available(&game,11),"Radio introduction retains Save without exposing flight or maps early");
 game.opening.stage=OP_SOL_MAP;
 INPUT_CHECK(deck_visible_groups(groups)==2&&!opening_menu_available(&game,2)&&!opening_menu_available(&game,7)&&!opening_menu_available(&game,22),"Sol reveals flight and System Details while keeping galaxy factions lore absent");
 game.opening.stage=OP_EARTH_NET;
 INPUT_CHECK(deck_visible_groups(groups)==3&&opening_menu_available(&game,14)&&!opening_menu_available(&game,15),"Earth GalacticNet introduces Discover category without Discovery Codex");
 game.opening.stage=OP_LOADOUT;
 INPUT_CHECK(opening_menu_available(&game,21)&&!opening_menu_available(&game,3)&&!opening_menu_available(&game,4)&&!opening_menu_available(&game,1),"Loadout lesson does not prematurely reveal Shipyard Outfitting or Cargo");
 game.opening.stage=OP_REPAIR;
 INPUT_CHECK(opening_menu_available(&game,24)&&opening_menu_available(&game,10)&&!opening_menu_available(&game,3)&&!opening_menu_available(&game,4)&&!opening_menu_available(&game,1),"Engineers introduction preserves earned Comms and hides future shops");
 game.opening.stage=OP_MAP_INTRO;
 INPUT_CHECK(!opening_menu_available(&game,2),"Galaxy Map remains absent until its actual reveal beat");
 game.opening.stage=OP_GALAXY;
 INPUT_CHECK(opening_menu_available(&game,2)&&!opening_menu_available(&game,23)&&!opening_menu_available(&game,26),"Galaxy reveal does not expose future app or television");
 game.opening.stage=OP_APP;game.opening.flags|=OF_APP;
 INPUT_CHECK(opening_menu_available(&game,23)&&!opening_menu_available(&game,26),"Advertisement installation reveals app before the TV introduction");
 game.opening.stage=OP_NEWS;
 INPUT_CHECK(opening_menu_available(&game,26)&&!deck_service_visible(27)&&!deck_service_visible(28)&&!deck_service_visible(29)&&!opening_menu_available(&game,9),"News tutorial keeps online cards and debug menus absent");
 game.opening.stage=OP_FREE;
 INPUT_CHECK(opening_menu_available(&game,15)&&opening_menu_available(&game,6)&&!opening_menu_available(&game,7)&&!opening_menu_available(&game,22),"Freeplay exposes conventional services while Factions and Lore still await evidence");
 game.opening.flags|=OF_FACTIONS|OF_LORE;
 INPUT_CHECK(opening_menu_available(&game,7)&&opening_menu_available(&game,22),"Later evidence unlocks Factions and Galactic Lore");
 game.opening.stage=0;
 INPUT_CHECK(opening_unlocked_services(&game)==OPENING_ALL_SERVICES&&deck_fill(4,vis)==5,"Legacy commanders retain their original deck services");
 game=unlock_saved;row=unlock_row;
}
