# v2.5.210 traffic verification

## Build and scope

PSP cross-compile succeeded. Final EBOOT SHA-256:
`46C3BE3A9412086D34DB58A71E143EBC69EB2413DC525C3D0094BB5FDA7937AB`.
Source changes: transient route/scanner state in game.h; traffic-routes.h;
game.c routing/dispatch/escape/damage-credit integration; module catalog/fit;
Circle tool UI; radar; System Operations route map; target activity labels;
faction descriptions; regressions and version/docs. Existing dirty work preserved.

## Emulator comparison

Both executions used silent-test.flag (audio hardware disabled). Baseline was
the previously packaged 2.5.209 executable, not rebuilt old source.

| Suite | 2.5.209 baseline | 2.5.210 full rerun |
|---|---:|---:|
| Game | 1 failure | same 1 failure |
| Input/UI | 38 failures | same 38 failures |
| Steering | 0 failures | 0 failures |
| Radio | 0 failures | 0 failures |
| Performance | 0 failures | 0 failures |

Existing failures: station swept-collision assertion; 36 chapter-response
assertions; GalacticNet scrollbar assertion; cockpit text-area assertion.
These are unresolved baseline failures, not a claim that the suite is green.
No physical PSP validation has been performed.

All added checks passed: route assignment for all 256 systems, unengaged Wanted
escape/protection, assisted bounty credit, port service and return, station
dispatch, missing-module rejection, scanner range/recharge/snapshot behaviour,
module save/load and transient clearing, Circle+R select vs tap activation,
route-map controls. Updated legacy tests verify the new escape policy rather
than demanding a police/pirate kill. A 100-second ambient simulation retains
all five unengaged Wanted targets. Existing hull separation and smooth movement
tests pass. An introduced rescue holding-pattern regression was fixed by
retaining the scripted waypoint>=8 cruise override; rescue checks now pass.

Native 480x272 tool/radar/map captures were inspected. The route map was then
scaled down slightly to keep its lowest marker clear of the target caption;
this final presentation-only adjustment is in the packaged executable.

Reports under the task directory:
- traffic-baseline-209 (previous executable)
- traffic-final-210 (full comparison run)
- traffic-release-210 (final package and spacing confirmation)

## Follow-up

Hardware-test the new tool gesture and radar, long flights, crowded station
approaches and bounty escapes. Tune interception frequency/travel times with
player feedback. Do not represent the current system as persistent economic
simulation or a permanent law-coverage guarantee. Separately diagnose the
baseline suite failures before claiming a clean overall release certification.
