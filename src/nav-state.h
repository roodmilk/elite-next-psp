#ifndef ELITE_NAV_STATE_H
#define ELITE_NAV_STATE_H
#define NAV_BUOYS 6
#define NAV_FIXED_KEYS 27
#define NAV_KEYS 42
#define NAV_PLANET(b) (32+(b))
#define NAV_MASK ((1u<<NAV_FIXED_KEYS)-1u)
#define NAV_STATION(h) (h)
#define NAV_DISCOVERY(s) (3+(s))
#define NAV_ANOMALY(s) (11+(s))
#define NAV_BUOY(s) (15+(s))
#define NAV_REGION(s) (21+(s))
enum {NAV_VISITED=1,NAV_BUOY_SOURCE=2,NAV_MISSION=4,NAV_RUMOUR=8,NAV_CHART=16};
typedef struct {
 uint32_t visited[256],supplied[256];
 uint8_t scanned[256],paid[256];
 uint16_t waypoint_system,waypoint_key; /* key+1; zero is no active waypoint. */
 uint16_t search_system,search_key; /* rumour search area; never a live target. */
} NavigationState;
#endif
