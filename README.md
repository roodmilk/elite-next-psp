# ELITE: NEXT — development build 2.5.5

## New in 2.5.5 — speaker chips, clearer menus, radio tuner, system variety

- `NAME SAYS` labels sit on a colour chip; Tracked Mission shows speaker faces.
- Outfitting explains tech gates (in stock / fitted / hub tech too low).
- Galacticnet: Messages sits beside Spacebook; Spacebook logo; Codex Systems + Planets.
- Radio tuner dial with OFF, station notches, retune static, and power off.
- Systems look and place traffic differently; warp arrives farther from the hub.

## New in 2.5.4 — wake from long sleep without a black screen

- Freezes MP3/file I/O as soon as the PSP begins suspending so the audio worker cannot hang on a spun-down Memory Stick.
- On resume, rebuilds display mode, both framebuffers, clock, controls and radio instead of only restarting audio.
- Presents frames with `NEXTFRAME` after vblank for more reliable hardware display recovery.
- Still confirm on a real PSP after sleeping for several hours mid-flight with custom radio playing.

## New in 2.5.3 — persistent multi-jump route goals

- Plotting a route on the full galaxy map now saves the final destination separately from the next hyperspace hop.
- After each jump the next hop toward that goal is refreshed until you arrive (then the goal clears).
- Nearby jump list and galaxy overview show `FINAL` / route-goal labels for manual routes as well as tracked story destinations.
- Save format 10 stores the route goal; older commanders still import.

## New in 2.5.2 — full galaxy route planner

- Triangle on the Galaxy Map switches between the nearby jump list and a spatial overview containing all 256 systems.
- The overview marks the current system in cyan, the tracked story destination in gold, and draws every intermediate jump in the mission route.
- The D-pad moves the map cursor between systems. L/R zoom from the complete 1x galaxy to a readable 4x local view.
- Cross plots a route to the cursor and returns to the nearby list with the first reachable jump selected.
- Story screens distinguish `NEXT` from `FINAL`, and the cockpit directs the player to the next reachable system rather than incorrectly naming the distant final destination.
- Route planning remains available with an empty tank so the map can show where to go after refuelling.

## New in 2.5.1 — clean custom MP3 playback

- Increased the PSP compressed-audio reservoir from 16 KB to 64 KB to absorb Memory Stick read stalls.
- Increased the hardware output block from 256 to 2,048 stereo frames, providing roughly 46 ms for decoding and refills instead of roughly 6 ms.
- Replaced linear sample-rate conversion with bounded four-point interpolation for smoother 32 kHz and 48 kHz music at the PSP's 44.1 kHz output rate.
- Verified the normal suite and an additional PPSSPP run using the four real MP3 files in `music/Deep Field`.

## New in 2.5.0 — The Open Channel campaign

- The first flight now leads into a 24-chapter Kei and Ryn campaign designed around travel, survey, salvage, conflict and four persistent decisions.
- New story threads explore Lave's GalCop inheritance, an INRA-era mycoid record, competing Federal, Imperial and Alliance offers, an Aegis research echo and one rare, uncertain nonhuman contact.
- Story destinations bind to the real galaxy and remain stable in the save. A chapter only counts actions performed after its briefing and never relies on a random mission-board offer.
- Mission Log and the cockpit objective now follow the active chapter. Dialogue and player decisions retain the blue/orange visual language at PSP resolution.
- Save format 9 preserves all chapter progress, trust and choices while continuing to import older commanders.
- The complete campaign plan and production rules are in [The Open Channel Campaign Bible](docs/OPEN-CHANNEL-CAMPAIGN.md).

Validation: all game, input, steering, radio and performance regression checks pass in PPSSPP. Physical PSP testing remains recommended.

## New in 2.0.5 — priority stability and PSP-screen audit

- Exercised the opening story's accept, launch, flight, docking, reward, optional-assignment and completion states through the real input handlers.
- Captured and reviewed 26 additional runtime states at native 480x272, alongside the nine flight samples and nine story-state captures.
- Shortened outfitting list labels instead of clipping equipment names mid-word. The description panel now uses its available width.
- Clarified the SpaceBook footer's comment and tab actions.
- Re-ran save recovery, save migration, manual/guided docking, landing, EVA, takeoff, warp, death recovery, mission, freight, mining, radio, steering and UI regressions.

No physical PSP was available for hardware testing. The Elite-series lore rewrite remains future work.

## New in 2.0.4 — clear story navigation

- Story screens use visible choices: Up/Down to choose, Cross to select, Circle to return. Hidden story/Guild shortcuts were removed.
- A highlighted next action follows actual progress: accept, launch/resume, docking guidance, collect the reward, then optional assignments. A paid reward is never offered again.
- Optional assignments guide you to the matching contract, an existing job, signals, docking or another system. The flight guide is explicitly separate from the story.
- Footer icons sit beside their labels and match complete button tokens; ordinary words no longer produce spurious icons.
- Story screens reserve distinct areas for character dialogue, next action, choices and feedback at 480x272. The larger lore rewrite remains pending.

## New in 2.0.3 — small corrective release

Retains 2.0.2 mining, freight routes and graphics. Corrects the opening briefing's incorrect assumption that the player's ship is a Cobra, and fixes the Meridian-echo grammar. The requested Elite-series lore rewrite and wider art audit are pending; they are not part of this release.

## New in 2.0.2 — mining and working freight routes

