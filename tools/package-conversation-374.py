"""Package the checked conversation ownership fix and baseline core comparison."""
from pathlib import Path
import hashlib, json, re, shutil, struct, zipfile

root = Path(__file__).resolve().parents[1]
read = lambda p: p.read_text(encoding='utf-8-sig')
digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
baseline = json.loads(read(root/'outputs/releases/2.5.373/test-evidence.json'))
artifact = root/'outputs/conversation374-candidate/EBOOT.PBP'
sha = digest(artifact)
assert read(root/'VERSION').strip() == '2.5.374'
data = artifact.read_bytes()
offsets = struct.unpack_from('<8I', data, 8)
assert b'ELITE: NEXT 2.5.374' in data[offsets[0]:offsets[1]]
film_sha = hashlib.sha256(data[offsets[7]:]).hexdigest().upper()
assert film_sha == baseline['film_sha256']
sources = {}
allowed={'game.c','game.h','comms-panel.h','comms-console.h','main.c','quiet-hails.h','comms-console-tests.h','comms-input-tests.h','quiet-hails-tests.h','comms-panel-review.h','conversation-routing-review.h'}
for name in sorted(set(baseline['source_sha256'])|allowed):
    previous=baseline['source_sha256'].get(name)
    p = root/'src'/name
    assert p.stat().st_mtime <= artifact.stat().st_mtime, name
    sources[name] = digest(p)
    if previous and name not in allowed:
        assert sources[name] == previous, f'Unrelated source changed: {name}'
assert 'No contacts in that band.' not in read(root/'src/flight-extras.h')
checks, runs = {}, {}
for name, expected in (('comms',85),('boot',2),('progress',11),('dossier',5),('quiet',3)):
    log = read(root/f'outputs/conversation374-{name}.log')
    assert 'RESULT 0 failures' in log and not re.search(r'^FAIL ', log, re.M), name
    run = Path(re.search(r'^Review:\s*(.+)$', log, re.M)[1].strip())
    assert digest(run/'EBOOT.PBP') == sha
    checks[name] = len(re.findall(r'^PASS ', log, re.M))
    assert checks[name] == expected, (name, checks[name])
    runs[name] = str(run)
core_before=Path(read(root/'outputs/conversation374-baseline-core-path.txt').strip())
core_after=Path(read(root/'outputs/conversation374-core-path.txt').strip())
assert digest(core_after/'EBOOT.PBP')==sha
before_fail=re.findall(r'^FAIL .+$',read(core_before/'game-check.txt'),re.M)
after_fail=re.findall(r'^FAIL .+$',read(core_after/'game-check.txt'),re.M)
assert before_fail==after_fail and len(after_fail)==15, 'Core failures changed'
assert 'RESULT 15 failures' in read(core_after/'game-check.txt')
for name in ('radio','steering','npc-combat'):
    report=read(core_after/f'{name}-check.txt')
    assert 'RESULT 0 failures' in report and not re.search(r'^FAIL ',report,re.M),name
release = root/'outputs/releases/2.5.374/ELITE-NEXT-PSP'
for p in (root/'EBOOT.PBP', root/'dist/ELITE-NEXT-PSP/EBOOT.PBP', release/'EBOOT.PBP'):
    if p.exists():
        assert digest(p) in (baseline['sha256'], sha), 'Shared release changed'
