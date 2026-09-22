#!/usr/bin/env bash
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
mkdir -p "$BUILD"
CFLAGS=(-std=gnu11 -O2 -G0 -Wall -Wextra "-I$SDK/include" -D_PSP_FW_VERSION=600)
OBJECTS=()
for source in game ships main; do
  obj="$BUILD/$source.o"
  psp-gcc "${CFLAGS[@]}" -c "$ROOT/src/$source.c" -o "$obj"
  OBJECTS+=("$obj")
done
psp-gcc -G0 "-L$SDK/lib" "-specs=$SDK/lib/prxspecs" -Wl,-q,-T"$SDK/lib/linkfile.prx" -Wl,-zmax-page-size=128 \
  "${OBJECTS[@]}" "$SDK/lib/prxexports.o" \
  -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpsppower -lpsprtc -lpspaudio -lpspmp3 -lpsputility -lm \
  -o "$BUILD/elite-a.elf"
psp-fixup-imports "$BUILD/elite-a.elf"
psp-prxgen "$BUILD/elite-a.elf" "$BUILD/elite-a.prx"
mksfoex -d MEMSIZE=0 'ELITE: NEXT 2.5.29' "$BUILD/PARAM.SFO"
pack-pbp "$ROOT/EBOOT.PBP" "$BUILD/PARAM.SFO" NULL NULL NULL NULL NULL "$BUILD/elite-a.prx" NULL
echo "Built $ROOT/EBOOT.PBP"
