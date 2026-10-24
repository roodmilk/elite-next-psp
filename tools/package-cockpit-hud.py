"""Package only executable-bound native cockpit evidence, preserving shortfalls."""
from pathlib import Path
import argparse, hashlib, html, json, re, shutil, struct, zipfile
from PIL import Image
import csv

ap=argparse.ArgumentParser()
ap.add_argument('--build',required=True)
ap.add_argument('--prefix',required=True)
ap.add_argument('--logic-tag',required=True)
args=ap.parse_args()
root=Path(__file__).resolve().parents[1]
binary=root/args.build/'EBOOT.PBP'
data=binary.read_bytes();sha=hashlib.sha256(data).hexdigest().upper()
assert data[:4]==b'\0PBP'
assert data[struct.unpack('<10I',data[:40])[9]:]==(root/'assets/intro/cinematic.enm').read_bytes()
bench=root.parent/'erl-implementation-candidate/outputs/magnetic-slice'
logic=bench/f'regression-{args.logic_tag}-diagnostic-cockpit-hud-review'
report=(logic/'cockpit-hud-review.txt').read_text(encoding='utf-8')
assert 'RESULT 0 failures' in report and 'native cached pixel mismatches 0' in report
assert json.loads((logic/'save-isolation.json').read_text(encoding='utf-8-sig'))['all_unchanged']
logic_binding=json.loads((logic/'binding.json').read_text(encoding='utf-8-sig'))
gallery=root/'assets/generated/cockpit-hud478';gallery.mkdir(exist_ok=True)
cases=[('idle','No target'),('combined','Both computers / eight slots'),('onegun','One-slot hull'),('alert','Missile alert'),('unavailable','Unavailable equipment'),('longname','Full gun names'),('confirmed','Browsing and confirmation'),('legacy','Legacy comparison'),('twin-idle','Twin Stack idle'),('twin-miss','Twin Stack / missed shots'),('twin-shield','Shield-target fixture / scenery occlusion'),('twin-hull','Hull-target fixture / scenery occlusion'),('twin-occlusion','Foreground station occlusion'),('twin-chase','Chase camera'),('twin-rapid','Rapid pulse'),('twin-switch','Real weapon switching'),('twin-contrast','High contrast'),('twin-combat','24-hostile combat stress'),('twin-station','Near station'),('twin-planet','Near planet'),('twin-anomaly','Near encounter'),('twin-variants','Hull variation'),('twin-music','Firing with streamed music')]
checks=[];shortfalls=[];cards=[]
style='<style>body{background:#091018;color:#d8e6ed;font:16px system-ui;max-width:1040px;margin:32px auto;padding:0 16px}a{color:#72d4dd}img{image-rendering:pixelated;max-width:100%;height:auto}section{margin:32px 0}figure{display:inline-block;margin:10px 12px 20px 0}figcaption{font-size:12px;color:#9ab4c4}</style>'
for slug,name in cases:
    run=bench/f'regression-{args.prefix}-{slug}-diagnostic-encounter-flight-capture'
    binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
    assert binding['eboot_sha256']==sha
    assert binding.get('internal_resolution')==1
    text=(run/'encounter-flight-capture.txt').read_text(encoding='utf-8')
    assert 'starves 0 drops 0' in text
    assert all('orbit' in row for row in text.splitlines() if row.startswith('FAIL '))
    fps=list(map(float,re.findall(r'orbit ([0-9.]+) FPS',text)));assert len(fps)==3
    for band,value in zip(('window 1','window 2','window 3') if slug.startswith('twin-') else ('near','medium','distant'),fps):
        if value<29.5:shortfalls.append(dict(scene=name,band=band,fps=value,strict_floor=29.5))
    log=(run/'emulator.log').read_text(encoding='utf-8',errors='replace')
    assert not any(s in log for s in ('Bad memory access','MEMBLOCK_ALLOC_FAILED'))
    if slug!='legacy':assert 'layout overflow counter 0' in text
    checks.append(dict(scene=name,fps=fps,binding=binding,report=text))
    out=gallery/slug;out.mkdir(exist_ok=True)
    frames=[]
    for p in sorted(run.glob('encounter-retail-*.bmp'),key=lambda p:int(p.stem.rsplit('-',1)[1])):
        im=Image.open(p);assert im.size==(480,272)
        im.convert('RGB').save(out/(p.stem+'.png'));frames.append(p.stem+'.png')
    assert len(frames)==7
    (out/'verification.txt').write_text(text,encoding='utf-8')
    (out/'binding.json').write_text(json.dumps(binding,indent=2),encoding='utf-8')
    bands='Consecutive time-window' if slug.startswith('twin-') else 'Near / medium / distant'
    body=f'<h1>{html.escape(name)}</h1><p>Actual 480×272 in-game frames. {bands} averages: {fps} FPS in PPSSPP, internal resolution explicitly native 1×. Physical PSP pending.</p>'
    body+=''.join(f'<figure><img width="480" height="272" src="{p}"><figcaption>{p}</figcaption></figure>' for p in frames)
    (out/'index.html').write_text('<!doctype html><meta charset="utf-8">'+style+body,encoding='utf-8')
    if slug!='legacy':cards.append(f'<section><h2>{html.escape(name)}</h2><a href="{slug}/index.html"><img width="480" height="272" src="{slug}/encounter-retail-30.png"></a><p>{fps} FPS — <a href="{slug}/index.html">more native frames</a></p></section>')

