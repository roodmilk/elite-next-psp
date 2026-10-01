# Lave I walkabout — local 2.5.236 candidate

## Verified status (supersedes the 2.5.235 notes below)

The 1666×944 concept is an art-direction target, not an in-game screenshot.
The game outputs 480×272 using a software 3D rasterizer. Do not claim an exact
match, seamless performance, or physical PSP certification from a build alone.
Native review of 2.5.235 found mostly bare ground and missing/sliding vegetation.

### Implemented and inspected

- Corrected vegetation projection: the old call passed world X as screen X.
  Upright pixel billboards now use perspective-correct row depth when looking
  up/down, share the ground/building depth buffer, and no longer shrink to a
  fixed maximum size when approached.
- Corrected the source atlas crop so all four trees retain their roots and the
  four understory sprites no longer contain fragments of trees above them.
  Source art is AI-generated, then cropped and palette-quantized by the bake;
  it is not hand-drawn artwork. Eight 48×64 indexed sprites use 24 KiB plus a
  32-colour palette. The original source image stays in assets/source.
- Lave I has a green island palette, rolling walkable ground, an observatory
  hill, world-anchored grass detail and a curved dirt path drawn into the ground
  material. Trees, ferns, flowers, shrubs and rocks remain at seeded positions.
- Replaced Lave's overlapping coarse/fine ground and intermediate depth clear
  with one 160 m fixed lattice. Collision samples the exact same triangles.
  Texture detail is independent of this low-poly geometry; no camera-following
  ground noise or floating path polygons. Existing ocean boundary remains.
- Moved the guaranteed observatory into the actual surface_poi function;
  the previous site_xz override affected a different, unused site identifier.
  Landmark 3 is now 1,280 m north of the pad and reachable through the existing
  navigation, point-and-click scene and investigation systems.
- Lave observatories have a polygonal hemispherical dome, window band and mast.
  Collision bounds are shared with navigation and interaction. Other planet
  observatory models and non-Lave terrain profiles are preserved.
- Mountain noise now wraps correctly through a full camera revolution. These
  distant ridges remain a sky backdrop, not newly accessible terrain. This
  continuity fix applies to the shared ridge renderer on other worlds too.
- Reduced cost through opaque sprite-row spans, early depth rejection, native
  two-pixel sprite sampling, front-to-back vegetation, cached placements/heights,
  bounded distant woodland, far dome detail reduction and offscreen culling.

### Test/review workflow

Build into a disposable directory using build.ps1 -BuildDirectory and -OutputPath.
Place lave-review.flag beside its EBOOT, then run tools/review-lave.ps1 -Folder
with that directory. Never distribute review flags with the playable EBOOT.
The harness uses the game's real renderer/movement and writes native BMPs plus
lave-review.txt. tools/lave-proof.py packages captures into PNGs and a sampled
walk GIF. The GIF is a simulation-time preview, not a real-time FPS recording.

Focused checks cover 202 walking/rover path positions, slope safety, observatory
identity/navigation/perimeter interaction, full-turn mountain continuity, camera
turns/pitch, and framebuffer guard words. Captures include noon, dusk and night.
Performance is measured separately from screenshot writing; update + render
timings omit audio, real controller handling and display wait and are not a
physical PSP framerate guarantee. See the final review report for measurements.

Proof/build folders under the current task workspace:

- lave-motion/baseline: unmodified 2.5.235 rendering plus the capture harness.
- lave-motion/candidate through candidate6: staged visual/performance iteration.
- lave-motion/final-review: final-version focused checks and native captures.
- Lave-I-2.5.236/EBOOT.PBP: playable local candidate without review flags.
- lave-motion/baseline-smoke and candidate-smoke: broad regression logs.

Baseline draw cost was 41.680 ms average, 49.184 ms worst. The first dense
candidate regressed to 119.139 ms; it was not shipped. Candidate6 measured
31.009 ms average / 34.947 ms worst on the same sampled route. This is not an
all-views locked-30-FPS claim. Broad shared-tree smoke failures are separate
from the focused scene checks; do not describe the entire game as bug-free.

