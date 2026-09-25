# Changelog

## 2.5.145 — Native pixel monitor for Ship Decorator

- Replaced the generic decorator panels with an original 480×272 pixel-art CRT and custom-site plate, inspired by the user's monitor references.
- Reserved fixed artwork windows for eight selectable paint finishes and a live, enlarged 3D preview of the current hull.
- Added swatches, equipped/price feedback, a compact message area and controls inside the monitor frame.
- Kept preset swatches separate from each ship's applied colour, fixed the paint fee to match its displayed units, and expanded the per-ship paint array to cover all ten purchasable hulls.
- Quantized the plate on a 2-pixel grid for sharper PSP-scale art, added a wireframe outline to the live ship, and kept unrelated mission notices off this screen.

## 2.5.144 — Loadout quick stats

- Removed the redundant `CURRENT SHIP` label from the Outfitting loadout panel.
- Added a clear `MISSILES n` count beneath the utility slot.

## 2.5.143 — Shipyard descriptions

- Replaced `ABOUT THIS HULL` with a full three-line `SHIP DESCRIPTION` block in the open lower area.

## 2.5.142 — Radio host alignment

- Nudged the tiny radio host face down a few pixels for cleaner alignment beside the ticker.

## 2.5.141 — Shipyard units badge

- Added the fixed bottom-right units balance to Shipyard, matching Outfitting and Decorator.

## 2.5.140 — Outfitting copy fit and readability

- Expanded the lower-left item description to three native-width lines.
- Wrapped long Effect text so module stat information cannot run out of the panel.
- Compressed cargo capacity to one clean line: `CARGO HOLD 2/8T`.

## 2.5.139 — Outfitting stock scrollbar

- Renamed the stock panel to `AVAILABLE TO BUY`.
- Replaced the page counter with a slim left-side scroll rail and thumb.

## 2.5.138 — Cargo hold label

- Replaced the cramped `HOLD` line with a clear `CARGO HOLD` label and separate capacity count.
- Kept the label and count inside the left loadout panel boundary.

## 2.5.137 — Complete stat change copy

- Replaced all remaining `SEE EFFECT` placeholders with concise concrete stat results for every outfitting module.

## 2.5.136 — Outfitting copy cleanup

- Removed the redundant `READY TO FIT` and `CURRENT EMPTY` labels.

## 2.5.135 — Outfitting description band

- Moved highlighted module descriptions into the open lower-left area of Outfitting.
- Kept the right card focused on effect, exact stat change, fit state and cost.

## 2.5.134 — Chunky monitor decorator

- Reframed Ship Decorator inside a large native pixel monitor bezel.
- Added a few larger hand-drawn sticker/ad blocks and simplified the page copy.
- Kept only the paint choices, live ship preview and essential purchase status visible.

## 2.5.133 — Expressive radio host

- Enlarged the tiny host face to a readable 12×9 pixel badge.
- Added smile, talking, surprised and blink expressions that cycle during broadcasts.

## 2.5.132 — Radio host placement

- Moved the animated host face directly beside the scrolling radio ticker so it reads as the speaker.

## 2.5.131 — Exact outfitting stat previews

- Added a concise before → after stat line beneath every module effect.
- Cargo upgrades now show exact storage changes such as `10 > 18 MAX STORAGE`.

## 2.5.130 — Outfitting detail layout

- Moved item descriptions to the open lower area of the detail panel.
- Removed the unexplained TECH level label and kept cost, effect and fit status clear.

## 2.5.129 — Separate fuel accounting

- Fuel no longer consumes a cargo tonne or blocks refuelling when the hold is full.
- Outfitting now shows the current fuel level directly above the fixed units balance.

## 2.5.128 — Ship Tech Board clarity

- Shortened empty slot labels so they stay inside each hardware tile.
- Replaced the overlapping right-panel heading with a clean Slot Details header.
- Added four-direction D-pad movement across the 2×3 loadout grid.

## 2.5.127 — Pimp My Ship web page

- Reworked Ship Decorator into a radically different GalacticNet-style fake website.
- Added browser chrome, navigation links, sponsored customs tiles, featured preview framing and web-store copy while preserving repaint controls.

## 2.5.126 — Radio ticker visual cleanup

- Moved the long waveform rail below the scrolling radio ticker.
- Replaced the second equalizer with a tiny animated talking-host face.

## 2.5.125 — Boost speedometer clarity

- Speed fill now stays on the normal cruise scale while boosting instead of appearing to reset.
- Boost adds a red, pulsing/jittering meter treatment; the numeric SPD readout remains clear beside the shortened bar.

## 2.5.124 — C64 sticker pack

- Added chunky native-resolution workshop stickers, badge blocks and bezel graffiti to Ship Decorator.
- Kept every mark inside the PSP-safe panels so the playful art never collides with the paint list or live ship preview.

## 2.5.123 — Ship Decorator personality pass

- Added a playful custom-hull-shop presentation with workshop signage, sticker-like bezel decals and clearer “apply finish” language.
- Kept the live preview, paint list, prices and fixed credits badge inside the native PSP layout.

## 2.5.122 — Comms Panel conversation controls

- Flight chatter now shows `△ CLOSE` for one-way messages and `△ REPLY` for reply-capable contacts.
- Triangle on a reply-capable transmission opens the Talk/Ignore choice; holding Triangle still opens the Comms Panel.
- Renamed the deck service to Comms panel, moved Display & Chatter settings into it, and kept those controls available from the Fly group.

## 2.5.121 — live decorator preview

- Ship Decorator now has a dedicated top-right live display port.
- Moving through finishes recolours the preview immediately; the applied ship colour still changes only after purchase.

## 2.5.120 — targeting lock prompt

- The held-Square targeting computer now shows the concise `(R = LOCK)` prompt at its base.

## 2.5.119 — flight speed and approach validation

- Speed lines now scale from low cruise through dense boost streaks, with the numeric speed shown beside the bottom-right speed bar.
- R/L acceleration, braking, double-tap boost and hard-brake behavior remain covered by the flight input checks.
- Relay docking regression now verifies the visible approach sequence before services open.

## 2.5.118 — wanted docking interception

- A wanted commander is intercepted by local law before station entry and must settle the warrant, surrender to custody, or run before docking can continue.

## 2.5.117 — docking approach and debug tools

- Relay and outpost docking now use the visible approach and welcome sequence instead of skipping to services.
- Added debug actions for installing a pulse laser, revealing Codex records and restoring heat and shields.

## 2.5.116 — combat target lock

- Firing at a selected ship or freighter now promotes that contact to the active lock immediately, keeping the target HUD synchronized with the attack.

## 2.5.115 — archive progress and service balance clarity

- Added a right-side progress scrollbar to each three-file Cosmic Archive category.
- Kept ship service credits in the fixed badge and ship loadout view.

## 2.5.114 — weapon validation and shipyard stat checks

