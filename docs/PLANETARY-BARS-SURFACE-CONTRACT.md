# The Silent Mast — proposed surface contract

Design/source contract only, 23 September 2026. No runtime implementation.
Station owns the bars vertical slice; Planetary owns traversal/site execution;
Gameplay is the single mission/event/save authority; Story chooses location/text.
Depends on integrated traversal 6341b51803ee92dccf560e5d1153ff618503cd0b,
landing-kit gate and agreed mission persistence. Current verified main ac8a581.

## One short, reliable excursion

Propose one recorder mast on the chosen nearby solid world, at a fixed offset
110 m north of surface_site(g,1), ground height from terrain_height. Destination
proposal is Reorte39/body1, verified below and awaiting Story clearance. On current traversal this lies inside the flat
200 m landing region, the 140 m exposure-safe radius, the dry ocean island and
the 480 m field. These are source-derived expectations, not movement proof.
The destination must be revalidated after PlanetProfile or landing changes.

The mast is visible and repeatable for the mission; the recorder's recoverable
state comes only from Gameplay. One explicit grounded close-range interaction
(proposed X within 20 m planar, standing-height tolerance matching boarding)
reports recovery. Square keeps ordinary surveying; Circle keeps boarding;
Triangle keeps facing the ship. No direct surface credit/cargo reward, no life
pool conversion and no second mission flag. Full mission/cargo semantics and
save/reload/abandon/reaccept rules require Gameplay's contract before coding.

## Source reality and bounded implementation

There is currently no interactive recorder/mast. surface_site indices 0 and 2
use seeded positions that can lie outside the local field, so they must not be
assumed reachable. planet.h currently has a fixed PlanetProp[96] decoration
list, pad geometry and a decorative hut. Proposed representation: reserve one
entry in that existing 96-entry list for a mast marker on the mission world,
with a bounded simple renderer; reduce decorative capacity by one if necessary.
Do not grow the list, add a broad POI registry or consume a mineral/life slot.
The exact position helper and mission-state query names remain unassigned until
Gameplay and Systems agree their interface. Do not change global site_xz rules.

Potential later hunks: game.c pure site-position/eligibility helper; game.h only
agreed declarations; planet.h one marker plus contextual instruction; main.c
one X branch inside dedicated EVA dispatch calling the mission-owned action.
Gameplay owns validation/event commit and persistent recovered state; Planetary
must not duplicate it. Audio owns any successful recovery sound after agreement.
UI owns wording fit review; existing return/exposure bands keep their priority.

## Required combined acceptance

- Correct destination remains stable across menu visits, entry and save/load;
  wrong world/body and inactive job never recover the recorder.
- Real held-input walk from actual disembark position to the marker, close-range
  interaction, Triangle return, grounded board, takeoff/orbit and recipient
  return. No target teleport may substitute for walkability proof.
- Repeat/held X yields one recovery event, no additional payout; airborne/far
  interaction rejects. Square surveying and all existing EVA controls remain.
- Recovery survives supported save/recovery; abandon/reaccept and full-log/full-
  cargo outcomes follow Gameplay's explicit policy. Surface never pays credits.
- Pre-kit atmosphere escape remains possible; post-kit and completed/legacy
  paths reach the site. Debug fixture grants only agreed test capability.
- All five smoke groups, V13/migration compatibility as applicable, 480x272
  native marker/interaction/return capture, and matched performance. Combined
  current-main validation by Lead is required before compatibility claims.

Open decisions: Story clearance/recipient, final mission interface and state storage,
interaction policy, abandon/reaccept/save semantics, exact shared hunks. No
implementation until traversal integration and owners settle these decisions.
Minimum-mineral availability is a separate checkpoint and does not supply this
mission object or event state.

## Verified destination proposal and agreed adapter

Selected for Station/Story confirmation: REORTE I, system39/body1, ROCKY,
seed2152466608, mission-scoped site1. Runtime-generated pad eye location
(703.68,46.00,-853.27); marker ground position (703.68,24.00,-743.27).
An isolated PSP-target diagnostic on traversal6341b51 placed the ship on the
pad, invoked normal land/disembark, then advanced game_eva_tick walking toward
the unchanged proposed point: 24 steps of0.05s reached18.83m; 11 return steps
reached valid boarding, with zero exposure and successful takeoff/orbit. Reorte
II also passed, but body1 is the selected single site. This tests movement and
collision from a pad-centre fixture, not player input or an implemented object.
It does not validate a later PlanetProfile or Story landing gate.

An initial diagnostic aggregate failed because a local variable named ok was
shadowed by the existing CHECK macro; individual route records passed. Renamed
the diagnostic variable and reran; no production bug or runtime fix involved.

Gameplay agreed pure helper context: exact mission/system/body/site, alive,
on_foot, pressed_recover, finite planar_distance_sq in[0,400], finite absolute
eye_ground_error<=2 and phase ACCEPTED. Planetary's authoritative adapter alone
computes these from Game and site, including eye_ground_error relative to
terrain_height+22; UI cannot fabricate context. Repeat is a no-op. Persisted
recovery must commit atomically; a save failure leaves the recorder collectible.
No direct surface payout or cargo use. Exact persistence API is still pending.

Landing kit in Story's candidate is a tutorial capability granted at transition
to STORY_WORLD after the power lesson, with stage>=WORLD legacy/free fallback.
It is not a purchasable fitted item. Offer acceptance must use the final canonical
capability query and show a truthful training hint when unavailable. Station
owns this offer behavior; no second flag or invisible kit grant is permitted.

Dedicated EVA dispatch currently owns no X action and returns before ship fire;
a future interaction must be pressed-edge only inside that dispatch, after
existing pause/death/modal gates. Other controls remain as documented.

Final isolated diagnostic: work/smoke-20260923-012803-397, all five groups pass.
Diagnostic EBOOT SHA256 d8d8a47202aaa323ca1bdbe766dada8ff41e83539de83c3ec73594ba5e8ef50b.
To reproduce, copy tools/bar-site-audit.h into src/ in an isolated traversal
checkout and include immediately after planet-eva-tests.h inside game_tests.
Do not add that include to shipping source; it writes bar-site-audit.txt.
This contract/tool commit has no production source or binary changes.
