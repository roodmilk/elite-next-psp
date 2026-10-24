"""Package the exact reviewed 2.5.361 bytes; preserve mixed shared work."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile

root = Path(__file__).resolve().parents[1]
artifact = root / 'outputs/menu361-candidate/EBOOT.PBP'
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
read = lambda p: p.read_text(encoding='utf-8-sig')
sha = digest(artifact)
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.361' in data[offsets[0]:offsets[1]]
counts, runs = {}, {}
for name in ('deck', 'boot', 'regions', 'eva', 'opening'):
    log = read(root / f'outputs/menu361-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M), name
    match = re.search(r'^Review:\s*(.+?)(?=\n\S|\Z)', log, re.M | re.S)
    run = Path(re.sub(r'\r?\n\s*', '', match[1]).strip())
    assert digest(run / 'EBOOT.PBP') == sha, f'{name}: stale artifact'
    counts[name] = len(re.findall(r'^PASS ', log, re.M))
    runs[name] = str(run.relative_to(root))
assert counts == dict(deck=36, boot=2, regions=31, eva=12, opening=343), counts

core = root / '../../work/core-20261010-120020-538'
assert digest(core / 'EBOOT.PBP') == sha
failures = lambda s: re.findall(r'^FAIL .+$', s, re.M)
assert failures(read(core / 'game-check.txt')) == failures(read(root / 'outputs/planets359-core.log'))
for name in ('radio', 'steering'):
    assert 'RESULT 0 failures' in read(core / f'{name}-check.txt')
smoke = root / '../../work/smoke-20261010-120153-086'
assert digest(smoke / 'EBOOT.PBP') == sha
assert failures(read(smoke / 'game-check.txt')) == failures(read(core / 'game-check.txt'))
smoke_input = read(smoke / 'input-check.txt')
baseline_input = read(root / '../../work/smoke-20261010-111439-530/input-check.txt')
assert set(failures(smoke_input)) <= set(failures(baseline_input)), 'New broad input failures require investigation'

details = '''# Menu openings — 2.5.361

Selecting a Command Deck service pulses its button for 70 ms, then expands the live destination from that button to the full native 480×272 screen over 200 ms. Cubic easing and the current ship paint colours keep the transition consistent with the console. Launch skips this overlay so flight remains visible immediately. Nested shop and console switches retain immediate openings.

One clean off-screen Command Deck snapshot uses 557,056 static bytes. No frame allocations or live/front-buffer reads occur during activation. In-place destination compression uses one scratch row and outward traversal from the ceiling fixed point; the sampler matches an immutable reference across four overlap cases, including near-full-screen dimensions. Unused framebuffer gutters remain untouched. No feedback replay, scan bands or full-screen white flash is used.

Circle cancels the transition and returns immediately. Directional input cancels it and moves selection immediately. A second rapid X reveals the destination without buying an item that has not yet been shown; subsequent X works normally. Chart avoids a second dissolve. Changing page cancels stale animation state.

The exact release passes 36 Command Deck checks and two normal boot checks. The 17 sampled opening frames average 6.805 ms, worst 9.100 ms, in PPSSPP; clean activation costs 8.556 ms. These are emulator CPU timings, not a physical PSP frame-rate guarantee. Native captures include pulse, expansion, final page and 42 paint-theme views. Physical PSP startup, input feel and frame pacing remain a player test.
'''
(root / 'docs/MENU-TRANSITIONS-361.md').write_text(details, encoding='utf-8')
planet = root / 'docs/PLANET-REGIONS-V1.md'
s = read(planet)
start = s.index('## Resumed surface pass')
end = s.index('## Continuation', start)
s = s[:start] + '''## Resumed surface pass — 2.5.361

The player explicitly requested continuation after the menu checkpoint. Signed lattice coordinates were added to unsigned hash jitter, wrapping negative prop positions to roughly 4.29 billion metres. Converting the lattice term to float before addition restores trees and rocks in those quadrants. Drawing and obstacle collision share the corrected positions; the matching cosmetic grass expression is fixed too. Positive cells, biome selection, terrain, seeds, generation v1 and V32 saves remain unchanged. Safe-position recovery handles resumed checkpoints that overlap a restored obstacle.

A tagged 16-entry pad-origin cache and per-profile biome ground-colour cache avoid repeated seed trigonometry and constant palette calculations. Exact old arithmetic is verified over all 1,380 destinations, forced cache aliases and region sample positions. An attempted water occupancy optimization failed image-equivalence and was removed before release; it is not in this EBOOT.

31 region checks pass, including 7,203 signed cells across all quadrants. The fern-basin viewpoint now contains 70 visible production props rather than zero, ice canyon four rather than zero, and giant-forest port 50 rather than 17. Ten native region captures and twelve same-position timing scenes are recorded. Earth opening has 343 passing checks; EVA has 12. Normal boot completes with the retained 7 MiB heap.

Lave II surface CPU averages are 41.495/33.058/35.907 ms in the three region views, versus 41.848/32.185/33.488 ms before this repair. Restored props increase some costs; stable 30–60 fps is not yet achieved or certified. Comparable walking means range from 6.864 to 40.327 ms. Preserve this correctness repair and target measured terrain/sky/prop work next. Specialist cave, root, fungal/crystal silhouettes and gas weather remain deferred. Evidence: BUILD-2.5.361.txt.

''' + s[end:]
s = s.replace('Stop after the first EBOOT for player feedback on region scale, travel, appearance and frame rate.', 'The first checkpoint stopped for feedback; the player subsequently authorized resumed planet development and interrupted it for the 2.5.361 menu-transition release. Continue from the repaired regional foundation after delivering that EBOOT.')
planet.write_text(s, encoding='utf-8')

evidence = f'''ELITE: NEXT 2.5.361 — Pulse-and-grow menus; restored planetary props
Built 2026-10-10 on tested local 2.5.360. V32 and generation v1 unchanged.
SHA256: {sha}
EBOOT size: {artifact.stat().st_size} bytes
Build: outputs/menu361-build; log outputs/menu361-build.log
Details: docs/MENU-TRANSITIONS-361.md; docs/PLANET-REGIONS-V1.md

Menu buttons pulse 70 ms and grow into their live page over 200 ms.
Back/directions interrupt immediately; rapid second X reveals without purchase.
Launch skips; nested menus remain immediate; current ship paint theme retained.
Native sampling, framebuffer gutters and overlapping in-place copies verified.
Opening render: mean 6.805 ms, worst 9.100 ms; activation 8.556 ms in PPSSPP.
One 557056-byte clean source snapshot; no per-frame allocations.
Normal startup passes with the retained 7 MiB heap; hardware testing pending.

Planet repair: negative signed cell arithmetic no longer wraps to unsigned.
Fern basin 0->70 props; ice canyon 0->4; giant forest port 17->50.
7203 signed cells checked; tagged pad and biome-colour caches match old math.
Experimental water cache was removed after failing pixel equivalence.
Existing 1380 destinations, 3898 regions, no new assets or galaxy expansion.
Surface FPS still needs work: Lave II region means 41.495/33.058/35.907 ms.
These are emulator CPU measurements, not certified hardware frame rates.

Exact-artifact checks:
''' + ''.join(f'- {k}: {v} PASS; {runs[k]}\n' for k,v in counts.items()) + f'''
Total: {sum(counts.values())} focused checks, zero focused failures.
Core game: same 15 inherited failures as planets359-core.log; no additional
core failures. Radio/steering pass. Broad smoke input reports 61 failures,
all present in the baseline's 63 reported failures, and remains incomplete
in its 120-second window. Whole-game certification,
hardware suspend/audio and two-device Wi-Fi remain open.
Core: {core.resolve()}
Smoke: {smoke.resolve()}

Local only. Git pull blocked by pre-existing untracked music placeholders;
autostash restored. Mixed shared changes preserved; no commit/tag/push claimed.
Root, dist, versioned and ZIP EBOOT bytes match. Player music, saves, settings,
online.cfg and credentials excluded. Replace only EBOOT.PBP.
'''
release = root / 'outputs/releases/2.5.361/ELITE-NEXT-PSP'
previous = 'D3504C35AF1CE7FBF473D2D0BFFDC640D91A1ADC4FCF587528BC06A0F2A37555'
for target in (root, root / 'dist/ELITE-NEXT-PSP', release):
    target.mkdir(parents=True, exist_ok=True)
    dest = target / 'EBOOT.PBP'
    if dest.exists():
        assert digest(dest) in (previous, sha), 'Shared release changed'
    shutil.copy2(artifact, dest)
    (target / 'BUILD-2.5.361.txt').write_text(evidence, encoding='utf-8')
    if target != root:
        (target / 'README.txt').write_text('ELITE: NEXT 2.5.361\nReplace ONLY EBOOT.PBP; keep music, saves and settings.\nPulse-and-grow menu openings and restored planetary trees/rocks.\n424 focused checks pass; physical PSP playtest pending.\n15 inherited broad failures remain; see BUILD-2.5.361.txt.\nLocal release; GitHub not updated.\n', encoding='utf-8')
        (target / 'SHA256.txt').write_text(f'{sha}  EBOOT.PBP\n', encoding='ascii')
        shutil.copy2(root / 'BUILD-2.5.360.txt', target / 'BUILD-2.5.360.txt')
for folder in (root / 'music').iterdir():
    if folder.is_dir():
        dest = release / 'music' / folder.name
        dest.mkdir(parents=True, exist_ok=True)
        (dest / 'README.txt').write_text('Add your tracks here, then restart the game.\n', encoding='utf-8')
archive = release.parent / 'ELITE-NEXT-PSP-v2.5.361.zip'
with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():
            assert p.suffix.lower() in ('.txt', '.pbp'), p
            z.write(p, p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.361 — Pulse-and-grow menu openings; restored planet props

Command Deck choices pulse briefly and expand into the live destination over 270 ms, using the ship paint theme. No stale framebuffer replay or flash; Back and directions interrupt immediately. Launch stays immediate. Opening drawing averages 6.805 ms (worst 9.100 ms) in PPSSPP; physical PSP frame pacing remains untested.

Repairs unsigned wrapping of negative planetary prop positions, restoring trees/rocks in fern basins and ice canyons. Adds exact tagged pad-origin and biome-colour caches; retains geography v1, V32 saves, the full 2.5.360 UI/theme work, Multiplayer and existing regional generation. 424 focused checks pass on this exact EBOOT, including normal boot, native menu captures, region/collision/save tests and the complete authored opening. Same 15 inherited core failures remain; radio/steering pass and broad input smoke is incomplete. Surface FPS still needs tuning. See BUILD-2.5.361.txt and docs/MENU-TRANSITIONS-361.md.

Next: deliver this menu EBOOT, then resume measured regional surface optimization and visual progress using the asset coverage in docs/PLANET-REGIONS-V1.md. Do not claim specialist planetary art or hardware certification complete. Local release only; blocked Git pull and mixed shared changes prevent safe publication. SHA256: {sha}.

'''
for name in ('CHANGELOG.md', 'README.md', 'CLAUDE-HANDOFF.md'):
    p = root / name
    s = read(p)
    if not s.startswith('## 2.5.361'):
        p.write_text(entry + s, encoding='utf-8')
for name in ('DESIGN-BIBLE.md', 'DESIGN-BIBLE-2.0.md'):
    p = root / 'docs' / name
    s = read(p)
    if not s.startswith('<!-- 2.5.361 -->'):
        p.write_text('<!-- 2.5.361 -->\nImplemented checkpoint: paint-themed 270 ms pulse/grow Command Deck openings, interruptible input, and corrected negative-coordinate planetary prop placement. V32/generation v1 retained. Details and remaining regional art/performance gaps: PLANET-REGIONS-V1.md and MENU-TRANSITIONS-361.md.\n\n' + s, encoding='utf-8')
print(json.dumps(dict(version='2.5.361', sha256=sha, checks=counts, total=sum(counts.values()), eboot=str(release/'EBOOT.PBP'), zip=str(archive)), indent=2))
