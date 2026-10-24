/* Presentation state only. No commander/save layout changes. */
static int hc_world_clip_enabled=1,hc_world_raster_clip=0;
static int hc_space_depth=0,hc_overlay_pending=0;static int hc_cache_dirty=1;static int hc_force_legacy=0;static uint64_t hc_copy_us=0;static uint64_t hc_profile_us[5];static unsigned hc_profile_frames=0;
static int hc_browser=0,hc_category=2,hc_highlight=-1,hc_ids[TARGET_CAPACITY],hc_count=0;
static float hc_refresh=0,hc_open_age=0;
/* Presentation is allowed during the guided launch; manual computers and
   targeting controls still require ordinary free flight. */
static int hc_presentation_context(void){return !hc_force_legacy&&hc_space_depth<2&&page==FLIGHT&&!game.docked&&!game.dead&&game.planet<0&&game.jump<=0&&(game.dock_stage==0||game.dock_stage==4)&&game.approach<0&&!game.police_stop&&!race_is_active(&game);}
static int hc_context(void){return hc_presentation_context()&&!game.dock_stage&&!departure_release;}
static int hc_full_console_visible(void){return hc_presentation_context()&&(paused||hc_browser||tools_open||(hud_mode==0&&!hud_hidden));}
static int hc_detected(int id){
 if(navigation_enabled(&game)&&(id==ROUTE_TARGET_ID||id==RUMOUR_TARGET_ID))return 0;
 if(!target_detected(id))return 0;
 /* Mapped bodies and plotted navigation points keep their existing meaning.
  * All physical station contacts, including primary ID zero, obey scanners. */
 if(IS_STATION_ID(id))return length(sub(target_raw_position(id),game.pos))<=sensor_range()*(navigation_enabled(&game)?1.2f:2.8f);
 return 1;
}
static void hc_validate_lock(void){
 if(!hc_context())return;
 if(selected_target>=0&&!hc_detected(selected_target)){if(navigation_enabled(&game)&&nav_virtual_ready(selected_target)){nav_virtual_target=selected_target;nav_follow=autoaim;}selected_target=-1;target_explicit=0;autoaim=0;sensor_last_id=-1;nav_body=-1;}
}
static void hc_contacts(void){
 int raw[TARGET_CAPACITY],n=0;hc_count=0;
 if(hc_category==5){
  for(int key=0;key<NAV_KEYS;key++)if(navigation_source(&game,key))hc_ids[hc_count++]=NAV_UI_ID+key;
  if(game.navigation.search_key&&game.navigation.search_system==game.system)hc_ids[hc_count++]=NAV_UI_ID+NAV_KEYS;
  for(int id=0;id<=FLIGHT_TARGET_MAX;id++)if(nav_virtual_ready(id)&&valid_target(id))hc_ids[hc_count++]=NAV_VIRTUAL_ID+id;
  int found=0;for(int k=0;k<hc_count;k++)if(hc_ids[k]==hc_highlight)found=1;
  if(!found)hc_highlight=hc_count?hc_ids[0]:-1;hc_refresh=.12f;return;
 }
 n=collect_scan_ids(raw,hc_category);
 float ranges[TARGET_CAPACITY];
 for(int k=0;k<n;k++)if(hc_detected(raw[k])){
  Vec3 p=sub(target_raw_position(raw[k]),game.pos);float range=dot(p,p);int j=hc_count++;
  while(j>0&&(ranges[j-1]>range||(ranges[j-1]==range&&hc_ids[j-1]>raw[k]))){ranges[j]=ranges[j-1];hc_ids[j]=hc_ids[j-1];j--;}
  ranges[j]=range;hc_ids[j]=raw[k];
 }
 int found=0;for(int i=0;i<hc_count;i++)if(hc_ids[i]==hc_highlight)found=1;
 if(!found)hc_highlight=hc_count?hc_ids[0]:-1;
 hc_refresh=.12f;
}
static int hc_browser_hold(unsigned held){
 int was=hc_browser;
 if(!hc_context()||!(held&PSP_CTRL_SQUARE)){hc_browser=0;hc_open_age=0;if(was)hc_cache_dirty=1;return was;}
 if(!comms_quick&&!(held&PSP_CTRL_START)&&!hc_browser){hc_browser=1;hc_open_age=0;hc_refresh=0;hc_highlight=selected_target;hc_contacts();hc_cache_dirty=1;game.cue=SFX_UI;}
 return hc_browser||was;
}
static int hc_browser_input(unsigned pressed,unsigned held,float dt){
 hc_validate_lock();int consumed=hc_browser_hold(held);
 if(!hc_browser||comms_quick||(held&PSP_CTRL_START))return consumed;
 hc_open_age+=fmaxf(0,dt);hc_refresh-=fmaxf(0,dt);
 if(hc_refresh<=0)hc_contacts();
 if(pressed&(PSP_CTRL_LEFT|PSP_CTRL_RIGHT)){hc_category=(hc_category+(pressed&PSP_CTRL_RIGHT?1:5))%6;if(hc_category<5)scan_cat=hc_category;hc_highlight=-1;hc_contacts();game.cue=SFX_SELECT;}
 if(hc_count&&(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))){int at=0;for(int i=0;i<hc_count;i++)if(hc_ids[i]==hc_highlight)at=i;hc_highlight=hc_ids[(at+(pressed&PSP_CTRL_DOWN?1:hc_count-1))%hc_count];game.cue=SFX_SELECT;}
 if(hc_category==5){if((pressed&(PSP_CTRL_CROSS|PSP_CTRL_RTRIGGER))&&nav_ui_record(hc_highlight)){nav_ui_select(hc_highlight);if(pressed&PSP_CTRL_RTRIGGER)nav_follow=1;}return 1;}
 if((pressed&(PSP_CTRL_CROSS|PSP_CTRL_RTRIGGER))&&hc_highlight>=0&&hc_detected(hc_highlight)){lock_local_target(hc_highlight,"Computer");if((pressed&PSP_CTRL_RTRIGGER)||(held&PSP_CTRL_RTRIGGER)){autoaim=1;if(selected_target==0)campaign_event(&game,CP_LOCK);}game.cue=SFX_SELECT;}
 return 1;
}
static int hc_next_weapon_slot(void){
 int capacity=fit_capacity(game.ship,FIT_WPN),bank=game.active_weapon/6;
 for(int k=1;k<=capacity;k++){int slot=((bank+k)%capacity)*6;if(fit_is_active_weapon(game.fit[slot]))return slot;}
 return -1;
}
static const char*hc_tool_reason(int action){
 static char wait[12];
 if(action==1&&game.flare_cd>0){snprintf(wait,sizeof(wait),"WAIT %dS",(int)ceilf(game.flare_cd));return wait;}
 if(action==3&&(game.upgrades&2048)&&game.heat_sink_cd>0){snprintf(wait,sizeof(wait),"WAIT %dS",(int)ceilf(game.heat_sink_cd));return wait;}
 if(action==0)return game.missiles<=0?"NO AMMO":!IS_NPC_ID(selected_target)||!hc_detected(selected_target)?"NO LOCK":length(sub(target_raw_position(selected_target),game.pos))>12000?"TOO FAR":"READY";
 if(action==1)return game.flare_charges<=0?"NO CHARGE":game.flare_cd>0?"COOLING":"READY";
 if(action==2){if(game.tractor_time>0)return "PULLING";if(tools_tractor>=0)return "ALIGNING";if(cargo_used(&game)>=cargo_capacity(&game))return "BAY FULL";for(int i=0;i<DEBRIS_COUNT;i++)if(tools_recoverable(DEBRIS_ID_MIN+i))return "READY";return "NO CARGO";}
 if(action==3)return !(game.upgrades&2048)?"NO UNIT":game.heat_sink_cd>0?"COOLING":game.heat<1?"COOL":"READY";
 return fit_find(&game,LAW_SCANNER_ITEM)<0?"NO UNIT":"READY";
}
