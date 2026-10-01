# Mega-capital architectural variants — 2.5.245
30 September 2026. Based on cumulative 2.5.244.

## Player-visible changes
Mega-capital stations now mix domed roofs, spherical habitation/reactor modules, long capsule pods, octagonal towers, bevelled towers and pyramid crowns with the existing advertising towers and service bridges. Each system's stable seed selects its proportions and architectural mix. Dome glazing, sphere equator bands, pod ribs, window lights and flat facet shading retain the existing pixel-rendered city style.

All nine capitals receive the new architecture. City component count remains 58, with the existing 72-part limit. The central core, five docking slits, clear forward departure corridor, 16km capital comms and normal docking services are retained.

## Geometry and safety
New mega-city-geometry.h supplies six bounded convex faceted templates, at most 40 faces per template (48-face/50-vertex capacity). The same normalized vertices and planes are used for visible geometry and collision. An enclosing box rejects distant sweeps; plane clipping then tests the actual shape. Empty corners around spheres/domes stay flyable. Docking routes, NPC avoidance and freighter clearance use that shared test. There is no per-frame mesh allocation.
The renderer transforms each visible curved hull's vertices once, rejects back faces and reuses the existing depth/occlusion pipeline and facade raster. Planar advertisements remain on box towers/tenders; curved structures receive windows, bands and facet shading.
No controls or save layout changed. Previous city, map/Codex, starport, wildlife and debug changes retained. These are station exterior modules, not new room interiors or player ships.

## Final verification
Snapshot and proof: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/capital-variants-245
- PSP SDK build succeeds: build-variants-final.log.
- variants-final/mega-city-review.txt: 292 checks pass, RESULT 0. Covers all nine capitals and 108 guided docking routes, manual port entry, city collisions, stable architecture after leaving/returning, template convexity, flyable bounding-box corners, all six shape types, native framebuffer guards and depth-state restoration.
- Native overviews captured for every capital (systems 0,12,17,48,83,156,184,228,236), plus close shape views including rolled cameras. Final dome/sphere/pod/bevel pictures inspected.
- map-final/field-map-review.txt: RESULT 0. Four-world map, Codex and button-release checks.
- integration-final/integration-241.txt: RESULT 0. Fuel/jump debug, map lifecycle and HUD controls.
- smoke-final/game-check.txt: RESULT 1, the same previously recorded ordinary-station swept-tunnelling assertion. No whole-game green claim; broad input/performance groups not completed.

## Performance and limits
Final native city view averages 30.79/25.21/29.81/31.18/39.52/40.80ms; worst sample 41.58ms, excluding audio/display. Some close views still exceed the 30FPS budget. Physical PSP frame pacing remains unverified.
Templates are intentionally low-facet geometry, not smooth high-resolution meshes. Shared generated templates add about 24.6KiB BSS over 244; final ELF text/data/BSS total 8,437,146 bytes (does not include all runtime allocations).
Next: hardware fly-through and performance profiling of close city views; independent repair of the inherited broad station-collision failure.

## Source and delivery
Canonical source matches tested snapshot ignoring line endings. New geometry header plus narrow layout/render/review/test changes only; version/build labels are 2.5.245. Existing dirty work, root/dist EBOOTs, saves/music and saved 244/recovery folders retained. No pull/autostash, commit, push, tag or public release.

Delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/ELITE-NEXT-2.5.245/EBOOT.PBP
7,202,743 bytes
SHA256 9F676AF0FB20E6EBB2145493D035A744CA2AD48B9EE0DEF6DE17CFA3D61926B5
All geometry embedded. Keep existing saves/music when replacing EBOOT. Do not install review flags or test saves.
