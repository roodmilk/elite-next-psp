# ELITE: NEXT 2.0 — PSP GAME DESIGN AND PRODUCTION BIBLE

**Revision:** 2.0 • **Platform:** PSP-2000/3000 and PPSSPP • **Status:** living implementation specification

**Implemented world update, build 2.0.2:** mineable belt entities replace decorative rocks. Cross damages/fractures them; Circle collects the resulting ore within 500 m. Three freight hull classes share stable geometry and oriented collision. Haulers visit physical outer hub berths, trade their manifests once and warp out; arrival schedules and occupancy vary by system activity and prosperity. Route, cargo and current activity are available through hails and targeting. See FREIGHT-2.0.2.md. This is a current-system simulation with a shared local market, not a persistent, independently simulated galactic economy. Scene depletion and traffic state are regenerated on launch/re-entry.

**Implemented presentation update, build 2.0.1:** the current flight layout is specified in UI-SPEC.md's 2.0.1 section. Routine text lives in top/bottom bands; the central view keeps only spatial markers. Native HUD/field sprites, 360-degree radar, clipped glints, coloured solar detail and shaded close freighters are integrated. Early WALK screens still use a fixed perspective: they are prototypes and do not yet fulfil the room-graph/true first-person interior design below. Completion claims require feature-specific gameplay tests as well as smoke results.

This document supersedes the feature priorities in `DESIGN-BIBLE.md` while retaining its audited 1.9.9 behaviour. It is the authority for future work. A feature marked **SHIPPED** exists in the current source; **SLICE** is being implemented as a bounded playable version; **EXPANSION** is designed and gated behind memory, frame-time and playtest evidence; **OUT** is deliberately excluded from the PSP release.

## 1. Product promise

ELITE: NEXT is a self-contained handheld space adventure that combines Elite's piloting, trade, risk and readable ship silhouettes with the discovery, ownership and personal stories associated with modern space exploration games. The player can fly a courier route, fight or avoid a pirate, land on a named world, walk away from the ship, meet people, tune an atmospheric radio station, discover something unusual and return to a hub with a consequence that persists.

The game is not a literal port of ArcElite, No Man's Sky, Elite Dangerous or Starfield. Elite-A data and algorithms are the historical foundation; all new writing, art, interfaces, audio and gameplay code are original PSP adaptations. The target is a coherent small galaxy with strong verbs, not a catalogue of disconnected imitations.

### Design pillars

1. **Flight is the home.** Steering, throttle, roll, target lock, radar, docking and warp must remain the fastest, clearest actions.
2. **Every place has a reason to exist.** A planet, wreck, station deck or radio broadcast offers a choice, a story beat, a resource or a memory.
3. **Handheld clarity beats spectacle.** The 480×272 screen uses short labels, colour-coded speakers, stable panels and an uncluttered scenic HUD.
4. **A bounded world can feel alive.** Ships use routes, formations and faction policies; distant battles use cheap proxies; only nearby contacts receive expensive simulation.
5. **Consequences are local and explainable.** Warrants, prices, relationships, discoveries and mission states are seeded, visible and saved.
6. **One good loop before ten thin systems.** Discover → approach → act → reward → record → choose the next lead.

## 2. Audited 1.9.9 baseline

The current build already provides: a 256-system Elite-derived galaxy; seeded economies and 17 goods; seven player hulls; 34 converted ship blueprints; four factions; danger and local wanted ratings; sparse traffic and freighters; station variants and guided/manual docking; warp; target computer and auto-aim; planets, suns, gas giants and belts; atmosphere flight; a landable pad; first-person planetary EVA with scanable life; five mission slots and five contract types; Kei/Ryn story scaffolding; local police encounters; local Codex; GalacticNet/SpaceBook; procedural multi-station radio with alien chatter; HUD modes; pause power allocation; versioned saves; and a hold-Triangle Comms panel.

The current build is an emulator-validated prototype, not proof of zero defects on every physical PSP. Every new tier must preserve the existing smoke, input, steering, radio, game and performance checks.

## 3. Reference translation

| Reference feeling | ELITE: NEXT implementation | PSP boundary |
|---|---|---|
| Elite / Elite-A piloting | Newtonian-feeling pitch/yaw/roll, throttle, trade, docking etiquette, local law | Keep pools and draw list bounded |
| No Man's Sky discovery | Seeded bodies, life, minerals, Codex, names, scans, small surface regions | No infinite streaming or online discoveries |
| Elite Dangerous scale | System map, danger/security, faction traffic, signal sources, route planning | 256 stable systems; local simulation only near the commander |
| Starfield-style people and interiors | First-person station/ship decks, named contacts, compact conversations, mission terminals | Authored rooms, billboard characters, no full city simulation |
| Modern accessibility | Smart HUD, contextual prompts, remappable comfort presets, high contrast, reduced effects | All text fits native 8×8 layout at 480×272 |

