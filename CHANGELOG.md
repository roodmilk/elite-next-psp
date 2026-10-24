2.5.488 packaging candidate keeps the detailed Twin Stack gun housings permanently extended and restores immediate X firing. Circle+R is unassigned and no longer appears in the controls. The commander menu removes the decorative SHIP / COMMAND LINK heading and omits STANDBY for a new commander. ERL finishes now skip the automatic replay and immediately run the native ship-through-position animation with dynamic numerals and ordinal titles. Recorded action remains available for optional LocalTV highlights. Circle+Triangle remains flares, Circle+Down remains heat sink and Circle+Right remains next fitted gun. No save-format or weapon-balance change.

2.5.487 hardpoints: hold Circle and tap R to retract/extend the detailed Twin Stack housings. Hold X while stowed to deploy; primary fire waits for the 0.55-second actuator to finish and then fires while X remains held. Retraction moves the actual 3D model below the sill; the fully extended cached artwork is unchanged. Runtime actuator state resets on ship change; no save-format change. Circle+Triangle now selects flares, replacing the former duplicate heat-sink shortcut; Circle+Down remains heat sink. Circle+Right gun selection and ordinary R boost remain separate. 73 focused native checks pass (7 hardpoint, 66 cockpit). Retail PSP-3000 memory budget passes. New performance/hardware measurements pending. Original 486 and PSP stable packages retained. No Git metadata exists here; no push/tag claimed.

## 2.5.486 â€” exploration, navigation and spatial regions

2.5.486 local PSP-3000 exploration playtest adds passive bearings, six linked physical navigation buoys, a separate navigation log, persistent visited coordinates, mission-only coordinate providers, and three remote local search offers. Live scanner locks remain range-limited and independent of navigation. V39 appends CRC-protected knowledge and paid data; V37/V38 migration passes. Existing HUD, guns, cosmetics, story/tutorial, menu/Extras and ERL integrations are retained.

Six finite spatial-region families use seeded world-space layers, opacity artwork and local lighting through a batched PSP GU atlas. Gas, magnetic activity, auroral curtains, wreckage wakes, comet streams and quiet dust keep depth/parallax and gradual boundaries. Magnetic activity has one bounded temporary engine-output response. Ordinary flight works at zero fuel. The expanded layout is shared across 256 systems and rejects unsafe arrivals/inactive-body anchors.

360 focused native checks pass. A complete script-steered Iron Cathedral test of the Lave search/buoy/listening-post branch and return takes 206.6 seconds and 2.393 LY; the zero-fuel 16 km recovery takes 64.9 seconds. These are actual simulation journeys, not human playthrough claims. Native engine images, moving-camera footage, reference comparison and reports: assets/generated/navigation486/index.html.

Retail native PPSSPP performance with the actual four-gun Anaconda and current 3D cockpit housings: gas: 29.45, 29.82, 29.82 FPS; storm: 29.37, 29.82, 29.82 FPS; aurora: 52.8, 59.64, 59.64 FPS; wreckage: 27.32, 29.82, 29.89 FPS; comet: 57.57, 59.64, 31.98 FPS; veil: 57.57, 59.64, 59.64 FPS; quiet: 56.93, 59.35, 59.64 FPS. Zero audio buffer starvation/drops in these tests. Strict 29.5 FPS limitations: ['gas: [29.45, 29.82, 29.82]', 'storm: [29.37, 29.82, 29.82]', 'wreckage: [27.32, 29.82, 29.89]']. Memory: PASS retail PSP memory budget: image=23005752 heap=12582912 main_stack=1048576 headroom=17888712 psp3000=True. Physical PSP confirmation and existing frame-time spikes remain open. This is a local PSP-3000/emulator playtest, not a locked-framerate or complete commercial-release claim. The broader old smoke suite is blocked by the pre-existing obsolete departure_ad_text test helper. The 24-entry R&D catalogue is not 24 shipped mechanics; this release establishes six environmental families through shared systems.

Hold Square and use Left/Right to open NAV LOG. X plots coordinates without changing your target. R aligns the ship to the plotted lead. Physical contacts still lock/track in their own categories.
A faint scanner trace gives a rough bearing, UP/DN and strength. It is not an identified live contact. Slow below 120 m/s within 700 m of a buoy for two seconds to read its local links.
GalacticNet > Rumours works remotely: three local search offers cost 25/35/45 U. Owned/scanned directions stay free. Previously bought sightings remain available. A search area is not a weapon lock.
On an inhabited ocean planet's parked-ship screen, Square accepts the local courier. Its station coordinates belong to that mission unless independently supplied or visited.
At zero fuel, ordinary flight remains usable; boost and hyperspace are unavailable.

Original PSP stable and 2.5.485 download remain intact. No Git metadata exists in this mirror, so no commit/push/tag or remote release is claimed. SHA-256: 996216D312D3CA0FBC506A66182C1D821DCDE4CA3DA8192284EDBE7B943CD8E5

## 2.5.485 — solid 3D twin-barrel guns

2.5.485 PSP-3000 local playtest rebuilds the short lower-corner Twin Stack as solid 3D guns. Each bank has two separated eight-sided barrels with closed caps, raised sleeve collars, narrow closed breeches, top/side/underside facet shading, vents, bolts and cyan material accents. Both sides of each receiver and all barrel faces carry depth, replacing the broad flat receiver sheets. The short inward-facing centreline and lower-corner mounting positions are retained. Alternating discharge now uses the actual lower/upper barrel centres at six-unit spacing, with emission beyond their matching far tips. Weapon slots, input, damage, cooldowns and aiming rules are unchanged.

308 focused native checks pass, including actual Anaconda shipyard purchase, all four fitted gun identities, Circle+Right immediate switch/wrap, lower outer-corner receiver projections and matching far-tip emission axes. Previous cockpit, fitting/save, cosmetics, menu, departure/adverts and ERL tests pass. All-hull captures are retained with the diagnostic reports; source hashes are in design/guns485. Native before/after, approved reference and80-frame lossless real retail footage: assets/generated/guns485/index.html. Footage writing overhead is excluded from separate performance tests.

Native-resolution PPSSPP Anaconda timings: hit 31.94, 29.89, 29.9 FPS; switch 31.94, 29.89, 29.9 FPS; busy 28.38, 28.75, 28.67 FPS; station 29.44, 29.75, 29.24 FPS; thirdperson 28.46, 29.1, 28.81 FPS. Zero audio starvation and command drops. Strict-floor limitations: ['busy misses the strict 29.5 FPS floor: 28.38, 28.75, 28.67 FPS', 'station misses the strict 29.5 FPS floor: 29.44, 29.75, 29.24 FPS', 'thirdperson misses the strict 29.5 FPS floor: 28.46, 29.1, 28.81 FPS']. Worst frames are in validation.json; physical PSP confirmation, occasional frame-time spikes and pre-existing broader smoke failures remain open. Memory: PASS retail PSP memory budget: image=22851608 heap=12582912 main_stack=1048576 headroom=18042856 psp3000=True. The user accepts approximately29FPS with visual fidelity preserved; continue to investigate noticeable frame-time spikes rather than reducing the approved model to chase exactly30FPS. This is a local playtest, not a locked-framerate release claim. Saves protected and intro bytes unchanged. Previous484 package and original PSP stable distribution retained. No Git metadata exists here; no commit, push, tag or GitHub release is claimed. SHA-256: 2CF6DB3A524029EC6F69ACE4BD21662B368BA3303B0D3297F9B69DCEFD9F1F38

## 2.5.484 — lower-corner inward-facing guns

2.5.484 PSP-3000 local playtest moves detailed Twin Stack receivers outward and rearward to the lower cockpit corners, with short stacked sleeves facing inward. The sleeves stop near the lower corners and shots converge from their matching tips. The lower mounting edge reaches the cockpit sill. Cross-section girth is doubled so the detailed receivers and separated sleeves remain substantial at this sweeping angle. No sprites or material details were removed. Muzzle emission follows the actual slanted barrel tangent beyond its tip. Hull attachment anchors, damage, cooldowns, aiming and four-slot selection behavior are preserved.

308 focused native checks pass, including actual Anaconda shipyard purchase, all four fitted gun identities, Circle+Right immediate switch/wrap, lower outer-corner receiver projections and matching far-tip emission axes. Previous cockpit, fitting/save, cosmetics, menu, departure/adverts and ERL tests pass. All-hull captures are retained with the diagnostic reports; source hashes are in design/guns484. Native before/after, approved reference and80-frame lossless real retail footage: assets/generated/guns484/index.html. Footage writing overhead is excluded from separate performance tests.

Native-resolution PPSSPP Anaconda timings: hit 35.38, 29.9, 29.89 FPS; switch 35.14, 29.88, 29.91 FPS; busy 28.46, 28.82, 28.6 FPS; station 29.28, 29.6, 29.38 FPS. Zero audio starvation and command drops. Strict-floor limitations: ['busy misses the strict 29.5 FPS floor: 28.46, 28.82, 28.6 FPS', 'station misses the strict 29.5 FPS floor: 29.28, 29.6, 29.38 FPS']. Worst frames are in validation.json; physical PSP confirmation, occasional frame-time spikes and pre-existing broader smoke failures remain open. Memory: PASS retail PSP memory budget: image=22848344 heap=12582912 main_stack=1048576 headroom=18046120 psp3000=True. This is a local playtest, not a locked-framerate release claim. Saves protected and intro bytes unchanged. Previous483 package and original PSP stable distribution retained. No Git metadata exists here; no commit, push, tag or GitHub release is claimed. SHA-256: 0E976412D67A4C7CD2178564F0657E41478FBC596C8626FDD632EEE90675E16D

## 2.5.483 — full-sized large-ship guns

2.5.483 PSP-3000 local playtest fixes tiny, high Twin Stack housings on large ships. The old 12m model-size ceiling shrank the Anaconda's guns despite their real hull mounts being over 200m ahead of the cockpit origin. Housings now retain their approved proportions at each actual hardpoint depth, including closed receivers, stacked sleeves, braces, material details and reflections. Muzzle effects continue to originate at the same far-tip geometry. No hardpoints, aiming, damage, cooldowns, slot capacity or approved artwork were replaced.

306 focused native checks pass. The new regression buys an Anaconda through the actual shipyard, confirms four WPN slots, fires each of four fitted weapon identities, and uses Circle+Right to switch immediately through all four and wrap. It checks substantial receiver size and lower cockpit placement; all previous cockpit, fitting/save, cosmetics, menu/intro, departure and ERL checks pass. All-hull native captures are retained. Existing482 station adverts and eight-second departures are preserved. Source snapshot: design/guns483/final-source.zip and final-source-hashes.json.

Native before/after, approved reference and80-frame lossless actual retail firing footage: assets/generated/guns483/index.html. Footage has disk-writing overhead and is not an FPS benchmark. Separate native-resolution PPSSPP Anaconda timings, without compilers/recording: hit 38.31, 29.96, 29.91 FPS; switch 37.19, 30.05, 29.89 FPS; busy 28.95, 29.17, 29.02 FPS; station 30.24, 29.9, 29.45 FPS. Zero audio starvation and command drops. Busy combat and one station window miss the strict29.5FPS benchmark floor (roughly29FPS busy,29.45FPS station); this limitation is retained in validation.json rather than marked as a pass. The gun correction is delivered as a local playtest, not a completed performance milestone. Worst-frame timings, occasional spikes, physical PSP confirmation and pre-existing broader smoke failures remain open. PASS retail PSP memory budget: image=22847768 heap=12582912 main_stack=1048576 headroom=18046696 psp3000=True. Intro bytes unchanged; protected saves unchanged. Earlier482 package and original PSP stable distribution remain available. No Git repository metadata exists in this mirror, so no commit/push/tag or GitHub release is claimed. SHA-256: 510C41E99DFE69520AF49A4C6537054C32CEF7F4BA7663BA13F0988C8C13956F

## 2.5.482 — illustrated perspective adverts and station departure

2.5.482 local PSP-3000 station departure update: six custom 64x32 pixel-art advertisements (KOYO bowl, NOVA star/casino, VOX radio, DRIFT courier, AZURE helmet and ORBIT hotel) map to real wall/building planes with perspective-correct UV interpolation and near clipping. Exact background-paper masking preserves foreground rails, ship geometry and HUD. Lave's corridor uses DRIFT/VOX rather than another KOYO label; station profile varies the pair. Larger building adverts stay visible for about four seconds at Lave. Native proof: assets/generated/port482/index.html, 81 losslessly verified 480x272 frames spanning 8.1 seconds.

Normal station departure lasts eight seconds with slower motion near building adverts, then a small smooth lane curve starting at least 100m beyond the station hull. Position and camera use the same physical path; handoff is aligned at cruise speed with all staged scenery cleared. The first authored Earth ascent retains its original five seconds and straight path, preserving the existing city cutscene. Poor/rich/mega/relay geometry and traffic use the same transformed camera basis. Crossing/outbound/inbound background ships are deterministic decorative meshes, not new persistent simulated NPCs. The player's inbound docking cinematic is not redesigned.

302 targeted native checks pass:15 port art/timing/clearance,24 departure HUD/tracking,60 cockpit,32 fitting/save,78 dashboard,35 hanging charms,7 menu and51 ERL routes. All256 system departure profiles finish smoothly without fuel/hull loss; all256 clear staged scenery before handoff. Poster raster preserves foreground geometry and HUD; perspective and advert dwell are checked. Protected saves remain unchanged. Previous481 controls, cockpit, cosmetics and fitting are retained without art simplification. Intro embedded bytes remain identical.

Separate final retail timings: lave 42.99, 41.91, 29.9 FPS; capital 57.9, 32.23, 31.23 FPS; relay 57.25, 51.67, 35.19 FPS; cruise 36.79, 29.9, 29.89 FPS; boost 35.5, 59.64, 59.64 FPS. Runs use native-resolution PPSSPP PSP-3000, host muted, mixer/display enabled, private saves; no recording, compilation or compression during measurement. Launch windows include the actual eight-second departure followed by coasting. Zero audio starvation/command drops. Per-window worst frame times and strict29.5FPS floor status are recorded in validation.json; averages are not a locked-framerate guarantee. Physical PSP confirmation, frame-time spikes and pre-existing broader legacy smoke failures remain open. Memory: PASS retail PSP memory budget: image=22847064 heap=12582912 main_stack=1048576 headroom=18047400 psp3000=True. Original PSP stable distribution and481 package are preserved. No Git metadata or GitHub publication claim. SHA-256: F6D87574EA71F7B1918436CB103A8786F9CA8BC79C6DAFD0E49ED1B0FBCAF5D9

## 2.5.481 — consistent launch cockpit and R-to-track

2.5.481 local PSP-3000 station-departure HUD and R tracking fix: guided station launch (dock_stage4) uses the same modern cockpit overlay as ordinary flight, including rolling speed digits, Twin Stack housings, reflections and fitted hanging/dashboard cosmetics. Autopilot status sits at y40 below the top console. Presentation eligibility is separate from input eligibility: manual targeting/tools remain disabled during departure and the fresh-button-release gate. The ordinary flight overlay is factored into one shared function; no approved art was simplified. Dock-stage changes invalidate the cockpit cache immediately at entry/handoff. Hold Square and press R to lock the highlighted detected contact and turn toward it; X alone still locks without turning, and X+R remains supported. Computer input consumes R so it cannot also boost or accelerate while browsing. The R shoulder key is labelled TRACK in the target list.

287 focused native checks pass:24 launch/tracking,60 cockpit,32 quick fitting/save,78 dashboard,35 hanging charms,7 menu/intro and51 ERL routes. Launch is checked across poor/rich/capital/relay station views, all153 frame-sized steps per class, and actual menu Launch with button mashing through the five-second path and fresh-release guard. Native glyph checks confirm the modern target-console label throughout launch and handoff. Exact assets and installed cosmetics remain visible. R confirms the highlighted contact, enables autoaim and produces an actual yaw change; it leaves boost off and does not accelerate. Protected saves unchanged.

Native gallery assets/generated/departure481/index.html includes51 losslessly verified renderer frames over5.1seconds, launch start/end/free-flight stills and R tracking. This fixed-step fixture is not an FPS measurement. Separate final retail workloads: launch 49.95, 35.47, 29.89 FPS; cruise 36.79, 29.9, 29.89 FPS; boost 35.5, 59.64, 59.64 FPS. All runs have zero audio starvation/command drops. The launch workload executes the actual five-second departure followed by ordinary coasting; window timings include the handoff/free-flight period, rather than claiming a pure five-second port-only average. Physical PSP confirmation, occasional frame-time spikes, broader pre-existing smoke failures and commercial release gates remain open. Memory: PASS retail PSP memory budget: image=22791816 heap=12582912 main_stack=1048576 headroom=18102648 psp3000=True. V38 saves, intro embedded bytes and previous480 package retained. Original PSP stable distribution unchanged; only PSP-3000 local playtest promoted. No Git metadata, commit/push/tag or GitHub release claim. SHA-256: AA64AF00DDF57CFBB60447B74CABBD3A8EA3C895263334BF830AE4CB20D0B4A4

## 2.5.480 — quick fitting, held targeting and rolling speed

2.5.480 local PSP-3000 loadout/HUD playtest: X over a Ship Loadout slot opens right-side Item Storage filtered to that category. Up/Down browses stored hardware including exact enhanced instances; X fits into the selected bank, returning the old item to storage; Circle cancels. Triangle full storage and Square removal remain. Existing cargo, passenger, capacity and independent cosmetic-family restrictions retained. Hold Square opens targeting immediately; release closes even while Circle tools or Start power controls own input. Lock changes require confirmation; live flight continues. The native speed counter rolls four game-font digits every rendered frame through a 32x8 aperture, independently of cached console painting. Speed smoothing uses elapsed game time. Acquisition, panel-edge reveal, selected-tool/gun transitions, confirmation rings, receiver/Comms activity and local warnings animate without gating input. Console redraws are bounded to 15Hz plus immediate state/input updates; small effects run at render cadence. Approved artwork retained.

Equipment autosave now defers unsupported free-flight checkpoints instead of producing a repeated false failure warning; pending changes retry on the docked command deck. Genuine creation/write/seal/validation/replacement failures retain useful reasons, with previous valid-save backup behavior unchanged. Save validation and V38 serialization remain enabled and unchanged in format. This fixes a reproduced warning route; it does not claim recovery of an unseen human session. Save successfully at a station before replacing an actively running old build.

263 focused native checks pass: 32 quick-fitting/hold/save, 60 cockpit, 78 dashboard, 35 hanging charms, 7 menu/intro and 51 ERL route checks. Protected test saves unchanged. Native UI captures and a lossless 90-frame animation are in assets/generated/loadout480/index.html, alongside the supplied reference. Animation is a fixed-step scripted renderer demonstration; retail FPS is measured separately without recording or compilers. Final exact-retail measurements: cruise: 36.79, 29.90, 29.89 FPS; boost: 35.63, 59.64, 59.64 FPS; station: 30.33, 29.89, 29.60 FPS; computers: 34.08, 32.58, 30.43 FPS. Zero audio starvation/command drops across all four workloads. Boost uses real double-R input and remains active over 500 frames; its later windows are open-space flight. Station/expanded computers are staged native orbit workloads. See validation.json for worst-frame times and strict-floor status. Occasional frame-time spikes, physical PSP confirmation, long sessions and pre-existing broader smoke failures remain open. No locked FPS/commercial-release or GitHub-sync claim. Memory: PASS retail PSP memory budget: image=22803080 heap=12582912 main_stack=1048576 headroom=18091384 psp3000=True. Embedded intro byte-identical. Prior479 and original-model PSP stable distribution preserved. SHA-256: 5460460DEAEEE6E6F1C68D605354A656B642D3BB350AD235C170540B5F46586F

2.5.479 local PSP-3000 cockpit cosmetics playtest: all four approved hanging charms and sixteen dashboard sprites use supplied pixel art directly. Lave stocks all twenty at a proposed25U each. Purchases enter existing Item Storage as normal module instances; fitting/removal/save paths reused. One hanging charm per ship uses Utility, and one dashboard cosmetic uses Hold/Cabin, independently across all banks/types. Multiple owned copies allowed; remove the current item before fitting another from its family. No rarity/power tier or stat bonuses. Violet/lilac names, distinct icons and full cosmetic labels in shop/storage/fitting. Hanging motion follows actual turns/acceleration/braking/impacts with frame-independent bounded springs; dashboards remain static at native(0,116). Comms icon/text moved10px right to clear the supplied shelf. CLOSE/Triangle and speaker tag now sit within a padded caption header, with all three body rows retained.78 dashboard,35 charm,60 cockpit,51 ERL and7 menu/intro native checks pass (231 total), including exact opaque sprite pixels, legacy/new saves, duplicate limits and expanded-computer occlusion. Ten exact-retail performance cases have zero audio starvation/command drops; both cosmetics draw in about0.7ms/frame. Music settles around29.9FPS; station averages29.28/29.75/29.37FPS, with two retained29.5 strict-floor failures. Memory budget passes with18,097,608bytes headroom. Original V38 save stream width115 retained; new ownership uses existing DockRoll records. Intro byte-identical. Native gallery assets/generated/cosmetics479/index.html; all4 native motion/reference comparisons in motion.html. Prior478 and PSP-original stable dist preserved. Physical PSP, long-session checks and existing broad legacy smoke failures remain open. No Git metadata; no commit/push/tag claim. SHA-256: C805500477E1C54CFE45474C493A925866005487D22B22B6BFED086035B69A8E

