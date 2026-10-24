from pathlib import Path
from PIL import Image
import json
root=Path(__file__).resolve().parents[1]
run=root.parent/'erl-implementation-candidate/outputs/magnetic-slice/regression-menu-options-v1-diagnostic-menu-prototypes'
out=root/'assets/concepts/main-menu/in-engine'
out.mkdir(parents=True,exist_ok=True)
names=['01-classic-pulse','02-sliding-console','03-minimal-brackets','04-centred-cinema','05-amber-instruments','06-violet-scanner','07-floating-strips','08-horizontal-cards']
for i,name in enumerate(names):
    frames=[Image.open(run/f'menu-{i:02d}-{f:02d}.bmp').convert('RGB') for f in range(32)]
    assert all(x.size==(480,272) for x in frames)
    frames[8].save(out/f'{name}.png')
    frames[0].save(out/f'{name}.gif',save_all=True,append_images=frames[1:],duration=125,loop=0,optimize=False)
print(out)
print('8 PNG stills and 8 GIF animations, all 480x272')
