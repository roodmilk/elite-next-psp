{
 int identities=1,sites=1;
 game_init(&g);
 for(int sys=0;sys<256;sys++){g.system=sys;system_bodies(&g);
  for(int body=1;body<BODY_COUNT;body++){g.planet=body;FieldProfile p=field_profile(&g,sys,body);identities&=p.seed==g.bodies[body].seed&&field_type(sys,body)==g.bodies[body].type;
   for(int id=1;id<10;id++){if(id==6)continue;Vec3 a=surface_poi(&g,id),b=surface_poi(&g,id);sites&=a.x==b.x&&a.z==b.z&&!terrain_is_water(&g,a.x,a.z);}
  }
 }
 CHECK(identities,"all 1024 planetary identities match orbital seeds/types");
 CHECK(sites,"all 1024 worlds provide deterministic dry activity sites");
 game_init(&g);launch(&g);g.story_flags|=STORY_EV_LANDING_TECH;g.approach=1;enter_planet(&g);Vec3 pad=surface_site(&g,1);
 int exits=1;for(int ship=0;ship<player_ship_count;ship++){g.ship=ship;g.pos=pad;g.pos.y=42;g.surface=1;exits&=eva_toggle(&g)&&eva_position_allowed(&g,g.pos.x,g.pos.z)&&eva_can_board(&g);g.surface=1;}
 CHECK(exits,"all player hulls permit a collision-free exit and immediate boarding");
 const FieldBuilding *terminal=&field_port_buildings[0];
 g.surface=2;g.ship_pos=pad;g.rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,0,-20});g.pos=(Vec3){pad.x+terminal->x,46,pad.z+terminal->z+terminal->d+40};g.yaw=3.14159265f;
 for(int i=0;i<80;i++)game_eva_tick(&g,.1f,0,0,1,0,0);
 CHECK(g.pos.z>=pad.z+terminal->z+terminal->d,"walking cannot penetrate the port terminal");
 game_init(&g);launch(&g);
}
