#if defined(ELITE_ENABLE_REVIEW)||defined(ELITE_ENCOUNTER_REVIEW)
static int dashboard_review(void){
 FILE*flag=fopen("dashboard-review.flag","r");if(!flag)return 0;fclose(flag);FILE*out=fopen("dashboard-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int fails=0,checks=0;
 #define DC(c,label) do{int ok=!!(c);checks++;fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);}while(0)
 hc_test_setup();game.docked=1;page=EQUIP;game.credits=100000;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));memset(&game.dockbay,0,sizeof(game.dockbay));game.cargo[0]=0;
 DC(cosmetic_installed(&game,DASHBOARD_FIRST)<0,"no free default dashboard cosmetic");
 for(int item=DASHBOARD_FIRST;item<=DASHBOARD_LAST;item++){
  int a[EQUIP_COUNT],n=equipment_stock_list(a,EQUIP_COUNT);row=-1;for(int i=0;i<n;i++)if(a[i]==item)row=i;
  DC(row>=0&&equip_slot_for(item)==FIT_HOLD,"Lave stocks the approved dashboard item in an existing Hold/Cabin slot category");
  int cash=game.credits,count=module_store_total();input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
  DC(game.credits==cash-250&&module_store_total()==count+1&&cosmetic_installed(&game,item)<0,"actual shop purchase charges25U into Item Storage without installing");
 }
 module_buy_to_store(CHARM_UFO);page=STORAGE;storage_sell_mode=storage_bank_focus=0;charm_review_select(CHARM_UFO);module_bank=0;module_swap_selected();unsigned charm_ref=game.dockbay.fitted[5];
 charm_review_select(DASHBOARD_FIRST);module_bank=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 DC(game.fit[3]==DASHBOARD_FIRST&&game.fit[5]==CHARM_UFO&&game.dockbay.fitted[5]==charm_ref,"real storage input fits a dashboard item without replacing the hanging charm");
 int count=module_store_total();unsigned dash_ref=game.dockbay.fitted[3];charm_review_select(DASHBOARD_FIRST+1);module_bank=1;
 DC(!module_swap_selected()&&game.dockbay.fitted[3]==dash_ref&&game.fit[9]==FIT_EMPTY&&module_store_total()==count,"second dashboard type in another bank is blocked without losing ownership");
 module_bank=0;DC(!module_swap_selected()&&game.dockbay.fitted[3]==dash_ref,"same-bank replacement also requires explicit dashboard removal");
 page=INVENTORY;row=3;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
 DC(cosmetic_installed(&game,DASHBOARD_FIRST)<0&&db_instance_stored(&game,dash_ref-1)&&game.dockbay.fitted[5]==charm_ref,"actual Square removal returns the exact dashboard instance and leaves the hanging charm fitted");
 page=STORAGE;charm_review_select(DASHBOARD_FIRST+5);module_bank=5;DC(module_swap_selected()&&game.fit[33]==DASHBOARD_FIRST+5,"another dashboard type fits only after the current one is removed");
 DC(save_game(&game,"dashboard-roundtrip.sav"),"existing sealed save writes both cosmetic families and owned items");
 Game*copy=malloc(sizeof(Game));if(copy){*copy=game;DC(load_game(&game,"dashboard-roundtrip.sav")&&game.fit[33]==DASHBOARD_FIRST+5&&game.fit[5]==CHARM_UFO&&!memcmp(game.dockbay.owned,copy->dockbay.owned,sizeof(game.dockbay.owned)),"reload preserves dashboard, hanging charm and every stored instance");free(copy);}else DC(0,"allocate save comparison");
 game.fit[3]=DASHBOARD_FIRST;game.dockbay.fitted[3]=dash_ref;DC(!save_game(&game,"dashboard-invalid.sav"),"sealed save refuses a corrupt second dashboard across banks before committing");game.fit[3]=FIT_EMPTY;game.dockbay.fitted[3]=0;
 DC(equip_mask_for(DASHBOARD_FIRST)==0&&module_hold_bonus(DASHBOARD_FIRST)==0&&tech_module(DASHBOARD_FIRST)==0,"dashboard cosmetics add no cargo, power or technical bonuses");
 DC(load_game(&game,"legacy-v38.sav")&&cosmetic_installed(&game,DASHBOARD_FIRST)<0,"pre-cosmetic V38 saves remain loadable with no invented items");
 unsigned*previous=fb,*pixels=malloc(STRIDE*H*4);if(pixels){fb=pixels;
  for(int item=0;item<16;item++){
   hc_test_setup();fit_clear_all(&game);game.fit[5]=CHARM_UFO;game.fit[3]=DASHBOARD_FIRST+item;fit_rebuild(&game);charm_tick();space();
   char path[80];snprintf(path,sizeof(path),"dashboard-%02d-native.bmp",item);dump_native_bmp(path);
   int mismatch=0;for(int y=0;y<32;y++)for(int x=0;x<24;x++){unsigned c=dashboard_pixels[item][y*24+x];if((c>>24)==255&&pixels[(116+y)*STRIDE+x]!=c)mismatch++;}
   DC(!mismatch,"every opaque supplied dashboard pixel is unchanged at native(0,116)");
   unsigned before[72*18];for(int y=0;y<18;y++)for(int x=0;x<72;x++)before[y*72+x]=pixels[(137+y)*STRIDE+29+x];dashboard_draw();int changed=0;for(int y=0;y<18;y++)for(int x=0;x<72;x++)changed+=before[y*72+x]!=pixels[(137+y)*STRIDE+29+x];
   DC(!changed,"shelf artwork cannot overwrite the shifted Comms button or label");
   game.docked=1;page=EQUIP;game.message_time=game.voice_time=0;int a[EQUIP_COUNT],n=equipment_stock_list(a,EQUIP_COUNT);for(int i=0;i<n;i++)if(a[i]==DASHBOARD_FIRST+item)row=i;rect(0,0,W,H,BG);equipment();snprintf(path,sizeof(path),"dashboard-%02d-shop.bmp",item);dump_native_bmp(path);
  }
  hc_test_setup();game.fit[3]=DASHBOARD_FIRST;hc_browser=1;hc_cache_dirty=1;space();unsigned*copy=malloc(STRIDE*H*4);if(copy){memcpy(copy,pixels,STRIDE*H*4);dashboard_draw();DC(!memcmp(copy,pixels,STRIDE*H*4),"expanded target computer fully occludes even the shelf pixels outside its left border");free(copy);}dump_native_bmp("dashboard-expanded.bmp");
  game.docked=1;page=INVENTORY;row=3;loadout_store_until=-1;loadout_console_draw();dump_native_bmp("dashboard-fitted.bmp");module_buy_to_store(DASHBOARD_FIRST+4);page=STORAGE;charm_review_select(DASHBOARD_FIRST+4);storage_console_draw();dump_native_bmp("dashboard-storage.bmp");
  hc_test_setup();game.voice_time=0;game.message_time=5;snprintf(game.message,sizeof(game.message),"COMMS LAYOUT CHECK. CLOSE TRIANGLE IS INSIDE ITS OWN HEADER ROW. THREE READABLE MESSAGE ROWS ARE RETAINED WITHOUT TEXT TOUCHING THE BORDER.");
  for(int y=24;y<=35;y+=11){space();speech_box(8,y,464);int covered=0;for(int x=392;x<471;x++)covered+=pixels[y*STRIDE+x]!=RGB(193,139,77);DC(!covered,"CLOSE/Triangle header does not cross the message border at either native caption anchor");}
  dump_native_bmp("caption-close-fixed.bmp");fb=previous;free(pixels);
 }else DC(0,"allocate native framebuffer");
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,fails);fclose(out);return 1;
 #undef DC
}
#endif
