# ELITE: NEXT — station room visual targets

**Status:** proposed art handoff, 22 September 2026  
**Audience:** station-crawl implementation lane and future asset work  
**Runtime target:** PSP 480×272, fixed-size geometry/sprites, native nearest-neighbour sampling

This document is based on the current station-crawl room inventory on the implementation branches. It does not modify or replace `src/station-crawl.h`. Room names and gameplay roles are implementation evidence; the visual treatments below are proposed targets.

## Room inventory

The current station crawl defines a connected 5×4 grid containing these room types:

| Runtime room | Player-facing name | Function | Visual focal object |
|---|---|---|---|
| `SC_ROOM_EMPTY` | HALL / CORRIDOR | traversal and breathing space | long perspective, conduit boxes, destination door |
| `SC_ROOM_HUB` | ARRIVALS / HUB | Venn, dockhand, traffic and return-home context | arrivals board or berth window |
| `SC_ROOM_SHOP` | CHANDLERY | exclusive stock, mechanic and equipment | lit parts counter with stacked crates |
| `SC_ROOM_BAR` | CANTEEN / BAR | rumours and passenger/taxi interaction | warm service counter and table lamps |
| `SC_ROOM_BAY` | CARGO BAY | loader tip job and freight identity | pallet/crate lane with cargo lift |
| `SC_ROOM_CLINIC` | CLINIC | medic and recovery utility | illuminated treatment bench |
| `SC_ROOM_GUILD` | GUILD DESK | Kei, surveyor and story context | green survey terminal / evidence wall |
| `SC_ROOM_LOCK` | CUSTOMS / SECURITY | law context and restricted-goods warning | red inspection gate / scanner |

The runtime also uses an economy-dependent substitution between shop/bar and shop/bay positions. The art must preserve each room’s silhouette and palette identity even when its location changes.

## Shared PSP room grammar

Every room should be composed as a small illustrated set, not a texture-heavy 3D environment:

1. **Back plane:** one broad wall colour with a single architectural shape.
2. **Middle plane:** focal desk, hatch, counter, terminal or berth.
3. **Foreground plane:** floor seams, threshold, crate or shadow that anchors the camera.
4. **Light key:** one practical warm lamp or room-specific coloured light.
5. **Navigation read:** door, label and threshold must remain distinct at 1×.
6. **Interaction read:** 3–5 anchors maximum; each has a silhouette plus a tiny edge/light cue.

Use the established visual authority: charcoal/navy voids, slate construction, dusty olive utility panels, rust wear, cream labels, cyan navigation and amber focus. Keep routine text in the existing footer band and leave the centre clear of long copy.

## Room-by-room targets

### HALL / CORRIDOR

- **Composition:** strongest vanishing point, with one far hatch and two side alcoves.
- **Materials:** slate bulkhead, olive service conduit, cream directional plate, rust at floor seams.
- **Lighting:** one overhead warm strip; a faint cyan line only at the safe route.
- **Anchor:** far door or side door; no decorative NPC crowd.
- **Avoid:** repeating checkerboard floor that reads as a game grid rather than a physical deck.

### ARRIVALS / HUB

- **Composition:** offset arrivals board on one side, berth window or large docking slit on the other.
- **Materials:** more maintained than other rooms; cream board frame, cyan berth indicators, ochre lamps.
- **Lighting:** cool exterior spill through the window plus one warm board light.
- **Anchors:** arrivals board, berth/traffic window, Venn, dockhand.
- **Scale cue:** a tiny ship silhouette crossing the window or a moving cargo pallet.

### CHANDLERY

- **Composition:** central counter at the one-third line; stacked parts create a diagonal toward the shopkeeper.
- **Materials:** ochre/rust crates, dark slate shelves, cream inventory tags.
- **Lighting:** warm counter key with a cyan inspection lamp on the selected item.
- **Anchors:** counter, exclusive-stock item, mechanic, spare-clamp crate.
- **Readability rule:** selected equipment gets an amber edge and a simple 8×8 category mark; do not rely on item colour alone.

### CANTEEN / BAR

