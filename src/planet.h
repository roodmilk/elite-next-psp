/* Local surface: biome from orbit sprite family, tinted by body colour/accent. */
static int surface_nav_poi=-1;
#include "field-sprites.h"
#include "generated/lave-landscape-pixels.h"
#include "generated/biome-props-pixels.h"
static unsigned planet_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static unsigned mix_rgb(unsigned a,unsigned b,float t){
 int ar=a&255,ag=(a>>8)&255,ab=(a>>16)&255,br=b&255,bg=(b>>8)&255,bb=(b>>16)&255;
 if(t<0)t=0;
 if(t>1)t=1;
 return RGB((int)(ar+(br-ar)*t),(int)(ag+(bg-ag)*t),(int)(ab+(bb-ab)*t));
}
/* Match orbit sprite families: ocean, desert, ice, volcanic, forest. */
enum { BIOME_OCEAN=0, BIOME_DESERT, BIOME_ICE, BIOME_VOLCANIC, BIOME_FOREST, BIOME_CLOUD };
static int planet_lave_i(void){return game.system==7&&game.planet==1;}
static int planet_biome(const Body *b){
 if(b&&b->type==GAS)return BIOME_CLOUD;
 if(planet_lave_i())return BIOME_FOREST;
 if(!b||b->type==OCEAN)return BIOME_OCEAN;
 int art=planet_sprite_index(b->seed,b->type);
 if(art==2)return BIOME_DESERT;
 if(art==3)return BIOME_ICE;
 if(art==4)return BIOME_VOLCANIC;
 return BIOME_FOREST;
}
static void planet_cliprect(int x,int y,int w,int h,unsigned c){
 int top=view_top(),bot=view_bot()+1;
 if(x<0){w+=x;x=0;}
 if(y<top){h-=top-y;y=top;}
 if(x+w>W)w=W-x;
 if(y+h>bot)h=bot-y;
 if(w>0&&h>0){if(surface_depth_on&&surface_sprite_z>0){c=surface_shade(c);for(int yy=y;yy<y+h;yy++){float den=1-surface_sprite_tan*(proj_oy-yy)/240.f;if(den<=.01f)continue;unsigned depth=surface_encode_depth(den/surface_sprite_z);for(int xx=x;xx<x+w;xx++)surface_depth_pixel(xx,yy,depth,c);}}else rect(x,y,w,h,c);}
}
static void planet_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned col){
 queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),col);
 queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),col);
}
typedef struct {float z,x,w;int kind;unsigned h;} PlanetProp;
static PlanetProp lave_prop_cache[2600];static int lave_prop_count;static unsigned lave_prop_seed;static int lave_prop_ready;
static unsigned lave_tree_inks[4][32];
static unsigned biome_prop_inks[4][128];
static unsigned char biome_prop_spans[24][64][2];static int biome_prop_spans_ready;
static unsigned char lave_sprite_spans[8][64][2];static int lave_spans_ready;
static void lave_prepare_props(Vec3 pad,const Vec3 *sites,const FieldBuilding *shapes,unsigned seed){
 if(!lave_spans_ready){
  for(int kind=0;kind<8;kind++)for(int row=0;row<64;row++){
   int left=48,right=0;for(int x=0;x<48;x++)if(lave_vegetation_pixels[kind][row*48+x]!=255){if(x<left)left=x;right=x+1;}
   lave_sprite_spans[kind][row][0]=left;lave_sprite_spans[kind][row][1]=right;
  }lave_spans_ready=1;
 }
 if(lave_prop_ready&&lave_prop_seed==seed)return;
 lave_prop_ready=1;lave_prop_seed=seed;lave_prop_count=0;
 int cx=(int)floorf(pad.x/70),cz=(int)floorf(pad.z/70);
 for(int iz=-28;iz<=28;iz++)for(int ix=-28;ix<=28;ix++){
  float x,z;unsigned h;int tree=field_lave_tree_cell(seed,cx+ix,cz+iz,&x,&z,&h);
  if(terrain_is_water(&game,x,z)||(fabsf(x-pad.x)<field_port_clear(&game)&&fabsf(z-pad.z)<field_port_clear(&game)))continue;
  float t=(z-pad.z)/FIELD_OBSERVATORY_DISTANCE,side=seed&1?1.f:-1.f;
  if(t>0&&t<1&&fabsf(x-(pad.x-side*420*t*(1-t)))<38)continue;
  int reserved=0;for(int i=1;i<10;i++){if(i==6)continue;if(fabsf(x-sites[i].x)<shapes[i].w+30&&fabsf(z-sites[i].z)<shapes[i].d+30){reserved=1;break;}}if(reserved)continue;
  int kind=tree?(h>>16)%4:4+((h>>12)&7)/2;
  if(lave_prop_count<2600)lave_prop_cache[lave_prop_count++]=(PlanetProp){terrain_height(&game,x,z),x,z,kind,h};
 }
}
static int planet_prop_cmp(const void *a,const void *b){float d=((const PlanetProp*)b)->z-((const PlanetProp*)a)->z;return d>0?1:d<0?-1:0;}
static void draw_tree_billboard(float x,float z,unsigned h,int kind){
 float ground=terrain_height(&game,x,z);if(terrain_is_water(&game,x,z))return;
 Body *b=&game.bodies[game.planet];int biome=planet_biome(b);
 float height=kind==1?12.f:kind==2?8.f:biome==BIOME_FOREST?90.f+(h%65):38.f+(h%25);
 if(biome==BIOME_DESERT&&kind==0)height=14.f+(h%9);
 if(biome==BIOME_ICE&&kind==0)height=18.f+(h%11);
 Vec3 base={x,ground,z},top={x,ground+height,z};
 Vec3 vb=camera(&game,base),vt=camera(&game,top);
 if(vb.z<10||vb.z>780)return;
 Point pb=project(vb),pt=project(vt);
 surface_sprite_z=vb.z-surface_sprite_tan*vb.y;int s=(int)fmaxf(3,fminf(100,(kind==0?height*65:720.f)/vb.z));
 int x0=(int)pt.x,y0=(int)pt.y,y1=(int)pb.y;
 if(y1<=y0)y1=y0+s;
 unsigned leaf=mix_rgb(b->accent,b->color,.35f);
 unsigned dark=mix_rgb(leaf,RGB(12,40,16),.4f);
 unsigned trunk=mix_rgb(b->color,RGB(92,62,38),.55f);
 if(biome==BIOME_DESERT){leaf=mix_rgb(b->accent,RGB(150,130,70),.4f);trunk=mix_rgb(b->color,RGB(110,80,45),.5f);}
 if(biome==BIOME_ICE){leaf=mix_rgb(b->accent,RGB(180,210,230),.5f);dark=mix_rgb(leaf,RGB(80,120,150),.35f);trunk=mix_rgb(b->color,RGB(160,180,200),.45f);}
 if(biome==BIOME_VOLCANIC){leaf=mix_rgb(b->accent,RGB(180,60,40),.4f);dark=mix_rgb(leaf,RGB(40,20,20),.4f);trunk=mix_rgb(b->color,RGB(50,40,40),.5f);}
 if(biome==BIOME_FOREST)leaf=mix_rgb(b->accent,RGB(36,110,42),.4f);
 /* Ground contact cue: a compact, low-contrast shadow keeps small native
  * billboards attached to the terrain instead of reading as floating HUD art. */
 if(!high_contrast){
  unsigned shade=mix_rgb(b->color,RGB(18,22,24),.72f);
  int sw=kind==2?s+2:kind==1?s+1:(s*3)/4;
  if(sw<3)sw=3;
  planet_cliprect(x0-sw/2,y1-1,sw,2,shade);
 }
 if(kind==3){
  int step=((int)(game.time*3)+(int)h)&1;
  unsigned hide=mix_rgb(b->accent,RGB(184,145,91),.6f);
  planet_cliprect(x0-s,y1-s,s*2,s/2+1,hide);planet_cliprect(x0+s/2,y1-s-s/3,s,s/2,hide);
  planet_cliprect(x0-s+step,y1-s/2,2,s/2,trunk);planet_cliprect(x0+s/2-step,y1-s/2,2,s/2,trunk);return;
 }
 if(kind==2){
  unsigned rock=mix_rgb(b->color,RGB(110,96,78),.45f);
  if(biome==BIOME_ICE)rock=mix_rgb(b->accent,RGB(200,220,235),.4f);
  if(biome==BIOME_VOLCANIC)rock=mix_rgb(b->color,RGB(70,50,48),.5f);
  planet_cliprect(x0-s,y1-s,s*2,s,rock);
  planet_cliprect(x0-s/2,y1-s/2,s,s/2,mix_rgb(rock,RGB(150,130,100),.35f));
  return;
 }
 if(kind==1){
  planet_cliprect(x0-s,y1-s,s*2,s,leaf);
  planet_cliprect(x0-s/2,y1-s-s/3,s,s/2,dark);
  return;
 }
 planet_cliprect(x0-s/5,y0+s/3,s/2,y1-(y0+s/3),trunk);
 planet_cliprect(x0-s,y0,s*2,s,leaf);
 planet_cliprect(x0-s*2/3,y0-s/2,s+s/2,s,dark);
 planet_cliprect(x0-s/2,y0-s/3,s,s/2,mix_rgb(leaf,RGB(200,220,90),.2f));
}
static void draw_lave_vegetation(float x,float z,unsigned h,int kind){
 float ground=terrain_height(&game,x,z);
 float height=kind<4?58.f+((h>>9)%31):kind==4?18.f:kind==5?17.f:kind==6?22.f:18.f;
 int other=game.system==7&&game.planet>1,row=game.planet==2?4:game.planet==4?1:5;
 if(other&&!biome_prop_spans_ready){for(int k=0;k<24;k++)for(int y=0;y<64;y++){int lo=48,hi=0;for(int x=0;x<48;x++)if(biome_prop_pixels[k][y*48+x]!=255){if(x<lo)lo=x;hi=x+1;}biome_prop_spans[k][y][0]=lo;biome_prop_spans[k][y][1]=hi;}biome_prop_spans_ready=1;}
 const unsigned char *pixels=other?biome_prop_pixels[row*4+kind]:lave_vegetation_pixels[kind];
 if(other)height=kind==0?(game.planet==3?42:88)+(h%28):kind==1?22:kind==2?30:13;
 if(other&&game.planet==4&&kind>0)height=kind==1?9:kind==2?22:5;
 float width=height*.75f;
 Vec3 base=camera(&game,(Vec3){x,ground,z}),topv=camera(&game,(Vec3){x,ground+height,z});
 float plane=base.z-surface_sprite_tan*base.y;if(plane<2)return;
 int first=surface_y_min,last=surface_y_max;
 if(topv.z>4)first=(int)fmaxf(first,floorf(project(topv).y));
 if(base.z>4)last=(int)fminf(last,ceilf(project(base).y));
 int fog=(int)fmaxf(0,fminf(3,(plane-200)/300));unsigned *palette=other?biome_prop_inks[fog]:lave_tree_inks[fog];
 first=(first+1)&~1;
 for(int y=first;y<=last;y+=2){
  float ry=(proj_oy-y-.5f)/240.f,inv=(1-surface_sprite_tan*ry)/plane;if(inv<=0)continue;
  float wy=game.pos.y-ground+(ry*surface_cp+surface_sp)/inv;
  int sy=(int)((1-wy/height)*64);if(sy<0||sy>=64)continue;
  float left=proj_ox+(base.x-width*.5f)*inv*240,sw=width*inv*240;
  int lo=other?biome_prop_spans[row*4+kind][sy][0]:lave_sprite_spans[kind][sy][0],hi=other?biome_prop_spans[row*4+kind][sy][1]:lave_sprite_spans[kind][sy][1];if(lo>=hi||sw<.5f)continue;
  int x0=(int)fmaxf(0,ceilf(left+sw*lo/48)),x1=(int)fminf(W-1,left+sw*hi/48);
  unsigned depth=surface_encode_depth(inv);
  x0=(x0+1)&~1;
  float step=48/sw,u=(x0+1.f-left)*step;
  for(int px=x0;px<=x1;px+=2,u+=step*2){
   int at=y*W+px,out=y*STRIDE+px;
   if(depth>surface_depth[at]&&depth>surface_depth[at+1]&&(y+1>surface_y_max||(depth>surface_depth[at+W]&&depth>surface_depth[at+W+1])))continue;
   int sx=(int)u;if(sx<0||sx>=48)continue;int index=pixels[sy*48+sx];
   if(index!=255){unsigned c=palette[index];
    if(depth<=surface_depth[at]){surface_depth[at]=depth;fb[out]=c;}if(depth<=surface_depth[at+1]){surface_depth[at+1]=depth;fb[out+1]=c;}
    if(y+1<=surface_y_max){at+=W;out+=STRIDE;if(depth<=surface_depth[at]){surface_depth[at]=depth;fb[out]=c;}if(depth<=surface_depth[at+1]){surface_depth[at+1]=depth;fb[out+1]=c;}}
   }
  }
 }
}
static float planet_ridge_noise(float bearing,unsigned seed,float frequency){
 int period=(int)(frequency*6.2831853f);float u=bearing*period/6.2831853f;int i=(int)floorf(u);float f=u-i;f=f*f*(3-2*f);i=(i%period+period)%period;
 float a=(planet_hash(seed^(unsigned)i*0x9e3779b9u)&65535)/65535.f;
 float b=(planet_hash(seed^(unsigned)((i+1)%period)*0x9e3779b9u)&65535)/65535.f;
 return a+(b-a)*f;
}
#include "field-world.h"
#include "field-leaves.h"
static void draw_life_billboard(const Lifeform *l){
 field_draw_world(l);
}
static void surface_box(Vec3 p,float w,float d,float height,unsigned ink){
 /* Lave's dense authored port/terrain kit is the profiled safe case for box
  * rejection. Procedural worlds can place small landmark parts on arbitrary
  * edge slopes, where preserving them is preferable to a few saved faces. */
 if(surface_fast&&game.system==7&&!surface_cull_suspended){
  Vec3 centre=camera(&game,(Vec3){p.x,p.y+height*.5f,p.z});
  /* A conservative enclosing sphere: only discard boxes wholly outside
   * a screen plane. No distance cutoff or reduced detail. */
  float radius=sqrtf(w*w+d*d+height*height*.25f),guard=radius*1.50f+12;
  if(centre.z+guard<4||
     centre.x-centre.z*1.01f>guard*1.422f||-centre.x-centre.z*1.01f>guard*1.422f||
     centre.y-surface_top_slope*centre.z>guard*surface_top_norm||
     -centre.y-surface_bottom_slope*centre.z>guard*surface_bottom_norm)return;
 }
 float y=p.y;Vec3 a={p.x-w,y,p.z-d},b={p.x+w,y,p.z-d},c={p.x+w,y,p.z+d},e={p.x-w,y,p.z+d};
 Vec3 A=add(a,(Vec3){0,height,0}),B=add(b,(Vec3){0,height,0}),C=add(c,(Vec3){0,height,0}),E=add(e,(Vec3){0,height,0});
 if(game.pos.z<p.z-d)planet_quad(a,b,B,A,ink);
 if(game.pos.x>p.x+w)planet_quad(b,c,C,B,mix_rgb(ink,RGB(10,20,30),.3f));
 if(game.pos.z>p.z+d)planet_quad(c,e,E,C,ink);
 if(game.pos.x<p.x-w)planet_quad(e,a,A,E,mix_rgb(ink,RGB(10,20,30),.2f));
 if(game.pos.y>y+height)planet_quad(A,B,C,E,mix_rgb(ink,WHITE,.25f));
 if(game.pos.y<y)planet_quad(a,e,c,b,mix_rgb(ink,RGB(10,20,30),.4f));
}
static void surface_bridges(void){
 if(game.system==7&&game.planet==3)return;
 for(int i=0;i<6;i++){
  Vec3 p,across;if(!terrain_bridge_position(&game,i,&p,&across))continue;
  Vec3 view=camera(&game,p);if(view.z< -80||view.z>2350||fabsf(view.x)>view.z*1.5f+220)continue;
  Vec3 along={-across.z,0,across.x},aa=mul(across,178),ww=mul(along,15);float y=p.y+.8f;
  Vec3 a=add(add(p,aa),ww),b=add(sub(p,aa),ww),c=sub(sub(p,aa),ww),d=add(sub(p,ww),aa);a.y=b.y=c.y=d.y=y;
  planet_quad(a,b,c,d,RGB(88,104,106));
  for(int rail=-1;rail<=1;rail+=2){
   Vec3 side=mul(ww,(float)rail),r0=add(sub(p,aa),side),r1=add(add(p,aa),side),r2=r1,r3=r0;
   r0.y=r1.y=y+3;r2.y=r3.y=y+13;planet_quad(r0,r1,r2,r3,RGB(168,151,104));
  }
  for(int mark=-3;mark<=3;mark++){Vec3 q=add(p,mul(across,mark*46.f)),m0=sub(q,mul(along,2)),m1=add(q,mul(along,2));m0.y=m1.y=y+.2f;Vec3 m2=add(m1,mul(across,3)),m3=add(m0,mul(across,3));planet_quad(m0,m1,m2,m3,RGB(211,185,102));}
 }
 flush_meshes();
}
/* Ruins are deliberately not a service building with a different label.
 * Their broken silhouette remains legible from a distance: no roof, no glazing,
 * a collapsed front and scattered masonry around the surviving stonework. */
