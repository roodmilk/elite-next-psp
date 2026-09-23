# Procedural generation and art pipeline handoff

Branch: `specialist/systems-qa`

## Delivered contracts

- `src/planet-profile.h`: save-neutral planet family/style/palette/density
  descriptor; rocky family remains `1 + (body.seed % 4)` for orbit/ground parity.
- `src/planet-noise.h`: bounded, allocation-free PSP terrain sample preserving
  the established terrain output.
- `src/station-profile.h`: C-parity station selectors from compiled `System`
  fields plus hub identity; presentation-only and not serialized.
- `src/planet-pack.h` / `src/station-pack.h`: optional checksum-validated
  `EPWP`/`EPST` sidecar loaders with deterministic fallback behavior.

## Offline artifacts

- `tools/generate-planet-previews.mjs`: 256-system/768-body profile manifest and
  contact atlas.
- `tools/generate-station-activity.mjs`: 256-system × 3-hub activity manifest.
- `tools/generate-planet-art-atlas.mjs`: 160 native-scale 32×32 art reference
  tiles.
- `tools/export-planet-content-pack.mjs`: `planet.content` / `EPWP` v1.
- `tools/export-station-content-pack.mjs`: `station.content` / `EPST` v1.
- `tools/validate-art-pipeline.mjs`: contract, bounds, identity uniqueness,
  pack-header, payload-size, and checksum gate.

Regenerate and validate the complete pipeline with:

```text
node tools/generate-planet-previews.mjs --out work/planet-preview
node tools/generate-station-activity.mjs --out work/planet-preview
node tools/generate-planet-art-atlas.mjs --out work/planet-preview
node tools/export-planet-content-pack.mjs --manifest work/planet-preview/planet-profiles.json --out planet.content
node tools/export-station-content-pack.mjs --manifest work/planet-preview/station-activity.json --out station.content
node tools/validate-art-pipeline.mjs --dir work/planet-preview --contract tools/planet-art-contract.json --pack planet.content
```

## Safety boundary

No `Game` or commander-save fields were added. Sidecars are advisory inputs;
missing/invalid packs fall back to deterministic runtime generation. Station
records are not yet consumed by room rendering, NPC simulation, mission payout,
or traversal. That final presentation slice requires Station/Planetary owner
allocation and the acceptance matrix in `docs/STATION-ACTIVITY-INTEGRATION.md`.

Latest validated checkpoints include `f636bff` and `471eb4d`; the complete
branch history remains reviewable and unmerged.
