#ifndef PLANET_PROFILE_H
#define PLANET_PROFILE_H

/* Save-neutral procedural descriptor. It is derived from Body.seed at runtime;
 * never serialize this structure into commander files. */
enum { PLANET_FAMILY_OCEAN, PLANET_FAMILY_DESERT, PLANET_FAMILY_ICE, PLANET_FAMILY_VOLCANIC, PLANET_FAMILY_FOREST };
typedef struct {
 uint32_t seed;
 uint8_t family,terrain_style,palette,sea_level;
 uint8_t prop_density,activity_density;
} PlanetProfile;

static unsigned planet_profile_hash(unsigned x){
 x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);
}
static PlanetProfile planet_profile_for_body(const Body *body){
 PlanetProfile p={0};if(!body)return p;p.seed=body->seed;
 unsigned h=planet_profile_hash(body->seed^((unsigned)body->type*0x9e3779b9u));
 if(body->type==OCEAN)p.family=PLANET_FAMILY_OCEAN;
 else p.family=(uint8_t)(PLANET_FAMILY_DESERT+(body->seed%4));
 p.terrain_style=(uint8_t)((h>>8)%4);
 p.palette=(uint8_t)((h>>12)%8);
 p.sea_level=(uint8_t)(body->type==OCEAN?42:0);
 p.prop_density=(uint8_t)(35+(h%46));
 p.activity_density=(uint8_t)(20+((h>>16)%61));
 return p;
}
static int planet_profile_valid(const PlanetProfile *p){
 return p&&p->family<=PLANET_FAMILY_FOREST&&p->terrain_style<4&&p->palette<8&&p->prop_density<=100&&p->activity_density<=100;
}
#endif
