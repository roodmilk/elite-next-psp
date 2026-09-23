# Reusable Element Kit — screen coverage map

Native-first coverage for station-facing screens. This is an art dependency map,
not a claim that every screen is implemented.

| Screen / room | Reusable kit pieces | Required authored shell | Required state set | Current status |
|---|---|---|---|---|
| Arrivals | person standing/carrying, dock window, panel sign, hatch, lamp | berth window + arrival board | default, selected, speech, high contrast | authored candidate; runtime/native combined QA pending |
| Canteen / Bar | person standing/seated, counter, lamp, window, dice, panel, hatch | Second Shift authored scene | default, speech, free-practice choice/result, contrast | authored Second Shift asset; interaction/runtime QA pending |
| Cargo | person carrying, cargo rack, counter, hatch, panel, lamp | lift/manifest composition | default, selected, speech, disabled, result, contrast | generic fallback; Team/Composer allocation needed |
| Guild / Mission | person standing, survey console, panel, rack, hatch | mission desk/notice wall | default, offer, selected, speech, accepted/result, contrast | generic fallback; Gameplay/Story truth required |
| Clinic | person standing/seated, bench, panel, lamp, hatch | treatment bench/medical bay | default, selected, speech, disabled/result, contrast | generic fallback; content owner needed |
| Customs | person standing, console, panel, hatch, warning lamp | scanner gate | default, selected, warning/result, contrast | generic fallback; warning state must be truthful |
| Shop / Chandlery | person standing, counter, rack, panel, hatch | parts counter | default, selected, purchase/confirm/result, contrast | generic fallback; purchase UI owner needed |
| Market | person standing/seated, counter, panel, window | ledger/booth shell | default, selected, quote/confirm/result, contrast | outside crawl; Team allocation needed |
| Shipyard / Outfitting | person carrying, counter, rack, console, panel | bay/repair shell | default, selected, preview/confirm/result, contrast | outside crawl; Team allocation needed |
| GalNet | person seated, console, panel, lamp | terminal/reading shell | default, selected, speech/article/result, contrast | outside crawl; Team allocation needed |

## Shared rules

All screens retain room `(6,20,340,168)`, options `(354,20,120,168)`, feedback
from `y=190` and SHIP safe rectangle `(274,158,68,26)`. Runtime UI owns options,
speech, feedback, selected edges and stateful warnings. Room art must not imply
an unavailable job, purchase, destination, reward or audio event.

People and route doors are authored role anchors. Procedural composition must not
place duplicates over them. Variants may select declared props, material planes,
window bodies and controlled wear from stable identity; no gameplay RNG.

Story & Dialogue owns speech copy, speaker roles and box-flow readability.
Gameplay owns offer/purchase/mission/dice truth. Systems & QA owns selector/hash,
budget, fallback and save/runtime safety. PSP UI & Art Procedural Room Composer
owns family assembly/dispatch. PSP UI & Art Manager owns cross-screen art
direction and integration review. PSP UI & Art Reusable Element Kit owns these
modular authored pieces. Team allocation is still required for unowned content
screens before production art is multiplied.

## Native QA gate

Each promoted screen needs normal + high-contrast 480×272 captures for default,
selected, speech, disabled and result states as applicable; static packed bytes;
frame-time delta; safe-area/SHIP checks; repeatable identity; and unchanged
generic fallback on other rooms/hubs. The current assembled kit frames are
layout/contrast evidence only, not runtime captures.
