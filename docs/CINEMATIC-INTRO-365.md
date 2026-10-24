# 2.5.365 — Cloud flight and station reveal

Implemented in the PSP executable, not just the browser preview. This build
starts with a continuous native 480×272 pixel-art film: approach Earth, descend
through its separate cloud layer, accelerate through outward-flowing wisps,
dip beneath the cloud crest, then rise into a station/freighter/sun vista.

The final effect pass adds thresholded highlight bloom, sun halo and lens ghosts
that peak around the station reveal, cloud motion exposure, speed streaks,
coloured space dust, ship engine trails and animated station/star lights.
The bloom layer uses transparent shadows; it must never darken the whole frame.
The previously approved camera path and cloud skim are retained. Art and frames
are authored/rendered at 480×272, with crisp UI drawn separately by the game.

## Timing and controls

- 0–3.2 seconds: Cula Games presents… over the moving approach.
- 3.2–7 seconds: Elite re-imagining credit, including Braben and Bell.
- The remaining journey accelerates through clouds into the space vista.
- Logo fades in at 27 seconds and remains visible.
- X at any time reveals the existing menu over the continuing film; it does
  not start a commander on the same press. Menu fades for 800 ms, requires X
  release, then accepts a fresh press. D-pad selects as before.
- Without X, the menu fades in automatically at 36.34 seconds.
- At 48 seconds the film repeats the animated 40–48 second station vista.
- Returning to the title opens the ready menu at the vista immediately.
- Continue, New Commander, Load Commander, Tutorial and the existing Square
  Multiplayer shortcut remain. An unfinished tutorial must not block the title
  loader/Multiplayer or draw its story overlay over the title.

## Native player and packaging

The movie is embedded in EBOOT.PBP's DATA.PSAR. No extra install folder is
required. `src/intro-movie.h` streams indexed baseline 4:2:0 JPEG frames through
the PSP JPEG decoder at 30 fps. It reuses cached decoded frames on faster redraws
and skips to the current film time if rendering falls behind. Only one compressed
frame and one 480×272 RGBA frame are resident: under 640 KiB of frame buffers,
plus a bounded 7 KiB index. The archive is not loaded into RAM in full.

Exit/title changes and suspend close the stream and release JPEG resources.
Resume reopens at the retained time. The movie does not modify commander saves,
inventory, mission state or simulation. Invalid/missing media falls back to the
previous static title art. Existing radio preferences/music remain; this patch
does not add an orchestral recording or bundle any player-supplied music.

`build.ps1` and `build.sh` link pspjpeg, read VERSION and embed the film when
present. PowerShell also accepts `-GameVersion` for isolated candidate builds.

## Reproduce and verify

Sources: `assets/source/intro-film/intro.html` and its standalone
`preview-browser.html`. Bake with Node `tools/bake-cinematic-intro.cjs` using
Playwright/Chromium (CODEX_NODE_MODULES and CHROMIUM_EXECUTABLE can override
local runtime paths). It writes `assets/intro/cinematic.enm`, metadata and
native JPEG proof frames. Run Python `tools/check-intro-film.py EBOOT.PBP`
to check every frame header, archive boundaries/checksum and PSP sampling.

The archive contains 1,440 frames, 30 fps, loop index 1,200; 42,372,480 bytes;
largest frame 40,438 bytes. SHA256:
`0112B4C740CD880B8F0AF4BBF28D0BD4F17D91B1519EE4971EA5CA56395A499C`.

`tools/review-intro-film.ps1` tests the exact EBOOT via PPSSPP, capturing
native journey/loop/long-name frames and checking decoder startup, guards,
memory, cached frames, suspend/reopen, malformed media, X guarding, loader,
creation, Multiplayer and automatic menu. `tools/review-intro-integration.ps1`
runs the existing opening, Multiplayer, portraits and Command Deck fixtures
sequentially, avoiding concurrent emulator graphics-backend contention.
An optional `-MusicTrack` MP3 is copied only into the disposable review folder;
three additional checks require decoded MP3 samples to advance before/during
JPEG playback and after JPEG resources are released. It is never packaged.
The opening fixture also now accepts legal v3 portrait DNA while still rejecting
its reserved bits; this replaces the obsolete expectation that every v3 ID is
malformed. Concurrent portrait fixture corrections are preserved.

Final evidence and EBOOT SHA are in `BUILD-2.5.365.txt`. Physical PSP decoding,
memory-stick throughput, frame pacing, audio coexistence and real sleep still
require hardware playtesting. This intro release does not certify the entire
game or resolve previously recorded broader gameplay failures.

## Shared-work boundary

The candidate was compiled from `outputs/intro365-source`, frozen from the
shared local 2.5.364 development source. Its own VERSION is 2.5.365. The final
movie was copied into that snapshot before packaging. Preserve concurrent
portrait work and unrelated mixed changes. No wholesale shared checkout commit
or GitHub release was made; remote DNS was unavailable during this task.
