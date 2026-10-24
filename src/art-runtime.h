#include "planet-phase.h"
#include "next-art.h"
#include "campaign-art.h"
#include "sun-sprites.h"
#include "system-almanac-art.h"
#include "intro-art.h"
/* Assets are compiled ARGB1555. All sampling stays on the native pixel grid. */
static unsigned sun_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static void draw_next_art(const uint16_t *data,int sw,int sh,int x,int y,int w,int h){
 if(w<=0||h<=0||sw<=0||sh<=0)return;
 int left=x<0?-x:0,right=w<W-x?w:W-x,top=y<0?-y:0,bottom=h<H-y?h:H-y;
 if(left>=right||top>=bottom)return;
 /* Native artwork needs no resampling. Scaled artwork computes its X samples
  * once per draw, not one PSP integer divide for every destination pixel.
  * The clipped map is bounded by the physical 480px screen, with no heap. */
 int native=w==sw&&h==sh;unsigned short samples[W];
 if(!native)for(int dx=left;dx<right;dx++)samples[dx-left]=dx*sw/w;
 for(int dy=top;dy<bottom;dy++){int yy=y+dy,sy=native?dy:dy*sh/h;const uint16_t *source=data+sy*sw;unsigned *dest=fb+yy*STRIDE+x+left;
  for(int dx=left;dx<right;dx++,dest++){unsigned p=source[native?dx:samples[dx-left]];if(!(p&0x8000))continue;
   int r=(p>>10)&31,g=(p>>5)&31,b=p&31;
   *dest=RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));
  }
 }
}
static int planet_sprite_index(unsigned seed,int type){
 static const int rocky[]={2,3,4,7};
 return type==OCEAN?(int)(seed%2):type==GAS?5+(int)(seed%2):rocky[seed%4];
}
static unsigned planet_rgb_mix(unsigned a,unsigned b,int t){
 int ar=a&255,ag=(a>>8)&255,ab=(a>>16)&255,br=b&255,bg=(b>>8)&255,bb=(b>>16)&255;
 if(t<0)t=0;if(t>255)t=255;
 return RGB((ar*(255-t)+br*t)/255,(ag*(255-t)+bg*t)/255,(ab*(255-t)+bb*t)/255);
}
static unsigned planet_lush_grade(unsigned c,int amount,int lift){
 int r=c&255,g=(c>>8)&255,b=(c>>16)&255;
 int grey=(r*77+g*150+b*29)>>8;
 r=grey+(r-grey)*amount/100+lift;
 g=grey+(g-grey)*amount/100+lift;
 b=grey+(b-grey)*amount/100+lift;
 if(r<0)r=0;if(r>255)r=255;if(g<0)g=0;if(g>255)g=255;if(b<0)b=0;if(b>255)b=255;
 return RGB(r,g,b);
}
static int planet_noise(unsigned seed,int x,int y,int scale){
 int gx=x/scale,gy=y/scale,fx=x%scale,fy=y%scale;
 unsigned a=sun_hash(seed^(unsigned)gx*0x9e3779b9u^(unsigned)gy*0x85ebca6bu);
 unsigned b=sun_hash(seed^(unsigned)(gx+1)*0x9e3779b9u^(unsigned)gy*0x85ebca6bu);
 unsigned c=sun_hash(seed^(unsigned)gx*0x9e3779b9u^(unsigned)(gy+1)*0x85ebca6bu);
 unsigned d=sun_hash(seed^(unsigned)(gx+1)*0x9e3779b9u^(unsigned)(gy+1)*0x85ebca6bu);
 int u=fx*255/scale,v=fy*255/scale;u=u*u*(765-2*u)/65025;v=v*v*(765-2*v)/65025;
 int ab=((int)(a&255)*(255-u)+(int)(b&255)*u)/255,cd=((int)(c&255)*(255-u)+(int)(d&255)*u)/255;
 return (ab*(255-v)+cd*v)/255;
}
/* Deterministic native-resolution world painter. It treats the 64x64 tile as
 * a tiny authored canvas: seed-selected palettes, warped multi-scale land,
 * caps, clouds, storms, bands and craters. No two bodies share only a flipped
 * source sprite, while the same body remains identical in every interface. */
