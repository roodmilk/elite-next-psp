# Deep Chart visual pass — 30 September 2026

## Scope and shared-checkout safety

This is a local galaxy-chart presentation update, not a new galaxy simulation or planetary change. Existing L/R zoom, analog pan, prefix search, route plotting and the flight route-star integration remain in place. No depth stalks or rotate-help banner. Source edits were applied narrowly around the chart in the shared checkout; no autostash, reset, pull, commit, push, tag or shared release-binary replacement was performed. Other contributors' unfinished changes remain intact.

## What changed

### Nine-filter rail

Square cycles ALL, VISITED, UNVISITED, RICH, POOR, MEGA, JOBS, IN RANGE and ROUTE. The top-left panel is 90×86 pixels (13 pixels wider than the previous four-category panel, same height), with five 16-pixel rows, native 5×7 glyphs and a proportional scrollbar. Label exclusion bounds follow the slightly wider panel. The selected filter is always in the visible five-row window; wrapping returns to ALL.

Visited filters read the saved visit bits; station-class filters read actual station classifications. JOBS uses accepted board contracts (origins/destinations), active saga travel and carried passengers, not unaccepted offers or a manually plotted trip. IN RANGE uses current fuel and fitted ship range, including the gameplay fuel-boundary tolerance. ROUTE shows the plotted journey's stops, not a different highlighted preview route. Current location and the plotted destination remain visible for orientation. Empty categories show NO JOBS, NO ROUTE or NO MATCHES; no new bookmarking system is introduced. Selecting a search result outside the current filter switches to ALL so the searched-for star remains visible.

The opt-in review now includes all nine filter screenshots and membership/fuel/empty-state/scroll-window regression fixtures in `src/deep-chart-filter-tests.h`. No save-format or planetary-system changes.

- Removed the decorative point-star/dust field. Background illumination is now a starless, authored spiral-galaxy texture; point markers represent real systems and actual route hops.
- Added diffuse additive star halos, brighter route stops, yellow current-system and pink highlighted-system markers. Kept real system positions, distances and navigation data rather than inventing locations to imitate the concept.
- Rebuilt glass panels and introduced chart-specific native-size lettering. VISITED fits its highlight. Selected names, labels, status text, search keyboard and search results are more legible. Search has a full-width, non-overlapping icon footer.
- Kept a subdued committed route while previewing the highlighted destination. Its jump count and route are now computed together with a cached bounded breadth-first traversal, avoiding repeated large Game copies during drawing.
- Constrained map labels to avoid fixed panels and each other. The visible-node count excludes stars covered by the panels.
- Removed an obsolete unsafe chart capture from input_tests that attempted drawing without allocating a framebuffer. The new explicit chart-review flag allocates its own guarded buffer before audio initialization and never reads/writes commander saves.

## Files

- `src/ui-modern.h`: chart rendering, route cache, search presentation.
- `src/deep-chart-art.h`: native lettering, embedded-art sampling, glow, glass, bounded labels.
- `src/generated/deep-chart-nebula.h`: generated 512×256 RGB565 texture, 256 KiB constant storage; no runtime asset loading or heap allocation.
- `src/main.c`: chart projection adjustment and opt-in review entry point.
- `src/deep-chart-review.h`: native screenshots and focused regression checks.
- `tools/bake-deep-chart.ps1`: reproducible bitmap-to-header conversion; 2-pixel diffuse background shading leaves all stars, lines and text at native resolution.
- `tools/build-chart-candidate.ps1`: compiles a private source snapshot into a unique outputs folder; does not replace shared build objects or release binaries.
- `tools/review-deep-chart.ps1`: runs only its own hidden disposable emulator instance, exports native captures and reports test failures.

## Art provenance

Created with the built-in image-generation tool, not a mock screenshot. Original source is preserved at `assets/source/deep-chart/nebula.png`; baked preview is `assets/preview/deep-chart/nebula-native.png`. Final on-screen destination markers, routes, lettering and controls are actual game rendering, not painted into the image. The reference informed palette, atmosphere and panel hierarchy; its obsolete stalks/rotation instructions were deliberately not restored.

Generation prompt:

