{
 /* Ordinary boosting must not hit thermal danger within a short travel burst. */
 game_init(&g);launch(&g);g.pos=(Vec3){0,100000,-100000};g.speed=0;g.heat=0;g.boost=1;
 for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 for(int i=0;i<240;i++)game_tick(&g,1.f/60,0,0,1,0);
 CHECK(g.boost&&g.heat>0&&g.heat<40,"boost: four-second travel burst stays below 40 heat at default engine power");
 /* Neutral and friendly locks are valid; existing hit() owns legal consequences. */
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,-20000};
 for(int role=0;role<FACTION_COUNT;role++){
  g.npc[0].alive=1;g.npc[0].role=role;g.npc[0].target=-1;
  g.npc[0].pos=add(g.pos,(Vec3){0,0,1200});g.missiles=2;g.missile_time=0;
  CHECK(fire_missile(&g,BODY_COUNT+1)&&g.missiles==1&&g.missile_target==0,"missiles: every ship faction accepts a non-hostile lock");
 }
 g.missile_time=0;g.npc[0].alive=0;
 CHECK(!fire_missile(&g,BODY_COUNT+1)&&g.missiles==1,"missiles: dead targets do not consume ammunition");
 g.npc[0].alive=1;g.npc[0].pos=add(g.pos,(Vec3){0,0,13000});
 CHECK(!fire_missile(&g,BODY_COUNT+1)&&g.missiles==1,"missiles: non-hostile targets still obey range limit");
 game_init(&g);launch(&g);for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;g.pos=(Vec3){0,0,-20000};g.speed=0;
 CHECK(!activate_ecm(&g)&&!dump_heat_sink(&g),"tools: unfitted modules cannot activate");
 g.incoming_missile=1.2f;g.incoming_source=-1;int stock=g.flare_charges;
 CHECK(deploy_flare(&g)&&g.flare_charges==stock-1&&dot(sub(g.flare_pos,g.pos),forward(&g))<0,"tools: decoy consumes one charge and deploys behind the ship");
 game_tick(&g,.016f,0,0,0,0);
 CHECK(g.incoming_missile==0&&!deploy_flare(&g),"tools: timed flare diverts missile and cannot repeat during cooldown");
 g.flare_fx=g.flare_cd=0;g.incoming_missile=3.5f;deploy_flare(&g);
 for(int i=0;i<19;i++)game_tick(&g,.1f,0,0,0,0);
 CHECK(g.incoming_missile>0&&g.flare_fx==0,"tools: deploying too early cannot protect indefinitely");
 g.incoming_missile=0;
 g.flare_fx=g.flare_cd=0;g.flare_charges=0;g.flare_reload=19.95f;game_tick(&g,.1f,0,0,0,0);
 CHECK(g.flare_charges==1,"tools: standard decoy bank recharges one cell per 20 seconds");
 g.flare_charges=0;CHECK(!deploy_flare(&g),"tools: empty decoy bank cannot deploy");
 g.fit[FIT_DEF]=16;fit_rebuild(&g);g.energy=17;g.ecm_cd=0;CHECK(!activate_ecm(&g)&&g.energy==17,"tools: insufficient shield energy blocks ECM");
 g.fit[FIT_UTIL]=9;fit_rebuild(&g);g.heat=90;g.heat_sink_cd=0;g.laser=1;game_tick(&g,.016f,0,0,0,1);
 CHECK(g.heat_sink_cd==0,"tools: firing no longer auto-dumps a heat sink");
 CHECK(dump_heat_sink(&g)&&!dump_heat_sink(&g),"tools: manual heat sink accepts once then enforces cooldown");
 g.energy=100;g.incoming_missile=1.75f;g.ecm_cd=0;game_tick(&g,.1f,0,0,0,0);
 CHECK(g.incoming_missile>0,"tools: automatic ECM waits until missile enters defence window");
 game_tick(&g,.1f,0,0,0,0);game_tick(&g,.1f,0,0,0,0);
 CHECK(g.incoming_missile==0&&g.ecm_cd>17&&g.energy<83,"tools: fitted ECM protects automatically with energy cost and cooldown");
 g.docked=1;g.flare_charges=3;g.heat_sink_cd=0;
 CHECK(!deploy_flare(&g)&&!activate_ecm(&g)&&!dump_heat_sink(&g),"tools: docked state forbids all tool discharge");
 game_init(&g);
}
