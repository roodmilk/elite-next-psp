# Planetary push — shared checkpoint contract

Feature/design owner: Planetary Exploration. Integration/release owner: Lead.
Coordination/file reservations: Team Manager. Base: main ac8a581 (v2.5.47).

## Playable checkpoint 1

One bounded local excursion: orbit approach -> atmosphere flight -> pad landing
-> EVA look/walk/jet -> existing survey/mineral scan -> locate/board ship ->
takeoff/orbit -> existing hub sale/save. Quality means responsive controls,
readable place/goal/return, truthful rewards and safe recovery, not world size.

| Acceptance | Owner | Status |
|---|---|---|
| Safe solid/gas approach and outward orbit return | Planetary | Shipped 2.5.47; preserve regressions |
| Nub look; pad walk/strafe; L+pad look fallback; no ship chords in EVA | Planetary | Implemented; PPSSPP regressions pass |
| Hold R lift/release fall, bounded height; Circle nearby grounded boarding | Planetary | Implemented; PPSSPP regressions pass |
| Ground mesh and collision share a deterministic 40-unit triangulated grid | Planetary | Implemented; PPSSPP regressions pass |
| Water/local edge blocks traversal, no opposite-edge teleport | Planetary | Implemented; PPSSPP regressions pass |
| Visible ship bearing/distance, Triangle face ship, recoverable round trip | Planetary | Implemented; PPSSPP regressions pass |
| Square survey, mineral cargo/refinery/full hold, retained reward on return | Gameplay + Planetary | Existing mechanics; audit and end-to-end regression |
| Truthful controls/coaching and native 480x272 screen review | UI + Story + Planetary | Review support; implementation in Planetary slice |
| All five smoke groups, save compatibility, frame-time comparison | Systems + Planetary | Required before handoff |
| First-time reward/revisit rules and guaranteed useful activity | Gameplay | Audit/proposal; no unapproved state expansion |

## Ownership and write boundaries

- Planetary owns game.c planetary height/movement/collision/API sections, game.h
  matching APIs/constants, main.c EVA dispatch/axes/tests/help bounds, planet.h
  shared ground mesh/HUD, deck-ui.h EVA help and ui-navigation-tests bounds.
- Story owns wording authority. Planetary requests exact coaching changes before
  editing story.h. Story provides brief original context and complete action
  wording; no new campaign scene/state is assumed implemented.
- UI/Art reviews control hierarchy, safe bands, return cue and terrain readability;
  no simultaneous planet.h/main.c writes. Review doc: PLANETARY-UI-REVIEW.md.
- Gameplay audits existing survey/activity/resource usefulness and duplicate
  rewards; no edits to survey_scan, sectors or docking in this checkpoint.
  Review doc: PLANETARY-ACTIVITY-REVIEW.md. Propose one bounded next mechanic.
- Systems reviews edge cases, native costs, save compatibility and evidence;
  no shared runtime edits. Review doc: PLANETARY-QA-REVIEW.md.
- Team Manager agrees any new write boundary before work; lead integrates tested
  commits and owns version/tag/release. Existing unrelated candidates stay parked.

## Gates and explicit limits

Required: analog enabled/disabled; fallback look excludes walking; jet release;
no roll/autoaim/weapon leakage; board rejects airborne/far/dead states; scan button
remains functional; edge/shore permits safe retreat; full excursion returns to
orbit with existing cargo/discoveries; all help pages wrap; native captures in
normal/high contrast; existing approach/save/campaign tests; five smoke groups.
Compare frame time to unchanged base. Capture I/O is not a performance run.
Keep pools/draw-list fixed and no per-frame allocations. No new Game fields or
save schema planned. No discovery persistence/reward changes without a concrete
proposal. Revisit farming, decorative huts/skyline and hardware-only limits must
remain honestly documented until addressed. Physical PSP testing is separate.

Next checkpoint follows evidence: one useful existing-state surface activity or
clearer survey goal, plus authored local visual interest. No cities, unlimited
streaming, rover/base systems or unrestricted planetary travel are promised.

Checkpoint evidence: PLANETARY-EVA-HANDOFF.md. Useful mineral loop verified on
the starting ocean world; all-world activity and revisit policy remain proposals.
