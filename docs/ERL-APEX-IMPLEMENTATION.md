# ERL Apex implementation — 2.5.416 development

Latest playable release remains 2.5.415 until the final retail candidate is verified and frozen. This record describes development, not a physical PSP certification.

## Authority and preservation

Read START-HERE, LATEST-APPROVED-DESIGN, HUD-FIRE-AND-POSITION-MOTION and the full implementation specification from ERL-APEX-IMPLEMENTATION-HANDOFF.zip. Inspected all 112 frames of the full pixel-lossless concept sequence directly in ordered contact sheets, plus the native still. Older red pads, short timed FOV and earlier HUD alternatives are superseded.

Keep the current pink/purple/cyan menus, per-system seasons, V37 explicit save stream, earned prizes, board sponsor rewards and completed-result checkpoint fix, Sol Rock audio, Picross and real-time Shockwave victory. No rebase/reset/stash of the shared tree. Existing 2.5.415 release remains frozen.

## Implemented source

- Wide translucent pink spline deck, fading shoulder bands, luminous pink centreline and moving Flow Ribbons. Ordered broad sector/lap checks replace ordinary floating ring artwork.
- Curved Ion Cyan floor pads share the same sampled surface and forward swept trigger. High/side/backward/stationary passes are rejected. One boost/repair choice per actor and passage; consumption persists in existing save words. Speed kick is 0.9 seconds; reserve never increases on a pad.
- Reserve drains 25 percentage points/sec while manual boost spends. Recharge is 4 points/sec after a 0.55-second cooldown, suspended during pad/item boost; exhausted held R still requires release before restart.
- Square held drift strengthens bank and lateral slip without cancelling boost or imposing additional reserve drain. Triangle toggles camera; Circle toggles true rear mirror; X retains actual item/fire handling. Drift command is transient; saved pose/roll retain movement state without changing V37.
- Centred heading-aligned chase camera, slight drift pullback/tilt; speed/state-linked sustained FOV. Peripheral magenta/violet rush, road-only boost highlight echoes, short pad-contact shake with level HUD/mirror.
- Bounded historical nozzle-rooted fiery trails and actual outlet flares. Authored Cobra twin and asymmetric Needlefish outlet, actual capital-part roots; bounded world sparks/embers and projected hull silhouettes. Shared capital geometry now supplies actual tapered hull support rather than ordinary proxy bounds.
- Native Apex HUD, 200x40 rear viewport, filled blue item bolt, real reserve fire, actual rank bump/glints and glyph-masked shimmer. Mirror pad geometry is flushed before restoring forward-camera state.

## Verification in progress

149 production integration checks passed against the first Apex physics revision. Four physically steered held-boost controller courses took 102.58–118.32 seconds and placed 2nd/5th rather than guaranteeing victory. This result predates the final visual/collision refinements and must be rerun before release.

The corrected native review passes all 25 Apex checks. Eight native groups also pass: ERL console, arcade visuals, holographic track, actual flight controls/save, Shockwave victory, LocalTV and progression. These results belong to the pre-polish development candidate, not a finished visual acceptance.

The populated retail combat test failed the 55 FPS requirement: 29.59 FPS, worst 83.42 ms, zero frames above 100 ms, eight completed checkpoint saves, Sol Rock MP3 station 8, safe worker stack. Floor rendering averaged 6.400 ms per view. The 32 MiB retail image memory guard passed. Do not ship this candidate.

Current optimisation/polish: exact half-open floor spans to remove doubled translucent edges; draw each travelling Flow Ribbon highlight once per section rather than once per tessellation cell; cache immutable curved pad geometry; use filled paired cyan chevrons, a clearer centred chase composition and authored flame silhouettes. These changes are compiling and have no measured performance claim yet.

Remaining: visual acceptance against the lossless reference (the user explicitly prioritises cleaner track readability), engine/paint comparisons, final mode/control/save regressions, rerun production integration after final physics changes, populated retail performance with mirror/MP3/autosaves, exact EBOOT startup/memory checks, frozen release evidence. Physical PSP testing remains pending. Latest playable stays 2.5.415.

