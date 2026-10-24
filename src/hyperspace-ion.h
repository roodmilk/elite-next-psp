/* Approved Ion Cathedral, real-time rendering only; never changes gameplay state. */
static inline __attribute__((always_inline)) float ion_smooth(float t){if(t<0)t=0;if(t>1)t=1;return t*t*(3-2*t);}
static float journey_envelope=1;
static float ion_tail=-100,ion_last_jump=-100;
static int ion_in_jump,ion_charge_cached;
static float ion_seconds(void){
 if(game.dead||game.docked||thargoid_active){ion_in_jump=0;ion_tail=-100;return -1;}
 if(game.jump>0){ion_in_jump=1;ion_last_jump=game.time;float s=10-game.jump;return s<0?0:s;}
 if(ion_in_jump){ion_in_jump=0;ion_tail=game.time;}
 if(game.time>=ion_tail&&game.time-ion_tail<(journey_variant?1.4f:.65f))return 10+game.time-ion_tail;
 if(route_target_ready()&&nav_route_selected()&&game.boost&&route_boost_charge>0)return route_boost_charge;
 return -1;
}
static const unsigned char ion_art_nibbles[]={69,17,17,17,17,17,17,17,69,17,17,17,17,17,17,17,69,68,68,68,68,68,68,17,69,50,51,51,51,51,35,17,69,50,51,51,51,99,35,17,69,50,51,51,51,100,35,17,69,50,51,51,51,100,35,17,69,50,51,51,51,100,35,17,69,50,51,51,51,100,35,17,69,50,17,17,17,100,35,17,69,50,33,34,34,100,35,17,69,50,65,68,68,100,35,17,69,50,33,34,34,100,35,17,69,50,33,34,34,100,35,17,69,50,33,0,0,100,35,17,69,50,33,34,34,100,35,17,69,50,33,34,34,100,35,17,69,50,33,0,0,100,35,17,69,50,33,34,34,100,35,17,69,50,33,34,34,100,35,17,69,50,33,0,0,100,35,17,69,50,33,34,34,100,35,17,69,50,33,34,34,100,35,17,69,50,33,0,0,100,35,17,69,50,33,34,34,100,35,17,69,50,17,17,17,97,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,115,119,119,51,35,17,69,50,51,51,51,51,35,17,69,50,115,119,119,51,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,17,17,17,17,35,17,69,50,33,34,34,18,35,17,69,50,65,68,68,18,35,17,69,50,33,34,34,18,35,17,69,50,33,34,34,18,35,17,69,50,33,0,0,18,35,17,69,50,33,34,34,18,35,17,69,50,33,34,34,18,35,17,69,50,33,0,0,18,35,17,69,50,33,34,34,18,35,17,69,50,33,34,34,18,35,17,69,50,33,0,0,18,35,17,69,50,102,34,34,18,35,17,69,50,102,102,34,18,35,17,69,50,33,102,6,18,35,17,69,50,33,34,38,18,35,17,69,50,17,17,17,17,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,115,119,119,51,35,17,69,50,51,51,51,51,35,17,69,50,115,119,119,51,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,50,51,51,51,51,35,17,69,0,0,0,0,0,0,17,69,17,17,17,17,17,17,17,69,17,17,17,17,17,17,17};
static float __attribute__((noinline,optimize(ELITE_VISUAL_OPT))) ion_speed_original(float s){if(s<5)return 1.6f+1.2f*powf(s/5,3)+1.8f*ion_smooth((s-4.45f)/.25f);if(s<7)return 4.6f;return 4.6f-4.48f*ion_smooth((s-7)/3);}
static float __attribute__((noinline,optimize(ELITE_VISUAL_OPT))) ion_distance_original(float s){float d;if(s<5){d=1.6f*s+.0024f*s*s*s*s;float t=(s-4.45f)/.25f;if(t>1)d+=1.8f*(s-4.575f);else if(t>0)d+=.45f*(t*t*t-.5f*t*t*t*t);}else if(s<7)d=10.265f+4.6f*(s-5);else{float q=s-7;d=19.465f+4.6f*q-4.48f*(q*q*q/9-q*q*q*q/54);}return d+.18f*(1+tanhf((s-5.85f)/.25f))+.23f*(1+tanhf((s-6.65f)/.23f));}
static float __attribute__((noinline,optimize(ELITE_VISUAL_OPT))) ion_burst_original(float s){float a=(s-4.65f)/.17f,b=(s-5.85f)/.27f,c=(s-6.65f)/.25f;return 1.22f*expf(-a*a)+.60f*expf(-b*b)+.8f*expf(-c*c);}
static float journey_step_integral(float s,float begin,float duration){float t=(s-begin)/duration;if(t<=0)return 0;if(t>=1)return s-begin-duration*.5f;return duration*(t*t*t-.5f*t*t*t*t);}
static const float journey_crest_time[6]={5.62f,6.36f,7.19f,8.02f,8.70f,9.30f};
static const float journey_crest_gain[6]={.78f,.46f,.95f,.57f,.86f,.64f};
static float journey_crest(float s){
 if(s<journey_crest_time[0]||s>journey_crest_time[5]+.39f)return 0;
 for(int i=0;i<6;i++){float age=s-journey_crest_time[i];if(age>=0&&age<.39f)return journey_crest_gain[i]*(ion_smooth(age/.13f)-ion_smooth((age-.13f)/.26f));}return 0;
}
static float journey_crest_distance(float s){
 float sum=0;for(int i=0;i<6;i++){float age=s-journey_crest_time[i];if(age<=0)break;if(age>=.39f)sum+=journey_crest_gain[i]*.195f;else sum+=journey_crest_gain[i]*(journey_step_integral(s,journey_crest_time[i],.13f)-journey_step_integral(s,journey_crest_time[i]+.13f,.26f));}return sum;
}
static float journey_exit_speed(float s){return 9.f*(ion_smooth((s-9.52f)/.38f)-ion_smooth((s-10.08f)/.87f));}
static float journey_exit_distance(float s){return 9.f*(journey_step_integral(s,9.52f,.38f)-journey_step_integral(s,10.08f,.87f));}
static float journey_exit_open(float s){return ion_smooth((s-10)/.85f);}
static unsigned journey_exit_tint(float seconds){
 int system=seconds>=10?game.system:game.destination;if(system<0||system>=256)system=game.system;
 unsigned sun=sun_light_colour(space_sky_seed_for(system));float t=ion_smooth((seconds-9.65f)/.5f)*.60f;
 return RGB((int)(141*(1-t)+(sun&255)*t),(int)(225*(1-t)+((sun>>8)&255)*t),(int)(255*(1-t)+((sun>>16)&255)*t));
}
static float ion_speed(float s){
 if(!journey_variant)return ion_speed_original(s);
 if(s<5)return 1.6f+1.2f*powf(s/5,3)+2.f*ion_smooth((s-4.45f)/.25f);
 return 4.8f*(1-ion_smooth((s-9.65f)/1.75f))+4.2f*(ion_smooth((s-5)/.4f)-ion_smooth((s-9.5f)/.5f))+2.4f*journey_crest(s)+journey_exit_speed(s);
}
static float ion_distance(float s){
 if(!journey_variant)return ion_distance_original(s);
 float q=fminf(s,5),d=1.6f*q+.0024f*q*q*q*q+2.f*journey_step_integral(q,4.45f,.25f);
 if(s>5)d+=4.8f*(s-5)-4.8f*journey_step_integral(s,9.65f,1.75f);
 d+=4.2f*(journey_step_integral(s,5,.4f)-journey_step_integral(s,9.5f,.5f))+2.4f*journey_crest_distance(s)+journey_exit_distance(s);
 return d+.12f*(1+tanhf((s-6.15f)/.34f))+.16f*(1+tanhf((s-8.1f)/.38f));
}
static float ion_burst(float s){
 if(!journey_variant)return ion_burst_original(s);
 float a=(s-4.65f)/.17f,b=(s-6.15f)/.34f,c=(s-8.1f)/.38f,d=(s-9.88f)/.22f;
 return fminf(1.22f,1.12f*expf(-a*a)+.40f*expf(-b*b)+.48f*expf(-c*c)+.50f*expf(-d*d)+.9f*journey_crest(s)+.42f*expf(-((s-9.98f)/.15f)*((s-9.98f)/.15f)));
}
typedef struct{unsigned short u,z;float r;} JumpPolar;
static unsigned *jump_polar;
static unsigned char *jump_panel_art;
static unsigned *jump_scene_pixels;
/* Dense shared polar samples: exact native pixels, no spatial downsampling. */
typedef struct {int red,green,blue;unsigned opaque;} IonColor;
typedef struct {int z,v,inv,gain,red,green,blue,bleed;} IonRadius;
typedef struct {float u,v;unsigned color;float x,y,z;} IonVertex;
static unsigned*ion_gu_list;static int ion_gu_ready;
static float ion_sin[128],ion_cos[128],ion_ring_cos[65],ion_ring_sin[65],ion_ring_radius[32],ion_streak_cos[112],ion_streak_sin[112],ion_hoop_cos[41],ion_hoop_sin[41];static int ion_ring_index[32];
static void ion_basis_init(void){
 for(int i=0;i<128;i++){ion_sin[i]=sinf(i*.0490874f);ion_cos[i]=cosf(i*.0490874f);}
 for(int i=0;i<=64;i++){float a=i*6.283185307f/64-3.14159265f;ion_ring_cos[i]=cosf(a);ion_ring_sin[i]=sinf(a);}
 for(int i=0;i<32;i++){ion_ring_radius[i]=expf(i*(6.2402758f/31))-1;ion_ring_index[i]=(int)(ion_ring_radius[i]*(256.f/260));if(ion_ring_index[i]>511)ion_ring_index[i]=511;}
 for(int i=0;i<112;i++){ion_streak_cos[i]=cosf(i*2.39996f);ion_streak_sin[i]=sinf(i*2.39996f);}
 for(int i=0;i<=40;i++){ion_hoop_cos[i]=cosf(i*6.2831853f/40);ion_hoop_sin[i]=sinf(i*6.2831853f/40);}
}

