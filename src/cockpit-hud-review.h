#if defined(ELITE_ENABLE_REVIEW)||defined(ELITE_ENCOUNTER_REVIEW)
static void hc_test_setup(void){
 game_init(&game);deck_reset();hc_force_legacy=0;hc_cache_dirty=1;story_complete(&game);game.system=7;system_bodies(&game);game_spawn(&game);game.opening.stage=OP_FREE;game.tutorial_step=0;
 page=FLIGHT;hud_mode=hud_hidden=third_person=0;game.docked=game.dead=0;game.planet=game.approach=-1;game.jump=game.dock_stage=game.police_stop=0;game.pos=(Vec3){0,0,-20000};game.yaw=game.pitch=game.roll=0;game.speed=100;
 paused=comms_quick=ps_open=ps_release=departure_release=planet_landing_menu=0;game.voice_time=game.message_time=0;game.rift_report=0;night_wait=480;tracked_mission=0;fire_blocked=0;
 tools_reset_gesture();tools_wait_release=0;tools_selected=0;tools_confirm_time=0;tools_cancel_tractor();hc_browser=0;hc_category=1;hc_refresh=0;sensor_hud_range=-1;selected_target=-1;autoaim=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;
 for(int i=0;i<3;i++){NPC*n=&game.npc[i];n->alive=1;n->role=TRADERS;n->mesh=mesh_id(i==1?"VIPER":"ADDER");n->scale=1;n->radius=30;n->mega_hull=-1;n->target=-1;n->bounty_slot=-1;n->traveller=-1;n->freighter=0;n->health=100;n->shield=40;n->name_known=1;n->cooldown=100;n->pos=add(game.pos,(Vec3){i*300.f,0,i==0?3000:1000+i*1000.f});}
 game.ship=14;fit_clear_all(&game);game.fit[0]=1;game.fit[12]=69;game.fit[30]=2;game.fit[42]=26;fit_rebuild(&game);game.active_weapon=0;
 input(0,0,.016f,0,0);tools_wait_release=0;fire_blocked=0;
}
static int cockpit_hud_review(void){
 FILE*flag=fopen("cockpit-hud-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE*out=fopen("cockpit-hud-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int fails=0,checks=0;
 #define HC_CHECK(c,s) do{int ok=!!(c);checks++;fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);}while(0)
 hc_test_setup();input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,PSP_CTRL_CIRCLE,.21f,0,0);
 HC_CHECK(tools_open&&game.active_weapon==0,"hold Circle opens without changing the equipped gun");
 int expected[]={12,30,42,0};
 for(int k=0;k<4;k++){input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);HC_CHECK(game.active_weapon==expected[k]&&tools_open,"each Right tap during one Circle hold immediately equips the next fitted bank and wraps");input(0,PSP_CTRL_CIRCLE,.016f,0,0);}
 for(int k=0;k<20;k++)input(0,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);
 HC_CHECK(game.active_weapon==0,"holding Right does not auto-repeat weapon changes");
 int ammo=game.missiles;input(0,0,.016f,0,0);HC_CHECK(!tools_open&&game.active_weapon==0&&game.missiles==ammo,"Circle release closes immediately, keeps the equipped gun and cannot fire a tool");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);float yaw=game.yaw,time=game.time;Vec3 pos=game.pos;
 input(0,PSP_CTRL_CIRCLE,.1f,.8f,.5f);HC_CHECK(game.yaw!=yaw&&game.time>time&&length(sub(game.pos,pos))>0,"analog steering, movement and simulation continue while the weapon computer is held");input(0,0,.016f,0,0);
 hc_test_setup();game.ship=0;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);
 HC_CHECK(game.active_weapon==0&&tools_weapon_result==2&&fit_capacity(game.ship,FIT_WPN)==1,"one-slot hull keeps its only gun and reports a non-error result");input(0,0,.016f,0,0);
 fit_clear_all(&game);fit_rebuild(&game);input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);
 HC_CHECK(fit_weapon_item(&game)==FIT_EMPTY&&tools_weapon_result<0,"empty loadout never equips an empty slot as a gun");input(0,0,.016f,0,0);
 HC_CHECK(!strcmp(hc_tool_reason(4),"NO UNIT")&&!strcmp(hc_tool_reason(3),"NO UNIT"),"missing law scanner and heat sink remain represented with actual equipment reasons");
 game.missiles=0;game.flare_charges=0;HC_CHECK(!strcmp(hc_tool_reason(0),"NO AMMO")&&!strcmp(hc_tool_reason(1),"NO CHARGE"),"unavailable shoulder tools retain their ammo and charge reasons");
 hc_test_setup();input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_TRIANGLE,PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE,.016f,0,0);
 HC_CHECK(tools_selected==1&&tools_open&&!game.boost,"Circle plus Triangle selects flares without leaking into boost");
 int tool_before=tools_selected;input(PSP_CTRL_RTRIGGER,PSP_CTRL_CIRCLE|PSP_CTRL_RTRIGGER,.016f,0,0);
 HC_CHECK(tools_selected==tool_before&&!game.boost,"Circle plus R is unassigned and cannot change tools or trigger boost");
 input(PSP_CTRL_LTRIGGER,PSP_CTRL_CIRCLE|PSP_CTRL_LTRIGGER,.016f,0,0);HC_CHECK(tools_selected==0&&tools_open,"Circle plus L selects missiles in the left shoulder bay");
 input(PSP_CTRL_UP,PSP_CTRL_CIRCLE|PSP_CTRL_UP,.016f,0,0);HC_CHECK(tools_selected==4,"Circle plus Up selects law scan");
 input(PSP_CTRL_LEFT,PSP_CTRL_CIRCLE|PSP_CTRL_LEFT,.016f,0,0);HC_CHECK(tools_selected==2,"Circle plus Left preserves tractor selection");
 input(PSP_CTRL_DOWN,PSP_CTRL_CIRCLE|PSP_CTRL_DOWN,.016f,0,0);HC_CHECK(tools_selected==3,"Circle plus Down selects the heat sink");
 input(PSP_CTRL_TRIANGLE,PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE,.016f,0,0);HC_CHECK(tools_selected==1,"Circle plus Triangle selects flares; Down retains heat sink");input(0,0,.016f,0,0);
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_DOWN,PSP_CTRL_CIRCLE|PSP_CTRL_DOWN,.016f,0,0);input(0,0,.016f,0,0);
 game.upgrades|=2048;game.heat=90;game.heat_sink_cd=0;input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(0,0,.016f,0,0);
 HC_CHECK(game.heat<51&&game.heat_sink_cd>29,"a fresh Circle tap still activates the selected tool through the existing equipment system");
 hc_test_setup();selected_target=NPC_ID_MIN;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);input(0,PSP_CTRL_SQUARE,.016f,0,0);
 HC_CHECK(hc_browser&&page==FLIGHT&&selected_target==NPC_ID_MIN,"Square opens one in-flight target computer without changing the lock");
 int sorted=1;float last=-1;for(int k=0;k<hc_count;k++){float d=length(sub(target_raw_position(hc_ids[k]),game.pos));sorted&=d>=last;last=d;}
 HC_CHECK(sorted&&hc_count==3,"browser contains detected contacts sorted by real distance");
 input(PSP_CTRL_UP,PSP_CTRL_SQUARE|PSP_CTRL_UP,.016f,0,0);HC_CHECK(hc_highlight!=selected_target&&selected_target==NPC_ID_MIN,"browsing changes only the highlight; current target details retain their lock");
 int highlight=hc_highlight;input(PSP_CTRL_CROSS,PSP_CTRL_SQUARE|PSP_CTRL_CROSS,.016f,0,0);HC_CHECK(selected_target==highlight&&hc_browser,"Cross confirms the highlighted contact and leaves the computer open");input(0,PSP_CTRL_SQUARE,.016f,0,0);
 int locked=selected_target;input(PSP_CTRL_RIGHT,PSP_CTRL_SQUARE|PSP_CTRL_RIGHT,.016f,0,0);HC_CHECK(hc_category==2&&selected_target==locked,"category changes do not acquire another target");
 input(0,0,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
 HC_CHECK(hc_browser&&hc_category==2&&selected_target==locked,"closing and reopening remembers the last category and current lock");input(0,PSP_CTRL_SQUARE,.016f,0,0);
 hc_category=1;hc_contacts();game.npc[2].pos=add(game.pos,(Vec3){0,0,sensor_range()*3});hc_contacts();int hidden=1;for(int i=0;i<hc_count;i++)hidden&=hc_ids[i]!=NPC_ID_MIN+2;
 HC_CHECK(hidden,"contacts beyond scanner range never appear in the browser");
 hc_highlight=NPC_ID_MIN+2;input(PSP_CTRL_CROSS,PSP_CTRL_SQUARE|PSP_CTRL_CROSS,.016f,0,0);HC_CHECK(selected_target==locked,"confirmation rejects a highlighted contact that has just left range");
 game.npc[selected_target-NPC_ID_MIN].pos=add(game.pos,(Vec3){0,0,sensor_range()*4});input(0,PSP_CTRL_SQUARE,.016f,0,0);
 HC_CHECK(selected_target==-1&&!autoaim&&!target_explicit,"leaving scanner range clears the lock without silently switching to a looked-at object");
 selected_target=0;game.pos=add(hub_position(&game,0),(Vec3){0,0,-sensor_range()*4});hc_validate_lock();HC_CHECK(selected_target==-1,"primary station ID zero also respects the physical scanner boundary");
 hc_test_setup();game.pos=add(hub_position(&game,0),(Vec3){0,0,-4000});selected_target=-1;hc_category=2;hc_browser=1;hc_contacts();hc_highlight=0;
 r_tap=0;input(PSP_CTRL_CROSS|PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,.016f,0,0);
 HC_CHECK(selected_target==0&&autoaim&&!game.boost&&r_tap>1,"R plus Cross confirms station alignment and still satisfies the First Light lock objective");
 input(0,PSP_CTRL_CROSS,.016f,0,0);
 HC_CHECK(!hc_browser&&fire_blocked,"closing while confirmation remains held blocks accidental laser fire");input(0,PSP_CTRL_SQUARE,.016f,0,0);
 hc_test_setup();hc_browser=1;tools_open=1;float px=-200,py=260;hc_indicator_anchor(&px,&py);
 HC_CHECK(px>=172&&px<=224&&py>=72&&py<=130,"urgent bearing indicators stay clear of both computers and the instrument text");
 hc_test_setup();selected_target=NPC_ID_MIN;hc_browser=1;game.incoming_missile=4;game.incoming_source=1;game.message_time=3;snprintf(game.message,sizeof(game.message),"URGENT TEST");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.016f,0,0);input(0,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.21f,0,0);
 HC_CHECK(hc_browser&&tools_open&&game.incoming_missile>0,"urgent missile and message states cannot close either computer");
 input(0,PSP_CTRL_SQUARE,.016f,0,0);yaw=game.yaw;time=game.time;pos=game.pos;input(0,PSP_CTRL_SQUARE,.1f,.8f,.4f);
 HC_CHECK(hc_browser&&game.time>time&&game.yaw!=yaw&&length(sub(game.pos,pos))>0,"browsing preserves analog flight and world simulation");
 int power=game.pip_sys;input(PSP_CTRL_START,PSP_CTRL_START,.016f,0,0);input(PSP_CTRL_UP,PSP_CTRL_START|PSP_CTRL_UP,.016f,0,0);
 HC_CHECK(paused&&!hc_browser&&game.pip_sys>=power,"Start power controls remain available and releasing Square closes the browser");input(0,PSP_CTRL_SQUARE,.016f,0,0);
 hc_test_setup();game.missiles=4;change_page(HOME);input(PSP_CTRL_CIRCLE,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.016f,0,0);for(int k=0;k<20;k++)input(0,PSP_CTRL_CIRCLE,.016f,0,0);
 HC_CHECK(page==FLIGHT&&!tools_open&&game.missiles==4,"menu Back cannot leak a held Circle into the weapon computer or missile firing");input(0,PSP_CTRL_SQUARE,.016f,0,0);
 hc_test_setup();Debris*d=&game.debris[0];d->alive=1;d->rock=0;d->qty=1;d->good=0;d->life=100;d->pos=add(game.pos,(Vec3){0,0,700});
 HC_CHECK(!tools_recoverable(DEBRIS_ID_MIN),"base tractor does not claim cargo beyond its actual fitted range");
 game.fit[FIT_HOLD]=TECH_HEAVY_TRACTOR;fit_rebuild(&game);
 HC_CHECK(tech_tractor_range(&game)>700&&tools_recoverable(DEBRIS_ID_MIN),"upgraded tractor availability uses the real module range rather than a fixed500m limit");
 game.flare_cd=2.2f;HC_CHECK(!strcmp(hc_tool_reason(1),"WAIT 3S"),"unavailable tools preserve their actual cooldown reading");
 hc_test_setup();hc_cache_ready=0;game.time=20;
 HC_CHECK(hc_repaint_needed()&&!hc_repaint_needed(),"unchanged cockpit paint is cached without slowing the world simulation");
 game.active_weapon=12;HC_CHECK(hc_repaint_needed(),"equipping a gun invalidates the visual cache immediately before its cosmetic refresh deadline");
 tools_open=1;HC_CHECK(hc_repaint_needed()&&hc_cache_clear,"opening the weapons computer redraws immediately and clears obsolete transparent regions");
 tools_open=0;HC_CHECK(hc_repaint_needed()&&hc_cache_clear,"releasing Circle removes the selector immediately rather than waiting for the visual refresh");
 unsigned*previous=fb;unsigned*pixels=calloc(STRIDE*H,sizeof(unsigned));fb=pixels;hc_layout_errors=0;
 if(pixels){
  for(int ship=0;ship<23;ship++){
   hc_test_setup();game.ship=ship;fit_clear_all(&game);game.fit[0]=69;fit_rebuild(&game);tools_open=tools_armed=1;hc_browser=1;hc_contacts();hud_mode=hud_hidden=0;selected_target=NPC_ID_MIN;
   memset(pixels,0,STRIDE*H*sizeof(unsigned));cockpit();tools_panel();
  }
 }
 HC_CHECK(pixels&&hc_layout_errors==0,"all 23 actual hull capacities render with bounded native text fields including the longest gun name");
 if(pixels){
  unsigned*expected_pixels=malloc(STRIDE*H*sizeof(unsigned));
  if(expected_pixels){
   for(int k=0;k<STRIDE*H;k++)pixels[k]=(k&16)?RGB(17,31,49):RGB(43,61,89);memcpy(expected_pixels,pixels,STRIDE*H*sizeof(unsigned));
   fb=expected_pixels;hc_frame(8,40,152,229,HC_CYAN);hc_words(20,80,128,WHITE,"MINIBOLT LITE");hc_key(70,110,'O',0);
   fb=pixels;int saved_gu=ion_gu_ready;ion_gu_ready=0;hc_cache_clear=1;hc_present_begin();hc_frame(8,40,152,229,HC_CYAN);hc_words(20,80,128,WHITE,"MINIBOLT LITE");hc_key(70,110,'O',0);hc_present_finish();ion_gu_ready=saved_gu;
   HC_CHECK(!memcmp(pixels,expected_pixels,STRIDE*H*sizeof(unsigned))&&!hc_output&&fb==pixels,"cached cockpit composition preserves every pixel and restores the original framebuffer");
   if(ion_gu_ready&&(((unsigned)previous&0x3f000000u)==0x04000000u)){
    for(int k=0;k<STRIDE*H;k++)previous[k]=(k&16)?RGB(17,31,49):RGB(43,61,89);
    fb=previous;unsigned calls=hc_overlay_calls;hc_cache_clear=1;hc_present_begin();hc_frame(8,40,152,229,HC_CYAN);hc_words(20,80,128,WHITE,"MINIBOLT LITE");hc_key(70,110,'O',0);hc_present_finish();
    int mismatch=0;for(int y=0;y<H;y++)for(int x=0;x<W;x++)mismatch+=previous[y*STRIDE+x]!=expected_pixels[y*STRIDE+x];
    fprintf(out,"INFO native cached pixel mismatches %d\n",mismatch);dump_native_bmp("hud-gu-actual.bmp");fb=expected_pixels;dump_native_bmp("hud-gu-reference.bmp");fb=previous;
    HC_CHECK(hc_overlay_calls>calls&&!mismatch,"actual native cached overlay is pixel-identical to the native CPU reference including transparent background");
   }else HC_CHECK(0,"overlay verification requires initialized native display buffer");
   fb=pixels;free(expected_pixels);
  }else HC_CHECK(0,"composition reference allocation");
  hc_test_setup();hc_browser=1;hc_category=2;hc_contacts();selected_target=ROUTE_TARGET_ID;game.route_goal=game.destination=(game.system+1)&255;autoaim=1;game.boost=1;route_boost_charge=3.5f;
  HC_CHECK(route_prejump_active(),"existing route spool remains active with the target computer open");
  memset(pixels,0,STRIDE*H*sizeof(unsigned));fb=pixels;cockpit();dump_native_bmp("hud-warp-charge.bmp");
  game.job_n=1;game.job_sel=0;game.jobs[0].type=EXPEDITION_FIRST;memset(&game.expeditions[0],0,sizeof(game.expeditions[0]));game.expeditions[0].systems[1]=game.system;char progress[64];
  HC_CHECK(expedition_header_info(progress,sizeof(progress))&&strstr(progress,"STAGE 1/4"),"mission header reads the real contract stage and objective counters");
  memset(pixels,0,STRIDE*H*sizeof(unsigned));cockpit();dump_native_bmp("hud-mission-progress.bmp");
 }
 hc_test_setup();game.pos=add(hub_position(&game,0),(Vec3){0,0,-1200});game.yaw=game.pitch=game.roll=0;selected_target=-1;space();HC_CHECK(selected_target==-1,"normal rendering never silently replaces NO TARGET with the nearby primary station");
 hc_test_setup();game.ship=0;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);game.shot=0;game.heat=0;game.npc[0].pos=add(game.pos,(Vec3){0,0,2000});for(int k=1;k<NPC_COUNT;k++)game.npc[k].alive=0;game_tick(&game,.016f,0,0,0,1);
 HC_CHECK(primary_visual_event.owner==&game&&primary_visual_event.serial==game.shots&&primary_visual_event.hit_id==NPC_ID_MIN&&primary_visual_event.shield_hit,"real discharge records confirmed shield contact without altering the damage path");
 hc_test_setup();game.shot=0;int immediate=game.shots;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 HC_CHECK(game.shots>immediate,"X fires immediately with permanently extended hardpoints");
 float cooldown=game.shot;int shots=game.shots;game_tick(&game,.001f,0,0,0,1);HC_CHECK(game.shots==shots&&game.shot<cooldown,"visual event cannot retrigger a shot or bypass real weapon cooldown");
 game.shot=0;game.heat=0;for(int k=0;k<NPC_COUNT;k++)game.npc[k].alive=0;game_tick(&game,.016f,0,0,0,1);HC_CHECK(primary_visual_event.hit_id==-1&&!primary_visual_event.shield_hit,"a genuine miss never creates a confirmed contact effect");
 hc_test_setup();game.ship=0;game.yaw=game.pitch=game.roll=0;memset(twin_attachment_ready,0,sizeof(twin_attachment_ready));int anchors=1;
 for(int ship=0;ship<player_ship_count;ship++){game.ship=ship;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);twin_mounts_prepare();for(int k=0;k<2;k++){Vec3 actual;if(player_hardpoint(k?1:-1,1,&actual))anchors&=length(sub(actual,twin_mounts[k].anchor))<.1f;}}
 HC_CHECK(anchors,"every Twin Stack attachment matches the actual ship hardpoint; slot counts do not create extra banks");
 if(pixels){hc_test_setup();game.ship=0;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;game.shot=0;game.heat=0;game_tick(&game,.016f,0,0,0,1);camera_override_clear();
  unsigned*expected=malloc(STRIDE*H*sizeof(*expected));int allocated=expected!=0,mismatch=0;if(expected){for(int pose=0;pose<2;pose++){if(!pose)primary_visual_event.time=game.time-1;else primary_visual_event.time=game.time;
   for(int k=0;k<STRIDE*H;k++)pixels[k]=expected[k]=(k&16)?RGB(17,31,49):RGB(43,61,89);fb=expected;twin_mounts_prepare();memset(surface_depth,255,sizeof(surface_depth));twin_raster_depth=surface_depth;twin_draw_geometry();
   fb=pixels;mega_depth_valid=0;twin_housings();for(int k=0;k<STRIDE*H;k++)mismatch+=pixels[k]!=expected[k];}
   free(expected);}
  HC_CHECK(allocated&&!mismatch,"depth-aware cached Twin Stack preserves every idle/firing housing pixel including its actual reflected colours");fprintf(out,"INFO Twin Stack cache/reference mismatches %d\n",mismatch);
 }
 if(pixels){int uncovered=0;for(int state=0;state<4;state++){hc_test_setup();hc_browser=state&1;tools_open=(state>>1)&1;hc_cache_clear=1;fb=pixels;memset(pixels,0,STRIDE*H*sizeof(unsigned));hc_present_begin();cockpit();tools_panel();hc_present_finish();for(int y=0;y<H;y++)if(y<34||y>=164)for(int x=0;x<W;x++)uncovered+=(hc_canvas[y*STRIDE+x]>>24)!=255;}
  HC_CHECK(!uncovered,"every pixel skipped by cockpit world clipping is fully opaque in all four computer combinations");fprintf(out,"INFO cockpit hidden-region uncovered pixels %d\n",uncovered);
 }
 if(pixels){hc_test_setup();game.ship=18;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);for(int k=0;k<NPC_COUNT;k++)game.npc[k].alive=0;game.shot=0;game.heat=0;game_tick(&game,.016f,0,0,0,1);primary_visual_event.time=game.time-.10f;camera_override_clear();fb=pixels;for(int k=0;k<STRIDE*H;k++)pixels[k]=RGB(3,5,7);mega_depth_valid=0;weapon_fx_fire();int painted=0;for(int y=0;y<H;y++)for(int x=0;x<W;x++)painted+=pixels[y*STRIDE+x]!=RGB(3,5,7);
  HC_CHECK(!twin_visible_count&&primary_visual_event.hit_id<0&&painted>3,"offscreen Nautilus housings remain hidden while their real missed-shot traces can enter the canopy");
 }
 if(pixels){unsigned*expected=malloc(STRIDE*H*sizeof(*expected));int allocated=expected!=0,mismatch=0;if(expected){for(int r=1;r<=24;r++){memset(pixels,0,STRIDE*H*sizeof(*pixels));memset(expected,0,STRIDE*H*sizeof(*expected));fb=expected;circle(240,110,r,HC_CYAN);fb=pixels;hc_circle_fast(240,110,r,HC_CYAN);for(int k=0;k<STRIDE*H;k++)mismatch+=pixels[k]!=expected[k];}free(expected);}HC_CHECK(allocated&&!mismatch,"cached native ring outlines match every original circle pixel for all 24 cockpit radii");}
 if(pixels){int visible=1;for(int mode=1;mode<=2;mode++){hc_test_setup();hud_mode=mode;hud_hidden=mode==2;float time=game.time;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,.2f,-.1f);fb=pixels;space();visible&=hc_browser&&game.time>time&&(hc_canvas[20*STRIDE+20]>>24)==255&&(hc_canvas[200*STRIDE+20]>>24)==255;}
  HC_CHECK(visible,"target computer temporarily restores the complete readable consoles in minimal and hidden/scenic HUD modes while flight continues");}
 if(pixels){hc_test_setup();for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;selected_target=-1;fb=pixels;hc_ship_instruments();unsigned reference[144*16];for(int y=0;y<16;y++)for(int x=0;x<144;x++)reference[y*144+x]=pixels[(253+y)*STRIDE+172+x];NPC*n=&game.npc[0];n->alive=1;n->mesh=mesh_id("ADDER");n->radius=30;n->pos=add(game.pos,(Vec3){0,-5000,-5000});selected_target=NPC_ID_MIN;hc_ship_instruments();int changes=0;for(int y=0;y<16;y++)for(int x=0;x<144;x++)changes+=reference[y*144+x]!=pixels[(253+y)*STRIDE+172+x];HC_CHECK(!changes,"a selected contact behind/below the ship cannot draw radar focus marks over power controls");}
 hc_test_setup();hc_browser=1;hc_category=1;game.npc[0].pos=add(game.pos,(Vec3){0,0,4000});game.npc[1].pos=add(game.pos,(Vec3){0,0,1000});hc_contacts();game.npc[0].pos=add(game.pos,(Vec3){0,0,500});hc_refresh=0;int original_lock=selected_target;float before_time=game.time;input(PSP_CTRL_CIRCLE,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.22f,.2f,0);
 HC_CHECK(tools_open&&hc_browser&&hc_ids[0]==NPC_ID_MIN&&selected_target==original_lock&&game.time>before_time,"contact distances keep sorting while the weapons computer owns Circle and flight keeps advancing without changing the lock");
 hc_test_setup();game.ship=0;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);game.docked=1;game.credits=10000000;
 for(int sys=0;sys<256;sys++){game.system=sys;if(shipyard_stocks_ship(&game,6))break;}
 HC_CHECK(buy_ship(&game,6)&&game.ship==6&&fit_capacity(game.ship,FIT_WPN)==4,"real shipyard purchase gives the Anaconda its four weapon slots");
 game.docked=0;game.pos=(Vec3){0,0,-20000};game.yaw=game.pitch=game.roll=0;game.system=7;system_bodies(&game);game_spawn(&game);game.docked=0;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 fit_clear_all(&game);int guns[4]={1,2,26,29};for(int i=0;i<4;i++)game.fit[i*6]=guns[i];fit_rebuild(&game);game.active_weapon=0;
 int switching=1,firing=1;input(0,0,.016f,0,0);for(int i=0;i<4;i++){game.shot=0;game.heat=0;int before=game.shots;game_tick(&game,.016f,0,0,0,1);firing&=game.shots==before+1&&primary_visual_event.weapon==guns[i]&&primary_visual_event.serial==game.shots;input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);input(PSP_CTRL_RIGHT,PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,.016f,0,0);switching&=game.active_weapon==((i+1)%4)*6;input(0,0,.016f,0,0);}
 HC_CHECK(switching,"Anaconda Circle+Right visits all four fitted guns immediately and wraps");
 HC_CHECK(firing,"all four Anaconda guns produce a real discharge with their own weapon identity");
 camera_override_clear();flight_camera_seen=-1;twin_mounts_prepare();int substantial=1;for(int k=0;k<2;k++){TwinMount*m=&twin_mounts[k];Point tip=project(camera(&game,twin_point(m,0,1,0))),rear=project(camera(&game,twin_point(m,0,0,0)));substantial&=m->size>25&&rear.y-tip.y>20&&rear.y>145&&tip.y>120;fprintf(out,"INFO Anaconda bank %d size %.2f tip %.1f %.1f rear %.1f %.1f\n",k,m->size,tip.x,tip.y,rear.x,rear.y);}
 HC_CHECK(substantial,"Anaconda keeps full-sized stacked receivers low on the cockpit sill instead of tiny floating guns");
 int corners=1,emission=1;for(int k=0;k<2;k++){TwinMount*m=&twin_mounts[k];Point rear=project(camera(&game,twin_point(m,0,0,0))),tip=project(camera(&game,twin_point(m,0,1,0)));corners&=rear.y>150&&(k?rear.x>400&&tip.x<rear.x-40&&tip.x>rear.x-110:rear.x<80&&tip.x>rear.x+40&&tip.x<rear.x+110);Vec3 muzzle=twin_muzzle(m,0),axis=norm(sub(twin_point(m,0,1,0),twin_point(m,0,.99f,0)));emission&=dot(norm(sub(muzzle,twin_point(m,0,1,0))),axis)>.999f;}
 HC_CHECK(corners,"Anaconda receivers emerge from lower outer corners with short inward-facing sleeves");
 HC_CHECK(emission,"shots start beyond the far tip along the actual inward-facing barrel axis");
 if(pixels){fb=pixels;primary_visual_event.time=game.time-1;space();dump_native_bmp("anaconda-idle.bmp");game.shot=0;game.heat=0;game.active_weapon=0;game_tick(&game,.016f,0,0,0,1);space();dump_native_bmp("anaconda-firing.bmp");}
 fprintf(out,"INFO native per-hull mount projections (offscreen mounts remain offscreen)\n");
 for(int ship=0;ship<player_ship_count;ship++){game.ship=ship;twin_mounts_prepare();fprintf(out,"MOUNT %d %s",ship,player_ships[ship].name);for(int k=0;k<2;k++){Vec3 tip=camera(&game,twin_point(&twin_mounts[k],0,1,0));Point p=project(tip);fprintf(out," / %.1f %.1f z%.1f visible%d",p.x,p.y,tip.z,twin_mounts[k].visible);}fprintf(out,"\n");}
 if(pixels){FILE*mountflag=fopen("cockpit-mounts.flag","r");if(mountflag){fclose(mountflag);for(int ship=0;ship<player_ship_count;ship++){hc_test_setup();game.ship=ship;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);fb=pixels;space();char path[48];snprintf(path,sizeof(path),"twin-mount-%02d.bmp",ship);dump_native_bmp(path);}}}
 fb=previous;free(pixels);fprintf(out,"CHECKS %d\nINFO isolated injected input/native-layout fixture; no retail FPS claim\nRESULT %d failures\n",checks,fails);fclose(out);
 #undef HC_CHECK
 return 1;
}
#endif
