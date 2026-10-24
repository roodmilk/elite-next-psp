# ELITE: NEXT — Planet Classes, Four-Region Landing and EVA Expansion

Status: approved implementation handoff 1.0  
Game baseline: ELITE: NEXT 2.5.344+  
Target hardware: Sony PSP, native 480×272  
Purpose: extend the existing planetary approach and EVA engine into a varied four-region-per-planet system without discarding its tested traversal, rover, discovery, landing or authored Lave work.

## 1. Product goal

Every landable planet should feel like a world rather than one repeated expedition field. Each planet receives a meaningful physical class and exactly four selectable landing regions. Regions combine authored identity with deterministic terrain, ecology, weather, sites and discoveries. The player chooses a region while already travelling through atmospheric clouds, preserving immersion and hiding staged preparation work.

The player experience is:

1. Fly toward and enter a planet normally.
2. Pass into dense moving cloud, dust, vapour or atmospheric cover appropriate to the world.
3. The ship enters an automatic holding descent while four region cards appear over the live view.
4. Select a region with the D-pad, confirm with X or abort with Circle.
5. Terrain, collision, props, weather, life and sites prepare incrementally behind the cover.
6. The cover dissipates into the existing approach, landing and disembark sequence.
7. Explore a dense medium-sized regional landscape on foot or by rover.
8. Return to the ship, take off and regain orbital flight before selecting another region.

## 2. Existing foundation to preserve

Do not replace the current Planetary EVA engine wholesale. Preserve and deliberately migrate:

- Orbit targeting, approach, automatic atmospheric descent, landing and takeoff.
- Cloud-covered seamless entry.
- First-person walking, looking, strafing and bounded jet movement.
- Rover driving, braking, reversing, steering, boost, drift and dismount.
- Shared terrain rendering and collision contract.
- Water rejection and safe local boundaries.
- Ship bearing, boarding volume and return route.
- Terrain, vegetation, weather, particles and ambience.
- Eight discoverable subjects and existing life behaviour families.
- Surface sites, point-and-click scenes, activities and rewards.
- Field Map, Codex and persistent discovery records.
- Ports, landing terraces, observatory and authored Lave scenery.
- Current mission validation and reachable-object guarantees.
- PSP-native art, bounded pools and no per-frame heap allocation.

Existing documentation to reconcile rather than duplicate:

- `docs/PLANETARY-EVA-HANDOFF.md`
- `docs/PLANETARY-ENGINE-AUDIT.md`
- `docs/PLANET-INTEGRATION-249.md`
- `docs/LAVE-I-LUSH-EXPLORATION.md`
- `docs/LAVE-WORLDS-239.md`
- `docs/PLANET-BEAUTY-253.md`
- `docs/PLANETARY-PUSH.md`
- `docs/PLANET-SITE-SCENES.md`

## 3. Planet class and biome are separate

A **planet class** describes the entire world's physical identity: composition, atmosphere, gravity, temperature, water, broad hazards and orbital appearance.

A **biome** describes the local environment inside a selected landing region: terrain, ground cover, vegetation, local weather, life, resources and sites.

Example:

```text
Lave I
Planet class: Temperate Terrestrial

Region: Observatory Ridge
Dominant biome: Temperate Forest
Secondary biome: Rocky Highlands
Transition: trees thin, ground steepens, exposed stone increases
```

Do not treat colour variation alone as a new planet class. Classes must create meaningful visual or gameplay differences.

## 4. Planned planet classes

Implement a bounded class catalogue of roughly 14–16 types:

1. Temperate terrestrial
2. Ocean world
3. Desert world
4. Ice world
5. Volcanic world
6. Barren rocky world
7. Toxic world
8. Jungle world
9. Swamp world
10. Super-Earth
11. Gas giant
12. Ice giant
13. Cloud world
14. Tidally locked world
15. Artificial or engineered world
16. Rogue planet

Each class defines:

