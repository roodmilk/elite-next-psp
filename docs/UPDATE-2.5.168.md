# Police encounter update

## Cause
Input moved police_choice, but dialogue_reply highlighted the unrelated global menu row. Rendering now temporarily maps police_choice into the shared yellow reply layout, then restores menu focus.

## Behaviour
- Three replies with a clean hold; four with restricted cargo, paged three at a time.
- Only this inspection's cargo charge is tracked in transient police_cargo_heat. Surrender removes restricted goods and subtracts that attributed charge, never an existing warrant. Existing/legacy offences are conservatively retained; no guess is made about their cause.
- Charge-cap handling prioritises unrelated offences over the removable cargo component.
- A successful cargo-only surrender stops Law pursuit and presents a receipt; a mixed-offence surrender returns to warrant settlement.
- Paid and no-funds custody use separate transfer/review/release phases. Existing fee and property consequences remain. State transitions apply consequences once, and release waits indefinitely for acknowledgement.
- No new textures or save-version change. The charge attribution field is transient, not serialised; saves cannot be made during the custody modal.

## Verification
Regression cases cover Up/Down and wraparound, dirty-only reply visibility, cargo-only and mixed warrants, repeat surrender and maximum-warrant abuse, paid/no-funds custody, release acknowledgement and no accidental firing. Native captures cover each reply and custody phase. Full smoke suite also checks existing saves, tutorial, Wanted, radio, steering and performance.

Final run and binary hash are recorded in the delivery TEST-RESULTS.txt.

## Limits / next steps
No physical PSP is attached. Device checks should cover held-button repeats, both custody outcomes, saved criminal records and audio during a full arrest. The custody animation is a small PSP-native pixel scene, not a new walkable prison.
