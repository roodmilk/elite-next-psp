# ELITE: NEXT — DEVELOPMENT HANDOFF

## Released 2.5.93 — unified PSP control icons

Added one shared 10x10 pixel renderer for all PSP face buttons, D-pad directions, D-pad cluster, L/R shoulders, Start, Select and the analog nub. Footer prompts now resolve these tokens to aligned, consistently coloured symbols.

## Released 2.5.92 — richer cockpit radio chatter

Expanded the five cockpit radio stations to 24 snippets each, with more adverts and alien talk-show comedy. Added a lightweight varied pause before each ticker line begins.

## Released 2.5.91 — solar-system Discovery Codex

Discovery Codex is now organized as a hierarchy: visited systems → station/star/world records → planet flora and fauna details. System pages also summarize mineral signatures, echoes and local life/traffic records.

## Released 2.5.90 — expanded Galactic Lore archive

Galactic Lore is now a first-class Discover menu entry beside Discovery Codex and GalacticNet. Its archive has twelve longer entries and a full readable detail panel. The station action is labelled `DISEMBARK`.

## Released 2.5.89 — PSP XMB identity artwork

Added `assets/xmb/ICON0.PNG` and `assets/xmb/PIC1.PNG`, and wired them into `pack-pbp` so the PSP XMB shows the Elite: NEXT icon and a space starfield background. Standard PIC1 artwork is static; the in-game starfield remains animated. A PMF/ICON1 animated XMB asset would need a PSP video encoder not included in the current toolchain.

## Released 2.5.88 — clearer radio tuning feedback

The radio page now uses a green Triangle glyph for power, and its station title changes to `STATIC` for the exact short tuning-noise interval between stations.

## Released 2.5.87 — visible hyperspace arrival

The destination system now fades in with a blue braking/arrival wash after hyperspace, revealing the new solar-system ship view smoothly while keeping the cockpit HUD sharp.

## Released 2.5.86 — ambient space encounters

Added a reusable low-cost encounter deck covering traffic, law, pirates, distress calls, cargo/wreckage, smugglers, bounty leads, and unknown contacts. Encounter transmissions use the existing full-screen comms panel: hold Triangle in flight, then choose Respond or Ignore. Responses hook into current trader, police, hostile, cargo, passenger, and credit systems.

## Released 2.5.85 — reliable docking, flight attitude, and warp presentation

Removed the unused cockpit background artifact, hardened Circle station docking and its third-person arrival path, cleared residual roll on boost release, and kept the ship view visible during the initial hyperdrive charge.

## Released 2.5.84 — functional power distribution

WEP/SYS/ENG pip allocation is now regression-tested across weapon output, shield recharge, and engine behavior.

## Released 2.5.83 — clearer mission and wanted HUD cues

Tracked mission instructions now render as green `>> ... >>` route cues. The opening Kei mission remains tracked by default and the tracking path is regression-tested. The cockpit hides the clear-state wanted counter and shows an escalating orange/red/flashing warning only while Law is after the commander.

## Released 2.5.82 — cockpit text radio

Far Horizons is the fresh-launch radio default. Flight view now shows a low-cost scrolling station-talk ticker beside the animated activity strip, with varied generated captions for all five stations. Radio tuning uses PSP L/R, the Music/SFX rows are the only Up/Down choices, and the radio screen has a visible Triangle power button.

## Released 2.5.81 — cleaner cockpit menu footer

The main cockpit menu no longer displays the navigation hint along the bottom edge; other screens retain their own footer guidance.

## Released 2.5.80 — menu wording refresh

Updated the requested main menu labels and subtitles, including `Explore station`, the new launch/upgrade/ship/loadout/control/radio copy, and the undocked `Cargo` label.

## Released 2.5.79 — law warnings and no-funds custody

Wanted commanders now receive a warning before arrest. If station custody is accepted with zero units, the game plays a jail transfer and law seizure sequence, clears credits/cargo/equipment/current ship, and releases the commander in a basic ship at the local station.

## Released 2.5.78 — readable Galactic Lore details

The Galactic Lore codex now places the selected topic and wrapped brief in a full-width bottom panel so longer entries are readable.

## Released 2.5.74 — Trader cargo offers

Trader hails now create a system-local commodity exchange for ordinary traders. The player is told what cargo to buy, and hailing the same named contact again completes the swap. Scanner identification is stored on the live NPC slot so the targeting computer retains the trader's callsign after the player returns from the market. The offer is session-local; commander saves remain V13-compatible.

## Released 2.5.73 — Kei replies directly

Kei's opening briefing no longer repeats the commander's selected speech in a
separate echo panel. Selecting a reply advances directly to Kei's response;
the existing reply choices and acceptance step remain intact.

## Released 2.5.72 — attacking enemies alert

