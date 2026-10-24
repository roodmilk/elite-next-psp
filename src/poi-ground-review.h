/* Opt-in native placement regression. No player saves or controls changed. */
static int poi_ground_review(void){
 FILE *flag=fopen("poi-ground-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("poi-ground-review.txt","w");if(!f)return 1;
 int failures=0,sites=0,kinds=0;float worst_clearance=1e9f;
 for(int sys=0;sys<256;sys++)for(int body=1;body<=4;body++){
  field_review_setup(sys,body);
  for(int id=1;id<10;id++){if(id==6)continue;Vec3 p=surface_poi(&game,id);FieldBuilding s=field_site_building(&game,id);int kind=field_site_kind(&game,sys,body,id);
   if(id==1){static const int jobs[]={FIELD_WEATHER,FIELD_GARDEN,FIELD_ARCHIVE};kind=jobs[field_profile(&game,sys,body).job];}kinds|=1<<kind;
   float grade=lm_footing_grade(p,s.w,s.d,kind),again=lm_footing_grade(p,s.w,s.d,kind),spread=lm_natural_site(kind)?1.16f:1.10f;
   int ok=grade==again;for(int z=0;z<5;z++)for(int x=0;x<5;x++){
    float wx=p.x+s.w*spread*(x*.5f-1),wz=p.z+s.d*spread*(z*.5f-1);
    float ground=fmaxf(lm_mesh_height(wx,wz,sys==7?160:40),lm_mesh_height(wx,wz,sys==7?960:280));float clearance=grade-ground;
    worst_clearance=fminf(worst_clearance,clearance);ok&=clearance>=.58f;
   }
   if(!ok){fprintf(f,"FAIL system%d world%d POI%d footprint/cached grade\n",sys,body,id);failures++;}sites++;
  }
 }
 fprintf(f,"%s %d footprints checked, family mask%03x, minimum terrain clearance%.3fm\n",failures?"FAIL":"PASS",sites,kinds,worst_clearance);failures+=kinds!=4095;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fclose(f);return 1;}fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 field_review_setup(7,1);Vec3 p=surface_poi(&game,2);FieldBuilding s=field_site_building(&game,2);float ground=terrain_height(&game,p.x,p.z),grade=lm_footing_grade(p,s.w,s.d,FIELD_DISH);
 fprintf(f,"Lave I observatory centre ground%.3f, graded earth%.3f, basement bottom%.3f (old%.3f)\n",ground,grade,grade+1.2f,ground-8);
 for(int view=0;view<12;view++){
  float a=(view%4)*1.5707963f,range=view<4?300:view<8?900:2200;
  game.pos=(Vec3){p.x+sinf(a)*range,0,p.z-cosf(a)*range};game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
  Vec3 target={p.x,grade+60,p.z},dir=sub(target,game.pos);game.yaw=atan2f(dir.x,dir.z);game.pitch=atan2f(dir.y,sqrtf(dir.x*dir.x+dir.z*dir.z));game.roll=0;
  char name[64];snprintf(name,sizeof(name),"observatory-ground-%02d.bmp",view);field_review_frame(name);
 }
 int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 fprintf(f,"%s native multi-angle framebuffer guards\n",guard?"PASS":"FAIL");failures+=!guard;
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);return 1;
}
