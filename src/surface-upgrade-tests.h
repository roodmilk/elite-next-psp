{
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
 input(0,0,.016f,0,0);int scans=game.discoveries;Vec3 before=game.pos;
 input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
 for(int i=0;i<15;i++)input(0,PSP_CTRL_SQUARE,.016f,0,0);
 INPUT_CHECK(surface_target_open&&game.discoveries==scans,"surface visor: hold opens without scanning");
 input(PSP_CTRL_DOWN,PSP_CTRL_SQUARE|PSP_CTRL_DOWN,.016f,0,0);
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);
 INPUT_CHECK(surface_target_lock==1&&surface_nav_poi==0&&length(sub(before,game.pos))<.1f&&!game.boost,"surface visor: R tracks port; D-pad does not walk; R does not jet");
 INPUT_CHECK(surface_turn_active,"surface visor: tracking starts a smooth turn instead of snapping");
 Vec3 turnGoal=sub(surface_target_position(surface_target_lock),game.pos);float turnYaw=atan2f(turnGoal.x,turnGoal.z);
 game.yaw=turnYaw-3.10f;float prior=fabsf(atan2f(sinf(turnYaw-game.yaw),cosf(turnYaw-game.yaw)));
 surface_target_turn(.016f);float after=fabsf(atan2f(sinf(turnYaw-game.yaw),cosf(turnYaw-game.yaw)));
 INPUT_CHECK(after<prior&&after>1,"surface visor: turn advances gradually across a large bearing difference");
 for(int step=0;step<32;step++)input(0,PSP_CTRL_SQUARE,.016f,0,0);
 INPUT_CHECK(dot(forward(&game),norm(sub(surface_poi(&game,0),game.pos)))>.999f,"surface visor: R turns the camera toward the selected POI");
 input(0,PSP_CTRL_RTRIGGER,.016f,0,0);INPUT_CHECK(fabsf(game.pos.y-before.y)<.01f&&game.jetpack<=0,"surface visor: keeping R held while closing the computer does not jump");
 input(0,0,.016f,0,0);INPUT_CHECK(!surface_target_open&&game.discoveries==scans,"surface visor: releasing a hold does not scan");
 INPUT_CHECK(surface_target_lock==1,"surface visor: tracked contact remains selected after closing");
 game.yaw=0;surface_target_lock=-1;surface_target_cycle_front();int first_target=surface_target_lock;surface_target_cycle_front();
 INPUT_CHECK(first_target>=0&&camera(&game,surface_target_position(first_target)).z>15,"surface visor: Square tap chooses a visible target in front");
 INPUT_CHECK(surface_target_lock>=0&&surface_target_lock!=first_target&&camera(&game,surface_target_position(surface_target_lock)).z>15,"surface visor: repeated Square taps cycle forward-view targets");
 surface_target_cat=1;surface_target_row=0;int ids[19],count=surface_target_list(ids);
 if(count){int slot=ids[0]-11;game.life[slot].pos=game.pos;surface_target_input(PSP_CTRL_CIRCLE);INPUT_CHECK(game.life[slot].scanned&&field_species_logged(&game,game.system,game.planet,slot),"surface visor: Circle scans selected species and persists its record");input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==CODEX&&codex_scope==ATLAS_RECORD&&atlas_entry==slot&&codex_system==game.system&&codex_body==game.planet+1,"surface visor: Triangle opens the exact scanned Codex record");change_page(FLIGHT);}
 game.world_clock=543.25f;game.docked=1;INPUT_CHECK(save_game(&game,"test-clock.sav")&&load_game(&game,"test-clock.sav")&&game.world_clock==543.25f,"surface clock: exact V18 save/load round trip");remove("test-clock.sav");remove("test-clock.sav.bak");
 surface_target_open=0;surface_target_lock=-1;surface_target_cat=surface_target_row=0;TEST_INIT();
}
