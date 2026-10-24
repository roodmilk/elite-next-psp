/* Opt-in isolated retail benchmark: normal game loop, mixer and display path.
 * No commander is loaded or saved. A private flag selects the encounter. */
static int charm_capture_item=-1,dashboard_capture_item=-1,twin_capture_ship=-1;
static int encounter_capture_nav=-1;
static int encounter_capture_scene=-1,encounter_capture_hud=-1;static int twin_capture_footage=0;static FILE*encounter_capture_file;static int encounter_capture_reported;static unsigned encounter_capture_vblank_reuse;
static double encounter_capture_seconds[3];static int encounter_capture_frames[3];
static float encounter_capture_worst[3];static int capture_boost_frames;static float capture_peak_speed;
static uint64_t encounter_capture_mark,encounter_capture_us[3][3];static int encounter_capture_band;
static void encounter_capture_begin(int frame){encounter_capture_band=frame/200;if(encounter_capture_band>2)encounter_capture_band=2;encounter_capture_mark=sceKernelGetSystemTimeWide();}
static void encounter_capture_section(int section){if(encounter_capture_scene<0)return;uint64_t now=sceKernelGetSystemTimeWide();encounter_capture_us[encounter_capture_band][section]+=now-encounter_capture_mark;encounter_capture_mark=now;}
static const char *encounter_capture_names[]={"outer relay","frontier outpost","animal carrier","sanctuary","cinema A","closed resort","mobile mechanic","cinema B","salvage village","shipbreakers yard","community greenhouse","patchwork fuel stop","old passenger ferry","local radio shack","miners tea barge","cooperative ore depot","parts exchange","beacon repair crew"};
static int encounter_capture_setup(void){
 FILE*f=fopen("encounter-flight-capture.flag","r");if(!f)return 0;int scene=-1;fscanf(f,"%d",&scene);fclose(f);if(scene>=45&&scene<=51){encounter_capture_nav=scene-45;scene=8;}if(scene<0||scene>44)return 0;if(scene>=18){encounter_capture_hud=scene-18;scene=8;}
 if(scene==8)discovery_capture_entry=1;
 if(scene==9)discovery_capture_entry=15;
 if(scene==10)discovery_capture_entry=3;
 if(scene==11)discovery_capture_entry=0;
 if(scene==12)discovery_capture_entry=4;
 if(scene==13)discovery_capture_entry=14;
 if(scene==14)discovery_capture_entry=6;
 if(scene==15)discovery_capture_entry=8;
 if(scene==16)discovery_capture_entry=9;
 if(scene==17)discovery_capture_entry=10;
 FILE*ship=fopen("cockpit-ship.flag","r");if(ship){fscanf(ship,"%d",&twin_capture_ship);fclose(ship);if(twin_capture_ship<0||twin_capture_ship>=player_ship_count)twin_capture_ship=-1;}
 FILE*unclipped=fopen("cockpit-unclipped.flag","r");if(unclipped){hc_world_clip_enabled=0;fclose(unclipped);}
 FILE*footage=fopen("cockpit-footage.flag","r");if(footage){twin_capture_footage=1;fclose(footage);}
 encounter_capture_file=fopen("encounter-flight-capture.txt","w");encounter_capture_scene=scene;render_profile_enabled=1;game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);game_spawn(&game);game.opening.stage=OP_FREE;game.tutorial_step=0;game.docked=0;game.planet=game.approach=-1;game.speed=0;game.voice_time=game.message_time=0;page=FLIGHT;third_person=0;hud_mode=hud_hidden=0;radio_off=1;comms_quiet=1;
 if(encounter_capture_hud>=0){
  game.ship=14;fit_clear_all(&game);game.fit[0]=1;game.fit[12]=69;game.fit[30]=2;game.fit[42]=26;fit_rebuild(&game);game.active_weapon=0;
  if(encounter_capture_hud==2||encounter_capture_hud==4){game.ship=0;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);}
  hc_layout_errors=0;memset(hc_profile_us,0,sizeof(hc_profile_us));hc_profile_frames=0;hc_copy_us=0;hc_paint_count=hc_overlay_calls=0;hc_cache_ready=0;hc_cache_dirty=1;hc_cache_clear=1;hc_category=1;hc_refresh=0;tools_selected=0;game.missiles=3;game.flare_charges=2;
 }
 if(encounter_capture_nav>=0){game.world_clock=600;for(int ship=0;ship<player_ship_count;ship++)if(!strcmp(player_ships[ship].name,"ANACONDA"))game.ship=ship;fit_clear_all(&game);game.fit[0]=1;game.fit[6]=2;game.fit[12]=69;game.fit[18]=26;fit_rebuild(&game);game.active_weapon=0;}
 if(encounter_capture_nav>=0||twin_capture_footage||encounter_capture_hud>=23){radio_off=0;radio_station=7;radio_volume=5;audio_title_active=0;radio_static_ms=0;}
 if(encounter_capture_hud>=9){game.ship=0;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;hc_browser=0;tools_reset_gesture();}
 FILE*charm=fopen("charm-capture.flag","r");if(charm){int id=0;fscanf(charm,"%d",&id);fclose(charm);if(id>=1&&id<=4){charm_capture_item=CHARM_UFO+id-1;game.fit[FIT_UTIL]=charm_capture_item;fit_rebuild(&game);}}
 FILE*dashboard=fopen("dashboard-capture.flag","r");if(dashboard){int id=0;fscanf(dashboard,"%d",&id);fclose(dashboard);if(id>=1&&id<=16){dashboard_capture_item=DASHBOARD_FIRST+id-1;game.fit[FIT_HOLD]=dashboard_capture_item;fit_rebuild(&game);}}
 return 1;
}
static void twin_capture_pose(int frame){
 int mode=encounter_capture_hud;
 if(mode==26){if(frame==0){int kind=1;FILE*pk=fopen("port-capture-kind.flag","r");if(pk){fscanf(pk,"%d",&kind);fclose(pk);}if(kind!=1){int wanted=kind==0?STATION_POOR:STATION_MEGA;for(int sys=0;sys<256;sys++)if(station_class_for_system(&game,sys)==wanted){game.system=sys;break;}system_bodies(&game);game_spawn(&game);}game.docked=1;game.station_variant=kind==3?1:0;launch_departure(&game);r_tap=l_tap=10;hc_browser=0;tools_reset_gesture();selected_target=-1;hc_cache_dirty=1;}return;}
 if(mode>=24){if(frame==0){game.pos=add(hub_position(&game,0),(Vec3){4000,0,-12000});game.yaw=game.pitch=game.roll=0;game.speed=0;game.boost=0;r_tap=l_tap=10;hc_browser=0;selected_target=-1;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;flight_camera_seen=-1;}return;}
 Vec3 c=discovery_position(4);game.pos=add(c,(Vec3){0,400,-4200});game.yaw=0;game.pitch=-.04f;game.roll=0;game.speed=0;game.boost=0;game.voice_time=game.message_time=0;hc_browser=0;tools_reset_gesture();selected_target=-1;autoaim=0;third_person=mode==14?1:0;high_contrast=mode==17;flight_camera_seen=-1;
 if(mode==9||mode==10){game.yaw=.45f;game.pitch=.04f;}
 if(mode==19||mode==13){game.pos=add(hub_position(&game,0),(Vec3){0,0,-5000});game.pitch=0;}
 if(mode==20){int b=-1;for(int i=1;i<BODY_COUNT;i++)if(body_active(&game.bodies[i])&&game.bodies[i].type!=SUN){b=i;break;}if(b>=0){game.pos=add(game.bodies[b].pos,(Vec3){0,0,-game.bodies[b].radius-6000});game.pitch=0;}}
 if(frame==0){game.ship=twin_capture_ship>=0?twin_capture_ship:mode==16?14:0;fit_clear_all(&game);game.fit[0]=mode==15?26:1;if(mode==16){game.fit[6]=2;game.fit[12]=69;game.fit[18]=29;}fit_rebuild(&game);game.active_weapon=0;game.shot=0;}
 if(mode==22&&frame%100==0){game.ship=(frame/100)%player_ship_count;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);game.active_weapon=0;}
 Vec3 f=forward(&game);NPC*n=&game.npc[0];if(!n->alive||frame==0){memset(n,0,sizeof(*n));n->alive=1;n->mesh=mesh_id("ADDER");n->scale=1;n->radius=30;n->mega_hull=-1;n->health=80;n->shield=mode==12?0:40;n->bounty_slot=n->traveller=n->target=-1;n->name_known=1;n->role=PIRATES;}
 n->pos=add(game.pos,mul(f,mode==13?10000:1800));if(mode==9||mode==10)n->pos=add(n->pos,(Vec3){350,0,0});n->dir=f;n->cruise=0;n->cooldown=100;
 if(mode==18){for(int i=1;i<24;i++){NPC*q=&game.npc[i];if(!q->alive){*q=*n;q->alive=1;}q->pos=add(game.pos,(Vec3){(i%7-3)*230.f,(i%3-1)*90.f,1000+i*180.f});q->role=PIRATES;q->target=-2;q->dir=norm(sub(game.pos,q->pos));q->cruise=350;q->cooldown=fminf(.4f,q->cooldown);}}
 game.dead=0;game.hull=fmaxf(90,game.hull);game.energy=fmaxf(80,game.energy);game.heat=fminf(65,game.heat);fire_blocked=0;hc_cache_dirty|=frame==0;
}
static unsigned twin_capture_controls(int frame){
 if(encounter_capture_hud==26)return 0;
 if(encounter_capture_hud>=24){if(frame<5)return 0;if(encounter_capture_hud==25&&frame==6)return 0;return PSP_CTRL_RTRIGGER;}
 if(encounter_capture_hud<9)return 0;
 if(encounter_capture_hud==16&&frame%120>=100&&frame%120<103)return PSP_CTRL_CIRCLE|(frame%120==100?PSP_CTRL_RIGHT:0);
 return encounter_capture_hud==9||encounter_capture_hud==22||frame<20?0:PSP_CTRL_CROSS;
}
static void encounter_capture_pose(int frame){
 if(encounter_capture_scene<0)return;
 if(encounter_capture_nav>=0){
  int region=encounter_capture_nav;Vec3 c=region<6?navigation_position(&game,21+region):add(hub_position(&game,0),(Vec3){0,70000,-90000});
  float radius=region<6?spatial_region_radius(region):12000,angle=frame*.002f;
  game.pos=add(c,(Vec3){sinf(angle)*radius*.4f,800,-cosf(angle)*radius*(frame<200?1.1f:frame<400?.55f:.12f)});
  Vec3 aim=norm(sub(c,game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;game.speed=0;game.boost=0;game.voice_time=game.message_time=0;hc_browser=0;selected_target=-1;flight_camera_seen=-1;return;
 }
 if(encounter_capture_hud>=9){twin_capture_pose(frame);if(charm_capture_item>=0&&game.fit[FIT_UTIL]!=charm_capture_item){game.fit[FIT_UTIL]=charm_capture_item;fit_rebuild(&game);}if(dashboard_capture_item>=0&&game.fit[FIT_HOLD]!=dashboard_capture_item){game.fit[FIT_HOLD]=dashboard_capture_item;fit_rebuild(&game);}return;}int scene=encounter_capture_scene,slot=scene>=8?4:scene<2?-scene-1:scene,band=frame/200;if(band>2)band=2;
 Vec3 c=slot<0?hub_position(&game,-slot):discovery_position(slot);float range=slot<0?station_profile_for(&game,-slot).radius*5.5f:scene>=8?4200:3400;
 float orient=slot<0?-slot*.47f:(discovery_seed(slot)&255)*.006f,a=-orient+(frame%200)*6.2831853f/200,dist=range*(band==0?1:band==1?1.6f:2.8f);
 game.pos=add(c,(Vec3){sinf(a)*dist,dist*(.15f+sinf(a)*.2f),-cosf(a)*dist});Vec3 focus=c;if(scene==13)focus.y-=200;else if(scene==8||scene==15)focus.y-=300;Vec3 aim=norm(sub(focus,game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;game.speed=0;game.boost=0;game.voice_time=game.message_time=0;flight_camera_seen=-1;selected_target=slot<0?STATION_TARGET_ID(-slot):DISCOVERY_ID_MIN+slot;
 if(encounter_capture_hud>=0){
  int mode=encounter_capture_hud;hc_force_legacy=mode==7;hud_mode=mode==8?2:0;hud_hidden=mode==8;Vec3 f=forward(&game),right={cosf(game.yaw),0,-sinf(game.yaw)};
  for(int i=0;i<2;i++){NPC*n=&game.npc[i];n->alive=1;n->mesh=mesh_id(i?"VIPER":"ADDER");n->scale=1;n->radius=30;n->mega_hull=-1;n->cruise=650;n->dir=f;n->role=i?LAW:TRADERS;n->freighter=0;n->health=i?100:70;n->shield=i?55:27;n->bounty_slot=-1;n->traveller=-1;n->name_known=1;n->target=-1;n->cooldown=100;n->pos=add(add(game.pos,mul(f,2200+i*1100)),mul(right,i?-300:350));}
  game.missiles=3;selected_target=mode==0?-1:mode==6&&frame>=120?NPC_ID_MIN+1:NPC_ID_MIN;target_explicit=selected_target>=0;autoaim=0;
  hc_browser=mode==1||mode==3||mode==4||mode==6;hc_highlight=NPC_ID_MIN+1;hc_contacts();
  tools_open=mode>=1&&mode<=5;tools_armed=tools_open;tools_hold=1;tools_consumed=1;tools_confirm_time=.3f;tools_confirm_action=3;tools_weapon_result=1;
  if(mode==3){game.incoming_missile=4;game.incoming_source=1;}
  if(mode==4){game.missiles=0;game.flare_charges=0;game.flare_cd=3;game.upgrades&=~2048;tools_selected=3;}
  if(mode==5)game.active_weapon=(frame%200)<100?0:12;
 }

}
static void encounter_capture_record(float dt,int frame){
 if(game.boost)capture_boost_frames++;capture_peak_speed=fmaxf(capture_peak_speed,game.speed);
 if(encounter_capture_scene<0||frame<=30)return;int band=(frame-1)/200;if(band>2)band=2;
 encounter_capture_seconds[band]+=dt;encounter_capture_frames[band]++;if(dt>encounter_capture_worst[band])encounter_capture_worst[band]=dt;
}
static void encounter_capture_report(void){
 if(encounter_capture_scene<0||encounter_capture_reported)return;FILE*f=encounter_capture_file?encounter_capture_file:fopen("encounter-flight-capture.txt","w");if(!f)return;int fail=0;
 if(encounter_capture_hud==26)fprintf(f,"INFO port queue peak %d / 2048; six native pixel posters; guided launch duration 8s\n",port_queue_peak);
 fprintf(f,"DASHBOARD render calls %u mean %.6f ms; exact static supplied sprites\n",dashboard_draw_calls,dashboard_draw_calls?dashboard_draw_us*.001/dashboard_draw_calls:0);
 fprintf(f,"CHARM render calls %u mean %.6f ms; exact supplied sprites; native inverse-nearest rotation\n",charm_draw_calls,charm_draw_calls?charm_draw_us*.001/charm_draw_calls:0);
 fprintf(f,"INFO retail 600-frame orbit of %s; PPSSPP PSP-3000; real mixer/display; hardware testing pending\n",encounter_capture_nav>=0?(encounter_capture_nav<6?nav_region_names[encounter_capture_nav]:"quiet space"):encounter_capture_names[encounter_capture_scene]);
 if(encounter_capture_nav>=0)fprintf(f,"INFO equipped ANACONDA; four fitted weapon banks; current 3D Twin Stack cockpit housings rendered\n");
 fprintf(f,"PROFILE clouds build %.3f depth %.3f GPU %.3f ms/frame; single atlas batch\n",spatial_profile_us[0]*.001/fmaxf(1,spatial_profile_frames),spatial_profile_us[1]*.001/fmaxf(1,spatial_profile_frames),spatial_profile_us[2]*.001/fmaxf(1,spatial_profile_frames));
 fprintf(f,"INFO cloud GPU passes %u cards %u; identical 64px alpha assets and world layers\n",spatial_ge_calls,spatial_ge_cards);
 fprintf(f,"INFO real boost frames %d peak speed %.2f\n",capture_boost_frames,capture_peak_speed);
 if(encounter_capture_hud>=9)fprintf(f,"INFO Twin Stack real-input fixture mode %d; fixed camera; repeated targets; heat/health protected for sustained rendering load; footage capture %d\n",encounter_capture_hud,twin_capture_footage);
 if(encounter_capture_hud>=0){fprintf(f,"INFO staged native cockpit mode %d; layout overflow counter %d\n",encounter_capture_hud,hc_layout_errors);fprintf(f,"PROFILE HUD header %.3f target %.3f instruments %.3f comms %.3f tools %.3f copy %.3f ms/view\n",hc_profile_us[0]*.001/fmaxf(1,hc_profile_frames),hc_profile_us[1]*.001/fmaxf(1,hc_profile_frames),hc_profile_us[2]*.001/fmaxf(1,hc_profile_frames),hc_profile_us[3]*.001/fmaxf(1,hc_profile_frames),hc_profile_us[4]*.001/600,hc_copy_us*.001/600);fprintf(f,"INFO HUD repaint %u cached overlays %u\n",hc_paint_count,hc_overlay_calls);if(hc_layout_errors){fail++;fprintf(f,"FAIL cockpit text fields exceeded their reserved lines\n");}}
 for(int band=0;band<3;band++){double fps=encounter_capture_seconds[band]>0?encounter_capture_frames[band]/encounter_capture_seconds[band]:0;int ok=fps>=29.5;fail+=!ok;fprintf(f,"%s %s orbit %.2f FPS; worst %.2f ms; %d frames\n",ok?"PASS":"FAIL",encounter_capture_hud>=9?(band==0?"window 1":band==1?"window 2":"window 3"):(band==0?"near":band==1?"medium":"distant"),fps,encounter_capture_worst[band]*1000,encounter_capture_frames[band]);fprintf(f,"PROFILE band %d input/audio %.3f draw %.3f present/wait %.3f ms/frame\n",band,encounter_capture_us[band][0]*.001/200,encounter_capture_us[band][1]*.001/200,encounter_capture_us[band][2]*.001/200);}
 int audio_ok=audio_buffer_starves==0&&audio_command_drops==0;fail+=!audio_ok;fprintf(f,"%s audio buffers: starves %u drops %u\n",audio_ok?"PASS":"FAIL",audio_buffer_starves,audio_command_drops);
 fprintf(f,"INFO reused %u already-active vblank intervals\n",encounter_capture_vblank_reuse);
 fprintf(f,"INFO GPU ready %d fleet calls %u textured triangles %u\nRESULT %d failures\n",ion_gu_ready,fleet_ge_calls,fleet_ge_triangles,fail);fclose(f);encounter_capture_file=0;encounter_capture_reported=1;
}
