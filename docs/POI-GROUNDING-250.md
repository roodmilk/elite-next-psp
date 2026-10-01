# 2.5.250 — Biome-grounded planetary POIs

30 September 2026. The user reported that every planetary point of interest sat on a grey stand and did not blend with the planet.

## Change

The shared full-height grey rectangular plinth was removed. Every landmark now establishes a flat structural grade from the highest of five real terrain samples, then joins that grade to four expanded terrain corners with shallow sloped earth/rock faces. Foundation colours are derived deterministically from the current body's colour and ocean, forest, desert, ice or volcanic biome.

Engineered caches, observatories, rescue camps, gardens, vents, migration posts, archives and weather arrays retain a narrow inset footing blended mostly toward their local ground. Ruins, fossil beds, wrecks and crystal grottos use only the natural berm, so they emerge from the landscape rather than a manufactured pad.

The existing FieldBuilding footprint remains authoritative for collision, POI reservations and interaction. Activity IDs, scanner/Codex identity and commander save layout are unchanged.

## Verification

Exact delivered binary reports in task/landmark-grounding-250:

- art-250-exact/field-art-review.txt: RESULT0. Covers1024 worlds,8192 species identities, all12 real POI families at two native angles, visible-perimeter interaction, scan/Codex/save, leaves/wildlife depth and framebuffer guards. Explicit checks confirm varied biome foundation colours and exactly four natural-ground site families.
- pilot-250-exact/cinematic-port-review.txt: RESULT0, including64-world transfer-path clearance.
- map-250 and smoke-250-exact: map, game594, radio48 and steering18 all RESULT0.
- rich-250/rich-station.txt:1568 passes, RESULT0, preserving all2.5.248 station work.
- perf-250-final2/surface-perf-review.txt:144 frames across16 worlds match reference pixels exactly, RESULT0. Lave paired render39.058ms reference /36.899ms fast average.

An initial comparison found three deterministic4–8pixel differences at procedural-world viewport edges. They came from broad box rejection inherited from249, exposed by the newly graded geometry. Increasing its margin was insufficient. Broad rejection is now limited to the profiled Lave authored layout; procedural landmark pieces are preserved. This is a visual-correctness choice, not a reduced-detail workaround.

Native captures were inspected for natural ruins, an observatory, garden and industrial site. Physical PSP appearance/performance remains untested.

## Delivery

task/ELITE-NEXT-2.5.250/EBOOT.PBP,7,321,487 bytes.

SHA256: 4E0A37BFD64A5A3797A470B7915030345F12B3C56357217023912796E3E031DF

Replace only EBOOT.PBP. Keep saves, radio.cfg and music. Embedded art requires no additional files.

Canonical touched files and prior binaries are recoverable from task/landmark-grounding-250/canonical-before-250. No public release or repository sync was performed.
