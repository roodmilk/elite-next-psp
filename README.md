2.5.488 packaging candidate keeps the detailed Twin Stack gun housings permanently extended and restores immediate X firing. Circle+R is unassigned and no longer appears in the controls. The commander menu removes the decorative SHIP / COMMAND LINK heading and omits STANDBY for a new commander. ERL finishes no longer force a six-second replay: the existing native ship flyby begins immediately, strikes the player's real position numeral and resolves to the correct ordinal result (1ST, 2ND, 3RD, etc.). Race recordings remain available only as optional LocalTV highlights. Circle+Triangle remains flares, Circle+Down remains heat sink and Circle+Right remains next fitted gun. No save-format or weapon-balance change.

2.5.487 hardpoints: hold Circle and tap R to retract/extend the detailed Twin Stack housings. Hold X while stowed to deploy; primary fire waits for the 0.55-second actuator to finish and then fires while X remains held. Retraction moves the actual 3D model below the sill; the fully extended cached artwork is unchanged. Runtime actuator state resets on ship change; no save-format change. Circle+Triangle now selects flares, replacing the former duplicate heat-sink shortcut; Circle+Down remains heat sink. Circle+Right gun selection and ordinary R boost remain separate. 73 focused native checks pass (7 hardpoint, 66 cockpit). Retail PSP-3000 memory budget passes. New performance/hardware measurements pending. Original 486 and PSP stable packages retained. No Git metadata exists here; no push/tag claimed.

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

Highest-priority remaining work: physical PSP-3000 check of navigation, cloud boundaries and saves; investigate remaining frame-time spikes; repair the pre-existing full-smoke test-helper drift; validate extended career/escort/planetary playthroughs and expand the remaining R&D event proposals only from these shared foundations.

2.5.485 PSP-3000 local playtest rebuilds the short lower-corner Twin Stack as solid 3D guns. Each bank has two separated eight-sided barrels with closed caps, raised sleeve collars, narrow closed breeches, top/side/underside facet shading, vents, bolts and cyan material accents. Both sides of each receiver and all barrel faces carry depth, replacing the broad flat receiver sheets. The short inward-facing centreline and lower-corner mounting positions are retained. Alternating discharge now uses the actual lower/upper barrel centres at six-unit spacing, with emission beyond their matching far tips. Weapon slots, input, damage, cooldowns and aiming rules are unchanged.

308 focused native checks pass, including actual Anaconda shipyard purchase, all four fitted gun identities, Circle+Right immediate switch/wrap, lower outer-corner receiver projections and matching far-tip emission axes. Previous cockpit, fitting/save, cosmetics, menu, departure/adverts and ERL tests pass. All-hull captures are retained with the diagnostic reports; source hashes are in design/guns485. Native before/after, approved reference and80-frame lossless real retail footage: assets/generated/guns485/index.html. Footage writing overhead is excluded from separate performance tests.

Native-resolution PPSSPP Anaconda timings: hit 31.94, 29.89, 29.9 FPS; switch 31.94, 29.89, 29.9 FPS; busy 28.38, 28.75, 28.67 FPS; station 29.44, 29.75, 29.24 FPS; thirdperson 28.46, 29.1, 28.81 FPS. Zero audio starvation and command drops. Strict-floor limitations: ['busy misses the strict 29.5 FPS floor: 28.38, 28.75, 28.67 FPS', 'station misses the strict 29.5 FPS floor: 29.44, 29.75, 29.24 FPS', 'thirdperson misses the strict 29.5 FPS floor: 28.46, 29.1, 28.81 FPS']. Worst frames are in validation.json; physical PSP confirmation, occasional frame-time spikes and pre-existing broader smoke failures remain open. Memory: PASS retail PSP memory budget: image=22851608 heap=12582912 main_stack=1048576 headroom=18042856 psp3000=True. The user accepts approximately29FPS with visual fidelity preserved; continue to investigate noticeable frame-time spikes rather than reducing the approved model to chase exactly30FPS. This is a local playtest, not a locked-framerate release claim. Saves protected and intro bytes unchanged. Previous484 package and original PSP stable distribution retained. No Git metadata exists here; no commit, push, tag or GitHub release is claimed. SHA-256: 2CF6DB3A524029EC6F69ACE4BD21662B368BA3303B0D3297F9B69DCEFD9F1F38

Next: physically check the Anaconda cockpit housings, four fitted guns and far-tip firing on PSP-3000. Continue frame-time optimisation and outstanding encounter work without reducing approved artwork. Preserve the separation between weapon slot capacity and the visible paired Twin Stack housings.

2.5.484 PSP-3000 local playtest moves detailed Twin Stack receivers outward and rearward to the lower cockpit corners, with short stacked sleeves facing inward. The sleeves stop near the lower corners and shots converge from their matching tips. The lower mounting edge reaches the cockpit sill. Cross-section girth is doubled so the detailed receivers and separated sleeves remain substantial at this sweeping angle. No sprites or material details were removed. Muzzle emission follows the actual slanted barrel tangent beyond its tip. Hull attachment anchors, damage, cooldowns, aiming and four-slot selection behavior are preserved.

308 focused native checks pass, including actual Anaconda shipyard purchase, all four fitted gun identities, Circle+Right immediate switch/wrap, lower outer-corner receiver projections and matching far-tip emission axes. Previous cockpit, fitting/save, cosmetics, menu, departure/adverts and ERL tests pass. All-hull captures are retained with the diagnostic reports; source hashes are in design/guns484. Native before/after, approved reference and80-frame lossless real retail footage: assets/generated/guns484/index.html. Footage writing overhead is excluded from separate performance tests.

Native-resolution PPSSPP Anaconda timings: hit 35.38, 29.9, 29.89 FPS; switch 35.14, 29.88, 29.91 FPS; busy 28.46, 28.82, 28.6 FPS; station 29.28, 29.6, 29.38 FPS. Zero audio starvation and command drops. Strict-floor limitations: ['busy misses the strict 29.5 FPS floor: 28.46, 28.82, 28.6 FPS', 'station misses the strict 29.5 FPS floor: 29.28, 29.6, 29.38 FPS']. Worst frames are in validation.json; physical PSP confirmation, occasional frame-time spikes and pre-existing broader smoke failures remain open. Memory: PASS retail PSP memory budget: image=22848344 heap=12582912 main_stack=1048576 headroom=18046120 psp3000=True. This is a local playtest, not a locked-framerate release claim. Saves protected and intro bytes unchanged. Previous483 package and original PSP stable distribution retained. No Git metadata exists here; no commit, push, tag or GitHub release is claimed. SHA-256: 0E976412D67A4C7CD2178564F0657E41478FBC596C8626FDD632EEE90675E16D

Next: physically check the Anaconda cockpit housings, four fitted guns and far-tip firing on PSP-3000. Continue frame-time optimisation and outstanding encounter work without reducing approved artwork. Preserve the separation between weapon slot capacity and the visible paired Twin Stack housings.

2.5.483 PSP-3000 local playtest fixes tiny, high Twin Stack housings on large ships. The old 12m model-size ceiling shrank the Anaconda's guns despite their real hull mounts being over 200m ahead of the cockpit origin. Housings now retain their approved proportions at each actual hardpoint depth, including closed receivers, stacked sleeves, braces, material details and reflections. Muzzle effects continue to originate at the same far-tip geometry. No hardpoints, aiming, damage, cooldowns, slot capacity or approved artwork were replaced.

306 focused native checks pass. The new regression buys an Anaconda through the actual shipyard, confirms four WPN slots, fires each of four fitted weapon identities, and uses Circle+Right to switch immediately through all four and wrap. It checks substantial receiver size and lower cockpit placement; all previous cockpit, fitting/save, cosmetics, menu/intro, departure and ERL checks pass. All-hull native captures are retained. Existing482 station adverts and eight-second departures are preserved. Source snapshot: design/guns483/final-source.zip and final-source-hashes.json.

Native before/after, approved reference and80-frame lossless actual retail firing footage: assets/generated/guns483/index.html. Footage has disk-writing overhead and is not an FPS benchmark. Separate native-resolution PPSSPP Anaconda timings, without compilers/recording: hit 38.31, 29.96, 29.91 FPS; switch 37.19, 30.05, 29.89 FPS; busy 28.95, 29.17, 29.02 FPS; station 30.24, 29.9, 29.45 FPS. Zero audio starvation and command drops. Busy combat and one station window miss the strict29.5FPS benchmark floor (roughly29FPS busy,29.45FPS station); this limitation is retained in validation.json rather than marked as a pass. The gun correction is delivered as a local playtest, not a completed performance milestone. Worst-frame timings, occasional spikes, physical PSP confirmation and pre-existing broader smoke failures remain open. PASS retail PSP memory budget: image=22847768 heap=12582912 main_stack=1048576 headroom=18046696 psp3000=True. Intro bytes unchanged; protected saves unchanged. Earlier482 package and original PSP stable distribution remain available. No Git repository metadata exists in this mirror, so no commit/push/tag or GitHub release is claimed. SHA-256: 510C41E99DFE69520AF49A4C6537054C32CEF7F4BA7663BA13F0988C8C13956F

Next: physically check the Anaconda cockpit housings, four fitted guns and far-tip firing on PSP-3000. Continue frame-time optimisation and outstanding encounter work without reducing approved artwork. Preserve the separation between weapon slot capacity and the visible paired Twin Stack housings.

2.5.482 local PSP-3000 station departure update: six custom 64x32 pixel-art advertisements (KOYO bowl, NOVA star/casino, VOX radio, DRIFT courier, AZURE helmet and ORBIT hotel) map to real wall/building planes with perspective-correct UV interpolation and near clipping. Exact background-paper masking preserves foreground rails, ship geometry and HUD. Lave's corridor uses DRIFT/VOX rather than another KOYO label; station profile varies the pair. Larger building adverts stay visible for about four seconds at Lave. Native proof: assets/generated/port482/index.html, 81 losslessly verified 480x272 frames spanning 8.1 seconds.

Normal station departure lasts eight seconds with slower motion near building adverts, then a small smooth lane curve starting at least 100m beyond the station hull. Position and camera use the same physical path; handoff is aligned at cruise speed with all staged scenery cleared. The first authored Earth ascent retains its original five seconds and straight path, preserving the existing city cutscene. Poor/rich/mega/relay geometry and traffic use the same transformed camera basis. Crossing/outbound/inbound background ships are deterministic decorative meshes, not new persistent simulated NPCs. The player's inbound docking cinematic is not redesigned.

