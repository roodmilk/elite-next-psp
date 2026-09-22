/* Hand-authored 8x8 silhouettes at 1:1 scale; transparent background.
 * Station, world, ship, unknown. No runtime resampling or atlas allocation. */
static const unsigned char hud_icons[4][8]={
 {0x18,0x7e,0x66,0xc3,0xc3,0x66,0x7e,0x18},
 {0x3c,0x66,0xc3,0xdb,0xdb,0xc3,0x66,0x3c},
 {0x18,0x18,0x3c,0x3c,0x7e,0xff,0x24,0x00},
 {0x3c,0x42,0x02,0x0c,0x10,0x00,0x10,0x00}
};
static void hud_pixel_icon(int x,int y,int type,unsigned ink){
 if(type<0||type>3)return;
 for(int j=0;j<8;j++)for(int i=0;i<8;i++)if(hud_icons[type][j]&(128>>i))pixel(x+i,y+j,ink);
}
/* Native 16x16 field silhouettes: alien bloom, leafback, mineral cluster.
 * A dark edge and palette-index highlights keep small shapes readable. */
static const uint16_t field_masks[3][16]={
 {0x0180,0x03c0,0x07e0,0x1ff8,0x3ffc,0x7ffe,0x3ffc,0x1ff8,0x0180,0x2184,0x33cc,0x1ff8,0x07e0,0x0180,0x0180,0x03c0},
 {0x0020,0x0060,0x00e0,0x01f0,0x03f0,0x1fe0,0x3fe0,0x7ff0,0xfff0,0x7fe0,0x3fc0,0x36c0,0x2240,0x2240,0x66c0,0x0000},
 {0x0200,0x0700,0x0700,0x0f90,0x0fb8,0x1ff8,0x1ff8,0x3ffc,0x3ffc,0x7ffe,0x7ffe,0x3ffc,0x3ffc,0x1ff8,0x0ff0,0x0000}
};
static void field_sprite(int cx,int bottom,int size,int kind,unsigned ink,int pose){
 if(size<4)size=4;
 if(size>48)size=48;
 unsigned shadow=RGB((ink&255)/3,((ink>>8)&255)/3,((ink>>16)&255)/3);
 unsigned highlight=RGB(((ink&255)+220)/2,(((ink>>8)&255)+228)/2,(((ink>>16)&255)+220)/2);
 for(int y=0;y<size;y++)for(int x=0;x<size;x++){
  int sy=y*16/size,sx=x*16/size;
  if(kind==1&&pose&&sy>=12)sx+=(sy&1)?1:-1;
  if(sx<0||sx>15||!(field_masks[kind][sy]&(0x8000u>>sx)))continue;
  int px=cx-size/2+x,py=bottom-size+y;if(py<view_top()||py>view_bot())continue;
  unsigned col=(sx+sy)%7==0?highlight:sx>9||sy>12?shadow:ink;
  if(kind==1&&sy==4&&sx==10)col=WHITE;
  pixel(px,py,col);
 }
}
/* World-space particles use this clipped pixel path, never raw UI rects. */
static void world_spark(int x,int y,int radius,unsigned ink){
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 for(int j=-radius;j<=radius;j++)for(int i=-radius;i<=radius;i++){
  if(x+i<0||x+i>=W||y+j<top||y+j>bot)continue;
  int dist=abs(i)+abs(j);if(dist>radius)continue;
  unsigned bg=fb[(y+j)*STRIDE+x+i];int alpha=(radius+1-dist)*112/(radius+1);
  int r=((bg&255)*(255-alpha)+(ink&255)*alpha)/255;
  int g=(((bg>>8)&255)*(255-alpha)+((ink>>8)&255)*alpha)/255;
  int b=(((bg>>16)&255)*(255-alpha)+((ink>>16)&255)*alpha)/255;
  pixel(x+i,y+j,RGB(r,g,b));
 }
}
