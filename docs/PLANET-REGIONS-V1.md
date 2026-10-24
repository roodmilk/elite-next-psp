# Planet regions v1 — first playtest checkpoint

Scope: compact handoff dated 2026-10-10. Existing destinations only; tutorial Earth remains authored. User chose nearby Lave comparison regions and preservation of safe saved surface locations, with same-world pad recovery only when needed. No new artwork is generated.

## Asset coverage

The six kit rows each contain four 48×64 props, not a complete environment set. Existing baked pixels retain their subject bounds and transparency. All types below are integrated/adapted through the shared surface framework; bespoke assets listed on the right remain missing.

| Ground type | Reused assets | Procedural treatment | Missing bespoke art/mechanics |
|---|---|---|---|
| Temperate | Lave trees; forest/coast kit | Meadow, forest, valley and cliff relief/cover | Dedicated cliff assets |
| Overgrown | Forest trees/ferns | Tall trees; dense and open basins; wet pockets | Giant roots/vines |
| Ocean | Coast kit | Island relief, tidal flats and coastal wetness | Coral/floating plants |
| Archipelago | Coast kit | Lagoon/beach/highland recipes | Mangroves/reefs |
| Desert | Desert kit | Dunes, mesas, dry basins and rare wet oases | Arches |
| Salt | Adapted desert rocks | Pale stepped flats, brine pockets | Salt spires/caverns |
| Frozen | Ice rocks/low plants | Snowfields, glaciers and icy ridges | Ice caves |
| Tundra | Ice kit | Moss colours, sparse conifers, thaw pockets | Dedicated tundra flora |
| Barren | Shared basalt rocks | Crater depression, dust flats, exposed ridges | Lava tubes |
| Scorched | Desert rocks | Craters, baked flats, dark uplands | Glass formations |
| Volcanic | Volcanic kit | Basalt, ash and caldera relief | Lava-specific geometry |
| Hell | Volcanic kit | Harsh relief, dark crust/ember palette | Chasms/obsidian structures |
| Toxic | Adapted forest scrub | Chemical colours and wetland pockets | Toxic flora/vents/hazards |
| Fungal | Adapted forest plants | Purple groves, soft wet basins | Mushroom silhouettes/spores |
| Swamp | Forest kit | Dense low wooded wetlands | Roots and dedicated reeds |
| Crystal | Adapted ice rocks | Coloured mineral ridges/terraces | Crystal forests/caves |
| Carbon | Basalt rocks | Graphite dunes, dark ridges and pools | Diamond formations |
| Metal-rich | Desert rocks | Rust plains, ore ridges and scree | Metallic spires |

Gas families use existing platform machinery/gardening and linked decks. Banded, storm, ice and hot families receive deterministic compatible region metadata and orbital palettes. Bespoke gas-region weather/gameplay is deferred. Shared fauna/resources remain existing species; this pass does not claim regional species adaptation.

## Generation contract

`src/planet-regions.h` freezes generation version **1**, 18 ground IDs, four gas IDs, 108 ground biome IDs and 16 gas biome IDs. IDs, salts, selection, centres and recipe values define this version's derived geography. Future geography changes must explicitly migrate/version it rather than silently modifying v1. V32 saves retain their existing format and world IDs; normal commanders still load docked as before. Resumable non-Earth opening surface checkpoints keep safe positions, with floor correction/recovery when necessary.

Type weights use the handoff distribution, conditioned on the existing broad class: ocean slots select Temperate/Ocean/Archipelago/Swamp; rocky slots select compatible ground types. Exact quotas are not forced by changing body classes. Lave I/II/III/IV retain Temperate/Overgrown/Banded/Frozen overrides. Earth keeps its existing terrain, flora and story.

A bounded 16-entry profile cache seed-selects compatible pools using the 20/50/25/5 selection proportions, clamped to four for gas families. Weighted nearest-centre regions blend relief and colour continuously. Lave has deliberate close comparison regions; the existing coast/observatory remain. Shared region recipes drive actual collision-mesh relief, terrain colour, water mask and prop density/placement. Drawing and collision share prop cells and world water cells. Pads and interaction sites are kept clear. The existing geometry LOD, depth buffer and tagged caches are reused; there are no per-frame allocations.

`planet.h` uses existing baked kit pixels galaxy-wide, with restricted mineral-only silhouettes on bare worlds. Native ground grain retains the region's colour instead of overwriting it with the former universal meadow palette. Landing voice and landed Codex records show the generated type; Codex discovery gates remain. Orbital art uses the same seed/type palettes. Specialist named cave/arch/root regions currently have terrain/cover recipes, not actual caves, arches or root labyrinths.

## Resumed surface pass — 2.5.361

The player explicitly requested continuation after the menu checkpoint. Signed lattice coordinates were added to unsigned hash jitter, wrapping negative prop positions to roughly 4.29 billion metres. Converting the lattice term to float before addition restores trees and rocks in those quadrants. Drawing and obstacle collision share the corrected positions; the matching cosmetic grass expression is fixed too. Positive cells, biome selection, terrain, seeds, generation v1 and V32 saves remain unchanged. Safe-position recovery handles resumed checkpoints that overlap a restored obstacle.

