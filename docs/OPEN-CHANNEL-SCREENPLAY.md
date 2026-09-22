# THE OPEN CHANNEL — Feature Screenplay & Campaign Design

**Status:** Canonical story authority for ELITE: NEXT 2.5.21+ (playable prose quality pass 2.5.31)  
**Form:** Feature-length interactive screenplay (approx. 110–130 script pages / ~10 hours critical path)  
**Playable binding:** `src/saga.h` (six-beat PSP briefs with chapter-authored player asks) · prologue lines in `src/narrative-nav.h` · `docs/OPEN-CHANNEL-CAMPAIGN.md` (summary bible)  
**Rule:** GalNet, novels and tourist-beacon text are **reference only** — never reproduced. All dialogue below is original.

**Assembly:** Merged full Act I–IV drafts (character bible, lore ledger, branching scenes) into one shippable authority document.

**Playable prose (2.5.31–2.5.32):** Brief lines are full spoken sentences that wrap on the Story screen (three lines). Player replies are chapter-specific questions; beat N+1 answers the ask on beat N. Chapter closers and permanent-choice reactions are character-voiced. Radio captions use a 160-char voice buffer.

---

## 0. Logline

A Guild coordinator and an independent pilot chase a missing surveyor through the Old Worlds and discover that a navigation corporation has been selling “safe lanes” by amplifying the migration songs of living spaceborne animals — and that something nonhuman has begun to answer the silence left behind.

---

## 1. Theme

**Routes are power.** Whoever writes the chart writes who lives.  
**Observation is not ownership.** Scanning is an ethic, not a loot verb.  
**Institutions fail when their records cannot be challenged.** GalCop’s seal once meant the lane belonged to everyone; near the end it meant nobody knew who was responsible.  
**Dependence is not consent.** Reliable private routes can still be a cage.

Tone: working pilots, moral ambiguity, quiet dread. Thargoids remain rare and unknowable — traces, one remote contact, one answering echo. No disposable alien parade. Mycoid is history, never a craftable weapon.

---

## 2. Setting frame

| Fact (public lore, paraphrased) | How we use it |
|---|---|
| Lave was GalCop’s capital; GalCop dissolved 19 Aug 3174 | Venn’s archive; berth culture |
| First Thargoid War; INRA mycoid bioweapon; later exposure | “The Mycoid Ledger” — evidence, never a tech tree |
| Guardians fought Thargoids long ago | Aegis Echo discusses science vs escalation |
| Aegis created / dismantled / returned | Institutional memory |
| Azimuth / Salvation / Proteus Wave / Titans (to Dec 3310) | Background trauma; civilians tired of saviours |
| Federation / Empire / Alliance diplomacy styles | Envoy chapters — listen without forced allegiance |
| Uncertain deep-space anomalies / contested signals | Coldest Signal / Answering Dark preserve uncertainty |

**Original orgs:** Pale Meridian, local Explorers Guild cell, Open Channel relay coalition.  
**Year of play:** ~3311, after the Titan war.

### Playable permanent decisions

| # | Saga ch | Title | A Public | B Guild | C Law / Independent* |
|---|---|---|---|---|---|
| 1 | 5 | Price of Silence | Publish ledger | Guild peer review | Lodge with Iona |
| 2 | 11 | What We Carry Home | Broadcast now | Verify first | Limited alert* |
| 3 | 17 | No Easy Flag | Public convoy | Guild survey | Lawful force |
| 4 | 22 | Who Keeps the Light | Public custody | Guild custody | Lawful archive |

\*Limited alert also raises Independent trust. Helpers/epilogue via `saga_trust_*` in `src/saga.h`.

### Display / continuity rules

- Do not reorder `saga_beats[]`. Saves store chapter indices.
- Choice UI: `saga_choice_label(chapter, option)`.
- Framebuffer present: `PSP_DISPLAY_SETBUF_IMMEDIATE` after vblank (NEXTFRAME strobes on hardware).
- PSP Story briefs wrap spoken sentences (three lines, ~46 glyphs); this document still holds full scene scripts beyond the six-beat binding.

---

# PART A — Character Bible, Lore Ledger, Act II

> Source draft: Act II design packet (character bible + chapters 08–13).

## ELITE: NEXT — The Open Channel: Character Bible, Lore Ledger, Act II Screenplay

Campaign organisations **Pale Meridian** and the **Explorers Guild local cell (Lave / Old Worlds)** are ORIGINAL to ELITE: NEXT. They are not Frontier entities. Established Elite history (GalCop, INRA, Aegis, Guardians, Thargoids, the three superpowers) informs setting only. No GalNet article text, novel prose, or tourist-beacon copy is reproduced here.

UI chapter numbers below match playable saga indexing: Tracked Mission `CHAPTER %02d` = `saga_chapter + 2`. Chapters **08–13** = saga indices **6–11**.

---

## Character Bible

### Kei Aven
**Age band:** Mid-thirties (adult).  
**Appearance:** Adult lavender-gray alien woman; pearl-gray hair; subtle antennae; teal eyes; navy Explorers Guild survey jacket; posture that stays composed even when her voice tightens.  
**Speech pattern:** Precise, warm, slightly over-structured when afraid. She restates the next safe action as if repetition were a life-preserver. Humour arrives dry and late.

Sample lines:
1. “Ryn never missed a call. She missed meals, permits, and once an entire moon. Never a call.”
2. “Start with ‘I'm alive.’ The clever part can wait.”
3. “I asked you to bring her home. I did not ask you to become her cage.”

**Want:** Ryn returned, alive, and still herself.  
**Need:** To trust care that does not control.  
**Wound:** She once overrode a surveyor's judgment “for safety” and lost a year of friendship.  
**Lie:** If she holds every schedule tightly enough, nobody she loves will vanish.  
**Relationships:** Protective peer/rival with Ryn; professional respect for Oru; wary ally of Iona; politely hostile toward Voss; relies on the player as the person who can leave when she cannot.  
**Arc (24 chapters):** Act I coordinator who converts fear into procedure → Act II confronts the cost of guarding Ryn by controlling news → Act III listens to three powers without adopting their language as her own → Act IV releases custody of the channel and admits the player was given a home by the same ask that found Ryn.

### Ryn Vale
**Age band:** Early thirties.  
**Appearance:** Human; windburned surveyor's hands; hair cut with ship scissors; patched environmental suit with Guild tape over a torn sleeve; eyes that catalogue rooms before people.  
**Speech pattern:** Observant, sideways funny, allergic to being rescued as a prize. She answers emotion with logistics until she trusts the listener.

Sample lines:
1. “I'm alive. The clever explanation can wait — unless Kei is listening, in which case she'll invent one for me.”
2. “The relays aren't calling the giants. They're eavesdropping. Meridian turned the eavesdrop into a toll booth.”
3. “Pick allies, not owners. We live with that after the credits roll.”

**Want:** The migration lanes left alone enough for settlements to live.  
**Need:** To trust a crew with danger instead of vanishing alone.  
**Wound:** A prior “safe route” she certified killed strangers when a corporation moved the markers.  
**Lie:** Isolation is the only ethical form of protection.  
**Relationships:** Deep unfinished conversation with Kei; field kinship with Oru; transactional respect for Sable; contempt for Voss's reasonableness; fondness for Pip's honesty.  
**Arc:** Missing problem-to-solve → reluctant witness → returning contact with agency → co-architect of the open channel who refuses to become anyone's symbol.

### Venn Calder
**Age band:** Late fifties.  
**Appearance:** Human; Lave traffic-controller vest over old GalCop archival greys; tired eyes; coffee ring permanently ghosted on his sleeve cuff.  
**Speech pattern:** Procedure first, grief second. Short sentences. When emotion breaks through, he cites a regulation number like a prayer.

Sample lines:
1. “Berth six is held. Not for nostalgia. For continuity.”
2. “The old seal meant the lane belonged to everyone. Near the end, it meant nobody knew who was responsible.”
3. “I can clear you for departure. I cannot clear you for regret.”

**Want:** Lave's lanes to remain usable and accountable.  
**Need:** To mourn GalCop without reinventing its failures.  
**Wound:** He filed a quiet report that arrived after a convoy was already gone.  
**Lie:** Correct paperwork can redeem a late warning.  
**Relationships:** Mentors the player; soft spot for Kei; archives shared with Iona; distrust of Meridian's “efficiency”; cordial distance from superpower envoys.  
**Arc:** Gatekeeper of berths → keeper of uncomfortable history → voice of public lanes under blockade → man who assigns a permanent home berth as act of faith, not nostalgia.

### Iona Marr
**Age band:** Early forties.  
**Appearance:** Local law officer; practical uniform without parade polish; evidence satchel; scar at the temple from a docking-bay scuffle she refuses to mythologize.  
**Speech pattern:** Flat, sceptical, exact. She asks for timestamps the way other people ask for names.

Sample lines:
1. “A badge is not evidence. Mine included.”
2. “That theft report predates the cargo. Prove it, or I will.”
3. “Someone must hold the keys and accept inspection. Prefer the boring option.”

**Want:** An evidence chain that survives politics.  
**Need:** To believe law can serve people who do not wear uniforms.  
**Wound:** She once enforced a closure order that stranded a clinic overnight.  
**Lie:** Neutrality is the same as justice.  
**Relationships:** Professional friction-then-trust with Kei; mutual respect with Venn; adversary/auditor to Voss; cautious contact with Sable; later coalition auditor.  
**Arc:** Obstacle with a warrant → ally who preserves evidence → custodian candidate who tests Alliance minutes → charter author who keeps losers' access intact.

### Oru Sen
**Age band:** Late forties.  
**Appearance:** Xenobiologist; layered field coat; sample cases that never look like weapons; calm face that softens when listening to static.  
**Speech pattern:** Patient teacher. Metaphorical without being precious. Corrects “threat” language gently.

Sample lines:
1. “Silence is part of the message. People forget that when they own a transmitter.”
2. “Cut your engines. We are visitors here.”
3. “Then our first useful act was making room.”

**Want:** Observation before ownership.  
**Need:** To accept that publishing knowledge can also endanger the observed.  
**Wound:** A paper of theirs was used to justify a cull.  
**Lie:** If the science is careful enough, politics cannot misuse it.  
**Relationships:** Mentor to Ryn; steadying presence for Kei; Pip treats them as preferred human; Hale finds them “unhelpfully nuanced.”  
**Arc:** Quiet Act I naturalist → Act II migration reader who finds Ryn's cadence → Act III interpreter of the cold signal → Act IV advocate for uncertainty preserved in the log.

### Tamsin Reed
**Age band:** Mid-forties.  
**Appearance:** Convoy captain; grease on a good jacket; headset permanently half-on; posture of someone who sleeps in chairs.  
**Speech pattern:** Working-pilot cadence. Practical, blunt, loyal to crews over flags.

Sample lines:
1. “A convoy vanished where the route says safe. Routes don't get to say that without witnesses.”
2. “I'll take fuel tenders over speeches. Speeches don't dock.”
3. “Federal coverage sounds like sleep. Read the fine print before you close your eyes.”

**Want:** Routes that do not murder her tenders.  
**Need:** To admit civility sometimes needs coalitions she distrusts.  
**Wound:** Lost a tender on a “newly published safe lane.”  
**Lie:** Independents can outwork institutions forever.  
**Relationships:** Trusts the player after rescue work; respectful sparring with Hale; allergy to Lysara's courtesy; finds Mirek's minutes usefully dull.  
**Arc:** Victim of falsified routes → working-class spine of Act II relief → voice of Federal cost/benefit → Black Flight survivor who still prefers lives over wreckage.

### Nadi Quill
**Age band:** Late twenties.  
**Appearance:** Salvage engineer; gloves clipped to belt; goggles pushed into curly hair; Meridian seals photographed like crime-scene stickers.  
**Speech pattern:** Fast, technical, gallows-humour. Names parts before motives.

Sample lines:
1. “That wreck still has a recorder worth saving. Scrap can wait.”
2. “Pip found a route key in a maintenance checklist. Of course they hid power in boredom.”
3. “If Meridian paid pirates to steal their own shipment, congratulate accounting — then arrest them.”

**Want:** Useful wrecks recovered before politics strip them bare.  
**Need:** To stop treating people like recoverable modules.  
**Wound:** Sold a part that later powered a blockade buoy.  
**Lie:** If something still works, ethics are a luxury.  
**Relationships:** Pip's partner/parent/crew; trades intel with Sable carefully; Kei hires them when Guild polish fails.  
**Arc:** Comic salvage lead → evidence technician → quiet conscience when scrap and weapons blur → postgame salvage-tug advocate.

### Pip
**Age band:** Maintenance drone (apparent “young” personality; no human age).  
**Appearance:** Compact utility drone; scuffed chassis; status light that flickers when lying would be efficient; tool arms that gesture like hands.  
**Speech pattern:** Literal, clipped, oddly tender. Confidence scores stated aloud. Humour accidental until it isn't.

Sample lines:
1. “Signal structure: nonhuman. Confidence: troubling.”
2. “Do not fire. Record. Leave a polite wake.”
3. “Berth warm. Crew accounted. Pip approving temporary joy.”

**Want:** Systems that stay repairable.  
**Need:** To understand that some silences are not faults.  
**Wound:** Was ordered to “optimise” a relay until fauna bled noise.  
**Lie:** Completeness equals care.  
**Relationships:** Bonded to Nadi; reports to Oru on biology adjacent signals; soft spot for Ryn's shelter repairs.  
**Arc:** Comic relief with tools → finder of route keys → detector of the coldest signal → moral instrument that refuses target-lock language.

### Sable Rook
**Age band:** Mid-thirties.  
**Appearance:** Pirate negotiator; neat coat that suggests legitimacy without claiming it; smile that arrives on schedule; empty hands shown deliberately.  
**Speech pattern:** Courteous threat. Keeps bargains explicitly. Never pretends trust equals safety.

Sample lines:
1. “Meridian pays well for routes nobody questions. You can sell silence, or keep a conscience.”
2. “I honour refusal. Public data still reaches pirates. Plan for that honesty.”
3. “The Empire never calls a debt a chain. Courtesy is a contract with nicer stationery.”

**Want:** Predictable lanes she can tax without slaughter.  
**Need:** To admit that “honest piracy” still harms the weak.  
**Wound:** A broken bargain got her crew killed by a corporation that preferred martyrs.  
**Lie:** Clear rules make predation ethical.  
**Relationships:** Antagonist-ally to Kei; useful mirror for Lysara; respects Iona's chain-of-custody; never friends with Voss.  
**Arc:** Choice broker in Act I → warning voice through Act II → Imperial courtesy translator → covert route helper if Independent trust is high, still dangerous.

### Cassian Voss
**Age band:** Early fifties.  
**Appearance:** Pale Meridian executive; pale suit; small silver relay pin; immaculate calm; never raises his voice because volume would admit doubt.  
**Speech pattern:** Reasonable coercion. Speaks in reliability, margins, “dependable routes.” Treats deaths as regrettable line items.

Sample lines:
1. “Control created lanes people could schedule their lives around. Chaos is not freedom.”
2. “I am not asking for worship. I am asking for continuity.”
3. “If you open the channel to everyone, prepare to bury the unlucky with better speeches.”

**Want:** Private custody of route certainty.  
**Need:** To face that dependence is not consent.  
**Wound:** Watched a public bureaucracy strand a system for a quarter while committees argued.  
**Lie:** Reliable suffering is preferable to unreliable hope.  
**Relationships:** Antagonist to Kei/Ryn/Iona; tries to buy Nadi; underestimates Pip; can stand down, testify, or flee — story never requires his death.  
**Arc:** Offstage author of forged hazards → presentable face of monopoly → courtroom of public lanes → defeat that can be dignified, cowardly, or cooperative depending on trust.

