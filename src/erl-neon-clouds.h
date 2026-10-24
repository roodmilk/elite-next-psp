/* Sunset above the clouds, bounded for PSP. A seamless 1KB bake
 * replaces runtime sine noise; its repeat is a hardware-budget approximation.
 * World anchoring is retained; the lavender deck sits at y=-95 design units.
 * Interpolate a 16x16 native grid smoothly; no heap or framebuffer-sized scratch.
 * The original 64x64 bake is box-filtered to 32x32 for this atmosphere.
 * Keep the 128-unit sampling scale if regenerating this compact tile. */
/* BEGIN GENERATED CLOUD TILE */
static const unsigned char neon_cloud_noise[32][32]={
 {146,176,177,169,175,196,206,186,147,120,118,128,122,99,86,102,135,152,141,121,117,128,129,103,64,44,54,74,79,70,71,101},
 {180,182,174,181,204,216,197,157,127,123,128,118,92,75,89,121,141,133,117,117,133,139,115,78,58,67,86,89,77,77,106,150},
 {183,175,184,207,221,203,163,132,126,128,115,85,65,76,107,129,125,113,117,138,149,130,95,75,82,99,99,85,82,109,151,181},
 {171,180,205,221,204,166,135,128,128,112,79,55,63,94,116,115,107,116,142,158,144,112,92,99,113,110,93,86,109,149,177,178},
 {171,198,215,201,166,136,129,129,111,75,48,53,81,103,104,99,112,144,166,157,128,110,116,128,121,98,87,105,142,168,168,161},
 {184,205,194,162,136,131,131,113,75,45,45,71,91,92,89,106,142,170,167,143,126,132,142,132,104,87,100,132,153,152,144,155},
 {189,183,156,135,134,137,119,80,47,44,65,83,82,78,96,136,170,173,153,141,148,157,144,111,87,93,119,136,132,123,135,166},
 {169,147,132,137,144,130,91,57,50,67,79,74,68,85,127,164,172,158,150,161,171,157,120,90,89,108,119,110,99,111,143,170},
 {135,127,139,153,143,107,73,65,76,84,72,61,74,114,152,165,156,153,169,182,170,132,98,91,103,106,90,76,85,119,148,151},
 {119,139,160,157,126,95,86,95,97,79,60,66,101,138,151,146,149,169,189,181,145,110,100,105,101,78,57,63,95,125,131,120},
 {133,162,166,143,117,111,120,119,95,67,65,93,123,134,129,135,161,188,186,156,124,114,116,107,77,49,48,75,103,109,102,107},
 {157,168,152,135,135,147,145,117,84,74,92,114,118,109,115,145,177,183,161,136,130,133,122,87,53,45,65,87,89,82,90,121},
 {160,151,142,151,169,170,144,107,92,101,114,108,92,93,123,158,170,157,140,142,151,142,107,68,54,67,81,76,65,71,104,143},
 {139,137,153,180,188,166,132,115,120,125,109,84,78,101,135,150,142,134,145,164,162,131,92,75,81,87,74,54,55,86,124,142},
 {119,142,176,192,178,151,137,143,144,123,89,73,88,115,128,121,118,137,165,173,149,116,101,105,104,83,55,48,71,104,120,117},
 {121,160,183,176,158,153,164,167,144,106,82,88,107,112,100,97,120,154,171,157,133,124,131,129,103,67,52,66,90,100,93,95},
 {136,162,161,152,157,176,186,167,129,102,102,112,108,88,79,99,135,156,151,136,136,150,153,127,88,66,73,88,88,74,71,96},
 {137,139,135,148,177,196,184,152,128,126,131,118,90,71,83,115,136,134,125,134,157,168,148,111,89,90,98,88,65,55,74,111},
 {115,113,130,166,194,191,167,150,152,156,139,103,76,78,102,117,112,105,120,151,170,159,129,111,113,117,102,71,52,64,95,115},
 {92,110,149,182,186,170,162,171,180,164,125,91,85,99,105,94,83,98,133,160,157,137,127,134,141,124,89,64,68,90,103,97},
 {94,131,164,171,162,162,180,196,185,148,112,101,108,105,84,66,76,110,140,144,133,132,149,163,149,114,86,85,100,105,91,79},
 {116,146,152,145,150,175,199,195,164,131,120,123,113,84,58,60,90,119,125,119,126,153,176,169,139,113,110,120,117,95,76,84},
 {131,133,124,131,160,189,192,168,141,135,138,127,93,60,55,77,101,106,102,114,148,179,182,158,137,137,146,138,109,82,82,107},
 {116,103,107,138,171,179,161,142,141,150,141,106,68,58,74,92,92,86,99,137,175,185,170,155,159,170,160,127,94,87,105,120},
 {83,85,113,146,157,144,132,139,154,150,118,81,67,79,91,86,75,85,124,165,181,172,163,174,188,180,145,108,95,106,114,103},
 {64,89,121,133,124,117,131,153,156,129,94,80,89,98,87,71,77,112,153,171,167,164,179,198,193,159,120,104,109,110,92,67},
 {69,99,110,102,99,119,148,157,136,106,94,104,110,95,74,73,104,143,161,157,157,177,201,199,167,129,110,112,109,84,55,46},
 {80,91,84,83,107,141,157,143,117,109,120,125,108,81,75,101,135,150,146,147,169,196,198,169,132,114,115,108,80,46,34,53},
 {77,70,71,98,137,158,149,128,124,137,143,124,93,81,101,130,141,134,134,157,187,193,167,133,116,116,108,78,41,26,42,68},
 {63,65,93,135,160,155,139,138,153,160,141,107,90,104,127,134,124,122,144,175,184,162,131,116,118,110,79,40,23,37,61,70},
 {64,93,137,164,163,150,152,169,178,157,121,100,108,127,129,115,109,130,162,174,156,128,116,120,114,83,44,25,38,61,68,61},
 {96,141,170,170,160,164,184,193,173,135,110,114,127,125,107,98,116,148,163,149,124,116,123,121,92,52,32,44,65,72,64,66},
};
/* END GENERATED CLOUD TILE */

