# Planetary checkpoint two — a mineral on every solid world

Base: released v2.5.48 main1e3fab88090e328916dc487f1c99031daf0a9061.
Branch: specialist/planetary-activity-minimum. Lead owns integration/release.

## Change and compatibility contract

After the final enter_planet life-generation loop, count alive minerals. If none
exist, change only slot0.kind to LIFE_MINERAL. This is an entry-time operation,
not per-frame work. It adds no objects, Game fields, save fields, heap allocations,
reward changes, terrain/render changes or control changes. Existing populated
pools and every original position are retained, including a former fauna slot's
higher original Y. Future profile/density selection must occur before this final
minimum check or explicitly preserve it. Released main has no later selector.
Systems was consulted about pending profile integration; do not assume an
unintegrated profile/debug/bar branch has been validated by this checkpoint.

The correction guarantees an available material opportunity in generated pools,
not a new exploration objective or durable first-discovery record. Re-entry still
regenerates scanned flags and permits reward farming. See the separate read-only
PLANETARY-REVISIT-POLICY-OPTIONS.md; no policy there is implemented.

## Evidence on the released combined base

- Instrumented baseline PSP-target smoke: work/smoke-20260923-013525-974.
  Actual generation reports768 solid worlds,19 empty mineral pools. No ticks,
  scans or object edits precede the all-world snapshot.
- Final candidate smoke (-MenuPreview, no capture I/O): work/smoke-20260923-014409-442. All five groups pass:
  331 game and309 input checks, plus steering/radio/performance.
- Every6144 semantic object record was compared. Exactly19 slot0.kind values
  change; all coordinates, identities, alive/scanned fields and749 previously
  populated pools are identical. No19-world constant controls runtime behavior
  or the minimum assertion. tools/compare-planet-activity-pools.py reproduces
  the comparison from the before/after planet-activity-pools.txt files.
- All768 corrected pools have at least one mineral. Existing landing-kit,
  traversal/return, campaign, V13/migration and audio regressions remain green.
- A real originally empty deposit (system0/body3/slot0) retains its position
  and passes normal/refinery/full-hold/repeat arithmetic, existing SFX_SCAN on
  success only, board/orbit/guided hub return, V13 save/load before sale and
  sale of earned ore/alloy at the actual market price. Other objects are made
  unavailable only for this arithmetic fixture, not availability comparison.
  Pad/hub placement is deliberate; no claim of unassisted flight or walking.
- Baseline and candidate normal performance both56.63 average FPS,33.37ms
  worst frame,23 frames over25ms. Candidate surface scenes36..39 are
  59.94/39.96/37.46/39.96FPS. No physical PSP performance claim.
-44 existing compiler warnings on both builds. Three native-art contracts pass.
- Separate -MenuPreview run work/smoke-20260923-013836-238 passes all five
  groups plus panel sentinel, unchanged Game, landed/EVA restoration, all-hull
  margins and loop checks (84 samples,mean0.9898ms,worst1.7730ms).

## Focused visual evidence

UI requested former-fauna mineral contact review due preserved Y. Diagnostic-only
main.c capture hooks produced fallback-near.png and fallback-mid.png in
work/smoke-20260923-013946-032. World0/body3, seed2250431604,slot0; camera is
45/100m east of the original deposit at terrain+22, looking toward it, pitch0.
All five diagnostic groups pass, but capture-binary timings are not performance
evidence. Hook removed afterward; main.c is byte-identical to released base.
After removing capture hooks, final checks additionally assert exact credits
through orbit and docking. The final tested EBOOT SHA256 is
07EF1BCBFB04362A1D19168C482AC3C5D51D2EA32AB62BBDE2BCB775877F86B8.
UI accepts mineral recognition/readability: no HUD clipping or obstruction.
The45m image appears slightly raised;100m is too small to establish contact.
Rendering projects the preserved position directly, so former-fauna Y16 versus
normal-mineral Y7 is a real known presentation limitation, not certified ground
contact. The actual fallback scan/return/economy test passes at this unchanged
position; no interaction obstruction was observed in that fixture. Lead should
review this explicit tradeoff; future renderer-only footing work is separate.
These images came from the temporary-hook binary, not the final restored EBOOT.

## Shared interfaces and next integration gate

Only production hunk is enter_planet's final generated pool. Tests add one
include and src/planet-activity-tests.h. Story landing access, audio dispatch,
EVA controls/HUD, menu preview and save formats are unchanged. Gameplay agreed
minimum-slot policy and existing award-only cue test; Audio retains dispatch
ownership. The single bar recorder uses its own mission/site identity and is
not part of this pool, payout or ledger. Lead must refresh current main and run
combined build/all-five/native/performance/save checks after any later profile,
bar or debug integration. This isolated green checkpoint cannot certify those.
