from pathlib import Path
import json,hashlib,re,struct,shutil,zipfile,html
root=Path(__file__).resolve().parents[1];binary=root/'build-working-retail/EBOOT.PBP';data=binary.read_bytes();sha=hashlib.sha256(data).hexdigest().upper()
offsets=struct.unpack('<10I',data[:40]);assert data[:4]==b'\0PBP' and data[offsets[9]:]==(root/'assets/intro/cinematic.enm').read_bytes()
with zipfile.ZipFile(root/'deliverables/ELITE-NEXT-2.5.474-source-evidence.zip') as z:
 old=z.read('src/encounter-custom.h').decode().replace('\r\n','\n');new=(root/'src/encounter-custom.h').read_text()
 begin=new.index('/* Shared ore equipment');end=new.index('static int encounter_custom(',begin)
 stripped=new[:begin]+new[end:]
 for case in ('\n if(entry==8){encounter_ore_depot(c,yaw);return 1;}','\n if(entry==9){encounter_parts_exchange(c,yaw);return 1;}','\n if(entry==10){encounter_beacon_crew(c,yaw);return 1;}'):stripped=stripped.replace(case,'')
 assert stripped==old
 assert [s.rstrip(',') for s in (root/'src/encounter-art-tiles.h').read_text().splitlines()[:60]]==[s.rstrip(',') for s in z.read('src/encounter-art-tiles.h').decode().splitlines()]
 for name in ('src/main.c','src/space-discoveries.h'):
  assert (root/name).read_bytes()==z.read(name)
checks=[]
for entry,slug in [(8,'ore-depot475'),(9,'parts-exchange475'),(10,'beacon-crew475'),(1,'salvage475-baseline')]:
 p=root/'assets/generated/encounter-upgrades'/slug;b=json.loads((p/'binding.json').read_text());assert b['eboot_sha256']==sha
 report=(p/'verification.txt').read_text();assert 'RESULT 0 failures' in report and 'starves 0 drops 0' in report
 fps=[float(x) for x in re.findall(r'orbit ([0-9.]+) FPS',report)];assert len(fps)==3 and min(fps)>=29.5
 checks.append(dict(entry=entry,slug=slug,fps=fps,worst_ms=max(map(float,re.findall(r'worst ([0-9.]+) ms',report)))))
boot=root.parent/'erl-implementation-candidate/outputs/magnetic-slice/regression-cat475-boot-diagnostic-normal-boot'
assert 'RESULT 0 failures' in (boot/'normal-boot.txt').read_text() and json.loads((boot/'binding.json').read_text(encoding='utf-8-sig'))['eboot_sha256']==sha
memory=re.findall(r'^PASS retail PSP memory budget: (.+)$',(root/'build-working-retail.log').read_text(encoding='utf-8',errors='replace'),re.M);assert len(memory)==1
manifest=dict(version='2.5.475',sha256=sha,kind='PSP-3000 encounter-art playtest',new_entries=[8,9,10],exact_binary_orbit_checks=checks,prior_seven_geometry_and_materials_unchanged=True,embedded_intro_unchanged=True,normal_startup_passed=True,memory=memory[0],physical_psp_verified=False,limitations=['Orbit averages in PPSSPP; occasional long frames remain.','No new placement, interactions, services or save-format changes.','Previous wave exact-retail evidence remains in474 validation; this build additionally rechecks salvage village.','17 of75 catalogue entries bespoke;58 remain. Natural-anomaly audit is separate and pending.'])
out=root/'deliverables/ELITE-NEXT-2.5.475-ENCOUNTER-WAVE-2';out.mkdir(exist_ok=True);shutil.copy2(binary,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(manifest,indent=2))
(out/'README.txt').write_text('ELITE NEXT2.5.475 - PSP-3000 encounter-art playtest\n\nIncludes all previous474 upgrades plus the Cooperative Ore Depot, Parts Exchange and Beacon Repair Crew. Replace EBOOT.PBP in your existing game folder; retain saves and music. Music is not bundled. Intro and artwork are embedded.\n\nNative orbit tests and startup passed; physical PSP confirmation pending. This is a playtest, not a bug-free or locked-frame-rate claim. Existing encounter distribution and interactions are retained.\n\nSHA-256: '+sha+'\n')
archive=out.with_name(out.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 for p in sorted(out.iterdir()):z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(archive) as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha
ledger=json.loads((root/'design/encounter-catalogue-progress.json').read_text());ledger['latest_playtest']=dict(version='2.5.475',sha256=sha,native_verified_new_entries=[8,9,10],baseline_entry=1,remaining_bespoke_entries=58);(root/'design/encounter-catalogue-progress.json').write_text(json.dumps(ledger,indent=2))
gallery=root/'assets/generated/encounter-upgrades/catalogue-progress.html'
slugs=['salvage','shipbreaker','greenhouse','fuelstop','ferry','radio','teabarge','ore-depot475','parts-exchange475','beacon-crew475'];ids=[1,15,3,0,4,14,6,8,9,10]
body='<h1>Ten new encounter upgrades</h1><p>Actual480×272 in-game captures. 17/75 catalogue types now bespoke;58 still pending. Images use a private forced-entry fixture; existing encounter distribution is unchanged. Physical PSP testing pending.</p>'
for i,slug in zip(ids,slugs):
 name=html.escape(ledger['entries'][i]['name']);body+=f'<section><h2>{name}</h2><a href="{slug}/index.html"><img width="480" height="272" src="{slug}/encounter-retail-30.png"></a><p><a href="{slug}/index.html">All native angles and measured performance</a></p></section>'
gallery.write_text('<!doctype html><meta charset="utf-8"><title>ELITE NEXT native encounter progress</title><style>body{background:#091018;color:#d8e6ed;font:16px system-ui;max-width:1000px;margin:32px auto}section{margin:32px 0}img{image-rendering:pixelated}a{color:#72d4dd}</style>'+body,encoding='utf-8')
print(json.dumps(dict(eboot=str(out/'EBOOT.PBP'),zip=str(archive),sha256=sha,gallery=str(gallery)),indent=2))
