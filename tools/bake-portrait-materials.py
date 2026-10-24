"""Authored source-pixel anatomy, including necks and collars, for all ten rigs.
No image colour thresholds, rectangular colour overlays or runtime polygons.
Inclusive spans below are traced against tools/portrait-source-grids.py.
"""
from pathlib import Path
import re
root=Path(__file__).resolve().parents[1]
hair_values=[int(v,16) for v in re.findall(r'0x([0-9a-f]+)ULL',(root/'src/portrait-hair-masks.h').read_text())]
def spans(*segments):
 rows=[0]*64
 for ya,yb,xa,xb in segments:
  for y in range(ya,yb+1): rows[y]|=sum(1<<x for x in range(xa,xb+1))
 return rows
head=[
 spans((0,43,0,63),(44,44,25,48),(45,45,25,47),(46,48,25,46),(49,50,25,34),(51,52,26,35),(53,54,27,36)),
 spans((0,51,0,63),(52,63,23,36)),
 spans((18,38,0,63),(39,42,0,44),(43,46,0,42),(47,49,0,39),(50,52,18,36),(53,55,20,33),(56,58,23,31),(59,60,24,29),(61,63,25,28)),
 spans((0,48,0,63),(49,50,24,39),(51,54,24,34),(55,58,24,33),(59,63,27,32)),
 spans((6,44,0,63),(45,46,25,41),(47,49,25,40),(50,53,27,40),(54,55,30,40),(56,56,33,40),(57,57,35,39)),
 spans((0,48,0,63),(49,51,19,43),(52,54,21,42),(55,57,24,41),(58,60,28,40),(61,63,30,39)),
 spans((0,45,0,63),(46,46,24,42),(47,47,24,39),(48,49,25,37),(50,51,27,36),(52,52,29,34)),
 spans((0,42,0,63),(43,43,19,44),(44,44,19,42),(45,47,19,37),(48,48,20,37),(49,49,22,37),(50,50,27,36)),
 spans((11,53,0,63),(54,54,28,45),(55,57,31,43)),
 spans((0,45,0,63),(46,49,23,46),(50,51,25,45),(52,53,29,43)),
]
hair=[hair_values[:64],spans(),spans(),
 spans((0,17,0,63),(18,19,0,25),(20,22,0,23),(23,25,0,16),(18,25,41,63)),
 spans((6,15,0,63),(16,19,0,29),(20,26,0,25),(27,44,0,24),(45,47,0,23)),
 head[5].copy(),
 spans((16,35,10,23)),hair_values[64:],
 spans((11,24,0,63),(25,25,0,33),(26,29,0,30),(30,51,0,25),(25,29,45,63),(30,51,47,63),(52,53,0,25),(52,53,47,63)),
 spans((0,19,0,63),(20,26,0,29),(20,26,43,63),(27,31,0,23),(27,31,49,63))]
# Beak: the remaining avian head is feather material, including its long neck.
beak=spans((24,27,46,50),(28,30,44,52),(31,35,43,54),(36,40,46,55),(41,43,51,54))
hair[5]=[a&~b for a,b in zip(hair[5],beak)]
eyes=[spans((26,26,35,39),(27,27,46,48)),
 spans((25,32,28,34),(29,35,42,45)),
 spans((22,22,25,29),(23,23,24,31),(24,33,22,32),(34,34,24,31),(35,35,26,29)),
 spans((26,28,19,25),(29,40,18,29),(41,43,22,29),(29,40,44,47)),
 spans((21,24,35,39),(25,26,37,40)),
 spans((20,27,35,41),(23,28,40,42)),
 spans((21,27,35,40),(24,28,48,50)),
 spans((23,24,32,39),(25,25,35,38),(25,27,45,47)),
 spans((33,35,30,31),(32,37,37,41)),spans()]
lips=[spans((38,39,40,46)),spans((44,45,35,39)),
 spans((38,39,11,23),(40,40,23,29)),
 spans((46,48,30,35)),spans((35,36,31,46),(37,38,46,53)),
 spans(),spans((35,36,40,46),(39,40,39,45)),
 spans((35,37,36,43)),spans((44,49,31,42)),spans()]
values=[int(v,16) for v in re.findall(r'0x([0-9A-Fa-f]{4})',(root/'src/portrait-atlas.h').read_text())]
bases=(0,16,24,32,40,48,64,76,56,88);miny=(0,0,18,0,6,0,0,0,11,0)
out=['/* Generated reviewed skin/crest/clothing/eye/lip source-pixel masks. */','static const uint64_t portrait_material_rows[10][5][64]={']
for sp in range(10):
 layers=[[0]*64 for _ in range(5)]
 for y in range(miny[sp],64):
  for x in range(64):
   if not values[bases[sp]*4096+y*64+x]&32768:continue
   bit=1<<x;layer=0 if head[sp][y]&bit else 2
   if hair[sp][y]&head[sp][y]&bit:layer=1
   if eyes[sp][y]&bit:layer=3
   if lips[sp][y]&bit:layer=4
   layers[layer][y]|=bit
 out.append(' {')
 for layer in layers:
  out.append('  {')
  out.extend('   '+','.join(f'0x{v:016x}ULL' for v in layer[i:i+4])+',' for i in range(0,64,4))
  out.append('  },')
 out.append(' },')
out+=['};','']
(root/'src/portrait-material-masks.h').write_text('\n'.join(out),encoding='ascii')
print('Baked 10 reviewed material rigs; skin includes neck, clothing excludes it.')
