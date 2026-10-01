## 2.5.252 — Development snapshot

Latest packaged EBOOT and cumulative project snapshot, including Elite Exploration radio and all preceding station/planetary work. See CHANGELOG.md. Known inherited input/outfitting failures and physical PSP performance validation remain outstanding; this is not an all-green certification. Active, unfinished work after this snapshot is not included. User saves, personal music and build logs are excluded.

## 2.5.250 — Planetary POIs grounded into their biomes

Planetary landmarks no longer sit on identical grey display plinths. Each POI is graded into four real terrain samples with a shallow sloped earth/rock berm coloured for its planet's biome. Engineered locations retain only a narrow inset footing; ruins, fossil beds, wrecks and crystal grottos emerge directly from natural ground. Collision, interaction ranges, identities and saves are unchanged. All twelve POI families, planetary paths, gameplay/radio/steering and Rich-station regressions pass. See docs/POI-GROUNDING-250.md.

## 2.5.249 — Seamless planetary entry and modern Roamer controls

Retains all cumulative 2.5.248 work, including sixteen Rich-station layouts, station identities and Mega Capital architecture. Moving inward near an eligible planet automatically starts cloud-covered first-person arrival and automatic disembark. Open landing platforms vary by world; Lave I has a smaller port and higher/farther observatory. Field Map supports nub panning. Roamer now has R throttle, L brake/reverse, Circle drift and R+X boost. Pixel-identical planetary rendering optimisations reduce sampled Lave render time about 7%. Focused and game/radio/steering checks pass; full inherited input/performance certification and physical PSP validation remain outstanding. See docs/PLANET-INTEGRATION-249.md.

## 2.5.248 — Rich habitat stations

All 174 Rich main stations now occupy a distinct 2.8–4.6km middle tier with sixteen seeded retro-future layouts: wheels, garden domes, linked globes, stacked saucers, towers and cruise ports. Procedural service pods, mounted animated signs and solar plates share render/collision geometry. Slower spin, larger guidance grid and 8km/12km comms fit the new scale. Poor ports and nine capitals retained; focused and gameplay/map/radio/steering checks pass. Hardware performance and full inherited input suite remain unverified. See docs/RICH-STATIONS-248.md.

## 2.5.247 — Correct main-station and relay targeting

Audited all 256 systems: 73 Poor, 174 Rich and 9 Mega Capital main stations match their listed class and physical hull. Main stations, relays and outposts now have separate stable targets and correct names; Almanac locks and Comms docking no longer silently substitute a smaller nearest hub. Retains cumulative 246 station designs and planetary work. Focused and gameplay/map/radio/steering checks pass; hardware and full inherited input/performance validation remain outstanding. Delivery and proof: docs/STATION-IDENTITY-247.md.

## 2.5.246 — Pulp-era orbital stations

The 256 main stations now use eight connected ordinary hull families or four large capital crown silhouettes: wheels, saucers, globes, tapered palaces and asymmetric liners. Continuous rings have real flyable gaps; shared collision, rotated guidance and freight clearance match the geometry. Station/game/map/radio/steering checks pass. Physical PSP performance, expensive capital views and the full inherited input suite remain unverified. See docs/PULP-STATIONS-246.md for the cumulative EBOOT, actual captures and limits.

## 2.5.245 — Mega-capital architectural variations

Capital skylines now mix seeded domes, spherical modules, habitat pods, octagonal/bevelled towers and pyramid crowns. Shared faceted hulls supply both rendering and collision; docking routes retain their clearance. All nine capitals and 108 guidance routes pass focused checks, along with map/HUD controls. Hardware performance and the inherited broad station collision issue remain; see docs/CAPITAL-VARIANTS-245.md.

## 2.5.244 — Mega-capital cities and planetary Field Map

Resumed and combined both saved development tracks with the cumulative 243 game. Nine mega-capitals now have solid tower districts, animated facade ads and collision-checked docking routes. START opens a player-centred terrain map; Triangle links to the current planet's Codex and Back returns to the map. Focused emulator checks pass; physical PSP performance and the inherited broad station collision issue remain. See docs/CAPITAL-MAP-244.md for the tested EBOOT and limits.

## 2.5.243 — Large futuristic planetary starports

Airport grounds are now 3.24 times larger, with 440m player pads, heavy traffic berths, vaulted hangars, glazed terminals and tall control towers. Shared collision, garage, foundation and local map updated together. Focused emulator tests pass; physical PSP performance and existing broad issues remain. See docs/STARPORTS-243.md for the cumulative EBOOT and verification limits.

## 2.5.242 — Small ground shadows for planetary animals

Fauna now have softly edged pixel contact shadows that follow the ground, blend with the terrain and lighten when airborne. Native shadow/animal and debug/map/HUD checks pass; physical PSP and existing broad issues remain. See docs/FAUNA-SHADOWS-242.md for the cumulative EBOOT and proof.

