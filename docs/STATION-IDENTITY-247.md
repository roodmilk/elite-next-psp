# 2.5.247 — Physical station identities across the galaxy

30 September 2026. User reported a tiny station in an unidentified far-right Mega Capital system, then clarified they were not using the new 2.5.246 EBOOT. That exact historical observation cannot be reproduced without the system/build. Independently confirmed a current identity bug: target zero followed the nearest hub, while its name came from the last docking context. A compact auxiliary could be labelled as the main Mega Capital; selecting the main station in the Almanac could turn toward that auxiliary.

## Changes

- Target zero now always identifies the system's physical main station. Two new runtime IDs identify its Outer Relay and Frontier Outpost; existing celestial/NPC/debris/rift/route IDs and save layouts remain unchanged.
- STATIONS scanning, cycling, contact lists, target details, flight HUD and radar consistently include the three physical hubs. Enlarged bounded lists accommodate the maximum 125 contacts.
- Hailing or requesting auto-dock uses the selected station, not a silently substituted nearest hub. Out-of-range requests are refused. Generic backend nearest-hub docking remains available for existing non-target-specific callers.
- The Almanac main-station selection always selects the real primary. Docked station services retain their actual dock context.
- No station-class allocation or architecture was changed in this pass. The cumulative 246 ordinary hulls and capital city crowns remain intact; small auxiliary stations are intentional and now separately named.

## Exact-binary verification

Fresh opt-in PPSSPP folders under the task's station-audit-247 snapshot:

| Report | Passes | Result |
| --- | ---: | --- |
| audit-final/station-audit.txt | 3088 | 0 failures |
| pulp-final/pulp-station-review.txt | 1541 | 0 failures |
| capital-final/mega-city-review.txt | 293 | 0 failures |
| map-final/field-map-review.txt | 45 | 0 failures |
| integration-final/integration-241.txt | 47 | 0 failures |
| smoke-final/game-check.txt | 594 | 0 failures |
| smoke-final/radio-check.txt | 48 | 0 failures |
| smoke-final/steering-check.txt | 18 | 0 failures |

All 256 systems: 73 Poor, 174 Rich, 9 Mega Capital. Every listed class matches the actual main profile/seed/geometry. Capital systems 0,12,17,48,83,156,184,228,236 each have at least 60 city components, five docking ports and an x-span of at least 16.6km. Tests exercise both auxiliaries in every system with stale dock context, stable locks, names/categories, exact-hub docking and wrong-hub refusal. Actual Almanac X, station cycling and canonical Comms hail/X handlers pass. Fully occupied contact lists and framebuffer guards pass.

Native captures of all nine primary capitals inspected, plus the small relay and the three-entry STATIONS list in audit-final. Ordinary and capital guidance regressions cover 494 and 108 routes respectively. Review uses disposable commanders/files, not user saves.

## Delivery and preservation

C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/ELITE-NEXT-2.5.247/EBOOT.PBP

7,249,143 bytes; SHA256 5BA1CC827827A0ED5D9DC47CCF55ECC872EB2499B6E7975A497908751AE707AE.

All generated visual assets embedded. Keep existing music folders and saves when replacing the PSP game's EBOOT. Canonical src matches the tested isolated snapshot; narrow source/build backport applied only after confirming the touched shared files still matched 246. Root/dist binaries, dirty unrelated work, recovery folders and player saves/music preserved. No pull/autostash, commit/push/tag or public release.

## Limits and next steps

Physical PSP validation remains outstanding. Full inherited input/performance suites are not certified; passing focused and broad gameplay reports do not mean every game option is bug-free. Large capital views still have the performance limits recorded in PULP-STATIONS-246.md, and auxiliary hulls remain compact. Next: test this EBOOT on PSP, particularly distinct hub selection and main-city docking; optimise capital rendering and later align miniature Almanac art with the physical hulls.
