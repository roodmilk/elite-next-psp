from pathlib import Path
import hashlib,json,shutil
root=Path(__file__).resolve().parents[1]
out=root/'deliverables/ELITE-NEXT-2.5.482-PORT-ADVERTS'
v=json.loads((out/'validation.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert (root/'VERSION').read_text().strip()=='2.5.481' and sha(out/'EBOOT.PBP')==v['sha256']
assert all(sha(root/f).lower()==h for f,h in v['source_hashes'].items())
stable=root/'dist/ELITE-NEXT-PSP/EBOOT.PBP';stable_sha=sha(stable)
assert stable_sha=='ABF212BD1D7AF7140D467300FF3C701A110442F0A1BC1BA9055C6A6D44A9ED05'
perf='; '.join(c+' '+', '.join(str(n) for n in d['fps_windows'])+' FPS' for c,d in v['performance'].items())
entry=f'''2.5.482 local PSP-3000 station departure update: six custom 64x32 pixel-art advertisements (KOYO bowl, NOVA star/casino, VOX radio, DRIFT courier, AZURE helmet and ORBIT hotel) map to real wall/building planes with perspective-correct UV interpolation and near clipping. Exact background-paper masking preserves foreground rails, ship geometry and HUD. Lave's corridor uses DRIFT/VOX rather than another KOYO label; station profile varies the pair. Larger building adverts stay visible for about four seconds at Lave. Native proof: assets/generated/port482/index.html, 81 losslessly verified 480x272 frames spanning 8.1 seconds.

Normal station departure lasts eight seconds with slower motion near building adverts, then a small smooth lane curve starting at least 100m beyond the station hull. Position and camera use the same physical path; handoff is aligned at cruise speed with all staged scenery cleared. The first authored Earth ascent retains its original five seconds and straight path, preserving the existing city cutscene. Poor/rich/mega/relay geometry and traffic use the same transformed camera basis. Crossing/outbound/inbound background ships are deterministic decorative meshes, not new persistent simulated NPCs. The player's inbound docking cinematic is not redesigned.

{v['native_checks']} targeted native checks pass:15 port art/timing/clearance,24 departure HUD/tracking,60 cockpit,32 fitting/save,78 dashboard,35 hanging charms,7 menu and51 ERL routes. All256 system departure profiles finish smoothly without fuel/hull loss; all256 clear staged scenery before handoff. Poster raster preserves foreground geometry and HUD; perspective and advert dwell are checked. Protected saves remain unchanged. Previous481 controls, cockpit, cosmetics and fitting are retained without art simplification. Intro embedded bytes remain identical.

Separate final retail timings: {perf}. Runs use native-resolution PPSSPP PSP-3000, host muted, mixer/display enabled, private saves; no recording, compilation or compression during measurement. Launch windows include the actual eight-second departure followed by coasting. Zero audio starvation/command drops. Per-window worst frame times and strict29.5FPS floor status are recorded in validation.json; averages are not a locked-framerate guarantee. Physical PSP confirmation, frame-time spikes and pre-existing broader legacy smoke failures remain open. Memory: {v['memory_budget']}. Original PSP stable distribution and481 package are preserved. No Git metadata or GitHub publication claim. SHA-256: {v['sha256']}'''
priority='Next: physically test PSP-3000 station departures, including first story ascent, rich/capital/relay ads, held-button handoff and real save loading. Investigate remaining frame-time spikes without reducing approved artwork. Inbound player docking scenery remains a separate future improvement. Continue other encounter interaction/placement work from the existing audit.'
for name in ['README.md','CLAUDE-HANDOFF.md']:
 p=root/name;p.write_text(entry+'\n\n'+priority+'\n\n'+p.read_text(encoding='utf-8-sig'),encoding='utf-8')
p=root/'CHANGELOG.md';p.write_text('## 2.5.482 — illustrated perspective adverts and station departure\n\n'+entry+'\n\n'+p.read_text(encoding='utf-8-sig'),encoding='utf-8')
(root/'design/PORT-ADVERTS482.md').write_text(entry+'\n\n'+priority+'\n\nProduction and compiled diagnostic runtime sources were frozen before final v3 compilation. Two excluded full-EnableReview legacy fixture files were updated afterwards to account for the eight-second path; see design/port482/NONCOMPILED-REVIEW-UPDATE.json. They are excluded from both delivered retail and EncounterReview builds. final-source.zip and final-source-hashes.json record current sources. Earlier v1/v2 reports are historical; final validation binds all reports to the final v3 binaries.\n',encoding='utf-8')
(root/'VERSION').write_text('2.5.482\n')
for name in ['build.ps1','build.sh']:
 p=root/name;p.write_text(p.read_text(encoding='utf-8-sig').replace('Release 2.5.481:','Release 2.5.482:'),encoding='utf-8')
shutil.copy2(out/'EBOOT.PBP',root/'EBOOT.PBP');shutil.copy2(out/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP3000/EBOOT.PBP')
assert sha(stable)==stable_sha and sha(root/'EBOOT.PBP')==v['sha256'] and sha(root/'dist/ELITE-NEXT-PSP3000/EBOOT.PBP')==v['sha256']
(root/'design/port482/PROMOTION.json').write_text(json.dumps({'version':'2.5.482','sha256':v['sha256'],'native_checks':v['native_checks'],'original_psp_stable_sha256':stable_sha,'physical_psp_verified':False,'git_synced':False},indent=2))
print('Promoted tested local PSP-3000 2.5.482; original PSP stable package unchanged.')