static unsigned planet_proc_pixel(unsigned seed,int type,int sx,int sy){
 float nx=(sx-31.5f)/30.f,ny=(sy-31.5f)/30.f,q=nx*nx+ny*ny;if(q>1)return 0;
 unsigned style=sun_hash(seed^0x6c8e9cf5u);
 float z=sqrtf(fmaxf(0,1-q));int u=(int)((nx+z*((seed>>3)&7)*.085f+1.4f)*96),v=(int)((ny+1.15f)*80);
 int n=(planet_noise(seed,u,v,23)*3+planet_noise(seed^0x51ed270bu,u,v,11)*2+planet_noise(seed^0xa24baed5u,u,v,5))/6;
 static const unsigned ocean[][3]={
  {0x883410,0x4b8e3c,0xd6d2a8},{0x963f18,0x387c70,0xe8e2ce},{0x7c4b20,0x638f42,0xc4dba8},{0xa45d24,0x397f90,0xf0ddd0},
  {0x5e284b,0x496e9b,0xd5b6dc},{0x7b6331,0x689e75,0xd8e6bd},{0x9b5830,0x786547,0xe0bd88},{0x73325f,0x568fb2,0xbadcf1}};
 static const unsigned rock[][3]={
  {0x241b35,0x493756,0xc04f22},{0x315d82,0x4b83aa,0xd8e9ee},{0x654638,0x936044,0xd18b63},{0x3d5e4b,0x6f8762,0xb9b185},
  {0x49396f,0x72579a,0xbe8cad},{0x474c55,0x777e82,0xc3bca9},{0x23334a,0x406b78,0x72c9c3},{0x3a4f27,0x64803c,0xa9bd55},
  {0x32295d,0x614d91,0xb783cf},{0x573028,0x98483b,0xe07b55}};
 static const unsigned gas[][3]={
  {0x81542d,0xc98f55,0xf0d5a0},{0x355f8b,0x65a5c7,0xc8e1e0},{0x604777,0xa276a8,0xdec3c6},{0x3a796c,0x75b79b,0xd1d39b},
  {0x7c4735,0xbd7658,0xe3b780},{0x39275f,0x7451a0,0xd19ccf},{0x24495d,0x428da1,0x8ee3d0},{0x4a5530,0x84933d,0xd2ce6d},
  {0x52353d,0x8f4d67,0xe18aa1}};
 unsigned c;const WorldType *world=&world_types[world_type_seed(seed,type)];unsigned wp[3]={world->ground,world->accent,world_mix(world->ground,WORLD_RGB(236,228,198),.58f)};
 if(type==GAS){
  int theme=style%9;const unsigned *p=wp;(void)gas;float freq=8.f+(style>>8)%8;int bands=(int)(sinf((ny*freq+((style>>12)&15))*1.3f)*36)+n/4;
  c=planet_rgb_mix(p[0],p[1],bands+92);if(((sy+(style>>5))%(13+(style&7)))<3)c=planet_rgb_mix(c,p[2],105);
  int storms=1+((style>>24)&1);for(int i=0;i<storms;i++){unsigned h=sun_hash(style+i*911u);int ex=16+(h%33),ey=15+((h>>8)%35),dx=sx-ex,dy=sy-ey;if(dx*dx*2+dy*dy<24+(int)((h>>17)&15))c=planet_rgb_mix(c,p[2],160);}
 }else if(type==OCEAN){
  int theme=style%8;wp[0]=world->water;wp[1]=world->ground;const unsigned *p=wp;(void)ocean;int coast=103+(int)((style>>12)&47)-15;
  c=n>coast?planet_rgb_mix(p[1],p[2],(n-coast)*3):planet_rgb_mix(p[0],p[1],n/5);
  /* Broken, noisy polar shelves avoid the flat solid-colour strips that read
   * as a clipped texture at small Codex sizes. */
  int cap=(int)(fabsf(ny)*255),capline=(theme==7||theme==1)?170:210-((style>>17)&23);
  int polar_noise=planet_noise(seed^0x7f4a7c15u,u+31,v-19,9);
  if(cap+polar_noise/7>capline){int ice=(cap+polar_noise/7-capline)*7;if(ice>150)ice=150;c=planet_rgb_mix(c,theme==4?0xe6d8f1:0xe8f2ee,ice);}
  int cloud=planet_noise(seed^0xd1b54a35u,u+17,v,7+(theme&3));if(cloud>178+theme*2&&theme!=2)c=planet_rgb_mix(c,theme==4?0xe7c9ee:0xe8eff0,(cloud-175)*2);
 }else{
  int theme=style%10;const unsigned *p=wp;(void)rock;c=n<105?planet_rgb_mix(p[0],p[1],n*2):planet_rgb_mix(p[1],p[2],(n-105)*2);
  if(theme==0&&abs(n-126)<7)c=planet_rgb_mix(c,0x39a0ff,205); /* molten fault lines */
  if(theme==1&&(int)(fabsf(ny)*255)>155)c=planet_rgb_mix(c,0xe7f3f3,180); /* ice world */
  if(theme==6&&n>165)c=planet_rgb_mix(c,0x9debd7,120); /* crystalline shelves */
  if(theme==7){int haze=planet_noise(style,u,v,14);if(haze>150)c=planet_rgb_mix(c,0x62bd70,(haze-150));}
  int crater_count=2+(seed&3);for(int i=0;i<crater_count;i++){unsigned h=sun_hash(seed+i*0x27d4eb2du);int ex=10+(h%44),ey=10+((h>>8)%44),rr=3+((h>>16)%7),dx=sx-ex,dy=sy-ey,dd=dx*dx+dy*dy;if(dd<rr*rr)c=planet_rgb_mix(c,dd>(rr-2)*(rr-2)?0xd4c5a8:0x292b32,dd>(rr-2)*(rr-2)?105:75);}
 }
 /* A restrained per-world grade prevents palette twins even when two bodies
  * share the same broad climate family. Type-aware ranges keep oceans blue,
  * rocks mineral-rich and giants atmospheric rather than random neon. */
 {unsigned grade;
  if(type==OCEAN)grade=RGB(12+(style&63),65+((style>>7)&127),115+((style>>15)&127));
  else if(type==GAS)grade=RGB(55+(style&159),52+((style>>8)&159),58+((style>>16)&159));
  else grade=RGB(40+(style&175),38+((style>>8)&175),42+((style>>16)&175));
  c=planet_rgb_mix(c,grade,42+((style>>27)&19));
  c=planet_lush_grade(c,type==OCEAN?132:type==GAS?122:128,type==OCEAN?8:5);}
 int light=(int)((z*.72f+(-nx*.20f-ny*.09f)+.27f)*255);if(light<55)light=55;if(light>255)light=255;
 int edge=(int)(z*255);if(edge<110)light=light*edge/110;
 int cr=(c&255)*light/255,cg=((c>>8)&255)*light/255,cb=((c>>16)&255)*light/255;
 if(((sx+sy+(seed&7))&7)==0&&z>.72f){cr+=8;cg+=8;cb+=8;}
 return RGB(cr>255?255:cr,cg>255?255:cg,cb>255?255:cb)|0xff000000u;
}
/* Same hash used by system_bodies so chart / codex / flight share one identity. */
static unsigned body_art_seed(int sys,int body){return sun_hash((unsigned)(sys+1)*911u+(unsigned)body*65537u);}
static int sun_family(unsigned seed){return (int)(seed%8);}
/* One stellar palette for the visible disc, halo and reflected light. */
static unsigned sun_light_colour(unsigned seed){
 /* Colour is independent of the eight surface-animation families. Higher seed
  * bits keep every system stable across reloads without adding save fields. */
 static const unsigned char colours[32][3]={{255,205,112},{140,198,255},{235,242,255},{255,112,76},{255,166,80},{208,145,255},{255,218,158},{174,215,255},{255,235,150},{255,187,100},{255,145,92},{255,94,104},{255,132,173},{255,167,213},{240,136,240},{218,168,255},{180,149,255},{152,170,255},{115,183,255},{117,219,255},{126,242,255},{133,255,226},{155,255,201},{188,255,168},{220,255,166},{247,255,190},{216,240,255},{255,232,218},{255,202,182},{255,193,228},{216,212,255},{192,255,246}};
 const unsigned char *c=colours[(seed>>3)&31];return RGB(c[0],c[1],c[2]);
}

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
#ifdef TRAVEL_POLISH_REVIEW
static void draw_sun_sprite_reference(int cx,int cy,int radius,unsigned tint,unsigned seed,float time,int xt,int yt,int xb,int yb){
 if(radius<1)return;
 unsigned stellar=sun_light_colour(seed);int sr=stellar&255,sg=(stellar>>8)&255,sb=(stellar>>16)&255;
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
   sun_bloom_dot(x,y,RGB(strength*sr/255,strength*sg/255,strength*sb/255),xt,yt,xb,yb);continue;
  }
  int sx=(dx+radius)*31/(radius*2),sy=(dy+radius)*31/(radius*2);
  unsigned p=sheet[sy*32+sx];int grain=(((p>>10)&31)+((p>>5)&31)+(p&31))/3;
  int core=(r2-q)*55/r2,heat=grain/3+(fam%3)*5;
  pixel(x,y,RGB(sr+(255-sr)*(core+heat)/100,sg+(255-sg)*(core+heat)/100,sb+(255-sb)*(core+heat)/100));
 }
 /* Short outward flares, clipped pixel by pixel so previews never bleed. */
 for(int k=0;k<9;k++){
  float a=k*6.2831853f/9+seed*.001f+time*.06f;
  int reach=radius/6+1+(int)((.5f+.5f*sinf(time*1.8f+k+fam))*radius*.12f);
  for(int n=0;n<reach;n++){
   int x=cx+(int)(cosf(a)*(radius+n)),y=cy+(int)(sinf(a)*(radius+n));
   int fade=(reach-n)*90/reach;sun_bloom_dot(x,y,RGB(fade*sr/255,fade*sg/255,fade*sb/255),xt,yt,xb,yb);
  }
 }
 (void)tint;
}
#endif
/* Shared exact palettes remove divisions and RGB construction from every sun
 * pixel. Native output stays identical; no smaller framebuffer or skipped frames. */
