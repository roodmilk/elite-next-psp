#include "next-art.h"
#include "campaign-art.h"
#include "sun-sprites.h"
#include "system-almanac-art.h"
#include "intro-art.h"
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
/* All families are luminous stars, never dark-centred anomaly silhouettes.
 * Existing animated sheets provide granulation only, not transparency or shading. */
static void draw_sun_sprite(int cx,int cy,int radius,unsigned tint,unsigned seed,float time,int xt,int yt,int xb,int yb){
 if(radius<1)return;
 int fam=sun_family(seed),frame=((int)(time*6.f+(seed&7))&3);
 const uint16_t *sheet=sun_sprites[fam][frame];
 int glow=radius+radius/3+2,left=cx-glow,top=cy-glow,right=cx+glow+1,bottom=cy+glow+1;
 if(left<xt)left=xt;if(left<0)left=0;if(top<yt)top=yt;if(top<0)top=0;
 if(right>xb)right=xb;if(right>W)right=W;if(bottom>yb+1)bottom=yb+1;if(bottom>H)bottom=H;
 int r2=radius*radius,g2=glow*glow;
 for(int y=top;y<bottom;y++)for(int x=left;x<right;x++){
  int dx=x-cx,dy=y-cy,q=dx*dx+dy*dy;if(q>g2)continue;
  if(q>r2){
   int strength=(g2-q)*58/(g2-r2);
   sun_bloom_dot(x,y,RGB(strength,strength*2/3,strength/6),xt,yt,xb,yb);continue;
  }
  int sx=(dx+radius)*31/(radius*2),sy=(dy+radius)*31/(radius*2);
  unsigned p=sheet[sy*32+sx];int grain=(((p>>10)&31)+((p>>5)&31)+(p&31))/3;
  int core=(r2-q)*55/r2,heat=grain/3+(fam%3)*5;
  pixel(x,y,RGB(255,165+core+heat,45+core*2+heat*2));
 }
 /* Short outward flares, clipped pixel by pixel so previews never bleed. */
 for(int k=0;k<9;k++){
  float a=k*6.2831853f/9+seed*.001f+time*.06f;
  int reach=radius/6+1+(int)((.5f+.5f*sinf(time*1.8f+k+fam))*radius*.12f);
  for(int n=0;n<reach;n++){
   int x=cx+(int)(cosf(a)*(radius+n)),y=cy+(int)(sinf(a)*(radius+n));
   int fade=(reach-n)*90/reach;sun_bloom_dot(x,y,RGB(fade,fade*2/3,fade/5),xt,yt,xb,yb);
  }
 }
 (void)tint;
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
 /* Resolve x sampling once and tint each source scanline once, not per screen pixel. */
 int sample[W];for(int px=left;px<right;px++){int sx=2+(px-x)*60/d;sample[px]=(seed&16)?63-sx:sx;}
 unsigned colours[64];int previous_sy=-1;
 for(int py=top;py<bottom;py++){int sy=2+(py-y)*60/d;
  if(sy!=previous_sy){previous_sy=sy;for(int sx=0;sx<64;sx++){
   unsigned p=data[sy*64+sx];if(!(p&0x8000)){colours[sx]=0;continue;}
   int cr=(p>>10)&31,cg=(p>>5)&31,cb=p&31;
   colours[sx]=RGB(((cr<<3)|(cr>>2))*tr/255,((cg<<3)|(cg>>2))*tg/255,((cb<<3)|(cb>>2))*tb/255)|0xff000000u;
  }}
  for(int px=left;px<right;px++){unsigned c=colours[sample[px]];if(c)pixel(px,py,c);}
 }
}
/* Inverse-map the billboard using the same screen-space roll as camera().
 * Cache the tiny tinted sheet; inner loop is additions and nearest sampling.
 * Menus retain draw_planet_sprite and never inherit the flight camera. */
static void draw_planet_sprite_rolled(int cx,int cy,int r,unsigned seed,int type,float roll,int xt,int yt,int xb,int yb){
 if(r<1||type==SUN)return;
 float c=cosf(roll),s=sinf(roll);
 if(fabsf(s)<.00001f&&c>.99999f){draw_planet_sprite(cx,cy,r,seed,type,xt,yt,xb,yb);return;}
 static unsigned palette[64*64],cached_seed;static int cached_type=-1;
 if(cached_type!=type||cached_seed!=seed){
  cached_type=type;cached_seed=seed;
  const uint16_t *data=planet_sprites[planet_sprite_index(seed,type)];
  int tr=224+((seed>>8)&31),tg=224+((seed>>13)&31),tb=224+((seed>>18)&31);
  for(int i=0;i<64*64;i++){
   unsigned p=data[i];int cr=(p>>10)&31,cg=(p>>5)&31,cb=p&31;
   palette[i]=(p&0x8000)?RGB(((cr<<3)|(cr>>2))*tr/255,((cg<<3)|(cg>>2))*tg/255,((cb<<3)|(cb>>2))*tb/255)|0xff000000u:0;
  }
 }
 int extent=(int)ceilf(r*(fabsf(c)+fabsf(s)));
 int left=cx-extent,right=cx+extent+1,top=cy-extent,bottom=cy+extent+1;
 if(left<xt)left=xt;if(left<0)left=0;if(right>xb)right=xb;if(right>W)right=W;
 if(top<yt)top=yt;if(top<0)top=0;if(bottom>yb)bottom=yb;if(bottom>H)bottom=H;
 float step=30.f/r,du=c*step,dv=-s*step;
 for(int py=top;py<bottom;py++){
  float u=32.f+((left-cx)*c+(py-cy)*s)*step;
  float v=32.f+(-(left-cx)*s+(py-cy)*c)*step;
  for(int px=left;px<right;px++,u+=du,v+=dv){
   if(u<2.f||u>=62.f||v<2.f||v>=62.f)continue;
   int sx=(int)u,sy=(int)v;if(seed&16)sx=63-sx;
   unsigned colour=palette[sy*64+sx];if(colour)pixel(px,py,colour);
  }
 }
}

