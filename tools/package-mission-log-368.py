"""Publish only the exact native-reviewed Mission Log executable."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/missionlog368-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.368'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.368' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha=='0112B4C740CD880B8F0AF4BBF28D0BD4F17D91B1519EE4971EA5CA56395A499C'
runs={};checks={}
for name,count in (('missions',21),('deck',51),('boot',2)):
    report=read(root/f'outputs/missionlog368-{name}.log')
    assert 'RESULT 0 failures' in report and not re.search(r'^FAIL ',report,re.M),name
    run=Path(re.search(r'^Review:\s*(.+)$',report,re.M)[1].strip())
    assert digest(run/'EBOOT.PBP')==sha,name
    checks[name]=len(re.findall(r'^PASS ',report,re.M));assert checks[name]==count,(name,checks[name])
    runs[name]=str(run)
captures=sorted(Path(runs['missions']).glob('log-*.bmp'))
assert len(captures)==8
for capture in captures:assert struct.unpack_from('<ii',capture.read_bytes(),18)==(480,272)
sources={}
baseline=json.loads(read(root/'outputs/releases/2.5.367/test-evidence.json'))['source_sha256']
for name in ('main.c','ui-modern.h','mission-log-review.h','command-deck-console.h','audio.h','audio-intro.h','radio-config.h','portrait-parts.h','portrait-material-masks.h'):
    p=root/'src'/name;assert p.stat().st_mtime<=artifact.stat().st_mtime,name;sources[name]=digest(p)
    if name in baseline and name!='main.c':assert sources[name]==baseline[name],f'Unexpected unrelated source change: {name}'
previous='B51E9AA02837D57C6A4013F624FD661AE2E5AD392D0C19A23F7228CD83C8706B'
release=root/'outputs/releases/2.5.368/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (previous,sha),'Shared release changed'
evidence=f'''ELITE: NEXT 2.5.368 — Accurate Mission Log tracking and readable titles
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/missionlog368-final-build.log

Only the actual tracked mission displays TRACKED MISSION on the right.
Other highlighted entries display MISSION DETAILS. X still tracks one mission;
Square still opens its next step. Existing left tracking marker retained.
Highlighted long titles pan within a 150x8 native pixel region at 24px/sec,
with one-second pauses at both ends. Full final words remain readable.
Short and unselected titles stay still. Unselected long names use ellipses.
Selection changes, reopening and title changes reset the reading pause.
Pixel clipping protects the marker, panel edge and framebuffer gutters.
No mission/save/audio/control changes. Previous menu icons, cinematic,
portraits and Elite Exploration startup music treatment retained.

74 focused emulator checks PASS on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Eight actual native 480x272 Mission Log captures inspected. Mission checks cover
main, contract and remapped special tracking IDs; hover/X/Square; scrolling,
pauses, final words and reset; all story/Guild titles; guarded clipping.
Command Deck checks cover menus, transitions, palette changes and input.
Physical PSP testing pending. This is not whole-game certification.
Full smoke was not repeated for this focused UI change. Previously documented
broader failures remain unresolved: 15 inherited core failures; extended input
recorded 130 failures with incomplete coverage. See BUILD-2.5.365.txt.

Replace only EBOOT.PBP; keep music, saves and settings. No music bundled.
Details: docs/MISSION-LOG-368.md.
Local release only. Git pull blocked by pre-existing untracked music placeholders;
autostash restored. Mixed shared work preserved. No GitHub push.
'''
release.mkdir(parents=True,exist_ok=True)
for p in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,p/'EBOOT.PBP');assert digest(p/'EBOOT.PBP')==sha
    (p/'BUILD-2.5.368.txt').write_text(evidence,encoding='utf-8')
    if p!=root:
        (p/'README.txt').write_text('ELITE: NEXT 2.5.368\nAccurate Mission Log tracking and scrolling highlighted titles.\n74 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep music, saves and settings.\n',encoding='utf-8')
        (p/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.368',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,captures=[str(p) for p in captures],source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved'),indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.368.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.368 — Mission Log tracking and title ticker

The dossier says TRACKED MISSION only for the one actually tracked entry; other selections say MISSION DETAILS. Highlighted long titles scroll within a clipped native 150×8 pixel region, with reading pauses at both ends. Short titles remain static; unselected long names use ellipses. Selection/reopen/title changes restart the ticker. Tracking controls and progression remain unchanged. Preserves 2.5.367 menu icons, cinematic, portraits and Elite Exploration startup music.

74 focused emulator checks pass: 21 Mission Log checks, 51 Command Deck checks and two normal startup checks. Native 480×272 captures inspected. Physical PSP testing and previously documented broad failures remain open. Details: docs/MISSION-LOG-368.md and BUILD-2.5.368.txt. Highest-priority remaining work: hardware playtest, inherited gameplay/input failures, then existing planetary work.

Local release: outputs/releases/2.5.368. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and unrelated shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;t=read(p)
    if not t.startswith('## 2.5.368'):p.write_text(entry+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.368',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
