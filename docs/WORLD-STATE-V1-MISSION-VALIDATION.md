# World State v1 + Mission Validation v1

The live `Game` remains the source of truth. `src/world-state.h` exposes a canonical read-only snapshot for a system: economy, government, technology, danger, station count, planet count, faction traffic counts and active local events. Reputation is derived from the existing guild, legal, wanted and combat records, so no screen can invent a conflicting value.

Every mission offer passes `mission_offer_valid` before it is displayed or accepted. Validation requires a real destination within jump range, an available station hub, and a landable body for exploration work. Accepted exploration missions use the same deterministic body identity used by flight and the Discovery Codex. Invalid offers are omitted rather than directing the commander to an empty or impossible destination.

## Lave certification slice

System 7 is the release gate: peaceful danger rating, reconciled Trader/Law/Pirate/Explorer traffic, station hub and bar access, market, outfitting, decorator, repairs, targeting, local police and trader behaviour, planet approach and landing, Codex records, GalacticNet, and save/load. Automated game checks cover mission validity, landable identity, faction totals, reputation progression and the Lave snapshot. Galaxy expansion waits until this slice remains green.
