# Station activity integration contract

Status: proposal backed by the offline manifest; runtime presentation remains
Station-owner gated.

## Input and identity

The authoring source is `tools/generate-station-activity.mjs`. Each record is
identified by `(system, hub)` where `hub` is `0` for the primary hub, `1` for
the outer relay, and `2` for the frontier outpost. The record seed is derived
from those IDs and is stable across revisits.

The runtime adapter must derive the same record from current system data or
consume the validated `planet.content`-style sidecar. It must not add a
commander-save field, use gameplay RNG, or mutate mission/reward state.

## Presentation fields

| Field | Allowed range | Intended use |
| --- | ---: | --- |
| `windows` | 4–13 | bounded station window/lamp activity |
| `traffic` | 2–10 | authored traffic/activity intensity, not NPC spawn authority |
| `security` | 1–7 | signage, patrol/readability treatment |
| `service` | 1–5 | authored service/room emphasis |
| `mood` | quiet/working/busy/crowded | text/art direction label only |

The first vertical slice should use `windows` and `mood` in presentation only.
Traffic, security, and service should remain available to station-room art and
dialogue after their gameplay implications are separately approved.

## Acceptance gate

1. The same `(system, hub)` produces identical fields after leave/re-enter.
2. Primary, relay, and outpost records remain distinct where authored.
3. No existing save bytes or commander migration version changes.
4. Station rendering remains within the native viewport and the existing frame
   budget; `traffic` must not silently increase NPC simulation counts.
5. Invalid/missing records fall back to current prosperity-based rendering.
6. Full game/input/steering/radio/performance smoke remains green, followed by
   Planetary traversal and Station native capture review.

Until this gate is accepted, the offline manifest and pipeline validator are
the authoritative implementation of station/activity variation.
