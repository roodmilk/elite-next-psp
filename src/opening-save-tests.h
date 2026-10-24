/* Narrative checkpoints use the commander slot and survive both object choices. */
{
 static Game og,orr;int rounds=1;
 for(int stage=1;stage<=OPENING_STAGE_COUNT;stage++){
  game_init(&og);og.opening.stage=stage;og.opening.flags=OF_OBJECT|OF_PIZZA;
  og.opening.choice=stage%2+1;if(og.opening.choice==1)og.opening.flags|=OF_SHOWN;
  if(stage>=OP_WAKE)og.opening.flags|=OF_DISPLACED|(15<<16);og.opening.flags|=(1<<12);
  og.opening.counter=2;og.opening.distance=123;og.opening.elapsed=4;
  og.docked=0;og.pos=(Vec3){120,80,-900};og.yaw=.4f;og.pitch=.1f;og.roll=.2f;og.speed=80;
  og.hull=93;og.damaged=1;og.cargo[0]=1;og.cargo[16]=1;
  opening_world_apply(&og);
  if(!save_game(&og,"test-opening.sav")||!load_game_file(&orr,"test-opening.sav")||
   orr.opening.stage!=stage||orr.opening.flags!=og.opening.flags||orr.opening.choice!=og.opening.choice||
   orr.opening.counter!=2||orr.opening.distance!=123||orr.opening.elapsed!=4||
   orr.docked!=og.docked||orr.pos.x!=og.pos.x||orr.pos.z!=og.pos.z||orr.hull!=93||
   orr.cargo[0]!=1||orr.cargo[16]!=1||orr.dock_stage||orr.planet_sequence)rounds=0;
 }
 CHECK(rounds,"opening: every checkpoint, shown/hidden object and displaced cargo round-trip");
 game_init(&og);og.opening.stage=OP_PEP;og.opening.flags=OF_OBJECT;og.opening.choice=2;
 opening_world_apply(&og);og.docked=0;og.planet=1;og.surface=2;
 og.pos=(Vec3){12,44,88};og.ship_pos=(Vec3){90,30,110};og.orbit_pos=(Vec3){900,0,700};
 og.orbit_yaw=.7f;og.orbit_speed=50;
 CHECK(save_game(&og,"test-opening.sav")&&load_game(&orr,"test-opening.sav")&&orr.surface==2&&orr.planet==1&&orr.ship_pos.x==90&&orr.orbit_pos.x==900&&orr.orbit_speed==50,"opening: countryside EVA restores ship and return-to-orbit state");
 og.dock_stage=1;CHECK(!save_game(&og,"test-opening.sav"),"opening: docking animation cannot overwrite checkpoint");og.dock_stage=0;
 og.planet_sequence=1;CHECK(!save_game(&og,"test-opening.sav"),"opening: planetary animation cannot overwrite checkpoint");og.planet_sequence=0;
 og.jump=1;CHECK(!save_game(&og,"test-opening.sav"),"opening: hyperspace cannot overwrite checkpoint");og.jump=0;
 og.opening.stage=OPENING_STAGE_COUNT+1;
 CHECK(!save_game(&og,"test-opening.sav")&&load_game(&orr,"test-opening.sav")&&orr.opening.stage==OP_PEP,"opening: invalid stage rejected without replacing commander");
 game_init(&og);og.docked=0;CHECK(!save_game(&og,"test-opening.sav"),"opening: ordinary flight save guard remains docked-only");
 og.docked=1;og.opening.stage=OP_FREE;og.opening.flags=OF_OBJECT|OF_PIZZA|OF_DISPLACED|OF_NEWS;
 CHECK(save_game(&og,"test-opening.sav")&&load_game(&orr,"test-opening.sav")&&orr.opening.stage==OP_FREE&&orr.opening.flags==og.opening.flags,"opening: completed story retains permanent flags");
 /* Strip only V27 data to manufacture a properly sealed V26 migration fixture. */
 static unsigned char opening_bytes[262144];size_t opening_n=0;FILE *of=fopen("test-opening.sav","rb");
 if(of){opening_n=fread(opening_bytes,1,sizeof(opening_bytes),of);fclose(of);}
 int old_ok=opening_n>OPENING_SAVE_BYTES+4&&opening_n<sizeof(opening_bytes);
 if(old_ok){opening_bytes[4]=26;of=fopen("test-opening-v26.sav","wb");if(of){size_t payload=opening_n-SAVE_V28_EXPANSION_BYTES-OPENING_SAVE_BYTES-(FIT_V29_BYTES+MODULE_STORE_BYTES+RUMOUR_SAVE_BYTES+NEWSPAPER_REWARD_SAVE_BYTES+EXPEDITION_SAVE_BYTES+(ERL_SAVE_BYTES+DB_SAVE_BYTES))-4;old_ok=fwrite(opening_bytes,1,payload,of)==payload;fclose(of);old_ok=old_ok&&save_seal("test-opening-v26.sav");}else old_ok=0;}
 CHECK(old_ok&&load_game(&orr,"test-opening-v26.sav")&&!orr.opening.stage,"opening: V26 commanders migrate without starting the opening");
 game_init(&og);og.opening.stage=OP_WAKE;og.opening.flags=OF_OBJECT|OF_PIZZA|OF_DISPLACED;og.cargo[0]=og.cargo[16]=1;
 int recovery=save_game(&og,"test-opening.sav");og.opening.stage=OP_LONELY;recovery=recovery&&save_game(&og,"test-opening.sav");
 of=fopen("test-opening.sav","rb+");if(of){fseek(of,-8,SEEK_END);int byte=fgetc(of);fseek(of,-1,SEEK_CUR);fputc(byte^1,of);fclose(of);}else recovery=0;
 CHECK(recovery&&!load_game_file(NULL,"test-opening.sav")&&load_game(&orr,"test-opening.sav")&&orr.opening.stage==OP_WAKE&&orr.cargo[0]==1&&orr.cargo[16]==1,"opening: corrupted snapshot fails CRC and backup restores prior story checkpoint");
 remove("test-opening.sav");remove("test-opening.sav.bak");remove("test-opening.sav.tmp");remove("test-opening-v26.sav");
}
