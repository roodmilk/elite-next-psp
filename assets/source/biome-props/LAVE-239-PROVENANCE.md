# Lave environmental props

Mode: built-in imagegen tool, imagegen skill, transparent_background=true.
Edit target: assets/source/biome-props/kit-pending.png (earlier generated kit; original prompt in README.md).

Saved original output: C:/Users/skarm/.codex/generated_images/01a0df5c-bb6e-7c81-be10-50ed226d4e55/exec-15b6eaac-1788-4f71-98de-32fb20bf4c2a.png.
Project copy: assets/source/biome-props/kit-alpha.png.
Bake: tools/bake-biome-props.py.
Outputs: assets/generated/biome-props/kit.png and src/generated/biome-props-pixels.h.

RGBA alpha was inspected directly: empty corners are alpha 0 despite coloured RGB data remaining underneath transparency in some previews. Runtime conversion thresholds alpha at 160. Do not flatten the source onto its hidden RGB background.

## Exact final edit prompt

Use case: background-extraction. Edit target: the attached 4-column by 6-row pixel-art environmental prop sheet. Remove ONLY every coloured gradient/background pixel and replace with genuine transparent alpha, including all spaces between branches and leaves. Preserve all 24 individual props, their exact four-column/six-row grid positions, pixel shading, colours, scale, complete silhouettes and bottom alignment. No opaque matte, no checkerboard baked into pixels, no cast shadows. Do not redesign or add objects. This must be a usable transparent sprite atlas for a game, not a presentation image.
