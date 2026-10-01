{
 game_init(&g);launch(&g);NPC *victim=&g.npc[0];victim->alive=1;victim->role=LAW;victim->freighter=0;victim->bounty_slot=-1;victim->health=1000;victim->shield=0;
 hit(&g,0,1,1);police_begin(&g,0);char detail[512];police_charge_details(&g,detail,sizeof(detail));
 CHECK((g.crime_record[g.system]&CRIME_LAW_ASSAULT)&&strstr(police_accusation(&g),"law-enforcement")&&!strstr(detail,"Restricted"),"law: actual police hit records attack, never empty cargo accusation");
 victim->health=1;hit(&g,0,10,1);
 CHECK((g.crime_record[g.system]&CRIME_LAW_DESTRUCTION)&&strstr(police_accusation(&g),"destroyed a patrol"),"law: destruction escalates the correct offence");
 game_init(&g);launch(&g);g.npc[0].alive=1;g.npc[0].role=TRADERS;g.npc[0].freighter=0;g.npc[0].health=1000;g.npc[0].shield=0;hit(&g,0,1,1);
 CHECK((g.crime_record[g.system]&CRIME_ASSAULT)&&strstr(police_accusation(&g),"not a lawful target"),"law: civilian assault distinguished from police assault");
 int violence=g.legal;g.cargo[6]=2;police_begin(&g,1);police_scan_submit(&g);g.docked=1;
 CHECK(save_game(&g,"test-crime.sav")&&load_game(&g,"test-crime.sav")&&g.police_cargo_heat==4,"law: saved mixed offence retains cargo attribution");
 police_begin(&g,0);CHECK(police_surrender_cargo(&g)&&g.legal==violence&&(g.crime_record[g.system]&CRIME_ASSAULT)&&!(g.crime_record[g.system]&CRIME_CARGO),"law: reloaded surrender clears only cargo offence");
 police_begin(&g,1);police_scan_submit(&g);
 CHECK(g.police_stop&&g.police_phase==0&&g.legal==violence,"law: clean scan cannot release an existing violent warrant");
 g.credits=100000;police_resolve(&g,0);
 CHECK(!g.crime_record[g.system]&&!strstr(g.message,"seized"),"law: fine clears offence and does not invent a cargo seizure");
 record_crime(&g,4,CRIME_ASSAULT);int origin=g.system;g.system=8;g.legal=g.wanted[8];record_crime(&g,10,CRIME_ESCAPE);
 CHECK((g.crime_record[origin]&CRIME_ASSAULT)&&(g.crime_record[8]&CRIME_ESCAPE),"law: systems keep distinct offences");
 g.docked=1;CHECK(save_game(&g,"test-crime.sav"),"law: multisystem ledger saves");
 unsigned char bytes[131072];FILE *src=fopen("test-crime.sav","rb");size_t n=0;if(src){n=fread(bytes,1,sizeof(bytes),src);fclose(src);}
 int fixture=n>1284+(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES)&&n<sizeof(bytes);
 if(fixture){bytes[4]=19;FILE *old=fopen("test-crime-v19.sav","wb");if(old){fixture=fwrite(bytes,1,n-1284-(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES),old)==n-1284-(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES);fclose(old);fixture=fixture&&save_seal("test-crime-v19.sav");}else fixture=0;}
 CHECK(fixture&&load_game_file(&g,"test-crime-v19.sav")&&g.crime_record[origin]==CRIME_UNKNOWN&&g.crime_record[8]==CRIME_UNKNOWN&&strstr(police_accusation(&g),"unavailable"),"law: V19 warrants import honestly without invented crime");
 game_init(&g);launch(&g);g.npc[0].alive=1;g.npc[0].role=PIRATES;g.npc[0].health=1000;g.npc[0].shield=0;hit(&g,0,1,1);
 CHECK(!g.legal&&!g.crime_record[g.system],"law: pirate combat remains lawful");
 g.cargo[6]=1;police_begin(&g,1);police_scan_refuse(&g);
 CHECK(g.crime_record[g.system]&CRIME_REFUSAL,"law: refusal recorded");police_escape(&g);
 CHECK(g.crime_record[g.system]&CRIME_ESCAPE,"law: fleeing recorded");
 remove("test-crime.sav");remove("test-crime.sav.bak");remove("test-crime-v19.sav");game_init(&g);
}
