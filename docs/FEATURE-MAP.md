# ELITE: NEXT feature map

This maps modern space-game fantasies to a PSP-sized implementation so the
design bible stays ambitious without becoming an unbounded promise.

| Player fantasy | ELITE: NEXT implementation | PSP rule |
|---|---|---|
| Explore an enormous galaxy | Deterministic 256-system chart with seeded bodies, economies, traffic, danger, fauna and anomalies | Stable seeds; rendering never changes gameplay RNG |
| Feel a living space lane | Faction AI, convoys, freighters, distant battles, police responses and local warrants; **12 named travellers** hop systems with stable callsigns (`docs/LIVING-GALAXY-ALIVENESS.md`) | Bounded NPC/debris pools; no unloaded-world simulation of all ships |
| Notice a living system | Rate-limited scanner incidents plus a live System Details traffic briefing for trader, law, pirate and explorer activity | Events reuse the existing message/alert band; no permanent HUD clutter |
| Build a personal ship | Shipyard hull exchange, outfitting, power pips, cargo, missiles, laser and docking upgrades | One active hull; upgrades persist in commander V8 |
| Choose a life | Trade, bounty, exploration, rescue, covert work, story and Guild jobs | Five-slot mission log with deterministic risk/reward |
| Discover strange worlds | Planet art families, varied system layouts, surface prototype, flora/fauna/mineral scans, whales and anomalies | Stable authored IDs before new procedural types |
| Own a place in the galaxy | Future station restoration projects and a mobile freighter home | Expand through missions; no placeholder menus |
| Meet memorable people | Kei, Venn and authored actors with faction-coded portraits and dialogue | Identity records survive NPC slot reuse |
| Feel cinematic | Warp tunnel, lens flare, speed lines, engine plume, target pulse, CRT dust and station rings | Effects stay behind warnings/telemetry and pass performance smoke |
| Always know what to do | Story `!` marker, `STORY NEXT` breadcrumb, selected mission brief, risk/payout column and plain-language prompts | Pair symbols with words; never rely on color alone |
| Plan a long journey | Full 256-system galaxy overview, cached multi-jump path, saved `route_goal` separate from the next hop (save V10) | Generic for every destination; no special-cased systems |
| Feel the galaxy is occupied | Twelve named travellers hop systems (save V12); bulk traffic stays local proc-gen; GalNet names remote routes | Not a full unloaded-world ship sim |
| Relax with a radio | Five procedural stations, layered ambience and Far Horizons alien chatter; CC0 source manifest; optional user MP3 folders | Integer mixer; hardware decoder with large stream buffer |

Modern inspirations contribute loops, not wholesale feature counts. No Man’s
Sky’s exploration, bases, settlements and evolving missions; Starfield’s crew,
ship fitting, outposts and faction choices; and Elite Dangerous’ galaxy,
trade, reputation and news are represented as bounded offline-first systems.
Multiplayer, infinite terrain and a full AAA asset catalogue remain outside
the PSP build contract.

## Glance-first interface rule

Full HUD is for learning and planning. Minimal HUD is for long travel: system,
danger, speed, shield, target and critical warnings only. Scenic HUD removes
routine labels while retaining safety prompts. Action strips use a tiny PSP
glyph and a plain-English word, and focused items receive shape/fill as well
as color. High Contrast Focus strengthens the selected fill without changing
faction colors or relying on color alone.
