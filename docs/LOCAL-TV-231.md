# Local TV: approved-art rebuild (2.5.231)

The previous procedural scene and cropped Mira did not match the approved concept. This rebuild uses the exact user-supplied full studio illustration, downsampled to 480x272 and compiled as opaque ARGB1555. Mira's seated pose, desk, mug, plants, detailed station/planet/sun window and original CH8 orbital ident are retained. Native live text replaces the concept's baked captions, headings, schedule cards and controls.

## Implementation

- Discover opens Local TV; Left/Right selects CH8 Evening Orbit, CH12 Farmers' Market or CH19 Night Stories. X restarts the selected programme. Circle returns to Discover, with priority over simultaneous tuning input.
- All three Lave programmes contain four passages and loop automatically. Text uses fractional time accumulation at 28 characters/second, then holds for six seconds. The former integer-per-frame accumulator could stall at 60 fps.
- Full-height game glyphs retain their original pixels; only horizontal side bearings are trimmed. Captions wrap before reveal, within three native rows. Farmers' Market wraps in the sidebar rather than being squeezed into illegible text.
- Talking remaps only the original tilted smile's texels. The face stays registered; the mouth rests after each caption. Window traffic is masked behind the station silhouette and away from the host. Sun shimmer and star glints remain subtle.
- CH8 retains the original logo; CH12/19 reuse the orbital frame with cyan/violet fitted numerals. This is caption-driven animation, not recorded speech or lip-synced audio.
- No save-format change, gameplay simulation update, PNG decoder or runtime artwork allocation. Full artwork costs 261,120 compiled bytes. Legacy Mira assets remain on disk but are no longer included in main.c.
- main.c owns page input/entry; deck-ui.h prevents unrelated old game messages from obscuring the broadcast. local-tv.h contains the renderer and scripts; local-tv-tests.h is opt-in native QA only.

## Verification and limits

Native PSP compiler builds successfully. PPSSPP executes the actual binary's TV test mode and captures its framebuffer, not a browser reconstruction. Tests cover 30/60/120fps reveal, all three channels, all 12 caption fits, wrap/restart/passage progression, mouth activity/rest, window mask, framebuffer bounds, menu return, and unchanged units/system/hull. Focused report: zero failures; measured TV render mean approximately 15.4 ms in the emulator, not a physical PSP performance guarantee.

Broad baseline and candidate smoke runs both report the inherited `swept collision blocks station tunnelling` failure. Both also stop producing input-test output at the same campaign-response line, without reaching the full performance report. Thus the full regression suite is NOT green and is NOT claimed as passed. No unrelated gameplay fixes or release push were attempted.

The authored studio, scripts and branding are Lave-specific. Other systems can open the page using their dynamic menu/title, but currently receive the Lave broadcast; 256 unique studios or 768 authored channels are not implemented. Future expansion should use bounded per-system data and streamed/shared studio art rather than embedding hundreds of full-frame illustrations. Physical PSP testing remains necessary.

## Shared-build integration

Refreshing the isolated build from the shared source preserved concurrent chart, portrait and rear-view-mirror work. That refresh exposed the mirror's use of dim_rgb before its later voyage.h definition; one matching forward declaration was added before flight-extras.h. No mirror behavior was changed.

## Reproduction and next steps

Run tools/bake-local-tv.py with Pillow to compile assets/source/local-tv-reference.png into src/local-tv-art.h. Build with build.ps1 and the installed PSP SDK. In an isolated test directory only, tv-check.flag runs focused tests/captures and exits. tools/run-tv-qa.ps1 prepares that fixture; tools/tv-proof.py exports the real captured frames as PNG/GIF and an approved-reference comparison. Do not include test flags in PSP deliveries.

Next: test on physical PSP, investigate the inherited suite stall independently, then expand authored channel content and system branding without claiming synthetic traffic or prices are live gameplay facts. Shared repository edits from the Almanac/chart/portrait work are preserved; this is a local development build, not a published release.
