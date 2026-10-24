from pathlib import Path
import zipfile
files=['src/main.c','src/ui-modern.h','src/loadout-console.h','src/dockbay-pages.h','src/cockpit-computer-controls.h','src/cockpit-hud.h']
with zipfile.ZipFile('design/loadout480/baseline479.zip','w',zipfile.ZIP_DEFLATED) as z:
 for f in files:z.write(f,f)
p=Path('src/main.c');s=p.read_text();s=s.replace('static int shipyard_stats_view=1;','static int loadout_picker_slot=-1,loadout_picker_row=0;\nstatic int shipyard_stats_view=1;');s=s.replace('static void change_page(int p){','static void change_page(int p){\n loadout_picker_slot=-1;')
needle='  int saga_choices=tracked_mission'
insert='''  if(page==INVENTORY&&loadout_picker_slot>=0){
   int list[EQUIPMENT_COUNT+DB_ITEMS],n=loadout_picker_list(list,EQUIPMENT_COUNT+DB_ITEMS);
   if(pressed&PSP_CTRL_CIRCLE){loadout_picker_slot=-1;game.cue=SFX_UI;return;}
   if(n&&(pressed&(PSP_CTRL_UP|PSP_CTRL_DOWN))){loadout_picker_row=(loadout_picker_row+(pressed&PSP_CTRL_DOWN?1:n-1))%n;game.cue=SFX_SELECT;}
   if(n&&(pressed&PSP_CTRL_CROSS)){if(loadout_picker_row>=n)loadout_picker_row=n-1;if(module_fit_token(list[loadout_picker_row],loadout_picker_slot))loadout_picker_slot=-1;else{snprintf(loadout_store_note,sizeof(loadout_store_note),"%s",game.message);loadout_store_until=preview_time+4.f;}}
   return;
  }
'''
s=s.replace(needle,insert+needle);s=s.replace('  else if(page==INVENTORY&&(pressed&PSP_CTRL_TRIANGLE))','  else if(page==INVENTORY&&(pressed&PSP_CTRL_CROSS)){if(!game.docked)message(&game,"Dock to change fitted modules.");else if(row>=0&&row<FIT_SLOTS&&fit_slot_open(&game,row)){loadout_picker_slot=row;loadout_picker_row=0;loadout_store_until=-1;game.cue=SFX_UI;}}\n  else if(page==INVENTORY&&(pressed&PSP_CTRL_TRIANGLE))')
p.write_text(s)
p=Path('src/ui-modern.h');s=p.read_text();needle='#include "loadout-console.h"';s=s.replace(needle,'''static int module_fit_token(int token,int slot);
static int loadout_picker_list(int*out,int cap){int all[EQUIPMENT_COUNT+DB_ITEMS],n=module_owned_list(all,EQUIPMENT_COUNT+DB_ITEMS),count=0;if(loadout_picker_slot<0||loadout_picker_slot>=FIT_SLOTS)return 0;for(int i=0;i<n&&count<cap;i++)if(equip_slot_for(module_item_base(all[i]))==loadout_picker_slot%6)out[count++]=all[i];return count;}
'''+needle)
a=s.index('static int module_swap_selected(void){');b=s.index('\nstatic int module_sell_selected',a);old=s[a:b];old=old.replace('static int module_swap_selected(void){\n int token=module_selected_item(),item=module_item_base(token);','static int module_fit_token(int token,int slot){\n if(token<0||slot<0||slot>=FIT_SLOTS||!fit_slot_open(&game,slot)||equip_slot_for(module_item_base(token))!=slot%6)return 0;\n int item=module_item_base(token);');old=old.replace(' int cat=equip_slot_for(item),cap=fit_capacity(game.ship,cat);if(module_bank<0)module_bank=0;if(module_bank>=cap)module_bank=cap-1;int slot=cat+module_bank*6;',' int cat=slot%6;');s=s[:a]+old+'''\nstatic int module_swap_selected(void){int token=module_selected_item();if(token<0)return 0;int cat=equip_slot_for(module_item_base(token)),cap=fit_capacity(game.ship,cat);if(module_bank<0)module_bank=0;if(module_bank>=cap)module_bank=cap-1;return module_fit_token(token,cat+module_bank*6);}
'''+s[b:];p.write_text(s)
p=Path('src/loadout-console.h');s=p.read_text();pos=s.index('static void loadout_console_draw(void)');s=s[:pos]+'''static void loadout_picker_draw(void){
 int list[EQUIPMENT_COUNT+DB_ITEMS],n=loadout_picker_list(list,EQUIPMENT_COUNT+DB_ITEMS);
 if(loadout_picker_row<0)loadout_picker_row=0;if(n&&loadout_picker_row>=n)loadout_picker_row=n-1;
 lc_plate(242,59,232,188);lc_inset(247,63,222,179);
 text_px(253,67,LC_GOLD,"ITEM STORAGE / %s",lc_cats[loadout_picker_slot%6]);
 text_px(253,80,LC_CYAN,"FIT TO BANK %d  / %d ITEMS",loadout_picker_slot/6+1,n);
 line(252,91,464,91,LC_EDGE);
 int first=loadout_picker_row<5?0:loadout_picker_row-4;
 for(int k=0;k<5&&first+k<n;k++){int token=list[first+k],y=98+k*19,sel=first+k==loadout_picker_row;
  if(sel){rect(251,y-2,213,18,LC_RAISE);rect(251,y-2,2,18,LC_GOLD);}
  tech_module_icon(256,y-1,module_item_base(token));module_rare_shine(256,y-1,16,16,token);
  text_px(277,y+3,cosmetic_ink(module_item_base(token),sel?LC_GOLD:LC_INK),"%.23s",module_item_short(token));
 }
 if(!n){lc_copy(253,101,25,3,LC_DIM,"No compatible stored items. Buy modules in Outfitting.",1);}
 line(252,195,464,195,LC_EDGE);
 if(n){int token=list[loadout_picker_row];lc_copy(253,202,26,3,cosmetic_ink(module_item_base(token),LC_INK),module_item_name(token),1);text_px(253,234,LC_DIM,"%d/%d   OWNED %d",loadout_picker_row+1,n,module_item_count(token));}
 rect(8,250,464,18,LC_PANEL);lc_hint(14,'P',"ITEM");if(n)lc_hint(120,'X',"FIT");lc_hint(220,'O',"CANCEL");
}
'''+s[pos:];s=s.replace('lc_hint(14,\'P\',"SLOT");lc_hint(105,\'S\',"STORE");lc_hint(225,\'T\',"ITEM STORAGE");lc_hint(396,\'O\',"BACK");','lc_hint(14,\'P\',"SLOT");lc_hint(96,\'X\',"FIT");lc_hint(168,\'S\',"STORE");lc_hint(260,\'T\',"STORAGE");lc_hint(396,\'O\',"BACK");\n if(loadout_picker_slot>=0)loadout_picker_draw();');p.write_text(s)
p=Path('src/cockpit-computer-controls.h');s=p.read_text();s=s.replace('if(pressed&PSP_CTRL_SQUARE){hc_browser=!hc_browser;hc_open_age=0;hc_refresh=0;if(hc_browser){hc_highlight=selected_target;hc_contacts();}game.cue=SFX_UI;}','if(!(held&PSP_CTRL_SQUARE)){int was=hc_browser;hc_browser=0;hc_open_age=0;if(was){hc_cache_dirty=1;return 1;}return 0;}\n if(!hc_browser){hc_browser=1;hc_open_age=0;hc_refresh=0;hc_highlight=selected_target;hc_contacts();hc_cache_dirty=1;game.cue=SFX_UI;}');p.write_text(s)
