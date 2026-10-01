# Battle music and pursuit verification

The installed Tremor PSP library produces corrupted PCM for the user's
spacebattle.ogg: standard deviation 26,913, about 65% of samples at full scale,
and correlation -0.00455 against the reference decoder. The problem exists
before the mixer or sound device. Earlier buffering changes could not repair it.

The replacement libvorbisfile decoder was run on all four actual user tracks
in silent PSP executables under PPSSPP. No sound device was opened. The same
game sample/resampling function produced eight seconds of output per track:

| Track | Source rate | Correlation vs reference | Clipped samples |
|---|---:|---:|---:|
| spacebattle | 44100 | 1.000000 | 0 |
| spacebattle2 | 48000 | 0.999941 | 0 |
| spacebattle3 | 48000 | 0.999944 | 0 |
| spacebattle4 | 48000 | 0.999946 | 0 |

The reference comparison uses linear resampling; the game uses cubic
interpolation, so small differences at 48 kHz are expected. The old truncated
Q16 phase increment introduced cumulative timing error and has been replaced
by an exact rational sample-rate accumulator.

Reproduction tools: tools/audio-decoder-probe.c, tools/build-audio-probes.ps1,
tools/run-audio-probes.ps1 and tools/analyze-audio-probes.py. Player recordings
remain ignored by Git. Probe outputs are outside the source checkout.

Encounter runtime checks passed rewards, outcomes, protected introduction,
visible fire, continuous stars, bounded formations and a 45-enemy full clear.
The broader game suite reports a station-tunnelling collision failure; that
system was not changed here. Physical PSP audio deadlines and extended combat
frame rate are not established by these emulator tests.
