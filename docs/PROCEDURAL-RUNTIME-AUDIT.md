# Procedural runtime compatibility audit

Audit target: Art runtime `specialist/station-art-rollout` commit `4a4a9ab`.
This is a read-only audit; Systems did not merge or modify the Art branch.

## Compatible boundary

- `ProcRoomIdentity` uses the intended compact identity shape and
  `PROC_ROOM_PLAN_MAX_ELEMENTS` is bounded at 12.
- The native screen, room viewport, options panel, feedback band and SHIP
  rectangle match the Systems/Art1 contract.
- The plan API has an explicit version and unsupported versions return fallback.

## Activation blockers

1. `sc_proc_plan()` recomputes family, arrangement, landmark and material from
   live `System` fields instead of consuming the Systems `room-descriptors.json`
   fields. That can disagree with the canonical register and offline bake.
2. The Art hash uses `proc_room_mix`, while Systems descriptors use FNV-1a over
   the canonical selector tuple. Repeated identity parity is therefore not
   demonstrated across the two implementations.
3. `stable_seed` is an additional selector input not present in the Systems
   descriptor contract and is not versioned in the room fingerprint.
4. Runtime construction forces `exception_id=0`, so the two authored Reorte H0
   exceptions are not preserved by this path.
5. The runtime plan does not enforce the measured static asset, plan, temporary
   memory, draw-call or frame-time budgets. The 12-element count is only a
   structural bound, not a PSP performance certification.

## Required compatibility handoff

Before activation, Art should either consume the Systems descriptor directly or
provide an exact C-parity adapter for its fields and fingerprint. The adapter
must preserve authored exception IDs, reject unsupported selector versions,
retain the generic renderer fallback, and emit a measured native normal/
high-contrast capture plus plan/draw/temp/frame budget report. No gameplay,
save, economy, mission, NPC or capability writes are permitted.

Until those gates pass, the Systems-generated descriptors remain build-time
planning artifacts and the existing truthful station renderer remains the
authoritative fallback.

Follow-up Art commits `48871ba` and `d0c2cd4` address the selector/hash,
exception, draw-bound and descriptor-source gaps. Re-audit evidence: the
generated header embeds the SHA-256 of the Systems descriptor JSON, lookup is
bounded to 256×3×7 records, and the live adapter consumes the baked family,
layout, landmark, material, art-version, exception and selector-hash fields.
The plan rejects mismatched hashes and unsupported selector versions. Systems
treats `art_version` as a compatibility field, not a fingerprint input, and
keeps `selector_version` as the version gate.

Remaining acceptance evidence is native normal/high-contrast capture and
measured plan/temporary-memory/draw/frame cost; this audit does not claim those
are complete, and it does not merge or release the Art branch.

The Reusable Element Kit extraction audit is intentionally a separate gate:
`tools/validate-family-extraction.mjs` refuses certification while required
Prospector/Research anchors are missing or have the wrong native dimensions.

The three-family proof gate is `tools/validate-station-proof-slice.mjs`. Run it
with the descriptor bake and a proof manifest containing 63 samples:

```text
node tools/validate-station-proof-slice.mjs --descriptors work/planet-preview/room-descriptors.json --proof work/planet-preview/station-proof-slice.json --out work/planet-preview/station-proof-report.json
```

It covers Freight, Prospector and Research at H0/H1/H2 across all seven rooms,
and refuses certification until selector parity, native normal/contrast captures,
dialogue truth, save/economy safety, duplicate-anchor checks and numeric static/
plan/draw/temp/frame measurements are present.
