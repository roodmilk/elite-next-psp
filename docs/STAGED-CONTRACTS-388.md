# Staged contracts and music repair — 2.5.388

Forty additional contracts, containing 155 authored steps, join the existing
26 contract families and local races. The station board offers five distinct
families plus its local race. Availability uses actual reachable systems,
planet types, field sites, whale pods and equipment requirements.

See [the complete catalogue](MISSION-STAGES-388.md) for every sequence.

## Playing a contract

Accept at the mission board and confirm. The contact sends an animated,
paused transmission explaining the first task. X acknowledges; left/right
and X make an explicit decision when the contract branches. Later steps
send the same kind of update, with the same issuer and portrait. Essential
danger, docking and race sequences take priority over incoming updates.

Mission Log selects the tracked contract. Its next-step screen navigates
to the actual objective. Square on that screen reopens the contact record;
left/right review earlier stages and Circle returns. Flight shows a compact
stage/progress/distance strip and an objective bracket or directional arrow.
Only the tracked contract constructs a physical encounter; all five accepted
jobs preserve their independent stages, cargo, choices and unread updates.

Objectives require actual kills, marked recoveries, mined material, market
purchases, escort arrival, ordered waypoints, sustained observation, surface
scans, restored sites, discovery interactions, anomaly analysis or race wins.
Hailing an unrelated ship, buying minerals for a mining order, or simply
docking cannot substitute for those actions. Required cargo is reserved.
A full hold leaves marked containers outside rather than selling them.

Escorts and guards protect real ships with persistent health. Rescue jobs
can pay proportionally for survivors brought home early. Competitive salvage
has a moving rival; procurement advances reconcile against the final prize
and return unspent funds on abandonment. Choices can reroute cargo, follow
an investigation, change a passenger trip or withdraw from a championship.
The championship requires first place in three existing physical races at
different hubs; only the final result pays its commission.

V33 saves append fixed-width staged progress after existing V32 data and
retain the existing checksum. V1–V32 imports remain supported. Keep backup
saves; a save written by 2.5.388 requires 2.5.388 or newer to load.

## Music

The title always uses Elite Exploration, even when the saved flight radio
station differs or ordinary music is off. It skips 23 seconds, matching
2.5.384, then fades in over one second. Flight preferences remain intact.

Rock FM and Sol Rock are the same channel. Both normal tuning and races
read `music/Sol Rock` beside EBOOT.PBP. Races prefer `Secret Agent Rock.mp3`
when present and otherwise use another supported track in that folder.
No filename rename or replacement of the player's music folder is needed.
Race playback does not apply the title's 23-second skip and restores the
previous station afterward. Quiet Mode still silences race music.

The MP3 loader skips embedded ID3 artwork before decoding and declares the
compiled SDK version required by PSP MP3 decoding for 48 kHz input. The
player's actual 48 kHz Secret Agent Rock file was used for emulator testing.
Audio and JPEG module loading are independent, and relative music paths
resolve from the EBOOT directory. No player music is distributed.

## Verification and remaining work

The release evidence records exact EBOOT and production-source hashes,
actual stage-handler tests, all-stage save/reload, choice and negative cases,
native 480×272 captures, music decoding, opening and core regressions.
Emulator success does not confirm physical PSP playback or performance.
Next: physical PSP cold boot, both music paths and mission playtesting;
then tune difficulty and address separately recorded inherited failures.
Planetary expansion and unrelated UI/portrait work remain separate.
