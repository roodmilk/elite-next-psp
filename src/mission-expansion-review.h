static int mission_expansion_review(void){
 FILE *flag=fopen("mission-expansion-review.flag","r");if(!flag)return 0;fclose(flag);
 int failures=expedition_tests("mission-expansion-backend.txt");FILE *out=fopen("mission-expansion-review.txt","w");if(!out)return 1;
 unsigned *oldfb=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));Game *snapshot=malloc(sizeof(Game));if(!pixels||!snapshot){fclose(out);free(pixels);free(snapshot);return 1;}
 fb=pixels+16;for(int k=0;k<16;k++)pixels[k]=pixels[16+STRIDE*H+k]=0x8f12479au;
 #define XPUI(c,label) do{int good=!!(c);failures+=!good;fprintf(out,"%s %s\n",good?"PASS":"FAIL",label);fflush(out);}while(0)
 int cards=0,fit=1;
 for(int index=0;index<EXPEDITION_COUNT;index++){
  game_init(&game);story_complete(&game);game.opening.stage=0;game.jobs[0]=(Job){0};game.job_n=1;game.job_sel=0;game.jobs[0].type=EXPEDITION_FIRST+index;game.expeditions[0].seed=index+17;page=HOME;
  const ExpeditionDef *d=expedition_def(game.jobs[0].type);
  for(int stage=0;stage<d->count;stage++){
   game.expeditions[0].stage=stage;game.expeditions[0].unread=1;mission_card_reset();mission_card_sync();expedition_call_age=1;expedition_answer=0;
   rect(0,0,W,H,BG);expedition_call_draw();char path[64];snprintf(path,sizeof(path),"mission-%02d-stage-%02d.bmp",index+1,stage+1);dump_native_bmp(path);cards++;
   const char *objective=d->stages[stage].objective;
   int ok=!*dialogue_skip(dialogue_source,dialogue_pages*4,39)&&(dialogue.context_page?dialogue_lines(objective,54)<=2:strstr(dialogue_source,objective)!=0);fit&=ok;if(!ok)fprintf(out,"FAIL text overflow mission %d stage %d\n",index+1,stage+1);
   if(d->stages[stage].kind==XP_CHOICE){expedition_answer=1;rect(0,0,W,H,BG);expedition_call_draw();snprintf(path,sizeof(path),"mission-%02d-stage-%02d-alternate.bmp",index+1,stage+1);dump_native_bmp(path);}
  }
 }
 XPUI(cards==155&&fit,"all 155 authored stage cards render at 480x272 with complete titles, speech and objectives");
 uint64_t pictured=0;game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.credits=100000;game.fit[FIT_WPN]=1;fit_rebuild(&game);page=MISSIONS;preview_time=18;
 for(int system=0;system<256&&pictured!=((1ull<<40)-1);system++)for(int hub=0;hub<HUB_COUNT;hub++){
  game.system=system;game.station_variant=hub;
  for(int offer=0;offer<5;offer++){int type=mission_type_at(&game,system,hub,offer),index=type-EXPEDITION_FIRST;if(index<0||index>=40||(pictured&(1ull<<index)))continue;
   row=game.system==7?offer+1:offer;rect(0,0,W,H,BG);mission_board();char path[64];snprintf(path,sizeof(path),"board-%02d.bmp",index+1);dump_native_bmp(path);pictured|=1ull<<index;
  }
 }
 XPUI(pictured==((1ull<<40)-1),"all forty contracts appear on actual native station boards");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.system=7;system_bodies(&game);expedition_start(&game,EXPEDITION_FIRST+32,7,12000);expedition_ack(&game,0);launch(&game);page=FLIGHT;expedition_tick(&game,.01f);expedition_flight_sync();selected_target=mission_target_id(&game,0);game.pos=add(discovery_position(0),(Vec3){0,0,-13000});hail_target();int post_ok=game.expeditions[0].stage==0&&selected_target==DISCOVERY_ID_MIN;game.pos=add(discovery_position(0),(Vec3){0,0,-1000});hail_target();XPUI(post_ok&&game.expeditions[0].stage==1,"navigate and contact use the actual listening post, reject distant inspection and advance only in range");
 game_init(&game);story_complete(&game);game.opening.stage=0;for(int sys=0;sys<256;sys++)if(system_whales(sys)){game.system=sys;break;}system_bodies(&game);expedition_start(&game,EXPEDITION_FIRST+33,game.system,12000);expedition_ack(&game,0);launch(&game);page=FLIGHT;expedition_tick(&game,.01f);expedition_flight_sync();selected_target=mission_target_id(&game,0);game.pos=add(whale_position(&game,0),(Vec3){0,0,-3000});hail_target();int whale_ok=game.expeditions[0].stage==0;game.pos=add(whale_position(&game,0),(Vec3){0,0,-1000});hail_target();XPUI(whale_ok&&game.expeditions[0].stage==1,"the flight contact action records the actual whale pod only within its 2400 metre observation range");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.credits=100000;game.fit[FIT_WPN]=1;fit_rebuild(&game);expedition_start(&game,EXPEDITION_FIRST,game.system,12000);expedition_ack(&game,0);mission_card_reset();tracked_mission=2;page=CAMPAIGN;row=0;
 rect(0,0,W,H,BG);campaign_screen();dump_native_bmp("tracked-convoy.bmp");page=MISSIONLOG;row=2;rect(0,0,W,H,BG);mission_log();dump_native_bmp("mission-log-convoy.bmp");
 launch(&game);page=FLIGHT;expedition_tick(&game,.01f);game.pos=add(expedition_waypoint(&game),(Vec3){0,0,-1500});game.yaw=game.pitch=game.roll=0;game.message_time=game.voice_time=0;rect(0,0,W,H,BG);space();dump_native_bmp("flight-convoy-marker.bmp");
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.credits=100000;game.fit[FIT_WPN]=1;fit_rebuild(&game);expedition_start(&game,EXPEDITION_FIRST,game.system,12000);page=FLIGHT;game.docked=0;game.attacked=0;mission_card_reset();mission_card_sync();
 memcpy(snapshot,&game,sizeof(game));input(0,0,.2f,1,1);XPUI(!memcmp(snapshot,&game,sizeof(game)),"incoming mission transmission freezes flight, NPCs, danger and mission timers");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.01f,0,0);XPUI(mission_card_open(),"an early or held X cannot accidentally dismiss a transmission");
 input(0,0,.5f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.02f,0,0);XPUI(!mission_card_open()&&!game.expeditions[0].unread,"fresh X acknowledges the stage without firing the ship weapon");input(0,0,.02f,0,0);
 page=CAMPAIGN;tracked_mission=2;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.02f,0,0);XPUI(expedition_history&&mission_card_open(),"Square reopens the tracked contact's stage history");input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.02f,0,0);XPUI(!expedition_history,"Circle leaves contact history and restores its previous screen");
 int guards=1;for(int k=0;k<16;k++)guards&=pixels[k]==0x8f12479au&&pixels[16+STRIDE*H+k]==0x8f12479au;XPUI(guards,"all mission transmission renders preserve framebuffer bounds");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(pixels);free(snapshot);
 #undef XPUI
 return 1;
}
