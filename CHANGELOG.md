## 2.5.252 — Elite Exploration radio

Added Elite Exploration as an eighth selectable station with a dedicated `music/Elite Exploration` OGG/MP3 folder, exploration ticker, green/gold visual identity and distinct generated fallback soundtrack. Kept SPACE TALK and VOID TALES as the only scripted presenter channels. Radio settings now save as version 3 while safely migrating the old version-2 station-7 OFF marker. Radio regression suite passes with zero failures; broader inherited input/outfitting failures remain outside this scoped change.

## 2.5.250 — Biome-grounded planetary landmarks

Replaced the universal grey POI plinth with a shallow four-corner terrain berm using deterministic ocean/forest/desert/ice/volcanic soil palettes. Engineered sites receive a thin inset footing blended into that ground; ruins, fossils, wrecks and crystals receive no manufactured pad. Structure geometry, collision reservations, interactions, activities, scanner identities and save layout are retained. Tightened rendering safety after comparison tests exposed 4–8 edge pixels on procedural worlds: broad box rejection now stays on the profiled Lave layout and all 144 reference/fast frames match exactly. Field-art, planetary transfer/map, game/radio/steering and Rich-station checks pass. Proof: docs/POI-GROUNDING-250.md.

## 2.5.249 — Planetary side-work integrated into cumulative main build

Merged the independently tested planetary candidate into the verified 248 source, preserving Rich/Poor/Mega architecture, station targeting and all intervening work. Automatic inward cloud-covered entry; faster clouds and pixel-identical surface raster/frustum optimisations; varied open landing platforms; higher/farther Lave I observatory; nub-pan Field Navigator; inertial R-throttle/L-brake-reverse/Circle-drift/R+X-boost Roamer with temporary collision cracks. Saved commander layout unchanged. Updated the obsolete zero-entry-speed assertion to retained speed while preserving its weapon/threat/damage checks. Exact-binary station, planetary, gameplay/map/debug/radio/steering reports pass. Full inherited input suite, costly cinematic shots and hardware performance remain uncertified. Proof and hashed delivery: docs/PLANET-INTEGRATION-249.md.

## 2.5.248 — Procedural middle-tier Rich stations

Added sixteen Rich-only whole-hull layouts and seeded pod/array/billboard combinations inspired by the user's retro-future references. Expanded Rich profiles, reduced rotation, added greenhouse/solar/deck facade patterns, larger collision-checked guidance lattice and useful standard/enhanced comms ranges. Existing Poor/auxiliary/capital architecture, saves and cumulative 247 work retained. Dedicated 174-system/696-route checks and final station/game/map/radio/steering regressions pass. Updated obsolete fixed-position tests to generated entrances without relaxing collision checks. Actual native views, performance limits and delivery in docs/RICH-STATIONS-248.md.

## 2.5.247 — Stable station identities and exact-hub docking

Fixed the nearest-hub/main-name mismatch that could label a small relay as a Mega Capital. Three stable physical station targets now share consistent scanner, HUD, contact, radar and Comms identity. Almanac main locks stay on the real primary; auto-dock requests honor the selected hub. All 256 main hulls match their class, including nine city-sized capitals; 3088 focused identity/UI checks pass with station, capital, map/debug and gameplay regressions. Existing saves and cumulative 246 content retained. Notes: docs/STATION-IDENTITY-247.md.

## 2.5.246 — Distinct pulp-era station silhouettes

Replaced the shared ordinary Coriolis-with-attachments exterior with eight deterministic whole-hull families, including continuous wheels with real gaps. Added four dominant capital crowns; all occur in the current galaxy. Shared geometry now drives primary-station drawing, rotated swept collision, guidance, NPC clearance and freight routes. Updated obsolete hard-coded collision-test positions without relaxing collision requirements. Exact-binary station, capital, gameplay, map/HUD, radio and steering reports pass. Full inherited input/performance suite and real PSP validation remain outstanding. Delivery/proof: docs/PULP-STATIONS-246.md.

## 2.5.245 — Curved pods and varied capital skylines

Added six shared convex shape templates, seed-selected architectural mixes and facade bands/glazing. Curved hulls use matching plane-swept collision instead of invisible solid box corners. Retains cumulative 244. Final city292/map/debug checks pass; native shape captures inspected. Notes and hashed delivery: docs/CAPITAL-VARIANTS-245.md.

## 2.5.244 — Resumed city stations and Field Map

Integrated saved mega-city geometry/rendering/collision/guidance/NPC changes and the new terrain chart/Codex return flow. Retains cumulative 243 airport and wildlife work. Native city/map, debug/HUD, transfer, airport, shadow and Lave activity checks pass. Broad game retains one older station collision assertion; full input suite and hardware performance remain outstanding. Notes and delivery: docs/CAPITAL-MAP-244.md.

## 2.5.243 — Heavy-ship planetary airports

Expanded shared airport footprint and pads, added futuristic architectural models, Python/Anaconda traffic, wider garage and matching map/collision/cloud decks. Retains cumulative 242. Final layout/native checks pass; known orbital-station test failure remains and broad input is incomplete. Details and hashed delivery: docs/STARPORTS-243.md.

## 2.5.242 — Fauna contact shadows

Added small terrain-following, depth-tested soft pixel shadows under discoverable animals in the shared planet renderer. Family-scaled footprint, height/distance fading and biome-colour blending; no save/input changes. Native shadow/animal and 241 integration checks pass. docs/FAUNA-SHADOWS-242.md records limits and hashed delivery.

## 2.5.241 — Combined debug tools, planetary map and small HUD pass

Integrated runtime-only unlimited fuel/range, modal explored-area planetary map and compact EVA condition meters. Hardened map input release/dead-state handling, preserved its fog across boarding, corrected grid bounds and native R glyph alignment. Retains cumulative cinematic and environment work. Focused final emulator and transfer checks pass; broad existing station/input failures remain. Isolated hashed delivery and notes: docs/INTEGRATED-241.md.

## 2.5.240 — Live first-person planet approach, seats and departure

Replaced the simplified transfer set with the production landscape renderer. Cloud-covered pre-entry, ten-second scenic approach, smooth touchdown/automatic airlock, animated ship/roamer seat transfers, live parked windshield, forward departure and pitched ascent. Preserved action/release gates and model state boundaries; added terrain/landmark clearance sampling. Expanded port apron/building layout and moved NPC pads away from the rover exit; static discovery specimens relocated off the runway without changing scan/save identity. Removed covered terrain/deck overdraw. Native transfer/input and Lave expedition checks pass; physical PSP, worst-frame optimisation and inherited station-collision failure remain. Details: docs/CINEMATIC-PORT-240.md.

## 2.5.239 — Remaining Lave planetary environments (local candidate)

Upgraded Lave II forest and IV ice environments using terrain/collision-matched hills and generated pixel-art prop families. Added an actual connected deck footprint, visible guard edges, research platforms, planted specimens and animated cloud backdrop for Lave III. World/Codex species palettes and regional/site descriptions agree with those environments. Existing species/site/save IDs and reward paths retained. Native reachability, landing/boarding, all 24 scans, 21 optional site activities, port jobs, repeat-reward protection and save/load checks pass; Lave I and fauna regressions pass. Physical PSP untested; IV sampled rendering remains above 30-FPS budget. See docs/LAVE-WORLDS-239.md.

## 2.5.238 — Fauna life, before wider biome rollout (local candidate)

Replaced sliding wildlife motion with deterministic idle/wander/feed/alert/flee/return states and 32 new pixel-art poses across eight existing families. Movement-distance gait, hopping, wing motion, resting poses, smooth heading changes, route clearance and animal separation. Walkers, running commanders and rovers startle animals at different distances. Existing discovery identity and save format preserved. Focused animal, art/scan/save and Lave movement tests pass; inherited broad station-collision failure remains, physical PSP untested. docs/FAUNA-LIFE-238.md contains evidence and limitations.

## 2.5.237 — Shared POI exterior and wildlife art (local candidate)

Reworked all twelve POI families into distinct detailed structures: arches, biodomes, lattice towers, faceted crystals, stacked cargo, rescue shelter, fossil dig, broken hull, thermal stacks, lookout, vault and observatory. Added physical signboards and port details while preserving site identity, existing collision reservations and interactions. New 48x64 pixel-art flora/fauna families are reused consistently in the world/guide/Codex, with seeded tints and simple animation. Added up to 24 depth-tested falling leaves within 180 m of real trees.

Native catalogue, POI, scanning/Codex, docked save/load, leaf and framebuffer checks pass. Lave trail/rover and interaction regression checks pass. Sampled emulator update + draw is 33.738 ms average / 36.096 ms worst, excluding audio/display wait/capture writes; no locked-FPS or physical PSP guarantee. Source art provenance and limitations: docs/POI-WILDLIFE-237.md. Isolated local EBOOT; shared root/dist, saves/music and unrelated dirty changes preserved. No public release.

## 2.5.236 — Lave I native rendering verification and repairs (local candidate)

Fixed vegetation using world X as screen X, truncated tree roots, pitch/depth projection, oversized near-sprite clamps, terrain-layer seams and the disconnected observatory location. Added one collision-matched rolling terrain surface with world-anchored grass/path detail, a domed observatory and longer-range woodland. Sprite spans, cached placements/heights and culling keep the added detail bounded. The visible shore now follows the same 40 m shoreline test as walking collision.

The real 480×272 renderer was inspected across camera turns/pitch, observatory approaches, dusk and night. Focused checks pass, including 202 path positions for walkers/rovers. Final emulator update + render cost: 31.600 ms average, 34.946 ms worst on the sampled route, excluding capture writes/display wait/audio. Not a reference-image match, locked-30-FPS guarantee or physical PSP certification. Details and remaining work: docs/LAVE-I-LUSH-EXPLORATION.md. Shared root/dist EBOOTs and unrelated dirty work are preserved; no public release.

## 2.5.235 — Lave I lush, walkable expedition landscape

Lave I now has a dedicated temperate-island treatment: softly rolling terrain that blends back to its ocean shore, three irregular distance-layered mountain ridges, denser grass, authored transparent pixel-art trees and understory plants, and a winding footpath leading to a guaranteed, large Sky Observatory. The shared vegetation-cell layout is also used for walker/rover tree collision; other planets and their existing terrain profiles are unchanged. Added source art, a repeatable 32-colour sprite bake, and development notes in `docs/LAVE-I-LUSH-EXPLORATION.md`.

Validation: PSP EBOOT build succeeds. No automated tests or emulator run in this implementation pass; visual scale/performance on PPSSPP and real PSP remain to be checked.

## 2.5.234 — One scheduled Local TV broadcast

Replaced manual three-channel tuning with a real-local-clock CH8 programme schedule. Clock, NEXT and LATER share the same schedule, including midnight rollover. Five-minute slots cycle Evening Orbit, Farmers' Market and Night Stories; opening joins the live caption position. Added low-level, smoothly enveloped speech babble on the SFX bus, a slower storyteller voice, radio fade while watching, and clearer two-direction ship traffic inside the studio window. Existing approved art and registered mouth motion remain. Circle is the only advertised control.

Native PSP build and focused scheduled-TV checks pass, including captured PCM, mouth/window movement and schedule/clock boundaries. Physical PSP verification remains. Baseline/candidate broad smoke both retain one station-collision failure, 138 input/outfitting failures and a campaign-input stall; radio/steering pass. No full-suite or published-release claim. Details: docs/LOCAL-TV-SCHEDULE.md.

## Unreleased local candidate — Giant background freighters

Distant freighters and their engine wakes now render before planets, so planetary discs correctly cover ships crossing behind them. Four larger modular silhouettes, six coordinated colour families, pointed prows and twin long tapered exhausts replace the two small enlarged fighter meshes. Passage time increases to 150–230 seconds for slower, weightier movement. Five full-flight PPSSPP cases pass the nominal 30 FPS test at 29.97–37.40 FPS; native clear and planet-blocked captures were inspected. Reachable freighter gameplay is unchanged. Local v2.5.232 candidate only; see docs/DEEP-FREIGHTER-POLISH.md.

## Unreleased local candidate — Soft flight skies without moving blocks

Replaced screen-space colour stamps and angle-dependent noise with continuous seeded nebula skies. Clouds follow the actual camera, remain stable under ship movement/time, and retain soft edges through pitch and roll. Slower stellar twinkle, tapered star rays and feathered lens-flare spots replace harsher flicker; removed the random 20 Hz extra dust overlay. All 256 generated skies have distinct content hashes. 48 focused rendering checks pass; sampled complete-flight averages are 29.97–37.40 FPS in PPSSPP, not a physical PSP guarantee. Local v2.5.232 candidate only; shared releases and existing gameplay preserved. See docs/SOFT-SPACE-SKY.md for captures, timing limits, known broad-suite issues and the tested EBOOT.

## Unreleased local candidate — Chart response and native text

Inverted vertical analog chart panning. Replaced stretched body/selected-name lettering with clean, smaller native 5x7 glyphs. Kept chart hold-to-zoom timing independent of the physics timestep cap and optimized duplicate filtering/background blending. Instrumented Jobs zoom drawing improves from 21.66 to 17.41 ms/frame in PPSSPP; the reported Jobs-only hardware slowdown remains to be confirmed on PSP. 26 focused checks pass. Latest separate EBOOT path is in CLAUDE-HANDOFF.md; shared release files untouched.

## Unreleased local candidate — Nine compact galaxy filters

Added UNVISITED, RICH, POOR, IN RANGE and ROUTE alongside ALL, VISITED, MEGA and JOBS. Five-row scrolling rail with a small scrollbar fits the existing top-left height. Jobs now means accepted work; range follows current fuel; route shows plotted stops. Context markers stay visible. 22 focused emulator checks pass; separate tested EBOOT and capture locations are recorded in CLAUDE-HANDOFF.md. No physical PSP or complete-release certification.

## Unreleased local candidate — Deep Chart visual polish

Starless authored blue-galaxy background replaces decorative false-star dots. Actual systems and route hops get soft glows; current/highlighted destinations use yellow/pink. Rebuilt glass panels, bigger lettering, bounded labels and readable search controls. Cached chart routing matches the selected destination's jump count. Existing zoom/pan/search/flight-route behavior preserved. Nine focused emulator checks pass; physical PSP and coordinated whole-game release verification remain pending. See docs/DEEP-CHART-VISUALS.md. Shared release binaries and other contributors' edits were not overwritten.

## 2.5.232 — TV-style station exploration shell

- Restyled the station walkabout's right rail as `EXPLORE`, with a stronger amber focus row tied to the glowing hotspot in the 340x168 room art.
- Added `HERE` as the first option in every station room. Selecting it restores the full room description that played on entry.
- The station header now names the current system's station while retaining live credits, persistent long-form reading, and bottom-edge controls.

## 2.5.231 — Local TV matches the approved studio artwork

Rebuilt Local TV from the original supplied full-studio concept at native 480x272. Preserves seated Mira, desk, plants, detailed orbital window and CH8 logo. Adds registered talking-mouth animation, masked passing ships, sun shimmer, star glints, sibling channel idents, readable native captions and three looping Lave programmes. Corrects frame-rate-dependent stalled text and prevents stale game notices from covering the broadcast. Left/Right tunes, X restarts, Circle returns to Discover. No save-format change.

PSP build and focused native TV tests pass. Broad smoke has the same inherited station-collision failure and campaign-input stall in both baseline and candidate; not a fully green release. See docs/LOCAL-TV-231.md for evidence, scope and physical-PSP checks still needed. Other contributors' Almanac/chart/portrait edits were preserved. No GitHub release pushed.

## 2.5.229 — Extreme Thargoid hyperspace routes

Thargoid interdictions now play as a staged first-person rail-shooter route. Five nose-on alien silhouettes enter in four formation styles, hold position to fire, dive past the cockpit and reform. The existing three-stage 45-kill blockade, 20-unit rewards, Select skip, battle music, victory continuation and fuel-safe defeat path remain intact.

Enemy wings now launch readable homing missiles alongside ordinary bolts. Missiles have bright trails, appear in a compact warning counter, can be destroyed by aiming and firing, and inflict heavier hull damage if they reach the player. The route continuously changes between open hyperspace, giant asteroid tunnels, alien crystal lanes, megastructure rings and a rotating void storm. Periodic velocity surges lengthen every star and debris streak without altering save or navigation state.

