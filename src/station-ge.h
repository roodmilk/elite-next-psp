/* PSP-3000 near-station panel acceleration; CPU visibility is retained. */
typedef struct {float u,v,x,y,z;} StationGeVertex;
#include "fleet-ge.h"
typedef struct {unsigned palette[16];int style,role,stamp,dim,ready,flat;unsigned char glyphs[16];} StationGeKey;
enum {STATION_GE_TEXTURE_SLOTS=144};
static unsigned *station_ge_pixels;static StationGeKey *station_ge_keys;
static int station_ge_ready,station_ge_failed,station_ge_predepth;
static unsigned station_ge_calls,station_ge_triangles;
static unsigned station_ge_list_peak;
static uint64_t station_ge_profile_us[5];
static unsigned station_ge_mip_pixels[6][8][768] __attribute__((aligned(16)));
static StationGeKey station_ge_mip_keys[6][8];
static const int station_ge_mip_offsets[5]={0,256,384,512,640};
static unsigned*station_ge_mip_prepare(int face,int role){
 StationGeKey*k=&station_ge_mip_keys[face][role];unsigned*data=station_ge_mip_pixels[face][role];
 if(!k->ready||k->style!=station_art_style||k->dim!=station_art_face.flat||memcmp(k->palette,station_art_face.palette,64)){
  uint64_t stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
  k->ready=1;k->style=station_art_style;k->dim=station_art_face.flat;memcpy(k->palette,station_art_face.palette,64);
  for(int level=0;level<5;level++){int size=16>>level,height=size<8?8:size;unsigned*out=data+station_ge_mip_offsets[level];const unsigned*in=station_art_face.mip_data+station_art_mip_offset[level];for(int y=0;y<height;y++)for(int x=0;x<16;x++){int at=(y>>3)*128+(x>>2)*32+(y&7)*4+(x&3);out[at]=station_art_face.flat?station_art_face.palette[1]:in[(y<size?y:size-1)*size+(x<size?x:size-1)];}}
  sceKernelDcacheWritebackRange(data,768*4);if(render_profile_enabled)station_ge_profile_us[0]+=sceKernelGetSystemTimeWide()-stamp;
 }return data;
}
static int station_ge_prepare(void){
 if(station_ge_failed)return 0;if(station_ge_ready)return 1;
 station_ge_pixels=memalign(16,STATION_GE_TEXTURE_SLOTS*1024*4);station_ge_keys=calloc(STATION_GE_TEXTURE_SLOTS,sizeof(*station_ge_keys));
 if(!station_ge_pixels||!station_ge_keys){free(station_ge_pixels);free(station_ge_keys);station_ge_pixels=0;station_ge_keys=0;station_ge_failed=1;return 0;}
 station_ge_ready=1;return 1;
}
static int station_ge_view_ready(void){
 if(!flight_cached_frame||!ion_gu_ready||page!=FLIGHT||clipy0>=0||game.docked||game.dead||game.planet>=0||game.approach>=0||game.dock_stage>=2||game.police_stop||game.rift_report||thargoid_active||race_ready(&game)||opening_dream(&game)||planet_entry_body||planet_orbit_veil>0||((hud_mode!=0||hud_hidden)&&!hc_full_console_visible())||high_contrast||!station_art_enabled||render_reference)return 0;
 /* Avoid paying GPU transfer costs for a tiny distant station. Hysteresis
  * prevents repeated path changes while hovering around the boundary. */
 static int near_station,system=-1;Vec3 d=sub(game.pos,(Vec3){0,0,STATION_Z});float distance2=dot(d,d);
 if(system!=game.system){system=game.system;near_station=0;}
 if(distance2>9000.f*9000.f)near_station=0;else if(distance2<7000.f*7000.f)near_station=1;
 return near_station&&mc_scene_visible()&&station_ge_prepare();
}
/* Keep the established CPU visibility surface for ships, lights and targeting.
 * This pass writes depth only: hardware owns panel shading/filtering. */