evidence = f'''ELITE: NEXT 2.5.374 — Correct conversation ownership
EBOOT SHA256: {sha}
Embedded cinematic SHA256: {film_sha} (unchanged)
Build: outputs/conversation374-build.log

Discovery, station and ordinary outgoing pilot channels use their own speaker,
portrait seed and speech, and a close-channel option. Only an explicitly authored
encounter transmission owns encounter replies. Panel actions verify the captured
transmission serial, kind, caller, system and liveness. Changed or lost channels
cannot apply consequences to a different speaker. Quiet Watch checks its source
immediately; world spawning clears stale ownership. No save format change.
Promising convoy assistance no longer grants a false rescue or free 350 units.
A new ambient convoy completion/reward loop is not implemented by this fix.

106 focused emulator checks pass on this exact executable:
'''+''.join(f'{name}: {checks[name]} PASS; {runs[name]}\n' for name in checks)+f'''
The 19 new routing checks cover all 75 catalogue identities against six actionable
encounters and all 32 placed discoveries against all 14 background encounter types,
using actual hail and X-close input, plus stale/dead/replaced channels and legitimate
pirate responses. Native 480x272 sanctuary screenshot inspected.
Core comparison: {core_before} -> {core_after}
The same 15 existing game-core failures remain; radio, steering and NPC combat
checks pass. No additional core failures. Full extended input run not repeated.
Details: docs/CONVERSATION-OWNERSHIP-374.md. Physical PSP testing pending.
Git pull blocked by existing untracked music placeholders; autostash restored.
Mixed shared work preserved. Local only; GitHub not updated.
Replace ONLY EBOOT.PBP; keep saves, settings and music.
Highest-priority remaining work: hardware playtest, inherited reliability,
then planetary work.
'''
release.mkdir(parents=True, exist_ok=True)
for folder in (root, root/'dist/ELITE-NEXT-PSP', release):
    shutil.copy2(artifact, folder/'EBOOT.PBP')
    assert digest(folder/'EBOOT.PBP') == sha
    (folder/'BUILD-2.5.374.txt').write_text(evidence, encoding='utf-8')
    if folder != root:
        (folder/'README.txt').write_text('ELITE: NEXT 2.5.374\nCorrect conversation ownership.\n106 focused emulator checks pass; physical PSP testing pending.\nReplace ONLY EBOOT.PBP; keep saves, settings and music.\n', encoding='utf-8')
        (folder/'SHA256.txt').write_text(sha+'  EBOOT.PBP\n', encoding='ascii')
(release.parent/'test-evidence.json').write_text(json.dumps(dict(version='2.5.374',sha256=sha,film_sha256=film_sha,checks=checks,runs=runs,source_sha256=sources,physical_psp='pending',core_runs=[str(core_before),str(core_after)],inherited_core_failures=after_fail,full_smoke='extended input not repeated'),indent=2),encoding='utf-8')
archive = release.parent/'ELITE-NEXT-PSP-v2.5.374.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for p in sorted(release.iterdir()):
        assert p.suffix.lower() in ('.txt','.pbp')
        z.write(p,p.relative_to(release.parent))
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None
    assert hashlib.sha256(z.read('ELITE-NEXT-PSP/EBOOT.PBP')).hexdigest().upper() == sha
entry = f'''## 2.5.374 — Correct conversation ownership

Fixed discovery contacts inheriting another pilot's encounter replies and consequences. Sanctuary buoys now use Ranger speech and a close-channel option. Contact-only station and pilot hails also retain their own speaker and text. Incoming replies explicitly belong to a caller, encounter and system; the panel snapshots and validates the transmission before acting. Quiet Watch verifies its current speaker. Promising convoy assistance no longer claims a successful rescue or grants an unconditional 350 units; this fix does not add a new ambient convoy completion/reward loop. Mission-board escort rewards remain unchanged. Save format unchanged; all 2.5.373/372 fixes retained.

106 focused emulator checks pass, including 19 conversation-routing matrix checks, 66 existing Comms checks, Codex/reward persistence, Quiet Watch and startup. Native 480×272 sanctuary capture inspected. Core comparison against 2.5.373 has the same 15 inherited failures; radio, steering and NPC combat checks pass. Physical PSP testing remains pending. Details: docs/CONVERSATION-OWNERSHIP-374.md and BUILD-2.5.374.txt. Highest-priority work: hardware playtest, inherited reliability, then planetary work.

Local release: outputs/releases/2.5.374. SHA256: {sha}. GitHub not updated; pull blocked by existing untracked music placeholders, autostash restored and mixed shared work preserved.

'''
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
    p = root/name
    text = read(p)
    if not text.startswith('## 2.5.374'):
        p.write_text(entry+text,encoding='utf-8')
print(json.dumps(dict(version='2.5.374',sha256=sha,checks=checks,eboot=str(release/'EBOOT.PBP'),zip=str(archive)),indent=2))
