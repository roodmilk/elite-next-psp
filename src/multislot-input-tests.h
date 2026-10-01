{
 TEST_INIT();game.ship=6;game.credits=100000;game.systems[7].tech=15;game.systems[7].economy=1;
 equip_target=-1;buy_equipment(1,0);buy_equipment(2,0);buy_equipment(20,0);buy_equipment(25,0);
 INPUT_CHECK(game.fit[0]==1&&game.fit[6]==2&&game.fit[12]==20&&game.fit[18]==25,"banks UI: four distinct weapons fill four free slots");
 int cash=game.credits;buy_equipment(25,0);INPUT_CHECK(game.credits==cash,"banks UI: duplicate module never charges");
 page=INVENTORY;row=0;input(PSP_CTRL_RIGHT,0,.016f,0,0);INPUT_CHECK(row==6,"banks UI: right chooses second WPN slot");
 input(PSP_CTRL_SQUARE,0,.016f,0,0);INPUT_CHECK(game.active_weapon==6&&laser_shot_damage(&game)==36,"banks UI: Square arms selected weapon");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);INPUT_CHECK(page==INVENTORY&&row==6,"banks UI: Triangle has no action on the tech board");
 row=18;input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.fit[18]==25,"banks UI: module sale still requires confirmation");
 input(PSP_CTRL_CROSS,0,.016f,0,0);INPUT_CHECK(game.fit[18]==FIT_EMPTY&&game.credits==cash+equipment_costs[25]/2,"banks UI: confirmed sale removes only selected module");
 equip_target=-1;buy_equipment(10,0);game.systems[7].economy=1;buy_equipment(11,0);buy_equipment(22,0);
 INPUT_CHECK(cargo_capacity(&game)==124&&(game.upgrades&512),"banks UI: cargo upgrades coexist with passenger cabin");
 game.passenger_dest=129;INPUT_CHECK(!unequip_slot(fit_find(&game,22),1),"banks UI: occupied extra-slot cabin cannot be sold");
 game.passenger_dest=-1;game.cargo[0]=124;INPUT_CHECK(!unequip_slot(fit_find(&game,11),1)&&cargo_capacity(&game)==124,"banks UI: stacked hold removal cannot strand cargo");
 game.cargo[0]=2;
 TEST_INIT();page=INVENTORY;row=0;input(PSP_CTRL_RIGHT,0,.016f,0,0);INPUT_CHECK(row==0,"banks UI: starter hull cannot select locked slots");
 {int visible=0;for(int c=0;c<6;c++)visible+=fit_capacity(game.ship,c);INPUT_CHECK(visible==6,"banks UI: starter board exposes exactly its six usable slots");}
 game.ship=6;{int visible=0;for(int c=0;c<6;c++)visible+=fit_capacity(game.ship,c);INPUT_CHECK(visible==22,"banks UI: largest hull exposes all and only its usable slots");}

 TEST_INIT();
}

