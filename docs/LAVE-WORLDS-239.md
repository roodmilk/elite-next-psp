# Lave II–IV planetary exploration — 2.5.239

This completes the current bounded expedition-area pass for the other three Lave planets. It does not make whole spherical planets traversable, create thousands of unique species or roll the new scenery out to the other 255 systems. Lave I's existing island remains intact.

## Playable worlds

| Planet | Existing orbital identity | New expedition presentation |
| --- | --- | --- |
| Lave II | Rocky, forest art family | Mosswood: gentle wooded hills, hanging foliage, ferns, mossy rocks, flowers, drifting spores, falling leaves and existing research/archaeology sites |
| Lave III | Gas giant | Cloud-sea skyport: central landing/garage deck, broad connected causeways, separate research decks, visible parapets, planted specimens, living managed fauna and moving cloud layers |
| Lave IV | Rocky, ice art family | Glacial survey range: rolling frozen ground, snow-covered conifers, ice boulders, low frost scrub, snowfall, pale distant ridges and cold-adapted species colours |

Planet names, body seeds/types, orbital art, species noun families, site IDs and discovery bits remain stable. The Codex now names Lave's expedition regions. Landing computer messages describe the actual environments. Cloud-site names/briefs explain salvaged archaeology, imported samples and managed research rather than implying natural solid ground on a gas giant.

Existing eight discoverable subjects, seven optional sites, a contract installation and a port remain on each planet. Existing point-and-click scenes and reward paths are reused, including the observatory decision where present. The previous eight-family animation/behaviour system remains active. Shared animal trait text was corrected to describe actual movement families instead of assigning random incompatible behaviours.

## Ground, collision and visuals

Lave II/IV now use the same single 160m terrain lattice for rendering and collision, replacing their overlapping coarse/fine meshes. Heights are cached and the port foundation stays flat. World-anchored ground material, sky/day/night/clouds and palette-baked environmental props share the existing depth buffer. Plants/animals receive subtle appropriate Lave III/IV palette adaptations in both world and Codex.

Trees and rocks on II/IV use a shared 70m placement helper and collision reservations. Decorative undergrowth is shorter and range-limited; it is not all solid geometry. Existing 2.1km-radius expedition bounds remain. Far ridges are decorative horizons, not reachable extra continents.

Lave III uses a cached 40m occupancy grid containing the central deck, each actual POI footprint and connecting L-shaped causeways. Visible deck surfaces, exposed-edge parapets, water/void rejection, walker/rover margins and fauna routing share that grid. There is no invisible walkable island between the decks. The slab sides extend beneath the upper surface; clouds are atmospheric backdrop layers, not land. A full volumetric cloud simulation, platform interiors and free-fall are not added.

New pixel-art asset: assets/source/biome-props/kit-alpha.png, generated with the built-in imagegen tool. The imagegen skill supplied a transparent prop kit; tools/bake-biome-props.py crops, palette-bakes and embeds 24 cells at 48x64 (73,728 bytes plus palette). Only the forest, ice and skyport rows are used here; the remaining rows are preparation, not proof of an all-system rollout. Source art is AI-generated, not hand-drawn. Exact prompt/mode/output paths are recorded in assets/source/biome-props/LAVE-239-PROVENANCE.md.

## Native verification

Reports/captures: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/lave-worlds/

- final-worlds/lave-world-tests.txt: 0 failures. Enter, land, disembark, reboard and take off on II/III/IV; finite gentle terrain; all nine site perimeters reachable on foot and by rover using a connected grid with intermediate collision checks; all 24 subjects actually scanned from reachable ground; all 21 optional activities resolve; each port contract pays once; repeat visits do not duplicate cash/cargo; scan/activity progress survives docked save/load. Observatory tests supply the prerequisite clue flags before calling the existing atomic decision API, rather than exercising every dialogue button.
- final-worlds/lave-world-review.txt: 0 failures. Native day/night/turn/pitch/near-prop views, movement captures and framebuffer guards. Capture viewpoints are deterministic QA fixtures, not a recording of a manual full expedition. Native gallery: final-worlds/lave-worlds.png; per-world motion GIFs in the same folder.
- verified-fauna/: focused behaviour and four-pose tests pass; all 2,831 current animal spawns safe across 1,024 worlds.
- verified-lave/: Lave I walk/rover route, observatory and rendering regression passes.
- verified-art/: all 8,192 identity slots, all twelve POI families, scanning/Codex, docked save/load, leaves and framebuffer checks pass.
- verified-broad/: retained one inherited station-tunnelling failure; radio and steering pass. Broad input/performance groups not completed in this pass. No whole-game green claim.

Verification correction: earlier reused output folders could expose an old completed report after its timestamp changed but before new buffered output replaced it. The world runner now refuses an existing report; all final checks above ran in fresh folders. Disregard earlier candidate/broad reports as final evidence. The compiled boundary diagnostic appears in verified-broad and confirms zero outward displacement and the corrected warning.

Sampled emulator update + draw, excluding audio, display wait and capture IO:

| Planet | Average | Worst |
| --- | --- | --- |
| II | 25.418ms | 26.246ms |
| III | 30.457ms | 32.693ms |
| IV | 40.131ms | 43.265ms |
| I regression | 34.041ms | 36.547ms |

Physical PSP is untested. In particular IV remains slower than a 33.3ms/30-FPS budget in the sampled view; this is a playable visual/content pass, not a locked-30-FPS certification or a 1:1 reproduction of the high-resolution reference art.

## Delivery and files

Isolated build: build.ps1 -BuildDirectory <task>/lave-worlds/build -OutputPath <task>/lave-worlds/candidate/EBOOT.PBP.

Player delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Lave-System-2.5.239/EBOOT.PBP

6,984,015 bytes; SHA256 9C32C94540697FD63A876B1CC990F9F4010ECC5F5E383A56AD733631A81A0F76.

The final rebuild changes the boundary warning to begin with “Field edge”, preserving the existing regression contract while removing the obsolete Triangle-turn instruction, and adds an opt-in smoke-test boundary diagnostic. The fresh broad run confirms the wording fix; no movement rule was relaxed.

Only replace EBOOT.PBP; retain the existing music directories and commander saves. All new artwork is embedded. Save at a station before changing builds. No review flags/test saves belong in the player folder.

Key sources: cloud-platforms.h, cloud-render.h, planet.h, planet-profile.h, game.c, field-sprites.h, field-world.h, fauna-behaviour.h, discovery-atlas.h, surface-activities.h. Test harnesses: lave-world-tests.h, lave-world-review.h; tools/review-lave-worlds.ps1 and tools/lave-world-proof.py.

Shared dirty tree, root/dist binaries, existing saves/music and unrelated edits preserved. No pull/autostash, commit, push, tag or public release. Future priorities: physical PSP stability/performance, IV rendering cost, more directional animal animation, then deliberate rollout to other systems. Do not claim the unused desert/volcanic/coastal kit has shipped on those other worlds.