static void ion_gu_mesh(const IonRadius*radial,int top,int bot,int bleed_scale,int bleed){
 for(int y=top;y<=bot;y+=16){int end=y+16;if(end>bot+1)end=bot+1;
  IonVertex*v=sceGuGetMemory(62*sizeof(*v));int n=0;
  for(int x=0;x<=W;x+=16)for(int row=0;row<2;row++){
   float yy=row?end:y,dx=x-240,dy=(yy-110)*1.55f;int ri=(int)(sqrtf(dx*dx+dy*dy)*(256.f/260));if(ri>511)ri=511;
   const IonRadius*q=&radial[ri];float sx=240+dx*q->inv*(1.f/65536),sy=110+(yy-110)*q->inv*(1.f/65536);
   if(sx<0)sx=0;if(sx>W-1)sx=W-1;if(sy<top)sy=top;if(sy>bot)sy=bot;
   if(bleed){sx=240+(sx-240)*bleed_scale*(1.f/256);sy=110+(sy-110)*bleed_scale*(1.f/256);if(sy<top)sy=top;if(sy>bot)sy=bot;}
   unsigned alpha=bleed?(unsigned)(q->bleed*255/256):255;v[n++]=(IonVertex){sx,sy,0xffffffu|(alpha<<24),(float)x,yy,0};
  }
  sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,n,0,v);
 }
}
#include "vortex-cloud-art.h"
static float vortex_twist[32],vortex_scale[32],vortex_x[32],vortex_y[32],vortex_active,vortex_flash;
static int vortex_warmth[128];
static void vortex_prepare(float seconds,float distance){
 float envelope=journey_variant==1?ion_smooth((seconds-4.96f)/.48f)*(1-ion_smooth((seconds-9.66f)/.30f)):0;
 vortex_active=envelope;vortex_flash=envelope*journey_crest(seconds);
 for(int k=0;k<32;k++){
  float r=ion_ring_radius[k],depth=k*.2013f,edge=r/(r+125.f);
  if(envelope<=0){vortex_twist[k]=vortex_x[k]=vortex_y[k]=0;vortex_scale[k]=1;continue;}
  vortex_twist[k]=envelope*(sinf(depth*1.65f-distance*.27f)*6+cosf(depth*.84f+distance*.19f)*3);
  vortex_scale[k]=1+envelope*.065f*sinf(depth*1.4f-distance*.65f);
  vortex_x[k]=envelope*edge*sinf(depth*.62f+distance*.21f)*13;
  vortex_y[k]=envelope*edge*cosf(depth*.71f+distance*.18f)*6;
 }
 int phase=(int)(distance*3);
 for(int i=0;i<128;i++){float a=ion_sin[(i+phase)&127];vortex_warmth[i]=a>0?(int)(a*a*205):0;}
}
typedef struct {unsigned color;float x,y,z;} IonSkyVertex;
static int ion_sky_gpu(int left,int right,int top,int bot,Vec3 rightward,Vec3 upward,Vec3 ahead){
 if(!ion_gu_ready||fb!=dc_open_source||left!=0||right!=W)return 0;
 unsigned bytes=STRIDE*H*4,offset=2*STRIDE*H*4;sceKernelDcacheWritebackInvalidateRange(fb,bytes);
 sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)offset,STRIDE);sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(left,top,right,bot);
 sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_TEXTURE_2D);sceGuDisable(GU_BLEND);sceGuDisable(GU_DEPTH_TEST);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);sceGuShadeModel(GU_SMOOTH);
 unsigned rows[2][W/8+2];int columns=(right-left+7)/8;
 for(int row=0,y0=top;y0<bot;row++,y0+=8){int y1=y0+8;if(y1>bot)y1=bot;
  for(int side=row?1:0;side<2;side++){int y=side?y1:y0;Vec3 base=add(mul(ahead,240),mul(upward,(float)(proj_oy-y)));
   for(int col=0;col<=columns;col++){int x=left+col*8;if(x>right)x=right;rows[(row+side)&1][col]=soft_sky_sample(add(base,mul(rightward,(float)(x-proj_ox))));}}
  IonSkyVertex*v=sceGuGetMemory((columns+1)*2*sizeof(*v));int n=0;for(int col=0;col<=columns;col++){int x=left+col*8;if(x>right)x=right;v[n++]=(IonSkyVertex){rows[row&1][col],x,y0,0};v[n++]=(IonSkyVertex){rows[(row+1)&1][col],x,y1,0};}
  sceGuDrawArray(GU_TRIANGLE_STRIP,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,n,0,v);
 }
 sceGuTexSync();sceGuCopyImage(GU_PSM_8888,left,top,right-left,bot-top,STRIDE,(void*)(0x04000000u+offset),left,top,STRIDE,fb);sceGuFinish();int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,bytes);if(result<0){ion_gu_ready=0;return 0;}return 1;
}
typedef struct {unsigned color;float x,y,z;} IonGlowVertex;
static int ion_gu_tunnel(const IonRadius*radial,const unsigned base[64][128],int top,int bot,int angle_shift,int scroll,int partial,int bleed_scale){
 if(!ion_gu_ready||fb!=dc_open_source)return 0;
 const unsigned offset=2*STRIDE*H*4;unsigned bytes=STRIDE*H*4;
 if(partial)sceKernelDcacheWritebackRange(jump_scene_pixels,STRIDE*H*4);sceKernelDcacheWritebackRange(base,64*128*4);sceKernelDcacheWritebackInvalidateRange(fb,bytes);
 sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)offset,STRIDE);
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,top,W,bot+1);
 sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_DEPTH_TEST);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);sceGuEnable(GU_TEXTURE_2D);sceGuShadeModel(GU_SMOOTH);
 sceGuTexMode(GU_PSM_8888,0,0,0);sceGuTexFilter(journey_variant?GU_LINEAR:GU_NEAREST,journey_variant?GU_LINEAR:GU_NEAREST);sceGuTexWrap(GU_REPEAT,GU_REPEAT);sceGuTexScale(1,1);sceGuTexOffset(0,0);sceGuAmbientColor(0xffffffff);
 if(partial){sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexImage(0,512,512,STRIDE,jump_scene_pixels);sceGuTexFlush();sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);sceGuDisable(GU_BLEND);ion_gu_mesh(radial,top,bot,bleed_scale,0);}
 sceGuTexWrap(GU_REPEAT,GU_REPEAT);sceGuTexImage(0,128,64,128,base);sceGuTexFlush();sceGuTexFunc(partial?GU_TFX_MODULATE:GU_TFX_ADD,partial?GU_TCC_RGBA:GU_TCC_RGB);
 if(partial){sceGuEnable(GU_BLEND);sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);}else sceGuDisable(GU_BLEND);
 const float*ca=ion_ring_cos,*sa=ion_ring_sin,*r=ion_ring_radius;const int*ri=ion_ring_index;
 for(int ring=0;ring<31;ring++){
  IonVertex*v=sceGuGetMemory(66*sizeof(*v));int n=0;for(int a=0;a<=32;a++)for(int row=0;row<2;row++){int k=ring+row;float depth=(radial[ri[k]].z/3.f-scroll)*(journey_variant==1?.24f:1.f),u=a*4.f+angle_shift+(journey_variant==2?depth*.055f*fminf(1,ion_ring_radius[k]/70.f):journey_variant==3?4.f*sinf(depth*.018f)*fminf(1,ion_ring_radius[k]/70.f):0)+vortex_twist[k];const IonRadius*q=&radial[ri[k]];unsigned c=partial?(0xffffffu|((unsigned)(q->gain*255/256)<<24)):RGB(q->red>255?255:q->red,q->green>255?255:q->green,q->blue>255?255:q->blue);v[n++]=(IonVertex){u,depth,c,240+ca[a*2]*r[k]*vortex_scale[k]+vortex_x[k],110+sa[a*2]*r[k]*vortex_scale[k]/1.55f+vortex_y[k],0};}
  sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,n,0,v);
 }
 if(partial){
  sceGuDisable(GU_TEXTURE_2D);sceGuEnable(GU_BLEND);sceGuBlendFunc(GU_ADD,GU_FIX,GU_FIX,0xffffffff,0xffffffff);
  for(int ring=0;ring<31;ring++){
   IonGlowVertex*v=sceGuGetMemory(66*sizeof(*v));int n=0;for(int a=0;a<=32;a++)for(int row=0;row<2;row++){int k=ring+row;const IonRadius*q=&radial[ri[k]];int rr=q->red*q->gain/256,gg=q->green*q->gain/256,bb=q->blue*q->gain/256;unsigned c=RGB(rr>255?255:rr,gg>255?255:gg,bb>255?255:bb);v[n++]=(IonGlowVertex){c,240+ca[a*2]*r[k]*vortex_scale[k]+vortex_x[k],110+sa[a*2]*r[k]*vortex_scale[k]/1.55f+vortex_y[k],0};}
   sceGuDrawArray(GU_TRIANGLE_STRIP,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,n,0,v);
  }
  sceGuEnable(GU_TEXTURE_2D);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexImage(0,512,512,STRIDE,jump_scene_pixels);sceGuTexFlush();sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_FIX,0,0xffffffff);ion_gu_mesh(radial,top,bot,bleed_scale,1);
 }
 sceGuTexSync();sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top+1,STRIDE,(void*)(0x04000000u+offset),0,top,STRIDE,fb);sceGuFinish();int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,bytes);
 if(result<0){ion_gu_ready=0;return 0;}return 1;
}
static int ion_gu_warp(const IonRadius*radial,int top,int bot,int bleed_scale){
 if(!ion_gu_ready||fb!=dc_open_source)return 0;
 /* Third VRAM plane is private scratch; display flipping remains CPU-owned. */
 const unsigned offset=2*STRIDE*H*4;unsigned bytes=STRIDE*H*4;
 sceKernelDcacheWritebackRange(jump_scene_pixels,STRIDE*H*4);sceKernelDcacheWritebackInvalidateRange(fb,bytes);
 sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)offset,STRIDE);
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,top,W,bot+1);
 sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_DEPTH_TEST);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);sceGuEnable(GU_TEXTURE_2D);sceGuShadeModel(GU_SMOOTH);
 sceGuTexMode(GU_PSM_8888,0,0,0);sceGuTexFilter(journey_variant?GU_LINEAR:GU_NEAREST,journey_variant?GU_LINEAR:GU_NEAREST);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexScale(1,1);sceGuTexOffset(0,0);sceGuAmbientColor(0xffffffff);
 sceGuTexImage(0,512,512,STRIDE,jump_scene_pixels);sceGuTexFlush();sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);sceGuDisable(GU_BLEND);ion_gu_mesh(radial,top,bot,bleed_scale,0);
 sceGuEnable(GU_BLEND);sceGuBlendFunc(GU_ADD,GU_FIX,GU_SRC_ALPHA,0xffffffff,0);
 for(int y=0;y<H;){int size=y+128<=H?128:16;int end=y+size,lo=y>top?y:top,hi=end<bot+1?end:bot+1;
  if(hi>lo){sceGuTexImage(0,512,size,STRIDE,fb+y*STRIDE);sceGuTexFlush();IonVertex*v=sceGuGetMemory(2*sizeof(*v));v[0]=(IonVertex){0,(float)(lo-y),0xffffffff,0,(float)lo,0};v[1]=(IonVertex){W,(float)(hi-y),0xffffffff,W,(float)hi,0};sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,2,0,v);}y=end;
 }
 sceGuTexImage(0,512,512,STRIDE,jump_scene_pixels);sceGuTexFlush();sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_FIX,0,0xffffffff);ion_gu_mesh(radial,top,bot,bleed_scale,1);
 sceGuTexSync();sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top+1,STRIDE,(void*)(0x04000000u+offset),0,top,STRIDE,fb);sceGuFinish();int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,bytes);
 if(result<0){ion_gu_ready=0;return 0;}return 1;
}

