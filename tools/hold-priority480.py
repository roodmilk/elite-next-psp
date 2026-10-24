from pathlib import Path
p=Path('src/cockpit-computer-controls.h');s=p.read_text();a=s.index('static int hc_browser_input(');b=s.index(' hc_open_age+=',a)
head='''static int hc_browser_hold(unsigned held){
 int was=hc_browser;
 if(!hc_context()||!(held&PSP_CTRL_SQUARE)){hc_browser=0;hc_open_age=0;if(was)hc_cache_dirty=1;return was;}
 if(!comms_quick&&!(held&PSP_CTRL_START)&&!hc_browser){hc_browser=1;hc_open_age=0;hc_refresh=0;hc_highlight=selected_target;hc_contacts();hc_cache_dirty=1;game.cue=SFX_UI;}
 return hc_browser||was;
}
static int hc_browser_input(unsigned pressed,unsigned held,float dt){
 hc_validate_lock();int consumed=hc_browser_hold(held);
 if(!hc_browser||comms_quick||(held&PSP_CTRL_START))return consumed;
'''
s=s[:a]+head+s[b:];p.write_text(s)
p=Path('src/main.c');s=p.read_text().replace(' if(pressed)hc_cache_dirty=1;hc_validate_lock();if(!hc_context())hc_browser=0;',' if(pressed)hc_cache_dirty=1;hc_validate_lock();hc_browser_hold(held);');p.write_text(s)
p=Path('src/loadout-picker-review.h');s=p.read_text().replace('input(0,0,.016f,0,0);\n if(pixels){hc_test_setup();','input(PSP_CTRL_CIRCLE,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.016f,0,0);input(0,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.21f,0,0);PC(hc_browser&&tools_open,"both held computers remain available together");input(0,PSP_CTRL_CIRCLE,.016f,0,0);PC(!hc_browser&&tools_open,"Square release closes targeting even while Circle tools consume input");input(0,0,.016f,0,0);\n if(pixels){hc_test_setup();');p.write_text(s)
