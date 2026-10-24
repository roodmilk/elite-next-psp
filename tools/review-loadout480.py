from pathlib import Path
p=Path('src/main.c');s=p.read_text().replace('#include "dashboard-review.h"','#include "dashboard-review.h"\n#include "loadout-picker-review.h"');s=s.replace('if(dashboard_review()||','if(loadout_picker_review()||dashboard_review()||');p.write_text(s)
p=Path('tools/run-menu-prototypes.ps1');s=p.read_text().replace("'charm-review','dashboard-review'","'charm-review','dashboard-review','loadout-picker-review'");p.write_text(s)
