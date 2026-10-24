# ERL arcade expansion implementation record

## Current work: 2.5.415 gameplay fixes

User has finalized visuals in a side chat and will supply the final handoff. Do not redesign the menus or race presentation. Canonical renderer is restored to the tested 2.5.414 source; the rejected experimental visual candidates are archived in outputs/erl415/deferred-visual-experiment and must not be shipped.

Mission Board races now use the same sponsor reward allocator as league races: 400 units and the exact advertised enhanced module, or the disclosed 800-unit cash alternative when the reserved reward queue is full. Completed optional sponsors remain independent of the main contract goal; withdrawal and expiry pay no sponsor reward. The consumed contract prevents repeat payment. Collecting equipment still requires an ERL headquarters and real Item Storage capacity. Save format is unchanged.

149 production integration checks pass, including 12 new board reward, loss, withdrawal, full queue, exact instance, storage and reload checks. A newly found end-of-board save gap is fixed in source: final results mark the state dirty, retry asynchronous checkpoint submission after an older writer, and join/commit before Continue closes the result. Failed saves keep the result open with a retry message. Final validation passes: 39 actual-control flight/save checks, all existing ERL/native suites and 149 production integration checks. Exact retail SHA256 1f9042a67ddede213acf15c6e55c7dabb1efe610a12fc6c7ceeaf22368c04e2a; original 32 MiB model 57.58 FPS, worst 33.37 ms, zero >100 ms frames, 4/4 completed asynchronous saves, worker stack unused 261816 bytes. Five exact-retail startup/MP3 checks pass; minimum free kernel memory 3405824 bytes. Loaded image 13219652 bytes, heap 7340032, main stack 1048576, allowance 3557564 bytes. 2.5.415 is the new tested local EBOOT. Physical PSP remains unverified. The core log retained in this release is the 414 baseline with 15 known failures, not a fresh 415 core run. No all-green project claim.


Source: outputs/erl-arcade-handoff/ERL-ARCADE-ITEMS-AND-HUD-HANDOFF.md. Full document and PNG references inspected on 11 October 2026. References are design composites, not working gameplay.

LATEST EXPLICIT OVERRIDES (12 October): Pink holographic track beneath racers and NO large top guidance arrows now supersede the arrow/no-ribbon references below. Compact animated position, lap and live instruments follow the latest player screenshots.

Previous explicit overrides (11 October): user selected **rectangular repair tunnel from codex-clipboard-c249818b-3e16-4319-97de-3ce35c96c1ba.png**, replacing the earlier cylinder. Match solid steel rectangular ribs/footings, pink energy rails/grid/corner lights, tilted speed digits, circular LIVE light and a detailed actual rear camera. Commentary is **captions in a ticker at the top right**; no recorded voice is claimed. Separate earlier reference codex-clipboard-bb71fa96-991c-43a8-8702-ce4088a67861.png specifies the bold filled purple arrow and nearby-only curved translucent violet grid/ripple. These supersede stale handoff images where they conflict.

User decisions: preserve current pink/purple menus/navigation/animations and separate winner work. Use approved-hud.png with later corrections. Combat uses continuous corridors without mandatory rings. Repair uses the rectangular steel-framed pink tunnel on the longer detour (latest explicit override). Pursuit capture requires four continuous seconds in the lead, resetting on loss of lead. Solo championship rival times are persistent deterministic skill-based results under the same medal/points rules.

Playable release: 2.5.407 is frozen from 2.5.406 and fixes ERL branding independently of ship paint. Its 282 focused checks pass, including exact retail countdown/music/autosaves. The unfinished arcade expansion described below is NOT packaged in that EBOOT.

Development resumed after delivering that hotfix. Shared ingredient rules, integer damage, single-slot item replacement, frame-rate-independent ammunition and fixed simulated trial times are implemented in source. Fixed pools for missiles/tracers/pickups and AI item decisions are integrated into race simulation, with normalized generation-bound targeting and swept collision. These gameplay paths still require full event and visual integration; they are not claimed complete or shipped.