2.5.478 local PSP-3000 cockpit + G Twin Stack playtest: approved armoured computers, native font, full tool names, real slot dots, immediate Circle+Right fitted-gun cycling and explicit Square/D-pad/X contact confirmation. Both computers preserve live analog flight; R+X confirms and aligns without boosting. Twin housings retain stacked sleeves, closed receivers, panel detail, far-tip firing, recoil, glow and varied reflections at actual hull hardpoints. Native colour/depth caches and opaque HUD spans optimise drawing without an art-detail cut. All 23 hull mount views and 60 native control/layout/cache checks pass. Menu/intro 7 and ERL 51 checks pass with private saves unchanged; embedded intro is byte-identical. Exact-retail native benchmarks, strict frame-rate shortfalls and separate capture-timed APNG comparisons are recorded in assets/generated/cockpit-hud478. Rapid-fire sound-command overflow fixed using 384 extra bytes of bounded queue storage. Weapon balance and save layout retained. Physical PSP testing remains pending; separate broad legacy smoke is not all green. Prior 477 and PSP-original stable dist preserved. No Git metadata, commit, push or tag claim. SHA-256: 026555735FEA6461DB582EFCC2DB57AB1DC59B2741EF828CD6C07F1878E9210E

2.5.477 local PSP-3000 encounter art reworks: the other nine rejected474/475 drafts now use purpose-built structures and individually composed major-surface pixel paintings, following the user-approved476 radio. Salvage village, shipbreaker, greenhouse, fuel stop, ferry, tea barge, ore depot, parts exchange and beacon repair crew replaced.650 new storage tiles from93 continuous paintings;951 total atlas tiles. All nine exact-retail near/medium/distant orbit checks pass at30.43–32.93FPS, audio starvation/drops0. Radio geometry/art byte-identical to476; its final orbit29.60–30.66FPS. Normal startup and memory budget pass (19,307,112bytes headroom). Older relay near29.28 and carrier near28.70 remain strict-floor failures, disclosed in validation. Occasional long frames and physical PSP verification remain open. Disjoint65536+2*tile+half material IDs avoid racing/fleet collisions; CPU/GPU paths and mega-ship showroom migrated together.260 host order/bounds cases pass and art regenerates byte-identically. Separate diagnostic build passes51 quick-race and7 menu/intro checks; private saves unchanged. Existing intro byte-identical; encounter placement, interactions and save format retained. These nine are galaxy catalogue types, not nine extra Lave locations. Lave has8 built encounters plus3 natural anomalies. Native gallery assets/generated/nine-encounter-reworks/index.html; package deliverables/ELITE-NEXT-2.5.477-NINE-REWORKS.zip. Prior476 retained; stable PSP-original dist remains separate. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: D043EA4DBE7499A21B596D0C7F396768CF84AC487C74C07DAC67AC966FF36329

2.5.476 local PSP-3000 radio art rework: user rejected all ten474/475 encounter previews as too basic. Rebuilt ONLY radio entry14 with separate continuous front/rear/side/roof paintings, rounded16-sector/7-band service pod, railed deck, braced mast and separate dish.186 new storage tiles;301 total atlas tiles. Fixed the exposed fixed-size180-material/Adder128-offset batch bug using disjoint dynamic ranges; sparse ascending material iteration preserves draw order.260 host boundary/order cases pass. Exact retail radio orbit30.42/31.14/29.75FPS, zero audio starvation/drops. Mechanic/salvage and normal startup pass; relay near29.11 and carrier near28.70 miss the29.5 strict floor and remain explicitly open. No memory faults in final five600-frame runs. Retail memory budget passes with23,437,432bytes headroom; physical PSP pending. Intro byte-identical; other nine geometry and old72 materials unchanged. Prior ten basic previews are not approved finished art; nine still need the same redesign. No new placement/services/save-format changes. Native gallery assets/generated/radio-shack-rework/index.html; package deliverables/ELITE-NEXT-2.5.476-RADIO-REWORK.zip. Failedv1 is excluded; previous475 remains recoverable. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: 01A28C631593CFD2B5D7F1E60F6B02B34465D1C25F5BBB0C38FD647ED1AFA453

2.5.475 local PSP-3000 encounter catalogue wave 2: Cooperative Ore Depot, Parts Exchange and Beacon Repair Crew now have individual geometry,12 fitted material plates and mechanical animation. All three exact-retail near/medium/distant orbit tests passed at32.14–34.85FPS, audio starvation/drops0; occasional50–67ms frames remain. Previous salvage village baseline and normal native startup pass on the same executable. Previous seven474 geometry and60 material plates are byte-identical; src/main.c and space-discoveries.h are unchanged from the474 source snapshot. Embedded intro is byte-identical. No new services, placement or save changes. 17/75 catalogue types now bespoke;58 remain. Natural anomalies are tracked separately. Physical PSP and broader stress testing pending. Package deliverables/ELITE-NEXT-2.5.475-ENCOUNTER-WAVE-2.zip; all ten new native previews assets/generated/encounter-upgrades/catalogue-progress.html. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: A0969A67E49FD52C28B8E10E08C253C95FCBDC3FD651848FDCB31A139016D717

2.5.474 local PSP-3000 encounter catalogue wave 1: seven new bespoke encounter designs (salvage village, shipbreaker yard, community greenhouse, patchwork fuel stop, old passenger ferry, local radio shack, miners tea barge), with 28 role-specific pixel-art plates, mapped geometry, real sunlight and bounded animation. All seven exact-retail near/medium/distant orbit checks passed, averages31.84–35.68FPS, audio starvation/drops0; occasional50–67ms frames remain. Normal native startup and retail memory budget passed; embedded intro film is byte-identical. Physical PSP, broader gameplay and crowded-view testing remain open. Existing placement, interactions, saves, menu, ERL and hyperspace source retained. Four-system encounter certification restriction remains; native gallery forces catalogue entries into a private fixture. 14/75 catalogue types now have bespoke upgrades;61 remain. Separate32-type natural-anomaly audit remains pending. Local playtest only; no Git metadata in this copy, no GitHub sync/tag claim. Package deliverables/ELITE-NEXT-2.5.474-ENCOUNTER-WAVE-1.zip; native gallery assets/generated/encounter-upgrades/catalogue-wave-1.html. SHA-256: D4D4D0BA711CA17791C7D7F5CB17DEE3150010C6215CE34479EAF739866BF4A9

2.5.469 local PSP-3000 travel/shipyard pass: all three approved mega showroom styles distributed deterministically; eight-row mega catalogue, real six-category capacities, coloured spec meters, capacity tiles and full descriptions. Missing MC_BOX station-preview geometry restored, authored panel textures and closer sunlit Continue camera. Real distant traffic/weapon/cloud polish and compensated swept-clear scenic docking. ERL bookings permit travel; race-entry resources and remote cancellation protect fuel/progression/location. PSP-3000 asynchronous compressed MP3 read-ahead and validated128kbps Elite Exploration copy installed with original backup. Exact sun/atmosphere pixel checks, copied real commander round-trip and native travel/audio checks pass. Busy-view FPS and physical PSP testing remain unfinished; no locked30/60FPS or stable-release claim. Details docs/TRAVEL-POLISH-469.md and design/travel-polish469/HANDOFF.md. Stable VERSION/dist retained; no GitHub-sync claim.

2.5.468 local PSP-3000 boost onset: hyperspace streaks now grow outward and fade over0.85seconds with matching FOV easing; individual streaks fade at wrap boundaries. Ordinary speed motes integrate a continuous phase across double-R boost and use smooth colour/speed transitions, avoiding the previous absolute-time phase jump. Controls, acceleration, saves and existing audio unchanged. Exact retail startup and flight+MP3 checks pass silently; native transition recording/physical PSP remain unverified. Artifact milestones/2.5.468-boost-onset/EBOOT.PBP; source patch design/hyperwarp-current/playable468.patch. No locked60FPS or stable-release claim.

2.5.467 local playable PSP-3000 hyperwarp: approved electric charging, rainbow storm tunnel, six speed pulses and destination opening integrated into the full current game. Existing entry method, audio and all other game source preserved; rejected sound candidate excluded. Exact retail intro/home/flight+MP3 checks pass silently. Artifact milestones/2.5.467-hyperwarp/EBOOT.PBP; details design/hyperwarp-current/PLAYABLE-467.md. Physical PSP, exact-retail full-route input test and sustained frame pacing remain unverified; no locked60FPS/stable-release claim. Stable VERSION/dist retained.

## Unreleased hyperwarp audio study — 15 October2026

Deep-drive audio study (15 October 2026): original charge/launch/transit/six-crest/arrival sounds now run from real route state in the isolated native hyperwarp candidate. 302,966 bytes resident16-bit PCM, no rejected defect-fan/noise layer; bounded voices and smooth cancellation/restart.85 portable-voice checks plus six actual-route cancellation cases, real route/guards, streamed MP3 and device continuity all pass. Successful sound starts verified: entry1/crests6/arrival1; max3 voices observed, no starvation/decoder errors, no clipped samples in captured device mix. Separate PPSSPP pacing: charge29.62FPS, launch29.97, transit56.77, arrival29.99;84.790ms transition spike remains. Muted-default native audiovisual gallery outputs/hyperwarp-journey-lab/audio-gallery/index.html. ALL future PC testing must remain muted unless user asks otherwise; keep PSP DSP enabled and host volumes0. Authored source/sounds/evidence protected in design/hyperwarp-current/HYPERWARP-AUDIO.md and AUDIO-VERIFICATION.json. Canonical source/playable466 EBOOT unchanged; production integration, listening approval and physical PSP checks pending. No stable-release/GitHub-sync claim.

2.5.466 local PSP-3000 playtest: 28 new natural anomaly types, 384 new sites across all256 systems, fixed balanced geographic quotas (20 regular/13 unusual/5 rare), live shaded crystal/ice/rubble and animated three-dimensional plasma/dust/light forms. Existing four types and slot0/1 locations retained. New sites use existing saved slots2/3; save/load byte-identical. Actual scanning/reports/Codex/rewards integrated; no repeat payment. Availability-aware remaining-signal counts and saved contact identities. Bounded distance detail, cached framebuffer, prepared integer glow and one-pass dust composition. Dedicated all-system save/placement checks pass; broader687 assertions retain identical18 existing failures. Native gallery and final test limits: outputs/releases/2.5.466-natural-anomalies; docs/NATURAL-ANOMALIES-466.md. Stable VERSION/dist retained; physical PSP and crowded-view FPS remain pending; no GitHub sync claim.

2.5.465 local PSP-3000 fleet/discovery detail playtest: all29 standard hulls receive fitted panel art/lighting/glazing/outlets with distance detail; approved Adder retained. All12 capital hulls textured and actual named capital NPCs now use their hull instead of an Anaconda proxy; three freight styles updated. Explicit75-entry discovery visual registry preserves existing8-slot/four-system selection, listening post and wreck; four rift types use world-space depth-checked light. Shared atlas, startup-resident textures, material batches, guarded512KiB command arena and preserved foreground geometry support native480x272 rendering. Saves/game.c/game.h/ships.c/audio/race rules/victory retained. Native hull/catalogue/UV/guards,24 exact projection comparisons, foreground/behind and Game-state checks pass. Exact retail station-flight music about32.86FPS vs33.37 baseline, no starvation/decoder errors; separate active-fleet+engine+traffic+MP3 close-four-ship test remains slow (~20/12/30FPS vs baseline30/15/30), with continuous audio. Existing18 gameplay-suite failures unchanged. Physical PSP, close-crowd FPS and final individual art approval remain open. Package/gallery/source/evidence: outputs/releases/2.5.465-fleet-and-discoveries; details docs/FLEET-AND-DISCOVERIES-465.md. Local playtest only; stable VERSION/dist retained and Git pull blocked by pre-existing untracked music placeholders; no GitHub/stable-release claim.

## 2.5.464 - nearby station hardware panels and FPS (local PSP-3000 playtest)

2.5.464 local PSP-3000 station-performance playtest: nearby visible stations use hardware panel shading with aligned/swizzled authored textures and padded mip levels; exact original CPU visibility/depth is retained for ships/lights/targeting. Near-only 7km/9km hysteresis avoids distant-view transfer regressions. Paired staged moving station draw cost improves27.118→23.462ms,54.229→45.104ms,36.542→27.158ms (~13–26%);72 matched moving poses keep exact depth/guards. Exact-retail supplied-MP3 flight improves29.76→33.27FPS versus previous463 under the same current test; zero starvation/decoder errors and378 actual hardware passes. Engine-only passes33.80FPS; startup passes31,348,480bytes minimum kernel headroom. Native all28 station layouts/256-system roles, six scene depth checks,401,808 material comparisons and real streamed-MP3 jump route pass. This station optimisation changes main.c, station-authored-material.h, mega-city-render.h and new station-ge.h. Concurrent ERL-preview-memory/listening-post/derelict-art changes are preserved in the exact tested retail EBOOT and recorded separately; staged fixture predates the last parallel derelict-art addition. Gameplay/saves/audio/station assets/menus/ERL/victory retained. Package/EBOOT/native comparison gallery: outputs/releases/2.5.464-station-performance. Physical PSP timing/listening, exhaustive switching/sleep/docking, entry/arrival hitch and broader baseline release gate remain open. Stable VERSION/dist452 and USB files retained; no GitHub release/sync claim.

## 2.5.463 - compact hyperdrive HUD and PSP-3000 performance (local playtest)

2.5.463 local PSP-3000 hyperdrive playtest: hardware-rendered Ion tunnel/warp sky/glow, cached native framebuffer work and smaller audio output bursts. Removed HYPERSPACE / SLIPSTREAM banner; charging box reduced38→24px, positioned162..185. Exact-retail startup and MP3 checks pass (29.76FPS, zero buffer starvation/decoder errors); engine-only FPS gate fails at26.29. Previous462 also fails the same comparison at26.58FPS. Exact-retail OGG continuity passes at21.59FPS with zero starvation/decoder errors. Corrected native scripted route profiler improves27.68→44.18FPS overall; settled tunnel approximately60FPS, early charge/arrival near30, arrival hitch~68ms remains. User accepts working non-locked60 playtest. Package/EBOOT/native gallery/evidence: outputs/releases/2.5.463-hyperdrive-performance. No source changes outside six selected renderer/audio files vs task-start snapshot; saves/progression/menus/art/ERL/victory retained. Earlier460/461 combined hyperspace/music harnesses muted radio with comms_quiet=1; their music-labelled timings are rendering diagnostics only, not concurrent playback proof. Exact-retail music checks remain valid; corrected463 harness asserts source/format/device activity. Physical PSP, sustained approach/arrival pacing and broader baseline release gate remain open. Stable VERSION/dist452 and USB files retained; no GitHub release/sync claim.

## 2.5.462 - station texture stability (local PSP-3000 trial)

2.5.462 local PSP-3000 station texture TRIAL: perspective-span footprint filtering and a fixed65,472-byte lit RGB mip cache reduce panel shimmer; full-detail pixels remain nearby. Native matched motion uses the actual renderer:72 views retain identical geometry/depth and framebuffer guards; all28 layouts/256-system role coverage pass. Whole-viewport temporal change falls~30-50% on three staged camera paths. Final staged render30.369ms baseline/36.631ms filtered: material cost remains, not a free FPS optimisation. Exact retail emulator boot and MP3 busy flight pass (30.34FPS, worst83.42ms, no buffer starvation/decoder errors). EBOOT/gallery/source patch/evidence: outputs/releases/2.5.462-station-texture-trial. Main/audio/hyperdrive/sky source files match461 byte-for-byte. Original2x2 station raster and silhouette jaggies remain; no full anti-aliasing claim. Priority: optimise minification cost, improve silhouette coverage, inspect larger close station paths and test physical PSP. Stable VERSION/dist452 retained; USB EBOOT/saves/music unchanged this turn. GitHub DNS still unavailable; no stable promotion, commit/push or sync claim.

## 2.5.461 - PSP-3000 audio and native hyperspace hotfix (local playtest)

2.5.461 local PSP-3000 audio/hyperdrive hotfix: corrected cockpit mixer overflow, reduced fuzzy layers while preserving recorded engines, bounded peak control and faster exact PCM interpolation. Removed 2x2 warp sampling, skips hidden world rendering and prepares arrival sky incrementally. Tested PSP-3000 profile requests larger memory; RAM alone does not produce a large FPS gain. Exact retail engine/MP3 tests pass at 30.42/30.48 FPS, zero buffer starvation/decoder errors; short frame spikes remain. Fourteen authorised USB music tracks converted to native-rate MP3 with verified PC backups; actual battle track improved from 21.45 to 30.64 FPS in emulator, Sol Rock essentially unchanged. Package: outputs/releases/2.5.461-psp3000-audio-hyperdrive. Original music: outputs/music-originals-psp3000-461. USB EBOOT/saves unchanged. Physical PSP listening/frame pacing and broader baseline release acceptance remain open. Stable VERSION/dist452 retained; remote DNS unavailable, no sync/release claim.

## 2.5.460 - Ion Cathedral hyperspace (local playtest)

- Integrates the approved native-rendered Ion Cathedral: fast approach, FOV launch, pixel-art tunnel, moving illumination, bursts, extra streaks and edge electrical arcs.
- Retains target/align/boost charging, real fuel eligibility, jump completion, interception, save format, menus, audio and ERL/victory work. A short cosmetic reveal runs over the actual arrival without delaying controls.
- Bounds startup workspace and caches lighting/lens calculations; native tunnel detail is retained, transitional shading uses2x2 sampling. Original memory guard passes (image13,630,748bytes; headroom3,146,468bytes).
- Production-route native checks pass: charge5.033s, arrival10.033s, framebuffer guards. Render-only staged mean28.530ms; cold destination sky still produces a slow frame. Exact retail original-model startup/MP3/OGG continuity pass, zero audio starvation/decoder errors; ordinary busy-flight30.40/22.17FPS. Physical PSP and exhaustive visual/suspend/cancellation acceptance remain pending.
- EBOOT and native captures: outputs/releases/2.5.460-ion-cathedral. Stable VERSION/dist remain452 because broad baseline regressions are unaccepted; not a GitHub release or sync claim.

## 2.5.452 - music continuity and boost-stable stations (local hotfix)

- Separate the deadline-driven audio device thread from variable-time MP3/Vorbis decoding and mixing. A bounded five-block PCM queue primes before playback and retains two submitted buffers until safe reuse. Audio work outranks busy rendering; native-rate music bypasses cubic resampling. Recorded PCM interpolation uses bounded 32-bit arithmetic.
- Keep a 48 KiB encoded MP3 reservoir and the existing original-PSP retail memory guard. Add actual device-buffer starvation diagnostics, rather than treating SFX command drops as proof of continuous music.
- Make station facade mapping and face visibility use the real shaken/chase camera. Derive mapping axes directly from rotation to avoid large-coordinate cancellation; 48 native first/third-person boost samples stay within 0.001 units. Retain latest station lighting and mesh detail; cache repeated station profile/rotation/vertex work.
- Saves, progression, menus, music folders, controls and unrelated work are retained. A short bounded audio queue adds buffering latency; separate low-latency SFX scheduling is not part of this hotfix.
- Native PPSSPP diagnostics and exact-retail original-32-MiB-model playback checks are recorded in outputs/releases/2.5.452-audio-hotfix. Physical PSP confirmation remains pending. OGG decoding still costs frame time: the stress scene runs about 27 FPS, so this is not a claim that all station FPS work is complete.
- Local checkpoint only: the shared tree contains extensive other contributors' unfinished work. Pull/rebase was blocked by pre-existing untracked music placeholders; autostash was restored. No GitHub release/tag or clean-tree claim.
Ordinary-flight lighting side checkpoint (2026-10-13): sun halo, directional station/rim light and lamp glow integrated without changing saves, ERL or ship meshes. Local tested retail EBOOT: outputs/releases/2.5.450-lighting-playtest/ELITE-NEXT-PSP/EBOOT.PBP. Evidence/limitations in READ-ME.md alongside: original-PSP-model emulator flight/music/ERL startup/memory checks pass; close station rendering remains above 33 ms, physical PSP unverified. Local O2+LTO source snapshot, manifest and focused patch: outputs/space-lighting450. Shared VERSION/dist/compiler settings and GitHub unchanged to preserve concurrent work. Parent ERL handoffs remain open.

2.5.416 PLAYTEST checkpoint: first round ERL circuit has a quarter-lap magnetic sector, real booster launch/gap/landing, and retained menus/V37 progression/victory/Picross. 380 ERL checks and five exact-retail startup/MP3 checks pass. Full-grid all-hull circuits take about 131–135 seconds. Retail magnetic/jump test: 40.93 FPS, worst 50.05 ms, zero >100 ms frames, six completed saves, Sol Rock; 55 FPS target still fails. Physical PSP, full Apex acceptance and remaining handoffs are unfinished. Playtest EBOOT: outputs/releases/2.5.416-preview/ELITE-NEXT-PSP/EBOOT.PBP. Latest stable remains 2.5.415. GitHub unchanged.

