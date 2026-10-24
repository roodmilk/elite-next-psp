/* Native Shockwave Flyby v6. Fixed geometry/effect pools; no video, allocation,
 * save writes, gameplay camera changes or payments in this renderer. */
#include "erl-shockwave-motion.h"
#define SW_SLAB_STARS 128
/* Blend in cached main memory, then copy once to uncached display memory. */
/* Flight results and Command Deck opening cannot overlap; share its scratch plane. */
#define sw_pixels dc_open_source

static int sw_slab_id[7],sw_slab_valid[7];
static unsigned char sw_sun_halo[141*141];
#define SW_SKY 192
typedef struct {Vec3 p;float phase,rate,light;} SwStar;
static SwStar sw_slab[7][SW_SLAB_STARS];
typedef struct {Vec3 p,v,offset[3];float spin;unsigned ink;} SwShard;
static struct {SwStar sky[SW_SKY];SwShard shard[128];int ready,shards,place;} sw_world;
static ErlShockPose sw_view;static float sw_bank_c,sw_bank_s;
static unsigned sw_seed=428;
static int sw_measure_enabled,sw_measure_frames;static uint64_t sw_measure_tick,sw_measure_us[6];
static void sw_measure(int phase){if(!sw_measure_enabled)return;uint64_t now=sceKernelGetSystemTimeWide();if(phase>=0)sw_measure_us[phase]+=now-sw_measure_tick;sw_measure_tick=now;}
static float sw_rand(void){sw_seed=sw_seed*1664525u+1013904223u;return (sw_seed>>8)*(1.f/16777216.f);}
static Vec3 sw_unproject(float x,float y){
 Vec3 f=norm((Vec3){-230,-62,0}),r={0,0,1},u=sw_cross(f,r);float depth=sqrtf(230*230+62*62);
 return add(add(add((Vec3){230,62,24000*(SW_IMPACT-1.1f)},mul(f,depth)),mul(r,(x-240)*depth/238)),mul(u,(142-y)*depth/238));
}
static void sw_number_layout(int place,char *number,int *x,int *scale){
 if(place<1)place=1;if(place>99)place=99;snprintf(number,3,"%d",place);
 int digits=(int)strlen(number);*scale=digits==1?23:11;*x=digits==1?266:250;
}
static void sw_init(void){
 int place=game.erl.result_place;if(place<1)place=1;
 if(sw_world.ready&&sw_world.place==place)return;memset(&sw_world,0,sizeof(sw_world));memset(sw_slab_valid,0,sizeof(sw_slab_valid));sw_world.ready=1;sw_world.place=place;sw_seed=428;
 for(int y=-70;y<=70;y++)for(int x=-70;x<=70;x++){float d=(x*x+y*y)/(71.f*71.f);sw_sun_halo[(y+70)*141+x+70]=d<1?(int)((1-d)*(1-d)*42):0;}
 for(int i=0;i<SW_SKY;i++){float y=sw_rand()*2-1,a=sw_rand()*6.2831853f,r=sqrtf(1-y*y);sw_world.sky[i]=(SwStar){{cosf(a)*r,y,sinf(a)*r},i*2.399f,.7f+(i%17)*.073f,.4f+sw_rand()*.6f};}

 const unsigned colors[]={EP,ET,ER,ER,EC};char number[3];int nx,scale;sw_number_layout(place,number,&nx,&scale);
 for(int digit=0;number[digit];digit++)for(int y=0;y<8;y++)for(int x=0;x<8;x++)if(font8[number[digit]-32][y]&(128>>x))for(int k=0;k<5&&sw_world.shards<128;k++){
  SwShard *s=&sw_world.shard[sw_world.shards++];float a=k*1.570796327f,b=a+1.570796327f;
  float px[3]={11.5f,11.5f+cosf(a-.785398f)*16.263f,11.5f+cosf(b-.785398f)*16.263f};
  float py[3]={11.5f,11.5f+sinf(a-.785398f)*16.263f,11.5f+sinf(b-.785398f)*16.263f};
  float cx=(px[0]+px[1]+px[2])/3,cy=(py[0]+py[1]+py[2])/3;
  float sx=nx+(digit*8+x)*scale+cx*scale/23.f,sy=42+y*scale+cy*scale/23.f,dx=sx-365,dy=sy-142,rr=fmaxf(1,sqrtf(dx*dx+dy*dy)),speed=90+sw_rand()*160;
  s->p=sw_unproject(sx,sy);s->v=(Vec3){(sw_rand()-.5f)*220,-dy/rr*speed+10,24000+900+sw_rand()*1850+dx/rr*speed};s->spin=(sw_rand()-.5f)*9;s->ink=colors[(int)(sw_rand()*5)];
  for(int j=0;j<3;j++)s->offset[j]=sub(sw_unproject(sx+px[j]-cx,sy+py[j]-cy),s->p);
 }
}
static Vec3 sw_camera(Vec3 p){Vec3 v=sub(p,sw_view.eye);return (Vec3){dot(v,sw_view.right),dot(v,sw_view.up),dot(v,sw_view.forward)};}
static int sw_project_pose(Vec3 p,const ErlShockPose *pose,Point *out){Vec3 v=sub(p,pose->eye);float z=dot(v,pose->forward);if(z<14)return 0;*out=(Point){240+dot(v,pose->right)*238/z,142-dot(v,pose->up)*238/z,z};return isfinite(out->x)&&isfinite(out->y);}
static int sw_project(Vec3 p,Point *out){return sw_project_pose(p,&sw_view,out);}
/* Packed integer alpha: two independent RB lanes plus G, <=1 colour-step
 * rounding difference from /255, no divisions in the effects raster loop. */
