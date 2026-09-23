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
- `tools/rebuild-procedural-pipeline.mjs`: one deterministic rebuild/index step
  for the planet, station, pack, room-descriptor and element-kit outputs.
- `tools/create-station-proof-scaffold.mjs` and
  `tools/validate-station-proof-slice.mjs`: 63-sample owner handoff and proof
  gate for Freight/Prospector/Research at H0/H1/H2 across seven rooms.
- `tools/validate-family-extraction.mjs`: native-size audit for the authored
  Prospector/Research candidate manifest.
- `tools/validate-family-element-budget.mjs`: per-family packed4bit budget
  check that keeps candidate art separate from the frozen generic kit.

Regenerate and validate the complete pipeline with:

```text
node tools/generate-planet-previews.mjs --out work/planet-preview
node tools/generate-station-activity.mjs --out work/planet-preview
node tools/generate-planet-art-atlas.mjs --out work/planet-preview
node tools/export-planet-content-pack.mjs --manifest work/planet-preview/planet-profiles.json --out planet.content
node tools/export-station-content-pack.mjs --manifest work/planet-preview/station-activity.json --out station.content
node tools/validate-art-pipeline.mjs --dir work/planet-preview --contract tools/planet-art-contract.json --pack planet.content
```

With the Station Art Manager's native candidate manifest, the complete rebuild
also accepts:

```text
node tools/rebuild-procedural-pipeline.mjs --out work/planet-preview --register <ROOM-COVERAGE-REGISTER.json> --element-manifest <frozen ELEMENT-MANIFEST.json> --family-manifest <family-elements/MANIFEST.json>
node tools/validate-family-extraction.mjs --manifest <family-elements/MANIFEST.json> --out work/planet-preview/family-extraction-audit-native.json
```

The current candidate result is 12 of 12 native dimension matches and both
family variants fit the 28,560-byte room candidate ceiling. It is not runtime
certification: hotspot ownership, selector placement, deterministic repeat,
normal/high-contrast native captures, zero-allocation proof and integrated
frame measurements remain pending.

## Safety boundary

No `Game` or commander-save fields were added. Sidecars are advisory inputs;
missing/invalid packs fall back to deterministic runtime generation. Station
records are not yet consumed by room rendering, NPC simulation, mission payout,
or traversal. That final presentation slice requires Station/Planetary owner
allocation and the acceptance matrix in `docs/STATION-ACTIVITY-INTEGRATION.md`.

Latest validated checkpoints include `f636bff` and `471eb4d`; the complete
branch history remains reviewable and unmerged.

The current PSP smoke harness still fails before `game-check.txt` and the other
report files are emitted. Build and offline validation pass; no native runtime,
input traversal, save/economy, dialogue, or performance certification is
claimed until the emulator/report boundary is repaired and the owner proof
packet is populated.
