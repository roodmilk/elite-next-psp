# Commander Creation — shipped in 2.5.365

## Repair

The old `y >= 51` clothing rule recoloured exposed neck skin. Coarse alien
polygons also included incorrect materials and some eye/mouth sockets were
offset from the authored features. This release replaces that classification
with per-pixel source-art material masks for all ten 64px rigs, including neck,
collar, feathers, antennae, crest, hood, eye and lip boundaries. Human eyelid
skin is separated from the actual iris. No runtime colour-threshold mask is
used. Original shading, transparency and dark outlines remain in the source.

Masks are authored in `tools/bake-portrait-materials.py`, baked into
`src/portrait-material-masks.h`, and reviewed against exact source tile grids.
The existing Human/Android scalp contours are retained. Feature sockets are
traced separately for each species; local resampling keeps the selected skin
and hair materials, rather than painting rectangles over the portrait.

## Choices

| Species | Head | Skin/shell/beak | Hair/crest/hood | Eyes | Uniform | Marks | Mouth | Ears/sensors |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| Human | 2 | 12 | 12 | 3 | 6 | 3 | 3 | 3 |
| Orrith | 1 | 12 | 1 | 3 | 6 | 1 | 1 | 1 |
| Amphibian | 1 | 12 | 1 | 3 | 6 | 1 | 3 | 1 |
| Insectoid | 1 | 12 | 12 | 3 | 6 | 1 | 3 | 1 |
| Sauran | 1 | 12 | 12 | 3 | 6 | 1 | 3 | 1 |
| Avian | 1 | 12 | 12 | 3 | 6 | 1 | 1 | 1 |
| Robot | 1 | 12 | 12 | 3 | 6 | 1 | 3 | 3 |
| Android | 2 | 12 | 12 | 3 | 6 | 3 | 3 | 3 |
| Masked | 1 | 12 | 12 | 3 | 6 | 1 | 3 | 1 |
| Crystal | 1 | 12 | 12 | 1 | 6 | 1 | 1 | 1 |

A count of one means the control is hidden, rather than advertising anatomy
the source rig does not support. Background roulette remains hidden.

New palettes: olive/bronze/umber/porcelain skin; emerald/amber/pearl/magenta
shell or beak; black/pink/mint/auburn hair or crest; indigo/ivory uniforms.
Existing colours retain their field values. Additional styles: slender head,
luminous iris optics, smirk, pointed ears/angled robot sensors and freckles.
Slender heads transform the assembled head coherently and taper to the
unchanged neck and clothing. Marks are limited to the supported cheek skin.

The visible choices yield **330,912 selectable combinations**. The previous
v2 creator has another **14,272 combinations** checked for compatibility.

## Save and input contract

New looks use DNA v3 within the existing 32-bit V32 portrait field. Bits 19–21,
previously an unused background control in v2, extend the three colour fields.
Species and existing feature indices retain their meanings. The validator
checks every field, and changing one control preserves all other selections.
Inactive selections are retained when switching species.
Adjusting an inactive control is a no-op, including the retired background
field, so it cannot corrupt the expanded colour bits. Random selects from all
supported choices, including the new palette entries.

V2 records upgrade without changing selected fields or rendered appearance.
Existing v0/v1 saved portraits keep the original renderer and remain read-only
until explicit **Create New Look**. Opening an existing v3 editor does not
normalise away inactive selections. An older EBOOT cannot read new v3 portrait
records; retain the previous save backup if downgrading.

Up/down selects supported controls; left/right changes the selection; Square
randomises a new look; Circle completes editing. The 480×272 editor keeps its
96px preview and 32px dialogue preview, readable captions and bounded rows.

## Verification method

`tools/portrait-host.c` compiles the **production C headers** as a local DLL.
The host harness stubs the framebuffer and unrelated legacy drawing helpers;
it does not reimplement the new renderer in Python. Every selectable complete
combination is rendered at 96, 32 and 20 pixels. Checks cover legal DNA, exact
field values, other-field preservation, part validity, foreground coverage,
output bounds and framebuffer guards. Hash collisions are compared pixel by
pixel before declaring two portraits identical.

