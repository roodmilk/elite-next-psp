"""Package the exact emulator-reviewed 2.5.381 candidate, preserving other work."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
from PIL import Image
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.380/test-evidence.json'))
artifact=root/'outputs/load381-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.381'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.381' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha==baseline['film_sha256'],'Embedded intro changed'
allowed={'menu-ship-preview.h','load-arrival.h','load-arrival-review.h'}
sources={}
for p in sorted((root/'src').rglob('*')):
 if not p.is_file():continue
 name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
 assert p.stat().st_mtime<=artifact.stat().st_mtime,f'Unbuilt source: {name}'
 if name not in allowed:assert sources[name]==baseline['source_sha256'][name],f'Unrelated source changed: {name}'
assert set(sources)-set(baseline['source_sha256'])==set()
log=read(root/'outputs/load381-tests.log');suite=Path(re.search(r'^Review:\s*(.+)$',log,re.M)[1].strip())
results=json.loads(read(suite/'results.json'));checks={};runs={};reports={}
for case in results:
 name=case['test'];run=suite/name;t=read(Path(case['report']));reports[name]=t;runs[name]=str(run)
 assert digest(run/'EBOOT.PBP')==sha,f'Tests used another EBOOT: {name}'
 if name!='core-check':assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),name
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
assert checks['load-arrival-review']==53 and checks['opening-check']==345
core_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert re.search(r'^RESULT 15 failures$',reports['core-check'],re.M),'Core review incomplete'
assert len(core_fail)==15 and core_fail==baseline['core_inherited_failures'],'New core regression'
for name in ('radio','steering'):
 t=read(Path(runs['core-check'])/(name+'-check.txt'))
 assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
boot_log=read(root/'outputs/load381-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip());assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.381/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
for p in (suite/'load-arrival-review').glob('*.bmp'):
 im=Image.open(p);assert im.size==(480,272);im.save(screens/(p.stem+'.png'))
focused=sum(v for n,v in checks.items() if n!='core-check')
evidence=f'''ELITE: NEXT 2.5.381 - Continuous approach and CRT dashboard startup
EBOOT SHA256: {sha}
Embedded intro SHA256: {film_sha} (unchanged)
Built on local 2.5.380; portrait fixes and all unrelated work preserved.

Continue / Load Commander from the main menu now establish the actual saved
orbital station, then keep accelerating/zooming throughout the fade to black.
A bounded dolly plus lens push never crosses the station mesh. Navigation
categories flicker independently in a shuffled order before settling on.
The real station/sun art powers on from a thin CRT beam into a full raster.
Three quiet synthesized sweeps accompany the dashboard.
6.65-second presentation; no additional framebuffer or simulation writes.
Select on the main menu replays the intro; Circle leaves New Commander.
Saved commanders and radio preferences are preserved.

{focused} focused emulator checks pass; native 480x272 captures inspected.
Core: {checks['core-check']} pass; the same 15 inherited failures remain.
Physical PSP testing pending. This is not a whole-game zero-bug certification.
Build: outputs/load381-build.log
Review: {suite}
Normal boot: {boot}

Local release only. GitHub was not updated: automatic approval review rejected
pull/rebase of the mixed shared working tree. Existing work is preserved.
Replace ONLY EBOOT.PBP; keep commander saves, settings and music.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
 folder.mkdir(parents=True,exist_ok=True)
 if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha),'Shared release changed'
 shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
 (folder/'BUILD-2.5.381.txt').write_text(evidence,encoding='utf-8')
 if folder!=root:
  (folder/'README.txt').write_text('ELITE: NEXT 2.5.381\nContinuous station zoom, shuffled navigation lamps and CRT monitor startup.\nEmulator tested; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
  (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.381',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,core_inherited_failures=core_fail,physical_psp='pending',git='local only; automatic approval rejected pull/rebase of mixed tree')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.381.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for p in sorted(release.iterdir()):
  assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.381 — Continuous approach and CRT dashboard startup

The saved-station approach keeps zooming throughout its fade to black, using a lens push after safe physical camera travel ends. Dashboard navigation categories flicker independently in a shuffled order before staying on. Actual station/sun background art expands from a thin CRT beam into the full image. Effects settle to the exact normal menu pixels; no extra framebuffer, save changes or simulation advancement. Select intro replay, creator Back and startup sounds are preserved.

Validation: {focused} focused emulator checks pass; native 480×272 station/startup captures inspected. Core {checks['core-check']} pass with the same 15 inherited failures; physical PSP testing pending. Unrelated source and embedded intro hashes match 2.5.380. Exact candidate copied to root, dist and versioned release; SHA256 {sha}. Local only: automatic approval rejected pull/rebase of the mixed shared working tree; GitHub not updated.

Next: playtest the continuous zoom and CRT startup on physical PSP, then resume authored character-part work, inherited reliability fixes and the paused planetary expansion.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;t=read(p)
 if not t.startswith('## 2.5.381'):p.write_text(entry+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.381',sha256=sha,focused_passes=focused,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
