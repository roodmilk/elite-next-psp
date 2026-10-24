from pathlib import Path
p=Path('src/cockpit-hud.h');s=p.read_text();i=s.index('static void hc_ship_instruments(void)');s=s[:i]+'''static void hc_speed_ticker(void){
 /* Only this 32x8 aperture is updated at display cadence; never repaint the
    whole cockpit just to roll four digits. Use the supplied game glyphs. */
 float speed=fmaxf(0,fminf(9999,hud_smooth[4]));int value=(int)speed;unsigned ink=game.boost?HC_GOLD:WHITE;
 rect(205,168,32,10,HC_BG);
 int place=1000;for(int k=0;k<4;k++,place/=10){int digit=value/place%10;float phase=fmaxf(0,fmodf(speed,(float)place)-place+1);if(place==1)phase=speed-value;int offset=(int)(phase*8);
  if(k<3&&value<place)continue;
  for(int next=0;next<2;next++){const unsigned char*g=font8['0'+(digit+next)%10-32];int top=169-offset+next*8;
   for(int y=0;y<8;y++){int yy=top+y;if(yy<169||yy>176)continue;for(int x=0;x<8;x++)if(g[y]&(128u>>x))fb[yy*STRIDE+205+k*8+x]=ink;}
  }
 }
}
'''+s[i:];s=s.replace('text_px(205,169,game.boost?HC_GOLD:WHITE,"%d",(int)hud_smooth[4]);','');s=s.replace('hc_alerts_and_comms();hc_motion_draw();hc_profile_us[3]','hc_alerts_and_comms();hc_profile_us[3]');s=s.replace('line(16,y,151,y,RGB(43,87,92))','line(14,y,15,y+6,HC_CYAN)').replace('line(254,y,467,y,RGB(68,76,53))','line(470,y,470,y+6,HC_GOLD)');p.write_text(s)
p=Path('src/main.c');s=p.read_text().replace('hud_animation_tick();hud_canopy_feedback();\n  if(hc_repaint_needed())','hud_animation_tick();hud_canopy_feedback();hc_motion_tick();\n  if(hc_repaint_needed())');s=s.replace('  twin_cockpit_reflections();dashboard_draw();rear_view_mirror','  hc_speed_ticker();hc_motion_draw();twin_cockpit_reflections();dashboard_draw();rear_view_mirror');p.write_text(s)
p=Path('src/voyage.h');s=p.read_text().replace(' for(int i=0;i<5;i++){float rate=i==4?.22f:.18f;',' static float speed_stamp=-1;float speed_dt=game.time-speed_stamp;if(speed_dt<=0||speed_dt>.15f)speed_dt=1.f/30;speed_stamp=game.time;\n for(int i=0;i<5;i++){float rate=i==4?1-expf(-speed_dt*9):.18f;');p.write_text(s)
p=Path('src/encounter-flight-capture.h');s=p.read_text().replace('scene>41','scene>43');s=s.replace(' int mode=encounter_capture_hud;Vec3 c=', ''' int mode=encounter_capture_hud;
 if(mode>=24){if(frame==0){game.pos=add(hub_position(&game,0),(Vec3){4000,0,-12000});game.yaw=game.pitch=game.roll=0;game.speed=0;game.boost=0;r_tap=l_tap=10;hc_browser=0;selected_target=-1;for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;flight_camera_seen=-1;}return;}
 Vec3 c=''');s=s.replace(' if(encounter_capture_hud<9)return 0;',' if(encounter_capture_hud>=24){if(frame<5)return 0;if(encounter_capture_hud==25&&frame==6)return 0;return PSP_CTRL_RTRIGGER;}\n if(encounter_capture_hud<9)return 0;');s=s.replace('if(twin_capture_footage||encounter_capture_hud==23)','if(twin_capture_footage||encounter_capture_hud>=23)');s=s.replace('static float encounter_capture_worst[3];','static float encounter_capture_worst[3];static int capture_boost_frames;static float capture_peak_speed;');s=s.replace(' if(encounter_capture_scene<0||frame<=30)return;',' if(game.boost)capture_boost_frames++;capture_peak_speed=fmaxf(capture_peak_speed,game.speed);\n if(encounter_capture_scene<0||frame<=30)return;');s=s.replace(' if(encounter_capture_hud>=9)fprintf(f,',' fprintf(f,"INFO real boost frames %d peak speed %.2f\\n",capture_boost_frames,capture_peak_speed);\n if(encounter_capture_hud>=9)fprintf(f,');p.write_text(s)