302 targeted native checks pass:15 port art/timing/clearance,24 departure HUD/tracking,60 cockpit,32 fitting/save,78 dashboard,35 hanging charms,7 menu and51 ERL routes. All256 system departure profiles finish smoothly without fuel/hull loss; all256 clear staged scenery before handoff. Poster raster preserves foreground geometry and HUD; perspective and advert dwell are checked. Protected saves remain unchanged. Previous481 controls, cockpit, cosmetics and fitting are retained without art simplification. Intro embedded bytes remain identical.

Separate final retail timings: lave 42.99, 41.91, 29.9 FPS; capital 57.9, 32.23, 31.23 FPS; relay 57.25, 51.67, 35.19 FPS; cruise 36.79, 29.9, 29.89 FPS; boost 35.5, 59.64, 59.64 FPS. Runs use native-resolution PPSSPP PSP-3000, host muted, mixer/display enabled, private saves; no recording, compilation or compression during measurement. Launch windows include the actual eight-second departure followed by coasting. Zero audio starvation/command drops. Per-window worst frame times and strict29.5FPS floor status are recorded in validation.json; averages are not a locked-framerate guarantee. Physical PSP confirmation, frame-time spikes and pre-existing broader legacy smoke failures remain open. Memory: PASS retail PSP memory budget: image=22847064 heap=12582912 main_stack=1048576 headroom=18047400 psp3000=True. Original PSP stable distribution and481 package are preserved. No Git metadata or GitHub publication claim. SHA-256: F6D87574EA71F7B1918436CB103A8786F9CA8BC79C6DAFD0E49ED1B0FBCAF5D9

Next: physically test PSP-3000 station departures, including first story ascent, rich/capital/relay ads, held-button handoff and real save loading. Investigate remaining frame-time spikes without reducing approved artwork. Inbound player docking scenery remains a separate future improvement. Continue other encounter interaction/placement work from the existing audit.

2.5.481 local PSP-3000 station-departure HUD and R tracking fix: guided station launch (dock_stage4) uses the same modern cockpit overlay as ordinary flight, including rolling speed digits, Twin Stack housings, reflections and fitted hanging/dashboard cosmetics. Autopilot status sits at y40 below the top console. Presentation eligibility is separate from input eligibility: manual targeting/tools remain disabled during departure and the fresh-button-release gate. The ordinary flight overlay is factored into one shared function; no approved art was simplified. Dock-stage changes invalidate the cockpit cache immediately at entry/handoff. Hold Square and press R to lock the highlighted detected contact and turn toward it; X alone still locks without turning, and X+R remains supported. Computer input consumes R so it cannot also boost or accelerate while browsing. The R shoulder key is labelled TRACK in the target list.

287 focused native checks pass:24 launch/tracking,60 cockpit,32 quick fitting/save,78 dashboard,35 hanging charms,7 menu/intro and51 ERL routes. Launch is checked across poor/rich/capital/relay station views, all153 frame-sized steps per class, and actual menu Launch with button mashing through the five-second path and fresh-release guard. Native glyph checks confirm the modern target-console label throughout launch and handoff. Exact assets and installed cosmetics remain visible. R confirms the highlighted contact, enables autoaim and produces an actual yaw change; it leaves boost off and does not accelerate. Protected saves unchanged.

Native gallery assets/generated/departure481/index.html includes51 losslessly verified renderer frames over5.1seconds, launch start/end/free-flight stills and R tracking. This fixed-step fixture is not an FPS measurement. Separate final retail workloads: launch 49.95, 35.47, 29.89 FPS; cruise 36.79, 29.9, 29.89 FPS; boost 35.5, 59.64, 59.64 FPS. All runs have zero audio starvation/command drops. The launch workload executes the actual five-second departure followed by ordinary coasting; window timings include the handoff/free-flight period, rather than claiming a pure five-second port-only average. Physical PSP confirmation, occasional frame-time spikes, broader pre-existing smoke failures and commercial release gates remain open. Memory: PASS retail PSP memory budget: image=22791816 heap=12582912 main_stack=1048576 headroom=18102648 psp3000=True. V38 saves, intro embedded bytes and previous480 package retained. Original PSP stable distribution unchanged; only PSP-3000 local playtest promoted. No Git metadata, commit/push/tag or GitHub release claim. SHA-256: AA64AF00DDF57CFBB60447B74CABBD3A8EA3C895263334BF830AE4CB20D0B4A4

Highest-priority remaining work after481: confirm the station-exit HUD and R-to-track on physical PSP-3000, including first launch, relay/capital departures, held-button release and a real saved commander. Investigate existing frame-time spikes without reducing artwork. Retain the separation between hc_presentation_context (rendering) and hc_context (manual controls); do not enable input merely to show the HUD. Other specialised cinematic presentation paths were not redesigned in this fix. Continue the remaining encounter interaction/placement and art work from the source audit.

2.5.480 local PSP-3000 loadout/HUD playtest: X over a Ship Loadout slot opens right-side Item Storage filtered to that category. Up/Down browses stored hardware including exact enhanced instances; X fits into the selected bank, returning the old item to storage; Circle cancels. Triangle full storage and Square removal remain. Existing cargo, passenger, capacity and independent cosmetic-family restrictions retained. Hold Square opens targeting immediately; release closes even while Circle tools or Start power controls own input. Lock changes require confirmation; live flight continues. The native speed counter rolls four game-font digits every rendered frame through a 32x8 aperture, independently of cached console painting. Speed smoothing uses elapsed game time. Acquisition, panel-edge reveal, selected-tool/gun transitions, confirmation rings, receiver/Comms activity and local warnings animate without gating input. Console redraws are bounded to 15Hz plus immediate state/input updates; small effects run at render cadence. Approved artwork retained.

Equipment autosave now defers unsupported free-flight checkpoints instead of producing a repeated false failure warning; pending changes retry on the docked command deck. Genuine creation/write/seal/validation/replacement failures retain useful reasons, with previous valid-save backup behavior unchanged. Save validation and V38 serialization remain enabled and unchanged in format. This fixes a reproduced warning route; it does not claim recovery of an unseen human session. Save successfully at a station before replacing an actively running old build.

263 focused native checks pass: 32 quick-fitting/hold/save, 60 cockpit, 78 dashboard, 35 hanging charms, 7 menu/intro and 51 ERL route checks. Protected test saves unchanged. Native UI captures and a lossless 90-frame animation are in assets/generated/loadout480/index.html, alongside the supplied reference. Animation is a fixed-step scripted renderer demonstration; retail FPS is measured separately without recording or compilers. Final exact-retail measurements: cruise: 36.79, 29.90, 29.89 FPS; boost: 35.63, 59.64, 59.64 FPS; station: 30.33, 29.89, 29.60 FPS; computers: 34.08, 32.58, 30.43 FPS. Zero audio starvation/command drops across all four workloads. Boost uses real double-R input and remains active over 500 frames; its later windows are open-space flight. Station/expanded computers are staged native orbit workloads. See validation.json for worst-frame times and strict-floor status. Occasional frame-time spikes, physical PSP confirmation, long sessions and pre-existing broader smoke failures remain open. No locked FPS/commercial-release or GitHub-sync claim. Memory: PASS retail PSP memory budget: image=22803080 heap=12582912 main_stack=1048576 headroom=18091384 psp3000=True. Embedded intro byte-identical. Prior479 and original-model PSP stable distribution preserved. SHA-256: 5460460DEAEEE6E6F1C68D605354A656B642D3BB350AD235C170540B5F46586F

Highest-priority remaining work after480: test manual and deferred saves from a real commander on physical PSP-3000, including any specific new failure reason; verify hold/release targeting, quick fitting, native rolling digits and both cosmetic families. Investigate remaining frame-time spikes without reducing the approved assets. Broader pre-existing smoke failures and hardware release gates remain open. Continue encounter-art production from existing approved references.

2.5.479 local PSP-3000 cockpit cosmetics playtest: all four approved hanging charms and sixteen dashboard sprites use supplied pixel art directly. Lave stocks all twenty at a proposed25U each. Purchases enter existing Item Storage as normal module instances; fitting/removal/save paths reused. One hanging charm per ship uses Utility, and one dashboard cosmetic uses Hold/Cabin, independently across all banks/types. Multiple owned copies allowed; remove the current item before fitting another from its family. No rarity/power tier or stat bonuses. Violet/lilac names, distinct icons and full cosmetic labels in shop/storage/fitting. Hanging motion follows actual turns/acceleration/braking/impacts with frame-independent bounded springs; dashboards remain static at native(0,116). Comms icon/text moved10px right to clear the supplied shelf. CLOSE/Triangle and speaker tag now sit within a padded caption header, with all three body rows retained.78 dashboard,35 charm,60 cockpit,51 ERL and7 menu/intro native checks pass (231 total), including exact opaque sprite pixels, legacy/new saves, duplicate limits and expanded-computer occlusion. Ten exact-retail performance cases have zero audio starvation/command drops; both cosmetics draw in about0.7ms/frame. Music settles around29.9FPS; station averages29.28/29.75/29.37FPS, with two retained29.5 strict-floor failures. Memory budget passes with18,097,608bytes headroom. Original V38 save stream width115 retained; new ownership uses existing DockRoll records. Intro byte-identical. Native gallery assets/generated/cosmetics479/index.html; all4 native motion/reference comparisons in motion.html. Prior478 and PSP-original stable dist preserved. Physical PSP, long-session checks and existing broad legacy smoke failures remain open. No Git metadata; no commit/push/tag claim. SHA-256: C805500477E1C54CFE45474C493A925866005487D22B22B6BFED086035B69A8E

Highest-priority remaining work after479: confirm all20 cosmetics, collision/braking motion, shop/save flow and text readability on physical PSP-3000; review proposed25U pricing and Hold/Cabin category balance. Address disclosed station/frame-pacing limits without reducing approved art. Resolve the pre-existing broad-smoke failures before a commercial release gate. No existing mirror-sticker equipment was found; do not claim it is implemented or tested. Keep future cosmetic-family limits independent and preserve existing bezel stickers. Continue remaining encounter art under design/ENCOUNTER-ART-STANDARD.md without undoing the cockpit, intro, ERL or saves.

