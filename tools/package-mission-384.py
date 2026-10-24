"""Publish only the exact optimized, reviewed 2.5.384 candidate."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
from PIL import Image

root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.383/test-evidence.json'))
artifact=root/'outputs/mission384-candidate/EBOOT.PBP'
sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.384'
assert "'-O3','-flto'" in read(root/'build.ps1')
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.384' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha==baseline['film_sha256'],'Embedded cinematic changed'
allowed=set('campaign-runtime.h docking.h game.c game.h journey.h main.c mission-network-tests.h mission-reward-card.h mission-reward-review.h saga.h sectors.h station-crawl.h station-lave.h station-tour.h story.h tutorial-runtime.h'.split())
sources={}
for p in sorted((root/'src').rglob('*')):
 if not p.is_file():continue
 name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
 assert p.stat().st_mtime<=artifact.stat().st_mtime,f'Unbuilt source: {name}'
 if name not in allowed:assert sources[name]==baseline['source_sha256'][name],f'Unrelated source changed: {name}'
assert set(sources)==set(baseline['source_sha256'])|{'mission-reward-card.h','mission-reward-review.h'}
main=read(root/'src/main.c')
initializer=main[main.index('static void display_start_black'):main.index('static void display_recover')]
assert 'pspDebugScreenInit();' in initializer and 'PSP_DISPLAY_SETBUF_NEXTFRAME' in initializer
assert 'InitEx(' not in initializer.replace('InitEx(setup=0)','')
log=read(root/'outputs/mission384-tests.log')
suite=Path(re.search(r'^Review:\s*(.+)$',log,re.M)[1].strip())
checks={};runs={};reports={}
for case in json.loads(read(suite/'results.json')):
 name=case['test'];run=suite/name;t=read(Path(case['report']))
 reports[name]=t;runs[name]=str(run)
 assert digest(run/'EBOOT.PBP')==sha,f'Tests used another EBOOT: {name}'
 if name!='core-check':assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),name
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
assert checks['mission-reward-review']==37 and checks['intro-film-review']>=35 and checks['opening-check']==345
assert 'startup discards exactly twenty-three seconds' in reports['intro-film-review']
assert 'all 1280 contracts record exactly one correct reward' in reports['core-check']
core_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert core_fail==baseline['core_inherited_failures'] and re.search(r'^RESULT 15 failures$',reports['core-check'],re.M),'Core failures changed or report incomplete'
for name in ('radio','steering'):
 t=read(suite/'core-check'/(name+'-check.txt'))
 assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
 checks[name]=len(re.findall(r'^PASS ',t,re.M))
boot_log=read(root/'outputs/mission384-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip())
assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.384/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
for name in ('mission-reward-review','intro-film-review'):
 for p in (suite/name).glob('*.bmp'):
  im=Image.open(p);assert im.size==(480,272);im.save(screens/(p.stem+'.png'))
focused=sum(v for n,v in checks.items() if n!='core-check')
evidence=f'''ELITE: NEXT 2.5.384 - PSP display setup and mission transmissions
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Built on local 2.5.383; unrelated source hashes preserved.

Physical PSP startup regression: restores the full PSP SDK display initializer
and initial NEXTFRAME framebuffer latch, skipped by 2.5.383's setup=0 path.
Both full planes remain black; the first movie frame is prepared offscreen.
The suspected startup cause was isolated from the last-build display changes.
The physical PSP fix still requires the player's hardware confirmation.
Intro keeps its 23-second music skip, one-second audio/title fades, red/blue
CuLa tiles and no hidden startup greeting sound. Cinematic asset unchanged.

Completed board contracts, Guild assignments, story chapters, first flight,
passenger transfers and station fieldwork produce a received video-call card.
Native portraits match their canonical conversation/station identities.
Shows the actual credited reward, natural thanks, sliding warp entrance,
signal animation and margin fireworks. Fresh X continues; gameplay pauses.
Several dock payouts queue separately. Acknowledgement cannot fire a weapon,
skip the next card or pay again. Save/load keeps the committed payment and
progress but never reissues session-only presentation receipts. Save V32 stays
unchanged; bounded queue adds about 1.8 KiB, with no new framebuffer allocation.

{focused} focused emulator checks pass. All 1280 galaxy-wide board contracts
also record one correct receipt through real objective handlers. Native
480x272 captures inspected, including long text, large rewards and slide phases.
Core: {checks['core-check']} pass; same 15 inherited failures remain.
Physical PSP boot/audio playtest pending; emulator success is not certification.
Build: outputs/mission384-build.log
Review: {suite}
Normal boot: {boot}

Local release only. GitHub remains unchanged after the earlier automatic
approval rejection of pull/rebase on the mixed shared working tree.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
 folder.mkdir(parents=True,exist_ok=True)
 if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha),'Shared release changed'
 shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
 (folder/'BUILD-2.5.384.txt').write_text(evidence,encoding='utf-8')
 if folder!=root:
  (folder/'README.txt').write_text('ELITE: NEXT 2.5.384\nRestored full PSP display setup and mission-completion transmissions.\nEmulator tested; physical PSP startup confirmation needed.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
  (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.384',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,core_inherited_failures=core_fail,physical_psp='pending: fixes reported physical black-screen regression; confirmation required',git='local only; prior automatic approval rejected shared-tree pull/rebase')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.384.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for p in sorted(release.iterdir()):
  assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.384 — PSP screen setup and mission-completion transmissions

Responds to the physical PSP black-screen regression reported in 2.5.383: restores the PSP SDK's full display initialization and its first NEXTFRAME latch. Keeps both planes black and prepares the first movie frame offscreen. The display change is the suspected regression cause; hardware confirmation is still required. Intro film, 23-second music skip, one-second fades, CuLa tiles and silence during hidden setup are retained.

Actual mission payouts now queue a native 480×272 transmission card with the correct contact portrait, natural thanks, actual reward, animated warp arrival, signal lamps and fireworks clear of the text. Board missions, Guild claims, first flight, story chapters, station quests and passenger transfers are covered. Simulation pauses during the call. Fresh X acknowledges exactly one card, with an input-release gate before normal controls resume. Several dock completions receive separate cards; acknowledgements never award money. Receipts are transient and bounded; existing save V32 persists payment and progress without replaying celebrations after load.

Validation: {focused} focused emulator checks pass, including 37 new mission-card integration checks, normal boot, full intro with a real MP3, opening, radio and steering. All 1280 galaxy-wide contracts verify one correct receipt through real completion paths. Core: {checks['core-check']} pass with the same 15 inherited failures. Native screenshots inspected. Physical PSP black-screen fix needs playtesting. Exact optimized EBOOT SHA256 {sha}. Local release only; GitHub unchanged following the earlier automatic approval rejection of shared-tree pull/rebase.

Highest-priority remaining work: confirm cold boot and X-to-menu on the player's physical PSP before claiming the regression resolved; check mission celebrations in play; then remaining authored character parts, inherited reliability failures and paused planetary expansion.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;t=read(p)
 if not t.startswith('## 2.5.384'):p.write_text(entry+t,encoding='utf-8')
note='''## Mission completion and PSP startup — 2.5.384

Every committed contract/assignment payout records one session-only receipt: canonical title, contact, issuer and actual amount paid. The fullscreen received-transmission card freezes gameplay, uses the existing contact portrait identity, wraps native 8×8 text into reserved regions, and shows actual credited units. Warp/slide, signal and fireworks animations cannot cover the message. X acknowledges one card after entrance and release; no input leaks into firing or menus. Simultaneous dock payouts queue separately. Abandon/failure and ordinary initialization do not celebrate; loading preserves paid progress without reissuing cards. Police, docking and hyperspace cinematics take precedence. No save-format change or additional framebuffer.

Cold boot uses full PSP SDK display setup and an initial NEXTFRAME latch, both planes precleared black, with first movie preparation offscreen. This restores omitted setup after the physical PSP black-screen report; actual hardware confirmation remains an explicit release gate. Keep the existing 23-second audio skip, one-second fades, CuLa tiles and cinematic asset.

'''
for name in ('docs/DESIGN-BIBLE.md','docs/DESIGN-BIBLE-2.0.md'):
 p=root/name;t=read(p)
 if 'Mission completion and PSP startup — 2.5.384' not in t:p.write_text(note+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.384',sha256=sha,focused_passes=focused,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
