static int lave_world_review(void){
 FILE *flag=fopen("lave-world-review.flag","r");if(!flag)return 0;fclose(flag);
 int failures=lave_world_tests("lave-world-tests.txt");
 FILE *f=fopen("lave-world-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!f||!pixels)return 1;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;fb=pixels+16;
 for(int body=2;body<=4;body++){
  field_review_setup(7,body);Vec3 pad=surface_site(&game,1);
  static const int days[]={600,900,1200,1800};float h=field_local_hour(&game,7,body);game.world_clock=(10-h+24)*days[(field_seed(7,body)>>24)&3]/24.f;
  fprintf(f,"Planet %d orbital type %d biome %d\n",body,game.bodies[body].type,planet_biome(&game.bodies[body]));
  for(int view=0;view<9;view++){
   game.pos=add(pad,(Vec3){-100,0,body==3?240:420});game.yaw=view<4?view*1.5707963f:0;game.pitch=view==4?-.55f:view==5?.5f:0;
   if(view>=6){Vec3 p=surface_poi(&game,2);game.pos=add(p,(Vec3){0,0,-330});game.yaw=0;game.pitch=.18f;if(view==7)game.world_clock+=days[(field_seed(7,body)>>24)&3]*.5f;}
   if(view==8){
    game.world_clock=(10-h+24)*days[(field_seed(7,body)>>24)&3]/24.f;
    if(body==3){game.pos=add(pad,(Vec3){-180,0,200});game.yaw=2.7f;game.pitch=0;}
    else {
     int found=0;for(int dz=4;dz<14&&!found;dz++)for(int dx=-6;dx<7&&!found;dx++){
      unsigned seed;float x,z;int k=field_prop_cell(7,body,game.bodies[body].seed,(int)floorf(pad.x/70)+dx,(int)floorf(pad.z/70)+dz,&x,&z,&seed);if(k!=0||fabsf(x-pad.x)<280&&fabsf(z-pad.z)<280)continue;
      int clear=1;for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(&game,id);FieldBuilding b=field_site_building(&game,id);if(fabsf(x-p.x)<b.w+30&&fabsf(z-p.z)<b.d+30)clear=0;}
      if(clear){game.pos=(Vec3){x-25,0,z-100};game.yaw=.15f;game.pitch=.08f;found=1;}
     }
    }
   }
   game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
   char file[60];snprintf(file,sizeof(file),"world-%d-view-%d.bmp",body,view);field_review_frame(file);
  }
  field_review_setup(7,body);h=field_local_hour(&game,7,body);game.world_clock=(10-h+24)*days[(field_seed(7,body)>>24)&3]/24.f;
  game.pos=add(pad,(Vec3){-100,0,body==3?220:420});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=0;
  uint64_t total=0,worst=0;
  for(int frame=0;frame<60;frame++){
   uint64_t a,b;sceRtcGetCurrentTick(&a);game_eva_tick(&game,1.f/30,frame>30?.3f:0,0,frame<30?1:0,0,0);game.time+=1.f/30;field_review_frame(0);sceRtcGetCurrentTick(&b);
   if(frame>=5){total+=b-a;if(b-a>worst)worst=b-a;}
   if(frame%5==0){char file[60];snprintf(file,sizeof(file),"world-%d-motion-%02d.bmp",body,frame);dump_native_bmp(file);}
  }
  fprintf(f,"Planet %d update+draw %.3fms avg / %.3fms worst (no audio/display/capture)\n",body,total*1000.f/sceRtcGetTickResolution()/55,worst*1000.f/sceRtcGetTickResolution());
 }
 for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)failures++;
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
