# Menu openings — 2.5.361

Selecting a Command Deck service pulses its button for 70 ms, then expands the live destination from that button to the full native 480×272 screen over 200 ms. Cubic easing and the current ship paint colours keep the transition consistent with the console. Launch skips this overlay so flight remains visible immediately. Nested shop and console switches retain immediate openings.

One clean off-screen Command Deck snapshot uses 557,056 static bytes. No frame allocations or live/front-buffer reads occur during activation. In-place destination compression uses one scratch row and outward traversal from the ceiling fixed point; the sampler matches an immutable reference across four overlap cases, including near-full-screen dimensions. Unused framebuffer gutters remain untouched. No feedback replay, scan bands or full-screen white flash is used.

Circle cancels the transition and returns immediately. Directional input cancels it and moves selection immediately. A second rapid X reveals the destination without buying an item that has not yet been shown; subsequent X works normally. Chart avoids a second dissolve. Changing page cancels stale animation state.

The exact release passes 36 Command Deck checks and two normal boot checks. The 17 sampled opening frames average 6.805 ms, worst 9.100 ms, in PPSSPP; clean activation costs 8.556 ms. These are emulator CPU timings, not a physical PSP frame-rate guarantee. Native captures include pulse, expansion, final page and 42 paint-theme views. Physical PSP startup, input feel and frame pacing remain a player test.
