#include "freighter-art-tiles.h"
#include "listening-post-art-tiles.h"
#include "wreck-art-tiles.h"
static unsigned freight_art_ink(unsigned base,int index){
 /* Capital hulls need broad readable metal, not black wireframe recesses. */
 static const unsigned short gains[16]={200,256,285,310,235,256,160,128,180,300,290,260,180,280,240,220};
 if(index==5||index==7||index==8||index==9||index==10||index==11)return ship_art_ink(base,index);
 int k=gains[index],r=(base&255)*k/256,g=((base>>8)&255)*k/256,b=((base>>16)&255)*k/256;
 return RGB(r>255?255:r,g>255?255:g,b>255?255:b);
}
static void freight_art_triangle(DrawTri*t){
 Point a=t->p[0],b=t->p[1],c=t->p[2];float area=edge(a,b,c.x,c.y);if(fabsf(area)<.05f)return;
 int material=t->material-5000,tile=material/6,second=material&1,mip=(material%6)/2,listening=tile>=6;
 if(listening){tile-=6;mip=0;}
 int size=mip==0?32:mip==1?16:8,offset=mip==0?0:mip==1?512:640;
 float ia=1/(a.z*area),ib=1/(b.z*area),ic=1/(c.z*area);
 int left=clipy0>=0?clipx0:0,right=clipy0>=0?clipx1-1:W-1,top=clipy0>=0?clipy0:view_top(),bot=clipy0>=0?clipy1-1:view_bot();
 int x0=(int)surface_max(left,surface_floor(surface_min(a.x,surface_min(b.x,c.x)))),x1=(int)surface_min(right,surface_ceil(surface_max(a.x,surface_max(b.x,c.x))));
 int y0=(int)surface_max(top,surface_floor(surface_min(a.y,surface_min(b.y,c.y)))),y1=(int)surface_min(bot,surface_ceil(surface_max(a.y,surface_max(b.y,c.y))));
 static const unsigned listen_palette[16]={0x00483d36,0x00685b51,0x009b8d7d,0x00291e17,0x00342b25,0x00201815,0x006b6259,0x0044aef2,0x0067cef8,0x003d769e,0x004ea7dc,0x00c6c23f,0x001a1310,0x005f5147,0x008d8175,0x00a69b8c};
 unsigned palette[16];for(int i=0;i<16;i++)palette[i]=listening?listen_palette[i]:freight_art_ink(t->color,i);
 for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){
  float ea=edge(b,c,x+.5f,y+.5f),eb=edge(c,a,x+.5f,y+.5f),ec=edge(a,b,x+.5f,y+.5f);
  if(!((ea>=0&&eb>=0&&ec>=0)||(ea<=0&&eb<=0&&ec<=0)))continue;
  float z=ea*ia+eb*ib+ec*ic;if(z<=0)continue;
  float u=(second?eb*ib:(eb*ib+ec*ic))/z,v=(second?(eb*ib+ec*ic):ec*ic)/z;
  int tx=(int)(u*(size-1)),ty=(int)(v*(size-1));if(tx<0)tx=0;if(tx>=size)tx=size-1;if(ty<0)ty=0;if(ty>=size)ty=size-1;
  int index=ty*size+tx,packed=listening?listening_post_art_tiles[tile][index>>1]:freight_art_tiles[tile][offset+(index>>1)];fb[y*STRIDE+x]=palette[(packed>>((index&1)*4))&15];
 }
}

static void wreck_art_triangle(DrawTri*t){
 Point a=t->p[0],b=t->p[1],c=t->p[2];float area=edge(a,b,c.x,c.y);if(fabsf(area)<.05f)return;
 int material=t->material-5100,tile=material/6,second=material&1,mip=(material%6)/2;
 int size=mip==0?32:mip==1?16:8,offset=mip==0?0:mip==1?512:640;
 float ia=1/(a.z*area),ib=1/(b.z*area),ic=1/(c.z*area);
 int left=clipy0>=0?clipx0:0,right=clipy0>=0?clipx1-1:W-1,top=clipy0>=0?clipy0:view_top(),bot=clipy0>=0?clipy1-1:view_bot();
 int x0=(int)surface_max(left,surface_floor(surface_min(a.x,surface_min(b.x,c.x)))),x1=(int)surface_min(right,surface_ceil(surface_max(a.x,surface_max(b.x,c.x))));
 int y0=(int)surface_max(top,surface_floor(surface_min(a.y,surface_min(b.y,c.y)))),y1=(int)surface_min(bot,surface_ceil(surface_max(a.y,surface_max(b.y,c.y))));
 unsigned palette[16];for(int i=0;i<16;i++)palette[i]=freight_art_ink(t->color,i);
 for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){
  float ea=edge(b,c,x+.5f,y+.5f),eb=edge(c,a,x+.5f,y+.5f),ec=edge(a,b,x+.5f,y+.5f);
  if(!((ea>=0&&eb>=0&&ec>=0)||(ea<=0&&eb<=0&&ec<=0)))continue;
  float z=ea*ia+eb*ib+ec*ic;if(z<=0)continue;
  float u=(second?eb*ib:(eb*ib+ec*ic))/z,v=(second?(eb*ib+ec*ic):ec*ic)/z;
  int tx=(int)(u*(size-1)),ty=(int)(v*(size-1));if(tx<0)tx=0;if(tx>=size)tx=size-1;if(ty<0)ty=0;if(ty>=size)ty=size-1;
  int index=ty*size+tx,packed=wreck_art_tiles[tile][offset+(index>>1)];fb[y*STRIDE+x]=palette[(packed>>((index&1)*4))&15];
 }
}
