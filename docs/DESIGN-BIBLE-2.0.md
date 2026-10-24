## Staged contracts — 2.5.388

Forty additional contracts with 155 real objective stages, branching decisions,
animated paused contact updates and reviewable contact records. Existing
contract families and winding arcade races remain. Tasks use actual combat,
escort ships, marked salvage, extraction/refining, cargo purchases, observation,
surface sites and scans, discoveries, anomalies and championship races.
Progress, required cargo, choices and ship health persist in V33 saves;
V1-V32 imports remain supported. New saves need this version or newer.

Title music uses Elite Exploration independently of saved radio preferences,
skips exactly 23 seconds (2.5.384 timing), then fades in over one second.
Rock FM and races use music/Sol Rock beside EBOOT.PBP. Actual Secret Agent
Rock.mp3 (48 kHz, large embedded artwork) now decodes in emulator testing.
Metadata skipping and modern PSP MP3 decoder support fix that file's failure.
No MP3s, saves or private settings are included.

See STAGED-CONTRACTS-388.md and MISSION-STAGES-388.md.

## Winding arcade races — 2.5.387

32 ordered physical gates replace the almost straight twelve-gate sprint.
Seeded courses cover roughly 900 km with repeated S bends, climbing turns
and descending chicanes. A starter Adder finishes in about two minutes using
normal steering, throttle and double-tap-R boost. Bring your own ship: its
actual speed, handling, acceleration, installed modules and fuel still apply.
Gold boost/cooling rings on every leg support sustained arcade racing.
Four scenery sections: asteroid slalom, split freighter wrecks, floodlit
port grandstands/pylons and ice fields, with open racing lanes and trail lamps.
5-10 physical rivals include clean racers and opportunistic shoulder-checkers.
Swept relative-motion collisions catch high-speed rams between frames.
Gentle contact separates the ships and costs speed; hard/boosted impacts
break the rival's actual hull mesh into spinning debris, sparks and a shockwave.
Destroyed racers leave the leaderboard and cannot finish or take the prize.
A bounded 1.2-second slow-motion orbit frames the takedown, then a 0.6-second
swing returns to the actual first/chase/cinematic camera. Safe autopilot runs
through the shot and for one further second afterward, using real gates.
Every racer shares the slow simulation; menus pause the camera and actors.
Held boost resumes on return if still held; there is a cooldown between shots.
Targeting browser, quick-target and target-lock combos are disabled while
racing, including the menu route. Square never steals R throttle during races.
Race HUD shows section, next gate distance, place, time and takedowns.
First place alone earns the existing actual mission prize once. Free retries
reset every rival, effect, gate and camera. Five-minute safety timeout.
Race music uses the MP3 library in music/Sol Rock beside EBOOT.PBP (Rock FM).
Playback works even when ordinary radio music is off, respects Quiet Mode,
does not apply the intro-song skip and restores previous radio preferences.
No MP3s, saves or private settings are included in this package.
Existing V32 save compatibility, mission roster and law enforcement remain.
The 21-second intro music start/fade and SDK display-startup repair remain.

Exact release validation:
INFO production flight: phase 5 gate 32 place 1 time 119.85 heat 0.0 fuel 47.1 hull 100.0
INFO dense race draw: 730 max submitted triangles, 55.89 emulator render FPS (no hardware guarantee)

## Intro music timing — 2.5.386

Elite Exploration starts at 21 seconds with its existing one-second fade.
Restores only the latest two-second cut; earlier timing edits and cinematic
are unchanged. Exact decoder samples and normal boot pass emulator checks.

## Space racing — 2.5.385

The mission board preserves its five contracts and adds a sixth local Space Race.
Accept, launch, then navigate from Tracked Mission, or select RACE START GRID
in the targeting computer's OTHER category. The published waypoint remains
available beyond sensor range. Enter within 450 m to line up automatically.
Checkered physical starting gantry, 5-10 varied solid rival hulls, four-second
READY / 3 / 2 / 1 / GO countdown with audio cues and 12 ordered 3D checkpoints.
Use normal steering, R throttle, L brake and double-tap R then hold for boost.
Gold rings inject speed without reducing higher velocity, cool the engine
and provide two seconds of free powered coasting. HUD shows place, elapsed
time, checkpoint count and direction arrows, including targets behind you.
First place earns the actual advertised prize once, through Race Control's
mission-completion transmission. Losing/three-minute timeout offers free
RETRY / LEAVE. Retry resets every checkpoint, boost gate, clock and rival.
Lasers and missiles are locked; existing police/warrant enforcement remains.
Menus pause the simulation. Rock FM temporarily overrides music using the
existing music/Sol Rock folder or generated rock fallback. Original tuning
and power settings are never overwritten; Quiet Mode continues to apply.
Finish, abort or loading restores the original station/settings automatically.
Existing V32 saves retain accepted entries and their originating hub. Loading
rebuilds the original grid and resets attempts; ordinary saves stay dock-only.
All 768 seeded system/hub courses validate clearance from canonical planets,
rings and stations. Rendering is bounded to three upcoming checkpoints,
two boost rings and ten nearby solid hulls. No extra framebuffer is allocated.

