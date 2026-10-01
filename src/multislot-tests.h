{
 game_init(&g);g.ship=6;fit_clear_all(&g);
 g.fit[0]=1;g.fit[6]=2;g.fit[12]=20;g.fit[18]=25;
 g.fit[3]=10;g.fit[9]=11;g.fit[15]=22;g.fit[21]=23;
 g.fit[1]=7;g.fit[7]=16;g.fit[5]=24;g.fit[11]=17;g.fit[17]=19;
 g.active_weapon=6;fit_rebuild(&g);
 CHECK(cargo_capacity(&g)==132&&(g.upgrades&512),"banks: three distinct cargo expansions and cabin work together");
 CHECK(laser_shot_damage(&g)==36&&!(g.upgrades&65536),"banks: inactive mining laser does not affect active beam");
 CHECK(ecm_fitted(&g)&&flare_capacity(&g)==6&&(g.upgrades&32768)&&(g.upgrades&262144),"banks: extra-slot ECM/chaff/repair/heat-buffer effects coexist");
 g.active_weapon=12;fit_rebuild(&g);CHECK(mine_shot_damage(&g)==54&&laser_shot_damage(&g)==18,"banks: switching arms mining laser only");
 g.active_weapon=18;fit_rebuild(&g);CHECK(laser_shot_damage(&g)==60&&!(g.upgrades&65536),"banks: fourth weapon is a working heavy laser");
 CHECK(save_game(&g,"test-banks.sav")&&load_game_file(&loaded,"test-banks.sav")&&!memcmp(g.fit,loaded.fit,24)&&loaded.active_weapon==18,"banks: V24 saves all fitted modules and chosen laser");
 int credits=g.credits;CHECK(!buy_ship(&g,0)&&g.ship==6&&g.credits==credits&&g.fit[21]==23,"banks: smaller ship rejects overflow without losing modules or money");
 g.credits=2000000;CHECK(!buy_ship(&g,7)&&g.ship==6,"banks: downgrade respects four-weapon versus hull limits where applicable");
 CHECK(buy_ship(&g,9)&&g.ship==9&&g.fit[g.active_weapon]==25&&cargo_capacity(&g)==80,"banks: compatible hull exchange preserves modules, weapon and stacked cargo");
 /* V23 fixtures retain the old primary bank and omit the appended V24 bytes. */
 game_init(&g);g.fit[0]=2;g.fit[3]=11;fit_rebuild(&g);
 CHECK(save_game(&g,"test-banks-old.sav"),"banks: migration source saved");
 unsigned char bytes[131072];FILE *source=fopen("test-banks-old.sav","rb");size_t size=0;
 if(source){size=fread(bytes,1,sizeof(bytes),source);fclose(source);}
 int migrated=0;if(size>FIT_SAVE_BYTES+4&&size<sizeof(bytes)){
  bytes[4]=23;FILE *old=fopen("test-banks-v23.sav","wb");
  if(old){size_t payload=size-FIT_SAVE_BYTES-4;migrated=fwrite(bytes,1,payload,old)==payload;fclose(old);migrated=migrated&&save_seal("test-banks-v23.sav");}
 }
 CHECK(migrated&&load_game_file(&loaded,"test-banks-v23.sav")&&loaded.fit[0]==2&&loaded.fit[3]==11&&loaded.fit[6]==FIT_EMPTY,"banks: V23 existing modules migrate into first slots without loss");
 g.ship=0;g.fit[6]=25;
 CHECK(!save_game(&g,"test-banks-old.sav"),"banks: invalid locked slot cannot overwrite a valid save");
 remove("test-banks.sav");remove("test-banks.sav.bak");remove("test-banks-old.sav");remove("test-banks-old.sav.bak");remove("test-banks-v23.sav");
}
