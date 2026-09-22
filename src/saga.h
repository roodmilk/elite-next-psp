/* The Open Channel: data-driven main campaign.
 * Briefings follow ask-then-answer (same rhythm as the prologue): beat N+1
 * answers the commander's ask on beat N. Acts I–III (chs 02–19 / indices 0–17)
 * ship eight page-script beats with locked codas. Feature screenplay:
 * docs/OPEN-CHANNEL-SCREENPLAY.md */
enum { SAGA_DOCK, SAGA_SCAN, SAGA_HUNT, SAGA_HOME, SAGA_CHOICE };
enum { SAGA_BRIEF_BEATS = 8 };
enum { SAGA_TRUST_PUBLIC=0, SAGA_TRUST_GUILD=1, SAGA_TRUST_LAW=2, SAGA_TRUST_INDEPENDENT=3 };
typedef struct {
 const char *title,*speaker;
 /* Eight NPC beats (line..talk8), then eight commander replies (ask1..ask8). */
 const char *line,*talk2,*talk3,*talk4,*talk5,*talk6,*talk7,*talk8;
 const char *ask1,*ask2,*ask3,*ask4,*ask5,*ask6,*ask7,*ask8;
 const char *objective; unsigned char kind,role;
} SagaBeat;
static const SagaBeat saga_beats[]={
 /* Act I Ch.02 — page script */
 {"Something Worth Carrying","KEI",
  "Ryn left a sealed survey receiver with Mara at an agricultural dock — and she asked for someone who still answers when a call goes missing.",
  "For you, if you will take it. Mara held it because I could not leave Lave without abandoning the rest of the Guild board.",
  "Because Ryn skips meals and permits, not check-ins. Three quiet calls is not a mood. It is an alarm.",
  "No. Curiosity turns evidence into a story you cannot prove. Keep the seal until we are somewhere with witnesses.",
  "It smells official and timestamps eleven minutes early against the public bulletin. Ignore the detour if your tank allows.",
  "A sealed arrival and a name she can trust on the log. She will not ask what is inside. That kindness is rarer than jump fuel.",
  "Then you keep flying. Do not turn a delivery into a duel over stationery. The seal matters more than looking brave on a chart.",
  "Next step: deliver Mara's sealed receiver to Lave before someone rewrites the route around you.",
  "Who is she holding it for?",
  "Why would Ryn miss three calls?",
  "Should I open the case in flight?",
  "What about the hazard notice?",
  "What does Mara expect at the dock?",
  "And if someone tries to take it?",
  "Sealed and straight home. Understood.",
  "Accept the delivery run.",
  "Deliver Mara's sealed receiver",SAGA_DOCK,0},
 /* Act I Ch.03 — page script */
 {"The Quiet Between Pings","ORU",
  "The animals out there answer a navigation signal our charts still pretend is empty sky.",
  "The pauses between bursts. Silence is part of the message — people forget that when they own a transmitter.",
  "Correct. Lasers teach them that observation looks like hunting, and then they give you nothing useful.",
  "Then the observation resets. Living migration does not perform on demand, and we do not punish it for that.",
  "A marked ping in the destination system. Soft engines. Clean scan. Leave them the road afterward.",
  "Match their drift. When the ping sings, wait. When it stops, listen — a road-song, not a weapon signature.",
  "Amplification is shouting into a conversation. I claim interference I have measured, not pain I have not.",
  "Next step: scan the migration signal in the marked system without firing.",
  "What am I listening for?",
  "So I should not shoot at all?",
  "What if I scare them off?",
  "Where do I take the scan?",
  "How do I approach without noise?",
  "Are they being hurt by this?",
  "Quiet approach. No weapons.",
  "Begin the quiet scan.",
  "Scan a signal in the marked system",SAGA_SCAN,1},
 /* Act I Ch.04 — page script */
 {"All Hands Accounted For","TAMSIN",
  "My convoy followed a newly published safe route and lost a tender where the chart still claims the lane is clean.",
  "Mine. Working crews, not a flag exercise. I need their last port on a board, not another comforting rumour.",
  "Yes — the outbound stamp. Someone moved the route after we left, which is a person choosing who gets stranded.",
  "Pale Meridian sells clear lanes. Whether they moved this one or only profit from the lie, I want the stamp before it is scrubbed.",
  "If you can. Independent crews remember who counted them when the paperwork did not. Then pull the record.",
  "Lifeboat beacons flickered once, then Meridian advisories filled the band with convenient static. Optional — not a trap.",
  "I can fight a company with a timestamp. I cannot fight a shrug. Bring me the stamp even if the pods stay quiet.",
  "Next step: find the convoy's last port and pull the outbound record.",
  "Which convoy are we talking about?",
  "You need the last port stamp?",
  "Who moved the route on you?",
  "Should I look for pods too?",
  "What happened to the beacons?",
  "Stamp first — even without pods?",
  "I will get the stamp.",
  "Accept the search.",
  "Find the convoy's last port",SAGA_DOCK,2},
 /* Act I Ch.05 — page script */
 {"A Lawful Mistake","IONA",
  "Pale Meridian filed a theft report on Ryn's receiver that predates the cargo you are carrying.",
  "Because the filing is early on purpose, or a clerk failed — and I do not give corporations the benefit of tidy accidents.",
  "I will. A badge is not evidence — mine included. Bring the timestamps and we compare them in public light.",
  "Either. Both keep the evidence intact. Running turns you into their proof, so do not gift them that.",
  "The complete, unedited records back at Lave Hub, so the early filing cannot be washed into a polite story.",
  "Early reports are prophecy or paperwork weapons. We treat clocks as witnesses, not decoration.",
  "If they offer to settle quietly, refuse the quiet. Quiet is how an early badge becomes tomorrow's truth.",
  "Next step: bring the intact records to Iona at Lave Hub.",
  "How can theft predate the cargo?",
  "Will you stand with me on this?",
  "Inspection or pay under protest?",
  "What exactly do you need?",
  "Why do the clocks matter so much?",
  "What if they offer a quiet deal?",
  "I will keep the records clean.",
  "Return the records to Iona.",
  "Bring the records to Iona",SAGA_HOME,3},
 /* Act I Ch.06 — Voss tape + Pip key */
 {"The Last Useful Thing","NADI",
  "That wreck still has a recorder worth saving, and scavengers are already arguing about the shiny bits.",
  "Intact enough for a dump if you move before they strip the useful parts. Pip is watching the checklist like a hawk.",
  "A route key buried in maintenance notes like spare gasket stock — and a pointer to the unedited recording underneath.",
  "Meridian amplifying a biological navigation signal and selling the clear lanes. That is the useful thing. Scrap is noise.",
  "No. Scrap is how you become a second wreck. Pull the dump the moment the recorder answers, then leave.",
  "On the tape Voss calls predictability mercy with a spreadsheet. Pip found the route key under 'recalibrate audience.'",
  "If scavengers crowd you, talk once, then go. Useful truth does not need an audience of jackals.",
  "Next step: recover a fresh scan from the wreck and get clear.",
  "Is the wreck still intact?",
  "What did Pip find in there?",
  "What is on the recorder?",
  "Should I take scrap too?",
  "What did Voss say on the tape?",
  "What if scavengers press me?",
  "Dump and pull clear. Got it.",
  "Begin the wreck scan.",
  "Recover a fresh scan from the wreck",SAGA_SCAN,1},

 /* Act I Ch.07 — Sable bargain staging */
 {"The Price of Silence","SABLE",
  "Do not flinch. If I wanted your hull, you would already be a story people tell wrong.",
  "Safe passage through Meridian's favourite patrol habits — if the ledger stays dark long enough for my clients to move.",
  "Publish for the public. Peer review through Kei's Guild. Or lodge it with Iona where a badge cannot quietly rewrite the stamp.",
  "Kei wants light. Iona wants a chain of proof. The Guild is slower and, inconveniently, harder to bribe.",
  "I honour explicit bargains, including a refusal. I do not honour lectures. Public data also reaches pirates.",
  "I am on time for money. You are early for conscience. Cute overlap — choose before poetry invents itself.",
  "Intentions are interior decorating. Outcomes are architecture. Pick who holds the keys, then live in the building.",
  "Next step: choose who receives the route ledger.",
  "What are you offering me?",
  "What are my real options?",
  "Who actually wants this ledger?",
  "And if I refuse your deal?",
  "Why the hurry on this buoy?",
  "So the choice is permanent?",
  "I understand the cost.",
  "Make the custody choice.",
  "Choose who receives the route ledger",SAGA_CHOICE,2},

 /* Act II Ch.08 — clinic page script */
 {"No Cargo Is Just Cargo","MARA",
  "Clinic is on emergency batteries. The map says the lane is closed. The map is lying with a company accent.",
  "Patients who cannot jump. The cells are already paid for. The lane is the lie that is killing them.",
  "Escort, carry, or clear the interference buoy. Every route keeps them alive; the method only changes who trusts you later.",
  "To them? No. Lights on is lights on. To the rest of us watching? Yes — people remember how you opened a door.",
  "Interference transmitter dressed as a hazard beacon — Meridian subcontract plate. Log the serial if you clear it.",
  "If Meridian offers a faster lane with invoices attached, refuse the invoice. Clinics already paid once.",
  "Pick a verb. People are not a debate club. I will meet you on the pad with a boring lie if that opens the dock.",
  "Next step: reach the isolated settlement and open the dock.",
  "Who is stranded out there?",
  "How do I get them power?",
  "Does the method matter?",
  "What is on that buoy?",
  "What about Meridian invoices?",
  "So I just pick a verb?",
  "I will open that dock.",
  "Set course for the settlement.",
  "Reach the isolated settlement",SAGA_DOCK,0},

 /* Act II Ch.09 — GalCop page script */
 {"Ghosts of GalCop","VENN",
  "Welcome to the part of Lave tourists skip. The air tastes like old ink and unfinished apologies.",
  "Near the end the seal meant the witnesses had been promoted out of honesty — quieter cruelty than a loud pirate.",
  "Corruption, exhaustion, and records nobody could challenge. Institutions fail when the receipts stop meaning anything.",
  "Beacons that still answer. Not for poetry. For continuity. Press the poll. Hear who claims the lane.",
  "No. Nostalgia is a chemical. This is an inspection: each node, logged, before the signal dies of neglect.",
  "Meridian did not invent the sin. They leased it from history and called it innovation — same route-move trick, new letterhead.",
  "On the last morning the seal meant nobody knew who was responsible, and everyone still stamped the form.",
  "Next step: inspect the abandoned GalCop beacon chain.",
  "Why bring me down here?",
  "Why did GalCop fail, then?",
  "What should I listen for?",
  "Is this just nostalgia?",
  "Did Meridian invent this?",
  "What did the seal mean when it died?",
  "I will walk the chain.",
  "Begin the beacon survey.",
  "Inspect the abandoned beacon chain",SAGA_SCAN,1},

 /* Act II Ch.10 — mycoid page script */
 {"The Mycoid Ledger","IONA",
  "This box is not treasure. It is a confession with footnotes — history careful enough to sound like virtue if you read too quickly.",
  "A fungal weapon used against living Thargoid technology — and the pilot who was denied a return. It is history, not a catalogue.",
  "Because organic relays share ugly surface traits with that old work, and they would rather own the research than the guilt.",
  "Correct. Mycoid stays history in this ship and every ship after it. We do not craft atrocity into a trade advantage.",
  "Enough to warn: a weapon sold as a disablement; a pilot denied a return. Enough to forbid repetition. Not enough to repeat.",
  "If Meridian calls it stewardship research, translate: they want a weapon with better branding. Stamp DENIED and date it.",
  "Secure the crate sealed. Curiosity is how history becomes a recipe. I will not approve recipes.",
  "Next step: secure the sealed historical record.",
  "What is actually in the ledger?",
  "Why does Meridian want it?",
  "We will not weaponise this?",
  "What can we safely know?",
  "What if they call it research?",
  "Sealed — no peeking en route?",
  "I will secure it intact.",
  "Accept the archive run.",
  "Secure the sealed historical record",SAGA_DOCK,3},

 /* Act II Ch.11 — giants page script */
 {"Where the Giants Turn","ORU",
  "Cut your engines. We are visitors here. Visitors do not rearrange the furniture — or teach the giants that people are weather.",
  "They are large. Large is not the same as hostile. Your weapons will not make you larger — only louder.",
  "Three matched migration calls. Ryn wrote a shelter coordinate into their cadence the way other pilots write waypoints.",
  "Yes — living timing. She said patience is the password. Brilliant or unbearable, depending how long you have waited.",
  "By not herding and not firing. Log the matches, leave their road clear, and let patience be the instrument.",
  "Hear the pause? That pause is grammar. Meridian amplified the grammar until it screamed, then sold maps of the silence.",
  "When three match, you have a door, not a trophy. The first useful act was making room. Do not announce the door.",
  "Next step: observe the migration signal and log the matched calls.",
  "Why cut the engines?",
  "Are they dangerous?",
  "She wrote in their calls?",
  "How do I avoid scaring them?",
  "What am I hearing in the pauses?",
  "Matched calls stay quiet?",
  "Observe only. No force.",
  "Begin the quiet survey.",
  "Observe the migration signal",SAGA_SCAN,1},

 /* Act II Ch.12 — reunion page script */
 {"The Person on the Other End","RYN",
  "I am alive. The clever explanation can wait until you are close enough that Kei can shout at me without a relay.",
  "Yes. Also cold. Also furious. Also slightly proud of my handwriting in whale-song. Come anyway — tools, not speeches.",
  "Because the noise is selling our silence as a product. I vanished so Meridian would chase a ghost instead of a migration.",
  "The coords are rough on purpose. Geometry that stays precise gets leased. I will tighten them when you are close enough to matter.",
  "The local amplifier. Disable it when you arrive so we can talk in air that is not full of someone else's profit.",
  "If that dish keeps shouting, the giants turn into traffic hazards and Meridian sells the detour. Rude first. Then tea.",
  "Tell her she is allowed to be furious. You do not get to decide alone who I endanger by coming home loud — nor she alone.",
  "Next step: reach Ryn's shelter and shut the local amplifier down.",
  "You are really alive?",
  "Why did you go silent?",
  "How rough are those coords?",
  "What do I shut down there?",
  "Amplifier before repairs?",
  "Any message for Kei?",
  "I am on my way.",
  "Accept the shelter run.",
  "Reach Ryn's shelter",SAGA_DOCK,0},

 /* Act II Ch.13 — volume choice page script */
 {"What We Carry Home","KEI",
  "Ryn is safe — and her evidence makes us dangerous to people who sell maps as cages with reliable schedules.",
  "Yes. Safe, angry, and still herself. That is the win. The evidence is the part that can get us killed politely.",
  "Publish now and settlements prepare loudly. Verify first and the law grows teeth later. Limited alert is quieter.",
  "The law grows teeth later, while some lanes stay hungry in the meantime. Slower justice is still justice — if people survive.",
  "Quieter, less public trust, maybe less panic from Meridian before we are ready. I am professionally terrified. Choose the volume.",
  "I talk too much when I am scared. Treat that as data, not a leadership audition. Iona can work with any shape of truth — not a myth.",
  "Whatever you pick, we live with the map afterward. I am practising accepting an answer that is not mine.",
  "Next step: decide how openly to publish Ryn's evidence.",
  "She is really safe?",
  "What if we publish now?",
  "What if we verify first?",
  "What is the limited alert?",
  "Are you asking me to lead?",
  "Is the choice permanent?",
  "I understand the stakes.",
  "Make the publication choice.",
  "Decide how openly to publish",SAGA_CHOICE,0},

 /* Act III Ch.14 — map page script */
 {"A Map for Everyone","RYN",
  "Three copies. Same map. Same doubts printed in the margin. Nobody gets a cleaner version than anyone else.",
  "Federation, Empire, Alliance — each with a different smile. Copies go to a neutral archive with doubt and provenance attached.",
  "Neutral means shared custody of uncertainty, not a flag that pretends the sky is solved. No exclusives.",
  "They will. It will sound like relief until you notice the lock. Refusal is enough; you do not owe them a fight for saying no.",
  "Correct. Deposit the packets. Keep your hands free of anyone's exclusive stamp.",
  "Authoritative is a word people use when they want the others to go quiet. Messy truth beats polished hazard notices.",
  "If anyone tries to buy the master, log the offer as evidence. Romanticise the argument that kept the seal honest — not the seal.",
  "Next step: take copies to a neutral archive without granting exclusives.",
  "Who wants this map?",
  "Neutral archive — what does that mean?",
  "What about a Meridian buyout?",
  "So I refuse exclusives?",
  "Why attach doubt to every copy?",
  "What if they offer mid-dock?",
  "I will deliver the packets.",
  "Accept the archive run.",
  "Take copies to a neutral archive",SAGA_DOCK,3},

 /* Act III Ch.15 — Federal page script */
 {"Federal Measure","TAMSIN",
  "The Federation does not need your map. They need your lane not to kill civilians while three governments write letters.",
  "Patrol shells, rapid medevac, priority jump slots for relief — and operational telemetry shared for the crisis duration.",
  "That you will trade ship IDs, vector, cargo class for the jacket. Not cabin audio. Not private mail. Still a lease if you sleep.",
  "They will stage a rescue exercise. Watch the paperwork that makes the cost permanent while the benefit looks obvious.",
  "No. You are listening like someone who still has to fly tomorrow. Keep your own counsel until all three powers speak.",
  "Ugly doctrine: move first, publish second, apologise in footnotes. Ugly I can fly with. Invisible conditions I cannot.",
  "Record the telemetry clause verbatim. If cargo class becomes cargo contents, Iona wants the delta timestamped.",
  "Next step: hear the Federal envoy without committing a flag.",
  "What are they offering?",
  "What is the catch in the fine print?",
  "Will they show a rescue?",
  "Should I refuse them today?",
  "How ugly is their doctrine?",
  "What should Iona watch?",
  "I will listen carefully.",
  "Meet the Federal envoy.",
  "Hear the Federal envoy",SAGA_DOCK,2},

 /* Act III Ch.16 — Imperial page script */
 {"Imperial Courtesy","SABLE",
  "The Empire never calls a debt a chain. They call it courtesy, which lasts longer and photographs better.",
  "Long-term underwriting and personal surety — plus Imperial observers and first refusal on survey work for ten years.",
  "Yes. Courtesy is a contract. First refusal is a soft monopoly wearing perfume. Smile if you must; do not marry it.",
  "Whoever smiles while counting what you will owe when the map becomes inconvenient. That person is the real brief.",
  "Correct. Leave with information and your hands free. She says walk-away ends the underwriting — no chase. Rare and true.",
  "Walk-away is real. The lie by omission is preference: it reshapes every later contract around the named house.",
  "Hear her. Do not marry her. Continuity with a face still needs a charter that survives the funeral.",
  "Next step: hear the Imperial envoy without accepting a debt.",
  "What does their courtesy cost?",
  "Is the funding real?",
  "Who should I watch?",
  "We are not signing today?",
  "What about the walk-away?",
  "Any pirate advice?",
  "I will hear them out.",
  "Meet the Imperial envoy.",
  "Hear the Imperial envoy",SAGA_DOCK,2},

 /* Act III Ch.17 — Alliance page script */
 {"Alliance of Necessity","IONA",
  "The Alliance offers witnesses, not certainty — slower and less flattering than a flag that claims to already know.",
  "Public minutes and shared blame. Three mirrors, three jurisdictions. If any hash drifts, the session voids.",
  "Because witnesses matter when ambition meets a chart. If you cannot show your work, you do not get the lane.",
  "Yes. I will test their audit trail before you choose. Witnesses only matter if the record survives contact with ambition.",
  "Who can be cross-examined when this story becomes expensive — then decide with your eyes open.",
  "Ask who keeps the minutes when the minutes become dangerous. Put delay-notice requirements in the draft before you trust them.",
  "Coalition seat, not a coronation. Continuity without a single throat to choke. Listen to all three, then choose allies — not owners.",
  "Next step: hear the Alliance envoy and weigh their audit trail.",
  "What do they actually offer?",
  "Why prefer witnesses to certainty?",
  "Will you test their trail?",
  "What should I ask them?",
  "What is the dangerous question?",
  "Any last counsel before I choose?",
  "I will hear their case.",
  "Meet the Alliance envoy.",
  "Hear the Alliance envoy",SAGA_DOCK,3},

 /* Act III Ch.18 — cold signal page script */
 {"The Coldest Signal","PIP",
  "Signal structure: nonhuman. Confidence interval: troubling. Recommendation: curiosity without munitions.",
  "High enough to refuse a hunt. Low enough to refuse a sermon. Record. Leave a polite wake.",
  "Correct. It will not paint like ordinary target traffic. Turning it into a hunt teaches every later pilot the wrong first verb.",
  "It may paint like a contact. Treat the paint as a question, not a hunting licence. Oru calls it unresolved return.",
  "A clean sample, then home, before someone invents doctrine out of a waveform.",
  "Contact geometry unstable. Not debris. Not a filed beacon. Incomplete on purpose — or because we lack the sense it was made for.",
  "Firing solution was available. I deleted it from the helpful tips. Uncertainty is not a failure state. It is the only honest one.",
  "Next step: scan the silent contact and do not fire.",
  "Nonhuman — how sure are you?",
  "So no weapons at all?",
  "Will it look like a target?",
  "What do I bring back?",
  "What is it, then?",
  "Confirm weapons stay cold?",
  "Record and withdraw.",
  "Begin the silent scan.",
  "Scan the silent contact; do not fire",SAGA_SCAN,1},

 /* Act III Ch.19 — coalition choice page script */
 {"No Easy Flag","RYN",
  "Pick allies, not owners. We have to live with the difference after the speeches end and the fuel bills remain.",
  "Owners write your schedule. Allies stand on the channel without claiming the people who depend on it.",
  "Hold the relay under inspection. Prior trust brings fuel tenders, survey markers, or lawful ceasefire calls — help with a personality.",
  "Useful. Not free. Every option still supports a non-lethal finish; Meridian does not get to write the ending in wreckage.",
  "Yes. Choose who stands with us like someone who still wants a berth afterward.",
  "Public tenders, Guild markers, or lawful force — each helps without owning your callsign. Ideals do not hold a lane alone.",
  "Hold the line: allies, not owners. After the cut, we still have to dock somewhere that remembers our names.",
  "Next step: choose the relay coalition.",
  "Allies or owners — meaning?",
  "What does a coalition actually do?",
  "What does prior trust buy?",
  "Can we finish without killing?",
  "What are the three options?",
  "Any last advice?",
  "I understand the choice.",
  "Choose the coalition.",
  "Choose the relay coalition",SAGA_CHOICE,0},

 {"The Black Flight","VENN",
  "Meridian paint just crossed a public lane. That is not a security envelope. That is theft with thrusters.",
  "Pale Meridian interceptors — private hulls on a public ribbon, selling panic as procedure.",
  "Disable, outmanoeuvre, or draw them off while Tamsin's convoy clears the gate. Destruction counts; it is never the only verb.",
  "Confirmed. Progress saves if you have to breathe. Prefer lives over wreckage — I have buried enough grief under procedure.",
  "Then regroup on the far mark. The convoy still clears if you leave a road instead of a funeral. Fail-forward is allowed.",
  "Yes. If you can pull them off the public vector, do that first. Spectacle is cheaper than funerals — and harder to invoice.",
  "I will hold the tower channel. Prefer the living kind of argument when you come back.",
  "Next step: break the Meridian blockade on the public lane.",
  "Who crossed the public lane?",
  "How do you want it broken?",
  "Lives over wreckage — confirmed?",
  "What if I fail out there?",
  "Can I draw them off?",
  "Will you stay on channel?",
  "I will clear the lane.",
  "Engage the blockade.",
  "Break their blockade",SAGA_HUNT,2},
 {"Aegis Echo","ORU",
  "This buoy remembers when three powers tried to share a nightmare without sharing a single throat to choke.",
  "That shared science saves lives, and secrecy magnifies risk until risk looks like destiny. Rescue ships, not only sealed rooms.",
  "Because the alternative is owning a song until it kills someone. Observation first. Ownership never.",
  "A safer amplifier shutdown sequence than the one Meridian would sell you — triage and off-switches, not funerals.",
  "Treat the buoy as research, not a prize fight. Bring the packet home without declaring a war over understanding.",
  "Leave with the sample you have. A partial buoy beats a sermon written in wreckage. Shame is a start; upload before scrap.",
  "Shared science over sealed rooms. Say it aloud if you need the reminder under fire. Restraint is also research.",
  "Next step: recover the research buoy intact.",
  "What did Aegis learn the hard way?",
  "Why share the science?",
  "What does the buoy unlock?",
  "How do I avoid a fight?",
  "What if they contest it?",
  "Any mantra under pressure?",
  "I will recover it intact.",
  "Begin the buoy recovery.",
  "Recover the research buoy",SAGA_SCAN,1},
 {"The Open Channel","KEI",
  "Disable the amplifiers. Keep the crews alive. This is the cut where precision matters more than revenge.",
  "Correct. Dead amplifiers open roads. Dead crews only prove Voss's story that chaos needs a private jailer.",
  "That control created dependable routes. Answer with evidence and an open channel — dependence is not consent.",
  "With the truth on the wire and people still breathing. Not with a body count he can wave as proof he was necessary.",
  "Then get them clear. I am learning not to turn care into control — do not let me fail that lesson through you.",
  "No. He needs to be answerable. Stand-down, evacuate, or flee — never a required death. Precision over revenge.",
  "If crews are in the way, peel them free first. An open channel built on graves is just another cage.",
  "Next step: reach and clear the primary relay.",
  "Disable — not destroy?",
  "What will Voss claim?",
  "How do we answer him?",
  "What if crews are in the way?",
  "Does Voss have to die?",
  "Confirm: crews first?",
  "Precision over revenge.",
  "Assault the primary relay.",
  "Reach and clear the primary relay",SAGA_HUNT,0},
 {"The Answering Dark","RYN",
  "Something answered the empty space we left when the amplifiers died — and it may never have meant us at all.",
  "Maybe. Record the echo. Do not chase it into myth, doctrine, or a career made of pointing guns at uncertainty.",
  "Yes. Curiosity without restraint is how people invent wars out of waveforms. Log it. Hold still.",
  "No pursuit. No salvage. No defensive residue nonsense. Oru says our first useful act was making room — she is right.",
  "She means: we met them first by stopping a lie. Second useful act — not filling the dark with our need to be protagonists.",
  "A clean log and a warm berth. No sermons. That is the whole victory condition tonight.",
  "I will be on channel, allergic to being anyone's prize. Come home before myth invents itself.",
  "Next step: record the distant Thargoid echo and return.",
  "It answered us?",
  "So we only record?",
  "No pursuit at all?",
  "Oru said making room?",
  "What is the win tonight?",
  "Will you stay on channel?",
  "Record and leave.",
  "Begin the echo recording.",
  "Record the distant Thargoid echo",SAGA_SCAN,1},
 {"Who Keeps the Light","IONA",
  "Someone must hold the keys and accept inspection, or the open channel becomes private weather with better branding.",
  "Because unwatched power is how we got forged hazard notices. Custody without a charter is just a quieter Meridian.",
  "Public custody, Guild custody, or a lawful independent archive. Losers keep access to the public route data either way.",
  "No. Access stays. Fuel and repair stay. Trust changes tone, prices, and who answers when you call — never whether you have a home.",
  "Tone, prices, and the voice on the radio. Not the map. Not the berth. Choose who wears the keys with that in mind.",
  "Day one. No honeymoon for power. Whoever wins accepts inspection the moment you stop talking — that is the charter.",
  "Choose like someone who still wants a berth when the speeches end. I will watch the watchers.",
  "Next step: choose public, Guild, or lawful custody of the relay.",
  "Why does someone hold the keys?",
  "What are the three options?",
  "Do losers lose access?",
  "What does trust change?",
  "When does inspection start?",
  "Any last counsel?",
  "I will choose carefully.",
  "Choose the custodian.",
  "Choose public, Guild, or lawful custody",SAGA_CHOICE,3},
 {"A Berth Kept Warm","KEI",
  "You brought Ryn home. Venn is holding berth six permanently — his way of saying thank you without risking a feeling in public.",
  "Yes. Permanently. Procedure is how he survives gratitude. Boring allocation is the dream.",
  "A place to return to. I am still learning how to offer that without turning it into a leash.",
  "Yes. Free flight stays. The channel stays open. Another horizon is allowed — alone is optional.",
  "I will be on this frequency, talking too much when I am scared and precisely when I am not.",
  "Home is rarer than jump fuel. Take the berth. Refuse the leash. That is the whole lesson I have.",
  "When you are ready — not when I am lonely. I am practising that sentence until it sticks.",
  "Next step: return to Lave System Hub and claim the berth that stayed warm.",
  "Berth six is permanent?",
  "You are giving me a home?",
  "Does free flight still stay?",
  "Will you stay on channel?",
  "Is there a catch in the gift?",
  "When should I come back?",
  "I will come home to Lave.",
  "Return to Lave Hub.",
  "Return to Lave System Hub",SAGA_HOME,0},
};
#define SAGA_COUNT ((int)(sizeof(saga_beats)/sizeof(saga_beats[0])))
/* Choice chapters: 5 Silence, 11 Carry Home, 17 No Easy Flag, 22 Who Keeps Light. */
static const char *saga_choice_label(int chapter,int option){
 static const char *silence[3]={"Publish the ledger now","Give ledger to the Guild","Lodge ledger with Iona"};
 static const char *carry[3]={"Broadcast evidence now","Verify evidence first","Alert only active hazards"};
 static const char *flag[3]={"Public convoy network","Guild survey teams","Lawful supervised force"};
 static const char *light[3]={"Public custody + inspection","Explorers Guild custody","Lawful independent archive"};
 const char **table=light;
 if(chapter==5)table=silence;
 else if(chapter==11)table=carry;
 else if(chapter==17)table=flag;
 else if(chapter==22)table=light;
 if(option<0||option>2)option=0;
 return table[option];
}
/* Short consequence blurb for the highlighted permanent decision. */
static const char *saga_choice_blurb(int chapter,int option){
 static const char *silence[3]={
  "Loud truth. Settlements hear it — so do pirates.",
  "Slower peer review. Stamp survives. Meridian gains time.",
  "Evidence chain. Charges become possible. Release waits."};
 static const char *carry[3]={
  "Light the board. Clinics prepare loud — Meridian feels observed.",
  "Boring vault + chain. Teeth later. Meridian gets spin time.",
  "Active hazards only. Safer ops, less public trust, quieter counterplay."};
 static const char *flag[3]={
  "Civilian tenders on the clock. Ideals do not hold a lane alone.",
  "Guild survey markers — help without an ownership claim.",
  "Lawful force under inspection. Watchers get watched."};
 static const char *light[3]={
  "Commons with receipts. Charter starts the moment you stop talking.",
  "Guild keys plus inspection — or the joke ends.",
  "Lawful archive. Your name stays where it can be checked."};
 const char **table=light;
 if(chapter==5)table=silence;
 else if(chapter==11)table=carry;
 else if(chapter==17)table=flag;
 else if(chapter==22)table=light;
 if(option<0||option>2)option=0;
 return table[option];
}
/* Spoken reaction after a permanent decision — trust should be felt, not only counted. */
static const char *saga_choice_reaction(int chapter,int option){
 static const char *silence[3]={
  "Sable: Loud it is. Pirates will hear it too — that was always the price.",
  "Kei: Peer review is slower. I can live with slower if the stamp survives.",
  "Iona: Lodged. Now it is an evidence chain, not a rumour with better stationery."};
 static const char *carry[3]={
  "Kei: Then we warn them loud. Meridian will move — so will the people we care about.",
  "Iona: Verify first. Teeth later. I will keep the clock honest.",
  "Ryn: Limited alert. Less trust, fewer fireworks. I can live with careful."};
 static const char *flag[3]={
  "Tamsin: Convoy network it is. Working pilots show up when the fuel is real.",
  "Kei: Guild survey teams. Markers, not owners. That is the deal.",
  "Iona: Lawful force under inspection. I will watch the watchers."};
 static const char *light[3]={
  "Kei: Public custody with a charter. The sky stays a commons with receipts.",
  "Kei: Guild custody. We hold keys and accept inspection — or I resign the joke.",
  "Iona: Lawful archive. Your name stays where it can be checked."};
 const char **table=light;
 if(chapter==5)table=silence;
 else if(chapter==11)table=carry;
 else if(chapter==17)table=flag;
 else if(chapter==22)table=light;
 if(option<0||option>2)option=0;
 return table[option];
}
/* Chapter-complete radio — character voice, not a menu status string. */
static const char *saga_close_line(int chapter){
 static const char *c[SAGA_COUNT]={
  "Kei: Case is home. Do not get used to sealed things staying sealed forever.",
  "Oru: Good scan. You left them the road. That is the whole lesson.",
  "Tamsin: Stamp recovered. My people are not weather. Thank you.",
  "Iona: Records landed. Early filing does not get to become truth today.",
  "Nadi: Dump is clean. Pip is insufferably pleased. Move on.",
  "Sable: Choice logged. Live with the map you just voted for.",
  "Mara: Dock open. Lights beat paperwork. Remember that.",
  "Venn: Beacon chain logged. History only works if someone keeps receipts.",
  "Iona: Ledger secured. History stays history — no craftable ghosts.",
  "Oru: Migration logged. Patience still counts as flying.",
  "Ryn: You found me. Kei is allowed to shout. You are allowed to dock.",
  "Kei: Volume chosen. We live with how loud we just became.",
  "Ryn: Packets delivered. No exclusive stamps. Good.",
  "Tamsin: Federal case heard. Keep your own counsel.",
  "Sable: Imperial courtesy heard. Hands still free — rare.",
  "Iona: Alliance case heard. Ask who can be cross-examined.",
  "Pip: Sample stored. No shots fired. Preference: continue that habit.",
  "Ryn: Coalition set. Allies, not owners. Hold that line.",
  "Venn: Lane clear enough to argue. Prefer the living kind of argument.",
  "Oru: Buoy recovered. Shared science over sealed rooms.",
  "Kei: Amplifiers down. Crews alive. That is the cut that matters.",
  "Ryn: Echo logged. We made room. Come home before myth invents itself.",
  "Iona: Custodian chosen. Inspection starts the moment you stop talking.",
  "Kei: Berth six is warm. You have a place to return to."
 };
 if(chapter<0)chapter=0;
 if(chapter>=SAGA_COUNT)chapter=SAGA_COUNT-1;
 return c[chapter];
}
/* Locked completion codas through Act IV berth (chs 02-25 / indices 0-23). */
enum { SAGA_ACT1_END = 5, SAGA_ACT2_END = 11, SAGA_ACT3_END = 17, SAGA_ACT4_END = 23 };
static int saga_coda_pending=-1;
static int saga_has_coda(int chapter){return chapter>=0&&chapter<=SAGA_ACT4_END;}
static const char *saga_coda_speaker(int chapter){
 static const char *s[]={"KEI","ORU","TAMSIN","IONA","NADI","SABLE","MARA","VENN","IONA","ORU","RYN","KEI","RYN","TAMSIN","SABLE","IONA","PIP","RYN","VENN","ORU","KEI","RYN","IONA","KEI"};
 if(!saga_has_coda(chapter))return "KEI";
 return s[chapter];
}
static const char *saga_coda_line1(int chapter){
 static const char *a[]={
  "Venn labelled the case into evidence cage three — habits from when the seal meant the lane belonged to everyone.",
  "The pause pattern matches Ryn's receiver. Someone is selling animal songs as clear lanes.",
  "They moved the safe line after we were committed. A timestamp is a weapon if you keep it alive.",
  "Early badges do not get to become tomorrow's truth. Clocks are witnesses. Treat them that way.",
  "On the tape Voss calls predictability mercy with a spreadsheet. Pip hid the key where clerks get bored.",
  "The ledger has a keeper now. Intentions decorate. Outcomes build the house you sleep in.",
  "Lane open. Batteries kissing mains. Dock fees waived — pride not. Lights beat paperwork.",
  "Meridian leased an old sin from history and called it innovation. Walk the chain before you salute ghosts.",
  "Custody sealed. Meridian stamped DENIED and dated after the denial. History stays history.",
  "Three matches. Shelter bearing plotted. We observed; we did not herd; we did not fire.",
  "Amplifier offline. Empty space is what consent sounds like when the universe is big.",
  "Volume chosen. Settlements, clocks, or quiet lanes — we live with the map we just voted for.",
  "Packets deposited. Same doubts in every margin. Map is for everyone — including people who will hate us for it.",
  "Federal packet logged. Ugly doctrine keeps people breathing. Invisible conditions still kill convoys.",
  "Imperial packet heard. Walk-away is real. Preference is the quiet monopoly — do not marry it.",
  "Alliance hashes tested. Delay notices required. Continuity without a single throat to choke.",
  "Unresolved return filed. No shots. Uncertainty kept honest — the only ending that is not a war.",
  "Coalition set. Allies, not owners. Hold that line when the cameras leave.",
  "Lane ugly but open. Convoy through the gate. Prefer the living kind of argument.",
  "Packet home. Shared science over sealed rooms — off-switches before funerals.",
  "Amplifiers dark. Crews breathing. Dependence is not consent — and Voss knows we said it aloud.",
  "Echo logged. Unresolved on purpose. We made room and did not fill it with a war.",
  "Custodian chosen. Inspection starts the moment you stop talking. Losers keep the map.",
  "Berth six is warm. Home without a leash. Come back when you are ready — not when I am lonely.",
 };
 if(!saga_has_coda(chapter))return "";
 return a[chapter];
}
static const char *saga_coda_line2(int chapter){
 static const char *b[]={
  "Next we compare living silence to that box. Quiet work. Harder than it sounds.",
  "Leave them the sky. Your engines are weather they did not request.",
  "I can fight a company with a stamp. Anger without evidence is just weather.",
  "Paper is about to get interested in your cargo history. That is a warning, not a threat from us.",
  "You have enough for a war of memos. Try not to start the other kind until Iona finishes her coffee.",
  "I honour refusals. I update prices. Live with the map you just voted for.",
  "Bless your hold. Curse anyone who seals it. Sable called the open lane weather — she was not wrong.",
  "Leave a note if you must: records require challengers. Then go home before the ghosts recruit you.",
  "Serious people built it the first time. We will not reenact the burial.",
  "Go to her. And remember: the first useful act was making room.",
  "Stay for tea if your plot allows. Then we decide what we carry home — and how loudly.",
  "I am over-explaining again. Berth six. Come home when you can.",
  "Come home before someone invents a new forgery with better stationery.",
  "Hear the Empire and Alliance before anyone signs a colour. Ideals do not hold a lane alone.",
  "Take the packet. Drink the tea. Sign nothing. Someone still pays when cameras leave.",
  "Listen to all three. Then choose who stands with you — not who owns you.",
  "If they heard the scream we made, leave a silence clearer than the lie.",
  "Prior trust brings tenders, markers, or ceasefires. Use them. Do not sell the callsign.",
  "Next problem wears a nicer suit. Precision over revenge starts with an open road.",
  "Bring the checklist to the Crown. Tools that prefer off-switches to funerals.",
  "Broadcast what you can prove. Hold the quiet you made — do not chase glory into capital guns.",
  "Come home before myth invents itself. File unknown and mean it.",
  "Wear the keys without pretending the choice was free of consequence.",
  "Free flight stays. The channel stays open. Alone is optional.",
 };
 if(!saga_has_coda(chapter))return "";
 return b[chapter];
}
static int saga_trust_total(const Game *g){
 return g->saga_trust[0]+g->saga_trust[1]+g->saga_trust[2]+g->saga_trust[3];
}
static int saga_dominant_trust(const Game *g){
 int best=0,v=g->saga_trust[0];
 for(int i=1;i<4;i++)if(g->saga_trust[i]>v){v=g->saga_trust[i];best=i;}
 return best;
}
static const char *saga_trust_helper(const Game *g){
 /* Visible coalition helpers unlocked by earlier permanent decisions. */
 if(g->saga_trust[SAGA_TRUST_PUBLIC]>=2)return "Civilian fuel tenders are standing by on your route.";
 if(g->saga_trust[SAGA_TRUST_GUILD]>=2)return "Guild survey markers are already painting your next hops.";
 if(g->saga_trust[SAGA_TRUST_LAW]>=2)return "Lawful ceasefire calls are ready against Meridian interceptors.";
 if(g->saga_trust[SAGA_TRUST_INDEPENDENT]>=1)return "Sable's covert corridor is open if you need a quiet road.";
 return "You fly with the people who decided to trust you.";
}
static const char *saga_epilogue_line(const Game *g){
 switch(saga_dominant_trust(g)){
 case SAGA_TRUST_PUBLIC:return "Public relays still answer when your callsign clears the noise.";
 case SAGA_TRUST_GUILD:return "Guild charts still mark your berth as a place worth returning to.";
 case SAGA_TRUST_LAW:return "Iona's audit trail still carries your name where it can be checked.";
 default:return "Independents still leave a warm berth for the pilot who counted them.";
 }
}
static int saga_voice_who(const SagaBeat *b){
 if(!b)return VOICE_CONTACT;
 if(b->role==0)return VOICE_KEI;
 if(b->role==3)return VOICE_LAW;
 return VOICE_CONTACT;
}
static int saga_system(const Game *g,int chapter){
 int want=(chapter*37+19)&255,best=7,score=999999;
 for(int i=0;i<256;i++){if(i==7)continue;int d=(int)(distance_ly(g,7,i)*10);int s=abs(i-want)+d/4;if(d>=25&&d<=180&&s<score){score=s;best=i;}}
 return best;
}
/* Story navigation must remain explainable even with an almost empty tank.
 * Plan against the fitted drive, then let the map mark the next hop LOW until
 * the player refuels.  The selectable marker is always a visible local star. */