While holding Square, the ENEMIES scan tab flashes red during active player
attack or incoming-missile states, then returns to its normal tab color.

## Released 2.5.71 — clearer held-Square target controls

The held-Square scan banner now shows `L TARGET IN FRONT` and `R LOCK ON`;
the old `D-PAD BANDS`/cycle-view wording is removed.

## Released 2.5.70 — unrestricted flight pitch

Space and atmospheric flight pitch now wraps continuously at ±π instead of
stopping at the old ±1.5/±1.2 radian limits. Holding Up or Down can complete
a full loop in either direction. Landed surface movement retains its normal
look limit. Build and all five smoke suites passed; physical PSP testing is
still unverified.

## Released 2.5.69 — animated cockpit activity display

The main flight cockpit now uses a small native-pixel activity display in the
top-center band. It cycles through stars, radar, telemetry, planet/galaxy and
ship schematic loops, with faster motion at cruise speed and boost streaks.
The existing system, wanted, mission, lower-instrument and combat HUD behavior
remains intact; high danger uses the display border/markers as a compact alert.
Build and smoke evidence for this release should be recorded below.

## Previously released 2.5.68 — development paused

Loadout and Mission Log fixes below are now packaged and smoke-tested in 2.5.68.
This supersedes their pending-release status. See docs/SMOKE-EVIDENCE-2.5.68.md.
User requested stop after delivery to conserve credits. Do not resume goals until asked.
Unintegrated station port and Lave fixture remain in specialist worktrees.

## Previously pending — full Mission Log layout

Main/side missions now form two vertically separated groups. All five contract
rows fit above an independent three-line objective area. Main missions no
longer advertise the inapplicable Abandon action. Selection/tracking indices
and job behavior remain unchanged. Two full-capacity native captures and a
framebuffer assertion cover the formerly hidden fifth contract.

All five smoke suites passed in `work/smoke-20260923-125242-382`.
Development EBOOT SHA256: `53607524418DB88A6AC4348EDC5CC4915212BB90E1B2303162D434B4F5704902`.
Normal full-log capture was visually inspected at 480x272. This supersedes the
earlier pending loadout binary while retaining that fix. No new package/tag:
the combined Lave/station release remains pending. The existing 2.5.67 release
does not contain these pending UI corrections.

## Pending combined release — loadout readability correction

Main now corrects loadout labels that used pixel positions as text columns.
The hull panel shows six short slot labels; the right panel owns module names.
In-flight help says to dock before changing modules. The internal release
number remains 2.5.67 pending the combined Lave/station integration; the existing
2.5.67 ZIP does not contain this pending source change.

Validation: all five smoke suites pass in `work/smoke-20260923-124844-899`.
Development EBOOT SHA256: `51885875FC631491078011628C885C70C5EE32D7CEE62E30EB60AF1EE09565C1`.
Twelve native captures cover every selected slot in both palettes; framebuffer
assertions require visible selected label glyphs. Normal DEF and contrast UTIL
captures were visually reviewed. Average 56.63 FPS, worst 33.37 ms, 23 frames
over 25 ms; this does not satisfy a stricter no-slow-frame gate.

Highest-priority remaining work: integrate the corrected station composer
without replacing newer main behavior; complete the Systems QA continuous
Lave traversal/capture fixture; review native Lave art quality and memory
evidence; then produce one combined, versioned release. v0.7 remains incomplete.

## Integrated 2.5.51 — Station Welcome mission

The current build adds a session-local onboarding mission once the opening campaign
is complete. It plots Lave → Reorte, then changes its objective as the commander
docks, disembarks, walks from Arrivals to **THE SECOND SHIFT**, and talks to
**Lysa Kest**. Existing station-bar interactions are reused; no save fields,
credits, rewards, or bar economy are changed. The cockpit cue stays short at the
PSP's native width (`JUMP: REORTE`, `WALK: CANTEEN`, `TALK: LYSA KEST`).

## Integrated 2.5.50 — native station visual rollout

The station crawl now uses a shared native 480×272 shell and an authored Reorte
primary Arrivals scene in the Second Shift visual language. The existing bar,
hotspots, options rail, speech area and SHIP return remain intact. Arrivals is
the first additional authored room; Market, Shipyard, Mission Board and GalNet
art families remain in rollout and must pass native 1:1 review before adoption.
Story & Dialogue owns station exploration writing and speech-box flow; footer,
help and chrome belong to the named PSP UI & Art managers. There is no separate
"Art3" team member.

The PSP UI & Art Manager is the department lead for visual direction and
production across the whole game. That role coordinates the Procedural Room
Composer and Reusable Element Kit across the thousands of first-person,
point-and-click station screens, along with planetary, ship, GalNet and other
game interfaces. The two specialist groups own their bounded implementations;
the manager owns cohesion, allocation and native 480×272 art acceptance.

