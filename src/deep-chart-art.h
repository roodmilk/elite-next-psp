/* Deep Chart glass and diffuse galactic light. Only the system renderer draws
 * point lights: this background must never masquerade as selectable stars.
 * Embedded 256 KiB RGB565 art; no runtime files, generation or allocation. */
#include "generated/deep-chart-nebula.h"
/* Native 5x7 lettering. Integer scale only: no duplicated, uneven strokes. */
static const unsigned char chart_letters[36][7]={
 {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},
 {30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
 {14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{31,4,4,4,4,4,31},
 {7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
 {17,27,21,21,17,17,17},{17,25,25,21,19,19,17},{14,17,17,17,17,17,14},
 {30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
 {15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
 {17,17,17,17,17,10,4},{17,17,17,21,21,27,17},{17,17,10,4,10,17,17},
 {17,17,10,4,4,4,4},{31,1,2,4,8,16,31},
 {14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},
 {30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
 {14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},{14,17,17,15,1,1,14}
};
static void chart_type(int x,int y,unsigned ink,int scale,const char *s){
 for(int i=0;s[i];i++,x+=5*scale+2){
  int ch=s[i],index=ch>='A'&&ch<='Z'?ch-'A':ch>='0'&&ch<='9'?26+ch-'0':-1;
  for(int row=0;row<7;row++)for(int col=0;col<5;col++){
   int on=index>=0?(chart_letters[index][row]&(16>>col))!=0:
    ch=='/'?col==4-row*5/7:ch=='.'?row>=5&&col==2:ch=='-'?row==3:ch==':'?(row==2||row==5)&&col==2:0;
   if(on){if(scale==1)pixel(x+col,y+row,ink);else rect(x+col*scale,y+row*scale,scale,scale,ink);}
  }
 }
}
static void chart_text(int x,int y,unsigned ink,const char *fmt,...){
 char out[96];va_list args;va_start(args,fmt);vsnprintf(out,sizeof(out),fmt,args);va_end(args);chart_type(x,y,ink,1,out);
}
static unsigned chart_decode(unsigned c){
 int r=(c>>11)&31,g=(c>>5)&63,b=c&31;
 return RGB((r<<3)|(r>>2),(g<<2)|(g>>4),(b<<3)|(b>>2));
}
static unsigned chart_lerp(unsigned a,unsigned b,int t){
 int s=256-t;
 /* Red/blue occupy independent 16-bit lanes, so interpolate together. */
 unsigned rb=((((a&0x00ff00ffu)*s)+((b&0x00ff00ffu)*t))>>8)&0x00ff00ffu;
 unsigned g=((((a&0x0000ff00u)*s)+((b&0x0000ff00u)*t))>>8)&0x0000ff00u;
 return 0xff000000u|rb|g;
}
static void chart_galaxy_background(void){
 int step=(int)(256.f/chart_zoom),origin=(int)((256.f+(-250.f-chart_pan_x)/chart_zoom)*256.f);
 /* Diffuse light has no one-pixel detail. A two-pixel shading rate keeps the
  * background inexpensive while all stars, text and lines remain native. */
 for(int y=25;y<246;y+=2){
  int sy=(int)((128.f+(y-144.f-chart_pan_y)/chart_zoom)*256.f),iy=sy>>8,ty=sy&255;
  for(int x=2,sx=origin+step*2;x<478;x+=2,sx+=step*2){
   int ix=sx>>8,tx=sx&255;unsigned c=RGB(2,5,11);
   if(ix>=0&&ix<511&&iy>=0&&iy<255){unsigned u=chart_lerp(chart_decode(chart_nebula[iy][ix]),chart_decode(chart_nebula[iy][ix+1]),tx),v=chart_lerp(chart_decode(chart_nebula[iy+1][ix]),chart_decode(chart_nebula[iy+1][ix+1]),tx);c=chart_lerp(u,v,ty);}
   fb[y*STRIDE+x]=fb[y*STRIDE+x+1]=c;
   if(y+1<246)fb[(y+1)*STRIDE+x]=fb[(y+1)*STRIDE+x+1]=c;
  }
 }
}
static void chart_add_light(int x,int y,unsigned c,int alpha){
 if(x<2||x>=478||y<25||y>=246)return;
 if(clipy0>=0&&(x<clipx0||x>=clipx1||y<clipy0||y>=clipy1))return;
 unsigned b=fb[y*STRIDE+x];int r=(b&255)+((c&255)*alpha>>8),g=((b>>8)&255)+(((c>>8)&255)*alpha>>8),bl=((b>>16)&255)+(((c>>16)&255)*alpha>>8);
 if(r>255)r=255;if(g>255)g=255;if(bl>255)bl=255;fb[y*STRIDE+x]=RGB(r,g,bl);
}
static void chart_halo(int x,int y,int radius,unsigned ink){
 if(x+radius<2||x-radius>=478||y+radius<25||y-radius>=246)return;
 int rr=radius*radius;
 for(int dy=-radius;dy<=radius;dy++)for(int dx=-radius;dx<=radius;dx++){
  int d=dx*dx+dy*dy;if(d>=rr)continue;int a=(rr-d)*128/rr;chart_add_light(x+dx,y+dy,ink,a*a/128);
 }
}
static void chart_glow(int x,int y,unsigned core,int selected){
 chart_halo(x,y,selected?11:8,core);
 for(int i=-5;i<=5;i++){int alpha=(6-abs(i))*20;chart_add_light(x+i,y,core,alpha);chart_add_light(x,y+i,core,alpha);}
 rect(x-1,y-1,3,3,core);pixel(x,y,WHITE);
}
static void chart_glass(int x,int y,int w,int h,unsigned edge){
 for(int yy=1;yy<h-1;yy++)rect(x+1,y+yy,w-2,1,RGB(3,12+(h-yy)*5/h,21+(h-yy)*6/h));
 line(x+3,y,x+w-4,y,edge);line(x+3,y+h-1,x+w-4,y+h-1,edge);
 line(x,y+3,x,y+h-4,edge);line(x+w-1,y+3,x+w-1,y+h-4,edge);
 line(x,y+3,x+3,y,edge);line(x+w-4,y,x+w-1,y+3,edge);
 line(x,y+h-4,x+3,y+h-1,edge);line(x+w-4,y+h-1,x+w-1,y+h-4,edge);
}
/* Label rectangles are kept clear of the fixed panels and of one another.
 * Text drawing itself does not obey preview_clip, so constrain it explicitly. */
static int chart_label_boxes[4][4],chart_label_n;
static int chart_box_overlap(int x,int y,int w,int h,int xx,int yy,int ww,int hh){return x<xx+ww&&x+w>xx&&y<yy+hh&&y+h>yy;}
static int chart_label_free(int x,int y,int w){
 if(x<5||x+w>475||y<27||y+14>228)return 0;
 if(chart_box_overlap(x,y,w,14,6,29,95,91)||chart_box_overlap(x,y,w,14,342,29,132,96))return 0;
 for(int i=0;i<chart_label_n;i++)if(chart_box_overlap(x,y,w,14,chart_label_boxes[i][0]-3,chart_label_boxes[i][1]-2,chart_label_boxes[i][2]+6,18))return 0;
 return 1;
}
static void chart_label(int id,unsigned ink){
 int x,y;galaxy_xy(id,&x,&y);if(x<3||x>476||y<27||y>239)return;
 int w=(int)strlen(game.systems[id].name)*7+6,xx=0,yy=0,found=0;
 for(int i=0;i<8;i++){
  xx=i&1?x-w-9:x+9;yy=y-6+(i/2==1?17:i/2==2?-17:i/2==3?31:0);
  if(chart_label_free(xx,yy,w)){found=1;break;}
 }
 if(!found)return;
 for(int dy=0;dy<14;dy++)for(int dx=0;dx<w;dx++){unsigned c=fb[(yy+dy)*STRIDE+xx+dx];fb[(yy+dy)*STRIDE+xx+dx]=chart_lerp(c,RGB(2,8,16),180);}
 chart_text(xx+3,yy+2,ink,"%s",game.systems[id].name);
 if(chart_label_n<4){int *box=chart_label_boxes[chart_label_n++];box[0]=xx;box[1]=yy;box[2]=w;box[3]=14;}
}