- Mining regression fixtures now equip a real pulse laser, matching the player rule that empty weapon slots cannot fire.
- Shipyard and flight continue to use the same hull capacity, speed, range and price data for each ship.

## 2.5.113 — clearer commodity warnings

- Restricted cargo now uses a concise red `RESTRICTED GOODS!` warning.
- Removed the redundant buy/sell advice subtitle beneath market averages.

## 2.5.112 — weapon gating and outfitting labels

- Firing is now disabled until a weapon is fitted in the WPN slot, with a clear outfitting prompt.
- Widened outfitting catalogue labels so full item names remain readable.

## 2.5.111 — bounded shipyard catalogue

- Shipyard hull listings now stay inside a seven-row scrollable panel with a left-side thumb.
- Added a compact description for the selected hull in the lower-left information bay.

## 2.5.110 — control icon and radio HUD cleanup

- Removed the redundant `RADIO` caption beside the cockpit equalizer bars; the bars and scrolling chatter remain.
- Standardized footer controls so D-pad directions render as arrows and shoulder controls render as L/R button icons, with Start/Select and face-button glyphs preserved.
- Kept the held-Square targeting hint and power-bank selection free of extra underline clutter.

## 2.5.109 — held-Square targeting guidance

- Removed the cluttered D-pad, band and release instructions from the held-Square overlay.
- Added a high-contrast `R LOCK ON` callout.
- Replaced clipped scan-tab abbreviations with a full `BAND: ...` label for the active targeting category.

## 2.5.108 — HUD and station service layout polish

- Moved the speed bar directly below the FUEL bar and removed its former upper-right numerical readout.
- Fixed the Display & Chatter list so Third-person view is selectable and functional.
- Moved Ship Decorator and Engineers into the docked Ship category as separate services.
- Decorator selection now scrolls through eight finishes, including Neon Grid and Starfall Theme, with a live painted ship preview.

## 2.5.107 — World State v1, Mission Validation v1 and Lave certification

- Added the canonical world-state snapshot for economy, danger, government, factions, traffic, stations, planets and active events.
- Routed mission display and acceptance through reachability, station and landable-body validation.
- Added Lave certification checks for peaceful onboarding, faction reconciliation, reputation progression, planet identity and playable mission offers.
- Documented the release gate in `docs/WORLD-STATE-V1-MISSION-VALIDATION.md`.

## 2.5.106 — richer Discovery Codex system previews

- System archive entries now show their matching station, star or planet artwork in the right-hand description panel.
- Scrolling through a system updates the panel with the selected record's short summary before opening its full X detail page.

## 2.5.105 — ship decorator and third-person display

- Added a station-only Ship Decorator menu with eight paid paint finishes and a live exterior preview.
- Added a Third-person view toggle under Commander → Display & Chatter.
- The selected paint colour is used by the ship preview and in-flight exterior view; Triangle from the decorator opens Engineers.

## 2.5.104 — boost cooling, speed HUD and damage smoke

- Heat now cools immediately after boost release, including while the ship is still travelling at high speed.
- Added a compact lower-right speed readout and bar above the power indicators.
- Reworked cockpit damage smoke into dim particle drift whose count increases with hull damage.

## 2.5.103 — clear cockpit radio ticker

- Removed the `ALIEN` label from the upper-right radio strip so it cannot overlap the scrolling ticker subtitle.
- All stations now use the same compact `RADIO` identifier; station-specific colour and broadcast copy remain intact.

## 2.5.102 — native targeting computer rewrite

- Reworked the full targeting page into a 480×272 monitor layout with a six-row contact list and dedicated readout panel.
- Added clear contact status, faction/class, range, mission color, compact hull/shield bars and a short Triangle details reveal.
- Kept the existing monitor bezel/stickers and held-Square flight targeting controls while removing overlapping center-view text.
- Documented the layout and validation rules in `docs/TARGETING-COMPUTER-PSP-SPEC.md`.
- Fixed left/right steering inversion after a full vertical loop by mirroring yaw input across the pitch pole.

## 2.5.101 — radio station/level navigation

- Removed the duplicate STATIC label during station changes.
- Radio Up/Down now switches only between Music and FX levels; Left/Right adjusts the selected level, while L/R changes station.

## 2.5.100 — quick-response comms

- Triangle taps now ignore an incoming chat immediately.
- Holding Triangle opens an in-flight Respond/Ignore overlay, while Respond continues into a fuller multi-option discussion screen.

## 2.5.99 — planet survey records

- Removed Flora and Fauna from the Discovery Codex section cycle.
- Selected landed planets now open expanded survey pages with flora, fauna and mineral records.

## 2.5.98 — cosmic Galactic Lore archive

- Removed Galactic Lore from the Discovery Codex tab cycle; it is now opened only from the Discover menu entry.
- Rebuilt the lore view as a readable cosmic archive with six sections, 18 expanded entries and lightweight 1950s pulp-sci-fi-inspired cover art.

## 2.5.97 — landing-gated planet Codex

- Discovery Codex planet and system archive entries now appear only after the commander lands on that planet.
- Landing flags persist in saves, and the ship computer confirms the first time each planet is added to the Codex.

## 2.5.96 — first-arrival system briefings

- On the first warp into a system, the ship computer now announces the arrival and reports its economy, government, technology and local danger level.
- The briefing confirms that the system’s station and four planets were added to the Discovery Codex; repeat visits use the normal concise arrival message.

## 2.5.95 — matching Discovery Codex planets

- Discovery Codex planet entries now use the exact in-system names (`Lave 1`, `Lave 2`, etc.) instead of generic World labels.
- Codex planet records and previews now use the matching system body type, palette, seed and artwork.

## 2.5.94 — readable Start/Select icons

- Changed the tiny L/R, Start and Select symbols to black rounded PSP-style buttons with white letter marks.
- Start shows `ST`, Select shows `SE`, and the shoulder buttons show `L`/`R` so they are immediately distinguishable at small size.

## 2.5.93 — unified PSP control icons

- Added a shared tiny icon set for Cross, Circle, Triangle, Square, D-pad directions, the full D-pad, L/R shoulders, Start, Select and the analog nub.
- Updated footer prompts to use consistent aligned symbols and PSP button colours instead of mixed text-only labels.

## 2.5.92 — richer cockpit radio chatter

- Expanded each radio station’s cockpit ticker from 10 to 24 snippets, including adverts, alien call-ins, arguments and strange talk-show moments.
- Added a short varied pause before each new ticker line begins scrolling, making broadcasts feel less mechanical while staying PSP/60 FPS friendly.

## 2.5.91 — solar-system Discovery Codex

- Discovery Codex now drills down from visited solar systems into their station, star and four worlds.
- World records show potential flora and fauna; system records summarize minerals, echoes, life logs and station traffic.
- Added X to open a selected record and Circle to step back through the hierarchy.

## 2.5.90 — expanded Galactic Lore archive

