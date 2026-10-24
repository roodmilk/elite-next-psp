"""Package the exact console-reviewed executable; retain unrelated shared work."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/comms369-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.369'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.369' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha=='0112B4C740CD880B8F0AF4BBF28D0BD4F17D91B1519EE4971EA5CA56395A499C'
runs={};checks={}
for name,count in (('comms',66),('deck',51),('boot',2),('audio',26),('radio',61)):
    text=read(root/f'outputs/comms369-{name}.log')
    assert 'RESULT 0 failures' in text and not re.search(r'^FAIL ',text,re.M),name
    run=Path(re.search(r'^Review:\s*(.+)$',text,re.M)[1].strip())
    assert digest(run/'EBOOT.PBP')==sha,name
    checks[name]=len(re.findall(r'^PASS ',text,re.M));assert checks[name]==count,(name,checks[name]);runs[name]=str(run)
captures=sorted(Path(runs['comms']).glob('comms-focus-*.bmp'));assert len(captures)==6
for p in captures:assert struct.unpack_from('<ii',p.read_bytes(),18)==(480,272)
baseline=json.loads(read(root/'outputs/releases/2.5.368/test-evidence.json'))['source_sha256'];sources={}
for name in ('main.c','comms-console.h','comms-panel-review.h','ui-modern.h','mission-log-review.h','command-deck-console.h','audio.h','audio-intro.h','radio-config.h','intro-film-review.h','radio-tests.h','portrait-parts.h','portrait-material-masks.h'):
    p=root/'src'/name;assert p.stat().st_mtime<=artifact.stat().st_mtime,name;sources[name]=digest(p)
    if name in baseline and name not in ('main.c','audio.h','audio-intro.h','intro-film-review.h'):assert sources[name]==baseline[name],f'Unexpected unrelated source change: {name}'
previous='D2813B23129EFE911F9885C5E4D550D1B16F39BA4306193C368BE856425A6D58'
release=root/'outputs/releases/2.5.369/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (previous,sha),'Shared release changed'
evidence=f'''ELITE: NEXT 2.5.369 — Global console radio tuning, clear selection and 15-second intro skip
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/comms369-final-build.log

L/R tune the radio from every highlighted console control without moving focus
or activating it. Uses existing radio_tune, preserves Quiet Mode, wraps stations
and retains existing Circle preference saving. Simultaneous shoulders cancel.
Conversation shoulder controls are unchanged; tuner X still advances stations.
Focus uses a complete two-pixel accent frame, bright corners and tinted face.
Frame remains within each existing control, including disabled selections.
Native footer adds proper PSP L/R shoulder icons and RADIO.
Intro music now starts 15 seconds in (previously 10), retaining its one-second
fade. First startup only; saved station/off/volume and later tracks unchanged.
Preserves Mission Log fix, menu icons, cinematic and portraits.

206 focused emulator checks PASS on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Complete existing console suite plus both tuning directions at all fourteen
controls; wrapping, focus, pause, Quiet Mode, saved radio preferences, dialogue
isolation, disabled actions and selection borders across all sixteen palettes.
Native 480x272 console captures inspected, including red paint and unavailable
Contact selection. Command Deck, normal startup, real-MP3 intro playback and
radio checks also pass. No player music is bundled.
Physical PSP testing pending. This is not whole-game certification.
Full smoke was not repeated for this focused UI change. Previously documented
broader failures remain unresolved (15 inherited core failures; extended input
recorded 130 failures with incomplete coverage). See BUILD-2.5.365.txt.

Replace ONLY EBOOT.PBP; retain music, saves and settings. No music bundled.
Details: docs/COMMS-PANEL-369.md. Local release only; Git pull blocked by existing
untracked music placeholders, autostash restored. Mixed shared work preserved.
GitHub not updated. Next: PSP playtest, inherited reliability, planetary work.
'''
release.mkdir(parents=True,exist_ok=True)
for p in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,p/'EBOOT.PBP');assert digest(p/'EBOOT.PBP')==sha
    (p/'BUILD-2.5.369.txt').write_text(evidence,encoding='utf-8')
    if p!=root:
        (p/'README.txt').write_text('ELITE: NEXT 2.5.369\nGlobal Comms Panel L/R radio tuning, stronger selection highlight and 15-second intro skip.\n206 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep music, saves and settings.\n',encoding='utf-8')
        (p/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.369',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,captures=[str(p) for p in captures],source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved'),indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.369.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.369 — Comms Panel tuning, clear focus and 15-second intro skip

L/R tune the radio from any highlighted console button while keeping focus and flight paused. Quiet Mode remains muted; station preferences save through the existing exit path. Conversations retain their shoulder controls. Selected buttons now have a full two-pixel accent frame, bright pale corners and tinted face; footer adds proper L/R shoulder icons with RADIO. All existing controls retained. Elite Exploration startup music begins 15 seconds in with the existing one-second fade. Saved preferences and later tracks are unchanged. Preserves Mission Log, menu icons, cinematic and portraits.

206 focused emulator checks pass: 66 complete-console/global-tuning checks, 51 Command Deck checks, two startup checks, 26 real-MP3 intro checks and 61 radio checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/COMMS-PANEL-369.md and BUILD-2.5.369.txt. Highest-priority remaining work: hardware playtest, inherited reliability issues, then planetary work.

Local release: outputs/releases/2.5.369. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and unrelated shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;text=read(p)
    if not text.startswith('## 2.5.369'):p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.369',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
