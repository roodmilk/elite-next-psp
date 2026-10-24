# ELITE: NEXT flight-audio vertical slice

## Baseline audit — 2.5.416 snapshot

The pre-audio snapshot supports stereo output, MP3 and Ogg radio/music, intro playback, generated presenter audio, station/surface ambience, music ducking, audio shutdown before suspend and audio reconstruction after resume. It has 28 ordinary cue identifiers. Ordinary cues were generated at 44.1 kHz and shared one replaceable playback slot, so a menu sound could interrupt a warning or boost. They were centred and had no world position, voice priority or independent logical buses.

The untouched retail build reports a 13,557,768-byte image, 7,340,032-byte heap, 1,048,576-byte main stack and 3,219,448 bytes of calculated headroom. This is the budget baseline for the slice. The copied legacy smoke harness does not currently pass its own broad frame target: it measured 30.59 FPS and omitted several expected reports, so comparisons must use identical focused review scenes and the known current release checks rather than claim a clean global baseline.

## Milestone A — event mixer foundation

- Eight allocation-free event voices replace the single replaceable effect slot.
- A 16-command main-to-audio queue prevents same-frame submissions from mutating worker-owned voices.
- Critical warnings and threats outrank damage, weapons, major vehicle events, interactions and UI.
- Voice stealing is deterministic and a lower-priority cue cannot evict a higher-priority one.
- Per-voice gain ramps prevent hard starts/stops.
- Four logical event buses exist: player ship, world, warning/combat and UI/comms.
- World-facing playback supports -100..+100 panning. Incoming missile warnings now derive pan from the actual attacking NPC bearing.
- Smooth music ducking responds to any active major or critical voice rather than only the former single slot.
- Worker diagnostics count peak concurrency, steals and dropped commands.

The retail image grew by 4,644 bytes and retains 3,214,804 bytes of calculated headroom. An embedded PSP-runtime review passed eight-way concurrency, warning-over-UI stealing and stereo panning with zero command drops. The review build uses extended emulator memory only because the project's complete embedded QA suite exceeds the retail 32 MiB model; the retail build and its memory check remain on the real limit.

## Milestone B — recorded cockpit and responsive engine

- Three matched 48 kHz/24-bit review journeys use verified CC0 physical fan
  and vehicle recordings. Each follows the same idle, rise, cruise, boost and
  return gesture so the signature can be judged fairly.
- `a-industrial-weight` is the current recommendation. Its low mechanical body
  gives the Cobra-like ship useful mass without occupying the warning band.
- The preferred machinery and engine loops are resident 22.05 kHz/16-bit PCM.
  Playback performs no flight-time storage reads or allocation.
- Engine pitch and load follow real ship speed and boost. Steering adds a
  restrained stereo load shift; low hull increases machinery strain.
- The resident set is 55,124 bytes. The instrumented retail image retains
  3,155,092 bytes before codec and worker overhead, above the 3 MiB gate.

### Boost revision after listening review

The first boost experiment was rejected as grating and generic, removed from
generation and barred from integration. The replacement designs use the
approved engine as their sustained identity. A short CC0 engine-start catch and
metal-body impulse provide physical ignition; an original restrained sub layer
provides scale. There is no sustained air or jet-noise layer. Each review file
contains two triggers in engine context so fatigue is easier to hear.

`a-warm-gravity` was selected in listening review on 2026-10-13: it has the
softest balanced push and leaves warnings and radio clear. `b-deep-displacement` emphasizes hull
weight; `c-mechanical-catch` exposes more of the brief machine response. A
compact 8 kHz/8-bit onset from A is resident while the existing engine supplies
the long acceleration in real time. Its baked attack and release return the
one-shot to zero so stopping and retriggering cannot expose an 8-bit edge.

### Dynamic flight scene

Player-engine level is driven by separate idle, actual speed, positive
acceleration, held thrust and boost contributions. Releasing thrust therefore
settles toward a quieter coast even while the ship retains speed. Every change
is ramped in the audio worker.

The nearest live traffic contact inside 6,000 metres has an independent
spatial voice. Distance controls level, relative bearing controls stereo pan,
and contact cruise speed controls pitch. Capital/freighter traffic uses a lower
pitch and heavier distance curve. This first-pass single-contact budget avoids
turning a busy station into an indistinct engine wall; multi-contact handoff,
planet/station occlusion and passing-speed emphasis remain capture-tuning work.

The next pass adds contact-selection hysteresis, smoothed pan, radial-speed
fly-by emphasis, and occlusion. A body or station between the cockpit and the
contact reduces the level to 30 percent and applies a strong low-pass response.
Steering also raises a quiet recorded-machinery attitude-jet layer on the
opposite cockpit side. It fades to silence at centred controls.

Station structure now rises from 16 km using a low, spatial version of the
recorded machinery bed. Guidance capture and final berth lock trigger a compact
CC0 metal-body clamp transient with zero-valued boundaries and no clipped or
half-scale adjacent steps. The complete build retains 3,146,884 bytes of
calculated PSP headroom and the focused PPSSPP flight run measured 33.97 FPS
with zero mixer command drops.

PPSSPP recorded the game's actual stereo output through idle, acceleration,
steering, boost and deceleration. The 18.54-second, 44.1 kHz capture measured
34.59 FPS, zero command drops and no failures. Its SHA-256 is
`660D6BB84E6BBF8767176C1076E4BC61508F68FD87EDC5B4BD30B7083F927259`.
This is emulator evidence; physical PSP playback remains outstanding.

## First representative journey

Use the player's default Cobra-like medium multipurpose ship: cold cockpit/power-up, undock, station traffic, low throttle, cruise, manoeuvre, boost, incoming threat, hyperspace charge/jump/arrival, station approach, gear/clamps and final dock lock. This exercises the most systems without changing saves or adding a ship-specific persistence format.

## Next implementation order

1. Add validated resident PCM assets and persistent loop voices without storage reads in the audio callback.
2. Feed throttle, acceleration, steering, braking, boost, hull and camera state to independent cockpit/power/engine/strain layers.
3. Produce level-matched recorded-source engine, boost and hyperspace alternatives; keep selected material swappable.
4. Integrate traffic, threat, station approach and docking events, then capture the full journey with radio playing.
5. Measure mixer time, missed buffers, resident memory and worst-case gameplay; repeat suspend/resume and physical-PSP checks before any release EBOOT.

