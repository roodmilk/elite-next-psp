# Audio audit and acceptance — 23 September 2026

Source baseline: `ac8a5810037dec52c26bbe962973f7af49764a40` (main, 2.5.47).
Source observations below are not physical PSP listening results.

## Implemented behavior and gaps

- `src/radio-files.h`: five named folders beside EBOOT, startup/resume scan,
  at most 24 paths per station, 160 bytes per path. MP3 only; WAV is not
  implemented. No live rescan control. Long paths can truncate; directory entries
  are selected by suffix without a regular-file check. Systems owns these I/O fixes.
- `src/audio.h:mp3_shuffle_track`: random sampling with replacement can revisit
  a failed candidate and miss a playable alternative. Avoids the current slot
  only within the current selection call. Station switching loses that history.
  No shuffle bag, no persisted per-station last track.
  Concrete baseline reproduction: seed `0x7141`, count 3, previous slot 0
  selects slot 1 on all three attempts; if 1 is malformed and 2 playable,
  slot 2 is missed. The candidate regression models exactly this library.
- `mp3_sample` is called per output sample. If all files fail to open, selection
  may repeat for each sample, making fallback expensive. Files that open but
  fail decoding need a separate failure policy. Neither is measured as the cause
  of reported hardware crackle; Systems must own bounded retry/cooldown changes.
- `audio_worker`: one stereo music source, one replaceable mono SFX voice,
  single pending SFX mailbox. Events can overwrite warnings; no voice priority.
  Generated fallback has seven music voices and filtered stereo delay
  (`src/radio-synth.h`), plus authored nonverbal Far Horizons chatter.
  This is procedural sound, not recorded spoken dialogue.
- Mix defaults music 5/10, FX 8/10; output volume is one third of PSP maximum.
  Music gain ramps one unit per sample (full range ~22.7 ms), ducking targets
  45% in combat/alert and 75% for dialogue/police. Final clamp is +/-30000.
  Loud custom MP3 plus SFX can clamp. Raising loudness needs measured peak/RMS
  and physical speaker/headphone approval, not a blind gain increase.
- SFX lengths are samples, not milliseconds: selection 140 (~3.2 ms), boost
  500 (~11.3 ms), warning 1400 (~31.7 ms). No persistent engine or biome bed.
  Station retune static counter is decremented per 2048-frame block despite
  its `_ms` name. Track transitions do not have a dedicated crossfade.
- `audio_prepare_suspend` freezes worker flags; `audio_stop` waits up to 100 ms
  then can terminate the thread before resource teardown. `main.c` rebuilds
  display/input/audio after wake. This is not proof of safe interrupted SDK I/O.
- `src/radio-tests.h` tests synth bounds/determinism/distinctness and preferences.
  Two suspend checks use unconditional true assertions after calls: evidence of
  reaching those points, not decoder recovery, audible output or power-cycle tests.
- `src/radio-ui.h` shows FOLDER MP3 from detected count, not actual playback.
  README's historical filename/count claim no longer matches this screen.

## Prioritized acceptance matrix

| Priority / feature | Automated / PPSSPP acceptance | Physical PSP acceptance | Owner / status |
|---|---|---|---|
| P0 crackle | Original fixtures: mono/stereo, 32/44.1/48 kHz, CBR/VBR; collect decode/read/block timings, failure and clamp counters during flight/combat/menu | At least 20 minutes per representative format, speakers and headphones, Memory Stick details; no unintended crackle/dropout | Systems runtime + Audio listening; unverified |
| P0 sleep recovery | Repeated stop/start with active MP3; verify output progress and resource reuse, malformed and missing media | 20 short cycles plus multi-hour sleep, during decode/retune/combat; screen/input/music return without hangs or stale SFX | Systems; unverified |
| P1 bounded shuffle | Every candidate attempted at most once per selection; avoid previous when an alternative opens; previous allowed only as last fallback; zero/one/24 tracks | Mixed good/bad folder reaches good audio promptly, no repeated stall | Audio policy + agreed Systems adapter |
| P1 corrupt library | Empty, all invalid, truncated, deleted, long filename, directory named .mp3; bounded retries and truthful fallback | Responsive menus and stable fallback with slow/failing storage | Systems + Audio; retry budget not yet implemented |
| P1 library workflow | Five folders; startup rescan without rebuild; WAV explicit supported/unsupported UI; future safe rescan request | Add/remove files, reboot and recover; no user assets bundled | Systems I/O + UI; WAV/live rescan deferred |
| P1 balance / silence | Peak/RMS/clamp counts at 0/5/10; music OFF leaves effects; Quiet Comms suppresses talk; priority tests | Music spacious and audible; warning/dialogue cues intelligible, no speaker distortion | Audio; listening pending |
| P2 transitions / variety | No immediate repeat across station returns; bounded fade and fallback transition; long synth renders | No click on tune/EOF/mute; 30-minute variety review | Audio, future slice |
| P2 EVA | Distance-based grounded steps; one jet start per hold; stop on board/death/pause/menu; survey result cues | Restrained, distinguishable feedback without masking warnings | Planetary + Audio; proposal only |

No PPSSPP pass certifies PSP storage latency, decoder timing, power recovery or
subjective sound quality. Record hardware model, firmware, storage, format,
scenario, duration and listener result for every physical claim.

## Budget evidence and implementation boundary

