/* Opt-in isolated visual review: never compiled into retail. */
static int flight_lighting_review(void){
 FILE *flag=fopen("flight-lighting-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("flight-lighting-review.txt","w");unsigned *saved=fb,*mem=calloc(STRIDE*H+32,sizeof(unsigned));if(!out||!mem)return 1;
 fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0xabc12345;
 game_init(&game);game.system=7;system_bodies(&game);launch(&game);story_complete(&game);page=FLIGHT;hud_mode=hud_hidden=high_contrast=third_person=0;preview_reset();game.voice_time=game.message_time=0;game.dock_stage=0;game.planet=-1;game.dead=game.police_stop=0;game.time=50;game.speed=0;selected_target=BODY_COUNT;
 int failures=0,addok=1;
 for(int a=0;a<256;a+=7)for(int b=0;b<256;b+=11){unsigned c=flight_light_add(RGB(a,255-a,a/2),RGB(b,b/2,255-b));int r=a+b,g=255-a+b/2,bl=a/2+255-b;unsigned expected=RGB(r>255?255:r,g>255?255:g,bl>255?255:bl);if((c&0xffffff)!=(expected&0xffffff))addok=0;}
 fprintf(out,"%s saturating glow blend agrees with reference\n",addok?"PASS":"FAIL");failures+=!addok;
 Vec3 station={0,0,STATION_Z},toward=norm(sub(game.bodies[0].pos,station));
 for(int view=0;view<3;view++){
  Vec3 offset=view==0?(Vec3){-.25f,.18f,0}:view==1?(Vec3){.3f,-.14f,0}:(Vec3){-.8f,.25f,0};
  Vec3 dir=norm(add(toward,offset));game.pos=sub(station,mul(dir,view==2?8500:4500));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=0;
  for(int enabled=0;enabled<2;enabled++){
   flight_lighting_enabled=enabled;float sum=0,worst=0;uint64_t a,b;render_profile_enabled=1;memset(render_phase_ms,0,sizeof(render_phase_ms));
   for(int frame=0;frame<13;frame++){rect(0,0,W,H,BG);drawcount=0;sceRtcGetCurrentTick(&a);space();sceRtcGetCurrentTick(&b);float ms=(b-a)*1000.f/sceRtcGetTickResolution();if(frame){sum+=ms;worst=fmaxf(worst,ms);}}
   char name[64];snprintf(name,sizeof(name),"flight-lighting-%d-%s.bmp",view,enabled?"after":"before");dump_native_bmp(name);
   fprintf(out,"PERF view=%d lighting=%d full-flight mean=%.3f worst=%.3f ms; excludes display/audio\n",view,enabled,sum/12,worst);for(int phase=0;phase<7;phase++)fprintf(out,"PHASE %d %.3f ms\n",phase,render_phase_ms[phase]/13);fflush(out);
  }
 }
 int mapping_ok=1;float mapping_worst=0;
 for(int cam=0;cam<2;cam++)for(int frame=0;frame<24;frame++){
  third_person=cam;game.boost=1;game.boost_spool=1;game.speed=player_ships[game.ship].speed*5;game.time=50+frame/30.f;
  flight_camera_prepare();mega_city_depth_begin();
  Vec3 origin=camera_origin(&game),eye=rotate(sub(origin,(Vec3){0,0,STATION_Z}),0,-station_angle(&game));
  Vec3 local={113,87,-240},view=camera(&game,station_vertex_at(local,(Vec3){0,0,STATION_Z},station_angle(&game)));
  float expected[3]={local.x,local.y,local.z},offset[3]={eye.x,eye.y,eye.z};
  for(int axis=0;axis<3;axis++){float recovered=offset[axis]+mc_axis[axis].x*view.x+mc_axis[axis].y*view.y+mc_axis[axis].z*view.z;float error=fabsf(recovered-expected[axis]);mapping_worst=fmaxf(mapping_worst,error);if(error>1.f)mapping_ok=0;}
  mega_city_depth_end();camera_override_clear();
 }
 fprintf(out,"%s boost-shaken first/third-person station surface mapping stays world-attached (worst %.4f units)\n",mapping_ok?"PASS":"FAIL",mapping_worst);failures+=!mapping_ok;
 third_person=0;game.boost=0;game.boost_spool=0;
 render_profile_enabled=0;flight_lighting_enabled=1;int safe=1;for(int i=0;i<16;i++)if(mem[i]!=0xabc12345||mem[STRIDE*H+16+i]!=0xabc12345)safe=0;
 fprintf(out,"%s framebuffer guard words unchanged\n",safe?"PASS":"FAIL");failures+=!safe;
 high_contrast=1;int disabled=!flight_lighting_active();high_contrast=0;preview_clip(240,110,53,39,317,167);disabled&=!flight_lighting_active();preview_reset();page=HOME;disabled&=!flight_lighting_active();page=FLIGHT;
 fprintf(out,"%s decorations disabled in high contrast, previews and menus\n",disabled?"PASS":"FAIL");failures+=!disabled;
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=saved;free(mem);return 1;
}


