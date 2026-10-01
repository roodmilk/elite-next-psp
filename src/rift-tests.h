{
 game_init(&g);launch(&g);g.pos=g.anomaly[0].pos;int money=g.credits;
 CHECK(analysis_scan(&g,ANOMALY_ID_MIN)&&g.rift_logged[g.system]==1&&g.rift_report==1&&g.voice_who==VOICE_COMP,"rift: scan records exact identity and opens computer report");
 CHECK(!analysis_scan(&g,ANOMALY_ID_MIN)&&g.credits==money+280,"rift: repeat report never pays twice");
 game_spawn(&g);g.pos=g.anomaly[0].pos;
 CHECK(g.anomaly[0].scanned&&!analysis_scan(&g,ANOMALY_ID_MIN)&&g.credits==money+280,"rift: revisiting preserves scan and prevents reward farming");
 g.docked=1;g.rift_logged[255]=15;
 CHECK(save_game(&g,"test-rift.sav")&&load_game(&g,"test-rift.sav")&&g.rift_logged[7]==1&&g.rift_logged[255]==15,"rift: first and last system records survive save");
 unsigned char bytes[131072];FILE *src=fopen("test-rift.sav","rb");size_t n=0;if(src){n=fread(bytes,1,sizeof(bytes),src);fclose(src);}
 int fixture=n>260+(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES)&&n<sizeof(bytes);if(fixture){bytes[4]=20;FILE *old=fopen("test-rift-v20.sav","wb");if(old){fixture=fwrite(bytes,1,n-260-(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES),old)==n-260-(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES);fclose(old);fixture=fixture&&save_seal("test-rift-v20.sav");}else fixture=0;}
 CHECK(fixture&&load_game_file(&g,"test-rift-v20.sav")&&!g.rift_logged[7]&&!g.rift_logged[255],"rift: V20 imports without inventing old scan locations");
 g.rift_logged[0]=16;CHECK(!save_game(&g,"test-rift.sav"),"rift: invalid scan masks rejected atomically");
 remove("test-rift.sav");remove("test-rift.sav.bak");remove("test-rift-v20.sav");game_init(&g);
}
