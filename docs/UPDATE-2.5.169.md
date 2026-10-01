# Outfitting and Mission Board integration — 2.5.169

## Implementation

Outfitting uses a shared 25-item bound, exact fit-slot validation, economy/technology/prosperity stock rules and seeded secondary-hub restrictions. The special Chandler validates its own advertised stock rather than silently rejecting it against ordinary-shop rules. Replacement quotes subtract resale value, including a refund for cheaper replacements, and require confirmation. Hold changes cannot strand cargo or passengers. Outfitting and Loadout link with Triangle.

The Heat Buffer's final catalogue index and upgrade bit are now accepted in saves. Auto Repair repairs actual hull (2/s while cool and out of combat). Escape pods recover fatal/thermal damage and consume their fitted slot. Mining damage is 54 against rocks, 18 against ships. Scanner identification is bounded at 5000 m with the module, 2500 m without. Nav Beacon highlights the selected next jump. Instructions distinguish passive effects, services and actual controls.

Boards now expose all five existing job types in every system. Seeded destinations are within the current hull's full-tank range, with a local fallback for isolated systems. Economy and risk still affect pay, and authored briefs vary. Board artwork uses the existing portrait renderer, not new assets. Acceptance tracks the job; selecting an accepted card opens its briefing. Local navigation while docked explains that launch is required and returns to station services. Remote navigation uses the existing fuel-safe route planner. Guild offer lookup now matches the same five-slot rule.

All board expiry was removed, including legacy one-job migration. Duration fields remain positive legacy save fields but are never decremented. Board, Tracked Mission, GalacticNet Jobs and tutorial text no longer advertise countdowns. Story/custody/flight-animation timers are unchanged.

## Verification

- Every system's five offers: valid/reachable, acceptance, real objective-handler completion, reward payment and no duplicate payout (1280 contracts).
- All 256 systems and three station variants: stock constraints and available catalogue coverage.
- Every fitted module: purchase, repeat-purchase protection, save/load, sale and effect-bit removal.
- Actual repair, escape-pod, scoop, mining and missile-defence simulation checks; existing power, scanning and flight suites retained.
- Capacity/passenger protection, special-stock purchase, net refunds, cross-screen controls and Board-to-Tracked-Mission input checks.
- Native 480x272 Outfitting, Loadout and Board captures; wrapped brief/instruction fit assertions.

## Compatibility and remaining work

Save layout remains V18. Back up commander files before upgrade; old executables reject Heat Buffer state. No extra runtime assets required. This remains a local build; no GitHub publication or destructive repository operations. Physical PSP soak-testing (especially long play sessions, saved-game resume and audio/input) is still needed. The sweep calls real completion handlers with deterministic encounter placement; it does not manually fly every route. All five contract archetypes are functional but this update does not add wholly new mission archetypes or persistent exhausted boards.
