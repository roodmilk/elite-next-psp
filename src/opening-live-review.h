/* Opt-in disposable QA: runs normal main input/audio/render/VRAM cadence. */
static struct {int enabled,last,ticks,frames,ascent,planes,maps,fail,captured,confirmation;FILE *trace;} opening_live;
static void opening_live_trace(const char *phase){
 if(!opening_live.enabled||!opening_live.trace)return;
 fprintf(opening_live.trace,"%s frame%d stage%d page%d dock%d timer%.3f plane%d radio%d off%d audio%d stack%d\n",phase,opening_live.frames,game.opening.stage,page,game.dock_stage,game.dock_timer,buffer,radio_station,radio_off,audio_run,sceKernelCheckThreadStack());fflush(opening_live.trace);
}
static void opening_live_setup(void){
 FILE *flag=fopen("meadow-live.flag","r");int meadow_only=flag!=NULL,dream_only=0;
 if(!flag){flag=fopen("dream-live.flag","r");dream_only=flag!=NULL;}
 if(!flag)flag=fopen("opening-live.flag","r");
 if(!flag)return;
 fclose(flag);
 opening_live.enabled=meadow_only?3:dream_only?2:1;opening_live.last=-1;opening_live.trace=fopen("opening-live-trace.txt","w");
 commander_slots[0]="test-live-commander.sav";commander_slots[1]="test-live-commander2.sav";commander_slots[2]="test-live-commander3.sav";
 for(int station=0;station<RADIO_STATION_COUNT;station++)for(int line=0;line<24;line++)if(!radio_talk_lines[station][line]||!radio_talk_lines[station][line][0])opening_live.fail++;
 commander_active_slot=0;smoke=0;change_page(INTRO);intro_choice=1;opening_live_trace("SETUP");
 if(dream_only){opening_new_game();game.opening.stage=OP_DREAM;opening_prepared=-1;opening_prepare();}
 if(meadow_only){
  opening_new_game();game.opening.stage=OP_FIND;game.opening.flags=OF_DIALOG_DONE;
  opening_world_apply(&game);game_spawn(&game);game.planet=1;game.surface=2;game.docked=0;game.speed=0;
  game.ship_pos=surface_site(&game,1);game.pos=meadow_ground_point(game.ship_pos,78,-58,22);
  game.yaw=.10f;game.pitch=-.1f;game.roll=0;opening_frontend_reset();opening_prepare();page=FLIGHT;
 }
}
static unsigned opening_live_buttons(void){
 if(!opening_live.enabled)return 0;
 opening_live.frames++;int s=game.opening.stage;if(s!=opening_live.last){opening_live.last=s;opening_live.ticks=0;opening_live_trace("STAGE");}
 int t=++opening_live.ticks;int tap=t%6==1;
 if(opening_live.enabled==3){
  if(t==3)return PSP_CTRL_TRIANGLE; /* Real optional flower conversation. */
  if(t==90)return PSP_CTRL_CROSS;
  if(t>=150&&t<240)return PSP_CTRL_UP;
  if(t>=270){
   /* Diagnostic poses use the normal main-loop renderer and alternate VRAM.
    * Freeze position at the actual overlook, then sweep every bearing/pitch. */
   game.pos=meadow_ground_point(game.ship_pos,125,245,22);
   game.yaw=(t-270)*.019f;game.pitch=sinf((t-270)*.04f)*.75f;
  }
  return 0;
 }
 if(opening_live.enabled==2){
  /* Normal hardware input route: keep the first formation in view, then dodge
   * and throttle. No teleports, time jumps or health top-ups in this live run. */
  if(dream_time>18&&dream_time<21)return PSP_CTRL_RIGHT|PSP_CTRL_RTRIGGER;
  if(dream_time>27&&dream_time<30)return PSP_CTRL_LEFT|PSP_CTRL_LTRIGGER;
  if(dream_time>35&&dream_time<36)return PSP_CTRL_CROSS;
  return 0;
 }
 if(page==INTRO)return tap?PSP_CTRL_CROSS:0;
 if(s==OP_CREATE){if(t==1||t==7)return PSP_CTRL_DOWN;if(t==13)return PSP_CTRL_CROSS;return 0;}

 if(s==OP_SOL_MAP&&game.dock_stage==4){opening_live.ascent++;return 0;}
 const OpeningDialogueNode *node=opening_current_dialogue();
 if(node){
  if(opening_dialogue_revealed()&&node->choice_count&&game.opening.counter%10!=0)return tap?PSP_CTRL_DOWN:0;
  return tap?PSP_CTRL_CROSS:0;
 }
 if(opening_input_gate)return 0;
 if(s==OP_DREAM){if(t<3)return 0;return PSP_CTRL_RIGHT|PSP_CTRL_UP|PSP_CTRL_CROSS;}
 if(s==OP_SOL_MAP&&page==DETAILS)return opening_live.maps>=180&&tap?PSP_CTRL_CIRCLE:0;
 if(s==OP_YAW)return t<3||departure_release?0:PSP_CTRL_RIGHT; if(s==OP_PITCH)return t<3?0:PSP_CTRL_UP;
 if(s==OP_THROTTLE)return PSP_CTRL_RTRIGGER; if(s==OP_BRAKE)return PSP_CTRL_LTRIGGER;
 if(s==OP_EARTH_NET&&page==GALNET)return tap?PSP_CTRL_CIRCLE:0;
 if(s==OP_EARTH_ROUTE&&page==DETAILS)return tap?PSP_CTRL_CROSS:0;
 if(s==OP_APPROACH)return tap?PSP_CTRL_CROSS:0;
 if(s==OP_TOUCHDOWN||s==OP_WALK)return 0;
 if(s==OP_RADIO&&page==RADIO)return tap?(radio_off?PSP_CTRL_CROSS:PSP_CTRL_CIRCLE):0;
 return tap?PSP_CTRL_CROSS:0;
}
static void opening_live_after_draw(void){
 if(!opening_live.enabled)return;opening_live.planes|=1<<buffer;opening_live_trace("DRAW_DONE");
 if(opening_live.enabled==3){
  static int dirty=0,cleared=0,saw_dad=0;static SceUInt64 started=0;
  if(!started)started=sceKernelGetSystemTimeWide();
  saw_dad|=opening_notice&&!strcmp(opening_notice_speaker,"DAD");
  if(!opening_notice&&opening_live.ticks>92){
   cleared++;
   for(int y=0;y<H;y++)if((y>=24&&y<28)||(y>=240&&y<258))for(int x=0;x<W;x++){
    if(x>=180&&x<300)continue; /* Reserved Triangle Inspect prompt. */
    if(fb[y*STRIDE+x]!=RGB(21,28,39))dirty++;
   }
  }
  if(opening_live.ticks==40||opening_live.ticks==94||opening_live.ticks==180||(opening_live.ticks>=270&&opening_live.ticks%30==0)){
   char name[64];snprintf(name,sizeof(name),"live-meadow-282-%03d.bmp",opening_live.ticks);dump_native_bmp(name);
  }
  if(opening_live.ticks>=630){
   FILE *f=fopen("meadow-live-check.txt","w");if(f){
    int pass=!dirty&&cleared>400&&saw_dad&&opening_live.planes==3&&audio_run&&game.opening.stage==OP_FIND;
    double elapsed=(sceKernelGetSystemTimeWide()-started)/1000000.;
    fprintf(f,"%s Dad dialogue closes cleanly across both VRAM buffers during walking and full landscape sweeps\n",pass?"PASS":"FAIL");
    fprintf(f,"INFO %d frames %.2f FPS, %d clean-frame checks, %d stale pixels, Dad=%d, VRAM=%d, audio=%d\n",opening_live.frames,opening_live.frames/fmax(1,elapsed),cleared,dirty,saw_dad,opening_live.planes,audio_run);
    fprintf(f,"RESULT %d failures\n",!pass);fclose(f);
   }running=0;
  }return;
 }
 if(opening_live.enabled==2){
  static unsigned captures=0;static SceUInt64 started=0;
  if(!started)started=sceKernelGetSystemTimeWide();
  static const float times[]={.35f,.60f,.90f,1.3f,1.8f,5.2f,8.f,10.5f,12.f,14.f,16.f,24.f,40.f,70.f,110.f};
  for(int i=0;i<15;i++)if(!(captures&(1u<<i))&&dream_time>=times[i]&&opening_dream(&game)){
   char name[48];snprintf(name,sizeof(name),"live-dream-281-%02d.bmp",i);dump_native_bmp(name);captures|=1u<<i;
  }
  /* Allow the full 126 seconds at either 30 or 60 Hz, plus setup margin. */
  if(game.opening.stage==OP_HOME||opening_live.frames>9000){FILE *f=fopen("dream-live-check.txt","w");if(f){
   double seconds=(sceKernelGetSystemTimeWide()-started)/1000000.;int pass=game.opening.stage==OP_HOME&&opening_live.planes==3&&dream_shots_fired>0&&game.legal==0;
   fprintf(f,"%s live battle renders and wakes without crime state\n",pass?"PASS":"FAIL");
   fprintf(f,"INFO %d frames %.2f FPS %.2f dream seconds %d enemy shots %d hits captures=%u audio=%d\n",opening_live.frames,opening_live.frames/fmax(1,seconds),dream_time,dream_shots_fired,dream_hits,captures,audio_run);
   fprintf(f,"RESULT %d failures\n",!pass);fclose(f);
  }running=0;}return;
 }
 const OpeningDialogueNode *node=opening_current_dialogue();
 if(node&&node->choice_count&&opening_dialogue_revealed()&&opening_live.captured!=node->id){char path[64];snprintf(path,sizeof(path),"live-choice-%d.bmp",node->id);dump_native_bmp(path);opening_live.captured=node->id;}
 if(game.opening.stage==OP_DEALER&&opening_dealer_confirm&&!opening_live.confirmation){dump_native_bmp("live-dealer-confirm.bmp");opening_live.confirmation=1;}
 if(game.opening.stage==OP_SOL_MAP&&game.dock_stage==4&&opening_live.ascent%45==1){char path[64];snprintf(path,sizeof(path),"live-launch-%03d-plane%d.bmp",opening_live.ascent,buffer);dump_native_bmp(path);}
 if(page==FLIGHT&&opening_live.ticks==60&&(game.opening.stage==OP_YAW||game.opening.stage==OP_THROTTLE)){char path[64];snprintf(path,sizeof(path),"live-top-coach-%02d.bmp",game.opening.stage);dump_native_bmp(path);}
 if(game.opening.stage==OP_SOL_MAP&&page==DETAILS){opening_live.maps++;if(opening_live.maps==90)dump_native_bmp("live-sol-almanac.bmp");}
 if(game.opening.stage==OP_EARTH_NET&&page==GALNET&&opening_live.ticks==1)dump_native_bmp("live-earth-net.bmp");
 if(game.opening.stage==OP_TOUCHDOWN&&opening_live.ticks%60==1){char path[64];snprintf(path,sizeof(path),"live-earth-entry-%03d.bmp",opening_live.ticks);dump_native_bmp(path);}
 if(game.opening.stage==OP_WALK&&!planet_seat.active&&planet_controls_ready){
  dump_native_bmp("live-earth-walk.bmp");FILE *f=fopen("opening-live-check.txt","w");if(f){
   int pass=!opening_live.fail&&opening_live.ascent>100&&opening_live.planes==3&&radio_station==RADIO_DEFAULT_STATION&&!radio_off&&audio_run;
   fprintf(f,"%s real New Game, character completion, launch, flight lessons, Earth atmospheric entry, touchdown and EVA rendered with live Elite Exploration audio\n",pass?"PASS":"FAIL");
   fprintf(f,"INFO %d ascent frames, VRAM mask%d, %d main frames, stack%d\n",opening_live.ascent,opening_live.planes,opening_live.frames,sceKernelCheckThreadStack());fprintf(f,"RESULT %d failures\n",!pass);fclose(f);
  }running=0;
 }
 if(opening_live.frames>5400){FILE *f=fopen("opening-live-check.txt","w");if(f){fprintf(f,"FAIL trapped stage%d page%d\nRESULT 1 failures\n",game.opening.stage,page);fclose(f);}running=0;}
}
static void opening_live_finish(void){if(opening_live.trace){fclose(opening_live.trace);opening_live.trace=NULL;}}
