{
 TEST_INIT();game.systems[game.system].tech=15;game.systems[game.system].economy=0;change_page(EQUIP);
 for(int item=0;item<EQUIP_COUNT;item++){
  if(item==0||item==23)continue;
  game.station_variant=0;
  for(int e=0;e<8;e++){game.systems[game.system].economy=e;if(equipment_in_stock(item))break;}
  int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);row=0;
  for(int j=0;j<n;j++)if(list[j]==item)row=j;
  rect(0,0,W,H,BG);equipment();const char *left;
  text_wrap(26,16,31,3,DIM,equipment_details[item],&left);
  INPUT_CHECK(!*left,"outfitting: full instructions fit the detail panel");
  if(item==0||item==7||item==24){char file[48];snprintf(file,sizeof(file),"outfitting-%02d.bmp",item);dump_native_bmp(file);}
 }
 for(int slot=0;slot<6;slot++){
  const int modules[]={2,7,12,11,15,24};game.fit[slot]=modules[slot];fit_rebuild(&game);
  row=slot;page=INVENTORY;rect(0,0,W,H,BG);inventory_screen();
  const char *left;text_wrap(35,13,23,4,DIM,equipment_details[modules[slot]],&left);
  INPUT_CHECK(!*left,"loadout: fitted-module instructions fit selected slot panel");
  if(slot==5)dump_native_bmp("loadout-heat-buffer.bmp");
 }
 TEST_INIT();page=EQUIP;game.systems[game.system].tech=1;game.systems[game.system].economy=7;
 row=0;rect(0,0,W,H,BG);equipment();dump_native_bmp("outfitting-low-tech.bmp");
 TEST_INIT();page=EQUIP;row=0;
 {int shown[7]={0},captures=0;
  for(int sys=0;sys<256;sys++)for(int hub=0;hub<HUB_COUNT;hub++){
   game.system=sys;game.station_variant=hub;
   int kind=equipment_shop_kind();if(shown[kind])continue;
   shown[kind]=1;rect(0,0,W,H,BG);equipment();
   char file[48];snprintf(file,sizeof(file),"outfitting-shop-%d.bmp",kind);dump_native_bmp(file);captures++;
  }
  INPUT_CHECK(captures>=4,"outfitting: varied shop identities have visual coverage");
 }
 TEST_INIT();page=MISSIONS;
 for(int offer=0;offer<5;offer++){
  row=offer;rect(0,0,W,H,BG);mission_board();const char *left;
  text_wrap(26,16,31,4,DIM,mission_brief(&game,offer),&left);
  INPUT_CHECK(!*left,"mission board: complete brief fits native detail card");
  if(offer==0||offer==3){char file[48];snprintf(file,sizeof(file),"mission-board-%d.bmp",offer);dump_native_bmp(file);}
 }
 TEST_INIT();page=GALNET;
 for(int tab=3;tab<=5;tab++){
  galnet_tab=tab;int total=galnet_rows(),visible=tab==5?3:2;
  int x=tab==5?8:105,y=tab==5?61:68,h=tab==5?114:168;
  unsigned track=tab==5?UI_RAISED:RGB(210,219,230),thumb=tab==5?UI_GOLD:RGB(49,82,145);
  int valid=1;
  for(int r=0;r<total;r++){
   row=r;rect(0,0,W,H,BG);galnet_screen();
   int first=r/visible*visible,end=first+visible;if(end>total)end=total;
   for(int yy=0;yy<h;yy++)valid&=pixels[(y+yy)*STRIDE+x]==(yy>=h*first/total&&yy<h*end/total?thumb:track);
   if(r==0||r==total-1){char file[64];snprintf(file,sizeof(file),"galnet-scroll-%d-%d.bmp",tab,r);dump_native_bmp(file);}
  }
  INPUT_CHECK(valid,"GalacticNet scrollbar matches all visible pages including final partial page");
  row=total-1;input(PSP_CTRL_DOWN,0,.016f,0,0);
  INPUT_CHECK(row==0,"GalacticNet scroll selection wraps from last to first");
 }
 TEST_INIT();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;
 for(int tool=0;tool<2;tool++){
  tools_selected=tool;game.missiles=4;game.flare_charges=3;space();
  dump_native_bmp(tool?"footer-flares-count.bmp":"footer-missiles-count.bmp");
 }
 tools_selected=0;
 row=0;galactic_lore_screen();dump_native_bmp("lore-sector-label.bmp");
 TEST_INIT();page=HOME;
}

