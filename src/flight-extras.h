#include "game.h"
#include "travellers-data.h"
#include "space-discoveries.h"
/* Flight presentation and target selection. Target IDs: station, bodies, NPCs, debris. */
static int selected_target=0,look_target=-1,autoaim=0,scan_cat=2,square_held=0,police_choice=0;
static int nav_active_point(Vec3*out);
static int nav_route_selected(void){return selected_target==ROUTE_TARGET_ID||nav_virtual_target==ROUTE_TARGET_ID;}
static int nav_route_tracking(void){return (selected_target==ROUTE_TARGET_ID&&autoaim)||(nav_virtual_target==ROUTE_TARGET_ID&&nav_follow);}
static int target_explicit=0; /* Primary hub ID zero can also be an unselected fallback. */
/* dialogue-ui.h is included later by main.c; keep the arrest renderer on the
 * same standard conversation primitives without changing include order. */
static void dialogue_begin(const char *title);
static void dialogue_speech(const char *speaker,int role,int seed,const char *a,const char *b);
static void dialogue_context(const char *label,const char *body);
static void dialogue_reply(int index,const char *label);
static float r_tap=10,l_tap=10,hard_brake=0,hard_brake_age=0;
static const char *scan_cat_names[]={"PLANETS","SHIPS","STATIONS","OTHER","FOE","NAV LOG"};
/* Each physical hub has a stable target ID; zero always means PRIMARY. */
static int route_target_ready(void){return game.route_goal>=0&&game.destination>=0&&game.destination<256&&game.destination!=game.system;}
static Vec3 route_target_direction(void){
 int dest=game.destination,dx=game.systems[dest].x-game.systems[game.system].x,dz=game.systems[dest].y-game.systems[game.system].y;
 unsigned h=(unsigned)(game.system+1)*2246822519u^(unsigned)(dest+1)*3266489917u;Vec3 d={(float)dx,(float)((int)((h>>27)&15)-7)*.35f,(float)-dz};if(length(d)<1)d=(Vec3){0,0,1};return norm(d);
}
/* ENEMIES = ships currently going after the player. SHIPS lists every alive contact. */
static int npc_is_hostile(const NPC *n){return n&&n->target==-2;}
static int target_category(int id){
 if(id==PLAYER_TARGET_ID)return 1;
 if(id==ROUTE_TARGET_ID)return 3;
 if(IS_STATION_ID(id))return 2;
 if(id>0&&id<=BODY_COUNT)return 0;
 if(IS_NPC_ID(id))return npc_is_hostile(&game.npc[id-BODY_COUNT-1])?4:1;
 return 3;
}
static const char *tracked_hud_cue(void);
static int target_detected(int id);
static int scanner_known(int id);
static int rumour_area_ready(void);static Vec3 rumour_area_position(void);
static Vec3 target_raw_position(int id){if(IS_BUOY_ID(id))return navigation_position(&game,NAV_BUOY(id-BUOY_ID_MIN));if(id==DOCKBAY_TARGET_ID)return dockbay_point(&game,game.dockbay.route-1);if(id==EXPEDITION_TARGET_ID)return expedition_waypoint(&game);if(id==RACE_TARGET_ID)return race_waypoint(&game);if(id==RUMOUR_TARGET_ID)return rumour_area_position();if(id==WHALE_TARGET_ID)return whale_position(&game,0);if(IS_DISCOVERY_ID(id))return discovery_position(id-DISCOVERY_ID_MIN);if(id==PLAYER_TARGET_ID)return wifi_contact_position();if(id==ROUTE_TARGET_ID)return add(game.pos,mul(route_target_direction(),180000.f));if(IS_STATION_ID(id))return hub_position(&game,station_target_hub(id));if(id<=BODY_COUNT)return game.bodies[id-1].pos;if(IS_NPC_ID(id))return game.npc[id-BODY_COUNT-1].pos;if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].pos;return game.anomaly[id-ANOMALY_ID_MIN].pos;}
static const char *target_raw_name(int id){if(IS_BUOY_ID(id)){static char buoy[28];snprintf(buoy,sizeof(buoy),"LOCAL NAV BUOY %d",id-BUOY_ID_MIN+1);return buoy;}if(id==DOCKBAY_TARGET_ID)return "DOCKBAY MEETING POINT";if(IS_NPC_ID(id)){int ad=dockbay_actor(&game,id-NPC_ID_MIN);if(ad>=0)return dockbay_seller_name(game.dockbay.ad[ad].seller);}if(id==EXPEDITION_TARGET_ID)return "MISSION WAYPOINT";if(IS_NPC_ID(id)&&expedition_npc(&game,id-NPC_ID_MIN)>=0)return expedition_npc_name(&game,id-NPC_ID_MIN);if(id==RACE_TARGET_ID){static char label[32];if(game.race.phase==RACE_RUNNING)snprintf(label,sizeof(label),"RACE CHECKPOINT %d/%d",game.race.gate+1,RACE_GATES);else snprintf(label,sizeof(label),"RACE START GRID");return label;}if(id==RUMOUR_TARGET_ID)return "RUMOUR SEARCH AREA";if(id==WHALE_TARGET_ID)return "SPACE WHALE POD";if(IS_DISCOVERY_ID(id))return discovery_data(id)->name;if(id==PLAYER_TARGET_ID)return wifi_contact_name();if(id==ROUTE_TARGET_ID){static char route[32];snprintf(route,sizeof(route),"ROUTE: %.11s",game.systems[game.destination].name);return route;}if(IS_STATION_ID(id))return station_name_for(&game,station_target_hub(id));if(id<=BODY_COUNT)return game.bodies[id-1].name;static char label[32];if(IS_DEBRIS_ID(id)){const char *mission_label=expedition_pod_name(&game,id-DEBRIS_ID_MIN);if(mission_label)return mission_label;Debris *d=&game.debris[id-DEBRIS_ID_MIN];if(d->rock)snprintf(label,sizeof(label),d->rock==2?"ICE ASTEROID":"MINERAL ASTEROID");else if(d->wreck)snprintf(label,sizeof(label),"WRECKAGE #%02d",id-DEBRIS_ID_MIN+1);else snprintf(label,sizeof(label),"%s POD",goods[d->good>=0&&d->good<GOODS?d->good:12].name);return label;}if(IS_ANOMALY_ID(id)){Anomaly *a=&game.anomaly[id-ANOMALY_ID_MIN];snprintf(label,sizeof(label),"%s",rift_names[rift_type(game.system,id-ANOMALY_ID_MIN)]);return label;}NPC *n=&game.npc[id-BODY_COUNT-1];if(n->bounty_slot>=0){bounty_target_label(&game,n->bounty_slot,label,sizeof(label));return label;}if(n->traveller>=0)return traveller_name(n->traveller);int serial=(id*17+game.system*7)&255;if(n->role==LAW)snprintf(label,sizeof(label),"ENCRYPTED // %02X",serial);else if(n->role==PIRATES)snprintf(label,sizeof(label),"RAIDER %c-%02d",'A'+(serial%26),serial%100);else if(n->role==EXPLORERS)snprintf(label,sizeof(label),"GUILD %c-%02d",'A'+(serial%26),serial%100);else if(n->freighter)snprintf(label,sizeof(label),"CAPITAL MERCHANT %02d",serial%100);else snprintf(label,sizeof(label),"MERCHANT %c-%02d",'A'+(serial%26),serial%100);return label;}
static int valid_target(int id){if(IS_BUOY_ID(id))return navigation_enabled(&game);if(id==DOCKBAY_TARGET_ID)return dockbay_ready(&game);if(id==EXPEDITION_TARGET_ID)return expedition_ready(&game);if(id==RACE_TARGET_ID)return race_ready(&game);if(id==RUMOUR_TARGET_ID)return rumour_area_ready();if(id==WHALE_TARGET_ID)return !opening_active(&game)&&system_whales(game.system);if(IS_DISCOVERY_ID(id))return id-DISCOVERY_ID_MIN<discovery_count();if(id==PLAYER_TARGET_ID)return wifi_contact_ready();if(IS_STATION_ID(id))return 1;if(id==ROUTE_TARGET_ID)return route_target_ready();if(id<0||id>ANOMALY_ID_MAX)return 0;if(id<=BODY_COUNT)return id>0&&body_active(&game.bodies[id-1]);if(IS_NPC_ID(id))return game.npc[id-BODY_COUNT-1].alive;if(IS_DEBRIS_ID(id))return game.debris[id-DEBRIS_ID_MIN].alive;return game.anomaly[id-ANOMALY_ID_MIN].alive;}
static uint8_t discovery_identified[256],debris_identified[DEBRIS_COUNT];
static int debris_identity_system=-1;
static float debris_identity_life[DEBRIS_COUNT];
static uint8_t anomaly_identified[256];
static int identity_slot=-99;
static unsigned identity_checksum(const uint8_t *data,int count){unsigned h=2166136261u;for(int i=0;i<count;i++)h=(h^data[i])*16777619u;return h;}
static void identity_sync(void){
 if(identity_slot==commander_active_slot)return;identity_slot=commander_active_slot;
 memset(discovery_identified,0,sizeof(discovery_identified));memset(anomaly_identified,0,sizeof(anomaly_identified));
 char path[64];snprintf(path,sizeof(path),"contact-identities-%d.cfg",identity_slot);FILE *f=fopen(path,"rb");if(!f)return;
 uint32_t head[2];uint8_t data[512];int ok=fread(head,1,sizeof(head),f)==sizeof(head)&&fread(data,1,sizeof(data),f)==sizeof(data);fclose(f);
 if(ok&&head[0]==0x49444531u&&head[1]==identity_checksum(data,512)){memcpy(discovery_identified,data,256);memcpy(anomaly_identified,data+256,256);}
}
static void identity_save(void){
 uint8_t data[512];memcpy(data,discovery_identified,256);memcpy(data+256,anomaly_identified,256);
 uint32_t head[2]={0x49444531u,identity_checksum(data,512)};char path[64],tmp[68];snprintf(path,sizeof(path),"contact-identities-%d.cfg",identity_slot);snprintf(tmp,sizeof(tmp),"%s.tmp",path);
 FILE *f=fopen(tmp,"wb");if(!f)return;int ok=fwrite(head,1,sizeof(head),f)==sizeof(head)&&fwrite(data,1,sizeof(data),f)==sizeof(data);if(fflush(f))ok=0;if(fclose(f))ok=0;if(ok){remove(path);rename(tmp,path);}
}
static void debris_identity_refresh(void){
 if(debris_identity_system!=game.system){memset(debris_identified,0,sizeof(debris_identified));memset(debris_identity_life,0,sizeof(debris_identity_life));debris_identity_system=game.system;}
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||d->life>debris_identity_life[i]+1)debris_identified[i]=0;debris_identity_life[i]=d->life;}
}
static int sensor_last_id=-1,sensor_last_system=-1;static Vec3 sensor_last_position;
static float sensor_hud_range=-1;
static float sensor_range(void){return sensor_hud_range>=0?sensor_hud_range:module_scan_range(&game)*4.f*tech_scan_multiplier(&game);}
#include "rumours.h"
static int target_detected(int id){
 if(!valid_target(id))return 0;
 if(navigation_enabled(&game)&&(id==RUMOUR_TARGET_ID||id==ROUTE_TARGET_ID||id==EXPEDITION_TARGET_ID||id==DOCKBAY_TARGET_ID||id==RACE_TARGET_ID))return 0;
 if(id==EXPEDITION_TARGET_ID||id==DOCKBAY_TARGET_ID)return 1;
 if((id>0&&id<=BODY_COUNT)||id==ROUTE_TARGET_ID||id==RUMOUR_TARGET_ID||id==RACE_TARGET_ID)return 1;
 if(!navigation_enabled(&game)&&IS_STATION_ID(id)&&is_mission_target(&game,id))return 1;
 if(id==0&&!navigation_enabled(&game)){const char *cue=tracked_hud_cue();if(!strncmp(cue,"DOCK:",5)||!strncmp(cue,"LOCK:",5))return 1;}
 float range=sensor_range();if(IS_STATION_ID(id))range*=navigation_enabled(&game)?1.2f:2.8f;
 if(IS_NPC_ID(id)&&game.npc[id-NPC_ID_MIN].freighter)range*=2.f;
 return length(sub(target_raw_position(id),game.pos))<=range;
}
static Vec3 target_position(int id){
 if(sensor_last_system!=game.system){sensor_last_system=game.system;sensor_last_id=-1;}
 Vec3 p=target_raw_position(id);
 if(id==selected_target){if(target_detected(id)){sensor_last_id=id;sensor_last_position=p;}else if(sensor_last_id==id)return sensor_last_position;}
 return p;
}
static int scanner_known(int id){
 identity_sync();
 if(IS_BUOY_ID(id))return length(sub(target_raw_position(id),game.pos))<1800||(navigation_source(&game,NAV_BUOY(id-BUOY_ID_MIN))&NAV_VISITED);
 if(id==WHALE_TARGET_ID)return rumour_found(12);
 if(IS_NPC_ID(id))return game.npc[id-NPC_ID_MIN].name_known;
 if(IS_DISCOVERY_ID(id)&&navigation_enabled(&game)&&(navigation_source(&game,NAV_DISCOVERY(id-DISCOVERY_ID_MIN))&NAV_VISITED))return 1;
 if(IS_DISCOVERY_ID(id))return (discovery_identified[game.system]>>(id-DISCOVERY_ID_MIN))&1;
 if(IS_DEBRIS_ID(id)){debris_identity_refresh();return debris_identified[id-DEBRIS_ID_MIN];}
 if(IS_ANOMALY_ID(id))return ((anomaly_identified[game.system]|game.rift_logged[game.system])>>(id-ANOMALY_ID_MIN))&1;
 return 1;
}
static const char *target_name(int id){return scanner_known(id)?target_raw_name(id):"UNKNOWN";}
static int nearest_hostile_target(void){int best=-1;float range=1e9f;for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&npc_is_hostile(&game.npc[i])){float d=length(sub(game.npc[i].pos,game.pos));if(d<range){range=d;best=BODY_COUNT+1+i;}}return best;}
static void pick_look_target(void);
static int occluded(Vec3 pos);
static int collect_scan_ids(int *ids,int cat){
 int n=0;
 if(cat==1&&wifi_contact_ready())ids[n++]=PLAYER_TARGET_ID;
 if(cat==0){for(int i=0;i<BODY_COUNT;i++)if(body_active(&game.bodies[i])&&game.bodies[i].type!=SUN)ids[n++]=i+1;}
 else if(cat==1){for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive)ids[n++]=BODY_COUNT+1+i;} /* all ships, hostiles included */
 else if(cat==2){for(int hub=0;hub<HUB_COUNT;hub++)ids[n++]=STATION_TARGET_ID(hub);}
 else if(cat==4){for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&npc_is_hostile(&game.npc[i]))ids[n++]=BODY_COUNT+1+i;} /* engaging player only */
 else {
  if(navigation_enabled(&game))for(int b=0;b<NAV_BUOYS;b++)ids[n++]=BUOY_ID_MIN+b;
  if(dockbay_ready(&game))ids[n++]=DOCKBAY_TARGET_ID;
  if(expedition_ready(&game))ids[n++]=EXPEDITION_TARGET_ID;
  if(race_ready(&game))ids[n++]=RACE_TARGET_ID;
  if(rumour_area_ready())ids[n++]=RUMOUR_TARGET_ID;
  if(!opening_active(&game)&&system_whales(game.system))ids[n++]=WHALE_TARGET_ID;
  for(int i=0;i<discovery_count();i++)ids[n++]=DISCOVERY_ID_MIN+i;
  if(route_target_ready())ids[n++]=ROUTE_TARGET_ID;
  for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive)ids[n++]=ANOMALY_ID_MIN+i;
  for(int i=0;i<DEBRIS_COUNT;i++)if(game.debris[i].alive)ids[n++]=DEBRIS_ID_MIN+i;
 }
 int kept=0;for(int i=0;i<n;i++)if(target_detected(ids[i]))ids[kept++]=ids[i];return kept;
}
static void ensure_scan_cat_for_target(int id){
 if(!valid_target(id))return;
 int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,scan_cat);
 for(int i=0;i<n;i++)if(ids[i]==id)return;
 scan_cat=target_category(id);
}
static void lock_local_target(int id,const char *kind){
 nav_follow=0;selected_target=id;target_explicit=1;

 /* Stay on SHIPS when browsing all traffic — hostiles live there too. */
 if(!(IS_NPC_ID(id)&&(scan_cat==1||scan_cat==4)))scan_cat=target_category(id);
 else ensure_scan_cat_for_target(id);
 nav_body=id>0&&id<=BODY_COUNT?id-1:-1;autoaim=0;
 (void)kind;
 story_event(&game,STORY_EV_TARGET);
}
static void step_scan_cat(int dir){
 /* Always visit every band, including empty ENEMIES, so Square+Left/Right is predictable. */
 scan_cat=(scan_cat+dir+5)%5;
}
static void tab_flight_category(int dir){
 step_scan_cat(dir);
 int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,scan_cat);
 if(!n)return; /* Empty targeting categories stay silent. */
 int best=0;float br=1e9f;
 for(int i=0;i<n;i++){float d=length(sub(target_position(ids[i]),game.pos));if(d<br){br=d;best=i;}}
 static const char *lab[]={"Planet","Ship","Station","Other","Enemy"};
 lock_local_target(ids[best],lab[scan_cat]);
}
static void cycle_scan_item(int dir){
 int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,scan_cat);
 if(!n)return; /* Empty targeting categories stay silent. */
 int cur=-1;for(int i=0;i<n;i++)if(ids[i]==selected_target)cur=i;
 if(cur<0)cur=dir>0?-1:0;
 static const char *lab[]={"Planet","Ship","Station","Other","Enemy"};
 lock_local_target(ids[(cur+dir+n)%n],lab[scan_cat]);
}
static void cycle_front_target(void){
 int ids[TARGET_CAPACITY],n=0,cur=-1;
 int top=view_top()+3,bot=view_bot()-3;
 for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(target_detected(id)){
  Vec3 world=target_position(id),v=camera(&game,world);if(v.z<20)continue;
  Point p=project(v);if(p.x<8||p.x>W-8||p.y<top||p.y>bot)continue;
  if(id>BODY_COUNT&&occluded(world))continue;
  ids[n]=id;if(id==selected_target)cur=n;n++;
 }
 if(!n){message(&game,"No contacts in front of the ship.");return;}
 int next=cur<0?0:(cur+1)%n;lock_local_target(ids[next],"In view");
}
static int target_nearest_reticle(void){
 int best=-1,cy=(view_top()+view_bot())/2;float best_score=1e30f;
 for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(target_detected(id)){
  Vec3 world=target_position(id),v=camera(&game,world);if(v.z<20)continue;
  Point p=project(v);if(p.x<8||p.x>W-8||p.y<view_top()+3||p.y>view_bot()-3)continue;
  if(id>BODY_COUNT&&occluded(world))continue;
  float dx=p.x-W*.5f,dy=p.y-cy,score=dx*dx+dy*dy+v.z*.0001f;
  if(score<best_score){best_score=score;best=id;}
 }
 if(best<0)return 0;
 lock_local_target(best,"Reticle");return 1;
}
static void tractor_beam_effect(void){
 if(game.tractor_time<=0||!IS_DEBRIS_ID(game.tractor_target))return;
 int i=game.tractor_target-DEBRIS_ID_MIN;if(i<0||i>=DEBRIS_COUNT||!game.debris[i].alive)return;
 Vec3 v=camera(&game,game.debris[i].pos);if(v.z<20)return;Point p=project(v);int cx=240,cy=view_bot()-18;
 float phase=game.time*18.f;for(int k=0;k<4;k++){int wob=(int)(sinf(phase+k*1.7f)*5);line(cx+k*2-3,cy,p.x+wob+k*2,p.y,CYAN);}
 circle((int)p.x,(int)p.y,8+(int)(sinf(phase)*2),CYAN);circle((int)p.x,(int)p.y,3,GOLD);
}
static int flight_target_combo(unsigned pressed,unsigned held){
 if(game.planet>=0||game.approach>=0||game.jump>0)return 0;
 if(!((held|pressed)&PSP_CTRL_SQUARE))return 0;
 unsigned lr=PSP_CTRL_LEFT|PSP_CTRL_RIGHT,ud=PSP_CTRL_UP|PSP_CTRL_DOWN;
 unsigned dir=0;
 if((pressed&PSP_CTRL_SQUARE)&&(held&(lr|ud)))dir=held&(lr|ud);
 if((held&PSP_CTRL_SQUARE)&&(pressed&(lr|ud)))dir=pressed&(lr|ud);
 if(!dir)return 0;
 if(dir&lr)tab_flight_category((dir&PSP_CTRL_LEFT)?-1:1);
 else cycle_scan_item((dir&PSP_CTRL_UP)?-1:1);
 return 1;
}
/* Reply choices belong only to a currently audible human transmission.
 * A stale encounter flag must never turn a computer notice into dialogue. */
