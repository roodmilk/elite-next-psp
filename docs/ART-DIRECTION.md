# ELITE: NEXT — visual identity direction

**Status:** proposed art authority, 22 September 2026  
**Scope:** planets, deep space, stations, MacVenture-style room scenes, HUD atmosphere and presentation  
**Platform constraint:** PSP-2000/3000, 480×272, native nearest-neighbour art

This is a visual direction document, not a claim that the proposed look is already implemented. Existing captures remain the **CURRENT** reference; concept boards and targets in this document are **PROPOSED**. Station-crawl code is owned by the station implementation lane and is intentionally untouched here.

## The north star

ELITE: NEXT should feel like a lost 1950s–70s aerospace illustration brought to life by a late-1980s handheld computer:

- **Illustrated first, glossy second.** Strong shape design, painted atmosphere and practical-looking light beat generic bloom or photoreal noise.
- **Built, not grown.** Ships, planets and rooms use joined plates, ribs, service panels, antennae, handrails and docking hardware. Every large form should imply how it is maintained.
- **Majestic scale with human evidence.** A tiny running light, figure, cargo pallet, window or landing pad gives the player scale.
- **One readable idea per frame.** At 480×272, the silhouette and light hierarchy must survive before texture detail is added.
- **Warm machines in cold space.** Deep charcoal/navy space frames rust, ochre, cream and dusty olive structures. Cyan and amber are signals, not wallpaper.

The reference is the *design grammar* of period production paintings and miniatures, not a reproduction of Ralph McQuarrie, Star Wars, Battlestar Galactica or any other protected design. New silhouettes, markings, names and compositions must remain original to ELITE: NEXT.

## Palette and material language

| Role | Colour | Use |
|---|---|---|
| Void | `#080D18` | Deep sky, untouched negative space |
| Charcoal | `#151C27` | Hull shadow, room recesses, UI ground |
| Slate | `#293646` | Structural plates and station walls |
| Dust olive | `#5A604C` | Survey gear, room panels, utility paint |
| Rust | `#8B4B37` | Wear, hazard bands, pirate salvage, warm shadow |
| Ochre | `#C18B4D` | Lamps, trade identity, sunlit hull edges |
| Cream | `#E5D2A3` | Labels, paper-like maps, readable neutral highlights |
| Lavender grey | `#9B9AA5` | Atmospheric haze, alien identity, cool metal |
| Signal cyan | `#55D4D4` | Selection, navigation, safe docking, law |
| Signal amber | `#F0B45B` | Objective, focus, trade and warm warning |
| Danger red | `#C85A4B` | Critical danger only; never routine decoration |

Use 4–7 values per object. Reserve pure bright pixels for a light source, a selected edge, or a safety-critical mark. Avoid full-screen gradients: use stepped bands, dither clusters and clipped additive glints.

## Lighting rules

1. Give every scene one declared key: star, docking beacon, practical ceiling strip, planet rim or console.
2. Add one soft fill from the environment: reflected planet colour, station windows, dust or hull bounce.
3. Keep the far side dark enough to preserve silhouette. Do not flatten the object with equal ambient brightness.
4. Make lights belong to hardware: lamps sit in housings, windows repeat with structure, beacons have a visible mount.
5. Use a maximum of two animated light families in a frame: e.g. station beacon plus engine pulse.
6. Lens ghosts and bloom are optional seasoning. They must never sit over text, targets or mission markers.
7. Reduced-effects mode removes flare, vignette and unnecessary flashing without changing scene readability.

## Subject targets

### Planets — “painted bodies, not UI marbles”

- Make the limb the hero: a thin, controlled atmosphere rim separates the body from the void.
- Use broad seeded regions: cloud belt, ocean value, desert plane, ice cap, volcanic stain or fungal haze.
- Give each world one iconic read at thumbnail size: ring angle, cloud slash, copper crescent, ice cap or blue limb.
- Keep surface detail below the approach silhouette. The player should identify the world before reading its name.
- Rings are sparse and slightly broken, with a shadow side. Gas giants are approach vistas, not busy texture balls.
- On maps and in flight, the same world keeps its palette family, phase, tilt and naming.

### Deep space — “ink with depth”

- Use three star depth bands: a few large coloured anchors, a restrained mid-field, and sparse pinpricks.
- Nebulae are asymmetrical painted masses, never a uniform gradient or a competing wallpaper.
- Place one or two framing elements—relay silhouette, belt, freighter wake, moon or sun flare—then protect negative space around the target.
- Motion is expressed through camera/parallax and a few purposeful streaks, not constant screen-wide particles.
- Every system gets a quiet/active composition seeded from danger, economy and body roster.

