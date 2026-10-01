static void roamer_review_setup(void){
 pilot_review_reset(1);enter_planet(&game);game.planet_sequence=0;game.approach=-1;game.surface=2;
 game.ship_pos=surface_site(&game,1);game.rover_pos=add(game.ship_pos,(Vec3){FIELD_GARAGE_X,0,-10});
 game.pos=add(game.rover_pos,(Vec3){0,22,38});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 game.rover_driving=0;surface_rover(&game);planet_seat.active=0;planet_controls_ready=1;
 game.pos=add(game.ship_pos,(Vec3){0,22,300});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 game.rover_pos=game.pos;game.voice_time=game.message_time=0;eva_map_open=eva_map_release=0;game.yaw=0;
 input(0,0,.016f,0,0);
}
static int roamer_review(void){
 FILE *flag=fopen("roamer-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("roamer-review.txt","w");if(!f)return 1;int failures=0;
 #define RCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 roamer_review_setup();Vec3 start=game.pos;
 input(PSP_CTRL_UP,PSP_CTRL_UP,.05f,0,0);
 RCHECK(length(sub(start,game.pos))<.01f,"D-pad Up no longer drives the Roamer");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.05f,0,0);
 RCHECK(game.speed>0&&game.speed<10,"R starts gradual acceleration, not instant top speed");
 for(int i=0;i<18;i++)input(0,PSP_CTRL_RTRIGGER,.05f,0,0);
 float v=length(game.rover_velocity);Vec3 pos=game.pos;
 RCHECK(game.rover_last_controls==ROAM_ACCEL&&game.rover_charge==100,"R acceleration never enables or drains boost without X");
 input(0,0,.05f,0,0);RCHECK(length(sub(pos,game.pos))>1&&length(game.rover_velocity)<v,"release throttle coasts and decelerates");
 for(int i=0;i<12;i++)input(0,PSP_CTRL_LTRIGGER,.05f,0,0);
 fprintf(f,"INFO brake velocity %.3f wait %.3f surface %d controls %u ticks %u\n",length(game.rover_velocity),game.rover_reverse_wait,game.surface,game.rover_last_controls,game.rover_ticks);
 RCHECK(length(game.rover_velocity)<8,"L brakes to nearly stopped");
 for(int i=0;i<20;i++)input(0,PSP_CTRL_LTRIGGER,.05f,0,0);
 RCHECK(game.rover_velocity.z< -15&&game.rover_velocity.z>=-55.1f,"continued L reverses at a bounded speed");
 roamer_review_setup();for(int i=0;i<20;i++)input(0,PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,.05f,0,0);
 RCHECK(game.rover_driving&&length(game.rover_velocity)>95&&game.rover_charge<80,"X boosts under R without dismounting");
 float charge=game.rover_charge;float clock=game.time;unsigned ticks=game.rover_ticks;input(0,0,.1f,0,0);
 fprintf(f,"INFO recharge controls %u ticks %u -> %u\n",game.rover_last_controls,ticks,game.rover_ticks);
 fprintf(f,"INFO charge %.3f -> %.3f time %.3f -> %.3f vehicle %d controls %d page %d approach %d\n",charge,game.rover_charge,clock,game.time,game.rover_driving,planet_controls_ready,page,game.approach);
 RCHECK(game.rover_charge>charge,"boost recharges when released");
 fprintf(f,"INFO release boost %d brake %d surface %d dead %d dock %d tractor %.3f jump %.3f seat %d sequence %d veil %.3f ps %d/%d map %d/%d\n",game.rover_boost,game.rover_brake,game.surface,game.dead,game.dock_stage,game.tractor_time,game.jump,planet_seat.active,game.planet_sequence,planet_orbit_veil,ps_open,ps_release,eva_map_open,eva_map_release);
 game_rover_tick(&game,.05f,0,0,0);fprintf(f,"INFO direct recharge %.3f\n",game.rover_charge);
 roamer_review_setup();Game initial=game;game.rover_velocity=(Vec3){0,0,130};
 for(int i=0;i<8;i++)game_rover_tick(&game,.05f,.9f,0,ROAM_ACCEL);
 float grippy=fabsf(game.rover_velocity.x*cosf(game.yaw)-game.rover_velocity.z*sinf(game.yaw));
 game=initial;game.rover_velocity=(Vec3){0,0,130};
 for(int i=0;i<8;i++)input(0,PSP_CTRL_RTRIGGER|PSP_CTRL_CIRCLE,.05f,.9f,0);
 float slide=fabsf(game.rover_velocity.x*cosf(game.yaw)-game.rover_velocity.z*sinf(game.yaw));
 RCHECK(slide>grippy*1.5f,"Circle permits a controllable momentum drift");
 RCHECK(game.rover_driving&&game.jetpack==0&&game.boost==0,"driving never activates ship boost or EVA jetpack");
 RCHECK(surface_scan_prompt<0,"Circle handbrake does not run planetary scanner");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);RCHECK(game.rover_driving,"cannot step out of a moving Roamer");
 roamer_review_setup();Vec3 pad=game.ship_pos;
 game.pos=add(pad,(Vec3){FIELD_GARAGE_X,22,-10});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 game.yaw=3.14159265f;game.rover_velocity=(Vec3){0,0,-200};
 for(int i=0;i<12;i++)game_rover_tick(&game,.05f,0,0,0);
 RCHECK(game.pos.z>pad.z-45&&game.rover_crack>0,"hard garage-wall hit stops safely and cracks the view");
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(pixels){
  fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
  space();dump_native_bmp("roamer-cracked.bmp");unsigned t=sceKernelGetSystemTimeWide();
  for(int i=0;i<100;i++)rover_windshield();
  fprintf(f,"INFO crack overlay %.3f ms\n",(sceKernelGetSystemTimeWide()-t)/100000.f);
  int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
  RCHECK(guard,"crack overlay preserves framebuffer bounds");fb=saved;free(pixels);
 }
 game.rover_velocity=(Vec3){0,0,0};for(int i=0;i<60;i++)game_rover_tick(&game,.05f,0,0,0);
 RCHECK(game.rover_crack==0,"windshield crack clears automatically");
 game.pos=add(pad,(Vec3){0,22,300});game.rover_pos=game.pos;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 input(0,0,.016f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.016f,0,0);
 RCHECK(!game.rover_driving&&planet_seat.active,"Triangle parks and animates safe exit when stopped");
 roamer_review_setup();game.rover_charge=0;
 for(int i=0;i<10;i++)input(0,PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,.05f,0,0);
 RCHECK(game.rover_charge==0&&length(game.rover_velocity)<40,"empty held boost does not flicker or recharge itself");
 input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);RCHECK(eva_map_open&&game.rover_driving,"Start opens field map from the driver's seat");
 input(0,0,.016f,0,0);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);
 Vec3 parked=game.pos;input(0,PSP_CTRL_CIRCLE|PSP_CTRL_RTRIGGER,.1f,1,0);
 RCHECK(!eva_map_open&&eva_map_release&&length(sub(game.pos,parked))<.01f,"map exit consumes held drift/throttle controls");
 input(0,0,.016f,0,0);
 roamer_review_setup();game.planet=3;system_bodies(&game);enter_planet(&game);game.surface=2;game.approach=-1;game.rover_driving=1;
 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,22,0});game.rover_velocity=(Vec3){0,0,250};game.yaw=0;game.rover_charge=100;
 for(int i=0;i<90;i++)game_rover_tick(&game,.05f,0,0,ROAM_ACCEL|ROAM_DRIFT|ROAM_BOOST);
 RCHECK(surface_cloud_deck(&game,game.pos.x,game.pos.z)&&isfinite(game.pos.y),"boosted drifting remains on real gas-world platforms");
 RCHECK(game.rover_charge>=0&&game.rover_charge<=100,"boost reservoir remains bounded");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);
 #undef RCHECK
 return 1;
}
