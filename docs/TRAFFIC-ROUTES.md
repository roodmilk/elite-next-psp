# Local traffic routes — 2.5.210

Four deterministic three-node lanes connect the main station's approach to the
existing world-port approach points. Lanes 1–2 are main patrol corridors; 3–4
are offset outer routes. Position, height and length follow actual system
geometry. System Operations → Square shows their top-down schematic.

Traders traverse their assigned route, pause for cargo transfer at either end,
then reverse. Existing capital-freighter arrival, service and departure schedules
and explorer formations remain intact. Traders damaged by other NPCs no longer
retaliate against the player. Law patrols main corridors, pursues nearby suspects
with a lane leash, and dispatches reinforcements from the station approach rather
than spawning them next to the player. Pirates hunt outer-lane traders, evade
nearby Law, and return after escape. Existing government-dependent population
budgets, ship avoidance, crime records, mission/trader hails and bounty boards
remain in use. The forced arrival pirate/police duel is removed.

Unengaged named Wanted targets survive lethal NPC damage and escape; posters
remain open and point to the same real ship. A target hit by the player in the
last 90 simulation seconds credits the player when police finish it. Player
weapons can still kill an escaping target normally. Rescue protection remains.

## Law Scanner

Catalogue item 56, UTIL, 480 units, displayed tech 3+, any economy with trade
rating 1+. Hold Circle and press the R shoulder button to select it; release,
then tap Circle to scan. Existing four directional tool assignments are unchanged.
The tool panel and Outfitting describe this fifth shoulder-button selection.

Scan range is 20 km; planets obstruct reception. Scans recharge in 3 seconds.
Radar shows public main-route traces and sampled 650 m inspection volumes at
the detected patrol positions. These are snapshots, fade after 6 seconds and
expire after 12, not omniscient live positions. The age is displayed. Patrols
can move between scans; outer routes and empty scans are not safety guarantees.

New NPC route state and scanner snapshots are transient bounded arrays. No
heap allocation or save-layout expansion. The existing V26 fitted-item bytes
persist the new module; old saves load, but use the new executable for saves
containing item 56. Scans clear on system reconstruction/load.

## Deliberate limits / next steps

This is local route-based traffic, not a persistent interstellar economy. Trader
service does not consume station stock; departing and returning regenerates local
traffic as before. Named travellers retain their existing persistence. Main lanes
describe patrol likelihood, not imaginary fixed automatic police checkpoints.
The compact map omits height; the cockpit scanner projects actual 3D positions.
Pirate refuge locations are local flight destinations, not new dockable bases.
Test real PSP performance and balance route travel times, interception frequency
and escape duration before expanding to more hubs, convoy escorts or traffic jobs.

## Verification

New automated checks cover all 256 route assignments, Wanted protection and
assistance payout, trader service/return, station dispatch, scanner range/recharge,
snapshot immutability, fitted-module save/load, Circle+R selection versus firing,
and route-map navigation. Native radar/tool/map captures accompany input checks.
Build and runtime results are recorded in CHANGELOG and the handoff.
