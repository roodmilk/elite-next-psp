# PSP-native visual audit plan

Scope: existing ELITE: NEXT presentation only. The audit uses the Second
Shift native room as the quality floor: every upgrade is authored at its final
pixel size, keeps the current interaction contract, and must be proven in both
normal and high-contrast `480x272` captures before promotion.

## Ranked weak areas

### 1. Lave canteen identity

The generic canteen renderer was visibly flatter than the Second Shift room,
and the best authored bar exception was bound to Reorte H0. The first concrete
patch is now in `src/station-crawl.h`: Lave system 7 gets a native 1x scene
with an old-GalCop arch, Lave Public sign, bar back, practical lamps, juke,
booth, floor rail and doors aligned to the existing exit rectangles.

Acceptance: normal and high-contrast runtime captures, the existing BARTEND
and TRAVELER hotspots still select correctly, and SHIP remains reachable.

### 2. Generic station-room fallback

The seven-room graph and procedural descriptors are present, but Lave’s
non-canteen rooms still rely on the generic illustration path. The next small
patch should add one native identity anchor per room, reusing the frozen cue
kit for signs/doors and the procedural family elements for material variation.
It must not add new interaction IDs or let procedural accents cover a person,
hero prop or door.

Acceptance: seven Lave room captures, normal/contrast pairs, exact hotspot
rectangles, repeatable selector output, and no focal-object collisions.

### 3. Missing integrated captures for flight presentation

The code has native pixel paths for cockpit HUD, targeting, ship/loadout,
planet approach, EVA and transition FX, but the available proof set does not
contain a successful Lave runtime capture for these surfaces. This is an
evidence weakness rather than permission to replace the working systems.

Acceptance: capture cockpit, targeting, approach, landing/EVA and return
transition from one EBOOT; compare normal/high contrast; record frame-time and
confirm the screen remains 480x272 with no filtered or resampled art.

## Existing evidence

- `docs/LAVE-PSP-NATIVE-VISUAL-AUDIT.md` contains the current PASS/PARTIAL/
  BLOCKED matrix and hashes for the available native frames.
- `43f59d4` adds native targeting and loadout polish.
- `2f4b5c9` and `c2af5e9` contain the station cue kit and its approval matrix.
- `ab586a1` packages the Lave proof assets and audit report.

## Promotion gate

Do not call a surface clean until its normal/contrast runtime pair exists,
its native bounds are verified, its interactions are unchanged, and a repeat
capture is byte-stable or the intentional animation variance is documented.
The current PPSSPP smoke blocker (`Missing game report`) prevents that final
runtime claim.
