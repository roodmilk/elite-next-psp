# First Light — playable tutorial and script

Version 2.5.151. Choose **Start tutorial** on the title menu. **Continue tutorial** resumes its checkpoint; **New game** starts Open Channel directly; **Load commander** loads the regular save.

The tutorial begins with only FLY / Launch visible. Its 61 ordered lessons introduce every command-deck service, all six GalacticNet tabs, the seven station rooms, basic piloting and a surface survey. Successful practical actions advance the lesson. Review lessons advance when the taught screen/tab is closed; ship exchange, repairs, paint, passenger booking and contracts are explained without mandatory spending or acceptance. Combat missiles are explained rather than fired at an NPC; the pulse-laser exercise uses real firing.

## Safety, persistence and recovery

- `tutorial.sav` is separate from `commander.sav`; dockside lesson completions checkpoint automatically with the existing CRC/backup writer.
- Save V15 adds two little-endian fields: lesson and acknowledged briefing. V1–V14 saves keep tutorial disabled.
- Briefings pause simulation and consume their confirmation input. Flight has a narrow objective strip; Triangle on the paused deck reopens the current lesson.
- The briefing offers confirmed checkpoint recovery and confirmed exit from training. Start after death reloads the last dockside tutorial checkpoint. Flight progress since that dock can be replayed.
- First completed return releases a one-time 500 U allowance. The player buys the Pulse Laser themselves; practice canister and calibration echo are placed nearby only for their lessons.
- Completion grants the existing harbour licence and hands into the Open Channel story without repeating its harbour flight. Ending early opens all menus but does not grant that licence.
- Debug is a read-only introduction by instruction, not a forced action. Native debug tools continue to behave as development controls.

## Acceptance coverage

The PSP smoke suite drives all 61 completion conditions through the actual input handlers. It uses controlled positions/state for docking and planet fixtures; this is not a human real-time playthrough. It checks initial visibility, blocked shortcuts, briefing input consumption, ordered events, every lesson save round-trip, V14 migration, invalid checkpoint rejection, and native 480×272 text fit. Native captures cover title, briefing, FLY-only menu, Spacebook evidence and planetary guidance.

## Full screenplay and objectives

### 01. THE EMPTY BERTH

**KEI:** Ryn left her kettle on. Search Control calls that irrelevant. I call it a reason to keep looking. Before I give you her route, I need to know you can come home. Venn has lent us an Adder and cleared a practice lane. One switch first. The rest can wait.

**Do:** FLY > Launch. Press X.

Completion: `LAUNCH`. Introduces service ID 0.

### 02. A SMALL CORRECTION

**VENN:** Tower here. Kei tells everyone I am strict. I prefer 'predictably disappointed'. Nudge the nose left or right with the D-pad or nub. Watch the stars move. Small corrections; you are steering a ship, not stirring soup.

**Do:** D-pad LEFT/RIGHT or nub: turn for 1 second.

Completion: `YAW`.

### 03. ABOVE AND BELOW

**VENN:** Good. Now raise or lower the nose. Keep the hub out of your path while you practise. Space has no floor, but stations are remarkably committed to having walls.

**Do:** D-pad UP/DOWN or nub: pitch for 1 second.

Completion: `PITCH`.

### 04. A LITTLE MOMENTUM

**KEI:** Ryn used to say engines sound different when you stop being afraid of them. I think she meant practise. Hold R and watch your speed rise. Release it when you are comfortable; the ship keeps moving.

**Do:** Hold R until speed reaches 150.

Completion: `THROTTLE`.

### 05. THE MOST USEFUL BUTTON

**VENN:** And now the skill that saves more pilots than marksmanship: slowing down. Hold L until the speed display drops below forty. There is no prize for reaching the berth before your brakes.

**Do:** Hold L. Slow below 40.

Completion: `BRAKE`.

### 06. THE LITTLE BLUE WORLD

**KEI:** Roll changes the view, not which way your steering works. Hold L with Left or Right briefly. Ryn rolled every time she saw an ocean. She called it waving. Please do not wave inside the station.