The V36 additive stream writes explicit words for temporary arcade state; old V35 is readable. Save validation now rejects malformed effects, targets, ammo and geometry. Expired projectiles clamp their lifetime to zero so later atomic checkpoints remain valid. Historical truncation-based test fixtures exclude the new suffix. Shared stream tests: 60 checks pass on the PSP CPU emulator (OpenGL backend after Direct3D initialization failures). The full-game integration harness passes 10 checks with production O3/LTO settings: actual released V35 migration, exact combat state reload, expired projectiles, failed-save rejection and preservation of the prior checkpoint. Evidence: outputs/erl-arcade-{core,integration}-test.log. A preliminary O2/non-LTO standalone harness faulted in the word reader before reporting; it is not a passing configuration and its underlying cause has not been established. The supported production configuration is independently verified. Current game source passes PSP compiler syntax checks.

Latest user direction: ERL branding is ALWAYS magenta/pink, deep purple and cyan across every system. Pimp-my-Ship only colours actual ship previews within ERL. Keep this exception when adding new pages and motion.

Stages: shared event ingredients and race-only item/damage state; validated additive saves; real rear view and approved HUD; pickups/AI/corridors/repair; event catalogue and physical staging; independent contracts/season continuation; persistent prizes/career/integrations; native visual and performance verification.

Final user decision: implement Needlefish only as the new first prize; retain existing later prize ships and all earned Krait ownership. Inspect commentary capability before claiming speech. Preserve other winner-animation work.


## Current completion audit - 12 October, 2.5.414 playable checkpoint

Latest playable local checkpoint is 2.5.414. Full handoff acceptance remains open. The current source contains the work below; source presence is not evidence of PSP hardware stability.

Current candidate evidence: 137 production gameplay checks pass, including four complete physically driven clean races, 720 catalogue/licence combinations, 1536 board-rule variants, exact V35/V36 migration to V37, local championships, away staging, objective dispatch, payments and rewards. Board races now persist exact pose, timer, checkpoint, reserve, ammunition and RNG and reload paused. READY, periodic, sleep and result checkpoints use the same valid save gate; confirmed withdrawal fails only that contract and returns its ordinary 10-unit deposit, without a race prize.

Rivals now deliberately collect their own green-ring refills instead of exhausting reserve while aiming only at checkpoints. Their 82–94% cruise personalities and restrained forward attacks are race-only. No hidden recharge or player-relative speed correction is used. Four held-boost controller runs finish in 126.83–135.46 seconds in positions 6–7, with all six rivals physically reaching the final quarter. The controller does not activate collected items; these results demonstrate competition, not human difficulty approval. Source acceptance still requires player playtesting.

The latest exact retail candidate passes the original 32 MiB memory guard (13,219,336-byte loaded image, 7 MiB heap, 1 MiB main stack; 3,557,880 bytes of system/codec/thread allowance), startup/MP3 checks and populated countdown/music/four-save stress testing at 57.58 FPS, worst 33.37 ms, no frames over 100 ms, worker unused stack 261816 bytes. Retail SHA256 `4eaa806a4f2749d3c7b7e14d3957eae07e190a4b380c37dd508ebeef30240462`. Diagnostics alone use 64 MiB because embedded fixtures/render buffers exceed the retail memory footprint; do not substitute that EBOOT for the retail executable.

Native acceptance includes the real rear camera, 48/1/0 ammo, missile lock/no-lock, active boost/shield break, 10/11 placing, course/wall/repair scenes, truthful board briefs, Driver, away itinerary, paused board reload/withdraw, isolated replay/LocalTV and preserved Shockwave. The historical full console suite has been updated for Needlefish, exact enhanced rewards, deliberate local-season return, actual timed-drone rules and legacy Governor compatibility. Its final run passes all 147 checks. Native HUD, floor, flight-state, Shockwave, LocalTV and progression suites pass. The core suite retains the exact same 15 baseline failures; V7/V14/V28 migration fixtures and the appended Needlefish catalogue check now pass. Older V18/V19/V20/V21/V22/V23/V25/V26 fixtures still fail and must not be claimed verified. The wider input smoke run timed out and is incomplete; no all-green suite claim is made.

