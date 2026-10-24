/* Private PC test directory / P2P presence. No shared economy or NPC authority. */
#include <pspnet_apctl.h>
#include <pspnet_inet.h>
#include <psputility_netparam.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "online-address.h"
#define ONLINE_MAGIC 0x41454f4eu
#define ONLINE_LIMIT 8
enum { ONLINE_OK,ONLINE_WLAN,ONLINE_PROFILE,ONLINE_START,ONLINE_SERVER,ONLINE_FULL,ONLINE_LOST };
static int online_error=ONLINE_OK,online_save_dirty=0,online_save_desired=0;
typedef struct {uint32_t magic,version,type,id,token,sequence;WifiPilot pilot;uint32_t ip,port;} OnlinePacket;
_Static_assert(sizeof(WifiPilot)==124&&sizeof(OnlinePacket)==156,"Online wire layout changed");
typedef struct {uint32_t id,token,sequence;uint64_t seen;WifiPilot pilot,display;struct sockaddr_in address;} OnlinePeer;
static struct {int enabled,sock,stage,common,module,net,inet,apctl,selected,choice,profile,active_profile,profile_index,profile_count,profiles[100];uint32_t id,token,sequence;uint64_t started,last_send,last_server,retry;char host[32],note[80];struct sockaddr_in server;OnlinePeer peer[ONLINE_LIMIT];} online={.sock=-1,.selected=-1,.profile=1};
static uint64_t online_now(void){return sceKernelGetSystemTimeWide();}
static int online_peer_ready(int i){return online.stage==3&&i>=0&&i<ONLINE_LIMIT&&online.peer[i].id&&online_now()-online.peer[i].seen<12000000;}
static void online_apply(int i){if(online_peer_ready(i)){online.selected=i;wifi.remote=online.peer[i].pilot;wifi.display=online.peer[i].display;}}
static void online_forget_peer(int i){
 memset(&online.peer[i],0,sizeof(online.peer[i]));
 if(online.selected==i){online.selected=-1;if(selected_target==PLAYER_TARGET_ID){selected_target=0;autoaim=0;}if(surface_target_lock==19){surface_target_lock=-1;surface_turn_active=0;}}
}
static void online_send(OnlinePacket *p,const struct sockaddr_in *to){if(online.sock>=0)sceNetInetSendto(online.sock,p,sizeof(*p),MSG_DONTWAIT,(const struct sockaddr*)to,sizeof(*to));}
static OnlinePacket online_packet(int type){OnlinePacket p={0};p.magic=ONLINE_MAGIC;p.version=1;p.type=type;p.id=online.id;p.token=online.token;p.sequence=++online.sequence;p.pilot=wifi_local();return p;}
static void online_close(const char *note){
 if(online.id&&online.sock>=0){OnlinePacket p=online_packet(6);online_send(&p,&online.server);}
 if(online.sock>=0)sceNetInetClose(online.sock);online.sock=-1;
 if(online.apctl){sceNetApctlDisconnect();sceNetApctlTerm();}if(online.inet)sceNetInetTerm();if(online.net)sceNetTerm();
 if(online.module)sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);if(online.common)sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
 online.common=online.module=online.net=online.inet=online.apctl=online.stage=0;online.id=online.token=0;online.selected=-1;memset(online.peer,0,sizeof(online.peer));online.retry=UINT64_MAX;if(game.planet<1)game.local_pilot=0;
 if(selected_target==PLAYER_TARGET_ID){selected_target=0;autoaim=0;}if(surface_target_lock==19){surface_target_lock=-1;surface_turn_active=0;}
 if(note)snprintf(online.note,sizeof(online.note),"%s",note);
}
static void online_fail(int error,const char *note){online_close(note);online_error=error;}
static void online_config_load(void){
 snprintf(online.host,sizeof(online.host),"127.0.0.1");online.server.sin_family=AF_INET;online.server.sin_len=sizeof(online.server);online.server.sin_port=htons(25926);online_ipv4_parse(online.host,&online.server.sin_addr.s_addr);FILE *f=fopen("online.cfg","r");if(!f)return;int port=25926;
 if(fscanf(f,"%d %31s %d %d",&online.enabled,online.host,&port,&online.profile)!=4){online.enabled=0;snprintf(online.host,sizeof(online.host),"127.0.0.1");online.profile=1;port=25926;}fclose(f);
 if(online.enabled!=0&&online.enabled!=1)online.enabled=0;if(online.profile<1||online.profile>100)online.profile=1;
 online.server.sin_family=AF_INET;online.server.sin_len=sizeof(online.server);online.server.sin_port=htons(port>=1024&&port<=65535?port:25926);
 if(!online_ipv4_parse(online.host,&online.server.sin_addr.s_addr)){online.enabled=0;snprintf(online.host,sizeof(online.host),"127.0.0.1");online_ipv4_parse(online.host,&online.server.sin_addr.s_addr);}
}
static int online_config_save(int enabled){
 FILE *f=fopen("online.cfg.tmp","w");int ok=f&&fprintf(f,"%d %s %d %d\n",enabled,online.host,ntohs(online.server.sin_port)?ntohs(online.server.sin_port):25926,online.profile)>0;if(f&&fclose(f))ok=0;
 if(ok){remove("online.cfg.bak");rename("online.cfg","online.cfg.bak");if(rename("online.cfg.tmp","online.cfg")){rename("online.cfg.bak","online.cfg");ok=0;}}
 online_save_dirty=!ok;online_save_desired=enabled;return ok;
}
static void online_disable(const char *note){online.enabled=0;online_close(note);online_error=ONLINE_OK;online_config_save(0);}
static void online_request(void){
 if(wifi.state||online.stage)return;
 if(!online_config_save(1))return;
 online.enabled=1;online_error=ONLINE_OK;online.retry=0;snprintf(online.note,sizeof(online.note),"Connecting...");
}
static void online_retry_save(void){
 int desired=online_save_desired;if(!online_save_dirty||!online_config_save(desired))return;
 if(desired&&!wifi.state&&!online.stage){online.enabled=1;online.retry=0;online_error=ONLINE_OK;}
}
static void online_toggle(void){if(online.enabled||online.stage)online_disable("Offline");else online_request();}
static void online_find_profiles(void){
 online.profile_count=online.profile_index=0;
 /* Put the saved preference first, then every other saved Infrastructure
  * connection. Missing slots are skipped rather than waiting on them. */
 if(sceUtilityCheckNetParam(online.profile)==0)online.profiles[online.profile_count++]=online.profile;
 for(int id=1;id<=100;id++)if(id!=online.profile&&sceUtilityCheckNetParam(id)==0)online.profiles[online.profile_count++]=id;
}
static int online_connect_next_profile(void){
 while(online.profile_index<online.profile_count){
  online.active_profile=online.profiles[online.profile_index++];
  int error=sceNetApctlConnect(online.active_profile);
  if(error>=0){online.stage=1;online.started=online_now();snprintf(online.note,sizeof(online.note),"Trying Wi-Fi profile %d (%d/%d)...",online.active_profile,online.profile_index,online.profile_count);return 1;}
 }
 return 0;
}
static int online_start(void){
 if(wifi.state)return 0;
 if(!sceWlanGetSwitchState()){online_fail(ONLINE_WLAN,"Enable the WLAN switch.");return 0;}
 online_error=ONLINE_OK;
 int error=sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);if(error<0)goto fail;online.common=1;
 error=sceUtilityLoadNetModule(PSP_NET_MODULE_INET);if(error<0)goto fail;online.module=1;
 error=sceNetInit(128*1024,0x30,4096,0x30,4096);if(error<0)goto fail;online.net=1;
 error=sceNetInetInit();if(error<0)goto fail;online.inet=1;error=sceNetApctlInit(0x2000,0x30);if(error<0)goto fail;online.apctl=1;
 online_find_profiles();if(!online.profile_count)goto fail_none;
 if(!online_connect_next_profile())goto fail;
 return 1;
 fail_none:online_fail(ONLINE_PROFILE,"No saved Infrastructure Wi-Fi profiles.");return 0;
 fail:online_fail(ONLINE_START,"Wi-Fi could not start. Offline play continues.");return 0;
}
static int online_from_server(const struct sockaddr_in *a){return a->sin_addr.s_addr==online.server.sin_addr.s_addr&&a->sin_port==online.server.sin_port;}
static void online_receive(const OnlinePacket *p,const struct sockaddr_in *from){
 if(p->magic!=ONLINE_MAGIC||p->version!=1)return;int trusted=online_from_server(from);
 if(p->type==2&&trusted&&p->id&&p->id<=2&&p->token&&(online.stage==2||(online.stage==3&&p->id==online.id&&p->token==online.token))){online.id=p->id;online.token=p->token;online.stage=3;online_error=ONLINE_OK;if(game.planet<1)game.local_pilot=p->id;online.last_server=online_now();snprintf(online.note,sizeof(online.note),"Connected to PC test universe / Wi-Fi %d",online.active_profile);return;}
 if(p->type==7&&trusted&&online.stage>=2){online_fail(ONLINE_FULL,"Test universe full. Offline play continues.");return;}
 if(p->type==6&&trusted){for(int i=0;i<ONLINE_LIMIT;i++)if(online.peer[i].id==p->id)online_forget_peer(i);return;}
 if(online.stage!=3||(p->type!=4&&p->type!=5)||!p->id||p->id==online.id||!p->token||!wifi_pilot_valid(&p->pilot))return;
 int slot=-1;for(int i=0;i<ONLINE_LIMIT;i++)if(online.peer[i].id==p->id){slot=i;break;}
 if(p->type==5){if(!trusted||p->port<1||p->port>65535)return;online.last_server=online_now();if(slot<0)for(int i=0;i<ONLINE_LIMIT;i++)if(!online.peer[i].id){slot=i;break;}}
 if(slot<0)return;OnlinePeer *peer=&online.peer[slot];if(p->type==4&&!trusted&&(peer->token!=p->token||peer->address.sin_addr.s_addr!=from->sin_addr.s_addr||peer->address.sin_port!=from->sin_port))return;
 if(trusted)online.last_server=online_now();peer->seen=online_now();if(p->type==5){peer->address.sin_len=sizeof(peer->address);peer->address.sin_family=AF_INET;peer->address.sin_addr.s_addr=p->ip;peer->address.sin_port=htons(p->port);}
 if(p->type==5&&peer->id&&peer->token!=p->token){peer->id=peer->sequence=0;}
 if(peer->id&&peer->sequence&&!wifi_newer(p->sequence,peer->sequence))return;
 int snap=!peer->id||peer->pilot.system!=p->pilot.system||peer->pilot.planet!=p->pilot.planet||peer->pilot.sequence!=p->pilot.sequence;peer->id=p->id;peer->token=p->token;peer->sequence=p->sequence;peer->pilot=p->pilot;if(snap)peer->display=p->pilot;if(online.selected<0)online.selected=slot;
}
static void online_tick(float dt){
 if(!online.enabled||game.tutorial_step||wifi.state){if(online.stage)online_close("Offline / local Wi-Fi");return;}
 if(page==INTRO&&!online.stage)return;
 uint64_t now=online_now();if(!online.stage){if(now>=online.retry)online_start();return;}
 if(!sceWlanGetSwitchState()){online_fail(ONLINE_WLAN,"WLAN off. Offline play continues.");return;}
 if(online.stage==1){int state=0;if(sceNetApctlGetState(&state)<0)state=0;if(state==4){online.profile=online.active_profile;online.sock=sceNetInetSocket(AF_INET,SOCK_DGRAM,0);if(online.sock<0){online_fail(ONLINE_START,"Could not open test link");return;}struct sockaddr_in addr={0};addr.sin_len=sizeof(addr);addr.sin_family=AF_INET;if(sceNetInetBind(online.sock,(struct sockaddr*)&addr,sizeof(addr))<0){online_fail(ONLINE_START,"Could not bind test link");return;}online.stage=2;online.started=now;online.last_send=0;}else if(now-online.started>8000000){sceNetApctlDisconnect();if(!online_connect_next_profile()){online_fail(ONLINE_START,"No saved Wi-Fi profile connected.");return;}}return;}
 for(int budget=0;budget<16;budget++){unsigned char bytes[sizeof(OnlinePacket)+1];struct sockaddr_in from={0};socklen_t n=sizeof(from);int size=(int)sceNetInetRecvfrom(online.sock,bytes,sizeof(bytes),MSG_DONTWAIT,(struct sockaddr*)&from,&n);if(size<0)break;if(size==sizeof(OnlinePacket)){OnlinePacket p;memcpy(&p,bytes,sizeof(p));online_receive(&p,&from);}if(!online.stage)return;}
 now=online_now();
 if(now-online.last_send>=(online.stage==2?500000:100000)){OnlinePacket p=online_packet(online.stage==2?1:3);online_send(&p,&online.server);if(online.stage==3){p.type=4;for(int i=0;i<ONLINE_LIMIT;i++)if(online_peer_ready(i)&&online.peer[i].pilot.system==game.system&&online.peer[i].pilot.planet==game.planet)online_send(&p,&online.peer[i].address);}online.last_send=now;}
 if((online.stage==2&&now-online.started>15000000)||(online.stage==3&&now-online.last_server>15000000)){online_fail(online.stage==3?ONLINE_LOST:ONLINE_SERVER,"PC server unavailable. Offline play continues.");return;}
 float blend=1-expf(-dt*12);for(int i=0;i<ONLINE_LIMIT;i++){OnlinePeer *p=&online.peer[i];if(p->id&&now-p->seen>12000000){online_forget_peer(i);continue;}if(!p->id)continue;WifiPilot *d=&p->display;d->pos=add(d->pos,mul(sub(p->pilot.pos,d->pos),blend));d->yaw=wifi_angle(d->yaw,p->pilot.yaw,blend);d->pitch+=(p->pilot.pitch-d->pitch)*blend;d->roll=wifi_angle(d->roll,p->pilot.roll,blend);d->car=add(d->car,mul(sub(p->pilot.car,d->car),blend));d->car_yaw=wifi_angle(d->car_yaw,p->pilot.car_yaw,blend);d->parked=p->pilot.parked;d->system=p->pilot.system;d->planet=p->pilot.planet;d->surface=p->pilot.surface;d->rover=p->pilot.rover;d->sequence=p->pilot.sequence;d->jumping=p->pilot.jumping;d->docked=p->pilot.docked;d->ship=p->pilot.ship;d->paint=p->pilot.paint;d->speed=p->pilot.speed;memcpy(d->name,p->pilot.name,25);}
 online_apply(online.selected);
}
