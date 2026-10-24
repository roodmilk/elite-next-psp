"""Package only the exact reviewed portrait executable and verified evidence."""
from pathlib import Path
import ctypes as c,hashlib,json,re,shutil,struct,zipfile
from PIL import Image,ImageChops
root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
artifact=root/'outputs/portrait364-final/EBOOT.PBP';sha=digest(artifact)
data=artifact.read_bytes();offsets=struct.unpack_from('<8I',data,8)
assert b'ELITE: NEXT 2.5.364' in data[offsets[0]:offsets[1]]
runs={};counts={}
for name in ('parts','opening','boot','deck'):
 log=read(root/f'outputs/portrait364-{name}.log')
 assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ',log,re.M),name
 match=re.search(r'^Review:\s*(.+?)(?=\n\S|\Z)',log,re.M|re.S)
 run=Path(re.sub(r'\r?\n\s*','',match[1]).strip());assert digest(run/'EBOOT.PBP')==sha,name
 runs[name]=run;counts[name]=len(re.findall(r'^PASS ',log,re.M))
assert counts==dict(parts=43,opening=343,boot=2,deck=36),counts
host=root/'outputs/portrait364-host';audit=json.loads(read(host/'audit-final/exhaustive-audit.json'))
feature=json.loads(read(host/'audit-final/feature-audit.json'));v2=json.loads(read(host/'v2-audit.json'))
assert audit['combinations']==330912 and audit['renders']==992736 and audit['structural_failures']==0
assert all(item['identical_portraits']==0 for item in audit['collisions'].values())
assert feature['checks']==12162 and not feature['failures']
assert v2['combinations']==14272 and v2['renders']==42816 and not v2['structural_failures']
sources={}
for name in ('modular-portrait.h','portrait-parts.h','portrait-dna.h','portrait-material-masks.h','portrait-atlas.h'):
 p=root/'src'/name;assert p.stat().st_mtime<=(host/'portrait-verified.dll').stat().st_mtime,name
 sources[name]=digest(p)

# Independent portability check: every captured feature comparison and every
# species preview must match the exhaustively tested host C renderer exactly.
lib=c.CDLL(str(host/'portrait-verified.dll'));lib.ph_dna.restype=lib.ph_adjust.restype=c.c_uint
lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int];lib.ph_count.argtypes=lib.ph_value.argtypes=[c.c_uint,c.c_int]
lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
def select(d,p,value):
 while lib.ph_value(d,p)!=value:d=lib.ph_adjust(d,p,1)
 return d
def base(sp):
 d=lib.ph_dna(sp)
 if lib.ph_count(d,3)>1:d=select(d,3,1)
 return select(d,5,1)
def render(d,side):
 raw=(c.c_uint*(side*side))();lib.ph_render(d,side,raw)
 im=Image.new('RGB',(side,side));im.putdata([(v&255,(v>>8)&255,(v>>16)&255) for v in raw]);return im
crop_checks=0
def equal(im,x,y,side,d):
 global crop_checks
 expected=render(d,side).crop((1,1,side-1,side-1))
 actual=im.crop((x+1,y+1,x+side-1,y+side-1))
 assert ImageChops.difference(expected,actual).getbbox() is None,(x,y,side,hex(d))
 crop_checks+=1
for part in (0,1,2,3,4,5,6,8,9):
 a=base(0);b=lib.ph_adjust(a,part,1);im=Image.open(runs['parts']/f'comparison-{part:02d}.bmp').convert('RGB')
 assert im.size==(480,272)
 for x,d in ((24,a),(264,b)):
  equal(im,x,44,96,d);equal(im,x+112,44,20,d);equal(im,x+112,76,32,d)