### Mara
**Age band:** Late thirties.  
**Appearance:** Agricultural-port quartermaster; apron over station blues; practical strength; keeps sealed cases like sleeping animals.  
**Speech pattern:** Grounded, maternal without softness-as-weakness. Protects packages and people with the same stubbornness.

Sample lines:
1. “Ryn left this sealed. I will not open it. Neither should you — not yet.”
2. “A clinic has power cells and no open lane. People die if this stays paperwork.”
3. “Bring the cells, escort the convoy, or clear the buoy. Just don't bring me a speech.”

**Want:** Local settlements kept alive without becoming anyone's PR.  
**Need:** To accept that delivery alone does not fix the map.  
**Wound:** Watched a sealed “hazard” starve a neighbour for three days.  
**Lie:** If she holds the package tightly enough, the lane will forgive them.  
**Relationships:** Bridge between Ryn and Kei early; Act II mission giver for clinic relief; later quiet beneficiary of route choices.  
**Arc:** Prologue keeper of the receiver → Act II conscience of cargo → offstage community voice whose survival measures player methods.

### Federal Envoy Hale
**Age band:** Late forties.  
**Appearance:** Federal diplomatic coat; neat rank tabs; tablet always awake; smile trained for cameras and withdrawn for contracts.  
**Speech pattern:** Efficient, civic, slightly paternal. Frames everything as capability and coverage.

Sample lines:
1. “We can put patrols on that lane by next cycle. Capability is not tyranny.”
2. “Rescue first. Oversight second. The order matters — and so does the second.”
3. “Independent pilots are free to refuse. They are also free to die unassisted.”

**Want:** Operational access packaged as safety.  
**Need:** To hear that surveillance language is part of the offer, not a footnote.  
**Wound:** A Federal delay cost civilian ships while paperwork cleared.  
**Lie:** Speed of response equals moral clarity.  
**Relationships:** Courtship of Tamsin; rivalry with Lysara's patronage model; impatience with Mirek's minutes; professional interest in the player.  
**Arc:** Act III showcase of logistics benefit → pressure during coalition choice → later radio colour reflecting Federal trust.

### Imperial Patron Lysara
**Age band:** Early fifties.  
**Appearance:** Imperial court-adjacent elegance without parade armour; jewellery that doubles as credentials; voice that makes obligations sound like gifts.  
**Speech pattern:** Courteous, layered, never crude. Debt arrives as favour.

Sample lines:
1. “Long-term funding is a form of care. Care expects remembrance.”
2. “Personal accountability means a name answers when the lane fails.”
3. “We do not seize your channel. We endow it — and visit.”

**Want:** Patronage custody of the relay's future.  
**Need:** To admit courtesy can be a chain.  
**Wound:** A client system thrived until the patron's attention moved elsewhere.  
**Lie:** Beautiful responsibility is better than ugly democracy.  
**Relationships:** Sable names the debt aloud; Kei refuses flattery; Voss finds her competition irritatingly refined.  
**Arc:** Act III temptation → coalition option that changes tone and prices → epilogue flavour of Imperial “visits.”

### Alliance Delegate Mirek
**Age band:** Late fifties.  
**Appearance:** Alliance practicals; minutes tablet; tired optimism; ink on fingers from signing too many shared statements.  
**Speech pattern:** Inclusive, slow, allergic to heroics. Offers witnesses rather than certainty.

Sample lines:
1. “Slower decisions. Public minutes. Shared blame. That is the feature.”
2. “We cannot promise a perfect map. We can promise you will see who moved the pins.”
3. “Decentralisation spreads power and responsibility together. Refuse one, lose the other.”

**Want:** A coalition charter with audit trails.  
**Need:** To accept that deliberation can cost lives in the short term.  
**Wound:** Minutes recorded a warning that arrived after the funeral.  
**Lie:** Transparency alone prevents harm.  
**Relationships:** Iona's preferred stress-test subject; Hale finds them slow; Lysara finds them provincial; Ryn finds them usable.  
**Arc:** Act III alternative to empire/federation → audit-trail proof → possible lawful custody path in Who Keeps the Light.

---

## Lore Research Ledger

Summaries are in our words. Sources are cited for designers; campaign dialogue must remain original. **Pale Meridian** and the **Explorers Guild local cell** are ORIGINAL campaign orgs.

### First Thargoid War / mycoid / INRA / Jameson
In Elite Dangerous history, the first major human–Thargoid conflict of the 32nd century ended after a covert military science body—INRA—developed a fungal bioweapon (mycoid) against living Thargoid technology. A celebrated pilot, John Jameson, was tasked to deliver it under incomplete briefings; he succeeded operationally, discovered the weapon's lethality exceeded the story he was sold, and did not return—his ship sabotaged to keep the secret. Abandoned INRA sites and later wreck finds turned the “victory” into a public moral wound: humanity had won a war by making an atrocity efficient.

**Campaign use:** Chapter 10's archive is a *witness ledger*, not a recipe. Mycoid is **never craftable**, never a player weapon, never a research unlock that produces ordinance. Pale Meridian's interest is opportunistic analogy (“organic relays”), which the story refuses.

**Sources (titles/URLs):**
- Elite Dangerous Wiki — “Mycoid”: https://elite-dangerous.fandom.com/wiki/Mycoid
- Elite Dangerous Wiki — “John Jameson”: https://elite-dangerous.fandom.com/wiki/John_Jameson
- Elite Dangerous Wiki — “Intergalactic Naval Reserve Arm”: https://elite-dangerous.fandom.com/wiki/Intergalactic_Naval_Reserve_Arm

### GalCop rise/fall, Lave capital, 3174 dissolution
The Galactic Cooperative rose from Old Worlds trade culture with Lave as a historic capital of mutual defence and commerce. War spending, member poaching by larger powers, and corruption scandals hollowed it until CEO Simmone Hendry rescinded the charter on **19 August 3174**. The collapse impoverished the Old Worlds and, later, helped create political space for the Alliance's rise.

**Campaign use:** Venn's Act II archive is about *responsibility for lanes*—what a seal meant when it meant “everyone,” and what it meant when it meant “nobody.” Nostalgia is allowed; resurrection cosplay is not.

**Sources:**
- Elite Dangerous Wiki — “Galactic Cooperative”: https://elite-dangerous.fandom.com/wiki/Galactic_Cooperative
- Elite Dangerous Wiki — “Old Worlds”: https://elite-dangerous.fandom.com/wiki/Old_Worlds

### Aegis creation / Baumann / dismantling / return
Aegis began (3303) as a tri-superpower body for Thargoid research, defence, and rescue. After disasters and the Baumann Report's findings of organisational failure, it was dismantled (3308). Escalating war and political necessity led to formal reinstatement (announced **13 February 3309**), with reforms acknowledging prior criticism and a renewed cross-power structure.

**Campaign use:** Chapter 20's buoy is an *echo of method*—shared science and rescue as virtues; secrecy and escalation as failure modes—not a lore dump of named disasters. Our story's lesson rhymes without quoting GalNet.

**Sources:**
- Frontier Developments — GalNet “Aegis Returns” (13 Feb 3309): https://www.elitedangerous.com/news/galnet/aegis-returns
- Frontier Developments — GalNet “Aegis Demands Reinstatement”: https://www.elitedangerous.com/news/galnet/aegis-demands-reinstatement
- Frontier Developments — GalNet “Tesreau Announces Bid to Reform Aegis”: https://www.elitedangerous.com/news/galnet/tesreau-announces-bid-reform-aegis

### Azimuth / Salvation / Proteus Wave / Titans (high level)
Azimuth (in successive corporate skins) pursued anti-Thargoid supremacy through Guardian-derived weapons under the figure known as Salvation. The Proteus Wave project at HIP 22460 sought a decisive purge; activation produced catastrophe for human forces and empowered a new phase of Thargoid presence, including capital-scale Titans. The episode is a cautionary monument: “end the threat forever” as a sales pitch for monopoly science.

**Campaign use:** Background weather only. Open Channel does not restage HIP 22460. Voss's rhetoric of “dependable control” rhymes with Salvation-era certainty without naming him in Act II dialogue unless a later optional Codex beat requires it.

**Sources:**
- Frontier Developments — GalNet “A Retrospective of 3308 (Part Two)”: https://www.elitedangerous.com/en-US/news/galnet/retrospective-3308-part-two
- Frontier Developments — GalNet “Proteus Wave Approaches Completion”: https://www.elitedangerous.com/news/galnet/proteus-wave-approaches-completion
- Frontier Developments — GalNet “Azimuth Refocuses on Titan Research”: https://www.elitedangerous.com/news/galnet/azimuth-refocuses-titan-research

### Guardians as ancient Thargoid adversaries
Archaeological and Guardian-site evidence in the setting establishes an ancient war between Guardians and Thargoids long before human spaceflight. Guardian technology is powerful, poorly understood by most humans, and historically catastrophic when weaponised without humility.

**Campaign use:** Optional Codex flavour; Oru may note that “older wars leave tools lying around for newer fools.” No Guardian weapon crafting in Open Channel.

**Sources:**
- Community research digest context — Canonn “Galnet Digest: The Complete Azimuth Saga”: https://canonn.science/uncategorised/galnet-digest-the-complete-azimuth-saga/
- (Designers may also consult in-game Guardian Codex entries; do not paste Codex text into scripts.)

### Federation / Empire / Alliance character in diplomacy
- **Federation:** Speed, coverage, logistics, and surveillance bundled as civic duty.  
- **Empire:** Personal patronage, long funding, obligations framed as honour.  
- **Alliance:** Coalition process, public minutes, slower decisions, shared blame.

**Campaign use:** Chapters 14–16 (after this Act II packet) stage these as *heard cases*, not forced allegiances. Act II plants the need for a later flag choice without resolving it.

**Sources (character of powers; read for tone, do not lift copy):**
- Frontier Developments — GalNet “Federation Pledges Resources to Aegis”: https://www.elitedangerous.com/news/galnet/federation-pledges-resources-aegis
- Elite Dangerous Wiki — “Galactic Cooperative” / Alliance origin notes linked from Old Worlds collapse: https://elite-dangerous.fandom.com/wiki/Old_Worlds

