# Ship module banks — v2.5.194

| Hull | WPN | DEF | NAV | HOLD | FUEL | UTIL | Total |
|---|---:|---:|---:|---:|---:|---:|---:|
| Adder | 1 | 1 | 1 | 1 | 1 | 1 | 6 |
| Gecko | 2 | 1 | 1 | 1 | 1 | 2 | 8 |
| Moray | 2 | 2 | 2 | 1 | 2 | 2 | 11 |
| Cobra Mk 1 | 2 | 2 | 2 | 2 | 2 | 2 | 12 |
| Cobra Mk 3 | 3 | 2 | 3 | 2 | 2 | 3 | 15 |
| Fer-de-Lance | 3 | 3 | 3 | 2 | 2 | 3 | 16 |
| Krait | 4 | 3 | 3 | 3 | 2 | 3 | 18 |
| Python | 3 | 3 | 3 | 4 | 2 | 4 | 19 |
| Ophidian | 4 | 3 | 4 | 4 | 2 | 4 | 21 |
| Anaconda | 4 | 4 | 4 | 4 | 2 | 4 | 22 |

Total capacity rises with price. Freight ships prioritise hold/utility; combat hulls gain more weapon options. The fourth Anaconda defence slot leaves room for future defence catalogue expansion (currently three distinct DEF items). Buying duplicate copies is intentionally disabled.

## Using the board

- Up/Down: category; Left/Right: individual slot. Locked cells are skipped.
- Square: arm selected installed weapon. Available docked or in the paused flight menu. Only one primary weapon fires at a time; it is underlined green. The Circle secondary-tool selector is unchanged.
- Triangle while docked: browse Outfitting for this slot. The quote names the exact destination or replacement.
- X while docked: select sale, then confirm. Occupied passenger cabins and cargo capacity needed by current cargo cannot be removed.
- Ordinary purchases fill the first free category slot. When full, the first slot is offered as a confirmed trade-in; select another slot in Loadout to replace that instead.

Distinct passive modules work together. Cargo additions are summed. Shield/scoop tiers use the strongest fitted rate, not multiplied bonuses. A second weaker shield/scoop is therefore redundant; keep the space for other capabilities. The heavy laser trades slower firing and more heat for larger individual hits. Mining bonuses only apply when the mining laser is armed.

## Data and integration

fit[bank*6+category] preserves the old six primary slots. active_weapon is a slot index. The first bank remains in the legacy save location; V24 adds 18 slots plus active/reserved bytes after the Spacebook extension. Extra storage is bounded (20 saved bytes, 19 Game bytes before alignment). Rebuild combines passive effects and validates physical slot capacity. Save loading rejects duplicates, invalid IDs/categories, locked-slot occupancy and invalid active weapons. Existing CRC/backup workflow remains.

Ship exchanges count each category before charging, refuse overflow, compact slots without losing items, retain the selected weapon and verify cargo fits. No automatic sale of excess modules. Old saves retain their modules in bank one. Save backups are essential: older executables cannot read V24.

## Verification and remaining checks

PSP compilation and tools/check-module-banks.mjs cover table completeness, price progression, description bounds, board geometry and integration wiring. New multislot-tests.h and multislot-input-tests.h compile tests for actual core/input/save paths. They have NOT been run because the emulator remains closed. Runtime regression, V23 migration, controller feel, readability and physical PSP testing remain outstanding. No new external assets.
