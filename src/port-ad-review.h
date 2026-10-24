#if defined(ELITE_ENABLE_REVIEW)||defined(ELITE_ENCOUNTER_REVIEW)
static unsigned departure_review_hash_482(const unsigned*p){unsigned h=2166136261u;for(int y=0;y<H;y++)for(int x=0;x<W;x++)h=(h^p[y*STRIDE+x])*16777619u;return h;}
static int port_ad_review(void){
 FILE*flag=fopen("port-ad-review.flag","r");if(!flag)return 0;fclose(flag);FILE*out=fopen("port-ad-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int checks=0,fails=0;
 #define PA(c,label) do{int ok=!!(c);checks++;fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);}while(0)
 port_ads_init();unsigned hashes[6];int distinct=1;for(int id=0;id<6;id++){hashes[id]=2166136261u;for(int i=0;i<2048;i++)hashes[id]=(hashes[id]^port_ad_art[id][i])*16777619u;for(int j=0;j<id;j++)distinct&=hashes[id]!=hashes[j];}PA(distinct,"six illustrated pixel-art adverts are distinct and use the actual game font");
 unsigned*prior=fb,*pixels=calloc(STRIDE*H,sizeof(unsigned));PA(pixels!=0,"allocate native framebuffer");if(!pixels){fclose(out);return 1;}fb=pixels;
 hc_test_setup();game.ship=0;game.system=7;system_bodies(&game);game.docked=1;game.station_variant=0;game.dockbay.dirty=0;launch_departure(&game);hc_cache_ready=0;hc_cache_dirty=1;hud_anim_ready=0;departure_release=0;port_queue_peak=0;
 int left=0,right=0,wall[2]={0},modern=1,frames=0;float min_z=1e9f,max_bend=0;unsigned shots=game.shots;float fuel=game.fuel,hull=game.hull;
 for(int frame=0;frame<243;frame++){
  input(0,0,1.f/30,0,0);if(frame%3)continue;memset(port_ad_written,0,sizeof(port_ad_written));space();frames++;
  left+=port_ad_written[0]>80;right+=port_ad_written[1]>80;wall[0]+=port_ad_written[16]>80;wall[1]+=port_ad_written[17]>80;
  if(game.dock_stage==4){modern&=hc_presentation_context()&&!hc_context();float travelled=dot(sub(game.pos,game.dock_from),game.dock_to),half=station_half_for(&game,game.station_variant);Vec3 axis=add(game.dock_from,mul(game.dock_to,travelled));float bend=length(sub(game.pos,axis));max_bend=fmaxf(max_bend,bend);if(bend>.1f)min_z=fminf(min_z,travelled-half);}
  char file[64];snprintf(file,sizeof(file),"port-native-%03d.bmp",frame);dump_native_bmp(file);
 }
 fprintf(out,"INFO Lave left building %.2fs right building %.2fs; wall pictures %.2f/%.2fs; samples %d; queue peak %d/2048\n",left*.1f,right*.1f,wall[0]*.1f,wall[1]*.1f,frames,port_queue_peak);
 PA(left>=10&&right>=10,"both main building ads have at least one second of visible illustrated face in the native viewport");PA(wall[0]>=8&&wall[1]>=8,"both early wall pictures remain visible long enough to recognise");PA(modern,"modern HUD remains present while guided controls stay disabled");PA(min_z>=99.5f&&max_bend>40,"gentle physical bend starts only after clearing the hull by 100m");PA(!game.dock_stage&&!departure_release&&game.shots==shots&&game.fuel==fuel&&game.hull==hull,"eight-second route completes without shooting boost fuel loss or damage");PA(port_queue_peak<1950,"traffic and port geometry retain triangle-queue headroom");
 hc_test_setup();game.docked=1;launch_departure(&game);departure_view_prepare(-110);Vec3 a=departure_view_point((Vec3){-69.5f,15,75}),b=departure_view_point((Vec3){-69.5f,15,135});Point aa=project(a),bb=project(b);PA(fabsf(aa.y-bb.y)>10&&fabsf(aa.x-bb.x)>40,"wall artwork is genuinely perspective angled rather than screen facing");
 for(int i=0;i<STRIDE*H;i++)pixels[i]=RGB(31,43,57);unsigned before=departure_review_hash_482(pixels);preview_clip(240,95,0,35,W,164);port_ad_map((Vec3){-30,20,150},(Vec3){30,20,150},(Vec3){30,-20,150},(Vec3){-30,-20,150},0,0);PA(before==departure_review_hash_482(pixels),"poster raster cannot overwrite foreground geometry or HUD pixels");preview_reset();
 int safe=1,clear=1,pending=1;for(int sys=0;sys<256;sys++){
  game_init(&game);story_complete(&game);game.system=sys;system_bodies(&game);game.docked=1;game.station_variant=0;launch_departure(&game);float saved_fuel=game.fuel,saved_hull=game.hull;
  for(int i=0;i<162&&game.dock_stage;i++)game_tick(&game,.05f,0,0,0,0);
  safe&=!game.dock_stage&&fabsf(game.speed-100)<.1f&&fabsf(game.roll)<.001f&&dot(forward(&game),game.dock_to)>.99999f&&game.fuel==saved_fuel&&game.hull==saved_hull;
  float travelled=dot(sub(game.pos,game.dock_from),game.dock_to);game.dock_stage=4;preview_clip(240,95,0,0,W,191);int submitted=render_submitted;departure_district(travelled,departure_grade());clear&=render_submitted==submitted;game.dock_stage=0;
 }
 PA(safe,"all 256 system departures finish smoothly at cruise speed with aligned heading and no fuel or hull loss");PA(clear,"all 256 station profiles clear staged buildings and traffic before the free-flight handoff");preview_reset();
 game_init(&game);story_complete(&game);game.docked=1;launch_departure(&game);game.dock_phase=1;for(int d=0;d<256;d++)if(d!=game.system&&distance_ly(&game,game.system,d)*10<=game.fuel){game.destination=d;break;}for(int i=0;i<150;i++)game_tick(&game,.05f,0,0,0,0);pending=game.dock_stage==4&&game.jump==0;for(int i=0;i<12;i++)game_tick(&game,.05f,0,0,0,0);PA(pending&&!game.dock_stage&&game.jump>0,"queued hyperspace remains deferred until the extended departure is clear");
 game_init(&game);story_complete(&game);game.opening.stage=OP_LAUNCH;game.docked=1;launch_departure(&game);game.opening.stage=OP_SOL_MAP;for(int i=0;i<98;i++)game_tick(&game,.05f,0,0,0,0);PA(game.dock_duration==5&&game.dock_stage==4,"authored Earth first ascent retains its original five-second timing");for(int i=0;i<4;i++)game_tick(&game,.05f,0,0,0,0);float axial=dot(sub(game.pos,game.dock_from),game.dock_to);PA(!game.dock_stage&&length(sub(game.pos,add(game.dock_from,mul(game.dock_to,axial))))<.01f,"first story ascent remains straight and completes without extending the city cutscene");
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,fails);fclose(out);fb=prior;free(pixels);return 1;
 #undef PA
}
#endif