The Python audit additionally checks every pair of choices in two contexts at
three sizes, excludes background-only changes, and checks named-part/material
isolation and useful hair-colour contrast. Independent neck and collar
landmarks are checked across all skin/hair/uniform palettes. These coordinates
are separate fixtures, not inferred from the material mask being tested.

Automated exhaustive rendering does **not** establish that every image has
been inspected by a human. Visual review covers every feature sheet, mixed
stress portraits and actual native PSP captures. The standalone PPSSPP review
uses real editor input and real save/load routines, including all ten species,
legacy/v2/v3 records, maximum new colour indices and inactive selections.

Final exact-artifact results and native comparisons are recorded in
`BUILD-2.5.365.txt` after verification. Physical PSP testing remains pending.
The unrelated broad smoke baseline contains 15 known game-check failures;
these are not portrait failures and this release is not whole-game certification.

## Final exact-release evidence


EXHAUSTIVE PORTRAIT AUDIT — included in this exact 2.5.365 executable
All seven portrait/editor/review headers match the frozen 365 source snapshot.
330,912 selectable v3 + 14,272 v2 combinations at 96/32/20px:
1,035,552 production-C renders; zero structural failures or identical v3 looks.
12,162 foreground/material/anatomy feature checks pass.
94 native crops from this exact 365 EBOOT match that C renderer pixel for pixel.
The existing 505 focused checks include 43 portrait/editor/save checks,
2,292 foreground render cases and 50 real V32 save/load round trips.
All option sheets and representative mixed/native portraits visually reviewed;
every combination was automatically checked, not manually inspected.
Neck/collar, hair, eyelids and local anatomy have independent material masks.
Adds 12 skin/shell colours, 12 hair/crest colours, six uniforms and supported
head/eye/mouth/ear/cheek styles. Unsupported controls are hidden.
Legacy looks preserved; v2 migration retains fields and appearance.
New v3 portraits require this or a compatible newer EBOOT; back up saves
before downgrading. Physical PSP testing remains pending.
Native comparisons: portrait-comparisons. Manifest: portrait-test-evidence.json.
Details: docs/COMMANDER-CREATION-365.md.

Additional broad run on the separately reviewed 2.5.364 portrait candidate:
Core completed with the same 15 inherited failures. Radio and steering pass.
The 600-second input run remained incomplete: 130 observed failures.
17 unique failures occur beyond the old truncated baseline (first at line
1581 versus 1260 baseline lines). They remain unclassified, not proven
pre-existing or newly introduced. This is not whole-game certification.
These are not claimed as new broad test results for the 365 executable.
C:\Users\skarm\Documents\Codex\2026-09-14\let\work\smoke-20261010-143904-967
FAIL Wanted poster survives six seconds of actual flight rendering
FAIL Wanted target pursuit accepts laser fire after selection release
FAIL Wanted target pursuit accepts normal double-R boost
FAIL comms settings: HUD setting changes actual flight layout
FAIL comms settings: High contrast focus toggles visibly
FAIL comms settings: Radio returns to its owning screen and row
FAIL comms settings: audio is reachable through visible menus
FAIL comms settings: final option wraps to first
FAIL comms settings: first option wraps to final
FAIL comms settings: third-person view toggles actual camera preference
FAIL controls: Back returns to Comms instead of losing location
FAIL controls: pages wrap and remain bounded
FAIL planet boundary: solid worlds and gas platforms automatically land and disembark
FAIL tutorial: complete authored route awards harbour licence and reaches unrestricted play
FAIL tutorial: lesson 17 completes through input (TOOLS BEFORE HEROICS)
FAIL tutorial: only FLY is visible at the start
FAIL tutorial: script introduces every command-deck service

