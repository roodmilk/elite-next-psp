/* Authored architectural suites for rich, frontier and capital families. */
#include "station-art-atlas.h"
static int station_art_enabled=1,station_art_ready,station_art_style,station_art_override=-1;
static unsigned station_art_seed;
static unsigned char *station_art_pixels;
static int station_art_last=-1,station_art_last_role;
static int station_art_readability=1;
/* Only minified facade art is filtered. Original palette pixels remain exact
 * when magnified; no history buffer, camera snapping or animated dither. */
static int station_art_stable=1;
static float station_art_footprint;
static int station_art_filter_level,station_art_filter_weight;
static unsigned station_art_mips[6][8][341],station_art_mip_palette[6][16];
static int station_art_mip_style=-1;
static const int station_art_mip_offset[5]={0,256,320,336,340};
static unsigned station_art_average(unsigned a,unsigned b,unsigned c,unsigned d){
 return RGB(((a&255)+(b&255)+(c&255)+(d&255)+2)>>2,(((a>>8)&255)+((b>>8)&255)+((c>>8)&255)+((d>>8)&255)+2)>>2,(((a>>16)&255)+((b>>16)&255)+((c>>16)&255)+((d>>16)&255)+2)>>2);
}
static inline __attribute__((always_inline)) unsigned station_art_mix(unsigned a,unsigned b,int w){
 unsigned rb=(((a&0xff00ff)*(256-w)+(b&0xff00ff)*w+0x800080)>>8)&0xff00ff;
 unsigned g=(((a&0xff00)*(256-w)+(b&0xff00)*w+0x8000)>>8)&0xff00;
 return rb|g|0xff000000u;
}
static inline __attribute__((always_inline)) unsigned station_art_mip_sample(int face,int role,int level,int x,int y){
 int shift=level+1,size=16>>level;
 return station_art_mips[face][role][station_art_mip_offset[level]+(y>>shift)*size+(x>>shift)];
}
static Vec3 station_art_inverse;static float station_art_cos[12],station_art_sin[12],station_art_ring_inv;
static unsigned station_art_palette[6][16];
static const unsigned char station_art_roles[37]={0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,3,3,4,4,2,5,2,6,2,5,2,6,2,5,2,5,2,5,2,9,2,9};
static int station_art_family(void){if(station_art_override>=0)return station_art_override;if(mc_profile.station_class==STATION_MEGA)return 5;if(mc_profile.station_class==STATION_POOR)return 3;switch(mc_profile.family){case 4:case 15:return 1;case 1:case 3:case 11:case 13:return 2;case 6:case 7:case 9:case 14:return 4;default:return 0;}}
static int station_art_active(void){return station_art_enabled&&!high_contrast&&mc_draw_city&&mc_draw_city->n<=MEGA_BLOCK_MAX;}
static int station_art_role(const MegaBlock *b,int id){
 if(mc_profile.seed==0x8af9b3e8u&&mc_draw_city->n==37)return station_art_roles[id];
 if(mc_profile.station_class==STATION_MEGA){if(!id)return 0;if(b->kind==3)return 9;if(b->kind==4)return 2;if(b->shape==MC_SPHERE||b->shape==MC_DOME)return 3;if(b->kind==2||b->kind==5||b->shape==MC_POD)return 4;return 5;}
 if(!id)return 0;if(b->kind==1||b->kind==5)return 9;if(b->kind==7)return 6;
 if(b->shape>=MC_RING0)return 1;if(b->shape==MC_SPHERE||b->shape==MC_DOME)return 3;
 if(b->shape==MC_POD)return b->e.z>b->e.x*1.8f?4:5;
 if(b->shape==MC_BEVEL&&(b->e.x>b->e.y*3||b->e.y>b->e.x*3))return 2;
 if(b->shape==MC_OCTAGON&&b->e.x<mc_profile.radius*.12f)return 9;
 return 5;
}
/* Quotient-rule UV gradients at the span centre, including grazing angles.
 * Gradients use the real 2-pixel facade sample footprint. */