Five short procedural sounds distinguish player rail fire, alien fire, missile launches, enemy breakups and speed surges. They are bounded, softly enveloped and continue to duck beneath user-provided battle music. All arrays remain fixed-size for PSP memory safety.

Verification: PSP build passed. Encounter checks pass for skip priority, countdown safety, victory/defeat route preservation, 45-enemy completion, finite formation passes, all five routes, destructible missiles, missile damage and every new sound. Native framebuffer captures confirm all five routes are rich and visually distinct. Planetary Square target cycling, Circle scanning, Codex links and the planetary ambience layer remain present in this same build. The inherited station swept-collision test remains the only broader game-suite failure.

## 2.5.229 — Distinct animated effects for every fitted weapon

All 22 fitted primary weapons now have dedicated native-pixel firing signatures instead of sharing five generic beam shapes. Pulse weapons travel as packets; beams carry illuminated cores; mining fire uses an amber cutting crown; scatter, flare and shard weapons visibly fan out; ion and phase weapons travel as animated globes; arcs and ripples use different wave geometry; lances and rails have precision needles and impact rings; and Blackstar has a dark core surrounded by an unstable violet corona. Muzzle light and impact colour feed through the existing canopy bloom without textures, allocation or save-state changes.

Rainbow Prism now genuinely cycles its gold, pink and cyan ordering on successive shots, matching its Outfitting description. Automated PSP framebuffer checks confirm that all 22 weapons produce distinct visible signatures and that Prism changes between shots. The PSP build passes. The broader inherited suite still reports its pre-existing station swept-collision failure.

## 2.5.212 — Ship Tech Board shows usable slots only

The Ship Tech Board no longer displays meaningless locked placeholders. Every WPN/DEF/NAV/HOLD/FUEL/UTIL row shows only the one to four slots supported by the current hull and expands those real slots across the available width. Empty usable slots remain. Navigation stays within actual capacity, installed totals ignore inaccessible storage, and Law Scanner now has a safe board abbreviation. Triangle no longer opens Outfitting from this page. The footer advertises SQUARE ARM only when a WPN category is highlighted.

Verification: PSP build passed; starter/largest-hull capacity and Triangle-inactivity checks pass. All six categories were inspected in native normal and high-contrast captures. Performance remains green. The inherited baseline UI and station-collision failures remain separately documented.

## 2.5.211 — Spacebook creature avatars

Spacebook usernames now generate stable native-pixel profile pictures instead of repeatedly borrowing the general NPC portrait. Ten families cover humans, reptilians, insectoids, robots, aquatic beings, fungi, avians, furry creatures, crystalline life and ship/logo accounts. Username hashing also varies palettes, eyes, expressions, silhouettes, visors, cyber markings, collars and account badges. The same name always produces the same avatar, case-insensitively, with no save-format change, textures, heap allocation or background simulation cost. Only visible feed cards are drawn.

Verification: PSP build passed; username stability and all ten-family coverage checks pass. An 80-account native 480x272 gallery and live two-card Spacebook capture were inspected. Performance suite remains at 0 failures. Full inherited UI suite remains at its existing 38 failures from v2.5.210; no additional failures. Physical PSP validation remains.

## 2.5.210 — Purposeful traffic and Law Scanner

Four deterministic station/world routes replace arbitrary civilian orbiting. Traders service ports and return; Law patrols main lanes and dispatches from the station, while pirates hunt outer routes and flee patrols. Existing freight schedules and survey formations remain. Unengaged Wanted targets survive NPC combat; recent player involvement receives the bounty for a police-assisted takedown.

System Operations: Square toggles the route map. New Law Scanner (UTIL, 480 U, tech 3+) uses Circle + R shoulder to select, then a Circle tap to scan. Its 20 km radar snapshots show 650 m inspection reach, fade and expire after 12 seconds. Tractor and heat-sink assignments are unchanged. Target details and faction lore explain activities. Existing saves load; new module ID 56 needs this executable. No persistent traffic/economy simulation is claimed.

Verification: PSP build and every added traffic/scanner check pass. Full emulator comparison matches 2.5.209's existing failures (1 game, 38 input/UI); steering, radio and performance pass. No new failing checks. Native screens inspected; physical PSP validation remains. See docs/TRAFFIC-ROUTES.md and docs/TRAFFIC-210-VERIFICATION.md for limits and next steps. Repository contains extensive earlier uncommitted work; no automatic commit, push or tag was made.

## 2.5.209 — Select skips Thargoid encounters

Validation: PSP build passed; muted emulator input checks passed all three skip phases, including simultaneous fire and an incoming lethal hit. Checks verify preserved route/fuel/credits/kills, cleared projectiles and no immediate encounter retrigger.

Press Select at any time during a Thargoid encounter, including the opening countdown and reinforcement pauses, to resume the original hyperspace jump. The footer shows the existing Select button graphic with SKIP. Skip takes priority over firing and incoming damage, clears the encounter and battle music state, preserves earned rewards, and grants no additional kills or cash. Saves are unchanged.

## 2.5.208 — Verified battle decoding and 3D hyperspace pursuit

Reproduced the garbled battle music using the actual spacebattle.ogg file in a silent PSP executable: the installed Tremor decoder clipped about 65% of samples and had effectively zero correlation with reference Vorbis output. Replaced that decoder with libvorbisfile/libvorbis, explicitly requesting signed little-endian 16-bit PCM. Fixed the shared resampler's truncated 48 kHz phase step by accumulating exact sample-rate units.

All four user tracks passed eight-second silent PSP emulator decode comparisons. Correlations with reference PCM: 1.000000, 0.999941, 0.999944, 0.999946; zero clipped samples. This validates decoding/resampling, not physical PSP audio timing. Double output buffers, failed-track quarantine and silent battle fallback remain.

The encounter uses shaded 3D Thargoid meshes with bank/yaw/depth motion, perspective dust streaks, passing 3D rocks and cockpit rails. It now releases 45 attackers through staggered reinforcements, with no displayed wave labels or clear banners. Controls and 20U kill rewards are unchanged.

Silent runtime tests passed encounter rewards, defeat/victory route preservation, protected countdown, visible firing, star continuity, bounded formations and 45-enemy completion. A captured encounter frame was inspected. The broader game suite reports one station-tunnelling collision failure outside the changed code; it is not a clean full-suite pass. Hardware frame rate and sustained audio playback still need verification.

## 2.5.207 — Thargoid encounter stability and readability

Replaced the time-reseeded starfield (18 random rearrangements per second) with persistent moving stars; removed full-screen hit flicker and overlapping HUD text. Added a four-second protected briefing, visible player lasers, solid enemy silhouettes, slower staggered bolts and stable attack formations without near-plane teleport damage.

Audio now alternates two static output buffers and gives the Vorbis worker a 64 KiB stack instead of sharing a 16 KiB stack with an 8 KiB output block. Failed tracks are quarantined until restart, chained Vorbis channel/rate metadata is validated, and an empty/unreadable battle library stays silent. OGG/MP3 folder and saves are unchanged.

Validation: PSP compilation and source checks; new countdown, firing, star-continuity and formation regressions compile in the input suite. Emulator remains closed by user preference; regressions and the reported hardware audio fault still need runtime verification with the user's tracks. These checks do not prove audio playback quality.

## 2.5.206 — Native OGG battle music

- Added direct `.ogg`/OGG Vorbis scanning and playback to every custom music source, including `music/Thargoid Battle`; MP3 remains supported.
- Linked the PSP-friendly fixed-point Tremor decoder and reused the existing bounded shuffle/resampling/crossfade path.
- Kept OGG playback independent of PSP MP3 module availability, so an OGG-only library does not load the MP3 decoder.
- Added corrupt-stream retry bounds, case-insensitive extension checks and updated drop-folder instructions. Save/config formats unchanged; emulator and hardware audio remain untested.

## 2.5.205 — Thargoid battle music library

- Added a dedicated `music/Thargoid Battle` MP3 drop folder, scanned and created alongside existing music libraries at startup.
- Crossfades into shuffled battle music when a Thargoid interdiction begins and restores the selected radio source when it ends.
- Keeps battle score independent of the radio power switch while respecting the existing MUSIC volume, including MUSIC 0.
- Uses the generated Pixel Comet action arrangement when no battle MP3 is installed.
- Expanded bounded music-source tables without adding a visible radio station or changing radio preferences.
- Added compile/static checks for folder allocation, encounter wiring, source restoration and fallback behavior. Save format unchanged; emulator and hardware audio remain untested.

## 2.5.204 — System Operations and Thargoid interdictions

- Replaced the shorthand System Details page with a native 480x272 local-map and selected-object dossier.
- Connected the page to real traffic, missions, local law, bounties, station activity, planetary landings, species/site records and stellar-rift logs.
- Added a transparent 78-step system progress calculation and an ordered, gameplay-aware NEXT recommendation.
- Corrected gas-giant details to advertise their implemented floating skyports.
- Kept unknown surface records hidden until first landing; deterministic local time/environment is shown only for visited worlds.
- Added compiled progress/action regressions and a static check rejecting fictional live-condition labels.
- Added a rare three-wave Thargoid rail-shooter interruption during hyperspace, with patterned approaches, enemy bolts, aiming, rapid fire and a dedicated survival meter.
- Thargoid kills pay 20 units immediately. Victory resumes the original jump; defeat restores its departure system and selected destination without consuming fuel.
- Kept the arcade encounter transient and isolated from ordinary flight, docking, targeting and commander save data.
- Added compiled reward/victory/defeat regressions and a static hyperspace-state safety check.
- Save layout and assets unchanged. PSP build/static checks pass; emulator and physical PSP testing remain outstanding.

## 2.5.203 — Larger depth-correct planetary rings

Ringed gas giants now use a four-line band extending roughly 1.82-2.06 planet radii. The rear half renders behind the globe and a brighter near half renders across its foreground, fixing the missing front arc. Seeded thickness/skew adds variation and the complete ring follows cockpit roll. Applied consistently in normal flight, visible hyperspace and station departure views.

No save or external asset changes. PSP build and static ring-order checks pass; emulator remains closed. See docs/PLANET-RINGS.md.

## 2.5.202 — Closed station tunnels and varied orbital architecture

Station flight tunnels now end at an opaque, lit pressure door, so stars, planets and traffic behind a station are no longer visible through its far side. Main hubs, outer relays and frontier outposts all retain the same safe 140x64 flight slit.

Every system now derives station radius, depth, spin, hull/trim/light colours, bands, pods and one of six structural families from its identity. Main hubs vary by more than 2x in width and depth; secondary hubs receive independent profiles. Rendering, docking guidance and swept collision share the generated dimensions.

No save or external asset changes. PSP build and static checks covering all 768 hubs pass; runtime fixtures compile but were not executed. Emulator remains closed. See docs/STATION-EXTERIORS.md.

## 2.5.201 — Sky Observatory: The Signal Between

Implemented the observatory investigation at existing generated sites: telescope, console and log clues persist; a named observer explains an irreversible confirmed choice between public-beacon reset (+20U bonus) and preserved trace (+1 extra discovery), in addition to the normal site reward. Outcomes alter return dialogue, instrument display and the Codex field-site report. Existing completed sites remain legacy-complete and cannot pay again. Lave 1 has no observatory; Lave 2 POI 3 and Lave 4 POI 5 do.

Refined native 340x168 observatory art, protected 14,146-pixel window mask with local planet/biome/time rendering, animated receiver display and a quiet volume-controlled receiver sound. No resized concept image. Static generator checks enumerate 581 observatories across 1,024 worlds with no duplicate per world.

Save V26 uses reserved surface bits 20-24 for choice and clues, no payload growth. V25 and earlier remain loadable; older executables cannot read V26. Back up commander files before upgrading. Added core/input/save/legacy fixtures; compiled but NOT executed. PSP build and static checks pass; physical PSP and runtime visuals/controller/audio/performance remain unverified, emulator kept closed. See docs/OBSERVATORY-ENCOUNTER.md.

## 2.5.200 — Planetary site scene foundation

All planetary sites now open station-style single-location scenes. Added sixteen native 340x168 pixel layouts with shared hotspot anchors, planet/time palette variation, site-specific inspection and NPC conversation text. Existing objectives/rewards and save completion are preserved; entry/exit input is isolated and outdoor time pauses. This is a working foundation, not the complete art/36-encounter milestone. See docs/PLANET-SITE-SCENES.md for changes, verification and remaining work. PSP build/static checks only; runtime fixtures compiled, not executed. Emulator remains closed.

## 2.5.199 — Expanded equipment catalogue

Added 30 modules (54 installable total): six distinct primary weapons, six defences, five navigation tools, three cargo bays, four fuel-system modules and six utilities. Includes +10% firepower, hotter +20% overdrive, shield disruption, shield-piercing plasma, shot suppression, armour and specialised damage protection. Passive effects support existing hull slot capacities; stronger scanner/shield/scoop/repair tiers win, distinct cargo bonuses sum. Stock follows system tech/economy/prosperity. Full catalogue and stacking notes: docs/EQUIPMENT-CATALOGUE.md.

V25 saves expand accepted module IDs with unchanged V24 payload size; duplicate validation now uses a byte table instead of an overflowing 32-bit mask. Existing saves remain loadable; back up before saving with this build, as older builds cannot read V25. Weapon effects remain hitscan with differentiated visual beams, not physical projectiles.

Verification: PSP build and static catalogue/layout/256-system stock checks. Added compiled effect/save regressions; runtime tests, balance and native PSP visuals NOT verified. Emulator remains closed. Next: execute regression suite with permission, check specialist weapon effects and module loadouts on hardware. No new external assets or published release.

## 2.5.198 — Mechanics service name

Renamed SHIP > Engineers to Mechanics, including the service header, fee label and tutorial directions. Repairs/refuelling behavior is unchanged. Includes the v2.5.197 fix for duplicate Loadout feedback and HOLD/HULL overlap. PSP build checked; emulator kept closed. No save or asset changes.

## 2.5.197 — Single unobstructed Loadout feedback

Loadout now owns its feedback: the generic menu_notice overlay skips INVENTORY. HOLD/HULL stays at y=224; arm/sale feedback uses y=236, below the stats and above the y=248 footer. Removed both the duplicate global notice and same-baseline local overdraw. Updated selected-label fixture coordinates for the module-bank layout and added framebuffer regressions for unchanged stats and no generic duplicate after Square. PSP build checked; runtime fixtures compiled but not executed, emulator kept closed. No save/assets changes.

## 2.5.196 — Refuelling separated from Outfitting

Removed fuel purchases from every Outfitting stock list and blocked direct catalogue refuel transactions. Fuel scoops remain installable modules. Paid refuelling is now a separate Triangle action in SHIP > Engineers, with current/maximum fuel and a fuel-only price; X still repairs without buying fuel. Same missing-fuel pricing, dock/full-tank/funds guards. Updated menu hint, tutorial text/action and transaction fixtures. Empty low-tech stock pages now have a safe no-stock view (no zero-length indexing/division). Save format and assets unchanged. PSP build checked; runtime fixtures updated/compiled, not run; emulator remains closed.

## 2.5.195 — Station exploration balance

Added a right-aligned live currency balance to the top-right header of every point-and-click station room, including shop, conversation and reading views. Uses the game's tenths-of-a-unit precision, updates from current credits each frame, and fits the maximum validated balance without overlapping the room identity or option rail. No save, controls or asset changes. PSP build and width checks only; emulator remains closed.

## 2.5.194 — Ship-specific module banks

Ship Loadout is now a six-row tech board with up to four individual slots per category. D-pad Up/Down selects category; Left/Right selects a slot. Locked slots are visible but cannot be selected. Square arms a fitted WPN laser, including while paused in flight; the active weapon has a green underline. X sells the selected module at a station with confirmation; Triangle opens Outfitting for that exact slot. Ordinary Outfitting purchases fill a free compatible slot first, falling back to a confirmed replacement when full.

Price progression gives Adder 6, Gecko 8, Moray 11, Cobra Mk1 12, Cobra Mk3 15, Fer-de-Lance 16, Krait 18, Python 19, Ophidian 21 and Anaconda 22 total slots. Shipyard lists category capacities. Different modules coexist; duplicate catalogue items cannot be bought twice. Cargo Bay, Freight Rack and exclusive clamp add +8/+16/+8 tonnes together, alongside a passenger cabin. Passive capabilities combine; strongest shield/scoop tier wins rather than stacking rates. ECM/chaff and recharge now search all slots. Station clamp gifts use free HOLD slots.

