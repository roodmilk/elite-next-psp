# Local TV scheduled broadcast (2.5.234)

Supersedes the three selectable channels described in LOCAL-TV-231.md. The approved studio art is unchanged. Lave Local remains CH8 throughout all programmes.

## Clock and schedule

The top-right clock shows HH:MM from the PSP real local clock (sceRtcGetCurrentClockLocalTime), not the simulated planetary day/night clock. NEXT and LATER show matching start times. Three five-minute slots form a continuous fifteen-minute cycle: Evening Orbit at :00/:15/:30/:45, Farmers' Market at :05/:20/:35/:50, Night Stories at :10/:25/:40/:55. Start-time labels wrap correctly at midnight.

Opening the page joins the current programme and caption position. Nothing is restarted by Left/Right or X; these controls and their prompts are removed. Circle returns to Discover. Returning from another menu, sleeping/resuming or changing the PSP clock seeks the current broadcast immediately on its next update. No save data is required for the schedule. If the RTC read fails, the broadcast continues using elapsed time but displays --:-- rather than inventing a real clock reading.

Existing four-passage scripts repeat within each five-minute slot. This does not add five minutes of unique writing per show or unique TV channels for all 256 systems. The current art/scripts remain the Lave pilot. Captions use the existing 28-character/second reveal with six-second reading pauses, positioned deterministically from wall-clock time rather than accumulated frame counts.

## Voice and window

Mira's registered mouth animation runs during text letters and rests during punctuation/reading holds. A small procedural two-oscillator babble voice follows the same active caption letters. Night Stories uses a lower pitch and slower cadence. This is fictional speech SFX, not intelligible recorded dialogue. Smoothed envelopes, filtering and a bounded +/-1024 sample range avoid loud onset clicks and clipping; the existing SFX master and quiet-chatter preference apply.

The TV voice has its own worker-owned state and a single packed syllable publication; it never replaces or repeatedly queues gameplay SFX. Radio audio fades out while watching and returns afterward without changing tuning, volume or power settings. Leaving TV or pausing clears its audio gate. No music files, decoder allocations, extra threads or save changes are added.

Native traffic sprites have visible hull highlights and tiny engine lights. Three ships pass in both directions, inside the window mask and behind the station. Explicit sprites replace source-image highlight sampling, which was too faint to reliably show movement. The original studio art, subtle sun shimmer and star glints are preserved.

## Verification and remaining work

Final focused run: 45 PASS, RESULT 0 failures, in local-tv-scheduled/qa-tv-20260930-082047-520 under the 2026-09-26 task workspace. Delivery: outputs/ArcElite-v2.5.234-scheduled-TV/EBOOT.PBP in that workspace. SHA256 B94FA7E4E4E6B997AE3B3FA07CBFE0161DB75D4773E5729C287272C2937BAE80 (6,614,575 bytes); the canonical dist/ELITE-NEXT-PSP/EBOOT.PBP matches. The shared root EBOOT was not overwritten. Test flags are excluded from the player package.

Opt-in tv-check.flag tests exercise exact slot boundaries, full cycle, midnight, reopening, frame-rate-independent captions, removed tuning/restart inputs, all caption layouts, mouth motion/rest, actual ship displacement, audio gating/quiet mode, non-silent bounded PCM, fade to silence, framebuffer bounds, menu return and the real PSP clock API. Native screenshots include all programmes and 23:59; the motion GIF and PCM/WAV preview come from the actual PSP binary's functions.

Baseline and scheduled candidate broad smoke runs match: one inherited station swept-collision failure, 138 inherited outfitting/input failures, zero radio/steering failures, and incomplete input output at the same campaign-response test region. Both time out at 120 seconds. The full suite is not green; physical PSP audio/visual testing remains required. Do not publish a supposedly fully verified release based on the focused TV checks.

Next: physical PSP clock/timezone, sound-level and suspend/resume check; improve programme script variety; investigate inherited broad-suite failures separately. Preserve the other contributors' shared source edits. No automatic git autostash, commit/push/tag or save/music replacement was performed.
