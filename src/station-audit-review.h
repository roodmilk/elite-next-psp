/* Opt-in full-galaxy physical-hub identity and production UI regression. */
static int station_audit_review(void){
 FILE *flag=fopen("station-audit.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("station-audit.txt","w");if(!f)return 1;
 int failures=0,classes[3]={0},capital_system=-1;unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));
 if(!pixels){fclose(f);return 1;}for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xcafe2471;fb=pixels+16;
#define ACHECK(c,label) do{int audit_pass=!!(c);fprintf(f,"%s system %d %s\n",audit_pass?"PASS":"FAIL",game.system,label);failures+=!audit_pass;}while(0)
 game_init(&game);story_complete(&game);preview_reset();high_contrast=0;
 for(int sys=0;sys<256;sys++){
  game.system=sys;system_bodies(&game);int kind=station_class_for_system(&game,sys);classes[kind]++;
  StationProfile p=station_profile_for(&game,0);const MegaCity *m=station_architecture_for(&game);
  float lo=1e9f,hi=-1e9f;for(int k=0;k<m->n;k++){lo=fminf(lo,m->b[k].c.x-m->b[k].e.x);hi=fmaxf(hi,m->b[k].c.x+m->b[k].e.x);}
  ACHECK(p.station_class==kind&&m->system==sys&&m->seed==p.seed,"listed class and physical station use the same system identity");
  if(kind==STATION_MEGA){capital_system=sys;ACHECK(m->n>=60&&hi-lo>=16600&&p.radius>=940&&station_port_count_for(&game,0)==5,"Mega Capital is a city over 16.6 km wide with five entrances");}
  else if(kind==STATION_RICH)ACHECK(m->n>=17&&m->n<=60&&p.radius>=460&&p.radius<=620&&station_port_count_for(&game,0)==1,"Rich station has bounded middle-tier hull and one entrance");
  else ACHECK(m->n>3&&m->n<40&&p.radius<400&&station_port_count_for(&game,0)==1,"Poor station retains compact hull and one entrance");
  int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,2);
  ACHECK(n==HUB_COUNT&&ids[0]==0&&ids[1]==STATION_TARGET_ID(1)&&ids[2]==STATION_TARGET_ID(2),"main, relay and outpost have separate scanner entries");
  game.docked=game.dock_stage=game.dead=game.legal=game.police_stop=0;game.planet=-1;game.jump=0;
  for(int hub=1;hub<HUB_COUNT;hub++){
   game.pos=hub_position(&game,hub);game.station_variant=hub;selected_target=0;
   ACHECK(valid_target(0)&&length(sub(target_position(0),hub_position(&game,0)))<.01f,"primary lock stays on the primary even next to a relay and with stale dock context");
   ACHECK(strstr(target_name(0),kind==STATION_MEGA?"Mega Capital":kind==STATION_POOR?"Free Port":"Orbital Citadel"),"main target name agrees with main hull class");
   int id=STATION_TARGET_ID(hub);selected_target=id;
   ACHECK(valid_target(id)&&target_category(id)==2&&length(sub(target_position(id),hub_position(&game,hub)))<.01f&&strstr(target_name(id),hub==1?"Outer Relay":"Frontier Outpost")&&!strstr(target_name(id),"Mega Capital"),"small auxiliary hub cannot masquerade as the capital");
   ACHECK(station_target_hub(id)==hub&&dock_selected_station()&&game.station_variant==hub&&game.dock_stage==2,"selected auxiliary gets its own docking request");game.dock_stage=0;
  }
  selected_target=0;game.pos=hub_position(&game,1);
  ACHECK(!dock_selected_station()&&!game.dock_stage,"out-of-range primary request never silently docks the nearby relay");
 }
 ACHECK(classes[0]>0&&classes[1]>0&&classes[2]==9,"all 256 system classes audited, including every one of nine capitals");
 game_init(&game);game.system=capital_system;system_bodies(&game);story_complete(&game);launch(&game);game.dock_stage=0;game.pos=hub_position(&game,1);game.speed=0;page=FLIGHT;selected_target=STATION_TARGET_ID(1);speech_ok();
 /* The real Almanac X handler must choose MAIN, not whatever is nearest. */
 change_page(DETAILS);row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);
 ACHECK(page==FLIGHT&&selected_target==0&&autoaim&&length(sub(target_position(selected_target),hub_position(&game,0)))<.01f,"Almanac selects actual main capital from beside the small relay");
 scan_cat=2;cycle_scan_item(1);ACHECK(selected_target==STATION_TARGET_ID(1),"station cycling selects relay separately");cycle_scan_item(1);ACHECK(selected_target==STATION_TARGET_ID(2),"station cycling selects outpost separately");cycle_scan_item(1);ACHECK(selected_target==0,"station cycling returns to the same main hub");
 for(int hub=1;hub<HUB_COUNT;hub++){
  page=FLIGHT;game.docked=game.dock_stage=game.police_stop=game.legal=0;game.pos=hub_position(&game,hub);selected_target=STATION_TARGET_ID(hub);speech_ok();hail_target();
  ACHECK(page==COMMS_PANEL&&row==3&&selected_target==STATION_TARGET_ID(hub),"hailing auxiliary opens canonical docking channel for that exact target");
  input(PSP_CTRL_CROSS,0,.016f,0,0);
  ACHECK(page==FLIGHT&&game.station_variant==hub&&game.dock_stage==2&&selected_target==STATION_TARGET_ID(hub),"actual Comms X handler docks the selected auxiliary and retains its identity");
 }
 page=FLIGHT;game.docked=game.dock_stage=0;game.pos=hub_position(&game,1);selected_target=0;speech_ok();hail_target();input(PSP_CTRL_CROSS,0,.016f,0,0);
 ACHECK(page==COMMS_PANEL&&!game.dock_stage&&selected_target==0,"actual main docking request refuses nearby relay substitution");
 /* Maximum contact occupancy proves array sizing and all station readouts. */
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=1;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=1;for(int i=0;i<ANOMALY_COUNT;i++)game.anomaly[i].alive=1;
 game.route_goal=game.destination=(game.system+1)&255;contacts_refresh();target_filter=0;target_refresh();
 ACHECK(contact_count==TARGET_CAPACITY&&target_count==TARGET_CAPACITY,"full contact lists fit the enlarged bounded arrays without dropping hubs");
 target_filter=7;target_refresh();ACHECK(target_count==3,"station-only filter contains exactly three physical hubs");
 for(int hub=0;hub<HUB_COUNT;hub++){int id=STATION_TARGET_ID(hub);ACHECK(target_status(id)[0]&&scanner_known(id)&&contact_color(id)==UI_CYAN,"station details remain classified as stations, not NPCs or anomalies");}
 /* Native production captures: all capitals and the formerly misleading relay. */
 for(int sys=0;sys<256;sys++)if(station_class_for_system(&game,sys)==STATION_MEGA){
  game.system=sys;system_bodies(&game);game.station_variant=1;game.docked=0;game.dock_stage=0;game.dead=0;game.planet=-1;game.time=50;game.voice_time=game.message_time=0;page=FLIGHT;selected_target=0;
  game.pos=(Vec3){-18000,9000,STATION_Z-20000};Vec3 dir=norm(sub((Vec3){0,2200,STATION_Z+4500},game.pos));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=0;
  drawcount=0;rect(0,0,W,H,BG);space();char name[64];snprintf(name,sizeof(name),"audit-main-capital-%03d.bmp",sys);dump_native_bmp(name);
 }
 game.system=capital_system;system_bodies(&game);game.pos=add(hub_position(&game,1),(Vec3){0,100,-1800});game.yaw=0;game.pitch=-.05f;selected_target=STATION_TARGET_ID(1);scan_cat=2;page=FLIGHT;
 drawcount=0;rect(0,0,W,H,BG);space();dump_native_bmp("audit-small-relay-labelled-correctly.bmp");
 page=TARGETING;row=0;targeting_screen();dump_native_bmp("audit-three-station-entries.bmp");
 for(int i=0;i<16;i++)failures+=pixels[i]!=0xcafe2471||pixels[STRIDE*H+16+i]!=0xcafe2471;
 ACHECK(!surface_depth_on&&!surface_material,"flight depth state restored before menu rendering");
 fprintf(f,"INFO %d poor %d rich %d mega systems\nRESULT %d failures\n",classes[0],classes[1],classes[2],failures);fclose(f);fb=saved;free(pixels);return 1;
#undef ACHECK
}
