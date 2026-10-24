"""Bind all20 approved cosmetics, native captures and retail metrics to the delivered PBP."""
from pathlib import Path
from PIL import Image
import hashlib,json,re,shutil,struct,zipfile,csv,html
root=Path(__file__).resolve().parents[1];bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
retail=root/'build-cosmetics479-v5/EBOOT.PBP';review=root/'build-cosmetics479-review-v6/EBOOT.PBP';motion_review=root/'build-cosmetics479-review-v5/EBOOT.PBP'
frozen=json.loads((root/'design/charms479/candidate-v6-hashes.json').read_text())
assert all(sha(root/p).lower()==h for p,h in frozen.items())
data=retail.read_bytes();assert data[:4]==b'\0PBP' and data[struct.unpack('<10I',data[:40])[9]:]==(root/'assets/intro/cinematic.enm').read_bytes()
manifest=dict(version='2.5.479',profile='PSP-3000 local cosmetics playtest',sha256=sha(retail),prices='Proposed25U each for all20 items; cosmetics are not a rarity or power tier.',save_stream='Existing V38 format,115-byte historical spare stream; new ownership uses existing DockRoll records. Old saves tested.',physical_psp_verified=False,asset_rgba_bytes=65536,source_hashes=frozen,checks={},performance=[],strict_fps_shortfalls=[],limitations=['Native automated input fixtures are not a human career playthrough.','Motion capture uses fixed1/30s game steps in the real native engine, not a retail FPS measurement.','Physical PSP testing and the pre-existing broad legacy smoke failures remain open.','No mirror-sticker equipment modules were found in this source; existing bezel stickers are retained and no global cosmetic-family cap was added.'])
def run(slug,flag):return bench/f'regression-cos479-v6-{slug}-diagnostic-{flag}'
for slug,flag,binary in [('dashboard','dashboard-review',review),('charms','charm-review',motion_review),('hud','cockpit-hud-review',review),('menu','final-menu',review),('erl','erl-quick-race-review',review)]:
 p=run(slug,flag);text=(p/(flag+'.txt')).read_text(encoding='utf-8');b=json.loads((p/'binding.json').read_text(encoding='utf-8-sig'))
 assert 'RESULT 0 failures' in text and b['eboot_sha256']==sha(binary)
 if (p/'save-isolation.json').exists():assert json.loads((p/'save-isolation.json').read_text(encoding='utf-8-sig'))['all_unchanged']
 manifest['checks'][slug]=dict(report=text,binding=b)
# The sole v5->v6 change is a diagnostic assertion; all production files are identical.
manifest['review_source_equivalence']=json.loads((root/'design/charms479/REVIEW-ONLY-DIFFERENCE.json').read_text())
for slug in ['baseline','ufo','alien','burger','duck','dashboard-worst','both','station','music','computers']:
 p=run(slug,'encounter-flight-capture');t=(p/'encounter-flight-capture.txt').read_text();b=json.loads((p/'binding.json').read_text(encoding='utf-8-sig'))
 assert b['eboot_sha256']==sha(retail) and b['internal_resolution']==1 and 'starves 0 drops 0' in t
 assert all('orbit' in x for x in t.splitlines() if x.startswith('FAIL '))
 fps=list(map(float,re.findall(r'orbit ([0-9.]+) FPS',t)));assert len(fps)==3
 c=re.search(r'CHARM render calls (\d+) mean ([0-9.]+) ms',t);d=re.search(r'DASHBOARD render calls (\d+) mean ([0-9.]+) ms',t);assert c and d
 if b['charm']:assert int(c[1])>0
 if b['dashboard'] and slug!='computers':assert int(d[1])>0
 if slug=='computers':assert int(d[1])==0
 log=(p/'emulator.log').read_text(errors='replace');assert 'Bad memory access' not in log and 'MEMBLOCK_ALLOC_FAILED' not in log
 entry=dict(scene=slug,fps=fps,charm_mean_ms=float(c[2]),dashboard_mean_ms=float(d[2]),binding=b,report=t);manifest['performance'].append(entry)
 for i,f in enumerate(fps):
  if f<29.5:manifest['strict_fps_shortfalls'].append(dict(scene=slug,window=i+1,fps=f))
