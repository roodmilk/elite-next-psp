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
