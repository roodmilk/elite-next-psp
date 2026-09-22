# Design-bible implementation ledger

## ELITE: NEXT 2.0.5 — priority stability and native-resolution audit

- Audited 26 sequential runtime states plus dedicated story and flight captures at 480x272. Reviewed menus, docking, freighters, combat warnings, targeting, planet flight and EVA for clipping and panel overlap.
- Replaced clipped outfitting list strings with purpose-written short labels and widened its detail copy to the actual panel capacity.
- Clarified SpaceBook's comment/tab footer.
- Full automated regression covers save recovery and migration, docking from multiple approaches, atmosphere entry, landing, EVA, takeoff, warp, death recovery, campaign state, missions, economy, mining and freight.
- Physical PSP controls, display response, audio output and long-session battery/thermal behaviour remain unverified.

## ELITE: NEXT 2.0.4 — story navigation and truthful button hints

- Story, optional assignments and the flight guide are explicitly different screens. All use Up/Down, Cross to select the highlighted row, and Circle to return.
- The primary action is computed once and shared by its label and handler. Accept, launch/resume, route to Lave, docking, claim and completion states no longer reuse an unconditional reward footer.
- Assignment actions choose the matching available offer, track an already accepted contract, find an unscanned signal or explain that another system is needed. Completed assignments offer a return to the menu.
- Removed hidden story/Guild/radio shortcuts from the main deck; their visible service entries remain. Removed Triangle acceptance and the unexplained Triangle-to-Guild switch.
- Footer icons parse whole button tokens and sit beside their labels. Ordinary words such as NEXT and STORY no longer create button icons.
- Dialogue, choices and feedback occupy separate bands; Kei uses a square 48-pixel portrait. Native captures cover opening, accepted, flight, reward, paid, assignment, earned and completed states plus the guide.
- Campaign input tests exercise every primary-action transition, one-time rewards, inactive Triangle, controls/back navigation, contract routing and token parsing. Existing saves and campaign progression are unchanged.

## ELITE: NEXT 2.0.2 — mineable belts and scheduled freight

- Replaced decorative-only asteroid belts with 18–28 ordinary rocks and, where seeded, 10–14 ice rocks in a bounded 64-slot object pool. Sparse systems retain three ordinary rocks. Every belt model comes from an actual targetable object.
- Laser hits damage rocks, respect a nearer rock in front of a ship, flash, fracture and release collectible mineral pods. Reusing the rock slot prevents full-pool loss or repeat extraction. Contextual footer shows Cross MINE / Circle COLLECT.
- Added three modular freight hulls. Renderer, broadphase radius and swept oriented collision use shared dimensions; removed huge trailing lines and the radically different close/far models.
- Haulers follow clear routes between distant warp gates and outer hub berths. Each system supports 0–3 freighters, multi-minute arrival opportunities and at least 45 seconds between warp events. Replacements after destruction wait four minutes.
- Cargo manifests are tied to nearby systems and their economies. A completed inbound leg updates local stock once, loads available exports and then leaves. Hails identify cargo and route. Destruction releases the current manifest.
- Extended regression coverage for actual firing, aim misses, blocked collection, full pools, manifests, replenishment timing, hull collision, all-system route clearance and quiet/busy system variation.
- See FREIGHT-2.0.2.md for acceptance evidence and limitations. This does not implement unloaded-galaxy commerce, persistent asteroid depletion, freighter boarding or player-owned freighters.

## ELITE: NEXT 2.0.1 — clear canopy and renderer corrections

- Native frame captures exposed caption overflow, pips over labels, clipped rear radar contacts and effects applied over UI. Replaced the flight layout with reserved top/bottom bands and wrapped target identity.
- Removed floating relay/objective labels and moved routine captions and warnings to the top. Surface card uses local pad/parked ship coordinates.
- Replaced half-plane bearing strip with a 360-degree ship-relative plan radar. Added direction and bounds regression tests.
- Corrected the rasterizer's exclusive preview boundary, odd pixel-block bounds, and shared 2px sampling grid. Added framebuffer assertions that cockpit UI never touches rows 56–191 and preview pixels stay in their rectangle.
- Added hand-authored native HUD and surface-life silhouettes, restrained clipped glints, coloured solar granulation and shaded capital panels. Effects precede text; high contrast omits flare/vignette.
- Comms paginates its 11 actions without overlapping feedback. This distinct version does not replace the 2.0.0 archive.
- Validation evidence lives with the 2.0.1 package. Emulator evidence is not a physical-PSP or zero-defect claim.