- **Blast the belts:** every rendered belt rock is a targetable, damageable asteroid. Fire with Cross; tougher rocks take several hits. Fractures scatter sparks and leave 1–3 tonnes of mineral cargo. Close within 500 m and press Circle to collect it. The footer switches between MINE and COLLECT.
- **Recognisable freighters:** container carriers, stepped tankers and broad ore barges have separate silhouettes, painted cargo, bridges and stern engines. Their shapes stay consistent at near and far distances. Swept, oriented hull collisions replace the oversized sphere.
- **Occasional warp traffic:** haulers arrive through a small local warp effect, travel to an outer cargo berth at one of the three hubs, transfer goods, turn slowly and depart. New arrival opportunities are 150–316 seconds apart when capacity is available; warp events have a 45-second separation. Lave begins with one delivery underway.
- **Trade with consequences:** each hauler has a reachable partner system and an economy-based cargo manifest. Its visit adds imports and removes available exports from the local market once. Hail it to hear its route, cargo and activity. Destroyed freighters release the cargo they actually carried.
- **Quiet and busy space:** systems allow zero to three freighters according to traffic and prosperity. Initial occupancy, hull types, routes, berth choice, travel speed, loading time, belt layout and rock size vary. Small ships steer clear of the capital lanes.

This is a bounded simulation of the current system. Unloaded systems do not keep trading in the background; all local hubs still share one market. Local rocks and traffic regenerate when a scene is launched/re-entered, while collected cargo and market stock use the existing save system. Physical PSP testing remains outstanding.

## New in 2.0.1 — clear canopy / graphics fixes

- Routine captions stay in the top 56 pixels; instruments and control hints live in the bottom 80 pixels. No floating ship, relay or objective names in the viewing area. Decision screens still pause play.
- Full station names wrap inside the target card. Surface flight shows the local landing pad; walking shows the parked ship.
- Radar covers all 360 degrees: ahead at the top, behind at the bottom, faction colours for ships, squares for hubs and diamonds for worlds. Distance compresses logarithmically; this is not a scale map.
- Power pips have a separate lane from shield, speed, heat and fuel labels. Comms uses two pages without drawing through the feedback area.
- New native pixel silhouettes for HUD objects, surface blooms, fauna and minerals; shaded close freighters; star-coloured solar texture; subtle clipped engine and entrance glows.
- Removed dirty scanlines over text. High contrast disables the vignette and lens flares. Fixed leaking preview pixels, dotted facet seams and detached planet halos.

Validation: PSP compiler warning-free; emulator regression checks and 480x272 frame captures. Physical PSP performance and an exhaustive all-systems graphics audit remain unverified. This release is a graphics/UI pass; the 2.0 interior/spacewalk screens remain early prototypes.

## New in 2.0.0

- Added the authoritative [Design Bible 2.0](docs/DESIGN-BIBLE-2.0.md), covering the Elite/No Man's Sky/Elite Dangerous/Starfield-inspired PSP scope, controls, interiors, EVA, spacewalk, missions, audio, memory budgets and release gates.
- Added the first playable first-person station-deck walk slice. Hold Triangle in flight or on the deck, choose **Walk station deck**, then use the nub/D-pad to move and look, X to interact and Circle to return.
- Added the smart-control-view specification and reserved the interior room graph for the next derelict/ship deck slice.

## New in 1.9.9

- Hold Triangle now opens an expanded quick Comms menu with chatter mute, dismiss, hail, clear target, next radio station, radio page, docking request, Guild and HUD controls.
- Radio remains original/procedural with alien chatter layers; no copyrighted game soundtrack is bundled.

## New in 1.9.8

- The pause/deck menu’s right-side space view now gently orbits a local planet with parallax stars, lens flare and relay silhouettes for a richer, calmer animated backdrop.

## New in 1.9.7

- Removed the redundant “Open Controls” coaching card from the live flight display. Controls remain available from the deck and Help screens, while the opening voice line now welcomes the commander instead.

## New in 1.9.6

- Each system now has a primary hub plus two deterministic relay/outpost hubs. The relays are visible in flight and accept communicator docking when you are within range.
- The bottom-right power strip no longer shares space with circular decorative art; pips, fuel, heat and shield bars occupy reserved lanes.

## New in 1.9.5

- The idle cockpit target card is now glance-first: it shows only target name and range/altitude. Category, hull and orientation details appear while holding Square for the target tool.

## New in 1.9.4

- PSP button glyphs now use dedicated layout lanes in footers and cockpit action prompts, eliminating text/icon baseline collisions.

## New in 1.9.3

- NPC contacts now use faction-specific callsigns: Raiders, Merchants and Guild surveyors get readable identities; Law units deliberately expose encrypted telemetry instead of personal names.

## New in 1.9.2

- Selected planets, stations and contacts now receive a compact in-world nameplate, while the cockpit header explicitly reads `SYSTEM:`.

## New in 1.9.1

- Active mission and story targets now get a compact directional `NEXT` beacon in flight, with an arrow, target name and no extra menu step.

## New in 1.9.0

- Gas giants now render deterministic tilted ring systems with sparse animated glints, giving each system a stronger planetary silhouette at PSP resolution.

## New in 1.8.9

- Nearby traffic now has faction-colored engine glows; capital freighters get longer pulsing wakes.
- The starfield gains restrained deterministic twinkle on a small subset of stars, preserving the low-resolution look while making long flights feel alive.

## New in 1.8.8

- Added low-resolution celestial post-processing: solar halos, animated aperture ghosts, planet atmosphere rims, surface glints and a pulsing station entrance beacon.
- Effects are drawn within the existing cockpit view budget and stay behind warning, target and telemetry layers.

## New in 1.8.7

- Target locks now use faction-colored brackets and animated auto-aim corners, so the target’s identity is visible in the flight view before reading the scanner card.
- Planet approach prompts now use native PSP button glyphs and explain gas-giant behavior in plain language.

## New in 1.8.6

- Cockpit comms now use a readable speech-bubble tail, clear speaker identity and an explicit Triangle hide-chatter affordance.
- Faction portraits gain restrained pulsing identity lights that stay legible at native PSP card sizes.

## New in 1.8.5

- Live scanner incidents call out law engagements and trader convoys under pirate attack, turning distant faction AI into readable events without adding cockpit clutter.
- System Details now provides a live station briefing with faction traffic counts, danger, prosperity and a plain-language activity summary.

## New in 1.8.4

- Added a visible High Contrast Focus toggle under Display & Chatter.
- High contrast strengthens selected-row fills while all text, button glyphs and safety prompts remain unchanged.

