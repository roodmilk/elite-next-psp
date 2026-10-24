from pathlib import Path
from PIL import Image
import hashlib,json,struct,zipfile
root=Path(__file__).resolve().parents[1];out=root/'deliverables/ELITE-NEXT-2.5.486-EXPLORATION';v=json.loads((out/'validation.json').read_text(encoding='utf-8'));sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert v['native_checks']==360 and (root/'VERSION').read_text(encoding='utf-8').strip()=='2.5.486'
for p in [out/'EBOOT.PBP',root/'EBOOT.PBP',root/'dist/ELITE-NEXT-PSP3000/EBOOT.PBP',root/'build-nav486-retail-i/EBOOT.PBP']:assert sha(p)==v['retail_sha256']
assert sha(root/'dist/ELITE-NEXT-PSP/EBOOT.PBP')==v['original_psp_stable_sha256']
assert sha(root/'deliverables/ELITE-NEXT-2.5.485-SOLID-TWIN-GUNS/EBOOT.PBP')=='2CF6DB3A524029EC6F69ACE4BD21662B368BA3303B0D3297F9B69DCEFD9F1F38'
for file,digest in v['source_hashes'].items():assert sha(root/file).lower()==digest
with zipfile.ZipFile(str(out)+'.zip') as z:
 assert z.testzip() is None and sorted(z.namelist())==sorted(['ELITE_NEXT/EBOOT.PBP','ELITE_NEXT/READ-ME.txt','ELITE_NEXT/validation.json'])
 assert hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==v['retail_sha256']
 assert len(z.read('ELITE_NEXT/READ-ME.txt').decode('utf-8').splitlines())>12
with zipfile.ZipFile(root/'design/nav-events486/release-source.zip') as z:
 assert z.testzip() is None
 for file,digest in v['source_hashes'].items():assert hashlib.sha256(z.read(Path(file).as_posix())).hexdigest()==digest
for name in ['README.md','CLAUDE-HANDOFF.md','CHANGELOG.md']:
 s=(root/name).read_text(encoding='utf-8');assert s.startswith(('2.5.486','## 2.5.486')) and '2.5.485' in s and len(s)>10000
for name in ['build.ps1','build.sh']:assert 'Release 2.5.486:' in (root/name).read_text(encoding='utf-8')[:150]
gallery=root/'assets/generated/navigation486';proof=json.loads((gallery/'retail-footage-proof.json').read_text(encoding='utf-8'));assert proof['eboot_sha256']==v['retail_sha256'] and proof['pixel_identical_to_native_rgb']
with Image.open(gallery/'retail-cloud-motion.apng') as im:assert im.size==(480,272) and im.n_frames==80
s=(gallery/'index.html').read_text(encoding='utf-8');assert 'retail-cloud-motion.apng' in s and 'Full validation' in s
result={'result':'PASS','version':'2.5.486','sha256':v['retail_sha256'],'focused_checks':360,'native_retail_frames':80,'native_dimensions':[480,272],'source_archive_verified':True,'intro_unchanged':v['intro_unchanged'],'original_psp_and_485_preserved':True,'physical_psp_verified':False,'full_legacy_suite_passed':False}
(root/'design/nav-events486/FINAL-VERIFY.json').write_text(json.dumps(result,indent=2),encoding='utf-8');print(json.dumps(result,indent=2))
