#if defined(ELITE_ENABLE_REVIEW)||defined(ELITE_ENCOUNTER_REVIEW)
static void nav_review_aim(Vec3 point){Vec3 d=norm(sub(point,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=asinf(d.y);game.roll=0;}
static void nav_review_frame(const char*name){hc_cache_dirty=1;drawcount=0;space();dump_native_bmp(name);}
static int nav_review_fly(Vec3 point,float*seconds){
 for(int f=0;f<18000;f++){
  float distance=length(sub(point,game.pos));if(distance<620&&game.speed<100)return 1;
  nav_review_aim(point);float desired=fminf(4000,fmaxf(0,(distance-400)*.45f));game.boost=distance>9000&&game.fuel>0;
  game.mega_brake=0;game_tick(&game,.1f,0,0,game.speed<desired?1:-1,0);*seconds+=.1f;
  if(game.dead||game.planet>=0||game.dock_stage)return 0;
 }return 0;
}
static void nav_review_dwell(void){for(int f=0;f<24;f++)game_tick(&game,.1f,0,0,-1,0);}
static int navigation_review(void){
 FILE*flag=fopen("navigation-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE*out=fopen("navigation-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int checks=0,fails=0;
 #define NC(c,s) do{int ok=!!(c);checks++;fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);}while(0)
 unsigned*pixels=malloc(STRIDE*H*sizeof(unsigned));if(pixels)fb=pixels;
 hc_test_setup();game.job_n=0;game.contract=-1;memset(&game.navigation,0,sizeof(game.navigation));game.credits=2000;
 NC(navigation_enabled(&game),"Lave free-flight exploration uses the integrated game");
 NC(!navigation_source(&game,0)&&!navigation_source(&game,3),"unknown stations and encounters are absent from permanent navigation");
 Vec3 hub=navigation_position(&game,0);game.pos=add(hub,(Vec3){0,0,-50000});selected_target=0;hc_validate_lock();
 NC(!hc_detected(0)&&selected_target<0,"old DOCK cue cannot retain a station lock outside scanners");
 NC(!navigation_select(&game,0),"unknown exact coordinates cannot be plotted");
 NC(navigation_source(&game,NAV_PLANET(1))==NAV_CHART,"visible planet navigation does not reveal its station");
 int money=game.credits;NC(rumour_purchase(13)==1&&game.credits==money-250,"remote GalacticNet buoy search charges its stated price");
 NC(!navigation_source(&game,15),"paid lead does not grant exact buoy coordinates");
 NC(rumour_purchase(13)==2&&game.credits==money-250,"buying the same local lead again never charges twice");
 NC(rumour_track(13)&&game.navigation.search_key==1,"tracking a paid search chooses an area, not a physical lock");
 Vec3 buoy=navigation_position(&game,15),area=navigation_search_position(&game);
 NC(length(sub(area,buoy))>500&&length(sub(area,buoy))<2000,"paid area contains the real buoy without giving its exact position");
 game.pos=add(buoy,(Vec3){0,0,-sensor_range()*1.5f});nav_trace_system=-1;
 for(int k=0;k<20;k++)nav_passive_tick(.05f);
 NC(nav_trace_key>=0,"passive reception stabilises without a ping");
 NC(!hc_detected(BUOY_ID_MIN),"faint buoy bearing is not a resolved live contact");
 nav_review_aim(buoy);nav_review_frame("nav486-faint.bmp");
 game.pos=add(buoy,(Vec3){0,0,-600});game.speed=0;game.boost=0;nav_review_aim(buoy);game.buoy_scan_slot=-1;
 navigation_tick(&game,1);NC(!game.navigation.scanned[7],"buoy download requires a dwell rather than an instant flyby");
 navigation_tick(&game,1.1f);NC(game.navigation.scanned[7]&1,"two-second close slow approach scans the physical buoy");
 int keys[4],n=navigation_links(&game,0,keys,4);NC(n==3,"buoy exposes only its three authored local links");
 NC(navigation_source(&game,16)&NAV_BUOY_SOURCE,"linked buoy coordinates have an independent supplied provider");
 NC(!navigation_source(&game,3)&&!navigation_source(&game,1),"first buoy does not reveal all encounters or remote facilities");
 selected_target=BUOY_ID_MIN;target_explicit=1;nav_review_frame("nav486-buoy.bmp");
 Vec3 leg_start=game.pos,leg_end=navigation_position(&game,16);float leg_fuel=game.fuel,leg_seconds=0;int reached=0;
 for(int f=0;f<18000;f++){
  float distance=length(sub(leg_end,game.pos));if(distance<620&&game.speed<100){reached=1;break;}
  nav_review_aim(leg_end);float desired=fminf(4000,fmaxf(0,(distance-400)*.45f));game.boost=distance>9000&&game.fuel>0;
  game.mega_brake=0;game_tick(&game,.1f,0,0,game.speed<desired?1:-1,0);leg_seconds+=.1f;
  if(game.dead||game.planet>=0||game.dock_stage)break;
 }
 NC(reached,"buoy-to-buoy journey completes with real flight, fuel and collision simulation");
 fprintf(out,"INFO Lave buoy leg %.0f m / %.1f seconds / %.3f boost-fuel units; deterministic steering fixture, not a human playthrough\n",length(sub(leg_end,leg_start)),leg_seconds,leg_fuel-game.fuel);
 game.pos=add(buoy,(Vec3){0,0,-600});game.speed=0;nav_review_aim(buoy);

 hc_category=5;hc_contacts();hc_highlight=NAV_UI_ID+16;hc_browser=1;
 int previous=selected_target;hc_browser_input(PSP_CTRL_CROSS,PSP_CTRL_SQUARE|PSP_CTRL_CROSS,.016f);
 NC(game.navigation.waypoint_key==17&&selected_target==previous,"navigation confirmation plots a return point without changing the live lock");
 nav_review_frame("nav486-local-leads.bmp");hc_browser=0;
 NC(!navigation_links(&game,3,keys,4),"an unscanned buoy does not disclose links");
 for(int b=2;b<=3;b++){game.pos=add(navigation_position(&game,NAV_BUOY(b)),(Vec3){0,0,-500});game.speed=0;navigation_tick(&game,1);navigation_tick(&game,1.1f);}
 NC(navigation_source(&game,3)&NAV_BUOY_SOURCE,"linked branch leads to the actual listening post");
 navigation_select(&game,3);Vec3 post=navigation_position(&game,3);game.pos=add(post,(Vec3){0,150,-700});game.speed=0;navigation_tick(&game,.05f);
 selected_target=DISCOVERY_ID_MIN;hail_target();
 NC((navigation_source(&game,3)&NAV_VISITED)&&game.voice_time>0,"arriving and hailing uses the real listening-post interaction and records the visit");
 nav_review_aim(post);nav_review_frame("nav486-listening-post.bmp");
 game.planet=1;game.surface=1;game.bodies[1].type=OCEAN;NC(navigation_planet_job(&game)==1,"planet courier uses a real delivery contract and cargo");
 NC(navigation_source(&game,0)&NAV_MISSION,"planet delivery supplies exact station coordinates through its own job");
 navigation_select(&game,0);abandon_mission(&game,game.job_n-1);navigation_tick(&game,.016f);
 NC(!(navigation_source(&game,0)&NAV_MISSION),"abandoning the courier removes mission-only station knowledge");
 game.job_n=0;game.contract=-1;navigation_visit(&game,0);game.planet=1;game.surface=1;navigation_planet_job(&game);abandon_mission(&game,game.job_n-1);
 NC(navigation_source(&game,0)&NAV_VISITED,"independent visited coordinates survive contract abandonment");
 game.planet=-1;game.surface=0;game.navigation.waypoint_key=4;game.navigation.waypoint_system=7;
 game.docked=1;game.station_variant=0;
 NC(save_game(&game,"nav486-test.sav"),"V39 writes a sealed commander with navigation provenance");
 Game*loaded=malloc(sizeof(Game));NC(loaded&&load_game(loaded,"nav486-test.sav")&&(loaded->navigation.visited[7]&(1u<<3))&&loaded->navigation.scanned[7]&&loaded->navigation.paid[7]==1,"visited places, scanned buoys and paid data survive reload");free(loaded);game.docked=0;
 game.pos=add(hub,(Vec3){0,0,-50000});selected_target=DISCOVERY_ID_MIN;autoaim=1;hc_validate_lock();
 NC(selected_target<0&&!autoaim&&game.navigation.waypoint_key==4,"losing a live lock leaves the separately saved waypoint intact");
 game.fuel=0;game.boost=1;game.speed=0;game_tick(&game,.1f,0,0,1,0);
 NC(!game.boost&&game.speed>0,"empty fuel disables boost while ordinary flight still accelerates");
 NC(!jump_start(&game),"empty fuel cannot start hyperspace");
 Vec3 centre=navigation_position(&game,21);float radius=spatial_region_radius(0);
 NC(spatial_density(&game,0,add(centre,(Vec3){radius*1.01f,0,0}))==0,"gas field ends at a finite world boundary");
 float a=spatial_density(&game,0,add(centre,(Vec3){radius*.99f,0,0})),b=spatial_density(&game,0,add(centre,(Vec3){radius*.98f,0,0}));
 NC(a>0&&b>a&&b-a<.01f,"outer cloud density ramps gradually rather than appearing in one frame");
 game.world_clock=600;game.pos=navigation_position(&game,22);float penalty=spatial_engine_factor(&game);
 NC(penalty>=.75f&&penalty<1,"magnetic field applies one bounded engine-output penalty");game.pos=add(game.pos,(Vec3){50000,0,0});
 NC(spatial_engine_factor(&game)==1,"leaving the magnetic field restores engine output");
 game.world_clock=300;game.pos=navigation_position(&game,22);NC(spatial_engine_factor(&game)==1,"charted magnetic region can be quiet when its weather has ended");
 game.fuel=30;game.speed=0;game.boost=0;game.world_clock=600;game.message_time=game.voice_time=0;selected_target=-1;
 for(int r=0;r<6;r++){
  centre=navigation_position(&game,21+r);game.pos=add(centre,(Vec3){0,1300,-spatial_region_radius(r)*.45f});nav_review_aim(centre);navigation_tick(&game,.05f);
  char name[48];snprintf(name,sizeof(name),"nav486-region-%d.bmp",r);nav_review_frame(name);
 }
 /* Native sequential frames follow one moving camera through the same field. */
 centre=navigation_position(&game,21);for(int f=0;f<40;f++){game.pos=add(centre,(Vec3){sinf(f*.035f)*1600,600,-17000+f*390.f});nav_review_aim(add(centre,(Vec3){1500,0,6000}));game.time+=.1f;game.world_clock+=.1f;navigation_tick(&game,.1f);char name[48];snprintf(name,sizeof(name),"nav486-cloud-%02d.bmp",f);nav_review_frame(name);}

 /* Shared geography is inspected after the Lave gate, using real jump completion. */
 hc_test_setup();game.job_n=0;game.contract=-1;game.debug_flags|=DEBUG_UNLIMITED_FUEL|DEBUG_UNLIMITED_RANGE;
 int world_safe=1,arrival_safe=1,quiet_arrivals=0,records_ok=1,buoys_distinct=1;
 for(int sys=0;sys<256;sys++){
  game.docked=game.dead=game.dock_stage=game.police_stop=0;game.planet=game.approach=-1;game.race.phase=0;game.speed=0;game.boost=0;game.destination=sys;game.system=(sys+13)%256;game.jump=.01f;game_tick(&game,.02f,0,0,0,0);
  if(game.system!=sys){arrival_safe=0;fprintf(out,"INFO jump did not reach system %d\n",sys);}
  float nearest=1e9f;for(int h=0;h<HUB_COUNT;h++){float distance=length(sub(game.pos,hub_position(&game,h)));if(distance<18000)arrival_safe=0;nearest=fminf(nearest,distance);}
  if(nearest>sensor_range()*1.2f*2.2f)quiet_arrivals++;
  for(int b=0;b<BODY_COUNT;b++)if(body_active(&game.bodies[b])&&length(sub(game.pos,game.bodies[b].pos))<game.bodies[b].radius+4900)arrival_safe=0;
  for(int key=3;key<27;key++)if(key<11||key>=15){Vec3 p=navigation_position(&game,key);if(!isfinite(p.x)||!isfinite(p.y)||!isfinite(p.z)||length(sub(p,hub_position(&game,0)))>300000){world_safe=0;fprintf(out,"INFO invalid or remote anchor system %d key %d\n",sys,key);}}
  for(int b=0;b<NAV_BUOYS;b++)for(int c=b+1;c<NAV_BUOYS;c++)if(length(sub(navigation_position(&game,NAV_BUOY(b)),navigation_position(&game,NAV_BUOY(c))))<1)buoys_distinct=0;
  if(discovery_count()!=8)records_ok=0;
 }
 NC(arrival_safe,"all 256 actual jump completions clear planets and nearby civilisation");
 NC(world_safe,"shared buoy, encounter and region anchors stay finite and local in sparse/multiple-star systems");
 NC(buoys_distinct,"all six physical buoys have distinct scannable positions in every system");
 NC(records_ok,"shared encounter catalogue is available in every free-exploration system");
 NC(quiet_arrivals>0,"some arrivals have no immediate useful civilisation-route reception");fprintf(out,"INFO %d / 256 arrival fixtures outside all station faint-reception envelopes\n",quiet_arrivals);

 /* Full Lave branch and saved return: only initial pose is injected. */
 hc_test_setup();game.job_n=0;game.contract=-1;game.credits=2000;game.pos=add(hub_position(&game,0),(Vec3){0,18000,-50000});game.speed=0;game.fuel=60;
 int bought=rumour_purchase(13)>0&&rumour_track(13);float journey=0,fuel_start=game.fuel;
 int loop=bought&&nav_review_fly(navigation_search_position(&game),&journey)&&nav_review_fly(navigation_position(&game,15),&journey);nav_review_dwell();loop=loop&&(game.navigation.scanned[7]&1);
 loop=loop&&navigation_select(&game,17)&&nav_review_fly(navigation_position(&game,17),&journey);nav_review_dwell();loop=loop&&(game.navigation.scanned[7]&(1u<<2));
 loop=loop&&navigation_select(&game,18)&&nav_review_fly(navigation_position(&game,18),&journey);nav_review_dwell();loop=loop&&(game.navigation.scanned[7]&(1u<<3));
 loop=loop&&navigation_select(&game,3)&&nav_review_fly(navigation_position(&game,3),&journey);selected_target=DISCOVERY_ID_MIN;hail_target();loop=loop&&(navigation_source(&game,3)&NAV_VISITED);
 NC(loop,"complete Lave paid-search / linked-buoys / listening-post loop flies through real simulation");
 int returned=loop&&navigation_select(&game,15)&&nav_review_fly(navigation_position(&game,15),&journey);
 NC(returned,"visited coordinates support the real return flight without repurchasing or rescanning");
 fprintf(out,"INFO full Lave branch and return %.1f seconds / %.3f fuel units (%.3f LY); script-steered actual simulation, no teleport between legs\n",journey,fuel_start-game.fuel,(fuel_start-game.fuel)*.1f);
 /* Real ordinary-flight recovery, with no money and no fuel. */
 hc_test_setup();game.fuel=0;game.credits=0;game.pos=add(navigation_position(&game,15),(Vec3){0,0,-16000});game.speed=0;float recovery=0;
 int recover=nav_review_fly(navigation_position(&game,15),&recovery);nav_review_dwell();NC(recover&&game.fuel==0&&game.credits==0&&(game.navigation.scanned[7]&1),"zero-money zero-fuel ordinary flight can find and scan a buoy");fprintf(out,"INFO zero-fuel 16 km recovery %.1f seconds\n",recovery);
 NC(rumour_purchase(13)==2&&game.credits==0&&!game.navigation.paid[7],"already scanned buoy directions remain free even with no money");
 navigation_visit(&game,3);NC(rumour_purchase(0)==2&&game.credits==0,"a visited fixed encounter is never charged again for directions");
 /* Route spool remains functional without using a weapon/contact lock. */
 hc_test_setup();for(int sys=0;sys<256;sys++)if(sys!=7&&distance_ly(&game,7,sys)*10<game.fuel){game.destination=game.route_goal=sys;break;}
 nav_ui_select(NAV_VIRTUAL_ID+ROUTE_TARGET_ID);nav_follow=1;Vec3 direction=route_target_direction();game.yaw=atan2f(direction.x,direction.z);game.pitch=asinf(direction.y);game.boost=1;selected_target=-1;
 for(int f=0;f<70&&game.jump<=0;f++)input(0,PSP_CTRL_RTRIGGER,.1f,0,0);
 NC(game.jump>0&&route_jump_engaged&&selected_target<0,"navigation-only route alignment still enters the existing hyperspace sequence");
 /* Incoming encounters remain transient; only fixed keys enter the nav record. */
 hc_test_setup();memset(&game.navigation,0,sizeof(game.navigation));unsigned before=game.navigation.visited[7];game.pos=(Vec3){300000,200000,300000};game.npc[0].pos=game.pos;navigation_tick(&game,.05f);game.npc[0].alive=0;
 NC(game.navigation.visited[7]==before,"a passing ship leaves no permanent navigation destination");
 /* Existing bought data remains remotely usable after the new local offers. */
 game.rumours.owned[7]|=1;game.rumours.paid[7][0]=550;rumour_page_system=-1;rumour_layout();int legacy_listed=0;for(int i=0;i<rumour_links_n;i++)if(rumour_links[i].slot==0)legacy_listed=1;
 NC(legacy_listed&&rumour_track(0),"previously bought legacy sightings remain listed and trackable");
 Vec3 legacy_point;NC(nav_active_point(&legacy_point)&&!hc_detected(RUMOUR_TARGET_ID),"legacy paid area is navigation data without becoming a live scanner contact");
 rumour_page_system=-1;page=GALNET;galnet_tab=5;hc_cache_dirty=1;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("nav486-paid-leads.bmp");
 /* Verify existing shipped commander formats without changing their files. */
 loaded=malloc(sizeof(Game));NC(loaded&&load_game(loaded,"legacy-v38.sav"),"existing V38 commander migrates to navigation-aware code");free(loaded);
 loaded=malloc(sizeof(Game));NC(loaded&&load_game(loaded,"legacy-v37.sav"),"existing V37 commander remains readable");free(loaded);
 fprintf(out,"INFO footage: 40 native 480x272 sequential renderer frames; injected camera path, not a hardware FPS measurement\n");
 fprintf(out,"RESULT %d failures / %d checks\n",fails,checks);fclose(out);free(pixels);
 #undef NC
 return 1;
}
#endif
