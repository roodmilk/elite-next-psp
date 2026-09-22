# ELITE: NEXT — DEVELOPMENT HANDOFF

Prepared 22 September 2026. Current build: **2.5.12**.

## Start here

This is a native PSP homebrew game inspired by Elite-A and the wider Elite lineage. It is no longer a literal port: it has a new flight/world simulation, 256 seeded systems, modern PSP interface, multi-body systems, factions, missions, planetary flight/EVA, custom radio folders and an original Kei/Ryn campaign called **The Open Channel**.

Read these documents in this order:

1. `CLAUDE-HANDOFF.md` — current implementation state and working rules.
2. `docs/DESIGN-BIBLE-2.0.md` — concise product and technical direction.
3. `docs/OPEN-CHANNEL-CAMPAIGN.md` — the 24-chapter main campaign.
4. `docs/UI-SPEC.md` — PSP-specific layout and interaction rules.
5. `docs/DESIGN-BIBLE.md` — exhaustive historical design record and detailed original scripts.
6. `docs/PROGRESS.md` and `docs/FEATURE-MAP.md` — implementation history and feature inventory.

The newest explicit user feedback overrides older prose in the large design bible.

## Non-negotiable product direction

- Keep the title **ELITE: NEXT** and its existing visual identity.
- Native PSP resolution is 480×272. Every screen, portrait, icon and footer must be verified at that size.
- Preserve the retro 1980s science-fiction tone: restrained neon, low-resolution pixel art, wireframe heritage and readable silhouettes. Avoid bright generic mobile-game or exaggerated anime styling.
- The interface must remain glance-readable. Put immediate flight information at the top and bottom; keep the central canopy clear.
- Blue speech panels belong to named NPCs. Orange right-tailed panels belong to the player.
- A tracked mission must always show one short, truthful next action. Never direct the player to a random contract that may not exist.
- Thargoids are rare and unsettling. Do not turn them into routine disposable traffic.
- All essential objectives need recovery paths. Avoid random availability, missable actors and repeatable rewards.
- Do not promise “zero bugs.” Build, run the complete regression suite and state what was actually tested.

## Current build and controls

The packaged executable is `EBOOT.PBP`. Put the game folder under `PSP/GAME/ELITE-NEXT/`.

Custom MP3s belong beside the EBOOT:

```text
music/
  Deep Field/
  Neon Transit/
  Pixel Comet/
  Velvet Orbit/
  Far Horizons/
```

The folders are rescanned on startup. MP3 files are intentionally excluded from the handoff archives; users provide music with suitable rights.

Important controls:

- Select: command deck.
- Hold Triangle: quick communications.
- Square tap: targeting computer.
- Hold Square + D-pad: browse target groups without turning.
- Hold Square + L: cycle contacts currently in front.
- Hold Square + R: lock highlighted target and engage auto-turn.
- L + Left/Right: roll.
- Double-tap L while fast: hard brake.
- Double-tap and hold R: high boost (blocked when heat is critical).
- Heat: speed / boost / sun raise HEAT; cool when not boosting and clear of the star; max heat destroys the ship.
- Galaxy Map: Triangle switches between nearby jumps and the full 256-system map; D-pad moves between systems; L/R zoom; X plots a multi-jump route and saves the final goal.

## Recent changes that must be preserved

### 2.5.0 — The Open Channel

`src/saga.h` adds a 24-chapter data-driven follow-on to the first-flight prologue. Story state lives in `Game` as `saga_chapter`, `saga_step`, `saga_flags`, `saga_choice`, `saga_dest`, `saga_start` and four trust values. Save format 9 appends ten little-endian values and still imports older saves.

The currently playable chapter actions are deliberately compact: dock, scan, defeat hostile ships, return home or make a choice. The campaign bible contains richer bespoke scenes that still need staged implementation. Do not describe all proposed set-pieces as already implemented.

### 2.5.1 — custom MP3 quality

`src/audio.h` uses the PSP hardware MP3 decoder. It now has a 64 KB compressed stream buffer, 2,048-frame stereo output blocks and bounded Catmull-Rom resampling for 32/44.1/48 kHz sources. Suspend/resume tears down and safely recreates decoder resources. Preserve the larger buffers; the earlier 16 KB/256-frame design produced intermittent crackle on hardware.

