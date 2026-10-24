#!/usr/bin/env bash
# Release 2.5.488: permanent gun housings and immediate primary fire; retail excludes embedded review code.
# Native Linux build for ELITE: NEXT (mirrors build.ps1).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
TOOLCHAIN="${1:-${TOOLCHAIN:-$HOME/work/toolchain}}"
if [[ ! -x "$TOOLCHAIN/bin/psp-gcc" ]]; then
  echo "PSP toolchain not found at $TOOLCHAIN" >&2
  exit 1
fi
export PATH="$TOOLCHAIN/bin:$PATH"
export PSPDEV="$TOOLCHAIN"
SDK="$TOOLCHAIN/psp/sdk"
BUILD="$ROOT/build"
OUTPUT="$ROOT/EBOOT.PBP"
if [[ "${ELITE_ENABLE_REVIEW:-0}" == 1 ]]; then
  BUILD="$ROOT/outputs/review-build"
  OUTPUT="$BUILD/EBOOT.PBP"
fi
mkdir -p "$BUILD"
CFLAGS=(-std=gnu11 -O3 -flto -fno-math-errno -G0 -Wall -Wextra "-I$SDK/include" -D_PSP_FW_VERSION=600)
# ELITE_ENABLE_REVIEW=1 is for disposable emulator QA, never retail packaging.
if [[ "${ELITE_ENABLE_REVIEW:-0}" == 1 ]]; then CFLAGS+=(-DELITE_ENABLE_REVIEW=1); fi
OBJECTS=()
for source in game ships main; do
  obj="$BUILD/$source.o"
  psp-gcc "${CFLAGS[@]}" -c "$ROOT/src/$source.c" -o "$obj"
  OBJECTS+=("$obj")
done
# Bound the host compiler's LTO heap; retain the same PSP optimisation level.
psp-gcc -O3 -flto --param=ggc-min-expand=20 --param=ggc-min-heapsize=32768 -fno-math-errno -G0 "-L$SDK/lib" "-specs=$SDK/lib/prxspecs" -Wl,-q,-T"$SDK/lib/linkfile.prx" -Wl,-zmax-page-size=128 \
  "${OBJECTS[@]}" "$SDK/lib/prxexports.o" \
  -lpspdebug -lpspgu -lpspdisplay -lpspge -lpspctrl -lpsppower -lpsprtc -lpspaudio -lpspmp3 -lpspjpeg -lpsputility -lpspnet_apctl -lpspnet_inet -lpspnet_adhocctl -lpspnet_adhoc -lpspnet -lpspwlan -lvorbisfile -lvorbis -logg -lm \
  -o "$BUILD/elite-a.elf"
psp-fixup-imports "$BUILD/elite-a.elf"
psp-prxgen "$BUILD/elite-a.elf" "$BUILD/elite-a.prx"
if [[ "${ELITE_ENABLE_REVIEW:-0}" != 1 ]]; then
  # Keep the 7 MiB heap, 1 MiB main stack and 3 MiB firmware reserve viable.
  image_bytes=$(psp-size "$BUILD/elite-a.elf" | awk 'NR==2 {print $4}')
  if (( image_bytes + 11*1024*1024 > 24*1024*1024 )); then
    echo "Retail PSP memory budget exceeded: $image_bytes image bytes" >&2; exit 1
  fi
fi
GAME_VERSION=$(tr -d '\r\n' < "$ROOT/VERSION")
mksfoex -d MEMSIZE=0 "ELITE: NEXT $GAME_VERSION" "$BUILD/PARAM.SFO"
INTRO_FILM="$ROOT/assets/intro/cinematic.enm"
if [ ! -f "$INTRO_FILM" ]; then INTRO_FILM=NULL; fi
pack-pbp "$OUTPUT" "$BUILD/PARAM.SFO" "$ROOT/assets/xmb/ICON0.PNG" NULL NULL "$ROOT/assets/xmb/PIC1.PNG" NULL "$BUILD/elite-a.prx" "$INTRO_FILM"
echo "Built $OUTPUT"






