"""Attach the exhaustive portrait audit to the newer, already tested release.

Never replace its executable or discard the separately verified intro work.
"""
from pathlib import Path
import ctypes as c, hashlib, json, re, zipfile
from PIL import Image, ImageChops

root=Path(__file__).resolve().parents[1]
read=lambda p:p.read_text(encoding='utf-8-sig')
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
release=root/'outputs/releases/2.5.365/ELITE-NEXT-PSP'
artifact=release/'EBOOT.PBP'; sha=digest(artifact)
assert sha=='359630D396FAD70FC1E4F5055C6A14AD948569C106DE85F852D136132DEF4571'
assert read(root/'VERSION').strip()=='2.5.365'
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP'):
    assert digest(p)==sha
host=root/'outputs/portrait364-host'
audit=json.loads(read(host/'audit-final/exhaustive-audit.json'))
feature=json.loads(read(host/'audit-final/feature-audit.json'))
v2=json.loads(read(host/'v2-audit.json'))
assert audit['combinations']==330912 and audit['renders']==992736 and not audit['structural_failures']
assert all(not v['identical_portraits'] for v in audit['collisions'].values())
assert feature['checks']==12162 and not feature['failures']
assert v2['combinations']==14272 and v2['renders']==42816 and not v2['structural_failures']
sources={}
for n in ('modular-portrait.h','portrait-parts.h','portrait-dna.h','portrait-material-masks.h','portrait-atlas.h','commander-ui.h','portrait-parts-review.h'):
    p=root/'src'/n; sources[n]=digest(p)
    assert sources[n]==digest(root/'outputs/intro365-source/src'/n),n
    assert p.stat().st_mtime<=(host/'portrait-verified.dll').stat().st_mtime,n

integration=root/'outputs/intro-integration-20261010-143529-857'
runs={}; checks={}
for name,expected in (('opening-check',343),('multiplayer-console-review',58),('portrait-parts-review',43),('command-deck-review',36)):
    p=integration/name; t=read(p/(name+'.txt'))
    assert digest(p/'EBOOT.PBP')==sha,name
    assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M),name
    checks[name]=len(re.findall(r'^PASS ',t,re.M));assert checks[name]==expected
    runs[name]=p
for name,expected in (('audio',23),('boot',2)):
    t=read(root/f'outputs/intro365-release-{name}.log')
    assert 'RESULT 0 failures' in t and not re.search(r'^FAIL ',t,re.M)
    p=Path(re.search(r'^Review:\s*(.+)$',t,re.M)[1].strip())
    assert digest(p/'EBOOT.PBP')==sha
    checks[name]=len(re.findall(r'^PASS ',t,re.M));assert checks[name]==expected
assert sum(checks.values())==505

lib=c.CDLL(str(host/'portrait-verified.dll'))
lib.ph_dna.restype=lib.ph_adjust.restype=c.c_uint
lib.ph_adjust.argtypes=[c.c_uint,c.c_int,c.c_int]
lib.ph_count.argtypes=lib.ph_value.argtypes=[c.c_uint,c.c_int]
lib.ph_render.argtypes=[c.c_uint,c.c_int,c.POINTER(c.c_uint)]
def select(d,p,v):
    for _ in range(16):
        if lib.ph_value(d,p)==v:return d
        d=lib.ph_adjust(d,p,1)
    raise AssertionError((p,v))
def base(sp):
    d=lib.ph_dna(sp)
    if lib.ph_count(d,3)>1:d=select(d,3,1)
    return select(d,5,1)
def render(d,side):
    a=(c.c_uint*(side*side))();lib.ph_render(d,side,a)
    im=Image.new('RGB',(side,side));im.putdata([(v&255,(v>>8)&255,(v>>16)&255) for v in a]);return im
