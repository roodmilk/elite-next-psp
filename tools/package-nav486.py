import sys,os
if not sys.flags.utf8_mode:
 os.execv(sys.executable,[sys.executable,'-X','utf8',__file__,*sys.argv[1:]])
from pathlib import Path
import argparse,hashlib,json,re,shutil,struct,zipfile,html
p=argparse.ArgumentParser();p.add_argument('--stage',default='i');a=p.parse_args();root=Path(__file__).resolve().parents[1];stage=a.stage
bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice';evidence=root/'design/nav-events486'
review=root/f'build-nav486-review-{stage}/EBOOT.PBP';retail=root/f'build-nav486-retail-{stage}/EBOOT.PBP'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
source=json.loads((evidence/f'{stage}-source-hashes.json').read_text());assert all(sha(root/f).lower()==h for f,h in source.items()),'Source changed after validation snapshot'
validation={'version':'2.5.486','retail_sha256':sha(retail),'review_sha256':sha(review),'source_hashes':source,'physical_psp_verified':False,'git_synced':False,'checks':{},'performance':{},'broader_smoke':json.loads((evidence/'BROAD-SMOKE-LIMITATION.json').read_text())}
flags=['navigation-review','port-ad-review','departure-hud-review','loadout-picker-review','dashboard-review','charm-review','cockpit-hud-review','final-menu','erl-quick-race-review']
for flag in flags:
 tag=f'nav486-{stage}' if flag=='navigation-review' else f'nav486-{stage}-regress';run=bench/f'regression-{tag}-diagnostic-{flag}';report=(run/f'{flag}.txt').read_text();binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
 assert 'RESULT 0 failures' in report and binding['eboot_sha256']==sha(review),flag
 m=re.search(r'CHECKS (\d+)',report) or re.search(r'/ (\d+) checks',report);count=int(m[1]) if m else len(re.findall(r'^PASS ',report,re.M))
 validation['checks'][flag]={'count':count,'report':str(run/f'{flag}.txt')}
 iso=run/'save-isolation.json'
 if iso.exists():assert json.loads(iso.read_text(encoding='utf-8-sig'))['all_unchanged'],flag
validation['native_checks']=sum(x['count'] for x in validation['checks'].values());assert validation['native_checks']==360,validation['native_checks']
for case in ['gas','storm','aurora','wreckage','comet','veil','quiet']:
 run=bench/f'regression-nav486-{stage}-{case}-diagnostic-encounter-flight-capture';report=(run/'encounter-flight-capture.txt').read_text();binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
 assert binding['eboot_sha256']==sha(retail) and binding['internal_resolution']==1 and not binding['footage'],case
 assert 'starves 0 drops 0' in report and re.search(r'^RESULT ',report,re.M),case
 windows=[float(x) for x in re.findall(r'orbit ([\d.]+) FPS',report)];worst=[float(x) for x in re.findall(r'worst ([\d.]+) ms',report)];assert len(windows)==3
 validation['performance'][case]={'fps_windows':windows,'worst_frame_ms':worst,'strict_29_5_failures':len(re.findall(r'^FAIL ',report,re.M)),'report':str(run/'encounter-flight-capture.txt')}
 # Preserve every strict-floor failure in the local-playtest report; do not claim a locked framerate.
validation['memory_budget']=re.search(r'PASS retail PSP memory budget:[^\r\n]+',(root/f'build-nav486-retail-{stage}.log').read_text())[0]
validation['performance_limitations']=[f'{case}: {data["fps_windows"]}' for case,data in validation['performance'].items() if data['strict_29_5_failures']]
def psar_hash(path):
 data=path.read_bytes();offsets=struct.unpack('<10I',data[:40])[2:];return hashlib.sha256(data[offsets[7]:]).hexdigest()
old=root/'deliverables/ELITE-NEXT-2.5.485-SOLID-TWIN-GUNS/EBOOT.PBP';assert psar_hash(old)==psar_hash(retail);validation['intro_unchanged']=True
stable=root/'dist/ELITE-NEXT-PSP/EBOOT.PBP';stable_sha=sha(stable);assert stable_sha=='ABF212BD1D7AF7140D467300FF3C701A110442F0A1BC1BA9055C6A6D44A9ED05';validation['original_psp_stable_sha256']=stable_sha
out=root/'deliverables/ELITE-NEXT-2.5.486-EXPLORATION';out.mkdir(parents=True,exist_ok=True);shutil.copy2(retail,out/'EBOOT.PBP')
(out/'validation.json').write_text(json.dumps(validation,indent=2))
controls="""Hold Square and use Left/Right to open NAV LOG. X plots coordinates without changing your target. R aligns the ship to the plotted lead. Physical contacts still lock/track in their own categories.
A faint scanner trace gives a rough bearing, UP/DN and strength. It is not an identified live contact. Slow below 120 m/s within 700 m of a buoy for two seconds to read its local links.
GalacticNet > Rumours works remotely: three local search offers cost 25/35/45 U. Owned/scanned directions stay free. Previously bought sightings remain available. A search area is not a weapon lock.
On an inhabited ocean planet's parked-ship screen, Square accepts the local courier. Its station coordinates belong to that mission unless independently supplied or visited.
At zero fuel, ordinary flight remains usable; boost and hyperspace are unavailable.
"""
limits='Physical PSP confirmation and existing frame-time spikes remain open. This is a local PSP-3000/emulator playtest, not a locked-framerate or complete commercial-release claim. The broader old smoke suite is blocked by the pre-existing obsolete departure_ad_text test helper. The 24-entry R&D catalogue is not 24 shipped mechanics; this release establishes six environmental families through shared systems.'
(out/'READ-ME.txt').write_text('ELITE NEXT 2.5.486 / EXPLORATION\n\nPSP-3000 / PPSSPP. Copy ELITE_NEXT into PSP/GAME, or replace EBOOT.PBP in your existing installation. Keep your saves and existing music. Music is optional and not included; the introduction is embedded. No personal save files are bundled.\n\n'+controls+'\nNew saves use V39; older V37/V38 saves load. Preserve an older backup if reverting to a previous executable.\n\n'+limits+'\n\nSHA-256 '+sha(retail)+'\n')
with zipfile.ZipFile(str(out)+'.zip','w',zipfile.ZIP_DEFLATED) as z:
 for name in ['EBOOT.PBP','READ-ME.txt','validation.json']:z.write(out/name,'ELITE_NEXT/'+name)
