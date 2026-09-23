# PSP UI & Art manager split

Agreed 23 September 2026 against main `1e3fab88090e328916dc487f1c99031daf0a9061`.

## Shared contract

- Author every final screen at native 480×272 first; larger views are derivatives only.
- Room canvas `(6,20) 340×168`; options `(354,20) 120×168`; feedback begins at `y=190`.
- Keep the lower-right SHIP return rectangle `(274,158) 68×26` clear for runtime UI.
- Use the repository’s actual 8×8 font and canonical palette roles from `src/station-art-kit.h`; `station-art-style.h` only aliases those roles and defines layout/family metadata.
- Use nearest-neighbour sampling, deliberate pixel clusters and readable silhouettes. Validate normal and high contrast at 1:1.
- Art owns no Game/RNG/save/economy writes and must not imply unavailable gameplay.
- Every handoff includes native PNG evidence, state/readability notes, static bytes and measured frame cost when wired.

## Ownership

| Manager | Category | Exact responsibility | Current status |
|---|---|---|---|
| PSP UI & Art 1 | Second Shift / Canteen / Bar | authored room, Lysa/Pell/Dax visual language, bar scene helper and shared station style/layout contract | native bar asset delivered separately; interaction/runtime integration pending |
| PSP UI & Art 2 | Arrivals, station exteriors, planet/body windows | arrivals room art, exterior silhouette families, authoritative body-window composition and scene dispatch | Arrivals native-first candidate in this branch; runtime dispatch pending |
| PSP UI & Art 3 | presentation chrome | narrow bar footer/help framing and exceptional-action readability | footer candidate `f97626fa9d313509ee54d4a5eb90a10c6ce9f4fc`; no room asset ownership |

Market, Shipyard/Outfitting, Mission Board and GalNet remain Team-allocated categories, not silently assigned to Art 3. They use the shared contract and should be assigned explicitly before production edits.

## Integration order

1. Lead/Team confirm current base and exact reserved hunks.
2. Merge shared layout/style contract once, preserving `station-art-kit.h` as palette authority.
3. Wire one native-first Arrivals scene and one Second Shift bar scene; capture normal/high contrast and verify options, speech, hotspots and SHIP at 1:1.
4. Only after combined acceptance, assign Market, Shipyard, Mission Board and GalNet asset work to named owners.

## Conflict rules

Do not duplicate generation or redefine palette tokens. Do not cherry-pick the historical 6048a23 arrivals board wholesale. Do not treat coverage registers or family tables as completed runtime art. Renderer helpers are reserved by category; Station owns room entry/routing and Gameplay/Systems own truth, transactions and persistence.
