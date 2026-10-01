# Faction almanac — 2.5.179

The WORK / FACTIONS screen is now a readable native 480x272 dossier browser.
Four faction-coloured emblems, existing uniform portraits and a persistent
selector replace the crowded ship-count list. Identity, In Play and Channel
pages provide twelve authored paragraphs plus crew sayings, gameplay pointers
and existing story-aware channel notes. No invented reputation/joining system.

Up/Down selects a faction and returns to Identity. X cycles the three pages.
Triangle retains the existing nearest living contact selection; while flying,
it returns to flight and aligns. Docked use never silently launches the ship.
Circle returns to the owning menu. Page changes no longer generate chatter.

LOCAL SNAPSHOT counts alive NPCs in the currently loaded system by role. It is
not global faction population, reputation or strength; the menu is not claiming
ships continue to simulate while it is open. Counts refresh from current state.

Tests cover all twelve native paragraph/note layouts, page cycling and faction
reset, and the count changing when a contact dies. Captures cover every page.
No extra assets, save changes or changes to faction combat/economy behaviour.

Verified ../../work/smoke-20260927-141818-299: all five groups RESULT 0 failures;55.98 FPS average,worst133.47ms,22 frames>25ms,planetary scenes >=24 FPS. All twelve paragraphs/notes fit; page cycling, identity reset and living-contact counts pass. Native dossier captures inspected, including corrected snapshot/footer spacing. SHA256 FE3ECF5D1A792863E682834F2D127A1B8343C08875DA71C7CBDF16817FB973AB. No extra assets/save changes; no commit/push/release. Highest remaining priority: physical PSP review and the earlier unresolved planetary shutdown investigation. Preserve prior local changes; no publishing in this turn.
Previous physical PSP landing-shutdown verification remains outstanding.
