from pathlib import Path
from PIL import Image
import hashlib,json,re,shutil,zipfile,struct
root=Path(__file__).resolve().parents[1];bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice';out=root/'deliverables/ELITE-NEXT-2.5.481-LAUNCH-HUD';gallery=root/'assets/generated/departure481';retail=root/'build-departure481/EBOOT.PBP';review=root/'build-departure481-review-v2/EBOOT.PBP';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
hashes=json.loads((root/'design/departure481/final-source-hashes.json').read_text());assert all(sha(root/f).lower()==h for f,h in hashes.items())
v={'version':'2.5.481','profile':'PSP-3000 local playtest','sha256':sha(retail),'source_hashes':hashes,'checks':{},'performance':{},'physical_psp_verified':False,'git_synced':False,'limitations':['Native clip is a fixed-step actual-renderer fixture, not a retail FPS measurement.','Physical PSP confirmation and existing broader legacy smoke failures remain open.','Measured averages are not a locked-framerate guarantee; occasional frame-time spikes remain.']}
for flag in ['departure-hud-review','cockpit-hud-review','loadout-picker-review','dashboard-review','charm-review','final-menu','erl-quick-race-review']:
 r=bench/f'regression-dep481-final-diagnostic-{flag}';t=(r/(flag+'.txt')).read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'));assert 'RESULT 0 failures' in t and b['eboot_sha256']==sha(review);m=re.search(r'CHECKS (\d+)',t);v['checks'][flag]={'count':int(m[1]) if m else len(re.findall(r'^PASS ',t,re.M)),'report':str(r/(flag+'.txt'))};iso=r/'save-isolation.json'
 if iso.exists():assert json.loads(iso.read_text(encoding='utf-8-sig'))['all_unchanged']
v['native_checks']=sum(x['count'] for x in v['checks'].values())
for case in ['launch','cruise','boost']:
 r=bench/f'regression-dep481-{case}-diagnostic-encounter-flight-capture';t=(r/'encounter-flight-capture.txt').read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'));assert b['eboot_sha256']==sha(retail) and not b['footage'];assert 'audio buffers: starves 0 drops 0' in t
 fps=[float(n) for n in re.findall(r'orbit ([\d.]+) FPS',t)];assert len(fps)==3
 v['performance'][case]={'fps_windows':fps,'worst_frame_ms':[float(n) for n in re.findall(r'worst ([\d.]+) ms',t)],'strict_29_5_floor_pass':'RESULT 0 failures' in t,'audio_starves':0,'audio_drops':0,'report':str(r/'encounter-flight-capture.txt')}
v['memory_budget']=re.search(r'PASS retail PSP memory budget:.*',(root/'build-departure481.log').read_text())[0]
data=retail.read_bytes();assert data[struct.unpack('<10I',data[:40])[9]:]==(root/'assets/intro/cinematic.enm').read_bytes();v['intro_identical']=True
r=bench/'regression-dep481-final-diagnostic-departure-hud-review';gallery.mkdir(parents=True,exist_ok=True)
paths=sorted(r.glob('launch-native-*.bmp'));frames=[Image.open(p).convert('RGBA') for p in paths];assert len(frames)==51
frames[0].save(gallery/'launch-native-motion.png',save_all=True,append_images=frames[1:],duration=100,loop=0,disposal=0,blend=0)
expanded=[]
with Image.open(gallery/'launch-native-motion.png') as motion:
 for k in range(motion.n_frames):motion.seek(k);expanded.extend([motion.convert('RGBA').tobytes()]*round(motion.info.get('duration',100)/100))
