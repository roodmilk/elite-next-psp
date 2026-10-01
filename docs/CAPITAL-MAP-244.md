# Mega-capital city and Field Map — 2.5.244
30 September 2026. Resumed the locally saved mega-city and field-map-244 work following the app restart.

## Included
Mega-capitals now use a deterministic city of 58 solid pieces: eight districts of stepped towers, service viaducts, central docking core and three moving advertising tenders. The city spans at least 16.6km. Streets remain stationary. Perspective-correct facade adverts, windows and depth testing replace the old large hull and floating text signs.

The collision and routing code uses the same solid list as rendering. Capital traffic control can be contacted within 16km of the main hub. Guidance searches a bounded 3D grid, checks swept clearance and guides the ship to the real central aperture; it can stop for an obstruction. Five front docking apertures remain. NPC avoidance and freighter berths account for city structures. Current station classification is unchanged: nine of 256 systems are mega-capitals, including Tibedied.

START on foot or in the rover opens the new Field Map. The chart follows the player, remains north-up, and draws actual terrain, water/cloud-platform topology, airport structures, ship and rover. Walking reveals the current visit's survey mask; discovered site icons use existing IDs and the tracked visible site receives its name. An overview inset and metric scale match the main chart. Triangle opens the current planet's Discovery Codex; Back from that planet root returns to the map. Circle/START closes it. Held buttons are consumed until released when closing or returning, preventing accidental gameplay actions.

All 2.5.243 planetary starports, animal shadows/behaviours, debug fuel/jump toggles, environments, first-person planetary transfers and prior embedded content are retained. Saved city/map folders are preserved. No new save fields, assets to install or player music changes. The survey mask remains visit-scoped, as before.

## Recovery and integration
The saved mega-city EBOOT predates the latest city renderer and copied 243 integration edits. The saved field-map-244 EBOOT includes its latest map changes. Neither separate binary was treated as the final combined game.
Created combined-244 from canonical 243 source/assets, applied the reviewed city differences, then the map differences. The only overlapping implementation file, main.c, contains both sets of hooks. Final canonical src matches the tested combined-244 src ignoring line endings. Version and build labels are 2.5.244. Root/dist binaries and other checkout work were not written. No pull/autostash, commit, push, tag or public release.

## Proof on the final combined EBOOT
Directory: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/combined-244
- PSP SDK build succeeds.
- city-01/mega-city-review.txt: 273 checks passed; RESULT 0. All nine capitals, 108 guided approaches, manual aperture entry, solid district collision, central clear corridor, native views and renderer state/guard checks.
- map-01/field-map-review.txt: 45 checks passed; RESULT 0. Four Lave planets, terrain cache, centred metric projection, Codex categories and Back chain, release safety, dead-state prevention and native framebuffer guards.
- integration-01/integration-241.txt: 47 checks passed; RESULT 0. Existing runtime debug toggles, save exclusion, map lifecycle/release controls and HUD.
- pilot-01/cinematic-port-review.txt: RESULT 0. Arrival, auto-disembark, seat transfers, launch, button gates and sampled 64-world landmark clearance.
- port-01: layout and native airport checks RESULT 0.
- shadows-01/fauna-shadow-review.txt: RESULT 0.
- worlds-01: Lave II–IV activities, walk/rover access, discovery/reward/save and native views RESULT 0.
- smoke-01/game-check.txt: RESULT 1, the same previously recorded swept-collision station-tunnelling assertion. Broad input/performance groups were not certified. No whole-game green claim.
Actual 480x272 screenshots inspected: capital overview/close facade, Lave I initial map and Lave III explored map. Additional native captures remain in the review folders.

## Budget and remaining checks
The map terrain cache is 36,864 bytes. First opening sampled 85–135ms to build; warm chart rendering about 24ms. No per-frame heap allocation. The city uses a bounded block list, 48KiB facade atlas, ~64KiB depth summary and bounded routing scratch; it reuses the surface depth buffer.
Combined ELF text/data/BSS is 8,399,646 bytes; this does not measure all runtime allocations.
City flight view averages range from 25.7 to 39.0ms, worst sampled 39.5ms, excluding audio/display. Some views exceed a 30FPS budget. The cloud starport also remains expensive. Physical PSP frame pacing/memory is unverified.
City buildings are exterior collision volumes, not new explorable interiors or purchasable capital ships. Planet map covers the existing bounded expedition region; its overview is that region, not a map of an entire spherical planet.
Next: physical PSP city fly-through/auto-dock and map input/readability review; profile expensive close-city/cloud-port views; repair the inherited broad station collision issue independently.

## Player delivery
C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/ELITE-NEXT-2.5.244/EBOOT.PBP
7,186,111 bytes
SHA256: 5A41EA9BCCD0671C745DA497E61A9199F4B5850713ACEE5BA41F8B3D68854A3D

Replace the EBOOT in the existing PSP/GAME folder and keep existing saves/music. No review flags or test saves belong in the install.
