{
 /* Compiled regressions; execute with the existing game self-test runner. */
 game_init(&g);g.ship=6;fit_clear_all(&g);g.pip_wep=2;
 g.fit[0]=27;g.fit[5]=50;g.fit[11]=51;g.fit[17]=52;g.fit[23]=53;fit_rebuild(&g);
 CHECK(fabsf(weapon_output_multiplier(&g)-1.3f)<.001f,"catalogue: firepower bonuses add to 30 percent");
 CHECK(fabsf(weapon_cycle(&g)-.595f)<.001f&&fabsf(weapon_heat(&g)-28)<.001f,"catalogue: servo, overdrive and cold coil combine");
 g.fit[2]=40;g.fit[8]=41;g.fit[14]=38;g.fit[20]=39;fit_rebuild(&g);
 CHECK(fabsf(weapon_range(&g)-5040)<.01f&&fabsf(weapon_cone(&g)-.0144f)<.0001f,"catalogue: optics and targeting lens affect armed lance");
 CHECK(module_scan_range(&g)==7500&&module_survey_range(&g)==760,"catalogue: deep and survey scanners have independent ranges");
 g.fit[1]=34;g.fit[7]=33;g.fit[3]=43;g.fit[9]=44;g.fit[15]=45;g.fit[21]=11;g.fit[4]=47;fit_rebuild(&g);
 CHECK(shield_regen_rate(&g)==7&&fit_hold_bonus(&g)==76&&fuel_scoop_rate(&g)==1.25f,"catalogue: shield relay, cargo banks and corona scoop work");
 CHECK(save_game(&g,"test-expanded.sav")&&load_game_file(&loaded,"test-expanded.sav")&&!memcmp(g.fit,loaded.fit,24),"catalogue: IDs above 31 round-trip without bitmask aliasing");
 for(int item=26;item<EQUIPMENT_COUNT;item++){
  fit_clear_all(&g);int cat=equip_slot_for(item);g.fit[cat]=item;fit_rebuild(&g);
  CHECK(fit_find(&g,item)==cat&&save_game(&g,"test-expanded.sav")&&load_game_file(&loaded,"test-expanded.sav")&&loaded.fit[cat]==item,"catalogue: every new module survives rebuild and save/load");
 }
 game_init(&g);launch(&g);fit_clear_all(&g);g.fit[1]=32;g.fit[7]=35;g.fit[13]=36;g.ship=6;fit_rebuild(&g);g.energy=0;g.hull=100;
 player_damage(&g,10,0);CHECK(fabsf(g.hull-96.4f)<.001f,"catalogue: reactive armour reduces hull damage");
 g.energy=100;thermal_damage(&g,10,0);CHECK(fabsf(g.energy-93)<.001f,"catalogue: thermal liner reduces thermal damage");
 g.energy=100;collision_damage(&g,10,0);CHECK(fabsf(g.energy-92.5f)<.001f,"catalogue: impact damper reduces collision damage");
 for(int w=29;w<=31;w++){
  game_init(&g);launch(&g);fit_clear_all(&g);g.fit[0]=w;g.pip_wep=2;fit_rebuild(&g);g.job_n=0;g.contract=-1;
  NPC *n=&g.npc[0];n->alive=1;n->role=TRADERS;n->freighter=0;n->bounty_slot=-1;n->health=100;n->shield=100;n->cooldown=0;
  primary_weapon_hit(&g,0,100);
  CHECK(g.wanted[g.system]>0,"catalogue: specialist weapons still report civilian assault");
  if(w==29)CHECK(n->shield==63&&n->health==100,"catalogue: ion drains shields before normal damage");
  if(w==30)CHECK(n->shield==47.5f&&n->health==82.5f,"catalogue: plasma bypasses one quarter through shields");
  if(w==31)CHECK(n->cooldown>=1.2f,"catalogue: disruptor delays return fire");
 }
 remove("test-expanded.sav");remove("test-expanded.sav.bak");
}
