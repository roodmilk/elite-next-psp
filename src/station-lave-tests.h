/* Integration checks inside input_tests; no player saves are touched. */
{
 TEST_INIT();game.docked=1;page=WALK;game.system=7;game.station_variant=0;sc_built_for=-1;sc_build_map();
 int graph_ok=1,anchors_ok=1,identities_ok=1;unsigned visited=1;
 for(int pass=0;pass<7;pass++)for(int room=0;room<7;room++)if(visited&(1u<<room)){
  int exits[4],n=sc_exits(room,exits,4);for(int i=0;i<n;i++){if(exits[i]<0||exits[i]>=7)graph_ok=0;else visited|=1u<<exits[i];}
 }
 INPUT_CHECK(graph_ok&&visited==127,"station kit: seven-room graph fully reachable");
 for(int sys=0;sys<256;sys++)for(int hub=0;hub<HUB_COUNT;hub++){
  game.system=sys;game.station_variant=hub;StationIdentity a=sc_identity(),b=sc_identity();
  identities_ok&=a.system==sys&&a.variant==hub&&a.seed==b.seed;
  for(int room=0;room<7;room++){
   sc_room=room;ScHot h[24];int n=sc_hotspots(h,24);if(n<3||n>=24)anchors_ok=0;
   for(int i=0;i<n;i++)if(h[i].x<SC_VX||h[i].y<SC_VY||h[i].x+h[i].w>SC_VX+SC_VW||h[i].y+h[i].h>SC_VY+SC_VH||!h[i].label)anchors_ok=0;
   unsigned seed=sc_room_seed(room);if(seed!=sc_room_seed(room))identities_ok=0;
  }
 }
 INPUT_CHECK(identities_ok,"station kit: deterministic identity/room seeds in all 256 systems and three hub variants");
 INPUT_CHECK(anchors_ok,"station kit: every generated room has bounded labelled hotspots (5376 rooms)");
 TEST_INIT();game.docked=1;page=WALK;sc_built_for=-1;sc_build_map();
 tracked_mission=TRACK_LAVE;
 INPUT_CHECK(mission_track_at(mission_track_row())==TRACK_LAVE,"Lave log: browsing the lead before accepting still selects the correct log entry");
 tracked_mission=0;
 int cash=game.credits;sc_room=SC_R_CARGO;
 lave_action(SC_R_CUSTOMS);INPUT_CHECK(sc_lave_stage()==0&&game.credits==cash,"Lave: out-of-order clearance does not skip story");
 lave_action(SC_R_CARGO);INPUT_CHECK(sc_lave_stage()==1&&tracked_mission==TRACK_LAVE,"Lave: cargo forewoman starts tracked activity");
 sc_hot=sc_find_hot(SC_H_PROP,17);sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(sc_lave_stage()==2&&sc_manifest_state()==1,"Lave: actual manifest hotspot collects sheet");
 const int rooms[]={SC_R_CANTEEN,SC_R_SHOP,SC_R_CLINIC,SC_R_CUSTOMS,SC_R_CARGO,SC_R_GUILD};
 int flow_ok=1,save_ok=1;
 for(int i=0;i<6;i++){
  sc_room=rooms[i];sc_menu=SC_MENU_TALK;sc_talk_who=0;sc_talk_row=1;sc_input(PSP_CTRL_CROSS);
  if(sc_lave_stage()!=i+3)flow_ok=0;
  if(!save_game(&game,"test-lave.sav")||!load_game(&game,"test-lave.sav")||sc_lave_stage()!=i+3)save_ok=0;
 }
 INPUT_CHECK(flow_ok,"Lave: visible NPC replies progress entire manifest and guild follow-up");
 INPUT_CHECK(save_ok,"Lave: every intermediate mission stage survives V19 save/load");
 INPUT_CHECK(game.credits==cash+600&&sc_manifest_state()==2,"Lave: manifest reward paid once");
 lave_action(SC_R_CARGO);lave_action(SC_R_GUILD);
 INPUT_CHECK(game.credits==cash+600&&sc_lave_stage()==8,"Lave: no repeat payment or survey payment without records");
 game.surface_progress[7][1]|=1u<<8;lave_action(SC_R_GUILD);lave_action(SC_R_GUILD);
 INPUT_CHECK(sc_lave_stage()==9&&game.credits==cash+1500,"Lave: real Lave I scan completes follow-up exactly once");
 game.system=42;game.station_variant=1;sc_manifest_set(1);game.station_variant=2;sc_manifest_set(2);
 game.system=7;game.station_variant=0;INPUT_CHECK(sc_lave_stage()==9&&sc_manifest_state()==2,"station progress: visiting other hubs preserves Lave completion");
 INPUT_CHECK(save_game(&game,"test-lave.sav")&&load_game(&game,"test-lave.sav")&&game.station_progress[42][1]==1&&game.station_progress[42][2]==2,"station progress: separate hub activities survive save/load");
 remove("test-lave.sav");remove("test-lave.sav.bak");
 for(int r=0;r<7;r++){sc_room=r;sc_menu=0;sc_hot=0;sc_read_room();game.message_time=0;memset(pixels,0,STRIDE*H*sizeof(unsigned));sc_draw_ui();char path[64];snprintf(path,sizeof(path),"lave-room-%d.bmp",r);dump_native_bmp(path);}
 sc_room=SC_R_CARGO;sc_menu=SC_MENU_TALK;sc_talk_who=0;sc_talk_row=1;ScNpc captured[3];sc_fill_npcs(sc_room,captured,3);sc_read_contact(&captured[0]);game.message_time=5;sc_draw_ui();menu_notice();dump_native_bmp("lave-dialogue.bmp");
 /* Normal service screens return to the same room, not to flight. */
 sc_room=SC_R_SHOP;sc_menu=0;sc_hot=sc_find_hot(SC_H_FEATURE,101);page=WALK;nav_depth=0;sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(page==EQUIP,"Lave services: painted outfitting terminal opens fitted-slot shop");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);INPUT_CHECK(page==WALK&&sc_room==SC_R_SHOP,"Lave services: Circle returns to the same room");
 /* Legacy cargo bits migrate once into the matching hub without losing gifts. */
 game.system=42;game.station_variant=1;*sc_progress()=0;game.gift_flags=(int)(sc_manifest_key()<<10)|(1<<8)|3;sc_manifest_prepare();
 INPUT_CHECK(sc_manifest_state()==1&&game.gift_flags==3,"station migration: old manifest moves to matching hub and preserves low gift bits");
 /* A specimen in another system cannot satisfy this local survey. */
 ScNpc survey={0};survey.act=SC_ACT_QUEST;survey.quest_pay=600;survey.name="SURVEYOR";survey.role=EXPLORERS;survey.line="Bring a local scan.";survey.offer="Upload record";
 game.system=42;game.station_variant=0;*sc_progress()=0;cash=game.credits;sc_do_npc_choice(&survey,1);
 INPUT_CHECK(game.credits==cash,"station survey: remote records do not pay for a local survey");
 game.surface_progress[42][1]|=1u<<8;sc_do_npc_choice(&survey,1);sc_do_npc_choice(&survey,1);
 INPUT_CHECK(game.credits==cash+600,"station survey: local scan pays once per hub");
 game.system=7;game.station_variant=0;page=WALK;sc_menu=0;sc_built_for=-1;sc_build_map();
 unsigned long long t0,t1;float worst=0;
 for(int r=0;r<7;r++){sc_room=r;sceRtcGetCurrentTick(&t0);for(int j=0;j<10;j++)sc_draw_ui();sceRtcGetCurrentTick(&t1);float ms=(float)(t1-t0)*100.f/sceRtcGetTickResolution();if(ms>worst)worst=ms;}
 fprintf(f,"METRIC station worst room render %.2f ms (10 draws per room)\n",worst);
 INPUT_CHECK(worst<33.3f,"station kit: all seven room CPU render costs within a 30fps frame in emulator");
 TEST_INIT();sc_built_for=-1;
}