static int saga_next_hop(const Game *g,int *jumps){
 Game route=*g;route.fuel=(float)player_ships[g->ship].range;
 return route_next_hop(&route,g->saga_dest,jumps);
}
static void saga_begin(Game *g){
 if(g->campaign_stage<6||g->saga_chapter>=SAGA_COUNT||g->saga_step)return;
 const SagaBeat *b=&saga_beats[g->saga_chapter];g->saga_dest=(b->kind==SAGA_HOME)?7:saga_system(g,g->saga_chapter);
 g->saga_start=(b->kind==SAGA_HUNT)?g->npc_kills:g->discoveries;g->saga_step=1;
 message(g,b->objective);speak(g,saga_voice_who(b),b->talk8);
 if(g->saga_chapter>=17)message(g,saga_trust_helper(g));
}
static int saga_ready(const Game *g){
 if(!g->saga_step||g->saga_chapter>=SAGA_COUNT)return 0;
 const SagaBeat *b=&saga_beats[g->saga_chapter];
 if(b->kind==SAGA_CHOICE)return g->saga_choice>0;
 if(b->kind==SAGA_HOME)return g->docked&&g->system==7;
 if(b->kind==SAGA_DOCK)return g->docked&&g->system==g->saga_dest;
 if(b->kind==SAGA_SCAN)return g->system==g->saga_dest&&g->discoveries>g->saga_start;
 return g->system==g->saga_dest&&g->npc_kills>g->saga_start;
}
static int saga_advance(Game *g){
 if(!saga_ready(g))return 0;
 const SagaBeat *b=&saga_beats[g->saga_chapter];
 int finished=g->saga_chapter,choice=g->saga_choice-1;
 if(b->kind==SAGA_CHOICE){
  /* Map each choice chapter onto a trust bucket: Public / Guild / Law.
   * Carry-Home option 2 (limited alert) also nudges Independent trust. */
  if(g->saga_chapter==11&&choice==2)g->saga_trust[SAGA_TRUST_INDEPENDENT]++;
  else if(choice>=0&&choice<3)g->saga_trust[choice]++;
  g->saga_flags|=1u<<(g->saga_chapter==5?0:g->saga_chapter==11?1:g->saga_chapter==17?2:3);
 }
 /* Prior trust softens later payouts into tangible coalition support. */
 int reward=1800+g->saga_chapter*120+saga_trust_total(g)*80;
 if(g->credits<=100000000-reward)g->credits+=reward;
 g->saga_chapter++;g->saga_step=0;g->saga_choice=0;g->cue=SFX_SELECT;
 if(g->saga_chapter>=SAGA_COUNT){
  message(g,"The Open Channel is complete. Free flight continues with the berth still warm.");
  speak(g,VOICE_KEI,saga_epilogue_line(g));
 }else{
  const char *line=b->kind==SAGA_CHOICE?saga_choice_reaction(finished,choice):saga_close_line(finished);
  const char *body=strchr(line,':');
  speak(g,saga_voice_who(b),body&&body[1]==' '?body+2:line);
  message(g,"Chapter complete. The next briefing is ready when you are.");
  if(saga_has_coda(finished))saga_coda_pending=finished;
 }
 return 1;
}
