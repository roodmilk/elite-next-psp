# ELITE: NEXT — THE COMPLETE STORY & MISSIONS MANUSCRIPT

**Status:** Living master manuscript (begun 22 September 2026, build 2.5.32+)  
**Story Manager:** narrative authority for tone, cast, missions, and campaign binding  
**Platform:** PSP 480×272 — every scene must remain playable as short spoken beats plus one true objective  
**Rule:** Elite novels, GalNet articles, and tourist-beacon text are **reference only**. All dialogue below is original.

### How to use this book

| Volume | Contents | Binding |
|---|---|---|
| **I** | Galaxy promise, themes, cast, loops | Design bible + playable tone |
| **II** | Mission Network (board jobs) | `src/sectors.h`, mission log |
| **III** | Explorers Guild optional assignments | `src/guild.h` |
| **IV** | The Open Channel (main campaign) | `src/saga.h` + `docs/OPEN-CHANNEL-SCREENPLAY.md` |
| **V** | Living galaxy texture (stations, freight, radio, GalNet) | UI / freight / radio systems |
| **VI** | Postgame, expansion seeds, implementation map | Future slices |

**Companion documents (do not duplicate wholesale):**
- Full Open Channel theatrical scenes → `docs/OPEN-CHANNEL-SCREENPLAY.md`
- Short campaign ops summary → `docs/OPEN-CHANNEL-CAMPAIGN.md`
- Product/tech direction → `docs/DESIGN-BIBLE-2.0.md`
- UI / feature inventory → `docs/UI-SPEC.md`, `docs/FEATURE-MAP.md`

---

# VOLUME I — THE GALAXY AND THE PROMISE

## I.1 Logline (whole game)

In a 256-star pocket of the Old Worlds, an independent commander flies trade lanes that still remember GalCop, takes work that keeps people fed, and answers a Guild channel that will not close — until a missing surveyor's silence points at a corporation selling "safe routes" stolen from living migration songs, and something nonhuman answers the quiet left behind.

## I.2 Product promise (story version)

ELITE: NEXT is not a catalogue of systems. It is a **handheld space novel you fly**.

The player can:
1. Leave a warm berth and come back.
2. Carry something that matters to someone with a name.
3. Listen before shooting.
4. Choose who holds power over routes — and live with the map afterward.
5. Keep flying after the credits, because the galaxy does not end when the channel opens.

## I.3 Themes (non-negotiable)

1. **Routes are power.** Whoever writes the chart writes who lives.
2. **Observation is not ownership.** Scanning is an ethic, not a loot verb.
3. **Institutions fail when records cannot be challenged.** GalCop's seal once meant the lane belonged to everyone.
4. **Dependence is not consent.** Reliable private routes can still be cages.
5. **Home is rarer than jump fuel.** Berths, badges, and return vectors are emotional technology.

**Tone:** working pilots, moral ambiguity, quiet dread, dry humour under pressure.  
**Thargoids:** rare, unknowable — traces and one remote answer, never a parade.  
**Mycoid:** history and evidence only — never a craftable weapon.

## I.4 Setting frame (~3311)

After the Titan war, civilians are tired of saviours. Lave still wears GalCop paint under newer seals. The Federation, Empire, and Alliance offer help that arrives with stationery. Pale Meridian (original) sells clear lanes. The Explorers Guild local cell still answers missing persons as if that were the whole job.

| Public lore (paraphrased) | How we use it |
|---|---|
| Lave / GalCop capital; dissolution 3174 | Venn's archive, berth culture |
| First Thargoid War; INRA mycoid | Mycoid Ledger chapter — evidence, not tech tree |
| Guardians / Aegis institutional memory | Aegis Echo — rescue ships for knowledge |
| Fed / Empire / Alliance styles | Envoy chapters — listen, no forced flag |
| Uncertain deep-space signals | Coldest Signal / Answering Dark |

## I.5 Main cast (voice bible — short form)

### Kei Aven — Guild coordinator
**Want:** Ryn alive and still herself.  
**Need:** Care that does not become control.  
**Speech:** Precise; over-explains when scared; dry humour late.  
**Arc:** Procedure → confrontation → listening without adopting power's language → releasing custody of the channel.

### Ryn Vale — missing surveyor
**Want:** Protect others by vanishing if needed.  
**Need:** Trust a crew with danger.  
**Speech:** Observant, sideways funny; allergic to being a prize.  
**Arc:** Silence → shelter → recurring contact → co-steward of the open channel.

### Venn Calder — Lave traffic / former archivist
**Want:** Order on the pad.  
**Need:** Grief that procedure can no longer fully hide.  
**Speech:** Tower dryness; feelings filed as protocol.

### Iona Marr — local law
**Want:** Evidence over uniforms.  
**Need:** To become the coalition's auditor.  
**Speech:** Legal clarity; "a badge is not evidence."

### Oru Sen — xenobiologist
**Want:** Understand before naming.  
**Need:** Teach restraint as skill.  
**Speech:** Quiet scientific warmth.

### Tamsin Reed — convoy captain
**Want:** Crews alive.  
**Need:** Working-pilot politics without a flag.  
**Speech:** Blunt, practical.

### Nadi Quill & Pip — salvage / drone
**Want:** Recover useful truth.  
**Need:** Humour that keeps hands moving.  
**Speech:** Salvage slang / clipped drone logic.

### Sable Rook — pirate negotiator
**Want:** Honour explicit bargains.  
**Need:** Remain dangerous; trust ≠ harmless.  
**Speech:** Transactional courtesy with teeth.

### Director Cassian Voss — Pale Meridian
**Want:** Private control of reliable routes.  
**Need:** Stand down, testify, or flee — **death never required**.  
**Speech:** Executive calm; cages sold as safety.

