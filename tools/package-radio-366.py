"""Publish only the exact EBOOT used by the startup-audio reviews."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/radio366-final/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.366'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.366' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha=='0112B4C740CD880B8F0AF4BBF28D0BD4F17D91B1519EE4971EA5CA56395A499C'
runs={};checks={}
for n,count in (('audio',26),('radio',61),('boot',2),('opening',343)):
    t=read(root/f'outputs/radio366-{n}.log')
    assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),n
    p=Path(re.search(r'^Review:\s*(.+)$',t,re.M)[1].strip())
    assert digest(p/'EBOOT.PBP')==sha,n
    checks[n]=len(re.findall(r'^PASS ',t,re.M));assert checks[n]==count,(n,checks[n])
    runs[n]=str(p)
sources={}
for n in ('audio.h','audio-intro.h','radio-config.h','radio-tests.h','intro-film-review.h','opening-dialogue.h','opening-runtime.h','opening-input-tests.h','opening-live-review.h','main.c'):
    p=root/'src'/n;assert p.stat().st_mtime<=artifact.stat().st_mtime,n;sources[n]=digest(p)
previous='359630D396FAD70FC1E4F5055C6A14AD948569C106DE85F852D136132DEF4571'
release=root/'outputs/releases/2.5.366/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',release/'EBOOT.PBP'):
    if p.exists():assert digest(p) in (previous,sha),'Shared release changed'
evidence=f'''ELITE: NEXT 2.5.366 — Elite Exploration startup
EBOOT SHA256: {sha}
Embedded intro SHA256: {film_sha} (unchanged from 2.5.365)
Build: outputs/radio366-final-build; outputs/radio366-final-build.log

Elite Exploration is the default for new/missing radio preferences.
Saved station/off/volume settings remain authoritative.
On first application audio start with Elite Exploration selected, discard
exactly ten seconds of decoded source samples, then fade audible music from
zero to the configured volume over 44,100 output samples (one second).
MP3 reservoir/VBR state is preserved; Vorbis uses the same decoded-frame path.
Subsequent tracks, station changes and audio restarts do not repeat the skip.
Quiet/off/source changes cancel the startup treatment. SFX remain unchanged.
Player music, saved commanders and preference formats are unchanged.
Tutorial prompts now name the actual default station.

432 focused PPSSPP checks PASS on this exact executable:
'''+''.join(f'{n}: {checks[n]} PASS; {runs[n]}\n' for n in checks)+'''
Real 44.1 kHz MP3 plays alongside native intro JPEG decoding. Checks cover
exact ten-second discard, one-second fade, continuous playback across film
and decoder release, restart without repeating skip, preference persistence,
off-marker migration, backup recovery, shuffle, synthesis and suspend.
Opening sequence and normal nonblank startup also pass.
The embedded cinematic and portrait source remain intact.
Physical PSP testing pending. This is not whole-game certification.
Existing broader failures/incomplete input coverage documented in
BUILD-2.5.365.txt remain unresolved; full smoke was not repeated for this
focused audio change. The previous 2.5.364 core run recorded 15 inherited
failures, and extended input recorded 130 failures with incomplete coverage.

Replace only EBOOT.PBP; keep your music, saves and settings.
Music is not bundled or modified. Details: docs/INTRO-RADIO-366.md.
Local release only. Git pull was blocked by pre-existing untracked music
placeholders; autostash restored. Mixed shared work preserved. No GitHub push.
'''
entry=f'''## 2.5.366 — Elite Exploration intro music

Elite Exploration is the default station for new preferences. Its first startup track starts ten seconds in and fades in over one second at the saved music volume. Saved station/off settings are preserved; later tracks, tuning and audio restarts play normally. Keeps the 2.5.365 cinematic and portrait work. Tutorial prompts name the updated default.

432 focused native checks pass on the exact EBOOT: 26 real-MP3/intro checks, 61 radio/settings/suspend checks, two startup checks and 343 opening checks. Physical PSP testing and full-game certification remain pending; previously documented broad failures are unresolved. Details: docs/INTRO-RADIO-366.md and BUILD-2.5.366.txt.

Local release: outputs/releases/2.5.366. SHA256: {sha}. GitHub not updated; pull remains blocked by existing untracked music placeholders, autostash restored and unrelated shared work retained.

'''
release.mkdir(parents=True,exist_ok=True)
for p in (root,root/'dist/ELITE-NEXT-PSP',release):
    shutil.copy2(artifact,p/'EBOOT.PBP');(p/'BUILD-2.5.366.txt').write_text(evidence,encoding='utf-8')
    if p!=root:
        (p/'README.txt').write_text('ELITE: NEXT 2.5.366\nElite Exploration startup: ten-second skip, one-second fade-in.\n432 focused emulator checks pass. Physical PSP testing pending.\nSaved radio preferences remain in use. Full-game certification remains open.\nReplace ONLY EBOOT.PBP; keep music, saves and settings.\n',encoding='utf-8')
        (p/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
manifest=dict(version='2.5.366',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',full_smoke='not repeated; prior documented failures unresolved')
(release.parent/'test-evidence.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.366.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
for n in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/n;t=read(p)
    if not t.startswith('## 2.5.366'):p.write_text(entry+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.366',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
