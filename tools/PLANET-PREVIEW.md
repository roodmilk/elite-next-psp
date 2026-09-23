# Planet profile preview

Generate an offline profile manifest and 256-system atlas with:

```text
node tools/generate-planet-previews.mjs --out work/planet-preview
node tools/validate-planet-previews.mjs --manifest work/planet-preview/planet-profiles.json
node tools/export-planet-content-pack.mjs --manifest work/planet-preview/planet-profiles.json --out work/planet-preview/planet.content
```

The Node generator mirrors the deterministic arithmetic in `src/planet-profile.h`
and `src/sectors.h`. It emits:

- `planet-profiles.json`: profile fields for every landable/ocean world.
- `planet-profiles.ppm`: a dependency-free 256×256 contact atlas; each 16×16
  tile represents one system and each coloured block represents a planet.

This is an authoring/QA tool. The PSP still derives profiles at runtime and
does not load the JSON or PPM files.

`generate-planet-previews.py` is also provided for contributors who prefer a
Python-only authoring environment; the Node version is the repository smoke
path because Node is already used by the ship extraction tools.

The validator is an offline release gate. It checks 256-system/768-body
coverage, bounded profile ranges, and parity with the existing orbit art family.
A future runtime adapter must pass the same checks before it can influence
traversal, collision, or save data.

`planet-art-contract.json` is the versioned handoff for offline art tools. It
defines the independent seed channels, family mapping, stable scannable rule,
native PSP budgets, and representative capture matrix. Blender, Aseprite, or
heightfield exporters should consume this contract rather than inventing new
runtime-facing IDs.

Station/activity authoring can be previewed independently with:

```text
node tools/generate-station-activity.mjs --out work/planet-preview
```

This emits three stable station records per system for the hub, relay, and
frontier outpost. It is intentionally offline until the Station owner signs
off on the runtime presentation interface.

The exporter writes `EPWP` version 1: fixed-size 16-byte records with a
checksum-protected header. The PSP loader validates the header and checksum,
but the pack remains advisory until Planetary approves using records to drive
traversal or art systems.

When `planet.content` is placed beside `EBOOT.PBP`, the optional loader validates
and accepts it at boot. The smoke harness copies that sidecar automatically;
missing packs still use deterministic runtime generation.