- Valid orbital themes and atmosphere.
- Gravity range.
- Temperature and exposure range.
- Water or fluid coverage.
- Valid dominant and secondary biomes.
- Valid weather.
- Valid life and mineral families.
- Surface/sky-platform hazards.
- Mission and site suitability.
- Landing-cover effect.
- Codex description vocabulary.
- Audio and ambient profile.

Suns remain navigation bodies and are never landable regions.

## 5. Four regions for every landable planet

Every landable planet exposes exactly four selectable regions. Do not make famous planets mechanically privileged by giving them more slots. Important worlds distinguish themselves through authored quality, landmarks, story use and visual identity inside the same four-region structure.

Four regions provide:

- A predictable PSP interface.
- Enough variety to represent a planet.
- Stable save/discovery indexing.
- Bounded content and test matrices.
- Clear mission destinations.

Each region should normally take roughly 10–15 minutes to cross on foot from one meaningful edge or landmark cluster to another. Rover travel should make traversal substantially faster. Avoid enormous empty worlds and avoid tiny arenas.

Each region contains:

- One dominant biome.
- Zero or one secondary biome.
- One or more natural transition areas where appropriate.
- A safe landing location.
- Several visible navigation anchors.
- A bounded set of authored/procedural sites.
- Regional wildlife, flora and minerals.
- Regional weather and ambience.
- Discoveries and Codex identity.
- A natural-looking playable boundary.

Playable boundaries must be disguised as believable geography: dense rocks and trees, cliff faces, deep water, ravines, unstable ground, ice walls, lava, toxic marsh, artificial bulkheads or comparable class-appropriate features. Avoid visible force fields, square walls, abrupt terrain endings and endless voids unless an authored artificial location explicitly justifies them. If a player still reaches the limit, provide a concise environmental warning and gently prevent further movement without teleporting or snapping the camera.

Regions may contain explorable caves, tunnels, mines and underground facilities. Entrances must be visible, collision-safe and linked to bounded interior geometry. Interior transitions may use a short darkness, airlock, lift or tunnel mask to prepare the interior without a visible stall.

## 6. Transition areas

A transition area is a portion of one region where the dominant biome gradually becomes its secondary biome. It is not a fifth region and is not selected separately.

Examples:

- Forest gradually thinning into rocky highlands.
- Grassland becoming marsh near a lake.
- Desert dunes giving way to canyon badlands.
- Glacier becoming exposed stone near a geothermal valley.
- Jungle becoming mangrove coast.
- Volcanic ash becoming an active lava field.
- Permanent night ice becoming twilight scrub on a tidally locked world.

Transitions interpolate or progressively replace:

- Terrain shape and material.
- Ground colour and texture.
- Props and vegetation density.
- Wildlife and mineral tables.
- Fog, particles and local weather.
- Ambient sound.
- Hazards and traversal conditions.

Use deterministic masks based on the planet seed and region profile. Avoid hard straight borders, instant palette swaps or visible square biome tiles.

## 7. Immersive cloud-region selection

Do not cut to a separate conventional map menu. Region choice occurs during atmospheric entry.

### Entry flow

1. The existing approach begins.
2. Appropriate atmospheric cover thickens until the surface is fully hidden.
3. Space simulation and inappropriate flight actions stop safely.
4. The ship maintains a looping descent/holding animation.
5. Four region cards fade into the live cloud view.
6. Selection remains responsive for as long as needed.
7. X commits to a region; Circle aborts and performs a visible climb back to orbit.
8. After confirmation, flight remains automatic: clouds clear directly into the selected destination's landing sequence.

There is no free atmospheric flight mode over a selected region. The player does not steer around the planet between the selector and touchdown. Region choice, loading cover, reveal and landing form one seamless automatic descent.

Keep descent primarily in first-person. One brief external cinematic shot may show the player's actual ship approaching or touching down at the selected site, then return cleanly to first-person/disembark presentation. The shot must use the correct ship model, paint and region geometry and must never delay control unnecessarily.

### Native 480×272 region cards

Use a stable two-by-two card arrangement in the lower/middle safe area. Each card may show:

