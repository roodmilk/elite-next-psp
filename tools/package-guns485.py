from pathlib import Path
from PIL import Image
import hashlib,json,re,shutil,zipfile,struct
root=Path(__file__).resolve().parents[1];bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice'
out=root/'deliverables/ELITE-NEXT-2.5.485-SOLID-TWIN-GUNS';gallery=root/'assets/generated/guns485'
retail=root/'build-guns485/EBOOT.PBP';review=root/'build-guns485-review/EBOOT.PBP'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
hashes=json.loads((root/'design/guns485/final-source-hashes.json').read_text());assert all(sha(root/f).lower()==h for f,h in hashes.items())
v={'version':'2.5.485','sha256':sha(retail),'source_hashes':hashes,'checks':{},'performance':{},'physical_psp_verified':False,'git_synced':False}
for flag in ['port-ad-review','departure-hud-review','cockpit-hud-review','loadout-picker-review','dashboard-review','charm-review','final-menu','erl-quick-race-review']:
 r=bench/f'regression-guns485-diagnostic-{flag}';t=(r/(flag+'.txt')).read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'))
 assert 'RESULT 0 failures' in t and b['eboot_sha256']==sha(review),flag
 m=re.search(r'CHECKS (\d+)',t);v['checks'][flag]={'count':int(m[1]) if m else len(re.findall(r'^PASS ',t,re.M)),'report':str(r/(flag+'.txt'))}
 iso=r/'save-isolation.json'
 if iso.exists():assert json.loads(iso.read_text(encoding='utf-8-sig'))['all_unchanged']
v['accepted_performance_target']='User accepts approximately 29 FPS; preserve visual detail and investigate noticeable frame-time spikes. Physical PSP confirmation remains pending.'
v['native_checks']=sum(x['count'] for x in v['checks'].values());assert v['native_checks']==308
for case in ['hit','switch','busy','station','thirdperson']:
 r=bench/f'regression-guns485-{case}-diagnostic-encounter-flight-capture';t=(r/'encounter-flight-capture.txt').read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'))
 assert b['eboot_sha256']==sha(retail) and b['cockpit_ship']==6 and not b['footage']
 assert 'audio buffers: starves 0 drops 0' in t
 fps=[float(x) for x in re.findall(r'orbit ([\d.]+) FPS',t)];assert len(fps)==3
 v['performance'][case]={'fps_windows':fps,'worst_frame_ms':[float(x) for x in re.findall(r'worst ([\d.]+) ms',t)],'strict_29_5_floor_pass':'RESULT 0 failures' in t,'audio_starves':0,'audio_drops':0,'report':str(r/'encounter-flight-capture.txt')}