- Added `GALACTIC LORE` as its own Discover menu entry beside Discovery Codex and GalacticNet.
- Expanded the lore archive to twelve readable entries covering the galaxy, Sol, colonies, hyperspace, pilots, law, history, science, trade and the future.
- The lore screen now uses the full panel layout for longer descriptions and clear entry numbering.
- Renamed the station menu action to `DISEMBARK`.

## 2.5.89 — PSP XMB identity artwork

- Added an Elite: NEXT game icon for the PSP XMB using the existing Elite logo.
- Added a deep-space starfield background for the PSP XMB game selection screen and packaged it into EBOOT.PBP as PIC1.PNG.

Note: standard PSP XMB backgrounds are still images; the game itself retains the animated starfield once launched. A true animated XMB background would require a firmware-specific PMF/ICON1 video asset and encoder that is not present in the project toolchain.

## 2.5.88 — clearer radio tuning feedback

- Replaced the radio menu's `TRI POWER` text with a green PlayStation Triangle button glyph.
- During the brief station-change static interval, the station title now reads `STATIC` instead of showing the old or new station name.

## 2.5.87 — visible hyperspace arrival

- The destination solar-system cockpit view now fades in through a cool blue arrival wash during the first 1.35 seconds after hyperspace braking completes, while the cockpit HUD remains readable.

## 2.5.86 — ambient space encounters

- Added a lightweight encounter deck for trader traffic, police, pirates, distress calls, cargo, wreckage, smugglers, bounty leads, and mysterious contacts.
- Holding Triangle on an encounter transmission now opens Respond/Ignore in the existing comms panel; Respond follows the encounter hook and Ignore closes it cleanly.
- Encounter timing and presentation reuse the existing speech and traffic systems so the new activity stays PSP/60 FPS friendly.

## 2.5.85 — reliable docking, flight attitude, and warp presentation

- Removed the unused decorative shape that could appear at the top-left of the flight view.
- Circle now prioritizes a centered station in docking range, and guided docking recovers safely into the third-person arrival sequence.
- Boost release clears residual roll so flight steering cannot remain rotated after boosting; full pitch loops remain intact.
- Hyperdrive charge now keeps the ship view visible and progressively dimmed before the hyperspace corridor begins.

## 2.5.84 — functional power distribution

- WEP pips now use a shared weapon-output multiplier for ship lasers and mining.
- Added regression coverage for WEP output and SYS/ENG power effects.

## 2.5.83 — clearer mission and wanted HUD cues

- Tracked mission instructions now show in green with `>> ... >>` route arrows.
- Kei's opening mission remains the default tracked objective and is covered by the HUD tracking checks.
- The idle `WANTED 0/5` counter is hidden; wanted commanders instead see an escalating orange/red flashing `LAW IS AFTER YOU!` warning.

## 2.5.82 — cockpit text radio

- Far Horizons is now the default station on a fresh launch.
- Added a lightweight scrolling radio-talk ticker beside the cockpit activity animation, with varied station-specific captions.
- Radio controls now use PSP L/R for tuning, Up/Down for Music/SFX selection, and a visible Triangle power button.

## 2.5.81 — cleaner cockpit menu footer

- Removed the `LEFT/RIGHT TAB / UP/DOWN` navigation hint from the main cockpit menu footer.

## 2.5.80 — menu wording refresh

- Updated launch, station exploration, shipyard, outfitting, controls, radio, and loadout subtitles.
- Undocked Ship menu continues to show `Cargo` instead of `Cargo & market`.

## 2.5.79 — law warnings and no-funds custody

- Wanted commanders now receive a local law warning before an arrest is initiated.
- Zero-unit custody now plays a jail transfer and law seizure sequence before releasing the commander with only the basic ship at the local station.

## 2.5.78 — readable Galactic Lore details

- Galactic Lore entries now use a full-width bottom panel with wrapped text instead of the narrow right detail column.

## 2.5.77 — concise cargo menu label

- The undocked SHIP menu now labels the cargo screen simply `Cargo`; docked stations retain `Cargo & market`.

## 2.5.76 — longer engine boost endurance

- Boost generates less heat overall, and ENG pips further reduce boost heat while increasing active cooling.
- The overheat warning now reads `ENGINES OVERHEATING... COOL OFF!!` while critical-overheat protection remains intact.

## 2.5.75 — staged hyperdrive jump

- Galaxy-map jumps now return to the cockpit and build through charging, escalating streaks/shake, a vivid hyperspace corridor, and a braking phase before arrival.

## 2.5.73 — Kei replies directly

- Choosing a reply in Kei's opening briefing now advances straight to Kei's response without redrawing the commander's selected speech.

## 2.5.72 — attacking enemies alert

- While holding Square, the `ENEMIES` tab now flashes red when the player is under attack or has an incoming missile.

## 2.5.71 — clearer held-Square target controls

- Replaced the old `D-PAD BANDS` scan hint with `L TARGET IN FRONT` and `R LOCK ON`.

## 2.5.70 — unrestricted flight pitch

- Removed the artificial up/down pitch limits during space and atmospheric flight.
- Up and Down now allow continuous looping turns through a full 360 degrees in either direction.
- Added a regression check covering a complete pitch loop; landed surface movement limits remain unchanged.

## 2.5.69 — animated cockpit activity display

- Replaced the top-center heading/danger glyph cluster with a lightweight native-pixel sci-fi display.
- Cycles through drifting stars, radar sweep, telemetry waveform, rotating planet/galaxy, and ship schematic patterns.
- Motion speeds up with cruise velocity and switches to hyperspace-like streaks during boost; danger remains visible through the alert border/markers.
- Preserved the rest of the cockpit HUD and gameplay state.

## 2.5.68 — loadout and mission log readability

- Separated Mission Log main/side sections and objective area so all five contracts remain visible. Hide the Abandon prompt on main missions.

- Corrected pixel/character coordinate confusion that hid ship slot labels.
- Kept module names in the details panel and made the in-flight hint explain docking is required.
- Verified all six selected labels in both palettes, with native captures and a passing emulator smoke run.

## 2.5.67 - Lave signage draw-order fix

- Moved the Lave Public canteen identity signage after the back-wall layer so the nameplate remains visible at native resolution.

## 2.5.66 - Lave canteen visual slice

- Added a Lave-only native pixel-art window, crescent world, hanging lamps, booth silhouette and floor lighting to the public canteen.
- Preserved the existing station room graph, selectors, NPC hotspots and PSP 480x272 layout.

## 2.5.65 - v0.5 gate repair

- Corrected the campaign input regression harness to return to the briefing after opening Mission Log and after backing out of a coda.
- Updated the undocked deck expectation to match the intended hidden-service list: Shipyard, Outfitting and Mission Board are removed from navigation while flying.
- Full emulator smoke now passes game, input, steering, radio and performance checks.

## 2.5.64 - planetary presentation polish

