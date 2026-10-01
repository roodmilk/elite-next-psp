# 2.5.241 — Debug toggles, EVA map and HUD integration

30 September 2026. User explicitly requested combining the completed debug,
planetary-map and small HUD tracks with the cumulative 2.5.240 game.

## Included

- Runtime-only unlimited fuel and unlimited jump-range toggles in Debug Tools.
  Fuel protects boost and jump consumption; range permits direct travel to any
  other system. Each can be disabled independently. Saves do not store the flags.
- START opens a modal, fixed-grid planetary map while on foot or in the rover.
  START/Circle closes it; Triangle opens Field Guide. A 200-byte visited-cell
  mask reveals the current landing's route. Real terrain/water/cloud-deck data,
  ship, rover, port buildings and discovered-site markers are used.
- Compact exposure/suit and health meters, condition colours, reduced footer
  clutter, R movement hint and START MAP hint. Targeting/scanning still work.
- All prior cumulative planet environments, wildlife, POIs, station/TV/space
  presentation and actual-world first-person planetary transfers are retained.

## Integration corrections

Map input consumes held buttons until release after closing, so it cannot leak
into scanning, movement or ship interactions. It rejects dead/invalid contexts.
Map fog now survives boarding and stepping outside again during the same landing;
orbit resets it. Negative-edge grid coordinates use floor rather than truncation.
The R hint glyph is aligned inside its shoulder-button box at native resolution.

Existing specialist work was already in the shared source. An isolated copy was
built to avoid racing shared binaries. Narrow integration changes were then
applied back to the canonical source; final source-directory comparison matched.
No unrelated work was reverted and no Git pull/autostash, push or public release
was performed. Separate expanded TV/Crown of Dust content remains excluded.

## Exact delivery and proof

Workspace: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an

- Source/build snapshot: integrated-241/
- Player delivery: ELITE-NEXT-2.5.241/ELITE-NEXT-PSP/EBOOT.PBP
- 7,108,951 bytes; SHA256
  `7017267D1E5C413B65C47B69D794CE13A7EDCB4F1E950361DE480876551C4239`.
- integrated-241/focused-final/integration-241.txt: 47 checks, zero failures.
  Both debug states/disabled behaviour, save exclusion, per-Lave map input,
  fog lifecycle, release gating, guide navigation, dead-state prevention,
  HUD colours and framebuffer guards. Native debug/map/HUD BMP captures alongside.
- integrated-241/pilot-final/cinematic-port-review.txt: zero failures.
  Actual arrival/automatic disembark/boarding/launch input, button-spam safety,
  existing landing/EVA/save/reward checks and 64-world landmark clearance.
- integrated-241/worlds-final/: Lave II–IV activity/reachability/discovery/save
  checks and native render guards pass, both reports zero failures. Sampled
  update+draw averages II 26.097ms, III 29.106ms, IV 36.150ms; audio/display
  excluded. IV therefore remains over a 30-FPS frame budget.
- integrated-241/smoke-final/: game one failure (station swept-collision
  tunnelling); radio and steering zero failures; input completes with 184
  failures, matching the count reported by the pre-integration specialist run.
  These include equipment/module-bank and older dialogue assertions. Do not
  call the broad regression green. Broad performance group was not completed.

Physical PSP is not tested. Cinematic slow frames and broader existing test
failures remain. No new art assets, test flags or test saves are required in the
player install. Keep commander saves and custom music when replacing the EBOOT.

Next: physical-PSP map/readability and frame-pacing check, then investigate the
inherited broad failures independently. Do not silently roll expanded TV content
or additional environment work into this hashed delivery.
