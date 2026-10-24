2.5.488 packaging candidate keeps the detailed Twin Stack gun housings permanently extended and restores immediate X firing. Circle+R is unassigned and no longer appears in the controls. The commander menu removes the decorative SHIP / COMMAND LINK heading and omits STANDBY for a new commander. ERL finishes skip the automatic replay and immediately use the native ship-through-position flyby with the player's actual finishing numeral and ordinal title; LocalTV retains optional recorded highlights. Circle+Triangle remains flares, Circle+Down remains heat sink and Circle+Right remains next fitted gun. No save-format or weapon-balance change.

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

Highest-priority remaining work after477: show the nine individual native designs for user feedback; confirm on physical PSP-3000 with streamed music; address known close relay/carrier pacing; continue the remaining58 catalogue types individually under design/ENCOUNTER-ART-STANDARD.md. Do not treat passing FPS as user art approval. Preserve menu, intro, ERL, saves and prior playtests.

2.5.476 local PSP-3000 radio art rework: user rejected all ten474/475 encounter previews as too basic. Rebuilt ONLY radio entry14 with separate continuous front/rear/side/roof paintings, rounded16-sector/7-band service pod, railed deck, braced mast and separate dish.186 new storage tiles;301 total atlas tiles. Fixed the exposed fixed-size180-material/Adder128-offset batch bug using disjoint dynamic ranges; sparse ascending material iteration preserves draw order.260 host boundary/order cases pass. Exact retail radio orbit30.42/31.14/29.75FPS, zero audio starvation/drops. Mechanic/salvage and normal startup pass; relay near29.11 and carrier near28.70 miss the29.5 strict floor and remain explicitly open. No memory faults in final five600-frame runs. Retail memory budget passes with23,437,432bytes headroom; physical PSP pending. Intro byte-identical; other nine geometry and old72 materials unchanged. Prior ten basic previews are not approved finished art; nine still need the same redesign. No new placement/services/save-format changes. Native gallery assets/generated/radio-shack-rework/index.html; package deliverables/ELITE-NEXT-2.5.476-RADIO-REWORK.zip. Failedv1 is excluded; previous475 remains recoverable. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: 01A28C631593CFD2B5D7F1E60F6B02B34465D1C25F5BBB0C38FD647ED1AFA453

20 October2026 user correction: ALL TEN474/475 encounter previews rejected as too basic. Do not call17/75 finished or approved. New requirement is continuous custom pixel art for each side and believable supporting geometry. Radio entry14 is being rebuilt on a raised deck over a round service pod; native v2 passes30.42/31.14/29.75FPS. Renderer atlas limit bug fixed; regression checks running. Root475 executable remains until476 checks finish. Details design/ENCOUNTER-ART-STANDARD.md and design/RADIO-SHACK-REWORK.md.

2.5.475 local PSP-3000 encounter catalogue wave 2: Cooperative Ore Depot, Parts Exchange and Beacon Repair Crew now have individual geometry,12 fitted material plates and mechanical animation. All three exact-retail near/medium/distant orbit tests passed at32.14–34.85FPS, audio starvation/drops0; occasional50–67ms frames remain. Previous salvage village baseline and normal native startup pass on the same executable. Previous seven474 geometry and60 material plates are byte-identical; src/main.c and space-discoveries.h are unchanged from the474 source snapshot. Embedded intro is byte-identical. No new services, placement or save changes. 17/75 catalogue types now bespoke;58 remain. Natural anomalies are tracked separately. Physical PSP and broader stress testing pending. Package deliverables/ELITE-NEXT-2.5.475-ENCOUNTER-WAVE-2.zip; all ten new native previews assets/generated/encounter-upgrades/catalogue-progress.html. No Git metadata in this copy; no GitHub sync/tag claim. SHA-256: A0969A67E49FD52C28B8E10E08C253C95FCBDC3FD651848FDCB31A139016D717

2.5.474 local PSP-3000 encounter catalogue wave 1: seven new bespoke encounter designs (salvage village, shipbreaker yard, community greenhouse, patchwork fuel stop, old passenger ferry, local radio shack, miners tea barge), with 28 role-specific pixel-art plates, mapped geometry, real sunlight and bounded animation. All seven exact-retail near/medium/distant orbit checks passed, averages31.84–35.68FPS, audio starvation/drops0; occasional50–67ms frames remain. Normal native startup and retail memory budget passed; embedded intro film is byte-identical. Physical PSP, broader gameplay and crowded-view testing remain open. Existing placement, interactions, saves, menu, ERL and hyperspace source retained. Four-system encounter certification restriction remains; native gallery forces catalogue entries into a private fixture. 14/75 catalogue types now have bespoke upgrades;61 remain. Separate32-type natural-anomaly audit remains pending. Local playtest only; no Git metadata in this copy, no GitHub sync/tag claim. Package deliverables/ELITE-NEXT-2.5.474-ENCOUNTER-WAVE-1.zip; native gallery assets/generated/encounter-upgrades/catalogue-wave-1.html. SHA-256: D4D4D0BA711CA17791C7D7F5CB17DEE3150010C6215CE34479EAF739866BF4A9

Catalogue continuation,20 October2026: user authorised bespoke art for all75 space-discovery entries, showing each completed native encounter and continuing without approval pauses. Progress: design/encounter-catalogue-progress.json. Salvage village entry1 implemented and native verified at32.98/33.77/33.23FPS; physical PSP pending. Shipbreaker entry15 in progress. Retail473 remains stable;474 candidate only. No Git metadata in this working directory; no sync claim.

2.5.469 local PSP-3000 travel/shipyard pass: all three approved mega showroom styles distributed deterministically; eight-row mega catalogue, real six-category capacities, coloured spec meters, capacity tiles and full descriptions. Missing MC_BOX station-preview geometry restored, authored panel textures and closer sunlit Continue camera. Real distant traffic/weapon/cloud polish and compensated swept-clear scenic docking. ERL bookings permit travel; race-entry resources and remote cancellation protect fuel/progression/location. PSP-3000 asynchronous compressed MP3 read-ahead and validated128kbps Elite Exploration copy installed with original backup. Exact sun/atmosphere pixel checks, copied real commander round-trip and native travel/audio checks pass. Busy-view FPS and physical PSP testing remain unfinished; no locked30/60FPS or stable-release claim. Details docs/TRAVEL-POLISH-469.md and design/travel-polish469/HANDOFF.md. Stable VERSION/dist retained; no GitHub-sync claim.

2.5.468 local PSP-3000 boost onset: hyperspace streaks now grow outward and fade over0.85seconds with matching FOV easing; individual streaks fade at wrap boundaries. Ordinary speed motes integrate a continuous phase across double-R boost and use smooth colour/speed transitions, avoiding the previous absolute-time phase jump. Controls, acceleration, saves and existing audio unchanged. Exact retail startup and flight+MP3 checks pass silently; native transition recording/physical PSP remain unverified. Artifact milestones/2.5.468-boost-onset/EBOOT.PBP; source patch design/hyperwarp-current/playable468.patch. No locked60FPS or stable-release claim.

2.5.467 local playable PSP-3000 hyperwarp: approved electric charging, rainbow storm tunnel, six speed pulses and destination opening integrated into the full current game. Existing entry method, audio and all other game source preserved; rejected sound candidate excluded. Exact retail intro/home/flight+MP3 checks pass silently. Artifact milestones/2.5.467-hyperwarp/EBOOT.PBP; details design/hyperwarp-current/PLAYABLE-467.md. Physical PSP, exact-retail full-route input test and sustained frame pacing remain unverified; no locked60FPS/stable-release claim. Stable VERSION/dist retained.

Protected study source and original sound assets committed locally as d5cb928. Canonical playable files remain unchanged; no push/tag/stable-release claim.

Deep-drive audio study (15 October 2026): original charge/launch/transit/six-crest/arrival sounds now run from real route state in the isolated native hyperwarp candidate. 302,966 bytes resident16-bit PCM, no rejected defect-fan/noise layer; bounded voices and smooth cancellation/restart.85 portable-voice checks plus six actual-route cancellation cases, real route/guards, streamed MP3 and device continuity all pass. Successful sound starts verified: entry1/crests6/arrival1; max3 voices observed, no starvation/decoder errors, no clipped samples in captured device mix. Separate PPSSPP pacing: charge29.62FPS, launch29.97, transit56.77, arrival29.99;84.790ms transition spike remains. Muted-default native audiovisual gallery outputs/hyperwarp-journey-lab/audio-gallery/index.html. ALL future PC testing must remain muted unless user asks otherwise; keep PSP DSP enabled and host volumes0. Authored source/sounds/evidence protected in design/hyperwarp-current/HYPERWARP-AUDIO.md and AUDIO-VERIFICATION.json. Canonical source/playable466 EBOOT unchanged; production integration, listening approval and physical PSP checks pending. No stable-release/GitHub-sync claim.

Arrival slingshot study (15 October 2026): latest user requests forward zoom at tunnel end and a visual new-space cue. Added continuous final velocity/distance surge, destination-sun-coloured throat,0.85-second opening into the ACTUAL entered system, settling lens/wakes and a release shell. Native gallery outputs/hyperwarp-journey-lab/arrival-gallery/index.html includes a3-second ending and matched prior storm-vortex frames. Route/guards and audio continuity pass; transit50.51FPS, arrival30.99FPS, with101.205ms transition spike still pending tuning. Zero device starvation/decoder errors. Source protected in design/hyperwarp-current; details ARRIVAL-SLINGSHOT.md. Canonical playable source/EBOOT unchanged; preview only, physical PSP and production integration pending.

Storm time-vortex study (15 October 2026): latest user asks for more Doctor Who intro inspiration added to the rainbow/burst design. Original periodic128x64 blue-violet/gold cloud folds, bounded per-ring torsion/breathing/bends and crest lightning, retaining electric charge/glitches, fast flow, rainbow spirals and all six speed circles. Native gallery outputs/hyperwarp-journey-lab/vortex-gallery/index.html, compared with previous burst study. Fresh actual-MP3 test: transit51.20FPS/draw14.094ms versus previous52.20/14.013, charge/arrival about30–32; route/guards pass, zero starvation/decoder errors. Authored source protected under design/hyperwarp-current; details STORM-VORTEX.md. Canonical playable source and archived466 EBOOT unchanged; preview only, physical PSP and production integration pending.

Current hyperwarp study (15 October 2026): faster integrated flow, thick pulsing bent rails, fluid rainbow spirals and SIX varied expanding-circle speed bursts with coordinated forward/FOV surges. Electric bars and both charging-box glitches retained. Authored source now protected under design/hyperwarp-current; the separate Studio Lead cleanup deleted active temporary outputs, so source was recovered and previous/current native renders rebuilt. Latest gallery outputs/hyperwarp-journey-lab/burst-gallery/index.html. Fresh actual-MP3 pacing: transit52.20FPS (restored earlier rainbow58.35), charge/arrival about30–32; zero starvation/decoder errors, route/guards pass. Notes design/hyperwarp-current/RUSH-AND-BURSTS.md. Canonical playable source and archived466 EBOOT unchanged; native preview only, physical PSP and production integration remain open. Keep the authored design folder during temporary-output cleanup.

Rainbow hyperwarp refinement (15 October 2026): isolated native study adds three perspective rainbow helices with outward-moving pulses and0.18-second digital appearance/disappearance glitches for the compact charging panel. Electric bars/arcs retained. New native full journey/transit/glitch gallery: outputs/hyperwarp-journey-lab/rainbow-gallery/index.html; notes: RAINBOW-REFINEMENT.md. Real route/guards and actual-MP3 continuity pass; transit58.56FPS in PPSSPP, about0.82ms additional draw cost versus electric study. Canonical source and playable466 remain unchanged. Physical PSP and production integration remain open.

Electric hyperwarp study refinement (15 October 2026): latest approved-looking Ion preview now has blue-white travelling charge conduits, frontier sparks and coherent branching peripheral arcs that warp toward the tunnel. Actual native480x272 full journey and charge detail: outputs/hyperwarp-journey-lab/electric-gallery/index.html; evidence/limits: ELECTRIC-REFINEMENT.md in that lab. Real route and guard checks pass; independent actual-MP3 run transit59.36FPS, charge/arrival about30–33FPS, zero starvation/decoder errors. Canonical source/playable466 unchanged; production integration and physical PSP validation remain open.

Native hyperwarp visual studies (15 October 2026): current 466 source/EBOOT preserved. Complete real-route native baseline plus Ion Continuum, Solar Undertow and Aurora Slipstream previews; sustained flow, finer lit surfaces and a 1.4-second opening into the actual destination. No entry/control/save changes or production integration. Gallery: outputs/hyperwarp-journey-lab/gallery/index.html; notes: outputs/hyperwarp-journey-lab/DESIGN-NOTES.md. Four 360-frame native sequences, exact 12-second lossless previews, route/bounds checks and separate actual-MP3 pacing. Transit about58-59 FPS; charge/arrival about30-35 with about67 ms transition spikes. Physical PSP and production integration remain open. Follow the user's visual direction before integrating.

Highest-priority remaining work after466: physical PSP-3000 anomaly/station/ship frame pacing and listening; profile close crowds and busy discovery views; retain the existing race/menu/progression/victory work.

2.5.466 local PSP-3000 playtest: 28 new natural anomaly types, 384 new sites across all256 systems, fixed balanced geographic quotas (20 regular/13 unusual/5 rare), live shaded crystal/ice/rubble and animated three-dimensional plasma/dust/light forms. Existing four types and slot0/1 locations retained. New sites use existing saved slots2/3; save/load byte-identical. Actual scanning/reports/Codex/rewards integrated; no repeat payment. Availability-aware remaining-signal counts and saved contact identities. Bounded distance detail, cached framebuffer, prepared integer glow and one-pass dust composition. Dedicated all-system save/placement checks pass; broader687 assertions retain identical18 existing failures. Native gallery and final test limits: outputs/releases/2.5.466-natural-anomalies; docs/NATURAL-ANOMALIES-466.md. Stable VERSION/dist retained; physical PSP and crowded-view FPS remain pending; no GitHub sync claim.

Highest-priority remaining work for 2.5.465: profile close detailed-ship crowds and exterior-view frame pacing with streamed music; test the playtest on physical PSP-3000; review individual hull/discovery art using the native gallery. Retain the existing ERL, menus, progression and victory work.

2.5.465 local PSP-3000 fleet/discovery detail playtest: all29 standard hulls receive fitted panel art/lighting/glazing/outlets with distance detail; approved Adder retained. All12 capital hulls textured and actual named capital NPCs now use their hull instead of an Anaconda proxy; three freight styles updated. Explicit75-entry discovery visual registry preserves existing8-slot/four-system selection, listening post and wreck; four rift types use world-space depth-checked light. Shared atlas, startup-resident textures, material batches, guarded512KiB command arena and preserved foreground geometry support native480x272 rendering. Saves/game.c/game.h/ships.c/audio/race rules/victory retained. Native hull/catalogue/UV/guards,24 exact projection comparisons, foreground/behind and Game-state checks pass. Exact retail station-flight music about32.86FPS vs33.37 baseline, no starvation/decoder errors; separate active-fleet+engine+traffic+MP3 close-four-ship test remains slow (~20/12/30FPS vs baseline30/15/30), with continuous audio. Existing18 gameplay-suite failures unchanged. Physical PSP, close-crowd FPS and final individual art approval remain open. Package/gallery/source/evidence: outputs/releases/2.5.465-fleet-and-discoveries; details docs/FLEET-AND-DISCOVERIES-465.md. Local playtest only; stable VERSION/dist retained and Git pull blocked by pre-existing untracked music placeholders; no GitHub/stable-release claim.

2.5.464 local PSP-3000 station-performance playtest: nearby visible stations use hardware panel shading with aligned/swizzled authored textures and padded mip levels; exact original CPU visibility/depth is retained for ships/lights/targeting. Near-only 7km/9km hysteresis avoids distant-view transfer regressions. Paired staged moving station draw cost improves27.118→23.462ms,54.229→45.104ms,36.542→27.158ms (~13–26%);72 matched moving poses keep exact depth/guards. Exact-retail supplied-MP3 flight improves29.76→33.27FPS versus previous463 under the same current test; zero starvation/decoder errors and378 actual hardware passes. Engine-only passes33.80FPS; startup passes31,348,480bytes minimum kernel headroom. Native all28 station layouts/256-system roles, six scene depth checks,401,808 material comparisons and real streamed-MP3 jump route pass. This station optimisation changes main.c, station-authored-material.h, mega-city-render.h and new station-ge.h. Concurrent ERL-preview-memory/listening-post/derelict-art changes are preserved in the exact tested retail EBOOT and recorded separately; staged fixture predates the last parallel derelict-art addition. Gameplay/saves/audio/station assets/menus/ERL/victory retained. Package/EBOOT/native comparison gallery: outputs/releases/2.5.464-station-performance. Physical PSP timing/listening, exhaustive switching/sleep/docking, entry/arrival hitch and broader baseline release gate remain open. Stable VERSION/dist452 and USB files retained; no GitHub release/sync claim.

2.5.463 local PSP-3000 hyperdrive playtest: hardware-rendered Ion tunnel/warp sky/glow, cached native framebuffer work and smaller audio output bursts. Removed HYPERSPACE / SLIPSTREAM banner; charging box reduced38→24px, positioned162..185. Exact-retail startup and MP3 checks pass (29.76FPS, zero buffer starvation/decoder errors); engine-only FPS gate fails at26.29. Previous462 also fails the same comparison at26.58FPS. Exact-retail OGG continuity passes at21.59FPS with zero starvation/decoder errors. Corrected native scripted route profiler improves27.68→44.18FPS overall; settled tunnel approximately60FPS, early charge/arrival near30, arrival hitch~68ms remains. User accepts working non-locked60 playtest. Package/EBOOT/native gallery/evidence: outputs/releases/2.5.463-hyperdrive-performance. No source changes outside six selected renderer/audio files vs task-start snapshot; saves/progression/menus/art/ERL/victory retained. Earlier460/461 combined hyperspace/music harnesses muted radio with comms_quiet=1; their music-labelled timings are rendering diagnostics only, not concurrent playback proof. Exact-retail music checks remain valid; corrected463 harness asserts source/format/device activity. Physical PSP, sustained approach/arrival pacing and broader baseline release gate remain open. Stable VERSION/dist452 and USB files retained; no GitHub release/sync claim.

2.5.462 local PSP-3000 station texture TRIAL: perspective-span footprint filtering and a fixed65,472-byte lit RGB mip cache reduce panel shimmer; full-detail pixels remain nearby. Native matched motion uses the actual renderer:72 views retain identical geometry/depth and framebuffer guards; all28 layouts/256-system role coverage pass. Whole-viewport temporal change falls~30-50% on three staged camera paths. Final staged render30.369ms baseline/36.631ms filtered: material cost remains, not a free FPS optimisation. Exact retail emulator boot and MP3 busy flight pass (30.34FPS, worst83.42ms, no buffer starvation/decoder errors). EBOOT/gallery/source patch/evidence: outputs/releases/2.5.462-station-texture-trial. Main/audio/hyperdrive/sky source files match461 byte-for-byte. Original2x2 station raster and silhouette jaggies remain; no full anti-aliasing claim. Priority: optimise minification cost, improve silhouette coverage, inspect larger close station paths and test physical PSP. Stable VERSION/dist452 retained; USB EBOOT/saves/music unchanged this turn. GitHub DNS still unavailable; no stable promotion, commit/push or sync claim.

2.5.461 local PSP-3000 audio/hyperdrive hotfix: corrected cockpit mixer overflow, reduced fuzzy layers while preserving recorded engines, bounded peak control and faster exact PCM interpolation. Removed 2x2 warp sampling, skips hidden world rendering and prepares arrival sky incrementally. Tested PSP-3000 profile requests larger memory; RAM alone does not produce a large FPS gain. Exact retail engine/MP3 tests pass at 30.42/30.48 FPS, zero buffer starvation/decoder errors; short frame spikes remain. Fourteen authorised USB music tracks converted to native-rate MP3 with verified PC backups; actual battle track improved from 21.45 to 30.64 FPS in emulator, Sol Rock essentially unchanged. Package: outputs/releases/2.5.461-psp3000-audio-hyperdrive. Original music: outputs/music-originals-psp3000-461. USB EBOOT/saves unchanged. Physical PSP listening/frame pacing and broader baseline release acceptance remain open. Stable VERSION/dist452 retained; remote DNS unavailable, no sync/release claim.

Latest local playable checkpoint: 2.5.460 Ion Cathedral is now integrated, superseding the earlier preview-only notes below. EBOOT/native gallery/source patch/evidence: outputs/releases/2.5.460-ion-cathedral. Keep target/align/boost meter and gameplay rules unchanged. New renderer owns only visual state; exact game/save/audio/menu/ERL/victory files unchanged. Charge and real arrival native checks pass; actual input also reached the unchanged alien interception. Exact-retail original-model startup and MP3/OGG continuity pass. Full-resolution closed tunnel,2x2 transitional shader,1.08MiB bounded heap, unchanged memory guard passes. Remaining priority: cold destination-sky arrival hitch, physical PSP performance/listening, exhaustive cancellation/sleep/interception-return visuals and broader baseline release gate. Stable VERSION/dist452 retained; remote pull failed DNS, no commit/push/release claim. Combined native hyperspace/music report is packaged separately when complete.

Native HD-2D / spacecraft studies completed in isolated outputs/lighting-native453/captures-hd2d. Real PSP-GU glow, sun-lit materials, bounded ship detail and tapered exhaust are preview-only. CPU grades add 22–26 ms and are NOT production-ready; individual hull redesign/acceptance is unfinished. Canonical code and 2.5.452 EBOOT preserved. See preview README and docs/PRODUCTION-HANDOFF-2026-10-13.md for safe integration boundaries.

Refined lighting preview: outputs/lighting-native453/captures-soft contains native 480x272 warm-lit facets, softer sun/corona and warm glow. Isolated source only; 2.5.452 and canonical game source unchanged. Guarded 48-pose captures pass. Software render overhead remains ~10.7 ms, so this is NOT ready for release. GPU glow/cached face-light factors are a candidate for benchmarking, not an implemented optimization. See outputs/lighting-native453/README.md.

Native lighting preview, 13 October 2026: isolated source/build/captures in outputs/lighting-native453. See its README.md for exact staged-scene and timing limitations. Actual 480x272 PSP executable captures demonstrate sun/shadow/specular bands, local engine spill and glints. Preview is NOT merged or shipped; canonical main/audio/station/voyage source hashes and the 2.5.452 tested EBOOT are preserved. Heavy-scene FPS, artistic polish and hardware acceptance remain open.

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

2.5.412: ERL League opens reliably with an accepted pending race. Keep HQ browsing/animations and the exact accepted course; docked pending brief sets the beacon rather than starting the race. 26 native pending-menu/sleep regression checks and five retail HQ startup/music checks pass. Native HQ screenshot inspected. Preserves intro X/logo fix, ERL sleep fixes, Shockwave and Picross. Local release outputs/releases/2.5.412; physical PSP confirmation pending; GitHub unchanged.

2.5.411: X reveals the ELITE: NEXT logo together with the menu at any point in the intro. The film and audio continue without seeking. Preserves 2.5.410 ERL flight/sleep fixes, victory and Picross. 33 native intro checks and five exact-retail startup/music checks pass; native captures inspected. Local release outputs/releases/2.5.411; physical PSP confirmation pending. GitHub unchanged.

2.5.410: Accepted ERL events keep ordinary flight HUD/radio until READY. Sleep/resume only pauses active countdown/running races. Recover old pre-start pause/resume flags without cancelling contracts; active countdown Resume consumes X without skipping the countdown. Save format unchanged. 20 native flight-state regression checks and retail boot/countdown/music checks pass; native captures inspected. 59.94 FPS in PPSSPP. Physical PSP sleep confirmation pending. Local release outputs/releases/2.5.410; GitHub unchanged. Preserve 2.5.409 victory and Picross.

2.5.409 verification complete: 23 focused victory checks, five retail startup checks, countdown/MP3/four saves and victory performance all pass. Victory 56.80 FPS; populated racing 59.94 FPS in PPSSPP original 32 MiB model. Renderer mean 10.02ms. Native 480x272 captures inspected. Frozen local EBOOT/evidence: outputs/releases/2.5.409. Physical PSP pending; GitHub unchanged.

2.5.409 ERL Shockwave Flyby replaces the checkerboard victory animation on top of 2.5.408. Native real-time camera, stars, numeral fragments, actual hull/paint/engine roots, sunlit finish and real reward strip. Save format remains V36, with V35 migration retained. Full arcade ZIP and newspaper expansion remain paused. Validation and frozen release evidence: docs/ERL-SHOCKWAVE-IMPLEMENTATION.md and outputs/releases/2.5.409. Physical PSP verification remains pending; local release, GitHub unchanged on the shared unfinished tree.

2.5.408 playable combat playtest: outputs/releases/2.5.408. Exact retail startup/countdown/MP3/autosave checks pass (59.94 FPS emulator, worst 19.05ms). 44 domain checks pass; complete real steering/item-input race 140.55 seconds; native HUD/tunnel/wall/replay captures verified. Full ZIP unfinished, wider work paused for player feedback. V36 reads V35; back up before migration. Physical PSP confirmation pending; GitHub unchanged.

2.5.408 FIRST ARCADE PLAYTEST: user requested one new playable race now, then pause full expansion. Entry ERL > RACE > ARCADE EVENTS, event 02 COMBAT RACE (selection 1); seven racers, two laps, all four items. Steering-only full game simulation finished in 135.26 seconds. 44 integration checks pass including migration, independent seasons and exact module claims. Native HUD, tunnel, world gantry and replay checks pass. Final retail evidence will be frozen in outputs/releases/2.5.408. Physical PSP confirmation pending. Full ZIP is NOT complete. Remaining: mode-by-mode live validation, wider integration and visual polish after feedback. Keep newspaper work paused. Latest repair choice RECTANGULAR; top-right commentary caption ticker. Preserve 256KiB autosave stack fix and external winner animation.

ERL replay checkpoint 22:52: nine native checks pass (bounded real capture, three presentation-only camera cuts, exact authoritative/pay/points isolation and consumed X skip). Native captures show trackside framing needs larger/more readable ships before final packaging. This does not complete the ZIP and is not a released EBOOT. Latest user requirement, independent per-system championships, is implemented and covered by the 33-check domain suite. Documentation/evidence: docs/ERL-ARCADE-IMPLEMENTATION.md; outputs/erl408-{domain,replay}-test.log.

ERL development, 11 October 22:49: per-system seasons implemented. Each Mega Capitol system independently retains rounds/points, seed, ship registration, fixed trial times and exact module offer, with global licence/XP/fleet. Domain harness now 33/33 (720 catalogue combinations plus migration, travel/reload, repair/boost exclusivity and exact pending prize claim). Native HUD five checks pass using a diagnostics-only 64 MiB executable; original review executable exceeds 32 MiB. Retail remains MEMSIZE=0 and passes memory guard plus 59.94 FPS countdown/MP3/four autosaves (worst 19.02ms; no 100ms stalls). This retail candidate predates the newly added genuine bounded replay; do not ship it as complete. Replay integration and isolation/native camera checks are compiling. Needlefish appended at hull22; existing earned Krait and later prizes remain. Full ZIP remains unfinished; see docs/ERL-ARCADE-IMPLEMENTATION.md. Released 2.5.407 stays untouched.

Arcade development checkpoint: shared rules/serialization 60 checks pass, full-game production O3/LTO save migration/atomic checkpoints 10 checks pass. Current V36 development is unshipped; full HUD/corridor/event/prize integration remains. Preserve the separately frozen 2.5.407 EBOOT while continuing.

Playable 2.5.407: outputs/releases/2.5.407 contains the tested EBOOT, source snapshot and patch (282 focused checks). Current src also contains unfinished arcade expansion with draft V36 saves; do NOT rebuild current src and label it the released 2.5.407. See docs/ERL-ARCADE-IMPLEMENTATION.md for remaining work.

2.5.407: ERL branding is fixed magenta/pink, deep purple and cyan in every system, including conversations. Actual ship paint remains on Garage previews. Built from the tested 2.5.406 source; unfinished arcade expansion is excluded from this hotfix.

2.5.406 verified locally: 281 focused checks pass. Tested EBOOT and evidence are in outputs/releases/2.5.406. Countdown hotfix: ERL autosaves now use a 256 KiB worker stack. Complete V35 save validation exceeds 100 KiB; the previous 64 KiB stack could corrupt PSP memory during countdown. Current ERL designs, progression and V35 saves are preserved. Arcade expansion is paused for this hotfix; physical PSP confirmation remains pending.

2.5.405 verified locally: 279 focused checks pass (ERL 142, conversations 44, racing 86, retail startup 5, retail performance 2). Live native motion phases and film captures inspected. Tested EBOOT, source snapshot and evidence: outputs/releases/2.5.405. Highest-priority remaining ERL work: physical PSP-2000/ARK playtest of the combined motion, audio and racing. Newspaper expansion stays paused. GitHub is unchanged on the shared unfinished tree.

Local 2.5.405: ERL combines 120ms Broadcast Neon moving focus rims, interruptible 220ms Slipstream page transitions, aligned Paddock Signal portrait reveals, once-only XP fills, sparse star shimmer, Season route lights and Garage platform lights. Inputs commit immediately; no action queue, save-format or race-rule change. Authored victory reveal is retained. Physical PSP testing pending.

2.5.404 verified locally: 261 focused checks pass (ERL 124, conversations 44, racing 86, retail boot 5, retail performance 2). Native Events and all six Rivals captures inspected. EBOOT and frozen evidence: outputs/releases/2.5.404. Physical PSP testing pending; GitHub unchanged on the shared unfinished tree.

Local 2.5.404: Events and Rivals match the approved native screenshot layouts, with a large qualifier XP readout/progress bar, diagonal hero panels and a 3x2 pilot grid. Existing controls, other ERL pages, race rules and V35 saves are retained. Physical PSP testing pending.

2.5.403 release verified: 247 focused checks pass; final EBOOT and native captures in outputs/releases/2.5.403. Physical PSP playtest pending. V35 format and race rules are unchanged. Newspaper expansion stays paused.

Final 2.5.403 correction: put event labels at (20,132) below the large race name; use a target sprite for TARGETS, a clock for MINUTES and loop arrow for LAPS. Do not move the qualifier label back over the right-hand photograph. All nine palette sprites are resident and theme-aware.

Latest local 2.5.403: approved ERL visual race cards and headquarters panels, palette-region icons, separated rewards/XP and no redundant control legends. Native layouts follow outputs/erl-readable402. Saves and race rules unchanged; newspaper work stays paused.

Latest local update 2.5.402: selected Hyper Circuit ERL style implemented. XP remains an unlock for the next qualifier, not automatic promotion. Presentation uses real courses/ships, a bounded native image cache and precomputed geometry. No save-format or race-rule change. Newspaper expansion remains paused.

2.5.402 final visual correction: Season uses 47px circular cyan rims and 2x round numbers with the active round pink, exact approved zigzag positions and the simple cup-prize line. HQ shows ROUND above its large 4x number, with no repeated round subtitle. Final native captures and focused verification are frozen in outputs/releases/2.5.402. Physical PSP-2000/ARK playtesting is the next ERL step. GitHub remains unchanged on the shared unfinished tree; do not discard or commit unrelated work wholesale.

Latest local work: 2.5.401 keeps Introduction to the ERL stable through referral and completes it on rookie qualification. Conversation header pagination removed. Newspaper expansion remains paused.

## 2.5.400 — ERL checkpoints, introductions and navigation

Active ERL racing queues an immutable V35 save snapshot every five seconds on a lower-priority worker. Pause, results, menu changes, sleep and shutdown join the writer before later save/load actions. Only one bounded snapshot may exist. This removes synchronous checkpoint file writes from flight; physical PSP frame-time confirmation remains pending.

Boost rings are green and grant a short free burst with peripheral green speed streaks. Lave's NICE FLYING / MEET TORR BELL contact is first on the mission board with a moving rainbow sheen. Stable contract IDs and existing jobs are unchanged. Torr congratulates test winners and plots the nearest reachable Mega Capitol; Lave never opens the league dashboard after the test. First-time HQ visitors read an organiser introduction explaining the free rookie qualifier and championships before seeing the rookie league card. The acknowledged introduction uses an unused bit in the existing ERL inbox field; the V35 format is unchanged.

Circle returns Mission Log, Mission Board, Galaxy.Maps (including search) and System Details straight to the Command Deck, clearing shortcut history. Paged conversations put R and a right arrow inside the speech bubble's bottom-right corner, with L/back on later pages. Printed newspapers and other local work are retained. Newspaper expansion is paused at the user's request.

Highest-priority remaining work: playtest 2.5.400 racing on physical PSP-2000/ARK, especially five-second checkpoint writes with the player’s Sol Rock MP3. Newspaper expansion is paused by the user; preserve its 58 accepted pages and unpublished seekable-pack prototype. GitHub remains unchanged on the shared unfinished tree. See outputs/releases/2.5.400/BUILD-2.5.400.md for release evidence.

## 2.5.399 — approved newspaper first batch

NEWS now includes five complete original issues in the approved native print styles: Lave 16 pages, Diso 12, Quator 12, Riedquat 10 and Leesti 8. Twenty layouts use real font8 lettering, fixed publication palettes, native illustrations, letters, classifieds, long-feature continuations and working puzzles. All 58 pages have individual visual notes and hashes in the completion ledger. Other GalacticNet sections and V35 saves are preserved.