music=(run('music','encounter-flight-capture')/'audio-flight-performance.txt').read_text();assert 'source 7;' in music and 'decoder error 0' in music and 'buffer starves 0' in music;manifest['music_report']=music
p=run('boot','normal-boot');assert 'RESULT 0 failures' in (p/'normal-boot.txt').read_text() and json.loads((p/'binding.json').read_text(encoding='utf-8-sig'))['eboot_sha256']==sha(retail)
manifest['memory']=re.search(r'^PASS retail PSP memory budget: (.+)$',(root/'build-cosmetics479-v5.log').read_text(errors='replace'),re.M)[1]
gallery=root/'assets/generated/cosmetics479';gallery.mkdir(exist_ok=True)
style='<style>body{background:#09121d;color:#e0def0;font:16px system-ui;max-width:1020px;margin:24px auto;padding:0 16px}a{color:#cfa6f4}img{image-rendering:pixelated}figure{margin:12px 0}section{margin:30px 0}.pair{display:flex;gap:16px;overflow-x:auto}.pair figure{flex:0 0 480px}td,th{padding:8px;text-align:left;border-bottom:1px solid #3e354e}</style>'
head='<!doctype html><meta charset="utf-8"><title>ELITE NEXT native cosmetics</title>'+style
motion_html=head+'<h1>Hanging charms — native in-engine motion</h1><p>Left: approved offline reference. Right: actual480×272 native renderer with real input/game physics at1/30s simulation steps. Idle, turns, boost, braking, actual staged ship collision, recovery, then expanded targeting. Performance is measured separately in the retail executable.</p>'
charm=run('charms','charm-review');proof=[]
for i,name in enumerate(['ufo','alien','burger','duck']):
 rows=list(csv.reader((charm/f'charm-{i}-motion.csv').open()));assert len(rows)==240
 frames=[]
 for frame in range(240):
  raw=(charm/f'charm-{i}-frame-{frame:03}.rgba').read_bytes();assert len(raw)==512*272*4
  frames.append(Image.frombytes('RGBA',(512,272),raw).crop((0,0,480,272)).convert('RGB'))
 durations=[round((n+1)*1000/30)-round(n*1000/30) for n in range(240)]
 frames[0].save(gallery/(name+'-native.apng'),format='PNG',save_all=True,append_images=frames[1:],duration=durations,loop=0)
 frames[0].save(gallery/(name+'-native.png'))
 assert float(re.search(r'IMPACT '+str(i)+r' shield_loss ([0-9.]+)',manifest['checks']['charms']['report'])[1])>0
 assert float(rows[59][3])<0 and float(rows[89][3])>0 and float(rows[119][1])>float(rows[29][1]) and float(rows[149][1])<float(rows[119][1]) and rows[200][7]=='1'
 reference=root/'design/charm479-reference'/(name+'-approved.gif');shutil.copy2(reference,gallery/(name+'-approved.gif'))
 motion_html+=f'<section><h2>{html.escape(name.title())}</h2><div class="pair"><figure><figcaption>Approved reference</figcaption><img width="480" height="272" src="{name}-approved.gif"></figure><figure><figcaption>Native in-engine</figcaption><img width="480" height="272" src="{name}-native.apng"></figure></div></section>'
 proof.append(dict(item=name,frames=240,simulation_seconds=8,actual_collision_shield_loss=float(re.search(r'IMPACT '+str(i)+r' shield_loss ([0-9.]+)',manifest['checks']['charms']['report'])[1])))
manifest['motion_proofs']=proof;(gallery/'motion.html').write_text(motion_html,encoding='utf-8')
dash=run('dashboard','dashboard-review');items=json.loads((root/'design/dashboard479-reference/manifest.json').read_text())['items'];cards=[]
for i,item in enumerate(items):
 for kind in ['native','shop']:Image.open(dash/f'dashboard-{i:02}-{kind}.bmp').convert('RGB').save(gallery/f'dashboard-{i:02}-{kind}.png')
 cards.append(f'<section><h2>{html.escape(item["name"])}</h2><img width="480" height="272" src="dashboard-{i:02}-native.png"> <a href="dashboard-{i:02}-shop.png">Native shop view</a></section>')
