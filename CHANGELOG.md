# Changelog

All notable playable releases are recorded here. `VERSION` is the current canonical version.

## 2.5.39 - 2026-09-22

- **MacVenture A++ composition polish:** each station room has one hero focal object, three depth planes, and warmer ochre/cream staging (`docs/CINEMATIC-MOCKUP-TARGETS.md` + ART DIRECTOR pixel grammar).
- Side hatch doors replace top chrome door strips so the focal object stays clear; UI chrome thins to cream/cyan hairlines.
- Fewer decorative frames (no checker floors / lamp rows); SHIP return and talk/shop/gift/taxi unchanged.

## 2.5.38 - 2026-09-22

- Soft-FB Wave A canopy FX on the unified tip: denser engine plumes, boost heat shimmer, hit sparks and explosion embers (`space-fx.h`).
- Cheap fixed pools, canopy-clipped; high contrast / warp / dock mute decorative sparks.
- Station art kit re-baked against the unify tip. No crawl or story rewrites.

## 2.5.37 - 2026-09-22

- **Unified playable tip:** MacVenture station deck (art-kit soft-FB rooms, clear SHIP return, talk/shop/gift/taxi) plus Open Channel Act I–II eight-beat page scripts with choice blurbs and locked codas (Ch.02–13 through Carry Home).
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
