# ELITE: NEXT — interface, identity and presentation specification

## 2.0.1 flight layout authority

This section overrides earlier flight coordinates below. At 480x272, rows 0–23 hold system/heading/danger/objective; rows 24–55 are optional two-line captions with a native-size portrait. Rows 56–191 are reserved for the world, reticle and lock brackets. Rows 192–261 hold the target card, plan radar and telemetry. Rows 262–271 hold button hints. Paused approach/police decisions may occupy the centre.

Target names wrap on spaces at 18 characters per line, with two lines. Full names remain available in Contacts. Computer captions use the reserved height without a portrait. No tails or hide-message text extend beyond the caption. Comms uses eight rows per page. Text is drawn after scene effects.

Radar axes are ship-relative: front up, rear down, left/right match the ship. Radial distance is compressed and all bearings remain visible. Selected contacts have white brackets; hubs use squares, bodies diamonds, ships faction colours. Vertical stems indicate elevation. Surface radar uses local sites and the parked ship.

New art must retain its silhouette at native resolution. HUD icons are authored as 8x8 masks and surface icons as 16x16 masks in hud-pixels.h. No smooth resampling. Sun corona/ring radii match the projected body. Light glints are clipped to the scene. High contrast bypasses flare and vignette. Viewport edges are exclusive in preview rasterization.


Revision 2, 21 September 2026. Supplements DESIGN-BIBLE.md. The latest user direction is a restrained late-1980s science-fiction interface, with readable low-resolution text, adult alien characters, atmospheric music and a little neon. This document separates implemented behaviour from remaining acceptance work. It does not claim every future interface is complete.