Checkpoint completion uses swept directional plane crossings and an aperture
intersection test, preventing tunnelling, reverse crossings and skipped gates.
Rivals physically steer/accelerate toward apertures; rank compares real route
progress and interpolated finish times. Race attempts are transient; V32 Job
type and item retain mission identity and station. No save-format migration.
Existing missions, station services, law and shared-world identities remain.
Emulator integration, real MP3 switching, native screenshots and all 768
clearances are verified; physical PSP testing remains required.

## Mission completion and PSP startup — 2.5.384

Every committed contract/assignment payout records one session-only receipt: canonical title, contact, issuer and actual amount paid. The fullscreen received-transmission card freezes gameplay, uses the existing contact portrait identity, wraps native 8×8 text into reserved regions, and shows actual credited units. Warp/slide, signal and fireworks animations cannot cover the message. X acknowledges one card after entrance and release; no input leaks into firing or menus. Simultaneous dock payouts queue separately. Abandon/failure and ordinary initialization do not celebrate; loading preserves paid progress without reissuing cards. Police, docking and hyperspace cinematics take precedence. No save-format change or additional framebuffer.

Cold boot uses full PSP SDK display setup and an initial NEXTFRAME latch, both planes precleared black, with first movie preparation offscreen. This restores omitted setup after the physical PSP black-screen report; actual hardware confirmation remains an explicit release gate. Keep the existing 23-second audio skip, one-second fades, CuLa tiles and cinematic asset.

## Intro startup presentation — 2.5.383

Cold boot publishes black only after both full framebuffers are initialized; first cinematic decoding occurs offscreen. No hidden setup communication sound. CuLa Games presents... and its red/blue letter tiles fade in over one second. Elite Exploration startup skips 23 seconds using actual decoded source samples and keeps its one-second audio fade. Select replay retains these rules and existing radio preferences. No new framebuffer or save format.

<!-- 2.5.382 -->
Implemented departure presentation: illuminated rich ports and deeper capital districts with solid structures, transit bridges, adverts, window banks and passing ship meshes. Bounded camera-local detail, aperture clipping, no added simulation NPCs or runtime allocations; five-second launch and input handoff unchanged. Station departure/docking and planetary transfer share a slowly pulsing green AUTOPILOT ENGAGED label; redundant stage/guidance captions and PSP-resume message removed. See DESIGN-BIBLE.md. Physical PSP performance review remains pending.

<!-- portrait-audit-365 -->
Commander portraits require authored per-pixel material regions, neck/collar separation, aligned anatomy, useful foreground changes at 20/32/96px, real save/input checks and exhaustive combination rendering. Distinguish automated coverage from manual visual review. See COMMANDER-CREATION-365.md.

<!-- 2.5.365 -->
Implemented native 480×272 title cinematic; continuous cloud flight, sunlit station reveal, apex effects, separate crisp UI and X menu fade/release guard. Embedded 30fps film, bounded frame memory. Details: CINEMATIC-INTRO-365.md. Existing music/settings retained; physical PSP and whole-game certification remain open.

<!-- 2.5.363 -->
Hair palette corrections must follow reviewed pixel anatomy, not just self-consistent mask bounds. Human/Android scalp contours and independent landmarks replace the rejected 2.5.362 polygon. Details: PORTRAIT-HAIR-MASKS-363.md.

<!-- 2.5.362 -->
Commander Creation: aligned authored feature parts, explicit palette masks, supported controls only; unchanged saved legacy appearances, v2 new looks in the V32 field. Verification and native comparisons: COMMANDER-CREATION-362.md.