**Do:** Hold L + LEFT/RIGHT to roll for 1 second.

Completion: `ROLL`.

### 07. BORROWED SPEED

**VENN:** Double-tap R, holding the second tap, for boost. Then let go. Heat climbs quickly; boosting hot drains shields and damage means an engineer's bill. This is a short test, not an invitation to discover your insurance limits.

**Do:** Double-tap R; hold the second tap briefly.

Completion: `BOOST`.

### 08. THREE WAYS HOME

**KEI:** Hold Start to pause and move power between SYS, ENG and WEP. Left/Right chooses a bank; Up/Down moves a pip. SYS supports shields, ENG supports engines, WEP supports weapons. Spend power on what you need now.

**Do:** Hold START; select a bank and move a pip.

Completion: `PIP`.

### 09. FIND THE LIGHTHOUSE

**VENN:** Hold Square for the targeting computer. Left/Right changes category; Up/Down chooses a contact. Find the station. While still holding Square, press R to lock it. Auto-turn points the nose for you; manual steering takes control back.

**Do:** Hold SQUARE, choose STATION, then press R.

Completion: `LOCK`. Introduces service ID 8.

### 10. A CHANNEL WITH A PURPOSE

**KEI:** Select opens your paused command deck. Comms is now available under FLY. It can request docking, open radio settings and hail a target. Routine chatter is information, not homework. Open Comms, read the options, then Circle back.

**Do:** SELECT > FLY > Comms panel. Read; O back.

Completion: `VIEW` (10). Introduces service ID 10.

### 11. COME HOME BORING

**VENN:** Keep the hub selected and approach slowly. Circle near the hub requests guided docking; Comms also has a docking request. If you are too far away, close the distance first. Let the arrival sequence finish. I will complain about your parking only after you survive it.

**Do:** Face the hub, approach, press O. Finish docking.

Completion: `DOCK`.

### 12. THE BORROWED SHIP

**KEI:** That was a landing, not a promise. Good. The Guild has released 500 units for training expenses. Before we spend them, open Save / status. Records tell you what you own and what the local law thinks of you. Tutorial saves use their own file.

**Do:** COMMANDER > Save / status. Read; O back.

Completion: `VIEW` (5). Introduces service ID 5.

### 13. WRITE IT DOWN

**VENN:** A memory stick is more reliable than a pilot saying 'I will remember'. Open Save / status and press X to save while docked. We also checkpoint completed dockside lessons. Your normal commander has a separate file.

**Do:** Save / status: X SAVE while docked.

Completion: `SAVE`.

### 14. TWO TONNES OF BREAKFAST

**KEI:** I put two tonnes of food in the hold. No, it is not all for you. Sell one unit of Food at Cargo & market with Left. Prices are per unit; restricted goods bring law attention. A contract's reserved cargo cannot be sold out from under it.

**Do:** SHIP > Cargo & market. FOOD: LEFT to sell 1.

Completion: `SELL`. Introduces service ID 1.

### 15. A RECEIPT, NOT A FAVOUR

**KEI:** Now buy one unit of Food back with Right. Check the balance and hold space. Trading is real even during training. A passenger also needs hold room; fuel is kept separately. Ryn always checked her receipts. We will need that habit.

**Do:** Cargo & market. FOOD: RIGHT to buy 1.

Completion: `BUY`.

### 16. SIX PLACES FOR TROUBLE

**VENN:** Open Ship loadout. WPN, DEF, NAV, HOLD, FUEL and UTIL each take one fitted module. Replacing a module gives up its old effect. Removing a hold expansion is refused if the cargo will not fit. Inspect a slot, then come back.

**Do:** SHIP > Ship loadout. Inspect slots; O back.

Completion: `VIEW` (21). Introduces service ID 21.

### 17. TOOLS BEFORE HEROICS

**KEI:** Open Outfitting and fit a Pulse Laser. The training allowance covers it. Select the actual item and press X; the receipt shows the cost. Equipment stock varies between stations. We are fitting a tool for controlled practice, not buying permission to shoot strangers.