- Region name.
- Dominant and secondary biome names/icons.
- Landing type: port, settlement, wilderness, platform or special site.
- Concise danger level.
- Survey percentage.
- A compact survey progress bar paired with the percentage.
- Tiny native-size representative regional preview where space allows.
- Mission marker when appropriate.

The selected card uses the current UI selection language without obscuring the moving atmosphere. A mission destination pulses separately and becomes the initial selection. Long names must wrap or shorten cleanly.

Do not remember the last highlighted region between approaches. Begin from the stable default region unless a tracked mission selects its required destination.

Preview art represents the region's identity but does not update for current daylight or weather. Current hazards and weather may still be communicated concisely through readable text or icons.

### Controls

- D-pad moves predictably through the four cards.
- X confirms the selected region.
- Circle aborts entry and returns to space through an animated climb.
- Flight controls remain suspended during selection.
- Carried buttons cannot accidentally confirm, fire or abort.

## 8. Atmospheric loading mask

Use atmospheric cover to hide staged preparation, not as a fixed artificial delay.

Possible cover by class:

- Clouds and rain for temperate/ocean worlds.
- Dust and sand for desert worlds.
- Snow and ice haze for frozen worlds.
- Ash and smoke for volcanic worlds.
- Toxic fog for toxic/swamp worlds.
- Dense cloud bands and lightning for gas/ice giants.
- Re-entry plasma, light panels or signal interference for artificial worlds.
- Darkness, frost and sparse particulate for rogue worlds.

### Staged preparation

Split work across frames:

1. Make atmospheric cover opaque.
2. Stop drawing hidden space objects.
3. Release or recycle unnecessary space caches.
4. Present lightweight region metadata immediately.
5. After confirmation, initialise the region seed and profile.
6. Generate terrain height/material caches.
7. Build collision and reachable-path data.
8. Prepare prop palettes and bounded draw lists.
9. Place port/clearing, landmarks, mission sites and boundaries.
10. Prepare weather, life, discoveries and ambience.
11. Render a hidden warm-up surface frame.
12. Thin the cover only when the initial playable view is ready.

Never perform all generation in one visible frame. Keep cloud animation, selection and audio responsive. If preparation takes longer, extend the atmospheric loop naturally rather than revealing incomplete terrain.

This masks loading spikes but does not excuse poor surface performance. Region rendering still requires culling, bounded transparency, fixed pools and measured draw distance.

## 9. Region landing types

Not every region requires a spaceport.

Supported landing types:

- Major port or settlement terrace.
- Scientific station.
- Agricultural or industrial pad.
- Wilderness clearing.
- Beach, plateau or ice landing zone.
- Floating platform.
- Cloud city dock.
- Airborne research station.
- Artificial habitat bay.
- Emergency/story landing site.

Wilderness regions use a safe, visually credible landing clearing selected and validated by terrain rules. The ship remains a reliable return/save-direction anchor. Do not place the ship on steep ground, water, hazards, props or blocked routes.

Wilderness clearings are fixed authored locations on important worlds and deterministic prevalidated locations elsewhere. Do not add a freely movable landing cursor. This keeps collision, mission placement, return routes and PSP presentation reliable.

Revisiting a region returns to the same validated landing location. Terrain, ship placement, return routes and major landmarks remain stable across visits.

Ships land only at each region's validated clearing, pad, platform or dock. Free landing on arbitrary terrain is outside this design.

## 10. Gas giants, ice giants and cloud worlds

These classes require varied solid 3D destinations rather than ordinary ground terrain.

Their four regions may include:

- Floating research platforms.
- Cloud cities.
- Refinery decks.
- Suspended observatories.
- Balloon-supported settlements.
- Storm-monitoring arrays.
- Abandoned atmospheric stations.
- Layered causeways between structures.

Every region must have a different silhouette, deck plan, cloud colour, weather character and purpose. Avoid reusing one flat platform with a palette change.

Use solid 3D geometry with collision-matched walkable decks, visible undersides, parapets, machinery, lights, traffic and distant platform silhouettes. Cloud layers are atmosphere, not invisible ground. Falling or leaving walkable geometry must have a clear recovery or failure rule.

