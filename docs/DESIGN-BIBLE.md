# ELITE: NEXT

> **Current authority:** [DESIGN-BIBLE-2.0.md](DESIGN-BIBLE-2.0.md). This historical 1.x bible remains as the detailed audit and shipped-behaviour reference; new implementation and prioritisation follow the 2.0 document.
## Complete game design and implementation bible

### 1.7.4 contract presentation rule

Every repeatable board offer must communicate a reason, destination and local
condition in one short deterministic brief. The brief is derived from the
offer's type plus the destination system's danger and prosperity; it never
changes the saved mission payload or consumes mission time. Only the focused
offer shows the brief, leaving the 480×272 board readable. Future authored
chapters may replace the brief with named dialogue, but must retain the same
objective/reward/risk contract shape and recovery guarantees.

The board also exposes a 1–5 risk rating before acceptance. Destination danger
and inherently risky job types raise the rating and add a deterministic payout
premium. The accepted job stores that payout, so the player is never baited by
an offer that changes after pressing X.

**Approved game title:** ELITE: NEXT (formerly Elite: Starfarer — The Open Channel). Older title references below describe the original design draft.  
**Platform:** PSP homebrew, with PPSSPP as a development target  
**Design revision:** 1.0 — 21 September 2026  
**Audited foundation:** Elite-A PSP 1.3.8, `outputs/arcelite-psp/`  
**Purpose:** A self-contained specification for future ChatGPT/Codex development sessions. This is a design deliverable, not a claim that its proposed features are already in the EBOOT.

> Fly somewhere beautiful. Notice something unusual. Make a useful choice. Return with a story, a discovery, or a ship that feels more yours.

## Current interface and art authority

The [UI, identity and presentation specification](UI-SPEC.md) expands the five-category design into screen layouts, navigation rules, a full screen inventory, faction/character mappings, the latest late-1980s art direction, audio rules and release gates. Read it alongside this bible for interface work. It distinguishes shipped changes from planned acceptance work.

## How to use this document

Read sections 1–5 before implementing anything. Sections 6–21 specify the game. Sections 22–27 provide the campaign, repeatable missions and production-ready dialogue. Sections 28–35 explain implementation, sprites, performance, persistence and verification. Sections 36–39 define delivery order, coverage of the user's requests, sources and the handoff prompt.

Status language is mandatory:

- **CURRENT:** directly observed in the 1.3.8 source or its existing test output. It does not imply every interaction has been verified on a physical PSP.
- **PROPOSED CORE:** required for the next coherent release described here.
- **EXPANSION:** designed now, implemented only after its dependencies and performance gates pass.
- **EXCLUDED:** intentionally outside this product's current scope.

All numeric tuning values below are proposed starting points unless explicitly identified as current code constants. Playtesting may change them; changes must preserve the design intent and update this document.

## 1. The game we are making

A personal, offline space adventure that combines Elite's compact trading and piloting with the pleasure of discovering strange places. It should feel like a small living galaxy, not a theme park that spawns every event around the player.

The player inherits a modest ship and joins Handler Kei's search for the missing surveyor Ryn. Ordinary jobs finance the journey. Traders, patrols, explorers and pirates pursue their own routes. A good scanner reveals opportunities rather than a screenful of chores. A safe station always feels like coming home.

The central promise is **a complete small adventure, then a generous sandbox**. The campaign has an ending. Procedural work continues afterwards. No multiplayer service, daily login, paid currency or internet connection is required.

### Design pillars

1. **Pleasure in flight.** Responsive controls, readable motion, satisfying docking and beautiful unobstructed views come first.
2. **Curiosity pays.** Scanning, helping, trading and salvaging must advance the player as reliably as shooting.
3. **A living neighbourhood.** Ships have destinations, identities and relationships. Lave is a welcoming home, not a combat tutorial ambush.
4. **A handheld-sized adventure.** A worthwhile action takes seconds; a job fits into 5–12 minutes; quitting safely is easy.
5. **Readable retro craft.** Crisp sprites, deliberate colour, strong silhouettes and short English sentences. The screen is 480×272, not a shrunk desktop monitor.
6. **Trustworthy consequences.** Docking, saving, rewards, target locks and scene changes obey explicit rules. The game explains why an action is unavailable.

### What success feels like

After ten minutes, a new player has flown, docked, earned something, met two characters and chosen a destination. After an hour, their ship has a meaningful upgrade and their Codex contains something memorable. After a weekend, they have resolved Ryn's story, made a lasting decision, and still have reasons to fly.

**Fun hypotheses to test:** at least four of five new testers can dock with guidance without coaching; at least three voluntarily investigate an optional signal; none loses a save or mission because they read a menu; most can describe a favourite place or character rather than only their credit balance. These are research goals, not existing results.

## 2. What actually exists in 1.3.8

The project is a native C adaptation built from documented Elite-A data and new gameplay code. It is not a binary port of ArcElite, and no ArcElite source was obtained in this project.

| Area | CURRENT evidence | What still needs to become dependable or deeper |
|---|---|---|
| Galaxy and markets | `game.c`: 256 seeded systems, Elite-derived economy/market logic, 17 commodities | Richness needs readable consequences and stable trade history |
| Ships | `ships.c`: 34 converted blueprints; player ships, factions, freighters | More meaningful hull roles, formation behaviour and encounter balancing |
| Space | `sectors.h`, `flight-extras.h`: varied backgrounds, bodies, belts, scenery, effects | Interaction must match what the scanner promises; visibility and collision must agree |
| Docking | `docking.h`, game docking stages: rotating opening, guidance, arrival presentation | Regressions at every approach angle, speed, station scale and interruption |
| Surfaces | `planet.h`, `game.c`: atmosphere scene, landing pad, EVA, jetpack, scanable life | `terrain_height()` currently returns a flat 24; no continuous planet-scale terrain |
| Missions | `voyage.h`, `Job jobs[5]`: five slots and delivery/hunt/exploration/rescue/smuggling | General event-driven stages, durable target identities, authored story resolution |
| Story | `story.h`: Kei/Ryn tutorial, service unlocks, skip option | Tutorial completion leaves Ryn unresolved; the full campaign below is new |
| Law | Per-system wanted records, police reinforcement/interception and payments | Fair cargo law, route-specific enforcement, full persistence/escape tests |
| UI | Native 8×8 text, command deck, cargo, outfitting, contacts, GalacticNet, Codex | Consistent navigation, fewer mandatory chords, one source of truth for controls |
| Input | Analog readiness/deadzone, roll, boost, target chords, HUD modes | First-class action map and test coverage for simultaneous/held inputs |
| Audio | Integer-only procedural worker in `audio.h` | Queued priorities, surface ambience, balance and hardware profiling |
| Saves | Version 7; imports earlier formats; warrants/jobs/story/upgrades | Atomic replacement with backup, durable discoveries, safe resume states |
| Sprites | `sprites.h`, `pixel-art.h`, Python/PowerShell baking tools | Portable asset sources, alpha correctness, animation metadata and atlas budgets |

**Audited constraints:** `NPC_COUNT=48`, `DEBRIS_COUNT=24`, `ANOMALY_COUNT=4`, `LIFE_COUNT=8`, `BODY_COUNT=5`, `MISSION_SLOTS=5`; an 8 MiB configured heap; a 2,048-triangle software draw list; 512-pixel framebuffer stride. These are current pool sizes and implementation choices, not invitations to increase everything.

The last packaged smoke run reported 59.94 FPS over 42 short scenes with no recorded check failures. This demonstrates an emulator run, not a sustained real-PSP performance guarantee. Recount tests from the actual log rather than copying a historical README total.

### Important source findings

- `main.c`, `game.c` and large included headers combine UI, state, simulation and tests. Change boundaries must become clearer before adding many systems.
- The mission array and legacy `contract`/`mission_*` fields mirror each other. Future work must eliminate ambiguous ownership, not add another copy.
- Array-slot target IDs can identify a different ship after replacement or a system change. Use durable IDs plus generation checks.
- Menus and modal conversations currently treat mission clocks differently. The proposed default is that reading never costs mission time.
- `save_game()` writes a temporary file and removes the destination before rename. A failed rename after removal can lose the only good save. Fix with verified backup/recovery handling before expanding persistence.
- The sprite baker labels its output “16-bit” but emits `unsigned` RGBA values: normally 32 bits per pixel on this target. File size and texture estimates must use actual representation.
- `bake-sprites.py` refers to an external `.cursor/.../assets` directory and a personal absolute output path. Its later magenta test can erase intended magenta artwork even after border flood removal. Replace that pipeline with explicit alpha and repository-relative inputs.
- A story line calls the starting ship a Cobra; verify the selected start hull and generate such names from game data. Dialogue must not contradict the player's actual ship.

## 3. Scope, taste and inspiration

### Elite and ArcElite

