"""Package only the exact Commander Creation executable reviewed in PPSSPP."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile
from PIL import Image, ImageChops

root = Path(__file__).resolve().parents[1]
artifact = root / 'outputs/portrait362f-candidate/EBOOT.PBP'
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
sha = digest(artifact)
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.362' in data[offsets[0]:offsets[1]]
counts, runs = {}, {}
for name in ('parts', 'opening', 'boot', 'deck'):
    log = read(root / f'outputs/portrait362f-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M), name
    match = re.search(r'^Review:\s*(.+?)(?=\n\S|\Z)', log, re.M | re.S)
    run = Path(re.sub(r'\r?\n\s*', '', match[1]).strip())
    assert digest(run / 'EBOOT.PBP') == sha, f'{name}: stale artifact'
    counts[name] = len(re.findall(r'^PASS ', log, re.M))
    runs[name] = run
assert counts == dict(parts=35, opening=343, boot=2, deck=36), counts

# Compare the old face against pixels captured from the shipped 2.5.361 EBOOT.
crop = (24, 82, 120, 178)
old = Image.open(root / 'outputs/opening-check-20261010-115945-032/portrait-creator-277.bmp').convert('RGB').crop(crop)
new = Image.open(runs['parts'] / 'legacy-preserved.bmp').convert('RGB').crop(crop)
assert ImageChops.difference(old, new).getbbox() is None, 'Legacy appearance changed'

def matching_run(kind):
    for p in sorted((root / '../../work').glob(kind + '-*'), reverse=True)[:4]:
        if (p / 'EBOOT.PBP').exists() and digest(p / 'EBOOT.PBP') == sha:
            return p.resolve()
    raise AssertionError('Missing exact-artifact ' + kind)

failures = lambda s: re.findall(r'^FAIL .+$', s, re.M)
core, smoke = matching_run('core'), matching_run('smoke')
baseline = failures(read(root / 'outputs/planets359-core.log'))
assert len(baseline) == 15
assert failures(read(core / 'game-check.txt')) == baseline
assert failures(read(smoke / 'game-check.txt')) == baseline
for kind in ('radio', 'steering'):
    assert 'RESULT 0 failures' in read(core / (kind + '-check.txt'))
input_log = read(smoke / 'input-check.txt')
baseline_input = read(root / '../../work/smoke-20261010-111439-530/input-check.txt')
assert set(failures(input_log)) <= set(failures(baseline_input)), 'New broad input failures'
input_count = len(failures(input_log))

release = root / 'outputs/releases/2.5.362/ELITE-NEXT-PSP'
previous = '13DFB7F1F38E70BBF23B31974C236FADD917E03870EC0D5CB55E950649169C8F'
for target in (root, root / 'dist/ELITE-NEXT-PSP', release):
    dest = target / 'EBOOT.PBP'
    if dest.exists():
        assert digest(dest) in (previous, sha), 'Shared release changed'

evidence = f'''ELITE: NEXT 2.5.362 — Isolated Commander Creation controls
Built 2026-10-10 on the local 2.5.361 work, retaining menus and planets.
SHA256: {sha}
EBOOT size: {artifact.stat().st_size} bytes
Build: outputs/portrait362f-build; log outputs/portrait362f-build.log
Details: docs/COMMANDER-CREATION-362.md

Ten authored 64px face rigs; explicitly aligned anatomy/material masks.
Skin/hair palettes preserve source shading; eyes/mouth/ears edit local parts.
Unsupported controls hidden, stray atlas strips and generic overlays removed.
A shaded cheek scar excludes hair/eyes/lips/ears. Background changes never count.
Blonde, brown and blue hair use readable shaded palettes with original outlines.
New looks use DNA v2 in the existing V32 four-byte field. Legacy rendering
retained; existing looks require explicit New Look before feature editing.
Real save/load tested on numeric IDs, v1 DNA and v2 DNA across all species.
Legacy portrait crop is pixel-identical to the shipped 2.5.361 capture.
Keep save backups before downgrading: older builds do not read v2 portraits.

Exact-artifact focused checks:
''' + ''.join(f'- {name}: {counts[name]} PASS; {runs[name]}\n' for name in counts) + f'''
Total: {sum(counts.values())} focused checks, zero focused failures.
Parts: 1578 foreground render cases at 96/32/20px, all-pairs choices,
material isolation, unchanged selected fields, background-only negative
control, actual D-pad navigation, 30 save/load cases and 1000 random looks.
Native comparisons inspected and shown to the player before packaging.

Broader checks: same 15 inherited game failures; radio/steering pass.
Broad input smoke reports {input_count} failures already in the baseline;
its 120-second run remains incomplete. Whole-game certification is not claimed.
Core: {core}
Smoke: {smoke}
Physical PSP testing remains pending.

Local release only. Git pull blocked by existing untracked music placeholders;
autostash restored. Shared mixed changes preserved; no commit/tag/push claimed.
Root, dist, versioned and ZIP EBOOT bytes match. Music, saves, settings and
credentials excluded. Replace ONLY EBOOT.PBP and retain your data.
'''
parts_log = read(root / 'outputs/portrait362f-parts.log')
cases = re.search(r'INFO (\d+) real foreground render cases', parts_log)[1]
evidence = evidence.replace('1578 foreground', cases + ' foreground')

comparison = release.parent / 'comparisons'
comparison.mkdir(parents=True, exist_ok=True)
for name in ('comparison-02', 'comparison-03', 'comparison-04', 'comparison-06', 'comparison-09', 'hair-brown-blonde', 'hair-brown-blue', 'species-parts', 'editor-before', 'editor-after'):
    im = Image.open(runs['parts'] / (name + '.bmp'))
    assert im.size == (480, 272)
    im.save(comparison / (name + '.png'))
for target in (root, root / 'dist/ELITE-NEXT-PSP', release):
    target.mkdir(parents=True, exist_ok=True)
    shutil.copy2(artifact, target / 'EBOOT.PBP')
    (target / 'BUILD-2.5.362.txt').write_text(evidence, encoding='utf-8')
    if target != root:
        (target / 'README.txt').write_text('ELITE: NEXT 2.5.362\nReplace ONLY EBOOT.PBP; retain music, saves and settings.\nCommander feature controls repaired; old saved appearances preserved.\n416 focused checks pass; PSP hardware playtest pending.\nSame 15 inherited broad game failures remain. See BUILD-2.5.362.txt.\nLocal release; GitHub not updated.\n', encoding='utf-8')
        (target / 'SHA256.txt').write_text(sha + '  EBOOT.PBP\n', encoding='ascii')
for folder in (root / 'music').iterdir():
    if folder.is_dir():
        dest = release / 'music' / folder.name
        dest.mkdir(parents=True, exist_ok=True)
        (dest / 'README.txt').write_text('Add your tracks here, then restart the game.\n', encoding='utf-8')
archive = release.parent / 'ELITE-NEXT-PSP-v2.5.362.zip'
with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():
            assert p.suffix.lower() in ('.txt', '.pbp'), p
            z.write(p, p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha

entry = f'''## 2.5.362 — Commander Creation feature isolation

Commander Creation now uses ten authored portrait rigs with explicit skin/hair material masks and local eye, mouth and ear parts. Each offered control changes its named foreground feature while retaining other selected fields. Removes unsupported face/head/background roulette, tiny unreadable choices and stray atlas overlays. Species-specific controls skip absent anatomy. New looks use DNA v2; legacy saved appearances retain the original renderer and require explicit New Look to replace them. V32 field layout unchanged; back up saves before downgrading to an older EBOOT.

416 focused checks pass on the exact executable: 35 feature/save/navigation checks, 343 opening checks, two boot checks and 36 Command Deck checks. All-pairs foreground comparisons at 96/32/20px reject background-only changes. Hair palettes also require a meaningful colour contrast. The blue stripe is replaced with a shaded cheek scar on supported rigs. Native comparison captures were inspected and shown before packaging. The legacy crop matches the shipped 2.5.361 pixels. Broader game checks retain the same 15 inherited failures; radio/steering pass, broad input smoke remains incomplete, physical PSP testing pending. Retains 2.5.361 pulse/grow menus and restored regional props. See docs/COMMANDER-CREATION-362.md and BUILD-2.5.362.txt.

Local release only; Git pull remains blocked by untracked music placeholders and mixed shared work is preserved. SHA256: {sha}. Resume measured planetary optimization after this portrait repair; do not claim whole-game certification.

'''
for name in ('CHANGELOG.md', 'README.md', 'CLAUDE-HANDOFF.md'):
    p = root / name
    s = read(p)
    if not s.startswith('## 2.5.362'):
        p.write_text(entry + s, encoding='utf-8')
for name in ('DESIGN-BIBLE.md', 'DESIGN-BIBLE-2.0.md'):
    p = root / 'docs' / name
    s = read(p)
    if not s.startswith('<!-- 2.5.362 -->'):
        p.write_text('<!-- 2.5.362 -->\nCommander Creation: aligned authored feature parts, explicit palette masks, supported controls only; unchanged saved legacy appearances, v2 new looks in the V32 field. Verification and native comparisons: COMMANDER-CREATION-362.md.\n\n' + s, encoding='utf-8')
doc = root / 'docs/COMMANDER-CREATION-362.md'
s = read(doc).replace(' — 2.5.362 candidate', ' — 2.5.362')
start = s.find('Verification pending:')
if start < 0:
    start = s.index('Verification complete on the exact packaged artifact:')
s = s[:start] + f'''Verification complete on the exact packaged artifact: 35 portrait checks ({cases} foreground render cases), 343 opening checks, two normal-boot checks and 36 Command Deck checks. Every pair of offered values is compared at 96px, 32px and 20px; changed background pixels cannot satisfy the minimum two foreground pixels. Hair colours additionally require at least three changed pixels and a mean RGB-channel change of 18 or more. Unrelated materials/parts must remain unchanged. Actual editor navigation, inactive species selection suppression, explicit legacy opt-in, 30 real V32 save/load round trips, 1000 random looks and framebuffer guards pass. The old portrait crop is pixel-identical to the shipped 2.5.361 capture. Native comparison images were inspected and shown before packaging; copies are in outputs/releases/2.5.362/comparisons.

The same 15 inherited broad game-check failures remain; radio/steering pass. Broad input smoke remains incomplete at its existing 120-second window, with only baseline failures observed. Physical PSP testing is pending. Full evidence and exact artifact hash: BUILD-2.5.362.txt.\n'''
doc.write_text(s, encoding='utf-8')
print(json.dumps(dict(version='2.5.362', sha256=sha, checks=counts, total=sum(counts.values()), cases=int(cases), eboot=str(release / 'EBOOT.PBP'), zip=str(archive)), indent=2))
