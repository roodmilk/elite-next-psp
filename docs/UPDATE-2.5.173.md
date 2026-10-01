# 2.5.173 — Planet cinematics and exploration polish

## Changes
- Restored external 3D arrival, touchdown and departure shots. The ship descends to the pad, then automatically disembarks; departure pitches the painted hull upward with an engine plume and a darkening star field.
- Cinematic transforms use a separate render-only camera. They never temporarily overwrite the live Game camera, position or surface state. Existing unskippable input ownership, release gates, validated airlock exit and Triangle-board / R-launch flow remain intact.
- POI lettering is made of world-space glyph strips attached just outside a visible building wall, depth-tested against the scene. GARAGE appears on the exterior side/back walls, not over the rover. No added textures.
- Five deterministic sky traffic lanes at different heights/distances supplement the existing two spaceport landing/takeoff lanes. These are visual traffic, not targetable combat NPCs.
- Square + R begins a fast exponential shortest-bearing camera turn. Tracking brackets and a direction arrow beside the target name remain; ordinary manual looking cancels unfinished turning.
- Removed the redundant bottom ship-bearing/distance line and offscreen TARGET LEFT/RIGHT/BEHIND text. Planet controls, target computer and transfer prompts use existing button glyphs. Compass ship carries a green Triangle glyph. Corrected shared Triangle/Cross glyph colours to green/blue.
- No save-format changes or additional runtime assets.

## Verification
First two full runs passed all five smoke groups (smoke-20260927-110703-873 and smoke-20260927-110925-803). Native arrival, touchdown, departure, compass and tracking-panel captures inspected. Further checks cover guarded intermediate touchdown frames across every purchasable hull and Lave body, smooth-turn progression/convergence and moving/distant traffic. Close-up captures exposed mirrored wall signs; reversed wall tangents and rechecked all face orientations. Final results recorded in handoff.

## Safety and remaining work
Physical PSP shutdown remains unconfirmed: passing emulator checks does not establish a hardware fix. Retest landing without pressing buttons, then press/hold buttons during a repeat arrival, walk, board with Triangle, release and launch with R. Inspect close wall signs and sky traffic on hardware.
Keep landing-trace.txt and commander save if the PSP powers off; obtain PSP model/firmware and ship/body. Existing bounded checkpoint logging is retained.
All changes are local; preserve the repository's earlier uncommitted work. No publication, resets or save migration.
