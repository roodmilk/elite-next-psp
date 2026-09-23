# Strange worlds — first runtime art slice

Base: released main1e3fab88090e328916dc487f1c99031daf0a9061, VERSION2.5.48.
Branch: cursor/strange-worlds-art. Lead owns combined integration/version/release.

## Art intent and actual scope

The user requested strange-world atmosphere drawing on Peter Jones/Solar Wind,
Rodney Matthews, Asimov-like science-fiction imagination, Tim White and Ralph
McQuarrie. This is an original interpretation, not copied artwork: sculptural
shell-like architectural ribs, a monumental dock, a swept freighter, deep space,
and warm paperback-cover light translated into native hard-edged pixel shapes.

Implemented here: ARRIVALS backdrop only. Other six rooms, bar cast, person
sprites, surface flora/rocks/trees and exterior stations are NOT restyled by
this patch. Preserve the broader room rollout and original Art's bar work.

The current system's body1 appears through the window using the existing
draw_planet_sprite selector with its actual Body seed/type. No rehashed biome
or invented planet. This is a composed backdrop, not a heading/distance display.
Far dock shapes are decorative, not extra explorable locations.

## Integration boundaries

- New station-arrivals-art.h: authored canonical340x168 renderer, integer
  clipped spans, shared palette. No texture, heap, cache, RNG consumption,
  audio trigger, Game write or save change. Caller only supplies canonical size.
- station-crawl.h: replace sc_illust_arrivals body with the new helper; no
  hotspots/doors/people/options/SHIP/service changes. Existing overlays render
  afterward and retain priority.
- main.c: one test include beside menu-preview-tests.h, no input changes.
- New arrivals-art-tests.h and tools/test-arrivals-art.ps1: opt-in diagnostics.

Original Art explicitly approved superseding historical unintegrated6048a23
board composition. That candidate is preserved; do NOT cherry-pick its26KB
board asset alongside this replacement. This implementation adds no bitmap.

## Native rules

Scene(6,20,340,168), screen480x272, framebuffer stride512. Existing options,
feedback and SHIP anchors unchanged. Scenery uses eleven shared palette colours;
the unchanged authoritative orbit sprite retains its existing colour rendering
and is an explicit palette-test exception. Normal mode adds sparse seeded
stars and two dark depth bands; high contrast removes those decorative layers.
Lamps and berth marks are steady. They assert no music playback, alarm, job,
stock or arrival timing. Text remains the real8x8 font, never baked into art.

## Tests and measured trade-off

Released clean baseline build/all5smoke passed in work/smoke-20260923-013906-127.
Matched instrumented baseline: arrivals-20260923-014117-970,96draws over
systems7/39/0/255, normal+contrast,12times each. Mean1.9641ms/worst2.6370ms;
containment, unchanged Game, repeat and all5groups passed.

Initial candidate arrivals-20260923-014306-906: mean2.7368ms/worst3.4230ms.
Mean increase0.7727ms(~39.3% of backdrop draw cost), not a speed improvement.
All5groups passed; full smoke56.63FPS, worst33.37ms. Native screenshots for
Lave/Reorte/system0 reviewed at480x272; controls and anchors retained.
Final strengthened run arrivals-20260923-014451-689 adds scenery-palette and
draw-budget gates; all pass. Mean2.7125ms/worst3.4230ms: +0.7484ms mean versus
matched baseline. Instrumented whole-smoke56.91FPS. Separate normal run
work/smoke-20260923-014607-015 passes all five groups without capture flag.

Released uninstrumented ELF text1267310/data4580/BSS276020. Initial candidate
including diagnostic code: text1269122/data4580/BSS276020 (+1812total bytes).
Final ELF text1268566/data4580/BSS276020, +1256total bytes versus released
uninstrumented baseline, including diagnostics. Production heap additions0;
peak runtime stack not measured. These are emulator checks, not physical PSP
measurements. Capture I/O is outside the timed drawing loop.

## Before integration

Review final native captures and full regression evidence. Preserve original
Art's canteen helper/dispatch and Station interaction hunks; no whole-function
replacement beyond arrivals. Reconcile source checkpoint if main moves.
No claim that this proves whole-game visual conversion or all-station-family
variation. Systems' future shared descriptor may replace the existing planet
query only through a coordinated API change.