Preserve piloting, recognisable ship silhouettes, cargo risk, station etiquette, fuel planning and independent traffic. Elite-A's documented source is valuable for data and algorithm study. Mark Moxon's site describes the annotated implementation and its lineage; it does not make the original game material an unrestricted asset library. See [About Elite-A](https://elite.bbcelite.com/elite-a/) and the local upstream README.

ArcElite is the user's reference for the feeling of a world that keeps moving without the player. In this document that is a design target, not a claim to reproduce or possess its original AI. No proposed capital ship, mission or fauna feature should be advertised as recovered ArcElite behaviour.

### No Man's Sky research boundary

Research checked on 21 September 2026 includes the official update index through Cosmos 7.0 and the listed Cosmos patches. The useful ideas are exploration, discovery, ownership, salvage and place-making; our names, writing, art, characters and implementations should be our own. [Official release log](https://www.nomanssky.com/release-log/).

Recent reference points include modular inhabited ships in [Voyagers](https://www.nomanssky.com/voyagers-update/), gravity-assisted salvage in [Remnant](https://www.nomanssky.com/remnant-update/), creature contests in [Xeno Arena](https://www.nomanssky.com/xeno-arena-update/), coordinated combat threats in [The Swarm](https://www.nomanssky.com/swarm-update/), and station ownership, outposts, derelict salvage and local navigation in [Cosmos](https://www.nomanssky.com/cosmos-update/). Those are source facts; the PSP designs below are original proposals.

### Feasible adaptations

| Reference experience | Our playable interpretation | Tier | What is deliberately reduced |
|---|---|---|---|
| Procedural universe | Stable 256-system galaxy; authored route through seeded places | Core | Quality combinations, not astronomical planet counts |
| Discover and name life | Persistent local Codex, observations and player nicknames | Core | No global first-discovery server |
| Planet exploration | Small streamed regions, several named landing sites | Core | Explicit orbit/surface transition |
| Mining and refining | Identify seams, cut a weak point, choose valuable ore | Core | Short recipes, no inventory busywork |
| Multitool | Scan/cut/repair functions sharing a readable tool UI | Core | No dozens of near-identical weapons |
| Ship building | Hull plus three authored module sockets, paint and badges | Expansion A | No arbitrary physical ship assembly |
| Freighter ownership | One mobile base with cargo, workshop and three crew roles | Expansion A | One loaded interior, bounded fleet simulation |
| Base building | Snap 12 modules at a chosen surface site | Expansion A | No voxel terrain editing or infinite constructions |
| Settlements | One outpost with three upgrade branches and incidents | Expansion A | Visits and missions drive changes; no real-time chores |
| Relics and archaeology | Six-fragment artefacts, tool choices and evidence puzzles | Expansion A | Finite curated combinations |
| Companions | Bond with one field companion; three utility behaviours | Expansion A | No huge breeding simulation |
| Underwater exploration | Separate shallow reef/submersible scene | Expansion B | No planetary oceans simulated in 3D |
| Ground vehicles | One rover with survey, cargo or rescue fitting | Expansion A | Small collision/handling model |
| Fishing/cooking | Optional two-minute shore activity and useful provisions | Expansion B | Small species/recipe roster, no mandatory hunger |
| Derelicts and spacewalking | Bounded salvage rooms, tether, integrity and exit decisions | Expansion A | No freely simulated gigantic destructible wreck |
| Gravity manipulation | Tractor lock and a constrained carry/tow state | Expansion A | No general-purpose rigid-body sandbox |
| Creature arena | Optional sprite-based research simulations, three-action tactics | Expansion B | Offline leagues; animals are not harmed |
| Huge swarm threat | One capital encounter, six live attackers, distant visual proxies | Expansion B | No real simultaneous hundreds-of-ships battle |
| Station directorship | Late-game council decisions, three service upgrades, changed exterior | Expansion B | One owned hub, no multiplayer territory war |
| Expeditions | Permanent authored challenge itineraries with seed codes | Expansion A | No deadlines imposed by a calendar |
| Multiplayer alliances | NPC coalitions and shareable offline seeds | Expansion B | Actual networking excluded until separately commissioned |
| Living ships | Rescue and bond with an unusual organic craft | Expansion B | Hand-authored hull and growth choices |
| Seamless infinite traversal | Convincing transitions with matching sky/location | Excluded as a requirement | Avoid an engine rewrite to remove a brief transition |

**The rule:** complete each feature's discover → act → reward → persist → return loop before adding its next variation. A whale that is just scenery must be labelled scenery; a surveyable whale must have a stable identity, behaviour, reward and saved record.

## 4. The complete gameplay loop

### Second-to-second

Look, steer, hear engines respond, read the current contact, choose approach/scan/hail/fire. Contextual prompts state one primary action. A useful response arrives within 100 ms where practicable; expensive actions show clear progress immediately.

### Two-to-five-minute loop

Notice a signal → inspect its risk and distance → travel → perform one skill or decision → receive a visible outcome. Examples: manual docking, matching a signal, rescuing a drifting pilot, finding a biological trace, recovering a valuable crate.

### Five-to-twelve-minute loop

Take one job and an optional nearby opportunity → prepare at the hub → fly a route with a landmark → complete the job → return or choose the next leg → spend earnings on something that changes play.

### Forty-five-minute loop

Resolve a story chapter, complete a discovery set, change a ship fitting, or establish a new safe route. Every session should leave a visible memory: a journal entry, named specimen, saved photograph, station decoration or improved relationship.

### Long loop

Find Ryn → uncover why survey routes are being suppressed → decide who should control the new navigation network → build a life in that network. The campaign target is 6–8 hours for a first playthrough, plus optional work. This is a production target requiring playtesting, not a promised duration.

### Things we do not use to manufacture engagement

No login streaks, loot boxes, real-time crop deaths, forced grind gates, loss of progress while the PSP is suspended, or “come back tomorrow” tasks. Completion medals reward mastery and variety, not repetition of the same trivial action.

## 5. First hour and onboarding

| Time target | Player experience | Guaranteed support |
|---|---|---|
| 0–2 min | Choose Guided Start or Free Commander; meet Kei | All accessibility, save and help options available |
| 2–5 min | Launch over Lave, turn, throttle and lock the hub | Clear gold objective; no player-targeting pirates |
| 5–8 min | Guided dock; see ship enter; hear Venn welcome them | Manual docking remains an optional mastery goal |
| 8–15 min | Deliver a local package and see an explorer formation | Cargo reserved automatically; no required combat |
| 15–25 min | Approach a planet; decline once; enter and scan one organism | Returning to orbit preserves a safe position |
| 25–35 min | Choose a first upgrade and a nearby route | Preview exact effect, cost and return fuel |
| 35–50 min | Rescue a pilot or recover a signal recorder | Combat, stealth and service alternatives |
| 50–60 min | Reveal a credible clue to Ryn; choose the next lead | First hour ends with a question and a reachable destination |

Replace mandatory service unlocking with recommendations. Guided Start highlights one next step; advanced services can show “You have not used this yet” with an explanation rather than an arbitrary lock. Keep the old tutorial skippable and preserve imported commanders' unlocked state.

## 6. Controls, cameras and command deck

### Stable flight bindings

Retain the latest working bindings rather than reverting to an earlier conversation's layout:

| Input | Space flight | On foot | Menu |
|---|---|---|---|
| Nub | Pitch/yaw | Look; optional movement preset | Optional navigation, off by default |
| D-pad | Alternate steering | Movement in the proposed Standard EVA preset | Select row/column |
| R / L | Accelerate / decelerate | Forward / backward in Legacy EVA preset | Tab where labelled |
| Double-tap then hold R | Boost | Jetpack, when fitted | Never activates gameplay |
| L + left/right | Roll | Strafe in Legacy EVA | No hidden action |
| X | Laser/tool | Use selected tool | Confirm |
| L + X | Missile with valid hostile lock | Unbound unless displayed | No purchase shortcut |
| Circle | Context action: dock/approach/salvage | Interact/board | Back |
| Square tap on release | Select object nearest centre reticle | Survey mode | Page-specific labelled action |
| Square hold | Targeting computer after a short delay | Survey mode | Page-specific labelled action |
| Square + left/right | Contact category | Tool category if exposed | No inherited flight action |
| Square + up/down | Next/previous contact in category | Survey contact | No inherited flight action |
| Triangle | Acknowledge speech, then hail | Takeoff/return where valid | Details or labelled secondary action |
| Select | Command deck | Field log | Return to owning screen |
| L + Select | Full → Minimal → Scenic HUD | Same where safe | Not a flight command |
| Start | Pause/options/power | Pause | Pause/help; never deletes selection |

Offer Standard EVA (D-pad movement, nub look) and Legacy EVA (R/L movement) as explicit presets. Show the selected preset in the Controls screen. Generate every prompt and controls row from the same action bindings. A chord consumes its base button; releasing Square after a chord must not open another screen.

### Camera reliability

Proposed core: store ship orientation as a normalized quaternion or orthonormal basis, not a pitch value clamped below vertical. Apply local yaw, pitch and roll; derive camera vectors, compass, weapons and docking alignment from that single orientation. Re-normalize periodically. Keep Euler values only as a compatibility/import layer until migration is complete.

Tests must cover two full loops, rolling while pitching through vertical, long continuous flight, target auto-alignment behind the player, and zero-length target vectors. “You can turn all the way around” is an acceptance criterion, not a screenshot.

### Command deck structure

Use five primary doors, with persistent focus and a small breadcrumb:

1. **Fly:** Resume/Launch, Targets, Local Map, Galaxy Map, Comms.
2. **Ship:** Cargo, Loadout, Power, Shipyard, Outfitting.
3. **Work:** Mission Board, Mission Log, Factions, GalacticNet.
4. **Discover:** Codex, Photographs, Research, Outpost/Fleet when unlocked.
5. **Commander:** Save/Load, Controls, Accessibility, Audio, Debug.

Docked services are visible as unavailable in flight, with “Dock to view this station's stock.” They must not reveal remote live prices through a secondary page. GalacticNet may show historical price reports explicitly dated “last visit” or qualitative rumours, not secret live shop inventory.

The deck opens paused in this offline game. Returning restores the prior camera, focus and throttle. Destructive purchases, mission abandonment and overwriting saves have clear confirmation; ordinary navigation does not.

### HUD specification

- Minimal is the new-player default: small reticle, speed/fuel, selected target and compact radar. Full is an optional instrument panel.
- Scenic hides informational text, borders and reticle. Explicitly chosen danger alerts may briefly show; a strict photo mode freezes the scene and removes all overlays.
- Keep at least 80% of the vertical view available in Minimal; target names appear near the object but never across the reticle.
- Maximum one routine objective and one priority alert. Radio speech waits behind a safety alert; duplicate alerts coalesce.
- Wanted: five outlined star sprites with filled positions. Danger: five filled/unfilled warning pips plus a word: Calm, Watched, Unsettled, Hostile, Warzone.
- Faction identification combines colour, shape and label: Trader diamond, Law shield, Pirate wedge, Explorer compass. Do not rely on colour alone.
- Font: retain pixel sharpness; offer 8×8 normal and 8×12 readable text with reflow. No ellipsis on the action needed to progress.

## 7. Travel, speed and recovery

Normal cruise reaches the station in a comfortable short trip. Boost is for interplanetary travel, not a mandatory escape from boredom. Target an ordinary station trip of 20–45 seconds and a boosted planet trip of 25–60 seconds after route selection. Adjust world layout and travel speed together.

**Normal speed lines:** begin around 80% cruise, sparse peripheral particles, no central obstruction. **Boost lines:** denser and longer with an engine pitch rise and strong entry/exit cues. Lines scale with actual velocity, not merely the held button. Stop them in a frozen menu, docking cinematic, landed state and ordinary EVA.

Boost spends fuel, but a reserve locks out further boost before consuming the minimum return/safe-warp allowance. Show “Reserve fuel protected.” The player may explicitly override in advanced controls. A free emergency tow to the nearest legal hub prevents a softlock; it may forfeit unprotected ordinary cargo, never story-critical cargo. Recovery terms are shown before accepting.

Warp requires a reachable destination, sufficient fuel and no incompatible modal state. Reserve the cost at initiation; commit arrival once. All scene changes invalidate runtime target handles. The destination is immutable during the transition. Cancelling before commitment refunds the reservation.

## 8. Stations and docking: the trust-critical feature

Every station has a persistent identity, name, transform, spin axis, angular speed, entrance transform and collision definition. A cosmetic station mesh must never supply a contradictory invisible collision wall.

### Manual docking

1. A glowing entrance, short approach beacons and a rotation marker show the actual opening.
2. Transform the swept player segment into the station's local docking frame, including rotation at the relevant crossing time.
3. Detect crossing of the entrance plane from outside to inside. Check corridor bounds, heading, roll tolerance and speed before processing the surrounding hull hit.
4. A valid crossing captures the ship into the docking state exactly once. Disable player weapons, collision damage and free-flight input during the captured sequence.
5. Play a 2–3 second exterior ship-entry shot, then a one-second welcome: “Welcome to {station_name}.” Open services only after the welcome completes.

Starter tuning: maximum docking speed 200 in existing simulation units; lateral clearance based on player hull radius; heading error ≤15°; roll error ≤20°. Assisted manual mode adds a gentle correction only inside the approach corridor. Tighten later only if docking remains enjoyable on the nub.

The station rotates slowly enough for a novice to read its motion: initial target 1–2° per second, scaled only modestly for station type. Different station silhouettes and sizes share the same port contract. Large hubs may have rings and distant traffic, but only the lit assigned port accepts docking in the first release.

Hitting the surrounding hull outside the port produces the requested fatal crash and wireframe explosion. It must not trap the camera, continuously subtract damage, or open the shop afterwards. A valid entrance trajectory never counts as a hull crash.

### Guided docking

Hail a nearby hub or use Circle while it is the valid context target. Standard communication range is 2,500 m; docking computer extends to 8,000 m. Denials state the reason and next action. A wanted player receives the police choice rather than an unexplained refusal.

Guidance owns a staged path: clear hull → approach point → align moving slot → enter → external shot → welcome → services. Test rear, side, above, ring-interior and boosted requests. If no safe path exists, hold at a safe waypoint and offer Cancel; never interpolate through the station.

The cinematic can be skipped after the first viewing, but the transition still commits its rewards and save checkpoint exactly once. On resume after suspend, rebuild from the stage and validated anchors, not a half-updated position.

### Hub variety

Core: compact Coriolis-derived trade hub, elongated industrial dock and large ringed civic hub. Expansion: research spindle and damaged frontier outpost. Shape, lighting, market speciality and audio distinguish them. Entry port colour remains consistent for usability.

## 9. Lave, factions, traffic and danger

Lave is fixed at Calm. Pirates do not select the player as prey there. Distant pirate-versus-Law encounters remain possible, but mission-critical routes and station approaches receive a safety bubble. Deliberate attacks on civilians still cause local consequences; starter safety is not immunity from chosen crimes.

| Faction | Motivation | Normal behaviour | Response to player |
|---|---|---|---|
| Traders | Deliver cargo and preserve ships | Dock, depart, form small convoys, divert around danger | Trade tips; distress calls; flee aggression |
| Law | Protect legal traffic locally | Patrol lanes, escort, intercept known offenders | Warn, inspect, fine or arrest within this system |
| Pirates | Obtain valuable cargo with tolerable risk | Scout, shadow, demand cargo, raid isolated convoys | Telegraph a demand before most attacks outside Lave |
| Explorers Guild | Survey routes and preserve knowledge | Visit bodies in loose groups, scan anomalies, assist | Share clues, pay discoveries, offer research work |

Danger affects encounter probability and preparedness, not just health multipliers. Proposed encounters per 10 active minutes on normal routes: Calm 0 player ambushes; Watched 0–1; Unsettled 1–2; Hostile 2–3; Warzone 3–4. These are scheduler budgets, not guaranteed attacks on a timer. Combat cooldown, current mission, fuel, location and recent recovery can suppress an event.

Traffic plans exist independently of the player: origin, destination, faction, cargo class, group ID and current intent. Simulate nearby ships fully; distant routes update at low frequency; unloaded systems retain compact schedule records. A trader actually reaches a destination or leaves the region. Do not replace every ship with another chasing the camera.

Avoidance priorities: station and terrain clearance; immediate collision separation; formation offset; mission/route destination; visual banking. Use bounded turn rates and acceleration. Explorers make broad arcs around a survey point. Pirates break attack runs instead of orbiting forever. Police abandon a chase at a system transition.

Freighters have readable slow movement, a different sound, distinct collision bounds and escort lanes. Core battles cap detailed actors; distant volleys and silhouettes imply scale without pretending to simulate hundreds of ships. Never place a huge freighter across the mandatory docking corridor.

## 10. Combat, crime and defeat

### Combat loop

Identify intent → decide fight, flee, hail or surrender cargo → manage SYS/ENG/WEP → use terrain/formation gaps → resolve the encounter → salvage or continue. A pirate demand gives 8–12 active seconds to respond unless the player has already fired.

Weapons must have different jobs: pulse laser for efficient short bursts; beam for tracking with heat risk; mining cutter for seams and machinery; missiles for a costly burst against a locked hostile. All communicate range, heat and lock failure. Camera-facing UI does not confer line of sight through a planet.

Shield feedback precedes hull damage. Incoming missile warning gives direction and remaining time; evasion has clear speed/obstruction criteria. No offscreen missile spawns inside the player's collision radius.

### Local law contract

Preserve system-local warrants and the player's requested fine/custody interaction. Buying locally prohibited goods, assault and destruction record a crime type, severity and jurisdiction. Goods can be prohibited in one system and legal elsewhere; show this before buying. Prevent repeated per-frame crime reports from a single hit.

At interception, the world and mission clocks pause. Display the offence, fine, available money and custody consequence. Paying clears the local warrant after one debit. Custody returns the player to that local hub, takes the stated capped release payment, clears the warrant and protects essential mission items. Insufficient money must not trap the player in the dialog.

Retain existing 50 units per wanted star and 25 per star custody payment as compatibility defaults until the new economy is calibrated. Add a one-minute grace period after release unless a fresh crime occurs. Faction reputation changes may persist across systems; the legal pursuit itself does not.

### Defeat without contempt for the player's time

Default Adventure mode offers recovery at the last safe hub, loss of a capped fraction of unprotected cargo and a modest repair bill. Do not delete the commander. A separate Iron Commander setting may enable harsher losses, clearly explained at creation. Story-critical evidence, established discoveries and accessibility settings survive normal defeat.

Wireframe fragments come from the player's actual hull. Slow the final beat briefly, show cause and a useful recovery choice. Avoid a long unskippable death sequence.

## 11. Economy, inventory and ship growth

Preserve the recognizable 17-goods foundation. Present it as understandable commodities, not 17 unrelated numbers. Each system has a production profile, demand profile, prosperity tier, technology tier and security rating. Industrial/agricultural classification is separate from wealth: an agricultural world can be prosperous.

**Inventory domains:** cargo in tonnes; tools and upgrades in fitted slots; mission cargo in reserved capacity; samples/evidence in a small protected research case. Mission acceptance validates capacity and reserves it. The player cannot sell a sealed mission package without an explicit abandon/confiscate decision.

Market list: item, price, hold quantity, station quantity. Detail pane: average comparison, legal status, volume, a short demand explanation and last known price elsewhere. Batch controls offer 1, 5 and maximum affordable/available; preview the total before a batch transaction. One purchase action performs one checked integer transaction.

Prices are integer tenths of a unit internally. Store original cost for ordinary cargo to show likely profit. Avoid selling upgrades for more than purchase cost or cycling ship exchanges for free money. Stock replenishes on a deterministic active-game schedule; save/reload and entering a menu do not reroll it.

### Initial balance targets

| Activity | Active time | Base net payout target | Additional value |
|---|---:|---:|---|
| Local starter delivery | 4–6 min | 120–180 units | Trade contact |
| Common first scan set | 3–5 min | 100–160 | Research progress |
| Nearby intersystem delivery | 7–10 min | 250–400 | Route reputation |
| Rescue | 6–10 min | 200–350 | Character relationship |
| Mining/salvage outing | 5–8 min | 150–300 | Crafting materials |
| Pirate hunt | 6–10 min | 250–450 after ammunition | Security reputation |
| Risky covert delivery | 8–12 min | 400–650 | Greater legal exposure |

First useful upgrade target: 15–25 minutes. First alternate hull: 60–100 minutes of varied play. A mature trade route should remain worthwhile without making discoveries financially irrelevant. Test net return after fuel, repairs and cargo costs.

### Upgrade roster

| Upgrade | Visible effect | Source | Tradeoff |
|---|---|---|---|
| Docking computer | Guided request range 2.5 → 8 km | Technology 4+ | Nav slot |
| Shield booster | +25% shield capacity | Technology 6+ | Higher SYS demand |
| Laser cooling | +25% cooling rate | Technology 5+ | Weapon support slot |
| Cargo rack | +8 t, current baseline compatibility | Industrial hubs | Modest turn/acceleration penalty |
| Long-range scanner | Identifies distant threats and valuable signals | Guild / technology 6+ | More signature while active |
| Fuel scoop | Collects fuel in a marked safe stellar band | Technology 5+ | Heat management |
| Survey array | Reveals one extra clue at a biological site | Guild research | Tool slot |
| Tractor clamp | Captures/tows one salvage object | Industrial mission | Reduced boost while towing |
| Hazard lining | Doubles safe excursion time in one environment class | Research | Choose a speciality |
| Rescue berth | Carries one additional rescued person safely | Civic hubs | Cargo space |
| Quiet drive | Slower pirate detection while coasting | Frontier contacts | Lower peak thrust |
| Field fabricator | Makes three essential consumables aboard ship | Late core | Material cost |

Every item shows exact before/after stats, fitting conflicts and whether it is already installed. Sell/uninstall options cannot strand the player by removing the only necessary recovery tool.

## 12. Discovery, gamification and lasting rewards

Three career tracks: **Pathfinder** (survey/exploration), **Broker** (trade/logistics), **Guardian** (rescue/security). Award points for completed outcomes, with diversity bonuses for a new region or problem type. Do not award unlimited progress for repeatedly scanning the same specimen.

Proposed rank thresholds per track: 0, 100, 250, 500, 900, 1,400. Rewards alternate between cosmetics, useful access and modest sidegrades. No rank grants a permanent damage multiplier that makes starter areas meaningless.

Examples:

- Pathfinder II: identify a biome from orbit; IV: place a research beacon; VI: a unique survey livery.
- Broker II: compare stored prices; IV: reserve one hub shipment; VI: a decorated freight licence and extra logistics job choice.
- Guardian II: distress signal priority; IV: one escort assist per sortie; VI: a commemorative hull plate and specialist rescue missions.

**Collections:** a region's four landmark postcards; a three-species ecological relationship; six fragments of one wreck's history; a set of trade-route stamps. Completion rewards something usable or displayable. Partial collections still reward individual finds.

**Mastery medals:** clean manual docking, return from a high-risk survey without firing, rescue both crews in a branching incident, map a route without entering reserve fuel, identify all species at a site without disturbing them. These are optional, permanent and not required to progress.

**Discovery quality:** basic scan identifies; observation reveals one behaviour; contextual evidence links it to another organism or ruin. A rare palette alone is not a new meaningful species. Each Codex entry contains one visual, one useful fact and one short piece of evocative writing.

The reward screen answers three questions: What did I do? What changed? What can I try next? Use a small sound and one card, not five overlapping XP bars.

## 13. Procedural systems that create places

Persist a universe seed and generator version. Derive independent streams for galaxy, bodies, terrain, flora, traffic, markets and missions. Rendering particles and audio must not consume gameplay RNG.

System identity = star palette + nebula family + economy + security + station family + world mix + one unusual motif. Constrain combinations for legibility. A blue star does not automatically mean ice; use deliberate world temperature parameters rather than accidental palette logic.

Core world families: temperate archipelago, red dune basin, pale ice shelf, violet fungal plain, copper badlands and airless ruin. Each has a distinct silhouette, movement hazard, material and optional story clue. Water worlds always have a guaranteed dry first landing pad. Gas giants and stars are approachable vistas/hazard regions, not fake landable surfaces.

For each surface region guarantee: safe entry volume; return ship/pad; two normal resources; one observation site; one optional risk/reward landmark; a traversable route among essential sites. A validator rejects impossible layouts before the player sees them.

Use weighted landmark grammars: quiet foreground → readable trail → discovery silhouette → interaction space → return vista. A randomly scattered hundred objects is not a designed location.

Orbit and ground must agree on palette, cloud cover, day side and named body. Store the orbital anchor before entry and return outside the atmosphere facing away. Never regenerate the whole solar system merely because the player left a surface scene.

## 14. Surfaces, EVA and meaningful life

Proposed core replaces the flat test pad with bounded heightfield regions, initially about 1–2 km across. One region can feel substantial when sightlines, paths and landmarks are deliberate. Additional sites on the same planet load as separate regions through a clear travel action.

Terrain uses low-frequency height plus a few authored features. Flatten pads and mission approaches, cap required walking slopes, and provide a fallback traversal corridor. Visible geometry and collision sample the same height function/version. Water has a clear shoreline and an explicit movement rule.

On-foot loop: leave ship → follow a signal or landmark → observe/repair/sample → choose whether to risk one more find → return and bank the result. Baseline oxygen does not drain in safe environments. Hazards are specific: electrical storm, extreme cold, toxic plume. Each has a visible boundary, audio cue and a shelter/reversal option.

Jetpack has a clear charge bar and predictable arc. A depleted pack never leaves the player on an inescapable shelf generated by the mission. “Return to ship” recovery is available if the player is stuck, with a visible tradeoff and no loss of story items.

### Creature behaviour

Core species roles: grazer, burrower, flock animal, scavenger and guardian. Behaviour states: rest, forage, notice, flee, investigate and defend. Most life is peaceful. Use environmental behaviour to create scan opportunities: a grazer visits luminous flowers at dusk; a burrower follows vibration; a scavenger exposes a buried fragment.

At most one rare defensive encounter per ordinary survey region. Let the player learn and avoid it. Attacking harmless animals does not grant a superior progression path.

Space whales are rare authored/procedural encounters with a migration lane and one scan opportunity. Core gives a safe observation contract and Codex record; expansion adds a rescue from abandoned nets. Whales have bounded movement and no enormous physics skeleton. Asteroids in the active mining set have real collision and yields; distant belt decoration is explicitly not individually targetable.

## 15. Salvage, crafting and outposts

Core crafting uses at most twelve material categories, no more than two ingredients per essential recipe, and three-input recipes only for special projects. Materials live in shared storage at a hub; the crafting UI automatically shows available versus carried stock.

Starter recipes: repair patch (alloy + sealant), survey charge (crystal + cell), hazard pack (fibre + coolant), beacon (alloy + circuit). Exact resource names are original to this game; do not copy another game's full resource taxonomy.

Salvage choice: scan an object, identify hazardous and valuable sections, cut one attachment, capture it and return. A damaged object can be safely abandoned; losing a bonus object cannot invalidate the main mission. Tractor physics is a constrained spring/tow line with maximum mass and distance, not a universal simulation.

Expansion A outpost: one landing site, 12 module slots, six functional module types (power, shelter, storage, workshop, survey, garden). Connectivity is a simple socket graph. Cosmetic props have a separate cap. Preview cost and clearance before placement. Essential pad access cannot be blocked. Stored supplies persist immediately after successful placement transactions.

An outpost gives reasons to revisit: a prepared expedition kit, a new observation clue, a visiting character or a project choice. It does not punish absence. Resource production advances only through active play milestones, never while the PSP is switched off.

## 16. Capital ships, stations and late-game ownership

Expansion A awards one modest freighter through a mission, not a random grind. It becomes a mobile home with three interior rooms loaded separately: bridge, cargo bay and workshop. Crew have roles and dialogue, not dozens of opaque stats. A mission planner assigns up to three abstract support sorties; the player can intervene at a generated encounter, but the fleet does not simulate in full while elsewhere.

Modular corvette interpretation: select a hull, a utility module and an engine family. All combinations come from authored connection points with prevalidated collision/entry geometry. A preview shows performance and cargo effects. No freeform part overlap or unbounded construction.

Expansion B station council: restore one frontier hub through delivery, survey and rescue projects. Choose one of three service specialities. The exterior gains a visible module, a named character moves in, markets and mission offerings change. The player can change specialisation later through another project; a story choice cannot permanently lock out basic repairs.

The final campaign decision changes who governs a navigation relay, not who owns every system. Ownership should make a place personal without turning the whole game into an administrative dashboard.

## 17. Optional feature cards: each must be a complete game

These are designed expansion targets, not permission to add menu placeholders.

### Archaeology — The Shape of an Absence

Discover a ground echo; choose brush, cutter or stabilizer based on scan evidence; expose one fragment; assemble a six-piece exhibit at a hub. Wrong tools reduce the optional specimen grade, not mission access. A complete exhibit unlocks a story recording and decoration. Use a 2D assembly panel with silhouettes, stable fragment IDs and three authored final shapes. Test duplicate fragments, full cargo, leaving mid-dig and reconstruction after loading.

### Rover — Tracks Between the Lights

Choose survey or cargo fitting at a pad; drive to two sites; collect or recover; return. Bounded heightfield, wheel contact rays, simple steering and a self-right action. No mission requires a jump over a physics-dependent gap. A route stamp and a useful sample reward skilled driving. Ship retrieval remains possible if the rover is abandoned.

### Companion — A Small Passenger

Observe a peaceful creature three times in different contexts; choose to invite it; build trust by participating in an optional activity. It can point to a nearby clue, fetch a small sample or warn of a hazard. One companion active, three stored records initially. Missing a day never reduces affection. Palette and behaviour variation are saved, and dismissing it safely returns it home.

### Creature league — Holographic Field Trials

At a hub, select two scanned species and one research tactic. Three-turn rounds use Guard, Probe and Burst, plus one species trait. An opponent telegraphs a tendency. Victory earns league stamps and cosmetics, not superior main-campaign weapons. The displayed animals are simulations. Twelve opponents and one repeatable seeded bracket are enough. No breeding or online matchmaking dependency.

### Shore activity — The Quiet Catch

At marked safe water, select bait category, read ripple timing and keep a short tension bar in range. A failed catch loses a few seconds, not expensive equipment. Record species, release or cook. Recipes grant a modest temporary expedition comfort benefit, never a mandatory hunger loop. Six fish silhouettes and two habitat conditions can support this entire optional loop.

### Underwater — The Bell Below

A docked submersible enters a separate reef region. Follow beacon lines, identify a ruin, retrieve a recording and return before optional oxygen bonus time ends. Three landmark rooms, sprite kelp, capped particles and simplified buoyancy. Entry always creates a return marker and emergency ascent route. No surface-to-ocean streaming requirement.

### Space hulk — The Last Useful Thing

Enter through a docking tether, map up to six rooms, detach salvage and choose between one more item and a safe exit. Removing structural salvage reduces a deterministic integrity score. Warning thresholds are explicit. Exterior ship, interior room graph and salvage payloads share stable IDs. A final 30-second evacuation timer begins only after a clearly confirmed risky cut; safe salvage has no hidden countdown.

### Swarm event — The Lantern Engine

An original automated mining network becomes aggressive. Disable three visibly telegraphed relay nodes, then expose the core. Maintain at most six fully simulated enemy escorts; distant drones are harmless visuals. A noncombat route is available through earlier research and a repair/hack action. This is a authored event, not an unbounded “epic battle” spawner.

### Offline expeditions — The Long Weekend

Choose a permanent itinerary of five objectives with a printed seed code. Rewards are badges, liveries, ship ornaments and journal pages. Replay with another approach. No expiry date, exclusive login reward or calendar dependence. Content is installed with normal game updates; an optional future import format must validate its data before use.

## 18. Accessibility and comfort

Rebind or expose alternate presets for required chord actions. A single-button menu route must exist for boosting, targeting, missiles, HUD mode and roll assistance. Adjustable analog deadzone, invert pitch, hold/toggle boost, auto-roll levelling, camera shake, flashes, motion lines, subtitle speed and music/SFX volume are required settings.

Support a low-flash mode: no white-screen warp, rapid emergency strobes or high-frequency star flicker. Threats still communicate through shape, sound and words. Colourblind palettes retain faction glyphs. Story speech waits for acknowledgement by default; important lines go into a transcript.

Tutorial instructions name the actual action and currently bound button. Never say “press the action button.” A player who has forgotten the game after a week should find a short “Where was I?” card in the mission log.

## 19. Audio and emotional pacing

Keep the existing integer audio path as the baseline. Use a small priority queue: collision/interception alert > incoming missile > dialogue chirp > engine/transitions > UI tick. Routine ticks never cut off a danger cue. Audio thread work must remain bounded and must not allocate memory per buffer.

Three music moods: safe harbour, open sky and pressure. Crossfade simple layers rather than launching another synthesizer per actor. Silence is part of exploration. A rare whale call should not compete with a constant scanner beep.

Characters use a brief radio identity sound and text, not hours of sampled speech. Different speakers have different cadence, vocabulary and priorities. Restrict jokes during casualties; let humour return at safe moments.

## 20. Mission framework and clock policy

Support five tracked jobs plus the separate main-story chapter. One job is focused for navigation; focusing does not overwrite the others. The board filters unreachable or impossible offers before displaying them.

Offer card: objective, destination, estimated active duration, cargo/gear required, legal risk, guaranteed reward, optional bonus, failure cost and timer policy. Accepting reserves requirements and stable mission entities in one transaction.

Most jobs have no deadline. Timed jobs are clearly marked **Urgent** and measure active simulation time only. Pause, ordinary menus, dialogue, police choices, cinematics, loading and PSP suspend freeze those clocks. A special in-world countdown must explicitly say that the world is continuing, and still pause on system suspend. This deliberately supersedes current always-five-minute behaviour.

Mission states: Offered → Accepted → Travelling → OnSite → ObjectiveResolved → Returning (if required) → RewardClaimed. Terminal states: Completed, Failed, Abandoned. Each transition has one idempotency key. Credit, reputation, inventory and story rewards commit once, together.

Reserve separate capacity for story evidence so ordinary cargo cannot softlock it. A required actor that becomes unavailable is reconstructed from its persistent mission identity or replaced by a documented recovery objective. It must never silently point to whoever reused its NPC slot.

## 21. Narrative premise and cast

### The Open Channel

Ryn has found that a chain of old navigation relays carries the migration signals of enormous spaceborne organisms. A shipping combine, **Pale Meridian**, has been falsifying route hazards to monopolise safe travel and extract energy from the relays. Ryn went silent to protect the creatures and the settlements dependent on those routes. She is alive, but her rescue alone will not solve the problem.

The player's work uncovers the truth through cargo records, observations, rescue testimony and a recovered recorder. The final decision distributes control of the navigation network between local communities, a regulated authority, or the independent Guild. All endings reconnect the isolated settlements; they differ in trust, access and future work.

This story is original. Existing “Atlas echo” labels become **Meridian echoes** for new content; old save IDs retain compatibility aliases. Do not import No Man's Sky's Atlas mythology or characters.

| Character | Role and voice | Want / flaw | Visual brief |
|---|---|---|---|
| Kei Aven | Guild handler; precise, warm, economical | Wants Ryn home; mistakes control for care | Muted lavender-gray alien woman, pearl-gray hair, subtle antennae, navy jacket; see UI-SPEC.md |
| Ryn Vale | Surveyor; patient, observant, stubborn | Wants routes protected; excludes friends to protect them | Sun-faded scarf, patched suit, hand-drawn specimen notes |
| Venn Sorel | Lave dockmaster and useful gossip | Wants safe arrivals; hides worry behind dry humour | Heavy work vest, broad hands, cyan berth lamp |
| Marshal Iona Renn | Local Law commander; measured and fair | Wants evidence; trusts procedure longer than she should | Blue collar, worn badge, no villainous sneer |
| Nadi Quill | Independent salvager; fast, inventive, candid | Wants her own tug; overpromises | Orange tool harness, mismatched gloves |
| Dr Oru Sen | Field ecologist; delighted but exact | Wants living systems understood, not collected indiscriminately | Green visor, specimen sketches, gentle posture |
| Captain Tamsin Ro | Freighter captain; practical, protective | Wants every crew member returned; accepts too much risk herself | Gold flight coat, old convoy patch |
| Sable Marr | Pirate negotiator; courteous, calculating | Wants independence; confuses freedom with impunity | Red scarf, immaculate borrowed coat |
| Director Halden Voss | Pale Meridian executive; reasonable-sounding coercion | Wants reliable profitable routes; treats people as costs | Pale suit, small silver relay pin |
| Pip | Ryn's compact maintenance drone; literal, unexpectedly funny | Wants to complete its checklist; cannot rank emotional priorities | Three-legged yellow sprite, one expressive lens |

Writing rules: fluent natural English, specific observations, no pseudo-mystical filler. Characters do not all speak in clipped fragments. An important instruction gets a plain action sentence after the emotional line. Dialogue is stored as full text and wrapped by the UI, never manually truncated into broken grammar.

## 22. Campaign production rules

The following twelve chapters are the complete proposed main campaign. Their dialogue is original authored copy, ready for editing into content data. They are not already implemented in `story.h`.

**Route binding:** Lave is the existing system index 7. All other story locations use semantic roles such as `NEARBY_AGRI`, `FRONTIER_RELAY` and `INDUSTRIAL_HUB`. At campaign creation, select reachable systems using the actual galaxy graph, current starting jump range and guaranteed refuelling hubs. Store those bindings permanently. If no valid route exists, fall back to another validated candidate; never invent a named system and assume it is within range. Names such as “Stillwater” below are authored site names within the bound system, not replacements for Elite's generated system names.

**Story rewards:** numeric units below are starting values, awarded once. Optional objectives add side rewards; they never withhold the next chapter. Important dialogue remains in the transcript. The story can be paused for ordinary work and resumed from the Mission Log.

**Common recovery:** death returns to a safe checkpoint; essential items are protected; replay reconstructs the same mission identity; abandoning a story chapter suspends it rather than destroying it. Ordinary combat defeat never changes a character from alive to dead without an authored branch.

### Campaign map

| ID | Chapter | New play skill | Main payoff | Approx. active minutes |
|---|---|---|---|---:|
| OC01 | A Ship with Your Name on It | Fly and dock | A home and a reason to leave | 8–12 |
| OC02 | Something Worth Carrying | Cargo and route planning | Venn's trust; first income | 10–15 |
| OC03 | The Quiet Between Pings | Observe and scan | Evidence of a living signal | 12–18 |
| OC04 | All Hands Accounted For | Rescue / resolve encounter | Witness and choice of method | 15–20 |
| OC05 | A Lawful Mistake | Evidence and local law | Iona becomes an ally | 12–18 |
| OC06 | The Last Useful Thing | Salvage and repair | Ryn's unedited recorder | 15–25 |
| OC07 | No Cargo Is Just Cargo | Convoy logistics and ethics | An isolated community survives | 18–25 |
| OC08 | Where the Giants Turn | Follow a migration signal | Discover Ryn's refuge | 15–20 |
| OC09 | The Person on the Other End | EVA recovery and dialogue | Ryn returns as a person, not a prize | 15–25 |
| OC10 | A Map That Belongs to Everyone | Prepare with allies | A plan shaped by prior choices | 20–30 |
| OC11 | The Open Channel | Final multi-role operation | Reopen the route; decide governance | 20–30 |
| OC12 | A Berth Kept Warm | Return and reflect | Closure and a continuing sandbox | 8–12 |

This table sums to less than the 6–8-hour first-playthrough target because travel, optional work, exploration and learning occupy the remaining time. Do not artificially stretch chapter timers to force that duration.

## 23. Complete campaign scripts — Act I: A place to return to

### OC01 — A Ship with Your Name on It

**Approved character art (1.6.0):** Kei Aven is an adult female alien with lavender skin, fluffy pearl-white hair, small antennae, teal eyes and a warm, gentle manner. Use the new Kei atlas for her campaign and Comms identity. See PROGRESS.md for the implemented chapter boundary.


**Start:** New Guided Commander at Lave System Hub. **Actors:** Kei, Venn. **Required:** starting ship, full local-trip fuel, no weapon purchase. **Clock:** none. **Objective:** launch, lock the hub, make a short flight and dock. **Reward:** 100 units, harbour badge, OC02.

**Opening conversation**

KEI: “I'm Kei Aven, Explorers Guild. The ship is yours for as long as you need it.”

PLAYER CHOICE A: “What's the catch?”

KEI: “There is a missing surveyor named Ryn. I need someone who will look carefully. But first, I need you to come back from a flight.”

PLAYER CHOICE B: “Was it Ryn's ship?”

KEI: “She flew one like it. I keep almost calling it hers. Ignore me if I do.”

KEI, instruction: “Open Controls if you need them. When you're ready, choose Launch.”

**First flight, after the player has steering control**

VENN: “Lave traffic, new commander leaving berth six. Give them a little room.”

KEI: “See the cyan entrance behind you? That's home. Lock the hub so your compass can bring you back.”

VENN, if the player requests guidance: “Clearance granted. Keep your hands off the controls for a moment. I'll show you the way in.”

VENN, if the player docks manually: “Clean entry. You can pretend that was easy; I won't tell anyone.”

**Arrival**

SYSTEM: “Welcome to Lave System Hub.”

KEI: “Good. Now I know where to find you.”

**State specification:** `briefed → launched → hub_locked → flight_distance_met(600m) → docking_completed`. Alternative success accepts a legitimate docking after launch even if the target tutorial was skipped; explain the compass afterwards rather than blocking completion.

**Recovery:** guidance always available within range; crash resumes from launch checkpoint with this chapter's training cargo/fuel intact. No unlimited training reward farming. **Acceptance:** reward cannot be claimed twice; cinematic opens services once; Start during the sequence follows the defined docking policy.

### OC02 — Something Worth Carrying

**Start:** Venn's board at Lave. **Location:** `NEARBY_AGRI` hub, bound to a reachable low-risk system. **Payload:** sealed greenhouse pump, 1 t reserved. **Clock:** none. **Reward:** 180 units, Broker +40, a saved price report, OC03.

VENN: “A greenhouse pump. One tonne. No teeth, no weapons, no reason to be interesting.”

KEI: “You're making it sound suspicious.”

VENN: “I'm trying to make honest work sound exciting. It's harder.”

PLAYER: “What happens if I take my time?”

VENN: “They use the spare. Then the spare breaks, and they call me names. Bring it over when you're ready.”

**At the destination**

RECIPIENT, Mara Pell: “You brought the proper coupling. Last shipment was a box of door handles.”

PLAYER CHOICE A: “Venn checked it himself.”

MARA: “Then tell him I take back half of what I said.”

PLAYER CHOICE B: “Anything useful for the return trip?”

MARA: “Textiles are plentiful here. Check the market before you buy; yesterday's bargain is only a rumour today.”

**Completion radio**

KEI: “Ryn used to stop here. Mara found something in an old receiver. It's on your map.”

**Stages:** accept/reserve → select route → arrive → dock/deliver → optional ordinary trade. The chapter completes on delivery, not on purchasing another item. **Recovery:** damaged mission package becomes a repair task paid by the employer; no negative balance. **Acceptance:** reserved cargo is not sellable; board acceptance fails clearly if a valid route cannot be bound.

### OC03 — The Quiet Between Pings

**Location:** Stillwater Listening Post on a safe solid world in `NEARBY_AGRI`. **Actors:** Oru, Kei. **Actions:** enter atmosphere, land, repair receiver, observe a grazer near luminous flora, compare two signals. **Reward:** 200 units, Pathfinder +60, survey-array blueprint, OC04.

ORU: “Don't scan everything at once. Sit with one thing long enough and it stops looking like scenery.”

PLAYER: “What am I looking for?”

ORU: “The little grazer beside the flowers. Its throat light pulses before the receiver answers. Ryn noticed it first.”

**Observation trigger:** remain within scan range without firing for five active seconds while the grazer feeds. The behaviour is guaranteed on a bounded cycle; the player is not asked to wait for real nighttime.

ORU: “There. A reply, not an echo.”

KEI: “She sent me a recording of that rhythm. I thought the microphone was faulty.”

PLAYER CHOICE A: “You couldn't have known.”

KEI: “I know. I still wish I'd listened.”

PLAYER CHOICE B: “We can listen now.”

KEI: “Yes. Send me the clean sample.”

**Receiver repair puzzle:** two connectors have matching pulse symbols; selecting the wrong pair resets the local attempt, with no item loss. **Recovery:** receiver and specimen use stable IDs; re-entry restores repaired/scanned state. **Acceptance:** the player can decline planet entry, turn away and return without moving the post or duplicating scans.

## 24. Complete campaign scripts — Act II: People in the route

### OC04 — All Hands Accounted For

**Location:** a convoy distress site in `TRADE_CORRIDOR`. **Actors:** Tamsin, Nadi, Sable. **Situation:** a freighter is disabled; a pirate group wants its sealed cargo. **Reward:** 300 units, Guardian +60, Tamsin relationship, witness statement, OC05. **Optional:** save a second maintenance drone for a cosmetic tug charm.

TAMSIN: “This is the freighter Long Measure. Engines are out. Nine aboard, nine accounted for. I'd like to keep that number.”

SABLE: “No one needs to die over a sealed box, Captain. Release it and we'll leave.”

TAMSIN: “I don't know what's in it. I do know whose names are on my manifest.”

Three playable approaches:

- **Escort:** defeat or drive off two attackers while Nadi repairs the drive. Enemy waves are capped and visible.
- **Negotiate:** hand over a decoy transponder prepared by Nadi, then tow the freighter beyond the search cone. Optional payment comes from the mission fund, not an affordability gate.
- **Rescue:** extract the crew into a protected rescue transfer and mark the freighter for Law recovery. No ship-capacity impossibility: rescue launches a shuttle transaction, not nine tonnes silently squeezed into the player's hold.

NADI: “I can make the box look more expensive than it is. That's almost my profession.”

SABLE, after the negotiated route: “You have a very persuasive radio. Try not to make a habit of using it on me.”

TAMSIN, completion: “You saved people you hadn't met. That goes in my log before anything else.”

NADI: “The cargo seal belongs to Pale Meridian. Someone paid pirates to steal their own shipment.”

**Failure-forward:** if the freighter hull reaches the mission threshold, switch to lifeboat recovery; crew casualties are not randomly rolled. **Acceptance:** all three approaches reach OC05; a noncombat ship can finish; duplicate rescue interaction cannot duplicate passengers or rewards.

### OC05 — A Lawful Mistake

**Location:** `SECURE_HUB`. **Actors:** Iona, Kei, Pale Meridian representative. **Situation:** the recorder is flagged as stolen property. **Reward:** 250 units, local warrant resolution for this scripted incident, evidence receipt, OC06.

IONA: “Commander, your cargo beacon matches a theft report. Reduce speed. We can settle this without anyone making a poor decision.”

PLAYER: “The report is false.”

IONA: “Then give me something better than a second assertion.”

**Choices:** present Tamsin's witness statement; submit to station inspection; pay the normal fine and later challenge it. Each route preserves the evidence. This is a separate scripted case ID; it must not erase unrelated player crimes for free.

At the station, compare seal time, convoy log and shipping route. Correct evidence is highlighted by observable contradictions, not by guessing legal jargon.

IONA: “The theft report was filed six hours before the shipment departed.”

REPRESENTATIVE: “Administrative systems are imperfect.”

IONA: “So are people. That's why I prefer three records to one confident man.”

KEI: “Are we clear?”

IONA: “For this shipment. And Commander—keep copies.”

**Recovery:** if the player paid first, refund that scripted fine after verification. Custody never consumes the evidence. **Acceptance:** all inputs remain frozen under interception; case rewards and refunds are each idempotent; ordinary local warrants remain local.

### OC06 — The Last Useful Thing

**Location:** damaged relay tender at `FRONTIER_RELAY`. **Actors:** Nadi, Pip, Ryn recording. **Core implementation:** exterior salvage and three repair nodes. **Expansion replacement:** six-room derelict interior with the same mission ID and outcomes. **Reward:** 350 units, tractor blueprint, recovered route key, OC07.

NADI: “That wreck has three useful things left: a battery, a recorder and a door that hasn't fallen off. We need the recorder.”

PIP: “Correction. The door has fallen off.”

NADI: “Excellent. One fewer obstacle.”

**Play:** scan structural supports → power the recorder from the intact battery → choose safe removal or optional extra salvage → tow/collect. The safe path has no countdown. A risky optional cut explicitly starts a 60-second evacuation bonus challenge.

RYN, recorder: “Kei, if this reaches you, don't follow my last published route. They have it. Follow the pauses in the signal. The creatures leave room for one another. The company doesn't.”

KEI: “That's her. That's not an old log.”

PIP: “Recording age: nineteen days. Confidence: high.”

NADI: “Nineteen days is a head start. It's not an ending.”

**Recovery:** recorder recovery is protected before optional salvage begins. Losing the wreck later does not delete evidence. **Acceptance:** towing cannot accelerate an object through the hub; the recorder appears once in the evidence case; core exterior and expansion interior versions yield the same flags.

### OC07 — No Cargo Is Just Cargo

**Location:** isolated settlement in `FRONTIER_RELAY`. **Actors:** Mara, Tamsin, Voss. **Situation:** a route closure has stranded medical supplies and power cells. **Reward:** 400 units, Broker +60, community standing, OC08.

VOSS: “Those routes are closed for your protection. Unauthorized travel makes rescue very expensive.”

MARA: “So does leaving a clinic without power.”

PLAYER CHOICE A: “I'll carry the supplies.”

TAMSIN: “Then I'll carry what doesn't fit. We leave together.”

PLAYER CHOICE B: “Can we reopen the route first?”

KEI: “We can clear the interference beacon. The supplies still need a pilot. Choose which job you want.”

Two routes: carry 2 t of protected supplies alongside a convoy, or disable the false hazard beacon while Tamsin delivers. Both have navigation challenges and optional combat; neither requires buying a larger ship.

**At delivery**

MARA: “The lights are on. I know that sounds small from orbit.”

PLAYER: “It doesn't.”

MARA: “Good. Come down sometime. We make terrible tea and we're very proud of it.”

**Evidence:** shipping combine hazard buoys repeat a forged signature. **Recovery:** destroyed ordinary cargo is replaced at a designated depot; essential supplies remain recoverable. **Acceptance:** convoy actors have destinations and completion cannot depend on following an NPC forever.

### OC08 — Where the Giants Turn

**Location:** a protected migration lane in `QUIET_BLUE_STAR`. **Actors:** Oru, Kei, space-whale family. **Reward:** 300 units, Pathfinder +80, migration chart, OC09. **Clock:** none.

ORU: “Keep your engines quiet when you reach the markers. You're visiting, not herding.”

PLAYER: “Will they notice us?”

ORU: “Almost certainly. The better question is whether they'll mind.”

**Play:** scan three migration markers; coast beside the family without entering its personal-space volume; align the receiver with three low-frequency pulses. Markers remain selectable from the contact list for players who have difficulty finding them visually.

KEI: “Ryn used to say they navigated by remembering everyone they'd travelled with.”

ORU: “She was being poetic. Then she went and found evidence.”

**Discovery**

SYSTEM: “Unlisted beacon resolved: Lantern Shore.”

KEI: “Ryn, if you can hear this, don't move. For once in your life, please don't move.”

RYN, faint: “That depends. Are you still angry?”

**Recovery:** startling a whale resets the observation window, not the mission. It returns on a bounded safe path. **Acceptance:** no infinite waiting; no shooting requirement; migration chart and destination are saved before the next chapter unlocks.

## 25. Complete campaign scripts — Act III: The open channel

### OC09 — The Person on the Other End

**Location:** Lantern Shore research shelter. **Actors:** Ryn, Kei, Pip. **Actions:** land, repair a shelter power junction, reach Ryn, choose how to help. **Reward:** 350 units, rescue completion, Ryn as recurring contact, OC10.

RYN: “I had a speech prepared. It was very convincing when I was alone.”

KEI: “Try starting with 'I'm alive.'”

RYN: “I'm alive.”

KEI: “Good. You can do the clever part later.”

PLAYER CHOICE A: “Let's get you home.”

RYN: “Yes. But I won't leave the route key for them. Help me disconnect the relay and I'll come.”

PLAYER CHOICE B: “Tell me what you found.”

RYN: “The relays aren't calling the creatures. They're listening to them. Pale Meridian has been amplifying the signal until it hurts.”

**Repair play:** route power through two stable nodes, disable the amplifier, collect Ryn's research case. Pip highlights a failed connection after one unsuccessful attempt. There is no dexterity-only story gate.

**After boarding**

KEI: “You could have told me.”

RYN: “I thought if you knew, they'd come for you.”

KEI: “They came anyway. Next time, let me decide what I can carry.”

RYN: “All right. Next time.”

**Recovery:** Ryn waits in shelter if the player leaves; the chapter is not invalidated. **Acceptance:** boarding moves the character's persistent state once; the same Ryn cannot remain on the surface and appear at the hub simultaneously.

### OC10 — A Map That Belongs to Everyone

**Location:** Lave hub planning room presented through portraits and a map. **Actors:** Kei, Ryn, Iona, Tamsin, Nadi, Venn. **Objective:** complete any two of three preparation jobs. **Reward:** 450 units, a free support fitting, OC11.

RYN: “We can reopen the relays without the amplifier. But someone has to keep the company from taking control again.”

IONA: “Evidence can stop a contract. It cannot stop a ship already on its way.”

TAMSIN: “Then we need both: proof and pilots.”

Three preparations:

1. **Proof:** compare logs at two public receivers; improves the lawful resolution.
2. **People:** escort volunteer engineers to the relay; improves safe repair and community governance.
3. **Path:** survey an alternate approach lane; improves stealth and noncombat access.

Past side missions supply one preparation credit where appropriate, without skipping the planning scene. The player chooses two; all three are optional for a “Prepared for Everyone” medal.

NADI: “I can open the maintenance lock. I can't promise it will still count as a lock afterwards.”

VENN: “Bring back the tools you borrow.”

NADI: “You say that like it's personal.”

VENN: “It has been personal since the ladder.”

**Recovery:** each preparation is independently checkpointed. **Acceptance:** no preparation depends on an earlier optional moral choice; a solo explorer with a basic weapon can reach the finale.

### OC11 — The Open Channel

**Location:** the primary Meridian relay. **Actors:** full cast, Voss, autonomous defence drones. **Objective:** disable amplification, broadcast evidence, reconnect the route. **Reward:** 1,000 units, unique hull livery, governance project, OC12.

VOSS: “You mistake control for cruelty. We made these routes dependable.”

RYN: “You made them yours. That's not the same thing.”

VOSS: “And when no one is responsible?”

PLAYER CHOICE A: “The people who use them will be.”

PLAYER CHOICE B: “The Guild will publish every change.”

PLAYER CHOICE C: “Law will hold the records. In public.”

The governance choice is recorded now, but the player must still complete the operation. It alters supporting radio and the epilogue, not the required flight controls.

**Three coordinated tasks:**

- Approach through the surveyed lane or a defended front corridor.
- Disable three amplifier nodes using weapons, repair/hack interaction, or ally assistance earned in OC10.
- Hold position in a safe broadcast cone for 12 cumulative active seconds; progress survives brief evasion.

At most six combat escorts are live. The capital structure has a readable weak point and telegraphed sweeps. No instant-kill beam fires before its warning. Noncombat mode replaces destructive node attacks with three closer tool interactions while allies draw fire.

IONA, if proof prepared: “Pale Meridian vessels, your operating authority is suspended. Your crews are not targets. Stand down.”

TAMSIN, if engineers delivered: “Repair team is in. Keep the lane clear; they're working.”

KEI, player near defeat: “Break away. The progress is saved. You don't have to finish this in one breath.”

**At broadcast completion**

RYN: “Listen.”

The music drops. A distant migration call answers. Route lights appear one at a time.

MARA: “Lantern settlements receiving. We can see the traffic again.”

VENN: “Lave Hub to everyone out there: the channel is open.”

**Failure-forward:** retreat preserves repaired nodes after a safe regroup checkpoint. A retry restores bounded enemy support, not an ever-growing fleet. **Acceptance:** every completion path commits the same route reopening; the final reward cannot repeat on reload; no ending requires killing Voss.

### OC12 — A Berth Kept Warm

**Location:** Lave System Hub. **Objective:** return, speak to the crew, select the first post-story project. **Reward:** commander memorial plate, end credits, unlocked free sandbox. **Clock:** none.

VENN: “Berth six is yours. Try not to look surprised; you've been paying for it in gossip.”

KEI: “I spent so long asking you to bring someone home that I forgot you might need a home too.”

RYN: “There's a survey seat open. No disappearing this time. We file a route, we take supplies, and we answer the radio.”

PLAYER CHOICE A: “That sounds almost sensible.”

RYN: “We'll leave room for improvement.”

PLAYER CHOICE B: “Where first?”

RYN: “Somewhere neither of us has a story about.”

**Governance epilogue cards**

- **Community:** “The settlements appoint rotating relay keepers. Routes are slower to change and harder to sell. You receive the first invitation to help build a public waystation.”
- **Guild:** “The Guild publishes the relay maps and their uncertainties. New survey teams leave Lave every week. Ryn insists that every route includes a return plan.”
- **Law:** “Iona places the relay records in a public archive. Patrols protect the lanes under published rules. The first audit is uncomfortable, which she calls a promising start.”

KEI, final line: “Whenever you need us, this channel is still here.”

Credits can be skipped without skipping rewards. Afterwards choose a project: establish a research beacon, organise a relief route, or refurbish a salvage tug. The galaxy remains available; the story does not reset.

## 26. Repeatable mission library and supporting stories

All templates have legal prerequisites, route feasibility, stable targets, capped rewards and a recovery state. Choose destination and encounter seed at acceptance, not whenever the player opens the board.

| Template ID | Objective / twist | Completion proof | Reward and recovery |
|---|---|---|---|
| JOB_DELIVERY | Deliver a sealed package; optional careful-handling bonus | Correct hub transaction with reserved package ID | Units + Broker; recover package at depot after normal defeat |
| JOB_SURVEY | Scan three related organisms or signals | Unique discoveries tied to site and mission | Units + Pathfinder; partial observations persist |
| JOB_RESCUE | Locate a pilot, hail, make safe transfer | Rescue actor transferred to protected passenger manifest | Units + Guardian; target reconstructed if unloaded |
| JOB_HUNT | Identify and stop a named hostile | Disabled/captured/destroyed target according to contract | Bounty + Guardian; wrong target does not count |
| JOB_COVERT | Deliver prohibited data/cargo through a watched route | Cargo and destination validated; local legal exposure | High units; evidence of risk shown before acceptance |
| JOB_ESCORT | Protect a convoy across three route gates | Leader or sufficient lifeboats delivered | Pay scales with optional assets saved; no endless following |
| JOB_REPAIR | Restore a beacon by matching power symbols | Nodes repaired in any valid order | Units + service access; repair supplies loaned if essential |
| JOB_RECOVERY | Extract one marked salvage item | Stable item ID checked at return | Units/material choice; optional wreck collapse never destroys committed main proof |
| JOB_CONSERVE | Observe or free wildlife without harming it | Behaviour record or net release event | Research + cosmetics; fleeing creatures return on a bounded cycle |
| JOB_ARCHEOLOGY | Assemble clues from three sites | Correct evidence set, not a random quiz answer | Research/artefact; duplicates become tradeable fragments |

### Side story S01 — The Borrowed Ladder

**Actors:** Venn and Nadi. **Loop:** recover Venn's old inspection ladder from a floating salvage cage; choose to return it or buy him a replacement and let Nadi keep the original.

VENN: “She borrowed it for an afternoon.”

NADI: “Technically, I haven't returned to that afternoon yet.”

On return, Venn paints the ladder with the player's ship badge. If replaced, he adds an anonymous donation note to Nadi's tug fund. Reward: 120 units either way and a tiny visible hangar prop. No combat required; teaches salvage ownership.

### Side story S02 — The Last Crate of Summer

Mara asks the player to carry a crop sample to Oru. A buyer offers more money for it. Choosing the sale gives the advertised cash but closes this particular research bonus; delivering the sample unlocks a new plant display and equal long-term value through a later job.

MARA: “It isn't rare because it's expensive. It's rare because it grew after everyone told us it wouldn't.”

Reward baseline: 200 units; optional research relationship. The decision is explicit and reversible until transfer. A cargo menu action alone cannot silently make the moral choice.

### Side story S03 — A Very Small Emergency

Pip repeatedly reports a critical malfunction: a loose decorative panel on a research ship. Investigation reveals that it conceals a damaged emergency beacon. Repair it, then choose whether Pip's original report was technically correct.

PIP: “Prevented emergency. Current casualties: zero. I consider that a satisfactory emergency.”

Reward: 150 units and a Pip portrait expression. Teaches repair and rewards attention to apparently mundane signals.

### Side story S04 — Terms of Passage

Sable offers safe passage through one pirate-held lane in exchange for recovering a stranded member of her crew. The rescued pilot turns out to have left voluntarily. Ask the pilot's wishes: return them, escort them to a neutral station, or negotiate a debt settlement.

PILOT: “I owe her money. I don't owe her the rest of my life.”

SABLE, if negotiated: “Very well. A debt with an ending is easier to collect.”

All routes award completion. Reputation and future chatter differ. No route removes essential access to the main campaign. Reward: 250 units or an explicitly chosen safe-passage token.

### Side story S05 — Proof of Life

Oru asks for a photograph of a grazer's behaviour, not another specimen. Find tracks, wait for its feeding action, take the picture from a comfortable distance and return. A photo checker uses observable state, distance and line of sight rather than image recognition.

ORU: “A good picture tells me what the animal was doing before it noticed you.”

Reward: 180 units, Codex illustration and a habitat clue. Accessibility alternative: record a five-second survey clip using the same state checks.

### Side story S06 — The Empty Seat

Tamsin asks the player to deliver a retired crewmember's tools to a memorial buoy. No ambush occurs. A distant convoy passes and acknowledges the signal. The mission exists to give the galaxy quiet emotional space.

TAMSIN: “He taught three generations of engineers. He would hate that we're calling it three generations. He never thought he was that old.”

Reward: a convoy patch and modest fuel reimbursement. Completion needs only the buoy interaction; the player can stay as long as they wish.

### Side story S07 — The Visitor at Berth Nine

A peaceful unusual vessel arrives with a failing translator. Match three signal patterns using icons and rhythm, then trade a repair cell for a unique star chart. No random language guessing; repeated requests provide another cue.

TRANSLATOR, first success: “We have travelled far to misunderstand you this badly.”

VENN: “You're in the right port. We practise every day.”

Reward: a chart to an optional rare system feature and a visitor portrait. Serves as the opening for a future organic-ship arc.

### Side story S08 — A Fair Price

Iona investigates a relief shipment sold above its posted emergency ceiling. The player gathers three public price records, then chooses to expose an exploitative merchant or demonstrate a real supply shortage. The correct factual outcome depends on the seeded records, not faction loyalty.

IONA: “I don't need someone to agree with me. I need someone to check.”

Reward: 220 units and local trust. Teaches historical market information, evidence and the separation between economy and morality.

### Barks and failure copy

Use short, concrete lines. Examples ready for localisation:

- Dock denied: “You're too far from the hub. Move within 2.5 km and hail again.”
- Manual approach too fast: “Entrance aligned. Slow below 200 before crossing.”
- Target vanished: “Contact lost. Your mission marker is still available in the log.”
- No cargo room: “This job needs 2 t. Your hold has 1 t free.”
- Fuel reserve: “Boost stopped to protect your return fuel.”
- Scan complete: “Observation saved. New behaviour added to the Codex.”
- Previously scanned: “Already recorded. Look for a new behaviour or another specimen.”
- Guided docking failure: “Approach obstructed. Holding clear of the hull. You may cancel.”
- Police fine unaffordable: “You cannot cover the fine. Station custody is still available.”
- Save failed: “Save not replaced. Your previous commander is still available.” Only display this if the implementation actually preserved it.
- Mission expired: “The urgent window has closed. Your earlier discoveries are safe.”
- Stuck recovery: “Return to the ship? You will leave uncollected salvage here.”

## 27. Data-driven mission scripts

Use a small validated content format compiled into compact C tables or a bounded binary bundle. JSON is an authoring format; do not introduce an unrestricted runtime scripting language on the PSP merely to show dialogue.

### Proposed schema example — not implemented in 1.3.8

```json
{
  "schema": 1,
  "id": "OC03",
  "title": "The Quiet Between Pings",
  "location_role": "NEARBY_AGRI",
  "prerequisites": ["OC02.completed"],
  "clock": "none",
  "checkpoint": "each_stage",
  "resources": {"protected_evidence_slots": 1, "required_free_cargo_t": 0},
  "stages": [
    {"id": "reach_site", "on": "player.landed", "where": {"site": "stillwater"},
     "say": "OC03.oru.look", "next": "repair_receiver"},
    {"id": "repair_receiver", "on": "device.repaired", "where": {"entity": "receiver"},
     "next": "observe_grazer"},
    {"id": "observe_grazer", "on": "observation.completed",
     "where": {"entity": "grazer", "behaviour": "feeding"},
     "say": "OC03.oru.reply", "next": "submit"},
    {"id": "submit", "on": "evidence.submitted", "where": {"record": "living_signal"},
     "next": "complete"}
  ],
  "reward": {"transaction": "OC03.final", "units_tenths": 2000,
             "pathfinder": 60, "unlock": "survey_array_blueprint"},
  "recovery": {"missing_entity": "restore_reserved_entity",
               "defeat": "last_safe_checkpoint"}
}
```

**Event order:** collect input → update simulation → emit typed events → resolve mission transitions in a bounded queue → commit one transaction → refresh derived UI. Dialogue cannot directly mutate money or teleport an actor; it requests a validated action.

**Validation before build:** all IDs unique; all next states exist; dialogue keys resolve; every nonterminal state has a completion or recovery route; route is feasible; essential cargo fits; reward is within a declared cap; no dependency cycle without an explicit repeatable marker; supported schema version; text fits/reflows in the smallest UI panel.

**Reward journal:** store `mission_instance_id + reward_transaction_id` as the durable receipt. Replaying an event after loading, duplicate controller input or an interrupted cinematic must be a no-op after receipt exists.

**Five-job migration:** retain the old `Job` importer, convert each active job to the new runtime structure, generate stable mission instance IDs and map its legacy stage explicitly. Do not restart timers or remove carried mission cargo when importing.

## 28. Implementation architecture: evolve the running game

The following architecture is proposed. It is not a description of files that already exist. Preserve a working PSP build after each small change. Do not replace the renderer, save format, flight model and mission system together.

| Current area | Responsibility to preserve | Proposed extraction when needed |
|---|---|---|
| `game.c`, `game.h` | Flight, simulation, galaxy, economy | `world`, `flight`, `economy`, stable world identifiers |
| `main.c`, `flight-extras.h` | Input, view, frame orchestration | `input_actions`, `flight_view`, common target projection |
| `docking.h` | Docking state and presentation | Shared station aperture/collision definition |
| `ui-modern.h`, `font8.h` | Readable menus and text | Common focus, scrolling, wrapping and action hints |
| `voyage.h`, `story.h` | Jobs, tutorial, narrative | Compiled mission data and bounded event interpreter |
| `sectors.h`, `planet.h` | System population and local surface | Deterministic system descriptions and region instances |
| `sprites.h`, `pixel-art.h` | Sprite and portrait rendering | Generated asset tables with format metadata |
| `audio.h` | Sound queue and synthesis | Existing worker, explicit priorities and bounded queue |

Extract a module only when a feature or defect makes the boundary useful. Large headers currently contain implementation; moving them to `.c` files is a maintenance task, not a prerequisite for fun.

**Stable identity.** Use `SystemId`, `BodyId`, `StationId`, `ActorId` and `MissionInstanceId`. An array slot is never a persistent identity. A target stores an ID and generation counter; deletion invalidates it safely. Names are presentation. The target computer, local map, mission log and docking welcome all resolve the same object record.

**Coordinates.** Keep authoritative local positions in a documented unit. Choose metres internally after auditing existing units; migrate constants deliberately. Use a floating origin for rendering if range causes visible jitter. Subtract the origin before converting positions to renderer floats. Changing the render origin must not change generation, collisions, mission distances or saved locations. Galaxy coordinates and local-system coordinates are separate types.

**State ownership.** Use one top-level mode: `FLIGHT`, `GUIDED_DOCK`, `DOCK_CINEMATIC`, `DOCKED`, `PLANET_PROMPT`, `SURFACE_TRANSITION`, `SURFACE`, `WARP`, `DESTROYED`. Menus and conversation are overlays with a declared clock policy. Input is converted to actions once; simulation consumes actions once. Entering a mode clears inappropriate held/edge actions. Closing dialogue cannot also fire a weapon or accept a second choice.

**Simulation time.** Use a fixed simulation step, initially 1/60 second, with an accumulator and a bounded catch-up count. Measure whether the PSP can maintain this; 30 Hz simulation with interpolated rendering is an acceptable tested fallback. Never run an unbounded catch-up loop after suspend. Clamp an exceptional wall-time gap, record it, and resume from a safe state. Menu, police conversation, loading, welcome and planet-choice time freezes follow the rules in section 20.

**Docking geometry.** Define the station's local-space solid hull, aperture rectangle and entry plane once. Transform both the ship position and its swept movement segment into station coordinates using the station's current rotation. Test a swept ship volume against the opening's clearance, then the solid hull. Crossing from outside to inside through a valid opening starts docking exactly once. A hull hit cannot trigger docking. Collision must be continuous at boost speeds; testing only the final position allows tunnelling.

**Guided docking.** Request clearance; reserve the opening; choose an exterior approach waypoint; align to the rotating opening; reduce speed; cross the same validated entry plane; play the cinematic. A route from behind the station must go around the hull. If obstructed, hold safely and explain the delay. Cancel returns controllable flight outside collision geometry. A timeout must never leave the ship locked forever.

**Orientation.** A normalized quaternion or orthonormal basis supports unrestricted pitch, yaw and roll. Normalize after integration, not only on loading. The radar uses the inverse of this same orientation. The camera, weapons, compass, docking checks and velocity must agree about forward/up/right.

**Population.** Maintain a lightweight system population registry and a bounded active actor pool. Distant ships advance along routes at low frequency; nearby ships get steering and collision. Promotion from abstract to active preserves role, route, cargo and damage. Spawn outside the player's view where possible, with a minimum safety distance; never spawn an attacker inside firing range merely because the player looked away. Mission actors get priority over decorative traffic.

## 29. Sprite art bible and production inventory

Preserve the wireframe identity. Sprites supply readable interfaces, characters, surface life and atmospheric effects; they do not turn every spacecraft into a flat picture. Recognizable large ships and stations remain geometry. Use pixel art with deliberate silhouettes, limited palettes, nearest-neighbour sampling and restrained animation.

**Visual language:** deep navy backgrounds, warm amber interaction, cyan navigation, green confirmed safe action, red immediate danger. Faction identity also uses a symbol and name, so colour alone is never essential. Reserve bright white for focus, nearby highlights and warning emphasis. Avoid large opaque cockpit panels in flight.

| Asset family | Initial scope | Source frame size | Animation / use |
|---|---:|---|---|
| Named character portraits | 10 | 64×64 | Neutral portrait; optional 2-frame blink and 2 mouth frames |
| Faction insignia | 4 plus neutral | 16×16, 32×32 | Static, also recognizable in monochrome |
| Commodity icons | 17 | 16×16 | Silhouette first; text supplies exact identity |
| Equipment and crafting icons | 24 | 16×16 | Shared frames/palette, no tiny lettering |
| Input prompts | Actual supported actions | 12–20 px display height | PSP face/shoulder buttons and nub; test legibility |
| Planet/map category icons | 8 | 16×16 and 32×32 | Rock, ocean, gas, sun, station, anomaly, belt, mission |
| Surface vegetation | 12 silhouettes | 32×48 or 48×64 | 2–3 subtle wind frames only for nearby objects |
| Surface fauna, first slice | 3 species | 32×32 or 48×32 | Idle 2; walk 4; alert 2; initially 4 directions |
| Surface fauna, expanded | 6 archetypes | Same families | Palette/part variants, not hundreds of loaded sheets |
| Pip | 1 | 24×24 | Idle 4, scan 4, react 2 |
| Particles | 8 shapes | 8×8 to 32×32 | Sparks, dust, smoke, scan ring, flare elements |
| Salvage and surface props | 16 | 32×32 to 64×64 | Consistent grounding pivot and optional shadow |
| Fishing/creature league icons | Expansion only | 24×24 | Build only when the underlying game loop is approved |

Sizes and counts are production targets, not promises that all assets are resident simultaneously. Atlas allocation includes padding, palettes and metadata. A 64×64 portrait with five frames does not cost the same as a single portrait.

**Animation rules.** Locomotion follows actual speed. Direction changes select a facing set; they do not restart walking every frame. Pausing freezes animation where simulation is frozen. Billboards use a ground pivot, separate collision radius and a small shadow. Objects behind terrain are occluded; plants do not slide to face the camera through the ground. Do not mirror characters with asymmetric story markings. Keep effects short and permit reduced flashing.

**Space whales:** use a few low-polygon/wireframe articulated body segments for close views and a silhouette sprite at distance. Tail phase derives from time plus a stable actor offset. This avoids a flat whale revealing itself when flown around. Asteroids are geometric at collision distance; distant belts may use sparse sprites with no false solid silhouettes.

**Portrait direction:** Kei has an alert, practical expression; Ryn looks tired but curious; Venn's immaculate collar contrasts with a repaired headset; Iona wears a plain service jacket; Nadi's patched suit carries tool labels; Oru has a weathered field coat; Tamsin looks calm under pressure; Sable smiles as if already negotiating; Voss is controlled rather than grotesquely villainous. Pip communicates with a small light array. Distinct silhouettes matter more than detail.

## 30. Asset sources and a reproducible sprite pipeline

The following sources were checked during research. They are candidates, not a statement that files have been downloaded, adapted or licensed into this build.

| Source | Useful material | Adaptation / provenance |
|---|---|---|
| [Kenney Input Prompts](https://kenney.nl/assets/input-prompts) | Controller symbols and generic input artwork | CC0 page; redraw/downsample carefully for native PSP size |
| [Kenney UI Pack](https://kenney.nl/assets/ui-pack) | Panel and interface building blocks | CC0 page; use selectively, preserving this game's retro identity |
| [Kenney Foliage Sprites](https://kenney.nl/assets/foliage-sprites) | Starting silhouettes for surface vegetation | CC0 page; palette and scale pass required |
| [Kenney's Space Shooter Redux upload](https://opengameart.org/node/21238) | Space icons and placeholder sprite vocabulary | Creator upload identifies CC0; primarily prototypes, not a substitute for 3D ship meshes |
| Original commissioned or team-made pixel art | Main cast, faction insignia, story objects, distinctive fauna | Preferred final identity; record author and reuse rights |

Kenney explains its asset licensing on its [support page](https://kenney.nl/support). Preserve a local copy of each asset page/license and record source, creator, download date, modifications and content hash. Software licensing and exported artwork licensing are different questions. Do not extract Elite or No Man's Sky artwork and assume it is freely redistributable.

**Fix the existing pipeline first.** `bake-sprites.py` and the cleanup helper contain personal absolute paths. The generated header describes 16-bit data but stores 32-bit `unsigned` RGBA values. The key-colour cleanup can erase legitimate magenta pixels. Replace these assumptions with explicit project-relative inputs, exact pixel format metadata and authored alpha.

Proposed layout:

```text
assets/source/portraits/kei.aseprite
assets/source/fauna/leafback.aseprite
assets/export/portraits/kei.png
assets/export/portraits/kei.json
assets/manifest.json
assets/licenses/<asset-id>/
tools/build_assets.py
src/generated/assets.h
src/generated/assets.c
```

An optional [Aseprite CLI](https://www.aseprite.org/docs/cli/) export, run from the project root with Aseprite available:

```sh
aseprite -b assets/source/portraits/kei.aseprite --sheet assets/export/portraits/kei.png --data assets/export/portraits/kei.json --format json-array
python tools/build_assets.py --manifest assets/manifest.json --output src/generated
```

The first command uses documented Aseprite export options; the second describes a **tool to implement**, not an existing command. PNG plus explicit JSON is also an acceptable authoring route; Aseprite is optional. The [sprite-sheet documentation](https://www.aseprite.org/docs/sprite-sheet/) explains the export model.

Example proposed manifest entry:

```json
{
  "schema": 1,
  "id": "fauna.leafback.walk",
  "image": "export/fauna/leafback.png",
  "metadata": "export/fauna/leafback.json",
  "format": "indexed8",
  "transparent_index": 0,
  "pivot": [24, 30],
  "frame_ms": 125,
  "directions": 4,
  "palette_group": "temperate_life",
  "collision_radius_world": 0.65,
  "license_id": "original-leafback-001"
}
```

**Converter requirements:** validate paths and licenses; load RGBA; preserve authored alpha; map pixels to a declared palette or quantize reproducibly; reserve transparent index zero; reject accidental opaque edges; pack frames with padding; emit fixed-width types, frame rectangles, pivots, durations and byte counts. Produce a contact sheet at 1× and 4× for review. Output order must be deterministic. Do not use the same string for an asset's identity and its generated array address.

For a CPU renderer, indexed sprites can be decoded directly through a small palette or stored as transparent row runs. A row-run record contains skip count, visible pixel count and palette indices; clipping must never read beyond the row. Profile it against existing `blit_spr_fit`; compression is useful only if decode cost and asset size improve the target scene. Keep the old blitter as a reference until output agrees.

For GU textures, convert to a documented supported format, pad dimensions to the required texture layout, align data and swizzle only when appropriate for that format. Palette upload and alpha blending must match the selected format. Avoid repeated conversion/upload in the frame loop. Reject an atlas exceeding the declared pool rather than silently allocating more memory.

**Asset acceptance:** native-size readability; no pink fringes; no invisible faces against dark space; correct pivots; animation duration stable at 30 and 60 fps; no texture bleeding; checked memory report; provenance recorded. A generated illustration is only source material until it passes this pixel-art and runtime process.

## 31. Rendering plan: attractive within PSP limits

Keep the current CPU renderer as the baseline. Improve clipping, deterministic draw ordering, sprite transparency and effects budgeting before a graphics rewrite. The existing draw list is bounded; overflow must select lower-priority decoration for omission and report a counter, never overwrite memory.

**Starfield:** derive stable directions from a seeded distribution on a sphere. Rotate by camera orientation. Very distant stars do not translate like nearby dust. Use sparse brightness tiers, mild colour differences and few larger highlights. Nebulae can be low-resolution background layers; they must not hide targets or consume the whole fill budget. System palette is stable on return.

**Speed:** separate real celestial stars from transient speed particles. Ordinary high speed gives sparse peripheral streaks; sustained double-tap boost gives longer, denser peripheral streaks and a stronger engine cue. Centre aim stays readable. Streak intensity uses actual velocity and boost state, never a random permanent effect.

**Suns and flares:** draw the sun disc and corona, then a restrained additive flare only if the sun is on screen and not occluded by a planet or station. Screen-space elements lie along the sun-to-screen-centre axis. Smooth visibility changes; cap brightness and element count. Reduced-effects mode disables flare streaks. No hardware shader assumption is needed.

**Planets:** share geometry between a small number of body resolutions. Use seeded palette bands, low-cost cloud overlays and a lit/unlit distinction. Clouds rotate slowly in the body's frame, not in the HUD. Gas bands and sun colour provide variety. Close approach ends in a deliberate transition; it does not promise continuous planet-scale terrain streaming.

**Wireframe explosions:** ship-shaped fragments inherit velocity, separate, rotate and fade. Use a bounded fragment pool with deterministic expiry. Preserve the last safe camera view, then show loss/recovery choices. Do not repeatedly retrigger explosion audio while the destroyed ship remains in collision.

**Optional GU migration gate.** Build a small comparison scene first: opaque geometry, one textured billboard, alpha sprite, depth occlusion, text and clipped near-plane geometry. Compare frame time, memory and visual output with the CPU version on emulator and real hardware. Only then move passes incrementally. Define framebuffer ownership and synchronization explicitly; CPU writes racing a GU pass can corrupt the frame.

The installed SDK has concrete examples under `work/toolchain/psp/sdk/samples/gu/`, including `blit`, `clut` and `blend`. Consult their actual code and retain notices when reusing it. [PSPSDK GU documentation](https://pspdev.github.io/pspsdk/group__GU.html) describes texture formats, sprites and state. A screen-space UI sprite and a depth-tested world billboard are different rendering cases. Use world triangles/quads for billboards that need reliable perspective and depth. Alpha objects require controlled ordering and depth-write policy; opaque geometry should establish depth first.

## 32. Performance and memory budgets

Target the original 32 MB PSP as the baseline. Sony's [original hardware specification](https://sonyinteractive.com/uploads/sites/5/2023/02/040512a.pdf) lists the 480×272 display, 32 MB main memory and CPU clock up to 333 MHz. Additional memory on later devices is optional headroom, not a dependency.

**Target:** responsive 60 fps flight in ordinary scenes; a stable, explicitly tested 30 fps fallback is preferable to unstable pacing. No milestone is called performant from an average alone. Capture frame-time percentiles, worst frame, actor count, draw count and allocation peaks. The existing short emulator run is encouraging, not proof for every new scene or physical hardware.

Initial working limits, to revise from profiling:

| Resource | Starting limit / policy |
|---|---|
| Active space actor slots | Preserve 48-slot ceiling initially; reserve mission/police capacity |
| Full steering/combat updates | Nearby priority set, approximately 12–16 actors; remaining actors staggered |
| Large freighters | 2 near-detail, further ships lower detail |
| Surface visible fauna | 8 active, 3–4 high-detail nearby |
| Surface vegetation | Bounded visible list, culled by region and distance |
| Explosion fragments | 24–48 shared, reduced under load |
| Cosmetic particles | 64 shared starting cap |
| Flare elements | Maximum 5 for the dominant visible sun |
| Runtime allocations | None in ordinary flight update/render after scene load |
| Generation | Bounded jobs during transition; visible progress if loading is noticeable |

These proposed actor budgets must fit existing pool semantics; do not simply increase `NPC_COUNT` to create a busy galaxy. Distant lightweight traffic creates scale much more cheaply than dozens of close combatants.

**Illustrative 8 MiB heap plan:** simulation 0.5; world/region 2; decoded assets 2; loading staging 1; audio 0.125; missions 0.25; save staging 0.125; render scratch 1; reserve 1. Total 8 MiB. This is an allocation ceiling sketch. Static arrays, code, stacks, SDK overhead and other allocations also occupy main RAM and must be measured with a linker map and runtime high-water marks. Never treat the entire 32 MB as free heap.

**Illustrative VRAM arithmetic:** at a 512-pixel stride and 272 lines, two 16-bit colour buffers plus one 16-bit depth buffer use 835,584 bytes, or 816 KiB. Of a 2 MiB pool this leaves 1,232 KiB before textures and other use. Two 32-bit colour buffers plus 16-bit depth use 1,360 KiB, leaving 688 KiB. These are alternative plans, not a claim about the current renderer's allocation.

A 256×256 RGBA4444 atlas costs 128 KiB. An 8-bit indexed atlas costs 64 KiB plus palette; a 4-bit atlas costs 32 KiB plus palette. Include padding and duplicate resident frames. Start with a conservative texture pool, upload by scene, and retain a margin for driver/state requirements. Measure gradients and transparency before selecting 16-bit output; dithering may help but can harm clean UI.

**Degradation order:** reduce decorative particles, vegetation distance, distant ship detail and cloud layers; then lower background detail. Never remove an objective actor, collision, warning text or readable target marker to save time. Use hysteresis so detail does not oscillate every frame. Log repeated pool exhaustion as a defect.

## 33. Saving, migration and interruption safety

Current save version 7 contains raw structures and appended data, with imports for earlier versions. Its replacement sequence removes the previous path before renaming the temporary file. Improve this before adding much more persistent content: a failed rename must not destroy the only good save.

**Proposed version 8:** an explicit header, schema version, payload length, checksum, content-generation version and tagged sections. Serialize fixed-width fields deliberately; never dump the new runtime `Game` structure. Include current location/mode, inventory, upgrades, per-system warrants, discoveries, mission states and reward receipts, station/base ownership and changed world objects. Regenerable world data remains a seed plus version and bounded deltas.

Write a temporary file, flush/close with checked results, validate it, and replace through a tested backup/recovery procedure supported by the PSP filesystem. Retain the last known-good backup. On startup choose the newest valid complete save, not simply the newest filename. Test the actual filesystem semantics; desktop rename behaviour is not sufficient evidence.

Import versions 1–7 through explicit conversion functions and golden sample files. Keep a copy of the original save. Verify money, cargo, fuel, upgrades, location, active jobs and warrants after migration. If a new feature cannot map an old state, give a documented safe fallback and compensation, rather than discard the whole commander.

Save after completed docking, completed warp, mission reward transaction and explicit docked save. Avoid saving an unsafe intermediate cinematic. Suspend pauses all game clocks and clears held input on resume. Revalidate audio and rendering resources. Interrupted docking loads at a safe exterior checkpoint or a committed docked state; interrupted warp loads at its recorded origin or committed destination, never a hybrid.

Discovery records use stable IDs and bounded storage. Aggregate counts alone cannot support an honest catalogue. Define a practical catalogue capacity and page/archive policy before endless collection expands saves without limit. Never silently forget a discovery while displaying its reward as unclaimed.

## 34. Quality plan: earn reliability instead of promising it

No design can guarantee a bug-free game. A release can require no known progression blockers, no reproducible save loss, no known crashes in the release scenarios, and clear reporting of remaining cosmetic issues. Distinguish implemented, tested in host logic, tested in PPSSPP and verified on physical PSP.

| Scenario | Required result |
|---|---|
| Manual entry, aligned, slow, every station shape | Cross opening once, no hull damage, cinematic then exact station welcome |
| Entry edge / incorrect roll / excessive speed | Consistent collision or clear refusal; no clipping or repeated damage after transition |
| Boost across hull or small asteroid | Swept collision catches impact; no tunnelling |
| Guided docking from front, rear, side, near obstruction | Safe route, alignment, cancellation and timeout recovery |
| Station rotates while docking | Opening visuals and collision transform agree throughout |
| Reject planet approach at multiple headings | Nearby safe offset, face away, input restored; no repeated prompt until rearmed |
| Full pitch/yaw/roll circuits | No angle lock, radar inversion, camera flip or growing orientation drift |
| Startup centred nub; reconnect; suspend | No constant unwanted turn; dead zone and calibration remain usable |
| Double-tap R timing, hold, release, menus | Boost state predictable; no accidental sticky boost |
| All button chords and text dialogue | One intended action, no leaked fire/dock/confirm input |
| Target disappears, pool slot reused | Target clears or resolves by stable ID; never switches silently to unrelated ship |
| Local map / HUD / mission / welcome names | Same object displays the same canonical name |
| Lave ordinary exploration | Civilian life and background incidents; no unsolicited pirate attack on player |
| Crime then warp then return | Local warrant persists only where committed; no cross-system police pursuit |
| Police encounter during job | Simulation/mission clock policy honoured; pay/refuse/jail all resolve safely |
| Filled hold and essential mission reward | Reserve, transfer or recovery option; no unwinnable inventory state |
| Duplicate completion event / reload reward | Exactly one reward and reputation change |
| Every story choice and noncombat route | Reachable next state and ending; no missing speaker or dead-end stage |
| Flee, destroy, ignore or unload mission actor | Documented recovery path; log gives actionable next step |
| Save full disk, interrupted write, bad checksum | Previous good commander recoverable; understandable error |
| Import old saves | Preserved essentials, stable identifiers, no duplicated rewards |
| Suspend in flight, surface, dialogue and transition | Safe resume with no enormous time step or held-button action |
| Worst crowded fight and worst surface vista | Pools bounded, playable frame pacing, readable targets |
| Transparent sprites over black/bright backgrounds | No key-colour loss, fringe, bleed, out-of-bounds clipping |
| Low funds, low fuel, damaged ship far from station | Emergency route available; setback meaningful but no permanent softlock |
| HUD hidden; urgent danger arises | View remains clean; agreed critical cue policy works; restore action accessible |

**Test layers:** pure host tests for deterministic generation, mission transitions, transactions and collision math; scripted simulator scenarios for known bugs; emulator playthroughs for controls/presentation; physical PSP checks for memory, input, sound, suspend and frame pacing. Tests should assert outcomes, not duplicate implementation formulas. Record build hash and seed with every report.

Use at least three reference situations: peaceful Lave onboarding, a crowded dangerous-system convoy fight, and a busy surface region with fauna and effects. Add a long exploration/warp/dock session to find leaks and accumulating state errors. Choose soak duration proportionate to the release; do not claim a short smoke test proves an hour-long session.

## 35. Development tools and reproducible content

The debug menu should expose add units, restore fuel/hull, clear local warrant, spawn each faction, select danger level, reveal target geometry, trigger a mission state, teleport to a safe test position, set seed and display performance counters. Debug actions label the save as modified; they must not corrupt ordinary progression. Never allow teleport inside a station solid or below terrain.

Create a reproducible bug report bundle: build version, seed, system/body IDs, top-level mode, mission stages, recent bounded event log, actor/pool counts and frame-time summary. Exclude unrelated personal files. A replay records actions plus simulation ticks if deterministic reproduction is feasible; otherwise preserve a small failing checkpoint.

Content build checks should validate asset bounds/palette sizes, missing text keys, English punctuation, dialogue wrapping, unreachable mission states, impossible routes, economy outliers and generation invariants. A station cannot spawn inside a planet. Essential mission destinations must be reachable with the equipment available at that story stage. A safe system cannot accidentally inherit hostile player-targeting policy from its visual seed.

Keep generation test seeds for a binary star, very large station, dense belt, sparse economy, high danger, ocean surface and longest permitted mission route. Random fuzz tests supplement these fixed cases. Record the seed on failure so the result can be reproduced.

## 36. Delivery roadmap and implementation tickets

No calendar estimate is reliable until the first slice is measured on hardware. Build in bounded releases. Expansion features are not permission to delay the stable core indefinitely.

| Milestone | Deliverable | Exit gate |
|---|---|---|
| P0 — Trust the ship | Docking/collision/input/planet turnback fixes; safe save replacement; canonical targets and names | Relevant regression cases pass; current game remains playable |
| P1 — Understand the journey | Unified controls/help, five-door UI, local system information, mission tracking, clean HUD | New player can launch, target, travel, dock and trade without explanation |
| P2 — One excellent evening | Complete OC01–OC04 slice, 3 side jobs, meaningful trade/scan/rescue rewards, polished Lave | 30–60 minute playtest meets clarity and fun goals; no progress blockers |
| P3 — The Open Channel | All 12 chapters, branches, 8 side stories, save-safe mission runtime | Every route completed; rewards idempotent; old saves import |
| P4 — Living systems | Stable traffic routes/LOD, freighters, varied stations, bounded battles, fauna, improved skies | Busy scenarios fit measured budgets; Lave stays welcoming |
| P5 — A place to land | One polished surface biome, scan/salvage/crafting loop, 3 fauna, 1 small base | Planet round trip and save/load reliable; 15-minute surface loop is fun |
| P6 — Wider discovery | More biomes, archaeology, fleet assignments, broader technology, optional station role | New content uses proven systems and does not break old saves |
| Expansion A | Rover, companion, fishing, short derelict interiors, creature league | Each has its own enjoyable vertical slice before expansion |
| Expansion B | Underwater pocket regions, extended station/freighter interiors, larger ownership loops | Remaining hardware budget and playtests justify inclusion |

A milestone is not complete because the menu contains its name. A button leading to a placeholder is either clearly labelled unavailable or omitted from the released interface.

**First ready-to-implement tickets:**

1. **Save replacement safety:** reproduce rename/write failures; implement checked backup recovery; test old save import. No renderer changes.
2. **Shared docking aperture:** instrument local entry plane and hull volumes; use one geometry record for manual and guided entry; test three headings and rotating alignment.
3. **Planet turnback:** store rejected body ID, move to a safe nearby radius, reverse heading and rearm only after leaving/re-entering the approach condition. Test immediate turn-back.
4. **Input action table:** generate help labels from mappings; separate tap/hold/chord actions; clear edges after modal UI. Preserve a legacy preset.
5. **Stable target IDs:** canonical object lookup in HUD/map/log; verify destruction and pool reuse. Do not migrate unrelated systems simultaneously.
6. **Portable asset converter:** explicit alpha and pixel formats, native-size review sheet, memory report; migrate one icon family before portraits.
7. **Mission transaction runtime:** implement OC01 and one repeatable courier using data; test duplicate events/rewards, full hold and saving mid-stage.
8. **Complete OC03 scan slice:** target, scan feedback, signal discovery, dialogue and reward with a recoverable state machine.
9. **Living Lave routes:** trader and explorer circuits, police patrol and non-player incident; prove safe-player policy remains intact.
10. **Performance comparison scene:** measure baseline and optional GU pass; retain the simpler approach unless benefits are demonstrated.

Every ticket states changed files, acceptance outcomes, testing performed and remaining limitations. Produce an EBOOT only from a known passing build and identify it with a version/hash. Do not overwrite the user's only working package during an experiment.

## 37. Traceability to the complete conversation

This table maps the user's requests to the intended design. “Existing” means code exists in the audited project; it does not certify final quality or every edge case. Sections 2 and 34 define the limits of that evidence.

| Request | Design location / completion meaning |
|---|---|
| Elite-A basis with ArcElite spirit | §§1–3: preserve Elite flight/trade identity; explicitly distinguish this native adaptation from an exact ArcElite source port |
| Stop constant initial turning | §§6, 34, 36: calibration/dead zone, modal input clearing, startup regression |
| Friendly retro UI and useful controls screen | §§5–6, 18: shared action labels, readable focus, consistent back/confirm |
| Pirates, Law, Traders, Explorers with colours and behaviours | §§9–10: colour plus symbols, faction screen, role-based routes and decisions |
| Multiple planets, gas giants and suns | §§13, 31: stable named body records and differentiated appearance |
| System danger determines battles and pirate aggression | §§9–10: visual danger, encounter policy, Lave safety exception |
| Top-screen attack warning | §§6, 10: short clear threat banner and direction cue |
| Easy intersystem warp and effect | §7: reachable route selection, fuel clarity, interrupt-safe transition |
| Physical collision and obvious station entrances | §§8, 28, 34: shared rotating aperture, luminous opening, swept collision |
| Better starfield and extreme boost streaks | §§7, 31: stable stars, separate velocity streaks, unobstructed centre |
| Double-tap and hold R for faster boost | §§6–7, 34: explicit gesture state machine, tested cancellation |
| Look-at planet labels matching map | §§6, 13, 28: canonical target record and stable name |
| Say “System: Lave” | §§5–6: explicit system context separate from body/station names |
| NPCs everywhere, groups and chases | §§9, 28, 32: routes across the system, abstract distant population, active nearby groups |
| Stations on local map, select/aim targets | §§6–8: target list, local map and assisted alignment share IDs |
| Circle approach planets; reject and face away nearby | §§7, 13, 28, 34: proximity context action, safe turnback and rearm |
| Explore planets eventually | §§13–15, 36: bounded landable regions with real activities, not an empty surface button |
| Modern inventory; sales visible when docked | §11: own cargo always available, confirmed local stock at station |
| Lens flares | §31: occluded, bounded, reduced-effects option |
| Per-system wanted stars and police demand | §10: local warrants, readable choices, frozen encounter clock and station penalty |
| Select debug menu with add units | §35: practical cheats and test controls |
| Radar includes ships/bodies/stations | §§6, 28, 34: consistent transforms and type filters; no stale array targets |
| Easy roll | §6: existing shoulder modifier mapping plus clear help |
| Wireframe destruction when hitting hull | §§10, 31: bounded fragments and recovery choices |
| Slow station rotation, watched automatic docking | §§8, 28: real exterior approach and shared manual entry |
| Entry cinematic then named welcome | §8: explicit ordered presentation before services |
| Economy details, richer stock and more work | §§11, 20: economy drives availability without hiding essential survival |
| Peaceful Lave, visible police taking down pirates | §9: background life without unsolicited player attacks |
| Different skies, planets, clouds and sun colours | §§13, 31: deterministic palette/shape/cloud variety |
| Giant freighters, diverse ships and huge station variants | §§9, 16, 32: geometry and LOD, safe approach scaling, bounded battles |
| More high-economy outfitting technology | §11: useful tradeoffs, clear requirements, no mandatory grind wall |
| Mission board and mission log | §§20–27: five active jobs, objective tracking, full original story |
| Wanted stars and visual danger | §§6, 10: five-star warrant display and separate danger indicator |
| Full unrestricted flight and clean view / hide HUD | §§6, 18, 28: robust orientation, minimal HUD preset and restore shortcut |
| Space whales and asteroid fields | §§9, 13, 31: migration encounters, geometric nearby hazards |
| No Man's Sky breadth adapted for PSP | §§3, 13–17, 36: scoped core plus playable expansion slices |
| Complete scripts and an enjoyable repeating loop | §§4, 20–27: trade/explore/rescue/salvage rewards and authored dialogue |
| “Bug free” experience | §§33–35: concrete release gates, honest remaining issues, recovery paths |
| Finished EBOOT | §36: future implementation/build deliverable; this document itself does not rebuild the game |

## 38. Research ledger, credits and boundaries

Research checked 21 September 2026. Links below support reference facts and implementation options; gameplay rules and numeric budgets in this document are original proposals. New updates after the research date are outside this revision. No claim of having implemented every No Man's Sky feature is intended.

**Elite references:** [Mark Moxon's Elite-A site](https://elite.bbcelite.com/elite-a/) and [annotated Elite-A repository](https://github.com/markmoxon/elite-a-source-code-bbc-micro). The local upstream README identifies original Elite, Angus Duggan's enhancements and Mark Moxon's annotations and does not provide a blanket permissive license. Source availability is not the same as permission to redistribute all code or assets. Maintain a file-level provenance inventory before a public release. The original authors' work should be credited accurately; this design does not imply their endorsement.

**No Man's Sky references:** [official release log](https://www.nomanssky.com/release-log/), [Cosmos](https://www.nomanssky.com/cosmos-update/), [Voyagers](https://www.nomanssky.com/voyagers-update/), [Remnant](https://www.nomanssky.com/remnant-update/), [Xeno Arena](https://www.nomanssky.com/xeno-arena-update/) and [Swarm](https://www.nomanssky.com/swarm-update/). These informed the translation table and bounded expansion ideas. Their exact technology, network scale, artwork, characters and terminology are not being reproduced. This game uses an original setting and story.

**PSP implementation references:** [Sony hardware specification](https://sonyinteractive.com/uploads/sites/5/2023/02/040512a.pdf), [PSPSDK GU API](https://pspdev.github.io/pspsdk/group__GU.html), and the installed SDK samples noted in section 31. API examples establish a route to implementation; profiling establishes whether that route is appropriate for this game.

**Art production references:** [Aseprite CLI](https://www.aseprite.org/docs/cli/), [sprite sheets](https://www.aseprite.org/docs/sprite-sheet/), [Kenney licensing](https://kenney.nl/support), and the individually linked asset pages in section 30. No candidate pack is automatically final game art.

**Local evidence:** the audited `outputs/arcelite-psp` source tree, its README/build notes, existing generated sprites, asset tools and packaged emulator log. This document describes current code where inspected and labels proposed replacements. It is not a physical-hardware certification or a new build report.

Before distributing, choose a final title after checking naming/branding suitability. “ELITE: STARFARER — The Open Channel” is a working title, not a cleared commercial identity.

## 39. Handoff to the next implementation session

Paste the following prompt alongside this entire document and the current project. The document is the durable specification; the source remains the evidence of what is implemented.

> Work on the existing PSP game in `outputs/arcelite-psp` using this design bible. First inspect the repository instructions, current source, build tools, save format and most recent successful EBOOT. Do not assume a proposed feature in this document already exists. Report the current version and compare the requested milestone with the source. Preserve the user's working build and save compatibility.
>
> Implement one bounded milestone or the specifically requested tickets, starting with P0 unless I select another. Preserve the native PSP target and retro visual identity. Keep Lave safe, names consistent, controls discoverable and essential activities recoverable. Reuse the running systems; avoid a wholesale rewrite. Treat all numeric design values as tuning proposals and measure changes on representative scenes.
>
> Use stable IDs, shared docking collision geometry, an explicit time policy, idempotent mission rewards and checked save recovery. Do not add a menu entry until it leads to a playable feature. Follow the mission scripts and original setting; do not copy No Man's Sky assets or story. Build assets reproducibly from documented inputs with a memory report and provenance.
>
> Run tests appropriate to the changes, including the relevant cases in section 34. Distinguish host tests, emulator tests and physical PSP checks. Do not claim a bug-free game, hardware performance or completed content without evidence. If a feature exceeds the budget, keep a useful smaller playable version and document the limitation.
>
> Deliver the changed source, a versioned EBOOT when the build passes, installation notes, concise controls, completed acceptance criteria and remaining issues. Update a progress ledger against sections 36 and 37 so the next session can continue without guessing. Do not silently count expansions as completed core features.

**Suggested first request:** “Implement P0, starting with save safety and the shared rotating docking aperture. Preserve the current controls. Show which regression cases pass and produce a versioned EBOOT when they do.”

**Design decisions still requiring playtest evidence:** travel time and boost consumption; comfortable manual docking clearance; encounter density; reward pacing; surface-region size; sprite animation readability; renderer choice; physical PSP frame pacing. Resolve these with the smallest relevant playable test. The creative direction is established; these are measured tuning questions, not reasons to stop building.

**Completion standard for the eventual game:** a new player understands their first flight, has several worthwhile activities without grinding, meets memorable people, finishes a satisfying story, can keep exploring afterwards, and can trust the game with their progress.
