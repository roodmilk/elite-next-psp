"""Package the exact optimized 2.5.385 candidate reviewed by PPSSPP."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
from PIL import Image
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.384/test-evidence.json'))
artifact=root/'outputs/race385-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.385'
assert "'-O3','-flto'" in read(root/'build.ps1')
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.385' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper();assert film_sha==baseline['film_sha256']
allowed=set('audio.h campaign-ui.h comms-console.h flight-extras.h game.c game.h journey.h main.c mission-network-tests.h mission-reward-card.h radio-config.h radio-synth.h radio-ui.h sectors.h space-racing-review.h space-racing-ui.h space-racing.h tutorial-runtime.h ui-modern.h voyage.h'.split())
added={'space-racing.h','space-racing-ui.h','space-racing-review.h'};sources={}
for p in sorted((root/'src').rglob('*')):
 if not p.is_file():continue
 name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
 assert p.stat().st_mtime<=artifact.stat().st_mtime,f'Unbuilt source: {name}'
 if name not in allowed:assert sources[name]==baseline['source_sha256'][name],f'Unrelated source changed: {name}'
assert set(sources)==set(baseline['source_sha256'])|added
main=read(root/'src/main.c');initializer=main[main.index('static void display_start_black'):main.index('static void display_recover')]
assert 'pspDebugScreenInit();' in initializer and 'PSP_DISPLAY_SETBUF_NEXTFRAME' in initializer
log=read(root/'outputs/race385-tests.log');suite=Path(re.search(r'^Review:\s*(.+)$',log,re.M)[1].strip())
checks={};runs={};reports={}
for case in json.loads(read(suite/'results.json')):
 name=case['test'];run=suite/name;t=read(Path(case['report']));reports[name]=t;runs[name]=str(run)
 assert digest(run/'EBOOT.PBP')==sha,f'Tests used a different candidate: {name}'
 if name!='core-check':assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),name
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
assert checks['space-racing-review']>=47 and checks['mission-reward-review']==37 and checks['intro-film-review']>=35 and checks['opening-check']==345
assert 'real MP3 decoder plays and advances the Rock FM source' in reports['space-racing-review']
assert 'all 768 system/hub courses' in reports['space-racing-review'] and 'standard unmodified ship can win' in reports['space-racing-review']
assert 'startup discards exactly twenty-three seconds' in reports['intro-film-review']
core_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert core_fail==baseline['core_inherited_failures'] and re.search(r'^RESULT 15 failures$',reports['core-check'],re.M),'Core failures changed'
for name in ('radio','steering'):
 t=read(suite/'core-check'/(name+'-check.txt'));assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
boot_log=read(root/'outputs/race385-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip());assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.385/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
for name in ('space-racing-review','mission-reward-review','intro-film-review'):
 for p in (suite/name).glob('*.bmp'):
  im=Image.open(p);assert im.size==(480,272);im.save(screens/(p.stem+'.png'))
focused=sum(v for n,v in checks.items() if n!='core-check')
feature='''The mission board preserves its five contracts and adds a sixth local Space Race.
Accept, launch, then navigate from Tracked Mission, or select RACE START GRID
in the targeting computer's OTHER category. The published waypoint remains
available beyond sensor range. Enter within 450 m to line up automatically.
Checkered physical starting gantry, 5-10 varied solid rival hulls, four-second
READY / 3 / 2 / 1 / GO countdown with audio cues and 12 ordered 3D checkpoints.
Use normal steering, R throttle, L brake and double-tap R then hold for boost.
Gold rings inject speed without reducing higher velocity, cool the engine
and provide two seconds of free powered coasting. HUD shows place, elapsed
time, checkpoint count and direction arrows, including targets behind you.
First place earns the actual advertised prize once, through Race Control's
mission-completion transmission. Losing/three-minute timeout offers free
RETRY / LEAVE. Retry resets every checkpoint, boost gate, clock and rival.
Lasers and missiles are locked; existing police/warrant enforcement remains.
Menus pause the simulation. Rock FM temporarily overrides music using the
existing music/Sol Rock folder or generated rock fallback. Original tuning
and power settings are never overwritten; Quiet Mode continues to apply.
Finish, abort or loading restores the original station/settings automatically.
Existing V32 saves retain accepted entries and their originating hub. Loading
rebuilds the original grid and resets attempts; ordinary saves stay dock-only.
All 768 seeded system/hub courses validate clearance from canonical planets,
rings and stations. Rendering is bounded to three upcoming checkpoints,
two boost rings and ten nearby solid hulls. No extra framebuffer is allocated.
'''
evidence=f'''ELITE: NEXT 2.5.385 - Space racing
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Built on local 2.5.384; unrelated source hashes preserved.

{feature}
{focused} focused emulator checks pass, including {checks['space-racing-review']}
race checks: a real production flight win in an unmodified ship; all rival
finishes through actual apertures; prize once; retries; pause; save/load;
Law interruption; disabled weapons; real MP3 source switching and return.
Native 480x272 board/grid/countdown/pack/boost/arrows/result/prize captures
inspected. Ten-rival view reports bounded geometry and emulator render FPS.
All 1280 original contracts still complete and pay once through real handlers.
Core: {checks['core-check']} pass; same 15 inherited failures remain.
Optimized O3/LTO build: outputs/race385-build.log
Review: {suite}
Normal boot: {boot}
Physical PSP boot, race audio/controls and FPS need hardware playtesting.
The 2.5.384 display initializer/first latch and intro settings are retained.

Local release only. GitHub unchanged after earlier automatic approval rejected
pull/rebase on the mixed shared working tree. Replace ONLY EBOOT.PBP; keep
saves, settings and music. This ZIP contains no player music or saves.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
 folder.mkdir(parents=True,exist_ok=True)
 if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha),'Shared release changed'
 shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
 (folder/'BUILD-2.5.385.txt').write_text(evidence,encoding='utf-8')
 if folder!=root:
  (folder/'README.txt').write_text('ELITE: NEXT 2.5.385\nSpace races: sixth mission-board offer, 5-10 rivals, checkpoint/boost rings, Rock FM and first-place prize.\nEmulator tested; physical PSP playtest pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
  (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.385',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,core_inherited_failures=core_fail,physical_psp='pending: cold boot, controls, race FPS and music confirmation',git='local only; prior automatic approval rejected shared-tree pull/rebase')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.385.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for p in sorted(release.iterdir()):assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.385 — Local space races

{feature}
Validation: {focused} focused emulator checks pass, including {checks['space-racing-review']} race checks and real MP3 override/return. An unmodified ship wins through production flight physics without teleporting or altered stats; every rival finishes through real crossings. Mission transmissions, intro, opening, radio, steering and normal boot checks pass. All 1280 original contracts still complete and pay once. Core: {checks['core-check']} pass with the same 15 inherited failures. Native 480×272 captures inspected. Optimized EBOOT SHA256 {sha}. Physical PSP boot/audio/controls/race FPS remain a playtest gate. The 2.5.384 SDK display initialization and intro remain intact. Local release only; GitHub unchanged following the earlier automatic approval rejection of shared-tree pull/rebase.

Highest-priority remaining work: physical PSP cold-boot and race playtest; tune course/difficulty from feedback; then inherited reliability failures, authored portraits and paused planetary expansion.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;t=read(p)
 if not t.startswith('## 2.5.385'):p.write_text(entry+t,encoding='utf-8')
note='## Space racing — 2.5.385\n\n'+feature+'''
Checkpoint completion uses swept directional plane crossings and an aperture
intersection test, preventing tunnelling, reverse crossings and skipped gates.
Rivals physically steer/accelerate toward apertures; rank compares real route
progress and interpolated finish times. Race attempts are transient; V32 Job
type and item retain mission identity and station. No save-format migration.
Existing missions, station services, law and shared-world identities remain.
Emulator integration, real MP3 switching, native screenshots and all 768
clearances are verified; physical PSP testing remains required.

'''
for name in ('docs/DESIGN-BIBLE.md','docs/DESIGN-BIBLE-2.0.md'):
 p=root/name;t=read(p)
 if 'Space racing — 2.5.385' not in t:p.write_text(note+t,encoding='utf-8')
(root/'docs/SPACE-RACING-V1.md').write_text(note+'\n'+evidence,encoding='utf-8')
print(json.dumps(dict(version='2.5.385',sha256=sha,focused_passes=focused,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