2.5.415 playable local checkpoint: Mission Board sponsor rewards now deliver the exact advertised enhanced equipment, or the disclosed full-queue cash alternative. Final board results are saved before Continue closes them, including when an older checkpoint is writing. 149 production integration and 39 real-control flight/save checks pass; full ERL/native regressions pass. Exact retail: 57.58 FPS, zero >100ms frames, four completed saves, startup/MP3 verified on original 32 MiB emulator model. Menus/visuals remain 414 pending the user's finalized side-chat handoff. Physical PSP pending; full arcade acceptance remains open; GitHub unchanged.

2.5.414 playable local checkpoint: expanded ERL contracts, competitive green-ring AI, eliminator final sprint, V37 career records, exact-once rewards, independent seasons, factual LocalTV and sponsor equipment. 137 integration checks and 147 full-console checks pass; native HUD/track/flight/victory/TV/progress suites pass. Exact retail 57.58 FPS, no >100 ms frames, startup/MP3/four saves verified in original 32 MiB emulator model. Broader core retains the same 15 baseline failures; not an all-green release. Full handoff visual acceptance remains open. Physical PSP pending; GitHub unchanged.

2.5.413: pink holographic racing track, compact animated position/lap/live HUD and subtle rival damage cues. 15 native floor/HUD checks and 44 gameplay checks pass. Exact retail: 55.26 FPS populated countdown/race test, worst 33.37ms, no frames over 100ms, four completed saves, safe worker stack; five startup/MP3 checks pass on original 32 MiB emulator model. Physical PSP confirmation pending. Full arcade handoff remains unfinished; GitHub unchanged.

## 2.5.412 - reopen ERL with a pending race

- Let accepted pre-start races fall through to headquarters and Command Deck menu handling.
- Keep Season, Driver, Rivals, Garage, Back and selection animations responsive.
- Require the actual start beacon for READY; preserve pending course and progression.

## 2.5.411 - intro skip reveals the title

- Fade the ELITE: NEXT logo in with the menu when X is pressed early.
- Keep the moving film, audio timing and existing input protection.

## 2.5.410 - ERL pre-start flight and sleep recovery

- Keep ordinary flight HUD before READY and clear stale pre-start pause flags.
- Pause on sleep only during an actual countdown or race; resume/withdraw remain operable.
- Preserve contracts, saves, rewards, Shockwave victory and newspaper Picross.

## 2.5.409 - native Shockwave Flyby victory

- Replace the checkerboard win animation with immediate launch, continuous camera chase, world-space twinkling stars and depth-tested shattering numeral.
- Use the player's actual ship, selected paint and engine roots, animated exhaust/vapour, sunlit finish and gently moving 1ST PLACE title.
- Keep exact rewards, progression, audio restoration, replay and result controls. No save-format change.
- Bound effects and share the inactive Command Deck scratch framebuffer to avoid expensive display-memory blending and extra memory pressure.
- Full arcade expansion and newspaper work remain paused; this release adds the approved victory sequence to 2.5.408.

## 2.5.408 - first ERL arcade combat playtest

- Seven-racer, two-lap combat exhibition with temporary items, Vector HUD, green boost rings, nearby violet boundaries and rectangular repair tunnels.
- Keep the approved ERL menus, branding, motion and authored victory reveal. Add physical countdown lights, live rear view and bounded recent-action replay.
- Independent system championships and V35-to-V36 migration preserve earned fleet and pending prizes.
- Early playable build: full arcade handoff remains unfinished and paused for feedback.

## 2.5.407 - consistent ERL branding

- Keep league colours independent of Pimp-my-Ship across all ERL pages and conversations.
- Preserve actual ship paint on Garage previews, current animations, V35 saves and the countdown stack fix.

## 2.5.406 - ERL countdown autosave stack fix

- Increase checkpoint worker stack from 64 KiB to 256 KiB for full save validation.
- Measure unused worker stack and test live countdown with music and concurrent saves.
- Preserve race rules, menus, animations and V35 compatibility.

## 2.5.405 — responsive ERL motion

- Add Broadcast Neon focus rims and brief action pulses across ERL.
- Add interruptible Slipstream transitions and aligned Paddock Signal portrait reveals.
- Animate XP fills, subtle stars, Season routes and Garage platform lights with bounded native drawing.
- Keep input immediate, authored victory animation, other UI layouts, race rules and V35 saves.

## 2.5.404 — Events and Rivals reference layouts

- Match the approved diagonal event hero, horizontal event cards and 3x2 Rivals paddock.
- Show larger remaining qualifier XP and a segmented progress bar.
- Preserve existing race logic, save format and other approved ERL pages.

## 2.5.403 — ERL visual race cards

- Implement approved option 1 HQ, briefing and reward-card layouts with authored icons, clear XP panels and real outcome values.
- Place the event label directly beneath the large course name in the left briefing banner.
- Match the native palette, geometry and bottom action buttons across ERL; remove redundant control legends.
- Preserve checkerboard reveal, championship circles, controls, race rules and V35 saves.

## 2.5.402 — Hyper Circuit ERL

- Implement the selected neon ERL design across headquarters, events, briefs, Season route, driver, Rivals, Garage, standings, conversations, pause and confirmations.
- Show segmented earned/remaining XP and the exact amount needed; handle qualifier-ready and Elite states.
- Keep ship paint themes, controls, career rules and V35 saves. Preserve the checkerboard/trophy victory reveal, followed by the Hyper Circuit result composition.
- Cache one native race preview and precompute orbit graphics to bound menu cost.
- Match the approved championship's 47px circular markers and double-size round numbers; keep the prize line simple. HQ labels its large round number above the artwork rather than repeating it under the race title.

## 2.5.401

- Keep Introduction to the ERL as a stable mission title through referral and complete it upon rookie qualification.
- Remove the conversation speaker panel page counter; retain speech bubble paging arrows.

## 2.5.400 — ERL checkpoints, introductions and navigation

Active ERL racing queues an immutable V35 save snapshot every five seconds on a lower-priority worker. Pause, results, menu changes, sleep and shutdown join the writer before later save/load actions. Only one bounded snapshot may exist. This removes synchronous checkpoint file writes from flight; physical PSP frame-time confirmation remains pending.

Boost rings are green and grant a short free burst with peripheral green speed streaks. Lave's NICE FLYING / MEET TORR BELL contact is first on the mission board with a moving rainbow sheen. Stable contract IDs and existing jobs are unchanged. Torr congratulates test winners and plots the nearest reachable Mega Capitol; Lave never opens the league dashboard after the test. First-time HQ visitors read an organiser introduction explaining the free rookie qualifier and championships before seeing the rookie league card. The acknowledged introduction uses an unused bit in the existing ERL inbox field; the V35 format is unchanged.

Circle returns Mission Log, Mission Board, Galaxy.Maps (including search) and System Details straight to the Command Deck, clearing shortcut history. Paged conversations put R and a right arrow inside the speech bubble's bottom-right corner, with L/back on later pages. Printed newspapers and other local work are retained. Newspaper expansion is paused at the user's request.

## 2.5.399 — approved newspaper first batch

NEWS now includes five complete original issues in the approved native print styles: Lave 16 pages, Diso 12, Quator 12, Riedquat 10 and Leesti 8. Twenty layouts use real font8 lettering, fixed publication palettes, native illustrations, letters, classifieds, long-feature continuations and working puzzles. All 58 pages have individual visual notes and hashes in the completion ledger. Other GalacticNet sections and V35 saves are preserved.

Paper ink bypasses ship-paint tinting. Correct puzzle instructions and printed apertures replace mismatched wording and phantom back-page actions. Existing 1,000-unit prizes, persistence, navigation and Earth onboarding remain intact. Retail startup/audio and all five front pages pass emulator checks; physical PSP playtest remains pending. The other 251 galaxy editions remain drafts, not completed publications. See docs/NEWSPAPERS-APPROVED-PROGRESS.md. Local release; GitHub is unchanged.

## 2.5.398 — DockBay physical marketplace

GalacticNet MARKET now uses the approved DockBay v3 cream classifieds at
native 480×272. Three adverts per page, filters, MY DEALS, exact equipment
figures, free navigation data and physical named seller rendezvous replace
the previous market website. Other GalacticNet sections remain intact.

Fixed-price, untimed equipment and bulk-cargo lots retain their remaining
stock. Cargo purchases and buying notices transfer only the chosen batch;
mission-reserved goods stay protected. Permanent worn, modded and rare
module instances preserve their exact rolled statistics through storage,
fitting, resale, ship exchange and the ERL Garage. Comparable effectiveness
is bounded at twice normal, including combined weapon damage and cadence.

Twenty encounter flavours include mechanics, travelling shops, convoys,
pre-trade traps, post-trade attacks and outside pirates. The exact advertised
seller owns the conversation and goods. Five distinct peaceful completed
meetings grant permanent trust; several purchases during one shop visit
count once. Trusted sellers never betray. Real combat allows fighting or
escape; paused transmissions cannot damage the player. Unpaid wreck modules
can be recovered once, retaining their original roll through reloads.

Validated additive V35 saves migrate earlier fitted and stored equipment
as normal instances. Replace only EBOOT.PBP; keep saves, settings and music.
See docs/DOCKBAY.md and outputs/releases/2.5.398/BUILD-2.5.398.md for controls,
verification and limits. Physical PSP-2000/ARK playtest remains pending.
Local release; GitHub has not been updated on the shared development tree.

## 2.5.397 — Elite Racing League

Playable ERL career in the approved native 480×272 Broadcast design, with
starfields, real-course preview art, solid scenery and the compact race HUD.
Lave mechanic Torr Bell offers the introductory test; a persistent referral
leads to a reachable Mega Capitol HQ. WORK > ERL LEAGUE includes qualification,
four local championship rounds, Driver, Rivals and the shared ERL Garage.

Regulated own-ship racing, limited boost, actual timed takedown disciplines,
repair drones, protected cinematic handovers and sticky official results.
The revised checkerboard flight reveals the gold trophy once, then holds on
actual rewards. Races play the real Sol Rock MP3 and restore audio settings.

Permanent Krait/Ophidian/Solar Clipper/Mirage division prizes preserve the
original ship, equipment, appearance and condition. Cargo/storage overflow
blocks swaps. Exclusive usable modules defer claiming until storage has room.
Validated V34 saves add five-second race checkpoints and paused recovery.
Other local development is retained. See docs/ERL.md for rules and controls.

Verification: outputs/releases/2.5.397/BUILD-2.5.397.md. Physical PSP-2000/ARK
playtest pending. Local release; GitHub remains unchanged on the shared tree.

## 2.5.396 — GalacticNet Crime Watch

Wanted uses the approved concept 6 native 480x272 Crime Watch composition,
real 8x8 game glyphs, original space art and corrected complete portrait tiles.
D-pad Left/Right browses five warrants; shoulders L/R switch sections;
Up/Down and X do not track or launch. Circle returns after a 120ms reverse wipe.
260ms entry wipe, 180ms clipped new-portrait arrival, quiet signal lamps,
faint 8% green portrait tint and viewport-fixed scanlines drifting at 4px/sec.
Names, rewards and risk change together; rapid input always wins.

Actual board bounties now pay 600–2,500 units, derived from danger and warrant
priority. The board and destruction path read one reward function. Explicit
claimed-state protection prevents a stale/reanimated actor paying twice.
Five decoded source tiles are cached once per system; no per-frame asset decode
or allocations. Existing portrait repairs are retained; older design captures
with the fish/hooded shoulder strips are not copied into the new screen.
No save-format change. Other GalacticNet sections, Rumours adverts and races retained.
Verification: outputs/releases/2.5.396/BUILD-2.5.396.md.
Local release; physical PSP testing pending. GitHub unchanged on shared dirty tree.

## 2.5.395 — Daft Rumours pop-up adverts

Rumours has 24 original joke adverts with individual authored pixel pictures.
The first appears after eight seconds of browsing; subsequent ads wait 22-35
seconds after dismissal. Triangle closes them with a 110ms shrink and thwip;
opening uses a 140ms grow/settle animation and a rising whoop. Adverts capture
underlying offer inputs, never charge units and never appear in SpacePay.
L/R categories and Circle Back remain available. No extra framebuffer or heap
allocation; native artwork and text scale together in the short transition.
Built on local 2.5.394; NEWS controls, icons, Rumours offers and races retained.
Verification: outputs/releases/2.5.395/BUILD-2.5.395.md.
Local release; physical PSP testing pending. GitHub unchanged.

## 2.5.394 — NEWS page controls

In GalacticNet NEWS, Left/Right turns the newspaper pages with existing wrap,
paper sound and turn animation. Up/Down remains an alternate page control.
Only L/R switches the top categories: L goes to Rumours; R goes to Market.
The footer now shows directional arrows for PAGES and L/R for SECTIONS.
Puzzle editing keeps its own D-pad/shoulder controls. No other sections changed.
Built on local 2.5.393, preserving its icons, Rumours advert and race updates.
Verification: outputs/releases/2.5.394/BUILD-2.5.394.md.
Local release; physical PSP testing pending. GitHub unchanged.

## 2.5.393 — Clearer navigation and shipyard icons

System details uses a planet, sun and orbital arc. Galaxy.Maps uses a spiral
galaxy. Disembark uses a walking shoe. Regular Shipyards show a swept-wing
spaceship, and Mega Shipyards show a distinct broad capital ship.
All are authored 12x12 pixel silhouettes using the existing live ship palette.
Menu actions, animations, service availability and layouts are unchanged.
Built on local 2.5.392; its Rumours advert and the 391 race work are retained.
Verification: outputs/releases/2.5.393/BUILD-2.5.393.md.
Local release; physical PSP testing pending. GitHub unchanged.

## 2.5.392 — Rumours explanatory advert

The Space Secrets banner now explains the service in its existing 448x38 area:
"PAY UNITS FOR NAVIGATION DATA."
"DISCOVER HIDDEN PLACES IN SPACE."
The native 8x8 lettering, eye graphic, offers, scrolling and SpacePay flow remain.
Built on the local 2.5.391 race update. Saves and player music are unchanged.
Verification and release evidence: outputs/releases/2.5.392/BUILD-2.5.392.md.
Local release; physical PSP playtest pending. GitHub unchanged.

## 2.5.391 — Race boost reserve and readable opponents

Racing now uses a four-second boost reserve with recovery between bursts and
24% gold-ring refills. Race cruise remains quick, and race energy replaces
travel-fuel use during the attempt. Ordinary flight boost is unchanged.
The race HUD shows reserve, recharge and a release-R prompt when exhausted.

Rival hulls are 50% larger with matching bounds and stronger burst exhaust.
Opponents manage reserves, boost on straights and slow into turns, using
the player's fitted performance class for speed, handling and acceleration.
Courses scale to that class for roughly two-minute races, with tighter gates
in each spectacle sector. There is no teleporting or position rubber-banding.
Saves, Style15 conversations, Sol Rock playback and PSP boot fixes are retained.

Verification: docs/RACE-BOOST-391.md and release test evidence.
Local release; physical PSP playtest pending. GitHub unchanged.

## 2.5.390 — Portrait Broadcast conversations

Implements approved style 15 on the local 2.5.389 PSP launch hotfix: native
480x272 monitor surround, full-colour caller portrait, a cream speech bubble
pointing toward the caller, animated waveform and large cream/amber reply keys.
Story, missions, trades, private channels, computer reports and local-law screens
share the layout. All 155 staged mission calls and contact records use it too.

Left/right chooses reply cards; up/down remains available. Four police replies
form a 2x2 D-pad grid. X/Circle keep existing actions. L/R pages longer speech.
Objectives and cargo offers have reserved space; long labels stay readable.
The Comms Panel settings console and mission reward cards are unchanged.

Retail excludes the embedded QA suite and retains the 2.5.389 memory gate.
V33 saves, staged contracts and the 23-second intro-music offset are preserved.
494 focused checks pass; 155 staged-call screens captured at native resolution. The broad
core suite still reports its 15 pre-existing failures. Verification and native
layout: docs/CONVERSATION-BROADCAST-390.md.
Local build; physical PSP confirmation pending. GitHub unchanged.
## 2.5.389 — Physical PSP launch hotfix

The retail EBOOT excludes the embedded automated test suite, freeing about
4.4 MiB of resident memory. Existing missions, races, artwork, saves and music
fixes remain. Firmware/codec headroom rises from 435,736 to 5,082,560 bytes
before runtime allocations. Build scripts enforce a retail memory reserve;
full QA builds use separate outputs/review-build paths via -EnableReview.
Startup writes boot-status.txt for hardware diagnosis without continuous logging.

The exact retail EBOOT passes PSP-1000/2000 emulator startup with the supplied
Elite Exploration MP3, command deck/flight rendering and live dream with the
48 kHz Sol Rock MP3. Native 480x272 captures inspected. Review-suite compilation
preserved. These checks do not confirm the physical ARK launch fix; PSP retest
is the next priority. No gameplay algorithm changes or save migration changes.

Details: docs/PSP-BOOT-HOTFIX-389.md. Local release; GitHub unchanged.

## 2.5.388 — Staged contracts and music repair

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

Controls and full catalogue: docs/STAGED-CONTRACTS-388.md and docs/MISSION-STAGES-388.md.
Validation: 917 focused checks pass; 664 core checks pass with 15 inherited core failures. Input has 165 inherited failures independently reproduced on 2.5.387; no additional core/input failures. Exact user MP3s tested; native captures inspected. Optimized EBOOT SHA256 EAEA1BF259DABC19836D5FB867847EA4E336949FEF33890CA73A3B05FDD95EC3. Physical PSP playtest pending. Local release; GitHub unchanged.

Highest-priority remaining work: physical PSP mission/music/cold-boot playtest, difficulty tuning, inherited reliability failures, authored portraits and paused planetary expansion.

## 2.5.387 — Winding arcade races and takedowns

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

Validation: 572 focused emulator checks pass, including 71 race checks and actual MP3 playback/return. INFO production flight: phase 5 gate 32 place 1 time 119.85 heat 0.0 fuel 47.1 hull 100.0. Core: 664 pass with the same 15 inherited failures. Native 480×272 captures inspected. Optimized EBOOT SHA256 7A4D203DC55FD2A16CFB906314BCBBEE30B3293898C3C7068685C2C35D688472. Physical PSP confirmation pending. Local release only; GitHub unchanged after the earlier shared-tree pull/rebase approval rejection.

Highest-priority remaining work: physical PSP race/cold-boot playtest, tune course and collision feel from feedback, then inherited reliability failures, authored portraits and paused planetary expansion.

## 2.5.386 — Restore two seconds of intro music

Elite Exploration now starts 21 seconds into the intro song, restoring only
the most recent two-second cut. The one-second fade and all 2.5.385 features,
including racing and the display-startup repair, remain unchanged.

Validation: 37 real-MP3 intro checks and 2 normal-boot checks pass on the exact
optimized EBOOT. All other production source hashes match 2.5.385. That
release's broader regression results are retained, not newly rerun; the same
15 inherited failures remain unresolved. Physical PSP confirmation is pending.
Local release only; GitHub unchanged after the earlier shared-tree approval rejection.

Highest-priority remaining work: physical PSP cold-boot/race playtest, then
inherited reliability failures, authored portraits and paused planetary expansion.

## 2.5.385 — Local space races

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

Validation: 548 focused emulator checks pass, including 47 race checks and real MP3 override/return. An unmodified ship wins through production flight physics without teleporting or altered stats; every rival finishes through real crossings. Mission transmissions, intro, opening, radio, steering and normal boot checks pass. All 1280 original contracts still complete and pay once. Core: 664 pass with the same 15 inherited failures. Native 480×272 captures inspected. Optimized EBOOT SHA256 904D9CDDBAE902153D75DD0C6997670CB555516CFC9376D19581AD40358B7E48. Physical PSP boot/audio/controls/race FPS remain a playtest gate. The 2.5.384 SDK display initialization and intro remain intact. Local release only; GitHub unchanged following the earlier automatic approval rejection of shared-tree pull/rebase.

Highest-priority remaining work: physical PSP cold-boot and race playtest; tune course/difficulty from feedback; then inherited reliability failures, authored portraits and paused planetary expansion.

## 2.5.384 — PSP screen setup and mission-completion transmissions

Responds to the physical PSP black-screen regression reported in 2.5.383: restores the PSP SDK's full display initialization and its first NEXTFRAME latch. Keeps both planes black and prepares the first movie frame offscreen. The display change is the suspected regression cause; hardware confirmation is still required. Intro film, 23-second music skip, one-second fades, CuLa tiles and silence during hidden setup are retained.

Actual mission payouts now queue a native 480×272 transmission card with the correct contact portrait, natural thanks, actual reward, animated warp arrival, signal lamps and fireworks clear of the text. Board missions, Guild claims, first flight, story chapters, station quests and passenger transfers are covered. Simulation pauses during the call. Fresh X acknowledges exactly one card, with an input-release gate before normal controls resume. Several dock completions receive separate cards; acknowledgements never award money. Receipts are transient and bounded; existing save V32 persists payment and progress without replaying celebrations after load.

