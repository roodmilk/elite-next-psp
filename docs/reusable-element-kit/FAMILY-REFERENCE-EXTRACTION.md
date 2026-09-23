# PSP family-reference extraction handoff

This is the art extraction brief for the first authored station families. The source images are references for hand-authored native pixel art; they are not runtime textures and must not be shipped as-is.

## Non-negotiable native target

- Compose at `480x272`, one framebuffer pixel per authored pixel.
- Reserve the established room frame at `x=6,y=20,w=340,h=168`; options remain `x=354,y=20,w=120,h=168`.
- Keep the feedback strip at `y=190` and the SHIP control in its safe region. Do not put routine X/Circle legends along the bottom of a bar.
- Use nearest-neighbour preview only. No resampling, blur, subpixel placement, or large-image downscaling as an art workflow.
- Runtime candidates must be reduced to the canonical 11-role palette, packed 4-bit indices, and the current family budget/draw-call ceilings.

## What the station and gameplay contracts require

The art may show only capabilities the station manager exposes as hotspots and the gameplay systems can resolve. A prop, person, or control is not interactive merely because it is visible. The extraction pass therefore tags every anchor with a hotspot kind: `person`, `exit`, `prop`, or `feature`.

The story team owns all speech, names, and result copy. Art supplies readable pose and identity cues only; it does not invent dialogue, mission outcomes, economy effects, or audio playback. Audio cues/warnings may be suggested by lighting only when the verified state supports them.

## Prospector / Research authored targets

The following anchors are required for the first rich vertical slice. Their suggested native footprints are deliberately small enough to tile into multiple arrangements while retaining a strong family landmark.

| Family | Anchor | Native target | Depth/material cue | Contract owner |
|---|---|---:|---|---|
| Prospector | ore/sample wall with claim tags | 92x72 | rust, ochre, cream; rear wall landmark | station + gameplay |
| Prospector | assay bench and task lamp | 74x42 | warm lamp pool; foreground interaction | gameplay |
| Prospector | cargo rack / packed crates | 52x64 | dark voids between crates; side depth | station |
| Prospector | lander/desert window | 82x52 | cyan rim, dust-gold exterior | station + planetary |
| Prospector | standing prospector with sample case | 18x48 | strong silhouette; readable case | story + gameplay |
| Prospector | seated miner silhouette | 18x38 | secondary activity, no duplicate pose | story |
| Research | specimen cabinet | 64x86 | cool glass, amber specimen points | gameplay |
| Research | survey console | 70x44 | cyan controls, cream desk plane | gameplay |
| Research | telescope / orbit instrument | 58x58 | diagonal silhouette; focal prop | station + gameplay |
| Research | ringed-planet orbit window | 104x64 | cyan/olive sky; family landmark | station + planetary |
| Research | standing researcher | 18x48 | upright silhouette, clipboard/scan wand | story |
| Research | seated researcher / lounge chair | 22x40 | relaxed secondary pose | story |

Each family must also receive a shared set of neutral hull seams, floor bands, overhead lamps, benches, doors, and wall panels from the reusable kit. The family landmark is selected by the frozen `room-selector-v1` cosmetic identity; gameplay state must not be selected by that hash.

## Extraction order

1. Block the room depth stack and family landmark at native resolution.
2. Hand-author the three to five anchor clusters above, including people/activity silhouettes.
3. Produce normal and high-contrast variants without changing hotspot truth or anchor placement.
4. Export a deterministic arrangement manifest with element IDs, pivots, draw order, palette roles, packed byte cost, and hotspot kind.
5. Submit to Systems for byte-equality repeat, budget, and allocation checks; then submit to Station for hotspot/availability checks; then Story for pose/readability and copy hooks.

## Acceptance gate

The current assembled Freight/Prospector/Research sheets remain layout and contrast evidence only. They fail the authored-art gate until they have layered depth, a strong landmark, 3–5 real material clusters, readable people/activity, and a deliberate light pattern. Sparse geometric placeholders must not be promoted to the vertical slice.

The first proof order remains Reorte H0 Arrivals → Canteen/Bar → Shop → Cargo → Guild/Mission → Clinic → Customs, then Qube H0 Freight and Xequerin H0 Research. H1/H2 use generic fallback until the proof pass is complete.