Four laser choices: Pulse 24, Beam 36, Mining 18 versus ships / 54 versus rocks, and new Heavy 60 damage with 0.36s cycle and 22 heat (others 0.18s and 12 heat). Heavy costs 1,400 U, displayed tech 10+, industrial economies 0–2. Only the armed laser fires/contributes mining mode. Ship exchanges repack modules, retain the active laser and reject insufficient slot/cargo capacity before charging; occupied cabins and loaded holds remain protected.

V24 appends 20 bytes (18 extra module slots, active index, reserved zero) under existing CRC/backup validation. V23 and older fits migrate to bank one. Invalid/duplicate/wrong-category/locked-slot records and invalid active indices are rejected. Back up saves before upgrading: older builds cannot read V24. No added art/audio assets.

Verification: PSP builds and tools/check-module-banks.mjs static catalogue, capacity/progression, layout and wiring checks. Runtime fixtures added for extra-slot effects, four weapons, install/sell/arm controls, cargo/cabin protection, hull transfer, V24 roundtrip and V23 migration; legacy migration offsets updated. Fixtures compiled, NOT executed; emulator deliberately kept closed. Next: authorised runtime regression/visual checks and physical PSP testing. Local only, no push/release.

## 2.5.193 — PIMP-MY-SHIP.NET menu branding

Renamed the SHIP category's Ship decorator entry to PIMP-MY-SHIP.NET, including its detail heading. Native lettering uses cyan, pink and gold word accents with a subtle fixed one-pixel stagger. Selection highlight and cursor are unchanged; high-contrast mode uses straight, theme-readable text. No extra assets, gameplay changes or save changes. Build verification only; emulator remains closed.

## 2.5.192 — Replies go straight to the contact

Removed the main-story chapter 2+ commander echo state, its extra confirmation press and its upper player speech/portrait. Selecting a bottom reply now advances immediately to the next authored NPC answer. Final acceptance remains explicit and occurs only at the final beat. Prologue, guild, police, comms and station conversation paths were inspected for player-echo rendering; the remaining active echo path was in saga briefings. Shared dialogue rendering no longer has a special commander portrait branch.

Updated input and visual fixtures, including every saga chapter and each question/answer beat. PSP build checked; fixtures compiled but not executed because the emulator is kept closed. Runtime and hardware verification remain outstanding. No save format or content assets changed; existing chapter progress is preserved. Local-only delivery.

## 2.5.191 — Flight message spacing

Moved the body text in top-of-screen flight captions down exactly two pixels. Increased the caption backing by two pixels to retain all three text rows. Speaker labels, controls, wrapping width and dialogue behavior are unchanged. Save format V23 unchanged; no new assets. PSP build validation only; emulator kept closed, visual PSP check remains outstanding.

## 2.5.190 — Planet artwork follows flight roll

Fixed upright planet billboards during barrel rolls: flight now inverse-samples the planet artwork using the same screen-space roll as the existing camera projection. Surface features and baked shading rotate together; planet centres already followed camera roll. Menus, chart illustrations and HUD remain unchanged. Zero/full roll uses the existing fast path; rotated rendering clips to the viewport and uses a 16 KiB static tinted-sheet cache, with no heap allocations or per-pixel trigonometry. Save format remains V23; no additional assets.

Verification: PSP build succeeded. Headless mathematical/source checks in tools/check-planet-roll.mjs passed camera orientation, both roll directions, quarter/half/full turns and clipped near/far sampling (2,641,244 visible samples). These are not actual PSP render tests. Emulator deliberately left closed. Next: visual continuity and frame-rate check on PSP, especially rolling beside a large planet. Existing unrelated changes preserved; local-only delivery, no sync or release.

## 2.5.189 — One simple Spacebook history

One newest-first feed follows the commander across every system, retaining up to 4,096 posts with their original location, date and Like/Dislike reaction. Two full-width cards, a scrollbar, Up/Down selection and Left/Right jumps of ten posts keep navigation simple. No reply feature or extra feed menus. Profile names display naturally (BEN becomes Ben) without changing the saved name.

160 unique templates across 20 event types mix genuine thanks, gossip, dry humour, complaints, sarcasm, insults and indifference. Consecutive posts of the same event type avoid identical variants. Existing event hooks remain; posts do not invent completed actions. Return gossip now requires a day away. Ambient chatter is bounded to avoid swamping recent activity.

V23 merges existing V22 system histories into the global archive, preserving dates, authors, wording variants and reactions. Back up saves before upgrading: older builds cannot read V23. History is saved with manual commander checkpoints, not separately autosaved. No additional assets required.

Verification: PSP compilation and static checks only for this version. The 160 templates pass 800 actual-font/name layout checks. Runtime save migration, controller handling and visual checks remain outstanding; no emulator was launched at the user's request.

## 2.5.188 — Spacebook commander names and reactions only

Reactive posts now name the current commander profile (for example BEN), not the historical ship model. All 57 player-event variants use natural person-based wording; ambient posts remain unrelated local chatter. Existing V22 posts adopt the profile name immediately, and renaming the commander updates their displayed name throughout the feed. Stored dates, authors, event history and reactions are unchanged. No save migration.

Removed Spacebook's Local replies text, Triangle footer prompt, reply overlay and Triangle input action. X Like / Square Dislike remain. Inbox is unchanged. Tutorial guidance now describes Like/Dislike. Regression coverage checks BEN in every player-event variant, 24-character name fit, and Triangle no-op on Spacebook.

## 2.5.187 — Local Spacebook history

Spacebook now prepends a bounded history of 16 reactive posts per system, ahead of the seven existing community/story wires. Twenty event categories have three prose variants each, with generated spacey handles. Events capture the ship model at the time; viewing a card never rerolls its author/text. Actual successful custody, fines, fleeing, player ship kills (pirates distinguished from other ships), station departures/docking, paint purchase, ship purchase, planet landing, flora/fauna/rift scans, salvage, non-smuggling contract completion and completed hyperspace travel produce local posts. Routine same-type posts throttle for 90 seconds. Ambient humour is eligible every four minutes of active simulation, capped at three retained ambient posts so idling cannot erase the entire activity history. No combat RNG is consumed.

X toggles Like; Square toggles Dislike. Switching replaces the previous reaction; pressing the same button clears it. Reactions travel with posts as new cards push older ones down. Seven legacy wire reactions persist separately. Tutorial evidence remains first, and Messages retains its separate inbox.

PSP RTC dates are saved as UTC and shown YYYY-MM-DD HH:MM UTC. Clock-unavailable records are explicitly labelled, never given invented calendar dates. Returning after 3+ minutes away (including loading after a real-world absence) adds a wondering-about-the-ship post followed by a welcome-back sighting, both dated when actually generated. No fabricated offline events/backdating. Changed/backward clocks do not underflow cooldown/absence arithmetic. Date accuracy depends on the PSP clock.

V22 appends 34,824 bytes for 256 bounded feeds, timestamps, local reactions and metadata. Older saves load with empty reactive history, not reconstructed memories. Older builds cannot read V22: back up saves. History is part of manual commander checkpoints, not a separate unlimited/autosaved journal. Only the newest 16 reactive records per system survive rollover. Native rendering, no external art/audio/network dependencies. Main thread stack explicitly 1 MiB for bounded Game snapshots/migration/test fixtures; 8 MiB heap unchanged.

## 2.5.186 — Living stellar rifts and instrument reports

Replaced WORM meshes and fixed close-range circles with layered, additive cosmic fields: eight inclined filaments, bright knots, drifting dust and breathing cores. Four stable profiles (Aurora Veil, Gravity Lace, Ember Nursery and Meridian Echo) share their names, palettes, reports and lore across targeting, the flight view and Codex. Fields remain one anomaly ID each. Drawing has bounded loops, close-up size limits, view clipping, distance culling and a low-glow high-contrast mode. Existing mystery/scan mission hooks remain.

Triangle analysis opens a paginated Ship Computer report with existing babble audio; Triangle/Circle closes, L/R reads. No human reply choices and no invented teleportation, mining or gravity mechanics: reports separate survey observations from folklore. Tutorial briefings wait until the report closes.

V21 adds 256 bytes of per-system four-bit rift identity masks under the existing CRC and atomic save validation. Revisit/load restores scanned status; repeated scans reopen the report without another discovery/reward. Older V20 and earlier saves import with unlocated historic totals, not invented locations. Back up saves: older EBOOT versions cannot read V21. Space Signals in the new Codex now lists actual logged field identities and opens illustrated reports. No external art/audio assets required.

## 2.5.185 — First-person station departures

Player-facing Launch, narrative Launch, docked wanted pursuit and docked chart departure now enter a five-second first-person sequence. The ship starts inside its own berth, accelerates through an illuminated tunnel aperture, bursts clear and eases to 100 m/s before returning control. Geometry uses a continuous analytic speed/distance curve. Relay departures retain their original hub instead of teleporting to the primary. Arrival docking is unchanged. Raw launch remains the low-level live-space initialiser used by isolated simulation fixtures; all player launch paths use launch_departure.

The actual system sun now lies beyond the primary port's outward (-Z) axis, at a safe 130-142 km distance; a narrow clear sightline is reserved without clustering the other planets. Existing sun rendering/flaring is used, with a wider departure flare that respects high-contrast suppression. Tunnel masks hide external space beyond the mouth until it clears the canopy. Existing docking and boost cues accompany the launch. No external art or save-format change.

Inputs cannot skip or steer the sequence; held buttons must be released before fresh actions can fire. Docked chart jumps queue until departure completes. Launch guidance consumes no boost fuel and bypasses collisions only during the controlled corridor traversal.

## 2.5.184 — Hierarchical Discovery Star Atlas

Discovery Codex now opens a visited-system atlas with a star-map locator, seven-row windows, scrollbars and shoulder-button paging. System directories contain charted station/star records and only landed worlds. Illustrated planet dossiers show shared world type, settlement/weather profile, and four selectable category tiles: Flora, Fauna, Minerals and completed Field Sites. Collections enumerate actual per-world saved IDs, not commander-wide counts; individual dossiers use the same animated species sprites/names/traits as the surface. Breadcrumbs and an eight-frame bounded navigation stack restore parent selections at each Back. Empty collections explain how to fill them and cannot open invented records.

Removed fabricated mineral/echo location lists and seeded discovery totals. Space Signals explicitly explains the current limitation: space scan totals have no saved per-system identity and cannot be retrospectively located. Charted station/star entries are labelled as chart knowledge, not claimed visits. Galactic Lore remains separate and unchanged.

The present engine supports 256 systems x 4 landable planets = 1,024 worlds, with 8 field slots per world (8,192 records). Browser providers enumerate only the selected branch; no galaxy-sized UI allocation. New categories/levels can extend the provider and bounded navigation model. This is not unlimited engine/storage expansion and adds no new species. No save-format change (still V20), no external art dependency.

## 2.5.183 — Ship tools / tractor recovery and truthful Law stops

Circle+Left now equips TRACTOR in the compact Ship Tools selector. Release, then tap Circle: prefer the selected recoverable object within 500 m, otherwise find the nearest visible recoverable object in range; smoothly auto-turn before running the existing beam animation and inventory transfer. Rocks, distant/occluded/dead objects cannot be collected. Manual steering, changing target, leaving flight or a police stop cancels pending alignment. Menu-Back suppression is preserved. Missiles, flares and heat sinks retain their directions. To keep the displaced fitted ECM useful, it automatically pulses against close incoming missiles, still costing 18 shield energy with an 18-second cooldown.

Law now records civilian/police assault and destruction, contraband, refusal and fleeing independently per system. Dialogue names the actual offence, includes a paginated charge list and only mentions goods aboard when present. Fine replies and receipts no longer claim an empty hold was confiscated. A clean scan cannot erase an existing warrant. Surrender removes only cargo-attributed charges, including after saving or revisiting a system; violence survives. Existing yellow reply selection is preserved; L/R shoulder buttons read longer testimony.

V20 appends 1,024 bytes of offence/cargo-charge records under the existing checksum and atomic save validation. Older saves import; their unknown incident details are explicitly described as unavailable rather than invented. Back up saves before upgrading: older EBOOT versions cannot read V20 saves. Art/audio remain embedded; only EBOOT replacement needed.

## 2.5.182 — Station reading and glowing focus

Fixed station option highlight/text alignment by drawing both at exact pixel coordinates. Controls now occupy the bottom 16 pixels. Room entry shows persistent room prose; hovering only changes visual focus. Cross explicitly inspects or speaks. Long descriptions and conversations are word-wrapped into six-line pages, navigated with Left/Right without triggering actions. Replies use the right rail and retain yellow selection, leaving the lower panel for full-width speech. Expanded room, landmark, prop and crew writing, with authored responses for each Lave activity step. Selected people, props and doors receive a clipped, softly pulsing cyan/cream outline with bright corners, including high-contrast support. No new save format or art files.

## 2.5.181 — Lave / Berth Six station

Seven native 340×168 raster rooms with a shared 32-colour kit, transparent crew/prop atlases, deterministic economy/seed variations and shared visual/hotspot anchors. Lave primary has a named cast, a cross-room missing medical-manifest activity (60 U), a Lave I survey follow-up (90 U), station service links, yellow reply selection, ambient machinery and a canteen jukebox using the existing procedural music channel. Reorte's Second Shift/Arrivals and the tutorial's original contacts remain intact.

V19 adds 3,072 bytes of per-system/per-hub activity state; earlier saves import. Generic cargo progress no longer resets on visiting another hub, and survey tips require local scan records and cannot be repeatedly farmed. All artwork is embedded: replace EBOOT.PBP only, keeping saves/config/music. Back up saves before upgrading; older builds cannot read V19 saves.

## 2.5.180 — Combat bearings, missile launches and space effects

Thin camera-relative indicators show actual recent incoming fire (double chevrons for rear sources), including freighter guns and incoming missiles. Player missiles emerge forward, coast briefly, then turn with a bounded rate and a short world-space exhaust trail; swept collision preserves fast hits. Distant ship dots gain faction-tinted rear trails. Sun-facing/nearby lens flares grow, and close solar exposure rapidly heats and damages shields/hull without a false combat alert. Enlarged planets reuse sample/tint calculations instead of repeating them per pixel. No save-format or asset changes.

## 2.5.179 — Faction almanac

Rebuilt Factions as four illustrated dossiers with Identity, In Play and Channel pages. Full paragraphs explain actual gameplay, with crew sayings, practical menu links and preserved story channel notes. Local living-ship totals are labelled LOCAL SNAPSHOT rather than faction strength. X cycles pages, Up/Down selects faction, Triangle preserves nearest-contact selection. No new faction mechanics or save changes.

## 2.5.178 — GalacticNet scrollbars and clearer outfitting

This build also advances ordinary NPC positions/collisions every frame (AI decisions remain staggered), eases lock-on alignment near its bearing, reduces boost-only speed heat (coefficient 10 to 0.35; normal overspeed, solar heat and ENG cooling unchanged), and tightens the Weapon Computer from 352x150 to 272x120 with dark amber combat styling. Added motion-per-frame, lock easing and boost endurance regressions.

Spacebook, Inbox and Jobs now show a scrollbar matched to the visible cards, including partial final pages. Galactic Lore keeps SECTOR when selected. Outfitting uses STATS: and COSTS:, removes ONE MODULE PER SLOT, and names the destination slot beside empty/replaced modules. Missiles now accept any living ship target, not just hostiles, while retaining range and existing legal consequences. The bottom Circle hint includes missile/flare stock. Prices, fitting rules and saves are unchanged.

## 2.5.177 — Select weapons, then tap to fire

Hold Circle for the Weapon Computer; a direction equips and closes it. Release safely, then tap Circle to activate the selected weapon/tool. The bottom hint names the selection. Menu Back cannot leak into the weapon gesture. Triangle on a station opens Comms with REQUEST AUTO-DOCK; confirm that option to begin guidance. FLY Disembark is hidden when undocked. Updated all relevant control instructions; see docs/UPDATE-2.5.177.md.

## 2.5.176 — Circle tools / Triangle interactions