### Originality boundary (state clearly)
**Pale Meridian** (private route-monopoly combine; amplifiers; forged hazards; Director Cassian Voss) and the **Explorers Guild local cell** (Kei's Lave-facing survey desk, not a Frontier faction clone) are **ORIGINAL** ELITE: NEXT campaign inventions. They sit *against* established history; they do not replace it.

---

## Act II Screenplay — Chapters 08–13 (Saga 6–11)

### 08 — NO CARGO IS JUST CARGO
**SETTING:** Outbound from Lave toward an agricultural clinic settlement whose published lane shows “CLOSED — UNSTABLE LANE.” Dockside cold light; Mara on open channel; Tamsin's convoy holding at the buoy line; interference hash on the nav strip.  
**MISSION:** Reach the isolated settlement. Method options (all keep the clinic alive): carry power cells, escort Tamsin, or clear the interference buoy. Method shifts Independent vs Law vs Guild confidence later.

**OPTIONAL BEATS:** Scan the buoy's manufacturer seal (Pale Meridian subcontract). Deliver without weapons fire for Independent trust. Allow Iona's remote warrant check if the player requests lawful cover.

**DIALOGUE**

1. **MARA:** “Clinic's on emergency batteries. The map says the lane is closed. The map is lying with a company accent.”  
2. **KEI:** “We need power cells on their pad before the night cycle. Prefer a clean dock. Accept a messy one.”  
3. **PLAYER:** “How messy is legal?”  
4. **IONA (comm):** “If you clear a buoy, log the serial. If you escort, log the times. If you only carry, still log the times. I eat timestamps for breakfast.”  
5. **TAMSIN:** “My tender's already queued. I'll take an escort if you've got teeth. I'll take cargo space if you've got manners.”  
6. **PLAYER:** “What's on the buoy?”  
7. **NADI:** “Interference transmitter dressed as a hazard beacon. Meridian subcontract plate. Pip wants to unscrew it politely.”  
8. **PIP:** “Polite unscrew: yes. Explosive unscrew: statistically louder.”  
9. **VENN:** “Lave clears you outbound. Bring me a story that matches your flight recorder. I am old-fashioned that way.”  
10. **SABLE (optional intercept):** “If you open that lane, my people will use it too. That is not a threat. That is weather.”  
11. **KEI:** “Sable, not now.”  
12. **SABLE:** “Now is when clinics die, Coordinator. Call me weather again when you're warm.”  
13. **MARA:** “Commander — cells, escort, or buoy. Pick a verb. People are not a debate club.”  
14. **PLAYER (carry path):** “Loading cells. Keep the dock lights on.”  
15. **MARA:** “Bless your hold. Curse anyone who seals it.”  
16. **PLAYER (escort path):** “Tamsin, form on my wing. No hero runs.”  
17. **TAMSIN:** “Hero runs are how I lost the last one. Matching speed.”  
18. **PLAYER (buoy path):** “Pip, mark the transmitter. Nadi, I want the plate intact.”  
19. **NADI:** “Intact plates make Iona affectionate. Standing by.”  
20. **CLINIC DISPATCH (on arrival):** “Lane open. Batteries kissing mains. Whoever you are — dock fees waived, pride not.”  

**OPTIONAL CLOSING:** If buoy cleared, Iona files a premature-hazard complaint. If escorted, Independent chatter praises the commander. If carried only, Mara sends a quiet thank-you and a cheaper fuel rate next visit.

---

### 09 — GHOSTS OF GALCOP
**SETTING:** Beneath Lave's old traffic network — a maintenance archive Venn still has keys for. Dust; sealed cabinets; a chain of abandoned beacons still answering intermittent polls like ghosts on a switchboard.  
**MISSION:** Inspect the abandoned beacon chain (scan objective). Learn that institutions fail when records cannot be challenged — neither nostalgia nor contempt is the lesson.

**OPTIONAL BEATS:** Recover a GalCop mutual-defence clause fragment for Codex. Decline Voss's remote offer to “digitise and host” the archive.

**DIALOGUE**

1. **VENN:** “Welcome to the part of Lave tourists skip. The air tastes like old ink and unfinished apologies.”  
2. **PLAYER:** “You're still authorised here?”  
3. **VENN:** “Authorised is a strong word. Tolerated is accurate. Come.”  
4. **KEI:** “Ryn used to say GalCop's seal meant the road had witnesses.”  
5. **VENN:** “Early on, yes. Near the end, the seal meant the witnesses had been promoted out of honesty.”  
6. **IONA:** “I need chain-of-custody on anything you pull. History is evidence when corporations rewrite maps.”  
7. **PLAYER:** “What am I listening for?”  
8. **VENN:** “Beacons that still answer. Not for poetry. For continuity. Press the poll. Hear who claims the lane.”  
9. **PIP:** “Node three responds. Identity string degraded. Confidence: nostalgic.”  
10. **ORU:** “Nostalgia is a chemical. Do not inhale too deeply.”  
11. **VOSS (cold call, optional):** “Director Voss. Pale Meridian can preserve that archive in redundant vaults. Free of charge. Think of the children.”  
12. **VENN:** “I am thinking of the children. That is why you will not host our memory.”  
13. **VOSS:** “Sentiment is not a backup strategy.”  
14. **VENN:** “Neither is a monopoly with better lighting.”  
15. **PLAYER:** “Beacon four's log shows a route move after departure stamps.”  
16. **IONA:** “Same pattern as Tamsin's convoy. Old trick. New letterhead.”  
17. **KEI:** “So Meridian didn't invent the sin.”  
18. **VENN:** “No. They leased it from history and called it innovation.”  
19. **PLAYER:** “What did the seal mean, the day it died?”  
20. **VENN:** “On the last morning, it meant nobody knew who was responsible — and everyone still stamped the form. Walk the rest of the chain. Then go home before you start saluting ghosts.”  

**OPTIONAL CLOSING:** Player may leave a public annotation on the archive terminal: “Records require challengers.” Venn pretends not to be moved.

---

### 10 — THE MYCOID LEDGER
**SETTING:** A sealed historical annex off a lawful vault world (destination bound at chapter start). Cold room; INRA-era storage language on the crates; Iona present as custodian of process; Kei present as custodian of panic.  
**MISSION:** Secure the sealed historical record. Refuse Meridian's research claim. **Mycoid is NEVER craftable** — UI, inventory, and crafting trees must hard-block any weaponisation path.

**OPTIONAL BEATS:** Read one redacted pilot log (original paraphrase only). Hand physical custody to Iona, Guild peer review, or sealed public museum hold — flavour only; record remains secured either way.

**DIALOGUE**

1. **IONA:** “This box is not treasure. It is a confession with footnotes.”  
2. **KEI:** “Meridian filed a research request forty minutes after our beacon scan hit open nets. That is not curiosity. That is appetite.”  
3. **PLAYER:** “Appetite for what?”  
4. **ORU:** “They see organic relays and remember a fungal weapon that ended a war by making biology the battlefield. They want analogy. Analogy is how atrocities get new stationery.”  
5. **NADI:** “For the record: there is no ‘craft mycoid’ option in any sane workshop. If a UI designer adds one, I will personally recycle their keyboard.”  
6. **PIP:** “Crafting pathway: absent. Good.”  
7. **VOSS (comm):** “Commander. That ledger could prevent another tragedy if studied by serious people. Pale Meridian employs serious people.”  
8. **PLAYER:** “Serious people built it the first time.”  
9. **VOSS:** “And unserious people romanticise inaction. Lives have weight. So do schedules.”  
10. **IONA:** “Schedules do not get a vote on bioweapons. Commander, secure the crate. Do not open the agent protocols.”  
11. **KEI:** “Ryn would say the same. She vanishes to protect living things. She does not manufacture better poisons.”  
12. **PLAYER:** “What's inside that we *can* know?”  
13. **IONA:** “Enough: a weapon sold as a disablement; a pilot denied a return; a victory that taught the wrong lesson. Enough to warn. Not enough to repeat.”  
14. **SABLE (optional):** “If you burn it, someone will reconstruct from ash and rumour. Better a locked ledger than a market for ghosts.”  
15. **PLAYER:** “Sable giving ethics advice?”  
16. **SABLE:** “I give storage advice. Ethics is a side effect.”  
17. **ORU:** “Log this sentence into Codex in our words: humanity once ended a war by infecting a living ship-culture, then buried the pilot with the secret. We will not reenact the burial.”  
18. **VENN (comm):** “If that crate comes through Lave, it rides in a hold that does not advertise. I can provide boring.”  
19. **PLAYER:** “Boring accepted.”  
20. **IONA:** “Custody sealed. Meridian's request stamped DENIED — and dated after the denial, so they cannot invent priority. Welcome to adulthood, Commander.”  

**HARD DESIGN RULE:** No blueprint, no component, no “research complete” that yields mycoid munitions. Meridian NPCs may *ask*; systems must *refuse*.

---

### 11 — WHERE THE GIANTS TURN
**SETTING:** Deep black near a migration corridor. Radio open. Engines cut. Enormous spaceborne organisms as distant luminous shapes — scenery and sound first, threat language forbidden.  
**MISSION:** Observe the migration signal; match three calls; recover Ryn's shelter coordinate embedded in cadence. Scan only. No hunt objective.

**OPTIONAL BEATS:** If the player fires, observation resets (Oru's line). Sit through a full radio bed with no UI pressure for 30–60 seconds once.

**DIALOGUE**

1. **ORU:** “Cut your engines. We are visitors here. Visitors do not rearrange the furniture.”  
2. **PLAYER:** “Are they dangerous?”  
3. **ORU:** “They are large. Large is not the same as hostile. Your weapons will not make you larger — only louder.”  
4. **KEI:** “Ryn said she'd write where words would be searched. She wrote in their turning.”  
5. **PIP:** “Listening. Call one… match incomplete.”  
6. **RYN (pre-recorded, thin):** “If you're hearing this, you're patient. Patience is the password.”  
7. **PLAYER:** “That's her.”  
8. **KEI:** “Don't speak yet. Let the second call land.”  
9. **TAMSIN (comm, quiet):** “Convoy holding far side. Tell your scientist we can wait. We have practiced waiting.”  
10. **ORU:** “Second call. Hear the pause? That pause is grammar.”  
11. **PLAYER:** “And Meridian amplified the grammar until it screamed.”  
12. **ORU:** “Yes. Then sold maps of the silence they created. Third call coming.”  
13. **PIP:** “Third match. Coordinate decrypted. Shelter bearing plotted. Confidence: Ryn.”  
14. **KEI:** “Alive enough to be clever. I'm going to kill her carefully.”  
15. **ORU:** “Log the migration. Leave their road clear. The giants are not a resource node.”  
16. **PLAYER:** “Codex entry?”  
17. **ORU:** “Write: we observed; we did not herd; we did not fire; we found a friend in a rhythm older than our flags.”  
18. **SABLE (distant):** “Pretty speech. Clear lane behind you is prettier. Moving my people off your sky.”  
19. **KEI:** “Thank you — temporarily.”  
20. **ORU:** “Go to her. And remember: the first useful act was making room.”  

---

### 12 — THE PERSON ON THE OTHER END
**SETTING:** Ryn's improvised shelter — a scarred relay tender hull half-buried in ice-rock, amplifier dish limping on the ridge. Interior: foil blankets, jury-rigged scrubbers, a mug glued to the table. Kei on open channel until local range; then voice in the room.  
**MISSION:** Reach Ryn's shelter; repair life support enough to last; disable the local amplifier. Reunion is relieved, angry, affectionate. Ryn becomes a recurring contact, not a reward object.

**OPTIONAL BEATS:** Player may ask Ryn to board or remain as field contact — either keeps her alive and available by comm. Combat optional against a Meridian watcher drone; destruction not required if jammed.

**DIALOGUE**

1. **RYN (comm):** “I'm alive. The clever explanation can wait.”  
2. **KEI:** “Start with ‘I'm alive’ again. I collect them now.”  
3. **RYN:** “I'm alive. Also cold. Also furious. Also slightly proud of my handwriting in whale-song.”  
4. **PLAYER:** “Docking. Don't shoot the friendly idiot.”  
5. **RYN:** “I only shoot idiots who bring amplifiers. You brought a person. Acceptable.”  
6. *(airlock cycles; shelter ambience)*  
7. **KEI:** “You vanished on three scheduled calls.”  
8. **RYN:** “I vanished so Meridian would chase a ghost instead of a migration. It worked until it didn't.”  
9. **KEI:** “You don't get to decide alone who pays for your ethics.”  
10. **RYN:** “You don't get to decide alone who I endanger by coming home loud.”  
11. **PLAYER:** “Both of you can be right after we fix the scrubber.”  
12. **PIP:** “Scrubber: dying. Amplifier: rude. Priority: rude first?”  
13. **RYN:** “Rude first. If that dish keeps shouting, the giants turn into traffic hazards and Meridian sells the detour.”  
14. **NADI (comm):** “Remote patch packet sent. Bolt the coupling like you mean it.”  
15. **PLAYER:** “Amplifier offline.”  
16. **RYN:** “Good. Listen — empty space. That's what consent sounds like when the universe is big.”  
17. **KEI:** “I hate that you're poetic when I'm angry.”  
18. **RYN:** “I hate that you're careful when I'm scared. Come here anyway.”  
19. *(beat: Kei's breath; Ryn's laugh that breaks)*  
20. **RYN:** “Commander. Thank you for not turning me into a mission complete screen. Stay for tea if your plot allows. Then we decide what we carry home — and how loudly.”  

**OPTIONAL BEAT:** If Kei trust high, she admits: “I asked the Guild to flag you missing as equipment loss. I am not proud.” Ryn: “Equipment loss? I'll invoice you.”

---

### 13 — WHAT WE CARRY HOME
**SETTING:** Lave System Hub — briefing room off Venn's traffic floor. Present: Kei, Ryn, Iona, Oru; Tamsin on speaker; Voss attempting a polite intrusion that Venn mutes. Wall display shows three release modes.  
**MISSION:** Persistent choice — how openly to publish Ryn's evidence. **THREE OUTCOMES.** No choice blocks Act III; each changes support, radio colour, prices, and legal teeth.

**CHOICE A — BROADCAST NOW**  
Public: ↑↑  Guild: ↑  Law: →/↓  Independent: ↑  
Settlements prepare immediately; Meridian alerted; later legal cases harder; more civilian helpers early Act III.

**CHOICE B — VERIFY FIRST**  
Public: →  Guild: ↑  Law: ↑↑  Independent: →  
Stronger later warrants and Iona's coalition leverage; delayed route warnings; Meridian gets time to spin.

**CHOICE C — LIMITED ALERT**  
Public: ↓  Guild: →  Law: ↑  Independent: ↑  
Active hazard revealed to affected lanes only; safer operationally; earns less public trust; quieter Meridian counterplay.

#### Shared opening dialogue

1. **VENN:** “Berth six is warm. The galaxy is not. Speak carefully.”  
2. **RYN:** “We have proof Meridian amplified biological navigation and sold the clear lanes. We have names, times, and a dish that still smells like arrogance.”  
3. **KEI:** “Publish now and settlements prepare — loudly. Verify first and the law grows teeth later. Limited alert is quieter and earns less trust.”  
4. **IONA:** “I can work with any of the three. I cannot work with a myth. Choose the shape of the truth.”  
5. **ORU:** “Whatever you choose, keep uncertainty where it belongs. Do not sell omniscience.”  
6. **TAMSIN (speaker):** “My crews need warnings more than poetry. But wrong warnings get people killed too.”  
7. **VOSS (muted, still somehow audible):** “Commander, responsible disclosure—”  
8. **VENN:** “Muted. Continue.”  
9. **PLAYER:** “If we broadcast now?”  

#### Outcome A — Broadcast now

10A. **KEI:** “Then we light the board. Every clinic, convoy, and gossip channel gets the map.”  
11A. **RYN:** “Meridian will know we have them. Good. Let them feel observed.”  
12A. **IONA:** “I'll preserve originals, but juries hate stampedes. You've chosen people over prosecutions.”  
13A. **ORU:** “People preparing is a form of observation that saves blood.”  
14A. **TAMSIN:** “I'll take loud and early. Fuel tenders are already spooling.”  
15A. **SABLE (comm):** “Public data reaches pirates. Congratulation: weather incoming. I'll keep my weather polite if you keep your bargain sense.”  
16A. **PLAYER:** “Broadcast it.”  
17A. **PIP:** “Packet away. Mirror count: rising. Meridian legal bots: also rising.”  
18A. **VOSS (breaking mute):** “You have mistaken theatre for governance.”  
19A. **RYN:** “You mistook a toll booth for a lighthouse.”  
20A. **VENN:** “Lane notices updating galaxy-wide. May the gods of scheduling forgive us.”  

**Outcome A epilogue sting:** Radio news praises the commander; Meridian issues a denial before breakfast; Act III opens with civilian tenders already in place.

#### Outcome B — Verify first

10B. **IONA:** “Then it comes to my desk, Guild peer review, and a boring vault. We build a chain Meridian cannot saw through.”  
11B. **KEI:** “Settlements wait. I hate waiting. I hate brittle cases more.”  
12B. **RYN:** “Every hour we wait, someone buys a ‘safe’ route that isn't. Own that.”  
13B. **ORU:** “We can issue a soft advisory without the full ledger — but that is almost Choice C.”  
14B. **PLAYER:** “No halfway. Verify. Full teeth later.”  
15B. **TAMSIN:** “I'll hold my convoys on old lanes. Costly. Survivable.”  
16B. **VENN:** “I'll stamp provisional holds without publishing causes. Traffic will complain. Traffic always complains.”  
17B. **SABLE:** “Quiet rooms favour people with lawyers. Try not to become one of Meridian's lawyers.”  
18B. **PIP:** “Evidence sealed. Hash published. Contents: not.”  
19B. **VOSS:** “Wise. Adults verify. Children shout.”  
20B. **IONA:** “Adults also prosecute. Enjoy your temporary compliment, Director.”  

**Outcome B epilogue sting:** Fewer immediate helpers; Iona later produces warrants that snap; Act III Federal/Alliance scenes gain sharper leverage.

#### Outcome C — Limited alert

10C. **KEI:** “Affected lanes only. Hazard true. Motive redacted. Public trust takes a bruise.”  
11C. **RYN:** “It's the cowardice I can live with. Barely.”  
12C. **IONA:** “It's also the version that keeps witnesses alive long enough to testify.”  
13C. **ORU:** “A narrow truth is still a truth. Do not let it become a habit.”  
14C. **PLAYER:** “Limited alert. Active hazard only.”  
15C. **TAMSIN:** “Send me the list. I'll reroute the names that matter tonight.”  
16C. **PIP:** “Audience: constrained. Leak probability: nonzero. Pip dislikes nonzero.”  
17C. **SABLE:** “Smaller audience means my competitors pay more for rumour. Amusing. Don't die.”  
18C. **VENN:** “Notices pushed to marked systems. Lave stays quiet. Quiet is a kind of berth.”  
19C. **VOSS:** “Containment. Almost professional.”  
20C. **RYN:** “Containment is how you sleep. Open is how you wake. We'll wake later.”  

**Outcome C epilogue sting:** Independent pilots tip well; public radio cooler; Meridian slower to full counter-offensive; Act III starts with fewer crowds and cleaner ops.

---

## Theme Essay — Routes, Observation, Records

The Open Channel is not a story about finding a missing person, though it wears that coat in Act I. It is a story about **who is allowed to define a road**.

A route looks like neutral infrastructure: a line on a chart, a buoy, a schedule. In practice it is power. Whoever can mark a lane “safe” or “closed” decides which clinics keep their lights, which convoys keep their tenders, which migrations remain grammar instead of product. Pale Meridian's crime is not merely greed; it is the privatisation of certainty. They amplify a living signal until the sky hurts, then sell relief from the pain they manufactured. That is empire by navigation—an older human habit wearing a modern pin.

Against that stands a second theme: **observation versus ownership**. Oru's chapters insist that seeing is not the same as seizing. The giants turn without needing a flag. Thargoid echoes, when they come later, must remain non-targets—recorded, not reduced. The campaign's refusal to make mycoid craftable is the same ethic in a harder key: some knowledge exists to forbid repetition, not to complete a tech tree. INRA's historical sin, in our telling, is the moment observation became instrument and instrument became atrocity. Jameson's denied return is the narrative scar that says: if your victory requires silencing the witness, you did not win a peace worth scheduling.

Third: **institutions and records**. GalCop's ghost is not a plea to rebuild a logo. It is a warning about seals—symbols that once meant “the lane belongs to everyone” and later meant “nobody is responsible.” Aegis's dismantling and return, in the wider Elite century, rhymes with the same lesson: shared instruments fail when secrecy outruns rescue, and return only matters if structure changes. Our Alliance minutes, Federal coverage, and Imperial patronage are not costume changes; they are competing theories of custody. Iona's demand for timestamps is the prosaic heart of the epic. Freedom without records becomes rumour. Records without challengers become propaganda. The player's four persistent choices are custody decisions: who holds the ledger, how loud the warning, which coalition approaches the relay, who keeps the light under inspection.

Kei and Ryn enact the intimate version of the thesis. Kei's care tilts toward control; Ryn's ethics tilt toward disappearance. Neither is villainy. Both become dangerous when they refuse witnesses. The player is the third point of the triangle: the person who can carry packages, clear buoys, and refuse to turn a friend into a completion screen. Home, in the final berth, is not a reward chest. It is what happens when a lane stays open long enough for someone to return without paying a toll for breath.

Movie-quality, on a PSP, means sentences that can survive forty-three characters when needed and still sing in the design bible at full length. It means antagonists who sound reasonable. It means jokes that arrive from Pip like truth with a status light. It means the channel opens not because humanity deserves the giants, but because dependence is not consent—and empty space, sometimes, is the most honest answer we can leave.

---

*End of Act II design packet (characters + lore ledger + ch.08–13 + theme).*


---

# PART B — Prologue & Act I (Chapters 01–07)

> Source draft: Act I feature screenplay.

## The Open Channel — Feature Screenplay (Prologue & Act I)

**Game:** ELITE: NEXT  
**Campaign:** The Open Channel  
**Scope:** Chapter 01 (Prologue) through Chapter 07 (saga indices 0–5)  
**Tone:** Working pilots, moral ambiguity, intrigue without purple prose  
**Lore note:** Original dialogue set against Elite history (Lave / GalCop collapse, post–Titan war ~3311). Pale Meridian and all scenes are fictional. Thargoids appear only as distant mystery.

---

## Prologue — First Flight (Chapter 01)

**TITLE:** First Flight  
**LOGLINE:** A new commander learns the ship at Lave Hub under Guild coordinator Kei Aven, while traffic controller Venn Calder keeps the bay honest.  
**SETTING:** Lave System Hub — berth corridor, then tower channel. Year ~3311. Independent traffic, old GalCop paint still visible under newer seals.  
**MISSION VERBS:** Launch → lock station → fly out ~600 m → return and dock.  
**RECOVERY:** If lost, Venn re-clears the pad; Kei reopens Tracked Mission with the same objective. Death / suspend preserves prologue stage.

### FULL SCENE — Lave Hub (Kei + Venn)

*(Berth six. The ship sits cold. A Guild badge reader blinks amber. KEI AVEN stands at the hatch with a slate; VENN CALDER is on the overhead speaker, then steps into the corridor with a coffee he will not finish.)*

**KEI:** Commander. Kei Aven, Explorers Guild. This berth is assigned until you prove you can leave it and come back.

**VENN:** Lave Tower. Calder. If you're on my channel, say so. I don't shout into empty air.

**PLAYER:** Tower, this is—

**VENN:** Good. Name later. Callsign now. Keep it short.

**KEI:** You can use the Guild temporary. Or invent something that fits on a status line. Either way, Venn logs it.

**VENN:** And if you invent poetry, I shorten it. Poetry blocks the board.

**KEI:** *(to player, precise)* Controls first. Pitch and yaw with the stick. Throttle is not courage. Throttle is distance over time. You will launch, clear the pad, lock the station, fly out six hundred metres, then return on the same vector. No shortcuts through the sun.

**VENN:** Also no “practice fire” at the docking collar. People try that. People get towed.

**PLAYER:** Understood.

**KEI:** Say it like you mean the checklist, not like you're agreeing to a rumour.

**PLAYER:** Launch. Lock. Six hundred. Return. Dock.

**KEI:** Better. If your fuel reads empty before you leave the bay, you skipped a pump. Ask the deck. Do not invent a solution with engines.

**VENN:** Deck pumps are on the left of the ramp. The right is scrap. Scrap looks helpful until it isn't.

*(A soft chime. Departure window opens.)*

**VENN:** You're cleared. Soft launch. If you graze the ring, I will know before you do.

**KEI:** I'll stay on Guild short-range. If you panic, over-explain to me, not to the tower. Venn has enough grief without your narrative.

**VENN:** *(dry)* I heard that.

**KEI:** You were meant to.

*(Player launches. Space opens. Station lock tone.)*

**KEI:** Lock acquired. Good. Now push out. Watch your relative. If the station grows instead of shrinks, you turned around while congratulating yourself.

**VENN:** Traffic note: freighter on your three is sleeping. Sleeping freighters still have mass.

**PLAYER:** Crossing six hundred.

**KEI:** Hold. Breathe. Turning home is not a failure. Turning home is the point.

*(Return approach.)*

**VENN:** Align. Speed down. If your approach looks like a dare, abort and try again. I prefer boring pilots.

**KEI:** Docking collar will catch you if you're honest. If you're clever, it won't.

*(Hard dock. Bay pressure equalises.)*

**VENN:** Pad secure. Welcome back, such as it is.

**KEI:** Harbour badge is yours. Not a medal. A receipt: you left and you returned. That is the only skill that scales.

**VENN:** Berth six stays warm for one more lesson set. After that, the Guild starts asking for distances that matter.

**KEI:** When you're ready, open Tracked Mission. We have a sealed case waiting at an agricultural dock. Ryn Vale left it. She missed three calls. That is… not her pattern. I will explain more when your hands are not still shaking from the stick.

**VENN:** *(quietly, almost to himself)* People who never miss calls are the ones who leave the worst silence.

**KEI:** Venn.

**VENN:** Procedure. I'm fine. Commander — fuel, repair, then sleep if you need it. The lane will still be wrong tomorrow.

**KEI:** Next step when you accept: deliver Mara's sealed receiver. Until then, walk the deck. Learn where the doors are. I talk too much when I'm scared. Today I am… professionally concerned. That is close enough.

### OPTIONAL SIDE BEATS

- Dockhand offers a fuel top-up and a joke about first launches; Venn cuts the joke short if it blocks the channel.
- If player opens Comfort / Help mid-prologue: Kei repeats the four verbs, no new lore.

### BRANCH NOTES

None. Linear training. Completing the badge unlocks Open Channel Chapter 02 (saga index 0).

---

## Act I — A place to return to

---

## 02. Something Worth Carrying

**SAGA INDEX:** 0  
**LOGLINE:** Mara holds a sealed receiver Ryn left behind; the player carries it to Lave while a forged hazard notice tries to herd traffic onto Pale Meridian's preferred lanes.  
**SETTING:** Agricultural dock (outsystem) → jump corridor → Lave Hub intake.  
**MISSION VERBS:** Dock at Mara's port → collect sealed case (flagged evidence, not ordinary cargo) → home dock at Lave.  
**RECOVERY:** If case not collected, Mara holds it indefinitely. If destroyed en route, chapter soft-fails to “re-collect duplicate seal hash from Mara's backup stamp” (same dock). Forged hazard can be ignored; flying the Meridian “safe” detour still completes delivery but flags later trust tone.

### FULL SCENE SCRIPT

*(Agricultural dock. Grease, grain dust, a hand-painted berth number. MARA meets the player at the cargo cage. KEI on radio.)*

**MARA:** You're Guild? Or just the person Kei trusts with sealed things?

**KEI:** *(radio)* Both, if we're lucky. Mara, is the case still sealed?

**MARA:** Sealed. Cold. And I didn't open it, before you ask in that careful voice.

**KEI:** I was going to ask in a careful voice.

**MARA:** Ryn left it with a note that said “if I miss three, give this to someone who still answers.” She smiled when she wrote it. I hated that smile.

**PLAYER:** What's inside?

**MARA:** A receiver. Old survey kit, patched. She said it listens better than it talks. That's all I get, and all you get, until Kei says otherwise.

**KEI:** Do not open it in flight. Do not open it for curiosity. Curiosity is how evidence becomes a story you can't prove.

**MARA:** Also: there's a hazard notice on your outbound. Fresh paint. Looks official. Smells like someone wants you on a longer road.

**KEI:** I see it. Timestamp is wrong by eleven minutes against the public bulletin. Either a clerk failed, or someone published early on purpose.

**MARA:** Pale Meridian's been selling “clear lanes” since the Titans went quiet. Clear for who?

**KEI:** For people who pay. Carry the case. Ignore the detour if your tank allows. If you take it, note why. We may need the why later.

**MARA:** Hands. *(passes the case)* If anyone asks, you collected agricultural sensors. Boring lies travel further.

**PLAYER:** I'll get it to Lave.

**MARA:** Get yourself to Lave too. Cases don't dock themselves.

*(Jump. Mid-corridor: KEI again.)*

**KEI:** Ryn never missed a call. She missed meals, permits, once an entire moon survey window because she was drawing something with too many legs. Never a call. Three misses means she chose silence, or silence chose her. I am… I am listing possibilities because listing is how I keep my voice level.

**PLAYER:** We'll find her.

**KEI:** First we deliver what she trusted to a dock worker with better instincts than half the Guild board. Then we listen to the pauses on that box. Machine timing is regular. Life isn't.

*(Lave intake. VENN.)*

**VENN:** Incoming Guild packet. Seal intact?

**PLAYER:** Intact.

**VENN:** Then it goes to evidence cage three, not general cargo. I still remember how to label a thing so it doesn't walk away. Habits from when the seal meant the lane belonged to everyone.

**KEI:** Thank you, Venn.

**VENN:** Don't thank me for procedure. Thank me when I break it for a good reason. Today is procedure day.

**KEI:** Commander — claim the chapter when you're docked. Next, Oru wants a comparison. Living signal against this box. Quiet work. Harder than it sounds.

### OPTIONAL SIDE BEATS

- Scan the forged hazard bulletin: reveals Meridian relay imprint in the footer metadata.
- Buy grain from Mara; she undercharges “for Ryn's friend,” then corrects: “for Ryn's errand.”

### BRANCH NOTES

Detour taken → later radio: Meridian ads feel personal. Detour refused → Sable mentions “you don't spook easy” in Ch.07.

---

## 03. The Quiet Between Pings

**SAGA INDEX:** 1  
**LOGLINE:** Xenobiologist Oru Sen teaches that migration calls live in the gaps between navigation bursts; a scan must be gentle or the observation resets.  
**SETTING:** Marked system — sparse traffic, bioluminescent “sky-fish” / spaceborne fauna on long elliptical paths (visual only; not combat targets).  
**MISSION VERBS:** Arrive → approach cool → scan marked ping → withdraw.  
**RECOVERY:** Hot approach or weapons fire resets observation progress; objective remains. Leaving system preserves chapter; return to marked ping.

### FULL SCENE SCRIPT

*(Local space. ORU SEN on science channel; KEI listens in.)*

**ORU:** Cut your relative. We are not hunting. We are borrowing silence.

**KEI:** Oru, the commander has a sealed receiver from Ryn. We need a living comparison.

**ORU:** Then do not name the living thing a threat before it has finished speaking. People get loud after the Titans. Loud is understandable. Loud is still noise.

**PLAYER:** What am I scanning?

**ORU:** A migration call. It sits in the pauses between the navigation pings your charts worship. The charts hear the ticks. The animals answer the rests.

**KEI:** Ryn's receiver had irregular gaps. I thought it was damage.

**ORU:** Damage is regular in its own ugly way. This is phrasing.

*(Fauna pass at distance — slow, vast, indifferent.)*

**ORU:** If you paint them with lasers, they will leave, and your scan will be a story about your impatience. Approach too hot and the observation resets. The universe is not obliged to be convenient for Guild schedules.

**PLAYER:** Holding cold.

**ORU:** Good. Match their drift. When the ping sings, wait. When it stops, listen. Your scanner should catch a structure in the quiet — not a weapon signature, not a distress. A road-song.

**KEI:** *(soft)* She used to hum when she logged routes. I thought it was nerves.

**ORU:** Perhaps it was translation practice.

*(Scan completes.)*

**ORU:** There. The pause pattern matches your receiver within tolerance. Someone has been amplifying the biological lane markers and selling the “clear” path as a product.

**KEI:** Pale Meridian.

**ORU:** I did not say the company name. You did. Useful. Names make lawsuits. Observations make maps.

**PLAYER:** Are they hurting the animals?

**ORU:** Amplification is a kind of shouting into a conversation. Whether it hurts depends on the throat. I will not claim pain I have not measured. I will claim interference I have.

**KEI:** Next steps mean people. Convoys follow published safe routes. If the route moves after departure—

**ORU:** —then accounting for hands becomes a moral problem, not a navigation one. Go. Leave them the sky. Your engines are a weather system they did not request.

**KEI:** Commander, return when the log is clean. And… thank you for not shooting the lesson.

### OPTIONAL SIDE BEATS

- Secondary scan of a dead ping buoy: Meridian firmware stub, wiped incompletely.
- If player fires once: Oru's line hardens; Independent trust soft penalty; scan still completable after reset.

### BRANCH NOTES

No story fork. Tone flag if weapons discharged.

---

## 04. All Hands Accounted For

**SAGA INDEX:** 2  
**LOGLINE:** Convoy captain Tamsin Reed followed a newly published safe route and lost a tender; the player finds the last port stamp and optional lifeboat pods before the record is scrubbed.  
**SETTING:** Last outbound port (independent station) → local search grid for pods.  
**MISSION VERBS:** Dock / recover outbound stamp → (optional) locate pods → confirm account.  
**RECOVERY:** Stamp is authored evidence; if player leaves early, stamp remains until collected. Pods are optional; missing pods still advance chapter with reduced Independent trust.

### FULL SCENE SCRIPT

*(Station ops loft. TAMSIN REED, tired, helmet under one arm. Station clerk nervous in background.)*

**TAMSIN:** You're the Guild errand with the quiet animals and the loud company. Fine. I need a place, not a tidy rumour.

**KEI:** *(radio)* Tamsin Reed, convoy *Lower Light*. We read your loss report.

**TAMSIN:** Loss report. Soft words. We followed a route that went green the morning we left. Mid-leg, the tender dropped off grid. No mayday. Just a clean absence, like someone edited the sky.

**PLAYER:** Last port?

**TAMSIN:** Here. Outbound stamp still exists if nobody has been paid to lose it. I already shouted. Shouting doesn't recover telemetry.

**CLERK:** The stamp is in the archive cage. Pale Meridian auditors asked for a copy yesterday. They were… polite.

**TAMSIN:** Polite is a threat with better shoes.

**KEI:** Commander, get the original stamp before it becomes a summary. Summaries forget names.

*(Player retrieves stamp. Data: route revision published 40 minutes after convoy departure.)*

**TAMSIN:** There. They moved the “safe” line after we were already committed. Our tender rode the old coordinates into a shear they knew about.

**PLAYER:** Survivors?

**TAMSIN:** Lifeboat beacons flickered once. Then Meridian traffic advisories filled the band with “debris caution” noise. Convenient static.

**KEI:** Optional work: each pod you mark steadies trust with people who fly for a living. Not a requirement. I will not make grief into a checklist trap.

**TAMSIN:** If you find them, you bring me counts. If you don't, you still bring me the stamp. I can fight a company with a timestamp. I can't fight a shrug.

*(If pods recovered — short insert:)*

**TAMSIN:** Four alive. One quiet. Quiet isn't always dead — sometimes it's a med bay that won't open on open channel. I'll take quiet. Thank you. I don't say that for free.

*(If pods missed:)*

**TAMSIN:** Then we fly angry and careful. Anger without evidence is just weather. You gave me evidence.

**KEI:** Dock when you can. Iona Marr is about to become interested in your cargo history. That is not a threat from us. It is a warning about paper.

### OPTIONAL SIDE BEATS

- Buy spare life support from Tamsin's remaining crew; they refuse charity, accept fair trade.
- Sable Rook overheard on a side channel offering “retrieval rates” — declined by Tamsin.

### BRANCH NOTES

Pods all recovered → Independent trust up; Tamsin aids later blockade chapter. Partial/none → chapter still completes.

---

## 05. A Lawful Mistake

**SAGA INDEX:** 3  
**LOGLINE:** Law officer Iona Marr stops the player on a Pale Meridian theft report for Ryn's receiver; timestamps prove the report predates the alleged theft.  
**SETTING:** Lave approach / inspection dock.  
**MISSION VERBS:** Submit to inspection **or** pay under protest **or** present timestamps; return records intact to Iona (home).  
**RECOVERY:** All paths preserve evidence flags. Refusing both inspection and payment loops a soft warn, then allows timestamp compare as third door. Warrant for this incident clears on completion.

### FULL SCENE SCRIPT

*(Inspection bay. IONA MARR, no theatre. MERIDIAN LIAISON on a secondary screen.)*

**IONA:** Hold position. Local law. I'm Officer Marr. You're carrying a sealed receiver reported stolen by Pale Meridian as of… *(checks)* …two days before you collected it from an agricultural dock. Explain that arithmetic without poetry.

**PLAYER:** The report's early.

**IONA:** Good start. Early reports are either prophecy or paperwork weapons.

**LIAISON:** The asset belongs to Pale Meridian research logistics. We filed promptly when the surveyor absconded.

**KEI:** *(radio)* Ryn Vale did not “abscond.” She missed calls. There is a difference between flight and disappearance.

**LIAISON:** Internal distinction. Externally, property is property.

**IONA:** Externally, I need chain of custody. Commander: you can submit to full inspection, pay the release fee under protest, or put the timestamps on my desk and let me be inconvenient in an official way.

**PLAYER:** Timestamps.

*(Compare: Mara's seal time, player's collection log, Meridian theft filing.)*

**IONA:** Filing predates collection. Predates Mara's seal note. Predates, if I'm rude, the existence of a crime.

**LIAISON:** Clerical—

**IONA:** Say “clerical” again and I open an audit on your last twelve filings. A badge is not evidence. Mine included. Yours especially.

**KEI:** We keep the receiver.

**IONA:** You keep it as evidence under my temporary seal, not as loot. If Meridian wants it, they can request it through a court that still remembers how clocks work.

**LIAISON:** Director Voss will hear—

**IONA:** Director Voss can hear the time. Time is hard to bribe.

*(Liaison drops. Bay lights normalise.)*

**IONA:** You're free to dock. Bring me the comparison packet on the surface. I will stand with you if this grows teeth. I will not stand with you if you start shooting clerks to feel righteous.

**PLAYER:** Understood.

**IONA:** People say that. Then they meet a pirate who keeps bargains and a company that keeps lawyers. We'll see which lesson sticks.

**KEI:** Thank you, Iona.

**IONA:** Thank the clocks. And tell your missing surveyor, if she can hear you, that vanishing to protect people still leaves a mess for the living.

**KEI:** She knows. That's why she does it. I hate that sentence.

### OPTIONAL SIDE BEATS

- Pay-under-protest path: same reveal, Meridian gets a fee, Iona annotates “coerced.”
- Full inspection: short wait; Pip (remote) jokes that inspectors never look in the right panel.

### BRANCH NOTES

Path chosen is flavour + minor Law trust. Evidence always survives. No combat required.

---

## 06. The Last Useful Thing

**SAGA INDEX:** 4  
**LOGLINE:** Salvage engineer Nadi Quill and drone Pip pull a clean dump from Ryn's damaged relay tender: Pale Meridian amplified a biological navigation signal and sold the clear lanes; a route key hides in a maintenance checklist.  
**SETTING:** Wreck site — relay tender adrift, scavenger risk on sensors.  
**MISSION VERBS:** Approach → scan / dump recorder → extract → leave (do not linger for scrap).  
**RECOVERY:** If scavengers strip before scan, Nadi provides a secondary “cold mirror” buoy Ryn left; same objective verb. Combat optional; fleeing preserves mission if dump complete.

### FULL SCENE SCRIPT

*(Wreck on scopes. NADI QUILL in a patched suit on local; PIP, drone, flat affect.)*

**NADI:** There she is. Ryn's tender. Looks like it kissed a relay the wrong way and apologised with its hull.

**PIP:** Structural integrity: negotiable. Recorder rack: present. Scavenger signatures: approaching on a polite delay.

**KEI:** *(radio)* Get a clean dump. Do not turn this into a scrap holiday.

**NADI:** Kei, I only steal from the dead when the dead left instructions. Ryn left instructions. Mostly in the form of stubbornness.

**PLAYER:** Scanning.

**PIP:** Dump in progress. Noise floor elevated. Pattern match: biological migration carrier under navigation burst schedule. Amplification factor: commercially useful. Ethically: not my department.

**NADI:** Ethically is everyone's department, Pip. You just don't get paid in it.

**PIP:** Correct. I get paid in power cells and lack of contradiction.

*(Recorder yields unedited audio — VOSS, filtered, and an engineer.)*

**VOSS:** *(recording)* If the animals mark the lane, we mark the product. Reliability is a public good. Public goods need stewards who can invoice.

**ENGINEER:** *(recording)* The organisms are adjusting. Stress indicators—

**VOSS:** *(recording)* Then adjust the gain. Settlements need predictable jumps. Predictability is mercy with a spreadsheet.

**KEI:** Turn it off. I have the shape. Cassian Voss, making mercy sound like a subscription.

**NADI:** Wait — Pip, the checklist.

**PIP:** Maintenance checklist item seventeen: “recalibrate audience.” Mislabel. Embedded route key. Classic. Hide the important thing where clerks are bored.

**NADI:** Ryn always did that. Put truth in the dull file.

**PLAYER:** Key recovered.

**PIP:** Recommendation: withdraw. Scavengers will arrive to argue with physics. I am bad at arguments. I am good at leaving.

**NADI:** Commander, you have enough to start a war of memos. Try not to start the other kind until Iona finishes her coffee.

**KEI:** Come home. Sable Rook will want a word about silence and who buys it. I want publication. Iona wants a chain. The Guild wants peer review. You will have to choose who holds the ledger. I am… pre-explaining because the choice will hurt, and I prefer hurt with context.

**NADI:** Soft landing, Kei. We're working.

### OPTIONAL SIDE BEATS

- Brief scavenger hail: buy them off with scrap rights to non-recorder hull; Nadi approves.
- Fail first scan approach: Pip restates geometry without sarcasm upgrade (sarcasm is already max).

### BRANCH NOTES

None for critical path. Dump + key required. Voss is voice only — alive, elsewhere, capable of standing down later.

---

## 07. The Price of Silence

**SAGA INDEX:** 5 · **CHOICE CHAPTER**  
**LOGLINE:** With the ledger and route key in hand, the player chooses who receives authority over the truth: public broadcast, Guild peer review, or Law's evidence chain — while pirate negotiator Sable Rook offers safe passage for silence and honours a refusal.  
**SETTING:** Neutral meeting buoy at the edge of Lave's traffic bubble; Kei, Iona, Sable on channel; Guild board as text/presence.  
**MISSION VERBS:** Arrive / open choice → select Public / Guild / Law → confirm.  
**RECOVERY:** Choice can be re-opened until confirm; after confirm, persistent flag set (Public / Guild / Law trust). Refusal of Sable's deal is always allowed.

### FULL SCENE SCRIPT — Common approach

*(Buoy. SABLE ROOK's ship holds a respectful knife-range. KEI and IONA on split channel.)*

**SABLE:** Don't flinch. If I wanted your hull, you would already be a story people tell wrong.

**KEI:** Sable Rook. You're early.

**SABLE:** I'm on time for money. You're early for conscience. Cute overlap.

**IONA:** State terms without theatre.

**SABLE:** Pale Meridian pays for routes nobody questions. I can sell you quiet passage through their favourite patrol habits. Price: the ledger stays dark long enough for my clients to move. Or you keep a conscience and I keep my other offers. I honour refusals. I don't honour lectures.

**PLAYER:** We have proof they amplified migration signals—

**SABLE:** I know what you have. I have ears. Ears are cheaper than scanners.

**KEI:** Publish it. Settlements deserve the pauses back. If people know the “clear lane” is a rented animal song, they can stop buying mercy.

**IONA:** Publication without chain is a rumour with better fonts. Give me the ledger under seal. I can make clocks into charges. Slow. Boring. Binding.

**KEI:** The Guild can peer-review — verify, redact what gets people killed for no reason, release what opens roads. Slower than a broadcast. Safer than a martyr file.

**SABLE:** And I remain available if you decide silence is a tool, not a sin. Choose. I won't wait while you become poetry.

**KEI:** Commander. Who holds the ledger?

---

### OUTCOME A — Public

**PLAYER:** Public. Full release. Open channel.

**KEI:** *(breath)* Yes. Thank you. I will not pretend this is tidy.

**IONA:** Then I annotate “released against advice” and I still keep a hash. When the lawyers come, I want a fingerprint of the truth even if the truth is already shouting.

**SABLE:** Brave. Also loud. Loud data reaches pirates too. I'll keep my bargain shape: I offered silence for passage. You refused. I don't punish refusal. I do update prices. Expect more traffic that smiles with knives.

**KEI:** We can live with knives if the map is honest.

**SABLE:** Speak for yourself. I live with knives professionally.

**PIP:** *(if present on relay)* Public packet queued. Redaction: none. Consequence estimate: high. Emotional valence: Kei elevated.

**NADI:** *(optional patch)* You're going to make a lot of captains rewrite their habits overnight. Some will thank you. Some will hate you for making them think.

**VOSS:** *(cold open on hijacked advisory band)* Reliability requires stewards. You have chosen spectators. Spectators do not keep clocks.

**KEI:** Cassian. If you're listening: dependence is not consent. Come argue in daylight.

**VOSS:** Daylight is a luxury of people who do not move freight. We will speak again when your broadcast finishes breaking the thing it claims to save.

**IONA:** Let him talk. Talk is admissible when it's stupid on an open band.

**KEI:** Commander — choice locked. Public trust up. Meridian attention up. Next work gets noisier. I am over-explaining again. I will stop after this sentence. After this sentence. …One more: come home when you can. Berth six.

### OUTCOME B — Guild

**PLAYER:** Guild peer review. Verify first. Then release what holds.

**KEI:** I… wanted faster. I will take careful. Careful is not betrayal.

**IONA:** Guild review still needs an external hash. I'll hold a sealed copy. If your peers “lose” a page, I become unpleasant.

**SABLE:** Committees. My favourite predator. Slow enough that my clients can move without buying your silence directly. Convenient. I didn't even have to win.

**KEI:** That is not why we chose—

**SABLE:** Intentions are interior decorating. Outcomes are architecture. I'll be around when architecture cracks.

**GUILD BOARD:** *(text / voice)* Packet accepted. Review window opened. Provisional advisory to independents: treat Meridian “clear lane” premiums as unverified. Full release pending.

**ORU:** *(optional)* Peer review may sand off the animal grief until it looks like a chart error. Do not let them. Leave the pauses in the report.

**KEI:** I'll fight for the pauses. Commander, you bought us time and a process. Processes fail. They also sometimes work. I will try not to control this into safety theatre.

**PLAYER:** You asked me to choose.

**KEI:** I did. And I am practising accepting an answer that isn't mine. Progress. Claiming chapter.

### OUTCOME C — Law

**PLAYER:** Law. Iona holds the chain.

**IONA:** Then we do this ugly and correct. Sealed transfer. Dual hash. Your Guild gets read access under audit, not edit access.

**KEI:** Edit access is how truth becomes kinder than it earned. Fine. Read only. I hate how much sense that makes.

**SABLE:** Courts. Paper. Delays. My clients love delays they didn't pay for. You're making me look generous by accident.

**IONA:** Keep looking. If you interfere with witnesses, I stop distinguishing your bargains from Meridian's filings.

**SABLE:** Threat accepted as weather. Weather passes. I'll honour your refusal of my silence fee. Don't confuse that with friendship.

**VOSS:** *(on a private line Iona traces)* Officer Marr. We can settle property without spectacle.

**IONA:** Director Voss. You filed a theft before a theft. Spectacle already booked itself. Stand down from the receiver. If you want to argue stewardship, do it where clocks are public.

**VOSS:** Stewardship without control is a slogan. But I hear you. For now.

**KEI:** He can stand down. Remember that. The story does not need him dead. It needs him answerable.

**IONA:** Answerable is my favourite word that still fits on a form. Commander — Law trust up. Release delayed. Charges become possible. People may die in the delay. I will not lie about that. I will also not pretend a broadcast is a conviction.

**PLAYER:** Understood.

**IONA:** Dock. Sign. Sleep if you can. The next act is history in the route, and history does not care that you're tired.

**KEI:** Berth six. I'll be there with less speech and more tea. Possibly fail at both.

---

### OPTIONAL SIDE BEATS (all outcomes)

- Tamsin sends a one-line ack keyed to choice: Public — “Captains heard you.” Guild — “Don't sand it smooth.” Law — “I'll wait if the wait has teeth.”
- Mara: short cargo ping; if Public, grain prices jitter; she complains and means thanks.

### BRANCH NOTES

| Choice | Trust | Immediate tone | Later hooks |
|--------|-------|----------------|-------------|
| Public | Public + | Fast advisories, pirate pressure, Meridian hostility | Easier civilian helpers; harder quiet approaches |
| Guild | Guild + | Provisional notices, slower full truth | Survey markers, peer cover, Oru leverage |
| Law | Law + | Sealed case, Voss pressured not destroyed | Iona ceasefires, audits, courtroom tools |

Sable remains a bargainer, not an ally. Voss remains alive and capable of stand-down / evacuate / flee in Act IV. No outcome blocks Chapter 08.

---

## Production appendix (for implementation)

- These scenes are **feature-quality target dialogue**, richer than the six-beat PSP briefing strings in `src/saga.h`. Compress to HUD beats; keep full scenes for Mission Log / Comms playback / future expanded UI.
- Speaker panels: named blue (NPC), orange (player). No emoji.
- Essential items (receiver, stamp, recorder dump, route key, ledger) are **flags**, not cargo slots.
- Thargoids: absent as targets in Act I; only implied by post-Titan world anxiety and Oru's refusal to over-name.
- Historical names (GalCop, INRA, Aegis, Federation / Empire / Alliance) appear only as context in later acts; Act I stays local to independents, Guild, Law, Meridian, and working pilots.

---

*End of Prologue & Act I screenplay section.*


---

# PART C — Acts III–IV (Chapters 14–25)

> Source draft: Acts III–IV feature screenplay, branching matrix, mission appendix.

## ELITE: NEXT — The Open Channel
### Feature Screenplay: Acts III–IV (Chapters 14–25 / Saga 12–23)

**Lore frame:** post-Titan war, ~3311. Pale Meridian privatized lanes by amplifying biological navigation signals. Thargoid contact: one remote scan-only encounter plus an answering echo; never combat loot. Historical reference only (never quoted as source text): INRA mycoid atrocity; GalCop dissolution 3174; Aegis virtues and failures; Federal, Imperial, and Alliance diplomacy styles.

**Continuity:** Assumes Decisions 1–2 already set (ledger custody; how openly Ryn’s evidence left Lave). Trust buckets: **Public / Guild / Law / Independent**.

**Dialogue rule:** All speech original to this campaign. No GalNet or novel text.

---

# ACT III — Three Powers and No Easy Flag

---

## Chapter 14 — A Map for Everyone
**Saga:** 12 · **UI:** CHAPTER 14

### SETTING
Neutral archive station *Amber Ledger*, an independent data vault hung off a quiet system’s outer belt. Docking ring smells of coolant and paper substitutes. Three sealed packets on a cart. Meridian soft-contact already logged.

### MISSION
Dock at the archive. Deposit three provenance-tagged map copies. Refuse any exclusive buyout. Combat optional if Meridian agents escalate; refusal alone completes the story beat.

### FULL SCENE

*(Kei, Ryn, and the commander in the archive anteroom. A clerk waits behind frosted glass. A Meridian intermediary in a pale coat stands too close to the cart.)*

**RYN:** Three copies. Same map. Same doubts printed in the margin. Nobody gets a cleaner version than anyone else.

**KEI:** If we label it “safe,” someone dies believing us. If we label it “ours,” someone dies waiting for permission.

**MERIDIAN AGENT:** Pale Meridian can host the authoritative set. Public mirrors drift. We don’t.

**RYN:** Authoritative is a word people use when they want the others to go quiet.

**AGENT:** We pay for exclusivity. Fairly. Your surveyor’s name stays off the invoice if that’s the concern.

**KEI:** Her name is not a line item. And the concern is the lane, not the branding.

**AGENT:** Without a steward, the next forge will look like a community update. You’ve seen what rumour does to fuel prices.

**RYN:** We’ve seen what a private steward does to lifeboats. I’ll take messy truth over polished hazard notices.

**IONA** *(on short-range, delayed):* Archive receipt must show timestamps and checksums. If they offer a private vault, walk. A badge I trust still needs paper I can read.

**AGENT:** You’re asking three powers to share a map none of them wrote. That invitation expires when someone else claims the relay.

**COMMANDER** *(player option, tone):* We deposit. No exclusives.

**AGENT:** Then you’ve chosen theatre. Meridian will still be on the frequency when your coalition argues about minutes.

**RYN:** Good. Let them hear us arguing in public. That’s the point.

**KEI:** Clerk—three packets, open provenance, no embargo. If anyone tries to buy the master, log the offer as evidence.

**CLERK:** Logged. And for what it’s worth—GalCop used to hang seals on walls like this. The seals outlived the people who meant them.

**VENN** *(radio, dry):* Don’t romanticise the seal. Romanticise the argument that kept it honest. Deposit and come home before someone invents a new forgery.

**RYN:** Packets are in. Map’s for everyone. Including the people who will hate us for it.

### BRANCH NOTES
- **High Public trust (Decision 2 broadcast):** civilian techs already waiting; deposit is fast; Meridian agent leaves without threat.
- **High Guild trust:** Guild notarises checksums; later Chapter 19 survey markers unlock early.
- **High Law trust:** Iona’s receipt language appears on-screen; Meridian “theft” counter-claim auto-fails.
- **Low all / Independent:** agent offers credits; refusal still succeeds; optional skirmish if player fires first—does not gate deposit.
- **Evidence flag:** `MAP_NEUTRAL_DEPOSIT` set. Buyout acceptance is not offered as a completing path.

---

## Chapter 15 — Federal Measure
**Saga:** 13 · **UI:** CHAPTER 15

### SETTING
Federal logistics barge *Measure of Care*, temporary berth near a rescue-exercise range. Clean corridors. Screens that count response times. Envoy Hale in a service-cut jacket, not ceremonial.

### MISSION
Attend Hale’s briefing and complete a timed rescue exercise (pick up one stranded tender or clear a debris lane). Listen and record terms. No allegiance choice yet.

### FULL SCENE

**HALE:** Commander. Captain Reed. The Federation doesn’t need your map. We need your lane not to kill civilians while three governments write letters.

**TAMSIN:** Letters don’t refuel a convoy. What are you actually offering?

**HALE:** Patrol shells on the open approaches. Rapid medevac. Priority jump slots for relief hulls. In exchange: operational telemetry shared with Federal Traffic for the duration of the crisis.

**KEI:** Define “duration.” Define “telemetry.”

**HALE:** Duration ends when the amplifiers are dark and a public inspection charter is ratified. Telemetry means ship IDs, vector, cargo class—not cabin audio, not private mail.

**RYN:** And if a settlement doesn’t want Federal paint on their sky?

**HALE:** Then they keep dying on a privatised “safe route.” I’m not here to be liked. I’m here to shrink the body count before your coalition finishes arguing about custody.

**TAMSIN:** Show me the exercise. Talk is cheap; minutes-to-contact isn’t.

*(Cut to range: a disabled tender, timer running. Hale watches without commentary until the dock clamp locks.)*

**HALE:** That was fifty-one seconds. Meridian’s private patrol average in this sector last month was four minutes—when they bothered.

**NADI:** Pip says their “bothered” correlates with paid lane subscriptions. Shocker.

**PIP:** Correlation flagged. Causation: not my department. Sarcasm: borrowed.

**HALE:** Take the numbers home. I’m not asking for a flag today. I’m asking you not to pretend neutrality is free.

**IONA** *(radio):* Record the telemetry clause verbatim. If “cargo class” becomes “cargo contents,” I want the delta timestamped.

**HALE:** Officer Marr is right to be suspicious. Good. Suspicion is how institutions stay useful after the speeches.

**KEI:** We’ll hear the Empire and the Alliance before anyone signs a colour.

**HALE:** Expected. Federal doctrine after the last shared disasters is simple: move first, publish second, apologise in footnotes if we must. It’s ugly. It keeps people breathing.

**TAMSIN:** Ugly I can fly with. Invisible conditions I can’t. Send the packet.

**HALE:** Sent. One more thing—when you choose a coalition, remember who shows up when the clock is loud. Ideals don’t hold a lane alone.

### BRANCH NOTES
- Completing the rescue under par time: **+Independent** soft credit (helpers later); Hale’s radio tone warms in Ch. 20.
- Ignoring the exercise but attending briefing: chapter still completes; Hale’s later support is thinner (no medevac wing).
- Prior **Public** Decision 2: Hale cites settlement prep as proof rapid disclosure works—offers extra fuel tender in Ch. 19 Public path.
- Prior **Law** Decision 1: Hale’s telemetry language tightens automatically (Iona pre-redline).
- Combat policy: none required; pirates can spawn on approach—optional.

---

## Chapter 16 — Imperial Courtesy
**Saga:** 14 · **UI:** CHAPTER 16

### SETTING
Imperial hospitality suite aboard the yacht *Second Silence*—loaned, not owned by the campaign. Soft light. Real fruit. Patron Lysara seated as if the room were a favour she is allowing you to notice.

### MISSION
Hear Patron Lysara’s patronage offer. Optional: review the debt schedule Sable surfaces. No signature. Dock-and-listen completes.

### FULL SCENE

**LYSARA:** Please, sit. I dislike negotiating while people stand like petitioners. It invents a hierarchy we haven’t agreed to yet.

**SABLE:** Careful. When an Imperial says “yet,” start counting the links.

**LYSARA:** Mistress Rook. Still translating courtesy into threats. How refreshing.

**KEI:** We’re here for terms, Patron. Funding, accountability, and what you call the lane when the crisis ends.

**LYSARA:** Long-term underwriting for relay maintenance. Personal surety—mine—against sabotage claims. In return, Imperial observers on the inspection board, and first refusal on contracted survey work for ten years.

**RYN:** First refusal is a soft monopoly wearing perfume.

**LYSARA:** Monopolies are crude. Preference is civilisation. Someone will repair those nodes. Better a named house than a committee that discovers courage after the outage.

**SABLE:** Named houses collect favours the way stations collect dust. I’ve sold silence to people who smiled like you. The invoice arrives later, in a nicer font.

**LYSARA:** And yet you keep bargains. So do I. Pale Meridian’s sin is not ambition—it is pretending ambition was charity. I won’t insult you with that costume.

**COMMANDER** *(listen):* What’s the debt if we walk away after the amplifiers fall?

**LYSARA:** Then you walk. The underwriting ends. No chase, no warrant, no sudden “security review” of your berth. Empire remembers who refused cleanly. We dislike unfinished theatre more than we dislike refusal.

**KEI:** That’s… clearer than I expected.

**LYSARA:** Clarity is a luxury we can afford. The Federation sells speed. The Alliance sells minutes. We sell continuity—and the expectation that continuity has a face.

**RYN:** Faces change. Charters shouldn’t need a face to survive a funeral.

**LYSARA:** Then write your charter so well that my successors are bored. I can live with boredom. I cannot live with another private firm teaching biology to scream for profit.

**SABLE** *(aside to commander):* She’s not lying about the walk-away. She’s lying by omission about how preference reshapes every contract around the preference. Hear her. Don’t marry her.

**LYSARA:** Take the packet. Drink the tea if you like. Sign nothing. When you choose your coalition, remember that someone will still be paying when the cameras leave.

### BRANCH NOTES
- Reading Sable’s annotated debt schedule: unlocks **Independent** tip in Ch. 19 (covert approach lane).
- High **Guild** trust: Lysara’s “first refusal” is publicly minuted later if Guild coalition chosen—reduces hidden debt risk.
- Flattering Lysara / accepting gifts: cosmetic only; no mechanical lock-in.
- Combat policy: none. Yacht security stands down if player complies with docking protocol.

---

## Chapter 17 — Alliance of Necessity
**Saga:** 15 · **UI:** CHAPTER 17

### SETTING
Alliance free-conference module *Shared Floor*—folding chairs, bad coffee, excellent recording lights. Delegate Mirek with a stack of printed agendas no one requested and everyone needs.

### MISSION
Hear the Alliance case. Assist Iona’s audit-trail test (compare three public minute hashes). No allegiance choice yet.

### FULL SCENE

**MIREK:** Thank you for coming without a parade. Parades make people lie faster.

**IONA:** I’m here to see whether your “public minutes” survive a hostile clerk. Start the test.

**MIREK:** Three mirrors, three jurisdictions, one draft charter for interim relay custody. If any hash drifts, the session voids. Slow? Yes. Faster than rebuilding trust after a secret edit.

**KEI:** GalCop promised mutual defence and local voice. It dissolved when responsibility became a rumour. How do you avoid becoming a prettier rumour?

**MIREK:** We don’t avoid it by speeches. We avoid it by making the edit log louder than the speech. Alliance doctrine after years of being the “third voice” in rooms that preferred two: if you can’t show your work, you don’t get the lane.

**RYN:** And if showing your work tips Meridian to move first?

**MIREK:** Then we have witnesses when they move. The Federation can arrive in force. The Empire can arrive in certainty. We arrive with people who will still be arguing after the smoke—which is annoying, and also how you keep a commons.

**NADI:** Pip ran the hashes. Two match. One’s late by six seconds.

**PIP:** Late mirror attributed to relay congestion, not rewrite. Still ugly. Recommend: demand signed delay notice next time.

**IONA:** Accepted. Mirek—put the delay notice requirement in the draft before I recommend anyone take you seriously.

**MIREK:** Done. See? Decentralisation spreads power and the homework.

**TAMSIN:** Homework doesn’t stop interceptors.

**MIREK:** No. Homework tells the next court who armed them. If your coalition wants ceasefire language that courts recognise, you want us in the room—not as owners, as witnesses who refuse to lose the tape.

**VENN** *(radio):* Lave remembers committees that loved procedure more than people. Don’t become that. But don’t throw away procedure because a company sold you fear of paperwork.

**MIREK:** Fair. We’re offering a coalition seat, not a coronation. When you pick a flag—or refuse a flag—know that Alliance pilots will still fly relief if the minutes stay public. That’s the necessity: not victory. Continuity without a single throat to choke.

**IONA:** I’ll allow it onto the option board. Commander—listen to all three. Then choose who stands with you, not who owns you.

### BRANCH NOTES
- Passing Iona’s audit test: **+Law** soft credit; Ch. 22 stand-down threshold slightly easier.
- Failing once then retrying: still completes; Mirek’s later radio is cooler.
- Prior **Law** Decision 1 (ledger with Iona): Alliance draft auto-includes chain-of-custody language.
- Combat policy: none. Optional protest ships outside—nonlethal disperse available.

---

## Chapter 18 — The Coldest Signal
**Saga:** 16 · **UI:** CHAPTER 18

### SETTING
Deep black beyond the secondary relay—no station lights, only the hum of the ship and Pip’s status glyphs. A structured return on sensors that refuses ordinary classification. Oru on science channel. Weapons soft-locked by mission brief (player can override; override fails the clean ending and forces recovery scan from farther out).

### MISSION
Scan the silent contact from safe range. Do not fire. Do not pursue. Withdraw with the recording. Contact never becomes a normal target and never drops loot.

### FULL SCENE

**PIP:** Signal structure: nonhuman. Confidence interval: troubling. Recommendation: curiosity without munitions.

**ORU:** Cut thrust. We are not introducing ourselves with a wake that looks like a hunt.

**KEI:** Is it… them?

**ORU:** If you need a name before a measurement, you will invent a war to feel tidy. Record first.

**RYN:** The amplifiers made our routes scream. If something out there listened, it heard a lie wearing a migration’s skin.

**PIP:** Contact geometry unstable. Not debris. Not a beacon we filed. Not a human IFF ghost. Label options include several words newsrooms love.

**ORU:** Label it *unresolved return*. Let the newsrooms be wrong without our help.

**COMMANDER** *(scan):* Holding distance. Passive only.

**PIP:** Scan complete. Packet is… incomplete on purpose, I think. Or incomplete because we lack the sense it was made for. Either way: do not chase the missing piece into a story.

**RYN:** People will say Thargoid. Some will say omen. Some will say weapon test.

**ORU:** People said similar things the last time humanity decided living systems were obstacles. We are not repeating that arrogance with better sensors.

**KEI:** Pull us back. If it’s curious, let it stay curious. If it’s not for us, we don’t get to demand a translation.

**PIP:** Withdraw vector plotted. Note: firing solution was available. I deleted it from the helpful tips. You’re welcome.

**NADI:** Kid’s learning ethics. Terrifying.

**ORU:** Ethics is just delayed panic with better handwriting. Come home with the file. Uncertainty is not a failure state. It is the only honest one we have.

**RYN** *(quiet):* If they heard the scream we made… I hope the silence we leave is clearer.

### BRANCH NOTES
- Clean scan, no weapons: sets `COLDEST_SIGNAL_CLEAN`; Oru’s Ch. 23 lines acknowledge restraint; **+Guild** soft if Guild trust already high.
- Weapons fired / too-close approach: contact vanishes; recovery = long-range residual scan; chapter completes with `COLDEST_SIGNAL_TAINTED`; news radio later overclaims “engagement.”
- Never spawns as killable NPC; never yields cargo.
- Prior mycoid-archive chapter (Act II) only colours Oru/Iona subtext—no mechanical weapon unlock. Ever.

---

## Chapter 19 — No Easy Flag
**Saga:** 17 · **UI:** CHAPTER 19 · **PERSISTENT DECISION 3**

### SETTING
Lave Hub planning room—map table, three empty chairs that will not stay empty. Ryn at the head because she refuses the head. Hale, Lysara, and Mirek present by holo. Crew in person.

### MISSION
Choose the coalition that will approach the primary relay: **Public convoy network**, **Guild survey teams**, or **Lawful task force**. Every path supports non-lethal completion. Prior trust spawns visible helpers.

### CHOICE PROMPT
**RYN:** Pick allies, not owners. We live with the people who stand on the channel with us.

| Option | Custody flavour | Helper preview |
|--------|-----------------|----------------|
| A Public | Working pilots, open fuel | Tamsin’s tenders |
| B Guild | Survey markers, peer review | Kei’s ranging buoys |
| C Law | Ceasefire calls, warrants | Iona’s suspend authority |

---

### OUTCOME A — Public Convoy

**TAMSIN:** Then it’s us. No parade paint. Just ships that already know how to share a lane without asking a throne.

**HALE:** Federal medevac will still answer distress. We won’t pretend this is our operation.

**LYSARA:** Charming. Try not to romanticise fatigue. Fatigue makes mistakes expensive.

**MIREK:** We’ll minute your choice. Public doesn’t mean undocumented.

**KEI:** I’ll hate the chaos. I’ll love that the chaos answers to people who burn fuel for a living.

**SABLE:** I’ll sell you a quiet approach if your Independent friends paid for it. If not, fly loud and mean it.

**RYN:** Loud and shared. Meridian can’t buy every volunteer.

**VENN:** Lave traffic will prioritise relief hulls. Don’t make me regret the word “prioritise.”

**NADI:** Pip’s already renaming the wing. Please stop him.

**PIP:** Wing designation: *People Who Bring Their Own Spanners*. Final.

**IONA:** I’ll ride legal shadow—evidence, not command. Break their blockade without breaking the case.

**RYN:** Then we fly under no easy flag. Just a route that doesn’t belong to a logo.

### OUTCOME B — Guild Survey

**KEI:** Survey teams, ranging buoys, published uncertainty. We open the channel by measuring it in public.

**RYN:** You’re going to over-explain every waypoint, aren’t you.

**KEI:** Panic makes me precise. This is the good version of that.

**HALE:** We’ll hold a perimeter if asked. We won’t own your markers.

**LYSARA:** Peer review is a polite way to delay courage. Prove me wrong.

**MIREK:** Alliance observers can certify buoy integrity. Slow, visible, hard to forge.

**ORU:** Guild rules already say observe before naming. Keep that when the amplifiers scream.

**SABLE:** Markers are lights. Lights attract thieves. I’ll honour a bargain if you hire one.

**IONA:** Guild custody of method, not of the lane. Write that twice.

**VENN:** File your buoys with traffic control or I will personally haunt your spectrum.

**PIP:** Buoy humour module loaded. Suppressing it. For now.

**RYN:** Guild it is. We map the cut so nobody can pretend the dark was empty by accident.

### OUTCOME C — Lawful Task Force

**IONA:** Then we go with warrants, suspend language, and a chain of evidence Meridian can’t rebrand as piracy.

**RYN:** Law moves slow.

**IONA:** Law moves on paper so ships don’t have to move on graves. I’ll take slow if it stays legible.

**HALE:** Federal patrol can carry your suspend order if the packet is clean. That’s the deal.

**LYSARA:** Empire recognises lawful interim authority when it is not a costume. Don’t let it become one.

**MIREK:** We’ll witness. If your ceasefire call is real, our minutes will make it expensive to ignore.

**KEI:** I’m afraid of uniforms that forget they’re borrowed. Stay borrowed.

**IONA:** Count on it. Crews are not targets. Hardware that screams biology is.

**SABLE:** If your lawful friends shoot first, my bargain ends. Keep them housebroken.

**TAMSIN:** We’ll still fly relief under your umbrella. Just tell us when the umbrella is actually open.

**VENN:** Lave will broadcast the suspend text on civilian bands. No surprises for honest traffic.

**RYN:** Lawful task force. Make it true enough that Voss can’t call it theatre.

### BRANCH NOTES (all outcomes)
- Choice sets `FLAG_PUBLIC` / `FLAG_GUILD` / `FLAG_LAW` and increments matching trust.
- Helpers scale with prior Decisions 1–2 and soft credits from 15–17.
- No path removes Chapters 20–25. Prices/radio/helpers diverge; fuel and repair remain available.

---

# ACT IV — The Channel Opens

---

## Chapter 20 — The Black Flight
**Saga:** 18 · **UI:** CHAPTER 20

### SETTING
Public lane *Needle Reach*—traffic ribbons cut by Pale Meridian interceptor wings. Civilian transponders screaming polite panic. Tamsin’s convoy stacked behind a debris false-wall Meridian “relocated.”

### MISSION
Break the blockade: disable, outmanoeuvre, or draw ships away while Tamsin passes. Destruction counts; it is never the only verb. Checkpoint when convoy clears the gate buoy.

### FULL SCENE

**VENN:** Meridian paint just crossed a public lane. That is not a “security envelope.” That is theft with thrusters.

**TAMSIN:** Convoy’s holding. We can take hits. We can’t take a closed gate forever.

**KEI:** Commander—break them. Prefer lives. Progress saves if you have to breathe.

*(If FLAG_PUBLIC)*  
**TAMSIN:** Fuel tenders are on your wing. They’ll catch a missile meant for you once. Don’t make them do it twice.

*(If FLAG_GUILD)*  
**KEI:** Survey markers painted a cold corridor. Use it. Heroics are optional; geometry isn’t.

*(If FLAG_LAW)*  
**IONA:** All Meridian hulls in Needle Reach: operating authority under review. Crews are not targets. Power down weapons and drift. This is your clean exit.

**VOSS** *(wideband, composed):* You are interfering with a private safety corridor. Casualties on your conscience are still casualties.

**RYN:** You moved the corridor after people launched. Don’t lecture us about conscience with a forged chart in your hand.

**SABLE** *(if Independent soft credit):* Back door’s open for six minutes. After that I start charging interest in favours.

**NADI:** Interceptor port shield modulating—Pip marked the stutter.

**PIP:** Stutter window: 1.8 seconds. Please invent less sports metaphors while I count.

**COMMANDER** *(action):* Gate buoy clearing—

**TAMSIN:** We’re through. Don’t chase glory into their capital guns. Regroup on the far mark.

**HALE** *(if Public or prior rescue par):* Medevac wing sweeping your wounded. Federal colours, civilian patients. Don’t shoot the help.

**LYSARA** *(if Guild path and high Guild):* My observers confirm Meridian fired first on a public ribbon. How inconvenient for their poets.

**MIREK** *(if Law path):* Ceasefire compliance at forty percent. Ugly. Better than zero. Keep recording.

**VENN:** Lane’s ugly but open. Next problem wears a nicer suit.

### BRANCH NOTES
- Non-lethal clear (disable/draw-off): **+Independent**; Voss radio later less absolute.
- High collateral civilian damage: trust penalties soft; epilogue tone cooler; chapter still completable via recovery regroup.
- Fail-forward: retreat preserves convoy progress after safe checkpoint; retry does not escalate fleet forever.
- Combat policy: bounded interceptors; capital structure not required this chapter.

---

## Chapter 21 — Aegis Echo
**Saga:** 19 · **UI:** CHAPTER 21

### SETTING
Drifting research buoy *Joint Reminder*—scarred, still singing on a forgotten cooperative frequency. Oru and Kei on comms. Interior is scan-access, not a dungeon crawl.

### MISSION
Recover the buoy’s intact research packet. Unlocks safer amplifier shutdown sequence for Chapter 22. Scan objective; combat optional vs scavengers only.

### FULL SCENE

**ORU:** This buoy remembers a time when three powers tried to share a nightmare without sharing a throat to choke.

**KEI:** Aegis. Virtues: shared science, defence, rescue that didn’t wait for perfect minutes. Failures: secrecy, escalation, the way fear makes committees cruel.

**RYN:** We’re not rebuilding their letterhead. We’re stealing their hard-won “don’t make it worse” checklist.

**IONA:** If the packet includes bioweapon pathways, we quarantine and burn. Meridian does not get a sequel to old atrocities wearing new logos.

**ORU:** Agreed. The mycoid years taught the wrong people that living systems are puzzles you solve with extinction. This buoy’s useful pages are about shutdown, triage, and not inventing enemies to feel decisive.

**PIP:** Packet integrity 91%. Missing pages look like deliberate redaction, not rot. Someone was ashamed in advance.

**NADI:** Shame’s a start. Upload before scavengers decide history is scrap mass.

**HALE** *(optional radio):* Federal archives still argue about what Aegis owed the dead. Don’t let your shutdown sequence become another classified regret.

**LYSARA:** Empire remembers joint failures with exquisite detail. Use the checklist. Refuse the romance of unity theatre.

**MIREK:** Alliance will publish that you recovered a cooperative safety method. Not a weapon. Say it out loud so nobody “mishears” later.

**KEI:** Commander—bring it home. Precision over revenge starts with tools that prefer off-switches to funerals.

**ORU:** And when the cold signal comes back to mind—remember restraint is also research.

**RYN:** Packet’s ours. Next stop, we make the scream stop.

### BRANCH NOTES
- Clean recover: `AEGIS_ECHO_SAFE_SHUTDOWN` — Ch. 22 nodes can be disabled via tool interaction even under fire.
- Buoy damaged in fight: recovery still possible; shutdown minigame harder; no story softlock.
- Combat policy: scavengers only; buoy is not a damage sponge boss.

---

## Chapter 22 — The Open Channel
**Saga:** 20 · **UI:** CHAPTER 22

### SETTING
Primary Pale Meridian relay—*White Meridian Crown*—three amplifier nodes braided through a living migration corridor. Coalition wings as chosen. Voss on command deck holo, reasonable and lethal in the way executives are.

### MISSION
Disable three amplifiers; broadcast evidence; keep crews alive. Checkpoints after each node. Voss resolution branches on **Law** and **Public** trust (see below).

### FULL SCENE (common spine)

**KEI:** Disable the amplifiers. Keep the crews alive. This is the cut. Precision over revenge.

**VOSS:** You mistake control for cruelty. We made these routes dependable. Dependence is a kind of peace.

**RYN:** Dependence without consent is just a softer boarding action. You amplified a living signal until the lane belonged to your invoices.

**VOSS:** And when no one is responsible? When the next forge wears a community badge? I have met committees. They bury people in procedure.

**IONA** *(if FLAG_LAW):* Pale Meridian vessels: operating authority suspended. Your crews are not targets. Stand down and preserve your careers.

**TAMSIN** *(if FLAG_PUBLIC):* Repair volunteers inbound. Keep the lane clear; they’re carrying spare breakers, not speeches.

**KEI** *(if FLAG_GUILD):* Markers are live. Cut on the ranked nodes—uncertainty published as we go.

**NADI:** Node one soft. Pip’s singing the Aegis off-sequence like a lullaby for monsters we invented.

**PIP:** Lullaby complete. Please do not clap. Clapping vibrates things that should not vibrate.

**COMMANDER:** Node two—

**SABLE** *(if Independent path helpers):* I’m holding a private exit for Meridian civilians. Don’t shoot the people taking it. I keep bargains. So should you.

**VOSS:** You will open a commons and call it virtue. The commons will be hungry. Hungry things sell themselves again.

**RYN:** Then we write inspection into the hunger. You’re not the only adult in the sky, Cassian.

---

### VOSS BRANCH — Stand Down
**Requires:** high **Law** trust (typically Decision 1 Law + Decision 3 Law, or Law + strong Ch. 17 audit). Evidence broadcast integrity high.

**IONA:** Director Voss, the suspend packet is clean. Testimony buys you a future that isn’t a manhunt.

**VOSS:** …You prepared this like you expected me to be intelligent.

**RYN:** We expected you to prefer a room with lawyers to a hull with no friends.

**VOSS:** Amplifiers powering down under my seal. Meridian crews: weapons cold. I will answer questions. I will not apologise for wanting order—only for the biology I treated as infrastructure.

**KEI:** That’s not enough. It’s a start we can put on a docket.

**VOSS:** Broadcast your evidence. I’ll stand here and listen to what I funded. That is my surrender. Try not to waste it on applause.

**Flag:** `VOSS_STAND_DOWN`

---

### VOSS BRANCH — Evacuate
**Requires:** high **Public** trust (broadcast Decision 2 + Decision 3 Public, or Public + strong civilian helper presence). Law not required.

**TAMSIN:** Cassian—your people are still people. There’s a corridor open. Use it before someone makes you a symbol.

**VOSS:** Symbols are durable. Evacuations are honest.

**RYN:** Then be honest. Leave the nodes. Leave the myth that only you could keep the lights steady.

**VOSS:** Meridian personnel: abandon the Crown in good order. No scuttling. No parting shots. If the commons fails, it will not be because I salted the ground.

**KEI:** We’ll remember that sentence. Don’t make us regret believing it.

**VOSS:** Believe the act. Sentences are cheap. *(Holo cuts as shuttles push clear.)*

**Flag:** `VOSS_EVACUATE`

---

### VOSS BRANCH — Flee
**Default / low Law and Public:** after node three dies, Voss rabbits.

**VOSS:** You have your open channel. Enjoy the noise. I decline to be the villain in a play written by people who never had to price a route.

**IONA:** He’s running. Don’t chase into an ambush. The relay matters more than your need to finish the argument.

**SABLE:** I can sell you his likely hole. Price is a favour. Or let him become tomorrow’s problem while you finish today’s.

**RYN:** Let him run. The lane stays. The invoices don’t get to be destiny.

**KEI:** Copy. Commander—broadcast. Then we hold the quiet we made.

**Flag:** `VOSS_FLEE`

---

### Broadcast close (all branches)

**RYN:** Evidence is out. Amplifiers dark.

**VENN:** Lave Hub to everyone on this band: the channel is open. Fly like it belongs to more than one logo.

**ORU:** Listen for what returns when we stop screaming.

### BRANCH NOTES
- Safe shutdown item from Ch. 21 converts combat node destruction into tool disables.
- Player defeat: break away; node progress checkpointed; retry bounded.
- Voss death is never required and not authored as a success condition.

---

## Chapter 23 — The Answering Dark
**Saga:** 21 · **UI:** CHAPTER 23

### SETTING
The same black that held the coldest signal—now emptier, amplifiers silent. A distant structured call answers the absence. Visual language suggests Thargoid-adjacent technology without certainty. Scan-only; no pursuit objective.

### MISSION
Record the distant echo. Do not chase. Return with the log. Uncertainty preserved in the mission complete text.

### FULL SCENE

**RYN:** Something answered. It may never have meant us.

**ORU:** It answered the empty space we left. That is not the same as a conversation. Do not force it to become one.

**PIP:** Echo packet differs from prior contact. Same family of wrongness. Confidence: still troubling. Advice: still no guns.

**KEI:** The news will name it. We will file it as unresolved. I’m practising saying that without shaking.

**NADI:** Shaking’s allowed. Inventing a war to stop shaking isn’t.

**IONA:** For the record: no engagement authorised, no salvage claimed, no “defensive residue” nonsense in the log.

**TAMSIN** *(if Public):* Convoy pilots want to know if they should be afraid. Tell them honest: we don’t know. Fear with a map beats fear with a rumour.

**HALE / LYSARA / MIREK** *(one line by coalition, compressed):*  
- Hale: “Federal watch will observe, not claim.”  
- Lysara: “Empire will not decorate uncertainty with prophecy.”  
- Mirek: “Alliance minutes will say *unknown* and mean it.”

**RYN:** Record and hold. If they’re out there—if that’s even the right *they*—we met them first by stopping a lie.

**ORU:** Then our first useful act was making room. Second useful act: not filling it with our need to be protagonists.

**COMMANDER:** Log sealed. Coming home.

**VENN:** Copy. Berth lights are on. Try not to arrive as myths.

### BRANCH NOTES
- Clean record: `ANSWERING_DARK_CLEAN`.
- Pursuit beyond marker: echo fades; recovery scan still completes chapter; epilogue mentions restraint failure softly if tainted.
- No loot, no wreck, no boarding.

---

## Chapter 24 — Who Keeps the Light
**Saga:** 22 · **UI:** CHAPTER 24 · **PERSISTENT DECISION 4**

### SETTING
Lave Hub council balcony overlooking traffic. Inspection charter on the table. Losing factions retain access to public route data regardless of winner.

### MISSION
Choose final custodian under a public inspection charter: **Public**, **Guild**, or **Law**.

### CHOICE PROMPT
**IONA:** Someone must hold the keys and accept inspection. Losers keep the map. Trust changes tone—not fuel, not repair, not whether you still have a home.

---

### OUTCOME A — Public Custody

**TAMSIN:** Then the pilots who burn the lane keep the keys, and everyone else gets to watch the ledger.

**KEI:** I’ll sleep worse. I’ll work better. Care isn’t control—I finally heard myself say it.

**RYN:** Public custody with teeth. Inspection days are mandatory, not decorative.

**MIREK:** Alliance will mirror your maintenance logs. Slowly. Relentlessly.

**HALE:** Federal traffic will honour your published windows. We’ll complain in private. That’s healthy.

**LYSARA:** Empire retains observer seat only. Preference is retired. Continuity is your problem now—wear it well.

**VENN:** Berth allocation stays boring. Boring is the dream.

**SABLE:** I’ll still find edges. Edges are my trade. At least now the map isn’t a hostage.

**VOSS** *(if stand-down):* I’ll testify that public custody can work if inspection is real. Don’t make me a liar twice.

**PIP:** Renaming wing again: *Keepers Who Answer Radios*.

**NADI:** Denied. We’re not a cult.

**RYN:** Light stays on. Hands stay plural.

### OUTCOME B — Guild Custody

**KEI:** Guild holds method and maintenance. Uncertainty stays published. If I start sounding like a gate, someone kick me.

**RYN:** I’ll do it. Affectionately. With a boot.

**ORU:** Survey ethics baked into custody. Good. Don’t let peer review become peer silence.

**IONA:** Law audits the Guild on a fixed calendar. No friendly skips.

**TAMSIN:** As long as fuel prices don’t grow a membership fee, we’re in.

**MIREK:** Minutes will show when buoys change. If they don’t, we void the session.

**HALE:** Acceptable. Just remember rescue doesn’t wait for a symposium.

**LYSARA:** First refusal is dead. Good. Patronage hates being bored; boredom can be a virtue.

**VENN:** File your changes before gossip does.

**SABLE:** Guild maps are neat. Neat attracts forgers. Stay ugly in the margins.

**KEI:** Margins it is. We keep the light by admitting what we don’t know.

### OUTCOME C — Law Custody

**IONA:** Lawful archive holds the keys. Public inspection charter binds us harder than it binds you—that’s the point of a badge worth having.

**RYN:** Don’t become the uniform you used to distrust.

**IONA:** Then keep distrusting me in public. It’s part of the design.

**KEI:** Guild will still publish surveys. We just won’t own the switch.

**TAMSIN:** As long as a warrant can’t close a lane without a hearing, my convoys will live with it.

**MIREK:** We’ll witness every custody transfer. Alliance speciality: being inconvenient on purpose.

**HALE:** Federal prosecutors will love you. Federal captains will grumble. Both are fine.

**LYSARA:** Empire respects a clean chain. Try to remain clean when frightened.

**VOSS** *(if flee):* You’ll need better hunters than principles. *(If stand-down):* I’ll sit for the record. Chain me to the truth I funded.

**VENN:** Lave Hub recognises interim lawful custody. Bring back the stamps you borrow.

**NADI:** Stamps. Always stamps.

**PIP:** Stamp humour suppressed. Barely.

**IONA:** Light kept. Watchers watching the watchers. That’s the job.

### BRANCH NOTES
- Sets final trust bump; epilogue tone and radio faction colour follow winner.
- Losing groups retain `MAP_NEUTRAL_DEPOSIT` access—mechanical availability unchanged.
- No softlock; choice can differ from Decision 3 (tension acknowledged in epilogue).

---

## Chapter 25 — A Berth Kept Warm
**Saga:** 23 · **UI:** CHAPTER 25 · **EPILOGUE**

### SETTING
Lave System Hub, berth six. Soft traffic noise. Credits-ready. Crew present as relationships allow. Four decision echoes play according to save flags, then three postgame project pitches.

### MISSION
Return to Lave Hub. Hear the echoes. Select first post-story project. Free flight continues.

### FULL SCENE

**VENN:** Berth six is yours. Try not to look surprised; you’ve been paying for it in gossip and unpaid favours.

**KEI:** I spent so long asking you to bring someone home that I forgot you might need a home too. I’m sorry for the control I called care.

**RYN:** There’s a survey seat open. No disappearing this time—or if I do, I leave a map and a radio schedule like a civilised menace.

**NADI:** Pip welded a plate with your name. It’s crooked. Crooked means handmade.

**PIP:** Crooked means NADI moved the clamp. Attribution matters.

---

### FOUR DECISION ECHOES
*(Play all four in sequence; lines swap by recorded choice.)*

**Echo 1 — The Price of Silence**  
- *Public ledger:* **SABLE:** “Your light reached thieves and clinics in the same hour. I kept my bargain. The dark adapted. So did you.”  
- *Guild ledger:* **KEI:** “Peer review slowed the knife. It also blunted a few panics. I’ll take that trade again.”  
- *Law ledger:* **IONA:** “The chain held. Ugly, legible, enough. That’s what a badge is for.”

**Echo 2 — What We Carry Home**  
- *Broadcast now:* **TAMSIN:** “Settlements moved before Meridian finished its press release. Loud saved lives. Loud also drew teeth. We lived anyway.”  
- *Verify first:* **IONA:** “The docket grew fangs. Some routes waited. Waiting is a cost we chose with open eyes.”  
- *Limited alert:* **VENN:** “Quiet warnings kept traffic breathing without a riot. Trust came slower. People still docked.”

**Echo 3 — No Easy Flag**  
- *Public:* **TAMSIN:** “We flew under no logo that owned us. Chaos answered the radio. I’ll take chaos that shares fuel.”  
- *Guild:* **ORU:** “Markers told the truth mid-cut. Measurement is a kind of courage.”  
- *Law:* **HALE:** “Suspend language worked often enough to matter. Often enough is the adult version of victory.”

**Echo 4 — Who Keeps the Light**  
- *Public custody:* **MIREK:** “Your commons is noisy. Noise is harder to forge than silence.”  
- *Guild custody:* **LYSARA:** “You chose continuity without my perfume. Impressive. Try to stay bored.”  
- *Law custody:* **VOSS** *(stand-down)* / **SABLE** *(flee)* / **TAMSIN** *(evacuate):* matching closer—testimony / “he’ll surface” / “he left the ground unsalted.”

**Closing joint line**  
**RYN:** It answered the empty space we left.  
**ORU:** Then we keep leaving room—and answering our own radios first.

---

### THREE POSTGAME PROJECT PITCHES

**1) Research Beacon Chain** — **ORU:** “Plant quiet listeners along the migration road. No amplifiers. No claims. Just better questions for the next cold signal.”

**2) Relief Route Compact** — **TAMSIN:** “Publish a volunteer fuel-and-medevac loop the supers can’t privatise without everyone noticing.”

**3) Salvage Tug Cooperative** — **NADI:** “Pull wrecks before companies invent ownership of other people’s disasters. Pip names the tug. I retain veto.”