static unsigned sw_pixel_mix(unsigned a,unsigned b,int alpha){unsigned t=alpha+1,u=256-t;return 0xff000000u|((((a&0x00ff00ffu)*u+(b&0x00ff00ffu)*t)>>8)&0x00ff00ffu)|((((a&0x0000ff00u)*u+(b&0x0000ff00u)*t)>>8)&0x0000ff00u);}
static void sw_blend(int x,int y,unsigned ink,int alpha,float z){
 if(x<0||x>=W||y<0||y>=H||alpha<=0||z<4)return;int at=y*W+x;
 if(surface_encode_depth(1.f/z)>surface_depth[at])return;
 unsigned *p=&fb[y*STRIDE+x];*p=sw_pixel_mix(*p,ink,alpha>255?255:alpha);
}
static void sw_ray(Vec3 from,Vec3 to,unsigned ink,int alpha,int radius){
 Vec3 a=sw_camera(from),b=sw_camera(to);if(a.z<14&&b.z<14)return;
 if(a.z<14)a=add(a,mul(sub(b,a),(14-a.z)/(b.z-a.z)));if(b.z<14)b=add(b,mul(sub(a,b),(14-b.z)/(a.z-b.z)));
 Point p=project(a),q=project(b);float dx=q.x-p.x,dy=q.y-p.y,lo=0,hi=1;
 float ds[4]={-dx,dx,-dy,dy},es[4]={p.x,W-1-p.x,p.y,H-1-p.y};
 for(int k=0;k<4;k++){if(fabsf(ds[k])<.00001f){if(es[k]<0)return;continue;}float v=es[k]/ds[k];if(ds[k]<0)lo=fmaxf(lo,v);else hi=fminf(hi,v);if(lo>hi)return;}
 int steps=(int)fminf(600,ceilf(fmaxf(fabsf(dx),fabsf(dy))*(hi-lo)));if(steps<1)steps=1;
 for(int i=0;i<=steps;i++){float v=lo+(hi-lo)*i/steps,z=1/((1-v)/a.z+v/b.z);int x=(int)(p.x+dx*v),y=(int)(p.y+dy*v);for(int yy=-radius;yy<=radius;yy++)for(int xx=-radius;xx<=radius;xx++)sw_blend(x+xx,y+yy,ink,alpha,z);}
}
static void sw_triangle(Vec3 a,Vec3 b,Vec3 c,unsigned ink){queue_triangle(sw_camera(a),sw_camera(b),sw_camera(c),ink);}
static void sw_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){sw_triangle(a,b,c,ink);sw_triangle(a,c,d,ink);}
static void sw_cloud(Vec3 p,float radius,unsigned ink,int alpha){
 Point q;if(!sw_project(p,&q))return;float fade=sw_ease((q.z-14)/50);int r=(int)fminf(24,fmaxf(1,radius*238/q.z));
 int x0=(int)fmaxf(0,(int)q.x-r),x1=(int)fminf(W-1,(int)q.x+r),y0=(int)fmaxf(0,(int)q.y-r),y1=(int)fminf(H-1,(int)q.y+r);
 unsigned depth=surface_encode_depth(1.f/q.z);float scale=1.f/(r*r+1.f);for(int y=y0;y<=y1;y++)for(int x=x0;x<=x1;x++){if(depth>surface_depth[y*W+x])continue;float dx=x-q.x,dy=y-q.y,d=(dx*dx+dy*dy)*scale;if(d<1){int opacity=(int)(alpha*fade*(1-d)*(1-d));if(opacity>0){unsigned *p=&fb[y*STRIDE+x];*p=sw_pixel_mix(*p,ink,opacity);}}}
}
static int sw_ribbon_samples;
static const float sw_flame_taper[13]={1.000000000f,0.945012244f,0.888243811f,0.829449255f,0.768317423f,0.704443094f,0.637280314f,0.566060294f,0.489633568f,0.406126198f,0.312033834f,0.198853020f,0.000000000f};
static void sw_alpha_triangle(Vec3 wa,Vec3 wb,Vec3 wc,unsigned ink,int alpha){
 Vec3 in[4]={sw_camera(wa),sw_camera(wb),sw_camera(wc)},out[4];int count=0;
 for(int i=0;i<3;i++){Vec3 a=in[i],b=in[(i+1)%3];int ai=a.z>=14,bi=b.z>=14;if(ai)out[count++]=a;if(ai!=bi)out[count++]=add(a,mul(sub(b,a),(14-a.z)/(b.z-a.z)));}
 for(int k=1;k<count-1;k++){
  Point p[3]={project(out[0]),project(out[k]),project(out[k+1])};float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.01f)continue;
  int first=(int)fmaxf(0,ceilf(fminf(p[0].y,fminf(p[1].y,p[2].y))-.5f)),last=(int)fminf(H-1,floorf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))-.5f));
  for(int y=first;y<=last;y++){float yy=y+.5f,left=1e9f,right=-1e9f;for(int e=0;e<3;e++){Point a=p[e],b=p[(e+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*(b.x-a.x)/(b.y-a.y);left=fminf(left,x);right=fmaxf(right,x);}}
   int x0=(int)fmaxf(0,ceilf(left-.5f)),x1=(int)fminf(W-1,floorf(right-.5f));
   float xx=x0+.5f,inv=(edge(p[1],p[2],xx,yy)/p[0].z+edge(p[2],p[0],xx,yy)/p[1].z+edge(p[0],p[1],xx,yy)/p[2].z)/area;
   float step=((p[2].y-p[1].y)/p[0].z+(p[0].y-p[2].y)/p[1].z+(p[1].y-p[0].y)/p[2].z)/area;
   for(int x=x0;x<=x1&&sw_ribbon_samples<100000;x++,sw_ribbon_samples++,inv+=step){if(inv>0&&surface_encode_depth(inv)<=surface_depth[y*W+x]){unsigned *pixel=&fb[y*STRIDE+x];*pixel=sw_pixel_mix(*pixel,ink,alpha);}}
  }
 }
}
static void sw_ribbon(Vec3 a,Vec3 b,float wa,float wb,unsigned ink,int alpha){
 Vec3 aa=add(a,mul(sw_view.up,wa)),ab=add(a,mul(sw_view.up,-wa)),ba=add(b,mul(sw_view.up,wb)),bb=add(b,mul(sw_view.up,-wb));
 sw_alpha_triangle(aa,ab,bb,ink,alpha);sw_alpha_triangle(aa,bb,ba,ink,alpha);
}
static unsigned sw_hash(unsigned h){h^=h>>16;h*=2246822519u;h^=h>>13;h*=3266489917u;return h^(h>>16);}
static float sw_unit(unsigned h){return (h>>8)*(1.f/16777216.f);}
static void sw_stars(float wall){
 ErlShockPose old=sw_pose(fmaxf(0,wall-.028f));
 for(int layer=0;layer<2;layer++){int count=layer?7*SW_SLAB_STARS:SW_SKY;int cell=(int)floorf(sw_view.eye.z/12000)-3;
  for(int i=0;i<count;i++){SwStar *s;
   if(layer){int slab=cell+i/SW_SLAB_STARS,slot=((slab%7)+7)%7;
    if(!sw_slab_valid[slot]||sw_slab_id[slot]!=slab){sw_slab_valid[slot]=1;sw_slab_id[slot]=slab;for(int n=0;n<SW_SLAB_STARS;n++){unsigned hash=sw_hash((unsigned)slab*7919u+n*374761393u+428u);sw_slab[slot][n]=(SwStar){{(sw_unit(hash)-.5f)*18000,(sw_unit(sw_hash(hash+1))-.5f)*12000,(slab+sw_unit(sw_hash(hash+2)))*12000},(hash&65535)*.003f,.8f+(hash%13)*.11f,.4f+sw_unit(sw_hash(hash+3))*.6f};}}
    s=&sw_slab[slot][i%SW_SLAB_STARS];}else s=&sw_world.sky[i];
   Vec3 world=layer?s->p:add(sw_view.eye,mul(s->p,60000));Point p;if(!sw_project(world,&p)||p.x<0||p.x>=480||p.y<0||p.y>=272)continue;
   float distance=layer?length(sub(s->p,sw_view.eye)):0;if(layer&&distance>32000)continue;
   float pulse=.5f+.5f*sinf(wall*s->rate+s->phase);pulse=.32f+.68f*pulse*pulse;float alpha=s->light*pulse*(layer?sw_ease((32000-distance)/12000):1);
   Point previous;Vec3 wp=layer?s->p:add(old.eye,mul(s->p,60000));if(sw_project_pose(wp,&old,&previous)){
    float dx=previous.x-p.x,dy=previous.y-p.y,len=sqrtf(dx*dx+dy*dy),trail=fminf(len,layer?24:8);int n=(int)trail;
    if(len>.65f)for(int k=0;k<=n;k++)sw_blend((int)(p.x+dx/len*k),(int)(p.y+dy/len*k),RGB(109,183,211),(int)(alpha*100*(1-k/(n+1.f))),60000);
   }
   unsigned ink=s->light>.9f?RGB(213,232,239):s->light>.55f?RGB(131,171,199):RGB(66,110,150);sw_blend(p.x,p.y,ink,(int)(alpha*255),60000);
   if(s->light>.975f){float shimmer=sw_ease((pulse-.62f)/.35f);int r=1+(int)(shimmer*2);for(int k=-r;k<=r;k++){sw_blend(p.x+k,p.y,ET,(int)(shimmer*alpha*150),60000);sw_blend(p.x,p.y+k,EC,(int)(shimmer*alpha*180),60000);}}
  }
 }
}
typedef struct {float scale;Vec3 centre;float nose;int mesh,mega;Vec3 engines[8];int count;} SwHull;
static SwHull sw_hull(int ship){
 /* All 23 current hulls remain cached; future hulls use the exact fallback. */
 static SwHull cache[24];static unsigned char valid[24];if(ship>=0&&ship<24&&valid[ship])return cache[ship];
 SwHull h={0};h.mesh=mesh_id(player_ships[ship].name);h.mega=mega_hull_art_id(player_ships[ship].name);Vec3 min={1e9f,1e9f,1e9f},max={-1e9f,-1e9f,-1e9f};
 if(h.mega>=0){const MegaHullArt *m=&mega_hull_art[h.mega];min=(Vec3){-m->hx,-m->hy,-m->hz};max=(Vec3){m->hx,m->hy,m->hz};for(int i=0;i<m->parts&&h.count<8;i++)if(m->part[i].shade==3){const MegaHullPart *p=&m->part[i];h.engines[h.count++]=(Vec3){-p->x,p->y,-p->z-p->hz-.6f};}}
 else {const Mesh *m=&meshes[h.mesh];for(int i=0;i<m->vertices;i++){Vec3 v=m->v[i];min=(Vec3){fminf(min.x,v.x),fminf(min.y,v.y),fminf(min.z,v.z)};max=(Vec3){fmaxf(max.x,v.x),fmaxf(max.y,v.y),fmaxf(max.z,v.z)};}
  if(!strcmp(m->name,"NEEDLEFISH"))h.engines[h.count++]=(Vec3){40.5f,0,-66.6f};
  else if(!strcmp(m->name,"COBRA MK 3")){h.engines[h.count++]=(Vec3){-22,-2,-40.6f};h.engines[h.count++]=(Vec3){22,-2,-40.6f};}
  else for(int side=-1;side<=1;side+=2)for(int attempt=0;attempt<3;attempt++){
   float width=fmaxf(fabsf(min.x),fabsf(max.x)),x=side*width*(attempt==0?.32f:attempt==1?.14f:0),y=(min.y+max.y)*.5f,z=1e9f;int found=0;
   for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];Vec3 a=m->v[t->a],b=m->v[t->b],c=m->v[t->c];float d=(b.y-c.y)*(a.x-c.x)+(c.x-b.x)*(a.y-c.y);if(fabsf(d)<.0001f)continue;float u=((b.y-c.y)*(x-c.x)+(c.x-b.x)*(y-c.y))/d,v=((c.y-a.y)*(x-c.x)+(a.x-c.x)*(y-c.y))/d;if(u<-.0001f||v<-.0001f||u+v>1.0001f)continue;z=fminf(z,u*a.z+v*b.z+(1-u-v)*c.z);found=1;}
   if(found){Vec3 root={x,y,z-.6f};if(!h.count||length(sub(root,h.engines[0]))>.1f)h.engines[h.count++]=root;break;}
  }
 }
 if(h.mega>=0){const MegaHullArt *m=&mega_hull_art[h.mega];
  if(!h.count)h.engines[h.count++]=(Vec3){0,0,-m->hz-.6f};
  /* An engine root must clear every intervening aft hull section. */
  for(int e=0;e<h.count;e++)for(int j=0;j<m->parts;j++){const MegaHullPart *b=&m->part[j];if(fabsf(h.engines[e].x+b->x)<=b->hx&&fabsf(h.engines[e].y-b->y)<=b->hy)h.engines[e].z=fminf(h.engines[e].z,-b->z-b->hz-.6f);}
 }
 h.centre=mul(add(min,max),.5f);h.scale=fminf(128/fmaxf(1,(max.x-min.x)*.5f),fminf(40/fmaxf(1,(max.y-min.y)*.5f),210/fmaxf(1,max.z-min.z)));
 h.nose=max.z;if(ship>=0&&ship<24){cache[ship]=h;valid[ship]=1;}return h;
}
static Vec3 sw_rotate(Vec3 v){return (Vec3){v.x*sw_bank_c-v.y*sw_bank_s,v.x*sw_bank_s+v.y*sw_bank_c,v.z};}
static Vec3 sw_local(const SwHull *h,Vec3 v){v=mul(sub(v,h->centre),h->scale);v.z+=90-(h->nose-h->centre.z)*h->scale;return add(sw_view.ship,sw_rotate(v));}
static unsigned sw_paint(unsigned paint,Vec3 normal,float sun){
 float shade=.3f+.7f*fmaxf(0,dot(normal,norm((Vec3){-.5f,.8f,.8f}))),warm=sun*fmaxf(0,dot(normal,norm((Vec3){.7f,.5f,-.2f})));
 int r=fminf(255,(paint&255)*shade+warm*80),g=fminf(255,((paint>>8)&255)*shade+warm*63),b=fminf(255,((paint>>16)&255)*shade+warm*22);return RGB(r,g,b);
}
static void sw_hull_face(const SwHull *h,Vec3 a,Vec3 b,Vec3 c,Vec3 normal,unsigned paint,float sun){
 Vec3 n=sw_rotate(normal),wa=sw_local(h,a),wb=sw_local(h,b),wc=sw_local(h,c);if(dot(n,sub(sw_view.eye,mul(add(add(wa,wb),wc),1.f/3)))<=0)return;
 sw_triangle(wa,wb,wc,sw_paint(paint,n,sun));
}
static void sw_ship(const SwHull *h,unsigned paint,float sun){
 if(h->mega<0){const Mesh *m=&meshes[h->mesh];for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];sw_hull_face(h,m->v[t->a],m->v[t->b],m->v[t->c],t->normal,paint,sun);}}
 else {const MegaHullArt *m=&mega_hull_art[h->mega];static const int sides[6][4]={{0,2,3,1},{4,5,7,6},{0,1,5,4},{2,6,7,3},{0,4,6,2},{1,3,7,5}};
  for(int i=0;i<m->parts;i++){const MegaHullPart *p=&m->part[i];Vec3 v[8];for(int k=0;k<8;k++){float taper=k&4?1:.62f;v[k]=(Vec3){-p->x+((k&1)?p->hx:-p->hx)*taper,p->y+((k&2)?p->hy:-p->hy)*(.78f+.22f*taper),-p->z+((k&4)?p->hz:-p->hz)};}for(int s=0;s<6;s++){const int *q=sides[s];unsigned ink=p->shade==3?RGB(44,72,87):livery_tint(paint,p->shade*8);Vec3 normal=norm(sw_cross(sub(v[q[1]],v[q[0]]),sub(v[q[2]],v[q[0]])));sw_hull_face(h,v[q[0]],v[q[1]],v[q[2]],normal,ink,sun);sw_hull_face(h,v[q[0]],v[q[2]],v[q[3]],normal,ink,sun);}}
 }
}
static Vec3 sw_flame(Vec3 root,float u,float length,float t,float phase){float env=u*u*(3-2*u);return add(root,sw_rotate((Vec3){sinf(t*21-u*8+phase)*env*3,sinf(t*15-u*6-phase)*env*2,-u*length}));}
static void sw_exhaust(const SwHull *h,float t,float power){
 sw_ribbon_samples=0;
 for(int e=0;e<h->count;e++){
  Vec3 root=sw_local(h,h->engines[e]);float phase=e*2.173f+h->engines[e].x*.173f,len=(60+135*power)*(1+.10f*sinf(t*19+phase)+.055f*sinf(t*43-phase));
  /* Stable port centres; independent downstream curls, no screen-space beams. */
  for(int i=0;i<8;i++){float age=fmodf(t*(.62f+(i%3)*.09f)+i/8.f+phase*.07f,1);if(age<0)age+=1;float angle=i*2.399f+t*2,spread=age*(6+age*22);Vec3 p=add(root,sw_rotate((Vec3){cosf(angle)*spread,sinf(angle*.8f)*spread*.65f,-age*(220+power*160)}));sw_cloud(p,3+age*13,i%3?RGB(76,138,255):ER,(int)(sinf(age*3.141593f)*sw_ease(age/.12f)*46));}
  const unsigned colors[]={RGB(75,169,239),ET,RGB(234,255,255)};const float widths[]={8,3,1};const int alpha[]={145,210,255};
  for(int layer=0;layer<3;layer++){float reach=layer?1-layer*.13f+.04f*sinf(t*(13+layer*4)+phase):1;
   for(int j=0;j<12;j++){float u=j/12.f*reach,v=(j+1)/12.f*reach;Vec3 a=sw_flame(root,u,len,t,phase),b=sw_flame(root,v,len,t,phase);
    float width=widths[layer]*sw_flame_taper[j];
    /* Filled, near-clipped ribbons replace sparse point beams. */
    sw_ribbon(a,b,width,widths[layer]*sw_flame_taper[j+1],colors[layer],(int)(alpha[layer]*(1-.55f*v)));
   }
  }
  for(int side=-1;side<=1;side+=2)for(int j=2;j<10;j++){float u=j/12.f,v=(j+1)/12.f;Vec3 a=sw_flame(root,u,len,t,phase),b=sw_flame(root,v,len,t,phase);a=add(a,sw_rotate((Vec3){side*(4+sinf(t*17-u*12+phase)*3)*u,cosf(t*13-u*9)*u*3,0}));b=add(b,sw_rotate((Vec3){side*(4+sinf(t*17-v*12+phase)*3)*v,cosf(t*13-v*9)*v*3,0}));sw_ray(a,b,ER,(int)(120*(1-u)),0);}
  sw_cloud(root,5,ET,170);
 }
}
static void sw_ring(float r,float z,unsigned ink,float width,int alpha){
 for(int j=0;j<64;j++){float a=j*6.2831853f/64,b=(j+1)*6.2831853f/64;Vec3 p={cosf(a)*r,sinf(a)*r,z},q={cosf(b)*r,sinf(b)*r,z};sw_ray(p,q,ink,alpha,(int)width);}
}
static void sw_glint(int x,int y,float age,int size){if(age<0||age>.7f)return;int r=(int)(sinf(age/.7f*3.141593f)*size);if(r<1)return;for(int k=-r;k<=r;k++){sw_blend(x+k,y,ET,220,4);sw_blend(x,y+k,EC,220,4);}}
static void sw_numeral(float t){
 if(t<2.77f||t>=SW_IMPACT)return;char number[3];int target,scale;sw_number_layout(game.erl.result_place,number,&target,&scale);int digits=(int)strlen(number),width=digits*8*scale;
 float enter=1-powf(1-sw_clamp((t-2.77f)/.45f),3);int x=(int)sw_mix(500,target,enter),y=42;
 erl_text(x+6,y+7,EP,number,scale);erl_text(x+3,y+3,ET,number,scale);erl_text(x,y,ER,number,scale);
 float sweep=x-50+(t-2.9f)*250;for(int yy=0;yy<8*scale;yy++)for(int xx=0;xx<width;xx++){int digit=xx/(8*scale),gx=(xx/scale)%8;if(!(font8[number[digit]-32][yy/scale]&(128>>gx)))continue;float left=sweep+yy*.395f;if(x+xx>=left&&x+xx<left+19)pixel(x+xx,y+yy,EC);}
 float build=sw_ease((t-2.83f)/.36f),shine=sw_clamp((t-3.22f)/.48f);int rx=x+width-10;line(rx,200,rx+(int)(18*build),200-(int)(140*build),RGB(24,75,91));line(rx+1,200,rx+1+(int)(18*build),200-(int)(140*build),ET);
 if(shine<=build){int px=rx+18*shine,py=200-140*shine;line(px,py+5,px+1,py-5,EC);}
 sw_glint(x+103,y+4,t-3.24f,13);sw_glint(x+143,y+137,t-3.48f,10);
}
static void sw_shards(float t){
 float age=t-SW_IMPACT;if(age<0||age>2.3f)return;
 for(int i=0;i<sw_world.shards;i++){SwShard *s=&sw_world.shard[i];Vec3 centre=add(s->p,mul(s->v,age));centre.y-=age*age*20;float a=s->spin*age,c=cosf(a),sn=sinf(a);Vec3 v[3];for(int k=0;k<3;k++){Vec3 p=s->offset[k];v[k]=add(centre,(Vec3){p.x*c-p.y*sn,p.x*sn+p.y*c,p.z*cosf(a*.6f)});}sw_triangle(v[0],v[1],v[2],planet_rgb_mix(s->ink,EB,(int)(sw_clamp((age-1.35f)/.95f)*255)));}
 for(int i=0;i<7;i++){float f=(i+.5f)/7;Vec3 drift={age*(150+i*13),age*(i-3)*35,age*(24000+1050+i*120)};sw_ray(add(sw_unproject(440+18*(f-.055f),200-140*(f-.055f)),drift),add(sw_unproject(440+18*(f+.055f),200-140*(f+.055f)),drift),ET,(int)(255*sw_clamp(1-age/.65f)),0);}
}
static void sw_title(float wall){
 float age=wall-5.46f;if(age<0)return;float arrival=sw_ease(age/.65f);int y=(int)(-38+66*arrival);float amplitude=3.5f*expf(-age*1.35f)+1.1f,settle=expf(-age*1.6f);int place=game.erl.result_place;const char *suffix=place%100>=11&&place%100<=13?"TH":place%10==1?"ST":place%10==2?"ND":place%10==3?"RD":"TH";char title[24];snprintf(title,sizeof(title),"%d%s PLACE",place,suffix);const char *s=title;int n=strlen(s);
 for(int i=0;i<n;i++){char one[2]={s[i],0};int x=240-n*12+i*24+(int)roundf(sinf(age*5-i*.45f)*settle),dy=(int)roundf(sinf(age*7.5f-i*.62f)*amplitude+sinf(age*2.4f)*.5f);erl_text(x+1,y+dy+2,ER,one,3);erl_text(x,y+dy,EC,one,3);}
 if(arrival>.8f)for(int i=0;i<28;i++){int x=126+i*228/28,dy=(int)roundf(sinf(age*6-i*.4f)*fminf(1,amplitude*.3f));line(x,y+27+dy,x+228/28,y+27+dy,ET);}
 if(wall>6.01f){int n=strlen(game.commander_name);erl_fit(n*8<=440?240-n*4:20,67,440,ET,game.commander_name);}
}
static void sw_rewards(float wall){
 float p=sw_ease((wall-6.71f)/.65f);if(p<=0)return;int y=(int)(272-62*p);ErlCareer *e=&game.erl;erl_chamfer(4,y,472,62,EB,ET);rect(4,y+8,5,54,ER);line(174,y+8,174,y+39,EP);line(329,y+8,329,y+39,EP);
 char cash[32],xp[24],detail[32];snprintf(cash,sizeof(cash),"+%d.%d U",e->result_units/10,abs(e->result_units%10));snprintf(xp,sizeof(xp),"+%d",e->result_xp);
 const char *label="TAKEDOWNS";snprintf(detail,sizeof(detail),"%d",e->result_takedowns);
 if(e->event_kind==ERL_OFFICIAL){label=e->round==4?"CUP PLACE":"POINTS";snprintf(detail,sizeof(detail),"%d",e->round==4?e->season_place:e->result_points);
  if(e->round==4&&e->season_place==1){if(e->result_units<102000*(e->division+1)){label="PRIZE SHIP";snprintf(detail,sizeof(detail),"%s",player_ships[erl_prizes[e->division]].name);}else {label="CHAMPION";snprintf(detail,sizeof(detail),"CUP + EQUIPMENT");}}
 }
 else if(e->event_kind==ERL_QUALIFIER&&e->result_place==1){label="LICENCE";snprintf(detail,sizeof(detail),"%s",erl_division_name(e->licence-1));}
 else if(e->event_kind==ERL_INTRO&&e->result_place==1){label="REFERRAL";snprintf(detail,sizeof(detail),"ERL HQ");}
 erl_text(20,y+9,ER,"PRIZE",1);erl_fit(20,y+24,148,EC,cash);erl_text(190,y+9,ER,"RACE XP",1);erl_fit(190,y+24,132,ET,xp);erl_text(342,y+9,ER,label,1);erl_fit(342,y+24,128,EC,detail);
 if(wall>=SW_READY){int retry=e->event_kind!=ERL_OFFICIAL&&e->result_place!=1;if(e->event_kind==ERL_OFFICIAL||retry){erl_button(12,246,224,retry?"RETRY":"STANDINGS",!erl_choice,1);erl_button(244,246,224,"CONTINUE",erl_choice,1);}else erl_button(12,246,456,"CONTINUE",1,1);}
}
static void erl_shockwave_draw(float wall){
 sw_measure(-1);unsigned *display=fb;fb=sw_pixels;sw_init();sw_view=sw_pose(wall);sw_bank_c=cosf(sw_view.bank);sw_bank_s=sinf(sw_view.bank);float t=sw_view.time;SwHull h=sw_hull(game.ship);
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cx1=clipx1,cy0=clipy0,cy1=clipy1,depth=surface_depth_on,ym=surface_y_min,yM=surface_y_max,encoding=mega_depth_encoding,material=surface_material;float focal=proj_focal,light=surface_light;
 preview_clip(240,142,0,0,W,H);proj_focal=238;surface_depth_on=1;surface_y_min=0;surface_y_max=H-1;mega_depth_encoding=surface_material=0;surface_light=1;memset(surface_depth,255,sizeof(surface_depth));drawcount=0;
 for(unsigned *p=fb,*end=fb+STRIDE*H;p<end;p++)*p=EB;sw_measure(0);
 float sun=sw_ease((t-5.25f)/1.75f);int sx=(int)sw_mix(327,354,sun),sy=(int)sw_mix(176,112,sun);
 /* Background glow uses a 43-colour ramp; no per-pixel alpha/depth read. Stars
  * remain world anchored and are drawn over this distant atmospheric glow. */
 if(sun>0){unsigned ramp[43];int strength=(int)(sun*256);for(int a=0;a<43;a++)ramp[a]=planet_rgb_mix(EB,RGB(255,174,59),a);for(int yy=-70;yy<=70;yy++)for(int xx=-70;xx<=70;xx++){int alpha=(strength*sw_sun_halo[(yy+70)*141+xx+70])>>8;if(alpha)pixel(sx+xx,sy+yy,ramp[alpha]);}}
 sw_stars(wall);
 if(sun>0){for(int yy=-16;yy<=16;yy++)for(int xx=-16;xx<=16;xx++){int rr=xx*xx+yy*yy;if(rr<=256)pixel(sx+xx,sy+yy,rr<36?RGB(255,244,213):rr<144?RGB(255,232,173):RGB(255,202,112));}}
 sw_measure(1);if(t<2.95f){float age=t-1.35f;if(age>0){float z=-5155+24000*(t-1.1f),r=35+age*760;sw_ring(r,z,ET,1,(int)(255*sw_clamp(1-age/1.8f)));sw_ring(r*1.03f,z,ER,0,190);sw_ring(r*.88f,z-25,RGB(81,68,172),1,100);}}
 sw_numeral(t);sw_ship(&h,ship_paint[game.ship],sun);sw_shards(t);flush_meshes();sw_measure(2);
 sw_exhaust(&h,t,t<2.95f?.35f+1.3f*sw_ease(t/1.4f):sw_mix(1.8f,.8f,sw_ease(fmaxf(0,t-SW_IMPACT)/1.85f)));
 sw_measure(3);if(sun>.1f){float alpha=sw_ease((t-5.8f)/1.1f);for(int j=0;j<4;j++){float f=.45f+j*.43f;int x=sx+(240-sx)*f,y=sy+(136-sy)*f,r=4+j*3;for(int yy=-r;yy<=r;yy++)for(int xx=-r;xx<=r;xx++)if(xx*xx+yy*yy<r*r)sw_blend(x+xx,y+yy,j&1?ET:ER,(int)(alpha*20),60000);}for(int x=sx-32;x<=sx+32;x++)sw_blend(x,sy,EC,(int)(alpha*120),60000);sw_glint(316,119,t-6.15f,11);sw_glint(191,157,t-7.1f,7);}
 sw_title(wall);sw_rewards(wall);sw_measure(4);
 memcpy(display,fb,STRIDE*H*sizeof(unsigned));fb=display;sw_measure(5);if(sw_measure_enabled)sw_measure_frames++;
 preview_clip(ox,oy,cx0,cy0,cx1,cy1);proj_focal=focal;surface_depth_on=depth;surface_y_min=ym;surface_y_max=yM;mega_depth_encoding=encoding;surface_material=material;surface_light=light;
}
