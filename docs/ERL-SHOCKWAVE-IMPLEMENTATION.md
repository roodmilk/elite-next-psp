# ERL Shockwave Flyby - 2.5.409

Implemented from the 2026-10-11 v6 handoff on top of frozen 2.5.408. Full handoff and all supplied reference frames/motion source inspected. Local preview browser playback was security-blocked; no claim of watching that video playback. Native in-game motion frames are captured separately.

## Playable behaviour

Actual 480x272 software-rendered animation, not video playback. Racing-speed world motion from frame one, launch at 0.25s, eased side swing, shining numeral, impact at 2.71s, 0.22x presentation slow motion and continuous catch-up/hero arc. Stars are anchored in deterministic world slabs, with stable infinity sky and twinkling. Camera and ship continue moving while waiting for input; the reveal never loops.

The actual player mesh or authored capital hull, paint and exhaust roots are used. Single asymmetric Needlefish engine, Cobra roots and capital pods are supported. Filled near-clipped exhaust ribbons, curling vapour and numeral fragments use hull depth. Sun and warm hull lighting build during the finish. 1ST PLACE gently wobbles; commander name and exact committed rewards remain readable. Existing recent-action replay occurs first. X skips only the reveal when early; subsequent X selects the existing action. Official STANDINGS/CONTINUE and Circle are retained.

No changes to payment, progression, season ownership, fleet rewards, saved paint, race audio restoration or V36 serialization. Existing V35 migration and the 256 KiB checkpoint worker remain. Full arcade handoff is not completed by this victory change and stays paused for player feedback, as does newspaper expansion.

## Performance design

Fixed 40 numeral fragments, seven cached star slabs, bounded ribbon samples. No allocations in animation frames. Render alpha effects in cached RAM and copy the completed plane once; reuse the Command Deck's scratch plane because its opening transition cannot run during a flight result. Preserve and restore native render state and framebuffer pointer. No extra persistent framebuffer allocation. All authored effects remain real time.

## Verification

23 focused checks pass. Renderer mean 10.02ms, worst 16.90ms. All 23 player hulls and long-name/large-reward/official-result native captures inspected. Populated retail countdown/racing with Sol Rock and four background saves: 59.94 FPS, worst 19.26ms, zero 100ms stalls; worker free stack 261816 bytes.

Retail victory with MP3: 56.80 FPS average, worst 33.37ms, zero frames over 100ms or 250ms. Original 32 MiB emulated PSP model; actual user music fixture decoded. Cold boot passes five checks with 3,504,640 bytes minimum free kernel memory. These are emulator measurements, not physical PSP guarantees.

Final test results and hashes are recorded in the frozen release BUILD report and evidence directory. Physical PSP-2000/ARK testing remains pending. Emulator measurements are not hardware guarantees. Early failed candidates are retained under outputs/erl409 for diagnosis and are not shipped. Broader pre-existing arcade expectations remain stale; focused victory verification is reported separately, never as a claim that all game tests pass.

## Source/publication

Frozen source, baseline-relative patch, native screenshots and test logs accompany the local EBOOT. GitHub is unchanged: previous automatic approval review rejected rebasing/autostashing the shared tree containing unfinished unrelated work. No shared work was reset or discarded.