**Do:** SHIP > Outfitting. Select PULSE LASER; X.

Completion: `FIT` (1). Introduces service ID 4.

### 18. THE ENGINEER'S INVOICE

**VENN:** Engineers repair persistent ship damage for a fee. Shields recovering does not mend a damaged hull. Smoke, shaking and poor performance are reasons to visit. Read your condition and quote. We will not break the ship just to demonstrate the bill.

**Do:** SHIP > Engineers. Inspect condition; O back.

Completion: `VIEW` (24). Introduces service ID 24.

### 19. SOMETHING THAT IS YOURS

**KEI:** Ryn painted a small blue stripe on every ship she flew. Said the galaxy was too large to leave everything anonymous. Ship decorator previews finishes before you pay. Browse them. Buying paint is optional; your next meal is not.

**Do:** SHIP > Ship decorator. Browse; O back.

Completion: `VIEW` (23). Introduces service ID 23.

### 20. THE PRICE OF A BIGGER HOLD

**VENN:** Shipyard exchanges your current hull. Compare hold, speed, range and the exchange price. Your cargo must fit the replacement. Modules transfer, and excess fuel is capped to the new tank. Browse the ships; you do not need to buy one for this lesson.

**Do:** SHIP > Shipyard. Compare hulls; O back.

Completion: `VIEW` (3). Introduces service ID 3.

### 21. THE ROOM WE LIVE IN

**KEI:** The receipt from Ryn's last resupply says Lave. That gives us a place, not a culprit. System details lists the hub, worlds, economy and local risk. Open it and read the room before we start accusing the furniture.

**Do:** FLY > System details. Read; O back.

Completion: `VIEW` (11). Introduces service ID 11.

### 22. COLOURS ARE NOT MOTIVES

**KEI:** Factions tells you who shares the lanes: traders, law, pirates and the Explorers Guild. A colour is a role, not a biography. Ryn once trusted a pirate's weather report and distrusted a Guild accountant. She was right both times.

**Do:** WORK > Factions. Inspect a faction; O back.

Completion: `VIEW` (7). Introduces service ID 7.

### 23. THE PUBLIC VERSION

**KEI:** GalacticNet is the station's public memory. NEWS is what happened, or what someone wants recorded. Use L/R to change its tabs. Read a news item, then press Circle to finish each network lesson. We will compare six different kinds of evidence.

**Do:** DISCOVER > GalacticNet. NEWS tab; O back.

Completion: `NET`. Introduces service ID 14.

### 24. THE PRICE OF A RUMOUR

**KEI:** MARKET is useful context, not a guaranteed profit. Prices change between systems; cargo space, legality and fuel all belong in the calculation. Ryn's food receipt looks ordinary. That is why someone missed it.

**Do:** GalacticNet: L/R to MARKET. Read; O back.

Completion: `NET` (1).

### 25. FACES ON THE BOARD

**VENN:** WANTED lists people the law is looking for. Read before acting; random violence earns your own notice. A poster is not a firing solution. This lesson is reconnaissance, and every civilian in my lane would like it to stay that way.

**Do:** GalacticNet: L/R to WANTED. Read; O back.

Completion: `NET` (2).

### 26. THE LAST ORDINARY POST

**KEI:** Open SPACEBOOK. People post jokes, warnings and small things that later become important. I have pinned Ryn's last receipt here. It says: 'If I miss supper, ask who kept the light on.' Read the post; X likes it, Triangle opens comments.

**Do:** GalacticNet: SPACEBOOK. Read; try X; O back.

Completion: `NET` (3).

### 27. PEOPLE, NOT MARKERS

**KEI:** INBOX holds local messages. Read the sender and the request before acting. Its social buttons are presentation, not mission acceptance; real contracts belong on the Mission board. Named travellers can also meet you out in space. People leave traces in other people's routines.

**Do:** GalacticNet: L/R to INBOX. Read; O back.

Completion: `NET` (4).

### 28. THE COST OF A NAME