Remaining selected presentation work after this playable checkpoint: stronger depth-correct rival neon outlines; coherent venue lighting including sun/eclipse variants; remaining pickup/shield/gate sparkle refinements and native readability/performance review. Full ZIP acceptance is NOT complete, and physical PSP confirmation remains pending. Preserve the current menus, earned ships/modules, Shockwave and Picross while finishing these details.

| Area | Implemented and verified evidence | Outstanding acceptance |
|---|---|---|
| Current menus, colours and saves | Preserve 413 pink/purple/cyan pages, navigation and ship paint | Recheck new labels/captures; hardware |
| Ingredients and courses | 720 valid licence/catalogue combinations; live hunter/escape/drone/endurance/solo acceptance | Full combinations remain broader than individual live fixtures |
| Combat/items/AI | Integer damage, 48 rounds, independent held/active items, AI, actual swept shots | Expanded native effects/performance inspection |
| Floor, walls, repair and boost | 413 native floor tests; rectangular tunnel; persistent mutually exclusive branches | Recheck moving repair sweep and finite floor edges |
| Contracts | Top-three, named-Rafe, timed, finish-with-takedowns, solo drones, eliminator, hunter, escape and endurance; 1536 valid shared-rule variants; actual exact-once payment checks | Expanded native brief captures and final retail regression |
| Eliminator/pursuit/escape | Saved final sprint, eliminated projectile removal; four-second capture/reset | Full live objective suite and finals presentation |
| Seasons and prizes | Independent systems; reachable later away rounds, disclosed destinations; accepted course/save/arrival/home-only points tested | Native final itinerary/reward checks and hardware |
| Career and sponsors | V37 records/titles/course bests/system-specific rival identities; old milestone migration; reserved championship prize capacity | Final native career/roster acceptance |
| LocalTV | Factual result/sponsor inserts; matching bounded actual replay; 19 native replay/HUD/isolation checks and schedule regression pass | Broader authentic finish footage/readability and hardware |
| Replay and winner | Existing isolated 11-second replay and Shockwave retained | Regression with new hazards/protection passes |
| Race presentation/audio | Compact HUD, distance-weighted gold pace, mirror warnings, small Boost FOV, repair sweep, smoke, ghost protection, shield shards, cue duck | Visual states and original-model performance; remaining selected art effects |
| Remaining integration | Away venues, pool-correct shuffle, pickup pull/reveal, minigun vibration, resource-free near-miss cues added | Additional selected local dressing, full native/effect/audio regressions and package |

Stage evidence before the newest effects: `outputs/erl414/integration-test.log`, 68 production PSP CPU checks, zero failures. Includes real third-place crossing and exact contract payment, no double pay, persisted no-immediate-retry rule, exact sponsor instance, one-time career/public updates, invalid V37 save rejection preserving the prior checkpoint. New live mode/hazard/pace tests and full review/retail builds are in progress; they must pass before packaging.

V37 appends explicit little-endian progress data after the unchanged V36 streams. Existing V1-V36 saves remain readable; no historical best times are fabricated. Contracts accepted under older saves retain their first-place goal. New race Job target values 40-47 are race-only goal IDs within the existing validated target range. The final target offers escape at even-numbered home systems and endurance at odd-numbered systems, derived from the saved Job origin. Board attempts refresh on a twenty-minute UTC bucket and only consume their attempt at READY/countdown, not merely on acceptance. Endurance adverts pay twice the local base race reward; the accepted Job stores the exact amount.

## Development acceptance evidence, 11 October (2.5.408 review only)

Latest player requirement: every Mega Capitol SYSTEM owns independent seasons. Global licence, XP and owned fleet remain shared; round, points, championship seed, registered ship, offer, fixed trial times and local history are stored per system. The active legacy fields are a view of that system, with old V35 seasons migrated to their original system. Accepted/running attempts keep their original context.

Production O3/LTO domain harness: **33 checks, zero failures**, `outputs/erl408-domain-test.log`. Includes two rounds in system A, one in B, both directions of travel, atomic reload of both records, V35 migration, malformed-save rejection, all 720 catalogue/licence combinations and nonzero orthonormal course geometry. One-lap interpolation fixed duplicate checkpoints. Actual energy-wall deflection causes no hull/shield damage; cylinder exit repairs and excludes collecting the same section's boost; branch state persists.

