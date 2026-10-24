from pathlib import Path
import hashlib,json,shutil
root=Path(__file__).resolve().parents[1];out=root/'deliverables/ELITE-NEXT-2.5.483-ANACONDA-GUNS'
v=json.loads((out/'validation.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert (root/'VERSION').read_text().strip()=='2.5.482' and sha(out/'EBOOT.PBP')==v['sha256']
assert all(sha(root/f).lower()==h for f,h in v['source_hashes'].items())
# Keep the stricter performance failures visible; this is a focused gun fix,
# not a claim that every workload meets a locked 30 FPS release gate.
stable=root/'dist/ELITE-NEXT-PSP/EBOOT.PBP';stable_sha=sha(stable)
assert stable_sha=='ABF212BD1D7AF7140D467300FF3C701A110442F0A1BC1BA9055C6A6D44A9ED05'
perf='; '.join(c+' '+', '.join(str(x) for x in d['fps_windows'])+' FPS' for c,d in v['performance'].items())
entry=f'''2.5.483 PSP-3000 local playtest fixes tiny, high Twin Stack housings on large ships. The old 12m model-size ceiling shrank the Anaconda's guns despite their real hull mounts being over 200m ahead of the cockpit origin. Housings now retain their approved proportions at each actual hardpoint depth, including closed receivers, stacked sleeves, braces, material details and reflections. Muzzle effects continue to originate at the same far-tip geometry. No hardpoints, aiming, damage, cooldowns, slot capacity or approved artwork were replaced.

{v['native_checks']} focused native checks pass. The new regression buys an Anaconda through the actual shipyard, confirms four WPN slots, fires each of four fitted weapon identities, and uses Circle+Right to switch immediately through all four and wrap. It checks substantial receiver size and lower cockpit placement; all previous cockpit, fitting/save, cosmetics, menu/intro, departure and ERL checks pass. All-hull native captures are retained. Existing482 station adverts and eight-second departures are preserved. Source snapshot: design/guns483/final-source.zip and final-source-hashes.json.

Native before/after, approved reference and80-frame lossless actual retail firing footage: assets/generated/guns483/index.html. Footage has disk-writing overhead and is not an FPS benchmark. Separate native-resolution PPSSPP Anaconda timings, without compilers/recording: {perf}. Zero audio starvation and command drops. Busy combat and one station window miss the strict29.5FPS benchmark floor (roughly29FPS busy,29.45FPS station); this limitation is retained in validation.json rather than marked as a pass. The gun correction is delivered as a local playtest, not a completed performance milestone. Worst-frame timings, occasional spikes, physical PSP confirmation and pre-existing broader smoke failures remain open. {v['memory_budget']}. Intro bytes unchanged; protected saves unchanged. Earlier482 package and original PSP stable distribution remain available. No Git repository metadata exists in this mirror, so no commit/push/tag or GitHub release is claimed. SHA-256: {v['sha256']}'''
priority='Next: physically check the Anaconda cockpit housings, four fitted guns and far-tip firing on PSP-3000. Continue frame-time optimisation and outstanding encounter work without reducing approved artwork. Preserve the separation between weapon slot capacity and the visible paired Twin Stack housings.'
for name in ['README.md','CLAUDE-HANDOFF.md']:
 p=root/name;p.write_text(entry+'\n\n'+priority+'\n\n'+p.read_text(encoding='utf-8-sig'),encoding='utf-8')
p=root/'CHANGELOG.md';p.write_text('## 2.5.483 — full-sized large-ship guns\n\n'+entry+'\n\n'+p.read_text(encoding='utf-8-sig'),encoding='utf-8')
(root/'design/ANACONDA-GUNS483.md').write_text(entry+'\n\n'+priority+'\n',encoding='utf-8')
(root/'VERSION').write_text('2.5.483\n')
for name in ['build.ps1','build.sh']:
 p=root/name;p.write_text(p.read_text(encoding='utf-8-sig').replace('Release 2.5.482:','Release 2.5.483:'),encoding='utf-8')
for dest in [root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP3000/EBOOT.PBP']:
 shutil.copy2(out/'EBOOT.PBP',dest);assert sha(dest)==v['sha256']
assert sha(stable)==stable_sha
(root/'design/guns483/PROMOTION.json').write_text(json.dumps({'version':'2.5.483','sha256':v['sha256'],'native_checks':v['native_checks'],'original_psp_stable_sha256':stable_sha,'physical_psp_verified':False,'git_synced':False},indent=2))
print('Promoted verified local 2.5.483 PSP-3000 build; original PSP stable unchanged.')
