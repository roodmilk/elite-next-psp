# Natural anomalies — 2.5.466 local PSP-3000 playtest

28 additional natural phenomena are integrated with the existing four field types. They use live world-space rendering, real targeting, production analysis scans, instrument reports, Discovery Codex records and existing research rewards.

## Catalogue

Regular (20 sites each): Glass Reef, Shattered Moonlet, Comet Garden, Diamond Rain, Magnetic Jellyfish, Plasma River, Lightning Tree, Frozen Supernova, Pearl Swarm, Solar Lotus, Dust Waterfall, Obsidian Crown.

Unusual (13 sites each): Twin Vortex, Singing Needles, Ember Geysers, Aurora Sail, Gravitational Pearl, Prism Arch, Dark Nebula Lung, Stellar Chrysalis.

Rare (five sites each): Spiral Fossil, Ion Thunderhead, Tidal Ribbons, Wandering Glacier, Void Eye, Fractal Bloom, Mirror Shoal, Phase Cathedral.

Aurora Veil, Gravity Lace, Ember Nursery and Meridian Echo retain their original system/slot identities and positions.

## Placement and progression

There are 384 new sites across all 256 generated systems. Every system receives slot 2; exactly 128 systems also receive slot 3. No system receives the same new type twice. Assignments are fixed, with explicit rarity quotas and a geographical balancing cost that penalizes nearby repeats and concentration within the same region. Regular types cover 14–16 of the 16 map regions, unusual types 12–13, and each rare type covers five distinct regions. The generated galaxy is connected at a seven-light-year jump range; all new types occur within that network.

New positions use a bounded deterministic search around the system hub and remain outside active body surfaces, all three station hubs and other local anomalies. The native all-system checks confirm the clearance bounds. Their positions and type assignments do not depend on visiting order or the traffic random stream. Tutorial Sol continues to suppress anomalies under its existing rules.

The existing four-bit per-system scan records accommodate the new sites. Save/load serialization remains byte-identical to the preceding build; there is no save-version change. Existing records and claimed rewards are retained. Reopening a scan pays nothing a second time. These are observational discoveries: they do not add damage, teleportation, fuel collection or mining mechanics.

The Codex and System Operations calculate remaining signals from actual local availability, so systems with fewer than four active sites no longer suggest impossible extra scans. Saved analysis records now also establish a known contact name if the separate identity sidecar is absent. Controls and existing scan range remain unchanged.

## Rendering

Solid mineral, ice and rubble forms have three-dimensional facets, sun-facing colour and cold shaded backs. The light forms have distinct world-space paths: folded curtains, elongated plumes, coils, pleated sheets, paired vortices, split tails and branching secondary/tertiary forks. Sparse motes, smooth time-based pulses and restrained haze supply motion and depth. Absorbing dark dust respects foreground depth; individual light samples also check celestial occlusion. The mineral silhouettes use the established centre-based body visibility convention.

Loops, sample counts, glow radii, geometry counts and work buffers are bounded. No new render-time file reads or heap allocations are introduced. Distant fields reduce strand counts and motes. Visible fields use the existing cached framebuffer path. Small glow falloffs and cloud weights are prepared once; overlapping dust layers accumulate in a transient byte mask and colour the framebuffer once per pixel. This is software shading with existing opaque rendering, not a programmable PSP shader or a full-screen blur.

## Verification and limitations

The dedicated native test covers all 256 systems, 384 new sites, quota/region counts, exact legacy positions, body/station clearance, revisit stability, real analysis/report selection, single rewards and save/reload of every system's old and new slot bits. The review gallery provides native approach, side, reverse, inside and distant views for all 32 types, with eight moving camera studies. The camera scenes are staged in PPSSPP using the real PSP executable and game renderer; their motion is scripted camera/game time, not a recorded human playthrough.

Foreground tests cover all 28 new luminous/absorbing effects with a fully opaque depth mask. Framebuffer guards cover close and inside views. Real MP3 and recorded player-engine playback are measured concurrently in six anomaly scenes; raw reports accompany the local package. The first cloud implementation was too expensive. Prepared integer falloffs, row spans and combined cloud opacity reduce measured cloud drawing from about 16 ms to about 3 ms and roughly halve light drawing cost in those scenes.

The broader gameplay test runs 687 assertions and retains the same 18 pre-existing failures as 2.5.465, with an identical failing-assertion list. This is not an all-green stable release. Busy station/ship views remain performance work. Physical PSP-3000 frame pacing, listening, prolonged travel and sleep/resume remain unverified. Emulator timings are not physical-device certification.

## Delivery and repository state

Local playtest: `outputs/releases/2.5.466-natural-anomalies/`. Replace only EBOOT.PBP in the installed game folder; retain saves, music and other files. The package includes the complete source snapshot, focused patch, generator, authored legacy catalogue, location registry and verification reports. Audio, ship art, station art, menus, race mechanics, approved race controls and victory source files outside the listed integration edits are preserved.

The starting build was compiled. The generic smoke script expects an embedded-review executable and cannot obtain its game report from retail; that limitation is recorded, and dedicated native checks plus exact-retail startup/playback checks are used instead. The starting game.c source matches the preceding tested 2.5.465 snapshot. Stable VERSION/dist remain unchanged because the broader gate is not green. Git pull failed DNS resolution and the submodule shell could not initialize in the sandbox; no GitHub sync, stable promotion or release tag is claimed. The shared tree contains substantial other contributors' work, which is preserved.

The eight exported motion studies use a scripted 2.88x preview clock at20 frames per second; they illustrate shapes and are explicitly labelled separately from measured playback frame pacing.
