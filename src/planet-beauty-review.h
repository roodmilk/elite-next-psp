static int planet_beauty_review(void){
 FILE *flag=fopen("planet-beauty-review.flag","r");if(!flag)return 0;fclose(flag);
 int failures=planet_beauty_tests("planet-beauty-collision.txt");
 FILE *f=fopen("planet-beauty-review.txt","w");if(!f)return 1;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 field_review_setup(7,1);Vec3 pad=surface_site(&game,1);
 static const float offsets[][2]={{-120,400},{0,1000},{0,2200},{0,3600},{3000,4000},{0,5000},{420,-820}};
 double total=0;float worst=0;int count=0;
 for(int view=0;view<7;view++){
  game.pos=add(pad,(Vec3){offsets[view][0],0,offsets[view][1]});
  game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
  Vec3 target=surface_poi(&game,2);Vec3 dir=sub(target,game.pos);
  game.yaw=view==6?0:atan2f(dir.x,dir.z);game.pitch=0;
  for(int frame=0;frame<8;frame++){
   game.time=20+frame*.1f;uint64_t a=sceKernelGetSystemTimeWide();field_review_frame(0);
   float ms=(sceKernelGetSystemTimeWide()-a)*.001f;total+=ms;worst=fmaxf(worst,ms);count++;
  }
  char path[64];snprintf(path,sizeof(path),"beauty-view-%d.bmp",view);dump_native_bmp(path);
  int ok=lave_prop_count<=2600&&abs(lave_cache_cx-(int)floorf(game.pos.x/70))<=4&&field_leaf_count<=24;
  failures+=!ok;fprintf(f,"%s view %d scenery cache follows player and leaf budget preserved\n",ok?"PASS":"FAIL",view);
 }
 fprintf(f,"SUMMARY walking render avg %.3fms worst %.3fms; cache refresh included\n",total/count,worst);
 for(int i=0;i<16;i++)failures+=pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu;
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
