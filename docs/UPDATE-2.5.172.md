# Planetary transfer redesign

The user reported that v2.5.171 still freezes and powers off on Circle disembark. The prior input fix did not establish or fix the hardware fault. This update replaces the ambiguous transfer flow and adds diagnostic checkpoints; it must not be represented as a proven physical-PSP crash fix without a retest.

## Player flow

1. Target/approach a planet and explicitly confirm X: land and exit.
2. Guided landing and airlock preparation run without skip actions. Circle/Triangle/R cannot interrupt or launch.
3. Touchdown automatically disembarks to a collision-checked exit.
4. On foot, Triangle beside the ship boards. Farther away it faces the ship; R remains jump/run.
5. A separate parked-ship screen requires button release, then R launches to orbit or X steps outside. Triangle/Circle do not launch.

Atmospheric Circle opens an explicit landing confirmation instead of instantly landing. Existing station walking controls are untouched.

## Implementation

planet-sequence.h uses compact, non-interruptible transfer screens. No temporary camera/surface mutations or extra ship geometry are used in these screens. main.c gives these states exclusive control ownership before ordinary flight/EVA input, with release gating between transfers. Surface target/return state is cleared after disembark. game.c splits board_planet and disembark_planet; the latter validates ship/world/exit state before committing, resets movement/rover/jump state and does not start another computer voice. The legacy eva_toggle wrapper remains only for model compatibility/tests. Tutorial, story and HUD hints now match the controls.

## Verification and diagnostics

Real-button tests cover held/pressed buttons during transfer, automatic exit, release gating, Triangle boarding, R launch, X exit and atmospheric confirmation/cancel. Guarded rendering covers every actual purchasable hull and all four Lave bodies. The old world exit test incorrectly looped over 16 hulls despite only 10 being defined; corrected to player_ship_count, with production index guards added. Tutorial tests now wait for guided touchdown/auto-exit and explicitly release buttons before scan/launch. Model exploration/commerce/save tests remain in place.

landing-trace.txt records bounded checkpoints before/after airlock transition, first EVA render, first EVA movement tick, boarding and launch; includes system/body/ship and remaining thread stack. It resets at arrival and truncates after 64 entries. It is disabled during smoke tests and harmless if the file cannot be written. If hardware still powers off, request this file plus commander save, ship/body, PSP model and firmware. Diagnostic logging is not proof of the crash cause.

No physical PSP is available for verification. The renderer/transition redesign and tests reduce unsafe shared state, but hardware shutdown resolution remains unconfirmed.