### 2.5.2 — galaxy navigation

The nearby list only shows local candidates. Triangle opens a spatial overview of all 256 systems. It marks the current system in cyan, the tracked mission destination in gold, and caches/draws every intermediate jump. L/R zoom from 1× to 4×. X converts the selected long route into its first reachable jump. This logic is generic; **Quator has no special code or significance** and was only the system that exposed the old UI flaw.

Story navigation plans against the fitted drive even if the tank is empty, then marks the next hop as low-fuel until the player refuels. The story screen distinguishes `NEXT` from `FINAL`, and the cockpit names the next reachable hop.

### 2.5.12 — local wanted under system name

Cockpit header shows `Wanted n/5` on the line under `System:` (red when police may pursue, dim at 0/5).

### 2.5.11 — mission cue top-right

Tracked HUD cue is flush-right on the top header (1-col inset), kept clear of the danger badge, with a longer buffer so the short guide stays readable.

### 2.5.10 — Start power on cockpit meters

Hold Start: Left/Right pick SYS/ENG/WEP on the existing meters; Up/Down move pips. No separate overlay panel.

### 2.5.9 — hard brake / heat polish, target bars, freighter fight-back

Double-tap L hard brake and heat rules keep their regressions. Target panel shows HULL/SHLD bars. Damaged freighters return fire and destroying non-pirates raises extra local warrant. Engine plumes attach at mesh aft. Station walk is a FLY deck door with talkable NPCs; planet surfaces pull tint from world colours.

### 2.5.8 — per-system skies and far warp-in

Orbital templates, world-type permutations and seeded traffic make each system look different. Hyperspace drops you farther from the hub.

### 2.5.7 — radio chassis and audible static

Radio UI is a dial chassis (OFF + 1–5). Retune injects audible static; OFF is silence. SHIPS lists all contacts; ENEMIES stays hostiles-only.

### 2.5.6 — red alert and enemies band

Combat cues use a bottom-of-canopy RED ALERT strip. Square+D-pad scanner bands include ENEMIES (hostiles only). Galacticnet Spacebook/Messages and Codex Systems/Planets remain as shipped in 2.5.5.

### 2.5.5 — speaker chips, menus, radio, system variety

Dialogue name plates use `speaker_name_tag` so `NAME SAYS` sits on a faction-coloured chip. Tracked Mission draws speaker portraits. Outfitting explains tech gates in plain language. Galacticnet order is Spacebook then Messages with a parody logo. Codex lists Systems and Planets discovered by visiting. Radio is a tuner with OFF, static on retune, and Triangle power. Planet types/traffic layout vary per system; hyperspace arrival is farther from the hub.

### 2.5.4 — sleep/resume recovery

Long PSP sleep could leave the MP3 decoder blocked on Memory Stick I/O and the LCD framebuffer unrestored, producing a permanent black screen on wake. Suspend now freezes MP3 sampling from the power callback; resume rebuilds display mode, both framebuffers, controls, clock and audio. Frame presentation uses `NEXTFRAME` after vblank. Emulator smoke covers a double recover path; confirm on physical hardware after multi-hour sleep.

### 2.5.3 — manual route persistence

`Game.route_goal` stores the final destination of a manually plotted multi-jump route separately from `destination` (the immediate hop). Save format 10 appends that goal and still imports V1–V9 commanders. After hyperspace, `route_refresh_destination` advances the next hop toward the saved goal, or clears the goal on arrival. The galaxy overview labels a saved manual goal in amber when no story destination is tracked. Contract “next step” navigation also sets `route_goal`.

## Architecture

The project is intentionally small and header-heavy.

- `src/game.c`, `src/game.h`: core world state, galaxy, spawning, physics, AI, save/load and tests.
- `src/main.c`: PSP startup, input routing, rendering loop, screen dispatch and input regressions.
- `src/ships.c`, `src/ships.h`, `src/mesh.h`: ship definitions and geometry.
- `src/saga.h`: long campaign data and runtime.
- `src/campaign*.h`: first-flight prologue and tracked-story UI.
- `src/story.h`: optional new-player coach; separate from the authored campaign.
- `src/guild.h`, `src/journey.h`, `src/sectors.h`: Guild assignments, route reliability and repeatable contracts.
- `src/ui-modern.h`, `src/voyage.h`, `src/narrative-nav.h`: menus, galaxy map, HUD guidance and shared narrative actions.
- `src/audio.h`, `src/radio-*.h`: hardware MP3 playback, station folders, generated fallback music and audio preferences.
- `src/*tests.h`: tests compiled into the PSP executable and run by smoke mode.
- `assets/`: source and generated visual assets. Runtime art is mostly embedded in compiled headers.
- `elite-a/`: upstream/reference Elite-A material. Retain provenance and do not assume every file is part of the new runtime.
- `tools/`: asset conversion and validation helpers.

