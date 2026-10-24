from pathlib import Path
import hashlib,json,re,shutil,struct,zipfile,html
from PIL import Image
root=Path(__file__).resolve().parents[1];bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice';gallery=root/'assets/generated/loadout480';out=root/'deliverables/ELITE-NEXT-2.5.480-LOADOUT-HUD';retail=root/'build-loadout480-v4/EBOOT.PBP';review=root/'build-loadout480-review-v4/EBOOT.PBP'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert retail.exists() and review.exists()
hashes=json.loads((root/'design/loadout480/final-source-hashes.json').read_text());assert all(sha(root/f).lower()==h for f,h in hashes.items())
validation={'version':'2.5.480','profile':'PSP-3000 local playtest','sha256':sha(retail),'source_hashes':hashes,'checks':{},'performance':{},'physical_psp_verified':False,'git_synced':False,'limitations':['PPSSPP measurements are not physical PSP measurements.','Frame-time spikes remain; averages are not a locked framerate guarantee.','Pre-existing broader legacy smoke failures remain open.','The native animation preview uses scripted game state and fixed 1/30s steps; it is not the FPS measurement.','No supplied cosmetic sprites or Twin Stack artwork were simplified. Music is not bundled.']}
flags=['loadout-picker-review','cockpit-hud-review','dashboard-review','charm-review','final-menu','erl-quick-race-review']
for flag in flags:
 r=bench/f'regression-load480-v4-diagnostic-{flag}';t=(r/(flag+'.txt')).read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'));assert 'RESULT 0 failures' in t and b['eboot_sha256']==sha(review)
 n=re.search(r'CHECKS (\d+)',t);validation['checks'][flag]={'count':int(n[1]) if n else len(re.findall(r'^PASS ',t,re.M)),'report':str(r/(flag+'.txt'))}
 iso=r/'save-isolation.json'
 if iso.exists():assert json.loads(iso.read_text(encoding='utf-8-sig'))['all_unchanged']
for case in ['cruise','boost','station','computers']:
 r=bench/f'regression-load480-v4-{case}-diagnostic-encounter-flight-capture';t=(r/'encounter-flight-capture.txt').read_text();b=json.loads((r/'binding.json').read_text(encoding='utf-8-sig'));assert b['eboot_sha256']==sha(retail) and not b['footage'];assert 'audio buffers: starves 0 drops 0' in t
 fps=[float(x) for x in re.findall(r'orbit ([\d.]+) FPS',t)];assert len(fps)==3
 boost=re.search(r'real boost frames (\d+) peak speed ([\d.]+)',t)
 validation['performance'][case]={'fps_windows':fps,'worst_frame_ms':[float(x) for x in re.findall(r'worst ([\d.]+) ms',t)],'strict_29_5_floor_pass':'RESULT 0 failures' in t,'boost_frames':int(boost[1]),'peak_speed':float(boost[2]),'audio_starves':0,'audio_drops':0,'report':str(r/'encounter-flight-capture.txt')}
assert validation['performance']['boost']['boost_frames']>500 and validation['performance']['cruise']['boost_frames']==0
validation['native_checks']=sum(v['count'] for v in validation['checks'].values())
validation['memory_budget']=re.search(r'PASS retail PSP memory budget:.*',(root/'build-loadout480-v4.log').read_text())[0]
data=retail.read_bytes();assert data[:4]==b'\0PBP' and data[struct.unpack('<10I',data[:40])[9]:]==(root/'assets/intro/cinematic.enm').read_bytes();validation['intro_identical']=True
r=bench/'regression-load480-v4-diagnostic-loadout-picker-review';gallery.mkdir(parents=True,exist_ok=True)
for name in ['picker-weapons','picker-empty','picker-cosmetic-limit']:Image.open(r/(name+'.bmp')).save(gallery/(name+'.png'))
frames=[Image.open(p).convert('RGBA') for p in sorted(r.glob('hud-motion-*.bmp'))];assert len(frames)==90
frames[0].save(gallery/'hud-motion.png',save_all=True,append_images=frames[1:],duration=[33,33,34]*30,loop=0,disposal=0,blend=0)
with Image.open(gallery/'hud-motion.png') as animation:
 assert animation.n_frames==90
 for i,frame in enumerate(frames):animation.seek(i);assert animation.convert('RGBA').tobytes()==frame.tobytes()