**KEI:** Pick one when you’re ready. Free flight stays. The channel stays open. Berth six stays warm.

**VENN:** Undock when you like. Try to come back before the gossip invents your ending.

### BRANCH NOTES
- Epilogue tone: warm if ≥2 trust buckets ≥1 and Voss ≠ unresolved massacre fantasy; bittersweet if Voss fled + low Public; stern-hopeful if Law custody + stand-down.
- Project choice is cosmetic unlock + sandbox hook; does not rewind campaign.
- All essential services remain available regardless of custody.

---

# Branching Consequence Matrix

Persistent decisions: **D1** Price of Silence · **D2** What We Carry Home · **D3** No Easy Flag · **D4** Who Keeps the Light  
Trust buckets gain +1 on matching choice (Independent gains from soft credits / non-lethal play, not D1–D4 directly).

| Decision | Choice → bucket | Radio colour (mid/late) | Helpers | Prices / logistics | Voss (Ch.22) | Epilogue tone |
|----------|-----------------|-------------------------|---------|--------------------|--------------|---------------|
| **D1** Silence | Public | Civilian rumour nets; fast hazard corrections | Early volunteer spotters | Fuel slightly cheaper at independent docks | Soft ease on Evacuate if also D2 Public | Commons pride; thieves mentioned |
| | Guild | Peer-review bulletins; slower, precise | Notary checksums; early survey buoys | Survey tool discounts | Mild ease on all branches via cleaner evidence | Methodical; Kei self-checks control |
| | Law | Warrant language; Meridian counterclaims fail | Iona on short-range in diplomacy beats | Legal fine risk ↓; bribes rarer | **Major ease Stand-Down** | Docket warmth; stern hope |
| **D2** Carry Home | Broadcast | Loud settlement prep; Meridian alerted early | Medevac / tenders thicken | Insurance ↑ short-term; relief cargo ↑ | **Major ease Evacuate** | Loud gratitude; scars admitted |
| | Verify | Court-ready packets; delayed warnings | Stronger Ch.17–22 legal lines | Delayed route discounts | Ease Stand-Down if with D1 Law | Patient; “fangs grew” |
| | Limited | Quiet advisories; less Public trust | Fewer civilian wings | Stable prices; less chaos | Neutral | Subdued; survival without riot |
| **D3** Flag | Public | Convoy chatter; Reed wing callsigns | Fuel tenders; draw-off wings | Volunteer fuel priority | Ease Evacuate | Working-pilot warmth |
| | Guild | Marker pings; uncertainty headers | Cold corridors; ranging buoys | Repair kit discounts | Neutral / slight Stand-Down if evidence clean | Measured pride |
| | Law | Ceasefire calls; suspend texts | Lawful perimeter; fewer “pirate” mislabels | Fine forgiveness once | **Major ease Stand-Down** | Institutional hope / fear balanced |
| **D4** Light | Public | Open maintenance logs on civilian bands | Postgame relief compact strong | Commons fee: none; gossip tax: high | N/A (resolved) | Noisy commons; hard to forge |
| | Guild | Published uncertainty forever | Research beacon project strong | Peer subscription cosmetic only | N/A | Care-without-control cadence |
| | Law | Inspection calendar public | Salvage/legal recovery strong | Warrant hearings required to close lanes | N/A if Stand-Down: testimony coda | Watchers watching watchers |
| **Independent soft** | Non-lethal / Sable bargains / rescue par | Rook tips; quiet exits | Covert lanes; civilian Meridian exits | Favours ≠ credits | Flee less punitive; Evacuate corridor open | Crooked-plate humour; bargains kept |
| **Voss Flee default** | Low Law + low Public | Manhunt murmur; no loot hunt mission | Optional Sable tip (favour) | Bounty boards flavour only | **Flee** | Unfinished argument; lane still open |
| **Cross-tension** | D3 ≠ D4 | Characters name the contradiction | Still fully playable | No service lockout | Already resolved | Explicit “allies ≠ owners ≠ custodians” beat |

