"""Package the exact reviewed winding-race release, preserving unrelated work."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
from PIL import Image
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.386/test-evidence.json'))
artifact=root/'outputs/race387-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.387'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.387' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha==baseline['film_sha256']
allowed=set('game.h game.c main.c flight-extras.h space-racing.h space-racing-ui.h space-racing-review.h radio-tests.h'.split())
sources={}
for p in sorted((root/'src').rglob('*')):
 if not p.is_file():continue
 name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
 assert p.stat().st_mtime<=artifact.stat().st_mtime,f'Unbuilt source: {name}'
 if name not in allowed:assert sources[name]==baseline['source_sha256'][name],f'Unrelated source changed: {name}'
assert set(sources)==set(baseline['source_sha256'])
main=read(root/'src/main.c');initializer=main[main.index('static void display_start_black'):main.index('static void display_recover')]
assert 'pspDebugScreenInit();' in initializer and 'PSP_DISPLAY_SETBUF_NEXTFRAME' in initializer
log=read(root/'outputs/race387-tests.log');suite=Path(re.search(r'^Review:\s*(.+)$',log,re.M)[1].strip())
checks={};runs={};reports={}
for case in json.loads(read(suite/'results.json')):
 name=case['test'];run=suite/name;t=read(Path(case['report']));reports[name]=t;runs[name]=str(run)
 assert digest(run/'EBOOT.PBP')==sha
 if name!='core-check':assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),name
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
assert checks['space-racing-review']>=71 and checks['mission-reward-review']==37 and checks['intro-film-review']==37 and checks['opening-check']==345
race=reports['space-racing-review']
assert 'music/Sol Rock, without applying the intro-song skip' in race
assert 'all 768 system/hub courses' in race and 'a standard unmodified ship can win' in race
assert 'roughly two-minute winding race' in race and 'high-speed swept ram destroys' in race
assert 'actual selected first/chase/cinematic camera without a jump' in race
assert 'startup discards exactly twenty-one seconds' in reports['intro-film-review']
core_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert core_fail==baseline['core_inherited_failures'] and re.search(r'^RESULT 15 failures$',reports['core-check'],re.M)
for name in ('radio','steering'):
 t=read(suite/'core-check'/(name+'-check.txt'));assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
boot_log=read(root/'outputs/race387-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip());assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.387/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
for name in ('space-racing-review','intro-film-review'):
 for p in (suite/name).glob('*.bmp'):
  im=Image.open(p);assert im.size==(480,272);im.save(screens/(p.stem+'.png'))
focused=sum(v for n,v in checks.items() if n!='core-check')
flight_info=re.search(r'^INFO production flight:.+$',race,re.M)[0]
render_info=re.search(r'^INFO dense race draw:.+$',race,re.M)[0]
feature='''32 ordered physical gates replace the almost straight twelve-gate sprint.
Seeded courses cover roughly 900 km with repeated S bends, climbing turns
and descending chicanes. A starter Adder finishes in about two minutes using
normal steering, throttle and double-tap-R boost. Bring your own ship: its
actual speed, handling, acceleration, installed modules and fuel still apply.
Gold boost/cooling rings on every leg support sustained arcade racing.
Four scenery sections: asteroid slalom, split freighter wrecks, floodlit
port grandstands/pylons and ice fields, with open racing lanes and trail lamps.
5-10 physical rivals include clean racers and opportunistic shoulder-checkers.
Swept relative-motion collisions catch high-speed rams between frames.
Gentle contact separates the ships and costs speed; hard/boosted impacts
break the rival's actual hull mesh into spinning debris, sparks and a shockwave.
Destroyed racers leave the leaderboard and cannot finish or take the prize.
A bounded 1.2-second slow-motion orbit frames the takedown, then a 0.6-second
swing returns to the actual first/chase/cinematic camera. Safe autopilot runs
through the shot and for one further second afterward, using real gates.
Every racer shares the slow simulation; menus pause the camera and actors.
Held boost resumes on return if still held; there is a cooldown between shots.
Targeting browser, quick-target and target-lock combos are disabled while
racing, including the menu route. Square never steals R throttle during races.
Race HUD shows section, next gate distance, place, time and takedowns.
First place alone earns the existing actual mission prize once. Free retries
reset every rival, effect, gate and camera. Five-minute safety timeout.
Race music uses the MP3 library in music/Sol Rock beside EBOOT.PBP (Rock FM).
Playback works even when ordinary radio music is off, respects Quiet Mode,
does not apply the intro-song skip and restores previous radio preferences.
No MP3s, saves or private settings are included in this package.
Existing V32 save compatibility, mission roster and law enforcement remain.
The 21-second intro music start/fade and SDK display-startup repair remain.
'''
evidence=f'''ELITE: NEXT 2.5.387 - Winding arcade races and takedowns
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Built on local 2.5.386; unrelated production source hashes preserved.

{feature}
Fresh validation: {focused} focused emulator checks pass, including
{checks['space-racing-review']} racing checks, real MP3 decoding from music/Sol Rock,
all 768 deterministic clear courses, unmodified production flight, physical
contacts, shared slow motion, safe camera return in all three camera modes,
pause, retry, prize once, save/load, targeting lockout and framebuffer guards.
{flight_info}
{render_info}
Native 480x272 scenery, route, pack, collision and camera captures inspected.
Core: {checks['core-check']} pass; same 15 inherited failures remain.
Review: {suite}
Normal boot: {boot}
Physical PSP boot, race controls, sound and FPS still need hardware playtesting.

Local release only. GitHub remains unchanged following the earlier automatic
approval rejection of pull/rebase on the mixed shared working tree.
Replace ONLY EBOOT.PBP; keep commander saves, settings and music.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
 folder.mkdir(parents=True,exist_ok=True)
 if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha)
 shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
 (folder/'BUILD-2.5.387.txt').write_text(evidence,encoding='utf-8')
 if folder!=root:
  (folder/'README.txt').write_text('ELITE: NEXT 2.5.387\nWinding two-minute starter races, scenery, physical rival crashes and safe slow-motion cameras.\nUses your MP3s in music/Sol Rock beside EBOOT.PBP; retain that folder.\nEmulator tested; physical PSP confirmation pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
  (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.387',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,
            source_sha256=sources,core_inherited_failures=core_fail,
            production_flight=flight_info,dense_render=render_info,
            physical_psp='pending',git='local only; prior shared-tree pull/rebase rejection')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.387.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for p in sorted(release.iterdir()):assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.387 — Winding arcade races and takedowns

{feature}
Validation: {focused} focused emulator checks pass, including {checks['space-racing-review']} race checks and actual MP3 playback/return. {flight_info}. Core: {checks['core-check']} pass with the same 15 inherited failures. Native 480×272 captures inspected. Optimized EBOOT SHA256 {sha}. Physical PSP confirmation pending. Local release only; GitHub unchanged after the earlier shared-tree pull/rebase approval rejection.

Highest-priority remaining work: physical PSP race/cold-boot playtest, tune course and collision feel from feedback, then inherited reliability failures, authored portraits and paused planetary expansion.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;t=read(p)
 if not t.startswith('## 2.5.387'):p.write_text(entry+t,encoding='utf-8')
note='## Winding arcade races — 2.5.387\n\n'+feature+'\nExact release validation:\n'+flight_info+'\n'+render_info+'\n\n'
for name in ('docs/DESIGN-BIBLE.md','docs/DESIGN-BIBLE-2.0.md'):
 p=root/name;t=read(p)
 if not t.startswith('## Winding arcade races — 2.5.387'):p.write_text(note+t,encoding='utf-8')
(root/'docs/SPACE-RACING-V1.md').write_text(note+'\n'+evidence,encoding='utf-8')
print(json.dumps(dict(version='2.5.387',sha256=sha,focused_passes=focused,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