### Supporting named contacts
**Mara** — agricultural dock; sealed things and honest lies.  
**Dockhand_77** — bay teasing; local colour.  
**Marshal Iona Renn** — GalNet law voice (may align with Iona Marr identity in play).

## I.6 Player fantasy tracks (story-facing)

| Track | Story flavour | Mission verbs |
|---|---|---|
| Pilot | Come home in one piece | Launch, dock, navigate, survive |
| Surveyor | Listen twice | Scan, Codex, anomaly, migration |
| Operator | Keep promises moving | Deliver, trade, freight hail |
| Citizen | Live with warrants and trust | Law stop, choice chapters, GalNet |
| Builder | Make the ship yours | Outfitting, hull exchange (light narrative) |

## I.7 Core narrative loops

**Moment:** Look → identify → approach / hail / scan / dock / trade / fire / leave.  
**Excursion (2–5 min):** Signal → travel → one skill action → reward or clue → hub or continue.  
**Session (10–30 min):** One or two jobs, a distinct system, spend, log a consequence.  
**Campaign (~10 hr critical path):** Find Ryn → expose Meridian → open the channel → choose custodians.  
**Postgame:** Procedural jobs, trust-coloured helpers, warm berth, unfinished listening.

## I.8 Safety & writing rules for all volumes

1. One **true** next objective on screen — never point at a random contract that may not exist.
2. Ask-then-answer in Story UI (player orange bubble first).
3. PSP wrap: ~46 glyphs × 3 lines for Story speech; radio ≤160 chars.
4. Recovery paths for every essential objective.
5. No missable actors required for campaign progress.
6. Thargoids stay rare; mycoid stays historical.
7. Newest Commander note overrides older bible prose.

---

# VOLUME II — THE MISSION NETWORK

Board jobs are the galaxy's daily novel. They must feel authored even when destinations are seeded.

**Code binding:** five types in `MISSION_*` (`src/game.h` / `src/sectors.h`). Five-slot log. Clocks pause in menus, docking, and dialogue.

## II.1 Mission type overview

| Type | Board name | Fantasy | Core verb | Failure tone |
|---|---|---|---|---|
| 0 | Food delivery | Keep someone fed | Carry crate → dock dest | Hunger, not spectacle |
| 1 | Pirate hunt | Clear a lane | Defeat / drive off marked hostile | Fear that becomes quiet |
| 2 | Exploration scan | Notice the unusual | Scan marked body/anomaly | Wonder without ownership |
| 3 | Pilot rescue | Nobody becomes a number | Hail → recover → hub | Grief if late; relief if on time |
| 4 | Covert delivery | Quiet promises | Carry illicit → dock without fuss | Paranoia, not cool-factor swagger |

## II.2 Patron voice banks (board flavour)

Use these as mission-brief authors and completion radio. Rotate by system seed.

### Food delivery patrons
1. **Clinic quartermaster:** "Power cells and protein packs are not romance. They are whether someone wakes up."
2. **Station school cook:** "If the crate is late, I invent excuses. Children notice inventing."
3. **Tamsin's convoy second:** "We eat when the tender docks. That is the whole religion."
4. **Oru (field note):** "Survey camps forget meals. Bring food so the listening continues."
5. **Anonymous market:** "Margins are thin. The hunger on the far pad is not."

### Pirate hunt patrons
1. **Iona:** "I want a lane you can explain to a civilian. Disable preferred. Corpses make tidy lies."
2. **Venn:** "Raiders on the public approach. Prefer boring pilots to clever wrecks."
3. **Sable (ironic):** "Someone is hunting without a bargain. That offends my professional pride."
4. **Independent hauler:** "They painted us twice. Third time I stop believing in luck."
5. **Guild bulletin:** "Marked hull. Clear it. Do not become the next bulletin."

### Exploration scan patrons
1. **Oru:** "Get close. Press the scan. Leave the road clearer than you found it."
2. **Kei:** "Ryn taught me to listen twice. Second listen is where the map changes."
3. **Codex clerk:** "Untitled anomaly. Title it with evidence, not hope."
4. **Nadi:** "If it sings, record it before scavengers rename it scrap."
5. **Pip:** "Structure unusual. Confidence: enough to bother. Recommendation: soft approach."

### Pilot rescue patrons
1. **Kei:** "A beacon is still a person until we prove otherwise."
2. **Tamsin:** "Count them. Paperwork can wait until the counting finishes."
3. **Venn:** "Lifeboat telemetry is not weather. Bring them to a pad with air."
4. **Iona:** "Rescue first. Statements second. Running from a beacon is its own crime."
5. **Dockhand_77:** "I hate empty seats at the bar. Fill one."

### Covert delivery patrons
1. **Sable:** "Boring lie, sealed crate, no speeches. Speeches are how customs wakes up."
2. **Anonymous:** "If asked, you carry agricultural sensors. Always."
3. **Mara:** "I don't open sealed things. Neither should you."
4. **Meridian whistle (rare):** "They already forged the hazard notice. Don't forge your own courage."
5. **Kei (reluctant):** "I hate quiet crates. I hate louder injustices more."

## II.3 Full brief templates (player-facing)

Each template: **Offer line** (board) → **Accept radio** → **In-flight reminder** → **Complete** → **Fail/expire**.

### II.3.A Food delivery — Template FD-01 "Clinic Lights"
- **Offer:** "Essential cargo to {DEST}; the clinic's generator eats packs faster than the schedule admits."
- **Accept (patron):** "Dock with the crate still aboard. If you sell it early, someone else's morning ends."
- **Reminder:** "Food delivery still sealed. {DEST} Hub is the only honest unlock."
- **Complete:** "Clinic confirms receipt. Lights hold. Thank you for treating dinner like a route."
- **Expire:** "The clinic improvised. Improvisation tastes like fear. Next time, fly sooner."

