# Planetary mineral availability — runtime audit

23 September 2026. Stable traversal base: 6341b51803ee92dccf560e5d1153ff618503cd0b.
Canonical main comparison: ac8a5810037dec52c26bbe962973f7af49764a40.
The stable traversal candidate and its tested binary were not changed.

## Result

An isolated PSP-target diagnostic under PPSSPP confirms 768 generated solid
worlds, of which 19 have no mineral. All five source-audit examples match their
expected eight-entry kind arrays. Lave I has minerals at indices 0 and 5.
Every entry began alive and unscanned. All generated positions were dry and
inside the 480 m field geometrically; this is not proof of safe walking/return.

The diagnostic uses a fresh Game per system/body, sets the system before launch,
skips generated gas/sun bodies, clears generic jobs and invokes actual
approach_planet then enter_planet from radius+500 with forward heading. No
life kinds/positions/flags were modified, and no simulation ticks or scans ran
before enumeration. Source expected counts are reproduced here, not asserted
as a timeless generation contract after future profile changes.

Diagnostic smoke: work/smoke-20260923-005908-141. All five groups pass.
Diagnostic binary SHA256:
02057ba0d1cdf88a3e5e8437c9eb7948e2d32f2aee8b39e666c7b7cd2d7a58e6.
This binary contains test-only instrumentation and is not a release candidate.
Do not substitute its timings or binary for traversal's normal validation.

## Reproduction

On an isolated checkout of the full traversal SHA, copy
`tools/planet-mineral-audit.h` to `src/planet-mineral-audit.h`, then include it
immediately after `planet-eva-tests.h` inside game_tests in src/game.c. Build
with the normal PSP toolchain and run smoke. The diagnostic writes
planet-mineral-audit.txt beside the smoke reports. The header's expected-count
check tests this historical reproduction, not a future feature requirement.
Never add this diagnostic include to the shipping source.

## Agreed next integration proposal — not implemented

Partner: Gameplay & Loops. If a generated solid world's alive pool has no
mineral, convert existing slot 0's kind to LIFE_MINERAL. Preserve position,
alive/scanned state, count, survey_scan arithmetic, Game and save schema.
Player benefit: each landable world offers a material opportunity.

Interface: final enter_planet life-pool generation step. Systems PlanetProfile
must select density/types before this minimum check, or explicitly preserve
it afterward. Compare semantic fields, not struct padding. On this baseline,
749 existing mineral worlds should remain identical and only slot0.kind should
change in the 19 empty pools. Recount after profile integration rather than
forcing historical counts. Existing normal/refinery/full/repeated scan and
real movement/boarding/orbit/hub-sale/save regressions must remain green.
Revisit reward farming and weak search objectives remain separate design gaps.

No guarantee fix is authorized in this audit. Implement only after traversal
integration and the next checkpoint is agreed. Lead must build the combined
current main with all five smoke groups, native visuals, performance and saves;
individual green tests do not prove combined compatibility.

Other concrete partner acceptance agreed this checkpoint: UI's menu preview
must preserve Game bytes and restore clipping while landed/on foot (snapshot
after legitimate Select input clears boost); Gameplay/Audio will verify that
existing scan-success audio corresponds to an actual award, clearing/consuming
old cue state before failed/repeat tests. No duplicate production changes here.
