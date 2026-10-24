"""Package only the exact optimized, reviewed staged-contract build."""
from pathlib import Path
import difflib, hashlib, json, re, shutil, struct, zipfile
from PIL import Image

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root/'outputs/releases/2.5.387/test-evidence.json'))
artifact = root/'outputs/mission388-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root/'VERSION').strip() == '2.5.388'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.388' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {p.relative_to(root/'src').as_posix(): digest(p)
           for p in sorted((root/'src').rglob('*')) if p.is_file()}
allowed = set('game.h game.c sectors.h journey.h docking.h surface-activities.h space-racing.h ui-modern.h campaign-ui.h flight-extras.h mission-network-tests.h main.c mission-reward-card.h space-racing-ui.h audio.h audio-intro.h radio-tests.h intro-film-review.h space-racing-review.h comms-console.h narrative-nav.h campaign-tests.h crime-record-tests.h multislot-tests.h observatory-tests.h opening-save-tests.h rift-tests.h social-tests.h station-save-tests.h tutorial-save-tests.h'.split())
changed = []
for name, hashed in sources.items():
    assert (root/'src'/name).stat().st_mtime <= artifact.stat().st_mtime, f'Unbuilt source: {name}'
    if hashed != baseline['source_sha256'].get(name):
        assert name in allowed or name.startswith('mission-expansion-') or name == 'mission-expansion.h', f'Unrelated source changed: {name}'
        changed.append(name)
assert not (set(baseline['source_sha256'])-set(sources)), 'Production source removed'
log = read(root/'outputs/mission388-final-tests.log')
suite = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
checks, runs, reports = {}, {}, {}
for case in json.loads(read(suite/'results.json')):
    name = case['test']; run = suite/name; report = read(Path(case['report']))
    assert digest(run/'EBOOT.PBP') == sha
    if name not in ('core-check', 'profile-law-review'):
        assert 'RESULT 0 failures' in report and not re.search(r'^FAIL ', report, re.M), name
    checks[name] = len(re.findall(r'^PASS ', report, re.M))
    runs[name], reports[name] = str(run), report
backend = read(suite/'mission-expansion-review/mission-expansion-backend.txt')
assert 'RESULT 0 failures' in backend and not re.search(r'^FAIL ', backend, re.M)
assert 'INFO completed gameplay stages 155 / 155' in backend
checks['staged-gameplay'] = len(re.findall(r'^PASS ', backend, re.M))
assert checks['mission-expansion-review'] >= 8 and checks['space-racing-review'] >= 72
assert checks['mission-reward-review'] == 37 and checks['intro-film-review'] == 37
assert checks['opening-check'] == 345
assert 'source 48000 Hz; output 44100 Hz' in reports['space-racing-review']
assert 'Secret Agent Rock.mp3' in reports['space-racing-review']
assert 'startup discards exactly twenty-three seconds' in reports['intro-film-review']
core_fail = re.findall(r'^FAIL .+$', reports['core-check'], re.M)
assert set(core_fail).issubset(baseline['core_inherited_failures']), 'New core regression'
baseline_input_log = read(root/'outputs/mission388-baseline-input.log')
baseline_input_run = Path(re.search(r'^Review:\s*(.+)$', baseline_input_log, re.M)[1].strip())
assert digest(baseline_input_run/'profile-law-review/EBOOT.PBP') == baseline['sha256']
baseline_input_report = read(baseline_input_run/'profile-law-review/input-check.txt')
input_fail = re.findall(r'^FAIL .+$', reports['profile-law-review'], re.M)
baseline_input_fail = re.findall(r'^FAIL .+$', baseline_input_report, re.M)
assert set(input_fail).issubset(baseline_input_fail), 'New input regression'
assert re.search(r'^RESULT \d+ failures$', reports['profile-law-review'], re.M)
for name in ('radio', 'steering'):
    report = read(suite/'core-check'/(name+'-check.txt'))
    assert 'RESULT 0 failures' in report and not re.search(r'^FAIL ', report, re.M)
    checks[name] = len(re.findall(r'^PASS ', report, re.M))
boot_log = read(root/'outputs/mission388-boot.log')
assert 'RESULT 0 failures' in boot_log
boot = Path(re.search(r'^Review:\s*(.+)$', boot_log, re.M)[1].strip())
assert digest(boot/'EBOOT.PBP') == sha
checks['boot'] = len(re.findall(r'^PASS ', boot_log, re.M)); runs['boot'] = str(boot)
release = root/'outputs/releases/2.5.388/ELITE-NEXT-PSP'
release.mkdir(parents=True, exist_ok=True)
screens = release.parent/'screenshots'; screens.mkdir(exist_ok=True)
patch = []
for name in changed:
    before = root/'outputs/mission-expansion-388-baseline/src'/name
    after = root/'src'/name
    if before.exists(): assert digest(before) == baseline['source_sha256'][name]
    patch.extend(difflib.unified_diff(read(before).splitlines(True) if before.exists() else [],
                                    read(after).splitlines(True),
                                    fromfile='a/src/'+name if before.exists() else '/dev/null',
                                    tofile='b/src/'+name))