## New in 1.8.3

- Contextual cockpit prompts now include tiny PSP glyphs beside the plain-English action strip: X/Circle for approach and police choices, X for the active story step, and Triangle/Square for ordinary flight communication and targeting.

## New in 1.8.2

- Added `docs/FEATURE-MAP.md`, mapping the useful Starfield, No Man’s Sky and Elite Dangerous player fantasies to bounded PSP systems.
- Added a formal glance-first HUD rule: minimal mode keeps only system/danger, speed, shield, target and critical warnings visible.

## New in 1.8.1

- Added tiny colored PSP face-button glyphs to footer prompts while retaining plain-English labels.
- Added a restrained animated CRT dust layer to the cockpit post-processing pass.

## New in 1.8.0

- Added an animated low-resolution engine plume that reacts to speed and boost, with a brighter cyan core during mega-boost.
- The effect is drawn inside the flight viewport, stays behind cockpit text and uses bounded integer primitives for PSP performance.

## New in 1.7.9

- Mission rewards now scale with destination danger, and bounty/covert work carries an extra risk premium.
- The mission board shows each offer's risk beside its payout and explains the selected contract in plain language before acceptance.

## New in 1.7.8

- Mission abandonment is now a two-step action: Triangle asks, X confirms, and Circle cancels. The job, timer and cargo remain safe until confirmation.

## New in 1.7.7

- Story guidance is now explicit across the deck and cockpit: the next story door is marked with `!`, the active story screen is gold, and the current objective is shown as `NEXT` in plain English.
- Flight prompts now use the actual current story instruction instead of a generic controls hint while the tutorial is active.

## New in 1.7.6

- All faction and Kei portrait callers now use an aspect-preserving fit path. Busts are letterboxed inside card-shaped PSP boxes instead of being stretched on one axis.
- Small portraits use the dedicated 32×32 atlas and integer nearest-neighbour sampling for sharper native-resolution silhouettes.

## New in 1.7.5

- Added `docs/A-PLUS-AUDIT.md`, a release gate covering audio, flight, missions, factions, graphics, saves, memory, QA and physical PSP certification.
- The audit explicitly separates implemented behavior from the remaining streamed-radio, later-campaign, surface, Codex and hardware milestones.

## New in 1.7.4

- Mission board offers now carry short, system-aware briefs: prosperous systems advertise cargo demand, dangerous systems describe active raids or distress lanes, and quiet systems use civilian survey language.
- The brief is shown only for the selected offer, keeping the board readable at 480×272 while making contracts feel authored rather than interchangeable.

## New in 1.7.3

- Far Horizons now carries an atmospheric, non-verbal alien chatter layer in the procedural radio mix.
- Radio source manifest added for three verified CC0 space tracks: Magic Space, Persistence (loop short) and Floating in Space.
- Target locks gain a restrained pulsing pixel reticle for clearer feedback without obscuring the cockpit view.

## New in 1.7.2

- Planets are soft approach boundaries: X offers landing, Circle reverses away, and no planetary collision damage is dealt. Gas giants clearly report that they cannot be entered; suns remain dangerous.
- Deterministic system generation now varies orbital phase, scale, tilt, vertical placement and traffic density per system, while Lave remains a busy safe starting hub.
- The flight HUD uses a small pixel danger-star badge and separates power pips from the bottom-right telemetry bars.

Validation: game, input, steering, radio and performance smoke checks pass; physical PSP testing is still outstanding.

## New in 1.7.1

- Corrected faction speaker identity: traders, Law, pirates and Explorers use distinct grounded portrait families; ordinary explorers no longer appear as Kei.
- Corrected Venn’s dockmaster portrait and captions, and hid live market tips while undocked.
- Tightened faction descriptions so they fit completely at 480x272.
- Rebuilt the five-category deck, Controls pages and louder layered radio mix after the identity fixes.

Validation: 192 game, 97 input, 13 steering and 15 radio assertions pass; PPSSPP averages 59.94 FPS with no frame over 25 ms. This remains a development build; physical PSP testing is outstanding.

## New in 1.7.0

- Five deck categories with twenty visible service entries, remembered focus and return-to-owner menus. Campaign, Guild, Radio and Display no longer require discovery of shortcuts.
- Four shorter Controls pages, a visible HUD/chatter settings page and reserved action-feedback bands. Routine character chatter no longer covers ordinary menus. Faction descriptions now fit as complete sentences.
- Restrained late-1980s faction portraits: distinct trader, police, pirate and explorer uniforms. Kei uses a separate muted adult alien portrait. Venn is consistently the dockmaster; generic explorer hails do not impersonate Kei; the computer has no alien portrait; SpaceBook authors use explicit faction identities.
- Station Comms no longer discloses live buy/sell recommendations while undocked.
- Seven-voice music with softer attacks, extended harmony, detuned layers and filtered stereo delay. Music-only gain increased; menu clicks no longer duck it. Existing volume/mute preferences are kept. Five 32-second renders passed headroom/DC checks; subjective listening and physical PSP audio checks remain outstanding.
- UI-SPEC.md is the detailed interface and identity authority, with research references, layouts, per-screen inventory, edge cases and unfinished acceptance work. Larger-text reflow, further purchase/abandon confirmations, stable NPC generations and richer institutional avatars remain planned.

Validation: warning-free PSP build; 192 game, 97 input, 13 steering and 15 radio assertions (317 total), all passing. PPSSPP smoke: 59.94 FPS average, 17.33 ms worst frame, no frames over 25 ms. Nine changed screen types inspected at native resolution, with the corrected faction screen recaptured. No physical PSP certification. V8 commander format and V2 radio settings unchanged from 1.6.0; no save reset required.
## New in 1.6.0