Artificial or engineered worlds may divide their four regions between inhabited interior habitats and exterior hull landscapes. Exterior regions can include maintenance trenches, antenna fields, armour surfaces, docking structures and damaged superstructure; interior regions can include parks, districts, industrial decks or abandoned habitat volumes.

## 10A. Water and underwater scope

The first regional rollout focuses ocean worlds on islands, coasts, wetlands, floating structures and shallow water. Full underwater exploration is a later phase and must not block the four-region framework.

Players may swim or wade through safe shallow water. Deep water requires suitable equipment or a future dedicated vehicle/system. Without that equipment, depth, current or suit warnings create a visible recoverable boundary rather than an invisible wall or instant death.

Later underwater regions may add reefs, trenches, submerged ruins, research habitats and aquatic life through the same region/profile contract. Do not claim them implemented until movement, camera, oxygen, collision, recovery and performance are certified.

## 11. Gravity and movement

Planet class and world seed determine gravity within safe authored ranges. Gravity should affect:

- Jump/jet rise and fall.
- Falling speed.
- Footstep feel.
- Rover grip, suspension and airtime.
- Particle fall and thrown/debris motion where present.

Differences should be noticeable but remain controllable on PSP. Preserve the existing reliable control contract and avoid turning high-gravity planets into sluggish frustration or low-gravity planets into uncontrollable bouncing.

UI/Codex should state gravity clearly. Movement tests must cover minimum and maximum supported gravity.

Night and low-visibility exploration use working ship landing lights and a suit light. Lights must illuminate useful navigation space without excessive transparent overdraw or abrupt clipping.

Ship and suit lights activate automatically when local lighting or visibility falls below a safe threshold. Players retain a manual toggle; manual-off may be overridden only by critical safety/tutorial requirements. The current state must be visually obvious without persistent intrusive messages.

## 12. Hazards and equipment

Hostile regions may require suitable suits, shielding or vehicle upgrades. Examples:

- Extreme heat/cold.
- Toxic atmosphere.
- Radiation.
- Corrosive rain.
- Volcanic gas and lava.
- Electrical storms.
- High pressure.
- Low visibility.

Requirements must be communicated before confirmation. The card should show the hazard and missing protection. Do not let players unknowingly enter an immediately lethal region.

Every class needs safe introductory examples. Hazard progression should create outfitting goals without blocking all early planetary exploration.

Equipment-locked hazardous planets may appear in any part of the galaxy rather than only in late-game space. Their requirements must be visible before descent. Mission and story validation must never assign a mandatory objective there until the required protection is obtainable by that commander.

Some selected settlements contain explorable interiors such as shops, bars, laboratories, administration rooms, clinics, engineering spaces or habitation areas. Reuse the game's point-and-click/adventure presentation and native dialogue standards where appropriate. Do not require a full interior for every regional settlement.

The number of inhabited regions follows system population, economy, history, government and habitability. Do not impose a fixed settlement count by planet class. A temperate world may be untouched wilderness; an engineered barren world may be densely occupied.

## 13. Saving, death and persistence

Saving remains restricted to space stations. Do not add surface, ship, shelter or port saving in this expansion.

On EVA death, retain the established death presentation and offer:

- Load Commander / last save.
- Return to Main Menu.

Unsaved regional progress after the last station save may be lost. Communicate the save restriction before long hazardous expeditions where appropriate.

Persist through normal station saves:

- Landed/discovered regions.
- Scanned flora, fauna and minerals.
- Completed one-off sites and rewards.
- Survey completion.
- Required story and mission state.
- Region survey contribution to the system Discovery Record and claimed milestone state.

Revisiting a region must not duplicate one-time rewards. Renewable resources must be explicitly classified and bounded.

## 14. Mission and objective integration

All four detected regions are immediately selectable. Missions may target a specific planet and region.

All four region names and their basic orbital-scan information are visible immediately. Landing and surveying reveal detailed ecology, sites and records rather than merely revealing the name.

