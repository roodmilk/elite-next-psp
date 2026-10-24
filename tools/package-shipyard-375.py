"""Package the dealer-based shipyard fix, preserving the previous game source."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root/'outputs/releases/2.5.374/test-evidence.json'))
artifact = root/'outputs/shipyard375-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root/'VERSION').strip() == '2.5.375'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.375' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {}
allowed={'shipyard-console.h','shipyard-console-review.h'}
for name in sorted(set(baseline['source_sha256'])|allowed):
    previous=baseline['source_sha256'].get(name)
    p = root/'src'/name
    assert p.stat().st_mtime <= artifact.stat().st_mtime, name
    sources[name] = digest(p)
    if previous and name not in allowed:
        assert sources[name] == previous, f'Unrelated source changed: {name}'
assert 'int mega=mega_yard_stock_count(&game)>0;' in read(root/'src/shipyard-console.h')
checks, runs = {}, {}
for name, expected in (('yard',26),('boot',2)):
    log = read(root/f'outputs/shipyard375-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M), name
    run = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
    assert digest(run/'EBOOT.PBP') == sha
    checks[name] = len(re.findall(r'^PASS ', log, re.M))
    assert checks[name] == expected, (name, checks[name])
    runs[name] = str(run)
release = root/'outputs/releases/2.5.375/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP', root/'dist/ELITE-NEXT-PSP/EBOOT.PBP', release/'EBOOT.PBP'):
    if p.exists():
        assert digest(p) in (baseline['sha256'], sha), 'Shared release changed'
evidence = f'''ELITE: NEXT 2.5.375 — Shipyard identity owns its layout
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/shipyard375-build.log

The dealer's mega-stock capability selects the showroom presentation. Mega
shipyards retain the Crown Grand layout, dealer name and showroom when browsing
basic or capital ships, in stats or description mode. This includes Lave's full
mega showroom even though Lave's station class differs. Ordinary dealers retain
Orbit Motors and regular stock, regardless of the commander's current hull.
Names, artwork, local stock lists, exchange calculations and purchase actions
unchanged. Preserves the 2.5.374 conversation ownership correction and prior fixes.

28 focused emulator checks pass on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
The shipyard suite includes all 22 Lave offers, both reading modes, regular and
capital ownership, regional Mega basic/capital transitions, ordinary stock across
the galaxy, named dealer identity/art/stock, clipping, prices, refunds, purchases
and delivery-card exit. Native 480x272 screenshots inspected.
The prior release's 22 shipyard checks also passed as a baseline.
Physical PSP testing pending. Full smoke not repeated; the previously documented
15 inherited game-core failures and extended input issues remain unresolved.
No commander save format, existing game source or embedded movie changes.
Git pull blocked by existing untracked music placeholders; autostash restored.
Mixed shared work preserved. Local only; GitHub not updated.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
Highest-priority remaining work: hardware playtest, inherited reliability,
then planetary work.
'''
release.mkdir(parents=True, exist_ok=True)
for folder in (root, root/'dist/ELITE-NEXT-PSP', release):
    shutil.copy2(artifact, folder/'EBOOT.PBP')
    assert digest(folder/'EBOOT.PBP') == sha
    (folder/'BUILD-2.5.375.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.375\nConsistent Mega and regular shipyard layouts.\n28 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n', encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n', encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.375',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',full_smoke='not repeated; inherited failures remain'),indent=2),encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.375.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.txt','.pbp')
        z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.375 — Shipyard identity owns its layout

Mega shipyards keep their premium layout for every listed hull, including basic ships. The dealer's mega-stock capability now chooses the layout; the highlighted ship does not. Lave's complete mega showroom also stays premium throughout its 22-ship list. Ordinary dealers retain their regular layout and regular stock, including when the commander owns a capital ship. Dealer names, pictures, stock lists, prices and purchases are unchanged. Preserves the 2.5.374 conversation ownership fix.

28 focused emulator checks pass: 26 shipyard checks and two startup checks. Native 480×272 captures inspected for Lave basic/capital, regional Mega basic/capital and ordinary dealers. Physical PSP testing and inherited broad failures remain open. Details: BUILD-2.5.375.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.375. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p = root/name
    text = read(p)
    if not text.startswith('## 2.5.375'):
        p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.375',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