## Manager operating contract

The PSP is the primary platform. Every manager designs and tests against native
480×272 first, including memory, framebuffer, input, audio and performance
limits. Larger previews are derivatives only. A feature is not complete from a
concept, plan or compile alone: it needs an integrated build, native normal and
high-contrast evidence, real input/state checks, truthful capability text,
save/economy safety and performance evidence. Generic fallback is unfinished
and must not imply unsupported gameplay. Managers coordinate before shared
edits and keep their ownership boundaries explicit; the Lead integrates and
releases.

## Project-wide native-art acceptance rule

All station artwork and station UI must be authored and judged at the PSP's
native 480×272 resolution. Larger exports are presentation derivatives only;
they must never be the source design that is simply shrunk. Every scene must
retain readable people, selectable hotspots, speech boxes, headings, page
indicators and SHIP/navigation controls at 1:1 pixels before integration.

Composition follows a practical golden-ratio hierarchy adapted to the small
screen: one clear focal area, a stable information/selection column, safe edge
spacing, and deliberate visual weight between art and controls. The ratio is a
guide for placing important elements, never a reason to make text or controls
too small. Native readability and separated hit areas always win.

## Integrated 2.5.49 — The Second Shift playable preview

Reorte's primary hub now has a playable illustrated bar reached through WALK,
ARRIVALS and CANTEEN. Lysa Kest, Pell Sorn and Dax Neral provide short dialogue,
Reorte I navigation help and free session-only High Orbit practice. The activity
does not write credits, cargo, jobs or saves; secondary hubs and other systems
retain their ordinary canteens. The PSP build completes successfully. The local
PPSSPP launcher produced no smoke reports during packaging, so physical PSP and
fresh-emulator interaction remain the immediate verification step.

## Integrated 2.5.48 — planetary EVA traversal

The specialist/planetary-eva checkpoint adds independent look/walk/strafe/jet,
grounded boarding, continuous shared terrain, shore/field bounds and clear ship
return/exposure cues. All five PPSSPP smoke groups pass (324 game / 309 input,
57.18 average FPS). EVA scene38 is 31.55 FPS versus 40.46 baseline: the full
viewport has a measurable cost. No physical PSP test or save-layout change.
See docs/PLANETARY-EVA-HANDOFF.md for evidence, scope and next activity/revisit
gaps. Lead owns integration and release. Story approved the coaching string.

## Integrated 2.5.48 — current-ship preview

specialist/menu-ship-preview branches from ac8a581. See
docs/MENU-SHIP-PREVIEW.md for files, reserved interfaces, native captures,
matched benchmark evidence and agreed Planetary integration checks.
This candidate changes the menu inset only. No save/audio/input/planetary
production changes. Lead must validate the combined build before release;
version, main and release tags remain unchanged.

## Integrated 2.5.48 — event identity and bounded shuffle

Base `ac8a581`, branch `cursor/audio-bounded-shuffle`. Original event sounds move
from 3–32 ms fragments to restrained 20–300 ms cues using the existing single
mono voice. New pure candidate-order policy visits every track once, with the
previous track last. Only the agreed selection and SFX rendering hunks in
`audio.h` are in scope; Systems owns decoding, buffers, I/O and power recovery.
No Game/save, main.c, music gain or station-composition change. See
`docs/AUDIO-AUDIT.md` for source evidence, budget and acceptance matrix.
Lead alone owns combined integration, versioning and release artifacts.
Final candidate PSP build and all-five PPSSPP smoke passed (57.98 FPS average,
33.37 ms worst, 44 existing warnings). Sampled music+SFX peak 11,161 with no
clamps; no physical PSP listening or custom-MP3 certification. Detailed report
and exact test-run identifier are in the audio audit. Combined integration remains
Lead's gate; this isolated candidate is not yet a playable main release.

## Integrated 2.5.47 — safe planetary approach

Approach now stops the remaining collision-frame simulation and pauses threats
in the simulation API. Gas boundaries offer Circle to turn away, with an explicit
explanation if X is pressed. Solid-world X remains surface flight, not instant
landing. The safety panel stays in the same clear centre in all HUD modes.
Docking/police states cannot be replaced by approach/entry calls. No `Game` or
V13 save-layout change, mission reward change, or surface rendering change.

See `docs/PLANETARY-ENGINE-AUDIT.md` for the read-only audit and staged acceptance
plan. The next planetary task is reliable EVA controls/traversal; durable survey
rewards and mission hooks require joint design with Gameplay and Systems.
Validated on main `7ef5fb0`: all five smoke groups pass (57.74 average FPS),
three native-art scene contracts pass, and six 480x272 prompts reviewed.
Existing compiler warnings and capture-I/O timing limits are recorded in the
audit. Release numbering, tags and publishing remain Manager-owned.
## Integrated 2.5.47 — truthful pilot-rescue controls