static unsigned ion_beam_texture[16][16] __attribute__((aligned(16)));static int ion_fx_gpu;
static void ion_fx_begin(int top,int bot,int current){
 ion_fx_gpu=ion_gu_ready&&fb==dc_open_source;if(!ion_fx_gpu)return;
 unsigned bytes=STRIDE*H*4,offset=2*STRIDE*H*4;sceKernelDcacheWritebackInvalidateRange(fb,bytes);sceKernelDcacheWritebackRange(ion_beam_texture,sizeof(ion_beam_texture));
 sceGuStart(GU_DIRECT,ion_gu_list);sceGuDrawBuffer(GU_PSM_8888,(void*)offset,STRIDE);if(!current){sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top+1,STRIDE,fb,0,top,STRIDE,(void*)(0x04000000u+offset));sceGuTexSync();}
 sceGuOffset(2048-W/2,2048-H/2);sceGuViewport(2048,2048,W,H);sceGuScissor(0,top,W,bot+1);sceGuEnable(GU_SCISSOR_TEST);sceGuDisable(GU_DEPTH_TEST);sceGuDisable(GU_CULL_FACE);sceGuDisable(GU_LIGHTING);sceGuDisable(GU_ALPHA_TEST);sceGuDisable(GU_STENCIL_TEST);sceGuDisable(GU_DITHER);sceGuDisable(GU_FOG);sceGuEnable(GU_TEXTURE_2D);sceGuEnable(GU_BLEND);sceGuShadeModel(GU_SMOOTH);
 sceGuTexMode(GU_PSM_8888,0,0,0);sceGuTexImage(0,16,16,16,ion_beam_texture);sceGuTexFlush();sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);sceGuTexFilter(GU_LINEAR,GU_LINEAR);sceGuTexWrap(GU_CLAMP,GU_CLAMP);sceGuTexScale(1,1);sceGuTexOffset(0,0);sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_FIX,0,0xffffffff);
}
static void ion_beam_quad(Point a,Point b,float wa,float wb,unsigned color){
 float dx=b.x-a.x,dy=b.y-a.y,len2=dx*dx+dy*dy;if(len2<.04f)return;float inv=1.f/sqrtf(len2),nx=-dy*inv,ny=dx*inv;IonVertex*v=sceGuGetMemory(4*sizeof(*v));
 v[0]=(IonVertex){0,0,color,a.x+nx*wa,a.y+ny*wa,0};v[1]=(IonVertex){0,16,color,a.x-nx*wa,a.y-ny*wa,0};v[2]=(IonVertex){16,0,color,b.x+nx*wb,b.y+ny*wb,0};v[3]=(IonVertex){16,16,color,b.x-nx*wb,b.y-ny*wb,0};sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D,4,0,v);
}
static void ion_stroke(Point a,Point b,float wa,float wb,unsigned tint,float intensity,int hot){
 if(journey_variant)intensity*=journey_envelope;if(!ion_fx_gpu){ship_wake_stroke(a,b,wa,wb,tint,intensity,hot);return;}if(ship_wake_budget<=0||intensity<=0)return;ship_wake_budget-=32;
 int alpha=(int)(intensity*311);if(alpha>255)alpha=255;ion_beam_quad(a,b,wa+1.2f,wb+1.2f,(tint&0xffffffu)|((unsigned)alpha<<24));
 if(hot){alpha=(int)(intensity*150);if(alpha>255)alpha=255;ion_beam_quad(a,b,wa*.35f+.1f,wb*.35f+.1f,0xffe6c8u|((unsigned)alpha<<24));}
}
static void ion_fx_end(int top,int bot){
 if(!ion_fx_gpu)return;sceGuTexSync();sceGuCopyImage(GU_PSM_8888,0,top,W,bot-top+1,STRIDE,(void*)(0x04000000u+2*STRIDE*H*4),0,top,STRIDE,fb);sceGuFinish();int result=sceGuSync(GU_SYNC_FINISH,GU_SYNC_WHAT_DONE);sceKernelDcacheInvalidateRange(fb,STRIDE*H*4);if(result<0)ion_gu_ready=0;ion_fx_gpu=0;
}
static unsigned*ion_opaque_colors;
static IonColor*ion_colors;static unsigned short*ion_indices,*ion_pairs;static int ion_pair_count;