Paper ink bypasses ship-paint tinting. Correct puzzle instructions and printed apertures replace mismatched wording and phantom back-page actions. Existing 1,000-unit prizes, persistence, navigation and Earth onboarding remain intact. Retail startup/audio and all five front pages pass emulator checks; physical PSP playtest remains pending. The other 251 galaxy editions remain drafts, not completed publications. See docs/NEWSPAPERS-APPROVED-PROGRESS.md. Local release; GitHub is unchanged.

Highest-priority remaining work: continue the approved newspaper overhaul after the first-batch checkpoint. Current accepted count is 58/3,086 pages (five galaxy issues); retain the separate Earth paper. Do not regenerate collection.json with --draft. Use the hash-bound completion ledger, preserve the latest DockBay/ERL work and V35 saves. Add a bounded per-edition asset pack/cache before scaling artwork widely. Continue reporting progress every 10 minutes while actively working; the user has now authorised continued newspaper work after delivery of 2.5.399.

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

Highest-priority remaining work: playtest DockBay navigation, contact,
bulk purchases, rare modules, hostile meetings and save/reload on physical
PSP-2000/ARK. Verify menu and full-field frame times on hardware. Preserve
all planetary, ERL and unrelated work. The inherited broad failures are
recorded in the release report; do not describe those suites as green.
Retail excludes embedded QA. Both build scripts read VERSION. V35 saves
require this release or newer. GitHub remains unchanged until the shared
unfinished tree can be integrated safely.

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

Highest-priority remaining work: physical PSP ERL playtest (full-field FPS,
Sol Rock, sleep/reload, collisions and prize-ship ownership). Preserve the
planetary work and unrelated changes. Broad legacy failures are listed in the
release report; do not call those suites green. Checkpoints may lose up to
five seconds after power loss. Retail excludes embedded QA. Both build scripts
read the version from VERSION. GitHub has not been updated.

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

Next: playtest Crime Watch Left/Right, portrait readability and frame time on PSP-2000/ARK. The 40 focused checks pass; broader smoke failures match 395 (15 game, 162 input, two performance assertions in one failed group). Preserve the separate planets work and shared unfinished changes.

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
Next: playtest race burst timing, braking and rival visibility on PSP-2000/ARK.
Previous broad core/input failures remain separate work.

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
Next: test 2.5.390 on the PSP-2000/ARK, then playtest reply navigation, trade
offers and police stops. The known broad core failures remain separate work.
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

## PC Port Authority Workstation 1.0 — side-chat app, 10 October 2026

Desktop server launcher replaced by the selected pixel-console roster/detail
workstation: Pilots, Flight Log, Link Check, real hull/paint/location/activity,
packet counters and bounded Start/Stop/Restart. New server/status.mjs exposes
local status only; wire protocol and two-player cap unchanged. Sources/build:
server/launcher/PortAuthority.cs, Bake-Assets.py and Build-Launcher.ps1. See
server/launcher/WORKSTATION-1.0.md for behaviour, tests and local deployment.
Desktop app installed; server restarted empty on 192.168.1.105:25926. Game
VERSION/EBOOT and main-thread changes preserved. Local only; no GitHub sync.
## Physical PSP networking correction — side-chat patch, 10 October 2026

Real PSP diagnostics proved startup `inet_addr()` returned the not-yet-linked
module error 0x8002013A, sending packets to 58.1.2.128 instead of the configured
PC. `src/online-address.h` now parses IPv4 without PSP module imports; both
startup conversions in `online-presence.h` use it. Three checks were added to
`online-review.h`. Details: `docs/PSP-SERVER-ADDRESS-FIX.md`. A temporary NET FIX
EBOOT and exact test/installation evidence are isolated under workspace
`work/psp-network-fixed`. Incorporate this source fix in the next normal build.
Do not replace VERSION or normal release artifacts with the temporary test
build. Physical PSP CONNECTED / ONLINE is confirmed by the player and PC log
(12:28 BST: HELLO accepted from 192.168.1.171, sustained traffic, zero rejection
or send errors). Incorporation into the next normal release remains; no GitHub sync.

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


Verified local EBOOT: `4A29E776973B23F0A91224CC6FFFEA9C53DCEEA422CB72342FFC1F911AD56495`. 178 focused UI checks pass; core has the same 15 baseline failures, steering/radio zero. Native captures: `outputs/deck-review-20261010-104219-373`. Release: `outputs/releases/2.5.358`. Concurrent planetary source changes after the 10:42 build are not included in this tested menu artifact. Local release only; no GitHub publication.

## 2.5.357 — Multiplayer Retro Terminal

Implements the approved native 480×272 Multiplayer console on local 2.5.356. One Fly entry and title Square open online connection and local Host/Join together. CONNECTED keeps a plain red disconnect X; real PSP action icons stay in the footer. Server connection stages are truthful, peer names and locations are bounded, and local flight codes come from real advertisements. Predictable four-region D-pad navigation, explicit cancellation, No-default transport/end confirmations and caller-specific returns are implemented. Online/local exclusivity, settings retry and sleep cleanup use the existing transports; protocol and two-pilot server capacity remain unchanged.

See docs/MULTIPLAYER-357.md and BUILD-2.5.357.txt for final verification, native captures, artifact hash and limitations. Preserve unrelated shared work and player music. Existing broader game and planetary Wi-Fi failures remain baseline issues; physical PSP/two-device testing is pending. Local build only; no GitHub publication.

Verified local EBOOT: `68ED085740E86B135A29E8D25833AB6EC73B8BD0EC9B8831C95BAA065A7C5564`. Final Multiplayer console (58), online fixtures (12), real server connection (7) and unchanged server (13) checks pass. All 30 native states inspected; 93 recent UI checks pass and 120 prior captures remain byte-identical. Core retains the same 15 failures; local planetary Wi-Fi retains the same three. Highest-priority remaining work: those inherited failures and physical PSP/two-device/sleep testing. Local release only; GitHub is not updated. Full evidence: BUILD-2.5.357.txt.


## 2.5.356 — Orbit Motors and Crown Grand shipyards

Built on the current local 2.5.355, including the Pilot's Field Guide and exact boost instruction shown in the user's screenshot. Implements the approved native shipyard designs, final smaller EXCHANGE YOUR SHIP box, COSTS:/YOU RECEIVE: labels, coloured stat categories and green-up/red-down comparisons. Square changes only the reading panel between full prose and six bare-hull stats. Real source hulls are tilted, lit and bounded within their apertures; current ship paint is retained. Every Mega dealer has a unique name, stable native showroom artwork and distinct four-hull stock. Lave still sells all 22 hulls. Existing purchasing and congratulations cards remain shared. V32 saves are unchanged.

Source and coordinates: docs/SHIPYARDS-356.md. Test evidence, build hash and package locations: BUILD-2.5.356.txt. Highest-priority remaining work: physical PSP playtesting, inherited broad-check failures and the existing content/editorial gates. Do not infer full-game completion from this screen update. Git pull was attempted but blocked by two existing untracked music placeholder files; Git restored the autostash. Existing player music and mixed unfinished changes were preserved. No GitHub publication; do not commit/publish the mixed checkout wholesale.

## 2.5.355 — Illustrated Controls handbook

Implements approved concept 3, the Pilot's Field Guide, directly at 480×272 with the real 8×8 game font. Five topics list forty controls on the left; selected native diagrams animate on the right with short instructions. Boost reads “Double press R + keep it held.” Up/Down chooses a control; L/R shoulders (or Left/Right) change topic; Square toggles analog steering; Circle returns to the calling menu. Gameplay controls and V32 saves remain unchanged. Existing Cargo, Circuit Console Loadout and separate Item Storage are retained.

Current local EBOOT and dist copy are updated at the user's request. Focused review: 18 checks pass and all forty native entries are captured. See docs/CONTROLS-HANDBOOK.md and BUILD-2.5.355.txt for final hash, core comparison and limitations. Physical PSP testing remains pending; this does not certify the full game. No GitHub publication; preserve the mixed shared checkout.

## 2.5.354 — Circuit Console loadout and Item Storage

Built on the verified local 2.5.353 Cargo/exchange build. The selected first loadout/storage concepts are implemented with native hardware cartridges, actual hull/bank capacities, painted real-hull miniatures and a prominent Triangle storage action. Storage retains item-then-bank fitting and shows only matching banks and real spare copies. Its lower detail strip contains just the selected name/effect; the rejected right-hand comparison was removed. Long effects wrap instead of clipping. L/R shoulders select loadout categories; existing D-pad/Square/Triangle/X/Circle actions remain. Outfitting resale retains its original confirmation and pricing. No save format change (V32).

Source and coordinates: docs/LOADOUT-CONSOLE-354.md. Focused verification and packaging evidence: BUILD-2.5.354.txt. Highest priority remains physical PSP playtesting and inherited broad-check failures; this screen update does not certify the whole game. No GitHub publication; do not commit/publish the mixed checkout wholesale.

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

Current local build: 2.5.349. EBOOT SHA256 05582760CC907E3262FEBDD35EE76D59E3CFD7C666A7AB0960129A91FFF464D4.

Verification: PSP build succeeds. Spacebook home review RESULT 0 failures across 12 checks: one NPC-only feed in every opening phase, existing reactions, migrated private letters/photo, offline Dad reply, chapter spoiler gating, page clamping, Mission Log navigation and save/load. Opening integration reports RESULT 0 failures for all three starter ships through guided free-play progression. Seven native 480x272 feed, Inbox, attachment, final-archive and long-name captures inspected. Inherited broader regression failures remain open; physical PSP certification is not claimed.

Evidence: outputs/spacebook-home-20261009-214238-189 and outputs/opening-check-20261009-214238-988. Run tools/review-spacebook-home.ps1 and tools/review-eva-performance.ps1 -Review opening-check. See docs/SPACEBOOK-349.md.

Highest-priority remaining work: physical PSP playtesting, inherited full-game failures and EVA/audio profiling. Inbox archives derive from saved progress; no new save format. Preserve shared unfinished changes. No GitHub publication.

## 2.5.348 - Informational wanted posters

Removed the GalacticNet Wanted poster X action and both tracking prompts (on the poster and in the footer). Open warrants now consistently say LIVE WARRANT. Browsing posters no longer launches a docked commander or alters a flight target. Names, portraits, bounty rewards, risk and taken-down status remain available; identify suspects through normal local targeting and hailing.

## 2.5.347 - Raiders react to combat

Direct player hits now interrupt pirate patrols, including wanted-poster raiders in peaceful Lave. An engaged raider prioritises its attacker within 12 km rather than switching to a closer trader. Healthy raiders turn back and return fire, weave on approach and complete close attack passes before circling back. Badly damaged raiders or those facing two nearby patrols retreat from their actual threats at escape speed. A lone nearby patrol no longer constantly cancels retaliation. Untouched Lave pirates remain peaceful toward the player. Existing bounty claims and police-assist credit remain intact. Civilian defensive aim no longer gets replaced by an unrelated pirate-avoidance waypoint. No save or network packet layout change.

## 2.5.346 - GalacticNet market neon sign

Removed the pink Left Sell / Right Buy sentence from the GalacticNet Market page. LIVE TERMINAL is now a slightly larger native pixel neon sign with a soft pink glow, slow brightness drift and brief character/scanline glitches. It occupies its own strip below the ticker rows and above the footer. Undocked access retains Dock for live trading; prices and trade controls are unchanged.

## 2.5.345 - A personal shipyard congratulations card

Successful shipyard purchases now open a warm delivery card bearing the new hull name, native-size animated handwriting, restrained pixel fireworks and a shipyard seal. Six messages vary without immediate repeats, including "Enjoy your new ride!". X finishes the handwriting, then continues; Circle closes the card. Purchase input is guarded and the card blocks underlying shipyard actions, so it cannot buy twice. Failed or already-owned exchanges show no celebration. Credits, cargo, fitted modules and Item Storage retain the existing exchange rules; no save change.

Verification: PSP build succeeds. Guild navigation review RESULT 0 failures: 1,536 system/hub/assignment board combinations retain the promised family and five distinct choices; all Lave hub food/rescue actions open the right board; confirmation reserves real Food; accepted work selects its existing log entry; local flight avoids Galaxy.Maps; save/load preserves the offer. Native 480x272 briefing and board captures inspected and byte-identical in the final EBOOT. Broader inherited regression failures remain open; this is not full-game certification.

Highest-priority remaining work: resolve inherited broad regression failures and measure physical PSP EVA/audio performance. Preserve shared unfinished changes; no GitHub release was published.

Evidence: outputs/guild-navigation-20261009-205147-910. Run tools/review-guild-navigation.ps1.

## 2.5.344 - Local Guild contract guidance

Tamsin's food-delivery and the Guild rescue assignments now use the real five-offer board catalogue. While each assignment is active, a missing required contract replaces only the final board offer, preserving five distinct choices. Already docked in the right system, Next Step opens and highlights the local contract; in local flight it guides docking, and an accepted contract leads to its mission-log entry. Existing Guild progress needs no save migration.

Verification: PSP build succeeds; mission-board review reports RESULT 0 failures. Native 480x272 Wildlife Survey and Multi-stop Logistics confirmation captures inspected: full names fit, and button labels/instructions are centred. Broader inherited regression failures remain unresolved; no full-game certification or GitHub publication.

Evidence: outputs/mission-confirm-343-20261009-201104.

## 2.5.343 - Readable mission acceptance

Mission acceptance shows complete mission names wrapped and centred inside the confirmation card. YES/NO labels and the instructions below them are centred; destination text has its own clear row. Mission acceptance actions are unchanged.

Verification: PSP build succeeds; native 480x272 outfitting screenshot inspected. Cost and Balance are adjacent, with teal Cost and gold Balance. Existing mega-yard review retains its identical eight-bank drawing assertion failure in both 2.5.341 and 2.5.342; other three checks pass. Broader inherited regression failures remain unresolved. No GitHub publication.

## 2.5.342 - Cleaner outfitting prices

Removed the delivery, fit-later and loadout disclaimer lines from the module card. Cost now sits directly above Balance; Cost is teal (red when unaffordable), while Balance remains gold. Purchase and storage behaviour is unchanged.

Final 2.5.341 verification: four focused review groups RESULT 0 failures; native captures inspected and exact final captures match. Broad game failure lines remain identical to 2.5.340 (15); steering/radio pass, input times out. Dist EBOOT SHA256 82BD38F8D9E9E35DA10FD5835673D1C25496B7F4B7FE147D13787AEC8DF296DF. No GitHub publication.

Highest-priority remaining work for 2.5.341: physical PSP EVA/audio/display profiling; deep forest and cold approach spikes still exceed budget. Focused EVA/port/bridge/tutorial correctness checks pass, but full-game certification remains open. See docs/EVA-PERFORMANCE-341.md for exact timings, limits and artifact hash.

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

## 2.5.326 - Animated Discovery system dossiers

- Replaced the sparse Discovery galaxy locator with an adaptive system dossier that shows every generated sun and planet on slowly animated elliptical orbits, plus the system hub.
- Added a real System Record percentage derived from landings, field records, logged space signals and identified bounty targets, with a concise Next action leading toward completion.
- Added ten visible milestone markers. Triangle claims available rewards, pays once, and persists the claimed tier in the existing commander save data.
- Added a dramatic reward card that accelerates in from the right, settles at screen centre, bursts deterministic sparkles and reveals its message and reward text in stages.
- Preserved the 2.5.324 System Directory structure, undiscovered-world gating, quick facts and station sprites.

Verification: PSP build succeeds. Focused native 480x272 review passes dossier progress, one-time 20% payout, save/load persistence, X dismissal, slide, sparkle and completed-card captures. The broad source regression currently stops later on three inherited equipment-damage assertions outside this change.
## 2.5.327 - Exterior and cinematic flight cameras

- `third_person` is now a three-state camera preference: 0 First, 1 Third, 2 Cinematic.
- `camera_override_set/clear` supplies a render-only viewpoint; simulation, aiming and controls still use the player's `Game` transform.
- Cinematic mode has twelve deterministic compositions, advances every ten seconds and eases between shots.
- Comms settings stores Cinematic in the previously unused bit 8 while retaining the legacy bit layout.
- Focused `camera-review.flag` checks framing, shot variation and settings persistence and exports four native captures.

## 2.5.328 - Exterior camera combat polish

- Third-person framing is now `{right 78, up -30, ahead -215}` and tracks ship translation before applying rotational/offset easing.
- `weapon_fx_external()` derives paired hardpoints from the active hull mesh and projects fire toward the real target.
- Player exterior rendering adds paired world-space exhaust trails with boost variants.
- Distant star points originate from `camera_origin()`, eliminating exterior-camera translational parallax; local dust remains world-relative.
- Full HUD rendering is restored in exterior Full mode. Minimal/Hidden exterior modes explicitly draw the Square targeting browser while held.
- Focused camera review now covers boost tracking, weapon hardpoints, targeting and radio overlap.

## Historical build: 2.5.328

The Mega Shipyard and technology expansion is implemented in the shared source: twelve Mega hulls, regional/Lave catalogues, 48 fitted slots with V29 migration, 43 new modules with runtime effects, solid preview art, rare named Mega traffic, 15-minute wrecks and shuttle planetary access. Run the native `mega-yard-review.flag` fixture when doing the next visual certification pass. Hardware PSP playtesting and balance tuning remain required before retail release.

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

`death_effect()` now owns a fixed-camera starfield, easing hull-fragment separation, radial embers and an expanding core. `death_shutdown_overlay()` is composited after the cockpit: it progressively removes powered HUD regions, adds bounded sync faults, then draws the recovery panel after 2.6 seconds. Dead-flight input ignores START/Select and maps D-pad plus X to Load Commander or Main Menu. Load first respects tutorial/campaign recovery, then normal slot saves. `death-review.flag` captures and verifies explosion, shutdown and recovery stages at native resolution.

## 2.5.293 - Clear, natural space conversations

`comms_answer_incoming()` now opens conversation state without calling `encounter_respond()`. `comms_encounter_reply()` provides kind-specific consequence labels, and `comms_encounter_explain()` supplies a non-committing clarification. Row 0 explicitly commits, clears the actionable encounter and returns to flight with its outcome; row 2 declines. Random pirate/bounty transmissions now set `voice_role=PIRATES`. `trader_offer_hail()` states and confirms both inventory deltas, preserves the pending system/NPC identity and never implies a credit payment.

## 2.5.292 - Physical tool-selection confirmation

`tools_direction_box()` gives all four Circle directional controls bounded recessed boxes. `tools_direction_flash()` handles two red pulses for ordinary selections and the longer successful weapon-cycle timeline. Selection confirmation colours the complete border/background while preserving cream labels and the existing weapon ticker animation. The tool-feedback fixture now requires a substantial exact-red area in the selected Tractor box.

## 2.5.291 - Spiral warp apex

`warp_apex_vortex()` in `flight-extras.h` overlays phase 1 with six curved corkscrew rails, three to eight broken elliptical rings and a luminous central throat. Its intensity is a midpoint envelope, so the geometry eases in and out rather than popping. The fixed segment/ring bounds keep the effect predictable on PSP. `warp-review.flag` now verifies the midpoint palette and reports the visible vortex separately from the existing panel-removal and transition checks.

## 2.5.290 - Seamless hyperspace surge

`warp_effect()` leaves phase 1 entirely free of status chrome. Phase 0/2 labels use `text_px()` with deterministic small position and ghost offsets and no percentages. Phase 0 progress above `.62` eases in a second set of 12–64 long radial streaks, clamped to the canopy bounds, to bridge speed lines into the corridor. `warp-review.flag` captures pre-corridor, corridor and braking frames and checks that the former corridor panel is absent.

## 2.5.289 - Radar system identity

`cockpit()` in `voyage.h` now centres `game.systems[game.system].name` above the plan radar with a 16-character bound. The unconditional `AFT` draw is gone. The existing active Law Scanner status remains the temporary owner of that top radar row. `tool-feedback-check.flag` also emits `radar-system-label.bmp` for native-resolution inspection.

## 2.5.288 - Quiet cockpit actions and visual tool confirmation

`flight-tools-ui.h` owns transient selection feedback through `tools_confirm_time/action`. Directional tool choices no longer call `message()`: ordinary tools use two visual pulses, while WPN cycling stores the previous/current short names for a clipped horizontal transition and green pulses. Failed cycling uses two red pulses. Tractor failure now selects one of eight contextual lines, including 500 m range guidance. The hard-brake and flight HUD shortcut paths no longer post messages, and `encounter_ignore()` clears the channel silently.

## 2.5.287 - Docked station orbit preview

`menu_space_view()` now dispatches to `menu_station_view()` when `game.docked`. Hub zero renders the complete `station_architecture_for()` block/shape set; relay hubs use their actual station profile and stretched Coriolis hull. Poor ports retain a continuous orbit. Rich and Mega camera paths interpolate across five authored compositions, with class-specific zoom and offsets; capitals mix wide and close structural passes. The bounded preview adds a sun and restrained flare before station geometry so the hull occludes it naturally. The old travelling ship loop remains the undocked path.

## 2.5.286 - Automatic PSP Wi-Fi profile search

Online presence enumerates valid saved Infrastructure profiles with `sceUtilityCheckNetParam`, prioritises the configured profile, and advances after an eight-second unsuccessful attempt. Nearby Players exposes the active attempt and successful profile. No save or wire-protocol change.

Both normal and opening-story Inbox rendering now use Spacebook's pale-blue social-network skin. `spacebook_screen(1)` handles the local avatar/thread layout; `opening_spacebook_inbox()` preserves the five authored Homeward contacts and cached-Earth send action in the same visual system. Native captures are `spacebook-inbox-local.bmp` and `spacebook-inbox-homeward.bmp`.

## 2.5.285 - Ship-themed Galaxy.Maps and Comms Panel

Player-facing galaxy navigation is now Galaxy.Maps in both the command deck and screen masthead; the old ARCElite corner label is gone. `ship-theme.h` now visibly drives Galaxy.Maps and `comms-console.h`, including metal panels, illuminated controls, signal lamps and selection accents. Functional warning and PSP button colours stay semantic. Automated framebuffer checks require visually distinct Galaxy.Maps and Comms renders for all sixteen fitted finishes.

## 2.5.284 - Readable system-map labels

System Almanac's selected-object callout now blends a dark navy background at 179/255 opacity before drawing its cyan border and white name. The Lave III case is covered by a native 480x272 capture and an automated contrast/translucency check. No save-format change.

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

Exact packaged EBOOT verified: 323 opening, 615 game, 59 radio, 18 steering PASS, plus live meadow playback; all RESULT 0 failures. Live test: 538 clean-frame checks on both VRAM buffers, zero stale pixels. See BUILD-2.5.282.txt for exact hash, captures and emulator-only performance measurements.

Implementation: earth-meadow-scene.h owns the expanded connected grid and horizon, earth-meadow-render.h caches the far groves, and planet_eva_hud owns all rows outside the surface viewport. meadow-live.flag exercises real flower dialogue, dismissing it, walking and a camera sweep using the main loop/audio/alternating VRAM. No save-format change.

Preserve unrelated local work. No pull/autostash, commit, push, tag or public release unless newly requested. Remaining: restore normal dream cockpit HUD per user feedback; physical PSP walking/performance review; known broad legacy smoke issues.

## 2.5.281 - Empty-space dream battle

The opening eyes now blink and open in 1.6 seconds. Three Thargoids arrive together in an arrowhead, close in, break away and turn back into attack runs. Their positions advance continuously in world space; they are no longer repositioned around the camera. Bounded, aimed projectiles use swept collision, so a missed shot causes no damage. Player steering, throttle, braking, lasers, heat and shields retain the normal flight implementation.

The dream takes place in empty space, without planets, stations, civilians, freighters or ordinary mission/law displays. Dream hits cannot create crimes, loot or bounty rewards. A dedicated Night Shift radio ticker supplies two minutes of surreal stories that hint indirectly at the dream. SOL ROCK starts with the attack and fades while waking; saved radio choices remain unchanged. Waking occurs after defeating all three ships, reaching low hull, or 120 seconds. No save-format change.

Private local build. See BUILD-2.5.281.txt for exact-EBOOT verification and hardware limits.

Verified exact EBOOT: 319 opening, 615 game, 59 radio and 18 steering PASS checks, plus uninterrupted live battle/wake playback; all RESULT 0 failures. See BUILD-2.5.281.txt. Physical PSP and broad legacy smoke remain outstanding.

Implementation: src/opening-dream.h owns transient formation/attack legs, 18 projectile slots, eyelids, battle rendering and radio copy. src/game.c retains normal flight/weapon mechanics but suppresses ordinary world traffic and hit consequences during OP_DREAM. src/opening-dream-tests.h verifies formation, continuous movement, projectile hits/misses, empty-space rendering, timeout, victory and commander/radio restoration. Tutorial state IDs and saves stay compatible.

Preserve unrelated local work. No pull/autostash, commit, push, tag or public release unless newly requested.

## 2.5.280 - Meadow flower-trail discovery

earth-meadow-clue.h renders 25 three-flower clusters using native-size pixel petals, plus the stone's bounded depth-tested halo/core/motes. Shared earth_meadow_flower coordinates in earth-meadow-world.h drive artwork, proximity and route checks. Plants lead around the landing apron to the existing stone at local (95,55); they add no collision obstacles.

Inspecting the trail sets OF_MEADOW_FLOWERS (the previously reserved 0x80000 bit, already supported by the save mask) and OF_MEADOW_GLOW, then shows Dad's one-off clue without advancing OP_FIND. The last part of the trail leaves interaction to the stone. Skipping the clue retains the timed/walking reveal. Existing stone dialogue, object decision, stage IDs and save format remain intact.

earth-meadow-clue-tests.h exercises actual walking along the trail, optional/repeated interaction, save/reload, the fallback reveal, distant glow and occlusion/behind-camera/collected suppression. See BUILD-2.5.280.txt for exact binary results and native captures. The 2.5.279 terrain and sky coverage checks remain active. Physical PSP testing and broad legacy smoke remain outstanding.

Private local build. Preserve unrelated edits; no pull/autostash, commit, push, tag or public release unless newly requested.

## 2.5.279 - Meadow rendering repair

The 2.5.278 ground patch was only 440m wide despite a roughly 700m-wide walking area, with additional gaps before its distant tiles. Its partial picture overlay also exposed a different sky colour at steep pitch. Treat that build's visual certification as insufficient; the new checks explicitly exercise the reported positions and viewing angles.

earth-meadow-scene.h owns one 51x51 connected vertex lattice (40m spacing across the playable region, widening beyond it), flat depth-tested water, world-elevation sky and outcrop geometry. Near ground triangles match terrain_mesh_height, which uses 40m cells for tutorial Earth only. No cut-out cliff/valley sheets, partial panorama or midpoint-only tile rejection. earth-meadow-world.h shares cached tree/rock coordinates, irregular wooded bank/cliff boundaries and basin height generation with collision. Other planets retain their existing terrain spacing. opening_prepare now checks the actual exported eva_position_allowed rule when rescuing obsolete checkpoints.

earth-meadow-tests.h checks both triangle halves of all 1,024 inner tiles, coverage from 320 poses, sky continuity, changed-obstacle checkpoint recovery and native screenshots. opening-world-tests.h uses real collision to flood-fill routes from the ship to the orb, bank and overlook. See BUILD-2.5.279.txt for exact-binary results. The previous fixed mountain-coordinate assertion was updated to the relocated range. Continue to test physical PSP frame pacing and the known broad legacy smoke issues; this is not full-game certification.

The Windows linker hit a host LTO heap-allocation failure during this work. Both build scripts now request earlier GCC garbage collection at link time (ggc-min-expand=20, ggc-min-heapsize=32768), retaining O3/LTO and PSP memory settings.

Preserve unrelated local edits. Private local build: no pull/autostash, commit, push, tag or public release unless newly requested.

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

Highest-priority remaining work: physical PSP test of opening/input/audio/memory and planetary FPS; fix broad legacy smoke harness; continue post-Lave story. Local private build; preserve unrelated shared edits, no pull/autostash/push/tag/public release.

## 2.5.266 — Earth vista, safe approach and native tutorial screens

First-orbit conversation stays over Earth artwork. Premature Earth entry turns safely outward with a nonblocking Dad reminder. Alpine waterfall art replaces the countryside vista. System Details now uses the native almanac with safe selection bounds; GalacticNet shares native newspaper/Spacebook/chrome styling and Earth data. Flight lessons accept controls immediately and use compact coaching. Fixed departure completion on release frames. Exact-binary opening, live atmospheric entry/EVA and core game/radio/steering tests pass. See docs/HOMEWARD-FIXES-266.md and BUILD-2.5.266.txt. Physical PSP and pre-existing broad legacy issues remain outstanding.

Highest-priority remaining work: physical PSP test of opening/input/audio/memory and planetary FPS; fix broad legacy smoke harness; continue post-Lave story. Local private build; preserve unrelated shared edits, no pull/autostash/push/tag/public release.

## 2.5.265 — Authored Homeward opening, 3 October 2026

Rebuilt the new-game tutorial with 268 branching dialogue nodes, deliberate text/release pacing, six illustrated opening backgrounds, eight character portraits, three starter-ship illustrations and an Earth party photograph. Ship selection now compares strengths and requires Dad's confirmation; Square previews the actual rotating hull. Untaught command-deck services and empty categories are hidden, and direct page gates use the earned-service mask. Mum's call responds with Triangle; Sol TV recognition follows opening the broadcast. Saved dialogue choices and action-completion state survive reload.

Fixed SOL ROCK's absent ninth cockpit-ticker row (24 null entries), plus a defensive fallback. Exact final tutorial journeys for all three starters/both object choices, 268-node graph, save/reload, earned-menu fixtures and normal main-loop launch/audio pass. Live run: 1,202 frames,148 ascent frames,both VRAM planes,no invalid memory reads. Core game/radio/steering RESULT0. Broader legacy smoke is not green:191 input assertions, 35.81 FPS average and legacy null framebuffer accesses; investigate separately, do not claim a certified full-game release or unchanged complete baseline.

Exact EBOOT SHA256 CA0EE2B8A51ABB63874AA1E49FC9D9A431B1A48FC19E21A6AE8E2A003CF8766C,9,872,231 bytes. Read docs/HOMEWARD-POLISH-265.md and BUILD-2.5.265.txt. Source backported only after all 17 touched/new source guards passed; recoverable264 backup is retained in this task's work/canonical-before-265. Local private build; no Git pull/autostash/commit/push/tag/public release or sync claim.

Highest-priority remaining work: physical PSP opening/input/audio/memory/FPS test; repair broad legacy harness/assertions and heavy planetary performance; continue the post-Lave Dad/object/homecoming story. The wider flight/planet renderer retains its existing 3D style and Sol flight remains five-body. Preserve online/multiplayer handoff priorities below.

## 2.5.264 — Homeward new-game tutorial, 3 October 2026

New Game now creates a commander before the 65-stage Earth/Sol-to-Lave tutorial. Includes family/city/dealer scenes, actual planetary EVA and object choice, mining/loadout/mission tracking, Mum's in-flight call, real station services and pizza purchase, Thargoid displacement, delayed Galaxy Map, rock radio, regional GalacticNet/Inbox, commercial decorator install and Lave's Sol-attack TV bulletin. V27 preserves opening progress, choices and active ship/EVA snapshots; legacy commanders remain stage0.

Exact EBOOT SHA256 ede04957d57f809844c7435560b50e9283d26114ebde9c7782e0749e18fd8da6, 8858231 bytes. Final native reports: opening58/game612/radio59/steering18 RESULT0, with three full input journeys and both object choices. Read BUILD-2.5.264.txt and docs/HOMEWARD-OPENING-264.md for controls, implementation and limits. Baseline legacy input/outfitting failures remain; full legacy input/performance and physical PSP testing are not certified. This is a local private build; no public Git release or sync claim.

Highest-priority tutorial work: physical PSP testing, followed by owner decisions for the post-Lave Dad rescue/object/homecoming story. Full eight-body Sol flight would need changes to the current five-body engine. Preserve the existing online/multiplayer remaining-work notes below.

## 2.5.263 — Final private test handoff

EBOOT SHA25608FDA64B772DE4E9227F94F88AF3345E9050CC15462CFFF9B863DB916F150051,8712823bytes. Exact release: server13 checks, two actual Inet emulator clients RESULT0; online fixtures12/local Wi-Fi fixtures59/game594/radio54/steering18/terrain8192/tools/map/entry all RESULT0. Legacy local-planets-release both actual ad-hoc pilots RESULT0, including normal controls, planetary travel and purchase/save persistence. Start/Stop lifecycle verified. Source backported only after baseline-overlap checks, matches candidate; recoverable262 backup in online-presence-263/canonical-before-263. See DEVELOPMENT-263.md. No public deployment or physical PSP certification.

## 2.5.263 — Private PC online presence development, 1 October2026

Current-task/online-presence-263 snapshots full canonical262 with baseline. New online-presence.h uses saved infrastructure profile/UDP to Node server; online directory is presence only, not shared NPC/economy/combat authority. Protocol magic41454f4e/version1,156bytes, WifiPilot124. Server cap2; client reserves8 future slots. Server assigns stable berth IDs1/2 with fresh cookies. Local Wi-Fi262 wire unaffected. Online/ad-hoc mutually exclusive; on-foot berth ownership survives disconnect until orbit. No main-menu/save rollback; V26 saves untouched.

Pre-game Square opens online options; Triangle toggles and atomically writes online.cfg (enabled IPv4 port profile). Directory sorts own system first, X plots fuel-aware route/planet/person tracking. Depth-tested existing block avatar/car/ship rendering reused, filtered by location. Background network tick remains active in menus. Development server scripts require Node and do not alter firewall/router; private LAN only, not production secure. User authorized using PC as development/test server, not paid hosting/public deployment.

