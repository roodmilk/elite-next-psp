# Planetary site scenes — v2.5.200 working milestone

## Implemented

- All nine interactable site slots per planet route X to an overlay instead of instant rewards: seven discoveries, contract facility, port. Rover remains its existing interaction.
- Station-style 340x168 art area, right selection rail, committed paginated reading, live funds, yellow NPC reply selection, controller icons at bottom.
- Inspect, equipment, records, occupant and explicit work action. Main inspection required before completing discovery work. Four dialogue choices include a personal conversation, with no player echo.
- Twelve authored objective descriptions, twelve equipment passages and twelve personal NPC passages. This is NOT the planned 36 separate encounter scripts.
- Existing surface_interact is the sole reward/contract path. Existing completed sites remain complete; no save version change from V25.
- Frozen outdoor simulation, UI-only animation clock, entry/exit release latches, exact position retained, stale-context rejection, tutorial input wrapper isolation.
- Sixteen native pixel plates: 12 discovery families, port, power relay, biology array, archive uplink. Exact 340x168 source coordinates, 24-colour indexed output, no source resizing. Shared artwork and highlight anchors. Reuses existing crew sprites. Planet colours and local time tint selected palette groups.
- Unstaffed sites show empty chairs. Rescue occupants are present until completion, then gone.

## Art production

tools/build-planet-site-art.mjs is the editable native recipe source. It emits src/generated/planet-site-pixels.h and native PNG previews under assets/generated/planet-sites. The contact sheet contains sixteen 340x168 cells in catalogue order. Plates use 913,920 indexed bytes plus the palette/anchors. They are compiled read-only assets, with no PNG parsing during play.

The earlier observatory-concept.png is a mood reference only, not loaded or resized into the game. Current native art is a first functional kit, not final richly detailed art. Background/secondary furniture repetition still needs refinement. True weather, terrain silhouette and narrative-state-specific imagery are unfinished.

## Verification

Static scene checks and art-generation bounds/palette checks pass. Native contact sheet and observatory preview inspected. PSP build passes with existing warnings. Controller regression fixtures cover entry latch, frozen movement/time, committed text, repeated reward, personal reply, exit latch and scene resolution over all 1024 planets; compiled, NOT executed. No emulator launched, no physical PSP testing and no runtime UI screenshot claimed.

## Remaining milestone work

- Refine native art quality and distinct secondary equipment; add condition variants and richer per-biome backgrounds.
- Add three genuinely distinct encounters per discovery family (36), named persistent occupants and context-sensitive choices.
- Add persistent branch consequences after choosing a bounded save extension and migration strategy. Present inspection/conversation reading position is transient; objective completion persists.
- Connect rare opportunities to existing missions/trade/Codex, without invented rewards or promises.
- Add site-specific audio and ambient animations; current pass uses existing UI feedback only.
- Runtime regression suite, save/load playthrough, representative screenshots, hardware memory/FPS and controller testing (emulator remains closed per user preference).
- Do not describe this working milestone as the entire finished planetary encounter feature.

## Controls

Approach on foot, X opens; release controls once. Up/down selects, X inspects or replies. Left/right pages committed prose. Circle leaves dialogue or returns outside. Opening a site does not auto-pay, auto-board or move the player.

