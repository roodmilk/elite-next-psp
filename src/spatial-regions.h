/* Finite, seeded world volumes. Back-to-front cards, opaque scene depth,
 * no fullscreen fog overlay and no per-frame allocation or random state. */
typedef struct {Vec3 world,view;float size,opacity,aspect;unsigned ink;int texture;} SpatialCard;
static unsigned char spatial_cloud[3][64*64];
static unsigned spatial_cloud_rgba[64*256] __attribute__((aligned(16)));
static uint64_t spatial_profile_us[3];static unsigned spatial_profile_frames;static int spatial_assets_ready;
static unsigned spatial_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static float spatial_lerp(float a,float b,float t){return a+(b-a)*t;}
static float spatial_noise(int x,int y,int scale,unsigned seed){
 int ix=x/scale,iy=y/scale;float u=(x%scale)/(float)scale,v=(y%scale)/(float)scale;u=u*u*(3-2*u);v=v*v*(3-2*v);
 float a=(spatial_hash(seed+ix*193u+iy*1723u)&255)/255.f,b=(spatial_hash(seed+(ix+1)*193u+iy*1723u)&255)/255.f;
 float c=(spatial_hash(seed+ix*193u+(iy+1)*1723u)&255)/255.f,d=(spatial_hash(seed+(ix+1)*193u+(iy+1)*1723u)&255)/255.f;
 return spatial_lerp(spatial_lerp(a,b,u),spatial_lerp(c,d,u),v);
}
static void spatial_assets_init(void){
 if(spatial_assets_ready)return;
 for(int k=0;k<3;k++)for(int y=0;y<64;y++)for(int x=0;x<64;x++){
  float dx=(x-31.5f)/32,dy=(y-31.5f)/32,r=dx*dx+dy*dy;
  float noise=.56f*spatial_noise(x,y,16,171+k*193)+.29f*spatial_noise(x,y,8,933+k*13)+.15f*spatial_noise(x,y,4,315+k*31);
  float edge=fmaxf(0,1-r);edge=edge*edge*(3-2*edge);
  spatial_cloud[k][y*64+x]=(unsigned char)(255*edge*fmaxf(0,fminf(1,(noise-.16f)*1.55f)));
  spatial_cloud_rgba[y*256+k*64+x]=0x00ffffffu|((unsigned)spatial_cloud[k][y*64+x]<<24);
 }
 sceKernelDcacheWritebackRange(spatial_cloud_rgba,sizeof(spatial_cloud_rgba));spatial_assets_ready=1;
}
static int spatial_view_needed(void){
 if(!navigation_enabled(&game)||game.docked||game.planet>=0||race_is_active(&game))return 0;
 for(int r=0;r<6;r++)if(length(sub(game.pos,navigation_position(&game,NAV_REGION(r))))<90000)return 1;return 0;
}
static int spatial_top(void){return clipy0>=0?(int)fmaxf(view_top(),clipy0):view_top();}
static int spatial_bot(void){return clipy1>=0?(int)fminf(view_bot()+1,clipy1):view_bot()+1;}
static void spatial_card_draw(const SpatialCard*c){
 if(c->view.z<60||c->opacity<=0)return;Point p=project(c->view);float size=proj_focal*c->size/c->view.z;
 if(size<.5f)return;int x0=fmaxf(0,p.x-size),x1=fminf(W-1,p.x+size),y0=fmaxf(spatial_top(),p.y-size*c->aspect),y1=fminf(spatial_bot()-1,p.y+size*c->aspect);
 if(x1<x0||y1<y0)return;unsigned depth=surface_encode_depth(1.f/c->view.z);float sx=32/size,sy=32/(size*c->aspect);int gain=(int)(c->opacity*256);
 int cr=c->ink&255,cg=(c->ink>>8)&255,cb=(c->ink>>16)&255;
 /* Texture opacity is integrated before pixel blending; transparent gaps do
  * not read the framebuffer. Fine detail remains native, with no blur pass. */
 for(int y=y0;y<=y1;y++){int ty=(int)((y-p.y)*sy+32);if(ty<0||ty>63)continue;
  for(int x=x0;x<=x1;x++){int tx=(int)((x-p.x)*sx+32);if(tx<0||tx>63)continue;int a=spatial_cloud[c->texture][ty*64+tx]*gain>>8;if(a<2)continue;
   if(mega_depth_valid&&depth>surface_depth[y*W+x])continue;
   unsigned old=fb[y*STRIDE+x];int inverse=256-a;
   fb[y*STRIDE+x]=RGB(((old&255)*inverse+cr*a)>>8,(((old>>8)&255)*inverse+cg*a)>>8,(((old>>16)&255)*inverse+cb*a)>>8);
  }
 }
}
static unsigned spatial_ge_calls,spatial_ge_cards;
static int spatial_gpu_draw(const SpatialCard*cards,int count){
 if(!count)return 1;if(!ion_gu_ready||fb!=dc_open_source)return 0;
 unsigned color_offset=2*STRIDE*H*4,depth_offset=3*STRIDE*H*4,bytes=STRIDE*H*4;int top=spatial_top(),bot=spatial_bot();
 sceKernelDcacheWritebackInvalidateRange(fb,bytes);sceKernelDcacheWritebackRange(surface_depth,sizeof(surface_depth));
 sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)color_offset,STRIDE);sceGuDepthBuffer((void*)depth_offset,STRIDE);
 sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top,STRIDE,fb,0,top,STRIDE,(void*)(0x04000000u+color_offset));
 /* The existing scene's exact RAM depth is already encoded in ascending
  * world depth. A 2D sprite uses that same encoding without another camera. */
 sceGuCopyImage(GU_PSM_5650,0,top,W,bot-top,W,surface_depth,0,top,STRIDE,(void*)(0x04000000u+depth_offset));sceGuTexSync();
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,top,W,bot);sceGuEnable(GU_SCISSOR_TEST);
 sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);
 sceGuEnable(GU_TEXTURE_2D);sceGuEnable(GU_BLEND);sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);sceGuEnable(GU_DEPTH_TEST);sceGuDepthFunc(GU_LEQUAL);sceGuDepthMask(GU_TRUE);sceGuDepthRange(0,65535);
 sceGuTexMode(GU_PSM_8888,0,0,0);sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);sceGuTexFilter(GU_LINEAR,GU_LINEAR);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexScale(1,1);sceGuTexOffset(0,0);sceGuTexLevelMode(GU_TEXTURE_CONST,0);
 typedef struct{float u,v;unsigned color;float x,y,z;} CloudVertex;CloudVertex*vertices=sceGuGetMemory(count*2*sizeof(*vertices));int n=0;
 sceGuTexImage(0,256,64,256,spatial_cloud_rgba);sceGuTexFlush();
 for(int k=0;k<count;k++){
  const SpatialCard*c=&cards[k];if(c->view.z<60||c->opacity<=0)continue;Point p=project(c->view);float radius=proj_focal*c->size/c->view.z;if(radius<.5f)continue;
  float left=p.x-radius,right=p.x+radius,upper=p.y-radius*c->aspect,lower=p.y+radius*c->aspect;
  float x0=fmaxf(0,left),x1=fminf(W,right),y0=fmaxf(top,upper),y1=fminf(bot,lower);if(x1<=x0||y1<=y0)continue;
  unsigned ink=(c->ink&0xffffffu)|((unsigned)(fminf(1,c->opacity)*255)<<24);float depth=mega_encode_depth(c->view.z);
  CloudVertex*v=vertices+n;v[0]=(CloudVertex){c->texture*64+(x0-left)*32/radius,(y0-upper)*32/(radius*c->aspect),ink,x0,y0,depth};v[1]=(CloudVertex){c->texture*64+(x1-left)*32/radius,(y1-upper)*32/(radius*c->aspect),ink,x1,y1,depth};
  n+=2;spatial_ge_cards++;
 }
 if(n)sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,n,0,vertices);
 sceGuDisable(GU_BLEND);sceGuDepthMask(GU_FALSE);sceGuDisable(GU_DEPTH_TEST);sceGuTexSync();sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top,STRIDE,(void*)(0x04000000u+color_offset),0,top,STRIDE,fb);sceGuFinish();int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,bytes);spatial_ge_calls++;
 return result>=0;
}
static void spatial_opaque_depth(void){
 if(!mega_depth_valid)memset(surface_depth,255,sizeof(surface_depth));
 /* Reuse the exact submitted hull triangles, rather than approximate spheres
  * that would erase cloud wisps in gaps between a station's structures. */
 /* Hardware hull passes already wrote their exact CPU visibility depth. */
 static float ray_x[W],ray_x2[W],last_focal;static int last_ox=-999;
 if(last_focal!=proj_focal||last_ox!=proj_ox){for(int x=0;x<W;x++){ray_x[x]=(x+.5f-proj_ox)/proj_focal;ray_x2[x]=ray_x[x]*ray_x[x];}last_focal=proj_focal;last_ox=proj_ox;}
 for(int body=0;body<BODY_COUNT;body++){
  Body*b=&game.bodies[body];if(!body_active(b)||!render_sphere_visible(b->pos,b->radius))continue;Vec3 c=camera(&game,b->pos);float radius=b->radius;if(c.z+radius<=1)continue;
  float distance=length(c);if(distance<=radius)continue;float bound=proj_focal*radius/fmaxf(1,c.z-radius);Point p=project(c);
  int x0=fmaxf(0,p.x-bound),x1=fminf(W-1,p.x+bound),y0=fmaxf(spatial_top(),p.y-bound),y1=fminf(spatial_bot()-1,p.y+bound);float constant=dot(c,c)-radius*radius;
  for(int y=y0;y<=y1;y++){float ry=(proj_oy-y-.5f)/proj_focal;for(int x=x0;x<=x1;x++){
   float length2=ray_x2[x]+ry*ry+1,along=ray_x[x]*c.x+ry*c.y+c.z,disc=along*along-length2*constant;if(disc<=0||along<=0)continue;
   float depth=(along-sqrtf(disc))/length2;if(depth<=0)continue;unsigned encoded=mega_encode_depth(depth);if(encoded<surface_depth[y*W+x])surface_depth[y*W+x]=encoded;
  }}
 }
 mega_depth_valid=1;
}
static void spatial_regions_draw(void){
 if(!spatial_view_needed())return;uint64_t stamp=sceKernelGetSystemTimeWide();int prior_encoding=mega_depth_encoding;mega_depth_encoding=1;spatial_assets_init();SpatialCard cards[216];int count=0;
 static const unsigned palette[6][3]={
  {0xffa87635,0xffc6a546,0xff483019},{0xffb17c54,0xffa86e8b,0xff695153},
  {0xff9ca64d,0xff8f9e35,0xffbdac72},{0xff756450,0xff8b7963,0xff514e49},
  {0xffb7b39d,0xffab9668,0xffbfaaa0},{0xff665d51,0xff8a7862,0xff51483e}};
 for(int r=0;r<6;r++){
  Vec3 centre=navigation_position(&game,NAV_REGION(r));float distance=length(sub(centre,game.pos));if(distance>90000)continue;
  float radius=spatial_region_radius(r),fade=fminf(1,(90000-distance)/20000),activity=r==1?spatial_density(&game,1,centre):1;
  if(activity<=0)continue;
  for(int k=0;k<36;k++){
   unsigned h=spatial_hash((game.system+1)*17231u+r*137+k*193u);float a=k*2.399963f,u=((h>>8)&255)/255.f;
   Vec3 offset={cosf(a)*radius*sqrtf(u)*.82f,sinf(k*1.914f)*radius*.36f,sinf(a)*radius*sqrtf(u)*.82f};
   offset.x+=sinf(game.world_clock*.007f+k)*190;offset.y+=cosf(game.world_clock*.009f+k)*130;
   if(r==4){offset.x*=1.6f;offset.y*=.26f;offset.z*=.32f;}
   if(r==2){offset.x=cosf(k*.085f)*radius*.7f;offset.z=(k-17.5f)*radius*.042f;offset.y=sinf(k*.19f+game.world_clock*.013f)*radius*.12f;}
   Vec3 world=add(centre,offset),v=camera(&game,world);if(v.z<60)continue;
   float size=radius*(r==2?.13f:.25f+((h>>20)&31)*.003f),opacity=(r==2?.15f:r==3?.11f:.27f)*fade*activity*fminf(1,(v.z-60)/700);
   if(r==4)opacity*=1.f-k/48.f;
   unsigned ink=palette[r][k%3];
   /* Brief, local internal illumination; not a whole-screen flash. */
   float phase=fmodf(game.world_clock+r*11,17);if(r==1&&phase<.16f&&k%6==0){ink=RGB(188,200,237);opacity=fminf(.55f,opacity*2);}
   float aspect=r==2?2.8f:.68f;Point projected=project(v);float pixels=proj_focal*size/v.z;
   if(projected.x+pixels<0||projected.x-pixels>W||projected.y+pixels*aspect<spatial_top()||projected.y-pixels*aspect>=spatial_bot())continue;
   SpatialCard card={world,v,size,opacity,aspect,ink,(int)((h>>16)%3)};int at=count++;while(at&&cards[at-1].view.z<v.z){cards[at]=cards[at-1];at--;}cards[at]=card;
  }
 }
 spatial_profile_us[0]+=sceKernelGetSystemTimeWide()-stamp;stamp=sceKernelGetSystemTimeWide();if(count)spatial_opaque_depth();spatial_profile_us[1]+=sceKernelGetSystemTimeWide()-stamp;stamp=sceKernelGetSystemTimeWide();
 if(!spatial_gpu_draw(cards,count))for(int k=0;k<count;k++)spatial_card_draw(&cards[k]);
 spatial_profile_us[2]+=sceKernelGetSystemTimeWide()-stamp;spatial_profile_frames++;
 Vec3 storm=navigation_position(&game,22);float pulse=fmodf(game.world_clock+11,17);
 if(pulse<.16f&&length(sub(storm,game.pos))<35000&&spatial_density(&game,1,storm)>.2f){
  for(int branch=0;branch<3;branch++)for(int k=0;k<32;k++){
   float u=k/31.f;Vec3 p=add(storm,(Vec3){(u-.5f)*4300+(branch?u*branch*750:0),sinf(k*.73f+branch)*170+branch*u*1100,700+sinf(k*.37f)*180});
   rift_volume_sample(p,RGB(161,202,239),2,.6f);
  }
 }

 /* Depth-distributed pinpoint grains, never a screen-locked star overlay. */
 if(game.spatial_level>.05f&&game.spatial_region>=0){int r=game.spatial_region;Vec3 c=navigation_position(&game,NAV_REGION(r));for(int k=0;k<24;k++){float a=k*2.39996f;Vec3 p=add(c,(Vec3){cosf(a)*(1800+k*173),sinf(k*1.7f)*3200,sinf(a)*(1800+k*173)});rift_volume_sample(p,RGB(104,169,187),1,.34f);}}
 mega_depth_encoding=prior_encoding;
}
