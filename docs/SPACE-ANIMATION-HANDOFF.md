# ELITE: NEXT — space animation handoff

**Status:** proposed implementation batch, unified release baseline  
**Resolution:** native 480×272 software framebuffer  
**Ownership:** visual presentation only; no station-crawl input/state changes

## Current animated coverage

The unified release already contains these bounded animation families:

| Family | Source | Frames / timing | Purpose |
|---|---|---:|---|
| Sun families | `src/sun-sprites.h`, `src/art-runtime.h` | 8 families × 4 native 32×32 frames | warm/cool star identity, charts, approach and surface sun |
| Planet families | `src/campaign-art.h`, `src/art-runtime.h` | seeded native sprite families | consistent world identity across map and flight |
| Nebula ribbons | `src/space-fx.h` | deterministic particles + time drift | painted depth behind the cockpit |
| Meteors | `src/space-fx.h` | rare 0.55s event window | occasional life in deep space |
| Star twinkle | `src/space-fx.h`, `src/flight-extras.h` | seeded phase | restrained moving star field |
| Warp/speed | `src/flight-extras.h`, `src/voyage.h` | speed/jump-driven primitives | motion and acceleration language |
| Engine/freighter wakes | `src/voyage.h`, `src/flight-extras.h` | speed/faction/state-driven | ship scale and traffic readability |
| Station windows | `src/voyage.h` | bounded 1px practical-light pulses | lived-in orbital station exterior |
| Native animation masks | `src/space-animation-kit.h` | 3 families × 4 8×8 frames | shared plume, beacon and sparkle language |

## Current batch: station exterior windows + shared masks

`station_window_animation()` adds deterministic warm practical lights to the exterior station silhouette used by the menu space preview and docking arrival camera. It is intentionally:

- 10 lights in ordinary systems, 20 in prosperous systems;
- one-pixel native sparks with a small pulse;
- seeded by system and station angle;
- drawn only after the station model and before the ship/arrival foreground;
- independent of station-crawl geometry, room state and interaction verbs.

`space-animation-kit.h` now supplies four-frame native masks for:

- plume: engine flare animation;
- beacon: station window/docking-light pulse;
- sparkle: meteor head bloom.

These are shared visual primitives rather than separate gameplay systems. The caller supplies colour and timing, so law/trader/explorer identity and gameplay triggers remain outside the art kit.

## Next large animation drop

For the next BIG RELEASES ONLY batch, add authored native sheets for:

1. four-frame engine plume families: amber courier, cyan explorer, red raider, white law;
2. four-frame docking beacon/aperture states: idle, sweep, aligned, arrival;
3. six-frame creature silhouettes for one rare fauna encounter;
4. four-frame station traffic markers for arrival, loading, turn and departure;
5. four-frame planetary atmosphere glints keyed to the six biome families.

Each sheet must include a source image, native-size preview, frame dimensions, timing, palette role, memory cost and reduced-effects behavior. Do not add a sheet merely to decorate an empty screen.

## Performance rules

- Keep sheets native-size and nearest-neighbour.
- Use fixed arrays or baked headers; no heap allocation in the frame loop.
- Keep animation families bounded and deterministic.
- Never animate over dialogue, objective text or target cards.
- High contrast/reduced effects must remove decorative motion without removing navigation or safety feedback.
- Measure the densest combined scene on PSP hardware before promoting the batch.

## Release boundary

This handoff does not edit `src/station-crawl.h`. The release agent owns station interaction integration. The art lane supplies the frame sheets, palette roles, timing and composition targets; gameplay owns when an animation is semantically triggered.
