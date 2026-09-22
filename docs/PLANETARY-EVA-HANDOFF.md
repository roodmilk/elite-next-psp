# Planetary EVA — traversal checkpoint

Branch: specialist/planetary-eva. Base: ac8a581 (2.5.47).
Prepared 23 September 2026. Pending lead integration; no version/tag/release here.

## Player result

The local excursion has distinct look and movement controls, shared visible/walkable
terrain, bounded shore/field travel and an explicit return route. Nub looks;
D-pad walks/strafe; L+D-pad looks without walking when analog is disabled. Hold R
lifts and release falls. Triangle faces the ship. Circle boards within 60 m only
at standing height (2-unit tolerance). Disembarking starts within boarding range.
Help page 5 and Story-approved coaching teach those actions.

Terrain uses a fixed 40-unit lattice with identical A-C triangle interpolation in
rendering and collision. The landing region and ocean island are level; rocky
terrain varies continuously. Shore collision follows complete visible water tiles.
EVA is pad-centred within 480 m, with sliding/retreat instead of wraparound. Jet
height is bounded to 120 m above standing height and consumes no ship fuel.
Atmospheric flight retains its existing larger wrapped area.

The world fills rows 28..239 between the controls and return strip. Ship bearing,
distance, exposure and health stay visible. Scan feedback takes priority over the
nearby boarding cue; damaging exposure takes priority over routine messages.
Landing/reboarding clears flight-inappropriate roll, aim and jet velocity.

## Validation and evidence

PSP-target executable under PPSSPP, not a physical PSP playtest.

- Base smoke: work/smoke-20260923-002635-867; all five groups pass.
- Final normal smoke: work/smoke-20260923-004633-989; 324 game checks,
  309 input checks, and all five groups report zero failures.
- Existing 44 compiler warnings remain; no new warnings. Three native-art
  contracts and git diff whitespace validation pass.
- Baseline average 57.99 FPS / worst 33.37 ms / 13 frames over 25 ms.
  Final average 57.18 FPS / worst 35.11 ms / 19 frames over 25 ms.
  Surface scenes 36..39: 59.94 / 42.81 / 31.55 / 54.49 FPS, versus baseline
  59.94 / 45.47 / 40.46 / 58.55. EVA remains above the existing 24 FPS floor,
  but its larger viewport and ground rendering cost require hardware validation
  and later profiling. This is not a claim of sustained 60 FPS.
- Corrected the benchmark's boarding setup from ship-centre height to actual
  standing height; otherwise the stricter gate correctly rejected that fixture.
- Seeded solid-world land/disembark/board/takeoff/orbit round trips, triangle
  interpolation and cell-edge continuity, diagonal speed, shore/edge retreat,
  jet ceiling/release, airborne/dead boarding rejection, analog-off look,
  weapons/roll isolation and Triangle press/release are covered.
- Resource matrix covers existing mineral/refinery and full-hold conversion,
  capacity and same-object repeat rejection. The input fixture scans a naturally
  spawned Lave ocean mineral, walks away/home, boards, returns to orbit, uses
  guided hub docking, saves/loads unsold ore and discovery, then sells it.
  Precise starting pad and hub-approach placements and other objects' scanned
  flags isolate the behaviour; this is not unassisted manual navigation.

Native 480x272 captures cover normal/high-contrast boarding, return direction,
scan feedback near ship, airborne rejection, exposure at HP35, maximum pitch in
both directions and Help page 5. Planetary visually reviewed corrected captures;
UI reviewed the earlier set and requested the fixes now present. Final independent
UI re-review was requested and is pending at preparation time.

## Scope and next checkpoint

No Game fields, V13 layout, reward arithmetic, generation pool, economy, mission
or persistent-discovery changes. Fixed draw lists remain bounded; no per-frame
heap allocations added. src/story.h has only the approved coaching replacement.
Shared main.c changes are confined to EVA controls/steering, viewport bounds,
Help bounds and test hooks. Systems' subsequent PlanetProfile work must preserve
this terrain/collision contract and merge deliberately after traversal.

Remaining limits: exposure can still kill and shares ship energy/death presentation;
this checkpoint does not add emergency teleport recovery. Buildings/skyline are
set dressing, not interiors. Re-entry resets scanned flags and permits repeat
reward farming. Gameplay's static audit found 19/768 solid worlds with no mineral;
runtime confirmation and a deterministic fallback mineral are proposed next,
not implemented here. A durable first-visit reward ledger needs joint design.
The current loop can scan from near the ship; it does not yet offer a strong
exploration objective. No unrestricted worlds, cities, rover or base system.

Lead owns integration, release numbering, tags and publishing. Gameplay's separate
docs-only activity review is f3f69d21dde5489fa29714a0b00c523129d91719.

Final capture run: work/smoke-20260923-004801-614 (all five groups pass).
Tested EBOOT SHA256: ca26ddeeb5dc90222c61878bf0c16435d5e9692988cc2dd887d984622989a658.
The exact tested binary is copied to dist/ELITE-NEXT-PSP/EBOOT.PBP.
