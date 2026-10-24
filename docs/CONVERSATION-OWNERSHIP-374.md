# Conversation ownership — 2.5.374

Discovery contacts previously replaced the visible speech without replacing the
shared encounter kind. Contact Target then treated all speech as an actionable
encounter, showing replies from a different pilot and applying their consequences.

Ordinary speech now clears action ownership. Only explicitly authored encounter
transmissions bind to an encounter kind, caller and system. Quick Reply checks
that binding. The paused panel snapshots its speaker, portrait seed, speech and
encounter context, and checks the transmission serial, caller, kind, system and
liveness before accepting a choice. Replaced or expired channels close without
acting on or dismissing another transmission. World spawning clears old bindings.

Outgoing discovery, station and ordinary pilot channels show their actual speaker
and a close-channel response. They cannot inherit assistance, cargo, bounty,
passenger, pirate or law consequences. Quiet Watch also verifies its own source
and text immediately, rather than waiting for its next simulation tick.

Promising convoy assistance acknowledges the promise. It no longer awards 350
units or announces a rescue before any rescue has occurred. This correction does
not implement a new ambient convoy completion/reward system. Mission-board
escort rewards are unchanged.

The new regression matrix checks every one of 75 catalogue speakers against six
actionable encounter types, and all 32 placed discoveries across 14 background
encounter types using actual Contact Target and X-close input. It checks unchanged
credits, cargo, passengers, combat and unrelated encounter state; changed voices,
callers, encounter kinds, systems, expiry and dead callers; legitimate pirate
clarification/refusal; Quiet Watch identity and world resets. Native 480×272
sanctuary screenshots are inspected. Existing Comms, startup, Codex and Quiet
Watch checks run on the same candidate. Exact results and hash: BUILD-2.5.374.txt.

Ownership fields are transient. Commander save version, appearance, discoveries,
module inventory and rewards remain compatible. No new art or music is bundled.
Physical PSP testing remains pending; inherited broad failures are not resolved.
Git pull remains blocked by existing untracked music placeholders; autostash was
restored. Mixed work preserved; this release is local and GitHub is not updated.
Highest-priority work: hardware playtest, inherited reliability, planetary work.
