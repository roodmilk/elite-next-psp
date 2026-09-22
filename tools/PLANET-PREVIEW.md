# Planet profile preview

Generate an offline profile manifest and 256-system atlas with:

```text
node tools/generate-planet-previews.mjs --out work/planet-preview
node tools/validate-planet-previews.mjs --manifest work/planet-preview/planet-profiles.json
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
