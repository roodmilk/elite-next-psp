from pathlib import Path
from PIL import Image
r=Path('../erl-implementation-candidate/outputs/magnetic-slice/regression-load480-v2-diagnostic-loadout-picker-review')
o=Path('assets/generated/loadout480')
for name in ['picker-weapons','picker-empty','picker-cosmetic-limit']:
 Image.open(r/(name+'.bmp')).save(o/(name+'.png'))