- Added compact native-resolution contact shadows beneath surface flora, bushes and rocks so close props read as grounded in the terrain.
- Kept the effect low-contrast and disabled in high-contrast mode; collision, terrain and EVA behavior are unchanged.

## 2.5.50 - native station visual rollout

- Added a shared native 480×272 station presentation shell with consistent headers, options rail, feedback area and SHIP return treatment.
- Added an authored Reorte primary Arrivals scene using the Second Shift palette and staging language.
- Kept all existing station hotspots and interaction behavior intact while the remaining room art is upgraded.

## 2.5.49 - 2026-09-23

- Added the playable Second Shift bar at Reorte's primary hub using the approved native PSP pixel artwork.
- Added Lysa Kest, Pell Sorn and Dax Neral conversations, safe Reorte I navigation guidance, and free High Orbit dice practice.
- Kept the preview session-only: dice cannot change credits, cargo, missions or saves, and ordinary canteens in other hubs remain unchanged.

## Unreleased - current-ship menu beauty preview

- The command-deck inset shows the actual owned hull in a quiet 96-second
  flight loop with distant system planets, a steady sun and sparse stars.
- Explicit panel clipping and an independent presentation camera replace
  live-world camera mutation. The ship stays fitted throughout its camera arc.
- Optional -MenuPreview smoke checks cover all owned hulls, clipping,
  unchanged simulation, restored render state, loop continuity and timing.

All notable playable releases are recorded here. `VERSION` is the current canonical version.

## 2.5.74 — Trader cargo offers

- Traders now offer a small commodity-for-commodity exchange during a hail. Buy the requested cargo, find the same trader again, and hail them to complete the deal.
- Scanned ship contacts keep their callsigns in the targeting computer for the current system, making it possible to relocate a trader after visiting the market.
- Trader offers remain session-local so the existing V13 commander-save format stays compatible.
- Cargo canisters now use a short tractor-beam pickup animation that stops the ship, holds position, and completes the collection after the beam finishes.
- Discovery Codex gains a `GALACTIC LORE` tab with original compact reference entries about the Milky Way setting.

## 2.5.48 - 2026-09-23

- Reworked planetary EVA with separate look and movement, shared rendered/collision terrain, bounded shores and fields, jet descent, reliable boarding, ship guidance, suit status and the full scan-to-sale regression journey.
- Added a story-awarded atmospheric landing kit while preserving atmosphere escape and completed, skipped and older-story compatibility.
- Replaced the static menu ship inset with a clipped looping beauty view of the commander’s actual ship, current-system sun and planets.
- Added 15 longer original action cues and bounded radio shuffle ordering without changing decoder, sleep, save or game-state formats.

## Unreleased — planetary EVA traversal

- Separate on-foot look, walk/strafe and hold-R jet controls; analog-off fallback and a dedicated fifth Help page.
- Match terrain collision to the rendered triangles, block water/local edges, and permit grounded boarding immediately after disembarking.
- Show ship bearing/distance, exposure and remaining suit health; preserve scan feedback and fill the full EVA viewport.
- Add traversal, resource, save and mineral-to-hub-sale regressions. V13 layout and existing reward rules are unchanged.

## Unreleased — original event audio and bounded shuffle

- Original 20–300 ms event cues give scans, docking, warnings, combat and engines distinct identities with smooth endpoints; existing stereo stations and volume preferences remain unchanged.
- Shuffle visits each candidate once before giving up, deferring the previous track until alternatives fail.
- Added duration, peak/headroom, candidate-order and optional original audition checks; documented MP3-only support and physical PSP crackle/suspend verification gaps.

## 2.5.47 - 2026-09-23

- Planet approaches now stop the remaining collision frame safely, pause threats while the choice is open, and give gas giants truthful turn-away controls.
- Chapter 05 adds its timestamp evidence loop, requires both prior records, and safely reopens compatible older Chapter 05 saves.
- Pilot-rescue objectives now correctly teach Circle to lock and Triangle to hail within 600 m, with the complete pickup, return, payment and retry loop covered by input regressions.

## Unreleased — safe planetary approach

- Planetary boundary interception stops weapons and remaining simulation immediately; active approach choices pause threats and clocks.
- Gas giants offer a truthful Circle escape, and reject X entry with an explanation.
- Approach text and actions stay inside the safety panel in Full, Minimal and Scenic HUD modes.
- Added non-sun boundary, sun damage, turn-away, orbit return, PSP-input and native framebuffer regressions. Save V13 and mission rewards unchanged.

## Unreleased — story-awarded planetary landing kit

- Planetary landing is now gated behind an authored story milestone after the power lesson. Kei awards an atmospheric landing kit found in Ryn's locker, making the first landing a meaningful progression beat without adding a save-field or schema change.
- Atmosphere approach remains available before the handoff, but landing on the surface pad is blocked until the kit is awarded. Completed/free-story saves remain compatible.

## Unreleased — authored Chapter 05 evidence loop

- Added the timestamp comparison at Lave and inspection/protest/open-case choices for the early Meridian filing.
- Both prior records are required; partial evidence, wrong-system returns, and repeated comparison actions cannot advance the chapter. Legacy Chapter 05 saves get an explicit case-reopen recovery. State reuses existing `saga_flags` without a save-version or `Game` layout change.
## Unreleased — pilot rescue control guidance

- Generic pilot-rescue objectives now explain locking the rescue ship and using Triangle to hail within 600 m; Circle locks the contact rather than collecting the pilot.
- Added input regressions for rescue acceptance, tracking, pickup boundaries, unrelated contacts, guided hub return, single contract/Guild payment, and expiry/retry. Rewards, timers and saves are unchanged.

## Unreleased — authored Open Channel Chapters 02–04

- Added sealed-receiver pickup and home delivery, quiet-signal observation with fire/heat reset and re-entry recovery, a required convoy port stamp, and an optional lifeboat scan that grants Independent trust once.
- Reused reserved `saga_flags` bits without changing the save schema; unrelated scans, kills, cargo and generic docking cannot advance these objectives.

## Unreleased — native art pipeline and benchmark scenes

- Added a manifest and validator for 480×272, ARGB1555, nearest-neighbour,
  palette-limited runtime art.
- Registered Station Arrivals, Planet Approach and Campaign Dialogue as the
  first native composition contracts and wired station viewport geometry to the
  shared layout header.

## Unreleased — shared painted cover field

- Flight, docking, and command-deck previews now share a deterministic stepped
  sky field with an asymmetrical period-illustration mass and tiny relay cue.
- The centre remains quiet for the reticle and HUD; no simulation or targeting
  state is added.

## Unreleased — native deck copy guard

- Shortened command-deck helper copy to fit the 27-column detail pane at 480×272.
- Bounded the detail line in the renderer so future copy cannot run into the right edge.

## Unreleased — Systems/QA V13 bounds hardening

- Loadout display and module refunds validate fitted catalog indexes before using equipment arrays.
- Added V13 slot-value regression checks; save version and V12 migration remain unchanged.

