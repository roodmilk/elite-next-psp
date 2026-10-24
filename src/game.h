#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#include "mesh.h"
#include "opening-state.h"
#include "rumour-state.h"
#include "nav-state.h"
#include "dockbay-state.h"
#define STATION_Z 3500.f
#define STATION_PORT_HALF_W 70.f
#define STATION_PORT_HALF_H 32.f
#define STATION_SHIP_HALF_W 18.f
#define STATION_SHIP_HALF_H 10.f
#define HUB_COUNT 3
#define NEWSPAPER_REWARD_SAVE_BYTES 257 /* Two puzzle-kind claim bits per system; Earth is 256. */
#define FIT_SAVE_BYTES 20
#define FIT_V29_BYTES 24
#define MODULE_STORE_CAPACITY 40
#define MODULE_STORE_BYTES 115 /* Historical V34+ stream width; new charms use existing DockRoll records. */
#define SOCIAL_KEEP 4096
#define SOCIAL_VARIANTS 8
#define SOCIAL_TAIL_VARIANTS 16
#define SOCIAL_POST_VARIANTS (SOCIAL_VARIANTS*SOCIAL_TAIL_VARIANTS)
#define SOCIAL_SAVE_BYTES (SOCIAL_KEEP*12+1040)
enum {SB_ARRIVE=1,SB_LEAVE,SB_DOCK,SB_LAUNCH,SB_ARREST,SB_FINE,SB_FLEE,
 SB_BOUNTY,SB_DESTROY,SB_PAINT,SB_SHIP,SB_LAND,SB_FLORA,SB_FAUNA,SB_RIFT,
 SB_SALVAGE,SB_JOB,SB_MISSED,SB_RETURN,SB_AMBIENT,SB_ERL_WIN,SB_ERL_LICENCE,SB_ERL_TROPHY,SB_ERL_MILESTONE,SB_COUNT};
typedef struct {uint32_t post[SOCIAL_KEEP],stamp[SOCIAL_KEEP],origin[SOCIAL_KEEP];
 uint32_t seen[256],clock,serial,count,evidence_reaction;} SocialState;
#define EQUIPMENT_COUNT 135
#define LAW_SCANNER_ITEM 56
#define NPC_COUNT 48
#define DEBRIS_COUNT 64
#define ANOMALY_COUNT 4
#define LIFE_COUNT 8
#define SURFACE_CELL 40
#define EVA_FIELD_RADIUS 10500.f
#define GOODS 17
#define NPC_ID_MIN (BODY_COUNT+1)
#define NPC_ID_MAX (BODY_COUNT+NPC_COUNT)
#define DEBRIS_ID_MIN (BODY_COUNT+NPC_COUNT+1)
#define DEBRIS_ID_MAX (BODY_COUNT+NPC_COUNT+DEBRIS_COUNT)
#define ANOMALY_ID_MIN (DEBRIS_ID_MAX+1)
#define ANOMALY_ID_MAX (DEBRIS_ID_MAX+ANOMALY_COUNT)
#define ROUTE_TARGET_ID (ANOMALY_ID_MAX+1)
#define PLAYER_TARGET_ID (ROUTE_TARGET_ID+HUB_COUNT)
#define DISCOVERY_COUNT 8
#define DISCOVERY_ID_MIN (PLAYER_TARGET_ID+1)
#define IS_DISCOVERY_ID(id) ((unsigned)((id)-DISCOVERY_ID_MIN)<DISCOVERY_COUNT)
#define RUMOUR_TARGET_ID (DISCOVERY_ID_MIN+DISCOVERY_COUNT)
#define WHALE_TARGET_ID (RUMOUR_TARGET_ID+1)
#define RACE_TARGET_ID (WHALE_TARGET_ID+1)
#define DOCKBAY_TARGET_ID (RACE_TARGET_ID+2)
#define BUOY_ID_MIN (DOCKBAY_TARGET_ID+1)
#define IS_BUOY_ID(id) ((unsigned)((id)-BUOY_ID_MIN)<NAV_BUOYS)
#define FLIGHT_TARGET_MAX (BUOY_ID_MIN+NAV_BUOYS-1)
#define TARGET_CAPACITY (FLIGHT_TARGET_MAX+1)
#define STATION_TARGET_ID(hub) ((hub)==0?0:ROUTE_TARGET_ID+(hub))
#define IS_STATION_ID(id) (station_target_hub(id)>=0)
#define BOUNTY_POSTER_COUNT 5
#define BOUNTY_NPC_FIRST 24
#define IS_NPC_ID(id) ((unsigned)((id)-NPC_ID_MIN)<(unsigned)NPC_COUNT)
#define IS_DEBRIS_ID(id) ((unsigned)((id)-DEBRIS_ID_MIN)<(unsigned)DEBRIS_COUNT)
#define IS_ANOMALY_ID(id) ((unsigned)((id)-ANOMALY_ID_MIN)<(unsigned)ANOMALY_COUNT)
enum { TRADERS, LAW, PIRATES, EXPLORERS, FACTION_COUNT };
enum { SUN, ROCKY, OCEAN, GAS };
enum {
 MISSION_DELIVERY, MISSION_BOUNTY, MISSION_EXPLORATION, MISSION_RESCUE, MISSION_SMUGGLING,
 MISSION_CONVOY, MISSION_REPAIR, MISSION_SALVAGE, MISSION_WILDLIFE, MISSION_ARCHAEOLOGY,
 MISSION_MINING, MISSION_DISTRESS, MISSION_PATROL, MISSION_CARGO_RECOVERY, MISSION_MISSING_PERSON,
 MISSION_PROCUREMENT, MISSION_MECHANIC, MISSION_BLACK_BOX, MISSION_PLANET_SAMPLE, MISSION_LISTENING_POST,
 MISSION_POLICE_ASSIST, MISSION_PASSENGER, MISSION_MEMORIAL, MISSION_MULTI_STOP, MISSION_FREIGHTER_DEFENCE,
 MISSION_DERELICT, MISSION_RACE, MISSION_COMPLEX, MISSION_TYPES=67
};
#define MISSION_OFFER_SLOTS 6
#include "mission-expansion-state.h"
/* New families reuse one of the five proven objective runtimes until their
 * specialised multi-stage version is promoted. The family identity remains
 * saved in Job.type, so presentation and later migration stay lossless. */
