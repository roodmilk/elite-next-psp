{
 game_init(&g);g.docked=1;g.station_progress[7][0]=2|(9u<<2)|(127u<<8);g.station_progress[255][HUB_COUNT-1]=1;
 CHECK(save_game(&g,"test-station-v19.sav"),"station save: V19 fixture saved");
 unsigned char bytes[131072];FILE *src=fopen("test-station-v19.sav","rb");size_t n=0;if(src){n=fread(bytes,1,sizeof(bytes),src);fclose(src);}
 int fixture=n>(256*HUB_COUNT*4+1280+(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES))+4&&n<sizeof(bytes);
 if(fixture){bytes[4]=18;FILE *old=fopen("test-station-v18.sav","wb");if(old){fixture=fwrite(bytes,1,n-(256*HUB_COUNT*4+1280+(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES))-4,old)==n-(256*HUB_COUNT*4+1280+(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES))-4;fclose(old);fixture=fixture&&save_seal("test-station-v18.sav");}else fixture=0;}
 CHECK(fixture&&load_game_file(&g,"test-station-v18.sav")&&g.station_progress[7][0]==0&&g.station_progress[255][HUB_COUNT-1]==0,"station save: real V18 fixture imports with clean station state");
 CHECK(load_game_file(&g,"test-station-v19.sav")&&g.station_progress[7][0]==(2|(9u<<2)|(127u<<8))&&g.station_progress[255][HUB_COUNT-1]==1,"station save: first and last station records preserved");
 g.station_progress[0][0]=3;CHECK(!save_game(&g,"test-station-v19.sav"),"station save: invalid manifest stage rejected atomically");
 g.station_progress[0][0]=10u<<2;CHECK(!save_game(&g,"test-station-v19.sav"),"station save: invalid story stage rejected atomically");
 g.station_progress[0][0]=1u<<31;CHECK(!save_game(&g,"test-station-v19.sav"),"station save: reserved progress bits rejected atomically");
 CHECK(load_game_file(&g,"test-station-v19.sav")&&g.station_progress[0][0]==0,"station save: rejected writes preserve previous valid commander");
 remove("test-station-v19.sav");remove("test-station-v19.sav.bak");remove("test-station-v18.sav");game_init(&g);
}
