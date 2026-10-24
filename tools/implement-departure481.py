from pathlib import Path
import zipfile,json,hashlib
files=['src/main.c','src/cockpit-computer-controls.h','src/cockpit-hud.h','src/station-departure-view.h','src/cockpit-charms.h','src/dashboard-cosmetics.h','src/twin-stack.h','src/weapon-fx.h']
with zipfile.ZipFile('design/departure481/baseline480.zip','w',zipfile.ZIP_DEFLATED) as z:
 for f in files:z.write(f,f)
p=Path('src/cockpit-computer-controls.h');s=p.read_text();a=s.index('static int hc_context(void)');b=s.index('static int hc_detected(',a)
s=s[:a]+'''/* Presentation is allowed during the guided launch; manual computers and
   targeting controls still require ordinary free flight. */
static int hc_presentation_context(void){return !hc_force_legacy&&hc_space_depth<2&&page==FLIGHT&&!game.docked&&!game.dead&&game.planet<0&&game.jump<=0&&(game.dock_stage==0||game.dock_stage==4)&&game.approach<0&&!game.police_stop&&!race_is_active(&game);}
static int hc_context(void){return hc_presentation_context()&&!game.dock_stage;}
static int hc_full_console_visible(void){return hc_presentation_context()&&(paused||hc_browser||tools_open||(hud_mode==0&&!hud_hidden));}
'''+s[b:];p.write_text(s)
p=Path('src/cockpit-hud.h');s=p.read_text().replace('key[45]=hud_hidden;','key[45]=hud_hidden;key[46]=game.dock_stage;').replace('if(!hc_context()){cockpit_legacy();return;}','if(!hc_presentation_context()){cockpit_legacy();return;}');p.write_text(s)
for name in ['cockpit-charms.h','dashboard-cosmetics.h','twin-stack.h','weapon-fx.h']:
 p=Path('src')/name;s=p.read_text().replace('hc_context()','hc_presentation_context()');p.write_text(s)
p=Path('src/main.c');s=p.read_text();a=s.index('  hud_animation_tick();hud_canopy_feedback();hc_motion_tick();\n  if(hc_repaint_needed())');b=s.index('camera_override_clear();render_phase(6);return;',a)
body=s[a:b];body=body.replace('  ',' ',1)
helper='static void hc_flight_overlay(void){\n charm_draw();\n'+body+'\n}\n'
s=s.replace('#include "station-departure-view.h"','static void hc_flight_overlay(void);\n#include "station-departure-view.h"')
s=s.replace('#include "dashboard-cosmetics.h"','#include "dashboard-cosmetics.h"\n'+helper)
a=s.index(' charm_draw();\n if(hc_full_console_visible()){',s.index('static void space_render(void){'));b=s.index('\n }',a)+3
s=s[:a]+''' if(hc_full_console_visible()){
  hc_flight_overlay();camera_override_clear();render_phase(6);return;
 }'''+s[b:];p.write_text(s)
p=Path('src/station-departure-view.h');s=p.read_text().replace(' cockpit();autopilot_annunciator(34,game.dock_timer);',' weapon_fx_fire();\n if(hc_full_console_visible())hc_flight_overlay();\n autopilot_annunciator(40,game.dock_timer);');
# weapon renderer is included after this header; declare its existing entry point.
s='static void weapon_fx_fire(void);\n'+s;p.write_text(s)
