#define FLEET_QUAD_BASE 65536
#include "erl-racer-pixels.h"
#include "ship-art-panels.h"
#include "ship-art-atlas.h"
#include "fleet-detail-faces.h"
#include "fleet-detail-atlas.h"
static unsigned char (*ship_art_pixels)[512];
static int ship_art_unpack(void){
 if(ship_art_pixels)return 1;unsigned char*p=malloc(26624);if(!p)return 0;unsigned read=0,written=0;
 while(read<sizeof(ship_art_packed)){unsigned token=ship_art_packed[read++],kind=token>>6,count=(token&63)+(kind?3:1);if(count>26624-written)goto bad;
  if(!kind){if(count>sizeof(ship_art_packed)-read)goto bad;memcpy(p+written,ship_art_packed+read,count);read+=count;}
  else if(kind==1){if(read>=sizeof(ship_art_packed))goto bad;memset(p+written,ship_art_packed[read++],count);}
  else {if((kind==3?2u:1u)>sizeof(ship_art_packed)-read)goto bad;unsigned distance=ship_art_packed[read++]+1;if(kind==3)distance+=ship_art_packed[read++]<<8;if(distance>written)goto bad;for(unsigned i=0;i<count;i++)p[written+i]=p[written+i-distance];}
  written+=count;
 }if(written!=26624)goto bad;ship_art_pixels=(void*)p;return 1;
 bad:free(p);return 0;
}