static inline int mission_base_type(int type){
 static const unsigned char base[MISSION_TYPES]={
  MISSION_DELIVERY,MISSION_BOUNTY,MISSION_EXPLORATION,MISSION_RESCUE,MISSION_SMUGGLING,
  MISSION_RESCUE,MISSION_EXPLORATION,MISSION_DELIVERY,MISSION_EXPLORATION,MISSION_EXPLORATION,
  MISSION_DELIVERY,MISSION_RESCUE,MISSION_EXPLORATION,MISSION_DELIVERY,MISSION_RESCUE,
  MISSION_DELIVERY,MISSION_RESCUE,MISSION_DELIVERY,MISSION_EXPLORATION,MISSION_EXPLORATION,
  MISSION_BOUNTY,MISSION_RESCUE,MISSION_DELIVERY,MISSION_DELIVERY,MISSION_BOUNTY,MISSION_EXPLORATION,MISSION_RACE
 };
 return type>=27&&type<MISSION_TYPES?MISSION_COMPLEX:type>=0&&type<27?base[type]:MISSION_DELIVERY;
}
enum { LIFE_FLORA, LIFE_FAUNA, LIFE_MINERAL };
enum { SFX_NONE, SFX_UI, SFX_LASER, SFX_HIT, SFX_WARP, SFX_SCAN, SFX_LAND, SFX_MINE, SFX_BOOST, SFX_COMM, SFX_DOCK, SFX_MISSILE, SFX_ALERT, SFX_DEATH, SFX_SELECT, SFX_TALK, SFX_OBSERVATORY, SFX_ALIEN_PLAYER, SFX_ALIEN_FIRE, SFX_ALIEN_MISSILE, SFX_ALIEN_KILL, SFX_SPEED_SURGE, SFX_PAPER, SFX_POWER_BUS, SFX_POWER_RELAY, SFX_POWER_READY, SFX_AD_OPEN, SFX_AD_CLOSE };
enum { VOICE_NONE, VOICE_KEI, VOICE_VENN, VOICE_DOCK, VOICE_LAW, VOICE_COMP, VOICE_CONTACT };
#define LEGACY_BODY_COUNT 5
#define BODY_COUNT 10
enum { SYS_CLASSIC, SYS_SOLITARY, SYS_MOON_SWARM, SYS_THREE_GIANTS,
 SYS_ASTEROID, SYS_BINARY, SYS_TRINARY, SYS_RED_DWARF, SYS_DEAD,
 SYS_HABITABLE, SYS_ROGUE, SYS_GAS_NURSERY, SYS_ARCHETYPE_COUNT };
