/* Disposable deterministic PSP-side workload review. No commander files or
 * settings are touched. Times exclude vblank and therefore expose real work. */
static int perf_old_landable(const Game *g,int system,int body){
 if(!world_planet_available(g,system,body))return 0;
 Game probe=*g;probe.system=system;system_bodies(&probe);
 return probe.bodies[body].type!=SUN&&probe.bodies[body].type!=GAS;
}
static int perf_old_destination(const Game *g,int offer){
 int nearby[256],n=0,type=mission_type_for_offer(g,offer);float range=fminf(10.f,player_ships[g->ship].range*.1f);
 for(int i=0;i<256;i++)if(i!=g->system&&distance_ly(g,g->system,i)<=range+.001f){
  WorldState w;world_state_build(g,i,&w);if(!w.station_count)continue;
  int playable=1;if(mission_base_type(type)==MISSION_EXPLORATION){playable=0;for(int b=1;b<BODY_COUNT;b++)if(perf_old_landable(g,i,b)){playable=1;break;}}
  if(playable)nearby[n++]=i;
 }
 return n?nearby[(g->system*13+offer*7)%n]:g->system;
}
static double perf_ms(uint64_t start){uint64_t end;sceRtcGetCurrentTick(&end);return (end-start)*1000.0/sceRtcGetTickResolution();}
static void perf_scene(int scene){
 game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);launch(&game);page=FLIGHT;paused=0;
 game.pos=(Vec3){0,0,STATION_Z-12000};game.yaw=game.pitch=game.roll=0;game.speed=0;game.time=50;
 game.voice_time=game.message_time=0;game.dock_stage=0;game.planet=-1;game.approach=-1;game.police_stop=0;
 hud_mode=hud_hidden=0;third_person=scene==4?1:0;selected_target=0;autoaim=0;flight_camera_seen=-1;
 if(scene==0){game.docked=1;page=MISSIONS;return;}
 NPC sample=game.npc[0];
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&game.npc[i];*n=sample;n->alive=1;n->role=i%FACTION_COUNT;n->target=-1;n->flash=0;n->freighter=i%8==0;n->freight_style=i%3;
  n->dir=(Vec3){0,0,1};n->pos=add(game.pos,(Vec3){((i%8)-3.5f)*(scene==2?1800:430),((i/8)-2.5f)*300,scene==2?18000+i*550.f:2500+i*75.f});
  if(scene==3)n->pos.z=game.pos.z-4000-i*140.f;
 }
 for(int i=0;i<DEBRIS_COUNT;i++){
  Debris *d=&game.debris[i];memset(d,0,sizeof(*d));d->alive=1;d->rock=1;d->radius=20+i%5*5;d->health=100;d->life=100;
  d->pos=add(game.pos,(Vec3){((i%8)-3.5f)*650,((i/8)-3.5f)*240,scene==2?22000+i*100.f:3200+i*70.f});
  if(scene==3)d->pos.z=game.pos.z-3000-i*80.f;
 }
 if(scene==5){
  for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;
  game.pos.z=STATION_Z-80000;
  for(int b=1;b<LEGACY_BODY_COUNT;b++){game.bodies[b].pos=add(game.pos,(Vec3){(b-2.5f)*4000,(b&1?1:-1)*900,20000+b*1800.f});game.bodies[b].radius=600+b*400;}
 }
}
static int performance_review(void){
 FILE *flag=fopen("performance-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("performance-review.txt","w");if(!f)return 1;
 unsigned *saved=fb;fb=malloc(STRIDE*H*sizeof(*fb));if(!fb){fprintf(f,"RESULT 1 failures\n");fclose(f);return 1;}
 int fails=0,mismatch=0,land_mismatch=0;uint64_t start;volatile int sink=0;
 game_init(&game);story_complete(&game);
 for(int system=0;system<256;system++){
  game.system=system;
  for(int b=0;b<BODY_COUNT;b++)land_mismatch+=mission_landable_body(&game,system,b)!=perf_old_landable(&game,system,b);
  int dest[5],valid[5],risk[5],reward[5];
  for(int i=0;i<5;i++){dest[i]=perf_old_destination(&game,i);valid[i]=mission_offer_valid(&game,i,0);risk[i]=mission_risk(&game,i);reward[i]=mission_reward(&game,i);}
  mission_queries_begin(&game);
  for(int i=0;i<5;i++)mismatch+=dest[i]!=mission_destination(&game,i)||valid[i]!=mission_offer_valid(&game,i,0)||risk[i]!=mission_risk(&game,i)||reward[i]!=mission_reward(&game,i);
  mission_queries_end();
 }
 fprintf(f,"%s all 1280 mission destinations, validation, risks and rewards unchanged\n",mismatch?"FAIL":"PASS");fails+=!!mismatch;
 fprintf(f,"%s all 2560 planet availability checks match generated bodies\n",land_mismatch?"FAIL":"PASS");fails+=!!land_mismatch;fflush(f);
 /* Pixel identity matters more than a fast but visibly changed cache. */
 unsigned *planet_pixels=malloc(STRIDE*H*sizeof(*fb));int pixel_mismatch=0;
 if(!planet_pixels)pixel_mismatch=1;else{
  for(int b=1;b<BODY_COUNT;b++)for(int rolled=0;rolled<2;rolled++){
   unsigned seed=body_art_seed(7,b);int type=b%3==0?GAS:b%3==1?OCEAN:ROCKY;
   memset(fb,0,STRIDE*H*sizeof(*fb));render_reference=1;draw_planet_sprite_rolled(240,110,31,seed,type,rolled?.61f:0,0,0,W,H-1);memcpy(planet_pixels,fb,STRIDE*H*sizeof(*fb));
   memset(fb,0,STRIDE*H*sizeof(*fb));render_reference=0;draw_planet_sprite_rolled(240,110,31,seed,type,rolled?.61f:0,0,0,W,H-1);pixel_mismatch+=memcmp(planet_pixels,fb,STRIDE*H*sizeof(*fb))!=0;
  }
  unsigned misses=planet_sheet_misses;for(int b=1;b<BODY_COUNT;b++)planet_sheet(body_art_seed(7,b),b%3==0?GAS:b%3==1?OCEAN:ROCKY);
  pixel_mismatch+=misses!=planet_sheet_misses;free(planet_pixels);
 }
 fprintf(f,"%s cached planet art is pixel-identical when level/rolled and a full system stays cached\n",pixel_mismatch?"FAIL":"PASS");fails+=!!pixel_mismatch;
 game.pos=(Vec3){0,0,0};game.yaw=game.pitch=game.roll=0;camera_override_clear();preview_reset();render_reference=0;
 int bounds=render_sphere_visible((Vec3){0,0,100},10)&&!render_sphere_visible((Vec3){0,0,-100},10)&&render_sphere_visible((Vec3){120,0,100},20)&&!render_sphere_visible((Vec3){500,0,100},10)&&render_sphere_visible((Vec3){0,0,10},20);
 fprintf(f,"%s screen culling preserves edge and near-plane intersections\n",bounds?"PASS":"FAIL");fails+=!bounds;
 game.system=7;sceRtcGetCurrentTick(&start);
 for(int frame=0;frame<6;frame++)for(int q=0;q<35;q++)sink+=perf_old_destination(&game,q%5);
 double old=perf_ms(start)/6;sceRtcGetCurrentTick(&start);
 for(int frame=0;frame<6;frame++){mission_queries_begin(&game);for(int q=0;q<35;q++)sink+=mission_destination(&game,q%5);mission_queries_end();}
 double fast=perf_ms(start)/6;
 fprintf(f,"MISSION queries reference %.3f ms; optimized %.3f ms per board frame\n",old,fast);
 fprintf(f,"%s batched mission queries reduce board work\n",fast<old?"PASS":"FAIL");fails+=fast>=old;
 const char *names[]={"mission-scroll","crowded-near","crowded-distant","crowded-behind","crowded-third-person","planet-field"};double optimized[6]={0};
 for(int scene=0;scene<6;scene++)for(int reference=1;reference>=0;reference--){
  perf_scene(scene);render_reference=reference;preview_reset();rect(0,0,W,H,BG);drawcount=0;
  if(scene==0)mission_board();else space(); /* Warm atlas/layout caches. */
  double sum=0,worst=0;int culled=0,lod=0,tri=0;memset(render_phase_ms,0,sizeof(render_phase_ms));render_profile_enabled=1;
  for(int frame=0;frame<24;frame++){
   game.time=50+frame/60.f;row=frame%5;game.yaw=scene?sin(frame*.12f)*.06f:0;if(scene==5)game.roll=.4f;
   rect(0,0,W,H,BG);drawcount=0;render_culled=render_lod=render_submitted=0;
   sceRtcGetCurrentTick(&start);if(scene==0)mission_board();else space();double ms=perf_ms(start);sum+=ms;if(ms>worst)worst=ms;
   culled+=render_culled;lod+=render_lod;tri+=render_submitted;
   if(!reference&&frame==12){char path[64];snprintf(path,sizeof(path),"performance-%s.bmp",names[scene]);dump_native_bmp(path);}
  }
  fprintf(f,"SCENE %s %s mean %.3f ms; worst %.3f ms; triangles %.0f; culled %.0f; LOD %.0f\n",names[scene],reference?"reference":"optimized",sum/24,worst,tri/24.,culled/24.,lod/24.);fflush(f);
  if(!reference)optimized[scene]=sum/24;
  render_profile_enabled=0;if(scene){fprintf(f,"PHASE sky %.3f; freight %.3f; planets %.3f; station/discoveries %.3f; hulls %.3f; effects %.3f; HUD %.3f ms\n",render_phase_ms[0]/24,render_phase_ms[1]/24,render_phase_ms[2]/24,render_phase_ms[3]/24,render_phase_ms[4]/24,render_phase_ms[5]/24,render_phase_ms[6]/24);fflush(f);}
 }
 perf_scene(1);sceRtcGetCurrentTick(&start);for(int i=0;i<24;i++)game_tick(&game,1.f/60,0,0,0,0);double sim=perf_ms(start)/24;fprintf(f,"SIMULATION crowded traffic %.3f ms/tick\n",sim);
 for(int scene=1;scene<6;scene++){int ok=optimized[scene]+sim<=33.42;fprintf(f,"%s %s mean render + crowded simulation fits nominal 30 FPS CPU budget (%.3f ms)\n",ok?"PASS":"FAIL",names[scene],optimized[scene]+sim);fails+=!ok;}
 render_reference=0;camera_override_clear();mission_queries_end();free(fb);fb=saved;
 fprintf(f,"INFO CPU workload timings exclude vblank; emulator results do not certify real PSP FPS. checksum %d\n",sink);
 fprintf(f,"RESULT %d failures\n",fails);fclose(f);return 1;
}
