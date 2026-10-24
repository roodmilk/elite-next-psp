from pathlib import Path
p=Path('src/cockpit-hud.h');s=p.read_text().replace('17+(int)(14*(1-age/.6f))','17+(int)(7*(1-age/.6f))');s=s.replace(' unsigned signal=(game.voice_time',' if(tools_open&&tools_confirm_time>0){int radius=14+(int)(5*fminf(1,tools_confirm_time/.36f));hc_circle_fast(359,112,radius,tools_weapon_result<0?RED:HC_GOLD);}\n unsigned signal=(game.voice_time');p.write_text(s)