static inline int system_archetype(int system){return system==7?SYS_CLASSIC:(system%SYS_ARCHETYPE_COUNT+SYS_ARCHETYPE_COUNT)%SYS_ARCHETYPE_COUNT;}
static inline int system_body_slots(int system){
 static const unsigned char count[SYS_ARCHETYPE_COUNT]={5,1,10,4,8,6,7,6,8,10,7,8};
 return count[system_archetype(system)];
}
static inline int system_body_active_slot(int system,int body){return body>=0&&body<system_body_slots(system);}
static inline int system_body_type_slot(int system,int body){
 int a=system_archetype(system);if(body<0||body>=system_body_slots(system))return ROCKY;
 /* Slot zero is deliberately an unlandable rogue giant. Surface/save code uses
  * bit zero for world slot one, so keeping this landmark unlandable preserves
  * old saves while the remaining rogue worlds stay fully explorable. */
 if(a==SYS_ROGUE)return body==0?GAS:(body%3==1?ROCKY:body%3==2?OCEAN:GAS);
 if(body==0)return SUN;
 if(a==SYS_BINARY&&body==1)return SUN;
 if(a==SYS_TRINARY&&body<=2)return SUN;
 if(a==SYS_MOON_SWARM)return body==1?GAS:(body%4==0?OCEAN:ROCKY);
 if(a==SYS_THREE_GIANTS||a==SYS_GAS_NURSERY)return GAS;
 if(a==SYS_ASTEROID||a==SYS_DEAD||a==SYS_RED_DWARF)return ROCKY;
 if(a==SYS_HABITABLE)return body%3==0?ROCKY:OCEAN;
 {static const int classic[4]={OCEAN,ROCKY,GAS,ROCKY};return classic[(body-1)&3];}
}
static inline int station_target_hub(int id){return id==0?0:id>ROUTE_TARGET_ID&&id<PLAYER_TARGET_ID?id-ROUTE_TARGET_ID:-1;}
#define MISSION_SLOTS 5
typedef struct { int dest,type,stage,target,item,origin,reward; float time; } Job;
/* Race attempts are transient. The accepted Job is saved using unchanged V32. */
#define RACE_GATES 32
#define RACE_RIVALS 14
#define RACE_LIMIT 300.f
#define RACE_BOOST_SECONDS 4.f
#define RACE_BOOST_RECHARGE 10.f
#define RACE_BOOST_RING_GAIN 24.f
#define RACE_SHIP_SCALE 1.35f
enum {RACE_NONE,RACE_GRID,RACE_COUNTDOWN,RACE_RUNNING,RACE_LOST,RACE_WON};
#include "erl-arcade-state.h"
#include "erl-race-drive.h"
typedef struct {Vec3 pos,normal,right,up,booster;float radius;int boost_used;} RaceGate;
typedef struct {Vec3 pos,dir;float speed,pace,finished,bump,burst,reserve,boost_delay,radius;int gate,mesh,disabled,boosting;
 float protection,health;int score,knockouts,boost_gate;float score_time;Vec3 impact;
} RaceRival;
typedef struct {int phase,system,hub,job,gate,rivals,place;unsigned seed;
 float countdown,elapsed,boost_time,flash,result_age,length,crash_time,crash_cooldown;
 float reserve,boost_delay,top_speed;int boost_lock;
 int crash_id,takedowns,crash_manual_boost;Vec3 start,dir,right,up;
 RaceGate gates[RACE_GATES];RaceRival pilot[RACE_RIVALS];
 int discipline,layout,laps,gate_count,target_score,knockouts;float off_course,repair,protection,handover,health,limit;
 int feed_code,feed_id;float feed_age,last_score;
 RaceArcade arcade;
 int drift_input; /* signed tap-drift direction; transient timer lives in ERL input */
 int manual_input; /* transient R state, independent of automatic propulsion */
 int rear_input; /* transient Down modifier only while a rocket is equipped */
 Vec3 crash_pos,crash_dir; /* optional presentation receipt, never save payload */
 uint32_t crash_awards; /* outstanding once-only takedown fuel receipts */
} SpaceRace;
#include "erl-state.h"
#include "erl-local-state.h"
#include "erl-event-catalogue.h"
/* Session-only presentation receipts: payment and progression commit first.
 * Six simultaneous dock payouts fit comfortably; nothing changes save V32. */