Review history: live01 timer unsigned underflow after ACK plus second emulator graphics startup failed; refreshed monotonic now after receive. Live02 connection worked but one emulator failed startup; delayed second launch and used D3D11. Live-final both peers connect/move/change systems/explore/save/disconnect but test driver repeatedly pressed Square rather than holding; corrected one-time diagnostic press. Only subsequent exact release reports count. Existing SDK import-fixup warnings remain; physical PSP imports/WLAN/FPS not certified. Do not claim finished MMO, public Internet or larger populations. Final hashes/report inventory to follow.

## 2.5.262 — Personal progress persistence, 1 October2026

Current task/adhoc-progress-262 is based on complete canonical261. wifi.playing distinguishes failed setup rollback from successful real-commander play. wifi_flight_start frees the setup snapshot; wifi_close never restores active commanders or resets planetary frontends. Save guard is docked-only again; STATUS load ends Wi-Fi then follows normal input. Disconnect retains local_pilot on a planet solely for berth ownership, clearing it in offline orbital input. Host launch retains health/fuel/legal status. Guest initialization preserves origin/destination warrants, creates local fallback traffic and keeps personal rift-scanned bits. Protocol262; V26 saves unchanged.

Do not claim authoritative shared combat, autosaves, physical PSP verification or GitHub publication. Prior historical sections describe their old reversible behavior, now superseded. Exact test results and delivery hashes are recorded in current-task/adhoc-progress-262/DEVELOPMENT-262.md and docs/LOCAL-WIFI-PROGRESS-262.txt.

Final release EBOOT:8705615bytes, SHA256702259ABFBEBB9420C61B4C6F800BC65C493D05CF6478851A055C873D110BF8B. All13 completed reports pass on this exact binary: protocol-262-release58 fixtures/all256 systems/all1024 planet berths; live-planets-262-release BOTH peers; live-travel-262-release BOTH peers; live-loss-262-release GUEST after owned host termination (host partial expected); core-262-release game594/radio54/steering18; footing-262-release8192 placements; tools/map/entry-262-release. No remaining owned emulators. Full inherited input/outfitting suite and physical PSP Wi-Fi/FPS are not certified.

Five changed sources narrowly backported after baseline hash checks; canonical-before-262 backup retained. Canonical source bytes equal reviewed candidate. Delivery/root/dist EBOOT hashes equal. Existing art/music/saves retained; no shared dirty Git mutation.

## 2.5.261 — Normal multiplayer controls and universe contacts, 1 October2026

Isolated current-task/adhoc-universe-261 based on complete260. Ten changed sources narrowly backported after frozen baseline hashes matched canonical; all canonical sources verified byte-identical to candidate. baseline and canonical-before-261 retained. Delivery binary8693967bytes, SHA256 D5366F8035FECBE1D39F91C3165A0F9053C73A25D26769B0C8593A27CC3E39D3; static ELF9939236bytes, Game85064bytes, MEMSIZE0/8MiBheap/1MiBstack retained. No unrelated source/art/music/save replacement. Shared checkout has substantial pre-existing uncommitted/untracked work: no pull/autostash/commit/push/tag/public release; do not claim GitHub sync.

wifi_input now only owns lobby/save protection and otherwise returns to ordinary game_input. Space/on-foot Square and Circle computers, normal menus/maps/sites/Codex, landing/launch/docking/jumps use SP controls. wifi_overlay empty: no banner/footer. Optional session screen Resume/End only, reopened through existing FLY Host/Join. New PLAYER_TARGET_ID after original station IDs; station_target_hub explicitly excludes it. SHIPS and planetary SITES list friend, status includes system/body/mode. Cross-system lock plots route_next_hop with normal fuel/range, exact route_target_direction alignment and preserved friend lock across arrivals. Other-planet target resolves orbital body; surface smooth turn/marker only in same coordinate space.

Protocol261 system/epoch fields, compact pilot/control datagrams, world-system consistency validation, stale epoch rejection, per-received-chunk interpolation mask/reset, actual monotonic time rather than clamped simulation dt,24 recv/frame and60s quiet/connect grace. Newer/duplicate peer packets keep link alive without replaying stale world state. save_game blocks local_pilot; quit_to_main_menu closes link and restores original Game.

Verified live-travel-final BOTH RESULT0 (actual ad-hoc handshake, normal Square/Circle, independent jumps, friend routing, snapshots and restore), live-loss-final GUEST RESULT0 after owned host deliberately killed (host partial expected). Planet control reviews validate same normal space/on-foot computers, walking/rover/boarding/launch and avatars. Full final report inventory lives in candidate DEVELOPMENT-261.md and docs/LOCAL-WIFI-UNIVERSE-261.txt.

Review corrections: protocol-01 accidentally tested old260 while build ran, excluded. First controller driver cleared its own held buttons; corrected. First on-foot Square window overlapped touchdown; later window passes. Legacy Triangle-to-face-friend capture opened normal COMMS, preventing guest jump simulation in travel review; removed old test action. No fake transport or global emulator configuration changes; owned hidden processes stopped.

Final exact-binary inventory: live-planets-final BOTH RESULT0 (normal space and on-foot computers plus complete vehicle transitions); protocol-final51 checks/all256 worlds/all1024 planet berth pairs RESULT0; core-final game594/radio54/steering18 RESULT0; footing-final8192 placements, water-tools-final, map-final and entry-final all RESULT0. Exact binary copied to canonical root/dist and task/ELITE-NEXT-2.5.261, matching hash/source/build/version verified; all owned emulators closed. Full inherited input/outfitting suite and hardware FPS remain uncertified.

Highest-priority remaining work: two physical PSPs / WLAN stalls, repeated jumps/landings, suspend/rejoin and memory/FPS; persistent cooperative saves/transactions and authoritative guest combat/salvage; shared NPC/wildlife authority across different locations and guest proximity; actual link diagnostics/reconnection/host migration. Normal tools are available but guest hits/loot are not reconciled into host-authoritative snapshots; this is NOT finished persistent/full-combat co-op. Original Game is restored on exit, including session-local rewards. Existing planetary FPS limits unchanged. See docs/LOCAL-WIFI-UNIVERSE-261.txt.

## 2.5.260 — Local Wi-Fi planetary co-exploration, 1 October2026

Tested current-task/adhoc-planets-260 narrowly backported after touched canonical hashes matched frozen259. Eleven sources plus version/build/docs; baseline and canonical-before-260 retained. EBOOT8708839bytes, SHA2565F50FA96CD0E87C86F28F14DCA68F411ED6805FBC9C5D55BA782F0CD0C1BDED5. Preserves all259 work/assets/music/saves. No Git pull/autostash/commit/push/tag/public release.

Select session chooses world1..4 or Resume. Independent cloud/approach/touchdown/seat/departure sequences with release guards. On-foot/rover/scanner/map/Codex/site-scene callbacks poll Wi-Fi every frame. Heavy berths become PILOT1/PILOT2, ambient traffic suppressed there, owned Roamers40m apart at open garage front. surface_landing_pad selects host-left/guest-right only with transient local_pilot. Single-player geometry/traffic/story gate unchanged; session kit bypass never grants permanent technology. Peer vehicle collision matches displayed geometry; animated block suit/shadow/vehicles depth-tested against terrain.

Protocol260 packet1144bytes publishes body/surface/vehicle/transition/parked positions; validated modes and finite vectors, coordinate-space switch snaps then interpolates. Additional18th world chunk WIFI_LIFE; guest skips fauna simulation while sharing host planet, retains own scan flags. Different planets hide peer entities and simulate their own wildlife. Static ELF9950196bytes, Game85064bytes, MEMSIZE0/8MiBheap/1MiBstack retained. Complete original Game restored; no persistent co-op missions/economy/combat/hyperspace or host migration.

Final planets-fixture-final42 checks RESULT0: all256 wire worlds and both dry berths, collision-validated exits and distinct Roamers on all1024 planets. Native planets-live-final BOTH RESULT0 for landing/walking/driving/boarding/launch, peer state/wildlife/NPC/debris and full restore. planets-drop-final guest RESULT0 after runner deliberately kills owned host during surface session; host report incomplete by design. Separate configs/MACs/memsticks/localhost, owned emulators stopped. Avatar image planets-fixture-final/wifi-surface-avatar.bmp. Earlier live01 advanced boarding before release guard; automation corrected, live02/final pass. Initial >=40m test failed float rounding; final0.01m tolerance passes. Final fresh core game/radio/steering,8192-footing,water-hand,map,seamless-entry reports RESULT0. First core runner asked for wrong filename game-tests.txt, timed out despite actual game-check passing; corrected fresh planets-core-verified passes. Full inherited input/outfitting suite remains uncertified.

Highest priorities: two physical PSPs (WLAN/suspend/rejoin/loss/repeated landing/memory/FPS); controller-driven multiplayer map/POI/Codex/scanner flows; fauna reacting to guest proximity, shared wildlife when host elsewhere, visible remote cinematic approach hulls; transactional authority/save design for persistent progression. Existing heavy planetary FPS limitation remains. See docs/LOCAL-WIFI-PLANETS-260.txt.

## 2.5.259 — Local Wi-Fi shared free flight, 1 October2026

Canonical changed sources/build scripts match the isolated current-task/adhoc-flight-259 final candidate byte-for-byte. Delivery ELITE-NEXT-2.5.259/EBOOT.PBP,8685639 bytes,SHA256 CD12FBFF2A6829E83F12CE60A36CFF960001939718728591EA5465AC288CA4C7. Full frozen258 baseline and canonical-before-259 recovery copies retained. Cumulative258 and user assets/music/saves preserved. No Git pull/autostash/commit/push/tag/public release.

local-wifi.h uses firmware common/adhoc modules, product ELITENEXT, unique AE+MAC groups, scanned8-room list, PDP25925 and1100-byte version259 datagrams. Remote pilot20Hz; world/NPC/debris chunks paced at most2/frame, max8 received/frame. Host authority for scenery/traffic, guest skips NPC simulation, pilot and NPC display smoothing, bounded/finite packet validation and MAC/session/sequence checks. Both Games85,016bytes each protected in a session-local heap backup; net pool256KiB; MEMSIZE0 and8MiB heap retained. Static ELF9,931,412bytes. Raw structs rely on identical PSP build/ABI; not an authenticated/public internet protocol.

FLY Host27/Join28 stable appended IDs and7 visible docked entries. Arrays expanded where needed; bottom buttons separate from normal list and feedback. Session modal input intercepts save/shops/jobs/jumps/landing/weapons. Triangle faces friend, R/L speed,R+Up boost,Select session,modal Circle exits. Guest and host restore complete single-player Game on leave/BYE/10s quiet timeout/sleep; held exit button consumed until release. First scope is two-pilot reversible free flight in one host system only, NOT shared progression/combat/landing/hyperspace. No save format changes.

Final wifi-fixture-final37 checks (including all256 wire worlds), wifi-live-final BOTH real PPSSPP peers and wifi-drop-final GUEST all RESULT0. Drop runner deliberately kills only its owned host instance; that host report is partial by design, guest certifies quiet-timeout restore. Separate configs/MACs/memory-stick roots, localhost built-in ad-hoc server, no UPnP/public server, no user global config edits. All owned emulator processes stopped. Initial live01/02 failed80410180 because Net MAC queried before group; fixed to sceWlanGetEtherAddr, live03/final pass. Initial fixture01 copied258 while build ran and timed out; final fixtures use correct259. Core game594/radio54/steering18 and wifi-footing-final8192 footprints/native views RESULT0; full inherited input/outfitting suite and hardware remain uncertified.

Highest-priority remaining work: test two physical PSPs (WLAN channels, cold init, suspend/rejoin, repeated join/leave and memory/FPS); improve friend radar/rear-mirror/exhaust integration; reliable authority/events before allowing co-op combat or missions, followed by shared jumps/planetary sessions. Do not unlock single-player mutations during this sandbox without transactional save/world design. Keep ongoing heavy planetary FPS work from258. See docs/LOCAL-WIFI-259.txt and candidate/live-review.ps1 for reproducible emulator setup.

## 2.5.258 — Observatory/POI footing correction, 1 October2026

Exact tested current-task/poi-footing-258 narrowly backported after frozen257 concurrency checks. Delivery ELITE-NEXT-2.5.258/EBOOT.PBP,8578167 bytes,SHA2561426926A938BC9D82FC0CDCBA5D12C06AE52CC6C5927F06F44B1B537A9CA750D. Recovery baseline and canonical-before-258 retained. Cumulative257 preserved; no Git pull/autostash/commit/push/tag/public release.

Lave I authored dome bypassed its earth foundation and started basement8m below centre height. Shared foundations missed interior terrain extrema/coarse horizon overlap. Cached16-entry rectangle/triangle extrema grade covers both ground LODs; basement/signed walls/targets/visibility now use safe origin. Lave I old basement1102.218m ->1162.527m, grade1161.327m. Open solid-world crystal caves retain ground-level interiors; no physics reservation, save/control/music changes.

Initial two144-frame parity failures (4–48pixels) exposed unstable coplanar qsort ties. Batch separation alone insufficient; final deterministic submission-order tie only for planetary surfaces fixes144/144 comparisons, preserving legacy space/mega-city sort. ship-footing8192 rectangles/all12families/25 samples perLOD/12 observatory captures and ship-perf/speed/art/walk/map/tools/world/entry all RESULT0. Core594/radio54/steering18 pass; runner stops on game report, full inherited input/outfitting NOT certified. Walking41.520ms vs25741.092 (~1% higher), aggregate31.561 vs31.372. Heavy views, cold cloud spikes and physical PSP memory/audio/input still outstanding. Added about9KiB BSS, MEMSIZE0 retained. See docs/POI-FOOTING-258.txt. Next: real-device and more steep-site validation; ongoing render profiling.

## 2.5.257 — Planetary FPS-focused optimization, 1 October 2026

Exact tested current-task/planet-fps-257 candidate backported with touched-file concurrency checks. Delivery ELITE-NEXT-2.5.257/EBOOT.PBP,8558103 bytes,SHA256 BF0DBE40F37824B31179A8D2C341D76C3783DD45B5917B2A211D969AE8909A66. Frozen baseline/src and canonical-before-257 backups retained. No Git pull/autostash/commit/push/tag/public release. Preserves256 cumulative gameplay, saves/music and all scenery/ranges/starport sizes.

main.c finite raster floor/ceil/min/max, one depth mask per textured2x2 sample, conservative post-sort screen rejection; atmosphere-clouds.h preweighted source blends; both build scripts O3/flto/fno-math-errno, no fast-math. Static ELF9.825MB vs8.802MB256, BSS effectively unchanged, MEMSIZE0 retained; real32MiB hardware not certified. Early pre-sort cull failed11 strict comparisons due coplanar tie ownership; rejected and moved AFTER sorting. Final144/144 comparisons and12/12 byte-identical baseline profile screenshots pass.

Final speed/perf/cloud/entry/tools/walk/map/rover/world/art reports RESULT0; game594/radio54/steering18 pass. Core runner stops on game report; full inherited input/outfitting suite NOT certified. Draw-time gains: approach16–26%, wilderness12–18%, touchdown10–25%; Lave I approach50.769->42.800ms, wilderness54.912->47.011. Broader approach39.872->33.117avg/worst167.462->126.507; walking47.278->41.092avg/worst64.026->55.762. Not locked30 or full-game/hardware FPS. Highest priorities: split sky/starport profiling, textured raster work, cold cloud-deck setup, physical PSP pacing/audio/input/memory; GPU/VFPU path only with parity tests. See docs/PLANET-FPS-257.txt.

## 2.5.256 — Outward cloud reveal, planetary FPS work and bridge visibility, 1 October 2026

Canonical source/root/dist verified against final cloud-tunnel-256 candidate; delivery current task/ELITE-NEXT-2.5.256/EBOOT.PBP,7379751 bytes,SHA256 2D10EA5885704E8D2F12DFC3534D64167A51A8B78D4B2D00BB4F57C010B00D29. All255 work preserved. Frozen baselines and canonical-before-256 recovery retained. Narrow source patches concurrency checked; no Git pull/autostash/commit/push/tag/public release.

planet_cloud_cover_flow adds outward sampling expansion and a widening central corridor only to post-handoff approach; complete cover remains opaque. New28KiB32x32 tagged biome-prop ring reuses exact cells across moving views, rather than rebuilding hundreds per frame; cloud-platform Lave III skips unused generic props but keeps authored planters. Dry-bit32 meadow materials skip water sampling only for conservative dry quads; near-first terrain, exact flat-port early return, integer floor equivalence and cached river basis reduce CPU work. Bridges now have5m solid deck AND ramp skins, and sphere-like300m/450m visibility guards replace the bad centre-behind-camera rejection. No collision footprint, save schema, controls or authored scenery density removed.

Final ship-speed/cloud/hand-water/art/walk/world/perf/entry/rover/map report RESULT0. Twelve profile screenshots byte/pixel-identical before/after optimization;32704 cached generation identities and12 bridge angles pass.144 reference/fast frames match. Game594/radio54/steering18 in ship-core complete RESULT0; runner stops after game report, not full inherited input/outfitting certification. Initial diagnostic emulator launch produced no report; only our owned process was stopped and a fresh flagged retry passed; final reviews all completed.

Measurements: arrival/ascent48.978->39.872ms avg (~19% lower), worst176.033->167.462. Wilderness Lave I73.765->54.912, II50.564->35.873, III17.759->9.257, IV64.302->45.199ms (~26–48% lower). Standard walking47.278ms avg64.026worst vs25556.534/89.806; aggregate Lave37.043ms vs25546.963. Isolated river52.217ms vs25550.137 due added bridge faces. Lave I approach still50.769ms (only~1% lower), cold cloud-world spikes remain. Not a universal FPS uplift or locked30/hardware certification. Highest priority: profile Lave I starport/sky pass, cold cloud-deck scenes and software raster overdraw; validate physical PSP FPS/input/audio. See docs/CLOUD-FPS-BRIDGES-256.txt.

## 2.5.255 — Animated planetary water and Hand Tools, 1 October 2026

Canonical source/root/dist match final tested water-hand-tools-255 candidate, preserving cumulative254. Delivery current task/ELITE-NEXT-2.5.255/EBOOT.PBP,7367871 bytes,SHA256 9DD931353933AEE50D8F43AF6477095A305881879F13D223C4320B60003CBF43. Recovery water-hand-tools-255/canonical-before-255. Narrow source patches concurrency checked against frozen254; no Git pull/autostash/commit/push/tag/public release.

Raw terrain_mesh_height and terrain_water_tile are separate from bridge-floor terrain_height and bridge-aware terrain_is_water. Water shader caches exact40m shore flags in80KiB, rejects conservatively dry regions and preserves flat fast rendering for dry distant quads. Matching bridge deck spans +/-240m across river, +/-26m along, with40m ramps reaching +/-280m; footprint and physics share these bounds. Riverbed cut persists below deck. Hand Tools are scanner(default) and empty hands only, not pretend weapons: Circle hold>=.20s; Up/Down equip and close until release; quick tap scans. Visible first-person handset, existing scan rewards/Codex/save path. Hand selection is transient, not serialized. Modal Back/landing/rover contexts suppress gestures. Rover Circle drift remains intact; rover target-panel scan explicitly preserved.

All final ship-tools-final/ship-world-final/ship-art-final/ship-walk-final/ship-map-final/ship-rover-final/ship-entry-final/ship-perf-final reports RESULT0. Game594/radio54/steering18 in ship-core-final complete RESULT0; full input/outfitting suite was not certified and inherited limitations remain.144 reference/fast comparisons identical; river frame50.137ms avg50.968worst; aggregate Lave46.963ms; walking56.534avg89.806worst, worse than254 walking sample44.905/63.145. Do NOT claim locked30 or overall FPS improvement. Optimize heavy textured-ground/river overdraw and cold map/foliage work, then validate real PSP buttons/audio/FPS. See docs/WATER-HAND-TOOLS-255.txt.

## 2.5.254 — Planetary cloud layers and bounded rendering savings, 1 October 2026

Canonical source and root/dist binary match final tested planet-clouds-254, retaining cumulative253. Procedural code-native8KiB cloud silhouettes with rounded lobes/rim shading, three world-bearing sky banks, three-scale drifting entry/ascent cover, integer palette compositing. Fully covered transition remains opaque; zero cover leaves framebuffer untouched. Prewarm cache at startup to avoid first planet-frame build cost. Controls, timing, physics, saves and music unchanged.

Final snapshot final-cloud/final-entry/final-perf144/final-paths64-world/final-walk/final-art1024-worlds and game594/radio54/steering18 all RESULT0. Updated only the obsolete entry assertion requiring old wavy cloud colours to match: tests now verify all pixels remain translucent below full cover and no loading background leaks at full cover. Original output differs intentionally. An initial entry run used the wrong flag filename; stopped the owned emulator and reran with seamless-entry.flag/seamless-entry.txt.

Measured253->254: cover13.564->9.365ms (~31% less),16 sampled arrival/ascent views49.921->47.676ms (~4.5% less), worst177.588->174.449ms. Lave aggregate46.921->46.405ms; walking45.130->44.905ms, worst63.234->63.145. Still not locked30; heavy cloud-world scenes and inherited full input/outfitting failures remain. See docs/PLANET-CLOUDS-254.md.

Delivery current task/ELITE-NEXT-2.5.254/EBOOT.PBP,7355023 bytes,SHA256 F9E08AF9731C70D061EABB6D8960DD81B933DE59BB1EB6978460912DB42BF64D. Recovery planet-clouds-254/canonical-before-254. Narrow source patches were concurrency checked and compared with final snapshot; no Git pull/autostash/commit/push/tag/public release. Highest priorities: real PSP input/audio/FPS validation; profile heavy cloud-deck geometry and terrain/sprite overdraw; finish inherited full-suite failures. Do not revert newer code to the253 or earlier candidate.

## 2.5.253 — Planetary beautification and real collision clearance, 1 October 2026

Canonical source and root/dist binary match the isolated planet-beauty-253 candidate, preserving cumulative 252 radio and existing station/river/vehicle work. No Git pull/autostash/commit/push/tag/public release. Frozen baseline and canonical-before-253 recovery are in the current task's planet-beauty-253 folder. New delivery ELITE-NEXT-2.5.253/EBOOT.PBP: 7,347,887 bytes, SHA256 9FF633A4CCBA5673FD51C76D37FA366FFC3CACD2DB0085F3A1505D06CB741EC8.

Implemented higher Lave I ridge + visible domed observatory, 7km distant solid-Lave terrain, camera-following deterministic vegetation cache, taller varied trees, bounded nearby wind/flowers, asymmetric starport terminal pods, traversable low terraces/vacant traffic berths, open solid-world crystal cave entrances. Lave I flat apron widened to encompass interpolation vertices; baseline port test failed there and final all1024 port checks pass. Backend changes preserve solid buildings, traffic hull clearance and cloud platform boundaries.

Exact final binary: release-beauty, release-perf144 native-identical comparisons/guards, release-map, release-art1024 worlds/8192 species/12 POI families, release-port1024 foundations, release-paths64-world cinematic paths, release-rover, release-worlds all RESULT0. Game594/radio54/steering18 pass. Full smoke attempt times out while inherited input tests show outfitting failures; not repaired or certified. See docs/PLANET-BEAUTY-253.md.

Highest-priority remaining work: reduce measured planetary render time (paired sampled walking39.144->41.431ms, +5.8%; new wilderness view45.130ms avg/63.234worst including cache refresh), profile actual hardware/audio/display, smooth cache refresh work, replace remaining broad POI footprint reservations where geometry is open, explicit water/survey-boundary readability, authored branching paths beyond the existing observatory trail. Do not describe this as locked30, invisible-wall-free across every world, or a match to high-resolution concept art. Physical PSP remains untested.

## 2.5.250 — Planetary POIs blended into terrain, 30 September 2026

User reported every planetary POI sitting on a grey stand. Root cause was one shared full-height grey plinth in surface-landmarks.h. Replaced it with a shallow sloped berm sampling actual terrain at four expanded corners and deterministic biome colours. Engineered sites retain a narrow inset footing; ruins/fossils/wrecks/crystals have natural ground only. Collision and interaction footprints unchanged; save format unchanged.

Exact tested task snapshot landmark-grounding-250. Field-art review covers all1024 worlds/8192 species plus all12 POI families at two native angles and interaction/save/framebuffer checks. Game594/radio48/steering18, all-world cinematic paths, map and Rich1568 pass. Surface differential initially exposed deterministic4–8px procedural edge omissions from 249's broad box cull. Restricting that cull to its profiled Lave layout restores144/144 pixel-identical frames while retaining Lave speed (39.058->36.899ms average in review). Physical PSP remains untested.

Delivery task/ELITE-NEXT-2.5.250/EBOOT.PBP,7,321,487 bytes,SHA256 4E0A37BFD64A5A3797A470B7915030345F12B3C56357217023912796E3E031DF. Canonical source/build metadata and root/dist binaries updated to exact tested snapshot after touched-file concurrency checks. Recovery: task/landmark-grounding-250/canonical-before-250. Previous249 delivery and user saves/music preserved; no Git pull/autostash/commit/push/tag/public release.

## 2.5.249 — Planetary candidate merged into 248, 30 September 2026

Explicit side-conversation request to integrate with the 21:17 Rich-station build and deliver EBOOT. Verified canonical src matched rich-stations-248; integrated by narrow hunks into isolated combined-249, tested, then concurrency-checked backported. Retains all 248 stations and cumulative game content. Automatic inward eligible planet entry/cloud handoff, open varied platforms, higher/farther Lave I observatory, nub-pan map, modern inertial Roamer controls and pixel-identical planetary optimisations. Saved commander fields/layout unchanged.

Final exact-binary Rich1568/ordinary1500/capital293/identity3088/map71/debug47/game594/radio48/steering18 all RESULT0, plus entry/roamer/cinematic/walk and 144-frame/16-world pixel comparison. Updated one obsolete game test: entry speed200 remains200 instead of0; weapon/energy/heat/threat-clock protection unchanged. Paired Lave render38.838->36.081ms (~7% less); walk30.449ms avg/33.228 worst; cinematics still reach94.633ms. Not locked30 or physical-PSP/full-input certified. Detailed proof: docs/PLANET-INTEGRATION-249.md.

Delivery task/ELITE-NEXT-2.5.249/EBOOT.PBP,7,319,495 bytes,SHA256 3E7FE026417B0FACC55D7B73D675566A345DD2520CD61ED22D9C4DAF9B514277. Canonical root/dist now contain this exact tested binary; prior binaries/touched source/docs preserved in task/combined-249/canonical-before-249. Previous248 delivery remains. Shared dirty work/saves/music untouched; no autostash/pull, Git commit/push/tag or public release.

Highest-priority remaining work: hardware entry/collision/vehicle validation; optimise heavy cloud-world cinematic shots without sacrificing scenery; finish inherited input/performance suite; match miniature Almanac station art to the new templates. Do not restore the old244 candidate over main.

## 2.5.248 — Rich middle-tier habitats, 30 September 2026

User supplied retro station references and requested richer middle-tier architecture. All174 Rich primaries now use sixteen templates with independently seeded service pods, signs, masts and solar plates, greenhouse/deck facade patterns, slower spin and actual2.812–4.566km longest spans. Shared solids drive collision/render/guidance; Rich grid500m, standard8km/enhanced12km Comms. Poor73, Mega9, auxiliaries, target IDs/save layout and cumulative247 planetary/debug/map content retained.

Final exact-binary Rich1568/ordinary1500/capital293/identity3088/map45/integration47/game594/radio48/steering18 passes, all RESULT0. Rich696 front/side/upper routes plus ordinary494 and capital108. Initial broad14 failures traced to obsolete fixed docking coordinates/ranges and downstream save failures; reanchored tests to actual hull/slit/range, retained strict collision checks. Full inherited input/performance suite not certified. 32 Rich view samples26.425–36.395ms avg/37.013ms worst excluding audio/display; not locked30/hardware-tested. Native catalogue32 views. See docs/RICH-STATIONS-248.md.

Delivery task/ELITE-NEXT-2.5.248/EBOOT.PBP,7,287,023 bytes,SHA256 23F8FC2D384A50447269097C7F243A7CF3216366D2440A9877E44FF921651260. Narrow concurrency-checked backport; canonical src matches tested rich-stations-248 snapshot. Root/dist, unrelated dirty files, saves/music and recovery folders preserved; no sync/autostash/commit/push/tag/public release. Next: hardware Rich close views/old saves near changed hulls/traffic/docking, optimise expensive views, then miniature Almanac matching.

## 2.5.247 — Galaxy-wide station identity fix, 30 September 2026

User's tiny-capital report was on an older EBOOT, exact system unknown. Independently found and fixed current target0 nearest-hub/name-context alias. Main0 is now stable; relay/outpost have runtime IDs123/124, bounded contact capacity125. Target-aware Comms docking uses explicit dock_hub; legacy generic dock retains nearest-hub behavior. Scanner/contacts/details/HUD/radar/Almanac agree. Save layouts, class allocation and 246 architecture retained.

Final exact-binary identity3088/ordinary1541/capital293/map45/integration47/game594/radio48/steering18 passes, all RESULT0. All256 systems: 73 Poor,174 Rich,9 Mega; every main class/profile/seed matches geometry. Actual Almanac/cycling/Comms inputs and maximum contacts tested; native all-nine-city and relay captures inspected. Full inherited input/performance suite and physical PSP unverified; capital performance limits remain. See docs/STATION-IDENTITY-247.md.

Delivery task/ELITE-NEXT-2.5.247/EBOOT.PBP, 7,249,143 bytes, SHA256 5BA1CC827827A0ED5D9DC47CCF55ECC872EB2499B6E7975A497908751AE707AE. Canonical src matches tested station-audit-247 snapshot after concurrency-safe narrow backport. Root/dist, unrelated dirty work, saves/music and recovery folders preserved; no pull/autostash/commit/push/tag/public release. Next: physical PSP hub/port validation, optimise capitals, then matching miniature art and auxiliary berths.

## 2.5.246 — Pulp-era station overhaul, 30 September 2026

User requested less uniform, less satellite-like stations inspired by 1950s sci-fi covers. All 256 primaries now use eight seeded ordinary whole-hull families or four dominant capital crowns. Continuous convex ring sectors replace the initial rejected pod-bracelet design; axial docking hubs preserve the front slit. Shared render/collision/rotation transforms and obstruction-checked guidance cover ordinary stations as well as capitals. Light NPC/freight clearance updated. Save layout, existing planetary/map/debug work and controls retained; auxiliary relays and miniature almanac art are unchanged.

Final exact-binary reports: ordinary1541/capital293/map45/integration47/game594/radio48/steering18 passes, all RESULT0. This includes 494 ordinary and 108 capital docking routes. The old broad station-tunnelling plane and invalid side-start test positions now use the generated hull; do not treat their replacement as proof of every inherited bug being fixed. Full input/performance suite not certified. Native ordinary samples21.2–27.6ms avg, capitals25.9–44.4ms avg/45.0ms worst excluding audio/display; physical PSP untested. See docs/PULP-STATIONS-246.md for provenance, actual captures, limits and next steps.

Delivery task/ELITE-NEXT-2.5.246/EBOOT.PBP, 7,236,207 bytes, SHA256 DF2B750F2BB5C7441F5AF769071BF6359793D7115985421A5D93D7DE4A03860D. Canonical source/build matches tested isolated pulp-stations-246 snapshot. Root/dist binaries, shared dirty work, saves/music and recovery folders preserved; no sync/autostash/commit/push/tag/public release. Next: real-PSP port/fly-through checks, capital performance optimisation, then matching miniature art and auxiliary geometry/berths.

## 2.5.245 — Capital architecture variants, 30 September 2026

User requested domes/spheres/mega-pods and varied 3D capital structures. New mega-city-geometry.h supplies six bounded convex templates shared by collision/rendering; narrow layout/render/test/review changes retain 58 components and five docking slits. Seeded mixes and proportions now use dome/pyramid roofs, octagonal/bevelled towers, sphere modules and long pods. Canonical src matches isolated capital-variants-245 snapshot; save/input layout retained.

Final city292 checks pass across nine systems/108 guidance routes; actual curved-hull corners are flyable, stable revisit and convexity checks pass, native views inspected. Map and debug/HUD regressions pass. Broad game retains the old station-tunnelling assertion; full input/performance suite not certified. Some city views remain over30FPS budget (sample25.2–40.8ms average); hardware untested. Scope/proof/next steps: docs/CAPITAL-VARIANTS-245.md.

Delivery task/ELITE-NEXT-2.5.245/EBOOT.PBP, 7,202,743 bytes; SHA256 9F676AF0FB20E6EBB2145493D035A744CA2AD48B9EE0DEF6DE17CFA3D61926B5. Root/dist, other contributor work, prior recovery folders and saves/music preserved; no Git sync/autostash/commit/push/tag/public release. Next: physical PSP city fly-through/close-view profiling, then independent inherited collision repair.

## 2.5.244 — Resumed mega-capitals and Field Map, 30 September 2026

Recovered saved mega-city and field-map-244 tracks; both directories retained. Final combined-244 snapshot starts at canonical 243 and contains both main.c hook sets. Canonical src now matches that tested snapshot. Nine capital systems use 58-piece stationary solid cities and swept guidance; map uses actual terrain with player-centred metric chart, overview and planet Codex return. No save fields changed; visit-scoped map fog retained.

