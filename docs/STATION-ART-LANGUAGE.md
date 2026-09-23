# Station art language — native PSP rollout

This is the shared visual contract for station rooms, bars and station-facing screens. It extends the approved Second Shift direction: authored people, layered lived-in architecture, warm pulp light and deliberate pixel clusters at native PSP resolution.

## Native contract

- Frame: 480×272, 1× source intent, framebuffer stride 512.
- Room: `(6,20) 340×168`; options: `(354,20) 120×168`; feedback begins at `y=190`.
- Sampling: nearest neighbour only. Runtime scene bitmaps are opaque ARGB1555 or packed palette indices with an explicit conversion.
- Safe anchors: header, options, feedback and `SHIP` return are UI-owned. Room art leaves the lower-right `SHIP` rectangle `(274,158) 68×26` clear.
- Readability: people are silhouettes first, faces/props second; no detail that disappears into noise at 480×272. High contrast must preserve shapes and labels, not just recolour them.
- Pixel language: hard edges, clustered shading, selective ordered dithering, restrained highlights and visible material wear. Avoid sparse geometric placeholders and avoid smooth painted gradients.

## Palette roles

The 11 roles are canonically owned by `src/station-art-kit.h`; `src/station-art-style.h` aliases them and adds layout/family contracts. VOID, CHARCOAL, SLATE, OLIVE, RUST, OCHRE, CREAM, LAVENDER, CYAN, AMBER and DANGER are not to be redefined per screen. Danger red is a semantic warning, never ambient decoration. Cyan identifies functional light or scan state. Cream/Ochre carry readable structure; Charcoal/Slate carry depth; Olive/Rust carry material identity.

## Reusable family modules

| Family | Structural hero | Foreground | Middle depth | Lighting |
|---|---|---|---|---|
| Freight | berth window and arrival board | counter/timetable | rail and cargo lane | warm work lamps |
| Prospectors' rest | ore samples and lander window | assay bench | racks and bunk | dusty task light |
| Research lounge | observation window and instrument | survey console | cabinet and rail | cool cyan instruments |
| Merchant salon | ledger rotunda and private booth | curved sales counter | manifests/display | polished amber |
| Frontier mess | patched service hatch and ration wall | repair bench | lockers/dock | low olive worklight |
| Outlaw den | coded notice wall and guarded hatch | card table | screened booth/cargo door | red only for explicit danger |

These are authored composition modules. Procedural variation may select stable fittings, material wear, window body and lamp pattern from immutable station/body identity. It must not randomize anchors, NPC identity, mission truth, collision, or palette roles. A family is not complete until its normal and high-contrast captures are readable.

## Screen ownership

- Arrivals, Canteen/Bar, Market, Shipyard/Outfitting, Mission Board and GalNet use the same frame grammar and options treatment.
- Authored bitmap owns room atmosphere and major silhouettes.
- Runtime UI owns options, text, selected outlines, SHIP and stateful warnings.
- Gameplay owns which entries exist and their truth. Art must not imply a job, destination, purchase or reward that the state does not authorize.
- Station owns entry/return and room identity; Systems/Gameplay own persistence and transaction semantics.

## Rollout status

Second Shift is the approved first bar concept and asset family. This rollout adds Arrivals as the first additional complete screen. Market, Shipyard, Mission Board and GalNet receive family/layout contracts here, not invented runtime screenshots or fake completed content. The directly authored Arrivals bitmap is a bounded runtime candidate but still requires scene dispatch and native combined capture before it can be called shipped. The generated larger-canvas Arrivals image remains style reference only.

## QA gates

For every new family/screen: native normal + high contrast capture; options/feedback/SHIP bounds; no Game or RNG mutation by art; byte count and per-frame cost; repeated identity gives identical pixels; current generic room behavior remains intact elsewhere. Report emulator evidence separately from physical PSP validation.
