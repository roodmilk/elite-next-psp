/* Continuous distant sky. The system's six 65x65 faces share exact edge
 * directions. A small perspective grid samples them with bilinear filtering;
 * integer interpolation shades EVERY output pixel, never blocks or stamps.
 * 101400 bytes of fixed cache; no per-frame allocation or random reseeding. */
#define SOFT_SKY_FACE 65
#define SOFT_SKY_STEP 8
static unsigned soft_sky_faces[6][SOFT_SKY_FACE][SOFT_SKY_FACE];
static unsigned soft_sky_cached_seed;
static int soft_sky_ready;
static float soft_sky_clamp(float x){return x<0?0:x>1?1:x;}
static float soft_sky_smooth(float x){x=soft_sky_clamp(x);return x*x*(3-2*x);}
static float soft_sky_hash(unsigned seed,int x,int y,int z){return (field_hash(seed^(unsigned)x*0x9e3779b9u^(unsigned)y*0x85ebca6bu^(unsigned)z*0xc2b2ae35u)&65535)/65535.f;}
static float soft_sky_noise(unsigned seed,Vec3 p){
 int x=(int)floorf(p.x),y=(int)floorf(p.y),z=(int)floorf(p.z);
 float u=soft_sky_smooth(p.x-x),v=soft_sky_smooth(p.y-y),w=soft_sky_smooth(p.z-z);
 float a=soft_sky_hash(seed,x,y,z),b=soft_sky_hash(seed,x+1,y,z),c=soft_sky_hash(seed,x,y+1,z),d=soft_sky_hash(seed,x+1,y+1,z);
 float e=soft_sky_hash(seed,x,y,z+1),f=soft_sky_hash(seed,x+1,y,z+1),g=soft_sky_hash(seed,x,y+1,z+1),h=soft_sky_hash(seed,x+1,y+1,z+1);
 float lo=(a+(b-a)*u)*(1-v)+(c+(d-c)*u)*v,hi=(e+(f-e)*u)*(1-v)+(g+(h-g)*u)*v;
 return lo+(hi-lo)*w;
}
static Vec3 soft_sky_face_direction(int face,float u,float v){
 Vec3 d=face==0?(Vec3){1,v,u}:face==1?(Vec3){-1,v,u}:face==2?(Vec3){u,1,v}:face==3?(Vec3){u,-1,v}:face==4?(Vec3){u,v,1}:(Vec3){u,v,-1};return norm(d);
}
static void soft_sky_prepare(unsigned seed){
 if(soft_sky_ready&&seed==soft_sky_cached_seed)return;
 /* Paired cool/warm hues keep gas luminous without a neon-green wash. */
 static const unsigned gas[]={RGB(40,112,174),RGB(101,65,153),RGB(27,118,126),RGB(142,76,56),RGB(73,83,168),RGB(135,101,57),RGB(38,110,161),RGB(121,57,101)};
 static const unsigned edge[]={RGB(122,85,161),RGB(47,131,164),RGB(56,86,174),RGB(83,74,150),RGB(32,136,153),RGB(72,89,144),RGB(83,65,141),RGB(57,112,152)};
 unsigned style=(seed>>27)&7;float angle=(seed&1023)*.006135923f,tilt=((seed>>12)&1023)*.006135923f;
 float ca=cosf(angle),sa=sinf(angle),ct=cosf(tilt),st=sinf(tilt),width=.40f+((seed>>8)&15)*.014f;
 for(int face=0;face<6;face++)for(int y=0;y<SOFT_SKY_FACE;y++)for(int x=0;x<SOFT_SKY_FACE;x++){
  Vec3 d=soft_sky_face_direction(face,x/32.f-1,y/32.f-1);
  Vec3 q={d.x*ca+d.z*sa,d.y*ct+(-d.x*sa+d.z*ca)*st,-d.y*st+(-d.x*sa+d.z*ca)*ct};
  Vec3 p=add(mul(q,2.8f),(Vec3){7.3f,11.8f,3.9f});
  float broad=soft_sky_noise(seed,p),grain=soft_sky_noise(seed^0x73a95bdu,mul(p,2.2f)),fine=soft_sky_noise(seed^0xd5934ad1u,mul(p,5.3f));
  float warp=(broad-.5f)*.44f+(grain-.5f)*.11f;
  float band=soft_sky_smooth(1-fabsf(q.y+warp)/width);
  if(((seed>>24)&3)==1)band=fmaxf(band*.85f,soft_sky_smooth(1-fabsf(q.y-warp-.28f)/.24f)*.7f);
  float cloud=soft_sky_smooth((broad*.64f+grain*.25f+fine*.11f-.26f)*1.65f);
  float density=(.20f+band*1.15f)*cloud;
  float lane=soft_sky_smooth(1-fabsf(q.y+warp*.65f)/(.045f+grain*.055f));
  density*=1-lane*.73f;
  float curl=soft_sky_smooth(1-fabsf(grain-.52f)*7.f)*band*cloud;
  unsigned tint=sky_mix(gas[style],edge[style],soft_sky_smooth((grain-.27f)*2.4f));
  float light=density*.95f+curl*.10f,hot=soft_sky_smooth((density-.5f)*2.f)*.23f;
  int r=3+(int)((tint&255)*light+165*hot),g=5+(int)(((tint>>8)&255)*light+186*hot),b=11+(int)(((tint>>16)&255)*light+206*hot);
  soft_sky_faces[face][y][x]=RGB(r>200?200:r,g>210?210:g,b>225?225:b);
 }
 soft_sky_cached_seed=seed;soft_sky_ready=1;
}
static unsigned soft_sky_lerp(unsigned a,unsigned b,int t){
 int s=256-t;
 unsigned rb=((((a&0x00ff00ffu)*s)+((b&0x00ff00ffu)*t))>>8)&0x00ff00ffu;
 unsigned g=((((a&0x0000ff00u)*s)+((b&0x0000ff00u)*t))>>8)&0x0000ff00u;
 return 0xff000000u|rb|g;
}
static unsigned soft_sky_sample(Vec3 d){
 float ax=fabsf(d.x),ay=fabsf(d.y),az=fabsf(d.z),u,v,m;int face;
 if(ax>=ay&&ax>=az){m=ax;face=d.x>=0?0:1;u=d.z;v=d.y;}
 else if(ay>=az){m=ay;face=d.y>=0?2:3;u=d.x;v=d.z;}
 else{m=az;face=d.z>=0?4:5;u=d.x;v=d.y;}
 float scale=8192.f/m;int fx=(int)((u+m)*scale),fy=(int)((v+m)*scale);
 if(fx<0)fx=0;if(fy<0)fy=0;if(fx>16383)fx=16383;if(fy>16383)fy=16383;
 int x=fx>>8,y=fy>>8;unsigned a=soft_sky_lerp(soft_sky_faces[face][y][x],soft_sky_faces[face][y][x+1],fx&255),b=soft_sky_lerp(soft_sky_faces[face][y+1][x],soft_sky_faces[face][y+1][x+1],fx&255);
 return soft_sky_lerp(a,b,fy&255);
}
static void space_fx_nebula(void){
 if(high_contrast||game.jump>0)return;
 soft_sky_prepare(space_sky_seed());
 int left=clipy0>=0?clipx0:0,right=clipy0>=0?clipx1:W,top=clipy0>=0?clipy0:view_top(),bot=clipy0>=0?clipy1:view_bot()+1;
 if(left<0)left=0;if(right>W)right=W;if(top<0)top=0;if(bot>H)bot=H;if(left>=right||top>=bot)return;
 /* Inverse of camera(): exact perspective, no translation, fractional angle
  * multipliers, Euler scrolling, or time-varying texture hashes. */
 float sy=sinf(game.yaw),cy=cosf(game.yaw),sp=sinf(game.pitch),cp=cosf(game.pitch),sr=sinf(game.roll),cr=cosf(game.roll);
 Vec3 r={cy,0,-sy},u={-sy*sp,cp,-cy*sp},ahead={sy*cp,sp,cy*cp};
 Vec3 rightward=add(mul(r,cr),mul(u,sr)),upward=add(mul(r,-sr),mul(u,cr));
 unsigned rows[2][W/SOFT_SKY_STEP+2];int columns=(right-left+SOFT_SKY_STEP-1)/SOFT_SKY_STEP;
 for(int row=0,y0=top;y0<bot;row++,y0+=SOFT_SKY_STEP){
  int y1=y0+SOFT_SKY_STEP;if(y1>bot)y1=bot;
  for(int side=row?1:0;side<2;side++){int y=side?y1:y0;Vec3 base=add(mul(ahead,240),mul(upward,(float)(proj_oy-y)));
   for(int col=0;col<=columns;col++){int x=left+col*SOFT_SKY_STEP;if(x>right)x=right;rows[(row+side)&1][col]=soft_sky_sample(add(base,mul(rightward,(float)(x-proj_ox))));}
  }
  for(int y=y0;y<y1;y++){int t=y1-y0==8?(y-y0)*32:(y-y0)*256/(y1-y0);unsigned *out=fb+y*STRIDE;
   for(int col=0,x0=left;x0<right;col++,x0+=SOFT_SKY_STEP){int x1=x0+SOFT_SKY_STEP;if(x1>right)x1=right;
    unsigned a=soft_sky_lerp(rows[row&1][col],rows[(row+1)&1][col],t),b=soft_sky_lerp(rows[row&1][col+1],rows[(row+1)&1][col+1],t);
    int red=(a&255)<<8,green=((a>>8)&255)<<8,blue=((a>>16)&255)<<8,n=x1-x0;
    int dr=(int)(b&255)-(int)(a&255),dg=(int)((b>>8)&255)-(int)((a>>8)&255),db=(int)((b>>16)&255)-(int)((a>>16)&255);
    if(n==8){dr*=32;dg*=32;db*=32;}else{dr=dr*256/n;dg=dg*256/n;db=db*256/n;}
    for(int x=x0;x<x1;x++){out[x]=RGB(red>>8,green>>8,blue>>8);red+=dr;green+=dg;blue+=db;}
   }
  }
 }
}