# ELITE: NEXT 2.0.0 — design authority and interior-walk slice

- Cockpit readability pass: station target cards now use a compact `STATION HUB` label; center-view target nameplates are removed; ship-computer chatter uses a thin compact box; the bottom radar is now a labelled relative-bearing strip (`REAR / AHEAD / RIGHT`) with contacts plotted by bearing and vertical offset.
- Added a bounded tethered spacewalk slice: lock a nearby anomaly/derelict, hold Triangle, choose Spacewalk salvage, manage oxygen and suit integrity, recover a relay core, then return through the airlock safely. Station concourse Triangle now transfers into the player's personal ship deck.
- Added a colour/livery pass to vector ships: faction-tinted two-tone facets, upper-hull highlights, darker undersides, cockpit pixels, panel stripes and paired engine pixels at native PSP scale. Existing stations retain their wireframe identity with coloured entrance and relay accents.
- Replaced the planning authority with `DESIGN-BIBLE-2.0.md`, an implementation-oriented PSP bible covering the current baseline and the bounded path to interiors, spacewalks, richer planets, factions, missions, audio, UI and ownership systems.
- Added a first-person station concourse prototype reachable from hold-Triangle Comms while docked. It uses bounded movement, a stable low-resolution perspective, named station identity and safe return to the command deck.
- Full smoke suite remains green after the new page and Comms route.

## ELITE: NEXT 1.9.9 — quick Comms and radio control

- Hold Triangle now opens a nine-row quick Comms menu: mute chatter, dismiss the current card, hail the selected contact, clear target, advance the radio station, open the full radio page, request docking, open Guild and cycle HUD layout.
- The menu reuses the existing comms return path and radio settings persistence, keeping fast actions consistent with their full-screen owners.
- Radio remains an original procedural soundtrack with layered alien chatter rather than copied commercial game music.

## ELITE: NEXT 1.9.8 — animated pause vista

- The right-side pause/deck preview now focuses the local or selected planet, uses a slow orbital camera, and renders parallax stars, solar flare and distant relay silhouettes inside the existing clipped preview window.
- The preview restores the live flight camera state after drawing, so it is purely presentational and cannot alter navigation or saves.

## ELITE: NEXT 1.9.7 — display chatter reduction

- The live cockpit no longer injects the opening `Open CONTROLS` coaching sentence into the display view. The instruction remains available in the authored story/help pages where it can be read deliberately.
- The initial Kei voice line now welcomes the commander, reducing the amount of text competing with the first-flight view.

## ELITE: NEXT 1.9.6 — local hubs and telemetry separation

- Every system now exposes three deterministic hub locations: the primary station, an outer relay and a frontier outpost. The existing station contact resolves to the nearest hub, and communicator docking at either secondary hub is immediate and service-safe.
- Secondary hubs render as scaled orbital structures with relay labels, so the extra destinations are visible before the player parks.
- The bottom-right telemetry panel now reserves a flat power strip above the status bars, removing the decorative-circle overlap reported on PSP-sized displays.
- Added a deterministic game assertion covering secondary relay docking.

## ELITE: NEXT 1.9.5 — cockpit text reduction

- Routine target telemetry was reduced from five lines to a compact identity plus range/altitude readout.
- Detailed category, hull and orientation data remain available in the existing Square target view, so information is deferred rather than removed.
- The change follows the design-bible glance-first rule and keeps warnings, objective beacons and safety prompts visible.

## ELITE: NEXT 1.9.4 — button glyph layout fix

