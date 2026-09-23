# Native Canteen capture handoff

The current proof set is deliberately limited to three exact identities:

| System / hub / room | Family | Selector hash | Runtime scene |
|---|---|---|---|
| Reorte / H0 / CANTEEN | Frontier authored exception | `0x193c0782` | Second Shift |
| Qube / H0 / CANTEEN | Freight | `0x2e5fa27c` | native Freight Canteen |
| Xequerin / H0 / CANTEEN | Research | `0x5dd95221` | native Research Canteen |

`src/station-crawl.h` now requires the exact baked Systems family, exception and
selector hash before selecting a native scene. A descriptor change falls back
instead of silently reusing an art payload. All other Canteens remain on the
truthful generic renderer.

## Review evidence versus runtime evidence

The three 480×272 review frames are art evidence only. The Qube and Xequerin
review PNGs still carry the Reorte/Second Shift header, options copy and footer
because they were generated from the shared review template. Runtime owns those
bands and overlays the current room/player truth; the packed scene payloads are
the only room art consumed by `src/station-family-art.h`.

Before promotion, capture each identity in the integrated EBOOT at native 1×:

1. enter from flight and disembark into the station;
2. walk/select a person and open truthful speech/action choices;
3. exercise available and unavailable capability states without inventing work;
4. traverse a room door and return through SHIP/Triangle;
5. repeat in normal and high contrast while recording frame time, static bytes,
   temporary memory and save/economy stability.

No emulator certification is claimed until those captures and reports exist.