static void station_art_span(const MegaBlock*b,int face,float u,float v,float z,float di,float du,float dv,float iy,float uy,float vy){
 station_art_footprint=0;if(!station_art_stable)return;
 float ux=z*(du-u*di),vx=z*(dv-v*di),uyy=z*(uy-u*iy),vyy=z*(vy-v*iy);
 int id=(int)(b-mc_draw_city->b),role=station_art_role(b,id);
 float us=face<4?1/b->e.x:1/b->e.z,vs=(face==2||face==3)?1/b->e.z:1/b->e.y;
 float ax=ux*us*16,bx=vx*vs*16,ay=uyy*us*16,by=vyy*vs*16;
 if(role==1&&face<2){int s=b->shape-MC_RING0;float ca=station_art_cos[s],sa=station_art_sin[s],inv=1/b->e.x;
 ax=(-sa*ux+ca*vx)*inv*53.76f;ay=(-sa*uyy+ca*vyy)*inv*53.76f;
 bx=(ca*ux+sa*vx)*inv*100.32f;by=(ca*uyy+sa*vyy)*inv*100.32f;}
 station_art_footprint=fmaxf(fmaxf(fabsf(ax),fabsf(ay)),fmaxf(fabsf(bx),fabsf(by)));
 station_art_filter_level=0;float edge=2.f;while(station_art_filter_level<4&&station_art_footprint>edge*2){station_art_filter_level++;edge*=2;}
 float blend=station_art_footprint<2?station_art_footprint-1:(station_art_footprint-edge)/edge;
 station_art_filter_weight=(int)(fminf(1,fmaxf(0,blend))*256);

}
static int station_art_unpack(void){
 if(station_art_pixels)return 1;unsigned char*p=malloc(24576);if(!p)return 0;unsigned read=0,written=0;
 while(read<sizeof(station_art_packed)){unsigned token=station_art_packed[read++],kind=token>>6,count=(token&63)+(kind?3:1);if(count>24576-written)goto bad;
  if(!kind){if(count>sizeof(station_art_packed)-read)goto bad;memcpy(p+written,station_art_packed+read,count);read+=count;}
  else if(kind==1){if(read>=sizeof(station_art_packed))goto bad;memset(p+written,station_art_packed[read++],count);}
  else {if((kind==3?2u:1u)>sizeof(station_art_packed)-read)goto bad;unsigned distance=station_art_packed[read++]+1;if(kind==3)distance+=station_art_packed[read++]<<8;if(distance>written)goto bad;for(unsigned i=0;i<count;i++)p[written+i]=p[written+i-distance];}
  written+=count;
 }if(written!=24576)goto bad;station_art_pixels=p;return 1;
 bad:free(p);return 0;
}
static void station_art_prepare(void){
 if(!station_art_active()||!station_art_unpack())return;
 station_art_style=station_art_family();
 if(!station_art_ready||station_art_seed!=mc_profile.seed){station_art_ready=1;station_art_seed=mc_profile.seed;station_art_last=-1;for(int i=0;i<12;i++){station_art_cos[i]=cosf(i*.5235987756f);station_art_sin[i]=sinf(i*.5235987756f);}}
 static const unsigned char colors[6][16][3]={{{104,122,137},{143,156,158},{176,181,170},{38,55,68},{53,70,81},{21,54,69},{57,115,125},{204,156,85},{226,184,112},{146,112,75},{183,154,84},{102,183,181},{29,43,54},{82,107,125},{157,170,165},{180,181,165}},{{87,102,115},{76,99,113},{204,208,189},{24,33,43},{44,53,62},{15,33,45},{65,105,116},{255,167,57},{255,209,113},{130,103,61},{227,159,65},{117,205,215},{13,24,34},{71,80,89},{96,117,127},{180,181,165}},{{110,134,148},{192,209,211},{224,236,224},{30,49,60},{44,74,83},{19,54,73},{53,139,160},{123,231,223},{179,243,240},{82,126,144},{105,216,226},{169,233,213},{17,37,51},{74,106,121},{158,181,186},{180,181,165}},{{112,109,101},{156,149,123},{205,193,158},{40,43,44},{67,66,55},{28,44,52},{68,100,103},{232,158,65},{242,190,98},{139,117,74},{192,146,69},{145,198,172},{23,31,36},{127,109,91},{151,151,129},{180,181,165}},{{129,139,151},{208,202,177},{237,221,185},{34,39,53},{54,61,73},{22,39,64},{71,117,147},{235,190,113},{247,218,155},{156,129,80},{199,165,98},{148,215,211},{19,27,42},{91,104,130},{169,178,180},{180,181,165}},{{92,119,144},{167,184,190},{219,222,204},{25,39,56},{46,64,81},{20,46,67},{62,121,151},{236,182,99},{247,217,147},{96,128,146},{108,188,201},{149,224,217},{13,27,43},{74,98,122},{130,153,167},{180,181,165}}};
 for(int face=0;face<6;face++)for(int i=0;i<16;i++){
  const unsigned char*q=colors[station_art_style][i];unsigned raw=q[0]|q[1]<<8|q[2]<<16;unsigned c=i==7||i==11?raw:dg_material(raw,dg_station_lit[face]);
  /* Shadow-only cool fill on selected armour. Sunlit finish, structural
   * recesses, glass and emissive colours remain exactly as authored. */
  if(station_art_readability&&(i==0||i==1||i==14)&&dg_station_lit[face]<128){int fill=(128-dg_station_lit[face])*12/128;c=RGB((c&255)+fill*3/4,((c>>8)&255)+fill,((c>>16)&255)+fill*5/4);}
  station_art_palette[face][i]=c;
 }
 for(int face=0;face<6;face++){
  if(station_art_mip_style==station_art_style&&!memcmp(station_art_mip_palette[face],station_art_palette[face],sizeof(station_art_palette[face])))continue;
  memcpy(station_art_mip_palette[face],station_art_palette[face],sizeof(station_art_palette[face]));
  for(int role=0;role<8;role++){
   unsigned*m=station_art_mips[face][role];const unsigned char*src=station_art_pixels+(station_art_style*8+role)*512;
   for(int y=0;y<16;y++)for(int x=0;x<16;x++){
    int a=src[y*32+x],b=src[y*32+16+x];
    m[y*16+x]=station_art_average(station_art_palette[face][a&15],station_art_palette[face][a>>4],station_art_palette[face][b&15],station_art_palette[face][b>>4]);
   }
   for(int level=1;level<5;level++){int size=16>>level,prev=size*2;unsigned*dst=m+station_art_mip_offset[level];const unsigned*q=m+station_art_mip_offset[level-1];
    for(int y=0;y<size;y++)for(int x=0;x<size;x++){int at=y*2*prev+x*2;dst[y*size+x]=station_art_average(q[at],q[at+1],q[at+prev],q[at+prev+1]);}}
  }
 }
 station_art_mip_style=station_art_style;
}
/* Geometry, material role and palette ownership stay constant for a face.
 * Prepare them once per triangle, rather than once per 2x2 pixel sample. */
