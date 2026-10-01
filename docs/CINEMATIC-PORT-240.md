# First-person planetary transfers — 2.5.240

The earlier first-person attempt was found in the separate 2026-09-30/elite-space-work checkout. It still rendered a separate flat miniature set and a static pilot-seat panel. This implementation is based on the newer Lave 2.5.239 checkout, preserving its forests, glacial world, cloud-platforms, animal animation and discovery/save work. The other checkout and its active TV work were not modified.

## Player-facing changes

- Confirm landing: the orbital view is covered by a drifting, continuously blended cloud veil. A fully opaque frame precedes synchronous surface initialization; this is visual masking, not asynchronous streaming or seamless planetary-scale physics.
- Actual landscape: approximately ten seconds of first-person descent and forward approach through the same production renderer used on foot, followed by 2.2 seconds of braking/touchdown and automatic disembarkation. No separate flat stage or exterior ship shot.
- Boarding: a 1.05-second camera move lifts slightly into the actual ship/roamer seat; short angular easing and a restrained canopy replace the static fake windshield. The parked ship shows live terrain and traffic, with bounded nub look.
- Departure: 1.5 seconds of lift/settling, three seconds of forward scenic flight, then four seconds of accelerating pitch-up. Clouds mask the existing orbit handoff; control returns after the reveal and button release.
- Triangle boards a nearby ship. Fresh R launches only after boarding completes and buttons are released; X steps outside. X still boards/parks the roamer. No skip/menu/launch actions are processed during transfers.
- Spaceports: approximately 800m-square aprons, seven shared collision-backed buildings, a tall control tower, hangar-door detail, approach lights and wide clear lanes. Traffic pads moved outward to avoid blocking the rover garage exit. Existing garage and ship spawn identities retained. Cloud-deck footprint expanded to support the larger port.
- Static scan specimens sit beside the port instead of on the central approach lane. Their identity, scan bits, rewards and save format are unchanged.

## Safety and performance design

Camera paths are smooth Hermite segments with matching approach/touchdown endpoints. Tall generated landmarks add a smooth clearance envelope. Rendering snapshots only the small camera/viewport fields and restores them; it does not copy the large Game object onto the live PSP render stack. The actual landing, boarding, takeoff and orbital-return model functions still commit gameplay state once at the boundary.

Frame/timer steps are capped; sequence input refreshes button latches without dispatching actions. Held buttons cannot carry into an immediate second launch/boarding. Opaque cloud frames avoid rendering a hidden landscape. Integer cloud blending and removal of terrain/deck spans completely covered by the opaque apron reduce overdraw. No save format changes or external art dependencies.

## Verification and delivery

Isolated build/delivery: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Cinematic-Port-2.5.240/EBOOT.PBP.
Size: 7,056,943 bytes. SHA256: A9E1D04A178F7206A096AAF308D87FE32AE25A71C68EC8A1E2D76AD6F8D25695.

Native reports under that task's cinematic-port directory:

- final-pilot/cinematic-port-review.txt: zero failures. Four Lave worlds; actual-input cloud-entry timing, all-button spam, automatic disembark, ship/roamer seat transitions, release-gated departure, exact approach/touchdown endpoints, camera-state restoration and framebuffer guards. Existing landing safety, approach, repeated transfer, EVA controls, mineral/Codex/reward/trade/save regression tests also pass. Sampled 64 worlds across the galaxy for camera/landmark clearance. A tall site in system153/body2 was detected in the first sample and the smooth clearance envelope resolves it.
- final-worlds/: zero failures for the existing II–IV expedition activities, all nine site perimeters by foot/rover, 24 scans, 21 optional site activities, port jobs, repeat-reward protection and docked save/load. Native view guards also pass.
- final-fauna/: separate animal behaviour/art regression report.
- final-smoke/: broader model/radio/steering report; the inherited station-tunnelling failure is not fixed by this task. Input/performance groups are not claimed complete.

Native screenshot gallery: cinematic-port/final-pilot/cinematic-port.png. Arrival, touchdown and departure GIFs in that folder sample the production renderer at about three frames per second for compact review; their playback is NOT the game's actual frame rate. These are real native 480x272 framebuffers, not concept art or a manual whole-flight recording.

Measured full-scene cinematic render time (audio/display excluded), including initial scene/cache costs and opaque frames:

| Lave world | Average | Worst sampled |
| --- | --- | --- |
| I | 37.516ms | 71.494ms |
| II | 33.344ms | 60.194ms |
| III | 40.479ms | 104.396ms |
| IV | 34.690ms | 62.228ms |

The ordinary II–IV walk samples measure 26.175/27.136ms, 29.156/34.023ms and 36.216/38.810ms average/worst. Further hardware profiling and scene-cost optimisation remain; neither locked30 nor hitch-free motion is certified. Review flags belong only in fresh disposable folders; reusing reports is prohibited after the stale-buffer finding in 2.5.239.

Key files: src/planet-sequence.h, src/main.c, src/planet.h, src/planet-profile.h, src/cloud-platforms.h, src/cloud-render.h, src/surface-landmarks.h, src/surface-activities.h, src/game.c. Native proof: src/cinematic-port-review.h and tools/cinematic-port-proof.py. Existing landing/EVA input tests were updated for the new durations and pre-entry cloud stage.

Physical PSP testing, full-game regression completion and sustained 30 FPS are not assumed. Existing scene art remains PSP-scale 3D geometry and pixel billboards, not the high-resolution concept illustration. Shared dirty work and root/dist binaries remain untouched; no GitHub release, commit, push or tag was made.