## 2.5.241 — Debug toggles, planetary map and EVA HUD (local candidate)

Debug Tools now offers independent unlimited fuel and jump range. START on the surface opens the local explored-area map; START/Circle closes, Triangle opens Field Guide. Map progress lasts for the landing and survives boarding. Compact suit/health meters and fewer prompts preserve existing controls. This cumulative build retains the 2.5.240 environments and cinematics. Focused emulator checks pass, broader existing failures and physical PSP validation remain; see docs/INTEGRATED-241.md for the exact EBOOT and verification limits.

## 2.5.240 — First-person planetary transfers and larger ports (local candidate)

Arrival, touchdown, ship/roamer boarding and departure now show the actual explorable landscape through a live pilot view. Cloud cover masks the orbit/surface handoff; no static cockpit panel or separate flat cinematic set. Larger collision-backed ports provide clear rover circulation, relocated traffic pads and more buildings. Focused native transfer/input, camera clearance and expedition checks pass. Physical PSP and sustained frame pacing remain unverified; see docs/CINEMATIC-PORT-240.md for actual captures, performance limits and the isolated EBOOT.

## 2.5.239 — Lave II, III and IV expedition worlds (local candidate)

Lave II now has mosswood hills and pixel-art forest props; III has connected floating skyport decks, safety edges and a cloud sea; IV has snowy hills, conifers, ice boulders and low frost scrub. Their current sites, discoveries, rover routes and port jobs are linked and tested; Lave I is preserved. These remain bounded expedition areas, not whole-planet traversal. Physical PSP testing and IV performance work remain. See docs/LAVE-WORLDS-239.md for actual captures, tests, delivery and limitations.

## 2.5.238 — Animal animation and behaviour (local candidate)

Animals now pause to feed, wander, notice approaching walkers/runners/rovers, flee and settle again. Eight fauna families have real separate idle/stride/wing/feed poses, with grounded gait driven by actual travel and collision-aware local routes. Lave I native captures and focused behaviour, scan/save and movement checks pass; physical PSP testing remains. See docs/FAUNA-LIFE-238.md for delivery, proof and limitations. The wider biome/cloud-platform rollout is still pending.

## 2.5.237 — Planetary landmarks, wildlife and falling leaves (local candidate)

All 12 planetary POI families now use a more detailed shared 3D kit, with additional port details. Discoverable flora/fauna use 16 pixel-art families shared with the Codex, subtle animation and depth-tested world projection. Nearby trees shed a bounded set of falling leaves. Focused native checks cover catalogue identity, all POI families, scanning, save/load and leaf occlusion; physical PSP profiling and further visual polish remain. See docs/POI-WILDLIFE-237.md for actual captures, delivery and limitations.

## 2.5.236 — Verified Lave I rendering improvements (local candidate)

Lave I now renders seeded pixel trees, flowers, rocks and ground detail in actual walking views, with a collision-matched path to the domed Sky Observatory. Corrected projection, atlas cropping, terrain seams and the landmark's real navigation position. Native daytime/dusk/night captures and focused movement checks pass. The sampled emulator route averages 31.6 ms for update + rendering; physical PSP testing and further visual polish remain. This is not yet the supplied concept's visual quality. See docs/LAVE-I-LUSH-EXPLORATION.md for proof, limitations and next steps.

## 2.5.235 — Lave I lush planetary exploration

Lave I now reads as a walkable temperate island world: rolling green ground, layered mountain ridges, denser grass, authored pixel-art trees and undergrowth, and an ochre trail leading to a guaranteed Sky Observatory. Other planets keep their existing procedural profiles. See `docs/LAVE-I-LUSH-EXPLORATION.md` for scope, build notes and the remaining visual/performance checks.

## 2.5.234 — Scheduled Local TV

Local TV is now one CH8 broadcast: a real PSP local clock at top right, timed NEXT/LATER listings and automatic five-minute programme changes. Left/Right and X no longer tune/restart it; Circle returns. Mira has gentle caption-linked babble (SFX volume/quiet-chatter respected), slower Night Stories delivery, and visible masked window traffic. The radio fades out while watching. See docs/LOCAL-TV-SCHEDULE.md for controls, scope and test limits.

## Local development — Galaxy filters

Deep Chart now has nine filters: ALL, VISITED, UNVISITED, RICH, POOR, MEGA, JOBS, IN RANGE and ROUTE. Square cycles a compact five-row scrolling list. L/R still zoom, the analog nub pans, and Triangle searches. Jobs means accepted work; range uses current fuel. See CLAUDE-HANDOFF.md for the separate tested candidate and docs/DEEP-CHART-VISUALS.md for verification limits.

## 2.5.231 — Lave Local TV