- The required region card pulses and displays `MISSION DESTINATION`.
- It becomes the initial selection.
- The player may still inspect other cards before confirming.
- Mission validation confirms that the planet, region, landing site, objective, route and required equipment exist.
- Objectives spawn only in reachable, collision-valid locations.
- Aborting atmospheric entry does not advance the mission.
- Landing in the wrong region does not falsely complete the objective.

## 15. Lave I certification slice

Use Lave I as the first complete proof without discarding Observatory Ridge.

Recommended four regions:

1. **Observatory Ridge** — temperate forest to rocky highlands; existing port, flower trail, giant trees and observatory remain intact.
2. **Western Coast** — woodland, cliffs, beaches, islands, sea life and coastal weather.
3. **Lake Country** — grasslands, lakes, rivers, wetlands and scattered woodland.
4. **Agricultural Belt** — fields, settlements, roads, farms and local industry.

Lave I should migrate from the internal `OCEAN` plus forced `FOREST` combination to a meaningful Temperate Terrestrial class with substantial water coverage. Existing body identity, story, discoveries and observatory progress must remain valid.

Certification must prove:

- All four cards fit and work at 480×272.
- Cloud selection/abort/confirm never exposes loading.
- Each region has a distinct silhouette and biome identity.
- Transitions are gradual and collision-safe.
- Ship, rover, sites and boundaries remain reachable.
- Observatory Ridge retains its existing story and landmark behaviour.
- Stable performance on emulator and physical PSP.

## 16. Ordinary-world generation and authored quality

Four regions per planet does not mean four random colour swaps. Create region profiles from class-compatible authored templates and deterministic seeds, then review their output.

Each region identity includes:

```c
typedef struct {
    unsigned seed;
    unsigned planet_class;
    unsigned dominant_biome;
    unsigned secondary_biome;
    unsigned weather;
    unsigned hazard;
    unsigned landing_type;
    unsigned culture;
    unsigned site_set;
    unsigned ambience;
} RegionProfile;
```

Important worlds receive authored names, landmark placement and art direction within the same four-region contract. Procedural worlds still require uniqueness checks and representative manual inspection.

Regions use stable local planetary time derived from the world and region coordinates. Revisiting may show dawn, day, dusk or night while retaining the same terrain and landmarks. Lighting changes must preserve navigation, readable UI and mission-object visibility.

Weather may change between visits according to the class, biome, region seed and local time. Changes remain deterministic and plausible, preserve mission reachability and cannot place the player into unavoidable immediate death on arrival.

Weather may also evolve gradually during an active expedition. Use readable transitions in cloud, wind, light, particles and ambience rather than instant state changes. Dangerous escalation requires advance warning and cannot invalidate the only safe return path.

Not every planet needs an inhabited region. Some worlds may have four wilderness, ruin, scientific-remnant or natural regions. Settlements and ports must follow system population, economy, history and habitability rather than appearing merely to fill a slot.

Populated regions may contain bounded moving pedestrians, rovers, service vehicles and local traffic. Activity density follows settlement scale and PSP performance budgets. Actors need simple readable routines and collision-safe routes; do not simulate a whole city off-screen.

Appropriate wildlife may become hostile and injure the player. Aggression must be species-specific, clearly telegraphed and compatible with the region's ecology. Safe introductory worlds and non-hostile species remain common.

Within bounded nearby simulation, wildlife may hunt, flee, defend territory, interact with other species, threaten NPC explorers or approach settlement edges. Do not simulate distant ecosystems off-screen. Encounters must respect population caps, mission safety, performance budgets and clear player-readable behaviour.

Players may defend themselves with both non-lethal tools and suitable ordinary weapons. Non-lethal deterrence, stunning or repelling should usually be the safer ecological choice, while lethal defence remains possible where equipment and rules permit. Wildlife reactions, rewards, reputation and Codex records must never encourage indiscriminate farming. Weapon input must not conflict with scanning, interaction, boarding or rover controls.