Validation: 501 focused emulator checks pass, including 37 new mission-card integration checks, normal boot, full intro with a real MP3, opening, radio and steering. All 1280 galaxy-wide contracts verify one correct receipt through real completion paths. Core: 664 pass with the same 15 inherited failures. Native screenshots inspected. Physical PSP black-screen fix needs playtesting. Exact optimized EBOOT SHA256 B141379815F7CEA9F1B7E77EB7B530BA2BC5C36B52B714C7FF2C5807BC2E0C65. Local release only; GitHub unchanged following the earlier automatic approval rejection of shared-tree pull/rebase.

Highest-priority remaining work: confirm cold boot and X-to-menu on the player's physical PSP before claiming the regression resolved; check mission celebrations in play; then remaining authored character parts, inherited reliability failures and paused planetary expansion.

## 2.5.383 — Clean intro startup

Intro music starts two seconds later (23 seconds into the track), preserving its one-second fade and saved radio preferences. “CuLa Games presents...” and its red Cu/blue La tiles fade in together over one full second. Intro entry and Select replay clear the hidden setup greeting and its queued comms blip. Cold boot initializes both full display planes to black before publishing a framebuffer, then decodes the first cinematic into the hidden back plane so setup and themed clear colours cannot flash on screen. Embedded film and unrelated game source are unchanged; no new buffers or save changes.

Validation: 464 focused emulator checks pass, including real MP3 decode/skip, startup display, title fade, replay, opening, radio, steering and normal boot. Native 480×272 captures inspected. Core 663 pass with the same 15 inherited failures; physical PSP testing pending. Exact reviewed candidate copied to root, dist and versioned release; SHA256 0ABFA5C02F97A97FCD0FEB2E6A101C72302080BA32BFC40C05C263CB2104B150. Local only; GitHub not updated following the earlier automatic-approval rejection of pull/rebase on the mixed shared working tree.

Next: PSP startup playtest; then remaining authored character parts, inherited reliability fixes and paused planetary expansion.

## 2.5.382 — Illuminated station departures

Rich-station departures show illuminated gantries, adverts and port-side architecture. Mega capitals add deeper layered districts, transit bridges, side-facing windows and passing solid ship meshes. Station-seeded trim; poor and relay berths keep simpler styling. Bounded presentation-only geometry and aperture clipping preserve the five-second launch, actual sun, input lock/release gate and simulation state without extra runtime allocations. All 256 station profiles clear decorative geometry before manual handoff, avoiding a final-frame pop.

Station departure, station docking and planetary transfers share a slowly pulsing green AUTOPILOT ENGAGED indicator. Removed departure guidance footer, docking guidance wording and planetary stage captions; planetary indication sits in the canopy footer. PSP sleep recovery retains its hardware/audio/control restoration and removes the redundant message.

Validation: 529 focused emulator checks pass; actual native 480×272 departure, docking and planetary captures inspected. Core 663 pass with the same 15 inherited failures; physical PSP testing pending. Unrelated source and embedded intro hashes match 2.5.381. Exact reviewed candidate copied to root, dist and versioned release; SHA256 11F97BDFADAE3C0245F411A519D51350332849E52880609BD37F3B7E02FFC7F6. Local only: automatic approval rejected pull/rebase of the mixed shared working tree; GitHub not updated.

Next: playtest the richer departures and guidance readability on PSP, then resume authored character-part work, inherited reliability fixes and paused planetary expansion.

## 2.5.381 — Continuous approach and CRT dashboard startup

The saved-station approach keeps zooming throughout its fade to black, using a lens push after safe physical camera travel ends. Dashboard navigation categories flicker independently in a shuffled order before staying on. Actual station/sun background art expands from a thin CRT beam into the full image. Effects settle to the exact normal menu pixels; no extra framebuffer, save changes or simulation advancement. Select intro replay, creator Back and startup sounds are preserved.

Validation: 558 focused emulator checks pass; native 480×272 station/startup captures inspected. Core 663 pass with the same 15 inherited failures; physical PSP testing pending. Unrelated source and embedded intro hashes match 2.5.380. Exact candidate copied to root, dist and versioned release; SHA256 F16BC48C47693113B0C3826F447BAB3D479C60B9829A074A83B66DBD30D9891E. Local only: automatic approval rejected pull/rebase of the mixed shared working tree; GitHub not updated.

Next: playtest the continuous zoom and CRT startup on physical PSP, then resume authored character-part work, inherited reliability fixes and the paused planetary expansion.

## 2.5.380 — Station arrival and dashboard startup

Main-menu Continue/Load now shows the actual saved orbital station with an orbit, accelerating approach, black transition and staggered dashboard lighting with three original synthesized startup sounds. Select replays the intro; Circle cancels New Commander creation. Existing saves and radio preferences remain intact. No additional framebuffer or simulation advancement.

Validation: 544 focused emulator checks pass; native 480×272 station/startup captures inspected. Core 663 pass with the same 15 inherited failures; physical PSP testing pending. Unrelated source and embedded intro hashes match 2.5.379. Exact candidate copied to root, dist and versioned release; SHA256 07A508F733BA37E6D22D6AA4FD42796476467D77DEE81DE9FF84AD4A718E2966. Local only: automatic approval rejected pull/rebase of the mixed shared working tree; GitHub not updated.

Next: playtest station framing/startup sound levels on physical PSP, then resume authored character-part work, inherited reliability fixes and the paused planetary expansion.

## 2.5.379 — Clean legacy photos and authored female parts

Corrected source-sheet row boundaries for all 96 legacy portraits, removing previous-row shoulder strips at the crown. Legacy whole-face colours remain authored instead of broad hair/outfit recolouring that tinted necks. Face identity, saved DNA and v2/v3 rig alignment remain intact. Female Human/Android Cropped hair, Smirk and extended/pointed ears now use native authored parts; ear choices cannot restore rectangular bob-hair patches. All-category fit/mask/palette production standard is recorded in docs/DESIGN-BIBLE.md; remaining transformed choices still require conversion and visual review. Preserves 378 Engineers preview, 377 title and earlier work.

511 focused emulator checks pass, plus 17,993 host checks and 1,265 independent source checks. Actual native editor/profile/comparison screenshots inspected. Lossless atlas packing saves 243,024 bytes, with all twelve modular rig source tiles byte-identical. Core retains the same 15 inherited failures. Physical PSP testing and the player's exact Ben/Jameson save verification remain pending. Details: BUILD-2.5.379.txt. Highest priority: player profile playtest, remaining authored creator conversions, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.379. SHA256: C31E97445B4DBF0B1F517483D7E54F960948AC6ECB8EA2E4A41B0160B59A4EE3. GitHub not updated: existing untracked music placeholders block pull; autostash restored and mixed shared work preserved.

## 2.5.378 — Your own ship at Engineers

Engineers now shows the commander's actual hull and current paint using the same renderer as Shipyard, including all Mega ships. The model turns gently inside a native 246×64 aperture, clear of diagnostic labels. Repair/refuel behaviour and saves unchanged. Preserves the 377 fading CuLa title and earlier work.

34 focused emulator checks pass: six Engineers checks, 26 existing Shipyard checks and two startup checks. Actual hull pixels verified for all 22 designs at three angles and every paint finish on regular/Mega ships; repair/refuel input and prices verified. Five native 480×272 captures inspected. Physical PSP testing and 15 inherited broad failures remain open. Details: BUILD-2.5.378.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.378. SHA256: 8541F327D530186180B33CEDDE7F4A31A843B3A2C7710A982F7485F95A3FC66C. GitHub not updated: pull blocked by existing untracked music placeholders; autostash restored and mixed shared work preserved.

## 2.5.377 — CuLa presentation title

Opening credit now reads “CuLa Games Present”, with a red 16×16 square behind “Cu” and a blue square behind “La”. Lettering and tiles share the 800ms fade-in and existing fade-out; the full title is centred at native 480×272. Cinematic, music, menu timing, portraits and saves unchanged.

27 focused emulator checks pass, including exact glyphs, tile bounds, blending, fade timing, film/menu integration and normal boot. Seven native title captures inspected. Physical PSP testing remains pending; the same 15 inherited broad failures remain open. Details: BUILD-2.5.377.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.377. SHA256: 23FCC6311DBA651A569673D61D9BF2E53F2E9EB2328CF21F13D0C3A97CCCA1B6. GitHub not updated: pull blocked by existing untracked music placeholders; autostash restored and mixed shared work preserved.

## 2.5.376 — Expanded Commander portraits

Added Male/Female Human and Android rigs and four hairstyle choices with separately traced anatomy. Twelve hair colours use stronger four-tone shading on female/new styles; Classic male palettes preserve existing art. Eye styles expand from three to five on rigs with eyes. Correct female eye, mouth and ear alignment, readable cheek freckles, crown padding and a one-screen native 480×272 editor. Changing features retains the other selected values, including inactive choices when entering the editor. Old appearance records and V32 save structure remain compatible; expanded appearance uses a validated four-byte encoding. Preserves the 375 Mega shipyard and 374 conversation fixes.

559 focused/startup/radio/steering emulator checks pass. Portrait input and real saves verified; 800 profile photo/theme comparisons; actual native editor/profile screenshots inspected. Host: 17,993 checks and 9,774 render cases pass. All 330,912 older combinations rerendered at three sizes with byte-identical render records. Core has the same 15 inherited failures; physical PSP testing remains pending. Details: docs/COMMANDER-CREATION-376.md and BUILD-2.5.376.txt. Highest priority: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.376. SHA256: 5983E97F198C7F7898667FEA9E3A2738E39E3DA0DBF4E0FED4F5DB9DBF7A94E7. GitHub not updated; existing untracked music placeholders block pull and mixed shared work is preserved.

## 2.5.375 — Shipyard identity owns its layout

Mega shipyards keep their premium layout for every listed hull, including basic ships. The dealer's mega-stock capability now chooses the layout; the highlighted ship does not. Lave's complete mega showroom also stays premium throughout its 22-ship list. Ordinary dealers retain their regular layout and regular stock, including when the commander owns a capital ship. Dealer names, pictures, stock lists, prices and purchases are unchanged. Preserves the 2.5.374 conversation ownership fix.

28 focused emulator checks pass: 26 shipyard checks and two startup checks. Native 480×272 captures inspected for Lave basic/capital, regional Mega basic/capital and ordinary dealers. Physical PSP testing and inherited broad failures remain open. Details: BUILD-2.5.375.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.375. SHA256: F835F07D31BEB6914AD25AE2512AA0DC8DED79DF25075FC3F467636F7A4CA091. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

## 2.5.374 — Correct conversation ownership

Fixed discovery contacts inheriting another pilot's encounter replies and consequences. Sanctuary buoys now use Ranger speech and a close-channel option. Contact-only station and pilot hails also retain their own speaker and text. Incoming replies explicitly belong to a caller, encounter and system; the panel snapshots and validates the transmission before acting. Quiet Watch verifies its current speaker. Promising convoy assistance no longer claims a successful rescue or grants an unconditional 350 units; this fix does not add a new ambient convoy completion/reward loop. Mission-board escort rewards remain unchanged. Save format unchanged; all 2.5.373/372 fixes retained.

106 focused emulator checks pass, including 19 conversation-routing matrix checks, 66 existing Comms checks, Codex/reward persistence, Quiet Watch and startup. Native 480×272 sanctuary capture inspected. Core comparison against 2.5.373 has the same 15 inherited failures; radio, steering and NPC combat checks pass. Physical PSP testing remains pending. Details: docs/CONVERSATION-OWNERSHIP-374.md and BUILD-2.5.374.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.374. SHA256: F8CE5E45C803398BA08E1FB3796881D9626D2055CC626BF28EC43370D44FD464. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

## 2.5.373 — Silent empty targeting categories

Removed the computer's “No contacts in that band” message from both category switching and list cycling. Empty bands remain selectable, keep the current target and leave any active message alone. Preserves the 2.5.372 Codex progress fill and checkpoint illumination.

68 focused emulator checks pass: 66 Comms Panel and two normal boot checks. Both silent branches verified in source; all other baseline-tracked sources and the embedded cinematic are unchanged. Physical PSP testing and inherited broad failures remain open. Details: BUILD-2.5.373.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.373. SHA256: CA7FD7FD3EBB2EB7C0288B5722119BFE07549D91B7D0549BB1E3386DC3DE6A9E. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

## 2.5.372 — Discovery Codex progress fill

System Record now visibly fills in the ship signal colour: its frame draws first so it no longer covers the interior. Every reached ten-percent arrow lights up, claimed checkpoints stay gold and future ones remain dim. Exploration calculations, rewards, controls and save formats are unchanged. Preserves the 2.5.371 menu/cargo fixes, clean Commander Profile and 21-second startup music skip.

69 focused emulator checks pass: 11 progress checks (every percentage from 0 through 100 in all sixteen themes, actual dossier renders, arrow states and guards), five existing Codex reward/persistence checks, 51 Command Deck and two startup checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/CODEX-PROGRESS-372.md and BUILD-2.5.372.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.372. SHA256: AC89BB9D35DF6DAFF1A7F5ABCEE9963BDF6AA8FCAB855B00218E33CB7ECAD294. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

## 2.5.371 — System readouts and menu controls

System Details shows stars/worlds counts underneath the label. Comms Contact/Docking navigate directly across their row. Cargo Square jettisons one recoverable consignment in open space, protecting mission cargo and preventing loss when the debris pool is full. Left/Right trade popups removed. Ship Loadout's heading sits lower; footer shows Square Store and Item Storage with no L/R hints. Square stores the selected docked bank with bounded confirmation or refusal reason. Existing shoulder category shortcuts remain. Preserves clean profile portraits, centered profile buttons and the 21-second startup music skip.

198 focused emulator checks pass: 17 new menu checks (including all 256 system readouts and actual jettison/tractor recovery), 17 Cargo, 24 Loadout, 66 Comms, 21 Commander Profile, 51 Command Deck and two startup checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/MENU-FIXES-371.md and BUILD-2.5.371.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.371. SHA256: C479CD4FF814BFBDB7551FFD9F85790999F073BB6068F5C644B88DB3EF3166C7. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

## 2.5.370 — Clean Commander Profile and 21-second intro skip

Removed the ship-themed scanline dots over the Commander Profile photo, leaving the actual portrait artwork intact. SAVE PROFILE and RENAME COMMANDER now center using the real 8×8 font. Existing actions and saved appearance remain unchanged. Startup music begins 21 seconds in with its one-second fade; saved preferences and subsequent tracks remain intact. Preserves Comms Panel global L/R tuning, clearer focus, cinematic and other recent work.

227 focused emulator checks pass: 21 profile checks (480 exact theme/species/format photo comparisons, original identities, actual label pixels, persistence and controls), 51 Command Deck, 66 Comms Panel, two startup, 26 real-MP3 intro and 61 radio checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/COMMANDER-PROFILE-370.md and BUILD-2.5.370.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.370. SHA256: CECD509365DA4C227A67F5DA4EF5BEBDCECAF8C3F8A909A18AA1DEEC661796B2. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

## 2.5.369 — Comms Panel tuning, clear focus and 15-second intro skip

L/R tune the radio from any highlighted console button while keeping focus and flight paused. Quiet Mode remains muted; station preferences save through the existing exit path. Conversations retain their shoulder controls. Selected buttons now have a full two-pixel accent frame, bright pale corners and tinted face; footer adds proper L/R shoulder icons with RADIO. All existing controls retained. Elite Exploration startup music begins 15 seconds in with the existing one-second fade. Saved preferences and later tracks are unchanged. Preserves Mission Log, menu icons, cinematic and portraits.

206 focused emulator checks pass: 66 complete-console/global-tuning checks, 51 Command Deck checks, two startup checks, 26 real-MP3 intro checks and 61 radio checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/COMMS-PANEL-369.md and BUILD-2.5.369.txt. Highest-priority remaining work: hardware playtest, inherited reliability issues, then planetary work.

Local release: outputs/releases/2.5.369. SHA256: DA98BAC39346A7387F734A2F8E133ACE12E0495CD8A19CF49F28D1A385CF2C53. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and unrelated shared work preserved.

## 2.5.368 — Mission Log tracking and title ticker

The dossier says TRACKED MISSION only for the one actually tracked entry; other selections say MISSION DETAILS. Highlighted long titles scroll within a clipped native 150×8 pixel region, with reading pauses at both ends. Short titles remain static; unselected long names use ellipses. Selection/reopen/title changes restart the ticker. Tracking controls and progression remain unchanged. Preserves 2.5.367 menu icons, cinematic, portraits and Elite Exploration startup music.

74 focused emulator checks pass: 21 Mission Log checks, 51 Command Deck checks and two normal startup checks. Native 480×272 captures inspected. Physical PSP testing and previously documented broad failures remain open. Details: docs/MISSION-LOG-368.md and BUILD-2.5.368.txt. Highest-priority remaining work: hardware playtest, inherited gameplay/input failures, then existing planetary work.

Local release: outputs/releases/2.5.368. SHA256: D2813B23129EFE911F9885C5E4D550D1B16F39BA4306193C368BE856425A6D58. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and unrelated shared work preserved.

## 2.5.367 — Distinct menu icons

Adds native 12×12 music notes, an exit door, mission board, mission checklist, star map, open codex and TV for Radio & audio, Quit to main menu, Mission board, Mission log, Galaxy.Maps, Discovery Codex and Local TV. Icons follow ship paint and selection colours on menu buttons, descriptions and opening transitions. Existing controls, cinematic, portraits and Elite Exploration startup music remain intact.

79 focused emulator checks pass on the exact EBOOT: 51 Command Deck checks, two startup checks and 26 real-MP3/intro checks. Actual native 480×272 captures inspected. Physical PSP testing and previously documented broad failures remain open. Details: docs/MENU-ICONS-367.md and BUILD-2.5.367.txt. Highest-priority remaining work: PSP playtest, inherited gameplay/input failures, then existing planetary work.

Local release: outputs/releases/2.5.367. SHA256: B51E9AA02837D57C6A4013F624FD661AE2E5AD392D0C19A23F7228CD83C8706B. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and unrelated shared work preserved.

## 2.5.366 — Elite Exploration intro music

Elite Exploration is the default station for new preferences. Its first startup track starts ten seconds in and fades in over one second at the saved music volume. Saved station/off settings are preserved; later tracks, tuning and audio restarts play normally. Keeps the 2.5.365 cinematic and portrait work. Tutorial prompts name the updated default.

432 focused native checks pass on the exact EBOOT: 26 real-MP3/intro checks, 61 radio/settings/suspend checks, two startup checks and 343 opening checks. Physical PSP testing and full-game certification remain pending; previously documented broad failures are unresolved. Details: docs/INTRO-RADIO-366.md and BUILD-2.5.366.txt.

Local release: outputs/releases/2.5.366. SHA256: AE97CABF5CC060B50A4D1BA16FC26BAEACE73041687469FD0DFDB1BD2F5D4048. GitHub not updated; pull remains blocked by existing untracked music placeholders, autostash restored and unrelated shared work retained.

## 2.5.365 — Cinematic cloud flight and station reveal

Portrait audit included in this exact release: all 345,184 selectable v2/v3 combinations rendered at 96/32/20px; 12,162 foreground/material checks pass. Corrected neck/collar and feature masks across all ten species; expanded palettes and supported facial styles. All seven relevant headers match the frozen 365 snapshot; 94 native portrait crops match the tested C renderer. Details and broader test limitations: docs/COMMANDER-CREATION-365.md and BUILD-2.5.365.txt.


The approved continuous 480×272 intro now plays inside EBOOT.PBP: Earth approach,
fast cloud skim and outward wisps, then a sunlit station/freighter reveal with
apex lens flares, highlight bloom, coloured dust, trails and twinkling lights.
X reveals the menu over the continuing film with an 800 ms fade/release guard;
automatic menu at 36.34 seconds, logo at 27 seconds, animated vista thereafter.
Existing title actions, saves, radio settings and current portrait work retained.

505 focused native checks pass on the exact EBOOT, including tutorial, portraits,
Multiplayer, Command Deck, startup and real 44.1 kHz MP3/JPEG coexistence.
All 1,440 film frame headers validate. Physical PSP testing and broader game
certification remain open; existing core checks are incomplete, previous build
recorded 15 inherited failures. Details: docs/CINEMATIC-INTRO-365.md and BUILD-2.5.365.txt.

Local release: outputs/releases/2.5.365. SHA256: 359630D396FAD70FC1E4F5055C6A14AD948569C106DE85F852D136132DEF4571.
GitHub was not updated (DNS unavailable); preserve mixed shared work and player music.
Next: test this cinematic on PSP, then resume the existing planetary/reliability work.

## 2.5.363 — Correct hair coverage, not a forehead overlay

Replaces the coarse Human/Android hair polygon from 2.5.362 with pixel-traced scalp masks at the authored 64px resolution. Blonde and blue now follow the full curls, temples and sideburns; forehead and ear pixels remain unchanged. Ear-part sampling uses the same corrected material map. Retains strand shading, outlines, selected DNA, legacy appearances and unrelated work.

