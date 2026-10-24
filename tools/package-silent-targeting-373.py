"""Package the checked silent-targeting executable, preserving the prior release."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root/'outputs/releases/2.5.372/test-evidence.json'))
artifact = root/'outputs/silent373-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root/'VERSION').strip() == '2.5.373'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.373' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {}
for name, previous in baseline['source_sha256'].items():
    p = root/'src'/name
    assert p.stat().st_mtime <= artifact.stat().st_mtime, name
    sources[name] = digest(p)
    if name != 'flight-extras.h':
        assert sources[name] == previous, f'Unrelated source changed: {name}'
text = read(root/'src/flight-extras.h')
sources['flight-extras.h'] = digest(root/'src/flight-extras.h')
assert (root/'src/flight-extras.h').stat().st_mtime <= artifact.stat().st_mtime
assert 'No contacts in that band.' not in text
assert text.count('if(!n)return; /* Empty targeting categories stay silent. */') == 2
checks, runs = {}, {}
for name, expected in (('comms',66),('boot',2)):
    log = read(root/f'outputs/silent373-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M), name
    run = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
    assert digest(run/'EBOOT.PBP') == sha
    checks[name] = len(re.findall(r'^PASS ', log, re.M))
    assert checks[name] == expected, (name, checks[name])
    runs[name] = str(run)
release = root/'outputs/releases/2.5.373/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP', root/'dist/ELITE-NEXT-PSP/EBOOT.PBP', release/'EBOOT.PBP'):
    if p.exists():
        assert digest(p) in (baseline['sha256'], sha), 'Shared release changed'
evidence = f'''ELITE: NEXT 2.5.373 — Silent empty targeting categories
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/silent373-build.log

Removed "No contacts in that band" from category switching and list cycling.
Both empty-category branches now return silently, retaining the selected target
and any current message. Category navigation remains predictable.
All baseline-tracked sources unchanged except flight-extras.h. Preserves the
2.5.372 Codex fill, reached checkpoint lights and all previous menu fixes.

68 focused emulator checks pass on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
The two silent branches and absence of the removed message verified in source.
Physical PSP testing pending. Full smoke not repeated; previously documented
broad gameplay/input failures remain unresolved. Not whole-game certification.
Git pull blocked by existing untracked music placeholders; autostash restored.
Mixed shared work preserved. Local release only; GitHub not updated.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
Highest-priority remaining work: hardware playtest, inherited reliability,
then planetary work.
'''
release.mkdir(parents=True, exist_ok=True)
for folder in (root, root/'dist/ELITE-NEXT-PSP', release):
    shutil.copy2(artifact, folder/'EBOOT.PBP')
    assert digest(folder/'EBOOT.PBP') == sha
    (folder/'BUILD-2.5.373.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.373\nSilent empty targeting categories.\n68 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n', encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n', encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.373',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',full_smoke='not repeated; inherited failures remain'),indent=2),encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.373.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.txt','.pbp')
        z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.373 — Silent empty targeting categories

Removed the computer's “No contacts in that band” message from both category switching and list cycling. Empty bands remain selectable, keep the current target and leave any active message alone. Preserves the 2.5.372 Codex progress fill and checkpoint illumination.

68 focused emulator checks pass: 66 Comms Panel and two normal boot checks. Both silent branches verified in source; all other baseline-tracked sources and the embedded cinematic are unchanged. Physical PSP testing and inherited broad failures remain open. Details: BUILD-2.5.373.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.373. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p = root/name
    text = read(p)
    if not text.startswith('## 2.5.373'):
        p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.373',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