for kind in ('species-parts','species-max'):
 im=Image.open(runs['parts']/(kind+'.bmp')).convert('RGB')
 for sp in range(10):
  d=base(sp)
  if kind=='species-max':
   for part in range(1,10):d=select(d,part,lib.ph_count(d,part)-1)
  x=14+(sp%5)*94;y=32+(sp//5)*108;equal(im,x,y,64,d);equal(im,x+68,y+4,20,d)
old=Image.open(root/'outputs/opening-check-20261010-115945-032/portrait-creator-277.bmp').convert('RGB').crop((24,82,120,178))
legacy=Image.open(runs['parts']/'legacy-preserved.bmp').convert('RGB').crop((24,82,120,178))
assert ImageChops.difference(old,legacy).getbbox() is None

failures=lambda s:re.findall(r'^FAIL .+$',s,re.M)
assert 'Failed game checks' in read(root/'outputs/portrait364-smoke-error.log')
smoke=next(p.resolve() for p in sorted((root/'../../work').glob('smoke-20*'),reverse=True) if (p/'EBOOT.PBP').exists() and digest(p/'EBOOT.PBP')==sha)
baseline=failures(read(root/'outputs/planets359-core.log'))
assert len(baseline)==15 and failures(read(smoke/'game-check.txt'))==baseline
assert 'RESULT 15 failures' in read(smoke/'game-check.txt')
for name in ('radio','steering'):assert 'RESULT 0 failures' in read(smoke/(name+'-check.txt'))
input_text=read(smoke/'input-check.txt');input_fail=failures(input_text)
input_complete=bool(re.search(r'^RESULT ',input_text,re.M))
input_status='completed' if input_complete else 'incomplete with the extended 600-second allowance'
input_baseline=read(root/'../../work/smoke-20261010-111439-530/input-check.txt')
input_extra=sorted(set(input_fail)-set(failures(input_baseline)))
# The extended run covers more tests than the truncated previous baseline.
# Keep that uncovered portion explicitly unclassified, never call it a pass
# or claim that its failures are proven pre-existing.
assert all(i+1>len(input_baseline.splitlines()) for i,line in enumerate(input_text.splitlines()) if line in input_extra)
cases=int(re.search(r'INFO (\d+) real foreground render cases',read(root/'outputs/portrait364-parts.log'))[1])
previous='F6CBD5DDC6C65282544942F1C04C8BF20F26E3DD4915B41C9B39D7A756BD1ECC'
release=root/'outputs/releases/2.5.364/ELITE-NEXT-PSP'
for target in (root,root/'dist/ELITE-NEXT-PSP',release):
 if (target/'EBOOT.PBP').exists():assert digest(target/'EBOOT.PBP') in (previous,sha),'Shared release changed'
entry=f'''## 2.5.364 — Portrait anatomy and complete combination audit

Replaces the neck-breaking clothing cutoff and rough alien polygons with reviewed pixel material maps for all ten species. Corrects feature socket alignment and separates skin, neck, collar, scalp, crest, feathers, hood and iris regions. Adds 12 skin/shell colours, 12 hair/crest colours and six uniforms, with additional supported head, eye, mouth, ear and cheek-mark styles. Hidden anatomy is not advertised. New DNA v3 retains existing selected feature meanings and the V32 save field; v2 upgrades preserve fields and appearance, while v0/v1 saved looks keep their original renderer until explicit New Look.

Every one of 330,912 new selectable combinations and 14,272 v2 combinations rendered at 96/32/20px: 1,035,552 renders. 12,162 all-pairs/material/anatomy checks pass. Actual PPSSPP: 424 focused checks pass, including 43 portrait/editor/save checks; {crop_checks} native portrait crops match the exhaustive host C renderer pixel for pixel. All feature sheets and representative stress combinations/native captures inspected; comparisons shown before packaging. This is automated exhaustive coverage, not a claim that every combination was manually viewed. Physical PSP testing remains pending.

Broad smoke retains the same 15 inherited game failures; radio/steering pass. Extended input smoke {input_status}: {len(input_fail)} observed failures, including {len(input_extra)} unique failures beyond the old truncated baseline. Those additional failures are unclassified, not proven pre-existing or newly introduced. Local release only; upstream pull remains blocked by pre-existing untracked music placeholders, autostash restored and unrelated shared work retained. GitHub not updated. SHA256: {sha}. Details: docs/COMMANDER-CREATION-364.md and BUILD-2.5.364.txt. Full game certification remains open.

'''
evidence=f'''ELITE: NEXT 2.5.364 — Portrait anatomy, options and exhaustive audit
SHA256: {sha}
Build: outputs/portrait364-final-build3; log outputs/portrait364-final-build3.log
Source documentation: docs/COMMANDER-CREATION-364.md

All 330,912 selectable v3 combinations and 14,272 v2 combinations rendered
at 96/32/20px: 1,035,552 production-C renders, zero structural failures.
All v3 portrait pairs distinct; hash collision candidates compared pixelwise.
12,162 all-pairs/anatomy checks pass, including independent neck/collar
fixtures across all skin/hair/uniform colours, isolated controls and contrast.
Every option sheet and representative mixed stress portraits/native captures inspected.
Every combination was automatically checked, not manually inspected.
{crop_checks} actual native PSP crops match the host renderer pixel for pixel.
Legacy crop remains pixel-identical to the shipped 2.5.361 reference.
Comparisons shown before packaging. Physical PSP testing pending.

'''+''.join(f'{name}: {counts[name]} PASS; {runs[name]}\n' for name in counts)+f'''
424 focused emulator checks pass; {cases} foreground feature render cases.
Includes actual input, inactive fields, 50 real V32 save/load round trips,
maximum expanded indices and v2 migration without appearance changes.
New v3 records require this or a compatible newer EBOOT. Keep the previous
save backup if downgrading. Original v0/v1 faces remain unchanged.
Broad smoke: same 15 inherited game failures; radio/steering pass.
Input smoke {input_status}; {len(input_fail)} observed failures.
{len(input_extra)} unique failures occur beyond previous baseline coverage and remain
unclassified. This is not whole-game regression certification.
Smoke: {smoke}
Root/dist/versioned/ZIP EBOOT bytes match. Local release; no GitHub update.
Replace only EBOOT.PBP; retain music, saves and settings.
'''
evidence+='\nUnclassified failures beyond previous input coverage:\n'+'\n'.join(input_extra)+'\n'
comparisons=release.parent/'comparisons';comparisons.mkdir(parents=True,exist_ok=True)
for file in runs['parts'].glob('*.bmp'):
 im=Image.open(file);assert im.size==(480,272);im.save(comparisons/(file.stem+'.png'))
manifest=dict(version='2.5.364',sha256=sha,checks=counts,host=audit,old_v2=v2,native_matching_crops=crop_checks,renderer_source_sha256=sources,smoke=str(smoke),known_broad_failures=15,input_complete=input_complete,input_observed_failures=len(input_fail),input_unclassified_unique_failures=input_extra,physical_psp='pending')
(release.parent/'test-evidence.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
for target in (root,root/'dist/ELITE-NEXT-PSP',release):
 target.mkdir(parents=True,exist_ok=True);shutil.copy2(artifact,target/'EBOOT.PBP')
 (target/'BUILD-2.5.364.txt').write_text(evidence,encoding='utf-8')
 if target!=root:
  (target/'README.txt').write_text('ELITE: NEXT 2.5.364\nReviewed portrait anatomy; expanded colour and feature choices.\n345,184 complete combinations checked at 96/32/20px.\n424 focused emulator checks pass. Core: 15 inherited failures.\nExtended input checks remain failing and incomplete; see BUILD-2.5.364.txt.\nPhysical PSP testing pending; local release only.\nNew v3 portraits require this or a compatible newer EBOOT.\nKeep previous save backups if downgrading.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
  (target/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.364.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
 for p in sorted(release.rglob('*')):
  if p.is_file():assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None and hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;s=read(p)
 if not s.startswith('## 2.5.364'):p.write_text(entry+s,encoding='utf-8')
for name in ('DESIGN-BIBLE.md','DESIGN-BIBLE-2.0.md'):
 p=root/'docs'/name;s=read(p)
 if not s.startswith('<!-- 2.5.364 -->'):p.write_text('<!-- 2.5.364 -->\nCommander portraits require authored pixel material regions including neck/collar boundaries, local anatomy, useful small-size feature changes and actual save/input validation. Exhaustive selectable-combination rendering must be distinguished from manual visual review. Details: COMMANDER-CREATION-364.md.\n\n'+s,encoding='utf-8')
p=root/'docs/COMMANDER-CREATION-364.md';s=read(p)
if '## Final evidence' not in s:p.write_text(s+f'\n## Final evidence\n\n424 focused PPSSPP checks pass; {crop_checks} native crops match the exhaustive host renderer. All 345,184 selectable v2/v3 combinations rendered at three sizes. Exact runs, hash and inherited broad-suite limitations: BUILD-2.5.364.txt. Native comparisons: outputs/releases/2.5.364/comparisons.\n',encoding='utf-8')
print(json.dumps(dict(version='2.5.364',sha256=sha,checks=counts,native_crops=crop_checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
