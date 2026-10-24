# Performance pass: 2.5.339

Mission-board and crowded-flight performance improvements, measured on the PSP executable in PPSSPP on 2026-10-09. These measurements are not physical PSP certification.

## Results

| Live flight case | Previous 2.5.338 | 2.5.339 |
| --- | ---: | ---: |
| Lave, fixed camera | 21.52 FPS | 29.97 FPS |
| Lave, rotating camera | 25.24 FPS | 39.52 FPS |
| Gelaed, rotating camera | 21.56 FPS | 41.38 FPS |
| Orerve, rotating camera | 25.57 FPS | 32.12 FPS |
| Gelaed, rotating camera, hidden HUD | 18.31 FPS | 39.48 FPS |

All five live runs pass the existing nominal 30 FPS average gate. Worst live frame in the optimized runs is **66.73 ms**. The build is faster; it is not guaranteed to hold a fixed frame rate in every situation.

| Deterministic workload, excluding vblank | Reference path | Optimized path |
| --- | ---: | ---: |
| Mission destination queries for a board frame | 32.038 ms | 0.055 ms |
| Crowded nearby hulls | 47.460 ms | 28.682 ms |
| Crowded distant hulls | 27.028 ms | 19.849 ms |
| Crowded hulls behind the camera | 27.247 ms | 18.477 ms |
| Crowded third-person view | 46.480 ms | 28.735 ms |
| Rolled multi-planet view | 77.421 ms | 23.391 ms |

Crowd fixtures activate all 48 NPC and 64 debris slots and include six freighters. Simulation costs a further 3.806 ms per tick in this fixture. Each optimized workload fits the 33.42 ms average CPU budget after including that simulation cost. Reference rendering is an in-executable diagnostic comparison, not a separately compiled old executable. Both mission-scroll screen timing rows use the new mission batching; the separately measured query comparison above reproduces the old board-query work.

## Changes and compatibility

- Batch mission-board destination searches for one drawing pass, then invalidate the scope before input. Read planet availability from the same seeded type data as generated bodies, including the tutorial Sol override.
- Cache ten immutable 64x64 planet sheets by seed/type. Both level and rolled drawing preserve their exact artwork. Cache miss generation and system-entry timing still need hardware profiling.
- Cull conservative model bounds and off-screen triangles. Preserve partially visible hulls and near-plane crossings.
- Cache mesh radii and star directions, share model transforms, and avoid painter sorting for opaque hull batches already resolved by the depth buffer.
- Retain luminous small-ship wakes while scaling extra flame effects by screen size and proximity. Additional decorative plumes have a ten-contact budget; simulation and collision remain unchanged.
- Render the already filtered nebula wash with 2x2 writes and avoid the underlying redundant background pass. Stars, planets, ship geometry and UI retain their existing resolution.

## Verification and remaining work

`tools/review-performance.ps1` reports **RESULT 0 failures**:

- All 1,280 galaxy mission offers retain their destinations, validation, risk and rewards.
- All 2,560 planet-availability queries match generated body types.
- Cached level/rolled planet images are pixel-identical; a complete system stays cached.
- Viewport rejection preserves edge and near-plane intersections.
- Five render-plus-simulation workload budget gates pass. Mission scrolling and all six native 480x272 captures are also exercised; mission, close-crowd and planet captures were inspected.

Reports: `outputs/performance-review-20261009-171950-400` and `outputs/soft-sky-performance-20261009-172029-941`. The unchanged broad smoke baseline reports 15 game failures and has inherited input failures. This performance pass does not claim those unrelated systems are certified. Recheck actual PSP frame pacing, MP3 playback contention, cold art caches, large station approaches and extended sessions on hardware.

The final broad run has the same 15 game failures as baseline; radio and steering remain at zero. Both input runs were interrupted by the existing 120-second smoke timeout. The first 403 complete input report lines match exactly; the optimized build reached more of the old suite. Raw failure totals from the incomplete input reports are not comparable, and later input coverage is not certified by this pass.

Version, build titles, changelog, design bibles and the shared handoff are updated. Repository sync was blocked by automatic approval review because the checkout already contains extensive unfinished shared changes. The package is local; no GitHub release was published by this pass.
