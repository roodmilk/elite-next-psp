/* Verify every checkpoint and legacy migration without changing the Save core. */
{
 Game tg,round;game_init(&tg);story_complete(&tg);
 int roundtrips=1;
 for(int i=0;i<=TUTORIAL_COUNT+1;i++){
  tg.tutorial_step=i;tg.tutorial_seen=i;
  if(!save_game(&tg,"test-tutorial.sav")||!load_game(&round,"test-tutorial.sav")||round.tutorial_step!=i||round.tutorial_seen!=i)roundtrips=0;
 }
 CHECK(roundtrips,"tutorial: every V15 lesson checkpoint round-trips with acknowledgement");
 /* Produce a sealed V14 fixture, whose payload ends before tutorial state. */
 {
  unsigned char bytes[4096];size_t n=0;FILE *src=fopen("test-tutorial.sav","rb");
  if(src){n=fread(bytes,1,sizeof(bytes),src);fclose(src);}
  int fixture_ok=n>12;FILE *old=fopen("test-tutorial-v14.sav","wb");
  if(old&&fixture_ok){bytes[4]=14;fixture_ok=fwrite(bytes,1,n-12,old)==n-12;fclose(old);fixture_ok=fixture_ok&&save_seal("test-tutorial-v14.sav");}else {if(old)fclose(old);fixture_ok=0;}
  CHECK(fixture_ok&&load_game(&round,"test-tutorial-v14.sav")&&!round.tutorial_step&&tutorial_service(&round,24),"tutorial: V14 commanders retain unrestricted services");
  remove("test-tutorial-v14.sav");
 }
 tg.tutorial_step=TUTORIAL_COUNT+2;
 CHECK(!save_game(&tg,"test-tutorial.sav"),"tutorial: out-of-range lesson rejected before replacing a valid save");
 CHECK(load_game(&round,"test-tutorial.sav")&&round.tutorial_step==TUTORIAL_COUNT+1,"tutorial: failed checkpoint preserves previous commander");
 remove("test-tutorial.sav");remove("test-tutorial.sav.bak");remove("test-tutorial.sav.tmp");
}
