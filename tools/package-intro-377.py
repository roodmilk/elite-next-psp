"""Package the tested presentation title without altering unrelated game source."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile
from PIL import Image

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root/'outputs/releases/2.5.376/test-evidence.json'))
artifact = root/'outputs/intro377-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root/'VERSION').strip() == '2.5.377'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.377' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {}
allowed = {'intro.h', 'intro-film-review.h'}
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
for name in ('film', 'boot'):
    log = read(root/f'outputs/intro377-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M)
    run = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
    assert digest(run/'EBOOT.PBP') == sha
    checks[name] = len(re.findall(r'^PASS ', log, re.M))
    assert checks[name] >= (20 if name == 'film' else 2)
    runs[name] = str(run)
for p in (root/'EBOOT.PBP', root/'dist/ELITE-NEXT-PSP/EBOOT.PBP'):
    assert digest(p) in (baseline['sha256'], sha), 'Shared executable changed'
release = root/'outputs/releases/2.5.377/ELITE-NEXT-PSP'
release.mkdir(parents=True, exist_ok=True)
screens = release.parent/'screenshots'
screens.mkdir(exist_ok=True)
for p in sorted(Path(runs['film']).glob('intro-title-*.bmp')):
    with Image.open(p) as im:
        assert im.size == (480,272)
        im.save(screens/(p.stem+'.png'))
total = sum(checks.values())
evidence = f'''ELITE: NEXT 2.5.377 — CuLa presentation title
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/intro377-build.log

The opening credit reads CuLa Games Present. Cu has a native 16x16 red
square behind it and La a separate blue square. Both squares and their
lettering fade in together over 800ms, hold, and fade away before the next
credit. Existing cinematic, music timing, title-menu timing and saves retained.

{total} focused emulator checks pass on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Checks cover tile bounds/colours, transparent and half-opacity blending,
exact native-font lettering, fade timing, embedded film decoding, title
menu input, memory guards and ordinary startup. Native 480x272 title
captures inspected at seven points through the fade and credit transition.
The previous 376 intro suite passed as baseline.
All unrelated source hashes match 2.5.376.
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
    (folder/'BUILD-2.5.377.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.377\nFading CuLa intro title with red/blue letter tiles.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\nPhysical PSP testing pending.\n', encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n', encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.377',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',full_smoke='not repeated; inherited failures remain'),indent=2),encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.377.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.txt','.pbp')
        z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.377 — CuLa presentation title

Opening credit now reads “CuLa Games Present”, with a red 16×16 square behind “Cu” and a blue square behind “La”. Lettering and tiles share the 800ms fade-in and existing fade-out; the full title is centred at native 480×272. Cinematic, music, menu timing, portraits and saves unchanged.

{total} focused emulator checks pass, including exact glyphs, tile bounds, blending, fade timing, film/menu integration and normal boot. Seven native title captures inspected. Physical PSP testing remains pending; the same 15 inherited broad failures remain open. Details: BUILD-2.5.377.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.377. SHA256: {sha}. GitHub not updated: pull blocked by existing untracked music placeholders; autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p = root/name
    contents = read(p)
    if not contents.startswith('## 2.5.377'):
        p.write_text(entry+contents, encoding='utf-8')
print(json.dumps(dict(version='2.5.377',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
