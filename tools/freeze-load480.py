from pathlib import Path
import hashlib,json,zipfile
files=['src/main.c','src/ui-modern.h','src/loadout-console.h','src/dockbay-pages.h','src/cockpit-computer-controls.h','src/cockpit-hud.h','src/voyage.h','src/game.c','src/save-safety.h','src/encounter-flight-capture.h','src/loadout-picker-review.h','src/cockpit-hud-review.h']
hashes={f:hashlib.sha256(Path(f).read_bytes()).hexdigest() for f in files}
Path('design/loadout480/final-source-hashes.json').write_text(json.dumps(hashes,indent=2))
with zipfile.ZipFile('design/loadout480/final-source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for f in files:z.write(f,f)