static void __attribute__((optimize(ELITE_VISUAL_OPT))) ion_init(void){
 /* Station/fleet acceleration must not depend on successful warp-art allocation. */
#ifdef ELITE_PSP3000
 if(!ion_gu_list)ion_gu_list=memalign(16,131072*4);
 if(ion_gu_list&&!ion_gu_ready&&sceGuInit()>=0)ion_gu_ready=1;
#endif
 if(jump_polar)return;ion_basis_init();for(int y=0;y<16;y++)for(int x=0;x<16;x++){float d=(y+.5f-8)*(1.f/8),q=fmaxf(0,1-d*d);ion_beam_texture[y][x]=0xffffffu|((unsigned)(255*q*q)<<24);}{
  jump_polar=malloc(W*H*sizeof(unsigned));jump_scene_pixels=memalign(16,STRIDE*(
#ifdef ELITE_PSP3000
512
#else
H
#endif
)*4);jump_panel_art=malloc(1024);
  if(!jump_polar||!jump_scene_pixels||!jump_panel_art){free(jump_polar);free(jump_scene_pixels);free(jump_panel_art);jump_polar=0;jump_scene_pixels=0;jump_panel_art=0;return;}
 int top=24,bot=H-1;for(unsigned i=0;i<512;i++){jump_panel_art[i*2]=ion_art_nibbles[i]&15;jump_panel_art[i*2+1]=ion_art_nibbles[i]>>4;}
  for(int y=top;y<=bot;y++)for(int x=0;x<W;x++){
   float dx=x-240,dy=(y-110)*1.55f,r=sqrtf(dx*dx+dy*dy)/260.f;
   unsigned u=(unsigned)((atan2f(dy,dx)+3.14159265f)*(128.f/6.2831853f))&127,z=(unsigned)(310.f/(r+.055f)),rr=(unsigned)fminf(511,r*256);jump_polar[y*W+x]=u|(z<<7)|(rr<<20);
  }
 }
#ifdef ELITE_PSP3000
 /* Dense remapping avoids allocating/lighting all 65,536 polar bins. */
 unsigned short*slots=malloc(65536*2);ion_indices=malloc(W*H*2);ion_pairs=malloc(65536*2);
 if(slots&&ion_indices&&ion_pairs){memset(slots,255,65536*2);for(int y=24;y<H;y++)for(int x=0;x<W;x++){
  unsigned bits=jump_polar[y*W+x],pair=((bits>>20)<<7)|(bits&127);if(slots[pair]==65535){slots[pair]=(unsigned short)ion_pair_count;ion_pairs[ion_pair_count++]=(unsigned short)pair;}ion_indices[y*W+x]=slots[pair];}
  ion_colors=malloc(ion_pair_count*sizeof(*ion_colors));ion_opaque_colors=malloc(ion_pair_count*4);}
 free(slots);if(!ion_colors||!ion_opaque_colors){free(ion_colors);free(ion_opaque_colors);ion_colors=0;ion_opaque_colors=0;free(ion_indices);free(ion_pairs);ion_indices=ion_pairs=0;ion_pair_count=0;}
#endif
}

