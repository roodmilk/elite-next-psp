# ELITE: NEXT 2.0.2 — mining and freight acceptance

## Player-facing behaviour

Cross fires the existing laser. Ordinary and ice-belt asteroids take repeated hits according to their hardness and the player's weapon/power setup. Hit flashes and outward particles mark damage/fracture. A fractured rock becomes a 1–3 tonne mineral canister; Circle collects it within 500 metres. The canister retains its target ID, and the footer changes from MINE to COLLECT. Intact rocks cannot be collected; further shots cannot extract the same ore again. The existing full-hold salvage sale behaviour is retained.

All belt geometry is now backed by game entities. The 64-slot pool contains 18–28 ordinary belt rocks (three in sparse systems), an additional 10–14 ice rocks in ice-belt systems, initial salvage and spare slots for destroyed ships. Rocks drift slowly and have seeded size, hardness, quantity and position. Fracture reuses its own slot so a full pool cannot lose its ore.

Freighters have three readable low-poly hulls: container carrier, tanker and bulk barge. Their engines, bridges and cargo modules share one silhouette across rendering distances. A shared half-extent defines each hull: 144×90×560, 116×100×480 and 166×82×640 metres respectively. An oriented bounding box plus player clearance replaces the oversized collision sphere; it is intentionally conservative around gaps between containers. Ordinary NPCs steer around capital ships.

## Commerce and schedules

1. System activity/prosperity allow zero, one, two or three haulers. Initial occupancy also varies; Lave starts with one delivery underway.
2. Each hauler has a reachable neighbouring trade partner, an economy-based legal cargo manifest, a hull style, a hub and a safe route.
3. An arrival uses a 2.5-second warp effect at a distant gate. Routes are rejected if they cross planets, hubs or nearby capital ships at planning time. A new gate is at least 2,500 metres from the player.
4. The hauler cruises at 60–100 m/s to a cargo-transfer berth outside the hub, keeping clear of the small docking aperture.
5. Reaching the berth adds its imports once. It loads available exports, never taking stock below zero. Imports saturate at 99 without erasing existing player-sold inventory above that level.
6. Loading lasts 32–56 seconds. The hull turns gradually, then flies back to its gate and charges warp for three seconds.
7. At least 45 seconds separate warp events. New arrival opportunities are spaced 150–316 seconds apart and wait for capacity. Departed hulls wait another 150–269 seconds before reuse; destroyed hulls wait 240 seconds. Safe-route failure retries after 20 seconds.
8. Hails report origin/destination, cargo and current activity. Target details show the active phase. Destroyed haulers drop their actual remaining manifest.

Small ships retain the existing faction AI, danger rules, police behaviour and exploration formations. Freighters are traders and may be attacked by the existing pirate AI.

## Verification on the release binary

- PSP GCC build with -Wall/-Wextra: no warnings.
- PPSSPP: 217 gameplay checks, 105 input/UI checks, 13 steering checks and 15 radio checks passed — 350 total.
- Added gameplay checks cover actual laser firing/missing, foreground rocks shielding ships behind them, warp restrictions, full debris pools, one-time ore collection, exact manifests, one-time stock changes, stock saturation, cargo drops, delayed replacement, complete freight cycles, and swept hull collisions during boosted flight.
- Route clearance, reachable trade partners, all hull types and traffic-capacity variation checked across all 256 generated systems.
- Standard emulator benchmark: 42 scenes / 425 frames, average 59.94 FPS, worst frame 17.34 ms, zero frames over 25 ms.
- Final native 480×272 frame captures inspected: container carrier, tanker, bulk barge, warp arrival, warp charge and mineable asteroid. Included in the package's previews folder.
- Regression report files are included in validation. The packaged EBOOT hash is checked against the tested working binary.

## Boundaries and next work

This is local, bounded commerce. All hubs in the current system share one market; unloaded systems do not continue simulating trade. Cargo and market values use the existing save format. Local traffic, rock damage and depletion reset on launch/re-entry. Warp events and arrivals pause with the flight simulation. The route planner checks nearby ships when a route is chosen, rather than reserving a globally conflict-free timetable.

Capital collision is a conservative hull envelope, not per-container collision. Freighters do not yet offer boarding, player ownership or repair contracts. Comets/space fauna retain their separate scenery behaviour. Physical PSP testing and extended manual playtesting remain outstanding; emulator checks are not a zero-bug guarantee.

Future work must preserve: stable silhouettes, no spawning on the player, no repeated transfer rewards, no UI text in the central view, and no new constant warp spam.