<!-- 2.5.361 -->
Implemented checkpoint: paint-themed 270 ms pulse/grow Command Deck openings, interruptible input, and corrected negative-coordinate planetary prop placement. V32/generation v1 retained. Details and remaining regional art/performance gaps: PLANET-REGIONS-V1.md and MENU-TRANSITIONS-361.md.

## 2.5.358 — Observation Lounge Command Deck

Built on local 2.5.357, preserving its Multiplayer console and prior native UI updates. Implements approved 480x272 6D layout: original logo, green docking status with animated ship icon, glass service buttons, distinct teal category focus and amber service focus, native readable paintbrush/spanner icons, bounded description card and fixed footer funds. Left/Right and L/R shoulders slide between remembered categories; service openings flash briefly, slide and reveal the destination within 340ms. A new input interrupts cosmetic transitions without delaying Back. Station art stays still with twinkling port lights; star glints, nebula glow and restrained native sun shimmer animate. Opening highlights now follow the actual new row geometry. Real availability, actions, networking and V32 saves remain authoritative.

Implementation/coordinates: docs/COMMAND-DECK-358.md. Final validation, captures and EBOOT hash: BUILD-2.5.358.txt. Highest-priority remaining work: inherited broad failures and physical PSP input, sleep and two-device tests. This update is not whole-game certification. GitHub DNS is unavailable; preserve the mixed shared checkout and do not publish unrelated work wholesale.

## 2.5.357 — Multiplayer Retro Terminal

Implements the approved native 480×272 Multiplayer console on local 2.5.356. One Fly entry and title Square open online connection and local Host/Join together. CONNECTED keeps a plain red disconnect X; real PSP action icons stay in the footer. Server connection stages are truthful, peer names and locations are bounded, and local flight codes come from real advertisements. Predictable four-region D-pad navigation, explicit cancellation, No-default transport/end confirmations and caller-specific returns are implemented. Online/local exclusivity, settings retry and sleep cleanup use the existing transports; protocol and two-pilot server capacity remain unchanged.

See docs/MULTIPLAYER-357.md and BUILD-2.5.357.txt for final verification, native captures, artifact hash and limitations. Preserve unrelated shared work and player music. Existing broader game and planetary Wi-Fi failures remain baseline issues; physical PSP/two-device testing is pending. Local build only; no GitHub publication.

Verified local EBOOT: `68ED085740E86B135A29E8D25833AB6EC73B8BD0EC9B8831C95BAA065A7C5564`. Final Multiplayer console (58), online fixtures (12), real server connection (7) and unchanged server (13) checks pass. All 30 native states inspected; 93 recent UI checks pass and 120 prior captures remain byte-identical. Core retains the same 15 failures; local planetary Wi-Fi retains the same three. Highest-priority remaining work: those inherited failures and physical PSP/two-device/sleep testing. Local release only; GitHub is not updated. Full evidence: BUILD-2.5.357.txt.


## 2.5.356 — Shipyard showroom standard (implemented)

Orbit Motors is the compact regular workshop. Crown Grand is the panoramic Mega showroom with a larger hero scene and distinct dealer branding, site artwork and actual stock. Every capital dealer receives its own deterministic name and four-hull collection. All controls and typography are designed for native 480×272. Square toggles complete prose and six comparable bare-hull stats, with gold category labels and small green-up/red-down arrows. EXCHANGE YOUR SHIP sits above COSTS: or YOU RECEIVE:; balance remains bottom-right. The catalogue scrolls within its own bounds, the actual painted hull is fitted to its aperture, and existing purchase validation and celebration remain shared. Lave keeps all hulls for testing. See docs/SHIPYARDS-356.md and BUILD-2.5.356.txt. Existing broad game failures and hardware gates remain open; no save-format change.

## 2.5.350 - Local newspapers and playable paper puzzles (candidate)

NEWS now loads one fixed edition per canonical galaxy system, plus the separately approved four-page Earth tutorial paper. The 256 galaxy editions contain 3,082 pages with lengths of 4-24; Lave has 16. Sixteen native arrangements, sixteen publication families, 24 readable palettes, distinct masthead registers, bounded captions, native illustrations, paper turns and Sudoku/crossword input are implemented. Puzzle progress saves separately in small checksummed files. Other GalacticNet sections retain their existing code and controls.

