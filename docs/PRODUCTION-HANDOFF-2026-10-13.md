Graphics vertical-slice review packet: outputs/lighting-native453/VERTICAL-SLICE-REVIEW.md. Recommended target: Diorama Gold appearance, preserving actual paint/engine identity. Static preview image grows 82,192 bytes and fails the existing original-PSP retail headroom guard by 76,076 bytes. Combined preview frame times remain 64–90 ms. Do not promote the diagnostic PBP. Native captures, implemented/staged distinctions and proposed subsystem boundaries are recorded in the packet. Released 2.5.452 is unchanged.

# Production handoff — 13 October 2026

Authoritative workspace: C:/Users/skarm/Documents/Codex/2026-09-14/let/outputs/arcelite-psp.

## Current playable checkpoint

Local retail 2.5.452: outputs/releases/2.5.452-audio-hotfix/ELITE-NEXT-PSP/EBOOT.PBP, also copied to dist/ELITE-NEXT-PSP/EBOOT.PBP. SHA256 ABF212BD1D7AF7140D467300FF3C701A110442F0A1BC1BA9055C6A6D44A9ED05. Original-32-MiB-model emulator startup/MP3/ERL boot and MP3/OGG/generated-radio buffer-continuity checks pass. Physical PSP pending. Close-station and OGG CPU load remain open; this is not a complete ERL handoff acceptance or all-green global regression claim.

## Safe boundary / active work

Current HD-2D and spacecraft exploration is isolated in outputs/lighting-native453. Canonical src/main.c, src/audio.h, src/mega-city-render.h and src/voyage.h match the frozen 2.5.452 focused snapshot. No new EBOOT was promoted. Preview source includes experimental ship surface modules/outlets, sun shading, additive trails, soft glow, actual PSP-GU filtered glow, and costly CPU colour grades. Captures/evidence/limitations are documented in the preview README. Individual ship redesign, wireframe views, custom Mega hulls and full all-hull validation are unfinished.

Canonical files touched this preview run: CLAUDE-HANDOFF.md and this handoff document only. Experimental edits are in the isolated copy's src/main.c, flight-lighting.h, flight-extras.h, mega-city-render.h, lighting-lab-material.h, lighting-lab-capture.h, lighting-gu-lab.h, ship-style-lab.h and ship-trails-lab.h. Copied build.ps1 produces explicitly 64 MiB diagnostic packages and skips the retail guard; never promote it as a retail build script.

## Next integration risks

- CPU grading costs 22–26 ms; complete staged scenes are 64–90 ms before audio/display. Preview appearance is not PSP frame-pacing acceptance.
- GPU glow capture stages CPU framebuffer into VRAM and reads back. Benchmark a production VRAM-resident path before claiming speedup.
- Ship kit is procedural and distance-limited; original mesh arrays/collision envelopes and wireframe views are not redesigned. Validate every hull, outlet and viewing angle. The close Cobra survey is clipped.
- Preserve the five-block audio queue, device/decoder priority boundaries, suspend handling and memory guard from 2.5.452. Use real device-buffer starvation metrics, not SFX command-drop counts.
- Shared repository has extensive unfinished changes. Previous pull/rebase was blocked by pre-existing untracked music placeholders; autostash was restored. No new GitHub release/tag/clean-tree claim.

## Proposed ownership and interfaces (coordination proposal, not assignments)

Integrator: canonical src/main.c, src/game.c, game.h/save migrations, build scripts, VERSION/dist and final release checks. R&D: isolated render/model/effect prototypes and source patches with native evidence and costs; no direct master replacement. Audio specialist: scoped audio/radio files and device/codec timing evidence. Agree exact file ownership before overlapping implementation; the studio-lead chat coordinates assignments.

Gameplay remains authoritative for poses, ship/paint, velocity, boost, damage, race state and rewards. Presentation should consume a read-only snapshot and authoritative engine-outlet metadata; it must not change physics, payouts or saves. Audio should consume the existing quantised audio_flight_update/audio_traffic_update/audio_approach_update boundary; no per-frame allocations, file I/O or locks on the gameplay thread. These separation recommendations are not yet a new snapshot API implementation.

## Best independent gameplay work

A self-contained ERL event-rule/scenario regression harness under tools/, consuming src/erl-arcade-rules.h read-only initially, can validate pursuit/escape, objectives, item constraints and deterministic scoring without touching the renderer or audio. Proposed rule changes should be a separate approved handoff. Keep game.c/main.c/save schema integration with the integrator. This is a recommendation, not a redirected chat or new assignment.

## Gameplay & Systems: non-overlapping faction/living-galaxy boundary

Proposed owner-private files (new, not yet implemented): src/galaxy-sim-state.h, src/galaxy-sim-rules.h, src/faction-sim-rules.h, tools/review-galaxy-sim.c and docs/GALAXY-SIM-HANDOFF.md. Start with deterministic pure domain rules and a standalone harness; no rendering, SDK/audio calls, assets or per-frame allocation. Do not create a second authoritative economy/faction source.

Read-only inputs: current WorldState from src/world-state.h, existing faction IDs, a bounded game-clock delta and an explicit deterministic seed. Keep prototype state in an owner-private GalaxySimState, separate from Game serialization until a save design is approved. Output a bounded GalaxyEvent/command list containing system/faction IDs and domain effects (e.g. influence delta, encounter proposal, trade-pressure signal). No pixels, UI strings, audio commands or live NPC-array mutation from the rule module.

Integration-owned files remain src/game.c, src/game.h, src/world-state.h, src/sectors.h and all save-integrity/migration helpers. Proposed changes to these arrive as a reviewed integration patch. Existing src/traffic-routes.h, travellers.h/data, mission logic and social-save.h are reference consumers at first; do not take ownership or edit them concurrently without a file agreement. src/faction-dossier.h, social-feed.h, dialogue/UI and presentation hooks stay outside the domain developer's initial boundary.

Required tests before integration: deterministic reload/travel results; bounded work/event capacity; existing faction ID/reputation consistency; NPC and station availability limits; no duplicated payments/rewards; no save/schema change in the first domain prototype. Run simulation on a bounded low-frequency tick, with explicit catch-up limits; no unbounded whole-galaxy work in the render loop. Any economic rewards, faction renumbering, persisted fields or changed mission behavior require an approved design and integrator-owned migration/tests.

Rendering reads authoritative actor poses/state after gameplay applies approved events. Audio receives quantised existing signals or approved event cues through its current boundary. Neither specialist writes galaxy simulation state. This gives faction/living-galaxy development an independent module/test surface while graphics work continues in its isolated renderer copy. It is a boundary proposal for studio-lead coordination, not a dispatched assignment.
