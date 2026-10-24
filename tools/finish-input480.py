from pathlib import Path
p=Path('src/loadout-console.h');s=p.read_text();s=s.replace(' if(n){int token=list[loadout_picker_row];lc_copy(253,202,26,3,cosmetic_ink(module_item_base(token),LC_INK),module_item_name(token),1);text_px(253,234,LC_DIM,"%d/%d   OWNED %d",loadout_picker_row+1,n,module_item_count(token));}',' if(preview_time<loadout_store_until)lc_copy(253,202,26,4,LC_GOLD,loadout_store_note,1);\n else if(n){int token=list[loadout_picker_row];lc_copy(253,202,26,3,cosmetic_ink(module_item_base(token),LC_INK),module_item_name(token),1);text_px(253,234,LC_DIM,"%d/%d   OWNED %d",loadout_picker_row+1,n,module_item_count(token));}');p.write_text(s)
p=Path('src/cockpit-hud-review.h');s=p.read_text();start=s.index(' hc_test_setup();selected_target=NPC_ID_MIN;input(PSP_CTRL_SQUARE');end=s.index(' hc_test_setup();Debris*d=',start);block=s[start:end];
block=block.replace('input(0,0,.016f,0,0)','input(0,PSP_CTRL_SQUARE,.016f,0,0)').replace('input(0,0,.1f,.8f,.4f)','input(0,PSP_CTRL_SQUARE,.1f,.8f,.4f)')
for key in ['UP','CROSS','RIGHT','CIRCLE']:
 block=block.replace('input(PSP_CTRL_'+key+',PSP_CTRL_'+key+',','input(PSP_CTRL_'+key+',PSP_CTRL_SQUARE|PSP_CTRL_'+key+',')
block=block.replace('PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,.016f','PSP_CTRL_SQUARE|PSP_CTRL_RTRIGGER|PSP_CTRL_CROSS,.016f')
block=block.replace('input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);input(0,PSP_CTRL_SQUARE,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);','input(0,0,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);')
block=block.replace('input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE|PSP_CTRL_CROSS,.016f,0,0);','input(0,PSP_CTRL_CROSS,.016f,0,0);')
block=block.replace('input(0,PSP_CTRL_CIRCLE,.21f','input(0,PSP_CTRL_SQUARE|PSP_CTRL_CIRCLE,.21f')
s=s[:start]+block+s[end:];p.write_text(s)