2.5.478 local PSP-3000 cockpit + G Twin Stack playtest: approved armoured computers, native font, full tool names, real slot dots, immediate Circle+Right fitted-gun cycling and explicit Square/D-pad/X contact confirmation. Both computers preserve live analog flight; R+X confirms and aligns without boosting. Twin housings retain stacked sleeves, closed receivers, panel detail, far-tip firing, recoil, glow and varied reflections at actual hull hardpoints. Native colour/depth caches and opaque HUD spans optimise drawing without an art-detail cut. All 23 hull mount views and 60 native control/layout/cache checks pass. Menu/intro 7 and ERL 51 checks pass with private saves unchanged; embedded intro is byte-identical. Exact-retail native benchmarks, strict frame-rate shortfalls and separate capture-timed APNG comparisons are recorded in assets/generated/cockpit-hud478. Rapid-fire sound-command overflow fixed using 384 extra bytes of bounded queue storage. Weapon balance and save layout retained. Physical PSP testing remains pending; separate broad legacy smoke is not all green. Prior 477 and PSP-original stable dist preserved. No Git metadata, commit, push or tag claim. SHA-256: 026555735FEA6461DB582EFCC2DB57AB1DC59B2741EF828CD6C07F1878E9210E

Highest-priority remaining work after 478: review the native side-by-side footage on PSP-3000 and confirm input, sound, long-session saves and suspend/resume on hardware; address disclosed heavy-combat/chase/high-contrast frame pacing without simplifying approved artwork. Resolve or modernise the recorded broad-smoke failures before a commercial release gate. Continue the remaining 58 encounter catalogue designs individually under design/ENCOUNTER-ART-STANDARD.md after this cockpit review; preserve 477 art, intro, ERL and saves. This is a local playtest, not an all-green commercial release.

2.5.477 local PSP-3000 encounter art reworks: the other nine rejected474/475 drafts now use purpose-built structures and individually composed major-surface pixel paintings, following the user-approved476 radio. Salvage village, shipbreaker, greenhouse, fuel stop, ferry, tea barge, ore depot, parts exchange and beacon repair crew replaced.650 new storage tiles from93 continuous paintings;951 total atlas tiles. All nine exact-retail near/medium/distant orbit checks pass at30.43–32.93FPS, audio starvation/drops0. Radio geometry/art byte-identical to476; its final orbit29.60–30.66FPS. Normal startup and memory budget pass (19,307,112bytes headroom). Older relay near29.28 and carrier near28.70 remain strict-floor failures, disclosed in validation. Occasional long frames and physical PSP verification remain open. Disjoint65536+2*tile+half material IDs avoid racing/fleet collisions; CPU/GPU paths and mega-ship showroom migrated together.260 host order/bounds cases pass and art regenerates byte-identically. Separate diagnostic build passes51 quick-race and7 menu/intro checks; private saves unchanged. Existing intro byte-identical; encounter placement, interactions and save format retained. These nine are galaxy catalogue types, not nine extra Lave locations. Lave has8 built encounters plus3 natural anomalies. Native gallery assets/generated/nine-encounter-reworks/index.html; package deliverables/ELITE-NEXT-2.5.477-NINE-REWORKS.zip. Prior476 retained; stable PSP-original dist remains separate. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: D043EA4DBE7499A21B596D0C7F396768CF84AC487C74C07DAC67AC966FF36329



2.5.476 local PSP-3000 radio art rework: user rejected all ten474/475 encounter previews as too basic. Rebuilt ONLY radio entry14 with separate continuous front/rear/side/roof paintings, rounded16-sector/7-band service pod, railed deck, braced mast and separate dish.186 new storage tiles;301 total atlas tiles. Fixed the exposed fixed-size180-material/Adder128-offset batch bug using disjoint dynamic ranges; sparse ascending material iteration preserves draw order.260 host boundary/order cases pass. Exact retail radio orbit30.42/31.14/29.75FPS, zero audio starvation/drops. Mechanic/salvage and normal startup pass; relay near29.11 and carrier near28.70 miss the29.5 strict floor and remain explicitly open. No memory faults in final five600-frame runs. Retail memory budget passes with23,437,432bytes headroom; physical PSP pending. Intro byte-identical; other nine geometry and old72 materials unchanged. Prior ten basic previews are not approved finished art; nine still need the same redesign. No new placement/services/save-format changes. Native gallery assets/generated/radio-shack-rework/index.html; package deliverables/ELITE-NEXT-2.5.476-RADIO-REWORK.zip. Failedv1 is excluded; previous475 remains recoverable. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: 01A28C631593CFD2B5D7F1E60F6B02B34465D1C25F5BBB0C38FD647ED1AFA453



2.5.475 local PSP-3000 encounter catalogue wave 2: Cooperative Ore Depot, Parts Exchange and Beacon Repair Crew now have individual geometry,12 fitted material plates and mechanical animation. All three exact-retail near/medium/distant orbit tests passed at32.14–34.85FPS, audio starvation/drops0; occasional50–67ms frames remain. Previous salvage village baseline and normal native startup pass on the same executable. Previous seven474 geometry and60 material plates are byte-identical; src/main.c and space-discoveries.h are unchanged from the474 source snapshot. Embedded intro is byte-identical. No new services, placement or save changes. 17/75 catalogue types now bespoke;58 remain. Natural anomalies are tracked separately. Physical PSP and broader stress testing pending. Package deliverables/ELITE-NEXT-2.5.475-ENCOUNTER-WAVE-2.zip; all ten new native previews assets/generated/encounter-upgrades/catalogue-progress.html. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: A0969A67E49FD52C28B8E10E08C253C95FCBDC3FD651848FDCB31A139016D717



2.5.474 local PSP-3000 encounter catalogue wave 1: seven new bespoke encounter designs (salvage village, shipbreaker yard, community greenhouse, patchwork fuel stop, old passenger ferry, local radio shack, miners tea barge), with 28 role-specific pixel-art plates, mapped geometry, real sunlight and bounded animation. All seven exact-retail near/medium/distant orbit checks passed, averages31.84–35.68FPS, audio starvation/drops0; occasional50–67ms frames remain. Normal native startup and retail memory budget passed; embedded intro film is byte-identical. Physical PSP, broader gameplay and crowded-view testing remain open. Existing placement, interactions, saves, menu, ERL and hyperspace source retained. Four-system encounter certification restriction remains; native gallery forces catalogue entries into a private fixture. 14/75 catalogue types now have bespoke upgrades;61 remain. Separate32-type natural-anomaly audit remains pending. Local playtest only; no Git metadata in this copy, no GitHub sync/tag claim. Package deliverables/ELITE-NEXT-2.5.474-ENCOUNTER-WAVE-1.zip; native gallery assets/generated/encounter-upgrades/catalogue-wave-1.html. SHA-256: D4D4D0BA711CA17791C7D7F5CB17DEE3150010C6215CE34479EAF739866BF4A9



2.5.468 local PSP-3000 boost onset: hyperspace streaks now grow outward and fade over0.85seconds with matching FOV easing; individual streaks fade at wrap boundaries. Ordinary speed motes integrate a continuous phase across double-R boost and use smooth colour/speed transitions, avoiding the previous absolute-time phase jump. Controls, acceleration, saves and existing audio unchanged. Exact retail startup and flight+MP3 checks pass silently; native transition recording/physical PSP remain unverified. Artifact milestones/2.5.468-boost-onset/EBOOT.PBP; source patch design/hyperwarp-current/playable468.patch. No locked60FPS or stable-release claim.



2.5.467 local playable PSP-3000 hyperwarp: approved electric charging, rainbow storm tunnel, six speed pulses and destination opening integrated into the full current game. Existing entry method, audio and all other game source preserved; rejected sound candidate excluded. Exact retail intro/home/flight+MP3 checks pass silently. Artifact milestones/2.5.467-hyperwarp/EBOOT.PBP; details design/hyperwarp-current/PLAYABLE-467.md. Physical PSP, exact-retail full-route input test and sustained frame pacing remain unverified; no locked60FPS/stable-release claim. Stable VERSION/dist retained.



Deep-drive audio study (15 October 2026): original charge/launch/transit/six-crest/arrival sounds now run from real route state in the isolated native hyperwarp candidate. 302,966 bytes resident16-bit PCM, no rejected defect-fan/noise layer; bounded voices and smooth cancellation/restart.85 portable-voice checks plus six actual-route cancellation cases, real route/guards, streamed MP3 and device continuity all pass. Successful sound starts verified: entry1/crests6/arrival1; max3 voices observed, no starvation/decoder errors, no clipped samples in captured device mix. Separate PPSSPP pacing: charge29.62FPS, launch29.97, transit56.77, arrival29.99;84.790ms transition spike remains. Muted-default native audiovisual gallery outputs/hyperwarp-journey-lab/audio-gallery/index.html. ALL future PC testing must remain muted unless user asks otherwise; keep PSP DSP enabled and host volumes0. Authored source/sounds/evidence protected in design/hyperwarp-current/HYPERWARP-AUDIO.md and AUDIO-VERIFICATION.json. Canonical source/playable466 EBOOT unchanged; production integration, listening approval and physical PSP checks pending. No stable-release/GitHub-sync claim.



2.5.466 local PSP-3000 playtest: 28 new natural anomaly types, 384 new sites across all256 systems, fixed balanced geographic quotas (20 regular/13 unusual/5 rare), live shaded crystal/ice/rubble and animated three-dimensional plasma/dust/light forms. Existing four types and slot0/1 locations retained. New sites use existing saved slots2/3; save/load byte-identical. Actual scanning/reports/Codex/rewards integrated; no repeat payment. Availability-aware remaining-signal counts and saved contact identities. Bounded distance detail, cached framebuffer, prepared integer glow and one-pass dust composition. Dedicated all-system save/placement checks pass; broader687 assertions retain identical18 existing failures. Native gallery and final test limits: outputs/releases/2.5.466-natural-anomalies; docs/NATURAL-ANOMALIES-466.md. Stable VERSION/dist retained; physical PSP and crowded-view FPS remain pending; no GitHub sync claim.