static int incoming_reply_ready(void){return game.message_time<=0&&game.voice_time>0&&game.voice[0]&&game.voice_who!=VOICE_COMP&&encounter_requires_reply(&game);}
static int night_ready(void);
static int docking_target_hub(void){return game.docked?game.station_variant:IS_STATION_ID(selected_target)?station_target_hub(selected_target):nearest_hub(&game);}
static int dock_selected_station(void){return dock_hub(&game,docking_target_hub());}
static int speech_active(void){
 if(game.voice_time>0&&game.voice[0]&&(!quiet_comms||game.voice_who==VOICE_KEI||game.voice_who==VOICE_VENN||game.voice_who==VOICE_LAW||game.attacked>0||encounter_requires_reply(&game))){
  if(game.voice_who==VOICE_COMP)return 0;
  if(game.voice_who==VOICE_LAW&&!game.police_stop&&game.legal<=0&&game.attacked<=0)return 0;
  if(game.voice_who==VOICE_CONTACT&&game.attacked<=0&&game.encounter_kind!=ENCOUNTER_NONE&&!encounter_requires_reply(&game))return 0;
  return 1;
 }
 if(game.message_time>0&&!game.dead&&game.message[0]&&strncmp(game.message,"WELCOME",7)&&strncmp(game.message,"TRIANGLE",8))return 1;
 return 0;
}
static void speech_ok(void){game.voice_time=0;game.voice[0]=0;game.message_time=0;game.cue=SFX_UI;}
static void contact_speak(int role,const char *line){speak(&game,VOICE_CONTACT,line);game.voice_role=role;game.voice_seed=game.system*NPC_COUNT+selected_target;if(game.attacked>0||role==PIRATES)game.cue=SFX_TALK;}
static void hail_target(void){
 if(IS_BUOY_ID(selected_target)){message(&game,"Nav buoy: hold below 120 m/s within 700 m for two seconds.");return;}
 pick_look_target();
 int id=valid_target(selected_target)?selected_target:look_target;
 if(id==DOCKBAY_TARGET_ID){if(game.dockbay_npc>=0&&db_open_contact(game.dockbay_npc))return;message(&game,"Fly to the meeting point, then hail the seller within 600 m.");return;}
 if(id==EXPEDITION_TARGET_ID){ExpeditionRuntime *r=&game.expedition_runtime;const ExpeditionStage *s=expedition_stage(&game,game.job_sel);if(s&&(s->kind==XP_HAIL||s->kind==XP_PATROL)){for(int i=0;i<6;i++)if(r->npc[i]>=0&&game.npc[r->npc[i]].alive&&length(sub(game.npc[r->npc[i]].pos,game.pos))<600){expedition_hail(&game,NPC_ID_MIN+r->npc[i]);return;}}message(&game,expedition_objective(&game,game.job_sel));return;}
 if(id==RACE_TARGET_ID){speak(&game,VOICE_COMP,race_is_active(&game)?"Fly the rings in order. Green rings cool your engines and give you a free boost!":"Race Control has your entry. Fly within 450 metres of the checkerboard start grid to line up.");return;}
 if(id==RUMOUR_TARGET_ID){message(&game,"These are directions. Search the area, then hail a nearby contact.");return;}
 if(id==WHALE_TARGET_ID){if(!target_detected(id)||length(sub(whale_position(&game,0),game.pos))>2400){message(&game,"Move within 2,400 m to observe the whale pod.");return;}
  unsigned bit=1u<<12;if(!(game.rumours.found[game.system]&bit)){game.rumours.found[game.system]|=bit;game.discoveries++;}
  expedition_event(&game,XP_WHALE,id,0,1);speak(&game,VOICE_COMP,"Space whale pod recorded. Easy on the engines; give them room to drift.");return;}
 if(IS_DISCOVERY_ID(id)){if(length(sub(target_position(id),game.pos))>12000){message(&game,"Discovery outside 12,000 m contact range.");return;}expedition_event(&game,XP_DISCOVERY,id,0,1);identity_sync();discovery_identified[game.system]|=1u<<(id-DISCOVERY_ID_MIN);identity_save();navigation_visit(&game,NAV_DISCOVERY(id-DISCOVERY_ID_MIN));speak(&game,VOICE_CONTACT,discovery_data(id)->reply);game.voice_role=TRADERS;game.voice_seed=-1000-discovery_entry(id-DISCOVERY_ID_MIN)-75*(game.system*DISCOVERY_COUNT+id-DISCOVERY_ID_MIN);return;}
 if(id==PLAYER_TARGET_ID){message(&game,wifi_contact_status());return;}
 if(!valid_target(id)){message(&game,"No one on comms. Lock a ship, then Triangle.");return;}
 if(IS_NPC_ID(id)){
  if(!target_detected(id)||length(sub(target_raw_position(id),game.pos))>module_scan_range(&game)){message(&game,"Move closer to hail this contact.");return;}
  game.npc[id-NPC_ID_MIN].name_known=1;
  if(db_open_contact(id-NPC_ID_MIN))return;
  if(mission_interact(&game,id))return;
  NPC *n=&game.npc[id-BODY_COUNT-1];
  selected_target=id;scan_cat=target_category(id);autoaim=0;story_event(&game,STORY_EV_TARGET);
  if(n->traveller>=0){contact_speak(n->role,traveller_hail(&game,n->traveller));traveller_note_hail(&game,n->traveller);return;}
  if(n->role==PIRATES){speak(&game,VOICE_COMP,"No reply. They're painting us.");game.cue=SFX_TALK;}
  else if(n->role==LAW)contact_speak(LAW,game.legal?"Stop. Pay the fine or take custody.":"Clear. Keep the lane clean.");
  else if(n->role==TRADERS){if(n->freighter){char cargo[80];snprintf(cargo,sizeof(cargo),"%s %s. %d%c %s. %s.",n->freight_state<=FREIGHT_INBOUND?"From":"To",game.systems[n->freight_peer].name,n->freight_qty,goods[n->freight_good].unit,goods[n->freight_good].name,freight_status(n));contact_speak(TRADERS,cargo);}else if(trader_offer_hail(&game,id-BODY_COUNT-1)){game.voice_role=TRADERS;game.voice_seed=game.system*NPC_COUNT+selected_target;}}
  else contact_speak(EXPLORERS,"Survey channel. We are mapping this sky.");
  return;
 }
 if(IS_STATION_ID(id)){selected_target=id;target_explicit=1;scan_cat=2;open_station_channel();return;}
 if(id>0&&id<=BODY_COUNT){
  Body *b=&game.bodies[id-1];
  if(b->type==SUN){message(&game,"A star has no landing approach.");return;}
  if(id==1){message(&game,"This rogue giant has no safe landing approach.");return;}
  if(approach_planet(&game,id-1)){selected_target=id;scan_cat=0;autoaim=0;}return;
 }
 if(IS_ANOMALY_ID(id)){if(analysis_scan(&game,id)){identity_sync();anomaly_identified[game.system]|=1u<<(id-ANOMALY_ID_MIN);identity_save();}return;}
 if(IS_DEBRIS_ID(id)){float inspect=1800.f*tech_identification_multiplier(&game);if(length(sub(target_raw_position(id),game.pos))>inspect){message(&game,"Move closer to inspect the salvage contact.");return;}scanner_known(id);debris_identified[id-DEBRIS_ID_MIN]=1;autoaim=0;speak(&game,VOICE_COMP,target_raw_name(id));if(!game.debris[id-DEBRIS_ID_MIN].rock)salvage(&game,id);return;}
}
static int occluded(Vec3 pos){
 if(mega_depth_valid&&clipy0<0&&!(pos.x==0&&pos.y==0&&pos.z==STATION_Z)){Vec3 v=camera(&game,pos);if(v.z>15){Point p=project(v);int x=(int)p.x,y=(int)p.y;if(x>=0&&x<W&&y>=view_top()&&y<=view_bot()&&mega_encode_depth(v.z)>mega_occlusion[(y/2)*240+x/2]+4u)return 1;}}
 Vec3 delta=sub(pos,game.pos);float distance2=dot(delta,delta);if(distance2<.000001f)return 0;float inverse=1.f/distance2;
 for(int i=0;i<BODY_COUNT;i++){const Body *b=&game.bodies[i];if(!body_active(b))continue;Vec3 d=sub(b->pos,game.pos);float along=dot(d,delta);if(along<=0||along>=distance2)continue;Vec3 offset=sub(d,mul(delta,along*inverse));if(dot(offset,offset)<b->radius*b->radius)return 1;}
 return 0;
}
static int station_circle_ready(void){
 if(game.docked||game.dead||game.dock_stage||game.jump>0||game.planet>=0)return 0;
 int hub=IS_STATION_ID(selected_target)?station_target_hub(selected_target):nearest_hub(&game);Vec3 world=hub_position(&game,hub);
 Vec3 p=camera(&game,world);float range=station_comms_range(&game,hub);
 return p.z>1&&sqrtf(p.x*p.x+p.y*p.y)<=fmaxf(160.f,p.z*.10f)&&length(sub(world,game.pos))<=range;
}
static void pick_look_target(void){look_target=-1;float best=1e9f;for(int id=0;id<=FLIGHT_TARGET_MAX;id++){if(!target_detected(id))continue;Vec3 world=target_position(id),p=camera(&game,world);if(p.z<1)continue;float radius=id==ROUTE_TARGET_ID?2200:id>0&&id<=BODY_COUNT?game.bodies[id-1].radius:IS_NPC_ID(id)?game.npc[id-BODY_COUNT-1].radius:IS_DEBRIS_ID(id)?game.debris[id-DEBRIS_ID_MIN].radius:IS_ANOMALY_ID(id)?40:160;float lateral=sqrtf(p.x*p.x+p.y*p.y);if(lateral>fmaxf(radius,p.z*.08f))continue;float distance=length(p)-radius;if(distance<best&&(id>0&&id<=BODY_COUNT?1:!occluded(world))){best=distance;look_target=id;}}}
static void align_target(float dt,float ax,float ay){
 Vec3 point;
 if(nav_follow){if(!nav_active_point(&point)||fabsf(ax)>.1f||fabsf(ay)>.1f){nav_follow=0;return;}}
 else{
  if(!autoaim)return;
  if(!target_detected(selected_target)){autoaim=0;return;}
  if(selected_target==PLAYER_TARGET_ID)wifi_contact_route();
  if(!valid_target(selected_target)||fabsf(ax)>.1f||fabsf(ay)>.1f){autoaim=0;return;}point=target_position(selected_target);
 }
 Vec3 d=norm(sub(point,game.pos));float yaw=atan2f(d.x,d.z),ny=fmaxf(-1,fminf(1,d.y)),pitch=asinf(ny);float diff=atan2f(sinf(yaw-game.yaw),cosf(yaw-game.yaw));float blend=1-expf(-12*fmaxf(0,dt));game.yaw+=fmaxf(-dt*2,fminf(dt*2,diff*blend));game.pitch+=fmaxf(-dt*2,fminf(dt*2,(pitch-game.pitch)*blend));
}
static void route_hyperdrive_tick(float dt){
 int armed=(game.fuel>0||(game.debug_flags&DEBUG_UNLIMITED_FUEL))&&page==FLIGHT&&!game.docked&&!game.dead&&!game.dock_stage&&game.jump<=0&&game.planet<0&&game.approach<0&&route_target_ready()&&(nav_route_tracking()||(selected_target==PLAYER_TARGET_ID&&!wifi_contact_local()&&autoaim))&&game.boost;
 float alignment=armed?dot(forward(&game),route_target_direction()):-1.f;
 if(!armed||alignment<.985f){route_boost_charge=fmaxf(0,route_boost_charge-dt*2.5f);return;}
 route_boost_charge+=dt;
 if(route_boost_charge<5.f)return;
 route_boost_charge=0;
 if(jump_start(&game)){
  /* The visible route charge now occupies the second half of the blue
   * alignment bar. At full charge the route goes straight into the corridor;
   * it must not impose the legacy three-second charge a second time. */
  route_jump_engaged=1;game.jump=5.f;autoaim=0;nav_follow=0;
  message(&game,"Vector locked. Entering hyperspace.");
 }
}
static float route_prejump_progress(void){
 float p=(route_boost_charge-2.5f)/2.5f;return p<0?0:p>1?1:p;
}
static int route_prejump_active(void){
 return game.jump<=0&&route_target_ready()&&nav_route_selected()&&route_boost_charge>=2.5f;
}
static void route_electric_bar(int x,int y,int w,int h,float p);
static void route_target_star(void){
 if(!route_target_ready()||game.jump>0)return;
 Vec3 v=camera(&game,target_position(ROUTE_TARGET_ID));if(v.z<=15)return;Point p=project(v);
 int top=view_top(),bot=view_bot(),x=(int)p.x,y=(int)p.y;if(x<5||x>=W-5||y<top+5||y>bot-5)return;
 float pulse=sinf(game.time*3.4f)*.5f+.5f;unsigned core=RGB(230,247,255),glow=RGB(52+(int)(pulse*35),160+(int)(pulse*50),220+(int)(pulse*35));
 line(x-5,y,x+5,y,glow);line(x,y-5,x,y+5,glow);circle(x,y,2,core);pixel(x,y,WHITE);
 if(nav_route_selected()){
  int w=76,filled=(int)(w*fminf(1.f,route_boost_charge/5.f));
  if(route_boost_charge>0){rect(x-w/2,y+12,w,5,RGB(15,31,46));if(journey_variant)route_electric_bar(x-w/2,y+12,w,5,route_boost_charge/5.f);else if(filled)rect(x-w/2,y+12,filled,5,CYAN);}
 }
}
static void route_prejump_lines(void){
 if(!route_prejump_active())return;
 int top=view_top(),bot=view_bot(),cx=240,cy=(top+bot)/2;float p=route_prejump_progress(),pulse=.78f+.22f*sinf(game.time*18.f);
 int count=10+(int)(p*38.f);
 for(int i=0;i<count;i++){
  float a=i*2.3999632f+game.time*(.22f+p*.7f),seed=fmodf(i*29.f+game.time*(22.f+p*55.f),148.f);
  float r0=18.f+seed,r1=r0+8.f+p*(26.f+(i%6)*5.f);
  int x0=(int)fmaxf(1,fminf(W-2,cx+cosf(a)*r0)),y0=(int)fmaxf(top+1,fminf(bot-1,cy+sinf(a)*r0*.47f));
  int x1=(int)fmaxf(1,fminf(W-2,cx+cosf(a)*r1)),y1=(int)fmaxf(top+1,fminf(bot-1,cy+sinf(a)*r1*.47f));
  unsigned ink=i%4==0?RGB(210,241,255):i%4==1?RGB(61,190,236):i%4==2?RGB(97,123,210):RGB(191,145,230);
  line(x0,y0,x1,y1,dim_rgb(ink,(int)(pulse*3.f),4));
 }
}
static void route_prejump_panel_original(void){
 if(!route_prejump_active())return;
 float p=route_prejump_progress();int pct=(int)(p*100.f+.5f),x=105,y=162,w=270;
 unsigned edge=RGB(193,139,77),fill=p>.82f?RGB(235,128,82):RGB(73,190,226);
 rect(x,y,w,24,RGB(13,20,32));rect(x,y,w,1,edge);rect(x,y+23,w,1,RGB(41,54,70));
 text_px(x+12,y+4,RGB(232,211,164),"HYPERDRIVE CHARGING");
 text_px(x+w-47,y+4,p>.82f?RGB(255,174,103):RGB(133,214,239),"%3d%%",pct);
 rect(x+12,y+16,w-24,3,RGB(25,37,53));
 if(p>0)rect(x+12,y+16,(int)((w-24)*p),3,fill);
}
/* Native pixel electric conduit: no allocations, random flicker or extra HUD height. */
static void route_electric_bar(int x,int y,int w,int h,float p){
 int n=(int)(w*fmaxf(0,fminf(1,p)));float t=game.time;
 rect(x,y,w,h,RGB(18,33,51));if(n<=0)return;
 rect(x,y,n,h,RGB(30,107,175));
 for(int i=0;i<n;i++){
  int wave=(int)roundf(sinf(i*.25f-t*13)*.6f),cy=y+h/2+wave;
  pixel(x+i,cy,RGB(102,221,255));
  sun_bloom_dot(x+i,y-1,RGB(0,13,25),x,y-2,x+w,y+h+2);
  sun_bloom_dot(x+i,y+h,RGB(0,13,25),x,y-2,x+w,y+h+2);
 }
 for(int packet=0;packet<3;packet++){
  int head=(int)(fmodf(t*(60+p*90)+packet*w/3.f,w));
  for(int j=0;j<9;j++){int i=head-j;if(i<0||i>=n)continue;
   unsigned c=RGB(55+(8-j)*22,160+(8-j)*11,255);
   pixel(x+i,y+h/2,c);if(j<3)pixel(x+i,y+h/2-1,RGB(91,204,255));
  }
 }
 int tip=x+n-1,cy=y+h/2;pixel(tip,cy,RGB(235,252,255));
 /* Tiny coherent discharge around the powered frontier, below the label. */
 for(int j=0;j<2;j++){
  float phase=t*8+j*3.14159265f;int reach=2+(int)((.5f+.5f*sinf(phase))*4*p);
  int sign=j?1:-1,bx=tip-reach/2,by=cy+sign*2,ex=tip-reach,ey=cy+sign*(2+(int)(p*2));
  if(bx>=x&&ex>=x){line(tip,cy,bx,by,RGB(126,227,255));line(bx,by,ex,ey,RGB(61,156,224));pixel(bx,by,RGB(218,247,255));}
 }
}
static void route_prejump_panel(void){
 /* The new full cockpit reserves a charging strip in its header. */
 if(page==FLIGHT&&hud_mode==0&&!hud_hidden&&game.planet<0&&!game.dead&&!game.dock_stage&&!game.police_stop&&game.approach<0&&!race_is_active(&game))return;
 if(!journey_variant){route_prejump_panel_original();return;}
 static int was_active;static float shown=-100,hidden=-100,lastp;
 int active=route_prejump_active();
 if(active&&!was_active)shown=game.time;
 if(!active&&was_active)hidden=game.time;
 was_active=active;
 if(active)lastp=route_prejump_progress();
 float age=game.time-(active?shown:hidden);int transient=age>=0&&age<.18f;
 if(!active&&!transient)return;
 int phase=(int)(age*60);
 if(transient&&(active?(age<.025f||(age>=.075f&&age<.112f)):!((age<.038f)||(age>=.078f&&age<.118f))))return;
 float p=active?route_prejump_progress():lastp;int pct=(int)(p*100.f+.5f),x=105,y=162,w=270;
 rect(x,y,w,24,RGB(13,20,32));rect(x,y,w,1,RGB(193,139,77));rect(x,y+23,w,1,RGB(41,54,70));
 text_px(x+12,y+4,RGB(232,211,164),"HYPERDRIVE CHARGING");
 text_px(x+w-47,y+4,RGB(151,227,255),"%3d%%",pct);
 route_electric_bar(x+12,y+16,w-24,3,p);
 if(transient){
  /* Tear only two bands inside the panel; never touch the flight view. */
  for(int band=0;band<2;band++){
   int yy=y+3+((phase*3+band*9)%18),shift=band?2:-2;
   unsigned *row=fb+yy*STRIDE+x;
   if(shift>0){memmove(row+shift,row,(w-shift)*4);rect(x,yy,shift,1,RGB(13,20,32));}
   else{memmove(row,row-shift,(w+shift)*4);rect(x+w+shift,yy,-shift,1,RGB(13,20,32));}
   line(x+8+phase*9,yy,x+38+phase*9,yy,RGB(116,220,255));
  }
 }
}
static void starfield(void){
 /* Every system rotates and reorders the same bounded direction table, then
  * applies its own temperature balance and twinkle clock. This yields a
  * recognisable sky without 256 textures or per-frame allocation. */
 static Vec3 stars[600];static int initialized=0;
 if(!initialized){unsigned seed=91731;for(int i=0;i<600;i++){seed=seed*1664525u+1013904223u;float a=(seed&65535)*6.2831853f/65536;seed=seed*1664525u+1013904223u;float y=(seed&65535)/32767.5f-1;float h=sqrtf(1-y*y);stars[i]=(Vec3){cosf(a)*h,y,sinf(a)*h};}initialized=1;}
 int xt=clipy0>=0?clipx0:1,xb=clipy0>=0?clipx1-1:478,yt=clipy0>=0?clipy0:view_top(),yb=clipy0>=0?clipy1-1:view_bot();
 unsigned seed=space_sky_seed(),style=(seed>>27)&7;float sky_yaw=(seed%6283)*.001f,sky_roll=((int)((seed>>11)&255)-128)*.006f,cy=cosf(sky_yaw),sy=sinf(sky_yaw),cr=cosf(sky_roll),sr=sinf(sky_roll);
 static Vec3 direction_cache[600];static unsigned hash_cache[600],cached_seed;static int cache_ready;
 if(!cache_ready||cached_seed!=seed){
  for(int i=0;i<600;i++){Vec3 base=stars[(i+(seed%599))%600],yawed={base.x*cy+base.z*sy,base.y,-base.x*sy+base.z*cy};direction_cache[i]=(Vec3){yawed.x*cr-yawed.y*sr,yawed.x*sr+yawed.y*cr,yawed.z};hash_cache[i]=field_hash(seed^(unsigned)(i+1)*0x9e3779b9u);}
  cached_seed=seed;cache_ready=1;
 }
 Vec3 rightward,upward,ahead;camera_axes(&game,&rightward,&upward,&ahead);
 for(int i=0;i<600;i++){
  unsigned h=hash_cache[i];Vec3 direction=direction_cache[i];
  Vec3 p=render_reference?camera(&game,add(camera_origin(&game),mul(direction,30000))):mul((Vec3){dot(direction,rightward),dot(direction,upward),dot(direction,ahead)},30000);if(p.z<100)continue;Point q=project(p);
  if(q.x<xt||q.x>xb||q.y<yt||q.y>yb)continue;
  int temperature=(h>>19)%5;unsigned c=temperature==0?RGB(105,181,255):temperature==1?RGB(197,226,255):temperature==2?RGB(255,246,205):temperature==3?RGB(255,192,126):RGB(223,151,187);
  if(style==0||style==6)c=sky_mix(c,RGB(91,181,255),.20f);else if(style==1||style==7)c=sky_mix(c,RGB(237,128,205),.16f);else if(style==3||style==5)c=sky_mix(c,RGB(255,190,92),.18f);
  if(!high_contrast&&((h&7)==0||(h&63)==1))c=space_fx_twinkle(c,i+(seed&255),game.time);
  if(high_contrast)pixel((int)q.x,(int)q.y,c);else space_star_dot(q.x,q.y,c,xt,yt,xb,yb);
  if((h&7)==0&&!high_contrast){unsigned halo=sky_mix(RGB(0,0,0),c,.22f);space_star_dot(q.x+1,q.y,halo,xt,yt,xb,yb);space_star_dot(q.x,q.y+1,halo,xt,yt,xb,yb);}
  if(!high_contrast&&(h&31)==0){
   /* Fixed-size tapered rays. Only their light varies gently; expanding,
    * contracting solid crosses are distracting during camera motion. */
   int x=(int)q.x,y=(int)q.y,flare=3+((h>>5)&1);float light=.91f+.06f*sinf(game.time*.55f+(h&1023)*.006f);
   for(int k=1;k<=flare;k++){unsigned glow=sky_mix(RGB(0,0,0),c,light*(flare+1-k)/(flare+1));
    space_star_dot(q.x-k,q.y,glow,xt,yt,xb,yb);space_star_dot(q.x+k,q.y,glow,xt,yt,xb,yb);
    space_star_dot(q.x,q.y-k,glow,xt,yt,xb,yb);space_star_dot(q.x,q.y+k,glow,xt,yt,xb,yb);
   }
   space_star_dot(q.x,q.y,sky_mix(c,WHITE,.20f),xt,yt,xb,yb);
  }
 }
 for(int i=0;i<48;i++){Vec3 d={(float)((i*719)%4000)-2000,(float)((i*353)%4000)-2000,(float)((i*991)%4000)-2000};d.x-=game.pos.x;d.y-=game.pos.y;d.z-=game.pos.z;d.x-=floorf((d.x+2000)/4000)*4000;d.y-=floorf((d.y+2000)/4000)*4000;d.z-=floorf((d.z+2000)/4000)*4000;Vec3 p=camera(&game,add(game.pos,d));if(p.z<100)continue;Point q=project(p);if(q.x<xt||q.x>xb||q.y<yt||q.y>yb)continue;if(game.boost){float stretch=.04f;line((int)q.x,(int)q.y,(int)(q.x+(q.x-proj_ox)*stretch),(int)(q.y+(q.y-proj_oy)*stretch),DIM);}else pixel((int)q.x,(int)q.y,RGB(60,80,105));}
}
static void station_entrance(void){
 float entry=station_entry_z_for(&game,0);
 Vec3 p[4];for(int i=0;i<4;i++){Vec3 corner=station_port_corner_for(&game,0,i);corner.z-=1;p[i]=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));}
 if(game.pos.z<entry){
  int top=view_top(),bot=view_bot();
  for(int i=0;i<4;i++)if(p[i].z>15&&p[(i+1)%4].z>15){
   Point a=project(p[i]),b=project(p[(i+1)%4]);
   /* Warm structural rim + soft cyan aperture (nav cut only). */
   line((int)a.x,(int)a.y,(int)b.x,(int)b.y,RGB(193,139,77));
   line((int)a.x+1,(int)a.y,(int)b.x+1,(int)b.y,RGB(60,140,145));
   if(!high_contrast)sfx_add((int)((a.x+b.x)*.5f),(int)((a.y+b.y)*.5f),RGB(40,90,95),top,bot);
  }
 }
}
static void target_overlay(void){
 static int animated_target=-999;static float acquire=0,lost=0;static int lost_x=240,lost_y=112;
 pick_look_target();
 if(animated_target!=selected_target){animated_target=selected_target;acquire=.20f;lost=.18f;}
 if(acquire>0){acquire-=.0167f;if(acquire<0)acquire=0;}if(lost>0){lost-=.0167f;if(lost<0)lost=0;}
 if(lost>0){int spread=4+(int)((.18f-lost)*55.f);unsigned fade=lost>.09f?RGB(166,118,62):RGB(73,67,58);line(lost_x-spread,lost_y-spread,lost_x-spread-4,lost_y-spread-4,fade);line(lost_x+spread,lost_y-spread,lost_x+spread+4,lost_y-spread-4,fade);line(lost_x-spread,lost_y+spread,lost_x-spread-4,lost_y+spread+4,fade);line(lost_x+spread,lost_y+spread,lost_x+spread+4,lost_y+spread+4,fade);}
 if(valid_target(selected_target)){Vec3 p=camera(&game,target_position(selected_target));if(p.z>15){Point q=project(p);if(q.x>16&&q.x<464&&q.y>84&&q.y<168){int x=(int)q.x,y=(int)q.y;lost_x=x;lost_y=y;unsigned lock=selected_target==DOCKBAY_TARGET_ID?UI_ACCENT:autoaim?CYAN:AMBER;if(IS_NPC_ID(selected_target))lock=faction_colors[game.npc[selected_target-BODY_COUNT-1].role];int close=14+(int)(acquire*50.f);line(x-close,y-close,x-close+9,y-close,lock);line(x-close,y-close,x-close,y-close+9,lock);line(x+close,y+close,x+close-9,y+close,lock);line(x+close,y+close,x+close,y+close-9,lock);line(x-close,y+close,x-close+9,y+close,lock);line(x+close,y-close,x+close-9,y-close,lock);int pulse=1+(int)(fabsf(sinf(game.time*4))*3);circle(x,y,pulse,lock);if(autoaim){int arm=20+(int)(acquire*24.f);line(x-arm,y,x-arm+4,y,CYAN);line(x+arm-4,y,x+arm,y,CYAN);line(x,y-arm,x,y-arm+4,CYAN);line(x,y+arm-4,x,y+arm,CYAN);}if(IS_STATION_ID(selected_target)&&length(sub(target_position(selected_target),game.pos))<6000){int guide=close+8;line(x-guide,y-3,x-guide,y+3,UI_SIGNAL);line(x+guide,y-3,x+guide,y+3,UI_SIGNAL);}}}}
}
/* Bottom-of-canopy combat cue — keeps speech free at the top. */
static int combat_alert_active(void){return game.incoming_missile>0||game.attacked>0||game.collision>0;}
static void combat_alert_banner(void){
 if(!combat_alert_active())return;
 int bot=view_bot(),y=bot-14;if(y<28)y=28;
 if(y+12>=192&&hud_mode==0)y=178; /* sit in the canopy, above the full HUD strip */
 rect(100,y,280,12,RGB(110,14,20));rect(102,y+1,276,10,RGB(62,8,12));
 const char *msg=game.incoming_missile>0?"RED ALERT - MISSILE":game.collision>0?"RED ALERT - IMPACT":"RED ALERT";
 int len=(int)strlen(msg);text(30-len/2,y/8,WHITE,"%s",msg);
}
/* Live aft camera for the cockpit header.  The very wide projection behaves
 * like a vehicle mirror: it sacrifices vertical sky for useful battle
 * awareness, but reads real world positions rather than a radar abstraction. */
