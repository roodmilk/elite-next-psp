#!/usr/bin/env bash
# Bake ART DIRECTOR station kits → src/station-art-kit.h + preview crops.
# Does not touch station-crawl.h.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
export PATH="${PSPDEV:-$HOME/pspdev}/bin:${PATH}"
# Default: header + PNG crops. Pass --bin2c for thumb .bin/.c experiments.
python3 "$ROOT/tools/bake-station-art.py" "$@"
echo "Tip: release agent can #include \"station-art-kit.h\" and use art_style_for_sc_room()."
