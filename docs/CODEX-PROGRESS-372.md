# Discovery Codex progress — 2.5.372

The System Record bar now keeps its coloured interior visible. Previously a solid
rectangle intended as the frame was drawn after the fill, covering every filled
pixel. The frame now draws first, then the dark recess and ship-themed signal fill.
The track retains its native 196×5 footprint and fills proportionally from 0–100%.

Every reached ten-percent checkpoint arrow lights in the signal colour, whether
or not its reward has been claimed. Claimed checkpoints remain gold; future ones
stay dim. Exploration calculations, rewards, controls and save formats are unchanged.

Native tests cover every percentage from 0 through 100 in all sixteen ship themes,
empty/half/full tracks, the frame, all reached/claimed/future arrow states, clamping,
framebuffer guards and actual Codex dossier renders for charted/explored/completed
systems. Existing dossier reward, save persistence, Command Deck and normal boot
checks run on the same executable. Native 480×272 captures are inspected.
Exact test counts, paths and EBOOT hashes are in BUILD-2.5.372.txt.

The 2.5.371 menu/cargo fixes, clean Commander Profile and 21-second startup music
skip are preserved. Physical PSP testing remains pending; inherited broader
gameplay/input failures remain unresolved. This is a focused local release.
Git pull was blocked by existing untracked music placeholders and restored its
autostash; mixed shared work is preserved. GitHub is not updated. Next: PSP playtest,
inherited reliability work, then planetary work.
