# Comms Panel — 2.5.369

L and R now tune the previous/next radio station from any highlighted console
control. The highlighted control remains selected and its action is not
activated, including when it is unavailable. Tuning wraps across the existing
station catalogue through the same `radio_tune` action as before. Quiet Mode
stays muted; Circle uses the existing preference-save path. Simultaneous L/R
cancel. The tuner still supports X to choose the next station. Conversation
screens retain their existing shoulder-button controls.

Selected controls now have a complete two-pixel accent frame, bright pale
corner brackets, a stronger left accent and a tinted face. The frame stays
inside the existing control bounds, including during the press animation.
Unavailable icons remain crossed out and their actions remain blocked. Ship
paint colours still drive the console. The bottom footer now shows proper PSP
L/R shoulder icons with RADIO beside the existing selection/operate/back hints.

`tools/review-comms-panel.ps1` runs the existing complete console control suite
plus tuning from every selectable button, both directions, wrapping, focus
retention, pause, Quiet Mode, preference persistence, conversation isolation,
disabled actions and focus-frame rendering for all fourteen buttons across all
sixteen paint palettes. Native 480×272 captures include tuner, disabled contact,
HUD, music and mission buttons plus a red-theme camera selection. Command Deck
and normal startup checks run on the same EBOOT, alongside real-MP3 intro
playback and radio regression checks. Final counts, hashes and review
paths are in BUILD-2.5.369.txt and the versioned test-evidence.json.

Elite Exploration now starts 15 seconds into the first startup track, an
additional five seconds beyond the previous ten-second skip. The one-second
fade remains. Saved station, off and volume preferences remain authoritative;
later tracks and restarts do not repeat the skip.

Preserves the Mission Log tracking/ticker fix, menu icons, cinematic, portraits,
Elite Exploration startup treatment and all existing save formats. Physical PSP
testing and inherited broader gameplay/input failures remain open. Full smoke
was not repeated for this focused console change. Next: hardware playtest and
inherited reliability work, then the existing planetary work.

Local release only. Git pull was blocked by existing untracked music placeholders
and restored its autostash; shared work is preserved. No GitHub release published.
