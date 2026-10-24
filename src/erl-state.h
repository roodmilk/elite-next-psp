/* V34 ERL career. Five identities cover the original ship and the four unique
 * division prizes. Repeat trophies award units, never silently discarded hulls. */
#define ERL_DIVISIONS 4
#define ERL_FLEET 5
#define ERL_SAVE_BYTES (905+RACE_GATES*68+RACE_RIVALS*108)
enum { ERL_EXHIBITION,ERL_INTRO,ERL_QUALIFIER,ERL_OFFICIAL,ERL_PRACTICE,ERL_TRIAL };
enum { ERL_SPRINT,ERL_CIRCUIT,ERL_RUSH,ERL_TARGET };
typedef struct {
 uint32_t id,paint;int hull,missiles,damaged;uint8_t fit[48],weapon;
 float fuel,health,shield,heat;
} ErlShip;
typedef struct {
 uint32_t serial,paid,season,ship_id,prize_mask,module_pending;
 int referral,ref_system,ref_hub,licence,xp,wins,podiums,takedowns,trophies;
 int active_ship,fleet_n,championship,champ_system,champ_hub,round,division;
 int points[7],round_wins[7],places[4],season_place;
 int event_kind,event_type,event_layout,event_system,event_hub,qualifier;
 int result,result_place,result_units,result_xp,result_points,result_takedowns;
 int track,inbox,dirty;float save_age;
 float real_hull,real_shield,real_heat,real_fuel;int real_damaged;
 Vec3 resume_pos;float resume_yaw,resume_pitch,resume_roll,resume_speed;
 uint32_t resume_rng;int resume,paused;
 ErlShip fleet[ERL_FLEET];
} ErlCareer;

static const int erl_thresholds[4]={0,900,2400,4800};
static const int erl_prizes[4]={22,9,16,19}; /* Needlefish; later prizes unchanged. Legacy Krait ownership stays saved. */
