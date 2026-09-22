# Living galaxy aliveness — Gameplay Designer lock

**Status:** Phase 2 shipping on tip (untagged until next big drop)  
**Owner:** Gameplay Designer  
**Date:** 22 September 2026

## Decision (commander question)

**Neither pure option.** Do not simulate every ship in all 256 systems. Do not leave the sky as only anonymous warp-in/out fog.

| Layer | Choice | Why |
|---|---|---|
| **Bulk traffic** | Proc-gen, session-local warp in/out | PSP pool is 48 NPCs; FEATURE-MAP forbids unloaded-world sim; Elite DNA |
| **Named travellers** | Small persistent registry (12) | Same callsign can reappear after a jump; world feels occupied |
| **Story faces** | Stay story (Kei/Ryn/…) | Manuscript authority; not traffic sims |
| **Station people** | Stay MacVenture room cast | Soft identity via gifts; not undocked space ships |

## Player-visible rule

When you leave a system and come back (or meet a named pilot on their route), **the same person can still be out there** — not a new random serial. Anonymous merchants and raiders still churn.

## Architecture (bible §28, PSP-sized)

1. **Abstract registry** — `TravellerLive[12]`: current system, destination, met flag. Lives on `Game`; **save V12** packs `sys|dest<<8|flags<<16` after gift flags. Older saves keep seeded routes.
2. **Active pool** — existing `npc[48]`. On `game_spawn`, promote travellers whose `sys == current` into free civilian slots with stable names.
3. **Bulk traffic** — unchanged `traffic_budget` / freighter cycles / AI respawn.
4. **Advance on hyperspace** — before rebuild, hop travellers toward destinations (cheap; no continuous off-screen steering).
5. **GalNet colour** — Traffic Control + Spacebook Spotters name a traveller on a remote route so the wire feels occupied beyond the local sky.

## Explicit non-goals

- No galaxy-wide continuous flight physics for hundreds of ships
- No shared ID between crawl “VENN” and a space trader (yet)
- No micro Release tag for this alone

## Phase 2 (this pass)

- Save V12: persist traveller sys/dest/met
- GalNet / Spacebook one-liners that name travellers elsewhere

## Optional later

- One freighter callsign that mirrors a traveller capital route
- Fold #23 → #18 tip when convenient