## 4. Player fantasy and progression

The commander begins with a small, imperfect ship and a safe home in Lave. Kei's investigation into Ryn's disappearance gives the first ten hours a clear question. Trading, rescue, scanning, salvage, combat, smuggling and diplomacy are equally valid routes to money and clues. Progression is horizontal wherever possible: a scanner reveals more, a cooler extends combat, a docking computer lowers friction, and a new hull changes risk without invalidating earlier play.

### Progression tracks

- **Pilot:** steering, manual docking, combat accuracy and boost discipline.
- **Surveyor:** body, life, anomaly and signal discoveries recorded in the Codex.
- **Operator:** trade margins, mission reliability, station relationships and route planning.
- **Citizen:** legal standing, local warrants, faction trust and story choices.
- **Builder:** ship sockets, paint/badge identity, one mobile workshop and later one outpost.

No track is mandatory for campaign completion. Rewards state their practical effect before purchase.

## 5. Core loops

### Moment-to-moment

Look → identify the target → choose **approach**, **hail**, **scan**, **dock**, **trade**, **fire** or **leave**. The crosshair never silently changes meaning. The selected contact, body name and system name remain distinct: `System: Lave` / `Body: Lave II` / `Station: Lave System Hub`.

### Short excursion (2–5 minutes)

Read a signal, set a lock, travel, perform one skill action, receive a reward or new clue, and decide whether to return or continue. The player can end safely at a hub after any completed action.

### Session (10–30 minutes)

Accept one or two jobs, travel through a visually distinct system, land or board a small interior, spend earnings on a visible upgrade and log a discovery or consequence.

### Campaign (6–10 hours target)

Find Ryn, learn who is manipulating navigation data, decide whether the network should be controlled by a corporation, a guild or a public relay council, and live with the changed routes and station services. The campaign is finite; procedural jobs continue after the ending.

## 6. Controls and smart control view

The control source of truth is one action table consumed by flight prompts, EVA prompts, help screens, Comms and the pause view.

| Input | Flight | EVA / interior | Menus |
|---|---|---|---|
| Nub / D-pad | pitch/yaw | look or move using selected preset | move selection |
| R / L | accelerate / brake | forward / backward in Legacy preset | page tab where labelled |
| Double-tap then hold R | boost | jetpack only when fitted | never activates gameplay |
| L + left/right | roll | strafe in Legacy preset | none |
| X | laser / use | use tool / interact | confirm |
| Circle | contextual approach, dock, land, board | interact / leave | back |
| Square | targeting computer; hold + direction cycles contacts | survey / scanner | page action where labelled |
| Triangle | hail / acknowledge | take off or return | details / secondary |
| Select | command deck | field log | owning screen |
| L + Select | Full → Minimal → Scenic HUD | comfort HUD | no hidden destructive action |
| Hold Triangle | quick Comms | quick Comms | unchanged |
| Start | pause/power | pause/power | pause/help |

**Smart control view** is a context strip that appears for 1.2 seconds after entering a new mode or when the player pauses. It shows only the three most relevant actions with real PSP glyphs, for example `X USE SCANNER  O BOARD  △ LEAVE`. It never covers the reticle, target card or objective beacon. A comfort option changes it to icon-only, text-plus-icon, or off. The full Controls page remains available.

## 7. Camera and modes

1. **Cockpit flight:** first-person wireframe and holographic HUD; scenic mode removes nonessential text.
2. **Atmosphere:** low-altitude first-person flight with a shared terrain/collision function.
3. **Planet EVA:** first-person walking and jetpack on a bounded local region; current shipped slice.
4. **Ship/station interior:** first-person corridor rooms rendered with flat-shaded geometry, sprite panels and billboard characters; first playable slice is the docked hub deck.
5. **Spacewalk:** tethered first-person salvage around a derelict; expansion after interior navigation is stable.
6. **Menus:** paused command deck, two-pane layouts, one focus highlight and one action footer.

Seamless loading is not a requirement. Transitions hide behind a short warp, airlock, elevator or docking shot while preserving sky palette, body identity and objective state.

## 8. Interiors: the PSP-feasible design

Interiors use authored room graphs, not general procedural architecture. Each room is a 12–24 metre rectangle or corridor with a ceiling height, collision bounds, two to six interactable anchors and one exit anchor. Geometry is flat-shaded and low-poly; walls, consoles and signs use 8×8 or 16×16 nearest-neighbour sprites. Characters are 32×48 or 48×64 four-frame billboards with a shadow and a speaker colour.

