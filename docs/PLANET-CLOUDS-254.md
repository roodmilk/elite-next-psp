# Planet clouds — 2.5.254

## Visuals

Four cached code-native64x32 silhouettes use six overlapping rounded lobes, feathered pixel edges, bright rims and shaded undersides. No generated external bitmap dependency. Three sky banks use different distances, altitudes, widths and drift speeds; projected bearings still follow camera yaw/pitch. Cloud palette follows the local sky lighting.

Entry and ascent sample large, middle and near cloud layers with separate drift directions/rates. Scenery remains visible through partial cover; the final loading veil is fully opaque regardless of background. Cinematic paths, duration, pilot canopy, input suppression and saves are unchanged.

Cloud cache is8KiB, created once at startup. Palette/alpha tables are prepared per draw; bounded integer compositing replaces per-pixel floating colour interpolation and veil sine-wave evaluation. No per-frame heap allocation.

## Exact final-binary checks

Snapshot: current task/planet-clouds-254.

- final-cloud: zero-cover preservation, full-cover background independence and framebuffer guards.
- final-entry: automatic landing, spam suppression, equipment gates, translucent cover and opaque handoff.
- final-perf:144 identical reference/fast frames across16 worlds; framebuffer guards.
- final-paths: sampled64-world cinematic corridor safety.
- final-walk: scenery/collision/particle checks and seven viewpoints including cold vegetation refresh.
- final-art: all1024 world profiles/8192 species,12 POI families, foreground occlusion and bounds.
- final-game: game594, radio54, steering18 completed checks all pass. Full input/performance suite is not certified.

Native captures: final-cloud/atmosphere-1-0.bmp (landing), atmosphere-1-3.bmp (ascent), cloud-cover-00/04/08/12/16.bmp (cover progression), with all four Lave worlds also captured. These use the actual480x272 renderer.

## Performance — guest render times, not hardware FPS

Baseline253 with the same custom review vs final254:

| Measurement | 253 | 254 |
|---|---:|---:|
| Cloud cover compositing average |13.564ms|9.365ms|
|16 sampled arrival/ascent views average|49.921ms|47.676ms|
| Arrival/ascent worst |177.588ms|174.449ms|
| Lave aggregate144-frame comparison subset average|46.921ms|46.405ms|
| Seven walking views average including vegetation refresh|45.130ms|44.905ms|
| Walking worst|63.234ms|63.145ms|

Cloud effect costs about31% less; sampled arrival/ascent drawing about4.5% less overall. Improvements are modest for the whole renderer; terrain/cloud-deck geometry dominates heavy shots. Not locked30, seamless hardware-certified, or a fix for inherited input/outfitting/performance-suite failures. No physical PSP tests performed.

Initial old-colour comparison failed as expected for intentionally changed clouds. Replaced it with meaningful transparency/full-coverage checks; retained the old shader only as a QA benchmark. Initial entry launch used the wrong flag and was cancelled before the correct focused run.

## Delivery

EBOOT7355023 bytes; SHA256 F9E08AF9731C70D061EABB6D8960DD81B933DE59BB1EB6978460912DB42BF64D.

Canonical source and root/dist binaries match the tested snapshot; previous files are in canonical-before-254. Music/saves and cumulative253 features preserved. No public release or Git mutation.

Next: real PSP validation; optimise cloud-platform meshes and terrain/sprite overdraw; finish inherited full input/performance checks before claiming broader certification.