2.5.465 local PSP-3000 fleet/discovery detail playtest: all29 standard hulls receive fitted panel art/lighting/glazing/outlets with distance detail; approved Adder retained. All12 capital hulls textured and actual named capital NPCs now use their hull instead of an Anaconda proxy; three freight styles updated. Explicit75-entry discovery visual registry preserves existing8-slot/four-system selection, listening post and wreck; four rift types use world-space depth-checked light. Shared atlas, startup-resident textures, material batches, guarded512KiB command arena and preserved foreground geometry support native480x272 rendering. Saves/game.c/game.h/ships.c/audio/race rules/victory retained. Native hull/catalogue/UV/guards,24 exact projection comparisons, foreground/behind and Game-state checks pass. Exact retail station-flight music about32.86FPS vs33.37 baseline, no starvation/decoder errors; separate active-fleet+engine+traffic+MP3 close-four-ship test remains slow (~20/12/30FPS vs baseline30/15/30), with continuous audio. Existing18 gameplay-suite failures unchanged. Physical PSP, close-crowd FPS and final individual art approval remain open. Package/gallery/source/evidence: outputs/releases/2.5.465-fleet-and-discoveries; details docs/FLEET-AND-DISCOVERIES-465.md. Local playtest only; stable VERSION/dist retained and Git pull blocked by pre-existing untracked music placeholders; no GitHub/stable-release claim.



2.5.464 local PSP-3000 station-performance playtest: nearby visible stations use hardware panel shading with aligned/swizzled authored textures and padded mip levels; exact original CPU visibility/depth is retained for ships/lights/targeting. Near-only 7km/9km hysteresis avoids distant-view transfer regressions. Paired staged moving station draw cost improves27.118→23.462ms,54.229→45.104ms,36.542→27.158ms (~13–26%);72 matched moving poses keep exact depth/guards. Exact-retail supplied-MP3 flight improves29.76→33.27FPS versus previous463 under the same current test; zero starvation/decoder errors and378 actual hardware passes. Engine-only passes33.80FPS; startup passes31,348,480bytes minimum kernel headroom. Native all28 station layouts/256-system roles, six scene depth checks,401,808 material comparisons and real streamed-MP3 jump route pass. This station optimisation changes main.c, station-authored-material.h, mega-city-render.h and new station-ge.h. Concurrent ERL-preview-memory/listening-post/derelict-art changes are preserved in the exact tested retail EBOOT and recorded separately; staged fixture predates the last parallel derelict-art addition. Gameplay/saves/audio/station assets/menus/ERL/victory retained. Package/EBOOT/native comparison gallery: outputs/releases/2.5.464-station-performance. Physical PSP timing/listening, exhaustive switching/sleep/docking, entry/arrival hitch and broader baseline release gate remain open. Stable VERSION/dist452 and USB files retained; no GitHub release/sync claim.



2.5.463 local PSP-3000 hyperdrive playtest: hardware-rendered Ion tunnel/warp sky/glow, cached native framebuffer work and smaller audio output bursts. Removed HYPERSPACE / SLIPSTREAM banner; charging box reduced38→24px, positioned162..185. Exact-retail startup and MP3 checks pass (29.76FPS, zero buffer starvation/decoder errors); engine-only FPS gate fails at26.29. Previous462 also fails the same comparison at26.58FPS. Exact-retail OGG continuity passes at21.59FPS with zero starvation/decoder errors. Corrected native scripted route profiler improves27.68→44.18FPS overall; settled tunnel approximately60FPS, early charge/arrival near30, arrival hitch~68ms remains. User accepts working non-locked60 playtest. Package/EBOOT/native gallery/evidence: outputs/releases/2.5.463-hyperdrive-performance. No source changes outside six selected renderer/audio files vs task-start snapshot; saves/progression/menus/art/ERL/victory retained. Earlier460/461 combined hyperspace/music harnesses muted radio with comms_quiet=1; their music-labelled timings are rendering diagnostics only, not concurrent playback proof. Exact-retail music checks remain valid; corrected463 harness asserts source/format/device activity. Physical PSP, sustained approach/arrival pacing and broader baseline release gate remain open. Stable VERSION/dist452 and USB files retained; no GitHub release/sync claim.



2.5.462 local PSP-3000 station texture TRIAL: perspective-span footprint filtering and a fixed65,472-byte lit RGB mip cache reduce panel shimmer; full-detail pixels remain nearby. Native matched motion uses the actual renderer:72 views retain identical geometry/depth and framebuffer guards; all28 layouts/256-system role coverage pass. Whole-viewport temporal change falls~30-50% on three staged camera paths. Final staged render30.369ms baseline/36.631ms filtered: material cost remains, not a free FPS optimisation. Exact retail emulator boot and MP3 busy flight pass (30.34FPS, worst83.42ms, no buffer starvation/decoder errors). EBOOT/gallery/source patch/evidence: outputs/releases/2.5.462-station-texture-trial. Main/audio/hyperdrive/sky source files match461 byte-for-byte. Original2x2 station raster and silhouette jaggies remain; no full anti-aliasing claim. Priority: optimise minification cost, improve silhouette coverage, inspect larger close station paths and test physical PSP. Stable VERSION/dist452 retained; USB EBOOT/saves/music unchanged this turn. GitHub DNS still unavailable; no stable promotion, commit/push or sync claim.



2.5.461 local PSP-3000 audio/hyperdrive hotfix: corrected cockpit mixer overflow, reduced fuzzy layers while preserving recorded engines, bounded peak control and faster exact PCM interpolation. Removed 2x2 warp sampling, skips hidden world rendering and prepares arrival sky incrementally. Tested PSP-3000 profile requests larger memory; RAM alone does not produce a large FPS gain. Exact retail engine/MP3 tests pass at 30.42/30.48 FPS, zero buffer starvation/decoder errors; short frame spikes remain. Fourteen authorised USB music tracks converted to native-rate MP3 with verified PC backups; actual battle track improved from 21.45 to 30.64 FPS in emulator, Sol Rock essentially unchanged. Package: outputs/releases/2.5.461-psp3000-audio-hyperdrive. Original music: outputs/music-originals-psp3000-461. USB EBOOT/saves unchanged. Physical PSP listening/frame pacing and broader baseline release acceptance remain open. Stable VERSION/dist452 retained; remote DNS unavailable, no sync/release claim.



2.5.460 local hyperspace playtest: approved Ion Cathedral now renders in game with extra speed lines and electrical buildup. Target a system, align and boost to fill the existing meter as before. EBOOT/native captures/evidence: outputs/releases/2.5.460-ion-cathedral. Original-model emulator checks pass; destination-sky hitch and physical PSP acceptance remain open. Stable dist remains452 pending the broader release gate.



2.5.452 local audio/station hotfix: bounded PCM playback queue prevents the reproduced OGG gaps; boost/chase-camera station surface mapping is stable. Latest lighting, saves and shared work retained. Physical PSP and further station FPS improvements remain open. Local tested package/evidence: outputs/releases/2.5.452-audio-hotfix.



2.5.416 PLAYTEST checkpoint: first round ERL circuit has a quarter-lap magnetic sector, real booster launch/gap/landing, and retained menus/V37 progression/victory/Picross. 380 ERL checks and five exact-retail startup/MP3 checks pass. Full-grid all-hull circuits take about 131–135 seconds. Retail magnetic/jump test: 40.93 FPS, worst 50.05 ms, zero >100 ms frames, six completed saves, Sol Rock; 55 FPS target still fails. Physical PSP, full Apex acceptance and remaining handoffs are unfinished. Playtest EBOOT: outputs/releases/2.5.416-preview/ELITE-NEXT-PSP/EBOOT.PBP. Latest stable remains 2.5.415. GitHub unchanged.



2.5.415 playable local checkpoint: Mission Board sponsor rewards now deliver the exact advertised enhanced equipment, or the disclosed full-queue cash alternative. Final board results are saved before Continue closes them, including when an older checkpoint is writing. 149 production integration and 39 real-control flight/save checks pass; full ERL/native regressions pass. Exact retail: 57.58 FPS, zero >100ms frames, four completed saves, startup/MP3 verified on original 32 MiB emulator model. Menus/visuals remain 414 pending the user's finalized side-chat handoff. Physical PSP pending; full arcade acceptance remains open; GitHub unchanged.



2.5.414 playable local checkpoint: expanded ERL contracts, competitive green-ring AI, eliminator final sprint, V37 career records, exact-once rewards, independent seasons, factual LocalTV and sponsor equipment. 137 integration checks and 147 full-console checks pass; native HUD/track/flight/victory/TV/progress suites pass. Exact retail 57.58 FPS, no >100 ms frames, startup/MP3/four saves verified in original 32 MiB emulator model. Broader core retains the same 15 baseline failures; not an all-green release. Full handoff visual acceptance remains open. Physical PSP pending; GitHub unchanged.



2.5.413: pink holographic racing track, compact animated position/lap/live HUD and subtle rival damage cues. 15 native floor/HUD checks and 44 gameplay checks pass. Exact retail: 55.26 FPS populated countdown/race test, worst 33.37ms, no frames over 100ms, four completed saves, safe worker stack; five startup/MP3 checks pass on original 32 MiB emulator model. Physical PSP confirmation pending. Full arcade handoff remains unfinished; GitHub unchanged.



2.5.412: ERL League opens reliably with an accepted pending race. Keep HQ browsing/animations and the exact accepted course; docked pending brief sets the beacon rather than starting the race. 26 native pending-menu/sleep regression checks and five retail HQ startup/music checks pass. Native HQ screenshot inspected. Preserves intro X/logo fix, ERL sleep fixes, Shockwave and Picross. Local release outputs/releases/2.5.412; physical PSP confirmation pending; GitHub unchanged.



2.5.411: X reveals the ELITE: NEXT logo together with the menu at any point in the intro. The film and audio continue without seeking. Preserves 2.5.410 ERL flight/sleep fixes, victory and Picross. 33 native intro checks and five exact-retail startup/music checks pass; native captures inspected. Local release outputs/releases/2.5.411; physical PSP confirmation pending. GitHub unchanged.