static void station_ge_depth_triangle(DrawTri*t){
 Point*p=t->p;float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 float iz0=(1.f/p[0].z)/area,iz1=(1.f/p[1].z)/area,iz2=(1.f/p[2].z)/area,di=2*((p[2].y-p[1].y)*iz0+(p[0].y-p[2].y)*iz1+(p[1].y-p[0].y)*iz2);
 int first=(int)surface_max(surface_y_min,surface_floor(surface_min(p[0].y,surface_min(p[1].y,p[2].y)))),last=(int)surface_min(surface_y_max,surface_ceil(surface_max(p[0].y,surface_max(p[1].y,p[2].y))));first=(first+1)&~1;
 float slope[3];for(int k=0;k<3;k++){Point a=p[k],b=p[(k+1)%3];slope[k]=b.y!=a.y?(b.x-a.x)/(b.y-a.y):0;}
 for(int y=first;y<=last;y+=2){float yy=y+1,left=1e9f,right=-1e9f;for(int k=0;k<3;k++){Point a=p[k],b=p[(k+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*slope[k];if(x<left)left=x;if(x>right)right=x;}}
  if(left>right)continue;int x0=(int)surface_max(0,surface_ceil(left-1)),x1=(int)surface_min(W-1,surface_floor(right-1));x0=(x0+1)&~1;float a=edge(p[1],p[2],x0+1,yy),b=edge(p[2],p[0],x0+1,yy),c=edge(p[0],p[1],x0+1,yy),iz=a*iz0+b*iz1+c*iz2;
  for(int x=x0;x<=x1;x+=2,iz+=di){if(iz<=0)continue;unsigned depth=mega_encode_depth(1.f/iz);int at=y*W+x;if(depth<=surface_depth[at])surface_depth[at]=depth;if(depth<=surface_depth[at+1])surface_depth[at+1]=depth;if(y+1<=surface_y_max){if(depth<=surface_depth[at+W])surface_depth[at+W]=depth;if(depth<=surface_depth[at+W+1])surface_depth[at+W+1]=depth;}}
 }
}
/* Blocks with the same face palette/role share identical pixels. Numbered ring
 * panels retain their own slots, so no artwork or lettering is discarded. */
static int station_ge_slot(int id,int face,int far){
 if(station_art_face.stamp_y==11){int sector=mc_draw_city->b[id].shape-MC_RING0;return 96+(face*12+sector)*2+far;}
 return face*16+station_art_face.role*2+far;
}
static unsigned*station_ge_texture(int id,int face,int far){
 int slot=station_ge_slot(id,face,far);StationGeKey*k=&station_ge_keys[slot];unsigned*texture=station_ge_pixels+slot*1024;
 unsigned char glyphs[16]={0};int sy=station_art_face.stamp_y;if(sy>=0){memcpy(glyphs,station_art_face.glyph0,8);memcpy(glyphs+8,station_art_face.glyph1,8);}
 if(!k->ready||k->style!=station_art_style||k->role!=station_art_face.role||k->stamp!=sy||k->dim!=far||k->flat!=station_art_face.flat||memcmp(k->palette,station_art_face.palette,64)||memcmp(k->glyphs,glyphs,16)){
  uint64_t stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
  k->ready=1;k->style=station_art_style;k->role=station_art_face.role;k->stamp=sy;k->dim=far;k->flat=station_art_face.flat;memcpy(k->palette,station_art_face.palette,64);memcpy(k->glyphs,glyphs,16);
  for(int y=0;y<32;y++)for(int x=0;x<32;x++){int packed=station_art_face.pixels[y*16+(x>>1)],ci=x&1?packed>>4:packed&15;
   if(sy>=0&&x>=8&&x<24&&y>=sy&&y<sy+8){const unsigned char*g=x<16?glyphs:glyphs+8;if(g[y-sy]&(128>>((x-8)&7)))ci=sy==11?10:1;}
   if(far&&(ci==7||ci==11))ci=station_art_face.dim_ci;int at=(y>>3)*256+(x>>2)*32+(y&7)*4+(x&3);texture[at]=station_art_face.flat?station_art_face.palette[1]:station_art_face.palette[ci];
  }sceKernelDcacheWritebackRange(texture,4096);if(render_profile_enabled)station_ge_profile_us[0]+=sceKernelGetSystemTimeWide()-stamp;
 }return texture;
}
#include "station-ge-batch.h"
#include "encounter-ge-legacy.h"
static int station_ge_flush(void){
 if(!(station_ge_active_frame||fleet_ge_active_frame)||!flight_cached_frame||!ion_gu_ready||fb!=dc_open_source||!mc_depth_active||game.planet>=0||race_ready(&game)||!station_art_enabled||high_contrast||drawcount<1)return 0;
 if(fleet_ge_active_frame&&(fleet_ge_prior_overflow||fleet_ge_prior_count*48+drawcount*72+8192>470000))return 0;
 int authored=0;for(int i=0;i<drawcount;i++)authored+=drawlist[i].material<0;if(!authored&&!fleet_ge_active_frame)return 0;
 if(authored&&!station_ge_prepare())return 0;
 uint64_t gpu_stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
 unsigned offset=2*STRIDE*H*4,depth_offset=3*STRIDE*H*4,bytes=STRIDE*H*4;int top=surface_y_min,bot=surface_y_max+1;unsigned char fallback[2048]={0};
 sceKernelDcacheWritebackInvalidateRange(fb,bytes);sceKernelDcacheWritebackRange(station_art_mips,sizeof(station_art_mips));
 if(++encounter_ge_epoch==0){memset(encounter_ge_used,0,sizeof(encounter_ge_used));encounter_ge_epoch=1;}
 sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)offset,STRIDE);sceGuDepthBuffer((void*)depth_offset,STRIDE);
 sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top,STRIDE,fb,0,top,STRIDE,(void*)(0x04000000u+offset));sceGuTexSync();
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,top,W,bot);sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_BLEND);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);sceGuEnable(GU_CLIP_PLANES);sceGuEnable(GU_TEXTURE_2D);sceGuEnable(GU_DEPTH_TEST);sceGuDepthFunc(GU_LEQUAL);sceGuDepthMask(GU_FALSE);sceGuDepthRange(0,65535);sceGuClearDepth(65535);sceGuClear(GU_DEPTH_BUFFER_BIT);
 float near=1e9f;for(int i=0;i<drawcount;i++)for(int k=0;k<3;k++)if(drawlist[i].p[k].z<near)near=drawlist[i].p[k].z;if(fleet_ge_active_frame)for(int i=0;i<fleet_ge_prior_count;i++)for(int k=0;k<3;k++)near=fminf(near,fleet_ge_prior[i].p[k].z);near*=.5f;if(near<1)near=1;

 ScePspFMatrix4 identity={0},projection={0};identity.x.x=identity.y.y=identity.z.z=identity.w.w=1;projection.x.x=2*proj_focal/W;projection.y.y=2*proj_focal/H;projection.z.x=2.f*proj_ox/W-1;projection.z.y=1-2.f*proj_oy/H;projection.z.z=1;projection.w.z=-2*near;projection.z.w=1;
 sceGuSetMatrix(GU_PROJECTION,&projection);sceGuSetMatrix(GU_VIEW,&identity);sceGuSetMatrix(GU_MODEL,&identity);sceGuAmbientColor(0xffffffff);sceGuTexMode(GU_PSM_8888,5,0,1);sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);sceGuTexFilter(GU_NEAREST_MIPMAP_LINEAR,GU_NEAREST);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexScale(1,1);sceGuTexOffset(0,0);sceGuTexLevelMode(GU_TEXTURE_AUTO,1.f);
 if(fleet_ge_active_frame)fleet_ge_previous_depth();
 if(fleet_ge_active_frame)fleet_ge_batch(fallback);
 if(authored)station_ge_authored_batch(fallback);
 int previous=-1;fleet_ge_bound=fleet_ge_level=-1;unsigned command_limit=fleet_ge_active_frame?480000:230000;
 for(int i=drawcount-1;i>=0;i--){DrawTri*t=&drawlist[i];if(fallback[i])continue;if(sceGuCheckList()>command_limit){fallback[i]=1;continue;}
  if(fleet_ge_active_frame&&t->material>=0&&(t->material==0||fleet_ge_material(t->material))){if(t->material==0)fleet_ge_draw_flat(t);else fleet_ge_draw(t);previous=-1;continue;}
  if(fleet_ge_active_frame&&encounter_ge_legacy_draw(t)){previous=-1;continue;}
  if(t->material>=0){fallback[i]=1;continue;}int id=(-t->material-1)/6,face=(-t->material-1)%6;const MegaBlock*b=&mc_draw_city->b[id];station_art_face_prepare(b,face);
  if(sceGuCheckList()>command_limit){fallback[i]=1;continue;}fleet_ge_bound=fleet_ge_level=-1;
  if(!station_art_face.active||station_art_face.ad){fallback[i]=1;continue;}
  int far=t->depth>9000,key=station_ge_slot(id,face,far);if(key!=previous){sceGuEnable(GU_TEXTURE_2D);sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);sceGuTexMode(GU_PSM_8888,5,0,1);sceGuTexFilter(GU_NEAREST_MIPMAP_LINEAR,GU_NEAREST);sceGuTexLevelMode(GU_TEXTURE_AUTO,1.f);unsigned*texture=station_ge_texture(id,face,far),*mip_data=station_ge_mip_prepare(face,station_art_face.role);sceGuTexImage(0,32,32,32,texture);for(int level=1;level<=5;level++){int size=32>>level;sceGuTexImage(level,size,size,16,mip_data+station_ge_mip_offsets[level-1]);}sceGuTexFlush();previous=key;}
  StationGeVertex*v=sceGuGetMemory(3*sizeof(*v));int ua=face<4?0:2,va=face==2||face==3?2:1;float offsets[3]={mc_eye.x-b->c.x,mc_eye.y-b->c.y,mc_eye.z-b->c.z};
  for(int k=0;k<3;k++){Point p=t->p[k];float rx=(p.x-proj_ox)/proj_focal,ry=(proj_oy-p.y)/proj_focal,u=offsets[ua]+p.z*(mc_axis[ua].x*rx+mc_axis[ua].y*ry+mc_axis[ua].z),vv=offsets[va]+p.z*(mc_axis[va].x*rx+mc_axis[va].y*ry+mc_axis[va].z),tu=(u*station_art_face.us+1)*.5f,tv=(1-vv*station_art_face.vs)*.5f;
   if(station_art_face.ring){float ca=station_art_face.ca,sa=station_art_face.sa,inv=station_art_face.inv;tu=.5f+(-sa*u+ca*vv)*inv*1.68f;tv=1-((ca*u+sa*vv)*inv+.208f)*3.135f;}if(station_art_face.swap){float q=tu;tu=tv;tv=q;}
   v[k]=(StationGeVertex){tu,tv,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};
  }sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,v);station_ge_triangles++;
 }
 sceGuTexSync();sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top,STRIDE,(void*)(0x04000000u+offset),0,top,STRIDE,fb);unsigned list_size=sceGuFinish();if(list_size>station_ge_list_peak)station_ge_list_peak=list_size;
 /* CPU visibility writes a separate RAM surface; overlap it with GPU drawing
  * and framebuffer transfer, then synchronize before touching colour pixels. */
 uint64_t depth_stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
 for(int i=drawcount-1;i>=0;i--)if(fallback[i]!=1&&(!station_ge_predepth||drawlist[i].material>=0))station_ge_depth_triangle(&drawlist[i]);
 if(render_profile_enabled)station_ge_profile_us[2]+=sceKernelGetSystemTimeWide()-depth_stamp;
 uint64_t wait_stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
 int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,bytes);
 if(render_profile_enabled){station_ge_profile_us[1]+=sceKernelGetSystemTimeWide()-gpu_stamp;station_ge_profile_us[4]+=sceKernelGetSystemTimeWide()-wait_stamp;}
 if(result<0){station_ge_failed=1;return 0;}
 uint64_t fallback_stamp=render_profile_enabled?sceKernelGetSystemTimeWide():0;
 for(int i=drawcount-1;i>=0;i--)if(fallback[i]==1)triangle(&drawlist[i]);if(render_profile_enabled)station_ge_profile_us[3]+=sceKernelGetSystemTimeWide()-fallback_stamp;if(authored)station_ge_calls++;if(fleet_ge_active_frame)fleet_ge_calls++;return 1;
}