A tagged 16-entry pad-origin cache and per-profile biome ground-colour cache avoid repeated seed trigonometry and constant palette calculations. Exact old arithmetic is verified over all 1,380 destinations, forced cache aliases and region sample positions. An attempted water occupancy optimization failed image-equivalence and was removed before release; it is not in this EBOOT.

31 region checks pass, including 7,203 signed cells across all quadrants. The fern-basin viewpoint now contains 70 visible production props rather than zero, ice canyon four rather than zero, and giant-forest port 50 rather than 17. Ten native region captures and twelve same-position timing scenes are recorded. Earth opening has 343 passing checks; EVA has 12. Normal boot completes with the retained 7 MiB heap.

Lave II surface CPU averages are 41.495/33.058/35.907 ms in the three region views, versus 41.848/32.185/33.488 ms before this repair. Restored props increase some costs; stable 30–60 fps is not yet achieved or certified. Comparable walking means range from 6.864 to 40.327 ms. Preserve this correctness repair and target measured terrain/sky/prop work next. Specialist cave, root, fungal/crystal silhouettes and gas weather remain deferred. Evidence: BUILD-2.5.361.txt.

Native visual review of this exact EBOOT: `outputs/planet-regions-20261010-115921-678`. Fern basin has restored large tree trunks/canopies, but close low foliage remains visibly coarse. The frozen region has continuous snow terrain and distant ridges, but its named canyon lacks convincing dedicated canyon geometry. Desert and toxic captures have distinct sky/ground palettes and sparse/dense vegetation differences; toxic flora still reuses forest silhouettes. These are working generated regions, not completed bespoke environments.

Next bounded performance investigation: the walking report groups early tree drawing and airport meshes into its “sky” stage, so do not attribute the entire stage to sky gradients. Terrain vertex misses also recompute five seed-fixed mountain centres with trigonometry; investigate caching those constants with full identity tags and exact coordinate/output comparison before changing mesh density. Keep the tested 2.5.361 release intact while measuring a separate candidate. Ask before generating specialist art.

## Continuation

Changed source: `planet-regions.h`, `planet-profile.h`, `game.c`, `game.h`, `planet.h`, `surface-water.h`, `art-runtime.h`, `discovery-atlas.h`, `main.c`; opt-in review: `planet-regions-review.h` and `tools/review-planet-regions.ps1`.

First checkpoint: **2.5.359**, retaining 2.5.358 Command Deck and 2.5.357 Multiplayer. Exact EBOOT SHA256 `AAD49EB1F9FB64714D263955F4AB522E4D60FDDD811045E2E930FAA173529AF3`. Normal startup now completes after reducing the fixed heap from 8 to 7 MiB: the larger combined executable could not allocate its former heap in PSP-1000 user memory. Region review retains over 6 MiB of uncommitted heap; actual PSP boot remains a player test.

The bounded galaxy sweep reports 1,038 ground worlds, 342 gas destinations and 3,898 selected regions. Counts by type: Temperate 230, Overgrown 46, Ocean 76, Archipelago 60, Desert 98, Salt 27, Frozen 96, Tundra 69, Barren 110, Scorched 43, Volcanic 45, Hell 9, Toxic 26, Fungal 10, Swamp 62, Crystal 5, Carbon 11, Metal-rich 15; Banded/Storm/Ice/Hot giants 83/93/80/86. These are seeded results conditioned on existing world classes, not forced catalogue quotas.

27 region checks, 343 Earth opening checks and 27 Command Deck checks pass on that exact artifact. Regions, collision continuity, cache eviction/revisit, all landing pads, Lave site exits/trail, four landing/EVA/takeoff flows, checkpoint recovery and V32 persistence are covered. Actual native captures are in `outputs/planet-regions-20261010-111311-090`. All authored meadow captures remain byte-identical to the pre-change baseline. Ten representative surface views are captured; no claim of individually inspecting every destination.

Emulator CPU rendering in the twelve same-position scenes: ground averages 31.644–44.698 ms versus 28.306–41.732 ms baseline; Lave II increases about 8–18%, requiring further tuning. Gas averages 35.290/17.992/6.880 ms versus 43.661/17.748/6.864 ms. This is not measured hardware FPS or stable 60 fps certification. Fern-basin and ice viewpoints can remain sparse despite visible forest props at the port; specialist vegetation/geometry is deferred. Detailed evidence: `BUILD-2.5.359.txt`.

The same 15 inherited core failures remain; steering/radio pass. The broad input fixture is incomplete within its 120-second smoke window. Whole-game certification, PSP suspend/audio and two-device tests remain open. User subsequently requested an immediate menu-speed/no-flash/ship-theme update on top of this checkpoint; retain this planetary foundation while making that focused update.

The first checkpoint stopped for feedback; the player subsequently authorized resumed planet development and interrupted it for the 2.5.361 menu-transition release. Continue from the repaired regional foundation after delivering that EBOOT. New signature art, gas-region mechanics, caves, swimming and a wider polish pass require a later request. Git pull is currently blocked by the two pre-existing untracked music placeholders; autostash was restored. Preserve unrelated shared work and player music.