Discover's Local TV screen now preserves the approved illustrated studio at native PSP resolution, with Mira's animated mouth, moving window traffic, original orbital channel art and clean live caption/schedule boxes. Left/Right tunes three Lave programmes; X restarts; Circle returns. See docs/LOCAL-TV-231.md for tests and current scope. This is a locally tested development build: focused TV tests pass, but inherited broad-suite failures/stall and physical PSP verification remain outstanding.

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

The dedicated `music/Thargoid Battle` folder now accepts the user's OGG Vorbis tracks directly, as well as MP3. OGG playback uses the PSP-friendly fixed-point Tremor decoder and shares the existing bounded shuffle, resampling, crossfade, music-volume, suspend and generated-fallback path. Put up to 24 `.ogg` files in the folder before starting the game; no conversion or special filenames are required.

No save changes. PSP build and static decoder checks pass; emulator remains closed. See `music/Thargoid Battle/PUT_OGG_FILES_HERE.txt`.

## 2.5.205 — Thargoid battle music library

Thargoid hyperspace interdictions now own a dedicated `music/Thargoid Battle` source. Entering an encounter smoothly fades away from the selected radio station, shuffles up to 24 MP3 files from that folder and restores the selected station afterward. Battle score is dramatic music rather than radio, so Radio Off does not silence it; the existing MUSIC level still controls volume and MUSIC 0 remains fully silent. An empty folder uses the generated Pixel Comet action arrangement.

Folder scanning happens at startup and supports mono/stereo MP3 at 8-48 kHz through the existing bounded PSP decoder. No save changes. PSP build and static source/mixer checks pass; emulator remains closed. See `music/Thargoid Battle/PUT_BATTLE_MP3_FILES_HERE.txt`.

## 2.5.204 — System Operations and Thargoid interdictions

Rebuilt Fly > System Details as a full operational dashboard. A compact local map keeps the station, star and all four worlds selectable; the dossier side now shows station architecture, economy and tech, current faction traffic, local missions, wanted targets, warrant status and cargo-manifest activity, or world access, range and persisted field progress. Landed worlds expose their real local clock, generated settlement/environment profile, life log and completed-site totals. The star page reports the existing heat hazard and logged stellar rifts. Gas giants correctly show floating-skyport access.

A saved-progress meter combines actual landings, species, sites, rifts, bounties and manifest completion. The NEXT strip prioritises a local mission, warrant, active manifest lead, bounty work, first landing, unfinished fieldwork, rift scans or the station lead. It does not invent live weather, market events or other system-condition mechanics. Existing Cross lock and Triangle align/return-to-flight controls remain intact.

No save or asset changes. PSP build and static state-wiring checks pass; compiled input regressions were added but not executed because the emulator remains closed. See docs/SYSTEM-OPERATIONS.md.

Hyperspace jumps now have a rare 14% chance of a Thargoid interdiction. The jump corridor gives way to a compact three-wave rail shooter: aim with the nub or D-pad, hold Cross to fire, dodge incoming bolts and destroy patterned Thargoid craft for 20 units each. Clearing all three waves resumes the original jump. Losing the encounter returns the commander to the exact departure system with the selected route intact and without spending jump fuel. This temporary mode owns input and rendering while active and does not add save payload. See docs/THARGOID-INTERDICTIONS.md.

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

## Power selection and 16 paint themes — 2.5.166

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

## Police encounters and release — 2.5.168

Up/Down now visibly selects the actual yellow police reply; X performs that selection. If restricted cargo is aboard, Up from the first reply (or Down through the list) reveals a fourth reply: surrender illegal cargo. Only the cargo charge from this inspection is waived. Other offences and warrants in other systems remain. Legal cargo is preserved.

Accepting custody shows a short shuttle transfer and detention scene. If funds are available, the existing affordable release-fee rule applies and legal property is retained. With no funds, the existing ship/cargo/equipment seizure applies and an Adder with fuel is assigned at the station. The consequence is stated before choosing custody. Release papers explain the outcome and wait for X before returning to station services. Cargo-only stand-down also waits for a receipt acknowledgement.

## Shared Comms, Shipyard and newspaper — 2.5.167

Display & chatter is no longer a Commander entry. FLY > Comms panel and holding Triangle in ordinary flight reach the same panel. Continue down past the seven channel actions for HUD layout, text chatter, contrast, Radio and audio, Controls and third-person view. Existing incoming conversations retain their reply choices.

Every purchasable hull has its own Shipyard description beneath the list and preview. Close spacecraft gain a bounded recessed-panel detail pass; distant ships retain the cheaper original mesh. The Gazette retains all sixteen articles and its Sudoku, with illustrated commercials and small notices filling the lower section. Wanted has a left-side position thumb; selecting posters still uses normal target-lock/flight flow.

