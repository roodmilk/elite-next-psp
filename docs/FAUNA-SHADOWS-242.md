# Planetary fauna shadows — 2.5.242

Small three-band soft contact decals beneath the existing discoverable fauna.
They darken existing terrain/deck colour, follow world position and local ground
slope, scale with the same animal-family size, lighten with lift, and fade from
400 to 600m camera depth. Water/void, inactive fauna and behind-camera contacts
are rejected. Draw occurs before the animal sprites. Existing depth is sampled,
never rewritten; nearer geometry occludes the decal. No allocations, new art,
save fields or changes to animal behaviour.

These are cheap contact blobs, not sun-directional silhouette shadow maps.
Native terrain sampling and a small surface-height tolerance avoid z-fighting.

Isolated source/build/proof is in the task workspace `fauna-shadows-242/`.
- `shadows-final/fauna-shadow-review.txt`: 29 checks, zero failures across four
  Lave worlds. Native ground/lift captures inspected; occlusion, empty background,
  inactive/behind-camera cases, unchanged depth and framebuffer guards pass.
- `fauna-final/fauna-review.txt` and `fauna-behaviour.txt`: zero failures.
  Ground animal update+draw 28.848ms avg / 38.764ms worst; flyer 28.692/29.137ms,
  excluding audio/display/capture. Not a physical-PSP or locked-FPS guarantee.
- `integration-final/integration-241.txt`: all 47 map/debug/HUD checks pass.

Delivery: task/ELITE-NEXT-2.5.242/EBOOT.PBP, 7,114,359 bytes;
SHA256 `DBB57B09D9972E92F6B5385239D7C37A64A49FAFE9A83B331EA11E2121A093F2`.
Canonical source receives only these narrow changes. Existing dirty work,
root/dist executables, saves and music preserved; no public release or Git sync
claimed. All cumulative 2.5.241 features retained. Broad 2.5.241 failures remain;
the full suite was not rerun for this rendering-only patch. Next: physical PSP
and steep-slope/very-close fauna shadow visual/performance review.
