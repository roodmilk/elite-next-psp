# Procedural room proof — Qube H0 Cargo

Status: deterministic composer proof prepared; runtime activation remains off.
This is the next bounded room after the H0 Canteen selector packet because its
Systems descriptor selects the frozen Freight family and its three composer
modules already have stable generic Element Kit roles.

## Systems identity

| Field | Value |
|---|---:|
| system | Qube (`system_id=1`) |
| galaxy / hub | `G0 / H0` |
| room | `CARGO` (`room_id=3`) |
| family | Freight (`family_id=0`) |
| arrangement | `98` |
| landmark | `102` |
| material | `18` |
| exception | `0` |
| art version | `1` |
| selector hash | `0xbe79913e` |

The hash is read from `src/generated/procedural-room-descriptors.h` and checked
by `tools/procedural-room-proof-test.c` through the `room-selector-v1` function.
The proof does not derive Loader availability, job completion, economy, door
state or SHIP navigation from this identity.

## Deterministic composition

Room-relative coordinates use the native room viewport `(0,0,340,168)`; screen
origin is `(6,20)`. Options, feedback, speech and SHIP regions remain reserved.

| Order | Frozen Element Kit role | Manifest ID | Variant channel | Position | Owner |
|---:|---|---|---|---:|---|
| 1 | cargo rack | `cargo-rack` | selector-derived rack variant | `(62,45)` | Station |
| 2 | panel/sign | `panel-sign` | selector-derived landmark treatment | `(166,36)` | Composer/Kit |
| 3 | counter/work surface | `counter` | selector-derived counter variant | `(118,112)` | Station/Gameplay |

The plan is identical on repeat and differs only by the contrast flag in the
high-contrast pass. The composer owns these three cosmetic placements only. The
Station/Story layer owns the Loader person, door/hatch anchors and hotspot
rectangles; the composer must not duplicate them.

## Capability truth

The authoritative Qube H0 Cargo capability set is: `LOADER existing tip job`,
room doors, and `YOUR SHIP / Triangle return`. The art may show a cargo lane,
lift/rack and work surface, but it must not imply a completed job, reward,
available cargo, alarm, music or additional service. The Loader action remains
the existing Gameplay result path.

## Cost and acceptance

The three frozen generic modules account for `1248 + 576 + 2016 = 3840` packed
4-bit bytes, excluding the shared 44-byte palette and owner-layer people/doors.
The proof also requires the normal/high-contrast native capture pair, exact
hotspot/placement ownership, static/temporary/frame budget measurement, input
traversal (enter → select Loader/props → use truthful job → SHIP), save/economy
preservation and performance evidence.

Current blocker: the composer plan and selector proof are ready, but there is no
integrated native Qube Cargo capture or measured EBOOT runtime report. Keep this
room on the truthful generic renderer until those gates and the Second Shift
depth/material/people/activity review pass.