static void surface_ruins(Vec3 p,FieldBuilding shape,unsigned ink,unsigned seed){
 unsigned stone=mix_rgb(ink,RGB(105,91,72),.52f);
 unsigned worn=mix_rgb(stone,RGB(34,31,30),.38f);
 unsigned pale=mix_rgb(stone,RGB(174,156,119),.22f);
 float w=shape.w,d=shape.d,h=shape.height;
 /* Two unequal wall fragments leave the centre visibly open. */
 surface_box((Vec3){p.x-w*.54f,p.y,p.z-d*.12f},w*.30f,d*.30f,h*(.62f+(seed&7)*.035f),stone);
 surface_box((Vec3){p.x+w*.52f,p.y,p.z+d*.10f},w*.27f,d*.32f,h*(.31f+((seed>>3)&7)*.025f),worn);
 /* A low rear course and offset pillars imply an old enclosure, not a house. */
 surface_box((Vec3){p.x,p.y,p.z+d*.68f},w*.82f,d*.15f,h*.22f,worn);
 surface_box((Vec3){p.x-w*.77f,p.y,p.z-d*.70f},w*.11f,d*.12f,h*.82f,pale);
 surface_box((Vec3){p.x+w*.70f,p.y,p.z-d*.65f},w*.10f,d*.12f,h*.48f,stone);
 for(int i=0;i<5;i++){
  unsigned r=field_hash(seed+i*977u);float x=p.x+((int)(r%200)-100)*w*.009f;
  float z=p.z+((int)((r>>8)%190)-95)*d*.010f;
  surface_box((Vec3){x,p.y,z},w*(.10f+(r&3)*.025f),d*(.08f+((r>>3)&3)*.025f),3+(r>>7)%7,mix_rgb(worn,pale,(r>>12&3)*.08f));
 }
}
/* Seed gardens are climate domes: a low service ring, flower beds visible
 * through segmented glass, and a broad hemisphere rather than a boxy house. */
