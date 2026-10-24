from pathlib import Path
from PIL import Image,ImageDraw
import re,collections
from encounter_panel_design import panel, ROLES
import csv
root=Path.cwd();p=root/'src/fleet-detail-atlas.h';s=p.read_text();m=re.search(r'(fleet_tiles\[\]\[672\]=\{)(.*?)(\};)',s,re.S);body=m.group(2)
if '#include "encounter-art-tiles.h"' in body: body=body.split(',\n#include "encounter-art-tiles.h"')[0]
font_text=(root/'src/font8.h').read_text();glyphs=[[int(x.rstrip('uU'),16) for x in re.findall(r'0x[0-9a-fA-F]+u?',row)] for row in re.findall(r'\{([^{}]+)\}',font_text.split('font8[95][8]=',1)[1])[:95]]
base=len(re.findall(r'\{[^{}]*\}',body));tiles=[];previews=[]
for family in range(18):
 for role in range(4):
  im=panel(family,role)
  arr=list(im.getdata());data=[]
  for size in (32,16,8):
   step=32//size;small=[]
   for y in range(size):
    for x in range(size):small.append(collections.Counter(arr[(y*step+dy)*32+x*step+dx] for dy in range(step) for dx in range(step)).most_common(1)[0][0])
   data += [small[i]|small[i+1]<<4 for i in range(0,len(small),2)]
  tiles.append('{'+','.join(map(str,data))+'}');previews.append(im)
(root/'src/encounter-art-tiles.h').write_text(',\n'.join(tiles)+'\n')
extra=',\n#include "radio-shack-tiles.h"\n' if (root/'src/radio-shack-tiles.h').exists() else ''
if (root/'src/nine-encounter-tiles.h').exists():extra+=',\n#include "nine-encounter-tiles.h"\n'
s=s[:m.start(2)]+body+',\n#include "encounter-art-tiles.h"\n'+extra+s[m.end(2):];p.write_text(s)
(root/'src/encounter-art-ids.h').write_text(f'#define ENCOUNTER_TILE_BASE {base}\n#define ENCOUNTER_TILE(family,role) (ENCOUNTER_TILE_BASE+(family)*4+(role))\n')
out=root/'assets/generated/encounter-upgrades';out.mkdir(parents=True,exist_ok=True)
palette=[(7,14,22),(22,34,45),(52,73,85),(100,130,145),(163,192,197),(220,159,62),(12,29,35),(13,45,65),(20,84,105),(160,210,222),(60,220,239),(79,157,114),(69,80,89),(185,195,199),(222,226,198),(83,107,120)]
board=Image.new('RGB',(32*4,32*18))
for i,im in enumerate(previews):im.putpalette(sum((list(c) for c in palette),[])+[0]*720);board.paste(im.convert('RGB'),((i%4)*32,(i//4)*32))
board.resize((512,2304),Image.Resampling.NEAREST).save(out/'panel-atlas.png')
print('Dedicated 32x32 plates:',len(tiles),'base',base)

with (out/'panel-purpose.csv').open('w',newline='') as f:
 w=csv.writer(f);w.writerow(['Object','Panel','Purpose','Texture size'])
 for family,name in enumerate(['Outer relay','Frontier outpost','Animal carrier','Sanctuary','Cinema','Closed resort','Mobile mechanic','Asteroid foundation','Salvage village','Shipbreakers yard','Community greenhouse','Patchwork fuel stop','Old passenger ferry','Local radio shack','Miners tea barge','Cooperative ore depot','Parts exchange','Beacon repair crew']):
  for role,purpose in enumerate(ROLES[family]):w.writerow([name,role,purpose,'32x32 with 16x16 and 8x8 mipmaps'])

# Regenerate continuous radio paintings when their atlas base changes.
if (root/'src/radio-shack-tiles.h').exists():
 import subprocess,sys
 subprocess.run([sys.executable,str(root/'tools/generate-radio-shack-detailed.py')],check=True)

if (root/'src/nine-encounter-tiles.h').exists():
 subprocess.run([sys.executable,str(root/'tools/generate-nine-reworks.py')],check=True)
