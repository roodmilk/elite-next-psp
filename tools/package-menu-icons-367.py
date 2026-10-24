"""Package the exact executable verified by the native menu and boot reviews."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile

root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/icons367-candidate/EBOOT.PBP'
sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.367'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.367' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha=='0112B4C740CD880B8F0AF4BBF28D0BD4F17D91B1519EE4971EA5CA56395A499C'
runs={};checks={}
for name,count in (('deck',51),('boot',2),('audio',26)):
    report=read(root/f'outputs/icons367-{name}.log')
    assert 'RESULT 0 failures' in report and not re.search(r'^FAIL ',report,re.M),name
    run=Path(re.search(r'^Review:\s*(.+)$',report,re.M)[1].strip())
    assert digest(run/'EBOOT.PBP')==sha,name
    checks[name]=len(re.findall(r'^PASS ',report,re.M))
    assert checks[name]==count,(name,checks[name])
    runs[name]=str(run)
for name in ('music','exit','board','log','map','codex','tv'):
    p=Path(runs['deck'])/f'deck-icon-{name}.bmp';bmp=p.read_bytes()
    assert struct.unpack_from('<ii',bmp,18)==(480,272),name
sources={}
for name in ('command-deck-console.h','command-deck-review.h','deck-nav.h','audio.h','audio-intro.h','radio-config.h','portrait-parts.h','portrait-material-masks.h','main.c'):
    p=root/'src'/name
    assert p.stat().st_mtime<=artifact.stat().st_mtime,name
    sources[name]=digest(p)
previous='AE97CABF5CC060B50A4D1BA16FC26BAEACE73041687469FD0DFDB1BD2F5D4048'
release=root/'outputs/releases/2.5.367/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (previous,sha),'Shared release changed'
evidence=f'''ELITE: NEXT 2.5.367 — Distinct native menu icons
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/icons367-build.log

Added actual-size 12x12 icons: Radio & audio (music notes), Quit to main menu
(exit door), Mission board (pinned notices), Mission log (checklist),
Galaxy.Maps (connected stars), Discovery Codex (open book), Local TV (TV).
Uses existing ship paint colours and selection highlights on buttons,
description cards and opening transitions. No new allocation or asset loading.
Controls, services and save/settings formats are unchanged.
Preserves the cinematic, portraits and Elite Exploration startup treatment.

79 focused PPSSPP checks PASS on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Seven real native 480x272 menu captures inspected, plus category and paint
captures. Menu review includes selection, navigation, clipping geometry,
animation interruption, pause, all sixteen paint palettes on fourteen pages,
and render timing. MP3/intro checks preserve ten-second skip / one-second fade.
Physical PSP testing is pending. This is not whole-game certification.
Existing broader failures/incomplete input coverage documented in
BUILD-2.5.365.txt remain unresolved; full smoke was not repeated for this
focused icon change. Previous core run recorded 15 inherited failures,
and extended input recorded 130 failures with incomplete coverage.

Replace only EBOOT.PBP; retain music, saves and settings. No music bundled.
Details: docs/MENU-ICONS-367.md.
Local release only. Git pull was blocked by pre-existing untracked music
placeholders; autostash restored. Mixed shared work preserved. No GitHub push.
'''
release.mkdir(parents=True,exist_ok=True)
for p in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,p/'EBOOT.PBP')
    (p/'BUILD-2.5.367.txt').write_text(evidence,encoding='utf-8')
    if p!=root:
        (p/'README.txt').write_text('ELITE: NEXT 2.5.367\nSeven distinct native menu icons.\n79 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; retain music, saves and settings.\n',encoding='utf-8')
        (p/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.367',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved'),indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.367.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.367 — Distinct menu icons

Adds native 12×12 music notes, an exit door, mission board, mission checklist, star map, open codex and TV for Radio & audio, Quit to main menu, Mission board, Mission log, Galaxy.Maps, Discovery Codex and Local TV. Icons follow ship paint and selection colours on menu buttons, descriptions and opening transitions. Existing controls, cinematic, portraits and Elite Exploration startup music remain intact.

79 focused emulator checks pass on the exact EBOOT: 51 Command Deck checks, two startup checks and 26 real-MP3/intro checks. Actual native 480×272 captures inspected. Physical PSP testing and previously documented broad failures remain open. Details: docs/MENU-ICONS-367.md and BUILD-2.5.367.txt. Highest-priority remaining work: PSP playtest, inherited gameplay/input failures, then existing planetary work.

Local release: outputs/releases/2.5.367. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and unrelated shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;t=read(p)
    if not t.startswith('## 2.5.367'):p.write_text(entry+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.367',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