### Stations — “industrial icons”

- Read the station as a silhouette before its details: ring, spine, arms, port, cargo yard or clustered habitat.
- Use repeating construction logic: radial windows, maintenance lights, bay doors, antenna masts and service trusses.
- Docking entrances are cyan only at the aperture and approach markers; the rest remains structural.
- Prosperous hubs can add lit windows and cargo traffic. Dangerous or poor hubs use dark bays, patched plates and uneven light.
- Keep the approach frame open enough for manual docking. The station must be impressive without becoming a collision-reading problem.

### MacVenture rooms — “painted panels with gameable anchors”

Station interiors should look like illustrated adventure-game rooms, then gain restrained depth through low-poly planes, shadows and parallax:

- One strong establishing composition per room: desk, berth, market kiosk, guild counter, airlock or window.
- 3–5 interactable anchors with a clear silhouette and a small practical highlight.
- Background props use grouped shapes and texture blocks; mission-critical props get a distinct edge, icon or lamp.
- Walls are slate/olive/cream panels with rust seams and visible fasteners. Avoid pristine white corridors.
- Characters are adult, graphic silhouettes with 2–3 value groups, not tiny realistic faces.
- Dialogue panels are blue for named NPCs and orange right-tailed panels for the player, consistent with the existing UI identity.
- Camera framing should leave a quiet lower band for action feedback and footer controls.

## Screen composition contract

At 480×272:

- Keep the central scenic field legible from y=56–191.
- Keep routine captions in the established top band and target/telemetry in the bottom band.
- Place the visual focal point on a one-third intersection, not behind the reticle or objective text.
- Use a 2-pixel minimum silhouette gap between adjacent dark forms where possible.
- Author all small icons at native size (8×8 HUD, 16×16 surface); no smooth resampling.
- Validate the image at 1×, not only enlarged in an emulator window.

## Handoff targets for the station implementation lane

These are implementation targets, not requests to change station code in this lane:

1. **Room shell:** charcoal/slate planes, dusty olive utility panels, rust edge wear, cream labels.
2. **Practical light:** one warm ceiling or console key light; cyan airlock/wayfinding light; dark recesses.
3. **Panel language:** thin cream/cyan rules, amber focused action, no decorative borders around every prop.
4. **Depth:** 2–3 large depth planes, a contact shadow under objects, one parallax layer, no expensive full-scene post process.
5. **Interaction read:** each anchor has a shape cue and one small light/pulse; selected anchors use fill + edge, not colour alone.
6. **Character read:** 32×48 or 48×64 billboard, shadow, faction silhouette and speaker-colour dialogue.
7. **Performance:** fixed pools, no runtime asset loading, nearest-neighbour sprites, no heap allocation in the frame loop.

## Research basis

The direction translates broad, publicly documented period practices into original PSP art:

- [StarWars.com — How To Spot a Ralph McQuarrie](https://www.starwars.com/news/how-to-spot-a-ralph-mcquarrie): composition, refined palette and original ideas are useful principles; the project does not copy licensed imagery.
- [American Society of Cinematographers — Star Wars: Miniature and Mechanical Special Effects](https://theasc.com/article/star-wars-miniature-effects-dykstra/): repeatable miniature passes, practical hull lights and designed mounting points support our “built, not grown” rule.
- [Lucasfilm — The ILM Dykstraflex](https://www.lucasfilm.com/news/lucasfilm-originals-the-dykstraflex/): camera movement and layered passes inform parallax, not a literal visual asset source.
- [NASA Ames 1970s space-colony art archive](https://nss.org/settlement/nasa/70sArtHiRes/70sArt/art.html): public-domain period references for monumental station silhouettes, cutaways and warm/cool habitat lighting.
- [NASA Apollo crew-station lighting report](https://ibiblio.org/apollo/Documents/Apollo%20Experience%20Report%20Crew%20Station%20Integration%20Volume%20I%20Crew%20Station%20Design%20and%20Development.pdf): floodlighting, incandescent task lights and electroluminescent panel logic inform believable interiors.

## Mockup labeling and review

- **CURRENT:** native captures of the present executable. They document what is implemented today.
- **PROPOSED:** concept board, paintover, palette card or station-room target. They communicate direction only.
- A proposed image must not be committed as runtime art until it has a source/provenance note, native-size reduction test, memory cost, frame-time result and a matching implementation acceptance test.

## Definition of visual done

An art pass is ready when the player can identify the scene, focal object and next safe action at native size; the scene still reads with reduced effects and high contrast; the same seeded world retains its identity across map/flight/approach; station rooms share the panel and light language; and the implementation remains inside PSP memory, draw-list and frame-time budgets.