Final city273/map45/integration47 checks pass; pilot/port/shadow/Lave world reports RESULT0. Broad game retains the known station-tunnelling assertion; broad input/performance groups not certified. City sample25.7–39.0ms average, some views exceed30FPS; map first cache85–135ms then ~24ms. Hardware testing remains. Full scope/proof: docs/CAPITAL-MAP-244.md. Next: hardware city/map review, expensive-view profiling and independent inherited collision repair.

Delivery: task/ELITE-NEXT-2.5.244/EBOOT.PBP, 7,186,111 bytes; SHA256 5A41EA9BCCD0671C745DA497E61A9199F4B5850713ACEE5BA41F8B3D68854A3D. All assets embedded. Root/dist binaries, other checkout work, saves/music preserved; no Git sync/autostash/commit/push/tag/public release.

## 2.5.243 — Larger futuristic starports, 30 September 2026

User requested much larger futuristic planetary airports. Canonical source now matches isolated task/starports-243. Apron 1440m square, player pad 440m square, two 320m traffic pads, seven new architectural exteriors, larger traffic models, moved/widened garage. Shared map/collision/foundation/cloud deck and POI reservations updated; all existing saves/IDs retained. Focused native port, transfer, Lave activities, shadow and debug/map/HUD checks pass. Final broad game retains one inherited station-tunnelling failure; radio/steering pass, broad input timed out (inherited failures observed), no all-green claim. Rendering is not locked30 and physical PSP remains untested. See docs/STARPORTS-243.md for proof, performance and next steps.

Delivery: task/ELITE-NEXT-2.5.243/EBOOT.PBP, 7,131,063 bytes, SHA256 097D3FBABAE480AD7E675AEF81DFB5253A84FA4A5ACBD5C8DD19D2931FE926EA. Root/dist binaries and other contributor work preserved; no Git sync, autostash, commit, push, tag or public release. All geometry embedded; no review flags in player folder.

## 2.5.242 — Fauna ground shadows, 30 September 2026

Small soft contact shadows now use the shared planetary fauna renderer, grounded beneath the animal and lightened by lift. Existing depth/biome colour retained, water/void rejected. No save/input/behaviour changes; cumulative 241 retained. Native four-Lave shadow checks, fauna regressions and all 47 debug/map/HUD tests pass; physical PSP and steep/close-angle profiling remain. Existing broad failures are not fixed or retested. docs/FAUNA-SHADOWS-242.md has scope/proof. Delivery: task/ELITE-NEXT-2.5.242/EBOOT.PBP, SHA256 DBB57B09D9972E92F6B5385239D7C37A64A49FAFE9A83B331EA11E2121A093F2. Shared dirty work and root/dist builds preserved; no public release.

## 2.5.241 — Combined debug, planetary map and HUD, 30 September 2026

User requested the three specialist tracks in the cumulative EBOOT. Integrated runtime unlimited fuel/range toggles, visit-scoped START map and simplified EVA condition meters. Corrected map close/release safety, dead contexts, boarding fog retention and negative grid edge; aligned R glyph and added START MAP prompt. Canonical source matches isolated integrated-241 source snapshot. No other contributor changes reverted; root/dist binaries remain separate from the player delivery, no public release or Git sync claim.

Delivery: task/ELITE-NEXT-2.5.241/ELITE-NEXT-PSP/EBOOT.PBP. SHA256 7017267D1E5C413B65C47B69D794CE13A7EDCB4F1E950361DE480876551C4239. Focused 47 checks and cinematic/input regressions pass; native screenshots inspected. Broad radio/steering pass, game retains one station-tunnelling failure, input completes with 184 failures (same count as specialist run); no whole-game green claim. Physical PSP/performance still unverified. Detailed scope/proof/controls: docs/INTEGRATED-241.md. Expanded TV remains excluded. Next: physical PSP review and independent broad-failure repair.

## 2.5.240 — Actual-world first-person transfers, 30 September 2026

User requested cinematic first-person entry/landing/boarding/departure and a bigger freely navigable spaceport. The earlier attempt is in 2026-09-30/elite-space-work and still used a separate flat render set/static cockpit panel; it was inspected, not overwritten. This pass extends this newer 2.5.239 tree and preserves the Lave environmental/animal work. Other checkout's active TV work is separate.

Live actual-world camera, opaque cloud handoff, smooth approach/touchdown, automatic animated airlock, ship/roamer seat hop, live parked cockpit, forward reveal then pitched departure. Seven port buildings, broad apron/lanes, clear garage exit, outward traffic pads, expanded Lave III deck and specimen relocation. Saves/IDs unchanged. Native focused transfer and existing landing/EVA input tests pass; all LaveII–IV activities/routes/scan/save checks pass. 64-world landmark path sample passes after fixing one tall-site obstruction. Reports/captures: task/cinematic-port/{final-pilot,final-worlds,final-fauna,final-smoke}. See docs/CINEMATIC-PORT-240.md.

Delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Cinematic-Port-2.5.240/EBOOT.PBP, SHA256 A9E1D04A178F7206A096AAF308D87FE32AE25A71C68EC8A1E2D76AD6F8D25695. All assets embedded. Root/dist EBOOTs, other checkout, user saves/music untouched; no pull/autostash, commit, push, tag or public release.

Next: actual PSP playback and cold/worst-frame profiling. Cinematic render averages I37.516/II33.344/III40.479/IV34.690ms; worst samples71.494/60.194/104.396/62.228ms, excluding audio/display. Do not claim locked30, hitch-free motion or high-resolution concept quality. Broad inherited station-tunnelling issue remains; complete broad input/performance suite is not certified. Always use fresh review folders and await emulator exit before launching the next check.

## 2.5.239 — Other Lave planets, 30 September 2026

User requested completion of Lave's other planets. Current bounded expedition areas for II (mosswood forest), III (floating cloud skyport) and IV (glacial) now use distinct environments matching the existing orbital type/art. Solid Lave worlds share the 160m terrain lattice for draw/collision. III uses a shared 40m deck/void footprint connecting every actual POI; visible parapets and collision margins prevent stepping/driving into void. Embedded pixel-art prop kit, biome-aware species palettes, regional Codex text and cloud-site descriptions; old animal behaviours, site/scan IDs and save layout retained.

Delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Lave-System-2.5.239/EBOOT.PBP. SHA256 9C32C94540697FD63A876B1CC990F9F4010ECC5F5E383A56AD733631A81A0F76. Full notes/provenance/test limits: docs/LAVE-WORLDS-239.md. Final reports under task/lave-worlds/{final-worlds,verified-fauna,verified-lave,verified-art,verified-broad}. Native landing, all-nine-site walker/rover reachability per world, 24 scans, 21 optional activities, three port jobs/repeat protection and save/load pass. Lave I, fauna and catalogue regressions pass. Broad retains one station-tunnelling failure; radio/steering pass; input/performance groups not completed. Corrected boundary warning and diagnostic verified. Earlier reused report folders could return stale buffered contents with fresh timestamps: always use fresh directories; world runner now enforces this.

Performance samples exclude audio/display/capture: II 25.418ms avg/26.246 worst; III 30.457/32.693; IV 40.131/43.265; I regression34.041/36.547. Physical PSP untested; IV is NOT a locked-30-FPS result. New trees are billboards, distant ridges/clouds backdrops; no whole-planet traversal, navmesh ecosystem or unique per-species skeletons.

Next: physical PSP tests and IV renderer profiling, then remaining-system biome rollout. Only Lave uses the new environmental kit/decks. Other prepared atlas rows are not integrated elsewhere. Keep root/dist binaries and shared dirty work intact; no autostash/pull, commit/push/tag or public release this pass. Do not install review flags with the player build.

## 2.5.238 — Animals first, 30 September 2026

User paused the every-planet biome rollout to fix sliding wildlife on Lave I. Shared fauna now has idle/wander/feed/alert/flee/return behaviour, independent deterministic RNG, collision-checked routes, actual-distance grounded gait, family hop/wing/rest poses, and walking/running/rover proximity reactions. New 8x4 pose atlas was made with built-in imagegen and palette-baked; provenance and exact prompt are in assets/source/fauna-animation/README.md. Full notes: docs/FAUNA-LIFE-238.md. Species/save identity and player controls unchanged.

Delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Animal-Life-2.5.238/EBOOT.PBP. SHA256 C5218D49DB0315A4CFEF94F1150988D95975BC24EFB2B44D11EF7575369895AB. Native focused fauna/art/Lave checks pass in fauna-life/{candidate,regression,lave}; 2,831 animal spawns across all 1,024 planet slots checked. Lave update+draw averages 33.883ms, worst36.304ms excluding audio/display/capture. Physical PSP untested. Broad game still has one inherited station-tunnelling failure; radio/steering pass, input/performance not completed.

Next: physical PSP animal motion/performance; richer directional poses if needed; resume biome-matched every-planet scenery and floating gas-giant settlements (explicit user choice). Gas visits already work but use a flat research island, not actual finished platforms. Pending biome-props kit is NOT integrated and needs alpha cleanup/regeneration. Local avoidance is not a navmesh or ecosystem; these remain four-pose side-view sprites.

Shared dirty tree preserved; no autostash/pull, commit/push/tag or public release; root/dist binaries unchanged. Keep review flags out of deliveries. Do not claim whole-game green or concept-level visuals.

## 2.5.237 — Shared planetary POI/wildlife pass, 30 September 2026

All twelve exterior POI families use src/surface-landmarks.h, including physical signboards and more detailed port roofs/windows. Stable site IDs, seeds, encounters, rewards, collision reservations and save format are unchanged. field-sprites.h + field-world.h share a new embedded 16-family flora/fauna atlas with the Codex; field-leaves.h adds at most 24 nearby tree leaves. Art source is AI-generated and palette-baked, not hand-drawn; prompt is in assets/source/field-wildlife/README.md. Full scope/proof: docs/POI-WILDLIFE-237.md. Other planets retain their earlier terrain/vegetation quality; this does not turn every background into lush Lave.

Delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/POI-Wildlife-2.5.237/EBOOT.PBP. SHA256 E62448EDD2CAFD2DF774582232FB0A4C0EB5368E7A263B52A1DF56E03DBF518E. Focused native art and Lave regression reports beneath that task in poi-wildlife/final-review and final-lave pass. Sampled update + draw 33.738 ms average / 36.096 ms worst, not including audio/display wait/capture IO; physical PSP untested. Native full-catalogue checks cover 8,192 existing identity slots, not 8,192 unique authored sprite assets. Review flags must never be copied into the delivery folder.

Highest-priority remaining work: physical PSP close-tree/biodome/port profiling; further material/animation detail within budget; unrelated inherited broad-suite failures. POI footprints remain conservative solid interaction sites, not new 3D walk-through interiors. Shared dirty tree and root/dist EBOOTs preserved; no autostash, commit, push, tag or public release. Do not claim whole-game green or a 1:1 match to the high-resolution concept.

Broad 2.5.237 check: same one inherited station-tunnelling failure, radio/steering pass; input/performance not completed in this run. Reports: poi-wildlife/final-smoke-verified. The delivery contains no review flags or test saves.

## 2.5.236 — Lave I native walkabout repairs, 30 September 2026

The earlier 2.5.235 concept-targeting pass was not visually verified and actually rendered mostly bare ground: draw_lave_vegetation passed world X as a screen coordinate. Corrected that plus atlas crop, upright billboard pitch/depth, shared terrain/collision, connected surface_poi observatory position, a real dome, far woodland, path/shore rendering and multiple CPU bottlenecks. All local changes are described in docs/LAVE-I-LUSH-EXPLORATION.md; do not repeat the old claim that the concept already runs 1:1.

Final local delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Lave-I-2.5.236/EBOOT.PBP. Native proof and focused report: lave-motion/final-review/ beneath that task workspace. Focused checks pass; final sampled update + draw 31.600 ms average / 34.946 ms worst (without audio/display wait/capture writes). Physical PSP and sustained all-views 30 FPS are unverified. There remains a substantial art-quality gap from the high-resolution reference.

Highest-priority remaining work: close-tree/port/rover worst-case hardware profiling; richer ground cover and less repeated vegetation; more sustained route/interaction/save checks; existing broad-suite failures. Keep distant ridge backdrops distinct from the bounded walkable island. The source image is AI-generated, not hand-drawn. No input/save-format changes. Shared dirty tree, root/dist EBOOTs, music and other contributors' work are preserved; no autostash, push, tag or public release. The opt-in review flag belongs only in disposable test directories.

Broad check comparison for this pass: baseline game 2 failures / input 182; final game 1 inherited station-tunnelling failure after updating obsolete Lave assertions. Radio/steering pass. Candidate input was stopped at 145 seconds in campaign speech, with 139 recorded failures all present in baseline; it did not complete. Reports: lave-motion/{baseline-smoke,candidate-smoke,final-smoke}. Final EBOOT SHA256 55E8E50928A5B1182588ED9FCD960812EF4DACE01DEF6FEB0A02F842C7CC3046. No whole-game green status is claimed.

## 2.5.234 — Scheduled Local TV, 30 September 2026

CH8 is now ONE station with three five-minute programmes, driven by PSP real local clock. Top-right HH:MM and NEXT/LATER times agree; midnight wraps; page entry/resume joins the current caption, not a restarted programme. Removed Left/Right/X tuning and footer prompts; Circle exits. Mira uses caption-gated soft babble, slower Night Stories cadence and existing mouth motion. Speech shares SFX/quiet-chatter controls; radio fades out on TV and returns without retuning. Native masked ships now visibly cross the window in both directions.

Focused final native QA: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/local-tv-scheduled/qa-tv-20260930-082047-520/, RESULT 0 failures. Actual framebuffer/animation/PCM proof is alongside in proof/. Both baseline and candidate broad smoke retain the same one collision + 138 input failures and campaign-region stall; radio/steering pass. Physical PSP and full-suite green status are not claimed. Existing scripts loop within each five-minute slot; Lave-only authorship remains. docs/LOCAL-TV-SCHEDULE.md supersedes the old selectable-channel documentation.

Highest-priority remaining work: physical PSP sound-level, timezone and suspend/resume test; expand programme script variety; fix inherited broad-suite issues independently. Other contributors' shared edits are preserved. No auto-stash, GitHub push/tag or release was performed.

## Giant background freighters and planet occlusion — local candidate, 30 September 2026

Moved deep-traffic hull/exhaust rendering before planet discs, fixing distant freighters drawing over worlds. Replaced two enlarged fighter meshes with four approximately twice-size modular capital silhouettes, six system/lane palettes, pointed prows and long twin coloured wakes. Crossings now take 150–230 seconds. Reachable capital-freighter gameplay is unchanged. Details: docs/DEEP-FREIGHTER-POLISH.md.

Tested candidate: outputs/chart-build-20260930-012536-359/EBOOT.PBP (v2.5.232), SHA256 4733A9C26DF06DCF72EB75B7FAF9FF0EC1615E61394F7E4250391D8039C776C1. Delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/outputs/ArcElite-v2.5.232-deep-freighters/EBOOT.PBP. Native captures: outputs/deep-freighter-review-20260930-012652-671/. Clear routes and a deliberately planet-blocked route were inspected; the planet masks hull and fire correctly. Performance: outputs/soft-sky-performance-20260930-012714-920/, five passes at 29.97–37.40 FPS, worst sampled frame 39.00 ms. Physical PSP still required.

The shared source picked up concurrent unfinished station-room changes that failed on missing lave_arrivals_hero_pixels. Final EBOOT therefore uses the last successful 01:19 snapshot plus final freighter geometry; current main.c/voyage.h match it exactly, while only station-crawl.h and station-room-kit.h differ. Shared source, root/dist EBOOTs, VERSION and Git state were not overwritten. Broad inherited station/outfitting failures remain unresolved.

## Soft flight sky and camera comfort — local candidate, 30 September 2026

Removed the screen-space hashed colour patches responsible for the blocks and rapid pattern changes on pitch. Replaced them with seeded continuous celestial cubemaps, exact camera-relative perspective/roll, soft per-pixel colour, dark dust gaps, calmer star twinkle and feathered sun flare spots. Removed the redundant 20 Hz random dust overlay. Ship controls, planetary gameplay, station work, chart changes and saves remain unchanged. Fixed cache: 101,400 bytes; warm sky rendering 7.87 ms in PPSSPP. Details: docs/SOFT-SPACE-SKY.md.

Latest isolated full-game candidate: outputs/chart-build-20260930-005542-746/EBOOT.PBP (v2.5.232). Native captures/report: outputs/soft-sky-review-20260930-005658-365/. 48 focused checks plus complete-catalogue generation checks pass: all 256 maps have distinct pixel hashes and varied cloud/dark regions. Visual inspection covers the six-system gallery, selected close sun/planet views and Lave pitch frames, not every direction in every system. Full-flight tests: outputs/soft-sky-performance-20260930-005812-106/, averages 29.97–37.40 FPS; worst sampled frame 38.96 ms. Nominal 30 FPS test explicitly accepts half-refresh 29.97 with a 29.90 tolerance, not a guarantee of >30 on every frame.

Delivery copy: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/outputs/ArcElite-v2.5.232-soft-space/EBOOT.PBP. SHA256: 63C55846EB47913B40D83DC7A608D06069BFDDB1C40189A493D6B3D2D9A8C958. Shared root/dist binaries, VERSION and unrelated edits preserved; no Git mutations or release publication. Physical PSP comfort/frame-pacing review is still required. First sky generation costs ~0.19 s on system entry. The inherited station collision failure and campaign-input smoke stall are not fixed or claimed green.

Broad verification follow-up: candidate reports outputs/soft-sky-smoke-20260930-005935-230/, pre-sky baseline outputs/soft-sky-smoke-20260930-010208-697/ using the 003414-752 EBOOT. Both have identical failure lists: one station swept-collision failure and 138 outfitting/module-bank input failures. Both pass radio/steering; input remains incomplete at the same campaign-response line and broad performance is not reached. Candidate smoke timed out at 120 seconds. Do not describe the whole suite as green; investigate these separately before a coordinated release.

## Deep Chart PSP feedback — local candidate, 30 September 2026

Small chart text is now exact 5x7 pixels, replacing uneven 6x9/8x12 stretching; header retains exact 2x scale. Vertical nub pan is inverted only on the chart. Chart controls use a bounded wall-clock timestep rather than the physics 50ms cap, maintaining hold-to-zoom speed on slower frames. Filter membership is evaluated once per node and packed-lane nebula interpolation preserves identical colors with less work.

Latest candidate: outputs/chart-build-20260930-003414-752/EBOOT.PBP. Report/native captures: outputs/chart-review-20260930-003521-506/. 26 focused checks pass, including vertical inversion, 10/60 FPS zoom timing, native glyph pixels and interpolation equivalence. Same-camera Jobs zoom draw fell from 21.66 to 17.41 ms/frame in PPSSPP (~20% less draw time); ALL from 23.65 to 19.51. A Jobs-only slowdown was NOT reproduced in the emulator baseline; real PSP confirmation remains necessary. Full-game suite remains unverified; no shared root/dist overwrite, version bump, commit, push or tag. No planetary or flight-control changes. See docs/DEEP-CHART-VISUALS.md.

## Deep Chart nine-filter rail — local candidate, 30 September 2026

Square cycles ALL / VISITED / UNVISITED / RICH / POOR / MEGA / JOBS / IN RANGE / ROUTE. A 90x86 top-left panel shows five readable rows with a proportional scrollbar; same height, 13 pixels wider. JOBS excludes unaccepted offers, IN RANGE uses current fuel, ROUTE uses the plotted path. Empty-state hints and context pins are retained. No bookmarks or save-format changes; no planetary changes.

Latest tested EBOOT: outputs/chart-build-20260930-002531-098/EBOOT.PBP. Captures/report: outputs/chart-review-20260930-002650-091/. All 22 focused checks pass; long-label, middle-scroll and final-scroll native captures inspected. Warm all-systems rendering 23.79 ms in PPSSPP. Physical PSP and whole-game integration verification still pending. Shared root/dist binaries, version and other contributors' work were not overwritten; no push/tag. See docs/DEEP-CHART-VISUALS.md and src/deep-chart-filter-tests.h.

## Deep Chart visual pass — local candidate, 30 September 2026

Replaced decorative point dust with an embedded starless blue galaxy illustration; only actual systems/route hops are sharp points. Added softer star glows, glass panels, larger chart typography, bounded labels, readable search and icon controls. L/R zoom, nub pan and plotted route-star behavior remain intact. Details and art provenance: docs/DEEP-CHART-VISUALS.md.

Tested isolated candidate: outputs/chart-build-20260930-001230-933/EBOOT.PBP (built from a v2.5.231 source snapshot). Native 480x272 ALL/MEGA/zoom/search captures and nine passing focused checks: outputs/chart-review-20260930-001351-721/. Warm all-systems chart draw measured 23.71 ms in PPSSPP; physical PSP not tested. No claim that the broader inherited smoke suite is green. Shared root/dist EBOOTs were deliberately not overwritten and no version bump, commit, push or tag was made, to preserve concurrent work. Next: physical PSP legibility/frame-pacing review, then coordinated integration build/release. Build with tools/build-chart-candidate.ps1 and verify with tools/review-deep-chart.ps1.

## 2.5.231 — Local TV approved-art rebuild, local development build

Local TV now uses the exact full approved studio illustration at PSP resolution with live native captions, original CH8 logo and cyan/violet sibling idents. Mouth motion stays registered to Mira; window traffic is masked behind the station. Three four-passage Lave programmes loop; Left/Right tunes, X restarts, Circle backs out. Full-height glyphs replace compressed labels. Source, provenance and opt-in native tests are integrated; details in docs/LOCAL-TV-231.md.

Highest-priority remaining work: physical PSP test; investigate inherited station swept-collision failure and campaign-input smoke stall independently; later author per-system TV branding/scripts. Focused TV QA is green, broad smoke is NOT green. Only Lave has authored TV content, not 256 unique stations. No remote push/tag was made because the shared checkout contains other contributors' unfinished work and broad verification is incomplete. Do not autostash or overwrite those edits. Current delivery is a local v2.5.231 test EBOOT.

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

Custom music scanning accepts case-insensitive `.ogg` and `.mp3`. `audio.h` dispatches OGG tracks through Tremor (`libvorbisidec` + `libogg`) into the same cubic 44.1 kHz resampler, source shuffle, crossfade and suspend lifecycle used by MP3. OGG-only installs skip PSP MP3 utility/resource initialization. Thargoid score remains source 7, outside the visible radio station count, and still falls back to generated Pixel Comet action music when empty.

`tools/check-thargoid-music.mjs` covers scanning, decoder/linker wiring, encounter routing and instructions. PSP build passes; native PSP OGG timing, corrupt-file recovery and mix balance still require hardware testing. Save/config formats unchanged.

## 2.5.205 — Thargoid battle music library

The audio worker has an eighth file source at `music/Thargoid Battle` without increasing `RADIO_STATION_COUNT` or exposing it on the tuner. `audio_battle` follows transient `thargoid_active`. Source transitions fade gain to zero before switching decoder/synth state, then restore the unchanged selected radio source after combat. Battle score bypasses Radio Off but obeys `radio_volume`; source 7 falls back to synth station 2 when empty. Runtime startup creates/scans the folder and accepts up to 24 mono/stereo MP3s at 8-48 kHz through the existing decoder.

`tools/check-thargoid-music.mjs` passes and the PSP build succeeds. Compiled radio coverage verifies source allocation. Emulator remained closed; native PSP MP3 transition timing, decoder recovery and mix balance still require testing. Save/config formats unchanged.

## 2.5.204 — System Operations and Thargoid interdictions

Fly > System Details is now an operational 480x272 dashboard rather than a shorthand body list. It retains six selectable targets and existing lock/align behavior. Its station dossier reads current NPC roles, local jobs, local warrant/fine, per-system bounty completion and station manifest progress; celestial dossiers read range, access, persisted landings, per-world life/site records and local rift logs. Gas worlds correctly identify floating skyports. A 78-point progress meter is derived only from saved accomplishments, and an ordered NEXT strip recommends an existing activity.

There is deliberately no invented “live system conditions” layer. Visited planets may show their already-implemented deterministic local clock, culture and sky profile; unvisited records remain locked. New helper regressions compile in the input suite and `tools/check-system-operations.mjs` checks source wiring. PSP build/static checks pass, emulator kept closed; native PSP readability and controller testing remain required. Save/assets unchanged.

Hyperspace now performs one 14% encounter check after entering the corridor. A triggered Thargoid ambush exclusively owns rendering/input and pauses the underlying jump. Three waves contain 3, 4 and 5 generated raiders with three movement patterns, recurring attack passes and aimed bolts. Nub/D-pad moves the sight; held Cross fires. Each kill adds 20 units. Victory resumes at the braking phase; defeat cancels the jump, restores the origin/destination and spends no jump fuel. State is transient and not saved. `tools/check-thargoid-ambush.mjs` plus compiled input fixtures cover trigger wiring, reward and both outcomes; no runtime/emulator or hardware playtest yet.

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

Final package: EBOOT 3,232,479 bytes; SHA256 19A78D023514AE00A536AFA04418A3609086BF24571DC22BC20F058A75401351. Copied to dist/ELITE-NEXT-PSP and current-thread outputs/ArcElite-v2.5.194. Core changed-source whitespace check passed. Runtime checks remain unexecuted.

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

Final package: EBOOT.PBP 3,213,247 bytes; SHA256 C0100E5FD8717F156EB4DA591BCAEEEF6C6EA5322233D09D3B684FB326D2A984. Delivered to current-thread outputs/ArcElite-v2.5.189 and canonical dist/ELITE-NEXT-PSP. Build succeeded, static checks passed, git diff --check clean. Local only; no commit, push or release. Runtime tests remain unexecuted.

## 2.5.188 — Spacebook commander names and reactions only

Reactive posts now name the current commander profile (for example BEN), not the historical ship model. All 57 player-event variants use natural person-based wording; ambient posts remain unrelated local chatter. Existing V22 posts adopt the profile name immediately, and renaming the commander updates their displayed name throughout the feed. Stored dates, authors, event history and reactions are unchanged. No save migration.

Removed Spacebook's Local replies text, Triangle footer prompt, reply overlay and Triangle input action. X Like / Square Dislike remain. Inbox is unchanged. Tutorial guidance now describes Like/Dislike. Regression coverage checks BEN in every player-event variant, 24-character name fit, and Triangle no-op on Spacebook.

Verification: ../../work/smoke-20260927-183214-505; all five groups RESULT 0 failures. Native Spacebook capture inspected: BEN named in launch/paint posts, Like/Dislike only, no reply footer/action. All event variants pass BEN and maximum 24-character profile-name tests; Triangle no-op covered. EBOOT 3,181,559 bytes; SHA256 D5BB80FABBBD6ACEA63647FA65FD33276EF839BB9EAF70335896304C9A2E982E. Copied to dist and current-thread outputs/ArcElite-v2.5.188. Local-only, no commit/push/release. Save format unchanged (V22). Physical PSP testing remains outstanding. git diff --check clean.

## 2.5.187 — Local Spacebook history

Spacebook now prepends a bounded history of 16 reactive posts per system, ahead of the seven existing community/story wires. Twenty event categories have three prose variants each, with generated spacey handles. Events capture the ship model at the time; viewing a card never rerolls its author/text. Actual successful custody, fines, fleeing, player ship kills (pirates distinguished from other ships), station departures/docking, paint purchase, ship purchase, planet landing, flora/fauna/rift scans, salvage, non-smuggling contract completion and completed hyperspace travel produce local posts. Routine same-type posts throttle for 90 seconds. Ambient humour is eligible every four minutes of active simulation, capped at three retained ambient posts so idling cannot erase the entire activity history. No combat RNG is consumed.

X toggles Like; Square toggles Dislike. Switching replaces the previous reaction; pressing the same button clears it. Reactions travel with posts as new cards push older ones down. Seven legacy wire reactions persist separately. Tutorial evidence remains first, and Messages retains its separate inbox.

PSP RTC dates are saved as UTC and shown YYYY-MM-DD HH:MM UTC. Clock-unavailable records are explicitly labelled, never given invented calendar dates. Returning after 3+ minutes away (including loading after a real-world absence) adds a wondering-about-the-ship post followed by a welcome-back sighting, both dated when actually generated. No fabricated offline events/backdating. Changed/backward clocks do not underflow cooldown/absence arithmetic. Date accuracy depends on the PSP clock.

V22 appends 34,824 bytes for 256 bounded feeds, timestamps, local reactions and metadata. Older saves load with empty reactive history, not reconstructed memories. Older builds cannot read V22: back up saves. History is part of manual commander checkpoints, not a separate unlimited/autosaved journal. Only the newest 16 reactive records per system survive rollover. Native rendering, no external art/audio/network dependencies. Main thread stack explicitly 1 MiB for bounded Game snapshots/migration/test fixtures; 8 MiB heap unchanged.

Final verification: ../../work/smoke-20260927-181522-276; all five groups RESULT 0 failures. Emulator average 55.06 FPS, worst 150.15 ms, 28 frames >25 ms; planetary scenes >=24 FPS. Earlier native captures exposed a time_t ABI conversion issue (1975); replaced it with pointer-based PSP RTC tick conversion, then verified the displayed date against the actual RTC year and inspected corrected native screenshots. Updated the existing Spotters fixture to account for prepended history; Spotters now only reports local travellers. Current captures show dated feed cards, correct reaction placement, scrollbar and native controls. Save validation/migrations, history isolation, real jump/custody hooks, like/dislike/clear, fallback/backward clocks, rollover and idle cap pass. EBOOT 3,182,631 bytes; SHA256 92C41A2A00964D537E27C7237F63D5E635A18B96A4317C9C8A30270FFC811011. Copied to dist and current-thread outputs/ArcElite-v2.5.187. Local only: no commit/push/release. git diff --check clean. Next: physical PSP date/timezone, stack/memory, saved-history and control checks; previous planetary hardware shutdown remains unverified. Extension notes: docs/SPACEBOOK-HISTORY.md.

## 2.5.186 — Living stellar rifts and instrument reports

Replaced WORM meshes and fixed close-range circles with layered, additive cosmic fields: eight inclined filaments, bright knots, drifting dust and breathing cores. Four stable profiles (Aurora Veil, Gravity Lace, Ember Nursery and Meridian Echo) share their names, palettes, reports and lore across targeting, the flight view and Codex. Fields remain one anomaly ID each. Drawing has bounded loops, close-up size limits, view clipping, distance culling and a low-glow high-contrast mode. Existing mystery/scan mission hooks remain.

Triangle analysis opens a paginated Ship Computer report with existing babble audio; Triangle/Circle closes, L/R reads. No human reply choices and no invented teleportation, mining or gravity mechanics: reports separate survey observations from folklore. Tutorial briefings wait until the report closes.

V21 adds 256 bytes of per-system four-bit rift identity masks under the existing CRC and atomic save validation. Revisit/load restores scanned status; repeated scans reopen the report without another discovery/reward. Older V20 and earlier saves import with unlocated historic totals, not invented locations. Back up saves: older EBOOT versions cannot read V21. Space Signals in the new Codex now lists actual logged field identities and opens illustrated reports. No external art/audio assets required.

Verification: ../../work/smoke-20260927-173730-593, all five groups RESULT 0 failures. Native captures of all four fields, both report pages and the Codex dossier inspected. Scan, repeat reward, revisit, V21 save/load, V20 migration, invalid-mask rejection, report input and Codex identity checks pass. Emulator average 55.06 FPS, worst 150.15 ms, 28 frames >25 ms; planetary scenes >=24 FPS. EBOOT 3,092,391 bytes; SHA256 A0C2E6F3018B5FF818C308645DACAB45F33CE640C30929DB1319FECDDEF39E15. Delivered in dist and current-thread outputs/ArcElite-v2.5.186. git diff --check clean. Local only: no commit/push/release. Next: physical PSP review of close-range fields, report readability and audio; original planetary shutdown report remains unverified on hardware.

## 2.5.185 — First-person station departures

Player-facing Launch, narrative Launch, docked wanted pursuit and docked chart departure now enter a five-second first-person sequence. The ship starts inside its own berth, accelerates through an illuminated tunnel aperture, bursts clear and eases to 100 m/s before returning control. Geometry uses a continuous analytic speed/distance curve. Relay departures retain their original hub instead of teleporting to the primary. Arrival docking is unchanged. Raw launch remains the low-level live-space initialiser used by isolated simulation fixtures; all player launch paths use launch_departure.

The actual system sun now lies beyond the primary port's outward (-Z) axis, at a safe 130-142 km distance; a narrow clear sightline is reserved without clustering the other planets. Existing sun rendering/flaring is used, with a wider departure flare that respects high-contrast suppression. Tunnel masks hide external space beyond the mouth until it clears the canopy. Existing docking and boost cues accompany the launch. No external art or save-format change.

Inputs cannot skip or steer the sequence; held buttons must be released before fresh actions can fire. Wanted target auto-tracking resumes after the release gate. Tutorial lessons/drawing defer until departure finishes, and the scripted tutorial now allows departure plus warp time. Departure flare strength eases back to normal over two seconds after the handoff. Docked chart jumps queue until departure completes. Launch guidance consumes no boost fuel and bypasses collisions only during the controlled corridor traversal.

