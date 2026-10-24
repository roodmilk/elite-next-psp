"""Publish only the exact locally tested portrait-repair candidate."""
from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile
from PIL import Image
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.378/test-evidence.json'))
artifact=root/'outputs/profile379-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.379'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.379' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper();assert film_sha==baseline['film_sha256']
allowed={'modular-portrait.h','portrait-parts.h','commander-profile-review.h','portrait-expansion-review.h','portrait-authored-parts.h','portrait-legacy-atlas.h','portrait-rig-atlas.h'}
sources={}
for p in sorted((root/'src').rglob('*')):
    if not p.is_file():continue
    name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
    assert p.stat().st_mtime<=artifact.stat().st_mtime,f'Unbuilt source: {name}'
    if name not in allowed:assert sources[name]==baseline['source_sha256'][name],f'Unrelated source changed: {name}'
assert set(sources)-set(baseline['source_sha256'])=={'portrait-authored-parts.h','portrait-legacy-atlas.h','portrait-rig-atlas.h'}
host=json.loads(read(root/'outputs/portrait379-host/audit.json'))
independent=json.loads(read(root/'outputs/profile379-audit/source-checks.json'))
assert host['checks']==17993 and host['render_cases']==9774 and not host['failures']
assert independent['checks']==1265 and independent['legacy_faces']==96 and not independent['failures']
assert (root/'outputs/portrait379-host/portrait379.dll').stat().st_mtime>=max((root/'src'/n).stat().st_mtime for n in ('modular-portrait.h','portrait-parts.h','portrait-authored-parts.h','portrait-legacy-atlas.h'))
suite_log=read(root/'outputs/profile379-tests.log');suite=Path(re.search(r'^Review:\s*(.+)$',suite_log,re.M)[1].strip())
results=json.loads(read(suite/'results.json'));checks={};runs={};reports={}
for result in results:
    name=result['test'];report=Path(result['report']);run=report.parent;t=read(report)
    assert digest(run/'EBOOT.PBP')==sha and re.search(r'^RESULT \d+ failures$',t,re.M)
    if name!='core-check':assert result['passed'] and 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
    checks[name]=len(re.findall(r'^PASS ',t,re.M));runs[name]=str(run);reports[name]=t
assert checks['portrait-parts-review']==60 and checks['profile-review']==25 and checks['opening-check']==345
old_core=root/'outputs/portrait376-release-20261010-172949-509/core-check/game-check.txt'
old_fail=re.findall(r'^FAIL .+$',read(old_core),re.M);new_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert len(old_fail)==15 and new_fail==old_fail,'New core regression'
for name in ('radio','steering'):
    t=read(Path(runs['core-check'])/(name+'-check.txt'));assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
    checks[name]=len(re.findall(r'^PASS ',t,re.M))
boot_log=read(root/'outputs/profile379-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip());assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.379/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
for name in ('portrait-parts-review','profile-review'):
    for p in Path(runs[name]).glob('*.bmp'):
        with Image.open(p) as im:
            assert im.size==(480,272);im.save(screens/(p.stem+'.png'))
for p in (root/'outputs/profile379-audit').glob('*before-after*.png'):
    with Image.open(p) as im:assert im.size==(480,272)
    shutil.copy2(p,screens/p.name)
focused=sum(v for n,v in checks.items() if n!='core-check')
evidence=f'''ELITE: NEXT 2.5.379 — Clean legacy photos and authored female parts
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Built on local 2.5.378; Engineers hull preview and CuLa title preserved.
Build: outputs/profile379-build.log

Legacy sheets have nonuniform row heights. Reviewed row gutters prevent the
previous row's shoulders becoming a red strip across the next face's crown.
Legacy whole-face rendering retains authored RGB1555 colours instead of
guessing hair/garment regions by hue/rectangle and tinting the exposed neck.
Face index and saved four-byte DNA remain unchanged; no save migration.
Female Human/Android Cropped hair, Smirk and extended/pointed ears have
separate native pixel-art parts with fixed rig fits, material ownership and
guarded occlusion. Ear choices cannot restore rectangular bob-hair patches.
Original v2/v3 rig atlas and modular palette masks remain unchanged.

All-category authoring standard recorded in docs/DESIGN-BIBLE.md. Other
transformed feature options still need authored replacements and visual
review before the complete creator can be called professionally finished.

{focused} focused emulator checks pass on this exact executable:
'''+''.join(f'{n}: {v} PASS; {runs.get(n,runs["core-check"])}\n' for n,v in checks.items())+'''
Core retains the same 15 inherited failures; no new core failures.
Host: 17,993 checks / 9,774 render cases pass, with per-feature isolation,
independent neck/face landmarks, migration and mixed female hair/skin sets.
Independent source audit: 1,265 checks pass across all 96 legacy faces at
20/32/80/96px, including exact foreground colour and clean gutter checks.
The shipped old renderer fails the independent corrected-source comparison.
Native 480x272 profile/editor/feature comparison screenshots inspected.
Lossless row spans and twelve original rig tiles use 543,408 bytes including
the 8KB legacy decode cache, saving 243,024 bytes against the old atlas.
Normal emulator startup and framebuffer guards pass. Physical PSP testing
remains pending. An earlier duplicate-atlas candidate failed heap allocation
and was discarded; only the reduced-memory candidate is packaged.
Every unrelated source hash matches 2.5.378; embedded film unchanged.
Git pull blocked by existing untracked music placeholders; autostash restored.
Mixed shared work preserved. Local only; GitHub not updated.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
Highest priority: player verification of Ben/Jameson profile on hardware,
then authored replacements for remaining transformed creator options,
inherited reliability failures and the paused planetary work.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
    if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha),'Shared release changed'
    shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
    (folder/'BUILD-2.5.379.txt').write_text(evidence,encoding='utf-8')
    if folder!=root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.379\nClean legacy photos; authored female crop and smirk.\nFocused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.379',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,host_checks=host['checks'],host_renders=host['render_cases'],independent_source_checks=independent['checks'],core_inherited_failures=new_fail,physical_psp='pending',git='local only; untracked music placeholders block pull')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.379.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.379 — Clean legacy photos and authored female parts

Corrected source-sheet row boundaries for all 96 legacy portraits, removing previous-row shoulder strips at the crown. Legacy whole-face colours remain authored instead of broad hair/outfit recolouring that tinted necks. Face identity, saved DNA and v2/v3 rig alignment remain intact. Female Human/Android Cropped hair, Smirk and extended/pointed ears now use native authored parts; ear choices cannot restore rectangular bob-hair patches. All-category fit/mask/palette production standard is recorded in docs/DESIGN-BIBLE.md; remaining transformed choices still require conversion and visual review. Preserves 378 Engineers preview, 377 title and earlier work.

{focused} focused emulator checks pass, plus 17,993 host checks and 1,265 independent source checks. Actual native editor/profile/comparison screenshots inspected. Lossless atlas packing saves 243,024 bytes, with all twelve modular rig source tiles byte-identical. Core retains the same 15 inherited failures. Physical PSP testing and the player's exact Ben/Jameson save verification remain pending. Details: BUILD-2.5.379.txt. Highest priority: player profile playtest, remaining authored creator conversions, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.379. SHA256: {sha}. GitHub not updated: existing untracked music placeholders block pull; autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;t=read(p)
    if not t.startswith('## 2.5.379'):p.write_text(entry+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.379',sha256=sha,focused_passes=focused,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