418 focused checks pass, including independent source-art hair/forehead landmark checks, all-pairs colour contrast and 20/32/96px portrait comparisons, save/load, opening, boot and Command Deck checks. Comparisons inspected and shown before packaging. Broad smoke retains the same 15 inherited game failures; radio/steering pass, input smoke remains incomplete and physical PSP testing pending. Details: docs/PORTRAIT-HAIR-MASKS-363.md and BUILD-2.5.363.txt.

Local release only; blocked Git pull restored its autostash, shared mixed work preserved. GitHub not updated. SHA256: F6CBD5DDC6C65282544942F1C04C8BF20F26E3DD4915B41C9B39D7A756BD1ECC. Resume the existing planetary work after this correction; full game certification remains open.

## 2.5.362 — Commander Creation feature isolation

Commander Creation now uses ten authored portrait rigs with explicit skin/hair material masks and local eye, mouth and ear parts. Each offered control changes its named foreground feature while retaining other selected fields. Removes unsupported face/head/background roulette, tiny unreadable choices and stray atlas overlays. Species-specific controls skip absent anatomy. New looks use DNA v2; legacy saved appearances retain the original renderer and require explicit New Look to replace them. V32 field layout unchanged; back up saves before downgrading to an older EBOOT.

416 focused checks pass on the exact executable: 35 feature/save/navigation checks, 343 opening checks, two boot checks and 36 Command Deck checks. All-pairs foreground comparisons at 96/32/20px reject background-only changes. Hair palettes also require a meaningful colour contrast. The blue stripe is replaced with a shaded cheek scar on supported rigs. Native comparison captures were inspected and shown before packaging. The legacy crop matches the shipped 2.5.361 pixels. Broader game checks retain the same 15 inherited failures; radio/steering pass, broad input smoke remains incomplete, physical PSP testing pending. Retains 2.5.361 pulse/grow menus and restored regional props. See docs/COMMANDER-CREATION-362.md and BUILD-2.5.362.txt.

Local release only; Git pull remains blocked by untracked music placeholders and mixed shared work is preserved. SHA256: B59CE0843254B98466F234B50655AC727F07CED22717A7E8725223C02673AAD0. Resume measured planetary optimization after this portrait repair; do not claim whole-game certification.

## Unreleased hardware correction — PSP server address conversion

Fixes configuration resolving the server address before PSP_NET_MODULE_INET
loads. A module-link error was being used as destination 58.1.2.128. IPv4 now
parses locally into exact network-order bytes, with malformed input rejected.
Config format, protocol and saves are unchanged. Temporary NET FIX hardware
verification build only; retain the main development version and release.
See `docs/PSP-SERVER-ADDRESS-FIX.md` and workspace `work/psp-network-fixed`.

## 2.5.361 — Pulse-and-grow menu openings; restored planet props

Command Deck choices pulse briefly and expand into the live destination over 270 ms, using the ship paint theme. No stale framebuffer replay or flash; Back and directions interrupt immediately. Launch stays immediate. Opening drawing averages 6.805 ms (worst 9.100 ms) in PPSSPP; physical PSP frame pacing remains untested.

Repairs unsigned wrapping of negative planetary prop positions, restoring trees/rocks in fern basins and ice canyons. Adds exact tagged pad-origin and biome-colour caches; retains geography v1, V32 saves, the full 2.5.360 UI/theme work, Multiplayer and existing regional generation. 424 focused checks pass on this exact EBOOT, including normal boot, native menu captures, region/collision/save tests and the complete authored opening. Same 15 inherited core failures remain; radio/steering pass and broad input smoke is incomplete. Surface FPS still needs tuning. See BUILD-2.5.361.txt and docs/MENU-TRANSITIONS-361.md.

Next: deliver this menu EBOOT, then resume measured regional surface optimization and visual progress using the asset coverage in docs/PLANET-REGIONS-V1.md. Do not claim specialist planetary art or hardware certification complete. Local release only; blocked Git pull and mixed shared changes prevent safe publication. SHA256: 13DFB7F1F38E70BBF23B31974C236FADD917E03870EC0D5CB55E950649169C8F.

## 2.5.360 — Responsive menus and matching ship themes

Menu selections now reveal their destination immediately without the full-screen replay, bright flash or CRT scan bands. Category slides and ambient animation remain. Native art avoids resampling, scaled art shares X samples, sun shimmer avoids per-pixel sine, and paint-theme lookup is cached. Command Deck drawing averages 8.873 ms in PPSSPP versus 12.112 ms before; hardware FPS remains to be tested.

Command Deck, Multiplayer, Cargo, Ship Loadout/Item Storage, Shipyards, Commander cards, Controls chrome, decorator selection and GalacticNet navigation follow the current hull's PIMP-MY-SHIP finish. All sixteen finishes are verified across fourteen main screens. 171 focused checks pass on the exact release, including normal boot and all changed console functions. Retains the 2.5.359 planetary foundation and 2.5.358/357 features; V32 saves remain compatible. Evidence: BUILD-2.5.360.txt; implementation: docs/MENU-RESPONSIVENESS-360.md. Release: outputs/releases/2.5.360. SHA256: D3504C35AF1CE7FBF473D2D0BFFDC640D91A1ADC4FCF587528BC06A0F2A37555.

Highest-priority continuation: player PSP boot/menu/paint test, Lave region/FPS feedback, inherited core failures, sparse biome props and physical suspend/audio/two-device checks. No new geography or art expansion before feedback. Local release only; GitHub is not updated. Preserve mixed shared work and player music.

## 2.5.359 — Planet regions v1 and boot fix

Retains 2.5.358 Command Deck and 2.5.357 Multiplayer. All 1,380 existing world destinations now receive deterministic compatible regional terrain, palettes, water and reused native props: 18 ground types, four gas families, 3,898 selected regions. Lave has close comparison regions; authored Earth remains intact. Safe resumable surface positions are preserved; unsafe locations recover at the same world's pad. V32 saves and destination IDs remain unchanged.

The combined executable could not allocate its former 8 MiB heap in PSP-1000 memory. A bounded 7 MiB heap restores normal startup in PPSSPP. Exact artifact: outputs/bootfix359/EBOOT.PBP; release: outputs/releases/2.5.359. 397 focused checks pass; the same 15 inherited core failures remain. Physical PSP startup and performance require playtesting. This is a foundation with adapted assets, not completed bespoke art for every type. See docs/PLANET-REGIONS-V1.md and BUILD-2.5.359.txt. Local release only; GitHub has not been updated.

## 2.5.358 — Observation Lounge Command Deck

Built on local 2.5.357, preserving its Multiplayer console and prior native UI updates. Implements approved 480x272 6D layout: original logo, green docking status with animated ship icon, glass service buttons, distinct teal category focus and amber service focus, native readable paintbrush/spanner icons, bounded description card and fixed footer funds. Left/Right and L/R shoulders slide between remembered categories; service openings flash briefly, slide and reveal the destination within 340ms. A new input interrupts cosmetic transitions without delaying Back. Station art stays still with twinkling port lights; star glints, nebula glow and restrained native sun shimmer animate. Opening highlights now follow the actual new row geometry. Real availability, actions, networking and V32 saves remain authoritative.

Implementation/coordinates: docs/COMMAND-DECK-358.md. Final validation, captures and EBOOT hash: BUILD-2.5.358.txt. Highest-priority remaining work: inherited broad failures and physical PSP input, sleep and two-device tests. This update is not whole-game certification. GitHub DNS is unavailable; preserve the mixed shared checkout and do not publish unrelated work wholesale.

## 2.5.357 — Multiplayer Retro Terminal

Implements the approved native 480×272 Multiplayer console on local 2.5.356. One Fly entry and title Square open online connection and local Host/Join together. CONNECTED keeps a plain red disconnect X; real PSP action icons stay in the footer. Server connection stages are truthful, peer names and locations are bounded, and local flight codes come from real advertisements. Predictable four-region D-pad navigation, explicit cancellation, No-default transport/end confirmations and caller-specific returns are implemented. Online/local exclusivity, settings retry and sleep cleanup use the existing transports; protocol and two-pilot server capacity remain unchanged.

See docs/MULTIPLAYER-357.md and BUILD-2.5.357.txt for final verification, native captures, artifact hash and limitations. Preserve unrelated shared work and player music. Existing broader game and planetary Wi-Fi failures remain baseline issues; physical PSP/two-device testing is pending. Local build only; no GitHub publication.

Verified local EBOOT: `68ED085740E86B135A29E8D25833AB6EC73B8BD0EC9B8831C95BAA065A7C5564`. Final Multiplayer console (58), online fixtures (12), real server connection (7) and unchanged server (13) checks pass. All 30 native states inspected; 93 recent UI checks pass and 120 prior captures remain byte-identical. Core retains the same 15 failures; local planetary Wi-Fi retains the same three. Highest-priority remaining work: those inherited failures and physical PSP/two-device/sleep testing. Local release only; GitHub is not updated. Full evidence: BUILD-2.5.357.txt.


## 2.5.356 — Orbit Motors and Crown Grand shipyards

Implements the approved native regular and premium Mega shipyards on top of local 2.5.355. Orbit Motors has a bounded seven-row catalogue and green workshop. Crown Grand has a wide orbital showroom, unique dealer names and site artwork, real animated hulls and a three-row catalogue. Square toggles complete descriptions and comparison stats in the same reading panel. Gold stat labels, teal values and green-up/red-down arrows compare real bare-hull values; equal stats have no arrow. The compact exchange box reads EXCHANGE YOUR SHIP above COSTS: or YOU RECEIVE:, with fixed bottom-right funds.

Every actual Mega dealer has a distinct deterministic four-hull collection; Lave retains all 22 ships for testing. Existing exchange validations, cargo/module transfer, congratulations cards, Controls handbook, Cargo and separate Loadout/Storage remain. No V32 save change. Native bounds, all descriptions and all 484 exchange strings have automated coverage. Final verification and EBOOT hash: BUILD-2.5.356.txt. Physical PSP testing and inherited broader failures remain open; this is a local build, not whole-game certification. Preserve mixed shared changes; no GitHub publication.

## 2.5.355 — Illustrated Controls handbook

Implements approved concept 3, the Pilot's Field Guide, directly at 480×272 with the real 8×8 game font. Five topics list forty controls on the left; selected native diagrams animate on the right with short instructions. Boost reads “Double press R + keep it held.” Up/Down chooses a control; L/R shoulders (or Left/Right) change topic; Square toggles analog steering; Circle returns to the calling menu. Gameplay controls and V32 saves remain unchanged. Existing Cargo, Circuit Console Loadout and separate Item Storage are retained.

Current local EBOOT and dist copy are updated at the user's request. Focused review: 18 checks pass and all forty native entries are captured. See docs/CONTROLS-HANDBOOK.md and BUILD-2.5.355.txt for final hash, core comparison and limitations. Physical PSP testing remains pending; this does not certify the full game. No GitHub publication; preserve the mixed shared checkout.

## 2.5.354 — Circuit Console loadout and Item Storage

Implements the approved first native concept on top of 2.5.353. Ship Loadout now has six coloured cartridge rows, all eight usable bank positions, active-weapon lamps, a painted real-hull miniature and prominent Item Storage access. The separate matching storage screen shows real spare quantities and only the highlighted module's bank category, with a clean selected-module strip and no redundant comparison panel. Full names and effects fit, including two-line effects. D-pad navigation remains; L/R shoulders now switch loadout categories. Existing docking, cargo/cabin protection, swapping, resale and V32 saves remain authoritative. The 2.5.353 cargo console is retained.

Validation and candidate hash: BUILD-2.5.354.txt. Physical PSP certification and inherited broader failures remain open. Local candidate only; preserve mixed shared changes.

Verified local build: `B37BBBCAD6173297B85E19F7B697EC4DB666068993FF64752704C57B7F4EF74A`. 24 loadout/storage and 17 cargo checks pass; cargo and GalacticNet Market captures are byte-identical to 2.5.353. Core retains the same 15 failures. Full evidence: BUILD-2.5.354.txt. No GitHub publication.

## 2.5.353 — Native Cargo and exchange console

Cargo now follows the supplied 480×272 designs: selected item art and full description above a full-width commodity table, with a miniature cargo bay, share-of-hold bars and reserved quantities in flight. While docked, the same layout shows real shop stock and local prices. Carried goods unsupported by the current port remain visible as NOT TRADED; both buy and sell are blocked for those rows. Actual quantities, units, cargo capacity, money and mission reservations remain authoritative. Native small icons cover all commodities; credits and capacity have fixed bottom positions. Existing Left/Right trading, Up/Down selection and Circle return remain.

Save format remains V32. See docs/CARGO-CONSOLE-353.md for coordinates and implementation, and BUILD-2.5.353.txt for final evidence. This local candidate retains known inherited broad-check failures; physical PSP certification remains pending. Preserve the mixed shared checkout and do not publish it wholesale.

Verified local candidate: EBOOT SHA256 28DE99FF5EC3D4D02EC5F075976E28E5CEC2F0CDFF551FCEF91D17FA814D14EA. 17 cargo checks pass; all 17 descriptions fit; native references and edge-case captures inspected. Opening, newspapers, RUMOURS and Spacebook pass. GalacticNet Market is byte-identical in all five comparison states. Core retains the same 15 inherited failures; no new failures. Full evidence: BUILD-2.5.353.txt. No GitHub publication.

## 2.5.352 — Newspaper puzzle input and 1,000-unit prizes

X now opens and enlarges the Lave newspaper puzzle through the actual opening-story input dispatcher. Sol/Earth onboarding retains its existing exit progression. Completing either puzzle type opens an animated congratulations card and pays 1,000 units once per system/puzzle kind. Normal commander saves persist prize claims with credits in CRC-protected V32 data; V31 imports without invented claims. Existing answer files and RUMOURS remain intact.

See docs/PUZZLE-REWARDS-352.md for controls, save compatibility and test scope. This remains a local PSP candidate with inherited broad-check failures and physical PSP/editorial certification pending. Do not publish the mixed shared checkout wholesale. Final verification and download hash: BUILD-2.5.352.txt.

Verified local candidate: EBOOT SHA256 314B7465755AC3B0B671F2705C1D4AD34A2BC3C6D7245348D2D1D844531F29B3. 50 newspaper/puzzle, 37 RUMOURS, 343 opening and 12 Spacebook checks pass. Core retains exactly the same 15 inherited failures; radio/steering/NPC combat pass. Native puzzle/card screenshots inspected. No GitHub publication. Evidence: BUILD-2.5.352.txt.

## 2.5.351 - Space Secrets local rumours (PSP test candidate)

GalacticNet JOBS becomes RUMOURS: a purple, scrolling Space Secrets website with illustrated classifieds, travel reviews, funny adverts and signal diaries. Offers derive from real local discoveries, anomalies and whale pods. SpacePay confirms the exact cost, animates one payment and offers optional TRACK SEARCH AREA. Search regions work through the targeting computer without increasing sensor range, revealing contact identity or replacing the tracked mission. Ownership survives normal commander saves; leaving the system clears active directions, not purchases. Invalid paid sightings refund once. Naturally identified discoveries provide free directions.

Native 480x272 layouts use the game's real 8x8 font, clipped scrolling content, fixed chrome, distinct card arrangements and native illustrations. Subtle lights, whale wakes, a blinking eye, signal waveforms and a short receipt animation add motion without moving text. Whale pods now remain in a persistent local region so their directions remain honest. Save format V31 adds checksummed rumour state and imports V30 with no invented purchases.

This expands the local 2.5.350 candidate and preserves its newspaper implementation/content. See docs/RUMOURS-351.md for controls, source ownership, save schema and limitations. Final test evidence and EBOOT hash are recorded in BUILD-2.5.351.txt. This is not a completed-game certification or GitHub release: the 15 inherited broad game-check failures and the newspaper editorial/hardware gates remain open. The shared checkout contains unrelated unfinished work; do not commit or publish it wholesale.


Final verification (2026-10-09): 37 RUMOURS checks pass; all newspaper pages/puzzles and 12 Spacebook checks pass. Five Market captures differ only inside the exact renamed-tab glyph rectangle. The complete core report has 663 passes and the same 15 baseline failures; radio, steering and NPC combat have zero failures. Native captures inspected. EBOOT SHA256: 7D81C4E9F5E2443112E989E47798A6865753413F4ED3359F65E14D52BC77B893. Local packaged build: outputs/releases/2.5.351/ELITE-NEXT-PSP/EBOOT.PBP. No GitHub publication. Full evidence: BUILD-2.5.351.txt and outputs/rumours-test-evidence-351.json.

## 2.5.350 - Local newspapers and playable paper puzzles (candidate)

NEWS now loads one fixed edition per canonical galaxy system, plus the separately approved four-page Earth tutorial paper. The 256 galaxy editions contain 3,082 pages with lengths of 4-24; Lave has 16. Sixteen native arrangements, sixteen publication families, 24 readable palettes, distinct masthead registers, bounded captions, native illustrations, paper turns and Sudoku/crossword input are implemented. Puzzle progress saves separately in small checksummed files. Other GalacticNet sections retain their existing code and controls.

Lave has an original complete issue following the Night Shift Circle's lamp dedication, with reports, letters, adverts, corrections and a credited Nip and Tug comic. Seven other major systems have original front-page material. The rest is a fixed bank-derived DRAFT corpus: the full 256-paper bespoke writing/art handoff is NOT complete. The audit explicitly counts repeated printed paragraphs; unique metadata is not editorial approval. Every final page still requires an individual recorded native inspection.

Verification: final EBOOT newspaper checks RESULT 0 failures, including all 3,086 native pages and puzzle persistence/navigation. Market isolation is byte-identical in five states; Spacebook/Inbox and Wanted checks pass. Fifty-three actual paper pages and two enlarged puzzle views inspected; 3,033 individual paper inspections remain. Broad game failures exactly match the 15 pre-change failures; radio and steering pass. Physical PSP certification and full bespoke editorial/art acceptance remain open.

Final local EBOOT SHA256: 51DF45568F12828DC36097118DA26534896EF497659A836A3491BAB4FBCAF504.
Evidence: outputs/newspapers-20261009-224919-284; content/newspapers/test-evidence-350.json and native-review-350.json.

See docs/NEWSPAPERS-350.md for source ownership, controls, test evidence and remaining gates. Do not publish this candidate as the completed newspaper expansion. Physical PSP certification and the inherited broad game failures remain open. No GitHub publication.

## 2.5.349 - Spacebook homepage and private story archive

Spacebook has a blue masthead, profile/navigation rail, readable activity card, evidence strip and local-pilot sidebar designed at 480x272. Its feed now exclusively uses existing NPC reactions to commander activity, including during the opening and after it. Scott, Kat and the first-ship posts are remade as private Inbox correspondence; Scott's photo remains attached to his message. Inbox retains Earth family/friend conversations, the flight note, reached Homeward milestones, unlocked First Light briefings and current/completed Open Channel chapters. Future chapters stay hidden. Long letters page with Square, story letters open Mission Log with X, and Triangle switches between feed and Inbox. Dad's existing undelivered reply flag and saved NPC post reactions are retained. Archive records derive from existing saved progress; no save format change.

Verification: PSP build succeeds. Spacebook home review RESULT 0 failures across 12 checks: one NPC-only feed in every opening phase, existing reactions, migrated private letters/photo, offline Dad reply, chapter spoiler gating, page clamping, Mission Log navigation and save/load. Opening integration reports RESULT 0 failures for all three starter ships through guided free-play progression. Seven native 480x272 feed, Inbox, attachment, final-archive and long-name captures inspected. Inherited broader regression failures remain open; physical PSP certification is not claimed.

## 2.5.348 - Informational wanted posters

Removed the GalacticNet Wanted poster X action and both tracking prompts (on the poster and in the footer). Open warrants now consistently say LIVE WARRANT. Browsing posters no longer launches a docked commander or alters a flight target. Names, portraits, bounty rewards, risk and taken-down status remain available; identify suspects through normal local targeting and hailing.

Verification: PSP build succeeds. Wanted-poster review RESULT 0 failures: X leaves all five posters docked without changing targets or starting departure; in flight the existing target remains unchanged. Six native 480x272 captures produced; first sheet, second sheet and claimed status inspected. No tracking prompt remains. Inherited broader regression failures remain open; this focused change is not full-game certification.

## 2.5.347 - Raiders react to combat

Direct player hits now interrupt pirate patrols, including wanted-poster raiders in peaceful Lave. An engaged raider prioritises its attacker within 12 km rather than switching to a closer trader. Healthy raiders turn back and return fire, weave on approach and complete close attack passes before circling back. Badly damaged raiders or those facing two nearby patrols retreat from their actual threats at escape speed. A lone nearby patrol no longer constantly cancels retaliation. Untouched Lave pirates remain peaceful toward the player. Existing bounty claims and police-assist credit remain intact. Civilian defensive aim no longer gets replaced by an unrelated pirate-avoidance waypoint. No save or network packet layout change.

