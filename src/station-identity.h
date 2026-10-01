/* Procedural station identity shared by authored and generated rooms.
 * The record is derived from system + hub variant, so all 256 systems can
 * use the same room kit without per-station assets. */
#ifndef ELITE_STATION_IDENTITY_H
#define ELITE_STATION_IDENTITY_H

typedef struct {
 int system, variant, economy, government, tech;
 unsigned seed;
 const char *kind;
 const char *profile;
} StationIdentity;

static unsigned station_identity_hash(int system,int variant){
 unsigned h=(unsigned)(system+1)*0x9e3779b9u^(unsigned)(variant+17)*0x85ebca6bu;
 h^=h>>16;h*=0x7feb352du;h^=h>>15;return h;
}

static StationIdentity station_identity_for(const Game *g){
 StationIdentity s; memset(&s,0,sizeof(s));
 s.system=g->system;s.variant=g->station_variant;s.seed=station_identity_hash(s.system,s.variant);
 s.economy=g->systems[s.system].economy;s.government=g->systems[s.system].government;s.tech=g->systems[s.system].tech;
 if(s.variant==1)s.kind="OUTER RELAY";
 else if(s.variant==2)s.kind="FRONTIER OUTPOST";
 else s.kind="PRIMARY HUB";
 if(s.economy<=1)s.profile="INDUSTRIAL / CARGO";
 else if(s.economy>=5)s.profile="AGRICULTURAL / TRADE";
 else if(s.government>=5)s.profile="CORPORATE / SECURE";
 else s.profile="MIXED / TRANSIT";
 return s;
}

static int station_identity_manifest_reward(const StationIdentity *s){
 return 400+(int)(s->seed%3u)*100;
}

#endif
