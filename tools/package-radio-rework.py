"""Package the revised radio artwork with exact executable/evidence bindings."""
from pathlib import Path
import hashlib,json,re,struct,shutil,zipfile
root=Path(__file__).resolve().parents[1];binary=root/'build-radio-rework-v3/EBOOT.PBP';data=binary.read_bytes();sha=hashlib.sha256(data).hexdigest().upper()
assert data[:4]==b'\0PBP';offsets=struct.unpack('<10I',data[:40]);assert data[offsets[9]:]==(root/'assets/intro/cinematic.enm').read_bytes()
evidence=root.parent/'erl-implementation-candidate/outputs/magnetic-slice';checks=[];shortfalls=[]
for tag,name,radio in [('radio476-detailed-v3','radio shack',True),('radio476-v3-relay','outer relay',False),('radio476-v3-carrier','animal carrier',False),('radio476-v3-mechanic','mechanic',False),('radio476-v3-salvage','salvage village',False)]:
 p=evidence/f'regression-{tag}-diagnostic-encounter-flight-capture';b=json.loads((p/'binding.json').read_text(encoding='utf-8-sig'));assert b['eboot_sha256']==sha
 report=(p/'encounter-flight-capture.txt').read_text();assert 'starves 0 drops 0' in report
 assert all('orbit' in line for line in report.splitlines() if line.startswith('FAIL '))
 if radio:assert 'RESULT 0 failures' in report
 fps=[float(x) for x in re.findall(r'orbit ([0-9.]+) FPS',report)];assert len(fps)==3
 for band,value in zip(['near','medium','distant'],fps):
  if value<29.5:shortfalls.append(dict(scene=name,band=band,fps=value,strict_floor=29.5))
 log=(p/'emulator.log').read_text(encoding='utf-8',errors='replace');assert 'Bad memory access' not in log and 'Stopping emulation' not in log
 checks.append(dict(name=name,fps=fps,report=report,binding=b,worst_ms=max(map(float,re.findall(r'worst ([0-9.]+) ms',report)))))
boot=evidence/'regression-radio476-v3-boot-diagnostic-normal-boot';assert 'RESULT 0 failures' in (boot/'normal-boot.txt').read_text()
assert json.loads((boot/'binding.json').read_text(encoding='utf-8-sig'))['eboot_sha256']==sha
memory=re.findall(r'^PASS retail PSP memory budget: (.+)$',(root/'build-radio-rework-v3.log').read_text(encoding='utf-8',errors='replace'),re.M);assert len(memory)==1
with zipfile.ZipFile(root/'deliverables/ELITE-NEXT-2.5.475-source-evidence.zip') as z:
 for name in ('src/main.c','src/space-discoveries.h','src/encounter-art-tiles.h'):assert (root/name).read_bytes()==z.read(name)
 old=z.read('src/encounter-custom.h').decode().replace('\r\n','\n');new=(root/'src/encounter-custom.h').read_text()
 def exclude_radio(s):
  a=s.index('/* Independent neighbourhood broadcaster:');b=s.index('/* Tea barge:',a);return s[:a]+s[b:]
 assert exclude_radio(old)==exclude_radio(new)
manifest=dict(version='2.5.476',kind='PSP-3000 radio art rework playtest',sha256=sha,radio_entry=14,side_specific_facades=['front96x64','rear96x64','left64x64','right64x64','roof96x64'],rounded_skin='512x224 over16 sectors and7 latitude bands',new_material_tiles=186,atlas_materials=301,renderer_fix='Dynamic disjoint atlas/Adder groups and sparse ascending material iteration',host_regression_cases_passed=260,normal_startup_passed=True,memory=memory[0],embedded_intro_unchanged=True,other_nine_geometry_and_old_materials_unchanged=True,exact_binary_checks=checks,strict_fps_shortfalls=shortfalls,physical_psp_verified=False,limitations=['Radio orbit passes29.5FPS floor; relay/carrier near-view strict FPS failures remain as recorded.','Occasional50–68ms frames remain; this is not a locked-frame-rate or bug-free release claim.','Earlier ten draft previews were rejected as too basic; this release reworks only the radio.','Existing encounter distribution and interactions retained; gallery uses a private forced-entry fixture.'])
out=root/'deliverables/ELITE-NEXT-2.5.476-RADIO-REWORK';out.mkdir(exist_ok=True);shutil.copy2(binary,out/'EBOOT.PBP');(out/'validation.json').write_text(json.dumps(manifest,indent=2))
(out/'README.txt').write_text('ELITE NEXT2.5.476 - PSP-3000 RADIO ART REWORK PLAYTEST\n\nRadio shack rebuilt on a rounded service pod and railed platform. Each principal cabin side has its own continuous pixel-art painting. Dish, deck, roof and underside are fitted separately.\n\nReplace EBOOT.PBP in your existing game folder; keep your saves and music. Music is not bundled. Existing intro film is embedded unchanged.\n\nRadio native orbit averages30.42/31.14/29.75FPS in PPSSPP. Other checks: close relay29.11 and carrier28.70FPS miss the29.5 strict review floor; those performance issues remain open. No audio-buffer starvation/drops or memory faults in final checks. Physical PSP confirmation pending. This is a playtest.\n\nThe other nine earlier draft designs remain unchanged and await this deeper art treatment. Existing encounter placement and interactions are retained; the gallery forces entry14 for inspection.\n\nSHA-256: '+sha+'\n')
archive=out.with_name(out.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 for p in sorted(out.iterdir()):z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(archive) as z:assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha
(root/'assets/generated/radio-shack-rework/verification.json').write_text(json.dumps(manifest,indent=2))
print(json.dumps(dict(eboot=str(out/'EBOOT.PBP'),zip=str(archive),sha256=sha,radio_fps=checks[0]['fps'],strict_fps_shortfalls=shortfalls),indent=2))
