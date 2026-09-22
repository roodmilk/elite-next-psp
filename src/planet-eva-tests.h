/* Inside game_tests. Fixed geometry/transition checks, independent of rendering. */
{
 int terrain_ok=1,roundtrip_ok=1;
 for(int system=0;system<256;system+=17){
  game_init(&g);g.system=system;game_spawn(&g);launch(&g);
  for(int body=1;body<BODY_COUNT;body++)if(g.bodies[body].type!=GAS){
   g.approach=body;g.pos=add(g.bodies[body].pos,(Vec3){0,0,-g.bodies[body].radius-900});
   roundtrip_ok &= enter_planet(&g);
   Vec3 pad=surface_site(&g,1);
   for(int k=0;k<12;k++){
    float x=floorf((pad.x+210+k*43)/SURFACE_CELL)*SURFACE_CELL,z=floorf((pad.z-350+k*71)/SURFACE_CELL)*SURFACE_CELL;
    float a=terrain_height(&g,x,z),b=terrain_height(&g,x+40,z),c=terrain_height(&g,x+40,z+40),d=terrain_height(&g,x,z+40);
    float h1=terrain_height(&g,x+30,z+10),h2=terrain_height(&g,x+10,z+30);
    terrain_ok &= fabsf(h1-(a*.25f+b*.5f+c*.25f))<.01f&&fabsf(h2-(a*.25f+c*.25f+d*.5f))<.01f;
    terrain_ok &= fabsf(terrain_height(&g,x-.01f,z+20)-terrain_height(&g,x+.01f,z+20))<.1f;
   }
   g.pos=pad;g.speed=8;roundtrip_ok &= land_planet(&g)&&eva_toggle(&g)&&eva_can_board(&g)&&eva_toggle(&g)&&takeoff_planet(&g);
   leave_planet(&g);roundtrip_ok &= g.planet<0;
  }
 }
 CHECK(terrain_ok,"EVA terrain matches both rendered triangles and is continuous across cell edges");
 CHECK(roundtrip_ok,"seeded landable worlds permit land, disembark, immediate board, takeoff and orbit return");
 game_init(&g);launch(&g);g.approach=2;enter_planet(&g);g.bodies[2].type=ROCKY;
 Vec3 pad=surface_site(&g,1);g.pos=pad;g.speed=8;land_planet(&g);eva_toggle(&g);
 Vec3 before=g.pos;game_eva_tick(&g,.1f,1,1,0,0,0);
 CHECK(g.pos.x==before.x&&g.pos.z==before.z&&g.pitch>0&&g.roll==0,"EVA look changes camera without translating or rolling");
 g.yaw=0;g.pitch=.7f;before=g.pos;game_eva_tick(&g,.1f,0,0,1,1,0);
 CHECK(length((Vec3){g.pos.x-before.x,0,g.pos.z-before.z})<=6.201f&&g.pos.x>before.x&&g.pos.z>before.z,"EVA diagonal movement is level and speed capped while looking up");
 g.pos=(Vec3){pad.x+EVA_FIELD_RADIUS-.1f,0,pad.z};g.pos.y=terrain_height(&g,g.pos.x,g.pos.z)+22;g.yaw=1.5707963f;g.message_time=0;
 before=g.pos;game_eva_tick(&g,.1f,0,0,1,0,0);
 CHECK(length(sub(g.pos,before))<1&&strstr(g.message,"Field edge"),"EVA field edge blocks outward movement without teleporting");
 game_eva_tick(&g,.1f,0,0,-1,0,0);
 CHECK(g.pos.x<before.x-2,"EVA can retreat from the local field edge");
 /* Find the actual drawn shore tile boundary, then approach from dry land. */
 g.bodies[2].type=OCEAN;float shore=pad.x;while(!terrain_is_water(&g,shore,pad.z)&&shore<pad.x+600)shore+=1;
 g.pos=(Vec3){shore-2,46,pad.z};g.yaw=1.5707963f;g.message_time=0;before=g.pos;
 game_eva_tick(&g,.1f,0,0,1,0,1);
 CHECK(!terrain_is_water(&g,g.pos.x,g.pos.z)&&g.pos.x<shore&&strstr(g.message,"Shoreline"),"EVA shoreline blocks walking and jet translation into visible water");
 game_eva_tick(&g,.1f,0,0,-1,0,0);CHECK(g.pos.x<before.x-2,"EVA shore collision allows a safe retreat");
 g.pos=g.ship_pos;g.pos.y+=4;g.jetpack=0;float fuel=g.fuel;
 for(int i=0;i<120;i++)game_eva_tick(&g,.05f,0,0,0,0,1);
 CHECK(g.pos.y<=terrain_height(&g,g.pos.x,g.pos.z)+142.01f&&g.fuel==fuel,"EVA jet has bounded height and does not spend ship fuel");
 CHECK(!eva_toggle(&g)&&g.surface==2,"EVA cannot board a ship from high above it");
 for(int i=0;i<90;i++)game_eva_tick(&g,.05f,0,0,0,0,0);
 CHECK(fabsf(g.pos.y-(terrain_height(&g,g.pos.x,g.pos.z)+22))<.01f&&eva_can_board(&g),"EVA jet release lands at eye height and restores boarding");
 g.pos.y+=2.1f;CHECK(!eva_can_board(&g),"EVA boarding rejects even a low airborne hover");g.pos.y-=2.1f;
 g.dead=1;CHECK(!eva_toggle(&g),"dead commander cannot board");g.dead=0;
 CHECK(eva_toggle(&g)&&g.jetpack==0&&!g.boost&&g.pitch==0,"boarding clears jet velocity and restores level ship controls");
 /* Existing resource arithmetic, not a new reward or persistence rule. */
 int resource_ok=1;
 for(int refinery=0;refinery<2;refinery++)for(int full=0;full<2;full++){
  game_init(&g);launch(&g);g.approach=1;enter_planet(&g);g.pos=surface_site(&g,1);g.speed=8;land_planet(&g);eva_toggle(&g);
  if(refinery){g.fit[FIT_UTIL]=21;fit_rebuild(&g);}
  memset(g.cargo,0,sizeof(g.cargo));g.cargo[0]=cargo_capacity(&g)-(full?0:1);
  for(int i=1;i<LIFE_COUNT;i++)g.life[i].alive=0;
  g.life[0].pos=g.pos;int cash=g.credits,disc=g.discoveries;
  resource_ok &= survey_scan(&g)&&g.credits==cash+(full?160:120)&&g.discoveries==disc+1&&cargo_used(&g)==cargo_capacity(&g);
  resource_ok &= g.cargo[refinery?9:12]==(full?0:1)&&g.cargo[refinery?12:9]==0;
  cash=g.credits;resource_ok &= !survey_scan(&g)&&g.credits==cash;
 }
 CHECK(resource_ok,"EVA existing mineral/refinery and full-hold conversion conserve cargo and pay once per object");
 game_init(&g);launch(&g);
}