- Play the first authored chapter with Kei and Venn: choose your reply, accept at Lave, fly and return, then claim a harbour badge and 100 units once. X on the intro begins; Square on the deck or Guild screen opens Campaign. X asks a question, Triangle accepts. After acceptance X launches or locks the hub; Triangle opens Controls. Both real manual and guided arrivals count, even if you skip the compass lesson. OC02 and later chapters remain planned.
- Crash during active Lave training: Start restores your commander at the hub with cargo and money retained. Training recovery does not pay a reward. Ordinary rescue tows cannot complete the chapter.
- Kei now has original lavender alien girl pixel portraits, expressions and Comms talking animation. Eight new planet sprite families have stable world-specific palette/mirror variations, shared consistently between flight, maps and details. Suns and surface gameplay retain their existing implementation.
- V8 saves add campaign progress and CRC32 corruption detection, with exact-length validation and existing backup recovery. Older V1–V7 saves import. Copy your current ELITEA folder before upgrading if you want to return to an old build: old executables cannot load V8 saves. Keep the install directory PSP/GAME/ELITEA.

304 regression assertions pass, plus emulator boot and performance checks. See validation reports and docs/PROGRESS.md for the exact release boundary. Physical PSP testing remains outstanding. Save explicitly at a station; there is no autosave.
## New in 1.5.0

- ELITE: NEXT branding, approved AI-generated pixel logo and skippable starfield/story introduction. X begins, Triangle loads your commander, Start skips to the deck.
- Eight alien portraits and twenty item icons compiled as native-size ARGB1555 assets. Portraits appear in comms, profiles and system views; icons appear in inventory and outfitting. The portable asset compiler, source PNGs and provenance live in the project.
- Five original offline radio stations: Deep Field (ambient), Neon Transit (synthwave), Pixel Comet (chiptune), Velvet Orbit (lounge), Far Horizons (orchestral-inspired). Select on the command deck or Square in Comms opens Radio. X tunes; select Music/Effects and use Left/Right for levels; Triangle mutes music; Circle saves settings. Music fades between stations and ducks for comms/danger. No recordings, streaming, accounts or paid music services required.
- Custom station music: place MP3 files beside `EBOOT.PBP` in `music/Deep Field`, `music/Neon Transit`, `music/Pixel Comet`, `music/Velvet Orbit`, or `music/Far Horizons`. The game rescans at startup, shuffles up to 24 tracks per station, avoids an immediate repeat, and shows the detected count and current filename on the Radio screen. Copying music onto the PSP never requires rebuilding the game. Empty folders continue using the generated instrumental fallback.
- Hold Triangle for about half a second in flight to open channel control. Toggle quiet text chatter, dismiss a message, hail, request docking, open Radio/Guild or change HUD mode. Tap Triangle retains its existing context action. Important action feedback and safety prompts remain visible.
- Removed persistent coach text from ordinary menus and corrected speech-box heading placement. Quiet comms and audio levels persist in `radio.cfg`, separately from your commander.
- SpaceBook now has its own light-blue social feed, lowercase/proportional typography, profile pictures, post cards, fictional replies and local likes. X likes a post; Triangle toggles replies. No online posting occurs; reactions are session-only.
- Shipyard previews centre and scale each real mesh to fit the window through a full rotation, with a separate menu animation clock. Every purchasable hull is checked across yaw/roll angles.

Keep the existing `PSP/GAME/ELITEA` folder so commander files remain in place. The application name changes; no save reset or folder rename is needed. Retain `commander.sav`, `commander.sav.bak`, `radio.cfg` and `radio.cfg.bak` when replacing EBOOT.

Validation reports ship with the release. Physical PSP audio balance, performance, suspend and memory-stick testing remain outstanding. This release does not implement all remaining campaign/surface/ownership features in the design bible.

## New in 1.4.0 — a friendlier journey

Audio: existing procedural music and sound effects are included. Selectable radio stations are not yet included in this packaged release.

- **Guild assignments:** Triangle on the command deck opens four playable opening objectives: launch and return, deliver food, scan an anomaly, and bring a rescued pilot home. Dock and press X in the Guild screen to claim each bonus once. Triangle opens the mission board with an appropriate contract highlighted. Select opens the optional flight coach. This is an opening gameplay slice, not all twelve designed campaign chapters.
- **Mission navigation:** X in the mission log locks a local objective or plots the next fuel-safe jump. Multi-jump routes assume refuelling at intermediate hubs; open the log again after each stop to continue. Refuel first if no route is available.
- **Fair clocks:** contract time runs during active flight/surface play, and pauses in menus, police/planet choices, docking, hyperspace and while docked. Reading a screen cannot expire your delivery.
- **Protected cargo:** mission crates cannot be sold accidentally. Personal surplus remains tradable. Abandoning or expiring a cargo job returns its issued crate, preventing free-cargo duplication.
- **Recovery:** Comms, Triangle, then X confirms an emergency return to the hub with fuel restored. Cost is up to 50 units, capped at your available money. Local warrants remain. Recovery cannot interrupt surface operations, police stops, death, warp or docking.
- **Control improvements:** Circle cancels exterior docking guidance, planet turnback corrects unsafe positions, and holding menu-confirm no longer fires on return to flight. Controls now includes roll, HUD modes, Guild, navigation and recovery.
- **Open services:** the coach recommends your next lesson but no longer locks the shipyard, board or other services. Displayed Atlas echoes are renamed Meridian echoes for the original setting.

Guild progress is stored in previously reserved version-7 save fields. Existing saves import with the new assignments unstarted. Save through Save / Status at a station; this release does not add autosave. Returning to an older EBOOT and saving there loses the new Guild progress, so retain your backup.

The 12-chapter authored story, new biomes, base/freighter ownership, companions and other expansions remain planned. See [the progress ledger](docs/PROGRESS.md) for the actual implementation boundary.

## New in 1.3.9 — reliability foundation

