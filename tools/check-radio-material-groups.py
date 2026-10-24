"""Regression for the expanded atlas/Adder collision and sparse group ordering."""
from pathlib import Path
import json,random,re
root=Path(__file__).resolve().parents[1]
base=int(re.search(r'ENCOUNTER_TILE_BASE (\d+)',(root/'src/encounter-art-ids.h').read_text()).group(1))
old_tiles=len((root/'src/encounter-art-tiles.h').read_text().splitlines())
radio=json.loads((root/'assets/generated/radio-shack-rework/surface-map.json').read_text())
assert radio['base']==base+old_tiles
nine=json.loads((root/'assets/generated/nine-encounter-reworks/surface-map.json').read_text());assert nine['base']==base+old_tiles+radio['tile_count']
count=nine['base']+nine['tile_count'];flat=(count+52)*3;groups=flat+1
fleet={tile*3+level for tile in range(count) for level in range(3)}
adder={(count+tile)*3 for tile in range(52)}
assert not fleet&adder and flat not in fleet|adder
assert max(fleet|adder)<flat<32767 and 65536+2*count<2147483647
assert 'FLEET_QUAD_BASE 65536' in (root/'src/ship-art-raster.h').read_text()
assert 'material=5200+' not in (root/'src/shipyard-console.h').read_text()
assert 'material=FLEET_QUAD_BASE+' in (root/'src/shipyard-console.h').read_text()
for tile in range(count):
 for half in range(2):
  mat=65536+tile*2+half;assert (mat-65536)//2==tile and (mat-65536)&1==half
  assert not 7800<=mat<7904 and mat>=65536
# Legacy fitted quad addresses and new addresses must choose the same UV half.
# Racer addresses keep their separate tile selection and existing parity.
for tile in range(radio['base']+radio['tile_count']):
 for half in (0,1):
  old=5200+tile*6+half;new=65536+tile*2+half
  assert (old-5200)//6==(new-65536)//2 and (old-5200)&1==(new-65536)&1
for mat in range(7800,7804):
 assert ((mat-5200)&1)==((mat-65536)&1)
def sparse_order(keys):
 counts=[0]*groups;used=[0]*((groups+31)//32)
 for g in keys:
  assert 0<=g<groups;counts[g]+=1;used[g>>5]|=1<<(g&31)
 active=[];offsets={};total=0
 for word,bits in enumerate(used):
  while bits:
   bit=(bits&-bits).bit_length()-1;g=word*32+bit;bits&=bits-1
   active.append(g);offsets[g]=total;total+=counts[g]
 return active,offsets,total
rng=random.Random(476)
cases=[[],[flat],[0,31,32,63,64,flat],sorted(fleet|adder|{flat})]
for _ in range(256):cases.append([rng.choice(tuple(fleet|adder|{flat})) for _ in range(rng.randrange(1,2049))])
for keys in cases:
 active,offsets,total=sparse_order(keys);expected=sorted(set(keys));assert active==expected and total==len(keys)
 cursor=0
 for g in expected:assert offsets[g]==cursor;cursor+=keys.count(g)
print(f'PASS {len(cases)} sparse batch ordering/bounds cases; {count} atlas tiles,52 disjoint Adder tiles; no fixed180-group assumption')
