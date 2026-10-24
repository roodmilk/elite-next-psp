#ifndef ELITE_RUMOUR_STATE_H
#define ELITE_RUMOUR_STATE_H
/* Stable v1 slots: discoveries 0..7, canonical anomalies 8..11, whale pod 12.
 * No transient NPC/debris array index is ever sold as a durable identity. */
#define RUMOUR_SLOTS 13
#define RUMOUR_MASK ((1u<<RUMOUR_SLOTS)-1u)
#define RUMOUR_SAVE_BYTES (8+256*(3+RUMOUR_SLOTS)*4)
typedef struct {
 uint16_t owned[256],found[256],refunded[256];
 uint16_t paid[256][RUMOUR_SLOTS]; /* Existing credits use tenths of a unit. */
 uint32_t active; /* 0 none; 1 + system*RUMOUR_SLOTS + slot. */
} RumourState;
#endif
