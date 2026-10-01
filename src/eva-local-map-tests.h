/* EVA map owns no save data: these checks cover visit reset, walked reveal,
 * modal input and bounded rendering using the real surface functions. */
{
 TEST_INIT();game.system=7;system_bodies(&game);game.docked=0;game.planet=1;game.surface=2;
 Vec3 pad=surface_site(&game,1);game.ship_pos=pad;game.rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,0,-10});
 game.pos=pad;game.pos.y=terrain_height(&game,pad.x,pad.z)+22;page=FLIGHT;planet_controls_ready=1;
 eva_map_leave();eva_map_visit();int first=0;for(int i=0;i<EVA_MAP_WORDS;i++)first+=__builtin_popcount(eva_map_seen[i]);
 INPUT_CHECK(eva_map_active&&first>1&&first<80,"EVA map: a new visit reveals only a small area around the walked position");
 game.pos.x+=720;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;eva_map_visit();
 int walked=0;for(int i=0;i<EVA_MAP_WORDS;i++)walked+=__builtin_popcount(eva_map_seen[i]);
 INPUT_CHECK(walked>first&&walked<140,"EVA map: moving across real terrain expands fog without revealing the whole field");
 input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);INPUT_CHECK(eva_map_open,"EVA map: Start opens the local survey map on foot");
 Vec3 held=game.pos;input(PSP_CTRL_UP,PSP_CTRL_UP,.05f,0,0);
 INPUT_CHECK(game.pos.x==held.x&&game.pos.z==held.z,"EVA map: modal map blocks movement beneath it");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(!eva_map_open&&page==FLIGHT,"EVA map: Circle closes directly back to EVA");
 high_contrast=1;eva_map_open_now();rect(0,0,W,H,RGB(1,2,3));eva_map_draw();
 INPUT_CHECK(fb[32*STRIDE+8]!=RGB(1,2,3)&&fb[205*STRIDE+178]!=RGB(1,2,3),"EVA map: fixed native panel and map remain inside the 480x272 frame");
 high_contrast=0;eva_map_open=0;game.surface=1;eva_map_visit();
 INPUT_CHECK(eva_map_active&&!eva_map_open,"EVA map: boarding preserves the current landing's chart");
 game.surface=2;eva_map_visit();int retained=0;for(int i=0;i<EVA_MAP_WORDS;i++)retained+=__builtin_popcount(eva_map_seen[i]);
 INPUT_CHECK(retained==walked,"EVA map: stepping outside again retains explored cells");
 game.planet=-1;game.surface=0;eva_map_visit();
 INPUT_CHECK(!eva_map_active&&!eva_map_open,"EVA map: leaving the planet ends the visit");
}
