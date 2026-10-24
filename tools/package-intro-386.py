"""Package the reviewed intro timing correction without changing other systems."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile
from PIL import Image

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root / 'outputs/releases/2.5.385/test-evidence.json'))
artifact = root / 'outputs/intro386-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root / 'VERSION').strip() == '2.5.386'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.386' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {}
for p in sorted((root / 'src').rglob('*')):
    if not p.is_file():
        continue
    name = p.relative_to(root / 'src').as_posix()
    sources[name] = digest(p)
    assert p.stat().st_mtime <= artifact.stat().st_mtime, f'Unbuilt source: {name}'
    if name not in {'audio-intro.h', 'intro-film-review.h'}:
        assert sources[name] == baseline['source_sha256'][name], f'Unrelated change: {name}'
assert set(sources) == set(baseline['source_sha256'])
assert '#define AUDIO_INTRO_SKIP_SECONDS 21' in read(root / 'src/audio-intro.h')
checks, runs = {}, {}
for name in ('intro', 'boot'):
    log = read(root / f'outputs/intro386-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M)
    run = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
    assert digest(run / 'EBOOT.PBP') == sha
    checks[name] = len(re.findall(r'^PASS ', log, re.M))
    runs[name] = str(run)
    if name == 'intro':
        assert checks[name] == 37
        assert 'startup discards exactly twenty-one seconds of decoded music before playback' in log
assert checks['boot'] == 2
release = root / 'outputs/releases/2.5.386/ELITE-NEXT-PSP'
release.mkdir(parents=True, exist_ok=True)
screens = release.parent / 'screenshots'
screens.mkdir(exist_ok=True)
for name, directory in runs.items():
    for p in Path(directory).glob('*.bmp'):
        im = Image.open(p)
        assert im.size == (480, 272)
        im.save(screens / (name + '-' + p.stem + '.png'))
evidence = f'''ELITE: NEXT 2.5.386 - Intro music timing correction
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)

Restores the last two seconds removed from the Elite Exploration intro song:
playback begins 21 seconds into the track, instead of 23. Earlier skips and
the one-second fade-in remain. All other production source hashes match
2.5.385, including racing and the physical-PSP display-startup repair.

Fresh validation: 37 intro checks with a real MP3 and 2 normal-boot checks
pass against this exact optimized EBOOT. Exact decoded skip samples,
one-second fade, movie coexistence and restart behavior are verified.
Unchanged systems retain the 2.5.385 regression baseline: 548 focused passes
and 664 core passes with 15 inherited failures; not rerun for this audio edit.
Physical PSP confirmation remains pending.
Intro review: {runs['intro']}
Normal boot: {runs['boot']}
Build: outputs/intro386-build.log

Local release only. GitHub remains unchanged following the earlier automatic
approval rejection of pull/rebase on the mixed shared tree. Replace ONLY
EBOOT.PBP; retain saves, settings and music. No player music is distributed.
'''
for folder in (root, root / 'dist/ELITE-NEXT-PSP', release):
    folder.mkdir(parents=True, exist_ok=True)
    if (folder / 'EBOOT.PBP').exists():
        assert digest(folder / 'EBOOT.PBP') in (baseline['sha256'], sha)
    shutil.copy2(artifact, folder / 'EBOOT.PBP')
    assert digest(folder / 'EBOOT.PBP') == sha
    (folder / 'BUILD-2.5.386.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder / 'README.txt').write_text('ELITE: NEXT 2.5.386\nIntro music starts at 21 seconds instead of 23; one-second fade retained.\nLatest racing and all other features retained. Emulator tested; PSP confirmation pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n', encoding='utf-8')
        (folder / 'SHA256.txt').write_text(sha + '  EBOOT.PBP\n', encoding='ascii')
record = dict(version='2.5.386', sha256=sha, film_sha256=film_sha, checks=checks,
              runs=runs, source_sha256=sources,
              core_inherited_failures=baseline['core_inherited_failures'],
              retained_regression_baseline='2.5.385; unchanged systems, not rerun',
              physical_psp='pending', git='local only; earlier shared-tree pull/rebase rejection')
(release.parent / 'test-evidence.json').write_text(json.dumps(record, indent=2), encoding='utf-8')
archive = release.parent / 'ELITE-NEXT-PSP-v2.5.386.zip'
with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.pbp', '.txt')
        z.write(p, p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = '''## 2.5.386 — Restore two seconds of intro music

Elite Exploration now starts 21 seconds into the intro song, restoring only
the most recent two-second cut. The one-second fade and all 2.5.385 features,
including racing and the display-startup repair, remain unchanged.

Validation: 37 real-MP3 intro checks and 2 normal-boot checks pass on the exact
optimized EBOOT. All other production source hashes match 2.5.385. That
release's broader regression results are retained, not newly rerun; the same
15 inherited failures remain unresolved. Physical PSP confirmation is pending.
Local release only; GitHub unchanged after the earlier shared-tree approval rejection.

Highest-priority remaining work: physical PSP cold-boot/race playtest, then
inherited reliability failures, authored portraits and paused planetary expansion.

'''
for name in ('README.md', 'CHANGELOG.md', 'CLAUDE-HANDOFF.md'):
    p = root / name
    text = read(p)
    if not text.startswith('## 2.5.386'):
        p.write_text(entry + text, encoding='utf-8')
note = '## Intro music timing — 2.5.386\n\nElite Exploration starts at 21 seconds with its existing one-second fade.\nRestores only the latest two-second cut; earlier timing edits and cinematic\nare unchanged. Exact decoder samples and normal boot pass emulator checks.\n\n'
for name in ('docs/DESIGN-BIBLE.md', 'docs/DESIGN-BIBLE-2.0.md'):
    p = root / name
    text = read(p)
    if not text.startswith('## Intro music timing — 2.5.386'):
        p.write_text(note + text, encoding='utf-8')
print(json.dumps(dict(version='2.5.386', sha256=sha, fresh_checks=checks,
                      eboot=str(release / 'EBOOT.PBP'), zip=str(archive)), indent=2))
