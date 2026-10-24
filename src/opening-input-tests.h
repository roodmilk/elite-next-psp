#include "opening-dialogue-tests.h"
/* Run through input adapters rather than assigning story milestones. */
static void opening_test_step(unsigned p,unsigned h,float ax,float ay){input(0,0,1.f/30,0,0);input(p,h,1.f/30,ax,ay);}
static void opening_test_capture(const char *name){
 rect(0,0,W,H,BG);drawcount=0;
 switch(page){case FLIGHT:space();break;case CHART:chart();break;case YARD:yard();break;
 case EQUIP:equipment();break;case GALNET:galnet_screen();break;case DETAILS:system_details();break;
 case LOCALTV:local_tv_screen();break;case REPAIR:repair_screen();break;case MARKET:market_screen();break;
 case DECORATOR:decorator_screen();break;default:home();break;}
 opening_draw();dump_native_bmp(name);
}
static void opening_run_tests(void){
 FILE *f=fopen("opening-check.txt","w");if(!f)return;unsigned *prior_fb=fb,*test_pixels=malloc(STRIDE*H*sizeof(unsigned));if(!test_pixels){fclose(f);return;}fb=test_pixels;int failures=0,old_smoke=smoke,old_slot=commander_active_slot;
 static Game prior;prior=game;const char *old_paths[3];for(int i=0;i<3;i++)old_paths[i]=commander_slots[i];
 static const char *test_paths[]={"test-opening-slot-0.sav","test-opening-slot-1.sav","test-opening-slot-2.sav"};
 for(int i=0;i<3;i++)commander_slots[i]=test_paths[i];smoke=1;
#define OPEN_CHECK(c,n) do{int ok=(c);fprintf(f,"%s ship%d %s\n",ok?"PASS":"FAIL",ship,n);failures+=!ok;}while(0)
 failures+=opening_dialogue_validate(f);
 #include "opening-dream-tests.h"
 {int ship=0;game_init(&game);game.opening.stage=OP_FIND;game.opening.flags=OF_DIALOG_DONE;opening_world_apply(&game);game_spawn(&game);game.planet=1;game.surface=2;game.docked=0;game.ship_pos=surface_site(&game,1);Vec3 pad=game.ship_pos;page=FLIGHT;opening_frontend_reset();opening_prepare();
 OPEN_CHECK(earth_meadow_inside(95,55)&&earth_meadow_inside(-86,118)&&earth_meadow_inside(125,245),"meadow orb lake-bank and overlook destinations are on accessible land");
 OPEN_CHECK(!earth_meadow_inside(0,310)&&!earth_meadow_inside(360,0)&&!earth_meadow_inside(-174,132),"meadow footprint excludes cliff outer boundary and lake");
 game.pos=add(pad,(Vec3){340,0,0});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=1.5707963f;
 for(int k=0;k<60;k++)opening_test_step(0,PSP_CTRL_UP,0,0);
 OPEN_CHECK(hypotf(game.pos.x-pad.x,game.pos.z-pad.z)<351,"actual walking cannot escape the compact meadow");
 game.pos=opening_object_pos();int stage=game.opening.stage;opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);OPEN_CHECK(game.opening.stage==stage,"orb cannot skip Dad's exploration beat immediately");
 game.opening.flags|=OF_MEADOW_GLOW;game.opening.flags|=OF_MEADOW_LAKE|OF_MEADOW_VIEW;
 {static Game restored;OPEN_CHECK(save_game(&game,"test-meadow.sav")&&load_game(&restored,"test-meadow.sav")&&(restored.opening.flags&(OF_MEADOW_GLOW|OF_MEADOW_LAKE|OF_MEADOW_VIEW))==(OF_MEADOW_GLOW|OF_MEADOW_LAKE|OF_MEADOW_VIEW),"optional meadow discoveries and glow persist through save/reload");}
 game.pos=add(pad,(Vec3){35,0,-110});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.voice_time=game.message_time=0;game.pitch=0;game.yaw=.25f;
 for(int angle=0;angle<6;angle++){game.yaw=angle<4?angle*1.5707963f:.25f;game.pitch=angle==4?.6f:angle==5?-.6f:0;char shot[64];snprintf(shot,sizeof(shot),"meadow-278-view-%d.bmp",angle);opening_test_capture(shot);}

 game.pos=add(pad,(Vec3){125,0,245});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=0;game.pitch=-.25f;opening_test_capture("meadow-278-overlook.bmp");
 game.pos=add(pad,(Vec3){-86,0,118});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=-1.5707963f;game.pitch=-.1f;opening_test_capture("meadow-278-lake.bmp");
 game.pos=add(pad,(Vec3){1000,0,1000});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;opening_prepared=-1;opening_prepare();OPEN_CHECK(earth_meadow_inside(game.pos.x-pad.x,game.pos.z-pad.z),"legacy out-of-bounds meadow checkpoint returns to safe accessible ground");
 game.pos=add(pad,(Vec3){35,0,-110});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
 #include "earth-meadow-tests.h"
 #include "vegetation-tests.h"
 #include "earth-meadow-clue-tests.h"
 game.opening.stage=OP_FIND;game.opening.flags=OF_DIALOG_DONE|OF_MEADOW_GLOW;game.pos=opening_object_pos();game.pitch=game.yaw=0;opening_notice=0;opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);OPEN_CHECK(game.opening.stage==OP_OBJECT,"close Triangle interaction still enters expanded orb conversation");
 opening_frontend_reset();
 }

 {int ship=0,valid=1,wrap=1,bases=1;
 for(int sp=0;sp<10;sp++){unsigned d=portrait_seeded(177,TRADERS,sp);valid&=portrait_valid(d)&&portrait_species(d)==sp;
 for(int part=0;part<PORTRAIT_PART_COUNT;part++){unsigned q=d;for(int k=0;k<portrait_part_count(part);k++){valid&=portrait_valid(q);q=portrait_adjust(q,part,1);}wrap&=q==d;}
 unsigned q=portrait_set_field(portrait_set_field(d,4,3,0),16,7,0);int seen[96]={0};for(int k=0;k<8;k++){int b=portrait_pulp_base(q);bases&=!seen[b];seen[b]=1;q=portrait_adjust(q,PORTRAIT_PART_ACCESSORY,1);}}
 OPEN_CHECK(valid&&wrap,"portrait expanded features validate and wrap across all ten races");OPEN_CHECK(bases,"all eight face variants select distinct authored bases per race");
 /* 0x8c000000 is now a legal v3 appearance; bit 24 remains reserved. */
 OPEN_CHECK(portrait_valid(7)&&portrait_valid(0x80c3f103u)&&portrait_valid(0x8c000000u)&&!portrait_valid(0x8d000000u)&&!portrait_valid(0x8400000fu),"portrait old and v3 saves remain valid and malformed DNA is rejected");
 game_init(&game);game.commander_portrait=portrait_seeded(277,TRADERS,0);{static Game restored;unsigned expected=game.commander_portrait;OPEN_CHECK(save_game(&game,"test-portrait-277.sav")&&load_game(&restored,"test-portrait-277.sav")&&restored.commander_portrait==expected,"expanded portrait DNA persists through real save and reload");}profile_portrait_edit=1;profile_from_intro=0;profile_portrait_part=9;rect(0,0,W,H,BG);status();dump_native_bmp("portrait-creator-277.bmp");profile_portrait_edit=0;
 rect(0,0,W,H,BG);text(2,1,UI_GOLD,"TEN RACES / EIGHT FACE VARIANTS / NATIVE 22PX");for(int sp=0;sp<10;sp++)for(int v=0;v<8;v++){unsigned d=portrait_seeded(277,TRADERS,sp);d=portrait_set_field(d,4,3,0);d=portrait_set_field(d,16,7,v);portrait_draw(12+v*57,24+sp*24,22,22,d,TRADERS);}dump_native_bmp("portrait-diversity-277.bmp");
 }
 #include "portrait-qa-tests.h"

 #include "comms-console-tests.h"
 {int ship=0;game_init(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;page=FLIGHT;game.legal=game.wanted[game.system]=0;game.pos=hub_position(&game,0);const char *heard[20];int unique=1,short_copy=1;
 for(int i=0;i<20;i++){heard[i]=station_hail_line(0);short_copy&=strlen(heard[i])<150;for(int j=0;j<i;j++)if(!strcmp(heard[i],heard[j]))unique=0;}
 OPEN_CHECK(unique&&short_copy,"station cycles twenty distinct readable friendly greetings");
 game.wanted[game.system]=1;const char *law_reply=station_hail_line(0);OPEN_CHECK(strstr(law_reply,"warrant")||strstr(law_reply,"fine"),"wanted station replies require law clearance");game.wanted[game.system]=0;
 game.pos=add(hub_position(&game,0),(Vec3){0,0,-100000});const char *distant_reply=station_hail_line(0);OPEN_CHECK(strstr(distant_reply,"closer")||strstr(distant_reply,"range")||strstr(distant_reply,"Approach"),"distant station replies do not offer immediate docking");
 game.pos=add(hub_position(&game,0),(Vec3){0,0,-3000});selected_target=0;open_station_channel();OPEN_CHECK(page==FLIGHT&&!game.dock_stage&&!game.docked&&game.voice_who==VOICE_DOCK,"greeting stays in flight and never docks automatically");opening_test_capture("station-greeting-276.bmp");
 }

 {int ship=0;unsigned expected[32*32];rect(0,0,64,64,BG);opening_portrait("COMPUTER",0,0,32);for(int y=0;y<32;y++)for(int x=0;x<32;x++)expected[y*32+x]=fb[y*STRIDE+x];rect(0,0,64,64,BG);ship_computer_portrait(0,0,32,32);int same=1;for(int y=0;y<32;y++)for(int x=0;x<32;x++)same&=expected[y*32+x]==fb[y*STRIDE+x];OPEN_CHECK(same,"cockpit and intro use identical computer portrait pixels");
 game_init(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;page=FLIGHT;game.voice_time=0;message(&game,"Sensor check complete. All systems ready.");opening_test_capture("computer-face-275.bmp");}

 {int ship=0;game_init(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;page=FLIGHT;game.voice_time=game.message_time=0;comms_quick=0;night_npc=-1;triangle_arm=0;
 selected_target=0;target_explicit=1;game_input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.05f,0,0);game_input(0,0,.05f,0,0);
 OPEN_CHECK(page==FLIGHT&&game.voice_who==VOICE_DOCK,"tapping Triangle on station hails without opening console");
 game_input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.1f,0,0);for(int k=0;k<6;k++)game_input(0,PSP_CTRL_TRIANGLE,.1f,0,0);
 OPEN_CHECK(page==COMMS_PANEL,"holding Triangle still opens console after station hail");page=FLIGHT;comms_encounter_conversation=0;
 for(int i=0;i<75;i++){speak(&game,VOICE_CONTACT,discovery_catalogue[i].reply);game.voice_seed=-1000-i;OPEN_CHECK(discovery_voice_label()&&strlen(discovery_voice_label())<=12,"every discovery has a compact explicit speaker identity");}
 speak(&game,VOICE_CONTACT,discovery_catalogue[2].reply);game.voice_seed=-1002;game.voice_role=TRADERS;OPEN_CHECK(!strcmp(discovery_voice_label(),"MECHANIC")&&!discovery_voice_automated(),"mobile mechanic speaks as a mechanic");opening_test_capture("mechanic-speaker-274.bmp");
 speak(&game,VOICE_CONTACT,discovery_catalogue[61].reply);game.voice_seed=-1061;OPEN_CHECK(discovery_voice_automated(),"abandoned wreck is inspected by the computer");
 speak(&game,VOICE_KEI,"A new transmission.");OPEN_CHECK(!discovery_voice_label(),"new transmissions cannot inherit a previous discovery speaker");
 }

 {int ship=0;game_init(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;game.pos=(Vec3){0,0,-20000};
 NPC *n=&game.npc[0];n->alive=1;n->freighter=0;n->traveller=-1;n->bounty_slot=-1;n->role=EXPLORERS;n->name_known=0;
 n->pos=add(game.pos,(Vec3){0,0,2000});selected_target=NPC_ID_MIN;target_explicit=1;
 lock_local_target(selected_target,"test");OPEN_CHECK(!scanner_known(selected_target),"target lock never reveals contact identity");
 OPEN_CHECK(target_detected(selected_target)&&!strcmp(target_name(selected_target),"UNKNOWN"),"nearby contact starts unknown");
 page=FLIGHT;game.voice_time=game.message_time=0;square_held=1;scan_cat=1;opening_test_capture("sensor-unknown-273.bmp");square_held=0;
 hail_target();OPEN_CHECK(scanner_known(selected_target),"close successful hail reveals contact identity");
 Vec3 last=target_position(selected_target);n->pos=add(game.pos,(Vec3){0,0,100000});
 OPEN_CHECK(!target_detected(selected_target)&&length(sub(last,target_position(selected_target)))<.1f,"lost contact freezes last known position");
 int ids[TARGET_CAPACITY],count=collect_scan_ids(ids,1),hidden=1;for(int i=0;i<count;i++)if(ids[i]==selected_target)hidden=0;
 OPEN_CHECK(hidden,"out-of-range ship is absent from sensor list");
 n->pos=add(game.pos,(Vec3){0,0,12000});game.upgrades&=~16;for(int i=0;i<FIT_SLOTS;i++)if(game.fit[i]==38)game.fit[i]=-1;
 float base=sensor_range();game.upgrades|=16;OPEN_CHECK(sensor_range()>base&&target_detected(selected_target),"scanner upgrade expands actual detection range");
 OPEN_CHECK(target_detected(1)&&target_detected(2),"sun and planets remain visible beyond sensors");
 identity_sync();discovery_identified[game.system]|=4;identity_save();identity_slot=-99;identity_sync();OPEN_CHECK(discovery_identified[game.system]&4,"discovery identities persist across profile reload");
 n->pos=add(game.pos,(Vec3){0,0,100000});n->name_known=0;selected_target=NPC_ID_MIN;hail_target();OPEN_CHECK(!n->name_known&&comms_unavailable(1),"out-of-range hail cannot reveal identity");

 }

 {int ship=0;game_init(&game);game.opening.stage=OP_FREE;game.system=7;game_spawn(&game);game.docked=0;game.yaw=game.pitch=game.roll=0;page=FLIGHT;
 OPEN_CHECK(discovery_count()==8,"Lave discovery pool is bounded");
 for(int i=0;i<8;i++){int id=DISCOVERY_ID_MIN+i;Vec3 a=discovery_position(i),b=discovery_position(i);
 OPEN_CHECK(valid_target(id)&&length(sub(a,b))<.01f,"discovery has stable target and seeded position");
 int safe=1;for(int j=0;j<BODY_COUNT;j++)if(length(sub(a,game.bodies[j].pos))<game.bodies[j].radius+1000)safe=0;
 OPEN_CHECK(safe,"discovery lies outside planetary surfaces");
 selected_target=id;target_explicit=1;game.pos=add(a,(Vec3){0,0,-2000});
 OPEN_CHECK(!comms_unavailable(1),"nearby discovery can be contacted through console");hail_target();
 OPEN_CHECK(game.voice_time>0,"discovery responds through existing speech channel");if(i==2)opening_test_capture("discovery-cage-272.bmp");}
 game.system=5;OPEN_CHECK(discovery_count()==0,"uncertified systems retain existing content");
 }

 {
  int ship=0;game_init(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;
  int body=3;Body *b=&game.bodies[body];int rocks=0,first=-1,inside=1;
  for(int i=0;i<DEBRIS_COUNT;i++)if(game.debris[i].alive&&game.debris[i].rock){
   float d=length(sub(game.debris[i].pos,b->pos));
   if(d>=planet_ring_inner(b)-1&&d<=planet_ring_outer(b)+1){rocks++;if(first<0)first=i;}
  }
  OPEN_CHECK(rocks>=16,"gas giant has physical mineable ring asteroids");
  for(int i=0;i<24;i++){Vec3 p=planet_ring_point(b,i*.261799f,planet_ring_inner(b));float d=length(sub(p,b->pos));inside&=fabsf(d-planet_ring_inner(b))<2;}
  OPEN_CHECK(inside&&planet_ring_outer(b)<=b->radius*2.61f&&planet_ring_inner(b)>=b->radius*1.39f,"ring geometry uses a proportional tilted world-space annulus");
  game.pos=planet_ring_point(b,.8f,(planet_ring_inner(b)+planet_ring_outer(b))*.5f);game.approach=-1;game.planet=-1;game_tick(&game,.016f,0,0,0,0);
  OPEN_CHECK(game.planet<0&&game.approach<0,"flying in planetary rings does not initiate landing");
  game.voice_time=game.message_time=0;selected_target=body+1;game.yaw=atan2f(b->pos.x-game.pos.x,b->pos.z-game.pos.z);game.pitch=0;page=FLIGHT;opening_test_capture("rings-inside-270.bmp");
  game.pos=add(b->pos,(Vec3){0,b->radius*8,-b->radius*18});game.yaw=0;game.pitch=-atan2f(b->radius*8,b->radius*18);opening_test_capture("rings-wide-270.bmp");
  if(first>=0){game.fit[FIT_WPN]=1;fit_rebuild(&game);game.debris[first].health=1;game.pos=add(game.debris[first].pos,(Vec3){0,0,-200});OPEN_CHECK(mine_rock(&game,DEBRIS_ID_MIN+first)&&!game.debris[first].rock,"ring asteroid fractures into recoverable ore");}
 }
 {
  int ship=0;
  for(int outcome=0;outcome<3;outcome++){
   opening_new_game();game.opening.stage=OP_DREAM;opening_prepared=-1;opening_prepare();
   OPEN_CHECK(!game.npc[0].alive&&!opening_current_dialogue(),"dream begins with empty space and no blocking dialogue");
   input(0,0,.033f,0,0);OPEN_CHECK(game.opening.stage==OP_DREAM,"dream cannot end after a few shots or input frames");
   dream_time=6;input(0,0,.033f,0,0);OPEN_CHECK(dream_spawned==3,"dream introduces exactly three contacts");
   OPEN_CHECK(radio_dream_gain==0,"dream music waits for attack cue");
   dream_time=23;input(0,0,.033f,0,0);OPEN_CHECK(radio_dream_gain==1000,"attack cue enables rock soundtrack");
   if(outcome==0)opening_test_capture("dream-attack-269.bmp");
   if(outcome==0){for(int i=0;i<3;i++)game.npc[i].alive=0;}
   if(outcome==1)game.hull=24;
   if(outcome==2)dream_time=DREAM_LIMIT;
   input(0,0,.033f,0,0);OPEN_CHECK(dream_ending,"victory near-death and timeout each start waking");
   OPEN_CHECK(game.opening.stage==OP_DREAM,"wake transition keeps dream alive during fade");
   dream_wake=3;input(0,0,.033f,0,0);OPEN_CHECK(radio_dream_gain>0&&radio_dream_gain<1000,"dream music fades during waking");
   dream_wake=6;input(0,0,.033f,0,0);OPEN_CHECK(game.opening.stage==OP_HOME&&!game.shots&&!game.kills,"waking restores real commander without dream rewards");
   OPEN_CHECK(radio_dream_gain==-1,"waking releases temporary rock override");
  }
 }
 {
  int ship=0;opening_new_game();game.opening.stage=OP_YAW;game.docked=0;game.dock_stage=0;game.planet=-1;opening_world_apply(&game);change_page(FLIGHT);opening_prepared=-1;opening_prepare();
  game.surface=0;departure_release=0;input(0,0,1.f/30,0,0);float yaw=game.yaw;input(0,PSP_CTRL_RIGHT,1.f/30,.7f,0);OPEN_CHECK(game.yaw!=yaw&&!opening_current_dialogue(),"coaching permits steering immediately without X acknowledgement");
  game.opening.stage=OP_THROTTLE;opening_prepared=-1;opening_prepare();game.speed=0;input(0,PSP_CTRL_RTRIGGER,1.f/30,0,0);OPEN_CHECK(game.speed>0&&!opening_current_dialogue(),"Dad acceleration cue permits throttle immediately");
  Body *earth=&game.bodies[1];Vec3 previous=add(earth->pos,(Vec3){0,0,-earth->radius-2000});game.pos=add(earth->pos,(Vec3){0,0,earth->radius+2000});game.approach=-1;float hull=game.hull;
  int guarded=opening_keep_clear(&game,previous);OPEN_CHECK(guarded&&game.planet<0&&game.approach<0&&game.hull==hull&&dot(forward(&game),norm(sub(game.pos,earth->pos)))>.99f,"swept Earth crossing turns safely outward without damage or landing");
  OPEN_CHECK(!approach_planet(&game,1)&&game.approach<0,"direct premature Earth approach is rejected");game.approach=1;OPEN_CHECK(!enter_planet(&game)&&game.planet<0&&game.approach<0,"stale premature entry is recovered instead of trapping pilot");
  game.opening.stage=OP_APPROACH;game.pos=previous;game.yaw=game.pitch=0;game.approach=1;OPEN_CHECK(enter_planet(&game)&&game.planet==1,"story-authorized Earth landing still works");
  game.planet=-1;game.surface=0;game.opening.stage=OP_MINE;game.approach=1;OPEN_CHECK(!enter_planet(&game)&&game.planet<0,"Earth re-entry stays closed after countryside stop");
  game.opening.stage=OP_SOL_MAP;game.opening.flags&=~OF_DIALOG_DONE;game.opening.counter=0;page=HOME;opening_draw();OPEN_CHECK(fb[70*STRIDE+100]==opening_orbit_bitmap.palette[opening_orbit_bitmap.pixels[70*480+100]],"first Earth conversation renders over orbital art");
  page=DETAILS;row=11;system_details();OPEN_CHECK(row>=0&&row<=BODY_COUNT,"native almanac repairs tutorial menu row before sprite lookup");
 }

 for(int ship=0;ship<3;ship++){
  int choice=ship==1?2:1,last=-1,ticks=0,steps=0,yard_asked=0,outfit_asked=0,finished=0;
  int menu_checks=0,repair_before=0,food_before=0,food_price=0,refuel_before=0,graph_reload=0,action_reload=0,home_reload=0;
  remove(test_paths[ship]);char side[64];snprintf(side,sizeof(side),"%s.bak",test_paths[ship]);remove(side);
  if(ship==0){
   for(int k=0;k<opening_dialogue_count();k++){
    const OpeningDialogueNode *n=&opening_dialogue_nodes[k];int valid=n->id%100>0&&n->choice_count>=0&&n->choice_count<=3;
    for(int j=k+1;j<opening_dialogue_count();j++)if(opening_dialogue_nodes[j].id==n->id)valid=0;
    if(n->next){const OpeningDialogueNode *next=opening_dialogue_node(n->next);valid=valid&&next&&next->stage==n->stage&&next->id>n->id;}
    for(int j=0;j<n->choice_count;j++){const OpeningDialogueNode *next=opening_dialogue_node(n->choice_next[j]);valid=valid&&n->choices[j]&&next&&next->stage==n->stage&&next->id>n->id;}
    if(!valid){fprintf(f,"FAIL invalid conversation node%d\n",n->id);failures++;}
   }
  }
  /* This fixture starts at the already-revealed title menu. The film's X and
   * repeat guard have their own real-input checks in intro-film-review.h. */
  commander_active_slot=ship;opening_ship_choice=ship;change_page(INTRO);intro_menu_age=1;intro_wait_release=0;intro_choice=1;input(PSP_CTRL_CROSS,0,1.f/30,0,0);
  OPEN_CHECK(page==STATUS&&game.opening.stage==OP_CREATE,"real title New Game starts character creation");
  opening_test_step(PSP_CTRL_DOWN,0,0,0);opening_test_step(PSP_CTRL_DOWN,0,0,0);
  opening_test_step(PSP_CTRL_CROSS,0,0,0);
  OPEN_CHECK(game.opening.stage==OP_DREAM,"character completion starts combat dream");
  while(opening_active(&game)&&steps++<18000){
   if(!game.dock_stage)opening_prepare();int s=game.opening.stage;
   if(s!=last){
    last=s;ticks=0;fprintf(f,"STAGE ship%d %d page%d\n",ship,s,page);fflush(f);
    if(s==OP_HOME)OPEN_CHECK(!game.kills&&!game.legal&&!game.shots&&game.cargo[0]==0&&fit_find(&game,1)<0,"dream rewards and combat loadout do not leak into reality");
    if(s==OP_SAVE_INFO)OPEN_CHECK(game.ship==ship,"starter selection retained");
    if(s==OP_LAUNCH)OPEN_CHECK(!radio_off&&radio_station==(ship==1?7:RADIO_DEFAULT_STATION),"radio lesson defaults to Elite Exploration and accepts the chosen station");
    if(s==OP_CIVILISATION)OPEN_CHECK(!radio_off&&radio_station==RADIO_DEFAULT_STATION,"recovering the signal at Lave selects Elite Exploration rather than forcing rock");
    if(s==OP_SOL_MAP)OPEN_CHECK(!opening_service_visible(&game,2)&&!opening_service_visible(&game,7)&&!opening_service_visible(&game,22),"galaxy map factions and lore hidden in Sol");
    if(s==OP_MUM_RING)OPEN_CHECK(!game.docked&&game.planet<0&&!(game.opening.flags&OF_PIZZA),"Mum calls in flight before docking and pizza purchase");
    if(s==OP_REPAIR){repair_before=game.credits;OPEN_CHECK(game.hull<100&&game.damaged,"rock lesson creates actual hull damage");}
    if(s==OP_REFUEL){refuel_before=game.credits;OPEN_CHECK(game.hull==100&&!game.damaged&&game.credits<repair_before,"Engineers repair genuinely fixes and charges");}
    if(s==OP_YARD_INTRO)OPEN_CHECK(game.fuel==player_ships[game.ship].range&&game.credits<refuel_before,"Engineers refuel genuinely fills and charges");
    if(s==OP_FOOD){food_before=game.credits;food_price=game.price[0];}
    if(s==OP_HOME_ROUTE)OPEN_CHECK(game.cargo[0]==1&&(game.opening.flags&OF_PIZZA)&&game.credits==food_before-food_price,"pizza bought through real Food commodity transaction");
    if(s==OP_RETURN||s==OP_WAKE){
     int before_stage=s,before_flags=game.opening.flags;float hull=game.hull;Vec3 position=game.pos;
     int reload=save_game(&game,"test-opening-input.sav")&&load_game(&game,"test-opening-input.sav");
     if(reload){opening_restore();opening_prepare();}
     OPEN_CHECK(reload&&game.opening.stage==before_stage&&game.opening.flags==before_flags&&game.hull==hull&&game.pos.x==position.x&&game.pos.z==position.z,"checkpoint reload preserves chosen object and exact playable position");
     if(s==OP_WAKE)OPEN_CHECK((game.opening.flags&OF_OBJECT)&&(game.opening.flags&OF_DISPLACED)&&game.cargo[0]==1&&!strcmp(game.systems[7].name,"LAVE"),"displacement retains object and pizza while restoring Lave");
    }
    if(ship==0&&(s==OP_HOME||s==OP_CITY||s==OP_DEALER||s==OP_WALK||s==OP_PEP||s==OP_MUM_TALK||s==OP_THARGOID||s==OP_WAKE||s==OP_APP||s==OP_APP_VIEW||s==OP_OUTFIT||s==OP_NEWS)){
     char image[64];snprintf(image,sizeof(image),"opening-stage-%02d.bmp",s);opening_test_capture(image);
    }
   }
   if(++ticks>(s==OP_DREAM?2100:1000)){fprintf(f,"FAIL ship%d trapped stage%d page%d surface%d planet%d dock%d seq%d distance%.1f elapsed%.1f\n",ship,s,page,game.surface,game.planet,game.dock_stage,game.planet_sequence,game.opening.distance,game.opening.elapsed);fprintf(f,"DEBUG fireblocked%d laser%d fit%d shots%d rockhp%.1f orbitrelease%d veil%.1f toolswait%d tractor%d autoaim%d\n",fire_blocked,game.laser,game.fit[FIT_WPN],game.shots,game.debris[DEBRIS_COUNT-1].health,planet_orbit_release,planet_orbit_veil,tools_wait_release,tools_tractor,autoaim);failures++;break;}
   if(opening_notice){opening_test_step(PSP_CTRL_CROSS,0,0,0);continue;}
   const OpeningDialogueNode *node=opening_current_dialogue();
   if(node){
    if(opening_dialogue_revealed()&&node->choice_count){
     int target=s==OP_OBJECT_CHOICE?choice-1:ship%node->choice_count;
     if(game.opening.counter%10!=target){opening_test_step(PSP_CTRL_DOWN,0,0,0);continue;}
     if(s==OP_HOME&&!home_reload){
      int encoded=game.opening.counter;int saved=save_game(&game,"test-opening-input.sav")&&load_game(&game,"test-opening-input.sav");
      if(saved){opening_restore();opening_prepare();opening_dialogue_sync();}
      OPEN_CHECK(saved&&game.opening.counter==encoded&&opening_current_dialogue()&&opening_current_dialogue()->id==node->id,"HOME reload preserves selected authored conversation rather than restarting morning");home_reload=1;continue;
     }
     if(s==OP_OBJECT_CHOICE&&!graph_reload){
      int encoded=game.opening.counter;int saved=save_game(&game,"test-opening-input.sav")&&load_game(&game,"test-opening-input.sav");
      if(saved){opening_restore();opening_prepare();opening_dialogue_sync();}
      OPEN_CHECK(saved&&game.opening.counter==encoded&&opening_current_dialogue()&&opening_current_dialogue()->id==node->id,"save resumes exact conversation node and selected response");graph_reload=1;continue;
     }
    }
    if(ship==0&&opening_dialogue_revealed()&&node->choice_count){char image[64];snprintf(image,sizeof(image),"opening-choice-%d.bmp",node->id);opening_test_capture(image);}
    opening_test_step(PSP_CTRL_CROSS,0,0,0);continue;
   }
   if(s==OP_YAW&&!action_reload){
    int saved=save_game(&game,"test-opening-input.sav")&&load_game(&game,"test-opening-input.sav");
    if(saved){opening_restore();opening_prepare();opening_dialogue_sync();}
    OPEN_CHECK(saved&&(game.opening.flags&OF_DIALOG_DONE)&&!opening_current_dialogue(),"mid-action save retains completed dialogue without replaying it");action_reload=1;
   }
   const OpeningBeat *b=opening_beat();
   if(page==HOME&&b&&b->service>=0){
    int stage=s;opening_test_step(PSP_CTRL_DOWN|PSP_CTRL_SELECT,0,0,0);
    if(game.opening.stage!=stage||page!=HOME||row!=b->service){fprintf(f,"FAIL ship%d menu lock stage%d\n",ship,s);failures++;}
    menu_checks++;opening_test_step(PSP_CTRL_CROSS,0,0,0);continue;
   }
   if(opening_is_scene(s)||opening_is_dialogue(s)){
    if(s==OP_OBJECT_CHOICE&&game.opening.choice!=choice)opening_test_step(PSP_CTRL_RIGHT,0,0,0);
    opening_test_step(PSP_CTRL_CROSS,0,0,0);continue;
   }
   if(ship==0&&ticks==1&&(s==OP_YAW||s==OP_WALK||s==OP_MINE||s==OP_LONELY)){
    int old_page=page,old_stage=game.opening.stage;unsigned forbidden=PSP_CTRL_SELECT|PSP_CTRL_TRIANGLE|PSP_CTRL_SQUARE;if(s==OP_WALK)forbidden|=PSP_CTRL_START;opening_test_step(forbidden,forbidden,0,0);OPEN_CHECK(page==old_page&&game.opening.stage==old_stage&&!eva_map_open,"unrelated flight and EVA shortcuts cannot escape the current lesson");
   }
   switch(s){
    case OP_DREAM:if(ticks==1)opening_test_step(0,0,0,0);else opening_test_step(ticks==2?PSP_CTRL_CROSS:0,PSP_CTRL_CROSS,.7f,.3f);break;
    case OP_FIRST_SAVE:opening_test_step(PSP_CTRL_CROSS,0,0,0);break;
    case OP_RADIO:if(radio_off){opening_test_step(PSP_CTRL_CROSS,0,0,0);if(ship==1)radio_tune(7);}else opening_test_step(PSP_CTRL_CIRCLE,0,0,0);break;
    case OP_SOL_MAP:case OP_EARTH_NET:case OP_DAD_INBOX:case OP_TRACK:case OP_GALAXY:case OP_APP_VIEW:
     if(ship==0&&(s==OP_SOL_MAP||s==OP_EARTH_NET||s==OP_GALAXY||s==OP_APP_VIEW)){char image[64];snprintf(image,sizeof(image),"opening-stage-%02d-menu.bmp",s);opening_test_capture(image);}
     opening_test_step(PSP_CTRL_CIRCLE,0,0,0);break;
    case OP_EARTH_ROUTE:case OP_STATION_ROUTE:case OP_HOME_ROUTE:case OP_LOG:opening_test_step(PSP_CTRL_CROSS,0,0,0);break;
    case OP_YAW:opening_test_step(0,0,.8f,0);break;
    case OP_PITCH:opening_test_step(0,0,0,.8f);break;
    case OP_THROTTLE:opening_test_step(ticks==1?PSP_CTRL_RTRIGGER:0,PSP_CTRL_RTRIGGER,0,0);break;
    case OP_BRAKE:opening_test_step(ticks==1?PSP_CTRL_LTRIGGER:0,PSP_CTRL_LTRIGGER,0,0);break;
    case OP_APPROACH:opening_test_step(PSP_CTRL_CROSS,0,0,0);break;
    case OP_TOUCHDOWN:case OP_BURST:opening_test_step(0,0,0,0);break;
    case OP_WALK:opening_test_step(0,(planet_seat.active||!planet_controls_ready)?0:PSP_CTRL_DOWN,0,0);break;
    case OP_FIND:if(!(game.opening.flags&OF_MEADOW_GLOW))opening_test_step(0,PSP_CTRL_DOWN,0,0);else{game.pos=opening_object_pos();opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);}break;
    case OP_RETURN:game.pos=game.ship_pos;game.pos.y+=4;opening_test_step((planet_seat.active||!planet_controls_ready)?0:PSP_CTRL_TRIANGLE,0,0,0);break;
    case OP_ORBIT:opening_test_step((planet_seat.active||!planet_controls_ready||ticks%2)?0:PSP_CTRL_RTRIGGER,0,0,0);break;
    case OP_LOADOUT:opening_test_step(fit_find(&game,OPENING_MINIBOLT)<0?PSP_CTRL_CROSS:PSP_CTRL_CIRCLE,0,0,0);break;
    case OP_MINE:opening_test_step(0,(ticks==1||planet_orbit_release||planet_orbit_veil>0||departure_release||fire_blocked)?0:PSP_CTRL_CROSS,0,0);break;
    case OP_COLLECT:opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);break;
    case OP_MUM_RING:opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);break;
    case OP_MUM_TALK:opening_test_step(PSP_CTRL_CROSS,0,0,0);break;
    case OP_STATION_LOCK:input(ticks%25==1?PSP_CTRL_TRIANGLE:0,ticks%25<22?PSP_CTRL_TRIANGLE:0,1.f/30,0,0);break;
    case OP_DOCK:case OP_LAVE_DOCK:
     if(page==COMMS_PANEL)opening_test_step(PSP_CTRL_CROSS,0,0,0);
     else if(!game.dock_stage)input(ticks%25==1?PSP_CTRL_TRIANGLE:0,ticks%25<22?PSP_CTRL_TRIANGLE:0,1.f/30,0,0);
     else opening_test_step(0,0,0,0);break;
    case OP_REPAIR:opening_test_step(PSP_CTRL_CROSS,0,0,0);break;
    case OP_REFUEL:opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);break;
    case OP_YARD:case OP_OUTFIT:{
     int *asked=s==OP_YARD?&yard_asked:&outfit_asked;
     if(!*asked){int credits=game.credits,owned_ship=game.ship;unsigned fit=game.fit[0];opening_test_step(PSP_CTRL_CROSS,0,0,0);OPEN_CHECK(game.credits==credits&&game.ship==owned_ship&&game.fit[0]==fit&&opening_notice,"Dad refuses purchase without deductions or changes");*asked=1;}
     else opening_test_step(PSP_CTRL_CIRCLE,0,0,0);break;
    }
    case OP_FOOD:opening_test_step(PSP_CTRL_RIGHT,0,0,0);break;
    case OP_HOME_FLIGHT:case OP_LONELY:case OP_CIVILISATION:if(ship==0&&ticks==1&&s==OP_CIVILISATION)opening_test_capture("opening-civilisation.bmp");opening_test_step(ticks==1?PSP_CTRL_RTRIGGER:0,PSP_CTRL_RTRIGGER,0,0);break;
    case OP_NEWS:if(ship==0){char shot[64];snprintf(shot,sizeof(shot),"opening-news-%d.bmp",game.opening.counter);opening_test_capture(shot);}opening_test_step(PSP_CTRL_CROSS,0,0,0);break;
    default:opening_test_step(0,0,0,0);break;
   }
  }
  finished=opening_done(&game);
  OPEN_CHECK(finished&&menu_checks>=14&&graph_reload&&action_reload&&home_reload,"full tutorial reaches free play through guided menu inputs");
  if(finished){OPEN_CHECK(game.opening.choice==choice&&((game.opening.flags&OF_SHOWN)!=0)==(choice==1),"show or hide choice persists through final broadcast");
   OPEN_CHECK((game.opening.flags&(OF_APP|OF_NEWS|OF_MUM|OF_MINED))==(OF_APP|OF_NEWS|OF_MUM|OF_MINED)&&!opening_service_visible(&game,7)&&!opening_service_visible(&game,22),"TV app mining and family milestones complete while later systems remain locked");}
 }
 for(int i=0;i<3;i++){remove(test_paths[i]);char side[64];snprintf(side,sizeof(side),"%s.bak",test_paths[i]);remove(side);snprintf(side,sizeof(side),"%s.tmp",test_paths[i]);remove(side);commander_slots[i]=old_paths[i];}
 remove("test-opening-input.sav");remove("test-opening-input.sav.bak");remove("test-opening-input.sav.tmp");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=prior_fb;free(test_pixels);smoke=old_smoke;commander_active_slot=old_slot;game=prior;opening_frontend_reset();
#undef OPEN_CHECK
}

