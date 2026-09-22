# Planetary activity review — checkpoint 1

Read-only shared-code audit on main `ac8a5810037dec52c26bbe962973f7af49764a40`
(v2.5.47). Planetary owns feature/design direction. This branch adds this document
only; no movement, terrain, HUD, reward, persistence or docking implementation.
Traversal/coaching integrates first; activity verification follows on that base.

## Existing complete activity

Approach a solid world → surface flight → land at the pad → Circle exits ship →
Square scans a nearby unscanned mineral → Circle boards near ship → Triangle
takes off → Triangle returns to orbit → dock at a hub → sell mineral cargo.
Active notices can consume Triangle before its next action. Planetary owns the
final control/coaching wording and changes to movement or safe recovery.

The scan itself awards **120 internal credits = 12.0 displayed units**, one
discovery and the relevant category count. A mineral adds 1 tonne of Minerals
(cargo[12]), or Alloys (cargo[9]) with refinery bit 131072. A full hold instead
adds 40 internal credits (4.0 displayed units); total immediate payment is then
16.0 units. Boarding is not a second claim or payout gate. Hub sale earns the
current local price in addition to scan payment. Do not promise a new delivery
bonus, stored samples, permanent discoveries or a crafting system.

Source: `src/game.c:447` survey_scan; `:226` eva_toggle; `:201` leave_planet;
`:221` takeoff_planet; trade and save_game in the same file. Existing cargo and
aggregate counts survive docked V13 saves; save_game rejects undocked saving.

## Guarantees and limitations

| Area | Observed source behavior | Consequence |
|---|---|---|
| Availability | enter_planet generates eight life entries. Ocean worlds have minerals at indices 0 and 5; rocky worlds use hash modulo 3. | A mineral activity is not guaranteed on every solid world. |
| Enumeration | Reproduced unsigned 32-bit sector_hash with Math.imul, system-body permutations, Lave override and all eight life-kind assignments for 256 systems × four worlds. Of 768 solid worlds, 19 contain no mineral. Examples (zero-based system/body): 0/3, 42/4, 48/3, 73/3, 83/2. | Source-algorithm enumeration, not an in-engine acceptance test. Runtime enumeration should confirm before implementing a guarantee. |
| Range | survey_scan selects the nearest alive/unscanned object at strictly less than 380 m, or 560 m with upgrade bit4096. Exactly at the radius is rejected. | Test both sides of the boundary; do not describe it as inclusive. |
| Safe reach | Life spawns roughly 100–316 m from the pad; normal scan range is 380 m. Hazard rises outside 140 m from the pad, not from the ship. | Nearby scans may succeed from the safe area; checkpoint1 is a reliable activity, not a newly balanced expedition challenge. |
| HUD | Current planet_eva_hud scan hint uses 380 m even with the range upgrade. Its ship-near cue uses 70 m; boarding uses planar distance <=60 m. | Report to Planetary; avoid conflicting HUD/control edits here. |
| Repeated press | A successful scan sets life[i].scanned; later scans choose another unscanned entry. | Repeated presses can legitimately collect different nearby objects. Isolate one target when testing idempotency. |
| Full hold | Capacity includes passengers and existing cargo. Mineral scan never adds cargo when full; it pays a flat 4.0-unit conversion, even with refinery. | Avoid promising mineral cargo or later sale if the hold is full. |
| Return | Boarding restores ship position; takeoff/orbit return do not clear cargo/counts/credits. | Reward retention is already supported. Boarding adds no bonus. |
| Re-entry | enter_planet sets every life entry scanned=0 again. | Orbit/re-entry can farm the same deterministic site repeatedly. Explicit known checkpoint1 limitation. |
| Persistence | Save V13 stores cargo/credits, aggregate discovery counts and visited systems; it does not store per-body/per-object scan identity. | Docked reload retains earnings but cannot prevent repeat-world farming. Do not repurpose visited-system bits or campaign flags as an undocumented discovery ledger. |
| Failure | Hazard/death uses existing ship energy/death flow; surface saving is unavailable. | No promise of durable progress before reaching a dock and saving. Spacewalk oxygen is a separate mode, not a planetary EVA resource. |

Evidence: `src/game.c:182–190` entry/generation; `:226–249` boarding, traversal
and hazard; `:447–455` scan/reward; `:633` docked save guard and following save
payload; `src/game.h:20,55` life pool/record; `src/planet.h:274–277` activity hints;
`src/main.c:465–470` surface action routing. These are references to the audited
base, not reservations on Planetary's newer movement work.

## Acceptance rows for the integrated traversal checkpoint

| Fixture/action | Required result |
|---|---|
| Solid-world generated target inventory | Record mineral counts across all worlds; Lave I contains the authored ocean mineral entries. Gas/sun entry stays rejected. |
| One isolated mineral, normal/upgraded range | Just inside 380/560 scans; exactly at/outside does not. No scan while aboard or in surface flight. |
| Scan with one tonne free | +1 discovery/mineral count, +120 credits, +1 Minerals, no other commodity mutation. |
| Same scan with refinery | Same counts/payment, +1 Alloys instead of Minerals. |
| Full hold, with/without refinery/passenger occupancy | No capacity overflow; +160 credits total, correct full-hold explanation, no extra cargo. |
| Repeated press with only that target available | No duplicate count, credit or cargo. With another nearby target, exactly that second target may pay once. |
| Walk back and board, take off, return to orbit | Target/counter/cargo earnings retained; no second payout or accidental cargo loss; existing controls and recovery preserved. |
| Dock and sell one earned tonne | Only that tonne sold; credits increase by actual local price; no fixed-price promise. Market restriction to docked state remains. |
| Docked save/load after scan/sale | V13 round-trip preserves aggregate counts, credit/cargo totals and sale; no migration/schema change. |
| Leave and re-enter same world | Record current renewed eligibility/farming as known behavior, not a permanent-discovery success claim. |
| Native capture | Show target/range hint, scan result/cargo, return cue and hub sale at 480×272, coordinated with Planetary/UI. |

These are proposed acceptance tests, not newly executed runtime evidence. Existing
smoke has a nearby-surface scan check; it does not prove this entire mineral-sale
sequence. Do not reuse the parked secondary-hub branch as the planetary base.

## One smallest follow-up proposal — guaranteed mineral availability

After owner agreement, ensure at least one existing life slot is LIFE_MINERAL on
every landable world. Preserve all current assignments when a mineral already
exists; otherwise convert one deterministic existing slot, keeping its position,
alive/scanned state and the eight-entry pool. No new resources, payout changes,
Game fields or save schema. This uses existing V13-compatible state and makes
the promised scan→cargo→hub-sale activity available at every landing.

Proposed exact hunk: life-generation block in enter_planet (`src/game.c:190`),
owned by Planetary until explicitly handed over. Proposed verification: owner's
planet game-test header, enumerate all systems/solid bodies; assert >=1 mineral,
unchanged placements/count/seeds and unchanged kinds for worlds already having
minerals; then execute normal/refinery/full-hold activity cases. No main.c/HUD
or terrain hunk requested. This is a proposal only, not approved implementation.

Tradeoff: one flora/fauna entry changes on 19 audited worlds; no extra objects or
per-frame work. It does not solve revisit farming or add risk. Durable discovery
identity requires a separate jointly reviewed persistence design; no reward or
persistence implementation belongs to checkpoint1.