Severe weather may damage the player, rover or exposed ship systems only after clear visual, audio and UI warning. Forecasts and region cards identify known extreme conditions. Every damaging event requires shelter, retreat, suitable equipment or another fair response; no unavoidable damage immediately after landing.

A region reaches 100% survey completion when its required major sites are discovered and its required regional flora, fauna and minerals are scanned. Optional secrets may remain outside the percentage only when labelled accordingly. Completion contributes once to the containing system's Discovery Record milestones.

A completed region card shows one clean, small completion mark without adding celebratory clutter or reducing name/biome readability.

Completing a region grants one small, clearly presented survey reward in addition to its contribution toward larger system Discovery Record milestones. Reward size may reflect hazard and survey complexity but must remain bounded and claimable only once.

Do not add fast travel within regions. Players travel on foot, by rover or through any future explicitly simulated vehicle. Region scale and objective spacing must respect this decision.

Fully surveyed regions retain explicitly renewable activities such as bounded mining resources, wildlife observation and suitable local jobs. One-time discoveries, milestones and rewards remain one-time.

Renewable mineral nodes replenish after a defined in-game interval rather than on every landing or menu reload. Wildlife populations also recover gradually between visits according to local time and ecology. Save/load and rapid orbit/landing cycles must not bypass these timers.

## 14A. Field Map redesign

Include a native 480×272 Field Map update as part of this expansion. It should communicate the selected region rather than pretending to represent the entire planet.

Show:

- Dominant and secondary biome areas with readable colour/pattern differences.
- Natural transition area.
- Major paths, rivers, coasts, cliffs and natural boundaries.
- Ship and validated landing site.
- Discovered settlements, caves, interiors and major sites.
- Current mission objective.
- Scanned/known navigation targets.
- Player and rover position and facing.
- Region survey percentage and clean completion mark.

Do not show undiscovered secret sites, use free-form player markers or reveal wildlife through terrain. Keep panning/navigation coherent with the existing map controls, and ensure the map never moves the commander.

During atmospheric entry the player may move freely among all four region cards. Nothing commits, loads irreversible state or advances a mission until X confirms the highlighted region. After X, the cards disappear and descent preparation continues toward that selection.

Do not add free-form persistent player map markers. The map shows authored sites, discoveries, missions, ship location and validated navigation targets only.

Regional mission families may include rescue, wildlife management, scientific survey, delivery, repair, hunting, archaeology, salvage, missing persons, sample recovery and other objectives compatible with the local class/biome. Every generated offer must pass destination, equipment, reachability, target and return-path validation.

## 17. Save migration

Map legacy categories without changing system/body identity:

- `ROCKY` becomes an appropriate barren, desert, ice, volcanic, toxic, temperate or Super-Earth class based on existing seed and canonical overrides.
- `OCEAN` becomes ocean, temperate, jungle or swamp where appropriate.
- `GAS` becomes gas giant, ice giant or cloud world.

Existing discovery records attach to a deterministic default/legacy region, normally region zero. Lave I records attach to Observatory Ridge. Never silently delete landed flags, scans, sites, rewards or mission state.

The save version must migrate old files and reject corrupt region/class values safely.

## 18. Performance rules

- Generate and retain only the current region's expensive surface state.
- Region metadata for selection remains compact.
- Use fixed pools and bounded caches.
- Cull terrain, props, life and structures outside useful view ranges.
- Use cheap distant silhouettes beyond the playable terrain.
- Limit transparency layers and large overdraw.
- Maintain rendering/collision agreement.
- Avoid per-frame procedural string or terrain allocation.
- Benchmark dense forest, storms, gas platforms and maximum-gravity rover scenes.
- Treat emulator results as guidance; certify on physical PSP.

Region size alone does not determine FPS. Visible geometry, sprite density, transparency, particles, draw distance and cache behaviour determine the real cost.

## 19. Implementation stages

### Stage A — class data and migration

- Add planet classes and compatibility tables.
- Preserve body identity and old saves.
- Update orbital art, Codex language and mission validation.

### Stage B — four-region framework

- Add deterministic region profiles and progress indexing.
- Implement region-aware landing, discoveries and sites.
- Keep current region as the legacy default until selection is certified.