static unsigned sun_core_palette[56][32],sun_halo_palette[59];static unsigned sun_core_seed;static int sun_core_ready;
static void draw_sun_sprite(int cx,int cy,int radius,unsigned tint,unsigned seed,float time,int xt,int yt,int xb,int yb){
 if(radius<1)return;
 unsigned stellar=sun_light_colour(seed);int sr=stellar&255,sg=(stellar>>8)&255,sb=(stellar>>16)&255;
 int fam=sun_family(seed),frame=((int)(time*6.f+(seed&7))&3);
 if(!sun_core_ready||sun_core_seed!=seed){
  for(int core=0;core<=55;core++)for(int grain=0;grain<32;grain++){int heat=grain/3+(fam%3)*5;sun_core_palette[core][grain]=RGB(sr+(255-sr)*(core+heat)/100,sg+(255-sg)*(core+heat)/100,sb+(255-sb)*(core+heat)/100);}
  for(int i=0;i<59;i++)sun_halo_palette[i]=RGB(i*sr/255,i*sg/255,i*sb/255);
  sun_core_seed=seed;sun_core_ready=1;
 }
 const uint16_t *sheet=sun_sprites[fam][frame];
 int glow=radius+radius/3+2,left=cx-glow,top=cy-glow,right=cx+glow+1,bottom=cy+glow+1;
 if(left<xt)left=xt;if(left<0)left=0;if(top<yt)top=yt;if(top<0)top=0;
 if(right>xb)right=xb;if(right>W)right=W;if(bottom>yb+1)bottom=yb+1;if(bottom>H)bottom=H;
 unsigned r2=radius*radius,g2=glow*glow,halo_den=g2-r2;
 /* Exact radial thresholds: at most112 core transitions per full row,
  * rather than a divide or64-bit product for every visible pixel. */
 unsigned core_edge[56],halo_edge[59];for(unsigned i=0;i<56;i++)core_edge[i]=r2-(i*r2+54)/55;for(unsigned i=0;i<59;i++)halo_edge[i]=g2-(i*halo_den+57)/58;
 int squares[W],sample[W];for(int x=left;x<right;x++){int dx=x-cx;squares[x]=dx*dx;sample[x]=(dx+radius)*31/(radius*2);}
 for(int y=top;y<bottom;y++){
  int core=0,strength=0;int dy=y-cy,yy=dy*dy,sy=(dy+radius)*31/(radius*2);unsigned *dest=fb+y*STRIDE;
  for(int x=left;x<right;x++){
   unsigned q=squares[x]+yy;if(q>g2)continue;
   if(q>r2){while(strength<58&&q<=halo_edge[strength+1])strength++;while(strength>0&&q>halo_edge[strength])strength--;
    sun_bloom_dot(x,y,sun_halo_palette[strength],xt,yt,xb,yb);continue;}
   unsigned v=sheet[sy*32+sample[x]],grain=(((v>>10)&31)+((v>>5)&31)+(v&31))/3;
   while(core<55&&q<=core_edge[core+1])core++;while(core>0&&q>core_edge[core])core--;
   dest[x]=sun_core_palette[core][grain];
  }
 }
 for(int k=0;k<9;k++){
  float a=k*6.2831853f/9+seed*.001f+time*.06f;
  int reach=radius/6+1+(int)((.5f+.5f*sinf(time*1.8f+k+fam))*radius*.12f);
  float ca=cosf(a),sa=sinf(a);
  for(int n=0;n<reach;n++){
   int x=cx+(int)(ca*(radius+n)),y=cy+(int)(sa*(radius+n));
   int fade=(reach-n)*90/reach;sun_bloom_dot(x,y,RGB(fade*sr/255,fade*sg/255,fade*sb/255),xt,yt,xb,yb);
  }
 }
 (void)tint;
}
/* The same immutable body seed selects art on charts, cards and in flight.
 * Only visible pixels are sampled, even beside a planet filling the screen.
 */
