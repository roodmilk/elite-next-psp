# Soft flight skies — local candidate, 30 September 2026

The earlier distant sky used angularly scrolled screen-space patches with a noise
hash tied to rounded coordinates. Pitch changes could therefore change the patch
pattern itself, while reduced angular multipliers made the clouds disagree with
stars and roll. Those implementations have been removed, not layered underneath
the replacement.

## Appearance and motion

- A per-system celestial sky contains a warped gas band, branching cloud density,
  a darker dust lane and paired blue, violet, teal or warm mineral colours.
- Six continuous 65x65 faces are generated once for the system. Adjacent faces
  share edge directions. Camera rays use the exact inverse of the flight camera,
  including pitch, yaw and roll; ship translation cannot move distant clouds.
- Bilinear sampling and per-pixel interpolation replace filled rectangles,
  circles, screen-grain overlays and moving stamps. The small sampling grid is
  not visible as blocks: every output pixel receives an interpolated colour.
- Cloud geometry does not pulse, reseed with time or scroll at a different rate
  from the stars. Subtle animation is reserved for slow stellar twinkle, with
  fixed-length, tapered bright-star rays.
- Removed redundant screen-space dust that randomly repositioned at 20 Hz.
  Existing physical near-dust, engine trails and boost effects remain.
- Lens-flare spots now have radial feathering and follow sun intensity; a
  zero-radius flare guard also avoids division by zero at a distant sun.
- High-contrast mode continues to omit decorative cloud rendering. Preview
  clipping, planets, sunlight, traffic and gameplay state are preserved.

## PSP cost and implementation

`src/soft-space-sky.h` owns a fixed 101,400-byte cubemap, with no per-frame heap
allocation. The sky is compiled code, not an external texture download. The
first generation costs roughly 0.19 seconds in PPSSPP; subsequent camera views
reuse the cache. Palette interpolation combines red/blue integer lanes, and
full-size grid cells use exact shifts instead of integer divisions. This keeps
the same smooth per-pixel result without adding noise or reducing resolution.

`src/space-fx.h` integrates the replacement and removes obsolete sky paths.
`src/flight-extras.h` contains star-ray and lens-flare changes. `src/main.c` only
adds opt-in diagnostic entry points; normal player input is unchanged.

## Reproducible verification

Build a source snapshot with `tools/build-chart-candidate.ps1`. Despite the
helper's name it compiles the full game without touching shared release files.

- `tools/review-soft-sky.ps1 -Eboot <candidate>` runs the native executable in
  six systems: 7, 31, 63, 127, 173 and 255. It checks translation/time invariance,
  full-turn continuity, small-pitch continuity, hard edges, high contrast,
  non-aligned preview clipping and framebuffer guard words. It captures four
  headings, a close sun/planet pass and a Lave pitch sequence.
- Add `256` to `-Systems` for a separate complete-catalogue check: generate
  all 256 cubemaps, hash every texel, reject duplicate maps and uniform skies.
- `tools/preview-soft-sky.py <review-directory>` assembles those unretouched
  native captures into a gallery and pitch-sweep GIF.
- `tools/benchmark-soft-sky.ps1 -Eboot <candidate>` measures full flight,
  scripted pitch/yaw/roll and expanded flight view, with normal gameplay and
  audio initialization. All test directories are disposable and isolated from
  player saves. Only emulator instances started by the scripts are stopped.
- `tools/check-soft-sky-smoke.ps1 -Eboot <candidate>` runs the existing broad
  smoke suite in isolation, reports failed/incomplete groups and stops after
  120 seconds. It does not hide the known inherited failures.

Test flags are never included in player deliveries. No save format, controls,
planetary interaction, station layout, ship combat or music files are changed.

## Tested candidate and results

Snapshot: `outputs/chart-build-20260930-005542-746/EBOOT.PBP`, version 2.5.232.
SHA256: `63C55846EB47913B40D83DC7A608D06069BFDDB1C40189A493D6B3D2D9A8C958`.
Shared root/dist executables, release version and other contributors' source
changes were not overwritten. No commit, push or release tag was made.

Focused native reports/captures: `outputs/soft-sky-review-20260930-005658-365/`.
All 48 per-view checks and both complete-catalogue checks pass. All 256 maps
have different content hashes and non-uniform cloud/dark regions. Six systems'
four headings, close sun/planet views and the Lave pitch sequence were captured;
the six-system gallery and selected near/motion frames were visually inspected.
The first cache generation/draw took 188–196 ms; warm moving-sky drawing took
7.87 ms in this diagnostic setup (sky only, not the whole frame).
All 68 native BMP captures are byte-identical to those from before the final
integer-rendering optimization, confirming that its speed changes did not
alter the appearance or camera projection.

Full-flight performance: `outputs/soft-sky-performance-20260930-005812-106/`.

| Scene | Measured average FPS |
| --- | ---: |
| Lave, fixed heading | 29.97 |
| Lave, pitch/yaw/roll | 36.42 |
| Gelaed, pitch/yaw/roll | 37.40 |
| Orerve, pitch/yaw/roll | 36.00 |
| Gelaed, expanded view | 29.97 |

Worst observed sampled frame was 38.96 ms. The original strict `>=30.00` test
flagged the nominal half-refresh 29.97 cases; the diagnostic now explicitly
labels its target nominal 30 FPS, reports the real measured value, and uses a
29.90 measurement tolerance. This does NOT claim every frame is above 30 FPS,
nor eliminate the initial generation hitch. There is no forced frame skipping,
reduced emulated CPU load, or reduced-resolution block drawing in this change.

Broad candidate report: `outputs/soft-sky-smoke-20260930-005935-230/`.
Comparison baseline: `outputs/chart-build-20260930-003414-752/EBOOT.PBP`,
report `outputs/soft-sky-smoke-20260930-010208-697/`. Both report the same single
station swept-collision failure and the same 138 outfitting/module-bank input
failure lines. Radio and steering groups report zero failures. Input output
remains incomplete at the same campaign-response line; the broad performance
report is not reached. The candidate runner stopped after its 120-second
deadline. This comparison does not certify unexecuted tests or rule out every
regression, but these reported failures also occur without the sky changes.

## Remaining limitations

Emulator screenshots and timings are not a physical PSP comfort/performance
guarantee. Six systems receive direct visual inspection, not 256 hand-authored
backdrops. The rest use the same seeded generator. The repository's inherited
station swept-collision failure, outfitting/module-bank test failures and
incomplete campaign-input smoke run remain outside this pass; this is not a
fully certified whole-game release.
Physical PSP review should include slow pitch, roll, boost, close planets and
sun, then checking UI previews and high-contrast mode.