### Stage C — atmospheric selector

- Add two-by-two cards over continuous atmospheric entry.
- Add confirm, abort, mission focus and carried-input safety.
- Stage preparation across frames and warm the first surface frame.

### Stage D — Lave I vertical slice

- Preserve Observatory Ridge.
- Add Western Coast, Lake Country and Agricultural Belt.
- Certify transitions, navigation, visuals, objectives and performance.

Do not begin broad galaxy rollout until all four Lave I regions pass the integration and physical-PSP gates.

### Stage E — class/biome rollout

- Build class-specific orbital/surface/audio/hazard profiles.
- Create biome kits and transition rules.
- Create four varied 3D regions for gas/ice giants and cloud worlds.
- Roll classes out in small tested groups, preserving a working build after each group rather than attempting all classes in one unverified release.

### Stage F — galaxy certification

- Validate every active planet and all four regions.
- Render native 480×272 review captures.
- Test migration, persistence, missions, entry, exit and physical PSP performance.

## 20. Automated acceptance tests

The completed implementation must prove:

- Every landable planet exposes exactly four valid region profiles.
- Suns remain unlandable.
- Every region belongs to a biome allowed by its planet class.
- Dominant/secondary transitions contain no abrupt invalid terrain seams.
- Region cards fit at native resolution with worst-case names.
- D-pad navigation reaches all four cards predictably.
- X confirms once; Circle aborts once; carried buttons are harmless.
- Confirmation continues directly into automatic descent and landing; no free atmospheric controls become active.
- The optional external landing shot uses the correct ship, paint, camera clearance and selected region.
- Loading never reveals uninitialised terrain, props or collision.
- First visible surface frame is complete.
- Wilderness and port landings are stable and reachable.
- Revisits reproduce the same safe landing location and major geography.
- Ships cannot land outside validated regional landing sites.
- Shallow-water movement and deep-water equipment boundaries are visible, recoverable and collision-consistent.
- Gas/cloud regions use real collision-matched 3D platforms.
- Caves and underground facilities load safely and return to the correct region.
- Artificial-world interior and exterior regions preserve coherent world identity.
- Gravity extremes remain controllable on foot and by rover.
- Ship and suit lights remain useful and performant in every supported night/fog profile.
- Hazard requirements are visible before entry.
- Missions target real reachable region objectives.
- Mandatory missions never require hazard equipment before it is obtainable.
- Save migration preserves legacy progress.
- One-time rewards cannot be farmed by revisiting or reloading.
- Death offers load-last-save or main-menu choices.
- Station-only saving remains enforced.
- Surface, approach and return stay within PSP memory and performance gates.
- Survey completion updates the correct system Discovery Record milestones exactly once.
- Revisiting at different local times preserves geometry, collision and objective visibility.
- Weather variation preserves reachable paths and safe arrival conditions.
- Mid-expedition weather transitions are gradual, warned and never cut off the only return route.
- Natural boundary geometry contains the player without visible map edges, teleports or camera snaps.
- Settlement pedestrians and vehicles remain bounded, collision-safe and within performance limits.
- Every regional mission family validates its equipment, target, route and completion state.
- Hostile wildlife damage and warning behaviour are deterministic, avoid unavoidable spawn attacks and respect death/recovery rules.
- Wildlife-to-wildlife/NPC/settlement interactions remain bounded, readable and performant.
- Non-lethal and lethal wildlife-defence inputs work without colliding with scan/interact/vehicle controls.
- Severe weather always warns before damage and offers a reachable response.
- Region survey percentage reaches 100% exactly when its required sites and life/mineral records are complete.
- Region percentage and progress bar agree at every state.
- Completed region cards show a clean completion mark.
- Region completion reward is granted once and reconciles with system milestones.
- Renewable post-survey activities do not duplicate one-time discoveries or rewards.
- Mining and wildlife recovery obey in-game timers across save/load and rapid revisits.
- No fast-travel action appears in the field map or region UI.
- Field Map biome, path, boundary, ship, rover and objective positions agree with the playable region.
- Switching cards before X has no landing, mission or save-state side effects.
- No player-created markers leak into the save format or UI.
- Selected settlement interiors enter and return to the correct region safely.

