from pathlib import Path
import hashlib,json,shutil
root=Path(__file__).resolve().parents[1];out=root/'deliverables/ELITE-NEXT-2.5.485-SOLID-TWIN-GUNS'
v=json.loads((out/'validation.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert (root/'VERSION').read_text().strip()=='2.5.484' and sha(out/'EBOOT.PBP')==v['sha256']
assert all(sha(root/f).lower()==h for f,h in v['source_hashes'].items())
# Keep the stricter performance failures visible; this is a focused gun fix,
# not a claim that every workload meets a locked 30 FPS release gate.
stable=root/'dist/ELITE-NEXT-PSP/EBOOT.PBP';stable_sha=sha(stable)
assert stable_sha=='ABF212BD1D7AF7140D467300FF3C701A110442F0A1BC1BA9055C6A6D44A9ED05'
perf='; '.join(c+' '+', '.join(str(x) for x in d['fps_windows'])+' FPS' for c,d in v['performance'].items())
entry=f'''2.5.485 PSP-3000 local playtest rebuilds the short lower-corner Twin Stack as solid 3D guns. Each bank has two separated eight-sided barrels with closed caps, raised sleeve collars, narrow closed breeches, top/side/underside facet shading, vents, bolts and cyan material accents. Both sides of each receiver and all barrel faces carry depth, replacing the broad flat receiver sheets. The short inward-facing centreline and lower-corner mounting positions are retained. Alternating discharge now uses the actual lower/upper barrel centres at six-unit spacing, with emission beyond their matching far tips. Weapon slots, input, damage, cooldowns and aiming rules are unchanged.

{v['native_checks']} focused native checks pass, including actual Anaconda shipyard purchase, all four fitted gun identities, Circle+Right immediate switch/wrap, lower outer-corner receiver projections and matching far-tip emission axes. Previous cockpit, fitting/save, cosmetics, menu, departure/adverts and ERL tests pass. All-hull captures are retained with the diagnostic reports; source hashes are in design/guns485. Native before/after, approved reference and80-frame lossless real retail footage: assets/generated/guns485/index.html. Footage writing overhead is excluded from separate performance tests.

Native-resolution PPSSPP Anaconda timings: {perf}. Zero audio starvation and command drops. Strict-floor limitations: {v['performance_limitations']}. Worst frames are in validation.json; physical PSP confirmation, occasional frame-time spikes and pre-existing broader smoke failures remain open. Memory: {v['memory_budget']}. The user accepts approximately29FPS with visual fidelity preserved; continue to investigate noticeable frame-time spikes rather than reducing the approved model to chase exactly30FPS. This is a local playtest, not a locked-framerate release claim. Saves protected and intro bytes unchanged. Previous484 package and original PSP stable distribution retained. No Git metadata exists here; no commit, push, tag or GitHub release is claimed. SHA-256: {v['sha256']}'''
priority='Next: physically check the Anaconda cockpit housings, four fitted guns and far-tip firing on PSP-3000. Continue frame-time optimisation and outstanding encounter work without reducing approved artwork. Preserve the separation between weapon slot capacity and the visible paired Twin Stack housings.'
for name in ['README.md','CLAUDE-HANDOFF.md']:
 p=root/name;p.write_text(entry+'\n\n'+priority+'\n\n'+p.read_text(encoding='utf-8-sig'),encoding='utf-8')
p=root/'CHANGELOG.md';p.write_text('## 2.5.485 — solid 3D twin-barrel guns\n\n'+entry+'\n\n'+p.read_text(encoding='utf-8-sig'),encoding='utf-8')
(root/'design/SOLID-TWIN-GUNS485.md').write_text(entry+'\n\n'+priority+'\n',encoding='utf-8')
(root/'VERSION').write_text('2.5.485\n')
for name in ['build.ps1','build.sh']:
 p=root/name;p.write_text(p.read_text(encoding='utf-8-sig').replace('Release 2.5.484:','Release 2.5.485:'),encoding='utf-8')
for dest in [root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP3000/EBOOT.PBP']:
 shutil.copy2(out/'EBOOT.PBP',dest);assert sha(dest)==v['sha256']
assert sha(stable)==stable_sha
(root/'design/guns485/PROMOTION.json').write_text(json.dumps({'version':'2.5.485','sha256':v['sha256'],'native_checks':v['native_checks'],'original_psp_stable_sha256':stable_sha,'physical_psp_verified':False,'git_synced':False},indent=2))
print('Promoted verified local 2.5.485 PSP-3000 build; original PSP stable unchanged.')
