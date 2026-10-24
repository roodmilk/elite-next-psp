static int planet_speed_review(void){
 FILE *flag=fopen("planet-speed-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("planet-speed-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!f||!pixels)return 1;fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;
 for(int body=1;body<=4;body++)for(int mode=0;mode<3;mode++){
  field_review_setup(7,body);planet_seat.active=planet_entry_body=0;planet_orbit_veil=0;surface_target_reset();planet_controls_ready=1;
  Vec3 pad=surface_site(&game,1);game.surface=mode==2?2:0;game.pos=add(pad,(Vec3){0,22,mode==2?3600:400});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=0;
  if(mode<2){planet_sequence_begin(mode==0?1:2);game.planet_sequence_time=mode==0?3.5f:1.2f;}
  double total=0,stages[5]={0},worst=0;
  for(int frame=0;frame<10;frame++){uint64_t a=sceKernelGetSystemTimeWide();field_review_frame(0);double ms=(sceKernelGetSystemTimeWide()-a)*.001;total+=ms;worst=fmaxf(worst,ms);for(int i=0;i<5;i++)stages[i]+=(lave_render_marks[i+1]-lave_render_marks[i])*1000.0/sceRtcGetTickResolution();}
  fprintf(f,"PROFILE body %d mode %d avg %.3f worst %.3f sky %.3f ground %.3f buildings %.3f vegetation %.3f traffic/life %.3f\n",body,mode,total/10,worst,stages[0]/10,stages[1]/10,stages[2]/10,stages[3]/10,stages[4]/10);
  char file[64];snprintf(file,sizeof(file),"speed-%d-%d.bmp",body,mode);dump_native_bmp(file);
 }
 int cache_match=1,checked=0;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  if(sys==7&&(body==1||body==3))continue;field_review_setup(sys,body);Vec3 pad=surface_site(&game,1),sites[10];FieldBuilding shapes[10];for(int id=1;id<10;id++)if(id!=6){sites[id]=surface_poi(&game,id);shapes[id]=field_site_building(&game,id);}
  int biome=planet_biome(&game.bodies[body]);unsigned seed=game.bodies[body].seed;
  for(int cell=0;cell<16;cell++){int cx=(int)(pad.x/90)+(cell%4-2)*13,cz=(int)(pad.z/90)+(cell/4-2)*17;float x,z;unsigned h;field_prop_cell(sys,body,seed,cx,cz,&x,&z,&h);
   int allowed=!(sys!=7&&biome!=BIOME_FOREST&&(h&3)==0);if(allowed&&(terrain_water_tile(&game,x,z)||(fabsf(x-pad.x)<field_port_clear(&game)&&fabsf(z-pad.z)<field_port_clear(&game))))allowed=0;
   if(allowed)for(int id=1;id<10;id++)if(id!=6&&fabsf(x-sites[id].x)<shapes[id].w+30&&fabsf(z-sites[id].z)<shapes[id].d+30){allowed=0;break;}
   int kind=(h>>16)%4;if(sys==7&&body==4&&(kind==1||kind==3)&&(h&1))allowed=0;if(biome==BIOME_DESERT||biome==BIOME_VOLCANIC)kind=kind==0?2:kind;
   for(int hit=0;hit<2;hit++){PlanetProp p;int exists=planet_other_prop(cx,cz,pad,sites,shapes,seed,biome,&p);cache_match&=exists==allowed;if(exists)cache_match&=p.x==x&&p.w==z&&p.h==h&&p.kind==kind&&p.z==terrain_height(&game,x,z);checked++;}
  }
 }
 fprintf(f,"%s %d cached scenery miss/hit identities match uncached generation across the galaxy\n",cache_match?"PASS":"FAIL",checked);failures+=!cache_match;
 field_review_setup(7,1);Vec3 bridge,axis;int have=terrain_bridge_position(&game,3,&bridge,&axis);surface_bridge_only=3;
 for(int view=0;view<12&&have;view++){
  float angle=(view%6)*1.04719755f;Vec3 along={-axis.z,0,axis.x};game.pos=add(bridge,add(mul(axis,cosf(angle)*200),mul(along,sinf(angle)*90)));game.pos.y=bridge.y+(view<6?70:-12);
  Vec3 aim=bridge;if(view==10){game.pos=add(bridge,mul(axis,220));game.pos.y=bridge.y-10;aim=add(bridge,mul(axis,270));}
  aim.y=bridge.y-2;Vec3 d=sub(aim,game.pos);game.yaw=atan2f(d.x,d.z);game.pitch=atan2f(d.y,sqrtf(d.x*d.x+d.z*d.z));game.roll=0;
  rect(0,0,W,H,BG);memset(surface_depth,255,sizeof(surface_depth));surface_depth_on=1;surface_material=0;surface_light=1;surface_y_min=0;surface_y_max=H-1;preview_clip(240,110,0,0,W,H);drawcount=0;surface_bridges();int visible=0;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++)visible+=fb[y*STRIDE+x]!=BG;
  fprintf(f,"%s bridge angle %d: %d visible pixels\n",visible>32?"PASS":"FAIL",view,visible);failures+=visible<=32;
  char file[64];snprintf(file,sizeof(file),"bridge-angle-%02d.bmp",view);dump_native_bmp(file);
 }
 failures+=!have;surface_bridge_only=-1;surface_depth_on=0;preview_reset();
 for(int i=0;i<16;i++)failures+=pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu;
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
