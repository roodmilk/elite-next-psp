extern int mega_city_run_checks(FILE *f);
static int mega_city_review(void){
 FILE *flag=fopen("mega-city-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("mega-city-review.txt","w");if(!f)return 1;
 int failures=mega_city_run_checks(f);fflush(f);
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xfeedac12;
 fb=pixels+16;game_init(&game);for(int s=0;s<256;s++)if(station_class_for_system(&game,s)==STATION_MEGA){game.system=s;break;}
 system_bodies(&game);launch(&game);story_complete(&game);page=FLIGHT;hud_mode=0;hud_hidden=0;high_contrast=0;preview_reset();
 game.voice_time=game.message_time=0;game.dock_stage=0;game.planet=-1;game.dead=0;game.police_stop=0;game.time=50;game.speed=0;selected_target=-1;
 const Vec3 eye[]={{-18000,9000,-20000},{-4200,0,0},{-6200,-2100,-1500},{0,1900,2800},{9000,-2500,4600},{0,0,-2300}};
 const Vec3 aim[]={{0,2200,4500},{-6000,-2500,1800},{-6300,-2500,1400},{4200,2800,2400},{2000,-2500,2500},{0,0,0}};
 fprintf(f,"SYSTEM %d %s; %d solid parts; 480x272 native\n",game.system,game.systems[game.system].name,mega_city_for(&game)->n);
 for(int view=0;view<6;view++){
  game.pos=add(eye[view],(Vec3){0,0,STATION_Z});Vec3 dir=norm(sub(aim[view],eye[view]));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=0;
  uint64_t t0,t1;float sum=0,worst=0;
  for(int frame=0;frame<14;frame++){
   drawcount=0;rect(0,0,W,H,BG);game.time=50+frame/30.f;sceRtcGetCurrentTick(&t0);space();sceRtcGetCurrentTick(&t1);
   float ms=(t1-t0)*1000.f/sceRtcGetTickResolution();if(frame){sum+=ms;worst=fmaxf(worst,ms);}
  }
  char name[64];snprintf(name,sizeof(name),"capital-view-%d.bmp",view);dump_native_bmp(name);
  fprintf(f,"PERF full flight view %d avg %.3f worst %.3f ms (no audio/display)\n",view,sum/13,worst);fflush(f);
  mc_legacy_review=1;sceRtcGetCurrentTick(&t0);for(int k=0;k<10;k++){drawcount=0;rect(0,0,W,H,BG);space();}sceRtcGetCurrentTick(&t1);mc_legacy_review=0;
  fprintf(f,"PERF old hull at same view %d %.3f ms\n",view,(t1-t0)*1000.f/sceRtcGetTickResolution()/10.f);
  if(view==0){mc_legacy_review=1;drawcount=0;rect(0,0,W,H,BG);space();dump_native_bmp("capital-legacy-same-camera.bmp");mc_legacy_review=0;}
 }
 /* Show each seeded skyline and close-up of curved architecture through the
    same live flight renderer, including roll and near-plane clipping. */
 for(int sys=0;sys<256;sys++)if(station_class_for_system(&game,sys)==STATION_MEGA){
  game.system=sys;system_bodies(&game);game.time=50;game.dock_stage=0;game.planet=-1;game.roll=0;
  game.pos=(Vec3){-18000,9000,STATION_Z-20000};Vec3 dir=norm(sub((Vec3){0,2200,STATION_Z+4500},game.pos));
  game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);selected_target=-1;
  drawcount=0;rect(0,0,W,H,BG);space();char name[64];snprintf(name,sizeof(name),"capital-system-%03d.bmp",sys);dump_native_bmp(name);
  const MegaCity *city=mega_city_for(&game);
  for(int shape=MC_DOME;shape<MC_SHAPES;shape++)for(int k=0;k<city->n;k++)if(city->b[k].shape==shape){
   const MegaBlock *b=&city->b[k];Vec3 target=add(b->c,(Vec3){0,0,STATION_Z});
   float r=fmaxf(b->e.x,fmaxf(b->e.y,b->e.z));game.pos=add(target,(Vec3){(shape==MC_POD?1:-1)*r*2.2f,r*.8f,-r*2.8f});
   dir=norm(sub(target,game.pos));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=.18f;
   drawcount=0;rect(0,0,W,H,BG);space();
   if(sys==0){snprintf(name,sizeof(name),"capital-shape-%d.bmp",shape);dump_native_bmp(name);}
   break;
  }
 }
 for(int i=0;i<16;i++)if(pixels[i]!=0xfeedac12||pixels[STRIDE*H+16+i]!=0xfeedac12)failures++;
 fprintf(f,"%s depth state restored for ordinary UI/planet renderer\n",!surface_depth_on&&!surface_material?"PASS":"FAIL");failures+=surface_depth_on||surface_material;
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
