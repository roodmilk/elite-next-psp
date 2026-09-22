# ELITE: NEXT — native art kit handoff

**Status:** proposed reference; style bake shipped in `v2.5.37`  
**Resolution:** 480×272 exact  
**Runtime status:** not integrated; do not treat these boards as current game screens

## Artifacts

The companion media folder contains:

- `station-room-kit-proposed.svg` / `.png` — eight room compositions: HALL, HUB, SHOP, BAR, BAY, CLINIC, GUILD and CUSTOMS.
- `celestial-identity-kit-proposed.svg` / `.png` — six planet identity families plus the star-depth key.

Both PNGs are already rendered at exact 480×272. SVGs remain the editable source boards. The boards are intentionally presentation references, not runtime textures.

## v2.5.37 release status

The tagged unified release now carries the station preview crops, provenance record, `station-art-kit.h` style table and the ART DIRECTOR bake path. This is the correct handoff boundary:

- the station lane owns room geometry, navigation and interaction;
- the bake lane owns palette/style-table conversion and small preview assets;
- the visual lane owns composition, palette roles and future authored pixel scenes;
- `src/station-crawl.h` remains outside this art lane.

The release uses styles and small props rather than embedding eight 480×272 bitmaps. The full boards remain `PROPOSED` references for future composition polish.

## Bake targets

### Shared palette

```text
VOID       #080D18
CHARCOAL   #151C27
SLATE      #293646
OLIVE      #5A604C
RUST       #8B4B37
OCHRE      #C18B4D
CREAM      #E5D2A3
LAVENDER   #9B9AA5
CYAN       #55D4D4
AMBER      #F0B45B
DANGER     #C85A4B
```

### Room kit

- Use each panel as a composition target, not a literal full-screen background.
- Preserve the focal object, room palette role and route/door silhouette when simplifying.
- Keep 3–5 interactable anchors per room.
- Use one warm practical key plus one cool/room-specific signal family.
- Convert small markers and labels to native 8×8 or 16×16 assets; no smoothing.
- Keep NPCs as 32×48 or 48×64 silhouettes with a contact shadow.
- Keep the room centre free for navigation and the existing top/bottom text bands.

### Celestial kit

- `01` blue limb/cloud slash/moon: ocean or temperate.
- `02` copper crescent/dust bands: red dune.
- `03` pale cap/cyan rim: ice shelf.
- `04` green haze/dark islands: fungal plain.
- `05` warm face/rust ravines: copper badlands.
- `06` no atmosphere/one relic light: airless ruin.

The family is a visual identity layer only. Existing seeded body names, gameplay biome logic, phase, tilt and map/flight consistency remain authoritative.

## Native review gates

- [ ] Readable at 1× without emulator enlargement.
- [ ] Silhouette and focal object survive a 4–7 value reduction.
- [ ] High contrast/reduced effects preserve identity.
- [ ] Text remains in existing safe bands.
- [ ] Proposed/current label is present in capture documentation.
- [ ] Runtime bake records source, palette, dimensions, memory cost and frame-time result.

## Coordination boundary

This handoff deliberately does not edit `src/station-crawl.h`. The station implementation lane can use these targets while retaining its own geometry, input and state-machine decisions. If a target conflicts with performance, cut background decoration first and preserve the focal object, route read and interaction cue.