Verification: PSP build succeeds. All 14 focused NPC combat checks pass through real laser-hit and game-tick paths. Core comparison has the identical 15 inherited game failures, no new failures; radio and steering report RESULT 0 failures. Physical PSP combat feel remains to be playtested; full-game certification is not claimed.

## 2.5.346 - GalacticNet market neon sign

Removed the pink Left Sell / Right Buy sentence from the GalacticNet Market page. LIVE TERMINAL is now a slightly larger native pixel neon sign with a soft pink glow, slow brightness drift and brief character/scanline glitches. It occupies its own strip below the ticker rows and above the footer. Undocked access retains Dock for live trading; prices and trade controls are unchanged.

Verification: PSP build succeeds. Five native 480x272 captures cover the normal sign, three glitch phases and undocked access; normal, scanline-glitch and offline frames inspected. Ticker rows, sign and footer remain separate. Broader inherited failures remain open; physical PSP performance and full-game certification are not claimed.

## 2.5.345 - A personal shipyard congratulations card

Successful shipyard purchases now open a warm delivery card bearing the new hull name, native-size animated handwriting, restrained pixel fireworks and a shipyard seal. Six messages vary without immediate repeats, including "Enjoy your new ride!". X finishes the handwriting, then continues; Circle closes the card. Purchase input is guarded and the card blocks underlying shipyard actions, so it cannot buy twice. Failed or already-owned exchanges show no celebration. Credits, cargo, fitted modules and Item Storage retain the existing exchange rules; no save change.

Verification: PSP build succeeds. Ship-purchase review RESULT 0 failures across real menu transactions, exact credits/cargo/modules, repeated input, failed purchases, all six notes, animated pen stages, footer ownership and exit cleanup. Native 480x272 arriving, writing and completed captures inspected. Guild navigation/save-load regression also RESULT 0 failures. Broader inherited failures remain open; physical PSP performance and full-game certification are not claimed.

## 2.5.344 - Local Guild contract guidance

Tamsin's food-delivery and the Guild rescue assignments now use the real five-offer board catalogue. While each assignment is active, a missing required contract replaces only the final board offer, preserving five distinct choices. Already docked in the right system, Next Step opens and highlights the local contract; in local flight it guides docking, and an accepted contract leads to its mission-log entry. Existing Guild progress needs no save migration.

Verification: PSP build succeeds. Guild navigation review RESULT 0 failures: 1,536 system/hub/assignment board combinations retain the promised family and five distinct choices; all Lave hub food/rescue actions open the right board; confirmation reserves real Food; accepted work selects its existing log entry; local flight avoids Galaxy.Maps; save/load preserves the offer. Native 480x272 briefing and board captures inspected and byte-identical in the final EBOOT. Broader inherited regression failures remain open; this is not full-game certification.

## 2.5.343 - Readable mission acceptance

Mission acceptance shows complete mission names wrapped and centred inside the confirmation card. YES/NO labels and the instructions below them are centred; destination text has its own clear row. Mission acceptance actions are unchanged.

Verification: PSP build succeeds; native 480x272 outfitting screenshot inspected. Cost and Balance are adjacent, with teal Cost and gold Balance. Existing mega-yard review retains its identical eight-bank drawing assertion failure in both 2.5.341 and 2.5.342; other three checks pass. Broader inherited regression failures remain unresolved. No GitHub publication.

Verification: PSP build succeeds; mission-board review reports RESULT 0 failures. Native 480x272 Wildlife Survey and Multi-stop Logistics confirmation captures inspected: full names fit, and button labels/instructions are centred. Broader inherited regression failures remain unresolved; no full-game certification or GitHub publication.

## 2.5.342 - Cleaner outfitting prices

Removed the delivery, fit-later and loadout disclaimer lines from the module card. Cost now sits directly above Balance; Cost is teal (red when unaffordable), while Balance remains gold. Purchase and storage behaviour is unchanged.

## 2.5.341 - Compact planetary spaceports and EVA rendering

Planetary spaceports now use an 840m-wide apron instead of roughly 1.4km. Services, low terraces, the terminal and rover garage are grouped closer together, with fewer apron lamps and shorter roads. The shared geometry drives rendering, collision, landing reservations and the local map. The player pad retains clearance for the widest and longest hulls; two staggered traffic berths remain.

EVA retains the exact existing terrain mesh and 2x2 depth raster. Ground grain beyond 24m shares shading on a 4x4 screen grid; nearby props, signs, portraits and HUD retain their native detail. Distant dry ground avoids unnecessary texture reconstruction. Lave vegetation reuses tagged cells when moving between overlapping windows rather than regenerating the entire region. Quad submission transforms four vertices rather than six.

Verification and timing evidence are recorded in docs/EVA-PERFORMANCE-341.md. Physical PSP frame rate remains to be measured. This is a local private candidate; it is not a GitHub release. Existing broader regression failures remain open.

## 2.5.340 - Rear chase camera and aligned exterior effects

- Third-person view is centered behind and above the current hull, with distance scaled to its actual geometry.
- Chase camera, player hull, exhaust and weapon anchors share the same pitch/yaw/roll basis. Full rolls no longer move the camera sideways or reverse its bank at the angle wrap.
- Engine and gun mounts intersect actual hull faces; exhaust leaves the stern and weapons leave the forward surface.
- Exterior flame and gunfire use perspective-correct depth tests against the hull and depth-rendered world geometry. Exterior weapons never fall back to cockpit effects when offscreen.
- Native exterior review covers every hull across full rolls at three pitches, surface anchor availability, foreground occlusion and native captures.

Verification: PSP build succeeds. Final exterior review reports RESULT 0 failures across 8,514 hull/pitch/roll combinations, both engine/weapon anchors and foreground depth rejection. Native 480x272 level, quarter-roll, half-roll, full-roll and capital-hull captures inspected. The performance review reports RESULT 0 failures; crowded third-person render plus simulation averages 32.850 ms on the emulator. Real PSP performance and subjective handling remain to be tested. Existing broader regression failures remain recorded in the 2.5.339 handoff; complete game certification remains open. Preserve shared uncommitted changes; this local build is not a GitHub release.

Evidence: outputs/exterior-final-20261009-174150/exterior-review.txt and outputs/performance-review-20261009-174152-575/performance-review.txt. Run tools/review-exterior.ps1 for repeatable focused checks. The chase view uses the player's complete orientation without Euler-angle smoothing; cinematic continues to ease between its own shots. Capital-hull parts reuse the menu geometry, transformed into world space; ordinary hulls use their mesh faces. Exterior shots are depth-tested world-space beams/packets, while first-person weapon effects remain unchanged.

Broader regression (work/smoke-20261009-174239-184): steering and radio RESULT 0 failures. Game checks report 15 failures, with failure lines identical to the 2.5.339 baseline. Input checks do not finish before the existing 120-second timeout; do not claim a complete regression pass. Packaged EBOOT SHA256: FEDDA17E6875093ED29109099842EB6DCB0ED2E6E0E04CD08BD7CB3E2DA3BCA3.

## 2.5.338 - Clear empty-bank guidance

- Changed the Ship Loadout empty-bank instruction to “FIT AN ITEM FROM STORAGE >>”.

## 2.5.337 - Exterior camera stability

- Fixed the nebula using cockpit orientation while geometry and stars used the third-person or cinematic camera.
- Added a shared camera-axis API so every background layer follows the active view consistently.
- Made chase smoothing time-based, preventing different behaviour at fluctuating frame rates.
- Exterior cameras now derive their look direction from the smoothed camera position and current ship focus, keeping the ship framed during loops, rolls and camera-mode changes.
- Cinematic mode begins on its first authored shot instead of briefly jumping from the final shot.
- Speed streaks originate around the ship's projected exterior position instead of the cockpit centre.

## 2.5.336 - Two-step Item Storage fitting

- Item Storage first selects a stored module with X, then moves focus to that module category's real banks.
- Up/Down selects the bank; X fits; Circle returns to the item list without leaving the screen.
- Removed Left/Right bank controls and the Item Storage Triangle shortcut so the footer remains complete at 480x272.
- Removed the Triangle-to-Loadout shortcut from Outfitting.
- Renamed Outfitting's “Sell Stored” action to “Sell Menu.”
- Removed the duplicated Outfitting control instructions above the footer.

## 2.5.335 - Outfitting storage shop flow

- X in Outfitting buys the selected module directly into Item Storage; service items such as fuel and missile restocks still apply immediately.
- Square opens a dedicated stored-module resale screen showing quantities and the station's purchase price.
- Resale uses an X confirmation and is available only through Outfitting.
- Removed selling controls from Ship Loadout and the normal Item Storage fitting screen.
- Item Storage now draws only the compatible banks actually open on the current ship instead of showing locked placeholders up to bank eight.

## 2.5.334 - Store fitted modules with Square

- Square on Ship Loadout moves the highlighted fitted module into Item Storage.
- The action requires docking and refuses empty banks, full storage, occupied passenger cabins and hold removals that would strand cargo.
- Active weapon state and derived ship statistics rebuild safely after the transfer.
- Triangle continues to open the separate Item Storage screen; X retains its confirmed sell action.

## 2.5.333 - Eight-bank Ship Loadout restored

- Restored the full-width row-of-eight layout for every fitted module category.
- Moved selected-bank details below the board so banks five through eight never collide with another panel.
- Retained four-direction bank navigation, fitted-module sale confirmation, the large Item Storage action card and the separate Item Storage screen.
- Kept the Shipyard Square description/statistics toggle introduced in 2.5.332.

## 2.5.332 - Shipyard description and stats tabs

- Square switches the Shipyard's lower panel between the selected ship's description and statistics.
- Added authored descriptions for every standard and mega hull.
- The statistics view compares speed, acceleration, braking, hull, shields, heat, range, cargo and Item Storage against the current ship.
- Switching views preserves the selected hull and list position.

## 2.5.331 - Prominent Item Storage control

- Replaced the small Ship Loadout footer hint with a large dedicated Triangle / Item Storage button in the right module panel.
- The button shows the current hull-specific used and maximum item count plus a live capacity bar.
- Full storage switches to a red warning treatment while preserving the same Triangle shortcut.
- Kept the fitted-bank board, module descriptions and bottom controls clear of the new card at native 480×272.
## 2.5.330 - Hull-specific Item Storage

- Renamed Storage to Item Storage throughout the ship interface.
- Item Storage capacity now follows the current hull's total module-bank space: compact ships start at 12 items, larger multi-role hulls carry more, and mega ships reach the 40-item ceiling.
- The Item Storage counter shows the current ship-specific total.
- Changing to a smaller hull never deletes stored modules; an over-capacity store remains intact but cannot accept more items until space is freed.
- Fitted modules remain exclusive to Ship Loadout and appear in Item Storage only when an additional spare copy exists.
## 2.5.329 - Ship Loadout and Storage

- Restored the established Ship Loadout screen for fitted banks, installed-module details, weapon arming and fitted-module sales.
- Added Storage as a separate screen opened with Triangle from Ship Loadout.
- Storage lists only unfitted spare modules; installed equipment is excluded unless another copy is genuinely stored.
- Storage keeps the contextual compatible-bank preview and supports fit/swap, confirmed spare sales and the persistent 40-module capacity.
- Outfitting Square buys spare modules into Storage, including additional copies of an already fitted type.
- Save format V30 persists spare Storage contents and older commander files migrate with empty Storage.
- PSP build completed successfully.
## 2.5.328 - Exterior camera combat polish

- Moved third person to a substantially closer, low rear-quarter chase composition so the current hull remains the visual focus.
- Exterior laser effects now begin at two hardpoints derived from the current ship's actual mesh width and nose, then converge on the selected target in world space.
- Added twin engine flame trails to exterior ships. They lengthen, brighten and gain heat shimmer during boost.
- Made the chase camera inherit all ship translation immediately, preventing high-speed boost from leaving the ship ahead of its camera.
- Removed camera-position parallax from infinitely distant background stars while preserving nearby dust motion.
- Restored a complete freshly rendered HUD in third person, including radio dialogue and the held-Square targeting computer, eliminating stale alternating-framebuffer fragments.

Verification: PSP build succeeds. Focused PPSSPP checks pass close framing, ten-second cinematic shots, boost tracking, mesh-derived weapon hardpoints and camera persistence. Native 480x272 targeting and radio-overlap captures were inspected.

## 2.5.327 - Exterior and cinematic flight cameras

- Replaced the former decorative third-person model with a genuine exterior camera that follows the real player ship while flight physics, steering, targeting and collisions remain ship-centred.
- Expanded the Comms Panel camera selector to First, Third and Cinematic, with a clear three-position state display and backward-compatible settings persistence.
- Added twelve authored Cinematic compositions. The camera changes shot every ten seconds and eases between rear chase, front-quarter, side, overhead, low, engine, distant hero and flyby views.
- Exterior modes render the current hull and Pimp My Ship paint, with readable engine glow and without the cockpit frame or cockpit-only exhaust obscuring the view.
- Corrected solid-hull backface selection to use the active render camera, keeping player ships and nearby capital geometry visible from exterior angles.

Verification: PSP build succeeds. Focused PPSSPP review passes real-ship framing, distinct ten-second cinematic shots and cinematic settings persistence; four native 480x272 exterior captures were inspected.

## 2.5.326 - Animated Discovery system dossiers

- Replaced the sparse Discovery galaxy locator with an adaptive system dossier that shows every generated sun and planet on slowly animated elliptical orbits, plus the system hub.
- Added a real System Record percentage derived from landings, field records, logged space signals and identified bounty targets, with a concise Next action leading toward completion.
- Added ten visible milestone markers. Triangle claims available rewards, pays once, and persists the claimed tier in the existing commander save data.
- Added a dramatic reward card that accelerates in from the right, settles at screen centre, bursts deterministic sparkles and reveals its message and reward text in stages.
- Preserved the 2.5.324 System Directory structure, undiscovered-world gating, quick facts and station sprites.

Verification: PSP build succeeds. Focused native 480x272 review passes dossier progress, one-time 20% payout, save/load persistence, X dismissal, slide, sparkle and completed-card captures. The broad source regression currently stops later on three inherited equipment-damage assertions outside this change.
## 2.5.325 - Lush seamless planet art

- Replaced flat polar colour bands with irregular seeded ice shelves that blend into each world's terrain.
- Increased surface colour separation and brightness so oceans, continents, minerals and gas bands remain vivid at PSP display sizes.
- Preserved deterministic world seeds, so each planet keeps the same identity across flight, maps and the Discovery Codex.

## 2.5.324 - Honest system Discovery directories

- Added a divided Planets section beneath Space Signals in every system directory.
- Lists all charted planets while marking unlanded worlds Undiscovered.
- Hides Flora, Fauna, Minerals and Logged Sites until the first landing.
- Replaced generic directory guidance with selection-specific station, star, signal and planet facts.
- Added those facts beneath full record descriptions.
- Replaced the station plus badge with authored Poor, Rich or Mega Capital station artwork.

## 2.5.323 - Docking-ready Comms control

- Added a restrained green pulse to Request Docking while the player is inside a usable station's communication range.
- Reused the actual docking availability checks, including orbit, death, hyperspace, active docking and police-stop restrictions.
- Kept orange for cursor focus and the dimmed No Signal treatment when docking cannot be requested.

## 2.5.322 - Safe quit confirmation

- Added an Are you sure? panel before returning to the title screen.
- Shows the active checkpoint's relative save age, including just now, minutes, hours or days.
- Defaults to No and supports D-pad selection, X confirmation and Circle cancellation.

## 2.5.321 - Working character creator

- Made every structural character option select clean, distinct authored portrait art.
- Removed generic eye, mouth and ear stamps that caused misplaced pixels on differently shaped faces.
- Reworked hair and crest tinting into coherent regions instead of scattered checkerboard pixels.
- Preserved old save portrait compatibility and applied the repaired renderer to generated NPCs.

## 2.5.320 - Human mission guidance

- Rewrote contract briefings as natural dialogue tailored to each mission family, including memorial deliveries.
- Gave Current Objective a distinct navigation colour and a visual arrow into Navigate to objective.
- Changed Return to mission log to Go to mission log and Mission Log's next-step shortcut from Select to Square.
- Suppressed the redundant temporary acceptance notification after board confirmation.

## 2.5.319 - Readable cargo terminal

- Moved the commodity page counter below the title so HOLD aligns with held quantities.
- Reworded ownership, local price, galactic average and shop stock labels in plain English.
- Moved hold usage beside its capacity bar and added the standard bottom-right units box.

## 2.5.318 - Faster Command Deck navigation

- Added L/R shoulder-button category navigation across the Command Deck.
- Removed the redundant menu button-confirmation animation and footer button echo.
- Added navigation regression coverage for both shoulder directions.

## 2.5.317 - Mega livery integration

- Extended PIMP-MY-SHIP.NET finishes and persistence across all 22 hulls.
- Migrated existing 16-hull paint files without losing fitted colours.
- Mega hull paint now drives the cockpit/UI theme, decorator preview, Loadout portrait, shipyard portrait and exterior colour.
- Reworked Mega portrait geometry with tapered armoured modules for sleeker spacecraft silhouettes.

## 2.5.316 - Complete Lave outfitting and Loadout controls

Corrected Ship Loadout navigation so L/R changes equipment category and the D-pad selects banks. Lave now stocks the complete module catalogue, all equipment rows use the shared native icon language, and duplicated stat/detail copy is removed.

## 2.5.315 - Visible console and flight animation polish

Strengthened the previously subtle interface feedback with a CRT page-opening sweep, expanding button confirmation, animated selection cursor and live flight-instrument drive sweep. Boost now has a clearly visible amber vibration response while leaving the canopy unobstructed and controls immediate.

## 2.5.314 - Mega Shipyard and technology expansion

Implemented the complete twelve-hull Mega catalogue, deterministic regional sellers with Lave test stock, 48 fitted slots with V29 migration, all 43 specified technologies and their shared runtime stat policies. Shipyard and Loadout now present true hull comparisons, eight banks and native-size module icons. Rare named Mega vessels enter normal traffic, leave 15-minute class-tagged wrecks, and deploy a protected shuttle for planetary flight and EVA.

## 2.5.302 - Clean station arrivals

The shared primary-station docking cinematic now frames its camera from the generated station radius and depth rather than using one fixed close-up. Rich Orbital Citadels, compact ports and mega capitals retain more hull context at PSP resolution. Station geometry is flushed before the exterior ship pass, and the ship model is retired as it crosses the pressure threshold so triangles cannot leak through the dark entrance slit.

Removed the footer-wide `HULL DAMAGE / ENGINEERS REQUIRED` warning. Damage remains visible through the hull meter, smoke and repair services. Removed the flashing red/orange side chevron because it resembled a directional enemy indicator.
## 2.5.301 - Distinct stellar-system archetypes

The fixed five-body template is replaced by twelve deterministic structural families across the 256-system galaxy. Systems can now contain a solitary star, a gas giant with eight moons, three gas giants, asteroid-heavy planetoid fields, binary or trinary stars, frozen red-dwarf worlds, scorched dead rocks, crowded habitable worlds, rogue planets without a conventional star, gas nurseries, and the classic mixed arrangement retained by Lave.

The runtime supports up to ten celestial slots while inactive slots remain absent from rendering, collision, sensors, traffic, freight and discovery placement. System Details, the Codex and operations guidance derive their counts from the actual local layout. Secondary stars are treated as stars rather than landing targets. Rogue-system slot zero is an unlandable navigation giant, preserving the established landing/save bit layout while the remaining rogue worlds are explorable.

Mission Validation now routes exploration offers only to reachable systems containing a real landable world and selects the objective from that generated layout. All five mission types remain available in every system. Automated coverage checks all twelve archetypes, active/inactive slots, defining star and world counts, and all 1,280 generated mission offers.

Save format V28 widens landing masks and stores ten surface-progress slots per system while migrating V1-V27 saves from their original five-body records. Local Wi-Fi protocol 263 carries the expanded world layout and rejects malformed inactive slots.
## 2.5.300 - Staged route hyperdrive charge

- Makes the existing blue route-alignment bar the complete jump spool: the dedicated Hyperdrive Charging panel appears when it reaches 50% and maps the remaining half from 0 to 100%.
- Moves the charging panel lower in the canopy and adds a compact percentage bar that remains readable at native 480x272.
- Ramps coloured radial warp streaks into the view throughout the second half of the charge.
- Enters the hyperspace corridor as soon as both bars reach full, removing the previous extra three-second charging wait.
- Adds timing-boundary regression checks and a four-stage native-resolution visual review.

Verification: PSP build completes; route timing checks pass at 49%, 50%, 75% and full charge. Full gameplay checks pass; the broader input suite currently contains pre-existing failures outside this change and remains a separate cleanup target.
## 2.5.299 - Quiet Watch replies work

- Fixes the optional lonely-trader Quiet Watch call beginning 'Anyone awake on this frequency?' advertising Triangle Reply but immediately discarding its Talk/Ignore chooser.
- Triangle tap now keeps and visibly draws the shared Talk/Ignore choice; Respond opens the full paused Quiet Watch conversation and Ignore closes it normally.
- Combat, incoming missiles and active law interception now suppress the actionable reply state before cancelling the quiet call, preventing a stale Triangle prompt that cannot be used.
- Adds a focused regression for the exact trader call, its full conversation transition and its combat/law interruption state.

