"""Package the exact executable checked by the focused native menu suites."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/codex372-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.372'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.372' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.371/test-evidence.json'))
assert film_sha==baseline['film_sha256']
checks={};runs={}
for name,count in (('progress',11),('dossier',5),('deck',51),('boot',2)):
    text=read(root/f'outputs/codex372-{name}.log')
    assert 'RESULT 0 failures' in text and not re.search(r'^FAIL ',text,re.M),name
    run=Path(re.search(r'^Review:\s*(.+)$',text,re.M)[1].strip())
    assert digest(run/'EBOOT.PBP')==sha,name
    checks[name]=len(re.findall(r'^PASS ',text,re.M));assert checks[name]==count,(name,checks[name]);runs[name]=str(run)
captures=sorted(Path(runs['progress']).glob('*.bmp'));assert len(captures)==4
for p in captures:assert struct.unpack_from('<ii',p.read_bytes(),18)==(480,272)
sources={};names=set(baseline['source_sha256'])|{'discovery-atlas.h','codex-progress-review.h'}
for name in sorted(names):
    p=root/'src'/name;assert p.stat().st_mtime<=artifact.stat().st_mtime,name;sources[name]=digest(p)
    if name in baseline['source_sha256'] and name not in ('main.c','discovery-atlas.h'):
        assert sources[name]==baseline['source_sha256'][name],f'Unrelated source change: {name}'
assert '#define AUDIO_INTRO_SKIP_SECONDS 21' in read(root/'src/audio-intro.h')
release=root/'outputs/releases/2.5.372/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (baseline['sha256'],sha),'Shared release changed'
total=sum(checks.values());assert total==69
evidence=f'''ELITE: NEXT 2.5.372 — Discovery Codex progress fill
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/codex372-build.log

System Record fills proportionally in the ship signal colour. The frame draws
first, retaining the dark recess and visible fill; the former solid frame covered
all progress. All reached checkpoint arrows light up; claimed arrows stay gold
and future checkpoints stay dim. Native bar footprint, calculations, rewards,
controls and save format unchanged. Preserves 2.5.371 menus/cargo, clean profile
portraits and the 21-second startup music skip. No player music bundled.

{total} focused emulator checks PASS on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Every percentage 0 through 100 tested in all sixteen themes, including empty,
half/full, frame, reached/claimed/future arrows, clamping and guard words.
Actual Discovery Codex screenshots at native 480x272 inspected. Existing dossier
reward payment, save persistence and closing controls pass; baseline dossier
checks also passed on 2.5.371. Command Deck and normal boot pass.
Physical PSP testing pending. Not whole-game certification. Prior broad failures
remain unresolved: 15 inherited core failures; incomplete extended input run
recorded 130 failures. See BUILD-2.5.365.txt. Full smoke was not repeated.

Replace ONLY EBOOT.PBP; keep saves, settings and music.
Details: docs/CODEX-PROGRESS-372.md. Local only; Git pull blocked by untracked music
placeholders, autostash restored. Mixed work preserved. GitHub not updated.
Next: hardware playtest, inherited reliability, then planetary work.
'''
release.mkdir(parents=True,exist_ok=True)
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
    (folder/'BUILD-2.5.372.txt').write_text(evidence,encoding='utf-8')
    if folder!=root:
        (folder/'README.txt').write_text(f'ELITE: NEXT 2.5.372\nDiscovery Codex coloured progress fill and illuminated reached checkpoints.\n{total} focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.372',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,captures=[str(p) for p in captures],source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved'),indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.372.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.372 — Discovery Codex progress fill

System Record now visibly fills in the ship signal colour: its frame draws first so it no longer covers the interior. Every reached ten-percent arrow lights up, claimed checkpoints stay gold and future ones remain dim. Exploration calculations, rewards, controls and save formats are unchanged. Preserves the 2.5.371 menu/cargo fixes, clean Commander Profile and 21-second startup music skip.

69 focused emulator checks pass: 11 progress checks (every percentage from 0 through 100 in all sixteen themes, actual dossier renders, arrow states and guards), five existing Codex reward/persistence checks, 51 Command Deck and two startup checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/CODEX-PROGRESS-372.md and BUILD-2.5.372.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.372. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;text=read(p)
    if not text.startswith('## 2.5.372'):p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.372',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