Second populated candidate (with native capture I/O) measured 31.85 FPS, floor 8.215 ms/view, zero >100 ms frames, seven/eight saves completed by the stop point. It is still rejected for release. The next candidate resolves nearest floor depth before one alpha blend, hides farther road lines behind nearer deck, reduces core brightness, bounds near-camera exhaust glow and records pad/effect phase timings. Benchmark pending.

The first nearest-deck implementation exceeded the unchanged retail memory guard (3,005,864 bytes headroom versus the required 3 MiB). It was not packaged. Reusing the flushed triangle scratch arena restored the guard: image 13,534,808 bytes and headroom 3,242,408 bytes. Native floor clarity improved, but this candidate measured only 22.42 FPS; floor 11.982 ms/view, pad 1.592 ms/view and effects 1.315 ms/view. All ten checkpoint saves completed and no frames exceeded 100 ms. Still not releasable.

Current refinement shares nearest depth across 2x2 interior quads while retaining separate pixel edge coverage. It resolves alpha once per visible quad, reuses the existing triangle arena, and interpolates ribbon line depth over short spans rather than reciprocating for each pixel. Full phase timings were added to the disposable performance fixture. Compiling; visual and performance acceptance pending.

The first quad coverage candidate hit a genuine offscreen clipping memory fault in populated emulator testing and was rejected. Empty projected bounding boxes and invalid scanline spans are now rejected before scratch addressing. The corrected review passes all 25 Apex checks including framebuffer guards. Its native captures still failed the user's visual standard.

Latest user reference was inspected from the original native 480x272, 112-frame apex-quality-hud-lossless.webp. Current redesign uses one perspective-correct lateral-opacity strip rather than five separate band meshes, with 32 near tessellation segments (16 in the mirror) and sparse distant LOD. This allows smoother bends with fewer submitted triangles than the preceding five-band approach. It also attenuates stars through a dark purple core, fades shoulders, adds a near perspective grid, gentler floor banking, heading-aligned chase framing and faint real-nozzle flares. Source is compiling; no new FPS or visual acceptance claim yet.

The lateral-opacity strip revision passes all 25 native Apex checks, including guarded framebuffer coverage and actual input/save/pad checks (run-erl-apex-review-175038). Its native third-person capture remains below visual acceptance: excessive grid convergence obscures the horizon. The next revision replaces tessellation-dependent cross lines and tightly packed near lines with a sparse world-spaced 1,325-by-1,400-unit lattice. Syntax passes. Retail build and populated benchmark pending; no release has been replaced.

Populated lattice candidate run-race-performance-175521: 20.01 FPS (FAIL), floor 14.351 ms/view, pad 1.693 ms/view, effects 1.544 ms/view; worst 102.25 ms, one frame over 100 ms, none over 250 ms, ten/eleven checkpoint writes, Sol Rock station 8, countdown mask 30, worker free stack 261,816. No memory fault recurred. Retail budget passed: image 13,478,120, headroom 3,299,096. Capture showed missing near grid: behind sections were spending the bounded line/raster budgets before the current section. Current section is now submitted first, then forward/adjacent sections; syntax passes. This is not a released build.

Current-first revision also passes 25/25 native checks (run-erl-apex-review-180110), but captures remain below target. Near lattice is now restricted to the visible 40–8,000 camera-depth range at 240-unit lateral / 600-unit longitudinal spacing, avoiding kilometre-wide near cells. Quad depths and lateral opacity interpolate across short spans, and occluded farther samples no longer spend the visible-floor budget. Diagnostic non-LTO visual build is separate under outputs/erl416/visual-fast; it must not be distributed or used for final performance claims. Optimised retail build and final acceptance remain pending.

Native fine-grid diagnostic passes 25/25 (run-erl-apex-review-181629), including new actual-mesh cyan Cobra and magenta Needlefish captures. Grid density was then reduced slightly and faded by camera depth; race-only ambient lighting increases real paint readability and the floor-projected shadow is stronger. Optimised retail run-race-performance-182127: 21.43 FPS, floor 12.961 ms/view, no >100 ms or >250 ms frames, ten/eleven completed checkpoint writes, station 8, countdown mask 30 and safe stack. Still FAIL; do not distribute.