Lave has an original complete issue following the Night Shift Circle's lamp dedication, with reports, letters, adverts, corrections and a credited Nip and Tug comic. Seven other major systems have original front-page material. The rest is a fixed bank-derived DRAFT corpus: the full 256-paper bespoke writing/art handoff is NOT complete. The audit explicitly counts repeated printed paragraphs; unique metadata is not editorial approval. Every final page still requires an individual recorded native inspection.

Verification: final EBOOT newspaper checks RESULT 0 failures, including all 3,086 native pages and puzzle persistence/navigation. Market isolation is byte-identical in five states; Spacebook/Inbox and Wanted checks pass. Fifty-three actual paper pages and two enlarged puzzle views inspected; 3,033 individual paper inspections remain. Broad game failures exactly match the 15 pre-change failures; radio and steering pass. Physical PSP certification and full bespoke editorial/art acceptance remain open.

Final local EBOOT SHA256: 51DF45568F12828DC36097118DA26534896EF497659A836A3491BAB4FBCAF504.
Evidence: outputs/newspapers-20261009-224919-284; content/newspapers/test-evidence-350.json and native-review-350.json.

See docs/NEWSPAPERS-350.md for source ownership, controls, test evidence and remaining gates. Do not publish this candidate as the completed newspaper expansion. Physical PSP certification and the inherited broad game failures remain open. No GitHub publication.

## 2.5.345 - A personal shipyard congratulations card

Successful shipyard purchases now open a warm delivery card bearing the new hull name, native-size animated handwriting, restrained pixel fireworks and a shipyard seal. Six messages vary without immediate repeats, including "Enjoy your new ride!". X finishes the handwriting, then continues; Circle closes the card. Purchase input is guarded and the card blocks underlying shipyard actions, so it cannot buy twice. Failed or already-owned exchanges show no celebration. Credits, cargo, fitted modules and Item Storage retain the existing exchange rules; no save change.

## 2.5.344 implementation - Guild contract availability

The Guild food-delivery and rescue onboarding steps reserve a real required family on each five-card board when normal seeded offers omit it. The final card alone is replaced, without duplicate families. Board generation and Guild lookup share mission_type_at(system, hub, offer). Docked local guidance opens and highlights that offer; local-flight guidance selects the primary hub for docking, and accepted work opens its existing log entry. Save schema is unchanged.

## 2.5.341 - Compact planetary spaceports and EVA rendering

Planetary spaceports now use an 840m-wide apron instead of roughly 1.4km. Services, low terraces, the terminal and rover garage are grouped closer together, with fewer apron lamps and shorter roads. The shared geometry drives rendering, collision, landing reservations and the local map. The player pad retains clearance for the widest and longest hulls; two staggered traffic berths remain.

EVA retains the exact existing terrain mesh and 2x2 depth raster. Ground grain beyond 24m shares shading on a 4x4 screen grid; nearby props, signs, portraits and HUD retain their native detail. Distant dry ground avoids unnecessary texture reconstruction. Lave vegetation reuses tagged cells when moving between overlapping windows rather than regenerating the entire region. Quad submission transforms four vertices rather than six.

Verification and timing evidence are recorded in docs/EVA-PERFORMANCE-341.md. Physical PSP frame rate remains to be measured. This is a local private candidate; it is not a GitHub release. Existing broader regression failures remain open.

## 2.5.301 - Distinct stellar-system archetypes

The fixed five-body template is replaced by twelve deterministic structural families across the 256-system galaxy. Systems can now contain a solitary star, a gas giant with eight moons, three gas giants, asteroid-heavy planetoid fields, binary or trinary stars, frozen red-dwarf worlds, scorched dead rocks, crowded habitable worlds, rogue planets without a conventional star, gas nurseries, and the classic mixed arrangement retained by Lave.

The runtime supports up to ten celestial slots while inactive slots remain absent from rendering, collision, sensors, traffic, freight and discovery placement. System Details, the Codex and operations guidance derive their counts from the actual local layout. Secondary stars are treated as stars rather than landing targets. Rogue-system slot zero is an unlandable navigation giant, preserving the established landing/save bit layout while the remaining rogue worlds are explorable.

Mission Validation now routes exploration offers only to reachable systems containing a real landable world and selects the objective from that generated layout. All five mission types remain available in every system. Automated coverage checks all twelve archetypes, active/inactive slots, defining star and world counts, and all 1,280 generated mission offers.

