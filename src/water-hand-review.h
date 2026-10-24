/* Explicit opt-in review runs only in a disposable emulator folder. */
static int water_hand_review(void){
 FILE *flag=fopen("water-hand-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("water-hand-review.txt","w");if(!f)return 1;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;
 #define WCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 field_review_setup(7,1);planet_controls_ready=1;planet_seat.active=0;planet_entry_body=0;planet_orbit_veil=0;ps_open=ps_release=0;eva_map_open=eva_map_release=eva_map_codex=0;paused=0;surface_target_reset();
 game.pos=add(surface_site(&game,1),(Vec3){-120,22,400});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 game.life[0].alive=1;game.life[0].scanned=0;game.life[0].kind=LIFE_FAUNA;game.life[0].pos=add(game.pos,(Vec3){0,0,30});surface_target_lock=11;
 hand_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.1f);hand_input(0,PSP_CTRL_CIRCLE,.12f);
 WCHECK(hand_open&&!game.life[0].scanned,"holding Circle opens selector without scanning");
 field_review_frame("hand-selector.bmp");
 hand_input(PSP_CTRL_DOWN,PSP_CTRL_CIRCLE|PSP_CTRL_DOWN,.02f);hand_input(0,PSP_CTRL_CIRCLE,.05f);
 WCHECK(hand_selected==1&&!hand_open,"empty hands selection closes selector until Circle release");
 hand_input(0,0,.02f);hand_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.05f);hand_input(0,0,.02f);
 WCHECK(!game.life[0].scanned,"empty hands never scan");
 hand_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.22f);hand_input(PSP_CTRL_UP,PSP_CTRL_CIRCLE|PSP_CTRL_UP,.02f);hand_input(0,0,.02f);
 WCHECK(hand_selected==0&&!game.life[0].scanned,"equipping scanner does not trigger a scan");
 surface_target_lock=11;hand_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.05f);hand_input(0,0,.02f);
 WCHECK(game.life[0].scanned&&surface_scan_prompt==0&&surface_scan_fx>0,"tap scans fauna and offers its existing Codex page");
 field_review_frame("hand-scanner.bmp");
 int credits=game.credits;hand_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.05f);hand_input(0,0,.02f);
 WCHECK(game.credits==credits,"repeat scans never duplicate rewards");
 eva_map_open=1;hand_context_tick(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE);eva_map_open=0;hand_context_tick(0,PSP_CTRL_CIRCLE);
 hand_input(0,PSP_CTRL_CIRCLE,.5f);WCHECK(!hand_open,"Circle used to close a map cannot open hand tools");
 hand_context_tick(0,0);hand_input(0,0,.02f);game.rover_driving=1;WCHECK(!hand_context(),"rover retains separate controls");game.rover_driving=0;
 int bridges=0,good=1,masks=1;Vec3 sample={0},axis={0};
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  game.system=sys;system_bodies(&game);game.planet=body;
  for(int i=0;i<6;i++){Vec3 p,a;if(!terrain_bridge_position(&game,i,&p,&a))continue;bridges++;
   good&=!terrain_is_water(&game,p.x,p.z)&&fabsf(terrain_height(&game,p.x,p.z)-p.y)<.02f;
   for(int side=-1;side<=1;side+=2){Vec3 q=add(p,mul(a,side*220.f));good&=!terrain_is_water(&game,q.x,q.z)&&fabsf(terrain_height(&game,q.x,q.z)-p.y)<.02f;}
   if(sys==7&&body==1&&i==3){sample=p;axis=a;}
  }
  surface_water_prepare();Vec3 pad=surface_site(&game,1);
  for(int i=0;i<64;i++){float x=pad.x+(i%8-4)*1700.f,z=pad.z+(i/8-4)*1300.f;unsigned flags=water_cell(water_grid(x),water_grid(z));unsigned ink;if(game.bodies[body].type!=GAS){masks&=((flags&1)!=0)==terrain_water_tile(&game,x,z);masks&=surface_water_sample(x,z,200,&ink)==terrain_water_tile(&game,x,z);}}
 }
 WCHECK(good&&bridges>3000,"all generated bridges use the visible deck height and crossing width");
 WCHECK(masks,"water texture masks agree with raw river/ocean cells across every solid world (gas decks retain exact platform boundaries)");
 field_review_setup(7,1);planet_controls_ready=1;planet_seat.active=0;game.pos=sub(sample,mul(axis,340));game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=atan2f(axis.x,axis.z);game.pitch=-.15f;surface_scan_fx=0;
 field_review_frame("river-bridge.bmp");
 unsigned ink0=0,ink1=0;int animated=0;for(int i=0;i<500;i++){float x=sample.x+i*.7f,z=sample.z;water_phase=0;int wet=surface_water_sample(x,z,200,&ink0);water_phase=7;surface_water_sample(x,z,200,&ink1);animated|=wet&&ink0!=ink1;}
 WCHECK(animated,"world-anchored water ripple colours animate");
 double total=0,worst=0;for(int i=0;i<24;i++){uint64_t a,b;game.time+=.1f;sceRtcGetCurrentTick(&a);field_review_frame(0);sceRtcGetCurrentTick(&b);double ms=(b-a)*1000.0/sceRtcGetTickResolution();total+=ms;if(ms>worst)worst=ms;}
 fprintf(f,"INFO river scene draw average %.3f ms / worst %.3f ms; bounded water cache %u bytes\n",total/24,worst,(unsigned)(sizeof(water_tags)+sizeof(water_flags)));
 int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 WCHECK(guard,"native framebuffer guards preserved");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
 #undef WCHECK
}