### II.3.A Food delivery — Template FD-02 "Convoy Tender"
- **Offer:** "Fresh cargo to {DEST}; convoy tender running late and pride is a poor substitute for protein."
- **Accept:** "Tamsin's second will meet you on the pad. Do not explain the weather to hungry people."
- **Complete:** "Tender restocked. Convoy moves. You are briefly popular in a useful way."
- **Expire:** "They stretched rations. Popularity cancelled."

### II.3.A Food delivery — Template FD-03 "Survey Camp"
- **Offer:** "Survey camp at {DEST} forgot that listening burns calories."
- **Accept (Oru):** "Bring food. Leave their sky unowned."
- **Complete:** "Camp fed. Scanners resume. Oru sends a nod that counts as applause."

### II.3.B Pirate hunt — Template PH-01 "Approach Raider"
- **Offer:** "Raiders active near {DEST}; bounty live on one painted hull."
- **Accept (Iona):** "Clear the approach. Prefer disabled to deleted."
- **Complete:** "Lane reports quieter. Iona files it as 'useful violence, contained.'"
- **Expire:** "Raider moved on. So did someone's insurance premium."

### II.3.B Pirate hunt — Template PH-02 "Sleeping Freighter Wake"
- **Offer:** "Track one wanted hull beyond {DEST}. Freighters are sleeping; mass still exists."
- **Accept (Venn):** "Do not practice fire near docking collars. People try that. People get towed."
- **Complete:** "Wanted contact resolved. Tower prefers your boring version of events."

### II.3.C Exploration scan — Template EX-01 "Untitled Anomaly"
- **Offer:** "Survey unusual worlds around {DEST}."
- **Accept (Oru):** "Soft engines. Circle to scan. Do not invent a name before the instrument does."
- **Complete:** "Codex entry sealed. The unusual remains unusual — now with a receipt."
- **Expire:** "Anomaly drifted. Wonder delayed is still wonder; pay is not."

### II.3.C Exploration scan — Template EX-02 "Migration Whisper"
- **Offer:** "Quiet ping near {DEST}. Charts call it empty. Charts are often lonely."
- **Accept (Kei):** "Listen twice. Second listen is for Ryn's kind of stubborn."
- **Complete:** "Ping logged. Kei: 'That is how we keep people without owning them.'"

### II.3.D Pilot rescue — Template PR-01 "Beacon Still Calling"
- **Offer:** "Distress beacon near {DEST}."
- **Accept (Kei):** "Hail nearby, recover, return to hub. Nobody becomes a missing number today."
- **Complete:** "Pilot aboard and delivered. Kei sounds briefly less precise — which means relieved."
- **Expire:** "Beacon went quiet. File the silence. Do not decorate it."

### II.3.D Pilot rescue — Template PR-02 "Pod Count"
- **Offer:** "Civilian beacon in hostile lanes near {DEST}."
- **Accept (Tamsin):** "Count them. Then dock. Then talk."
- **Complete:** "All recovered seats filled. Tamsin: 'Paperwork can start breathing now.'"

### II.3.E Covert delivery — Template CD-01 "Agricultural Sensors"
- **Offer:** "Quiet courier run to {DEST}; local law is watching."
- **Accept (Sable):** "Boring lie. Sealed crate. No speeches."
- **Complete:** "Crate accepted without theatre. Sable: 'Professional. Don't get addicted to praise.'"
- **Expire / bust:** "Customs woke up. The lie was not boring enough — or you were."

### II.3.E Covert delivery — Template CD-02 "Sealed Favour"
- **Offer:** "Quiet run to {DEST}. Someone trusts you with a seal."
- **Accept (Mara):** "I don't open sealed things. Neither should you."
- **Complete:** "Seal intact on arrival. Trust compounds quietly."

## II.4 Mission Network dramatic rules

1. **Risk column is honesty.** High risk must mean real hostiles or law heat, not flavour text.
2. **White [M] markers** only on true mission contacts.
3. **Abandon returns crates** — never mint free cargo.
4. **Five slots** — fullness is a story beat ("too many promises").
5. Side jobs may **echo campaign themes** (forged notices, migration pauses, sealed crates) without spoiling Open Channel reveals.

## II.5 Systemic mission novellas (multi-job arcs)

These are optional chains the Mission Network can surface as linked briefs (implementation may start as flavour-linked singles).

### Arc MN-A — "The Hungry Pad" (3 deliveries)
1. Clinic pack → 2) School cook follow-up → 3) Convoy tender restock.  
**Theme:** Food as infrastructure.  
**Payoff radio (Tamsin):** "Three docks, zero speeches. That is how a lane stays civil."

### Arc MN-B — "Listen Twice" (2 scans + 1 rescue)
1. Untitled anomaly → 2) Migration whisper → 3) Beacon in the same sector.  
**Theme:** Observation before force.  
**Payoff (Oru):** "You scanned before you saved. That order matters."

### Arc MN-C — "Boring Lies" (2 covert + 1 hunt)
1. Agricultural sensors → 2) Sealed favour → 3) Clear the raider who shakes down quiet couriers.  
**Theme:** Silence as tool vs silence as product.  
**Payoff (Sable):** "You kept bargains. I keep mine. Uneasy friends are still friends."

---

# VOLUME III — EXPLORERS GUILD ASSIGNMENTS

Optional track (`src/guild.h`). Parallel to Open Channel; teaches verbs without spoiling Act I reveals.

## III.1 Assignment 0 — A Ship With Your Name On It
**Verbs:** Launch → return dock.  
**Kei:** "The ship is yours. Take your time."  
**Venn:** "Your berth will still be here. A safe return is a good first story."  
**Scene beat:** Soft launch, no combat required, harbour confidence.  
**Reward tone:** Receipt, not medal — same philosophy as prologue badge.

