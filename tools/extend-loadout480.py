from pathlib import Path
p=Path('src/dockbay-pages.h');s=p.read_text();s=s.replace('db_save_wait=5;game.dockbay.resume=!game.docked;char note[160];','db_save_wait=5;unsigned previous_resume=game.dockbay.resume;game.dockbay.resume=game.dockbay.ready&&!game.docked;if(!opening_save_allowed(&game)){game.dockbay.resume=previous_resume;return;}char note[160];');s=s.replace('else snprintf(db_notice,sizeof(db_notice),"SAVE FAILED / KEEP THIS SESSION OPEN");','else snprintf(db_notice,sizeof(db_notice),"SAVE FAILED: %.84s / KEEP SESSION OPEN",game.message);');p.write_text(s)
p=Path('src/game.c');s=p.read_text();s=s.replace('int save_game(Game *g,const char *path){if(!opening_save_allowed(g))return 0;','int save_game(Game *g,const char *path){if(!opening_save_allowed(g)){message(g,"Dock at a station to save a checkpoint.");return 0;}');s=s.replace('if(ok)ok=save_seal(tmp);if(ok)ok=save_commit(path,tmp,bak,rename);if(ok)g->cue=SFX_SELECT;message(g,ok?"Commander saved.":"Save failed.");return ok;','if(!ok){message(g,"Could not write commander data. Check free space.");return 0;}if(!save_seal(tmp)){message(g,"Could not seal commander data. Check free space.");return 0;}ok=save_commit(path,tmp,bak,rename);if(ok)g->cue=SFX_SELECT;message(g,ok?"Commander saved.":save_commit_failure==1?"Commander data failed validation. Previous save kept.":"Could not replace save file. Previous save kept.");return ok;');p.write_text(s)
p=Path('src/save-safety.h');s=p.read_text().replace('static int save_commit(const char *path','static int save_commit_failure; /* 1: validation, 2: filesystem replacement */\nstatic int save_commit(const char *path');s=s.replace(' if(!load_game_file(NULL,tmp))return 0;',' save_commit_failure=1;if(!load_game_file(NULL,tmp))return 0;save_commit_failure=2;');s=s.replace(' return 1;\n}\nint load_game',' save_commit_failure=0;return 1;\n}\nint load_game');p.write_text(s)
p=Path('src/cockpit-hud.h');s=p.read_text().replace('game.time+.1f','game.time+1.f/15').replace('hc_cache_due+=.1f','hc_cache_due+=1.f/15');s=s.replace('hc_key(95,150,\'S\',0);text_px(112,154,HC_DIM,"CLOSE")','hc_key(79,150,\'S\',0);text_px(96,154,HC_DIM,"RELEASE")');
# Display-only motion phases: never gate input or expose unrelated pixels.
s=s.replace('static void hc_target_card(void){','''static int hc_motion_target=-2,hc_motion_tool=-1,hc_motion_gun=-1,hc_motion_browser=0,hc_motion_tools=0;
static float hc_acquired_at=-10,hc_tool_at=-10,hc_gun_at=-10,hc_panel_at=-10;
static void hc_motion_tick(void){
 if(selected_target!=hc_motion_target){hc_motion_target=selected_target;hc_acquired_at=game.time;}
 if(tools_selected!=hc_motion_tool){hc_motion_tool=tools_selected;hc_tool_at=game.time;}
 if(game.active_weapon!=hc_motion_gun){hc_motion_gun=game.active_weapon;hc_gun_at=game.time;}
 if(hc_browser!=hc_motion_browser||tools_open!=hc_motion_tools){hc_motion_browser=hc_browser;hc_motion_tools=tools_open;hc_panel_at=game.time;}
}
static void hc_motion_draw(void){
 float age=game.time-hc_acquired_at;
 if(selected_target>=0&&age>=0&&age<.6f){int radius=17+(int)(14*(1-age/.6f));hc_circle_fast(53,233,radius,HC_CYAN);}
 age=game.time-hc_tool_at;if(age>=0&&age<.32f){int width=4+(int)(125*age/.32f);line(338,207,338+width,207,HC_GOLD);}
 age=game.time-hc_gun_at;if(age>=0&&age<.32f){int width=4+(int)(124*age/.32f);line(338,230,338+width,230,HC_CYAN);}
 age=game.time-hc_panel_at;if(age>=0&&age<.24f){if(hc_browser){int y=62+(int)(103*age/.24f);line(16,y,151,y,RGB(43,87,92));}if(tools_open){int y=76+(int)(54*age/.24f);line(254,y,467,y,RGB(68,76,53));}}
 unsigned signal=(game.voice_time>0||game.message_time>0)?HC_CYAN:HC_EDGE;for(int k=0;k<3;k++)rect(84+k*5,148,3,2,signal==HC_CYAN&&((int)(game.time*9)+k)%3?HC_CYAN:HC_EDGE);
 if(game.heat>78||game.incoming_missile>0||game.collision>0||game.attacked>0){unsigned warn=((int)(game.time*5)&1)?HC_GOLD:RGB(108,51,36);line(333,268,469,268,warn);}
}
static void hc_target_card(void){''');s=s.replace('hc_validate_lock();float previous=sensor_hud_range','hc_motion_tick();hc_validate_lock();float previous=sensor_hud_range');s=s.replace('hc_alerts_and_comms();hc_profile_us[3]','hc_alerts_and_comms();hc_motion_draw();hc_profile_us[3]');p.write_text(s)