static unsigned ship_art_ink(unsigned base,int texel){
 int r=base&255,g=(base>>8)&255,b=(base>>16)&255;

 const int gain[16]={65,130,242,275,292,180,60,75,130,300,260,230,110,265,210,155};
 int k=gain[texel];r=r*k/256;g=g*k/256;b=b*k/256;
 if(texel==5){r=210;g=150;b=65;}
 if(texel==7||texel==8){r=15;g=texel==7?45:85;b=texel==7?65:105;}
 if(texel==9||texel==10||texel==11){r=texel==9?180:45;g=texel==11?145:220;b=240;}
 if(texel==12){r=70;g=49;b=39;}
 /* Glazing, scorch and caution paint receive hull light; lamps stay emissive. */
 if(texel==5||texel==7||texel==8||texel==12){
  int shade=80+((base&255)*54+((base>>8)&255)*183+((base>>16)&255)*19)/256;
  if(shade>256)shade=256;r=r*shade/256;g=g*shade/256;b=b*shade/256;
 }
 return RGB(r>255?255:r,g>255?255:g,b>255?255:b);
}
static void ship_art_triangle(DrawTri*t){
 Point a=t->p[0],b=t->p[1],c=t->p[2];float area=edge(a,b,c.x,c.y);if(fabsf(area)<.05f)return;
 float ia=1/(a.z*area),ib=1/(b.z*area),ic=1/(c.z*area);
 int x0=(int)fmaxf(clipx0,floorf(fminf(a.x,fminf(b.x,c.x)))),x1=(int)fminf(clipx1-1,ceilf(fmaxf(a.x,fmaxf(b.x,c.x))));
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 int y0=(int)fmaxf(top,floorf(fminf(a.y,fminf(b.y,c.y)))),y1=(int)fminf(bot,ceilf(fmaxf(a.y,fmaxf(b.y,c.y))));
 int racer=t->material>=7800&&t->material<7804;
 int fleet=t->material>=8192&&t->material<FLEET_QUAD_BASE,quad=t->material>=FLEET_QUAD_BASE||racer;
 int face=t->material-(fleet?8192:4096),tile=quad?(t->material-FLEET_QUAD_BASE)/2:0;
 static const unsigned char quv[2][6]={{0,0,31,0,31,31},{0,0,31,31,0,31}};
 const unsigned char*uv=fleet?fleet_faces[face].uv:quad?quv[(t->material-FLEET_QUAD_BASE)&1]:ship_art_uv[face];
 if(fleet)tile=fleet_faces[face].tile;if(racer)tile=(t->material-7800)/2;
 /* Minify fine panel detail before it becomes flickering subpixel noise. */
 float span=fmaxf(fabsf(a.x-b.x)+fabsf(a.y-b.y),fmaxf(fabsf(b.x-c.x)+fabsf(b.y-c.y),fabsf(c.x-a.x)+fabsf(c.y-a.y)));
 int mip=(fleet||quad)?(span<12?2:span<25?1:0):0;
 int size=32>>mip,offset=mip==0?0:mip==1?512:640;
 static unsigned racer_palette[16],racer_color;unsigned local_palette[16],*palette=racer?racer_palette:local_palette;
 if(!racer||racer_color!=t->color){if(racer)racer_color=t->color;for(int i=0;i<16;i++){
  palette[i]=ship_art_ink(t->color,i);
  /* Painted white reflections preserve each team's lacquer underneath.
   * Restricted to racer materials; fleet/spaceflight art is unchanged. */
  if(racer&&(i==3||i==4||i==13||i==14)){int k=i==3?28:i==4?72:i==13?140:220;unsigned b=t->color;
   palette[i]=RGB(((b&255)*(256-k)+255*k)>>8,(((b>>8)&255)*(256-k)+255*k)>>8,(((b>>16)&255)*(256-k)+255*k)>>8);}
 }}
 /* Racer-only incremental edges: preserve native pixels, perspective paint
  * and the existing coverage tolerance. Reject hidden pixels before UV work.
  * Spacecraft and fleet rendering retain their established path below. */
 if(racer){
  float dx0=c.y-b.y,dx1=a.y-c.y,dx2=b.y-a.y;
  float zi=dx0*ia+dx1*ib+dx2*ic;
  for(int y=y0;y<=y1;y++){
   float e0=edge(b,c,x0+.5f,y+.5f),e1=edge(c,a,x0+.5f,y+.5f),e2=edge(a,b,x0+.5f,y+.5f);
   float inv=e0*ia+e1*ib+e2*ic;
   for(int x=x0;x<=x1;x++,e0+=dx0,e1+=dx1,e2+=dx2,inv+=zi){
    if(!((e0>=-.05f&&e1>=-.05f&&e2>=-.05f)||(e0<=.05f&&e1<=.05f&&e2<=.05f))||inv<=0)continue;
    unsigned depth=surface_depth_on?(neon_depth_mode?neon_encode_depth(inv):surface_encode_depth(inv)):0;if(surface_depth_on&&depth>surface_depth[y*W+x])continue;
    float reciprocal=1.f/inv;
    int u=(int)((e0*ia*uv[0]+e1*ib*uv[2]+e2*ic*uv[4])*reciprocal),v=(int)((e0*ia*uv[1]+e1*ib*uv[3]+e2*ic*uv[5])*reciprocal);
    if(u<0)u=0;if(u>31)u=31;if(v<0)v=0;if(v>31)v=31;
    int at=(v>>mip)*size+(u>>mip);unsigned char packed=erl_racer_pixels[tile][offset+(at>>1)];
    fb[y*STRIDE+x]=palette[(packed>>((at&1)*4))&15];if(surface_depth_on)surface_depth[y*W+x]=depth;
   }
  }return;
 }
 /* Incremental perspective planes retain the same UV/depth mapping while
  * removing repeated edge calls and redundant per-pixel divisions. */
 float dx0=c.y-b.y,dx1=a.y-c.y,dx2=b.y-a.y;
 float di=dx0*ia+dx1*ib+dx2*ic;
 float ui[3]={ia*uv[0],ib*uv[2],ic*uv[4]},vi[3]={ia*uv[1],ib*uv[3],ic*uv[5]};
 float du=dx0*ui[0]+dx1*ui[1]+dx2*ui[2],dv=dx0*vi[0]+dx1*vi[1]+dx2*vi[2];
 for(int y=y0;y<=y1;y++){
  float e0=edge(b,c,x0+.5f,y+.5f),e1=edge(c,a,x0+.5f,y+.5f),e2=edge(a,b,x0+.5f,y+.5f);
  float inv=e0*ia+e1*ib+e2*ic,uq=e0*ui[0]+e1*ui[1]+e2*ui[2],vq=e0*vi[0]+e1*vi[1]+e2*vi[2];
  for(int x=x0;x<=x1;x++,e0+=dx0,e1+=dx1,e2+=dx2,inv+=di,uq+=du,vq+=dv){
   if(!((e0>=-.05f&&e1>=-.05f&&e2>=-.05f)||(e0<=.05f&&e1<=.05f&&e2<=.05f))||inv<=0)continue;
   if(surface_depth_on){unsigned depth=surface_encode_depth(inv);if(depth>surface_depth[y*W+x])continue;surface_depth[y*W+x]=depth;}
   float reciprocal=1.f/inv;int u=(int)(uq*reciprocal),v=(int)(vq*reciprocal);if(u<0)u=0;if(u>31)u=31;if(v<0)v=0;if(v>31)v=31;
   int at=(v>>mip)*size+(u>>mip);unsigned char packed=(fleet||quad)?fleet_tiles[tile][offset+(at>>1)]:ship_art_pixels[ship_art_panel[face]][at>>1];
   fb[y*STRIDE+x]=palette[(packed>>((at&1)*4))&15];
  }
 }
}