### Interior room types

- **Ship deck:** cockpit, bunk, cargo latch, workshop and airlock. The player can inspect the ship identity, tune the radio, manage cargo and depart.
- **Station concourse:** arrivals board, market kiosk, outfitting, mission board, guild desk, local contact and airlock.
- **Station service rooms:** one compact shipyard bay, med bay, security office and salvage locker, unlocked by mission or reputation.
- **Freighter/derelict:** two to four rooms, one hazard, one log, one reward and a return route.
- **Planet outpost:** pad, shelter, trader, scan terminal and a safe return beacon.

### Interior interaction contract

Every interactable provides a name, one-line purpose, verb, result and cooldown. Circle opens the interaction card; X confirms; Circle backs out. A failed action explains the missing condition. Important objects have an optional pulse outline and a compass tick. No item is decorative in a mission-critical room unless it is clearly labelled scenery.

## 9. Spacewalk design

Spacewalk is a bounded EVA state entered from a derelict, disabled freighter or external repair point. The player has a tether length, suit oxygen, suit integrity and a return marker. Movement is six-directional in a 2.5D plane: nub looks, D-pad or Standard EVA movement translates, R/L moves forward/back, X uses scanner/tool, Circle grabs a handhold, Triangle returns to the airlock when within range.

The first implementation uses a single derelict room with six handhold nodes, three salvage sockets, one optional log and a visible tether. Oxygen drains only while detached from a handhold. Missing the return window moves the player to the airlock with a repair penalty; it never deletes the save. Later derelicts add rotating hull sections, but no free rigid-body physics.

## 10. Planets and surfaces

Each system gets a seeded body roster: star, gas giant, ocean/temperate body, rocky body and optional belts/comets/fauna. Body names and system names are stable. Landable worlds guarantee a safe pad, return ship, two common resources, one scan site, one risk/reward landmark and a traversable route. Water, gas and stars are approach vistas, not fake walkable surfaces.

Biomes use six palette families: temperate archipelago, red dune basin, ice shelf, fungal plain, copper badlands and airless ruin. Terrain geometry, visible props and collision sample the same deterministic height function. The player can scan flora, fauna, minerals, anomalies and named landmarks; discovery records persist.

Future surface slices add a rover, a small outpost, archaeology, shallow water pockets and a one-companion utility loop. Voxel terrain, infinite bases and full planetary circumnavigation are outside the PSP release.

## 11. NPC factions and behaviour

Faction colour is consistent across sprites, radar, nameplates, radio stings and menus: Traders amber, Law cyan/white, Pirates red, Explorers green. Law callsigns remain partially encrypted. Every active ship has a role, callsign, destination, route seed, faction, hull silhouette and current intention.

- **Traders:** route between a hub, a body and a second hub; flee pirates; may hail with legal cargo offers.
- **Law:** patrol, inspect wanted commanders, pursue pirates, protect starter Lave and never carry a warrant across a jump.
- **Pirates:** ambush valuable traffic in danger 2–5 systems; offer surrender, demand cargo or break off when outmatched.
- **Explorers Guild:** survey bodies, form loose groups, reveal anomalies and offer scan work.
- **Civilians and fauna:** provide ambience and discovery; never create unavoidable combat.

Simulation uses near/mid/far levels of detail. Near ships steer, collide and fire. Mid ships use route nodes and coarse combat. Far contacts are counters and audio reports. Pools remain bounded.

## 12. Missions and stories

The Mission Board offers delivery, bounty, exploration, rescue, covert delivery, salvage, repair, escort, legal inspection, archaeology and survey contracts. Each offer states destination, reason, risk, reward, time, required equipment and failure recovery. A mission is a state machine with a durable ID, not a copied array slot.

### Campaign chapters

1. **Welcome aboard:** Kei teaches launch, target and dock.
2. **A missing route:** a corrupted survey packet points to a quiet moon.
3. **Three signatures:** trader, law and explorer testimony disagree.
4. **The dead relay:** board a derelict and recover Ryn's voice log.
5. **A price on a map:** choose lawful inspection, pirate negotiation or covert extraction.
6. **Ryn's shelter:** meet Ryn on a surface outpost; boarding is a persistent state change.
7. **Who owns a safe route?:** corporation, guild or public relay choice.
8. **The open channel:** final flight through a contested system and epilogue jobs.

Dialogue uses short turns, speaker colour, portrait scale matched to its box, optional radio subtitles and a skip/quiet path. The radio keeps mood and context but never hides a required objective.