> Use case: stylized-concept. Create a production game background texture, NOT a screenshot or interface. Wide landscape 2:1 composition. A beautiful deeply atmospheric blue spiral galaxy seen obliquely as a broad flattened ellipse against almost-black navy space. Galaxy nucleus centred exactly halfway across and halfway down, arms extend across 90 percent of width and 60 percent of height. Horizontal major axis with very slight diagonal tilt. Rich fine wispy interstellar dust, smoky turbulent blue filaments, cold silver-blue diffuse luminous core, near-black dust lanes, textured and ethereal like deep astrophotography but art-directed for a premium retro sci-fi galaxy navigation chart. Keep overall exposure dark, readable UI and bright selectable destinations will be drawn over it by the game. Critical: NO isolated stars anywhere, NO sharp dots, NO sparkles, NO point lights; show only diffuse unresolved galactic glow and continuous detailed clouds. No rings, no orbits, no lines, no UI, no text, no planets, no borders, no symbols. Outer edges fade smoothly to uniform near black #02050b. Must remain beautiful and textured when baked down to 512x256 native pixels. Save as project asset suitable for a PSP game. This is the galaxy light layer only.

## Verification and limits

### PSP feedback: camera response and font

The 6×9 body and 8×12 selected-name text were non-integer stretches of 5×7 source glyphs, duplicating some strokes unevenly. All small chart text now uses exact 5×7 pixels; only the header uses an exact 2x scale. Label widths follow the new 7-pixel character advance. No fractional text scaling remains.

Vertical analog chart panning is inverted; horizontal motion and flight controls are unchanged. Chart input uses its own wall-clock delta (1–150 ms), separate from the physics loop's 50 ms cap. This preserves hold-to-zoom speed during slower frames while bounding suspend/stall camera jumps. World simulation still uses its existing timestep.

The renderer now checks filter membership once per system rather than twice. Nebula blending interpolates red/blue in parallel integer lanes, preserving the original pixels with less arithmetic. Regression checks compare 512 color/fraction combinations with the old formula. No added texture memory or reduced drawing resolution.

Instrumented baseline: `outputs/chart-build-20260930-003155-277/EBOOT.PBP`; report `outputs/chart-review-20260930-003406-559/chart-review.txt`. All nine filters were measured through the same 16 zoom frames: ALL 23.65 ms/frame, JOBS 21.66 ms/frame, all filters ending at zoom 1.505. The user's Jobs-specific hardware slowdown was not reproduced by this emulator fixture; do not claim a proven Jobs-only root cause.

Latest response/font candidate: `outputs/chart-build-20260930-003414-752/EBOOT.PBP`; report and captures `outputs/chart-review-20260930-003521-506/`. All 26 checks pass. ALL zoom draw 19.51 ms/frame; JOBS 17.41 ms/frame (~20% less drawing time); all nine filters still reach identical zoom 1.505. The 10/60 FPS timing test reaches the same zoom goal after a one-second hold, stall delta caps at 150ms, both vertical nub directions are tested, exact native A glyph pixels checked, and 512 blend values equal the old arithmetic. Native Jobs/Unvisited/search captures visually inspected. No extra texture memory, reduced background resolution, flight-control changes or planetary edits. Physical PSP confirmation still needed. Earlier filter/visual candidates follow for provenance.

Latest nine-filter candidate: `outputs/chart-build-20260930-002531-098/EBOOT.PBP`. Review/captures: `outputs/chart-review-20260930-002650-091/`. All 22 focused checks pass, including category partitions, accepted-job membership, exact/current fuel boundaries, route membership, empty route, all nine filter transitions and selection visibility. Native captures inspected for longest label, middle scroll and final scroll. Warm all-systems rendering: 23.79 ms in PPSSPP. This supersedes the visual-only candidate below; shared root/dist binaries were not replaced.

Earlier visual-only candidate: `outputs/chart-build-20260930-001230-933/EBOOT.PBP` (v2.5.231 source snapshot). SHA256: `77B32E6BEC4BAB42FF48B4E742E39B9D186C7963BDC39FD7B1EBEEF3FFEC372E`.

Final review: `outputs/chart-review-20260930-001351-721/`, nine PASS checks, RESULT 0 failures, warm all-systems draw 23.71 ms in PPSSPP. All four final native PNGs inspected, including search. No physical PSP measurement. Generated artwork is embedded, so the EBOOT is the only replacement game file needed; preserve existing music and saves.

Focused native emulator review checks prefix matches, empty/cleared search, cached routes against the existing gameplay planner, zoom in/out, analog pan, plotted route-star selection, and framebuffer guards across extreme zoom/pan and filters. Captures cover ALL, MEGA, zoomed route and search. This does not certify all gameplay systems or physical PSP performance. Earlier shared smoke failures and campaign stalls are outside this visual task and must not be represented as fixed.

Run `tools/build-chart-candidate.ps1`, then pass the resulting EBOOT path to `tools/review-deep-chart.ps1 -Eboot <path>`. Never copy chart-review.flag to a normal PSP installation: it is an explicit test-only startup mode.

Next: verify legibility, analog scrolling and frame pacing on a real PSP; use the full integration smoke suite before a coordinated shared release. Keep user music and commander saves untouched when deploying the candidate EBOOT.
