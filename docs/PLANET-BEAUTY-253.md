# Planetary beauty pass — 2.5.253

## Implemented

- Lave I: broader 1100m observatory mountain uplift, visible authored dome from the port, real distant terrain, deterministic 52–136m trees, flower verges along the existing winding trail.
- Real coarse terrain to 7km on solid Lave worlds; larger layered biome horizon silhouettes elsewhere. Not an all-world rewrite of near terrain.
- Leaves retain a 24-particle/180m cosmetic cap. Tree crowns, ferns and flowers sway while trunks remain anchored; rock sprites do not sway. Nearby Lave I grass is capped at a 5x5-cell search and 220m rendering distance; no simulation allocations.
- Tree scenery cache follows the player at 280m intervals. Physics and scenery use the same seed, lattice and clearance reservations. This fixes invisible collidable trees beyond the old port-centred cache.
- Shared starport kit: seeded unequal terminal upper decks and domes; control crown retained. Added geometry stays within solid footprints. Low platforms are almost flush and traversable.
- Traffic reservations track the same lift used by the visible ship animation: vacant berths open, low/parked hulls protected.
- Solid-world crystal grottos become open-front rock shelters with side/rear walls, roof and crystals kept out of the centre aisle. Cave clearance accounts for walker/rover margins. Cloud settlements keep original crystal spires and platform safety.
- Fixed inherited Lave I apron interpolation: flattening now covers neighbouring 160m terrain vertices. Saves, controls, missions, music folders and other contributor work are retained.

## Evidence

Final tested snapshot: current task/planet-beauty-253. Native views in release-beauty/beauty-view-0..6.bmp; port, cave and terrain views in release-port/release-art/release-map. These are 480x272 production-renderer captures, not concepts.

Final QA: backend real collision checks, seven scenery views/leaf budgets/framebuffer guards, all1024 port foundations and ship fits, all1024 worlds/8192 species plus12 POI families, map, rover, 64-world cinematic transfer sampling, expanded worlds, and144 reference/fast native pixel comparisons all report RESULT0. Game594, radio54, steering18 pass.

Starting port baseline failed Lave I foundation checks; candidate fixes them. Initial new review harness omitted its disposable framebuffer allocation and stalled; corrected before final validation. Map's old exact6m platform-height assertion was updated to test the new flush, traversable platforms.

## Performance and limits — not certification

- Paired Lave render including cinematic modes: baseline44.241ms average/68.072worst; final46.921ms/69.381worst.
- Same20 walking-camera samples: baseline39.144ms -> final41.431ms (+5.8%). Added scenery costs time.
- Seven new wilderness/port positions, including cold cache refresh: final45.130ms average/63.234worst.
- Times are PPSSPP guest render measurements, excluding audio/display; not physical PSP FPS. Not locked30 or completely seamless. Cloud-port extreme sampled shot remains around161ms.
- Full smoke/performance run timed out after240s; inherited input/outfitting failures appeared. Only completed game/radio/steering and focused suites are claimed green.
- The 10.5km circular survey edge, water exclusions, cloud-deck edges and broad solid POI boxes still constrain movement. This removes identified unintended blockers, not every possible invisible wall.
- Trees still have bounded draw distance. The observatory is visible from the port but nearby terrain/trees naturally occlude it from some positions.
- Flower verges currently follow the observatory trail; additional authored branches, broader biome-specific distant terrain and detailed cave geology remain next steps.

## Next work

Profile on real PSP; optimise coarse terrain/overdraw, wind sprite projection and cache rebuild peaks; preserve underfoot collision/draw agreement. Validate rover routes and landing/launch with physical input. Make remaining water/survey restrictions visible, audit open ruin/garden footprints, and add seeded branch paths to other reachable landmarks. Preserve the current cumulative build rather than reverting to an older candidate.

## Delivery and recovery

EBOOT.PBP: 7347887 bytes; SHA256 9FF633A4CCBA5673FD51C76D37FA366FFC3CACD2DB0085F3A1505D06CB741EC8.

Previous source/docs/root/dist binaries are in planet-beauty-253/canonical-before-253. Source backport used narrow patches after all touched files matched the frozen baseline; canonical content was re-compared to the tested snapshot. No shared music/saves were removed and no public release or Git mutation was made.