/* The study's fitted 16x64 pixel surfaces are generated once per design.
 * Exact original artwork is restored when current-game mode is selected. */
static void journey_art_prepare(int style){
 if(!jump_panel_art)return;
 if(!style){for(int i=0;i<512;i++){jump_panel_art[i*2]=ion_art_nibbles[i]&15;jump_panel_art[i*2+1]=ion_art_nibbles[i]>>4;}return;}
 for(int y=0;y<64;y++)for(int x=0;x<16;x++){
  int edge=x<8?x:15-x,ink=0;
  if(style==1){ink=edge<1?5:edge<2?4:edge<3?3:edge<5?2:1;if((y==9||y==39)&&edge<3)ink=6;if(y%16==0&&edge>2&&edge<6)ink=3;}
  else if(style==2){ink=edge<1?5:edge<2?4:edge<3?3:edge<5?2:1;if(y%32<2&&edge<2)ink=6;}
  else{ink=edge<2?5:edge<3?4:edge<6?2:1;if((y==16||y==48)&&edge<2)ink=6;}
  jump_panel_art[y*16+x]=ink;
 }
}
/* Three continuous helices, not discrete rings. The light packets advance
 * outward in logarithmic perspective and widen as they pass the viewer. */
static unsigned journey_spectrum(float hue){
 static const unsigned colors[7]={0x8c69ff,0x67c8ff,0x79edb2,0xebdf61,0xffb176,0xf978b9,0xe979ee};
 hue=fmodf(hue,7);if(hue<0)hue+=7;int n=(int)hue;float t=hue-n;
 unsigned a=colors[n],b=colors[(n+1)%7];
 return RGB((int)((a&255)*(1-t)+(b&255)*t),(int)(((a>>8)&255)*(1-t)+((b>>8)&255)*t),(int)(((a>>16)&255)*(1-t)+((b>>16)&255)*t));
}
static void journey_rainbow(float seconds,float distance){
 if(!journey_variant)return;
 float active=ion_smooth((seconds-4.92f)/.46f)*(1-ion_smooth((seconds-9.72f)/.42f));
 if(active<=0)return;ship_wake_budget=8000;
 for(int strand=0;strand<3;strand++){
  Point prev={0,0,1000};float lastwidth=0;
  for(int k=0;k<=40;k++){
   float depth=k*(1.f/40),lr=depth*2.85f,r=31*expf(lr);
   float fluid=sinf(lr*3-distance*.9f+strand*.7f),ripple=sinf(lr*7-distance*.63f);
   r*=1+active*(fluid*.10f+ripple*.035f);
   float angle=strand*2.0943951f+lr*2.45f-distance*.8f;
   angle+=active*(sinf(lr*2.5f-distance*.67f+strand)*.30f+sinf(lr*5.2f-distance*.9f)*.09f);
   Point q={240+cosf(angle)*r,110+sinf(angle)*r/1.55f,1000};
   float focus=ion_smooth((r-42)/45);
   float packet=powf(.5f+.5f*cosf(lr*6-distance*3.6f+strand),6);
   float breathing=.5f+.5f*sinf(lr*3.8f-distance*1.15f+strand*.8f);
   float width=(.8f+r*.026f)*(1+packet*.85f+breathing*.30f);
   unsigned c=journey_spectrum(lr*2.1f-distance*.66f+strand*2.3333f);
   if(k){
    float glow=active*focus*(.32f+packet*.31f+breathing*.06f);
    ion_stroke(prev,q,lastwidth*2.4f,width*2.4f,c,glow*.38f,0);
    ion_stroke(prev,q,lastwidth,width,c,glow,packet>.70f);
   }
   prev=q;lastwidth=width;
  }
 }
}
static void journey_accents(float seconds,float energy,float distance,float burst,unsigned tint){
 if(!journey_variant)return;
 float active=ion_smooth((seconds-3.35f)/1.15f),aspect=1.f/1.55f;
 float rush=ion_smooth((seconds-4.95f)/.5f)*(1-ion_smooth((seconds-9.7f)/.4f));
 ship_wake_budget=9000;
 int strands=journey_variant==1?(seconds>5&&seconds<9.95f?6:10):journey_variant==2?4:8;
 for(int strand=0;strand<strands;strand++){
  Point last={0,0,1000};
  for(int j=0;j<=20;j++){
   float r=18+j*18.5f,angle=strand*6.2831853f/strands;
   if(journey_variant==2)angle+=.56f*logf(r/18)+distance*.035f;
   if(journey_variant==3)angle+=sinf(r*.011f-distance*.10f+strand*.6f)*.18f;
   angle+=rush*.12f*sinf(r*.018f-distance*.9f+strand*.8f);
   r*=1+rush*.045f*sinf(r*.015f-distance*.8f);
   Point p={240+cosf(angle)*r,110+sinf(angle)*r*aspect,1000};
   if(j){unsigned color=journey_variant==2?(strand&1?RGB(95,215,255):RGB(255,176,93)):journey_variant==3?(strand&1?RGB(163,110,251):RGB(77,236,214)):RGB(103,219,250);
    float phase=.5f+.5f*sinf(j*.55f-distance*1.15f+strand*.73f),width=journey_variant==1?1.1f:journey_variant==2?3.2f+j*.16f:2.8f;
    width+=rush*(1.2f+r*.018f*(.6f+phase));
    ion_stroke(last,p,width,width*.92f,color,active*(.12f+phase*(.12f+rush*.12f)+burst*.10f),j%6==0);
   }last=p;
  }
 }
 /* A narrow destination-coloured throat becomes the real space opening.
  * It matches the aperture radius exactly across the actual arrival frame. */
 if(seconds>9.52f&&seconds<10.85f){
  float opened=journey_exit_open(seconds),r=seconds<10?6+13.5f*ion_smooth((seconds-9.52f)/.48f):(.075f+opened*1.65f)*260;
  float forming=ion_smooth((seconds-9.52f)/.25f),power=forming*(1-opened);
  unsigned horizon=journey_exit_tint(seconds);ship_wake_budget=7000;
  Point last={240+r,110,1000};
  for(int j=1;j<=64;j++){
   float angle=j*.09817477f;Point p={240+cosf(angle)*r,110+sinf(angle)*r*aspect,1000};
   ion_stroke(last,p,2.4f+power*2,2.4f+power*2,horizon,power*.52f,1);last=p;
  }
  /* A few short rim splinters peel outwards as the threshold passes. */
  if(seconds>=10)for(int i=0;i<6;i++){
   float a=i*1.04719755f,ca=cosf(a),sa=sinf(a),tail=8+opened*28;
   Point p={240+ca*r,110+sa*r*aspect,1000},q={240+ca*(r+tail),110+sa*(r+tail)*aspect,1000};
   ion_stroke(p,q,1.2f,.1f,horizon,(1-opened)*.40f,1);
  }
 }

}

