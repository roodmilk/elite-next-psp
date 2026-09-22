/* The Open Channel: data-driven main campaign.
 * Locked six-beat briefings use full spoken sentences (wrap on the Story screen).
 * Feature screenplay authority: docs/OPEN-CHANNEL-SCREENPLAY.md */
enum { SAGA_DOCK, SAGA_SCAN, SAGA_HUNT, SAGA_HOME, SAGA_CHOICE };
enum { SAGA_BRIEF_BEATS = 6 };
enum { SAGA_TRUST_PUBLIC=0, SAGA_TRUST_GUILD=1, SAGA_TRUST_LAW=2, SAGA_TRUST_INDEPENDENT=3 };
typedef struct {
 const char *title,*speaker;
 /* Six NPC beats (line..talk6), then six commander replies (ask1..ask6). */
 const char *line,*talk2,*talk3,*talk4,*talk5,*talk6;
 const char *ask1,*ask2,*ask3,*ask4,*ask5,*ask6;
 const char *objective; unsigned char kind,role;
} SagaBeat;
static const SagaBeat saga_beats[]={
 {"Something Worth Carrying","KEI",
  "Ryn left a sealed survey receiver with Mara at an agricultural dock, and she asked for someone who still answers when a call goes missing.",
  "She has missed three scheduled check-ins. Ryn skips meals and permits, Commander — she does not skip calls.",
  "Mara kept the case sealed because curiosity turns evidence into a story you cannot prove in front of anyone who matters.",
  "There is a fresh hazard notice on your outbound that smells official and timestamps eleven minutes early against the public bulletin.",
  "Dock with Mara, take the case as boring agricultural sensors if anyone asks, and bring it home without opening it.",
  "Next step: deliver Mara's sealed receiver to Lave before someone rewrites the route around you.",
  "Who is Mara holding it for?","Why would she miss three calls?","Should I open the case?","What about that hazard notice?","I will keep it sealed.","Accept the delivery run.",
  "Deliver Mara's sealed receiver",SAGA_DOCK,0},
 {"The Quiet Between Pings","ORU",
  "The animals out there answer a navigation signal our charts still pretend is empty sky, and the pauses between bursts are part of the message.",
  "If you paint the herd with lasers, you will teach them that observation looks like hunting, and they will give you nothing useful.",
  "Approach too hot and the observation simply resets — not as punishment, but because living migration does not perform on demand.",
  "I need a clean scan of the marked ping while your engines stay quiet enough that you remain a visitor instead of a threat.",
  "Record what they sing between the beacons, then leave them the road. Silence is data, not an invitation to fill it.",
  "Next step: scan the migration signal in the marked system without firing.",
  "What am I listening for?","So I should not shoot?","What if I scare them?","Where is the marked ping?","I will keep engines soft.","Begin the quiet scan.",
  "Scan a signal in the marked system",SAGA_SCAN,1},
 {"All Hands Accounted For","TAMSIN",
  "My convoy followed a newly published safe route and lost a tender where the chart still claims the lane is clean.",
  "Their last port still holds the outbound stamp. I need that place on a board, not another tidy rumour sold as comfort.",
  "Someone moved the route after we left. That is not a clerical shrug — that is a person choosing who gets stranded.",
  "If you can recover every pod, do it. Independent crews remember who counted them when the paperwork did not.",
  "Find the stamp before someone scrubs the log and turns my people into a weather report.",
  "Next step: find the convoy's last port and pull the outbound record.",
  "Which convoy went missing?","You need the last port?","Who moved the route?","I will look for pods.","I will get the stamp.","Accept the search.",
  "Find the convoy's last port",SAGA_DOCK,2},
 {"A Lawful Mistake","IONA",
  "Pale Meridian filed a theft report on Ryn's receiver that predates the cargo you are carrying, and that is not a coincidence I am willing to ignore.",
  "A badge is not evidence — mine included. Bring me the timestamps, and I will stand with you while we compare them in public light.",
  "You can submit to inspection or pay under protest. Either path keeps the evidence intact; running turns you into their proof.",
  "I need the records back at Lave, complete and unedited, so the early filing cannot be washed into a tidy story.",
  "Prove the report is a lawful mistake. Then we decide who gets to keep pretending it was honest.",
  "Next step: bring the intact records to Iona at Lave Hub.",
  "How can a theft predate cargo?","Will you stand with me?","Inspection or protest?","What do you need from me?","I will keep the records clean.","Return the records to Iona.",
  "Bring the records to Iona",SAGA_HOME,3},
 {"The Last Useful Thing","NADI",
  "That wreck still has a recorder worth saving, and scavengers are already arguing about who gets the shiny bits first.",
  "Scan before they strip the useful parts. Pip found a route key buried in a maintenance checklist like it was spare gasket stock.",
  "I want a clean dump of the unedited recording — the one that shows Meridian amplifying a biological navigation signal and selling the clear lanes.",
  "Do not linger for scrap. Scrap is how you become a second wreck with a better story and a worse outcome.",
  "Pull clear the moment the recorder answers. We can mourn metal later; we cannot remake a deleted truth.",
  "Next step: recover a fresh scan from the wreck and get clear.",
  "Is the wreck still intact?","What did Pip find?","What is on the recorder?","Should I take scrap too?","I will pull the dump and leave.","Begin the wreck scan.",
  "Recover a fresh scan from the wreck",SAGA_SCAN,1},
 {"The Price of Silence","SABLE",
  "Meridian pays well for routes nobody questions, and silence is a product they package with better stationery than most governments manage.",
  "You can sell that silence, hand the ledger to people who will argue about it, or lodge it where a badge cannot quietly rewrite the stamp.",
  "Kei wants light. Iona wants a chain of proof. The Guild offers peer review that is slower and, inconveniently, harder to bribe.",
  "I will honour a refusal. Public data also reaches pirates, so do not pretend transparency is free of teeth.",
  "Choose who holds the ledger. Choose like someone who has to live with the map afterward.",
  "Next step: choose who receives the route ledger.",
  "What are you offering?","What are my real options?","Who wants the ledger?","And if I refuse you?","I understand the cost.","Make the custody choice.",
  "Choose who receives the route ledger",SAGA_CHOICE,2},
 {"No Cargo Is Just Cargo","MARA",
  "A clinic has power cells sitting on a dock and no open lane, which is a paperwork problem until someone starts dying on a schedule.",
  "You can escort, carry, or clear the interference buoy. Every route keeps them alive; the method only changes who trusts you afterward.",
  "People out there do not care which flag looks clean on a briefing slide. They care whether the lights come back on.",
  "Reach the settlement, open the dock, and treat the cargo like a promise instead of a line item.",
  "If this stays a form, the form will outlive the patients. I would rather the opposite.",
  "Next step: reach the isolated settlement and open the dock.",
  "Who is stranded?","How do I get them power?","Does the method matter?","What do they need first?","I will open that dock.","Set course for the settlement.",
  "Reach the isolated settlement",SAGA_DOCK,0},
 {"Ghosts of GalCop","VENN",
  "The old GalCop beacons still remember better days, when the seal meant the lane belonged to everyone and not to whoever paid for the paint.",
  "Near the end it meant nobody knew who was responsible, and that is a quieter kind of cruelty than a pirate with a loud radio.",
  "Walk the abandoned chain. Listen for what still answers. Institutions fail when their records cannot be challenged.",
  "I am not asking you for nostalgia. I am asking you to inspect each node before the signal finishes dying of neglect.",
  "If something still sings under the old seal, log it. History is only useful when somebody bothers to keep the receipts.",
  "Next step: inspect the abandoned GalCop beacon chain.",
  "What did the seal mean?","Why did it fail?","What should I listen for?","Is this nostalgia?","I will walk the chain.","Begin the beacon survey.",
  "Inspect the abandoned beacon chain",SAGA_SCAN,1},
 {"The Mycoid Ledger","IONA",
  "INRA won a war and buried the price of it in language careful enough to sound like virtue if you read it too quickly.",
  "This record is heavy. Meridian wants organic-relay research because living navigation shares a few ugly surface traits with old bioweapon work.",
  "Mycoid stays history in this ship and every ship after it. We do not craft a weapon out of someone else's atrocity to win a trade dispute.",
  "Secure the sealed ledger, then decide who may read it without turning it into a catalogue of options.",
  "Refuse the research pitch. Bring me something that can be audited, not something that can be reverse-engineered into harm.",
  "Next step: secure the sealed historical record.",
  "What is in the ledger?","Why does Meridian want it?","We will not weaponise this?","Who should read it?","I will secure it intact.","Accept the archive run.",
  "Secure the sealed historical record",SAGA_DOCK,3},
 {"Where the Giants Turn","ORU",
  "Cut your engines. We are visitors here, and visitors who announce themselves with thrust teach the giants that people are weather.",
  "Observe. Do not herd. Do not fire. Three matched calls hide a shelter coordinate Ryn wrote into their migration cadence.",
  "Read carefully. She used living timing the way other pilots use waypoints, which is either brilliant or unbearable depending on how long you have waited.",
  "Log the migration and leave their road clear. If you force a path, you will erase the message you came to recover.",
  "When the third call lines up, you will know where to look. Until then, patience is the instrument.",
  "Next step: observe the migration signal and log the matched calls.",
  "Why cut engines?","What am I matching?","She wrote in their calls?","How do I avoid scaring them?","I will observe only.","Begin the quiet survey.",
  "Observe the migration signal",SAGA_SCAN,1},
 {"The Person on the Other End","RYN",
  "I am alive. The clever explanation can wait until you are close enough that Kei can shout at me without using a relay.",
  "Shelter coordinates are rough because amplifiers scramble polite geometry. Come anyway. Bring tools, not speeches.",
  "Kei will be furious, and she is allowed. Asking you to find me also put a ship under you, so try not to spend that gift on drama.",
  "Disable the local amplifier when you arrive. I can talk properly once the noise stops selling our silence as a product.",
  "Reach me. We can unpack the rest when there is air between us that is not full of someone else's profit.",
  "Next step: reach Ryn's shelter and shut the local amplifier down.",
  "You are really alive?","How rough are the coords?","Kei has been looking.","What do I shut down?","I am on my way.","Accept the shelter run.",
  "Reach Ryn's shelter",SAGA_DOCK,0},
 {"What We Carry Home","KEI",
  "Ryn is safe, and her evidence makes us dangerous to people who sell maps as cages with reliable schedules.",
  "Publish now and settlements can prepare — loudly. Verify first and the law grows teeth later, while some lanes stay hungry in the meantime.",
  "A limited alert is quieter and earns less public trust. It may also keep Meridian from panicking into something worse before we are ready.",
  "I talk too much when I am scared. Today I am professionally terrified, which is close enough to honesty.",
  "Decide how openly this goes out. We live with the volume of that choice.",
  "Next step: decide how openly to publish Ryn's evidence.",
  "She is really safe?","What if we publish now?","What if we wait to verify?","What is the quiet option?","I understand the stakes.","Make the publication choice.",
  "Decide how openly to publish",SAGA_CHOICE,0},
 {"A Map for Everyone","RYN",
  "Three powers want one map, and none of them should own the only copy of how living things move through this sky.",
  "Take packets to a neutral archive. No exclusives. Each copy carries doubt and provenance so nobody can pretend ownership is the same as care.",
  "Meridian will offer a buyout that sounds like relief until you notice the relief has a lock on it.",
  "Refusal is enough. You do not have to pick a flag to keep a road from becoming private weather.",
  "Deliver the packets. Then we still have a chance to argue like adults instead of tenants.",
  "Next step: take copies to a neutral archive without granting exclusives.",
  "Who wants the map?","Neutral archive — where?","What about a buyout?","So I refuse exclusives?","I will deliver the packets.","Accept the archive run.",
  "Take copies to a neutral archive",SAGA_DOCK,3},
 {"Federal Measure","TAMSIN",
  "The Federation offers security with conditions, which is another way of saying the fine print arrives wearing a rescue jacket.",
  "Patrol coverage and rapid logistics are real. So is the surveillance language that travels in the same briefing folder.",
  "They will stage a rescue exercise that makes the benefit obvious. Watch the paperwork that makes the cost permanent.",
  "Hear the envoy. Keep your own counsel. Working crews survive by noticing when help starts looking like a lease.",
  "You are not signing anything today. You are listening like someone who still has to fly tomorrow.",
  "Next step: hear the Federal envoy without committing a flag.",
  "What are they offering?","What is the catch?","Is the rescue real?","Should I refuse them?","I will listen carefully.","Meet the Federal envoy.",
  "Hear the Federal envoy",SAGA_DOCK,2},
 {"Imperial Courtesy","SABLE",
  "The Empire never calls a debt a chain. They call it courtesy, which lasts longer and photographs better.",
  "Funding arrives with nicer stationery than most pirates manage, and the expectation arrives in the same crate.",
  "Hear them out. Remember who smiles while they count what you will owe when the map becomes inconvenient.",
  "You are not signing anything today. Smile back if you must — just do not confuse manners with acquittal.",
  "Leave with information and your hands free. That is a rarer gift than patronage.",
  "Next step: hear the Imperial envoy without accepting a debt.",
  "What does courtesy cost?","Is the funding real?","Who should I watch?","We are not signing?","I will hear them out.","Meet the Imperial envoy.",
  "Hear the Imperial envoy",SAGA_DOCK,2},
 {"Alliance of Necessity","IONA",
  "The Alliance offers witnesses, not certainty, which is slower and less flattering than a flag that claims to already know.",
  "Public minutes and shared blame are inconvenient until the powerful rewrite a map and you need someone who kept the old stamp.",
  "I will test their audit trail before you choose. Witnesses only matter if the record survives contact with ambition.",
  "Hear the Alliance case. Ask who can be cross-examined when this story becomes expensive.",
  "Then decide with your eyes open, not with a slogan that fits neatly on a status line.",
  "Next step: hear the Alliance envoy and weigh their audit trail.",
  "What do they actually offer?","Why prefer witnesses?","Will you test them?","What should I ask?","I will hear their case.","Meet the Alliance envoy.",
  "Hear the Alliance envoy",SAGA_DOCK,3},
 {"The Coldest Signal","PIP",
  "Signal structure: nonhuman. Confidence: troubling. Recommendation: do not fire, do not narrate, record and leave a polite wake.",
  "It will not paint like ordinary target traffic. News will call it worse than evidence; Oru calls it evidence that still deserves room.",
  "Scan the silent contact. Withdraw. If you turn this into a hunt, you teach every later pilot the wrong first verb.",
  "I am a maintenance drone. I prefer problems that admit a wrench. This one admits a recorder and restraint.",
  "Get the clean sample and come home before someone invents a war out of a waveform.",
  "Next step: scan the silent contact and do not fire.",
  "Nonhuman — how sure?","So no weapons?","Will it look like a target?","What do I bring back?","Record and withdraw. Got it.","Begin the silent scan.",
  "Scan the silent contact; do not fire",SAGA_SCAN,1},
 {"No Easy Flag","RYN",
  "Pick allies, not owners. We have to live with the difference after the speeches end and the fuel bills remain.",
  "A coalition can hold a relay without owning the people who depend on it, if you choose partners who accept inspection.",
  "Prior trust brings fuel tenders, survey markers, or lawful ceasefire calls. Useful help still has a personality.",
  "Every option supports a non-lethal finish. That is not softness — that is refusing to let Meridian write the ending in wreckage.",
  "Choose who stands on the channel with us. Choose like someone who still wants a berth afterward.",
  "Next step: choose the relay coalition.",
  "Allies or owners — meaning?","What does a coalition do?","What does prior trust buy?","Can we finish without killing?","I understand the choice.","Choose the coalition.",
  "Choose the relay coalition",SAGA_CHOICE,0},
 {"The Black Flight","VENN",
  "Meridian interceptors crossed the public lane like the lane belonged to their shareholders and nobody else had standing to complain.",
  "Break the blockade. Prefer lives over wreckage. Disable, outmanoeuvre, or draw them off — destruction counts, and it is never the only verb.",
  "Clear the lane so the packets can move without turning every independent into collateral for a private chart.",
  "I have buried enough grief under procedure for one career. Do not make me file another quiet report that sounds like weather.",
  "Open the road. Then we can argue about custody with people who are still breathing.",
  "Next step: break the Meridian blockade on the public lane.",
  "Who crossed the lane?","How do I break them?","Lives over wreckage?","What happens if I fail?","I will clear the lane.","Engage the blockade.",
  "Break their blockade",SAGA_HUNT,2},
 {"Aegis Echo","ORU",
  "Aegis learned — slowly, expensively — that knowledge needs rescue ships too, not only classifications and sealed rooms.",
  "Shared science saves lives. Secrecy magnifies risk until the risk looks like destiny and nobody remembers who chose it.",
  "The research buoy still sings. Recover it intact; it unlocks a safer amplifier shutdown sequence than the one Meridian would sell you.",
  "Bring the research home without declaring a war over the right to understand a song.",
  "Observation first. Ownership never. That is the whole lesson, repeated until institutions hear it.",
  "Next step: recover the research buoy intact.",
  "What did Aegis learn?","Why share the science?","What does the buoy unlock?","How do I avoid a fight?","I will recover it intact.","Begin the buoy recovery.",
  "Recover the research buoy",SAGA_SCAN,1},
 {"The Open Channel","KEI",
  "Disable the amplifiers. Keep the crews alive. This is the cut where precision matters more than the satisfaction of revenge.",
  "Voss will argue that private control created dependable routes. Dependence is not consent, and dependable cages are still cages.",
  "Answer him with evidence and an open channel, not with a body count that lets him claim chaos as proof he was necessary.",
  "Reach the primary relay and clear it. I taught myself not to turn care into control — do not let me fail that lesson through you.",
  "When the noise stops, people get to choose a road again. That is the whole point of keeping anyone alive through this.",
  "Next step: reach and clear the primary relay.",
  "Disable — not destroy?","What will Voss claim?","How do we answer him?","What if crews are in the way?","Precision over revenge. Agreed.","Assault the primary relay.",
  "Reach and clear the primary relay",SAGA_HUNT,0},
 {"The Answering Dark","RYN",
  "Something answered the empty space we left when the amplifiers died, and it may never have meant us at all.",
  "Record the echo. Do not chase it into myth, doctrine, or a career made of pointing guns at uncertainty.",
  "Oru says our first useful act was making room. I hate how often she is right while sounding gentle about it.",
  "Log the distant contact and come home. Curiosity without restraint is how people invent wars out of waveforms.",
  "If it meant us, we will know later. If it did not, we still owed the sky a quieter night.",
  "Next step: record the distant Thargoid echo and return.",
  "It answered us?","So we only record?","Oru said making room?","Then we go home?","Record and leave. Understood.","Begin the echo recording.",
  "Record the distant Thargoid echo",SAGA_SCAN,1},
 {"Who Keeps the Light","IONA",
  "Someone must hold the keys and accept inspection, or the open channel becomes another private weather system with better branding.",
  "Public custody, Guild custody, or a lawful independent archive — pick one. The losers keep access to the public route data either way.",
  "Trust changes tone, prices, and who answers when you call. It does not change whether fuel and repair remain available.",
  "Choose custody. Then live with the watchers, because unwatched power is how we arrived at forged hazard notices in the first place.",
  "I will enforce the inspection charter. You choose who wears the keys without pretending the choice is free of consequence.",
  "Next step: choose public, Guild, or lawful custody of the relay.",
  "Why does someone hold keys?","What are the three options?","Do losers lose access?","What does trust change?","I will choose carefully.","Choose the custodian.",
  "Choose public, Guild, or lawful custody",SAGA_CHOICE,3},
 {"A Berth Kept Warm","KEI",
  "You brought Ryn home. Venn is holding berth six permanently, which is his way of saying thank you without risking a feeling in public.",
  "Asking you to find her also gave you a place to return to, and I am still learning how to offer that without turning it into a leash.",
  "Free flight stays. The channel stays open. There will always be another horizon, and you do not have to face it alone unless you choose to.",
  "Come back to Lave Hub when you are ready. I will be on this frequency, talking too much when I am scared and precisely when I am not.",
  "This berth is yours. Fly like you mean to come home — not because I require it, but because home is rarer than jump fuel.",
  "Next step: return to Lave System Hub and claim the berth that stayed warm.",
  "Berth six is permanent?","You are giving me a home?","Does free flight stay?","Will you stay on channel?","I will come home to Lave.","Return to Lave Hub.",
  "Return to Lave System Hub",SAGA_HOME,0}
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
 message(g,b->objective);speak(g,b->role==0?VOICE_KEI:b->role==3?VOICE_LAW:VOICE_CONTACT,b->talk6);
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
 if(b->kind==SAGA_CHOICE){
  int c=g->saga_choice-1;
  /* Map each choice chapter onto a trust bucket: Public / Guild / Law.
   * Carry-Home option 2 (limited alert) also nudges Independent trust. */
  if(g->saga_chapter==11&&c==2)g->saga_trust[SAGA_TRUST_INDEPENDENT]++;
  else if(c>=0&&c<3)g->saga_trust[c]++;
  g->saga_flags|=1u<<(g->saga_chapter==5?0:g->saga_chapter==11?1:g->saga_chapter==17?2:3);
 }
 /* Prior trust softens later payouts into tangible coalition support. */
 int reward=1800+g->saga_chapter*120+saga_trust_total(g)*80;
 if(g->credits<=100000000-reward)g->credits+=reward;
 g->saga_chapter++;g->saga_step=0;g->saga_choice=0;g->cue=SFX_SELECT;
 if(g->saga_chapter>=SAGA_COUNT){
  message(g,"The Open Channel is complete. Free flight continues with the berth still warm.");
  speak(g,VOICE_KEI,saga_epilogue_line(g));
 }else message(g,"Chapter complete. The next briefing is ready when you are.");
 return 1;
}
