from pathlib import Path
from PIL import Image
r=Path('../erl-implementation-candidate/outputs/magnetic-slice/regression-load480-v1-diagnostic-loadout-picker-review')
o=Path('assets/generated/loadout480')
frames=[Image.open(p).convert('RGBA') for p in sorted(r.glob('hud-motion-*.bmp'))]
frames[0].save(o/'hud-motion.png',save_all=True,append_images=frames[1:],duration=[33,33,34]*30,loop=0,disposal=0,blend=0)
frames[25].save(o/'hud-held.png');frames[60].save(o/'speed-ticker.png')