Static source budget: encoded MP3 buffer 65,536 bytes; decoder PCM 9,216;
worker stereo output 8,192 bytes inside a 16,384-byte thread stack; track paths
19,200 bytes; synth delay arrays 32,768 bytes plus voice/control state.
These named allocations total 143,104 bytes including the full worker stack
(do not count its output array twice), before SDK decoder memory, FILE buffers,
alignment and small globals. This is not a runtime heap/stack high-water reading.
2048/44100 = 46.44 ms output block duration. No measured decoder CPU or underrun
counter exists in this baseline; average rendering FPS cannot substitute for it.

First slice proposes a bounded candidate order helper and tests; the only runtime
integration is the agreed `mp3_shuffle_track` hunk. No buffer, gain, decoder,
thread, suspend, main.c, Game or save changes. Release/version remains Lead-owned.

Direct user follow-up also requests applying the needed SFX/music improvements.
The first audible change is `audio-sfx.h`: original integer synthesis, one existing
mono event voice, 20–300 ms cues, 128-sample attack and zero endpoints. Navigation
is quiet/short; scan rises; docking uses a two-note confirmation; boost is a low
swell; warning is two pulses. Music's existing stereo space and five identities
remain intact. This is not an engine loop, voice queue or ambient mixer.

The maximum mathematical SFX magnitude is 5,376 (death: 256*10 + 128*22),
before its <=1 envelope. Generated music clamps at 18,000, so the sum is at most
23,376 even at full Music/FX without ducking. Custom MP3 can reach full PCM scale,
so this guarantee does not apply to custom music; retain a separate clipping test.
No gain is raised. These cues still share the existing single-slot mailbox;
longer events can be interrupted. Warning priority is a separate required slice.

Helper validation before runtime integration: all-five PPSSPP smoke passed;
all 15 cues were rendered in 108,486 samples, measured maximum 5,172. Four-second
renders for each station with rotating full-volume effects measured peaks
9,768 / 10,831 / 9,209 / 9,987 / 11,161 and zero clamps. A pure SFX render took
32,671 emulator microseconds (not hardware CPU evidence). Baseline average was
57.99 FPS, helper run 57.98 FPS; both worst 33.37 ms. Final runtime validation
must be recorded separately. No physical listening has been performed.

The optional `audio-preview.flag` alongside `smoke.flag` exports a mono raw
full-FX audition, one event per second: UI, laser, hit, warp, scan, land, mine,
boost, comm, dock, missile, alert, death, select, talk. It omits PSP master gain
and represents synthesized assets, not a capture of PSP audio output.

## Planetary integration proposal (deferred)

After Lead integrates traversal, coordinate minimal hooks with Planetary:
distance accumulation after accepted grounded displacement in EVA update;
jet hold edges from dedicated EVA dispatch; successful `eva_toggle` return for
airlock confirmation; actual scan return/result for success versus rejection.
Keep accumulator/voice state outside Game/save. Existing BOOST must be replaced
or reused, never layered twice. Current one-voice mailbox cannot support a
sustained jet/ambient layer with warning priority; defer those until mixer policy
is agreed. First useful follow-on: success-only boarding cue using the existing
voice, with far-away rejection preserving its current feedback. Validate board,
death, menu, pause and scan spam jointly against integrated traversal.

## Final candidate evidence and integration gate

Team explicitly assigned helper includes, `mp3_shuffle_track` selection and
`audio_worker` SFX duration/render hunks after checking Systems had no conflicting
audio edits. Runtime now calls both helpers. The obsolete SFX oscillator helper
and per-sample phase increment were removed as part of that render replacement.
No decoder, file I/O, retry frequency, start/stop/channel or suspend logic changed.

Windows PSP toolchain build succeeded with 44 warnings, identical warning count
to baseline. Final PPSSPP run `smoke-20260923-011212-088` passed game, input,
steering, radio and performance (`RESULT 0 failures` each). Average 57.98 FPS,
worst 33.37 ms, 13 frames over 25 ms; baseline 57.99 / 33.37 / 13. SFX synthesis
measured 108,486 samples in 35,129 emulator microseconds. Audio results match the
sample peaks above. Existing input tests passed independent music/FX levels,
Radio OFF retaining effects and Quiet Comms toggling. Unchanged main.c dispatch
was inspected: Quiet Comms excludes only COMM/TALK and consumes each game cue
once. This is not a physical output-capture test or simultaneous-event guarantee.

Reported target `sizeof`: stream 65,536, PCM 9,216, RadioSynth 32,992, paths
19,200, output 8,192 within 16,384 stack. Named buffers + full stack total
143,328 bytes including the synth's 224 bytes of voice/control state beyond its
delay arrays. No new persistent audio buffers/voices.
Candidate-order scratch is 24 ints / 96 bytes when selecting, plus call-frame
overhead; no measured physical stack high-water. ELF: text 1,240,890, data 4,580,
BSS 276,020 bytes including the full game and test code, not audio alone.

Pure policy tests cover counts 0–24, every previous index, 32 seeded trials each,
invalid bounds, and the concrete malformed-slot model. They do not decode an
actual malformed MP3. Empty-library full game smoke exercises synthesized music;
custom codec corpus, output deadlines, PSP CPU/memory and listening remain open.

Partner contracts: UI preserves independent FX and Quiet Comms behavior; Gameplay
and Audio agree existing successful survey events emit SCAN, isolated rejected or
repeated scans do not, with prior cue cleared between direct tests. Gameplay owns
the mineral/refinery/full-hold assertions. Planetary gets the same 180 ms SCAN
and 160 ms BOOST identities through its existing events; no new event hooks.
Lead must test the combined traversal/gameplay/art build, native screens,
performance and save compatibility before claiming cross-branch compatibility.
No main merge, version change, packaged EBOOT, tag or release was produced.