## Unreleased — ART targeting CRT glass (tip; no micro-tag)

- Targeting computer monitor gets faint CRT scanlines + sparse static under the list (glyphs stay sharp; no text bloom). No shooting stars.

## Unreleased — Elite-style equipment modules (tip; no micro-tag)

- Outfitting **fits** modules into WPN/DEF/NAV/HOLD/FUEL/UTIL; Loadout shows the same slots; Square/X sells back at 50%.
- Fitted gear changes real stats: mil shield 4.5/s, pulse 24 / beam 36, ECM/chaff missile break, heat-sink dump, auto-repair, mining laser, refinery, pax cabin gate, agri scoop rate, escape pod recover.
- Save **V13** stores fit[6]; V12 loads synthesize slots from upgrade bits. Credits, stock, and loadout agree.

## Unreleased — law system rewrite (tip; no micro-tag)

- Law is a real loop: restricted goods only matter when scanned; warrant vs cargo-scan stops; settle / run; docked desk (Status Square) and CUSTOMS tip can clear heat.
- Assault/kill of protected ships still files immediate warrants. Pirate bounty unchanged. Save V12 / wanted[] migration kept.

## Unreleased — Commander play fixes (tip; no micro-tag)

- Undocked command deck **hides** Shipyard, Outfitting, and Mission board (dock-only services).
- Menu 3D ship viewport camera orbits much slower.
- Factions screen: X cycles Story channel lore (saga helpers only); Triangle locks nearest ship of that colour.

## 2.5.46 - 2026-09-22