enum { RECEIPT_BOARD,RECEIPT_GUILD,RECEIPT_CAMPAIGN,RECEIPT_SAGA,RECEIPT_STORY,RECEIPT_PASSENGER,RECEIPT_STATION,RECEIPT_ACCEPTED,RECEIPT_FAILED };
#define MISSION_RECEIPT_MAX 16
typedef struct {int kind,type,system,amount,role,portrait_seed;unsigned serial;char title[64],sender[24];} MissionReceipt;
typedef struct { Vec3 pos; float radius; int type; unsigned color,accent,seed; char name[24]; } Body;
static inline int body_active(const Body *b){return b&&b->radius>1.f;}
typedef struct { char name[12]; int x,y,economy,government,tech; } System;
typedef struct { const char *name; int base,factor,quantity,mask; char unit; } Good;
typedef struct {
 const char *name;
 int capacity,price,speed,range;
 /* Bare-hull values are authoritative runtime data. Percent fields use 100
    as the Cobra-like baseline, keeping the table compact on PSP. */
 uint8_t slots[6];
 uint16_t acceleration,braking,boost,pitch,yaw,roll,drift;
 uint16_t hull,shields,heat_capacity,heat_dissipation,fuel_capacity,hyperdrive_charge,collision_radius;
} PlayerShip;
enum { PLAYER_SHIP_STANDARD_COUNT=10, PLAYER_SHIP_MEGA_FIRST=10, PLAYER_SHIP_MEGA_COUNT=12 };
enum { FREIGHT_ABSENT, FREIGHT_ARRIVING, FREIGHT_INBOUND, FREIGHT_SERVICE, FREIGHT_OUTBOUND, FREIGHT_CHARGING };
enum { DEBUG_MODIFIED=1u, DEBUG_UNLIMITED_FUEL=2u, DEBUG_UNLIMITED_RANGE=4u, DEBUG_FLAGS_MASK=7u };
enum { ENCOUNTER_NONE, ENCOUNTER_TRADER, ENCOUNTER_POLICE, ENCOUNTER_PIRATE, ENCOUNTER_DISTRESS, ENCOUNTER_CARGO, ENCOUNTER_WRECKAGE, ENCOUNTER_DERELICT, ENCOUNTER_ESCAPE_POD, ENCOUNTER_SMUGGLER, ENCOUNTER_MYSTERY, ENCOUNTER_MINER, ENCOUNTER_CONVOY, ENCOUNTER_BOUNTY, ENCOUNTER_UNKNOWN };
typedef struct {
 Vec3 pos,dir; float health,shield,cooldown,flash,scale,radius,cruise;
 int role,mesh,target,alive,waypoint,freighter,bounty_slot;
 int mega_hull; uint32_t vessel_seed;
 uint32_t dockbay_id; /* Exact listing identity; transient actor ownership. */ /* transient class and individual identity */
 int8_t traveller; /* >=0 indexes TravellerLive; -1 = anonymous traffic */
 uint8_t name_known; /* scanner has identified this contact in the current system */
 int route_id,route_leg,route_step,route_task;
 float route_wait,player_tag,escape_time;
 Vec3 freight_gate,freight_berth;
 float freight_timer;
 int freight_state,freight_style,freight_hub,freight_peer,freight_good,freight_qty,freight_trip;
} NPC;
typedef struct { Vec3 pos,vel; int alive,good,qty,wreck,rock,mega_hull; float life,health,radius,flash; uint32_t dockbay_id; } Debris;
typedef struct { Vec3 pos; int alive,kind,scanned; } Anomaly;
enum { FAUNA_IDLE,FAUNA_WANDER,FAUNA_FEED,FAUNA_ALERT,FAUNA_FLEE,FAUNA_RETURN };
/* Behaviour is transient: saves retain the same stable species discovery bits. */
typedef struct { Vec3 pos; int alive,kind,scanned;
 Vec3 home,goal;float heading,speed,phase,state_time,calm_time,lift;
 uint32_t behaviour_rng;int behaviour,behaviour_ready;
} Lifeform;
typedef struct {
 uint32_t rng; System systems[256]; NPC npc[NPC_COUNT]; Debris debris[DEBRIS_COUNT];
 int local_pilot,surface_replica; /* Session-only; never serialized. */
 int shuttle_active,mega_parent_ship; /* transient Mega planetary tender state */
 int peer_surface;Vec3 peer_ship,peer_car;float peer_ship_w,peer_ship_d,peer_car_yaw;
 Anomaly anomaly[ANOMALY_COUNT]; Lifeform life[LIFE_COUNT];
 Body bodies[BODY_COUNT];
 uint8_t newspaper_rewards[NEWSPAPER_REWARD_SAVE_BYTES]; /* V32: paid puzzle prizes. */
 NavigationState navigation;
 int spatial_region;float spatial_level;
 int buoy_scan_slot;float buoy_scan_time;
 RumourState rumours; /* V31: purchases and search directions, separate from missions. */
 /* Local traffic and scanner contacts are transient; commander saves keep fits. */
 Vec3 traffic_nodes[4][3],law_echo[NPC_COUNT];
 uint8_t law_echo_seen[NPC_COUNT];
 float law_scan_time;int law_scan_valid;
 int dock_stage,dock_phase,station_variant; float dock_timer,dock_duration,departure_glow; Vec3 dock_from,dock_to;
 uint32_t crime_record[256]; /* low byte: offences; upper bits: waivable cargo charge */
 int police_cargo_heat; /* mirror of current system cargo charge */
 int wanted[256],police_stop,police_phase,police_warned,upgrades; float police_warning,police_timer,roll,explosion;
 uint8_t fit[48],active_weapon; /* Eight interleaved banks of WPN DEF NAV HOLD FUEL UTIL. */
 uint8_t module_store[EQUIPMENT_COUNT]; /* V30: spare modules, separate from cargo. */
 float heat_sink_cd;
 float energy_router_cd; uint8_t emergency_bulkheads_used;
 /* Transient rechargeable tool state; reset with a fresh flight, not save layout. */
 int flare_charges;float flare_reload,flare_cd,flare_fx,ecm_cd;Vec3 flare_pos,flare_dir;
 float freight_next,freight_gap;
 float attacked,collision,encounter,incoming_missile,police_grace; int boost,approach,planet,surface,incoming_source;
 /* Transient drive response; never serialized, so existing saves stay compatible. */
 float boost_spool,mega_brake;
 int encounter_kind,encounter_npc,encounter_payload;
 Vec3 orbit_pos,ship_pos; float orbit_yaw,orbit_pitch,orbit_roll,orbit_speed;
 Vec3 missile_pos,missile_dir,missile_trail[12]; float missile_time,missile_speed; int missile_target,missile_trail_n;
 float fire_bearing_time[NPC_COUNT]; /* transient actual incoming-shot sources */
 int tractor_target; float tractor_time;
 Vec3 pos; float yaw,pitch,speed,energy,hull,heat,fuel,time,jump,shot,message_time,hazard,jetpack,damage_fx;
 int eva_jump_held; /* Transient R edge latch; not part of the save format. */
 int eva_running,eva_run_arm,planet_sequence;float eva_run_hold,planet_sequence_time;Vec3 planet_sequence_anchor;
 int damaged;
 int system,destination,route_goal,credits,kills,legal,ship,docked,dead,laser,missiles,cue;
 unsigned debug_flags; /* Runtime-only Debug Tools state; deliberately not saved. */
 int cargo[GOODS],stock[GOODS],price[GOODS],contract,contract_reward;
 int trader_offer_active,trader_offer_system,trader_offer_npc,trader_offer_need,trader_offer_reward,trader_offer_qty;
 float contract_time; int mission_type,mission_stage,mission_target,mission_item,mission_origin,mission_result,last_mission_type,last_mission_system;
 Job jobs[MISSION_SLOTS]; int job_n,job_sel;
 ExpeditionState expeditions[MISSION_SLOTS];ExpeditionRuntime expedition_runtime;uint8_t debris_mined[DEBRIS_COUNT];
 SpaceRace race;
 ErlCareer erl;
 ErlLocal erl_local;
 DockBay dockbay;int dockbay_npc,dockbay_live_system;float dockbay_depart;
 MissionReceipt mission_receipts[MISSION_RECEIPT_MAX];int mission_receipt_n;unsigned mission_receipt_serial;
 int story,story_flags,pip_sys,pip_eng,pip_wep,voice_who;
 /* Optional First Light tutorial. Zero is normal play; V15 persists progress. */
 int tutorial_step,tutorial_seen;
 OpeningState opening;
 char commander_name[25]; uint32_t commander_portrait; /* V16 field: legacy card or modular portrait DNA. */
 uint32_t station_progress[256][HUB_COUNT]; /* V19: stable per-system/hub activity flags. */
 uint32_t surface_progress[256][BODY_COUNT]; /* V17: activities and surveyed life, per world. */
 int rover_driving; Vec3 rover_pos; float world_clock;
 /* Transient Roamer dynamics; commander save layout is unchanged. */
 Vec3 rover_velocity;float rover_charge,rover_crack,rover_impact_cd,rover_reverse_wait;
 int rover_brake,rover_drift,rover_boost;
 unsigned rover_last_controls,rover_ticks;
 int guild_chapter,guild_flags,guild_choice;
 int voice_role,voice_seed;
 /* Transient ownership: ordinary speech cannot inherit an encounter's actions. */
 unsigned voice_serial;
 int voice_encounter_kind,voice_encounter_npc,voice_encounter_system;
 int campaign_stage,campaign_choice,campaign_flags;
 float campaign_distance,campaign_fuel;
 int saga_chapter,saga_step,saga_flags,saga_choice,saga_dest,saga_start;
 int saga_trust[4];
 int passenger_dest,passenger_kind,passenger_pay,gift_flags;
 int npc_kills,shots,discoveries,scanned_flora,scanned_fauna,scanned_minerals,scanned_anomalies,ai_phase;
 uint8_t rift_logged[256]; int rift_report;
 SocialState social; float social_fraction;
 uint8_t visited[32],bounty_claimed[256]; uint16_t landed_planets[256]; char message[96],voice[160],collide[40]; float voice_time;
 /* Living-galaxy named travellers (save V12). See travellers.h */
 struct { uint8_t sys,dest; int8_t slot; uint8_t flags; } travellers[12];
} Game;
/* Presentation-only discharge event. Deliberately outside Game/save data. */
typedef struct {
 const Game *owner;int serial,weapon,hit_id,shield_hit,rock_hit;
 float time,cycle,distance;Vec3 origin,direction,impact;
} PrimaryVisualEvent;
extern PrimaryVisualEvent primary_visual_event;

