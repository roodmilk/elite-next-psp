/* Flat local surface: grass, water, trees, bushes and a cyan pad. */
static unsigned planet_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static unsigned mix_rgb(unsigned a,unsigned b,float t){
 int ar=a&255,ag=(a>>8)&255,ab=(a>>16)&255,br=b&255,bg=(b>>8)&255,bb=(b>>16)&255;
 if(t<0)t=0;
 if(t>1)t=1;
 return RGB((int)(ar+(br-ar)*t),(int)(ag+(bg-ag)*t),(int)(ab+(bb-ab)*t));
}
static void planet_cliprect(int x,int y,int w,int h,unsigned c){
 int top=view_top(),bot=view_bot()+1;
 if(x<0){w+=x;x=0;}
 if(y<top){h-=top-y;y=top;}
 if(x+w>W)w=W-x;
 if(y+h>bot)h=bot-y;
 if(w>0&&h>0)rect(x,y,w,h,c);
}
static void planet_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned col){
 queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),col);
 queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),col);
}
typedef struct {float z,x,w;int kind;unsigned h;} PlanetProp;
static int planet_prop_cmp(const void *a,const void *b){float d=((const PlanetProp*)b)->z-((const PlanetProp*)a)->z;return d>0?1:d<0?-1:0;}
static void draw_tree_billboard(float x,float z,unsigned h,int kind){
 float ground=terrain_height(&game,x,z);if(terrain_is_water(&game,x,z))return;
 float height=kind==1?12.f:kind==2?8.f:26.f+(h%17);
 Vec3 base={x,ground,z},top={x,ground+height,z};
 Vec3 vb=camera(&game,base),vt=camera(&game,top);
 if(vb.z<10||vb.z>780)return;
 Point pb=project(vb),pt=project(vt);
 int s=(int)fmaxf(3,fminf(26,720.f/vb.z));
 int x0=(int)pt.x,y0=(int)pt.y,y1=(int)pb.y;
 if(y1<=y0)y1=y0+s;
 Body *b=&game.bodies[game.planet];
 unsigned leaf=kind==1?RGB(70,130,55):mix_rgb(RGB(36,110,42),b->accent,.18f);
 unsigned dark=mix_rgb(leaf,RGB(12,40,16),.45f);
 unsigned trunk=RGB(92,62,38);
 if(kind==2){
  planet_cliprect(x0-s,y1-s,s*2,s,RGB(110,96,78));
  planet_cliprect(x0-s/2,y1-s/2,s,s/2,RGB(150,130,100));
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
 planet_cliprect(x0-s/2,y0-s/3,s,s/2,mix_rgb(leaf,RGB(200,220,90),.25f));
}
static void draw_life_billboard(const Lifeform *l){
 Vec3 v=camera(&game,l->pos);if(v.z<8||v.z>520)return;
 Point p=project(v);int top=view_top(),bot=view_bot();
 if(p.x<4||p.x>476||p.y<top+4||p.y>bot-4)return;
 int s=(int)fmaxf(4,fminf(18,480.f/v.z));
 unsigned c=l->scanned?CYAN:l->kind==LIFE_FAUNA?RGB(210,170,70):l->kind==LIFE_FLORA?RGB(210,70,130):RGB(210,180,70);
 int kind=l->kind==LIFE_FLORA?0:l->kind==LIFE_FAUNA?1:2;
 field_sprite((int)p.x,(int)p.y,s*2,kind,c,((int)(game.time*4))&1);
}
static void planet_view(void){
 Body *b=&game.bodies[game.planet];
 int top=view_top(),bottom=view_bot()+1;
 unsigned sky_hi=b->type==OCEAN?mix_rgb(b->color,RGB(78,150,220),.55f):mix_rgb(b->color,RGB(210,150,88),.4f);
 unsigned sky_lo=b->type==OCEAN?mix_rgb(b->accent,RGB(186,216,240),.5f):mix_rgb(b->accent,RGB(236,196,130),.45f);
 for(int y=top;y<bottom;y++){
  float t=(y-top)/(float)fmaxf(1,bottom-top);
  unsigned c=mix_rgb(sky_hi,sky_lo,t);
  for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;
 }
 int suny=top+28-(int)(game.pitch*40);if(suny<top+8)suny=top+8;if(suny>bottom-40)suny=bottom-40;
 circle(370,suny,14,mix_rgb(b->color,RGB(255,230,120),.25f));circle(370,suny,8,RGB(255,255,200));
 if(b->type==OCEAN)for(int i=0;i<3;i++){int cx=70+i*90,cy=top+18+(i%2)*10;planet_cliprect(cx,cy,50,8,RGB(230,236,242));planet_cliprect(cx+10,cy-6,34,8,RGB(242,246,250));}
 else for(int i=0;i<2;i++){int cx=90+i*140,cy=top+16+(i%2)*8;planet_cliprect(cx,cy,42,4,mix_rgb(b->accent,RGB(228,186,140),.35f));}
 Vec3 pad=surface_site(&game,1);
 unsigned grass=b->type==OCEAN?mix_rgb(b->color,RGB(74,140,68),.5f):mix_rgb(b->color,RGB(90,130,60),.62f);
 unsigned waterc=mix_rgb(b->color,RGB(52,118,168),.4f);
 int horizon=110+(int)(game.pitch*150.f);
 if(horizon<top+24)horizon=top+24;
 if(horizon>bottom-36)horizon=bottom-36;
 for(int y=horizon;y<bottom;y++){
  float t=(y-horizon)/(float)fmaxf(1,bottom-horizon);
  unsigned c=b->type==OCEAN&&t<.12f?waterc:mix_rgb(grass,mix_rgb(b->accent,RGB(48,78,40),.4f),t*.45f);
  for(int x=0;x<W;x++)fb[y*STRIDE+x]=c;
 }
 int cell=game.surface==2?40:56,span=5;
 int gx0=(int)floorf(game.pos.x/cell)-span,gz0=(int)floorf(game.pos.z/cell)-span;
 for(int iz=0;iz<span*2+1;iz++)for(int ix=0;ix<span*2+1;ix++){
  float x0=(gx0+ix)*(float)cell,z0=(gz0+iz)*(float)cell,x1=x0+cell,z1=z0+cell;
  float cx=x0+cell*.5f,cz=z0+cell*.5f;
  Vec3 mid=camera(&game,(Vec3){cx,24.f,cz});if(mid.z<20||mid.z>620)continue;
  if(terrain_is_water(&game,cx,cz))continue;
  int checker=((gx0+ix)+(gz0+iz))&1;
  unsigned col=checker?grass:mix_rgb(grass,b->accent,.28f);
  planet_quad((Vec3){x0,24,z0},(Vec3){x1,24,z0},(Vec3){x1,24,z1},(Vec3){x0,24,z1},col);
 }
 {
  float px=pad.x,pz=pad.z,h=24.6f,s=40.f;
  unsigned slab=RGB(118,214,220),edge=RGB(36,92,104),mark=RGB(248,252,236);
  planet_quad((Vec3){px-s-5,h,pz-s-5},(Vec3){px+s+5,h,pz-s-5},(Vec3){px+s+5,h,pz+s+5},(Vec3){px-s-5,h,pz+s+5},edge);
  planet_quad((Vec3){px-s,h+.2f,pz-s},(Vec3){px+s,h+.2f,pz-s},(Vec3){px+s,h+.2f,pz+s},(Vec3){px-s,h+.2f,pz+s},slab);
  planet_quad((Vec3){px-3,h+.4f,pz-24},(Vec3){px+3,h+.4f,pz-24},(Vec3){px+3,h+.4f,pz+24},(Vec3){px-3,h+.4f,pz+24},mark);
  planet_quad((Vec3){px-24,h+.4f,pz-3},(Vec3){px+24,h+.4f,pz-3},(Vec3){px+24,h+.4f,pz+3},(Vec3){px-24,h+.4f,pz+3},mark);
  if(game.surface==0){
   Vec3 a=camera(&game,(Vec3){px,h+.6f,pz}),b=camera(&game,(Vec3){px,92.f,pz});
   if(a.z>12&&b.z>12){Point p=project(a),q=project(b);if(p.y>top&&p.y<bottom&&q.y>top&&q.y<bottom){line((int)p.x,(int)p.y,(int)q.x,(int)q.y,CYAN);line((int)p.x+1,(int)p.y,(int)q.x+1,(int)q.y,WHITE);}}
  }
 }
 flush_meshes();
 PlanetProp prop[96];int nprop=0;
 unsigned seed=b->seed;
 float px=pad.x,pz=pad.z;
 for(int i=0;i<36&&nprop<80;i++){
  unsigned h=planet_hash(seed+i*7919u);float a=(h%6283)*.001f,d=130.f+(h%240);
  float x=px+cosf(a)*d,z=pz+sinf(a)*d;
  if(terrain_is_water(&game,x,z))continue;
  if(fabsf(x-px)<78&&fabsf(z-pz)<78)continue;
  Vec3 v=camera(&game,(Vec3){x,terrain_height(&game,x,z),z});if(v.z<12||v.z>760)continue;
  prop[nprop++]=(PlanetProp){v.z,x,z,0,h};
 }
 for(int i=0;i<22&&nprop<90;i++){
  unsigned h=planet_hash(seed+i*13007u+17);float a=(h%6283)*.001f,d=90.f+(h%160);
  float x=px+cosf(a)*d,z=pz+sinf(a)*d;
  if(terrain_is_water(&game,x,z)||(fabsf(x-px)<70&&fabsf(z-pz)<70))continue;
  Vec3 v=camera(&game,(Vec3){x,terrain_height(&game,x,z),z});if(v.z<10||v.z>520)continue;
  prop[nprop++]=(PlanetProp){v.z,x,z,1,h};
 }
 for(int i=0;i<12&&nprop<96;i++){
  unsigned h=planet_hash(seed+i*3571u+91);float a=(h%6283)*.001f,d=150.f+(h%280);
  float x=px+cosf(a)*d,z=pz+sinf(a)*d;
  if(terrain_is_water(&game,x,z)||(fabsf(x-px)<80&&fabsf(z-pz)<80))continue;
  Vec3 v=camera(&game,(Vec3){x,terrain_height(&game,x,z),z});if(v.z<10||v.z>560)continue;
  prop[nprop++]=(PlanetProp){v.z,x,z,2,h};
 }
 if(nprop>1)qsort(prop,nprop,sizeof(prop[0]),planet_prop_cmp);
 for(int i=0;i<nprop;i++)draw_tree_billboard(prop[i].x,prop[i].w,prop[i].h,prop[i].kind);
 /* Settlement hut on the pad edge. */
 {
  float hx=px+48,hz=pz-20,hh=terrain_height(&game,hx,hz);
  Vec3 A={hx-14,hh,hz-10},B={hx+14,hh,hz-10},C={hx+14,hh,hz+10},D0={hx-14,hh,hz+10};
  Vec3 E={hx-14,hh+16,hz-10},F={hx+14,hh+16,hz-10},G={hx+14,hh+16,hz+10},I={hx-14,hh+16,hz+10};
  unsigned wall=mix_rgb(b->accent,RGB(150,110,70),.35f),roof=mix_rgb(b->color,RGB(170,70,50),.4f);
  planet_quad(A,B,F,E,wall);planet_quad(B,C,G,F,mix_rgb(wall,RGB(40,30,20),.3f));planet_quad(E,F,G,I,roof);
  (void)D0;
  flush_meshes();
 }
 if(game.surface)shipwire_stretched(mesh_id(player_ships[game.ship].name),game.surface==1?game.pos:game.ship_pos,game.surface==1?game.yaw:0,.85f,1.f,GOLD);
 for(int i=0;i<LIFE_COUNT;i++)if(game.life[i].alive)draw_life_billboard(&game.life[i]);
 if(game.surface!=2){line(227,110,236,110,CYAN);line(244,110,253,110,CYAN);line(240,97,240,106,CYAN);line(240,114,240,123,CYAN);}
 if(game.surface==2){
  rect(0,top,W,18,RGB(6,16,24));rect(0,top+17,W,1,CYAN);
  text(1,top/8,CYAN,"EVA / %.14s",b->name);text(28,top/8,GOLD,"SQ SCAN");text(40,top/8,WHITE,"O BOARD");
 }else if(hud_mode==1){float alt=game.pos.y-terrain_height(&game,game.pos.x,game.pos.z);text(1,1,CYAN,"%s",b->name);text(36,1,WHITE,game.surface==2?"EVA":"ALT %d",(int)alt);}
}
