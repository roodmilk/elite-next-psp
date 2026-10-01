{
 TEST_INIT();launch(&game);page=FLIGHT;game.destination=(game.system+1)%256;game.jump=4;thargoid_begin();thargoid_intro=0;
 for(int i=0;i<THARGOID_MAX;i++)if(thargoid_raider[i].alive){thargoid_raider[i].entry=0;thargoid_raider[i].age=2+i*.13f;}
 unsigned hashes[5]={0};int distinct=1,rich=1;
 for(int section=0;section<5;section++){
  thargoid_time=section*9.f+.4f;if(section==2)thargoid_enemy_missile(322,92);
  memset(pixels,0,STRIDE*H*sizeof(unsigned));thargoid_view();unsigned hash=2166136261u;int lit=0;
  for(int y=24;y<231;y++)for(int x=0;x<W;x++){unsigned c=pixels[y*STRIDE+x];if(c){lit++;hash=(hash^c)*16777619u;}}
  hashes[section]=hash;rich&=lit>700;for(int prior=0;prior<section;prior++)distinct&=hashes[prior]!=hash;
  char file[48];snprintf(file,sizeof(file),"thargoid-route-%d.bmp",section);dump_native_bmp(file);
 }
 INPUT_CHECK(rich&&distinct,"thargoid visuals: all five high-speed routes paint rich and distinct scenes");
 unsigned transition_hash[2]={0};int transition_rich=1;
 for(int scene=0;scene<2;scene++){thargoid_wave=scene+1;thargoid_wave_delay=3.2f;memset(thargoid_raider,0,sizeof(thargoid_raider));memset(thargoid_missile,0,sizeof(thargoid_missile));memset(pixels,0,STRIDE*H*sizeof(unsigned));thargoid_view();unsigned hash=2166136261u;int lit=0;for(int y=24;y<231;y++)for(int x=0;x<W;x++){unsigned c=pixels[y*STRIDE+x];if(c){lit++;hash=(hash^c)*16777619u;}}transition_hash[scene]=hash;transition_rich&=lit>700;char file[56];snprintf(file,sizeof(file),"thargoid-transition-%d.bmp",scene);dump_native_bmp(file);}
 INPUT_CHECK(transition_rich&&transition_hash[0]!=transition_hash[1],"thargoid intermissions paint distinct asteroid-canyon and capital-graveyard turns");
 TEST_INIT();
}
