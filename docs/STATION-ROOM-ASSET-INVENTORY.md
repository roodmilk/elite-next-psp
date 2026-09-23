# Station room art inventory

Scope: seven logical rooms across 256 systems × 3 hubs. This is an ownership and readiness register, not a claim that every room instance has bespoke art.

Source baseline: main `1e3fab88090e328916dc487f1c99031daf0a9061`. Station’s population/coverage authority is `STATION-INTERIOR-COVERAGE-CONTRACT.md` and `BAR-COVERAGE-REGISTER.json`. The three-manager allocation is `PSP-ART-MANAGER-MATRIX.md`.

## Current logical-room status

| Logical room | Art owner | Concept/source | Integrated | QA/readiness | Missing work |
|---|---|---|---|---|---|
| Arrivals | Art 2 | Direct-native room candidate: `station-arrivals-authored-art.h`, 340×168 packed room, native normal/contrast PNGs | Reorte H0 conditional dispatch integrated by Station commit `19d140d` | Build succeeds; combined native captures, hitbox/readability/perf evidence pending | Representative H1/H2/family captures; authored variants beyond Reorte; final runtime evidence |
| Chandlery / Shop | Team allocation required | Existing generic shop renderer and `room-shop` preview reference | Existing truthful shop behavior | Existing gameplay only; no new style-migration evidence | Named owner; native-authored family screen; normal/contrast captures; purchase/sell state art |
| Canteen / Bar | Art 1 | Second Shift native room asset `f2fc14b8c083c06ebe9f5d3689778f265af8a4df`; Lysa/Pell/Dax | Station’s Reorte H0 preview shell is separate; art helper is not a claim of released runtime integration | Concept/native asset reviewed; combined interaction/state capture pending | Native talk/job/free-practice states; high contrast; closed/pre-kit/full-capacity states; final combined perf |
| Cargo | Team allocation required | Existing generic cargo renderer; no authored room asset | Existing truthful cargo behavior | No new art QA | Named owner; Freight/Frontier family asset; manifest/loading states; normal/contrast evidence |
| Guild / Mission Board | Team allocation required | Existing generic guild renderer; shell contract only | Existing truthful mission behavior | No new art QA | Named owner; Research/Freight family asset; offer/accepted/paid/empty states; native evidence |
| Clinic | Team allocation required | Existing generic clinic renderer; room-kit reference only | Existing truthful clinic behavior | No new art QA | Named owner; Frontier/Research family asset; treatment/result/empty states; native evidence |
| Customs | Team allocation required | Existing generic customs renderer; room-kit reference only | Existing truthful customs behavior | Existing gameplay only; no new style-migration evidence | Named owner; Frontier/Outlaw family asset; scan/clean/alert states; native evidence |

## Station-facing screens outside the crawl

Market, Shipyard/Outfitting, Mission Board and GalNet share the same native shell but are not room assets. Art 3 owns presentation chrome/footer/help only; it does not own their room illustrations. Team must allocate each screen explicitly before production art begins.

## Reusable family kits

Each kit changes structural massing, focal object, depth planes and lamp logic. Palette tint alone is not a variant.

| Family | Primary rooms | Authored modules | Procedural inputs allowed |
|---|---|---|---|
| Freight | Arrivals, Cargo, some Guild | berth window, timetable/manifest board, counter, rail, cargo lane | stable berth clutter, manifest marks, lamp phase and wear from station identity |
| Prospectors’ Rest | Canteen, Cargo, frontier Shop | assay bench, ore samples, lander window, bunk, battered stools | sample silhouette, rack contents, dust/wear pattern |
| Research Lounge | Guild, Clinic, GalNet | observation window, survey console, specimen cabinet, instrument light | body window, instrument glyphs, cabinet contents |
| Merchant Salon | Shop, Market, Canteen | ledger rotunda, curved sales counter, private booth, display racks | stock display arrangement, brass/cream wear, lamp placement |
| Frontier Mess | Canteen, Cargo, Clinic, Customs | patched hatch, ration wall, repair bench, utility lockers | patch layout, tool silhouettes, functional light placement |
| Outlaw Den | Customs, later Canteen/Shop | coded notice wall, screened booth, guarded hatch, card table | notice arrangement and wear only; red remains explicit danger |

## Universal runtime contract

Every family uses 480×272, room `(6,20) 340×168`, options `(354,20) 120×168`, feedback from `y=190`, actual 8×8 font, and clear SHIP rectangle `(274,158) 68×26`. Existing hotspot rectangles, options ordering, speech layout and room exits remain gameplay-owned. If authored art includes people or doors, Station must bypass only the duplicate generic draw while retaining the corresponding hitbox and interaction entry.

## State coverage still missing

For every future authored room, native normal and high-contrast evidence is required for: default/empty, selected hotspot, speech, disabled/unavailable action, successful result, and explicit warning/error. Additional screens add their own truthful states: shop price/confirm/failure, mission offer/accepted/paid/full, dice rules/stake/confirm/reveal/result, and GalNet page/navigation. No art asset may imply those states before Gameplay/Systems provides them.

## Conflicts and decisions

- Do not duplicate the Station coverage register’s 768 identity mapping.
- Do not assign Market/Shipyard/Guild/Mission/GalNet/Clinic/Customs assets to Art 3 without Team approval; Art 3’s current reservation is presentation chrome only.
- Do not mass-produce family variants until Systems publishes the versioned deterministic family selector and Lead accepts one combined representative capture.
- Keep Reorte H0 Arrivals and Second Shift as bounded authored exceptions; generic fallback remains correct elsewhere.
