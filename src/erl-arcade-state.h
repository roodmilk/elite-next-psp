#ifndef ERL_ARCADE_STATE_H
#define ERL_ARCADE_STATE_H
#include "erl-arcade-rules.h"
#define RA_SAVE_BYTES ((1437+22+2)*4)
#define RA_ACTORS 15
#define RA_PROJECTILES 80
#define RA_PICKUPS 24
typedef struct {Vec3 pos,dir;int kind,owner,target,live;uint32_t generation;float age,life;} RaceProjectile;
typedef struct {Vec3 pos;float cooldown;int section,lane;} RacePickup;
typedef struct {
 RaceRules rules;
 RaceItemActor actor[RA_ACTORS];
 uint32_t generation[RA_ACTORS],rng;
 uint32_t branch_boost[RA_ACTORS],branch_repair[RA_ACTORS];
 RaceProjectile shot[RA_PROJECTILES];
 RacePickup pickup[RA_PICKUPS];
 int ready,input_x,lock,warning,warning_side,last_hit,launch_seen,sponsor_done;
 float cue_age,lock_age,warning_gap,entry,wall_ripple,launch_age,sponsor_age;
 Vec3 wall_impact;
} RaceArcade;
#endif
