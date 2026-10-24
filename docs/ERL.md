Local 2.5.405: ERL combines 120ms Broadcast Neon moving focus rims, interruptible 220ms Slipstream page transitions, aligned Paddock Signal portrait reveals, once-only XP fills, sparse star shimmer, Season route lights and Garage platform lights. Inputs commit immediately; no action queue, save-format or race-rule change. Authored victory reveal is retained. Physical PSP testing pending.

Local 2.5.404: Events and Rivals match the approved native screenshot layouts, with a large qualifier XP readout/progress bar, diagonal hero panels and a 3x2 pilot grid. Existing controls, other ERL pages, race rules and V35 saves are retained. Physical PSP testing pending.

# Elite Racing League — 2.5.400

The playable league uses the approved Broadcast layout at 480×272, the game’s
8×8 font, actual ship meshes and solid course scenery. HQ previews are cached
renders of the same course geometry. It builds on local 2.5.396, retaining
Multiplayer, Crime Watch, newspapers, Rumours, missions and planetary work.

## Starting a career

Visit Lave Hub’s Mission Board and select **NICE FLYING**, the first card
with a moving rainbow sheen. Mechanic Torr Bell
offers a short, free one-on-one test. Losses can be retried. A win pays 500U,
earns a referral to a reachable real Mega Capitol and adds a durable Inbox
message. The result returns to Torr’s congratulations, with a route button
for the nearest reachable Mega Capitol, rather than opening a league desk
in Lave. Torr remains available on the board as **MEET TORR BELL**.

At a Mega Capitol, open **WORK > ERL LEAGUE**. On the first visit, read the
organiser’s welcome and explanation before opening the rookie league card.
The introduction is remembered by existing V35 saves. The organiser’s rookie qualifier
unlocks a galaxy-wide licence. Each HQ has a deterministic organiser and local
course/competitor identity. The Mission Log tracks the introduction, referral,
qualifier and race objective. Navigation in the objective system stays local.

## Races and controls

Bring your own ship. ERL regulates top speed and steering; hull and equipment
cannot bypass the race boost reserve. D-pad or analog steers, R accelerates,
L brakes, and double-tap R then hold boosts. Release R after depletion to
recharge. Start pauses; resuming grants three seconds of protected guidance.
Targeting, weapons and normal communication controls are unavailable in races.

Full races have 32 ordered gates and deliberately winding fixed layouts. The
circuit and capital final use two laps. Green rings add 24% boost once per pass
and power a free 0.65-second burst with peripheral speed streaks;
takedowns add 40%. A full reserve supplies four seconds of boost; recharge is
10% per second after a short delay. Holding boost is not an automatic win.

Timed Takedown Rush ranks actual takedowns over two minutes. Takedown Trial
requires five takedowns within two minutes. Ties use fewer own knockouts, then
earlier final scoring time. Glancing contacts nudge; strong impacts can cause
takedowns. Actual rocks, wreck ribs and station structures have collision
surfaces. Rival repair drones return racers after approximately three seconds.
Player knockouts use normal-speed repair and preserve the last valid gate.

A rival takedown uses one featured slow-motion camera at a time, followed by
protected guidance and a 3–2–1 control handover. All race simulation shares the
same slow-motion clock. Thirty consecutive voluntary seconds outside the route
records a DNF; returning resets the countdown. Repair, pause and forced cameras
do not add off-course time. Racing restores the real pre-race ship condition,
fuel and equipment; it does not provide free permanent repairs or refuelling.

Races override playback with the actual MP3 in **music/Sol Rock**, including
when normal music is off or Quiet Mode is on. The override leaves the saved
station and settings intact. Missing music uses the existing synthesised audio
fallback and does not block racing. Normal ambient chatter is replaced by the
compact racing feed.

## Seasons, rewards and Garage

Four local rounds: circuit, sprint, Takedown Rush and capital circuit final.
Points are 10/7/5/3/2/1/0. Official losses and withdrawals stick; there is no
official Retry action. Practice pays neither units nor XP. Beginner entry is
free. The ship identity stays locked throughout a championship, including
ordinary shipyard exchanges. Withdraw the season explicitly to unlock it.

Rookie/Club/Pro/Elite XP thresholds are 0/900/2400/4800. XP unlocks the next
qualifier; winning it earns promotion. Winning earns 180 + 50×division XP;
other finishers earn a smaller amount. Withdrawals earn none. Qualifier wins
pay 800U; official and Trial wins pay 1,200U × division level. Championship
podiums pay an additional 6,000/4,000/2,000U × division level.