Final verification: ../../work/smoke-20260927-172407-335, all five groups RESULT 0 failures. 56.11 FPS average; worst 133.47 ms; 21 frames >25 ms; planetary scenes >=24 FPS. Native berth/solar captures inspected. New tests exercise actual Launch input, mash/hold immunity, cruise slowdown, collision/fuel safety, outward solar alignment, relay origins, queued warp, first fresh Circle after release and resumed wanted tracking. Full tutorial passes with its launch lesson deferred and jump test allowing the new travel time; docking, system spacing, Codex and earlier regressions remain green. EBOOT 3,073,759 bytes; SHA256 5715662EC063EDFAEE7EAA905F3344638C0A879811CC8F5AD5927F41251C1A6B. Copied to dist and current-thread outputs/ArcElite-v2.5.185. Local-only; no commit/push/release. Next: physical PSP departure timing/flare/audio review; previously reported planetary landing shutdown remains unverified on physical hardware. git diff --check clean.

## 2.5.184 — Hierarchical Discovery Star Atlas

Discovery Codex now opens a visited-system atlas with a star-map locator, seven-row windows, scrollbars and shoulder-button paging. System directories contain charted station/star records and only landed worlds. Illustrated planet dossiers show shared world type, settlement/weather profile, and four selectable category tiles: Flora, Fauna, Minerals and completed Field Sites. Collections enumerate actual per-world saved IDs, not commander-wide counts; individual dossiers use the same animated species sprites/names/traits as the surface. Breadcrumbs and an eight-frame bounded navigation stack restore parent selections at each Back. Empty collections explain how to fill them and cannot open invented records.

Removed fabricated mineral/echo location lists and seeded discovery totals. Space Signals explicitly explains the current limitation: space scan totals have no saved per-system identity and cannot be retrospectively located. Charted station/star entries are labelled as chart knowledge, not claimed visits. Galactic Lore remains separate and unchanged.

The present engine supports 256 systems x 4 landable planets = 1,024 worlds, with 8 field slots per world (8,192 records). Browser providers enumerate only the selected branch; no galaxy-sized UI allocation. New categories/levels can extend the provider and bounded navigation model. This is not unlimited engine/storage expansion and adds no new species. No save-format change (still V20), no external art dependency.

Verified final build in ../../work/smoke-20260927-170300-458: all five groups RESULT 0 failures, 55.71 FPS average, worst 150.15 ms, 23 frames >25 ms; planetary scenes >=24 FPS. Native galaxy/system/world/category/record screenshots inspected; clipped list counter fixed and richer field notes added before final run. Enumeration covers all 1,024 worlds / 8,192 field IDs; empty, last-page and exact cursor/identity navigation checks pass. git diff --check clean. EBOOT 3,049,423 bytes, SHA256 7F3D49A35711D56975ED9ADC6ADCC7486C6221080C22E1B184F25E2BFF9E9985. Copied to dist and current-thread outputs/ArcElite-v2.5.184. Local-only; no commit/push/release. Design/data-extension notes: docs/DISCOVERY-ATLAS.md. Next: physical PSP readability/input review; persistent identified space-signal records; richer site-specific illustrations/filtering as collections expand; previously reported landing shutdown still unverified on hardware.

## 2.5.183 — Ship tools / tractor recovery and truthful Law stops

Circle+Left now equips TRACTOR in the compact Ship Tools selector. Release, then tap Circle: prefer the selected recoverable object within 500 m, otherwise find the nearest visible recoverable object in range; smoothly auto-turn before running the existing beam animation and inventory transfer. Rocks, distant/occluded/dead objects cannot be collected. Manual steering, changing target, leaving flight or a police stop cancels pending alignment. Menu-Back suppression is preserved. Missiles, flares and heat sinks retain their directions. To keep the displaced fitted ECM useful, it automatically pulses against close incoming missiles, still costing 18 shield energy with an 18-second cooldown.

Law now records civilian/police assault and destruction, contraband, refusal and fleeing independently per system. Dialogue names the actual offence, includes a paginated charge list and only mentions goods aboard when present. Fine replies and receipts no longer claim an empty hold was confiscated. A clean scan cannot erase an existing warrant. Surrender removes only cargo-attributed charges, including after saving or revisiting a system; violence survives. Existing yellow reply selection is preserved; L/R shoulder buttons read longer testimony.

V20 appends 1,024 bytes of offence/cargo-charge records under the existing checksum and atomic save validation. Older saves import; their unknown incident details are explicitly described as unavailable rather than invented. Back up saves before upgrading: older EBOOT versions cannot read V20 saves. Art/audio remain embedded; only EBOOT replacement needed.

Verification: ../../work/smoke-20260927-164241-476, game/input/steering/radio/performance all RESULT 0 failures. Average 55.45 FPS; worst frame 150.15 ms; planetary scenes >=24 FPS. Tests cover real hit attribution, destruction, mixed/capped warrants, V19 imports, V20 persistence, cargo-only surrender, no false clean-scan release, lawful pirate hits, tractor equip/tap/turn/recovery, range/rock rejection, manual/menu/police cancellation, ECM energy/cooldown, Law reply highlighting and shoulder pagination. Native Law/Ship Tools captures inspected. git diff --check clean. Packaged EBOOT 3,036,655 bytes, SHA256 1E97CBFD2D329048329AC5FACA93178A5393F9A49213D58C8564D2FB9883F8BB; copied to dist and current thread outputs/ArcElite-v2.5.183. Source remains local-only: no commit/push/release. Physical PSP not verified. Next: hardware tractor/control/readability checks, previously reported planetary landing shutdown, and further station content variation.

## 2.5.182 — Station reading and glowing focus

Fixed station option highlight/text alignment by drawing both at exact pixel coordinates. Controls now occupy the bottom 16 pixels. Room entry shows persistent room prose; hovering only changes visual focus. Cross explicitly inspects or speaks. Long descriptions and conversations are word-wrapped into six-line pages, navigated with Left/Right without triggering actions. Replies use the right rail and retain yellow selection, leaving the lower panel for full-width speech. Expanded room, landmark, prop and crew writing, with authored responses for each Lave activity step. Selected people, props and doors receive a clipped, softly pulsing cyan/cream outline with bright corners, including high-contrast support. No new save format or art files.

Final verification: ../../work/smoke-20260927-162447-488, all five groups RESULT 0 failures; 55.45 FPS average, station worst-room CPU draw 6.04 ms (ten draws per room). Tested room-entry prose, hover/message-expiry persistence, explicit Cross inspection, forward/back pages, reply highlight isolation, service return, complete paragraph pagination, glow clipping, Chandler stock wrapping and existing Lave/save/tutorial regressions. Native room, prop, two-page conversation, contrast and stock captures inspected. EBOOT SHA256 506DBA584C703138FABEDDEA8250C5FA9F899BCFE8ACA1FB01FCDCAB5F8E87AB; 2,932,031 bytes. Source and docs remain local-only; no commit/push/release. Physical PSP UI/audio/landing checks remain outstanding. Next: hardware review of readability/glow and broader station art/story expansion.

## 2.5.181 — Lave / Berth Six station

Seven native 340×168 raster rooms with a shared 32-colour kit, transparent crew/prop atlases, deterministic economy/seed variations and shared visual/hotspot anchors. Lave primary has a named cast, a cross-room missing medical-manifest activity (60 U), a Lave I survey follow-up (90 U), station service links, yellow reply selection, ambient machinery and a canteen jukebox using the existing procedural music channel. Reorte's Second Shift/Arrivals and the tutorial's original contacts remain intact.

V19 adds 3,072 bytes of per-system/per-hub activity state; earlier saves import. Generic cargo progress no longer resets on visiting another hub, and survey tips require local scan records and cannot be repeatedly farmed. All artwork is embedded: replace EBOOT.PBP only, keeping saves/config/music. Back up saves before upgrading; older builds cannot read V19 saves.

Verified final EBOOT in ../../work/smoke-20260927-151208-146: game/input/steering/radio/performance all RESULT 0 failures. 55.84 FPS average; worst frame 133.47 ms; 23 frames >25 ms; planetary scenes >=24 FPS. Seven room CPU draw benchmark max 5.13 ms (ten draws each). 5,376 seeded room/hub combinations validated. Native seven-room/dialogue captures visually inspected, including active-notice dialogue protection. Rebuilding all three art atlases produces identical hashes. SHA256 EC959179843B7B40211CE86683061F7345D994A18AEEC85EF7EED9355A11A267; 2,896,551 bytes. Local-only: no commit/push/release. Physical PSP remains unverified, especially previously reported landing shutdown. Source/prompt provenance in assets/source/lave-station/PROMPTS.md; scope, save layout and next steps in docs/STATION-FIRST-SLICE.md.

## 2.5.180 — Combat bearings, missile launches and space effects

Thin camera-relative indicators show actual recent incoming fire (double chevrons for rear sources), including freighter guns and incoming missiles. Player missiles emerge forward, coast briefly, then turn with a bounded rate and a short world-space exhaust trail; swept collision preserves fast hits. Distant ship dots gain faction-tinted rear trails. Sun-facing/nearby lens flares grow, and close solar exposure rapidly heats and damages shields/hull without a false combat alert. Enlarged planets reuse sample/tint calculations instead of repeating them per pixel. No save-format or asset changes.

Verified ../../work/smoke-20260927-142958-646: all five groups RESULT 0 failures;55.45 FPS average,worst150.15ms,25 frames>25ms,planetary scenes >=24 FPS. Forward/side/rear missile paths, camera-relative/expired fire hints, solar shield/hull damage and exact enlarged-planet pixel parity pass. Native missile stages, bearing arrows, distant trails and close sun inspected. SHA256 167367168E4707BB1444E0DE1CCAC259B0D949EF20A6227D0811ED20EE15A541. Local-only; no commit/push/release. Highest priority: physical PSP close-planet performance and effect review, plus earlier unresolved landing shutdown.

## 2.5.179 — Faction almanac

Rebuilt Factions as four illustrated dossiers with Identity, In Play and Channel pages. Full paragraphs explain actual gameplay, with crew sayings, practical menu links and preserved story channel notes. Local living-ship totals are labelled LOCAL SNAPSHOT rather than faction strength. X cycles pages, Up/Down selects faction, Triangle preserves nearest-contact selection. No new faction mechanics or save changes.

Verified ../../work/smoke-20260927-141818-299: all five groups RESULT 0 failures;55.98 FPS average,worst133.47ms,22 frames>25ms,planetary scenes >=24 FPS. All twelve paragraphs/notes fit; page cycling, identity reset and living-contact counts pass. Native dossier captures inspected, including corrected snapshot/footer spacing. SHA256 FE3ECF5D1A792863E682834F2D127A1B8343C08875DA71C7CBDF16817FB973AB. No extra assets/save changes; no commit/push/release. Highest remaining priority: physical PSP review and the earlier unresolved planetary shutdown investigation.

## 2.5.178 — GalacticNet scrollbars and clearer outfitting

This build also advances ordinary NPC positions/collisions every frame (AI decisions remain staggered), eases lock-on alignment near its bearing, reduces boost-only speed heat (coefficient 10 to 0.35; normal overspeed, solar heat and ENG cooling unchanged), and tightens the Weapon Computer from 352x150 to 272x120 with dark amber combat styling. Added motion-per-frame, lock easing and boost endurance regressions.

Spacebook, Inbox and Jobs now show a scrollbar matched to the visible cards, including partial final pages. Galactic Lore keeps SECTOR when selected. Outfitting uses STATS: and COSTS:, removes ONE MODULE PER SLOT, and names the destination slot beside empty/replaced modules. Missiles now accept any living ship target, not just hostiles, while retaining range and existing legal consequences. The bottom Circle hint includes missile/flare stock. Prices, fitting rules and saves are unchanged.

Local-only change; preserve existing dirty work. Verified ../../work/smoke-20260927-141110-131: all five groups RESULT 0 failures, 55.45 FPS average, worst150.15ms,25 frames>25ms; surface >=24 FPS. Every-row scrollbar pixel tests/wraparound, all-faction missile locks, actual-dt NPC motion, easing/no-overshoot and four-second boost heat checks pass. Native first/last feed pages, outfitting, compact weapons panel and FLARES count inspected. SHA256 EAD60F0E866B8D7F1CF59D81FE9FF222BB6E7B49A6C6753322A9BEE1E6833D27. No assets/save changes. Local-only; no commit/push/release. Highest priority remains physical PSP landing/shutdown retest from earlier builds; this UI-only change does not address that unresolved report.

## 2.5.177 — Weapon selector / Back isolation / explicit auto-dock (local-only)

Circle tap now activates selected tool on release; hold>=0.20s opens selector only while held; Circle+direction equips and closes immediately without activation. Selection release/long-hold release never fire. Removed both headings, footer names selected tool. All FLIGHT transitions impose release guard so Back cannot become weapon input. FLY Disembark visible only docked (stable ID20). Triangle station hail now opens canonical Comms at REQUEST AUTO-DOCK, explicit X starts guidance. Deliberate manual aperture entry remains unchanged. Relevant help/tutorial/outfitting/mission text updated. docs/UPDATE-2.5.177.md.

Final green ../../work/smoke-20260927-135749-688: all five groups RESULT0 failures,55.98FPS avg,worst133.47ms,22 frames>25ms,surface>=24FPS. Full tutorial/rescue/mission/navigation regressions pass. New tests exercise held-repeat, long-hold release, selection no-fire, fresh tap activation, four selections, Controls/Comms/Radio/GalacticNet Back (including GalNet->HOME->FLIGHT), explicit station request and dock-only visibility. Native selector/footer inspected; no removed captions. Earlier final run found overly long docking module text and a test assuming GalNet returns directly to FLIGHT; shortened text and tested actual two-level Back. No thresholds weakened.

Source/tested/dist/delivery SHA256 94C44F68CD12E568BCBD1A88A4CC51A3CFAF3FC78712A00FEB42379E934810BE. Delivered current-thread outputs/ArcElite-v2.5.177/EBOOT.PBP. No extra assets/save changes. Physical PSP input test still recommended; earlier planetary hardware shutdown remains unverified. Preserve existing dirty work; no pull/autostash/commit/push/release.

## 2.5.176 — Secondary tools and Triangle context actions (local-only)

Implemented approved provisional controls: Circle opens/holds D-pad tools; Up missile,Down rear decoy,Left fitted ECM,Right fitted heat sink. No tap discharge, held-repeat spam, ambiguous-direction firing or steering leakage. Triangle uses selected target for NPC hail,cargo tractor,anomaly scan,docking or landing clearance (X confirms/Circle cancels). Menus/ground/cutscene safety preserved. Old L+X remains compatible. New files flight-tools.h,flight-tools-ui.h and matching model/input tests. Documentation/balance in docs/UPDATE-2.5.176.md. Chaff upgrade improves rechargeable decoy bank; ECM costs shield energy; heat sink is now manual. Tool state transient, no save-format change.

Final green ../../work/smoke-20260927-134021-503: all five RESULT0 failures;55.98FPS average,worst133.47ms,22 frames >25ms,surface >=24FPS. Tests cover all chords, costs/cooldowns, rear/early/timely decoys, missing modules, opposite steering retained, no repeated discharge, confirmation and existing tutorial/rescue/mission/landing chains. Native weapons-pad.bmp inspected at480x272. Previous red runs were outdated Circle-lock/tracking wording assertions; updated to intentional controls and explicitly retained notice-dismissal test. No thresholds relaxed.

Tested/source/dist/delivery SHA256 D78AE30E657B753934E4E498769A0866EEB67B2165F844AD3C9688B720F9A841. Delivered current-thread outputs/ArcElite-v2.5.176/EBOOT.PBP. Includes v175 labels/missile icon. No additional assets. Next: PSP playtest new gestures and balance; prior planetary power-off remains unverified, request landing trace/save/hardware details if recurring. User explicitly considers assignments provisional. Preserve all dirty work; no pull/autostash/commit/push/release.

## 2.5.166 — Power-bank visibility / sixteen themes (local-only)

## 2.5.175 — Flight labels / missile indicator (local-only)

Completed flight HUD changes: hold-Square footer L:NEXT/R:LOCK with shoulder glyphs, Triangle COMMS without HOLD:, missile icon/count beside WEP instead of MS in bottom controls. ENG/WEP spacing now36px; existing highlight dimensions retained. Updated theme-coordinate assertions and added native label captures/hint pixel assertion. Details docs/UPDATE-2.5.175.md.

Final green ../../work/smoke-20260927-132438-237: all five groups RESULT 0 failures,55.84FPS average,worst133.47ms,23 frames >25ms. Native flight-control-labels.bmp and target-control-labels.bmp inspected; missile4 sits separately to right of WEP, footer hints fit. Tested/source/dist/delivery SHA256 FF08E3B1CB638E6FAE041CB8CFD1153440C7DC6B322C40D17ED4C07704CFFAB1. Delivered current-thread outputs/ArcElite-v2.5.175/EBOOT.PBP. No control/ammo/save changes. No pull/autostash/commit/push/release.

New user request: redesign Circle as secondary-weapons D-pad overlay, Up missile, Down rear flare, explore Left/Right/tap actions, cargo collection via Triangle. Read-only inspection so far: Circle currently also handles planet approach, docking, anomaly scanning, NPC selection; ECM/chaff and heat sink exist but trigger automatically. Need agree remaining mapping and keep context actions/landing safeguards. No Circle redesign implemented in v175.

## 2.5.174 — NPC overlap / close combat (local-only)

Confirmed code defects: combat slows to 40 m/s below 200 m while steering at the enemy centre; no light-NPC separation; normalized linear steering fails for exactly antiparallel directions. Added npc-steering.h with rate-bounded turning, predictive nearby traffic avoidance and three bounded overlap recovery passes. game.c uses a <450m breakaway and 300m/s combat cruise; firing alignment uses original attack vector, not avoidance. No save/model layout changes. Capital berth solver untouched. See docs/UPDATE-2.5.174.md.

Final green ../../work/smoke-20260927-131810-501: game/input/steering/radio/performance all RESULT 0 failures. Average55.45FPS, worst150.15ms,25 frames >25ms; surface >=24FPS. New npc-steering-tests.h reproduces coincident/overlapping/head-on patrol-pirate pairs, verifies clearance/continued movement/finite direction at60/20Hz, exact180-degree turn and reward/legal/health/berth neutrality. Independent NPC kills, police and freight suites also pass. git diff --check clean.

Tested/source/dist/delivery SHA256 8B41508854B53D67279184587645E73F52D1F69B1D5B9827564D6D4597BC7445. Delivered current-thread outputs/ArcElite-v2.5.174/EBOOT.PBP. No extra assets. Next: physical PSP close-combat/traffic soak, especially dense encounters; prior planetary power-off remains unverified and needs landing-trace/save/hardware details if recurring. No pull/autostash/commit/push/release; preserve existing dirty changes.

## 2.5.173 — Restored cinematics / planetary polish (local-only)

Restored unskippable arrival/touchdown/departure visuals using independent PlanetShot camera (no writes to live Game pose/surface). Retains v172 automatic disembark, input consumption, release gates and parked Triangle-board/R-launch/X-exit flow. World-plane garage/POI signs, five varied sky traffic lanes, eased shortest-bearing Square+R turn, arrows beside target name, compact icon HUD and green Triangle over compass ship. Shared Triangle/Cross colours corrected. Details: docs/UPDATE-2.5.173.md.

Final green: ../../work/smoke-20260927-111131-724 (-SurfaceCapture). All five groups RESULT 0 failures; avg55.45 FPS, worst150.15ms, 25 frames >25ms; surface >=24FPS. Full tutorial and real-input landing flow pass. Guarded 10-hull x 4-body intermediate touchdown rendering passes; smooth-turn and moving/distant traffic assertions pass. Native cinematic/compass/tracking/garage captures inspected. First closeup showed mirrored wall signs; wall tangents corrected and final capture GARAGE reads normally. No thresholds relaxed.

Highest priority remains actual PSP retest for reported power-off: NOT reproduced or proven fixed. Retain bounded landing-trace.txt; ask for it plus commander save, PSP model/firmware and hull/body if it recurs. Sky traffic is presentation-only. No extra assets/save changes. Source/tested/dist/delivery SHA256 D236DBD6EAABE132B7A28A7A8D5CDB4A9295055E2F0332A099D28C9D51183821. Delivered current-thread outputs/ArcElite-v2.5.173/EBOOT.PBP. No pull/autostash/commit/push/release; preserve dirty work.

## 2.5.172 — Explicit landing/boarding/launch flow (local-only)

v171 did not resolve user's physical PSP disembark shutdown. User requested automatic exit after landing, Triangle to board, R to launch and explicit screens. Implemented guided approach -> timed touchdown -> automatic disembark; buttons cannot skip/chain actions. Surface Triangle beside ship boards; parked screen requires release before R launch or X exit. Circle no longer toggles EVA. Separate model actions validate state/exit before commit. Transfer screens use only 2D UI: no temporary camera/surface mutation. Details and limitations: docs/UPDATE-2.5.172.md.

Final green run ../../work/smoke-20260927-104637-599 (-SurfaceCapture): all five groups RESULT 0 failures. Average 57.47 FPS, worst 133.47ms, 11 frames >25ms. Full tutorial completes. Real input and guarded 10-hull x 4 Lave-body rendering/transition sweeps pass. Inspected native landing and parked control screens. Earlier old test loop used 16 ships while only 10 exist; changed it to player_ship_count and added runtime index guards. This test bug is not evidence for the user's hardware fault.

Bounded landing-trace.txt now records airlock/first-EVA-render/first-EVA-tick/board/launch stages with remaining stack, resets at arrival or 64 entries. Disabled during smoke. Highest priority: physical PSP retest. If it powers off again, obtain landing-trace.txt and commander save plus PSP model/firmware and exact ship/body. Do not claim the shutdown cause identified or verified fixed. Never equate new button flow with proven crash repair.

Tested/source/dist/delivery SHA256 88176FBB4629FAA30AB71E073237966BA84F27DF085927AE9BD251A48CA6A265. Delivered current-thread outputs/ArcElite-v2.5.172/EBOOT.PBP. No new assets or save-format changes. git diff --check clean. No pull/autostash/commit/push/release; preserve all dirty work.

## 2.5.171 — Landing controls / reported PSP shutdown (local-only)

User confirmed Lave and Circle during the landing animation. Confirmed defects: planetary Triangle bypassed message dismissal; animation skip returned 0, letting the same press run through flight controls. Corrected close/skip ownership, explicit landing-to-EVA transition, sequence-state validation and cinematic mesh bounds. Boarded notice explicitly says LAUNCH, preserving single-Triangle departure. See docs/UPDATE-2.5.171.md.

Final green run ../../work/smoke-20260927-102640-018 (-SurfaceCapture), all five groups RESULT 0 failures. Average 56.51 FPS; worst 150.15 ms, 17 frames >25ms. New real-input tests cover natural entry, close/held buttons, arrival Circle skip without relanding, pad skip/disembark and stale state. Guarded rendering sweep covers every player hull and all four Lave bodies; no observed invalid positions/framebuffer boundary writes. Native pad-alignment capture inspected. Tutorial test originally relied on skip+land in one press; now completes arrival before the separate landing action and entire tutorial passes. No relaxed thresholds/deadlines.

Physical PSP shutdown NOT reproduced or proven fixed. Highest priority is user hardware retest; if persistent, obtain commander save, ship/body, PSP model/firmware and a hardware fault trace or reduced-rendering reproduction. Do not claim hardware crash eliminated based on emulator smoke alone.

Source/tested/dist/delivery SHA256 8743AB145E4673085D42178B166E8C1F5065FCC7659FD65BA9247BBF2C93FE62. Delivered current-thread outputs/ArcElite-v2.5.171/EBOOT.PBP. No new assets or save-format changes. git diff --check clean. Preserve dirty work; no pull/autostash/commit/push/release.

## 2.5.170 — Radio label (local-only)

src/radio-ui.h: tuned-in LOCKED becomes NOW PLAYING: (11 characters, fits the existing 162px dial). STATIC branch, duration, RADIO OFF and channel naming unchanged. Build and all five smoke groups passed in ../../work/smoke-20260927-101116-750. Tested/source/delivery SHA256 D3DFE820E2A0C544619AC3FFB2724F8AA3CC56DBE104A872822B809585D4AF95. Updated dist and current-thread outputs/ArcElite-v2.5.170/EBOOT.PBP. git diff --check clean. No new assets or save changes; no push/release. Existing physical PSP QA priorities below remain.

## 2.5.169 — Outfitting and mission integration (local-only)

Final green run: ../../work/smoke-20260927-100754-706 (-SurfaceCapture). Game/input/steering/radio/performance all RESULT 0 failures. Average 57.06 FPS; worst 133.47 ms, 14 frames >25 ms. Inspected native Outfitting, Loadout and mission card captures; removed an overlapping Loadout label and unsupported punctuation in briefs. Source/tested/dist/delivery EBOOT SHA256 DCB6CCF28C2525C30C81A163DDAF90E22380B620CCED476C86E03E181DE819A6.

See docs/UPDATE-2.5.169.md for file-level behavior, coverage and limitations. New tests cover 1280 real contract completions across all 256 systems, all-system station stocks, every module transaction/save/load/sale, protective capacity/passenger guards and actual simulation effects. Integration caught stale Guild lookup; guild.h now matches five offers everywhere. Removed actual board expiry, not just countdown labels. Legacy duration fields remain for V18 compatibility.

Earlier failures: old prosperity/expiry assertions replaced with new-rule assertions; Chandler test needed enough credits for its advertised military shield; Guild input assertions depended on old missing-food-board logic. Final suites passed without relaxing frame-rate thresholds or smoke deadlines. Delivered current-thread outputs/ArcElite-v2.5.169/EBOOT.PBP and updated dist. No extra assets required; no pull/autostash/commit/push/release. Preserve all existing dirty work.

Highest-priority remaining work: physical PSP soak test, especially saved Heat Buffer/loadout, long flight and input/audio; manual full-flight mission journeys beyond deterministic handler coverage; broader mission archetypes and persistent board progression remain future scope. Do not downgrade saves containing Heat Buffer to old builds.

Final green run smoke-20260927-090613-119 (-SurfaceCapture): all five reports pass; 57.06 FPS average, worst 133.47 ms, 14 frames over 25 ms. Earlier run smoke-20260927-090433-126 caught purchase/reload failures from paint_read's old i<8 whitelist; corrected to DECORATOR_COUNT. Inspected highlight captures and both finish pages. Delivered current-thread outputs/ArcElite-v2.5.166/EBOOT.PBP and updated dist; source/tested/output SHA256 2C3DCC6E8C27C8B1D8F50EFC283D5E746770514416A25D52ABB7A6B4721AEF6B. git diff --check clean; physical PSP QA still needed.

Root causes: active bank used only UI_ACCENT vs UI_MUTED (weak distinction on some themes); scenic cockpit returned immediately and minimal/scenic space dispatch omitted cockpit; grid text rounded bank x positions away from their pip bars. voyage.h now draws a fixed pale selection rectangle, dark label and pips at exact pixel coordinates and a footer with the selected bank name. paused bypasses hidden-HUD early return; space dispatch includes cockpit while paused. Release leaves hud_mode/hud_hidden unchanged. Power allocation logic unchanged.

DECORATOR_COUNT=16 and DECORATOR_PAGE=8 centralise finishes/names/fees in main.c. Original eight colours/fees retained; eight added palettes in ship-theme.h meet existing contrast checks. Settings validation accepts all sixteen; current 72-byte paint preferences format unchanged. Older builds will not accept newly added colours, so avoid downgrade after using them. deck-ui.h pages list and swatches together and shows FINISH nn/16, keeping v165 text positions.

power-theme-tests.h checks one visible readable selected bank across 16 themes x 3 HUD modes x 2 contrast settings x 3 banks; real Start/right/release scenic dispatch; page crossings/wrap; purchase fee/free reselection for each new finish; persistence of all sixteen colours. Existing ship-preview contrast/uniqueness checks expanded to sixteen. Captures power-selection-theme-00..15 plus both decorator pages. Preserve dirty work; no push/release.

## 2.5.165 — Decorator alignment (local-only)

Verified smoke-20260927-085835-803: all five report groups pass. Inspected native decorator-label-alignment.bmp; labels fit their boxes. Delivered current-thread outputs/ArcElite-v2.5.165/EBOOT.PBP and updated dist EBOOT. Tested/source/output SHA256 E1431335516ACE6608687113D77666F6E6F31D509C153ACB9EFC9E298B901734.

deck-ui.h: URL (108,26) -> (118,29); PAINT FINISHES grid (14,11), equivalent to (112,88), -> pixel (115,93); units grid (40,30), equivalent to (320,240), -> pixel (330,243). All use text_px, not rounded text-grid coordinates. Main smoke captures decorator-label-alignment.bmp. Paint fees/actions unchanged. No push/release; preserve previous dirty work.

## 2.5.164 — Wider systems / hot suns (local-only)

Final all-green run ../../work/smoke-20260927-085519-640 (-SurfaceCapture): 56.51 average FPS; planetary 59.94/42.81/31.55/54.49. Worst frame 150.15 ms, 17 over 25 ms; real PSP QA outstanding. Galaxy sweep minimum body clearance 53,934 m; 749 long freight routes, maximum leg 105,027 m. Added actual trader-return and explorer-waypoint progression checks. Inspected four warp bearings, all eight warm stars, Lave close-up and capital trail capture. Source/tested/output/dist SHA256 BEE69D39CE0F7189C4D7DCB362E802D33B893F4A50C96EE2D9D4D4C7C65B28B8. Delivered current-thread outputs/ArcElite-v2.5.164/EBOOT.PBP, README.txt and TEST-RESULTS.txt. No extra assets or save reset required. git diff --check clean.

Preserve dirty work; no pull/autostash/push/release. sectors.h keeps all body seeds, types, radii and surface identities but replaces clustered templates with four quarter-sector planet bearings (seed jitter, 42–73 km base radii, 0.92–1.11 scale); star in a more distant offset sector. Main station stays at STATION_Z for established docking/campaign logic; secondary hub_position offsets enlarged. system_arrival uses origin+destination hash, 15–20 km full-circle inner-system positions and separately hashed headings; frontend already clears autoaim after warp.

freight_route now selects planetary transfer gates (traffic_world_point plus bounded offsets), checks entire segments against bodies/hubs/other capitals, and retains physical inbound/service/outbound scheduling. Long-route completion regression uses actual route distance/cruise + bounded service allowance. Civilian traders use 1–4 outward world waypoints and 0 return-to-hub; explorers seek fixed reachable transfer points, not moving orbital targets. Capital LOD limit 55 km, small ships 7 km; aft trails visible to 55/18 km with existing bounded segment counts.

art-runtime.h reuses the eight existing animated 32px sheets for granulation, but maps the disc to bright warm emissive colours, replacing dark centres/transparency. Corona and short flares are clipped additive pixels. Sun tint palette warmed for consistent lens/atmosphere effects. No new raster assets, allocations, populations or save fields.

Tests: system-spread-tests.h sweeps 256 systems for >30 km body clearance, safe deterministic entry, front/rear planet bearings, non-forced station view and long freight routes. system-spread-visual-tests.h checks eight luminous animated centres, clipped corona, real warp input, four arrival headings, Lave sun close-up and capital route captures. Final verification and delivery details follow below.

## 2.5.163 — Planet transition/compass follow-up (local-only)

Final all-green run: ../../work/smoke-20260927-084354-443 (-SurfaceCapture). Average 54.68 FPS; planetary 42.81/37.46/29.97/42.81. Worst frame 133.47 ms; physical PSP QA still needed. Inspected external lift/tilt/star phases, pad descent, compass/garage and control-help captures. Source/tested/delivered/dist EBOOT SHA256 A8F3377D8CE99989FC81A18DC7B7E3C0601311C9B24A4723D9C6B368679F7785. Current-thread outputs/ArcElite-v2.5.163 includes EBOOT, install notes and test report. git diff --check clean. New camera follows the ship before pulling back, preserving visible lift/tilt/burn. Contract text uses displayed site 4 for internal id 3.

Preserve the existing dirty worktree; no pull, autostash, commit, push or release. New planet-sequence.h uses transient Game phase/time/anchor: 1 arrival, 2 pad descent, 3 departure. Only real flight input starts presentation; model takeoff remains usable for simulations. The renderer temporarily positions a chase camera and restores all physical camera/model fields. Departure input is consumed until auto-orbit or Circle skip; boarding and disembarking clear old phases. No serialized state added.

Valid landings snap to pad centre with yaw zero; rover parks at (-110,-10) in the shared field_garage_walls open-front structure. Shared wall footprints prevent walk/rover penetration. Garage jump ceiling leaves near-clip clearance. Surface near clip reduced from 15 to 4 (allowed wall clearance is 8); inverse-depth coefficient 262140 distinguishes nearby surfaces. Buildings use bounding-sphere culling and render undersides where relevant.

R gestures are transient Game fields, reset on init, menus, cinematic entry and targeting. Release before 0.20 seconds requests one grounded impulse; hold runs at 100 vs walking 62. Ground/air latch stays in model. Ship/rover compass icons replace letter markers, field_nav_number maps nine non-rover sites to 1–9; selected bearing wins overlap. Cardinal letters use a separate row. Computer/guide/building labels share mapping.

Regression files: planet-sequence-tests.h (three real arrival/land/EVA/board/one-press-launch cycles, active speech, automatic orbit, pad/garage positions, run gesture, nav labels); planet-sequence-visual-tests.h (camera-state restoration plus arrival/landing/departure, compass/garage captures); surface-closeup-tests.h (24 wall/corner/pitch views and near-depth ordering). Pre-fix close-up tests failed twice; first corrected full suite passed in smoke-20260927-083744-548. Final verification follows below when packaged.

## 2.5.162 — Planetary controls/field sites (local-only)

