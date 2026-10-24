from pathlib import Path
from PIL import Image
import hashlib,json,re,shutil,zipfile,struct

root=Path(__file__).resolve().parents[1]
bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice'
out=root/'deliverables/ELITE-NEXT-2.5.482-PORT-ADVERTS'
gallery=root/'assets/generated/port482'
retail=root/'build-port482-v3/EBOOT.PBP'
review=root/'build-port482-review-v3/EBOOT.PBP'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
hashes=json.loads((root/'design/port482/final-source-hashes.json').read_text())
assert all(sha(root/f).lower()==h for f,h in hashes.items())
v={'version':'2.5.482','profile':'PSP-3000 local playtest','sha256':sha(retail),'source_hashes':hashes,'checks':{},'performance':{},'physical_psp_verified':False,'git_synced':False,'limitations':['Native animation is a fixed-step actual-renderer fixture, not a retail FPS measurement.','Physical PSP confirmation and existing broader legacy smoke failures remain open.','Measured averages are not a locked-framerate guarantee; frame-time spikes are reported.','Inbound player docking cinematic was not redesigned; additional arriving ships are background scenery.']}
for flag in ['port-ad-review','departure-hud-review','cockpit-hud-review','loadout-picker-review','dashboard-review','charm-review','final-menu','erl-quick-race-review']:
 r=bench/f'regression-port482-v3-diagnostic-{flag}'
 t=(r/(flag+'.txt')).read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'))
 assert 'RESULT 0 failures' in t and b['eboot_sha256']==sha(review),flag
 m=re.search(r'CHECKS (\d+)',t)
 v['checks'][flag]={'count':int(m[1]) if m else len(re.findall(r'^PASS ',t,re.M)),'report':str(r/(flag+'.txt'))}
 iso=r/'save-isolation.json'
 if iso.exists():assert json.loads(iso.read_text(encoding='utf-8-sig'))['all_unchanged']
v['native_checks']=sum(x['count'] for x in v['checks'].values())
assert v['native_checks']==302,v['native_checks']
for case in ['lave','capital','relay','cruise','boost']:
 r=bench/f'regression-port482-v3-{case}-diagnostic-encounter-flight-capture'
 t=(r/'encounter-flight-capture.txt').read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'))
 assert b['eboot_sha256']==sha(retail) and not b['footage']
 assert 'audio buffers: starves 0 drops 0' in t
 fps=[float(n) for n in re.findall(r'orbit ([\d.]+) FPS',t)];assert len(fps)==3
 v['performance'][case]={'fps_windows':fps,'worst_frame_ms':[float(n) for n in re.findall(r'worst ([\d.]+) ms',t)],'strict_29_5_floor_pass':'RESULT 0 failures' in t,'audio_starves':0,'audio_drops':0,'port_kind':b.get('port_kind'),'report':str(r/'encounter-flight-capture.txt')}
v['memory_budget']=re.search(r'PASS retail PSP memory budget:.*',(root/'build-port482-v3.log').read_text())[0]
data=retail.read_bytes();assert data[struct.unpack('<10I',data[:40])[9]:]==(root/'assets/intro/cinematic.enm').read_bytes();v['intro_identical']=True
r=bench/'regression-port482-v3-diagnostic-port-ad-review'
v['port_report']=(r/'port-ad-review.txt').read_text()
gallery.mkdir(parents=True,exist_ok=True)
paths=sorted(r.glob('port-native-*.bmp'));frames=[Image.open(p).convert('RGBA') for p in paths];assert len(frames)==81
frames[0].save(gallery/'port-native-motion.png',save_all=True,append_images=frames[1:],duration=100,loop=0,disposal=0,blend=0)
expanded=[]
with Image.open(gallery/'port-native-motion.png') as motion:
 for k in range(motion.n_frames):
  motion.seek(k);expanded.extend([motion.convert('RGBA').tobytes()]*round(motion.info.get('duration',100)/100))