v['performance_limitations']=[c+' misses the strict 29.5 FPS floor: '+', '.join(str(x) for x in d['fps_windows'])+' FPS' for c,d in v['performance'].items() if not d['strict_29_5_floor_pass']]
v['memory_budget']=re.search(r'PASS retail PSP memory budget:.*',(root/'build-guns485.log').read_text())[0]
data=retail.read_bytes();assert data[struct.unpack('<10I',data[:40])[9]:]==(root/'assets/intro/cinematic.enm').read_bytes();v['intro_identical']=True
gallery.mkdir(parents=True,exist_ok=True);r=bench/'regression-guns485-diagnostic-cockpit-hud-review'
for source,name in [('anaconda-idle.bmp','idle.png'),('anaconda-firing.bmp','firing.png')]:Image.open(r/source).save(gallery/name)
shutil.copy2(root/'design/guns485/anaconda-before.png',gallery/'before.png')
ref=root.parent/'side-prototypes/flight-ui/STUDIO-LEAD-TWIN-STACK'
shutil.copy2(ref/'APPROVED-G-IDLE.png',gallery/'approved-idle.png');shutil.copy2(ref/'APPROVED-G-TWIN-STACK.gif',gallery/'approved.gif')
foot=bench/'regression-guns485-footage-diagnostic-encounter-flight-capture';binding=json.loads((foot/'binding.json').read_text(encoding='utf-8-sig'))
assert binding['eboot_sha256']==sha(retail) and binding['cockpit_ship']==6 and binding['footage']
rows=[line.split(',') for line in (foot/'twin-footage.csv').read_text().splitlines()];files=sorted(foot.glob('twin-frame-*.rgba'));assert len(files)==len(rows)==80
frames=[Image.frombytes('RGBA',(512,272),p.read_bytes()).crop((0,0,480,272)) for p in files]
durations=[max(10,round(float(row[1])*100)*10) for row in rows]
frames[0].save(gallery/'anaconda-motion.png',save_all=True,append_images=frames[1:],duration=durations,loop=0,disposal=0,blend=0)
expected=[]
for f,d in zip(frames,durations):expected.extend([f.tobytes()]*(d//10))
actual=[]
with Image.open(gallery/'anaconda-motion.png') as motion:
 for i in range(motion.n_frames):motion.seek(i);actual.extend([motion.convert('RGBA').tobytes()]*(round(motion.info['duration'])//10))
assert actual==expected;v['native_footage_frames']=80;v['footage_cadence_ms']=durations
perf=''.join('<tr><td>'+c+'</td><td>'+', '.join(f'{x:.2f}' for x in d['fps_windows'])+'</td></tr>' for c,d in v['performance'].items())
(gallery/'index.html').write_text('''<!doctype html><meta charset="utf-8"><title>Elite Next 2.5.485 — Anaconda guns</title><style>body{background:#07121b;color:#d5e3e4;font:16px system-ui;margin:24px;line-height:1.5}h1,h2{color:#f1ba72}p{max-width:94ch}main{display:flex;gap:20px;flex-wrap:wrap}img{width:480px;height:272px;image-rendering:pixelated;border:1px solid #48616d}td,th{text-align:left;padding:8px 24px 8px 0}</style><h1>Solid 3D twin barrels at the lower corners</h1><p>Each side now has two separate eight-sided solid barrels, raised sleeve collars, closed 3D breeches and a visible dark gap. Facet shading, side vents, bolts and cyan accents give the model volume. The broad receiver sheets are removed. Length and lower-corner placement are preserved; shots alternate between the actual barrel centres. Four weapon slots remain four fitted weapons selected with Circle + Right; weapon damage, cadence and ammunition rules are unchanged.</p><main><section><h2>Before — actual 2.5.484 Anaconda</h2><img src="before.png"></section><section><h2>Solid twin barrels — actual in-engine idle</h2><img src="idle.png"></section><section><h2>Approved composition reference</h2><img src="approved-idle.png"></section><section><h2>Corrected — real discharge</h2><img src="firing.png"></section><section><h2>Approved motion reference</h2><img src="approved.gif"></section><section><h2>Actual native retail firing footage</h2><img src="anaconda-motion.png"></section></main><p>All images are 480×272. The approved prototype is a visual reference, not game footage. Native motion contains 80 losslessly verified real-renderer frames; recorded frame timing is retained to the nearest 10ms. Capture adds file-writing overhead, so performance is measured separately without recording. The test uses real firing input and protected health/heat to sustain the workload.</p><h2>Separate Anaconda retail performance</h2><table><tr><th>Workload</th><th>Three FPS windows</th></tr>'''+perf+'''</table><p>PPSSPP PSP-3000, native resolution, real mixer/display, host muted, private saves, no concurrent compiler or recording. Any workloads that miss the strict 29.5 FPS floor remain explicitly flagged in validation.json. Per-window worst frames and floor results are in validation.json. Physical PSP confirmation and existing frame-time spikes remain open. Shipyard purchase, all four fitted weapon identities and immediate switch/wrap are checked along with the existing cockpit, save, fitting, menu, station departure and ERL checks.</p>''',encoding='utf-8')
out.mkdir(parents=True,exist_ok=True);shutil.copy2(retail,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(v,indent=2))
(out/'README.txt').write_text(f'''ELITE NEXT 2.5.485 — PSP-3000 LOCAL PLAYTEST

Rebuilds the short corner-mounted Twin Stack as solid 3D twin barrels with separate closed breeches, sleeve collars and shaded side faces. Firing remains attached to the same far-tip geometry. Four Anaconda weapon slots are checked individually, including Circle+Right switching and wrap. Existing 2.5.484 station adverts, departures, HUD, cosmetics, saves, menus and ERL work are retained.

Replace EBOOT.PBP in your existing PSP/GAME/ELITE_NEXT installation; keep your saves and music. The ZIP contains ELITE_NEXT. Music is not bundled; intro is embedded unchanged. {v['native_checks']} focused native checks pass. Native footage and performance evidence: assets/generated/guns485/index.html and validation.json. See the recorded strict 29.5 FPS floor results; this build does not claim a locked framerate. Physical PSP testing and existing frame-time spikes remain open. Local playtest, not a GitHub release claim.

SHA-256 {v['sha256']}
''',encoding='utf-8')
with zipfile.ZipFile(str(out)+'.zip','w',zipfile.ZIP_DEFLATED) as z:
 for p in out.iterdir():z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(str(out)+'.zip') as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==v['sha256']
print(json.dumps({'sha256':v['sha256'],'checks':v['native_checks'],'performance':v['performance']},indent=2))