Hold Circle for the secondary-tools D-pad: Up missile, Down rear decoy, Left fitted ECM, Right fitted heat sink. Circle alone is a harmless status preview. Manual tools have resources/cooldowns; automatic ECM/heat-sink triggers removed. Triangle now handles selected cargo, anomalies, station docking and planetary landing requests as well as NPC hails. Landing still requires confirmation and cutscenes remain input-locked. Mapping is easy to revise. See docs/UPDATE-2.5.176.md for balance and verification.

## 2.5.166 — Clear power-bank selection and eight extra themes

## 2.5.175 — Flight control labels

Hold-Square target computer footer now shows shoulder-button icons with : NEXT and : LOCK. Normal flight Triangle label is COMMS, without HOLD:. Missile count moves from the bottom control pane to a missile silhouette and number beside WEP, with tighter ENG/WEP spacing. Ammunition and controls are unchanged.

## 2.5.174 — Ship traffic and combat steering

Light ships now anticipate nearby traffic, separate overlapping hulls and fly close-range breakaway legs instead of slowing into one another. Exact-opposite steering works reliably, and weapons remain aimed at the actual opponent during avoidance. No extra assets or save changes. See docs/UPDATE-2.5.174.md.

## 2.5.173 — Restored planet cutscenes and exploration polish

Unskippable 3D landing/departure cutscenes are back, with a separate camera that never changes live player state. Landing automatically disembarks; Triangle beside the ship boards, and a fresh R press on the parked screen launches. Wall-mounted GARAGE/POI lettering, distant sky traffic, smooth Square + R tracking and a compact icon-based HUD complete the update. See docs/UPDATE-2.5.173.md. No new assets or save changes; the reported physical PSP shutdown still needs hardware verification.

## 2.5.172 — Explicit planetary transfers

Confirm landing with X at planetary approach, then wait: guidance lands at the pad and automatically puts you on foot. Landing/departure screens do not accept skip buttons. Triangle boards only beside the ship; the parked-ship screen then offers R to launch or X to step outside. Release all buttons before either action. Circle no longer toggles boarding/disembarking. Atmospheric Circle opens a separate landing confirmation.

The transfer renderer no longer temporarily changes the live camera/surface mode. Boarding and disembarking have separate validated model functions; exit coordinates are checked before committing on-foot state. Landing diagnostics are written to landing-trace.txt in the game folder (bounded, no per-frame writes after first-frame/tick checkpoints). Save layout and assets are unchanged. Physical PSP crash resolution still requires hardware verification.

## 2.5.171 — Landing message and cutscene input safety

Triangle CLOSE on atmosphere/pad messages only dismisses the notice; it cannot also return to orbit or take off. Circle/Triangle skipping arrival now consumes that press. Circle during pad alignment finishes the landing and disembarks directly, without running flight controls again. Invalid/stale cinematic states are discarded, and cinematic mesh indices are bounded. Boarding still supports one Triangle launch; its message now explicitly says LAUNCH, not CLOSE.

Regression checks cover natural/skipped arrivals, held buttons, pad messages, disembarking, repeated boarding/departure, tutorial landing and guarded rendering across every purchasable hull and all four Lave bodies. Physical PSP shutdown reproduction remains unverified.

## 2.5.170 — Radio now-playing label

Deep Space FM now displays NOW PLAYING: when tuned to a channel. STATIC during station switching, its duration, station names and RADIO OFF behavior are unchanged.

## 2.5.169 — Outfitting and connected mission boards (local build)

- Station stock varies by technology, economy, prosperity and primary/secondary hub. All 25 catalogue entries have accurate instructions, slot labels and effects; exclusive Chandler stock is purchasable.
- Two-column Outfitting shows actual replacement stats, trade-in, net cost/refund and confirmation. Triangle links Outfitting with Ship Loadout. Cargo capacity and occupied passenger cabins are protected.
- Heat Buffer now fits and survives save/load; Auto Repair repairs hull, the escape pod catches fatal damage, the mining cutter is stronger against rocks, and scanner identification respects its advertised range. Nav Beacon marks the selected jump destination.
- Mission boards offer five varied contract types in every system, with hull-reachable destinations, illustrated client cards, pay, risk, brief and exact objectives.
- Board jobs no longer expire. Old duration fields remain only for save compatibility; no countdown is displayed. Accepting a job tracks it; selecting an accepted card opens Tracked Mission. The Log retains navigation and abandonment.
- Added all-system contract acceptance/completion/payment checks, station-stock sweeps, module transaction/persistence/effect tests and native-size UI captures.
- Save layout remains V18. Back up saves before upgrading; older builds cannot safely read a saved Heat Buffer fitting. Physical PSP verification remains required.

## 2.5.168 — Police choices, cargo surrender and custody roleplay

- Police replies now highlight the actual police selection while preserving normal menu focus.
- A fourth, paged surrender reply appears only when restricted cargo is present.
- Voluntary surrender waives only the current scan's cargo charge. Existing violence, evasion and other offences remain, including at the warrant cap.
- Submitting to a scan identifies cargo without removing the chance to surrender it voluntarily. Forced inspection still confiscates cargo and adds refusal charges.
- Paid and no-funds custody both show a shuttle transfer and detention review, followed by explicit release papers.
- Release papers explain fees/confiscation and the return to a station berth; play resumes only after X acknowledgement.
- Custody's no-funds property-seizure consequence is shown before selection.
- Game, input and native highlight/custody-capture regressions added. Save format remains V18.

## 2.5.167 — Unified Comms, ship catalogue and fuller newspapers

- Removed Commander Display & chatter; its six settings/actions now live in the shared FLY / hold-Triangle Comms panel.
- Updated tutorial routing and menu regressions; the station-walk tutorial gate now follows its actual option. Existing encounter reply flows remain separate.
- Salvage now requires an undocked ship and a live nearby anomaly, preventing entry from a docked menu.
- Ten individual Shipyard descriptions, full-width reading area and separate balance line.
- Up to four recessed hull panels on close ships and menu previews; original silhouettes/collision dimensions unchanged, distant detail disabled.
- Newspaper lower sections with eight native-pixel commercial illustrations, adverts, classified notices, letters and oddities. Existing stories and Orbit Sudoku retained.
- Left-side Wanted selection scrollbar across all five system warrants; tracking and takedown logic unchanged.
- No commander-save format change.

- Theme-independent filled SYS/ENG/WEP selection with dark text/pips and explicit bank name.
- Hold Start temporarily reveals power controls in minimal/scenic HUD without changing the saved HUD preference.
- Correct pixel alignment between bank labels, highlights and pip bars.
- Eight new finishes and matching UI palettes, bringing the total to 16; paged list/swatches and shared price data.
- Expanded palette contrast, purchase, settings round-trip and power-highlight regression coverage.

## 2.5.165 — Decorator label alignment

- PIMP-MY-SHIP.NET moved 10 pixels right and 3 down.
- PAINT FINISHES moved 3 pixels right and 5 down.
- Bottom-right units moved 10 pixels right and 3 down.
- Pixel-positioned text only; paint selection, prices and gameplay unchanged.

## 2.5.164 — Wider systems, longer traffic lanes and luminous suns

- Seeded planets on separated bearings, with over 30 km clear body-to-body spacing checked across the galaxy.
- Full-circle warp-entry positions and independent headings; no automatic station-facing arrival. Main launch/docking corridor preserved.
- Secondary hubs farther out; capital routes connect station berths to planetary transfer space. Ordinary traders shuttle between hub and worlds; explorer waypoints no longer rotate faster than the ships can reach them.
- Large-hull visibility to 55 km and longer engine trails anchored behind ships.
- Eight animated warm sun families with luminous centres and soft corona; clipped previews.
- Added all-system spacing/arrival/route checks plus sun brightness/animation/clipping and warp-view captures. V18 saves unchanged.

## 2.5.163 — Fresh departures, pad alignment and compass identities

- Independent arrival, pad descent and departure cameras. Boarding clears stale arrival state; one Triangle launches and automatically returns to orbit, even with a computer message active.
- Centre valid landings on the pad; place rover in a signed, collidable nearby garage.
- Tap R to jump; hold R to run. Long-hold release and targeting gestures cannot trigger a jump.
- Ship/rover compass pictograms; consistent 1–9 POI labels across compass, nav computer, field guide and building signs.
- Fix close-range wall clipping/depth and building culling; add repeat-cycle, gesture, label and rendering regressions.
- V18 saves unchanged. Local build only; physical PSP QA remains outstanding.

## 2.5.162 — Short jump, left visor and larger field sites

- Left-side exploration computer preserves the centre/right world view. R faces the selected contact; labelled distance tracking persists after closing.
- Larger varied field-site buildings with windows, entrances, wall signs and matching collision footprints. Scenery is cleared from the enlarged sites.

- Replaced held-R thrust with one grounded jump impulse and gravity.
- Mid-air presses add no lift; holding through landing cannot auto-hop. Release and press again on the ground to jump.
- Added physics and real input regressions for long holds and double-tap-plus-hold; no save format changes.

## 2.5.161 — Planetary visor, living ports and surface depth

- Hold-Square exploration targeting, persistent species records, field guide and contextual X interaction.
- Deterministic identities, eight animated species slots and eight field activities per world; three port contract patterns. Gas giant skyports.
- Large exterior spaceports with tower/hangars and continuous staggered ambient landing/takeoff traffic.
- Shared surface depth, slope-supported rover wheels, solid hull/structure/tree footprints and substepped movement. Safe vehicle exits.
- Saved local day/night clock, world-positioned sun, sunset colours, drifting translucent clouds and night stars.
- Save V18 clock extension; older saves remain readable. Procedural/template-driven bounded worlds, not bespoke whole-planet terrain or a persistent traffic simulation.
- Added targeting/depth/clock tests, all-world seed/site checks, all-hull exit checks and native surface captures. Physical PSP visual/performance testing remains outstanding.

## 2.5.160 — Commander files and planetary field trips

- Three manual commander files, keeping legacy commander.sav as slot 1. Main-menu load/delete cards show name, portrait, units, system and ship. Tutorial has a separate title-screen box and file.
- SAVE/STATUS provides slot selection, docked checkpoint saving, loading, rename keyboard, eight male/female portrait choices and local fine payment. Overwrite, load and permanent deletion require confirmation. Validated saves retain backup recovery.
- V16 identity/bounty and V17 planetary progress extensions preserve completed Wanted posters, local surface activities and scanned life. Earlier saves import with the default JAMESON profile. Current-format saves require this build or newer.
- Replaced distant house silhouettes with seeded mountain ridges and camera-relative clouds. Ground near-plane coverage now retains tiles crossing the camera; collision and visuals still use the same triangular height field.
- Rocky-world exploration radius expanded to 1400m, with seeded wilderness props around the player. Ocean expeditions remain on their dry island. A ground-following rover is available at the port; X boards/parks it. On foot R remains the jetpack; vehicle mode does not fly.
- Top bearing strip and L+Triangle POI cycling, with Triangle always facing the ship. Square surveys or interacts at spaceport, relay, supply cache, ruins, observatory and rescue beacon. Relay work pays on return to port; one-time rewards and scans persist per world.
- Lower sustained exposure rate permits excursions; rover cabin and port provide shelter. Service hut/site footprints block walking through them. This is a bounded exploration zone with a small port, not a seamless planetary/town-interior simulation.
- Added slot/profile/delete/recovery/vehicle/activity tests and native captures. Increased smoke runner timeout for expanded checksum/migration coverage; performance assertions are unchanged. Physical PSP validation remains outstanding.

## 2.5.159 — Computer notices and quiet-watch conversations

- Computer notices can only be dismissed, never answered as encounters. Reply controls now require an active human transmission with no computer notice in front of it; stale encounter flags cannot expose Talk/Ignore.
- Three optional, authored civilian conversations: a retired copilot, a remembered greenhouse and a parent's engine-room wisdom. Contextual replies, complete endings and the standard speech/reply panel, with paging for longer text.
- First hail after at least eight eligible peaceful-flight minutes; subsequent hails have a ten-to-fifteen-minute cooldown. Actual nearby idle traders only; suppressed during tutorials, docking, travel and danger. Ignored hails expire. No new save fields.
- Optional cargo offers reuse the existing trader exchange with explicit agreement, range checks and cargo validation. These are trade opportunities, not new missions.
- Added notice/input/story/trade regression checks and native dialogue captures. Physical PSP validation remains outstanding.

## 2.5.158 — Ship paint and coordinated cockpit themes

- The regular command-deck ship window now shades the fitted hull colour instead of fixed bronze.
- Eight curated palettes connect paint to menu panels, focus, borders, readable text, conversation chrome and flight HUD. Warning/faction/world colours remain semantic.
- Each hull's paint persists in validated ship-paint.cfg preferences with backup recovery; failed storage cancels the paint charge. Commander save format unchanged.
- Added palette contrast, per-finish menu preview and preferences reload checks plus eight menu/HUD capture pairs.

## 2.5.157 — Wanted pursuit and Galactic Gazette

- Corrected unsigned pirate placement and unsafe extreme-line bounds; protected Wanted identities during traffic spawning.
- Poster selection uses validated live targets, normal cockpit flight, matching names and risk-aware condition bars.
- Added all-system bounty coverage and post-selection flight/render/fire/boost checks. Physical PSP confirmation still required.
- Sixteen readable newspaper articles with full-width wrapping, left scroll position, stories, jokes, letters and a properly aligned orbital puzzle.
- Rebuilt Wanted cards with paper grain, folds, torn corners, stains and contained status text.

## 2.5.156 — SPACE TALK and readable radio tuning

- Moved LOCKED/STATIC/RADIO OFF below the frequency numbers and expanded the dial glass so title and category have separate rows.
- Renamed station 6 to SPACE TALK, category TALK RADIO, without changing station IDs, folders or saved preferences.
- Replaced the repeating ~1.5-second babble loop and noisy, asymmetric throat pulse with integer-only varied speech phrases: six speaker profiles, different syllable lengths/vowels/pitch contours and genuinely silent pauses. No continuous noise bed.
- Added audio checks for variation, silence, smooth edges, deterministic reset and mix headroom; native locked/static/off radio captures and a 32-second station audition.

## 2.5.155 — Roll while boosting; quieter heat feedback

- Hold R after double-tap boost, then add L + Left/Right to roll without cutting boost. L does not slow or trigger hard brake while boosting; releasing L restores turning and releasing R ends boost. Pitch remains available during boosted rolls.
- Thermal shield/hull damage keeps damage effects but no longer marks the ship as attacked. Heat alone does not produce RED ALERT; genuine combat, missiles and collisions still do. Damage rates and repair costs are unchanged.
- Updated Controls help and added regression checks for boost acceleration/roll direction, modifier press/release order, hard-brake isolation, shield drain/shake and real-alert preservation. No save changes.

## 2.5.154 — Stable steering after docking cancellation

- Replaced world-axis/Euler steering with full cockpit-frame rotation in space and atmospheric flight. Banks, inverted flight and pole crossings no longer swap or reverse steering directions.
- Circle cancellation clears exterior docking phase/timing and braking while preserving position and view. Launch resets yaw, pitch and roll; Debug Return to station also clears docking, approach and auto-aim state.
- Removed the hidden roll reset when boost ends, preventing an uncommanded change of cockpit bank.
- Replaced an incorrect yaw-sign regression with camera-space direction checks. Added all four steering directions across banks/poles, repeated loops, every exterior docking leg, debug return/relaunch and boost-release coverage. Existing save formats are unchanged.

## 2.5.153 — Mission Log owns tracking; clearer mission instructions

- Removed the duplicate Explorers Guild entry from Work. Choose Guild, main story or a contract in Mission Log; Tracked Mission shows that selection's current step and working actions.
- Retired the legacy optional flight guide. First Flight now explains launch, targeting, practice distance, Circle docking and claiming the reward directly, keeping orange reply boxes. No separate docking-guidance page detour.
- Expanded Kei's first-flight, later main-story and aftermath dialogue into full sentences; Guild instructions now respond to launch, accepted-contract and reward state.
- Station Welcome is explicitly selectable in Mission Log and no longer overrides another tracked mission or plots an unrelated route. All five contracts plus both campaigns and the welcome mission fit in the log.
- Both X tracking and Select next-step synchronize contract selection; removing an earlier contract preserves the selected job. Local story objectives offer flight actions instead of plotting a route to the current system. Evidence choices appear after the required delivery.
- Updated the existing tutorial's Guild lesson to use Mission Log, without changing lesson timing. Added tracking regressions and native PSP captures.