with zipfile.ZipFile(str(out)+'.zip') as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha(retail)
perf='; '.join(f'{k}: '+', '.join(map(str,v['fps_windows']))+' FPS' for k,v in validation['performance'].items())
entry=f"""2.5.486 local PSP-3000 exploration playtest adds passive bearings, six linked physical navigation buoys, a separate navigation log, persistent visited coordinates, mission-only coordinate providers, and three remote local search offers. Live scanner locks remain range-limited and independent of navigation. V39 appends CRC-protected knowledge and paid data; V37/V38 migration passes. Existing HUD, guns, cosmetics, story/tutorial, menu/Extras and ERL integrations are retained.

Six finite spatial-region families use seeded world-space layers, opacity artwork and local lighting through a batched PSP GU atlas. Gas, magnetic activity, auroral curtains, wreckage wakes, comet streams and quiet dust keep depth/parallax and gradual boundaries. Magnetic activity has one bounded temporary engine-output response. Ordinary flight works at zero fuel. The expanded layout is shared across 256 systems and rejects unsafe arrivals/inactive-body anchors.

{validation['native_checks']} focused native checks pass. A complete script-steered Iron Cathedral test of the Lave search/buoy/listening-post branch and return takes 206.6 seconds and 2.393 LY; the zero-fuel 16 km recovery takes 64.9 seconds. These are actual simulation journeys, not human playthrough claims. Native engine images, moving-camera footage, reference comparison and reports: assets/generated/navigation486/index.html.

Retail native PPSSPP performance with the actual four-gun Anaconda and current 3D cockpit housings: {perf}. Zero audio buffer starvation/drops in these tests. Strict 29.5 FPS limitations: {validation['performance_limitations']}. Memory: {validation['memory_budget']}. {limits}

{controls}
Original PSP stable and 2.5.485 download remain intact. No Git metadata exists in this mirror, so no commit/push/tag or remote release is claimed. SHA-256: {sha(retail)}
"""
priority='Highest-priority remaining work: physical PSP-3000 check of navigation, cloud boundaries and saves; investigate remaining frame-time spikes; repair the pre-existing full-smoke test-helper drift; validate extended career/escort/planetary playthroughs and expand the remaining R&D event proposals only from these shared foundations.'
for name in ['README.md','CLAUDE-HANDOFF.md']:
 path=root/name;previous=path.read_text(encoding='utf-8-sig')
 if previous.startswith('2.5.486 '):previous=previous[previous.index('2.5.485 PSP-3000'): ]
 path.write_text(entry+'\n'+priority+'\n\n'+previous)
path=root/'CHANGELOG.md';previous=path.read_text(encoding='utf-8-sig')
if previous.startswith('## 2.5.486'):previous=previous[previous.index('## 2.5.485'): ]
path.write_text('## 2.5.486 â€” exploration, navigation and spatial regions\n\n'+entry+'\n'+previous)
(evidence/'RELEASE.txt').write_text(entry+'\n'+priority+'\n');(root/'VERSION').write_text('2.5.486\n')
for name in ['build.ps1','build.sh']:
 path=root/name;path.write_text(path.read_text(encoding='utf-8-sig').replace('Release 2.5.485:','Release 2.5.486:'))
for destination in [root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP3000/EBOOT.PBP']:shutil.copy2(retail,destination);assert sha(destination)==sha(retail)
assert sha(stable)==stable_sha
(evidence/'PROMOTION.json').write_text(json.dumps({'version':'2.5.486','sha256':sha(retail),'native_checks':validation['native_checks'],'original_psp_stable_sha256':stable_sha,'physical_psp_verified':False,'git_synced':False},indent=2))
with zipfile.ZipFile(evidence/'release-source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for file in source:z.write(root/file,file)
 for name in ['VERSION','build.ps1','build.sh','README.md','CLAUDE-HANDOFF.md','CHANGELOG.md']:z.write(root/name,name)
gallery=root/'assets/generated/navigation486';shutil.copy2(out/'validation.json',gallery/'validation.json')
page=gallery/'index.html';page.write_text(page.read_text().replace('<p><a href="navigation-review.txt">','<h2>Retail performance</h2><p>'+html.escape(perf)+'</p><p>'+html.escape(limits)+'</p><p><a href="validation.json">Full validation and timings</a></p><p><a href="navigation-review.txt">'))
print(json.dumps({'package':str(out),'sha256':sha(retail),'checks':validation['native_checks'],'performance':validation['performance']},indent=2))