/* One immutable 64x64 sheet per possible system body, shared by flight and
 * every menu. 160 KiB bounded BSS replaces procedural shading on each frame
 * and the old rolled-view cache which thrashed between neighbouring planets. */
static unsigned planet_sheet_misses;
static const unsigned *planet_sheet(unsigned seed,int type){
 static struct {unsigned seed,pixels[64*64];int type,valid;} cache[BODY_COUNT];static unsigned next;
 for(int i=0;i<BODY_COUNT;i++)if(cache[i].valid&&cache[i].seed==seed&&cache[i].type==type)return cache[i].pixels;
 unsigned slot=next++%BODY_COUNT;cache[slot].valid=1;cache[slot].seed=seed;cache[slot].type=type;planet_sheet_misses++;
 for(int sy=0;sy<64;sy++)for(int sx=0;sx<64;sx++)cache[slot].pixels[sy*64+sx]=planet_proc_pixel(seed,type,sx,sy);
 return cache[slot].pixels;
}
static void __attribute__((optimize(ELITE_VISUAL_OPT))) draw_planet_sprite(int cx,int cy,int r,unsigned seed,int type,int xt,int yt,int xb,int yb){
 if(r<1||type==SUN)return;
 int x=cx-r,y=cy-r,d=r*2;
 int left=x>xt?x:xt,top=y>yt?y:yt,right=x+d<xb?x+d:xb,bottom=y+d<yb?y+d:yb;
 if(left<0)left=0;
 if(top<0)top=0;
 if(right>W)right=W;
 if(bottom>H)bottom=H;
 /* Resolve x sampling once and paint each procedural source scanline once. */
 int sample[W];for(int px=left;px<right;px++){int sx=2+(px-x)*60/d;sample[px]=(seed&16)?63-sx:sx;}
 unsigned colours[64];int previous_sy=-1;
 const unsigned *sheet=render_reference?0:planet_sheet(seed,type);
 for(int py=top;py<bottom;py++){int sy=2+(py-y)*60/d;
  if(!sheet&&sy!=previous_sy){previous_sy=sy;for(int sx=0;sx<64;sx++){
   colours[sx]=planet_proc_pixel(seed,type,sx,sy);
  }}
  for(int px=left;px<right;px++){unsigned c=sheet?sheet[sy*64+sample[px]]:colours[sample[px]];if(c)pixel(px,py,planet_phase_ink(c,px,py));}
 }
}
/* Inverse-map the billboard using the same screen-space roll as camera().
 * Cache the tiny tinted sheet; inner loop is additions and nearest sampling.
 * Menus retain draw_planet_sprite and never inherit the flight camera. */
static void __attribute__((optimize(ELITE_VISUAL_OPT))) draw_planet_sprite_rolled(int cx,int cy,int r,unsigned seed,int type,float roll,int xt,int yt,int xb,int yb){
 if(r<1||type==SUN)return;
 float c=cosf(roll),s=sinf(roll);
 if(fabsf(s)<.00001f&&c>.99999f){draw_planet_sprite(cx,cy,r,seed,type,xt,yt,xb,yb);return;}
 static unsigned palette[64*64],cached_seed;static int cached_type=-1;
 const unsigned *sheet=render_reference?palette:planet_sheet(seed,type);
 if(render_reference&&(cached_type!=type||cached_seed!=seed)){
  cached_type=type;cached_seed=seed;
  for(int sy=0;sy<64;sy++)for(int sx=0;sx<64;sx++)palette[sy*64+sx]=planet_proc_pixel(seed,type,sx,sy);
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
   unsigned colour=sheet[sy*64+sx];if(colour)pixel(px,py,planet_phase_ink(colour,px,py));
  }
 }
}

