# ELITE: NEXT — native PSP art pipeline

The runtime art authority is the 480×272 PSP framebuffer. `tools/native-art-manifest.json`
defines the format and the first three benchmark compositions:

- `station_arrivals`: berth window, freighter and room interaction field.
- `planet_approach`: planet limb, atmosphere and sun-side composition.
- `campaign_dialogue`: portrait, speaker tail and readable narrative card.

## Rules

- Visible screen is 480×272; framebuffer stride remains 512.
- Runtime art uses ARGB1555 and nearest-neighbour sampling.
- Source art is authored at 1× native size or an integer-native sprite size.
  When a larger concept source is needed for composition review, it is never
  shipped directly: the native-grid baker rasterizes it once onto 480×272,
  snaps it to the runtime palette, and packs only the bounded 340×168 room
  viewport. This keeps the review frame and runtime payload on the same pixel
  geometry instead of relying on a filtered or repeatedly scaled derivative.
- Palette is limited to the shared 11-colour cover palette unless a documented
  runtime effect needs a derived shade.
- Safe bands are part of each scene contract. Art must not enter the header,
  footer, objective or interaction bands without an explicit layout change.
- Procedural art is acceptable for atmosphere, haze, stars and light; hero
  silhouettes and authored scene composition must have a named runtime owner.

## Validation

Run from the repository root:

```powershell
.\tools\validate-native-art.ps1
```

The validator checks the native screen, framebuffer stride, format, scale,
palette budget, scene bounds, safe bands, source files and runtime ownership.
The build includes `src/native-art-scenes.h`, so station viewport geometry uses
the same native layout contract as the manifest.

The PNGs under `assets/preview/` remain review references. A new authored bitmap
must be added to the manifest and pass the same dimensions/palette/nearest-neighbour
checks before it is wired into a screen.

Station-room family proof assets follow the same contract:

- `assets/second-shift-native-v2.png` / `src/generated/second-shift-pixels.h`
- `assets/qube-freight-canteen-v1-native.png` / `src/generated/qube-freight-canteen-pixels.h`
- `assets/xequerin-research-canteen-v1-native.png` / `src/generated/xequerin-research-canteen-pixels.h`

All three reviews are 480×272; each runtime header is exactly 28,560 packed
bytes for 340×168 4-bit pixels. The second and third scenes are selected only
for their deterministic proof identities; all other stations retain the
procedural room renderer until their family art passes the same gate.