Verification: focused Quiet Watch review passes all three scenarios.

## 2.5.298 - Issued Commander identity card

- Redesigns Commander Profile as a native 480x272 spacecraft pilot card with a holographic portrait, verification stamp, pilot serial and barcode, flight-record insignia, career chips and bounded controls.
- Separates balance, kills, legal state, discoveries and current berth into readable instrument cells while retaining the active commander identity and ship.
- Removes redundant button descriptions, centres the single-line SAVE PROFILE and RENAME COMMANDER labels, and shortens the footer to unambiguous icon-led OPEN and BACK controls.
- Adds a focused PSP framebuffer review using a long commander name and the longest shipyard label, covering both action-selection states.

Verification: native Commander Profile review passes both label-fit and selection-state checks.

## 2.5.297 - Giant freighter wrecks and authored discoveries

- Replaces the Abandoned Giant Freighter's temporary stretched box with a solid, shaded 5.4 km broken heavy freighter: six tapered armour sections, exposed spine, bridge tower, engine pods, breached edges and damaged hull colouring. Wire lines are retained only as seams, ribs and torn structure.
- Adds independently flickering navigation and emergency lights, sixteen drifting wreck fragments and a thirty-four-particle dust volume around the hull.
- Rebuilds the Listening Post, Animal Transport Cage and Wildlife Sanctuary Buoy as distinct three-dimensional structures, including animated life signs.
- Gives the remaining discovery pool varied vessel, habitat, platform and barge silhouettes instead of a shared rectangle or ring placeholder.
- Adds a native 480x272 three-range visual regression that verifies the wreck remains detailed and readable from navigation range through close inspection.

Verification: PSP build completes; focused wreck review passes at all three ranges; full PPSSPP core suite passes 615 gameplay, 60 radio and 18 steering checks.

## 2.5.296 - Continuous trader conversations

- Removes the redundant "Nothing changes until you choose and confirm a response" instruction above encounter replies.
- Trader cargo exchanges now remain inside the paused conversation screen from initial response through offer, accept/refuse decision and closing line.
- The offer is presented as a clear GIVE / RECEIVE card. Accepting completes the exchange when the cargo is aboard or records the deal for a later hail; refusing cancels it without changing cargo.
- Returning to flight happens only after the closing transmission, then the trader resumes its route in visible space.
## 2.5.295 - Commander Profile and local law terminal

- Replaces the in-game Save / Status list with a native 480x272 Commander Profile console: identity portrait, ship, berth, balance, kills, discoveries and legal state are arranged as a spacecraft personnel record.
- The in-game profile now offers only Save Profile and Rename Commander. It always writes the active commander record; slot switching, loading and deletion remain exclusively on the title-screen Load Commander flow.
- Adds a docked LAW service to the Fly panel. Its warrant ledger lists active fines by issuing system, shows offence and fine details, and opens a Local Law call to settle a warrant in the current system.
- Remote warrants remain visible but cannot be paid remotely. Paying Local Law clears only that system's warrant and leaves other jurisdictions intact.
- Preserves the new-commander name and portrait creator, tutorial saving, backup recovery and existing per-system warrant persistence.

Verification: PSP build completes; full PPSSPP core suite passes 615 gameplay, 60 radio and 18 steering checks. Added focused input checks for fixed profile records, retained title loading/creation, per-system warrant listing, local payment and remote-payment rejection.
## 2.5.294 - Cockpit destruction sequence

Death now holds the camera on a slow hull breakup and expanding explosion while the live cockpit fails around it. Scan-line faults, red/cyan sync tears and progressive instrument blackouts replace the instant result card; the top and bottom HUD finally lose power altogether. The obsolete wireframe-debris, hull-lost and START prompts are removed. After the cinematic, a D-pad/X recovery panel offers Load Commander or Main Menu, with tutorial and campaign recovery routed through the same choice.

## 2.5.293 - Clear, natural space conversations

Pirate cargo demands now identify as pirate transmissions instead of Trader calls. Opening an incoming conversation pauses at the decision and cannot silently accept an encounter. Every encounter gets plain-English, consequence-labelled replies plus a clarification response. Trader exchanges state the requested cargo, reward cargo and zero-credit terms, record the agreement, report insufficient cargo precisely, and confirm both sides of a completed transfer. Repeated combat and emergency chatter has also been rewritten as natural spoken sentences.

## 2.5.292 - Physical tool-selection confirmation

Circle plus a direction now flashes the complete corresponding control box red twice, including its border and recessed background. Labels stay cream and readable instead of carrying the confirmation alone. The right-hand weapon control retains its weapon-name transition and success/failure feedback inside the flashing physical control.

## 2.5.291 - Spiral warp apex

At the corridor midpoint, an original retro sci-fi vortex now eases into view, peaks around the ship and collapses behind it. Six curved spiral rails, rotating broken elliptical rings and a luminous central throat turn the apex into a clear destination that the ship visibly enters and exits. Rendering stays bounded to the flight canopy and uses a small fixed geometry budget suitable for PSP hardware.

## 2.5.290 - Seamless hyperspace surge

The hyperspace corridor no longer displays its title or progress percentage, leaving the colour tunnel unobstructed. Charging and braking retain short status labels without percentages and now acquire restrained position/ghost glitches. During the final 38% of charge, up to 64 long cyan, cream, orange and violet streaks grow out of the existing speed-line field, bridging the live cockpit into the full corridor instead of cutting abruptly.

## 2.5.289 - Radar system identity

The flight radar's upper `AHEAD` caption now shows the current solar-system name, centred within the instrument and capped at 16 characters for PSP readability. The redundant lower `AFT` lettering is removed. An active Law Scanner countdown still temporarily owns the upper label area.

## 2.5.288 - Quiet cockpit actions and visual tool confirmation

Hard braking, HUD layout changes, tool selection and ignored conversations no longer summon redundant ship-computer messages. An unsuccessful tractor attempt now chooses from eight conversational responses, including reminders that recoverable cargo must be within 500 metres. Circle+D-pad selections remain visible briefly and flash twice. NEXT WPN instead animates the old weapon name off-screen and the new name in, then pulses green; a single or missing weapon flashes the weapon control and current bank red twice without chatter.

## 2.5.287 - Docked station orbit preview

The command deck's right-hand beauty viewport now shows the exterior of the station currently holding the commander. Primary hubs use their complete seeded station architecture; auxiliary relays use their matching Coriolis profile. Small ports make a clean slow orbit, rich hubs glide through sunlit three-quarter compositions, and Mega Capitals alternate wide establishing angles with offset close passes over their major structures. All transitions ease continuously through a 96-second reel; rich and capital shots include a restrained, hull-occluded sun flare. Launching restores the existing ship-and-planets loop.

## 2.5.286 - Automatic PSP Wi-Fi profile search

Online mode now reads every saved PSP Infrastructure connection, tries the preferred profile first, then cycles through the remaining valid profiles until one connects. Missing profile slots are skipped immediately. Nearby Players shows which profile is being tried and reports the profile that reached the PC universe. Home Wi-Fi stored as profile 6 no longer requires editing `online.cfg`.

Spacebook Inbox again looks like part of the Facebook parody: the ordinary and Homeward/story inboxes now share the pale-blue site chrome, avatar contact rail, selected conversation header and clear sender/player message bubbles. Dad, Mum, Kat, Scott and Lave Dock Services threads retain their story actions.

## 2.5.285 - Ship-themed Galaxy.Maps and Comms Panel

Rename the galaxy interface to Galaxy.Maps throughout player-facing UI, remove the legacy ARCElite masthead, and carry the fitted PIMP-MY-SHIP.NET finish into the map glass, routes, navigation furniture and the physical Comms Panel. Semantic warnings and PSP button colours remain distinct. Native 480x272 palette checks cover all sixteen finishes.

## 2.5.284 - Readable system-map labels

The selected station, sun or planet in System Almanac now receives a dark navy name plate blended over the map at roughly 70% opacity. Bright artwork remains faintly visible beneath it while white text and the cyan border stay crisp. This fixes Lave III's label becoming lost over Lave's sun and applies consistently to every selected body.

Exact packaged EBOOT verified: 615 game, 60 radio and 18 steering checks pass with zero failures; the dedicated opacity check passes and the native Lave III screenshot was inspected. The broader input suite retains known unrelated failures. Private local build. See BUILD-2.5.284.txt for hash and hardware limits.

## 2.5.283 - Complete tree sprites and Far Horizons

Fixed vegetation extraction: the source trees overlap the atlas's nominal equal-width columns. Individual authored source rectangles now preserve complete canopies and exclude fragments of adjacent trees/ferns. The same corrected 48x64 sprites serve tutorial Earth, Lave I and the meadow's distant groves. Wind remains enabled. Renderer sampling rejects invalid cells and out-of-bounds coordinates before integer conversion, with samples aligned to the terrain's 2x2 pixel grid.

The second planetary atlas is corrected too: individually bounded silhouettes replace uniform 4x6 cuts, restoring the forest canopy and removing adjacent shrub/rock fragments. All 24 biome props now have distinct pixel ownership; their native preview matches the compiled PSP palette and alpha. Both atlases retain transparent gutters. Source PNGs remain unchanged.

Far Horizons remains the startup default and is now the tutorial radio choice and the station acquired on arrival at Lave. The radio lesson accepts other manually chosen stations too. Existing saved preferences remain respected. The dream's temporary Sol Rock battle score is unchanged. User MP3s stay in music/Far Horizons; no music or save files are replaced.

Private local build. See BUILD-2.5.283.txt for exact-EBOOT checks and remaining hardware limits.

Verified exact packaged EBOOT: 331 opening, 615 game, 60 radio, 18 steering and 1 live meadow PASS; all RESULT 0 failures. Five asset checks pass. Wind/pitch/edge rendering covered across 1,296 poses on four planets; live meadow checked 538 frames on both VRAM buffers with zero stale pixels. See BUILD-2.5.283.txt for hashes, captures and emulator-only performance limits.

## 2.5.282 - Meadow edges and distant countryside

The tutorial meadow repaints its complete header and footer every frame. The old gaps above and below the 3D viewport could retain parts of Dad's portrait after dialogue closed. Regression coverage now renders over previous dialogue without the screenshot helper's full-screen clear, including all HUD modes and both live VRAM buffers.

Removed cockpit bloom from the meadow: its in-place brightening turned the pale horizon into a speckled white strip and cropped edge shading at cockpit height. Sun and stone retain their local effects. Terrain rejects fully offscreen tiles above and below the view before rasterization.

One connected ground mesh now reaches sixty kilometres, with additional field detail outside the playable clearing and a pitch-correct land-haze horizon. Distant pastures and 96 trees in irregular groves include twelve taller landmarks. Far trees use the displayed mesh height so they stay planted on the slopes. Near walking terrain, collision, flower trail, optional conversations and orb choices remain compatible with existing checkpoints.

Private local build. Verification results and physical-PSP limits are recorded in BUILD-2.5.282.txt. This meadow update does not yet restore the standard cockpit HUD in the dream; that requested consistency correction remains next.

## 2.5.281 - Empty-space dream battle

The opening eyes now blink and open in 1.6 seconds. Three Thargoids arrive together in an arrowhead, close in, break away and turn back into attack runs. Their positions advance continuously in world space; they are no longer repositioned around the camera. Bounded, aimed projectiles use swept collision, so a missed shot causes no damage. Player steering, throttle, braking, lasers, heat and shields retain the normal flight implementation.

The dream takes place in empty space, without planets, stations, civilians, freighters or ordinary mission/law displays. Dream hits cannot create crimes, loot or bounty rewards. A dedicated Night Shift radio ticker supplies two minutes of surreal stories that hint indirectly at the dream. SOL ROCK starts with the attack and fades while waking; saved radio choices remain unchanged. Waking occurs after defeating all three ships, reaching low hull, or 120 seconds. No save-format change.

Private local build. See BUILD-2.5.281.txt for exact-EBOOT verification and hardware limits.

## 2.5.280 - Follow the strange flowers

A winding trail of pale violet and mint flowers leads from beside Dad's ship towards the meadow stone. Their tiny pixel petals lean towards it and softly glimmer. Triangle can inspect the flowers once for a short clue from Dad, awakening the stone early; ordinary exploration still reveals it if the clue is skipped. The discovery is saved and leads into the existing stone conversation and show/hide choice.

The stone has a brighter distant core, a soft pulsing halo and orbiting motes. Its glow respects terrain and foliage depth and disappears after collection. Effects use bounded native-resolution drawing. See BUILD-2.5.280.txt for exact-EBOOT verification. Physical PSP and broad legacy smoke remain separate gates. Private local build.

## 2.5.279 - Continuous meadow terrain and natural boundaries

Fixed the tutorial meadow's disappearing ground: the former 440m-wide patch left gaps inside the walkable clearing and never joined its distant terrain. One connected, cached lattice now covers the clearing and surrounding landscape, with matching walking heights and conservative whole-tile visibility checks. The pond and stream use flat water above solid basin terrain. Replaced the cropped Alpine picture overlay with a continuous sky and real distant hills, eliminating the abrupt blue edge when looking up.

The clearing now has irregular wooded banks, 72 trees, 18 collidable rock outcrops, a curved valley cliff and softer grass-covered slopes instead of the straight row of box-shaped stones. Preserved flowers, birds, butterflies, Dad's exploration dialogue and the orb objective. Removed the stray Earth garage sign and duplicate generic ambient effects. Cached tree/rock positions reduce repeated work during walking. Old checkpoints that overlap changed terrain obstacles recover beside the ship.

Validation is recorded in BUILD-2.5.279.txt: native-resolution views, 320-view depth coverage, actual collision-route reachability, sky continuity, full opening walkthroughs and core game/radio/steering checks. Physical PSP performance and broader legacy smoke remain separate, unverified gates. Private local build; no publication.

## 2.5.278 - Dad's Alpine meadow exploration

Dad's conversation now leads into an unhurried compact exploration beat. Optional lakeside and cliff-overlook inspections add family dialogue; after walking for a while the player notices the orb. The orb has a longer branching inspection before the existing show/hide choice. Optional discoveries use previously reserved opening flag bits; opening stage/node IDs and checkpoint layout remain intact.

The Earth clearing has cached shared-collision ground, 32 authored tree placements, bounded flower/undergrowth sprites, a lake and meandering stream, a physical cliff face and stone lip, birds and butterflies. Walkers are kept within the clearing and away from water/cliff edges and tree trunks. Distant Alpine art moves with camera pitch at fixed scale instead of stretching. Removed the top-left glowing-object instructions and full-width exploration coaching; nearby interactions show only a compact Triangle Inspect prompt. The generic Earth port/rover/POI clutter is suppressed. Rendering uses cached vertices and solid depth-tested ground instead of per-pixel meadow textures; physical PSP speed must still be measured.

Verification: exact packaged EBOOT passes 289 opening/comms/portrait/meadow checks, RESULT 0 failures. Three starter-ship tutorial walkthroughs reach free play; object show/hide outcomes persist. Native four-bearing/up/down/lake/overlook captures inspected. New scene render averages 21.480 ms against 56.073 ms for legacy ground and sky sampling across 12 bearings with identical new props (61.7% less render time). This is an emulator render-cost comparison, not physical PSP FPS certification. Scoped source diff check passes with Windows CRLF respected. Private local build; physical PSP and broad legacy regression remain outstanding.

## 2.5.277 - Expanded portrait creator and individual discovery faces

Eight eye shapes, eight mouth styles and eight ear/sensor styles for all ten races. Every face-variant entry now selects a different authored base; 96 existing pulp portrait bases remain in use. Discovery speakers gain stable individual seeds instead of sharing one face by occupation. Creator labels describe the actual parts and include a native-size preview. Four-byte portrait DNA now uses a validated extension version; old saved portraits remain accepted. Private local build; physical PSP and legacy broad regression remain outstanding.

## 2.5.276 - Varied station greetings

Twenty friendly station greetings plus context-specific distant, wanted and already-docked replies. Per-station rotation avoids consecutive repetition; each hub has a stable operator portrait. Captions say DOCK CONTROL. Hails remain in flight and do not grant docking, clear warrants, or bypass existing docking/security rules. Hold Triangle still opens the console; tapping hails/closes/quick-replies. Focused tests cover greeting variety, readable length, range/warrant conditions and no automatic docking. Private local build; physical PSP and legacy broad regression remain outstanding.

## 2.5.275 - One canonical computer portrait

The authored intro computer bitmap (opening_portrait6) is now the shared ship_computer_portrait renderer for the opening, cockpit computer notices/automated inspections and full dialogue computer portraits. Removed the different procedural robot face from these paths. Native nearest-neighbour rendering retains the same square face at 28/32/48/56 pixels. A pixel comparison checks that intro and cockpit render identical 32px output. Private local build; physical PSP and legacy broad regression remain outstanding.

## 2.5.274 - Discovery speakers and Triangle shortcuts

All 75 discovery entries now have explicit compact speaker identities. Mobile Mechanic speaks as MECHANIC, animal transport as KEEPER, and abandoned/silent objects use COMPUTER or RECORDING with the computer portrait. Ordinary transmissions reset the source marker so later speakers cannot inherit the previous discovery identity. Both flight captions and conversation rendering use the same source lookup.

Station hails no longer open the Comms settings console. Tap Triangle hails/closes or opens the quick reply chooser; hold Triangle for 0.55 seconds opens the paused console. Private incoming calls also use the quick chooser instead of jumping straight into the panel. Confirming RESPOND can still open the existing conversation, separate from console settings. Focused regression includes all catalogue speaker labels, mechanic/computer identity, stale-source clearing and tap-versus-hold station input. Private build; preserve shared edits and do not publish automatically. Physical PSP and legacy broad regression remain outstanding.

## 2.5.273 - Proportional translucent rings and contact sensors

Rings now span approximately 1.4-2.6 planet radii with half-opacity coloured material, density bands/gaps, soft annulus edges, globe concealment and planetary shadow; shared asteroid placement follows the smaller geometry. Sensor range uses actual scanner equipment (10/20/30 km for normal contacts, larger signatures for stations/freighters). Planets, suns, intersystem routes and mission docking waypoints remain navigation references. Out-of-range selected ships lose alignment and retain their last seen position. Locking/proximity no longer identifies ships. Close Triangle hail identifies them; close debris inspection preserves existing cargo recovery. Discovery/anomaly identities have per-commander checksummed sidecar files. NPC identification remains attached to the live NPC; new spawns reset it. Bounty selection no longer names/auto-aligns the suspect or highlights unidentified bounty ships. See docs/RINGS-SENSORS-273.md. Private local build; physical PSP testing and legacy broad regression remain outstanding.

## 2.5.272 — Space discovery foundation

Eight seeded lightweight discovery contacts in Lave and development systems 0/1/2; 75-entry authored response catalogue, appended target IDs, OTHER-band selection and shared Comms hails. Listening-post, derelict, animal-cage and buoy wire prototypes. Prototype geometry is not final art. Full expansion, traffic choreography, cruise liners, distance/warp/fuel revisions and physical PSP certification remain pending. See docs/SPACE-EXPANSION-272.md. Private local work; preserve unrelated edits and do not publish automatically.

## 2.5.271 — Single-screen comms console

Exact final EBOOT: 148 PASS checks, RESULT 0 failures through opening-check. Native screenshots inspected. See docs/COMMS-CONSOLE-271.md and BUILD-2.5.271.txt. Physical PSP audio/input and broad legacy regression remain outstanding.

comms-console.h owns physical control geometry, spatial D-pad navigation, native pixel glyphs, availability, activation and comms-console.cfg versioned/checksummed preferences with recovery backup. Legacy IDs 5/9 are excluded (station walking/contrast removed at user request); 4 is now Mission Log. Existing radio.cfg is unchanged. Quiet Mode snapshots prior radio_off/quiet_comms; optional captions are independent. Preserve essential mission/danger warnings. Hold Triangle always enters settings console; actionable calls no longer replace it until Answer Incoming. Console input intercepts before world simulation; conversations retain the existing dialogue renderer. comms-console-tests.h runs through the opening-check harness with geometry/reachability, input, pause, unavailable-state, audio/display, persistence and native screenshot checks. Private local work; preserve unrelated edits and do not publish automatically.

## 2.5.270 — Fly-through planetary rings

sectors.h exports planet_ring_inner/outer/point; physical mining rocks and celestial_rings now share a tilted 3D annulus. Inner radius 2.4x globe; outer 5.8–6.7x globe. Sixteen persistent-in-session ring rocks per gas giant use existing mine_rock/ore recovery. Cosmetic grains are not target entities. Spawn budget: up to 32 ring rocks, ten belt rocks, three existing salvage objects and eight route salvage stops; DEBRIS_COUNT remains 64. Do not increase without checking combat salvage capacity. Landing triggers remain at the globe, not rings. Source tests check ring geometry, no landing and ore fracture and capture wide/interior views. Larger bespoke listening posts, animal cages and enterable wreck interiors remain planned. Physical PSP performance verification remains outstanding. Private local build; preserve shared work.

