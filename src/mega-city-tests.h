/* Isolated opt-in native checks; no commander saves, RNG or gameplay writes. */
int mega_city_run_checks(FILE *f){
 Game *g=calloc(1,sizeof(*g));if(!g)return 1;game_init(g);int failures=0,systems=0,routes=0;unsigned shape_mask=0,crown_mask=0;
#define MC_CHECK(c,label) do{int mc_pass=(c);fprintf(f,"%s %s\n",mc_pass?"PASS":"FAIL",label);failures+=!mc_pass;}while(0)
 for(int s=0;s<256;s++){
  g->system=s;if(station_class(g)!=STATION_MEGA)continue;systems++;
  const MegaCity *m=mega_city_for(g);crown_mask|=1u<<((m->seed>>18)&3);int solid=1,curves=1,deterministic=1;float minx=0,maxx=0;
  MegaBlock identity[MEGA_BLOCK_MAX];int count=m->n;memcpy(identity,m->b,sizeof(identity));
  int original=g->system;g->system=7;mega_city_for(g);g->system=original;m=mega_city_for(g);
  for(int k=0;k<count-3;k++)deterministic&=identity[k].shape==m->b[k].shape&&identity[k].c.x==m->b[k].c.x&&identity[k].c.y==m->b[k].c.y&&identity[k].c.z==m->b[k].c.z&&identity[k].e.x==m->b[k].e.x&&identity[k].e.y==m->b[k].e.y&&identity[k].e.z==m->b[k].e.z;
  MC_CHECK(deterministic,"system architecture is stable after leaving and returning");
  for(int k=0;k<m->n;k++){
   MegaBlock b=m->b[k];float t;
   solid&=mega_city_hit(g,(Vec3){b.c.x-b.e.x-400,b.c.y,b.c.z},(Vec3){b.c.x+b.e.x+400,b.c.y,b.c.z},0,&t);
   minx=fminf(minx,b.c.x-b.e.x);maxx=fmaxf(maxx,b.c.x+b.e.x);
   shape_mask|=1u<<b.shape;
   if(b.shape){
    Vec3 corner=add(b.c,(Vec3){b.e.x*.95f,b.e.y*.95f,b.e.z*.95f});
    curves&=mega_box_hit(b.c,b.c,&b,0,0)&&!mega_box_hit(corner,corner,&b,0,0);
    const MegaGeometry *mesh=mega_geometry_for(b.shape);
    for(int v=0;v<mesh->vertices;v++)for(int face=0;face<mesh->faces;face++)curves&=dot(mesh->f[face].normal,mesh->v[v])<=mesh->f[face].plane+.0001f;
   }
  }
  MC_CHECK(curves,"faceted hulls are convex and their empty bounding-box corners stay flyable");
  MC_CHECK(m->n>=50&&m->n<=MEGA_BLOCK_MAX&&maxx-minx>=16600,"bounded solid city spans at least 16.6 km");
  MC_CHECK(solid,"boost-length sweeps hit every tower and bridge");
  MC_CHECK(!mega_city_hit(g,(Vec3){0,0,-12000},(Vec3){0,0,-station_half_for(g,0)-1},0,0),"central docking and launch corridor remains open");
  MC_CHECK(station_angle(g)==0,"city streets do not rotate across the pilot");
  static const Vec3 starts[]={{9000,0,2500},{-9000,0,2500},{0,6200,2000},{0,-6200,2000},{0,0,7600},{0,0,-6000},{-4200,0,1800},{4200,0,1800},{-4200,300,3900},{4200,-300,3900},{0,1900,2000},{0,-1900,2000}};
  for(unsigned k=0;k<sizeof(starts)/sizeof(*starts);k++){
   if(mega_city_hit(g,starts[k],starts[k],80,0)){MC_CHECK(0,"test departure point has free clearance");continue;}
   g->pos=add(starts[k],(Vec3){0,0,STATION_Z});g->docked=g->dead=g->dock_stage=g->legal=0;g->planet=-1;g->jump=0;g->police_stop=0;g->yaw=2;g->pitch=.2f;g->roll=.6f;g->time=50;g->speed=0;g->boost=0;
   int ok=dock(g),ticks=0;
   while(ok&&g->dock_stage==1&&ticks++<6000){g->time+=.05f;docking_tick(g,.05f);}
   MC_CHECK(ok&&g->dock_stage==2&&!g->dead,"auto-dock routes from city district to a real aperture");
   if(g->dock_stage==2){docking_tick(g,3.1f);docking_tick(g,1.5f);MC_CHECK(g->docked,"city guidance completes normal docking services");}
   routes++;
  }
  /* Front slit accepts manual docking, a side facade never does. */
  g->dock_stage=g->dead=g->docked=0;g->time=0;g->roll=g->yaw=g->pitch=0;g->speed=100;g->boost=0;
  float half=station_half_for(g,0);g->pos=(Vec3){0,0,STATION_Z-half+2};
  MC_CHECK(station_collision(g,(Vec3){0,0,STATION_Z-half-10})&&g->dock_stage==2&&!g->dead,"manual central port entry works");
  g->dead=g->dock_stage=0;MegaBlock b=m->b[1];g->pos=add(b.c,(Vec3){0,0,STATION_Z});
  MC_CHECK(station_collision(g,add(b.c,(Vec3){-b.e.x-500,0,STATION_Z}))&&g->dead&&!g->dock_stage,"district hulls collide instead of falsely docking");
 }
 MC_CHECK((shape_mask&((1u<<MC_DOCK)-1))==((1u<<MC_DOCK)-1),"capital catalogue contains domes, spheres, pods, octagons, bevels and pyramids");
 MC_CHECK(systems>0&&routes>100,"catalogue covers all mega-capitals and over 100 guidance routes");
 MC_CHECK(crown_mask==15,"all four large capital crown silhouettes occur in the current galaxy");
 g->system=7;MC_CHECK(mega_city_for(g)->n==0&&station_comms_range(g,0)==8000&&station_comms_range(g,1)==2500,"Rich Lave uses middle-tier comms, auxiliaries retain compact range");
 fprintf(f,"INFO %d capital systems, %d city docking routes\n",systems,routes);free(g);return failures;
#undef MC_CHECK
}
