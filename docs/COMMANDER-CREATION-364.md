# Commander Creation — 2.5.364

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
`BUILD-2.5.364.txt` after verification. Physical PSP testing remains pending.
The unrelated broad smoke baseline contains 15 known game-check failures;
these are not portrait failures and this release is not whole-game certification.