First championship victories in the four divisions permanently award Krait,
Ophidian, Solar Clipper and Mirage respectively. Later wins in an already
claimed division award an additional 3,000U × division level instead. No prize
is silently discarded because a fleet array is full: the five identities cover
the original ship and four unique division prizes. One exclusive module is
claimable per first division victory; full Item Storage defers the claim.

**ERL GARAGE** is accessible at every HQ. It preserves each ship’s identity,
paint, installed modules, fuel, missiles and condition. Swapping parks the
outgoing ship. Cargo and loose Item Storage transfer only if both fit; otherwise
the action explains the capacity problem and changes nothing. Fitted modules
stay with their ship and are never copied into loose storage.

Exclusive Vector Governor (NAV) improves normal-flight rotation by 12%;
Coolant Matrix (UTIL) reduces normal engine/boost heat by 15%. They have usable
item definitions and category icons, and are excluded from ordinary shops.

First place plays the revised checkerboard flight once, reveals a gold winged
trophy, then shows fireworks and the actual rewards. X finishes the reveal;
a fresh press activates the result action. Completed simulation stops behind
the card. Official results and rewards are recorded before the animation.

## Saves and verification boundaries

V34 appends an explicit, validated 4,161-byte ERL record and two module-storage
entries. The existing versioned import and atomic replacement/backup mechanisms
are retained. New saves need 2.5.397 or later; keep older backups when downgrading.
Ordinary exhibition contracts still reconstruct their grid after loading.

ERL checkpoints every five seconds of active racing using one immutable
snapshot on a lower-priority worker. Flight never waits for checkpoint file
writes; transitions, pauses, results, sleep and shutdown join it before later
save/load actions. Durable career actions remain synchronously committed. Sleep and restored interrupted races return paused; resume grants
protected guidance. An unexpected power loss can lose the checkpoint interval plus any pending
write time. Pending results include transaction identifiers to prevent repeated
payment. Swaps save the outgoing and incoming records together.

Run `tools/review-erl.ps1` against a review build for domain, input, save,
full-course timing, actual MP3 and native render checks. Review code is excluded
from retail. `tools/review-psp-boot.ps1` validates the exact retail EBOOT in an
isolated PPSSPP directory, including ERL HQ/race scenes. Do not copy review
flags into a player installation. Emulator timing is not physical PSP FPS.
Release evidence and remaining inherited broad-suite failures are recorded in
the versioned release report. Physical PSP-2000/ARK testing remains necessary.

The introductory mission is always called Introduction to the ERL. It stays active through referral and the first HQ welcome, then completes when the rookie licence is earned. Championships remain accessible at headquarters.

## Hyper Circuit presentation (2.5.402)

Headquarters, event selection, briefs, Season, Driver, Rivals, Garage, standings, conversations, pause and confirmations share the selected native neon style. The Season route shows all four actual rounds. Garage uses each stored hull and paint, preserving swap/lock checks and comparison values. HQ shows earned XP, a segmented bar and remaining XP; full XP unlocks the next qualifier. Elite shows TOP LEAGUE. Before the rookie licence, the display explains qualification instead. Existing controls and V35 saves are unchanged.

The approved checkerboard flight and trophy reveal still play once; the resting result card uses the new style and waits for input. Race art is rendered into one bounded 464x132 cache; menu animation never rebuilds the unchanged world. Orbit geometry is computed once.

Championship uses the approved zigzag route with 47px circular cyan rims and 2x font8 numbers, a pink active round, course names below and the single cup-prize line. HQ places ROUND above its 4x round number at the top right; no round subtitle duplicates it below the course name.

## Visual race cards (2.5.403)

Approved option 1 is implemented at its native coordinates: HQ preview (8,32,464,105), ROUND panel, title banner, commander/ship card, separate league XP card, visual cup-prize strip and five bottom navigation buttons. Briefs use three icon metrics plus a separate reward card; the sprint/circuit/takedown variants show their actual lap or scoring rules. Results separate units, XP and the actual third reward (ship, championship points/place, licence or referral). Large and long values fit or scroll inside the cards. All underlying input actions and V35 data remain intact. ERL control legends are removed; actionable buttons remain. Pending module claims still expose their existing Square action.

The original checkerboard fly-through and one-time trophy reveal are retained before the reward-card composition. All nine icons are tiny authored palette-region sprites, using ship-theme roles rather than fixed overlays. Background orbit coordinates and palette match the approved concepts. Other GalacticNet and conversation pages are unchanged.

Brief event labels (Rookie/Club/Pro/Elite qualifier, practice, championship and test) appear at (20,132) directly beneath the large course name in the left banner, never on the right of the picture.

Metrics use their own symbols: loop arrow for laps, clock for minutes, and concentric target/crosshair for targets.
