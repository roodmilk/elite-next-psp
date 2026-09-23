# Station room-sign and doorway cue kit

Status: candidate art-only handoff. No Station or gameplay runtime code is changed.

The kit supplies seven destination-room plaques, seven matching doorway cues,
route and ship-return markers, and one arrival-window identity treatment, each
in normal and high-contrast variants. It is authored as hard 1× pixels for the
PSP room viewport, not as a reduced large illustration.

## Runtime binding proposal

Station remains authoritative for the existing `SC_H_EXIT` hotspots and
`sc_room_short(destination_room)` labels. When an exit is visible, select the
cue by its destination room ID and place it in one of the existing four door
slots from `src/station-crawl.h`; no new hotspot or action is implied.

| Slot | Door scene-local anchor | Sign scene-local anchor | Existing geometry |
|---|---:|---:|---|
| TOP_LEFT | `(4,36) 50×60` | `(4,20) 80×20` | `door_xy[0]` |
| TOP_RIGHT | `(286,36) 50×60` | `(260,20) 80×20` | `door_xy[1]` |
| LOW_LEFT | `(70,120) 42×26` | `(50,100) 80×20` | `door_xy[2]` |
| LOW_RIGHT | `(180,120) 42×26` | `(160,100) 80×20` | `door_xy[3]` |

The lower doorway anchors are intentionally documented as cue-safe artwork
placements. Station must crop or choose the smaller lower-door cue geometry
when the existing 42×26 hotspot is used; the 52×60 art is the full side-hatch
variant and must not be allowed to obscure a focal object.

The arrival-window treatment is `128×54` at scene-local `(80,20)`, inside the
existing `BERTH WINDOW` feature hotspot `(72,10) 176×70`. The `24×12` ship-return
marker is centered at screen `(296,165)` inside the existing SHIP safe rectangle
`(274,158) 68×26`. The `16×8` route marker is reserved for the right-hand
destination selector row; it has no interaction semantics until Station binds an
exact draw point.

## Visual contract

- Labels exactly mirror the existing selector vocabulary: `ARRIVAL`, `SHOP`,
  `CANTEEN`, `CARGO`, `GUILD`, `CLINIC`, `CUSTOMS`.
- Room motifs distinguish function by shape: berth window, wrench, mug/lamp,
  cargo pallet, survey star, medical cross and scanner gate.
- Normal art uses the existing station palette roles. Contrast art preserves
  silhouette and geometry while lifting structural marks to cream/cyan.
- No asset creates an interaction. The only actionable object remains the
  existing destination exit hotspot.

## Files and generation

`assets/preview/station-cue-kit/STATION-CUE-KIT.json` is the manifest. The
fourteen room PNG pairs plus the wayfinding PNG pairs are native-size
candidates; the room and wayfinding `*-native.png` files are 480×272 proof
sheets. Rebuild them with:

```powershell
.\tools\bake-station-cue-kit.ps1
```

The generated-image reference sheet is kept separately under
`assets/source/station-art/room-cue-kit-reference.png`; it is a composition
reference only and is not a runtime source.

Promotion is blocked until Station confirms anchor ownership, Systems confirms
the packed budget/deterministic output, and Composer confirms that signs do not
collide with family focal objects.
