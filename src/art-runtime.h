#include "next-art.h"
#include "campaign-art.h"
#include "sun-sprites.h"
/* Assets are compiled ARGB1555. All sampling stays on the native pixel grid. */
static unsigned sun_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static void draw_next_art(const uint16_t *data,int sw,int sh,int x,int y,int w,int h){
 if(w<=0||h<=0)return;
 for(int dy=0;dy<h;dy++){int yy=y+dy;if(yy<0||yy>=H)continue;int sy=dy*sh/h;
  for(int dx=0;dx<w;dx++){int xx=x+dx;if(xx<0||xx>=W)continue;unsigned p=data[sy*sw+dx*sw/w];if(!(p&0x8000))continue;
   int r=(p>>10)&31,g=(p>>5)&31,b=p&31;
   fb[yy*STRIDE+xx]=RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));
  }
 }
}
static int planet_sprite_index(unsigned seed,int type){
 static const int rocky[]={2,3,4,7};
 return type==OCEAN?(int)(seed%2):type==GAS?5+(int)(seed%2):rocky[seed%4];
}
/* Same hash used by system_bodies so chart / codex / flight share one identity. */
static unsigned body_art_seed(int sys,int body){return sun_hash((unsigned)(sys+1)*911u+(unsigned)body*65537u);}
static int sun_family(unsigned seed){return (int)(seed%8);}
/* Soft additive write — used for bloom rings only; never darkens. */
static void sun_bloom_dot(int x,int y,unsigned c,int xt,int yt,int xb,int yb){
 if(x<xt||x>=xb||y<yt||y>yb)return;
 unsigned d=fb[y*STRIDE+x];
 int r=((d&255)+(c&255));if(r>255)r=255;
 int g=(((d>>8)&255)+((c>>8)&255));if(g>255)g=255;
 int b=(((d>>16)&255)+((c>>16)&255));if(b>255)b=255;
 fb[y*STRIDE+x]=RGB(r,g,b);
}
/* Animated pixel-art star. Family from seed; frame from time. Tint shifts the sheet toward the system colour. */
static void draw_sun_sprite(int cx,int cy,int radius,unsigned tint,unsigned seed,float time,int xt,int yt,int xb,int yb){
 if(radius<1)return;
 int fam=sun_family(seed),frame=((int)(time*6.f+(seed&7))&3);
 if(frame<0)frame=0;if(frame>3)frame=3;
 const uint16_t *sheet=sun_sprites[fam][frame];
 int tr=180+((tint&255)*76)/255,tg=180+(((tint>>8)&255)*76)/255,tb=180+(((tint>>16)&255)*76)/255;
 int d=radius*2;if(d<2)d=2;
 int x0=cx-radius,y0=cy-radius;
 int left=x0>xt?x0:xt,top=y0>yt?y0:yt,right=x0+d<xb?x0+d:xb,bottom=y0+d<yb?y0+d:yb;
 if(left<0)left=0;if(top<0)top=0;if(right>W)right=W;if(bottom>H)bottom=H;
 /* Disc from the 32x32 pixel-art sheet (nearest-neighbour — keeps chunky look). */
 for(int py=top;py<bottom;py++){
  int sy=((py-y0)*32)/d;if(sy<0)sy=0;if(sy>31)sy=31;
  for(int px=left;px<right;px++){
   int sx=((px-x0)*32)/d;if(sx<0)sx=0;if(sx>31)sx=31;
   unsigned p=sheet[sy*32+sx];if(!(p&0x8000))continue;
   int cr=(p>>10)&31,cg=(p>>5)&31,cb=p&31;
   int r=((cr<<3)|(cr>>2))*tr/255,g=((cg<<3)|(cg>>2))*tg/255,b=((cb<<3)|(cb>>2))*tb/255;
   pixel(px,py,RGB(r,g,b));
  }
 }
 /* Family-specific corona bloom — cheap rings, additive, clipped. */
 unsigned corona=RGB((tint&255)/3,((tint>>8)&255)/3,((tint>>16)&255)/3);
 float pulse=.9f+.1f*sinf(time*(1.2f+(fam&3)*.15f)+seed);
 int bloom=(int)(radius*(1.25f+.12f*(fam%3))*pulse);
 int step=bloom>40?3:2;
 for(int ring=0;ring<4;ring++){
  int rr=bloom-ring*(bloom-radius)/4;if(rr<=radius)break;
  unsigned glow=RGB(((corona&255)*(4-ring))/5,(((corona>>8)&255)*(4-ring))/5,(((corona>>16)&255)*(4-ring))/5);
  int samples=32+ring*12;float spin=time*(.1f+.02f*fam)+seed*.01f;
  for(int k=0;k<samples;k+=1){
   float a=spin+k*6.2831853f/samples;
   sun_bloom_dot(cx+(int)(cosf(a)*rr),cy+(int)(sinf(a)*rr*.95f),glow,xt,yt,xb,yb);
  }
 }
 /* Hot / flare families get a few rotating ray tips outside the sheet. */
 if(fam==1||fam==2||fam==6){
  int rays=fam==6?5:8;float spin=time*(.2f+.05f*fam);
  for(int k=0;k<rays;k++){
   float a=spin+k*6.2831853f/rays;float reach=radius*(1.15f+.08f*sinf(time*2+k));
   int x1=cx+(int)(cosf(a)*reach),y1=cy+(int)(sinf(a)*reach);
   int x2=cx+(int)(cosf(a)*(radius+1)),y2=cy+(int)(sinf(a)*(radius+1));
   if(y1>=yt&&y1<=yb&&y2>=yt&&y2<=yb)line(x2,y2,x1,y1,RGB((tint&255)/2,((tint>>8)&255)/2,((tint>>16)&255)/2));
  }
 }
 (void)step;
}
/* The same immutable body seed selects art on charts, cards and in flight.
 * Only visible pixels are sampled, even beside a planet filling the screen.
 */
static void draw_planet_sprite(int cx,int cy,int r,unsigned seed,int type,int xt,int yt,int xb,int yb){
 if(r<1||type==SUN)return;
 int x=cx-r,y=cy-r,d=r*2;
 int left=x>xt?x:xt,top=y>yt?y:yt,right=x+d<xb?x+d:xb,bottom=y+d<yb?y+d:yb;
 if(left<0)left=0;
 if(top<0)top=0;
 if(right>W)right=W;
 if(bottom>H)bottom=H;
 const uint16_t *data=planet_sprites[planet_sprite_index(seed,type)];
 int tr=224+(int)((seed>>8)&31),tg=224+(int)((seed>>13)&31),tb=224+(int)((seed>>18)&31);
 for(int py=top;py<bottom;py++){int sy=2+(py-y)*60/d;
  for(int px=left;px<right;px++){int sx=2+(px-x)*60/d;if(seed&16)sx=63-sx;
   unsigned p=data[sy*64+sx];if(!(p&0x8000))continue;
   int cr=(p>>10)&31,cg=(p>>5)&31,cb=p&31;
   pixel(px,py,RGB(((cr<<3)|(cr>>2))*tr/255,((cg<<3)|(cg>>2))*tg/255,((cb<<3)|(cb>>2))*tb/255));
  }
 }
}
