# Station cue-kit approval matrix

Commit under review: `2f4b5c9`  
Platform: PSP `480x272`, authored at `1x`  
Runtime code changes: none

This is the Station integration gate for the art-only cue kit. The art and
native proof sheets are complete; `runtime_ready` remains `false` until
Station confirms the existing hotspot owner and draw anchors.

| Room | Normal / contrast | Owner | Slot | Existing hotspot | runtime_ready |
|---|---|---|---|---|---|
| ARRIVAL | `room-00-sign-*`, `room-00-door-*`, `room-00-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |
| SHOP | `room-01-sign-*`, `room-01-door-*`, `room-01-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |
| CANTEEN | `room-02-sign-*`, `room-02-door-*`, `room-02-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |
| CARGO | `room-03-sign-*`, `room-03-door-*`, `room-03-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |
| GUILD | `room-04-sign-*`, `room-04-door-*`, `room-04-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |
| CLINIC | `room-05-sign-*`, `room-05-door-*`, `room-05-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |
| CUSTOMS | `room-06-sign-*`, `room-06-door-*`, `room-06-low-door-*` | Station | TOP_LEFT / TOP_RIGHT / LOW_LEFT / LOW_RIGHT | `SC_H_EXIT`, `sc_room_short(destination_room)` | `false` — Station approval pending |

## Fixed native anchors

| Cue | Native size | Anchor / safe rectangle | Binding | runtime_ready |
|---|---:|---|---|---|
| Top-left plaque | `80x16` | `(4,20)`; ends at `y=36` before the `(4,36)` door | selector label | `false` — Station approval pending |
| Top-right plaque | `80x16` | `(260,20)`; ends at `y=36` before the `(286,36)` door | selector label | `false` — Station approval pending |
| Lower-left doorway | `42x26` | `(70,120)` | `SC_H_EXIT` | `false` — Station approval pending |
| Lower-right doorway | `42x26` | `(180,120)` | `SC_H_EXIT` | `false` — Station approval pending |
| Ship return marker | `24x12` | screen `(296,165)` inside `(274,158) 68x26` | `SC_H_EXIT / SC_EXIT_SHIP` | `false` — Station approval pending |
| Arrival window | `128x54` | scene-local `(80,20)` inside `(72,10) 176x70` | `SC_H_FEATURE / BERTH WINDOW` | `false` — Station approval pending |
| Route marker | `16x8` | right destination row, draw point not reserved | visual-only | `false` — exact draw point pending |

## Evidence

- `./tools/test-station-cue-kit.ps1` passes: 48 native assets, 7 room
  bindings, canonical palette and 480x272 proof sheets.
- Repeat bake matches all 54 checked-in PNG hashes with zero mismatches.
- Anchor audit passes all four doorway slots with zero viewport failures.
- The art-only kit does not create, move or rename an interaction.

## Open gate

Station must confirm the anchor ownership and visual integration points above.
Until then, the manifest remains `candidate_art_only` and the assets must not
be promoted into runtime or merged into the gameplay branch.
