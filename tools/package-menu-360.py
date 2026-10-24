"""Package only a menu EBOOT whose exact bytes passed every recorded review."""
from pathlib import Path
import hashlib
import json
import re
import shutil
import struct
import zipfile

root = Path(__file__).resolve().parents[1]
artifact = root/'outputs/menu360-candidate/EBOOT.PBP'
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
sha = digest(artifact)
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.360' in data[offsets[0]:offsets[1]]
counts, runs = {}, {}
for name in ('deck','multiplayer','loadout','cargo','shipyard','controls','boot'):
    log = (root/f'outputs/menu360-{name}.log').read_text(encoding='utf-8-sig')
    assert 'RESULT 0 failures' in log and not any(s.startswith('FAIL ') for s in log.splitlines()), name
    run = Path(re.search(r'^Review: (.+)$', log, re.M)[1].strip())
    assert digest(run/'EBOOT.PBP') == sha, f'{name} report belongs to an older EBOOT'
    counts[name] = sum(s.startswith('PASS ') for s in log.splitlines())
    runs[name] = str(run.relative_to(root))
deck_log = (root/'outputs/menu360-deck.log').read_text(encoding='utf-8-sig')
ms = float(re.search(r'INFO ([\d.]+) ms average native deck render', deck_log)[1])
baseline_ms = 12.112
reduction = (baseline_ms-ms)*100/baseline_ms
evidence = f'''ELITE: NEXT 2.5.360 - Responsive menus and ship paint themes
Built 2026-10-10 on tested local 2.5.359. V32 save format unchanged.
SHA256: {sha}
EBOOT size: {artifact.stat().st_size} bytes
Source/build: outputs/menu360-build; log outputs/menu360-build.log.
Details and scope: docs/MENU-RESPONSIVENESS-360.md.

Changes: immediate page openings without the snapshot replay, bright flash
or CRT scan bands; retained category slides and ambient animations. Removes
499200 static transition-buffer bytes; keeps the bounded 7 MiB boot heap.
Native art bypasses resampling; scaled art divides once per X sample rather
than per pixel. Sun shimmer sine is evaluated per row. Theme lookup is cached.
Command Deck, Multiplayer, Cargo, Loadout/Storage, Shipyards, Commander cards,
Controls chrome, decorator selection and GalacticNet navigation use current
ship paint roles. Shared headers/panels/footers and legacy menu text follow.
Warnings, item categories, portrait/world art and green connection status
retain their semantic colours. No gameplay or transport changes in this pass.

Exact-artifact focused checks:
'''+''.join(f'- {k}: {v} PASS; {runs[k]}\n' for k,v in counts.items())+f'''
Total: {sum(counts.values())} focused checks, zero failures.
Normal boot reaches a complete nonblank native 480x272 title after normal
audio/network initialization, without MEMBLOCK_ALLOC_FAILED in the log.
All sixteen finishes alter fourteen main screens; 42 native theme captures.
Native/scaled/clipped/transparent art matches the shipped sampler exactly.
Actual menu actions, Back, availability, pause, shop/loadout/cargo controls
and Multiplayer fixtures pass. Physical PSP and two-device tests are pending.

Performance: {ms:.3f} ms average Command Deck render, 30 frames in PPSSPP,
versus 12.112 ms on 2.5.359 ({reduction:.1f}% less CPU render time).
This measures emulator drawing time, not guaranteed PSP hardware FPS.
Other menu timing samples:
'''+ '\n'.join(s for s in deck_log.splitlines() if s.startswith('MENU '))+'''

Retains 2.5.359 planetary foundation: 1038 ground worlds, 342 gas destinations,
18 ground types/four gas families, 3898 regions, V32 compatibility. Its 397
focused checks and inherited core comparison are recorded in BUILD-2.5.359.txt,
not claimed as rerun here. Same 15 inherited core failures remain open; the
broad smoke input fixture is incomplete. This is not whole-game certification.
Specialist planet art/geometry, sparse fern/ice regions, Lave II performance,
physical PSP suspend/audio and two-device Wi-Fi remain continuation work.

Root/dist/versioned/ZIP EBOOT copies are identical. Package excludes player
music, saves, preferences, online.cfg and credentials. Replace only EBOOT.PBP.
Local release only: pull blocked by existing untracked music placeholders;
mixed shared checkout preserved. No commit/tag/push/GitHub release is claimed.
'''
readme = f'''ELITE: NEXT 2.5.360

Replace ONLY EBOOT.PBP. Keep your saves, music and settings.
Faster menu drawing, immediate selections without the flashing transition,
and current ship paint themes across the main consoles, Cargo and Loadout.
Retains the animated Command Deck, Multiplayer and procedural planets.
The startup memory fix is included. Save format stays V32.

{sum(counts.values())} focused checks pass, including normal emulator boot.
Physical PSP startup/responsiveness and your paint colour need a playtest.
Inherited broad failures and unfinished planet art remain; see BUILD files.
Local build only; GitHub is not updated.
'''
release = root/'outputs/releases/2.5.360/ELITE-NEXT-PSP'
previous = 'AAD49EB1F9FB64714D263955F4AB522E4D60FDDD811045E2E930FAA173529AF3'
for target in (root, root/'dist/ELITE-NEXT-PSP', release):
    target.mkdir(parents=True, exist_ok=True)
    dest = target/'EBOOT.PBP'
    if dest.exists(): assert digest(dest) in (previous,sha), 'Shared release changed; do not overwrite.'
    shutil.copy2(artifact,dest)
    (target/'BUILD-2.5.360.txt').write_text(evidence, encoding='utf-8')
    if target != root:
        (target/'README.txt').write_text(readme, encoding='utf-8')
        (target/'SHA256.txt').write_text(f'{sha}  EBOOT.PBP\n',encoding='ascii')
        shutil.copy2(root/'BUILD-2.5.359.txt',target/'BUILD-2.5.359.txt')
