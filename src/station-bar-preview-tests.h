/* Smoke-only preview checks, included inside input_tests. */
{
 Game *bp_saved=(Game*)malloc(sizeof(Game));
 INPUT_CHECK(bp_saved!=NULL,"bar preview: test snapshot allocated");
 if(bp_saved){
  memcpy(bp_saved,&game,sizeof(Game));int bp_page=page,bp_hc=high_contrast;
  BarPracticeState bp_practice=bar_preview_practice;uint32_t bp_token=bar_preview_token;char bp_notes[3][96];memcpy(bp_notes,bar_preview_notes,sizeof(bp_notes));
  game_init(&game);game.system=39;game.station_variant=0;game.docked=1;game.dead=0;
  sc_built_for=-1;sc_build_map();sc_room=SC_R_CANTEEN;sc_menu=SC_MENU_NONE;
  sc_room=SC_R_ARRIVALS;
  INPUT_CHECK(station_authored_arrivals_at()&&strstr(sc_room_title(sc_room),"REORTE")!=NULL,"station shell: authored Arrivals plate is exact Reorte primary");
  INPUT_CHECK(NATIVE_ART_SCREEN_W==W&&NATIVE_ART_SCREEN_H==H&&SC_VX>=0&&SC_VY>=0&&SC_VX+SC_VW<=W&&SC_VY+SC_VH<=H&&SC_LX+SC_LW<=W&&SC_LY+SC_LH<=H,"station shell: native PSP scene and option rail stay inside 480x272");
  game.station_variant=1;INPUT_CHECK(!station_authored_arrivals_at(),"station shell: secondary hub keeps generic Arrivals art");
  game.station_variant=0;sc_build_map();sc_room=SC_R_CANTEEN;
  ScNpc bp_npcs[3];int bp_n=sc_fill_npcs(sc_room,bp_npcs,3);
  INPUT_CHECK(bp_n==3&&!strcmp(bp_npcs[0].name,"LYSA KEST")&&!strcmp(bp_npcs[2].name,"DAX NERAL"),"bar preview: three approved contacts at Reorte primary");
  int bp_cash=game.credits,bp_jobs=game.job_n;uint32_t bp_sim=game.rng;
  for(int bp_i=0;bp_i<100;bp_i++){bar_preview_token=bar_preview_practice.round_id+1;bar_preview_choose(SC_ACT_BAR_DAX,bp_i&1);}
  INPUT_CHECK(game.credits==bp_cash&&game.job_n==bp_jobs&&game.rng==bp_sim,"bar preview: free dice preserves credits jobs and simulation RNG");
  game.story=0;game.story_flags=0;nav_body=-1;bar_preview_choose(SC_ACT_BAR_PELL,1);
  INPUT_CHECK(nav_body==-1&&strstr(bar_preview_notes[1],"power lesson")!=NULL,"bar preview: pre-kit target rejected with training hint");
  game.story=STORY_WORLD;bar_preview_choose(SC_ACT_BAR_PELL,1);
  INPUT_CHECK(nav_body==1&&selected_target==2&&game.planet==-1&&game.discoveries==0,"bar preview: world choice sets navigation only");
  for(int bp_c=0;bp_c<2;bp_c++){high_contrast=bp_c;sc_menu=0;sc_hot=2;
   memset(pixels,0,STRIDE*H*sizeof(unsigned));sc_draw_ui();
   dump_native_bmp(bp_c?"bar-preview-contrast.bmp":"bar-preview-native.bmp");}
  sc_menu=SC_MENU_TALK;sc_talk_who=2;sc_talk_row=0;bar_preview_token=bar_preview_practice.round_id+1;sc_input(PSP_CTRL_CROSS);
  INPUT_CHECK(sc_menu==SC_MENU_NONE&&strstr(bar_preview_notes[2],"credits changed")!=NULL,"bar preview: Cross executes visible free dice choice");
  sc_input(PSP_CTRL_TRIANGLE);
  INPUT_CHECK(page==HOME,"bar preview: Triangle returns to ship deck");
  /* Full Reorte H0 room graph: every room door remains reachable through the selector. */
  memcpy(&game,bp_saved,sizeof(Game));game.system=39;game.station_variant=0;game.docked=1;game.dead=0;page=HOME;row=20;sc_built_for=-1;
  input(PSP_CTRL_CROSS,0,.016f,0,0);input(0,0,.016f,0,0);
  {const int from[]={SC_R_ARRIVALS,SC_R_SHOP,SC_R_CARGO,SC_R_CANTEEN,SC_R_ARRIVALS,SC_R_GUILD,SC_R_CLINIC,SC_R_CUSTOMS};
   const int to[]={SC_R_SHOP,SC_R_CARGO,SC_R_CANTEEN,SC_R_ARRIVALS,SC_R_GUILD,SC_R_CLINIC,SC_R_CUSTOMS,SC_R_ARRIVALS};
   int graph_ok=page==WALK;
   for(int gi_step=0;gi_step<8;gi_step++){
    sc_room=from[gi_step];input(0,0,.016f,0,0);
    ScHot graph_hot[24];int graph_n=sc_hotspots(graph_hot,24),graph_exit=-1;
    for(int graph_i=0;graph_i<graph_n;graph_i++)if(graph_hot[graph_i].kind==SC_H_EXIT&&graph_hot[graph_i].id==to[gi_step]){graph_exit=graph_i;break;}
    if(graph_exit<0){graph_ok=0;break;}
    sc_hot=graph_exit;input(PSP_CTRL_CROSS,0,.016f,0,0);
    if(sc_room!=to[gi_step]){graph_ok=0;break;}
   }
   INPUT_CHECK(graph_ok,"bar preview graph: Reorte H0 traverses all seven rooms via doors");
  }
  input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==HOME,"bar preview graph: full room traversal returns to ship");
  /* Proof manifest: full seven-room graph at Reorte H0, Qube H0 Freight, Xequerin H0 Research. */
  {const int proof_systems[]={39,1,4};const char *proof_names[]={"Reorte","Qube","Xequerin"};
   const int from[]={SC_R_ARRIVALS,SC_R_SHOP,SC_R_CARGO,SC_R_CANTEEN,SC_R_ARRIVALS,SC_R_GUILD,SC_R_CLINIC,SC_R_CUSTOMS};
   const int to[]={SC_R_SHOP,SC_R_CARGO,SC_R_CANTEEN,SC_R_ARRIVALS,SC_R_GUILD,SC_R_CLINIC,SC_R_CUSTOMS,SC_R_ARRIVALS};
   for(int proof_s=0;proof_s<3;proof_s++){
    memcpy(&game,bp_saved,sizeof(Game));game.system=proof_systems[proof_s];game.station_variant=0;game.docked=1;game.dead=0;page=HOME;row=20;sc_built_for=-1;
    input(PSP_CTRL_CROSS,0,.016f,0,0);input(0,0,.016f,0,0);int proof_ok=page==WALK;
    for(int proof_step=0;proof_step<8;proof_step++){
     sc_room=from[proof_step];input(0,0,.016f,0,0);
     ScHot proof_hot[24];int proof_n=sc_hotspots(proof_hot,24),proof_exit=-1;
     for(int proof_i=0;proof_i<proof_n;proof_i++)if(proof_hot[proof_i].kind==SC_H_EXIT&&proof_hot[proof_i].id==to[proof_step]){proof_exit=proof_i;break;}
     if(proof_exit<0){proof_ok=0;break;}
     sc_hot=proof_exit;
     if((proof_step&1)==0)sc_input(PSP_CTRL_CROSS);else input(PSP_CTRL_CROSS,0,.016f,0,0);
     if(sc_room!=to[proof_step]){proof_ok=0;break;}
    }
    INPUT_CHECK(proof_ok,proof_names[proof_s]);
    if(proof_s==0){ScNpc proof_people[3];INPUT_CHECK(sc_fill_npcs(SC_R_CANTEEN,proof_people,3)==3&&!strcmp(proof_people[0].name,"LYSA KEST"),"proof manifest: Reorte named bar cast");}
    else {ScNpc proof_people[3];INPUT_CHECK(sc_fill_npcs(SC_R_CANTEEN,proof_people,3)==2&&!strcmp(proof_people[0].name,"BARTEND"),"proof manifest: generic secondary cast");}
    input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==HOME,proof_names[proof_s]);
   }
  }
  int bp_hubs=1;for(int bp_h=1;bp_h<3;bp_h++){game.station_variant=bp_h;sc_build_map();bp_hubs&=sc_room==SC_R_ARRIVALS;bp_hubs&=sc_fill_npcs(SC_R_CANTEEN,bp_npcs,3)==2&&!strcmp(bp_npcs[0].name,"BARTEND");}
  INPUT_CHECK(bp_hubs,"bar preview: secondary hubs reset room and keep ordinary canteen");
  game.system=7;game.station_variant=0;INPUT_CHECK(!bar_preview_at(),"bar preview: no cast leakage to Lave");
  memcpy(&game,bp_saved,sizeof(Game));free(bp_saved);page=bp_page;high_contrast=bp_hc;
  bar_preview_practice=bp_practice;bar_preview_token=bp_token;memcpy(bar_preview_notes,bp_notes,sizeof(bp_notes));sc_built_for=-1;sc_build_map();
 }
}
