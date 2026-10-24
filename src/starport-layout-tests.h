int starport_layout_tests(const char *path){
 FILE *f=fopen(path,"w");if(!f)return 1;Game *g=calloc(1,sizeof(Game));if(!g){fclose(f);return 1;}
 int failures=0;
 #define PORT_CHECK(ok,name) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",name);failures+=!pass;fflush(f);}while(0)
 game_init(g);story_complete(g);int bounds=1,separation=1;
 for(int i=0;i<FIELD_PORT_BUILDINGS;i++){
  FieldBuilding actual=field_port_building(g,i);const FieldBuilding *a=&actual;
  bounds&=fabsf(a->x)+a->w<=FIELD_PORT_HALF&&fabsf(a->z)+a->d<=FIELD_PORT_HALF;
  separation&=fabsf(a->x)>a->w+FIELD_PLAYER_PAD+8||fabsf(a->z)>a->d+FIELD_PLAYER_PAD_DEPTH+8;
  for(int lane=0;lane<2;lane++)separation&=fabsf(a->x-(lane?FIELD_TRAFFIC_X:-FIELD_TRAFFIC_X))>a->w+FIELD_TRAFFIC_PAD+8||fabsf(a->z-FIELD_TRAFFIC_Z)>a->d+FIELD_TRAFFIC_PAD+8;
 }
 PORT_CHECK(bounds&&separation,"all airport buildings fit apron and clear player/heavy berths");
 int catalogue=1,flat=1,sites=1,worlds=0;
 for(int sys=0;sys<256;sys++){
  g->system=sys;system_bodies(g);
  for(int body=1;body<BODY_COUNT;body++){
   if(!body_active(&g->bodies[body])||g->bodies[body].type==SUN)continue;worlds++;
   g->planet=body;Vec3 pad=surface_site(g,1);
   for(int i=0;i<FIELD_PORT_BUILDINGS;i++){
    FieldBuilding a=field_port_building(g,i);
    catalogue&=fabsf(a.x)+a.w<=FIELD_PORT_HALF&&fabsf(a.z)+a.d<=FIELD_PORT_HALF;
    catalogue&=fabsf(a.x)>a.w+FIELD_PLAYER_PAD+8||fabsf(a.z)>a.d+FIELD_PLAYER_PAD_DEPTH+8;
    for(int lane=0;lane<2;lane++)catalogue&=fabsf(a.x-(lane?FIELD_TRAFFIC_X:-FIELD_TRAFFIC_X))>a.w+FIELD_TRAFFIC_PAD+8||fabsf(a.z-FIELD_TRAFFIC_Z)>a.d+FIELD_TRAFFIC_PAD+8;
   }
   for(int c=0;c<4;c++){float x=pad.x+(c&1?FIELD_PORT_HALF:-FIELD_PORT_HALF),z=pad.z+(c&2?FIELD_PORT_HALF:-FIELD_PORT_HALF);
    if(fabsf(terrain_height(g,x,z)-24)>.01f||terrain_is_water(g,x,z)){flat=0;fprintf(f,"Foundation mismatch %d/%d\n",sys,body);}
   }
   for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(g,id);FieldBuilding b=field_site_building(g,id);
    if(fabsf(p.x-pad.x)<FIELD_PORT_CLEAR+b.w+40&&fabsf(p.z-pad.z)<FIELD_PORT_CLEAR+b.d+40){sites=0;fprintf(f,"Site overlap %d/%d/%d\n",sys,body,id);}
   }
   if(!isfinite(pad.x)||!isfinite(pad.z))catalogue=0;
  }
 }
 fprintf(f,"INFO %d active landable worlds checked\n",worlds);
 PORT_CHECK(catalogue&&flat,"all active planetary ports are level, supported and clear of landing berths");
 PORT_CHECK(sites,"seeded POIs stay outside compact airport reservation");
 g->system=7;system_bodies(g);g->planet=1;Vec3 pad=surface_site(g,1);
 g->rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,0,-10});g->rover_pos.y=terrain_height(g,g->rover_pos.x,g->rover_pos.z)+22;
 for(int hull=0;hull<player_ship_count;hull++){
  g->ship=hull;float hw,hd;surface_ship_bounds(g,&hw,&hd);
  if(hw+18>=FIELD_PLAYER_PAD||hd+18>=FIELD_PLAYER_PAD_DEPTH)fprintf(f,"INFO hull %s requires %.2f x %.2f half-clearance\n",player_ships[hull].name,hw+18,hd+18);
  PORT_CHECK(hw+18<FIELD_PLAYER_PAD&&hd+18<FIELD_PLAYER_PAD_DEPTH,"player hull and airlock clearance fit the compact pad");
  g->surface=0;g->dead=0;g->pos=pad;g->pos.y=42;g->speed=0;g->story_flags|=STORY_EV_LANDING_TECH;
  PORT_CHECK(land_planet(g)&&disembark_planet(g)&&eva_can_board(g),"every player ship lands, disembarks and can reboard");
 }
 for(int body=1;body<=4;body++){
  g->planet=body;g->ship=0;g->surface=2;g->rover_driving=1;pad=surface_site(g,1);g->ship_pos=pad;
  int clear=1;for(int z=-10;z<=290;z+=10)clear&=eva_position_allowed(g,pad.x+FIELD_GARAGE_X,pad.z+z);
  PORT_CHECK(clear,"rover garage has an unobstructed forward exit lane");
  FieldBuilding terminal=field_port_building(g,0);g->rover_driving=0;g->pos=(Vec3){pad.x+terminal.x,46,pad.z+terminal.z+terminal.d+40};g->yaw=3.14159265f;
  for(int i=0;i<80;i++)game_eva_tick(g,.1f,0,0,1,0,0);
  PORT_CHECK(g->pos.z>=pad.z+terminal.z+terminal.d,"walking stops outside the actual compact terminal");
 }
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);free(g);return failures;
 #undef PORT_CHECK
}
