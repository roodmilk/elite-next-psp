# Planetary engine audit and staged plan

Audited 22 September 2026 against `9194c4c` on `origin/main`.
Owner: `specialist/planetary-engine`. Manager owns integration and releases.

## Implemented baseline

The current loop is orbit targeting -> approach choice -> local atmosphere
flight -> pad landing -> first-person EVA -> boarding -> takeoff -> orbit.
`game.c` owns transitions, height/water queries, eight lifeform slots, survey
rewards and collisions. `main.c` routes PSP input. `planet.h` draws the local
surface and EVA HUD; `flight-extras.h` and `voyage.h` draw approach and cockpit
guidance. Save V13 stores commander/discovery totals, not a planetary checkpoint;
saving is dock-only. No seamless planet or unrestricted world is implemented.

Reviewed AGENTS, handoff, README, VERSION/changelog, Design Bible 2.0, UI spec,
art direction/roadmap/kit and cinematic/animation guidance alongside these
code paths, body generation, surface target/radar guidance and existing tests.

## Findings, in priority order

1. **Approach safety (this slice).** Automatic non-sun boundary interception
   sets `approach` but used to continue weapons, heat, freighters and NPC logic
   for the rest of that tick. A later collision or police stop could overwrite
   the safe state. Direct `game_tick` calls also advanced incoming missiles
   while an approach choice was active, unlike the normal input modal.
2. **False gas action (this slice).** A gas-boundary collision advertised X to
   scan/fly, while `enter_planet` silently refused gas entry. The approach
   footer repeated X ENTER. Gas now has an explicit Circle escape only; pressing
   X defensively explains the rejection, without awarding a fictitious scan.
3. **Modal layout (this slice).** Minimal/Scenic moved the panel but its title
   and explanation used fixed full-HUD rows. The cockpit is still called for this view (and suppresses itself in Scenic).
   Use the same clear centre consistently, with a self-contained safety panel.
4. **EVA controls differ from help.** The HUD says nub look/D-pad move, but
   vertical nub drives walking and `planet_tick` forces pitch to zero. The
   flight roll chord still shares the input path. There is no independent
   free-look/strafe contract. Boarding checks horizontal distance only; the
   initial 55/40 offset is already outside the 60-unit boarding radius.
5. **Terrain/collision mismatch.** Heights hash individual coordinates; the
   renderer samples a tile centre and draws flat tiles, while collision samples
   the player's exact coordinates. Walking height can jump under an apparently
   flat tile. Ocean walking is not blocked, and crossing the +/-4200 boundary
   wraps the player away from the ship. The hut/props are non-collidable.
6. **Bounded content remains thin.** The cyan pad is real. The other radar sites
   and distant settlement silhouettes are not authored playable objectives.
   Fauna wander locally; scans select the nearest eligible item in range, not
   necessarily the one under the reticle. Full EVA replaces the cockpit and
   therefore omits its parked-ship radar/compass, leaving distance-only recovery.
7. **Persistence/rewards need joint design.** Life scanned flags reset on entry,
   so another visit can pay again. Only totals persist. Explicit approach
   completes exploration jobs; automatic boundary interception bypasses that
   hook. Neither behaviour is changed by this safety slice.

## Staged implementation and acceptance

### 1. Safe approach and return — implemented in this branch

- Stop the tick immediately after a planetary boundary opens a choice; pause
  simulation while the choice is active. Preserve damaging sun collision.
- Reject approach/entry during docking and police modes at the simulation API.
- Show only actions the body supports; preserve palette, 480x272 text, and
  the current surface appearance. No new runtime pools or persistent fields.
- Acceptance: every non-sun body stops outside its radius without damage,
  boundary frames cannot fire or advance outbound missiles, modal incoming
  missiles stay paused, Circle faces outward and flies clear for 60 ticks,
  embedded turn-away stays finite, landable bodies restore orbit position,
  gas X is explained and Circle works, and title/note stay inside the panel
  in Full/Minimal/Scenic (including high contrast). Existing landing/EVA,
  save/migration, campaign and all five smoke groups must remain green.

### 2. Reliable local traversal — proposed next bounded task

- Agree one native PSP look/move/jetpack contract with UI & Art; implement it
  and help together. Exclude spacecraft roll/autoaim from EVA input.
- Use one cheap continuous or shared triangulated terrain surface for both
  rendering and collision. Keep the guaranteed flat pad and fixed draw budget.
- Replace wraparound with an explained local boundary, prevent walking on
  open water, add a visible ship bearing and reachable boarding volume.
- Acceptance: controller tests for look versus movement; terrain continuity
  and rendered/collision sample checks; no edge teleport; water/jetpack/board
  recovery; full round trip on all seeded biome families; native captures;
  measure surface frame rate on emulator and physical PSP.

### 3. One meaningful survey excursion — proposed, jointly owned

- With Gameplay & Story, make one existing local landmark a named scan goal
  with a return cue and explicit completion semantics. Define whether repeated
  visits give discoveries, renewable resources, or no further payment.
- With Systems & QA, budget a durable body/object identity record and migrate
  saves only if required. Keep arrays bounded; do not simulate distant fauna.
- Acceptance: fixed-seed revisit and save/load cannot duplicate a one-off
  reward; missing/blocked targets have recovery; surface and orbit approach
  completion rules agree; old-save fixtures and corruption checks pass.

## Validation and limits

Both initial `b2e6834` and refreshed `9194c4c` baselines built with the shared
Windows PSP toolchain and passed game/input/steering/radio/performance smoke.
The baseline has existing compiler warnings in shared code; this slice does
not claim a warning-free project. Final branch evidence is recorded in the
specialist handoff. No physical PSP, prolonged soak or new surface content is
claimed. This is an unreleased specialist change, not a standalone version.

### Final specialist checkpoint

Validated after updating to `7ef5fb0b3fa97d41323dfc265f5df720d9af416c`.
The native-art validator passes all three scene contracts. PSP build succeeds;
44 existing warnings remain (43 on the audited baseline plus upstream's unused
`native_art_scene` helper). No new warning originates in this slice.
All five normal smoke groups report `RESULT 0 failures`: 297 game and 275 input
checks, plus steering, radio and performance. Average is 57.74 FPS; surface
scenes 36–39 are 59.94, 42.81, 39.96 and 54.49 FPS; worst frame is 33.37 ms
with 15 frames over 25 ms. This passes the executable's performance thresholds,
not the stricter aspirational no-frame-over-25-ms art gate or physical hardware.

Full/Minimal/Scenic solid and gas prompts were reviewed as six native 480x272
captures; Scenic also exercises high contrast. The capture run passed gameplay,
input, steering and radio but failed performance (46.40 average, 23.98 FPS in
EVA) while the existing frame-loop BMP dumps were enabled. Normal smoke above
runs without capture I/O. After the main update, the rebuilt EBOOT is byte-for-
byte identical to that captured/tested executable. The packaged EBOOT SHA256 is
`bea6f57a6884abeb79354840d8292d1fda44d7ae3254a12748b19352765f3a4f`.

Local evidence (relative to specialist task workspace):
- `work/smoke-20260922-235440-577/`: final normal smoke reports.
- `work/smoke-20260922-235010-987/`: native captures and capture-I/O reports.
- `work/planet-final-build.log`: complete compiler output.
- `outputs/planetary-evidence/`: copied reports, six PNGs, audit and handoff.

No merge, tag, release or version bump was made. Integration should preserve
upstream's native scene contract include in main.c and cover field in voyage.h;
this slice touches only tests in main.c and the approach footer in voyage.h.
The Manager should rebuild and run smoke on the integrated tip.
