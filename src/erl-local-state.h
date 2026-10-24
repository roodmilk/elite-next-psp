#ifndef ERL_LOCAL_STATE_H
#define ERL_LOCAL_STATE_H
/* The active legacy career fields are a view of one system's season. Career
 * XP, licence and fleet stay global. Byte-sized placings keep PSP memory bounded. */
typedef struct {
 uint32_t number,ship_id,seed,history_count,history_wins,history_units,history_xp;
 uint16_t points[7];uint8_t started,active,hub,round,division,place,wins[7],results[4][7];
 float trial_times[6];DockRoll prize;uint32_t prize_state;
} ErlLocalSeason;
typedef struct {
 uint32_t board_until[256][3];
 uint32_t milestones,rivals_beaten,records,latest_system,latest_mode,latest_objective,latest_place,latest_units,latest_xp,latest_sponsor;
 float latest_time,best[3][7],course_best[256][4];uint16_t beaten[256];
} ErlProgress;
/* Appended V37 suffix. The V36 local-season stream is unchanged. */
#define ERL_PROGRESS_BYTES (256*3*4+10*4+22*4+256*4*4+256*2)
typedef struct {uint32_t ready,current,selection,rules_pending,pending_count;DockRoll pending[32];RaceRules event_rules;ErlLocalSeason season[256];ErlProgress progress;} ErlLocal;
#define EAX_SEASON_BYTES (7*4+14+6+7+28+6*4+9*4)
#define EAX_SAVE_BYTES (20+32*32+64+256*EAX_SEASON_BYTES)
#endif
