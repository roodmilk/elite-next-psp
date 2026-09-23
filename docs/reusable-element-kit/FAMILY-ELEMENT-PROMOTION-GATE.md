# Family element promotion gate

This gate freezes the current Freight/Prospector/Research candidate set without promoting it to runtime.

## Ready

- Twelve approved anchors exist as normal/contrast pairs.
- Every candidate has an exact native target size.
- One variant set is estimated at `19,610` packed 4-bit bytes; both variants are `39,220` bytes before shared palette, alignment, metadata, and draw overhead.
- Native `480x272` normal and high-contrast proof frames exist.
- Systems reports all native dimensions valid, both family sets under the `28,560-byte` room ceiling, and selector scaffolding clean across 63 samples.
- Composer has deterministic room-relative placement slots in its crosswalk.
- Qube Freight now has ten individual normal/contrast candidate IDs at `11,405` packed bytes per variant set; its native proof captures are present.

## Still required before runtime promotion

1. Station and Gameplay must bind any visible candidate to an existing hotspot or explicitly mark it visual-only. The art kit cannot create a new action, NPC, door, feature, or capability.
2. Composer must reserve the final selector placement hunk and emit the candidate IDs through the frozen `room-selector-v1` path.
3. Systems must verify byte-equality on repeated generation, final packed representation, static/temp memory, draw count, and frame time from the integrated EBOOT.
4. Station/Story must capture entry, person selection, speech/result states, disabled state, and SHIP traversal at native resolution.
5. Planetary must approve the Prospector desert/lander and Research orbital-window motifs as cosmetic dependencies only; their presence must not alter world or mission state.

Until all five items are evidenced, candidate files stay `runtime_ready:false` and the generic station fallback remains the active implementation.