**VENN:** JOBS is the mission feed: active work, outcomes and station offers. It tells you what is happening; Mission board is where you sign up and Mission log is where you track it. Read the feed, then we will meet the people behind these screens.

**Do:** GalacticNet: L/R to JOBS. Read; O back.

Completion: `NET` (5).

### 29. FEET ON THE DECK

**KEI:** Disembark from FLY. The station is more than a vending machine with an airlock. Up/Down chooses an option and X acts; doors, people and objects are listed. Circle or Triangle brings you back aboard. Find Venn in Arrivals and hear him out.

**Do:** FLY > Disembark. VENN: choose first reply.

Completion: `TALK`. Introduces service ID 20.

### 30. THE THINGS OFF THE CATALOGUE

**VENN:** The chandlery stocks things the main outfitting board does not. From Arrivals take the SHOP door, talk to the Chandler and browse the exclusive stock. Opening a conversation does not commit you to a purchase.

**Do:** SHOP > CHANDLER > Browse exclusive stock.

Completion: `SHOP`.

### 31. THE LOADER'S LEDGER

**KEI:** From the shop, go to CARGO. Talk to the Loader and hear the first reply. Small paid favours live here too. His manifest puts Ryn's last crate on a survey run, not a smuggling run. That matters to me more than I expected.

**Do:** CARGO BAY > LOADER > first reply.

Completion: `TALK` (3).

### 32. SOMEONE REMEMBERS

**KEI:** Take the CANTEEN door. Ask the Bartend for a rumour. Passengers also look for lifts here; a taxi booking needs a fitted cabin and spare hold capacity. Today we need testimony. We are buying neither a drink nor a passenger's life story.

**Do:** CANTEEN > BARTEND > first reply.

Completion: `TALK` (2).

### 33. THE DESK WITH MY NAME

**KEI:** Return through Arrivals to the GUILD DESK. I am the one pretending paperwork is useful camouflage. Hear me out. Ryn went to check an old survey beacon. The rumour was that it still broadcast after its power supply was removed.

**Do:** ARRIVALS > GUILD > KEI > first reply.

Completion: `TALK` (4).

### 34. PUT YOUR OWN OXYGEN FIRST

**VENN:** The Guild passage leads to the MED CLINIC. Talk to the Medic. One-time supplies are exactly that: one time. Surface exploration has a return journey, and a clever discovery is no consolation to an empty suit.

**Do:** GUILD > CLINIC > MEDIC > first reply.

Completion: `TALK` (5).

### 35. A CLEAN MANIFEST

**VENN:** The clinic connects to CUSTOMS. Speak to the officer. Restricted cargo can trigger a scan; refusing law carries consequences. Do not accept an illicit job because the pay is printed in a cheerful colour. Return aboard when you are done.

**Do:** CLINIC > CUSTOMS > officer > first reply.

Completion: `TALK` (6).

### 36. WORK THAT KEEPS YOU FLYING

**KEI:** Back aboard, open Mission board. Read a brief: destination, objective, time and payment. Delivery, rescue, exploration, bounty and smuggling ask different things. Accepting is optional here. I will not make you sign a dangerous contract to pass a reading lesson.

**Do:** O aboard > WORK > Mission board. Read; O back.

Completion: `VIEW` (12). Introduces service ID 12.

### 37. ONE PROMISE AT A TIME

**KEI:** Mission log keeps accepted jobs, Guild work and the main story together. X tracks an entry; Select opens its next step. Timers pause while you read menus. Review the log, then back out. Tracking a task is a reminder, not teleportation.

**Do:** WORK > Mission log. Review entries; O back.

Completion: `VIEW` (13). Introduces service ID 13.

### 38. THE LONGER ROAD

**KEI:** Tracked mission carries Open Channel, the larger story of Ryn's disappearance. This training is the beginning of your life here, not the solution to her case. Read the briefing and return. We will leave its choices for after your licence.

**Do:** WORK > Tracked mission. Read; O back.

Completion: `VIEW` (17). Introduces service ID 17.

### 39. THE GUILD'S OTHER PROMISES

