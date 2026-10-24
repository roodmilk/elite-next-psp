from pathlib import Path
from PIL import Image
import argparse,shutil,json,html
p=argparse.ArgumentParser();p.add_argument('evidence');a=p.parse_args();root=Path(__file__).resolve().parents[1];e=Path(a.evidence).resolve();out=root/'assets/generated/navigation486';out.mkdir(parents=True,exist_ok=True)
report=(e/'navigation-review.txt').read_text();assert 'RESULT 0 failures' in report
for f in e.glob('nav486-*.bmp'):
 im=Image.open(f).convert('RGB');assert im.size==(480,272);im.save(out/(f.stem+'.png'))
frames=[Image.open(e/f'nav486-cloud-{k:02d}.bmp').convert('RGBA') for k in range(40)]
frames[0].save(out/'cloud-entry.apng',save_all=True,append_images=frames[1:],duration=100,loop=0,disposal=0,blend=0)
ref=root/'design/nav-events486/reference/references/space-region-concepts-1-native.png';shutil.copy2(ref,out/'approved-offline-reference.png')
shutil.copy2(e/'navigation-review.txt',out/'navigation-review.txt')
rows=[('Buoy close approach','nav486-buoy.png'),('Choose an independently plotted lead','nav486-local-leads.png'),('Actual listening-post encounter','nav486-listening-post.png'),('Passive reception before resolution','nav486-faint.png')]
regions=['Finite gas field','Magnetic activity','Peaceful auroral reach','Wreckage wake','Comet stream','Quiet dust veil']
rows += [(label,f'nav486-region-{k}.png') for k,label in enumerate(regions)]
if (out/'nav486-paid-leads.png').exists(): rows.append(('Remote local search offers','nav486-paid-leads.png'))
cards=''.join(f'<figure><figcaption>{html.escape(label)}</figcaption><img width="480" height="272" src="{file}"></figure>' for label,file in rows)
(out/'index.html').write_text(f'''<!doctype html><meta charset="utf-8"><title>ELITE NEXT — Exploration 2.5.486</title><style>body{{background:#09121b;color:#d6e4e7;font:16px system-ui;margin:24px}}h1{{color:#e5b676}}.grid{{display:flex;flex-wrap:wrap;gap:20px}}figure{{margin:0 0 20px}}figcaption{{margin:8px 0;color:#b9d5d9}}img{{image-rendering:pixelated;max-width:100%;height:auto;border:1px solid #42636a}}p{{max-width:1000px;line-height:1.5}}a{{color:#7dd4de}}pre{{white-space:pre-wrap}}</style><h1>Exploration / native engine evidence</h1><p>Actual 480×272 PSP renderer output, current integrated cockpit and guns. These are injected test poses and a sequential moving camera, not a human playthrough or physical PSP recording. Performance evidence is reported separately.</p><div class="grid"><figure><figcaption>Supplied offline concept — visual reference only</figcaption><img width="480" height="272" src="approved-offline-reference.png"></figure><figure><figcaption>Actual engine — finite cloud entry, world-space moving camera</figcaption><img width="480" height="272" src="cloud-entry.apng"></figure></div><p>The reference is a target, not a claimed pixel-for-pixel match. The engine uses seeded volume layers, native opacity textures, real geometry depth, local light and finite boundaries. Existing art remains intact.</p><div class="grid">{cards}</div><p><a href="navigation-review.txt">Complete navigation test report</a></p><pre>{html.escape(report)}</pre>''',encoding='utf-8')
print(out/'index.html')
