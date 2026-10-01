# Spacebook avatars — v2.5.211

Spacebook profile pictures are 20x20 native-pixel procedural portraits generated
from the full displayed username. FNV-1a hashing is case-insensitive, so a named
account retains one visual identity across posts, systems, saves and commander
renames without persisting avatar data.

Ten silhouettes: human, reptilian, insectoid, robot, aquatic, fungal, avian,
furry, crystalline and ship/logo. Eight coordinated palettes combine with eye
colour, eye spacing, expression, hair/appendages, visor/cyber stripe, collar pins
and a small account-status badge. This gives thousands of combinations while
retaining a deliberately chunky Spacebook style at PSP resolution.

The renderer uses only bounded integer hashing and existing rectangle primitives.
There are no textures, allocations, files, save fields or per-frame world updates.
Spacebook draws only the two visible cards. Other menus keep their authored NPC
portraits; the commander profile remains the selected commander portrait.

Tests confirm case-insensitive identity stability and coverage of all ten families
across generated usernames. Native evidence: `spacebook-avatar-gallery.bmp` and
`spacebook-live.bmp` in the v2.5.211 emulator test directory.
