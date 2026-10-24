"""Preserve native capture evidence and update the individual catalogue ledger."""
from pathlib import Path
import argparse,json,re,hashlib,html
from PIL import Image
root=Path(__file__).resolve().parents[1]
ap=argparse.ArgumentParser();ap.add_argument('--tag',required=True);ap.add_argument('--entry',type=int,required=True);ap.add_argument('--slug',required=True);args=ap.parse_args()
assert re.fullmatch(r'[a-zA-Z0-9_-]+',args.tag) and re.fullmatch(r'[a-zA-Z0-9_-]+',args.slug)
run=root.parent/'erl-implementation-candidate/outputs/magnetic-slice'/f'regression-{args.tag}-diagnostic-encounter-flight-capture'
report=(run/'encounter-flight-capture.txt').read_text();binding=json.loads((run/'binding.json').read_text(encoding='utf-8-sig'))
assert 'RESULT 0 failures' in report and 'starves 0 drops 0' in report
assert hashlib.sha256((run/'EBOOT.PBP').read_bytes()).hexdigest().upper()==binding['eboot_sha256'].upper()
fps=[float(x) for x in re.findall(r'orbit ([0-9.]+) FPS',report)];assert len(fps)==3 and min(fps)>=29.5
out=root/'assets/generated/encounter-upgrades'/args.slug;out.mkdir(exist_ok=True)
frames=[]
for p in sorted(run.glob('encounter-retail-*.bmp'),key=lambda p:int(p.stem.rsplit('-',1)[1])):
 im=Image.open(p);assert im.size==(480,272);im.convert('RGB').save(out/(p.stem+'.png'));frames.append(p.stem+'.png')
assert len(frames)>=3
(out/'verification.txt').write_text(report);(out/'binding.json').write_text(json.dumps(binding,indent=2))
p=root/'design/encounter-catalogue-progress.json';ledger=json.loads(p.read_text());entry=ledger['entries'][args.entry]
entry.update(status='implemented; native visual/orbit checks passed; physical PSP pending',fps=fps,evidence=str(out.relative_to(root)),eboot_sha256=binding['eboot_sha256'])
p.write_text(json.dumps(ledger,indent=2))
name=html.escape(entry['name'])
body=f'<h1>{name}</h1><p>Actual 480×272 PSP renderer. Camera angles from an isolated native test fixture; normal encounter placement is unchanged.</p><p>Near / medium / distant: {fps} FPS averaged in PPSSPP. Occasional long frames remain; physical PSP testing pending.</p>'
body+=''.join(f'<figure><img width="480" height="272" src="{html.escape(f)}"><figcaption>Orbit capture {html.escape(f)}</figcaption></figure>' for f in frames)
(out/'index.html').write_text('<!doctype html><meta charset="utf-8"><title>'+name+'</title><style>body{background:#091018;color:#d8e6ed;font:16px system-ui;max-width:1000px;margin:32px auto}figure{display:inline-block;margin:10px 8px 18px 0}img{image-rendering:pixelated;display:block}figcaption{font-size:12px;color:#9ab4c4}</style>'+body,encoding='utf-8')
print(entry['name'],fps,str(out/'index.html'))
