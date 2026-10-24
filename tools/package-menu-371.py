"""Package the exact executable checked by the focused native menu suites."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/menu371-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.371'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.371' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.370/test-evidence.json'))
assert film_sha==baseline['film_sha256']
checks={};runs={}
for name,count in (('fixes',17),('cargo',17),('loadout',24),('comms',66),('profile',21),('deck',51),('boot',2)):
    text=read(root/f'outputs/menu371-{name}.log')
    assert 'RESULT 0 failures' in text and not re.search(r'^FAIL ',text,re.M),name
    run=Path(re.search(r'^Review:\s*(.+)$',text,re.M)[1].strip())
    assert digest(run/'EBOOT.PBP')==sha,name
    checks[name]=len(re.findall(r'^PASS ',text,re.M));assert checks[name]==count,(name,checks[name]);runs[name]=str(run)
captures=sorted(Path(runs['fixes']).glob('*.bmp'));assert len(captures)==7
for p in captures:assert struct.unpack_from('<ii',p.read_bytes(),18)==(480,272)
sources={};names=set(baseline['source_sha256'])|{'cargo-console.h','loadout-console.h','menu-fixes-review.h','game.c','game.h'}
for name in sorted(names):
    p=root/'src'/name;assert p.stat().st_mtime<=artifact.stat().st_mtime,name;sources[name]=digest(p)
    if name in baseline['source_sha256'] and name not in ('main.c','ui-modern.h','comms-console.h'):
        assert sources[name]==baseline['source_sha256'][name],f'Unrelated source change: {name}'
assert '#define AUDIO_INTRO_SKIP_SECONDS 21' in read(root/'src/audio-intro.h')
release=root/'outputs/releases/2.5.371/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (baseline['sha256'],sha),'Shared release changed'
total=sum(checks.values());assert total==198
evidence=f'''ELITE: NEXT 2.5.371 — System readouts and menu controls
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/menu371-build.log

Stars/worlds count moved below its label, inside the System Details right panel.
Comms navigation prioritizes controls sharing a row/column: Contact Right goes
to Request Docking; Docking Left returns to Contact. All existing actions retained.
Cargo Square jettisons one recoverable consignment in open space without selling.
Mission cargo and full debris pool protected. Docked buy/sell retains prices,
quantities, credits and stock, without Left/Right action popup text.
Loadout detail heading moved down; Square Store/Item Storage hints are clear,
L/R footer hints removed. Existing shoulder category shortcuts retained.
Square stores the selected bank while docked, with bounded confirmation or reason;
no sale. Storage capacity, passenger and hold limits remain protected.
Prior clean Commander Profile, centered actions and 21-second intro skip retained.

{total} focused emulator checks PASS on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Native 480x272 captures inspected. All 256 system count readouts match their
actual bodies. Actual D-pad/Square actions, jettison/tractor recovery, final-item
selection, mission/debris guards, bank-three storage and footer verified.
Baseline 2.5.370 Cargo/Loadout/Comms suites also passed before this build.
Physical PSP testing pending. Not whole-game certification. Prior broad failures
remain unresolved: 15 inherited core failures; incomplete extended input run
recorded 130 failures. See BUILD-2.5.365.txt. Full smoke was not repeated.

Replace ONLY EBOOT.PBP; keep saves, settings and music. No player music bundled.
Details: docs/MENU-FIXES-371.md. Local only; Git pull blocked by untracked music
placeholders, autostash restored. Mixed work preserved. GitHub not updated.
Next: hardware playtest, inherited reliability, then planetary work.
'''
release.mkdir(parents=True,exist_ok=True)
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
    (folder/'BUILD-2.5.371.txt').write_text(evidence,encoding='utf-8')
    if folder!=root:
        (folder/'README.txt').write_text(f'ELITE: NEXT 2.5.371\nSystem count layout, Comms navigation, Cargo jettison and Loadout fixes.\n{total} focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.371',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,captures=[str(p) for p in captures],source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved'),indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.371.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.371 — System readouts and menu controls

System Details shows stars/worlds counts underneath the label. Comms Contact/Docking navigate directly across their row. Cargo Square jettisons one recoverable consignment in open space, protecting mission cargo and preventing loss when the debris pool is full. Left/Right trade popups removed. Ship Loadout's heading sits lower; footer shows Square Store and Item Storage with no L/R hints. Square stores the selected docked bank with bounded confirmation or refusal reason. Existing shoulder category shortcuts remain. Preserves clean profile portraits, centered profile buttons and the 21-second startup music skip.

198 focused emulator checks pass: 17 new menu checks (including all 256 system readouts and actual jettison/tractor recovery), 17 Cargo, 24 Loadout, 66 Comms, 21 Commander Profile, 51 Command Deck and two startup checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/MENU-FIXES-371.md and BUILD-2.5.371.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.371. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;text=read(p)
    if not text.startswith('## 2.5.371'):p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.371',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
