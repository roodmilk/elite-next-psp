# 2.5.246 — Pulp-era station exteriors

Implemented 30 September 2026, starting from the complete saved 2.5.245 source/assets. The request was for substantially different 1950s-science-fiction silhouettes, not additional satellite-like boxes attached to a common Coriolis.

## Player-visible result

All 256 primary stations retain their economy class and services. The 247 ordinary primaries now use eight deterministic whole-hull families: continuous habitat wheel, twin saucer, globe citadel, rocket cathedral, asymmetric orbital liner, double wheel, crystal palace and stacked orbital palace. Existing seeded dimensions, palettes and slower rotation vary their proportions. Broad connected inhabited masses replace small decorative attachments.

The nine mega-capitals retain their city districts and five entrance slits, but now have dominant connected crowns: broad saucer, twin globes, rocket palace or asymmetric liner. All four crown families actually occur in the current galaxy. Tiny relays and frontier outposts retain their earlier compact geometry; this is not a claim that every auxiliary structure has been redesigned.

The approach took inspiration from the wheel habitats and dramatic connected forms of period cover art, rather than copying cover pixels or importing external assets:

- [1952 Von Braun/Bonestell wheel-station concept, NASA image metadata](https://commons.wikimedia.org/wiki/File:Von_Braun_1952_Space_Station_Concept_9132079_original.jpg).
- [Cover-art reference collection, including Imagination 1953, Space Platform 1953, Spaceway 1954 and Galaxy 1958](https://sciencefictionruminations.com/2013/03/31/adventures-in-science-fiction-cover-art-the-torus-space-stationhabitat-part-i/).

This is live flat-shaded, faceted 3D geometry with procedural hull detail at native 480x272, not a high-resolution painted backdrop. Actual flight captures are included in the proof directories below.

## Code and safety

- `station-architecture.h`: bounded cached primary-station layouts, seeded eight-family builder, wheel segments/spokes and shared collision lookup.
- `mega-city-geometry.h`: axial bevelled docking hull and twelve convex annulus segments. Adjacent ring sections share their boundary; the wheel quadrants are genuinely empty, not an invisible solid disk.
- `mega-city-layout.h`: four large capital crowns, kept aft of the existing front entrances; maximum 62 of the existing 72 component slots.
- `mega-city-render.h` / `voyage.h`: same solids rendered with local camera/UV transforms, stronger facet lighting and world-attached windows. Ordinary station rotation is handled by the depth and texture basis. Removed the old floating lamp overlay from the new renderer.
- `docking.h` / `mega-city-guidance.h`: all primary stations use swept shared-hull collision and obstruction-checked guidance. Ordinary stations use the existing bounded lattice at 200m spacing, capitals at 900m. Guided waypoints follow the ordinary hull's rotation and align the ship's roll before entry. Failed searches leave control with the player; this path does not use the legacy seam-recovery teleport.
- `npc-steering.h` / `freight.h`: light traffic uses the new rotated solids for look-ahead and swept clearance. Main-hub freight berths account for the larger envelope, with routes checked against the architecture.
- Identity/menu family names and catalogue tests updated; dedicated opt-in native geometry/render checks added. No controller remapping, save fields, commander files, shop contents, planet environments, music or dialogue changes.

The old broad station-tunnelling test's hard-coded 3340m plane was replaced with the actual generated front plane and an outside-to-outside sweep. Reliability tests now use generated front/side starting positions: the old +/-500m start lay inside the new Lave wheel. Entry-edge, inside-start rejection and collision requirements are retained, not waived.

## Exact delivery and proof

Player folder: `C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/ELITE-NEXT-2.5.246/`.

EBOOT.PBP: **7,236,207 bytes**. SHA256: `DF2B750F2BB5C7441F5AF769071BF6359793D7115985421A5D93D7DE4A03860D`.

All new geometry is embedded; replace only EBOOT.PBP in the existing PSP game folder and keep saves/music. This is cumulative 245, including the planetary airport, wildlife, first-person transfers, fuel/jump debug toggles and Field Map work. Root/dist binaries were deliberately not replaced.

Source/build/proof snapshot: `C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/pulp-stations-246/`. Canonical source/build targets match this tested snapshot, ignoring line endings.

Final exact-binary reports:

| Directory / report | Result |
| --- | --- |
| pulp-catalogue/pulp-station-review.txt | 1,541 passes; zero failures. All 247 ordinary primaries, 494 rotating/front/rear guidance routes, manual entries, wheel gaps, stable revisit, native family views and framebuffer/depth guards. |
| capitals-catalogue/mega-city-review.txt | 293 passes; zero failures. All nine capitals, 108 guidance routes, convex-hull collision, catalogue crown coverage, native captures and renderer-state restoration. |
| map-catalogue/field-map-review.txt | 45 passes; zero failures. |
| integration-catalogue/integration-241.txt | 47 passes; zero failures. |
| smoke-catalogue/game-check.txt | 594 passes; zero failures, including dynamic station collision, law, missions and save/recovery checks. |
| smoke-catalogue/steering-check.txt | 18 passes; zero failures. |
| smoke-catalogue/radio-check.txt | 48 passes; zero failures. |

Earlier pod-ring geometry, rejected candidate runs and the shadowed local variable in the first diagnostic macro are superseded, not release evidence. Final geometry uses continuous ring sectors. All reviews used new isolated directories and stopped only their own hidden emulator instance. No review flags or test saves are in the player folder.

ELF text/data/BSS total: **8,508,394 bytes**, about 71KiB above 245. Bounded component and guidance arrays are retained; no runtime cover-image loading or per-frame path searches were introduced.

## Limits and next steps

Native ordinary overview/oblique samples average roughly 21.2–27.6ms, worst 28.0ms. Capital flight samples average 25.9–44.4ms, worst 45.0ms. These exclude audio, display waits and capture IO: this is NOT a locked-30-FPS or physical-PSP certification. More expensive capital close views still need profiling and hardware fly-throughs.

The full inherited input/performance suite was not completed or certified. Passing the listed groups does not imply whole-game regression freedom. Station interiors are unchanged, miniature almanac art was not re-authored to match each new hull, and auxiliary relays remain a future separate geometry/berth task.

Next: physical PSP close-view/manual-port/boost fly-through checks; capital rendering optimisation; then matching miniature art and auxiliary exterior families with their own shared collision/berth implementation. Shared dirty files and recovery folders were preserved. No pull/autostash, commit/push/tag or public GitHub release was performed.
