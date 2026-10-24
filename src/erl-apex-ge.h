/* PSP GE acceleration for the translucent deck only. The live CPU depth
 * surface is exchanged at the clipped viewport boundary; all other rendering
 * remains on the established path. Uses the already flushed triangle arena. */
typedef struct {unsigned colour;float x,y,z;} RaceDeckVertex;
static unsigned race_ge_list[1024] __attribute__((aligned(16)));
static int race_ge_ready,race_deck_gpu,race_ge_vertices,race_ge_overflow;
static RaceDeckVertex *race_ge_vertex;
static void race_ge_begin(void){
#ifndef ELITE_EXPERIMENTAL_RACE_GE
 race_deck_gpu=0;race_ge_vertices=race_ge_overflow=0;return;
#else
 if(!race_ge_ready)race_ge_ready=sceGuInit()==0?1:-1;
 race_deck_gpu=race_ge_ready==1;race_ge_vertices=race_ge_overflow=0;
 race_ge_vertex=(RaceDeckVertex *)render_scratch.deck;
#endif
}
static RaceDeckVertex race_ge_mix(RaceDeckVertex a,RaceDeckVertex b,float t){RaceDeckVertex v={0,a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,a.z+(b.z-a.z)*t};unsigned alpha=(unsigned)(((a.colour>>24)*(1-t)+(b.colour>>24)*t));v.colour=(RGB(64,32,72)&0xffffff)|(alpha<<24);return v;}
static void race_ge_triangle(Vec3 a,Vec3 b,Vec3 c,unsigned aa,unsigned ab,unsigned ac){
 Vec3 input[3]={camera(&game,a),camera(&game,b),camera(&game,c)},near[4];unsigned alpha[3]={aa,ab,ac},opacity[4];int n=0;
 for(int i=0;i<3;i++){int j=(i+1)%3;if(input[i].z>=15){near[n]=input[i];opacity[n++]=alpha[i];}if((input[i].z>=15)!=(input[j].z>=15)){float t=(15-input[i].z)/(input[j].z-input[i].z);near[n]=add(input[i],mul(sub(input[j],input[i]),t));opacity[n++]=alpha[i]*(1-t)+alpha[j]*t;}}
 if(n<3)return;RaceDeckVertex poly[12],next[12];for(int i=0;i<n;i++){Point p=project(near[i]);poly[i]=(RaceDeckVertex){(RGB(64,32,72)&0xffffff)|(opacity[i]<<24),p.x,p.y,mega_encode_depth(near[i].z)};}
 float edge[4]={clipx0,clipx1,clipy0<0?surface_y_min:clipy0,clipy1<0?surface_y_max+1:clipy1};
 for(int side=0;side<4&&n;side++){int count=0;for(int i=0;i<n;i++){RaceDeckVertex p=poly[i],q=poly[(i+1)%n];float x=side<2?p.x:p.y,y=side<2?q.x:q.y;int inside=side%2?x<=edge[side]:x>=edge[side],other=side%2?y<=edge[side]:y>=edge[side];if(inside)next[count++]=p;if(inside!=other)next[count++]=race_ge_mix(p,q,(edge[side]-x)/(y-x));}n=count;memcpy(poly,next,n*sizeof(*poly));}
 for(int i=1;i+1<n;i++){if(race_ge_vertices+3>(int)(sizeof(render_scratch.deck)/sizeof(RaceDeckVertex))){race_ge_overflow=1;return;}race_ge_vertex[race_ge_vertices++]=poly[0];race_ge_vertex[race_ge_vertices++]=poly[i];race_ge_vertex[race_ge_vertices++]=poly[i+1];}
}
static void race_ge_end(void){
 if(!race_deck_gpu)return;
 int top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max+1:clipy1;
 unsigned address=(unsigned)fb,offset=address&0x1fffff;int staged=(address&0xfe000000)!=0x44000000;
 if(staged)offset=0;
 for(int y=top;y<bottom;y++)memcpy(dc_open_source+y*STRIDE+clipx0,fb+y*STRIDE+clipx0,(clipx1-clipx0)*sizeof(unsigned));
 sceKernelDcacheWritebackRange(dc_open_source,sizeof(dc_open_source));
 unsigned short *depth=(unsigned short *)(0x44000000+STRIDE*H*8);
 sceKernelDcacheWritebackRange(surface_depth,sizeof(surface_depth));
 sceKernelDcacheWritebackRange(race_ge_vertex,race_ge_vertices*sizeof(RaceDeckVertex));
 sceGuStart(GU_DIRECT,race_ge_list);sceGuDrawBuffer(GU_PSM_8888,(void *)offset,STRIDE);sceGuDepthBuffer((void *)(STRIDE*H*8),STRIDE);
 sceGuCopyImage(GU_PSM_8888,clipx0,top,clipx1-clipx0,bottom-top,STRIDE,dc_open_source,clipx0,top,STRIDE,(void *)(0x44000000+offset));sceGuTexSync();
 /* Use a GE transfer for ownership as well as bytes. Direct CPU writes to
  * depth VRAM can leave an emulator's cached render target unchanged. */
 sceGuCopyImage(GU_PSM_5650,clipx0,top,clipx1-clipx0,bottom-top,W,surface_depth,clipx0,top,STRIDE,depth);sceGuTexSync();
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(clipx0,top,clipx1,bottom);sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_TEXTURE_2D);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_DITHER);sceGuShadeModel(GU_SMOOTH);
 sceGuEnable(GU_DEPTH_TEST);sceGuDepthFunc(GU_LEQUAL);sceGuDepthMask(GU_FALSE);sceGuDepthRange(0,65535);sceGuEnable(GU_BLEND);sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);
 ScePspFMatrix4 identity={.x={1,0,0,0},.y={0,1,0,0},.z={0,0,1,0},.w={0,0,0,1}};
 ScePspFMatrix4 ortho={.x={2.f/W,0,0,0},.y={0,-2.f/H,0,0},.z={0,0,2.f/65535,0},.w={-1,1,-1,1}};
 sceGuSetMatrix(GU_PROJECTION,&ortho);sceGuSetMatrix(GU_VIEW,&identity);sceGuSetMatrix(GU_MODEL,&identity);
 if(race_ge_vertices)sceGuDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,race_ge_vertices,0,race_ge_vertex);
 sceGuCopyImage(GU_PSM_5650,clipx0,top,clipx1-clipx0,bottom-top,STRIDE,(void *)depth,clipx0,top,W,surface_depth);sceGuTexSync();
 sceGuCopyImage(GU_PSM_8888,clipx0,top,clipx1-clipx0,bottom-top,STRIDE,(void *)(0x44000000+offset),clipx0,top,STRIDE,dc_open_source);sceGuTexSync();
 sceGuFinish();sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(surface_depth,sizeof(surface_depth));sceKernelDcacheInvalidateRange(dc_open_source,sizeof(dc_open_source));
 for(int y=top;y<bottom;y++)memcpy(fb+y*STRIDE+clipx0,dc_open_source+y*STRIDE+clipx0,(clipx1-clipx0)*sizeof(unsigned));

}