- Footer glyphs now occupy the upper strip of the 24-pixel footer, while footer text uses the lower baseline.
- Cockpit action glyphs now occupy a dedicated lane above the action hint. This removes the overlap that could make button prompts look corrupted on the PSP-sized framebuffer.

## ELITE: NEXT 1.9.3 — believable contact identities

- NPC scanner labels now use deterministic faction callsigns: `RAIDER`, `MERCHANT` and `GUILD` identities make traffic feel authored and persistent across a flight.
- Law ships intentionally report `ENCRYPTED // XX`, preserving their institutional identity while hiding personal callsigns as requested.
- Callsigns are derived from stable system and contact IDs, so they remain allocation-free and consistent across menus, target cards and world nameplates.

## ELITE: NEXT 1.9.2 — world-readable targets

- Selected and looked-at bodies, stations and ships now receive compact in-world nameplates with category and identity color. The plate is clamped to the flight view and disappears in scenic HUD mode.
- The cockpit header now explicitly prefixes the current system with `SYSTEM:` so local names cannot be mistaken for planet or station names.

## ELITE: NEXT 1.9.1 — objective beacon

- The cockpit now points toward the active story or mission target with a compact directional arrow, `NEXT` label and target name. It respects minimal/scenic HUD modes and disappears during docking, warp, custody and death states.
- This closes a player-audit gap: objectives are now stated and spatially pointed, rather than relying on text alone.

## ELITE: NEXT 1.9.0 — planetary identity pass

- Gas giants now receive deterministic tilted rings with sparse animated highlights. Ring scale, tilt and phase come from the body seed, so charts and flight remain consistent without consuming gameplay RNG.
- The effect is line-based and bounded, preserving the low-resolution aesthetic and the existing performance budget.

## ELITE: NEXT 1.8.9 — living traffic and starfield

- Nearby ships now emit faction-colored engine glows, with longer animated wakes for capital freighters. The effect is a view-space accent over the existing meshes, so it adds readability without another entity pass.
- A deterministic subset of stars twinkles slowly using the existing frame clock. The starfield remains fixed in position and does not consume gameplay RNG.
- This is the next release-hardening pass toward a lively but restrained PSP presentation.

## ELITE: NEXT 1.8.8 — celestial post-processing

- Suns now receive layered halo rings, cross-star highlights and animated aperture ghosts. Planets receive a restrained atmospheric rim and moving surface glint; stations gain a pulsing cyan/gold entrance beacon.
- The pass is deliberately pixel-native rather than a full-screen blur: it keeps the 1980s wireframe identity, avoids allocations and renders before warnings, target locks and telemetry.
- Full PSP smoke validation remains the release gate for the visual pass.

## ELITE: NEXT 1.8.7 — target focus pass

- Target reticles now inherit the selected ship’s faction color and gain animated corner marks while auto-aim is active. This makes law, pirate, trader and explorer contacts readable in the world view without opening a menu.
- Planet approach prompts now pair native PSP glyphs with direct actions and a gas-giant explanation, reducing the chance of an accidental landing expectation.
- All changes remain render-only and preserve the existing save format.

## ELITE: NEXT 1.8.6 — readable comms and living portraits

- Flight chatter now reads as a speech bubble with a faction-colored tail, speaker band and a visible Triangle hide-chatter hint. It preserves the existing compact text wrapping and does not cover telemetry.
- Small faction portraits gain restrained animated identity lights. These are rendered on the native pixel grid so the portraits remain crisp in 32-pixel cards.
- The pass keeps the low-resolution 1980s presentation while improving scanability and speaker recognition.

## ELITE: NEXT 1.8.5 — live system activity briefing

- Distant faction combat now produces rate-limited scanner incidents for law engagements and trader convoy attacks. The event uses the existing warning/message band and alert cue, so it remains legible on PSP without a permanent extra HUD panel.
- System Details now doubles as a live hub briefing: it shows current trader, law, pirate and explorer traffic, danger and prosperity ratings, and a short activity readout before the player locks a destination.
- The update preserves the existing save format and passed the complete game, input, steering, radio and performance smoke suite.

