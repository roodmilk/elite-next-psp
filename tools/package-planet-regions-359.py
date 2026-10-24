"""Package the exact tested planetary/boot checkpoint; never include player data."""
from pathlib import Path
import hashlib
import json
import shutil
import struct
import zipfile

root = Path(__file__).resolve().parents[1]
artifact = root / 'outputs/bootfix359/EBOOT.PBP'
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
sha = digest(artifact)
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.359' in data[offsets[0]:offsets[1]]
reports = {
    'planet regions': 'outputs/planets359-regions-final.log',
    'Earth opening': 'outputs/planets359-earth-final.log',
    'Command Deck': 'outputs/deck-review-20261010-111215-599/command-deck-review.txt',
}
counts = {}
for name, path in reports.items():
    log = (root/path).read_text(encoding='utf-8-sig')
    assert 'RESULT 0 failures' in log and not any(s.startswith('FAIL ') for s in log.splitlines()), name
    counts[name] = sum(s.startswith('PASS ') for s in log.splitlines())
def failures(path):
    return [s for s in (root/path).read_text(encoding='utf-8-sig').splitlines() if s.startswith('FAIL ')]
core_path = 'outputs/planets359-core.log'
assert failures(core_path) == failures('outputs/deck358-release-core.log')
assert len(failures(core_path)) == 15
assert (root/core_path).read_text(encoding='utf-8-sig').count('RESULT 0 failures') == 2
boot = root/'outputs/normal-boot359-20261010-111241/native-480x272.bmp'
assert boot.stat().st_size == 54+480*272*3
assert struct.unpack_from('<ii', boot.read_bytes(), 18) == (480,272)
assert len(set(boot.read_bytes()[54:])) > 32
assert 'MEMBLOCK_ALLOC_FAILED' not in boot.with_name('emulator.log').read_text(errors='replace')

