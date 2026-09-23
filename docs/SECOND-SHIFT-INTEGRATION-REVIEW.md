# Second Shift authored room integration review

## Scope

This increment promotes the Reorte system 39, primary hub, Canteen room to the
native authored-room path. It is the first runtime proof that a room can carry
the attached Second Shift / pulp-SF fidelity bar while the existing station
crawl remains interactive.

The room is selected only when:

```text
room = Canteen
system = 39 (Reorte)
station hub = 0 (primary / H0)
```

All other rooms retain their current authored procedural illustration path and
descriptor-backed cosmetic composition.

## Native asset contract

- Runtime scene: 340 x 168 pixels, native 1x, packed 4-bit palette indices.
- Screen placement: the existing 340 x 168 MAIN art viewport at native PSP
  resolution; no filtering, scaling, or heap allocation.
- Palette: 11 colors, held in `src/station-bar-art.h`.
- Review image: `assets/second-shift-native.png` is a 480 x 272 native screen
  review showing the art with the real options, speech, and SHIP bands.
- The illustration contains the room's three painted inhabitants. The runtime
  does not draw generic NPC sprites, generic hatches, or procedural accents on
  top of this exact room.

## Preserved runtime behavior

- The live options list, verb selection, speech, shop/taxi state, and SHIP
  return remain active.
- Existing Canteen people hotspots are remapped to the authored figure
  positions; selecting one adds an amber outline only.
- The SHIP affordance remains a live overlay and is not baked into the room
  illustration.
- High contrast still owns the runtime UI and feedback colors. A true native
  high-contrast capture is still required before this room is certified.

## Evidence and remaining gates

The native source review is present at `assets/second-shift-native.png`, and
the packed runtime source is `src/generated/second-shift-pixels.h`. The build
must still be run on the integrated branch and the following evidence must be
captured from the resulting EBOOT:

1. Reorte H0 Canteen in normal mode at native 480 x 272.
2. The same room in high-contrast mode.
3. Entry from Arrivals, person selection, speech/action feedback, and SHIP
   return.
4. A measured frame/static/temporary-memory report for the integrated EBOOT.

Until those captures exist, this is an integrated authored-room candidate, not
the final certification of the full station set. The same fidelity gate must
be applied to every remaining room family and authored exception; sparse
geometric or tint-only replacements do not pass.
