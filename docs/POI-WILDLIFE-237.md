# POI, wildlife and leaf visual pass — 2.5.237

## Scope

Shared 3D exterior rendering now handles all 12 planetary POI families, across the existing generated sites. Contract buildings use the same weather/garden/archive kits. Spaceport buildings receive windows, pitched roofs and framed entrances. This is exterior art work: it does not replace the existing point-and-click interior scenes, add new quests, change save format or make the solid POI footprints into enterable 3D interiors.

| Family | Visible design |
| --- | --- |
| Supplies | Loading canopy, stacked panelled crates and terminal |
| Ruins | Unequal stone columns, arch, bands, caps and mossy rubble |
| Observatory | Cream hemisphere, observation slit, window band and instrument mast |
| Rescue | Pitched shelter, medical cross, beacon mast and console |
| Seed garden | Ribbed biodome, open lower band, raised planting beds and access door |
| Fossils | Stepped dig edges, spine, ribs, skull and survey terminal |
| Thermal | Three cylindrical stacks, collars, pipes and control block |
| Wreck | Broken-wing hull, cockpit, detached pod and salvage console |
| Migration | Braced elevated observation cabin and antenna |
| Crystals | Cluster of faceted, pointed purple spires |
| Archive | Octagonal vault, tapered roof, framed entrance and terminal |
| Weather | Three lattice pylons, braces, crossarms, beacons and solar panels |

The models share restrained cream, blue-grey metal, teal glazing and ochre accents. Foundation skirts fill local terrain variation. Physical signboards replace labels that would otherwise float at the perimeter of the old rectangular models. Existing site seeds, navigation numbers, encounter IDs, rewards and collision reservations are preserved. Small trim projects into the pre-existing collision margin. Open arches/canopies remain perimeter-interaction sites, not newly walk-through interiors.

## Wildlife and foliage

- Eight authored flora families and eight fauna families replace the old simple generated silhouettes. Species seed selects the same noun family used in its name. Prefix-based subtle palette variation remains deterministic.
- World, field guide and Codex share the same 48x64 indexed sprite and colour selection. World projection is upright and depth-tested against terrain, structures and other sprites, including look-up/down movement. Palette channels are clamped.
- Plants sway; winged creatures have small wing motion; grounded creatures have a small foot movement. These are deliberately simple four-phase deformations, not bespoke full skeletal animations or new animal AI.
- Mineral discoveries retain deterministic faceted crystal artwork. Their identity and scanning are unchanged.
- New leaves originate around actual nearby trees, within 180 m, with no more than 24 visible leaf particles. Analytic falling/fluttering motion uses the tree seed and simulation time, no heap allocation or gameplay RNG. Foreground depth hides leaves. High-contrast mode disables them. Existing biome ambience is separate.
- Source atlas was AI-generated using the imagegen skill, then converted by the repeatable palette bake. Full prompt/provenance: `assets/source/field-wildlife/README.md`. The embedded art is 49,152 bytes plus its palette; mineral cache is 24,576 bytes and species palettes 2,048 bytes. No PNG download/runtime asset loading is required.

## Implementation

New `surface-landmarks.h`, `field-world.h`, `field-leaves.h`, generated `field-wildlife-pixels.h`; replaced `field-sprites.h`; wired into `planet.h`. Circle/dome lookup tables and hidden-face rejection avoid repeated trig and unnecessary triangles. Near details are bounded; distant models use fewer segments. Mesh batches flush before their fixed capacity is reached.

`field-art-review.h` is an opt-in native emulator harness, enabled only by `field-art-review.flag` in a disposable directory. It checks all 1,024 generated world identities / 8,192 species records (including records for non-landable gas worlds; not a claim those are explorable), captures each of the 12 actual POI families at two angles, scans a species into the Codex, tests persistence and validates leaves/depth/framebuffer bounds. `tools/review-field-art.ps1` runs it hidden and stops only its own emulator. `tools/field-art-proof.py` packages unmodified native framebuffers.

## Delivery and verification

Local delivery: `C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/POI-Wildlife-2.5.237/EBOOT.PBP`.

Review reports/captures are under that task workspace in `poi-wildlife/final-review`, `poi-wildlife/final-lave` and `poi-wildlife/final-smoke-verified`.

Packaged EBOOT SHA256: `E62448EDD2CAFD2DF774582232FB0A4C0EB5368E7A263B52A1DF56E03DBF518E` (6,762,999 bytes).

Final warning-only formatting cleanup rebuilt byte-for-byte identical to that tested EBOOT. The new field/landmark/review headers emit no compiler warnings; unrelated existing project warnings remain. `git diff --check` passes for the touched tracked runtime/build files.

- Final native art review: RESULT 0 failures. All catalogue slots have valid nonempty indexed art; all 16 authored families occur. All 12 real POI types render at two angles and can be interacted with from the perimeter. Flora scan -> Triangle -> exact Codex record works; discovery survives a docked save/load. Nearby/capped/moving/depth-occluded leaves and wildlife occlusion pass, as do framebuffer guards.
- Final Lave regression: RESULT 0 failures. All 202 walk/rover path positions clear; slopes, ridge wrap, observatory identity/location and accessible interaction pass. Actual walking/turning sequence captured at native 480x272, including other static day/dusk/night views.
- Broad game check on the same packaged binary: one inherited `swept collision blocks station tunnelling` failure; radio and steering RESULT 0 failures. This run deliberately stops after the game group; the inherited incomplete/failing input suite and broad performance group are not claimed to pass.
- Final sampled Lave update + draw: 33.738 ms average, 36.096 ms worst, versus 31.600 / 34.946 ms in 2.5.236. POI gallery draws: 32.819 ms average / 40.843 ms worst, with cold scene setup included. Measurements exclude audio, display wait and capture writes; they are not locked-30-FPS certification.
- The initial additional interaction test fixture missed the normal neutral-input release after landing and attempted saving while undocked. Corrected the fixture to follow those existing rules; did not bypass or change production controls/save restrictions. The failed fixture report is retained as `poi-wildlife/field-art-review-fixture-failure.txt`.

The POI gallery uses real generated sites in systems 0 and 1; those backgrounds have not received Lave I's lush terrain pass. The wildlife line-up is a deliberately staged placement rendered by the actual game, and the two species sheets are a native art-review layout. Codex and Lave tree views are separately captured. The leaf GIF advances the simulation by 0.25 s per image; it is not a real-time FPS recording.

Physical PSP testing remains necessary. No promise of seamless/locked 30 FPS or 1:1 correspondence with the earlier high-resolution concept art is made. Inherited broad-suite failures are not fixed by an exterior-art change. Shared root/dist EBOOTs, player music, saves and unrelated dirty work remain untouched; no auto-stash, push, tag or public release.

## Next steps

Profile near biodomes/trees and the port on a physical PSP, including audio and input. Add further material texture and hand-polished animation frames if the budget permits. Expand non-Lave terrain art independently. Review the existing conservative collision footprint before enabling walk-through ruins or canopy interiors; do not silently remove those collision protections.
