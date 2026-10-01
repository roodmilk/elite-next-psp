# Deep freighter polish — local candidate, 30 September 2026

## Player-visible result

The giant ships crossing the distant sky are now a true background layer.
Their hull polygons and engine effects are flushed before planets are painted,
so a planet always covers any freighter or exhaust behind it. The centre-ray
planet test is also retained as an inexpensive early rejection for fully hidden
ships.

Background freighters no longer reuse two enlarged fighter meshes. Four fixed-
cost modular silhouettes provide container trains, tankers, broad-wing carriers
and tall ark-like ships. Each system/lane chooses from six coordinated hull and
engine palettes. Their dimensions are roughly twice the first modular pass so
the designs remain legible at their 68–86 km presentation distance.

Twin engines produce long tapered exhaust: a bright coloured root, saturated
middle, dim outer wake, small soft joints and a slight slow curl. This is drawn
as bounded software-framebuffer geometry with no textures, allocation or new
audio. A complete crossing now lasts 150–230 seconds, reducing distracting
background motion and making the ships feel massive.

This changes set-dressing traffic only. Reachable capital freighter collision,
health, schedules, cargo, targeting and movement are unchanged. Controls, saves,
stations, planets and music are unchanged.

## Rendering order

The normal flight order is now:

1. background, continuous sky and stars;
2. deep freighter hulls, an immediate polygon flush, then deep exhaust;
3. rear rings, planetary rims/discs and front rings;
4. stations, reachable traffic, debris, rifts and cockpit effects.

That order handles partial overlap as well as a ship whose centre is behind a
planet. It avoids adding a second full-screen depth buffer to the PSP renderer.

## Review path

`open-deep-freighter.flag` is a diagnostic-only entry point. It accepts system,
lane and an occlusion-test toggle, then frames that lane away from the targeting
reticle. `tools/review-deep-freighters.ps1 -Eboot <candidate>` runs four clear
views and one deliberately planet-blocked view in isolated PPSSPP directories,
exports native 480x272 PNGs and stops only the emulator processes it launched.
These flags are not included beside the player EBOOT.

## Tested candidate

Candidate: `outputs/chart-build-20260930-012536-359/EBOOT.PBP` (v2.5.232).
SHA256: `4733A9C26DF06DCF72EB75B7FAF9FF0EC1615E61394F7E4250391D8039C776C1`.

Native review: `outputs/deep-freighter-review-20260930-012652-671/`.
Clear cyan/teal and orange/red routes were inspected at native 480x272. The
deliberately blocked view shows the foreground rocky planet covering the hull
and engine wake; no freighter pixels are painted over its disc. The visible
tails originate at the rear of the modular hull, not beneath it.

Full-flight performance: `outputs/soft-sky-performance-20260930-012714-920/`.
All five cases pass the nominal 30 FPS test: fixed/fullscreen views measure
29.97 FPS and turning views measure 35.09–37.40 FPS in PPSSPP. Worst sampled
frame was 39.00 ms. This is emulator evidence, not a physical PSP guarantee.

The live shared source gained two concurrent station files during the final
build and temporarily referenced a missing `lave_arrivals_hero_pixels` symbol.
To avoid overwriting or packaging that unfinished work, this EBOOT was compiled
from the last successful 01:19 source snapshot with only the final freighter
geometry applied. Its `main.c` and `voyage.h` are byte-identical to current live
source; only `station-crawl.h` and `station-room-kit.h` differ. Shared root/dist
binaries, saves, version, Git state and the contributor's station files remain
untouched.

Remaining requirement: confirm scale, motion comfort and frame pacing on a real
PSP. Broad inherited station/outfitting test failures remain separate and must
not be described as fixed by this visual pass.
