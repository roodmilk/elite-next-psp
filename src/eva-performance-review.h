/* Compare old/new terrain work in the same compact world, then walk across
 * cell boundaries and turn. CPU work is measured independently of vblank. */
static int eva_performance_review(void){
 FILE *flag=fopen("eva-performance-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("eva-performance-review.txt","w");unsigned *pixels=calloc(STRIDE*H+32,sizeof(unsigned)),*reference=malloc(STRIDE*H*sizeof(unsigned));unsigned short *old_depth=malloc(sizeof(surface_depth));if(!report||!pixels||!reference||!old_depth)return 1;
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xace5b007u;int failures=0;eva_profile=1;
 for(int scene=0;scene<10;scene++){
  int sys=scene<6?7:scene<8?0:128,body=scene<3?1:scene<6?scene-1:1;
  field_review_setup(sys,body);planet_seat.active=planet_entry_body=0;planet_orbit_veil=0;third_person=0;game.surface=2;game.planet_sequence=0;game.voice_time=game.message_time=0;
  Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){scene==0?-90:scene==2?240:0,0,scene==0?230:scene==2?560:3600});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=scene==7?1.57f:0;game.pitch=scene==8?-.55f:scene==9?.5f:0;
  eva_fast=0;field_review_frame(0);uint64_t start=sceKernelGetSystemTimeWide();for(int i=0;i<5;i++)field_review_frame(0);double old=(sceKernelGetSystemTimeWide()-start)*.0002;memcpy(reference,fb,STRIDE*H*sizeof(unsigned));memcpy(old_depth,surface_depth,sizeof(surface_depth));
  eva_fast=1;start=sceKernelGetSystemTimeWide();for(int i=0;i<5;i++)field_review_frame(0);double fast=(sceKernelGetSystemTimeWide()-start)*.0002;int changed=0;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++)changed+=fb[y*STRIDE+x]!=reference[y*STRIDE+x];int depth_changed=0,hud_changed=0;for(int y=0;y<H;y++)for(int x=0;x<W;x++){depth_changed+=surface_depth[y*W+x]!=old_depth[y*W+x];if(y<view_top()||y>view_bot())hud_changed+=fb[y*STRIDE+x]!=reference[y*STRIDE+x];}failures+=depth_changed!=0||hud_changed!=0;
  fprintf(report,"%s scene %d system %d body %d previous %.3f ms optimized %.3f ms; %d changed pixels\n",depth_changed||hud_changed?"FAIL":"PASS",scene,sys,body,old,fast,changed);fprintf(report,"DEPTH scene %d changed %d; HUD changed %d\n",scene,depth_changed,hud_changed);fflush(report);
  double total=0,worst=0,stages[5]={0},raster=0;int stable=1;
  for(int frame=0;frame<36;frame++){
   game.time+=1.f/60;game.yaw+=.018f;game.pos.x+=2;game.pos.z+=1.5f;game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
   start=sceKernelGetSystemTimeWide();game_tick(&game,1.f/60,0,0,0,0);field_review_frame(0);double elapsed=(sceKernelGetSystemTimeWide()-start)*.001;total+=elapsed;if(elapsed>worst)worst=elapsed;
   stable&=isfinite(game.pos.y)&&game.surface==2&&!game.dead;
   raster+=eva_raster_us*.001;
   for(int i=0;i<5;i++)stages[i]+=(lave_render_marks[i+1]-lave_render_marks[i])*1000.0/sceRtcGetTickResolution();
  }
  fprintf(report,"WALK scene %d mean %.3f ms worst %.3f ms (%.2f CPU FPS); sky %.3f terrain %.3f sites %.3f foliage %.3f traffic %.3f\n",scene,total/36,worst,36000/total,stages[0]/36,stages[1]/36,stages[2]/36,stages[3]/36,stages[4]/36);failures+=!stable;
  fprintf(report,"RASTER scene %d %.3f ms (terrain and subsequent mesh passes)\n",scene,raster/36);
  char file[48];snprintf(file,sizeof(file),"eva-scene-%02d.bmp",scene);dump_native_bmp(file);fflush(report);
 }
 /* Compare full scenery identities before/after entering neighbouring and
  * aliased cache cells, including a return to a previously visited window. */
 field_review_setup(7,1);Vec3 pad=surface_site(&game,1),sites[10];FieldBuilding shapes[10];for(int i=1;i<10;i++)if(i!=6){sites[i]=surface_poi(&game,i);shapes[i]=field_site_building(&game,i);}
 PlanetProp *old_props=malloc(sizeof(lave_prop_cache));int match=old_props!=NULL,windows=0;static const int shifts[]={0,1,-1,16,17,0,-17};
 for(int i=0;old_props&&i<7;i++){
  game.pos=add(pad,(Vec3){shifts[i]*280.f,22,3600+shifts[6-i]*280.f});unsigned seed=game.bodies[1].seed;
  eva_fast=0;lave_prop_ready=0;lave_prepare_props(pad,sites,shapes,seed);int count=lave_prop_count;memcpy(old_props,lave_prop_cache,count*sizeof(PlanetProp));
  eva_fast=1;lave_prop_ready=0;lave_prepare_props(pad,sites,shapes,seed);match&=count==lave_prop_count&&memcmp(old_props,lave_prop_cache,count*sizeof(PlanetProp))==0;windows++;
 }free(old_props);failures+=!match;fprintf(report,"%s foliage identities across %d cold, warm, boundary and aliased windows\n",match?"PASS":"FAIL",windows);
 int guards=1;for(int i=0;i<16;i++)guards&=pixels[i]==0xace5b007u&&pixels[STRIDE*H+16+i]==0xace5b007u;failures+=!guards;
 fprintf(report,"%s framebuffer guards; walking stays finite and alive\n",guards?"PASS":"FAIL");fprintf(report,"INFO CPU timings exclude vblank and do not certify physical PSP FPS.\nRESULT %d failures\n",failures);fclose(report);return 1;
}