Generic rescue objectives now read "Lock rescue ship. Within 600 m: Triangle hail."
This matches the existing flight controls: Circle locks a contact, Triangle dismisses
an active notice or hails when clear, and pickup requires the marked ship within
600 m. Story & Dialogue approved the PR-01 intent. Contract and Guild rewards,
deposit, timer, save V13 and Game layout are unchanged.

The existing journey input-test hook covers board acceptance, track/navigation,
Circle locking, notice acknowledgement, 601 m rejection, exact 600 m pickup,
unrelated contact rejection, repeated hail, guided system-hub docking, contract
and Guild payment once, redocking, expiry and retaking the offer. Tests place the
player at the destination and freeze a contact for precise radius checks; they
do not claim manual travel or a physical PSP soak. An optional
`rescue-capture.flag` in the smoke directory writes `rescue-objective.bmp` through
the production 480x272 tracked-mission renderer.

Validation on base `7ef5fb0b3fa97d41323dfc265f5df720d9af416c`: PSP build
and all five smoke groups passed (290 game / 288 input checks, 57.74 average FPS,
33.37 ms worst frame). Compiler warnings remain 44, matching baseline. Native
capture was visually checked: the complete instruction fits on one line without
overlap. Evidence is in the Gameplay & Loops task's `outputs/rescue-evidence`;
normal smoke run is `work/smoke-20260923-001105-281`. Capture I/O was a separate
run and is not the reported performance baseline.

Lead integration combined this slice with safe planetary approach and Chapter 05.
The 2.5.47 PSP build and all five PPSSPP smoke groups passed. Physical PSP testing
remains the next verification step; later gameplay slices must start from this
integrated checkpoint.

## Integrated Gameplay & Story pass — Chapters 02–04

Chapter 02 now requires the authored sealed receiver pickup at Mara's bound port before returning to Lave. Chapter 03 requires the authored signal scan; firing or overheating resets the observation, blocks completion, and requires a clean re-entry. Chapter 04 requires the outbound port stamp, with an optional first-anomaly lifeboat scan that adds one Independent trust. State reuses reserved bits in the existing `saga_flags` word (`0x10`–`0x200`), so no `Game` layout or save-version change is present. Generic scans, kills, cargo, and unrelated docking do not satisfy these objectives.

Manager integration passed the PSP build and all five PPSSPP smoke groups: game, input, steering, radio and performance.

The Chapter 05 slice on `specialist/gameplay-story-ch05` adds the authored early-filing timestamp comparison at Lave, then exposes inspection/protest evidence choices. It reuses `saga_flags` bits `0x400`–`0x2000`, preserves the sealed records, and requires both the sealed receiver and convoy port stamp before comparison can begin. Partial evidence, wrong-system docking, and repeated comparison actions are covered by campaign tests. Legacy Chapter 05 saves are explicitly reopened with both records at Lave rather than being permanently locked.

The planetary story now awards an atmospheric landing kit when the existing coaching reaches **Her World**, after the power lesson. The award uses the existing `story_flags` word (`STORY_EV_LANDING_TECH`), so no `Game` layout or save-version change is present. Atmospheric approach remains available, but `land_planet()` requires the kit; completed/free-story saves and skipped coaching remain compatible. The handoff dialogue names the kit as recovered from Ryn's locker so the first landing reads as a deliberate story milestone.

Prepared 23 September 2026. Current build: **2.5.49**.

## PSP UI/art specialist handoff — native deck readability

The integrated UI/art pass shortens command-deck helper copy to the existing 27-column
detail pane and bounds the renderer at that width. This is presentation-only: no
mission, economy, save, input, or audio state changed. The lead workspace completed
the PSP build and all five smoke-test groups successfully after integration.

## Working tip — equipment modules (shipped in 2.5.46)

Outfitting buys into six hardpoints (`fit[]`); Loadout sells at 50%. Bits still drive the sim; V13 persists slots. Targeting CRT fuzz from 2.5.45 retained.

The integrated Systems/QA follow-up hardens fitted catalog bounds in Loadout and refund paths and adds V13 slot-value regression checks. Save format remains V13; V12 migration remains unchanged. The lead PSP build and all five PPSSPP smoke groups pass after combining this with the deck readability update.

## Start here

This is a native PSP homebrew game inspired by Elite-A and the wider Elite lineage. It is no longer a literal port: it has a new flight/world simulation, 256 seeded systems, modern PSP interface, multi-body systems, factions, missions, planetary flight/EVA, custom radio folders and an original Kei/Ryn campaign called **The Open Channel**.

Read these documents in this order:

