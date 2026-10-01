# Sky Observatory — The Signal Between

## Where
Lave 2: POI 3. Lave 4: POI 5. Lave 1 does not have one.
Existing seed placements are preserved: 581 observatories across 1024 planets, at most one per world. Completed old sites remain completed and do not replay rewards. Use an unfinished observatory to experience the new decision.

## Flow
X near the observatory opens the scene. Release the entry button. Inspect telescope, signal console and observation log, in any order. Each clue persists in the normal commander save. Talk to the seeded named observer, then resolve the signal. A second confirmation is required; cancelling is free.

- Restore beacon: ordinary site survey payment plus 20 units. The volatile trace is erased.
- Preserve trace: ordinary payment and one additional discovery, no 20-unit service bonus. The recording is filed in the Codex.
- Return later: branch-specific text, instrument state and Codex record; neither extra payout nor changing the choice is permitted.
- These outcomes do not spawn ships, secretly change flight AI or promise an unimplemented mission. The report explicitly leaves the trace's origin unknown.

## Art / audio
Native tools/build-planet-site-art.mjs recipes use the exact 340x168 canvas. Observatory ribs, lamp, star chart, focus rings, machinery, mug, console details and protected window mask extend the existing kit. Runtime draws a seeded biome/day/night vista only through that mask, never through the telescope or frame. Gas worlds show cloud rather than ground; ocean worlds show water. The signal trace changes by outcome. UI animation does not advance outside time.

The receiver's quiet 240ms three-tone event follows the existing SFX volume path. Object inspection and completion use existing scan feedback. No ambient loop has been added to compete with radio. Scene/crew assets remain compiled into EBOOT; no extra assets need copying.

## Persistence
V26 uses existing surface word bits 20-21 for outcome (0 none/legacy, 1 beacon, 2 trace), 22-24 for the three clues. No payload length increase. Loader retains the older mask for earlier versions, and validates canonical site existence, legal outcome, completion and prerequisite clues for V26. CRC and backup commit remain unchanged. The base reward path remains surface_interact; observatory_resolve checks context and prevents repeats before calling it.

Old complete sites are deliberately not assigned a retrospective choice. Their Codex text explains the missing historical detail. Partially investigated new sites retain clue bits after docking, saving and loading; there is no in-scene save button or new autosave.

## Verification
Passed PSP compile and static scene/catalogue/observatory scripts. Read-only binary size inspection: approximately 3.58MB text, 4.9KB data, 1.21MB BSS (not a live memory/performance measurement). Window mask checked for bounds; native art preview inspected.

Added compiled C regressions for prerequisite gating, invalid choice, exact reward amounts, repeat/branch protection, partial/full save round-trip, V25 migration, corrupt flags, UI confirmation/cancel/re-entry and all-world uniqueness. These fixtures have NOT run. Emulator remains closed; no physical PSP tests or actual gameplay screenshots are claimed.

Before calling the milestone fully verified: execute the regression suite, play both branches, check native screen readability, hear the sound with radio on/off, test hold/release across entry/exit and save/load, then measure PSP memory/FPS. Further artistic polish can follow actual native-screen feedback.

## Related files
src/observatory-state.h; src/observatory-scene.h; src/observatory-tests.h; src/observatory-input-tests.h; tools/check-observatory.mjs.