crop_checks=0
def equal(im,x,y,side,d):
    global crop_checks
    expected=render(d,side).crop((1,1,side-1,side-1))
    actual=im.crop((x+1,y+1,x+side-1,y+side-1))
    assert ImageChops.difference(expected,actual).getbbox() is None,(x,y,side,hex(d))
    crop_checks+=1
parts=runs['portrait-parts-review']
for part in (0,1,2,3,4,5,6,8,9):
    a=base(0);b=lib.ph_adjust(a,part,1)
    im=Image.open(parts/f'comparison-{part:02d}.bmp').convert('RGB');assert im.size==(480,272)
    for x,d in ((24,a),(264,b)):
        equal(im,x,44,96,d);equal(im,x+112,44,20,d);equal(im,x+112,76,32,d)
for kind in ('species-parts','species-max'):
    im=Image.open(parts/(kind+'.bmp')).convert('RGB');assert im.size==(480,272)
    for sp in range(10):
        d=base(sp)
        if kind=='species-max':
            for p in range(1,10):d=select(d,p,lib.ph_count(d,p)-1)
        x=14+(sp%5)*94;y=32+(sp//5)*108
        equal(im,x,y,64,d);equal(im,x+68,y+4,20,d)
old=Image.open(root/'outputs/opening-check-20261010-115945-032/portrait-creator-277.bmp').convert('RGB').crop((24,82,120,178))
legacy=Image.open(parts/'legacy-preserved.bmp').convert('RGB').crop((24,82,120,178))
assert ImageChops.difference(old,legacy).getbbox() is None
assert crop_checks==94

# This additional broader run was on the 2.5.364 portrait candidate, not 365.
# Preserve that distinction and the newly exposed, unclassified failures.
smoke=(root/'../../work/smoke-20261010-143904-967').resolve()
failures=lambda t:re.findall(r'^FAIL .+$',t,re.M)
core=read(smoke/'game-check.txt');assert 'RESULT 15 failures' in core
assert failures(core)==failures(read(root/'outputs/planets359-core.log'))
inp=read(smoke/'input-check.txt');baseline=read(root/'../../work/smoke-20261010-111439-530/input-check.txt')
extra=sorted(set(failures(inp))-set(failures(baseline)))
assert len(failures(inp))==130 and len(extra)==17 and 'RESULT ' not in inp
assert all(i+1>len(baseline.splitlines()) for i,s in enumerate(inp.splitlines()) if s in extra)

comparison_dir=release.parent/'portrait-comparisons';comparison_dir.mkdir(exist_ok=True)
for p in parts.glob('*.bmp'):
    im=Image.open(p);assert im.size==(480,272);im.save(comparison_dir/(p.stem+'.png'))
manifest=dict(version='2.5.365',sha256=sha,focused_checks=checks,host=audit,old_v2=v2,feature_checks=feature['checks'],native_matching_crops=crop_checks,renderer_source_sha256=sources,additional_broad_candidate='2.5.364',additional_broad_run=str(smoke),core_inherited_failures=15,input_complete=False,input_observed_failures=130,input_unclassified_unique_failures=extra,physical_psp='pending')
(release.parent/'portrait-test-evidence.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
section='''
EXHAUSTIVE PORTRAIT AUDIT — included in this exact 2.5.365 executable
All seven portrait/editor/review headers match the frozen 365 source snapshot.
330,912 selectable v3 + 14,272 v2 combinations at 96/32/20px:
1,035,552 production-C renders; zero structural failures or identical v3 looks.
12,162 foreground/material/anatomy feature checks pass.
94 native crops from this exact 365 EBOOT match that C renderer pixel for pixel.
The existing 505 focused checks include 43 portrait/editor/save checks,
2,292 foreground render cases and 50 real V32 save/load round trips.
All option sheets and representative mixed/native portraits visually reviewed;
every combination was automatically checked, not manually inspected.
Neck/collar, hair, eyelids and local anatomy have independent material masks.
Adds 12 skin/shell colours, 12 hair/crest colours, six uniforms and supported
head/eye/mouth/ear/cheek styles. Unsupported controls are hidden.
Legacy looks preserved; v2 migration retains fields and appearance.
New v3 portraits require this or a compatible newer EBOOT; back up saves
before downgrading. Physical PSP testing remains pending.
Native comparisons: portrait-comparisons. Manifest: portrait-test-evidence.json.
Details: docs/COMMANDER-CREATION-365.md.

Additional broad run on the separately reviewed 2.5.364 portrait candidate:
Core completed with the same 15 inherited failures. Radio and steering pass.
The 600-second input run remained incomplete: 130 observed failures.
17 unique failures occur beyond the old truncated baseline (first at line
1581 versus 1260 baseline lines). They remain unclassified, not proven
pre-existing or newly introduced. This is not whole-game certification.
These are not claimed as new broad test results for the 365 executable.
'''+str(smoke)+'\n'+'\n'.join(extra)+'\n'
original_build=read(release/'BUILD-2.5.365.txt')
for p in (root/'BUILD-2.5.365.txt',root/'dist/ELITE-NEXT-PSP/BUILD-2.5.365.txt',release/'BUILD-2.5.365.txt'):
    t=read(p) if p.exists() else original_build
    if 'EXHAUSTIVE PORTRAIT AUDIT' not in t:p.write_text(t+section,encoding='utf-8')
doc=read(root/'docs/COMMANDER-CREATION-364.md').replace('# Commander Creation — 2.5.364','# Commander Creation — shipped in 2.5.365').replace('BUILD-2.5.364.txt','BUILD-2.5.365.txt')
doc+='\n## Final exact-release evidence\n\n'+section+'\n'
(root/'docs/COMMANDER-CREATION-365.md').write_text(doc,encoding='utf-8')
for p in (release/'README.txt',root/'dist/ELITE-NEXT-PSP/README.txt'):
    t=read(p)
    if 'Exhaustive portrait audit:' not in t:
        p.write_text(t+'\nExhaustive portrait audit: 345,184 selectable combinations at three sizes.\nCorrected neck/collar masks and expanded palettes/features included.\nNew v3 portraits require a compatible EBOOT; keep save backups before downgrading.\nFull-game checks remain failing/incomplete; see BUILD-2.5.365.txt.\n',encoding='utf-8')
paragraph='\nPortrait audit included in this exact release: all 345,184 selectable v2/v3 combinations rendered at 96/32/20px; 12,162 foreground/material checks pass. Corrected neck/collar and feature masks across all ten species; expanded palettes and supported facial styles. All seven relevant headers match the frozen 365 snapshot; 94 native portrait crops match the tested C renderer. Details and broader test limitations: docs/COMMANDER-CREATION-365.md and BUILD-2.5.365.txt.\n\n'
for n in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/n;t=read(p)
    if 'Portrait audit included in this exact release:' not in t:
        pos=t.index('\n')+1;p.write_text(t[:pos]+paragraph+t[pos:],encoding='utf-8')
for n in ('DESIGN-BIBLE.md','DESIGN-BIBLE-2.0.md'):
    p=root/'docs'/n;t=read(p)
    if '<!-- portrait-audit-365 -->' not in t:
        p.write_text('<!-- portrait-audit-365 -->\nCommander portraits require authored per-pixel material regions, neck/collar separation, aligned anatomy, useful foreground changes at 20/32/96px, real save/input checks and exhaustive combination rendering. Distinguish automated coverage from manual visual review. See COMMANDER-CREATION-365.md.\n\n'+t,encoding='utf-8')
archive=release.parent/'ELITE-NEXT-PSP-v2.5.365.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():
            assert p.suffix.lower() in ('.txt','.pbp');z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper()==sha
for p in (root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP/EBOOT.PBP',artifact):assert digest(p)==sha
print(json.dumps(dict(version='2.5.365',sha256=sha,focused_checks=sum(checks.values()),native_crops=crop_checks,eboot=str(artifact),zip=str(archive)),indent=2))
