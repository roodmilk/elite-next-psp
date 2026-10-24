# DockBay — ELITE: NEXT 2.5.398

Open GalacticNet > MARKET. DockBay advertises real goods aboard real ships,
with fixed prices and no offscreen expiry or competing buyers. Navigation
data is free. Reading or watching an advert never spends units.

## Website controls

- Up/Down selects an advert; Left/Right changes the three-card page.
- L/R shoulders switch GalacticNet sections, retaining the existing sections.
- X reads an advert, then requests its navigation data. Replacing an existing
  route asks for confirmation. Mission tracking stays unchanged.
- Square cycles ALL SYSTEMS, NEARBY, EQUIPMENT, CARGO and MY DEALS on the list;
  in a live advert it watches/unwatches that advert.
- Circle backs out. MY DEALS includes watched offers, routes, remaining lots,
  ended meetings and the 32 most recent transaction receipts.

Local navigation targets a physical rendezvous. Remote ads use the normal
multi-hop galaxy route. On arrival the waypoint changes to the local meeting.
The named seller appears within 12 km of the meeting; approach within 600 m.
Tap Triangle to contact, or hold Triangle and use Comms > Contact Target.
Both use the same seller identity, operation exclusions and distance checks.
Marketplace transactions are unavailable while docked, landing, jumping,
docking or setting up/running a race.

## Offers and ownership

Conversations pause flight. Left/Right chooses; X confirms; Circle declines.
NO DEAL is selected initially. Triangle opens optional working conversation
choices. Travelling shops can show another actual co-located advert.
For cargo, Square adjusts the batch: Left/Right changes one tonne, Up/Down
changes ten, X returns to the offer without paying. The receive/pay boxes
show actual quantities, total cost, remaining stock and resulting balance.
Disabled Accept explains insufficient units, storage or cargo capacity.
A failed or stale transfer changes neither money nor goods.

Modules enter Item Storage, never auto-fit. Permanent WORN equipment stays
weaker; MODDED and RARE equipment keeps the exact advertised roll. Rare art
has a restrained glow and shine. Different rolls of the same base module
remain separate items and can occupy separate banks. Storage, outfitter
resale, hull exchange and inactive ERL-owned ships preserve their identities.
Only compatible open banks accept an item. Storage and cargo limits block
unsafe swaps. Engineers repair ships without changing module quality.

Supported generated families are lasers and exclusive weapons, cargo bays,
long-range scanners, shield boosters, armour, heat buffers, boost injectors,
efficient boosters and efficient hyperdrives. Multi-stat weapons roll damage
and cadence; efficient boosters may roll fuel and heat efficiency. Weapon
sustained output cannot exceed twice normal. Consumption cannot improve
below half the comparable normal value. Normal legacy equipment retains its
original behaviour and catalogue resale price. Passive copies apply the best
fitted family benefit; cargo bays and separately selected weapons retain
individual effects. Heat benefits have a ship-wide comparable 50% floor.

Bulk cargo remains aboard the seller. Buy only what fits, unload elsewhere
and return for the unchanged unpaid stock. Buying notices pay the advertised
unit price for unreserved cargo. Mission-owned cargo cannot be sold here.
No barter, haggling, consumable buffs, deposits or player-posted adverts.

## People and trouble

24 persistent named sellers supply 48 live listings across the galaxy and
20 authored encounter flavours: enthusiast, moving-house clearance, breakup
sale, collector, first seller, bulk food, miner surplus, mobile mechanic,
cargo buyer, exceptional bargain, travelling shop, crew clearance/robbery,
lonely pilot, worn specialist, weapons courier, fake seller, double-cross,
scavenger, merchant convoy and recurring supplier.

A seller's identity, portrait, price, stats and chosen danger outcome are
saved together. Unrelated traders, buoys and mission agents cannot take over
the conversation. Outcomes are independent of goods, bargain prices and
portraits. Roughly 11% of new listings initially carry possible trouble:
4% pre-trade, 4% post-trade and 3% outside pirates. This is initial tuning.

Pre-trade traps cancel payment. Post-trade attacks preserve the completed
purchase and receipt. Outside pirates do not turn the seller into a betrayer.
Actual hostile ships enter controllable flight after the paused transmission.
Fight or escape beyond 24 km; switching adverts cannot erase a nearby fight.
An observed betrayal can be reported. Honest sellers defend themselves if
attacked. Convoy escorts cannot hijack the named seller's sale.

Five distinct completed peaceful meetings make a seller TRUSTED permanently.
Batches, hails, receipt reopens and multiple shop items in one physical visit
cannot farm trust. A planned double-cross cannot grant trust beforehand.
Trusted sellers never betray, though outside pirates can still attack them.
Completed, acknowledged lots are replaced only after their seller leaves;
new stock has a new advert ID and roll, while recent receipts remain in MY DEALS.
Unfinished lots are never replaced. Departures happen during controllable flight.

Destroying a seller can expose their actual unpaid cargo or module. A module
pod retains its exact roll, obeys tractor and storage limits and can be
collected once. Already purchased goods cannot drop a duplicate. A checkpoint
can recreate an uncollected module pod after reload.

## Saves, performance and installation

Additive V35 saves retain old commander, discovery, mission, appearance and
ERL records. Older equipment migrates as normal instances. Keep a backup if
you intend to return to a pre-V35 EBOOT: older builds cannot load newer saves.
Marketplace commits use the existing atomic save path. Flight checkpoints
retain position, condition, seller health, destroyed attackers, receipts and
transactions. A failed save displays a warning and keeps the live session.
Unsaved damage/movement between checkpoints can be lost on power removal.

Only the current rendezvous simulates ships; dormant sellers are small saved
records. Seller collision boundaries match their actual hull geometry; bulk
haulers leave clear approach room inside the 600 m contact range. Screens render the three visible cards and selected panel using the
real 8×8 font, native icons and simple primitives. Short transitions do not
block navigation. Review code is excluded from retail. Emulator measurements
are recorded in the release report; physical PSP performance is not yet proven.

Replace only EBOOT.PBP in your existing installation. Keep commander saves,
settings, music and other files. No personal music or save is included.
