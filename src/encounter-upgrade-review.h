static int encounter_upgrade_review(void){
 FILE *flag=fopen("encounter-upgrade.flag","r");if(!flag)return 0;fclose(flag);FILE*out=fopen("encounter-upgrade.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);
 unsigned *old=fb;fb=calloc(STRIDE*H,sizeof(unsigned));if(!fb){fprintf(out,"RESULT 1 failures allocation\n");fclose(out);return 1;}
 fprintf(out,"INFO GPU ready %d fleet assets %d clip %d cached %d warp colors %d\n",ion_gu_ready,fleet_ge_ready,clipy0,flight_cached_frame,ion_colors!=0);
 unsigned *ring_reference=malloc(STRIDE*H*sizeof(unsigned));int ring_mismatch=0,ring_checks=0;
 const char *names[]={"outer-relay","frontier-outpost","animal-carrier","sanctuary","cinema-a","closed-resort","mobile-mechanic","cinema-b"};int slots[]={-1,-2,2,3,4,5,6,7};int fails=0;
 for(int scene=0;scene<8;scene++){
  game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);game_spawn(&game);game.opening.stage=OP_FREE;game.tutorial_step=0;game.docked=0;game.planet=game.approach=-1;page=FLIGHT;third_person=0;hud_mode=hud_hidden=0;game.voice_time=game.message_time=0;game.speed=0;game.time=20;
  Vec3 centre=slots[scene]<0?hub_position(&game,-slots[scene]):discovery_position(slots[scene]);float range=slots[scene]<0?station_profile_for(&game,-slots[scene]).radius*5.5f:3400;
  selected_target=slots[scene]<0?STATION_TARGET_ID(-slots[scene]):DISCOVERY_ID_MIN+slots[scene];
  double sum=0,worst=0;int samples=0,peak=0;render_profile_enabled=1;memset(render_phase_ms,0,sizeof(render_phase_ms));memset(render_sub_ms,0,sizeof(render_sub_ms));memset(station_ge_profile_us,0,sizeof(station_ge_profile_us));memset(encounter_model_us,0,sizeof(encounter_model_us));memset(encounter_effect_us,0,sizeof(encounter_effect_us));encounter_flush_us=0;unsigned old_ge=fleet_ge_calls;
  for(int frame=0;frame<80;frame++){
   game.time=20+frame*.1f;float orient=slots[scene]<0?-slots[scene]*.47f:(discovery_seed(slots[scene])&255)*.006f;float a=-orient+.35f+sinf(frame*.1f)*.08f;float distance=frame>=12?range*2.8f:range;encounter_sprite_enabled=(frame%8)>=4;game.pos=add(centre,(Vec3){sinf(a)*distance,distance*.15f,-cosf(a)*distance});Vec3 aim=norm(sub(centre,game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;flight_camera_seen=-1;
   if(frame>=20){a=-orient+(frame-20)*6.2831853f/60;distance=range*(frame<40?1:frame<60?1.6f:2.8f);game.pos=add(centre,(Vec3){sinf(a)*distance,distance*(.15f+sinf(a)*.2f),-cosf(a)*distance});aim=norm(sub(centre,game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);}
   if(ring_reference&&(frame==0||frame==12||frame==35||frame==55||frame==75))for(int phase=0;phase<2;phase++){
    memset(fb,0,STRIDE*H*sizeof(unsigned));celestial_rings_reference(phase);memcpy(ring_reference,fb,STRIDE*H*sizeof(unsigned));memset(fb,0,STRIDE*H*sizeof(unsigned));celestial_rings(phase);ring_mismatch+=memcmp(ring_reference,fb,STRIDE*H*sizeof(unsigned))!=0;ring_checks++;
   }
   if(!frame)fprintf(out,"STATE %s fleet eligible %d wreck eligible %d clip %d dock %d police %d rift %d race %d\n",names[scene],fleet_ge_view_ready(),wreck_ge_view_ready(),clipy0,game.dock_stage,game.police_stop,game.rift_report,race_ready(&game));
   double old_planets=render_phase_ms[2],old_encounters=render_phase_ms[3];uint64_t begin=sceKernelGetSystemTimeWide();game_tick(&game,1.f/30,0,0,0,0);drawcount=0;render_submitted=0;space();double ms=(sceKernelGetSystemTimeWide()-begin)*.001;
   if(frame>=4&&ms>33.42)fprintf(out,"SLOW %s frame %d total %.3f planets %.3f encounters %.3f\n",names[scene],frame,ms,render_phase_ms[2]-old_planets,render_phase_ms[3]-old_encounters);
   if(frame>=4){sum+=ms;samples++;if(ms>worst)worst=ms;}if(render_submitted>peak)peak=render_submitted;
   if(frame==3||frame==7||frame==12||frame==16){char p[96];snprintf(p,sizeof(p),"upgrade-%s-%d.bmp",names[scene],frame);dump_native_bmp(p);}
  }
  int ok=sum/samples<=33.42&&worst<=40;fails+=!ok;fprintf(out,"%s %s simulation+render mean %.3f ms worst %.3f ms peak triangles %d samples %d orbit/near/far\n",ok?"PASS":"FAIL",names[scene],sum/samples,worst,peak,samples);
  fprintf(out,"PROFILE sky %.3f traffic %.3f planets %.3f encounters %.3f ships %.3f effects %.3f hud %.3f hardware calls %u\n",render_phase_ms[0]/80,render_phase_ms[1]/80,render_phase_ms[2]/80,render_phase_ms[3]/80,render_phase_ms[4]/80,render_phase_ms[5]/80,render_phase_ms[6]/80,fleet_ge_calls-old_ge);
  fprintf(out,"SUB rings-rear %.3f discs %.3f rings-effects %.3f station-model %.3f station-flush %.3f hubs %.3f discoveries %.3f ambient %.3f\n",render_sub_ms[0]/80,render_sub_ms[1]/80,render_sub_ms[2]/80,render_sub_ms[3]/80,render_sub_ms[4]/80,render_sub_ms[5]/80,render_sub_ms[6]/80,render_sub_ms[7]/80);
  fprintf(out,"GPU texture %.3f whole-pass %.3f wait %.3f CPU-depth %.3f CPU-fallback %.3f ms/view\n",station_ge_profile_us[0]*.001/80,station_ge_profile_us[1]*.001/80,station_ge_profile_us[4]*.001/80,station_ge_profile_us[2]*.001/80,station_ge_profile_us[3]*.001/80);
  for(int slot=0;slot<discovery_count();slot++)fprintf(out,"OBJECT slot %d entry %d model %.3f effects %.3f ms/view\n",slot,discovery_entry(slot),encounter_model_us[slot]*.001/80,encounter_effect_us[slot]*.001/80);fprintf(out,"OBJECT shared flush %.3f ms/view\n",encounter_flush_us*.001/80);
 }
 fprintf(out,"%s bounded ring raster pixel-identical to reference in %d views (%d mismatches)\n",ring_reference&&!ring_mismatch?"PASS":"FAIL",ring_checks,ring_mismatch);fails+=!ring_reference||ring_mismatch!=0;free(ring_reference);
 render_profile_enabled=0;free(fb);fb=old;fprintf(out,"INFO PSP-side workload timings in PPSSPP; excludes display pacing, hardware confirmation required\nRESULT %d failures\n",fails);fclose(out);return 1;
}
