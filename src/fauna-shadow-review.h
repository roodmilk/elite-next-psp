static int fauna_shadow_review(void){
 FILE *flag=fopen("fauna-shadow-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("fauna-shadow-review.txt","w");if(!f)return 1;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;
 #define SHCHECK(ok,name) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",name);failures+=!pass;fflush(f);}while(0)
 for(int body=1;body<=4;body++){
  field_review_setup(7,body);int slot=-1;
  for(int i=0;i<LIFE_COUNT;i++)if(game.life[i].kind==LIFE_FAUNA)slot=i;
  SHCHECK(slot>=0,"world supplies discoverable fauna");if(slot<0)continue;
  Lifeform *l=&game.life[slot];Vec3 pad=surface_site(&game,1);
  for(int i=0;i<LIFE_COUNT;i++)game.life[i].alive=0;
  l->pos=add(pad,(Vec3){100,0,-220});l->pos.y=terrain_height(&game,l->pos.x,l->pos.z);l->lift=0;l->speed=0;l->behaviour=FAUNA_IDLE;
  game.pos=add(l->pos,(Vec3){0,22,-55});game.pitch=-.15f;game.yaw=game.roll=0;
  field_review_frame(0);unsigned base=field_review_hash();surface_depth_on=1;
  unsigned depth_before=0;for(int i=0;i<W*H;i++)depth_before=depth_before*31+surface_depth[i];
  SHCHECK(field_fauna_shadow(l)==0,"inactive fauna do not leave shadows");
  l->alive=1;int count=field_fauna_shadow(l);
  SHCHECK(count>8&&count<2500&&base!=field_review_hash(),"small contact shadow darkens visible terrain");
  unsigned depth_after=0;for(int i=0;i<W*H;i++)depth_after=depth_after*31+surface_depth[i];
  SHCHECK(depth_before==depth_after,"shadows preserve terrain and object depth");
  field_draw_world(l);char path[48];snprintf(path,sizeof(path),"shadow-lave-%d.bmp",body);dump_native_bmp(path);
  l->lift=15;field_review_frame(0);snprintf(path,sizeof(path),"shadow-lift-lave-%d.bmp",body);dump_native_bmp(path);
  l->lift=0;surface_depth_on=1;memset(surface_depth,0,sizeof(surface_depth));
  SHCHECK(field_fauna_shadow(l)==0,"nearer occluder prevents shadow bleeding through");
  memset(surface_depth,255,sizeof(surface_depth));SHCHECK(field_fauna_shadow(l)==0,"empty background never receives a shadow");
  l->pos.z=game.pos.z-20;SHCHECK(field_fauna_shadow(l)==0,"behind-camera animal produces no shadow");
 }
 int guards=1;for(int i=0;i<16;i++)guards&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 SHCHECK(guards,"all native framebuffer guards preserved");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
 #undef SHCHECK
}
