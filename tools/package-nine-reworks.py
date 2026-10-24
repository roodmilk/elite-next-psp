"""Publish native, exact-binary evidence for the nine individually reworked encounters."""
from pathlib import Path
import hashlib, html, json, re, shutil, struct, zipfile
from PIL import Image

root = Path(__file__).resolve().parents[1]
binary = root / 'build-nine-reworks-final/EBOOT.PBP'
data = binary.read_bytes()
sha = hashlib.sha256(data).hexdigest().upper()
assert data[:4] == b'\0PBP'
assert data[struct.unpack('<10I', data[:40])[9]:] == (root/'assets/intro/cinematic.enm').read_bytes()
evidence = root.parent/'erl-implementation-candidate/outputs/magnetic-slice'
gallery = root/'assets/generated/nine-encounter-reworks'
cases = [(8,1,'salvage','Salvage Village'), (9,15,'breaker',"Shipbreaker’s Yard"),
         (10,3,'garden','Community Greenhouse'), (11,0,'fuel','Patchwork Fuel Stop'),
         (12,4,'ferry','Old Passenger Ferry'), (14,6,'tea','Miners’ Tea Barge'),
         (15,8,'ore','Cooperative Ore Depot'), (16,9,'parts','Parts Exchange'),
         (17,10,'beacon','Beacon Repair Crew')]
checks, shortfalls = [], []
ledger_path = root/'design/encounter-catalogue-progress.json'
ledger = json.loads(ledger_path.read_text(encoding='utf-8'))
style = '<style>body{background:#091018;color:#d8e6ed;font:16px system-ui;max-width:1040px;margin:32px auto;padding:0 16px}a{color:#72d4dd}img{image-rendering:pixelated;max-width:100%;height:auto}figure{display:inline-block;margin:10px 12px 20px 0}figcaption{font-size:12px;color:#9ab4c4}section{margin:32px 0}h2{font-size:22px}</style>'
cards = []
for scene, entry, slug, name in cases + [(13,14,'radio','Local Radio Shack'), (0,-1,'relay','Outer relay regression'), (2,-2,'carrier','Animal carrier regression'), (6,-3,'mechanic','Mobile mechanic regression')]:
    run = evidence/f'regression-nine477-{slug}-final-diagnostic-encounter-flight-capture'
    binding = json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
    assert binding['eboot_sha256'] == sha
    report = (run/'encounter-flight-capture.txt').read_text()
    assert 'starves 0 drops 0' in report
    assert all('orbit' in line for line in report.splitlines() if line.startswith('FAIL '))
    fps = list(map(float, re.findall(r'orbit ([0-9.]+) FPS', report)))
    assert len(fps) == 3
    log = (run/'emulator.log').read_text(encoding='utf-8', errors='replace')
    assert all(x not in log for x in ('Bad memory access','MEMBLOCK_ALLOC_FAILED','Stopping emulation'))
    for band,value in zip(('near','medium','distant'),fps):
        if value < 29.5: shortfalls.append(dict(scene=name, band=band, fps=value, strict_floor=29.5))
    checks.append(dict(name=name, scene=scene, entry=entry, fps=fps, report=report, binding=binding))
    if entry < 0: continue
    out = gallery/slug
    out.mkdir(exist_ok=True)
    frames = []
    for p in sorted(run.glob('encounter-retail-*.bmp'), key=lambda p:int(p.stem.rsplit('-',1)[1])):
        im = Image.open(p)
        assert im.size == (480,272)
        im.convert('RGB').save(out/(p.stem+'.png'))
        frames.append(p.stem+'.png')
    assert len(frames) == 7
    (out/'verification.txt').write_text(report)
    (out/'binding.json').write_text(json.dumps(binding, indent=2))
    state = 'reworked; native angles inspected; orbit floor passed; physical PSP pending' if min(fps)>=29.5 else 'reworked; native angles inspected; strict FPS shortfall recorded; physical PSP pending'
    ledger['entries'][entry].update(status=state, evidence=str(out.relative_to(root)), fps=fps, eboot_sha256=sha)
    body = f'<h1>{html.escape(name)}</h1><p>2.5.477 — actual 480×272 in-game renders, using the PSP game’s renderer. Separate major-surface pixel paintings and purpose-built geometry.</p><p>Near / medium / distant orbit: {fps} FPS in PPSSPP. These are averages, not a locked frame rate; occasional 50–68ms frames remain. Physical PSP testing pending.</p><p>Private camera fixture forces this catalogue entry for inspection. Normal encounter placement and interactions are retained.</p>'
    body += ''.join(f'<figure><img width="480" height="272" src="{f}"><figcaption>{f}</figcaption></figure>' for f in frames)
    (out/'index.html').write_text('<!doctype html><meta charset="utf-8"><title>'+html.escape(name)+'</title>'+style+body, encoding='utf-8')
    cards.append(f'<section><h2>{html.escape(name)}</h2><a href="{slug}/index.html"><img width="480" height="272" src="{slug}/encounter-retail-30.png"></a><p>{fps} FPS — <a href="{slug}/index.html">front, rear, side and distant views</a></p></section>')

