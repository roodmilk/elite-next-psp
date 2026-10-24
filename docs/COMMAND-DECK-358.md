# Command Deck 2.5.358

Built on verified local 2.5.357 (EBOOT SHA256
68ED085740E86B135A29E8D25833AB6EC73B8BD0EC9B8831C95BAA065A7C5564).
This is the approved Observation Lounge 6D concept, implemented in the PSP
renderer, rather than a fullscreen mockup placed over the old menu.

## Native layout

All coordinates are actual pixels on 480x272. Font remains src/font8.h, 8x8.

| Element | Rectangle / position |
| --- | --- |
| Original native logo | 8,3; 100x18, never resized |
| Green docking status plate | 270,2; 202x19 |
| Docked ship pictogram | 276,5; 16x12 |
| Categories | x8 + column*94, y31; 88x20 when all five are visible |
| Sliding category marker | y53; 36x2 |
| Category heading | 16,62; 176x15 |
| Service buttons | x12, y85 + index*25; 208x22 |
| Seven-service Fly bank | y85 + index*22; 208x20 |
| Actual-size service symbols | 12x12; x22 |
| Description card | 236,199; 236x43 |
| Description title | 266,206; up to 24 native cells |
| Description body | 246,222; 26 cells, two lines, 10px leading |
| Footer and units | y248; fixed right edge at x472 |

With fewer unlocked categories the visible tabs expand evenly. Real service
filtering still applies to opening progression, flight, poor ports and relays.
Station names come from the current actual hub. Lave System Hub is compacted
to Lave Hub in the status plate. Longer names use a two-line plate; text beyond
21 cells is ellipsised inside the same region. Large balances use M units.

## Visual and interaction contract

- Category focus: teal recessed illumination, shadow and sliding marker.
  Left/Right or L/R shoulders change categories immediately, with a 180ms
  ease-out service-bank slide. Each category retains its selected service.
- Service focus: amber edge and a small rim shine. Native icons depict cargo,
  a ship, a module chip, loadout slots, a broad flat paintbrush and a spanner.
  Work uses objective, board, log and faction symbols.
- X invokes the existing action once. Menu openings have a 60ms confirmation
  flash, outgoing buttons slide away, then the selected button rises toward
  the destination header. The destination is fully revealed by 340ms.
  Any new deliberate input interrupts the visual handoff rather than queuing
  input. Circle continues to return to its real caller. Launch/resume remains
  immediate. Opening/tutorial progression does not wait on cosmetic effects.
- The docked station sprite remains still. Its fixed port lights glow and
  twinkle; it never rotates or rocks. Poor/rich/mega hubs and relays use their
  corresponding art. In flight, the existing real-hull ship preview remains.
- Background star glints and broad nebula luminosity waves animate gently.
  Native sun art has at most 1px horizontal plasma drift and a small light
  modulation. No large deformation, spinning fireball or distracting flash.
- Docking status has a gently pulsing ship/berth icon. Labels stay still.
- Existing menu notices use the description card, preventing a full-width
  message plate from covering buttons or text. Tutorial service outlines use
  the new row geometry instead of writing a second obsolete list over it.
- Existing 2.5.357 Multiplayer, Controls, Cargo, Shipyards, Loadout, Storage,
  Outfitting, save/settings and music behavior remains canonical. V32 saves
  and all networking protocols are unchanged.

## Implementation and checks

src/command-deck-console.h owns the renderer and bounded visual handoff.
src/deck-nav.h owns remembered navigation and category motion state.
src/deck-ui.h routes home() to the console and preserves other screen code.
src/command-deck-review.h exercises production input and rendering through
tools/review-command-deck.ps1. Captures are native 480x272 BMP files.

No per-frame heap allocations are used. Two bounded static RGB565 snapshots
cost 499,200 bytes and are used only for the short service handoff. A 226,968-byte
native backdrop colour cache avoids decoding all background pixels every frame.
Background and sun artwork reuse existing compiled assets; no AI image scaling is used.
Actual framebuffer text is clipped during slides, not simply covered later.

See BUILD-2.5.358.txt for final evidence, limitations, hashes and package paths.
Physical PSP testing remains required; emulator render timing is not a claim
about PSP frame rate. The inherited broad game failures are tracked separately.
No GitHub sync is claimed while DNS/network access is unavailable.
