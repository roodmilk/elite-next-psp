/* Calls the real movement collision test; runs only under an opt-in QA flag. */
int planet_beauty_tests(const char *path){
 FILE *f=fopen(path,"w");if(!f)return 1;
 Game *g=calloc(1,sizeof(Game));if(!g){fclose(f);return 1;}
 int failures=0;
 #define BCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;}while(0)
 game_init(g);g->system=7;system_bodies(g);g->planet=1;g->ship=0;
 Vec3 pad=surface_site(g,1);g->ship_pos=pad;g->rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,0,-10});
 for(int rover=0;rover<2;rover++){
  g->rover_driving=rover;
  for(int id=2;id<=4;id++){FieldBuilding b=field_port_building(g,id);BCHECK(eva_position_allowed(g,pad.x+b.x,pad.z+b.z),"open landing terrace is traversable");}
  FieldBuilding b=field_port_building(g,0);BCHECK(!eva_position_allowed(g,pad.x+b.x,pad.z+b.z),"visible terminal remains solid");
  g->world_clock=120-(g->bodies[1].seed%40);
  BCHECK(field_traffic_lift(g,0)>100&&eva_position_allowed(g,pad.x-FIELD_TRAFFIC_X,pad.z+FIELD_TRAFFIC_Z),"vacant traffic berth is traversable");
  g->world_clock=50-(g->bodies[1].seed%40);
  BCHECK(!eva_position_allowed(g,pad.x-FIELD_TRAFFIC_X,pad.z+FIELD_TRAFFIC_Z),"parked traffic hull remains protected");
 }
 FieldBuilding cave={0,0,74,74,158};
 BCHECK(!field_cave_wall(0,-60,cave,18)&&!field_cave_wall(0,0,cave,18),"cave entrance and centre aisle are open");
 BCHECK(field_cave_wall(65,0,cave,18)&&field_cave_wall(0,65,cave,18),"visible cave side and rear walls are solid");
 int variety=1;float lo=1000,hi=0;
 for(unsigned h=0;h<256;h++){unsigned seed=field_hash(h);float height=field_lave_tree_height(seed);lo=fminf(lo,height);hi=fmaxf(hi,height);variety&=field_tree_trunk_radius(seed,1)<=22;}
 BCHECK(variety&&hi-lo>70,"varied tree heights and bounded rover trunk clearance");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);free(g);
 #undef BCHECK
 return failures;
}
