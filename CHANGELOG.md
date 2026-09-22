# Changelog

All notable playable releases are recorded here. Tagged GitHub Releases are the downloadable builds people should play.

For the long version-by-version development log (features, controls notes, validation), see [`docs/DEVELOPMENT-HISTORY.md`](docs/DEVELOPMENT-HISTORY.md).

## Latest

Download the newest package from **[Releases](https://github.com/roodmilk/elite-next-psp/releases/latest)**.

Release tags after `v2.5.2` are produced by the automated publish workflow and include:

- Station crawl / first-person deck walking improvements
- Flight HUD, GalNet, wanted and combat polish
- Campaign dialogue and mission cue fixes
- Space FX, engine trails and visual passes

Each Release page lists the exact `EBOOT.PBP` and `ELITE-NEXT-PSP-v*.zip` for that tag.

## 2.5.2 - 2026-09-22

- Added user-supplied radio station folders and shuffled MP3 playback on PSP.
- Improved MP3 playback stability and PSP suspend/resume recovery.
- Kept generated radio audio as a fallback when a station has no compatible tracks.
- Added mission availability checks so objectives point to stations that can actually offer the required contract.
- Expanded galaxy navigation planning so distant destinations can be located and routed through intermediate systems.
- Preserved the current UI, mission, faction, targeting, dialogue, art, and gameplay work documented in the development history.

## 2.5.1 and earlier

Detailed history for 2.5.1 back through the earliest native PSP builds is retained in [`docs/DEVELOPMENT-HISTORY.md`](docs/DEVELOPMENT-HISTORY.md), `docs/`, and the Git commit history.
