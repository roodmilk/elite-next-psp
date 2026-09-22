#include "next-art.h"
#include "campaign-art.h"
/* Assets are compiled ARGB1555. All sampling stays on the native pixel grid. */
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
/* System-coloured animated star. Same seed drives flight, cards and codex. */
static void draw_sun_sprite(int cx,int cy,int radius,unsigned tint,unsigned seed,float time,int xt,int yt,int xb,int yb){
 if(radius<1)return;
 int style=seed&7; /* 8 looks: yellow dwarf, blue, white, red giant, etc. via tint */
 unsigned corona=RGB((tint&255)*3/5,((tint>>8)&255)*3/5,((tint>>16)&255)*3/5);
 int cr=(tint&255)+40,cg=((tint>>8)&255)+30,cb=((tint>>16)&255)+20;if(cr>255)cr=255;if(cg>255)cg=255;if(cb>255)cb=255;unsigned core=RGB(cr,cg,cb);
 float spin=time*(.12f+.03f*(style&3))+(seed&255)*.01f;
 float pulse=.92f+.08f*sinf(time*(1.1f+(style&3)*.2f)+seed);
 /* Soft bloom rings — cheap concentric dots, no full-frame blur. */
 int bloom_r=(int)(radius*(1.35f+.15f*(style%3))*pulse);
 for(int ring=0;ring<3;ring++){
  int rr=bloom_r-ring*(bloom_r-radius)/3;if(rr<=radius)break;
  unsigned glow=RGB(((corona&255)*(3-ring))/4,(((corona>>8)&255)*(3-ring))/4,(((corona>>16)&255)*(3-ring))/4);
  for(int k=0;k<48+ring*16;k++){float a=k*6.2831853f/(48+ring*16)+spin*.2f;int x=cx+(int)(cosf(a)*rr),y=cy+(int)(sinf(a)*rr*.92f);if(x>=xt&&x<xb&&y>=yt&&y<=yb)pixel(x,y,glow);}
 }
 for(int k=0;k<80;k++){float a=k*6.2831853f/80+spin*.15f;float r=radius+2+(k%4)+2*sinf(time*2+k);int x=cx+(int)(cosf(a)*r),y=cy+(int)(sinf(a)*r);if(x>=xt&&x<xb&&y>=yt&&y<=yb)pixel(x,y,corona);}
 int rays=10+style*2;for(int k=0;k<rays;k++){
  float a=spin+k*6.2831853f/rays;float reach=radius*(1.1f+.06f*((k*5+style)%4)+.04f*sinf(time*1.7f+k));
  int x0=cx+(int)(cosf(a)*(radius+1)),y0=cy+(int)(sinf(a)*(radius+1)),x1=cx+(int)(cosf(a)*reach),y1=cy+(int)(sinf(a)*reach);
  if(y0>=yt&&y0<=yb&&y1>=yt&&y1<=yb)line(x0,y0,x1,y1,corona);
 }
 if(radius>10)for(int k=0;k<2+(style&3);k++){
  float a=spin*1.3f+k*2.1f+(seed&31)*.07f;int r=radius;
  int x0=cx+(int)(cosf(a)*r),y0=cy+(int)(sinf(a)*r),xm=cx+(int)(cosf(a+.12f)*(r+5+(style&3)*2)),ym=cy+(int)(sinf(a+.12f)*(r+5+(style&3)*2)),x1=cx+(int)(cosf(a+.25f)*r),y1=cy+(int)(sinf(a+.25f)*r);
  if(y0>=yt&&y0<=yb&&ym>=yt&&ym<=yb&&y1>=yt&&y1<=yb){line(x0,y0,xm,ym,corona);line(xm,ym,x1,y1,corona);}
 }
 int y0=yt>(int)(cy-radius)?yt:(int)(cy-radius),y1=yb<(int)(cy+radius)?yb:(int)(cy+radius);
 for(int y=y0;y<=y1;y++){
  float yy=(y-cy)/(float)radius,z=sqrtf(fmaxf(0,1-yy*yy));int width=(int)(radius*z),x0=xt>(cx-width)?xt:cx-width,x1=(xb-1)<(cx+width)?(xb-1):cx+width;
  for(int x=x0;x<=x1;x+=2){
   float xx=(x-cx)/(float)radius;float centre=fmaxf(0,1-xx*xx-yy*yy);
   int grain=((x-cx+radius)*24/(radius?radius:1)+(y-cy+radius)*19/(radius?radius:1)*7+(int)(seed&15))&7;
   float plasma=.90f+.10f*sinf((x+y*2+(int)(seed&255))*0.19f+time*(.35f+.05f*(style&3)));
   float shade=(.62f+.38f*centre)*(grain==0?.84f:grain==3?1.08f:1.f)*plasma*pulse;
   unsigned c=centre>.92f?core:tint;
   int rr=(int)((c&255)*shade),gg=(int)(((c>>8)&255)*shade),bb=(int)(((c>>16)&255)*shade);
   if(rr>255)rr=255;if(gg>255)gg=255;if(bb>255)bb=255;
   rect(x,y,x<x1?2:1,1,RGB(rr,gg,bb));
  }
 }
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
