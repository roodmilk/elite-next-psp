/* Crisp nearest presentation: no bilinear scene reconstruction.
 * Race-only mixed-resolution presentation. Simulation and depth remain CPU
 * owned. GE performs cache-friendly narrow textured strips: the scene, then native HUD.
 * No GPU depth readback, new heap, lost racers or frame skipping. */
typedef struct {float u,v,x,y,z;} NeonPresentVertex;
static int neon_hud_cached=1;
static int neon_present_sliced=1;
_Static_assert(sizeof(race_ge_list)>=120*sizeof(NeonPresentVertex)+1024,"GE strips plus state must fit the bounded ERL command list");
static unsigned *neon_present_target;
static unsigned neon_present_frames;
static unsigned neon_present_wall_frames;
static int neon_present_image,neon_present_enabled=1;
static int neon_present_begin(void){
 if(!neon_present_enabled||paused||game.erl.paused||((unsigned)fb&0xfe000000u)!=0x44000000u)return 0;
 if(!race_ge_ready)race_ge_ready=sceGuInit()==0?1:-1;if(race_ge_ready!=1)return 0;
 /* Stable sharp320x180 scene while profiling the physical PSP bottleneck.
  * Native480x272 HUD remains untouched; no resolution pops near walls. */
 neon_present_target=fb;neon_present_image=0;neon_render_width=320;neon_render_height=180;return 1;
}
static void neon_present_hud(void){
 /* The third VRAM plane is scratch only, never scanned out by the LCD. */
 fb=(unsigned *)((neon_hud_cached?0x04000000u:0x44000000u)+STRIDE*H*8);memset(fb,0,STRIDE*H*4);
}
/* PSP GE cache-friendly copy. Scene strips consume16 source texels (24
 * destination pixels at320x180); HUD strips are32 pixels. Endpoint ratios
 * are exact for320/384/480 sources, keeping nearest sampling continuous.
 * PSPSDK gu/blit advancedBlit uses the same narrow-sprite strategy. */
static void __attribute__((noinline,optimize("Os"))) neon_present_blit(int width,int height,int y,int output_height,int strip){
 if(!neon_present_sliced)strip=width;
 int count=(width+strip-1)/strip;NeonPresentVertex *vertices=sceGuGetMemory(count*2*sizeof(*vertices));
 for(int i=0;i<count;i++){int x=i*strip,end=x+strip;if(end>width)end=width;
  vertices[i*2]=(NeonPresentVertex){x,0,x*(W/(float)width),y,0};
  vertices[i*2+1]=(NeonPresentVertex){end,height,end*(W/(float)width),y+output_height,0};
 }
 sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D,count*2,0,vertices);
}
static void neon_present_finish(void){
 if(!neon_present_target)return;
 unsigned offset=(unsigned)neon_present_target&0x1fffffu;
 int width=neon_render_width,height=neon_render_height;
 for(int y=0;y<height;y++)dc_open_source[y*STRIDE+width]=dc_open_source[y*STRIDE+width-1];memcpy(dc_open_source+height*STRIDE,dc_open_source+(height-1)*STRIDE,(width+1)*4);
 if(neon_hud_cached)sceKernelDcacheWritebackRange((void *)(0x04000000u+STRIDE*H*8),STRIDE*H*4);
 sceKernelDcacheWritebackRange(dc_open_source,sizeof(dc_open_source));
 sceGuStart(GU_DIRECT,race_ge_list);sceGuDrawBuffer(GU_PSM_8888,(void *)offset,STRIDE);
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,0,W,H);
 sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_DEPTH_TEST);sceGuDepthMask(GU_TRUE);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_FOG);sceGuDisable(GU_DITHER);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_BLEND);
 sceGuEnable(GU_TEXTURE_2D);sceGuTexMode(GU_PSM_8888,0,0,0);sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexScale(1,1);sceGuTexOffset(0,0);
 sceGuTexFilter(GU_NEAREST,GU_NEAREST);sceGuTexImage(0,512,256,STRIDE,dc_open_source);sceGuTexFlush();
 neon_present_blit(width,height,0,H,16);
 sceGuTexFilter(GU_NEAREST,GU_NEAREST);sceGuTexImage(0,512,256,STRIDE,(void *)(0x44000000u+STRIDE*H*8));sceGuTexFlush();
 sceGuEnable(GU_BLEND);sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);
 neon_present_blit(W,256,0,256,32);
 /* Split the bottom 16 rows rather than declaring a texture past VRAM. */
 sceGuTexImage(0,512,16,STRIDE,(void *)(0x44000000u+STRIDE*H*8+256*STRIDE*4));sceGuTexFlush();
 neon_present_blit(W,16,256,16,32);
 sceGuFinish();sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);fb=neon_present_target;neon_present_target=0;neon_present_image=1;
 neon_render_width=W;neon_render_height=H;neon_present_frames++;neon_present_wall_frames+=width==320;
}
static void neon_present_readback(void){
 if(!neon_present_image)return;neon_present_image=0;
 sceGuStart(GU_DIRECT,race_ge_list);sceGuCopyImage(GU_PSM_8888,0,0,W,H,STRIDE,fb,0,0,STRIDE,dc_open_source);sceGuTexSync();sceGuFinish();sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);
 sceKernelDcacheInvalidateRange(dc_open_source,sizeof(dc_open_source));for(int y=0;y<H;y++)memcpy(fb+y*STRIDE,dc_open_source+y*STRIDE,W*4);
}
