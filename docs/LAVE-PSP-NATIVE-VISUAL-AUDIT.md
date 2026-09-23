# Lave PSP-native visual audit

Scope: Lave system `7`, native PSP `480x272`, normal and high-contrast review
where a capture exists. This audit deliberately does not promote new runtime
art or change gameplay/layout/hotspot code.

## Acceptance matrix

| Surface | Native contract | Lave evidence | Result | Required next action |
|---|---|---|---|---|
| Cockpit / command deck | 480x272 direct framebuffer; menu ship is clipped to a native viewport | Source uses `W=480`, `H=272` and integer pixel drawing; no Lave cockpit runtime frame is available | `BLOCKED` | Capture Lave boot/deck from a successfully booting PPSSPP run |
| Station rooms | 340x168 scene inside 480x272 shell; native 1x cue assets | Lave arrivals normal/contrast frames plus cue-kit normal/contrast proof sheets | `PARTIAL` | Station must approve anchors; Lave still needs an integrated seven-room runtime capture |
| Bar / canteen | Native authored room must be selected by a deterministic Lave identity | `second-shift-reference-native.png` is 480x272, but its runtime review is Reorte H0 (`system 39`), not Lave | `FAIL for Lave integration` | Author/select a Lave-specific canteen identity; do not silently reuse the Reorte exception |
| Ship / loadout | Native 1x slot chrome and hull stamp | `43f59d4` adds direct-pixel `loadout_hull_stamp`; no runtime frame yet | `PASS candidate` | Capture loadout normal and high contrast once PPSSPP boots |
| Targeting computer | Native CRT glass, text, bezel and scope pixels | `43f59d4` adds direct-pixel `targeting_scope_stamp`; no runtime frame yet | `PASS candidate` | Capture targeting with station, planet and hostile rows on Lave |
| Planet approach | Integer projection and native framebuffer path | No Lave approach frame in the available evidence set | `BLOCKED` | Capture approach normal/high contrast; check HUD/body identity at native size |
| Walking / station crawl | Native room viewport and fixed right options panel | Lave arrivals proof exists, but no Lave walk runtime capture | `BLOCKED` | Capture arrivals, canteen, shop, cargo, guild, clinic and customs walk states |
| High contrast | Role-collapsed palette must preserve focal silhouette and action cues | Cue-kit contrast proof and Lave arrivals contrast frame pass visual bounds | `PARTIAL` | Re-run the same normal/contrast capture pair for loadout, targeting, approach and walking |

## Native proof inventory

All files below are exact `480x272` captures or native proof sheets. Hashes are
SHA-256 and are recorded so later runtime captures cannot be mistaken for
these art-only candidates.

| File | SHA-256 |
|---|---|
| `assets/preview/lave-audit/lave-arrivals-normal.bmp` | `DCB3F2295AB3D2B43DE59C0995C8A56B3FF9DBFAF263350749D352537616102B` |
| `assets/preview/lave-audit/lave-arrivals-contrast.bmp` | `D627CAC2AB9BB6A6D9FD2459BCF762BE141D26ADE225C8D3CF899FEBDD82E117` |
| `assets/preview/lave-audit/second-shift-reference-native.png` | `D38A243947D36FDEB76DA4E37A8096515AF8E9DB66FF04DD425FD7CE84F963E0` |
| `assets/preview/lave-audit/lave-outfitting-candidate.bmp` | `2F1C3C1BAB6226541F041A3BCF10830F377B50A1218F08E7CA46A6631B416CCF` |
| `assets/preview/lave-audit/station-cue-kit-normal-native.png` | `B9219F49B4E65517FE66E9D4BB2389059808E49CB3133F87361A9A63460D43F0` |
| `assets/preview/lave-audit/station-cue-kit-contrast-native.png` | `CAD78DAA1B80056EA4031320F51DBD8E7F615FD07C4708B6478F7DD31387C881` |

## Blocker

The bounded PPSSPP run was attempted with the shared emulator. It terminated
before boot and produced no `game-check.txt`; the smoke harness reported:

```text
Missing game report in work/smoke-20260923-120140-696
```

Therefore this audit does not claim runtime, frame-time, or integrated visual
certification. The next useful step is to repair the emulator/package launch
path, then capture the seven Lave surfaces from the same EBOOT and compare the
normal/high-contrast pairs at exact native size.
