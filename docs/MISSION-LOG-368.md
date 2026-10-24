# Mission Log — 2.5.368

The dossier heading now reads **TRACKED MISSION** only when its selected entry's
canonical tracking ID equals `tracked_mission`. Other selected entries read
**MISSION DETAILS**. Hovering never transfers tracking; X still tracks one
entry and Square opens that selected entry's next-step screen. Main missions,
accepted contracts, the station tour and Lave's special entry share the same
comparison as the existing left-hand tracking marker.

Long highlighted names now scroll inside a native 150×8 pixel region on the
left. They pause for one second at each end and move at 24 pixels per second.
The full last word is exposed before returning to the beginning. Short names
stay still; unselected long names use an ellipsis. Changing the selected entry,
reopening the log or changing the title restarts the reading pause. The marker
gutter and panel edge remain separate from the title. Glyphs are clipped per
pixel because the normal text renderer does not support local clipping.

`tools/review-mission-log.ps1` exercises the actual PSP input and rendering with
the real “A SHIP WITH YOUR NAME ON IT” title. It verifies sole tracking across
main/contract/special IDs, hover versus X, Square navigation, full-list selection,
rendered heading changes, ticker motion and pauses, visible final words, resets,
all authored story/Guild title endpoints and untouched framebuffer guards.
Native 480×272 captures include both tracked and untracked dossiers and start,
middle and end positions of the Guild ticker. The Command Deck and normal boot
reviews run on the same candidate. Final evidence is in BUILD-2.5.368.txt and
outputs/releases/2.5.368/test-evidence.json.

No mission progression, controls, save formats or audio behaviour changed.
Preserves the 2.5.367 menu icons, cinematic, repaired portraits and Elite
Exploration ten-second startup skip and one-second fade-in.

Physical PSP testing and previously documented broad failures remain open.
Highest-priority follow-up: hardware playtest, inherited gameplay/input failures,
then existing planetary work. Full smoke was not repeated for this focused UI
change. Git pull was blocked by pre-existing untracked music placeholders and
restored its autostash; unrelated shared work is preserved. Local release only.
