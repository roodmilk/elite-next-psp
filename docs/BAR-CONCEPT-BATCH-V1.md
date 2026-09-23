# Space-station bar concept batch v1

**Status:** concept direction / kit extraction brief  
**Screen contract:** native 480x272; room 6,20,340x168; options 354,20,120x168; feedback from y190; SHIP safe area 274,158,68,26.  
**Reference sheet:** `outputs/bar-concepts/space-station-bar-concept-sheet-v1.png`

This batch expands the Second Shift bar into six distinct station identities while keeping one production language: painted 1950s science-fiction cover composition reduced to disciplined handheld pixel clusters, the canonical 11-colour palette, practical lighting, and 3–5 readable interaction anchors.

## Bar families

| Family | Hero read | Reusable emphasis | Gameplay/story hook |
|---|---|---|---|
| Freight Berth Taproom | berth window + timetable | counter, berth window, board, cargo clerk, work lamp | delays, freight tips, berth traffic, cargo jobs |
| Prospector Dusthouse | ore wall + lander window | assay bench, ore rack, dusty window, prospector poses | samples, mining rumours, survey leads |
| Research Observation Lounge | planet arch + survey console | observation window, console, specimen cabinet, seated researcher | scans, body identity, research requests |
| Merchant Orbital Salon | curved counter + manifest canopy | counter variants, display rack, booth, brass lamp, broker pose | prices, rare stock, passenger introductions |
| Frontier Repair Mess | service hatch + repair bench | hatch, bench, locker, tool rack, mechanic pose | repairs, ration trade, practical favours |
| Outlaw Coded Den | guarded hatch + notice wall | screened booth, card/dice table, coded board, shaded lamp | rumours, covert jobs, faction pressure; danger red only when gameplay says danger |

## Approved micro-identities

Each family gets one hero landmark, one repeatable prop motif and one local dialogue hook. Variation may change fittings, materials, outside view and wear; it must not imply a job, gambling, service or faction capability that Station and Gameplay have not enabled.

- **Freight:** arrival board; cargo window; shift mug marks.
- **Prospector:** sample shelf; scratched claim tags; one lit survey lamp.
- **Research:** telescope/orbit window; specimen case; quiet terminal.
- **Merchant:** route cards; ledger ribbons; private booth.
- **Frontier:** tool wall; patched lockers; repair queue board.
- **Outlaw:** covered lights; coded notice slips; guarded door, with no automatic illegal service.

## Kit extraction rules

- Extract shared structural pieces first: wall ribs, floor bands, hatch, window frame/body, counter, booth, rail, shelf, board and lamp.
- Extract readable actors second: bartender, clerk, miner, researcher, broker, mechanic, seated patron and carrying/working variants.
- Extract identity props third: ore/sample, manifest, specimen, tools, bottles, dice/card table, notice strips and cargo marks.
- Keep the room plan responsible for arrangement, family emphasis, wear and lighting pattern; keep the element kit responsible for authored pixels, pivots, hitboxes, normal/high-contrast variants and packed costs.
- Do not encode mission truth, economy, NPC identity, body identity or danger state in a cosmetic variant.
- Do not duplicate authored people, doors or hatches when the room plan already reserves those anchors.

## Acceptance set

The composer should produce one H0/H1/H2 sample for three families first: Freight, Prospector and Research. Each needs normal and high-contrast native captures plus default, selected, speech, disabled and result states. Story & Dialogue must approve speaker labels, copy, wrapping and choices; Gameplay must approve the available actions; Systems must approve selector hash, plan count, draw ceiling and packed budget.

The six-panel sheet is a direction reference, not a runtime bitmap. Runtime should use shared native elements and bounded plans so station identity comes from structure, hero object, lighting key, material wear and authored exceptions rather than six full-screen textures per station.
