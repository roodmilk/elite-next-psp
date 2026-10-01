{
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
 /* Deliberately reverse draw order: a nearer sprite must win both ways. */
 surface_y_min=28;surface_y_max=239;memset(surface_depth,255,sizeof(surface_depth));surface_depth_on=1;surface_light=1;
 surface_pixel(200,100,30,RED);surface_pixel(200,100,100,WHITE);
 INPUT_CHECK(pixels[100*STRIDE+200]==RED,"surface depth: farther objects cannot paint over a nearer sprite");
 surface_pixel(200,100,20,UI_CYAN);INPUT_CHECK(pixels[100*STRIDE+200]==UI_CYAN,"surface depth: nearer geometry covers a farther sprite");
 surface_depth_on=0;
 {surface_target_lock=-1;surface_target_open=1;surface_target_cat=surface_target_row=0;rect(0,0,W,H,RGB(7,11,19));surface_target_hud();int clear=1;for(int y=36;y<216;y++)for(int x=200;x<W;x++)if(pixels[y*STRIDE+x]!=RGB(7,11,19))clear=0;INPUT_CHECK(clear,"surface visor: left panel leaves centre/right world view untouched");surface_target_open=0;}
 FILE *flag=fopen("eva-capture.flag","r");if(flag){fclose(flag);
 TEST_INIT();launch(&game);game.approach=1;enter_planet(&game);game.pos=surface_site(&game,1);game.speed=8;land_planet(&game);eva_toggle(&game);page=FLIGHT;
 Vec3 pad=surface_site(&game,1);game.pos=(Vec3){pad.x,pad.y,pad.z-340};game.yaw=0;game.pitch=.1f;game.message_time=game.voice_time=0;
 drawcount=0;space();dump_native_bmp("spaceport-day.bmp");
 unsigned h=field_seed(game.system,game.planet);static const int days[]={600,900,1200,1800};
 game.world_clock=days[(h>>24)&3]*8.f/24;drawcount=0;space();dump_native_bmp("spaceport-sunset.bmp");
 game.world_clock=days[(h>>24)&3]*13.f/24;drawcount=0;space();dump_native_bmp("spaceport-night.bmp");
 game.world_clock=0;game.pos=add(game.rover_pos,(Vec3){35,0,-45});game.yaw=atan2f(game.rover_pos.x-game.pos.x,game.rover_pos.z-game.pos.z);
 for(int i=0;i<3;i++){game.pitch=-.2f-i*.25f;drawcount=0;space();char name[64];snprintf(name,sizeof(name),"rover-depth-angle-%d.bmp",i);dump_native_bmp(name);}
 surface_target_open=1;surface_target_cat=0;surface_target_row=0;drawcount=0;space();dump_native_bmp("surface-target-computer.bmp");surface_target_open=0;
 game.pos=surface_poi(&game,1);game.pos.z-=105;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;surface_target_row=2;surface_target_open=1;surface_target_input(PSP_CTRL_RTRIGGER);drawcount=0;space();dump_native_bmp("surface-poi-track-panel.bmp");
 surface_target_open=0;drawcount=0;space();dump_native_bmp("surface-poi-track-closed.bmp");surface_target_lock=-1;game.pitch=.18f;drawcount=0;space();dump_native_bmp("surface-poi-windows-sign.bmp");
 game.bodies[game.planet].type=ROCKY;game.bodies[game.planet].seed=3;pad=surface_site(&game,1);game.pos=(Vec3){pad.x+310,0,pad.z+300};game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=0;game.pitch=.3f;drawcount=0;space();dump_native_bmp("forest-large-trees.bmp");
 }TEST_INIT();
}