Hold Start: the active SYS/ENG/WEP bank now has a solid pale highlight with dark lettering and pips, plus its name in the footer. This selection treatment is independent of ship paint. Controls temporarily appear in minimal/scenic HUD modes while Start is held; releasing Start preserves the chosen HUD setting.

The decorator now offers 16 finishes. New options: Ice White, Cobalt Blue, Ion Teal, Copper Glow, Lime Circuit, Crimson Red, Tangerine and Ultraviolet. Up/Down moves through two eight-finish pages; the finish counter shows your position. Every finish includes matching ship paint and a readable interface palette. Original finishes/settings remain supported; new finishes save in the existing paint preferences. Do not downgrade after using new colours, since older builds do not recognise them. Commander save format remains V18. The v2.5.165 decorator-label pixel adjustments are retained.

## Wider systems and hot stars — 2.5.164

Planets now occupy widely separated bearings around the inner system. Warp entry uses a full-circle route-specific position and independent heading, rather than turning toward the hub. Hold Square to find contacts around you; Square+R turns toward the selected target. Main-station launch/docking remains familiar, while secondary hubs sit farther out.

Capital freighters now travel between outer station berths and planetary transfer areas over tens of kilometres. Trader shuttles run planet-to-hub legs; explorer groups use fixed interplanetary waypoints so they can actually reach the next destination. Large-hull visibility extends to 55 km, with longer aft-anchored engine trails. Existing bounded traffic counts, combat, cargo-transfer schedules and saved bounty identities remain intact.

All eight sun families now have bright warm surfaces, animated mottling, a soft additive corona and short flares, rather than dark-centred silhouettes. Save format remains V18. These remain stylised, static system layouts with local active-system traffic, not physically scaled orbital mechanics or unloaded-system simulation.

## Planetary landing, departure and navigation — 2.5.163

A valid slow approach now settles at the centre of the landing pad. The rover starts in the nearby signed, open-front garage. Circle disembarks/boards; after boarding, one Triangle press starts a fresh external departure animation and automatically returns to orbit. Arrival, final pad descent and departure have separate transient sequences; boarding clears stale landing state. Circle skips departure.

On foot: tap R for the short grounded jump; hold R to run without jumping. Releasing a run does not jump. Square+R still faces/tracks the selected contact without moving or jumping. Ship and rover have compass icons; numbered POIs match the exploration computer, field guide and nearby building signs. Selected compass markers take priority when bearings overlap.

Close-up walls use a surface-specific four-unit near clip with matching depth precision and full building bounds, keeping reachable walls/corners opaque. This is a targeted rendering fix, not a claim that every possible graphical defect is eliminated. Save format remains V18; physical PSP visual/performance validation is still needed.

## Planetary controls and field buildings — 2.5.162

The hold-Square exploration computer is now a narrow left-side panel. R faces the highlighted POI/species and retains a named distance tracker after closing; off-screen contacts show direction guidance. Field sites are larger solid buildings, with glazed windows, entrance panels and nearby wall-mounted identity signs. The renderer and collisions share building dimensions; surrounding decorative props are excluded from their footprints.

On foot, press R for one short jump/boost (about a second). Holding R cannot sustain flight or automatically jump again on landing. Double-tapping in mid-air adds no lift. Land and release R before pressing again. Rover, spacecraft boost and hold-Square targeting controls are unchanged. Save format remains V18.

## Planetary exploration — 2.5.161

Hold Square on foot or in the rover for the exploration computer. Left/Right selects sites, flora, fauna or minerals; Up/Down selects a contact; R tracks it; X scans the selected species within survey range. Release Square to close without scanning. A short tap still scans nearby unrecorded life. Start opens the local field guide; X uses a nearby site or rover. Existing spacecraft targeting is unchanged.

Spaceports now have large terminal/hangar blocks, a 160m tower and two reserved traffic pads. Ambient ships continuously descend, wait and depart on staggered cycles. You can watch safely from the port grounds. These are exterior landmarks with cosmetic traffic, not enterable terminals or persistent NPC flight schedules.

Local clocks, sky-positioned suns, sunset/night colours, stars and drifting clouds follow a saved world clock. Large forest trees and local animated species share the surface depth buffer with terrain, vehicles, structures and ships. Rover wheels now sit on a shared support height; walking/rover movement is subdivided to prevent tunnelling through obstacles.

All 1,024 planets have deterministic identities and activities; gas giants use bounded floating platforms. Wildlife designs are procedural pixel sprites, not 1,024 hand-authored asset sets. Rocky exploration remains a 1,400m field; islands/platforms are bounded. Field sites reuse twelve activity templates, eight species slots and three port job patterns. Geological/weather labels describe identity; there is no full weather simulation. Save V18 retains earlier profiles/progress and adds the world clock; saves still restore docked, not at an arbitrary surface coordinate.

