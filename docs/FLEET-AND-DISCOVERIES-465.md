# Fleet and discovery detail — 2.5.465 local PSP-3000 playtest

The space-flight detail pass is integrated into the game renderer. This is a playable local checkpoint, not a stable release or a claim of locked 60 FPS.

## What changed

- All 29 standard named spacecraft now have fitted pixel-panel artwork and sun-responsive materials, with physical glazing and authored engine outlets. The approved 80-vertex/144-face Adder is retained. The other classic silhouettes receive physical armour bevels, raised canopies and recessed engine collars; small distant geometry uses cheaper fitted models with the same panel art and outlet positions.
- All 12 named Mega capital hulls receive fitted surfaces. Actual capital NPC contacts now display their named hull instead of the previous Anaconda stand-in. Their original collision radius, identity, AI, loot and encounter rules are retained.
- The three NPC freight styles receive differentiated cargo, tank, machinery, glazing and engine materials, directional sunlight and anchored exhaust.
- The 75 discovery catalogue entries have explicit architectural footprints and functional material assignments across habitats, gardens, docks, liners, rings, spires, dishes, tanks, cargo structures, sails, fauna and ice forms. The detailed listening post and abandoned giant freighter are preserved. Generic modulo-four wireframe substitutes are removed.
- Aurora Veil, Gravity Lace, Ember Nursery and Meridian Echo use world-space layers, drifting motes and depth-tested glow. The duplicate screen-space pulse is removed. Supplementary wake lights and discovery details respect foreground depth.
- Existing shipyard and loadout preview areas fit the actual new standard geometry. Player paint is retained. Race controls, race rendering, progression, rewards, menus, victory sequence and the save format are preserved.

## Performance adaptations

The atlas shares functional panel art and adds individual hull markings, rather than compiling 44 full Adder texture banks. Source artwork occupies 28,896 packed bytes. Hardware textures are prepared once at startup; there are no new render-time file reads or heap allocations. Pixel minification uses 32/16/8 tiles, retaining crisp nearby art.

Matching opaque faces are batched by material and lit with vertex colours. A bounded resident texture bank, guarded 512 KiB command arena and cheaper middle-distance models reduce the cost. Previous opaque geometry is resubmitted with colour writes masked so a later ship pass cannot draw through an earlier station. A native foreground/behind test caught and corrected the positive-Z depth direction. CPU fallback remains available. The visible hardware output remains native 480×272; its auxiliary effect/depth mask retains the existing coarse station-mask convention.

GPU material lighting is an approximation of the software palette treatment: small inset lights and metallic trim are modulated with the hull's face lighting. The real outlet flares and distant beacons remain separate luminous effects. This is deliberate PSP adaptation, not a per-pixel programmable shader or full anti-aliasing implementation.

## Verification and limits

- Native bounds/UV/material coverage: all 29 standard hulls, their middle-distance variants, 12 live capital models, three freight styles, 75 catalogue entries, four natural fields and all 22 player/showroom hulls. Hundreds of native captures, including a moving discovery pass; framebuffer guards remain intact.
- Cached ship projection produces identical pixels in 24 paired native near-plane/angle checks. Separate hardware flushes correctly hide geometry behind a foreground object and reveal it in front. Rendering leaves the complete Game state unchanged.
- Exact retail PSP-3000 flight startup, supplied-MP3 decoding, memory and ERL race startup checks are included in verification/. The initial near-station retail MP3 check measures 32.86 FPS versus 33.37 FPS for the preserved starting build, with zero device-buffer starvation and no decoder errors. This workload did not activate the new detailed-fleet path.
- A separate native moving-ship test verifies the detailed hardware path, recorded engine/traffic sounds and real supplied MP3 playback concurrently. Four close ships measure about 20 FPS in its first view, 12 FPS from the exterior camera and 30 FPS on the discovery approach. The paired old-renderer workload measures about 30, 15 and 30 FPS. All report zero audio-buffer starvation and decoder errors. These slow close-up views remain performance work, not an achieved smoothness target.
- The broader gameplay suite retains exactly the same 18 pre-existing failing assertions as the preserved baseline. No new failing assertions were introduced. This is not an all-green stable-release claim.
- Physical PSP-3000 frame pacing, listening, prolonged travel/sleep and final individual art approval remain unverified. Captures are real engine output in PPSSPP with staged camera poses; they are not physical-device captures or proof of hardware performance.

## Scope retained

The current discovery selector still places eight local discoveries only in its existing certification systems (0, 1, 2 and 7). Entries 64–74 remain dormant under that existing selector. The art registry covers them without silently expanding spawns, adding services, moving existing objects or changing scan/progression rules. Ordinary auxiliary debris/pods and ERL's separate cinematic/race geometry are retained on their existing paths.

## Install and preservation

Use this PSP-3000 build by replacing only EBOOT.PBP in the existing game folder. Keep saves, music and the other game files. No USB files or personal music were changed by this task. The gallery offers native front/rear/side, middle/far and player/showroom views.

Canonical game.c, game.h, ships.c, audio.h, race rules and the victory renderer are unchanged from the task-start snapshot. The source snapshot, manifest and focused patch are included here. The current working tree contains other contributors' unfinished work; the attempted pull was blocked by pre-existing untracked music placeholders and its autostash was restored. Stable VERSION/dist remain at their prior checkpoint; no clean-tree, GitHub sync, stable promotion or release-tag claim is made.

Artifact: outputs/releases/2.5.465-fleet-and-discoveries/gallery.html
