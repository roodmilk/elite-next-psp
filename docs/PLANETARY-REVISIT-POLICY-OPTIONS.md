# Planetary revisit rewards — read-only policy options

Decision paper, not an implementation. Based on released main1e3fab88090e328916dc487f1c99031daf0a9061
and the minimum-mineral checkpoint. No save/economy/ledger change is included.

## Current behavior

enter_planet regenerates eight alive, unscanned objects. survey_scan pays120
internal units (12.0 displayed U) for each newly scanned object in that visit.
Minerals also give one tonne of minerals/alloys, or40 internal extra when the
hold is full. The same object cannot pay twice in one visit, but leaving and
re-entering resets eligibility. Docked V13 saves preserve aggregate discoveries,
credits and cargo; they do not identify rewarded world/object pairs. Therefore
repeat entry can farm both survey cash and minerals. The minimum guarantee does
not fix this and must not be described as durable discovery tracking.

## Options for a later agreed checkpoint

| Option | Player rule | Persistence/economy impact |
|---|---|---|
| Keep repeat expeditions | Each fresh visit can survey again; describe cash as repeat sample work, not first discovery | No schema change; farming remains and current discovery totals count repeat events |
| One world bonus | First visit gets a one-time bonus; later resource work follows a separately agreed rule | A256x4 world bitset is128bytes; changes reward meaning and needs explicit migration/resource policy |
| One award per object | Each stable system/body/object pays its survey/material award once per commander | A256x4x8 eligibility bitset is1024bytes before metadata; preserves existing per-object amounts while changing recurrence |

Recommended direction for joint Gameplay/Systems design: per-object eligibility,
with rescanning still allowed for information and a truthful already-recorded
message. Stable identity must be world seed/generation version plus body/slot;
future density or object remapping cannot silently turn an old slot into a new
free reward or suppress a legitimate new object. A separate renewable-resource
mechanic would need explicit replenishment rules; do not add a hidden timer now.

The byte estimates are logical payloads, not permission to add Game fields or
change V13. Systems must choose versioned/checksummed storage and recovery policy,
Gameplay remains reward authority, and Lead must approve combined migration.
Old aggregate totals cannot reconstruct exact past sites. State that limitation
and choose a fair forward-only eligibility rule rather than guessing history.

## Decision and validation needed

Agree survey-vs-resource repeat policy, old-save treatment, generation-version
identity and when rewards become durable. Credit/cargo/eligibility must commit
atomically; a failed save cannot consume the reward or duplicate it on reload.
Test same visit, orbit re-entry, another world, dock/save/load, old saves,
corrupt primary/backup recovery, full hold, refinery, death and failed commits.
No claim of rollback-cheat immunity follows from ordinary save/load tests.

This checkpoint implements none of these policies. The bar recorder uses its
own one-shot mission evidence contract and must not share a survey ledger or
mineral fallback flag.
