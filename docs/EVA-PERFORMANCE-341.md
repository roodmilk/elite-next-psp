# EVA performance and compact ports - 2.5.341

This is a local private build, not a certified 30-60 FPS release or a GitHub release.

## Changes

- Apron half-width 420m (previously 690m on Lave I and 720m elsewhere): area reduced by 63-66%.
- Lower terminal and control tower, closer service buildings, short roads, twelve edge lamps instead of twenty.
- Shared renderer/collision/map dimensions. Central pad half-size 125 x 220m; two heavy-traffic pads at x +/-335m; garage x -192m. Cobra width, Anaconda length, disembarking and rover exit retain clearance.
- Exact existing terrain geometry and 2x2 per-pixel depth ownership remain. Ground grain beyond 24m shares a bounded 4x4 shading cache. The change affects surface texture sampling, not foliage/HUD resolution, collision or the terrain silhouette. No ground geometry is decimated.
- Tagged Lave vegetation cells reuse overlapping windows; only new/aliased cells regenerate. Uncached generation remains available in the opt-in fixture.
- Opaque Lave foliage precedes ground so hidden ground can fail depth checks. Earth keeps its previous draw order. Sky no longer repaints rows that the ground backdrop owns. Quads transform each of four vertices once.
- Static caches add roughly 144 KiB. No per-frame heap allocation; no controls or save-format changes.

## Final EBOOT

SHA256: 82BD38F8D9E9E35DA10FD5835673D1C25496B7F4B7FE147D13787AEC8DF296DF
Build log: build-eva-341-verified.log

## Verification

- outputs/eva-performance-review-20261009-185217-842/eva-performance-review.txt: RESULT 0 failures. Ten native scenes compare old/new rendering inside the same compact layout; every depth value and all pixels outside the world viewport match. Ground texture colours intentionally differ. Walking and turning cross cell boundaries; framebuffer guards remain intact. Seven cold/warm/boundary/alias windows retain exact foliage identities.
- outputs/starport-review-20261009-185232-873/starport-review.txt and starport-layout-tests.txt: RESULT 0 failures. 1,380 active landable worlds have supported foundations and clear berths; every player hull lands/disembarks/reboards; four Lave rover lanes remain clear; all entry/departure camera samples avoid buildings.
- outputs/planet-speed-review-20261009-185237-351/planet-speed-review.txt: RESULT 0 failures. 73,664 cached/uncached scenery comparisons and twelve bridge angles pass.
- outputs/opening-check-20261009-185251-571/opening-check.txt: RESULT 0 failures. Tutorial dialogue, dream, meadow, save/reload, guided actions and free-play transitions pass. Meadow has no missing ground in 320 pitch/pond/boundary views, and 1,296 vegetation edge poses remain bounded.
- Native 480x272 captures inspected: compact Lave port overview, forest edge, close ground and ice wilderness. Terrain remains connected, tree silhouettes and viewport edges remain intact, and HUD is readable. These are in the review directories above.

## Measured limits

All timings are emulator CPU work, excluding display/vblank and ordinary audio playback. They are not measured physical PSP frame rates. A passing correctness fixture does not mean the performance budget passes.

Same-build paired forest-edge rendering: 37.488 -> 30.855 ms (about 21% greater CPU frame throughput). Deep-forest static rendering: 46.593 -> 40.646 ms. Moving tests average 30.533ms at the port, 31.180ms at the forest edge, 26.012ms in moss woodland, 29.957ms on ice, and 23.612-25.703ms in the sampled procedural worlds. Nine of ten moving scenes average under 33.333ms. Deep forest remains 37.435ms mean / 43.264ms worst, so stable 30 FPS is NOT certified.

In the unchanged planet-speed fixture, previous 2.5.340 versus final 2.5.341 remote EVA means are 48.125 -> 41.613ms (forest), 32.140 -> 28.218ms (moss), 7.611 -> 6.839ms (cloud), and 39.077 -> 34.574ms (ice). Evidence baseline: outputs/eva-profile-baseline-20261009-181239/planet-speed-review.txt.

Smaller ports expose more terrain; shrinking alone did not improve frame rate. Cold/high cinematic approaches still exceed budget (cloud first-view spike about 127ms); some approach means increased despite cheaper EVA. Do not advertise locked 30/60 or completed planetary certification.

## Next priorities

1. Profile deep-forest rasterization and cold approach spikes on real PSP, including music, display and cache refill costs.
2. Reduce remaining terrain raster cost while preserving native prop silhouettes, water/collision identity and connected ground.
3. Repair inherited broad regression failures separately; do not weaken assertions to declare the whole game green.
4. Preserve the shared dirty checkout and personal music. Do not autostash/pull/rebase/push/tag all contributors' unfinished work.

Run tools/review-eva-performance.ps1 with explicit Eboot and Emulator paths; Review supports eva-performance-review, starport-review, planet-speed-review, surface-perf-review and opening-check. All flags and generated saves belong in disposable review directories, never the delivered game folder.
Final broad regression: work/smoke-20261009-185352-748. Game checks report 15 failures; all failure lines match the 2.5.340 baseline exactly. The compact-terminal walking check now passes against the actual building dimensions. Steering and radio both report RESULT 0 failures. Input checks do not complete within the inherited 120-second limit, and no broad performance report is produced. Full-game certification remains open; do not claim all regression groups pass.

The final EBOOT's ten native EVA captures are byte-identical to the visually inspected renderer candidate. Final artifact hash was verified again after copying to dist.