- Saving validates the completed temporary file before replacement and keeps the previous valid commander as `commander.sav.bak`. Loading automatically falls back to that backup when the primary is missing or invalid. A failed replacement attempts rollback; an uncommitted temporary file is never loaded automatically.
- Guided docking now crosses the same checked rotating aperture as manual docking. The visible opening and collision clearance share their dimensions. Moving outward from inside the hull cannot trigger an arrival, and guidance refuses a position already inside solid geometry.
- Save payloads remain version 7; versions 1–6 still import. Keep both your commander and its `.bak` when copying saves. The previous EBOOT is preserved in `releases/1.3.8/`.
- The full game plan is now part of this project: [Design bible](docs/DESIGN-BIBLE.md). [Progress and remaining work](docs/PROGRESS.md) distinguish shipped changes from future milestones.

Validation: PSP cross-compile, game/input/steering regression suites and the PPSSPP 42-scene smoke test. Physical PSP power-loss, filesystem and performance testing remain outstanding. This is the first P0 increment, not the complete design-bible feature set.

## New in 1.3.8

- Command deck right pane shows live space. Docked, it shows the station you are in.
- Cargo prices mark ^ above or v below the galactic average so you can trade between hubs.
- Galaxy map right pane shows the destination sun and four worlds. Jump and boost both spend fuel.
- Shipyard previews the actual ship mesh. Outfitting is grouped FUEL / WPN / DEF / NAV / HOLD.
- Square+Up/Down cycles contacts inside the highlighted scanner band. Controls lists the live binds.
- Factions, Details and Comms are real pages: lock a body from Details, Venn's channel lists cheap/dear cargo, Codex lists finds by planets / flora / fauna / minerals / echoes. Collisions name what you hit.

## New in 1.3.7

- Triangle is OK on the speech box, then hail: talk to ships, Venn at the hub, and pick up a rescue by talking to them. It is no longer the warp button. Jump from the Galaxy map.
- Hold Square and Left/Right to tab Planets, Ships, Stations, Other and Enemies. Square+Up/Down are free. The targeting computer uses the same five headers.

## New in 1.3.6

- Pause actually pauses: boost, missiles, landing and EVA no longer run under the overlay. Approach and police still tick mission timers without unfreezing the world.
- Atmosphere Square turns you toward the pad. EVA hazard can kill you and shows on the HUD. Dying on a planet no longer dumps you into orbit first.
- Lasers and missiles explain why they will not fire. Abandoning a job returns the deposit. Docking still tells you if rescue or cargo is waiting.

## New in 1.3.5

- Feature pass: delivery and covert jobs actually put cargo in the hold; hunt and rescue spawn a marked ship if the system is quiet. Select opens the mission log. Start only pauses in flight.
- The long-range scanner now names distant ships. Circle on a contact locks it. Launch keeps a planet lock. Fuel scoop, refuel price and laser cooldown all report what they are doing.
- Rescue beacons cannot be shot down. The HUD bars say SHLD / SPD / HEAT / FUEL. Incoming warnings sit under the chat box instead of on it.

## New in 1.3.4

- One chat box everywhere. Flight and menus share the same portrait, name and two-line wrap. Kei, Venn, Law and the ship computer no longer dump into the footer.
- Menu copy is quieter: one job per screen, buttons stay in the footer, and the gold line is gone from the deck. Kei's chapters use natural spoken English.

## New in 1.3.3

- The whole game now speaks like a console tutorial: one next action, in complete English, on the deck, HUD, Guild brief and Kei's radio. Gold copy is the current task, not a slogan.
- Handler Kei's missing-surveyor chapters are rewritten as spoken sentences. Button names stay consistent (X, O, Square, Triangle, Select, Start, L, R). Units are units everywhere. Locked doors tell you why.
- Controls are grouped as FLY / LOOK / LAND. Pause explains SYS, ENG and WEP. Cockpit hints match the real button. Empty target filters and mission screens tell you what to do next.

## New in 1.3.2

- START no longer freezes hyperspace or docking guidance. Comms while already docked stays on the station instead of flashing into space.
- Loading a commander, launching from a hub, and finishing a jump all drop a stale target lock so you cannot lock a random new ship.
- Circle on the sun says it has no approach. Selling an empty hold, exchanging your current ship, rescue pickup, save and police fines now speak. Dying during a countdown cancels the jump.

## New in 1.3.1

- Circle on the docked command deck no longer resets the selected door. Square during hyperspace, docking or death no longer opens the targeting computer and freeze the countdown.
- Loading a commander clears Kei's intro radio so COMMANDER LOADED is the line you hear. Dead locks drop off the scanner. Ramming a marked pirate still pays the hunt; a rescue pilot stays alive.
- Chart highlight stays inside the destination list. EVA ship compass sits above the dash instead of on Kei's banner. Station panels fill down to the footer. Trade, outfitting, missions and debug tick when they succeed.

## New in 1.3.0

- Handler Kei's campaign is personal. The missing surveyor has a name: Ryn, Kei's friend. Briefings, radio, GalacticNet and the Codex speak like a late-night channel, not a tutorial overlay.
- New integer SFX: radio chirp, menu ticks, dock thump, missile, alert, death. Hits, police stops and incoming missiles finally make sound. Jetpack no longer retriggers boost audio every frame.
- Menus are tighter: gold corner ticks, thin cyan hairlines, a quieter docked motif, and a cinematic Guild brief with a NOW strip.

## New in 1.2.2

- Opening Controls now actually advances the Guild brief. The live deck was resetting the selected row before Kei saw the door, so the campaign could sit on the first lesson forever.
- Kei's persistent NEXT coach is a slim banner under the heading holos, so the stars stay visible. Speech still uses the full portrait box.
- Landing pad is cyan with a white cross and a skyward beacon, so the settlement is readable from the air. Ocean worlds keep clouds; dusty worlds get haze.
- Controls lists throttle, laser and missile. Analog toggle is L, so Square on that page no longer steals the computer lesson. Gold `*` on the deck is a warmer highlight.

## New in 1.2.1

- Systems are no longer packed with ships. A quiet star keeps a handful of contacts; a dangerous one adds pirates, not a crowd. Traders fly in toward the hub and leave. Law hunts pirates. Explorers survey a world together in formation.
- Space life sits in places, not everywhere: rock belts, cold ice belts, migrating space-whales and the odd comet. Lave keeps a modest rock belt and no whales.

