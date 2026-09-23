# Procedural room proof v1

Bounded proof targets: H0 CANTEEN at Qube, Xequerin and Reorte. The proof uses
the baked Systems descriptor table and the frozen Element Kit manifest; it does
not activate runtime content.

## Selector parity

Source header: `src/generated/procedural-room-descriptors.h`
Descriptor source SHA: `9a02e010ff9308522a9e0e9327d65ccd8b6e592254f335c36c9219fca04eee62`
Selector: `room-selector-v1` / selector version `1`
Room: `CANTEEN` / room id `2` / hub `H0` / galaxy `G0`

| Target | Systems id | Family | Arrangement | Landmark | Material | Exception | Selector hash |
|---|---:|---:|---:|---:|---:|---:|---:|
| Qube | 1 | Freight `0` | 98 | 155 | 18 | 0 | `0x2e5fa27c` |
| Xequerin | 4 | Research `2` | 238 | 155 | 72 | 0 | `0x5dd95221` |
| Reorte | 39 | Frontier `4` | 20 | 155 | 120 | 2 | `0x193c0782` |

`tools/procedural-room-proof-test.c` reconstructs each identity from the baked
descriptor, checks selector equality, checks deterministic repeat, checks the
normal/high-contrast plan pair, bounds placements to the room viewport, and
rejects identity-only dice activation.

## Native evidence packet

| Target | Normal evidence | High-contrast evidence | Current status |
|---|---|---|---|
| Reorte | `assets/second-shift-native-v2.png` | pending integrated capture | native-grid authored benchmark; not runtime proof |
| Qube | `assets/preview/reusable-element-kit/assembled-freight-normal.png` | `assets/preview/reusable-element-kit/assembled-freight-contrast.png` | layout/contrast evidence only |
| Xequerin | `assets/preview/reusable-element-kit/assembled-research-normal.png` | `assets/preview/reusable-element-kit/assembled-research-contrast.png` | layout/contrast evidence only |

The Qube and Xequerin sheets are deliberately not promoted as final art. They
still require the Second Shift gate: layered depth, immediate family landmark,
3–5 real hotspots, readable people/activity, material clusters, deliberate
lighting, clean UI bands and a truthful SHIP route. Reorte needs an integrated
normal/high-contrast room capture, not only the authored reference frame.

## Truthful capability state

All three targets retain the Systems/Station baseline CANTEEN capability set:
generic BARTEND local rumour, generic TRAVELER passenger enquiry subject to
existing cabin/hold checks, room doors, and YOUR SHIP / Triangle return. The
selector hash chooses only cosmetic identity. Dice/cards, wagers, faction
pressure, danger, services and audio are not inferred from the descriptor.

## Frozen kit and budget check

The proof consumes only manifest IDs and native dimensions from
`docs/reusable-element-kit/ELEMENT-MANIFEST.json`. Core packed costs are:
Freight `11055` bytes, Prospector `11108` bytes, Research `10872` bytes, plus
the shared `44` byte palette. People and doors remain single-owner layers;
the composer does not duplicate them. Static bytes, plan bytes, temporary memory,
draw calls, native input/state captures and save/economy checks remain required
before activation.

## Gate result

**Selector/plan proof: ready for Systems review.**
**Native integrated room proof: not yet passed.**
**Runtime activation: off.**