2.5.410: Accepted ERL events keep ordinary flight HUD/radio until READY. Sleep/resume only pauses active countdown/running races. Recover old pre-start pause/resume flags without cancelling contracts; active countdown Resume consumes X without skipping the countdown. Save format unchanged. 20 native flight-state regression checks and retail boot/countdown/music checks pass; native captures inspected. 59.94 FPS in PPSSPP. Physical PSP sleep confirmation pending. Local release outputs/releases/2.5.410; GitHub unchanged. Preserve 2.5.409 victory and Picross.



2.5.409 verification complete: 23 focused victory checks, five retail startup checks, countdown/MP3/four saves and victory performance all pass. Victory 56.80 FPS; populated racing 59.94 FPS in PPSSPP original 32 MiB model. Renderer mean 10.02ms. Native 480x272 captures inspected. Frozen local EBOOT/evidence: outputs/releases/2.5.409. Physical PSP pending; GitHub unchanged.



2.5.409 ERL Shockwave Flyby replaces the checkerboard victory animation on top of 2.5.408. Native real-time camera, stars, numeral fragments, actual hull/paint/engine roots, sunlit finish and real reward strip. Save format remains V36, with V35 migration retained. Full arcade ZIP and newspaper expansion remain paused. Validation and frozen release evidence: docs/ERL-SHOCKWAVE-IMPLEMENTATION.md and outputs/releases/2.5.409. Physical PSP verification remains pending; local release, GitHub unchanged on the shared unfinished tree.



2.5.408 playable combat playtest: outputs/releases/2.5.408. Exact retail startup/countdown/MP3/autosave checks pass (59.94 FPS emulator, worst 19.05ms). 44 domain checks pass; complete real steering/item-input race 140.55 seconds; native HUD/tunnel/wall/replay captures verified. Full ZIP unfinished, wider work paused for player feedback. V36 reads V35; back up before migration. Physical PSP confirmation pending; GitHub unchanged.



2.5.407: ERL branding is fixed magenta/pink, deep purple and cyan in every system, including conversations. Actual ship paint remains on Garage previews. Built from the tested 2.5.406 source; unfinished arcade expansion is excluded from this hotfix.



2.5.406 verified locally: 281 focused checks pass. Tested EBOOT and evidence are in outputs/releases/2.5.406. Countdown hotfix: ERL autosaves now use a 256 KiB worker stack. Complete V35 save validation exceeds 100 KiB; the previous 64 KiB stack could corrupt PSP memory during countdown. Current ERL designs, progression and V35 saves are preserved. Arcade expansion is paused for this hotfix; physical PSP confirmation remains pending.



2.5.405 verified locally: 279 focused checks pass (ERL 142, conversations 44, racing 86, retail startup 5, retail performance 2). Live native motion phases and film captures inspected. Tested EBOOT, source snapshot and evidence: outputs/releases/2.5.405. Highest-priority remaining ERL work: physical PSP-2000/ARK playtest of the combined motion, audio and racing. Newspaper expansion stays paused. GitHub is unchanged on the shared unfinished tree.



Local 2.5.405: ERL combines 120ms Broadcast Neon moving focus rims, interruptible 220ms Slipstream page transitions, aligned Paddock Signal portrait reveals, once-only XP fills, sparse star shimmer, Season route lights and Garage platform lights. Inputs commit immediately; no action queue, save-format or race-rule change. Authored victory reveal is retained. Physical PSP testing pending.



2.5.404 verified locally: 261 focused checks pass (ERL 124, conversations 44, racing 86, retail boot 5, retail performance 2). Native Events and all six Rivals captures inspected. EBOOT and frozen evidence: outputs/releases/2.5.404. Physical PSP testing pending; GitHub unchanged on the shared unfinished tree.



Local 2.5.404: Events and Rivals match the approved native screenshot layouts, with a large qualifier XP readout/progress bar, diagonal hero panels and a 3x2 pilot grid. Existing controls, other ERL pages, race rules and V35 saves are retained. Physical PSP testing pending.



Latest local 2.5.403: approved ERL visual race cards and headquarters panels, palette-region icons, separated rewards/XP and no redundant control legends. Native layouts follow outputs/erl-readable402. Saves and race rules unchanged; newspaper work stays paused.



Local release 2.5.402: Hyper Circuit ERL redesign with segmented XP, connected Season route, illuminated Garage, selected-pilot Rivals and matching result/conversation chrome. Preserve saves and music when replacing EBOOT.PBP.



Local update 2.5.401: stable Introduction to the ERL mission title, completion after rookie qualification, and cleaner conversation paging.



# Local release 2.5.400



ERL racing uses background checkpoints, green boost rings and a revised introduction/referral flow. Lave’s featured contact is first on the board; Circle exits the mission, chart and system screens to the Command Deck. Existing 2.5.399 newspapers and V35 saves are preserved. See `docs/ERL.md` and `outputs/releases/2.5.400/BUILD-2.5.400.md`. Physical PSP playtesting remains pending; GitHub is unchanged.



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



The new native 480×272 shipyards are implemented on the current 2.5.355 build. Browse real stock with Up/Down; Square switches stats and the full description; X exchanges using the existing validations and congratulations card; Circle returns. Mega dealers have individual names, orbital showroom artwork and distinct four-hull collections. Lave retains the full test catalogue. The compact EXCHANGE YOUR SHIP box shows COSTS: or YOU RECEIVE:, and comparison arrows identify stronger/weaker bare-hull stats. Controls, Cargo, Loadout, separate Item Storage and V32 saves remain compatible.



See docs/SHIPYARDS-356.md and BUILD-2.5.356.txt for implementation, verification, hash and limitations. Physical PSP testing and inherited broad-check failures remain pending. Local release only; preserve mixed shared changes.



## 2.5.355 — Illustrated Controls handbook



Implements approved concept 3, the Pilot's Field Guide, directly at 480×272 with the real 8×8 game font. Five topics list forty controls on the left; selected native diagrams animate on the right with short instructions. Boost reads “Double press R + keep it held.” Up/Down chooses a control; L/R shoulders (or Left/Right) change topic; Square toggles analog steering; Circle returns to the calling menu. Gameplay controls and V32 saves remain unchanged. Existing Cargo, Circuit Console Loadout and separate Item Storage are retained.



Current local EBOOT and dist copy are updated at the user's request. Focused review: 18 checks pass and all forty native entries are captured. See docs/CONTROLS-HANDBOOK.md and BUILD-2.5.355.txt for final hash, core comparison and limitations. Physical PSP testing remains pending; this does not certify the full game. No GitHub publication; preserve the mixed shared checkout.



## 2.5.354 — Circuit Console loadout and Item Storage



The approved native 480×272 Circuit Console is now implemented on top of 2.5.353, preserving the Cargo/exchange update. Six-category cartridge banks support all eight positions, with real fitted states, active lamps, complete selected module effects and a prominent storage card. Separate Item Storage shows real spare quantities and matching banks, with a clean detail strip. L/R shoulders switch loadout categories. Existing fitting, storing, Outfitting resale and V32 saves remain compatible. See docs/LOADOUT-CONSOLE-354.md and BUILD-2.5.354.txt for controls, coordinates, test scope and remaining limitations.



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



## 2.5.348 - Informational wanted posters



Removed the GalacticNet Wanted poster X action and both tracking prompts (on the poster and in the footer). Open warrants now consistently say LIVE WARRANT. Browsing posters no longer launches a docked commander or alters a flight target. Names, portraits, bounty rewards, risk and taken-down status remain available; identify suspects through normal local targeting and hailing.



## 2.5.347 - Raiders react to combat



Direct player hits now interrupt pirate patrols, including wanted-poster raiders in peaceful Lave. An engaged raider prioritises its attacker within 12 km rather than switching to a closer trader. Healthy raiders turn back and return fire, weave on approach and complete close attack passes before circling back. Badly damaged raiders or those facing two nearby patrols retreat from their actual threats at escape speed. A lone nearby patrol no longer constantly cancels retaliation. Untouched Lave pirates remain peaceful toward the player. Existing bounty claims and police-assist credit remain intact. Civilian defensive aim no longer gets replaced by an unrelated pirate-avoidance waypoint. No save or network packet layout change.



## 2.5.346 - GalacticNet market neon sign



Removed the pink Left Sell / Right Buy sentence from the GalacticNet Market page. LIVE TERMINAL is now a slightly larger native pixel neon sign with a soft pink glow, slow brightness drift and brief character/scanline glitches. It occupies its own strip below the ticker rows and above the footer. Undocked access retains Dock for live trading; prices and trade controls are unchanged.



## 2.5.345 - A personal shipyard congratulations card



Successful shipyard purchases now open a warm delivery card bearing the new hull name, native-size animated handwriting, restrained pixel fireworks and a shipyard seal. Six messages vary without immediate repeats, including "Enjoy your new ride!". X finishes the handwriting, then continues; Circle closes the card. Purchase input is guarded and the card blocks underlying shipyard actions, so it cannot buy twice. Failed or already-owned exchanges show no celebration. Credits, cargo, fitted modules and Item Storage retain the existing exchange rules; no save change.



## 2.5.344 - Local Guild contract guidance



Tamsin's food-delivery and the Guild rescue assignments now use the real five-offer board catalogue. While each assignment is active, a missing required contract replaces only the final board offer, preserving five distinct choices. Already docked in the right system, Next Step opens and highlights the local contract; in local flight it guides docking, and an accepted contract leads to its mission-log entry. Existing Guild progress needs no save migration.



## 2.5.343 - Readable mission acceptance



Mission acceptance shows complete mission names wrapped and centred inside the confirmation card. YES/NO labels and the instructions below them are centred; destination text has its own clear row. Mission acceptance actions are unchanged.



## 2.5.342 - Cleaner outfitting prices



Removed the delivery, fit-later and loadout disclaimer lines from the module card. Cost now sits directly above Balance; Cost is teal (red when unaffordable), while Balance remains gold. Purchase and storage behaviour is unchanged.



## 2.5.341 - Compact planetary spaceports and EVA rendering



Planetary spaceports now use an 840m-wide apron instead of roughly 1.4km. Services, low terraces, the terminal and rover garage are grouped closer together, with fewer apron lamps and shorter roads. The shared geometry drives rendering, collision, landing reservations and the local map. The player pad retains clearance for the widest and longest hulls; two staggered traffic berths remain.