Experimental PSP GE floor path is being tested in src/erl-apex-ge.h. It reuses the flushed triangle arena, clips projected floor polygons, blends three core/shoulder bands with GPU depth testing, exchanges only the active viewport with the software depth surface, and retains the software fallback. Diagnostic RAM framebuffers stage through VRAM for real native captures. Adds libpspgu to both build scripts; no GUM/matrices/textures/heap asset loads. This is unverified development source until native depth/viewport guards and exact optimised retail memory/performance pass.

GE prototype native runs: first direct VRAM readback did not expose the deck correctly in PPSSPP and was rejected. Explicit GE colour/depth transfers and standard viewport setup restore visible deck output. run-erl-apex-review-182703 and the corrected GE transfer run both pass 25 current checks, but those checks do not yet establish GPU/software depth exchange correctness. Add a foreground-depth occlusion canary before trusting the accelerated pass. Final optimised FPS, memory, startup and full native regressions remain pending. The CPU candidates remain rejected. User re-emphasised Wipeout Pure/Pulse-level polish: use PSP techniques and retain clear silhouettes; do not lower the visual acceptance threshold.

## 12 October: magnetic road / jump motion handoff

Read ERL-45-COURSES-HANDOFF START-HERE, EXTREME-TRACK-DECISIONS, DESIGN-AND-INTEGRATION, catalogue, schedules, motion and quality documents. Extracted under outputs/erl45-handoff. Verified Downloads/g1, g2 and g3 match the three latest lossless motion clips. Decoded all 156 frames of each clip and inspected complete sequential contact sheets plus the native launch/landing references. The user's latest request selects these three treatments; older proposal labels do not override that instruction.

Required implementation: explicit per-section holo/magnetic surface metadata; solid charcoal/violet mechanical panels with broad magenta shoulders and white emitters; attachment following the local 3D road normal without locking lateral steering/boost/drift; punchy entry with world-only shake; explicit ramp release into real airborne movement, lateral control and manually boosted forward speed; marked launch kick without reserve refill; readable landing ribbon; speed-preserving clean landing with suspension dip and pink-white sparks. Keep actual ship paint/nozzles, true rear view, level HUD and clear central first-person view. Do not turn every crest into a launch. Do not snap airborne actors onto nearby stacked roads.

The JSON's 45 routes remain draft polygons, not finished playable courses. It lacks loop/corkscrew/branch/jump progress and orientation data. Begin with separate beginner, mid-league and Last Horizon benchmarks rather than claiming all 45 implemented. All championship rounds remain at their home system; persistent two-slot local offers and additive league unlock migration need separate implementation/verification. No save format was changed in this inspection stage. Latest playable remains 2.5.415.

GE1 retail run-race-performance-183531 measured 29.87 FPS (FAIL), floor 8.540ms/view, worst 66.73ms, zero >100ms / >250ms stalls, eight/eight saves, Sol Rock station 8 and safe countdown worker stack. Retail memory passed (13,499,856 image, 3,277,360 headroom). New foreground canary exposed stale CPU/GPU framebuffer ownership: deck overwrote a nearer green pixel; the previous 25 checks were insufficient. Explicit depth transfer alone and orthographic drawing alone did not fix it. Also corrected alpha construction (RGB macro already sets alpha=255). Current experiment shares the existing Command Deck/victory scratch framebuffer for explicit colour upload/readback in one synchronized list; no extra full-frame allocation. It is compiling, not yet verified. Keep this renderer unshipped until depth/colour/viewport checks and retail performance pass.

## First round course checkpoint — 12 October, 19:35

Latest user priority narrows this checkpoint to the first round course: exactly one quarter of each lap is a magnetic road, including an explicit booster launch and gap. New layout ID 4 is stored in the existing V37 race layout field; already accepted layout 0–3 races retain their saved geometry. New ERL layout-0 events use the circular course. Menus, progression, rewards and save format are preserved.

