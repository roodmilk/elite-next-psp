# ELITE: NEXT audio integration handoff

## Integration boundary

- Audio development base: `2.5.416` in this isolated `elite-next-audio` mirror.
- Authoritative integration target: ELITE NEXT Manager build at
  `C:\Users\skarm\Documents\Codex\2026-09-14\let\outputs\arcelite-psp`.
- The audio branch does not intentionally change save formats, game rules,
  music/radio content, ERL rules or presentation flow.

## Changed source

- `src/audio.h`: allocation-free mixer layers, responsive player engine,
  approved Warm Gravity boost, manoeuvring and hull settle, three nearby-ship
  mechanical families, station/docking sequence, recorded-only staged
  hyperspace, warnings, physical combat and three-stage power-up behaviour.
- `src/main.c`: supplies existing flight, traffic, docking and hyperspace state
  to the audio layer; adds no new saved state.
- `audio/SOURCE-LICENCES.md`: provenance and licence record for resident source
  recordings.
- `docs/AUDIO-VERTICAL-SLICE.md` and
  `docs/ELITE-ENGINE-REFERENCE-NOTES.md`: design, review and measurement notes.
- `tools/review-flight-audio.ps1` and `tools/analyse-flight-capture.py`:
  repeatable actual-game capture and signal checks.

## Gameplay event inputs required

The mixer expects current player speed, steering, boost, hull, held thrust and
true acceleration; nearest audible ship distance, bearing, speed, class, stable
seed, occlusion and radial speed; nearest station distance/bearing; docking
stage/phase/final lock; hyperspace time remaining; and warning severity/source
bearing. These are read-only observations of existing gameplay state.

## PSP cost and validation

- Resident flight PCM: 51,582 bytes, 22.05 kHz 16-bit mono sources mixed to
  44.1 kHz stereo without callback file I/O.
- Retail image: 13,623,516 bytes; configured heap: 7,340,032; main stack:
  1,048,576; calculated headroom: 3,153,700 bytes (3 MiB gate passes).
- Exact-retail full radio-on journey in PPSSPP: 36.39 FPS, 72.53 ms worst
  frame, 355 frames over 25 ms, eight mixer voices, zero queue drops and one expected
  lower-priority rejection.
- Complete radio/music regression passes, including distinct station output,
  full-volume mix headroom, preference save/reload, MP3 freeze and repeated
  audio stop/start recovery. Evidence:
  `outputs/radio-review-20261013-114731-433/radio-check.txt`.
- Paired traffic benchmark: matched silent control 26.07 FPS; continuous agile,
  medium and heavy flybys 25.23 FPS; delta 0.84 FPS / 3.2 percent; zero queue
  drops. The scene baseline itself misses 30 FPS, so retain the passing full
  journey as the release performance gate and repeat this pair on PSP.
- Corrected sustained-traffic and continuous-station capture:
  `outputs/audio-flight-capture-20261013-114757/`; 44.1 kHz stereo, 23.024
  seconds, zero clipped samples and signal-analysis PASS. The former synthetic
  late-hyperspace triangle tone, exposed 520/680 Hz warning beep and tapping
  traffic body are absent.
- Retail playable: `outputs/audio-flight-retail-final/EBOOT.PBP`;
  SHA-256 `FE96D3D31CA0001CB5E5B48CA6B1844368A884303ECB579F18F25863AAB71375`.
- Exact-retail stereo journey capture:
  `outputs/audio-flight-capture-20261013-122858/capture-EBOO00649_2026-10-13_13-28-59.wav`;
  signal-analysis PASS, zero clipped samples, SHA-256
  `0FED6DD1DBE852CC8BC8FBF8E658FA6978A0C23912DA7A263D02C34A212BBF72`.

## Build and remaining gates

## Manager 2.5.452 integration result

The handoff is now selectively integrated into the current Manager working
tree. Manager's five-block device queue, separate mixer/device threads,
semaphores, starvation telemetry, native-rate MP3 path and 48 KiB encoded
reservoir remain intact. The exact retail output is
`outputs/audio-integrated-2.5.452/EBOOT.PBP`, SHA-256
`399C6DF9A9F0F939CED7952737F2AEB870110B0FC5590A5F49F2E5A2357F73A8`.

Its strict PSP memory result is a 13,629,308-byte loaded image and 3,147,908
bytes calculated headroom. Against an exact rebuild of the pre-integration
Manager files, the complete audio merge adds 9,632 loaded bytes and 11,496 PRX
and PBP bytes. The original PSP-model cold boot and PSP-2000 flight boot both
report zero failures.

With a real 44.1 kHz MP3, the Manager queue stress measured 33.77 FPS, 261
device blocks, zero buffer starves and zero command drops. The integrated
19.828-second stereo capture is in
`outputs/audio-flight-capture-20261013-125238/`; it has zero clipped samples,
signal-analysis PASS and SHA-256
`D4B1D331D6EC3E27AC87DFAD86B69ABFAAA3B8B1AF3CFB33B441CB1F269EA274`.
The complete radio/restart and ERL suites pass. Focused load/arrival, intro,
command deck, menu and opening suites pass. The current broad core fixture
retains 18 unrelated working-tree failures and is not an all-green claim.

Physical PSP listening, timing and suspend/resume remain the release gate.

Run `build.ps1` with the PSP toolchain path and a distinct build/output
directory. Run `tools/check-psp-memory.ps1` against the resulting ELF and PRX.
For an actual-game comparison, create an extended-memory review build and run
`tools/review-flight-audio.ps1 -Radio`, then analyse its WAV with
`tools/analyse-flight-capture.py`.

Before merging into the Manager build, reconcile `src/audio.h` and the small
audio-state feed block in `src/main.c` against the Manager versions rather than
copying either file wholesale. Repeat radio regression, save/load, ERL,
suspend/resume and full-journey checks after integration. Physical PSP playback,
performance and suspend/resume are still required before this EBOOT can be
called release-tested.
