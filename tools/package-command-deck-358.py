from pathlib import Path
import hashlib, json, shutil, zipfile, struct

root = Path(__file__).resolve().parents[1]
artifact = root / 'outputs/deck358-release/EBOOT.PBP'
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
base = '68ED085740E86B135A29E8D25833AB6EC73B8BD0EC9B8831C95BAA065A7C5564'
sha = digest(artifact)
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.358' in data[offsets[0]:offsets[1]]
names = ['deck','controls-handbook','cargo-console','loadout-console','shipyard-console','ship-purchase','multiplayer-console']
counts = {}
for name in names:
    log = (root / f'outputs/deck358-release-{name}.log').read_text(encoding='utf-8-sig')
    assert 'RESULT 0 failures' in log and not any(s.startswith('FAIL ') for s in log.splitlines()), name
    counts[name] = sum(s.startswith('PASS ') for s in log.splitlines())
failures = lambda name: [s for s in (root / f'outputs/{name}.log').read_text(encoding='utf-8-sig').splitlines() if s.startswith('FAIL ')]
assert failures('deck358-release-core') == failures('deck358-baseline-core')
assert len(failures('deck358-release-core')) == 15
release = root / 'outputs/releases/2.5.358/ELITE-NEXT-PSP'
release.mkdir(parents=True, exist_ok=True)
readme = '''ELITE: NEXT 2.5.358 - Observation Lounge Command Deck

Built on local 2.5.357, including its Multiplayer console.
Replace ONLY EBOOT.PBP in your existing PSP/GAME game folder.
Keep your saves, settings, online.cfg and music. Save format remains V32.

Native 480x272 command deck with the original game font and logo.
Left/Right or L/R shoulders: change category. Up/Down: select service.
X: open. Circle: back. Existing availability and actions are retained.
Categories slide, buttons shine and briefly flash when opened.
Station artwork stays still; lights, stars and sun gently animate.

178 focused UI checks pass. Steering and radio checks report zero failures.
The same 15 inherited broad game checks still fail; this is not whole-game
certification. Physical PSP, suspend/resume and two-device testing is pending.
This local release has NOT been published to GitHub.
See BUILD-2.5.358.txt for detailed evidence and limitations.

Music folders sit beside EBOOT.PBP. Add your tracks to the station folder
and restart the game. This package includes no player music or personal files.
'''
evidence = f'''ELITE: NEXT 2.5.358 - Observation Lounge Command Deck
Built 2026-10-10 on verified local 2.5.357. Save format V32 unchanged.
Baseline EBOOT SHA256: {base}
Release EBOOT SHA256: {sha}
Release size: {artifact.stat().st_size} bytes

Source implementation: src/command-deck-console.h, src/deck-nav.h,
src/deck-ui.h, src/main.c, src/opening-runtime.h, src/tutorial-runtime.h.
Coordinates and behavior: docs/COMMAND-DECK-358.md.
Build: outputs/deck358-release/EBOOT.PBP
Build log: outputs/deck358-release-build.log

Exact release emulator checks:
''' + ''.join(f'- {k}: {v} PASS, zero failures. outputs/deck358-release-{k}.log\n' for k,v in counts.items()) + '''
178 focused checks total. Core release failure lines exactly match the
compiled pre-change baseline (15); steering and radio report zero failures.
Baseline: outputs/deck358-baseline-core.log
Final: outputs/deck358-release-core.log
The broad smoke input run was incomplete and exposed inherited/stale failures;
no claim of a fully green whole-game smoke test is made.

Native review: outputs/deck-review-20261010-104219-373
Captures inspected at 480x272: all five categories, poor/rich/mega hubs,
relay, flight, long labels, large balance and transition frames. Actual
framebuffer animation: command-deck-live.gif in that directory.
30-frame emulator render measurement: 12.091ms average. Not PSP hardware FPS.
Bounded static transition snapshots: 499200 bytes; backdrop cache: 226968 bytes.
Physical PSP, suspend/resume, two-device ad-hoc and memory headroom testing
remain pending. The unchanged Multiplayer transport's previous real server
tests are recorded in BUILD-2.5.357.txt, not claimed as rerun in this update.

Root/dist/versioned EBOOT copies are hash-identical when packaged.
No saves, personal music, credentials or online.cfg are included in the ZIP.
Git pull was attempted but GitHub DNS resolution failed; submodule update
also hit the existing Windows MSYS initialization restriction. No commit,
tag, push or GitHub Release is claimed. Preserve the mixed shared checkout.
Concurrent planetary work changed source after this EBOOT was built; it is
NOT part of this tested menu release. This artifact remains the tested 10:42
menu snapshot; later source edits must be rebuilt and independently verified.

Inherited core failures:
''' + '\n'.join(failures('deck358-release-core')) + '\n'
for target in [root, root/'dist/ELITE-NEXT-PSP', release]:
    target.mkdir(parents=True, exist_ok=True)
    if target != root:
        (target/'README.txt').write_text(readme, encoding='utf-8')
        (target/'SHA256.txt').write_text(f'{sha}  EBOOT.PBP\n', encoding='ascii')
    (target/'BUILD-2.5.358.txt').write_text(evidence, encoding='utf-8')
    destination = target/'EBOOT.PBP'
    if destination.exists() and target != release:
        assert digest(destination) in (base,sha), 'A newer shared release exists; do not overwrite it.'
    shutil.copy2(artifact, destination)
    assert digest(destination) == sha
for folder in (root/'dist/ELITE-NEXT-PSP/music').iterdir():
    if folder.is_dir():
        d = release/'music'/folder.name
        d.mkdir(parents=True, exist_ok=True)
        (d/'README.txt').write_text('Add your music tracks here, then restart the game.\n',encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.358.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file(): z.write(p, p.relative_to(release.parent))
with zipfile.ZipFile(archive,'r') as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
summary = f'\nVerified local EBOOT: `{sha}`. 178 focused UI checks pass; core has the same 15 baseline failures, steering/radio zero. Native captures: `outputs/deck-review-20261010-104219-373`. Release: `outputs/releases/2.5.358`. Concurrent planetary source changes after the 10:42 build are not included in this tested menu artifact. Local release only; no GitHub publication.\n\n'
p=root/'CLAUDE-HANDOFF.md'
s=p.read_text(encoding='utf-8-sig')
if sha not in s:
    pos=s.index('## 2.5.357')
    p.write_text(s[:pos]+summary+s[pos:],encoding='utf-8')
print(json.dumps({'version':'2.5.358','sha256':sha,'focused_checks':counts,'total':sum(counts.values()),'eboot':str(release/'EBOOT.PBP'),'zip':str(archive),'zip_sha256':digest(archive)},indent=2))
