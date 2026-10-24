static unsigned station_review_depth_hash(void){unsigned h=2166136261u;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)h=(h^surface_depth[y*W+x])*16777619u;return h;}
static Vec3 station_review_vertex(const MegaBlock*b,Vec3 v){return station_vertex_at((Vec3){b->c.x+v.x*b->e.x,b->c.y+v.y*b->e.y,b->c.z+v.z*b->e.z},(Vec3){0,0,STATION_Z},station_angle(&game));}
static int station_stability_review(void){
 FILE*flag=fopen("station-stability.flag","r");if(!flag)return 0;fclose(flag);FILE*f=fopen("station-stability.txt","w");if(!f)return 1;
 unsigned*old=fb,*mem=calloc(STRIDE*H+32,4);if(!mem){fclose(f);return 1;}fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0x462ccafe;
 int fail=0;double ms[2]={0,0};unsigned depth[2][3][24];
 for(int mode=0;mode<2;mode++)for(int pose=0;pose<3;pose++){
  graphics_scene(0);station_art_stable=mode;soft_sky_prepare(space_sky_seed());
  for(int frame=0;frame<24;frame++){
   game.time=50+frame/30.f;float angle=pose==1?.5f:pose==2?.85f:0,distance=pose==0?4500:pose==1?2300:6500;
   game.pos=(Vec3){sinf(angle)*distance+frame*1.25f,200+frame*.4f,STATION_Z-cosf(angle)*distance};
   Vec3 aim=norm(sub((Vec3){0,0,STATION_Z},game.pos));game.yaw=atan2f(aim.x,aim.z);game.pitch=asinf(aim.y);game.roll=0;
   rect(0,0,W,H,BG);drawcount=0;uint64_t a=sceKernelGetSystemTimeWide();space();ms[mode]+=(sceKernelGetSystemTimeWide()-a)*.001;
   depth[mode][pose][frame]=station_review_depth_hash();char name[64];snprintf(name,sizeof(name),"stability-%d-%d-%02d.bmp",mode,pose,frame);dump_native_bmp(name);
  }
 }
 int same=1;for(int p=0;p<3;p++)for(int i=0;i<24;i++)same&=depth[0][p][i]==depth[1][p][i];fail+=!same;
 fprintf(f,"%s 72 matched moving native views retain exact geometry/depth\n",same?"PASS":"FAIL");
 int guards=1;for(int i=0;i<16;i++)guards&=mem[i]==0x462ccafe&&mem[STRIDE*H+16+i]==0x462ccafe;fail+=!guards;
 fprintf(f,"%s framebuffer guards\nINFO mean render baseline %.3fms filtered %.3fms; staged emulator, not physical PSP frame pacing\n",guards?"PASS":"FAIL",ms[0]/72,ms[1]/72);
 station_art_stable=1;station_art_footprint=0;fb=old;free(mem);fprintf(f,"RESULT %d failures\n",fail);fclose(f);return 1;
}
static int station_art_review(void){
 if(station_stability_review())return 1;
 FILE*flag=fopen("station-art-review.flag","r");if(!flag)return 0;fclose(flag);FILE*out=fopen("station-art-review.txt","w");if(!out)return 1;unsigned*old=fb,*memory=calloc(STRIDE*H+32,sizeof(unsigned));if(!memory){fclose(out);return 1;}fb=memory+16;for(int i=0;i<16;i++)memory[i]=memory[16+STRIDE*H+i]=0xa21204ce;int fail=0,targets[28];for(int i=0;i<28;i++)targets[i]=-1;graphics_scene(0);
 for(int sys=0;sys<256;sys++){game.system=sys;StationProfile p=station_profile_for(&game,0);int key=p.station_class==STATION_MEGA?24+((p.seed>>18)&3):p.station_class==STATION_RICH?8+p.family:p.family;if(targets[key]<0)targets[key]=sys;}
 for(int key=0;key<28;key++){
  if(targets[key]<0){fprintf(out,"FAIL no generated system for layout%d\n",key);fail++;continue;}
  graphics_scene(0);game.system=targets[key];system_bodies(&game);for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;
  mc_profile=station_profile_for(&game,0);const MegaCity*m=station_architecture_for(&game);mc_draw_city=m;station_art_override=-1;station_art_enabled=1;station_art_prepare();soft_sky_prepare(space_sky_seed());game.voice_time=game.message_time=0;
  Vec3 lo={1e9f,1e9f,1e9f},hi={-1e9f,-1e9f,-1e9f};for(int i=0;i<m->n;i++){const MegaBlock*b=&m->b[i];const MegaGeometry*g=mega_geometry_for(b->shape);for(int j=0;j<g->vertices;j++){Vec3 p=station_review_vertex(b,g->v[j]);lo.x=fminf(lo.x,p.x);lo.y=fminf(lo.y,p.y);lo.z=fminf(lo.z,p.z);hi.x=fmaxf(hi.x,p.x);hi.y=fmaxf(hi.y,p.y);hi.z=fmaxf(hi.z,p.z);}}
  Vec3 centre=mul(add(lo,hi),.5f);
  for(int angle=0;angle<3;angle++){
   Vec3 dir=norm(angle==0?(Vec3){.08f,.06f,1}:angle==1?(Vec3){.65f,.16f,1}:(Vec3){.4f,.13f,-1});Vec3 right=norm((Vec3){dir.z,0,-dir.x}),up=mc_cross(dir,right);float distance=100;
   for(int i=0;i<m->n;i++){const MegaBlock*b=&m->b[i];const MegaGeometry*g=mega_geometry_for(b->shape);for(int j=0;j<g->vertices;j++){Vec3 q=sub(station_review_vertex(b,g->v[j]),centre);distance=fmaxf(distance,fmaxf(fabsf(dot(q,right))*240/196-dot(q,dir),fabsf(dot(q,up))*240/70-dot(q,dir))+100);}}
   game.pos=sub(centre,mul(dir,distance));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=0;unsigned depth[2];double means[2];
   for(int mode=0;mode<2;mode++){station_art_enabled=mode;double sum=0;for(int frame=0;frame<8;frame++){rect(0,0,W,H,BG);drawcount=0;uint64_t a,b;sceRtcGetCurrentTick(&a);space();sceRtcGetCurrentTick(&b);if(frame>1)sum+=(b-a)*1000.0/sceRtcGetTickResolution();}means[mode]=sum/6;depth[mode]=station_review_depth_hash();if(mode){char name[64];snprintf(name,sizeof(name),"station-layout-%02d-angle-%d.bmp",key,angle);dump_native_bmp(name);}}
   int same=depth[0]==depth[1];fail+=!same;fprintf(out,"%s layout%02d angle%d geometry/depth unchanged base%.3f authored%.3fms\n",same?"PASS":"FAIL",key,angle,means[0],means[1]);fflush(out);
  }
  int roles=station_art_role(&m->b[0],0)==0;for(int i=1;i<m->n;i++){const MegaBlock*b=&m->b[i];if(mc_profile.station_class!=STATION_MEGA&&b->kind==1)roles&=station_art_role(&m->b[i],i)==9;if(b->kind==7)roles&=station_art_role(&m->b[i],i)==6;if(mc_profile.station_class==STATION_MEGA&&!b->shape&&(b->kind==1||b->kind==5)){unsigned sampled;roles&=!station_art_texel(b,0,0,0,1000,&sampled);}}fail+=!roles;fprintf(out,"%s layout%02d system%d class%d family%d components%d suite%d docking/billboard/radiator roles retained\n",roles?"PASS":"FAIL",key,game.system,mc_profile.station_class,mc_profile.family,m->n,station_art_style);
 }
 for(int crown=0;crown<4;crown++){
  graphics_scene(0);game.system=targets[24+crown];system_bodies(&game);for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;mc_profile=station_profile_for(&game,0);const MegaCity*m=station_architecture_for(&game);mc_draw_city=m;station_art_enabled=1;station_art_prepare();soft_sky_prepare(space_sky_seed());
  const MegaBlock*b=&m->b[1];Vec3 centre=station_vertex_at(b->c,(Vec3){0,0,STATION_Z},station_angle(&game)),dir=norm((Vec3){.3f,.06f,1});float distance=fmaxf(b->e.x*240/150,b->e.y*240/62)+b->e.z+600;game.pos=sub(centre,mul(dir,distance));game.yaw=atan2f(dir.x,dir.z);game.pitch=asinf(dir.y);game.roll=0;for(int frame=0;frame<8;frame++){rect(0,0,W,H,BG);drawcount=0;space();}char name[64];snprintf(name,sizeof(name),"capital-district-%d.bmp",crown);dump_native_bmp(name);fprintf(out,"PASS capital%d native near district view captured\n",crown);fflush(out);
 }
 int coverage=1;for(int sys=0;sys<256;sys++){game.system=sys;mc_profile=station_profile_for(&game,0);mc_draw_city=station_architecture_for(&game);station_art_enabled=1;station_art_prepare();coverage&=station_art_pixels&&station_art_active()&&station_art_style>=0&&station_art_style<6;for(int i=0;i<mc_draw_city->n;i++)coverage&=station_art_role(&mc_draw_city->b[i],i)<10;}fail+=!coverage;fprintf(out,"%s all256 actual systems have bounded architectural suite/roles\n",coverage?"PASS":"FAIL");high_contrast=1;int contrast=!station_art_active();high_contrast=0;fail+=!contrast;fprintf(out,"%s high contrast retains existing path\n",contrast?"PASS":"FAIL");int guard=1;for(int i=0;i<16;i++)guard&=memory[i]==0xa21204ce&&memory[STRIDE*H+16+i]==0xa21204ce;fail+=!guard;fprintf(out,"%s framebuffer guards retained\n",guard?"PASS":"FAIL");fprintf(out,"INFO native480x272 staged actual systems/meshes; all16rich,8frontier and4capital-crown designs. Performance not physicalPSP certification.\nRESULT %d failures\n",fail);fclose(out);fb=old;free(memory);return 1;
}


