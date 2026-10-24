"""Promote the validated local cockpit playtest without touching original-PSP dist."""
from pathlib import Path
import hashlib,json,shutil,re
root=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
frozen=json.loads((root/'design/cockpit-hud478/candidate-v17-source-hashes.json').read_text())
assert all(sha(root/f).lower()==h.lower() for f,h in frozen.items())
package=root/'deliverables/ELITE-NEXT-2.5.478-COCKPIT'
v=json.loads((package/'validation.json').read_text())
assert v['diagnostic_checks']==60 and v['native_reference_mismatches']==0
assert sha(package/'EBOOT.PBP')==v['sha256']==sha(root/'build-cockpit478-v17/EBOOT.PBP')
prior=root/'deliverables/ELITE-NEXT-2.5.477-NINE-REWORKS/EBOOT.PBP'
assert sha(prior)=='D043EA4DBE7499A21B596D0C7F396768CF84AC487C74C07DAC67AC966FF36329'
assert sha(root/'EBOOT.PBP') in (sha(prior),v['sha256'])
stable=root/'dist/ELITE-NEXT-PSP/EBOOT.PBP';stable_before=sha(stable)
summary='2.5.478 local PSP-3000 cockpit + G Twin Stack playtest: approved armoured computers, native font, full tool names, real slot dots, immediate Circle+Right fitted-gun cycling and explicit Square/D-pad/X contact confirmation. Both computers preserve live analog flight; R+X confirms and aligns without boosting. Twin housings retain stacked sleeves, closed receivers, panel detail, far-tip firing, recoil, glow and varied reflections at actual hull hardpoints. Native colour/depth caches and opaque HUD spans optimise drawing without an art-detail cut. All 23 hull mount views and 60 native control/layout/cache checks pass. Menu/intro 7 and ERL 51 checks pass with private saves unchanged; embedded intro is byte-identical. Exact-retail native benchmarks, strict frame-rate shortfalls and separate capture-timed APNG comparisons are recorded in assets/generated/cockpit-hud478. Rapid-fire sound-command overflow fixed using 384 extra bytes of bounded queue storage. Weapon balance and save layout retained. Physical PSP testing remains pending; separate broad legacy smoke is not all green. Prior 477 and PSP-original stable dist preserved. No Git metadata, commit, push or tag claim. SHA-256: '+v['sha256']
priority='Highest-priority remaining work after 478: review the native side-by-side footage on PSP-3000 and confirm input, sound, long-session saves and suspend/resume on hardware; address disclosed heavy-combat/chase/high-contrast frame pacing without simplifying approved artwork. Resolve or modernise the recorded broad-smoke failures before a commercial release gate. Continue the remaining 58 encounter catalogue designs individually under design/ENCOUNTER-ART-STANDARD.md after this cockpit review; preserve 477 art, intro, ERL and saves. This is a local playtest, not an all-green commercial release.'
for name in ('README.md','CHANGELOG.md','CLAUDE-HANDOFF.md'):
 p=root/name;s=p.read_text(encoding='utf-8-sig')
 if not s.startswith('2.5.478 local PSP-3000 cockpit'):
  s=summary+'\n\n'+(priority+'\n\n' if name=='CLAUDE-HANDOFF.md' else '')+s
  p.write_text(s,encoding='utf-8',newline='\n')
(root/'VERSION').write_text('2.5.478\n',encoding='utf-8')
for name in ('build.ps1','build.sh'):
 p=root/name;s=p.read_text(encoding='utf-8-sig').replace('Release 2.5.477:','Release 2.5.478:');p.write_text(s,encoding='utf-8',newline='\n')
shutil.copy2(package/'EBOOT.PBP',root/'EBOOT.PBP')
dist=root/'dist/ELITE-NEXT-PSP3000';dist.mkdir(exist_ok=True)
for name in ('EBOOT.PBP','README.txt','validation.json'):shutil.copy2(package/name,dist/name)
assert sha(stable)==stable_before and sha(root/'EBOOT.PBP')==v['sha256']
record=dict(version='2.5.478',status='Local PSP-3000 playtest; physical verification pending',sha256=v['sha256'],source_hashes=frozen,prior477_preserved=sha(prior),original_psp_dist_unchanged=stable_before,git_sync=False,known_limits=v['limitations'],strict_fps_shortfalls=v['strict_fps_shortfalls'])
(root/'design/cockpit-hud478/PROMOTION.json').write_text(json.dumps(record,indent=2),encoding='utf-8')
p=root/'design/cockpit-hud478/WORK-IN-PROGRESS.json';d=json.loads(p.read_text());d.update(promoted=True,local_playtest_version='2.5.478',root_and_dist_still='Root478; separatePSP3000dist478; originalPSPstable unchanged',next=['Physical PSP-3000 review; see PROMOTION.json and CLAUDE-HANDOFF.md'],reason_for_final_rebuild='Resolved v16 rapid-fire9-command overflow with384 bytes of fixed queue storage. Final v17 validation recorded in package.');p.write_text(json.dumps(d,indent=2),encoding='utf-8')
print(json.dumps(record,indent=2))
