int pulp_station_run_checks(FILE *f){
 Game *g=calloc(1,sizeof(*g));if(!g)return 1;game_init(g);int failures=0,routes=0;unsigned families=0;
#define PS_CHECK(c,label) do{int ps_pass=(c);fprintf(f,"%s system %d %s\n",ps_pass?"PASS":"FAIL",g->system,label);failures+=!ps_pass;}while(0)
 for(int sys=0;sys<256;sys++){
  g->system=sys;if(station_class(g)==STATION_MEGA)continue;
  StationProfile p=station_profile_for(g,0);families|=1u<<p.family;
  const MegaCity *m=station_architecture_for(g);int solid=1,front=1;
  MegaBlock saved[MEGA_BLOCK_MAX];int count=m->n;memcpy(saved,m->b,sizeof(saved));
  for(int k=0;k<m->n;k++){
   const MegaBlock *b=&m->b[k];
   solid&=mega_box_hit(add(b->c,(Vec3){-b->e.x-300,0,0}),add(b->c,(Vec3){b->e.x+300,0,0}),b,0,0);
   if(k)front&=b->c.z-b->e.z>-p.half;
  }
  g->system=(sys+1)&255;station_architecture_for(g);g->system=sys;m=station_architecture_for(g);
  PS_CHECK(count==m->n&&!memcmp(saved,m->b,count*sizeof(MegaBlock)),"architecture remains stable after revisit");
  PS_CHECK(solid&&front&&count>3&&count<=MEGA_BLOCK_MAX,"visible solids collide and keep forward entrance clear");
  PS_CHECK(!station_architecture_hit(g,(Vec3){0,0,-2500},(Vec3){0,0,-p.half-1},0,0),"launch corridor is empty");
  if(p.station_class==STATION_POOR&&(p.family==0||p.family==5)){float r=p.radius*(p.family==0?2.4f:1.95f)*(1.f+((p.seed>>18)&7)*.025f);
   PS_CHECK(!station_architecture_hit(g,(Vec3){r*.53f,r*.53f,p.half*.8f},(Vec3){r*.53f,r*.53f,p.half*.8f},0,0),"wheel quadrants contain real flyable gaps");}
  /* Two ordinary approaches per system, including a rotating rear detour. */
  for(int side=0;side<2;side++){
   g->time=side?87:0;float angle=station_angle(g);
   float clear=station_architecture_clearance(g);Vec3 local=side?(Vec3){-clear,-800,p.half*7.f+200}:(Vec3){0,0,-p.half-1200};
   g->pos=add(station_arch_rotate(local,angle),(Vec3){0,0,STATION_Z});g->planet=-1;g->docked=g->dead=g->dock_stage=g->legal=g->police_stop=0;g->jump=g->speed=g->boost=0;
   int ok=mega_guidance_start(g),ticks=0;
   while(ok&&g->dock_stage==1&&ticks++<4000){g->time+=.05f;docking_tick(g,.05f);}
   PS_CHECK(ok&&g->dock_stage==2&&!g->dead,"guided approach reaches the slit without teleporting");routes++;
  }
  g->time=40;g->dead=g->dock_stage=0;g->roll=station_angle(g);g->yaw=g->pitch=0;g->speed=100;g->boost=0;
  g->pos=(Vec3){0,0,STATION_Z-p.half+2};
  PS_CHECK(station_collision(g,(Vec3){0,0,STATION_Z-p.half-10})&&g->dock_stage==2&&!g->dead,"manual aligned entry works while rotating");
 }
 PS_CHECK(families==65535&&routes==494,"all sixteen Rich families and 494 ordinary docking routes covered");
 free(g);return failures;
#undef PS_CHECK
}