## ELITE: NEXT 1.8.4 — high-contrast focus mode

- Display & Chatter now exposes High Contrast Focus. It increases selected-row fill contrast without recoloring faction meanings or removing the plain-text labels.
- The setting is session-local, like HUD layout, so commander saves remain compatible.

## ELITE: NEXT 1.8.3 — contextual cockpit affordances

- The flight action strip now mirrors the menu footer with tiny PSP button glyphs, selected from the current state (story, approach, police or normal flight).
- Plain-English labels remain unchanged, so the glyphs reinforce recognition instead of replacing readable instructions.

## ELITE: NEXT 1.8.2 — living-universe feature map

- Added `FEATURE-MAP.md`, a concrete crosswalk from modern space-game loops to the PSP implementation and its memory/performance boundaries.
- Codified glance-first HUD acceptance so future feature additions cannot turn the cockpit into an unreadable wall of telemetry.

## ELITE: NEXT 1.8.1 — button affordances and cockpit post-processing

- Footer prompts now reinforce X, Circle, Triangle and Square with tiny color-coded glyphs, following PSP button conventions while keeping text for clarity.
- Cockpit post-processing adds a sparse moving CRT dust layer outside the warning and telemetry bands.
- The pass follows the PSP button reference and accessibility guidance: consistent symbols, visible focus/affordance, plain language and high contrast remain paired rather than color-only.

## ELITE: NEXT 1.8.0 — flight motion effects

- Added an animated engine flare to normal and boosted flight. It uses a compact tapered pixel plume, speed-scaled length and pulse, with no allocations or extra meshes.
- The plume is rendered before target/HUD overlays, so it adds motion without covering warnings, objectives or telemetry.

## ELITE: NEXT 1.7.9 — risk/reward contract loop

- Contract rewards now include a deterministic danger premium based on the destination system and job type. The accepted job stores the computed payout, so the offer cannot change underneath the player.
- The mission board adds a compact risk column and selected-offer risk summary, making the tradeoff legible before the player commits cargo, time or legal exposure.

## ELITE: NEXT 1.7.8 — safe mission actions

- Mission Log abandonment now uses an explicit confirmation state. The selected row shows `ABANDON? X CONFIRM / O CANCEL`; accidental Triangle presses cannot discard a job.
- The confirmation is UI-only and does not advance mission clocks, alter save data or change cargo until X confirms.

## ELITE: NEXT 1.7.7 — idiot-proof story guidance

- The active story door is marked with a gold `!` on the command deck, the right panel shows the next action, and the cockpit labels the story as `STORY NEXT` with the current chapter title.
- While the guided story is active, the flight action strip shows the concrete next step (for example, “Tap Square, or Square and Left”) instead of generic chatter controls.

## ELITE: NEXT 1.7.6 — portrait framing pass

- Added a shared aspect-preserving portrait fit helper and routed every faction/Kei card through it. Non-square dialogue, status, faction and SpaceBook boxes now letterbox the square source tile instead of distorting it.
- Small cards use the native 32×32 portrait atlas with integer nearest-neighbour sampling, matching the PSP's low-resolution aesthetic.

## ELITE: NEXT 1.7.5 — A++ release audit

- Added `A-PLUS-AUDIT.md`, a cross-system acceptance matrix tying audio, gameplay, interface, graphics, persistence, memory and delivery claims to concrete gates.
- The audit keeps streamed CC0 playback, later OC chapters, surface traversal, Codex depth, larger-text reflow and physical PSP certification visible as remaining work instead of silently claiming completion.

## ELITE: NEXT 1.7.4 — authored repeatable contracts

- Repeatable mission offers now expose a deterministic short brief derived from mission type, destination, economy and danger. Cargo, bounty, exploration, rescue and covert work each communicate a different reason to leave the hub.
- The selected offer owns the only extra description line, preserving the low-resolution hierarchy and preventing the mission board from becoming another wall of text.

