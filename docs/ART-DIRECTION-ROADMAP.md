# ELITE: NEXT — art direction completion roadmap

**Owner:** visual identity lane  
**Revision:** 22 September 2026  
**Status:** active production plan

**Release cadence:** BIG RELEASES ONLY. This lane batches visual upgrades into coherent art drops for the release agent. It does not create micro playable version tags or independently bump release versions.

This roadmap turns the visual identity direction into a sequence of evidence-based handoffs. It is intentionally implementation-aware but does not change station-crawl or ship code. Runtime art enters the game only after its native-size, performance and provenance gates pass.

## Definition of done

The art direction is complete when:

- planets, stars, stations, rooms, NPCs and UI read as one original world;
- a player can identify the focal object and safe next action at native 480×272;
- every room has a clear gameplay anchor, door/readable route and lighting key;
- the same seeded planet keeps its visual identity across map, approach, flight and surface;
- proposed art has been reduced to native pixels and tested in the real framebuffer;
- effects remain legible with high contrast/reduced effects enabled;
- station and ship implementation teams have source assets, dimensions, palette roles and cut priorities;
- screenshots show both the current executable and the proposed target without conflating them.

## Phase 0 — evidence and identity lock

**Goal:** establish the truth before repainting.

### Complete

- Read the current 480×272 UI and graphics contracts.
- Audited the station-crawl room inventory: HALL, HUB, CHANDLERY, BAR, CARGO BAY, CLINIC, GUILD and CUSTOMS.
- Locked the original visual thesis: period aerospace illustration and practical miniature lighting translated through late-1980s handheld graphics.
- Locked the palette, lighting rules, room grammar and current/proposed labeling.
- Captured the current title screen at native resolution.
- Created proposed station-approach and three-panel concept references.

### Remaining evidence

- Capture current command deck, cockpit, galaxy map, planet approach/surface and station crawl screens on a local PSP-input-capable emulator or physical PSP.
- Record one visual audit sheet per screen type: focal point, clutter, silhouette, text collision, palette drift and effect overload.
- Keep the current capture set immutable; never overwrite it with proposed art.

## Phase 1 — station room kit

**Goal:** give the station implementation lane a coherent room kit.

### Deliverables

- One 480×272 composition board for each of the eight runtime room types.
- Native-size wall, floor, trim, door, lamp, terminal, crate and sign primitives.
- Three depth treatments: near panel, middle prop, far destination.
- One NPC silhouette sheet covering trader, law, explorer and named Kei/Venn identity.
- Interaction marker sheet: selected, available, disabled, objective and danger.

### Acceptance gates

- One-second focal-object read at 1×.
- Three to five interactable anchors maximum per room.
- No room prop enters header, feedback or footer bands.
- All room labels fit without truncation.
- At most two animated light families.
- Station team confirms geometry can be simplified without losing focal object, palette role or route read.

### Cut order if over budget

1. Remove background decoration.
2. Reduce texture/dither density.
3. Reduce NPC animation frames.
4. Remove secondary light animation.
5. Preserve door silhouette, focal object, interaction cue and readable dialogue.

## Phase 2 — celestial identity pass

**Goal:** make planets and systems memorable without increasing simulation scope.

### Deliverables

- Six planet palette families aligned to existing biome/gameplay identities.
- Thumbnail silhouettes for ocean/temperate, red dune, ice, fungal, copper badlands and airless ruin.
- A star/nebula composition kit with three star-depth bands and seeded framing elements.
- Ring, atmosphere, cloud, surface-glint and sun-corona rules that share projected geometry.
- A world identity table recording palette family, phase, tilt, landmark shape and map/flight consistency.

### Acceptance gates

- The planet reads before its name is read.
- Approach, map and flight use the same palette family and body identity.
- Gas giants and suns remain visually distinct from landable bodies.
- Nebulae never compete with the objective or target marker.
- High contrast removes flare/vignette without removing the body silhouette.