## New in 1.2.0

- Handler Kei's campaign is a full missing-surveyor plot. Thirteen chapters still unlock the deck one door at a time, but every screen now tells you the next step: a gold `*` on the command deck, a persistent KEI / NEXT speech box, and a live hint on the HUD and GalacticNet.
- New commanders start on the Guild brief. X opens the deck on Controls. Food is already in the hold so the cargo lesson is not empty. Circle docks, lands or walks; Triangle opens the map, returns to orbit, or takes off.
- The EVA ship compass uses world heading, so SHIP BEHIND only appears when the parked ship is actually behind you.

## Fixes in 1.1.2

- Planetary landings are a flat field you can read at a glance: grass (or a shoreline on ocean worlds), a cyan pad with a cross, trees, bushes, rocks and a hut. Fly to the pad, Circle to land, Circle to walk.
- The old noisy column terrain is gone. Atmosphere no longer uses speed lines or HUD scanlines. On foot, nearby flora, fauna and ore are labelled.

## Fixes in 1.1.1

- Square tap opens the targeting computer only on release. Square+D-pad chords no longer open the list. Hold Square and tap a direction to cycle planets, ships, systems or the station.
- Removed the flashing amber bars in the top-left and top-right of the HUD. Menu titles, footers and Guild copy are clipped to the 480x272 screen.
- A speech box with the speaker's face sits under the heading holos. Kei, Officer Venn, Dockhand_77, Law and the ship computer use it. The Guild brief is a full four-line chapter with radio.
- Space is filled in every direction: more belt rocks, a 360-degree scatter field, and rocks around each world. Salvage and mining still work on the solid contacts.

## New in 1.1.0

- Guild Handler Kei runs a 13-chapter campaign that slowly unlocks the command deck. New commanders start on the Guild brief. Triangle opens it from the deck; Triangle on the brief skips the campaign if you already know the ship.
- Locked services show `[LOCKED]` until Kei teaches them: Controls and Launch first, then targeting, docking, system details, GalacticNet, missions, cargo, the chart, outfitting, planets, the Codex, and finally the shipyard and debug tools.
- START pauses a live power distributor. Left/Right pick SYS/ENG/WEP, Up/Down move pips. Eight pips, max four per bank. Shields, thrust and laser damage scale with the assignment. Default is 2/2/4 so combat feels like 1.0.1 until you rebalance.
- Saves are version 7 and store campaign chapter plus pips. Version 1–6 commanders still import, with the campaign marked complete so existing decks stay open.

## Hardware hotfix in 1.0.1

- Replaced the high-priority audio thread's floating-point oscillators with fixed-point integer wave generation. On a real PSP-3000 the old audio workload could starve menu rendering and controller polling, making selection changes lag and then appear frozen.
- Audio now shuts itself down safely if channel reservation, thread startup or output fails; gameplay and controls continue silently.
- Command deck now labels the current SYSTEM, local species, locked TARGET and PULSE destination on the right. In flight, Square+Up/Right/Left/Down cycles planets, ships, nearby star systems and the station. The Controls screen uses generated 16-bit pad and category icons.
- Flight uses an Elite Dangerous-style amber holographic HUD: SYSTEM and COMMS holos, a heading tape, target card, analog radar, ship hologram and SYS/ENG/WEP/FUEL pips. 3D markers stay inside the canopy. Cheap scanline, vignette and holo-tick post-processing runs on sparse framebuffer samples so it stays light on a PSP-3000.
- Mission Board and Mission Log mark accepted jobs as MISSION IN PROGRESS, including IN TRANSIT / ON SITE / RETURNING. Duplicate contracts are blocked. Triangle abandons the focused log job. UI text is a native 8x8 font on the real 480x272 framebuffer so columns cannot drift off the panels.

## New in 1.0

- Procedural cockpit music and sound effects (laser, warp, scan, land, mine, boost). Audio is silent-safe if the PSP channel cannot open.
- Command deck uses a two-pane chrome layout with a Discovery Codex. Galaxy map marks visited systems. Anomalies, wreckage and cargo appear on the scanner and in contacts.
- Analysis visor (Circle on an Atlas echo / stellar rift) and on-foot Square survey log flora, fauna and minerals. Quiet systems stay sparse; dangerous or rare stars hide extra anomalies. The Guild pays for first scans.
- Mining laser extracts ore from nearby wrecks and canisters. EVA jetpack (double-tap R), raised first-person eye clearance, a live ship compass and off-pad environmental hazard. Save files are version 6 and still import older commanders.
- Capital freighters use elongated single-pass hull rendering, fixed straight cruise headings and high-visibility engine trails. This avoids awkward turning and lowers their rendering cost.

## New in 0.9

- Circle no longer dumps you back to orbit. Triangle returns to space. Circle lands when you are slow and low over the cyan settlement pad.
- Atmosphere has gravity, so you have to fly the descent. Water rejects landing. Missiles cannot fire in atmosphere. Crashing the hull still damages shields.
- After landing, Circle starts a first-person EVA walk around the parked ship. Circle near the ship boards again. Triangle takes off, then Triangle again restores orbit.
- Death in atmosphere restores the orbital wreck view instead of leaving the camera inside the planet patch.

## New in 0.8

- Approaching an ocean or rocky world now lets you press X to enter the atmosphere and fly the generated surface. Circle still turns back to space from the approach prompt, and Circle again returns to the parked orbit position after entry.
- Surface flight uses a deterministic heightfield, ocean water where appropriate, and three landmarks (beacon, settlement, crater rim) on the scanner. Climbing high enough also restores orbit.
- Speed is limited in atmosphere. Warp, docking, targeting and salvage stay orbital. Landing, leaving the ship and walking remain future work.

## New in 0.7

