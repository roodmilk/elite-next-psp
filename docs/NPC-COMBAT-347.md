# NPC combat response - 2.5.347

## Player-visible rules

A real damaging hit gives the victim its existing 90-second attacker memory. Pirates immediately leave any service wait and select the player, including in Lave. Lave's peace rule prevents unprovoked attacks; it does not make wanted raiders defenceless. Recently engaged pirates prioritise the player within 12 km. Beyond that leash they return to local traffic rather than tracking the player across unlimited space.

A healthy raider commits to attack passes, leads moving opponents by a bounded amount and makes small deterministic approach jinks. Inside 400 m it breaks away; it completes that leg until 1,050 m before returning. Existing finite turn-rate steering, obstacle avoidance, staggered decisions and frame-by-frame movement remain active. Actual weapon fire still requires range, bearing and cooldown; no automatic damage merely for selecting the player.

An engaged pirate retreats when hull is below 30% and shields below 15%, or two Law ships are inside 2,200 m. It moves away from actual attacker/patrol positions at 1.65 times cruise. A lone patrol still makes an unengaged pirate evade, but no longer overwrites a healthy pirate's response to every player shot. Retreat never despawns a poster target or pays a reward. Existing player-tagged police assistance still awards the bounty.

Civilian ships retain assault reports and self-defence. Their defensive aim cannot be replaced with an avoidance waypoint while their guns are still targeting the player. Freighter turrets and the opening dream retain their separate existing logic.

## Implementation and verification

src/npc-combat.h owns bounded decision and manoeuvre helpers; src/traffic-routes.h supplies attack/break/escape speed and scanner activity labels. No heap allocation in normal combat, new NPC fields, save migration or Wi-Fi packet size change.

Run tools/review-npc-combat.ps1 for actual PSP simulation coverage: real laser hit in Lave, turnback and return fire, repeated pass, normalized steering, pursuit priority/range, lone and multiple patrols, damaged retreat, unchanged wanted-poster identity/payment, police assist and civilian defence. The suite also runs from game_tests. Full-game baseline has inherited failures; compare the final run rather than claiming full certification. Hardware combat feel still benefits from PSP playtesting.

## Final result

Verification: PSP build succeeds. All 14 focused NPC combat checks pass through real laser-hit and game-tick paths. Core comparison has the identical 15 inherited game failures, no new failures; radio and steering report RESULT 0 failures. Physical PSP combat feel remains to be playtested; full-game certification is not claimed.

Focused evidence: outputs/npc-combat-20261009-211959-537. Baseline/final core reports: ../../work/core-20261009-211501-232 and ../../work/core-20261009-212004-140.
