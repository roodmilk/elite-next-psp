# 2.5.248 — Rich retro-future habitat stations

30 September 2026. User supplied wheel/globe/axial habitats, domed gardens, saucers, pyramids, towers and linked industrial pods as architectural references, requesting a clearly intermediate Rich class and many procedural templates.

## Implemented

All 174 Rich main stations now use sixteen deterministic middle-tier layouts, distinct from the eight existing compact Poor families and four Mega Capital crowns. Class allocation remains 73 Poor / 174 Rich / 9 Mega. Poor ports, auxiliary relay/outpost geometry, capital cities, stable targeting IDs, saved commander fields and prior planetary/map/debug/cinematic work are retained.

Rich templates: globe wheel, botanical saucer, double axial wheel, linked globes, rocket cathedral, asymmetric liner, three-tower palace, stacked pleasure saucers, globe-shoulder wheel, bridged garden bowls, axial habitat drum, pyramid campus, wheel palace, twin garden saucers, tiered tower metropolis and industrial cruise port.

System hash selects layout, 460–620m docking-core radius, 350–500m depth, wheel proportions and independently seeded service modules. Three to five pod clusters vary side, position, dimensions and pod/globe shape. Attached billboards, stout masts and solar/radiator plates use the same bounded solid list. Garden domes use blue-green glazing/canopy patterns; hulls have broad deck bands, lit windows and panel seams; two mounted animated advertisements and blue segmented panels are visible at native resolution. No high-resolution reference-image fidelity claim: these are faceted PSP geometry and procedural facade pixels, not the attached paintings.

Actual longest physical dimension across the 174 Rich hulls: 2,812–4,566m. This is visibly above compact Poor ports and below the capital cities' >=16.6km horizontal spans. One common docking slit retained. Rich spin reduced to 38% of previous speed. Rich navigation lattice uses 500m cells, sharing exactly the generated solids with renderer/collision. Comms range 8km standard / 12km docking computer, so the equipment upgrade remains useful; auxiliaries keep 2.5km / 8km, capitals 16km. Out-of-range messages state the actual selected station's range.

## Verification on the exact delivered binary

Fresh disposable PPSSPP folders in task/rich-stations-248:

| Report | Passes | Result |
| --- | ---: | --- |
| rich-final/rich-station.txt | 1568 | 0 failures |
| pulp-final/pulp-station-review.txt | 1500 | 0 failures |
| capital-final/mega-city-review.txt | 293 | 0 failures |
| identity-final/station-audit.txt | 3088 | 0 failures |
| map-final/field-map-review.txt | 45 | 0 failures |
| integration-final/integration-241.txt | 47 | 0 failures |
| smoke-final/game-check.txt | 594 | 0 failures |
| smoke-final/radio-check.txt | 48 | 0 failures |
| smoke-final/steering-check.txt | 18 | 0 failures |

Dedicated audit covers all 174 Rich seeds, all sixteen layouts, exact convex vertex bounds, physical module/sign/panel collision, forward clearance, revisit stability and 696 front/left/right/upper guided approaches while rotating. Existing ordinary guidance adds 494 routes and capitals 108. Identity review preserves all three independently targetable hubs across all 256 systems. Native Rich catalogue: two views per family, rich-final/rich-family-00-view-0.bmp through rich-family-15-view-1.bmp; representative final views and all preliminary family silhouettes inspected.

Initial broad diagnostic had 14 failures after scale/range changes. Several checks used obsolete fixed entry/inside-hull positions (Lave z=3200/3400/4300) and old comms distances, causing downstream docking/save failures. Tests now use the actual generated entry plane, rotated local aperture and out-of-range distance; rear starts are beyond the larger hull and the longer route allowance is bounded. Collision requirements were not disabled or weakened. Poor wheel-gap checks retained only for their original layouts; new Rich structural/guidance audit separately covers Rich layouts. Final broad game report passes; not a claim that all inherited input defects are repaired.

## Performance and limits

32 native overview/oblique samples: 26.425–36.395ms average, 37.013ms worst, excluding audio/display. Some exceed the 33.3ms frame budget. No locked-30FPS, close-view, physical-PSP or complete inherited input/performance certification. Existing large-capital performance limits remain. Hardware review should include Rich close fly-bys, loading older saves near changed hulls, clear docking/launch paths and crowded traffic. Miniature Almanac art is unchanged and should later match the new physical templates.

## Delivery / preservation

C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/ELITE-NEXT-2.5.248/EBOOT.PBP

7,287,023 bytes. SHA256 23F8FC2D384A50447269097C7F243A7CF3216366D2440A9877E44FF921651260.

All geometry/facade assets embedded. Replace only EBOOT in the player's existing PSP game folder; retain saves, radio.cfg and music. Tested isolated snapshot narrowly backported after checking all touched canonical files against the frozen 247 baseline. Root/dist builds, unrelated dirty work and recovery snapshots preserved. No pull/autostash, commit/push/tag or public release.

Next: physical PSP review and Rich close-view profiling, rendering optimisation where needed, then miniature almanac matching and richer auxiliary berth architecture.
