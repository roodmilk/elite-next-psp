# Commander Creation — 2.5.376

The creator has four hairstyle choices (Classic, Cropped, High Sweep, Side
Part) and separate Male/Female controls for Human and Android commanders.
Both genders use existing authored 64px portraits with separately traced
material masks, eye/mouth sockets and ear direction. Hairstyles reshape the
authored scalp clusters; they do not switch the face or place a hair rectangle
over it. Unsupported controls remain hidden for the other species.

The twelve existing hair colours gain four intentional shading tones for the
new hairstyles and female rigs: roots, body, lit locks and glints. Blonde,
brown and blue colour the actual scalp mask. Forehead, visible ears, neck and
uniform retain their own material. Classic male rigs retain their original
palette to preserve saved appearance exactly.

Human and Android features remain independent: 2 head shapes, 12 skin/shell
tones, 12 hair colours, 5 eye styles, 6 outfit colours, 3 cheek marks, 3 mouth
styles and 3 ear/sensor styles. Wide and Angled join Natural, Narrow and
Luminous eyes for every rig with authored eyes; Crystal remains without this
control. Female iris colouring stays within the actual iris. Female freckles
are placed on the cheek and checked at dialogue size. All eleven Human/Android
controls fit on one 480×272 editor page in the real 8px game font. Crown padding
keeps enlarged hair clear of the portrait frame; the save reminder sits below
the last control.

## Appearance persistence

The save struct and its version remain unchanged. Expanded appearance is still
a four-byte field. A previously invalid version-zero/bit-30 combination tags
semantic DNA version 4; earlier v0/v1/v2/v3 encodings retain their original
meaning and renderer. Bits 26–27 are zero in this encoding.

| Field | Bit allocation |
|---|---|
| Species | 0–3 |
| Head | 4 |
| Skin/shell | 5–8 |
| Hair colour | 9–12 |
| Eyes | 13–15 |
| Outfit | 16–18 |
| Cheek marks | 19–20 |
| Mouth | 21–22 |
| Ears/sensors | 23–24 |
| Hair style | 25 and 28 |
| Gender | 29 |
| Expanded tag | 30 |
| Portrait marker | 31 |

The validator rejects out-of-range fields. Old saved portraits are not
randomised or rewritten on load. Opening a parts-based creator upgrades the
encoding with the same selected values and pixel-identical Classic male art.
Inactive selections survive switching species. Old whole-face v0/v1 art stays
read-only until the player explicitly requests a new look. New Commander and
Random use the expanded format. Existing NPC identities are unchanged.

## Verification

`tools/verify-portrait-expansion.py` compiles against the production C renderer
through `tools/portrait-host.c`. It checks all choice pairs at 20/32/96 pixels
in two mixed contexts, both genders, every species, field isolation, 300
appearance migrations, 2,304 mixed gender/style/skin/hair combinations and
independently traced female scalp, cheek, ear and neck landmarks. Background
changes cannot satisfy the foreground requirement. The current report records
17,993 checks and 9,774 render cases with zero failures.

The previous creator's 330,912 combinations were rerendered at all three
sizes: 992,736 renders, no structural failures or identical portraits. Its
full DNA/render-hash record file is byte-identical to the previous verified
record. This is automated coverage, not a claim of manually inspecting every
combination. The final crown-padding and editor-spacing adjustments apply only
to the expanded styles/editor and leave those earlier render paths unchanged.

The opt-in PSP portrait suite covers actual D-pad actions, every expanded
choice at native portrait sizes, unsupported choices, strict validation,
random coverage, real V32 saves, old appearance compatibility, bounds and
framebuffer guards. The profile suite compares direct portraits against the
actual Commander Profile photo in all sixteen UI themes, including both
expanded genders/styles, and verifies saves and centered buttons. Opening,
Command Deck, normal startup and a core baseline comparison are also required
before packaging. Actual native screenshots must be visually inspected; see
the release's test-evidence.json and BUILD-2.5.376.txt for final results.

Physical PSP testing remains pending. The broad game-core suite has previously
documented failures; this portrait work does not claim a complete game-wide
bug-free certification.