Save format V28 widens landing masks and stores ten surface-progress slots per system while migrating V1-V27 saves from their original five-body records. Local Wi-Fi protocol 263 carries the expanded world layout and rejects malformed inactive slots.
## 2.5.285 - Fitted-ship interface identity

The fitted ship finish is a cockpit-wide identity layer. Galaxy.Maps and the physical Comms Panel use its shared panel, raised, edge, accent, signal, text and muted roles at native 480x272. Warnings and PSP button identity remain semantically stable so cosmetic themes never weaken usability.

## 2.5.284 - System Almanac label contrast

The selected map object's name uses dark translucent glass at roughly 70% opacity. The underlying system illustration remains perceptible, while the name, selection brackets, connector and cyan frame remain readable against every stellar-body colour.

## 2.5.283 - Complete tree sprites and Far Horizons

Fixed vegetation extraction: the source trees overlap the atlas's nominal equal-width columns. Individual authored source rectangles now preserve complete canopies and exclude fragments of adjacent trees/ferns. The same corrected 48x64 sprites serve tutorial Earth, Lave I and the meadow's distant groves. Wind remains enabled. Renderer sampling rejects invalid cells and out-of-bounds coordinates before integer conversion, with samples aligned to the terrain's 2x2 pixel grid.

The second planetary atlas is corrected too: individually bounded silhouettes replace uniform 4x6 cuts, restoring the forest canopy and removing adjacent shrub/rock fragments. All 24 biome props now have distinct pixel ownership; their native preview matches the compiled PSP palette and alpha. Both atlases retain transparent gutters. Source PNGs remain unchanged.

Far Horizons remains the startup default and is now the tutorial radio choice and the station acquired on arrival at Lave. The radio lesson accepts other manually chosen stations too. Existing saved preferences remain respected. The dream's temporary Sol Rock battle score is unchanged. User MP3s stay in music/Far Horizons; no music or save files are replaced.

Private local build. See BUILD-2.5.283.txt for exact-EBOOT checks and remaining hardware limits.

## 2.5.282 - Meadow edges and distant countryside

The tutorial meadow repaints its complete header and footer every frame. The old gaps above and below the 3D viewport could retain parts of Dad's portrait after dialogue closed. Regression coverage now renders over previous dialogue without the screenshot helper's full-screen clear, including all HUD modes and both live VRAM buffers.

Removed cockpit bloom from the meadow: its in-place brightening turned the pale horizon into a speckled white strip and cropped edge shading at cockpit height. Sun and stone retain their local effects. Terrain rejects fully offscreen tiles above and below the view before rasterization.

One connected ground mesh now reaches sixty kilometres, with additional field detail outside the playable clearing and a pitch-correct land-haze horizon. Distant pastures and 96 trees in irregular groves include twelve taller landmarks. Far trees use the displayed mesh height so they stay planted on the slopes. Near walking terrain, collision, flower trail, optional conversations and orb choices remain compatible with existing checkpoints.

Private local build. Verification results and physical-PSP limits are recorded in BUILD-2.5.282.txt. This meadow update does not yet restore the standard cockpit HUD in the dream; that requested consistency correction remains next.

## 2.5.281 - Empty-space dream battle

The opening eyes now blink and open in 1.6 seconds. Three Thargoids arrive together in an arrowhead, close in, break away and turn back into attack runs. Their positions advance continuously in world space; they are no longer repositioned around the camera. Bounded, aimed projectiles use swept collision, so a missed shot causes no damage. Player steering, throttle, braking, lasers, heat and shields retain the normal flight implementation.

The dream takes place in empty space, without planets, stations, civilians, freighters or ordinary mission/law displays. Dream hits cannot create crimes, loot or bounty rewards. A dedicated Night Shift radio ticker supplies two minutes of surreal stories that hint indirectly at the dream. SOL ROCK starts with the attack and fades while waking; saved radio choices remain unchanged. Waking occurs after defeating all three ships, reaching low hull, or 120 seconds. No save-format change.

Private local build. See BUILD-2.5.281.txt for exact-EBOOT verification and hardware limits.

## 2.5.276 - Varied station greetings

Twenty friendly station greetings plus context-specific distant, wanted and already-docked replies. Per-station rotation avoids consecutive repetition; each hub has a stable operator portrait. Captions say DOCK CONTROL. Hails remain in flight and do not grant docking, clear warrants, or bypass existing docking/security rules. Hold Triangle still opens the console; tapping hails/closes/quick-replies. Focused tests cover greeting variety, readable length, range/warrant conditions and no automatic docking. Private local build; physical PSP and legacy broad regression remain outstanding.

