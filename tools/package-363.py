"""Package the reviewed raster hair-material correction, never an old candidate."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile
from PIL import Image, ImageChops

root = Path(__file__).resolve().parents[1]
artifact = root / 'outputs/portrait363-candidate/EBOOT.PBP'
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
sha = digest(artifact)
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.363' in data[offsets[0]:offsets[1]]
counts, runs = {}, {}
for name in ('parts', 'opening', 'boot', 'deck'):
    log = read(root / f'outputs/portrait363-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M), name
    match = re.search(r'^Review:\s*(.+?)(?=\n\S|\Z)', log, re.M | re.S)
    run = Path(re.sub(r'\r?\n\s*', '', match[1]).strip())
    assert digest(run / 'EBOOT.PBP') == sha, f'{name}: stale artifact'
    counts[name] = len(re.findall(r'^PASS ', log, re.M))
    runs[name] = run
assert counts == dict(parts=37, opening=343, boot=2, deck=36), counts
crop = (24,82,120,178)
old = Image.open(root / 'outputs/opening-check-20261010-115945-032/portrait-creator-277.bmp').convert('RGB').crop(crop)
new = Image.open(runs['parts'] / 'legacy-preserved.bmp').convert('RGB').crop(crop)
assert ImageChops.difference(old,new).getbbox() is None

error = root / 'outputs/portrait363-smoke-error.log'
assert error.exists() and 'Failed game checks' in read(error), 'Smoke must finish before packaging'
smoke = next(p.resolve() for p in sorted((root / '../../work').glob('smoke-*'),reverse=True)[:4]
             if (p/'EBOOT.PBP').exists() and digest(p/'EBOOT.PBP') == sha)
failures = lambda s: re.findall(r'^FAIL .+$',s,re.M)
baseline = failures(read(root / 'outputs/planets359-core.log'))
assert len(baseline) == 15 and failures(read(smoke/'game-check.txt')) == baseline
for kind in ('radio','steering'):
    assert 'RESULT 0 failures' in read(smoke/(kind+'-check.txt'))
input_log = read(smoke/'input-check.txt')
old_input = read(root / '../../work/smoke-20261010-111439-530/input-check.txt')
assert set(failures(input_log)) <= set(failures(old_input)), 'New input failures'
cases = re.search(r'INFO (\d+) real foreground render cases',read(root/'outputs/portrait363-parts.log'))[1]

release = root / 'outputs/releases/2.5.363/ELITE-NEXT-PSP'
previous = 'B59CE0843254B98466F234B50655AC727F07CED22717A7E8725223C02673AAD0'
for target in (root,root/'dist/ELITE-NEXT-PSP',release):
    if (target/'EBOOT.PBP').exists():
        assert digest(target/'EBOOT.PBP') in (previous,sha), 'Shared release changed'
evidence = f'''ELITE: NEXT 2.5.363 — Pixel-traced hair material
SHA256: {sha}
Build: outputs/portrait363-build; log outputs/portrait363-build.log
Details: docs/PORTRAIT-HAIR-MASKS-363.md

Replaces the rejected Human/Android hair polygon with native 64px traced
scalp contours, including curls, temples, sideburns and ear cutouts.
Blonde and blue colour the hair; the forehead and ears stay unchanged.
Original strand shading and dark outlines retained. No save migration.
Legacy crop remains pixel-identical to the shipped 2.5.361 reference.
Native comparisons inspected and shown before packaging.

''' + ''.join(f'{name}: {counts[name]} PASS; {runs[name]}\n' for name in counts) + f'''
418 focused checks pass. {cases} foreground render cases at 96/32/20px.
Two new independent anatomical landmark/render checks prevent a wrong mask
from passing merely because the renderer and test share the same mask.
Save/load, navigation, colour contrast and background-only rejection pass.
Broad smoke: same 15 inherited game failures; radio and steering pass.
Input smoke is incomplete after 120 seconds; {len(failures(input_log))}
reported failures are all in the previous baseline. Not whole-game certification.
Smoke: {smoke}
Physical PSP testing pending. Root/dist/versioned/ZIP EBOOT bytes match.
Local release only; Git pull still blocked by pre-existing untracked music
placeholders, autostash restored. Mixed work preserved. No GitHub update.
Replace ONLY EBOOT.PBP; keep music, saves and settings.
'''
comparisons = release.parent/'comparisons'
comparisons.mkdir(parents=True,exist_ok=True)
for name in ('hair-brown-blonde','hair-brown-blue','comparison-04','comparison-06','editor-before','editor-after'):
    im = Image.open(runs['parts']/(name+'.bmp'))
    assert im.size == (480,272)
    im.save(comparisons/(name+'.png'))
for target in (root,root/'dist/ELITE-NEXT-PSP',release):
    target.mkdir(parents=True,exist_ok=True)
    shutil.copy2(artifact,target/'EBOOT.PBP')
    (target/'BUILD-2.5.363.txt').write_text(evidence,encoding='utf-8')
    if target != root:
        (target/'README.txt').write_text('ELITE: NEXT 2.5.363\nPixel-traced hair masks; saved selections retained.\n418 focused checks pass; 15 inherited broad failures remain.\nPhysical PSP testing pending; local release only.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n',encoding='utf-8')
        (target/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n',encoding='ascii')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.363.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():
            assert p.suffix.lower() in ('.txt','.pbp')
            z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.363 — Correct hair coverage, not a forehead overlay

Replaces the coarse Human/Android hair polygon from 2.5.362 with pixel-traced scalp masks at the authored 64px resolution. Blonde and blue now follow the full curls, temples and sideburns; forehead and ear pixels remain unchanged. Ear-part sampling uses the same corrected material map. Retains strand shading, outlines, selected DNA, legacy appearances and unrelated work.

418 focused checks pass, including independent source-art hair/forehead landmark checks, all-pairs colour contrast and 20/32/96px portrait comparisons, save/load, opening, boot and Command Deck checks. Comparisons inspected and shown before packaging. Broad smoke retains the same 15 inherited game failures; radio/steering pass, input smoke remains incomplete and physical PSP testing pending. Details: docs/PORTRAIT-HAIR-MASKS-363.md and BUILD-2.5.363.txt.

Local release only; blocked Git pull restored its autostash, shared mixed work preserved. GitHub not updated. SHA256: {sha}. Resume the existing planetary work after this correction; full game certification remains open.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p=root/name; s=read(p)
    if not s.startswith('## 2.5.363'):
        p.write_text(entry+s,encoding='utf-8')
for name in ('DESIGN-BIBLE.md','DESIGN-BIBLE-2.0.md'):
    p=root/'docs'/name; s=read(p)
    if not s.startswith('<!-- 2.5.363 -->'):
        p.write_text('<!-- 2.5.363 -->\nHair palette corrections must follow reviewed pixel anatomy, not just self-consistent mask bounds. Human/Android scalp contours and independent landmarks replace the rejected 2.5.362 polygon. Details: PORTRAIT-HAIR-MASKS-363.md.\n\n'+s,encoding='utf-8')
doc=root/'docs/PORTRAIT-HAIR-MASKS-363.md'; s=read(doc)
start=s.find('Verification and exact artifact evidence')
if start<0: start=s.index('Verified on the packaged executable:')
s=s[:start] + f'Verified on the packaged executable: 37 portrait checks, 343 opening checks, two boot checks and 36 Command Deck checks. {cases} foreground render cases at 20/32/96px. Native captures were inspected and shown before packaging; copies are in outputs/releases/2.5.363/comparisons. The legacy portrait crop matches 2.5.361 pixel for pixel. Broad smoke retains the same 15 inherited game failures; radio/steering pass. Input smoke remains incomplete, with only baseline failures observed. Physical PSP testing remains pending. Exact hash and runs: BUILD-2.5.363.txt.\n'
doc.write_text(s,encoding='utf-8')
print(json.dumps(dict(version='2.5.363',sha256=sha,checks=counts,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
