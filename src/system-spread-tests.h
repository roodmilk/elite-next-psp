{
 int spacing=1,bearings=1,safe=1,deterministic=1,offaxis=0,long_routes=0;
 float min_gap=1e9f,max_trip=0;
 for(int sys=0;sys<256;sys++){
  game_init(&g);g.system=sys;game_spawn(&g);system_arrival(&g,(sys+37)%256);
  Vec3 spawn=g.pos;float heading=g.yaw;system_arrival(&g,(sys+37)%256);
  deterministic&=length(sub(spawn,g.pos))==0&&heading==g.yaw;
  int front=0,back=0;
  for(int b=1;b<BODY_COUNT;b++){float dotview=dot(forward(&g),norm(sub(g.bodies[b].pos,g.pos)));front+=dotview>0;back+=dotview<0;}
  bearings&=front>0&&back>0;
  offaxis+=dot(forward(&g),norm(sub(hub_position(&g,0),g.pos)))<.7f;
  for(int b=0;b<BODY_COUNT;b++){
   safe&=length(sub(g.bodies[b].pos,g.pos))>g.bodies[b].radius+9000;
   for(int j=b+1;j<BODY_COUNT;j++){float gap=length(sub(g.bodies[b].pos,g.bodies[j].pos))-g.bodies[b].radius-g.bodies[j].radius;min_gap=fminf(min_gap,gap);spacing&=gap>30000;}
   for(int h=0;h<HUB_COUNT;h++)safe&=length(sub(g.bodies[b].pos,hub_position(&g,h)))>g.bodies[b].radius+2000;
  }
  for(int h=0;h<HUB_COUNT;h++)safe&=length(sub(g.pos,hub_position(&g,h)))>2500;
  for(int slot=8;slot<36;slot+=12){NPC *n=&g.npc[slot];if(freight_begin(&g,n,slot,0)){float trip=length(sub(n->freight_gate,n->freight_berth));long_routes+=trip>15000;max_trip=fmaxf(max_trip,trip);}}
 }
 CHECK(spacing,"system space: every pair of bodies has over 30 km clear separation in all 256 systems");
 CHECK(bearings&&offaxis>180,"system arrival: planets occupy front and rear hemispheres; station is not forced ahead");
 CHECK(safe&&deterministic,"system arrival: deterministic route bearings clear bodies and stations across all systems");
 CHECK(long_routes>600&&max_trip>50000,"system freight: most routes cross tens of kilometres to planetary transfer space");
 fprintf(f,"INFO system minimum body gap %.0f m; longest freight leg %.0f m; long routes %d\n",min_gap,max_trip,long_routes);
 game_init(&g);launch(&g);g.freight_next=10000;g.ai_phase=-1;
 for(int i=0;i<NPC_COUNT;i++){g.npc[i].alive=0;g.npc[i].cooldown=10000;}
 NPC *ship=&g.npc[0];ship->alive=1;ship->role=TRADERS;ship->target=-1;ship->route_id=1;ship->route_leg=2;ship->route_step=1;ship->pos=traffic_world_point(&g,2);ship->dir=norm(sub(hub_position(&g,0),ship->pos));
 game_tick(&g,.016f,0,0,0,0);
 CHECK(ship->route_leg==1&&ship->route_step==-1&&ship->route_wait>0,"traffic routes: trader reaches a planet and switches to the return leg");
 ship->route_wait=0;ship->route_leg=0;ship->pos=g.traffic_nodes[1][0];g.ai_phase=-1;game_tick(&g,.016f,0,0,0,0);
 CHECK(ship->route_leg==1&&ship->route_step==1,"traffic routes: trader at hub starts another real world run");
 ship->role=EXPLORERS;ship->waypoint=2;ship->pos=traffic_world_point(&g,2);g.ai_phase=-1;game_tick(&g,.016f,0,0,0,0);
 CHECK(ship->waypoint==3,"traffic routes: explorer reaches its fixed waypoint and advances to another planet");
 game_init(&g);
}