### Expanded dialogue (Story-capable)
**KEI:** Borrowed hulls remember their last pilot. Learn her turns before you ask her for heroics.  
**PLAYER:** And if I scratch the paint?  
**VENN:** Then you learn with credits. Prefer learning with docking soft enough that I do not file you.

## III.2 Assignment 1 — Something Worth Carrying
**Verbs:** Accept food delivery → complete at dest.  
**Tamsin:** "A crate can look like a number. At the far end, someone needs dinner."  
**Kei:** "Take a food job from the board. Keep its crate aboard until you dock."  
**Note:** Title overlaps Open Channel ch.02 intentionally as **echo**, not duplicate plot — Guild version is ordinary hunger; campaign version is sealed evidence.

## III.3 Assignment 2 — The Quiet Between Pings
**Verbs:** Find unscanned anomaly → Circle scan.  
**Oru:** "Silence is rarely quite empty."  
**Kei:** "Ryn taught me to listen twice."  
**Echo:** Prepares player ears for campaign migration chapters without naming Meridian.

## III.4 Assignment 3 — All Hands Accounted For
**Verbs:** Pilot rescue board job → hail → return hub.  
**Kei:** "A rescue beacon is still calling… Nobody should become a missing number."  
**Echo:** Prepares emotional muscle for Tamsin's convoy chapter.

## III.5 Assignment complete
**Kei:** "You brought people and answers home. That matters. Her trail goes beyond these assignments. For now, the galaxy is yours to explore."

---


# VOLUME IV — THE OPEN CHANNEL (MAIN CAMPAIGN)

**Authority scenes:** `docs/OPEN-CHANNEL-SCREENPLAY.md`  
**Playable binding:** `src/saga.h` (24 chapters × 6 ask-then-answer beats + closes/reactions)  
**Ops summary:** `docs/OPEN-CHANNEL-CAMPAIGN.md`

This volume is the **mission & verb manuscript** for the campaign. Full theatrical scenes live in the screenplay; here we bind story → playable action → recovery → trust.

## IV.1 Campaign map

| UI Ch | Saga idx | Title | Speaker | Verb | Kind |
|---|---|---|---|---|---|
| 01 | — | First Flight (prologue) | Kei/Venn | Launch, 600m, dock | training |
| 02 | 0 | Something Worth Carrying | Kei | Deliver sealed receiver | DOCK |
| 03 | 1 | The Quiet Between Pings | Oru | Scan migration signal | SCAN |
| 04 | 2 | All Hands Accounted For | Tamsin | Find last port | DOCK |
| 05 | 3 | A Lawful Mistake | Iona | Return records | HOME |
| 06 | 4 | The Last Useful Thing | Nadi | Scan wreck recorder | SCAN |
| 07 | 5 | The Price of Silence | Sable | **Choice 1** ledger | CHOICE |
| 08 | 6 | No Cargo Is Just Cargo | Mara | Reach settlement | DOCK |
| 09 | 7 | Ghosts of GalCop | Venn | Beacon chain | SCAN |
| 10 | 8 | The Mycoid Ledger | Iona | Secure archive | DOCK |
| 11 | 9 | Where the Giants Turn | Oru | Observe migration | SCAN |
| 12 | 10 | The Person on the Other End | Ryn | Reach shelter | DOCK |
| 13 | 11 | What We Carry Home | Kei | **Choice 2** publish | CHOICE |
| 14 | 12 | A Map for Everyone | Ryn | Neutral archive | DOCK |
| 15 | 13 | Federal Measure | Tamsin | Hear envoy | DOCK |
| 16 | 14 | Imperial Courtesy | Sable | Hear envoy | DOCK |
| 17 | 15 | Alliance of Necessity | Iona | Hear envoy | DOCK |
| 18 | 16 | The Coldest Signal | Pip | Scan; do not fire | SCAN |
| 19 | 17 | No Easy Flag | Ryn | **Choice 3** coalition | CHOICE |
| 20 | 18 | The Black Flight | Venn | Break blockade | HUNT |
| 21 | 19 | Aegis Echo | Oru | Recover buoy | SCAN |
| 22 | 20 | The Open Channel | Kei | Clear primary relay | HUNT |
| 23 | 21 | The Answering Dark | Ryn | Record echo | SCAN |
| 24 | 22 | Who Keeps the Light | Iona | **Choice 4** custody | CHOICE |
| 25 | 23 | A Berth Kept Warm | Kei | Return Lave Hub | HOME |

## IV.2 Permanent decisions

| # | Chapter | A Public | B Guild | C Law/Independent |
|---|---|---|---|---|
| 1 | Price of Silence | Publish ledger | Guild peer review | Lodge with Iona |
| 2 | What We Carry Home | Broadcast now | Verify first | Limited alert (+Independent) |
| 3 | No Easy Flag | Public convoy | Guild survey | Lawful force |
| 4 | Who Keeps the Light | Public custody | Guild custody | Lawful archive |

Trust buckets: Public / Guild / Law / Independent. Helpers surface from ch.19+. Epilogue line follows dominant trust.

## IV.3 Chapter mission sheets (compressed)

Each sheet: **Dramatic question → Playable verb (now) → Ideal bespoke verb (ahead) → Recovery → Key line**.

