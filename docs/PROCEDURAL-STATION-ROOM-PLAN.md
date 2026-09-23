# Procedural station room art plan

This is the approved direction for making many stations look authored without storing a bespoke full-screen bitmap for every room.

## Three-manager split

### Art 1 — art direction and integration QA

Owns the canonical palette/layout contract, Second Shift benchmark, authored exceptions, family review, native 1:1 readability and final cohesion checks. Art 1 decides whether a generated family still looks like the same game. Art 1 does not own the procedural selector or every room draw call.

### Art 2 — procedural room composer

Owns deterministic assembly of room plans from stable station identity, room type and family. The composer chooses structural modules, depth ordering, lamp treatment, window/body treatment and constrained wear. It emits element references and native placements, not a new bitmap per station. It must preserve runtime-owned hitboxes, options, speech and SHIP rectangles.

### PSP UI & Art Reusable Element Kit

Owns the authored native pieces that make compositions feel rich: people and poses, counters, benches, lamps, windows, panels, doors, racks, displays, signs, dice and mission props as assigned. Every piece has a native size, pivot/anchor, palette-role mask, high-contrast treatment and measured bytes. Footer/help/chrome remains a Team-allocated responsibility between the named PSP UI & Art groups.

## Shared data boundary

Systems must publish a versioned deterministic selector before broad generation. Proposed read-only descriptor (names subject to Systems approval):

```text
StationArtIdentity {
  galaxy_id, system_id, hub_index,
  room_id, family_id, selector_version,
  stable_cosmetic_seed, condition_tag
}
RoomPlan {
  element_id, variant_id, x, y, layer,
  palette_mode, anchor_flags, hitbox_policy
}
```

`stable_cosmetic_seed` may select a lamp pattern, panel arrangement, material wear or window dressing. It must never select NPC truth, mission state, economy, collision, rewards, persistence or gameplay RNG. Replaying the same identity/version produces the same pixels.

## Composition recipe

Every room is built in the same order:

1. Structural silhouette: walls, arch, window/hatch and floor perspective.
2. Hero focal object: arrival board, assay bench, ledger, console, repair bay or notice wall.
3. Middle depth: rail, counter, racks, booth, lockers and authored people.
4. Functional lights and small material clusters.
5. Runtime overlays: options, speech, selected outlines, warnings and SHIP.

The family changes massing, focal object and light logic. A palette tint alone is not a station variant. Generated placements must keep the same safe bands and must not cover a required hotspot.

## Initial kit target

Start with a small cross-family kit, then expand only after native review:

- 3 person silhouettes with 2 poses each;
- 3 counters/benches;
- 4 lamps/worklights;
- 3 window/body treatments;
- 4 panels/boards/signs;
- 3 racks/crates/lockers;
- 2 doors/hatches;
- 2 small activity props (dice table and mission/manifest prop).

The pieces should be authored at native pixel sizes, packed with shared palette roles and reused by reference. Do not create a 340×168 texture for every room instance. The existing 4-bit native room candidate is 28,560 bytes plus palette; modular plans should be materially smaller than a room bitmap once the kit is shared, but Systems must measure the actual compiled result rather than accept a paper budget.

## Family assembly examples

| Family | Structure | Kit emphasis | Room variations |
|---|---|---|---|
| Freight | ribbed concourse, berth window, rail | arrival board, manifest, work lamp, cargo clerk | berth traffic, cargo lane, timetable wear |
| Prospectors’ Rest | low ceiling, assay wall, lander window | ore rack, scarred bench, sample lamp, prospector | sample layout, bunk, dust pattern |
| Research Lounge | observation arch, console wall | instrument, specimen cabinet, cyan lamp, operator | body window, cabinet arrangement, quiet booth |
| Merchant Salon | curved counter, ledger rotunda | display rack, booth, brass lamp, broker | stock wall, private booth, manifest composition |
| Frontier Mess | patched hatch, utility wall | repair bench, ration panel, locker, mechanic | patch layout, tool rack, service light |
| Outlaw Den | screened booth, guarded hatch | coded notice, card table, shaded lamp | notice arrangement and wear; danger red only when gameplay says danger |

## Memory and performance strategy

- Keep authored elements packed and shared; room instances store small plans, not copied pixels.
- Prefer 4-bit indices plus the canonical palette for static pieces where the runtime path supports it; use ARGB1555 only where the existing renderer requires it.
- Avoid per-room allocations and per-frame generation. Build deterministic plans from fixed arrays or bounded scratch storage.
- Measure static asset bytes, plan bytes, draw count, frame cost and peak temporary memory on the same emulator scene before and after the kit.
- Keep Second Shift and Arrivals as authored exceptions while the composer proves parity. Do not silently replace their quality benchmark with a sparse placeholder.

## Acceptance gates

For one representative H0, H1 and H2 across two families:

- normal and high-contrast native 480×272 captures;
- readable people, hotspots, speech, options, page indicators and SHIP;
- stable repeated identity/version pixels;
- generic fallback unchanged at unassigned stations;
- zero Game/RNG/save/economy writes from art;
- actual asset bytes, plan bytes, frame cost and memory report;
- no duplicate people, doors or hatches when an authored room includes them.

Only after that vertical slice passes should the composer distribute controlled variations across the 768 station identities and seven logical rooms. The goal is many distinct stations from a small professional kit, not 5,376 bespoke bitmaps.
