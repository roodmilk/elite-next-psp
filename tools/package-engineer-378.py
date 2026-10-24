"""Package the tested Engineers hull preview, preserving all other game source."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile
from PIL import Image

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root/'outputs/releases/2.5.377/test-evidence.json'))
artifact = root/'outputs/engineer378-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root/'VERSION').strip() == '2.5.378'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.378' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {}
allowed = {'ui-modern.h', 'shipyard-console-review.h'}
for p in sorted((root/'src').rglob('*')):
    if not p.is_file():
        continue
    name = p.relative_to(root/'src').as_posix()
    assert p.stat().st_mtime <= artifact.stat().st_mtime, name
    sources[name] = digest(p)
    if name not in allowed:
        assert sources[name] == baseline['source_sha256'][name], f'Unrelated source changed: {name}'
assert set(sources) == set(baseline['source_sha256'])
checks, runs = {}, {}
for name, expected in (('yard',32), ('boot',2)):
    log = read(root/f'outputs/engineer378-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M)
    run = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
    assert digest(run/'EBOOT.PBP') == sha
    checks[name] = len(re.findall(r'^PASS ', log, re.M))
    assert checks[name] == expected, checks
    runs[name] = str(run)
for p in (root/'EBOOT.PBP', root/'dist/ELITE-NEXT-PSP/EBOOT.PBP'):
    assert digest(p) in (baseline['sha256'], sha), 'Shared executable changed'
release = root/'outputs/releases/2.5.378/ELITE-NEXT-PSP'
release.mkdir(parents=True, exist_ok=True)
screens = release.parent/'screenshots'
screens.mkdir(exist_ok=True)
for p in sorted(Path(runs['yard']).glob('engineer-*.bmp')):
    with Image.open(p) as im:
        assert im.size == (480,272)
        im.save(screens/(p.stem+'.png'))
evidence = f'''ELITE: NEXT 2.5.378 — Your own ship at Engineers
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/engineer378-build.log

The Engineers left-hand preview now renders the current commander's actual
ship using the same model and current paint as Shipyard. Both regular meshes
and authored Mega hull assemblies fit a bounded 246x64 aperture between the
existing diagnostic boxes. The model gently turns with the menu preview clock.
Repairs, refuelling, prices, controls and commander save formats unchanged.
Preserves the 377 CuLa title and all earlier portrait/menu/gameplay work.

34 focused emulator checks pass on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Includes reference pixel comparisons for all 22 actual hulls at three angles,
all paint finishes on regular and Mega hulls, drawing without commander data
changes, restored renderer state, repair/refuel input and exact quoted charges.
The 26 existing Shipyard checks and two normal startup checks also pass.
Five native 480x272 Engineers screenshots inspected, including painted ships.
Previous 377 Shipyard baseline: 26 PASS, zero failures.
All unrelated source hashes match 2.5.377. Embedded movie unchanged.
Physical PSP testing pending. Broad checks not repeated; the previously
documented 15 inherited game-core failures remain unresolved.
Git pull blocked by existing untracked music placeholders; autostash restored.
Mixed shared work preserved. Local only; GitHub not updated.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
Highest priority: physical PSP playtest, inherited reliability, planetary work.
'''
for folder in (root, root/'dist/ELITE-NEXT-PSP', release):
    shutil.copy2(artifact, folder/'EBOOT.PBP')
    assert digest(folder/'EBOOT.PBP') == sha
    (folder/'BUILD-2.5.378.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.378\nYour current ship design and paint in Engineers.\n34 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n', encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n', encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.378',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',full_smoke='not repeated; inherited failures remain'),indent=2),encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.378.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.txt','.pbp')
        z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.378 — Your own ship at Engineers

Engineers now shows the commander's actual hull and current paint using the same renderer as Shipyard, including all Mega ships. The model turns gently inside a native 246×64 aperture, clear of diagnostic labels. Repair/refuel behaviour and saves unchanged. Preserves the 377 fading CuLa title and earlier work.

34 focused emulator checks pass: six Engineers checks, 26 existing Shipyard checks and two startup checks. Actual hull pixels verified for all 22 designs at three angles and every paint finish on regular/Mega ships; repair/refuel input and prices verified. Five native 480×272 captures inspected. Physical PSP testing and 15 inherited broad failures remain open. Details: BUILD-2.5.378.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.378. SHA256: {sha}. GitHub not updated: pull blocked by existing untracked music placeholders; autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p = root/name
    contents = read(p)
    if not contents.startswith('## 2.5.378'):
        p.write_text(entry+contents, encoding='utf-8')
print(json.dumps(dict(version='2.5.378',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
