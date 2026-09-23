# Lave H0 Merchant Canteen — bounded polish proof

Status: concrete native upgrade plan; no runtime activation. This is the
weakest current proof case because Lave H0 CANTEEN is still the generic
renderer while Reorte, Qube and Xequerin have native Canteen scenes.

## Exact Systems identity

| Field | Value |
|---|---:|
| system / hub / room | Lave `7` / H0 `0` / CANTEEN `2` |
| family | Merchant Salon `3` |
| arrangement | `229` |
| landmark | `155` |
| material | `147` |
| exception / art version | `0` / `1` |
| selector hash | `0xbb30036a` |

The identity is read from the baked descriptor table. The hash chooses only the
approved cosmetic arrangement; it must not choose NPCs, speech, prices, taxi
availability, economy, body identity, danger or audio.

## Before / after quality target

Current: generic Canteen renderer, no Lave-specific authored landmark, no
Merchant material treatment, and no native normal/high-contrast proof pair.

Target: a Merchant Orbital Salon that reads at a glance through a curved ledger
rotunda / broker booth, an authoritative orbital window, brass/amber practical
lighting against burgundy shadow, route-card and ledger material clusters, and
two readable activity silhouettes. The options, feedback, speech and SHIP
bands remain runtime-owned.

## Native room composition

Coordinates are relative to the room viewport `(0,0,340,168)`; screen origin is
`(6,20)`. The SHIP safe rectangle remains screen `(274,158,68,26)`, or room
relative `(268,138,68,26)`.

| Layer | Proposed authored module | Native target | Slot | Owner / truth |
|---|---|---:|---:|---|
| back | merchant orbit window / void frame | 104×54 | `(128,20)` | Station + Planetary; numeric body query only |
| back | curved ledger rotunda landmark | 96×72 | `(16,22)` | Station + Gameplay; visual anchor only |
| mid | route-card / ledger ribbon rack | 64×32 | `(232,34)` | Station; no fabricated prices or offers |
| mid | task lamp / brass practical | 21×18 | `(118,94)` | Element Kit; static light cue |
| front | broker booth counter | 88×36 | `(42,112)` | Station + Gameplay; existing BARTEND truth |
| front | standing broker pose | 24×52 | `(142,108)` | Story + Station; existing cast only |
| front | seated traveler pose | 30×42 | `(204,118)` | Story; existing traveler only |

The composition must reserve the four existing door slots, the right options
column, feedback from y190, and the SHIP return. No new hotspot is created by
the art. Candidate modules are new Merchant-family authored IDs unless the
Element Kit approves an exact footprint/role variant; no silent tinting of
Freight, Research or Reorte art is allowed.

## Capability truth

The current Lave Canteen capability set remains: generic BARTEND local rumour,
generic TRAVELER passenger enquiry subject to existing cabin/hold checks, room
doors, and YOUR SHIP / Triangle return. The room may show a ledger, route cards
and a booth as materials, but must not imply purchase stock, a paid gambling
table, a passenger offer, a market price, a faction service, active music or a
completed transaction.

## Acceptance packet

Produce one native normal and one high-contrast 480×272 proof frame, then an
integrated EBOOT capture covering entry/disembark, WALK, person selection,
speech, available/unavailable capability state, room-door traversal and SHIP
return. Validate the selector/hash, 1× nearest-neighbour pixels, canonical
palette, packed bytes (target no more than the existing 28,560-byte room index
payload plus shared palette), draw/frame/temp-memory budgets, save/economy
stability and deterministic repeat. The room stays generic fallback until this
packet passes Systems, Station, Story, Planetary and Art Manager review.