Exact pre-entry enhanced equipment offer survives the complete championship, pending/full-storage state and reload, then transfers once into real Item Storage. Tests compare all eight persisted instance fields individually. Needlefish is appended without renumbering any existing hull or mesh, with asymmetric authored geometry and single offset engine hardpoint; later prize hull indices are unchanged. V1/V2 cosmetic preferences remain readable; V3 appends Needlefish paint.

Native Vector HUD/rear mirror: previous review captures in `outputs/erl397/run-erl-arcade-visual-221602`, five passing checks for real entry, read-only mirror, live rear movement, actual X item use, framebuffer guards. A fresh full review build is being checked against the additional changes above. This is still development evidence, not a new playable release or physical-PSP confirmation.

Remaining acceptance work includes complete event/contract objective tests and staging, authentic bounded replay/pregrid sequence, remaining item/ship effects and audio, career integrations, full native menu/HUD verification and final retail performance/boot/countdown checks. Do not mark the full ZIP complete on the strength of these focused checks.


Replay development check: nine native checks pass in `outputs/erl408-replay-test.log`, including bounded twelve-second capture, playback before the existing reveal, three render-only camera cuts preserving authoritative pose/RNG/checkpoints/standings/payments and a consumed X skip. The 121-frame buffer stores compact real action, never full-frame video or full Game snapshots. Native cut PNGs are in `outputs/erl397/run-erl-arcade-visual-225145`. Camera framing still needs stronger ship readability, particularly the trackside cut; these captures are development evidence, not final visual approval. Existing trophy/checkerboard functions are unchanged.

The prior retail candidate (before replay) passes original 32 MiB memory budget: loaded image 12,757,484 bytes, 7 MiB heap, 1 MiB main stack, 4,019,732 bytes remaining for system/codec/thread overhead. Countdown with real Sol Rock MP3 and four asynchronous saves: 59.94 FPS, worst 19.02ms, zero frames over 100ms or 250ms; worker stack has 261,816 bytes free at its measurement point. Evidence `outputs/erl408-retail-countdown-vulkan.log`. A final retail rebuild/performance pass is required after replay and remaining changes. OpenGL/Direct3D initialization intermittently failed in this emulator session; Vulkan produced the successful native and retail runs. Diagnostics use `-EnableReview -ReviewExtendedMemory` only; retail extended memory is explicitly rejected.


## First playable scope: 2.5.408
User requested one new race now then pause. Default Arcade Events exhibition: seven-racer, two-lap COMBAT RACE, all four items, broad ordered corridor. Full actual game_tick steering-only test: 135.26s, 777099 world units, all checkpoints, first place and saved/reloaded result. 44 integration assertions pass. This is automated simulation, not hardware playtesting. Native captures: run-erl-arcade-visual-233131, actual 480x272 renderer. Wider event combinations still require live validation. Full handoff remains unfinished.

Final 2.5.408 evidence: outputs/releases/2.5.408/verification.json. Real item-input completion 140.55s (earlier fire-input-only run135.26s). Retail startup five checks, countdown/music three checks, native renderer 16 checks. Packaged SHA256 95e249feeae5b2c4ddcf4ff049578c0427108fda2791e1cc8629e747eba99b37. Broad scope paused.


## Resumed implementation, 12 October — 2.5.413 candidate

Latest player overrides: translucent pink course floor, no large top-screen guidance arrow; closer position denominator and moving numeral sheen; compact accented lap badge; shorter pink LIVE/timer frame; discreet rival health cues. Full-health racers have no health strip. Damaged nearby racers use a muted one-pixel bar, with a modest highlight on the active weapon target.

Shared spline geometry joins the authoritative checkpoint course. Floor collision allows free flight above the surface, preserves hull/shield on contact, and is tested at boosted speed in clean, combat and solo modes. Main camera, actual rear view, grid flyby and recorded replay share the derived geometry, without changing save fields or RNG. 15 native checks passed on the first candidate. 44 production gameplay checks pass after the physics change: steering/item-input combat completion in 137.92 simulated seconds, old-save migration, two-system seasons, branch exclusivity, exact prizes and result reload.

