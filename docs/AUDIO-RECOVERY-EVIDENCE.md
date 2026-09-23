# Released audio: recovery evidence and physical PSP investigation

Audit base: `1e3fab88090e328916dc487f1c99031daf0a9061`, v2.5.48.
Released `audio.h`, `audio-sfx.h`, `radio-playlist.h` and `radio-tests.h` are
byte-equivalent to candidate c673fa3 by Git diff. Preserve all SFX, shuffle,
buffers, gains, volume settings and decoder/lifecycle behavior in this slice.

## Existing evidence gap

The released baseline's two final radio suspend checks assert true after calling suspend
preparation and stop/init/stop. They prove those calls returned on that run only.
They cannot detect failed channel reservation, thread startup failure, a stopped
worker, missing PCM submissions or decoder failure after restart. The regular
smoke harness copies only EBOOT and flags, not a custom MP3 library.

`audio_init` reserves a channel then starts a worker; `audio_prepare_suspend`
sets frozen/run flags; `audio_stop` waits at most 100 ms, can forcibly terminate
the worker and closes/releases decoder/file/channel state. The main recovery
path also restores screen, clock and controls. Tests below do not replace that
complete runtime path or physical PSP power callbacks.

## Implemented minimal observability interface

Team explicitly assigned these evidence-only hunks after the Systems coordination
request; Systems retains lifecycle/decoder/I/O policy ownership. Three lifetime,
aligned 32-bit volatile counters in `audio.h`, written by the
worker and read as individual snapshots by tests:

- Successful PCM block submissions: increment after a nonnegative
  `sceAudioOutputBlocking` result. The local SDK documents this result as queued
  samples; name/report it as a successful submission, never audible completion.
- Failed output submissions: increment on a negative result, preserving the
  existing stop/break behavior.
- Valid MP3 decode blocks: increment only for a positive-frame decoded block.

No reset while the worker runs; compare unsigned deltas across bounded tests.
No file logging, timing calls or per-sample counters. Budget: 12 bytes plus
alignment, a few operations per block, zero voices/buffers/allocations. This is
the complete production change in this slice. Reading these counters
is not an atomic combined-state snapshot or speaker/headphone capture.

Audio added test assertions in `radio-tests.h`: reserve/start state; two successful
submissions within 500 ms on startup and each restart; freeze/run flags after
prepare; released thread/channel/decoder/file/resource state after stop; stable
submission count during a stopped interval; no new output failures. Use bounded
polling with yields, and always stop the test worker before game tests continue.
Empty folders exercise generated fallback only. Decoder-count delta stays zero
there and must not be reported as MP3 recovery. A separately enabled, original
MP3 corpus must show both decode and submission progress after restart.

## Source-backed failure hypotheses, not diagnoses

| Hypothesis | Source mechanism | Distinguishing evidence |
|---|---|---|
| All-invalid library repeatedly opens files | `mp3_sample` can call selection on every output sample; bounded selection alone does not bound repeated calls | Count opens/selection attempts over a bounded interval; compare empty versus all-invalid library, menu responsiveness and block generation time |
| Storage/decode stalls exceed output headroom | One worker does synchronous stream fill, decode, resampling and synthesis before blocking submission; 2048 frames = 46.44 ms | Separate generation, file-read and decode durations from normal output blocking; correlate spikes with captured dropout |
| Full-scale music plus SFX clips | Custom PCM plus mono event sums before +/-30000 clamp | Clamp counts and original near-full-scale fixture, FX 0 versus 10; compare recordings and peaks |
| Track/rate transitions click | New track resets interpolation state while source-frame caller advances history; no dedicated EOF crossfade | Short alternating 32/44.1/48 kHz fixtures with annotated boundaries; inspect/capture transition, no codec change until reproduced |
| Suspend interrupts SDK/file operation | Flag change cannot undo an already executing blocking SDK/file call; stop may terminate after 100 ms | Log forced-stop count and outstanding operation in separately agreed diagnostics; test sleep during active stream read/decode on physical PSP |
| Decoder loops or fails to advance library as expected | Selection is driven by decode returning no frames, with no explicit loop-policy call in current code | Verify SDK/emulator behavior using two short known tracks; record track changes and decode results; do not infer EOF behavior from synth smoke |

