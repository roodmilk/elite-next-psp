extern int pulp_station_run_checks(FILE *f);
static int pulp_station_review(void){
 FILE *flag=fopen("pulp-station-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("pulp-station-review.txt","w");if(!f)return 1;
 int failures=pulp_station_run_checks(f);fflush(f);
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xfeedac12;
 fb=pixels+16;game_init(&game);launch(&game);story_complete(&game);page=FLIGHT;hud_mode=0;hud_hidden=0;high_contrast=0;preview_reset();
 unsigned seen=0;
 for(int sys=0;sys<256;sys++){
  game.system=sys;StationProfile p=station_profile_for(&game,0);
  if(station_class(&game)!=STATION_RICH||(seen&(1u<<p.family)))continue;seen|=1u<<p.family;
  system_bodies(&game);game.voice_time=game.message_time=0;game.dock_stage=0;game.planet=-1;game.dead=0;game.police_stop=0;game.time=0;game.speed=0;selected_target=-1;
  float r=p.radius,h=p.half;
  for(int v=0;v<2;v++){
   Vec3 aim={0,r*.8f,STATION_Z+h*.7f};game.pos=add(aim,(Vec3){v?-r*4:0,r*.9f,-r*6-h*2});Vec3 d=norm(sub(aim,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=asinf(d.y);game.roll=v?.1f:0;
   uint64_t t0,t1;float sum=0,worst=0;
   for(int frame=0;frame<10;frame++){drawcount=0;rect(0,0,W,H,BG);game.time=frame/30.f;sceRtcGetCurrentTick(&t0);space();sceRtcGetCurrentTick(&t1);float ms=(t1-t0)*1000.f/sceRtcGetTickResolution();if(frame){sum+=ms;worst=fmaxf(worst,ms);}}
   char name[64];snprintf(name,sizeof(name),"pulp-family-%d-view-%d.bmp",p.family,v);dump_native_bmp(name);
   fprintf(f,"PERF family %d system %d view %d avg %.3f worst %.3f ms no audio/display\n",p.family,sys,v,sum/9,worst);fflush(f);
  }
 }
 for(int i=0;i<16;i++)if(pixels[i]!=0xfeedac12||pixels[STRIDE*H+16+i]!=0xfeedac12)failures++;
 failures+=surface_depth_on||surface_material;fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