**Reading the matrix:** buckets are additive. Extreme split (e.g., D1 Law + D2 Broadcast + D3 Public + D4 Guild) yields hybrid radio and the Voss branch from the stronger of Law/Public at Ch.22 resolve time; epilogue plays all four echoes honestly.

---

# Mission Design Appendix

| Ch | Title | Objective kind | Fail / recovery | Evidence flags | Combat policy |
|----|-------|----------------|-----------------|----------------|---------------|
| 14 | A Map for Everyone | Dock + deposit | Leave & return; packets persist on ship | `MAP_NEUTRAL_DEPOSIT`; optional `BUYOUT_OFFER_LOGGED` | Optional only; refusal completes |
| 15 | Federal Measure | Dock + listen (+ timed rescue soft) | Skip rescue → thinner helpers; briefing alone OK | `HALE_TERMS_RECORDED`; `RESCUE_PAR` optional | None required; ambient pirates optional |
| 16 | Imperial Courtesy | Dock + listen | Always completable; gift ignore OK | `LYSARA_TERMS_RECORDED`; `SABLE_DEBT_ANNEX` optional | None; protocol docking only |
| 17 | Alliance of Necessity | Dock + audit test | Fail hash once → retry; cannot softlock | `MIREK_TERMS_RECORDED`; `AUDIT_PASS` | None; protest disperse nonlethal |
| 18 | The Coldest Signal | Scan (safe range) | Fire/close → tainted residual scan recovery | `COLDEST_SIGNAL_CLEAN` / `_TAINTED` | **Weapons discouraged**; no target loot ever |
| 19 | No Easy Flag | Choice (D3) | Must pick 1 of 3; no timer wipe | `FLAG_PUBLIC` / `_GUILD` / `_LAW` | None in planning room |
| 20 | The Black Flight | Hunt / break blockade (disable counts) | Fail-forward checkpoint; bounded retry fleets | `NEEDLE_REACH_CLEAR`; collateral soft flags | Prefer disable/draw-off; kill allowed not required |
| 21 | Aegis Echo | Scan / recover buoy | Damaged buoy → harder shutdown, still OK | `AEGIS_ECHO_SAFE_SHUTDOWN` | Scavengers only; buoy not a boss |
| 22 | The Open Channel | Multi-node disable + broadcast | Per-node checkpoints; retreat & return | `NODE_1..3`; `EVIDENCE_BROADCAST`; `VOSS_*` | Crews non-targets; tools>guns if safe shutdown |
| 23 | The Answering Dark | Scan echo | Pursuit fade → long-range recovery | `ANSWERING_DARK_CLEAN` / `_TAINTED` | **No engagement**; no loot |
| 24 | Who Keeps the Light | Choice (D4) | Must pick; losers keep map access | `CUSTODY_PUBLIC` / `_GUILD` / `_LAW` | None |
| 25 | A Berth Kept Warm | Home dock + epilogue pick | Always available after Ch.24 | Echo flags read-only; `POSTGAME_PROJECT_*` | None |

