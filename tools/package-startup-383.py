"""Publish the exact reviewed local startup build; preserve saves and music."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
from PIL import Image
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.382/test-evidence.json'))
artifact=root/'outputs/intro383-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.383'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.383' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha==baseline['film_sha256'],'Embedded film changed'
allowed={'main.c','intro.h','intro-film-review.h','audio-intro.h','radio-tests.h'}
sources={}
for p in sorted((root/'src').rglob('*')):
 if not p.is_file():continue
 name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
 assert p.stat().st_mtime<=artifact.stat().st_mtime,f'Unbuilt source: {name}'
 if name not in allowed:assert sources[name]==baseline['source_sha256'][name],f'Unrelated source changed: {name}'
assert set(sources)==set(baseline['source_sha256'])
log=read(root/'outputs/intro383-tests.log');suite=Path(re.search(r'^Review:\s*(.+)$',log,re.M)[1].strip())
checks={};runs={};reports={}
for case in json.loads(read(suite/'results.json')):
 name=case['test'];run=suite/name;t=read(Path(case['report']));reports[name]=t;runs[name]=str(run)
 assert digest(run/'EBOOT.PBP')==sha,f'Tests used another EBOOT: {name}'
 if name!='core-check':assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),name
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
assert checks['intro-film-review']>=35 and checks['opening-check']==345
assert 'startup discards exactly twenty-three seconds' in reports['intro-film-review']
core_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert core_fail==baseline['core_inherited_failures'] and re.search(r'^RESULT 15 failures$',reports['core-check'],re.M),'Core failures changed or report incomplete'
for name in ('radio','steering'):
 t=read(suite/'core-check'/(name+'-check.txt'))
 assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
boot_log=read(root/'outputs/intro383-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip());assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.383/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
for p in (suite/'intro-film-review').glob('*.bmp'):
 im=Image.open(p);assert im.size==(480,272);im.save(screens/(p.stem+'.png'))
focused=sum(v for n,v in checks.items() if n!='core-check')
evidence=f'''ELITE: NEXT 2.5.383 - Clean intro startup
EBOOT SHA256: {sha}
Embedded film SHA256: {film_sha} (unchanged)
Built on local 2.5.382; all unrelated source hashes preserved.

Intro music skips another two seconds: 23 seconds total, with its existing
one-second audio fade. The decoder discards actual source samples (VBR safe).
CuLa Games presents... fades in for one full second with the red/blue tiles.
Hidden setup greeting no longer queues a comms sound on cold boot or replay.
Both complete display planes start black in the correct pixel format.
The first cinematic loads into the hidden back plane, avoiding a flash of
stale or ship-themed pixels while the first JPEG and commander list load.
No film re-encode, new framebuffers, save-format or radio-preference changes.

{focused} focused emulator checks pass, including real supplied MP3 decoding,
exact 23-second skip, title fade steps, both initial display planes, replay,
opening integration, radio and normal boot. Native 480x272 frames inspected.
Core: {checks['core-check']} pass; same 15 inherited failures remain.
Physical PSP boot/audio testing pending.
Build: outputs/intro383-build.log
Review: {suite}
Normal boot: {boot}

Local release only. GitHub not updated: automatic approval review previously
rejected pull/rebase of the mixed shared working tree. Existing work preserved.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
 folder.mkdir(parents=True,exist_ok=True)
 if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha),'Shared release changed'
 shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
 (folder/'BUILD-2.5.383.txt').write_text(evidence,encoding='utf-8')
 if folder!=root:
  (folder/'README.txt').write_text('ELITE: NEXT 2.5.383\nClean intro startup; 23-second music skip and one-second title fade.\nEmulator tested; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
  (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.383',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,core_inherited_failures=core_fail,physical_psp='pending',git='local only; automatic approval rejected pull/rebase of mixed tree')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.383.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for p in sorted(release.iterdir()):
  assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.383 — Clean intro startup

Intro music starts two seconds later (23 seconds into the track), preserving its one-second fade and saved radio preferences. “CuLa Games presents...” and its red Cu/blue La tiles fade in together over one full second. Intro entry and Select replay clear the hidden setup greeting and its queued comms blip. Cold boot initializes both full display planes to black before publishing a framebuffer, then decodes the first cinematic into the hidden back plane so setup and themed clear colours cannot flash on screen. Embedded film and unrelated game source are unchanged; no new buffers or save changes.

Validation: {focused} focused emulator checks pass, including real MP3 decode/skip, startup display, title fade, replay, opening, radio, steering and normal boot. Native 480×272 captures inspected. Core {checks['core-check']} pass with the same 15 inherited failures; physical PSP testing pending. Exact reviewed candidate copied to root, dist and versioned release; SHA256 {sha}. Local only; GitHub not updated following the earlier automatic-approval rejection of pull/rebase on the mixed shared working tree.

Next: PSP startup playtest; then remaining authored character parts, inherited reliability fixes and paused planetary expansion.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;t=read(p)
 if not t.startswith('## 2.5.383'):p.write_text(entry+t,encoding='utf-8')
for name in ('docs/DESIGN-BIBLE.md','docs/DESIGN-BIBLE-2.0.md'):
 p=root/name;t=read(p)
 note='## Intro startup presentation — 2.5.383\n\nCold boot publishes black only after both full framebuffers are initialized; first cinematic decoding occurs offscreen. No hidden setup communication sound. CuLa Games presents... and its red/blue letter tiles fade in over one second. Elite Exploration startup skips 23 seconds using actual decoded source samples and keeps its one-second audio fade. Select replay retains these rules and existing radio preferences. No new framebuffer or save format.\n\n'
 if 'Intro startup presentation — 2.5.383' not in t:p.write_text(note+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.383',sha256=sha,focused_passes=focused,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
