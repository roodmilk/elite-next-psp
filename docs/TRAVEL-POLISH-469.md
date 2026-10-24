# 2.5.469 travel polish — local PSP-3000 playtest

This continues the current shared game tree; it is not a restart or stable release.
The user's card initially contained EBOOT 2.5.465, SHA256 02ad0b1d5ba0abe219ed39ee2ae6a26e819e771d17271e92ed41a635ec084a88.

## Implemented

- Three approved mega showrooms: Royal Observatory, Orbital Foundry and Celestial Palace. Deterministic primary-dealer rotation and secondary-hub offset spread the styles; the native full-galaxy identity check finds 10 of each. Actual existing hull meshes, panel textures, planet sheets, paint, prices and stock are used. No extra ships or dealers are invented.
- Mega catalogue shows eight rows, regular catalogue seven. Both show all six actual module capacities before buying. Mega specs have coloured meters, current-ship markers, comparison arrows, six numbered module tiles and capacity pips. Square retains complete descriptions; native captures cover every purchasable hull.
- Continue station bug fixed: MC_BOX had no shared preview geometry, so ordinary sections were omitted. Its eight vertices and six faces now exist. The preview uses explicit local UVs and the shared authored station atlas, isolated renderer state, native one-pixel silhouettes, closer framing, gentle camera motion and an occluded background sun. Existing load/deck timings and release gates remain.
- Far traffic has persistent shimmering lights, a separate navigation blink, bounded tapered exhaust and aspect glints from real NPC state. Weapon hardpoints/impacts have bounded soft light. Planet transitions add palette-matched cloud banks without weakening the opaque handoff.
- Scenic docking may bend past an actual advertising facade only when both swept legs are clear. Longer paths get compensated cruise speed; the final aperture speed remains unchanged. 24 real game-tick comparisons complete, two use scenic routes, none take longer than baseline in this fixture.
- Booked ERL events no longer block intersystem travel. Their home championship, rounds and rewards remain. Actual active races still cannot jump. Race entry captures current post-travel ship resources; cancellation before entry does not restore stale fuel/hull. Cancelling remotely does not teleport or dock the ship for free. Victory/normal result return remains on its existing path.
- PSP-3000 MP3 input now uses two 128-KiB asynchronous read-ahead chunks, below the mixer/device priority and above rendering. The existing 48-KiB decoder reservoir, PCM queue, sample rate, mixer, engine sounds and SFX remain. Older memory profiles keep the original stream path. Explicit EOF advances the playlist once.
- Exact sun palettes/radial thresholds move work out of the native pixel loop. Atmospheres visit only annulus spans instead of scanning the empty centre. Both retain exact tested pixels; no framebuffer resolution reduction was added.

## Music on the card

Elite Exploration contains one 36:44.58, 44.1-kHz stereo 320-kbps MP3 (88,183,118 bytes).
The complete recording converted cleanly to 44.1-kHz stereo CBR128 MP3, metadata-free, 35,273,665 bytes: approximately60% less card bandwidth.
The converted file also passed a full independent PC decode without errors.
It has been installed on the card with hash verification. Original backup:
`D:/PSP/GAME/ELITEA/music-backup-20261017/Elite Exploration/EliteDangerous - Frameshift & StarportsOST.mp3`.
A private workspace backup and install receipts remain under outputs/music469; music and commander saves must never enter Git.
Track length does not require loading the whole recording. No arbitrary cuts or playlist reshuffling were introduced.

## Verification and limits

- Native framebuffer guards, opaque cloud handoff and distant-light occlusion pass.
- Six Continue cameras render identical pixels with clean versus deliberately polluted depth/material state, restore renderer state and leave Game unchanged.
- 32 sun comparisons and48 atmosphere comparisons preserve exact native pixels.
- Both complete compressed music files pass byte-for-byte native read-ahead and12 random-seek checks. Native opening decode produces identical PCM for original streaming versus read-ahead; converted MP3 decodes normally. No native read errors observed. This is not a full37-minute physical-device listening test.
- A private copy of the user's actual commander loads and round-trips credits, ship/system/story, cargo, fittings, spare modules and complete ERL progression.
- Actual race entry/finish and remote-cancellation checks preserve resources, location and active-race rules.
- Native 480x272 dealer views, all hull descriptions and145 Continue frames are in outputs/travel-polish469-styles/final-verified. The lossless gallery video is a staged camera study sampled at30fps; it does not measure live FPS.
- ALL PC sound tests remain muted, with PSP audio processing enabled and every host output volume0.
- Busy-view performance is still unfinished. The earlier combined charging fixture was about39ms, and the large-sun view about38ms, before the final atmosphere-span optimization. No locked30/60FPS claim. Physical PSP pacing/listening, exhaustive docking/suspend/playlist switching and the broader release gate remain open. The wider prior suite had18 existing failures; this pass does not claim those were resolved or rerun to green.

## Recovery / integration

Canonical current source is in src. Authored baseline files and integration469.patch protect this pass independently of the large pre-existing dirty shared tree. SOURCE-SHA256.json identifies exact touched files. Do not blanket revert, stage unrelated changes or apply the patch over conflicting newer edits. The protected baseline plus patch recovers these changes without private music/saves.

The playable EBOOT is milestones/2.5.469-travel-polish/EBOOT.PBP. Exact retail checks and final hash belong in that milestone's README/verification logs. Do not distribute outputs/*/CANDIDATE.PBP: those automatically run diagnostics and exit.
Stable VERSION/dist/GitHub release remain unchanged until the full release gate passes. No remote-sync or physical-PSP certification is claimed.
