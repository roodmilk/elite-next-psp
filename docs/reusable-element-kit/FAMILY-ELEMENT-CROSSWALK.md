# Family element candidate crosswalk

Candidate extraction commit: `8ce19e4`  
Status: native candidate evidence only; not runtime-ready and not a replacement
for the frozen generic manifest.

The family candidates are richer authored variants with deliberately different
footprints from the generic Element Kit. They must therefore receive new stable
manifest IDs or an approved variant mapping; they must not be silently cast to a
generic ID with a different pivot, hitbox or packed cost.

## Candidate → frozen role mapping

| Candidate ID | Frozen role / owner | Native size | Frozen generic size | Mapping decision |
|---|---|---:|---:|---|
| `prospector-ore-wall` | new Prospector landmark / Station + Gameplay | 92×72 | — | new authored ID required |
| `prospector-assay-bench` | `assay-bench` / Gameplay | 74×42 | 88×28 | authored family variant; new footprint metadata required |
| `prospector-cargo-rack` | `cargo-rack` / Station | 52×64 | 48×52 | authored family variant; new footprint metadata required |
| `prospector-lander-window` | `dock-window` body/window role / Station + Planetary | 82×52 | 128×54 | family-specific body window; authoritative query required |
| `prospector-standing-sample-case` | `person-standing` / Story + Gameplay | 18×48 | 24×52 | authored pose variant; new pivot/hitbox required |
| `prospector-seated-miner` | `person-seated` role / Story | 18×38 | 30×42 | authored pose variant; new pivot/hitbox required |
| `research-specimen-cabinet` | new Research landmark / Gameplay | 64×86 | — | new authored ID required |
| `research-survey-console` | `survey-console` / Gameplay | 70×44 | 64×46 | authored family variant; new footprint metadata required |
| `research-telescope` | new Research focal prop / Station + Gameplay | 58×58 | — | new authored ID required |
| `research-orbit-window` | `dock-window` body/window role / Station + Planetary | 104×64 | 128×54 | family-specific body window; authoritative query required |
| `research-standing-researcher` | `person-standing` / Story | 18×48 | 24×52 | authored pose variant; new pivot/hitbox required |
| `research-seated-researcher` | `person-seated` role / Story | 22×40 | 30×42 | authored pose variant; new pivot/hitbox required |

All twelve candidates have normal and contrast files. Their current manifest
marks them `runtime_ready:false`; that flag stays false until Systems validates
packed cost, Station validates hotspot ownership, Planetary validates windows,
and Story validates pose readability.

## Composer placement contract

Coordinates below are relative to the room viewport `(0,0,340,168)` whose screen
origin is `(6,20)`. They preserve the options column, feedback strip and SHIP
safe rectangle. They are proposed layout slots, not hitboxes.

| Family | Draw layer | Candidate slots `(x,y)` | Anchor / hotspot owner |
|---|---|---|---|
| Prospector | back | ore wall `(16,20)`, lander window `(132,20)` | Station/Gameplay; Station/Planetary |
| Prospector | mid | cargo rack `(240,34)`, assay bench `(34,110)` | Station; Gameplay |
| Prospector | front | standing prospector `(126,112)`, seated miner `(196,120)` | Story/Gameplay; Story |
| Research | back | orbit window `(16,20)`, specimen cabinet `(132,28)` | Station/Planetary; Gameplay |
| Research | mid | telescope `(216,36)`, survey console `(44,112)` | Station/Gameplay; Gameplay |
| Research | front | standing researcher `(132,112)`, seated researcher `(190,124)` | Story; Story |

The composer may place these slots deterministically from the frozen selector
hash only after the approved family arrangement is selected. It may not derive
capability state, NPC identity, dialogue, body identity, discovery, danger or
audio from the hash. People and doors remain single-owner layers; the composer
must not duplicate Station/Story-authored placements.

## Fidelity review result

The 480×272 proof composes are useful for footprint, contrast and packing review,
but are visually sparse compared with the Second Shift floor: they do not yet
prove a layered room shell, complete material clusters, authored activity density
or a full 3–5-hotspot playable room. Keep them in evidence status until the
family shell, lighting, people/activity and truthful interaction state are
assembled and captured in the integrated PSP build.