EVA retains the exact existing terrain mesh and 2x2 depth raster. Ground grain beyond 24m shares shading on a 4x4 screen grid; nearby props, signs, portraits and HUD retain their native detail. Distant dry ground avoids unnecessary texture reconstruction. Lave vegetation reuses tagged cells when moving between overlapping windows rather than regenerating the entire region. Quad submission transforms four vertices rather than six.



Verification and timing evidence are recorded in docs/EVA-PERFORMANCE-341.md. Physical PSP frame rate remains to be measured. This is a local private candidate; it is not a GitHub release. Existing broader regression failures remain open.



## 2.5.340 - Rear chase camera



Third person now uses a centered racing-style rear view, scaled to the hull. Full rolls keep the camera aligned with the ship. Exhaust and gunfire use hull surface anchors and shared depth tests. Capital ships use the same authored geometry as their shipyard preview.



## 2.5.338 - Clear empty-bank guidance



Empty banks in Ship Loadout now use the concise instruction “FIT AN ITEM FROM STORAGE >>”.



## 2.5.337 - Exterior camera stability



Third-person and cinematic views now share one camera basis with planets, stars and the nebula. Camera smoothing is frame-rate independent, exterior views keep looking at the ship through rapid turns and repeated mode changes, and speed streaks follow the ship's actual screen position.



## 2.5.336 - Two-step Item Storage fitting



Item Storage now uses a clear two-step interaction: select a stored module, then select one of the current ship's available banks. Outfitting uses the shorter “Sell Menu” label, no longer links to Ship Loadout with Triangle, and removes the duplicated instruction line above the footer.



## 2.5.335 - Outfitting storage shop flow



Outfitting X now buys modules into Item Storage without changing the fitted loadout. Square opens the outfitter's dedicated resale screen with stored quantities and station buy prices. Selling has been removed from Ship Loadout and the normal Item Storage fitting screen.



## 2.5.334 - Store fitted modules with Square



On Ship Loadout, Square moves the highlighted fitted module into Item Storage. Storage limits, occupied passenger cabins and cargo capacity are validated before anything is removed.



## 2.5.333 - Eight-bank Ship Loadout restored



Ship Loadout again gives every module category a full-width row capable of showing all eight banks. Selected-module details now sit below the board, and the prominent Triangle Item Storage card and separate Item Storage screen remain available.



## 2.5.332 - Shipyard description and stats tabs



Press Square in the Shipyard to switch the lower panel between an authored hull description and real gameplay statistics. The view works for every standard and mega ship while preserving the selected hull.



## 2.5.331 - Item Storage action card



Ship Loadout now presents Item Storage as a large Triangle action card with a live used/capacity count and fill bar, making the spare-module system easy to find.

## 2.5.330 - Hull-specific Item Storage



Item Storage capacity is based on the current ship's module-bank space, from 12 spare items on compact hulls up to 40 on mega ships. Installed equipment stays in Ship Loadout; Item Storage shows only genuine spare copies.

## 2.5.329 - Ship Loadout and Storage



Ship Loadout retains its fitted-bank board and installed module details. Press Triangle there to open the separate Storage screen. Storage contains only unfitted spare modules, supports up to 40 saved items, and shows every compatible bank for the highlighted spare. Buy spare modules with Square in Outfitting, including extra copies of equipment already installed.

## 2.5.328 - Exterior camera combat polish



Third person now uses a close low rear-quarter camera, follows boost without lag, draws twin speed-sensitive engine flames, and fires from hardpoints calculated from the current 3D hull. Distant stars remain fixed under camera translation. The complete flight HUD, radio dialogue and held-Square targeting computer now render cleanly over exterior views.



## 2.5.327 - Exterior and cinematic flight cameras



The Comms Panel Camera control now cycles through First, Third and Cinematic. Third person is a real rear exterior camera following the current ship. Cinematic mode remains live and changes among twelve composed exterior views every ten seconds, smoothly showing the current hull, paintwork, engine glow and surrounding system. The preference is saved. Focused native PSP checks pass camera framing, shot changes and persistence.



## 2.5.326 - Animated Discovery system dossiers



- Replaced the sparse Discovery galaxy locator with an adaptive system dossier that shows every generated sun and planet on slowly animated elliptical orbits, plus the system hub.

- Added a real System Record percentage derived from landings, field records, logged space signals and identified bounty targets, with a concise Next action leading toward completion.

- Added ten visible milestone markers. Triangle claims available rewards, pays once, and persists the claimed tier in the existing commander save data.

- Added a dramatic reward card that accelerates in from the right, settles at screen centre, bursts deterministic sparkles and reveals its message and reward text in stages.

- Preserved the 2.5.324 System Directory structure, undiscovered-world gating, quick facts and station sprites.



Verification: PSP build succeeds. Focused native 480x272 review passes dossier progress, one-time 20% payout, save/load persistence, X dismissal, slide, sparkle and completed-card captures. The broad source regression currently stops later on three inherited equipment-damage assertions outside this change.

## 2.5.325 - Lush seamless planet art



Planet previews now use naturally broken polar shelves instead of solid top and bottom bands that resembled clipped textures. A richer final colour grade keeps ocean blues, vegetation, mineral terrain, cloud cover and gas-giant bands clear and exciting in native 480×272 views while retaining each world's seeded visual identity.



## 2.5.324 - Honest system Discovery directories



System directories now separate charted stars and Space Signals from a labelled Planets section containing every local world. Unlanded planets remain marked Undiscovered and withhold flora, fauna, minerals and sites until first landing. Hovered records show useful quick facts; opened station, star and landed-world records repeat those facts beneath their description. Orbital stations use their authored class-specific sprite instead of the old plus badge.



## 2.5.323 - Docking-ready Comms control



The Comms Panel's Request Docking control now softly pulses green whenever the live station-range and docking-safety checks allow it to be used. Its orange navigation highlight remains visually distinct, while unavailable states retain the powered-down No Signal presentation.



## 2.5.322 - Safe quit confirmation



Quit to main menu now opens a dedicated confirmation panel, defaults to No, and reports the active commander's real checkpoint age in plain language such as `LAST SAVED 7 MINUTES AGO`. D-pad selects Yes or No, X confirms and Circle cancels.



## 2.5.321 - Working character creator



Head shape, skin or shell, hair or crest, eyes, mouth, ears and face variants now change the displayed portrait immediately. Character faces use clean authored pixel art rather than generic feature blocks that could land outside the intended face, eliminating the stray facial artifacts seen in the editor and small dialogue portraits. NPC portrait seeds use the same repaired system, increasing visible character variety throughout the game.



## 2.5.320 - Human mission guidance



Mission Desk briefings now speak naturally, Current Objective is visually connected to navigation, Mission Log uses Square for the tracked next step, and accepted contracts no longer add a redundant pop-up over the board.



## 2.5.319 - Readable cargo terminal



The Cargo screen now separates held quantity, shop stock and price comparisons clearly, keeps capacity beside its bar, and shows the commander's units in the standard bottom-right box.



## 2.5.318 - Faster Command Deck navigation



L/R now tabs directly between Fly, Ship, Work, Discover and Commander. Menu actions no longer cover the footer with a redundant animation showing the button just pressed.



## 2.5.317 - Mega livery integration



PIMP-MY-SHIP.NET now recolours and themes every standard and Mega hull consistently. Existing paint files migrate automatically, Mega previews use the fitted finish across ship screens, and their tapered armoured forms give them sleeker spacecraft silhouettes.



## 2.5.316 - Complete Lave outfitting and Loadout controls



- Ship Loadout now uses the L/R shoulder buttons to change WPN, DEF, NAV, HOLD, FUEL and UTIL categories; the D-pad stays within the selected category's banks.

- Lave Outfitting now exposes every purchasable module in the complete catalogue for certification testing.

- Every stock item and service uses a native-size category-coloured icon, including the original equipment catalogue.

- The detail panel suppresses repeated copy when the description duplicates the displayed stat effect.



## 2.5.315 - Visible console and flight animation polish



- Page changes now use a readable CRT frame-opening and scan-band sweep instead of an easily missed one-pixel flash.

- Button presses produce an expanding two-beat footer response, while selected controls carry a restrained moving hardware cursor.

- Normal flight animates the instrument bank with a drive sweep; boost adds a stronger amber vibration response without covering the view.

- All feedback remains presentation-only and non-blocking at PSP native 480x272.



## 2.5.314 - Mega Shipyard and technology expansion



- Adds twelve purchasable Mega hulls with fixed prices, genuine differences in handling, defence, heat, fuel, range, cargo and six equipment-bank capacities.

- Mega Capitals carry deterministic regional catalogues; Lave exposes the complete catalogue for development testing while still charging full prices.

- Expands fitted equipment from 24 to 48 slots with V29 migration from older saves, eight readable banks per category and safe one-copy/module-transfer rules.

- Adds all 43 expansion modules, unique native-size category-coloured icons, and connects their propulsion, handling, weapon, defence, heat, sensor, cargo, salvage, repair, fuel and hyperdrive effects to runtime systems.

- Adds twelve distinct solid capital-hull previews, bare-hull comparisons, rare named Mega traffic, long-lived capital wrecks and shuttle-based planetary access that preserves the parent ship in orbit.

- Adds native 480x272 Mega Shipyard, eight-bank Loadout and technology-icon review fixtures.



## 2.5.313 - Reactive flight instruments



- Added eased speed, shield, hull, heat and fuel instrument motion without altering simulation values.

- Boost, hard braking, high heat, fuel use, shield impacts and hull damage now produce distinct short cockpit responses.

- Target brackets converge on acquisition, break apart when selection changes, and gain close-station guidance marks.

- Selected radar contacts pulse, planet clearance panels settle into place, camera changes use a short scan transition, and arrival relights the instrument banks in sequence.

- Mission updates receive an amber-to-green underline sweep while keeping their text fixed and readable.

- Existing radio-host expressions, weapon-change ticker, speed lines, engine plumes and multi-stage vortex hyperspace effects remain integrated with the new layer.

- All animation state is presentation-only, fixed-cost and excluded from saves and gameplay physics.



## 2.5.312 - Physical menu feedback



- Face-button actions across non-flight screens now produce a short two-beat hardware-style pulse showing the button that was pressed.

- Page changes use a restrained 160 ms console-border sweep so screens no longer snap between states without feedback.

- Mission cards physically depress by one pixel when selected, while Yes/No confirmation buttons pulse green before the result appears.

