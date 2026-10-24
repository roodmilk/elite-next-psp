/* Explicit opt-in real ad-hoc test driver, not a mock transport. */
static struct {int enabled,host,started,connected,npcs,debris,moved,captured,oldstate,system_ok,drop,surface,landed,walked,car,launched,stage,avatars,life,square,tools,travel,travel_started,other_system,planet_pc,hand_pc,purchased,saved,cash_after;float elapsed,linked;Vec3 remote;FILE *log;} wifi_live;
static void wifi_live_setup(void){
 FILE *f=fopen("local-wifi-live.flag","r");if(!f)return;fscanf(f,"%d %d %d %d",&wifi_live.host,&wifi_live.drop,&wifi_live.surface,&wifi_live.travel);fclose(f);
 wifi_live.enabled=1;wifi_live.log=fopen("local-wifi-live.txt","w");if(!wifi_live.log){wifi_live.enabled=0;return;}
 game_init(&game);story_complete(&game);game.system=wifi_live.host?19:7;system_bodies(&game);game_spawn(&game);game.credits=10000000;launch(&game);game.dock_stage=0;
 snprintf(game.commander_name,25,wifi_live.host?"Host Pilot":"Guest Pilot");page=HOME;row=wifi_live.host?27:28;
 fprintf(wifi_live.log,"INFO real ad-hoc role %s\n",wifi_live.host?"host":"guest");fflush(wifi_live.log);
}
static int wifi_live_frame(float dt){
 if(!wifi_live.enabled)return 0;
 wifi_live.elapsed+=dt;
 if(!wifi_live.started){wifi_live.started=1;int result=wifi_init(wifi_live.host);fprintf(wifi_live.log,"INIT %d available %d wlan %d note %s message %s\n",result,wifi_available(),sceWlanGetSwitchState(),wifi.note,game.message);fflush(wifi_live.log);}
 if(wifi.state!=wifi_live.oldstate){fprintf(wifi_live.log,"STATE %d %.2f %s\n",wifi.state,wifi_live.elapsed,wifi.note);fflush(wifi_live.log);wifi_live.oldstate=wifi.state;}
 if(wifi.ready){
  if(!wifi_live.connected){wifi_live.connected=1;wifi_live.remote=wifi.remote.pos;fprintf(wifi_live.log,"CONNECTED system %d peer %.24s\n",game.system,wifi.remote.name);fflush(wifi_live.log);}
  wifi_live.linked+=dt;wifi_live.moved|=length(sub(wifi.remote.pos,wifi_live.remote))>40;
  wifi_live.system_ok|=game.system==19;
  int npc_chunks=0,debris_chunks=0;for(int i=1;i<=8;i++)npc_chunks+=wifi.seen[i]!=0;for(int i=9;i<=16;i++)debris_chunks+=wifi.seen[i]!=0;
  wifi_live.npcs|=npc_chunks==8;wifi_live.debris|=debris_chunks==8;
 }
 if(wifi.ready&&!wifi_live.purchased&&wifi_live.linked>3.3f){
  game.docked=1;game.systems[game.system].tech=15;int bought=buy_ship(&game,1),weapon=-1;for(int i=1;i<EQUIP_COUNT;i++)if(equipment_in_stock(i)&&equip_slot_for(i)==FIT_WPN&&!equipment_owned(i)){weapon=i;break;}
  equip_target=-1;if(weapon>0)buy_equipment(weapon,0);wifi_live.purchased=bought&&weapon>0&&fit_find(&game,weapon)>=0;
  game.discoveries+=1;page=STATUS;input(PSP_CTRL_CROSS,0,dt,0,0);Game *loaded=malloc(sizeof(Game));wifi_live.saved=loaded&&load_game(loaded,commander_save_path(&game))&&loaded->ship==game.ship&&loaded->credits==game.credits&&!memcmp(loaded->fit,game.fit,sizeof(game.fit));free(loaded);wifi_live.cash_after=game.credits;page=FLIGHT;game.docked=game.dock_stage=0;
 }
 unsigned pressed=0,held=0;float turn=.02f;
 if(page==MULTIPLAYER){
  if(wifi.host&&wifi.state==WIFI_HOST)pressed=PSP_CTRL_CROSS;
  if(!wifi.host&&wifi.state==WIFI_SCAN&&wifi.scan_done){
   if(wifi.rooms_n)pressed=PSP_CTRL_CROSS;
   else if(wifi.age>2){pressed=PSP_CTRL_SQUARE;wifi.age=0;}
  }
 }else held=wifi_live.linked<4?PSP_CTRL_RTRIGGER:0;
 if(wifi_live.travel&&wifi.ready){
  if(!wifi_live.host&&wifi.remote.system!=game.system){wifi_live.other_system=1;selected_target=PLAYER_TARGET_ID;autoaim=1;wifi_contact_route();}
  if(!wifi_live.travel_started&&wifi_live.linked>(wifi_live.host?5:20)){int dest=game.destination;if(wifi_live.host){for(int i=0;i<256;i++)if(i!=game.system&&distance_ly(&game,game.system,i)*10<=game.fuel){dest=i;break;}}game.destination=dest;wifi_live.travel_started=jump_start(&game);}
  if(wifi_live.travel_started&&game.system!=19)wifi_live.launched=1;
 }
 if(wifi_live.surface&&wifi.ready){
  held=0;turn=0;
  if(!wifi_live.stage&&wifi_live.linked>4){wifi_land(1);wifi_live.stage=1;}
  if(game.surface==2&&!planet_transfer_busy()&&planet_controls_ready){
   wifi_live.landed=1;wifi_live.avatars|=wifi.remote.surface==2&&!wifi.remote.rover;
   wifi_live.life|=wifi.seen[17]!=0;
   if(wifi_live.stage==1&&wifi_live.linked>20){game.yaw=0;held=PSP_CTRL_UP;wifi_live.walked|=game.speed>10;}
   if(wifi_live.stage==1&&wifi_live.linked>23){game.pos=add(game.rover_pos,(Vec3){0,0,30});pressed=PSP_CTRL_CROSS;wifi_live.stage=2;}
   if(wifi_live.stage==2&&wifi_live.linked>26&&game.rover_driving){held=PSP_CTRL_RTRIGGER;wifi_live.car|=game.speed>10;}
   if(wifi_live.stage==2&&wifi_live.linked>30){game.rover_velocity=(Vec3){0};pressed=PSP_CTRL_TRIANGLE;wifi_live.stage=3;}
   if(wifi_live.stage==3&&wifi_live.linked>33&&!game.rover_driving){game.pos=add(game.ship_pos,(Vec3){30,4,0});pressed=PSP_CTRL_TRIANGLE;}
  }
  if(game.surface==1&&!planet_transfer_busy()&&planet_controls_ready&&wifi_live.stage==3){pressed=PSP_CTRL_RTRIGGER;wifi_live.stage=5;}
  if(wifi_live.stage==5&&game.planet<0)wifi_live.launched=1;
 }
 if(wifi.ready&&game.planet<0&&wifi_live.linked>=1&&wifi_live.linked<2){static int begun=0;held=PSP_CTRL_SQUARE;if(!begun){pressed=PSP_CTRL_SQUARE;begun=1;}wifi_live.square|=square_held;}
 if(wifi.ready&&game.planet<0&&wifi_live.linked>=2.2f&&wifi_live.linked<3.2f){held=PSP_CTRL_CIRCLE;if(!tools_hold)pressed=PSP_CTRL_CIRCLE;wifi_live.tools|=tools_open;}
 if(wifi_live.surface&&hand_context()&&wifi_live.linked>=19.4f&&wifi_live.linked<20.4f){static int began=0;held=PSP_CTRL_SQUARE;if(!began){pressed=PSP_CTRL_SQUARE;began=1;}wifi_live.planet_pc|=surface_target_open;}
 if(wifi_live.surface&&hand_context()&&wifi_live.linked>=20.6f&&wifi_live.linked<21.6f){held=PSP_CTRL_CIRCLE;if(!hand_hold)pressed=PSP_CTRL_CIRCLE;wifi_live.hand_pc|=hand_open;}
 input(pressed,held,dt,turn,0);
 if(wifi_live.linked>(wifi_live.surface?21:5)&&!wifi_live.captured){if(wifi_live.surface){Vec3 d=norm(sub(wifi.remote.pos,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=0;}wifi_live.captured=1;}
 int finish=wifi_live.elapsed>(wifi_live.drop?120:wifi_live.surface?110:wifi_live.travel?60:40)||(!wifi_live.host&&!wifi_live.drop&&wifi_live.linked>(wifi_live.surface?55:wifi_live.travel?34:10))||(wifi_live.connected&&!wifi.state);
 if(finish){
  int system_ok=wifi_live.host||wifi_live.system_ok;
  int timeout_ok=!wifi_live.drop||strstr(wifi.note,"timed out")!=NULL;
  int samepage=page,body=game.planet;Vec3 before=game.pos;wifi_close(NULL);
  int restored=game.ship==1&&game.credits==wifi_live.cash_after&&wifi_live.purchased&&wifi_live.saved&&page==samepage&&game.planet==body&&length(sub(game.pos,before))==0&&!wifi.backup&&!wifi.state;
  fprintf(wifi_live.log,"%s normal Square target computer %d and Circle tools %d\n",wifi_live.square&&wifi_live.tools?"PASS":"FAIL",wifi_live.square,wifi_live.tools);
  int fail=!wifi_live.square||!wifi_live.tools||!wifi_live.connected||!wifi_live.moved||!system_ok||!restored||!timeout_ok||(!wifi_live.host&&(!wifi_live.npcs||!wifi_live.debris));
  if(wifi_live.travel){int ok=wifi_live.launched&&(wifi_live.host||wifi_live.other_system);fail|=!ok;fprintf(wifi_live.log,"%s independent hyperspace / friend route tracking\n",ok?"PASS":"FAIL");}
  if(wifi_live.surface&&!wifi_live.drop){fprintf(wifi_live.log,"%s on-foot target computer %d and handheld tools %d\n",wifi_live.planet_pc&&wifi_live.hand_pc?"PASS":"FAIL",wifi_live.planet_pc,wifi_live.hand_pc);int ok=wifi_live.planet_pc&&wifi_live.hand_pc&&wifi_live.landed&&wifi_live.walked&&wifi_live.car&&wifi_live.launched&&wifi_live.avatars&&(wifi_live.host||wifi_live.life);fail|=!ok;fprintf(wifi_live.log,"%s surface landing %d walking %d rover %d launch %d avatars %d life %d\n",ok?"PASS":"FAIL",wifi_live.landed,wifi_live.walked,wifi_live.car,wifi_live.launched,wifi_live.avatars,wifi_live.life);}
  if(wifi_live.drop)fprintf(wifi_live.log,"%s lost-host timeout keeps commander progress\n",timeout_ok?"PASS":"FAIL");
  fprintf(wifi_live.log,"%s handshake and moving remote pilot\n",wifi_live.connected&&wifi_live.moved?"PASS":"FAIL");
  fprintf(wifi_live.log,"%s host world and complete NPC/debris snapshots\n",system_ok&&(wifi_live.host||(wifi_live.npcs&&wifi_live.debris))?"PASS":"FAIL");
  fprintf(wifi_live.log,"%s purchased ship/equipment saved and kept without interruption\n",restored?"PASS":"FAIL");fprintf(wifi_live.log,"RESULT %d failures\n",fail);
  fclose(wifi_live.log);wifi_live.log=NULL;wifi_live.enabled=0;running=0;
 }
 return 1;
}
