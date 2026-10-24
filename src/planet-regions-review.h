/* Opt-in native renderer and bounded galaxy sweep. No release test content. */
static int planet_regions_review(void){
 FILE *flag=fopen("planet-regions-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("planet-regions-review.txt","w");unsigned *old=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!out||!pixels)return 1;fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0x35a87cb1u;
 int fail=0,checks=0;struct mallinfo memory=mallinfo();fprintf(out,"MEMORY initial heap used %d free %d uncommitted %d\n",memory.uordblks,memory.fordblks,7168*1024-memory.arena);fflush(out);
#define RC(c,s) do{int region_pass=(c);fprintf(out,"%s %s\n",region_pass?"PASS":"FAIL",s);checks++;fail+=!region_pass;fflush(out);}while(0)
 int cells_ok=1,cells=0;
 for(int location=0;location<3;location++)for(int z=-24;z<=24;z++)for(int x=-24;x<=24;x++){
  int sys=location==2?0:7,body=location==1?4:2,tile=location==2?90:70;float px,pz;unsigned h;
  field_prop_cell(sys,body,field_seed(sys,body),x,z,&px,&pz,&h);
  cells_ok&=isfinite(px)&&isfinite(pz)&&px>=x*(float)tile&&px<(x+1)*(float)tile&&pz>=z*(float)tile&&pz<(z+1)*(float)tile;cells++;
 }
 fprintf(out,"CELLS %d signed-coordinate prop cells checked\n",cells);
 RC(cells_ok,"vegetation remains in its own lattice cell in all four coordinate quadrants");
 /* Visible progress first: exact production world renderer, not illustrations. */
 for(int shot=0;shot<10;shot++){
  int sys=7,body=shot<4?1:shot==4||shot==9?2:shot==5?4:shot==8?3:1;
  if(shot==6||shot==7){int wanted=shot==6?WORLD_DESERT:WORLD_TOXIC,found=0;for(int s=0;s<256&&!found;s++)for(int b=1;b<system_body_slots(s)&&!found;b++)if(field_type(s,b)!=SUN&&world_type_seed(field_seed(s,b),field_type(s,b))==wanted){sys=s;body=b;found=1;}}
  field_review_setup(sys,body);const WorldProfile *p=world_profile(game.bodies[body].seed,game.bodies[body].type);Vec3 pad=surface_site(&game,1);
  int region=shot<4?shot:shot==4?1:shot==5?2:0;
  if(region>=p->count)region=0;
  game.pos=(Vec3){pad.x+p->x[region]-120,0,pad.z+p->z[region]-200};
  if(shot==0)game.pos=add(pad,(Vec3){-160,0,1100});
  if(shot==8)game.pos=add(pad,(Vec3){-100,0,220});
  if(shot==6||shot==7)game.pos=add(pad,(Vec3){1100,0,1100});
  Vec3 capture_origin=game.pos;
  for(int attempt=0;attempt<48;attempt++){float a=(attempt%16)*.392699f,r=attempt<16?80:attempt<32?160:240;float x=capture_origin.x+cosf(a)*r,z=capture_origin.z+sinf(a)*r;if(eva_position_allowed(&game,x,z)){game.pos.x=x;game.pos.z=z;break;}}
  game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.pitch=.04f;float best=1e30f;
  for(int direction=0;direction<16;direction++){float a=direction*.392699f,score=terrain_mesh_height(&game,game.pos.x+sinf(a)*140,game.pos.z+cosf(a)*140)+terrain_mesh_height(&game,game.pos.x+sinf(a)*360,game.pos.z+cosf(a)*360);if(score<best){best=score;game.yaw=a;}}
  if(shot==3)game.yaw=atan2f(game.pos.x-pad.x,game.pos.z-pad.z);
  if(shot==8)game.yaw=.25f;
  float hour=field_local_hour(&game,sys,body);static const int days[]={600,900,1200,1800};game.world_clock+=(11-hour+24)*days[(field_seed(sys,body)>>24)&3]/24.f;
  WorldRegion r=world_sample(p,game.pos.x-pad.x,game.pos.z-pad.z);char file[64];snprintf(file,sizeof(file),"region-%02d-%s.bmp",shot,world_types[p->type].name);
  field_review_frame(file);fprintf(out,"PROPS shot %d visible %d\n",shot,planet_visible_props);fprintf(out,"CAPTURE %s %s / %s at %.0f,%.0f\n",file,game.bodies[body].name,world_biomes[r.a],game.pos.x-pad.x,game.pos.z-pad.z);fflush(out);
  if(shot==9)RC(planet_visible_props>10,"Lave II giant forest contains visible production props");
  if(shot==4)RC(planet_visible_props>10,"negative-coordinate fern basin contains visible production props");
  if(shot==5)RC(planet_visible_props>3,"negative-coordinate ice canyon contains visible production rocks");
 }
 int count[WORLD_TYPE_COUNT]={0},ground=0,gas=0,valid=1,stable=1,finite=1,protected=1,regions=0;
 Game g;game_init(&g);story_complete(&g);
 for(int sys=0;sys<256;sys++){
  g.system=sys;system_bodies(&g);
  for(int body=1;body<system_body_slots(sys);body++){
   if(g.bodies[body].type==SUN)continue;g.planet=body;unsigned seed=g.bodies[body].seed;int broad=g.bodies[body].type;
   WorldProfile p=*world_profile(seed,broad);count[p.type]++;ground+=broad!=GAS;gas+=broad==GAS;regions+=p.count;
   valid&=p.count>=1&&p.count<=(broad==GAS?4:6)&&p.type>=0&&p.type<WORLD_TYPE_COUNT;
   int seen=0;for(int i=0;i<p.count;i++){int local=p.type<18?p.biome[i]-p.type*6:p.biome[i]-108-(p.type-18)*4;valid&=local>=0&&local<(broad==GAS?4:6)&&!(seen&(1<<local));seen|=1<<local;}
   world_profile(seed^0x10000000u,broad);WorldProfile again=*world_profile(seed,broad);stable&=!memcmp(&p,&again,sizeof(p));
   Vec3 pad=surface_site(&g,1);protected&=terrain_height(&g,pad.x,pad.z)==24&&!terrain_is_water(&g,pad.x,pad.z);
   for(int i=0;i<9;i++){float x=(i%3-1)*1900.f,z=(i/3-1)*1700.f;WorldRegion a=world_sample(&p,x,z),b=world_sample(&again,x,z);stable&=a.a==b.a&&a.b==b.b&&a.weight==b.weight&&a.ground==b.ground;float h=terrain_height(&g,pad.x+x,pad.z+z);finite&=isfinite(h)&&isfinite(a.relief)&&a.weight>=.5f&&a.weight<=1;}
  }
 }
 int cached_exact=1;
 for(int system=0;system<256;system++)for(int body=1;body<system_body_slots(system);body++)if(field_type(system,body)!=SUN){
  unsigned seed=field_seed(system,body),h=field_hash(seed+2u*9973u);float a=(h%6283)*.001f,d=480.f+(h%900),x,z;
  world_pad(seed,&x,&z);cached_exact&=x==cosf(a)*d&&z==sinf(a)*d;world_pad(seed^16u,&x,&z);world_pad(seed,&x,&z);cached_exact&=x==cosf(a)*d&&z==sinf(a)*d;
  const WorldProfile *p=world_profile(seed,field_type(system,body));for(int i=0;i<p->count;i++)cached_exact&=p->ground[i]==world_biome_ground(p,p->biome[i]);
  for(int i=0;i<9;i++){WorldRegion r=world_sample(p,(i%3-1)*1750.f,(i/3-1)*2250.f);cached_exact&=r.ground==world_mix(world_biome_ground(p,r.b),world_biome_ground(p,r.a),r.weight);}
 }
 RC(cached_exact,"cached origins and biome colours exactly match prior arithmetic across every world and cache alias");
 RC(valid,"every accessible destination has valid compatible nonempty distinct biome IDs");
 RC(stable,"types, biome pools and region samples survive cache eviction and revisit exactly");
 RC(finite,"galaxy terrain samples and blended weights remain finite");
 RC(protected,"all existing landing pads remain dry and level");
 RC(ground==1038&&gas==342,"existing 1038 ground worlds and 342 gas destinations preserved");
 int all=1;for(int i=0;i<WORLD_TYPE_COUNT;i++){fprintf(out,"TYPE %s %d\n",world_types[i].name,count[i]);all&=count[i]>0;}RC(all,"all eighteen ground types and four gas families occur");
 RC(world_type_seed(field_seed(7,1),OCEAN)==WORLD_TEMPERATE&&world_type_seed(field_seed(7,2),ROCKY)==WORLD_OVERGROWN&&world_type_seed(field_seed(7,3),GAS)==WORLD_BANDED&&world_type_seed(field_seed(7,4),ROCKY)==WORLD_FROZEN,"Lave authored type overrides retained");
 g.system=7;system_bodies(&g);g.planet=1;Vec3 pad=surface_site(&g,1);WorldProfile p=*field_world(&g,7,1);int visited=0,transitions=0;
 for(int z=-4500;z<=9500;z+=160)for(int x=-4500;x<=4500;x+=160){WorldRegion a=world_sample(&p,x,z),b=world_sample(&p,x+1,z);visited|=1<<(a.a%6);transitions+=a.a!=b.a;float h=terrain_height(&g,pad.x+x,pad.z+z),next=terrain_height(&g,pad.x+x+1,pad.z+z);finite&=isfinite(next)&&fabsf(next-h)<8;}
 RC((visited&0x33)==0x33,"Lave I contains real meadow, forest, river-valley and coastal regions");
 RC(finite,"Lave region boundaries do not tear the collision mesh");
 int continuity=1;for(int z=-2500;z<=2500;z+=41)for(int x=-2500;x<=2500;x+=97){WorldRegion a=world_sample(&p,x,z),b=world_sample(&p,x+.1f,z);continuity&=fabsf(a.relief-b.relief)<1.f;}RC(continuity,"region relief blends continuously over neighbouring coordinates");
 /* Shared collision must leave the authored observatory trail and site perimeters dry. */
 int sites_ok=1,trail_ok=1;
 for(int body=1;body<=4;body++){
  field_review_setup(7,body);Vec3 base=surface_site(&game,1);
  for(int id=1;id<10;id++)if(id!=6){Vec3 site=surface_poi(&game,id);FieldBuilding shape=field_site_building(&game,id);int exit=0;
   for(int side=0;side<16;side++){float a=side*.392699f,x=site.x+cosf(a)*(shape.w+38),z=site.z+sinf(a)*(shape.d+38);exit|=eva_position_allowed(&game,x,z);}sites_ok&=exit;
  }
  if(body==1)for(int step=0;step<=210;step++){float t=step/210.f,x=base.x-(game.bodies[1].seed&1?1.f:-1.f)*420*t*(1-t),z=base.z+FIELD_OBSERVATORY_DISTANCE*t;trail_ok&=!terrain_is_water(&game,x,z);}
 }
 RC(sites_ok,"Lave authored sites retain an accessible dry perimeter");RC(trail_ok,"Lave observatory trail remains dry");
 field_review_setup(7,1);game.surface=2;pad=surface_landing_pad(&game);game.ship_pos=pad;game.pos=add(pad,(Vec3){220,22,0});Vec3 saved=game.pos;int was_safe=eva_position_allowed(&game,saved.x,saved.z);unsigned progress=game.surface_progress[7][1];surface_region_resume(&game);
 RC(was_safe&&game.pos.x==saved.x&&game.pos.z==saved.z,"safe saved surface coordinates remain unchanged");
 game.pos=add(pad,(Vec3){12000,0,12000});game.ship_pos=game.pos;surface_region_resume(&game);
 RC(!terrain_is_water(&game,game.pos.x,game.pos.z)&&!terrain_is_water(&game,game.ship_pos.x,game.ship_pos.z)&&length(sub(game.pos,pad))<500&&game.surface_progress[7][1]==progress,"unsafe old surface coordinates return to same-world pad without losing progress");
 for(int body=1;body<=4;body++){field_review_setup(7,body);game.pos=surface_landing_pad(&game);game.pos.y=42;game.surface=0;game.speed=0;game.story_flags|=STORY_EV_LANDING_TECH;RC(land_planet(&game)&&disembark_planet(&game),"Lave landing and safe disembark remain playable");RC(eva_can_board(&game)&&board_planet(&game)&&takeoff_planet(&game),"Lave boarding and takeoff remain playable");}
 game_init(&g);story_complete(&g);g.surface_progress[7][1]=0x172u;g.landed_planets[7]=15;g.credits=123450;g.system=7;system_bodies(&g);
 WorldProfile before=*field_world(&g,7,1);RC(save_game(&g,"regions-v32.sav"),"V32 save writes without a generation-format change");Game loaded;int ok=load_game(&loaded,"regions-v32.sav");WorldProfile after=*field_world(&loaded,7,1);
 fprintf(out,"SAVE loaded %d credits %d progress %x landed %x identity %d type %d/%d seed %u/%u\n",ok,loaded.credits,loaded.surface_progress[7][1],loaded.landed_planets[7],memcmp(&before,&after,sizeof(before)),before.type,after.type,before.seed,after.seed);
 RC(ok&&loaded.credits==123450&&loaded.surface_progress[7][1]==0x172u&&loaded.landed_planets[7]==15&&!memcmp(&before,&after,sizeof(before)),"save/load preserves discoveries, funds and generated identity");
 /* Same camera positions and frame count as the baseline planet-speed review. */
 for(int body=1;body<=4;body++)for(int mode=0;mode<3;mode++){
  field_review_setup(7,body);planet_seat.active=planet_entry_body=0;planet_orbit_veil=0;surface_target_reset();planet_controls_ready=1;Vec3 pad=surface_site(&game,1);game.surface=mode==2?2:0;game.pos=add(pad,(Vec3){0,22,mode==2?3600:400});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=0;
  if(mode<2){planet_sequence_begin(mode==0?1:2);game.planet_sequence_time=mode==0?3.5f:1.2f;}double total=0,worst=0;
  for(int frame=0;frame<10;frame++){uint64_t at=sceKernelGetSystemTimeWide();field_review_frame(0);double ms=(sceKernelGetSystemTimeWide()-at)*.001;total+=ms;worst=fmaxf(worst,ms);}fprintf(out,"PROFILE body %d mode %d avg %.3f worst %.3f\n",body,mode,total/10,worst);
 }
 memory=mallinfo();fprintf(out,"MEMORY final heap used %d free %d uncommitted %d\n",memory.uordblks,memory.fordblks,7168*1024-memory.arena);RC(memory.fordblks+7168*1024-memory.arena>2*1024*1024,"region renderer retains at least 2 MiB of heap headroom on PSP-1000 configuration");
 int safe=1;for(int i=0;i<16;i++)safe&=pixels[i]==0x35a87cb1u&&pixels[STRIDE*H+16+i]==0x35a87cb1u;RC(safe,"native capture framebuffer guards intact");
 fprintf(out,"INFO generation %d; %d selected regions over %d existing destinations; %d checks. Specialist caves, roots, hazards and bespoke fungal/crystal art deferred.\nRESULT %d failures\n",WORLD_GENERATION_VERSION,regions,ground+gas,checks,fail);fclose(out);fb=old;free(pixels);
#undef RC
 return 1;
}
