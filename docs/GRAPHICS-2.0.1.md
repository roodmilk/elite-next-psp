# Graphics validation — ELITE: NEXT 2.0.1

Tested on 21 September 2026 using the PSP cross-compiler and PPSSPP.

- Compiler: no warnings.
- Gameplay: 193 checks passed.
- Input/UI: 105 checks passed, including caption-zone, raster clipping and radar bounds assertions.
- Steering: 13 checks passed.
- Radio: 15 checks passed.
- 42-scene smoke: 59.94 average FPS; worst measured frame 17.34 ms; no measured frames over 25 ms.
- Native 480x272 frames inspected: station approach, long system/station names, close freighter, scenic mode, missile warning, atmosphere, planetary EVA, docking welcome and approach decision.
- Frame captures were performed in a separate run; capture disk overhead is not part of the performance result.

Confirmed fixes: wrapped target names; captions within their band; separate telemetry labels/pips/bars; all bearings represented by radar; no ordinary text in the central canopy; no floating relay labels; no scanline darkening over text; no preview edge leaks; shared mesh raster grid; no stale warp imagery behind decision panels.

Presentation additions: 8x8 HUD silhouettes, 16x16 life/mineral silhouettes, shaded freighter panels, solar granulation following star colour, soft clipped engine/port glints, muted lens ghosts and optional vignette.

Scope: this does not certify every generated scene or physical PSP frame time. Global depth sorting remains painter-based; there is no full depth buffer. Interiors and spacewalks remain early prototypes, not a completed room/exploration system. The original elite-derived assets and current planet/portrait art were retained.

Installation: copy EBOOT.PBP into the existing game directory. Preserve commander.sav, its backup and radio.cfg. Do not copy test flags from development folders.
