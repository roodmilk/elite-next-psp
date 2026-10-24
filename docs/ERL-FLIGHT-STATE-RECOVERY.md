# ERL flight-state recovery - 2.5.410

Accepted events remain RACE_GRID until the player contacts the start beacon and confirms READY. Ordinary flight HUD, radio and controls remain active during this approach. Race HUD is limited to countdown/running states, including the existing replay's temporary running state.

Both PSP suspend and resume handlers use the same phase-aware pause function. Only actual ERL countdown/running states open Resume/Withdraw. Pre-start paused/resume flags from older builds are cleared in live input, UI ticks and save recovery without cancelling the contract or awarding/advancing anything. Pre-start save recovery does not force an airborne race camera. No serialized fields or save version changed.

Resume X during the opening countdown is consumed by the pause dialog before the optional flyby skip. Active-race checkpoint, protection countdown, withdrawal confirmation and result handling remain intact.

Validation: 20 native PSP regression checks pass, including an affected saved pending event, phase-specific suspend/resume, exact countdown/running progress, Resume/Circle/Withdraw/confirmation, beacon READY, HUD pixel isolation and radio selection. These exercise the production handlers; actual physical PSP power callbacks still require player confirmation.

Exact retail EBOOT: original 32 MiB emulator startup/music checks pass; countdown and populated racing with supplied MP3 and four async saves run at 59.94 FPS, worst 19.26ms, no >100ms stalls. Native 480x272 pending-flight and active-pause captures inspected. Existing Shockwave victory regression also passes. Picross implementation is unchanged from verified 2.5.409.

Release is local with frozen source and evidence. GitHub publication remains unavailable on the shared unfinished tree after the earlier automatic review rejected whole-tree rebase/autostash. Wider pre-existing harness failures and unfinished arcade/newspaper expansion are not claimed resolved.
