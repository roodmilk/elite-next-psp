# Current-ship menu preview

Candidate based on main ac8a581. The existing 218x92 panel at (246,64)
shows the commander ship selected by game.ship using its actual mesh and
the existing ochre player-hull livery. No cosmetic equipment model exists
in this build, so no invented attachment appearance is shown.

The 96-second continuous loop uses a restrained camera arc above the ship.
Two current-system world sprites, a steady tinted distant sun and 22 sparse
stars move with slight parallax. Body sizes stay fixed; no approach, docking,
combat, dialogue or extra labels are implied. High contrast removes the
background wash. The existing preview_time controls animation, including
the existing pause lifecycle.

## Interfaces and ownership

- src/voyage.h: only menu_space_view replaced with an include.
- src/menu-ship-preview.h: standalone camera-space drawing; no Game writes.
- src/main.c: one test include in the existing graphics test block.
- src/menu-preview-tests.h: opt-in captures, timing, sentinel, state and
  geometry tests.
- smoke-test.ps1: -MenuPreview opt-in flag.

Production rendering allocates no heap or per-frame cache. Mesh triangles
use the existing bounded draw queue; no new framebuffer or baked textures.
Caller projection/clip values are restored exactly. The menu calls this with
an empty queue; nonempty queues cause an early return without flushing them.

## Verification and performance

Run build.ps1 with the PSP toolchain, then smoke-test.ps1 -MenuPreview with
the explicit PPSSPP executable. Captures are full native 480x272 frames:
ADDER and ANACONDA at 0, 24, 48 and 72 seconds. Test all seven owned ships,
normal/high contrast, and landed/EVA contexts. The geometry sweep checks
48 camera positions per hull and state, not just cropped pixels.

Matched initial baseline (84 draws): mean 2.5432ms, worst 4.5520ms.
It failed the newly added outside-panel sentinel check. Existing simulation
state restoration passed. Do not describe that instrumented baseline as
all-green.

Final candidate smoke-20260923-010240-046: mean 0.9821ms, worst
1.6530ms; all five smoke groups passed, 57.98 FPS overall. Geometry,
sentinel, Game state, clip restoration and exact loop checks passed.
Candidate emulator gate: below 2ms mean / 4ms worst per preview draw.
These are emulator measurements, not a physical PSP timing claim.

Instrumented ELF baseline: text 1,239,186 / data 4,580 / BSS 276,020 bytes.
Final candidate: text 1,241,682 / data 4,580 / BSS 276,020.
Total increase 2,496 bytes, including additional test code; no data/BSS growth.
Production stack usage is bounded by mesh helpers and the existing queue;
peak runtime stack usage has not been measured.

## Combined-build acceptance

Planetary approved testing Select -> menu -> FLIGHT with nonzero pitch,
hazard and jet velocity. Snapshot Game immediately before/after rendering:
Select itself legitimately clears boost. On the combined build, verify
position, ship position, planet/surface and orientation survive, released R
allows descent, and grounded boarding still works.

Preserve Planetary's view_top/view_bot 28..239 changes and its voyage footer.
Preserve Systems' debug menu and outfitting candidate b80add6. The isolated
candidate does not establish whole-team compatibility. Lead owns the final
combined build/smoke/native/performance/save validation and release.
