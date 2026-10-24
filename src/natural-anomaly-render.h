/* Natural forms share geometry between live flight and the Discovery Codex.
 * Fixed loops and screen-space sampling budgets, no allocation or file I/O. */
#include "natural-anomaly-looks.h"
static unsigned char natural_weights[23][45*45];
static unsigned char natural_row_span[23][45];
static unsigned char natural_absorption[W*H];
#ifdef NATURAL_REVIEW_PROFILING
static uint64_t natural_cloud_us,natural_light_us;
#endif
static int natural_assets_ready;
static void natural_assets_init(void){
 if(natural_assets_ready)return;
 rift_glow_assets_init();
 for(int r=1;r<=22;r++)for(int y=-r;y<=r;y++)for(int x=-r;x<=r;x++){int d=x*x+y*y,rr=r*r;if(d<rr)natural_weights[r][(y+22)*45+x+22]=(rr-d)*255/rr;}
 for(int r=1;r<=22;r++)for(int y=-r;y<=r;y++)natural_row_span[r][y+22]=(int)sqrtf(fmaxf(0,r*r-y*y-1));
 natural_assets_ready=1;
}
static int natural_solid_type(int type){return type==4||type==5||type==7||type==15||type==17||type==20||type==21||type==24||type==27||type==30;}
static Vec3 natural_path(int type,int strand,float u,float time){
 float a=u*6.2831853f,s=strand,p=time*.13f,phi=s*2.399963f;
 switch(type){
 case 4:return (Vec3){-.8f+u*1.6f,.13f*sinf(a*2+s)+s*.08f-.25f,.24f*cosf(a+s)};
 case 5:return (Vec3){cosf(a+phi)*(.55f+s*.07f),sinf(a+phi)*(.4f+s*.04f),sinf(a*2+phi)*.35f};
 case 6:return (Vec3){-.55f+u*1.4f,-.35f+s*.19f+sinf(u*3+s)*.10f,cosf(s*2.3f)*.35f+u*.18f};
 case 7:return (Vec3){-.65f+s*.24f+u*.16f,.85f-u*1.7f,sinf(s*2.1f)*.4f};
 case 8:{float r=u<.35f?sinf(u/.35f*1.57f)*.48f:.48f*(1-(u-.35f)*.42f);return (Vec3){cosf(phi+p*.2f)*r+sinf(u*9+p+s)*u*.12f,.65f-u*1.65f,sinf(phi+p*.2f)*r};}
 case 9:return (Vec3){-.95f+u*1.9f,.33f*sinf(u*7+p)+s*.035f-.10f,.25f*cosf(u*5+p)+s*.06f-.18f};
 case 10:{float side=strand&1?1:-1;return (Vec3){side*(.06f+u*.65f)*(.4f+s*.10f),-.7f+u*1.45f,sinf(s*2.f)*u*.45f+sinf(u*24+s)*.025f};}
 case 11:{float r=.33f+s*.13f+.035f*sinf(p+s);return (Vec3){cosf(a)*r,sinf(a)*r*.8f,sinf(a*3+s)*.18f+(s-2)*.07f};}
 case 12:return (Vec3){cosf(a+phi+p*.15f)*(.2f+s*.13f),sinf(a*2+phi+p*.2f)*.48f,sinf(a+phi)*(.2f+s*.10f)};
 case 13:{float r=.68f*sinf(u*3.1415926f)*(.8f+.13f*sinf(p));return (Vec3){cosf(phi)*r,sinf(phi)*r,(u-.5f)*.6f+.2f*sinf(u*3.14f)};}
 case 14:return (Vec3){-.75f+s*.29f+.08f*sinf(u*10+p+s),.9f-u*1.8f,cosf(u*5+s)*.18f};
 case 15:{float r=.72f+.08f*sinf(a*5);return (Vec3){cosf(a)*r,sinf(a)*r*.65f,sinf(a*3)*.27f};}
 case 16:{float side=strand&1?1:-1,r=.10f+u*.52f;return (Vec3){side*.31f+cosf(a*2+side*p)*r,sinf(a*2+side*p)*r*.7f,(u-.5f)*.8f};}
 case 17:return (Vec3){(s-2.5f)*.19f+u*.12f,-.7f+u*1.4f,(s-2.5f)*.09f};
 case 18:return (Vec3){(s-2)*.24f+sinf(u*6+p+s)*u*.12f,-.75f+u*1.5f,cosf(u*4+s)*u*.30f};
 case 19:return (Vec3){-.95f+u*1.9f,(s-2)*.20f+sinf(u*9+p+s*.4f)*.14f,sinf(u*5+p)*.32f+(s-2)*.10f};
 case 20:{float r=.44f+s*.08f;return (Vec3){cosf(a+p*.3f)*r,sinf(a+p*.3f)*r*(strand&1?.7f:.2f),sinf(a+p*.3f)*r*(strand&1?.2f:.7f)};}
 case 21:return (Vec3){cosf(.2f+u*2.8f)*.8f,sinf(.2f+u*2.8f)*.65f-.2f,sinf(u*7)*.15f+(s-2)*.025f};
 case 22:{float side=strand&1?1:-1,r=.35f+.035f*sinf(p);return (Vec3){side*.37f+cosf(a)*r,sinf(a)*r*.95f,sinf(a*2+s)*.18f};}
 case 23:return (Vec3){cosf(a*2+phi+p*.2f)*sinf(u*3.14f)*.43f,(u-.5f)*1.7f,sinf(a*2+phi+p*.2f)*sinf(u*3.14f)*.43f};
 case 24:{float r=.08f+u*.78f;return (Vec3){cosf(a*2.3f)*r,sinf(a*2.3f)*r*.75f,(u-.5f)*.22f+(s-2)*.015f};}
 case 25:return (Vec3){cosf(a+phi)*(.35f+s*.065f),sinf(a*3+phi)*.18f+.25f-(s*.11f),sinf(a+phi)*.36f};
 case 26:return (Vec3){-.98f+u*1.96f,sinf(u*8+p+s*2.1f)*.35f,sinf(u*8+p+s*2.1f+1.57f)*.35f};
 case 27:return (Vec3){-.65f+u*1.6f,.20f*sinf(u*5+s)+s*.04f-.1f,.15f*cosf(u*6+s)};
 case 28:{float r=.54f+.07f*sinf(a*7+s);return (Vec3){cosf(a+p*.1f)*r,sinf(a+p*.1f)*r*.48f,sinf(a*2+s)*.19f};}
 case 29:{float side=s<3?-1:1,spread=.15f+(s-(s<3?0:3))*.14f;return (Vec3){side*u*(.4f+spread),-.55f+u*1.2f,sinf(phi)*u*.65f};}
 case 30:return (Vec3){cosf(phi+u*.8f)*(.35f+u*.4f),sinf(phi+u*.8f)*(.35f+u*.4f)*.65f,sinf(s*1.77f)*.35f};
 case 31:{float side=strand&1?1:-1;return (Vec3){side*(.26f+.45f*sinf(u*3.14f)),.85f-u*1.65f,((int)s/2-1)*.38f+.09f*cosf(a+p)};}
 default:return (Vec3){0,0,0};
 }
}
static Vec3 natural_orient(Vec3 p,int type){
 /* Every phenomenon has a stable world bearing, not a camera-facing disc. */
 static int cached=-1;static float c,s;if(type!=cached){cached=type;c=cosf(type*.381f);s=sinf(type*.381f);}return (Vec3){p.x*c+p.z*s,p.y,-p.x*s+p.z*c};
}
static Vec3 natural_world(Vec3 centre,Vec3 p,int type){return add(centre,mul(natural_orient(p,type),natural_looks[type].radius));}
static void natural_facet(Vec3 centre,Vec3 a,Vec3 b,Vec3 c,int type,int face){
 a=natural_world(centre,a,type);b=natural_world(centre,b,type);c=natural_world(centre,c,type);
 Vec3 u=sub(b,a),v=sub(c,a),n=norm((Vec3){u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x});
 if(dot(n,sub(camera_origin(&game),a))<=0)return;
 Vec3 sun=norm(sub(game.bodies[0].pos,centre));float light=.20f+.78f*fmaxf(0,dot(n,sun));
 unsigned paint=face%4==0?natural_looks[type].accent:natural_looks[type].ink;
 if(type==15)light*=.6f;
 unsigned ink=RGB((int)((paint&255)*light),(int)(((paint>>8)&255)*light),(int)(((paint>>16)&255)*light));
 queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),ink);
}
static void natural_crystal(Vec3 centre,Vec3 p,Vec3 e,int type,float angle,int rough){
 Vec3 q[6]={{0,e.y,0},{0,-e.y*.66f,0},{e.x,0,0},{0,0,e.z},{-e.x,0,0},{0,0,-e.z}};
 float c=cosf(angle),s=sinf(angle);
 for(int k=0;k<6;k++){float x=q[k].x,y=q[k].y;q[k].x=x*c-y*s;q[k].y=x*s+y*c;if(rough)q[k].z+=sinf(k*2.3f+angle)*e.z*.25f;q[k]=add(q[k],p);}
 static const unsigned char f[8][3]={{0,3,2},{0,4,3},{0,5,4},{0,2,5},{1,2,3},{1,3,4},{1,4,5},{1,5,2}};
 for(int k=0;k<8;k++)natural_facet(centre,q[f[k][0]],q[f[k][1]],q[f[k][2]],type,k);
}
static void natural_solids_at(Vec3 centre,int type,float time,float pixels){
 if(!natural_solid_type(type))return;int n=pixels<14?4:pixels<35?8:14;
 if(type==5||type==20||type==27){natural_crystal(centre,(Vec3){0,0,0},(Vec3){.31f,.36f,.31f},type,.6f,1);if(type==20)return;}
 for(int k=0;k<n;k++){
  float u=(k+.5f)/n,a=k*2.399963f;Vec3 p=natural_path(type,k%6,u,time),e={.055f,.18f,.055f};float tilt=.25f*sinf(a);
  switch(type){
  case 4:e=(Vec3){.075f,.18f+.12f*sinf(k*2.1f),.075f};tilt=.5f*sinf(a);break;
  case 5:e=(Vec3){.10f,.11f,.12f};tilt=a;break;
  case 7:e=(Vec3){.045f,.075f,.045f};p.y+=fmodf(time*.065f+k*.17f,1.f)*.22f;tilt=a;break;
  case 15:e=(Vec3){.10f,.24f,.12f};tilt=-atan2f(p.x,p.y);break;
  case 17:p=natural_path(type,k%6,.5f,time);e=(Vec3){.025f,.45f+.15f*sinf(k*3.f),.035f};tilt=(k%6-2.5f)*.17f;break;
  case 21:e=(Vec3){.065f,.14f,.065f};tilt=-atan2f(p.x,p.y+.2f);break;
  case 24:e=(Vec3){.06f,.085f,.09f};tilt=a;break;
  case 27:e=(Vec3){.16f*(1-u*.6f),.13f,.13f};p.x-=.25f;tilt=a;break;
  case 30:e=(Vec3){.13f,.035f,.08f};tilt=time*.06f+k;break;
  }
  natural_crystal(centre,p,e,type,tilt,type==5||type==27||type==15);
 }
}
static int natural_visible(const Anomaly*a,int type){
 float r=type<4?300:natural_looks[type].radius*1.6f;
 return a->alive&&length(sub(a->pos,game.pos))<24000&&render_sphere_visible(a->pos,r)&&(!natural_solid_type(type)||!rift_body_hidden(a->pos));
}
static void natural_solids(void){
 for(int i=0;i<ANOMALY_COUNT;i++){int type=rift_type(game.system,i);const Anomaly*a=&game.anomaly[i];if(type>=4&&natural_visible(a,type))natural_solids_at(a->pos,type,game.time,render_pixel_radius(a->pos,natural_looks[type].radius));}
}
static int natural_cached_view(void){
 if(!flight_cached_frame||page!=FLIGHT||clipy0>=0||game.docked||game.dead||game.planet>=0||game.approach>=0||game.jump>0||game.dock_stage>=2||race_ready(&game)||opening_dream(&game))return 0;
 for(int i=0;i<ANOMALY_COUNT;i++){const Anomaly*a=&game.anomaly[i];int type=rift_type(game.system,i);if(natural_visible(a,type)&&render_pixel_radius(a->pos,natural_looks[type].radius)>6)return 1;}
 return 0;
}
static unsigned natural_samples;
static void natural_light(Vec3 p,unsigned ink,int radius,float gain){if(natural_samples++<1800)rift_volume_sample(p,ink,radius,gain);}
static void natural_segment(Vec3 a,Vec3 b,unsigned ink,int bright){
 Vec3 av=camera(&game,a),bv=camera(&game,b);if(av.z<=12||bv.z<=12)return;
 Point ap=project(av),bp=project(bv);float len=fmaxf(fabsf(ap.x-bp.x),fabsf(ap.y-bp.y));
 int steps=(int)len+1;if(steps>6)steps=6;
 for(int k=0;k<steps;k++)natural_light(add(a,mul(sub(b,a),k/(float)steps)),ink,bright?2:1,bright?.36f:.72f);
}
static void __attribute__((optimize("O3"))) natural_cloud(Vec3 centre,int type,float time,float pixels){
 if(type!=22&&type!=25&&type!=28)return;
 natural_assets_init();
 /* Dark dust absorbs background light. Respect the same foreground mask as
  * luminous samples, so clouds cannot stain a nearby station or spacecraft. */
 const NaturalLook*l=&natural_looks[type];
 memset(natural_absorption,0,sizeof(natural_absorption));int minx=W,maxx=-1,miny=H,maxy=-1,top=view_top(),bot=view_bot();
 for(int k=0;k<18;k++){
  float a=k*2.399963f,side=k&1?1:-1;Vec3 q={type==22?side*.33f+cosf(a)*.20f:cosf(a)*.22f,sinf(a)*.32f,sinf(k*1.8f)*.28f};
  q.x*=1+.03f*sinf(time*.2f);Vec3 world=natural_world(centre,q,type);if(rift_body_hidden(world))continue;Vec3 v=camera(&game,world);if(v.z<12)continue;Point p=project(v);
  int r=(int)fminf(22,fmaxf(2,pixels*.18f));unsigned depth=surface_encode_depth(1.f/v.z);
  int cx=(int)p.x,cy=(int)p.y;
  int x0=cx-r<0?0:cx-r,x1=cx+r>=W?W-1:cx+r,y0=cy-r<top?top:cy-r,y1=cy+r>bot?bot:cy+r;
  if(x0>x1||y0>y1)continue;if(x0<minx)minx=x0;if(x1>maxx)maxx=x1;if(y0<miny)miny=y0;if(y1>maxy)maxy=y1;
  for(int y=y0;y<=y1;y++){int yy=y-cy,span=natural_row_span[r][yy+22],left=cx-span<0?0:cx-span,right=cx+span>=W?W-1:cx+span;
   for(int x=left;x<=right;x++){
   if(mega_depth_valid&&depth>surface_depth[y*W+x])continue;
   int alpha=natural_weights[r][(yy+22)*45+x-cx+22]*(type==25?33:66)>>8,at=y*W+x,old=natural_absorption[at];
   natural_absorption[at]=old+((256-old)*alpha>>8);
   }
  }
 }
 /* Composite overlapping cloud layers into one mask, then read/colour each
  * framebuffer pixel once. Keeps the same soft layered opacity with far less
  * overdraw; this mask is transient renderer state, never saved gameplay. */
 for(int y=miny;y<=maxy;y++)for(int x=minx;x<=maxx;x++){
  int alpha=natural_absorption[y*W+x];if(!alpha)continue;int inv=256-alpha;unsigned old=fb[y*STRIDE+x];
  fb[y*STRIDE+x]=RGB(((old&255)*inv+(l->ink&255)*alpha)>>8,(((old>>8)&255)*inv+((l->ink>>8)&255)*alpha)>>8,(((old>>16)&255)*inv+((l->ink>>16)&255)*alpha)>>8);
 }
}
static void natural_volume(Vec3 centre,int type,float time,float pixels){
#ifdef NATURAL_REVIEW_PROFILING
 uint64_t clock=sceKernelGetSystemTimeWide();
#endif
 const NaturalLook*l=&natural_looks[type];natural_samples=0;natural_cloud(centre,type,time,pixels);
#ifdef NATURAL_REVIEW_PROFILING
 natural_cloud_us+=sceKernelGetSystemTimeWide()-clock;clock=sceKernelGetSystemTimeWide();
#endif
 int strands=pixels<30?3:6,segments=pixels<30?12:pixels<75?20:28;
 for(int s=0;s<strands;s++){
  Vec3 previous={0,0,0};
  for(int j=0;j<=segments;j++){
   float u=j/(float)segments;Vec3 p=natural_world(centre,natural_path(type,s,u,time),type);
   unsigned ink=s&1?l->accent:l->ink;float pulse=.50f+.5f*sinf(time*1.1f-s*.7f-u*8);
   if(type==10||type==25||type==29)ink=mix_rgb(ink,l->core,pulse*.6f);
   /* Shoals and pearls use separated moving knots; mineral forms retain
    * sparse glints so their opaque facets carry the silhouette. */
   int dotted=type==7||type==12||type==30||natural_solid_type(type);
   if(j&&(!dotted||j%4==0)&&(type!=11||j%9>1))natural_segment(previous,p,ink,(s+j)%8==0);
   if((j+s*3)%11==0){natural_light(p,l->core,pixels<15?1:3,.26f+.19f*pulse);natural_light(p,l->core,1,.65f);}
   /* A restrained haze around broad plasma forms supplies volume, while
    * the bright narrow filament retains the readable pixel-art structure. */
   if(!natural_solid_type(type)&&j%5==0)natural_light(p,ink,pixels<15?2:pixels<60?4:7,.075f);
   previous=p;
  }
 }
 if(type==10||type==29){
  /* Actual secondary and tertiary forks, rather than six unrelated rays. */
  for(int s=0;s<6;s++)for(int fork=0;fork<3;fork++){
   float u=.30f+fork*.23f;Vec3 root=natural_path(type,s,u,time);float side=s&1?1:-1;
   Vec3 tip=add(root,(Vec3){side*(.14f+fork*.035f),.14f,sinf(s*2.1f+fork)*.15f});
   Vec3 knee=add(mul(add(root,tip),.5f),(Vec3){.016f*sinf(time*1.4f+s+fork),0,.018f});
   natural_segment(natural_world(centre,root,type),natural_world(centre,knee,type),l->accent,0);
   natural_segment(natural_world(centre,knee,type),natural_world(centre,tip,type),l->core,0);
   for(int branch=-1;branch<=1;branch+=2){Vec3 end=add(tip,(Vec3){branch*.075f,.075f,.03f*sinf(s+branch)});natural_segment(natural_world(centre,tip,type),natural_world(centre,end,type),l->ink,0);}
  }
 }
 if(type==6)for(int s=0;s<6;s++){Vec3 head=natural_world(centre,natural_path(type,s,0,time),type);natural_light(head,l->ink,6,.18f);natural_light(head,l->core,2,.8f);}
 if(type==28)for(int s=-1;s<=1;s+=2)for(int j=0;j<28;j++){float u=j/27.f;Vec3 p={.5f+u*.65f,s*(.16f+u*.15f)+sinf(u*8+time*.12f)*.06f,u*.18f};natural_light(natural_world(centre,p,type),l->accent,1,(1-u)*.5f);}
 /* Asymmetric external motes give depth and a smooth, non-random twinkle. */
 for(int k=0;k<(pixels<30?8:24);k++){float a=k*2.399963f+time*.012f;Vec3 p={cosf(a)*(.3f+(k%7)*.1f),sinf(a)*.67f,sinf(k*1.71f)*.63f};natural_light(natural_world(centre,p,type),k%3?l->ink:l->accent,1,.22f+.13f*sinf(time*.7f+k));}
#ifdef NATURAL_REVIEW_PROFILING
 natural_light_us+=sceKernelGetSystemTimeWide()-clock;
#endif
}
static void natural_portrait(int cx,int cy,float radius,int type,float time,int top,int bot){
 const NaturalLook*l=&natural_looks[type];
 for(int s=0;s<6;s++)for(int j=0;j<40;j++){
  Vec3 p=natural_orient(natural_path(type,s,j/39.f,time),type);int x=cx+p.x*radius,y=cy-p.y*radius;
  rift_glow(x,y,natural_solid_type(type)?2:1,s&1?l->accent:l->ink,top,bot);
  if(j%11==0)rift_glow(x,y,3,l->core,top,bot);
 }
}