assert expanded==[f.tobytes() for f in frames];v['lossless_native_frames']=len(expanded)
for src,name in [('launch-native-000.bmp','launch-start.png'),('launch-native-147.bmp','launch-end.png'),('launch-native-150.bmp','free-flight.png'),('target-r-track.bmp','r-track.png')]:Image.open(r/src).save(gallery/name)
rows=''.join('<tr><td>'+c+'</td><td>'+', '.join(f'{n:.2f}' for n in d['fps_windows'])+'</td></tr>' for c,d in v['performance'].items())
(gallery/'index.html').write_text('''<!doctype html><meta charset="utf-8"><title>Elite Next 2.5.481 — launch cockpit continuity</title><style>body{background:#07121b;color:#d5e3e4;font:16px system-ui;margin:24px;line-height:1.5}h1,h2{color:#f1ba72}p{max-width:94ch}main{display:flex;flex-wrap:wrap;gap:24px}img{width:480px;image-rendering:pixelated;border:1px solid #48616d}td,th{text-align:left;padding:8px 24px 8px 0}</style><h1>Current cockpit throughout station launch</h1><p>Actual 480×272 in-engine captures. The same cockpit renderer, rolling speed ticker, Twin Stack housings, fitted charm and dashboard artwork are used from launch through the free-flight handoff. Autopilot retains control until completion and button release. The indicator is below the top console without covering its text.</p><img src="launch-native-motion.png"><p>51 losslessly verified native frames covering 5.1 seconds. Fixed-step renderer/input fixture; FPS is measured separately in the exact retail build.</p><main><section><h2>Autopilot start</h2><img src="launch-start.png"></section><section><h2>Last launch frames</h2><img src="launch-end.png"></section><section><h2>Free-flight handoff</h2><img src="free-flight.png"></section><section><h2>R locks and turns toward the highlighted contact</h2><img src="r-track.png"></section></main><p>Hold Square to browse. X locks without turning; R locks the highlighted detected contact and enables alignment. The R press is consumed by the computer, preventing boost or throttle activation while browsing. Releasing Square preserves the confirmed tracking.</p><h2>Separate retail performance</h2><table><tr><th>Workload</th><th>Three FPS windows</th></tr>'''+rows+'''</table><p>PPSSPP PSP-3000 at native resolution, normal mixer/display, host muted. Launch workload uses the actual five-second departure and then normal coasting; the three windows include the subsequent free-flight period. No recording or compilers ran during these timings. Zero audio starvation and command drops. Physical PSP testing, occasional frame-time spikes and pre-existing broader smoke failures remain open.</p>''',encoding='utf-8')
out.mkdir(parents=True,exist_ok=True);shutil.copy2(retail,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(v,indent=2))
(out/'README.txt').write_text(f'''ELITE NEXT 2.5.481 — PSP-3000 LOCAL PLAYTEST

Replace EBOOT.PBP only in an existing installation; keep saves and music. Fresh: copy ELITE_NEXT into PSP/GAME/ or open EBOOT.PBP in PPSSPP. Music is not bundled. Intro is embedded unchanged. Save an active older session successfully before replacing its executable.

The current cockpit now remains visible throughout the five-second station departure and handoff, including fitted cosmetics, Twin Stack housings and the rolling speed display. Autopilot input and fresh-release protections remain in place.

Hold Square to browse targets. X locks without turning. R locks the highlighted contact AND turns to track it; it cannot also trigger boost/throttle while browsing. Release Square to close and retain tracking. Existing X+R alignment remains supported.

All 2.5.480 quick-fitting, cosmetics, save handling, menu, intro and ERL work is retained. {v['native_checks']} focused native checks pass. See validation.json and assets/generated/departure481/index.html for evidence, timing and limits. Physical PSP confirmation pending; frame-time spikes and pre-existing broader smoke failures remain open. Local build, no GitHub release claim.

SHA-256 {v['sha256']}
''',encoding='utf-8')
with zipfile.ZipFile(str(out)+'.zip','w',zipfile.ZIP_DEFLATED) as z:
 for p in out.iterdir():z.write(p,'ELITE_NEXT/'+p.name)
print(json.dumps({'sha256':v['sha256'],'native_checks':v['native_checks'],'performance':v['performance']},indent=2))
