# Menu icons — 2.5.367

Seven Command Deck entries now have distinct pixel silhouettes drawn at their
actual 12×12 display size:

| Service | Icon |
| --- | --- |
| Radio & audio | Joined music notes |
| Quit to main menu | Doorway with an outward arrow |
| Mission board | Pinned notices |
| Mission log | Checklist journal |
| Galaxy.Maps | Connected stars |
| Discovery Codex | Open book |
| Local TV | Television with antenna and controls |

The same icons appear on the service buttons, selected service description and
button opening transition. They use the existing ship paint palette and selected
highlight. Transparent pixels leave the button intact; the fixed-size table has
12 rows of 12 pixels plus a terminator for each row. No new asset loading, scaling,
allocation, settings or save format changes are introduced.

The production Command Deck review checks distinct mappings and visible rendered
pixels for all seven entries, checks icon bounds and captures each selected entry
inside its real 480×272 menu. Existing navigation, input, menu animation, pause,
render timing and all sixteen paint palettes across fourteen menu pages remain
part of that review. Normal startup and MP3/intro coexistence are checked on the
same EBOOT. Final counts, hashes and capture locations are in BUILD-2.5.367.txt
and the versioned release's test-evidence.json.

Keep the cinematic, repaired portraits and Elite Exploration ten-second startup
skip / one-second fade from 2.5.366. Replace only EBOOT.PBP; retain player music,
saved commanders and preferences. Physical PSP testing and the previously
documented broader game certification work remain open.

Highest-priority follow-up remains physical PSP playtesting and the inherited
gameplay/input failures, then the existing planetary work. This is a focused UI
update, not whole-game certification. Git pull was blocked by existing untracked
music placeholders; its autostash was restored. Shared changes are preserved and
this release is local only.
