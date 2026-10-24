# Homeward opening — 2.5.265

The opening has been rebuilt around authored conversations and artwork. Start NEW GAME, finish the name/portrait screen, and the tutorial begins with the combat dream. Existing commanders keep their progress.

## Playable changes

- 268 authored dialogue nodes with optional questions, distinct responses and converging branches. Dad feels like a parent who knows this seventeen-year-old; Mum's call includes the Tesco / fifteen-thousand-kilometre pizza joke.
- Text reveals at roughly 36 characters a second. Cross reveals a line; a fresh press continues. Up/Down selects a reply, then Cross confirms. Held Cross cannot skip from dialogue into the next scene or action.
- Six illustrated native-resolution backgrounds: bedroom, Earth street, showroom, countryside, Earth orbit and the empty cockpit. Eight portrait sprites, three illustrated starter ships and a party photograph follow the owner's pixel-art reference.
- A real dealership selection step: compare Adder, Gecko and Moray, see hold/speed/range and their strengths. Square toggles the rotating game hull. Choose, then confirm with Dad or keep looking. Dad pays for the ship; the starting credits remain for jobs and services.
- Untaught services and empty command-deck categories are absent. Earned services remain visible, with the current lesson highlighted and input restricted to its next action. Direct page changes also respect these unlocks. Galaxy Map appears only after displacement; Factions and Galactic Lore remain later discoveries.
- Longer steering practice and quiet travel beats. Conversations wait for the player rather than advancing on a timer. Mum's incoming call still teaches Triangle directly, then Talk. The Sol news recognition happens after opening Lave Local TV.
- The countryside pep talk and show/hide object choice use branching dialogue. Dad reacts differently at the Thargoid encounter depending on whether he already knows about the object. Object ownership and that choice persist.
- Earth Spacebook contains an illustrated party attachment; Cross opens it and Circle closes it. Home Inbox threads wrap as separate messages. Earth news, stocks and jobs use natural fictional-world copy; cached Earth messages survive the change of network.
- The city-to-orbit reveal blends authored scenery. Thargoid craft appear over the Earth view before the burst. On waking, the empty copilot seat has its own scene.

## Launch fix

SOL ROCK expanded the radio station count to nine, but the cockpit ticker previously had only eight rows. Its ninth row therefore contained null pointers and could call strlen(NULL) during launch. Added all 24 SOL ROCK lines and a defensive ticker fallback. The final tutorial and real main-loop launch runs have no invalid memory accesses in PPSSPP logs.

## Saving

V27 is retained. The existing counter stores stable dialogue-node ID and highlighted reply while talking; bit 12 marks that a stage's conversation is finished. The action then reuses its counter normally. Validators accept the flag and bounded encoding. Reload preserves a chosen conversation node/reply, the countryside object branch and completed flight instructions. Old commanders remain stage zero. Older 264 tutorial checkpoints can repeat the current stage's new conversation before returning to its action.

## Exact build validation

EBOOT SHA256: `CA0EE2B8A51ABB63874AA1E49FC9D9A431B1A48FC19E21A6AE8E2A003CF8766C` (9,872,231 bytes).

- All three starter ships completed the full tutorial using real guided inputs. Both object choices, real repairs/refuelling/pizza purchase, save/reload and final free play passed.
- The 268-node graph passes entry, reachability, forward-edge, terminal, effect, native text-layout and save-bound checks. Earned-menu fixtures pass.
- Actual normal main loop passed 1,202 frames, including 148 launch frames, both VRAM display planes and active SOL ROCK audio. No ignored invalid memory reads in this run or the tutorial run.
- Core game, radio and steering reports finish with RESULT 0 failures.
- The separate broad legacy smoke suite is not green: 191 input assertions fail and sampled performance averages 35.81 FPS, below its 50 FPS threshold. Outfitting, older campaign/dialogue assertions and legacy graphics/controls checks need further investigation. Its graphics fixtures also report ignored null framebuffer accesses; that test-harness issue is separate from the fixed live-launch ticker. Do not describe these broad results as a full-game certification or as a proven unchanged baseline.

This is a local development build, not a public GitHub release. Physical PSP testing is still required. The existing five-body Sol flight model and broader 3D planetary/space renderer remain; the eight planets are shown on the tutorial map, and the new authored images belong to the opening scenes. The post-Lave Dad rescue/homecoming campaign remains further story work.

## Artwork and rebuilding

Selected image-generation outputs are saved in `assets/story/`; native palette-indexed bitmaps are embedded in `src/opening-bitmaps.h`. Runtime loading requires no PNG files. `tools/compile-opening-art.py` uses Pillow to resize and quantize the supplied originals and preserve transparency in ship sprites. It prefers the included assets before the original generation directory. Ordinary build scripts compile the embedded header.

The image-generation skill and built-in tool were used with the owner's attached countryside screenshot as the style reference. No personal photographs or copyrighted songs are bundled. SOL ROCK is synthesized original music.
