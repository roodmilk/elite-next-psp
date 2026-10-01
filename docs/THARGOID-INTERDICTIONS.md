# Thargoid hyperspace interdictions

Version 2.5.204 adds a rare arcade interruption to normal hyperspace travel.

## Flow

Each jump performs one 14% check after the ship has entered the hyperspace corridor. A successful roll freezes the ordinary jump timer and opens a self-contained three-wave rail shooter. Wave sizes are three, four and five ships. Raiders use three crossing/oscillating patterns, move closer with depth scaling, fire aimed energy bolts and make damaging attack passes if not destroyed.

The nub or D-pad moves the sight. Holding Cross repeatedly fires the encounter cannon. Every destroyed Thargoid immediately awards 20 units and increments the normal kill count. The top panel shows wave, kills and accumulated reward; the lower panel shows encounter HP and the original destination.

Clearing wave three resumes the original jump at its braking phase, after which the existing arrival path spends the normal fuel and enters the selected system. Reaching zero encounter HP cancels the jump, returns to the original system, preserves the selected destination and does not spend hyperspace fuel, so the player can try the route again.

## Safety boundary

The encounter is transient: it is not written into commander saves and cannot be active while docked. While active it owns input and rendering, so flight weapons, menus, docking, targeting, police and ordinary jump countdown code cannot run underneath it. A failed encounter suppresses the normal successful-arrival fade.

## Verification

`tools/check-thargoid-ambush.mjs` verifies the trigger, three-wave completion, payment, failure restoration and exclusive main-loop wiring. Compiled input regressions check a paid kill, safe failure and successful jump continuation. The PSP build succeeds. The emulator remained closed, so difficulty, native-screen visual clarity, audio mix and physical PSP performance still need playtesting.
