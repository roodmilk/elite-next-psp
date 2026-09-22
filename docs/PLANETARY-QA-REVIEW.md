# Planetary / EVA QA review

Review target: `specialist/planetary-eva` working tree based on canonical `ac8a581`.

This review is read-only with respect to the Planetary implementation. It records
acceptance coverage and risks for the feature owner; it does not claim physical
PSP validation.

## Findings

- `terrain_height()` and `planet_view()` now use the same 40-unit lattice and
  diagonal interpolation. This removes the previous visible/collision terrain
  disagreement. The shoreline also uses cell centers for both rendering and
  collision.
- `eva_position_allowed()` prevents walking onto water or beyond the bounded
  EVA field. Rejected movement attempts slide one axis at a time rather than
  wrapping to the opposite side of the field.
- `eva_can_board()` requires the commander to be near the parked ship and within
  a vertical tolerance. `eva_toggle()` resets pitch, roll, jetpack and boost on
  boarding.
- The EVA input branch runs before spacecraft boost, brake, target and weapon
  chords. D-pad walking/strafe and L+pad look are explicit, while analog input
  remains available when enabled.
- The new terrain path is bounded but more expensive: each visible cell samples
  multiple interpolated vertices, and the current draw region is 11×11 cells.
  Frame-time evidence should be collected separately for atmosphere flight and
  on-foot EVA.

## Acceptance matrix

| Area | Required check | Current evidence | Remaining check |
|---|---|---|---|
| Approach | Approach, enter atmosphere, reject gas giant | `game_tests` | Keep green after terrain changes |
| Landing | Pad landing, high/fast rejection, water rejection | `game_tests` | Add edge-of-pad and shore-adjacent cases |
| EVA movement | Walk, strafe, look, bounded field, no water entry | Partial input/game coverage | Add D-pad strafe and shoreline clamp assertions |
| Jetpack | Hold/release, floor clearance, ceiling clamp | Existing release/floor test | Add repeated hold/release and max-height assertion |
| Boarding | Near ship boards; far ship rejected | Existing distance test | Add vertical mismatch rejection and exact boundary case |
| Recovery | Hazard/death, board, takeoff, orbit restore | Existing end-to-end path | Add hazard death followed by restart/return-state check |
| Reward | Scan life/resources, discovery and credit result | Existing scan test | Verify scan → board → orbit → sale without state loss |
| Controls | Analog on/off and L+pad fallback | Existing nub-look regression | Add explicit disabled-analog L+pad look/strafe test |
| Performance | Atmosphere and EVA frame rate, heap floor | Frame-rate smoke exists | Add heap samples and isolate terrain/EVA scenes |
| Suspend/audio | Resume from planetary and EVA states | General smoke suspend only | Add post-EVA suspend checkpoint; physical PSP still required |

## Suggested focused test cases

These belong in the Planetary-owned tests, not in shared runtime code:

1. Set `g.surface=2`, hold D-pad right for several ticks, and assert lateral
   displacement while forward displacement remains bounded.
2. Disable analog steering, send L+Up and L+Right, and assert camera look changes
   without walking; release L and assert D-pad movement resumes.
3. Place the commander exactly at the boarding radius and then just outside it;
   repeat with a vertical offset beyond the boarding tolerance.
4. Move toward a water cell and the EVA-field radius for many ticks; assert the
   position never enters water or wraps to the far side.
5. Hold jetpack for a sustained interval, release it, and assert altitude stays
   within the floor/ceiling bounds while the commander eventually settles.
6. Complete scan → board → takeoff → orbit restoration, then verify the reward
   remains present after docking/sale.

## PSP-specific limits

PPSSPP can establish deterministic state transitions and approximate frame cost.
It cannot certify analog feel, speaker continuity, suspend/resume after long
storage sleeps, or sustained memory fragmentation on physical hardware. Those
remain separate hardware gates.