- The Mission Board now offers five job types: cargo delivery, pirate hunt, exploration scan, pilot rescue and covert delivery. You can hold **five missions at once**. Each has its own 5:00 timer. The Mission Log shows every job; Up/Down sets the focused contract used by the HUD. Taking a sixth job is blocked until one completes or expires.
- Pirate hunts mark a specific hostile, exploration jobs mark a named planet, and rescue jobs mark a pilot who must be approached within 600 metres and collected with Circle before returning to the station. Delivery and covert work complete through docking at the destination.
- Mission contacts display a white `[M]` in the Targeting Computer and a larger white scanner marker. The Mission Log, GalacticNet and SpaceBook report the active objective and retain the latest completion or expiry result.
- NPC ships now have shields as well as hull strength. The targeting HUD shows both percentages, while a fitted long-range scanner adds threat, bounty and combat classification.
- R + Square locks the nearest hostile without opening the Targeting Computer. Hold L and press X to launch a homing missile at a locked hostile within 12,000 metres.
- Hostile ships can launch missiles in dangerous combat. A large countdown warning appears; boosting above four times normal maximum speed evades the missile before impact.
- Stations sell individual replacement missiles through Outfitting. The rack holds four missiles and each reload costs 100 units.

## New in 0.6

- Square opens a full targeting computer with All, Ships, Hostile, Police, Traders, Missions, Planets, Stations, Cargo/Debris, Freighters and Anomalies filters. Rows show distance and direction; Triangle opens status, faction, threat, hull and bounty details, and X locks with auto-alignment.
- GalacticNet is available in the cockpit command menu and at stations. Its News, Market, Bounties, SpaceBook, Missions and Messages tabs generate reports and pilot chatter from the current economy, danger, traffic, wanted record and mission state.
- The flight HUD now has Full, Minimal and Scenic modes. Press L + Select repeatedly to cycle them. The Full HUD includes a small GalacticNet alert panel.
- Paginated menus now display page numbers. Outfitting shows the exact numerical stat change and percentage where useful for every upgrade.
- The Mission Board shows all available jobs and the five-slot mission capacity. Mission acceptance remains available only from the board.

## New in 0.5

- Manual docking uses a real opening in the rotating station collision model. Match the cyan slot and station roll, stay below 200 speed, and fly through. Missing the opening destroys the ship rather than damaging and trapping it.
- Select > Station communicator requests guided docking within 2,500 m. The docking computer upgrade extends this to 8,000 m. Guidance flies around the hull, aligns with the rotating port, then shows an external arrival camera and “Welcome to Lave System Hub” before opening services.
- Normal maximum speed has subtle motion lines; boost has dense cyan streaks. L + Select cycles Full, Minimal and full-screen Scenic HUD modes.
- Circle approaches the solid planet currently targeted or under the reticle when close and facing it. Declining leaves the ship at the same distance, facing directly away, so the prompt cannot immediately reopen.
- Lave is fixed at risk one. Pirates fight patrols and traffic there but do not target the new commander. Other systems use visual five-star risk ratings.
- System backgrounds, nebula colours, sun colours, planet sizes and world palettes vary deterministically. Ocean worlds have moving cloud detail. Some systems contain asteroid belts or rare migrating space fauna.
- Traffic uses many Elite-A hulls, loose three-ship formations and slow capital freighters. Dangerous systems can produce larger battles; prosperous hubs gain large illuminated ring structures.
- Local system information shows economy, prosperity, technology, security and mission availability. Rich systems carry more market stock and more missions.
- Outfitting now offers refuelling, beam lasers, docking computers, shield boosters, laser cooling, cargo expansions, long-range scanners and fuel scoops. Advanced equipment requires a sufficiently high-tech hub.
- Five contract types share one Mission Board and Mission Log. The log shows the exact objective, destination, reward, timer, range and latest result.
- Wanted and danger levels use five-position visual meters. Wanted levels remain local to each system and previous commander files still import.

The scanner and contact list cover the hub, every planet and every active NPC ship. Belts, ice, whales and comets are scenery; the solid station, planets, NPC ships and belt rocks you can mine retain gameplay collision.

Retro space trading and combat with a redesigned command interface. This is a partial native Elite-A adaptation with new approximation code for NPC behaviour, not ArcElite's original AI.

## Install and saves

Replace EBOOT.PBP in PSP/GAME/ELITEA/ on a PSP configured for homebrew, or open it in PPSSPP. Keep commander.sav. This build imports version 1–6 commander files. Newly saved version 7 files store the Guild campaign, power pips, five independent missions, Codex discoveries and visited systems as well as local warrants and upgrades; older builds cannot read them. Do not copy smoke.flag from test directories.

## Getting started

A new commander starts on Handler Kei's Guild brief. X opens the command deck on Controls. The gold line is always the next thing to do; the gold `*` marks that door. Up/down still selects any unlocked door; locked rows show `[LOCKED]` until that lesson is done. O goes back. Triangle opens the brief from the deck, or skips the campaign from the brief if you already know the ship. Start pauses, assigns SYS/ENG/WEP pips, then resumes. Select opens the deck in flight.

- In flight, Cargo inventory shows only what you own. Station market stock, prices, ships and equipment are available only while docked.
- At a station, Cargo & station market has a scrolling commodity list and item details. Right buys one unit, left sells one. Balance and cargo capacity remain visible. Restricted purchases are labelled as crimes.
- Warp: choose a system marked READY and press X. From the station this launches and warps in one action. Fuel is required; refill in Outfitting.
- System & ship contacts lists every active NPC, all planets, the sun and CORIOLIS STATION. Ships have numbered identifiers. X locks a contact; Triangle locks and auto-aligns in flight.
- Commander shows your ship, cash, missiles, local wanted level, active mission and save/load controls. Saving and loading require docking.

## Flight controls

