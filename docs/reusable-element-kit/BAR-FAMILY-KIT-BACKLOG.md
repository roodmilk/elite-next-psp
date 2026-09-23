# Six bar families — reusable kit extraction backlog

Concept sheet reference: `outputs/bar-concepts/space-station-bar-concept-sheet-v1.png`.
Second Shift remains the visual benchmark for natural people, layered depth,
warm practical light and native readability. The sheet is direction only; no
full-screen panel becomes a runtime texture.

## Shared structural kit

| ID | Native size | Pivot / hitbox | Families | Packed target |
|---|---:|---|---|---:|
| wall-rib-short | 32×72 | wall / none | all | 1,152 B |
| floor-band | 160×18 | floor-left / none | all | 1,440 B |
| hatch-route | 52×60 | feet / full route | Freight, Frontier, Outlaw | 1,560 B |
| window-frame | 128×54 | wall-center / sill | Freight, Prospector, Research | 3,456 B |
| counter-front | 96×42 | front-left / top strip | Freight, Merchant, Frontier | 2,016 B |
| booth | 72×48 | floor / seat zone | Research, Merchant, Outlaw | 1,728 B |
| shelf-rack | 48×52 | floor-left / full rack | all | 1,248 B |
| task-lamp | 21×18 | ceiling / none | all | 189 B |
| notice-board | 64×36 | wall / read panel | Freight, Frontier, Outlaw | 1,152 B |

## Actors and poses

All actors use feet/seat pivots and fixed hitboxes. The composer selects role
and pose from stable station identity; it never spawns a second actor over an
authored anchor.

| ID | Native size | Pivot / hitbox | Read |
|---|---:|---|---|
| bartender/clerk-standing | 24×52 | feet(12,52) / 24×52 | service posture, readable head and arms |
| carrying-worker | 28×52 | feet(14,52) / 28×52 | manifest, ore case or tool case |
| seated-patron | 30×42 | seat(15,42) / 30×42 | table/booth conversation |
| researcher-at-console | 32×48 | feet(16,48) / 32×48 | hands toward survey display |
| mechanic-working | 30×50 | feet(15,50) / 30×50 | crouched or bench-facing silhouette |

Normal and high-contrast variants preserve head/body/arm silhouette. Clothing
accent roles may change by family, but faction/danger meaning does not belong to
the cosmetic palette.

## Family backlog

| Family | Hero pieces | Identity props | Lighting / depth variant |
|---|---|---|---|
| Freight Berth Taproom | counter, berth window, timetable, hatch | cargo clerk, manifest, cargo marks | warm amber work lamps, slate berth depth |
| Prospector Dusthouse | assay bench, ore wall, lander window | ore/sample rack, sample case | dusty ochre task light, rust/olive wear |
| Research Observation Lounge | planet arch, console, specimen cabinet | specimen case, telescope/survey panel | cool cyan instruments, charcoal quiet depth |
| Merchant Orbital Salon | curved counter, manifest canopy, booth | broker ledger, display rack, brass lamp | polished amber, cream highlight planes |
| Frontier Repair Mess | service hatch, repair bench, lockers | tools, ration tin, mechanic case | low olive worklight, slate utility depth |
| Outlaw Coded Den | guarded hatch, notice wall, card table | coded board, cards/dice, screened booth | shaded rust; danger red only for real danger |

## First vertical slice

1. Freight: add clerk/carrying-worker, timetable board and berth-window depth
   variant around the existing Second Shift quality bar.
2. Prospector: add assay bench, ore rack, lander-window body and sample-case
   pose, with native contrast capture.
3. Research: add observation window, survey console, specimen cabinet and
   seated/researcher pose, with native contrast capture.

For each: compose one H0, H1 and H2 sample; capture default, selected, speech,
disabled and result states where applicable; measure packed bytes and frame
delta; verify stable identity and unchanged generic fallback. Story approves
labels/copy, Gameplay approves actions, Systems freezes selector/hash/budget,
and the Composer owns placement/dispatch.

## Gaps

Merchant, Frontier and Outlaw remain kit backlog only. No runtime room plan,
mission truth, faction meaning, audio state or save behavior is implied by these
assets. Their first implementation requires Team allocation and the same native
evidence gate.