## 21. Confirmed owner decisions

1. Every landable planet receives four regions.
2. Regions normally take roughly 10–15 minutes to cross on foot; rovers are faster.
3. Wilderness regions may use safe clearings instead of ports.
4. Gas giants use varied, cool solid-3D platforms, cities and stations.
5. Regions normally combine one dominant and one smaller secondary biome through natural transition areas.
6. Gravity affects movement and vehicles while remaining controllable.
7. Hostile regions may require equipment, with safe introductory areas.
8. Saving remains available only at space stations.
9. EVA death offers loading the last save or quitting to the main menu.
10. All detected regions are immediately selectable and mission destinations are clearly marked.
11. Region choice occurs immersively inside continuous atmospheric cover rather than on a separate map screen.
12. Atmospheric cover hides staged preparation to prevent visible landing stutter.
13. Wilderness regions use fixed, prevalidated safe clearings rather than a movable landing cursor.
14. All four region names are visible immediately.
15. Selected settlements may contain explorable shops, bars, laboratories and other interiors.
16. Equipment-locked hazardous planets may appear anywhere in the galaxy.
17. Mandatory missions cannot target inaccessible hazard regions before protection is obtainable.
18. Regional surveys contribute to system Discovery Record milestones.
19. Revisits reflect local planetary time and changing daylight.
20. Weather may change deterministically between visits.
21. Ship and suit lights support night and low-visibility exploration.
22. Region preview pictures remain representative rather than live weather/daylight renders.
23. Some planets may have no inhabited region.
24. Appropriate hostile wildlife may injure the player.
25. Players cannot place free-form persistent map markers.
26. Ship and suit lights activate automatically in darkness and retain a manual toggle.
27. Players may use non-lethal tools or suitable ordinary weapons against hostile wildlife.
28. Severe weather may cause damage only after clear warning and with a fair response.
29. Required sites plus flora, fauna and mineral scans determine 100% regional survey completion.
30. Regions have no fast travel.
31. All four cards remain freely selectable until X confirms the landing region.
32. Initial ocean-world regions focus on coasts, islands, wetlands and shallow water; full underwater exploration is later.
33. Players may swim/wade in safe shallow water; deep water requires equipment.
34. Regions may contain explorable caves and underground facilities.
35. Artificial worlds may combine interior habitat and exterior hull regions.
36. Settlement presence follows population and history rather than planet-class quotas.
37. Nearby bounded wildlife ecology may involve other animals, NPC explorers and settlements.
38. Natural rocks, trees, cliffs, water and hazards contain players without obvious invisible walls.
39. Revisiting a region uses the same validated landing location.
40. Weather may change gradually during active exploration.
41. Populated regions may show bounded moving pedestrians, rovers and local vehicles.
42. Regions support varied validated surface mission families.
43. Lave I's complete four-region certification precedes class-by-class galaxy rollout.
44. Changing regions requires takeoff and return to orbit.
45. Confirming a region leads through automatic clouds directly into landing; there is no free atmospheric flight.
46. Ships land only at validated regional sites.
47. The selector does not remember the last highlighted region; missions may choose the initial card.
48. Completed regions show one clean, simple completion mark.
49. Fully surveyed regions retain renewable mining, observation and local-job activities.
50. Descent stays mostly first-person with one optional brief external landing shot.
51. Region cards show both survey percentage and a compact progress bar.
52. Renewable minerals and wildlife recover according to in-game time rather than every visit.
53. Each region grants one small one-time completion reward in addition to system milestones.
54. The project includes a native Field Map redesign for regional biomes, routes, boundaries and objectives.

## 22. Remaining implementation choices

The owner has confirmed the product behaviour. Engineering may choose exact bounded reward values, recovery intervals, palette indices, cinematic duration and cache sizes, provided all acceptance tests and PSP performance gates remain satisfied.
