# ELITE: NEXT 2.5.65 smoke evidence

Run date: 2026-09-23

## Tested artifact

- EBOOT: `dist/ELITE-NEXT-PSP/EBOOT.PBP`
- SHA-256: `FC1890D3AA1FC9C00ABD77C99FED426F443E83BA680FD947D8020C9639BD544F`
- Source commit: `c99c583`
- Package: `dist/ELITE-NEXT-PSP-v2.5.65.zip`

## PPSSPP smoke result

The current-main smoke harness completed with zero reported failures in all five suites:

- game
- input
- steering
- radio
- performance

The report directory was `work/smoke-20260923-123738-832` on the development machine.

This is emulator evidence for the exact EBOOT hash. It does not certify physical PSP suspend/resume, memory-stick I/O, audio hardware fidelity, or the complete v0.7 Galaxy Beta content and visual acceptance matrix.
