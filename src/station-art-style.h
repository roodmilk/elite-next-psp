/* Shared native art language for station rooms and their authored variants.
 * Values are PSP framebuffer RGB roles, not a free-form per-screen palette. */
#ifndef ELITE_STATION_ART_STYLE_H
#define ELITE_STATION_ART_STYLE_H

#include "station-art-kit.h"

#define STATION_ART_SCREEN_W 480
#define STATION_ART_SCREEN_H 272
#define STATION_ART_ROOM_X 6
#define STATION_ART_ROOM_Y 20
#define STATION_ART_ROOM_W 340
#define STATION_ART_ROOM_H 168
#define STATION_ART_OPTIONS_X 354
#define STATION_ART_OPTIONS_Y 20
#define STATION_ART_OPTIONS_W 120
#define STATION_ART_OPTIONS_H 168
#define STATION_ART_FEEDBACK_Y 190

/* Shared 11-colour pulp cover palette. Use roles, not arbitrary RGB drift. */
#define STATION_ART_VOID ART_VOID
#define STATION_ART_CHARCOAL ART_CHARCOAL
#define STATION_ART_SLATE ART_SLATE
#define STATION_ART_OLIVE ART_OLIVE
#define STATION_ART_RUST ART_RUST
#define STATION_ART_OCHRE ART_OCHRE
#define STATION_ART_CREAM ART_CREAM
#define STATION_ART_LAVENDER ART_LAVENDER
#define STATION_ART_CYAN ART_CYAN
#define STATION_ART_AMBER ART_AMBER
#define STATION_ART_DANGER ART_DANGER

enum {
 STATION_ART_FAMILY_FREIGHT=0,
 STATION_ART_FAMILY_PROSPECTOR,
 STATION_ART_FAMILY_RESEARCH,
 STATION_ART_FAMILY_MERCHANT,
 STATION_ART_FAMILY_FRONTIER,
 STATION_ART_FAMILY_OUTLAW
};

/* Family changes structure, lighting and props; tint alone is not a variant. */
typedef struct {
 const char *name;
 const char *hero;
 const char *foreground;
 const char *middle;
 const char *window;
 unsigned lamp;
} StationArtFamily;

static const StationArtFamily station_art_families[6]={
 {"FREIGHT","berth window + arrival board","counter / timetable","rail / cargo lane","warm work lamps",STATION_ART_AMBER},
 {"PROSPECTOR","ore sample wall + lander window","scarred assay bench","sample racks / bunk","dusty task light",STATION_ART_OCHRE},
 {"RESEARCH","observation window + instrument","survey console","specimen cabinet / rail","cool cyan instruments",STATION_ART_CYAN},
 {"MERCHANT","ledger rotunda + private booth","curved sales counter","hanging manifests / display","polished amber lamps",STATION_ART_AMBER},
 {"FRONTIER","patched service hatch + ration wall","repair bench","utility lockers / dock","low olive worklight",STATION_ART_OCHRE},
 {"OUTLAW","coded notice wall + guarded hatch","shadowed card table","screened booth / cargo door","red only for explicit danger",STATION_ART_RUST}
};

#endif
