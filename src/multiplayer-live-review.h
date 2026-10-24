/* Opt-in real infrastructure transport test through the production console. */
static struct {int active,step,fail,associated,handshake,captured;float time,linked,game_time;uint64_t first_server;FILE *log;} mp_live;
static void multiplayer_live_setup(void){
 FILE *flag=fopen("multiplayer-online-live.flag","r");if(!flag)return;fclose(flag);mp_live.log=fopen("multiplayer-online-live.txt","w");if(!mp_live.log)return;
 mp_live.active=1;game_init(&game);story_complete(&game);game.opening.stage=0;game.tutorial_step=0;online.enabled=0;online.retry=UINT64_MAX;page=HOME;row=29;multiplayer_enter(HOME);mp_live.game_time=game.time;
}
static void mp_live_check(int ok,const char *s){fprintf(mp_live.log,"%s %s\n",ok?"PASS":"FAIL",s);mp_live.fail+=!ok;fflush(mp_live.log);}
static int multiplayer_live_frame(float dt){
 if(!mp_live.active)return 0;
 mp_live.time+=dt;
 if(!mp_live.step){input(PSP_CTRL_CROSS,0,dt,0,0);mp_live.step=1;}
 else if(mp_live.step==1){
  mp_live.associated|=online.stage==2;mp_live.handshake|=online.stage==3;
  if(online.stage==3){mp_live.first_server=online.last_server;mp_live_check(!strcmp(mp_view().connect,"CONNECTED"),"real server handshake lights CONNECTED");mp_live.step=2;}
 }
 else if(mp_live.step==2){
  input(0,0,dt,0,0);mp_live.linked+=dt;
  if(mp_live.linked>2){mp_live_check(online.stage==3&&online.last_server>mp_live.first_server,"server heartbeats continue while console pauses gameplay");mp_live_check(game.time==mp_live.game_time,"commander simulation stays paused during live connection");input(PSP_CTRL_CIRCLE,0,dt,0,0);mp_live_check(page==HOME&&online.stage==3,"Circle returns to deck without ending actual socket connection");row=29;input(PSP_CTRL_CROSS,0,dt,0,0);mp_live_check(page==MULTIPLAYER&&online.stage==3,"returning to Multiplayer retains live connection");input(PSP_CTRL_CROSS,0,dt,0,0);mp_live_check(!online.stage&&!online.enabled&&online.sock<0&&online.retry==UINT64_MAX,"CONNECTED control closes socket, disables retry and returns OFFLINE");online_config_load();mp_live_check(!online.enabled&&!online_save_dirty,"explicit disconnect persists the offline preference");mp_live.step=3;}
 }
 if(mp_live.step==3||mp_live.time>60){
  if(mp_live.step!=3){fprintf(mp_live.log,"INFO stage %d error %d profile count %d WLAN %d note %s\n",online.stage,online_error,online.profile_count,sceWlanGetSwitchState(),online.note);mp_live_check(0,"real connection must complete within 60 seconds");}
  fprintf(mp_live.log,"INFO association observed %d; handshake observed %d; no synthetic transport hook used.\nRESULT %d failures\n",mp_live.associated,mp_live.handshake,mp_live.fail);fclose(mp_live.log);mp_live.active=0;running=0;
 }
 return 1;
}
static void multiplayer_live_capture(void){if(mp_live.active&&online.stage==3&&!mp_live.captured){dump_native_bmp("multiplayer-live-connected.bmp");mp_live.captured=1;}}
