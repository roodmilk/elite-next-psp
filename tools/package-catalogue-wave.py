"""Package only an exact retail executable with matching per-encounter evidence."""
from pathlib import Path
import hashlib,json,re,struct,shutil,zipfile,html
root=Path(__file__).resolve().parents[1]
binary=root/'build-catalogue-final/EBOOT.PBP';data=binary.read_bytes();sha=hashlib.sha256(data).hexdigest().upper()
assert data[:4]==b'\0PBP';offsets=struct.unpack('<10I',data[:40]);assert data[offsets[9]:]==(root/'assets/intro/cinematic.enm').read_bytes()
cases=[(1,'salvage'),(15,'shipbreaker'),(3,'greenhouse'),(0,'fuelstop'),(4,'ferry'),(14,'radio'),(6,'teabarge')]
ledger=json.loads((root/'design/encounter-catalogue-progress.json').read_text());checks=[]
for entry,slug in cases:
 out=root/'assets/generated/encounter-upgrades'/slug
 binding=json.loads((out/'binding.json').read_text());assert binding['eboot_sha256']==sha
 report=(out/'verification.txt').read_text();assert 'RESULT 0 failures' in report and 'starves 0 drops 0' in report
 fps=[float(x) for x in re.findall(r'orbit ([0-9.]+) FPS',report)];assert len(fps)==3 and min(fps)>=29.5
 checks.append(dict(entry=entry,name=ledger['entries'][entry]['name'],fps=fps,report=str(out.relative_to(root)/'verification.txt'),worst_ms=max(map(float,re.findall(r'worst ([0-9.]+) ms',report)))))
boot=root.parent/'erl-implementation-candidate/outputs/magnetic-slice/regression-cat474-boot-diagnostic-normal-boot'
assert 'RESULT 0 failures' in (boot/'normal-boot.txt').read_text()
assert json.loads((boot/'binding.json').read_text(encoding='utf-8-sig'))['eboot_sha256']==sha
memory=re.findall(r'^PASS retail PSP memory budget: (.+)$',(root/'build-catalogue-final.log').read_text(encoding='utf-8',errors='replace'),re.M);assert len(memory)==1
manifest=dict(version='2.5.474',kind='PSP-3000 playtest, encounter catalogue wave 1',sha256=sha,physical_psp_verified=False,embedded_intro_unchanged=True,normal_boot_passed=True,memory=memory[0],encounters=checks,limitations=['PPSSPP orbit averages, not locked frame rate; occasional long frames remain.','Existing encounter distribution retained; capture fixtures force each catalogue entry for inspection.','No new services, missions, or interactions implied by the art replacement.','Remaining catalogue entries and natural-anomaly audit remain unfinished.'])
out=root/'deliverables/ELITE-NEXT-2.5.474-ENCOUNTER-WAVE-1';out.mkdir(exist_ok=True)
shutil.copy2(binary,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(manifest,indent=2))
(out/'README.txt').write_text('ELITE NEXT 2.5.474 - PSP-3000 encounter art playtest\n\nSeven new custom encounters: salvage village, shipbreaker yard, community greenhouse, patchwork fuel stop, old passenger ferry, local radio shack, miners tea barge.\n\nReplace EBOOT.PBP in your existing game folder. Keep your saves and music. Artwork and the existing intro film are embedded. Music is not bundled.\n\nExisting 2.5.473 menu, ERL and hyperspace source retained. This is an encounter-art playtest, not a claim of a bug-free full release. All seven native orbit checks and normal startup passed against this exact executable; physical PSP confirmation is pending. Occasional long frames remain. Existing encounter distribution is retained; the gallery uses private forced-entry captures.\n\nSHA-256: '+sha+'\n')
archive=out.with_name(out.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 for p in sorted(out.iterdir()):z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha
gallery=root/'assets/generated/encounter-upgrades/catalogue-wave-1.html'
body='<h1>Encounter catalogue — wave 1</h1><p>Actual native-resolution PSP renders. 7 new custom encounters; 14 of 75 catalogue types now have the bespoke treatment. Individual capture fixtures force entries into a test slot; normal encounter distribution is unchanged. Physical PSP testing pending.</p>'
for item,(_,slug) in zip(checks,cases):
 body+=f'<section><h2>{html.escape(item["name"])}</h2><a href="{slug}/index.html"><img width="480" height="272" src="{slug}/encounter-retail-30.png"></a><p>Near / medium / distant: {item["fps"]} FPS averaged in PPSSPP. <a href="{slug}/index.html">All captured angles</a></p></section>'
gallery.write_text('<!doctype html><meta charset="utf-8"><title>ELITE NEXT native encounter gallery</title><style>body{background:#091018;color:#d8e6ed;font:16px system-ui;max-width:1000px;margin:32px auto}section{margin:32px 0}img{image-rendering:pixelated}a{color:#72d4dd}</style>'+body,encoding='utf-8')
print(json.dumps(dict(eboot=str(out/'EBOOT.PBP'),zip=str(archive),sha256=sha,gallery=str(gallery)),indent=2))
