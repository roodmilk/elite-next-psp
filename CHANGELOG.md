# Changelog

All notable playable releases are recorded here. `VERSION` is the current canonical version.

## Unreleased — Systems/QA V13 bounds hardening

## Unreleased — Compile-time equipment content pack

- Outfitting definitions now use one validated table for names, descriptions, costs, tech gates, and economy bands.
- Catalog IDs remain stable for V13 commander saves.

## Unreleased — Runtime performance telemetry

- Smoke performance reports now include minimum free heap and largest contiguous free block samples.
- Existing frame-rate and slow-frame checks remain unchanged.

## Unreleased — Selective GU acceleration

- PSP GU now accelerates full-buffer menu/background clears only; the software renderer remains authoritative for all scene pixels.
- GU state is rebuilt across suspend/resume and falls back to the existing CPU clear if initialization fails.

## Unreleased — Fixed-font localization

- Added ASCII-safe English, French, and German UI labels selected by `language.cfg`.
- The existing 8×8 bitmap font remains the renderer, preserving native resolution and memory bounds.

## Unreleased — Opt-in ad-hoc convoy transport

- Added a `convoy.flag`-gated PSP Adhoc transport foundation with bounded peer snapshots and stale-peer expiry.
- Added packet validation regression coverage without requiring a second PSP or network during smoke tests.
- Convoy pairing and real-hardware radio/suspend behavior remain physical-PSP validation items.

## Unreleased — Optional validated content pack

- Added an optional `content.pack` archive for bounded ASCII UI label overrides; absent or malformed packs leave built-in localization unchanged.
- Pack loading is atomic, length-limited and smoke-tested without changing commander save formats.

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
