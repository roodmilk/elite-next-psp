# Arrivals art delivery

Base: `1e3fab88090e328916dc487f1c99031daf0a9061`.

## Runtime-ready room candidate

- `src/station-arrivals-art.h` exposes `station_arrivals_art_draw(x,y,w,h)`.
- `src/generated/station-arrivals-pixels.h` is a native 340×168 packed 4-bit room: 28,560 bytes of indices plus 44 bytes of palette.
- The helper bounds every write to the 480×272 framebuffer and does not touch `Game`, RNG, input, audio or UI state.
- The room intentionally does not own the options panel, feedback band, dynamic SHIP return, hotspots or stateful text. The station screen must draw those with the existing runtime UI.
- Exact implementation hook: arrivals scene branch only. Do not replace canteen/bar or other room renderers with this asset.

The first generated Arrivals frame was superseded as a runtime source after the project-wide native-first rule. The production candidate is now `src/station-arrivals-authored-art.h` plus `src/generated/station-arrivals-authored-pixels.h`, generated directly on a 480×272 canvas with the canonical 1× palette and then packed only for runtime storage. It is the asset to review for integration.

## Native review frame

`outputs/arrivals-native-authored.png` and its contrast companion are 480×272 native-first review frames using the directly authored room plus the repository 8×8 font and canonical options/feedback/SHIP composition. They are not runtime captures. `outputs/arrivals-native.png` and `assets/concepts/arrivals-source.png` are reference-only generated material; source text/UI is not trusted.

The native-first review frame shows a dock clerk, traveler, ribbed concourse, layered berth rail, arrival board, freighter and orbital body. The lower-right SHIP rectangle is cleared for the runtime return affordance. It is a bounded native candidate, not proof that the whole game has reached the Second Shift richness target.

## Shared family contract

The six reusable family definitions and palette roles live in `src/station-art-style.h` and `docs/STATION-ART-LANGUAGE.md`. Arrivals is the first additional complete screen after Second Shift. Market, Shipyard/Outfitting, Mission Board and GalNet currently have role/layout contracts only; they are not represented by invented runtime screenshots.

## Remaining gates

Wire the helper through the existing Arrivals scene on the integrated Station branch, preserving current hotspots and options. Capture normal and high contrast native frames, check all bounds, measure static bytes and frame cost, and verify other rooms remain unchanged. Only then promote this from runtime-ready asset to shipped screen.
