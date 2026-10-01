/* Local, bounded fiction. Uses its own hash: never advances combat RNG.
 * Codes store event, the ship at the event, author/variant seed, reaction.
 * Save layout is explicit u32s, independent of struct padding. */
#include <psprtc.h>
#include <time.h>
static uint32_t social_test_time;
static uint32_t social_now(const Game *g){
 if(social_test_time)return social_test_time;
 ScePspDateTime d,epoch={1970,1,1,0,0,0,0};u64 tick=0,base=0;
 if(sceRtcGetCurrentClock(&d,0)>=0&&d.year>=2000&&d.year<2038&&
    sceRtcGetTick(&d,&tick)>=0&&sceRtcGetTick(&epoch,&base)>=0&&tick>=base)
  return (uint32_t)((tick-base)/sceRtcGetTickResolution());
 return 0x80000000u|g->social.clock;
}
static int social_elapsed(uint32_t now,uint32_t then,uint32_t seconds){
 return then&&((now^then)&0x80000000u)==0&&now>=then&&now-then>=seconds;
}
static unsigned social_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
/* Newest first, bounded 48 KiB archive. Event order survives clock adjustments. */
static void social_push(SocialState *s,unsigned code,unsigned stamp,unsigned origin,int at){
 if(at<0||at>=SOCIAL_KEEP)return;
 int end=s->count<SOCIAL_KEEP?(int)s->count:SOCIAL_KEEP-1;
 for(int i=end;i>at;i--){s->post[i]=s->post[i-1];s->stamp[i]=s->stamp[i-1];s->origin[i]=s->origin[i-1];}
 s->post[at]=code;s->stamp[at]=stamp;s->origin[at]=origin;
 if(s->count<SOCIAL_KEEP)s->count++;
}
static void social_insert(Game *g,int sys,int event,uint32_t when){
 if(sys<0||sys>255||event<1||event>=SB_COUNT)return;
 SocialState *s=&g->social;
 unsigned seed=social_hash(++s->serial+(unsigned)sys*997u)&0x1fffffu;
 /* Avoid the immediately previous full body combination for this kind;
  * usernames and historical text stay deterministic from the saved seed. */
 for(unsigned i=0;i<s->count;i++)if((s->post[i]&31)==(unsigned)event){
  unsigned previous=(s->post[i]>>9)&0x1fffff;
  if(!(s->origin[i]&256)&&seed%SOCIAL_POST_VARIANTS==previous%SOCIAL_POST_VARIANTS)seed=(seed+1)&0x1fffffu;
  break;
 }
 social_push(s,(unsigned)event|((unsigned)g->ship<<5)|((seed&0x1fffffu)<<9),when,(unsigned)sys,0);
}
void social_emit(Game *g,int event){
 if(!g||g->system<0||g->system>255||event<1||event>=SB_COUNT)return;
 SocialState *s=&g->social;if(!s->clock)s->clock=1;uint32_t now=social_now(g);
 /* Ambient jokes should not bury actual activity. At most three in the latest sixteen. */
 if(event==SB_AMBIENT){int idle=0;for(unsigned i=0;i<s->count&&i<16;i++)idle+=(s->post[i]&31)==SB_AMBIENT;if(idle>=3)return;}
 for(unsigned i=0;i<s->count;i++)if((s->origin[i]&255)==(unsigned)g->system&&(s->post[i]&31)==(unsigned)event){
  if(!social_elapsed(now,s->stamp[i],90))return;
  break;
 }
 social_insert(g,g->system,event,now);
}
void social_arrive(Game *g){
 SocialState *s=&g->social;uint32_t last=s->seen[g->system];
 if(!s->clock)s->clock=1;
 uint32_t now=social_now(g);
 if(social_elapsed(now,last,86400)){
  social_insert(g,g->system,SB_MISSED,now);
  social_emit(g,SB_RETURN);
 }else social_emit(g,SB_ARRIVE);
 s->seen[g->system]=now;
}
static void social_tick(Game *g,float dt){
 if(dt<=0||!isfinite(dt))return;
 if(!g->social.clock)g->social.clock=1;
 g->social_fraction+=dt;
 while(g->social_fraction>=1){
  g->social_fraction-=1;
  if(g->social.clock<0xfffffff0u)g->social.clock++;
  if(g->social.clock%240==0)social_emit(g,SB_AMBIENT);
 }
}
