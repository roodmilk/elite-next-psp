# 2.5.389 physical PSP launch hotfix

The user reported that 2.5.388 hard-powered-off their PSP-2000 running ARK;
the preceding build worked. Hardware confirmation of this replacement is pending.

## Memory finding and change

The 2.5.388 mapped executable occupied 16,341,480 bytes. Its fixed 7 MiB
heap and 1 MiB main stack left only 435,736 bytes of the original 24 MiB
user partition for firmware codec modules, audio threads and allocation overhead.
PPSSPP's emulated codec modules do not reproduce all hardware allocations.

The retail EBOOT now excludes the embedded automated regression/capture suite.
This removes code, diagnostic strings and large static test Game snapshots;
it does not remove missions, game assets or player-facing debug features.
The new mapped image occupies 11,694,656 bytes, leaving 5,082,560 bytes before
runtime codec/thread allocations. The PRX shrinks from 12,576,466 to 9,489,314
bytes. Actual emulator startup with the supplied intro MP3 retains 4,930,816
bytes of free kernel memory. This addresses a concrete hardware memory hazard;
the precise hardware failure point has not been directly observed.

Both optimized build scripts now separate retail and review output paths and
reject retail images that leave less than 3 MiB for firmware/runtime overhead.
The Windows gate also checks PRX staging plus mapped-image size against 24 MiB.

`boot-status.txt` beside EBOOT records main entry, display, intro, audio and
the first rendered frames, including free memory and stack headroom. It is
closed after startup; there is no continuous frame-loop disk logging.

## Preserved behavior

The forty staged contracts, 155 stages, V33 save compatibility, arcade races,
23-second Elite Exploration intro skip, one-second music fade and 48 kHz
Sol Rock MP3 support are unchanged. Commander saves, settings and personal
music are not included or replaced.

## Verification

The exact retail binary was exercised with isolated PSP-1000 and PSP-2000
emulator configurations, invalid-memory tolerance disabled, real supplied
MP3s, and native 480x272 intro, command deck, flight and live-dream captures.
The full QA variant passes a compiler syntax check. Gameplay algorithm files
are unchanged from the 2.5.388 regression-tested implementation; the original
917 focused passes belong to that earlier full-suite binary, not this retail
binary. The fifteen core and 165 input failures recorded there are not claimed
fixed. See the new release evidence for exact hotfix runs and hashes.

To build and run the full embedded suite separately:

    ./build.ps1 -EnableReview
    ./smoke-test.ps1 -Eboot outputs/review-build/EBOOT.PBP

Linux: `ELITE_ENABLE_REVIEW=1 ./build.sh`; its review EBOOT is also written
under outputs/review-build. Never distribute a review EBOOT to the physical PSP.

Install: replace only EBOOT.PBP, then retest on the PSP. If it still shuts down,
inspect boot-status.txt if created; no file suggests failure before main entry.