const char *dockbay_seller_name(int i);
void dockbay_roll_generate(DockRoll *r,unsigned seed,int base,int quality,int distant);
void dockbay_init(Game *g);
int dockbay_storage_used(const Game *g);
int dockbay_storage_capacity(const Game *g);
int dockbay_route(Game *g,int ad,int replace,char *reason,int size);
Vec3 dockbay_point(const Game *g,int ad);
int dockbay_ready(const Game *g);
int dockbay_actor(const Game *g,int npc);
void dockbay_restock(Game *g);
int dockbay_shop_next(Game *g,int index,int npc);
int dockbay_reveal(Game *g,int index);
int dockbay_report(Game *g,int index);
int dockbay_cargo_free(const Game *g,int item);
int dockbay_hail(Game *g,int npc,char *reason,int size);
const char *dockbay_contact_unavailable(const Game *g,int npc);
int dockbay_trade(Game *g,int ad,unsigned revision,int quantity,char *reason,int size);
int dockbay_check(const Game *g,int index,unsigned revision,int quantity,char *reason,int size);
int dockbay_wreck(Game *g,int npc);
int dockbay_salvage(Game *g,int debris);
void dockbay_departure(Game *g,int ad);
void dockbay_tick(Game *g,float dt);
int race_job(const Game *g);
int race_ready(const Game *g);
int race_is_active(const Game *g);
int race_course_build(const Game *g,int hub,SpaceRace *race);
int race_course_clear(const Game *g,const SpaceRace *race);
#include "mission-expansion-api.h"
void race_prepare(Game *g);
void race_restart(Game *g);
const char *race_start_blocker(const Game *g);
int race_preflight(Game *g,float dt);
void race_flight_control(Game *g,float *dt,float *turn,float *pitch,int *throttle);
void race_tick(Game *g,Vec3 previous,float dt);
void race_item_input(Game *g,int held);
Vec3 race_waypoint(const Game *g);
/* Shared curved holographic ribbon: rendering and floor contact use one frame. */
#define RACE_TRACK_FLOOR (-1600.f)
#define RACE_TRACK_WIDTH 8500.f
typedef struct {Vec3 centre,ahead,right,up;float t;int section;} RaceTrackFrame;
int race_track_surface(const SpaceRace *r,int section,float t);
RaceTrackFrame race_track_frame(const SpaceRace *r,int section,float t);
RaceTrackFrame race_track_nearest(const SpaceRace *r,int gate,Vec3 pos);
#define ERL_BOOST_RING_RADIUS 130.f
typedef struct {Vec3 pos,right,up;float until;unsigned serial,seed;int ring;} RaceBoostVisual;
const RaceBoostVisual *race_boost_visual(const Game *g);
RaceTrackFrame race_repair_frame(const SpaceRace *r,int section);
Vec3 race_pad_point(const SpaceRace *r,int section,float t,float lane);
int race_pad_crossing(const SpaceRace *r,int section,Vec3 from,Vec3 to,float support);
float race_floor_support(const Game *g,int id,RaceTrackFrame f);
float race_neon_scale(const SpaceRace *r);
#define RACE_NEON_EDGE 110.f
#define RACE_NEON_ROOF 135.f
float race_neon_body_radius(const Game *g);
float race_neon_clock(float arc);
float race_neon_pad_lane(const SpaceRace *r,int section);
int race_neon_pad(const SpaceRace *r,int section,float *centre,float *lane);
float race_neon_pad_t(const SpaceRace *r,int section,int station);
int race_hazard(const SpaceRace *r,int section,float time,Vec3 *centre,Vec3 *half);
float race_gold_delta(const Game *g);