`Game` is one monolithic persistent/runtime structure. New persistent fields require a new save version, strict range validation, an old-save migration path and corruption tests. Rewards and story transitions must remain idempotent.

## Building and testing

Windows PowerShell:

```powershell
./build.ps1
./smoke-test.ps1
```

Linux helpers in this cloud environment: `./build.sh` and `./smoke-test.sh` (PSPDEV + PPSSPP SDL).

`build.ps1` expects the PSP toolchain at `../../work/toolchain` unless `-Toolchain` is supplied. `smoke-test.ps1` expects PPSSPP at `../../work/ppsspp/PPSSPPWindows64.exe` unless `-Emulator` is supplied.

The build compiles `game.c`, `ships.c` and `main.c`, links PSP libraries and produces `EBOOT.PBP`. Smoke mode creates a disposable folder and must report zero failures for:

- game checks;
- input checks;
- steering checks;
- radio checks;
- performance checks.

The last verified 2.5.8 run passed every group under PPSSPP (game, input, steering, radio, performance). PPSSPP success does not replace physical PSP testing.

## Highest-priority remaining work

1. **Confirm 2.5.4 sleep/resume on physical PSP.** Put the handheld to sleep mid-flight and mid-radio for several hours, then wake — screen and audio must return. Also re-check 20+ minute MP3 playback across sample rates.
2. **Deepen the 24 chapters further.** Briefings are now three-beat locked conversations; many bible set-pieces still resolve through generic dock/scan/hunt actions.
3. **Visually inspect the full galaxy map at 480×272** and the new Select-deck ship preview / radio tuner on hardware.
4. **Physical performance and memory audit** of galaxy routing and the 64 KB MP3 buffer.

## Known limitations and honest status

- The design bible describes a far larger game than the current executable. Interiors, planetary exploration and spacewalks are bounded prototypes rather than Starfield-scale simulations.
- The campaign has 24 playable chapter records and persistent choices, but does not yet contain ten hours of unique bespoke mechanics and dialogue. Travel and ordinary play contribute to its intended duration.
- Only one galaxy seed of 256 classic Elite-style systems is active.
- The full-galaxy chart shows all systems, a cached route and a saved manual route goal, but has not yet had user testing on a physical PSP.
- The audio fix passed PPSSPP with the user's files; intermittent real-hardware behaviour still requires listening tests.
- Manual route persistence is playable in 2.5.3; sleep/resume black-screen recovery is in 2.5.4 but still needs multi-hour hardware confirmation.
- Speaker colour chips and clearer Outfitting tech copy shipped in 2.5.5.
- Chapter deepening and bespoke dialogue pages remain design-ahead of the executable.

## Safe continuation workflow

1. Pull and rebase before starting.
2. Run the unchanged build and smoke test to establish a baseline.
3. Make one coherent feature change.
4. Add a meaningful regression for its state transition or input path.
5. Rebuild without compiler warnings and run every check.
6. Inspect affected screens at native 480×272.
7. Increment `VERSION`, `build.ps1`, `CHANGELOG.md` and `CLAUDE-HANDOFF.md` together, copy `EBOOT.PBP` to `dist/ELITE-NEXT-PSP/`, commit and push.

Do not delete older-save handling, audio resume logic, campaign idempotency checks, mission availability checks or route-planning tests to make a new feature easier.

## Rights and source boundary

The project uses Elite-A/reference material and established Elite concepts. The new campaign, dialogue, UI and most new implementation are original. Do not copy dialogue, art, music or proprietary assets from Elite Dangerous, No Man's Sky, Starfield, novels or fan sites. Use lore facts as background and write original expression. Keep source/provenance notes and review redistribution rights before any public release.
