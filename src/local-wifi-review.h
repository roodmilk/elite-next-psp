/* Native protocol fixtures: no WLAN calls. Live discovery needs two peers. */
static int wifi_review(void){
 FILE *flag=fopen("local-wifi-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("local-wifi-review.txt","w");if(!f)return 1;
 int failures=0,checks=0;
#define WC(c,s) do {checks++;int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;}while(0)
 game_init(&game);story_complete(&game);deck_reset();page=HOME;row=20;
 int visible[9],n=deck_fill(0,visible);WC(n==6&&visible[5]==29,"FLY incorporates Host/Join into one Multiplayer entry");
 deck_step(1);WC(row==29,"Down from Disembark selects Multiplayer");deck_step(1);WC(row==0,"Multiplayer wraps to Resume/Launch");
 game.docked=0;WC(deck_fill(0,visible)==5&&!deck_service_visible(20),"Undocked FLY hides Disembark without overflowing list");
 game.tutorial_step=1;WC(!deck_service_visible(27)&&!deck_service_visible(28),"Tutorial hides multiplayer");game.tutorial_step=0;
 WC(sizeof(WifiPacket)<1400,"Bounded datagram fits small PDP packet");
 WifiPacket p;wifi_packet(&p,WIFI_PLAYER);
 WC(wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Valid pilot packet");
 WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)-1)&&!wifi_packet_valid(&p,wifi_wire_bytes(&p)+1),"Reject truncated and oversized datagrams");
 p.version++;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject mismatched versions");p.version--;
 p.pilot.pos.x=NAN;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject NaN position");p.pilot.pos.x=0;
 p.pilot.pitch=INFINITY;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject infinite angles");p.pilot.pitch=0;
 p.pilot.ship=player_ship_count;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject invalid ship index");p.pilot.ship=0;
 memset(p.pilot.name,'A',25);WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject unterminated pilot name");p.pilot=wifi_local();
 p.type=99;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject unknown message type");
 WC(wifi_newer(1,0xfffffff0u)&&!wifi_newer(0xfffffff0u,1)&&!wifi_newer(1,1),"Sequence ordering handles wrap and duplicates");
 WC(fabsf(wifi_angle(3.13f,-3.13f,.5f)-3.14159265f)<.02f,"Rotation interpolation takes shortest arc");
 wifi_packet(&p,WIFI_NPCS);p.count=6;memcpy(p.data.npc,game.npc,6*sizeof(NPC));
 WC(wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Real generated NPC packet passes validation");
 p.first=47;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject NPC chunk overflowing array");p.first=0;
 p.first=0xfffffffcu;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject wrapped unsigned NPC index");p.first=0;
 p.data.npc[0].mesh=mesh_count;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject invalid mesh before rendering");
 wifi_packet(&p,WIFI_DEBRIS);p.count=8;memcpy(p.data.debris,game.debris,8*sizeof(Debris));WC(wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Real debris chunk passes validation");
 p.data.debris[0].radius=NAN;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject corrupt debris geometry");
 {int valid=1;for(int system=0;system<256;system++){game.system=system;game_spawn(&game);for(int i=0;i<NPC_COUNT;i++)valid&=wifi_npc_valid(&game.npc[i]);for(int first=0;first<DEBRIS_COUNT;first+=8){wifi_packet(&p,WIFI_DEBRIS);p.first=first;p.count=8;memcpy(p.data.debris,game.debris+first,8*sizeof(Debris));valid&=wifi_packet_valid(&p,wifi_wire_bytes(&p));}}WC(valid,"All 256 generated systems pass NPC/debris wire validation");}
 game.system=7;system_bodies(&game);game.credits=123456;snprintf(game.commander_name,25,"Ben");game.fit[0]=2;game.social.serial=81;
 WifiPacket welcome;wifi_packet(&welcome,WIFI_WELCOME);welcome.session=77;welcome.pilot.system=19;welcome.count=BODY_COUNT;welcome.data.world.system=19;welcome.data.world.variant=0;welcome.data.world.time=120;welcome.data.world.clock=36000;
 Game *host=malloc(sizeof(Game));WC(host!=NULL,"Allocate test host world");if(!host){fclose(f);return 1;}game_init(host);host->system=19;system_bodies(host);memcpy(welcome.data.world.bodies,host->bodies,sizeof(host->bodies));
 WC(wifi_packet_valid(&welcome,sizeof(welcome)),"Welcome has validated host planets and star");
 wifi.backup=malloc(sizeof(Game));WC(wifi.backup!=NULL,"Allocate protected commander snapshot");if(!wifi.backup){free(host);fclose(f);return 1;}
 *wifi.backup=game;wifi.saved_page=HOME;wifi.saved_row=28;wifi.state=WIFI_CONNECT;wifi.host=0;unsigned char mac[6]={2,3,4,5,6,7};
 wifi_receive(&welcome,mac);
 WC(wifi.state==WIFI_GUEST&&wifi.ready&&game.system==19&&page==FLIGHT,"Guest joins host system and enters flight");
 WC(game.credits==123456&&!strcmp(game.commander_name,"Ben")&&game.fit[0]==2,"Guest keeps personal identity and ship equipment");
 WC(length(sub(game.bodies[1].pos,host->bodies[1].pos))==0,"Guest planets match host universe");
 wifi_packet(&p,WIFI_NPCS);p.sequence=100;p.count=6;memcpy(p.data.npc,host->npc,6*sizeof(NPC));p.data.npc[0].pos=(Vec3){10,20,30};wifi_receive(&p,mac);
 WC(game.npc[0].pos.x==10,"Host snapshot populates guest NPC");
 p.sequence=99;p.data.npc[0].pos.x=99;wifi_receive(&p,mac);WC(wifi.npc_goal[0].x==10,"Stale snapshot does not roll back NPC state");
 p.sequence=101;unsigned char outsider[6]={9,9,9,9,9,9};wifi_receive(&p,outsider);WC(game.npc[0].pos.x==10,"Ignore unrelated peer snapshot");
 p.session=78;wifi_receive(&p,mac);WC(game.npc[0].pos.x==10,"Ignore packets from other session");
 p.session=77;p.sequence=102;p.pilot.system=7;p.data.npc[0].pos.x=777;wifi_receive(&p,mac);WC(game.npc[0].pos.x==10,"Other-system snapshots cannot replace local ships");
 p.sequence=103;p.pilot.system=19;p.pilot.epoch=2;p.data.npc[0].pos.x=22;wifi_receive(&p,mac);WC(game.npc[0].pos.x==22,"New host visit snaps fresh NPCs instead of blending old world goals");
 p.sequence=104;p.pilot.epoch=1;p.data.npc[0].pos.x=333;wifi_receive(&p,mac);WC(game.npc[0].pos.x==22,"Old visit epochs cannot roll back current world");
 wifi.remote=wifi.display=wifi_local();wifi.remote.system=19;wifi.ready=1;wifi.quiet=0;
 WC(valid_target(PLAYER_TARGET_ID)&&!IS_STATION_ID(PLAYER_TARGET_ID)&&target_category(PLAYER_TARGET_ID)==1,"Friend is a real ship contact, not a station or array overflow");
 int contacts[TARGET_CAPACITY];WC(collect_scan_ids(contacts,1)>0&&contacts[0]==PLAYER_TARGET_ID,"Friend is first in targeting computer SHIPS band");
 int tracking=1;for(int sys=0;sys<256;sys++){game.system=sys;wifi.remote.system=(sys+1)%256;game.route_goal=-1;wifi_contact_route();if(route_target_ready())tracking&=dot(norm(sub(wifi_contact_position(),game.pos)),route_target_direction())>.9999f;tracking&=wifi_vec(wifi_contact_position());}
 WC(tracking,"Friend navigation stays finite and matches hyperdrive vector in all 256 systems");
 game.system=19;system_bodies(&game);wifi.remote.system=19;wifi.remote.planet=1;game.planet=-1;WC(length(sub(wifi_contact_position(),game.bodies[1].pos))==0,"Friend on a planet targets the correct orbital world");
 game.planet=1;wifi.remote.surface=2;surface_target_cat=0;int sites[20],sc=surface_target_list(sites);WC(sc==12&&sites[11]==19&&wifi_surface_contact_local(),"Planet targeting computer lists friend alongside normal sites");
 game.planet=-1;game.docked=1;game.local_pilot=2;game.credits=10000000;game.systems[game.system].tech=15;
 WC(buy_ship(&game,1),"Connected commander can purchase a new ship");
 int weapon=-1,defense=-1;for(int i=1;i<EQUIP_COUNT;i++)if(equipment_in_stock(i)&&!equipment_owned(i)){if(weapon<0&&equip_slot_for(i)==FIT_WPN)weapon=i;if(defense<0&&equip_slot_for(i)==FIT_DEF)defense=i;}
 equip_target=-1;if(weapon>0)buy_equipment(weapon,0);if(defense>0)buy_equipment(defense,0);
 WC(weapon>0&&defense>0&&fit_find(&game,weapon)>=0&&fit_find(&game,defense)>=0,"Normal outfitting purchases work while connected");
 game.discoveries+=2;game.surface_progress[19][1]|=1u<<8;game.station_progress[19][0]=1;game.rift_logged[19]|=1;game.social.serial=999;
 WC(save_game(&game,"wifi-persistent.sav"),"Connected commander can save normally at station");
 WC(load_game(host,"wifi-persistent.sav")&&host->ship==game.ship&&host->credits==game.credits&&!memcmp(host->fit,game.fit,sizeof(game.fit))&&host->discoveries==game.discoveries&&host->surface_progress[19][1]==game.surface_progress[19][1]&&host->station_progress[19][0]==1&&host->social.serial==999,"Saved ship/equipment/credits/discoveries/activity/social progress reload");
 remove("wifi-persistent.sav");remove("wifi-persistent.sav.bak");
 int cash=game.credits,ship=game.ship,discovered=game.discoveries;uint8_t fitted[sizeof(game.fit)];memcpy(fitted,game.fit,sizeof(game.fit));page=EQUIP;row=4;Vec3 position=game.pos;selected_target=PLAYER_TARGET_ID;autoaim=1;wifi_close(NULL);
 WC(game.system==19&&game.credits==cash&&game.ship==ship&&!memcmp(fitted,game.fit,sizeof(game.fit))&&game.discoveries==discovered&&game.social.serial==999&&!wifi.state&&!wifi.backup,"Disconnect keeps purchased hull/modules and all commander progress");
 WC(page==EQUIP&&row==4&&length(sub(game.pos,position))==0&&!wifi_release,"Disconnect preserves current menu/position/input");
 WC(!valid_target(PLAYER_TARGET_ID)&&selected_target!=PLAYER_TARGET_ID&&!autoaim&&!game.peer_surface&&!game.surface_replica,"Disconnect removes only friend contact and replica state");
 for(int role=1;role<=2;role++){game_init(&game);story_complete(&game);launch(&game);game.dock_stage=0;game.local_pilot=role;game.approach=1;enter_planet(&game);game.pos=surface_landing_pad(&game);game.speed=0;land_planet(&game);disembark_planet(&game);game.credits=777;page=FLIGHT;Vec3 p0=game.pos,pad=game.ship_pos,car=game.rover_pos;wifi.state=WIFI_HOST;wifi.playing=1;game.surface_replica=game.peer_surface=1;planet_entry_body=1;planet_entry_time=.3f;
 wifi_close(NULL);WC(game.planet==1&&game.surface==2&&game.local_pilot==role&&game.credits==777&&page==FLIGHT&&length(sub(p0,game.pos))==0&&length(sub(pad,game.ship_pos))==0&&length(sub(car,game.rover_pos))==0&&planet_entry_body==1&&!game.surface_replica&&!game.peer_surface,"Planet disconnect keeps host/guest berth, ship, rover, position and cinematic");planet_entry_body=0;}
 game_init(&game);wifi.backup=malloc(sizeof(Game));*wifi.backup=game;wifi.saved_page=HOME;wifi.saved_row=28;wifi.state=WIFI_CONNECT;game.credits=1;wifi_close(NULL);
 WC(game.credits==1000&&page==HOME&&row==28&&!wifi.backup,"Setup failure remains reversible");
 WC(wifi_release&&wifi_input(0,0,.016f,0,0)&&!wifi_release,"Setup cancel consumes only the dismissal button");
 int bays=1,exits=1,cars=1;Vec3 previous={0};
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++)for(int pilot=1;pilot<=2;pilot++){
  game_init(host);story_complete(host);host->system=sys;system_bodies(host);launch(host);host->local_pilot=pilot;host->approach=body;
  int entered=enter_planet(host);Vec3 bay=surface_landing_pad(host);host->pos=bay;host->speed=0;
  int landed=entered&&land_planet(host);bays&=landed&&!terrain_is_water(host,bay.x,bay.z);
  if(pilot==1)previous=host->rover_pos;else cars&=length(sub(previous,host->rover_pos))>=39.99f;
  exits&=landed&&disembark_planet(host);
 }
 WC(bays,"All 1024 planets have two dry usable player berths");WC(exits,"Both player berths have a collision-validated disembark exit on all planets");WC(cars,"All starports allocate two distinct Roamers");
 game_init(&game);story_complete(&game);launch(&game);wifi_packet(&p,WIFI_PLAYER);p.pilot.planet=BODY_COUNT;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject invalid surface planet");p.pilot=wifi_local();p.pilot.rover=1;WC(!wifi_packet_valid(&p,wifi_wire_bytes(&p)),"Reject a Roamer driving in orbit");
 game_init(&game);story_complete(&game);launch(&game);game.system=7;system_bodies(&game);game.approach=1;game.local_pilot=1;enter_planet(&game);game.pos=surface_landing_pad(&game);game.speed=0;land_planet(&game);disembark_planet(&game);
 Vec3 pad=surface_site(&game,1);game.pos=add(pad,(Vec3){0,0,150});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=0;page=FLIGHT;fb=(unsigned*)0x44000000u;
 wifi.state=WIFI_HOST;wifi.ready=1;wifi.remote=wifi.display=wifi_local();wifi.remote.pos=wifi.display.pos=add(game.pos,(Vec3){0,0,90});wifi.remote.parked=wifi.display.parked=add(pad,(Vec3){FIELD_TRAFFIC_X,0,FIELD_TRAFFIC_Z});wifi.remote.car=wifi.display.car=add(game.pos,(Vec3){55,0,90});wifi.remote.yaw=wifi.display.yaw=3.14159f;wifi.remote.speed=wifi.display.speed=20;
 planet_view();dump_native_bmp("wifi-surface-avatar.bmp");wifi_close(NULL);
 free(host);game_init(&game);story_complete(&game);page=HOME;row=27;fb=(unsigned*)0x44000000u;home();dump_native_bmp("wifi-fly-buttons.bmp");
 wifi.state=WIFI_SCAN;wifi.scan_done=1;wifi.rooms_n=2;strcpy(wifi.rooms[0].group,"AE123456");strcpy(wifi.rooms[1].group,"AEABCDEF");strcpy(wifi.note,"Choose a nearby flight.");wifi.choice=1;wifi_screen();dump_native_bmp("wifi-join-screen.bmp");wifi_close(NULL);
 fprintf(f,"INFO %d checks, packet %u bytes, protected Game %u bytes. Native Wi-Fi not exercised by fixtures.\n",checks,(unsigned)sizeof(WifiPacket),(unsigned)sizeof(Game));
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);
#undef WC
 return 1;
}
