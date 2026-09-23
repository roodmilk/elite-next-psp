
{
 FILE *a=fopen("bar-site-audit.txt","w");int route_ok=1,rocky=0;
 for(int body=1;body<BODY_COUNT;body++){
  game_init(&g);g.system=39;launch(&g);
  if(a)fprintf(a,"BODY %d TYPE %d SEED %u NAME %s\n",body,g.bodies[body].type,g.bodies[body].seed,g.bodies[body].name);
  if(g.bodies[body].type!=ROCKY)continue;
  rocky++;g.approach=body;int entry=enter_planet(&g);Vec3 pad=surface_site(&g,1);
  Vec3 site={pad.x,terrain_height(&g,pad.x,pad.z+110),pad.z+110};
  g.pos=pad;g.speed=8;int landing=land_planet(&g)&&eva_toggle(&g);int steps=0;
  float dx=site.x-g.pos.x,dz=site.z-g.pos.z;
  while(dx*dx+dz*dz>19*19&&steps<200){g.yaw=atan2f(dx,dz);game_eva_tick(&g,.05f,0,0,1,0,0);steps++;dx=site.x-g.pos.x;dz=site.z-g.pos.z;}
  float distance=sqrtf(dx*dx+dz*dz);int home=0;
  while(!eva_can_board(&g)&&home<200){g.yaw=atan2f(g.ship_pos.x-g.pos.x,g.ship_pos.z-g.pos.z);game_eva_tick(&g,.05f,0,0,1,0,0);home++;}
  float hazard=g.hazard;int boarded=eva_toggle(&g)&&g.surface==1;
  int dry=!terrain_is_water(&g,site.x,site.z);
  int orbit=takeoff_planet(&g);leave_planet(&g);orbit &= g.planet<0;
  int pass=dry&&entry&&landing&&steps<200&&distance<=19&&home<200&&boarded&&orbit&&!g.dead&&hazard==0;
  route_ok &= pass;
  if(a)fprintf(a,"SITE body=%d pad=(%.2f,%.2f,%.2f) recorder=(%.2f,%.2f,%.2f) walk_steps=%d closest=%.2f return_steps=%d hazard=%.2f board=%d orbit=%d PASS=%d\n",body,pad.x,pad.y,pad.z,site.x,site.y,site.z,steps,distance,home,hazard,boarded,orbit,pass);
 }
 if(a)fclose(a);
 CHECK(a&&rocky&&route_ok,"bar site diagnostic: Reorte rocky pad-offset sites allow real walking and boarding/orbit return without exposure");
 game_init(&g);launch(&g);
}