### Ch.02 Something Worth Carrying
- **Q:** Will you carry sealed trust without opening it?
- **Now:** Dock at dest (Mara's port binding via saga dest).
- **Ahead:** Collect flagged evidence item; forged hazard detour flag.
- **Recovery:** Mara holds duplicate seal hash indefinitely.
- **Line:** "Ryn never missed a call… Never a call."

### Ch.03 Quiet Between Pings
- **Q:** Can you observe without turning life into a target?
- **Now:** SCAN discoveries in dest system.
- **Ahead:** Soft-approach observation; reset if too hot; no fire objective.
- **Recovery:** Re-enter system; observation resets cleanly.
- **Line:** "Silence is part of the message."

### Ch.04 All Hands Accounted For
- **Q:** Who counts crews when charts lie?
- **Now:** DOCK last port.
- **Ahead:** Lifeboat telemetry + optional pod rescues → Independent trust.
- **Recovery:** Port stamp persists until scrubbed timer (design).
- **Line:** "I need a place, not another tidy rumour."

### Ch.05 A Lawful Mistake
- **Q:** Can evidence beat an early badge?
- **Now:** HOME to Lave with "records."
- **Ahead:** Timestamp compare; inspection or protest; never lose evidence.
- **Recovery:** Iona holds case open.
- **Line:** "A badge is not evidence. Mine included."

### Ch.06 The Last Useful Thing
- **Q:** What truth survives scavengers?
- **Now:** SCAN wreck.
- **Ahead:** Unedited recorder dump; Pip's checklist key; no scrap linger.
- **Recovery:** Wreck remains until dump taken.
- **Line:** "Pull clear once the recorder answers."

### Ch.07 The Price of Silence (CHOICE)
- **Q:** Who holds the ledger?
- **Now:** Choose A/B/C → trust + reaction line.
- **Ahead:** Sable bargain staging; refusal honoured.
- **Line:** "Public data also reaches pirates."

### Ch.08 No Cargo Is Just Cargo
- **Q:** Is a clinic a paperwork problem or a promise?
- **Now:** DOCK settlement.
- **Ahead:** Escort / carry / clear buoy — all keep settlement alive.
- **Recovery:** Settlement waits; method changes trust tone.
- **Line:** "Lights beat paperwork."

### Ch.09 Ghosts of GalCop
- **Q:** What did the seal mean when it stopped meaning?
- **Now:** SCAN beacons.
- **Ahead:** Multi-node chain; archive under Lave.
- **Line:** "Near the end, nobody knew who was responsible."

### Ch.10 The Mycoid Ledger
- **Q:** Will history become a weapon catalogue?
- **Now:** DOCK secure archive.
- **Ahead:** Refuse Meridian research pitch; mycoid non-craftable hard rule.
- **Line:** "Mycoid stays history."

### Ch.11 Where the Giants Turn
- **Q:** Can patience be an instrument?
- **Now:** SCAN migration.
- **Ahead:** Match three calls → shelter coordinate; no herd/fire.
- **Line:** "Cut your engines. We are visitors here."

### Ch.12 The Person on the Other End
- **Q:** Is Ryn a prize or a person?
- **Now:** DOCK shelter.
- **Ahead:** Repair shelter; disable amplifier; reunion scene (Kei/Ryn).
- **Line:** "I'm alive. The clever part can wait."

### Ch.13 What We Carry Home (CHOICE)
- **Q:** How loud does truth need to be?
- **Now:** Publish / verify / limited.
- **Ahead:** Act III support shifts.
- **Line:** "I talk too much when I'm scared."

### Ch.14–17 Envoy triad + Map
- **Q:** Must a map have a flag?
- **Now:** DOCK archive / hear envoys (listen only).
- **Ahead:** Buyout refusal; rescue exercise; courtesy debt; audit trail test.
- **Line:** "Pick allies, not owners." (setup for 19)

### Ch.18 The Coldest Signal
- **Q:** Can we refuse the hunt?
- **Now:** SCAN; design forbids fire as objective.
- **Ahead:** Non-target contact; no loot; leaves.
- **Line:** "Do not fire. Record. Leave a polite wake."

### Ch.19 No Easy Flag (CHOICE)
- **Q:** Who stands on the channel without owning you?
- **Now:** Coalition choice → helpers.
- **Ahead:** Visible tenders/markers/ceasefires/covert corridor.

### Ch.20–22 Black Flight / Aegis / Open Channel
- **Q:** Can precision beat revenge?
- **Now:** HUNT blockade; SCAN buoy; HUNT relay.
- **Ahead:** Non-lethal blockade paths; shutdown sequence; Voss confrontation without required death.
- **Line:** "Dependence is not consent."

### Ch.23 The Answering Dark
- **Q:** Must an answer mean us?
- **Now:** SCAN distant echo.
- **Ahead:** Record only; no myth chase.
- **Line:** "It may never have meant us."

### Ch.24 Who Keeps the Light (CHOICE)
- **Q:** Who holds keys under inspection?
- **Now:** Custody choice.
- **Ahead:** Losers keep public route access; trust changes tone not fuel.

### Ch.25 A Berth Kept Warm
- **Q:** What did asking cost — and give?
- **Now:** HOME Lave.
- **Ahead:** Permanent berth six; free flight; open channel epilogue by trust.
- **Line:** "This berth is yours now."

## IV.4 Ideal bespoke set-piece backlog (priority)

1. Sealed evidence item + forged hazard detour flag (Ch.02)
2. Soft observation reset (Ch.03 / 11)
3. Pod counting (Ch.04)
4. Timestamp courtroom beat (Ch.05)
5. Recorder dump mini-game (Ch.06)
6. Ryn reunion multi-page (Ch.12)
7. Non-lethal blockade (Ch.20)
8. Voss stand-down / testify / flee (Ch.22)
9. Trust-coloured epilogue berth scenes (Ch.25)

---


# VOLUME V — LIVING GALAXY TEXTURE

Narrative that makes systems feel inhabited without requiring new verbs.

## V.1 Station & berth life
- **Venn's tower:** clearance, soft launch, collar etiquette, grief-as-procedure.
- **Dockhand_77:** paint, boosters, dirty bays — comic relief that never blocks story.
- **Evidence cages / general cargo:** teaching players that some crates have legal ontology.
- **Berth six:** motif of home; cold → warm → permanent.

### Micro-dialogues (hail / crawl-safe)
1. **VENN:** "If your approach looks like a dare, abort. I prefer boring pilots."
2. **DOCKHAND_77:** "Stop boosting near the collar, you maniacs."
3. **KEI:** "Select opens the deck when you need air. Needing air is allowed."

## V.2 Freight & convoys
Haulers have routes, manifests, sleep, mass. Hails should report cargo and mood.
- Inbound: "From {PEER}. {QTY}{U} {GOOD}. {STATUS}."
- Story echo: forged "safe route" ads in Act I–II traffic chatter.
- Tamsin's convoy as recurring working-class chorus.

## V.3 Factions (conduct, not slogans)

| Faction | Player-facing conduct | Story use |
|---|---|---|
| Explorers | Share scans; don't hoard silence | Kei, Oru, Guild |
| Traders | Margins honest when watched | Mara, freighters |
| Law | Evidence > theatre | Iona |
| Pirates | Bargains explicit; betrayal expensive | Sable, raiders |

## V.4 Radio stations (narrative function)
- **Deep Field** — loneliness between jumps; good under Oru chapters.
- **Neon Transit** — corridor hustle; delivery/covert.
- **Pixel Comet** — bright dread; hunt/blockade.
- **Velvet Orbit** — berth night; Kei/Ryn talks.
- **Far Horizons** — alien chatter; Coldest Signal / Answering Dark adjacency (never translating Thargoids into neat English).

## V.5 GalNet / SpaceBook tone board
Posts should feel local and slightly wrong when Meridian is active.
- Iona: patrol honesty.
- Kei: missing ping / survey colour.
- Dockhand: slapstick safety.
- "DefinitelyNotAPirate": offers that are jokes until they aren't.
- Mission Network: clocks, slots, markers — diegetic UI.

### Campaign-reactive GalNet seeds
| Flag | Post seed |
|---|---|
| After Ch.07 publish | "Anonymous chart dump flooding local boards." |
| After Ch.07 Guild | "Explorers peer-review lane open — slow, stubborn." |
| After Ch.07 Iona | "Lawful evidence cage sealed; no comment." |
| After Ch.13 loud | Settlements prep; Meridian ads turn personal. |
| After Ch.19 | Helper faction mentions "standing by on your route." |
| After Ch.23 | "Unidentified echo — do not approach. Do not invent gods." |

## V.6 Police stop (citizen track)
Warrant dialogue stays adult and local:
- Pay / custody / run.
- Running raises heat and story tone ("you chose speed over argument").
- Never soft-lock campaign actors behind warrants.

---

# VOLUME VI — POSTGAME & EXPANSION SEEDS

## VI.1 Immediate postgame (shipped shape)
- Free flight continues; berth warm.
- Trust helpers remain.
- Mission Network stays the daily novel.
- Epilogue line on Story complete screen.

## VI.2 Expansion seeds (designed, not promised)

### Seed E1 — Station restoration project
A neglected outpost offers rebuild missions; narrative about commons vs private sponsorship (Meridian echo without sequel bait-and-switch).

### Seed E2 — Mobile freighter home
Player freighter as living room; crew notes from Tamsin/Nadi; radio permanent fixture.

### Seed E3 — Quiet migration calendar
Seasonal (seeded) migration events; Oru seasonal briefs; never turns whales into resources.

### Seed E4 — Audit adventures
Iona optional cases: forged notices, early filings, badge theatre — systemic "lawful mistake" generator.

### Seed E5 — Sable's ledger
Independent trust high → grey jobs that never require betraying Open Channel ethics; bargains stay explicit.

### Seed E6 — Voss epilogue variants
If fled / testified / stood down: rare GalNet sightings; no revenge fetch quest required.

## VI.3 Implementation map (story → code)

| Manuscript | Code / doc |
|---|---|
| Vol I themes/cast | Screenplay character bible; saga speakers |
| Vol II mission templates | `sectors.h` briefs → future authored brief table |
| Vol III Guild | `guild.h` lines/objectives |
| Vol IV campaign | `saga.h` + OPEN-CHANNEL-SCREENPLAY.md |
| Vol V texture | GalNet strings, radio, freight hails, crawl talk |
| Vol VI seeds | DESIGN-BIBLE expansion gates |

## VI.4 Manuscript revision log

| Date | Build | Change |
|---|---|---|
| 2026-09-22 | 2.5.32 | Manuscript begun: Vols I–VI scaffold + mission banks + campaign verb sheets |
| (next) | — | Expand Ch.02–07 full scenes into playable page scripts |
| (next) | — | Author 3 systemic mission novella chains in board data |
| (next) | — | GalNet reactive post table wired to `saga_flags` |

---


# VOLUME II (EXPANDED) — MISSION TEMPLATE BANK

Additional offer/complete pairs. Destinations remain `{DEST}`; risk adjectives scale with board risk.

## II.6 Food delivery bank (FD-04–FD-12)

| ID | Offer | Complete |
|---|---|---|
| FD-04 | "Protein crates to {DEST}; the outpost kitchen ran out of polite excuses." | "Kitchen confirms. Excuses retired." |
| FD-05 | "Greenhouse starters for {DEST}; someone planted hope without fertiliser." | "Starters docked. Hope may proceed." |
| FD-06 | "Water filters + rations to {DEST}; thirst makes bad navigators." | "Filters installed. Navigators rehydrate." |
| FD-07 | "Medical softpacks to {DEST}; not glamorous, merely necessary." | "Clinic signed. Merely necessary wins." |
| FD-08 | "Bulk grain to {DEST}; margins thin, hunger not." | "Grain accepted. Thin margins thanked you anyway." |
| FD-09 | "Emergency pantry to {DEST} after a forged 'safe route' delay." | "Pantry restocked. Ask who forged the delay later." |
| FD-10 | "Guild survey rations to {DEST}; listening burns calories." | "Survey fed. Listening resumes." |
| FD-11 | "Children's meal packs to {DEST} school dock." | "School dock quiet in the good way." |
| FD-12 | "Freighter crew mess resupply at {DEST}." | "Mess restocked. Crew less philosophical about soup." |

## II.7 Pirate hunt bank (PH-03–PH-10)

| ID | Offer | Complete |
|---|---|---|
| PH-03 | "Painted hull shaking down couriers beyond {DEST}." | "Courier lane reports fewer shakedowns." |
| PH-04 | "Interceptors loitering on the public approach to {DEST}." | "Approach cleared. Tower files 'contained.'" |
| PH-05 | "One loud pirate with a quiet freighter problem near {DEST}." | "Loud problem resolved. Freighter still sleeping." |
| PH-06 | "Bounty on a repeat offender tagged near {DEST}." | "Repeat offender interrupted. Prefer it stays interrupted." |
| PH-07 | "Raider wing testing the collar at {DEST}." | "Collar untested. Venn approves of boredom." |
| PH-08 | "Privateers flying Meridian-coloured excuses near {DEST}." | "Excuses failed. Lane holds." |
| PH-09 | "Salvage jackals claiming wrecks that still have owners near {DEST}." | "Owners thanked you. Jackals did not." |
| PH-10 | "A 'customs inspection' that is not customs near {DEST}." | "Fake customs closed. Real customs unamused." |

## II.8 Exploration bank (EX-03–EX-10)

| ID | Offer | Complete |
|---|---|---|
| EX-03 | "Mineral bloom on {DEST}'s inner body — catalogue, don't claim." | "Bloom logged. Ownership declined." |
| EX-04 | "Fauna migration fringe near {DEST}." | "Fringe recorded. Road left clear." |
| EX-05 | "Abandoned beacon with a pulse that is not GalCop-standard." | "Pulse archived. History gains a footnote." |
| EX-06 | "Ring shepherd anomaly at {DEST}." | "Shepherd logged. Rings remain unimpressed." |
| EX-07 | "Whisperband interference — soft scan only." | "Interference mapped. No sermons attached." |
| EX-08 | "Codex hole: unnamed moonlet around {DEST}." | "Moonlet named by evidence, not poetry." |
| EX-09 | "Derelict science buoy still singing." | "Song captured. Buoy left singing." |
| EX-10 | "Quiet giants' wake — observe, do not herd." | "Wake observed. Giants undisturbed." |

## II.9 Rescue bank (PR-03–PR-08)

| ID | Offer | Complete |
|---|---|---|
| PR-03 | "Lifeboat telemetry fading near {DEST}." | "Telemetry became a person again." |
| PR-04 | "Pilot punched out after forged-route ambush." | "Pilot home. Forgery still on the docket." |
| PR-05 | "Civilian pod adrift past the sun-line of {DEST}." | "Pod recovered. Sun-line less lonely." |
| PR-06 | "Guild scout overdue; beacon intermittent." | "Scout returned. Overdue cancelled." |
| PR-07 | "Freighter tender crew in suits — air thin, time thinner." | "Crew boarded. Air thickens." |
| PR-08 | "Anonymous distress with a polite encryption." | "Polite encryption opened into a living pilot." |

## II.10 Covert bank (CD-03–CD-08)

| ID | Offer | Complete |
|---|---|---|
| CD-03 | "Sealed legal filings to {DEST} — not for open channels." | "Filings arrived sealed. Iona prefers that." |
| CD-04 | "Spare parts that are also leverage, bound for {DEST}." | "Parts delivered. Leverage unused — for now." |
| CD-05 | "A letter that must not become a broadcast." | "Letter hand-delivered. Broadcast avoided." |
| CD-06 | "Quiet medicine for a clinic avoiding Meridian invoices." | "Medicine received. Invoices remain hated." |
| CD-07 | "Archive shard wrapped as farm sensors." | "Shard shelved. Farm story held." |
| CD-08 | "Witness schedule to a lawful cage — no speeches en route." | "Schedule lodged. Speeches can wait." |

## II.11 Failure & complication cards (attach randomly)

1. **Forged hazard overlay** — detour available; taking it flags Meridian comfort.
2. **Sleeping freighter** — mass in the lane; patience check.
3. **Early law filing** — inspection possible; evidence must survive.
4. **Scavenger race** — arrive before the useful parts leave.
5. **Beacon fade** — rescue timer pressure without soft-lock.
6. **Customs joke that isn't** — covert heat spike.
7. **Migration road** — weapons disabled ethically if giants present.
8. **Empty seat at the bar** — Dockhand_77 flavour on fail.

## II.12 Board diegesis — how jobs should *feel*

A good board job answers: **Who needs this?** **What breaks if I'm late?** **What lie would make this easier — and should I refuse that lie?**

If a brief cannot answer those three, rewrite it.

---

# VOLUME III (EXPANDED) — GUILD SCENES

## III.6 Full Guild page scripts (playable length)

### G0 — A Ship With Your Name On It
**KEI:** The ship is yours to learn, not to impress. Launch when your hands mean the checklist.  
**PLAYER:** What is the checklist?  
**KEI:** Leave. Clear. Return. Dock. Everything dramatic can wait until those four stop being news.  
**VENN:** Soft launch. If you graze the ring, I will know before you do.  
**PLAYER:** Cleared.  
**VENN:** Good. Bore me on the way home.

### G1 — Something Worth Carrying (Guild)
**TAMSIN:** A crate can look like inventory. At the far pad it looks like dinner.  
**PLAYER:** Just food?  
**TAMSIN:** Just civilisation. Take the board job. Keep the crate aboard — selling early is a different kind of piracy.  
**KEI:** She is right. Hunger is a route problem.

### G2 — The Quiet Between Pings (Guild)
**ORU:** Find an anomaly. Get close. Scan. Leave.  
**PLAYER:** Leave without naming it?  
**ORU:** Name it after the instrument speaks. Poetry is allowed later.  
**KEI:** Ryn listened twice. Second listen is where the map changes.

### G3 — All Hands Accounted For (Guild)
**KEI:** A beacon is still a person. Hail. Recover. Hub.  
**PLAYER:** And if I'm late?  
**KEI:** Then we file silence honestly. Do not decorate it. Go early instead.

---

# VOLUME IV (EXPANDED) — OPEN CHANNEL SCENE BRIDGES

Bridge scripts connect playable six-beat briefs to full screenplay scenes. Use when Story UI gains pages.

## IV.5 Bridge — Ch.02 Mara handoff (excerpt)
**MARA:** Guild, or just the person Kei trusts with sealed things?  
**KEI:** *(radio)* Both, if we are lucky. Still sealed?  
**MARA:** Sealed. Cold. I did not open it — before you ask in that careful voice.  
**PLAYER:** What's inside?  
**MARA:** A receiver that listens better than it talks. Boring lie if asked: agricultural sensors.  
**KEI:** Do not open it for curiosity. Curiosity is how evidence becomes a story you cannot prove.

## IV.6 Bridge — Ch.12 Reunion spine
**RYN:** I am alive.  
**KEI:** Good. The clever part can wait.  
**RYN:** You sent someone with a ship and a tendency to come back. Dangerous habit. Keep it.  
**PLAYER:** Amplifier?  
**RYN:** Kill the noise. Then we talk in air that is not for sale.

## IV.7 Bridge — Ch.22 Voss non-lethal endings
**VOSS:** Control created dependable routes.  
**KEI:** Dependence is not consent.  
**VOSS:** *(stand down)* Then keep your charter. I will not die for a map.  
**VOSS:** *(testify)* I will speak where receipts survive.  
**VOSS:** *(flee)* You can have the relay. The galaxy still buys comfort.  
*(Death is never a required state.)*

## IV.8 Trust helper radio (post choice 3)

| Trust | Helper line |
|---|---|
| Public ≥2 | "Civilian fuel tender on your vector. No speeches. Just fuel." |
| Guild ≥2 | "Survey markers painted. Follow the dots, not the ads." |
| Law ≥2 | "Ceasefire call ready if Meridian paints the public lane." |
| Independent ≥1 | "Sable's corridor is open. Explicit bargain. No surprises." |

---

# VOLUME V (EXPANDED) — SYSTEM STORY SEEDS

Every system can carry a one-line "story weather" used by GalNet and traffic.

## V.7 Story weather deck (roll by system seed)

1. Market fat / market thin.  
2. Patrol calm / patrol hunting paperwork.  
3. Pirate jokes that sound like offers.  
4. Meridian clear-lane ad with wrong timestamp.  
5. Guild missing-persons whisper.  
6. Freighter sleeping on your three.  
7. Migration interference on scanners.  
8. Old GalCop seal graffiti in the docking collar.  
9. Clinic waiting on a crate.  
10. Beacon that might be weather and might be a person.

## V.8 Named recurring locations (narrative)

| Place | Story job |
|---|---|
| Lave System Hub / Berth six | Home, procedure, reunion |
| Mara's agricultural dock | Sealed trust, honest lies |
| Clinic settlement (Ch.08) | Lights vs paperwork |
| GalCop beacon chain | Institutional memory |
| Ryn's shelter | Person, not prize |
| Primary relay | Cut without revenge |
| Neutral archive | Maps without flags |

---

# VOLUME VII — SIDE CHANNELS & CHARACTER CALLS

Optional radio calls between chapters. Keep short; never block campaign.

## VII.1 Kei check-ins
- After first delivery job: "You kept a promise moving. That scales."
- After first warrant: "Pay or argue — running writes a louder story."
- After Ryn found: "I am still talking too much. It is how I stay precise."

## VII.2 Ryn after reunion
- "I will not vanish without a note again. Notes can lie. I will try not to."
- "If Kei over-explains, she is scared. Translate that gently."

## VII.3 Oru seasonal
- "Giants turning early this seed. Soft engines if you share their road."

## VII.4 Sable
- "I keep bargains. Keep yours. Uneasy friends live longer."

## VII.5 Pip status
- "Maintenance complete. Existential confidence: unchanged. Recommendation: fly anyway."

---

# APPENDIX A — QUICK LINES WORTH TATTOOING ON THE HUD

1. "Ryn never missed a call."
2. "Silence is part of the message."
3. "A badge is not evidence."
4. "Dependence is not consent."
5. "Pick allies, not owners."
6. "Record. Leave a polite wake."
7. "This berth is yours now."
8. "Home is rarer than jump fuel."

# APPENDIX B — CHAPTER TITLE LIST (CANON)

First Flight; Something Worth Carrying; The Quiet Between Pings; All Hands Accounted For; A Lawful Mistake; The Last Useful Thing; The Price of Silence; No Cargo Is Just Cargo; Ghosts of GalCop; The Mycoid Ledger; Where the Giants Turn; The Person on the Other End; What We Carry Home; A Map for Everyone; Federal Measure; Imperial Courtesy; Alliance of Necessity; The Coldest Signal; No Easy Flag; The Black Flight; Aegis Echo; The Open Channel; The Answering Dark; Who Keeps the Light; A Berth Kept Warm.

---

*End of current manuscript draft. Continue by deepening Volume II template banks and Volume IV bespoke set-piece scripts without breaking `saga_beats[]` order.*