- **Equipment rewrite pack:** real outfitting modules that change ship stats (PR #32 `2c0373d`) on the v2.5.45 tip (targeting monitor CRT fuzz retained).
- Outfitting fits WPN/DEF/NAV/HOLD/FUEL/UTIL; Loadout sells at 50%; save **V13** stores `fit[6]`.

## 2.5.45 - 2026-09-22

- **ART targeting monitor fuzz:** faint CRT static on the targeting computer (`targeting_screen` in `ui-modern.h`, tip `178d34a`) on the v2.5.44 law tip.

## 2.5.44 - 2026-09-22

- **Law rewrite pack:** scan → settle → clear loop from PR #29 (`4fea6f2`) on the v2.5.43 tip.
- Illegal cargo (Slaves/Narcotics/Firearms) is not a warrant until Law scans; assault/kill still files heat.
- Cargo scan stop (submit / refuse / run); settle pay/custody/run; docked Status Square fine desk + CUSTOMS tip; leave system keeps warrant.
- Mission accept no longer auto-warrants smuggling. Save wanted[] / V12 kept.

## 2.5.43 - 2026-09-22

- **Commander play-fix pack:** Art HUD/disembark quiet (`1b4e08d`) + Designer dock-only deck services / slow menu cam / live Factions (`eb4d23a`) + Story Gazette tabloid + faction lore (`9a9c45f` / `gazette-lore.h`).
- Undocked deck hides Shipyard, Outfitting, Mission board; menu ship cam slowed; Factions lore channel + gazette voices.
- Drop HOLD SQ lock hint and text bloom; softer space; neat GalNet tabs; station OPTIONS list.
- Galactic Gazette tabloid jokes that still leak lore.

## 2.5.42 - 2026-09-22

- **Commander playable pack:** ART quiet pass `b0a97e1` on the v2.5.41 tip — permanent meteors/station glitter removed; soft docking lights and plumes kept; MacVenture disembark UI decluttered (no PACK/EXITS overlap).
- Retains ART_AMBER cue + living-galaxy travellers (save V12) from 2.5.41.
- **ART look tip (untagged, on PR #11):** drop always-on `HOLD SQ + R: LOCK`; remove vertical wiggly nebula band; softer cosmic-ocean haze (no noisy streaks); bloom never after UI glyphs; GalNet tabs shortened (BOOK/INBOX) so they no longer overlap; MacVenture disembark drops verb row + pack cue — room title full-width, right OPTIONS list is the only selector (X does talk/go/look/deal). No tag until Systems smoke.

## 2.5.41 - 2026-09-22

- **Commander playable pack:** ART tip `3f0dc20` (ART_AMBER cue freeze, no GOLD) + living-galaxy PR #23 `bc56842` (hybrid travellers, save V12).
- Mission cue ART amber with 2-col header margin; smoke green.
- Hybrid travellers: System Details here/inbound, Triangle hail, GalNet elsewhere mentions.
- **MacVenture disembark declutter (untagged tip):** one action-row verbs; full-width MAIN; PACK/EXITS side panels removed; door labels and prop-marker spam dropped; text band is label+look only. Overlap gone at 480×272.
- **Space FX quieting (same tip):** constant meteors removed; station traffic glitter / four-point sparkle masks dropped; window lamps + aperture/docking lights use soft additive haze; travel glitter stars removed. Plumes/nebula haze kept. No tag until smoke green.

## 2.5.40 - 2026-09-22

- **Big combined tip:** ART DIRECTOR McQuarrie visual style + Systems soft-FB Wave B/C beauty + Story tip-batches on MacVenture polish.
- Station MacVenture rooms: wall plates/rivets, floor seams, berth void stars + beacon, stronger hero staging ARRIVALS→CUSTOMS.
- UI chrome: charcoal + cream/ochre instrument panels; gold corner-bracket look removed.
- Soft-FB space beauty: planet bloom/specular/ocean/ice/city lights; travel glitter, solar wind, traffic wakes, dock beacons, anomaly pulses, denser warp tunnel.
- Atmosphere flight: sun bloom streak, water/ice glitter, volcanic embers; gas-giant ring sparkle.
- Story already on tip: Mission Network Vol II, GalNet colour, Guild Vol III, berth-six epilogue (from prior tip-batches).
- **ART DIRECTOR post-pack animation pass (untagged tip):** ARRIVALS freighter drift + blinking berth slots; room practical pulses; fauna families + field walk bob; docking warm aperture + corridor motes + near-station traffic glints; quieter MacVenture chrome. Presentation-only — no spawn/AI ownership.
- **ART DIRECTOR next visual gap (untagged tip):** shop/canteen/cargo/guild/clinic/customs prop personality; planet settlement window blinks + horizon haze + ochre pad apron/beacons; quieter EVA/cockpit/speech/menu/minimal HUD + portrait/card chrome; warmer warp tunnel + approach/police plates. No micro-tag.
- **ART DIRECTOR implementer tip (untagged, under art authority):** native 8×8 hotspot prop markers; ARRIVALS cargo-loader bob; walk/deck/help/comfort charcoal+ochre chrome; docking aperture + prosperity rings use kit cyan (nav only). No micro-tag.
- **Smoke fix (untagged):** mission cue stays ART amber in the top-right header with a 2-col margin (not flush to the edge); smoke expects ochre ink. No micro-tag.
- **Reconcile onto Designer tip #18 (`f003e09`):** rebased art implementer commits atop GOLD-restore tip; cue ink remains ART_AMBER per ART DIRECTOR palette; header-margin smoke assert preserved. No micro-tag.
- **ART look freeze for Systems combine/tag:** PR #11 tip `64977ba` is the art handoff — no further look work until after the bug-fixed EBOOT. Mission cue stays ART_AMBER (do not restore GOLD).
- **Story tip batch (untagged):** Galactic Gazette sometimes runs tabloid/joke pieces that still leak lore; Factions screen rotates distinct lore voices (`src/gazette-lore.h`). No layout/FX ownership. No micro-tag.

## 2.5.39 - 2026-09-22

- **ART DIRECTOR in-game visual style implementation (untagged tip):** MacVenture rooms gain wall plates, rivets, berth-window stars/beacons and stronger hero staging across ARRIVALS→CUSTOMS; deck/UI chrome drops gold corner-brackets for charcoal + cream/ochre instrument rules; Wave B/C soft-FB planet bloom, travel FX, warp tunnel and docking beacons folded from beauty lane.
- **MacVenture A++ composition polish:** each station room has one hero focal object, three depth planes, and warmer ochre/cream staging (`docs/CINEMATIC-MOCKUP-TARGETS.md` + ART DIRECTOR pixel grammar).
- Side hatch doors replace top chrome door strips so the focal object stays clear; UI chrome thins to cream/cyan hairlines.
- Fewer decorative frames (no checker floors / lamp rows); SHIP return and talk/shop/gift/taxi unchanged.
- **Working tip (no new tag):** Story PR #19 folded in — Act III–IV page scripts (Ch.14–25 through berth), dialogue chrome (COMMANDER bubble, no `YOU:`/`YOU SPOKE`), ask→answer realigned.
- Station TAKE on owned props opens the NPC deal; GO snaps to a hatch first — verb payoffs tightened for fun.
- ART DIRECTOR space animation kit folded in: `space-animation-kit.h` plume/beacon/spark masks, warm station exterior windows, meteor sparkle (`docs/SPACE-ANIMATION-HANDOFF.md`).
- ART DIRECTOR A+++ visual pass plan landed as composition guidance (`docs/A-PLUS-PLUS-VISUAL-PASS.md`) — execution continues on this tip without a micro-tag.
- Story confirm answers + Who Keeps the Light choice staging tightened from screenplay (PR #19 follow-up).
- Story Silence/Carry choice staging + spoken prologue asks folded from PR #19 follow-ups.
- **Story tip batch (untagged):** berth-six epilogue + coalition helpers speak screenplay decision echoes; complete-screen Kei line; GalNet Explorer Guild / Kei posts colour from Open Channel flags — land as one fold, not drip commits.
- **Story tip batch (untagged):** Mission Network briefs use Vol II authored banks (Hungry Pad / Listen Twice / Boring Lies flavour); GalNet Spacebook (Mira/Iona/freighter/Sable) + Mission Network react to Open Channel flags; Guild opening dialogue from manuscript Vol III.

## 2.5.38 - 2026-09-22

- Soft-FB Wave A canopy FX on the unified tip: denser engine plumes, boost heat shimmer, hit sparks and explosion embers (`space-fx.h`).
- Cheap fixed pools, canopy-clipped; high contrast / warp / dock mute decorative sparks.
- Station art kit re-baked against the unify tip. No crawl or story rewrites.

## 2.5.37 - 2026-09-22

- **Unified playable tip:** MacVenture station deck (art-kit soft-FB rooms, clear SHIP return, talk/shop/gift/taxi) plus Open Channel Act I–III eight-beat page scripts with choice blurbs and locked codas (Ch.02–19 through No Easy Flag).
- Act III (Ch.14–19) raised to page-script authority from the screenplay: map deposit, Federal/Imperial/Alliance envoys, Coldest Signal, coalition choice.
- ART DIRECTOR in-game look pass folded in: warmer planets, settlement silhouettes, richer space presentation (`planet.h` / `voyage.h`).
- Composition targets for post-unify MacVenture/station polish land in `docs/CINEMATIC-MOCKUP-TARGETS.md` (hero focus, fewer frames, warm staging).
- Single downloadable pack — story + station together.

## 2.5.36 - 2026-09-22

- MacVenture station polish: ART DIRECTOR bake kit palette/styles wired through `station-art-kit.h` (PR #12) into soft-FB room draw.
- Focal anchors, talk tips, TRI / >>SHIP / EXITS ship return. Bake kits remain proposed references for full-screen textures.

## 2.5.35 - 2026-09-22

- Act I Open Channel briefs (Ch.02–07) deepen to eight ask-then-answer page-script beats with screenplay texture.
- Permanent decisions show consequence blurbs; Act I chapter completes open a locked coda page before the next brief.
- Manuscript Volume IV documents the Act I page-script binding (`docs/ELITE-NEXT-MANUSCRIPT.md`).

## 2.5.34 - 2026-09-22

- Station interior rebuilt as a MacVenture-style point-and-click: Command / Main / Exits / Pack / Text windows, LOOK SPEAK GO TAKE verbs, illustrated hotspots.
- Seven lush rooms on a small graph. Talk, shop, gifts and taxis kept. Grid maze crawl removed.

## 2.5.33 - 2026-09-22

- Whole-game story and missions manuscript begun (`docs/ELITE-NEXT-MANUSCRIPT.md`).

## 2.5.32 - 2026-09-22

- Open Channel ask-then-answer story conversations (tagged release).

## 2.5.31 - 2026-09-22

- Open Channel dialogue quality raised to full spoken character voice (see tagged release notes).

## 2.5.30 - 2026-09-22

- Station crawl FP view rebuilt as a crisp MM6-style space dungeon: riveted wall panels, high-contrast checker floor, hanging lamp glow, labeled side/front doors showing destination room names, denser room props, and portrait NPC sprites with name plates.
- Header chrome separates station title from the current room badge; compass DOOR/WALL cue kept. Talk, shops, gifts and taxis unchanged.

## 2.5.29 - 2026-09-22

- Soft-framebuffer space FX kit (`space-fx.h`): per-system nebula ribbons, layered space clouds, galactic band haze, stronger star twinkle/sparkle, rare shooting stars. GU particle libs were surveyed and skipped — they do not fit this software renderer; effects stay additive, bounded, and high-contrast safe.

## 2.5.28 - 2026-09-22

- Station crawl FP view deepened toward OpenEnroth corridor language: depth-layered wall panels, facing-relative side portals with neighbour-room tint, arched passage frames, tile floor grid, ceiling beams, denser room props, minimap door links, step bob.
- Side doors now track turn facing (LEFT/RIGHT relative), with compass + DOOR/WALL cue; shops/talk/gifts/taxis unchanged.

## 2.5.27 - 2026-09-22

- Story chat is ask-then-answer: Cross speaks your line first (orange YOU bubble); the next Cross reveals Kei's reply. Saga briefs use acknowledgments instead of questions that were already answered on screen.
- Campaign input tests lock the ask→echo→answer order so the catch question never shares a beat with its answer.

## 2.5.26 - 2026-09-22

- GalNet WANTED board makes page 2 unmistakable (gold `>> PAGE 2` banner + bottom DOWN cue); five distinct poster wear styles (clean, corner rip, fray, dog-ear, bullet hole).
- GalNet L/R tab buttons are larger (`<L` / `R>`) and sit farther from NEWS / JOBS so the triggers no longer crowd the section names.

## 2.5.25 - 2026-09-22

- Eight animated 32x32 pixel-art sun families (yellow, blue, white, red giant, orange, violet, flare, binary) cycle frames in flight, charts, codex, system details and planet skies.
- Chart/codex body tints match the live system palette; soft additive corona bloom, anamorphic lens streaks and sparse canopy bloom stay glitch-free without a second framebuffer.

## 2.5.24 - 2026-09-22

- Prologue / saga replies ask first, then hear the answer on the next beat.
- GalNet WANTED board shows page 2 cue, varied torn/weathered posters; L/R sit farther from tab names.
- Per-system animated sun sprites with soft bloom rings (flight, cards, codex); sparse canopy bloom + lens streaks.
- Station crawl first-person view uses OpenEnroth-style layered walls, tile floor, door frames and room props.

## 2.5.23 - 2026-09-22

- Command deck third-person ship inset pulls the camera farther out so the full hull silhouette reads clearly.
- Speech and notice boxes word-wrap to their panel width — no more mid-sentence hard cuts in Kei briefs, flight speech, mission next-steps or station talk.
- Battle talk SFX: distant engagements and close combat now open the channel with a radio-voice cue (muted with Quiet Comms), plus short combat chatter lines.

## 2.5.22 - 2026-09-22

- Station disembark is a first-person NES dungeon-crawler with room grid, minimap, shops, gifts, quest tips and taxi passengers.
- Outfitting only lists gear this hub stocks (economy + tech); no more "needs tech 7 / warp richer" teases. Expanded catalog (~24 modules).
- Ship Loadout screen shows equip slots and cargo/passenger manifest. Passengers use 1t and chatter en route; fare on arrival. Save V11.

## 2.5.21 - 2026-09-22

- Expanded Open Channel screenplay to full Act I–IV authority (~2100 lines): character bible, lore ledger, complete scenes and branching outcomes.

## 2.5.20 - 2026-09-22

- Feature-length Open Channel screenplay (`docs/OPEN-CHANNEL-SCREENPLAY.md`): cast bible, full scenes, branching trees, lore ledger.
- Choice UI labels now match each permanent decision; trust drives helpers, rewards and epilogue colour.
- Retained IMMEDIATE framebuffer present (anti-strobe) and re-checked in campaign tests.

## 2.5.19 - 2026-09-22

- Fixed screen strobing / black flashes on hardware: framebuffer flip is IMMEDIATE again after vblank (2.5.4 NEXTFRAME painted the live buffer).

## 2.5.18 - 2026-09-22

- Planetary landings use biome families from each world’s orbit sprite (ocean / arid / ice / volcanic / forest), with ground, sky, flora and fauna tinted from that body’s colours.
- On-foot planet UI matches the station concourse: clear ON FOOT chrome, nub look + D-pad move, Square scan, Circle board.
- Docked Fly menu **Disembark / walk station** opens the concourse with more talkable locals.

## 2.5.17 - 2026-09-22

- Engine fire trails and aft glints start on each ship’s mesh stern (and freighter nozzles), not floating past the silhouette on a radius offset.

## 2.5.16 - 2026-09-22

- SHIPS band lists every ship, including hostiles; locking a hostile no longer flips you off SHIPS.
- ENEMIES lists only ships currently going after the player.

## 2.5.15 - 2026-09-22

- Bottom canopy **RED ALERT** stays clear of top speech during combat (status no longer hijacks the speech box); scenic HUD still shows the banner.
- Hold Square + Left/Right always cycles through the **ENEMIES** band (hostiles only), even when the list is empty.

## 2.5.14 - 2026-09-22

- Story briefs (first flight and every Open Channel chapter) run a six-beat locked conversation: Circle and Select stay blocked until you accept the next step.
- Final beat restates the objective; after accept, Tracked Mission only reinforces that step — no replaying old dialogue branches.
- Chapter dialogue expanded with character-specific lines drawn from the campaign bible.

## 2.5.13 - 2026-09-22

- Select / command deck top-right is a clearer third-person orbit of your ship in local space (closer camera, larger inset, station or planet backdrop).

## 2.5.12 - 2026-09-22

- Local wanted level under the system name reads as `Wanted n/5` (red when active, dim when clear) so police pressure is obvious at a glance.

## 2.5.11 - 2026-09-22

- Tracked mission cue sits flush on the top-right header (1-column inset), past the danger badge, with enough room to show the full short guide.

## 2.5.10 - 2026-09-22

- Hold Start redistributes SYS/ENG/WEP on the existing cockpit meters only (L/R select bank, U/D move power) — no separate bottom-right panel; input regressions cover the chord.

## 2.5.9 - 2026-09-22

- Double-tap L hard-brake and hull heat (speed / boost / sun → cool when clear; critical locks boost; max destroys) covered by regressions; controls docs updated.
- Targeted ships show compact HULL/SHLD bars on the lower-left target panel; freighters stay tough, return fire when damaged, and raise a heavier local warrant when destroyed.
- Engine exhaust plumes anchor to each mesh’s aft tip instead of floating behind the silhouette.
- Station deck walk is on the command deck (FLY → Walk station deck): talk to concourse NPCs with X. Planetary surfaces tint from each world’s colours; EVA gets a clearer top strip.

## 2.5.8 - 2026-09-22

- Each system uses a distinct orbital template, world-type permutation and colour set so planets look different from star to star.
- Ship, debris and anomaly placement is system-seeded; hyperspace arrival is farther from the hub on a unique bearing.

## 2.5.7 - 2026-09-22

- Radio page looks like a chassis with speaker grille, frequency dial (OFF + stations 1–5), and needle; retune plays audible static; Triangle / left-of-1 turns radio off.
- SHIPS scanner band lists every ship again; ENEMIES still lists only hostiles.

## 2.5.6 - 2026-09-22

- Combat warnings show a compact **RED ALERT** strip at the bottom of the canopy so speech at the top stays readable.
- Hold Square + Left/Right now includes an **ENEMIES** scanner band listing pirates and anyone currently hostile.
- Confirmed Galacticnet Spacebook/Messages order with parody logo, and Discovery Codex Systems + Planets (worlds unlocked by visiting a system).

## 2.5.5 - 2026-09-22

- Speaker labels (`KEI SAYS`, `IONA SAYS`, flight chatter, Law stops) now sit on a colour chip matching the speaker.
- Tracked Mission briefings show the active speaker's face beside the dialogue bubble for saga chapters and Guild assignments.
- Outfitting detail pane explains tech gates in plain language: in stock, already fitted, or hub tech too low with the needed level and “warp to a richer system.”
- Galacticnet tab order is News / Market / Wanted / Spacebook / Messages / Jobs; Spacebook and Messages show a Facebook-parody logo.
- Discovery Codex adds Systems (visited) and Planets (worlds discovered by visiting a system).
- Radio page is a tuner dial with OFF + stations 1–5, static while retuning, and Triangle power off.
- Each system rotates planet types/colours and traffic layout; hyperspace drops you farther from the hub on a system-unique bearing.
- Double-tap L hard-brakes when fast; heat rises from speed, boost and sun proximity — critical heat locks boost, max heat destroys the ship.
- Hold Start adjusts SYS/ENG/WEP on the existing cockpit meters (no separate overlay). Mission cue is right-aligned; wanted level shows under the system name; Select deck preview is a third-person ship view.
- Tracked story briefings are longer three-beat conversations; Circle is locked until the player finishes and accepts the next step (prologue Begin included).

## 2.5.4 - 2026-09-22

- Fixed black screen after PSP sleep/standby (especially long sleeps): freeze MP3 I/O on suspend, then fully rebuild display, controls and radio on resume.
- Switched framebuffer presentation to `NEXTFRAME` after vblank for more reliable hardware display recovery.
- Shortened audio teardown timeout and terminate-delete the worker if it is stuck on Memory Stick wake.
- Smoke exercises a double suspend/resume recover path; physical PSP confirmation still required.

## 2.5.3 - 2026-09-22

- Saved the final galaxy-map route goal separately from the immediate hyperspace hop (save format 10; older commanders still import).
- After each jump, the next hop toward that saved goal is refreshed automatically until arrival clears it.
- Nearby/galaxy UI shows `FINAL` / route-goal labels for manual multi-jump plans as well as tracked story destinations.
- Added journey/input regressions for plot, save/load, intermediate advance and arrival clear.

## 2.5.2 - 2026-09-22

- Added user-supplied radio station folders and shuffled MP3 playback on PSP.
- Improved MP3 playback stability and PSP suspend/resume recovery.
- Kept generated radio audio as a fallback when a station has no compatible tracks.
- Added mission availability checks so objectives point to stations that can actually offer the required contract.
- Expanded galaxy navigation planning so distant destinations can be located and routed through intermediate systems.
- Preserved the current UI, mission, faction, targeting, dialogue, art, and gameplay work documented in `README.md` and `CLAUDE-HANDOFF.md`.

## 2.5.1 and earlier

The detailed development history, controls, implemented feature list, validation notes, and future design plan are retained in `README.md`, `docs/`, and the Git commit history.
## 2.5.51 - Station Welcome mission

- Added an automatic, session-local Station Welcome mission after the opening campaign is complete.
- The mission plots a route from Lave to Reorte, then guides the commander to disembark, walk from Arrivals to **THE SECOND SHIFT**, and talk to **Lysa Kest**.
- Added concise tracked-mission objectives and cockpit cues (`JUMP: REORTE`, `WALK: CANTEEN`, `TALK: LYSA KEST`) so the authored station art and bar features are easy to find.
- The tour reuses existing station rooms, contacts, speech choices, and bar logic; it adds no save-format fields, rewards, or economy changes.
## 2.5.52 - Second Shift route correction

- The Station Welcome mission now advances only after docking at Reorte’s primary Hub H0, which is the station identity that owns the lush authored Second Shift scene.
- Secondary Reorte hubs now explain that the commander must use the primary Hub instead of silently advancing into the generic canteen renderer.
- Kept generic canteen rooms separate until their own native 480×272 art families pass the palette, payload, contrast, and traversal gates.
## 2.5.53 - Optional Station Welcome

- The bar tour no longer overrides the opening story or forces a Reorte jump. It becomes available after the opening campaign is complete and can then be followed from the tracked-mission screen.
- Reorte’s authored Second Shift scene remains restricted to the primary Hub H0 identity, with secondary hubs clearly identified instead of falsely advancing the tour.
## 2.5.54 - Compact target status card

- Stacked targeted ship hull and shield meters into separate compact rows in the lower-left target card.
- Kept the target distance, radar, power banks, and footer clear at the PSP’s native resolution.
## 2.5.55 - Dismissible conversations

- Circle now backs out of campaign conversations without advancing or losing the current dialogue position.
- Select can open the mission log while a conversation is paused, so players can return to what they were doing.
- Updated conversation footers to show the real available controls instead of claiming Circle and Select were locked.
## 2.5.56 - Outfitting loadout redesign
- Replaced the flat equipment list with a three-panel PSP-native layout: current loadout, available modules, and a focused module card.
- Added clear slot highlighting, fit/replacement state, page position, tech level, price, balance, and compact controls while preserving existing buy, fit, sell, and refuel behavior.
## 2.5.57 - Visual ship loadout
- Replaced the flat ship inventory list with a PSP-native pixel-art hull diagram tied to the selected ship mesh identity.
- Added six overlaid equipment markers, fitted/empty slot states, module summary, cargo, missiles, and capacity readouts while preserving removal behavior.
## 2.5.58 - Clearer command deck navigation
- GalacticNet now lives under Discovery with its own tab destination.
- Mission Log separates main missions from side work at a glance.
- Targeting Computer gains a compact CRT monitor bezel and status sticker while retaining the scanline display.
## 2.5.59 - New player hulls
- Added Fer-de-Lance, Krait, and Ophidian as purchasable player ships using existing detailed ship meshes, with distinct speed, range, capacity, and price profiles.
## 2.5.60 - Crew-decaled targeting monitor
- Added native PSP bezel decals to the full-screen targeting computer: peace symbol, heart sticker, and a small crew marker on the physical monitor frame.
## 2.5.61 - Lave acceptance visual slice
- Added an animated native-pixel targeting scope signature to the monitor-themed targeting computer.
- Added a compact stamped hull readout for Ship Loadout while preserving the fitted-slot inventory view.
- Began the Lave acceptance audit across station, flight, mission, planetary, audio, and QA flows; runtime certification remains dependent on the smoke harness report.
## 2.5.62 - Lave showcase visual pass
- Added a Lave-specific public-canteen identity treatment: arch, LAVE PUBLIC sign, and station seal, while preserving generic room geometry and all existing hotspots.
- Added the staged v0.5–v0.7 release plan and evidence gates to the repository.
## 2.5.63 - Grounded planetary ship presentation
- Reduced the close surface ship wireframe scale so it reads as a parked ship instead of a debug overlay.
- Added a restrained native contact shadow beneath the hull while preserving the existing mesh, landing, EVA, boarding, and takeoff contracts.
