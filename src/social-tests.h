{
 social_test_time=1800000000u;game_init(&g);unsigned rng=g.rng;
 social_emit(&g,SB_PAINT);unsigned post=g.social.post[0];
 CHECK((post&31)==SB_PAINT&&g.social.stamp[0]==social_test_time&&g.rng==rng,"social: real-date event snapshot never consumes combat RNG");
 social_emit(&g,SB_PAINT);
 CHECK(!g.social.post[1],"social: repeated action throttled within 90 seconds");
 g.social.post[0]|=2u<<30;g.ship=4;social_emit(&g,SB_LAUNCH);
 CHECK((g.social.post[1]>>30)==2&&((g.social.post[1]>>5)&15)==0,"social: reaction and original ship move with the post");
 g.system=129;social_arrive(&g);
 CHECK((g.social.post[0]&31)==SB_ARRIVE&&(g.social.post[1]&31)==SB_LAUNCH&&g.social.origin[0]==129&&g.social.origin[1]==7,"social: global feed retains actions and origins across systems");
 social_test_time+=86401;g.system=7;social_arrive(&g);
 CHECK((g.social.post[0]&31)==SB_RETURN&&(g.social.post[1]&31)==SB_MISSED&&g.social.stamp[1]==social_test_time,"social: returning after absence writes dated posts now, not fake offline history");
 for(int i=0;i<SOCIAL_KEEP+4;i++){social_test_time+=100;social_emit(&g,SB_SALVAGE);}
 CHECK((g.social.post[SOCIAL_KEEP-1]&31)==SB_SALVAGE,"social: bounded newest-first history rolls over safely");
 g.docked=1;g.social.evidence_reaction=2;g.system=255;social_emit(&g,SB_FINE);g.system=7;
 CHECK(save_game(&g,"test-social.sav")&&load_game_file(&g,"test-social.sav")&&g.social.evidence_reaction==2&&(g.social.post[0]&31)==SB_FINE,"social: all-system history and reactions persist in V23");
 unsigned char bytes[131072];FILE *src=fopen("test-social.sav","rb");size_t n=0;
 if(src){n=fread(bytes,1,sizeof(bytes),src);fclose(src);}
 int fixture=n>(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES)+4&&n<sizeof(bytes);
 if(fixture){bytes[4]=21;FILE *old=fopen("test-social-v21.sav","wb");if(old){size_t len=n-(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES)-4;fixture=fwrite(bytes,1,len,old)==len;fclose(old);fixture=fixture&&save_seal("test-social-v21.sav");}else fixture=0;}
 CHECK(fixture&&load_game_file(&g,"test-social-v21.sav")&&!g.social.post[0],"social: V21 imports without inventing past actions");
 /* A real V22 payload: interleaved dates from two old local feeds. */
 int legacy=fixture;bytes[4]=22;FILE *v22=fopen("test-social-v22.sav","wb");
 if(v22&&legacy){
  size_t core=n-(SOCIAL_SAVE_BYTES+FIT_SAVE_BYTES)-4;legacy=fwrite(bytes,1,core,v22)==core;
  legacy=legacy&&save_u32(v22,100)&&save_u32(v22,3);
  for(int sys=0;legacy&&sys<256;sys++){
   legacy=save_u32(v22,0)&&save_u32(v22,0);
   for(int i=0;legacy&&i<16;i++){
    uint32_t p=0,t=0;
    if(sys==7&&i==0){p=SB_PAINT|(2u<<30);t=1800000300u;}
    if(sys==7&&i==1){p=SB_DOCK;t=1800000100u;}
    if(sys==129&&i==0){p=SB_ARRIVE;t=1800000200u;}
    legacy=save_u32(v22,p)&&save_u32(v22,t);
   }
  }
  fclose(v22);legacy=legacy&&save_seal("test-social-v22.sav");
 }else {if(v22)fclose(v22);legacy=0;}
 CHECK(legacy&&load_game_file(&g,"test-social-v22.sav")&&g.social.count==3&&
  g.social.origin[0]==(7u|256u)&&g.social.origin[1]==(129u|256u)&&
  (g.social.post[0]>>30)==2&&(g.social.post[2]&31)==SB_DOCK,
  "social: V22 local histories merge by date with origin and reaction intact");
 g.social.post[0]=31;g.social.stamp[0]=social_test_time;
 CHECK(!save_game(&g,"test-social.sav"),"social: invalid post type rejected before replacing a good save");
 social_test_time=1800000000u;game_init(&g);launch(&g);g.destination=129;g.jump=.001f;game_tick(&g,.016f,0,0,0,0);
 CHECK((g.social.post[1]&31)==SB_LEAVE&&(g.social.post[0]&31)==SB_ARRIVE,"social: actual completed jump posts departure and arrival to correct systems");
 game_init(&g);g.police_stop=1;g.police_phase=0;police_resolve(&g,1);
 CHECK((g.social.post[0]&31)==SB_ARREST,"social: actual custody choice posts arrest");
 CHECK(!social_elapsed(1700000000u,1800000000u,90)&&!social_elapsed(0x80000001u,1800000000u,90),"social: backward and unavailable clocks never fabricate an absence");
 game_init(&g);for(int i=0;i<10;i++){social_test_time+=300;social_emit(&g,SB_AMBIENT);}
 CHECK(g.social.post[2]&&!g.social.post[3],"social: idle chatter cannot evict the whole activity history");
 remove("test-social.sav");remove("test-social.sav.bak");remove("test-social-v21.sav");remove("test-social-v22.sav");
 social_test_time=0;game_init(&g);
}