(release.parent/'SOURCE-CHANGES.patch').write_text(''.join(patch), encoding='utf-8')
for name in ('mission-expansion-review', 'space-racing-review', 'intro-film-review'):
    for p in (suite/name).glob('*.bmp'):
        image = Image.open(p); assert image.size == (480, 272)
        image.save(screens/(p.stem+'.png'))
focused = sum(v for n, v in checks.items() if n not in ('core-check', 'profile-law-review'))
feature = '''Forty additional contracts with 155 real objective stages, branching decisions,
animated paused contact updates and reviewable contact records. Existing
contract families and winding arcade races remain. Tasks use actual combat,
escort ships, marked salvage, extraction/refining, cargo purchases, observation,
surface sites and scans, discoveries, anomalies and championship races.
Progress, required cargo, choices and ship health persist in V33 saves;
V1-V32 imports remain supported. New saves need this version or newer.

Title music uses Elite Exploration independently of saved radio preferences,
skips exactly 23 seconds (2.5.384 timing), then fades in over one second.
Rock FM and races use music/Sol Rock beside EBOOT.PBP. Actual Secret Agent
Rock.mp3 (48 kHz, large embedded artwork) now decodes in emulator testing.
Metadata skipping and modern PSP MP3 decoder support fix that file's failure.
No MP3s, saves or private settings are included.
'''
evidence = f'''ELITE: NEXT 2.5.388 - Staged contracts and music repair
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Built on local 2.5.387; unrelated production source hashes preserved.

{feature}
Validation: {focused} focused emulator checks pass, including all 155 objective
stages through actual handlers, save/reload at each stage, forty completion
payments, alternate decisions, cargo provenance, traffic saturation and V32
import. All forty new families appear on actual boards. All 155 stage cards
and forty station-board views captured at native 480x272.
Core: {checks['core-check']} pass; {len(core_fail)} inherited failures remain.
Input: {checks['profile-law-review']} pass; {len(input_fail)} inherited failures
remain, independently reproduced on the prior 2.5.387 EBOOT. No additional
core or input failures. Full reports: {suite}
Normal boot: {boot}
Physical PSP boot, MP3 playback, controls and performance need playtesting.
Local release only; GitHub unchanged. Shared unfinished work was preserved.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
'''
for folder in (root, root/'dist/ELITE-NEXT-PSP', release):
    folder.mkdir(parents=True, exist_ok=True)
    if (folder/'EBOOT.PBP').exists(): assert digest(folder/'EBOOT.PBP') in (baseline['sha256'], sha)
    shutil.copy2(artifact, folder/'EBOOT.PBP'); assert digest(folder/'EBOOT.PBP') == sha
    (folder/'BUILD-2.5.388.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.388\nForty staged contracts and music repairs.\nReplace ONLY EBOOT.PBP. Keep saves, settings and music.\nRock FM/races: music/Sol Rock. Intro: music/Elite Exploration.\nV33 saves require this version or newer. Physical PSP confirmation pending.\n', encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n', encoding='ascii')
record = dict(version='2.5.388', sha256=sha, film_sha256=film_sha, checks=checks,
              runs=runs, source_sha256=sources, changed_sources=changed,
              core_inherited_failures=core_fail, input_inherited_failures=input_fail,
              baseline_input_run=str(baseline_input_run), physical_psp='pending', git='local only; shared work preserved')
(release.parent/'test-evidence.json').write_text(json.dumps(record, indent=2), encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.388.zip'
with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.pbp', '.txt'); z.write(p, p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.388 — Staged contracts and music repair

{feature}
Controls and full catalogue: docs/STAGED-CONTRACTS-388.md and docs/MISSION-STAGES-388.md.
Validation: {focused} focused checks pass; {checks['core-check']} core checks pass with {len(core_fail)} inherited core failures. Input has {len(input_fail)} inherited failures independently reproduced on 2.5.387; no additional core/input failures. Exact user MP3s tested; native captures inspected. Optimized EBOOT SHA256 {sha}. Physical PSP playtest pending. Local release; GitHub unchanged.

Highest-priority remaining work: physical PSP mission/music/cold-boot playtest, difficulty tuning, inherited reliability failures, authored portraits and paused planetary expansion.

'''
for name in ('README.md', 'CHANGELOG.md', 'CLAUDE-HANDOFF.md'):
    p = root/name; text = read(p)
    if not text.startswith('## 2.5.388'): p.write_text(entry+text, encoding='utf-8')
for name in ('docs/DESIGN-BIBLE.md', 'docs/DESIGN-BIBLE-2.0.md'):
    p = root/name; text = read(p)
    if not text.startswith('## Staged contracts — 2.5.388'):
        p.write_text('## Staged contracts — 2.5.388\n\n'+feature+'\nSee STAGED-CONTRACTS-388.md and MISSION-STAGES-388.md.\n\n'+text, encoding='utf-8')
print(json.dumps(dict(version='2.5.388', sha256=sha, focused_passes=focused, checks=checks, eboot=str(release/'EBOOT.PBP'), zip=str(archive)), indent=2))
