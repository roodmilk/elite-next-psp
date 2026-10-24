static int online_review(void){
 FILE *flag=fopen("online-review.flag","r");if(!flag)return 0;fclose(flag);FILE *f=fopen("online-review.txt","w");if(!f)return 1;int failures=0;
#define OC(c,s) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;}while(0)
 game_init(&game);story_complete(&game);online_config_load();
 uint32_t checked_address=0;const unsigned char lan_bytes[4]={192,168,1,105};
 OC(online_ipv4_parse("192.168.1.105",&checked_address)&&!memcmp(&checked_address,lan_bytes,4),"LAN address resolves to exact wire bytes without network imports");
 OC(online_ipv4_parse(online.host,&checked_address)&&checked_address==online.server.sin_addr.s_addr,"configuration resolves before loading the PSP networking module");
 checked_address=0x12345678;OC(!online_ipv4_parse("256.168.1.105",&checked_address)&&checked_address==0x12345678,"invalid IPv4 address rejected without overwriting destination");
 online.stage=2;OnlinePacket p=online_packet(2);p.id=1;p.token=13;online_receive(&p,&online.server);OC(online.stage==3&&online.id==1,"validated server handshake assigns presence ID");
 p=online_packet(5);p.id=77;p.token=21;p.sequence=1;p.ip=inet_addr("127.0.0.1");p.port=30000;snprintf(p.pilot.name,25,"Other Pilot");p.pilot.system=19;online_receive(&p,&online.server);OC(online_peer_ready(0)&&online.peer[0].pilot.system==19,"directory accepts a remote system");
 page=MULTIPLAYER;mp.caller=HOME;mp.focus=MP_LIST;online_return_page=HOME;online_page_input(PSP_CTRL_CROSS);OC(selected_target==PLAYER_TARGET_ID&&game.route_goal==19&&page==MULTIPLAYER,"docked tracking plots route without launching");
 p.sequence=2;p.pilot.system=game.system;p.pilot.planet=1;p.pilot.surface=2;online_receive(&p,&online.server);online_apply(0);OC(length(sub(wifi_contact_position(),game.bodies[1].pos))==0,"off-planet friend tracks their actual planet");
 OnlinePacket bad=p;bad.sequence=3;bad.pilot.pos.x=NAN;online_receive(&bad,&online.server);OC(online.peer[0].sequence==2,"nonfinite coordinates rejected");
 bad=p;bad.type=4;bad.sequence=3;bad.token=22;struct sockaddr_in stranger=online.peer[0].address;online_receive(&bad,&stranger);OC(online.peer[0].sequence==2,"direct packets require advertised peer token");
 game.credits=123456;page=EQUIP;row=3;Vec3 pos=game.pos;p.type=6;online_receive(&p,&online.server);OC(!online_peer_ready(0)&&game.credits==123456&&page==EQUIP&&row==3&&length(sub(pos,game.pos))==0,"departed peer alone removed; commander/menu/position intact");
 online_close(NULL);OC(game.credits==123456&&page==EQUIP,"closing directory never rolls back commander");online.enabled=0;
 game_init(&game);story_complete(&game);deck_reset();page=HOME;row=29;input(PSP_CTRL_CROSS,0,.016f,0,0);OC(page==MULTIPLAYER,"Fly opens Multiplayer");online_page_input(PSP_CTRL_CIRCLE);OC(page==HOME,"directory back returns to deck");
 page=INTRO;input(PSP_CTRL_SQUARE,0,.016f,0,0);OC(page==MULTIPLAYER&&online_return_page==INTRO,"title offers Multiplayer before loading");online_page_input(PSP_CTRL_CIRCLE);OC(page==INTRO,"title settings return to title");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);return 1;
#undef OC
}
static struct {int active,role,previous,connected,peer,other,planet,gone,saved,square,tools,started;float time,linked;FILE *log;} online_live;
static void online_live_setup(void){FILE *f=fopen("online-live.flag","r");if(!f)return;fscanf(f,"%d",&online_live.role);fclose(f);online_live.log=fopen("online-live.txt","w");if(!online_live.log)return;online_live.active=1;game_init(&game);story_complete(&game);launch(&game);game.dock_stage=0;game.pos=(Vec3){online_live.role*200.f,0,0};game.credits=123456;snprintf(game.commander_name,25,"Online Pilot %d",online_live.role);page=FLIGHT;online.enabled=1;}
static int online_live_frame(float dt){
 if(!online_live.active)return 0;online_live.time+=dt;
 if(online.stage!=online_live.previous){fprintf(online_live.log,"STATE %d %.2f %s\n",online.stage,online_live.time,online.note);fflush(online_live.log);online_live.previous=online.stage;}
 if(online.stage==3){online_live.connected=1;online_live.linked+=dt;int ids[ONLINE_LIMIT],n=online_list(ids);online_live.peer|=n>0;if(n){online_apply(ids[0]);online_live.other|=wifi.remote.system!=game.system;online_live.planet|=wifi.remote.planet==1&&game.planet==1&&wifi.remote.surface==2;}}
 unsigned held=0,pressed=0;if(online_live.linked>1&&online_live.linked<2){static int begun=0;held=PSP_CTRL_SQUARE;if(!begun){pressed=PSP_CTRL_SQUARE;begun=1;}online_live.square|=square_held;}if(online_live.linked>2.2f&&online_live.linked<3.2f){held=PSP_CTRL_CIRCLE;if(!tools_hold)pressed=PSP_CTRL_CIRCLE;online_live.tools|=tools_open;}
 input(pressed,held,dt,.02f,0);
 if(online_live.time>9&&!online_live.saved&&online.stage==3){game.docked=1;online_live.saved=save_game(&game,"online-commander.sav");game.docked=0;}
 if(online_live.role==1&&online_live.time>10&&online_live.time<12&&game.system==7){game.system=19;game_spawn(&game);}
 if(online_live.time>14&&online_live.time<16&&online_live.role==1&&game.system==19){game.system=7;game_spawn(&game);}
 if(online_live.time>(online_live.role==1?17:19)&&game.planet<0&&!online_live.started){online_live.started=1;game.approach=1;enter_planet(&game);game.pos=surface_landing_pad(&game);game.speed=0;land_planet(&game);disembark_planet(&game);}
 if(online_live.role==1&&online_live.time>27&&online.stage){online_close("Test pilot departed");online.enabled=0;}
 if(online_live.role==2&&online_live.peer&&online_live.time>29){int ids[ONLINE_LIMIT];online_live.gone|=online_list(ids)==0;}
 if(online_live.time>(online_live.role==1?28:36)){
  int fail=!online_live.connected||!online_live.peer||!online_live.planet||!online_live.saved||!online_live.square||!online_live.tools||game.credits!=123456;
  if(online_live.role==2)fail|=!online_live.other||!online_live.gone;
  fprintf(online_live.log,"CHECK connected %d peer %d other system %d planet %d departed %d saved %d Square %d Circle %d credits %d\n",online_live.connected,online_live.peer,online_live.other,online_live.planet,online_live.gone,online_live.saved,online_live.square,online_live.tools,game.credits);fprintf(online_live.log,"RESULT %d failures\n",fail);fclose(online_live.log);online_live.active=0;online_close(NULL);running=0;
 }
 return 1;
}