assert expanded==[f.tobytes() for f in frames];v['lossless_native_frames']=len(expanded)
for n in [0,120,150,210,240]:Image.open(r/f'port-native-{n:03d}.bmp').save(gallery/f'frame-{n:03d}.png')
shutil.copy2(root/'assets/generated/departure481/launch-start.png',gallery/'before481.png')
rows=''.join('<tr><td>'+c+'</td><td>'+', '.join(f'{n:.2f}' for n in d['fps_windows'])+'</td><td>'+', '.join(f'{n:.1f}' for n in d['worst_frame_ms'])+'</td></tr>' for c,d in v['performance'].items())
(gallery/'index.html').write_text('''<!doctype html><meta charset="utf-8"><title>Elite Next 2.5.482 — station departures</title><style>body{background:#07121b;color:#d5e3e4;font:16px system-ui;margin:24px;line-height:1.5}h1,h2{color:#f1ba72}p{max-width:94ch}main{display:flex;flex-wrap:wrap;gap:24px}img{width:480px;height:272px;image-rendering:pixelated;border:1px solid #48616d}td,th{text-align:left;padding:8px 24px 8px 0}pre{white-space:pre-wrap}</style><h1>Illustrated adverts and a more interesting station departure</h1><p>Actual 480×272 in-engine footage and stills. Six custom pixel-art adverts follow the perspective of their wall or building. Lave's first corridor has DRIFT courier and VOX radio pictures; the larger KOYO and NOVA building signs remain outside. Other stations vary the pictures.</p><img src="port-native-motion.png"><p>81 losslessly verified native frames covering 8.1 seconds, sampled every 0.1 seconds. Fixed-step renderer fixture; this animation is not a retail FPS measurement.</p><main><section><h2>Before: flat labels, 2.5.481</h2><img src="before481.png"></section><section><h2>Now: perspective wall pictures</h2><img src="frame-000.png"></section><section><h2>Building pictures and traffic, 4 seconds</h2><img src="frame-120.png"></section><section><h2>Passing the buildings, 5 seconds</h2><img src="frame-150.png"></section><section><h2>Gentle exit curve, 7 seconds</h2><img src="frame-210.png"></section><section><h2>Free-flight handoff, 8 seconds</h2><img src="frame-240.png"></section></main><p>Ordinary station departures now last eight seconds, slowing beside the adverts and gently curving only after clearing the station hull by 100 metres. Each main Lave building picture is visible for about four seconds. Foreground rails and geometry correctly cover the pictures. Crossing, approaching and departing background ships vary with the station; these are decorative traffic rather than additional persistent NPCs. The authored first Earth ascent retains its original five-second timing.</p><p>All 256 system profiles pass clearance, final speed and heading, and complete removal of staged scenery before control returns. The modern cockpit, fitted cosmetics, targeting controls and fresh-button release guard are retained. This update improves departure scenery; the player's inbound docking cinematic has not been redesigned.</p><h2>Separate final retail performance</h2><table><tr><th>Workload</th><th>Three FPS windows</th><th>Worst frame in each window, ms</th></tr>'''+rows+'''</table><p>PPSSPP PSP-3000 at native resolution, normal mixer/display, host muted. Departure workloads run the actual eight-second launch and then normal coasting; their timing windows include the free-flight period. No recording, compression or compilers ran during timing. Zero audio starvation and command drops. These averages are not a locked-framerate guarantee. Physical PSP testing and pre-existing broader smoke failures remain open.</p><h2>Native departure checks</h2><pre>'''+v['port_report']+'''</pre>''',encoding='utf-8')
out.mkdir(parents=True,exist_ok=True);shutil.copy2(retail,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(v,indent=2))
(out/'README.txt').write_text(f'''ELITE NEXT 2.5.482 — PSP-3000 LOCAL PLAYTEST

Replace EBOOT.PBP in an existing installation; keep saves and music. Fresh installation: copy ELITE_NEXT into PSP/GAME/ or open EBOOT.PBP in PPSSPP. Music is not bundled. Intro remains embedded unchanged. Save an active older session successfully before replacing its executable.

Perspective-correct illustrated station ads, larger building signs, an eight-second station departure with slower advert viewing, a safe gentle curve and varied background traffic. First story Earth ascent retains five seconds. The inbound player docking cinematic has not been redesigned. All prior cockpit, cosmetics, fitting, save handling, menu and ERL work is retained.

{v['native_checks']} focused native checks pass. See validation.json and assets/generated/port482/index.html for native footage and final retail timings. Physical PSP confirmation pending; frame-time spikes and pre-existing broader smoke failures remain open. This is a local playtest build, not a GitHub release claim.

SHA-256 {v['sha256']}
''',encoding='utf-8')
with zipfile.ZipFile(str(out)+'.zip','w',zipfile.ZIP_DEFLATED) as z:
 for p in out.iterdir():z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(str(out)+'.zip') as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==v['sha256']
print(json.dumps({'sha256':v['sha256'],'native_checks':v['native_checks'],'performance':v['performance']},indent=2))