## ELITE: NEXT 1.7.3 — radio atmosphere and lock feedback

- Far Horizons layers gated, deliberately unintelligible alien chatter into the existing integer radio synth with no extra buffers or runtime decoding.
- Added `assets/audio/CC0-SOURCES.md`, documenting three CC0 space tracks selected from OpenGameArt for the future streamed-radio decoder pass.
- Target locks now pulse as a small pixel reticle in flight, giving immediate visual confirmation while preserving the low-resolution cockpit.

## ELITE: NEXT 1.7.2 — natural systems and safer planetary flight

- Planet bodies now form a soft navigation boundary: entering a planet’s perimeter stops the ship, never damages the hull, and opens an explicit “X to land / Circle to reverse” choice. Gas giants explain that landing is unavailable while preserving the same reverse-away flow; suns retain hazardous collision behaviour.
- Solar-system body layouts use deterministic per-system orbital phase, scale, tilt and vertical offsets, so charts and flight scenes no longer share one repeated arrangement. Traffic budgets also vary by system seed and local government/economy, creating quiet frontier systems alongside busy hubs while preserving Lave’s welcoming activity.
- The cockpit danger rating is now a compact five-star pixel badge. Bottom-right power/status indicators have been separated from the fuel, heat, shield and speed bars so their pip markers cannot overlap the telemetry.

Validation: warning-free PSP build; game, input, steering, radio and performance smoke checks all pass. Physical PSP certification remains outstanding.

## ELITE: NEXT 1.7.0 — clearer interface, faction identity and music

- Five deck categories with twenty visible service entries, remembered focus and return-to-owner menus. Campaign, Guild, Radio and Display no longer require discovery of shortcuts.
- Four shorter Controls pages, a visible HUD/chatter settings page and reserved action-feedback bands. Routine character chatter no longer covers ordinary menus. Faction descriptions now fit as complete sentences.
- Restrained late-1980s faction portraits: distinct trader, police, pirate and explorer uniforms. Kei uses a separate muted adult alien portrait. Venn is consistently the dockmaster; generic explorer hails do not impersonate Kei; the computer has no alien portrait; SpaceBook authors use explicit faction identities.
- Station Comms no longer discloses live buy/sell recommendations while undocked.
- Seven-voice music with softer attacks, extended harmony, detuned layers and filtered stereo delay. Music-only gain increased; menu clicks no longer duck it. Existing volume/mute preferences are kept. Five 32-second renders passed headroom/DC checks; subjective listening and physical PSP audio checks remain outstanding.
- UI-SPEC.md is the detailed interface and identity authority, with research references, layouts, per-screen inventory, edge cases and unfinished acceptance work. Larger-text reflow, further purchase/abandon confirmations, stable NPC generations and richer institutional avatars remain planned.

Validation: warning-free PSP build; 192 game, 97 input, 13 steering and 15 radio assertions (317 total), all passing. PPSSPP smoke: 59.94 FPS average, 17.33 ms worst frame, no frames over 25 ms. Nine changed screen types inspected at native resolution, with the corrected faction screen recaptured. No physical PSP certification. V8 commander format and V2 radio settings unchanged from 1.6.0; no save reset required.


## ELITE: NEXT 1.6.0 — first authored chapter and world art