float race_crossing(Vec3 from,Vec3 to,Vec3 center,Vec3 normal,float radius);
int erl_hq(const Game *g);
const char *race_contract_goal(const Job *job);
RaceRules race_contract_rules(const Game *g,const Job *job,unsigned seed);
int race_scenery_near(const SpaceRace *r,int gate,Vec3 from,Vec3 to);
int race_boost_visible(const SpaceRace *r,int section);
void race_withdraw(Game *g);
void race_contract_resume(Game *g);
int race_contract_won(const Game *g,const Job *job,int place,int failed);
int erl_sponsor_module(const RaceRules *rules);
int erl_sponsor_room(const Game *g);
int erl_sponsor_award(Game *g);
int erl_round_venue(const Game *g,int round);
const char *erl_course_at(const Game *g,int system,int layout);
DockRoll erl_sponsor_offer(const RaceRules *rules);
void erl_local_sync(Game *g);
void erl_season_offer(Game *g);
RaceRules erl_entry_rules(const Game *g,int kind,int layout,int discipline);
int erl_standing_better(const Game *g,int left,int right);
int erl_begin(Game *g,int kind,int layout,int discipline,int qualifier);
void erl_finish(Game *g,int place,int withdrawn);
void erl_withdraw(Game *g);
int erl_season_start(Game *g);
int erl_next_qualifier(const Game *g);
int erl_swap(Game *g,int index,unsigned paint,char *reason,int size);
void erl_ship_capture(Game *g,unsigned paint);
void erl_resume(Game *g);
int erl_claim_module(Game *g,int capacity);
const char *erl_course_name(const Game *g,int layout);
const char *erl_organiser(const Game *g,int system);
const char *erl_objective(const Game *g,char *out,int size);
void mission_receipt_record(Game *g,int kind,int type,int amount,int issuer,const char *title,const char *sender);
void mission_acceptance_record(Game *g,int type,int amount,int issuer,const char *title,const char *sender,int role,int portrait_seed);
int mission_receipt_pop(Game *g);
void opening_world_apply(Game *g);
int opening_keep_clear(Game *g,Vec3 previous);
int opening_save_allowed(const Game *g);
static inline int opening_service_visible(const Game *g,int id){
 if(!opening_exists(g))return 1;
 if(id==2)return g->opening.stage>=OP_MAP_INTRO;
 if(id==7)return (g->opening.flags&OF_FACTIONS)!=0;
 if(id==22)return (g->opening.flags&OF_LORE)!=0;
 if(id==23)return (g->opening.flags&OF_APP)!=0;
 if(id>=27||id==9||id==18||id==19)return opening_done(g);
 return 1;
}
#include "planet-profile.h"
#include "observatory-state.h"
#include "station-profile.h"
#include "mega-city-layout.h"
#include "station-architecture.h"
int survey_scan_target(Game *g,int slot);
extern const Good goods[GOODS];
extern const PlayerShip player_ships[];
extern const int player_ship_count;
int player_ship_is_mega(int ship);
int mega_yard_stock_count(const Game *g);
int mega_yard_stock_ship(const Game *g,int slot);
int shipyard_stocks_ship(const Game *g,int ship);
int shipyard_list_count(const Game *g);
int shipyard_list_ship(const Game *g,int row);
int ship_exchange_cost(const Game *g,int ship);
void debug_add_units(Game *g,int displayed_units);
Vec3 add(Vec3 a,Vec3 b); Vec3 sub(Vec3 a,Vec3 b); Vec3 mul(Vec3 a,float s);
float dot(Vec3 a,Vec3 b); float length(Vec3 a); Vec3 norm(Vec3 a);
Vec3 forward(const Game *g); Vec3 camera(const Game *g,Vec3 p);
void camera_override_set(Vec3 pos,float yaw,float pitch,float roll);
void camera_override_clear(void);
Vec3 camera_origin(const Game *g);
void camera_axes(const Game *g,Vec3 *right,Vec3 *up,Vec3 *ahead);
void flight_steer(Game *g,float turn,float pitch,float step);
int mesh_id(const char *name); int fuel_cargo_units(const Game *g); int cargo_used(const Game *g); int cargo_capacity(const Game *g); int galactic_price(int item);
void galaxy(System out[256]); float distance_ly(const Game *g,int a,int b);
void game_init(Game *g); void game_spawn(Game *g); void market(Game *g);
const char *npc_mega_identity(const NPC *n,char *out,size_t cap);
int trader_offer_hail(Game *g,int npc_id);
int trade_job_is(const Job *j);
void trade_contact_name_at(int system,int npc,char *out,int cap);
void trade_contact_name(const Game *g,int npc,char *out,int cap);
int accept_trader_offer(Game *g);
void system_bodies(Game *g);
float planet_ring_inner(const Body *b);
float planet_ring_outer(const Body *b);
Vec3 planet_ring_point(const Body *b,float angle,float radius);
int danger_rating(const Game *g,int system);
unsigned anomaly_slots_mask(const Game *g,int system);
int system_is_lawful(const Game *g,int system);
void turn_back(Game *g);
int mission_cargo_reserved(const Game *g,int item);
int mission_target_id(const Game *g,int slot);
int guild_claim(Game *g);
int campaign_accept(Game *g);
int campaign_claim(Game *g);
int campaign_retry(Game *g);
void campaign_event(Game *g,int event);
int emergency_rescue(Game *g);
int route_next_hop(const Game *g,int destination,int *jumps);
void route_clear(Game *g);
void route_set_goal(Game *g,int goal);
void route_refresh_destination(Game *g);
int wanted_level(const Game *g);
enum {CRIME_ASSAULT=1,CRIME_LAW_ASSAULT=2,CRIME_DESTRUCTION=4,CRIME_LAW_DESTRUCTION=8,CRIME_CARGO=16,CRIME_REFUSAL=32,CRIME_ESCAPE=64,CRIME_UNKNOWN=128};
void add_crime(Game *g,int points);
void record_crime(Game *g,int points,unsigned reason);
const char *police_accusation(const Game *g);
void police_charge_details(const Game *g,char *out,int cap);
int bounty_target_index(const Game *g,int poster);
int bounty_target_taken(const Game *g,int poster);
int bounty_target_reward(const Game *g,int poster);
void bounty_target_label(const Game *g,int poster,char *out,int cap);
int cargo_contraband(const Game *g);
int goods_restricted(int item);
int police_fine(const Game *g);
int police_surrender_cargo(Game *g);
int police_acknowledge(Game *g);
int police_resolve(Game *g,int jail);
int police_escape(Game *g);
int police_scan_submit(Game *g);
int police_scan_refuse(Game *g);
int police_pay_desk(Game *g);
void police_begin(Game *g,int phase);
int approach_planet(Game *g,int body);
int enter_planet(Game *g); void leave_planet(Game *g);
int land_planet(Game *g); int takeoff_planet(Game *g); int eva_toggle(Game *g);
int disembark_planet(Game *g);
int board_planet(Game *g);
int eva_can_board(const Game *g);
int eva_position_allowed(const Game *g,float x,float z);
void game_eva_tick(Game *g,float dt,float turn,float pitch,int walk,float strafe,int jet);
enum { ROAM_ACCEL=1,ROAM_BRAKE=2,ROAM_DRIFT=4,ROAM_BOOST=8 };
void game_rover_tick(Game *g,float dt,float steer,float look,unsigned controls);
int fauna_tests(const char *path);
int surface_cloud_deck(const Game *g,float x,float z);
int lave_world_tests(const char *path);
int starport_layout_tests(const char *path);
int planet_beauty_tests(const char *path);
float terrain_height(const Game *g,float x,float z);
Vec3 surface_poi(const Game *g,int id);
const char *surface_poi_name(int id);
const char *surface_site_name(const Game *g,int sys,int body,int id);
const char *surface_site_brief(const Game *g,int sys,int body,int id);
int surface_nearest_site(const Game *g,float range);
void surface_ship_bounds(const Game *g,float *x,float *z);
int surface_interact(Game *g);
int observatory_resolve(Game *g,int id,int choice);
int surface_rover(Game *g);
int terrain_is_water(const Game *g,float x,float z);
int terrain_bridge_position(const Game *g,int index,Vec3 *position,Vec3 *across);
float terrain_mesh_height(const Game *g,float x,float z);
int terrain_water_tile(const Game *g,float x,float z);
float terrain_relief_scale(const Game *g);
Vec3 surface_site(const Game *g,int i);
Vec3 surface_landing_pad(const Game *g);
int analysis_scan(Game *g,int target_id);
int survey_scan(Game *g);
int mine_rock(Game *g,int target_id);
Vec3 freight_extent(const NPC *n);
Vec3 freight_world(const NPC *n,Vec3 p);
float freight_intersection(const NPC *n,Vec3 start,Vec3 end,float padding);
const char *freight_status(const NPC *n);
int systems_visited(const Game *g);
float station_angle(const Game *g);
const char *station_name(const Game *g);
const char *station_name_for(const Game *g,int hub);
int dock_hub(Game *g,int hub);
int station_nearest_port(const Game *g,Vec3 world);
Vec3 hub_position(const Game *g,int hub); int nearest_hub(const Game *g);
int prosperity(const Game *g,int system);
int system_rock_belt(int sys); int system_ice_belt(int sys); int system_whales(int sys); int system_comet(int sys); int traffic_budget(const Game *g);
Vec3 whale_home(const Game *g); Vec3 whale_position(const Game *g,int index);
int mission_count(const Game *g);
int jobs_active(const Game *g);
int mission_destination(const Game *g,int offer);
void mission_queries_begin(const Game *g);
void mission_queries_end(void);
int mission_type_for_offer(const Game *g,int offer);
int mission_type_at(const Game *g,int system,int hub,int offer);
const char *mission_name(int type);
const char *mission_brief(const Game *g,int offer);
const char *mission_requirement(int type);
const char *mission_difficulty_name(int risk);
int mission_reward(const Game *g,int offer);
int mission_risk(const Game *g,int offer);
const char *mission_objective(const Game *g);
const char *mission_objective_at(const Game *g,int slot);
int is_mission_target(const Game *g,int target_id);
int mission_interact(Game *g,int target_id);
int mission_offer_active(const Game *g,int offer);
int abandon_mission(Game *g,int slot);
int accept_mission(Game *g,int offer);
int accept_distress_call(Game *g,int caller);
void mission_timers(Game *g,float dt);
void message(Game *g,const char *s); void speak(Game *g,int who,const char *s); void encounter_speak(Game *g,int who,const char *s); int encounter_requires_reply(const Game *g); void encounter_respond(Game *g); void encounter_ignore(Game *g); void game_tick(Game *g,float dt,float turn,float pitch,int throttle,int fire);
void social_emit(Game *g,int event);
void social_arrive(Game *g);
void launch_departure(Game *g);
void launch(Game *g); int dock(Game *g); int refuel_full(Game *g); int trade(Game *g,int item,int buy);
int jettison_cargo(Game *g,int item);
int buy_ship(Game *g,int i); int jump_start(Game *g); int contract_accept(Game *g); int deploy_flare(Game *g);
int activate_ecm(Game *g);
int dump_heat_sink(Game *g);
 int law_scan(Game *g);
 const char *traffic_activity(const NPC *n);
