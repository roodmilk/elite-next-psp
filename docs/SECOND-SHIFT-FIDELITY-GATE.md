# Second Shift fidelity gate

**Status:** mandatory visual acceptance contract  
**Reference:** the attached six-bar sheet and the approved Second Shift native room benchmark.  
**Scope:** every station room and station-adjacent screen, including procedural variants.

The reference image is a visual target only. It does not override gameplay, Story, Systems, Station or PSP constraints. No procedural room is considered complete because it merely has a background, a few rectangles, or a valid selector.

## Native composition requirements

At 480×272 and 1× nearest-neighbour review, every room must contain:

1. A readable foreground/midground/background depth stack.
2. One hero landmark that identifies the room in under one second.
3. Three to five readable interactive anchors, aligned with actual hotspots.
4. At least two authored-looking people, silhouettes, or activity cues when the room’s Station/Gameplay state allows inhabitants.
5. Material clusters: ribs, trim, counters, racks, panels, paper, glass, tools, crates, bottles, samples or equivalent family-appropriate detail.
6. A deliberate lighting key and two or more controlled light accents; no random noise field.
7. A route/door/hatch read and a clear SHIP return area.
8. No art entering the options panel, feedback band, speech box or reserved controls.

## Fidelity parity rules

- A procedural room must look authored at a glance; family variation comes from massing, hero landmark, prop motifs, lighting and wear, not a palette tint alone.
- Reusable pieces must have readable silhouettes, visible depth/shadow treatment and native-scale identity. Placeholder tokens do not pass.
- People need a readable head, torso, pose and role cue at 1×. A generic block is not an NPC.
- Windows and body views must use the authoritative body/window policy; they cannot invent gameplay targets.
- Normal and high-contrast variants must preserve the hero landmark, route, people and interaction silhouettes.
- The room must remain truthful: art cannot imply gambling, repairs, missions, faction presence, danger, stock or services that Gameplay/Station has not enabled.
- All text, speech, choices, unavailable states and results must pass Story & Dialogue’s native wrapping and capability checks.

## Procedural quality budget

The composer must expose bounded counts for structural modules, hero modules, actors, activity props, lights and wear clusters. A family recipe fails if it spends its budget on decoration while losing the landmark, route, people or activity read. Static element bytes, plan bytes, draw calls, temporary memory and native captures are recorded per representative room.

## Required vertical slice

Before broad activation, Freight, Prospector and Research each need H0/H1/H2 rooms with:

- normal and high-contrast 480×272 captures;
- enter/disembark, walk/explore and SHIP return;
- NPC speech and correctly wrapped choices;
- one capability-backed useful activity;
- one truthful unavailable state;
- no duplicate authored people, doors or hatches;
- measured budget and deterministic repeat capture;
- visual review against the attached reference and Second Shift benchmark.

The generic fallback remains valid elsewhere until a family passes this gate. The goal is 5,376 rooms that feel like members of the same professional game—not 5,376 technically valid but visibly sparse placeholders.
