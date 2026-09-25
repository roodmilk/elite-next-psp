/* Included inside game_tests: exercise mining through actual fire input. */
{
 game_init(&g);launch(&g);g.fit[FIT_WPN]=1;fit_rebuild(&g);g.pos=(Vec3){0,0,-10000};g.speed=0;g.yaw=g.pitch=g.roll=0;g.freight_next=999;
 for(int i=0;i<NPC_COUNT;i++){g.npc[i].alive=0;g.npc[i].cooldown=999;}
 for(int i=0;i<DEBRIS_COUNT;i++)g.debris[i].alive=0;
 Debris *rock=&g.debris[0];*rock=(Debris){.pos={0,0,-9400},.alive=1,.good=12,.qty=2,.rock=1,.life=1000,.health=72,.radius=70};
 int before=cargo_used(&g),money=g.credits;
 CHECK(!salvage(&g,DEBRIS_ID_MIN)&&rock->rock,"mining: intact rocks cannot be collected");
 game_tick(&g,.016f,0,0,0,1);
 CHECK(rock->rock&&rock->health<72&&rock->health>0&&rock->flash>0&&cargo_used(&g)==before,"mining: one laser hit damages and flashes without free cargo");
 float hp=rock->health;g.yaw=1;g.shot=0;game_tick(&g,.016f,0,0,0,1);
 CHECK(rock->health==hp,"mining: aiming away misses the asteroid");
 g.yaw=0;g.jump=2;CHECK(!mine_rock(&g,DEBRIS_ID_MIN)&&rock->health==hp,"mining: warp locks extraction");g.jump=0;
 NPC *behind=&g.npc[0];behind->alive=1;behind->pos=(Vec3){0,0,-9000};behind->health=80;behind->shield=40;behind->role=TRADERS;
 g.shot=0;game_tick(&g,.016f,0,0,0,1);
 CHECK(behind->shield==40&&g.legal==0&&rock->health<hp,"mining: nearer rock intercepts shots before a ship behind it");
 for(int i=1;i<DEBRIS_COUNT;i++)g.debris[i]=(Debris){.alive=1,.life=1000,.pos={8000,8000,8000}};
 behind->alive=0;g.shot=0;g.heat=0;game_tick(&g,.016f,0,0,0,1);
 CHECK(rock->alive&&!rock->rock&&rock->qty==2&&rock->good==12&&cargo_used(&g)==before&&g.credits==money,"mining: fracture releases ore even when every debris slot is occupied");
 CHECK(!mine_rock(&g,DEBRIS_ID_MIN)&&!salvage(&g,DEBRIS_ID_MIN),"mining: loose ore is not mined twice or collected outside 500 m");
 g.pos=rock->pos;CHECK(salvage(&g,DEBRIS_ID_MIN)&&g.tractor_time>0,"mining: collection starts the tractor beam");for(int i=0;i<50;i++)game_tick(&g,1.f/60,0,0,0,0);CHECK(g.cargo[12]==2&&!rock->alive&&!salvage(&g,DEBRIS_ID_MIN),"mining: collection gives the exact yield once");
}
{
 int routes=1,quiet=0,busy=0,layouts=0,counts[4]={0},ice=0;
 for(int sys=0;sys<256;sys++){
  game_init(&g);g.system=sys;game_spawn(&g);g.pos=(Vec3){0,0,0};
  int cap=freight_capacity(&g);counts[cap]++;if(!cap)quiet++;if(cap==3)busy++;
  for(int i=0;i<DEBRIS_COUNT;i++)if(g.debris[i].alive&&g.debris[i].rock==2)ice++;
  for(int i=8;i<36;i+=12){NPC *n=&g.npc[i];layouts|=1<<n->freight_style;
   if(freight_begin(&g,n,i,0)){
    for(int b=0;b<BODY_COUNT;b++)if(route_clearance(n->freight_berth,n->freight_gate,g.bodies[b].pos)<g.bodies[b].radius+n->radius+499)routes=0;
    for(int h=0;h<HUB_COUNT;h++)if(route_clearance(n->freight_berth,n->freight_gate,hub_position(&g,h))<n->radius+599)routes=0;
    if(length(sub(n->pos,g.pos))<2500||n->freight_peer==sys||distance_ly(&g,sys,n->freight_peer)>10)routes=0;
   }
  }
 }
 CHECK(routes,"freight: routes across all 256 systems clear worlds, hubs and player; origins are reachable");
 CHECK(quiet>0&&busy>0&&counts[1]>0&&counts[2]>0,"freight: galaxy includes empty, light, medium and busy shipping systems");
 CHECK(layouts==7&&ice>0,"world: all three freight hulls and physical ice belts are generated");
 game_init(&g);launch(&g);NPC *n=&g.npc[8];g.freight_next=10000;g.freight_gap=0;
 for(int i=0;i<NPC_COUNT;i++)if(i!=8)g.npc[i].alive=0;
 CHECK(n->alive&&n->freight_state==FREIGHT_INBOUND,"freight: Lave begins with one ongoing delivery");
 int incoming=n->freight_good,quantity=n->freight_qty;
 for(int i=0;i<GOODS;i++)g.stock[i]=0;
 n->pos=add(n->freight_berth,mul(norm(sub(n->freight_gate,n->freight_berth)),1));n->dir=norm(sub(n->freight_berth,n->pos));
 freight_update(&g,.05f);
 CHECK(n->freight_state==FREIGHT_SERVICE&&g.stock[incoming]>=quantity-n->freight_qty,"freight: arrival at the berth performs a bounded cargo transfer");
 int exported=n->freight_good;
 CHECK(g.stock[incoming]==quantity-(incoming==exported?n->freight_qty:0)&&g.stock[exported]>=0,"freight: stock changes equal the imported and exported manifest");
 int inventory[GOODS];memcpy(inventory,g.stock,sizeof(inventory));
 for(int k=0;k<100;k++)freight_update(&g,.1f);
 CHECK(!memcmp(inventory,g.stock,sizeof(inventory)),"freight: loitering at the berth never repeats a delivery");
 for(int k=0;k<3000&&n->alive;k++)freight_update(&g,.1f);
 CHECK(!n->alive&&n->freight_state==FREIGHT_ABSENT&&n->freight_timer>0&&g.freight_gap>0,"freight: loading, outward flight and warp complete without teleporting back");
 g.freight_next=0;for(int k=0;k<400;k++)freight_update(&g,.1f);
 CHECK(!n->alive,"freight: a departing ship cannot instantly respawn");
 n->freight_timer=0;g.freight_gap=0;g.freight_next=0;freight_update(&g,.1f);
 CHECK(n->alive&&n->freight_state==FREIGHT_ARRIVING&&g.freight_next>=150&&g.freight_gap>40,"freight: fresh arrival uses a visible warp phase and multi-minute schedule");
 freight_update(&g,3);
 CHECK(n->freight_state==FREIGHT_INBOUND,"freight: arrival animation transitions into physical flight");
 n->shield=0;hit(&g,8,1000,1);g.freight_next=0;g.freight_gap=0;freight_update(&g,.1f);
 CHECK(!n->alive&&n->freight_timer>230,"freight: destroying a hull delays its replacement");
 game_init(&g);launch(&g);n=&g.npc[8];n->alive=1;n->freighter=1;n->health=850;n->shield=0;n->target=-2;n->cooldown=0;n->pos=(Vec3){0,0,800};n->dir=(Vec3){0,0,-1};g.pos=(Vec3){0,0,0};g.energy=100;g.attacked=0;
 freight_update(&g,.05f);CHECK(n->target==-2&&g.attacked>0&&g.energy<100,"freight: damaged hauler returns fire on the attacker");
 int shape_ok=1;
 for(int style=0;style<3;style++){
  n->freight_style=style;n->pos=(Vec3){200,300,400};n->dir=norm((Vec3){1,0,1});Vec3 e=freight_extent(n);
  Vec3 a=freight_world(n,(Vec3){0,0,-2000}),b=freight_world(n,(Vec3){0,0,2000});
  float t=freight_intersection(n,a,b,20);if(t<0||t>1)shape_ok=0;
  a=freight_world(n,(Vec3){e.x+60,0,-2000});b=freight_world(n,(Vec3){e.x+60,0,2000});if(freight_intersection(n,a,b,20)>=0)shape_ok=0;
 }
 CHECK(shape_ok,"freight: rotated swept hull catches fast passes but clears the empty space beside it");
 game_init(&g);n=&g.npc[8];n->freight_good=7;n->freight_qty=8;g.stock[7]=150;
 freight_trade(&g,n);
 CHECK(g.stock[7]==150,"freight: stock saturation never erases goods already sold by the player");
 game_init(&g);launch(&g);n=&g.npc[8];n->freight_good=8;n->freight_qty=7;n->shield=0;
 for(int i=0;i<DEBRIS_COUNT;i++)g.debris[i].alive=0;
 hit(&g,8,1000,1);int recovered=0;
 for(int i=0;i<DEBRIS_COUNT;i++)if(g.debris[i].alive&&!g.debris[i].wreck&&g.debris[i].good==8)recovered+=g.debris[i].qty;
 CHECK(recovered==7,"freight: destroyed hauler releases its actual cargo manifest");
 game_init(&g);launch(&g);n=&g.npc[8];n->pos=(Vec3){0,0,-10000};n->dir=(Vec3){0,0,1};n->freight_state=FREIGHT_SERVICE;n->freight_timer=100;n->freight_gate=(Vec3){0,0,0};n->freight_berth=n->pos;g.freight_next=999;
 for(int i=0;i<NPC_COUNT;i++)if(i!=8){g.npc[i].alive=0;g.npc[i].cooldown=999;}
 g.pos=(Vec3){0,0,-10650};g.yaw=g.pitch=g.roll=0;g.speed=player_ships[g.ship].speed*20;g.boost=1;float energy=g.energy;
 game_tick(&g,.1f,0,0,0,0);
 CHECK(g.collision>0&&g.energy<energy&&!g.boost&&g.speed==0,"freight: actual boosted flight collides with the visible capital hull");
}