## Phase 3 — station exterior and docking theatre

**Goal:** make stations feel monumental while preserving manual docking clarity.

### Deliverables

- Three original station silhouette families: ring-and-spine, clustered habitat and industrial truss.
- Prosperous, quiet and dangerous lighting variants.
- Docking aperture, cargo berth, beacon and maintenance detail primitives.
- Approach compositions with a clear sightline and tiny traffic scale cues.
- One external station paintover per silhouette family at native reduction.

### Acceptance gates

- Aperture is recognizable at approach distance.
- Cyan is limited to aperture/navigation; hull remains structural charcoal/slate.
- Traffic and windows communicate activity without becoming particle noise.
- Station edges remain separable from planet and sky.
- Docking collision/readability is not degraded by decorative geometry.

## Phase 4 — presentation integration

**Goal:** make the art direction survive the full game loop.

### Integration surfaces

- Intro/title and narrative panels.
- Command deck live-space preview.
- Full, minimal and scenic flight HUD.
- Galaxy map and route line.
- Planet approach, atmosphere and EVA.
- Station crawl and interaction cards.
- Comms portraits, radio and faction identity.
- Shipyard and equipment previews.

### Acceptance gates

- One shared palette role per signal: cyan navigation, amber objective/focus, red danger.
- NPC uniform, faction colour, portrait and dialogue colour agree.
- Scenic mode protects the world view; full mode protects information hierarchy.
- Every transition preserves system/body/station identity.
- No new art introduces text overlap or stale visual state behind a modal.

## Phase 5 — physical and release certification

**Goal:** prove the direction on the device it is designed for.

- Review all changed screens at 1× on physical PSP hardware.
- Check speaker/headphone contrast of visual feedback alongside radio/comms.
- Test reduced effects/high contrast and long-session eye comfort.
- Measure frame time and memory with the densest room, station exterior and planet scene.
- Record a final current-vs-proposed gallery with build/version labels.
- Add provenance for every external reference and source asset.
- Only then promote proposed art from direction board to runtime asset.

## Coordination protocol

The visual identity lane provides:

- the room/planet/station target;
- palette role and lighting key;
- native pixel dimensions;
- focal object and interaction hierarchy;
- cut order under PSP pressure;
- screenshot acceptance criteria.

Implementation lanes provide:

- actual draw-list and memory cost;
- collision/camera constraints;
- input and state-machine behavior;
- native captures and regression results.

When the two disagree, preserve gameplay clarity and the focal silhouette first. Record the compromise in the relevant visual handoff instead of silently changing the art language.

## Release batching rule

Art work stays in `PROPOSED` or handoff form until a meaningful group is ready: a room kit, a celestial pass, a cinematic composition set, or a complete screen-family polish. The release agent owns version bumps, builds, smoke tests, packaging and tags. A visual PR may land documentation and source art together, but it should not produce a standalone micro-release.

## Immediate next actions

1. **Done:** Build a native 480×272 room-kit sheet for the eight station rooms.
2. Capture the current station crawl once PSP input is available in the emulator or on hardware.
3. **Done:** Produce six planet silhouette variants and compare them against the existing seeded body families.
4. Produce three station exterior silhouettes with docking apertures.
5. Review each proposed sheet with the station implementation lane before runtime integration.

Execution artifacts and bake notes are in `docs/ART-KIT-HANDOFF.md`. The next implementation-facing artifact should be three compact station exterior silhouettes with docking apertures—not a large asset dump.

## v2.5.37 unification checkpoint

The art branch is now unified with tagged `v2.5.37`, which contains the story + MacVenture station tip and the station style-table bake. The release baseline was merged without taking ownership of `src/station-crawl.h`; the only merge resolutions were in the already-art-directed celestial presentation files, using the tagged release versions. The remaining visual work is authored pixel-scene composition and native screenshot review, not a second station runtime rewrite.