1.8.1 affordance rule: action strips pair the plain-English label with a tiny
PSP glyph, so a player can recognise the physical button without decoding a
color alone. This follows the PSP system button vocabulary and accessibility
guidance to make interactive elements visibly distinct and keep high-contrast
text alongside symbols. References: [PSP button help](https://manuals.playstation.net/document/en/psp/current/network/browser/keyassign.html), [Game Accessibility Guidelines](https://gameaccessibilityguidelines.com/full-list/).

1.7.2 flight rules: planets are approached through an explicit soft boundary. The cockpit must show the selected body, stop outside its surface, and offer X to land or Circle to reverse. Gas giants and suns must explain why entry is unavailable; only suns can still inflict collision damage. System maps and flight use the same seeded body positions, with per-system phase, tilt, scale and traffic density so quiet systems feel quiet and busy hubs feel active.

## 1. What this release fixes

The 1.6.0 Controls capture showed tightly packed instructions of equal emphasis. The old deck source mixed sixteen services over two list pages, reset focus on return, and exposed new features mostly through shortcuts. The faction screen truncated descriptive sentences. The portrait picker selected a face from the system seed without using the faction. Comms called the dockmaster a police officer, generic explorers could speak through Kei, and an undocked station channel disclosed market recommendations.

1.7.0 implements the five-category deck, visible entries for Campaign/Guild/Radio/Display, remembered category focus, return-to-owner navigation, four Controls pages, dedicated menu feedback, explicit faction artwork, corrected speaker identities, undocked market-tip hiding and a richer music mix. These are the current increment; the per-screen backlog below remains real work.

Capture flow used for review:

1. Open Controls on the old build; inspect the two dense columns at 480x272.
2. Open each new deck category; check selection, description, system and available service.
3. Enter Shipyard and return; verify the same Shipyard row stays selected.
4. Open Display → Audio, then return; verify Display and its Audio row return.
5. Read Controls, Factions and the campaign; inspect hierarchy, portraits and complete sentences.

Evidence lives in the release validation folder. Screenshot review cannot establish physical PSP readability, learning time, colour-vision accessibility, hearing comfort or screen-narration support. No new-player usability study is claimed.

## 2. Research and decisions

[Xbox Accessibility Guideline 112](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines/112) recommends clear, consistent navigation and predictable controls. Here this means a stable X/Open and Circle/Back pair, one focus, and an action strip in one place. This is an adaptation to PSP controls, not Xbox compliance certification.

[Game Accessibility Guidelines](https://gameaccessibilityguidelines.com/full-list/) recommends readable text and simple formatting. We apply that through shorter pages, complete action labels and whitespace. A reflowed larger-font mode is still planned; merely making the existing canvas larger would not solve the problem.

[Chris Foss's official archive](https://chrisfossart.com/) documents his science-fiction cover work, including Asimov. [JPL's Ken Hodges mission-art archive](https://www.jpl.nasa.gov/images/slice-of-history-titan-saturn-mission-artwork-1976/) supplies period space-design context. Our art direction takes broad period cues: substantial equipment, restrained lighting and functional silhouettes. Generated characters are original, not copies of those artists' works or existing anime characters.

## 3. Native screen contract

- Render at 480x272; framebuffer stride is 512. Never use the stride as the visible width.
- Header occupies y=0–21. The approved ELITE: NEXT logo stays at the left. Page title occupies the rest, never a second competing logo.
- Deck tabs: x=8 + 94*n, y=30, width 90, height 20. Five tabs always visible.
- Deck list: x=8, y=58, width 222, height 132. Preview/details: x=238, y=58, width 234, height 132.
- Deck context row: y=200. System, credits and Docked/Paused have distinct positions.
- Ordinary menu feedback uses y=192–219. Deck feedback uses y=216–243. Campaign feedback uses y=224–247. It must never replace an action button or hide a confirmation choice.
- Footer occupies y=248–271. Instructions start at x=8. Fit a maximum of 58 current-font characters, preferably fewer. Do not rely on a truncated footer to teach an action.
- Text uses the existing crisp 8x8 grid. A line can contain fewer glyph pixels than eight; the grid size is not a claim of accessible cap height. Critical reading still needs hardware review.
- Normal service rows have at least 16 px pitch. Controls uses 24 px pitch and six bindings per page.
- At x=248, a line may use at most 27 characters before the right margin. Rewrite text before truncating it. Proper names may be shortened in a list only if their detail view shows the complete identity.
- Focus combines a filled row and contrasting edge; colour alone must not communicate selection. Disabled services remain visible with an explanation in the detail pane.
- Subheadings describe the block beneath them. Avoid repeated all-caps messages competing with the selected action.
- The live deck preview is decorative context. It cannot obscure list text or alter simulation time.

## 4. Navigation and focus

The deck stores stable service IDs. Visual ordering is separate from simulation APIs and tutorial hooks. Left/Right changes category; Up/Down follows the visible list. Each category remembers its row for this session. Circle returns to the owning menu where possible; returning to the deck restores the selected service. Returning to flight clears the menu path and rearms fire only after release. Navigation never starts a purchase or a mission.

| Category | Visible services, in order |
|---|---|
| Fly | Launch/Resume, Contacts, System details, Galaxy map, Comms/docking |
| Ship | Cargo/market, Shipyard, Outfitting |
| Work | Story campaign, Guild assignments, Mission board, Mission log, Factions, GalacticNet |
| Discover | Discovery Codex |
| Commander | Save/status, Controls, Radio/audio, Display/chatter, Debug |

Discover deliberately has one real service. Do not add empty Photograph, Fleet or Base doors to make the tab look busy. Add each when its complete gameplay loop exists.

Existing Square/Campaign, Triangle/Guild and Select/Radio deck shortcuts remain valid, but every corresponding destination is also a visible row. Help describes the ordinary route first. Hold-Triangle Comms and flight chords remain optional fast paths.

Station-only services cannot open in flight. The failure leaves focus in place and says to dock. Ordinary reading freezes job clocks. Jobs do not expire while comparing descriptions or listening to dialogue.

The current menu path has a bounded eight-entry stack. HOME, FLIGHT and INTRO reset it. Targets retain Circle-to-flight semantics. A future navigation refactor must preserve these exceptions and test cyclic Board/Log routes rather than silently growing the stack.

## 5. Screen inventory and remaining work

| Surface | Purpose and primary action | Current / next acceptance work |
|---|---|---|
| Intro | Begin, load, or skip | Existing branded story. Begin enters Campaign. Never overwrite a commander just for viewing it. |
| Deck | Choose a service | Five categories implemented. Test physical readability and first-time discovery next. |
| Flight | Pilot and use selected object | Existing Full/Minimal/Scenic. Preserve safety modals, locks and rearming. Further central-view clearance remains planned. |
| Contacts / targeting | Identify and lock a contact | Existing category list and lock. Future stable actor generations must prevent replacement ships inheriting old locks. |
| System details | Understand economy, danger and bodies | Existing data. Replace every remaining numeric-only risk label with consistent pips plus words in a future pass. |
| Galaxy map | Inspect and choose a reachable destination | Existing route calculation and sprite previews. Future explicit jump confirmation should show destination, distance and fuel remaining together. |
| Comms | Docking request or recovery | Dockmaster identity corrected; no live goods tips while undocked. Recovery has explicit confirmation. |
| Hold-Triangle panel | Quick channels and view controls | Existing tap/hold separation. No invisible dialogue blocker when chatter is quiet. |
| Cargo/market | Inspect hold; trade when docked | Reserved crates remain protected. Future transaction step must state quantity, total cost, remaining credits and capacity before bulk trades. |
| Shipyard | Compare hulls | Mesh fitting already tested. Future exchange confirmation must explain net cost, cargo compatibility and changed capabilities. |
| Outfitting | Refuel and buy equipment | Existing tech/credit checks. Future rows should separate owned, unavailable-tech and unaffordable states, with one explanation per state. |
| Save/status | Save, load and review commander | V8 CRC and recovery already implemented. Future explicit overwrite/load confirmations must preserve existing backup semantics. |
| Controls | Find one action | Four implemented pages: Flight, Targets/Travel, Stations/Surfaces, Menus/Comfort. L still toggles analog. |
| Display/chatter | Set view and chatter | Implemented visible settings. HUD is session-only; quiet setting uses checked radio.cfg. Larger text and reduced motion are planned, not exposed as fake switches. |
| Radio/audio | Tune and set levels | Five stations, independent levels, mute and save. Future comfort testing should compare speakers/headphones and low volume. |
| Factions | Understand role and expected conduct | Uniform-specific portraits and complete short descriptions. Future species diversity must preserve uniforms and stable character identity. |
| Mission board | Choose paid work | Existing five slots. Future offer detail must show all fee, risk, cargo, deadline and failure terms before acceptance. |
| Mission log | Review and navigate | Existing route/target action. Abandonment confirmation remains planned. Never describe it as implemented until its test exists. |
| GalacticNet | Read fictional local information | Existing tab pages. Editorial portraits now use explicit roles. Institutional posts still need their own insignia rather than generic people. |
| SpaceBook | Read social parody feed | Distinct light social layout remains intentionally different. Kei and named authors use stable identity rules. Local reactions are session-only. |
| Codex | Review discoveries | Existing counters and visited systems. Durable per-object catalogue, naming and filters remain planned. |
| Campaign | Read choice, objective and reward | OC01 implemented. No deadline. Later chapters must declare every objective and recovery path before exposing the chapter. |
| Guild | Opening assignments | Existing legacy progression remains independent of OC01. Future presentation should further distinguish these bonuses from the main story. |
| Police stop | Understand and settle warrant | Existing forced modal and money/custody choice. World and mission clocks remain frozen. Name the system and consequences explicitly in future polish. |
| Planet approach | Enter or turn away | Existing safe turnback. Sun/gas targets must never offer landing. |
| Docking / welcome | Understand automation and arrival | Existing stages and cancel-before-entry. Do not allow menu action to fire a weapon after cinematic return. |
| Death | Recover or restart | OC01 training recovery preserves commander. Other recovery choices remain a future save-flow design task. |
| Debug | Explicit development tools | Keep under Commander, labelled as changing game state. Never disguise debug actions as ordinary rewards. |

Every new screen must add a row here, a control mapping, loading/empty/error/disabled states and a bounded Back path. No claim that all future screens have already been implemented follows from this inventory.

## 6. Character and faction identity contract

Identity is not decoration. A character's name, role, uniform, face, voice caption and conduct must agree. A system seed alone cannot choose a faction uniform. Do not use post index modulo faction count.

| Identity | Required visual and caption | Current mapping |
|---|---|---|
| Traders | Worn civilian cargo clothing; subdued ochre; no police cap | Atlas columns 0, variants 0/4 |
| Law | Navy uniform, peaked cap, small shield badge; blue accent | Atlas column 1, variants 1/5 |
| Pirates | Maroon bandana, patched/rough equipment, eye scar/patch; rust accent | Atlas column 2, variants 2/6 |
| Explorers | Sage survey suit, goggles/headset, compass equipment | Atlas column 3, variants 3/7 |
| Kei Aven | Adult lavender-gray alien woman, pearl-gray hair, subtle antennae, navy survey jacket, gentle expression | Dedicated eight-expression atlas; Campaign, coach, Comms and her SpaceBook post |
| Venn Sorel | Dockmaster, civilian station worker | Fixed trader portrait seed, caption DOCKMASTER VENN; never a Law officer |
| Marshal Iona Renn | Named Law professional | Law portrait on the local social post; use the same identity when authored dialogue arrives |
| Dockhand_77 | Civilian berth worker | Fixed trader identity |
| Ship computer | A system message, not a living explorer | Text caption; no alien face |
| Ordinary hailed ship | Actual faction; generic contact caption | Contact voice metadata prevents explorers from appearing as Kei |

Two generic faces per faction are a current asset limit, not a claim that the galaxy has only two species. The portrait is currently role-aware; it is not a full species/outfit compositor. Population descriptions are world information, not guaranteed descriptions of the illustrated contact.

Before adding named NPCs, create a character record with stable ID, preferred name, faction/job, species, portrait key, palette, allowed expressions, speech tone and mission role. Before reusing a dead NPC slot, assign a new generation. Migration must explicitly retain authored identities; arbitrary slot reuse is still unfinished architecture.

Edge-case rules for future work: unknown contacts receive an Unknown caption rather than an invented named character; disguises require explicit gameplay support; defection changes uniform only at a documented state transition; rescued civilians do not automatically become Guild officers; anonymous agency bulletins should use insignia; a ship's captain and ship chassis are different records; player portraits must not change on system arrival once character selection exists. Audit the target list, dialogue, social feed, mission board, Codex, law modal and reward card whenever an identity changes.

## 7. Art direction

Use late-1980s paperback/OVA/low-resolution-computer mood: functional hardware, restrained expressions, substantial collars, worn materials and selective instrument glow. Adult proportions and narrow, expressive eyes; no mascot faces, candy gradients or exaggerated modern mobile-game smiles. Kei may remain soft and friendly without looking like a child.

Base colours: charcoal, slate, dusty olive, rust, cream and lavender-gray. Accent cyan/amber is reserved for signals and focus. Preserve the approved logo. Preserve SpaceBook's intentionally different parody layout, but maintain readable text contrast. Keep faction differentiation in silhouette and equipment as well as colour.

The discarded bright faction concept is not a shipping reference. Current accepted-direction source files are factions-1988-source.png and kei-1988-source.png. Sources remain separate from native-size outputs. Bake through the repository script using alpha, nearest-neighbour sampling and ARGB1555; inspect the actual 64x64 result, then the actual screen. No runtime PNG loading.

Planets keep stable seed-selected art families across maps and flight. A future visual refresh must not silently change a world's gameplay biome or name. This release retains the 1.6.0 planet families; it does not redo every art asset in the new palette.

## 8. Music and feedback

Music should support long flight: sustained harmony, melody with breathing space, subdued percussion and depth. Current 1.7.0 synthesis has seven voices, gently detuned sustained layers, softened waveforms, extended harmony and bounded stereo feedback taps. Music-only gain is four times the previous synth output before saved volume and ducking. Hardware output and SFX levels are unchanged. Menu clicks no longer duck music; voice and danger retain lighter/stronger ducking respectively.

Keep user mute and levels. Do not silently unmute an existing commander. Tuning fades out, resets the arrangement, then fades in. Never allocate in the audio sample loop. The delay adds 32 KiB and the worker owns its static synth. Test reset determinism, headroom and all station IDs after changes.

Five 32-second pre-master renders were inspected numerically for peaks, DC bias and clipping; these checks do not establish musical taste, headphone comfort or real PSP speaker balance. Listening feedback remains necessary. Future music work should add longer phrase development and additional original pieces, not just higher volume.

## 9. Release gates

Required now: warning-free build; all existing regressions; each service appears once; directional focus follows the visible list; category focus survives return; nested Audio/Controls returns to Display; locked services leave focus and show an explanation; changing HUD affects flight; quiet mode persists; named/generic speaker separation; law/pirate portrait families never alias; native screenshots of all changed screen types; music headroom render; package EBOOT hash matches the tested build.

Required before calling the whole UI finished: physical PSP readability and audio checks, novice launch/target/dock/trade walkthrough, larger-font reflow, full modal/purchase confirmation coverage, motion comfort options, per-object target identities and every per-screen acceptance item above. Update PROGRESS.md with actual results and keep prior EBOOTs. Never replace an unfinished acceptance gate with a claim of perfection.
## Glance-first acceptance

The modern feature boundary and inspiration mapping live in [FEATURE-MAP.md](FEATURE-MAP.md). Minimal HUD is intended for long travel: system/danger, speed, shield, target and critical warnings remain; routine labels can disappear. Scenic mode still keeps safety prompts. Action strips pair a tiny PSP glyph with plain-English text, and focus uses shape/fill as well as color.
# Build 2.0.4 story navigation rule

Story screens must offer a visible highlighted action. Cross selects that action; Circle returns to the previous page and selection. Up/Down selects another visible row. Do not add hidden face-button shortcuts between story, assignments and coaching.

The screen title identifies Story, Optional assignments or Flight guide. Character dialogue explains the situation; the NEXT line and highlighted row name the immediate action. Labels and execution share the same action ID. A claimed reward must become a completion action immediately; an unfinished task must never offer Claim.

At 480x272, the 48x48 portrait is square and separate from dialogue. Main content ends at y=222, feedback uses y=224–247, and the footer uses y=248–271. Footer glyphs replace whole button tokens inline, with their action immediately beside them. Never infer a button from a letter inside a word.