Final all-green run: ../../work/smoke-20260927-082240-402 (-SurfaceCapture). Average 54.30 FPS; planetary 42.81/39.96/29.97/41.93. Inspected final left panel, persistent tracking and readable wall-mounted RELAY sign. Tested/output EBOOT SHA256 726F7E29BD238DB0E31272EBC8A1D1643F8BAF49581FF253200747831AFD8FC6. Delivered to current thread outputs/ArcElite-v2.5.162 and dist/ELITE-NEXT-PSP. No push/release; physical PSP QA remains the highest-priority validation.

surface-targeting.h now uses a 192px left panel. R sets yaw/pitch toward the actual contact without translating the player; four-corner marker, persistent name/distance and off-screen bearing supplement the compass. R held while closing the panel remains consumed until release (input dispatch preserves the jump latch). FieldBuilding from field_site_building is shared by renderer/collision; widths 44–64m, depths 40–48m, heights 38–72m. Buildings gain visible glass bands, doors and depth-tested nearby wall signs. Per-frame site caches bound work and keep decorative props out of footprints; collision uses the same exclusions. Added direction, retained-lock, right-view-clear and R-carryover regressions plus captures.

Cause: planet_tick applied +220*dt acceleration every frame while boost/R was held, clamping at 120m and restarting thrust at the cap. Now a transient Game.eva_jump_held latch accepts only a new grounded R press; it sets vertical velocity to 62, then gravity 140 applies regardless of hold. Approximate flat-ground arc is <1 second and <18m at tested timesteps. Air presses are consumed, not queued; holding across landing cannot retrigger. Spacecraft boost and rover suppression are unchanged. Save V18 uses an explicit payload and does not serialize the latch.

Regression coverage: planet-eva-tests.h checks six-second hold, height bound, landing without release, mid-air double tap, boarding and fuel preservation. planet-eva-input-tests.h reproduces double-tap/hold through the PSP input dispatcher and verifies release/grounded repress. Baseline is the tested v2.5.161 build from smoke-20260927-000524-073. Preserve existing dirty changes; no pull/autostash/commit/push of unrelated work.

## 2.5.161 — Surface expansion (local-only)

New planet-profile.h is the stable seed/type/species/site model. field-sprites.h keeps 8x4x20x20 palette indices (12.5 KiB), generated on world change. field-guide.h owns Start-on-foot. surface-targeting.h owns held-Square categories, selected-species scan, tracked marker; no spacecraft controls fire here. Tap scanning happens on release. Codex planet records use actual per-world survey bits.

planet.h uses surface-only 16-bit depth (255 KiB), scanline 2px triangle spans, upright sprite plane depth and back-face culled boxes. Other scenes retain their renderer. Fixed bounds, no per-frame allocation. Rover support height is shared across chassis/wheels. field_port_buildings supplies common visible/collision dimensions. Traffic is cosmetic, deterministic 150s cycles, two staggered pads; reserved pads block walkers. Large port grounds shelter hazard within 280m. Physics movement uses <=4m substeps. Safe ship/rover exits choose clear positions.

Save V18 appends float world_clock bits after V17 flags, before CRC; validates finite [0,86400). V1–V17 default clock 0. Historical V7/V14 fixture truncation updated. Activities 7–9 use bits16–18; old flags retain meanings. Surface position, rover and traffic are not saved (docked checkpoints).

Limitations: 1,400m rocky patches/420m island-platform shore; shared activity templates and procedural artwork, no bespoke full planets, weather simulation, building interiors or persistent traffic NPCs. Terrain stays flat through the port foundation. Decorative scenery is separate from eight survey species. Remaining hardware QA: steep camera angles, all ship sizes, forest density, long sessions and PSP-1000 memory/performance.

Final v2.5.161 all-green run: ../../work/smoke-20260927-000524-073 (-SurfaceCapture). 55.19 average FPS, planetary scenes 46.11/39.96/29.97/46.11. Final reciprocal-depth scanline renderer avoids per-block division; palette shading is per species, sprite depth encoded once per row. Collision checks are skipped when stationary. Inspected final rover/port/forest/targeting captures. All 1024 seed/site checks and all 16 hull exits pass. Delivered EBOOT SHA256 136889C3E2F9D0D00B9AF32B979A8C4100E3129BAE916CE07C0A5589DED4E272. Output: current thread outputs/ArcElite-v2.5.161. Preserve the existing dirty tree; no push/release performed. Physical PSP QA remains outstanding.

## 2.5.160 — Save slots/profile + bounded surface exploration

commander-ui.h owns the STATUS input/screen, cached slot metadata, name keyboard and confirmations. tutorial-runtime.h dispatches it while preserving tutorial modal priority and TU_VIEW/TU_SAVE events. Main menu has commander and tutorial panels; LOAD opens three cards. Manual paths are commander.sav / commander-2.sav / commander-3.sav; tutorial.sav remains separate. Delete removes only selected primary/bak/tmp after explicit confirmation. load resets frontend targeting and rare-hail state. UI renames are in-memory until saved. Profile is also visible in player dialogue and Spacebook.

game.c appends V16 name[25]/portrait u32/bounty[256] and V17 256*BODY_COUNT little-endian u32 activity flags. V1–V15 default to JAMESON/portrait 0, later fields zero. CRC, validate-before-commit, backup recovery and failed-load nonmutation remain. Historical fixture lengths updated, buffers enlarged to 16384. Current save version is 17. Docked checkpoints restore at station, not arbitrary mid-flight coordinates.

surface-activities.h provides deterministic port/relay/cache/ruins/observatory/rescue positions and finite rewards. Flags 0 job accepted, 1 relay, 2–5 sites, 6 job paid, 8–15 life scans. Rover state is transient (docked save policy), persistent completion is per system/body. X enters/parks, Square interacts, L+Triangle cycles target, Triangle faces ship. Existing L+X weapon chord cannot board the rover. Surface==2 is retained in rover mode so existing landing/boarding guards remain; dismount before boarding ship. Ground-following movement is capped, with 1400m field boundary, dry ocean shore and site/hut footprints. Cabin shelters exposure, outside exposure slowed to permit exploration. Port job lives in this local interaction flow, not the station mission log.

planet.h replaces house skyline with seeded ridges, adds world-bearing clouds, near-camera terrain coverage, player-local seeded flora/decorative fauna, bounded site/rover geometry and compass. Uses existing biome sprite-family mapping; no new bitmap assets or streaming. Limits: bounded patches, small exterior port, no town interiors/full planet traversal; procedural scenery is simpler than authored station art. Save flags are compact; geometry/prop counts stay bounded.

Regression run ../../work/smoke-20260926-232113-027 passed game/input/steering/radio/performance; 55.19 average emulator FPS, planetary scenes >=24 FPS. Final versioned build/capture results recorded below before delivery. New tests: commander-input-tests.h and surface-activity-tests.h. smoke-test.ps1 now accepts -SurfaceCapture and allows 120 seconds for growing migration/checksum coverage; FPS thresholds unchanged.

Final v2.5.160 EBOOT passed all five groups in ../../work/smoke-20260926-232554-639 with -SurfaceCapture. Inspected title/three-save-cards/name/status screens, on-foot boarding, looking up/down and forest/ice ground views. Parked player hull now renders solid with fitted paint. Output EBOOT hash matched the tested source build. git diff --check passes. Deliverable is in the current thread outputs/ArcElite-v2.5.160.

Highest priority: physical PSP check of save/backup/rename/portrait/card/delete flows, all planetary look angles, rover/shore/POI navigation and Wanted crash regression. Validate long controller sessions before calling surface glitches resolved on hardware. Preserve the unrelated dirty-tree work. Local build only; no remote tag/push in this turn.

## 2.5.159 — Quiet watch and non-conversational computer notices

incoming_reply_ready() in flight-extras.h gates encounter replies on active human speech and absence of a computer notice. main.c, comms-panel.h and voyage.h use this instead of stale encounter state. Triangle dismisses range/fuel/landing notices; it cannot start a conversation with them.

quiet-hails.h adds three frontend session-only civilian conversations, using the standard paginated dialogue layout. An actual idle nearby non-freighter trader calls after at least 480 eligible peaceful seconds; subsequent cooldown is 600–900 seconds, with a 20-second invitation. Tutorials, quiet chatter, combat, approach, docking and jumps suppress calls. Caller loss/system change/interruption cancels safely; an unrelated replacement transmission is not erased. Stories cycle before repeats. Optional cargo quote/consent reuses trader_offer_hail with 2500m range and identity validation. No mission generator or save-format change.

quiet-hails-tests.h covers stale encounter notices, Triangle dismissal, cooldown/combat suppression, all three story paths and explicit cargo exchange/no duplicate rewards. Native quiet-watch story captures are generated alongside existing graphics tests. Initial build passed all five smoke groups in ../../work/smoke-20260926-225901-088; versioned build verification recorded below before delivery.

Final v2.5.159 build passed all five groups in ../../work/smoke-20260926-230058-710 (game, input, steering, radio, performance: RESULT 0 failures). Native quiet-watch-0-1 and quiet-watch-2-2 captures inspected: full story/answer and three replies fit the shared layout. git diff --check passes.

Highest priority: physical PSP check of notice dismissal, invitation timing, long-text L/R paging and trade roundtrip; preserve outstanding Wanted crash hardware retest from v157. Session-only cooldown resets on application restart. Existing unrelated dirty-tree work is preserved; this update is local, not pushed or tagged remotely.

## 2.5.158 — Painted menu hull and interface palettes

ship-theme.h provides eight fixed palette sets keyed by the current hull's fitted colour. Shared GUI colour roles are used by menu chrome, deck, standard UI, conversations and voyage HUD. menu-ship-preview.h lights the actual paint colour; no frame-wide tint of planets, portraits or paper. paint_save/load uses a separate validated cosmetic file with backup and transaction rollback before charging. Cosmetic preferences are per hull type within this installation, shared by commander/tutorial as the previous frontend paint array was.

Tests capture every finish in home and flight, check readable palette brightness separation, distinct regular preview pixels and colour/theme reload. Physical PSP visual check remains outstanding; preserve v157 Wanted crash fixes and its pending hardware retest. New artifact v158 includes them. Earlier uncommitted work remains preserved; no remote publication in this turn.

## 2.5.157 — Wanted pursuit corrections and Gazette redesign

Fixed unsigned pirate spawn Z offset (could wrap to billions of metres), protected bounty NPC slots from traveller promotion and mission fallback replacement, validated bounty identity after launch, restored cockpit view and matching poster names. Target health meters now use risk-scaled maxima. Raster line rejection uses direct bounds rather than abs(INT_MIN). Hardware crash was reported by user; emulator does not reproduce a physical PSP shutdown, so do not claim hardware verification.

Added checks for all 1280 system/poster identities and local positions; five poster input/flight-render sequences including docked and in-flight selection, six seconds of simulation, firing and boost. Prior short assertion was insufficient to verify rendering. Gazette now has sixteen full-width article stops and a left scroll rail; puzzle uses 16px cells and pixel-aligned text. Posters have real edge cutouts, folds, paper texture and readable target status within each card.

Highest priority: test this uniquely versioned EBOOT on physical PSP. Existing working tree contains earlier uncommitted changes; preserved. No remote publication performed in this repair turn.

## 2.5.156 — SPACE TALK radio polish

radio-ui.h separates tuner number row 14, status 16, name 18 and genre 20; glass extends to y178. Station 6 labels are SPACE TALK / TALK RADIO; station ID, folder mapping and save format stay intact.

radio-synth.h replaces station 5's sample-count loop (65536 samples, ~1.5 sec), asymmetric throat waveform and boundary noise bursts with seeded phrase/syllable events, six alternating voice profiles, pitch contours and balanced triangle harmonics. Envelopes reach zero at both ends; word and phrase gaps output exact silence. PRNG updates only at boundaries; audio worker remains integer-only and allocation-free. The five music stations are unchanged.

Validation: final v2.5.156 build and all five smoke groups pass in `../../work/smoke-20260926-211228-988`; no new compiler warnings. Native 480x272 locked/static/off captures reviewed; final 0-6 dial labels checked after replacing cramped OFF notch text. radio-tests.h checks silent gaps, speaker/phrase variation, click bounds, metadata and existing stereo/headroom/reset rules, and exports radio-preview-6.wav. radio-input-tests.h captures locked/static/off states. Physical PSP listening remains outstanding; subjective sound approval belongs to the player.

Highest-priority remaining work: player listen to SPACE TALK and retest boosted rolls/docking handoff; deferred tutorial pacing; separate Heat Buffer catalogue/save-validation mismatch.

## 2.5.155 — Boost rolling and thermal alert separation

While boost owns R, L is a roll modifier, not slowdown/hard brake. game_input ignores/resets L tap timing while boosting, keeps boost during manual roll, and retains acceleration with R+L. Releasing R still stops boost; ordinary non-boost L controls and EVA remain unchanged. Boosted roll can retain pitch input. Help labels and README explain the chord.

player_damage_kind distinguishes thermal stress from an attack. Both critical-boost drain and full thermal runaway retain damage_fx, shield/hull drain and death behavior, but do not refresh attacked. Existing attack timers are preserved, so an actual attack/missile/impact remains visible even when boosting hot. No alert suppression based solely on boost/heat; no save changes.

Validation: baseline and v2.5.155 build pass all five smoke groups; final reports in `../../work/smoke-20260926-205230-638` (game/input/steering/radio/performance: zero failures). Added real input chord tests and thermal/combat-alert checks in journey-input-tests.h. Physical PSP playtesting remains outstanding.

Highest-priority remaining work: player boost-roll and docking-handoff retest on PSP; deferred tutorial pacing; separate Heat Buffer catalogue/save-validation mismatch. Preserve cockpit-frame steering and mission tracking/dialogue fixes.

## 2.5.154 — Cockpit-frame steering and docking handoff

Root cause: docking sets roll to station rotation, cancellation retained it, and launch reset yaw/pitch but not roll. The v150 world-axis steering change was not actually screen-relative; its yaw-sign test certified the wrong behavior. Atmospheric steering also had a partial Euler mix. `flight_steer` in game.c rotates the whole camera frame about local input axes and recovers canonical Euler storage with atan2 near the poles. Both free-flight branches use it; landed/EVA controls remain unchanged. No save-format change.

Circle cancellation clears docking phase/timers and hard brake without snapping position or orientation. Launch levels all three axes. Debug Return to station clears approach/docking/auto-aim and attitude. Removed the boost-release roll reset: now that roll carries actual frame orientation, forcibly zeroing it would cause a view jump. Preserve manual roll and rotating-station aperture alignment.

Regression evidence: baseline passed; new cockpit-space matrix failed on old code in both space and atmosphere, and the launch-roll test failed. Corrected build passes all five smoke groups in `../../work/smoke-20260926-204314-956`; final versioned release build also passes in `../../work/smoke-20260926-204508-384`. Added 192 bank/pole/direction cases, continuous full loops, and real-input cancellation of every exterior guidance leg at three station rotations followed by debug return/relaunch. Old yaw-sign assertion replaced by actual nose movement in the previous camera frame. Physical PSP testing remains outstanding.

Highest-priority remaining work: player retest of docking cancellation and repeated relaunch on real PSP; deferred tutorial pacing after speed/boost/brake; separate Heat Buffer catalogue/save-validation mismatch. Keep shared dialogue and Mission Log tracking behavior intact.

## 2.5.153 — Mission selection and readable instructions

Mission Log is the selection authority. X tracks; Select tracks and opens CAMPAIGN (Tracked Mission). Guild actions run there; Work service ID 18 is retired but other IDs remain stable. GUILD redirects to the tracked Guild; legacy STORY redirects to HOME and the frontend retires old coaching without rewards. The dedicated First Light tutorial remains intact and its Guild lesson now uses service 13. Tutorial pacing is still explicitly deferred.

Station Welcome now has a selectable log row after the contracts, mapping to stable frontend sentinel `TRACK_STATION_TOUR`. Its presentation/HUD/automatic route respect tracking. The eight-row maximum fits above the objective band. `mission-tracking-input.h` preserves a tracked contract's identity when another job is removed, using immutable contract fields. Tracking remains session-local as before; no save-format change.

First Flight has state-specific next-step dialogue and visible launch/resume/reward actions; no launch-generated fake target-lock event. Kei's four later briefings and associated aftermath have expanded prose. Saga local objectives explain scan/dock/hunt controls, with flight actions rather than same-system routing. Ch.05 choices wait for the evidence delivery. Keep shared `dialogue-ui.h` styling and orange replies.

Validation: final build and all five smoke groups passed in `../../work/smoke-20260926-203406-655` (game, input, steering, radio, performance: zero failures). Added `mission-tracking-tests.h` for log selection, tour isolation, job compaction, launch/docking instructions and full-log captures. Native 480x272 Work, full Mission Log, First Flight and Guild captures reviewed. Physical PSP playtesting remains outstanding.

Highest-priority remaining work: requested tutorial delay after speed/boost/brake practice (deferred); Heat Buffer catalogue/save-validation mismatch; real PSP control, readability and long-session testing. Do not reintroduce a separate Guild/optional coaching menu or automatic tracking overrides.

## 2.5.152 — Shared mission conversation UI

`src/dialogue-ui.h` owns all mission speech geometry, text paging, speaker presentation, objective/feedback band and response rows. Campaign (prologue, saga, coda, choices, epilogue, tracked Guild, station welcome, contracts), direct Guild and Triangle conversations call it. Do not add bespoke speech rectangles to these screens. `src/dialogue-visual-tests.h` captures 12 states in normal/high contrast and checks layout, selection, paging, safe punctuation and untracked-choice isolation. Existing gameplay actions are retained; no save-format changes.

Final build and all five emulator smoke groups passed: `../../work/smoke-20260926-200222-610` (game, input, steering, radio, performance each report zero failures). Native 480x272 captures reviewed across prologue, commander reply, saga decisions, direct/tracked Guild, contracts and Triangle channels, including high contrast. Physical PSP rendering/input remains unverified.

Highest-priority remaining work: tutorial timing explicitly deferred by the player. Allow a few seconds after double-tap R/L and speed exercises in a later pacing pass. Continue real-hardware playtesting; preserve separate tutorial/commander saves. Existing Heat Buffer catalog/save-validation mismatch remains separate. Legacy optional flight guide and paused First Light lesson presentation were intentionally not redesigned in this mission UI pass.

## 2.5.151 — First Light tutorial

Implemented `src/tutorial.h` (61 authored beats), `src/tutorial-runtime.h` (observed-action adapter and paused briefing), and separate tutorial save V15. Title menu explicitly offers Start/Continue tutorial and normal New/Load commander. Initial tutorial deck exposes only FLY; all other services unlock in sequence. Full script and coverage: `docs/FIRST-LIGHT-TUTORIAL.md`.

Validation: build + all five PSP emulator smoke groups pass (game, input, steering, radio, performance); reports in `../../work/smoke-20260926-193230-889`. Native 480x272 title/briefing/Fly-only/Spacebook/planet captures reviewed. The complete scripted input fixture passes all 61 transitions; physical PSP playtesting remains unverified. The incoming local comms/radio/loadout work is preserved separately in Git before the tutorial commit.

Highest-priority remaining work: human playtest pacing on PSP hardware; optional interactive lessons for advanced combat/mining/passenger contracts beyond the explanations already provided. Existing Heat Buffer catalog/save-validation mismatch from the prior local work is separate and still needs repair. Do not treat UI review lessons as completed purchases or mandatory contracts. Preserve regular `commander.sav`; tutorial uses `tutorial.sav` and dockside checkpoints.

## In progress 2.5.150 — Roll-independent flight controls

Flight yaw/pitch input is now independent of the visual roll axis and no longer mirrors yaw after pole crossings. This keeps UP/DOWN and LEFT/RIGHT uniform during rolls and loops. Build and smoke checks pass.

# ELITE: NEXT — DEVELOPMENT HANDOFF

## In progress 2.5.149 — Radio ticker alignment

The radio host face is five pixels lower and aligned to the ticker baseline. The equalizer is now a longer 28-bar rail centred below the ticker with slow station-colour crossfades.

## Completed 2.5.148 — Reticle-aware Square targeting

A quick Square tap silently selects the valid on-screen object whose projected centre is closest to the reticle. The left-side targeting computer appears only after a 0.20-second hold, and releasing a long hold preserves the selected target. Square+D-pad, Square+L and Square+R chords retain their existing behavior.

## Completed 2.5.147 — Cleaner replies and silent targeting

The in-flight reply chooser now shows only Ignore and Respond. Left/Right changes focus and X confirms it after Triangle has been released. Held-Square target browsing and Square+R locking no longer create computer chatter; the navigation overlay itself retains `(R = LOCK)` as the control reference.

## Completed 2.5.146 — Ship Decorator alignment polish

The decorator URL and bottom control prompt now use pixel-positioned text so they sit cleanly inside the monitor artwork. The active hull finish is permanently identified in the paint list with a gold swatch outline and small indicator, including while a different finish is being previewed.

## Completed 2.5.145 — native Ship Decorator monitor

The decorator now uses a generated, pixel-quantized 480×272 CRT plate with large bezel stickers and a fictional customization site. Code renders the selectable paint list and live ship inside exact blank windows. The art source is `assets/source/decorator-monitor-source.png`; regenerate the PSP plate/header with `tools/build-decorator-art.py`. Paint presets no longer mutate when a ship is repainted. Paint costs now match displayed units and all ten ship indices have a paint slot. Validate native captures before changing the plate layout.

## In progress 2.5.123 — Ship Decorator personality pass

Ship Decorator now feels like a playful custom spaceship workshop, with native-resolution signage, decals and clearer finish controls while retaining the live ship preview and purchase flow.

## In progress 2.5.122 — Comms Panel conversation controls

Flight chatter now labels one-way messages `△ CLOSE` and reply-capable messages `△ REPLY`; Triangle opens Talk/Ignore for replies, while holding Triangle opens the renamed Comms Panel. Display & Chatter settings are now inside that panel.

## In progress 2.5.121 — live decorator preview

Ship Decorator now renders the current hull in a dedicated top-right port using the highlighted finish, so every colour/pattern can be previewed before applying it.

## In progress 2.5.120 — targeting lock prompt

Held-Square targeting now labels the lock action as `(R = LOCK)` for an immediate, unambiguous control cue.

## In progress 2.5.119 — flight speed and approach validation

Speed lines now scale with cruise and boost velocity, and the speedometer shows the live numeric speed beside its bar. Relay docking tests now expect the visible approach sequence.

## In progress 2.5.118 — wanted docking interception

Dock requests now pause for a local-law intercept whenever the commander has a warrant, reusing the existing pay, custody and escape outcomes before the station approach may resume.

## In progress 2.5.117 — docking approach and debug tools

All station variants now use the visible docking approach/welcome sequence. Debug tools now include pulse-laser installation, local Codex reveal and heat/shield reset.

## In progress 2.5.116 — combat target lock

When the commander fires at a selected ship or freighter, the active target lock is enabled automatically so the HUD follows the contact being attacked.

## In progress 2.5.115 — archive progress and service balance clarity

Cosmic Archive categories now show a right-side three-file progress scrollbar, and ship loadout retains the fixed bottom-right balance badge.

## In progress 2.5.114 — weapon validation and shipyard stat checks

Mining regression fixtures now explicitly install a pulse laser, matching the empty-slot firing rule. Shipyard and flight continue to share the canonical per-hull capacity, speed, range and price values.

## In progress 2.5.113 — clearer commodity warnings

Commodity details now use a concise red restricted-goods warning, and the extra buy/sell advice line below price comparisons is gone.

## In progress 2.5.112 — weapon gating and outfitting labels

The flight loop now refuses laser fire when the WPN slot is empty and tells the commander to install a weapon. The outfitting list gives item names more room so Pulse Laser and Planet Scanner are not clipped.

## In progress 2.5.111 — bounded shipyard catalogue

The shipyard catalogue is now clipped to a seven-row left panel with a left-side scrollbar, and the lower-left bay explains the selected hull.

## In progress 2.5.110 — control icon and radio HUD cleanup

The cockpit radio ticker no longer draws a redundant `RADIO` word over the equalizer bars. Shared footer token rendering keeps D-pad directions as arrow glyphs and shoulder controls as L/R button icons, with Start/Select and face-button icons consistent across menus.

## In progress 2.5.109 — held-Square targeting guidance

The held-Square nav computer now shows only a distinct `R LOCK ON` instruction. The old D-pad/band/release clutter is gone, and the active scan category is written in full instead of clipped four-character tabs.

## In progress 2.5.108 — HUD and station service layout polish

Speed now sits below Fuel in the lower-right HUD. Third-person view is selectable under Display & Chatter. Ship Decorator and Engineers are separate docked entries in the Ship category, and the decorator supports scrolling finishes with a live painted preview.

## In progress 2.5.107 — World State v1, Mission Validation v1 and Lave certification

`src/world-state.h` now provides the canonical per-system snapshot. Mission offers are filtered through destination, station and landable-body validation before they appear or can be accepted. Lave certification checks cover peaceful danger, faction totals, reputation progression, planet identity and playable mission offers. See `docs/WORLD-STATE-V1-MISSION-VALIDATION.md`.

## In progress 2.5.106 — richer Discovery Codex system previews

The system archive now updates its right-hand panel as the selection moves. Station, star and discovered planet entries each show matching native artwork plus a concise summary; X still opens the full record.

## In progress 2.5.105 — ship decorator and third-person display

Stations now expose Ship Decorator in the Commander deck. Eight paid finishes update the rotating ship preview and the optional in-flight third-person exterior view. Commander → Display & Chatter includes a Third-person view toggle; the decorator's Triangle shortcut opens Engineers.

## In progress 2.5.104 — boost cooling, speed HUD and damage smoke

Boost heat now vents as soon as boost is released, even at high residual speed. The cockpit has a compact speed readout/bar above the power pips, and damage smoke is rendered as dim drifting particles with emission increasing as hull integrity falls.

## In progress 2.5.103 — clear cockpit radio ticker

The upper-right cockpit radio strip no longer prints `ALIEN`, which could collide with the scrolling ticker. Every station uses the compact `RADIO` identifier while retaining station colour and chatter.

## In progress 2.5.102 — native targeting computer rewrite

The full targeting page now uses the PSP-native 480×272 monitor layout: six visible scan rows on the left and a dedicated readout on the right for identity, status, range, faction/class and compact condition bars. Triangle reveals a short details line; X locks; L/R changes scan band. Flight yaw input now mirrors across the vertical pitch pole so left/right remains consistent after a full up/down loop. See `docs/TARGETING-COMPUTER-PSP-SPEC.md` for the canonical layout and checks.

## Released 2.5.101 — radio station/level navigation

The radio now shows only one STATIC label while changing stations. Up/Down moves between Music and FX levels, Left/Right adjusts the selected level, and L/R changes station; the footer documents these controls.

## Released 2.5.100 — quick-response comms

Incoming chats can now be ignored with a Triangle tap. Holding Triangle opens a compact in-flight Respond/Ignore overlay; Respond opens the full channel with Continue, Ask About This Encounter and End Channel options.

## Released 2.5.99 — planet survey records

Flora and Fauna are no longer top-level Discovery Codex sections. They are shown from each selected landed planet’s expanded survey record, alongside minerals and archive totals.

## Released 2.5.98 — cosmic Galactic Lore archive

Galactic Lore is no longer a Discovery Codex tab. The Discover menu entry opens a dedicated cosmic archive with six lore sections, 18 expanded entries, readable wrapped copy and PSP-friendly pulp-sci-fi cover illustrations.

## Released 2.5.97 — landing-gated planet Codex

Discovery Codex planet and system archive entries now appear only after a completed landing. Per-system landing flags persist in saves, and the ship computer gives a one-time confirmation when each planet is added.

## Released 2.5.96 — first-arrival system briefings

Warp completion now detects first-time system entry before marking the system visited. The computer gives a one-time system briefing with economy, government, technology, danger and Codex coverage; repeat arrivals remain concise.

## Released 2.5.95 — matching Discovery Codex planets

Discovery Codex now names and draws planets from the same deterministic system-body identity used by flight view, using names such as `Lave 1` and matching type, palette and artwork.

## Released 2.5.94 — readable Start/Select icons

Start, Select and L/R now use black rounded PSP-style mini-buttons with white pixel letters (`ST`, `SE`, `L`, `R`) for clearer recognition at a glance.

## Released 2.5.93 — unified PSP control icons

Added one shared 10x10 pixel renderer for all PSP face buttons, D-pad directions, D-pad cluster, L/R shoulders, Start, Select and the analog nub. Footer prompts now resolve these tokens to aligned, consistently coloured symbols.

## Released 2.5.92 — richer cockpit radio chatter

Expanded the five cockpit radio stations to 24 snippets each, with more adverts and alien talk-show comedy. Added a lightweight varied pause before each ticker line begins.

## Released 2.5.91 — solar-system Discovery Codex

Discovery Codex is now organized as a hierarchy: visited systems → station/star/world records → planet flora and fauna details. System pages also summarize mineral signatures, echoes and local life/traffic records.

## Released 2.5.90 — expanded Galactic Lore archive

Galactic Lore is now a first-class Discover menu entry beside Discovery Codex and GalacticNet. Its archive has twelve longer entries and a full readable detail panel. The station action is labelled `DISEMBARK`.

## Released 2.5.89 — PSP XMB identity artwork

Added `assets/xmb/ICON0.PNG` and `assets/xmb/PIC1.PNG`, and wired them into `pack-pbp` so the PSP XMB shows the Elite: NEXT icon and a space starfield background. Standard PIC1 artwork is static; the in-game starfield remains animated. A PMF/ICON1 animated XMB asset would need a PSP video encoder not included in the current toolchain.

## Released 2.5.88 — clearer radio tuning feedback

The radio page now uses a green Triangle glyph for power, and its station title changes to `STATIC` for the exact short tuning-noise interval between stations.

## Released 2.5.87 — visible hyperspace arrival

The destination system now fades in with a blue braking/arrival wash after hyperspace, revealing the new solar-system ship view smoothly while keeping the cockpit HUD sharp.

## Released 2.5.86 — ambient space encounters

Added a reusable low-cost encounter deck covering traffic, law, pirates, distress calls, cargo/wreckage, smugglers, bounty leads, and unknown contacts. Encounter transmissions use the existing full-screen comms panel: hold Triangle in flight, then choose Respond or Ignore. Responses hook into current trader, police, hostile, cargo, passenger, and credit systems.

## Released 2.5.85 — reliable docking, flight attitude, and warp presentation

Removed the unused cockpit background artifact, hardened Circle station docking and its third-person arrival path, cleared residual roll on boost release, and kept the ship view visible during the initial hyperdrive charge.

## Released 2.5.84 — functional power distribution

WEP/SYS/ENG pip allocation is now regression-tested across weapon output, shield recharge, and engine behavior.

## Released 2.5.83 — clearer mission and wanted HUD cues

Tracked mission instructions now render as green `>> ... >>` route cues. The opening Kei mission remains tracked by default and the tracking path is regression-tested. The cockpit hides the clear-state wanted counter and shows an escalating orange/red/flashing warning only while Law is after the commander.

## Released 2.5.82 — cockpit text radio

Far Horizons is the fresh-launch radio default. Flight view now shows a low-cost scrolling station-talk ticker beside the animated activity strip, with varied generated captions for all five stations. Radio tuning uses PSP L/R, the Music/SFX rows are the only Up/Down choices, and the radio screen has a visible Triangle power button.

## Released 2.5.81 — cleaner cockpit menu footer

The main cockpit menu no longer displays the navigation hint along the bottom edge; other screens retain their own footer guidance.

## Released 2.5.80 — menu wording refresh

Updated the requested main menu labels and subtitles, including `Explore station`, the new launch/upgrade/ship/loadout/control/radio copy, and the undocked `Cargo` label.

## Released 2.5.79 — law warnings and no-funds custody

Wanted commanders now receive a warning before arrest. If station custody is accepted with zero units, the game plays a jail transfer and law seizure sequence, clears credits/cargo/equipment/current ship, and releases the commander in a basic ship at the local station.

## Released 2.5.78 — readable Galactic Lore details

The Galactic Lore codex now places the selected topic and wrapped brief in a full-width bottom panel so longer entries are readable.

## Released 2.5.74 — Trader cargo offers

Trader hails now create a system-local commodity exchange for ordinary traders. The player is told what cargo to buy, and hailing the same named contact again completes the swap. Scanner identification is stored on the live NPC slot so the targeting computer retains the trader's callsign after the player returns from the market. The offer is session-local; commander saves remain V13-compatible.

## Released 2.5.73 — Kei replies directly

Kei's opening briefing no longer repeats the commander's selected speech in a
separate echo panel. Selecting a reply advances directly to Kei's response;
the existing reply choices and acceptance step remain intact.

## Released 2.5.72 — attacking enemies alert

While holding Square, the ENEMIES scan tab flashes red during active player
attack or incoming-missile states, then returns to its normal tab color.

## Released 2.5.71 — clearer held-Square target controls

The held-Square scan banner now shows `L TARGET IN FRONT` and `R LOCK ON`;
the old `D-PAD BANDS`/cycle-view wording is removed.

## Released 2.5.70 — unrestricted flight pitch

Space and atmospheric flight pitch now wraps continuously at ±π instead of
stopping at the old ±1.5/±1.2 radian limits. Holding Up or Down can complete
a full loop in either direction. Landed surface movement retains its normal
look limit. Build and all five smoke suites passed; physical PSP testing is
still unverified.

## Released 2.5.69 — animated cockpit activity display

