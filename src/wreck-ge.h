/* PSP GU acceleration for the abandoned freighter's authored pixel panels. */
typedef struct {float u,v,x,y,z;} WreckGeVertex;
typedef struct {unsigned color;unsigned char tile,ready;} WreckGeKey;
static unsigned *wreck_ge_pixels;static WreckGeKey wreck_ge_keys[96];static int wreck_ge_ready,wreck_ge_failed,wreck_ge_next;
static unsigned wreck_ge_calls,wreck_ge_triangles,wreck_ge_list_peak;
static int wreck_ge_view_ready(void){
 if(!flight_cached_frame||!ion_gu_ready||page!=FLIGHT||game.planet>=0||race_ready(&game)||high_contrast||render_reference)return 0;
 /* Discovery ordering varies as systems grow. Find the authored wreck by its
  * data entry instead of assuming it is always discovery number two. */
 for(int i=0;i<discovery_count();i++)if(discovery_entry(i)==61){Vec3 d=sub(discovery_position(i),game.pos);if(dot(d,d)<15000.f*15000.f)return 1;}
 return 0;
}
static int wreck_ge_prepare(void){if(wreck_ge_failed)return 0;if(wreck_ge_ready)return 1;wreck_ge_pixels=memalign(16,96*4096);if(!wreck_ge_pixels){wreck_ge_failed=1;return 0;}memset(wreck_ge_keys,0,sizeof(wreck_ge_keys));wreck_ge_ready=1;return 1;}
static unsigned*wreck_ge_texture(int tile,unsigned color){
 int slot=-1;for(int i=0;i<96;i++)if(wreck_ge_keys[i].ready&&wreck_ge_keys[i].tile==tile&&wreck_ge_keys[i].color==color){slot=i;break;}
 if(slot<0){slot=wreck_ge_next++%96;WreckGeKey*k=&wreck_ge_keys[slot];k->ready=1;k->tile=tile;k->color=color;unsigned*texture=wreck_ge_pixels+slot*1024,palette[16];for(int i=0;i<16;i++)palette[i]=0xff000000u|freight_art_ink(color,i);for(int y=0;y<32;y++)for(int x=0;x<32;x++){int packed=wreck_art_tiles[tile][(y*32+x)>>1],ci=(packed>>(((y*32+x)&1)*4))&15,at=(y>>3)*256+(x>>2)*32+(y&7)*4+(x&3);texture[at]=palette[ci];}sceKernelDcacheWritebackRange(texture,4096);}
 return wreck_ge_pixels+slot*1024;
}
static int wreck_ge_flush(void){
 if(fleet_ge_active_frame)return 0; /* Shared pass handles every opaque family together. */
 if(!wreck_ge_active_frame||!flight_cached_frame||!ion_gu_ready||fb!=dc_open_source||game.planet>=0||race_ready(&game)||high_contrast||drawcount<1)return 0;
 int authored=0;for(int i=0;i<drawcount;i++)authored+=drawlist[i].material>=5100&&drawlist[i].material<5172;if(!authored||!wreck_ge_prepare())return 0;
 unsigned offset=2*STRIDE*H*4,depth_offset=3*STRIDE*H*4,bytes=STRIDE*H*4;int top=view_top(),bot=view_bot();
 sceKernelDcacheWritebackInvalidateRange(fb,bytes);sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)offset,STRIDE);sceGuDepthBuffer((void*)depth_offset,STRIDE);sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top,STRIDE,fb,0,top,STRIDE,(void*)(0x04000000u+offset));sceGuTexSync();
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,top,W,bot);sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_BLEND);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);sceGuEnable(GU_CLIP_PLANES);sceGuEnable(GU_TEXTURE_2D);sceGuEnable(GU_DEPTH_TEST);sceGuDepthFunc(GU_LEQUAL);sceGuDepthMask(GU_FALSE);sceGuDepthRange(0,65535);sceGuClearDepth(65535);sceGuClear(GU_DEPTH_BUFFER_BIT);
 float near=50;ScePspFMatrix4 identity={0},projection={0};identity.x.x=identity.y.y=identity.z.z=identity.w.w=1;projection.x.x=2*proj_focal/W;projection.y.y=2*proj_focal/H;projection.z.x=2.f*proj_ox/W-1;projection.z.y=1-2.f*proj_oy/H;projection.z.z=-1;projection.w.z=2*near;projection.z.w=1;sceGuSetMatrix(GU_PROJECTION,&projection);sceGuSetMatrix(GU_VIEW,&identity);sceGuSetMatrix(GU_MODEL,&identity);sceGuAmbientColor(0xffffffff);sceGuTexMode(GU_PSM_8888,0,0,1);sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);sceGuTexFilter(GU_NEAREST,GU_NEAREST);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexScale(1,1);sceGuTexOffset(0,0);
 int previous=-1;for(int i=drawcount-1;i>=0;i--){DrawTri*t=&drawlist[i];if(t->material<5100||t->material>=5172)continue;int m=t->material-5100,tile=m/6,second=m&1,key=tile*4096+(t->color&0xfff);if(key!=previous){sceGuTexImage(0,32,32,32,wreck_ge_texture(tile,t->color));sceGuTexFlush();previous=key;}WreckGeVertex*v=sceGuGetMemory(3*sizeof(*v));static const float first_uv[6]={0,0,1,0,1,1},second_uv[6]={0,0,1,1,0,1};const float*uv=second?second_uv:first_uv;for(int k=0;k<3;k++){Point p=t->p[k];v[k]=(WreckGeVertex){uv[k*2],uv[k*2+1],(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,v);wreck_ge_triangles++;}
 sceGuTexSync();sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top,STRIDE,(void*)(0x04000000u+offset),0,top,STRIDE,fb);unsigned list_size=sceGuFinish();if(list_size>wreck_ge_list_peak)wreck_ge_list_peak=list_size;int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,bytes);if(result<0){wreck_ge_failed=1;return 0;}
 if(surface_depth_on){for(int i=drawcount-1;i>=0;i--)if(drawlist[i].material<5100||drawlist[i].material>=5172)triangle(&drawlist[i]);}else for(int i=0;i<drawcount;i++)if(drawlist[i].material<5100||drawlist[i].material>=5172)triangle(&drawlist[i]);wreck_ge_calls++;return 1;
}