static void surface_seed_garden(Vec3 p,FieldBuilding shape,unsigned ink,unsigned seed){
 unsigned frame=mix_rgb(ink,RGB(62,103,94),.42f);
 unsigned glass=mix_rgb(ink,RGB(114,222,178),.52f);
 unsigned glow=mix_rgb(glass,RGB(224,255,194),.34f);
 unsigned soil=mix_rgb(ink,RGB(73,54,33),.66f);
 float r=fmaxf(shape.w,shape.d)*1.12f,base_h=7.f,dome_h=shape.height*.72f;
 surface_box(p,r,r,base_h,frame);
 /* Beds sit in the open lower glass band, making the place read as a garden
  * even at a distance on the PSP screen. */
 for(int bed=0;bed<3;bed++){
  float x=p.x+(bed-1)*r*.48f,z=p.z-r*.18f;
  surface_box((Vec3){x,p.y+base_h,z},r*.15f,r*.34f,2,soil);
  surface_box((Vec3){x,p.y+base_h+2,z},r*.09f,r*.20f,4+(seed>>(bed*3)&3),mix_rgb(glow,RGB(238,115,157),bed*.16f));
 }
 /* Stepped octagonal shell approximates a dome without a costly mesh. */
 for(int ring=0;ring<5;ring++){
  float t=ring*.20f,rr=r*(1.f-t*.78f),hh=base_h+dome_h*(.22f+ring*.19f);
  surface_box((Vec3){p.x,p.y+hh,p.z},rr,rr,2,ring==4?glow:glass);
 }
 for(int rib=0;rib<4;rib++){
  float a=rib*.785398f+(seed&3)*.08f;
  Vec3 q={p.x+cosf(a)*r*.62f,p.y+base_h,p.z+sinf(a)*r*.62f};
  surface_box(q,2,2,dome_h*.78f,frame);
 }
 Vec3 door={p.x,p.y,p.z-r-.3f};surface_box(door,r*.20f,2,base_h+8,RGB(29,48,49));
}
static void surface_supply_cache(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned deck=mix_rgb(ink,RGB(79,91,82),.48f),crate=mix_rgb(ink,RGB(151,103,50),.42f),lamp=RGB(232,177,70);
 surface_box(p,s.w,s.d,3,deck);
 for(int i=0;i<6;i++){float x=p.x-s.w*.58f+(i%3)*s.w*.58f,z=p.z-s.d*.34f+(i/3)*s.d*.62f;surface_box((Vec3){x,p.y+3,z},s.w*.17f,s.d*.19f,12+(seed>>(i*3)&7),crate);}
 for(int side=-1;side<=1;side+=2)surface_box((Vec3){p.x+side*s.w*.80f,p.y+3,p.z},3,3,s.height*.72f,deck);
 surface_box((Vec3){p.x,p.y+s.height*.72f,p.z},s.w*.92f,s.d*.74f,3,deck);
 surface_box((Vec3){p.x,p.y+3,p.z-s.d*.72f},5,5,s.height*.92f,deck);
 surface_box((Vec3){p.x,p.y+s.height*.88f,p.z-s.d*.72f},13,3,4,lamp);
}
static void surface_observatory(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 if(planet_lave_i()){
  surface_box((Vec3){p.x,p.y-8,p.z},s.w,s.d,16,RGB(108,125,125));
  surface_box((Vec3){p.x,p.y+8,p.z},s.w*.88f,s.d*.86f,30,RGB(183,187,159));
  surface_box((Vec3){p.x,p.y+24,p.z},s.w*.90f,s.d*.88f,8,RGB(62,112,138));
  surface_box((Vec3){p.x,p.y+36,p.z},s.w*.94f,s.d*.9f,5,RGB(218,211,172));
  float radius=72,base=p.y+41;
  int distant=camera(&game,p).z>500,rows=distant?3:5,segments=distant?8:16;
  for(int row=0;row<rows;row++)for(int seg=0;seg<segments;seg++){
   float a=seg*6.2831853f/segments,b=(seg+1)*6.2831853f/segments,u=row*1.5707963f/rows,v=(row+1)*1.5707963f/rows;
   float r0=radius*cosf(u),r1=radius*cosf(v),y0=base+radius*sinf(u),y1=base+radius*sinf(v);
   unsigned c=mix_rgb(RGB(84,126,153),RGB(227,218,168),.5f+.5f*cosf(a+.8f));
   if(seg==segments/4)c=RGB(43,83,106);
   planet_quad((Vec3){p.x+r0*cosf(a),y0,p.z+r0*sinf(a)},(Vec3){p.x+r0*cosf(b),y0,p.z+r0*sinf(b)},(Vec3){p.x+r1*cosf(b),y1,p.z+r1*sinf(b)},(Vec3){p.x+r1*cosf(a),y1,p.z+r1*sinf(a)},c);
  }
  surface_box((Vec3){p.x+s.w*.85f,p.y+40,p.z},3,3,124,RGB(210,208,165));
  surface_box((Vec3){p.x+s.w*.85f,p.y+152,p.z},5,5,5,RGB(96,187,207));
  return;
 }
 unsigned tower=mix_rgb(ink,RGB(77,96,112),.48f),glass=mix_rgb(ink,RGB(103,199,218),.52f),rim=RGB(75,136,153);
 float h=s.height,r=s.w;
 surface_box(p,r*.42f,r*.42f,8,tower);
 surface_box((Vec3){p.x,p.y+8,p.z},r*.13f,r*.13f,h*.70f,tower);
 for(int level=0;level<3;level++)surface_box((Vec3){p.x,p.y+h*(.28f+level*.19f),p.z},r*.25f,r*.25f,3,rim);
 surface_box((Vec3){p.x,p.y+h*.68f,p.z},r*.72f,r*.72f,5,tower);
 for(int ring=0;ring<6;ring++){
  float rr=r*(.72f-ring*.105f),y=p.y+h*(.71f+ring*.045f);
  surface_box((Vec3){p.x,y,p.z},rr,rr,3,ring==5?RGB(209,232,220):glass);
 }
 for(int a=0;a<4;a++){float angle=a*.785398f+(seed&3)*.04f;surface_box((Vec3){p.x+cosf(angle)*r*.55f,p.y+h*.70f,p.z+sinf(angle)*r*.55f},2,2,h*.22f,rim);}
 surface_box((Vec3){p.x,p.y+h*.98f,p.z},3,3,h*.15f,RGB(221,184,83));
}
static void surface_rescue_site(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned pad=mix_rgb(ink,RGB(126,130,113),.5f),tent=mix_rgb(ink,RGB(211,115,62),.34f),mast=RGB(81,124,138);
 surface_box(p,s.w,s.d,2,pad);
 surface_box((Vec3){p.x-s.w*.34f,p.y+2,p.z+s.d*.12f},s.w*.30f,s.d*.42f,s.height*.30f,tent);
 surface_box((Vec3){p.x-s.w*.34f,p.y+s.height*.32f,p.z+s.d*.12f},s.w*.34f,s.d*.46f,3,RGB(226,158,82));
 surface_box((Vec3){p.x+s.w*.46f,p.y+2,p.z},4,4,s.height*.85f,mast);
 for(int arm=0;arm<3;arm++)surface_box((Vec3){p.x+s.w*.46f,p.y+s.height*(.45f+arm*.18f),p.z},15-arm*3,3,3,arm==2?RGB(242,104,78):mast);
 for(int c=0;c<4;c++){float x=p.x+(c&1?1:-1)*s.w*.78f,z=p.z+(c&2?1:-1)*s.d*.75f;surface_box((Vec3){x,p.y+2,z},3,3,14,RGB(233,177+(seed&15),61));}
}
static void surface_fossil_bed(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned edge=mix_rgb(ink,RGB(109,81,53),.55f),bone=mix_rgb(ink,RGB(225,207,162),.32f),soil=RGB(74,53,38);
 surface_box(p,s.w,s.d,2,soil);
 surface_box((Vec3){p.x-s.w*.88f,p.y+2,p.z},s.w*.12f,s.d,8,edge);surface_box((Vec3){p.x+s.w*.88f,p.y+2,p.z},s.w*.12f,s.d,8,edge);
 surface_box((Vec3){p.x,p.y+2,p.z-s.d*.84f},s.w*.76f,s.d*.16f,8,edge);surface_box((Vec3){p.x,p.y+2,p.z+s.d*.84f},s.w*.76f,s.d*.16f,8,edge);
 surface_box((Vec3){p.x,p.y+4,p.z},s.w*.58f,3,4,bone);
 for(int rib=0;rib<7;rib++){float x=p.x-s.w*.48f+rib*s.w*.16f;surface_box((Vec3){x,p.y+4,p.z+(rib&1?4:-4)},3,s.d*(.18f+((seed>>rib)&3)*.035f),3,bone);}
 surface_box((Vec3){p.x+s.w*.57f,p.y+4,p.z},10,8,6,bone);
}
static void surface_thermal_vent(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned metal=mix_rgb(ink,RGB(89,103,102),.53f),hot=RGB(219,104,49),pipe=RGB(68,83,86);
 surface_box(p,s.w,s.d,5,metal);
 for(int stack=0;stack<5;stack++){
  float a=stack*1.256637f+(seed&7)*.08f,rad=stack? s.w*.48f:0;
  float x=p.x+cosf(a)*rad,z=p.z+sinf(a)*rad,h=s.height*(stack? .40f+(stack%3)*.13f:.88f);
  surface_box((Vec3){x,p.y+5,z},7+(stack&1)*3,7+(stack&1)*3,h,stack?pipe:metal);
  surface_box((Vec3){x,p.y+5+h,z},11+(stack&1)*3,11+(stack&1)*3,4,hot);
 }
 surface_box((Vec3){p.x,p.y+12,p.z},s.w*.70f,4,5,pipe);surface_box((Vec3){p.x,p.y+12,p.z},4,s.d*.70f,5,pipe);
}
static void surface_drone_wreck(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned hull=mix_rgb(ink,RGB(82,105,116),.42f),scar=mix_rgb(hull,RGB(41,32,31),.5f),wing=RGB(97,127,136);
 surface_box((Vec3){p.x-s.w*.12f,p.y,p.z},s.w*.48f,s.d*.15f,s.height*.42f,hull);
 surface_box((Vec3){p.x+s.w*.43f,p.y,p.z+s.d*.08f},s.w*.22f,s.d*.13f,s.height*.26f,scar);
 surface_box((Vec3){p.x-s.w*.10f,p.y+s.height*.18f,p.z},s.w*.34f,s.d*.82f,4,wing);
 surface_box((Vec3){p.x+s.w*.64f,p.y+3,p.z-s.d*.53f},s.w*.19f,s.d*.24f,8,scar);
 surface_box((Vec3){p.x-s.w*.42f,p.y+s.height*.38f,p.z},4,4,s.height*.48f,hull);
 surface_box((Vec3){p.x-s.w*.42f,p.y+s.height*.77f,p.z},15,3,3,RGB(221,153+(seed&15),68));
}
static void surface_migration_post(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned leg=mix_rgb(ink,RGB(91,105,91),.5f),deck=RGB(112,139,128),glass=RGB(87,168,179);
 for(int x=-1;x<=1;x+=2)for(int z=-1;z<=1;z+=2)surface_box((Vec3){p.x+x*s.w*.52f,p.y,p.z+z*s.d*.52f},4,4,s.height*.66f,leg);
 surface_box((Vec3){p.x,p.y+s.height*.62f,p.z},s.w*.68f,s.d*.68f,6,deck);
 surface_box((Vec3){p.x,p.y+s.height*.68f,p.z},s.w*.42f,s.d*.42f,s.height*.20f,glass);
 surface_box((Vec3){p.x,p.y+s.height*.88f,p.z},5,5,s.height*.20f,leg);
 for(int arm=0;arm<2;arm++)surface_box((Vec3){p.x,p.y+s.height*(.95f+arm*.06f),p.z},18-arm*6,3,3,arm?RGB(226,191,83):leg);
 (void)seed;
}
static void surface_crystal_grotto(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned base=mix_rgb(ink,RGB(61,54,72),.62f);surface_box(p,s.w,s.d,4,base);
 for(int shard=0;shard<9;shard++){
  unsigned h=field_hash(seed+shard*313u);float a=shard*.698132f,rad=shard? s.w*(.26f+(h&31)*.012f):0;
  float x=p.x+cosf(a)*rad,z=p.z+sinf(a)*rad,hh=s.height*(.32f+((h>>7)&63)*.009f),r=5+(h&7);
  unsigned crystal=mix_rgb(ink,RGB(129+(h&63),91,215),.58f);
  surface_box((Vec3){x,p.y+4,z},r,r,hh,crystal);surface_box((Vec3){x,p.y+4+hh,z},r*.55f,r*.55f,8,RGB(190,154+(h&63),245));
 }
}
static void surface_archive_vault(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned stone=mix_rgb(ink,RGB(84,92,89),.55f),door=RGB(38,54,57),light=RGB(214,174,81);
 surface_box(p,s.w,s.d,s.height*.44f,stone);
 for(int step=0;step<4;step++)surface_box((Vec3){p.x,p.y+s.height*(.43f+step*.07f),p.z+s.d*.10f},s.w*(.82f-step*.12f),s.d*(.82f-step*.11f),4,mix_rgb(stone,ink,step*.08f));
 surface_box((Vec3){p.x,p.y,p.z-s.d-.5f},s.w*.24f,3,s.height*.38f,door);
 for(int side=-1;side<=1;side+=2)surface_box((Vec3){p.x+side*s.w*.54f,p.y+s.height*.18f,p.z-s.d-.8f},8,3,4,light);
 surface_box((Vec3){p.x+s.w*.62f,p.y+s.height*.52f,p.z},4,4,s.height*.52f,stone);
 surface_box((Vec3){p.x+s.w*.62f,p.y+s.height*.94f,p.z},16,3,3,RGB(92,183+(seed&31),193));
}
static void surface_weather_array(Vec3 p,FieldBuilding s,unsigned ink,unsigned seed){
 unsigned steel=mix_rgb(ink,RGB(73,95,107),.55f),live=RGB(83,189,211),warn=RGB(224,154,61);
 for(int tower=-1;tower<=1;tower++){
  float x=p.x+tower*s.w*.58f,h=s.height*(tower? .72f:.94f);
  for(int leg=-1;leg<=1;leg+=2)surface_box((Vec3){x+leg*8,p.y,p.z},3,3,h,steel);
  for(int level=0;level<4;level++){
   float y=p.y+h*(.24f+level*.21f),arm=16+level*4;
   surface_box((Vec3){x,y,p.z},arm,3,3,level==3?live:steel);
  }
  surface_box((Vec3){x,p.y+h,p.z},3,3,12,tower?warn:live);
 }
 surface_box((Vec3){p.x,p.y+5,p.z+s.d*.55f},s.w*.72f,5,7,steel);
 for(int box=-1;box<=1;box+=2)surface_box((Vec3){p.x+box*s.w*.30f,p.y+5,p.z+s.d*.55f},12,10,20,warn);
 (void)seed;
}
#include "surface-landmarks.h"
static void surface_field_structure(Vec3 p,FieldBuilding shape,int kind,unsigned ink,unsigned seed){
 /* Landmark silhouettes often sit exactly on the viewport edge. Their small
  * part boxes are cheap, so preserve them rather than applying broad box
  * rejection intended for repeated scenery/airport structures. */
 surface_cull_suspended++;
 if(surface_landmark_v2(p,shape,kind,ink,seed)){surface_cull_suspended--;return;}
 switch(kind){
  case FIELD_CACHE:surface_supply_cache(p,shape,ink,seed);break;
  case FIELD_RUINS:surface_ruins(p,shape,ink,seed);break;
  case FIELD_DISH:surface_observatory(p,shape,ink,seed);break;
  case FIELD_RESCUE:surface_rescue_site(p,shape,ink,seed);break;
  case FIELD_GARDEN:surface_seed_garden(p,shape,ink,seed);break;
  case FIELD_FOSSIL:surface_fossil_bed(p,shape,ink,seed);break;
  case FIELD_VENT:surface_thermal_vent(p,shape,ink,seed);break;
  case FIELD_WRECK:surface_drone_wreck(p,shape,ink,seed);break;
  case FIELD_MIGRATION:surface_migration_post(p,shape,ink,seed);break;
  case FIELD_CRYSTAL:surface_crystal_grotto(p,shape,ink,seed);break;
  case FIELD_ARCHIVE:surface_archive_vault(p,shape,ink,seed);break;
  default:surface_weather_array(p,shape,ink,seed);break;
 }
 surface_cull_suspended--;
}
/* Glyph strips lie on the actual wall plane, sharing terrain/hull depth. */
static void surface_building_sign(Vec3 p,Vec3 across,float width,const char *name){
 Vec3 v=camera(&game,p);int count=(int)strlen(name);if(v.z<4||v.z>400||count<1||count>31)return;
 if(fabsf(v.x)>v.z+width*.6f||fabsf(v.y)>v.z*.65f+width*.2f)return;
 float unit=width/(count*8);if(unit*240/v.z<.45f)return;
 Vec3 left=sub(p,mul(across,width*.5f));left.y+=4*unit;
 flush_meshes();
 for(int i=0;i<count;i++){unsigned char c=name[i];if(c<32||c>126)c='?';
  for(int gy=0;gy<8;gy++)for(int gx=0;gx<8;){
   if(!((font8[c-32][gy]>>(7-gx))&1)){gx++;continue;}
   int start=gx;while(gx<8&&((font8[c-32][gy]>>(7-gx))&1))gx++;
   Vec3 a=add(left,mul(across,(i*8+start)*unit)),b=add(left,mul(across,(i*8+gx)*unit));a.y-=gy*unit;b.y=a.y;
   Vec3 c=a,d=b;c.y-=unit;d.y-=unit;planet_quad(a,b,d,c,RGB(245,212,132));
  }
 }flush_meshes();
}
static Vec3 surface_sky_ship(int lane){
 Vec3 pad=surface_site(&game,1);float radius=750+lane*530,phase=game.world_clock*(.009f+lane*.002f)+lane*1.7f+(game.bodies[game.planet].seed%100)*.1f;
 return add(pad,(Vec3){sinf(phase)*radius,240+lane*145,cosf(phase)*radius});
}
#include "planet-ambience.h"
#include "cloud-render.h"
#include "starport-render.h"
static void planet_view(void){
 field_leaf_count=0;
 sceRtcGetCurrentTick(&lave_render_marks[0]);
 surface_sprite_z=0;surface_depth_on=0;surface_sprite_tan=tanf(game.pitch);
 surface_material=0;surface_trail_enabled=planet_lave_i();surface_cy=cosf(game.yaw);surface_sy=sinf(game.yaw);surface_cp=cosf(game.pitch);surface_sp=sinf(game.pitch);
 float hour=field_local_hour(&game,game.system,game.planet),sunangle=(hour-6)*.2617994f;
 float daylight=fmaxf(0,fminf(1,sinf(sunangle)*2+.15f));surface_light=.23f+.77f*daylight;
 Body *b=&game.bodies[game.planet];int biome=planet_biome(b);FieldProfile ambience_field=field_profile(&game,game.system,game.planet);SurfaceAmbience ambience=surface_ambience_profile(b,biome,ambience_field);
 audio_surface_ambient=(biome<<2)|(ambience_field.weather&3);
 int top=view_top(),bottom=view_bot()+1;surface_y_min=top;surface_y_max=bottom-1;
 surface_top_slope=(proj_oy-top+2)/240.f;surface_bottom_slope=(bottom+2-proj_oy)/240.f;
 surface_top_norm=sqrtf(1+surface_top_slope*surface_top_slope);surface_bottom_norm=sqrtf(1+surface_bottom_slope*surface_bottom_slope);
 /* Broad, sunlit book-cover colour blocks,
  * kept biome-aware. Backdrop stays deterministic and cheap. */
 unsigned sky_hi=mix_rgb(b->color,RGB(52,144,209),.45f),sky_lo=mix_rgb(b->accent,RGB(255,174,122),.5f);
 if(biome==BIOME_DESERT){sky_hi=mix_rgb(b->color,RGB(224,119,63),.55f);sky_lo=mix_rgb(b->accent,RGB(247,177,83),.5f);}
 if(biome==BIOME_ICE){sky_hi=mix_rgb(b->color,RGB(140,180,230),.55f);sky_lo=mix_rgb(b->accent,RGB(220,200,180),.45f);}
 if(biome==BIOME_VOLCANIC){sky_hi=mix_rgb(b->color,RGB(110,45,35),.5f);sky_lo=mix_rgb(b->accent,RGB(200,90,45),.45f);}
 if(biome==BIOME_FOREST){sky_hi=mix_rgb(b->color,RGB(70,130,180),.45f);sky_lo=mix_rgb(b->accent,RGB(200,170,110),.4f);}
 if(biome==BIOME_OCEAN){sky_hi=mix_rgb(b->color,RGB(52,144,209),.55f);sky_lo=mix_rgb(b->accent,RGB(255,174,122),.55f);}
 if(planet_lave_i()){sky_hi=RGB(49,120,190);sky_lo=RGB(137,181,171);}
 if(game.system==7&&game.planet==2){sky_hi=RGB(54,110,155);sky_lo=RGB(166,178,144);}
 if(game.system==7&&game.planet==3){sky_hi=RGB(65,77,131);sky_lo=RGB(204,161,162);}
 if(game.system==7&&game.planet==4){sky_hi=RGB(63,110,159);sky_lo=RGB(187,208,223);}
 float sunset=fmaxf(0,1-fabsf(sinf(sunangle))*4);sky_lo=mix_rgb(sky_lo,RGB(238,92,47),sunset*.7f);
 sky_hi=mix_rgb(RGB(4,7,20),sky_hi,daylight);sky_lo=mix_rgb(RGB(16,18,34),sky_lo,fmaxf(daylight,sunset*.35f));
 for(int y=top;y<bottom;y++){
  float t=(y-top)/(float)fmaxf(1,bottom-top);
  unsigned c=mix_rgb(sky_hi,sky_lo,t);
  for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;
 }
 Vec3 sunv=camera(&game,add(game.pos,(Vec3){cosf(sunangle)*9000,sinf(sunangle)*9000,2800}));Point sunp=project((Vec3){sunv.x,sunv.y,fmaxf(1,sunv.z)});int sunx=(int)sunp.x,suny=(int)sunp.y;
 if(daylight<.4f)for(int i=0;i<90;i++){unsigned h=planet_hash(i+game.bodies[game.planet].seed);float az=(h%6283)*.001f,el=.12f+((h>>12)%1400)*.001f;Vec3 sv=camera(&game,add(game.pos,(Vec3){sinf(az)*cosf(el)*10000,sinf(el)*10000,cosf(az)*cosf(el)*10000}));if(sv.z>100){Point sp=project(sv);planet_cliprect((int)sp.x,(int)sp.y,1+(i%9==0),1,RGB(160,177,200));}}
 /* Sky disc uses the same animated system sun as orbit / charts — slightly larger warm read. */
 if(sunv.z>100&&sinf(sunangle)>-.04f)draw_sun_sprite(sunx,suny,17,game.bodies[0].color,game.bodies[0].seed,game.time,0,top,W,bottom);
 /* Soft-FB atmosphere beauty: sun streak + biome specular / embers. */
 if(!high_contrast){

 }
 /* Clouds use distant world bearings, rotating and pitching with the camera. */
 for(int i=0;i<10;i++){
  float a=i*.8976f+(b->seed%628)*.01f+game.world_clock*(.001f+(i%3)*.0002f);
  Vec3 cloud=add(game.pos,(Vec3){sinf(a)*5000,1100+(i%3)*260,cosf(a)*5000});
  Vec3 cv=camera(&game,cloud);if(cv.z<1200)continue;Point cp=project(cv);
  unsigned ink=mix_rgb(sky_lo,WHITE,biome==BIOME_VOLCANIC?.12f:.55f);
  int cw=38+(i%3)*17;for(int yy=-10;yy<7;yy++)for(int xx=-cw;xx<cw;xx++){if(yy<0&&abs(xx)>cw-12-abs(yy)*2)continue;int sx=(int)cp.x+xx,sy=(int)cp.y+yy;if(sx>=0&&sx<W&&sy>=top&&sy<bottom)fb[sy*STRIDE+sx]=mix_rgb(fb[sy*STRIDE+sx],ink,(i&1)?.27f:.6f);}
 }
 Vec3 pad=surface_site(&game,1);Vec3 site_positions[10];FieldBuilding site_shapes[10];for(int i=1;i<10;i++){if(i==6)continue;site_positions[i]=surface_poi(&game,i);site_shapes[i]=field_site_building(&game,i);}
 unsigned grass=mix_rgb(b->color,RGB(90,130,60),.55f);
 if(biome==BIOME_OCEAN)grass=mix_rgb(b->color,RGB(74,140,68),.5f);
 if(biome==BIOME_DESERT)grass=mix_rgb(b->color,RGB(180,140,70),.55f);
 if(biome==BIOME_ICE)grass=mix_rgb(b->color,RGB(200,220,230),.5f);
 if(biome==BIOME_VOLCANIC)grass=mix_rgb(b->color,RGB(70,55,50),.55f);
 if(biome==BIOME_FOREST)grass=mix_rgb(b->color,RGB(50,110,55),.55f);
 if(biome==BIOME_CLOUD)grass=RGB(100,123,138);
 if(planet_lave_i())grass=RGB(98,137,60);
 if(game.system==7&&game.planet==4)grass=RGB(180,200,211);
 unsigned waterc=biome==BIOME_CLOUD?mix_rgb(sky_lo,WHITE,.3f):mix_rgb(b->color,RGB(52,118,168),.4f);
 int horizon=110+(int)(tanf(game.pitch)*240.f);
 /* Specular glitter on water / ice near the horizon band. */
 if(!high_contrast&&(biome==BIOME_OCEAN||biome==BIOME_ICE)){
  for(int i=0;i<28;i++){
   int x=16+(i*17+(int)(game.time*55))%(W-32);
   int y=horizon+8+(int)(sinf(game.time*5+i)*4)+(i&3);
   if(y>=bottom-4)y=bottom-5;
   unsigned ink=biome==BIOME_ICE?RGB(200,220,240):mix_rgb(waterc,RGB(220,240,255),.4f);
   sfx_add(x,y,ink,top,bottom-1);
   if((i&3)==0)pixel(x,y,WHITE);
  }
 }
 if(!high_contrast&&biome==BIOME_VOLCANIC){
  for(int i=0;i<12;i++){
   int x=50+i*32+(int)(sinf(game.time*2+i)*8);
   int y=bottom-48-(int)fmodf(game.time*40+i*19,36);
   sfx_add(x,y,RGB(200,70,25),top,bottom-1);
  }
 }
 /* Three world-anchored ridge layers break the old evenly repeating skyline
  * into a broad Lave valley with distant blue ranges and nearer green folds. */
 for(int x=0;x<W&&!(game.system==7&&game.planet==3);x+=3){
  float bearing=game.yaw+atanf((x-240)/240.f);
  float far=.52f*planet_ridge_noise(bearing,b->seed,7)+.31f*planet_ridge_noise(bearing,b->seed^0x91e10da5u,17)+.17f*planet_ridge_noise(bearing,b->seed^0x63d83595u,39);
  float middle=.58f*planet_ridge_noise(bearing,b->seed^0xa511e9b3u,12)+.42f*planet_ridge_noise(bearing,b->seed^0x7f4a7c15u,31);
  float near=.56f*planet_ridge_noise(bearing,b->seed^0x4cf5ad43u,19)+.44f*planet_ridge_noise(bearing,b->seed^0x52dce729u,47);
  int fy=horizon-(int)(19+far*32),my=horizon-(int)(8+middle*22),ny=horizon-(int)(3+near*13);
  unsigned fc=mix_rgb(sky_lo,grass,.20f),mc=mix_rgb(sky_lo,grass,.48f),nc=mix_rgb(grass,b->accent,.35f);
  planet_cliprect(x,fy,3,horizon-fy+1,surface_shade(fc));
  if(far>.82f)planet_cliprect(x,fy,2,2,surface_shade(mix_rgb(fc,RGB(206,218,207),.45f)));
  planet_cliprect(x,my,3,horizon-my+1,surface_shade(mc));
  planet_cliprect(x,ny,3,horizon-ny+1,surface_shade(nc));
 }
 for(int y=horizon>top?horizon:top;y<bottom;y++){
  float t=(y-horizon)/(float)fmaxf(1,bottom-horizon);
  unsigned c=surface_shade(biome==BIOME_OCEAN&&t<.12f?waterc:mix_rgb(grass,mix_rgb(b->accent,RGB(48,78,40),.35f),t*.45f));
  if(game.system==7&&game.planet==3)c=surface_shade(mix_rgb(waterc,RGB(83,100,146),.25f+.15f*sinf(y*.12f+game.time*.15f)));
  for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;
 }
 if(game.system==7&&game.planet==3){
  /* Layered cloud sea below the decks; bearings move with the view and wind. */
  for(int layer=0;layer<5;layer++)for(int x=0;x<W;x+=4){
   float bearing=game.yaw+atanf((x-240)/240.f)+game.world_clock*.001f*(layer+1);
   float n=planet_ridge_noise(bearing,b->seed+layer*137,8+layer*3);
   int y=horizon+layer*24-(int)(n*(16+layer*6));
   unsigned ink=surface_shade(mix_rgb(waterc,RGB(92,109,149),layer*.11f));
   planet_cliprect(x,y,4,26+layer*5,ink);
  }
 }
 memset(surface_depth,255,sizeof(surface_depth));surface_depth_on=1;
 if(game.system==7){
  /* Draw opaque airport first so expensive textured terrain/foliage behind it
   * fail depth testing. Other worlds retain their legacy depth-reset order. */
  float y=terrain_height(&game,pad.x,pad.z)+.35f;
  planet_quad((Vec3){pad.x-field_port_half(&game),y,pad.z-field_port_half(&game)},(Vec3){pad.x+field_port_half(&game),y,pad.z-field_port_half(&game)},(Vec3){pad.x+field_port_half(&game),y,pad.z+field_port_half(&game)},(Vec3){pad.x-field_port_half(&game),y,pad.z+field_port_half(&game)},RGB(62,80,89));flush_meshes();
  for(int i=0;i<FIELD_PORT_BUILDINGS;i++){FieldBuilding s=field_port_building(&game,i);Vec3 p={pad.x+s.x,24,pad.z+s.z};surface_starport_building(i,p,s,b->accent);}
 }
 sceRtcGetCurrentTick(&lave_render_marks[1]);
 if(game.system==7){
  surface_padx=pad.x;surface_padz=pad.z;surface_path_side=b->seed&1?1.f:-1.f;
  surface_ocean=surface_shade(waterc);
  for(int x=0;x<240;x++){float ray=(x*2+1-proj_ox)/240.f;surface_rayx[x]=surface_cy*ray;surface_rayz[x]=-surface_sy*ray;}
  for(int i=0;i<=320;i++){float t=i/320.f;surface_trailx[i]=pad.x-surface_path_side*420*t*(1-t);}
  for(int bank=0;bank<8;bank++)for(int i=0;i<32;i++)surface_meadow[bank][i]=surface_shade(mix_rgb(mix_rgb(RGB(78,119,43),RGB(132,159,72),i/31.f),RGB(29,65,44),bank*.065f));
  for(int i=0;i<16;i++)surface_path[i]=surface_shade(mix_rgb(RGB(137,104,66),RGB(193,155,102),i/15.f));
  if(game.planet>1){
   unsigned lo=game.planet==2?RGB(58,85,48):game.planet==4?RGB(183,202,213):RGB(82,108,129);
   unsigned hi=game.planet==2?RGB(104,131,70):game.planet==4?RGB(218,228,233):RGB(125,151,164);
   for(int bank=0;bank<8;bank++)for(int i=0;i<32;i++)surface_meadow[bank][i]=surface_shade(mix_rgb(mix_rgb(lo,hi,i/31.f),RGB(38,65,83),bank*.045f));
  }
  if(game.planet==3){draw_cloud_decks(pad);}
  else {
  /* One world-aligned lattice shared with collision, including distant hills.
   * No overlapping terrain layers or depth reset between ground and props. */
  int cell=SURFACE_CELL*4,cx=(int)floorf(game.pos.x/cell),cz=(int)floorf(game.pos.z/cell);
  for(int iz=-14;iz<=14;iz++)for(int ix=-14;ix<=14;ix++){
   float x=(cx+ix)*(float)cell,z=(cz+iz)*(float)cell;
   if(x>=pad.x-field_port_half(&game)&&x+cell<=pad.x+field_port_half(&game)&&z>=pad.z-field_port_half(&game)&&z+cell<=pad.z+field_port_half(&game))continue; /* Hidden by the port apron. */
   float dx=x+cell*.5f-game.pos.x,dz=z+cell*.5f-game.pos.z;
   float forward=dx*surface_sy+dz*surface_cy,side=dx*surface_cy-dz*surface_sy;
   if(forward< -240||forward>2240||fabsf(side)>forward*1.45f+240)continue;
   if(drawcount>1800)flush_meshes();
   float a=terrain_height(&game,x,z),bb=terrain_height(&game,x+cell,z),c=terrain_height(&game,x+cell,z+cell),d=terrain_height(&game,x,z+cell);
   int bank=(int)fmaxf(0,fminf(7,3+(bb-a+c-d)*.035f-(d-a+c-bb)*.02f));
   surface_material=terrain_is_water(&game,x+cell*.5f,z+cell*.5f)?0:1+bank;
   planet_quad((Vec3){x,a,z},(Vec3){x+cell,bb,z},(Vec3){x+cell,c,z+cell},(Vec3){x,d,z+cell},surface_material?grass:waterc);
  }
  flush_meshes();surface_material=0;
  }
 }else{
 /* Coarse outer terrain carries the ground right out to remote POIs.  It is
  * deliberately low density; the old detailed grid still takes over nearby. */
 int far_cell=280,far_span=8;
 int fgx0=(int)floorf(game.pos.x/far_cell)-far_span,fgz0=(int)floorf(game.pos.z/far_cell)-far_span;
 for(int iz=0;iz<far_span*2+1;iz++)for(int ix=0;ix<far_span*2+1;ix++){
  float x0=(fgx0+ix)*(float)far_cell,z0=(fgz0+iz)*(float)far_cell,x1=x0+far_cell,z1=z0+far_cell;
  if(x0>=pad.x-field_port_half(&game)&&x1<=pad.x+field_port_half(&game)&&z0>=pad.z-field_port_half(&game)&&z1<=pad.z+field_port_half(&game))continue;
  float cx=x0+far_cell*.5f,cz=z0+far_cell*.5f,gy=terrain_height(&game,cx,cz);
  Vec3 mid=camera(&game,(Vec3){cx,gy,cz});if(mid.z<-180||mid.z>2450)continue;
  int water=terrain_is_water(&game,cx,cz);unsigned far_hash=planet_hash(b->seed^(unsigned)(fgx0+ix)*73856093u^(unsigned)(fgz0+iz)*19349663u);
  unsigned col=water?waterc:planet_lave_i()?mix_rgb(grass,RGB(53,116,48),.05f+(far_hash&15)*.006f):mix_rgb(grass,b->accent,((ix+iz)&1)?.18f:.31f);
  planet_quad((Vec3){x0,terrain_height(&game,x0,z0),z0},(Vec3){x1,terrain_height(&game,x1,z0),z0},
              (Vec3){x1,terrain_height(&game,x1,z1),z1},(Vec3){x0,terrain_height(&game,x0,z1),z1},col);
 }
 /* Commit the coarse horizon first.  The detailed central mesh gets a fresh
  * depth buffer below, so coplanar terrain cannot flicker as the player moves. */
 flush_meshes();memset(surface_depth,255,sizeof(surface_depth));
 int cell=SURFACE_CELL,span=7;
 int gx0=(int)floorf(game.pos.x/cell)-span,gz0=(int)floorf(game.pos.z/cell)-span;
 for(int iz=0;iz<span*2+1;iz++)for(int ix=0;ix<span*2+1;ix++){
  float x0=(gx0+ix)*(float)cell,z0=(gz0+iz)*(float)cell,x1=x0+cell,z1=z0+cell;
  if(x0>=pad.x-field_port_half(&game)&&x1<=pad.x+field_port_half(&game)&&z0>=pad.z-field_port_half(&game)&&z1<=pad.z+field_port_half(&game))continue;
  float cx=x0+cell*.5f,cz=z0+cell*.5f;
  float gy=terrain_height(&game,cx,cz);
  Vec3 mid=camera(&game,(Vec3){cx,gy,cz});if(mid.z< -60||mid.z>620)continue;
  int water=terrain_is_water(&game,cx,cz);
  int checker=((gx0+ix)+(gz0+iz))&1;
  unsigned patch_hash=planet_hash(b->seed^(unsigned)(gx0+ix)*73856093u^(unsigned)(gz0+iz)*19349663u);
  unsigned col=water?waterc:planet_lave_i()?mix_rgb(grass,RGB(66,132,54),.035f+(patch_hash&15)*.008f):checker?grass:mix_rgb(grass,b->accent,.28f);
  planet_quad((Vec3){x0,terrain_height(&game,x0,z0),z0},(Vec3){x1,terrain_height(&game,x1,z0),z0},
              (Vec3){x1,terrain_height(&game,x1,z1),z1},(Vec3){x0,terrain_height(&game,x0,z1),z1},col);
 }
 }
 surface_bridges();
 sceRtcGetCurrentTick(&lave_render_marks[2]);
 /* Stable, biome-authored ground cover. Density is bounded by the ambience
  * profile and every tuft stays tied to its world cell while the player moves. */
 if(ambience.ground_density>0&&game.system!=7&&!(planet_scene_camera==1&&game.pos.y-terrain_height(&game,game.pos.x,game.pos.z)>100)){
  unsigned grass_ink=biome==BIOME_DESERT?mix_rgb(grass,RGB(157,119,55),.55f):biome==BIOME_ICE?mix_rgb(grass,RGB(132,184,201),.48f):biome==BIOME_OCEAN?mix_rgb(grass,RGB(43,123,91),.52f):mix_rgb(grass,RGB(46,112,48),.48f);
  int grass_cell=planet_lave_i()?56:80,gcx=(int)floorf(game.pos.x/grass_cell),gcz=(int)floorf(game.pos.z/grass_cell);
  for(int gz=-4;gz<=4;gz++)for(int gx=-4;gx<=4;gx++){
   int cx=gcx+gx,cz=gcz+gz;unsigned h=planet_hash(b->seed^(unsigned)cx*73856093u^(unsigned)cz*19349663u);
   if((int)(h&3)>=ambience.ground_density)continue;
   float x=cx*grass_cell+12+(h%54),z=cz*grass_cell+12+((h>>8)%54);if(terrain_is_water(&game,x,z))continue;
   float y=terrain_height(&game,x,z),height=planet_lave_i()?10.f+(h&7):(biome==BIOME_OCEAN?9.f:biome==BIOME_DESERT?3.f:4.f)+(h&7);Vec3 mid=camera(&game,(Vec3){x,y+height*.5f,z});if(mid.z<14||mid.z>580)continue;
   float sway=sinf(game.time*1.6f+(h&255))*.9f,width=1.5f+(h>>5&1);
   planet_quad((Vec3){x-width,y,z},(Vec3){x+width,y,z},(Vec3){x+sway+width*.35f,y+height,z},(Vec3){x+sway-width*.35f,y+height,z},grass_ink);
   planet_quad((Vec3){x,y,z-width},(Vec3){x,y,z+width},(Vec3){x+sway*.6f,y+height,z+width*.35f},(Vec3){x+sway*.6f,y+height,z-width*.35f},mix_rgb(grass_ink,b->accent,.2f));
   if(biome==BIOME_FOREST&&(h&15)==1)surface_ambient_mark((Vec3){x+sway,y+height+1,z},2,2,(h&16)?RGB(241,169,190):RGB(236,205,91));
   else if(biome==BIOME_ICE&&(h&15)==2)surface_ambient_mark((Vec3){x,y+height+2,z},2,3,RGB(187,225,239));
  }
 }
 {
  float y=terrain_height(&game,pad.x,pad.z)+.35f;
  planet_quad((Vec3){pad.x-field_port_half(&game),y,pad.z-field_port_half(&game)},(Vec3){pad.x+field_port_half(&game),y,pad.z-field_port_half(&game)},(Vec3){pad.x+field_port_half(&game),y,pad.z+field_port_half(&game)},(Vec3){pad.x-field_port_half(&game),y,pad.z+field_port_half(&game)},RGB(62,80,89));
  /* Wide clear circulation lanes, not additional collision obstacles. */
  for(int side=-1;side<=1;side+=2){
   float x=pad.x+side*185;
   planet_quad((Vec3){x-1,y+.08f,pad.z-700},(Vec3){x+1,y+.08f,pad.z-700},(Vec3){x+1,y+.08f,pad.z+700},(Vec3){x-1,y+.08f,pad.z+700},RGB(211,169,86));
  }
  for(int i=-17;i<=17;i++){
   float z=pad.z+i*40;planet_quad((Vec3){pad.x-2,y+.1f,z},(Vec3){pad.x+2,y+.1f,z},(Vec3){pad.x+2,y+.1f,z+16},(Vec3){pad.x-2,y+.1f,z+16},RGB(158,191,194));
  }
  for(int i=0;i<20;i++)for(int side=-1;side<=1;side+=2){Vec3 p={pad.x+side*198,y,pad.z-700+i*72};surface_box(p,2,2,2,RGB(61,196,211));}
 }
 {
  float px=pad.x,pz=pad.z,h=terrain_height(&game,px,pz)+.6f,s=FIELD_PLAYER_PAD;
  unsigned slab=RGB(55,72,85),edge=RGB(96,150,163),mark=RGB(248,252,236);
  /* Warm ochre apron so the pad reads as maintained hardware, not a UI glyph. */
  planet_quad((Vec3){px-s-8,h-.1f,pz-s-8},(Vec3){px+s+8,h-.1f,pz-s-8},(Vec3){px+s+8,h-.1f,pz+s+8},(Vec3){px-s-8,h-.1f,pz+s+8},RGB(139,75,55));
  planet_quad((Vec3){px-s-5,h,pz-s-5},(Vec3){px+s+5,h,pz-s-5},(Vec3){px+s+5,h,pz+s+5},(Vec3){px-s-5,h,pz+s+5},edge);
  planet_quad((Vec3){px-s,h+.2f,pz-s},(Vec3){px+s,h+.2f,pz-s},(Vec3){px+s,h+.2f,pz+s},(Vec3){px-s,h+.2f,pz+s},slab);
  planet_quad((Vec3){px-3,h+.4f,pz-24},(Vec3){px+3,h+.4f,pz-24},(Vec3){px+3,h+.4f,pz+24},(Vec3){px-3,h+.4f,pz+24},mark);
  planet_quad((Vec3){px-24,h+.4f,pz-3},(Vec3){px+24,h+.4f,pz-3},(Vec3){px+24,h+.4f,pz+3},(Vec3){px-24,h+.4f,pz+3},mark);
  if(game.surface==0){
   Vec3 a=camera(&game,(Vec3){px,h+.6f,pz}),bv=camera(&game,(Vec3){px,92.f,pz});
    if(a.z>12&&bv.z>12){Point p=project(a),q=project(bv);if(p.y>top&&p.y<bottom&&q.y>top&&q.y<bottom){line((int)p.x,(int)p.y,(int)q.x,(int)q.y,RGB(85,212,212));line((int)p.x+1,(int)p.y,(int)q.x+1,(int)q.y,RGB(229,210,163));}}
  }
  /* Pad corner beacons — presentation pulse. */
  if(!high_contrast&&game.surface!=2){
   for(int c=0;c<4;c++){
    float ox=(c&1)?s:-s,oz=(c&2)?s:-s;
    Vec3 cv=camera(&game,(Vec3){px+ox,h+1.f,pz+oz});if(cv.z<12||cv.z>520)continue;
    Point cp=project(cv);if(cp.y<top||cp.y>bottom)continue;
    unsigned lamp=((int)(game.time*3)+c)&2?RGB(255,183,76):RGB(211,145,65);
    space_anim_draw(SPACE_ANIM_BEACON,(int)cp.x,(int)cp.y,((int)(game.time*4)+c)&3,lamp);
   }
  }
 }
 for(int poi=1;poi<10;poi++){if(poi==6)continue;
  Vec3 p=site_positions[poi];p.y=terrain_height(&game,p.x,p.z);FieldBuilding bounds=site_shapes[poi];Vec3 v=camera(&game,add(p,(Vec3){0,bounds.height*.5f,0}));float radius=sqrtf(bounds.w*bounds.w+bounds.d*bounds.d+bounds.height*bounds.height*.25f)+6;if(v.z+radius<4||v.z-radius>2250||fabsf(v.x)>v.z+radius*1.5f||fabsf(v.y)>.65f*v.z+radius*1.5f)continue;
  unsigned ink=game.surface_progress[game.system][game.planet]&field_site_bit(poi)?RGB(75,105,95):RGB(190,158,87);
  FieldBuilding shape=site_shapes[poi];
  int site_kind=field_site_kind(&game,game.system,game.planet,poi);
  if(poi==1){static const int contract_shape[]={FIELD_WEATHER,FIELD_GARDEN,FIELD_ARCHIVE};site_kind=contract_shape[field_profile(&game,game.system,game.planet).job];}
  surface_field_structure(p,shape,site_kind,ink,field_hash(field_seed(game.system,game.planet)+poi*7159u));
 }
 for(int i=0;i<3;i++){const FieldBuilding *s=&field_garage_walls[i];Vec3 p={pad.x+s->x,terrain_height(&game,pad.x,pad.z),pad.z+s->z};surface_box(p,s->w,s->d,s->height,RGB(105,133,148));}
 {Vec3 roof={pad.x+FIELD_GARAGE_X,terrain_height(&game,pad.x,pad.z)+48,pad.z-20};surface_box(roof,46,48,4,RGB(49,93,111));}
 if(!game.rover_driving&&!(planet_scene_camera==2&&length(sub(game.pos,game.rover_pos))<70)){Vec3 rv=game.rover_pos;float base=terrain_height(&game,rv.x,rv.z);for(int wheel=0;wheel<4;wheel++)base=fmaxf(base,terrain_height(&game,rv.x+((wheel&1)?11:-11),rv.z+((wheel&2)?10:-10)));rv.y=base+5;surface_box(rv,10,16,8,RGB(179,126,57));rv.z-=3;rv.y=base+13;surface_box(rv,7,8,10,RGB(56,115,136));for(int wheel=0;wheel<4;wheel++){Vec3 w=game.rover_pos;w.x+=(wheel&1)?11:-11;w.z+=(wheel&2)?10:-10;w.y=base+.7f;surface_box(w,3,4,9,RGB(28,33,37));}}
 flush_meshes();
 surface_ambient_particles(b,ambience,ambience_field);
 surface_ambient_birds(b,ambience,daylight);
 surface_poi_ambient_fx(site_positions,site_shapes,b);
 sceRtcGetCurrentTick(&lave_render_marks[3]);
 static PlanetProp prop[1024];int nprop=0;
 unsigned seed=b->seed;
 float px=pad.x,pz=pad.z;
 if(game.system==7&&game.planet>1)for(int fog=0;fog<4;fog++)for(int i=0;i<128;i++)biome_prop_inks[fog][i]=surface_shade(mix_rgb(biome_prop_palette[i],sky_lo,fog*.16f));
 if(planet_lave_i()){
  lave_prepare_props(pad,site_positions,site_shapes,seed);
  for(int fog=0;fog<4;fog++)for(int i=0;i<32;i++)lave_tree_inks[fog][i]=surface_shade(mix_rgb(lave_vegetation_palette[i],RGB(117,166,139),fog*.15f));
  for(int i=0;i<lave_prop_count&&nprop<1024;i++){
   PlanetProp p=lave_prop_cache[i];float dx=p.x-game.pos.x,dz=p.w-game.pos.z;
   float depth=dx*surface_sy+dz*surface_cy,side=dx*surface_cy-dz*surface_sy;
   if(depth< -100||depth>1350||fabsf(side)>depth*1.35f+100||(p.kind>=4&&dx*dx+dz*dz>280*280))continue;
   p.z=depth;prop[nprop++]=p;
  }
 }else{
 /* Stable cells follow the player, so wilderness does not stop at the pad. */
 int tile=game.system==7?70:90,cx0=(int)floorf(game.pos.x/tile),cz0=(int)floorf(game.pos.z/tile);
 int range=game.system==7?14:4;
 for(int iz=-range;iz<=range&&nprop<1024;iz++)for(int ix=-range;ix<=range&&nprop<1024;ix++){
  unsigned h;float x,z;int lave_tree=0;
  if(planet_lave_i())lave_tree=field_lave_tree_cell(seed,cx0+ix,cz0+iz,&x,&z,&h);
  else {field_prop_cell(game.system,game.planet,seed,cx0+ix,cz0+iz,&x,&z,&h);if(game.system!=7&&biome!=BIOME_FOREST&&(h&3)==0)continue;}
  float dx=x-game.pos.x,dz=z-game.pos.z,depth=dx*surface_sy+dz*surface_cy,side=dx*surface_cy-dz*surface_sy;
  if(planet_lave_i()&&(depth< -100||depth>1350||fabsf(side)>depth*1.5f+150||(!lave_tree&&dx*dx+dz*dz>280*280)))continue;
  if(terrain_is_water(&game,x,z)||(fabsf(x-px)<field_port_clear(&game)&&fabsf(z-pz)<field_port_clear(&game)))continue;
  if(planet_lave_i()){float t=(z-pz)/FIELD_OBSERVATORY_DISTANCE;if(t>0&&t<1&&fabsf(x-(px-surface_path_side*420*t*(1-t)))<38)continue;}
  int reserved=0;for(int i=1;i<10;i++){if(i==6)continue;if(fabsf(x-site_positions[i].x)<site_shapes[i].w+30&&fabsf(z-site_positions[i].z)<site_shapes[i].d+30){reserved=1;break;}}if(reserved)continue;
  Vec3 v=camera(&game,(Vec3){x,terrain_height(&game,x,z),z});if(!planet_lave_i()&&(v.z<8||v.z>(game.system==7?1280:560)||fabsf(v.x)>v.z*1.4f+100))continue;
  int kind=(h>>16)%4;
  if(game.system==7&&game.planet==4&&(kind==1||kind==3)&&(h&1))continue;
  if(game.system==7&&kind!=0&&dx*dx+dz*dz>320*320)continue;
  if(game.system==7&&game.planet==3)continue; /* Purpose-placed planters below. */
  if(planet_lave_i()&&!lave_tree){int detail=(h>>12)&7;kind=detail<2?4:detail<4?5:detail<6?6:7;}
  else if(biome==BIOME_DESERT||biome==BIOME_VOLCANIC)kind=kind==0?2:kind;
  prop[nprop++]=(PlanetProp){v.z,x,z,kind,h};
 }
 }
 if(nprop>1)qsort(prop,nprop,sizeof(prop[0]),planet_prop_cmp);
 for(int index=0;index<nprop;index++){int i=game.system==7?nprop-1-index:index;
  if(planet_lave_i()){draw_lave_vegetation(prop[i].x,prop[i].w,prop[i].h,prop[i].kind);if(prop[i].kind<4)field_tree_leaves(prop[i].x,prop[i].w,prop[i].h,58.f+((prop[i].h>>9)%31));}
  else {if(game.system==7)draw_lave_vegetation(prop[i].x,prop[i].w,prop[i].h,prop[i].kind);else draw_tree_billboard(prop[i].x,prop[i].w,prop[i].h,prop[i].kind);if(prop[i].kind==0&&(biome==BIOME_FOREST||biome==BIOME_OCEAN))field_tree_leaves(prop[i].x,prop[i].w,prop[i].h,biome==BIOME_FOREST?90.f+(prop[i].h%65):38.f+(prop[i].h%25));}
  surface_sprite_z=0;
 }
 if(game.system==7&&game.planet==3)for(int i=0;i<12;i++){float x=pad.x-250+(i%6)*95,z=pad.z+(i<6?-235:235);draw_lave_vegetation(x,z,seed+i,i%4);}
 sceRtcGetCurrentTick(&lave_render_marks[4]);
/* Large port: shared collision geometry, perpetual staggered traffic. */
 if(game.system!=7)for(int i=0;i<FIELD_PORT_BUILDINGS;i++){FieldBuilding structure=field_port_building(&game,i);const FieldBuilding *s=&structure;Vec3 p={pad.x+s->x,0,pad.z+s->z};p.y=terrain_height(&game,p.x,p.z);surface_starport_building(i,p,*s,b->accent);}
 for(int lane=0;lane<2;lane++){Vec3 p={pad.x+(lane?FIELD_TRAFFIC_X:-FIELD_TRAFFIC_X),0,pad.z+FIELD_TRAFFIC_Z};p.y=terrain_height(&game,p.x,p.z)+.8f;surface_heavy_berth(p,lane);
 float t=fmodf(game.world_clock+lane*63+(b->seed%40),150),lift=t<40?(40-t)*(40-t)*.8f:t<90?0:(t-90)*(t-90)*.8f;
 Vec3 traffic=p;traffic.y+=32+lift;traffic.z+=lift*.15f;
 shipmesh_stretched(mesh_id(lane?"ANACONDA":"PYTHON"),traffic,lane?3.14159f:0,0,.6f,1, lane?RGB(225,146,71):RGB(100,185,218),0);
 }flush_meshes();









 for(int lane=0;lane<5;lane++){Vec3 p=surface_sky_ship(lane);Vec3 pad0=surface_site(&game,1);float heading=atan2f(p.z-pad0.z,-(p.x-pad0.x));shipmesh_stretched(mesh_id(lane%2?"COBRA MK 3":"ADDER"),p,heading,0,.5f+lane*.12f,1,RGB(157+lane*15,184,213-lane*18),0);}flush_meshes();
 /* Keep the parked hull readable as a ship, not a debug-sized wireframe.
  * The close surface camera has very little depth, so use a restrained native
  * scale while retaining the same mesh identity and landing/boarding state. */
 if(game.surface&&!game.planet_sequence&&!planet_scene_camera){
  Vec3 ship_pos=game.surface==1?game.pos:game.ship_pos;
  Vec3 ship_cam=camera(&game,ship_pos);
  if(ship_cam.z>18){Point sp=project(ship_cam);int sw=game.surface==1?18:14;
   (void)sp;(void)sw;
  }
  float scale=game.surface==1?.66f:.72f,low=0;const Mesh *hull=&meshes[mesh_id(player_ships[game.ship].name)];
  for(int i=0;i<hull->vertices;i++)low=fminf(low,hull->v[i].y*scale);
  ship_pos.y=terrain_height(&game,ship_pos.x,ship_pos.z)+2-low;
  shipmesh_stretched(mesh_id(player_ships[game.ship].name),ship_pos,game.surface==1?game.yaw:0,0,scale,1.f,ship_paint[game.ship%16],0);flush_meshes();
 }
 for(int i=0;i<LIFE_COUNT;i++)field_fauna_shadow(&game.life[i]);
 for(int i=0;i<LIFE_COUNT;i++)if(game.life[i].alive){Lifeform *l=&game.life[i];if(game.system==7&&game.planet==3&&l->kind!=LIFE_FAUNA){Vec3 p=l->pos;p.y=terrain_height(&game,p.x,p.z);surface_box(p,9,9,5,RGB(79,108,131));flush_meshes();}draw_life_billboard(l);}
 for(int poi=1;poi<10;poi++){if(poi==6)continue;FieldBuilding shape=site_shapes[poi];Vec3 p=site_positions[poi],across={1,0,0};float width=shape.w*.8f;
 float high=terrain_height(&game,p.x,p.z);for(int i=0;i<4;i++)high=fmaxf(high,terrain_height(&game,p.x+(i&1?shape.w:-shape.w),p.z+(i&2?shape.d:-shape.d)));
 p.y=high+17;p.z-=shape.d*.95f+.65f;
 Vec3 sign_view=camera(&game,p);if(sign_view.z<4||sign_view.z>900||fabsf(sign_view.x)>sign_view.z+width||fabsf(sign_view.y)>.65f*sign_view.z+width)continue;
 char sign[32];snprintf(sign,sizeof(sign),"%d %s",field_nav_number(poi),field_site_sign(&game,poi));surface_building_sign(p,across,width,sign);}
 {float y=terrain_height(&game,pad.x,pad.z)+27;
 if(game.pos.x<pad.x+FIELD_GARAGE_X-46)surface_building_sign((Vec3){pad.x+FIELD_GARAGE_X-46.6f,y,pad.z-20},(Vec3){0,0,-1},48,"GARAGE");
 if(game.pos.x>pad.x+FIELD_GARAGE_X+46)surface_building_sign((Vec3){pad.x+FIELD_GARAGE_X+46.6f,y,pad.z-20},(Vec3){0,0,1},48,"GARAGE");
 if(game.pos.z<pad.z-64)surface_building_sign((Vec3){pad.x+FIELD_GARAGE_X,y,pad.z-64.6f},(Vec3){1,0,0},45,"GARAGE");
 }
 surface_depth_on=0;surface_sprite_z=0;surface_light=1;
 sceRtcGetCurrentTick(&lave_render_marks[5]);
 if(game.surface!=2&&!planet_scene_camera){line(227,110,236,110,RGB(193,139,77));line(244,110,253,110,RGB(193,139,77));line(240,97,240,106,RGB(193,139,77));line(240,114,240,123,RGB(193,139,77));}
}
/* Dedicated on-foot chrome — charcoal + ochre, not gold/cyan debug bands. */
static void surface_compass_icon(int x,int rover,unsigned ink){
 if(rover){rect(x-5,19,11,5,ink);rect(x-3,17,7,3,ink);rect(x-5,24,3,2,ink);rect(x+3,24,3,2,ink);rect(x-2,18,5,2,RGB(21,28,39));}
 else {rect(x-6,8,13,8,RGB(21,28,39));button_icon(x-5,7,'T',UI_CYAN);line(x,16,x-7,24,ink);line(x,16,x+7,24,ink);line(x-7,24,x,21,ink);line(x,21,x+7,24,ink);rect(x-1,18,3,7,ink);}
}
static unsigned surface_condition_ink(int value){
 return value>60?RGB(78,190,104):value>30?RGB(232,142,54):RGB(210,66,60);
}
static void surface_condition_meter(int x,int y,int value,int suit){
 if(value<0)value=0;
 if(value>100)value=100;
 unsigned ink=surface_condition_ink(value),edge=high_contrast?WHITE:UI_MUTED;
 /* Distinct native silhouettes keep SUIT and HEALTH readable without labels. */
 if(suit){line(x+4,y,x+8,y,edge);line(x+2,y+2,x+4,y,edge);line(x+8,y,x+10,y+2,edge);line(x+2,y+2,x+3,y+8,edge);line(x+10,y+2,x+9,y+8,edge);line(x+3,y+8,x+6,y+11,edge);line(x+9,y+8,x+6,y+11,edge);rect(x+5,y+3,3,5,ink);}
 else {rect(x+4,y,5,11,edge);rect(x+1,y+3,11,5,edge);rect(x+5,y+1,3,9,ink);rect(x+2,y+4,9,3,ink);}
 int segments=(value+19)/20;
 for(int i=0;i<5;i++){int sx=x+15+i*8;rect(sx,y+3,6,6,RGB(32,40,48));rect(sx,y+3,6,1,edge);if(i<segments){rect(sx+1,y+4,4,4,ink);if(high_contrast&&i==segments-1)pixel(sx+2,y+5,WHITE);}}
}
static void surface_r_hint(int x,int y){
 rect(x,y,14,9,UI_RAISED);rect(x,y,14,1,high_contrast?WHITE:UI_MUTED);rect(x,y,1,9,high_contrast?WHITE:UI_MUTED);text_px(x+3,y+1,WHITE,"R");
}
static void planet_eva_hud(void){
 Body *b=&game.bodies[game.planet];
 static const char *biome_name[]={"OCEAN ISLAND","ARID FLATS","ICE FIELD","VOLCANIC SCRUB","FOREST RISE","CLOUD PLATFORM"};
 int biome=planet_biome(b);
 rect(0,0,W,28,RGB(21,28,39));rect(0,26,W,2,RGB(193,139,77));
 float hour=field_local_hour(&game,game.system,game.planet);text(50,0,RGB(85,212,212),"%02d:%02d",(int)hour,(int)(hour*60)%60);(void)biome_name;(void)biome;
 for(int i=0;i<4;i++){float a=i*1.5707963f-game.yaw;a=atan2f(sinf(a),cosf(a));if(fabsf(a)<1.5f)text((int)(30+a*18),1,UI_MUTED,"%c","NESW"[i]);}
 int occupied[20]={0};for(int pass=-1;pass<11;pass++){int i=pass<0?surface_nav_poi:pass-1;if(pass>=0&&i==surface_nav_poi)continue;if(i< -1||i>9)continue;Vec3 p=i<0?game.ship_pos:surface_poi(&game,i);float a=atan2f(p.x-game.pos.x,p.z-game.pos.z)-game.yaw;a=atan2f(sinf(a),cosf(a));if(fabsf(a)>1.5f)continue;int x=(int)(240+a*144),bucket=x/24;if(bucket<0||bucket>=20||occupied[bucket])continue;occupied[bucket]=1;unsigned ink=i==surface_nav_poi?UI_GOLD:UI_CYAN;if(i<0||i==6)surface_compass_icon(x,i==6,ink);else text((x-4)/8,2,ink,"%d",field_nav_number(i));}
 rect(0,240,W,32,RGB(21,28,39));rect(0,240,W,2,RGB(193,139,77));
 if(game.rover_driving){
  text_px(6,247,UI_CYAN,"R ACCEL  L BRAKE");button_icon(132,246,'O',UI_CYAN);text_px(145,248,UI_MUTED,"DRIFT");
  button_icon(190,246,'X',UI_CYAN);text_px(203,248,UI_MUTED,"BOOST");button_icon(248,246,'T',UI_CYAN);text_px(261,248,UI_MUTED,"PARK");
  rect(315,246,48,6,RGB(32,40,48));rect(316,247,(int)(46*game.rover_charge/100),4,UI_CYAN);
  text_px(370,246,WHITE,"%3d M/S",(int)game.speed);text_px(6,263,UI_MUTED,"NUB STEER / LOOK    START MAP");
  return;
 }
 button_icon(8,246,'X',UI_CYAN);text(3,31,UI_MUTED,"USE");
 button_icon(88,246,'O',UI_CYAN);text(13,31,UI_MUTED,"SCAN");
 text(21,31,UI_MUTED,"START MAP");
 surface_condition_meter(300,244,100-(int)game.hazard,1);
 surface_condition_meter(405,244,(int)game.energy,0);
 if(game.hazard>=100)text(1,33,RED,"EXPOSURE DAMAGES SUIT! RETURN TO YOUR SHIP");
 else if(game.message_time>0&&game.message[0])text(1,33,RGB(229,210,163),"%.58s",game.message);
 else {surface_r_hint(8,262);text(3,33,UI_MUTED,"BOOST / JUMP");}
}
