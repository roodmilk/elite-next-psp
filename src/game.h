#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#include "mesh.h"
#define STATION_Z 3500.f
#define STATION_HALF 160.f
#define STATION_PORT_HALF_W 70.f
#define STATION_PORT_HALF_H 32.f
#define STATION_SHIP_HALF_W 18.f
#define STATION_SHIP_HALF_H 10.f
#define STATION_ENTRY_Z (STATION_Z-STATION_HALF)
#define HUB_COUNT 3
static inline Vec3 station_port_corner(int i){
 return (Vec3){(i==0||i==3)?-STATION_PORT_HALF_W:STATION_PORT_HALF_W,
               i<2?-STATION_PORT_HALF_H:STATION_PORT_HALF_H,-STATION_HALF};
}
#define NPC_COUNT 48
#define DEBRIS_COUNT 64
#define ANOMALY_COUNT 4
#define LIFE_COUNT 8
#define SURFACE_CELL 40
#define EVA_FIELD_RADIUS 480.f
#define GOODS 17
#define NPC_ID_MIN (BODY_COUNT+1)
#define NPC_ID_MAX (BODY_COUNT+NPC_COUNT)
#define DEBRIS_ID_MIN (BODY_COUNT+NPC_COUNT+1)
#define DEBRIS_ID_MAX (BODY_COUNT+NPC_COUNT+DEBRIS_COUNT)
#define ANOMALY_ID_MIN (DEBRIS_ID_MAX+1)
#define ANOMALY_ID_MAX (DEBRIS_ID_MAX+ANOMALY_COUNT)
#define IS_NPC_ID(id) ((unsigned)((id)-NPC_ID_MIN)<(unsigned)NPC_COUNT)
#define IS_DEBRIS_ID(id) ((unsigned)((id)-DEBRIS_ID_MIN)<(unsigned)DEBRIS_COUNT)
#define IS_ANOMALY_ID(id) ((unsigned)((id)-ANOMALY_ID_MIN)<(unsigned)ANOMALY_COUNT)
enum { TRADERS, LAW, PIRATES, EXPLORERS, FACTION_COUNT };
enum { SUN, ROCKY, OCEAN, GAS };
enum { MISSION_DELIVERY, MISSION_BOUNTY, MISSION_EXPLORATION, MISSION_RESCUE, MISSION_SMUGGLING, MISSION_TYPES };
enum { LIFE_FLORA, LIFE_FAUNA, LIFE_MINERAL };
enum { SFX_NONE, SFX_UI, SFX_LASER, SFX_HIT, SFX_WARP, SFX_SCAN, SFX_LAND, SFX_MINE, SFX_BOOST, SFX_COMM, SFX_DOCK, SFX_MISSILE, SFX_ALERT, SFX_DEATH, SFX_SELECT, SFX_TALK };
enum { VOICE_NONE, VOICE_KEI, VOICE_VENN, VOICE_DOCK, VOICE_LAW, VOICE_COMP, VOICE_CONTACT };
#define BODY_COUNT 5
#define MISSION_SLOTS 5
typedef struct { int dest,type,stage,target,item,origin,reward; float time; } Job;
typedef struct { Vec3 pos; float radius; int type; unsigned color,accent,seed; char name[24]; } Body;
typedef struct { char name[12]; int x,y,economy,government,tech; } System;
typedef struct { const char *name; int base,factor,quantity,mask; char unit; } Good;
typedef struct { const char *name; int capacity,price,speed,range; } PlayerShip;
enum { FREIGHT_ABSENT, FREIGHT_ARRIVING, FREIGHT_INBOUND, FREIGHT_SERVICE, FREIGHT_OUTBOUND, FREIGHT_CHARGING };
enum { ENCOUNTER_NONE, ENCOUNTER_TRADER, ENCOUNTER_POLICE, ENCOUNTER_PIRATE, ENCOUNTER_DISTRESS, ENCOUNTER_CARGO, ENCOUNTER_WRECKAGE, ENCOUNTER_DERELICT, ENCOUNTER_ESCAPE_POD, ENCOUNTER_SMUGGLER, ENCOUNTER_MYSTERY, ENCOUNTER_MINER, ENCOUNTER_CONVOY, ENCOUNTER_BOUNTY, ENCOUNTER_UNKNOWN };
typedef struct {
 Vec3 pos,dir; float health,shield,cooldown,flash,scale,radius,cruise;
 int role,mesh,target,alive,waypoint,freighter;
 int8_t traveller; /* >=0 indexes TravellerLive; -1 = anonymous traffic */
 uint8_t name_known; /* scanner has identified this contact in the current system */
 Vec3 freight_gate,freight_berth;
 float freight_timer;
 int freight_state,freight_style,freight_hub,freight_peer,freight_good,freight_qty,freight_trip;
} NPC;
typedef struct { Vec3 pos,vel; int alive,good,qty,wreck,rock; float life,health,radius,flash; } Debris;
typedef struct { Vec3 pos; int alive,kind,scanned; } Anomaly;
typedef struct { Vec3 pos; int alive,kind,scanned; } Lifeform;
typedef struct {
 uint32_t rng; System systems[256]; NPC npc[NPC_COUNT]; Debris debris[DEBRIS_COUNT];
 Anomaly anomaly[ANOMALY_COUNT]; Lifeform life[LIFE_COUNT];
 Body bodies[BODY_COUNT];
 int dock_stage,dock_phase,station_variant; float dock_timer,dock_duration; Vec3 dock_from,dock_to;
 int wanted[256],police_stop,police_phase,police_warned,upgrades; float police_warning,police_timer,roll,explosion;
 uint8_t fit[6]; /* WPN DEF NAV HOLD FUEL UTIL — catalog index or 0xFF empty (save V13) */
 float heat_sink_cd;
 float freight_next,freight_gap;
 float attacked,collision,encounter,incoming_missile,police_grace; int boost,approach,planet,surface,incoming_source;
 int encounter_kind,encounter_npc,encounter_payload;
 Vec3 orbit_pos,ship_pos; float orbit_yaw,orbit_pitch,orbit_roll,orbit_speed;
 Vec3 missile_pos; float missile_time; int missile_target;
 int tractor_target; float tractor_time;
 Vec3 pos; float yaw,pitch,speed,energy,heat,fuel,time,jump,shot,message_time,hazard,jetpack;
 int system,destination,route_goal,credits,kills,legal,ship,docked,dead,laser,missiles,cue;
 int cargo[GOODS],stock[GOODS],price[GOODS],contract,contract_reward;
 int trader_offer_active,trader_offer_system,trader_offer_npc,trader_offer_need,trader_offer_reward,trader_offer_qty;
 float contract_time; int mission_type,mission_stage,mission_target,mission_item,mission_origin,mission_result,last_mission_type,last_mission_system;
 Job jobs[MISSION_SLOTS]; int job_n,job_sel;
 int story,story_flags,pip_sys,pip_eng,pip_wep,voice_who;
 int guild_chapter,guild_flags,guild_choice;
 int voice_role,voice_seed;
 int campaign_stage,campaign_choice,campaign_flags;
 float campaign_distance,campaign_fuel;
 int saga_chapter,saga_step,saga_flags,saga_choice,saga_dest,saga_start;
 int saga_trust[4];
 int passenger_dest,passenger_kind,passenger_pay,gift_flags;
 int npc_kills,shots,discoveries,scanned_flora,scanned_fauna,scanned_minerals,scanned_anomalies,ai_phase;
 uint8_t visited[32],landed_planets[256]; char message[96],voice[160],collide[40]; float voice_time;
 /* Living-galaxy named travellers (save V12). See travellers.h */
 struct { uint8_t sys,dest; int8_t slot; uint8_t flags; } travellers[12];
} Game;
extern const Good goods[GOODS];
extern const PlayerShip player_ships[];
extern const int player_ship_count;
Vec3 add(Vec3 a,Vec3 b); Vec3 sub(Vec3 a,Vec3 b); Vec3 mul(Vec3 a,float s);
float dot(Vec3 a,Vec3 b); float length(Vec3 a); Vec3 norm(Vec3 a);
Vec3 forward(const Game *g); Vec3 camera(const Game *g,Vec3 p);
int mesh_id(const char *name); int fuel_cargo_units(const Game *g); int cargo_used(const Game *g); int cargo_capacity(const Game *g); int galactic_price(int item);
void galaxy(System out[256]); float distance_ly(const Game *g,int a,int b);
void game_init(Game *g); void game_spawn(Game *g); void market(Game *g);
int trader_offer_hail(Game *g,int npc_id);
void system_bodies(Game *g);
int danger_rating(const Game *g,int system);
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
void add_crime(Game *g,int points);
int cargo_contraband(const Game *g);
int goods_restricted(int item);
int police_fine(const Game *g);
int police_resolve(Game *g,int jail);
int police_escape(Game *g);
int police_scan_submit(Game *g);
int police_scan_refuse(Game *g);
int police_pay_desk(Game *g);
void police_begin(Game *g,int phase);
int approach_planet(Game *g,int body);
int enter_planet(Game *g); void leave_planet(Game *g);
int land_planet(Game *g); int takeoff_planet(Game *g); int eva_toggle(Game *g);
int eva_can_board(const Game *g);
void game_eva_tick(Game *g,float dt,float turn,float pitch,int walk,float strafe,int jet);
float terrain_height(const Game *g,float x,float z);
int terrain_is_water(const Game *g,float x,float z);
Vec3 surface_site(const Game *g,int i);
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
Vec3 hub_position(const Game *g,int hub); int nearest_hub(const Game *g);
int prosperity(const Game *g,int system);
int system_rock_belt(int sys); int system_ice_belt(int sys); int system_whales(int sys); int system_comet(int sys); int traffic_budget(const Game *g);
int mission_count(const Game *g);
int jobs_active(const Game *g);
int mission_destination(const Game *g,int offer);
int mission_type_for_offer(const Game *g,int offer);
const char *mission_name(int type);
const char *mission_brief(const Game *g,int offer);
int mission_reward(const Game *g,int offer);
int mission_risk(const Game *g,int offer);
const char *mission_objective(const Game *g);
const char *mission_objective_at(const Game *g,int slot);
int is_mission_target(const Game *g,int target_id);
int mission_interact(Game *g,int target_id);
int mission_offer_active(const Game *g,int offer);
int abandon_mission(Game *g,int slot);
int accept_mission(Game *g,int offer);
void mission_timers(Game *g,float dt);
void message(Game *g,const char *s); void speak(Game *g,int who,const char *s); void encounter_respond(Game *g); void encounter_ignore(Game *g); void game_tick(Game *g,float dt,float turn,float pitch,int throttle,int fire);
void launch(Game *g); int dock(Game *g); int refuel_full(Game *g); int trade(Game *g,int item,int buy);
int buy_ship(Game *g,int i); int jump_start(Game *g); int contract_accept(Game *g); int fire_missile(Game *g,int target_id);
int salvage(Game *g,int target_id);
int save_game(Game *g,const char *path); int load_game(Game *g,const char *path);
int game_tests(const char *path);
#endif
