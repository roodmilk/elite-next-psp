"""Publish only the exact focused-test executable, preserving the prior release."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/profile370-candidate/EBOOT.PBP'
sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.370'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.370' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.369/test-evidence.json'))
assert film_sha==baseline['film_sha256']
runs={};checks={}
for name,count in (('profile',21),('deck',51),('comms',66),('boot',2),('audio',26),('radio',61)):
    text=read(root/f'outputs/profile370-{name}.log')
    assert 'RESULT 0 failures' in text and not re.search(r'^FAIL ',text,re.M),name
    run=Path(re.search(r'^Review:\s*(.+)$',text,re.M)[1].strip())
    assert digest(run/'EBOOT.PBP')==sha,name
    checks[name]=len(re.findall(r'^PASS ',text,re.M));assert checks[name]==count,(name,checks[name])
    runs[name]=str(run)
captures=sorted(Path(runs['profile']).glob('commander-profile-*.bmp'));assert len(captures)==4
for p in captures:assert struct.unpack_from('<ii',p.read_bytes(),18)==(480,272)
sources={}
names=set(baseline['source_sha256'])|{'commander-ui.h','commander-profile-review.h','pixel-art.h'}
for name in sorted(names):
    p=root/'src'/name;assert p.stat().st_mtime<=artifact.stat().st_mtime,name;sources[name]=digest(p)
    if name in baseline['source_sha256'] and name not in ('main.c','audio-intro.h','intro-film-review.h'):
        assert sources[name]==baseline['source_sha256'][name],f'Unrelated source change: {name}'
assert '#define AUDIO_INTRO_SKIP_SECONDS 21' in read(root/'src/audio-intro.h')
release=root/'outputs/releases/2.5.370/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (baseline['sha256'],sha),'Shared release changed'
total=sum(checks.values());assert total==227
evidence=f'''ELITE: NEXT 2.5.370 — Clean Commander Profile and 21-second intro skip
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/profile370-final-build.log

Removed themed scanline dots over the profile photo. The actual 80x80 photo
matches unmodified portrait artwork, including original saved identities.
Save Profile and Rename Commander centered with the actual 8x8 font metrics.
Existing button bounds, actions, confirmations and saved appearance retained.
Startup song skips 21 seconds, retaining the one-second fade and saved preferences.
Later tracks/restarts do not repeat the skip. No player music bundled.
Preserves 2.5.369 global Comms L/R tuning, clearer focus and unrelated shared work.

{total} focused emulator checks PASS on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+'''
Profile: 480 exact photo comparisons across ten species, three portrait formats
and sixteen themes, plus all eight original identities. Actual button pixels
and margins checked in both selection states/all themes; save/load, controls
and framebuffer guards pass. Native 480x272 profile screenshots inspected.
Real MP3 test checks exactly 21 seconds discarded and preserved fade/restart.
Physical PSP testing pending. Not whole-game certification. Prior broad failures
remain unresolved: 15 inherited core failures; incomplete extended input run
recorded 130 failures. See BUILD-2.5.365.txt. Full smoke was not repeated.

Replace ONLY EBOOT.PBP; retain saves, settings and music.
Details: docs/COMMANDER-PROFILE-370.md. Local release only; Git pull blocked by
existing untracked music placeholders, autostash restored. GitHub not updated.
Next: hardware playtest and inherited reliability work, then planetary work.
'''
release.mkdir(parents=True,exist_ok=True)
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
    (folder/'BUILD-2.5.370.txt').write_text(evidence,encoding='utf-8')
    if folder!=root:
        (folder/'README.txt').write_text(f'ELITE: NEXT 2.5.370\nClean profile portrait; centered actions; 21-second startup music skip.\n{total} focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.370',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,captures=[str(p) for p in captures],source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved'),indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.370.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.370 — Clean Commander Profile and 21-second intro skip

Removed the ship-themed scanline dots over the Commander Profile photo, leaving the actual portrait artwork intact. SAVE PROFILE and RENAME COMMANDER now center using the real 8×8 font. Existing actions and saved appearance remain unchanged. Startup music begins 21 seconds in with its one-second fade; saved preferences and subsequent tracks remain intact. Preserves Comms Panel global L/R tuning, clearer focus, cinematic and other recent work.

227 focused emulator checks pass: 21 profile checks (480 exact theme/species/format photo comparisons, original identities, actual label pixels, persistence and controls), 51 Command Deck, 66 Comms Panel, two startup, 26 real-MP3 intro and 61 radio checks. Native 480×272 captures inspected. Physical PSP testing and inherited broad failures remain open. Details: docs/COMMANDER-PROFILE-370.md and BUILD-2.5.370.txt. Highest-priority remaining work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.370. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;text=read(p)
    if not text.startswith('## 2.5.370'):p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.370',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