for folder in (root/'music').iterdir():
    if folder.is_dir():
        dest=release/'music'/folder.name;dest.mkdir(parents=True,exist_ok=True)
        (dest/'README.txt').write_text('Add your tracks here, then restart the game.\n',encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.360.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():
            assert p.suffix.lower() in ('.txt','.pbp'),p
            z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.360 — Responsive menus and matching ship themes

Menu selections now reveal their destination immediately without the full-screen replay, bright flash or CRT scan bands. Category slides and ambient animation remain. Native art avoids resampling, scaled art shares X samples, sun shimmer avoids per-pixel sine, and paint-theme lookup is cached. Command Deck drawing averages {ms:.3f} ms in PPSSPP versus 12.112 ms before; hardware FPS remains to be tested.

Command Deck, Multiplayer, Cargo, Ship Loadout/Item Storage, Shipyards, Commander cards, Controls chrome, decorator selection and GalacticNet navigation follow the current hull's PIMP-MY-SHIP finish. All sixteen finishes are verified across fourteen main screens. {sum(counts.values())} focused checks pass on the exact release, including normal boot and all changed console functions. Retains the 2.5.359 planetary foundation and 2.5.358/357 features; V32 saves remain compatible. Evidence: BUILD-2.5.360.txt; implementation: docs/MENU-RESPONSIVENESS-360.md. Release: outputs/releases/2.5.360. SHA256: {sha}.

Highest-priority continuation: player PSP boot/menu/paint test, Lave region/FPS feedback, inherited core failures, sparse biome props and physical suspend/audio/two-device checks. No new geography or art expansion before feedback. Local release only; GitHub is not updated. Preserve mixed shared work and player music.

'''
for name in ('CHANGELOG.md','README.md','CLAUDE-HANDOFF.md'):
    p=root/name;s=p.read_text(encoding='utf-8-sig')
    if not s.startswith('## 2.5.360'):p.write_text(entry+s,encoding='utf-8')
print(json.dumps({'version':'2.5.360','sha256':sha,'checks':counts,'total':sum(counts.values()),'deck_render_ms':ms,'eboot':str(release/'EBOOT.PBP'),'zip':str(archive)},indent=2))