## 13. Economy, ownership and crafting

The existing 17-good Elite economy remains the base. Station stock is visible only while docked; historical rumours are labelled. Upgrades occupy authored sockets: scanner, cooling, shields, cargo, fuel, docking, weapon and utility. The player may paint a hull, choose a badge and name the ship.

Expansion adds one mobile workshop, six repair/refine recipes, one outpost with three service branches and one companion. Ownership changes services and visuals; it does not create daily chores or a cash shop.

## 14. Audio and radio

Audio is original procedural/CC0-safe material generated for the project. Radio stations are mood systems, not copied game soundtracks:

- **Lave Public:** warm pads, slow arpeggios, traffic bulletins.
- **Frontier Signal:** sparse analog pulses, explorer logs and navigation tips.
- **Night Freight:** low bass, brushed percussion and trader dispatches.
- **Redline Relay:** tense sequencers, pirate rumours and combat warnings.
- **Deep Field:** long drones, choral harmonics and alien chatter.

Radio ducking protects speech and warnings. The quick Comms menu can mute text chatter, dismiss a line, hail the target, clear the target, change station, open the radio page, request docking, open Guild assignments or cycle HUD. Volume and reduced-effects settings persist.

## 15. Art and presentation

Art direction is late-1980s science fiction: hard silhouettes, restrained neon, dark blue/amber/cyan panels, low-res portraits, halftone stars and wireframe geometry. Avoid glossy anime gradients and over-detailed faces. Every small portrait has a 32×32 or 48×48 source and a readable two-tone expression at native scale.

Effects are additive but capped: sun corona and lens flare, restrained post glow, warp streaks, speed lines, station entrance lights, engine trails, collision sparks and a wireframe explosion. Reduced-effects mode removes flashing and flare streaks. No effect may obscure a target or objective.

## 16. Memory, performance and technical rules

- 480×272 framebuffer, nearest-neighbour sprites, fixed-size pools.
- 60 FPS target; 50 FPS minimum in release scenes; 30 FPS floor for expansion scenes.
- No heap allocation in the frame loop; no floating-point audio oscillators.
- Draw-list and texture budgets are measured in CI; asset source files stay outside the EBOOT.
- Interior rooms stream one room at a time; far NPCs are counters.
- Save schema uses explicit version, length, checksum, seed/version and tagged sections. Interrupted warp, docking, EVA and spacewalk resolve to a safe checkpoint.
- All content strings are clipped/wrapped for 480×272 and checked for overlap.

## 17. Release tiers

### Tier 1 — shipped foundation

1.9.9 flight, trade, docking, warp, planets, planetary EVA, missions, local law, radio and Comms.

### Tier 2 — next major build

Station/ship interior walk slice; smart control view; interior interaction cards; named concourse contacts; station deck ambient audio; save-safe interior checkpoints; updated Controls and Comms; regression tests.

### Tier 3 — exploration expansion

Derelict room graph, first tethered spacewalk, salvage sockets, oxygen/integrity, one rover route, outpost room and archaeology mission.

### Tier 4 — campaign and living systems

Full Ryn chapters, faction reputation, route ownership decision, station council, companion, expanded biomes, freighter interior and capital encounter.

### Tier 5 — optional experiments

Underwater pocket, creature research arena, living ship, station specialisation and expedition seed codes. Each requires a measured prototype before entering the main build.

## 18. Verification gates

Every tier must pass:

1. Compile with `-Wall -Wextra` without warnings.
2. Existing game, input, steering, radio and performance checks.
3. New deterministic unit tests for save/load, state transitions, collision and mission recovery.
4. A 42-scene emulator smoke run plus one 20-minute soak.
5. Native 480×272 screenshot review for every new screen.
6. Manual checks for suspend/resume, low fuel, low money, full cargo, active warrant, interrupted transition and inaccessible objective.
7. A release note that distinguishes tested emulator behaviour from physical PSP evidence.

## 19. Definition of done for the full PSP release

The release is complete when a new player can finish the campaign, trade or avoid combat, land and walk on a world, walk through a hub, complete a bounded spacewalk, tune radio, understand every prompt without a manual, recover from every failure, save safely and return to a changed galaxy. “Seamless”, “infinite” and “zero bugs” are not acceptance criteria; reliable, legible, replayable, measured behaviour is.

## 20. Source and rights boundary

Elite-A source and documentation are used as historical/reference material under their stated notices. New code, dialogue, sprites, procedural audio and design are original project work. No commercial No Man's Sky or Elite Dangerous music, art, fonts or proprietary source is bundled. External CC0 sources remain credited in `assets/audio/CC0-SOURCES.md` and art provenance files.