Completed and tested:
- OC01 has its own campaign screen, two authored Kei dialogue choices, explicit acceptance at Lave, hub lock / 600 m flight tracking, actual manual or guided arrival, a report action and a one-time 100-unit harbour badge reward. New players enter it from the intro; Square on the deck or Guild screen reopens it.
- Legitimate docking after launch also completes the chapter if the compass lesson was skipped. The debrief explains the compass. Tows, custody and remote-system arrivals cannot emit the authored completion event.
- Training death in Lave offers Start recovery, retaining the existing commander, cargo, jobs, credits and warrants, restoring launch fuel and resetting the flight objective. This is hub recovery, not an in-flight autosave checkpoint. No recovery reward is granted.
- Legacy four Guild assignments and their rewards remain separate. OC02–OC12 are still planned; this is not the complete OC01–OC04 milestone. The transition table currently implements OC01 only, not a general branching campaign interpreter.
- Save V8 preserves the legacy core payload and adds an explicitly little-endian campaign extension plus CRC32 over all payload bytes. Exact file length is checked. V1–V7 migration, backup recovery and checked replacement remain. Full portable serialization of the older core is still outstanding. V8 files are not readable by old builds; keep a copy of pre-upgrade saves.
- Kei is now an adult female alien with a soft lavender palette, white hair, antennae and a friendly expression. Eight original generated 64x64 expressions are compiled; campaign, coach and Comms use the new identity, including mouth animation in Comms.
- Every non-sun body uses seeded sprite art in flight, charts, Codex and details. Eight base planet designs combine with stable mirrored/palette variants; these are shared art families, not 1,024 separately painted assets. Body identities and gameplay types are unchanged. Suns retain their existing renderer. Ground biomes are not expanded in this release.
- Added art uses 131,072 bytes of static ARGB1555 data, bringing generated portrait/item/logo/planet arrays to 287,248 bytes. PNGs are source assets only; runtime has no image decoding or extra downloads.

Validation: warning-free PSP build; 192 game + 84 input + 13 steering + 15 radio checks = 304 passing assertions; PPSSPP 42-scene performance smoke averaged 59.94 FPS, worst frame 16.98 ms. Native 480x272 campaign, reward and planet-flight captures inspected. Physical PSP tests remain outstanding.

Next: OC02's reachable low-risk route binding, reserved greenhouse-pump delivery, Mara's dialogue, persistent delivery reward and recovery cases. OC03 still requires the listening-post surface interactions described in the bible.


## ELITE: NEXT 1.5.0 — radio, art and interface, 21 September 2026

User steering expanded this release to radio stations, smaller shipyard previews, less text clutter, hold-Triangle Comms, a distinct SpaceBook feed, AI pixel artwork and the ELITE: NEXT name/logo/intro. All of those have runtime integration in this build.

- Five original integer-only procedural stereo arrangements with independent music/effects levels, station fades, ducking, mute and checked settings backup. Audio shutdown joins the worker before releasing its channel.
- Hold-Triangle control panel; quiet chatter preference; persistent tutorial coach removed from ordinary menus; speech headings contained inside their own boxes. Critical feedback remains separate from optional chatter.
- Dedicated SpaceBook social layout with local reaction toggles and fictional reply display. Nothing is sent externally.
- Approved generated logo, eight race portraits and twenty item icons compiled to 156,176 bytes of ARGB1555 static data. Sources and hashes are retained; nearest-neighbour asset compilation is reproducible. Artwork is used in actual screens, not only concept previews. Full character sprite animation sheets remain future work.
- Branded intro with starfield animation, timed short story lines, immediate skip and commander load.
- Fixed shipyard centring/scale; bounding checks cover every playable hull at 256 angle combinations per hull. Rotating previews do not advance gameplay time.
- Save format remains version 7; radio/quiet preferences are separate version-2 settings. The original installation folder remains ELITEA for save continuity.

Final emulator test logs and native screen captures are included under the versioned release validation directory. Remaining design-bible features listed below stay unfinished; this release does not imply the full epic campaign or expansions are complete.

## 1.4.0 — journey and opening Guild slice, 21 September 2026

Packaging cutoff: user requested finishing the current build immediately. Existing procedural music and sound effects remain enabled. Selectable radio stations were started but are not integrated in this release; the unfinished preferences helper is preserved in `work/radio-pending/` outside the game source.

Completed this release:

