/* Native renderer proof: deliberately staged real Lave animals, fixed camera. */
static int fauna_review(void){
 FILE *flag=fopen("fauna-review.flag","r");if(!flag)return 0;fclose(flag);
 int failures=fauna_tests("fauna-behaviour.txt");
 FILE *report=fopen("fauna-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!report||!pixels)return 1;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;fb=pixels+16;
 for(int showcase=0;showcase<2;showcase++){
 field_review_setup(7,1);int slot=-1;for(int i=0;i<LIFE_COUNT;i++)if(game.life[i].kind==LIFE_FAUNA){int family=(field_species_seed(7,1,i)>>8)%8,flying=family==0||family==3||family==5;if(flying==showcase)slot=i;}
 if(slot<0)failures++;
 else {
  Lifeform *l=&game.life[slot];for(int i=0;i<LIFE_COUNT;i++)if(i!=slot)game.life[i].alive=0;
  Vec3 pad=surface_site(&game,1);l->pos=add(pad,(Vec3){0,0,280});l->home=l->pos;l->goal=add(l->pos,(Vec3){100,0,0});l->heading=1.5707963f;l->speed=0;l->phase=0;l->behaviour=FAUNA_WANDER;l->state_time=10;l->calm_time=0;
  game.pos=add(l->pos,(Vec3){35,22,-80});game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;game.yaw=game.pitch=game.roll=0;
  float total=0,worst=0;unsigned first=0;int changed=0;
  for(int frame=0;frame<40;frame++){
   uint64_t a,b;sceRtcGetCurrentTick(&a);game_eva_tick(&game,.05f,0,0,0,0,0);game.time+=.05f;field_review_frame(0);sceRtcGetCurrentTick(&b);
   float ms=(b-a)*1000.f/sceRtcGetTickResolution();total+=ms;worst=fmaxf(worst,ms);
   if(!frame)first=field_review_hash();else if(field_review_hash()!=first)changed++;
   char file[64];snprintf(file,sizeof(file),"%s-walk-%02d.bmp",showcase?"flyer":"animal",frame);dump_native_bmp(file);
  }
  fprintf(report,"Lave animal slot %d family %d: movement %.2fm; changed frames %d\n",slot,field_art_family[slot],sqrtf((l->pos.x-l->home.x)*(l->pos.x-l->home.x)+(l->pos.z-l->home.z)*(l->pos.z-l->home.z)),changed);
  if(l->speed<=0||!changed)failures++;
  fprintf(report,"Native update+draw %.3fms average / %.3fms worst, excluding audio/display/capture IO\n",total/40,worst);
  l->behaviour=FAUNA_FEED;l->state_time=4;l->speed=0;field_review_frame(showcase?"flyer-resting.bmp":"animal-feeding.bmp");
 }
 }
 /* Every frame differs in every family; verify real atlas rather than screen noise. */
 int bad=0;for(int family=0;family<8;family++)for(int pose=1;pose<4;pose++)if(!memcmp(fauna_anim_pixels[family][0],fauna_anim_pixels[family][pose],3072))bad++;
 fprintf(report,"%s all eight families have four distinct poses\n",bad?"FAIL":"PASS");failures+=bad;
 for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)failures++;
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=saved;free(pixels);return 1;
}