## 2.5.275 - One canonical computer portrait

The authored intro computer bitmap (opening_portrait6) is now the shared ship_computer_portrait renderer for the opening, cockpit computer notices/automated inspections and full dialogue computer portraits. Removed the different procedural robot face from these paths. Native nearest-neighbour rendering retains the same square face at 28/32/48/56 pixels. A pixel comparison checks that intro and cockpit render identical 32px output. Private local build; physical PSP and legacy broad regression remain outstanding.

## 2.5.274 - Discovery speakers and Triangle shortcuts

All 75 discovery entries now have explicit compact speaker identities. Mobile Mechanic speaks as MECHANIC, animal transport as KEEPER, and abandoned/silent objects use COMPUTER or RECORDING with the computer portrait. Ordinary transmissions reset the source marker so later speakers cannot inherit the previous discovery identity. Both flight captions and conversation rendering use the same source lookup.

Station hails no longer open the Comms settings console. Tap Triangle hails/closes or opens the quick reply chooser; hold Triangle for 0.55 seconds opens the paused console. Private incoming calls also use the quick chooser instead of jumping straight into the panel. Confirming RESPOND can still open the existing conversation, separate from console settings. Focused regression includes all catalogue speaker labels, mechanic/computer identity, stale-source clearing and tap-versus-hold station input. Private build; preserve shared edits and do not publish automatically. Physical PSP and legacy broad regression remain outstanding.

## 2.5.273 - Proportional translucent rings and contact sensors

Rings now span approximately 1.4-2.6 planet radii with half-opacity coloured material, density bands/gaps, soft annulus edges, globe concealment and planetary shadow; shared asteroid placement follows the smaller geometry. Sensor range uses actual scanner equipment (10/20/30 km for normal contacts, larger signatures for stations/freighters). Planets, suns, intersystem routes and mission docking waypoints remain navigation references. Out-of-range selected ships lose alignment and retain their last seen position. Locking/proximity no longer identifies ships. Close Triangle hail identifies them; close debris inspection preserves existing cargo recovery. Discovery/anomaly identities have per-commander checksummed sidecar files. NPC identification remains attached to the live NPC; new spawns reset it. Bounty selection no longer names/auto-aligns the suspect or highlights unidentified bounty ships. See docs/RINGS-SENSORS-273.md. Private local build; physical PSP testing and legacy broad regression remain outstanding.

## Space expansion implementation boundary (2.5.272)

Lightweight named discoveries use short hails, bounded seeded placement and existing targeting/comms. Lave and three development systems only. The 75-entry catalogue is authored data; bespoke art and full galaxy rollout are not complete. Follow SPACE-EXPANSION-272.md for the remaining traffic, liner, scale, hyperspace and fuel milestones. Decorative names do not imply station services.

# ELITE: NEXT 2.0 — PSP GAME DESIGN AND PRODUCTION BIBLE

**Comms console, build 2.5.271:** one native 480x272 console, no scroll or tabs. Triangle tap retains quick flight contact; hold opens paused controls. D-pad navigates neighbouring hardware controls, X operates, Circle returns. Radio tuner uses X next and shoulder L/R previous/next. Contact becomes Answer Incoming for actionable calls. Unavailable controls stay selectable with reasons in a reserved information strip and refuse activation. Music, optional chatter, captions, HUD and camera are directly controlled; Quiet Mode restores prior music/chatter choices. Essential mission and danger warnings remain available. Mission Log, salvage EVA and controls help remain accessible; station walking and contrast have been removed from this panel by user request. No message history, identify-target or volume controls are added.

**Planetary rings, build 2.5.270:** gas worlds have fly-through tilted rings in flight space, outside the landing boundary. Rendering and mining objects share seeded geometry. A bounded set of physical asteroids can be fractured and collected; fine ring grains are cosmetic. Seeded cargo and wreckage lie along interplanetary routes. Future locations should connect to missions, discoveries and local economies; the fifty-location idea list is planned content, not a shipped feature claim. Preserve clear routes and PSP frame/memory budgets as density increases.