static int rear_mirror_project(const Game *view,Vec3 world,int x,int y,int w,int h,int *sx,int *sy,float *depth){
 Vec3 v=camera(view,world);if(v.z<20)return 0;
 float fx=w*.33f,fy=h*.80f;int px=x+w/2+(int)(v.x*fx/v.z),py=y+h/2-(int)(v.y*fy/v.z);
 if(depth)*depth=v.z;if(sx)*sx=px;if(sy)*sy=py;
 return px>=x&&px<x+w&&py>=y&&py<y+h;
}
static unsigned rear_mirror_dim(unsigned c,int num,int den){return RGB((int)((c&255)*num/den),(int)(((c>>8)&255)*num/den),(int)(((c>>16)&255)*num/den));}
static void rear_view_mirror(int x,int y,int w,int h){
 if(w<48||h<12)return;
 unsigned seed=space_sky_seed(),wash=RGB(3+(seed&7),8+((seed>>4)&11),16+((seed>>9)&15));
 rect(x,y,w,h,wash);rect(x,y,w,1,UI_EDGE);rect(x,y+h-1,w,1,UI_RAISED);rect(x,y,1,h,UI_RAISED);rect(x+w-1,y,1,h,UI_RAISED);
 Game rear;rear.pos=game.pos;rear.yaw=game.yaw+3.14159265f;rear.pitch=-game.pitch;rear.roll=game.roll;
 int oldox=proj_ox,oldoy=proj_oy,oldx0=clipx0,oldx1=clipx1,oldy0=clipy0,oldy1=clipy1;
 preview_clip(x+w/2,y+h/2,x+1,y+1,x+w-1,y+h-1);
 /* Stable celestial points rotate with the ship, making the strip visibly a
  * camera rather than another decorative animation. */
 static unsigned star_seed;static int star_ready;static Vec3 star_dir[92];static unsigned star_hash[92];
 if(!star_ready||star_seed!=seed){star_ready=1;star_seed=seed;for(int i=0;i<92;i++){unsigned q=field_hash(seed^(unsigned)(i+1)*0x9e3779b9u);star_hash[i]=q;star_dir[i]=norm((Vec3){(float)((int)(q&1023)-511),(float)((int)((q>>10)&1023)-511),(float)((int)((q>>20)&1023)-511)});}}
 for(int i=0;i<92;i++){
  unsigned q=star_hash[i];Vec3 d=star_dir[i];
  int sx,sy;if(!rear_mirror_project(&rear,add(game.pos,mul(d,30000)),x+1,y+1,w-2,h-2,&sx,&sy,0))continue;
  unsigned c=(q&7)?RGB(75,104,130):(q&8)?RGB(255,205,126):RGB(134,198,255);pixel(sx,sy,c);if((q&31)==0)pixel(sx+1,sy,c);
 }
 /* Large real bodies are restrained to rings/discs so a nearby planet cannot
  * erase the entire tactical view. */
 for(int i=0;i<BODY_COUNT;i++){
  if(!body_active(&game.bodies[i]))continue;
  int sx,sy;float z;if(!rear_mirror_project(&rear,game.bodies[i].pos,x+1,y+1,w-2,h-2,&sx,&sy,&z))continue;
  int r=(int)(h*.8f*game.bodies[i].radius/z);if(r<1)r=1;if(r>h/2-2)r=h/2-2;
  unsigned c=game.bodies[i].type==SUN?RGB(255,190,74):game.bodies[i].color;circle(sx,sy,r,c);if(r>2)circle(sx,sy,r-2,rear_mirror_dim(c,2,3));
 }
 /* Station/secondary hub markers are world-positioned, not heading glyphs. */
 for(int hub=0;hub<HUB_COUNT;hub++){
  int sx,sy;float z;Vec3 p=hub_position(&game,hub);if(!rear_mirror_project(&rear,p,x+1,y+1,w-2,h-2,&sx,&sy,&z))continue;
  int r=(int)(9000.f/fmaxf(600,z));if(r<2)r=2;if(r>6)r=6;unsigned c=hub?UI_MUTED:UI_SIGNAL;
  line(sx-r,sy,sx,sy-r/2,c);line(sx,sy-r/2,sx+r,sy,c);line(sx+r,sy,sx,sy+r/2,c);line(sx,sy+r/2,sx-r,sy,c);
 }
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&game.npc[i];if(!n->alive)continue;int sx,sy;float z;if(!rear_mirror_project(&rear,n->pos,x+1,y+1,w-2,h-2,&sx,&sy,&z))continue;
  int r=(int)(n->radius*72.f/fmaxf(150,z));if(n->freighter)r+=2;if(r<2)r=2;if(r>8)r=8;
  unsigned c=n->flash>0?WHITE:faction_colors[n->role],dim=rear_mirror_dim(c,1,2);Vec3 tail=sub(n->pos,mul(n->dir,n->radius*(n->freighter?5.f:8.f)));int tx,ty;
  if(rear_mirror_project(&rear,tail,x+1,y+1,w-2,h-2,&tx,&ty,0))line(sx,sy,tx,ty,dim);
  line(sx-r,sy,sx,sy-r/2,c);line(sx,sy-r/2,sx+r,sy,c);line(sx+r,sy,sx,sy+r/2,c);line(sx,sy+r/2,sx-r,sy,c);pixel(sx,sy,WHITE);
  int id=NPC_ID_MIN+i,threat=game.fire_bearing_time[i]>0||(game.incoming_missile>0&&game.incoming_source==i);
  if(id==selected_target||threat){unsigned lock=threat?RED:UI_ACCENT;line(sx-r-3,sy-r-2,sx-r,sy-r-2,lock);line(sx-r-3,sy-r-2,sx-r-3,sy,lock);line(sx+r+3,sy+r+2,sx+r,sy+r+2,lock);line(sx+r+3,sy+r+2,sx+r+3,sy,lock);}
 }
 preview_clip(oldox,oldoy,oldx0,oldy0,oldx1,oldy1);
}
static void minimal_overlay(void){
 pick_look_target();
 rect(0,0,W,24,RGB(21,28,39));rect(0,23,W,1,RGB(193,139,77));
 rect(0,248,W,24,RGB(21,28,39));rect(0,248,W,1,RGB(193,139,77));
 rear_view_mirror(4,1,252,22);
 text(43,1,RGB(229,210,163),"SPD %d",(int)game.speed);
 text(1,32,game.energy<30?RED:RGB(85,212,212),"SHIELD %d%%",(int)game.energy);
 int id=valid_target(selected_target)?selected_target:valid_target(look_target)?look_target:-1;
 if(id>=0)text(25,32,RGB(240,180,91),"%.32s",target_name(id));
 combat_alert_banner();
}
static int fire_indicator_position(int source,float *x,float *y,float *dx,float *dy){
 if(source<0||source>=NPC_COUNT||!game.npc[source].alive||(game.fire_bearing_time[source]<=0&&!(game.incoming_missile>0&&game.incoming_source==source)))return 0;
 Vec3 p=camera(&game,game.npc[source].pos);
 float cy=(view_top()+view_bot())*.5f,rx=224,ry=(view_bot()-view_top())*.5f-22;
 if(ry<12)ry=12;
 if(p.z>15){
  Point q=project(p);float ex=(q.x-240)/rx,ey=(q.y-cy)/ry;
  if(ex*ex+ey*ey<.85f){*x=q.x;*y=q.y-12;*dx=0;*dy=1;return 1;}
 }
 float vx=p.x,vy=-p.y;
 if(fabsf(vx)+fabsf(vy)<.01f){vx=0;vy=p.z<0?1:-1;}
 float mag=sqrtf(vx*vx+vy*vy);vx/=mag;vy/=mag;
 *x=240+rx*vx;*y=cy+ry*vy;*dx=vx;*dy=vy;return p.z<0?2:1;
}
static void incoming_fire_indicators(void){
 if(game.docked||game.dead||game.planet>=0||game.jump>0)return;
 for(int i=0;i<NPC_COUNT;i++){
  float x,y,dx,dy;int rear=fire_indicator_position(i,&x,&y,&dx,&dy);if(!rear)continue;hc_indicator_anchor(&x,&y);
  unsigned ink=game.fire_bearing_time[i]>.8f?RGB(255,110,72):RGB(139,75,56);
  for(int k=0;k<(rear==2?2:1);k++){
   float tx=x-dx*k*4,ty=y-dy*k*4;
   line((int)(tx-dx*6-dy*3),(int)(ty-dy*6+dx*3),(int)tx,(int)ty,ink);
   line((int)(tx-dx*6+dy*3),(int)(ty-dy*6-dx*3),(int)tx,(int)ty,ink);
  }
 }
}
static void ship_wake_stroke(Point,Point,float,float,unsigned,float,int);
static void distant_ship_trail(const NPC*n){
 float d=length(sub(n->pos,game.pos));if(d>110000.f)return;
 float visibility=fminf(1.f,fmaxf(0,(110000.f-d)/35000.f));if(visibility<=0)return;
 Vec3 c=camera(&game,n->pos);if(c.z<30)return;Point p=project(c);
 int x=(int)p.x,y=(int)p.y,top=view_top()+2,bot=view_bot()-2;
 if(x<3||x>W-4||y<top+2||y>bot-2)return;
 if(mega_depth_valid&&mega_encode_depth(c.z)>surface_depth[y*W+x])return;
 float apparent=render_pixel_radius(n->pos,npc_visual_radius(n));
 unsigned tint=n->role==LAW?RGB(111,180,255):n->role==PIRATES?RGB(255,114,167):n->role==EXPLORERS?RGB(100,235,222):RGB(255,192,113);
 if(apparent>4){unsigned ink=faction_colors[n->role];rect(x-1,y,n->freighter?5:3,1,ink);}
 if(!high_contrast){
  float glow=fminf(1,fmaxf(.35f,(7-apparent)/5));
  unsigned depth=mega_encode_depth(c.z);
  for(int yy=-2;yy<=2;yy++)for(int xx=-2;xx<=2;xx++){
   int weight=6-xx*xx-yy*yy;if(weight<=0)continue;
   if(mega_depth_valid&&depth>surface_depth[(y+yy)*W+x+xx])continue;
   float strength=weight*glow*visibility/21.f;
   sfx_add(x+xx,y+yy,RGB((tint&255)*strength,((tint>>8)&255)*strength,((tint>>16)&255)*strength),top,bot);
  }
 }
 /* Never blink the whole ship out. Only the tiny navigation lamp flashes. */
 float phase=fmodf(game.time*.72f+(n->vessel_seed&1023)*.0009765625f,1.f);
 float shimmer=(.88f+.10f*sinf(game.time*2.3f+(n->vessel_seed&63)))*visibility;
 pixel(x,y,RGB((int)(241*shimmer),(int)(239*shimmer),(int)(218*shimmer)));
 if(phase<.085f)pixel(x+2,y-1,dim_rgb(n->role==LAW?RGB(130,217,255):RGB(255,112,91),(int)(visibility*256),256));
 /* Real traffic, optically enlarged at subpixel size. The body stays steady;
  * only its engine aspect and navigation light supply sparkle. */
 if(!high_contrast&&n->cruise>5){
  Vec3 direction=norm(n->dir),tail=add(n->pos,mul(direction,-fminf(1100,npc_visual_radius(n)*7+n->cruise*1.8f)));
  Vec3 rear=camera(&game,tail);float aspect=fmaxf(0,dot(norm(sub(n->pos,game.pos)),direction));
  if(rear.z>30){Point q=project(rear);ship_wake_stroke(p,q,.55f,.08f,tint,(.12f+.25f*aspect)*visibility,1);}
  float flare=powf(aspect,8)*visibility*(.55f+.45f*sinf(game.time*1.7f+(n->vessel_seed&31)));
  if(flare>.14f)for(int k=-4;k<=4;k++){
   if(!k)continue;int sx=x+k;if(sx<0||sx>=W)continue;
   if(mega_depth_valid&&mega_encode_depth(c.z)>surface_depth[y*W+sx])continue;
   sfx_add(sx,y,dim_rgb(tint,(int)((5-abs(k))*flare*28),256),top,bot);
  }
 }
}

