# Animal life — 2.5.238 local candidate

The user paused the wider planetary biome rollout after finding that animals on Lave I were merely sliding sprites. This pass addresses animation and behaviour first. It is shared by existing fauna on every planet, with native visual verification focused on Lave I.

## What changed

- Eight existing noun/art families now have four distinct poses each: idle, two strides/wing positions, feeding/resting. The imagegen skill supplied the new AI-generated sprite sheet; a repeatable palette bake embeds 98,304 bytes. Exact prompt and source paths: assets/source/fauna-animation/README.md.
- Grounded gait phase advances with actual distance travelled. Blocking an animal stops both translation and leg animation. Hoppers/skippers rise and settle with their hop cycle; fliers flap and lift when moving. Rays hover. Moths/gliders rest with folded wings.
- Deterministic idle, wander, feed, alert, flee and return-home states replace perpetual sine/cosine translation. Private per-animal randomness does not consume gameplay RNG. Ground family walking speeds differ, fleeing is faster, and turns are bounded.
- Walkers alert wildlife within 36 m, runners within 75 m, rovers within 105 m. Animals notice before fleeing, settle after the threat leaves and wander around their home region. These are proximity rules, not a simulated hearing/vision system.
- Collision uses existing scenery/shore/field restrictions, bounded route probes and separation from other animals. A first test exposed routes aimed through obstacles; destination-only checks were replaced with short corridor checks. Sharp turns now happen before stepping toward a threat.
- Original species seeds, names, scan bits, Codex records, controls and serialized save layout are unchanged. Behaviour itself is transient and resets on arrival. Plants retain their prior sway cycle.

Implementation: src/fauna-behaviour.h, Lifeform runtime fields in game.h, enter_planet/planet_tick in game.c, field-world.h, field-sprites.h and generated/fauna-animation-pixels.h.

## Verification

Proof root: C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/fauna-life/

- candidate/fauna-behaviour.txt: 0 failures. Feeding, gait, deterministic random isolation, alert/flee/settling, walking/running/rover thresholds, two simulated minutes on safe terrain, home range, blocked gait, and 2,831 safe animal spawns across all 1,024 current planet slots.
- candidate/fauna-review.txt: 0 failures. All eight families have distinct poses; native grounded hopper and feathered glider captures, framebuffer guards. Staged Lave captures use a fixed camera and real update/render functions, not an illustrated mockup.
- candidate/animal-walk.gif and animal-walk-*.png: actual 480x272 framebuffer sequence. Playback uses simulated 50ms steps, not evidence of live emulator frame pacing. flyer-walk-*.bmp contains the separate flier run.
- regression/field-art-review.txt: 0 failures. All 8,192 species identity slots, twelve POI families, perimeter interaction, scan-to-Codex, docked save/load, leaves and depth/framebuffer checks.
- lave/lave-review.txt: 0 failures, including existing walking/rover trail coverage. Update + draw: 33.883ms average / 36.304ms worst, excluding audio, display wait and capture IO. Staged close-animal cold render reached 41.031ms. No sustained 30 FPS guarantee.
- broad/: game retains the same inherited station-tunnelling failure; radio and steering pass. Broad input/performance groups were not completed in this run. No whole-game green claim.
- Physical PSP remains untested.

Build: build.ps1 with isolated fauna-life/build and candidate/EBOOT.PBP. Run tools/review-fauna.ps1 on a disposable folder with fauna-review.flag; tools/fauna-proof.py converts captured BMPs. Never install review flags or test saves in a player's game folder.

## Delivery

C:/Users/skarm/Documents/Codex/2026-09-26/referenced-chatgpt-conversation-this-is-an/Animal-Life-2.5.238/EBOOT.PBP

6,878,823 bytes. SHA256 C5218D49DB0315A4CFEF94F1150988D95975BC24EFB2B44D11EF7575369895AB.

Only EBOOT.PBP is required for this update; sprite data is embedded. Keep existing music folders and commander saves. Root/dist EBOOTs were not replaced. No public release, commit, push or tag.

## Limits and next steps

These remain depth-tested side-view sprites with four authored-by-generation poses, not skeletal or multi-directional animals. Some pose anatomy varies; additional direction/feeding frames would improve them. Avoidance is local, not a full navigation mesh; fauna uses conservative walker/rover collision margins. Population count and spawn distribution were not expanded. There is no new breeding, hunting, predator/prey, audio or ecosystem simulation.

Resume the requested varied terrain/scenery/flora/fauna rollout only after this priority pass. The user explicitly wants floating cloud-platform settlements on gas giants. Existing code already allows gas-giant visits but depicts a flat island-like research surface; this is not a finished cloud city. Unify biome identity between terrain, orbital sprites, lore and collisions before building the remaining environments.

The new assets/source/biome-props/kit-pending.png is NOT used by the game; its apparent background needs alpha inspection/regeneration before baking. Its generation prompt is saved alongside. Preserve the shared dirty tree and unrelated work.
