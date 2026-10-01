# Expanded equipment — v2.5.210

Added Law Scanner (UTIL, item 56): 480 U, displayed tech 3+, all economies with
trade rating 1+. Circle + R shoulder selects; tap Circle to scan. 20 km range,
3-second recharge, 12-second radar snapshots of actual patrol inspection reach.
This increases the installable catalogue from 54 to 55 modules. See TRAFFIC-ROUTES.md.

30 new modules; 54 installable modules total. Buy at Outfitting; use Ship Loadout to choose a slot or arm a fitted primary weapon with Square. X fires the active primary in flight; these do not change Circle's secondary tool selection. Fuel remains a Mechanics service.

## Rules

One of each item per ship. Hull category capacities still apply. Firepower Amp and Overdrive add +30% together before WEP pips. Overdrive and Servo add their heat penalties, then Cold Coil multiplies by 0.8. Range Optics affects mining range too. Shield, scanner, scoop and repair tiers use the best rate; Recharge Relay and Radiator Fins add to that baseline. Cargo expansions stack. Armour reduces only damage reaching hull; thermal, collision and missile protection reduce their respective incoming damage before shields. No immunity to police scans or solar hazards.

The scatter weapon widens hit tolerance and loses up to half damage with distance; it hits one contact per shot, not multiple physical pellets. All primary weapons are hitscan. Ion drains extra shield, plasma bypasses shields with 25% damage, and Disruptor delays the surviving target's next shot. Standard crime, bounty, rescue and mission handling remains in the common hit path.

## New catalogue

Prices in units; tech is displayed minimum. Economy masks and prosperity also restrict stock, with outer station variations. Every new item has at least one eligible system in the deterministic galaxy.

| Category | Item | Price | Tech | Effect / trade-off |
|---|---|---:|---:|---|
| WPN | RAPID PULSE | 680 | 6 | 14 damage; 0.08s cycle, 5 heat. Short 1800m rapid-fire streams. |
| WPN | PRECISION LANCE | 2400 | 11 | 90 damage; 0.70s cycle, 28 heat. Narrow aim; reaches 4200m. |
| WPN | SCATTER ARRAY | 950 | 7 | Wide shot cone; up to 48 damage, falling with distance. 900m range. |
| WPN | ION PROJECTOR | 1250 | 9 | 12 damage plus up to 25 shield drain. 0.25s cycle, 9 heat. |
| WPN | PLASMA CUTTER | 1850 | 10 | 70 damage; 25% bypasses shields. 0.50s cycle, 26 heat. 1600m. |
| WPN | DISRUPTOR | 1650 | 10 | 22 damage; delays struck ship's next shot by at least 1.2s. 2000m. |
| DEF | REACTIVE ARMOUR | 750 | 6 | Reduces damage reaching your hull by 20%. Shields are unchanged. |
| DEF | RECHARGE RELAY | 880 | 7 | Adds 1 shield energy/s before SYS pips. Works with other shields. |
| DEF | PRISMATIC SHIELD | 2100 | 11 | Shield recovery 6/s before SYS pips. Strongest shield tier wins. |
| DEF | THERMAL LINER | 690 | 6 | Reduces shield and hull damage from solar heat and runaway by 30%. |
| DEF | IMPACT DAMPER | 540 | 5 | Reduces collision damage by 25%. Does not reduce weapons fire. |
| DEF | MISSILE BULKHEAD | 1300 | 9 | Reduces damage from incoming missile impacts by 35%. |
| NAV | DEEP SCANNER | 1150 | 8 | Identifies unknown ships within 7500m. Strongest ID scanner wins. |
| NAV | SURVEY ARRAY | 980 | 7 | Extends planetary life and mineral scans to 760m. |
| NAV | RANGE OPTICS | 1050 | 8 | Extends the active primary weapon's reach by 20%. |
| NAV | TARGETING LENS | 820 | 6 | Widens primary shot tolerance by 20%; no automatic steering. |
| NAV | SALVAGE ANALYSER | 780 | 6 | Adds 20% cash from wreck recovery and automatic full-hold sales. |
| HOLD | COMPRESSED BAY | 1450 | 8 | Adds 24 tonnes. Stacks with different hold modules. |
| HOLD | BULK CONTAINER | 2300 | 10 | Adds 32 tonnes. Stacks with different hold modules. |
| HOLD | UTILITY LOCKER | 120 | 2 | Adds 4 tonnes of general cargo. Does not hide illegal goods. |
| FUEL | INDUSTRIAL SCOOP | 1100 | 8 | Collects 1 fuel/s near the sun. Strongest scoop wins. Watch heat. |
| FUEL | CORONA SCOOP | 1900 | 11 | Collects 1.25 fuel/s near the sun. Strongest scoop wins. |
| FUEL | BOOST ECONOMISER | 640 | 5 | Reduces boost fuel consumption by 20%. Jump fuel is unchanged. |
| FUEL | SOLAR BAFFLE | 850 | 7 | Reduces heat gained near suns by 25%. Does not prevent all damage. |
| UTIL | FIREPOWER AMP | 900 | 7 | Adds 10% primary weapon damage. Works with weapon power pips. |
| UTIL | WEAPON OVERDRIVE | 1750 | 10 | Adds 20% primary damage, but raises weapon heat by 15%. |
| UTIL | CYCLING SERVO | 1120 | 8 | Shortens primary shot intervals by 15%. Weapon heat rises 10%. |
| UTIL | COLD FIRING COIL | 970 | 7 | Reduces heat per primary shot by 20%. Does not cool the sun. |
| UTIL | REPAIR DRONES | 1550 | 9 | Repairs hull at 3.5/s out of combat below 80 heat. Best rate wins. |
| UTIL | RADIATOR FINS | 560 | 5 | Adds 8 heat/s to passive cooling, away from suns and boost. |

## Saves, verification and next steps

V25 reads existing saves but older executables cannot read new saves. Back up all commander files before upgrading. New IDs use duplicate-safe byte-table validation; no extra save payload beyond V24.

Static checks passed for catalogue completeness, description bounds, slot geometry and stock availability across all 256 systems. PSP build checked. Expanded effect and per-item save regressions are compiled, NOT executed. Emulator intentionally closed. Still required: runtime purchase/sale/save migration suite, visual review at 480x272, controller playthrough and balancing on physical PSP. No extra asset downloads required.

