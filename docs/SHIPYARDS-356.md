# 2.5.356 — Orbit Motors and Crown Grand

Implemented on the current local 2.5.355 source and release. The Pilot's Field Guide, exact boost instruction, Cargo console, Circuit Console Loadout, separate Item Storage and V32 saves are preserved.

## Native presentation

Both yards are drawn directly at 480×272 using `src/font8.h`. Normal text has 8px advance; dealer wordmarks use integer 16px glyphs and the exchange heading uses crisp 9px glyphs. No full-screen generated image is shrunk to fit the PSP.

Regular yards use Orbit Motors' green workshop, seven-row left catalogue, real rotating ship above the exchange box and a full-width bottom reading area. Crown Grand is the approved Mega layout: large gold dealer wordmark, panoramic orbital showroom, illuminated platform, small service craft and crew, three-row lower-left catalogue, lower-right reading panel and compact upper-right exchange box. A Mega hull selected at Lave also uses this presentation, so the full test catalogue can be viewed there.

Every canonical system/hub pair has its own deterministic dealer name. Lave's primary capital showroom is Crown Grand. Other dealer names combine short authored names through a collision-free mapping of system and hub identity. Their native scene uses the site's stable hash for planet, palette, window ribs, facility and traffic placement. Ship models and paint come from the real hull catalogue and decorator colours. Rotated source bounds determine model scale on every frame; the renderer clips only within the measured aperture and resets clipping afterwards.

## Exact principal coordinates

| Region | Regular x,y,w,h | Mega x,y,w,h |
|---|---|---|
| Header | 0,0,480,22 | 0,0,480,22 |
| Catalogue | 8,69,170,124 | 8,181,176,62 |
| Showroom | 188,58,284,94 | 8,65,464,108 |
| Model aperture | 207,69,246,70 | 113,76,262,78 |
| Exchange | 188,155,284,40 | 288,26,184,36 |
| Reading panel | 8,198,464,46 | 192,179,280,69 |
| Credits | 332,252,140,17 | 332,252,140,17 |

The header reads EXCHANGE YOUR SHIP. Payment underneath reads COSTS: followed by exact net units; a negative exchange reads YOU RECEIVE: with a positive amount. The compact box omits only redundant `.0` on a whole-unit refund if necessary to preserve readable 8px text. Current ownership reads CURRENT SHIP instead of implying that the player can sell the ship to itself.

Stat labels are gold, values teal. Native green up/red down arrows compare the selected bare hull against the player's current bare hull. Equal values have no arrow. HEAT CAP explicitly means capacity, so a larger value is an improvement. Cargo, range, speed, hull, shields and heat capacity all read the authoritative PlayerShip table, not a cosmetic stat list.

## Controls and economy

- Up/Down browses the complete local catalogue through a bounded scroll window.
- Square switches the same reading area between comparison stats and the complete description. Initial view is stats.
- X uses the existing ship exchange and purchase validation, then the existing congratulations card.
- Circle closes the card or returns to the calling menu.

The existing checks for money, cargo capacity, fitted banks and local availability remain in `buy_ship`. No save-format change. All ten standard hulls remain available. Lave retains all twelve Mega hulls. Other Mega dealers each offer four capital hulls, now chosen from the 495 distinct four-of-twelve combinations instead of twelve repeated cyclic collections. Their canonical system/hub ordinal is permuted into this space, giving every actual capital dealer a distinct deterministic collection. Purchase, display and availability checks share this catalogue.

## Verification and limitations

Run `tools/review-shipyard-console.ps1`. Its opt-in EBOOT fixture uses production renderer and input: complete descriptions, all payment/refund combinations, unique dealer names/images/stock, selection, Square, pause, arrows, model clipping and real exchanges. Native BMPs cover regular/Mega stats and descriptions, refunds, owned hulls and three other capital dealers.

`BUILD-2.5.356.txt` records final evidence, regression comparison, download hash and package paths. The inherited broad game-check failures and physical PSP certification remain separate from this UI work. The shared checkout contains unrelated unfinished changes; do not publish or commit it wholesale. Git pull was attempted before editing but blocked by existing untracked music placeholder files, with the autostash restored. Player music was not moved or overwritten.