**KEI:** Guild assignments live alongside the main story in Mission log. They reward exploration and helping people. Inspect the Guild page. You do not owe the Guild every waking minute, whatever its stationery implies.

**Do:** WORK > Explorers Guild. Inspect; O back.

Completion: `GUILD`. Introduces service ID 18.

### 40. KEEP THE VIEW YOURS

**VENN:** Display & chatter includes HUD layout, quiet text, contrast and third-person view. Set what helps you see. Essential safety prompts remain. This is your cockpit; comfort is equipment too.

**Do:** COMMANDER > Display & chatter. Review; O back.

Completion: `VIEW` (19). Introduces service ID 19.

### 41. THE VOICES BETWEEN STARS

**KEI:** Radio & audio gives you music, chatter and separate volume controls. L/R tunes stations; Left/Right adjusts the selected volume; Triangle turns radio on or off. Pick something you like. Silence is an option too.

**Do:** COMMANDER > Radio & audio. Tune; O back.

Completion: `VIEW` (16). Introduces service ID 16.

### 42. A REFERENCE, NOT AN EXAM

**VENN:** Controls keeps five reference pages, including targeting, travel, station menus and surface movement. Left/Right changes pages. L toggles analog steering if the nub is drifting. You can always come back here.

**Do:** COMMANDER > Controls. Browse pages; O back.

Completion: `VIEW` (6). Introduces service ID 6.

### 43. THE TEST OUTSIDE THE WINDOW

**KEI:** Launch again. Venn has put a harmless practice canister outside the flight lane. We will learn what the ship can do away from other people's paintwork. Your laser stays cold until I say where to point it.

**Do:** FLY > Launch.

Completion: `LAUNCH`.

### 44. A WEAPON IS A RESPONSIBILITY

**VENN:** Point into empty space, away from the hub and traffic, then press X for a short laser burst. L+X launches a missile at a locked hostile; we will not practise that on a neighbour. Empty WPN slots cannot fire. Watch the heat gauge.

**Do:** Face empty space. Tap X to fire the fitted laser.

Completion: `FIRE`.

### 45. THE THING YOU CAN BRING BACK

**KEI:** That blue canister ahead is ours: one sealed ration crate, placed for training. It is already selected. Circle within 500 metres starts the tractor. Let it finish. Ordinary salvage can contain restricted goods; a full hold may sell salvage automatically.

**Do:** Practice canister selected. O; wait for tractor.

Completion: `SALVAGE`.

### 46. A SIGNAL, NOT A GHOST

**KEI:** I have marked a calibration echo ahead. Circle within 1,100 metres scans it. This one is a training beacon, not Ryn. Learn the difference between collecting evidence and wanting an answer so badly that any light looks like one.

**Do:** Calibration echo selected. Press O to scan.

Completion: `SCAN`.

### 47. A RECORD THAT CAN BE CHECKED

**KEI:** Select pauses flight. Discovery Codex keeps visited systems and scanned life, minerals and anomalies. Find the new entry. The training echo is ordinary. That is useful: now you know what ordinary looks like.

**Do:** SELECT > DISCOVER > Discovery Codex. O back.

Completion: `VIEW` (15). Introduces service ID 15.

### 48. HISTORY HAS MARGINS

**KEI:** Galactic Lore is the archive of eras, peoples and powers. Use L/R for categories and Up/Down for entries. It gives the rumours context; it does not prove them. Ryn used to write questions in the margins. I saved those pages.

**Do:** DISCOVER > Galactic Lore. Browse; O back.

Completion: `VIEW` (22). Introduces service ID 22.

### 49. FUEL BEFORE DISTANCE

**VENN:** Return to the hub before the navigation lesson. Use your target browser to select the station, Square+R to align, then approach and Circle for docking. We check the tank on the ground, where mistakes are cheaper.

**Do:** Select station. Approach; O to dock.

Completion: `DOCK`.

### 50. FILL THE TANK

**VENN:** Outfitting's first entry refuels to your ship's range limit. Press X on it. A full tank already meets the check. The price is for missing fuel; fuel never consumes cargo tonnes. Do this before choosing a star.

