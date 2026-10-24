from pathlib import Path
p=Path('src/dockbay-pages.h');s=p.read_text().replace('if(page==FLIGHT&&game.dockbay.dirty&&db_save_wait<=0)db_commit();','if((page==FLIGHT||game.docked)&&game.dockbay.dirty&&db_save_wait<=0)db_commit();');p.write_text(s)
