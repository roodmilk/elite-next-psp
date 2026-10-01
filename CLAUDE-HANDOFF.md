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
