# Homeward opening tutorial — 2.5.264

New Game opens name and portrait creation. START NEW GAME begins the safe combat dream, then the authored first day on Earth in 2050. Existing commanders retain their existing campaigns; the old First Flight practice remains optional.

## Playable sequence

The 65-stage opening runs through waking with Dad, Earth city and a three-ship dealer choice, first manual save, Sol Rock radio, city ascent, Sol System Details, flight controls, Earth GalacticNet, countryside landing and EVA, Dad's pep talk, glowing object and show/hide choice, boarding and ascent, MiniBolt gift/loadout, Mission Log and Tracked Mission, actual mining and mineral collection, minor damage, Mum's incoming call and Talk before docking, station repairs/refuelling, aspirational shipyard and outfitting with purchase refusal, real Food/pizza purchase, homeward departure, Thargoid transmission and burst, waking without Dad, failed Earth communications, lonely flight, Galaxy Map reveal, Lave signal/traffic/rock music, commercial transmission and real PIMP-MY-SHIP.NET, Lave docking and the Sol attack TV broadcast, then open play.

## Files and integration

- opening-state.h defines saved stages, choices and unlock flags. Stage0 means a legacy commander; stage1000 means the new opening is finished.
- opening-runtime.h adapts the existing input/page/flight/landing/mining/trading/repair systems. Main's ordinary New Game path calls opening_new_game; character completion advances to OP_DREAM. The old tutorial is separate.
- opening-art.h draws bounded native pixel scenes and stable portraits. No downloaded imagery is required.
- opening-content.h switches GalacticNet between familiar Earth content and the local galaxy, retaining cached family/friend threads. Ordinary jobs remain in Mission Log and Tracked Mission.
- opening-save.h appends the V27 state and active tutorial position/condition/parked-ship/orbit snapshot. Existing V26 and older saves migrate with stage0. Atomic writes, CRC and backup recovery are retained.
- opening-input-tests.h is a native, input-driven emulator driver. opening-save-tests.h and opening-world-tests.h are core fixtures.

## Tutorial rules

Required menu choices flash; unrelated menu input is suppressed during lessons. Galaxy Map is hidden until the lonely phase. Factions unlock after a completed ordinary mission, Galactic Lore after a discovery. The decorator app appears and remains available after its advert. The opaque bottom80px dialogue panel masks system/warrant/unit text while preserving the upper menus and speed display.

Dad's MiniBolt is equipment69:54 rock damage and2 ship damage. The established Mining Laser20 is unchanged. Sol Rock is the ninth station, backed by original generated guitar/bass/drums and optional music/Sol Rock files. Radio settings migrate older OFF positions correctly.

Sol is an isolated tutorial world phase. Its chart names all eight planets; this build's existing five-body flight engine represents the Sun, Earth, Mars, Jupiter and Neptune at compressed distances. The mandatory landing is Earth countryside away from home. Displacement restores the original generated Lave system and clears tutorial Earth progress from that slot. Sol has no route back in the main galaxy.

Show/hide is saved and changes Dad's reaction during the incident. The object and pizza survive displacement. Undelivered Inbox replies are saved. The opening concludes with the missing-Dad motive; subsequent rescue/homecoming plot and the object's ultimate explanation are deliberately still open in the bible.

## Saving and resuming

The first save uses a real commander slot and its normal overwrite confirmation. Later safe milestones autosave to that chosen slot. Flight/EVA saves are allowed only during the opening, with docking, atmospheric/ascent animations, hyperdrive and active local sessions excluded. Completed and legacy commanders retain the normal docked save rule. Load from the title screen resumes the stage and appropriate menu/flight view.

## Validation and limits

Validation results and the exact binary hash are recorded in BUILD-2.5.264.txt. The initial263 baseline core, radio and steering reports pass, but its large input/outfitting suite already contains failures and does not finish within the standard120-second smoke window. Do not describe that inherited suite or physical PSP hardware as certified.

The automated opening driver covers all three starter ships, both show/hide outcomes, menu locks, real transactions, checkpoint reloads, disappearance, app, TV and free play. Native480x272 screenshots are reviewed for dialogue coverage and menu readability. It uses isolated temporary save slots; it never overwrites the player's saves.

Run opening-check.flag beside an isolated EBOOT to write opening-check.txt and native BMP screenshots. Remove the flag for ordinary play. The driver is diagnostic, not a tutorial skip accessible in normal menus.

## Remaining work

Test the exact build on physical PSP hardware, especially memory, frame rate, suspend/resume and sound. Develop the post-Lave main story once the owner decides the rescue and return-home plot. More Sol planets in actual flight would require expanding or streaming the existing five-body engine. Multiplayer and online authority changes are outside this tutorial implementation.
