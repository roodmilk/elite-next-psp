# Commander Profile and intro audio — 2.5.370

Removed the decorative dotted scanline layer drawn over the Commander Profile
photo. Ship paint had tinted those dots, producing coloured artifacts over the
face. The photo now displays the same unmodified artwork as the portrait renderer;
the surrounding card retains the ship theme. Saved appearance is not migrated.

SAVE PROFILE and RENAME COMMANDER now use the real eight-pixel font advance for
horizontal centering, and the eight-pixel glyph height for vertical centering.
The existing button bounds, selection, save confirmation and rename editor remain.

Elite Exploration starts 21 seconds into the first startup track, six seconds
beyond 2.5.369. The one-second fade, saved station/off/volume preferences and normal
playback of later tracks remain unchanged. No player music is bundled.

The profile review compares every pixel of the actual on-screen 80×80 photo with
the direct portrait renderer across all ten species, legacy/v2/v3 formats and all
sixteen themes (480 comparisons), plus the eight original integer identities.
It verifies actual label pixels and blank margins in both selection states across
all themes, saved appearance round trips, Save/Rename controls and framebuffer
guards. Native 480×272 captures cover both buttons and legacy photos in red/gold.
The obsolete six-pixel width assertion has been replaced with framebuffer checks.

Final checks also cover Command Deck, Comms Panel, normal boot, real-MP3 startup
skip/fade and radio persistence. Results, exact executable hashes and review paths
are recorded in BUILD-2.5.370.txt and the release test-evidence.json.

Physical PSP testing remains pending. Previously documented broad gameplay/input
failures remain unresolved; focused checks are not whole-game certification.
Git pull remains blocked by untracked music placeholders; its autostash was
restored. This release is local, preserves mixed shared work, and is not on GitHub.
Next: hardware playtest and inherited reliability work, then planetary work.
