# System Operations overview

Version 2.5.204 replaces Fly > System Details with a gameplay-facing system dashboard sized for the native 480x272 PSP display.

## What the page shows

The left map always contains the local hub, star and four generated worlds. Up/Down selects one; Cross locks it and Triangle aligns to it and returns to flight when already launched.

The station dossier shows its generated exterior family, local economy and technology level, current Trader/Law/Pirate/Explorer counts, missions terminating in the system, remaining Wanted targets, local warrant/fine and the persistent missing-manifest activity state.

The star dossier shows range, the existing close-solar heat/hull hazard and the number of local stellar rifts already logged. Planet dossiers show range and the implemented access mode, including floating skyports on gas giants. Before first landing, surface records remain explicitly locked. After landing, the page reads the world's existing deterministic local clock, settlement culture and sky profile, plus its eight life records and eight persistent field sites.

## Progress and recommendations

System progress is 78 explicit saved objectives: four first landings, 32 life records, 32 field sites, four rifts, five Wanted targets and the station manifest activity. It is a completion summary, not an invisible reputation score.

The NEXT strip uses existing state in this order: active local mission, local warrant, active station investigation, unfinished bounty board, unvisited world, incomplete field record, unlogged rift, available station lead, complete system.

No live weather, conflict, shortage, event or changing-condition system has been invented. Current traffic is labelled as traffic; visited-world sky data comes from the planetary renderer's existing stable profile.

## Verification

`tools/check-system-operations.mjs` verifies the real-state wiring, six-target navigation and absence of fictional live-condition labels. Input-suite cases cover combined saved-progress counting and NEXT recommendation ordering. The PSP build succeeds. The emulator was kept closed, so native-screen readability, input feel and physical PSP performance remain to be checked.