## 2.5.152 — One mission conversation style

- Unified first-flight dialogue, later story briefings/replies/decisions/codas, Guild assignments, tracked contracts, station welcome and Triangle conversations under one shared PSP layout.
- Consistent portrait/speaker, speech box, objective band and wrapped orange reply buttons. Guild speakers now match the authored dialogue. Long speech pages with L/R; unsupported smart punctuation is converted for the PSP font.
- Notifications use the objective band rather than covering replies. Fixed invisible choices leaking from an untracked story decision into Guild/contract menus.
- Open incoming conversations remain conversations when an encounter ends; Back clears conversation mode. Mission actions and tutorial pacing are unchanged.
- Added native-resolution captures for 12 conversation states in both palettes, geometry/highlight tests, paging, punctuation and reply-count regressions.

## 2.5.151 — First Light story tutorial

- Added Start tutorial, Continue tutorial, New game and Load commander to the title menu.
- Added 61 authored Kei/Venn lessons with gradual service unlocks, flight exercises, all station rooms and GalacticNet tabs, real fitting/trading and a planetary survey.
- Added separate tutorial checkpoints, save V15 migration, lesson replay/recovery and a harbour-licence handoff into Open Channel.
- Exposed Guild and Display & chatter as direct command-deck entries.
- Added complete input-path walkthrough, save/migration coverage and native PSP captures. Updated stale steering, sale-confirmation and title-menu test expectations.

## 2.5.150 — Roll-independent flight controls

Yaw and pitch steering now stay screen-relative after rolls and full vertical loops, so D-pad directions keep a consistent meaning. Steering and build checks pass.

# Changelog

## Unreleased — first explorable station slice

- Added a reusable procedural `StationIdentity` derived from system and hub
  variant for future 256-station content.
- Added a playable cargo-bay missing-manifest activity using the existing room,
  hotspot and dialogue systems; progress and reward survive commander saves.
- Documented the station audit, PSP layout constraints, verification status and
  next steps in `docs/STATION-FIRST-SLICE.md`.
- Removed the standalone Targeting computer from the FLY menu; held Square
  remains the quick targeting path.

## 2.5.149 — Radio ticker alignment

- Dropped the tiny radio host face five pixels so its expression sits on the ticker baseline.
- Moved the equalizer to a centred rail below the ticker, expanded it to 28 bars, and added a slow station-colour fade.

## 2.5.148 — Reticle-aware Square targeting

- Made a quick Square tap silently select the valid object closest to the centre reticle.
- Delayed the left-side targeting computer until Square has been held for 0.20 seconds.
- Prevented release after a long hold from replacing the target chosen inside the targeting computer.

## 2.5.147 — Cleaner replies and silent targeting

- Reduced the quick reply chooser to the two actionable boxes: `IGNORE` and `RESPOND`.
- Made X confirm the highlighted reply after Triangle opens the chooser; Triangle release is no longer an action.
- Removed navigation-computer chatter when selecting or locking targets with Square, while retaining the on-screen `(R = LOCK)` reference.

## 2.5.146 — Ship Decorator alignment polish

- Nudged the fictional site address and bottom control prompt into their pixel-art monitor recesses using pixel-precise placement.
- Marked the ship's currently equipped finish directly in the colour list, so it remains visible while previewing other paint choices.

## 2.5.145 — Native pixel monitor for Ship Decorator

- Replaced the generic decorator panels with an original 480×272 pixel-art CRT and custom-site plate, inspired by the user's monitor references.
- Reserved fixed artwork windows for eight selectable paint finishes and a live, enlarged 3D preview of the current hull.
- Added swatches, equipped/price feedback, a compact message area and controls inside the monitor frame.
- Kept preset swatches separate from each ship's applied colour, fixed the paint fee to match its displayed units, and expanded the per-ship paint array to cover all ten purchasable hulls.
- Quantized the plate on a 2-pixel grid for sharper PSP-scale art, added a wireframe outline to the live ship, and kept unrelated mission notices off this screen.

## 2.5.144 — Loadout quick stats

- Removed the redundant `CURRENT SHIP` label from the Outfitting loadout panel.
- Added a clear `MISSILES n` count beneath the utility slot.

## 2.5.143 — Shipyard descriptions

- Replaced `ABOUT THIS HULL` with a full three-line `SHIP DESCRIPTION` block in the open lower area.

## 2.5.142 — Radio host alignment

- Nudged the tiny radio host face down a few pixels for cleaner alignment beside the ticker.

## 2.5.141 — Shipyard units badge

- Added the fixed bottom-right units balance to Shipyard, matching Outfitting and Decorator.

## 2.5.140 — Outfitting copy fit and readability

- Expanded the lower-left item description to three native-width lines.
- Wrapped long Effect text so module stat information cannot run out of the panel.
- Compressed cargo capacity to one clean line: `CARGO HOLD 2/8T`.

## 2.5.139 — Outfitting stock scrollbar

- Renamed the stock panel to `AVAILABLE TO BUY`.
- Replaced the page counter with a slim left-side scroll rail and thumb.

## 2.5.138 — Cargo hold label

- Replaced the cramped `HOLD` line with a clear `CARGO HOLD` label and separate capacity count.
- Kept the label and count inside the left loadout panel boundary.

## 2.5.137 — Complete stat change copy

- Replaced all remaining `SEE EFFECT` placeholders with concise concrete stat results for every outfitting module.

## 2.5.136 — Outfitting copy cleanup

- Removed the redundant `READY TO FIT` and `CURRENT EMPTY` labels.

## 2.5.135 — Outfitting description band

- Moved highlighted module descriptions into the open lower-left area of Outfitting.
- Kept the right card focused on effect, exact stat change, fit state and cost.

## 2.5.134 — Chunky monitor decorator

- Reframed Ship Decorator inside a large native pixel monitor bezel.
- Added a few larger hand-drawn sticker/ad blocks and simplified the page copy.
- Kept only the paint choices, live ship preview and essential purchase status visible.

## 2.5.133 — Expressive radio host

- Enlarged the tiny host face to a readable 12×9 pixel badge.
- Added smile, talking, surprised and blink expressions that cycle during broadcasts.

## 2.5.132 — Radio host placement

- Moved the animated host face directly beside the scrolling radio ticker so it reads as the speaker.

## 2.5.131 — Exact outfitting stat previews

- Added a concise before → after stat line beneath every module effect.
- Cargo upgrades now show exact storage changes such as `10 > 18 MAX STORAGE`.

## 2.5.130 — Outfitting detail layout

- Moved item descriptions to the open lower area of the detail panel.
- Removed the unexplained TECH level label and kept cost, effect and fit status clear.

## 2.5.129 — Separate fuel accounting

- Fuel no longer consumes a cargo tonne or blocks refuelling when the hold is full.
- Outfitting now shows the current fuel level directly above the fixed units balance.

## 2.5.128 — Ship Tech Board clarity

- Shortened empty slot labels so they stay inside each hardware tile.
- Replaced the overlapping right-panel heading with a clean Slot Details header.
- Added four-direction D-pad movement across the 2×3 loadout grid.

## 2.5.127 — Pimp My Ship web page

- Reworked Ship Decorator into a radically different GalacticNet-style fake website.
- Added browser chrome, navigation links, sponsored customs tiles, featured preview framing and web-store copy while preserving repaint controls.

## 2.5.126 — Radio ticker visual cleanup

- Moved the long waveform rail below the scrolling radio ticker.
- Replaced the second equalizer with a tiny animated talking-host face.

## 2.5.125 — Boost speedometer clarity

- Speed fill now stays on the normal cruise scale while boosting instead of appearing to reset.
- Boost adds a red, pulsing/jittering meter treatment; the numeric SPD readout remains clear beside the shortened bar.

## 2.5.124 — C64 sticker pack

- Added chunky native-resolution workshop stickers, badge blocks and bezel graffiti to Ship Decorator.
- Kept every mark inside the PSP-safe panels so the playful art never collides with the paint list or live ship preview.

## 2.5.123 — Ship Decorator personality pass

- Added a playful custom-hull-shop presentation with workshop signage, sticker-like bezel decals and clearer “apply finish” language.
- Kept the live preview, paint list, prices and fixed credits badge inside the native PSP layout.

## 2.5.122 — Comms Panel conversation controls

- Flight chatter now shows `△ CLOSE` for one-way messages and `△ REPLY` for reply-capable contacts.
- Triangle on a reply-capable transmission opens the Talk/Ignore choice; holding Triangle still opens the Comms Panel.
- Renamed the deck service to Comms panel, moved Display & Chatter settings into it, and kept those controls available from the Fly group.

## 2.5.121 — live decorator preview

- Ship Decorator now has a dedicated top-right live display port.
- Moving through finishes recolours the preview immediately; the applied ship colour still changes only after purchase.

## 2.5.120 — targeting lock prompt

- The held-Square targeting computer now shows the concise `(R = LOCK)` prompt at its base.

## 2.5.119 — flight speed and approach validation

- Speed lines now scale from low cruise through dense boost streaks, with the numeric speed shown beside the bottom-right speed bar.
- R/L acceleration, braking, double-tap boost and hard-brake behavior remain covered by the flight input checks.
- Relay docking regression now verifies the visible approach sequence before services open.

## 2.5.118 — wanted docking interception

- A wanted commander is intercepted by local law before station entry and must settle the warrant, surrender to custody, or run before docking can continue.

## 2.5.117 — docking approach and debug tools

- Relay and outpost docking now use the visible approach and welcome sequence instead of skipping to services.
- Added debug actions for installing a pulse laser, revealing Codex records and restoring heat and shields.

## 2.5.116 — combat target lock

- Firing at a selected ship or freighter now promotes that contact to the active lock immediately, keeping the target HUD synchronized with the attack.

## 2.5.115 — archive progress and service balance clarity

- Added a right-side progress scrollbar to each three-file Cosmic Archive category.
- Kept ship service credits in the fixed badge and ship loadout view.

## 2.5.114 — weapon validation and shipyard stat checks

- Mining regression fixtures now equip a real pulse laser, matching the player rule that empty weapon slots cannot fire.
- Shipyard and flight continue to use the same hull capacity, speed, range and price data for each ship.

## 2.5.113 — clearer commodity warnings

- Restricted cargo now uses a concise red `RESTRICTED GOODS!` warning.
- Removed the redundant buy/sell advice subtitle beneath market averages.

## 2.5.112 — weapon gating and outfitting labels

- Firing is now disabled until a weapon is fitted in the WPN slot, with a clear outfitting prompt.
- Widened outfitting catalogue labels so full item names remain readable.

## 2.5.111 — bounded shipyard catalogue

- Shipyard hull listings now stay inside a seven-row scrollable panel with a left-side thumb.
- Added a compact description for the selected hull in the lower-left information bay.

## 2.5.110 — control icon and radio HUD cleanup

- Removed the redundant `RADIO` caption beside the cockpit equalizer bars; the bars and scrolling chatter remain.
- Standardized footer controls so D-pad directions render as arrows and shoulder controls render as L/R button icons, with Start/Select and face-button glyphs preserved.
- Kept the held-Square targeting hint and power-bank selection free of extra underline clutter.

## 2.5.109 — held-Square targeting guidance

- Removed the cluttered D-pad, band and release instructions from the held-Square overlay.
- Added a high-contrast `R LOCK ON` callout.
- Replaced clipped scan-tab abbreviations with a full `BAND: ...` label for the active targeting category.

## 2.5.108 — HUD and station service layout polish

- Moved the speed bar directly below the FUEL bar and removed its former upper-right numerical readout.
- Fixed the Display & Chatter list so Third-person view is selectable and functional.
- Moved Ship Decorator and Engineers into the docked Ship category as separate services.
- Decorator selection now scrolls through eight finishes, including Neon Grid and Starfall Theme, with a live painted ship preview.

## 2.5.107 — World State v1, Mission Validation v1 and Lave certification

- Added the canonical world-state snapshot for economy, danger, government, factions, traffic, stations, planets and active events.
- Routed mission display and acceptance through reachability, station and landable-body validation.
- Added Lave certification checks for peaceful onboarding, faction reconciliation, reputation progression, planet identity and playable mission offers.
- Documented the release gate in `docs/WORLD-STATE-V1-MISSION-VALIDATION.md`.

## 2.5.106 — richer Discovery Codex system previews

- System archive entries now show their matching station, star or planet artwork in the right-hand description panel.
- Scrolling through a system updates the panel with the selected record's short summary before opening its full X detail page.

## 2.5.105 — ship decorator and third-person display

- Added a station-only Ship Decorator menu with eight paid paint finishes and a live exterior preview.
- Added a Third-person view toggle under Commander → Display & Chatter.
- The selected paint colour is used by the ship preview and in-flight exterior view; Triangle from the decorator opens Engineers.

## 2.5.104 — boost cooling, speed HUD and damage smoke

- Heat now cools immediately after boost release, including while the ship is still travelling at high speed.
- Added a compact lower-right speed readout and bar above the power indicators.
- Reworked cockpit damage smoke into dim particle drift whose count increases with hull damage.

## 2.5.103 — clear cockpit radio ticker

- Removed the `ALIEN` label from the upper-right radio strip so it cannot overlap the scrolling ticker subtitle.
- All stations now use the same compact `RADIO` identifier; station-specific colour and broadcast copy remain intact.

## 2.5.102 — native targeting computer rewrite

- Reworked the full targeting page into a 480×272 monitor layout with a six-row contact list and dedicated readout panel.
- Added clear contact status, faction/class, range, mission color, compact hull/shield bars and a short Triangle details reveal.
- Kept the existing monitor bezel/stickers and held-Square flight targeting controls while removing overlapping center-view text.
- Documented the layout and validation rules in `docs/TARGETING-COMPUTER-PSP-SPEC.md`.
- Fixed left/right steering inversion after a full vertical loop by mirroring yaw input across the pitch pole.

## 2.5.101 — radio station/level navigation

- Removed the duplicate STATIC label during station changes.
- Radio Up/Down now switches only between Music and FX levels; Left/Right adjusts the selected level, while L/R changes station.

## 2.5.100 — quick-response comms

- Triangle taps now ignore an incoming chat immediately.
- Holding Triangle opens an in-flight Respond/Ignore overlay, while Respond continues into a fuller multi-option discussion screen.

## 2.5.99 — planet survey records

- Removed Flora and Fauna from the Discovery Codex section cycle.
- Selected landed planets now open expanded survey pages with flora, fauna and mineral records.

## 2.5.98 — cosmic Galactic Lore archive

- Removed Galactic Lore from the Discovery Codex tab cycle; it is now opened only from the Discover menu entry.
- Rebuilt the lore view as a readable cosmic archive with six sections, 18 expanded entries and lightweight 1950s pulp-sci-fi-inspired cover art.

## 2.5.97 — landing-gated planet Codex

- Discovery Codex planet and system archive entries now appear only after the commander lands on that planet.
- Landing flags persist in saves, and the ship computer confirms the first time each planet is added to the Codex.

## 2.5.96 — first-arrival system briefings

- On the first warp into a system, the ship computer now announces the arrival and reports its economy, government, technology and local danger level.
- The briefing confirms that the system’s station and four planets were added to the Discovery Codex; repeat visits use the normal concise arrival message.

## 2.5.95 — matching Discovery Codex planets

- Discovery Codex planet entries now use the exact in-system names (`Lave 1`, `Lave 2`, etc.) instead of generic World labels.
- Codex planet records and previews now use the matching system body type, palette, seed and artwork.

## 2.5.94 — readable Start/Select icons

- Changed the tiny L/R, Start and Select symbols to black rounded PSP-style buttons with white letter marks.
- Start shows `ST`, Select shows `SE`, and the shoulder buttons show `L`/`R` so they are immediately distinguishable at small size.

## 2.5.93 — unified PSP control icons

- Added a shared tiny icon set for Cross, Circle, Triangle, Square, D-pad directions, the full D-pad, L/R shoulders, Start, Select and the analog nub.
- Updated footer prompts to use consistent aligned symbols and PSP button colours instead of mixed text-only labels.

## 2.5.92 — richer cockpit radio chatter

