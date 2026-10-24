"""Package only the final verified EBOOT; preserve unrelated shared work."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile
from PIL import Image

root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline=json.loads(read(root/'outputs/releases/2.5.375/test-evidence.json'))
artifact=root/'outputs/portrait376-release-candidate/EBOOT.PBP';sha=digest(artifact)
assert read(root/'VERSION').strip()=='2.5.376'
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.376' in data[offsets[0]:offsets[1]]
film_sha=hashlib.sha256(data[offsets[7]:]).hexdigest().upper();assert film_sha==baseline['film_sha256']
allowed={'modular-portrait.h','portrait-dna.h','portrait-parts.h','commander-ui.h','opening-runtime.h','portrait-parts-review.h','commander-profile-review.h','portrait-expansion-review.h','portrait-female-masks.h'}
sources={}
for p in sorted((root/'src').rglob('*')):
    if not p.is_file():continue
    name=p.relative_to(root/'src').as_posix();sources[name]=digest(p)
    assert p.stat().st_mtime<=artifact.stat().st_mtime, f'Unbuilt source: {name}'
    old=baseline['source_sha256'].get(name)
    if old and name not in allowed:assert sources[name]==old,f'Unrelated source changed: {name}'
host=root/'outputs/portrait376-host';audit=json.loads(read(host/'audit.json'))
assert audit['checks']==17993 and audit['render_cases']==9774 and not audit['failures']
assert (host/'portrait376.dll').stat().st_mtime>=max((root/'src'/name).stat().st_mtime for name in ('modular-portrait.h','portrait-parts.h','portrait-dna.h','portrait-female-masks.h'))
legacy=root/'outputs/portrait364-host/audit-376-compat'
old_audit=json.loads(read(legacy/'exhaustive-audit.json'))
assert old_audit['combinations']==330912 and old_audit['renders']==992736 and old_audit['structural_failures']==0
assert not any(v['identical_portraits'] for v in old_audit['collisions'].values())
assert digest(legacy/'combinations.bin')==digest(root/'outputs/portrait364-host/audit-final/combinations.bin')
suite_log=read(root/'outputs/portrait376-release-tests.log')
suite=Path(re.search(r'^Review:\s*(.+)$',suite_log,re.M)[1].strip());results=json.loads(read(suite/'results.json'))
checks={};runs={};reports={}
for result in results:
    name=result['test'];report=Path(result['report']);run=report.parent;text=read(report)
    assert digest(run/'EBOOT.PBP')==sha,name
    assert re.search(r'^RESULT \d+ failures$',text,re.M),name
    if name!='core-check':assert result['passed'] and 'RESULT 0 failures' in text and not re.search(r'^FAIL ',text,re.M),name
    checks[name]=len(re.findall(r'^PASS ',text,re.M));runs[name]=str(run);reports[name]=text
assert checks['portrait-parts-review']==60
assert checks['profile-review']>=22 and checks['opening-check']>=343 and checks['command-deck-review']>=51
core=Path(runs['core-check']);old_core=Path(read(root/'outputs/conversation374-core-path.txt').strip())
old_fail=re.findall(r'^FAIL .+$',read(old_core/'game-check.txt'),re.M)
new_fail=re.findall(r'^FAIL .+$',reports['core-check'],re.M)
assert len(old_fail)==15 and new_fail==old_fail,'Core regression differs from the documented baseline'
for name in ('radio','steering'):
    text=read(core/(name+'-check.txt'));assert 'RESULT 0 failures' in text and not re.search(r'^FAIL ',text,re.M)
    checks[name]=len(re.findall(r'^PASS ',text,re.M))
boot_log=read(root/'outputs/portrait376-boot.log');assert 'RESULT 0 failures' in boot_log
boot=Path(re.search(r'^Review:\s*(.+)$',boot_log,re.M)[1].strip());assert digest(boot/'EBOOT.PBP')==sha
checks['boot']=len(re.findall(r'^PASS ',boot_log,re.M));runs['boot']=str(boot)
release=root/'outputs/releases/2.5.376/ELITE-NEXT-PSP';release.mkdir(parents=True,exist_ok=True)
screens=release.parent/'screenshots';screens.mkdir(exist_ok=True)
portrait=Path(runs['portrait-parts-review']);profile=Path(runs['profile-review'])
for name in ('expanded-editor-0-0','expanded-editor-0-1','expanded-editor-7-0','expanded-editor-7-1','expanded-gender','expanded-blonde-blue','expanded-wide-eyes','expanded-styles-0-0','expanded-styles-0-1','expanded-styles-7-0','expanded-styles-7-1'):
    im=Image.open(portrait/(name+'.bmp'));assert im.size==(480,272);im.save(screens/(name+'.png'))
im=Image.open(profile/'commander-profile-expanded-female.bmp');assert im.size==(480,272);im.save(screens/'commander-profile-expanded-female.png')
passed=sum(n for name,n in checks.items() if name!='core-check')
evidence=f'''ELITE: NEXT 2.5.376 — Expanded Commander portraits
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Built on local 2.5.375; all unrelated baseline-tracked source hashes retained.
Build: outputs/portrait376-release-build.log

Human/Android Male and Female controls, four hairstyle choices, five eye styles
for rigs with eyes, independently traced female anatomy, stronger shaded hair
colours for female/new styles, readable freckles, aligned feature sockets and
bounded native 480x272 editor. Classic male art retains its original palette.
Existing V32 save struct unchanged; expanded validated four-byte appearance.
Old appearance remains intact; inactive selections survive opening the editor.
Preserves 375 dealer-based Mega shipyards and 374 conversation ownership.

Final executable verification:
'''+''.join(f'{name}: {n} PASS; {runs.get(name,runs["core-check"])}\n' for name,n in checks.items())+f'''
{passed} focused/startup/radio/steering checks pass. Core has the same 15 inherited
failures as 374; no new failures. Full extended input smoke not repeated.
Portrait suite includes real V32 save/load, actual D-pad input, every expanded
option at 20/32/96px, malformed data, old-appearance migration and guard words.
Profile suite includes 800 theme/species/version photo comparisons and saves.
Native screenshots visually inspected: outputs/releases/2.5.376/screenshots.

Host: 17,993 checks, 9,774 new portrait render cases, zero failures. Includes
every choice pair in two contexts at three sizes, 2,304 mixed feature sets,
300 migrations and independent female face/scalp/neck material landmarks.
Legacy: 330,912 combinations and 992,736 renders, no structural failures or
identical portraits. The complete DNA/render record is byte-identical to the
previous verified record. Exhaustive coverage is automated, not manual review
of every combination. Final crown/editor tweaks affect expanded paths only.

Physical PSP testing pending. This is not a game-wide zero-bug certification.
Git pull blocked by existing untracked music placeholders; autostash restored.
Mixed shared work preserved. Local only; GitHub not updated.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
Highest-priority remaining work: hardware portrait/controls playtest, inherited
reliability failures, then the paused planetary work.
'''
for folder in (root,root/'dist/ELITE-NEXT-PSP',release):
    if (folder/'EBOOT.PBP').exists():assert digest(folder/'EBOOT.PBP') in (baseline['sha256'],sha),'Shared release changed'
    shutil.copy2(artifact,folder/'EBOOT.PBP');assert digest(folder/'EBOOT.PBP')==sha
    (folder/'BUILD-2.5.376.txt').write_text(evidence,encoding='utf-8')
    if folder!=root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.376\nExpanded Commander portraits, hairstyles, eyes and gender.\nFocused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
record=dict(version='2.5.376',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,host_checks=audit['checks'],host_renders=audit['render_cases'],legacy_combinations=330912,legacy_renders=992736,legacy_record_sha256=digest(legacy/'combinations.bin'),core_inherited_failures=new_fail,physical_psp='pending',full_smoke='extended input not repeated; inherited issues remain')
(release.parent/'test-evidence.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.376.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.pbp','.txt');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
entry=f'''## 2.5.376 — Expanded Commander portraits

Added Male/Female Human and Android rigs and four hairstyle choices with separately traced anatomy. Twelve hair colours use stronger four-tone shading on female/new styles; Classic male palettes preserve existing art. Eye styles expand from three to five on rigs with eyes. Correct female eye, mouth and ear alignment, readable cheek freckles, crown padding and a one-screen native 480×272 editor. Changing features retains the other selected values, including inactive choices when entering the editor. Old appearance records and V32 save structure remain compatible; expanded appearance uses a validated four-byte encoding. Preserves the 375 Mega shipyard and 374 conversation fixes.

{passed} focused/startup/radio/steering emulator checks pass. Portrait input and real saves verified; 800 profile photo/theme comparisons; actual native editor/profile screenshots inspected. Host: 17,993 checks and 9,774 render cases pass. All 330,912 older combinations rerendered at three sizes with byte-identical render records. Core has the same 15 inherited failures; physical PSP testing remains pending. Details: docs/COMMANDER-CREATION-376.md and BUILD-2.5.376.txt. Highest priority: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.376. SHA256: {sha}. GitHub not updated; existing untracked music placeholders block pull and mixed shared work is preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name;t=read(p)
    if not t.startswith('## 2.5.376'):p.write_text(entry+t,encoding='utf-8')
p=root/'docs/DESIGN-BIBLE.md';t=read(p)
if not t.startswith('## Implemented Commander portraits (2.5.376)'):
    p.write_text('## Implemented Commander portraits (2.5.376)\n\nHuman and Android commanders support Male/Female rigs, four hairstyle choices and twelve traced hair palettes with stronger shading for new styles. Rigs with eyes have five eye styles. Keep unsupported anatomy hidden, other selected features independent, and saved appearance intact. The expanded appearance fits the existing four-byte field without changing the V32 save struct. Every control must remain readable on one native 480×272 editor screen and at 20/32px dialogue size. Details and verification limits: COMMANDER-CREATION-376.md.\n\n'+t,encoding='utf-8')
print(json.dumps(dict(version='2.5.376',sha256=sha,focused_passes=passed,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
