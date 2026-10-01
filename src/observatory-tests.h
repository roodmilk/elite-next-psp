{
 /* Core regressions, compiled into the existing self-test runner. */
 game_init(&g);
 CHECK(observatory_site(&g,7,1)==2&&observatory_site(&g,7,2)==2&&observatory_site(&g,7,4)==4,"observatory: Lave I authored landmark and other generated sites agree");
 int identities=1;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  int count=0,id=observatory_site(&g,sys,body);for(int site=2;site<10;site++)if(site!=6&&field_site_kind(&g,sys,body,site)==FIELD_DISH)count++;
  identities&=count<=1&&(count==0?id<0:id>=2);
 }
 CHECK(identities,"observatory: one or no observatory per world, safe shared outcome bits");
 for(int choice=1;choice<=2;choice++){
  game_init(&g);launch(&g);g.planet=2;g.surface=2;int id=observatory_site(&g,7,2);g.pos=surface_poi(&g,id);
  int money=g.credits,discoveries=g.discoveries;
  CHECK(!observatory_resolve(&g,id,choice)&&g.credits==money,"observatory: cannot resolve without all three clues");
  g.surface_progress[7][2]|=OBS_CLUES;
  g.docked=1;CHECK(save_game(&g,"test-observatory.sav")&&load_game_file(&loaded,"test-observatory.sav")&&(loaded.surface_progress[7][2]&OBS_CLUES)==OBS_CLUES&&!observatory_choice(&loaded,7,2),"observatory: unfinished investigation survives save/load");g.docked=0;
  CHECK(!observatory_resolve(&g,id,0)&&!observatory_resolve(&g,id,3),"observatory: invalid choices rejected");
  CHECK(observatory_resolve(&g,id,choice)&&observatory_choice(&g,7,2)==choice,"observatory: either deliberate choice completes the work");
  int pay=150+(field_hash(field_profile(&g,7,2).seed+id)%36)*10;
  CHECK(g.credits==money+pay+(choice==1?200:0)&&g.discoveries==discoveries+(choice==2?2:1),"observatory: cash bonus versus extra discovery has exact outcomes");
  money=g.credits;discoveries=g.discoveries;
  CHECK(!observatory_resolve(&g,id,3-choice)&&g.credits==money&&g.discoveries==discoveries,"observatory: neither second reward nor changed decision on return");
  uint32_t state=g.surface_progress[7][2];g.docked=1;
  CHECK(save_game(&g,"test-observatory.sav")&&load_game_file(&loaded,"test-observatory.sav")&&loaded.surface_progress[7][2]==state,"observatory: clue and outcome state round-trips in V26");
  CHECK(!observatory_flags_valid(&g,7,2,(state&~(3u<<20))|(3u<<20))&&!observatory_flags_valid(&g,7,2,state&~OBS_CLUES),"observatory: invalid outcome and missing-clue saves rejected");
 }
 game_init(&g);g.surface_progress[7][2]=field_site_bit(2);
 CHECK(save_game(&g,"test-observatory.sav"),"observatory: completed legacy site fixture saved");
 static unsigned char ob_bytes[131072];FILE *ob_file=fopen("test-observatory.sav","rb");size_t ob_size=0;
 if(ob_file){ob_size=fread(ob_bytes,1,sizeof(ob_bytes),ob_file);fclose(ob_file);}
 int migrated=0;if(ob_size>8&&ob_size<sizeof(ob_bytes)){
  ob_bytes[4]=25;ob_file=fopen("test-observatory-v25.sav","wb");
  if(ob_file){migrated=fwrite(ob_bytes,1,ob_size-4,ob_file)==ob_size-4;fclose(ob_file);migrated=migrated&&save_seal("test-observatory-v25.sav");}
 }
 CHECK(migrated&&load_game_file(&loaded,"test-observatory-v25.sav")&&loaded.surface_progress[7][2]==field_site_bit(2)&&!observatory_choice(&loaded,7,2),"observatory: V25 completion migrates without inventing a branch or reopening rewards");
 remove("test-observatory.sav");remove("test-observatory.sav.bak");remove("test-observatory-v25.sav");
}