for name,folder in [('dashboard-fitted',dash),('dashboard-storage',dash),('dashboard-expanded',dash),('caption-close-fixed',dash),('charm-fitted',charm),('charm-storage',charm)]:Image.open(folder/(name+'.bmp')).convert('RGB').save(gallery/(name+'.png'))
rows=''.join('<tr><td>'+e['scene']+'</td><td>'+' / '.join(f'{f:.2f}' for f in e['fps'])+'</td><td>'+f'{e["charm_mean_ms"]:.4f} / {e["dashboard_mean_ms"]:.4f}'+'</td></tr>' for e in manifest['performance'])
body='<h1>ELITE NEXT2.5.479 —20 cockpit cosmetics</h1><p>Actual480×272 game renders. Supplied artwork retained directly, with original transparency. Proposed25U each at Lave. Hanging charms use Utility; dashboard items use Hold/Cabin. One of each family per ship; purchases enter Item Storage. No ship-stat bonuses or rarity tiers.</p><p><a href="motion.html">Watch all four hanging charms beside their approved references</a></p><h2>Caption fix and fitting</h2>'
for name in ['caption-close-fixed','dashboard-fitted','dashboard-storage','charm-fitted','dashboard-expanded']:body+=f'<figure><img width="480" height="272" src="{name}.png"><figcaption>{name}</figcaption></figure>'
body+='<h2>Retail performance — three sample windows</h2><p>Native1× PPSSPP PSP-3000 profile, normal mixer/display, host muted. Capture overhead excluded. Physical PSP confirmation pending. The dashboard-worst case uses Planet Globe, the greatest supplied per-pixel alpha workload.</p><table><tr><th>Scene</th><th>FPS</th><th>Charm / dashboard draw ms</th></tr>'+rows+'</table><p>Strict-floor shortfalls: '+html.escape(str(manifest['strict_fps_shortfalls']))+'</p><p><a href="validation.json">Full executable-bound results and limitations</a></p>'+''.join(cards)
(gallery/'index.html').write_text(head+body,encoding='utf-8');(gallery/'validation.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
out=root/'deliverables/ELITE-NEXT-2.5.479-COSMETICS';out.mkdir(exist_ok=True);shutil.copy2(retail,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
(out/'README.txt').write_text('ELITE NEXT2.5.479 - PSP-3000 COSMETICS PLAYTEST\n\nExisting installation: replace EBOOT.PBP only. Fresh installation: copy ELITE_NEXT from the ZIP to PSP/GAME/ or open its EBOOT.PBP in PPSSPP. Keep existing saves/music. Music not bundled. Intro remains embedded unchanged.\n\nLave Outfitting stocks all4 hanging charms and16 dashboard cosmetics. Proposed25U each. X buys into Item Storage without auto-installing. In storage, X selects an item; choose a compatible bank and X fits. Ship Loadout Square returns it to storage.\n\nHanging charms: existing Utility slots. Dashboard: existing Hold/Cabin slots. One installed item per family, independently; remove the current item before fitting another in that family. Multiple owned copies allowed. Existing storage, passenger and cargo-capacity rules remain. No stat bonuses. Dashboard artwork is static; hanging motion responds to actual movement/braking/damage.\n\nComms button moved10px right to clear the approved shelf. CLOSE/Triangle now has a padded header, retaining three message rows. Existing controls retained.\n\nNative automated checks pass; see validation.json for exact-retail FPS, any strict-floor shortfalls and broader existing test limitations. Physical PSP testing pending. Prior478 preserved.\n\nSHA-256: '+sha(retail)+'\n',encoding='utf-8')
archive=out.with_name(out.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 for p in sorted(out.iterdir()):z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(archive) as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha(retail)
print(json.dumps(dict(sha256=sha(retail),checks={k:re.findall(r'CHECKS (\d+)',v['report']) for k,v in manifest['checks'].items()},strict_fps_shortfalls=manifest['strict_fps_shortfalls'],memory=manifest['memory'],package=str(archive)),indent=2))
