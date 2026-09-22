# ELITE: NEXT A++ release audit

This is the release gate for the complete design bible. A feature is only
marked complete when the game path exists, the low-resolution presentation is
readable, persistence is safe, and a repeatable check covers the edge cases.

## Audio

- **Radio identity:** five stations have distinct tempo, harmony, stereo space
  and volume controls; Far Horizons adds a gated non-verbal alien chatter bed.
- **Effects:** docking, warp, boost, scan, laser, missile, warning, death and
  communications cues are bounded and duck music without muting it.
- **Source hygiene:** CC0 candidates and their URLs live in
  `assets/audio/CC0-SOURCES.md`; streamed decoder integration remains a named
  milestone rather than being implied by procedural playback.
- **Gate:** radio assertions pass, generated audio stays below clipping, and
  the PSP mixer remains allocation-free in its worker loop.

## Flight and gameplay

- **Navigation:** seeded bodies, quiet/busy traffic budgets, soft planetary
  approach, explicit land/reverse choice, station docking guidance, warp,
  target lock and local warrants all share the same system state.
- **Factions:** Law, Pirates, Traders and Explorers have distinct ships,
  colors, portraits, scanner labels and bounded AI priorities.
- **Missions:** five repeatable job types, five-slot persistence, selected
  offer briefs, route navigation, target marking, timers and failure-forward
  recovery. Authored OC chapters must preserve the same objective/reward/risk
  contract shape.
- **Gate:** every new interaction gets a deterministic game assertion and an
  input assertion; no mission can soft-lock through death, custody, full cargo,
  duplicate interaction or a missing target slot.

## Graphics and interface

- **PSP readability:** 480×272 hierarchy, reserved feedback bands, low-res
  faction portraits, pixel danger stars, target pulse, cockpit telemetry with
  separated pips, and a quiet-comms option.
- **World variety:** deterministic body phase/scale/tilt, seeded planet art,
  colored suns, belts, fauna, stations and distant freighters. Visual RNG is
  isolated from gameplay RNG.
- **Gate:** native-resolution captures for every changed screen, no cropped
  headings, no overlapping telemetry, and performance smoke with no frame over
  25 ms in the current PPSSPP profile.

## Code, saves and delivery

- **Persistence:** commander V8 and radio V2 formats remain backward-readable;
  writes are temp-file, sealed, verified and recoverable from backup.
- **Budget:** static pools stay within the documented PSP memory and entity
  limits; no runtime PNG decoding or unbounded allocations.
- **Release:** build, game, input, steering, radio and performance checks must
  pass before packaging. Physical PSP tests remain a separate gate and are
  never represented as completed by emulator results.

## Remaining named milestones

The following are intentionally visible rather than hidden behind vague
“polish”: streamed CC0 radio decoder, mission abandonment confirmation,
durable per-object Codex entries, later authored OC chapters, safe planet
surface traversal, rover/tractor expansions, larger-text reflow and physical
PSP certification. Each gets its own implementation, test and capture before
being called complete.