- **Composition:** long counter as a horizontal anchor, with two separated tables or booths.
- **Materials:** dusty olive wall panels, plum/rust upholstery, cream cups and menu cards.
- **Lighting:** warm amber counter pools; keep corners dark so the room feels lived-in.
- **Anchors:** bartender, rumour table, traveller, passenger berth prompt.
- **Tone:** sociable and slightly worn, never neon nightclub or generic cyberpunk.

### CARGO BAY

- **Composition:** cargo lane recedes toward a loading door; foreground pallets frame the player.
- **Materials:** ochre hazard stripes, slate lift, rusted clamps, cream manifest tags.
- **Lighting:** cool overhead industrial strips, amber loading beacon at the far door.
- **Anchors:** loader, marked crate, manifest terminal, cargo lift.
- **Scale cue:** one crate should be large enough to imply the ship’s cargo capacity.

### CLINIC

- **Composition:** treatment bench or med station centered slightly off-axis, with a narrow privacy screen.
- **Materials:** cool slate, lavender-grey medical surfaces, cream instrument marks, restrained cyan.
- **Lighting:** cool cyan task light plus a low warm utility bulb; no full-screen sterile white.
- **Anchors:** medic, treatment bench, medkit, diagnostic panel.
- **Accessibility:** clinic cyan must remain distinguishable from guild green through shape and panel trim, not hue alone.

### GUILD DESK

- **Composition:** evidence wall or survey terminal behind a single desk; keep Kei and surveyor separated.
- **Materials:** sage/dust olive survey gear, cream paper strips, cyan data window, amber mission pin.
- **Lighting:** quiet green/cyan task light with a warm desk pool for faces.
- **Anchors:** Kei, surveyor, evidence panel, tracked-mission terminal.
- **Story rule:** the room should feel like an investigation workspace, not a generic quest kiosk.

### CUSTOMS / SECURITY

- **Composition:** scanner gate creates a hard vertical frame; officer sits behind or beside it.
- **Materials:** dark navy, red/rust warning bands, cream legal placard, cyan scanner aperture.
- **Lighting:** narrow red inspection pulse plus neutral overhead fill; red is reserved for law/danger.
- **Anchors:** customs officer, scanner gate, restricted-goods sign, inspection terminal.
- **Tone:** firm and procedural, not villainous. Law identity stays navy/cyan outside the warning hardware.

## Characters and dialogue

Station NPCs should be readable as graphic adult silhouettes before their portraits are read:

- 32×48 or 48×64 billboard with a contact shadow.
- Uniform and equipment carry faction identity; face colour is secondary.
- Venn remains a civilian dockmaster, Kei remains the named explorer, and the medic/customs officer remain role-specific.
- NPC selection uses a fill plus edge/halo. Gold is focus; faction colour is identity.
- Dialogue panels remain blue for named NPCs and orange right-tailed panels for the player.

## Camera and controls

The current station crawl uses first-person facing, turn/move, action and deck return. Visuals must support that model:

- Side doors need a strong near/mid depth distinction.
- The destination door owns the visual centre; do not place a large NPC over it.
- The minimap is orientation support, not a substitute for visible doors.
- Keep the top header and bottom feedback/footer bands free of room props.
- Turning should change the composition enough to reveal the next anchor, without requiring smooth camera rendering.

## Implementation handoff checklist

Before a room is called art-complete:

- [ ] Silhouette reads at 480×272 and in reduced-effects mode.
- [ ] Focal object is identifiable in under one second.
- [ ] Every interaction has a distinct shape/edge cue.
- [ ] NPC names and room labels fit the existing text bands.
- [ ] No prop overlaps the reticle, header, feedback or footer.
- [ ] At most two animated light families are active.
- [ ] Asset source/provenance and native baked output are recorded.
- [ ] Frame-time and memory impact are measured in the station scene.
- [ ] Current implementation and proposed art are labeled separately in review captures.

## Coordination note

The station code lane owns implementation details and may simplify or rearrange geometry for performance. If a room must be reduced, preserve the focal object, palette role, practical light source and interaction silhouette first; background decoration is the first thing to cut.