- Expanded each radio station’s cockpit ticker from 10 to 24 snippets, including adverts, alien call-ins, arguments and strange talk-show moments.
- Added a short varied pause before each new ticker line begins scrolling, making broadcasts feel less mechanical while staying PSP/60 FPS friendly.

## 2.5.91 — solar-system Discovery Codex

- Discovery Codex now drills down from visited solar systems into their station, star and four worlds.
- World records show potential flora and fauna; system records summarize minerals, echoes, life logs and station traffic.
- Added X to open a selected record and Circle to step back through the hierarchy.

## 2.5.90 — expanded Galactic Lore archive

- Added `GALACTIC LORE` as its own Discover menu entry beside Discovery Codex and GalacticNet.
- Expanded the lore archive to twelve readable entries covering the galaxy, Sol, colonies, hyperspace, pilots, law, history, science, trade and the future.
- The lore screen now uses the full panel layout for longer descriptions and clear entry numbering.
- Renamed the station menu action to `DISEMBARK`.

## 2.5.89 — PSP XMB identity artwork

- Added an Elite: NEXT game icon for the PSP XMB using the existing Elite logo.
- Added a deep-space starfield background for the PSP XMB game selection screen and packaged it into EBOOT.PBP as PIC1.PNG.

Note: standard PSP XMB backgrounds are still images; the game itself retains the animated starfield once launched. A true animated XMB background would require a firmware-specific PMF/ICON1 video asset and encoder that is not present in the project toolchain.

## 2.5.88 — clearer radio tuning feedback

- Replaced the radio menu's `TRI POWER` text with a green PlayStation Triangle button glyph.
- During the brief station-change static interval, the station title now reads `STATIC` instead of showing the old or new station name.

## 2.5.87 — visible hyperspace arrival

- The destination solar-system cockpit view now fades in through a cool blue arrival wash during the first 1.35 seconds after hyperspace braking completes, while the cockpit HUD remains readable.

## 2.5.86 — ambient space encounters

- Added a lightweight encounter deck for trader traffic, police, pirates, distress calls, cargo, wreckage, smugglers, bounty leads, and mysterious contacts.
- Holding Triangle on an encounter transmission now opens Respond/Ignore in the existing comms panel; Respond follows the encounter hook and Ignore closes it cleanly.
- Encounter timing and presentation reuse the existing speech and traffic systems so the new activity stays PSP/60 FPS friendly.

## 2.5.85 — reliable docking, flight attitude, and warp presentation

- Removed the unused decorative shape that could appear at the top-left of the flight view.
- Circle now prioritizes a centered station in docking range, and guided docking recovers safely into the third-person arrival sequence.
- Boost release clears residual roll so flight steering cannot remain rotated after boosting; full pitch loops remain intact.
- Hyperdrive charge now keeps the ship view visible and progressively dimmed before the hyperspace corridor begins.

## 2.5.84 — functional power distribution

- WEP pips now use a shared weapon-output multiplier for ship lasers and mining.
- Added regression coverage for WEP output and SYS/ENG power effects.

## 2.5.83 — clearer mission and wanted HUD cues

- Tracked mission instructions now show in green with `>> ... >>` route arrows.
- Kei's opening mission remains the default tracked objective and is covered by the HUD tracking checks.
- The idle `WANTED 0/5` counter is hidden; wanted commanders instead see an escalating orange/red flashing `LAW IS AFTER YOU!` warning.

## 2.5.82 — cockpit text radio

- Far Horizons is now the default station on a fresh launch.
- Added a lightweight scrolling radio-talk ticker beside the cockpit activity animation, with varied station-specific captions.
- Radio controls now use PSP L/R for tuning, Up/Down for Music/SFX selection, and a visible Triangle power button.

## 2.5.81 — cleaner cockpit menu footer

- Removed the `LEFT/RIGHT TAB / UP/DOWN` navigation hint from the main cockpit menu footer.

## 2.5.80 — menu wording refresh

- Updated launch, station exploration, shipyard, outfitting, controls, radio, and loadout subtitles.
- Undocked Ship menu continues to show `Cargo` instead of `Cargo & market`.

## 2.5.79 — law warnings and no-funds custody

- Wanted commanders now receive a local law warning before an arrest is initiated.
- Zero-unit custody now plays a jail transfer and law seizure sequence before releasing the commander with only the basic ship at the local station.

## 2.5.78 — readable Galactic Lore details

- Galactic Lore entries now use a full-width bottom panel with wrapped text instead of the narrow right detail column.

## 2.5.77 — concise cargo menu label

- The undocked SHIP menu now labels the cargo screen simply `Cargo`; docked stations retain `Cargo & market`.

## 2.5.76 — longer engine boost endurance

- Boost generates less heat overall, and ENG pips further reduce boost heat while increasing active cooling.
- The overheat warning now reads `ENGINES OVERHEATING... COOL OFF!!` while critical-overheat protection remains intact.

## 2.5.75 — staged hyperdrive jump

- Galaxy-map jumps now return to the cockpit and build through charging, escalating streaks/shake, a vivid hyperspace corridor, and a braking phase before arrival.

## 2.5.73 — Kei replies directly

- Choosing a reply in Kei's opening briefing now advances straight to Kei's response without redrawing the commander's selected speech.

## 2.5.72 — attacking enemies alert

- While holding Square, the `ENEMIES` tab now flashes red when the player is under attack or has an incoming missile.

## 2.5.71 — clearer held-Square target controls

- Replaced the old `D-PAD BANDS` scan hint with `L TARGET IN FRONT` and `R LOCK ON`.

## 2.5.70 — unrestricted flight pitch

- Removed the artificial up/down pitch limits during space and atmospheric flight.
- Up and Down now allow continuous looping turns through a full 360 degrees in either direction.
- Added a regression check covering a complete pitch loop; landed surface movement limits remain unchanged.

## 2.5.69 — animated cockpit activity display

- Replaced the top-center heading/danger glyph cluster with a lightweight native-pixel sci-fi display.
- Cycles through drifting stars, radar sweep, telemetry waveform, rotating planet/galaxy, and ship schematic patterns.
- Motion speeds up with cruise velocity and switches to hyperspace-like streaks during boost; danger remains visible through the alert border/markers.
- Preserved the rest of the cockpit HUD and gameplay state.

## 2.5.68 — loadout and mission log readability

- Separated Mission Log main/side sections and objective area so all five contracts remain visible. Hide the Abandon prompt on main missions.

- Corrected pixel/character coordinate confusion that hid ship slot labels.
- Kept module names in the details panel and made the in-flight hint explain docking is required.
- Verified all six selected labels in both palettes, with native captures and a passing emulator smoke run.

## 2.5.67 - Lave signage draw-order fix

- Moved the Lave Public canteen identity signage after the back-wall layer so the nameplate remains visible at native resolution.

## 2.5.66 - Lave canteen visual slice

- Added a Lave-only native pixel-art window, crescent world, hanging lamps, booth silhouette and floor lighting to the public canteen.
- Preserved the existing station room graph, selectors, NPC hotspots and PSP 480x272 layout.

## 2.5.65 - v0.5 gate repair

- Corrected the campaign input regression harness to return to the briefing after opening Mission Log and after backing out of a coda.
- Updated the undocked deck expectation to match the intended hidden-service list: Shipyard, Outfitting and Mission Board are removed from navigation while flying.
- Full emulator smoke now passes game, input, steering, radio and performance checks.

## 2.5.64 - planetary presentation polish

- Added compact native-resolution contact shadows beneath surface flora, bushes and rocks so close props read as grounded in the terrain.
- Kept the effect low-contrast and disabled in high-contrast mode; collision, terrain and EVA behavior are unchanged.

## 2.5.50 - native station visual rollout

- Added a shared native 480×272 station presentation shell with consistent headers, options rail, feedback area and SHIP return treatment.
- Added an authored Reorte primary Arrivals scene using the Second Shift palette and staging language.
- Kept all existing station hotspots and interaction behavior intact while the remaining room art is upgraded.

## 2.5.49 - 2026-09-23

- Added the playable Second Shift bar at Reorte's primary hub using the approved native PSP pixel artwork.
- Added Lysa Kest, Pell Sorn and Dax Neral conversations, safe Reorte I navigation guidance, and free High Orbit dice practice.
- Kept the preview session-only: dice cannot change credits, cargo, missions or saves, and ordinary canteens in other hubs remain unchanged.

## Unreleased - current-ship menu beauty preview

- The command-deck inset shows the actual owned hull in a quiet 96-second
  flight loop with distant system planets, a steady sun and sparse stars.
- Explicit panel clipping and an independent presentation camera replace
  live-world camera mutation. The ship stays fitted throughout its camera arc.
- Optional -MenuPreview smoke checks cover all owned hulls, clipping,
  unchanged simulation, restored render state, loop continuity and timing.

All notable playable releases are recorded here. `VERSION` is the current canonical version.

## 2.5.74 — Trader cargo offers

- Traders now offer a small commodity-for-commodity exchange during a hail. Buy the requested cargo, find the same trader again, and hail them to complete the deal.
- Scanned ship contacts keep their callsigns in the targeting computer for the current system, making it possible to relocate a trader after visiting the market.
- Trader offers remain session-local so the existing V13 commander-save format stays compatible.
- Cargo canisters now use a short tractor-beam pickup animation that stops the ship, holds position, and completes the collection after the beam finishes.
- Discovery Codex gains a `GALACTIC LORE` tab with original compact reference entries about the Milky Way setting.

## 2.5.48 - 2026-09-23

- Reworked planetary EVA with separate look and movement, shared rendered/collision terrain, bounded shores and fields, jet descent, reliable boarding, ship guidance, suit status and the full scan-to-sale regression journey.
- Added a story-awarded atmospheric landing kit while preserving atmosphere escape and completed, skipped and older-story compatibility.
- Replaced the static menu ship inset with a clipped looping beauty view of the commander’s actual ship, current-system sun and planets.
- Added 15 longer original action cues and bounded radio shuffle ordering without changing decoder, sleep, save or game-state formats.

## Unreleased — planetary EVA traversal

- Separate on-foot look, walk/strafe and hold-R jet controls; analog-off fallback and a dedicated fifth Help page.
- Match terrain collision to the rendered triangles, block water/local edges, and permit grounded boarding immediately after disembarking.
- Show ship bearing/distance, exposure and remaining suit health; preserve scan feedback and fill the full EVA viewport.
- Add traversal, resource, save and mineral-to-hub-sale regressions. V13 layout and existing reward rules are unchanged.

## Unreleased — original event audio and bounded shuffle

- Original 20–300 ms event cues give scans, docking, warnings, combat and engines distinct identities with smooth endpoints; existing stereo stations and volume preferences remain unchanged.
- Shuffle visits each candidate once before giving up, deferring the previous track until alternatives fail.
- Added duration, peak/headroom, candidate-order and optional original audition checks; documented MP3-only support and physical PSP crackle/suspend verification gaps.

## 2.5.47 - 2026-09-23

- Planet approaches now stop the remaining collision frame safely, pause threats while the choice is open, and give gas giants truthful turn-away controls.
- Chapter 05 adds its timestamp evidence loop, requires both prior records, and safely reopens compatible older Chapter 05 saves.
- Pilot-rescue objectives now correctly teach Circle to lock and Triangle to hail within 600 m, with the complete pickup, return, payment and retry loop covered by input regressions.

## Unreleased — safe planetary approach

- Planetary boundary interception stops weapons and remaining simulation immediately; active approach choices pause threats and clocks.
- Gas giants offer a truthful Circle escape, and reject X entry with an explanation.
- Approach text and actions stay inside the safety panel in Full, Minimal and Scenic HUD modes.
- Added non-sun boundary, sun damage, turn-away, orbit return, PSP-input and native framebuffer regressions. Save V13 and mission rewards unchanged.

## Unreleased — story-awarded planetary landing kit

- Planetary landing is now gated behind an authored story milestone after the power lesson. Kei awards an atmospheric landing kit found in Ryn's locker, making the first landing a meaningful progression beat without adding a save-field or schema change.
- Atmosphere approach remains available before the handoff, but landing on the surface pad is blocked until the kit is awarded. Completed/free-story saves remain compatible.

## Unreleased — authored Chapter 05 evidence loop

- Added the timestamp comparison at Lave and inspection/protest/open-case choices for the early Meridian filing.
- Both prior records are required; partial evidence, wrong-system returns, and repeated comparison actions cannot advance the chapter. Legacy Chapter 05 saves get an explicit case-reopen recovery. State reuses existing `saga_flags` without a save-version or `Game` layout change.
## Unreleased — pilot rescue control guidance

- Generic pilot-rescue objectives now explain locking the rescue ship and using Triangle to hail within 600 m; Circle locks the contact rather than collecting the pilot.
- Added input regressions for rescue acceptance, tracking, pickup boundaries, unrelated contacts, guided hub return, single contract/Guild payment, and expiry/retry. Rewards, timers and saves are unchanged.

## Unreleased — authored Open Channel Chapters 02–04

- Added sealed-receiver pickup and home delivery, quiet-signal observation with fire/heat reset and re-entry recovery, a required convoy port stamp, and an optional lifeboat scan that grants Independent trust once.
- Reused reserved `saga_flags` bits without changing the save schema; unrelated scans, kills, cargo and generic docking cannot advance these objectives.

## Unreleased — native art pipeline and benchmark scenes

- Added a manifest and validator for 480×272, ARGB1555, nearest-neighbour,
  palette-limited runtime art.
- Registered Station Arrivals, Planet Approach and Campaign Dialogue as the
  first native composition contracts and wired station viewport geometry to the
  shared layout header.

## Unreleased — shared painted cover field

- Flight, docking, and command-deck previews now share a deterministic stepped
  sky field with an asymmetrical period-illustration mass and tiny relay cue.
- The centre remains quiet for the reticle and HUD; no simulation or targeting
  state is added.

## Unreleased — native deck copy guard

- Shortened command-deck helper copy to fit the 27-column detail pane at 480×272.
- Bounded the detail line in the renderer so future copy cannot run into the right edge.

## Unreleased — Systems/QA V13 bounds hardening

- Loadout display and module refunds validate fitted catalog indexes before using equipment arrays.
- Added V13 slot-value regression checks; save version and V12 migration remain unchanged.

## Unreleased — ART targeting CRT glass (tip; no micro-tag)

- Targeting computer monitor gets faint CRT scanlines + sparse static under the list (glyphs stay sharp; no text bloom). No shooting stars.

## Unreleased — Elite-style equipment modules (tip; no micro-tag)

- Outfitting **fits** modules into WPN/DEF/NAV/HOLD/FUEL/UTIL; Loadout shows the same slots; Square/X sells back at 50%.
- Fitted gear changes real stats: mil shield 4.5/s, pulse 24 / beam 36, ECM/chaff missile break, heat-sink dump, auto-repair, mining laser, refinery, pax cabin gate, agri scoop rate, escape pod recover.
- Save **V13** stores fit[6]; V12 loads synthesize slots from upgrade bits. Credits, stock, and loadout agree.

## Unreleased — law system rewrite (tip; no micro-tag)

- Law is a real loop: restricted goods only matter when scanned; warrant vs cargo-scan stops; settle / run; docked desk (Status Square) and CUSTOMS tip can clear heat.
- Assault/kill of protected ships still files immediate warrants. Pirate bounty unchanged. Save V12 / wanted[] migration kept.

## Unreleased — Commander play fixes (tip; no micro-tag)

- Undocked command deck **hides** Shipyard, Outfitting, and Mission board (dock-only services).
- Menu 3D ship viewport camera orbits much slower.
- Factions screen: X cycles Story channel lore (saga helpers only); Triangle locks nearest ship of that colour.

## 2.5.46 - 2026-09-22