## Commander files and planetary exploration — 2.5.160

The main menu separates commanders from First Light training. LOAD / DELETE SAVES opens three cards with saved identity, money, system and ship. Left/Right chooses a card; X loads after confirmation; Square permanently deletes only that slot and its backup after confirmation. Tutorial continues from its separate file.

COMMANDER → SAVE/STATUS: Up/Down selects save, load, rename, portrait or fine payment. On Save/Load, Left/Right chooses a slot. Dock before saving. Name editing uses a D-pad keyboard (Square deletes a character; Start applies; Circle cancels). Portrait has four male and four female variants. Save after editing. Slot 1 retains commander.sav; slots 2/3 use commander-2.sav and commander-3.sav. Keep the .bak recovery files. Existing saves import; do not downgrade the EBOOT after writing new-format saves.

On a solid planet: reach the landing pad, slow/descend and Circle to land; Circle again leaves the ship. D-pad moves, nub or L+D-pad looks, R jets on foot. X boards the nearby port rover or parks it; rover travel is faster and sheltered, with no jetpack. Square interacts with a nearby site or surveys life/minerals. L+Triangle cycles and faces points of interest; Triangle faces the ship. Circle boards your ship on the ground; Triangle takes off and then returns to orbit.

Visit SPACEPORT for a relay-repair job, repair RELAY, and return for payment. Other sites offer mineral cargo, ruins mapping, observatory data and a rescue-beacon reward. Activities and surveys are one-time per world and persist when you next save docked. Exploration is a bounded 1400m field on rocky worlds; ocean worlds retain their dry island shoreline. Port facilities are small exterior structures, not enterable towns. Cosmetic ship paint/radio preferences remain installation-wide.

## Quiet watch — 2.5.159

Computer range, fuel and landing notices are dismiss-only; Triangle cannot answer them as conversations. During long peaceful flights a nearby trader may rarely hail you for company. Press Triangle to listen, Up/Down and X to reply, L/R to page longer speech, or Circle to sign off. Ignore an invitation and it expires. Three stories have contextual replies and optional cargo-trade offers; asking for a quote does not exchange cargo. Agree explicitly within 2500m, with the requested cargo aboard. These are optional social/trade encounters, not new missions. Quiet chatter settings suppress unsolicited social hails.

## SPACE TALK — 2.5.156

Station 6 is now **SPACE TALK**, category **TALK RADIO**. Its generated babble has varied speakers, syllable lengths, pitch contours and phrase pauses, without the old noisy shuffle underneath. Radio status now has its own row below the channel numbers, including LOCKED, STATIC and RADIO OFF. Existing radio preferences and music folders are unchanged.

## Boost and roll — 2.5.155

Double-tap R and keep it held to boost. Add L + D-pad Left/Right to roll without cancelling boost or losing acceleration; release L to turn normally, or release R to stop boosting. While boosting, L is a roll modifier instead of a brake. Once R is released, normal L slowdown and double-L hard braking are available again.

Heat still drains shields and can damage the hull. Cockpit and speedometer shaking remain, but heat alone no longer triggers RED ALERT. Actual attacks, missiles and impacts still do.

## Cockpit-relative steering — 2.5.154

Up/Down and Left/Right now follow the cockpit through any bank or loop, in space and atmosphere. Cancelling exterior docking with Circle preserves your view and gives steering back immediately. Launch and Debug → Return to station reset the full attitude; releasing boost no longer unexpectedly levels the ship. No save reset is required.

## Pick your mission in the log — 2.5.153

In **Mission Log**, highlight a mission and press **X** to track it, or **Select** to track it and open its next-step details. **Tracked Mission** follows that choice: main story, Explorers Guild, accepted contracts or Station Welcome. Guild no longer has a duplicate Work menu. The old optional flight guide is retired; the dedicated Start tutorial mode remains available.

First Flight and Kei's main-story briefings now use fuller instructions. Orange replies offer the appropriate launch, return-to-flight, routing or reward action. To dock, approach the station and press Circle within range; the story no longer sends you to a separate guidance page.

## Consistent mission dialogue — 2.5.152

Tracked missions, Open Channel chapters, Explorers Guild, contracts and incoming Triangle conversations share one speech/reply layout. Use Up/Down to highlight replies and Cross to choose; L/R pages longer speeches when shown. Notifications no longer cover reply choices. Tutorial pacing is unchanged in this release.

## First Light tutorial — 2.5.151

Choose **Start tutorial** on the opening menu for a 61-lesson story with Kei and Venn. Only FLY is visible at first. Learn piloting, targeting, power, docking, trade, outfitting, station life, Spacebook, travel and planetary surveying as services unlock.

