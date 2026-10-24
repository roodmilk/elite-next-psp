{
 int ship=0;game_init(&game);game.opening.stage=OP_FREE;launch(&game);game.dock_stage=0;page=FLIGHT;game.voice_time=game.message_time=0;selected_target=target_explicit=0;comms_encounter_conversation=night_open=0;comms_return=FLIGHT;
 int layout=1,seen[16]={0};seen[10]=1;
 for(int i=0;i<16;i++)if(i!=5&&i!=9){CommsKey a=comms_keys[i];layout&=a.x>=8&&a.y>=40&&a.x+a.w<=472&&a.y+a.h<=236;
 for(int j=i+1;j<16;j++)if(j!=5&&j!=9){CommsKey b=comms_keys[j];layout&=!(a.x<b.x+b.w&&b.x<a.x+a.w&&a.y<b.y+b.h&&b.y<a.y+a.h);}}
 for(int pass=0;pass<16;pass++)for(int i=0;i<16;i++)if(seen[i]){seen[comms_neighbour(i,-1,0)]=1;seen[comms_neighbour(i,1,0)]=1;seen[comms_neighbour(i,0,-1)]=1;seen[comms_neighbour(i,0,1)]=1;}
 int reachable=1;for(int i=0;i<16;i++)if(i!=5&&i!=9)reachable&=seen[i];
 OPEN_CHECK(layout&&reachable,"console controls fit and all are reachable by neighbouring D-pad navigation");
 game_input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.1f,0,0);for(int i=0;i<6;i++)game_input(0,PSP_CTRL_TRIANGLE,.1f,0,0);
 OPEN_CHECK(page==COMMS_PANEL,"holding Triangle opens the console");
 Vec3 position=game.pos;float clock=game.time;for(int i=0;i<60;i++)game_input(0,0,.1f,0,0);
 OPEN_CHECK(game.time==clock&&length(sub(position,game.pos))==0,"console pauses simulation and ship movement");
 row=1;OPEN_CHECK(comms_unavailable(row)!=0,"contact disabled without a selected ship or station");game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==COMMS_PANEL&&!comms_encounter_conversation,"disabled contact cannot start dialogue");
 row=0;OPEN_CHECK(comms_unavailable(row)!=0,"dismiss disabled with no active message");row=2;OPEN_CHECK(comms_unavailable(row)!=0,"clear disabled without target");
 row=3;game.pos=(Vec3){900000,0,0};OPEN_CHECK(comms_unavailable(row)!=0,"docking unavailable beyond station range");game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(!game.dock_stage&&!game.docked,"unavailable docking cannot teleport or start approach");
 row=0;game.voice_time=game.message_time=0;comms_panel();dump_native_bmp("console-disabled-271.bmp");
 game.message_time=5;strcpy(game.message,"Test transmission.");row=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(game.message_time==0,"dismiss clears actual active message");
 selected_target=STATION_TARGET_ID(0);target_explicit=1;row=2;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(selected_target==0&&!autoaim&&!target_explicit,"clear releases selected target and pursuit");
 hud_mode=hud_hidden=third_person=0;comms_captions=1;row=7;for(int i=0;i<3;i++)game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(hud_mode==0&&!hud_hidden,"HUD selector cycles full minimal hidden");
 row=12;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(third_person==1,"camera selects third person");game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(third_person==2,"camera selects cinematic");game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(third_person==0,"camera returns to first person");
 row=14;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(!comms_captions,"caption rocker toggles optional subtitles");
 comms_quiet=radio_off=quiet_comms=0;row=15;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(comms_quiet&&radio_off&&quiet_comms,"quiet mode mutes music and ambient chatter together");
 speak(&game,VOICE_KEI,"Essential mission instruction.");OPEN_CHECK(speech_active(),"essential mission speech remains available in quiet mode");
 third_person=2;OPEN_CHECK(comms_settings_save("test-console.cfg"),"console preferences save");hud_mode=2;third_person=1;comms_captions=1;comms_quiet=0;OPEN_CHECK(comms_settings_load("test-console.cfg")&&hud_mode==0&&third_person==2&&!comms_captions&&comms_quiet,"display, cinematic camera and quiet restoration snapshot persist");
 row=15;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(!comms_quiet&&!radio_off&&!quiet_comms,"quiet mode restores previous settings");
 row=13;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(radio_off,"music power rocker works");row=8;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(quiet_comms,"ambient chatter rocker works");
 row=10;int station=radio_station;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(radio_station==(station+1)%RADIO_STATION_COUNT,"tuner changes station through the shared radio action");
 game_input(PSP_CTRL_LTRIGGER,0,.016f,0,0);OPEN_CHECK(radio_station==station,"left shoulder tunes previous station");game_input(PSP_CTRL_RTRIGGER,0,.016f,0,0);OPEN_CHECK(radio_station==(station+1)%RADIO_STATION_COUNT,"right shoulder tunes next station");
 int restore=1;for(int m=0;m<2;m++)for(int c=0;c<2;c++){comms_quiet=0;radio_off=m;quiet_comms=c;comms_toggle_quiet();comms_toggle_quiet();restore&=radio_off==m&&quiet_comms==c;}OPEN_CHECK(restore,"Quiet Mode restores every previous music/chatter combination");
 game.encounter_kind=ENCOUNTER_NONE;game.npc[0].alive=1;game.npc[0].role=EXPLORERS;game.npc[0].freighter=0;game.npc[0].traveller=-1;game.npc[0].pos=add(game.pos,(Vec3){0,0,500});selected_target=NPC_ID_MIN;game.message_time=game.voice_time=0;page=FLIGHT;quiet_comms=0;
 game_input(PSP_CTRL_TRIANGLE,0,.016f,0,0);char hailed[sizeof(game.voice)];snprintf(hailed,sizeof(hailed),"%s",game.voice);change_page(COMMS_PANEL);row=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(comms_encounter_conversation&&hailed[0]&&!strcmp(hailed,game.voice),"panel contact and Triangle tap use the same outgoing hail");comms_encounter_conversation=0;
 game.message_time=0;game.encounter_kind=ENCOUNTER_PIRATE;game.encounter=4;encounter_speak(&game,VOICE_CONTACT,"Incoming actionable call.");game.voice_time=10;row=1;OPEN_CHECK(comms_incoming(),"actionable incoming call lights Answer Incoming");comms_panel();dump_native_bmp("console-incoming-271.bmp");game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(comms_encounter_conversation,"Answer Incoming opens the shared conversation");
 comms_encounter_conversation=0;game.encounter_kind=ENCOUNTER_NONE;game.voice_time=game.message_time=0;selected_target=STATION_TARGET_ID(0);target_explicit=1;game.pos=(Vec3){0,0,2600};row=3;comms_panel();dump_native_bmp("console-station-271.bmp");game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==FLIGHT&&(game.dock_stage||game.police_stop),"available docking uses guided docking action");
 game.dock_stage=0;change_page(COMMS_PANEL);row=4;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==MISSIONLOG,"Mission Log button opens the log directly");
 game.police_stop=0;change_page(COMMS_PANEL);game.anomaly[0].alive=1;game.anomaly[0].pos=add(game.pos,(Vec3){0,0,100});selected_target=ANOMALY_ID_MIN;row=6;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==WALK&&walk_kind==2&&walk_oxygen==100,"available salvage EVA opens the existing spacewalk");
 change_page(COMMS_PANEL);row=11;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==HELP,"Controls key opens existing help");
 change_page(COMMS_PANEL);hud_mode=1;third_person=1;row=7;comms_panel();dump_native_bmp("console-selectors-271.bmp");
 change_page(COMMS_PANEL);comms_return=FLIGHT;game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);OPEN_CHECK(page==FLIGHT,"Circle resumes flight");
 clock=game.time;game_input(0,0,.1f,0,0);OPEN_CHECK(game.time>clock,"simulation resumes after leaving console");
 change_page(HOME);change_page(COMMS_PANEL);game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);OPEN_CHECK(page==HOME,"console opened from command deck returns to command deck");change_page(FLIGHT);comms_return=FLIGHT;
 game.message_time=0;game.encounter_kind=ENCOUNTER_PIRATE;game.encounter=4;encounter_speak(&game,VOICE_CONTACT,"Quick incoming call.");game_input(PSP_CTRL_TRIANGLE,PSP_CTRL_TRIANGLE,.05f,0,0);game_input(0,0,.05f,0,0);OPEN_CHECK(page==FLIGHT&&comms_quick,"Triangle tap retains quick reply overlay");comms_quick_choice=1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==COMMS_PANEL&&comms_encounter_conversation,"quick reply opens the same paused conversation");comms_encounter_conversation=comms_quick=0;
 for(int i=0;i<3;i++){const char *paths[]={"test-console.cfg","test-console.cfg.bak","test-console.cfg.tmp"};remove(paths[i]);}comms_quiet=quiet_comms=radio_off=0;comms_captions=1;hud_mode=hud_hidden=third_person=0;radio_dirty=0;comms_encounter_conversation=0;
}
