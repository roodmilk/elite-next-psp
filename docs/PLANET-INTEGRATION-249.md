# 2.5.249 — Cumulative planetary integration

30 September 2026. User explicitly requested merging the side-conversation planetary work into the completed 2.5.248 Rich-station build and receiving EBOOT.

## Included and preserved

- All cumulative 248 source and embedded art: sixteen Rich station families across 174 systems, 73 Poor stations, nine Mega Capital cities, stable primary/relay/outpost targeting, docking routes and prior gameplay.
- Moving inward near an eligible planet automatically starts an opaque cloud handoff to the actual-world first-person approach, touchdown and automatic disembark. Speed is retained during cloud cover. Outward/tangent/stationary/sun approaches do not force landing; existing landing-equipment/story eligibility is authoritative.
- Faster cloud rendering and skipping covered world/canopy pixels. Solid-colour raster spans and conservative off-screen box rejection keep sampling, depth and scenery unchanged. A duplicate-apron removal experiment was rejected because it changed pixels.
- Sleek low open landing terraces instead of barrel-vault hangars. Shared rendering/collision/map footprints vary by world; Lave I is smaller. Lave I observatory moves to1540m along its trail and onto a160m ridge, with a stronger silhouette.
- Field Map nub panning changes the chart centre, never the commander position. Bounds, markers and cache remain coherent.
- Roamer: R accelerate; L brake, then reverse after a brief stopped delay; nub steer/look; Circle handbrake/drift; R+X rechargeable boost; Triangle park/dismount below12m/s; START map. Hard impacts briefly crack the windshield, then clear automatically. Collision substeps stay <=3m.
- Roamer transient fields are not serialized; commander save layouts remain unchanged. Preserve the player's saves/music/radio.cfg.

## Exact-binary evidence

Reports live in task/combined-249. Build: build-249-final.log.

- rich-exact/rich-station.txt:1568 passes, RESULT0 (174 Rich systems,16 families,696 guidance routes).
- pulp-exact/pulp-station-review.txt:1500 passes, RESULT0.
- capital-exact/mega-city-review.txt:293 passes, RESULT0.
- identity-exact/station-audit.txt:3088 passes, RESULT0 across all256 systems.
- integration-exact/integration-241.txt:47 passes, RESULT0.
- map-exact/field-map-review.txt:71 passes, RESULT0.
- smoke-exact/game-check.txt:594 passes, RESULT0; radio48 and steering18, RESULT0.
- entry-exact/seamless-entry.txt, roamer-exact/roamer-review.txt, pilot-exact/cinematic-port-review.txt, walk-exact/lave-review.txt: RESULT0, including carried-button safety and64-world camera-path clearance.
- perf-exact/surface-perf-review.txt:144 frames across16 worlds, pixel-for-pixel identical fast/reference surface rendering and preserved framebuffer guards, RESULT0.

The initial broad game run had one obsolete assertion expecting zero speed at the planetary boundary. It now checks the preserved200m/s speed while retaining unchanged weapon suppression, energy/heat, missile timer, threat-clock and frozen boundary-position checks. No collision requirement was relaxed. Full inherited input/performance suite is not certified; the smoke review ends after the completed game report.

## Performance limits

Paired Lave render average38.838 ->36.081ms (~7.1% less rendering time), sampled worst72.758 ->71.500ms. Moving Lave I walk30.449ms average/33.228 worst; update+draw30.518/33.286ms. Previous candidate walk32.433/36.871ms. Four Lave cinematic averages36.526/30.746/37.738/33.822ms; worst65.042/55.862/94.633/59.739ms. Audio/display wait and capture IO excluded. These are emulator measurements, not physical PSP FPS; no locked30 claim.

## Delivery and recovery

task/ELITE-NEXT-2.5.249/EBOOT.PBP;7,319,495 bytes.

SHA256: 3E7FE026417B0FACC55D7B73D675566A345DD2520CD61ED22D9C4DAF9B514277

Replace only EBOOT in the existing PSP game folder; visual assets are embedded. Do not copy review flags/test folders. Canonical root and dist EBOOTs contain the exact tested binary.

Original canonical touched source, version/build scripts/docs and prior root/dist binaries are recoverable from task/combined-249/canonical-before-249. Previous ELITE-NEXT-2.5.248 delivery and all other development folders are untouched. Canonical backport was preceded by touched-file hash/version checks. The shared dirty checkout was not pulled/autostashed or committed/pushed/tagged; no public release/sync claim.

Next: physical PSP arrival/vehicle/collision tests; optimise heavy cinematic/cloud-world views; complete inherited input/performance coverage; later match miniature Almanac art to station families.
