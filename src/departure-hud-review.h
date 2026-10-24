#if defined(ELITE_ENABLE_REVIEW)||defined(ELITE_ENCOUNTER_REVIEW)
static int departure_hud_review(void){
 FILE*flag=fopen("departure-hud-review.flag","r");if(!flag)return 0;fclose(flag);FILE*out=fopen("departure-hud-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int checks=0,fails=0;
 #define DH(c,label) do{int ok=!!(c);checks++;fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);}while(0)
 unsigned*prior=fb,*pixels=calloc(STRIDE*H,sizeof(unsigned));DH(pixels!=0,"allocate native framebuffer");if(!pixels){fclose(out);return 1;}fb=pixels;
 for(int kind=0;kind<4;kind++){
  hc_test_setup();game.docked=1;game.station_variant=0;
  int sys=7;if(kind!=1)for(int i=0;i<256;i++)if(station_class_for_system(&game,i)==(kind==3?STATION_MEGA:kind)){sys=i;break;}
  game.system=sys;system_bodies(&game);game_spawn(&game);game.docked=1;game.station_variant=kind==3?1:0;game.ship=0;fit_clear_all(&game);game.fit[0]=1;game.fit[5]=CHARM_DUCK;game.fit[3]=DASHBOARD_FIRST;fit_rebuild(&game);game.dockbay.dirty=0;launch_departure(&game);hc_cache_ready=0;hc_cache_dirty=1;hud_anim_ready=0;third_person=0;hc_browser=0;tools_reset_gesture();
  int modern=1,blocked=1,label=1;unsigned charms=charm_draw_calls,dash=dashboard_draw_calls;
  for(int frame=0;frame<243;frame++){
   input(0,0,1.f/30,0,0);if(game.dock_stage==4){modern&=hc_presentation_context()&&hc_full_console_visible();blocked&=!hc_context()&&!hc_browser&&!tools_open;}
   space();
   /* This named console is absent from the legacy HUD. Check its actual glyph
      pixels through the entire flight, including both sides of handoff. */
   for(int y=0;y<8;y++)for(int x=0;x<120;x++){int ch="TARGET COMPUTER"[x/8];if(font8[ch-32][y]&(128>>(x%8)))label&=pixels[(172+y)*STRIDE+30+x]==HC_CYAN;}
   if(kind==1&&frame%3==0){char file[64];snprintf(file,sizeof(file),"launch-native-%03d.bmp",frame);dump_native_bmp(file);}
  }
  DH(modern&&label,"current cockpit remains visible throughout launch and free-flight handoff");DH(blocked,"presentation does not enable manual targeting/tools during autopilot");DH(charm_draw_calls>charms&&dashboard_draw_calls>dash,"installed charm and dashboard artwork remain visible on departure");DH(!game.dock_stage&&!departure_release,"eight-second guided launch and fresh-input handoff complete normally");
 }
 hc_test_setup();game.docked=1;page=HOME;row=0;departure_release=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);int shots=game.shots;Vec3 forward_start=forward(&game);for(int i=0;i<520;i++)input(PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,.016f,1,1);DH(game.shots==shots&&!hc_browser&&!tools_open&&!game.boost&&departure_release&&dot(forward_start,forward(&game))>.999f,"held controls cannot fire steer open computers or boost during guided departure");input(0,0,.016f,0,0);DH(!departure_release,"releasing held controls enables normal flight");
 hc_test_setup();game.npc[1].pos=add(game.pos,(Vec3){2500,300,1800});selected_target=NPC_ID_MIN;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);hc_highlight=NPC_ID_MIN+1;hc_refresh=1;float yaw=game.yaw,speed=game.speed;r_tap=0;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);DH(selected_target==NPC_ID_MIN+1&&autoaim&&hc_browser,"R alone locks highlighted contact and enables tracking while Square stays held");DH(!game.boost&&r_tap>1&&game.speed<=speed+.01f,"tracking R does not boost or accelerate");
 for(int i=0;i<12;i++)input(0,PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER,.016f,0,0);DH(game.yaw!=yaw&&autoaim&&selected_target==NPC_ID_MIN+1,"ship actually turns toward the tracked highlighted contact");space();dump_native_bmp("target-r-track.bmp");input(0,0,.016f,0,0);DH(!hc_browser&&autoaim&&selected_target==NPC_ID_MIN+1,"Square release closes computer and preserves confirmed tracking");
 hc_test_setup();input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);hc_highlight=NPC_ID_MIN+1;hc_refresh=1;input(PSP_CTRL_CROSS,PSP_CTRL_SQUARE|PSP_CTRL_CROSS,.016f,0,0);DH(selected_target==NPC_ID_MIN+1&&!autoaim,"X alone still locks without automatically turning");
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,fails);fclose(out);fb=prior;free(pixels);return 1;
 #undef DH
}
#endif