- Feedback is drawn over the existing PSP-native interface without scrolling, delaying controls, or permitting an action to execute twice.



## 2.5.311 - Mission acceptance confirmation



- Pressing X on a new mission now opens a clear Yes/No confirmation in the contract panel before the mission is accepted.

- Active offers display a centred green status marker beneath their destination.

- Pressing X on an active offer now opens and focuses that contract in the Mission Log; the mission-board footer explains this action.



## 2.5.310 - Clearer contract details



- Moved the Objective and Requirement sections upward in the selected mission brief.

- Objective text can now use six lines and Requirement text can use three, providing more room for clear instructions without crowding the reward or footer.



## 2.5.309 - Cleaner mission offer cards



- Replaces redundant origin-to-destination routes such as `LAVE > DISO` with the direct location label `IN DISO`.

- Removes repeated unit rewards from all five offer cards; payout remains visible once in the selected contract's dedicated Reward section.



Verification: PSP build and native Mission Board capture pass at 480x272.



## 2.5.308 - Mission detail text fit



- Constrains Mission Board detail titles, destinations and optional-bonus labels to the right column's actual native-resolution character width.

- Rewrites every optional bonus into concise natural copy of nineteen characters or fewer, preventing the final word or letter from being clipped by the panel edge.

- Replaces flavour prose in the Objective section with direct plain-English instructions describing what the player must do and where.

- Removes the duplicate X Accept prompt from inside the offer panel; the shared footer remains the single action guide.



Verification: PSP build and native Mission Board capture pass at 480x272.



## 2.5.307 - Mission Board contract console



- Rebuilds the Mission Board to match the approved native PSP layout: five fixed contract cards on the left and one focused contract brief on the right.

- Each card shows a purpose-built pixel icon, concise family name, destination, colour-coded difficulty and payout without scrolling or panel reflow.

- The focused brief separates objective, requirement, optional bonus and reward with clear hardware-console section bars.

- Long contracts receive an explicit red label; accepted work and a full Mission Log receive bounded status treatments without covering another card.

- Removes portraits and redundant instructional copy from the board, using the compact `UP/DOWN`, PSP X and Circle footer pattern.

- Adds a dedicated 480x272 Mission Board capture and five-card layout check.



Verification: PSP build and clean native-resolution board capture pass.



## 2.5.306 - Seeded planetary themes



- Extends procedural planet identity with 27 coherent visual themes: eight ocean climates, ten rocky/mineral environments and nine gas-giant atmospheres.

- Seeds now control broad world character as well as noise, including azure and emerald garden worlds, ice oceans, violet cloud worlds, rust seas, volcanic fault worlds, frozen rocks, crystalline shelves, toxic haze, storm giants and unusual coloured atmospheres.

- Varies continent coverage, polar extent, cloud density, gas-band frequency and the number and position of giant storms independently.

- Retains type-aware colour discipline and sphere lighting so unusual worlds remain polished planetary art rather than random colour noise.



Verification: PSP build and native crowded-system review completed at 480x272.



## 2.5.305 - Procedural custom-quality planets



- Replaces the eight visibly repeated planet sheets with a deterministic native-resolution planet painter shared by flight, System Details, maps, Codex and menu previews.

- Generates warped multi-scale continents, coastlines, polar caps and cloud systems for ocean worlds; mineral terrain and individually placed crater fields for rocky worlds; and atmospheric bands plus storm cells for gas giants.

- Applies restrained type-aware per-world colour grading so bodies sharing a climate family no longer appear as palette twins.

- Preserves stable world identity: the same immutable body seed produces the same art everywhere and across save/load, without storing textures or increasing the save format.

- Keeps lighting, limb shading and pixel alignment designed for the planets' actual small PSP display sizes.

- Adds a procedural-identity regression sampling 64 worlds and requiring at least 62 unique rendered fingerprints.



Verification: PSP build completes; the crowded ten-body System Details review passes at native 480x272 with full labels and distinct generated worlds.



## 2.5.304 - Expanded Mission Network foundation



- Expands the repeatable Mission Board from five names to the confirmed pool of 26 families, including convoy, repair, salvage, wildlife, archaeology, mining, distress, patrol, recovery, missing-person, procurement, mechanic, black-box, planetary, listening-post, police, passenger, memorial, logistics, freighter-defence and derelict work.

- Keeps exactly five distinct, stable local offers and varies them deterministically by system and station, avoiding duplicate families on one board without rerolling during a visit.

- Routes every displayed offer through the canonical destination, station, route and landable-body checks before it can be accepted.

- Preserves each expanded family identity in the existing mission save record while routing it through a proven cargo, hostile, survey, contact or covert objective runtime.

- Adds family-specific titles, natural briefs, requirements, difficulty labels and reward adjustments to the native 480x272 Mission Board.

- Updates targeting, docking, cargo reservation, mission tracking, abandonment and completion so expanded families resolve and pay exactly once through their validated base objective.

- Extends the galaxy-wide regression to prove all 26 families occur, all 1,280 displayed offers are reachable and acceptable, and every base objective completes without duplicate payment.



Specialised multi-stage versions, persistent one-hour board searches, urgent timers, optional bonuses and the new completion/failure presentation remain the next mission-network phases; this build does not pretend those deeper loops are complete.



Verification: PSP build completes. Expanded mission checks pass across every system; radio and steering checks remain clean. The full legacy game suite retains nine unrelated pre-existing migration/system-arrival fixture failures.



## 2.5.303 - Readable crowded System Details



- Enlarges suns, gas giants, ocean worlds and rocky planets in System Details while retaining room for systems containing up to ten visible bodies.

- Replaces the cramped floating object label with a wide translucent banner, allowing full generated planet and station names to remain readable.

- Rebalances the two-row planetary layout and station position so selected-object brackets, labels and the system summary do not overlap.

- Adds a native 480x272 crowded-system visual review covering ten selectable bodies and long world names.



Verification: PSP build completes; the focused crowded-system review passes with all ten bodies visible and the selected name fully displayed.



## 2.5.302 - Clean station arrivals



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



When the ship is destroyed, the view remains on its slow explosion while cockpit instruments glitch, black out and shut down. Once the sequence finishes, use the D-pad and X to choose Load Commander or Main Menu. The former START recovery prompt and technical debris labels are gone.



## 2.5.293 - Clear, natural space conversations



Incoming calls now wait for a confirmed response and describe the consequence of each choice. Pirate demands use pirate identities, while trader exchanges clearly name what leaves and enters your hold and explicitly state that no credits are charged. Follow-up lines explain missing cargo, full holds and completed transfers in natural English.



## 2.5.292 - Physical tool-selection confirmation



Choosing a flight tool with Circle plus a direction now flashes that direction's entire console box red twice. This makes the selected physical control immediately obvious without relying on a small text colour change.



## 2.5.291 - Spiral warp apex



The peak of every hyperspace jump now forms a luminous rotating spiral tunnel. Curved cyan and violet rails, broken elliptical rings and a bright central throat grow around the ship as it enters, then recede smoothly as it exits, while the corridor remains free of text overlays.



## 2.5.290 - Seamless hyperspace surge



The corridor effect is now free of the `HYPERSPACE CORRIDOR` title and percentage overlay. Charging and braking use concise, lightly glitching status text. A dense burst of long multicolour warp streaks grows through the last part of charging, visually joining ordinary high-speed flight to the full hyperspace tunnel.



## 2.5.289 - Radar system identity



The bottom flight radar now labels itself with the current solar-system name instead of `AHEAD`, while the unnecessary `AFT` caption has been removed. Long names are centred and bounded to the instrument width; the temporary Law Scanner countdown still replaces the label while active.



## 2.5.288 - Quiet cockpit actions and visual tool confirmation



Routine cockpit actions now communicate visually instead of interrupting flight with ship-computer text. Hard braking, HUD changes, tool selection and Ignore are silent. Failed cargo recovery uses eight natural computer responses, with several reminders of the 500 m tractor limit. Circle+D-pad selections flash twice; NEXT WPN slides the old name out and the new name in before green confirmation pulses, or flashes red twice when there is no alternative weapon.



## 2.5.287 - Docked station orbit preview



While docked, the command deck viewport now presents and labels your current station. Small ports receive a clean orbit, rich stations get sunlit angled pans, and Mega Capitals move between wide scale shots and close passes across major structures. The five smoothly joined compositions form a 96-second reel with a restrained lens flare behind the hull. Seeded primary hubs and auxiliary relays remain visually distinct; in flight, the viewport returns to the ship passing suns and planets.



## 2.5.286 - Automatic PSP Wi-Fi profile search



Online mode now searches every saved PSP Infrastructure connection automatically. It tries the preferred profile first, skips empty slots, and identifies the active profile on Nearby Players once connected. This supports home Wi-Fi stored as profile 6 without manually editing `online.cfg`.



Spacebook Messages now uses the same Facebook-parody design as the main feed. A compact avatar contact rail opens readable private threads with distinct received and player reply bubbles; the Homeward Dad/Mum/friends inbox uses the same layout instead of its old terminal-like override.



## 2.5.285 - Ship-themed Galaxy.Maps and Comms Panel



The galaxy interface is now consistently named Galaxy.Maps, no longer carries the ARCElite label, and uses the currently fitted ship finish for its glass, route and navigation accents. The single-screen Comms Panel uses the same fitted palette across its metalwork, screens, buttons, lamps and selection highlight while preserving clear warning colours.



## 2.5.284 - Readable system-map labels



Selected station and world names in System Almanac use a restrained translucent navy plate, keeping labels readable over suns and bright planets without hiding the illustrated map. See BUILD-2.5.284.txt for verification.



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



## 2.5.281 - Opening battle in empty space



The dream now opens with a quick curved blink, followed by three Thargoids arriving in formation and making repeated attack passes. Real travelling shots can be dodged. The scene contains no planets, stations or ordinary traffic, and combat cannot create local-law warnings or bounty rewards. A surreal Night Shift ticker covers the two-minute maximum battle; victory or low hull wakes the player sooner. See BUILD-2.5.281.txt for validation and physical-PSP limitations. Private local build.



## 2.5.267 — Space chatter above the cockpit



