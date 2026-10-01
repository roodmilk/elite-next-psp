{
 for(int mode=0;mode<3;mode++){
  game_init(&g);launch(&g);g.pos=(Vec3){0,100000,-100000};g.yaw=g.pitch=g.roll=0;g.speed=0;
  for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
  NPC *n=&g.npc[0];n->alive=1;n->freighter=0;n->role=PIRATES;n->target=-1;n->radius=30;n->health=80;n->shield=0;n->cooldown=1000;n->bounty_slot=-1;
  n->pos=add(g.pos,(Vec3){mode==1?1800:0,0,mode==2?-2200:1800});
  Vec3 target=n->pos;CHECK(fire_missile(&g,NPC_ID_MIN),"missile feedback: launch accepts forward, side and rear ship locks");
  Vec3 muzzle=g.missile_pos,dir=g.missile_dir;
  CHECK(dot(sub(muzzle,g.pos),forward(&g))>30&&dot(dir,forward(&g))>.999f,"missile feedback: visible muzzle starts ahead and faces forward");
  for(int frame=0;frame<4;frame++){n->pos=target;game_tick(&g,.03f,0,0,0,0);}
  CHECK(dot(g.missile_dir,dir)>.999f&&g.missile_pos.z>muzzle.z,"missile feedback: clearance phase flies forward before homing");
  for(int frame=0;frame<240&&g.missile_time>0;frame++){n->pos=target;game_tick(&g,mode==2?.05f:1.f/60,0,0,0,0);}
  CHECK(!n->alive&&g.missile_time<=0,"missile feedback: bounded homing and swept hit reach target at 60/20 Hz");
 }
 game_init(&g);launch(&g);g.speed=0;g.boost=0;g.heat=0;g.energy=100;g.hull=100;
 for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
 g.pos=add(g.bodies[0].pos,(Vec3){0,0,-g.bodies[0].radius-900});
 for(int i=0;i<120;i++)game_tick(&g,1.f/60,0,0,0,0);
 CHECK(g.heat>=85&&g.energy<100,"sun: close exposure rapidly heats hull and drains shields");
 g.energy=0;g.heat=95;float hull_before=g.hull;
 for(int i=0;i<15;i++)game_tick(&g,1.f/60,0,0,0,0);
 CHECK(g.hull<hull_before&&!g.attacked,"sun: unshielded exposure damages hull without inventing an attacker");
 game_init(&g);g.fire_bearing_time[0]=2;game_spawn(&g);
 CHECK(g.fire_bearing_time[0]==0,"incoming fire: entering a system clears old direction hints");
}
