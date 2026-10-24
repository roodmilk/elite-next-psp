"""Convert every emulator page to native PNG and build an unscaled review desk."""
import argparse, hashlib, html, json
from pathlib import Path
from PIL import Image
R=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser();parser.add_argument('capture_dir',type=Path);a=parser.parse_args();folder=a.capture_dir.resolve()
papers=json.loads((R/'content/newspapers/collection.json').read_text(encoding='utf-8'))
expected=[(f"news-s{p['system']:03d}-p{n:02d}",p['name'],p['identity']['masthead'],n) for p in papers for n,_ in enumerate(p['pages'])]
expected += [(f'news-earth-p{n:02d}','SOL','The Earth Daily',n) for n in range(4)]
entries=[];missing=[]
for stem,system,title,n in expected:
    path=folder/(stem+'.bmp')
    if not path.exists():path=folder/(stem+'.png')
    if not path.exists():missing.append(stem);continue
    image=Image.open(path);image.load();assert image.size==(480,272),(stem,image.size)
    dest=path.with_suffix('.png');image.save(dest,optimize=True)
    entries.append(dict(id=stem,system=system,title=title,index=n,image=dest.name,sha256=hashlib.sha256(image.tobytes()).hexdigest(),native_render='480x272 verified',manual_inspection='pending'))
for stem in ('news-sudoku-zoom','news-crossword-zoom'):
    path=folder/(stem+'.bmp')
    if not path.exists():path=folder/(stem+'.png')
    if path.exists():Image.open(path).save(path.with_suffix('.png'))
(folder/'native-render-manifest.json').write_text(json.dumps(dict(entries=entries,missing=missing,certified=False),indent=2)+'\n',encoding='utf-8')
# Fixed 480px image size, never CSS scaling. Review marks remain explicitly local.
page='''<!doctype html><meta charset="utf-8"><title>ELITE: NEXT newspaper review desk</title>
<style>body{background:#0e1722;color:#e7dfca;font:16px system-ui;margin:24px}main{width:520px}img{display:block;width:480px;height:272px;image-rendering:pixelated;margin:16px 0}button{padding:8px;margin:4px}input{width:480px;padding:8px}small{color:#9eb6c8}</style>
<h1>Native newspaper review desk</h1><p>Each image is exactly 480×272. Browser zoom: 100%. Draft collection; rendering does not certify prose.</p>
<main><div id="name"></div><img id="shot"><div id="state"></div><input id="note" placeholder="Review notes"><br>
<button onclick="go(-1)">Previous</button><button onclick="mark('needs work')">Needs work</button><button onclick="mark('visually inspected')">Inspected</button><button onclick="go(1)">Next</button><button onclick="download()">Export review notes</button></main>
<script>const pages=__DATA__;let index=0;let notes=JSON.parse(localStorage.getItem('elite-newspaper-native-review')||'{}');
function show(){const p=pages[index],old=notes[p.id],checked=old?.hash===p.sha256?old:null;document.querySelector('#shot').src=p.image;document.querySelector('#name').textContent=p.system+' — '+p.title+' — proof '+p.id;document.querySelector('#state').textContent=checked?.status||(old?'Image changed; inspect again':'Pending inspection');document.querySelector('#note').value=checked?.note||'';}
function go(d){index=(index+d+pages.length)%pages.length;show()}
function mark(status){const p=pages[index];notes[p.id]={status,note:document.querySelector('#note').value,hash:p.sha256};localStorage.setItem('elite-newspaper-native-review',JSON.stringify(notes));show()}
function download(){const a=document.createElement('a');a.href=URL.createObjectURL(new Blob([JSON.stringify(notes,null,2)],{type:'application/json'}));a.download='manual-newspaper-review.json';a.click()}
addEventListener('keydown',e=>{if(e.target.tagName==='INPUT')return;if(e.key==='ArrowDown'||e.key==='ArrowRight')go(1);if(e.key==='ArrowUp'||e.key==='ArrowLeft')go(-1)});show();</script>'''
(folder/'index.html').write_text(page.replace('__DATA__',json.dumps(entries)),encoding='utf-8')
print(f'{len(entries)} native pages converted; {len(missing)} missing. Manual review remains pending. Gallery: {folder / "index.html"}')
raise SystemExit(bool(missing))