## 2.5.269 — Adventure panel consistency and dream rock

adventure_panel in ui-modern.h is the shared solid native-resolution chrome used by opening speech, station/site reading, dialogue-ui (incoming conversations and tracked missions), and mission log. Preserve artwork viewports and current pagination. radio_dream_gain in radio-config.h is a transient audio-thread override: source 8/SOL ROCK, silent until the attack cue at 22 seconds, then full player-configured music level, fading through the six-second wake-up. Preferences are never retuned or saved by the dream. Physical PSP audio/layout review remains necessary. Private local work; preserve shared edits and do not publish automatically.

## 2.5.268 — Playable Thargoid dream

Opening dream now uses transient timers in opening-runtime.h: seven-second blinking reveal; three scripted Thargoid contacts at 10/13/16 seconds; attack pressure after 22 seconds; wake on all three defeated, hull at 25 or below, death, or 82 seconds total. Six-second white fade then restores the real commander through OP_HOME. Actual flight and laser input remain active. Focused tests exercise all endings and reward reset; walkthrough permits the longer dream. Physical PSP verification remains outstanding. Preserve unrelated shared work; this is a private local build.

## 2.5.267 — Space chatter above the cockpit

Moved tutorial Dad/computer coaching, longer space conversations and response choices just below the top HUD. Radar and controls remain visible. Ground/menu and illustrated Earth-view framing stays intact. Exact packaged opening and live launch/landing/EVA checks pass. Core objects are unchanged from the passing 2.5.266 build. See docs/HOMEWARD-FIXES-267.md and BUILD-2.5.267.txt. Physical PSP and pre-existing broad legacy issues remain outstanding.

## 2.5.266 — Earth vista, safe approach and native tutorial screens

First-orbit conversation stays over Earth artwork. Premature Earth entry turns safely outward with a nonblocking Dad reminder. Alpine waterfall art replaces the countryside vista. System Details now uses the native almanac with safe selection bounds; GalacticNet shares native newspaper/Spacebook/chrome styling and Earth data. Flight lessons accept controls immediately and use compact coaching. Fixed departure completion on release frames. Exact-binary opening, live atmospheric entry/EVA and core game/radio/steering tests pass. See docs/HOMEWARD-FIXES-266.md and BUILD-2.5.266.txt. Physical PSP and pre-existing broad legacy issues remain outstanding.

## 2.5.265 — 3 October 2026

Replace the rushed Homeward prototype presentation with authored branching dialogue, player-controlled text pacing, illustrated Earth/countryside/cockpit scenes and portraits, a starter-ship catalogue and confirmation step, hidden untaught services/categories, saved conversation state and a real Earth Spacebook photograph. Preserve actual flight/mining/services/comms and later Galaxy Map reveal. Fix missing SOL ROCK ticker entries and guard null radio copy. See docs/HOMEWARD-POLISH-265.md for exact passing tutorial/core/live checks and separate unresolved broad legacy smoke results.

## 2.5.264 — 3 October 2026

Implement Homeward as the default new-game tutorial after name/portrait creation:65 story milestones, Earth city/countryside scenes, gated menus, MiniBolt mining kit, rock radio, family communications, actual services and pizza purchase, Thargoid displacement, delayed Galaxy Map, Lave advert/app and Sol TV news. Preserve legacy campaigns and saves; add V27 story/position snapshots with CRC/backup migration. See docs/HOMEWARD-OPENING-264.md for implementation and limits.

## 2.5.263 — PC development server and online directory

Added bounded156-byte infrastructure presence protocol, private Node UDP directory/relay, endpoint advertisements/direct peer updates, connection cookies, receive budgets, stale-player expiry and offline recovery without commander rollback. Added Nearby Players page, pre-game persistent toggle, activity/system/planet tracking and multi-peer render wrappers. Direct ad-hoc transport remains separate and uses the unchanged262 wire protocol. V26 saves unchanged; first PC test is capped at two safe planetary berths.

Server startup/shutdown scripts manage only their recorded process; no firewall/router changes or public deployment. SDK Inet/Apctl libraries added. Native fixtures and live Inet tests retained in current-task/online-presence-263; final release evidence recorded after tests complete. Public security/authentication, true shared-world authority and capacity expansion remain future work.

## 2.5.262 — Persistent personal multiplayer progress

Removed active-session commander rollback and save protection. Ship purchases, equipped modules, credits and progression persist through leave/BYE/timeout/suspend. Disconnect removes peer contact/replica state without resetting menus, position, planetary cameras or landing/launch sequences. Retained surface berth ownership until returning to orbit prevents parked ship/Roamer relocation. Only failed setup rolls back its temporary snapshot; successful sessions free that snapshot immediately.

Normal docked SAVE/STATUS writes are enabled; load ends the link before using the normal loader. Hosting uses ordinary departure rather than free repair/refuel/amnesty. Guest arrival initializes a usable local system while preserving personal warrants and rift discovery flags. Protocol262 requires matching EBOOTs; save schema unchanged. Shared combat/loot reconciliation remains outside this fix.

## 2.5.261 — Normal controls, friend contacts and independent universe travel

Removed the multiplayer input replacement and permanent name/footer overlay. Normal space/on-foot targeting and Circle tools now work in connected sessions. Named friend contacts report system/planet/vehicle mode and support standard smooth lock and fuel-aware cross-system routes; locks survive normal hyperspace arrival. Other-system/older-visit snapshots cannot corrupt local traffic, and fresh visits reset interpolation goals.

Protocol261 adds system/visit tags and compact pilot/control messages, monotonic real-time deadlines,24-datagram receive budget and60-second lost-peer grace. Duplicate/reordered validated peer activity no longer causes false quiet timeouts. Saves remain protected; main-menu exit closes the session. Retains all260 content. Native two-peer control/planet, independent travel and lost-host recovery reviews are retained in the task candidate; hardware and persistent cooperative combat/progression remain unverified/unfinished.

## 2.5.260 — Planetary co-exploration over local Wi-Fi

Added independent planetary travel using existing cinematics, session world picker, surface/vehicle controls, scanner/Codex/map/site scenes. Two owned landing berths and Roamers, depth-tested animated block suits/shadows, visible remote vehicles with collision bounds. Protocol260 adds location/vehicle/transition snapshots and host wildlife on shared planets; no coordinate-space streaks or wrong-planet ghosts. Single-player layout/traffic/save format unchanged; all multiplayer progress remains session-local and original commanders restored on exit/disconnect/sleep.

Final42 fixtures including all1024 planet berths/exits/Roamer pairs, both native-ad-hoc emulator peers, lost-host guest recovery, core/radio/steering and footing/tools/map/entry checks pass. No physical PSP/FPS certification or persistent cooperative progression. Retains all259 content; see docs/LOCAL-WIFI-PLANETS-260.txt.

## 2.5.259 — Local ad-hoc multiplayer free flight

Added nearby HOST/JOIN buttons to FLY, scanned flight selection, versioned two-player handshake and nonblocking PSP PDP transport. Host supplies its current-system world/NPC/debris/rift state; guests never run independent NPC simulation. Friend ships show their own hull/paint/name with smoothed position/orientation and a friend marker. Bounded snapshots are spread over frames.

Both commanders are backed up in memory and restored on leave, disconnect, timeout or suspend; no save schema changes or guest save writes. Session controls isolate free flight from single-player landing/combat/mission/shop/jump actions. This is a first shared-space slice, not planetary/mission/combat co-op.

37 protocol/navigation/save-protection fixtures and all256 generated-world wire checks pass. Real two-instance PPSSPP discovery/join/shared snapshot/moving-pilot/leave tests pass, as does host-process-loss timeout recovery. Initial live tests caught an invalid pre-group Net MAC request; corrected to the WLAN hardware-address API. Final game594/radio54/steering18 and8192 POI footprint/native-view checks pass; full inherited input/outfitting and physical PSP Wi-Fi/FPS remain uncertified. Cumulative258 retained; no Git publication.

## 2.5.258 — Terrain-safe observatory and landmark foundations

The authored Lave observatory now uses the shared earth footing instead of starting its basement8m below centre-only ground. Foundation levels include clipped terrain-triangle extrema and both rendered LODs, cached in16 tagged entries. Visibility bounds, signs and target markers share the corrected height; open cave interiors remain ground-level. Added deterministic planetary coplanar sort ties after initial comparison failures, without changing space/mega-city ordering. Final8192 footprint checks/all12 families,12 observatory views,144 renderer comparisons and focused planetary/core suites pass. Sample walking cost~1% above257; heavy-scene/hardware/full inherited input-suite limits remain. No saves, collision reservations or controls changed.

## 2.5.257 — Planetary raster and compiler optimization

Replaced hot raster-row libm calls with exact finite-coordinate helpers, reused four-pixel depth masks, rejected off-screen triangles after sorting, and preweighted cloud sprite blends. Both builds enable O3/link-time optimization without fast-math. Starport scale, scenery density/ranges, controls and saves unchanged. Sampled approach drawing cost16–26% lower and wilderness12–18% lower than256; broader arrival/ascent39.872->33.117ms and walking47.278->41.092ms. Final144 image comparisons and12 baseline screenshots match exactly; focused planetary/game/radio/steering suites pass. Pre-sort cull experiment was rejected for coplanar pixel changes. Heavy views/cold spikes, inherited full input/outfitting failures and physical PSP certification remain. See docs/PLANET-FPS-257.txt.

## 2.5.256 — Central cloud corridor, cached planetary scenery and bridge visibility

Post-loading arrival veil now grows/flows outward around the ship's central flight corridor; original fully opaque handoff, timings, controls and departure veil are retained. Added a bounded28KiB tagged biome-prop ring cache; only newly encountered cells regenerate, preserving original positions, silhouettes, materials and draw distances. Cloud-platform worlds skip the old unused vegetation search. Near terrain draws before distant terrain; guaranteed-dry meadow quads skip redundant water sampling, while river/ocean masks remain exact. Flat port terrain returns its exact24m height before expensive noise; river basis and integer terrain-cell floors avoid repeated work. Bridge decks and ramps gain5m thickness, underside/edge faces, and a conservative full-span visibility bound rather than centre-only culling.

All twelve before/after profile screenshots are pixel-identical, all32704 cached property miss/hit checks match uncached generation across the galaxy, and twelve isolated bridge angles pass including underside/centre-behind-camera. Arrival/ascent sample48.978->39.872ms avg; four wilderness samples improve26–48%; standard Lave walking56.534->47.278ms. Gains vary: Lave I approach changes only about1%, cold cloud-world approach still reaches167.462ms in the broader suite, and thick bridge rendering has a small added cost. Final focused/cloud/entry/world/art/scanner/map/rover/144-frame renderer and game594/radio54/steering18 tests pass. Full inherited input/outfitting suite and physical PSP FPS are not certified.

## 2.5.255 — Water textures, bridge alignment and Hand Tools

Added bounded80KiB tagged water cells with moving ripple colours, shore foam and sky-tinted palette; exact40m river mask is independent of terrain LOD. Dry distant quads retain the inexpensive flat renderer. Raw riverbed remains under bridges; matching52m-wide decks span480m with40m bank ramps. Bridge floor height is shared by feet/wheels/rendering, rather than reserving a440m-wide invisible crossing. Vegetation generation/collision excludes the water under decks. Hold Circle on foot selects scanner/empty hands without scan-on-release; tap uses the visible first-person scanner and existing rewards/Codex/save flow. Map/menu/transfer release guards prevent control bleed; rover drift and rover target-panel scans remain separate. Save format and user music unchanged.

Final focused tool/water, all1024-world layout, field art/species/save, walking/collision, map, rover, landing,144 reference/fast frame comparisons and game594/radio54/steering18 checks pass. Not a full-suite or hardware certification: inherited input/outfitting issues remain, and walking render samples average56.534ms/worst89.806ms including cache refresh. See docs/WATER-HAND-TOOLS-255.txt.

## 2.5.254 — Layered cloud banks and atmosphere rendering savings

Replaced flat sky strips with shaded multi-lobed cloud silhouettes across three world-bearing altitude/distance banks. Entry/launch veil samples three independently drifting cloud scales, retaining translucent scenery gaps and strictly opaque loading handoffs. The four code-native64x32 cloud variants use8KiB, prewarmed at startup. Palette and alpha tables plus integer blending replace repeated pixel colour interpolation and veil waves. No assets, saves, music or controls removed.

Final native cloud compositing9.365ms vs253 baseline13.564ms (~31% less); sampled arrival/ascent47.676ms vs49.921ms (~4.5% less). Lave aggregate rendering46.405ms vs46.921ms; walking45ms remains above a30FPS budget. Cloud/entry safety,144 pixel comparisons, cinematic paths, walking/collision/art, game/radio/steering checks pass. Physical PSP and full inherited input/performance certification remain outstanding.

## 2.5.253 — Planetary scenery, ridge visibility and driving clearance

Added coarse 7 km real-terrain rendering on solid Lave worlds and a higher/broader Lave I observatory ridge with the authored domed silhouette visible from the port. Added bounded trail flower verges, 52–136 m tree-height variation, anchored foliage sway and nearby grass wind animation. Wilderness cache follows the player instead of leaving collidable but unrendered trees outside the original port area. Shared seeded starport terminals have unequal upper pods/decks; open terraces are nearly flush with the apron and traversable. Vacant traffic pads are traversable while descending/parked hulls retain clearance. Solid-world crystal grotto side/rear walls match an open entrance/aisle; cloud settlements retain their original crystal architecture. Fixed inherited Lave I apron interpolation unevenness. Save layout and controls unchanged.

Native focused scenery/collision, 1024-world port, field-art/species, map, rover, expanded-world, cinematic path and 144-frame differential checks pass. Game 594, radio 54, steering 18 pass. Added detail increases paired sampled Lave walking render from 39.144 to 41.431 ms; no locked-30 or hardware claim. Full inherited input/outfitting suite exposes failures and the broad smoke/performance run timed out. Detailed evidence and limitations in docs/PLANET-BEAUTY-253.md.

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






## 2.5.339 - Mission board and crowded-flight performance

- Mission-board cards share one render-scoped neighbourhood search; availability checks read deterministic planet types without copying the entire commander or regenerating systems.
- Reuse a bounded ten-sheet planet cache across menus and flight, including rolled views. Cached artwork preserves the existing pixels.
- Cull off-screen hulls, discoveries, stations and triangles before unnecessary model/raster work. Keep hulls crossing screen edges or the near plane.
- Cache mesh bounds, transform hull vertices using one basis per model, and reduce subpixel distant hull/debris details.
- Keep short luminous trails on small ships while reserving expensive flame particles for larger close-up ships. Budget the additional cosmetic plumes independently of gameplay.
- Avoid painting the background twice and use paired writes for the filtered nebula wash. Stars, world sprites and interface stay native resolution.
- Reuse system star directions and skip painter sorting for opaque hull batches already resolved by the depth buffer.
- Add `tools/review-performance.ps1`: exhaustive mission equivalence, pixel-identical planet-cache tests, screen-boundary tests, six deterministic workload comparisons and a nominal 30 FPS CPU budget check. Emulator timings do not certify physical PSP frame rate.

Verification: PSP build and all focused performance/compatibility gates pass. Five live rotating-flight benchmarks improve from 18.31–25.57 FPS to 29.97–41.38 FPS. Crowded close-up drawing improves from 47.46 to 28.68 ms; rolled multi-planet drawing from 77.42 to 23.39 ms. Worst live frame remains 66.73 ms: these are measured improvements, not a claim of locked hardware FPS. The unchanged baseline has inherited broad smoke failures; see the performance handoff for scope and evidence.



Graphics integration checkpoint (2026-10-13): station component art, sun-matched lighting, bespoke Adder NPC pilot, tapered wakes and distant steady cores/navigation blinks are now integrated into canonical src. Supersedes earlier notes saying these renderers are isolated only. Local retail 2.5.453 playtest: outputs/releases/2.5.453-graphics-playtest/ELITE-NEXT-PSP/EBOOT.PBP; READ-ME.md records evidence and limits. Original-PSP-model PPSSPP startup/music/countdown/concurrent saving pass; populated race 57.77 FPS; busy music stress 31.18 FPS with zero buffer starvation/decoder errors. Native station checks 119 pass; staged ship occlusion/blink checks pass. Remaining 43 fleet designs and physical PSP acceptance are unfinished; station FPS still needs work. No gameplay/save/audio/ERL protected source changes. Stable VERSION/dist remain 452 pending full gate; no remote sync/release claim.


2.5.454 local audio click fix (2026-10-13): integrated graphics 453 retained; audio stereo gain/pan multiplication overflow corrected with wide arithmetic in event and continuous layers. Native full-range probe 1,821,015 cases passes; real authored effects expose 39 vulnerable full-pan samples. Exact-retail original-model startup, MP3 and OGG continuity tests pass with zero buffer starvation/decoder errors. Captured PCM has no full-scale clipping or >half-scale adjacent steps. Host test buffering separately increased to 1024/ExtraAudioBuffering. Busy flight remains 24-30 FPS and physical PSP listening pending; do not claim every scene/source crackle-free. Local package outputs/releases/2.5.454-audio-click-fix; preserves saves, ERL, artwork and unrelated work. Stable VERSION/dist remain452 due existing full-suite failures; no GitHub release/sync claim.


2.5.455 space backdrop local playtest: cached system-varied clouds darkened for cleaner space; bilinear subpixel ordinary-flight stars reduce camera-turn popping; existing gentle twinkle/dust retained. Game-renderer native sky continuity/clip/guard checks and256system catalogue pass. Warm sky3.91ms; cold generation176.85ms remains a first-view/transition limitation. Actual retail MP3/OGG buffer continuity pass (30.37/24.05FPS busy scenes); race countdown/save test57.11FPS,4saves,zero>100ms. Audio454 and protected gameplay/save/ERL files unchanged. Native captures and EBOOT: outputs/releases/2.5.455-space-backdrop. Physical PSP, first-view prewarming and heavy station performance remain open. Stable452 and GitHub unchanged due existing broader failures.


2.5.456 local FPS/stability pass: station edge divisions hoisted per triangle, deterministic opaque station depth ties, initial Lave sky prewarmed during startup. Three matched native station views pixel-identical with station-stage savings0.8-1.0ms (4-7%).119 station checks and native sky/star clipping/continuity/guards pass. MP3 busy flight30.81FPS (prior30.37), no buffer starvation; OGG24.03FPS,worst113.72ms, no audio gaps but still slow. Initial sky draw5.48ms (prior cold176.85ms), generation moved tostartup; different-system cache cost still open. Local EBOOT/evidence outputs/releases/2.5.456-station-fps. Protected gameplay/audio hashes unchanged. Physical PSP, crowded station/OGG FPS and broader legacy regressions unfinished; no glitch-free/all-green or GitHub sync claim. Stable452 preserved.


2.5.457 local background freighter art: muted sunlit metal faces, drive collars and distance-limited ribs/glazing; shorter tapered budgeted wakes behind hulls, rear-axis engine flares after hulls, before planets. Existing4 deep-sky silhouettes/routes retained; not a full bespoke textured fleet completion. Six native staged cameras and24lossless motion frames/guards pass. Retail memory guard and MP3/OGG continuity pass, zero starvation/decoder errors;30.50/23.76FPS busy scenes, heavy station/OGG performance still open. Audio454,sky455,FPS456 and protected gameplay/save/ERL hashes retained. Local EBOOT/gallery outputs/releases/2.5.457-freighter-art. Physical PSP and exhaustive occlusion acceptance pending; stable452 and GitHub unchanged.


2.5.458 local textured background freighters: perspective-correct six fitted component pixel-art tiles with32/16/8mips, shared quad mip choice, dedicated readable metal palette and existing world-sun face shading.4032bytes texture data, no external assets/per-frame allocation. Retains457wake/flares, audio454,sky455,FPS456 and protected gameplay/save/ERL hashes. Native six staged poses/24lossless frames and guards pass. Retail MP3/OGG zero buffer starvation/decoder errors;30.48/23.67FPS busy scenes; heavy station/OGG still slow. Memory guard passes with5268bytes margin. EBOOT/gallery: outputs/releases/2.5.458-textured-freighters. Shared fitted component art on4existing silhouettes, not full bespoke fleet completion. Physical PSP and exhaustive temporal mip/occlusion acceptance pending. Stable452/GitHub unchanged.


2.5.459 local planet lighting: approved sun-driven day/night terminator integrated into opaque sprite texels, palette-related5pixel atmosphere glow before each disc. No shading of foreground ships/menus; high-contrast bypass; tiny normal/gain lookup allocated once with safe fallback. Retail memory guard passes by676bytes; avoid static growth. Actual MP3/OGG continuity logs packaged, no audio gaps; near-planet performance/hardware acceptance pending. Four isolated glow variants at moderate/far ship ranges,192nativeframes/guards/lossless clips: outputs/releases/2.5.459-planet-lighting/previews/gallery.html. Fixed preview hue/shimmer not installed settings; installed atmosphere uses world palette. Preserves458and protected gameplay/save/audio/ERL hashes. Physical PSP, close-planet FPS, exhaustive visual acceptance and broader legacy regressions unfinished; stable452/GitHub unchanged.
