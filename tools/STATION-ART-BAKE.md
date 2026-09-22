# Station art bake path

Host-side helper for the MacVenture + McQuarrie push. **Does not edit** `src/station-crawl.h`.

## Owners

| Lane | Owner |
|------|--------|
| Crawl runtime | Continue ELITE NEXT release |
| Look / kits | ART DIRECTOR (PR #11) |
| This bake path | Elite psp agent |

## Quick start

```bash
export PSPDEV=$HOME/pspdev   # or ~/work/toolchain
export PATH=$PSPDEV/bin:$PATH
./tools/bake-station-art.sh
```

### Outputs

| Path | Purpose |
|------|---------|
| `src/station-art-kit.h` | Palette macros + `ArtRoomStyle` table + `art_style_for_sc_room()` |
| `assets/preview/station-art/room-*.png` | 8 panel crops from the proposed kit |
| `assets/preview/station-art/room-*-thumb32.*` | 32×32 thumbs + optional `.bin` / `bin2c` `.c` |
| `assets/source/station-art/provenance.json` | Bake record |

### Source kits

Copied from ART DIRECTOR store boards into `assets/source/station-art/`:

- `station-room-kit-proposed.png` / `.svg`
- `celestial-identity-kit-proposed.png` / `.svg`

Authority docs (on art branch / PR #11): `ART-KIT-HANDOFF.md`, `STATION-ROOM-VISUALS.md`.

## Drop-in for release agent (they wire crawl)

```c
#include "station-art-kit.h"
/* inside soft-FB room paint: */
int ai = art_style_for_sc_room(room);
const ArtRoomStyle *st = art_room_style(ai);
unsigned wall = st->wall, wall2 = st->wall2, trim = st->trim;
```

Do not ship the 480×272 kit PNG as a fullscreen texture — use styles + small props.

## Memory rule

8 full-screen RGBA frames ≈ too big. Style tables are tiny; 32×32 thumbs are optional experiments (~4 KB each raw).
