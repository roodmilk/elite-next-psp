# Illustrated Controls handbook — local candidate

Implements approved Controls concept 3, the Pilot's Field Guide, on the current 2.5.354 shared source base. At the user's request, the current local EBOOT and dist copy are updated as 2.5.355. This does not certify the full game or publish a GitHub release. The save format, gameplay bindings and unrelated menus are preserved.

## Player experience

Open Controls from the Command Deck or Comms Panel. Five topics contain eight selectable entries each: Flight, Targets, Docking, Menus and On Foot. The left page lists controls with native PSP symbols; the right page immediately shows a small animated instructional drawing and one concise instruction. Each topic remembers its highlighted entry. All forty entries fit without a scrolling page of prose.

- D-pad Up/Down: choose an entry, wrapping at either end.
- L/R shoulders: previous/next topic. D-pad Left/Right also retains its existing topic shortcut.
- Square: toggle analog steering. The footer shows its current ON/OFF state. Re-enabling requires the usual nub centring. This replaces the old help-only L shortcut, freeing both shoulders for topics; no flight binding changes.
- Circle: return to the actual calling screen. Returning to flight retains the existing held-button firing/tool guard.
- Boost instruction: **Double press R + keep it held.**

Reading the guide freezes flight simulation. Its diagrams animate using the existing menu preview clock without moving the player's ship or changing inventory, money, targets or missions. No external assets, alternate fonts, scaled illustrations or save migration are required.

## Native layout

All coordinates are in a 480×272 framebuffer and use the actual `src/font8.h` 8×8 cells.

| Region | x | y | width | height |
|---|---:|---:|---:|---:|
| Navy masthead | 0 | 0 | 480 | 24 |
| Five topic tabs | 8 + 94 × topic | 30 | 88 | 22 |
| Left printed page | 8 | 59 | 184 | 183 |
| Binding | 194 | 65 | 5 | 174 |
| Right printed page | 200 | 59 | 272 | 183 |
| Selected list row | 13 | 88 + 18 × row | 174 | 17 |
| Instructional picture | 208 | 91 | 256 | 94 |
| Instruction copy | 212 | 195 | 240 | 40 |
| Footer | 0 | 248 | 480 | 24 |

Cream paper, brown printed ink, ochre tabs and navy illustration panels follow the selected book concept. Each drawing is generated directly at its displayed pixel size. Controller symbols reuse the game's proper face-button, D-pad, nub, Start/Select and shoulder renderers. The picture animates while the text stays still. Instructions wrap by words, with a checked maximum of 30 characters per line and four lines. Menu notices cannot cover this page.

## Source and verification

- `src/controls-handbook.h`: lesson data, native drawing, word wrapping and Controls input.
- `src/deck-ui.h`: replaces only the old help renderer with the handbook include.
- `src/main.c`: routes HELP input, suppresses unrelated menu notices over HELP and adds the opt-in review hook.
- `src/controls-handbook-review.h`: production-input and native-render checks, enabled only by `controls-handbook-review.flag`.
- `tools/review-controls-handbook.ps1`: disposable emulator runner with separate candidate, baseline/core and full regression modes.

Build without replacing another contributor's release:

```powershell
./build.ps1 -BuildDirectory "$PWD/outputs/controls-handbook-build" -OutputPath "$PWD/outputs/controls-handbook-candidate/EBOOT.PBP"
./tools/review-controls-handbook.ps1
./tools/review-controls-handbook.ps1 -Core
```

The focused review checks all forty copy/label bounds, requested boost wording, topic navigation/wrapping, per-topic selection memory, analog toggling/centring, paused simulation, stable text with animated art, both entry paths and safe return to flight. It writes forty actual 480×272 BMP captures plus two boost animation frames. Final reports, selected PNG previews and EBOOT checksum are recorded beside the candidate in `outputs/controls-handbook-candidate/BUILD.txt`.

Physical PSP verification remains pending. Known inherited broad-game failures must not be described as fixed by this UI change. Review the existing shared handoff before integrating or publishing; do not commit or publish the mixed checkout wholesale. Integrate version/changelog/release bookkeeping with the main thread deliberately.
