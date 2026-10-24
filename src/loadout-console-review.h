static int loadout_console_review(void){
 FILE *flag=fopen("loadout-console-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("loadout-console-review.txt","w");if(!out)return 1;int failures=0;
 unsigned *oldfb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels;
#define LCCHECK(c,label) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=0;game.docked=1;game.ship=14;game.system=7;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));
 int fitted[]={1,2,20,25};for(int b=0;b<4;b++)game.fit[b*6]=fitted[b];game.fit[1]=6;game.fit[7]=7;game.fit[2]=13;game.fit[3]=10;game.fit[4]=14;game.fit[5]=24;game.active_weapon=0;fit_rebuild(&game);
 int stored[]={2,24,13,7,10,14,8,4};for(int j=0;j<8;j++)game.module_store[stored[j]]=1;game.cargo[0]=2;game.credits=125000;game.message_time=0;preview_time=4;
 page=INVENTORY;row=0;inventory_screen();dump_native_bmp("loadout-console-reference.bmp");
 LCCHECK(lc_fitted()==10&&lc_banks()==36&&cargo_capacity(&game)==153&&module_store_total()==8&&module_store_capacity()==40,"real Iron Cathedral fixture matches 10/36 fitted, 2/153T cargo and 8/40 storage");
 int labels=1,effects=1;for(int i=1;i<EQUIP_COUNT;i++)if(equip_slot_for(i)>=0){labels&=strlen(equipment_names[i])<=34;effects&=lc_copy(0,0,34,2,LC_INK,equipment_effects[i],0)>0;}
 LCCHECK(labels&&effects,"every real module full name and effect fits the native loadout detail strip without truncation");
 int bounds=1;for(int s=0;s<player_ship_count;s++)for(int c=0;c<6;c++)bounds&=fit_capacity(s,c)>0&&fit_capacity(s,c)<=8;
 LCCHECK(bounds&&76+7*47+43<=472&&77+5*20+17<201,"every hull bank capacity fits the complete six-category board and all eight columns");
 for(int b=0;b<7;b++)input(PSP_CTRL_RIGHT,0,.016f,0,0);LCCHECK(row==42,"D-pad Right can reach the eighth weapon bank");
 input(PSP_CTRL_RTRIGGER,0,.016f,0,0);LCCHECK(row==43,"R shoulder switches category while retaining an available eighth bank");
 input(PSP_CTRL_RTRIGGER,0,.016f,0,0);LCCHECK(row==14,"category switch clamps selection to the three real navigation banks");
 input(PSP_CTRL_LTRIGGER,0,.016f,0,0);LCCHECK(row%6==FIT_DEF&&fit_slot_open(&game,row),"L shoulder reverses category selection without exposing locked banks");
 row=0;input(PSP_CTRL_TRIANGLE,0,.016f,0,0);LCCHECK(page==STORAGE&&!storage_sell_mode&&!storage_bank_focus,"Triangle opens separate Item Storage in item-selection mode");
 module_inventory_row=0;module_bank=0;storage_screen();dump_native_bmp("storage-console-reference.bmp");
 int owned[EQUIPMENT_COUNT],n=module_owned_list(owned,EQUIPMENT_COUNT),item=module_selected_item();
 LCCHECK(n==8&&item==2&&equip_slot_for(item)==FIT_WPN,"stored list contains only actual spare copies and selected Beam Laser shows weapon banks");
 int cash=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);LCCHECK(storage_bank_focus&&game.fit[0]==1&&game.credits==cash,"first X selects the spare without fitting it or charging money");
 storage_screen();dump_native_bmp("storage-console-bank-focus.bmp");
 input(PSP_CTRL_CROSS,0,.016f,0,0);LCCHECK(game.fit[0]==2&&game.module_store[2]==0&&game.module_store[1]==1&&!storage_bank_focus&&game.credits==cash,"second X swaps into chosen bank and returns old module to storage exactly once");
 page=INVENTORY;row=0;input(PSP_CTRL_SQUARE,0,.016f,0,0);LCCHECK(game.fit[0]==FIT_EMPTY&&game.module_store[2]==1&&game.credits==cash,"Square stores a fitted module without selling it or changing credits");
 page=STORAGE;module_inventory_row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);LCCHECK(page==STORAGE&&!storage_bank_focus,"Circle backs out of bank selection to the stored list");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);LCCHECK(page!=STORAGE,"Circle then returns through the existing menu route");
 page=INVENTORY;row=3;game.cargo[0]=153;int count=module_store_total();input(PSP_CTRL_SQUARE,0,.016f,0,0);LCCHECK(game.fit[3]==10&&module_store_total()==count,"storing a hold upgrade cannot strand carried cargo");game.cargo[0]=2;
 game.fit[3]=22;fit_rebuild(&game);game.passenger_dest=129;input(PSP_CTRL_SQUARE,0,.016f,0,0);LCCHECK(game.fit[3]==22,"occupied passenger cabin remains protected from storage");game.passenger_dest=-1;
 game.docked=0;row=6;game.fit[6]=2;fit_rebuild(&game);input(PSP_CTRL_SQUARE,0,.016f,0,0);LCCHECK(game.fit[6]==2,"loadout remains read-only in flight");
 page=STORAGE;module_inventory_row=0;storage_bank_focus=0;input(PSP_CTRL_CROSS,0,.016f,0,0);LCCHECK(!storage_bank_focus,"storage cannot fit a module while undocked");storage_screen();dump_native_bmp("storage-console-flight.bmp");game.docked=1;
 int narrow=1;for(int ship=0;ship<player_ship_count;ship++){game.ship=ship;row=0;inventory_screen();narrow&=clipx0==0&&clipx1==W;char file[64];snprintf(file,sizeof(file),"loadout-console-ship-%02d.bmp",ship);dump_native_bmp(file);}LCCHECK(narrow,"all hull previews remain bounded and restore the normal renderer clip");
 game.ship=14;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));game.fit[0]=1;game.fit[5]=24;game.active_weapon=0;fit_rebuild(&game);game.module_store[2]=2;game.credits=876543;
 int saved=save_game(&game,"loadout-console-save.dat");static Game loaded;memset(&loaded,0,sizeof(loaded));int restored=saved&&load_game(&loaded,"loadout-console-save.dat");LCCHECK(restored&&loaded.fit[0]==1&&loaded.fit[5]==24&&loaded.module_store[2]==2&&loaded.credits==876543,"normal V32 saves persist fitted modules, stored quantities and credits unchanged");
 remove("loadout-console-save.dat");remove("loadout-console-save.dat.tmp");remove("loadout-console-save.dat.bak");
 game.ship=0;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));row=0;page=INVENTORY;inventory_screen();dump_native_bmp("loadout-console-starter.bmp");page=STORAGE;module_inventory_row=999;module_bank=7;storage_bank_focus=0;storage_screen();dump_native_bmp("storage-console-empty.bmp");LCCHECK(module_selected_item()<0&&module_bank==0&&fit_capacity(0,FIT_WPN)==1,"empty starter storage safely clamps bank selection and shows only its single weapon bank");
 game.ship=14;for(int i=1,k=0;i<EQUIP_COUNT&&k<40;i++)if(equip_slot_for(i)>=0){game.module_store[i]=1;k++;}page=STORAGE;module_inventory_row=999;storage_screen();n=module_owned_list(owned,EQUIPMENT_COUNT);LCCHECK(module_inventory_row==n-1,"stale stored-list selection clamps to the final real item");dump_native_bmp("storage-console-last-page.bmp");
 storage_bank_focus=0;storage_sell_mode=1;module_inventory_row=0;int sale=module_selected_item(),before=game.module_store[sale];cash=game.credits;input(PSP_CTRL_CROSS,0,.016f,0,0);LCCHECK(game.module_store[sale]==before&&module_sell_item==sale,"Outfitting sell menu still requires confirmation");input(PSP_CTRL_CROSS,0,.016f,0,0);LCCHECK(game.module_store[sale]==before-1&&game.credits==cash+equip_sell_price(sale),"Outfitting confirmed sale retains exact stock and credit changes");storage_screen();dump_native_bmp("storage-console-sell-menu.bmp");storage_sell_mode=0;
 /* Full names and multi-line effects: capture the longest module in every category. */
 for(int cat=0;cat<6;cat++){int longest=-1;for(int i=1;i<EQUIP_COUNT;i++)if(equip_slot_for(i)==cat&&(longest<0||strlen(equipment_effects[i])>strlen(equipment_effects[longest])))longest=i;if(longest>=0){game.fit[cat]=(uint8_t)longest;page=INVENTORY;row=cat;inventory_screen();char name[64];snprintf(name,sizeof(name),"loadout-console-long-effect-%d.bmp",cat);dump_native_bmp(name);}}
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=oldfb;free(pixels);
#undef LCCHECK
 return 1;
}