static void missile_effects(void){
 int top=view_top(),bot=view_bot();
 if(game.missile_time>0&&!occluded(game.missile_pos)){
  for(int i=1;i<game.missile_trail_n;i++){
   Vec3 a=camera(&game,game.missile_trail[i-1]),b=camera(&game,game.missile_trail[i]);
   if(a.z<=15||b.z<=15)continue;Point p=project(a),q=project(b);
   if(p.x<3||p.x>476||q.x<3||q.x>476||p.y<top+3||p.y>bot-3||q.y<top+3||q.y>bot-3)continue;
   line((int)p.x,(int)p.y,(int)q.x,(int)q.y,i<4?RGB(255,176,72):RGB(108,112,126));
  }
  Vec3 p=camera(&game,game.missile_pos);if(p.z>15){Point q=project(p);
   if(q.x>5&&q.x<474&&q.y>top+5&&q.y<bot-5){int x=(int)q.x,y=(int)q.y;
    circle(x,y,2,GOLD);pixel(x,y,WHITE);
   }
  }
 }
}
static void planet_prompt(void){
 if(game.approach<0||game.approach>=BODY_COUNT)return;
 /* The safety modal always shares the full cockpit's clear centre. */
 static int shown=-1;static float reveal=0;if(shown!=game.approach){shown=game.approach;reveal=.18f;}if(reveal>0){reveal-=.0167f;if(reveal<0)reveal=0;}
 int y0=88+(int)(reveal*38.f),solid=game.bodies[game.approach].type!=SUN;
 rect(20,y0,440,84,RGB(21,28,39));rect(20,y0,440,2,RGB(193,139,77));rect(20,y0+82,440,2,RGB(41,54,70));
 text(7,(y0+8)/8,RGB(240,180,91),"LANDING CLEARANCE: %s",game.bodies[game.approach].name);
 if(solid){button_icon(58,y0+38,'X',RGB(85,212,212));text(10,(y0+39)/8,RGB(229,210,163),"land & exit");}
 button_icon(210,y0+38,'O',RED);text(29,(y0+39)/8,RGB(229,210,163),"cancel");
 text(7,(y0+64)/8,RGB(155,154,165),solid?"Guided landing, then automatic disembark.":"No solid surface. Circle turns back.");
}
static void celestial_rims(void){
 static float rim_x[96],rim_y[96];static int ready;
 if(!ready){for(int k=0;k<96;k++){float angle=k*6.2831853f/96;rim_x[k]=cosf(angle);rim_y[k]=sinf(angle);}ready=1;}
 int top=view_top(),bot=view_bot();
 for(int i=0;i<BODY_COUNT;i++){
  Body *b=&game.bodies[i];if(!body_active(b))continue;Vec3 v=camera(&game,b->pos);if(v.z<100)continue;Point p=project(v);
  int r=(int)fminf(700,240*b->radius/v.z);if(r<3||p.x+r<0||p.x-r>W||p.y+r<top||p.y-r>bot)continue;
  unsigned tint=b->type==SUN?b->color:b->type==OCEAN?RGB(70,150,195):b->type==GAS?RGB(105,115,143):RGB(133,102,82);
  /* Match the actual disc radius; no detached circular halos on approach. */
  for(int k=3;k>=1;k--){unsigned ink=RGB((tint&255)/(k+2),((tint>>8)&255)/(k+2),((tint>>16)&255)/(k+2));
   for(int s=0;s<96;s++){int x=(int)p.x+(int)(rim_x[s]*(r+k)),y=(int)p.y+(int)(rim_y[s]*(r+k));if(y>=top&&y<=bot)pixel(x,y,ink);}
  }
 }
}
/* Ringed worlds need painter-order depth: the distant half is drawn before
 * the globe, then the near half crosses visibly over its foreground. */
/* Ray/plane annulus raster: translucent material, not wireframe circles.
 * Fixed 2px sampling and screen bounds keep near-ring fill cost bounded. */