- Nub / D-pad: steer. Centre the nub after launch. Manual steering cancels auto-alignment.
- L / R: slow down / accelerate.
- Double-tap R within 0.32 seconds, holding the second press: boost up to 20 times normal speed. Release R to brake.
- Double-tap L while moving fast: hard brake that dumps speed quickly.
- Heat rises from overspeed, boost and flying near the sun. Critical heat locks boost and lasers; max heat destroys the ship. Cool by dropping boost, leaving the star and resting the guns.
- Hold L + D-pad left/right: roll the ship without changing throttle. Camera, steering and compass respond to roll.
- X: fire laser.
- Square: open the targeting computer. The list is grouped under Planets, Ships, Stations, Other and Enemies. L/R tabs those headers, Up/Down selects, Triangle shows details, X locks and auto-aligns, and Circle returns to flight. Hold Square and Left/Right in flight to tab the same five bands. Hold Square and Up/Down to cycle contacts inside the highlighted band. Hold R and press Square to lock the nearest hostile immediately.
- Hold L and press X: launch a missile at a locked hostile within 12,000 metres. Ordinary X fire remains the laser. Boost above four times normal speed to evade an incoming missile before its countdown expires.
- Circle while looking at or targeting a nearby solid planet: approach from within 1,000 metres of its surface. Proximity alone does not open the approach screen. X enters atmosphere flight; Circle turns back to space from that prompt. On the surface, Circle lands or walks; Triangle takes off or returns to orbit.
- Circle near the station opens guided docking. For manual docking, match the cyan rotating entrance, keep speed at or below 200, and fly through the slot. Flying into the surrounding hull destroys your ship and triggers a 3D wireframe debris explosion. Press Start for a new commander after destruction.
- Triangle: OK the speech box, then hail whoever you are looking at or have locked. Rescue pilots come aboard when you talk to them. Warp lives on the Galaxy map. Select: command menu. Start: pause.
- Controls screen: L toggles analog steering. D-pad remains usable.

## Wanted levels and police

Wanted levels 1-5 apply only to the system where the offence happened. Shooting protected ships adds five crime points; buying restricted slaves, narcotics or firearms adds one. Each five crime points raises a wanted level, up to five. Higher wanted levels dispatch two additional police per level.

A police ship within 650 metres intercepts a wanted player, stopping all simulation and opening a mandatory choice:

- X pays a fine of 50 units per wanted level and clears the local warrant. If you cannot afford it, the choice stays open.
- Circle accepts station custody. You are returned to the local station, charged a release bribe of 25 units per wanted level (capped at your available money), and released with the local warrant cleared.

Other systems do not inherit that wanted level and police ships do not follow through warp. Returning to a system restores any uncleared warrant there. All local warrants are saved.

## Debug menu

Select > Debug tools provides: add 1,000 units; refill fuel and repair shields; clear or raise the local wanted level; move to the station approach; move near the selected planet; and dock immediately. Changes affect your current commander and are saved if you save afterward.

## Space and navigation

The scanner displays all active ships, planets and the station using compressed distance scaling; colours match the faction guide. The compass points to the selected contact, including behind you. Names in flight match the contact list. The header explicitly labels the current system and its danger rating.

Danger 1-5 controls pirate numbers, pursuit range and firing frequency. A system only keeps about eight to eighteen civilian ships alive at once: traders inbound and outbound, a Law pair hunting pirates, explorers flying a shared survey, and maybe one slow freighter. Twelve extra police slots still wake if you are wanted.

Each system contains a sun, ocean world, gas giant and two rocky planets. Rock belts, cold ice belts, migrating space-whales and comets appear in some stars, not all. Coloured stars, nearby dust, boost streaks, sun lens flares and an animated warp tunnel preserve the retro presentation. Collisions use simplified volumes with swept player checks to prevent high-speed tunnelling.

## Planet exploration

Circle opens the approach choice near an ocean or rocky planet. X enters a flat surface: grass, trees, bushes and a cyan pad with a cross. Descend onto the pad and press Circle to land, Circle again to walk. Ocean worlds keep a grassy island around the pad and water beyond it. Square on foot surveys flora, fauna and minerals. Board the parked ship, Triangle to take off, Triangle again for orbit. Double-tap R while walking uses the jetpack. Walking too far from the pad still builds environmental hazard.

Suns and gas giants have no landing approach. You cannot land on water. Quiet systems stay sparsely populated; Atlas echoes and extra life appear in rarer or more dangerous stars.

## Verification

Compiled with PSP GCC using -Wall -Wextra without warnings. 134 gameplay, 52 interface/input and 13 steering checks passed inside PPSSPP (199 total, 0 failures). Average 59.94 FPS across 42 scenes; worst frame 16.74 ms. Physical PSP-3000 confirmation is still required.

## Build

From PowerShell:

    ./build.ps1 -Toolchain C:/path/to/pspdev

The default toolchain location is ../../work/toolchain relative to this project. It was downloaded from dmang-dev/pspdev-win release v2 and is not included in the source zip.

To regenerate the geometry with Node.js after fetching the source repository into elite-a/:

    node tools/extract-ships.mjs

The generated src/ships.c is included, so regeneration is optional when building.

## Source and credits

Original Elite: Ian Bell and David Braben; copyright Acornsoft 1984. Elite-A additions and blueprints: Angus Duggan. Documented source and commentary: Mark Moxon. Original rights and notices remain applicable; this project does not relicense that material.

- Elite-A source: https://github.com/markmoxon/elite-a-source-code-bbc-micro
- Elite-A documentation: https://elite.bbcelite.com/elite-a/
- Author's feature documentation: https://knackered.org/angus/beeb/elite.html
- PSPSDK: https://github.com/pspdev/pspsdk
- Portable compiler: https://github.com/dmang-dev/pspdev-win/releases/tag/v2
- Emulator: https://github.com/hrydgard/ppsspp/releases/tag/v1.20.4

src/game.c implements a partial native gameplay adaptation. Galaxy/economy and market data follow the documented Elite-A routines; combat is new approximation code, not ArcElite's original AI. src/ships.c is generated from Elite-A blueprint macros. src/main.c is the new PSP frontend.

## Remaining work

Richer EVA (slopes, sites, pickups), surface audio, additional Elite-A systems, and physical PSP performance and save/suspend testing.