Moved tutorial Dad/computer coaching, longer space conversations and response choices just below the top HUD. Radar and controls remain visible. Ground/menu and illustrated Earth-view framing stays intact. Exact packaged opening and live launch/landing/EVA checks pass. Core objects are unchanged from the passing 2.5.266 build. See docs/HOMEWARD-FIXES-267.md and BUILD-2.5.267.txt. Physical PSP and pre-existing broad legacy issues remain outstanding.



## 2.5.266 — Earth vista, safe approach and native tutorial screens



First-orbit conversation stays over Earth artwork. Premature Earth entry turns safely outward with a nonblocking Dad reminder. Alpine waterfall art replaces the countryside vista. System Details now uses the native almanac with safe selection bounds; GalacticNet shares native newspaper/Spacebook/chrome styling and Earth data. Flight lessons accept controls immediately and use compact coaching. Fixed departure completion on release frames. Exact-binary opening, live atmospheric entry/EVA and core game/radio/steering tests pass. See docs/HOMEWARD-FIXES-266.md and BUILD-2.5.266.txt. Physical PSP and pre-existing broad legacy issues remain outstanding.



## 2.5.265 — Illustrated opening and branching conversations



NEW GAME now has paced dialogue choices, authored pixel-art scenes and portraits, a proper starter dealership, and menu categories that appear as lessons unlock. Fixed SOL ROCK's launch-time null ticker access. The full tutorial and live launch pass in PPSSPP; broader legacy smoke and physical PSP certification remain outstanding. See docs/HOMEWARD-POLISH-265.md.



## 2.5.264 — Earth-to-Lave new-game opening



New Game now opens character creation and a guided story tutorial with Dad on Earth/Sol, countryside EVA, mining, Mum's call, station shopping, Thargoid displacement and arrival at Lave. Includes original Sol Rock radio, regional GalacticNet, saved object choices and V27 tutorial checkpoints. See docs/HOMEWARD-OPENING-264.md and BUILD-2.5.264.txt for controls, scope and validation.



## 2.5.263 — Private PC-server online presence test



Adds FLY / NEARBY PLAYERS and a saved Online switch accessible with Square on the title screen (Triangle toggles). A commander with Online enabled connects automatically to the configured PC server. Directory lists system, station/space/planet/Roamer status; X tracks using normal fuel-aware routes. Ships render only in the same system in space, characters/vehicles only on the same planet. Each pilot retains personal progress and normal manual saving. Existing direct ad-hoc Host/Join remains.



This is a TWO-player private development test, not public MMO hosting. PC server supplies directory/endpoints and relays matching-location movement; direct UDP peer updates are also attempted. No public hosting, router forwarding or firewall changes are performed. Node scripts are in server/. online.cfg defaults OFF and points to PC192.168.1.105, UDP25926, saved Wi-Fi profile1. Physical PSP Wi-Fi/NAT compatibility, larger populations and shared combat/economy remain unverified/unfinished. See docs/ONLINE-PC-263.txt.



## 2.5.262 — Keep your commander when friends leave



Local Wi-Fi now plays with your real commander. Shipyard/outfitting purchases, credits, missions, discoveries and other personal progress remain when either player leaves or the connection drops. The remaining player stays in their current view, position and planetary sequence; the friend's contact disappears. Normal SAVE/STATUS saving works while docked, including during multiplayer. Loading a commander ends the link cleanly first. Failed join/setup attempts still restore the starting state.



Both PSPs need262. Planetary ship and Roamer berths remain in place after disconnect. Save manually as usual: this does not add autosaving or saving while in flight. Existing save files remain compatible. Shared combat/loot authority and physical PSP Wi-Fi testing are still unfinished.



## 2.5.261 — Normal multiplayer controls and universe tracking



Local Wi-Fi now uses the normal single-player controls: hold Square for targeting, hold Circle for ship/handheld tools, Triangle for comms/interactions and Select for the command deck. No permanent Wi-Fi banner or replacement flight footer. Your friend appears first in the SHIPS targeting band and in planetary SITES / SHIP. Locking a friend in another system plots a normal fuel/range-aware route; planetary status tells you where they are. Pilots can jump independently, land, explore and use their own Roamers.



Both PSPs must use261. Smaller pilot packets, monotonic timers, validated peer activity and60-second recovery grace reduce false timeouts; disconnect safely restores your original commander. FLY Host/Join opens the session screen when already connected; X resumes, Circle ends it. Still session-only co-exploration: shared combat, permanent cooperative progression and automatic reconnection are not finished. Physical PSP Wi-Fi/FPS remains unverified. See docs/LOCAL-WIFI-UNIVERSE-261.txt.



## 2.5.260 — Local Wi-Fi planetary co-exploration



Two pilots can land independently on the same planet, see each other's animated block-style suits, walk, scan, use maps/site scenes and drive their own Roamers. Starports reserve two marked landing bays and two distinct Roamers. Existing first-person arrivals, seat transitions and departures remain. Host/Join from FLY, then Select -> choose a world -> X to land. Triangle boards your own nearby ship; R launches once aboard.



Still reversible co-exploration, not persistent cooperative progression: scans/site rewards are session-local and your original single-player state returns on exit. Both PSPs require this EBOOT. Two connected emulator instances passed landing, walking, driving, boarding, launch and disconnect recovery; both berths and distinct Roamers checked across all1024 planets. Physical PSP Wi-Fi/FPS remains unverified. See docs/LOCAL-WIFI-PLANETS-260.txt.



## 2.5.259 — Two-player local Wi-Fi flight



FLY now has large HOST and JOIN buttons. Turn on both PSP WLAN switches; Host opens a nearby flight, Join searches for it, and X joins. Both PSPs need this EBOOT. Fly together in the host's current system with your own ship/name/paint and shared traffic, planets, stations and rifts. R/L control speed; R+Up boosts; Triangle faces your friend; Select opens the session screen and Circle there ends it.



This first mode is reversible free flight, not full co-op: no combat, jumps, landing, jobs, purchases or saves during the session. Both original single-player states return on exit/disconnection/sleep. Actual discovery/join/moving ships/shared snapshots and lost-host recovery pass in two isolated PPSSPP instances. Physical PSP-to-PSP Wi-Fi is not yet certified. Retains all258 content and saves. See docs/LOCAL-WIFI-259.txt.



## 2.5.258 — Observatory and POI ground placement



Fixed Lave I's observatory bypassing its terrain footing and sinking into the hill. Shared graded POI foundations now account for higher ground inside their full footprint and both terrain detail levels; signs, visibility and targeting follow the corrected height. Checked8192 footprints and144 renderer comparisons; focused planetary/core checks pass. Retains257 performance work, with about1% more sampled walking draw time. Open cave interiors and saves remain unchanged. See docs/POI-FOOTING-258.txt.



## 2.5.257 — Faster planetary landing and walking



Optimized the planetary drawing loop and cloud blending, with whole-program compiler optimization. No scenery, starport size or draw distances removed: twelve before/after screenshots and144 renderer comparisons are pixel-identical. Emulator samples show16–26% lower approach drawing cost and12–18% lower wilderness cost versus256. Heavy views still exceed a30FPS budget; physical PSP testing remains required. Retains all256 gameplay and saves. See docs/PLANET-FPS-257.txt.



## 2.5.256 — Outward cloud reveal, planetary rendering savings and solid bridges



Arrival clouds expand toward the sides around a widening clear centre corridor, revealing the planet after the opaque loading handoff. Cached biome scenery, dry-ground shader rejection and near-first terrain reduce rendering cost without reducing scenery detail. Bridges now have solid deck/ramp undersides and side faces, with visibility checks covering their full span. Tested emulator samples use about19% less average arrival/ascent draw time and26–48% less in four wilderness views; Lave I landing remains a weak spot, and this is not locked30 or physical-PSP certification. Retains all255 content. See docs/CLOUD-FPS-BRIDGES-256.txt.



## 2.5.255 — Animated water and handheld survey tools



Planetary water now has world-anchored ripples, sky-tinted highlights and shoreline foam, aligned to the same river/ocean cells as collision and the map. Bridges now have matching raised decks and bank ramps instead of oversized invisible dry areas. Hold Circle on foot for HAND TOOLS, choose Up for the visible handheld scanner or Down for empty hands; tap Circle to scan, then Triangle opens the scanned Codex entry. Rover drift and spacecraft tools retain their controls. Retains cumulative254. Emulator checks pass, but heavy walking scenes remain slow and physical PSP testing is still required. See docs/WATER-HAND-TOOLS-255.txt.



## 2.5.254 — Layered planetary clouds



Planetary arrival/ascent now passes through shaded pixel-style cloud banks with three independently drifting scales; skies have layered high haze, middle billows and larger low clouds. Cached silhouettes and integer compositing reduce measured veil cost about31%, with about4.5% less sampled arrival/ascent rendering time. Cloud setup happens at startup, preserving full loading cover and existing controls. Retains all253 content. Not locked30 or hardware-certified. See docs/PLANET-CLOUDS-254.md.



## 2.5.253 — Planetary scenery and collision pass



Retains cumulative 2.5.252, including Elite Exploration radio and varied orbital stations. Lave I gains a visible high observatory ridge, flower-lined trail, varied-height trees, wind-blown foliage/grass and player-following wilderness scenery. Solid Lave worlds gain real distant terrain; starports gain asymmetric upper decks; solid-world crystal caves have open entrances. Low terraces and vacant traffic berths no longer act as invisible walls. Focused native tests pass; performance is not locked 30 FPS and physical PSP testing remains outstanding. See docs/PLANET-BEAUTY-253.md.



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



COMMANDER → COMMANDER PROFILE: Up/Down chooses Save Profile or Rename Commander. The page always uses the active commander record; save slots cannot be changed or loaded from inside the game. Dock before saving. The title-screen Load Commander page retains all three save cards, loading, deletion and recovery backups. Local and remote warrants appear under FLY → LAW while docked; only the current system can accept payment.



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

- Dock and open **FLY → LAW**, choose the current system warrant, call Local Law, and pay the displayed fine, or  

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













## 2.5.339 - Smoother mission boards and crowded space



Reduces repeated mission searches, caches planet artwork, skips off-screen model work and scales decorative ship effects to their screen size. Gameplay traffic and mission availability are preserved. Run `tools/review-performance.ps1` for reproducible PSP-side timing and compatibility checks; physical PSP performance still needs testing.