#if defined(ELITE_ENABLE_REVIEW) || defined(ELITE_ENCOUNTER_REVIEW)
static void celestial_rings_reference(int foreground){
 int top=view_top(),bot=view_bot();
 for(int i=0;i<BODY_COUNT;i++){
 Body *b=&game.bodies[i];if(!body_active(b)||b->type!=GAS)continue;
 Vec3 centre=camera(&game,b->pos);float inner=planet_ring_inner(b),outer=planet_ring_outer(b);
 if(centre.z+outer<30||length(centre)>outer*90)continue;
 float tilt=.18f+((b->seed>>11)&7)*.045f;
 Vec3 normal=sub(camera(&game,add(b->pos,(Vec3){0,cosf(tilt),-sinf(tilt)})),centre);
 Vec3 sunlight=norm(sub(camera(&game,game.bodies[0].pos),centre));
 float plane=dot(normal,centre);if(fabsf(plane)<.1f)continue;
 int x0=0,x1=W-1,y0=top,y1=bot;
 if(centre.z>outer*1.2f){Point p=project(centre);float r=240*outer/(centre.z-outer);
 x0=fmaxf(0,p.x-r);x1=fminf(W-1,p.x+r);y0=fmaxf(top,p.y-r);y1=fminf(bot,p.y+r);}
 for(int y=y0;y<=y1;y+=2)for(int x=x0;x<=x1;x+=2){
 Vec3 ray={(x+.5f-proj_ox)/240.f,(proj_oy-y-.5f)/240.f,1};float den=dot(normal,ray);if(fabsf(den)<.00001f)continue;
 float z=plane/den;if(z<30||((z<centre.z)?1:0)!=foreground)continue;
 Vec3 delta=sub(mul(ray,z),centre);float r2=dot(delta,delta);if(r2<inner*inner||r2>outer*outer)continue;
 /* Exact globe intersection avoids a far-side veil around the planet edge. */
 float aa=dot(ray,ray),along=dot(ray,centre),disc=along*along-aa*(dot(centre,centre)-b->radius*b->radius);
 if(disc>=0){float hit=(along-sqrtf(disc))/aa;if(hit>0&&hit<z)continue;}
 float u=(sqrtf(r2)-inner)/(outer-inner);if(u>.41f&&u<.46f||u>.72f&&u<.745f)continue;
 int band=(int)(u*24);int alpha=band%3==0?105:135;float edge=fminf(u,1-u)*18;if(edge<1)alpha*=fmaxf(0,edge);
 unsigned ink=RGB(133+(b->seed&15),124+band%4*4,119+((b->seed>>6)&15));
 float shadow=dot(delta,sunlight);if(shadow<0&&r2-shadow*shadow<b->radius*b->radius)ink=RGB((ink&255)*2/5,((ink>>8)&255)*2/5,((ink>>16)&255)*2/5);
 /* Nearby dust grain modulation; distant rings stay cohesive. */
 if(z<outer*.8f){unsigned h=(unsigned)(x/2)*73856093u^(unsigned)(y/2)*19349663u^b->seed;alpha=alpha*(75+(h%51))/100;}
 for(int dy=0;dy<2&&y+dy<=bot;dy++)for(int dx=0;dx<2&&x+dx<W;dx++){
 unsigned old=fb[(y+dy)*STRIDE+x+dx];unsigned c=RGB(((old&255)*(255-alpha)+(ink&255)*alpha)/255,(((old>>8)&255)*(255-alpha)+((ink>>8)&255)*alpha)/255,(((old>>16)&255)*(255-alpha)+((ink>>16)&255)*alpha)/255);fb[(y+dy)*STRIDE+x+dx]=c;}
 }
 }
}
#endif
/* Conservative row bounds for the projected outer annulus. The exact existing
 * ray/plane test still decides every pixel; empty sky is never scanned. */
