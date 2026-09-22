# ELITE: NEXT — cinematic composition targets

**Status:** `PROPOSED` art-direction handoff  
**Revision:** 22 September 2026  
**Implementation boundary:** no edits to `src/station-crawl.h`

These targets translate the supplied warm optimistic science-fiction references into original ELITE: NEXT compositions. The generated key art is mood and staging reference, not runtime texture. Runtime implementation must reduce each target to a few readable PSP value masses.

## New proposed key art

- `elite_next_proposed_cinematic_station_concourse.png` — sunlit planetary concourse, domes/towers, landing platform, travelers and an original service craft.
- `elite_next_proposed_cinematic_orbital_station.png` — monumental orbital station above a striped planet, tiny courier ship and protected reticle sightline.

The source references establish a broad visual grammar only. Do not copy their characters, creatures, ships, architectural marks or compositions.

## Composition rules for A++ polish

### 1. Planetary concourse

- **Hero:** one bright practical doorway or landing platform at a one-third intersection.
- **Scale:** two tiny travelers, one craft, and one distant tower group.
- **Layers:** foreground silhouette / middle playable anchor / distant domes and haze.
- **Palette:** ochre sun, coral sky, cream walls, cobalt shadow, turquoise signal.
- **UI:** interface recedes into thin cream/cyan instruments; never cover the focal doorway.
- **PSP reduction:** 5–7 value masses, no small texture that does not survive 1×.

### 2. Orbital station approach

- **Hero:** original station silhouette with a readable docking aperture or habitat ring.
- **Scale:** one small courier craft crossing the foreground; sparse window lights.
- **Layers:** planet limb / station body / foreground ship and beacon.
- **Palette:** warm planet against cobalt/violet space; amber windows; cyan navigation only.
- **UI:** central sightline remains empty for reticle and selected-target brackets.
- **PSP reduction:** station reads in silhouette before windows or labels are drawn.

### 3. Strange life and vehicles

- Use one unexpected biological or mechanical shape per scene, not a crowd of noise.
- Make the silhouette friendly/legible first; details are two-tone accents.
- Keep creatures and ships original to ELITE: NEXT and tied to a place, mission or ecology.
- Use contrasting scale: a small person beside a large creature, tower or ship.

## Release handoff

After story and station implementation are unified:

1. Pick one focal object for each screen.
2. Place the focal object before any HUD or dialogue layer.
3. Reduce the background to three depth planes.
4. Add one practical light and one environmental fill.
5. Add only the signal colour required by the action.
6. Capture at native 480×272.
7. Compare `CURRENT` and `PROPOSED` captures side by side.
8. Run high-contrast/reduced-effects review before promoting the art.

If performance or text safety conflicts with spectacle, cut background decoration first. Preserve the focal silhouette, route/interaction read, and objective visibility.