**Do:** SHIP > Outfitting > REFUEL; X.

Completion: `FUEL`.

### 51. ONE STAR AT A TIME

**KEI:** Galaxy map has a nearby list and a full map. Triangle switches them. On the full map, X plots a route; on the nearby list, X launches the selected jump. Choose a reachable nearby system. We are testing navigation, not chasing Ryn beyond our fuel.

**Do:** FLY > Galaxy map. Nearby reachable system: X.

Completion: `JUMP`. Introduces service ID 2.

### 52. ARRIVAL IS NOT THE END

**VENN:** Hyperspace leaves you outside the local hub. That is deliberate: nobody wants a visitor materialising in their kitchen. Find the station, close the distance and dock. A route can need several jumps, with refuelling between them.

**Do:** Target local station. Approach; O to dock.

Completion: `DOCK`.

### 53. THE SURVEYOR'S HABIT

**KEI:** One final field exercise: a landable world. We will launch, choose a non-gas planet, approach within 1,000 metres of its surface and press Circle. X accepts atmospheric entry. I have authorised the landing kit. The ground deserves patience.

**Do:** Launch. Target non-gas planet; near surface O, X.

Completion: `ATMOSPHERE`.

### 54. A PLACE TO SET DOWN

**VENN:** In atmosphere, Square points you toward the cyan landing pad. Reduce speed and get low over it; Circle lands when the approach is safe. Too high or too fast gives advice, not a pass. You can reread this lesson from the paused deck.

**Do:** SQUARE finds pad. Fly low and slow; O lands.

Completion: `LAND`.

### 55. LEAVE FOOTPRINTS

**KEI:** Circle leaves the landed ship. The D-pad walks and strafes; the nub looks. Hold L with the D-pad to look without moving. R is your jet lift; release it to descend. Keep the ship in sight. It is your way home.

**Do:** Landed: O to leave ship. Walk a few steps.

Completion: `EVA`.

### 56. FIRST LIGHT

**KEI:** Square surveys nearby unrecorded life or minerals. Move closer if nothing is in range. This is a real discovery, written to the same Codex as every other survey. Whatever happens to Ryn's case, today somebody looked carefully at this world. You did.

**Do:** On foot: approach life or minerals; SQUARE scans.

Completion: `SURVEY`.

### 57. THE RETURN HALF

**VENN:** Triangle faces the ship. Walk back close to it, settle onto the ground, then Circle boards. Jet lift helps with rough terrain, but you must come down to board. A survey is not finished until its pilot is aboard.

**Do:** TRIANGLE faces ship. Walk close; O boards.

Completion: `BOARD`.

### 58. LOOK BACK ONCE

**KEI:** Triangle takes off from the pad. Once airborne, Triangle returns to orbit; dismiss any active speech first. I used to think Ryn's courage was leaving. Now I think it was always expecting to return.

**Do:** TRIANGLE takes off; TRIANGLE again to orbit.

Completion: `ORBIT`.

### 59. A BERTH WITH YOUR NAME

**VENN:** One last dock. There is no timer and no bonus for speed. Use the tools you have learned. When you arrive, we will save your record and stop hiding the rest of the deck. You have earned the right to choose your own next mistake.

**Do:** Target station. Approach; O to dock.

Completion: `DOCK`.

### 60. THE MAINTENANCE HATCH

**VENN:** One final menu: Debug tools. These are development controls that change cash and world state. They are not paid station services or part of earning your licence. Inspect the list without activating anything, then return. In ordinary play they are entirely optional.

**Do:** COMMANDER > Debug tools. Read only; O back.

Completion: `VIEW` (9). Introduces service ID 9.

### 61. THE LIGHT STAYS ON

**KEI:** Venn has signed your licence. The loader kept the receipt; the bartender kept a chair; I kept the channel. That is what Ryn meant about the light. We still do not know where she is. But now you can follow the evidence and come home with it. Open Channel awaits in Tracked mission.

**Do:** X completes training. Every service opens.

Completion: `FINISH`.
