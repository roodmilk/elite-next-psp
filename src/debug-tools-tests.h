/* Runtime-only Debug Tools contracts. Included by game_tests with g/loaded,
 * CHECK and the save helpers in scope. */
{
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 int far=-1;float far_distance=-1.f;
 for(int i=0;i<256;i++)if(i!=g.system){float d=distance_ly(&g,g.system,i);if(d>far_distance){far_distance=d;far=i;}}
 g.fuel=.5f;g.destination=far;g.debug_flags=DEBUG_MODIFIED|DEBUG_UNLIMITED_RANGE|DEBUG_UNLIMITED_FUEL;
 int jumps=0;
 CHECK(route_next_hop(&g,far,&jumps)==far&&jumps==1,"debug unlimited range plots any system as one direct jump");
 float protected_fuel=g.fuel;
 CHECK(jump_start(&g),"debug unlimited range starts a jump beyond fitted range and available fuel");
 for(int i=0;i<500&&g.jump>0;i++)game_tick(&g,1.f/60.0f,0,0,0,0);
 CHECK(g.system==far&&fabsf(g.fuel-protected_fuel)<.001f,"debug unlimited fuel prevents hyperspace fuel depletion");

 int next=(far+1)&255;if(next==g.system)next=(next+1)&255;
 g.destination=next;g.fuel=.5f;g.debug_flags=DEBUG_MODIFIED|DEBUG_UNLIMITED_RANGE;
 CHECK(jump_start(&g),"debug unlimited range remains independent of unlimited fuel");
 for(int i=0;i<500&&g.jump>0;i++)game_tick(&g,1.f/60.0f,0,0,0,0);
 CHECK(g.system==next&&g.fuel==0,"jump fuel depletes normally when only unlimited range is enabled");
 g.destination=(next+1)&255;g.debug_flags=DEBUG_MODIFIED;
 CHECK(!jump_start(&g),"disabling unlimited range immediately restores normal fuel and range checks");

 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 g.fuel=10;g.boost=1;g.debug_flags=DEBUG_MODIFIED|DEBUG_UNLIMITED_FUEL;
 for(int i=0;i<60;i++)game_tick(&g,1.f/60.0f,0,0,0,0);
 CHECK(fabsf(g.fuel-10)<.001f,"debug unlimited fuel prevents boost depletion");
 g.debug_flags=DEBUG_MODIFIED;
 for(int i=0;i<60;i++)game_tick(&g,1.f/60.0f,0,0,0,0);
 CHECK(g.fuel<10,"disabling unlimited fuel immediately restores boost depletion");

 Game debug_loaded;game_init(&g);g.docked=1;g.debug_flags=DEBUG_FLAGS_MASK;
 CHECK(save_game(&g,"test-debug-runtime.sav")&&load_game(&debug_loaded,"test-debug-runtime.sav")&&debug_loaded.debug_flags==0,"debug toggles are runtime-only and never persist in commander saves");
 remove("test-debug-runtime.sav");remove("test-debug-runtime.sav.bak");
}