Choose **Continue tutorial** to resume a dockside checkpoint. Tutorial and normal commander saves are separate. During training, Triangle on the paused command deck reopens the lesson with recovery/exit options. Completing training awards the harbour licence and opens the full game.

[Full tutorial script, controls and acceptance coverage](docs/FIRST-LIGHT-TUTORIAL.md).

# ELITE: NEXT — development build 2.5.39

## Audio candidate — original event cues and bounded shuffle

The audio candidate gives menu, scan, docking, combat and engine events distinct
original synthesized cues with short fades and 20–300 ms durations. The five
existing original stereo music stations and saved Music/FX levels are preserved.
Shuffle tries each candidate once, leaving the previous track until all other
candidates fail. This does not fix decoder failures or certify PSP crackle/sleep.

Current library support is **MP3 only**, up to 24 files in each named `music/`
station folder beside EBOOT. Startup/resume rescans without rebuilding; WAV and
a live rescan button are not implemented. Empty stations use generated music.
The current radio chassis shows folder detection, not a verified playback status
or current filename. Never commit personal music. See
[audio audit and hardware acceptance](docs/AUDIO-AUDIT.md).

## New in 2.5.39 — MacVenture cinematic polish

- Station rooms restaged: one hero focus, three depth planes, warm ochre/cream staging.
- Side hatch doors; thinner cream/cyan MacVenture chrome so MAIN owns the eye.
- Composition from `docs/CINEMATIC-MOCKUP-TARGETS.md` + ART DIRECTOR kit grammar.
- Working tip also carries Act III–IV + dialogue flow + space animation kit — next GitHub Release only with a bigger combined drop.

## New in 2.5.38 — soft-FB Wave A canopy FX

- Denser engine plumes, boost heat shimmer, hit sparks and explosion embers on the unified tip.
- Soft-framebuffer only (fixed pools, canopy-clipped); high contrast skips decorative sparks.

## New in 2.5.37 — unified playable tip

- **One pack:** MacVenture station (LOOK/SPEAK/GO/TAKE, art-kit soft-FB rooms, clear SHIP return) **and** Open Channel Act I–III eight-beat page scripts with choice blurbs and locked codas (through No Easy Flag).
- Warmer planets, settlement silhouettes and richer space from ART DIRECTOR's in-game look pass.
- Post-unify polish targets: `docs/CINEMATIC-MOCKUP-TARGETS.md` (hero focus, fewer frames, warm staging).

## New in 2.5.36 — MacVenture station + art kit wire

