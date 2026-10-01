{
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);page=FLIGHT;
 FILE *flag=fopen("eva-capture.flag","r");int capture=flag!=NULL;if(flag)fclose(flag);
 int restored=1;
 for(int kind=1;kind<=3;kind++)for(int frame=0;frame<3;frame++){
  game.surface=kind==2?1:0;planet_sequence_begin(kind);game.planet_sequence_time=kind==3?.6f+frame*1.6f:.3f+frame*.7f;
  Vec3 p=game.pos;float yaw=game.yaw,pitch=game.pitch,roll=game.roll;int mode=game.surface;
  planet_sequence_view();
  restored&=length(sub(p,game.pos))<.001f&&yaw==game.yaw&&pitch==game.pitch&&roll==game.roll&&mode==game.surface;
  if(capture){char file[64];snprintf(file,sizeof(file),"planet-sequence-%d-%d.bmp",kind,frame);dump_native_bmp(file);}
 }
 INPUT_CHECK(restored,"planet cinematics: every camera phase preserves actual ship position and control state");
 game.planet_sequence=0;game.surface=1;eva_toggle(&game);
 game.pos=add(game.rover_pos,(Vec3){0,0,100});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 for(int id=-1;id<=6;id+=7){surface_nav_poi=id;Vec3 goal=id<0?game.ship_pos:game.rover_pos;game.yaw=atan2f(goal.x-game.pos.x,goal.z-game.pos.z);game.pitch=0;game.voice_time=game.message_time=0;drawcount=0;space();if(capture)dump_native_bmp(id<0?"compass-ship.bmp":"compass-rover-garage.bmp");}
 game.pos=add(surface_site(&game,1),(Vec3){-220,30,-100});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+30;Vec3 garage=add(surface_site(&game,1),(Vec3){-110,27,-20});Vec3 gd=sub(garage,game.pos);game.yaw=atan2f(gd.x,gd.z);game.pitch=0;drawcount=0;space();if(capture)dump_native_bmp("garage-wall-sign.bmp");
 Vec3 traffic=surface_sky_ship(0),fartraffic=surface_sky_ship(4);game.world_clock+=10;INPUT_CHECK(length(sub(traffic,surface_sky_ship(0)))>40&&length(sub(traffic,fartraffic))>500,"sky traffic: animated world positions span near and distant lanes");
 surface_nav_poi=-1;
 board_planet(&game);game.pos=game.ship_pos;game.surface=1;planet_controls_ready=0;
 planet_boarded_view();if(capture)dump_native_bmp("planet-boarded-release.bmp");
 planet_controls_ready=1;planet_boarded_view();if(capture)dump_native_bmp("planet-boarded-actions.bmp");
 game.surface=0;planet_landing_menu=1;planet_landing_prompt_view();if(capture)dump_native_bmp("planet-landing-confirm.bmp");
 TEST_INIT();
}