boot=bench/f'regression-{args.prefix}-boot-diagnostic-normal-boot'
assert 'RESULT 0 failures' in (boot/'normal-boot.txt').read_text(encoding='utf-8')
assert json.loads((boot/'binding.json').read_text(encoding='utf-8-sig'))['eboot_sha256']==sha
memory=re.findall(r'^PASS retail PSP memory budget: (.+)$',(root/(args.build+'.log')).read_text(encoding='utf-8',errors='replace'),re.M)
assert len(memory)==1
manifest=dict(version='2.5.478',kind='PSP-3000 cockpit integration playtest',sha256=sha,
              diagnostic_checks=int(re.search(r'CHECKS (\d+)',report)[1]),diagnostic_report=report,diagnostic_binding=logic_binding,
              native_reference_mismatches=0,twin_cache_reference_mismatches=0,original_circle_pixels_preserved=True,test_saves_unchanged=True,embedded_intro_unchanged=True,
              exact_retail_checks=checks,strict_fps_shortfalls=shortfalls,memory=memory[0],physical_psp_verified=False,
              rendering='Native font8, exact cached ring outlines, cached opaque UI row spans, depth-aware Twin Stack native colour/depth cache with live 3D fallback; real game data and discharge events. Live world and rear mirror every frame. Only pixels behind proven opaque shell areas are clipped.',
              limitations=['PPSSPP averages are not locked-frame-rate or physical PSP certification.','Strict FPS shortfalls are retained verbatim.','Native capture fixture stages UI states; injected input checks are separate from a human career playthrough.','Existing special atmospheric, docking, police, death, hyperspace and ERL HUD handlers retained.'])
assert logic_binding['eboot_sha256']==hashlib.sha256((root/args.build.replace('478-v','478-review-v')/'EBOOT.PBP').read_bytes()).hexdigest().upper()
assert 'Twin Stack cache/reference mismatches 0' in report
assert 'cockpit hidden-region uncovered pixels 0' in report
music=bench/f'regression-{args.prefix}-twin-music-diagnostic-encounter-flight-capture/audio-flight-performance.txt'
music_text=music.read_text(encoding='utf-8');assert 'source 7;' in music_text and 'decoder error 0' in music_text and 'buffer starves 0' in music_text
# Compare native images before/after the audio-only queue fix, retaining any differences.
unchanged=[]
for slug,_ in cases:
    old=bench/f'regression-hud478-v16n-{slug}-diagnostic-encounter-flight-capture'
    new=bench/f'regression-{args.prefix}-{slug}-diagnostic-encounter-flight-capture'
    pairs=[(p,new/p.name) for p in old.glob('encounter-retail-*.bmp')]
    unchanged.append(dict(scene=slug,paired_frames=len(pairs),byte_identical=sum(a.read_bytes()==b.read_bytes() for a,b in pairs)))