static void __attribute__((optimize(ELITE_VISUAL_OPT))) ion_render(float seconds){
 journey_envelope=journey_variant?(seconds>10?1-ion_smooth((seconds-10)/1.1f):ion_smooth(seconds/.85f)):1;
 int style=journey_variant==2?2:journey_variant==3?3:1;float elapsed=seconds<5?seconds*.6f:seconds-2;
 int top=view_top()<24?24:view_top(),bot=view_bot(),saved=mega_depth_valid;mega_depth_valid=0;ship_wake_budget=1024;
 if(!jump_polar){mega_depth_valid=saved;return;}
 if(seconds>=5&&seconds<10)memcpy(jump_scene_pixels,fb,STRIDE*24*4);else memcpy(jump_scene_pixels,fb,STRIDE*H*4);
 int phase=elapsed<3?0:elapsed<5?1:2;float t=phase==0?elapsed/3:phase==1?(elapsed-3)/2:(elapsed-5)/3;

 float speed=ion_speed(seconds),distance=ion_distance(seconds),burst=ion_burst(seconds);vortex_prepare(seconds,distance);
 float energy=phase==0?(.055f*ion_smooth(seconds/4.2f)+.945f*ion_smooth((seconds-4.2f)/.8f)):phase==1?1:seconds<10?1:1-ion_smooth((seconds-10)/.65f);
 if(journey_variant){energy=seconds<5?(.055f*ion_smooth(seconds/4.2f)+.945f*ion_smooth((seconds-4.2f)/.8f)):seconds<10.75f?1:1-ion_smooth((seconds-10.75f)/.40f);}
 unsigned tint=style==1?RGB(70,221,255):style==2?RGB(255,190,91):RGB(193,113,255);
 unsigned palettes[3][8]={
  {RGB(4,9,20),RGB(9,23,42),RGB(16,42,68),RGB(24,65,91),RGB(55,118,147),RGB(50,178,218),RGB(144,250,255),RGB(235,248,232)},
  {RGB(13,6,17),RGB(35,17,32),RGB(61,34,47),RGB(89,53,56),RGB(155,98,64),RGB(228,139,48),RGB(255,218,137),RGB(255,247,213)},
  {RGB(8,5,24),RGB(24,13,51),RGB(48,25,83),RGB(72,40,115),RGB(119,74,162),RGB(162,90,231),RGB(218,167,255),RGB(171,249,255)}
 };
 if(journey_variant){
  unsigned clean[3][8]={
   {RGB(2,6,16),RGB(5,13,28),RGB(9,28,48),RGB(15,49,76),RGB(32,97,139),RGB(54,184,222),RGB(151,239,247),RGB(236,249,248)},
   {RGB(8,5,13),RGB(22,13,24),RGB(48,30,37),RGB(81,48,44),RGB(146,89,61),RGB(223,150,77),RGB(255,222,160),RGB(244,249,239)},
   {RGB(4,5,17),RGB(13,12,34),RGB(29,24,61),RGB(57,42,96),RGB(96,74,151),RGB(152,112,222),RGB(127,235,219),RGB(233,246,249)}};
  memcpy(palettes,clean,sizeof(clean));
 }
 float wave[128];int lighting[128];if(energy>.06f){float ws=sinf(elapsed*2.2f),wc=cosf(elapsed*2.2f),ls=sinf(elapsed*.55f),lc=cosf(elapsed*.55f);
  for(int u=0;u<128;u++){wave[u]=ion_sin[u]*wc+ion_cos[u]*ws;lighting[u]=(int)(150+85*(ion_cos[u]*lc+ion_sin[u]*ls));}}

 /* The actual scene projection also widens with speed. Lens stretch is peripheral. */
 float exit=journey_variant?journey_exit_open(seconds):0;
 float zoom=1-energy*.30f*(1-exit)-burst*.025f*(1-exit);int scroll=(int)(distance*42);
 if(seconds<5)ion_charge_cached=seconds>=4.2f;
 int hardware_scene=0;
 if(energy>.06f){
 /* Radius-only lens and light work is prepared once, not repeated per pixel. */
 IonRadius radial[512];int hotlight[128];for(int i=0;i<128;i++)hotlight[i]=240+(int)(wave[i]*15);
 float launch=ion_smooth((seconds-4.35f)/.32f)*(1-ion_smooth((seconds-4.8f)/.35f));
 float crest_radius=-10,crest_power=0,crest_echo=0;
 if(journey_variant)for(int crest=0;crest<6;crest++){
  float age=seconds-journey_crest_time[crest];
  if(age>=0&&age<.52f){crest_radius=.045f+age*(2.9f+journey_crest_gain[crest]*1.6f);crest_power=journey_crest_gain[crest]*ion_smooth(age/.055f)*(1-ion_smooth(age/.52f));crest_echo=(crest&1)?0:.43f;}
 }
 for(int i=0;i<512;i++){
  float r=i*(1.f/256);IonRadius *q=&radial[i];q->z=(int)((310.f/(r+.055f))/(1+speed*.17f+burst*.07f));q->v=(q->z/3-scroll)&63;
  float fog=1-ion_smooth((r-.055f)*5),rim=ion_smooth((r-.18f)*2.8f);int ring=(q->z-scroll*3)&255;float pulse=ring<10?(10-ring)*.1f:0;
  float shock=0;if(launch>0)shock=fmaxf(0,1-fabsf(r-(seconds-4.35f)*2.1f)/.17f)*launch;
  if(crest_power>0){float delta=r-crest_radius;if(delta<0)delta=-delta;float front=1-delta*9.5238095f;if(front<0)front=0;
   if(crest_echo>0){delta=r-crest_radius*.82f;if(delta<0)delta=-delta;float echo=1-delta*13.333333f;if(echo>0)front+=crest_echo*echo;}
   shock+=front*crest_power;
  }
  q->red=(int)(fog*35+pulse*32)+(int)(burst*(28+rim*30))+(int)(shock*95);
  q->green=(int)(fog*73+pulse*45)+(int)(burst*(52+rim*48))+(int)(shock*140);
  q->blue=(int)(fog*99+pulse*55)+(int)(burst*(65+rim*55))+(int)(shock*165);
  float wall=ion_smooth((energy-(1-fminf(1,r))*.20f)*1.25f);if(journey_variant&&seconds>=10){float aperture=.075f+journey_exit_open(seconds)*1.65f;wall=ion_smooth((r-aperture)/.12f)*energy;}q->gain=(int)(wall*256);q->bleed=(int)(energy*.65f*(1-wall)*wall*256);
  float lens_release=journey_variant?(1-exit):1;float distortion=1+lens_release*(energy*.025f*wave[(q->z/5+(int)(elapsed*18))&127]+speed*.08f*r*r);q->inv=(int)(65536/(zoom*distortion));
 }
 int angle_shift=(int)(elapsed*3),bleed_scale=(int)((.85f-energy*.13f)*256);
 unsigned base_light[64][128] __attribute__((aligned(16)));if(ion_colors){unsigned lit_palette[8][128];
  for(int idx=0;idx<8;idx++)for(int u=0;u<128;u++){int lit=idx>=5?hotlight[u]:lighting[u];unsigned c=palettes[style-1][idx];
   if(journey_variant==1){
    static const unsigned cool[8]={0x140706,0x29120b,0x482419,0x723d31,0x9e6255,0xc68f72,0xe5c4a4,0xffe5cf};
    static const unsigned warm[8]={0x160912,0x281c30,0x323953,0x355b89,0x3b85b9,0x5fb6e3,0x8bdafa,0xb8edff};
    int t=vortex_warmth[u],q=256-t;unsigned a=cool[idx],b=warm[idx];
    int red=((a&255)*q+(b&255)*t)>>8;
    int green=(((a>>8)&255)*q+((b>>8)&255)*t)>>8;
    int blue=(((a>>16)&255)*q+((b>>16)&255)*t)>>8;
    c=RGB(red*3/4,green*3/4,blue*3/4);
   }
   lit_palette[idx][u]=RGB((c&255)*lit/256,((c>>8)&255)*lit/256,((c>>16)&255)*lit/256);}
  for(int v=0;v<64;v++)for(int u=0;u<128;u++){int idx=journey_variant==1?vortex_ink[v][u]:jump_panel_art[v*16+(u&15)];base_light[v][u]=lit_palette[idx][idx>=5?(v*2+u)&127:u];}
 }
 int gpu_tunnel=ion_colors&&ion_gu_tunnel(radial,base_light,top,bot,angle_shift,scroll,(energy<1.f||(journey_variant&&seconds>=10)),bleed_scale);
 if(ion_colors&&!gpu_tunnel)for(int i=0;i<ion_pair_count;i++){
  unsigned pair=ion_pairs[i];IonRadius*q=&radial[pair>>7];int au=((pair&127)+angle_shift)&127;unsigned c=base_light[q->v][au];IonColor*o=&ion_colors[i];
  int rr=(c&255)+q->red,gg=((c>>8)&255)+q->green,bb=((c>>16)&255)+q->blue;
  if((energy>=1.f&&!(journey_variant&&seconds>=10)))ion_opaque_colors[i]=RGB(rr>255?255:rr,gg>255?255:gg,bb>255?255:bb);
  else{o->red=rr*q->gain;o->green=gg*q->gain;o->blue=bb*q->gain;rr=o->red>>8;gg=o->green>>8;bb=o->blue>>8;
   ion_opaque_colors[i]=(RGB(rr>255?255:rr,gg>255?255:gg,bb>255?255:bb)&0xffffffu)|((unsigned)((256-q->gain)*255/256)<<24);}
 }
 int hardware=!gpu_tunnel&&ion_colors&&(energy<1.f||(journey_variant&&seconds>=10))&&ion_gu_ready&&fb==dc_open_source;
 if(hardware){for(int y=top;y<=bot;y++){unsigned*out=fb+y*STRIDE;const unsigned short*index=ion_indices+y*W;for(int x=0;x<W;x++)out[x]=ion_opaque_colors[index[x]];}hardware=ion_gu_warp(radial,top,bot,bleed_scale);}
 hardware_scene=gpu_tunnel||hardware;
 if(hardware_scene){}else if(ion_colors&&(energy>=1.f&&!(journey_variant&&seconds>=10))){
  for(int y=top;y<=bot;y++){unsigned*out=fb+y*STRIDE;const unsigned short*index=ion_indices+y*W;for(int x=0;x<W;x++)out[x]=ion_opaque_colors[index[x]];}
 }else for(int y=top;y<=bot;y++)for(int x=0;x<W;x++){
  unsigned bits=jump_polar[y*W+x];IonRadius q=radial[bits>>20];int au=((bits&127)+(int)(elapsed*3))&127;
  int rr,gg,bb;if(ion_colors){IonColor*o=&ion_colors[ion_indices[y*W+x]];rr=o->red;gg=o->green;bb=o->blue;}
  else{int idx=jump_panel_art[q.v*16+(au&15)],lit=idx>=5?hotlight[(q.v*2+au)&127]:lighting[au];unsigned c=palettes[style-1][idx];
   rr=((c&255)*lit/256+q.red)*q.gain;gg=(((c>>8)&255)*lit/256+q.green)*q.gain;bb=(((c>>16)&255)*lit/256+q.blue)*q.gain;}
  if(q.gain<256){int sx=240+((x-240)*q.inv>>16),sy=110+((y-110)*q.inv>>16);if(sx<0)sx=0;if(sx>=W)sx=W-1;if(sy<top)sy=top;if(sy>bot)sy=bot;unsigned a=jump_scene_pixels[sy*STRIDE+sx];int keep=256-q.gain;
   rr+=(a&255)*keep;gg+=((a>>8)&255)*keep;bb+=((a>>16)&255)*keep;
   if(q.bleed){int k=bleed_scale,rx=240+((sx-240)*k>>8),ry=110+((sy-110)*k>>8);if(ry<top)ry=top;if(ry>bot)ry=bot;unsigned b=jump_scene_pixels[ry*STRIDE+rx];rr+=(b&255)*q.bleed;gg+=((b>>8)&255)*q.bleed;bb+=((b>>16)&255)*q.bleed;}
  }
  rr>>=8;gg>>=8;bb>>=8;if(rr>255)rr=255;if(gg>255)gg=255;if(bb>255)bb=255;fb[y*STRIDE+x]=RGB(rr,gg,bb);
 }
 }
 ion_fx_begin(top,bot,hardware_scene);
 float cx=240+(journey_variant?0:sinf(elapsed*.8f)*5*energy),cy=110+(journey_variant?0:cosf(elapsed*.7f)*3*energy);
 int seeds=0;if(energy<=.98f)for(int yy=top+3;yy<bot-3&&seeds<24;yy+=3)for(int xx=3;xx<W-3&&seeds<24;xx+=3){
  unsigned star=jump_scene_pixels[yy*STRIDE+xx];int lum=(star&255)+((star>>8)&255)+((star>>16)&255);
  if(lum<480||energy>.98f)continue;float dx=xx-240,dy=yy-110;if(dx*dx+dy*dy<1200)continue;
  seeds++;float k=.045f*speed+.13f*burst;
  Point a={xx,yy,1000},b={xx+dx*k,yy+dy*k,1000};ion_stroke(a,b,.6f,.15f,RGB(180,230,255),fminf(.55f,.08f+energy*.5f),0);
 }
 float streak_cos=cosf(elapsed*.035f),streak_sin=sinf(elapsed*.035f);
 float streak_entry=journey_variant?ion_smooth(seconds/.85f):1;
 ship_wake_budget=6000;for(int i=0;i<(journey_variant?(seconds<5?72:48):(seconds<5?112:74))&&ship_wake_budget>0;i++){
  float ca=ion_streak_cos[i]*streak_cos-ion_streak_sin[i]*streak_sin,sa=ion_streak_sin[i]*streak_cos+ion_streak_cos[i]*streak_sin,phase=fmodf(i*31.f+distance*138,330)/330.f;
  float birth=journey_variant?ion_smooth(phase/.10f)*(1-ion_smooth((phase-.78f)/.22f)):1;
  float r=(12+phase*330)*(.18f+.82f*streak_entry),len=(3+speed*(11+r*.13f)+burst*(20+r*.12f))*(.08f+.92f*streak_entry)*birth;
  Point p={cx+ca*r,cy+sa*r*(journey_variant?1.f/1.55f:.59f),1000},q={cx+ca*(r+len),cy+sa*(r+len)*(journey_variant?1.f/1.55f:.59f),1000};
  if(vortex_flash>.12f&&i%8==0&&r>65){
   float kink=ion_sin[((int)(distance*40)+i*7)&127]*(3+vortex_flash*7);
   Point a={p.x+(q.x-p.x)*.34f-sa*kink,p.y+(q.y-p.y)*.34f+ca*kink*.645f,1000};
   Point b={p.x+(q.x-p.x)*.68f+sa*kink*.6f,p.y+(q.y-p.y)*.68f-ca*kink*.39f,1000};
   unsigned bolt=i%16?RGB(180,205,255):RGB(255,217,153);
   ion_stroke(p,a,1.1f,.7f,bolt,vortex_flash*.72f,1);ion_stroke(a,b,.7f,.6f,bolt,vortex_flash*.72f,1);ion_stroke(b,q,.6f,.1f,bolt,vortex_flash*.65f,0);
  }else ion_stroke(p,q,(journey_variant?.55f+energy*.50f+burst*.45f:.75f+energy*1.5f+burst*.8f),.15f,i%5==0?RGB(236,245,255):tint,(.15f+energy*.62f)*birth,i%11==0);
 }
 // Perspective energy hoops with softly lit edges; they move towards and past the viewer.
 ship_wake_budget=2000;for(int ring=0;ring<7&&ship_wake_budget>0;ring++){
  float r=16+fmodf(ring*49+distance*87,350),crest_ink=0;unsigned ring_tint=tint;
  if(journey_variant&&ring==0)for(int crest=0;crest<6;crest++){
   float age=seconds-journey_crest_time[crest];if(age<0||age>.52f)continue;
   float gain=journey_crest_gain[crest];r=12+age*(760+gain*420);
   crest_ink=gain*ion_smooth(age/.055f)*(1-ion_smooth(age/.52f));
   ring_tint=crest%3==0?RGB(125,228,255):crest%3==1?RGB(158,155,255):RGB(131,255,225);
  }
  if(journey_variant&&ring==0&&seconds>=10.22f&&seconds<10.77f){
   float age=seconds-10.22f;r=40+age*780;crest_ink=.65f*(1-ion_smooth(age/.55f));ring_tint=journey_exit_tint(seconds);
  }
  Point last={cx+r,cy,1000};
  for(int k=1;k<=40&&ship_wake_budget>0;k++){Point q={cx+ion_hoop_cos[k]*r,cy+ion_hoop_sin[k]*r*(journey_variant?1.f/1.55f:.59f),1000};
   ion_stroke(last,q,1.0f+energy+crest_ink*2,1.0f+crest_ink*2,ring_tint,energy*(.14f+burst*.22f)+crest_ink*.25f,crest_ink>.45f);last=q;}
 }
 if(!journey_variant){
 /* Coherent edge arcs follow the forming tunnel; deterministic temporal motion. */
 ship_wake_budget=4096;if(seconds<5){float build=ion_smooth(seconds/5);for(int side=0;side<2;side++)for(int arc=0;arc<6&&ship_wake_budget>0;arc++){
  float a=(side?0:3.14159265f)+(arc-2.5f)*.18f,rr=190+arc*13,ca=cosf(a),sa=sinf(a);Point prev={240+ca*rr,110+sa*rr*.62f,1000};
  for(int k=1;k<8&&ship_wake_budget>0;k++){unsigned h=(unsigned)(arc*197+side*31+k*337+(int)(game.time*24))*2654435761u;float r=rr-k*(12+build*7),j=((int)((h>>24)&15)-7)*build;
   Point q={240+ca*r+j,110+sa*r*.62f+j*.5f,1000};ion_stroke(prev,q,1+build*2,.3f,RGB(123,228,255),build*.85f,0);prev=q;
  }
 }}
 }else{
 /* Continuous peripheral filaments bend into the same tunnel axis.
  * Branches ride along them; all loops and GPU submissions remain bounded. */
 ship_wake_budget=6500;if(seconds<5){
  float build=ion_smooth(seconds/5),surge=ion_smooth((seconds-3.7f)/1.3f);
  for(int side=0;side<2;side++)for(int arc=0;arc<6&&ship_wake_budget>0;arc++){
   float base=(side?0:3.14159265f)+(arc-2.5f)*.18f,rr=204+arc*13;
   Point prev={240+cosf(base)*rr,110+sinf(base)*rr*.645f,1000};
   float packet=fmodf(seconds*(1.8f+surge*1.5f)+arc*.273f+side*.31f,1.f)*8;
   for(int k=1;k<=8&&ship_wake_budget>0;k++){
    float r=rr-k*(12+build*7),phase=seconds*(9+surge*6)+arc*1.9f+k*1.7f;
    float bend=(sinf(phase)*.034f+sinf(phase*1.73f)*.018f)*build;
    float a=base+bend+sinf(seconds*3.4f+arc*.7f+k*.46f)*surge*.055f;
    Point q={240+cosf(a)*r,110+sinf(a)*r*.645f,1000};
    float bright=fmaxf(0,1-fabsf(k-packet));
    ion_stroke(prev,q,1.05f+surge*.8f,.5f,RGB(78,189,255),build*(.40f+bright*.46f),bright>.6f);
    if((k==3||k==6)&&bright>.25f){
     float sign=(arc&1)?1:-1,reach=(5+surge*10)*bright;
     Point b={q.x-sinf(a)*reach*sign,q.y+cosf(a)*reach*.645f*sign,1000};
     Point c={b.x+cosf(a)*reach*.65f,b.y+sinf(a)*reach*.42f,1000};
     ion_stroke(q,b,.8f,.25f,RGB(154,226,255),build*bright*.9f,1);
     ion_stroke(b,c,.5f,.1f,RGB(107,195,255),build*bright*.65f,0);
     Point d={q.x+cosf(a)*4,q.y+sinf(a)*2.6f,1000};
     ion_stroke(q,d,2.4f,.15f,RGB(164,232,255),build*bright*.65f,1);
    }
    prev=q;
   }
  }
 }
 }
 journey_accents(seconds,energy,distance,burst,tint);journey_rainbow(seconds,distance);ion_fx_end(top,bot);
 for(int y=0;y<top;y++)memcpy(fb+y*STRIDE,jump_scene_pixels+y*STRIDE,STRIDE*4);
 mega_depth_valid=saved;
}
