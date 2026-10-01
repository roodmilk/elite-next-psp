# Landing input safety — 2.5.171

Reported on physical PSP at Lave: Triangle on the atmosphere-entry notice returns immediately to orbit; Circle during the landing animation replays a cinematic, followed by black screen/shutdown.

## Confirmed code defects and changes

- The planetary Triangle handler ran before speech acknowledgement. It now consumes dismissible status/speech before orbit or takeoff. The explicit post-boarding prompt remains one-press launch and is labelled LAUNCH rather than CLOSE.
- Arrival/pad animation skip previously returned to the general input handler with the same pressed bits. It now consumes the press. Arrival skip cannot also land or leave; Circle during pad alignment explicitly completes that phase and disembarks, then returns.
- Sequence kind, planet index, dock/death state and surface mode are checked before starting, advancing or drawing. Stale pad animations while on foot are discarded instead of replayed. Cinematic ship and triangle indices are checked before array access.
- Cutscene footer instructions match the actual phase-specific actions.

## Tests

New landing-safety-tests.h checks natural entry, Triangle CLOSE, held Triangle, pad landing, skipping pad alignment, pad-message CLOSE, Circle disembark, held Circle, stale sequence discard, Circle arrival skip and Circle during pad alignment. Existing three boarding/departure cycles retain single-press launch. Tutorial regression now lets arrival finish before the separate landing action.

New landing-render-tests.h uses a guarded native framebuffer and the real input/render paths for every purchasable hull across all four Lave bodies. It renders pad alignment, presses Circle, renders on foot, and checks state/positions and framebuffer boundary sentinels. Existing cinematic camera-restoration/capture checks retained with valid phase states.

## Limits and next steps

The input-routing defects are confirmed and covered. The physical PSP shutdown has not been reproduced in the emulator, so these changes must not be described as a proven hardware-crash fix. Retest Circle during landing on the user's PSP. If shutdown persists, obtain the commander save, specific ship and planet, PSP model/firmware, and investigate a hardware trace or reduced-rendering reproduction. No physical-device test was performed here. No save format or runtime assets changed.
