/* Navigation records are deliberately outside the physical target ID space. */
#define NAV_UI_ID 1000
#define NAV_VIRTUAL_ID 1100
static int nav_virtual_ready(int id){return id==ROUTE_TARGET_ID||id==RUMOUR_TARGET_ID||id==EXPEDITION_TARGET_ID||id==DOCKBAY_TARGET_ID||id==RACE_TARGET_ID;}
static int nav_active_point(Vec3*out){
 if(game.navigation.waypoint_key&&game.navigation.waypoint_system==game.system&&navigation_source(&game,game.navigation.waypoint_key-1)){*out=navigation_position(&game,game.navigation.waypoint_key-1);return 1;}
 if(game.navigation.search_key&&game.navigation.search_system==game.system){*out=navigation_search_position(&game);return 2;}
 if(nav_virtual_ready(nav_virtual_target)&&valid_target(nav_virtual_target)){*out=target_raw_position(nav_virtual_target);return nav_virtual_target==RUMOUR_TARGET_ID?2:3;}
 if(rumour_area_ready()){*out=rumour_area_position();return 2;}return 0;
}
static const char*nav_region_names[6]={"AZURE GAS FIELD","MAGNETIC STORM","AURORA REACH","WRECKAGE WAKE","COMET STREAM","QUIET DUST VEIL"};
static const char*nav_key_name(int key){
 if(key<3)return station_name_for(&game,key);
 if(key<11)return discovery_catalogue[discovery_entry(key-3)].name;
 if(key<15)return rift_names[rift_type(game.system,key-11)];
 if(key<21){static char name[24];snprintf(name,sizeof(name),"NAV BUOY %d",key-14);return name;}
 if(key<27)return nav_region_names[key-21];
 if(key>=33&&key<NAV_KEYS)return game.bodies[key-32].name;
 return "SEARCH AREA";
}
static int nav_trace_key=-1,nav_trace_system=-1,nav_trace_candidate=-1;
static float nav_trace_age,nav_trace_strength;
static Vec3 nav_trace_bearing;
static void nav_passive_tick(float dt){
 if(!navigation_enabled(&game)||game.docked||game.planet>=0||race_is_active(&game)){nav_trace_key=-1;return;}
 if(nav_trace_system!=game.system){nav_trace_key=nav_trace_candidate=-1;nav_trace_age=0;nav_trace_system=game.system;}
 int best=-1;float strength=0;
 for(int k=0;k<21;k++){
  if(k>=11&&k<15&&!game.anomaly[k-11].alive)continue;
  if(k>=3&&k<11&&k-3>=discovery_count())continue;
  float range=length(sub(navigation_position(&game,k),game.pos)),resolve=k<3?sensor_range()*1.2f:sensor_range();
  float reception=resolve*2.2f;
  if(range>resolve*.9f&&range<reception){float q=1-range/reception;if(q>strength){strength=q;best=k;}}
 }
 if(best!=nav_trace_candidate){nav_trace_candidate=best;nav_trace_age=0;}else nav_trace_age+=fmaxf(0,dt);
 if(nav_trace_age>.65f){nav_trace_key=best;nav_trace_strength=strength;if(best>=0)nav_trace_bearing=norm(sub(navigation_position(&game,best),game.pos));}
}
static const char*nav_header_cue(void){
 if(!navigation_enabled(&game))return tracked_hud_cue();
 Vec3 point;int active=nav_active_point(&point);if(active==1)return "NAV: SAVED COORDINATES";if(active==3)return "NAV: PROVIDED COORDINATES";
 if(active==2)return "NAV: PAID SEARCH AREA";
 if(!navigation_source(&game,NAV_STATION(0))&&!game.job_n&&!tracked_mission&&!(game.campaign_stage>=6&&game.saga_step>0))return nav_trace_key>=0?"PASSIVE TRACE / FOLLOW BEARING":"EXPLORE / GALACTICNET LEADS";
 return tracked_hud_cue();
}
static int nav_ui_record(int id){return (id>=NAV_UI_ID&&id<NAV_UI_ID+NAV_KEYS+1)||(id>=NAV_VIRTUAL_ID&&id<=NAV_VIRTUAL_ID+FLIGHT_TARGET_MAX&&nav_virtual_ready(id-NAV_VIRTUAL_ID));}
static Vec3 nav_ui_position(int id){if(id>=NAV_VIRTUAL_ID)return target_raw_position(id-NAV_VIRTUAL_ID);return id==NAV_UI_ID+NAV_KEYS?navigation_search_position(&game):navigation_position(&game,id-NAV_UI_ID);}
static const char*nav_ui_name(int id){if(id>=NAV_VIRTUAL_ID)return target_raw_name(id-NAV_VIRTUAL_ID);return id==NAV_UI_ID+NAV_KEYS?"PAID SEARCH AREA":nav_key_name(id-NAV_UI_ID);}
static void nav_ui_select(int id){
 nav_virtual_target=-1;nav_follow=0;
 if(id>=NAV_VIRTUAL_ID){int key=id-NAV_VIRTUAL_ID;if(nav_virtual_ready(key)&&valid_target(key)){nav_virtual_target=key;game.navigation.waypoint_key=game.navigation.search_key=0;game.cue=SFX_SELECT;}return;}
 if(id==NAV_UI_ID+NAV_KEYS){game.navigation.waypoint_key=0;game.cue=SFX_SELECT;return;}
 if(navigation_select(&game,id-NAV_UI_ID)){game.navigation.search_key=0;game.cue=SFX_SELECT;}
}

static void nav_world_buoys(void){
 if(!navigation_enabled(&game)||race_is_active(&game))return;
 for(int b=0;b<NAV_BUOYS;b++){
  Vec3 c=navigation_position(&game,NAV_BUOY(b));float range=length(sub(c,game.pos));if(range>40000)continue;
  float yaw=.17f*b+game.time*.013f;unsigned hull=RGB(132,154,163),gold=RGB(223,166,73);
  discovery_design_ring(c,110,17,yaw,hull,FLEET_ARMOUR,12);
  discovery_design_box(c,(Vec3){0,0,0},(Vec3){24,52,29},yaw,hull,FLEET_SERVICE,FLEET_ARMOUR,FLEET_WINDOWS,1);
  for(int side=-1;side<=1;side+=2){
   discovery_design_box(c,(Vec3){side*69,0,0},(Vec3){28,9,11},yaw,hull,FLEET_SERVICE,FLEET_ARMOUR,FLEET_ARMOUR,0);
   discovery_design_box(c,(Vec3){side*159,0,0},(Vec3){36,67,4},yaw,RGB(69,117,146),FLEET_SOLAR,FLEET_ARMOUR,FLEET_SOLAR,1);
   discovery_design_box(c,(Vec3){0,side*138,0},(Vec3){9,40,9},yaw,gold,FLEET_ARMOUR,FLEET_SERVICE,FLEET_SERVICE,1);
  }
  encounter_lamp(c,(Vec3){0,179,0},yaw,((int)(game.time*3)+b)%7<2?RGB(251,218,143):RGB(82,165,184));
 }
}