The main flight cockpit now uses a small native-pixel activity display in the
top-center band. It cycles through stars, radar, telemetry, planet/galaxy and
ship schematic loops, with faster motion at cruise speed and boost streaks.
The existing system, wanted, mission, lower-instrument and combat HUD behavior
remains intact; high danger uses the display border/markers as a compact alert.
Build and smoke evidence for this release should be recorded below.

## Previously released 2.5.68 — development paused

Loadout and Mission Log fixes below are now packaged and smoke-tested in 2.5.68.
This supersedes their pending-release status. See docs/SMOKE-EVIDENCE-2.5.68.md.
User requested stop after delivery to conserve credits. Do not resume goals until asked.
Unintegrated station port and Lave fixture remain in specialist worktrees.

## Previously pending — full Mission Log layout

Main/side missions now form two vertically separated groups. All five contract
rows fit above an independent three-line objective area. Main missions no
longer advertise the inapplicable Abandon action. Selection/tracking indices
and job behavior remain unchanged. Two full-capacity native captures and a
framebuffer assertion cover the formerly hidden fifth contract.

All five smoke suites passed in `work/smoke-20260923-125242-382`.
Development EBOOT SHA256: `53607524418DB88A6AC4348EDC5CC4915212BB90E1B2303162D434B4F5704902`.
Normal full-log capture was visually inspected at 480x272. This supersedes the
earlier pending loadout binary while retaining that fix. No new package/tag:
the combined Lave/station release remains pending. The existing 2.5.67 release
does not contain these pending UI corrections.

## Pending combined release — loadout readability correction

Main now corrects loadout labels that used pixel positions as text columns.
The hull panel shows six short slot labels; the right panel owns module names.
In-flight help says to dock before changing modules. The internal release
number remains 2.5.67 pending the combined Lave/station integration; the existing
2.5.67 ZIP does not contain this pending source change.

Validation: all five smoke suites pass in `work/smoke-20260923-124844-899`.
Development EBOOT SHA256: `51885875FC631491078011628C885C70C5EE32D7CEE62E30EB60AF1EE09565C1`.
Twelve native captures cover every selected slot in both palettes; framebuffer
assertions require visible selected label glyphs. Normal DEF and contrast UTIL
captures were visually reviewed. Average 56.63 FPS, worst 33.37 ms, 23 frames
over 25 ms; this does not satisfy a stricter no-slow-frame gate.

Highest-priority remaining work: integrate the corrected station composer
without replacing newer main behavior; complete the Systems QA continuous
Lave traversal/capture fixture; review native Lave art quality and memory
evidence; then produce one combined, versioned release. v0.7 remains incomplete.

## Integrated 2.5.51 — Station Welcome mission

The current build adds a session-local onboarding mission once the opening campaign
is complete. It plots Lave → Reorte, then changes its objective as the commander
docks, disembarks, walks from Arrivals to **THE SECOND SHIFT**, and talks to
**Lysa Kest**. Existing station-bar interactions are reused; no save fields,
credits, rewards, or bar economy are changed. The cockpit cue stays short at the
PSP's native width (`JUMP: REORTE`, `WALK: CANTEEN`, `TALK: LYSA KEST`).

## Integrated 2.5.50 — native station visual rollout

The station crawl now uses a shared native 480×272 shell and an authored Reorte
primary Arrivals scene in the Second Shift visual language. The existing bar,
hotspots, options rail, speech area and SHIP return remain intact. Arrivals is
the first additional authored room; Market, Shipyard, Mission Board and GalNet
art families remain in rollout and must pass native 1:1 review before adoption.
Story & Dialogue owns station exploration writing and speech-box flow; footer,
help and chrome belong to the named PSP UI & Art managers. There is no separate
"Art3" team member.

The PSP UI & Art Manager is the department lead for visual direction and
production across the whole game. That role coordinates the Procedural Room
Composer and Reusable Element Kit across the thousands of first-person,
point-and-click station screens, along with planetary, ship, GalNet and other
game interfaces. The two specialist groups own their bounded implementations;
the manager owns cohesion, allocation and native 480×272 art acceptance.

## Manager operating contract

The PSP is the primary platform. Every manager designs and tests against native
480×272 first, including memory, framebuffer, input, audio and performance
limits. Larger previews are derivatives only. A feature is not complete from a
concept, plan or compile alone: it needs an integrated build, native normal and
high-contrast evidence, real input/state checks, truthful capability text,
save/economy safety and performance evidence. Generic fallback is unfinished
and must not imply unsupported gameplay. Managers coordinate before shared
edits and keep their ownership boundaries explicit; the Lead integrates and
releases.

## Project-wide native-art acceptance rule

All station artwork and station UI must be authored and judged at the PSP's
native 480×272 resolution. Larger exports are presentation derivatives only;
they must never be the source design that is simply shrunk. Every scene must
retain readable people, selectable hotspots, speech boxes, headings, page
indicators and SHIP/navigation controls at 1:1 pixels before integration.

Composition follows a practical golden-ratio hierarchy adapted to the small
screen: one clear focal area, a stable information/selection column, safe edge
spacing, and deliberate visual weight between art and controls. The ratio is a
guide for placing important elements, never a reason to make text or controls
too small. Native readability and separated hit areas always win.

## Integrated 2.5.49 — The Second Shift playable preview

Reorte's primary hub now has a playable illustrated bar reached through WALK,
ARRIVALS and CANTEEN. Lysa Kest, Pell Sorn and Dax Neral provide short dialogue,
Reorte I navigation help and free session-only High Orbit practice. The activity
does not write credits, cargo, jobs or saves; secondary hubs and other systems
retain their ordinary canteens. The PSP build completes successfully. The local
PPSSPP launcher produced no smoke reports during packaging, so physical PSP and
fresh-emulator interaction remain the immediate verification step.

## Integrated 2.5.48 — planetary EVA traversal

The specialist/planetary-eva checkpoint adds independent look/walk/strafe/jet,
grounded boarding, continuous shared terrain, shore/field bounds and clear ship
return/exposure cues. All five PPSSPP smoke groups pass (324 game / 309 input,
57.18 average FPS). EVA scene38 is 31.55 FPS versus 40.46 baseline: the full
viewport has a measurable cost. No physical PSP test or save-layout change.
See docs/PLANETARY-EVA-HANDOFF.md for evidence, scope and next activity/revisit
gaps. Lead owns integration and release. Story approved the coaching string.

## Integrated 2.5.48 — current-ship preview

specialist/menu-ship-preview branches from ac8a581. See
docs/MENU-SHIP-PREVIEW.md for files, reserved interfaces, native captures,
matched benchmark evidence and agreed Planetary integration checks.
This candidate changes the menu inset only. No save/audio/input/planetary
production changes. Lead must validate the combined build before release;
version, main and release tags remain unchanged.

## Integrated 2.5.48 — event identity and bounded shuffle

Base `ac8a581`, branch `cursor/audio-bounded-shuffle`. Original event sounds move
from 3–32 ms fragments to restrained 20–300 ms cues using the existing single
mono voice. New pure candidate-order policy visits every track once, with the
previous track last. Only the agreed selection and SFX rendering hunks in
`audio.h` are in scope; Systems owns decoding, buffers, I/O and power recovery.
No Game/save, main.c, music gain or station-composition change. See
`docs/AUDIO-AUDIT.md` for source evidence, budget and acceptance matrix.
Lead alone owns combined integration, versioning and release artifacts.
Final candidate PSP build and all-five PPSSPP smoke passed (57.98 FPS average,
33.37 ms worst, 44 existing warnings). Sampled music+SFX peak 11,161 with no
clamps; no physical PSP listening or custom-MP3 certification. Detailed report
and exact test-run identifier are in the audio audit. Combined integration remains
Lead's gate; this isolated candidate is not yet a playable main release.

## Integrated 2.5.47 — safe planetary approach

Approach now stops the remaining collision-frame simulation and pauses threats
in the simulation API. Gas boundaries offer Circle to turn away, with an explicit
explanation if X is pressed. Solid-world X remains surface flight, not instant
landing. The safety panel stays in the same clear centre in all HUD modes.
Docking/police states cannot be replaced by approach/entry calls. No `Game` or
V13 save-layout change, mission reward change, or surface rendering change.

See `docs/PLANETARY-ENGINE-AUDIT.md` for the read-only audit and staged acceptance
plan. The next planetary task is reliable EVA controls/traversal; durable survey
rewards and mission hooks require joint design with Gameplay and Systems.
Validated on main `7ef5fb0`: all five smoke groups pass (57.74 average FPS),
three native-art scene contracts pass, and six 480x272 prompts reviewed.
Existing compiler warnings and capture-I/O timing limits are recorded in the
audit. Release numbering, tags and publishing remain Manager-owned.
## Integrated 2.5.47 — truthful pilot-rescue controls

Generic rescue objectives now read "Lock rescue ship. Within 600 m: Triangle hail."
This matches the existing flight controls: Circle locks a contact, Triangle dismisses
an active notice or hails when clear, and pickup requires the marked ship within
600 m. Story & Dialogue approved the PR-01 intent. Contract and Guild rewards,
deposit, timer, save V13 and Game layout are unchanged.

The existing journey input-test hook covers board acceptance, track/navigation,
Circle locking, notice acknowledgement, 601 m rejection, exact 600 m pickup,
unrelated contact rejection, repeated hail, guided system-hub docking, contract
and Guild payment once, redocking, expiry and retaking the offer. Tests place the
player at the destination and freeze a contact for precise radius checks; they
do not claim manual travel or a physical PSP soak. An optional
`rescue-capture.flag` in the smoke directory writes `rescue-objective.bmp` through
the production 480x272 tracked-mission renderer.

Validation on base `7ef5fb0b3fa97d41323dfc265f5df720d9af416c`: PSP build
and all five smoke groups passed (290 game / 288 input checks, 57.74 average FPS,
33.37 ms worst frame). Compiler warnings remain 44, matching baseline. Native
capture was visually checked: the complete instruction fits on one line without
overlap. Evidence is in the Gameplay & Loops task's `outputs/rescue-evidence`;
normal smoke run is `work/smoke-20260923-001105-281`. Capture I/O was a separate
run and is not the reported performance baseline.

Lead integration combined this slice with safe planetary approach and Chapter 05.
The 2.5.47 PSP build and all five PPSSPP smoke groups passed. Physical PSP testing
remains the next verification step; later gameplay slices must start from this
integrated checkpoint.

## Integrated Gameplay & Story pass — Chapters 02–04

Chapter 02 now requires the authored sealed receiver pickup at Mara's bound port before returning to Lave. Chapter 03 requires the authored signal scan; firing or overheating resets the observation, blocks completion, and requires a clean re-entry. Chapter 04 requires the outbound port stamp, with an optional first-anomaly lifeboat scan that adds one Independent trust. State reuses reserved bits in the existing `saga_flags` word (`0x10`–`0x200`), so no `Game` layout or save-version change is present. Generic scans, kills, cargo, and unrelated docking do not satisfy these objectives.

Manager integration passed the PSP build and all five PPSSPP smoke groups: game, input, steering, radio and performance.

The Chapter 05 slice on `specialist/gameplay-story-ch05` adds the authored early-filing timestamp comparison at Lave, then exposes inspection/protest evidence choices. It reuses `saga_flags` bits `0x400`–`0x2000`, preserves the sealed records, and requires both the sealed receiver and convoy port stamp before comparison can begin. Partial evidence, wrong-system docking, and repeated comparison actions are covered by campaign tests. Legacy Chapter 05 saves are explicitly reopened with both records at Lave rather than being permanently locked.

The planetary story now awards an atmospheric landing kit when the existing coaching reaches **Her World**, after the power lesson. The award uses the existing `story_flags` word (`STORY_EV_LANDING_TECH`), so no `Game` layout or save-version change is present. Atmospheric approach remains available, but `land_planet()` requires the kit; completed/free-story saves and skipped coaching remain compatible. The handoff dialogue names the kit as recovered from Ryn's locker so the first landing reads as a deliberate story milestone.

Prepared 23 September 2026. Current build: **2.5.49**.

## PSP UI/art specialist handoff — native deck readability

The integrated UI/art pass shortens command-deck helper copy to the existing 27-column
detail pane and bounds the renderer at that width. This is presentation-only: no
mission, economy, save, input, or audio state changed. The lead workspace completed
the PSP build and all five smoke-test groups successfully after integration.

## Working tip — equipment modules (shipped in 2.5.46)

Outfitting buys into six hardpoints (`fit[]`); Loadout sells at 50%. Bits still drive the sim; V13 persists slots. Targeting CRT fuzz from 2.5.45 retained.

The integrated Systems/QA follow-up hardens fitted catalog bounds in Loadout and refund paths and adds V13 slot-value regression checks. Save format remains V13; V12 migration remains unchanged. The lead PSP build and all five PPSSPP smoke groups pass after combining this with the deck readability update.

## Start here

This is a native PSP homebrew game inspired by Elite-A and the wider Elite lineage. It is no longer a literal port: it has a new flight/world simulation, 256 seeded systems, modern PSP interface, multi-body systems, factions, missions, planetary flight/EVA, custom radio folders and an original Kei/Ryn campaign called **The Open Channel**.

Read these documents in this order:

1. `CLAUDE-HANDOFF.md` — current implementation state and working rules.
2. `docs/DESIGN-BIBLE-2.0.md` — concise product and technical direction.
3. `docs/ELITE-NEXT-MANUSCRIPT.md` — whole-game story & missions manuscript (Story Manager living bible).
4. `docs/OPEN-CHANNEL-CAMPAIGN.md` — the 24-chapter main campaign (ops summary).
5. `docs/UI-SPEC.md` — PSP-specific layout and interaction rules.
6. `docs/DESIGN-BIBLE.md` — exhaustive historical design record and detailed original scripts.
7. `docs/PROGRESS.md` and `docs/FEATURE-MAP.md` — implementation history and feature inventory.

The newest explicit user feedback overrides older prose in the large design bible.

## Non-negotiable product direction

- Keep the title **ELITE: NEXT** and its existing visual identity.
- Native PSP resolution is 480×272. Every screen, portrait, icon and footer must be verified at that size.
- Preserve the retro 1980s science-fiction tone: restrained neon, low-resolution pixel art, wireframe heritage and readable silhouettes. Avoid bright generic mobile-game or exaggerated anime styling.
- The interface must remain glance-readable. Put immediate flight information at the top and bottom; keep the central canopy clear.
- Blue speech panels belong to named NPCs. Orange right-tailed panels belong to the player.
- A tracked mission must always show one short, truthful next action. Never direct the player to a random contract that may not exist.
- Thargoids are rare and unsettling. Do not turn them into routine disposable traffic.
- All essential objectives need recovery paths. Avoid random availability, missable actors and repeatable rewards.
- Do not promise “zero bugs.” Build, run the complete regression suite and state what was actually tested.

## Current build and controls

The packaged executable is `EBOOT.PBP`. Put the game folder under `PSP/GAME/ELITE-NEXT/`.

