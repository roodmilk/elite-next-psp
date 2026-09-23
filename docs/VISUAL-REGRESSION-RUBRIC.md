# ELITE: NEXT visual regression rubric

**Scope:** the procedural planet, station, room, wayfinding, ship-preview and targeting overhaul.

**Runtime target:** PSP 480×272, native 1× raster art, nearest-neighbour sampling, fixed-size pools.

This rubric assumes the underlying bars, stations, planets, FX, ships, UI and audio
features already exist. Its job is to measure before/after polish, feedback,
consistency and reliability; it does not authorize replacing working mechanics or
expanding feature breadth. This is an acceptance rubric, not a release claim. A visual result is never certified
from a stale EBOOT, a different branch, or a screenshot whose source build cannot be
identified. Art-only evidence may pass its offline gates while remaining blocked on
native runtime evidence.

## Result vocabulary

| Result | Meaning |
|---|---|
| `PASS` | The check ran against the identified current build and met its contract. |
| `FAIL` | The check ran against the current build and found a concrete defect. |
| `BLOCKED` | Required evidence could not be produced; do not infer success. |
| `NOT RUN` | The check is outside the current handoff scope. |

The overall visual result is `PASS` only when every must-pass gate is `PASS`.
`BLOCKED` takes precedence over an optimistic visual review.

## Evidence identity gate — must pass

Record these values before reviewing pixels:

- branch and commit;
- EBOOT byte length and SHA256;
- asset/content-pack manifest versions and hashes;
- toolchain identity;
- build and capture commands;
- capture directory.

Every `boot-check.txt`, `game-check.txt`, `input-check.txt`,
`steering-check.txt`, `radio-check.txt` and `performance-check.txt` must be in
the same smoke directory as the EBOOT being certified. Reusing a report from a
different EBOOT is a `BLOCKED` result, even if its numbers are better.

Minimum evidence:

```text
EBOOT.PBP
asset/content manifests
boot-check.txt
game-check.txt
input-check.txt
steering-check.txt
radio-check.txt
performance-check.txt
```

Missing current-build reports are reported as `BLOCKED`, not `PASS` or `FAIL`.

## Native 480×272 readability — must pass

Review normal and high-contrast states at 1× native size.

- framebuffer is exactly 480×272; visible geometry never uses the 512-pixel
  stride as screen width;
- text, labels, markers and focal silhouettes remain inside their declared
  rectangles;
- no clipping at x=0/479 or y=0/271;
- no overlap between text, mission markers, interaction anchors, footer or
  reserved HUD bands;
- station room candidates respect the 340×168 room viewport at `(6,20)`;
- planetary surface candidates respect the declared surface viewport;
- normal and high-contrast variants preserve the same geometry and interaction
  meaning;
- nearest-neighbour output has no accidental smoothing or fractional scaling.

Record the asset name, rectangle, state, and the first offending pixel for every
failure. A proof sheet is useful evidence but does not replace a current runtime
capture when the asset is bound to runtime.

## Palette and contrast — must pass

- use only the palette roles declared by the owning art manifest;
- normal mode preserves the established charcoal/navy, slate, olive, rust,
  cream, cyan and amber hierarchy;
- high contrast lifts structure and critical marks without changing layout;
- critical state is readable by silhouette, edge, icon or shape as well as hue;
- bloom, haze, light pulses and decorative particles never obscure text,
  targets, doors or mission markers;
- small labels remain legible without relying on a colour-only distinction.

Report the offending role, RGB value, asset, and state. A non-canonical colour
or an unreadable high-contrast state is a `FAIL`, even when dimensions pass.

## Clipping, overlap and anchor parity — must pass

For each Reorte, Qube and Xequerin representative, verify:

- visual anchors stay inside the native room rectangle;
- existing hotspot rectangles remain authoritative;
- destination labels equal the existing selector vocabulary;
- no duplicate actor, prop, door or ship anchors exist;
- family-specific art does not cover a focal object or interaction target;
- route and ship-return markers fit their reserved rectangles;
- normal and contrast assets use identical anchor coordinates;
- selector version/hash and art-family dispatch agree with the Systems descriptor.

Art must not create a new interaction. Any proposed new hotspot requires a
separate Station/Systems decision and is outside an art-only pass.

## Frame-time gate — must pass for runtime promotion

Use a current-hash PPSSPP report or an equivalent physical-PSP capture. Record
average FPS, worst frame time, slow-frame count, scene identifiers and the
capture mode.

The current smoke contract requires:

- overall average at least 50 FPS;
- planetary flight and EVA scenes at least 24 FPS;
- `RESULT 0 failures` in `performance-check.txt`;
- no visual scene omitted from the declared 42-scene run;
- the dense station, approach and EVA representatives included in the run.

Offline draw-count estimates are planning evidence only. They cannot certify
frame time, GPU cost, or input responsiveness.

## Memory and packed-budget gate — must pass for runtime promotion

Record minimum free memory, largest free block, packed asset bytes, plan bytes,
temporary working memory and runtime allocation count.

- packed assets fit the owning native budget;
- runtime generation performs zero unbounded allocations;
- temporary buffers are bounded and released or reused;
- no asset promotion changes save layout or gameplay RNG;
- collision and hotspot geometry do not come from an unreviewed art payload;
- `performance-check.txt` includes the heap values emitted by the runtime;
- any pending field is `BLOCKED`, not zero-filled or inferred.

The art budget report may prove dimensions and projected packed bytes. It does
not prove frame time or heap safety until the current runtime emits those values.

## Functional visual regression — must pass

The visual pass must be paired with the relevant functional traversal:

| Area | Required traversal |
|---|---|
| Lave opening | Story/flight guide → free commander → station tour |
| Station | Arrivals → room door → named contact → return to deck |
| Missions | Mission board → accept → Mission Log tracking → objective cue |
| Planet | Approach → atmosphere → turn back or land → EVA walk → board |
| Save/recovery | Save, replace, corrupt/missing primary, backup recovery, failed commit |
| Input | D-pad/analog fallback, held/released chords, modal rearming |

The corresponding game/input reports must be current-hash reports. Static source
coverage or a historical smoke bundle is useful review context but is not a
functional acceptance result.

## Offline evidence gate — useful but limited

The following can be certified without PPSSPP:

- deterministic bake repeat and per-asset SHA256 equality;
- native dimensions and canonical palette validation;
- manifest/selector/version consistency;
- 256-system / 768-body profile coverage;
- station/content-pack headers, record counts and checksums;
- room coverage, anchor math and packed-byte projections;
- PSP build success, EBOOT size and current hash.

These checks support an art-only or content-pack handoff. They cannot certify
boot, input, save/recovery, frame time, heap fragmentation, or physical PSP feel.

## Handoff decision

Use this final checklist in the review message:

```text
[ ] current branch/commit/EBOOT hash recorded
[ ] native 480x272 normal capture PASS
[ ] native 480x272 high-contrast capture PASS
[ ] palette/readability PASS
[ ] clipping/overlap/anchor parity PASS
[ ] selector/hotspot ownership PASS
[ ] current-hash boot/game/input reports PASS
[ ] current-hash performance report PASS
[ ] current-hash heap/packed-budget evidence PASS
[ ] save compatibility/recovery PASS
[ ] physical PSP checks separately labelled, if performed
```

If any unchecked item is required by the handoff, label the result
`FAIL_BLOCKED`, list the exact missing file or metric, and keep generic fallback
authoritative. Do not merge, release or widen runtime generation to make a
visual review appear complete.
