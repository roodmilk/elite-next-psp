/* Presentation-only 96-second loop. No simulation writes, heap or texture cache.
 * Own camera-space geometry; never call the live world/camera renderers here. */
#define MENU_BEAUTY_PREVIEW 1
static Vec3 menu_hull_rotate(Vec3 v,float yaw,float tilt,float bank){
 v=rotate(v,yaw,bank);
 float c=cosf(tilt),s=sinf(tilt);
 return (Vec3){v.x,v.y*c-v.z*s,v.y*s+v.z*c};
}
static void menu_hull_layout(int id,int height,Vec3 *centre,float *scale){
 const Mesh *m=&meshes[id];Vec3 lo=m->v[0],hi=lo;
 for(int i=1;i<m->vertices;i++){
  Vec3 v=m->v[i];
  lo.x=fminf(lo.x,v.x);lo.y=fminf(lo.y,v.y);lo.z=fminf(lo.z,v.z);
  hi.x=fmaxf(hi.x,v.x);hi.y=fmaxf(hi.y,v.y);hi.z=fmaxf(hi.z,v.z);
 }
 *centre=mul(add(lo,hi),.5f);float radius=1;
 for(int i=0;i<m->vertices;i++)radius=fmaxf(radius,length(sub(m->v[i],*centre)));
 float screen_radius=fminf(36.f,(height-16)*.5f);
 *scale=screen_radius*320.f/(radius*(240.f+screen_radius));
}
static void menu_space_view(int x,int y,int w,int h){
 if(w<32||h<32)return;
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cx1=clipx1,cy0=clipy0,cy1=clipy1;
 /* Called with an empty mesh queue by the menu; do not flush a caller's work. */
 if(drawcount)return;
 float phase=fmodf(fmaxf(0,preview_time),96.f)*(6.283185307f/96.f);
 unsigned seed=game.bodies[0].seed;
 unsigned sky=mix_rgb(RGB(8,13,24),RGB(27,18,35),.5f+.5f*sinf(phase));
 rect(x,y,w,h,sky);
 preview_clip(x+w/2,y+h/2+3,x+1,y+1,x+w-1,y+h-1);
 /* Broad quiet colour wash, clipped explicitly because rect is screen-wide. */
 if(!high_contrast)for(int band=0;band<5;band++){
  int yy=y+1+band*(h-2)/5,hh=(y+1+(band+1)*(h-2)/5)-yy;
  rect(x+1,yy,w-2,hh,mix_rgb(sky,RGB(24,32,46),band*.09f));
 }
 for(int i=0;i<22;i++){
  unsigned s=sun_hash(seed+(unsigned)i*7919u);
  int sx=x+3+(int)(s%(unsigned)(w-6))+(int)(sinf(phase)*3);
  int sy=y+3+(int)((s>>9)%(unsigned)(h-6))+(int)(cosf(phase)*2);
  pixel(sx,sy,i%6?RGB(61,73,89):RGB(132,139,151));
 }
 /* Bodies never grow toward the viewer: only restrained distant parallax. */
 for(int i=0;i<2;i++){
  const Body *b=&game.bodies[1+i];
  float a=phase+i*2.6f;
  int px=x+w/2+(int)(sinf(a+1.2f)*w*.42f);
  int py=y+18+i*8+(int)(cosf(a)*5);
  draw_planet_sprite(px,py,i?10:17,b->seed,b->type,x+2,y+2,x+w-2,y+h-2);
 }
 {
  int sx=x+w/2+(int)(cosf(phase+.5f)*w*.42f),sy=y+12+(int)(sinf(phase)*4);
  unsigned warm=mix_rgb(game.bodies[0].color,RGB(229,210,163),.45f+.15f*sinf(phase*3));
  /* Steady distant disc: no bloom, pulsing, flare or animation flicker. */
  for(int yy=-5;yy<=5;yy++)for(int xx=-5;xx<=5;xx++)
   if(xx*xx+yy*yy<=25)pixel(sx+xx,sy+yy,yy<0?warm:mix_rgb(warm,RGB(193,139,77),.25f));
 }
 int ship=game.ship;
 if(ship<0||ship>=player_ship_count)ship=0;
 int id=mesh_id(player_ships[ship].name);const Mesh *m=&meshes[id];
 Vec3 centre;float scale;menu_hull_layout(id,h,&centre,&scale);
 float yaw=.8f+.65f*sinf(phase),tilt=-.48f-.08f*sinf(phase*2),bank=.04f*sinf(phase);
 /* Aft glow is attached to the real mesh's aft extent. Draw behind hull. */
 float aft=m->v[0].z;for(int i=1;i<m->vertices;i++)aft=fminf(aft,m->v[i].z);
 Vec3 root={centre.x,centre.y,aft},tail={centre.x,centre.y,aft-12.f/scale};
 root=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(root,centre),yaw,tilt,bank),scale));
 tail=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(tail,centre),yaw,tilt,bank),scale));
 Point a=project(root),b=project(tail);
 line((int)a.x,(int)a.y,(int)b.x,(int)b.y,RGB(72,108,121));
 pixel((int)a.x,(int)a.y,RGB(166,204,205));
 for(int i=0;i<m->triangles;i++){
  const MeshTri *t=&m->t[i];
  Vec3 va=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[t->a],centre),yaw,tilt,bank),scale));
  Vec3 vb=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[t->b],centre),yaw,tilt,bank),scale));
  Vec3 vc=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[t->c],centre),yaw,tilt,bank),scale));
  Vec3 n=menu_hull_rotate(t->normal,yaw,tilt,bank);
  if(dot(n,mul(add(add(va,vb),vc),-1.f/3))<=0)continue;
  float light=.3f+.7f*fmaxf(0,dot(n,(Vec3){-.309426f,.721995f,-.618853f}));
  unsigned ink=RGB((int)(193*light),(int)(139*light),(int)(77*light));
  if(n.y>.42f)ink=livery_tint(ink,22);else if(n.y<-.35f)ink=livery_tint(ink,-20);else if((i&7)==0)ink=livery_tint(ink,12);
  queue_triangle(va,vb,vc,ink);
 }
 flush_meshes();
 /* Restore caller render state, not generic full-screen defaults. */
 preview_clip(ox,oy,cx0,cy0,cx1,cy1);
 rect(x,y,w,1,RGB(193,139,77));rect(x,y+h-1,w,1,RGB(41,54,70));
 rect(x,y,1,h,RGB(90,96,76));rect(x+w-1,y,1,h,RGB(41,54,70));
}