- **Equipment rewrite pack:** real outfitting modules that change ship stats (PR #32 `2c0373d`) on the v2.5.45 tip (targeting monitor CRT fuzz retained).
- Outfitting fits WPN/DEF/NAV/HOLD/FUEL/UTIL; Loadout sells at 50%; save **V13** stores `fit[6]`.

## 2.5.45 - 2026-09-22

- **ART targeting monitor fuzz:** faint CRT static on the targeting computer (`targeting_screen` in `ui-modern.h`, tip `178d34a`) on the v2.5.44 law tip.

## 2.5.44 - 2026-09-22

- **Law rewrite pack:** scan → settle → clear loop from PR #29 (`4fea6f2`) on the v2.5.43 tip.
- Illegal cargo (Slaves/Narcotics/Firearms) is not a warrant until Law scans; assault/kill still files heat.
- Cargo scan stop (submit / refuse / run); settle pay/custody/run; docked Status Square fine desk + CUSTOMS tip; leave system keeps warrant.
- Mission accept no longer auto-warrants smuggling. Save wanted[] / V12 kept.

## 2.5.43 - 2026-09-22

- **Commander play-fix pack:** Art HUD/disembark quiet (`1b4e08d`) + Designer dock-only deck services / slow menu cam / live Factions (`eb4d23a`) + Story Gazette tabloid + faction lore (`9a9c45f` / `gazette-lore.h`).
- Undocked deck hides Shipyard, Outfitting, Mission board; menu ship cam slowed; Factions lore channel + gazette voices.
- Drop HOLD SQ lock hint and text bloom; softer space; neat GalNet tabs; station OPTIONS list.
- Galactic Gazette tabloid jokes that still leak lore.

## 2.5.42 - 2026-09-22

- **Commander playable pack:** ART quiet pass `b0a97e1` on the v2.5.41 tip — permanent meteors/station glitter removed; soft docking lights and plumes kept; MacVenture disembark UI decluttered (no PACK/EXITS overlap).
- Retains ART_AMBER cue + living-galaxy travellers (save V12) from 2.5.41.
- **ART look tip (untagged, on PR #11):** drop always-on `HOLD SQ + R: LOCK`; remove vertical wiggly nebula band; softer cosmic-ocean haze (no noisy streaks); bloom never after UI glyphs; GalNet tabs shortened (BOOK/INBOX) so they no longer overlap; MacVenture disembark drops verb row + pack cue — room title full-width, right OPTIONS list is the only selector (X does talk/go/look/deal). No tag until Systems smoke.

## 2.5.41 - 2026-09-22

- **Commander playable pack:** ART tip `3f0dc20` (ART_AMBER cue freeze, no GOLD) + living-galaxy PR #23 `bc56842` (hybrid travellers, save V12).
- Mission cue ART amber with 2-col header margin; smoke green.
- Hybrid travellers: System Details here/inbound, Triangle hail, GalNet elsewhere mentions.
- **MacVenture disembark declutter (untagged tip):** one action-row verbs; full-width MAIN; PACK/EXITS side panels removed; door labels and prop-marker spam dropped; text band is label+look only. Overlap gone at 480×272.
- **Space FX quieting (same tip):** constant meteors removed; station traffic glitter / four-point sparkle masks dropped; window lamps + aperture/docking lights use soft additive haze; travel glitter stars removed. Plumes/nebula haze kept. No tag until smoke green.

## 2.5.40 - 2026-09-22

- **Big combined tip:** ART DIRECTOR McQuarrie visual style + Systems soft-FB Wave B/C beauty + Story tip-batches on MacVenture polish.
- Station MacVenture rooms: wall plates/rivets, floor seams, berth void stars + beacon, stronger hero staging ARRIVALS→CUSTOMS.
- UI chrome: charcoal + cream/ochre instrument panels; gold corner-bracket look removed.
- Soft-FB space beauty: planet bloom/specular/ocean/ice/city lights; travel glitter, solar wind, traffic wakes, dock beacons, anomaly pulses, denser warp tunnel.
- Atmosphere flight: sun bloom streak, water/ice glitter, volcanic embers; gas-giant ring sparkle.
- Story already on tip: Mission Network Vol II, GalNet colour, Guild Vol III, berth-six epilogue (from prior tip-batches).
- **ART DIRECTOR post-pack animation pass (untagged tip):** ARRIVALS freighter drift + blinking berth slots; room practical pulses; fauna families + field walk bob; docking warm aperture + corridor motes + near-station traffic glints; quieter MacVenture chrome. Presentation-only — no spawn/AI ownership.
- **ART DIRECTOR next visual gap (untagged tip):** shop/canteen/cargo/guild/clinic/customs prop personality; planet settlement window blinks + horizon haze + ochre pad apron/beacons; quieter EVA/cockpit/speech/menu/minimal HUD + portrait/card chrome; warmer warp tunnel + approach/police plates. No micro-tag.
- **ART DIRECTOR implementer tip (untagged, under art authority):** native 8×8 hotspot prop markers; ARRIVALS cargo-loader bob; walk/deck/help/comfort charcoal+ochre chrome; docking aperture + prosperity rings use kit cyan (nav only). No micro-tag.
- **Smoke fix (untagged):** mission cue stays ART amber in the top-right header with a 2-col margin (not flush to the edge); smoke expects ochre ink. No micro-tag.
- **Reconcile onto Designer tip #18 (`f003e09`):** rebased art implementer commits atop GOLD-restore tip; cue ink remains ART_AMBER per ART DIRECTOR palette; header-margin smoke assert preserved. No micro-tag.
- **ART look freeze for Systems combine/tag:** PR #11 tip `64977ba` is the art handoff — no further look work until after the bug-fixed EBOOT. Mission cue stays ART_AMBER (do not restore GOLD).
- **Story tip batch (untagged):** Galactic Gazette sometimes runs tabloid/joke pieces that still leak lore; Factions screen rotates distinct lore voices (`src/gazette-lore.h`). No layout/FX ownership. No micro-tag.

## 2.5.39 - 2026-09-22

- **ART DIRECTOR in-game visual style implementation (untagged tip):** MacVenture rooms gain wall plates, rivets, berth-window stars/beacons and stronger hero staging across ARRIVALS→CUSTOMS; deck/UI chrome drops gold corner-brackets for charcoal + cream/ochre instrument rules; Wave B/C soft-FB planet bloom, travel FX, warp tunnel and docking beacons folded from beauty lane.
- **MacVenture A++ composition polish:** each station room has one hero focal object, three depth planes, and warmer ochre/cream staging (`docs/CINEMATIC-MOCKUP-TARGETS.md` + ART DIRECTOR pixel grammar).
- Side hatch doors replace top chrome door strips so the focal object stays clear; UI chrome thins to cream/cyan hairlines.
- Fewer decorative frames (no checker floors / lamp rows); SHIP return and talk/shop/gift/taxi unchanged.
- **Working tip (no new tag):** Story PR #19 folded in — Act III–IV page scripts (Ch.14–25 through berth), dialogue chrome (COMMANDER bubble, no `YOU:`/`YOU SPOKE`), ask→answer realigned.
- Station TAKE on owned props opens the NPC deal; GO snaps to a hatch first — verb payoffs tightened for fun.
- ART DIRECTOR space animation kit folded in: `space-animation-kit.h` plume/beacon/spark masks, warm station exterior windows, meteor sparkle (`docs/SPACE-ANIMATION-HANDOFF.md`).
- ART DIRECTOR A+++ visual pass plan landed as composition guidance (`docs/A-PLUS-PLUS-VISUAL-PASS.md`) — execution continues on this tip without a micro-tag.
- Story confirm answers + Who Keeps the Light choice staging tightened from screenplay (PR #19 follow-up).
- Story Silence/Carry choice staging + spoken prologue asks folded from PR #19 follow-ups.
- **Story tip batch (untagged):** berth-six epilogue + coalition helpers speak screenplay decision echoes; complete-screen Kei line; GalNet Explorer Guild / Kei posts colour from Open Channel flags — land as one fold, not drip commits.
- **Story tip batch (untagged):** Mission Network briefs use Vol II authored banks (Hungry Pad / Listen Twice / Boring Lies flavour); GalNet Spacebook (Mira/Iona/freighter/Sable) + Mission Network react to Open Channel flags; Guild opening dialogue from manuscript Vol III.

## 2.5.38 - 2026-09-22

- Soft-FB Wave A canopy FX on the unified tip: denser engine plumes, boost heat shimmer, hit sparks and explosion embers (`space-fx.h`).
- Cheap fixed pools, canopy-clipped; high contrast / warp / dock mute decorative sparks.
- Station art kit re-baked against the unify tip. No crawl or story rewrites.

## 2.5.37 - 2026-09-22

- **Unified playable tip:** MacVenture station deck (art-kit soft-FB rooms, clear SHIP return, talk/shop/gift/taxi) plus Open Channel Act I–III eight-beat page scripts with choice blurbs and locked codas (Ch.02–19 through No Easy Flag).
- Act III (Ch.14–19) raised to page-script authority from the screenplay: map deposit, Federal/Imperial/Alliance envoys, Coldest Signal, coalition choice.
- ART DIRECTOR in-game look pass folded in: warmer planets, settlement silhouettes, richer space presentation (`planet.h` / `voyage.h`).
- Composition targets for post-unify MacVenture/station polish land in `docs/CINEMATIC-MOCKUP-TARGETS.md` (hero focus, fewer frames, warm staging).
- Single downloadable pack — story + station together.

## 2.5.36 - 2026-09-22

- MacVenture station polish: ART DIRECTOR bake kit palette/styles wired through `station-art-kit.h` (PR #12) into soft-FB room draw.
- Focal anchors, talk tips, TRI / >>SHIP / EXITS ship return. Bake kits remain proposed references for full-screen textures.

## 2.5.35 - 2026-09-22

- Act I Open Channel briefs (Ch.02–07) deepen to eight ask-then-answer page-script beats with screenplay texture.
- Permanent decisions show consequence blurbs; Act I chapter completes open a locked coda page before the next brief.
- Manuscript Volume IV documents the Act I page-script binding (`docs/ELITE-NEXT-MANUSCRIPT.md`).

## 2.5.34 - 2026-09-22

- Station interior rebuilt as a MacVenture-style point-and-click: Command / Main / Exits / Pack / Text windows, LOOK SPEAK GO TAKE verbs, illustrated hotspots.
- Seven lush rooms on a small graph. Talk, shop, gifts and taxis kept. Grid maze crawl removed.

## 2.5.33 - 2026-09-22

- Whole-game story and missions manuscript begun (`docs/ELITE-NEXT-MANUSCRIPT.md`).

## 2.5.32 - 2026-09-22

- Open Channel ask-then-answer story conversations (tagged release).

## 2.5.31 - 2026-09-22

- Open Channel dialogue quality raised to full spoken character voice (see tagged release notes).

## 2.5.30 - 2026-09-22

- Station crawl FP view rebuilt as a crisp MM6-style space dungeon: riveted wall panels, high-contrast checker floor, hanging lamp glow, labeled side/front doors showing destination room names, denser room props, and portrait NPC sprites with name plates.
- Header chrome separates station title from the current room badge; compass DOOR/WALL cue kept. Talk, shops, gifts and taxis unchanged.

## 2.5.29 - 2026-09-22

- Soft-framebuffer space FX kit (`space-fx.h`): per-system nebula ribbons, layered space clouds, galactic band haze, stronger star twinkle/sparkle, rare shooting stars. GU particle libs were surveyed and skipped — they do not fit this software renderer; effects stay additive, bounded, and high-contrast safe.

## 2.5.28 - 2026-09-22

- Station crawl FP view deepened toward OpenEnroth corridor language: depth-layered wall panels, facing-relative side portals with neighbour-room tint, arched passage frames, tile floor grid, ceiling beams, denser room props, minimap door links, step bob.
- Side doors now track turn facing (LEFT/RIGHT relative), with compass + DOOR/WALL cue; shops/talk/gifts/taxis unchanged.

## 2.5.27 - 2026-09-22

- Story chat is ask-then-answer: Cross speaks your line first (orange YOU bubble); the next Cross reveals Kei's reply. Saga briefs use acknowledgments instead of questions that were already answered on screen.
- Campaign input tests lock the ask→echo→answer order so the catch question never shares a beat with its answer.

## 2.5.26 - 2026-09-22

- GalNet WANTED board makes page 2 unmistakable (gold `>> PAGE 2` banner + bottom DOWN cue); five distinct poster wear styles (clean, corner rip, fray, dog-ear, bullet hole).
- GalNet L/R tab buttons are larger (`<L` / `R>`) and sit farther from NEWS / JOBS so the triggers no longer crowd the section names.

## 2.5.25 - 2026-09-22

- Eight animated 32x32 pixel-art sun families (yellow, blue, white, red giant, orange, violet, flare, binary) cycle frames in flight, charts, codex, system details and planet skies.
- Chart/codex body tints match the live system palette; soft additive corona bloom, anamorphic lens streaks and sparse canopy bloom stay glitch-free without a second framebuffer.

## 2.5.24 - 2026-09-22

- Prologue / saga replies ask first, then hear the answer on the next beat.
- GalNet WANTED board shows page 2 cue, varied torn/weathered posters; L/R sit farther from tab names.
- Per-system animated sun sprites with soft bloom rings (flight, cards, codex); sparse canopy bloom + lens streaks.
- Station crawl first-person view uses OpenEnroth-style layered walls, tile floor, door frames and room props.

## 2.5.23 - 2026-09-22

- Command deck third-person ship inset pulls the camera farther out so the full hull silhouette reads clearly.
- Speech and notice boxes word-wrap to their panel width — no more mid-sentence hard cuts in Kei briefs, flight speech, mission next-steps or station talk.
- Battle talk SFX: distant engagements and close combat now open the channel with a radio-voice cue (muted with Quiet Comms), plus short combat chatter lines.

## 2.5.22 - 2026-09-22

- Station disembark is a first-person NES dungeon-crawler with room grid, minimap, shops, gifts, quest tips and taxi passengers.
- Outfitting only lists gear this hub stocks (economy + tech); no more "needs tech 7 / warp richer" teases. Expanded catalog (~24 modules).
- Ship Loadout screen shows equip slots and cargo/passenger manifest. Passengers use 1t and chatter en route; fare on arrival. Save V11.

## 2.5.21 - 2026-09-22

- Expanded Open Channel screenplay to full Act I–IV authority (~2100 lines): character bible, lore ledger, complete scenes and branching outcomes.

## 2.5.20 - 2026-09-22

- Feature-length Open Channel screenplay (`docs/OPEN-CHANNEL-SCREENPLAY.md`): cast bible, full scenes, branching trees, lore ledger.
- Choice UI labels now match each permanent decision; trust drives helpers, rewards and epilogue colour.
- Retained IMMEDIATE framebuffer present (anti-strobe) and re-checked in campaign tests.

## 2.5.19 - 2026-09-22

- Fixed screen strobing / black flashes on hardware: framebuffer flip is IMMEDIATE again after vblank (2.5.4 NEXTFRAME painted the live buffer).

## 2.5.18 - 2026-09-22

- Planetary landings use biome families from each world’s orbit sprite (ocean / arid / ice / volcanic / forest), with ground, sky, flora and fauna tinted from that body’s colours.
- On-foot planet UI matches the station concourse: clear ON FOOT chrome, nub look + D-pad move, Square scan, Circle board.
- Docked Fly menu **Disembark / walk station** opens the concourse with more talkable locals.

## 2.5.17 - 2026-09-22

- Engine fire trails and aft glints start on each ship’s mesh stern (and freighter nozzles), not floating past the silhouette on a radius offset.

## 2.5.16 - 2026-09-22

- SHIPS band lists every ship, including hostiles; locking a hostile no longer flips you off SHIPS.
- ENEMIES lists only ships currently going after the player.

## 2.5.15 - 2026-09-22

- Bottom canopy **RED ALERT** stays clear of top speech during combat (status no longer hijacks the speech box); scenic HUD still shows the banner.
- Hold Square + Left/Right always cycles through the **ENEMIES** band (hostiles only), even when the list is empty.

## 2.5.14 - 2026-09-22

- Story briefs (first flight and every Open Channel chapter) run a six-beat locked conversation: Circle and Select stay blocked until you accept the next step.
- Final beat restates the objective; after accept, Tracked Mission only reinforces that step — no replaying old dialogue branches.
- Chapter dialogue expanded with character-specific lines drawn from the campaign bible.

## 2.5.13 - 2026-09-22

- Select / command deck top-right is a clearer third-person orbit of your ship in local space (closer camera, larger inset, station or planet backdrop).

## 2.5.12 - 2026-09-22

- Local wanted level under the system name reads as `Wanted n/5` (red when active, dim when clear) so police pressure is obvious at a glance.

## 2.5.11 - 2026-09-22

- Tracked mission cue sits flush on the top-right header (1-column inset), past the danger badge, with enough room to show the full short guide.

## 2.5.10 - 2026-09-22

- Hold Start redistributes SYS/ENG/WEP on the existing cockpit meters only (L/R select bank, U/D move power) — no separate bottom-right panel; input regressions cover the chord.

## 2.5.9 - 2026-09-22

- Double-tap L hard-brake and hull heat (speed / boost / sun → cool when clear; critical locks boost; max destroys) covered by regressions; controls docs updated.
- Targeted ships show compact HULL/SHLD bars on the lower-left target panel; freighters stay tough, return fire when damaged, and raise a heavier local warrant when destroyed.
- Engine exhaust plumes anchor to each mesh’s aft tip instead of floating behind the silhouette.
- Station deck walk is on the command deck (FLY → Walk station deck): talk to concourse NPCs with X. Planetary surfaces tint from each world’s colours; EVA gets a clearer top strip.

## 2.5.8 - 2026-09-22

- Each system uses a distinct orbital template, world-type permutation and colour set so planets look different from star to star.
- Ship, debris and anomaly placement is system-seeded; hyperspace arrival is farther from the hub on a unique bearing.

## 2.5.7 - 2026-09-22

- Radio page looks like a chassis with speaker grille, frequency dial (OFF + stations 1–5), and needle; retune plays audible static; Triangle / left-of-1 turns radio off.
- SHIPS scanner band lists every ship again; ENEMIES still lists only hostiles.

## 2.5.6 - 2026-09-22

- Combat warnings show a compact **RED ALERT** strip at the bottom of the canopy so speech at the top stays readable.
- Hold Square + Left/Right now includes an **ENEMIES** scanner band listing pirates and anyone currently hostile.
- Confirmed Galacticnet Spacebook/Messages order with parody logo, and Discovery Codex Systems + Planets (worlds unlocked by visiting a system).

## 2.5.5 - 2026-09-22

- Speaker labels (`KEI SAYS`, `IONA SAYS`, flight chatter, Law stops) now sit on a colour chip matching the speaker.
- Tracked Mission briefings show the active speaker's face beside the dialogue bubble for saga chapters and Guild assignments.
- Outfitting detail pane explains tech gates in plain language: in stock, already fitted, or hub tech too low with the needed level and “warp to a richer system.”
- Galacticnet tab order is News / Market / Wanted / Spacebook / Messages / Jobs; Spacebook and Messages show a Facebook-parody logo.
- Discovery Codex adds Systems (visited) and Planets (worlds discovered by visiting a system).
- Radio page is a tuner dial with OFF + stations 1–5, static while retuning, and Triangle power off.
- Each system rotates planet types/colours and traffic layout; hyperspace drops you farther from the hub on a system-unique bearing.
- Double-tap L hard-brakes when fast; heat rises from speed, boost and sun proximity — critical heat locks boost, max heat destroys the ship.
- Hold Start adjusts SYS/ENG/WEP on the existing cockpit meters (no separate overlay). Mission cue is right-aligned; wanted level shows under the system name; Select deck preview is a third-person ship view.
- Tracked story briefings are longer three-beat conversations; Circle is locked until the player finishes and accepts the next step (prologue Begin included).

## 2.5.4 - 2026-09-22

- Fixed black screen after PSP sleep/standby (especially long sleeps): freeze MP3 I/O on suspend, then fully rebuild display, controls and radio on resume.
- Switched framebuffer presentation to `NEXTFRAME` after vblank for more reliable hardware display recovery.
- Shortened audio teardown timeout and terminate-delete the worker if it is stuck on Memory Stick wake.
- Smoke exercises a double suspend/resume recover path; physical PSP confirmation still required.

## 2.5.3 - 2026-09-22

- Saved the final galaxy-map route goal separately from the immediate hyperspace hop (save format 10; older commanders still import).
- After each jump, the next hop toward that saved goal is refreshed automatically until arrival clears it.
- Nearby/galaxy UI shows `FINAL` / route-goal labels for manual multi-jump plans as well as tracked story destinations.
- Added journey/input regressions for plot, save/load, intermediate advance and arrival clear.

## 2.5.2 - 2026-09-22

- Added user-supplied radio station folders and shuffled MP3 playback on PSP.
- Improved MP3 playback stability and PSP suspend/resume recovery.
- Kept generated radio audio as a fallback when a station has no compatible tracks.
- Added mission availability checks so objectives point to stations that can actually offer the required contract.
- Expanded galaxy navigation planning so distant destinations can be located and routed through intermediate systems.
- Preserved the current UI, mission, faction, targeting, dialogue, art, and gameplay work documented in `README.md` and `CLAUDE-HANDOFF.md`.

## 2.5.1 and earlier

The detailed development history, controls, implemented feature list, validation notes, and future design plan are retained in `README.md`, `docs/`, and the Git commit history.
## 2.5.51 - Station Welcome mission

- Added an automatic, session-local Station Welcome mission after the opening campaign is complete.
- The mission plots a route from Lave to Reorte, then guides the commander to disembark, walk from Arrivals to **THE SECOND SHIFT**, and talk to **Lysa Kest**.
- Added concise tracked-mission objectives and cockpit cues (`JUMP: REORTE`, `WALK: CANTEEN`, `TALK: LYSA KEST`) so the authored station art and bar features are easy to find.
- The tour reuses existing station rooms, contacts, speech choices, and bar logic; it adds no save-format fields, rewards, or economy changes.
## 2.5.52 - Second Shift route correction

- The Station Welcome mission now advances only after docking at Reorte’s primary Hub H0, which is the station identity that owns the lush authored Second Shift scene.
- Secondary Reorte hubs now explain that the commander must use the primary Hub instead of silently advancing into the generic canteen renderer.
- Kept generic canteen rooms separate until their own native 480×272 art families pass the palette, payload, contrast, and traversal gates.
## 2.5.53 - Optional Station Welcome

- The bar tour no longer overrides the opening story or forces a Reorte jump. It becomes available after the opening campaign is complete and can then be followed from the tracked-mission screen.
- Reorte’s authored Second Shift scene remains restricted to the primary Hub H0 identity, with secondary hubs clearly identified instead of falsely advancing the tour.
## 2.5.54 - Compact target status card

- Stacked targeted ship hull and shield meters into separate compact rows in the lower-left target card.
- Kept the target distance, radar, power banks, and footer clear at the PSP’s native resolution.
## 2.5.55 - Dismissible conversations

- Circle now backs out of campaign conversations without advancing or losing the current dialogue position.
- Select can open the mission log while a conversation is paused, so players can return to what they were doing.
- Updated conversation footers to show the real available controls instead of claiming Circle and Select were locked.
## 2.5.56 - Outfitting loadout redesign
- Replaced the flat equipment list with a three-panel PSP-native layout: current loadout, available modules, and a focused module card.
- Added clear slot highlighting, fit/replacement state, page position, tech level, price, balance, and compact controls while preserving existing buy, fit, sell, and refuel behavior.
## 2.5.57 - Visual ship loadout
- Replaced the flat ship inventory list with a PSP-native pixel-art hull diagram tied to the selected ship mesh identity.
- Added six overlaid equipment markers, fitted/empty slot states, module summary, cargo, missiles, and capacity readouts while preserving removal behavior.
## 2.5.58 - Clearer command deck navigation
- GalacticNet now lives under Discovery with its own tab destination.
- Mission Log separates main missions from side work at a glance.
- Targeting Computer gains a compact CRT monitor bezel and status sticker while retaining the scanline display.
## 2.5.59 - New player hulls
- Added Fer-de-Lance, Krait, and Ophidian as purchasable player ships using existing detailed ship meshes, with distinct speed, range, capacity, and price profiles.
## 2.5.60 - Crew-decaled targeting monitor
- Added native PSP bezel decals to the full-screen targeting computer: peace symbol, heart sticker, and a small crew marker on the physical monitor frame.
## 2.5.61 - Lave acceptance visual slice
- Added an animated native-pixel targeting scope signature to the monitor-themed targeting computer.
- Added a compact stamped hull readout for Ship Loadout while preserving the fitted-slot inventory view.
- Began the Lave acceptance audit across station, flight, mission, planetary, audio, and QA flows; runtime certification remains dependent on the smoke harness report.
## 2.5.62 - Lave showcase visual pass
- Added a Lave-specific public-canteen identity treatment: arch, LAVE PUBLIC sign, and station seal, while preserving generic room geometry and all existing hotspots.
- Added the staged v0.5–v0.7 release plan and evidence gates to the repository.
## 2.5.63 - Grounded planetary ship presentation
- Reduced the close surface ship wireframe scale so it reads as a parked ship instead of a debug overlay.
- Added a restrained native contact shadow beneath the hull while preserving the existing mesh, landing, EVA, boarding, and takeoff contracts.

### Unreleased

- Unified the Fly-menu Comms entry with the held-Triangle Comms Panel. Removed duplicated display, chatter, radio, controls and third-person settings from the flight comms list; those remain under Commander / Display & Chatter.
- Standardized Local Law arrest and inspection conversations on the shared dialogue layout with yellow reply boxes.
- Added the slower seven-channel VOID TALES alien-story broadcast, with longer synthesized story phrases, alien vocal timbres, pauses and a matching cockpit ticker/library folder.
- Enriched NPC aft engine trails with faction/livery neon cores, reflected hot particles and smoke haze while retaining the mesh-derived rear nozzle anchor.
- Expanded GalacticNet News into an eight-item local edition with tabloid jokes, an Orbital Sudoku, a Captain's Column, varied paper treatments and rotating system-specific edition details.
- Reworked GalacticNet Wanted into five live targets per system: posters lock the real ship in the forward view, risk scales bounty target durability and firepower, kills pay Local Law bounties without adding heat, and completed posters show TARGET TAKEN DOWN.

## 2.5.219 — In-world Outfitting shop signs

Outfitting now opens beneath a slim station storefront sign instead of exposing the local economy, technology and trade values. Every station receives a stable, seeded shop identity—such as Major Lazor's Armaments, Aegis Defence Works or The Module Exchange—with a concise speciality caption. Lave remains a dedicated arms house; general and specialised stock keep a matching, readable identity whenever the player returns.

Verification: PSP build passed. Native Outfitting captures confirm the new arms and general headers at PSP resolution; input suite passes with 0 failures.

## 2.5.220 — Distinct planetary ruins

Planetary ruins now render as collapsed ancient structures rather than intact service houses: uneven surviving walls, broken pillars, an open breach and scattered masonry replace the roof, glazing and door. Normal site types retain their existing functional buildings. The established site collision boundary remains intact.

Verification: PSP build passed.

## 2.5.227 — Unique planetary POI structures

Every field POI now has a purpose-built 3D silhouette and matching collision footprint. Weather arrays use three tall transmission towers; observatories use a narrow tower beneath a huge segmented crown dome; rescue sites, supply caches, fossil excavations, thermal stacks, drone wrecks, migration lookouts, crystal grottoes and archive vaults are all visually distinct. Ruins and Seed Garden biodomes retain their landmark designs. Contract sites now appear as relay towers, biology domes or archive uplinks according to their actual job.

Verification: PSP build passed.

## 2.5.229 — Forward targeting and surface scanner

- Tapping Square on foot now selects the visible contact nearest the centre reticle; further taps cycle only through targets currently in front of the player.
- Circle scans the selected nearby flora, fauna or mineral and draws a short cyan scan beam from the explorer to the discovery.
- A completed or repeated scan offers a contextual Triangle shortcut to that exact persistent Discovery Codex record.
- Triangle no longer turns the player toward the ship. It still boards normally when the explorer is beside the parked vessel.
- The held-Square exploration computer now labels Circle as SCAN, while R keeps its existing smooth tracking turn.
- Scan prompts and target state are cleared after leaving a planet, preventing stale Triangle actions in later flight.
- Automated PPSSPP coverage now exercises forward target cycling, Circle scan rewards/no-double-pay, exact Codex routing, non-rotating distant Triangle, boarding and takeoff persistence.

## 2.5.228 — Cleaner EVA HUD and title-screen recovery

Planet exploration no longer repeats the selected POI name and distance at the top-left or above the controls; that information remains in the held-Square exploration computer, while the compass and world target marker remain visible. The Commander tab now places QUIT TO MAIN MENU directly below Debug tools. Select can open this menu after ship destruction, preventing a dead commander— including one lost while on foot—from becoming trapped without a restart route.

Verification: PSP build passed. Dedicated input checks confirm Commander ordering, dead-state Select access and return to the title screen.
## 2.5.230 — Authored System Almanac

System Details is now the native 480x272 System Almanac shown in the approved concept: an authored dense starfield/nebula plate, oversized granular sun, four detailed system-derived worlds, moving dotted orbit paths, small fly-by traffic and a foreground station. Poor, Rich and Mega Capital systems use three separately authored station silhouettes. Assets are compiled into bounded ARGB1555 arrays; the source art, deterministic bake script and exact native previews remain in the project.

The right dossier keeps the system name and description fixed while the player moves between targets. Government reflects Lawful or Pirate control, Economy uses Poor/Rich/Mega Capital, and Known Worlds reflects the four real bodies. X locks the selected station, sun or planet, closes the menu and starts the existing smooth flight turn. Square no longer replaces the Almanac with the old route dashboard.

Verification: PSP build passes; native framebuffer capture inspected. Almanac description, economy wording, target/return/turn and Square-preservation checks pass. Steering and radio report zero failures. The inherited station swept-collision failure and unrelated existing outfitting/dialogue/tutorial/UI failures remain outside this visual feature.

## 2.5.230 — Cinematic commander main menu

The title screen now uses the approved Build 1 composition at native 480x272: a baked Lave planet-and-station vista, the existing Elite:Next logo, one unambiguous vertical menu and a live commander summary. The primary row becomes `CONTINUE: <name>` when a valid save exists and opens the active or first available commander directly. New Commander, the three-card Load Commander screen and First Flight Tutorial remain separate choices; a saved tutorial changes the final row to Continue Tutorial.

Commander names are title-cased for display without rewriting save data. The summary uses the selected save's real portrait, system, ship and credits, plus a compact hull-specific pixel silhouette. High-contrast mode keeps a solid dark background. The source art, native preview and deterministic ARGB1555 bake script are retained under `assets/source/intro`, `assets/preview/intro` and `tools/bake-intro-art.ps1`.

Verification: PSP build passes and the final screen was captured from PPSSPP at native resolution. Full-view comparison against the selected concept passes with no unresolved visual P0/P1/P2 findings. Main-menu New Commander, Load Commander and Tutorial routes pass in the input report; steering and radio remain green. The inherited station swept-collision failure and unrelated existing outfitting/dialogue/tutorial/UI failures remain.

## 2.5.231 — Live rear-view flight mirror

The full and minimal flight HUDs no longer spend their upper-left strip on the current system label or the five-pattern decorative animation. That 252x22 native-pixel area is now a 117-degree live aft camera. It projects the real sky behind the ship, including stable stars, planets, suns, primary/secondary hubs and live NPC positions. Ships retain faction colours, readable silhouettes and trails; the selected contact is bracketed, while a ship actively firing at the player or launching an incoming missile receives a red threat bracket.

The mirror follows yaw, pitch and roll but never changes simulation state, targeting or controls. Its rendering uses bounded fixed loops and no textures, heap allocation or second framebuffer. The radio and tracked mission cue retain their existing right-side space.

Verification: PSP build passes. A native 480x272 PPSSPP capture with pirate, Law and trader contacts was inspected. A focused emulator regression confirms that the aft camera centres contacts behind the player and rejects contacts in front. The broad suite retains the inherited station swept-collision failure; steering and radio remain at zero failures.
# 2.5.233 — Authored Lave Arrivals Hall

Rebuilt the Lave Station Arrivals Hall around the approved 340x168 panoramic pixel-art composition: planet and freighter window, station branding, passengers, Venn, service robot, cargo porter, customs counter, plants and reflective deck. Venn is part of the room plate rather than a duplicate card sprite, while his focus frame remains live. The EXPLORE rail now uses six large framed rows so HERE, Venn and the four principal room routes match the selected walkabout layout; deeper props and services remain accessible by scrolling. The generic arrivals plate remains available to other stations while this Lave-specific asset establishes the authored override pipeline. Focused station exploration checks pass 10/10 in PPSSPP.