**Global policies**
- Essential evidence uses flags, never cargo slots.
- Chapter destination bound once and saved.
- Thargoid-adjacent contacts: scan/record only; never normal targets; never loot tables.
- Mycoid remains historical reference; never craftable.
- No chapter requires Voss’s death.
- Critical path never depends on random contract boards.

---

*End of Acts III–IV feature screenplay. Original dialogue throughout. Compatible with OPEN-CHANNEL-CAMPAIGN.md numbering offset: bible Ch.13–24 ≡ UI/saga Ch.14–25 (prologue counted).*


---

# PART D — Production checklist (playable)

- [x] Four choices with distinct UI labels (`saga_choice_label`)
- [x] Trust helpers + epilogue lines in playable build
- [x] Trust affects chapter rewards
- [x] Full Act I–IV screenplay merged as canonical long-form
- [ ] Future: unique non-target Thargoid contact entity
- [ ] Future: Voss stand-down as authored runtime state
- [ ] Future: postgame project picker
- [x] Display IMMEDIATE flip retained (anti-strobe)

## Credit

Original story for ELITE: NEXT. Elite / Elite Dangerous setting elements © Frontier Developments; used as historical backdrop under fan/homebrew context. No official affiliation. Do not paste proprietary GalNet or novel text into game assets.

*End of assembled screenplay authority.*
