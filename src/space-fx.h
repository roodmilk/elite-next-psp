/* Soft-framebuffer space FX kit for ELITE: NEXT.
 *
 * Search note (2026-09): PSP particle / GU libraries (pspgl samples, sceGu
 * particle demos) assume a textured GU pipeline. This engine paints a software
 * 480x272 framebuffer with pixel/rect/line only — those libs do not drop in.
 * This header is the in-engine effect library: deterministic, no heap, no
 * second buffer, additive soft-dots, bounded cost, clipped to the canopy.
 * Respects high_contrast (skips decorative haze / meteors).
 */
static void sfx_add(int x,int y,unsigned c,int top,int bot){
 if(x<0||x>=W||y<top||y>bot)return;
 sun_bloom_dot(x,y,c,0,top,W,bot);
}
static unsigned sfx_tint(unsigned seed,int slot){
 static const unsigned pal[]={
  RGB(28,18,48),RGB(18,32,52),RGB(42,16,28),RGB(12,36,40),
  RGB(36,24,18),RGB(22,14,44),RGB(14,28,36),RGB(40,20,36)
 };
 return pal[(seed+slot*3)&7];
}
/* Large nebula ribbons + layered space clouds. Drawn after the sector base fill. */
static void space_fx_nebula(void){
 if(high_contrast||game.jump>0)return;
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 unsigned seed=game.bodies[0].seed^((unsigned)game.system*2654435761u);
 /* Galactic band — soft diagonal haze for sky variety. */
 {
  float ang=((seed&255)/255.f)*1.2f+.4f;
  float ca=cosf(ang),sa=sinf(ang);
  unsigned band=sfx_tint(seed,0);
  unsigned haze=RGB((band&255)/5,((band>>8)&255)/5,((band>>16)&255)/6);
  int samples=high_contrast?0:90;
  for(int i=0;i<samples;i++){
   float t=(i+.5f)/samples;
   float along=(t-.5f)*520.f;
   float wobble=sinf(t*6.2831853f*2.f+game.time*.05f+(seed&31))*.35f;
   int x=(int)(240+ca*along-sa*wobble*40);
   int y=(int)((top+bot)/2+sa*along*.55f+ca*wobble*28);
   int rad=2+((i+seed)&3);
   for(int dy=-rad;dy<=rad;dy++)for(int dx=-rad;dx<=rad;dx++){
    if(dx*dx+dy*dy>rad*rad)continue;
    sfx_add(x+dx,y+dy,haze,top,bot);
   }
  }
 }
 /* 3–6 distant nebula clouds (camera-locked directions, Elite-style). */
 int clouds=3+(seed%4);
 for(int cloud=0;cloud<clouds;cloud++){
  float angle=game.system*1.73f+cloud*1.37f+(seed&15)*.07f;
  Vec3 direction={sinf(angle),sinf(angle*.73f+cloud)*.42f,cosf(angle)};
  Vec3 v=camera(&game,add(game.pos,mul(direction,48000)));
  if(v.z<2500)continue;
  Point p=project(v);
  float radius=fminf(300,2000000/v.z)*(1.f+((seed>>(cloud*3))&3)*.14f);
  unsigned tint=sfx_tint(seed,cloud+1);
  unsigned soft=RGB((tint&255)/4,((tint>>8)&255)/4,((tint>>16)&255)/5);
  unsigned bright=RGB((tint&255)/2,((tint>>8)&255)/2,((tint>>16)&255)/3);
  int density=36+((seed>>(cloud*4))&24);
  for(int k=0;k<density;k++){
   float a=k*2.39996f+cloud+game.time*.01f;
   float rk=radius*sqrtf((k+.35f)/density);
   float squash=.42f+.08f*(cloud&3);
   int x=(int)(p.x+cosf(a)*rk),y=(int)(p.y+sinf(a)*rk*squash);
   int size=1+((k+(seed&3))&2);
   unsigned ink=(k&7)==0?bright:soft;
   for(int dy=0;dy<size;dy++)for(int dx=0;dx<size*2;dx++)sfx_add(x+dx,y+dy,ink,top,bot);
  }
  /* Filament streaks across the cloud for structure. */
  for(int f=0;f<3;f++){
   float fa=cloud*1.1f+f*1.9f;
   int x0=(int)(p.x+cosf(fa)*radius*.2f),y0=(int)(p.y+sinf(fa)*radius*.12f);
   int x1=(int)(p.x+cosf(fa+1.2f)*radius*.85f),y1=(int)(p.y+sinf(fa+1.2f)*radius*.4f);
   for(int s=0;s<18;s++){
    float t=s/17.f;int x=(int)(x0+(x1-x0)*t),y=(int)(y0+(y1-y0)*t);
    sfx_add(x,y,soft,top,bot);sfx_add(x+1,y,soft,top,bot);
   }
  }
 }
 /* Local dust / micro-clouds that scroll with position cells. */
 unsigned cell=(unsigned)((int)(game.pos.x/5000))*73856093u^(unsigned)((int)(game.pos.z/5000))*19349663u^seed;
 int motes=24+(cell&47);
 for(int i=0;i<motes;i++){
  cell=cell*1664525u+1013904223u;int x=(cell>>16)%W;
  cell=cell*1664525u+1013904223u;int y=top+((cell>>16)%(bot-top+1));
  unsigned c=(i%11==0)?RGB(18,12,28):(i%5==0)?RGB(10,18,24):RGB(8,10,16);
  sfx_add(x,y,c,top,bot);
  if((cell&15)==0)sfx_add(x+1,y,c,top,bot);
 }
}
/* Rare shooting stars — brief streaks, seeded so systems feel different. */
static void space_fx_meteors(void){
 if(high_contrast||game.jump>0||game.boost)return;
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 unsigned seed=game.bodies[0].seed^0xA5A5u;
 float period=11.f+(seed%9); /* 11–19 s between chances */
 float phase=fmodf(game.time*.37f+(seed&255)*.13f,period);
 /* Active window ~0.55s; only one slot fires when the low bits match. */
 if(phase>0.55f)return;
 if(((seed>>4)+(int)(game.time/period))%5==0)return; /* skip 4/5 windows = rare */
 float t=phase/0.55f;
 float ang=((seed>>8)&255)*.0245f+1.1f;
 float ca=cosf(ang),sa=sinf(ang);
 int x0=40+((seed>>3)&127),y0=top+20+((seed>>11)&63);
 int len=70+((seed>>5)&63);
 int x1=x0+(int)(ca*len),y1=y0+(int)(sa*len*.55f);
 int x=(int)(x0+(x1-x0)*t),y=(int)(y0+(y1-y0)*t);
 int tx=(int)(x0+(x1-x0)*fmaxf(0.f,t-.18f)),ty=(int)(y0+(y1-y0)*fmaxf(0.f,t-.18f));
 if(y<top||y>bot||ty<top||ty>bot)return;
 unsigned head=RGB(220,230,255),tail=RGB(90,120,180);
 line(tx,ty,x,y,tail);
 sfx_add(x,y,head,top,bot);
 sfx_add(x+1,y,head,top,bot);
 pixel(x,y,WHITE);
}
/* Stronger star twinkle + occasional bright sparkle (called from starfield). */
static unsigned space_fx_twinkle(unsigned c,int i,float time){
 float rate=1.1f+(i&7)*.22f;
 float wave=sinf(time*rate+i*1.7f);
 int tw=(int)(wave*22);
 if((i%13)==0)tw+=(int)(fabsf(sinf(time*3.1f+i))*28); /* brighter sparkle subset */
 int r=(int)fmaxf(20,(c&255)+tw),g=(int)fmaxf(24,((c>>8)&255)+tw),b=(int)fmaxf(30,((c>>16)&255)+tw);
 if(r>255)r=255;if(g>255)g=255;if(b>255)b=255;
 return RGB(r,g,b);
}
