from pathlib import Path
p=Path('src/main.c');s=p.read_text();needle='  float jump_before=game.jump;'
insert='''  /* The staged expanded-computer benchmark must supply the same held
     buttons as a player. Releasing Square on every test frame would otherwise
     force a close/open and full cache repaint unrelated to real browsing. */
  if(encounter_capture_scene>=0&&encounter_capture_hud>=0&&encounter_capture_hud<9){int m=encounter_capture_hud;pad.Buttons=(m==1||m==3||m==4||m==6?PSP_CTRL_SQUARE:0)|(m>=1&&m<=5?PSP_CTRL_CIRCLE:0);pressed=frames?0:pad.Buttons;}
'''
assert s.count(needle)==1;s=s.replace(needle,insert+needle);p.write_text(s)