typedef struct {Vec3 eye,right,up,ahead;float plane,inv_focal;} NeonCloudView;
static inline unsigned neon_cloud_average(unsigned a,unsigned b){return (a&b)+(((a^b)&0xfefefefeu)>>1);}
static float neon_cloud_sample(float x,float z){
 float u=x*(1.f/128),v=z*(1.f/128);int ix=(int)u,iz=(int)v;if(u<ix)ix--;if(v<iz)iz--;u-=ix;v-=iz;
 /* Smooth the LUT interpolation derivatives without per-frame noise/sines. */
 int fx=(int)(u*u*(3-2*u)*256),fz=(int)(v*v*(3-2*v)*256),x0=ix&31,z0=iz&31,x1=(ix+1)&31,z1=(iz+1)&31;
 int a=neon_cloud_noise[z0][x0],b=neon_cloud_noise[z0][x1],c=neon_cloud_noise[z1][x0],d=neon_cloud_noise[z1][x1];
 int lo=a*256+(b-a)*fx,hi=c*256+(d-c)*fx;return (lo+(hi-lo)*fz/256)*(1.f/65280);
}
static unsigned neon_cloud_colour(const NeonCloudView *view,Vec3 ray,int row){
 if(ray.y>=-.025f||view->plane/ray.y<=0){
  /* Direction-anchored cloud canopy; no screen-fixed moving noise. */
  float nx=ray.x*7800+700,nz=ray.z*7800+ray.y*5100;float n=neon_cloud_sample(nx,nz)*.65f+neon_cloud_sample(nx*2.7f,nz*2.7f)*.35f,h=1-surface_min(1,fabsf(ray.y));n=surface_max(0,surface_min(1,(n-.28f)*2.15f));
  h=surface_max(0,surface_min(1,1-ray.y*1.5f));
  return RGB((int)(72+151*h-52*n),(int)(43+101*h-34*n),(int)(95+42*h+4*n));
 }
 float distance=view->plane/ray.y,noise=neon_cloud_sample(view->eye.x+ray.x*distance,view->eye.z+ray.z*distance),haze=surface_min(1,distance*(1.f/12000));
 return RGB((int)(142+noise*49+haze*25),(int)(102+noise*44+haze*23),(int)(155+noise*38-haze*10));
}
static void __attribute__((noinline,optimize("O3","fast-math"))) neon_cloud_row(const NeonCloudView *view,int y,unsigned *row,unsigned needed){
 Vec3 ray=sub(add(view->ahead,mul(view->up,(proj_oy-y)*view->inv_focal)),mul(view->right,proj_ox*view->inv_focal)),step=mul(view->right,16*view->inv_focal);
 for(int x=0;x<=W/16;x++){if(needed&(1u<<x))row[x]=neon_cloud_colour(view,ray,y);ray=add(ray,step);}
}
static void race_neon_base_sky(void){
 for(int y=0;y<H;y++)rect(0,y,W,1,RGB(74+y*112/H,44+y*78/H,98+y*48/H));
}
static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_cloud_sky(void){
 const SpaceRace *r=&game.race;Vec3 right,up,ahead;camera_axes(&game,&right,&up,&ahead);float scale=race_neon_scale(r);Vec3 origin=mul(sub(camera_origin(&game),r->start),1/scale);
 NeonCloudView view={{dot(origin,r->right),dot(origin,r->up),dot(origin,r->dir)-7200},
  {dot(right,r->right),dot(right,r->up),dot(right,r->dir)},
  {dot(up,r->right),dot(up,r->up),dot(up,r->dir)},
  {dot(ahead,r->right),dot(ahead,r->up),dot(ahead,r->dir)},0,1/proj_focal};view.plane=-95-view.eye.y;
 unsigned first[W/16+1],second[W/16+1],*top=first,*bottom=second,valid=0;
 /* Sixteen-pixel perspective samples, smoothly interpolated on the game's
  * native 2x2 geometry grid. Sky-only blocks fill the original per-row gradient.
  * The road has already written uniform quad depth. Background colours never
  * write depth, leaving every uncovered cloud pixel exactly as before. */
 for(int y=0;y<neon_render_height;y+=16){
  unsigned masks[W/16],needed=0;
  for(int x=0;x<neon_render_width/16;x++){
   const unsigned short *depth=surface_depth+y*W+x*16;unsigned uncovered=0;
   for(int col=0;col<4;col++)for(int row=0;row<4;row++)if(depth[row*4*W+col*4]==65535||depth[(row*4+2)*W+col*4]==65535||depth[row*4*W+col*4+2]==65535||depth[(row*4+2)*W+col*4+2]==65535)uncovered|=1u<<(col*4+row);
   masks[x]=uncovered;if(uncovered)needed|=3u<<x;
  }
  /* Compute only interpolation nodes that can reach an uncovered pixel.
   * Cached nodes and ray addition order are identical to the full sky. */
  neon_cloud_row(&view,y,top,needed&~valid);neon_cloud_row(&view,y+16,bottom,needed);
  for(int x=0;x<neon_render_width/16;x++){
   unsigned uncovered=masks[x];const unsigned short *depth=surface_depth+y*W+x*16;
   if(!uncovered)continue;
   unsigned a[4],b[4];a[0]=top[x];a[2]=neon_cloud_average(a[0],top[x+1]);a[1]=neon_cloud_average(a[0],a[2]);a[3]=neon_cloud_average(a[2],top[x+1]);
   b[0]=bottom[x];b[2]=neon_cloud_average(b[0],bottom[x+1]);b[1]=neon_cloud_average(b[0],b[2]);b[3]=neon_cloud_average(b[2],bottom[x+1]);
   unsigned *pixels=fb+y*STRIDE+x*16;for(int col=0;col<4;col++){unsigned mask=(uncovered>>(col*4))&15;if(!mask)continue;unsigned mid=neon_cloud_average(a[col],b[col]),colours[4]={a[col],neon_cloud_average(a[col],mid),mid,neon_cloud_average(mid,b[col])};
    for(int row=0;row<4;row++){if(!(mask&(1u<<row)))continue;for(int sy=0;sy<4;sy+=2)for(int sx=0;sx<4;sx+=2){if(depth[(row*4+sy)*W+col*4+sx]!=65535)continue;unsigned *out=pixels+(row*4+sy)*STRIDE+col*4+sx;out[0]=out[1]=out[STRIDE]=out[STRIDE+1]=colours[row];}}
   }
  }
  unsigned *swap=top;top=bottom;bottom=swap;valid=needed;
 }
}
