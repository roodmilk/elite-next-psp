/* Fixed-width persistent mission data; live NPC/debris handles never enter saves. */
#define EXPEDITION_FIRST 27
#define EXPEDITION_COUNT 40
#define EXPEDITION_STAGES 8
#define EXPEDITION_TARGET_ID (RACE_TARGET_ID+1)
enum {XP_HAIL,XP_DOCK,XP_COMBAT,XP_RECOVER,XP_BUY,XP_ESCORT,XP_CHASE,
 XP_PATROL,XP_CHOICE,XP_MINE,XP_REFINE,XP_COMPETE,XP_WAYPOINT,XP_FLORA,
 XP_BEHAVIOUR,XP_SAMPLE,XP_SITE,XP_DISCOVERY,XP_WHALE,XP_OBSERVE,XP_ANOMALY,
 XP_MAP,XP_RACE,XP_GOALS};
typedef struct {int kind,arg,need,loc,seconds;const char *objective,*speech;} ExpeditionStage;
typedef struct {const char *name,*sender;int role,gear,count;ExpeditionStage stages[EXPEDITION_STAGES];} ExpeditionDef;
typedef struct {
 uint32_t seed,stage,progress,seen,done,choice,unread,bonus,advance,spent;
 uint32_t systems[3],worlds[2],held[GOODS],history[EXPEDITION_STAGES];
 float elapsed,dwell,escort_distance;
 float health[6];
 uint32_t destroyed,recovered,rival_count,passengers,shots_at_start;
} ExpeditionState;
typedef struct {
 uint32_t key;int system,job,npc[6],pod[8],site,external_id;
 Vec3 anchor,waypoint,previous;int built,dead[6];
} ExpeditionRuntime;

#define EXPEDITION_SAVE_BYTES (sizeof(ExpeditionState)*MISSION_SLOTS)