Implemented: closed circular road; charcoal magnetic panels, magenta shoulders and white emitters over the quarter-lap sector; marked cyan ramp strip; real launch direction and speed kick; airborne gravity and steering; landing without an arbitrary speed penalty; no reserve refill from launch. The remaining road uses a sparse purple grid. Native acceptance first pass: 33 checks passed, including 2,903 game units of airborne clearance, successful landing at normal cruise, V37 mid-jump roundtrip and framebuffer guards. Full-lap testing and retail performance remain pending. This is development work, not a delivered 416 release.

The earlier experimental GE depth path remains disabled. RAM-based legacy Apex benchmark was 20.20 FPS (failed), so that candidate is not approved. The focused round-course renderer avoids that costly legacy path; its actual performance must be measured independently before distribution.

Second focused native pass: 34/34 pass. Actual game_tick steering completes both laps in 106.65 seconds, player gate 32, first three rivals at gates 29/30/29. Jump maximum clearance 3,451.48 game units; successful landing retains 5,600-unit cruise speed. Initial native captures accidentally retained the HQ page; corrected page=FLIGHT captures were inspected at 480×272. Launch/airborne views demonstrate the real road gap and magnetic panels, not mockups. Landing capture timing is being adjusted to show the settled camera rather than its transient downward airborne attitude.

Final round-course regression: 380 checks pass across Apex (34), full ERL console/all-hull gameplay (147), arcade visual (19), legacy holo-track (15), flight/control/save (39), Shockwave (23), TV (45), and progress/hazard (58). Repair/hazard fixtures explicitly use the retained legacy course; curved-wall placement uses the true track frame rather than the straight checkpoint chord. The first round course intentionally has no legacy floating hazards or repair fork.

Timing clarification: the focused jump test starts at section 6.75, so its 106.73-second finish is a partial-course continuation, not a full starting-grid race. Its test label has been corrected. The separate 147-check suite starts every player hull at the actual grid and records complete new circuits around 131–135 seconds, gate 32, with competitive rivals. Use those full-grid measurements for player-facing duration claims.

The circular course now uses exact analytic circle frames and bounded ordered nearest-point lookup, retaining the general spline path for legacy courses. Magnetic core and pink shoulder panels are non-overlapping geometry; this prevents equal-depth base triangles from overwriting the shoulder art. Final retail timing and native shoulder captures are pending as of this note. Earlier retail tests recorded 40.98 FPS (opening section) and 42.73 FPS (magnetic jump), without >100 ms stalls, with music and asynchronous saves; both missed the 55 FPS target.

## Playtest delivery 2.5.416

ELITE: NEXT 2.5.416 — first round-track PLAYTEST checkpoint

Replace EBOOT.PBP only. Keep commander saves, settings and music folders.
Start a new first-course ERL circuit (layout 0) to get the new round track.
Already-accepted saved races retain their original course; finish/withdraw the
existing event before accepting a fresh one. No new commander is required.

Included: circular first course; quarter-lap magnetic sector; charcoal road,
pink shoulder panels, emitters and marked cyan booster ramp; genuine airborne
jump across a gap; forward speed kick with no reserve refill; landing without
an arbitrary speed penalty; real steering, boost and drift; both race cameras.
Other ERL screens, rewards, V37 progression, Shockwave and Picross are retained.

Verified in PPSSPP, not on a physical PSP:
- 380 ERL native/gameplay/control/save/victory/progress checks pass.
- Complete grid-to-finish all-hull circuits run about 131–135 seconds.
- Five exact-retail startup/memory/MP3 checks pass on the 32 MiB model.
- Populated magnetic/jump retail test: 40.93 FPS, worst 50.05 ms,
  zero frames over 100 ms, six of six checkpoint saves completed,
  Sol Rock station 8; countdown 4/3/2/1 and actual jump confirmed.
- Minimum startup kernel free memory: 3,106,560 bytes.

UNFINISHED: 55 FPS performance target (the retail benchmark reports 1 failure).
Full Apex visual acceptance, the complete Arcade handoff and all 45 courses
remain in progress. This is a playable preview, not an all-green final release.
Whole-game smoke was not rerun for this checkpoint. Physical PSP confirmation
is still required. GitHub was not updated.

SHA256: de1f608dfb434f6811dff7c4ec302bc917c5f76b7eaeca14071a744832eed7b6

