/* Included inside game_tests: approach must be a safe, reversible modal. */
{
 int boundaries_ok=1,escapes_ok=1,returns_ok=1;
 for(int body=1;body<BODY_COUNT;body++){
  game_init(&g);launch(&g);
  for(int n=0;n<NPC_COUNT;n++)g.npc[n].alive=0;
  Body *world=&g.bodies[body];
  g.pos=add(world->pos,(Vec3){0,0,-world->radius-65});
  g.yaw=g.pitch=0;g.speed=200;g.energy=75;g.heat=20;
  g.missile_time=5;g.shot=0;
  game_tick(&g,.1f,0,0,0,1);
  boundaries_ok &= g.approach==body&&g.speed==0&&!g.boost&&g.energy==75&&g.heat==20&&g.shots==0&&g.missile_time==5;
  Vec3 stopped=g.pos;
  g.incoming_missile=.01f;float clock=g.time;
  game_tick(&g,.1f,1,1,1,1);
  boundaries_ok &= g.time==clock&&g.energy==75&&g.incoming_missile==.01f&&length(sub(g.pos,stopped))==0;
  g.incoming_missile=0;g.missile_time=0;
  turn_back(&g);float before=length(sub(g.pos,world->pos));
  for(int frame=0;frame<60;frame++)game_tick(&g,1.f/60,0,0,0,0);
  escapes_ok &= g.approach<0&&length(sub(g.pos,world->pos))>before+90&&dot(forward(&g),norm(sub(g.pos,world->pos)))>.99f;
  if(world->type!=GAS){
   g.pos=add(world->pos,(Vec3){0,0,-world->radius-500});g.yaw=g.pitch=0;
   Vec3 orbit=g.pos;
   returns_ok &= approach_planet(&g,body)&&enter_planet(&g);
   leave_planet(&g);
   returns_ok &= g.planet<0&&length(sub(g.pos,orbit))<1&&dot(forward(&g),norm(sub(g.pos,world->pos)))>.99f;
  }
 }
 CHECK(boundaries_ok,"planet boundary stops weapons and pauses threats for every non-sun body");
 CHECK(escapes_ok,"Circle recovery clears every non-sun boundary without re-prompting");
 CHECK(returns_ok,"every landable body restores its orbit position facing away");
 game_init(&g);launch(&g);g.approach=1;g.pos=g.bodies[1].pos;
 turn_back(&g);
 CHECK(length(sub(g.pos,g.bodies[1].pos))>g.bodies[1].radius+60&&isfinite(g.yaw)&&isfinite(g.pitch),"turn-away recovers an embedded ship with a finite outward heading");
 game_init(&g);launch(&g);g.pos=add(g.bodies[1].pos,(Vec3){0,0,-g.bodies[1].radius-500});g.yaw=g.pitch=0;
 g.dock_stage=1;CHECK(!approach_planet(&g,1),"planet approach cannot replace docking guidance");
 g.dock_stage=0;g.police_stop=1;CHECK(!approach_planet(&g,1),"planet approach cannot replace a police stop");
 game_init(&g);launch(&g);
 Body *sun=&g.bodies[0];Vec3 outside=add(sun->pos,(Vec3){0,0,-sun->radius-200});g.pos=sun->pos;
 world_collision(&g,outside);
 CHECK(g.approach<0&&g.energy==90&&g.collision>0,"sun collisions remain damaging and never offer surface flight");
 game_init(&g);launch(&g);
}