Do not treat initial performance logs with RESULT 0 as final approval: the early runner only checked stalls/countdown/saves. The runner now also requires at least 55 FPS. Intermediate candidates at 22.22, 29.07, 40.55 and 46.87 FPS are rejected. Final optimized retail verification is pending. Current rendering uses a bounded derived curve cache, incremental depth/edge planes, clipped scanline spans, packed integer translucency and separate main/rear fill budgets. Distant track edges remain visible beyond the softly filled near surface.

The entire arcade handoff is still unfinished. Remaining full acceptance includes live validation of every objective and event combination, broader career/publication integrations, complete effect/audio review, wider native-state coverage and physical PSP playtesting. Earlier menu/input/sleep fixes, Shockwave victory, Picross and independent system championship records remain in source.

Final 2.5.413 stage evidence: 15 native floor/HUD checks, 44 gameplay checks, five exact-retail startup/MP3 checks and populated countdown/performance checks pass. Race test 55.26 FPS, worst 33.37ms, no 100ms stalls, 4/4 saves and 261816-byte minimum completed-worker stack reading. Native 480x272 bend, elevated route and clean-ring captures inspected. Frozen release: outputs/releases/2.5.413. Full handoff remains unfinished; physical PSP pending; GitHub unchanged.


2.5.414 continuing validation (12 October): 106 focused production PSP CPU integration checks now pass (`outputs/erl414/integration-test-final.log`). This includes the exact V36-to-V37 payload migration, historical milestone flags without fabricated race records, all 256 systems' rival masks, reserved championship reward capacity, full live hunter/escape/drone/endurance paths, weighted gold pace, actual moving-hazard damage, exact third-place contract pay and reachable away-round staging/save/arrival/home-only points. Save assertions are separate so each stored field is independently diagnosed.

The current retail candidate, including close-exhaust clipping and bounded LocalTV highlights, passes 56.65 FPS, worst 33.37ms, zero frames over 100ms, actual MP3, 4/4 asynchronous saves and 261816-byte completed-worker stack headroom (`outputs/erl414/performance-test-final.log`). Exact SHA256: `380FDEF48CEC9B6F950AF63FF8319E20FB003FDAC98C3BE50549EFED769639B2`. This is emulator evidence, not physical PSP confirmation. Five exact-retail intro/music/boot checks also pass.

Native 480x272 Driver, factual LocalTV, permitted-item briefings, season destination map and away briefing inspected in `outputs/erl414/native-final`. Close respawn exhaust is now distance-capped and recaptured. Current regressions pass: 15 floor checks, 26 pending-race/input/sleep checks and 23 Shockwave checks. LocalTV schedule checks and 84/86/86 Picross/crossword/Sudoku distribution validation pass. The new LocalTV highlight fixture initially used unpaid practice and correctly rejected it; it is being rerun with a paid event to exercise the actual rendering/isolation path. Full handoff remains unfinished. Latest frozen released EBOOT is still 2.5.413.


## 2.5.415 presentation acceptance in progress

Bounded mesh-adjacency caches render silhouette edges after world depth: no internal triangulation, no outline through solid scenery, protected ghost hulls remain separate. Main/rear/recorded replay share this pass with 192/48 line limits. A fixed world-space sky direction and deterministic venue palettes add small icy suns, warm sunset discs and eclipse coronae; camera turns and rear view project the same sky. Only course scenery receives subtle venue material lighting. ERL branding, functional item colours and actual player hull paint remain unchanged. Pickup corners shimmer, electric-blue shield edges spark, clean/solo offscreen guidance is teal, and passed intact rings no longer redraw over the selected glass-shard effect. Native geometry/occlusion/state-isolation/performance tests are being built; these are not yet release claims.


## User steering: finalized side-chat visuals, 12 October

Further visual implementation is deferred until the user supplies their final design handoff. The unshipped 415 experiment is archived with a patch in outputs/erl415/deferred-visual-experiment. Its 54.96 and 54.19 FPS retail candidates were rejected; no 415 EBOOT was delivered. Canonical renderer files were restored byte-for-byte to the tested frozen 414 source, preserving all gameplay work. Earlier 415 visual notes above describe the archived experiment, not current canonical or shipped behaviour. Continue nonvisual event, save/resume, payouts and progression work.
