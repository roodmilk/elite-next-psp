# Arrivals native review

This is a 1× native authoring review, not a runtime capture.

Files:

- `outputs/arrivals-native-authored.png` — 480×272, 10 used colours from the canonical 11-role palette, SHA256 `819321DC46E8D8AB53184C9E6D4F5A0DBD9DE61451781682551DBF24A632358F`.
- `outputs/arrivals-native-authored-contrast.png` — 480×272 high-contrast companion, SHA256 `25A7BCCBEA7A0338F16F955BE0BA98E10B5DBC705DC64C696F713C2815F6EAC1`.
- `assets/generated/station-arrivals-native-authored.png` — source copy for the packed candidate.

At 1:1, the header reads `ARRIVALS`, the options entries remain separated, the feedback sentence fits the 480-pixel line, the `SHIP >` return is isolated in its reserved rectangle, and the room’s clerk, traveler, freighter, berth rail and planet remain distinguishable. The high-contrast frame preserves those silhouettes and labels while reducing material shades. The generated larger-canvas frame is not used for this check.

The art is directly authored on a 480×272 canvas with integer rectangles, polygons and pixel clusters. Only after the native review is the room packed into 28,560 bytes for runtime. This satisfies the native-first rule; it does not claim the renderer is already wired or that the screen has passed emulator performance.

Quality note: this is a bounded technical production candidate and does not yet match the full authored richness of the Second Shift bar. The shared family contract is ready; further screens should be reviewed against the bar benchmark before multiplying assets.
