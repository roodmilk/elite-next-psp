#include <pspnet.h>
#include <pspnet_adhoc.h>
#include <pspnet_adhocctl.h>

#define CONVOY_MAGIC 0x434f4e56u
#define CONVOY_VERSION 1
#define CONVOY_PORT 0x3e9
#define CONVOY_MAX_PEERS 4
typedef struct {uint32_t magic,version;uint16_t system;int16_t speed;float x,y,z,yaw,pitch;uint16_t energy,shield;} ConvoySnapshot;
typedef struct {unsigned char mac[6];ConvoySnapshot snapshot;unsigned age;} ConvoyPeer;
static int convoy_running=0,convoy_pdp=-1,convoy_peer_count=0;
static ConvoyPeer convoy_peers[CONVOY_MAX_PEERS];
static unsigned char convoy_broadcast_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};

static int convoy_snapshot_valid(const ConvoySnapshot *s){
 return s&&s->magic==CONVOY_MAGIC&&s->version==CONVOY_VERSION&&s->system<256&&s->speed>=0&&s->speed<20000&&s->energy<=100&&s->shield<=100&&isfinite(s->x)&&isfinite(s->y)&&isfinite(s->z)&&isfinite(s->yaw)&&isfinite(s->pitch);
}
static void convoy_tests(void){
 FILE *f=fopen("convoy-check.txt","w");if(!f)return;int failures=0;
 ConvoySnapshot s={CONVOY_MAGIC,CONVOY_VERSION,7,120,1,2,3,0.5f,-0.2f,90,80};
 #define CONVOY_CHECK(ok,label) do{if(!(ok)){failures++;fprintf(f,"FAIL %s\n",label);}}while(0)
 CONVOY_CHECK(convoy_snapshot_valid(&s),"valid snapshot");s.magic=0;CONVOY_CHECK(!convoy_snapshot_valid(&s),"bad magic");s.magic=CONVOY_MAGIC;s.version=0;CONVOY_CHECK(!convoy_snapshot_valid(&s),"bad version");s.version=CONVOY_VERSION;s.speed=-1;CONVOY_CHECK(!convoy_snapshot_valid(&s),"negative speed");s.speed=120;s.energy=101;CONVOY_CHECK(!convoy_snapshot_valid(&s),"energy overflow");s.energy=90;s.x=NAN;CONVOY_CHECK(!convoy_snapshot_valid(&s),"non-finite position");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);
 #undef CONVOY_CHECK
}
static int convoy_start(void){
 if(convoy_running)return 1;
 if(sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON)<0||sceUtilityLoadNetModule(PSP_NET_MODULE_ADHOC)<0)return 0;
 if(sceNetInit(128*1024,42,4*1024,42,4*1024)<0)goto fail_modules;
 if(sceNetAdhocInit()<0)goto fail_net;
 struct productStruct product;memset(&product,0,sizeof(product));product.unknown=2;memcpy(product.product,"ELNEXT01",8);
 if(sceNetAdhocctlInit(32*1024,0x20,&product)<0)goto fail_adhoc;
 if(sceNetAdhocctlCreate("ELITE")<0)goto fail_ctl;
 if(sceNetAdhocctlConnect("ELITE")<0)goto fail_ctl;
 unsigned char mac[6];if(sceNetGetLocalEtherAddr(mac)<0)goto fail_ctl;
 convoy_pdp=sceNetAdhocPdpCreate(mac,CONVOY_PORT,0x1000,0);if(convoy_pdp<0)goto fail_ctl;
 convoy_running=1;convoy_peer_count=0;return 1;
fail_ctl:sceNetAdhocctlTerm();
fail_adhoc:sceNetAdhocTerm();
fail_net:sceNetTerm();
fail_modules:sceUtilityUnloadNetModule(PSP_NET_MODULE_ADHOC);sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);return 0;
}
static void convoy_stop(void){
 if(!convoy_running)return;
 if(convoy_pdp>=0)sceNetAdhocPdpDelete(convoy_pdp,0);convoy_pdp=-1;
 sceNetAdhocctlDisconnect();sceNetAdhocctlTerm();sceNetAdhocTerm();sceNetTerm();
 sceUtilityUnloadNetModule(PSP_NET_MODULE_ADHOC);sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
 convoy_running=0;convoy_peer_count=0;
}
static void convoy_poll(const ConvoySnapshot *local){
 if(!convoy_running||!convoy_snapshot_valid(local))return;
 sceNetAdhocPdpSend(convoy_pdp,convoy_broadcast_mac,CONVOY_PORT,(void*)local,sizeof(*local),0,1);
 unsigned char mac[6];unsigned short port;unsigned char data[sizeof(ConvoySnapshot)];int len=sizeof(data);
 while(sceNetAdhocPdpRecv(convoy_pdp,mac,&port,data,&len,0,1)>=0){
  if(len!=(int)sizeof(ConvoySnapshot)||!convoy_snapshot_valid((ConvoySnapshot*)data))continue;
  int slot=-1;for(int i=0;i<convoy_peer_count;i++)if(!memcmp(convoy_peers[i].mac,mac,6)){slot=i;break;}
  if(slot<0&&convoy_peer_count<CONVOY_MAX_PEERS)slot=convoy_peer_count++;
  if(slot>=0){memcpy(convoy_peers[slot].mac,mac,6);memcpy(&convoy_peers[slot].snapshot,data,sizeof(ConvoySnapshot));convoy_peers[slot].age=0;}
  len=sizeof(data);
 }
 for(int i=0;i<convoy_peer_count;i++)if(convoy_peers[i].age++>180){convoy_peers[i]=convoy_peers[--convoy_peer_count];i--;}
}
