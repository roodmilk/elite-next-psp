{
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
 game.pos=game.rover_pos;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.rover_driving,"surface: X boards actual nearby rover");
 Vec3 before=game.pos;game.yaw=0;input(0,PSP_CTRL_UP,.05f,0,0);INPUT_CHECK(length(sub(game.pos,before))>8&&game.pos.y>=terrain_height(&game,game.pos.x,game.pos.z)+21.9f,"surface: rover moves faster while following ground");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.05f,0,0);INPUT_CHECK(game.jetpack<=0&&game.pos.y<=terrain_height(&game,game.pos.x,game.pos.z)+22.1f,"surface: rover cannot use the on-foot jetpack");
 Vec3 parked=game.pos;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(!game.rover_driving&&length(sub(parked,game.rover_pos))<1&&length(sub(game.pos,game.rover_pos))>=28,"surface: rover stays parked; commander exits clear of its collider");
 int cash=game.credits;game.pos=surface_poi(&game,0);surface_interact(&game);INPUT_CHECK(game.surface_progress[game.system][game.planet]&1,"surface: port offers a relay repair job");
 game.pos=surface_poi(&game,1);surface_interact(&game);game.pos=surface_poi(&game,0);surface_interact(&game);int paid=game.credits;surface_interact(&game);INPUT_CHECK(paid==cash+900&&game.credits==paid,"surface: complete and return job pays exactly once");
 for(int i=2;i<6;i++){game.pos=surface_poi(&game,i);surface_interact(&game);}int rewards=game.credits,ore=game.cargo[12];for(int i=2;i<6;i++){game.pos=surface_poi(&game,i);surface_interact(&game);}
 INPUT_CHECK(game.credits==rewards&&game.cargo[12]==ore,"surface: completed POIs cannot be farmed for duplicate loot");
 uint32_t flags=game.surface_progress[game.system][game.planet];game.docked=1;
 INPUT_CHECK(save_game(&game,"test-surface.sav")&&load_game(&game,"test-surface.sav")&&game.surface_progress[7][1]==flags,"surface: activity completion survives save/load");remove("test-surface.sav");remove("test-surface.sav.bak");
 TEST_INIT();
}