static int celestial_ring_span(Vec3 centre,Vec3 normal,float plane,float outer,int y,int x0,int x1,int *left,int *right){
 Vec3 ray={(x0+.5f-proj_ox)/240.f,(proj_oy-y-.5f)/240.f,1};
 float den=dot(normal,ray),dd=normal.x/240.f,rr=outer*outer;
 float end=den+(x1-x0)*dd,lo_den=fminf(den,end),hi_den=fmaxf(den,end);
 if(plane>0?(hi_den<=0||lo_den>plane/30.f):(lo_den>=0||hi_den<plane/30.f))return 0;
 Vec3 v=sub(mul(ray,plane),mul(centre,den)),step=sub((Vec3){plane/240.f,0,0},mul(centre,dd));
 float a=dot(step,step)-rr*dd*dd,b=2*(dot(v,step)-rr*den*dd),c=dot(v,v)-rr*den*den;
 *left=x0;*right=x1;if(a<=.00001f)return 1;
 float disc=b*b-4*a*c;if(disc<0)return c<=0;
 float root=sqrtf(disc),lo=(-b-root)/(2*a),hi=(-b+root)/(2*a);
 /* Two sample cells of slack protect the bound from float cancellation. */
 int l=x0+2*(int)floorf((lo-4)/2),r=x0+2*(int)ceilf((hi+4)/2);
 if(l>*left)*left=l;if(r<*right)*right=r;return *left<=*right;
}
static void __attribute__((optimize(ELITE_VISUAL_OPT))) celestial_rings(int foreground){
 int top=view_top(),bot=view_bot();
 for(int i=0;i<BODY_COUNT;i++){
 Body *b=&game.bodies[i];if(!body_active(b)||b->type!=GAS)continue;
 Vec3 centre=camera(&game,b->pos);float inner=planet_ring_inner(b),outer=planet_ring_outer(b);
 if(centre.z+outer<30||length(centre)>outer*90)continue;
 if(proj_focal==240.f&&!render_sphere_visible(b->pos,outer))continue;
 if(foreground&&centre.z<=30)continue;
 float tilt=.18f+((b->seed>>11)&7)*.045f;
 Vec3 normal=sub(camera(&game,add(b->pos,(Vec3){0,cosf(tilt),-sinf(tilt)})),centre);
 Vec3 sunlight=norm(sub(camera(&game,game.bodies[0].pos),centre));
 float plane=dot(normal,centre);if(fabsf(plane)<.1f)continue;
 int x0=0,x1=W-1,y0=top,y1=bot;
 if(centre.z>outer*1.2f){Point p=project(centre);float r=240*outer/(centre.z-outer);
 x0=fmaxf(0,p.x-r);x1=fminf(W-1,p.x+r);y0=fmaxf(top,p.y-r);y1=fminf(bot,p.y+r);}
 for(int y=y0;y<=y1;y+=2){int left,right;if(!celestial_ring_span(centre,normal,plane,outer,y,x0,x1,&left,&right))continue;for(int x=left;x<=right;x+=2){
 Vec3 ray={(x+.5f-proj_ox)/240.f,(proj_oy-y-.5f)/240.f,1};float den=dot(normal,ray);if(fabsf(den)<.00001f)continue;
 float z=plane/den;if(z<30||((z<centre.z)?1:0)!=foreground)continue;
 Vec3 delta=sub(mul(ray,z),centre);float r2=dot(delta,delta);if(r2<inner*inner||r2>outer*outer)continue;
 /* Exact globe intersection avoids a far-side veil around the planet edge. */
 float aa=dot(ray,ray),along=dot(ray,centre),disc=along*along-aa*(dot(centre,centre)-b->radius*b->radius);
 if(disc>=0){float hit=(along-sqrtf(disc))/aa;if(hit>0&&hit<z)continue;}
 float u=(sqrtf(r2)-inner)/(outer-inner);if(u>.41f&&u<.46f||u>.72f&&u<.745f)continue;
 int band=(int)(u*24);int alpha=band%3==0?105:135;float edge=fminf(u,1-u)*18;if(edge<1)alpha*=fmaxf(0,edge);
 unsigned ink=RGB(133+(b->seed&15),124+band%4*4,119+((b->seed>>6)&15));
 float shadow=dot(delta,sunlight);if(shadow<0&&r2-shadow*shadow<b->radius*b->radius)ink=RGB((ink&255)*2/5,((ink>>8)&255)*2/5,((ink>>16)&255)*2/5);
 /* Nearby dust grain modulation; distant rings stay cohesive. */
 if(z<outer*.8f){unsigned h=(unsigned)(x/2)*73856093u^(unsigned)(y/2)*19349663u^b->seed;alpha=alpha*(75+(h%51))/100;}
 for(int dy=0;dy<2&&y+dy<=bot;dy++)for(int dx=0;dx<2&&x+dx<W;dx++){
 unsigned old=fb[(y+dy)*STRIDE+x+dx];unsigned c=RGB(((old&255)*(255-alpha)+(ink&255)*alpha)/255,(((old>>8)&255)*(255-alpha)+((ink>>8)&255)*alpha)/255,(((old>>16)&255)*(255-alpha)+((ink>>16)&255)*alpha)/255);fb[(y+dy)*STRIDE+x+dx]=c;}
 }
 }
 }
}
static void station_glow(void){
 float entry=station_entry_z_for(&game,0);
 if(game.pos.z>=entry||occluded((Vec3){0,0,entry}))return;
 int top=view_top(),bot=view_bot();
 for(int i=0;i<4;i++){
  Vec3 corner=station_port_corner_for(&game,0,i);corner.z-=2;
  Vec3 v=camera(&game,add(rotate(corner,0,station_angle(&game)),(Vec3){0,0,STATION_Z}));
  if(v.z<20)continue;Point p=project(v);
  if(!high_contrast)sfx_add((int)p.x,(int)p.y,RGB(40,90,95),top,bot);
 }
}
/* Mesh ships are drawn with yaw only; freighters follow full dir. Match that here. */
static Vec3 npc_draw_facing(const NPC *n){
 if(n->freighter)return n->dir;
 Vec3 f={n->dir.x,0,n->dir.z};float L=length(f);return L>.001f?mul(f,1.f/L):(Vec3){0,0,1};
}
static float npc_engine_aft(const NPC *n){
 if(n->freighter)return freight_extent(n).z*.998f; /* capital stern nozzles */
 float aft=0;const Mesh *m=&meshes[n->mesh];
 for(int v=0;v<m->vertices;v++)aft=fmaxf(aft,-m->v[v].z*n->scale);
 if(aft<1)aft=n->radius*.45f;
 return aft;
}
static Vec3 npc_engine_root(const NPC *n,float lateral){
 Vec3 facing=npc_draw_facing(n),side=norm((Vec3){facing.z,0,-facing.x});
 if(n->freighter){Vec3 e=freight_extent(n);return freight_world(n,(Vec3){lateral,-e.y*.05f,-e.z*.998f-2.6f});}
 if(player_ship_is_mega(n->mega_hull)){const MegaHullArt*h=&mega_hull_art[n->mega_hull-PLAYER_SHIP_MEGA_FIRST];int chosen=-1;float best=-1e9f;for(int i=0;i<h->parts;i++)if(h->part[i].shade==3){float score=h->part[i].z+h->part[i].hz+(lateral<0?1:-1)*h->part[i].x;if(score>best){best=score;chosen=i;}}if(chosen>=0){const MegaHullPart*b=&h->part[chosen];return mega_npc_point(n,(Vec3){b->x,b->y,b->z+b->hz+.6f});}}
 if(!n->freighter&&n->mesh>=0&&n->mesh<35&&fleet_hulls[n->mesh].mesh){const FleetHull*h=&fleet_hulls[n->mesh];Vec3 root=h->outlet[lateral<0?0:h->engines-1];return add(n->pos,rotate(mul(root,n->scale),atan2f(facing.x,facing.z),0));}
 /* Flush to the aft face — not radius-behind, which floated past the silhouette. */
 return add(add(n->pos,mul(facing,-npc_engine_aft(n))),mul(side,lateral));
}
static int npc_outlet_count(const NPC*n){
 if(player_ship_is_mega(n->mega_hull)){const MegaHullArt*h=&mega_hull_art[n->mega_hull-PLAYER_SHIP_MEGA_FIRST];int count=0;for(int i=0;i<h->parts;i++)count+=h->part[i].shade==3;return count?count:1;}
 if(n->freighter||n->mesh==4)return 2;return n->mesh>=0&&n->mesh<35&&fleet_hulls[n->mesh].mesh?fleet_hulls[n->mesh].engines:1;
}
static Vec3 npc_outlet_world(const NPC*n,int index){
 if(player_ship_is_mega(n->mega_hull)){const MegaHullArt*h=&mega_hull_art[n->mega_hull-PLAYER_SHIP_MEGA_FIRST];int k=0;for(int i=0;i<h->parts;i++)if(h->part[i].shade==3){if(k++==index){const MegaHullPart*b=&h->part[i];return mega_npc_point(n,(Vec3){b->x,b->y,b->z+b->hz+.6f});}}}
 return npc_engine_root(n,npc_outlet_count(n)==2?(index?1:-1)*(n->freighter?freight_extent(n).x*.38f:17.f*n->scale):0);
}
static unsigned npc_trail_neon(const NPC *n){
 /* Keep the plume attached to the same aft root as the mesh. The tint is a
  * readable livery cue at distance, not a second light source under the hull. */
 if(n->role==PIRATES)return RGB(242,68,168);       /* hot magenta */
 if(n->role==EXPLORERS)return RGB(72,224,255);    /* ion cyan */
 if(n->role==LAW)return RGB(116,168,255);         /* patrol blue */
 return n->freighter?RGB(255,190,70):RGB(255,112,48); /* amber / orange */
}
static void ship_sprite_detail(const NPC *n,unsigned color){
 /* Cockpit/engine glint on the mesh aft tip (same anchor as the plume). */
 float d=length(sub(n->pos,game.pos));if(d>3000)return;
 Vec3 c=camera(&game,n->pos);if(c.z<25)return;
 Vec3 back=camera(&game,npc_engine_root(n,0));
 if(back.z<25)return;
 Point p=project(back);int s=(int)fmaxf(1,fminf(3,700.f/c.z));
 world_spark((int)p.x,(int)p.y,s,color);
}
/* Soft tapered world-space wakes. Fixed per-frame pixel budget, no allocations. */
static int ship_wake_budget;
static void ship_wake_stroke(Point a,Point b,float wa,float wb,unsigned tint,float intensity,int hot){
 if(ship_wake_budget<=0)return;
 float dx=b.x-a.x,dy=b.y-a.y,len2=dx*dx+dy*dy;
 if(len2<.04f)return;
 float radius=fmaxf(wa,wb)+2;
 float invlen=1.f/len2,invwa=1.f/(wa*wa),invwb=1.f/(wb*wb);
 float invga=1.f/((wa+1.2f)*(wa+1.2f)),invgb=1.f/((wb+1.2f)*(wb+1.2f));
 int x0=(int)fmaxf(1,floorf(fminf(a.x,b.x)-radius)),x1=(int)fminf(W-2,ceilf(fmaxf(a.x,b.x)+radius));
 int y0=(int)fmaxf(view_top()+1,floorf(fminf(a.y,b.y)-radius)),y1=(int)fminf(view_bot()-1,ceilf(fmaxf(a.y,b.y)+radius));
 for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){
  if(ship_wake_budget--<=0)return;
  float t=((x+.5f-a.x)*dx+(y+.5f-a.y)*dy)*invlen;
  if(t<0||t>1)continue;
  float px=x+.5f-a.x-dx*t,py=y+.5f-a.y-dy*t,radial=px*px+py*py;
  float q=radial*(invwa+(invwb-invwa)*t),gq=radial*(invga+(invgb-invga)*t);
  if(gq>=1)continue;
  if(mega_depth_valid){float z=1.f/((1-t)/a.z+t/b.z);if(mega_encode_depth(z)>surface_depth[y*W+x])continue;}
  float soft=fmaxf(0,1-q),haze=1-gq;float gain=intensity*(soft*soft+haze*haze*.22f);
  int rr=(int)((tint&255)*gain),gg=(int)(((tint>>8)&255)*gain),bb=(int)(((tint>>16)&255)*gain);
  if(hot&&q<.13f){rr+=(int)(125*gain);gg+=(int)(160*gain);bb+=(int)(180*gain);}
  sfx_add(x,y,RGB(rr>255?255:rr,gg>255?255:gg,bb>255?255:bb),view_top(),view_bot());
 }
}
static void npc_engine_glow(void){
 ship_wake_budget=24000;
 int top=view_top(),bot=view_bot();
 for(int i=0;i<NPC_COUNT&&ship_wake_budget>0;i++){
  NPC*n=&game.npc[i];if(!n->alive||occluded(n->pos))continue;
  float d=length(sub(n->pos,game.pos));if(d>(n->freighter?55000.f:18000.f))continue;
  Vec3 facing=npc_draw_facing(n),side=norm((Vec3){facing.z,0,-facing.x});
  int plumes=npc_outlet_count(n);
  float pixels=render_pixel_radius(n->pos,npc_visual_radius(n));
  int segments=pixels<8?4:7;unsigned neon=npc_trail_neon(n);
  float power=fminf(1,fmaxf(.12f,n->cruise/220.f));
  float wake=n->freighter?900.f+power*700.f:fminf(1400.f,fmaxf(130.f,n->scale*120.f+d*.065f))*power;
  for(int plume=0;plume<plumes;plume++){
   Vec3 rear=npc_outlet_world(n,plume),rv=camera(&game,rear);
   if(rv.z<25)continue;Point root=project(rv);
   if(root.x<3||root.x>W-4||root.y<top+3||root.y>bot-3)continue;
   if(mega_depth_valid&&mega_encode_depth(rv.z)>surface_depth[(int)root.y*W+(int)root.x])continue;
   float root_width=fminf(4.5f,fmaxf(1.25f,240.f*(n->freighter?24.f:4.f*n->scale)/rv.z));
   Point last=root;
   for(int k=1;k<=segments;k++){
    float t=k/(float)segments,previous=(k-1)/(float)segments;
    float flicker=sinf(game.time*8.f+i*1.7f+plume*2.1f+t*5.f);
    Vec3 tail=add(add(rear,mul(facing,-wake*t)),mul(side,flicker*t*t*(pixels<8?1.5f:3.f)));
    tail.y+=cosf(game.time*6.f+i+t*4.f)*t*t*1.5f;
    Vec3 tv=camera(&game,tail);if(tv.z<25||occluded(tail))break;Point q=project(tv);
    float w0=root_width*(1-previous*.85f)+.35f,w1=root_width*(1-t*.85f)+.35f;
    float gain=.58f*(1-t*.88f);
    if(high_contrast)line((int)last.x,(int)last.y,(int)q.x,(int)q.y,neon);
    else{
     /* Diffuse amber/ion sheath plus narrower luminous plasma core. */
     ship_wake_stroke(last,q,w0,w1,neon,gain,k<=2);
    }
    last=q;
   }
   if(!high_contrast){
    for(int y=-2;y<=2;y++)for(int x=-2;x<=2;x++){
     int weight=6-x*x-y*y;if(weight<=0)continue;
     sfx_add((int)root.x+x,(int)root.y+y,RGB((neon&255)*weight/16,((neon>>8)&255)*weight/16,((neon>>16)&255)*weight/16),top,bot);
    }
   }
   pixel((int)root.x,(int)root.y,RGB(255,241,211));
  }
 }
}
static void lens_flares(void){
 if(high_contrast||occluded(game.bodies[0].pos))return;
 Vec3 p=camera(&game,game.bodies[0].pos);if(p.z<=100)return;Point q=project(p);
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 if(q.x<-60||q.x>=W+60||q.y<top-60||q.y>bot+60)return;
 float power=fmaxf(0,1-length((Vec3){q.x-proj_ox,q.y-proj_oy,0})/320.f);
 if(power<=0)return;
 unsigned tint=game.bodies[0].color;int fam=sun_family(game.bodies[0].seed);
 /* Horizontal anamorphic streak */
 float apparent=240*game.bodies[0].radius/p.z;
 int span=(int)fminf(220,(60+fam*2+apparent*.9f)*power*(1.f+1.3f*fminf(1,game.departure_glow*.5f)));
 if(span<1)return;
 for(int dx=-span;dx<=span;dx+=2){
  int x=(int)q.x+dx,y=(int)q.y;if(x<0||x>=W||y<top||y>bot)continue;
  int fall=span?span-abs(dx):1;unsigned c=RGB(((tint&255)*fall*power)/(span*(3.f-1.6f*fminf(1,game.departure_glow*.5f))),(((tint>>8)&255)*fall*power)/(span*3),(((tint>>16)&255)*fall*power)/(span*3));
  sun_bloom_dot(x,y,c,0,top,W,bot);
  if((dx&3)==0&&y+1<=bot)sun_bloom_dot(x,y+1,c,0,top,W,bot);
 }
 /* Ghost orbs along the optical axis */
 for(int i=0;i<5;i++){float t=.3f+i*.4f;int x=(int)(q.x+(proj_ox-q.x)*t),y=(int)(q.y+(proj_oy-q.y)*t);
  if(x>3&&x<W-3&&y>top+2&&y<bot-2){
   unsigned ghost=i&1?RGB(40,70,100):RGB(90,60,30);
   int r=1+(int)fminf(5,apparent*.025f*power);
   for(int dy=-r;dy<=r;dy++)for(int dx=-r;dx<=r;dx++){int d=dx*dx+dy*dy;if(d<=r*r){float fade=power*(r*r+1-d)/(r*r+1.f)*.4f;sun_bloom_dot(x+dx,y+dy,sky_mix(RGB(0,0,0),ghost,fade),0,top,W,bot);}}
  }
 }
}
static void police_custody_animation(void){
 int transfer=game.police_phase==2||game.police_phase==4;
 float p=1-game.police_timer/(transfer?2.8f:3.5f);p=fmaxf(0,fminf(1,p));
 rect(16,194,448,48,RGB(5,12,23));
 if(transfer){
  for(int i=0;i<24;i++)pixel(20+(i*73+(int)(p*140))%436,197+i*17%31,RGB(112,142,164));
  int x=45+(int)(p*320);
  rect(394,198,48,32,RGB(57,68,83));rect(406,203,24,22,RGB(8,17,28));
  rect(x,203,44,18,RGB(124,142,159));rect(x+33,207,15,10,RGB(78,155,183));
  rect(x-10,208,10,7,RGB(241,153,60));rect(x+8,199,21,4,RGB(64,98,134));
  text_px(24,232,UI_GOLD,"SECURITY SHUTTLE / BERTH APPROACH");
 }else{
  rect(52,197,376,32,RGB(37,44,54));rect(82,208,102,12,RGB(73,89,106));
  for(int i=0;i<9;i++){int x=60+i*43+(int)((1-p)*20);rect(x,196,4,34,RGB(100,116,130));}
  rect(16,239,(int)(448*p),3,UI_GOLD);
  text_px(208,200,UI_CYAN,"DETENTION / REVIEW");
  text_px(208,215,WHITE,"TIME SERVED...");
 }
}
static void police_dialog(void){
 if(!game.police_stop)return;
 int saved_row=row;dialogue_begin("LOCAL LAW");
 if(game.police_phase>=6){
  int seized=game.police_phase==6,surrender=game.police_phase==8;
  dialogue_speech("LOCAL LAW",LAW,VOICE_LAW*37,
   surrender?"Voluntary cargo transfer confirmed. The cargo charge is waived; your local record is clear.":
   seized?"Your detention is complete. With no funds to settle, your ship, cargo and fitted equipment were seized.":
   "Your detention is complete. The release fee was paid on admission. Any restricted goods were confiscated.",
   surrender?"Patrols are standing down. Your legal cargo, ship and funds remain yours.":
   seized?"A basic Adder and fuel have been assigned to your station berth. Your local warrant is cleared. You may rebuild from here.":
   "Your ship, legal cargo and remaining equipment are waiting at the station. Your local warrant is cleared. Welcome back, Commander.");
  dialogue_context(surrender?"CARGO RECEIPT":"RELEASE PAPERS",surrender?"Other systems keep their own records. Confirm when ready to resume flight.":"Station security has returned your flight licence. Confirm when ready to return to your ship.");
  row=0;dialogue_reply(0,surrender?"Understood. Resume flight.":"Collect my release papers and return.");
  footer("X ACKNOWLEDGE   FLIGHT PAUSED");
 }else if(game.police_phase>=2){
  int transfer=game.police_phase==2||game.police_phase==4,unpaid=game.police_phase==2||game.police_phase==3;
  dialogue_speech("LOCAL LAW",LAW,VOICE_LAW*37,
   transfer?"Power down. A security shuttle is transferring you to the station detention wing.":"The cell lights dim. Beyond the door, a clerk reviews your record. Time passes...",
   unpaid?"No funds were available. Property seizure will settle the local warrant; a basic ship will be assigned on release.":"Your release fee is reserved. After processing and time served, your ship will be returned minus restricted cargo.");
  dialogue_context("CUSTODY",transfer?"ESCORT TRANSFER / LOCAL STATION":"DETENTION REVIEW / RELEASE AUTHORISATION");
  footer("CUSTODY IN PROGRESS   FLIGHT PAUSED");police_custody_animation();
 }else{
  int scan=game.police_phase==1,count=cargo_contraband(&game)>0?4:3;
  int selected=police_choice;if(selected<0||selected>=count)selected=0;
  char detail[512];if(scan)snprintf(detail,sizeof(detail),"This is a hold inspection. Submit to a scan, or transfer any restricted goods voluntarily. Refusing or fleeing creates a warrant.");else police_charge_details(&game,detail,sizeof(detail));
  dialogue_speech("LOCAL LAW",LAW,VOICE_LAW*37,scan?"Commander, Local Law here. Reduce power and stand by for a cargo inspection.":police_accusation(&game),detail);
  dialogue_context("LOCAL WARRANT",game.message_time>0?game.message:cargo_contraband(&game)>0?"Surrender waives the cargo charge only. Other offences remain.":"Pay the fine, accept custody, or run. Custody without funds means property seizure.");
  char custody[96];snprintf(custody,sizeof(custody),game.credits<=0?"Custody: ship and property will be seized":"Accept custody / fee %.1f units",fminf(game.credits,police_fine(&game)/2)*.1f);
  const char *options[]={scan?"Submit to scan":(cargo_contraband(&game)>0?"Pay fine; surrender restricted goods":"Pay the fine and settle my warrant"),scan?"Refuse inspection":custody,"Run from law","Surrender illegal cargo voluntarily"};
  row=selected;
  for(int i=0;i<count;i++)dialogue_reply(i,options[i]);
  footer("UP/DOWN   X CONFIRM   FLIGHT PAUSED");
 }
 row=saved_row;
}
static void death_effect(void){if(!game.dead)return;float age=game.explosion;
 rect(0,0,W,H,RGB(2,5,10));
 /* Keep a fixed camera in the wreck's local space. Sparse stars and slowly
  * separating hull plates make this a witnessed event, not a result card. */
 for(int i=0;i<58;i++){unsigned h=(unsigned)(i*2654435761u+game.system*977u);int x=(int)(h%W),y=(int)((h>>10)%H);unsigned c=(i%7==0&&age<2.2f)?RGB(102,126,150):RGB(40,52,66);pixel(x,y,c);}
 const Mesh *m=&meshes[mesh_id(player_ships[game.ship].name)];
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];Vec3 center=mul(add(add(m->v[t->a],m->v[t->b]),m->v[t->c]),1.f/3);Vec3 dir=norm(add(center,(Vec3){sinf(i*4.f)*25,cosf(i*2.f)*25,20}));Vec3 shift=add((Vec3){0,0,340},mul(dir,(1.f-expf(-age*.7f))*135));int indices[3]={t->a,t->b,t->c};Point p[3];int visible=1;for(int j=0;j<3;j++){Vec3 v=add(shift,rotate(mul(m->v[indices[j]],1.5f),age*.42f,age*.18f));if(v.z<15){visible=0;break;}p[j]=project(v);}if(!visible)continue;for(int j=0;j<3;j++){Point a=p[j],b=p[(j+1)%3];if(a.y>=28&&a.y<218&&b.y>=28&&b.y<218)line((int)a.x,(int)a.y,(int)b.x,(int)b.y,i%3?GOLD:RED);}}
 float burst=fminf(1.f,age/.9f),fade=fmaxf(0,1.f-(age-1.2f)/2.8f);int cx=240,cy=122;
 for(int i=0;i<28;i++){float a=i*2.399963f+age*.18f,r=(12+i%5*4)+burst*(34+i*3);int x=cx+(int)(cosf(a)*r),y=cy+(int)(sinf(a)*r*.58f);if(y>26&&y<220){unsigned c=(i+((int)(age*8)))%3?RGB(244,103,56):GOLD;if(fade>.25f)line(cx+(int)(cosf(a)*r*.35f),cy+(int)(sinf(a)*r*.2f),x,y,c);}}
 if(age<3.5f){int ring=8+(int)(burst*34);circle(cx,cy,ring,age<.45f?WHITE:GOLD);if(age<1.4f)circle(cx,cy,ring/2,WHITE);}
}
static void death_shutdown_overlay(void){
 if(!game.dead)return;float age=game.explosion;
 /* The cockpit loses scan lines, then whole powered sections. Rectangles are
  * deliberately opaque: this is electrical dropout, not screen decoration. */
 if(age<2.55f){
  int phase=(int)(age*18.f);for(int i=0;i<4+(int)(age*3);i++){int y=(phase*17+i*43)%H,h=1+((phase+i)&3);int reach=35+(int)(age*62.f);if((i+phase)&1)rect(0,y,fminf(W,reach+i*19),h,RGB(1,3,7));else rect(fmaxf(0,W-reach-i*23),y,fminf(W,reach+i*23),h,RGB(1,3,7));}
  if(age>.65f){int cut=(int)((age-.65f)*58.f);rect(0,0,fminf(W,cut),24,RGB(1,3,7));rect(fmaxf(0,W-cut),0,fminf(W,cut),24,RGB(1,3,7));}
  if(age>1.15f){int cut=(int)((age-1.15f)*110.f);rect(0,192,fminf(W,cut),80,RGB(1,3,7));rect(fmaxf(0,W-cut),192,fminf(W,cut),80,RGB(1,3,7));}
  if(((int)(age*13.f)&3)==0){int y=28+((int)(age*173.f)%145);rect(0,y,W,2,RGB(92,18,28));line(22,y+2,438,y+2,RGB(34,112,122));}
 }
 if(age>=2.15f){int dark=(int)fminf(80.f,(age-2.15f)*110.f);for(int y=0;y<H;y+=4)if(((y/4)+(int)(age*8))%3<2)rect(0,y,W,dark>45?3:1,RGB(1,3,7));}
 if(age<2.6f)return;
 rect(91,77,298,119,RGB(5,9,15));rect(91,77,298,2,RGB(152,55,51));rect(91,194,298,2,RGB(47,62,73));
 text_px(184,89,RED,"SHIP DESTROYED");text_px(137,104,RGB(142,155,166),"COCKPIT POWER AND TELEMETRY LOST");
 const char *choice[2]={"LOAD COMMANDER","MAIN MENU"};
 for(int i=0;i<2;i++){int y=124+i*27,sel=death_choice==i;rect(119,y,242,21,sel?RGB(112,36,38):RGB(17,25,34));rect(119,y,242,1,sel?RGB(255,111,92):RGB(64,77,88));if(sel){line(124,y+5,124,y+15,RGB(255,174,91));line(125,y+10,131,y+10,RGB(255,174,91));}text_px(151,y+7,sel?WHITE:RGB(151,163,173),"%s",choice[i]);}
 if(death_notice_time>0&&death_notice[0])text_px(151,181,RED,"%.30s",death_notice);
 button_icon(126,203,'U',UI_MUTED);button_icon(139,203,'D',UI_MUTED);text_px(154,206,UI_MUTED,"CHOOSE");button_icon(215,203,'X',UI_SIGNAL);text_px(230,206,UI_MUTED,"CONFIRM");
}
/* Small, clipped effects anchored to physical contacts; no extra HUD labels. */
static void freight_effects(void){
 for(int i=8;i<36;i+=12){
  NPC *n=&game.npc[i];if(!n->alive||!n->freighter||occluded(n->pos))continue;
  if(n->freight_state!=FREIGHT_ARRIVING&&n->freight_state!=FREIGHT_CHARGING)continue;
  if(n->freight_state==FREIGHT_CHARGING&&game.freight_gap>0)continue;
  Vec3 v=camera(&game,n->pos);if(v.z<50||v.z>22000)continue;Point p=project(v);
  float phase=n->freight_state==FREIGHT_ARRIVING?n->freight_timer/2.5f:1-n->freight_timer/3.f;
  int radius=(int)fminf(64,fmaxf(3,freight_extent(n).z*280/v.z*(.4f+phase)));
  for(int k=0;k<32;k++){float a=k*.1963495f;world_spark((int)p.x+(int)(cosf(a)*radius),(int)p.y+(int)(sinf(a)*radius*.45f),1,RGB(105,169,196));}
  world_spark((int)p.x,(int)p.y,2+(int)(phase*5),RGB(191,216,220));
 }
}
static void mining_effects(void){
 for(int i=0;i<DEBRIS_COUNT;i++){Debris *d=&game.debris[i];if(!d->alive||d->flash<=0||occluded(d->pos))continue;
  Vec3 v=camera(&game,d->pos);if(v.z<30||v.z>2400)continue;Point p=project(v);
  float age=(d->rock?.22f:.65f)-d->flash;
  for(int k=0;k<9;k++){float a=k*2.399963f+i,r=(d->rock?7:12)+age*(d->rock?35:90);
   world_spark((int)(p.x+cosf(a)*r),(int)(p.y+sinf(a)*r*.65f),1,k&1?GOLD:RGB(169,192,198));}
 }
}

