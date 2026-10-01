/* Disposable production-render and gameplay audit for the middle class. */
static int rich_station_review(void){
 FILE *flag=fopen("rich-station.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("rich-station.txt","w");if(!f)return 1;
 int failures=0,systems=0,routes=0,representative[16];unsigned seen=0;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xfeed2481;fb=pixels+16;
#define RCHECK(c,label) do{int pass248=!!(c);fprintf(f,"%s system %d %s\n",pass248?"PASS":"FAIL",game.system,label);failures+=!pass248;}while(0)
 game_init(&game);story_complete(&game);preview_reset();high_contrast=0;hud_hidden=0;hud_mode=0;
 for(int sys=0;sys<256;sys++){
  game.system=sys;if(station_class(&game)!=STATION_RICH)continue;systems++;
  StationProfile p=station_profile_for(&game,0);if(!(seen&(1u<<p.family)))representative[p.family]=sys;seen|=1u<<p.family;
  const MegaCity *m=station_architecture_for(&game);MegaBlock copy[MEGA_BLOCK_MAX];int count=m->n;memcpy(copy,m->b,sizeof(copy));
  float lo[3]={1e9f,1e9f,1e9f},hi[3]={-1e9f,-1e9f,-1e9f};int front=1,solid=1,signs=0,arrays=0;
  for(int k=0;k<count;k++){
   const MegaBlock *b=&copy[k];const MegaGeometry *shape=mega_geometry_for(b->shape);
   for(int axis=0;axis<3;axis++){float c=axis==0?b->c.x:axis==1?b->c.y:b->c.z,e=axis==0?b->e.x:axis==1?b->e.y:b->e.z;
    if(!b->shape){lo[axis]=fminf(lo[axis],c-e);hi[axis]=fmaxf(hi[axis],c+e);}
    else for(int v=0;v<shape->vertices;v++){float q=c+e*(axis==0?shape->v[v].x:axis==1?shape->v[v].y:shape->v[v].z);lo[axis]=fminf(lo[axis],q);hi[axis]=fmaxf(hi[axis],q);}
   }
   if(k)front&=b->c.z-b->e.z>-p.half;
   solid&=mega_box_hit(add(b->c,(Vec3){-b->e.x-100,0,0}),add(b->c,(Vec3){b->e.x+100,0,0}),b,0,0);
   signs+=b->kind==1&&!b->shape;arrays+=b->kind==7;
  }
  float span=fmaxf(hi[0]-lo[0],fmaxf(hi[1]-lo[1],hi[2]-lo[2]));
  RCHECK(span>2200&&span<10000&&count>=17&&count<=60,"middle tier is kilometre-scale and bounded below capital scale");
  RCHECK(front&&solid&&signs==2&&arrays==2,"all structural pods, signs and solar plates collide and stay behind the entrance");
  game.system=(sys+1)&255;station_architecture_for(&game);game.system=sys;m=station_architecture_for(&game);
  RCHECK(m->n==count&&!memcmp(copy,m->b,count*sizeof(MegaBlock)),"seeded districts remain identical after revisiting");
  RCHECK(station_comms_range(&game,0)==8000&&station_comms_range(&game,1)==2500&&station_port_count_for(&game,0)==1,"middle-tier traffic control reaches 8 km without altering relay range or common slit");
  RCHECK(!station_architecture_hit(&game,(Vec3){0,0,-5000},(Vec3){0,0,-p.half-1},0,0),"first-person departure corridor stays empty");
  for(int v=0;v<4;v++){
   float clear=station_architecture_clearance(&game);
   Vec3 local=v==0?(Vec3){0,0,-p.half-1500}:v==1?(Vec3){-clear,0,hi[2]+400}:v==2?(Vec3){clear,0,hi[2]+400}:(Vec3){0,hi[1]+500,hi[2]+400};
   game.time=v*43;game.pos=add(station_arch_rotate(local,station_angle(&game)),(Vec3){0,0,STATION_Z});
   game.dead=game.docked=game.dock_stage=game.legal=game.police_stop=0;game.planet=-1;game.jump=game.speed=game.boost=0;
   int ok=dock_hub(&game,0),ticks=0;while(ok&&game.dock_stage==1&&ticks++<6000)game_tick(&game,.05f,0,0,0,0);
   RCHECK(ok&&game.dock_stage==2&&!game.dead,"rotating front/left/right/upper approach reaches the slit collision-free");routes++;
  }
  fprintf(f,"INFO system %d family %d solids %d span %.0f m\n",sys,p.family,count,span);fflush(f);
 }
 RCHECK(systems==174&&seen==65535&&routes==696,"all 174 Rich systems, sixteen layouts and 696 docking approaches covered");
 for(int family=0;family<16;family++){
  game.system=representative[family];system_bodies(&game);StationProfile p=station_profile_for(&game,0);
  game.docked=game.dock_stage=game.dead=game.police_stop=0;game.planet=-1;game.time=0;game.speed=0;game.voice_time=game.message_time=0;game.route_goal=-1;page=FLIGHT;selected_target=0;
  for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
  for(int v=0;v<2;v++){
   Vec3 aim={0,p.radius*.9f,STATION_Z+p.half*2.5f};game.pos=add(aim,(Vec3){v?-p.radius*5:0,p.radius*1.0f,-p.radius*9});
   Vec3 dir=norm(sub(aim,game.pos));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=0;
   float sum=0,worst=0;
   for(int frame=0;frame<10;frame++){uint64_t a,b;game.time=frame/30.f;drawcount=0;rect(0,0,W,H,BG);sceRtcGetCurrentTick(&a);space();sceRtcGetCurrentTick(&b);float ms=(b-a)*1000.f/sceRtcGetTickResolution();if(frame){sum+=ms;worst=fmaxf(worst,ms);}}
   char name[64];snprintf(name,sizeof(name),"rich-family-%02d-view-%d.bmp",family,v);dump_native_bmp(name);
   fprintf(f,"PERF family %d system %d view %d avg %.3f worst %.3f ms no audio/display\n",family,game.system,v,sum/9,worst);fflush(f);
  }
 }
 for(int i=0;i<16;i++)failures+=pixels[i]!=0xfeed2481||pixels[STRIDE*H+16+i]!=0xfeed2481;
 RCHECK(!surface_depth_on&&!surface_material,"render guards and depth state restored");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
#undef RCHECK
}