**Adventure presentation, build 2.5.269:** opening conversations, station and planetary-site reading panels, mission log and tracked/incoming conversations share solid charcoal framing with restrained blue information accents. Player speech/replies use warm orange accents. Keep native 480x272 artwork viewports, text pagination and controls unobscured. The cockpit's compact top-right radio ticker remains separate. Dream attacks temporarily play SOL ROCK at the configured music level; fade across waking and restore the existing radio selection without writing preferences. Physical PSP review is still required.

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
# Mission Network implementation status — 2.5.304

The confirmed 26-family repeatable mission pool is now the canonical Mission Board catalogue. Each station presents five distinct deterministic offers and validates its destination, route, station and any landable world before display. Expanded family identity persists in the existing `Job.type` save field; each family currently delegates to one of the five proven objective runtimes (cargo, hostile, survey, contact or covert) so every visible offer remains completable while specialist multi-stage mechanics are developed incrementally.

The next mission-network phase adds station-owned one-hour replacement timers, explicit equipment gating, permanent failure cleanup, urgent active-play timers, optional bonuses and the dedicated completion/failure presentation. Convoy, repair, salvage, wildlife, archaeology, mining, distress, patrol, missing-person, mechanic, black-box, listening-post, passenger, multi-stop, freighter-defence and derelict families then receive their authored multi-stage runtimes without changing their saved family identities.

## 2.5.339 - Performance certification slice

Implemented: render-scoped mission batching, direct deterministic world-availability queries, a bounded ten-body planet-art cache, conservative visibility rejection, cached mesh bounds, shared hull transforms and screen-size-based decorative effects. Render culling must never affect live traffic, collisions, sensors or objectives. Small engines retain luminous world-space wakes while close-up flame particles have a fixed budget. The filtered nebula wash uses 2x2 writes; text, stars and planetary art stay at native resolution.

Verification: use `tools/review-performance.ps1` for galaxy-wide mission equivalence, exact level/rolled planet-cache comparisons, edge/near-plane visibility and six workload timings/captures. Nominal CPU budget is 33.42 ms including a crowded simulation tick. Physical PSP FPS, MP3 contention and cold-cache/system-entry behavior remain hardware release gates.



## 2.5.340 - Hull-relative chase view

Center the chase camera behind and above the current ship. Camera, hull and exhaust/weapon anchors share the same full orientation through complete rolls. Scale following distance to the actual hull; reuse capital preview geometry rather than a fallback starter hull. Depth-test exterior effects against solid geometry.

Verification: PSP build succeeds. Final exterior review reports RESULT 0 failures across 8,514 hull/pitch/roll combinations, both engine/weapon anchors and foreground depth rejection. Native 480x272 level, quarter-roll, half-roll, full-roll and capital-hull captures inspected. The performance review reports RESULT 0 failures; crowded third-person render plus simulation averages 32.850 ms on the emulator. Real PSP performance and subjective handling remain to be tested. Existing broader regression failures remain recorded in the 2.5.339 handoff; complete game certification remains open. Preserve shared uncommitted changes; this local build is not a GitHub release.

Evidence: outputs/exterior-final-20261009-174150/exterior-review.txt and outputs/performance-review-20261009-174152-575/performance-review.txt. Run tools/review-exterior.ps1 for repeatable focused checks. The chase view uses the player's complete orientation without Euler-angle smoothing; cinematic continues to ease between its own shots. Capital-hull parts reuse the menu geometry, transformed into world space; ordinary hulls use their mesh faces. Exterior shots are depth-tested world-space beams/packets, while first-person weapon effects remain unchanged.


## Combat response contract (2.5.347)

Damaging a pirate provokes self-defence even in peaceful onboarding systems. Engaged raiders prioritise their attacker within a bounded 12 km leash, fly attack passes and deliberately retreat when crippled or outnumbered. Retreat preserves poster identity and reward eligibility. No ambient patrol rule may silently cancel a healthy raider's retaliation every frame. See [NPC-COMBAT-347.md](NPC-COMBAT-347.md) for thresholds and integration checks.


## Wanted poster interaction (2.5.348)

GalacticNet Wanted is an informational warrant board. Browsing or pressing X never launches the commander or selects a hidden suspect. Open posters show LIVE WARRANT, with no X Track prompt in either poster or footer. Suspects are found through normal local sensor detection and close-range identification; names, portraits, risk, bounty and completed status remain visible on the board.


## Spacebook and story correspondence (2.5.349)

