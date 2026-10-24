/* Code-native pixel clouds: 8 KiB cached silhouettes, no per-frame allocation.
 * Lobes have shaded undersides, bright upper rims and feathered pixel edges. */
static unsigned char atmosphere_cloud[4][32][64];
static int atmosphere_cloud_ready;
static void atmosphere_cloud_init(void){
 if(atmosphere_cloud_ready)return;atmosphere_cloud_ready=1;
 for(int k=0;k<4;k++)for(int y=0;y<32;y++)for(int x=0;x<64;x++){
  float density=0;
  for(int l=0;l<6;l++){
   float cx=l?8+(l-1)*11.f:32,cy=l?13+(l*7+k*3)%9:23;
   float rx=l?10+(l+k)%5:28,ry=l?8+(l*3+k)%5:6;
   float dx=(x-cx)/rx,dy=(y-cy)/ry,d=1-dx*dx-dy*dy;
   density=fmaxf(density,d);
  }
  if(density<=0){atmosphere_cloud[k][y][x]=0;continue;}
  int alpha=(int)fminf(15, density*45),shade=(int)fmaxf(1,fminf(15,15-y*.34f+density*3-(x>40?1:0)));
  atmosphere_cloud[k][y][x]=(alpha<<4)|shade;
 }
}
static unsigned atmosphere_blend(unsigned p,unsigned ink,int weight){
 int inv=256-weight;
 return 0xff000000|(((((p&0xff00ff)*inv+(ink&0xff00ff)*weight)>>8)&0xff00ff))|
        (((((p&0x00ff00)*inv+(ink&0x00ff00)*weight)>>8)&0x00ff00));
}
static void atmosphere_cloud_sprite(int cx,int cy,int width,int height,int kind,unsigned ink,float opacity,int top,int bottom){
 if(width<2||height<2||opacity<=0)return;atmosphere_cloud_init();
 int left=cx-width/2,base=cy-height/2,x0=left<0?0:left,x1=left+width>W?W:left+width,y0=base<top?top:base,y1=base+height>bottom?bottom:base+height;
 if(x0>=x1||y0>=y1)return;
 unsigned palette[16],source_rb[256],source_g[256];int weights[16],columns[480];
 float light=((ink&255)+((ink>>8)&255)+((ink>>16)&255))/765.f;
 unsigned shadow=mix_rgb(ink,RGB(24,34,49),.25f),highlight=mix_rgb(ink,RGB(246,245,221),.03f+.65f*light);
 for(int i=0;i<16;i++){palette[i]=mix_rgb(shadow,highlight,i/15.f);weights[i]=(int)(i*(256.f/15)*opacity);}
 /* Source colour/alpha products are constant for a sprite, not per pixel. */
 for(int p=0;p<256;p++){source_rb[p]=(palette[p&15]&0xff00ff)*weights[p>>4];source_g[p]=(palette[p&15]&0x00ff00)*weights[p>>4];}
 for(int x=x0;x<x1;x++)columns[x]=(x-left)*64/width;
 for(int y=y0;y<y1;y++){
  const unsigned char *row=atmosphere_cloud[kind&3][(y-base)*32/height];
  unsigned *out=fb+y*STRIDE;
  for(int x=x0;x<x1;x++){int p=row[columns[x]],a=p>>4;if(a){
   unsigned old=out[x];int inv=256-weights[a];
   out[x]=0xff000000|(((((old&0xff00ff)*inv+source_rb[p])>>8)&0xff00ff))|(((((old&0x00ff00)*inv+source_g[p])>>8)&0x00ff00));
  }}
 }
}