/* Bounded world-space line geometry, clipped before projection. */
static void discovery_edge(Vec3 centre,Vec3 a,Vec3 b,unsigned ink){
 a=camera(&game,add(centre,a));b=camera(&game,add(centre,b));
 if(a.z<=30&&b.z<=30)return;
 if(a.z<30){float t=(30-a.z)/(b.z-a.z);a=add(a,mul(sub(b,a),t));}
 if(b.z<30){float t=(30-b.z)/(a.z-b.z);b=add(b,mul(sub(a,b),t));}
 Point p=project(a),q=project(b);float dx=q.x-p.x,dy=q.y-p.y,lo=0,hi=1;
 float ds[4]={-dx,dx,-dy,dy},es[4]={p.x,W-1-p.x,p.y-view_top(),view_bot()-p.y};
 for(int k=0;k<4;k++){if(fabsf(ds[k])<.00001f){if(es[k]<0)return;continue;}float t=es[k]/ds[k];if(ds[k]<0)lo=fmaxf(lo,t);else hi=fminf(hi,t);if(lo>hi)return;}
 int steps=(int)fminf(600,ceilf(fmaxf(fabsf(dx),fabsf(dy))*(hi-lo)));if(steps<1)steps=1;
 for(int k=0;k<=steps;k++){float t=lo+(hi-lo)*k/steps,z=1.f/((1-t)/a.z+t/b.z);int x=(int)(p.x+dx*t),y=(int)(p.y+dy*t);if(x<0||x>=W||y<view_top()||y>view_bot())continue;
  unsigned depth=surface_depth_on?surface_encode_depth(1.f/z):mega_encode_depth(z);if((surface_depth_on||mega_depth_valid)&&depth>surface_depth[y*W+x]+8)continue;pixel(x,y,ink);
 }
}
static void discovery_dot(Vec3 centre,Vec3 local,int radius,unsigned ink){Vec3 v=camera(&game,add(centre,local));if(v.z<30)return;Point p=project(v);unsigned depth=surface_depth_on?surface_encode_depth(1.f/v.z):mega_encode_depth(v.z);for(int yy=-radius;yy<=radius;yy++)for(int xx=-radius;xx<=radius;xx++){int x=(int)p.x+xx,y=(int)p.y+yy;if(x<0||x>=W||y<view_top()||y>view_bot())continue;if((surface_depth_on||mega_depth_valid)&&depth>surface_depth[y*W+x]+8)continue;pixel(x,y,ink);}}
static Vec3 discovery_yaw(Vec3 p,float yaw){static float angle,s=0,c=1;if(yaw!=angle){angle=yaw;s=sinf(yaw);c=cosf(yaw);}return (Vec3){p.x*c+p.z*s,p.y,-p.x*s+p.z*c};}
static void discovery_rot_edge(Vec3 centre,Vec3 a,Vec3 b,float yaw,unsigned ink){discovery_edge(centre,discovery_yaw(a,yaw),discovery_yaw(b,yaw),ink);}
static void discovery_box(Vec3 c,Vec3 p,float x,float y,float z,float yaw,unsigned ink,int damaged){
 Vec3 q[8];for(int i=0;i<8;i++)q[i]=add(p,(Vec3){(i&1?1:-1)*x,(i&2?1:-1)*y,(i&4?1:-1)*z});
 for(int i=0;i<8;i++)for(int bit=1;bit<=4;bit*=2)if(!(i&bit)&&(!damaged||((i*3+bit)&7)!=3))discovery_rot_edge(c,q[i],q[i|bit],yaw,ink);
}
static void discovery_solid_quad(Vec3 c,Vec3 a,Vec3 b,Vec3 d,Vec3 e,float yaw,unsigned ink){a=camera(&game,add(c,discovery_yaw(a,yaw)));b=camera(&game,add(c,discovery_yaw(b,yaw)));d=camera(&game,add(c,discovery_yaw(d,yaw)));e=camera(&game,add(c,discovery_yaw(e,yaw)));queue_triangle(a,b,d,ink);queue_triangle(a,d,e,ink);}
static __attribute__((noinline,optimize("Os"))) void discovery_art_quad(Vec3 c,Vec3 a,Vec3 b,Vec3 d,Vec3 e,float yaw,unsigned ink,int tile){a=camera(&game,add(c,discovery_yaw(a,yaw)));b=camera(&game,add(c,discovery_yaw(b,yaw)));d=camera(&game,add(c,discovery_yaw(d,yaw)));e=camera(&game,add(c,discovery_yaw(e,yaw)));int first=drawcount;queue_triangle(a,b,d,ink);int material=5036+(tile+(((int)(game.time*4.f)&1)*4))*6;if(drawcount==first+1)drawlist[first].material=material;first=drawcount;queue_triangle(a,d,e,ink);if(drawcount==first+1)drawlist[first].material=material+1;}
static void discovery_freight_quad(Vec3 c,Vec3 a,Vec3 b,Vec3 d,Vec3 e,float yaw,unsigned ink,int tile){a=camera(&game,add(c,discovery_yaw(a,yaw)));b=camera(&game,add(c,discovery_yaw(b,yaw)));d=camera(&game,add(c,discovery_yaw(d,yaw)));e=camera(&game,add(c,discovery_yaw(e,yaw)));int first=drawcount,material=5000+tile*6;queue_triangle(a,b,d,ink);if(drawcount==first+1)drawlist[first].material=material;first=drawcount;queue_triangle(a,d,e,ink);if(drawcount==first+1)drawlist[first].material=material+1;}
static void discovery_wreck_quad(Vec3 c,Vec3 a,Vec3 b,Vec3 d,Vec3 e,float yaw,unsigned ink,int tile){a=camera(&game,add(c,discovery_yaw(a,yaw)));b=camera(&game,add(c,discovery_yaw(b,yaw)));d=camera(&game,add(c,discovery_yaw(d,yaw)));e=camera(&game,add(c,discovery_yaw(e,yaw)));Vec3 ab=sub(b,a),ad=sub(d,a);float facing=(ab.y*ad.z-ab.z*ad.y)*a.x+(ab.z*ad.x-ab.x*ad.z)*a.y+(ab.x*ad.y-ab.y*ad.x)*a.z;if(facing<=0)return;int first=drawcount,material=5100+tile*6;queue_triangle(a,b,d,ink);if(drawcount==first+1)drawlist[first].material=material;first=drawcount;queue_triangle(a,d,e,ink);if(drawcount==first+1)drawlist[first].material=material+1;}
static void discovery_wreck_box(Vec3 c,Vec3 p,float x,float y,float z,float yaw,unsigned ink,int side,int top,int front){
 Vec3 q[8];for(int i=0;i<8;i++)q[i]=add(p,(Vec3){(i&1?1:-1)*x,(i&2?1:-1)*y,(i&4?1:-1)*z});
 discovery_wreck_quad(c,q[0],q[1],q[3],q[2],yaw,livery_tint(ink,-18),top);discovery_wreck_quad(c,q[4],q[6],q[7],q[5],yaw,livery_tint(ink,18),top);
 discovery_wreck_quad(c,q[0],q[4],q[5],q[1],yaw,livery_tint(ink,-8),front);discovery_wreck_quad(c,q[2],q[3],q[7],q[6],yaw,livery_tint(ink,12),front);
 discovery_wreck_quad(c,q[0],q[2],q[6],q[4],yaw,livery_tint(ink,-12),side);discovery_wreck_quad(c,q[1],q[5],q[7],q[3],yaw,livery_tint(ink,5),side);
}
static void discovery_solid_box(Vec3 c,Vec3 p,float x,float y,float z,float yaw,unsigned ink,int damaged){
 Vec3 q[8];for(int i=0;i<8;i++)q[i]=add(p,(Vec3){(i&1?1:-1)*x,(i&2?1:-1)*y,(i&4?1:-1)*z});
 discovery_solid_quad(c,q[0],q[1],q[3],q[2],yaw,livery_tint(ink,15));discovery_solid_quad(c,q[4],q[6],q[7],q[5],yaw,livery_tint(ink,-24));
 discovery_solid_quad(c,q[0],q[4],q[5],q[1],yaw,livery_tint(ink,-8));if(!damaged)discovery_solid_quad(c,q[2],q[3],q[7],q[6],yaw,livery_tint(ink,24));
 discovery_solid_quad(c,q[0],q[2],q[6],q[4],yaw,livery_tint(ink,-18));discovery_solid_quad(c,q[1],q[5],q[7],q[3],yaw,livery_tint(ink,5));
}
static void discovery_solid_freighter(Vec3 c,int slot){
 float yaw=.35f+(discovery_seed(slot)&255)*.004f;unsigned hull=RGB(63,79,85);
 const float z[6]={-2700,-1850,-800,350,1450,2350},w[6]={360,760,940,900,620,240},h[6]={310,520,610,570,420,190};
 /* Every hull zone now carries art made for its physical job.  Port section 2
  * is absent, exposing a dark structural spine rather than a painted hole. */
 static const unsigned char side[5]={4,3,6,2,1},top[5]={8,3,5,11,10},bottom[5]={4,7,5,2,1};
 for(int s=0;s<5;s++){Vec3 a={-w[s],-h[s],z[s]},b={w[s],-h[s],z[s]},d={w[s+1],-h[s+1],z[s+1]},e={-w[s+1],-h[s+1],z[s+1]};discovery_wreck_quad(c,a,b,d,e,yaw,livery_tint(hull,-30),bottom[s]);a=(Vec3){-w[s],h[s],z[s]};b=(Vec3){-w[s+1],h[s+1],z[s+1]};d=(Vec3){w[s+1],h[s+1],z[s+1]};e=(Vec3){w[s],h[s],z[s]};discovery_wreck_quad(c,a,b,d,e,yaw,livery_tint(hull,34),top[s]);if(s!=2){a=(Vec3){-w[s],-h[s],z[s]};b=(Vec3){-w[s+1],-h[s+1],z[s+1]};d=(Vec3){-w[s+1],h[s+1],z[s+1]};e=(Vec3){-w[s],h[s],z[s]};discovery_wreck_quad(c,a,b,d,e,yaw,livery_tint(hull,-20),side[s]);}a=(Vec3){w[s],-h[s],z[s]};b=(Vec3){w[s],h[s],z[s]};d=(Vec3){w[s+1],h[s+1],z[s+1]};e=(Vec3){w[s+1],-h[s+1],z[s+1]};discovery_wreck_quad(c,a,b,d,e,yaw,livery_tint(hull,14),s==2?6:side[s]);}
 /* Raised command decks, paired cargo blisters and machinery have their own
  * face art, so the silhouette and the pixels describe the same object. */
 discovery_wreck_box(c,(Vec3){0,760,-250},520,190,440,yaw,RGB(70,94,101),1,10,0);
 discovery_wreck_box(c,(Vec3){0,1030,-110},210,110,210,yaw,RGB(52,75,85),10,10,0);
 for(int e=-1;e<=1;e+=2){discovery_wreck_box(c,(Vec3){e*705,80,650},185,340,520,yaw,RGB(61,78,84),2,11,9);discovery_wreck_box(c,(Vec3){e*420,-120,-2520},250,250,270,yaw,RGB(48,62,69),4,3,4);}
 discovery_wreck_box(c,(Vec3){-900,-40,-230},90,250,620,yaw,RGB(54,67,71),5,5,5);
 if(render_reference||render_pixel_radius(c,3500)>40.f)for(int k=0;k<16;k++){float drift=game.time*(.8f+(k%3)*.17f)+k*13.f;Vec3 p={(float)((k*719)%3000-1500)+sinf(drift)*40,(float)((k*431)%1800-900)+cosf(drift*.7f)*30,(float)((k*997)%6200-3100)};float s=18+(k%5)*9;discovery_solid_box(c,p,s,s*.45f,s*1.8f,yaw+drift*.01f,k%4?RGB(47,58,62):RGB(104,52,38),1);}
}
static void listening_dish_angles(int q,float *turn,float *tilt){
 float speed=.19f+q*.047f,phase=game.time*speed+q*2.17f;
 /* Flatten the ends of the sine wave into short listening holds. */
 float sweep=sinf(phase);if(sweep>.82f)sweep=.82f;if(sweep<-.82f)sweep=-.82f;
 *turn=sweep*(.46f-q*.045f)+(q-1)*.12f;
 *tilt=.12f+sinf(phase*.61f+q)*(.12f+q*.025f);
}
static Vec3 listening_dish_point(Vec3 centre,Vec3 local,float turn,float tilt){
 static float last_tilt,last_turn,ct,st,cy,sy;static int ready;
 if(!ready||last_tilt!=tilt||last_turn!=turn){last_tilt=tilt;last_turn=turn;ct=cosf(tilt);st=sinf(tilt);cy=cosf(turn);sy=sinf(turn);ready=1;}
 Vec3 p={local.x,local.y*ct-local.z*st,local.y*st+local.z*ct};
 return add(centre,(Vec3){p.x*cy+p.z*sy,p.y,-p.x*sy+p.z*cy});
}
static __attribute__((noinline,optimize("Os"))) void discovery_solid_listening_post(Vec3 c){
 const float yaw=.18f;unsigned hull=RGB(48,57,66),panel=RGB(67,75,82),warm=RGB(190,126,48),dish=RGB(75,84,91);
 /* Lantern Array: inhabited operations hub, lower machinery lantern and
  * three deliberately broad receiver dishes.  Large colour blocks survive
  * the PSP's native resolution; fine structure is supplied by the line pass. */
 discovery_solid_box(c,(Vec3){0,0,0},430,300,430,yaw,hull,0);
 discovery_solid_box(c,(Vec3){0,390,0},250,105,250,yaw,panel,0);
 discovery_solid_box(c,(Vec3){0,-555,0},260,190,260,yaw,livery_tint(hull,-12),0);
 discovery_solid_box(c,(Vec3){0,-900,0},120,170,120,yaw,livery_tint(hull,-22),0);
 discovery_solid_box(c,(Vec3){-720,40,0},290,42,42,yaw,panel,0);
 discovery_solid_box(c,(Vec3){720,40,0},290,42,42,yaw,panel,0);
 discovery_solid_box(c,(Vec3){0,95,720},42,42,290,yaw,panel,0);
 /* Whole-face pixel plates sit a fraction above the structural boxes.  They
  * are mapped to the shape rather than painted over the final framebuffer. */
 discovery_art_quad(c,(Vec3){-420,-285,-438},(Vec3){420,-285,-438},(Vec3){420,285,-438},(Vec3){-420,285,-438},yaw,hull,0);
 discovery_art_quad(c,(Vec3){420,-285,438},(Vec3){-420,-285,438},(Vec3){-420,285,438},(Vec3){420,285,438},yaw,hull,0);
 discovery_art_quad(c,(Vec3){-438,-285,420},(Vec3){-438,-285,-420},(Vec3){-438,285,-420},(Vec3){-438,285,420},yaw,hull,0);
 discovery_art_quad(c,(Vec3){438,-285,-420},(Vec3){438,-285,420},(Vec3){438,285,420},(Vec3){438,285,-420},yaw,hull,0);
 discovery_art_quad(c,(Vec3){-268,-735,-268},(Vec3){268,-735,-268},(Vec3){268,-375,-268},(Vec3){-268,-375,-268},yaw,hull,1);
 discovery_art_quad(c,(Vec3){268,-735,268},(Vec3){-268,-735,268},(Vec3){-268,-375,268},(Vec3){268,-375,268},yaw,hull,1);
 discovery_art_quad(c,(Vec3){-268,-735,268},(Vec3){-268,-735,-268},(Vec3){-268,-375,-268},(Vec3){-268,-375,268},yaw,hull,1);
 discovery_art_quad(c,(Vec3){268,-735,-268},(Vec3){268,-735,268},(Vec3){268,-375,268},(Vec3){268,-375,-268},yaw,hull,1);
 discovery_art_quad(c,(Vec3){-1008,-5,-48},(Vec3){-432,-5,-48},(Vec3){-432,85,-48},(Vec3){-1008,85,-48},yaw,panel,2);
 discovery_art_quad(c,(Vec3){432,-5,-48},(Vec3){1008,-5,-48},(Vec3){1008,85,-48},(Vec3){432,85,-48},yaw,panel,2);
 const Vec3 dc[3]={{-1040,300,0},{1040,430,0},{0,570,1040}};
 const float dr[3]={520,620,470};
 for(int q=0;q<3;q++){
  float turn,tilt;listening_dish_angles(q,&turn,&tilt);if(q==2)turn+=3.141593f;
  Vec3 focus=listening_dish_point(dc[q],(Vec3){0,0,250},turn,tilt);
  int sweep=((int)(game.time*(1.4f+q*.37f))+q*4)%12;
  for(int k=0;k<12;k++){float a=k*.523599f,b=(k+1)*.523599f;Vec3 p=listening_dish_point(dc[q],(Vec3){cosf(a)*dr[q],sinf(a)*dr[q],0},turn,tilt);Vec3 n=listening_dish_point(dc[q],(Vec3){cosf(b)*dr[q],sinf(b)*dr[q],0},turn,tilt);unsigned skin=k==sweep?RGB(78,137,145):(k&1?dish:livery_tint(dish,12));discovery_solid_quad(c,dc[q],p,n,dc[q],yaw,skin);discovery_solid_quad(c,p,focus,n,p,yaw,k==sweep?RGB(81,185,193):(k&1?livery_tint(dish,-18):livery_tint(dish,-8)));}
 }
 /* The texture carries the observation windows; one inset amber service hatch
  * keeps the interaction point readable even after the texture mips away. */
 discovery_solid_quad(c,(Vec3){-110,-80,-442},(Vec3){110,-80,-442},(Vec3){110,70,-442},(Vec3){-110,70,-442},yaw,warm);
}
#include "discovery-detail.h"
#include "encounter-custom.h"
static uint64_t encounter_model_us[DISCOVERY_COUNT],encounter_effect_us[DISCOVERY_COUNT],encounter_flush_us;
static void discovery_solid_pass(void){
 for(int i=0;i<discovery_count();i++){Vec3 c=discovery_position(i),v=camera(&game,c);int entry=discovery_entry(i);if(v.z<-2700||length(v)>65000||!render_sphere_visible(c,entry==61?4600.f:2700.f)||occluded(c))continue;unsigned ink=RGB(46+(entry%3)*9,71+(entry%4)*8,77+(entry%2)*10);float yaw=(discovery_seed(i)&255)*.006f;
  uint64_t stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
  if(entry==61)discovery_solid_freighter(c,i);else if(entry==60)discovery_solid_listening_post(c);else if(!encounter_custom(c,entry,yaw))discovery_catalogue_model(c,entry,yaw);
  if(render_profile_enabled)encounter_model_us[i]+=sceKernelGetSystemTimeWide()-stamp;
 }
 uint64_t stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;flush_meshes();if(render_profile_enabled)encounter_flush_us+=sceKernelGetSystemTimeWide()-stamp;
}
static unsigned discovery_fog_mix(unsigned old,unsigned ink,int alpha){
 int inv=255-alpha;return RGB(((old&255)*inv+(ink&255)*alpha)/255,(((old>>8)&255)*inv+((ink>>8)&255)*alpha)/255,(((old>>16)&255)*inv+((ink>>16)&255)*alpha)/255);
}
static void discovery_wreck_fog(Vec3 centre,int foreground){
 /* A local debris-nebula, built from deterministic soft clouds. Rear layers
  * establish the volume; a few weak front wisps pass across the wreck. */
 static const unsigned ink[4]={0x00504438,0x00604468,0x00485e68,0x00645c48};
 /* Reserved for close fly-through motes; broad fog below carries the scene at
  * a fraction of the pixel cost. */
 for(int k=0;k<0;k++){
  unsigned h=2166136261u^(unsigned)k*16777619u;
  float z=(float)((int)((h>>18)&8191)-4096),x=(float)((int)(h&8191)-4096),y=(float)((int)((h>>9)&4095)-2048);
  int front=z<0;if(front!=foreground)continue;
  float drift=game.time*(.025f+(k%5)*.004f)+k*1.71f;
  Vec3 world=add(centre,(Vec3){x+sinf(drift)*180,y+cosf(drift*.73f)*110,z});Vec3 q=camera(&game,world);if(q.z<100)continue;
  Point p=project(q);float radius=(900.f+(h%1250))*proj_focal/q.z;if(radius<3)continue;if(radius>95)radius=95;
  int r=(int)radius,x0=(int)p.x-r,x1=(int)p.x+r,y0=(int)p.y-r,y1=(int)p.y+r;
  if(x0<clipx0)x0=clipx0;if(x1>=clipx1)x1=clipx1-1;if(y0<view_top())y0=view_top();if(y1>=view_bot())y1=view_bot()-1;
  float invr=1.f/(radius*radius);int peak=foreground?24:64+(k%4)*8;
  for(int sy=y0;sy<=y1;sy+=2)for(int sx=x0;sx<=x1;sx+=2){
   float dx=sx-p.x,dy=(sy-p.y)*1.28f,d=(dx*dx+dy*dy)*invr;if(d>=1)continue;
   unsigned grain=(unsigned)(sx*73856093u)^(unsigned)(sy*19349663u)^h;float edge=(1-d);int alpha=(int)(peak*edge*edge*(.68f+(grain&31)/100.f));
   if(alpha>0){unsigned c=ink[(h>>5)&3];for(int yy=0;yy<2&&sy+yy<=y1;yy++)for(int xx=0;xx<2&&sx+xx<=x1;xx++)fb[(sy+yy)*STRIDE+sx+xx]=discovery_fog_mix(fb[(sy+yy)*STRIDE+sx+xx],c,alpha);}
  }
 }
 /* A guaranteed uneven envelope at encounter distance. World-space motes
  * provide parallax; these broad pockets make the cloud readable on PSP. */
 Vec3 core=camera(&game,centre);if(core.z>100){Point cp=project(core);float extent=render_pixel_radius(centre,5200.f);if(extent>230)extent=230;if(extent<35)extent=35;
  int clouds=foreground?3:10,samples=foreground?80:210;for(int k=0;k<clouds;k++){float a=k*6.2831853f/clouds+.22f*sinf(game.time*.018f+k),px=cp.x+cosf(a)*extent*.78f,py=cp.y+sinf(a)*extent*.42f;float radius=extent*(.24f+(k%3)*.055f);int peak=foreground?10:78+(k&1)*18;
   for(int n=0;n<samples;n++){unsigned grain=(unsigned)(k*2246822519u+n*3266489917u);float dx=((grain&255)-127.5f)*radius/127.5f,dy=(((grain>>8)&255)-127.5f)*radius/(127.5f*1.35f),d=(dx*dx+dy*dy)/(radius*radius);if(d>=1)continue;int sx=(int)(px+dx),sy=(int)(py+dy);if(sx<clipx0||sx+2>=clipx1||sy<view_top()||sy+2>=view_bot())continue;float edge=1-d;int alpha=(int)(peak*edge*edge*(.78f+((grain>>16)&15)/90.f));unsigned c=discovery_fog_mix(fb[sy*STRIDE+sx],ink[k&3],alpha);for(int yy=0;yy<3;yy++)for(int xx=0;xx<3;xx++)if(((xx+yy+(grain>>20))&3)!=0)fb[(sy+yy)*STRIDE+sx+xx]=c;}
  }
 }
}
static void discovery_wreck_beams(Vec3 centre){
 static const Vec3 root[3]={{-520,180,-1850},{430,520,-500},{650,-120,1050}};
 static const Vec3 reach[3]={{-3900,300,-3400},{2600,3100,-900},{4300,-1300,2600}};
 for(int b=0;b<3;b++){Vec3 a=camera(&game,add(centre,root[b])),z=camera(&game,add(centre,reach[b]));if(a.z<50||z.z<50)continue;Point p=project(a),q=project(z);int steps=42;float dx=q.x-p.x,dy=q.y-p.y,len=sqrtf(dx*dx+dy*dy);if(len<1)continue;float nx=-dy/len,ny=dx/len;
  for(int n=0;n<=steps;n++){float t=n/(float)steps,cx=p.x+dx*t,cy=p.y+dy*t;int half=1+(int)(t*11);for(int o=-half;o<=half;o+=2){float edge=1-fabsf(o)/(half+1.f);int alpha=(int)(((1-t)*38+10)*edge);int x=(int)(cx+nx*o),y=(int)(cy+ny*o);if(x>=clipx0&&x<clipx1&&y>=view_top()&&y<view_bot())fb[y*STRIDE+x]=discovery_fog_mix(fb[y*STRIDE+x],b==1?RGB(96,174,191):RGB(135,165,178),alpha);}}
 }
}
static void discovery_wreck_smoke(Vec3 centre,int slot,float yaw){
 static const Vec3 vent[3]={{-620,120,-720},{470,-80,-2380},{760,180,620}};
 static const Vec3 flow[3]={{-540,310,-180},{280,-190,-620},{610,260,240}};
 static const unsigned tint[3]={0x00878a82,0x007c6685,0x00879570};
 for(int e=0;e<3;e++)for(int n=0;n<8;n++){
  float age=fmodf(game.time*(.115f+e*.012f)+n*.127f+e*.31f,1.f),curl=sinf(age*7.2f+n*2.1f+slot)*120.f;
  Vec3 p=add(vent[e],add(mul(flow[e],age),(Vec3){curl,cosf(age*5+n)*70.f,0}));p=discovery_yaw(p,yaw);Vec3 q=camera(&game,add(centre,p));if(q.z<50)continue;Point s=project(q);float radius=(65+age*230)*proj_focal/q.z;if(radius<1)radius=1;if(radius>22)radius=22;int r=(int)radius,x0=(int)s.x-r,x1=(int)s.x+r,y0=(int)s.y-r,y1=(int)s.y+r;if(x0<clipx0)x0=clipx0;if(x1>=clipx1)x1=clipx1-1;if(y0<view_top())y0=view_top();if(y1>=view_bot())y1=view_bot()-1;float inv=1.f/(radius*radius);int peak=(int)((1-age)*52);
  for(int y=y0;y<=y1;y+=2)for(int x=x0;x<=x1;x+=2){float dx=x-s.x,dy=(y-s.y)*1.15f,d=(dx*dx+dy*dy)*inv;if(d>=1)continue;unsigned h=(unsigned)(x*31337+y*6971+n*101+e*977);int alpha=(int)(peak*(1-d)*(1-d)*(.72f+(h&15)/70.f));if(alpha<2)continue;unsigned c=discovery_fog_mix(fb[y*STRIDE+x],tint[e],alpha);for(int yy=0;yy<2&&y+yy<=y1;yy++)for(int xx=0;xx<2&&x+xx<=x1;xx++)fb[(y+yy)*STRIDE+x+xx]=c;}
 }
}
static void discovery_wreck(Vec3 c,int slot){
 float yaw=.35f+(discovery_seed(slot)&255)*.004f;unsigned hull=RGB(105,128,137),rib=RGB(64,83,92),scar=RGB(178,91,57);
 /* A 5.4 km broken heavy freighter: tapered armoured sections, ribs, bridge,
  * engines and an exposed central spine. Missing edges form readable hull tears. */
 const float z[6]={-2700,-1850,-800,350,1450,2350},w[6]={360,760,940,900,620,240},h[6]={310,520,610,570,420,190};
 /* Only physical transverse ribs remain as lines. Pixel materials, lighting
  * and missing hull faces describe the outer silhouette. */
 /* Structure and engine housings are now modelled and textured in the solid
  * pass. Do not draw the former wireframe cage, spine or circular engines on
  * top of them. */
 /* Failed navigation and emergency lights flicker independently. */
 for(int k=0;k<18;k++){if(((int)(game.time*(2+(k&3)))+k*7+slot)&7)continue;Vec3 p={(k&1?1:-1)*(430+(k%4)*105),220-((k*37)%420),-1900+k*205};discovery_dot(c,discovery_yaw(p,yaw),k%5==0?2:1,k%3?RGB(237,117,55):RGB(72,178,187));}
 /* Slow fragments and particulate dust make the wreck occupy real volume. */
 for(int k=0;k<16;k++){float drift=game.time*(.8f+(k%3)*.17f)+k*13.f;Vec3 p={(float)((k*719)%3000-1500)+sinf(drift)*40,(float)((k*431)%1800-900)+cosf(drift*.7f)*30,(float)((k*997)%6200-3100)};float s=18+(k%5)*9;discovery_box(c,p,s,s*.45f,s*1.8f,yaw+drift*.01f,k%4?rib:scar,1);}
 for(int k=0;k<34;k++){float t=game.time*.04f+k*17.31f;Vec3 p={(float)((k*811)%4600-2300)+sinf(t)*90,(float)((k*353)%2600-1300)+cosf(t*.8f)*70,(float)((k*613)%7200-3600)};discovery_dot(c,discovery_yaw(p,yaw),k%11==0?1:0,RGB(92,99,98));}
 discovery_wreck_smoke(c,slot,yaw);
}
static __attribute__((noinline,optimize("Os"))) void discovery_listening_post(Vec3 c){
 float yaw=.18f;unsigned edge=RGB(102,142,151),dark=RGB(43,69,78),cyan=RGB(63,190,203),amber=RGB(242,174,68);
 /* Amber practicals sit on the textured hub; solid boundaries come from
  * material and lighting rather than a wireframe box drawn over the art. */
 for(int k=-2;k<=2;k++)discovery_dot(c,discovery_yaw((Vec3){k*125,-18,-447},yaw),1,amber);
 discovery_rot_edge(c,(Vec3){0,-745,0},(Vec3){0,-1150,0},yaw,edge);discovery_rot_edge(c,(Vec3){-90,-1150,0},(Vec3){90,-1150,0},yaw,amber);
 /* Three exposed trusses; repeated diagonals read as authored pixel detail. */
 const Vec3 arm[3]={{-1040,300,0},{1040,430,0},{0,570,1040}};
 for(int q=0;q<3;q++){Vec3 root={q==0?-400:q==1?400:0,q==2?130:40,q==2?400:0};discovery_rot_edge(c,root,arm[q],yaw,edge);for(int n=1;n<5;n++){float t=n*.2f;Vec3 p=add(root,mul(sub(arm[q],root),t));Vec3 o={q==2?100:0,q==2?0:100,q==2?0:100};discovery_rot_edge(c,add(p,o),sub(p,o),yaw,dark);}}
 /* Dish rims, radial ribs and animated feed lights. */
 const float dr[3]={520,620,470};
 for(int q=0;q<3;q++){float turn,tilt;listening_dish_angles(q,&turn,&tilt);if(q==2)turn+=3.141593f;Vec3 focus=listening_dish_point(arm[q],(Vec3){0,0,250},turn,tilt);for(int k=0;k<16;k++){float a=k*.392699f;Vec3 p=listening_dish_point(arm[q],(Vec3){cosf(a)*dr[q],sinf(a)*dr[q],0},turn,tilt);if(!(k&3))discovery_rot_edge(c,p,focus,yaw,dark);}discovery_dot(c,discovery_yaw(focus,yaw),2,((int)(game.time*4+q)&1)?amber:cyan);}
 /* Crown, navigation beacons and a cool status strip oppose the warm hub. */
 discovery_rot_edge(c,(Vec3){0,300,0},(Vec3){0,1500,0},yaw,edge);for(int y=650;y<1450;y+=170)discovery_rot_edge(c,(Vec3){-65,(float)y,0},(Vec3){65,(float)y,0},yaw,dark);
 discovery_dot(c,discovery_yaw((Vec3){0,1530,0},yaw),2,((int)(game.time*3)&1)?amber:RED);for(int k=-3;k<=3;k++)discovery_dot(c,discovery_yaw((Vec3){k*92,185,-445},yaw),1,k&1?cyan:amber);
}
static void discovery_cage(Vec3 c,int sanctuary){
 float yaw=.22f;for(int k=0;k<5;k++){float phase=game.time*.25f+k*2.f;Vec3 p={sinf(phase)*420,-120+k*55,cosf(phase*.7f)*620};discovery_dot(c,discovery_yaw(p,yaw),2,RGB(211,170,101));}
}
static void draw_space_discoveries(void){
 int old0=clipy0,old1=clipy1;clipy0=view_top();clipy1=view_bot();
 for(int i=0;i<discovery_count();i++)if(discovery_entry(i)==61){Vec3 c=discovery_position(i),v=camera(&game,c);if(v.z>-1800&&length(v)<65000&&render_sphere_visible(c,6500.f)&&!occluded(c))discovery_wreck_beams(c);}
 discovery_solid_pass();
 for(int i=0;i<discovery_count();i++){
 Vec3 centre=discovery_position(i),v=camera(&game,centre);
 if(v.z<-1800||length(v)>65000||!render_sphere_visible(centre,discovery_entry(i)==61?4600.f:2200.f)||occluded(centre))continue;
 int entry=discovery_entry(i);unsigned marker=RGB(92+(entry%3)*18,143+(entry%4)*11,153+(entry%2)*22);
 uint64_t stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
 encounter_sprite_details(centre,entry,(discovery_seed(i)&255)*.006f);
 if(entry==60)discovery_listening_post(centre);else if(entry==61)discovery_wreck(centre,i);else if(entry==62)discovery_cage(centre,0);else if(entry==63)discovery_cage(centre,1);
 if(v.z>30){Point q=project(v);if(q.x>0&&q.x<W&&q.y>view_top()&&q.y<view_bot())rect(q.x,q.y,2,2,sinf(game.time*2+i)>0?GOLD:marker);}
  if(render_profile_enabled)encounter_effect_us[i]+=sceKernelGetSystemTimeWide()-stamp;
 }
 clipy0=old0;clipy1=old1;
}