int flare_capacity(const Game *g);
int ecm_fitted(const Game *g);
int fire_missile(Game *g,int target_id);
int salvage(Game *g,int target_id);
int save_game(Game *g,const char *path); int load_game(Game *g,const char *path);
int ship_repair_cost(const Game *g); int repair_ship(Game *g);
int game_tests(const char *path);
#ifdef ELITE_ENABLE_REVIEW
int wanted_bounty_tests(Game *g,const char *path);
#endif
int npc_combat_tests(const char *path);
#include "world-state.h"
int mission_offer_valid(const Game *g,int offer,int *destination_out);
int mission_landable_body(const Game *g,int system,int body);

/* Derived geography migration: retain safe checkpoints, recover unsafe ones locally. */
void surface_region_resume(Game *g);
#include "erl-racers.h"
#ifdef ELITE_ENABLE_REVIEW
unsigned race_polish_traffic_review(Game *g);
#endif
void race_crash_present_tick(Game *g,float dt);
/* Shared deterministic exploration geometry and durable navigation knowledge. */
Vec3 exploration_discovery_position(const Game *g,int slot);
Vec3 navigation_position(const Game *g,int key);
int navigation_enabled(const Game *g);
int navigation_source(const Game *g,int key);
int navigation_select(Game *g,int key);
void navigation_visit(Game *g,int key);
int navigation_buoy_scan(Game *g,int buoy);
int navigation_links(const Game *g,int buoy,int *keys,int capacity);
int navigation_rumour_buy(Game *g,int offer);
Vec3 navigation_search_position(const Game *g);
int navigation_planet_job(Game *g);
void navigation_tick(Game *g,float dt);
void navigation_arrival_clearance(Game *g);
float spatial_density(const Game *g,int region,Vec3 position);
int spatial_region_kind(const Game *g,int region);
float spatial_region_radius(int region);
float spatial_engine_factor(const Game *g);
#endif

int expedition_tests(const char *path);