**Download:** every playable build must appear on the GitHub Releases page (latest: https://github.com/roodmilk/elite-next-psp/releases/latest). After smoke is green, tag `v<VERSION>` and push the tag — do not leave the EBOOT only on a branch.

Custom MP3s belong beside the EBOOT:

```text
music/
  Deep Field/
  Neon Transit/
  Pixel Comet/
  Velvet Orbit/
  Far Horizons/
```

The folders are rescanned on startup. MP3 files are intentionally excluded from the handoff archives; users provide music with suitable rights.

Important controls:

- Select: command deck.
- Hold Triangle: quick communications.
- Square tap: silently select the object nearest the centre reticle.
- Hold Square: open the targeting computer after a short delay.
- Hold Square + D-pad: browse target groups without turning.
- Hold Square + L: cycle contacts currently in front.
- Hold Square + R: lock highlighted target and engage auto-turn.
- L + Left/Right: roll.
- Double-tap L while fast: hard brake.
- Double-tap and hold R: high boost (blocked when heat is critical).
- Heat: speed / boost / sun raise HEAT; cool when not boosting and clear of the star; max heat destroys the ship.
- Galaxy Map: Triangle switches between nearby jumps and the full 256-system map; D-pad moves between systems; L/R zoom; X plots a multi-jump route and saves the final goal.

## Recent changes that must be preserved

### 2.5.39 — MacVenture A++ cinematic polish

Station interiors restaged from `docs/CINEMATIC-MOCKUP-TARGETS.md` + ART DIRECTOR kit grammar: one hero per room, three floor/sky planes, warm ochre/cream staging, side hatch doors (no top door chrome over the focal object), thinner cream/cyan window chrome. Verb/hotspot/SHIP return behaviour unchanged; TAKE→deal / GO hatch snap tightened for fun.

**ART DIRECTOR visual style implementation (shipped in 2.5.40 tip):** all MacVenture rooms draw wall plates/rivets/floor seams; ARRIVALS berth window gets void stars + beacon sparkle; deck/UI panels drop gold corner brackets for charcoal + cream/ochre instrument rules; Wave B/C soft-FB planet bloom, travel FX, denser warp and docking beacon masks folded in. Story/saga manuscripts untouched. Packaged as **2.5.40** combined tip for Systems Release.

**Post-2.5.40 ART animation pass (untagged tip on PR #11):** animated ARRIVALS traffic silhouette + blinking berth board; room practical pulses; fauna icon families + field walk bob; docking warm aperture/corridor motes; near-station traffic glints; quieter MacVenture chrome. Presentation draw only — Gameplay Designer owns 256-system aliveness / spawn. No micro-tag.

**Next visual gap (same PR #11, untagged):** remaining room prop markers (tool rack, steam, loader, mission pin, clinic screen, customs REST); planet skyline blinks + haze + ochre pad apron/corner beacons; charcoal/ochre cockpit, speech, menu notice, minimal HUD, approach/police plates; warmer warp streaks. Still presentation-only.

**Implementer tip under ART DIRECTOR authority (rebased on tip #18 `f003e09`):** 8×8 native prop markers; ARRIVALS cargo-loader cycle; walk/help/comfort chrome; aperture beacons kit cyan. Mission cue: ART_AMBER + 2-col header margin (Designer had restored GOLD for the old flush assert — palette stays with ART DIRECTOR; smoke uses header-margin assert). No tag.

**ART LOOK FREEZE (Commander: one bug-fixed EBOOT):** Look freeze base remains `3f0dc20` (ART_AMBER — do not restore GOLD). Post-v2.5.42 ART tip on PR #11: HUD lock hint off; soft cosmic oceans; no text bloom; GalNet tab neatness; MacVenture options-list disembark (no verb row / pack). No tag until Systems smoke.

**Working tip (Commander: BIG RELEASES ONLY):** **v2.5.42** shipped quiet FX. Next Systems pack waits on ART tip smoke after this look pass.

### 2.5.46 — Equipment modules change ship stats

PR #32 on v2.5.45 tip. Smoke-green Commander pack.

### 2.5.45 — Targeting monitor fuzz

Art tip `178d34a` faint CRT static on targeting computer. Smoke-green Commander pack.

### 2.5.44 — Law rewrite scan/settle/clear

Gameplay Designer PR #29 on v2.5.43 tip. Smoke-green Commander pack.

**ART tip (untagged, PR #11):** targeting computer CRT glass — faint scanlines/static under list glyphs (no text bloom, no meteors). Systems owns smoke/pack.

### 2.5.43 — Play-fix pack (art + Designer + Story)

Art HUD/disembark, Designer dock-menu/cam/Factions, Story gazette lore. Smoke-green Commander download.

**Law rewrite (untagged tip):** hold scan discovers restricted goods (no crime-on-buy); warrant vs scan stops; Status Square / CUSTOMS tip clear desk. Pirate bounty unchanged. No art restyle.

### 2.5.42 — ART quiet FX + declutter disembark

Quiet space FX (`b0a97e1`) on the combined tip. Smoke-green Commander pack. Follow-on untagged ART tip: options-list station deck + HUD/GalNet/space-ocean look fixes.

**Commander play fixes (untagged tip):** undocked deck hides Shipyard / Outfitting / Mission board; menu ship viewport orbit slowed; Factions X cycles Story channel lore, Triangle locks nearest contact. Station disembark layout stays ART-owned (no verb-bar restore).

### 2.5.41 — Art freeze + living-galaxy pack

ART tip `3f0dc20` (ART_AMBER) + galaxy `bc56842`. Smoke-green Commander download. Follow-on: MacVenture PACK/EXITS overlap removed; space meteors + station sparkle glitter dropped for soft haze.

### 2.5.38 — soft-FB Wave A canopy FX

Denser NPC/player engine plumes, boost heat shimmer, hit sparks and explosion embers in `space-fx.h`. Fixed pools, canopy-clipped; high contrast / warp / dock mute decorative sparks. Station art kit re-baked. No crawl or story ownership changes.

### 2.5.37 — unified playable tip

One pack: MacVenture station (art-kit soft-FB, SHIP return, talk/shop/gift/taxi) + Act I–IV eight-beat page scripts/codas (Ch.02–25 through berth). ART DIRECTOR warmer planets / settlement silhouettes / richer space folded in (`planet.h` / `voyage.h`). Post-unify A++ MacVenture polish composition targets: `docs/CINEMATIC-MOCKUP-TARGETS.md` (hero focus, fewer frames, warm staging) — do not edit story saga ownership.

### 2.5.36 — MacVenture + art kit wire

Soft-FB rooms use `station-art-kit.h` styles (PR #12 bake). Talk tips lead somewhere; ship return remains TRI / >>SHIP / EXITS. Kit boards are proposed — not claimed as baked runtime textures.

### 2.5.34 — MacVenture station deck

Illustrated rooms + LOOK/SPEAK/GO/TAKE hotspots (Shadowgate / Deja Vu grammar). Dense clickables; named doors in EXITS; TRI / YOUR SHIP boards from any room. Small interior; maze crawl retired.

### 2.5.30 — MM6-style station crawl polish

Crisp three-depth station walkaround: riveted panels, checker floor, destination-labeled doors, portrait NPC sprites, clearer room chrome. Interactions unchanged.

### 2.5.29 — soft-FB space FX kit

In-engine nebula, clouds, and gentle twinkle (`space-fx.h`). Constant meteors and station glitter masks removed (Commander FX quieting). No GU particle dependency; high contrast skips decorative haze.

### 2.5.28 — OpenEnroth-style station crawl depth

Facing-relative side portals, arched passages, tile floor, ceiling beams, denser props; interactions unchanged.

### 2.5.27 — ask-then-answer story chat

Prologue/saga Cross speaks the commander line first; NPC answer on the next beat. Saga replies are acknowledgments, not pre-answered questions.

### 2.5.26 — GalNet WANTED page cue + chrome

WANTED board gold page-2 banner and five poster wear styles; larger `<L`/`R>` pads clear of NEWS/JOBS.

### 2.5.25 — pixel-art suns + bloom

Eight animated sun sprite families shared across flight/UI; additive corona and sparse canopy bloom. Chart tints match live bodies.

### 2.5.24 — chat flow, GalNet wanted, sun bloom, station FP

Prologue asks then answers. WANTED board pages and poster variety. Animated system suns + bloom. Station crawl FP closer to OpenEnroth corridor style.

### 2.5.23 — menu zoom, speech wrap, battle talk

Select deck ship view orbits farther out. Speech/notice panels wrap instead of truncating. Combat radio uses `SFX_TALK` with battle chatter (respects Quiet Comms).

### 2.5.22 — station crawl + stocked outfitting

Disembark opens a first-person room-grid station (minimap, shops, gifts, taxis). Outfitting lists only local stock; `inventory_screen` shows equip slots. Passengers occupy 1t with route chatter (save V11).

### 2.5.21 — full screenplay assembly

`docs/OPEN-CHANNEL-SCREENPLAY.md` now holds the merged Act I–IV feature script (character bible, lore ledger, full scenes, branch matrices). Playable trust/choice wiring unchanged from 2.5.20.

### 2.5.20 — Open Channel screenplay + trust

Canonical long-form story is `docs/OPEN-CHANNEL-SCREENPLAY.md` (characters, full scenes, branching, lore ledger). `saga_choice_label` distinguishes the four permanent decisions; `saga_trust` surfaces helpers from chapter 19 and colours the epilogue. Display flip remains IMMEDIATE after vblank.

### 2.5.19 — hardware display flip

Main-loop `sceDisplaySetFrameBuf` uses `PSP_DISPLAY_SETBUF_IMMEDIATE` after vblank. Do not switch back to `NEXTFRAME` for ordinary frames — that caused live-buffer painting and black strobing on PSP. Sleep/resume still rebuilds both planes.

### 2.5.18 — varied planet landings + on-foot chrome

Surfaces pick a biome from the orbit sprite family and tint ground/flora/fauna from `Body.color`/`accent`. Planet EVA uses dedicated ON FOOT chrome (nub look, D-pad move). Station disembark remains on the docked Fly menu with talkable concourse NPCs.

### 2.5.17 — engine trails on the stern

Exhaust plumes and aft glints use the mesh rear tip (and freighter capital nozzles), matching the drawn silhouette. Do not reintroduce a radius-scaled glow behind the ship — that floated past wide hulls such as VIPER and COBRA MK 3.

### 2.5.16 — SHIPS vs ENEMIES

SHIPS lists all alive ships (hostiles included). ENEMIES lists only contacts currently engaging the player (`target == -2`). Browsing SHIPS stays on SHIPS when you highlight a hostile.

### 2.5.15 — RED ALERT bottom banner and ENEMIES band

Combat status uses a bottom-of-canopy **RED ALERT** strip so top speech stays free. Hold Square + Left/Right cycles PLANETS / SHIPS / STATIONS / OTHER / **ENEMIES**. From 2.5.16, ENEMIES is engage-only (`target == -2`); SHIPS lists every alive ship.

### 2.5.14 — locked story conversations

First-flight and Open Channel briefs are linear locked conversations (prologue six beats; saga eight). Circle and Select cannot leave Tracked Mission until the player accepts the next step. The last beat restates the objective; after accept, old dialogue options are gone and chatter only reinforces the current mission step. `src/saga.h` stores `line` plus `talk2`–`talk8` per chapter.

### 2.5.0 — The Open Channel

`src/saga.h` adds a 24-chapter data-driven follow-on to the first-flight prologue. Story state lives in `Game` as `saga_chapter`, `saga_step`, `saga_flags`, `saga_choice`, `saga_dest`, `saga_start` and four trust values. Save format 9 appends ten little-endian values and still imports older saves.

The currently playable chapter actions are deliberately compact: dock, scan, defeat hostile ships, return home or make a choice. The campaign bible contains richer bespoke scenes that still need staged implementation. Do not describe all proposed set-pieces as already implemented.

### 2.5.1 — custom MP3 quality

`src/audio.h` uses the PSP hardware MP3 decoder. It now has a 64 KB compressed stream buffer, 2,048-frame stereo output blocks and bounded Catmull-Rom resampling for 32/44.1/48 kHz sources. Suspend/resume tears down and safely recreates decoder resources. Preserve the larger buffers; the earlier 16 KB/256-frame design produced intermittent crackle on hardware.

### 2.5.2 — galaxy navigation

The nearby list only shows local candidates. Triangle opens a spatial overview of all 256 systems. It marks the current system in cyan, the tracked mission destination in gold, and caches/draws every intermediate jump. L/R zoom from 1× to 4×. X converts the selected long route into its first reachable jump. This logic is generic; **Quator has no special code or significance** and was only the system that exposed the old UI flaw.

Story navigation plans against the fitted drive even if the tank is empty, then marks the next hop as low-fuel until the player refuels. The story screen distinguishes `NEXT` from `FINAL`, and the cockpit names the next reachable hop.

### 2.5.13 — Select deck third-person ship

Command deck top-right inset orbits the fitted hull in local space (stars + station or planet), large enough to read the silhouette.

### 2.5.12 — local wanted under system name

Cockpit header shows `Wanted n/5` on the line under `System:` (red when police may pursue, dim at 0/5).

### 2.5.11 — mission cue top-right

Tracked HUD cue sits in the top header band, right-aligned with a 2-col margin (not flush to the screen edge), clear of the danger badge. Objective ink is ART amber (`RGB(240,180,91)`).

### 2.5.10 — Start power on cockpit meters

Hold Start: Left/Right pick SYS/ENG/WEP on the existing meters; Up/Down move pips. No separate overlay panel.

### 2.5.9 — hard brake / heat polish, target bars, freighter fight-back

Double-tap L hard brake and heat rules keep their regressions. Target panel shows HULL/SHLD bars. Damaged freighters return fire and destroying non-pirates raises extra local warrant. Engine plumes attach at mesh aft. Station walk is a FLY deck door with talkable NPCs; planet surfaces pull tint from world colours.

### 2.5.8 — per-system skies and far warp-in

Orbital templates, world-type permutations and seeded traffic make each system look different. Hyperspace drops you farther from the hub.

### 2.5.7 — radio chassis and audible static

Radio UI is a dial chassis (OFF + 1–5). Retune injects audible static; OFF is silence. SHIPS lists all ships; ENEMIES lists only contacts currently engaging the player.

### 2.5.6 — red alert and enemies band

Combat cues use a bottom-of-canopy RED ALERT strip. Square+D-pad scanner bands include ENEMIES (engage-only). Galacticnet Spacebook/Messages and Codex Systems/Planets remain as shipped in 2.5.5.

### 2.5.5 — speaker chips, menus, radio, system variety

Dialogue name plates use `speaker_name_tag` so `NAME SAYS` sits on a faction-coloured chip. Tracked Mission draws speaker portraits. Outfitting explains tech gates in plain language. Galacticnet order is Spacebook then Messages with a parody logo. Codex lists Systems and Planets discovered by visiting. Radio is a tuner with OFF, static on retune, and Triangle power. Planet types/traffic layout vary per system; hyperspace arrival is farther from the hub.

### 2.5.4 — sleep/resume recovery

Long PSP sleep could leave the MP3 decoder blocked on Memory Stick I/O and the LCD framebuffer unrestored, producing a permanent black screen on wake. Suspend now freezes MP3 sampling from the power callback; resume rebuilds display mode, both framebuffers, controls, clock and audio. Frame presentation uses IMMEDIATE after vblank (NEXTFRAME was reverted in 2.5.19 — it strobed on hardware). Emulator smoke covers a double recover path; confirm on physical hardware after multi-hour sleep.

### 2.5.3 — manual route persistence

`Game.route_goal` stores the final destination of a manually plotted multi-jump route separately from `destination` (the immediate hop). Save format 10 appends that goal and still imports V1–V9 commanders. After hyperspace, `route_refresh_destination` advances the next hop toward the saved goal, or clears the goal on arrival. The galaxy overview labels a saved manual goal in amber when no story destination is tracked. Contract “next step” navigation also sets `route_goal`.

## Architecture

The project is intentionally small and header-heavy.

- `src/game.c`, `src/game.h`: core world state, galaxy, spawning, physics, AI, save/load and tests.
- `src/main.c`: PSP startup, input routing, rendering loop, screen dispatch and input regressions.
- `src/ships.c`, `src/ships.h`, `src/mesh.h`: ship definitions and geometry.
- `src/saga.h`: long campaign data and runtime.
- `src/campaign*.h`: first-flight prologue and tracked-story UI.
- `src/story.h`: optional new-player coach; separate from the authored campaign.
- `src/guild.h`, `src/journey.h`, `src/sectors.h`: Guild assignments, route reliability and repeatable contracts.
- `src/ui-modern.h`, `src/voyage.h`, `src/narrative-nav.h`: menus, galaxy map, HUD guidance and shared narrative actions.
- `src/audio.h`, `src/radio-*.h`: hardware MP3 playback, station folders, generated fallback music and audio preferences.
- `src/*tests.h`: tests compiled into the PSP executable and run by smoke mode.
- `assets/`: source and generated visual assets. Runtime art is mostly embedded in compiled headers.
- `elite-a/`: upstream/reference Elite-A material. Retain provenance and do not assume every file is part of the new runtime.
- `tools/`: asset conversion and validation helpers.

`Game` is one monolithic persistent/runtime structure. New persistent fields require a new save version, strict range validation, an old-save migration path and corruption tests. Rewards and story transitions must remain idempotent.

## Building and testing

Windows PowerShell:

```powershell
./build.ps1
./smoke-test.ps1
```

Linux helpers in this cloud environment: `./build.sh` and `./smoke-test.sh` (PSPDEV + PPSSPP SDL).

`build.ps1` expects the PSP toolchain at `../../work/toolchain` unless `-Toolchain` is supplied. `smoke-test.ps1` expects PPSSPP at `../../work/ppsspp/PPSSPPWindows64.exe` unless `-Emulator` is supplied.

The build compiles `game.c`, `ships.c` and `main.c`, links PSP libraries and produces `EBOOT.PBP`. Smoke mode creates a disposable folder and must report zero failures for:

- game checks;
- input checks;
- steering checks;
- radio checks;
- performance checks.

The last verified 2.5.32 run passed every group under PPSSPP (game, input, steering, radio, performance), including ask-then-answer script checks. PPSSPP success does not replace physical PSP testing.

## Highest-priority remaining work

Planetary lane: the specialist/planetary-eva checkpoint now covers controls, shared terrain, local bounds and ship return (pending lead integration). Next, runtime-confirm mineral-free worlds and agree one guaranteed activity plus durable reward/revisit rules with Gameplay and Systems. See docs/PLANETARY-EVA-HANDOFF.md; do not promise these next mechanics as implemented.

Audio: measure malformed-file retry cost and output progress after sleep with
Systems; current suspend assertions only prove calls returned. Physical crackle,
custom-MP3 headroom and long sleep listening remain open. WAV/live rescan,
per-station shuffle history, warning priority and sustained EVA layers are future
coordinated slices, not implemented features of this candidate.

1. **Confirm 2.5.4 sleep/resume on physical PSP.** Put the handheld to sleep mid-flight and mid-radio for several hours, then wake — screen and audio must return. Also re-check 20+ minute MP3 playback across sample rates.
2. **Deepen the 24 chapters further.** Briefings now carry full spoken sentences and authored asks; many bible set-pieces still resolve through generic dock/scan/hunt actions rather than unique scenes.
3. **Visually inspect the full galaxy map at 480×272** and the new Select-deck ship preview / radio tuner on hardware.
4. **Physical performance and memory audit** of galaxy routing and the 64 KB MP3 buffer.

## Known limitations and honest status

- The design bible describes a far larger game than the current executable. Interiors, planetary exploration and spacewalks are bounded prototypes rather than Starfield-scale simulations.
- The campaign has 24 playable chapter records with ask-then-answer briefs and voiced closers (2.5.32), but does not yet contain ten hours of unique bespoke mechanics. Travel and ordinary play contribute to its intended duration.
- Only one galaxy seed of 256 classic Elite-style systems is active.
- The full-galaxy chart shows all systems, a cached route and a saved manual route goal, but has not yet had user testing on a physical PSP.
- The audio fix passed PPSSPP with the user's files; intermittent real-hardware behaviour still requires listening tests.
- Manual route persistence is playable in 2.5.3; sleep/resume black-screen recovery is in 2.5.4 but still needs multi-hour hardware confirmation.
- Speaker colour chips and clearer Outfitting tech copy shipped in 2.5.5.
- Full movie-length scenes still live in `docs/OPEN-CHANNEL-SCREENPLAY.md`; the executable binds six spoken beats per chapter.

## Safe continuation workflow

1. Pull and rebase before starting.
2. Run the unchanged build and smoke test to establish a baseline.
3. Make one coherent feature change.
4. Add a meaningful regression for its state transition or input path.
5. Rebuild without compiler warnings and run every check.
6. Inspect affected screens at native 480×272.
7. Increment `VERSION`, `build.ps1`, `CHANGELOG.md` and `CLAUDE-HANDOFF.md` together, copy `EBOOT.PBP` to `dist/ELITE-NEXT-PSP/`, commit and push.

Do not delete older-save handling, audio resume logic, campaign idempotency checks, mission availability checks or route-planning tests to make a new feature easier.

## Rights and source boundary

The project uses Elite-A/reference material and established Elite concepts. The new campaign, dialogue, UI and most new implementation are original. Do not copy dialogue, art, music or proprietary assets from Elite Dangerous, No Man's Sky, Starfield, novels or fan sites. Use lore facts as background and write original expression. Keep source/provenance notes and review redistribution rights before any public release.

Validation for 2.5.69: build succeeded; all five smoke suites passed in
C:\Users\skarm\Documents\Codex\2026-09-24\let\work\smoke-20260924-213124-036.
EBOOT SHA256: 00C66FD9C82FBFCE337F07BA24CF0CFAB7DF081A2450B242E208E8D1C0AB1870.
Physical PSP testing remains unverified.
## Released 2.5.75 — staged hyperdrive jump

Galaxy-map jumps now return directly to the cockpit and run an eight-second staged sequence: charging lines, stronger shake and blue energy, a vivid multi-colour hyperspace corridor, then braking before the existing system-arrival logic runs.
## Released 2.5.76 — longer engine boost endurance

Boost heat is now lower and scales with the ENG capacitor: higher ENG settings reduce heat generation and improve cooling while boosting. Critical lockout and destruction safeguards remain unchanged.
## Released 2.5.77 — concise cargo menu label

The SHIP tab now shows `Cargo` while flying and keeps `Cargo & market` while docked.

## 2.5.124 follow-up
- Ship Decorator now includes native C64 sticker marks, finish/pattern navigation cue, live preview/equipped state, and fee/status card.
## 2.5.219 — In-world Outfitting shop signs

Outfitting now opens beneath a slim station storefront sign instead of exposing the local economy, technology and trade values. Every station receives a stable, seeded shop identity—such as Major Lazor's Armaments, Aegis Defence Works or The Module Exchange—with a concise speciality caption. Lave remains a dedicated arms house; general and specialised stock keep a matching, readable identity whenever the player returns.

Verification: PSP build passed. Native Outfitting captures confirm the new arms and general headers at PSP resolution; input suite passes with 0 failures.

## 2.5.230 — Authored System Almanac

- System Details now uses the approved illustrated Almanac composition at native 480x272.
- Authored source art lives in `assets/source/system-almanac`; `tools/bake-system-almanac.ps1` compiles bounded ARGB1555 arrays and native previews.
- The scene composes real system body seeds/types with Poor, Rich and Mega Capital station art. The right dossier remains fixed while selection moves.
- X targets and returns to flight with auto-turn. Square intentionally remains on the illustrated Almanac instead of opening the superseded route dashboard.
- Feature checks pass. Broad suite still has the inherited station swept-collision failure plus unrelated existing input/outfitting/dialogue/tutorial/UI failures.

## 2.5.230 — Build 1 main menu

- Replaced the old 2x2 title choices with a single four-row vertical list over an authored Lave cinematic backdrop.
- Choice 0 is a real quick-continue route for the active/first valid commander; without a save it becomes Begin New Commander.
- Choice 1 starts a new commander, choice 2 opens the existing three save cards, and choice 3 starts or resumes First Flight Tutorial.
- The right card reads real save metadata and title-cases the commander name for display only. It uses the existing portrait identity and a tiny hull-varied pixel ship.
- Intro art is baked to `src/intro-art.h` by `tools/bake-intro-art.ps1`; source and preview live under `assets/source/intro` and `assets/preview/intro`.
- Native PPSSPP comparison and rationale are recorded in `design-qa.md`; final visual QA passed.
- PSP build passes. Current broad smoke still carries the known station swept-collision failure and unrelated pre-existing input/outfitting/dialogue/tutorial/UI failures; steering and radio report zero failures.

## 2.5.231 — Rear-view flight mirror

- Removed the full/minimal HUD system-name readout and the decorative cycling activity strip.
- Their 252x22 upper-left area is now a live 117-degree aft camera using real world positions.
- Mirror contents include stable rear stars, celestial bodies, all hubs and live NPC traffic. Faction colours, projected trails, selection brackets and red incoming-fire brackets make it useful during combat.
- The view follows ship yaw/pitch/roll and writes no gameplay or save state. Work is bounded and allocation-free.
- `rear-demo.flag` creates the reviewed three-contact capture fixture; `rear-test.flag` writes the focused direction report.
- Native capture: `design/rear-mirror-build/rear-mirror-native.png` in the task workspace. Focused emulator check passes. Broad suite still carries the inherited station swept-collision failure; steering/radio stay green.
## 2.5.268 — Playable opening dream

Opening dream now has a timed blink/introduction, three staggered Thargoid models, real ship steering and laser hits, scripted attack pressure, and victory/low-hull/timeout wake conditions. White fade lasts six seconds before the existing Homeward reset. No save schema change. Focused tests exercise all three wake paths. Preserve unrelated work and keep this a local private build.


2.5.277 final verification: 280 focused opening/comms/portrait checks PASS, RESULT 0 failures; real expanded-DNA save/reload included. Native creator and 22px diversity gallery inspected. Creator preview labels separated; human per-base landmarks and silhouette-attached ear details added. EBOOT 10028311 bytes, SHA256 D1C57BD4772AC3B34E2AA480B85F739331A3B20413A629C76637B5B2B5FB4FDB. Dist refreshed. Local only; no push/tag/release; hardware and legacy broad smoke remain outstanding.






## Current local performance work: 2.5.339

Verification complete: `perf-release-build.log` succeeds; `outputs/performance-review-20261009-171950-400/performance-review.txt` reports 0 failures, including five render-plus-simulation CPU budget gates. Native mission/crowd/planet screenshots inspected. Query workload 32.038 → 0.055 ms; close crowd 47.460 → 28.682 ms; rolled planets 77.421 → 23.391 ms. Live-flight results in `outputs/soft-sky-performance-20261009-172029-941`: all five nominal 30 FPS gates pass (29.97, 39.52, 41.38, 32.12, 39.48 FPS), versus 21.52, 25.24, 21.56, 25.57, 18.31 FPS in the saved 2.5.338 EBOOT. Worst live frame is still 66.73 ms; cold artwork entry and real hardware frame pacing remain open. Do not claim fixed/locked FPS.

Final broad-smoke comparison: `work/smoke-20261009-172225-097` and unchanged `work/smoke-20261009-165852-898` have exactly the same 15 game failures. Radio and steering report 0 in both. Both runs hit the 120-second harness timeout during input coverage; the optimized run progressed farther. The first 403 complete input report lines are identical. Later input assertions are not baseline-certified; do not compare raw failure totals from incomplete reports or claim the entire suite is green. The script exits nonzero on the known game failures. Packaged EBOOT hash: `0E6445A59B298222ED8D6E8A2DFEA1AE5307B245ABD7D7099A4EC097E5DEA857`.

Player reported slow mission-board scrolling and crowded/distant flight views. Mission generation now has explicit render-scoped `mission_queries_begin/end` batching; never retain that scope across input or commander mutation. `mission_landable_body` queries the same deterministic type table and Sol override as generated bodies. Station availability uses the canonical fixed `HUB_COUNT` without constructing an unrelated full world snapshot.

Renderer changes: conservative sphere/viewport rejection, triangle bounds rejection, cached mesh radii, one model basis per hull, distant hull/debris LOD, bounded close-up flame plumes and cheaper small-ship trails. Opaque hull batches use depth-buffer visibility without painter sorting. Star directions are cached per system. `planet_sheet` is a fixed ten-entry, 160 KiB cache keyed by seed/type and shared by level/rolled/menu views. Only the filtered nebula uses 2x2 writes; stars, sprites, UI and gameplay remain native. No save-format or control changes.

Use `tools/review-performance.ps1` for exhaustive 1280-offer destination/validation/risk/reward equivalence, 2560 generated planet checks, exact cached sprite comparisons, edge/near-plane culling tests and six workload reports/screenshots. The fixture's `render_reference` flag is diagnostic only; normal play keeps it zero. Stage timing is disabled outside the fixture. Do not interpret no-vblank emulator CPU timings as certified hardware FPS.

Baseline: `perf-baseline-build.log` built 2.5.338 successfully. The unchanged broad smoke run produced inherited game/input failures and did not finish its performance report within the existing timeout; radio and steering were green. Preserve this evidence and compare failures rather than claiming the whole legacy suite passed.

Highest-priority remaining work: physical PSP crowded-scene/MP3 listening and frame-pacing checks; resolve the broad legacy smoke fixture failures separately. This checkout contains extensive pre-existing shared edits. Repository synchronization was rejected by automatic approval review to protect unfinished changes; preserve them and do not publish/tag an unverified shared tree.


2.5.356 verified local artifact: A6522F226760A695479AC9A14268E113EC9D25390A085E52467DD9C253362C7B. 93 focused checks pass; all 42 Controls captures match 2.5.355. Core retains the identical 15 inherited failures; radio/steering pass. Full evidence: BUILD-2.5.356.txt. No GitHub publication.



Diorama Stage A candidate built in outputs/diorama-stage-a (original memory, unchanged guard). +2080-byte image, headroom3149764; exact retail flight boot and native lighting review pass. Both baseline/candidate performance review RESULT3 (mission-query mismatch and crowded near/third-person budget overrun). Stop before exhaust/glow/integration; review-only EBOOT, dist unchanged. See outputs/diorama-stage-a/REVIEW.md. Canonical source hashes preserved.

13 October: isolated Diorama gate diagnosis completed in outputs/diorama-gate-diagnosis. Stale five-slot/generic mission oracle corrected for six-slot/expedition dispatch; no gameplay mission change. Finite-helper station raster preserves repeated native first/third-person pixels and saves~1.3ms crowded render, clearing near/third-person33.42ms CPU gate. Final performance-review and flight-lighting-review RESULT0 failures. Guarded MEMSIZE0 candidate image13627132, headroom3150084; exact retail startup passes. Canonical src/dist untouched. Candidate and DIAGNOSIS.md available for review; no exhaust/glow or release integration. Physical PSP and combined live regressions remain open. VISUAL-OPTIONS.md contains proposals only, awaiting user choice.

13October: isolated cosmic backdrop strength slice captured in outputs/cosmic-strength-slice/views/native-backdrop-comparison.png (actual480x272 tiles, three angles). Seeded Deep Galaxy / Stellar Nursery / Ancient Cosmos, bounded24-point foreground dust and restrained station rim extension. Retail memory guard passes image13627996/headroom3149220; native determinism/reticle/bounds checks RESULT0. Not integrated/released: new-sky cache first frame~190–210ms, strong nebula regions need art balance, exact-retail runtime/audio/save and physicalPSP tests open. See REVIEW.md. Canonical src/dist unchanged. Separate proposed station-material brief remains queued, not implemented.

13October final bounded cosmic correction in outputs/cosmic-final-slice: darker blue star band; restrained cyan/violet nursery with dark gaps; ancient galaxy/arc with varied sparse stars; fixed spatially dithered faint foreground clusters. Actual480x272 three-angle sheet in views/. Sky cache incrementally built6rows/update during hidden transition; negative-timer instant-arrival renderer guard passes. RESULT0 native deterministic/bounds/reticle/guard checks, warmed maxdelta+0.586ms. Retail FINAL2 guard image13630252/headroom3146964 (only1236bytes reserve margin). Startup/hidden prep total~170–179ms spread65 steps max3.42ms; unusually short transitions may extend, and unrelated first renderer warmup69.7ms remains. No canonical src/dist integration or separate station-material project. See REVIEW.md for limitations and exact artifacts.

13October capture-only cosmic gate in outputs/cosmic-capture-gate: native lossless moving OFF/ON comparison, accepted render headers unchanged. Dust imperceptible (stable frames0–35 changed pixels/max4 channel), strict repeat-OFF control failed4/60 initial capture frames; raw evidence preserved. Studio rejects dust for integration; minimal dust-only omission list in HANDOFF.md, no further dust iteration. Live normal jump7->55(3.6LY), sky ready3.847s before arrival,0 corridor extension. MP3 output165 blocks/0 device starves, but cold priming continued into warp, so not full-jump music continuity proof. Canonical/released and accepted cosmic EBOOT unchanged. Queued station-local depth remains separate, unimplemented.

13 October 2026 isolated graphics gate: outputs/cosmic-memory-gate/REVIEW.md records 14,308-byte recovery, final original-PSP headroom 3,161,272 bytes, still 50,204 bytes short of the supplied Audio-inclusive floor. Transition scratch reuse is NO-GO: existing distributed page/result guards do not prove exclusion; no union or new build applied. See outputs/transition-scratch-feasibility/REVIEW.md. Canonical src/dist not edited by this gate; concurrent shared Audio/main edits must be preserved. Candidate only, not released.

Current-base isolated graphics measurement completed: outputs/current-manager-cosmic-gate/REVIEW.md. Current Manager already has the frozen Audio layers; no duplicate Audio addition. Corrected R2 image13,623,740/headroom3,153,476 passes reserve by7,748 bytes; exact original-model retail MP3 has303 blocks/zero starves. Focused save/intro/deck/menu/opening/radio and corrected native graphics tests pass; broad core18 failures and incomplete profile/input suite prevent an all-green release claim. No canonical src/dist update. Separate requested ray/ship-signature/pixel-texture experiments in outputs/sunshafts-native-preview are not included in this measurement.

Queued ERL READY checkpoint source review: outputs/erl-ready-checkpoint-review/REVIEW.md. All five current file hashes match supplier packaging base; logic-only scope suitable for selective owner integration, none applied here. Supplier performance fails55FPS and music absent; no full-journey or hardware acceptance. Neon files excluded.

User rejected generic station tiling. Replacement architecture-led Lave study: outputs/station-authored-study/DESIGN-PLAN.md and REVIEW.md; explicit37-part ownership,8 whole-surface maps, real native six-angle R2 captures. Atlas losslessly1120B; original-memory image13,628,572/headroom3,148,644 (margin2916). Eight native material checks and five exact-retail boot/MP3 checks pass; CPU-only costs/physical PSP limitations documented. Art study awaiting acceptance, no canonical src/dist integration; not a universal station/ship-texture completion claim.

User approved the architecture-led station design. Focused shadow-only refinement packaged at outputs/LAVE-STATION-SHADOW-READABILITY-CHECKPOINT.zip; outputs/station-readability-pass/REVIEW.md. Atlas/roles/geometry/sunlit/glass/recess/emissive colours retained; matched approach/close/wide plus approved close, ten native checks and five retail boot/MP3 checks pass. Original guard margin2724B, physical PSP untested. Queued ERL READY five-file source boundary rechecked after packaging; compatible, source-review only, no canonical merge/Neon work.

User reconfirmed the sunlit architecture-led station design on13October. Continued native four-angle review: outputs/station-readability-pass/views/approved-four-angles.png; fresh capture-approved-continuation reports ten checks and RESULT0. Keep role-specific maps/warm ceramic/cool structure; do not return to generic tiling. Isolated art study remains separate from canonical release; crowded-view performance and current combined-memory integration remain open.

Compact ERL READY review completed: outputs/erl-ready-compact-gate/REVIEW.md and outputs/ERL-READY-COMPACT-RECONCILIATION.zip. 55+147 checks pass; paired retail memory/start/MP3 passes, candidate headroom3154052. Exact retail live countdown/autosaves pass but52.68FPS misses55 gate. No merge: four authoritative ERL headers are pre-existing untracked baseline; reconciliation proposal and hashes provided. Preserve newer Audio main.c hooks. No station/Neon inclusion, dist unchanged.

Reconciliation step1 local commit b050e8b records exactly four unchanged pre-existing ERL headers. Step2 stopped: HEAD game.h lacks existing race API/type baseline; current game.h/main.c/game.c have extensive shared dirty changes. Do not stage wholesale. See outputs/erl-ready-compact-gate/RECONCILIATION-STOP.md. READY/memory patches not applied to canonical; no release/tag/push, current verified artifact remains isolated.

game.h prerequisite audit completed: outputs/erl-ready-compact-gate/game-header-audit/REVIEW.md classifies all15 hunks. Not declaration-only: fit/equipment/body/landing width and embedded save states change layout/serialization.124 extracted symbols accounted for in source; no full unrelated-feature completion claim. No header/READY/recovery commit; b050e8b remains latest. Exact patch/source/definition evidence in outputs/ERL-GAME-HEADER-BASELINE-AUDIT.zip.

Executed save/layout gate FAIL: outputs/erl-save-layout-audit/REVIEW.md and outputs/ERL-SAVE-LAYOUT-AUDIT.zip. Frozenbaseline core669pass18fail; expedition326pass3fail; DockBay116/planet31/ERL147/flight-state39/opening345pass. Existing V30/32/33/34/36/37 and laterV35 migrate/resave; two CRC-valid archived earlierV35formatvariants failload. Publicrelease provenance unknown. Stop embeddedstate hunk116→228 and related save-layout hunks; no game.h/READY/recovery commit, b050e8b latest. Preserve concurrent review-only Audio main.c change. No release/tag/push.

Isolated V35 repair closed: outputs/erl-save-compat-repair/REVIEW.md and outputs/ERL-V35-READER-REVIEW.zip. Two archived pre-release formats decoded/migrated, exact equipment/fleet preserved; current writer unchanged. Only game.c/dockbay-save.h in candidate, not canonical. Full regressions retain core18/expedition3fail; other116/31/147/39/345pass. Matchedretail headroom3146372→3146500, -128linkedbytes; no releasecert. No newcommit. Userprioritynowstationfamilyartwork in outputs/station-families-stage1.

User-priority station artwork: outputs/station-families-stage1 now has40whole-component maps/five suites andten native480x272 actual-system views in views/five-station-families.png.17geometry/mapping/guard checks pass. Rich/poor family mapping in isolatedrenderer; mega-capitals excludedpendingownplan. Not retailmemory/FPS acceptance; refine Scientificroof/Industrialsunlitmetal/Frontierclose views. PreserveapprovedLave and allcanonicalwork; no release/integration.

Completed isolated main-station artwork coverage: outputs/station-families-complete/REVIEW.md, gallery-final.html and outputs/ALL-MAIN-STATION-ARTWORK-CHECKPOINT.zip.16rich+8frontier+4capital crowns;48maps/6suites;88native480x272 captures;119checks pass. All88RGBexact after memory optimization. Originalretail image13582492/headroom3194724/margin48996; flight/home realMP3 checks10pass,minfree3075840. Existing48KiBbillboard cache moved into7MiBheap;24KiBatlas cache added once. Heavyview CPU46.548ms remains open; physicalPSP/combinedlatest integration pending. Secondary relays outside28-layoutscope unchanged. No stationcanonicalmerge/dist/tag/push; preserve latestAudio/Game/ERL source.
2026-10-13 local graphics work:
- Expanded deterministic visible sun/halo/reflection palette from 8 to 32 colours
  in canonical art-runtime.h and the isolated station-art candidate. All 32
  primary-star seed choices occur across 256 systems; save fields unchanged.
- Candidate EBOOT: outputs/station-families-complete/EBOOT-SUN-32.PBP.
  Memory guard and original-PSP-model native flight startup pass; not physical
  hardware tested, not merged over shared gameplay/dist.
- Started isolated Adder artwork pilot in outputs/ship-art-pilot: 34 vertices,
  50 triangles, four 16x16 authored material maps, twin engine roots/tapered fire.
  Other hulls unchanged; fleet rollout and full performance gates remain open.
  Initial screenshot fixture accidentally captured the sky-loading effect;
  those initial images are NOT proof of ship visuals. Corrected fixture pending.
Ship panel-art checkpoint (2026-10-13): outputs/ship-art-pilot/gallery.html has
actual native staged Adder views. Latest user requires bespoke physical-panel
art fitted to geometry. Superseded the generic 4-tile prototype with 26 named
16x16 panels and 50 triangle UV records; shared panel UV continuity checks pass.
34 vertices/50 triangles, restrained metallic NPC paint, original player paint
and engine roots preserved. Native staged render-only emulator means 8.3-11.8ms
exclude audio/display. Far poses align with the aiming reticle and are NOT
distance-readability acceptance. Other hulls, flyby motion, crowded views, retail
memory/performance integration and physical PSP are unfinished. Isolated source
only; do not overwrite current main/game/audio from this frozen candidate.
Official design references and detailed plan: outputs/ship-art-pilot/ART-DIRECTION.md.

Latest ship-art human references (2026-10-13): preserved in
outputs/ship-bespoke-development/references/. Sharp framed orange/red spacecraft
sets the fighter design direction; separate ivory/gold layered flagship sets
luxury direction. Do not make every hull a utility crate or recolour player paint
to match the reference. User requires bespoke lush individually mapped pixel
art and richer silhouettes, not shading alone. First volumetric utility Adder
native R&D captures are in outputs/ship-bespoke-development/capture/. Concept,
runtime prototype and hardware acceptance remain explicitly distinct.

Fleet artwork scope verified: 44 designs = 29 standard named spacecraft +
12 named Mega hulls + 3 NPC large-freighter styles. No player/NPC double counting.
Individual 44-hull briefs and status: outputs/ship-bespoke-development/FLEET-ART-ROSTER.md.
Only Adder is a native prototype, none final accepted. Next Viper fighter
geometry blockout is authored in outputs/ship-bespoke-development/viper/,
with explicit physical panel assignments, engine roots and navigation lights.
Viper artwork/native integration remains pending. Whole-fleet rollout requires
a bounded near-ship asset cache/preloading plan; do not compile all texture
banks into the original-PSP executable. Cache/performance not yet implemented.

Ship trails/distant-light checkpoint: isolated outputs/ship-bespoke-development
now has tapered additive wakes, steady distant cores, separate navigation blinks,
fading coloured tails and a bounded shared raster budget. No per-frame allocation.
capture-distance-final contains six native poses plus 24 staged flyby frames;
lossless WebP RGB equivalence verified. Real draw-path blink/core and foreground
occlusion checks pass (RESULT 0). Optimised renderer-only emulator means 8.6-18.0ms,
exclude audio/display; crowded/sustained/hardware performance unverified.
See TRAILS-AND-DISTANT-LIGHTS.md and gallery.html there. Not production integrated;
released game EBOOT, current gameplay, audio and saves unchanged. Fleet 44 hull
artwork project remains unfinished.


Graphics integration checkpoint (2026-10-13): station component art, sun-matched lighting, bespoke Adder NPC pilot, tapered wakes and distant steady cores/navigation blinks are now integrated into canonical src. Supersedes earlier notes saying these renderers are isolated only. Local retail 2.5.453 playtest: outputs/releases/2.5.453-graphics-playtest/ELITE-NEXT-PSP/EBOOT.PBP; READ-ME.md records evidence and limits. Original-PSP-model PPSSPP startup/music/countdown/concurrent saving pass; populated race 57.77 FPS; busy music stress 31.18 FPS with zero buffer starvation/decoder errors. Native station checks 119 pass; staged ship occlusion/blink checks pass. Remaining 43 fleet designs and physical PSP acceptance are unfinished; station FPS still needs work. No gameplay/save/audio/ERL protected source changes. Stable VERSION/dist remain 452 pending full gate; no remote sync/release claim.

Graphics checkpoint gate: compared full-review core baseline/candidate, both have identical 18 failure labels; no added core failures. Full input/performance suite remains unaccepted. Highest-priority remaining graphics work: validate on physical PSP, improve crowded/close station frame time without audio starvation, then bounded whole-fleet art rollout (43 hulls unfinished). Preserve stable 452 until full release gate; do not tag/push this playtest as a fully accepted release.


2.5.454 local audio click fix (2026-10-13): integrated graphics 453 retained; audio stereo gain/pan multiplication overflow corrected with wide arithmetic in event and continuous layers. Native full-range probe 1,821,015 cases passes; real authored effects expose 39 vulnerable full-pan samples. Exact-retail original-model startup, MP3 and OGG continuity tests pass with zero buffer starvation/decoder errors. Captured PCM has no full-scale clipping or >half-scale adjacent steps. Host test buffering separately increased to 1024/ExtraAudioBuffering. Busy flight remains 24-30 FPS and physical PSP listening pending; do not claim every scene/source crackle-free. Local package outputs/releases/2.5.454-audio-click-fix; preserves saves, ERL, artwork and unrelated work. Stable VERSION/dist remain452 due existing full-suite failures; no GitHub release/sync claim.


2.5.455 space backdrop local playtest: cached system-varied clouds darkened for cleaner space; bilinear subpixel ordinary-flight stars reduce camera-turn popping; existing gentle twinkle/dust retained. Game-renderer native sky continuity/clip/guard checks and256system catalogue pass. Warm sky3.91ms; cold generation176.85ms remains a first-view/transition limitation. Actual retail MP3/OGG buffer continuity pass (30.37/24.05FPS busy scenes); race countdown/save test57.11FPS,4saves,zero>100ms. Audio454 and protected gameplay/save/ERL files unchanged. Native captures and EBOOT: outputs/releases/2.5.455-space-backdrop. Physical PSP, first-view prewarming and heavy station performance remain open. Stable452 and GitHub unchanged due existing broader failures.


2.5.456 local FPS/stability pass: station edge divisions hoisted per triangle, deterministic opaque station depth ties, initial Lave sky prewarmed during startup. Three matched native station views pixel-identical with station-stage savings0.8-1.0ms (4-7%).119 station checks and native sky/star clipping/continuity/guards pass. MP3 busy flight30.81FPS (prior30.37), no buffer starvation; OGG24.03FPS,worst113.72ms, no audio gaps but still slow. Initial sky draw5.48ms (prior cold176.85ms), generation moved tostartup; different-system cache cost still open. Local EBOOT/evidence outputs/releases/2.5.456-station-fps. Protected gameplay/audio hashes unchanged. Physical PSP, crowded station/OGG FPS and broader legacy regressions unfinished; no glitch-free/all-green or GitHub sync claim. Stable452 preserved.


2.5.457 local background freighter art: muted sunlit metal faces, drive collars and distance-limited ribs/glazing; shorter tapered budgeted wakes behind hulls, rear-axis engine flares after hulls, before planets. Existing4 deep-sky silhouettes/routes retained; not a full bespoke textured fleet completion. Six native staged cameras and24lossless motion frames/guards pass. Retail memory guard and MP3/OGG continuity pass, zero starvation/decoder errors;30.50/23.76FPS busy scenes, heavy station/OGG performance still open. Audio454,sky455,FPS456 and protected gameplay/save/ERL hashes retained. Local EBOOT/gallery outputs/releases/2.5.457-freighter-art. Physical PSP and exhaustive occlusion acceptance pending; stable452 and GitHub unchanged.


2.5.458 local textured background freighters: perspective-correct six fitted component pixel-art tiles with32/16/8mips, shared quad mip choice, dedicated readable metal palette and existing world-sun face shading.4032bytes texture data, no external assets/per-frame allocation. Retains457wake/flares, audio454,sky455,FPS456 and protected gameplay/save/ERL hashes. Native six staged poses/24lossless frames and guards pass. Retail MP3/OGG zero buffer starvation/decoder errors;30.48/23.67FPS busy scenes; heavy station/OGG still slow. Memory guard passes with5268bytes margin. EBOOT/gallery: outputs/releases/2.5.458-textured-freighters. Shared fitted component art on4existing silhouettes, not full bespoke fleet completion. Physical PSP and exhaustive temporal mip/occlusion acceptance pending. Stable452/GitHub unchanged.


Further beautification concepts (2026-10-13): outputs/space-beauty-next/gallery.html has 16 NEW native 480x272 staged captures using latest458 assets, two views each of station-gap sunlight, Adder/material lighting, continuous planet atmosphere, night-side shading/cities, rocky atmosphere/moon, regional sky, textured freighter and salvage vista. Experimental overlays are isolated in outputs/space-beauty-next/src only. Canonical source hashes unchanged; playable458 unchanged. Framebuffer guards pass; diagnostic timing includes BMP writes, not gameplay pacing. No physical PSP/performance/integration acceptance.


2.5.459 local planet lighting: approved sun-driven day/night terminator integrated into opaque sprite texels, palette-related5pixel atmosphere glow before each disc. No shading of foreground ships/menus; high-contrast bypass; tiny normal/gain lookup allocated once with safe fallback. Retail memory guard passes by676bytes; avoid static growth. Actual MP3/OGG continuity logs packaged, no audio gaps; near-planet performance/hardware acceptance pending. Four isolated glow variants at moderate/far ship ranges,192nativeframes/guards/lossless clips: outputs/releases/2.5.459-planet-lighting/previews/gallery.html. Fixed preview hue/shimmer not installed settings; installed atmosphere uses world palette. Preserves458and protected gameplay/save/audio/ERL hashes. Physical PSP, close-planet FPS, exhaustive visual acceptance and broader legacy regressions unfinished; stable452/GitHub unchanged.


Travel visual style board (2026-10-13): outputs/travel-style-board/gallery.html, nine staged native subjects: departure, passing ships, cruise, freighters, planet approach, moon, asteroid salvage, boost, hyperspace.18stills+144lossless animation frames; framebuffer guards pass. Source hash comparison confirms canonical unchanged. Experimental lighting/speed/slipstream overlays isolated in its src. Fullscreen slipstream hides cockpit in preview only; no approved control/HUD change. Latest playable459 retained; design review pending, not performance/hardware approval.

Hyperspace design study: outputs/hyperspace-design-lab/gallery.html compares current renderer with Ion Slipstream, Solar Fold and Nebula Passage. Isolated PSP lab, 388 native captures, framebuffer guards pass. Motion is staged/editorially timed, not frame-pacing evidence. Canonical source unchanged. Route timing preserved in study: 5s charge + 2s transit + 3s braking. Candidate seamless destination blend and performance validation remain planned; no playable build promotion.

Revised immersive hyperspace lab: outputs/hyperspace-immersive-lab/gallery.html. User rejected quiet black-cut alternatives. New isolated native prototypes add continuous radial scene warp, bespoke 128x64 indexed panel atlas, software per-pixel tunnel lighting, depth fog, hoops and wakes. Cyan / warm faceted / twisting violet alternatives. Reticle excluded from warped layer. Destination is staged during braking: actual arrival integration pending. 388 captures, guards pass; canonical source hashes identical. Approx 1.6MiB exploratory heap scratch requires optimisation; no retail performance or physical PSP claim, no playable update.

Ion Cathedral selected; motion revision at outputs/hyperspace-ion-motion/gallery.html. Isolated native study: continuous integrated acceleration, world projection widens with speed, reduced central magnification, brighter entry surge at4.95s and crests5.85/6.65s. 300 native frames over10 logical seconds, framebuffer guards pass. Editorial playback is not frame-pacing proof. Canonical src unchanged. Retail integration/memory optimisation/physical PSP remain pending.

Ion Cathedral punch-launch study at outputs/hyperspace-ion-launch/gallery.html. User wants fast approach then snap/huge FOV propulsion, space becoming tunnel. Isolated study now starts visual speed high, snaps over0.25s near charge completion, has expanding light front, outside-in tunnel formation, actual scene light-seeded trails and source view receding into wide projection. Top24px instrument strip protected after catching a distortion bug. Canonical source unchanged; real speed inheritance, jump-state integration, memory optimisation and physical PSP checks remain pending.

Combined native hyperspace/music follow-up: MP3 and OGG pass, zero buffer starvation/decoder errors. Real production charge/arrival and guards pass. Render-only fixed-timestep means32.125/37.868ms, worst295.105/292.578ms (cold sky). Exact retail main-loop music checks separately pass; physical PSP still pending.
