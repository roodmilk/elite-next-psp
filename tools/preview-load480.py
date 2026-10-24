from pathlib import Path
from PIL import Image
r=Path('../erl-implementation-candidate/outputs/magnetic-slice/regression-load480-v1-diagnostic-loadout-picker-review')
o=Path('assets/generated/loadout480');o.mkdir(parents=True,exist_ok=True)
for name in ['picker-weapons','picker-empty']:
 Image.open(r/(name+'.bmp')).save(o/(name+'.png'))
