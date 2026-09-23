# Reusable Station Element Kit — native-first handoff

Concept/asset handoff only. No station runtime source is changed by this kit.

The kit is authored for the PSP framebuffer: 480×272 at 1×, hard pixel edges,
nearest-neighbour only. The contact sheet is a native composition, not a large
illustration reduced into a blur. Use the Second Shift’s warm pulp lighting and
layered depth while allowing the room family to change structure.

## Shared contract

- Room bounds: x6 y20 w340 h168.
- Options bounds: x354 y20 w120 h168.
- Feedback begins at y190.
- SHIP safe rectangle: x274 y158 w68 h26.
- Palette is the existing 11-role station palette: VOID, CHARCOAL, SLATE,
  OLIVE, RUST, OCHRE, CREAM, LAVENDER, CYAN, AMBER, DANGER.
- DANGER is semantic only; CYAN is functional light/scan; CREAM/OCHRE carry
  readable structure; CHARCOAL/SLATE carry depth.

## Element register

Static cost assumes packed 4-bit indices: width×height/2, plus one shared 44-byte
palette per room asset. It is a planning cost, not a measured compiled result.

| Element | Native size | Pivot | Hitbox / anchor | Freight use | Prospectors / Research use | Contrast behavior |
|---|---:|---|---|---|---|---|
| person standing | 24×52 | feet (12,52) | 24×52 | clerk/loader | prospector/researcher | preserve head/body silhouette; accent becomes CREAM |
| counter | 96×42 | front-left | top interaction strip | service/freight | assay counter variant | keep top edge OCHRE/CREAM |
| task lamp | 21×18 | ceiling mount (10,0) | none | amber work lamp | dusty or cyan task lamp | retain fixture silhouette; lamp role changes |
| dock window | 128×54 | wall center | lower sill | berth/freighter | lander/observation window | retain frame and body silhouette |
| cargo rack | 48×52 | floor-left | 48×52 | crates/timetable | sample racks/specimens | rack bars remain visible |
| assay bench | 88×28 | front-left | top work surface | manifest bench | ore/sample bench | work surface remains bright |
| survey console | 64×46 | feet (32,46) | 64×46 | arrival display | instrument/scan display | cyan functional bars remain |
| panel/sign | 48×24 | wall top-left | optional read hotspot | departures/notice | assay/observation label | label field stays CREAM |
| door/hatch | 52×60 | floor center | full route hitbox | cargo/berth route | lab/lander route | outline cannot disappear |
| dice prop | 42×18 | table center | 42×18 only when active | optional bar activity | not ambient decoration | pips stay CREAM/AMBER |

## Family assembly rules

Freight uses counter + task lamp + dock window + cargo rack. The hero is the
berth/window relationship; people should be placed in depth, not in a row.
Prospectors use assay bench + sample rack + lander window, with dusty OCHRE
light and visible evidence shapes. Research uses survey console + observation
window + specimen rack, with CYAN functional signals and quieter CHARCOAL depth.
These are structural variants, not palette swaps.

Procedural selection may vary rack contents, window body, lamp state and panel
marks from immutable station/body identity. It must not vary the SHIP return,
interaction anchors, NPC identity, mission truth, collision or palette roles.

## State coverage required before runtime use

Each assembled room needs native normal and high-contrast captures for default,
selected, speech, disabled and result states. Purchase, mission, dice and GalNet
specific states need their own owner and truthful state contract. The kit does
not imply any gameplay state or save behavior.

## Depth and pose extensions

The next authored pose set is `person-carrying` (28×52, feet pivot 14,52,
728 packed 4-bit bytes) and `person-seated` (30×42, seat pivot 15,42,
630 bytes). They add readable arms/tools and seated head/torso silhouettes.
They remain authored role selections; composition must not duplicate people.

Depth variants are authored modules: counter-front/counter-front-shadow,
window-frame/window-body and rack-full/rack-sparse. They keep identical anchors
while changing material planes and controlled wear. The outside window body is
authored atmosphere; a procedural body query may supply only a truthful outside
silhouette or accent. It must not create a destination, interaction or mission.

Measured packed 4-bit costs, excluding compiler alignment and one shared 44-byte
palette: standing person 624, carrying person 728, seated person 630, counter
2016, lamp 189, window 3456, rack 1248, bench 1232, console 1472, panel 576,
hatch 1560 and dice 378 bytes. These are asset-size measurements, not frame-time
claims. The manifest records assembly totals once a pose/content set is frozen.

## Gaps and ownership

PSP UI & Art Manager owns Second Shift/Canteen/Bar scene assembly and shared
presentation. PSP UI & Art Procedural Room Composer owns Arrivals, station
exteriors and planet/body window composition. PSP UI & Art Reusable Element Kit
owns these modular pieces. Market, Shipyard, Outfitting, Mission Board and GalNet
content art still require Team allocation. No shared runtime hunk is requested
by this kit; Team must reserve the eventual asset include/dispatch points.
