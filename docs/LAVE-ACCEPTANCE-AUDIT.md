# Lave art acceptance audit

Audit result: **FAIL — blocked from activation**

Scope: Lave station rooms and ship/targeting visuals on the PSP-native Build 2.5.53 proof baseline. This is an evidence audit only; it makes no runtime activation or capability change.

## Evidence reviewed

- `tools/validate-native-art.ps1` — PASS: `station_arrivals` 340x168 at (6,20), `planet_approach` 480x168 at (0,24), and `campaign_dialogue` 448x68 at (16,54).
- `tools/test-station-cue-kit.ps1` — PASS: 48 native assets, 7 room bindings, normal/high-contrast proof sheets, canonical 11-role palette.
- `src/generated/station-arrivals-authored-pixels.h` — native 340x168 packed room payload exists for the Arrivals authored renderer.
- `src/station-crawl.h` — Lave (system 7) is not one of the authored Canteen exceptions; Reorte H0, Qube H0, and Xequerin H0 are the only active family dispatches.
- `src/procedural-room-plan.h` — `PROC_ROOM_KIT_RUNTIME_READY` is `0`; procedural family dispatch fails closed and does not expose placeholder primitives.
- `src/menu-ship-preview.h` — ship preview is native-grid camera-space geometry with bounded clipping and no simulation writes.
- `src/ui-modern.h` — targeting computer uses native bezel/scope geometry, stable contact IDs, status text, and explicit target actions.

## Acceptance matrix

| Area | Result | Reason |
|---|---|---|
| PSP framebuffer/layout | PASS | Native screen and room bands validate at 480x272 / 340x168. |
| Canonical palette and contrast kit | PASS | Cue-kit validator passes normal/contrast assets and 11 palette roles. |
| Lave Arrivals art | PASS with scope limit | Native authored Arrivals payload exists; this is not proof of a full Lave station family. |
| Lave Shop/Canteen/Cargo/Guild/Clinic/Customs authored art | FAIL | These rooms currently use the generic renderer on Lave; no Lave-specific authored family is active. |
| Procedural room art truth | PASS | Fail-closed runtime flag prevents placeholder procedural accents from implying finished art. |
| Ship preview geometry | PASS static / FAIL evidence | Native-grid bounded renderer exists, but this audit has no integrated native capture or emulator performance report. |
| Targeting visuals | PASS static / FAIL evidence | Native scope/status/selection code exists, but no Lave-specific normal/contrast capture and integrated report were supplied. |
| Anchors and capabilities | PASS guard / BLOCK promotion | Existing Station/Gameplay graph remains authoritative; no art activation is permitted without captured hotspot/input evidence. |
| Budget | PASS static / BLOCK integrated | Native scene validator passes; integrated frame-time, temp-memory, and final EBOOT measurements are still missing. |

## Exact blockers

1. Produce and integrate a Lave-native family set for the six non-Arrivals rooms, or explicitly keep them labelled as generic fallback until such a set is approved.
2. Capture Lave station entry, room traversal, person selection, speech/result, disabled, and SHIP return in both normal and high-contrast modes.
3. Capture ship preview and targeting screens at native resolution from the integrated Build 2.5.53 EBOOT.
4. Supply deterministic-repeat, final packed-budget, draw-count, frame-time, and temp-memory evidence.
5. Have Station, Gameplay, Story, and Lead sign off that visible anchors map only to existing Lave capabilities.

Until these blockers are closed, the correct product state is: authored Arrivals where verified; generic fallback elsewhere; no claim of full Lave art acceptance.