- Optional coaching instead of tutorial service locks.
- Mission-clock policy shared by game simulation and UI: reading, choices and transitions do not spend contract time.
- Four opening Guild assignments with dialogue, progress indicator, real gameplay completion events, one-time bonus claims and persistent progress. These adapt the first four design objectives into the current systems; they are **not** the complete OC01–OC04 branching scripts or the complete 12-chapter campaign.
- Guild progress uses the three reserved integers in the version-7 story payload; old commanders start at assignment zero. Ordinary commander saving remains explicit. Older executables cannot retain these new fields when saving.
- Mission-log navigation with breadth-first fuel-safe route planning and local target selection. Subsequent hops assume station refuelling. The final contract destination is never overwritten by its intermediate route.
- Mission-crate reservation, surplus-only sales and issued-crate removal on expiry/abandonment.
- Affordable confirmed station recovery with refuel; local warrants persist.
- Exterior docking cancellation; planet-turnback safe clearance; held-confirm fire rearming.
- Help entries for roll, clean HUD, Guild, mission navigation and recovery. Original-setting Meridian echo naming.

Validation: final PSP cross-build, PPSSPP game/input/steering regression reports, 42-scene rendering/performance smoke test, and native 480×272 Guild-screen inspection. See the versioned release's validation folder. No physical PSP validation is claimed.

Still outstanding: remaining P0 architecture/save/checksum/hardware work; five-category deck redesign; stable actor identities; the full authored campaign and choices; durable per-object discovery catalogue; new sprite pipeline/art; richer surfaces; ownership, crafting and all expansion loops. The source already contains earlier basic versions of many world features, but this ledger does not mark their design-bible refinements complete.

Next useful milestone: a data-driven campaign runtime and the complete OC01–OC04 story slice, with specific destinations, actors, branches and recovery paths. Preserve the four objective rewards as legacy introductory assignments during migration. Then implement the remaining chapters and surface expansion gates in order.

## 1.3.9 — first P0 increment, 21 September 2026

The authoritative project specification is [DESIGN-BIBLE.md](DESIGN-BIBLE.md), revision 1.0. Continue from the existing source, not from an empty project. Proposed content in that document is not automatically implemented. The current controls and version-7 save payload remain compatible.

### Completed

- Preserved the previous 1.3.8 EBOOT under `releases/1.3.8/`.
- Added checked save-path construction, flush/close checks and full existing-format validation before commit.
- Replaced delete-first saving with backup rotation and rollback on rename failure. An existing recovery backup is retained when the primary is corrupt or missing.
- Added automatic backup loading; invalid/missing primary files no longer prevent recovery. Temporary files are deliberately not promoted because they may represent an uncommitted transaction.
- Initialized the save header before reading, avoiding undefined reads when a file is truncated.
- Shared station entrance dimensions between visible aperture, glowing outline and collision clearance.
- Routed guided entry through manual station collision checks instead of advancing directly into the arrival stage.
- Rejected inside-to-outside false entry, entrance-edge clipping and guidance requests inside the solid station.
- Added dedicated recovery/docking regression cases, including injected commit and backup rename failures and rotated approaches from both sides.

### Evidence

Build: `build.ps1` with the installed PSP SDK; no compiler warnings in the final build.

PPSSPP reports copied into `releases/1.3.9/validation/`: game, input, steering, performance and boot checks. The 42-scene smoke test reported 59.94 average FPS, 16.73 ms worst frame and no frame over 25 ms. These are short emulator measurements, not physical PSP certification.

### Remaining P0 work

- Physical PSP memory-stick replacement/power-interruption validation. Standard C flush/close plus recovery files does not guarantee storage durability during hardware power loss.
- Version-8 explicit serialization and checksums remain future work; version 7 has structural validation, not corruption detection for every possible byte change.
- Expand docking tests across station variants and add user cancellation/obstruction handling before calling the whole docking design complete. Decorative outer rings still use the existing rendering/collision treatment.
- Planet-turnback edge cases and modal input/rearming audit.
- Canonical stable target IDs and unrestricted orientation work from the design.

### Next bounded release

Finish planet turnback and modal input/time-policy regressions, then proceed to P1 navigation/help improvements. Keep new story content gated behind a tested mission runtime; the twelve-chapter campaign is designed but not yet implemented. Surface expansions, new sprites and ownership systems are also pending.

For every release, update this ledger with actual acceptance results and preserve a working EBOOT. Do not relabel this first increment as all of P0 complete.