manifest['audio_fix_native_frame_comparison']=unchanged
manifest['streamed_music_report']=music_text
manifest['resolved_audio_issue']='V16 rapid-fire test repeatedly dropped9 sound commands. V17 increases only the bounded event queue from16 to64 entries (+384 bytes); final exact-retail rapid-fire result is included. Original failing evidence retained under regression-hud478-v16n-rapid-first-diagnostic-encounter-flight-capture.'
extra={}
for flag,tag in [('final-menu','menu'),('erl-quick-race-review','erl')]:
    run=bench/f'regression-{args.prefix}-{tag}-diagnostic-{flag}'
    text=(run/(flag+'.txt')).read_text(encoding='utf-8');assert 'RESULT 0 failures' in text
    extra[tag]=dict(report=text,binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig')))
    assert extra[tag]['binding']['eboot_sha256']==logic_binding['eboot_sha256']
manifest['existing_integration_checks']=extra
manifest['broad_smoke']='Separate full-review v12 smoke was attempted. Radio, steering and NPC-combat groups pass; old game/input assertions are not green and the broad run timed out in obsolete input/shipyard coverage. Full-suite/broken-free certification is not claimed. See design/cockpit-hud478/BROAD-SMOKE.json.'
(gallery/'verification.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
for name in ('hud-gu-actual','hud-gu-reference','hud-warp-charge','hud-mission-progress'):
    Image.open(logic/(name+'.bmp')).save(gallery/(name+'.png'))
body='<h1>ELITE NEXT — cockpit computers</h1><p>2.5.478: actual native 480×272 game renders, not image-generated mockups. Private UI/camera fixtures; real equipment, contacts, scanner and weapon data.</p><p>Hold Circle + Right: next fitted gun immediately. Release Circle: close and keep it. Square: open/close target computer. D-pad: browse. X: confirm; R+X: confirm and align.</p>'
body+=f'<p>{manifest["diagnostic_checks"]} native input/layout checks passed; Cached cockpit and housing pixels match their uncached in-game drawing paths exactly; this does not claim a pixel-for-pixel match to the out-of-engine reference. Physical PSP testing pending. Orbit frame-rate averages and any strict-floor failures are recorded below and in verification.json.</p>'+'<p><a href="motion.html">Native idle/firing footage beside approved reference</a> · <a href="mounts/index.html">All 23 actual ship mount views</a></p>'+''.join(cards)
(gallery/'index.html').write_text('<!doctype html><meta charset="utf-8"><title>ELITE NEXT cockpit — native review</title>'+style+body,encoding='utf-8')
# Actual moving footage uses raw native framebuffer copies to avoid a slow
# per-pixel BMP conversion while capturing. Timing is cumulatively quantized,
# rather than speeding every 33 ms frame up to 30 ms in GIF's 10 ms timebase.
reference=root/'design/twin-stack-reference/APPROVED-G-TWIN-STACK.gif'
shutil.copy2(reference,gallery/'approved-reference.gif')
shutil.copy2(root/'design/twin-stack-reference/APPROVED-G-IDLE.png',gallery/'approved-idle.png')
motion=[];footage_proofs=[]
for slug,title in [('idle','Idle / receiver and cockpit animation'),('fire','Firing into empty space'),('hit','Firing near a planet'),('ui','Weapons and target computers')]:
    run=bench/f'regression-{args.prefix}-footage-{slug}-diagnostic-encounter-flight-capture'
    binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'));assert binding['eboot_sha256']==sha and binding['footage']
    rows=list(csv.reader((run/'twin-footage.csv').open()));assert len(rows)==80
    frames=[];durations=[];apng_durations=[];total=0;previous=0;previous_ms=0
    for row in rows:
        frame=int(row[0]);raw=(run/f'twin-frame-{frame:03d}.rgba').read_bytes();assert len(raw)==512*272*4
        im=Image.frombytes('RGBA',(512,272),raw).crop((0,0,480,272)).convert('RGB');frames.append(im)
        total+=float(row[1]);milliseconds=round(total*1000);apng_durations.append(max(1,milliseconds-previous_ms));previous_ms=milliseconds;quantized=round(total*100);duration=max(1,quantized-previous)*10;previous=quantized;durations.append(duration)
    frames[0].save(gallery/f'{slug}-native.apng',format='PNG',save_all=True,append_images=frames[1:],duration=apng_durations,loop=0)
    frames[0].save(gallery/f'{slug}-native.gif',save_all=True,append_images=frames[1:],duration=durations,loop=0,optimize=False)
    frames[0].save(gallery/f'{slug}-native.png')
    footage_proofs.append(dict(scene=title,binding=binding,frames=80,recorded_seconds=total,source='Native 512-stride RGBA8888 framebuffer; 480x272 visible pixels. Capture run excluded from FPS claims.'))
    motion.append(f'<h2>{title}</h2><div class="pair"><figure><figcaption>Approved out-of-engine reference</figcaption><img width="480" height="272" src="approved-reference.gif"></figure><figure><figcaption>Actual in-engine 480×272 — recorded timing</figcaption><img width="480" height="272" src="{slug}-native.apng"></figure></div>')
(gallery/'motion.html').write_text('<!doctype html><meta charset="utf-8"><title>Twin Stack native comparison</title>'+style+'<style>.pair{display:flex;gap:16px;overflow-x:auto}.pair figure{flex:0 0 480px;margin:0}img{max-width:none}</style><h1>G — Twin Stack: native game footage</h1><p>The left animation is the approved reference, not a PSP performance result. The right animations are actual native game framebuffers in lossless APNG at their recorded capture timing; GIF previews are also saved separately. The cockpit uses real contacts, equipment and discharge events. FPS comes from separate capture-free benchmark runs.</p>'+''.join(motion),encoding='utf-8')
pair=Image.new('RGB',(976,272),(9,16,24));pair.paste(Image.open(gallery/'approved-idle.png').convert('RGB'),(0,0));pair.paste(Image.open(gallery/'idle-native.png').convert('RGB'),(496,0));pair.save(gallery/'idle-comparison.png')
(gallery/'footage-bindings.json').write_text(json.dumps(footage_proofs,indent=2),encoding='utf-8')
mounts=gallery/'mounts';mounts.mkdir(exist_ok=True)
mount_cards=[]
for p in sorted(logic.glob('twin-mount-*.bmp')):
    im=Image.open(p);assert im.size==(480,272);im.save(mounts/(p.stem+'.png'))
    ship=int(p.stem.rsplit('-',1)[1]);match=re.search(r'^MOUNT '+str(ship)+r' (.*?) /',report,re.M);name=match[1] if match else str(ship)
    mount_cards.append(f'<figure><img width="480" height="272" src="{p.stem}.png"><figcaption>{html.escape(name)}</figcaption></figure>')
assert len(mount_cards)==23
(mounts/'index.html').write_text('<!doctype html><meta charset="utf-8">'+style+'<h1>Actual mounts — all 23 hulls</h1><p>Native in-engine diagnostic views, not concept art. Housings follow real hardpoints; offscreen mounts are not forced into the windshield. Their traces can still enter view. One equipment slot does not mean one visible barrel.</p>'+''.join(mount_cards),encoding='utf-8')
out=root/'deliverables/ELITE-NEXT-2.5.478-COCKPIT';out.mkdir(exist_ok=True)
shutil.copy2(binary,out/'EBOOT.PBP')
(out/'validation.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
(out/'README.txt').write_text('ELITE NEXT 2.5.478 - PSP-3000 COCKPIT + TWIN STACK PLAYTEST\n\nFor an existing installation, replace EBOOT.PBP only. For a fresh PSP install, copy ELITE_NEXT from the ZIP into PSP/GAME/. In PPSSPP, open ELITE_NEXT/EBOOT.PBP. Keep your saves and music; music is not bundled. Existing intro remains embedded unchanged.\n\nHold Circle + Right to equip the next fitted gun. Repeated taps work within the same hold. Empty banks are skipped. Release closes and keeps the gun. Circle+L missiles; Circle+R flares; Up law scan; Down heat sink; Left tractor. Release and tap Circle to use the selected tool. Circle+Triangle heat-sink alias remains.\n\nSquare toggles target computer. D-pad browses without changing the lock. X confirms. R+X confirms and aligns. Square closes. Analog flight continues.\n\nG Twin Stack uses actual ship hardpoints, closed stacked housings and real discharge/hit events. No weapon balance or save-layout changes.\n\nNative emulator validation and any strict frame-rate shortfalls: validation.json. Physical PSP testing remains pending. No locked-FPS or bug-free release claim.\n\nSHA-256: '+sha+'\n',encoding='utf-8')
archive=out.with_name(out.name+'.zip')
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
    for p in sorted(out.iterdir()):z.write(p,'ELITE_NEXT/'+p.name)
with zipfile.ZipFile(archive) as z:
    assert z.testzip() is None and hashlib.sha256(z.read('ELITE_NEXT/EBOOT.PBP')).hexdigest().upper()==sha
print(json.dumps(dict(sha256=sha,zip=str(archive),strict_fps_shortfalls=shortfalls),indent=2))
