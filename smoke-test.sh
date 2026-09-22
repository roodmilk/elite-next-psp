#!/usr/bin/env bash
# Native Linux smoke test for ELITE: NEXT (mirrors smoke-test.ps1).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
EMULATOR="${1:-${EMULATOR:-$HOME/work/ppsspp/bin/PPSSPPSDL}}"
if [[ ! -x "$EMULATOR" ]]; then
  echo "PPSSPP not found at $EMULATOR" >&2
  exit 1
fi
if [[ ! -f "$ROOT/EBOOT.PBP" ]]; then
  echo "Missing EBOOT.PBP — run build.sh first" >&2
  exit 1
fi
# PPSSPP SDL needs a real X display + PulseAudio (dummy SDL video breaks GL/software).
unset SDL_VIDEODRIVER || true
export DISPLAY="${DISPLAY:-:1}"
if ! xdpyinfo -display "$DISPLAY" >/dev/null 2>&1; then
  if command -v Xvfb >/dev/null 2>&1; then
    Xvfb "$DISPLAY" -screen 0 1024x768x24 >/tmp/elite-xvfb.log 2>&1 &
    sleep 0.6
  fi
fi
if ! pactl info >/dev/null 2>&1; then
  pulseaudio -D --exit-idle-time=-1 2>/dev/null || pulseaudio --start 2>/dev/null || true
  sleep 0.5
fi
if pactl info >/dev/null 2>&1; then
  export SDL_AUDIODRIVER="${SDL_AUDIODRIVER:-pulse}"
  if ! pactl list short sinks 2>/dev/null | grep -q .; then
    pactl load-module module-null-sink sink_name=elite_smoke_null >/dev/null 2>&1 || true
    pactl set-default-sink elite_smoke_null >/dev/null 2>&1 || true
  fi
fi
TEST_DIR="$HOME/work/smoke-$(date +%Y%m%d-%H%M%S-%3N)"
mkdir -p "$TEST_DIR"
cp -f "$ROOT/EBOOT.PBP" "$TEST_DIR/"
echo 1 > "$TEST_DIR/smoke.flag"
EBOOT="$TEST_DIR/EBOOT.PBP"
"$EMULATOR" --graphics=software --windowed "$EBOOT" >/tmp/ppsspp-smoke.log 2>&1 &
PID=$!
cleanup() { kill "$PID" 2>/dev/null || true; wait "$PID" 2>/dev/null || true; }
trap cleanup EXIT
DEADLINE=$((SECONDS + 420))
REPORT="$TEST_DIR/performance-check.txt"
while (( SECONDS < DEADLINE )); do
  if [[ -f "$REPORT" ]] && grep -q '^RESULT ' "$REPORT"; then
    break
  fi
  if ! kill -0 "$PID" 2>/dev/null; then
    echo "PPSSPP exited early; see /tmp/ppsspp-smoke.log" >&2
    break
  fi
  sleep 0.25
done
for name in game input steering radio performance; do
  path="$TEST_DIR/${name}-check.txt"
  if [[ ! -f "$path" ]]; then
    echo "Missing $name report in $TEST_DIR" >&2
    tail -40 /tmp/ppsspp-smoke.log || true
    exit 1
  fi
  if ! grep -q '^RESULT 0 failures$' "$path" || grep -q '^FAIL ' "$path"; then
    echo "Failed $name checks; see $path" >&2
    cat "$path"
    exit 1
  fi
  echo "$name checks passed"
done
echo "Reports: $TEST_DIR"
