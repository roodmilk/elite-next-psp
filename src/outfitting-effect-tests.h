{
 /* Tests use the real simulation with quiet, empty lanes. */
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,-20000};g.speed=0;g.freight_next=999;
 for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 g.fit[FIT_UTIL]=19;fit_rebuild(&g);g.hull=50;g.damaged=1;g.attacked=0;g.heat=0;
 for(int i=0;i<10;i++)game_tick(&g,.1f,0,0,0,0);
 CHECK(fabsf(g.hull-52)<.02f,"auto-repair repairs actual hull at two per second");
 g.attacked=5;float hull_before=g.hull;game_tick(&g,.1f,0,0,0,0);
 CHECK(g.hull==hull_before,"auto-repair cannot heal under attack");
 g.attacked=0;g.hull=99.9f;game_tick(&g,.1f,0,0,0,0);
 CHECK(g.hull==100&&!g.damaged,"auto-repair restores damaged systems only at full hull");
 for(int thermal=0;thermal<2;thermal++){
  game_init(&g);launch(&g);g.fit[FIT_UTIL]=18;fit_rebuild(&g);g.pos=(Vec3){0,0,-20000};g.speed=0;
  for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
  if(thermal){g.pos=add(g.bodies[0].pos,(Vec3){0,0,g.bodies[0].radius+3900});g.heat=100;}
  else {g.hull=0;g.damaged=1;g.dead=1;}
  game_tick(&g,.016f,0,0,0,0);
  CHECK(g.docked&&!g.dead&&g.hull>=25&&g.fit[FIT_UTIL]==FIT_EMPTY&&!(g.upgrades&16384),"escape pod recovers fatal/thermal damage and consumes its fitted module");
 }
 game_init(&g);g.fit[FIT_WPN]=20;fit_rebuild(&g);
 CHECK(laser_shot_damage(&g)==18&&mine_shot_damage(&g)==54,"mining cutter beats beam on rocks but remains weak against ships");
 for(int item=14;item<=15;item++){
  game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
  g.fit[FIT_FUEL]=item;fit_rebuild(&g);g.speed=0;g.fuel=20;
  g.pos=add(g.bodies[0].pos,(Vec3){0,0,g.bodies[0].radius+3900});float start=g.fuel;
  for(int i=0;i<10;i++)game_tick(&g,.1f,0,0,0,0);
  CHECK(fabsf(g.fuel-start-(item==15?.75f:.5f))<.01f,"both scoops add their advertised fuel near the sun");
 }
 game_init(&g);launch(&g);g.fit[FIT_DEF]=16;fit_rebuild(&g);g.incoming_missile=2;g.energy=100;
 CHECK(activate_ecm(&g)&&g.incoming_missile==0&&g.energy==82&&g.ecm_cd==18,"manual ECM breaks a lock for shield energy with cooldown");
 CHECK(!activate_ecm(&g)&&g.energy==82,"ECM cooldown prevents duplicate activation");
 game_init(&g);launch(&g);g.fit[FIT_UTIL]=17;fit_rebuild(&g);
 CHECK(flare_capacity(&g)==6&&!ecm_fitted(&g),"Chaff improves decoys without granting an ECM suite");
 game_init(&g);g.fit[FIT_DEF]=24;fit_rebuild(&g);
 CHECK(g.fit[FIT_DEF]==FIT_EMPTY&&!(g.upgrades&262144),"wrong-slot fitting cannot leak upgrade effects");
}