Final 2.5.236 focused report: RESULT 0 failures; draw 31.548 ms average /
34.928 ms worst; update + draw 31.600 ms average / 34.946 ms worst. The final
shoreline material samples the same 40 m shoreline cells as collision even
though the underlying rolling mesh is coarser.

Broad regression results: the 2.5.235 baseline had two game failures (station
tunnelling and the outdated no-observatory-on-Lave-I assertion), plus 182 input
failures. After updating the observatory and flat-island assertions to test the
new design, final-smoke/game-check.txt has only the inherited station tunnelling
failure. Radio and steering pass. The long candidate input run was stopped at
145 seconds in the campaign speech region; its 139 recorded failures all occur
in the baseline failure list. This is incomplete input coverage, not a clean
full-suite result. The final game-only confirmation intentionally stops after
the game report. No user saves or music were touched; spawned emulators closed.

Final EBOOT SHA256:
55E8E50928A5B1182588ED9FCD960812EF4DACE01DEF6FEB0A02F842C7CC3046.

### Remaining work

1. Profile full-frame cost on physical PSP, especially close trees, port traffic,
   rover speed and near buildings; keep a worst-case budget, not just averages.
2. Close the visual gap: richer ground textures/flower clusters, less repetitive
   vegetation silhouettes, better local wildlife and richer building surfaces.
3. Add more route/collision/night captures and sustained play/save/reload checks.
4. Resolve inherited whole-game regression failures independently before release.
5. Other planet biomes need their own authored kits; this work is Lave I only.

No save-format change, input remap, music replacement, automatic deployment,
GitHub release, or overwrite of shared root/dist EBOOTs is part of this candidate.

## Historical 2.5.235 notes (unverified at the time)

## Changes

- Lave I remains an ocean world, but its landing island gains gentle, seeded rolling ground between the landing apron and coast. The authored terrain fades back to level water at the existing shoreline.
- The Sky Observatory is guaranteed as landmark 3 at a stable location roughly 1.28 km from the spaceport. Its existing navigation, interaction and expedition activity remain in use.
- Added eight AI-generated transparent pixel-art tree/fern/flower/shrub/rock sprites, stored as source art and baked to a compact 32-colour indexed atlas. Rendering faults in this first pass were found and corrected in 2.5.236 above.
- The same deterministic tree-cell function drives the vegetation placements and EVA/rover trunk collision. Port/building clearances remain intact.
- The layered ridgeline treatment is keyed to world direction and seed so it turns with the player without becoming a screen-fixed backdrop. Existing systems/planet types retain their previous rendering.

## Changed files

- `src/game.c` — Lave I terrain shaping, observatory location, welcome line, and shared tree collision.
- `src/planet-profile.h` — guaranteed observatory identity and stable shared vegetation cells.
- `src/planet.h` — Lave-only lush rendering, ridge layers, ground trail and pixel-art sprites.
- `assets/source/lave-landscape/vegetation-atlas.png` — source pixel art.
- `tools/bake-lave-landscape.ps1`, `src/generated/lave-landscape-pixels.h`, `assets/generated/lave-landscape/vegetation-atlas.png` — repeatable 48×64, 32-colour art bake and preview.

## Validation and blockers

- The art bake completed successfully and produced eight indexed sprites (24,576 indexed texels plus the shared palette; generated header: 83,994 bytes; preview: `assets/generated/lave-landscape/vegetation-atlas.png`).
- PSP EBOOT compilation and packaging succeed. Automated tests and emulator/runtime checks were not run in this pass. Native 480×272 composition and performance inspection remain the immediate verification steps.
- Runtime blockers were not assessed in that pass. Lave I is still bounded by the existing expedition-field radius and ocean shoreline; expanding the explorable region is separate work.

## Next steps

1. Build the PSP EBOOT and inspect Lave I on PPSSPP at native resolution, including observatory approach, horizon, collision and the arrival view.
2. Tune sprite density/scale, hill amplitude, path visibility and frame cost from that capture.
3. Add more Lave-specific flora/fauna variants and authored encounter details after confirming this rendering slice.
4. Keep other procedural worlds on the shared terrain/POI framework and author their visual identity as separate biome profiles rather than copying Lave's palette.
