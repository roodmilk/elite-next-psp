# First bar: audio contract proposal

23 September 2026. Documentation only; runtime hooks are not assigned yet.
Canonical main checked: `ac8a5810037dec52c26bbe962973f7af49764a40`.
Audio candidate dependency: `c673fa37955266baba4638385785997c072aeb7b`, not yet
an ancestor of main. Preserve that candidate; do not cherry-pick its sounds twice.
Brief reviewed: Ideas Manager's `outputs/STATION-BARS-BRIEF.md`, bars-first.
Station has agreed first-slice reuse of UI/SELECT/SCAN only, zero ambience assets
and no room loop. Proposed location is Reorte (system 39), primary hub variant 0,
`SC_R_CANTEEN`, while docked in the station WALK view. Location remains Station's
design decision. ENTER/EXIT/PAUSE are presentation lifecycle events; job and dice
outcomes come only from Gameplay's authoritative success results.
Station owns room entry/state; Gameplay owns settled results; Systems owns audio
lifecycle; UI owns interaction presentation. Lead owns combined integration.

## Sound direction and first delivery boundary

Warm, quiet refuge: soft mechanical room tone, an occasional dry cup/table sound,
and concise action feedback. Keep the commander's chosen radio station; never
retune, unmute, restart or replace their music on entering a bar. No new soundtrack
is promised. Existing original stereo music can supply the musical atmosphere.

Current `audio.h` has one stereo music source and one replaceable mono SFX voice,
with no ambient bus or event queue. A looping ventilation bed on that SFX voice
would compete with interaction/warning cues. Therefore **no continuous room loop
in the first hook integration**. The agreed delivery has only existing event
sounds and silence between them. Optional original room-detail one-shots and a bed
require a separately agreed scheduler and lifecycle contract; do not simulate
them by writing a cue every frame.

## Event and lifecycle contract

| Event | Proposed audible result | Trigger / guard |
|---|---|---|
| Successful room entry | Keep existing SELECT door tick | Once on an actual transition into an eligible open bar, never from rendering/reopening text. |
| Navigation / open dialogue | Existing SELECT / UI | Use current Station action cue; never duplicate it in Audio. |
| Confirm dice stake | Existing UI only if no result cue occurs in the same action | Only after authoritative stake/outcome commit succeeds; one event, not one per die/frame. |
| Dice result | Existing UI, identical neutral cue for win/loss/draw | Only first settled-result presentation; visual copy carries outcome. No celebratory reward sound for losses; no replay on reopen/load. |
| Job accept or rumour pinned | Existing UI | Only successful authoritative action; full log/repeat/no target does not emit success. |
| Job paid | Existing SCAN confirmation, if Gameplay has no existing payment cue | After payment changes unpaid to paid, exactly once. Existing cue wins; do not overwrite it. |
| Invalid action | Existing rejection feedback or silence | No success cue; no new buzzer spam on held Cross. |
| Exit / board / death / room change | Cancel pending optional room detail; do not change radio | Existing short action cue may finish its <=300 ms tail. No ambient sound is allowed to persist as a loop. |
| Pause / menu overlay | Suppress scheduling optional room details; discard overdue detail | Existing radio/pause behavior unchanged. Current short cue may finish; immediate cancellation would require Systems-owned lifecycle changes. |
| Resume / restore / re-enter | Fresh optional-detail cooldown, no event replay | Never reconstruct dice/payment audio from saved result alone. No backlog or catch-up burst. |

Quiet Comms continues suppressing COMM/TALK only; dice, job and navigation cues
remain effects. Optional nonverbal room detail follows FX volume, but is suppressed
while dialogue/critical warnings are active. Radio OFF mutes music only and never
unmutes itself. FX 0 means no bar effects. No spoken recordings or crowd chatter
are introduced. No new Game/save fields are needed for transient audio bookkeeping.

## Proposed asset, voice and cost budget

Agreed first slice: **zero new asset bytes, zero audio state bytes, zero added
voices/channels, no new synth work**. Reuses existing UI/SELECT/SCAN; effects last
40/20/180 ms respectively in candidate c673fa3. The budget below is a future
optional soundscape ceiling, not first-slice implementation or a measured claim.

- Recorded assets: zero bytes. Any optional cup/dice cue is original integer
  synthesis in the Audio helper, not downloaded library sound.
- Additional voices/channels: zero. No new delay/PCM/encoded buffers or allocation.
- Optional room-detail peak <=600, dice peak <=1200; 3–5 ms attack/release and
  zero endpoints. Existing event levels remain the tested audio candidate values.
- New transient audio-only state cap: 32 bytes (room identity, cooldown and flags),
  subject to Systems review. No ambient state stored in saves.
- One optional room detail after entry, no repeating timer for this first slice;
  skip it on any pending action, dialogue, pause, exit or warning. No queue.
- Prospective added synth cost target: <=0.25 ms per active 2048-frame block on
  physical PSP. This is an acceptance target, not a measurement.

Existing candidate measured SFX peak 5172; full-volume five-station mix sample
peak 11161, zero clamps, analytic generated-music+existing-SFX bound 23376.
Optional room/dice replace the current event voice, not add to it: respective
generated-music bounds would be 18600/19200. Custom full-scale MP3 remains outside
that guarantee. Candidate named audio buffers+stack total 143328 bytes; no bar
runtime memory/CPU evidence exists until implemented and tested.

## Exact integration ownership to agree before code

Station proposes the successful bar-entry/exit and accepted-action points in its
current room implementation (baseline `src/station-crawl.h:sc_apply`
changes `sc_room`, while `sc_board_ship` returns to HOME). Current names/locations
must be checked after Station's bars branch exists; these are not reserved hunks.
Gameplay supplies committed dice and paid-job event results, not button intent.
Audio owns optional procedural waveform/helper and tests. New cue IDs, if needed,
require agreement on `src/game.h` enum additions; no change is requested yet.
Systems must agree any nonblocking admission/cooldown adapter and pause/exit
signals before Audio edits worker/shared dispatch. Do not add decoder/thread work.

For the agreed existing-cue slice, no Audio adapter or lifecycle hook is needed:
Station keeps room-entry/exit cues in `sc_apply`/`sc_board_ship`, and maps its new
authoritative result callbacks once. Rendering emits no cues. If one action
commits and displays a result immediately, emit only the result cue; the current
single mailbox cannot deliver two queued cues. A RECOVERED event on the planet
belongs to Planetary/Gameplay; do not replay it merely on returning to the bar.
Changing a menu or pausing requires no new cleanup because this slice adds no loop
or delayed audio. Strict instantaneous cancellation of an existing one-shot is
outside the agreed contract; its short tail may finish.

If exact hooks or audio integration are not ready, ship the Station slice with
existing UI/SELECT/SCAN events and explicit absence of new room ambience. Do not
hold gameplay correctness hostage to a room bed.

## Combined acceptance

Enter/exit ten times: no accumulation, radio selection/levels untouched. Zero
credits and rejected stake: no dice-success cue. Hold Cross during reveal: one
committed roll and one settled-result cue. Reload/reopen a paid job or result:
no second payment cue. Test win/loss/draw separately against actual settlement.
Quiet Comms, Radio OFF and FX 0 behave independently as above. Pause/exit/death
before an optional detail discards it; resume never flushes delayed events.
Warnings/dialogue suppress optional details. Test mono effects with stereo radio,
full-volume peaks/clamps and matched performance; measure actual PSP CPU/stack
before claiming the proposed budget. All-five combined smoke, native bar screens
and save/settlement tests remain Lead's gate. Physical listening remains separate.