Spacebook's News Feed is a fictional Facebook-style homepage at 480x272: blue wordmark masthead, commander profile/navigation rail, a complete readable NPC activity card, event evidence strip and a small local-pilot sidebar. It uses the same durable NPC event timeline before and after the opening. No Earth-friend posts, commander first-ship posts or pinned story evidence may displace NPC activity in the feed. Existing event names, reactions, timestamps and save history remain intact.

Inbox is the private story archive. Scott's party post/photo, Kat's first-flight and Friday messages, and the commander's first-ship note belong here alongside Dad and Mum. Reached Homeward milestones, unlocked First Light briefings and current/completed Open Channel chapters are reconstructed from existing saved story progress. Future chapters are excluded; archived briefings are labelled as correspondence rather than given invented historical timestamps or fabricated choices. Scott's photo is opened with Square and closed with Circle. Dad's unreachable Earth relay retains the existing undelivered-reply flag. Story letters link to Mission Log without accepting, advancing or completing a mission.

D-pad scrolls posts or letters. X and Square react to real feed events. Triangle switches feed/Inbox. Shoulder L/R change GalacticNet tabs. Square pages a long letter; Inbox selection has a bounded five-row rail. Letter pages clamp after content changes. Profile art uses the commander's actual portrait; Earth family/friend and computer portraits reuse the story identities. All screens preserve the ELITE: NEXT/GalacticNet header and have disjoint content, status and footer regions.


## RUMOURS / Space Secrets implementation — 2.5.351

GalacticNet RUMOURS replaces its redundant JOBS feed with a native 480x272 purple scrolling website. Fixed branding/tabs/URL/footer frame varied illustrated classifieds, local experiences, signal diaries and small adverts. Fictional SpacePay uses explicit units confirmation and one short transaction animation; no payment is real and no input can charge twice. This is implemented local exploration guidance, independent of mission generation and the Mission Log.

Every offer must refer to a genuine available discovery in the current canonical system. Quiet systems can have no offers; Earth has none. No fake leads, free introductory offers or cross-system archive. Prices vary, ownership is shared by repeated ads and is stored in the normal commander save. Buying never auto-tracks. The receipt offers TRACK SEARCH AREA or KEEP BROWSING. One optional area marker remains independent of mission/contact tracking. Square stops it; system travel clears it while retaining ownership for return visits.

Paid directions reveal an offset, safe search region, never the object's precise target or identity. Normal sensor range and close Triangle inspection remain necessary. Whale experiences use the real pod, whose compact persistent movement stays within the advertised area. Naturally identified discoveries offer free directions. Permanently unavailable paid targets refund once. V31 saves protect all rumour state with the commander checksum and migrate V30 without invented purchases. See docs/RUMOURS-351.md for exact layout, controls, implementation map and remaining verification limits. Do not treat this as complete-game certification or completion of the newspaper editorial handoff.

## Newspaper puzzle completion (2.5.352)
X must open the printed puzzle through the real game input route. A solved puzzle shows a native animated congratulations card and awards 1,000 units once per system/puzzle kind. Claims and credits save together; repeat solving cannot repeat payment. See PUZZLE-REWARDS-352.md and BUILD-2.5.352.txt for implementation and verification.


## Cargo / exchange reference implementation (2.5.353)
Use the approved native 480x272 layouts for cargo inspection and station exchange. Preserve the selected item's full description and native icon; display capacity and credits in stable bottom regions. Show carried unsupported commodities as NOT TRADED and block transactions. Hold shares and mission reservations must use live game state. Details and exact coordinates: CARGO-CONSOLE-353.md.

## 2.5.354 — implemented Circuit Console inventory standard

Ship Loadout and separate Item Storage use the user-selected native Circuit Console design: navy hardware, category-coloured cartridges, amber cursor, mint active lamps, exact 8px font and a bounded painted real-hull miniature. All usable banks remain visible up to eight per category. Full selected module names/effects must fit; effects can wrap to two lines. Triangle storage access remains a large occupancy card. Storage shows only actual spare copies; hovering an item shows only its matching usable banks. X selects an item then fits into a chosen bank, Circle backs out, and swapping returns the old module to storage. The lower strip contains only selected module details; no redundant right-hand comparison/instructions/capacity panel. Outfitting alone handles resale. No new save schema or fictional inventory stats. Full coordinates and verification gates: docs/LOADOUT-CONSOLE-354.md and BUILD-2.5.354.txt. This does not close the inherited whole-game or physical PSP gates.
