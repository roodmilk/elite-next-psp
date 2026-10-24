# Space expansion: implementation slice 272

The supplied ELITE-NEXT-SPACE-EXPANSION-HANDOFF.txt is the expansion brief. This is a staged implementation, not completion of that brief.

Implemented in this slice:
- Bounded eight-contact discovery layer, new target IDs appended after existing IDs. Existing saves and NPC/debris IDs remain unchanged.
- Authored catalogue of 75 names and short responses, imported from the brief. Four shared prototypes always present: listening post, abandoned freighter, animal transport cage and wildlife sanctuary buoy. Four additional contacts selected by economy/government/technology.
- Seeded positions outside planetary globes, rings and hub approach areas. Positions regenerate from the system seed, with a bounded cached pool and no per-frame heap allocations.
- Existing OTHER targeting band, reticle selection, lock-on and contact actions handle discoveries. Comms supports them with a 12,000 m transmission range. No mission rewards or fictitious station services are granted.
- Native wire geometry prototypes: dish and antenna, extended derelict ribs, enclosed swimming silhouette, buoy ring and blinking lamps. The other catalogue entries currently use placeholder structural geometry: bespoke artwork is NOT complete.
- Rollout limited to Lave and systems 0/1/2; tutorial has no new discovery clutter. These are development samples, not yet representative economy certification.

Remaining brief, in order:
1. Certify poor/rich/capital sample choices and bespoke discovery silhouettes; add solid-hull collision where appropriate, miniature contact animation and optional Codex records. No wreck interiors.
2. Rotate station shuttle port paths and freight-pad choreography with a protected player corridor; carry scenery to bar windows.
3. Add named scenic cruise liners and detailed freighter LOD/bounds.
4. Enlarge and rebalance system distances, map and mission units together; measure travel time and safe outer arrivals.
5. Continuous real-view hyperspace transition and monotonic charge/cancellation/fuel validation.
6. Clear ice-processing capability and dependable emergency fuel tender. Do not silently alter fuel consumption.
7. Expand catalogue art, certify integration and physical PSP performance, then allow wider rollout.

Prior ring implementation 270 is retained, including mineable rocks and landing exclusion. Ring depletion is persistent within the current system session only; persistence across revisits remains outstanding.

Validation: the opening-check harness includes bounded pool, deterministic identity/position, planet clearance, panel availability, short contact response, rollout and native cage capture. Exact binary results are recorded in BUILD-2.5.272.txt after execution. Physical PSP certification and legacy broad-smoke issues remain outstanding.