validation['native_preview_lossless_frames']=90
ref=root.parent/'side-prototypes/flight-ui/reference-cockpit-revision/cockpit-walkthrough.gif';shutil.copy2(ref,gallery/'approved-reference.gif')
rows=''.join(f'<tr><td>{c}</td><td>'+', '.join(f'{n:.2f}' for n in d['fps_windows'])+'</td></tr>' for c,d in validation['performance'].items())
(gallery/'index.html').write_text('''<!doctype html><meta charset="utf-8"><title>ELITE NEXT 2.5.480 — loadout and cockpit</title><style>body{background:#07121b;color:#d3e4e4;font:16px system-ui;margin:24px}h1,h2{color:#f3ba6b}main{display:flex;flex-wrap:wrap;gap:24px}section{max-width:600px}img{width:480px;image-rendering:pixelated;border:1px solid #50686d}p{max-width:90ch;line-height:1.5}td,th{text-align:left;padding:8px 20px 8px 0}a{color:#7fd8d1}</style><h1>ELITE NEXT 2.5.480</h1><p>Actual native 480×272 in-engine output. X over a Ship Loadout slot opens compatible Item Storage on the right. Up/Down chooses an item; X fits into that exact bank; Circle cancels. Triangle still opens full storage; Square still removes the fitted item.</p><main><section><h2>Weapons picker</h2><img src="picker-weapons.png"><h2>Compatible inventory empty</h2><img src="picker-empty.png"><h2>Existing cosmetic rule retained</h2><img src="picker-cosmetic-limit.png"></section><section><h2>In-engine animation</h2><img src="hud-motion.png"><p>Lossless 90-frame native capture at fixed 1/30s steps. Scripted speed rise/fall and held-Square computer opening/release demonstrate actual renderer output. No FPS claim from this recorded clip.</p><h2>Supplied reference</h2><img src="approved-reference.gif"><p>Original side-chat concept animation with illustrative values. The game uses real commander, equipment, targets and movement.</p></section></main><h2>Separate retail measurements</h2><table><tr><th>Case</th><th>Three FPS windows</th></tr>'''+rows+'''</table><p>PPSSPP PSP-3000, native resolution, real mixer, host muted; zero audio starvation and command drops in all cases. Boost is activated through the actual double-R input, not by setting its speed. Its later windows move into open space. Station/expanded-computer cases are staged orbit workloads. Physical PSP testing and occasional frame-time spikes remain open.</p><p>Save fix: unsupported free-flight equipment checkpoints defer rather than repeatedly reporting a false failure; they retry at the docked command deck. Genuine failures preserve their cause and prior valid saves. Existing save validation remains enabled.</p>''',encoding='utf-8')
out.mkdir(parents=True,exist_ok=True);shutil.copy2(retail,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(validation,indent=2))
readme=f'''ELITE NEXT 2.5.480 — PSP-3000 LOCAL PLAYTEST

Existing installation: replace EBOOT.PBP only. Keep your saves and music. Fresh: put ELITE_NEXT in PSP/GAME/ or open EBOOT.PBP in PPSSPP. Music is not included; the existing intro is embedded unchanged.

SHIP LOADOUT: select a slot, X opens compatible stored items on the right. Up/Down selects, X fits into that exact bank, Circle cancels. Occupied-slot equipment returns to storage. Triangle opens all Item Storage; Square removes the fitted item. Existing cargo/passenger/cosmetic restrictions remain.

TARGETING: hold Square to open; release to close. Browse using D-pad; X confirms. Circle weapon tools can remain held independently. Flight continues.

HUD: native rolling speed digits update every rendered frame, independently of the cached cockpit. Acquisition, reveal highlights, tool confirmation rings, receiver activity and local warning pulses animate without delaying input. Artwork retained.

SAVING: unsupported free-flight equipment checkpoints wait for a valid docked command-deck save rather than repeatedly showing a false failure. Real errors retain their reason and keep earlier valid saves. Do not discard an unsaved old session before saving it at a station.

{validation['native_checks']} focused native checks pass. See validation.json for separate exact-retail performance, frame-time limits and physical-hardware caveats. Prior 2.5.479 package preserved. Local build only; no GitHub release claim.

SHA-256 {sha(retail)}
'''
(out/'README.txt').write_text(readme,encoding='utf-8')
with zipfile.ZipFile(str(out)+'.zip','w',zipfile.ZIP_DEFLATED) as z:
 for p in out.iterdir():z.write(p,'ELITE_NEXT/'+p.name)
print(json.dumps({'checks':validation['native_checks'],'sha256':validation['sha256'],'performance':validation['performance']},indent=2))
