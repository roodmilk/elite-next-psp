# Descriptor-backed Freight / Research dispatch

Bounded dispatch plan for the next two family rooms. This is a read-only
descriptor-to-kit contract; it does not activate runtime art while the native
candidate files remain `runtime_ready:false`.

## Targets

| Target | Descriptor | Family | Selector hash | Current renderer |
|---|---|---|---|---|
| Qube H0 Canteen | system `1`, hub `0`, room `2` | Freight `0` | `0x2e5fa27c` | truthful generic fallback |
| Xequerin H0 Canteen | system `4`, hub `0`, room `2` | Research `2` | `0x5dd95221` | truthful generic fallback |

Both identities are read from `src/generated/procedural-room-descriptors.h` and
validated by `tools/procedural-room-proof-test.c`. The selector hash chooses
only arrangement/material variation; it does not choose NPCs, speech,
capability, body identity, danger, economy or audio.

## Frozen Element Kit dispatch

| Target | Draw order | Frozen ID | Native role | Placement `(x,y)` relative to room |
|---|---:|---|---|---:|
| Qube Freight | 1 | `counter` | service/work surface | `(64,100)` |
| Qube Freight | 2 | `task-lamp` | practical light | `(126,32)` |
| Qube Freight | 3 | `dock-window` | berth/body window frame | `(176,28)` |
| Xequerin Research | 1 | `survey-console` | survey activity landmark | `(64,100)` |
| Xequerin Research | 2 | `task-lamp` | practical light | `(126,32)` |
| Xequerin Research | 3 | `dock-window` | authoritative body window frame | `(176,28)` |

The Research branch intentionally uses `survey-console`; the frozen manifest
does not authorize a generic counter for Research. The composer owns only these
cosmetic placements. Station/Story owns people and doors; Gameplay owns the
actual Canteen actions; Planetary owns any body view; Story owns speech.

## Reorte exception and fallback

Reorte system `39` / H0 Canteen remains the authored Second Shift exception.
It bypasses procedural accents and generic NPC/door artwork in the exact room.
All other un-certified rooms remain on the truthful renderer. The runtime gate
`PROC_ROOM_KIT_RUNTIME_READY` is deliberately `0`; this prevents the current
primitive accent path from presenting as final family art.

## Blocker

The dispatch mapping is ready for Systems/Station review, but runtime promotion
is blocked by the missing integrated native room captures, hotspot ownership
sign-off, and measured frame/static/temporary-memory evidence. Enabling the gate
before those checks would violate the Second Shift fidelity contract.
