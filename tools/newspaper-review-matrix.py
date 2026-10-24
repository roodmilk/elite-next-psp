"""Select native proofs for real human/agent inspection; never approve them.

Each contact-sheet tile retains its original 480x272 pixels. This matrix is a
representative gate and does not replace inspecting the remaining pages.
"""
import argparse,json
from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[1]
parser=argparse.ArgumentParser();parser.add_argument('folder',type=Path);a=parser.parse_args();folder=a.folder
papers=json.loads((root/'content/newspapers/collection.json').read_text())
chosen=[];seen=set();families=set();palettes=set();layouts=set();archetypes=set()
def choose(p,n):
    stem=f"news-s{p['system']:03d}-p{n:02d}"
    if stem in seen:return
    chosen.append(stem);seen.add(stem);families.add(p['identity']['publication_family']);palettes.add(p['identity']['palette']);layouts.add(p['pages'][n]['layout']);archetypes.add(p['archetype'])
for p in papers:
    if p['name'] in ('LAVE','DISO','REORTE','LEESTI','QUATOR','RIEDQUAT','TIONISLA'):choose(p,0)
for n in range(16):choose(papers[7],n)
for p in papers:
    for n,page in enumerate(p['pages']):
        if p['identity']['publication_family'] not in families or p['identity']['palette'] not in palettes or page['layout'] not in layouts or p['archetype'] not in archetypes:choose(p,n)
for p in (papers[1],papers[255]):
    for n in (0,len(p['pages'])//2,len(p['pages'])-1):choose(p,n)
chosen += [f'news-earth-p{n:02d}' for n in range(4)]+['news-sudoku-zoom','news-crossword-zoom']
assert len(families)==16 and len(palettes)==24 and len(layouts)==16 and len(archetypes)==12
(folder/'selected-native-review.json').write_text(json.dumps(chosen,indent=2)+'\n')
for start in range(0,len(chosen),4):
    sheet=Image.new('RGB',(960,544),(5,12,22))
    for k,stem in enumerate(chosen[start:start+4]):
        path=folder/(stem+'.png')
        if not path.exists():path=folder/(stem+'.bmp')
        im=Image.open(path).convert('RGB');assert im.size==(480,272)
        sheet.paste(im,((k%2)*480,(k//2)*272))
    sheet.save(folder/f'proof-{start//4:02d}.png')
print(f'{len(chosen)} selected native proofs; 16 families, 24 palettes, 16 layouts, 12 archetypes. Inspection pending.')
