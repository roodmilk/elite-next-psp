/* Two PSPs, ad-hoc PDP, bounded nonblocking traffic. No saves or pointers on wire.
 * Session flight is deliberately a normal commander progression; only setup failures roll back.
 * Both peers must run the same protocol build. Local Wi-Fi is not authenticated. */
#include <stddef.h>
#include <pspnet.h>
#include <pspnet_adhoc.h>
#include <pspnet_adhocctl.h>
#include <pspwlan.h>
#include <psputility_netmodules.h>
#define WIFI_MAGIC 0x41454e58u
#define WIFI_VERSION 263u
#define WIFI_PORT 25925
enum { WIFI_OFF,WIFI_SCAN,WIFI_CONNECT,WIFI_HOST,WIFI_GUEST };
enum { WIFI_HELLO=1,WIFI_WELCOME,WIFI_PLAYER,WIFI_NPCS,WIFI_DEBRIS,WIFI_BYE,WIFI_FULL,WIFI_LIFE };
typedef struct { Vec3 pos;float yaw,pitch,roll,speed;uint32_t paint;int32_t ship;char name[25];
 int32_t system,planet,surface,rover,sequence,jumping,docked;uint32_t epoch;Vec3 parked,car;float car_yaw;
} WifiPilot;
typedef struct { int32_t system,variant;float time,clock;Body bodies[BODY_COUNT];Anomaly anomaly[ANOMALY_COUNT]; } WifiWorld;
typedef struct {
 uint32_t magic,version,session,sequence,type,first,count;
 WifiPilot pilot;
 union { WifiWorld world;NPC npc[6];Debris debris[8];Lifeform life[LIFE_COUNT]; } data;
} WifiPacket;
_Static_assert(sizeof(WifiPacket)<1400,"PDP packet must fit a single small datagram");
typedef struct {char group[9];int channel;} WifiRoom;
static struct {
 int state,playing,host,common,module,adhoc,net,ctl,pdp,paired,ready,scan_done,choice,rooms_n,burst;
 unsigned char mac[8],peer[6];char group[9],note[96];
 uint32_t session,sequence,seen[18],pilot_seen,world_seen;
 float age,quiet,send_age,world_age,hello_age,scan_age;uint64_t last_poll;uint32_t epoch;int last_system,replica_system;uint32_t replica_epoch,npc_received;
 WifiRoom rooms[8];WifiPilot remote,display;
 Vec3 npc_goal[NPC_COUNT],npc_dir[NPC_COUNT];
 Game *backup;int saved_page,saved_row;
 unsigned surface_held;float target_hold;
} wifi={.pdp=-1};
static int wifi_release=0;
static int wifi_ended=0;
static void wifi_surface_reset(void);
static int wifi_vec(Vec3 v){return isfinite(v.x)&&isfinite(v.y)&&isfinite(v.z)&&fabsf(v.x)<1e7f&&fabsf(v.y)<1e7f&&fabsf(v.z)<1e7f;}
static int wifi_float(float v,float bound){return isfinite(v)&&fabsf(v)<=bound;}
static int wifi_newer(uint32_t a,uint32_t b){return (int32_t)(a-b)>0;}
static int wifi_pilot_valid(const WifiPilot *p){
 if(!wifi_vec(p->pos)||!wifi_float(p->yaw,10000)||!wifi_float(p->pitch,4)||!wifi_float(p->roll,10000)||!wifi_float(p->speed,50000)||p->speed<0||p->ship<0||p->ship>=player_ship_count||p->name[24])return 0;
 for(int i=0;i<24&&p->name[i];i++)if((unsigned char)p->name[i]<32||(unsigned char)p->name[i]>126)return 0;
 if(p->system<0||p->system>255||p->jumping<0||p->jumping>1||p->docked<0||p->docked>1)return 0;
 if(p->planet< -1||p->planet==0||p->planet>=BODY_COUNT||p->surface<0||p->surface>2||p->rover<0||p->rover>1||p->sequence<0||p->sequence>3||!wifi_vec(p->parked)||!wifi_vec(p->car)||!wifi_float(p->car_yaw,10000))return 0;
 if((p->planet<0&&(p->surface||p->rover||p->sequence))||(p->rover&&p->surface!=2))return 0;
 return 1;
}
static int wifi_world_valid(const WifiWorld *w){
 if(w->system<0||w->system>255||w->variant<0||w->variant>=HUB_COUNT||!wifi_float(w->time,1e8f)||!wifi_float(w->clock,86400))return 0;
 for(int i=0;i<BODY_COUNT;i++){
  const Body *b=&w->bodies[i];int active=system_body_active_slot(w->system,i);
  if(!wifi_vec(b->pos)||!wifi_float(b->radius,1e6f)||b->type<SUN||b->type>GAS||b->name[23])return 0;
  if(active?(b->radius<=0):(b->radius!=0))return 0;
 }
 for(int i=0;i<ANOMALY_COUNT;i++)if(!wifi_vec(w->anomaly[i].pos)||w->anomaly[i].alive<0||w->anomaly[i].alive>1||w->anomaly[i].kind<0||w->anomaly[i].kind>15)return 0;
 return 1;
}
static int wifi_npc_valid(const NPC *n){
 if(!wifi_vec(n->pos)||!wifi_vec(n->dir)||!wifi_vec(n->freight_gate)||!wifi_vec(n->freight_berth))return 0;
 const float f[]={n->health,n->shield,n->cooldown,n->flash,n->scale,n->radius,n->cruise,n->route_wait,n->player_tag,n->escape_time,n->freight_timer};
 for(unsigned i=0;i<sizeof(f)/sizeof(f[0]);i++)if(!wifi_float(f[i],1e7f))return 0;
 return n->alive>=0&&n->alive<=1&&n->role>=0&&n->role<FACTION_COUNT&&n->mesh>=0&&n->mesh<mesh_count&&n->scale>=0&&n->scale<100&&n->radius>=0&&n->radius<50000&&n->freighter>=0&&n->freighter<=1&&n->freight_style>=0&&n->freight_style<3&&n->freight_state>=0&&n->freight_state<=FREIGHT_CHARGING&&n->freight_hub>=0&&n->freight_hub<HUB_COUNT;
}
static int wifi_packet_valid(const WifiPacket *p,int bytes){
 if(bytes<(int)offsetof(WifiPacket,data))return 0;
 int expected=(p->type==WIFI_WELCOME||p->type==WIFI_NPCS||p->type==WIFI_DEBRIS||p->type==WIFI_LIFE)?sizeof(*p):offsetof(WifiPacket,data);
 if(bytes!=expected||p->magic!=WIFI_MAGIC||p->version!=WIFI_VERSION||!wifi_pilot_valid(&p->pilot))return 0;
 switch(p->type){
 case WIFI_LIFE:if(p->count!=LIFE_COUNT||p->first||p->pilot.planet<1)return 0;for(int i=0;i<LIFE_COUNT;i++){const Lifeform *l=&p->data.life[i];if(!wifi_vec(l->pos)||!wifi_vec(l->home)||!wifi_vec(l->goal)||l->alive<0||l->alive>1||l->kind<0||l->kind>2||!wifi_float(l->heading,10000)||!wifi_float(l->speed,10000)||!wifi_float(l->phase,1e8f)||!wifi_float(l->state_time,1e8f)||!wifi_float(l->calm_time,1e8f)||!wifi_float(l->lift,10000))return 0;}return 1;
 case WIFI_HELLO:case WIFI_PLAYER:case WIFI_BYE:case WIFI_FULL:return p->count==0&&p->first==0;
 case WIFI_WELCOME:return p->session&&p->count==BODY_COUNT&&p->first==0&&wifi_world_valid(&p->data.world)&&p->data.world.system==p->pilot.system;
 case WIFI_NPCS:if(p->count!=6||p->first%6||p->first>NPC_COUNT-6)return 0;for(unsigned i=0;i<6;i++)if(!wifi_npc_valid(&p->data.npc[i]))return 0;return 1;
 case WIFI_DEBRIS:if(p->count!=8||p->first%8||p->first>DEBRIS_COUNT-8)return 0;for(int i=0;i<8;i++){const Debris *d=&p->data.debris[i];if(!wifi_vec(d->pos)||!wifi_vec(d->vel)||!wifi_float(d->life,1e7f)||!wifi_float(d->health,1e7f)||!wifi_float(d->radius,1e5f)||!wifi_float(d->flash,1e7f)||d->alive<0||d->alive>1||d->good<0||d->good>=GOODS||d->qty<0||d->qty>100000||d->rock<0||d->rock>2||d->wreck<0||d->wreck>1)return 0;}return 1;
 default:return 0;
 }
}
static WifiPilot wifi_local(void){
 WifiPilot p={0};p.pos=game.pos;p.yaw=game.yaw;p.pitch=game.pitch;p.roll=game.roll;p.speed=game.speed;p.ship=game.ship;p.paint=ship_paint[game.ship];
 p.system=game.system;p.epoch=wifi.epoch;p.jumping=game.jump>0;p.docked=game.docked;p.planet=game.planet;p.surface=game.surface;p.rover=game.rover_driving;p.sequence=game.planet_sequence;p.parked=game.ship_pos;p.car=game.rover_pos;p.car_yaw=game.rover_driving?game.yaw:0;
 snprintf(p.name,sizeof(p.name),"%.24s",game.commander_name);return p;
}
static void wifi_packet(WifiPacket *p,int type){
 memset(p,0,sizeof(*p));p->magic=WIFI_MAGIC;p->version=WIFI_VERSION;p->session=wifi.session;p->sequence=++wifi.sequence;p->type=type;p->pilot=wifi_local();
}
static int wifi_wire_bytes(const WifiPacket *p){return p->type==WIFI_WELCOME||p->type==WIFI_NPCS||p->type==WIFI_DEBRIS||p->type==WIFI_LIFE?sizeof(*p):offsetof(WifiPacket,data);}
static void wifi_send(WifiPacket *p,unsigned char *mac){if(wifi.pdp>=0)sceNetAdhocPdpSend(wifi.pdp,mac,WIFI_PORT,p,wifi_wire_bytes(p),0,1);}
static void wifi_close(const char *why){
 if(!wifi.state&&!wifi.backup&&!wifi.common&&!wifi.net)return;
 int playing=wifi.playing,lobby=page==MULTIPLAYER;
 wifi_ended=why!=NULL;
 if(wifi.paired&&wifi.pdp>=0){WifiPacket p;wifi_packet(&p,WIFI_BYE);wifi_send(&p,wifi.peer);}
 if(wifi.pdp>=0)sceNetAdhocPdpDelete(wifi.pdp,0);
 if(wifi.ctl){sceNetAdhocctlDisconnect();sceNetAdhocctlTerm();}
 if(wifi.adhoc)sceNetAdhocTerm();
 if(wifi.net)sceNetTerm();
 if(wifi.module)sceUtilityUnloadNetModule(PSP_NET_MODULE_ADHOC);
 if(wifi.common)sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
 Game *saved=wifi.backup;int sp=wifi.saved_page,sr=wifi.saved_row;
 /* Active play is the real commander: never replace it with the join snapshot.
  * Keep planetary berth ownership until orbit, otherwise ambient traffic can
  * reappear inside the player's parked ship. Only replica/contact state ends. */
 game.peer_surface=game.surface_replica=0;
 if(game.planet<1)game.local_pilot=0;
 if(selected_target==PLAYER_TARGET_ID){selected_target=0;autoaim=0;}
 if(surface_target_lock==19){surface_target_lock=-1;surface_turn_active=0;}
 memset(&wifi,0,sizeof(wifi));wifi.pdp=-1;
 if(saved){if(!playing){game=*saved;page=sp;row=sr;paused=0;analog_ready=0;wifi_release=1;}free(saved);}
 else if(lobby){page=game.docked?HOME:FLIGHT;wifi_release=1;}
 if(why){snprintf(wifi.note,sizeof(wifi.note),"%s",why);message(&game,why);}
}
static int wifi_available(void){return !game.tutorial_step&&!game.dead&&game.planet<0&&!game.dock_stage&&game.approach<0&&game.jump<=0&&!game.police_stop;}
static int wifi_init(int host){
 if(wifi.state){page=MULTIPLAYER;return 1;}
 online_close("Using direct local Wi-Fi");
 if(!wifi_available()){message(&game,"Use local Wi-Fi at a station or in normal space flight.");return 0;}
 if(!sceWlanGetSwitchState()){message(&game,"Turn on the PSP WLAN switch first.");return 0;}
 wifi_ended=0;
 wifi_close(NULL);wifi.backup=malloc(sizeof(Game));if(!wifi.backup){message(&game,"Not enough memory for a safe Wi-Fi session.");return 0;}
 *wifi.backup=game;wifi.saved_page=page;wifi.saved_row=row;wifi.host=host;wifi.pdp=-1;wifi.last_system=game.system;wifi.epoch=1;wifi.replica_system=-1;
 int error=sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);if(error<0)goto fail;wifi.common=1;
 error=sceUtilityLoadNetModule(PSP_NET_MODULE_ADHOC);if(error<0)goto fail;wifi.module=1;
 error=sceNetInit(256*1024,0x30,4096,0x30,4096);if(error<0)goto fail;wifi.net=1;
 error=sceNetAdhocInit();if(error<0)goto fail;wifi.adhoc=1;
 struct productStruct product={0};memcpy(product.product,"ELITENEXT",9);
 error=sceNetAdhocctlInit(0x2000,0x30,&product);if(error<0)goto fail;wifi.ctl=1;
 /* WLAN hardware address is available before a group exists. The Net-layer
  * address is not: asking for it here returns 80410180 on PSP-compatible HLE. */
 error=sceWlanGetEtherAddr(wifi.mac);if(error<0)goto fail;
 wifi.session=host?(sceKernelGetSystemTimeLow()^((uint32_t)wifi.mac[3]<<16)^((uint32_t)wifi.mac[4]<<8)^wifi.mac[5])|1u:0;
 snprintf(wifi.group,sizeof(wifi.group),"AE%02X%02X%02X",wifi.mac[3],wifi.mac[4],wifi.mac[5]);
 if(host){error=sceNetAdhocctlCreate(wifi.group);wifi.state=WIFI_CONNECT;snprintf(wifi.note,sizeof(wifi.note),"Opening nearby flight...");}
 else {error=sceNetAdhocctlScan();wifi.state=WIFI_SCAN;snprintf(wifi.note,sizeof(wifi.note),"Looking for nearby hosts...");}
 if(error<0)goto fail;page=MULTIPLAYER;row=0;paused=0;return 1;
 fail:{char note[96];snprintf(note,sizeof(note),"Wi-Fi could not start (%08X). Try again.",(unsigned)error);wifi_close(note);return 0;}
}
static void wifi_flight_start(void){
 /* Launch normally, including its cinematic. No free repairs/fuel/amnesty. */
 if(game.docked)launch(&game);
 game.local_pilot=wifi.host?1:2;game.surface_replica=0;
 wifi.playing=1;if(wifi.backup){free(wifi.backup);wifi.backup=NULL;}
 page=FLIGHT;selected_target=0;autoaim=0;analog_ready=0;
}
static void wifi_welcome(void){
 WifiPacket p;wifi_packet(&p,WIFI_WELCOME);p.count=BODY_COUNT;p.data.world.system=game.system;p.data.world.variant=game.station_variant;p.data.world.time=game.time;p.data.world.clock=game.world_clock;
 memcpy(p.data.world.bodies,game.bodies,sizeof(game.bodies));memcpy(p.data.world.anomaly,game.anomaly,sizeof(game.anomaly));wifi_send(&p,wifi.peer);
}
static void wifi_receive(const WifiPacket *p,const unsigned char mac[6]){
 if(p->type==WIFI_HELLO&&wifi.host){
  if(wifi.paired&&memcmp(mac,wifi.peer,6)){WifiPacket full;wifi_packet(&full,WIFI_FULL);wifi_send(&full,(unsigned char*)mac);return;}
  if(!wifi.paired){memcpy(wifi.peer,mac,6);wifi.paired=1;wifi.remote=wifi.display=p->pilot;wifi.quiet=0;snprintf(wifi.note,sizeof(wifi.note),"%.24s is joining.",p->pilot.name);}
  wifi_welcome();return;
 }
 if(p->type==WIFI_FULL&&wifi.state==WIFI_CONNECT&&!wifi.host){wifi_close("That flight already has two players.");return;}
 if(p->type==WIFI_WELCOME&&!wifi.host&&(wifi.state==WIFI_CONNECT||wifi.state==WIFI_GUEST)){
  if(wifi.ready&&(p->session!=wifi.session||memcmp(mac,wifi.peer,6)))return;
  if(!wifi.ready){
   memcpy(wifi.peer,mac,6);wifi.session=p->session;wifi.paired=wifi.ready=1;wifi.state=WIFI_GUEST;
   game.wanted[game.system]=game.legal;game.system=p->data.world.system;
   game.legal=game.wanted[game.system];game.police_cargo_heat=game.crime_record[game.system]>>8;
   market(&game);game_spawn(&game);game.station_variant=p->data.world.variant;game.docked=game.dock_stage=0;
   wifi_flight_start();memcpy(game.bodies,p->data.world.bodies,sizeof(game.bodies));
   game.pos=add(p->pilot.pos,(Vec3){320,100,-500});game.yaw=p->pilot.yaw;game.pitch=p->pilot.pitch;
   if(p->pilot.planet>=1){Body *b=&game.bodies[p->pilot.planet];game.pos=add(b->pos,(Vec3){0,0,-b->radius-5000});game.yaw=game.pitch=0;}
   wifi.remote=wifi.display=p->pilot;
  }
  if(wifi.pilot_seen&&wifi_newer(wifi.remote.epoch,p->pilot.epoch))return;
  if(wifi.world_seen&&!wifi_newer(p->sequence,wifi.world_seen))return;
  wifi.world_seen=p->sequence;wifi.quiet=0;
  if(p->pilot.system==game.system&&game.jump<=0){game.world_clock=p->data.world.clock;memcpy(game.bodies,p->data.world.bodies,sizeof(game.bodies));memcpy(game.anomaly,p->data.world.anomaly,sizeof(game.anomaly));for(int i=0;i<ANOMALY_COUNT;i++)game.anomaly[i].scanned=(game.rift_logged[game.system]>>i)&1;}
  return;
 }
 if(!wifi.paired||memcmp(mac,wifi.peer,6)||p->session!=wifi.session)return;
 wifi.quiet=0; /* A validated peer is alive even when a snapshot is stale. */
 if(p->type==WIFI_BYE){wifi_close("Local flight ended. Your commander continues.");return;}
 if((wifi.host&&p->type!=WIFI_PLAYER)||(!wifi.host&&p->type!=WIFI_PLAYER&&p->type!=WIFI_NPCS&&p->type!=WIFI_DEBRIS&&p->type!=WIFI_LIFE))return;
 int slot=p->type==WIFI_LIFE?17:p->type==WIFI_NPCS?1+p->first/6:p->type==WIFI_DEBRIS?9+p->first/8:0;
 if(wifi.seen[slot]&&!wifi_newer(p->sequence,wifi.seen[slot]))return;
 if(wifi.pilot_seen&&wifi_newer(wifi.remote.epoch,p->pilot.epoch))return;
 wifi.seen[slot]=p->sequence;wifi.quiet=0;wifi.ready=1;
 if(!wifi.pilot_seen||wifi_newer(p->sequence,wifi.pilot_seen)){wifi.pilot_seen=p->sequence;if(wifi.remote.epoch!=p->pilot.epoch||wifi.remote.system!=p->pilot.system||wifi.remote.planet!=p->pilot.planet||wifi.remote.surface!=p->pilot.surface)wifi.display=p->pilot;wifi.remote=p->pilot;}
 if(!wifi.host&&p->pilot.system==game.system&&(wifi.replica_system!=game.system||wifi.replica_epoch!=p->pilot.epoch)){wifi.replica_system=game.system;wifi.replica_epoch=p->pilot.epoch;wifi.npc_received=0;}
 if(!wifi.host&&p->type==WIFI_LIFE&&p->pilot.system==game.system&&game.planet==p->pilot.planet)for(int i=0;i<LIFE_COUNT;i++){int scanned=game.life[i].scanned;game.life[i]=p->data.life[i];game.life[i].scanned=scanned;}
 if(!wifi.host&&p->type==WIFI_NPCS&&p->pilot.system==game.system&&game.jump<=0)for(int i=0;i<6;i++){int id=p->first+i;Vec3 pos=game.npc[id].pos,dir=game.npc[id].dir;int alive=game.npc[id].alive&&(wifi.npc_received&(1u<<(p->first/6)));game.npc[id]=p->data.npc[i];wifi.npc_goal[id]=game.npc[id].pos;wifi.npc_dir[id]=game.npc[id].dir;if(alive&&game.npc[id].alive){game.npc[id].pos=pos;game.npc[id].dir=dir;}}
 if(!wifi.host&&p->type==WIFI_NPCS&&p->pilot.system==game.system&&game.jump<=0)wifi.npc_received|=1u<<(p->first/6);
 if(!wifi.host&&p->type==WIFI_DEBRIS&&p->pilot.system==game.system&&game.jump<=0)memcpy(game.debris+p->first,p->data.debris,8*sizeof(Debris));
}
static float wifi_angle(float from,float to,float blend){float d=fmodf(to-from,6.2831853f);if(d>3.14159265f)d-=6.2831853f;if(d< -3.14159265f)d+=6.2831853f;return from+d*blend;}
static void wifi_scan_results(void){
 union {void *align;unsigned char bytes[8192];} scan;
 int len=sizeof(scan.bytes);if(sceNetAdhocctlGetScanInfo(&len,scan.bytes)<0||len<0||len>sizeof(scan.bytes))return;
 wifi.rooms_n=0;
 struct SceNetAdhocctlScanInfo *entry=len?(void*)scan.bytes:NULL;
 for(int i=0;entry&&i<64;i++){
  uintptr_t start=(uintptr_t)scan.bytes,end=start+(unsigned)len,ptr=(uintptr_t)entry;
  if(len<sizeof(*entry)||ptr<start||ptr>end-sizeof(*entry)||ptr%4)break;
  if(entry->name[0]=='A'&&entry->name[1]=='E'&&wifi.rooms_n<8){
   int valid=1;for(int k=2;k<8;k++)if(!((entry->name[k]>='0'&&entry->name[k]<='9')||(entry->name[k]>='A'&&entry->name[k]<='F')))valid=0;
   int dup=0;for(int k=0;k<wifi.rooms_n;k++)if(!memcmp(wifi.rooms[k].group,entry->name,8))dup=1;
   if(valid&&!dup){WifiRoom *r=&wifi.rooms[wifi.rooms_n++];memcpy(r->group,entry->name,8);r->group[8]=0;r->channel=entry->channel;}
  }
  entry=entry->next;
 }
 wifi.scan_done=1;wifi.choice=0;snprintf(wifi.note,sizeof(wifi.note),wifi.rooms_n?"Choose a nearby flight.":"No hosts yet. Square searches again.");
}
static void wifi_poll(float dt){
 if(!wifi.state)return;
 uint64_t now=sceKernelGetSystemTimeWide();
 if(wifi.last_poll)dt=fmaxf(0,fminf(120,(now-wifi.last_poll)/1000000.f));
 wifi.last_poll=now;
 if(game.system!=wifi.last_system){wifi.last_system=game.system;wifi.epoch++;wifi.burst=0;wifi.npc_received=0;wifi.replica_system=-1;game.peer_surface=game.surface_replica=0;}
 wifi.age+=dt;wifi.quiet+=dt;wifi.send_age+=dt;wifi.world_age+=dt;wifi.hello_age+=dt;
 if(!sceWlanGetSwitchState()){wifi_close("WLAN switched off. Your commander continues.");return;}
 int state=0;if(sceNetAdhocctlGetState(&state)<0){wifi_close("Wi-Fi connection lost. Your commander continues.");return;}
 if(wifi.state==WIFI_SCAN){wifi.scan_age+=dt;if(state==0&&wifi.scan_age>1&&!wifi.scan_done)wifi_scan_results();if(wifi.scan_age>45&&!wifi.scan_done)wifi_close("Search timed out. Check the host and WLAN switch.");return;}
 if(wifi.state==WIFI_CONNECT&&state==1&&wifi.pdp<0){
  wifi.pdp=sceNetAdhocPdpCreate(wifi.mac,WIFI_PORT,32768,0);
  if(wifi.pdp<0){wifi_close("Could not open the local flight link.");return;}
  if(wifi.host){wifi.state=WIFI_HOST;wifi_flight_start();page=MULTIPLAYER;snprintf(wifi.note,sizeof(wifi.note),"Host ready. Friend: choose Join on your PSP.");}
 }
 if(wifi.pdp>=0){
  for(int i=0;i<24;i++){WifiPacket p={0};int size=sizeof(p);unsigned char mac[8]={0};unsigned short port=0;
   int r=sceNetAdhocPdpRecv(wifi.pdp,mac,&port,&p,&size,0,1);if(r<0)break;
   if(port==WIFI_PORT&&memcmp(mac,wifi.mac,6)&&wifi_packet_valid(&p,size))wifi_receive(&p,mac);
   if(!wifi.state)return;
  }
  if(!wifi.host&&!wifi.ready&&wifi.hello_age>=.5f){WifiPacket p;unsigned char broadcast[6]={255,255,255,255,255,255};wifi_packet(&p,WIFI_HELLO);wifi_send(&p,broadcast);wifi.hello_age=0;}
  if(wifi.paired&&wifi.send_age>=.05f){WifiPacket p;wifi_packet(&p,WIFI_PLAYER);wifi_send(&p,wifi.peer);wifi.send_age=0;}
  /* Spread world snapshots over frames: at most two chunks, rather than
   * sixteen driver calls spiking a single PSP frame every 200ms. */
  if(wifi.host&&wifi.paired){
   if(!wifi.burst&&wifi.world_age>=.20f){wifi.burst=1;wifi.world_age=0;}
   for(int budget=0;budget<2&&wifi.burst;budget++){
    int chunk=wifi.burst++;WifiPacket p;
    if(chunk==1)wifi_welcome();
    else if(chunk<=9){wifi_packet(&p,WIFI_NPCS);p.first=(chunk-2)*6;p.count=6;memcpy(p.data.npc,game.npc+p.first,6*sizeof(NPC));wifi_send(&p,wifi.peer);}
    else if(chunk<=17){wifi_packet(&p,WIFI_DEBRIS);p.first=(chunk-10)*8;p.count=8;memcpy(p.data.debris,game.debris+p.first,8*sizeof(Debris));wifi_send(&p,wifi.peer);}
    else if(game.planet>=1){wifi_packet(&p,WIFI_LIFE);p.count=LIFE_COUNT;memcpy(p.data.life,game.life,sizeof(game.life));wifi_send(&p,wifi.peer);}
    if(wifi.burst>18)wifi.burst=0;
   }
  }
 }
 if((wifi.paired&&wifi.quiet>60)||(wifi.state==WIFI_CONNECT&&wifi.age>60)){wifi_close("Connection timed out. Your commander continues.");return;}
 if(wifi.ready){float q=1-expf(-dt*12);wifi.display.car=add(wifi.display.car,mul(sub(wifi.remote.car,wifi.display.car),q));wifi.display.car_yaw=wifi_angle(wifi.display.car_yaw,wifi.remote.car_yaw,q);wifi.display.parked=wifi.remote.parked;wifi.display.system=wifi.remote.system;wifi.display.jumping=wifi.remote.jumping;wifi.display.docked=wifi.remote.docked;wifi.display.planet=wifi.remote.planet;wifi.display.surface=wifi.remote.surface;wifi.display.rover=wifi.remote.rover;wifi.display.sequence=wifi.remote.sequence;wifi.display.speed=wifi.remote.speed;}
 if(wifi.ready){float blend=1-expf(-dt*12);wifi.display.pos=add(wifi.display.pos,mul(sub(wifi.remote.pos,wifi.display.pos),blend));wifi.display.yaw=wifi_angle(wifi.display.yaw,wifi.remote.yaw,blend);wifi.display.pitch+=(wifi.remote.pitch-wifi.display.pitch)*blend;wifi.display.roll=wifi_angle(wifi.display.roll,wifi.remote.roll,blend);wifi.display.ship=wifi.remote.ship;wifi.display.paint=wifi.remote.paint;memcpy(wifi.display.name,wifi.remote.name,25);if(!wifi.host&&wifi.remote.system==game.system&&game.jump<=0)for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive&&(wifi.npc_received&(1u<<(i/6)))){game.npc[i].pos=add(game.npc[i].pos,mul(sub(wifi.npc_goal[i],game.npc[i].pos),blend));game.npc[i].dir=norm(add(game.npc[i].dir,mul(sub(wifi.npc_dir[i],game.npc[i].dir),blend)));}}
}
static int wifi_land(int body){
 if(body<1||body>=BODY_COUNT||game.planet>=0||planet_transfer_busy())return 0;
 /* Session travel uses the existing cloud/first-person approach, not a teleport
  * onto a shared pad. Orbit remains the player's own return location. */
 game.approach=body;game.speed=0;planet_entry_begin();page=FLIGHT;return planet_entry_body==body;
}
/* Only lobby and protected-save actions intercept the normal controls. */
static int wifi_input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 if(wifi_release){if(!held)wifi_release=0;return 1;}
 if(!wifi.state)return 0;
 wifi_poll(dt);if(!wifi.state)return 1;
 (void)ax;(void)ay;
 if(page==MULTIPLAYER)return 1; /* Canonical console owns lobby input. */
 /* Connected sessions use game_input verbatim: the normal Square/Circle,
  * Triangle, menus, map, docking, planet and hyperspace controls. */
 game.local_pilot=wifi.host?1:2;
 game.peer_surface=wifi.ready&&wifi.remote.system==game.system&&game.planet>=1&&wifi.remote.planet==game.planet&&wifi.remote.surface&&!wifi.remote.sequence;
 game.surface_replica=!wifi.host&&wifi.ready&&wifi.remote.system==game.system&&wifi.remote.planet==game.planet&&game.planet>=1;
 if(game.peer_surface){const Mesh *h=&meshes[mesh_id(player_ships[wifi.remote.ship].name)];float w=0,d=0;for(int i=0;i<h->vertices;i++){w=fmaxf(w,fabsf(h->v[i].x)*.72f);d=fmaxf(d,fabsf(h->v[i].z)*.72f);}game.peer_ship=wifi.remote.parked;game.peer_ship_w=w;game.peer_ship_d=d;game.peer_car=wifi.remote.car;game.peer_car_yaw=wifi.remote.car_yaw;}
 if(page==STATUS&&game.docked&&(pressed&PSP_CTRL_TRIANGLE))wifi_close("Wi-Fi ended for commander load.");
 if(game.dead&&(pressed&PSP_CTRL_START))wifi_close("Wi-Fi ended.");
 return 0;
}