Expanded counters/timing/fault injection above are follow-on proposals, not
authorization to change Systems-owned I/O, retry policy or lifecycle. Keep
diagnostics out of the per-sample mixer and avoid synchronous logs in the worker.

## Physical PSP crackle and recovery matrix

Record exact commit/EBOOT hash, PSP model/firmware, battery/AC, Memory Stick
brand/capacity/free space, headphone/speaker path, gain settings and fixture hash.
Use only original generated signals/music or user-authorized files kept outside
Git. WAV is unsupported by this release; do not label a WAV-only run an MP3 test.

1. Empty library, five synth stations: 20 minutes flight/menu/EVA/combat, including
   repeated cues, Radio OFF, FX 0 and full-volume combinations.
2. Original MP3 matrix: mono/stereo; 32, 44.1, 48 kHz; CBR and VBR. Include quiet
   passages, low-level tones and near-full-scale peaks. Run representative files
   at least 20 minutes each, logging audible issue timestamps and scenario.
3. Two short different-rate files: at least 20 transitions, station switches,
   mute/unmute and combat ducking. Distinguish intended tuning static from faults.
4. Empty, one valid, two valid, mixed valid/malformed, all malformed, truncated
   files and long filenames. Verify responsiveness and fallback. Stop any run
   that hangs; do not repeatedly abuse failing media to obtain a pass.
5. Twenty short sleep cycles, then multi-hour sleep mid-flight and mid-radio;
   cover active MP3, generated fallback, retune and combat. After each wake verify
   screen/input, correct levels/station and sustained new playback, not one chirp.
6. Capture output where feasible with event/time annotations. A listener records
   clicks/dropouts/distortion; PCM submission counters alone cannot detect them.

Acceptance: no unintended crackle/dropouts or hangs, responsive controls, stable
resource use, preserved preferences and sustained output after wake. Report each
unrun matrix row as NOT RUN. A green PPSSPP run means emulator lifecycle evidence
only. Current physical hardware and listening results: **NOT RUN**.

## Integration gate

Systems agrees instrumentation and low-level follow-up; Audio owns assertions,
audible acceptance and evidence wording. Lead must run the combined build/all-five
smoke, save checks, native display recovery and matched performance before
integration. Bar existing-cue contract remains unchanged. No release/version,
Game/save or production behavior changes are part of this evidence slice.

## Executed evidence

Baseline v2.5.48: `smoke-20260923-013511-596`. Final restored candidate:
`smoke-20260923-014115-128`. Both PSP builds and all five smoke groups pass;
both have 44 compiler warnings, 56.63 FPS average, 33.37 ms worst frame and
23 frames over 25 ms. Released SFX/shuffle helper files are unchanged.

Each of three candidate lifecycle cycles observed three successful block
submissions, zero decoded MP3 blocks and zero output errors. Reserve/start,
freeze flags, tracked cleanup state and stopped submission stability passed.
Cleanup checks verify tracked state, not SDK heap leak freedom. No MP3 fixture
was supplied, so every cycle explicitly reports MP3 recovery NOT VERIFIED.

Negative control `smoke-20260923-013959-306`: temporarily disabled only the
submission-counter increment in this isolated workspace. All three progress
assertions failed at their bounded deadlines, producing `RESULT 3 failures`;
the harness rejected radio checks. This tests sensitivity to absent evidence,
not an induced decoder or physical playback failure. The instrumentation was
restored before the final build; the experimental source is not committed.

Target object symbols confirm three 4-byte counters (12 bytes total). Final
full-game ELF text/data/BSS: 1269134 / 4580 / 276032 bytes. No extra audio voice,
buffer or allocation. Per-block integer increments add no per-sample work;
matched emulator frame metrics show no measured difference at their resolution.
No physical PSP CPU, stack high-water, Memory Stick latency or listening result
is claimed. Main display recovery is still covered only by existing smoke paths,
not by a new native-screen capture in this slice.
