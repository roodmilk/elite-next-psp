# 2.5.391 — Arcade race boost and readable rivals

Built on the local 2.5.390 Portrait Broadcast release and its 2.5.389 retail
PSP launch/memory fixes. Existing saves, conversations, music and unrelated
work are retained.

## Player rules

- Single R accelerates to race cruise; double-tap R and hold spends the race
  reserve. L brakes. Releasing R coasts and permits regeneration.
- A full reserve supplies four seconds of manual boost. After a 0.55-second
  recovery delay, it regenerates ten percentage points per second.
- Exhaustion cuts manual boost and displays RELEASE R. Holding R cannot
  automatically regenerate/restart boost. Release, then double-tap again.
- Gold rings add 24 percentage points, capped at 100, and provide a 0.65-second
  drive surge. Each player's ring can be used only once per attempt.
- Race cruise is 70% of the frozen performance class's boost speed. Burst
  speed is capped at that class's maximum; stacked speed modules cannot
  multiply the race speed cap a second time.
- Race reserve replaces boost fuel expenditure during the attempt. Ordinary
  flight keeps its existing boost, heat, fuel and upgrade behavior.
- The native 480x272 HUD displays reserve percentage, bar, recharge/ready
  state, and exhaustion prompt. Countdown explains gold rings and L braking.

## Course and opponents

The seeded thirty-two-gate course scales its distance to fitted speed, engine
pips and boost modifiers when the grid is created, targeting roughly two
minutes. Retry/grid preparation refreshes the class after relevant changes.
Its existing climbing S-bends and four spectacle sectors remain; the final
three gates of each sector use tighter 1,800 m apertures instead of 2,500 m.
Missing a gate still requires returning through its real directional aperture.

Rivals use actual physical gate crossings, distinct liveries and burst exhaust.
Their meshes are 50% larger than the previous race rendering (scale 0.9 to
1.35); collision/culling bounds follow the enlarged mesh. No extra textures,
unbounded particles or geometry buffers are allocated.

Opponents have the same four-second reserve, recovery and gold-ring refill.
They boost on aligned straights, recover between bursts, slow for difficult
turns and have turn/acceleration limits based on the player's performance
class. There is no position-based catch-up multiplier or teleporting. Contact,
destruction cinematics, safety autopilot and first-place-only prizes remain.

Pause freezes the simulation and reserve. Retry restores reserves and racers;
save/load restarts the accepted race at its grid using the existing save format.
The targeting computer remains unavailable during racing. Sol Rock playback
and restoration of the player's saved radio preference are unchanged.

## Verification

Final reports and native screenshots are archived with the release. Physical
PSP playtest remains necessary; emulator timing is not a hardware FPS claim.
The previously recorded broad-suite failures remain separately documented.

- 84 race assertions pass in the final optimized review build, including real
  shoulder input/exhaustion, pause, retry, directional gates, prizes, save/load,
  collisions, all three camera returns, framebuffer bounds and actual 48 kHz
  MP3 playback from Sol Rock.
- Scripted managed-boost runs physically finish with all 22 hulls in
  116.28–121.86 seconds. The standard ship finishes in 117.34 seconds.
- Continually held boost finishes third in 133.77 seconds; the fastest hull
  with speed/boost upgrades, engine pips and Advanced Heat Buffer also finishes
  third, in 132.46 seconds. These are controlled flight tests, not a promise of
  every human result or course difficulty.
- 37 mission reward, 37 intro-film and 345 opening-story checks pass. These
  regression runs precede the final two countdown hint-coordinate adjustments;
  the final race capture/test run includes those adjustments.
- The exact final retail EBOOT passes 15 PSP-2000 emulator startup/render/audio
  checks across intro, command deck and flight. Retail excludes embedded QA;
  its memory image is 11,698,840 bytes and the build reserve is 5,078,376 bytes.
- The broad core suite reports 664 passes and the exact same 15 failures as
  2.5.390. No broad-suite-green claim is made.
- Only six source files differ from the archived 2.5.390 source snapshot.
  The intro film payload, unrelated source, saves and music assets are retained.
