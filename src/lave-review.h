/* Opt-in native captures and timings; runs only in a disposable review folder. */
static int lave_review(void){
 FILE *flag=fopen("lave-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("lave-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!report||!pixels)return 1;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 fb=pixels+16;
 game_init(&game);game.system=7;system_bodies(&game);launch(&game);story_complete(&game);game.approach=1;enter_planet(&game);
 Vec3 pad=surface_site(&game,1),obs=surface_poi(&game,2);game.surface=2;game.planet_sequence=0;game.ship_pos=pad;game.ship_pos.y+=6;game.speed=0;game.world_clock=0;game.time=20;game.dead=0;
 page=FLIGHT;hud_mode=hud_hidden=high_contrast=0;surface_target_open=0;surface_target_lock=-1;surface_nav_poi=2;planet_landing_menu=0;preview_reset();
 fprintf(report,"OBSERVATORY %.1f m from pad\n",length(sub(obs,pad)));
 int failures=0,blocked=0,slope_bad=0;
 for(int driving=0;driving<2;driving++){
  game.rover_driving=driving;
  for(int step=0;step<=100;step++){
   float t=(320+step*(FIELD_OBSERVATORY_DISTANCE-460)/100.f)/FIELD_OBSERVATORY_DISTANCE,side=game.bodies[1].seed&1?1.f:-1.f;
   float x=pad.x-side*420*t*(1-t),z=pad.z+t*FIELD_OBSERVATORY_DISTANCE;
   game.pos=(Vec3){x,terrain_height(&game,x,z)+22,z};game.yaw=game.pitch=0;
   game_eva_tick(&game,.02f,0,0,1,0,0);
   /* Acceleration starts from rest: a 20 ms tick moves only 0.026 metres. */
   if(game.pos.z<=z+.001f){blocked++;fprintf(report,"BLOCK driving=%d step=%d x=%.1f z=%.1f\n",driving,step,x-pad.x,z-pad.z);}
   float h=terrain_height(&game,x,z),next=terrain_height(&game,x,z+1);
   if(!isfinite(h)||fabsf(next-h)>.8f)slope_bad++;
  }
 }
 game.rover_driving=0;game.rover_pos=add(pad,(Vec3){-110,0,-10});game.time=20;game.world_clock=0;
 fprintf(report,"%s trail walk/rover clearances (%d blocked); %s gentle terrain (%d steep/nonfinite)\n",blocked?"FAIL":"PASS",blocked,slope_bad?"FAIL":"PASS",slope_bad);failures+=blocked+slope_bad;
 float max_seam=0;for(int i=0;i<100;i++){float a=i*.17f;float d=fabsf(planet_ridge_noise(a,123,17)-planet_ridge_noise(a+6.2831853f,123,17));if(d>max_seam)max_seam=d;}
 fprintf(report,"%s full-turn mountain continuity %.6f\n",max_seam<.0001f?"PASS":"FAIL",max_seam);failures+=max_seam>=.0001f;
 int identity=field_site_kind(&game,7,1,2)==FIELD_DISH&&fabsf(obs.x-pad.x)<.01f&&fabsf(obs.z-pad.z-FIELD_OBSERVATORY_DISTANCE)<.01f;
 fprintf(report,"%s observatory identity and navigation\n",identity?"PASS":"FAIL");failures+=!identity;
 game.pos=(Vec3){obs.x,terrain_height(&game,obs.x,obs.z-105)+22,obs.z-105};
 int interaction=surface_nearest_site(&game,60)==2;fprintf(report,"%s observatory approachable interaction\n",interaction?"PASS":"FAIL");failures+=!interaction;
 for(int view=0;view<12;view++){
  game.pos=(Vec3){pad.x-100,0,pad.z+400};if(view==5)game.pos=(Vec3){obs.x,0,obs.z-220};
  if(view==6)game.pos=(Vec3){pad.x+20,0,pad.z+75};
  if(view==8)game.pos=(Vec3){pad.x-75,0,pad.z+780};
  if(view==9)game.pos=(Vec3){pad.x-120,0,pad.z+950};
  if(view>=10){game.pos=(Vec3){pad.x-120,0,pad.z+950};static const int days[]={600,900,1200,1800};game.world_clock=days[(field_seed(7,1)>>24)&3]*(view==10?8.f:13.f)/24;}
  game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
  game.yaw=atan2f(obs.x-game.pos.x,obs.z-game.pos.z)+(view<4?view*1.5707963f:0);game.pitch=view==4?-.35f:view==7?.25f:0;game.roll=0;
  game.voice_time=game.message_time=0;drawcount=0;rect(0,0,W,H,BG);space();char file[48];snprintf(file,sizeof(file),"lave-view-%d.bmp",view);dump_native_bmp(file);
 }
 game.world_clock=0;
 game.pos=(Vec3){pad.x-100,terrain_height(&game,pad.x-100,pad.z+400)+22,pad.z+400};game.yaw=atan2f(obs.x-game.pos.x,obs.z-game.pos.z);game.pitch=0;
 Vec3 start=game.pos;uint64_t total=0,worst=0,frame_total=0,frame_worst=0;int samples=0;
 for(int frame=0;frame<120;frame++){
  uint64_t begin;sceRtcGetCurrentTick(&begin);
  game_tick(&game,1.f/30,frame>=60?.35f:0,0,frame<60?1:0,0);game.voice_time=game.message_time=0;
  uint64_t a,b;sceRtcGetCurrentTick(&a);drawcount=0;rect(0,0,W,H,BG);space();sceRtcGetCurrentTick(&b);if(frame>10){total+=b-a;samples++;if(b-a>worst)worst=b-a;}
  if(frame>10){frame_total+=b-begin;if(b-begin>frame_worst)frame_worst=b-begin;}
  if(frame%3==0){char file[48];snprintf(file,sizeof(file),"lave-motion-%03d.bmp",frame);dump_native_bmp(file);}
 }
 fprintf(report,"Walk displacement %.2f m; draw average %.3f ms, worst %.3f ms\n",length(sub(start,game.pos)),total*1000.f/sceRtcGetTickResolution()/samples,worst*1000.f/sceRtcGetTickResolution());
 fprintf(report,"Update + draw average %.3f ms, worst %.3f ms (no capture IO/display wait)\n",frame_total*1000.f/sceRtcGetTickResolution()/samples,frame_worst*1000.f/sceRtcGetTickResolution());
 for(int i=1;i<6;i++)fprintf(report,"Render stage %d %.3f ms\n",i,(lave_render_marks[i]-lave_render_marks[i-1])*1000.f/sceRtcGetTickResolution());
 int safe=1;for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)safe=0;
 fprintf(report,"%s framebuffer guard words\nRESULT %d failures\n",safe?"PASS":"FAIL",failures+!safe);fclose(report);fb=saved;free(pixels);return 1;
}
