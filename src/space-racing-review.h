static void race_review_fixture(void){
 game_init(&game);story_complete(&game);game.opening.stage=0;mission_card_reset();deck_reset();paused=0;
 game.docked=1;game.credits=100000;accept_mission(&game,5);game.docked=0;game.dock_stage=0;game.planet=game.approach=-1;
 game.voice_time=game.message_time=0;game.encounter=999;game.freight_next=999;game.encounter_kind=0;tracked_mission=2;game.jump=0;page=FLIGHT;third_person=0;hud_mode=hud_hidden=0;selected_target=RACE_TARGET_ID;autoaim=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
}
static void race_review_frame(const char *name){rect(0,0,W,H,BG);drawcount=0;space();race_loss_draw();dump_native_bmp(name);}
static void race_review_drive(int held){
 int burst=0;
 for(int frame=0;frame<5600&&game.race.phase==RACE_RUNNING&&!game.dead;frame++){
  RaceGate *q=&game.race.gates[game.race.gate];Vec3 aim=add(q->pos,mul(q->normal,180));
  if(!q->boost_used&&dot(sub(game.pos,q->booster),q->normal)<0)aim=add(q->booster,mul(q->normal,100));
  Vec3 desired=norm(sub(aim,game.pos)),right,up,ahead;camera_axes(&game,&right,&up,&ahead);
  float turn=fmaxf(-1,fminf(1,dot(desired,right)*7)),pitch=fmaxf(-1,fminf(1,dot(desired,up)*7));
  if(game.race.reserve>65)burst=1;if(game.race.reserve<12)burst=0;
  game.boost=held||burst;
  game_tick(&game,.05f,turn,pitch,!held&&game.race.boost_lock?0:1,0);
 }
}
static int space_racing_review(void){
 FILE *flag=fopen("space-racing-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("space-racing-review.txt","w");if(!out)return 1;
 unsigned *oldfb=fb,*mem=calloc(STRIDE*H+32,sizeof(unsigned));Game *snapshot=malloc(sizeof(Game));
 if(!mem||!snapshot){fprintf(out,"RESULT 1 failures\n");fclose(out);free(mem);free(snapshot);return 1;}
 fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0xa278d31eu;int failures=0;
 #define RCHK(c,s) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;fflush(out);}while(0)
 race_review_fixture();int valid=1,distinct=0;unsigned lastseed=0;
 for(int sys=0;sys<256;sys++){
  game.system=sys;system_bodies(&game);
  for(int hub=0;hub<HUB_COUNT;hub++){SpaceRace a,b;valid&=race_course_build(&game,hub,&a)&&race_course_build(&game,hub,&b)&&race_course_clear(&game,&a)&&a.rivals>=5&&a.rivals<=10&&!memcmp(&a,&b,sizeof(a));distinct+=a.seed!=lastseed;lastseed=a.seed;}
 }
 RCHK(valid,"all 768 system/hub courses are deterministic, clear of planets/rings/stations and have 5-10 rivals");
 RCHK(distinct==768,"every local start grid has its own seeded course and rival roster");
 race_review_fixture();int bends=0,climbs=0;
 for(int k=1;k<RACE_GATES;k++){bends+=dot(game.race.gates[k-1].normal,game.race.gates[k].normal)<.93f;climbs+=fabsf(game.race.gates[k].normal.y-game.race.gates[k-1].normal.y)>.12f;}
 RCHK(game.race.length/game.race.top_speed>96&&game.race.length/game.race.top_speed<109&&RACE_GATES==32,"course distance matches the fitted performance class with thirty-two ordered gates");
 RCHK(bends>=18&&climbs>=12,"physical course requires substantial repeated turning, climbs and descents");
 race_review_fixture();page=MISSIONS;game.docked=1;row=5;mission_queries_begin(&game);
 RCHK(mission_count(&game)==6&&mission_destination(&game,5)==game.system&&mission_type_for_offer(&game,5)==MISSION_RACE,"race is the sixth local board offer alongside five existing contracts");
 mission_queries_end();rect(0,0,W,H,BG);mission_board();dump_native_bmp("race-board.bmp");
 int cash=game.credits;RCHK(game.job_n==1&&game.jobs[0].type==MISSION_RACE&&game.jobs[0].item==1,"race acceptance persists its station grid without reserving cargo or passenger space");
 RCHK(!accept_mission(&game,5)&&game.credits==cash,"duplicate race entry cannot deduct another deposit");
 race_review_fixture();RCHK(mission_target_id(&game,0)==RACE_TARGET_ID&&valid_target(RACE_TARGET_ID)&&target_detected(RACE_TARGET_ID),"race waypoint remains navigable outside sensor range");
 game.pos=add(game.race.start,mul(game.race.dir,-7500));game.yaw=atan2f(game.race.dir.x,game.race.dir.z);game.pitch=asinf(game.race.dir.y);
 race_review_frame("race-start-grid.bmp");game_tick(&game,.05f,0,0,0,0);RCHK(game.race.phase==RACE_GRID,"distant approach cannot start a race or teleport the player");
 game.pos=game.race.start;game_tick(&game,.05f,0,0,0,0);race_ui_sync();
 RCHK(game.race.phase==RACE_COUNTDOWN&&game.speed==0&&dot(forward(&game),game.race.dir)>.999f,"entering the marked grid aligns the player and starts a four-second countdown");
 game.race.countdown=2.8f;race_review_frame("race-countdown.bmp");Vec3 before=game.pos;game_tick(&game,.05f,1,1,1,1);
 RCHK(length(sub(before,game.pos))<.001f&&game.speed==0,"countdown locks movement, steering and firing while holding the grid");
 memcpy(snapshot,&game,sizeof(game));page=COMMS_PANEL;input(0,0,.05f,1,1);
 RCHK(!memcmp(snapshot,&game,sizeof(game)),"opening the paused Comms Panel freezes countdown and all race actors");page=FLIGHT;
 radio_station=4;radio_off=1;radio_dirty=0;race_ui_sync();
 RCHK(radio_race_on&&radio_display_station()==8&&radio_station==4&&radio_off==1&&!radio_dirty,"Rock FM automatically scores the race without changing saved station or music settings");
 for(int n=0;n<100&&game.race.phase==RACE_COUNTDOWN;n++)game_tick(&game,.05f,0,0,1,0);
 RCHK(game.race.phase==RACE_RUNNING&&game.race.elapsed==0,"GO releases normal flight exactly once without running the race clock during countdown");race_review_frame("race-go.bmp");
 input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.24f,0,0);
 RCHK(page==FLIGHT&&!square_held&&!autoaim&&selected_target==RACE_TARGET_ID&&game.speed>0,"holding Square cannot open targeting or steal R throttle during racing");
 input(0,0,.016f,0,0);RCHK(!square_held&&selected_target==RACE_TARGET_ID,"releasing Square cannot quick-target unrelated contacts during racing");
 change_page(TARGETING);RCHK(page==FLIGHT,"menu route to targeting is also unavailable while racing");
 RaceGate q=game.race.gates[0];Vec3 from=add(q.pos,mul(q.normal,-5000)),to=add(q.pos,mul(q.normal,5000));
 RCHK(fabsf(race_crossing(from,to,q.pos,q.normal,q.radius)-.5f)<.001f,"swept checkpoint crossing catches full-speed passes between frames");
 RCHK(race_crossing(to,from,q.pos,q.normal,q.radius)<0,"flying backwards through a checkpoint does not count");
 Vec3 offset=mul(q.right,q.radius+1);RCHK(race_crossing(add(from,offset),add(to,offset),q.pos,q.normal,q.radius)<0,"flying outside the actual ring aperture does not count");
 game.pos=game.race.gates[1].pos;race_tick(&game,add(game.pos,mul(q.normal,-500)),.05f);
 RCHK(game.race.gate==0,"skipping ahead to another checkpoint cannot advance the course");
 game.pos=add(q.booster,mul(q.normal,500));game.speed=9000;game.heat=80;game.race.reserve=20;race_tick(&game,add(q.booster,mul(q.normal,-500)),.05f);
 RCHK(game.race.gates[0].boost_used&&game.race.boost_time>0&&game.speed==9000&&game.heat==72&&fabsf(game.race.reserve-44)<.01f,"green ring restores reserve once and briefly powers the drive without instantly changing velocity");
 {float step=.016f,turn=0,pitch=0,reserve=game.race.reserve;int throttle=0;race_flight_control(&game,&step,&turn,&pitch,&throttle);RCHK(game.boost&&throttle==1&&game.race.reserve==reserve,"green boost ring powers a free short burst without spending the manual reserve");}
 game.heat=80;game.race.boost_time=.4f;race_tick(&game,add(q.booster,mul(q.normal,-500)),.05f);
 RCHK(game.heat==80&&game.race.boost_time<.4f,"each boost ring activates only once per attempt");
 game.pos=add(q.pos,mul(q.normal,-3800));game.yaw=atan2f(q.normal.x,q.normal.z);game.pitch=asinf(q.normal.y);game.race.elapsed=9.6f;game.race.boost_time=1;
 for(int i=0;i<game.race.rivals;i++){game.race.pilot[i].pos=add(game.pos,add(mul(q.normal,950+i*100),mul(q.right,(i%5-2)*230)));game.race.pilot[i].speed=3500;game.race.pilot[i].dir=q.normal;}
 race_review_frame("race-pack.bmp");
 game.pos=add(q.booster,mul(q.normal,-1800));race_review_frame("race-boost-ring.bmp");
 game.yaw+=3.14159265f;race_review_frame("race-turn-back.bmp");game.yaw-=3.14159265f;
 int basecash=game.credits,reward=game.jobs[0].reward;game.race.gate=0;
 for(int k=0;k<RACE_GATES;k++){RaceGate *gate=&game.race.gates[k];game.pos=add(gate->pos,mul(gate->normal,300));race_tick(&game,add(gate->pos,mul(gate->normal,-300)),.05f);}
 RCHK(game.race.phase==RACE_WON&&game.race.place==1&&!game.job_n&&game.credits==basecash+reward,"finishing every checkpoint first completes the real mission and pays the advertised prize");
 RCHK(game.mission_receipt_n==1&&game.mission_receipts[0].type==MISSION_RACE&&game.mission_receipts[0].amount==reward&&!strcmp(game.mission_receipts[0].sender,"RACE CONTROL"),"race win sends the actual Race Control prize transmission");
 race_tick(&game,game.pos,.05f);RCHK(game.credits==basecash+reward&&game.mission_receipt_n==1,"finishing or ticking again cannot award a second prize");
 race_ui_sync();RCHK(!radio_race_on&&radio_display_station()==4&&radio_off==1&&!radio_dirty,"race finish restores the original station and original music-off preference automatically");
 mission_card_sync();mission_card_age=1.4f;mission_card_draw();dump_native_bmp("race-prize-transmission.bmp");
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;
 Vec3 startpos=game.race.pilot[0].pos;for(int frame=0;frame<5800;frame++)race_tick(&game,game.pos,.05f);
 int finished=0;for(int i=0;i<game.race.rivals;i++)finished+=game.race.pilot[i].gate==RACE_GATES&&game.race.pilot[i].finished>0;
 RCHK(length(sub(startpos,game.race.pilot[0].pos))>150000,"rival ships move through the world rather than advancing a synthetic leaderboard");
 RCHK(finished==game.race.rivals,"all rival pilots steer through all thirty-two real checkpoint apertures and finish");
 basecash=game.credits;game.race.gate=RACE_GATES-1;q=game.race.gates[RACE_GATES-1];game.pos=add(q.pos,mul(q.normal,300));race_tick(&game,add(q.pos,mul(q.normal,-300)),.05f);
 RCHK(game.race.phase==RACE_LOST&&game.credits==basecash&&game.job_n==1&&!game.mission_receipt_n,"finishing behind rivals awards no money and keeps the entry available for retry");
 race_review_frame("race-result-retry.bmp");memcpy(snapshot,&game,sizeof(game));input(0,0,.6f,1,1);
 RCHK(!memcmp(snapshot,&game,sizeof(game)),"retry result pauses ship and rivals while awaiting a deliberate decision");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 RCHK(game.race.phase==RACE_COUNTDOWN&&game.race.gate==0&&game.race.elapsed==0&&game.credits==basecash,"fresh X retries from a clean grid without another entry fee");
 int reset=1;for(int i=0;i<RACE_GATES;i++)reset&=!game.race.gates[i].boost_used;for(int i=0;i<game.race.rivals;i++)reset&=game.race.pilot[i].gate==0;
 RCHK(reset,"retry resets all checkpoint progress, boost gates and rival finishes");
 game.race.phase=RACE_RUNNING;game.race.elapsed=RACE_LIMIT-.01f;race_tick(&game,game.pos,.05f);
 RCHK(game.race.phase==RACE_LOST&&game.credits==basecash,"five-minute timeout offers retry without paying an unfinished race");
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;game.race.gate=4;game.race.elapsed=24;
 RCHK(!save_game(&game,"race-flight-test.sav"),"ordinary flight cannot create a save or exploit an in-progress race");
 game.docked=1;int saved=save_game(&game,"race-test.sav");memset(snapshot,0,sizeof(*snapshot));int loaded=saved&&load_game(snapshot,"race-test.sav");
 RCHK(loaded&&snapshot->job_n==1&&snapshot->jobs[0].type==MISSION_RACE&&snapshot->credits==game.credits&&snapshot->race.phase==RACE_NONE,"save V32 preserves the race contract and money without persisting an unfair in-progress attempt");
 if(loaded){snapshot->docked=0;snapshot->dock_stage=0;race_prepare(snapshot);RCHK(snapshot->race.phase==RACE_GRID&&snapshot->race.gate==0&&snapshot->race.elapsed==0,"loading an accepted race reconstructs its original grid and resets the attempt");}
 remove("race-test.sav");remove("race-test.sav.bak");
 game.docked=1;race_preflight(&game,.05f);RCHK(game.race.phase==RACE_GRID&&game.job_n==1,"docking aborts the attempt without completing or losing the contract");
 RCHK(abandon_mission(&game,0),"race can be abandoned through the existing mission log");race_prepare(&game);RCHK(!race_ready(&game)&&!valid_target(RACE_TARGET_ID),"abandonment removes the start waypoint and race actors");
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;game.race.boost_time=0;game.boost=0;game.speed=game.race.top_speed;game.fit[FIT_WPN]=1;fit_rebuild(&game);float speed=game.speed;game_tick(&game,.05f,0,0,0,1);
 RCHK(game.shots==0&&game.speed<speed,"race rules suppress weapons while normal flight coasts after boost");

 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;game.race.countdown=0;r_tap=10;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);int single=!game.boost;
 input(0,0,.1f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 RCHK(single&&game.boost,"normal single R throttle and double-tap R boost controls work during a race");
 input(0,0,.016f,0,0);RCHK(!game.boost,"releasing R cancels manual boost during racing");
 /* An ordinary hull flies the entire course using production steering,
  * acceleration, heat, fuel, gates and rivals. No teleporting or stats cheat. */
 int managed_boost=0;
 for(int frame=0;frame<4600&&game.race.phase==RACE_RUNNING&&!game.dead;frame++){
  RaceGate *gate=&game.race.gates[game.race.gate];Vec3 aim=add(gate->pos,mul(gate->normal,180));
  if(!gate->boost_used&&dot(sub(game.pos,gate->booster),gate->normal)<0)aim=add(gate->booster,mul(gate->normal,100));
  Vec3 desired=norm(sub(aim,game.pos)),right,up,ahead;camera_axes(&game,&right,&up,&ahead);
  float turn=fmaxf(-1,fminf(1,dot(desired,right)*7)),pitch=fmaxf(-1,fminf(1,dot(desired,up)*7));
  if(game.race.reserve>65)managed_boost=1;if(game.race.reserve<12)managed_boost=0;
  game.boost=managed_boost;game_tick(&game,.05f,turn,pitch,game.race.boost_lock?0:1,0);
  if(frame==280)race_review_frame("race-live-flight.bmp");
 }
 fprintf(out,"INFO production flight: phase %d gate %d place %d time %.2f heat %.1f fuel %.1f hull %.1f\n",game.race.phase,game.race.gate,game.race.place,game.race.elapsed,game.heat,game.fuel,game.hull);
 RCHK(game.race.phase==RACE_WON&&game.race.gate==RACE_GATES&&!game.dead,"a standard unmodified ship can win through actual flight controls and boost rings");
 RCHK(game.race.elapsed>105&&game.race.elapsed<170&&game.fuel>0,"starter hull completes a roughly two-minute winding race without exhausting fuel");
 RCHK(game.heat<90&&game.energy>50,"using the gold gates keeps the standard hull's engines and shields healthy");

 /* Reserve behaviour through real shoulder input, not just a bar change. */
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;r_tap=10;
 for(int n=0;n<RACE_GATES;n++)game.race.gates[n].boost_used=1;
 for(int n=0;n<game.race.rivals;n++)game.race.pilot[n].pos=add(game.pos,mul(game.race.right,20000+n*500));
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);input(0,0,.08f,0,0);input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 for(int n=0;n<90;n++)input(0,PSP_CTRL_RTRIGGER,.05f,0,0);
 RCHK(game.race.reserve==0&&game.race.boost_lock&&!game.boost,"held double-tap R exhausts the race drive after four seconds and cuts boost");
 for(int n=0;n<30;n++)input(0,PSP_CTRL_RTRIGGER,.05f,0,0);
 RCHK(game.race.reserve==0&&!game.boost,"holding R after exhaustion cannot recharge or automatically restart boost");race_review_frame("race-reserve-empty.bmp");
 input(0,0,.05f,0,0);for(int n=0;n<35;n++)input(0,PSP_CTRL_RTRIGGER,.05f,0,0);
 RCHK(game.race.reserve>10&&!game.race.boost_lock&&!game.boost,"release then single R cruises while the reserve recharges without triggering boost");race_review_frame("race-reserve-recharge.bmp");
 float reserve=game.race.reserve;memcpy(snapshot,&game,sizeof(game));page=COMMS_PANEL;input(0,0,.1f,0,0);
 RCHK(game.race.reserve==reserve&&!memcmp(snapshot,&game,sizeof(game)),"paused menus cannot refill race boost or move rivals");page=FLIGHT;
 race_restart(&game);RCHK(game.race.reserve==100&&!game.race.boost_lock,"retry resets the race reserve and exhaustion latch");
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;race_review_drive(1);
 fprintf(out,"INFO held boost: phase %d gate %d place %d time %.2f\n",game.race.phase,game.race.gate,game.race.place,game.race.elapsed);
 RCHK(game.race.phase==RACE_LOST&&game.race.gate==RACE_GATES&&game.race.elapsed>105,"perfect gate steering with endlessly held boost no longer guarantees first place or a sub-minute race");
 int slowship=0,fastship=0;
 for(int i=0;i<player_ship_count;i++){if(player_ships[i].speed*player_ships[i].boost<player_ships[slowship].speed*player_ships[slowship].boost)slowship=i;if(player_ships[i].speed*player_ships[i].boost>player_ships[fastship].speed*player_ships[fastship].boost)fastship=i;}
 for(int type=0;type<2;type++){
  race_review_fixture();game.ship=type?fastship:slowship;fit_rebuild(&game);game.race.phase=RACE_NONE;race_prepare(&game);race_restart(&game);game.race.phase=RACE_RUNNING;
  race_review_drive(0);fprintf(out,"INFO class %s: phase %d gate %d place %d time %.2f top %.0f distance %.0f\n",player_ships[game.ship].name,game.race.phase,game.race.gate,game.race.place,game.race.elapsed,game.race.top_speed,game.race.length);
  RCHK(game.race.phase==RACE_WON&&game.race.gate==RACE_GATES&&game.race.elapsed>100&&game.race.elapsed<180&&!game.dead,"slowest and fastest hulls can win a real roughly two-minute course using managed boost");
 }
 int hull_timing=1;float fastest_time=999,slowest_time=0;int hull_wins=0;
 for(int ship=0;ship<player_ship_count;ship++){
  race_review_fixture();game.ship=ship;fit_rebuild(&game);game.race.phase=RACE_NONE;race_prepare(&game);race_restart(&game);game.race.phase=RACE_RUNNING;race_review_drive(0);
  hull_timing&=game.race.gate==RACE_GATES&&!game.dead&&game.race.elapsed>100&&game.race.elapsed<180;hull_wins+=game.race.phase==RACE_WON;
  fastest_time=fminf(fastest_time,game.race.elapsed);slowest_time=fmaxf(slowest_time,game.race.elapsed);
  fprintf(out,"INFO hull %s: phase %d place %d time %.2f\n",player_ships[ship].name,game.race.phase,game.race.place,game.race.elapsed);
 }
 RCHK(hull_timing,"all twenty-two own-ship hulls physically finish within the roughly two-minute course envelope");
 fprintf(out,"INFO hull timing range %.2f - %.2f seconds; managed wins %d/%d\n",fastest_time,slowest_time,hull_wins,player_ship_count);
 race_review_fixture();game.ship=fastship;game.fit[FIT_FUEL]=TECH_BOOST_INJECTORS;game.fit[FIT_FUEL+6]=TECH_CRUISE_THRUSTERS;game.fit[FIT_FUEL+12]=TECH_EFFICIENT_BOOSTER;game.fit[FIT_FUEL+18]=TECH_COMBAT_THRUSTERS;game.fit[FIT_UTIL]=TECH_ADVANCED_HEAT_BUFFER;game.pip_eng=3;fit_rebuild(&game);game.race.phase=RACE_NONE;race_prepare(&game);race_restart(&game);game.race.phase=RACE_RUNNING;
 race_review_drive(1);fprintf(out,"INFO upgraded held: phase %d place %d time %.2f top %.0f\n",game.race.phase,game.race.place,game.race.elapsed,game.race.top_speed);
 RCHK(game.race.phase==RACE_LOST&&game.race.gate==RACE_GATES&&game.race.elapsed>105,"speed upgrades, maximum engine pips and Advanced Heat Buffer cannot bypass the reserve or guarantee a held-boost win");
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;float old_top=game.race.top_speed;game.pip_eng=3;race_restart(&game);
 RCHK(game.race.top_speed>old_top&&game.race.reserve==100,"retry rebuilds its performance class after an engine-setting change");
 race_review_fixture();game.pos=add(game.race.start,mul(game.race.up,10000));abandon_mission(&game,0);race_prepare(&game);game.race.reserve=0;game.race.boost_lock=1;game.boost=1;game.speed=0;float normal_fuel=game.fuel;
 for(int n=0;n<100;n++)game_tick(&game,.05f,0,0,1,0);
 fprintf(out,"INFO normal flight: boost %d fuel %.2f/%.2f reserve %.1f phase %d dock %d planet %d dead %d\n",game.boost,game.fuel,normal_fuel,game.race.reserve,game.race.phase,game.dock_stage,game.planet,game.dead);
 RCHK(game.boost&&game.fuel<normal_fuel&&game.race.reserve==0,"normal spaceflight still permits held boost and spends normal fuel independently of the race reserve");

 /* Swept real contact, shared slow motion, safe camera return and reset. */
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;q=game.race.gates[0];
 game.pos=add(q.booster,mul(q.normal,-4000));game.yaw=atan2f(q.normal.x,q.normal.z);game.pitch=asinf(q.normal.y);game.speed=7200;game.boost=1;
 for(int i=0;i<game.race.rivals;i++)game.race.pilot[i].pos=add(game.race.start,add(mul(q.normal,-8000),mul(q.right,8000+i*600)));
 game.race.pilot[0].pos=game.pos;game.race.pilot[0].speed=1000;game.race.pilot[0].dir=q.normal;
 int legal=game.legal,ship=game.ship;float hull=game.hull,energy=game.energy;
 race_tick(&game,add(game.pos,mul(q.normal,-1200)),.05f);
 RCHK(game.race.pilot[0].disabled&&game.race.takedowns==1&&game.race.crash_time==5.8f,"high-speed swept ram destroys the actual rival and starts one takedown camera");
 RCHK(game.hull==hull&&game.energy==energy&&game.legal==legal&&game.ship==ship,"arcade contact preserves player hull, shields, warrant and chosen ship");
 RCHK(game.race.pilot[0].finished<0&&game.race.place==1,"destroyed racers cannot keep racing or hold a phantom leaderboard place");
 race_review_frame("race-takedown-impact.bmp");
 float slowdt=.05f,turn=0,pitch=0;int throttle=0;race_flight_control(&game,&slowdt,&turn,&pitch,&throttle);
 RCHK(slowdt<.02f&&throttle==1,"takedown slows the common simulation and guides the player through real gates");
 memcpy(snapshot,&game,sizeof(game));page=COMMS_PANEL;input(0,0,.05f,1,1);
 RCHK(!memcmp(snapshot,&game,sizeof(game)),"paused menus freeze a takedown camera and all physical racers");page=FLIGHT;
 for(int mode=0;mode<3;mode++){
  third_person=mode;flight_camera_seen=-1;game.race.crash_time=3.001f;flight_camera_prepare();Vec3 returning=camera_origin(&game);
  game.race.crash_time=3.f;flight_camera_prepare();Vec3 home=camera_origin(&game);
  RCHK(isfinite(length(returning))&&length(sub(returning,home))<8,"crash swing joins the actual selected first/chase/cinematic camera without a jump");
  camera_override_clear();
 }
 memcpy(&game,snapshot,sizeof(game));third_person=0;flight_camera_seen=-1;
 for(int n=0;n<17;n++)game_tick(&game,.05f,1,1,0,0);
 race_review_frame("race-takedown-fragments.bmp");
 RCHK(game.race.pilot[0].burst>.1f&&game.race.pilot[0].burst<.5f,"rival debris expands in actual shared slow motion while autopilot flies");
 for(int n=0;n<12;n++)game_tick(&game,.05f,-1,-1,0,0);
 race_review_frame("race-camera-return.bmp");
 for(int n=0;n<31;n++)game_tick(&game,.05f,-1,-1,0,0);
 RCHK(game.race.crash_time>0&&game.race.crash_time<3.f&&!race_crash_camera(),"normal camera returns while protected safety guidance remains");
 slowdt=.05f;throttle=0;race_flight_control(&game,&slowdt,&turn,&pitch,&throttle);
 RCHK(slowdt==.05f&&throttle==1,"recovery guidance continues at normal speed after the camera swing");
 for(int n=0;n<70;n++)game_tick(&game,.05f,0,0,0,0);
 turn=.35f;pitch=-.25f;throttle=-1;slowdt=.05f;race_flight_control(&game,&slowdt,&turn,&pitch,&throttle);
 RCHK(game.race.crash_time==0&&turn==.35f&&pitch==-.25f&&throttle==-1,"camera completes promptly and returns every flight control to the player");
 race_restart(&game);RCHK(game.race.crash_id==-1&&game.race.crash_time==0&&game.race.takedowns==0&&!game.race.pilot[0].disabled,"retry restores destroyed rivals and clears camera, debris and takedown state");
 game.race.phase=RACE_RUNNING;q=game.race.gates[0];game.pos=q.booster;game.speed=400;game.boost=0;game.race.boost_time=0;
 for(int i=0;i<game.race.rivals;i++)game.race.pilot[i].pos=add(game.pos,mul(q.right,8000+i*600));
 game.race.pilot[0].pos=add(game.pos,mul(q.normal,80));game.race.pilot[0].speed=400;game.race.pilot[0].dir=q.normal;
 race_tick(&game,game.pos,.016f);
 RCHK(!game.race.pilot[0].disabled&&game.race.pilot[0].bump>0&&game.speed<400&&game.race.crash_time==0,"gentle contact nudges the ships and costs speed without an explosion or forced camera");

 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;
 game.npc[0].alive=1;game.npc[0].pos=add(game.pos,mul(forward(&game),500));int ammo=game.missiles;
 RCHK(!fire_missile(&game,NPC_ID_MIN)&&game.missiles==ammo,"race rules also block missile launches without consuming ammunition");
 game.police_stop=1;race_preflight(&game,.05f);RCHK(game.race.phase==RACE_GRID&&game.police_stop,"Law interception interrupts the race and keeps the local warrant active");
 /* Dense view: ten rivals, multiple 3D rings, both boost gates and cockpit. */
 race_review_fixture();race_restart(&game);game.race.phase=RACE_RUNNING;game.race.rivals=10;
 for(int i=0;i<10;i++){game.race.pilot[i]=game.race.pilot[i%5];game.race.pilot[i].pos=add(game.pos,add(mul(game.race.dir,700+(i/5)*500),mul(game.race.right,(i%5-2)*230)));}
 game.pos=add(game.race.gates[0].pos,mul(game.race.gates[0].normal,-6000));game.yaw=atan2f(game.race.gates[0].normal.x,game.race.gates[0].normal.z);game.pitch=asinf(game.race.gates[0].normal.y);
 for(int i=0;i<10;i++)game.race.pilot[i].pos=add(game.pos,add(mul(forward(&game),700+(i/5)*500),mul(game.race.right,(i%5-2)*230)));
 SceUInt64 t0,t1;sceRtcGetCurrentTick(&t0);int submitted_max=0;
 for(int frame=0;frame<40;frame++){render_submitted=0;rect(0,0,W,H,BG);drawcount=0;space();if(render_submitted>submitted_max)submitted_max=render_submitted;}
 sceRtcGetCurrentTick(&t1);double seconds=(double)(t1-t0)/sceRtcGetTickResolution();
 fprintf(out,"INFO dense race draw: %d max submitted triangles, %.2f emulator render FPS (no hardware guarantee)\n",submitted_max,seconds>0?40/seconds:0);
 RCHK(submitted_max<2000,"ten-rival race view stays below the bounded 2048-triangle queue");dump_native_bmp("race-ten-rivals.bmp");
 for(int theme=0;theme<4;theme++){
  game.race.elapsed=30+theme*25;game.race.gate=theme*8;RaceGate *gate=&game.race.gates[game.race.gate];game.pos=add(gate->booster,mul(gate->normal,-6500));game.yaw=atan2f(gate->normal.x,gate->normal.z);game.pitch=asinf(gate->normal.y);
  char name[48];snprintf(name,sizeof(name),"race-spectacle-%d.bmp",theme);render_submitted=0;race_review_frame(name);
  RCHK(render_submitted<2048,"scenery sector stays within the shared geometry queue at native resolution");
 }
 FILE *audioflag=fopen("race-audio.flag","r");if(audioflag){
  fclose(audioflag);radio_race_on=1;radio_station=4;radio_off=1;comms_quiet=1;radio_dirty=0;audio_init();
  for(int wait=0;wait<80&&radio_file_station!=8;wait++)sceKernelDelayThread(25000);
  int samples=mp3_handle>=0?sceMp3GetSumDecodedSample(mp3_handle):-1;sceKernelDelayThread(150000);
  RCHK(audio_run&&radio_file_station==8&&music_format==1&&radio_file_error==0&&mp3_handle>=0&&sceMp3GetSumDecodedSample(mp3_handle)>samples,"real MP3 decoder advances Sol Rock even with saved music off and Quiet Mode on");
  RCHK(radio_file_track>=0&&!strcmp(radio_music_dirs[8],"music/Sol Rock")&&strstr(radio_tracks[8][radio_file_track],"music/Sol Rock/")==radio_tracks[8][radio_file_track]&&!audio_intro_skip_remaining,"race playback opens the MP3 in music/Sol Rock, without applying the intro-song skip");
  fprintf(out,"INFO actual race file: %s; source %d Hz; output 44100 Hz; SDK %08x\n",(char*)radio_file_title,mp3_rate,sceKernelGetCompiledSdkVersion());
  RCHK(mp3_step==mp3_rate&&mp3_rate>=8000&&mp3_rate<=48000&&sceKernelGetCompiledSdkVersion()>=0x03090500,"non-44.1kHz MP3 uses modern decoder support and correct output resampling");
  RCHK(comms_quiet==1&&radio_off==1,"race music override preserves the player's Quiet Mode and music settings");radio_race_on=0;radio_station=7;radio_off=0;comms_quiet=0;audio_intro_seek_pending=0;
  for(int wait=0;wait<100&&radio_file_station!=7;wait++)sceKernelDelayThread(25000);
  RCHK(radio_file_station==7&&music_format==1&&radio_file_error==0,"audio worker crossfades back to the selected station after the race override ends");audio_stop();
 }
 int guards=1;for(int i=0;i<16;i++)guards&=mem[i]==0xa278d31eu&&mem[STRIDE*H+16+i]==0xa278d31eu;
 RCHK(guards,"all native race renders preserve framebuffer bounds");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(mem);free(snapshot);radio_race_on=0;
 #undef RCHK
 return 1;
}
