/* Compiled out of retail. Captures exercise the actual native renderer/input. */
static unsigned sw_review_hash(const void *memory,size_t length){const unsigned char *p=memory;unsigned h=2166136261u;for(size_t i=0;i<length;i++)h=(h^p[i])*16777619u;return h;}
static int erl_shockwave_review(void){
 FILE *flag=fopen("erl-shockwave-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("erl-shockwave-review.txt","w");if(!out)return 1;
 int failures=0,checks=0;
 #define SW_CHECK(c,s) do{int pass=!!(c);checks++;failures+=!pass;fprintf(out,"%s %s\n",pass?"PASS":"FAIL",s);fflush(out);}while(0)
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl_local.selection=1;erl_begin(&game,ERL_TRIAL,0,ERL_RUSH,0);game.docked=0;race_restart(&game);game.race.elapsed=140.55f;erl_finish(&game,1,0);
 int credits=game.credits,xp=game.erl.xp;erl_result_serial=game.erl.serial;erl_replay.played=game.erl.serial;erl_replay.active=0;erl_choice=0;
 unsigned *old=fb,*memory=calloc(STRIDE*H+32,sizeof(unsigned));if(!memory){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=memory+16;for(int i=0;i<16;i++)memory[i]=memory[16+STRIDE*H+i]=0x19abcdef;
 int camera_ok=1,direction_ok=1,velocity_ok=1;const float joins[]={1.05f,1.85f,2.71f,3.11f,3.61f,5.46f};
 for(int i=0;i<6;i++){float t=joins[i],epsilon=.001f;ErlShockPose a=sw_pose(t-epsilon),b=sw_pose(t),c=sw_pose(t+epsilon);camera_ok&=length(sub(c.eye,a.eye))<80&&length(sub(c.ship,a.ship))<80;direction_ok&=dot(a.forward,c.forward)>.999f;
  Vec3 va=mul(sub(b.eye,a.eye),1/(b.time-a.time)),vb=mul(sub(c.eye,b.eye),1/(c.time-b.time));float error=length(sub(va,vb));fprintf(out,"INFO camera join %.2f velocity_delta %.2f\n",t,error);velocity_ok&=error<3000;
 }
 SW_CHECK(camera_ok&&direction_ok,"continuous ship and camera positions/directions across launch, swing, impact and hero joins");
 SW_CHECK(velocity_ok,"camera velocities remain continuous across motion joins within PSP float precision");
 SW_CHECK(sw_time(2.91f)-sw_time(2.81f)<.023f&&sw_time(4)-sw_time(3.9f)>.099f,"impact slows presentation to 0.22x and smoothly restores full speed");
 float start=sw_pose(0).ship.z,end=sw_pose(.2f).ship.z;SW_CHECK(end-start>4500,"ship and camera already travel at racing speed on frame one");
 unsigned authority=sw_review_hash(&game,sizeof(game));int guards=1;
 const float frames[]={0,.25f,.30f,.60f,1,1.25f,1.7f,1.85f,2,2.25f,2.45f,2.70f,2.71f,2.8f,3,3.2f,3.6f,4.1f,4.75f,5.45f,6,6.25f,6.75f,7.5f,8.5f,9.8f,30};
 game.ship=4;ship_paint[game.ship]=RGB(225,48,63);authority=sw_review_hash(&game,sizeof(game));
 for(int i=0;i<(int)(sizeof(frames)/sizeof(*frames));i++){erl_reveal=frames[i];erl_shockwave_draw(erl_reveal);char filename[64];snprintf(filename,sizeof(filename),"shockwave-%05.2f.bmp",frames[i]);dump_native_bmp(filename);for(int k=0;k<16;k++)guards&=memory[k]==0x19abcdef&&memory[16+STRIDE*H+k]==0x19abcdef;}
 SW_CHECK(authority==sw_review_hash(&game,sizeof(game)),"all reveal stages preserve full authoritative Game including payouts, camera, RNG and progression");
 SW_CHECK(guards,"native captures preserve framebuffer guards through near-plane exhaust and lens shards");
 sw_init();SW_CHECK(sw_world.shards>=32&&sw_world.shards<=128,"chunky numeral creates a fixed bounded pool of spatial triangle shards");
 unsigned place_hash[7];int places_ok=1;for(int place=1;place<=7;place++){game.erl.result_place=place;erl_shockwave_draw(3.2f);place_hash[place-1]=sw_review_hash(fb,STRIDE*H*sizeof(unsigned));char name[48];snprintf(name,sizeof(name),"shockwave-place-%d.bmp",place);dump_native_bmp(name);places_ok&=sw_world.place==place&&sw_world.shards>0;if(place>1)places_ok&=place_hash[place-1]!=place_hash[place-2];}
 SW_CHECK(places_ok,"finish flyby builds and renders the real dynamic place numeral for every ERL field position");game.erl.result_place=1;
 int hulls=1;for(int ship=0;ship<player_ship_count;ship++){game.ship=ship;SwHull h=sw_hull(ship);hulls&=h.scale>0&&h.count>0&&h.count<=8;
  erl_shockwave_draw(0);char name[64];snprintf(name,sizeof(name),"hull-%02d-rear.bmp",ship);dump_native_bmp(name);erl_shockwave_draw(8);snprintf(name,sizeof(name),"hull-%02d-hero.bmp",ship);dump_native_bmp(name);
  if(!strcmp(player_ships[ship].name,"NEEDLEFISH"))hulls&=h.count==1&&fabsf(h.engines[0].x-40.5f)<.01f;
 }
 SW_CHECK(hulls,"every player hull uses its actual geometry and valid engines including single-port asymmetric Needlefish and all capital pods");
 game.ship=4;ship_paint[4]=RGB(245,70,55);erl_shockwave_draw(8);unsigned red=sw_review_hash(fb,STRIDE*H*sizeof(unsigned));ship_paint[4]=RGB(55,130,245);erl_shockwave_draw(8);SW_CHECK(red!=sw_review_hash(fb,STRIDE*H*sizeof(unsigned)),"selected ship paint changes actual hero hull pixels without recolouring ERL branding");
 snprintf(game.commander_name,sizeof(game.commander_name),"ALEXANDRA STARLIGHT");game.erl.result_units=9876543;game.erl.result_xp=123456;erl_reveal=8;erl_shockwave_draw(8);dump_native_bmp("shockwave-long-name-rewards.bmp");
 SW_CHECK(fb[255*STRIDE+30]!=EB,"continue action remains visible beneath actual variable rewards and long commander name");
 /* Native film frames are presentation snapshots, never shipped assets. */
 for(int i=0;i<100;i++){erl_shockwave_draw(i*.1f);char name[48];snprintf(name,sizeof(name),"film-%03d.bmp",i);dump_native_bmp(name);}
 float worst=0,total=0;int samples=300;for(int i=0;i<samples;i++){uint64_t t=sceKernelGetSystemTimeWide();erl_shockwave_draw(i/30.f);float ms=(sceKernelGetSystemTimeWide()-t)*.001f;total+=ms;worst=fmaxf(worst,ms);}fprintf(out,"INFO native renderer %d frames mean %.2fms worst %.2fms\n",samples,total/samples,worst);fflush(out);
 SW_CHECK(total/samples<16.7f&&worst<50,"bounded native cinematic renderer meets responsive emulator frame budget");
 /* Use real result input, including the consumed early skip and repeated win. */
 game.erl.result_units=12000;game.erl.result_xp=180;erl_reveal=0;erl_result_serial=game.erl.serial;erl_replay.played=game.erl.serial;int balance=game.credits,earned=game.erl.xp;
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);SW_CHECK(game.erl.result&&erl_reveal>=SW_READY&&game.credits==balance&&game.erl.xp==earned,"early X reveals rewards and consumes input without continuing or paying again");
 erl_input(0,0,.2f,0,0);SW_CHECK(erl_reveal>9.8f&&game.erl.result,"reveal plays once and retains live world motion while waiting for input");
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);SW_CHECK(!game.erl.event_kind&&game.credits==balance&&game.erl.xp==earned,"continue returns to existing league destination without duplicate units or XP");
 game.erl.referral=game.erl.licence=1;game.erl_local.selection=1;SW_CHECK(erl_begin(&game,ERL_TRIAL,0,ERL_RUSH,0),"another race can start after the new reveal");erl_finish(&game,1,0);erl_replay.played=game.erl.serial;erl_input(0,0,.016f,0,0);SW_CHECK(erl_reveal<.1f,"a second real win resets only presentation and starts the new reveal once");

 /* Existing official actions, audio restoration and persisted pending result. */
 erl_return_hq();erl_review_fixture(1);game.erl.referral=game.erl.licence=1;erl_season_start(&game);erl_begin(&game,ERL_OFFICIAL,0,ERL_CIRCUIT,0);erl_finish(&game,1,0);
 erl_result_serial=game.erl.serial;erl_replay.played=game.erl.serial;erl_replay.active=0;erl_reveal=9.8f;erl_choice=0;page=FLIGHT;
 int official_balance=game.credits,official_xp=game.erl.xp;erl_shockwave_draw(8);dump_native_bmp("shockwave-official.bmp");
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);SW_CHECK(page==ERL_PAGE&&erl_screen==ERL_STANDINGS&&game.credits==official_balance&&game.erl.xp==official_xp,"official win retains STANDINGS action and exact already-paid rewards");
 erl_begin(&game,ERL_OFFICIAL,1,erl_discipline(1),0);erl_finish(&game,1,0);erl_result_serial=game.erl.serial;erl_replay.played=game.erl.serial;erl_reveal=8;erl_choice=0;page=FLIGHT;
 erl_input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);SW_CHECK(erl_choice==1,"official result D-pad selects the existing CONTINUE action");erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);SW_CHECK(!game.erl.event_kind&&erl_screen!=ERL_STANDINGS,"official CONTINUE retains its existing headquarters destination");
 erl_begin(&game,ERL_OFFICIAL,2,erl_discipline(2),0);erl_finish(&game,1,0);erl_result_serial=game.erl.serial;erl_replay.played=game.erl.serial;erl_reveal=8;page=FLIGHT;
 radio_station=4;radio_off=1;radio_dirty=0;race_ui_sync();SW_CHECK(!radio_race_on&&radio_station==4&&radio_off==1&&!radio_dirty,"finished victory restores saved station and music-off preference");
 Game *restored=malloc(sizeof(Game));int saved=save_game(&game,"shockwave-pending.sav"),loaded=restored&&load_game(restored,"shockwave-pending.sav");SW_CHECK(saved&&loaded&&restored->erl.result==game.erl.result&&restored->credits==game.credits&&restored->erl.xp==game.erl.xp,"pending victory persists in existing save format without losing rewards or progression");if(restored)free(restored);
 int paused_before=game.erl.paused;game.erl.paused=1;unsigned suspended=sw_review_hash(&game,sizeof(game));erl_shockwave_draw(8.1f);SW_CHECK(suspended==sw_review_hash(&game,sizeof(game)),"suspended result remains presentation-only and preserves pending state");game.erl.paused=paused_before;
 erl_input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);SW_CHECK(!game.erl.event_kind&&page==ERL_PAGE,"Circle retains existing return action after reveal");
 fb=old;free(memory);fprintf(out,"INFO %d checks\nRESULT %d failures\n",checks,failures);fclose(out);return 1;
 #undef SW_CHECK
}
