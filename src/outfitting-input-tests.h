/* Catalogue-wide transaction, stock and persistence contract. */
{
 TEST_INIT();int stock_ok=1,offered[EQUIP_COUNT]={0},variants=0;
 for(int sys=0;sys<256;sys++){
  uint64_t primary=0;
  for(int hub=0;hub<HUB_COUNT;hub++){
   game.system=sys;game.station_variant=hub;int list[EQUIP_COUNT],n=equipment_stock_list(list,EQUIP_COUNT);uint64_t mask=0;
   stock_ok &= n>=0&&!equipment_in_stock(0);
   for(int j=0;j<n;j++){
    int i=list[j];if(i<64)mask|=1ull<<i;offered[i]++;
    int tech=game.systems[sys].tech+1-(hub>0);if(tech<1)tech=1;
    stock_ok &= i!=0&&(sys==7?(equip_slot_for(i)==FIT_WPN||i==3):(equipment_tech[i]<=tech&&prosperity(&game,sys)>=equipment_trade[i]&&(equipment_econ[i]&(1u<<game.systems[sys].economy))));
   }
   if(!hub)primary=mask;else if(mask!=primary)variants++;
  }
 }
 INPUT_CHECK(stock_ok&&variants>0,"outfitting: all 256 systems and three hub variants obey tech/economy/trade stock rules");
 {int shop_kinds[7]={0},shop_names=0;char seen[32][40];
  for(int sys=0;sys<256;sys++)for(int hub=0;hub<HUB_COUNT;hub++){
   game.system=sys;game.station_variant=hub;int kind=equipment_shop_kind();shop_kinds[kind]=1;
   const char *name=equipment_shop_name();int known=0;for(int k=0;k<shop_names;k++)if(!strcmp(seen[k],name))known=1;
   if(!known&&shop_names<32)snprintf(seen[shop_names++],sizeof(seen[0]),"%s",name);
   stock_ok&=name[0]&&equipment_shop_line()[0];
  }
  int kinds=0;for(int k=0;k<7;k++)kinds+=shop_kinds[k];
  INPUT_CHECK(stock_ok&&kinds>=4&&shop_names>=10,"outfitting: deterministic station shop signs vary by stocked specialty");
 }
 int coverage=1;for(int i=1;i<EQUIP_COUNT;i++)if(i!=23&&!offered[i])coverage=0;
 INPUT_CHECK(coverage,"outfitting: every public item is obtainable somewhere, including Heat Buffer");
 for(int item=1;item<EQUIP_COUNT;item++)if(item!=3){
  TEST_INIT();fit_clear_all(&game);fit_rebuild(&game);game.credits=100000;game.systems[game.system].tech=15;
  for(int e=0;e<8;e++){game.systems[game.system].economy=e;if(equipment_in_stock(item)||item==23)break;}
  int slot=equip_slot_for(item),cash=game.credits;
  buy_equipment(item,item==23);
  INPUT_CHECK(game.fit[slot]==item&&game.credits==cash-equipment_costs[item],"outfitting: every module buys into its declared slot for exact cost");
  cash=game.credits;buy_equipment(item,item==23);
  INPUT_CHECK(game.credits==cash,"outfitting: already-owned module never charges again");
  Game loaded;int saved=save_game(&game,"test-all-equipment.sav")&&load_game(&loaded,"test-all-equipment.sav");
  INPUT_CHECK(saved&&loaded.fit[slot]==item&&loaded.upgrades==game.upgrades&&loaded.laser==game.laser,"outfitting: every fitted item/effect survives save and load");
  remove("test-all-equipment.sav");remove("test-all-equipment.sav.bak");
  sell_equipment_row(item);
  INPUT_CHECK(game.fit[slot]==FIT_EMPTY&&game.credits==cash+equipment_costs[item]/2&&game.upgrades==0&&!game.laser,"outfitting: every module sells for half and removes its effects");
 }
 TEST_INIT();game.systems[game.system].tech=15;game.systems[game.system].economy=0;game.credits=10000;
 game.fit[FIT_WPN]=2;fit_rebuild(&game);int before=game.credits;
 equipment_buy_action(1,0);INPUT_CHECK(game.fit[FIT_WPN]==2&&game.credits==before,"outfitting: replacement needs confirmation");
 equipment_buy_action(1,0);INPUT_CHECK(game.fit[FIT_WPN]==1&&game.credits==before-equipment_costs[1]+equipment_costs[2]/2,"outfitting: confirmed replacement settles exact trade-in");
 game.fit[FIT_DEF]=7;fit_rebuild(&game);before=game.credits;buy_equipment(16,0);
 INPUT_CHECK(game.fit[FIT_DEF]==16&&shield_regen_rate(&game)==1.5f&&!(game.upgrades&128),"outfitting: replacing shield with ECM removes shield bonus");
 game.fit[FIT_HOLD]=11;fit_rebuild(&game);game.cargo[0]=player_ships[game.ship].capacity+10;before=game.credits;
 buy_equipment(10,0);INPUT_CHECK(game.fit[FIT_HOLD]==11&&game.credits==before&&!unequip_slot(FIT_HOLD,1),"outfitting: cannot lose hold capacity needed by existing cargo");
 game.cargo[0]=0;game.fit[FIT_HOLD]=22;fit_rebuild(&game);game.passenger_dest=0;before=game.credits;
 buy_equipment(10,0);INPUT_CHECK(game.fit[FIT_HOLD]==22&&game.credits==before&&!unequip_slot(FIT_HOLD,1),"outfitting: occupied cabin cannot be sold or replaced");
 game.passenger_dest=-1;game.fit[FIT_NAV]=12;fit_rebuild(&game);game.systems[game.system].economy=5;
 before=game.credits;buy_equipment(5,0);INPUT_CHECK(game.fit[FIT_NAV]==5&&!(game.upgrades&16)&&(game.upgrades&1024)&&game.credits==before-1800+1500,"outfitting: NAV replacements remove old scanner and apply beacon");
 game.fit[FIT_DEF]=7;fit_rebuild(&game);before=game.credits;buy_equipment(17,0);
 INPUT_CHECK(game.fit[FIT_UTIL]==17&&game.fit[FIT_DEF]==7,"outfitting: Chaff is UTIL and leaves the shield fitted");
 before=game.credits;buy_equipment(7,0);INPUT_CHECK(game.credits==before,"outfitting: unavailable stock cannot charge");
 game.credits=100000;game.systems[game.system].tech=15;game.fit[FIT_DEF]=FIT_EMPTY;fit_rebuild(&game);before=game.credits;buy_equipment(7,1);
 INPUT_CHECK(game.fit[FIT_DEF]==7&&game.credits==before-equipment_costs[7],"chandler: advertised special stock is actually purchasable outside ordinary economy stock");
 TEST_INIT();game.systems[game.system].tech=15;game.credits=10000;game.fuel=player_ships[game.ship].range-3.5f;before=game.credits;
 buy_equipment(0,0);INPUT_CHECK(game.fuel==player_ships[game.ship].range-3.5f&&game.credits==before,"outfitting: fuel purchases are blocked");
 engineer_refuel();INPUT_CHECK(game.fuel==player_ships[game.ship].range&&game.credits==before-8,"engineers: refuel quote and debit agree");
 game.missiles=3;before=game.credits;buy_equipment(3,0);buy_equipment(3,0);
 INPUT_CHECK(game.missiles==4&&game.credits==before-equipment_costs[3],"outfitting: missile service caps at four without repeat charge");
 launch(&game);before=game.credits;buy_equipment(1,0);sell_equipment_row(game.fit[FIT_WPN]);
 INPUT_CHECK(game.credits==before,"outfitting: flight cannot buy or sell fitted items");
 TEST_INIT();game.systems[game.system].tech=15;game.systems[game.system].economy=0;change_page(EQUIP);row=0;
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==INVENTORY,"outfitting: Triangle opens ship loadout");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==INVENTORY,"loadout: Triangle remains unused on the tech board");
 TEST_INIT();game.systems[game.system].tech=15;game.systems[game.system].economy=5;
 game.fit[FIT_UTIL]=24;fit_rebuild(&game);before=game.credits;buy_equipment(17,0);
 INPUT_CHECK(game.fit[FIT_UTIL]==17&&game.credits==before+equip_sell_price(24)-equipment_costs[17],"outfitting: cheaper replacement returns the excess trade-in value");
 TEST_INIT();page=MISSIONS;row=0;input(PSP_CTRL_CROSS,0,0,0,0);
 INPUT_CHECK(game.job_n==1&&tracked_mission==2,"mission board: acceptance immediately tracks the new contract");
 input(PSP_CTRL_CROSS,0,0,0,0);
 INPUT_CHECK(page==CAMPAIGN&&game.job_n==1&&tracked_mission==2,"mission board: accepted card opens its tracked briefing without duplicate charge");
 TEST_INIT();page=HOME;
}
