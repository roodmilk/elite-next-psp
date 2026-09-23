#ifndef STATION_PROFILE_H
#define STATION_PROFILE_H

/* Save-neutral station identity/activity descriptor. It is derived from the
 * compiled System fields and hub index; it is not a commander-save record. */
typedef struct {
 uint8_t system,hub,architecture,arrangement,landmark,windows,traffic,security,service;
 uint32_t seed;
} StationProfile;
static unsigned station_profile_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static StationProfile station_profile_for(const System *system,int system_id,int hub){
 StationProfile p={0};if(!system||system_id<0||system_id>=256||hub<0||hub>=HUB_COUNT)return p;
 p.system=(uint8_t)system_id;p.hub=(uint8_t)hub;
 unsigned packed=(unsigned)system->x|((unsigned)system->y<<8)|((unsigned)system->government<<16)|((unsigned)system->economy<<19)|((unsigned)system->tech<<22);
 p.seed=station_profile_hash(packed^((unsigned)(hub+1)*0x9e3779b9u));
 p.architecture=(uint8_t)(p.seed%6);p.arrangement=(uint8_t)((p.seed>>3)%8);p.landmark=(uint8_t)((p.seed>>7)%8);
 p.windows=(uint8_t)(4+(p.seed%7)+(system->economy>=4?3:0));
 p.traffic=(uint8_t)(2+((p.seed>>8)%7)+(system->economy>=4?2:0));
 p.security=(uint8_t)(1+((p.seed>>16)%5)+(system->government>=5?2:0));
 p.service=(uint8_t)(1+((p.seed>>24)%4)+(hub>0?1:0));
 return p;
}
static int station_profile_valid(const StationProfile *p){return p&&p->hub<HUB_COUNT&&p->architecture<6&&p->arrangement<8&&p->landmark<8&&p->windows>=4&&p->windows<=13&&p->traffic>=2&&p->traffic<=10&&p->security>=1&&p->security<=7&&p->service>=1&&p->service<=5;}
#endif