1. `CLAUDE-HANDOFF.md` — current implementation state and working rules.
2. `docs/DESIGN-BIBLE-2.0.md` — concise product and technical direction.
3. `docs/ELITE-NEXT-MANUSCRIPT.md` — whole-game story & missions manuscript (Story Manager living bible).
4. `docs/OPEN-CHANNEL-CAMPAIGN.md` — the 24-chapter main campaign (ops summary).
5. `docs/UI-SPEC.md` — PSP-specific layout and interaction rules.
6. `docs/DESIGN-BIBLE.md` — exhaustive historical design record and detailed original scripts.
7. `docs/PROGRESS.md` and `docs/FEATURE-MAP.md` — implementation history and feature inventory.

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

**Download:** every playable build must appear on the GitHub Releases page (latest: https://github.com/roodmilk/elite-next-psp/releases/latest). After smoke is green, tag `v<VERSION>` and push the tag — do not leave the EBOOT only on a branch.

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

### 2.5.39 — MacVenture A++ cinematic polish

Station interiors restaged from `docs/CINEMATIC-MOCKUP-TARGETS.md` + ART DIRECTOR kit grammar: one hero per room, three floor/sky planes, warm ochre/cream staging, side hatch doors (no top door chrome over the focal object), thinner cream/cyan window chrome. Verb/hotspot/SHIP return behaviour unchanged; TAKE→deal / GO hatch snap tightened for fun.

**ART DIRECTOR visual style implementation (shipped in 2.5.40 tip):** all MacVenture rooms draw wall plates/rivets/floor seams; ARRIVALS berth window gets void stars + beacon sparkle; deck/UI panels drop gold corner brackets for charcoal + cream/ochre instrument rules; Wave B/C soft-FB planet bloom, travel FX, denser warp and docking beacon masks folded in. Story/saga manuscripts untouched. Packaged as **2.5.40** combined tip for Systems Release.

**Post-2.5.40 ART animation pass (untagged tip on PR #11):** animated ARRIVALS traffic silhouette + blinking berth board; room practical pulses; fauna icon families + field walk bob; docking warm aperture/corridor motes; near-station traffic glints; quieter MacVenture chrome. Presentation draw only — Gameplay Designer owns 256-system aliveness / spawn. No micro-tag.

**Next visual gap (same PR #11, untagged):** remaining room prop markers (tool rack, steam, loader, mission pin, clinic screen, customs REST); planet skyline blinks + haze + ochre pad apron/corner beacons; charcoal/ochre cockpit, speech, menu notice, minimal HUD, approach/police plates; warmer warp streaks. Still presentation-only.

**Implementer tip under ART DIRECTOR authority (rebased on tip #18 `f003e09`):** 8×8 native prop markers; ARRIVALS cargo-loader cycle; walk/help/comfort chrome; aperture beacons kit cyan. Mission cue: ART_AMBER + 2-col header margin (Designer had restored GOLD for the old flush assert — palette stays with ART DIRECTOR; smoke uses header-margin assert). No tag.

**ART LOOK FREEZE (Commander: one bug-fixed EBOOT):** Look freeze base remains `3f0dc20` (ART_AMBER — do not restore GOLD). Post-v2.5.42 ART tip on PR #11: HUD lock hint off; soft cosmic oceans; no text bloom; GalNet tab neatness; MacVenture options-list disembark (no verb row / pack). No tag until Systems smoke.

**Working tip (Commander: BIG RELEASES ONLY):** **v2.5.42** shipped quiet FX. Next Systems pack waits on ART tip smoke after this look pass.

### 2.5.46 — Equipment modules change ship stats

PR #32 on v2.5.45 tip. Smoke-green Commander pack.

### 2.5.45 — Targeting monitor fuzz

Art tip `178d34a` faint CRT static on targeting computer. Smoke-green Commander pack.

### 2.5.44 — Law rewrite scan/settle/clear

Gameplay Designer PR #29 on v2.5.43 tip. Smoke-green Commander pack.

**ART tip (untagged, PR #11):** targeting computer CRT glass — faint scanlines/static under list glyphs (no text bloom, no meteors). Systems owns smoke/pack.

### 2.5.43 — Play-fix pack (art + Designer + Story)

Art HUD/disembark, Designer dock-menu/cam/Factions, Story gazette lore. Smoke-green Commander download.

**Law rewrite (untagged tip):** hold scan discovers restricted goods (no crime-on-buy); warrant vs scan stops; Status Square / CUSTOMS tip clear desk. Pirate bounty unchanged. No art restyle.

### 2.5.42 — ART quiet FX + declutter disembark

Quiet space FX (`b0a97e1`) on the combined tip. Smoke-green Commander pack. Follow-on untagged ART tip: options-list station deck + HUD/GalNet/space-ocean look fixes.

**Commander play fixes (untagged tip):** undocked deck hides Shipyard / Outfitting / Mission board; menu ship viewport orbit slowed; Factions X cycles Story channel lore, Triangle locks nearest contact. Station disembark layout stays ART-owned (no verb-bar restore).

### 2.5.41 — Art freeze + living-galaxy pack

ART tip `3f0dc20` (ART_AMBER) + galaxy `bc56842`. Smoke-green Commander download. Follow-on: MacVenture PACK/EXITS overlap removed; space meteors + station sparkle glitter dropped for soft haze.

### 2.5.38 — soft-FB Wave A canopy FX

Denser NPC/player engine plumes, boost heat shimmer, hit sparks and explosion embers in `space-fx.h`. Fixed pools, canopy-clipped; high contrast / warp / dock mute decorative sparks. Station art kit re-baked. No crawl or story ownership changes.

### 2.5.37 — unified playable tip

One pack: MacVenture station (art-kit soft-FB, SHIP return, talk/shop/gift/taxi) + Act I–IV eight-beat page scripts/codas (Ch.02–25 through berth). ART DIRECTOR warmer planets / settlement silhouettes / richer space folded in (`planet.h` / `voyage.h`). Post-unify A++ MacVenture polish composition targets: `docs/CINEMATIC-MOCKUP-TARGETS.md` (hero focus, fewer frames, warm staging) — do not edit story saga ownership.

### 2.5.36 — MacVenture + art kit wire

Soft-FB rooms use `station-art-kit.h` styles (PR #12 bake). Talk tips lead somewhere; ship return remains TRI / >>SHIP / EXITS. Kit boards are proposed — not claimed as baked runtime textures.

### 2.5.34 — MacVenture station deck

Illustrated rooms + LOOK/SPEAK/GO/TAKE hotspots (Shadowgate / Deja Vu grammar). Dense clickables; named doors in EXITS; TRI / YOUR SHIP boards from any room. Small interior; maze crawl retired.

### 2.5.30 — MM6-style station crawl polish

Crisp three-depth station walkaround: riveted panels, checker floor, destination-labeled doors, portrait NPC sprites, clearer room chrome. Interactions unchanged.

### 2.5.29 — soft-FB space FX kit

In-engine nebula, clouds, and gentle twinkle (`space-fx.h`). Constant meteors and station glitter masks removed (Commander FX quieting). No GU particle dependency; high contrast skips decorative haze.

### 2.5.28 — OpenEnroth-style station crawl depth

Facing-relative side portals, arched passages, tile floor, ceiling beams, denser props; interactions unchanged.

### 2.5.27 — ask-then-answer story chat

Prologue/saga Cross speaks the commander line first; NPC answer on the next beat. Saga replies are acknowledgments, not pre-answered questions.

### 2.5.26 — GalNet WANTED page cue + chrome

WANTED board gold page-2 banner and five poster wear styles; larger `<L`/`R>` pads clear of NEWS/JOBS.

### 2.5.25 — pixel-art suns + bloom

Eight animated sun sprite families shared across flight/UI; additive corona and sparse canopy bloom. Chart tints match live bodies.

### 2.5.24 — chat flow, GalNet wanted, sun bloom, station FP

Prologue asks then answers. WANTED board pages and poster variety. Animated system suns + bloom. Station crawl FP closer to OpenEnroth corridor style.

### 2.5.23 — menu zoom, speech wrap, battle talk

Select deck ship view orbits farther out. Speech/notice panels wrap instead of truncating. Combat radio uses `SFX_TALK` with battle chatter (respects Quiet Comms).

### 2.5.22 — station crawl + stocked outfitting

Disembark opens a first-person room-grid station (minimap, shops, gifts, taxis). Outfitting lists only local stock; `inventory_screen` shows equip slots. Passengers occupy 1t with route chatter (save V11).

### 2.5.21 — full screenplay assembly

`docs/OPEN-CHANNEL-SCREENPLAY.md` now holds the merged Act I–IV feature script (character bible, lore ledger, full scenes, branch matrices). Playable trust/choice wiring unchanged from 2.5.20.

### 2.5.20 — Open Channel screenplay + trust

Canonical long-form story is `docs/OPEN-CHANNEL-SCREENPLAY.md` (characters, full scenes, branching, lore ledger). `saga_choice_label` distinguishes the four permanent decisions; `saga_trust` surfaces helpers from chapter 19 and colours the epilogue. Display flip remains IMMEDIATE after vblank.

### 2.5.19 — hardware display flip

Main-loop `sceDisplaySetFrameBuf` uses `PSP_DISPLAY_SETBUF_IMMEDIATE` after vblank. Do not switch back to `NEXTFRAME` for ordinary frames — that caused live-buffer painting and black strobing on PSP. Sleep/resume still rebuilds both planes.

### 2.5.18 — varied planet landings + on-foot chrome

Surfaces pick a biome from the orbit sprite family and tint ground/flora/fauna from `Body.color`/`accent`. Planet EVA uses dedicated ON FOOT chrome (nub look, D-pad move). Station disembark remains on the docked Fly menu with talkable concourse NPCs.

### 2.5.17 — engine trails on the stern

Exhaust plumes and aft glints use the mesh rear tip (and freighter capital nozzles), matching the drawn silhouette. Do not reintroduce a radius-scaled glow behind the ship — that floated past wide hulls such as VIPER and COBRA MK 3.

### 2.5.16 — SHIPS vs ENEMIES

SHIPS lists all alive ships (hostiles included). ENEMIES lists only contacts currently engaging the player (`target == -2`). Browsing SHIPS stays on SHIPS when you highlight a hostile.

### 2.5.15 — RED ALERT bottom banner and ENEMIES band

Combat status uses a bottom-of-canopy **RED ALERT** strip so top speech stays free. Hold Square + Left/Right cycles PLANETS / SHIPS / STATIONS / OTHER / **ENEMIES**. From 2.5.16, ENEMIES is engage-only (`target == -2`); SHIPS lists every alive ship.

### 2.5.14 — locked story conversations

First-flight and Open Channel briefs are linear locked conversations (prologue six beats; saga eight). Circle and Select cannot leave Tracked Mission until the player accepts the next step. The last beat restates the objective; after accept, old dialogue options are gone and chatter only reinforces the current mission step. `src/saga.h` stores `line` plus `talk2`–`talk8` per chapter.

### 2.5.0 — The Open Channel

`src/saga.h` adds a 24-chapter data-driven follow-on to the first-flight prologue. Story state lives in `Game` as `saga_chapter`, `saga_step`, `saga_flags`, `saga_choice`, `saga_dest`, `saga_start` and four trust values. Save format 9 appends ten little-endian values and still imports older saves.

The currently playable chapter actions are deliberately compact: dock, scan, defeat hostile ships, return home or make a choice. The campaign bible contains richer bespoke scenes that still need staged implementation. Do not describe all proposed set-pieces as already implemented.

### 2.5.1 — custom MP3 quality

`src/audio.h` uses the PSP hardware MP3 decoder. It now has a 64 KB compressed stream buffer, 2,048-frame stereo output blocks and bounded Catmull-Rom resampling for 32/44.1/48 kHz sources. Suspend/resume tears down and safely recreates decoder resources. Preserve the larger buffers; the earlier 16 KB/256-frame design produced intermittent crackle on hardware.

### 2.5.2 — galaxy navigation

The nearby list only shows local candidates. Triangle opens a spatial overview of all 256 systems. It marks the current system in cyan, the tracked mission destination in gold, and caches/draws every intermediate jump. L/R zoom from 1× to 4×. X converts the selected long route into its first reachable jump. This logic is generic; **Quator has no special code or significance** and was only the system that exposed the old UI flaw.

Story navigation plans against the fitted drive even if the tank is empty, then marks the next hop as low-fuel until the player refuels. The story screen distinguishes `NEXT` from `FINAL`, and the cockpit names the next reachable hop.

### 2.5.13 — Select deck third-person ship

Command deck top-right inset orbits the fitted hull in local space (stars + station or planet), large enough to read the silhouette.

### 2.5.12 — local wanted under system name

Cockpit header shows `Wanted n/5` on the line under `System:` (red when police may pursue, dim at 0/5).

### 2.5.11 — mission cue top-right

Tracked HUD cue sits in the top header band, right-aligned with a 2-col margin (not flush to the screen edge), clear of the danger badge. Objective ink is ART amber (`RGB(240,180,91)`).

### 2.5.10 — Start power on cockpit meters

Hold Start: Left/Right pick SYS/ENG/WEP on the existing meters; Up/Down move pips. No separate overlay panel.

### 2.5.9 — hard brake / heat polish, target bars, freighter fight-back

Double-tap L hard brake and heat rules keep their regressions. Target panel shows HULL/SHLD bars. Damaged freighters return fire and destroying non-pirates raises extra local warrant. Engine plumes attach at mesh aft. Station walk is a FLY deck door with talkable NPCs; planet surfaces pull tint from world colours.

### 2.5.8 — per-system skies and far warp-in

Orbital templates, world-type permutations and seeded traffic make each system look different. Hyperspace drops you farther from the hub.

### 2.5.7 — radio chassis and audible static

Radio UI is a dial chassis (OFF + 1–5). Retune injects audible static; OFF is silence. SHIPS lists all ships; ENEMIES lists only contacts currently engaging the player.

### 2.5.6 — red alert and enemies band

Combat cues use a bottom-of-canopy RED ALERT strip. Square+D-pad scanner bands include ENEMIES (engage-only). Galacticnet Spacebook/Messages and Codex Systems/Planets remain as shipped in 2.5.5.

### 2.5.5 — speaker chips, menus, radio, system variety

Dialogue name plates use `speaker_name_tag` so `NAME SAYS` sits on a faction-coloured chip. Tracked Mission draws speaker portraits. Outfitting explains tech gates in plain language. Galacticnet order is Spacebook then Messages with a parody logo. Codex lists Systems and Planets discovered by visiting. Radio is a tuner with OFF, static on retune, and Triangle power. Planet types/traffic layout vary per system; hyperspace arrival is farther from the hub.

### 2.5.4 — sleep/resume recovery

Long PSP sleep could leave the MP3 decoder blocked on Memory Stick I/O and the LCD framebuffer unrestored, producing a permanent black screen on wake. Suspend now freezes MP3 sampling from the power callback; resume rebuilds display mode, both framebuffers, controls, clock and audio. Frame presentation uses IMMEDIATE after vblank (NEXTFRAME was reverted in 2.5.19 — it strobed on hardware). Emulator smoke covers a double recover path; confirm on physical hardware after multi-hour sleep.

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

The last verified 2.5.32 run passed every group under PPSSPP (game, input, steering, radio, performance), including ask-then-answer script checks. PPSSPP success does not replace physical PSP testing.

## Highest-priority remaining work

Planetary lane: the specialist/planetary-eva checkpoint now covers controls, shared terrain, local bounds and ship return (pending lead integration). Next, runtime-confirm mineral-free worlds and agree one guaranteed activity plus durable reward/revisit rules with Gameplay and Systems. See docs/PLANETARY-EVA-HANDOFF.md; do not promise these next mechanics as implemented.

Audio: measure malformed-file retry cost and output progress after sleep with
Systems; current suspend assertions only prove calls returned. Physical crackle,
custom-MP3 headroom and long sleep listening remain open. WAV/live rescan,
per-station shuffle history, warning priority and sustained EVA layers are future
coordinated slices, not implemented features of this candidate.

1. **Confirm 2.5.4 sleep/resume on physical PSP.** Put the handheld to sleep mid-flight and mid-radio for several hours, then wake — screen and audio must return. Also re-check 20+ minute MP3 playback across sample rates.
2. **Deepen the 24 chapters further.** Briefings now carry full spoken sentences and authored asks; many bible set-pieces still resolve through generic dock/scan/hunt actions rather than unique scenes.
3. **Visually inspect the full galaxy map at 480×272** and the new Select-deck ship preview / radio tuner on hardware.
4. **Physical performance and memory audit** of galaxy routing and the 64 KB MP3 buffer.

## Known limitations and honest status

- The design bible describes a far larger game than the current executable. Interiors, planetary exploration and spacewalks are bounded prototypes rather than Starfield-scale simulations.
- The campaign has 24 playable chapter records with ask-then-answer briefs and voiced closers (2.5.32), but does not yet contain ten hours of unique bespoke mechanics. Travel and ordinary play contribute to its intended duration.
- Only one galaxy seed of 256 classic Elite-style systems is active.
- The full-galaxy chart shows all systems, a cached route and a saved manual route goal, but has not yet had user testing on a physical PSP.
- The audio fix passed PPSSPP with the user's files; intermittent real-hardware behaviour still requires listening tests.
- Manual route persistence is playable in 2.5.3; sleep/resume black-screen recovery is in 2.5.4 but still needs multi-hour hardware confirmation.
- Speaker colour chips and clearer Outfitting tech copy shipped in 2.5.5.
- Full movie-length scenes still live in `docs/OPEN-CHANNEL-SCREENPLAY.md`; the executable binds six spoken beats per chapter.

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

Validation for 2.5.69: build succeeded; all five smoke suites passed in
C:\Users\skarm\Documents\Codex\2026-09-24\let\work\smoke-20260924-213124-036.
EBOOT SHA256: 00C66FD9C82FBFCE337F07BA24CF0CFAB7DF081A2450B242E208E8D1C0AB1870.
Physical PSP testing remains unverified.
## Released 2.5.75 — staged hyperdrive jump

Galaxy-map jumps now return directly to the cockpit and run an eight-second staged sequence: charging lines, stronger shake and blue energy, a vivid multi-colour hyperspace corridor, then braking before the existing system-arrival logic runs.
## Released 2.5.76 — longer engine boost endurance

Boost heat is now lower and scales with the ENG capacitor: higher ENG settings reduce heat generation and improve cooling while boosting. Critical lockout and destruction safeguards remain unchanged.
## Released 2.5.77 — concise cargo menu label

The SHIP tab now shows `Cargo` while flying and keeps `Cargo & market` while docked.