- Soft-FB rooms pull ART DIRECTOR palette/styles from `station-art-kit.h` (bake path PR #12).
- Clearer hotspots, talk tips, dead-clear SHIP return.

## New in 2.5.35 — Act I page scripts

- Open Channel Act I briefs (Ch.02–07) deepen to eight ask-then-answer page-script beats.
- Permanent decisions show consequence blurbs; Act I completions open a locked coda before the next brief.

## New in 2.5.34 — MacVenture station deck

- Illustrated station rooms with LOOK / SPEAK / GO / TAKE (Shadowgate-style windows).
- Dense hotspots; EXITS names doors; TRI or YOUR SHIP returns to the command deck.

## New in 2.5.30 — MM6-style station crawl

- First-person station decks read as a classic grid dungeon: riveted panels, checker floor, labeled doors to named rooms, hanging lamp, room props, portrait NPCs with name plates.
- Talk, shops, gifts and taxis unchanged.

## New in 2.5.29 — living space backdrop

- Nebula, space clouds, twinkling stars and rare shooting stars via a soft-framebuffer FX kit (no heavy GU library — fits this engine).
- High contrast mode still strips decorative haze.

## New in 2.5.28 — deeper station crawl corridors

- OpenEnroth-style FP depth: side portals track facing, arched passages show the next room tint, tile floors and ceiling beams.
- Talk, shops, gifts and taxis unchanged.

## New in 2.5.27 — ask-then-answer story chat

- You speak first (orange YOU bubble); Kei answers on the next Cross.
- Saga briefs no longer ask questions the screen already answered.

## New in 2.5.26 — clearer GalNet WANTED board

- Page 2 is obvious (gold banner + DOWN cue); posters show five wear styles including rips and holes.
- Larger `<L` / `R>` sit farther from the tab names.

## New in 2.5.25 — animated pixel-art suns and bloom

- Eight system sun looks with looping pixel frames in every view.
- Soft corona bloom and lens streaks without a second framebuffer.

## New in 2.5.24 — chat flow, WANTED board, suns, richer station crawl

- Story replies ask before the answer beat lands.
- GalNet WANTED has page cues and torn posters; L/R sit clear of the tabs.
- System suns animate with bloom; station crawl looks more like a classic FP dungeon.

## New in 2.5.23 — clearer menu ship, full speech, battle talk

- Command deck ship inset is zoomed farther out.
- Speech and notice text wraps inside its boxes instead of cutting off.
- Combat opens a battle-talk radio cue with short engagement chatter.

## New in 2.5.22 — station crawl, shops, loadout

- First-person station map with characters, exclusive shops, free gifts, taxi passengers and quest tips.
- Outfitting shows only in-stock gear; Ship Loadout tracks **real** fitted slots (buy / replace / sell 50%) and hold.

## New in 2.5.21 — full Act I–IV screenplay authority

- Screenplay merged to complete character bible, lore ledger, and full scene scripts for every chapter and branch.

## New in 2.5.20 — Open Channel feature screenplay

- Full movie-length campaign screenplay with characters, branches, missions and Elite-lore research ledger.
- Permanent decisions use distinct choice labels; trust colours helpers and the epilogue.

## New in 2.5.19 — stop hardware screen flash

- Framebuffer presents with IMMEDIATE after vblank again so the canopy no longer strobes black on a real PSP.

## New in 2.5.18 — varied landings and clearer on-foot play

- Surfaces follow each planet’s orbit-sprite biome and body colours for ground, flora and fauna.
- Planet walking uses the same ON FOOT chrome as the station; docked Fly → Disembark / walk station talks to concourse locals.

## New in 2.5.17 — engine trails sit on the stern

- NPC exhaust plumes and aft glints start at the mesh stern (freighter nozzles), so the flame no longer floats behind the ship.

## New in 2.5.16 — SHIPS includes hostiles; ENEMIES is engage-only

- SHIPS lists every contact, including ships attacking you.
- ENEMIES lists only ships currently going after the player.

## New in 2.5.15 — clearer RED ALERT and ENEMIES band

- Under attack: compact **RED ALERT** at the bottom of the canopy; character speech stays at the top.
- Hold Square + Left/Right always includes **ENEMIES** (ships currently attacking you).

## New in 2.5.14 — locked, expanded story conversations

- First flight and every Open Channel chapter use a six-beat locked briefing: finish the conversation before Circle or Select can leave Tracked Mission.
- The last beat restates the next mission step; after accept, dialogue no longer loops old options.
- Chapter lines expanded with character-specific talk drawn from the campaign bible.

## New in 2.5.5 — speaker chips, clearer menus, radio tuner, system variety

- `NAME SAYS` labels sit on a colour chip; Tracked Mission shows speaker faces.
- Outfitting explains tech gates (in stock / fitted / hub tech too low).
- Galacticnet: Messages sits beside Spacebook; Spacebook logo; Codex Systems + Planets.
- Radio tuner dial with OFF, station notches, retune static, and power off.
- Systems look and place traffic differently; warp arrives farther from the hub.

## New in 2.5.4 — wake from long sleep without a black screen

- Freezes MP3/file I/O as soon as the PSP begins suspending so the audio worker cannot hang on a spun-down Memory Stick.
- On resume, rebuilds display mode, both framebuffers, clock, controls and radio instead of only restarting audio.
- Frame present is `IMMEDIATE` after vblank (2.5.19); `NEXTFRAME` painted the live buffer and strobed the whole screen.
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

- Square tap silently selects the visible object nearest the centre reticle. Hold Square briefly to open the targeting computer; Square+D-pad chords browse its target bands without turning the ship.
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

Replace EBOOT.PBP in PSP/GAME/ELITEA/ on a PSP configured for homebrew, or open it in PPSSPP. Keep commander.sav. This build imports version 1–12 commander files and writes version 13 (fitted module slots). Newly saved version 7+ files store the Guild campaign, power pips, five independent missions, Codex discoveries and visited systems as well as local warrants and upgrades; older builds cannot read newer saves. Do not copy smoke.flag from test directories.

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
- Hold L + D-pad left/right: roll the ship. Keep R held during boost to roll while accelerating; release R before using L to brake. Camera, steering and compass respond to roll.
- X: fire laser.
- Tap Square to select the visible object nearest the centre reticle; hold Square briefly to open the targeting computer. The list is grouped under Planets, Ships, Stations, Other and Enemies. Hold Square with Left/Right to tab those bands, Up/Down to cycle contacts, L to cycle contacts in front, and R to lock and auto-align. Hold R and press Square to lock the nearest hostile immediately.
- Hold L and press X: launch a missile at a locked hostile within 12,000 metres. Ordinary X fire remains the laser. Boost above four times normal speed to evade an incoming missile before its countdown expires.
- Circle while looking at or targeting a nearby solid planet: approach from within 1,000 metres of its surface. Proximity alone does not open the approach screen. X enters atmosphere flight; Circle turns back to space from that prompt. On the surface, Circle lands or walks; Triangle takes off or returns to orbit.
- Circle near the station opens guided docking. For manual docking, match the cyan rotating entrance, keep speed at or below 200, and fly through the slot. Flying into the surrounding hull destroys your ship and triggers a 3D wireframe debris explosion. Press Start for a new commander after destruction.
- Triangle: OK the speech box, then hail whoever you are looking at or have locked. Rescue pilots come aboard when you talk to them. Warp lives on the Galaxy map. Select: command menu. Start: pause.
- Controls screen: L toggles analog steering. D-pad remains usable.

## Wanted levels and police

Wanted levels 1–5 apply only to the system where the offence was recorded. Assault or killing protected ships (not pirates) still files heat immediately. **Restricted goods** (Slaves, Narcotics, Firearms) are **not** a crime until Local Law scans your hold.

### What triggers a stop

- **Warrant stop:** any unpaid local heat, Law within ~650 m.
- **Cargo scan:** clean warrant but restricted tonnes aboard, Law within ~650 m.

Flight freezes. Up/Down choose, **X confirms**.

### Scan menu

1. **Submit** — clean hold: released with a short grace. Dirty hold: goods seized, warrant filed, settle menu opens.
2. **Refuse** — forced open; seizure + extra heat; settle menu.
3. **Run** — escape with raised warrant and pursuit (same as settle-run).

### Settle menu

1. **Pay fine** — 50 units per wanted star; clears this system's warrant.
2. **Station custody** — half fine (or all you have); teleports you to the hub clear.
3. **Run** — escalate and flee.

### Getting clear

- Pay or take custody at the stop, or  
- Dock and open **Save / status**, then **Square** to pay the local fine at the desk, or  
- Ask CUSTOMS on the station walk when you have heat, or  
- Leave the system (the warrant waits for your return). Pirate bounties stay separate (15.0 U for destroying pirates).

Higher wanted levels still wake extra police. Warrants remain per-system across warp and save/load (V2+ `wanted[]`).

## Debug menu

Select > Debug tools provides: add 1,000 units; refill fuel and repair shields; clear or raise the local wanted level; move to the station approach; move near the selected planet; and dock immediately. Changes affect your current commander and are saved if you save afterward.

## Space and navigation

The scanner displays all active ships, planets and the station using compressed distance scaling; colours match the faction guide. The compass points to the selected contact, including behind you. Names in flight match the contact list. The header explicitly labels the current system and its danger rating.

Danger 1-5 controls pirate numbers, pursuit range and firing frequency. A system only keeps about eight to eighteen civilian ships alive at once: traders inbound and outbound, a Law pair hunting pirates, explorers flying a shared survey, and maybe one slow freighter. Twelve extra police slots still wake if you are wanted.

Each system contains a sun, ocean world, gas giant and two rocky planets. Rock belts, cold ice belts, migrating space-whales and comets appear in some stars, not all. Coloured stars, nearby dust, boost streaks, sun lens flares and an animated warp tunnel preserve the retro presentation. Collisions use simplified volumes with swept player checks to prevent high-speed tunnelling.

## Planet exploration

Circle opens the approach choice near an ocean or rocky planet. X enters a biome surface tinted from that world’s orbit sprite and colours (ocean island, arid, ice, volcanic or forest) with a cyan pad. Descend onto the pad and press Circle to land, Circle again to walk. Square on foot surveys flora, fauna and minerals coloured for that world. Board the parked ship, Triangle to take off, Triangle again for orbit. On foot: nub looks; D-pad walks/strafe; hold L + D-pad to look with analog disabled. Hold R to lift, release to descend. Triangle faces the ship; Circle boards within 60 m when grounded. The HUD shows ship bearing/distance, exposure and suit health. Water and the 480 m local field edge block travel while allowing retreat. Ground rendering and walking use the same triangulated terrain. Walking more than 140 m from the pad builds environmental hazard. Help page 5 lists EVA controls. Survey rewards currently reset on re-entry; persistent first-visit rewards and guaranteed minerals on every solid world remain planned. Docked: Fly → Disembark / walk station to talk on the concourse.

Suns and gas giants have no landing approach. A gas-giant boundary stop offers Circle to turn away; it does not offer a surface flight or scan reward. Approach choices pause threats and mission clocks. You cannot land on water. Quiet systems stay sparsely populated; Atlas echoes and extra life appear in rarer or more dangerous stars.

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
## 2.5.219 — In-world Outfitting shop signs

Outfitting now opens beneath a slim station storefront sign instead of exposing the local economy, technology and trade values. Every station receives a stable, seeded shop identity—such as Major Lazor's Armaments, Aegis Defence Works or The Module Exchange—with a concise speciality caption. Lave remains a dedicated arms house; general and specialised stock keep a matching, readable identity whenever the player returns.

Verification: PSP build passed. Native Outfitting captures confirm the new arms and general headers at PSP resolution; input suite passes with 0 failures.
