# 2.5.176 — Circle weapons/tools pad and Triangle interactions

## Controls
- Hold Circle in ordinary spaceflight for a D-pad-shaped tools panel. Tap Circle only previews status; it never fires by itself.
- While held: Up missile at selected hostile; Down rear decoy; Left ECM; Right heat sink.
- Each new direction press acts once; held/repeat and ambiguous simultaneous directions cannot repeatedly discharge. Tool chords do not steer, throttle, roll, boost or fire the primary laser. World simulation and incoming threats continue.
- Existing primary X laser and legacy L+X missile shortcut remain.
- Triangle tap interacts with the selected contact: hail NPC, tractor cargo, scan anomaly, request docking or request planetary landing clearance. Existing messages/reply choices have priority. Holding Triangle still opens Comms.
- Planet clearance requires X confirmation; Circle cancels. Landing/departure remain input-locked with automatic disembark. Ground controls and menu Cancel are not remapped.

## Balance and integration
- Missile ammunition/range/hostile validation unchanged.
- Standard rechargeable decoy bank: 3 charges, one restored every20s,3s launcher cooldown. Chaff dispenser in UTIL improves capacity to6/recharge10s. Docked launch replenishes bank.
- Rear decoy is a world-space flash lasting1.8s. It diverts an incoming missile with0.25–1.6s remaining. Too-early or too-late deployment is not protection. It does not damage ships.
- ECM requires DEF item16, spends18 shield energy and has18s cooldown. Breaks current incoming missile lock. No automatic activation.
- Heat sink requires the existing module, removes40 heat,30s cooldown. No automatic overheat trigger.
- Tool timers/decoy cells are transient rechargeable state (not save-format additions). Existing fitted modules persist through the normal save system.
- Outfitting stats/descriptions, tutorial, controls reference, campaign/Guild/mission instructions updated to actual controls.
- Mapping/name table and small dispatch function in flight-tools-ui.h allow future reassignment without rewriting the HUD.

## Verification
New model and input tests cover missing equipment, resource costs, cooldowns, recharge, rear deployment, early/timely flare behaviour, all four chords, repeated/ambiguous keys, no steering leakage and Triangle landing confirmation. Existing tutorial, rescue, cargo, docking and landing tests updated for intentional controls, not weakened.
Earlier runs exposed stale Circle assertions/prompts; corrected to Triangle. Final suite results and binary hashes recorded in handoff.
Manual PSP playtesting remains required; this update does not claim to resolve the previously reported planetary hardware shutdown.
