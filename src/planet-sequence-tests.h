/* Real input, not just model calls: repeated boarding must select departure. */
{
 TEST_INIT();launch(&game);page=FLIGHT;
 int cycles=1,centered=1,garage=1;
 for(int cycle=0;cycle<3;cycle++){
  game.approach=1;input(PSP_CTRL_CROSS,0,.016f,0,0);
  cycles&=planet_entry_body==1;
  for(int j=0;j<340&&planet_transfer_busy();j++)input(0,0,.05f,0,0);
  Vec3 pad=surface_site(&game,1);
  centered&=fabsf(game.ship_pos.x-pad.x)<.01f&&fabsf(game.ship_pos.z-pad.z)<.01f;
  garage&=fabsf(game.rover_pos.x-pad.x-FIELD_GARAGE_X)<.01f&&fabsf(game.rover_pos.z-pad.z+10)<.01f;
  cycles&=game.surface==2&&!game.planet_sequence;
  input(0,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
  cycles&=game.surface==1;for(int j=0;j<24&&planet_seat.active;j++)input(0,0,.05f,0,0);input(0,0,.016f,0,0);input(PSP_CTRL_RTRIGGER,0,.016f,0,0);
  cycles&=game.planet_sequence==3;
  for(int j=0;j<215&&planet_transfer_busy();j++)input(0,0,.05f,0,0);
  cycles&=game.planet<0&&!game.planet_sequence;input(0,0,.016f,0,0);
 }
 INPUT_CHECK(centered,"planet landing: guidance centres parked ship on the pad");
 INPUT_CHECK(garage,"planet landing: rover remains in its garage");
 INPUT_CHECK(cycles,"planet departure: three automatic arrival/walk/Triangle-board/R-launch cycles reach orbit");
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
 /* A clear stretch outside the ship/garage isolates the control gesture. */
 Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,0,-200});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=3.14159265f;
 input(0,0,.05f,0,0);Vec3 start=game.pos;input(0,PSP_CTRL_UP,.05f,0,0);float walking=length(sub(game.pos,start));
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.05f,0,0);for(int j=0;j<5;j++)input(0,PSP_CTRL_RTRIGGER,.05f,0,0);
 start=game.pos;input(0,PSP_CTRL_RTRIGGER|PSP_CTRL_UP,.05f,0,0);float running=length(sub(game.pos,start));
 INPUT_CHECK(game.eva_running&&running>walking*1.5f&&fabsf(game.pos.y-terrain_height(&game,game.pos.x,game.pos.z)-22)<.01f,"EVA run: held R increases walking pace without a jump");
 input(0,0,.05f,0,0);INPUT_CHECK(!game.eva_running&&game.jetpack<=0,"EVA run: releasing a long hold does not trigger a jump");
 int labels=1;for(int id=0;id<10;id++){char name[64],expected[12];surface_target_name(id+1,name,sizeof(name));if(id==6)labels&=!strcmp(name,"ROVER");else{snprintf(expected,sizeof(expected),"%d ",field_nav_number(id));labels&=!strncmp(name,expected,strlen(expected));}}
 INPUT_CHECK(labels&&field_nav_number(0)==1&&field_nav_number(5)==6&&field_nav_number(7)==7&&field_nav_number(9)==9,"surface navigation: all numbered compass POIs match computer labels; rover has its own identity");
 TEST_INIT();
}
