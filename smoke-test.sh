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
# PPSSPP SDL build in this environment needs a PulseAudio server.
if ! pactl info >/dev/null 2>&1; then
  pulseaudio -D --exit-idle-time=-1 2>/dev/null || pulseaudio --start 2>/dev/null || true
  sleep 0.5
fi
TEST_DIR="$HOME/work/smoke-$(date +%Y%m%d-%H%M%S-%3N)"
mkdir -p "$TEST_DIR"
cp -f "$ROOT/EBOOT.PBP" "$TEST_DIR/"
echo 1 > "$TEST_DIR/smoke.flag"
EBOOT="$TEST_DIR/EBOOT.PBP"
export DISPLAY="${DISPLAY:-:1}"
"$EMULATOR" --graphics=software --windowed "$EBOOT" >/tmp/ppsspp-smoke.log 2>&1 &
PID=$!
cleanup() { kill "$PID" 2>/dev/null || true; wait "$PID" 2>/dev/null || true; }
trap cleanup EXIT
DEADLINE=$((SECONDS + 120))
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