evidence = f'''ELITE: NEXT 2.5.359 - Planet regions v1 and PSP boot fix
Built 2026-10-10; includes 2.5.358 Observation Lounge and 2.5.357 Multiplayer.
Save format V32 unchanged. Generation version 1, existing body IDs unchanged.
SHA256: {sha}
EBOOT size: {artifact.stat().st_size} bytes

BOOT FIX
Combined native assets/caches and the fixed 8 MiB heap exceeded the available
PSP-1000 user memory allocation at startup. Reduce the fixed heap to 7 MiB.
The previous candidate logged MEMBLOCK_ALLOC_FAILED before any frame/report.
Normal boot now reaches the title through audio/network setup, with a complete
native 480x272 capture and no allocation failure in the emulator log.
Evidence: outputs/normal-boot359-20261010-111241.
Region capture heap: 559736 bytes used; 2952 free committed; 6777344 uncommitted.
This is emulator evidence; actual PSP boot/suspend/audio testing remains needed.

TESTED ARTIFACT
outputs/bootfix359/EBOOT.PBP (immutable download checkpoint)
Build log: outputs/planets359-build.log
'''+''.join(f'{k}: {v} PASS, zero failures. {reports[k]}\n' for k,v in counts.items())+'''
Planet region coverage: 1038 ground worlds +342 gas destinations, 3898 selected
regions, all 18 ground types and 4 gas families represented. Deterministic
cache eviction/revisit, dry flat pads, region continuity, Lave POI exits and
observatory trail, all four landing/disembark/board/takeoff flows, safe/unsafe
checkpoint recovery, V32 discovery/funds/identity persistence and framebuffer
guards pass. Ten actual production surface captures inspected at 480x272.
Some fern/ice viewpoints remain sparse; art is reused/adapted, not a finished
bespoke environment library. See docs/PLANET-REGIONS-V1.md for actual coverage.

Earth opening: 343 checks pass. Of 95 native opening-suite captures, 82 remain
byte-identical; changed captures include generated planet/ring views and menu
states. The authored meadow captures remain identical. Command Deck: 27 checks
pass, including animations, controls, availability, pause and retained Multiplayer.
No claim that all former 178 focused UI checks were rerun for this artifact.

Core failure lines exactly match 2.5.358 (15). Steering and radio: zero failures.
Core evidence: outputs/planets359-core.log.
Required broad smoke was attempted: outputs/planets359-smoke.log. Its game
group has the same 15 failures; the input group does not complete within the
120-second limit. Full smoke is not certified green. Three inherited local
planetary Wi-Fi issues were not rerun here; physical two-device testing is pending.

PERFORMANCE
Same twelve Lave scene positions and ten frames per scene as the compiled
pre-change baseline; emulator CPU render timings, not actual PSP frame rates.
Ground scene averages are 31.644-44.698ms versus 28.306-41.732ms baseline.
Lave II averages increase about 8-18%; its open EVA case needs further tuning.
Gas averages are 35.290/17.992/6.880ms versus 43.661/17.748/6.864ms.
No stable 60fps or universal performance-improvement claim is made.
Baseline: outputs/planets358-baseline-performance.log.
Final: outputs/planets359-regions-final.log.

RELEASE / CONTINUATION
Root, dist, versioned folder and ZIP EBOOT copies are hash-identical.
ZIP includes only EBOOT, readme, evidence/hash and empty station folders with
placeholders; no music tracks, saves, preferences, credentials or online.cfg.
Local release only: pull was blocked by two existing untracked music
placeholders; autostash was restored. Mixed checkout has unrelated work.
No commit, tag, push or GitHub publication is claimed. Preserve player music.
Next: physical PSP boot test; Lave region/travel/FPS feedback; inherited core
failures; later sparse-region vegetation/performance tuning and specialist art.
Do not broaden art/geometry until feedback on this first playable checkpoint.

Inherited core failures:
'''+ '\n'.join(failures(core_path))+'\n'
readme = '''ELITE: NEXT 2.5.359 - Planet regions and boot fix

Replace ONLY EBOOT.PBP in your existing PSP/GAME game folder.
Keep saves, settings, online.cfg and music. Save format remains V32.
Includes the animated 2.5.358 Command Deck and 2.5.357 Multiplayer screen.
Fixes the startup heap allocation that could leave a black screen.

Existing worlds now use seeded regional terrain, palettes and reused native
props. Lave I: temperate meadow/forest/valley/coast; II: overgrown;
III: cloud platforms; IV: frozen. No new worlds or bespoke art in this pass.
Some regions remain sparse; caves and signature environments are unfinished.

Normal emulator boot and 397 focused checks pass. Same 15 inherited broad
game checks still fail; full smoke and real PSP testing are not certified.
Please test startup on your PSP, then Lave surface travel/appearance/FPS.
Local release: GitHub has not been updated. Details: BUILD-2.5.359.txt.
Add tracks to music/station folders and restart. No player tracks included.
'''
release = root/'outputs/releases/2.5.359/ELITE-NEXT-PSP'
for target in [root, root/'dist/ELITE-NEXT-PSP', release]:
    target.mkdir(parents=True, exist_ok=True)
    (target/'BUILD-2.5.359.txt').write_text(evidence, encoding='utf-8')
    dest = target/'EBOOT.PBP'
    if dest.exists():
        assert digest(dest) in (sha, '4A29E776973B23F0A91224CC6FFFEA9C53DCEEA422CB72342FFC1F911AD56495'), 'Shared build changed; do not overwrite.'
    shutil.copy2(artifact, dest)
    assert digest(dest) == sha
    if target != root:
        (target/'README.txt').write_text(readme, encoding='utf-8')
        (target/'SHA256.txt').write_text(f'{sha}  EBOOT.PBP\n', encoding='ascii')
for folder in (root/'music').iterdir():
    if folder.is_dir():
        d = release/'music'/folder.name
        d.mkdir(parents=True, exist_ok=True)
        (d/'README.txt').write_text('Add your music here, then restart the game.\n', encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.359.zip'
with zipfile.ZipFile(archive, 'w', zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.rglob('*')):
        if p.is_file():
            assert p.suffix.lower() in ('.pbp','.txt'), p
            z.write(p, p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
print(json.dumps({'version':'2.5.359','sha256':sha,'focused_checks':counts,'total':sum(counts.values()),'eboot':str(release/'EBOOT.PBP'),'zip':str(archive)}, indent=2))