boot = evidence/'regression-nine477-boot-final-diagnostic-normal-boot'
assert 'RESULT 0 failures' in (boot/'normal-boot.txt').read_text()
assert json.loads((boot/'binding.json').read_text(encoding='utf-8-sig'))['eboot_sha256'] == sha
diagnostics=[]
diagnostic_sha=hashlib.sha256((root/'build-nine-reworks-review/EBOOT.PBP').read_bytes()).hexdigest().upper()
for tag,flag in [('race','erl-quick-race-review'),('menu','final-menu')]:
    run=evidence/f'regression-nine477-{tag}-logic-diagnostic-{flag}'
    report=(run/(flag+'.txt')).read_text()
    assert 'RESULT 0 failures' in report
    binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
    assert binding['eboot_sha256']==diagnostic_sha
    # This narrower review build is not the old EnableReview/ExtendedMemory profile.
    binding['profile']='EncounterReview + Psp3000 diagnostic build; not retail performance evidence'
    (run/'binding.json').write_text(json.dumps(binding,indent=2))
    diagnostics.append(dict(flag=flag,report=report,binding=binding))
assert json.loads((evidence/'regression-nine477-race-logic-diagnostic-erl-quick-race-review/save-isolation.json').read_text(encoding='utf-8-sig'))['all_unchanged']
memory = re.findall(r'^PASS retail PSP memory budget: (.+)$',(root/'build-nine-reworks-final.log').read_text(encoding='utf-8',errors='replace'),re.M)
assert len(memory) == 1
with zipfile.ZipFile(root/'deliverables/ELITE-NEXT-2.5.476-source-evidence.zip') as z:
    for name in ('src/radio-shack-detailed.h','src/radio-shack-art.h','src/radio-shack-tiles.h','src/encounter-art-tiles.h'):
        assert (root/name).read_bytes() == z.read(name),name
manifest = dict(version='2.5.477',kind='PSP-3000 nine encounter art reworks playtest',sha256=sha,
                atlas_tiles=951,new_tiles=650,continuous_authored_surfaces=93,
                exact_binary_checks=checks,strict_fps_shortfalls=shortfalls, separate_diagnostic_checks=diagnostics,
                normal_startup_passed=True,embedded_intro_unchanged=True,
                approved_radio_geometry_and_art_unchanged=True,memory=memory[0],
                material_namespace='Disjoint 65536+2*tile+half for fitted quads; fleet/Adder/racer ranges retained',
                host_boundary_order_cases_passed=260,physical_psp_verified=False,
                limitations=['PPSSPP orbit averages, not a locked frame-rate guarantee. Occasional long frames remain.',
                             'Physical PSP and sustained gameplay with streamed music remain unverified for this candidate.',
                             'Private capture fixture; no new encounter placements, services or save changes.',
                             'Nine earlier rejected drafts replaced. Other catalogue entries remain separate work.'])
(gallery/'verification.json').write_text(json.dumps(manifest,indent=2))
ledger['quality_gate'] = 'Purposeful structure; distinct continuous major-surface paintings; native front/rear/side/far review; measured orbit FPS with failures disclosed. User approval and physical PSP are separate.'
ledger_path.write_text(json.dumps(ledger,indent=2))
body = '<h1>Nine encounter reworks + approved radio</h1><p>2.5.477 — native 480×272 in-game captures. Click any image for its full orbit. These replace the earlier basic drafts; the radio’s approved geometry and paintings are preserved.</p><p>Measured on PPSSPP PSP-3000. Physical PSP testing pending; frame-rate averages do not promise a locked 30FPS.</p>'+''.join(cards)
(gallery/'index.html').write_text('<!doctype html><meta charset="utf-8"><title>ELITE NEXT — reworked encounters</title>'+style+body,encoding='utf-8')
(root/'assets/generated/encounter-upgrades/catalogue-progress.html').write_text('<!doctype html><meta charset="utf-8"><title>ELITE NEXT — revised encounter gallery</title>'+style+body.replace('href="','href="../nine-encounter-reworks/').replace('src="','src="../nine-encounter-reworks/'),encoding='utf-8')
out=root/'deliverables/ELITE-NEXT-2.5.477-NINE-REWORKS'
out.mkdir(exist_ok=True)
shutil.copy2(binary,out/'EBOOT.PBP')
(out/'validation.json').write_text(json.dumps(manifest,indent=2))
readme='ELITE NEXT 2.5.477 - PSP-3000 ENCOUNTER ART PLAYTEST\n\nNine redesigned encounters: salvage village, shipbreaker, greenhouse, fuel stop, passenger ferry, tea barge, ore depot, parts exchange and beacon repair crew. Approved radio retained. Each has fitted major-surface pixel art and a purpose-built structure.\n\nReplace EBOOT.PBP in your existing game folder. Keep your saves and music. Music is not bundled. Existing intro is embedded unchanged. PSP-3000 build; physical hardware confirmation pending.\n\nNear/medium/distant PPSSPP orbit averages and any strict-floor failures are recorded in validation.json. Occasional long frames remain. No locked-FPS or bug-free release claim.\n\nSHA-256: '+sha+'\n'
(out/'README.txt').write_text(readme)
archive=out.with_name(out.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
    for p in sorted(out.iterdir()):z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha
print(json.dumps(dict(eboot=str(out/'EBOOT.PBP'),zip=str(archive),sha256=sha,strict_fps_shortfalls=shortfalls),indent=2))
