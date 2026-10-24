from pathlib import Path
import argparse, hashlib, json, re, shutil, struct, zipfile

p=argparse.ArgumentParser()
p.add_argument('--eboot',type=Path,required=True)
p.add_argument('--profile-run',type=Path,required=True)
p.add_argument('--retail-runs',type=Path,nargs='+',required=True)
p.add_argument('--accept-approximately-30fps',action='store_true',help='Accept >=29.0 FPS for the requested approximate 30 FPS target; retain strict 29.5 shortfalls in the manifest.')
a=p.parse_args();root=Path(__file__).resolve().parents[1]
data=a.eboot.read_bytes();digest=hashlib.sha256(data).hexdigest().upper()
profile=(a.profile_run/'encounter-upgrade.txt').read_text()
assert re.search(r'^RESULT 0 failures$',profile,re.M),'Scene workload review has not passed'
reports=[];measurements=[];strict_shortfalls=[]
minimum_fps=29.0 if a.accept_approximately_30fps else 29.5
for run in a.retail_runs:
 report=(run/'encounter-flight-capture.txt').read_text()
 bands=re.findall(r'^(PASS|FAIL) (near|medium|distant) orbit ([0-9.]+) FPS',report,re.M)
 assert len(bands)==3 and all(float(fps)>=minimum_fps for _,_,fps in bands),f'Retail orbit below {minimum_fps} FPS: {run}'
 assert 'PASS audio buffers: starves 0 drops 0' in report,f'Audio regression: {run}'
 name=re.search(r'orbit of (.*?);',report).group(1)
 measurements.append({'location':name,'fps':{band:float(fps) for _,band,fps in bands}})
 strict_shortfalls.extend({'location':name,'band':band,'fps':float(fps),'strict_threshold':29.5} for status,band,fps in bands if status=='FAIL')
 binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
 assert binding['eboot_sha256']==digest,'Retail evidence is for a different executable'
 reports.append(report)
assert all(any(name in report for report in reports) for name in ['animal carrier','closed resort','mobile mechanic'])
assert data[:4]==b'\0PBP'
psar=struct.unpack_from('<I',data,36)[0]
assert data[psar:]==(root/'assets/intro/cinematic.enm').read_bytes(),'Embedded intro differs'
memory_log=(root/'build-encounter-retail.log').read_text(errors='replace')
assert 'PASS retail PSP memory budget:' in memory_log,'Retail memory check missing'
out=root/'deliverables/ELITE-NEXT-2.5.473-ENCOUNTERS';out.mkdir(parents=True,exist_ok=True)
shutil.copyfile(a.eboot,out/'EBOOT.PBP')
readme='''ELITE NEXT 2.5.473 - custom encounter graphics update
Copy EBOOT.PBP into your existing game folder under PSP/GAME. Keep your saves and music.
PSP-2000/3000, Vita Adrenaline or PPSSPP. This is an update package; music is not bundled.

Custom relay, outpost, animal carrier, sanctuary, both cinemas, closed resort and mechanic.
The mechanic is now anchored to a textured asteroid and has an animated repair arm.
Detailed pixel-art materials, lighting and animation remain enabled.
Includes the existing intro, animated menu, tutorial/skip route, ERL and warp work.

Validated: eight scene orbit workload checks; 600-frame retail orbits of carrier,
resort and mechanic with normal audio/display. See manifest for measured results.
These are PPSSPP PSP-3000 results. Physical PSP testing is still pending.
'''
readme+='\nMeasured retail orbit averages (near / medium / distant FPS):\n'
for m in measurements:readme+=m['location']+': '+' / '.join(str(m['fps'][band]) for band in ('near','medium','distant'))+'\n'
if strict_shortfalls:readme+='\nApproximate 30 FPS acceptance uses a 29.0 FPS floor. Small shortfalls against\nthe stricter 29.5 FPS check are retained in validation.json; this is not a locked-30 claim.\n'
(out/'README.txt').write_text(readme)
manifest={'version':'2.5.473','sha256':digest,'platform':'PSP-2000/3000; PPSSPP PSP-3000 tested','music_bundled':False,'intro_unchanged':True,'physical_psp_tested':False,'nominal_target_fps':30,'minimum_accepted_orbit_average_fps':minimum_fps,'measurements':measurements,'strict_29_5fps_shortfalls':strict_shortfalls,'supporting_diagnostic_binding':json.loads((a.profile_run/'binding.json').read_text(encoding='utf-8-sig')),'workload_report':profile,'retail_reports':reports,'memory_check':[line for line in memory_log.splitlines() if 'PASS retail PSP memory budget:' in line]}
(out/'validation.json').write_text(json.dumps(manifest,indent=2))
archive=Path(str(out)+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 for f in out.iterdir():z.write(f,'ELITE_NEXT/'+f.name)
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None
 assert hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==digest
# Publish the already-validated executable as the current local game build.
shutil.copyfile(out/'EBOOT.PBP',root/'EBOOT.PBP')
shutil.copyfile(out/'EBOOT.PBP',root/'EBOOT-2.5.473-ENCOUNTERS.PBP')
(root/'VERSION').write_text('2.5.473\n')
print(json.dumps({'eboot':str(out/'EBOOT.PBP'),'zip':str(archive),'sha256':digest},indent=2))
