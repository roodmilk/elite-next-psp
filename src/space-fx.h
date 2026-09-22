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
  unsigned haze=RGB((band&255)/3,((band>>8)&255)/3,((band>>16)&255)/4);
  int samples=high_contrast?0:110;
  for(int i=0;i<samples;i++){
   float t=(i+.5f)/samples;
   float along=(t-.5f)*540.f;
   float wobble=sinf(t*6.2831853f*2.f+game.time*.05f+(seed&31))*.45f;
   int x=(int)(240+ca*along-sa*wobble*48);
   int y=(int)((top+bot)/2+sa*along*.55f+ca*wobble*32);
   int rad=3+((i+seed)&3);
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
  float radius=fminf(320,2100000/v.z)*(1.f+((seed>>(cloud*3))&3)*.16f);
  unsigned tint=sfx_tint(seed,cloud+1);
  unsigned soft=RGB((tint&255)/2,((tint>>8)&255)/2,((tint>>16)&255)/3);
  unsigned bright=RGB((tint&255)*2/3,((tint>>8)&255)*2/3,((tint>>16)&255)/2);
  int density=48+((seed>>(cloud*4))&28);
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
/* Shooting stars removed — Commander: no permanent meteor shower. */
static void space_fx_meteors(void){}
/* Gentle star twinkle only — no bright four-point sparkle subset. */
static unsigned space_fx_twinkle(unsigned c,int i,float time){
 float rate=1.1f+(i&7)*.22f;
 float wave=sinf(time*rate+i*1.7f);
 int tw=(int)(wave*12);
 int r=(int)fmaxf(20,(c&255)+tw),g=(int)fmaxf(24,((c>>8)&255)+tw),b=(int)fmaxf(30,((c>>16)&255)+tw);
 if(r>255)r=255;if(g>255)g=255;if(b>255)b=255;
 return RGB(r,g,b);
}

/* ---- Wave A: plume densify / boost shimmer / hit sparks / embers ----
 * Soft-FB only, fixed pools, canopy-clipped. Release wires from flight paint.
 * Skip decorative work when high_contrast, warp, or guided dock clarity wins. */
enum { SFX_HIT_SLOTS=16, SFX_EMBER_SLOTS=12 };
typedef struct { float life,max; int x0,y0,x1,y1; unsigned c; } SfxStreak;
static SfxStreak sfx_hits[SFX_HIT_SLOTS];
static SfxStreak sfx_embers[SFX_EMBER_SLOTS];
static void sfx_fx_reset(void){
 for(int i=0;i<SFX_HIT_SLOTS;i++)sfx_hits[i].life=0;
 for(int i=0;i<SFX_EMBER_SLOTS;i++)sfx_embers[i].life=0;
}
static int sfx_fx_muted(void){
 return high_contrast||game.jump>0||game.dock_stage>=2||game.police_stop;
}
/* Extra soft-dots along an already-projected NPC aft root (after npc_engine_glow). */
static void sfx_engine_plume_at(int x,int y,int boostish,int top,int bot){
 if(sfx_fx_muted()||x<4||x>W-4||y<top+3||y>bot-3)return;
 int n=boostish?12:8;
 unsigned core=boostish?CYAN:RGB(255,185,70),edge=boostish?RGB(45,120,170):RGB(150,70,28);
 for(int i=0;i<n;i++){
  float t=i/(float)(n-1);
  int xx=x+(int)((i&1?1:-1)*(1+t*3)+(sinf(game.time*11+i)*.6f));
  int yy=y+(int)(t*(boostish?14:9)+cosf(game.time*9+i)*.4f);
  unsigned ink=(i<2)?core:(i<n/2)?edge:RGB(60,40,35);
  sfx_add(xx,yy,ink,top,bot);
  if(i<3)sfx_add(xx+1,yy,ink,top,bot);
 }
}
/* Player nozzle heat — screen-space under engine_flare, not a full-screen warp. */
static void sfx_boost_heat_shimmer(void){
 if(sfx_fx_muted()||!game.boost||game.dead||game.speed<80)return;
 int top=view_top(),bot=view_bot();if(bot<150)return;
 int y=bot-8;
 for(int i=0;i<22;i++){
  float a=i*2.39996f+game.time*14.f;
  int x=240+(int)(sinf(a)*18)+(i%5-2);
  int yy=y+(int)(cosf(a*1.3f)*3)+(i&3);
  unsigned ink=(i&3)==0?CYAN:RGB(40,90,130);
  sfx_add(x,yy,ink,top,bot);
 }
}
static void sfx_engine_plume_player(void){
 if(sfx_fx_muted()||game.dead||game.speed<80)return;
 int top=view_top(),bot=view_bot();if(bot<150)return;
 sfx_engine_plume_at(240,bot-6,game.boost,top,bot);
 if(game.boost)sfx_boost_heat_shimmer();
}
static void sfx_streak_spawn(SfxStreak *pool,int count,int sx,int sy,unsigned seed,float life,int spread){
 if(sfx_fx_muted()||sx<2||sx>=W-2)return;
 int armed=0;
 for(int i=0;i<count&&armed<8;i++){
  if(pool[i].life>0)continue;
  unsigned r=seed*1664525u+1013904223u*(unsigned)(i+1);
  float ang=((r&255)/255.f)*6.2831853f;
  int len=4+(r>>8)%spread;
  pool[i].x0=sx;pool[i].y0=sy;
  pool[i].x1=sx+(int)(cosf(ang)*len);pool[i].y1=sy+(int)(sinf(ang)*len*.7f);
  pool[i].life=pool[i].max=life*(.7f+((r>>16)&255)/500.f);
  pool[i].c=(i&1)?GOLD:RGB(220,230,255);
  armed++;
 }
}
static void sfx_hit_sparks_spawn(int sx,int sy,unsigned seed){
 sfx_streak_spawn(sfx_hits,SFX_HIT_SLOTS,sx,sy,seed,.32f,14);
}
static void sfx_explosion_embers_spawn(int sx,int sy,unsigned seed){
 sfx_streak_spawn(sfx_embers,SFX_EMBER_SLOTS,sx,sy,seed,.55f,22);
 for(int i=0;i<SFX_EMBER_SLOTS;i++)if(sfx_embers[i].life>0&&sfx_embers[i].c==GOLD)
  sfx_embers[i].c=RGB(224,76,28);
}
static void sfx_streaks_draw(SfxStreak *pool,int count,int top,int bot,float dt){
 for(int i=0;i<count;i++){
  if(pool[i].life<=0)continue;
  float u=pool[i].life/pool[i].max;if(u<0)u=0;
  int x0=pool[i].x0,y0=pool[i].y0;
  int x1=x0+(int)((pool[i].x1-x0)*u),y1=y0+(int)((pool[i].y1-y0)*u);
  if(y0>=top&&y0<=bot&&y1>=top&&y1<=bot)line(x0,y0,x1,y1,pool[i].c);
  sfx_add(x0,y0,pool[i].c,top,bot);
  sfx_add(x1,y1,pool[i].c,top,bot);
  if(u>.6f)pixel(x1,y1,WHITE);
  pool[i].life-=dt;
 }
}
static void sfx_hit_sparks_draw(float dt){
 if(high_contrast){for(int i=0;i<SFX_HIT_SLOTS;i++)sfx_hits[i].life=0;return;}
 int top=view_top(),bot=view_bot();
 sfx_streaks_draw(sfx_hits,SFX_HIT_SLOTS,top,bot,dt);
}
static void sfx_explosion_embers_draw(float dt){
 if(high_contrast){for(int i=0;i<SFX_EMBER_SLOTS;i++)sfx_embers[i].life=0;return;}
 int top=view_top(),bot=view_bot();
 sfx_streaks_draw(sfx_embers,SFX_EMBER_SLOTS,top,bot,dt);
}
/* Arm sparks on the first frames of a hull flash (flash starts ~0.12). */
static void sfx_maybe_flash_sparks(int sx,int sy,float flash,unsigned seed){
 if(flash>.095f&&flash<.13f)sfx_hit_sparks_spawn(sx,sy,seed);
}
/* Death / debris — burst once while explosion age is young. */
static void sfx_maybe_death_embers(void){
 if(sfx_fx_muted()||!game.dead)return;
 if(game.explosion>.05f&&game.explosion<.12f)sfx_explosion_embers_spawn(240,110,(unsigned)(game.explosion*1000)^0xE11Eu);
}

/* ---- Wave B: planet bloom / specular / travel beauty (soft-FB) ---- */
static unsigned sfx_planet_haze_tint(const Body *b){
 if(b->type==OCEAN)return RGB(40,90,130);
 if(b->type==GAS)return RGB(70,60,90);
 return RGB((b->color&255)/4,((b->color>>8)&255)/4,((b->color>>16)&255)/5);
}
/* Soft atmosphere shell + sun-side specular + optional ocean glint band. */
static void sfx_planet_bloom_one(const Body *b,int top,int bot){
 if(b->type==SUN)return;
 Vec3 v=camera(&game,b->pos);if(v.z<120)return;
 Point p=project(v);
 int r=(int)fminf(220,240*b->radius/v.z);if(r<4)return;
 if(p.x+r<-20||p.x-r>W+20||p.y+r<top-20||p.y-r>bot+20)return;
 unsigned haze=sfx_planet_haze_tint(b);
 /* Limb bloom — a few soft rings outside the disc (not a second framebuffer). */
 int rings=r>40?3:2;
 for(int ring=1;ring<=rings;ring++){
  int rr=r+ring*2+(ring==rings?1:0);
  int samples=24+ring*10;if(samples>56)samples=56;
  unsigned ink=RGB(((haze&255)*(rings+1-ring))/(rings+2),(((haze>>8)&255)*(rings+1-ring))/(rings+2),(((haze>>16)&255)*(rings+1-ring))/(rings+2));
  for(int s=0;s<samples;s++){
   float a=s*6.2831853f/samples+game.time*.02f+b->seed*.01f;
   sfx_add((int)p.x+(int)(cosf(a)*rr),(int)p.y+(int)(sinf(a)*rr*.92f),ink,top,bot);
  }
 }
 /* Specular — highlight toward projected sun. */
 Vec3 sv=camera(&game,game.bodies[0].pos);
 if(sv.z>100){
  Point sp=project(sv);
  float dx=sp.x-p.x,dy=sp.y-p.y,len=sqrtf(dx*dx+dy*dy);if(len<1)len=1;
  float ux=dx/len,uy=dy/len;
  int hx=(int)(p.x+ux*r*.42f),hy=(int)(p.y+uy*r*.42f);
  unsigned spec=b->type==OCEAN?RGB(180,220,255):b->type==GAS?RGB(200,190,160):RGB(220,210,180);
  int rad=2+(r>30)+(r>70);
  for(int dy=-rad;dy<=rad;dy++)for(int dx2=-rad;dx2<=rad;dx2++){
   if(dx2*dx2+dy*dy>rad*rad)continue;
   unsigned ink=RGB(((spec&255)*(rad+1-abs(dx2)))/(rad*3),(((spec>>8)&255)*(rad+1-abs(dy)))/(rad*3),(((spec>>16)&255)*(rad+1-abs(dx2)))/(rad*4));
   sfx_add(hx+dx2,hy+dy,ink,top,bot);
  }
  /* Ocean / ice reflection ribbon under the disc (screen-space fake reflection). */
  if((b->type==OCEAN||(b->type==ROCKY&&(b->seed&3)==0))&&r>12){
   int ry=(int)(p.y+r*.55f);unsigned glint=RGB(60,110,150);
   for(int i=-r/2;i<=r/2;i+=2){
    float fall=1.f-fabsf(i)/(r*.5f+.1f);if(fall<.15f)continue;
    int wobble=(int)(sinf(game.time*3.f+i*.2f+b->seed)*.8f);
    unsigned ink=RGB((int)((glint&255)*fall*.35f),(int)(((glint>>8)&255)*fall*.35f),(int)(((glint>>16)&255)*fall*.4f));
    sfx_add((int)p.x+i,ry+wobble,ink,top,bot);
    if((i&3)==0)sfx_add((int)p.x+i,ry+1+wobble,ink,top,bot);
   }
  }
 }
 /* Near-planet proximity wash — denser when close. */
 float dist=length(sub(b->pos,game.pos));
 if(dist<b->radius*8.f&&r>18){
  int motes=10+(int)fminf(18,(b->radius*4.f)/dist);
  unsigned wash=RGB((haze&255)/2,((haze>>8)&255)/2,((haze>>16)&255)/3);
  for(int i=0;i<motes;i++){
   float a=i*2.39996f+game.time*.4f+b->seed;
   float rad=r*(.7f+.35f*sinf(a*1.7f));
   sfx_add((int)(p.x+cosf(a)*rad),(int)(p.y+sinf(a)*rad*.85f),wash,top,bot);
  }
 }
 /* Ice worlds get a faint aurora ribbon above the limb. */
 if(b->type==ROCKY&&(b->seed%5)==2&&r>16){
  for(int i=0;i<18;i++){
   float t=i/17.f;float a=-.9f+t*1.8f+sinf(game.time*.7f+b->seed)*.1f;
   int x=(int)(p.x+cosf(a)*(r+4+t*6)),y=(int)(p.y-r*.55f+sinf(t*3.f+game.time)*5);
   sfx_add(x,y,RGB(40,90,70),top,bot);
   if((i&2)==0)sfx_add(x,y-1,RGB(30,120,90),top,bot);
  }
 }
 /* Night-side city lights — tiny warm sparks on the anti-sun half. */
 if(b->type!=GAS&&r>22&&sv.z>100){
  Point sp=project(sv);
  float sdx=sp.x-p.x,sdy=sp.y-p.y,slen=sqrtf(sdx*sdx+sdy*sdy);if(slen<1)slen=1;
  float nx=-sdx/slen,ny=-sdy/slen; /* night hemisphere direction */
  unsigned seed=b->seed^0xC17Fu;
  int lights=8+(r/12);if(lights>18)lights=18;
  for(int i=0;i<lights;i++){
   seed=seed*1664525u+1013904223u;
   float a=((seed&255)/255.f)*3.14159f-1.57f;
   float rad=r*(.25f+((seed>>8)&255)/400.f);
   int x=(int)(p.x+nx*r*.35f+cosf(a)*rad),y=(int)(p.y+ny*r*.35f+sinf(a)*rad*.9f);
   unsigned ink=(i&1)?RGB(255,180,80):RGB(255,220,140);
   sfx_add(x,y,ink,top,bot);
   if((i&3)==0)pixel(x,y,GOLD);
  }
 }
}
static void sfx_planet_beauty(void){
 if(sfx_fx_muted())return;
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 for(int i=1;i<BODY_COUNT;i++)sfx_planet_bloom_one(&game.bodies[i],top,bot);
}
/* Cruise haze filaments — soft additive only, no four-point glitter. */
static void sfx_travel_beauty(void){
 if(sfx_fx_muted()||game.dead)return;
 int top=view_top(),bot=view_bot();
 float normal=game.speed/fmaxf(1,player_ships[game.ship].speed);
 int n=game.boost?28:(normal>.6f?14:6);
 unsigned seed=game.bodies[0].seed^((unsigned)game.system*2654435761u);
 for(int i=0;i<n;i++){
  unsigned cell=seed*1664525u+(unsigned)(i*977)+((unsigned)(game.time*40)&255)*1013904223u;
  int x=(int)((cell>>8)%(W-8))+4;
  int y=top+8+(int)((cell>>16)%(bot-top-16));
  unsigned ink=(i%5==0)?RGB(90,120,160):(i%3==0)?RGB(50,70,100):RGB(28,40,60);
  sfx_add(x,y,ink,top,bot);
 }
 /* Motion streaks when boosting — short soft dashes toward canopy center. */
 if(game.boost){
  for(int i=0;i<18;i++){
   float a=i*2.39996f+game.time*9.f;
   float r=40+fmodf(i*29+game.time*180,160);
   int x0=240+(int)(cosf(a)*r),y0=110+(int)(sinf(a)*r*.5f);
   int x1=240+(int)(cosf(a)*(r+18)),y1=110+(int)(sinf(a)*(r+18)*.5f);
   if(y0>top&&y0<bot&&y1>top&&y1<bot){line(x0,y0,x1,y1,RGB(30,70,100));sfx_add(x1,y1,CYAN,top,bot);}
  }
 }
 /* Extra nebula filaments — 2 seeded streaks for place. */
 if(!game.boost){
  for(int f=0;f<2;f++){
   float ang=((seed>>(f*5))&255)*.02f+f*1.1f;
   float ca=cosf(ang),sa=sinf(ang);
   int x0=40+(int)((seed>>(f*3))&127),y0=top+30+((seed>>(f*7))&63);
   unsigned tint=sfx_tint(seed,f+3);
   unsigned soft=RGB((tint&255)/2,((tint>>8)&255)/2,((tint>>16)&255)/3);
   for(int s=0;s<22;s++){
    float t=s/21.f;
    int x=(int)(x0+ca*t*180),y=(int)(y0+sa*t*50+sinf(t*4+game.time*.2f)*3);
    sfx_add(x,y,soft,top,bot);
   }
  }
 }
}
/* Warm canopy wash when the sun fills the view — soft bloom, not a second buffer. */
static void sfx_sun_canopy_wash(void){
 if(sfx_fx_muted())return;
 Vec3 v=camera(&game,game.bodies[0].pos);if(v.z<80)return;
 Point p=project(v);
 int r=(int)fminf(260,240*game.bodies[0].radius/v.z);if(r<30)return;
 int top=view_top(),bot=view_bot();
 unsigned tint=game.bodies[0].color;
 unsigned wash=RGB((tint&255)/6,((tint>>8)&255)/7,((tint>>16)&255)/8);
 int samples=36;float spin=game.time*.15f;
 for(int k=0;k<samples;k++){
  float a=spin+k*6.2831853f/samples;
  int rr=r+(k&3)*3;
  sfx_add((int)p.x+(int)(cosf(a)*rr),(int)p.y+(int)(sinf(a)*rr*.9f),wash,top,bot);
 }
}
/* Prefer mask plume tip on densified NPC plumes. */
static void sfx_engine_plume_mask(int x,int y,int boostish){
 if(sfx_fx_muted())return;
 int frame=((int)(game.time*(boostish?12.f:7.f)))&3;
 unsigned ink=boostish?CYAN:RGB(255,185,70);
 space_anim_draw(SPACE_ANIM_PLUME,x,y,frame,ink);
}
/* ---- Wave C: denser fun travel FX (still soft-FB / fixed cost) ---- */
/* Coronal streamers + rainbow diffraction near the sun limb. */
static void sfx_solar_wind(void){
 if(sfx_fx_muted())return;
 Vec3 v=camera(&game,game.bodies[0].pos);if(v.z<100)return;
 Point p=project(v);
 int r=(int)fminf(200,240*game.bodies[0].radius/v.z);if(r<12)return;
 int top=view_top(),bot=view_bot();
 unsigned tint=game.bodies[0].color;
 for(int i=0;i<10;i++){
  float a=game.time*.2f+i*.63f+game.bodies[0].seed*.01f;
  int x0=(int)(p.x+cosf(a)*(r+2)),y0=(int)(p.y+sinf(a)*(r+2)*.9f);
  int x1=(int)(p.x+cosf(a)*(r+18+i)),y1=(int)(p.y+sinf(a)*(r+18+i)*.9f);
  if(y0>=top&&y0<=bot&&y1>=top&&y1<=bot)line(x0,y0,x1,y1,RGB((tint&255)/4,((tint>>8)&255)/5,((tint>>16)&255)/6));
  sfx_add(x1,y1,RGB((tint&255)/3,((tint>>8)&255)/4,((tint>>16)&255)/5),top,bot);
 }
 /* Soft prism fringe when the sun is large in frame. */
 if(r>50){
  for(int k=0;k<12;k++){
   float a=k*.52f+game.time*.4f;
   int x=(int)(p.x+cosf(a)*(r+6)),y=(int)(p.y+sinf(a)*(r+6)*.9f);
   unsigned ink=(k%3==0)?RGB(80,40,60):(k%3==1)?RGB(40,70,90):RGB(50,60,40);
   sfx_add(x,y,ink,top,bot);
  }
 }
}
/* Soft wakes behind nearby traffic. */
static void sfx_traffic_wakes(void){
 if(sfx_fx_muted())return;
 int top=view_top(),bot=view_bot();
 for(int i=0;i<NPC_COUNT;i++){
  NPC *n=&game.npc[i];if(!n->alive)continue;
  float d=length(sub(n->pos,game.pos));if(d>5500||d<80)continue;
  Vec3 facing=n->freighter?n->dir:(Vec3){n->dir.x,0,n->dir.z};
  float L=length(facing);if(L>.001f)facing=mul(facing,1.f/L);else facing=(Vec3){0,0,1};
  Point last={0,0};int have=0;
  for(int k=0;k<5;k++){
   Vec3 trail=add(n->pos,mul(facing,-(20.f+k*28.f)));
   Vec3 tv=camera(&game,trail);if(tv.z<25)break;Point q=project(tv);
   if(q.x<2||q.x>478||q.y<top+2||q.y>bot-2)break;
   unsigned ink=k<2?RGB(80,110,140):RGB(35,50,70);
   sfx_add((int)q.x,(int)q.y,ink,top,bot);
   if(have&&(int)last.y>=top&&(int)last.y<=bot)line((int)last.x,(int)last.y,(int)q.x,(int)q.y,ink);
   last=q;have=1;
  }
 }
}
/* Asteroid / debris dust motes when close to rocks. */
static void sfx_debris_dust(void){
 if(sfx_fx_muted())return;
 int top=view_top(),bot=view_bot(),drawn=0;
 for(int i=0;i<DEBRIS_COUNT&&drawn<40;i++){
  Debris *d=&game.debris[i];if(!d->alive||!d->rock)continue;
  float dist=length(sub(d->pos,game.pos));if(dist>2800)continue;
  Vec3 v=camera(&game,d->pos);if(v.z<30||v.z>4000)continue;Point p=project(v);
  unsigned seed=(unsigned)(i*7919u)^(unsigned)(game.system*131u);
  int motes=4+(int)(1800.f/dist);if(motes>10)motes=10;
  for(int k=0;k<motes;k++){
   seed=seed*1664525u+1013904223u;
   int x=(int)p.x+((int)(seed&15)-7),y=(int)p.y+((int)((seed>>4)&15)-7);
   sfx_add(x,y,d->rock==2?RGB(120,150,170):RGB(90,70,50),top,bot);
   drawn++;
  }
 }
}
/* Approach corridor — soft cyan haze toward the hub mouth (no beacon glitter). */
static void sfx_dock_corridor(void){
 if(sfx_fx_muted()||game.dock_stage)return;
 float dz=STATION_ENTRY_Z-game.pos.z;if(dz<200||dz>9000)return;
 if(fabsf(game.pos.x)>900||fabsf(game.pos.y)>900)return;
 int top=view_top(),bot=view_bot();
 for(int i=0;i<10;i++){
  float t=i/9.f;
  Vec3 w={(i&1?-1:1)*40.f*(1.f-t),((i&2)?1:-1)*28.f*(1.f-t),STATION_ENTRY_Z-t*dz*.85f};
  Vec3 v=camera(&game,w);if(v.z<20)continue;Point p=project(v);
  sfx_add((int)p.x,(int)p.y,RGB(30,90,110),top,bot);
  if((i&1)==0)sfx_add((int)p.x+1,(int)p.y,RGB(24,70,88),top,bot);
 }
}
/* Anomaly pulse rings when a rift is on-screen. */
static void sfx_anomaly_pulse(void){
 if(sfx_fx_muted())return;
 int top=view_top(),bot=view_bot();
 for(int i=0;i<ANOMALY_COUNT;i++)if(game.anomaly[i].alive){
  float d=length(sub(game.anomaly[i].pos,game.pos));if(d>12000)continue;
  Vec3 v=camera(&game,game.anomaly[i].pos);if(v.z<40)continue;Point p=project(v);
  int rad=6+(int)(sinf(game.time*3.f+i)*4)+((int)(game.time*2)+i)%5;
  unsigned ink=game.anomaly[i].kind?CYAN:GOLD;
  for(int s=0;s<24;s++){
   float a=s*6.2831853f/24;
   sfx_add((int)p.x+(int)(cosf(a)*rad),(int)p.y+(int)(sinf(a)*rad*.7f),ink,top,bot);
  }
 }
}
/* Bundle denser travel pass — call after sfx_travel_beauty. */
static void sfx_travel_fun(void){
 if(sfx_fx_muted())return;
 sfx_solar_wind();
 sfx_traffic_wakes();
 sfx_debris_dust();
 sfx_dock_corridor();
 sfx_anomaly_pulse();
}