static struct {int active,role,ring,swap,flat,ad,stamp_y,dim_ci;float us,vs,inv,ca,sa,ad_u,ad_v;const unsigned *palette,*mip_data;const unsigned char *pixels,*glyph0,*glyph1;} station_art_face;
static void station_art_face_prepare(const MegaBlock*b,int face){
 station_art_face.active=0;if(!station_art_pixels||!station_art_active())return;
 int id=(int)(b-mc_draw_city->b);if(id<0||id>=mc_draw_city->n)return;
 int role=station_art_role(b,id);if(role>=8)return;
 Vec3 inv={1/b->e.x,1/b->e.y,1/b->e.z};
 station_art_face.us=face<4?inv.x:inv.z;station_art_face.vs=face==2||face==3?inv.z:inv.y;
 station_art_face.ring=role==1&&face<2;station_art_face.inv=inv.x;
 if(station_art_face.ring){int sector=b->shape-MC_RING0;station_art_face.ca=station_art_cos[sector];station_art_face.sa=station_art_sin[sector];}else if(role==1)role=7;
 station_art_face.swap=role==2&&b->e.y>b->e.x;
 station_art_face.flat=role==3&&(face==2||face==3)&&station_art_style==0;
 if(role==3&&(face==2||face==3)&&station_art_style!=0)role=station_art_style==5?3:4;
 if(role==0&&face==1)role=5;
 station_art_face.ad=mc_profile.station_class==STATION_MEGA&&!b->shape&&(b->kind==1||b->kind==5)&&face<2;
 station_art_face.ad_u=b->e.x*.79f;station_art_face.ad_v=b->e.y*.80f;
 station_art_face.stamp_y=-1;
 if(role==1&&face<2&&((id-1)%3)==0){int sector=b->shape-MC_RING0+1;station_art_face.stamp_y=11;station_art_face.glyph0=font8['0'+sector/10-32];station_art_face.glyph1=font8['0'+sector%10-32];}
 if(role==0&&face==0){station_art_face.stamp_y=3;station_art_face.glyph0=font8['0'-32];station_art_face.glyph1=font8['1'-32];}
 station_art_face.role=role;station_art_face.dim_ci=role==1||role==7?4:5;
 station_art_face.palette=station_art_palette[face];station_art_face.mip_data=station_art_mips[face][role];station_art_face.pixels=station_art_pixels+(station_art_style*8+role)*512;station_art_face.active=1;
}
static inline __attribute__((always_inline)) int station_art_face_texel(float u,float v,float z,unsigned*out){
 if(!station_art_face.active)return 0;
 if(station_art_face.ad&&fabsf(u)<station_art_face.ad_u&&fabsf(v)<station_art_face.ad_v&&z<18000)return 0;
 const unsigned*palette=station_art_face.palette;
 if(station_art_face.flat){*out=palette[1];return 1;}
 float tu=(u*station_art_face.us+1)*.5f,tv=(1-v*station_art_face.vs)*.5f;
 if(station_art_face.ring){float ca=station_art_face.ca,sa=station_art_face.sa,inv=station_art_face.inv;tu=.5f+(-sa*u+ca*v)*inv*1.68f;tv=1-((ca*u+sa*v)*inv+.208f)*3.135f;}
 if(station_art_face.swap){float q=tu;tu=tv;tv=q;}
 /* Power-of-two mip coordinates are exact shifts of one clamped 32px
  * coordinate. Share conversions/clamps across both filter samples. */
 int x=(int)(tu*32),y=(int)(tv*32);if(x<0)x=0;if(x>31)x=31;if(y<0)y=0;if(y>31)y=31;
 if(station_art_stable&&station_art_footprint>1.f){
  int level=station_art_filter_level,w=station_art_filter_weight;
  int shift=level+1;const unsigned*mip_data=station_art_face.mip_data;
  unsigned filtered=mip_data[station_art_mip_offset[level]+(y>>shift)*(16>>level)+(x>>shift)];
  if(station_art_footprint<2.f){
   int q=station_art_face.pixels[y*16+(x>>1)];unsigned raw=palette[(x&1)?q>>4:q&15];filtered=station_art_mix(raw,filtered,w);}
  else if(level<4&&w){int next=level+1;filtered=station_art_mix(filtered,mip_data[station_art_mip_offset[next]+(y>>(shift+1))*(16>>next)+(x>>(shift+1))],w);}
  *out=filtered;return 1;
 }
 int packed=station_art_face.pixels[y*16+(x>>1)],ci=(x&1)?packed>>4:packed&15;
 /* Actual font stencil, placed once on selected whole ring sectors. */
 int sy=station_art_face.stamp_y;
 if(sy>=0&&x>=8&&x<24&&y>=sy&&y<sy+8){const unsigned char*g=x<16?station_art_face.glyph0:station_art_face.glyph1;if(g[y-sy]&(128>>((x-8)&7)))ci=sy==11?10:1;}
 if(z>9000&&(ci==7||ci==11))ci=station_art_face.dim_ci;
 *out=palette[ci];return 1;
}
static int station_art_texel(const MegaBlock*b,int face,float u,float v,float z,unsigned*out){station_art_face_prepare(b,face);return station_art_face_texel(u,v,z,out);}

