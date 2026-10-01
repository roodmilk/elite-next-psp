# ArcElite 2.5.167 — Comms, ships and GalacticNet

## Scope and changes
- Preserve the working 2.5.166 local tree and previous same-day changes. No pull, reset, commit, push or GitHub release.
- Remove service 19 from Commander navigation without renumbering existing service IDs. COMFORT/COMMS legacy routes resolve to COMMS_PANEL.
- Seven existing channel actions followed by six former Display & chatter options, with two readable pages. Dialogue/rare-hail reply presentation is retained.
- Tutorial introduction and walk gate updated for new routing. Salvage requires an undocked player and a live anomaly within 1400 m.
- Catalogue indexed descriptions for all ten purchasable hulls. Previous mixed-case matching never matched uppercase ship names. Four full-width lines below the catalogue, balance on the title strip.
- Runtime hull detail replaces at most four upper triangles with recessed seven-triangle panels (at most 24 extra triangles per ship). No new textures, allocations or generated mesh edits. Enabled in previews and within 1800 m only; rocks, containers and stations excluded. Original hull silhouettes and collision sizes remain.
- Main menu preview uses the same detail helper. Planet transition cinematic retains its existing separate renderer; this is not a full fleet remodel.
- Sixteen newspaper stops retained; nine rows for articles, separated lower ad/classified columns. Eight 8x8 pixel commercial motifs, printed at 4x scale. Short notices/personal ads/letters vary by page and system. Existing Orbit Sudoku remains within the paper. These are flavour items, not new purchase or puzzle interactions.
- Wanted left rail maps the selected warrant across the five actual system targets. Existing target lock, reward and taken-down paths unchanged.

## Verification
- PSP cross-compiler and PPSSPP smoke suite used; native framebuffer captures reviewed for Shipyard, both Comms pages, newspaper front/story/puzzle and Wanted final sheet.
- Regressions cover all 13 Comms options across channel/action and navigation suites; all ten description bodies and the ads/classifieds fit their reading areas. Article fit is checked for all 16 stops in the test system.
- Existing gameplay, tutorial, save, radio, steering, Wanted lock/flight and performance tests retained. Final run smoke-20260927-092306-020: all five groups pass; reported average 56.51 FPS, worst frame 150.15 ms. Built/tested/delivered SHA256: 3369D58974F62FC9BEF4EC16E7AED5EF3D15AD1E3BDEFE7C1CED041109F392ED.
- No physical PSP available: hardware performance and long-session play still need device testing. Do not interpret the emulator pass as a no-glitches guarantee.
- Save format remains V18; no extra assets required for this update.

## System Details proposal — NOT implemented
Use a narrower body list and wider selected-body card; expand traffic abbreviations into Traders, Law, Pirates and Explorers. Show real body type/landing style, distance, local time, survey progress, active jobs and Track/Face actions. Keep undiscovered species hidden.
Source issues to correct in that future slice: fixed type labels do not follow generated planet types; gas giants are labelled non-landable despite platform exploration; briefing text can exceed the right panel. Preserve real navigation and per-world survey state rather than inventing summary figures.

## Next steps
Physical-PSP checks: visit each Comms option via FLY and held Triangle; read all catalogue entries; browse all news/Wanted stops; track and fight a posted target; compare near/far ships and all paint themes. Implement the System Details redesign only if requested.

