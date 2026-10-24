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
typedef struct {float yaw,tilt,zoom,dx,dy;} MenuStationCamera;
/* A bounded cinematic camera; ordinary menu previews retain their own reel. */
static int menu_station_arrival_camera=0;
static MenuStationCamera menu_station_arrival;
static float menu_station_ease(float t){return t*t*(3.f-2.f*t);}
static MenuStationCamera menu_station_camera(int kind,float phase){
 /* Five joined compositions make a continuous 96-second reel.  Capitals use
    wider establishing shots and closer offset passes; rich hubs favour tidy,
    sun-facing three-quarter views. */
 static const MenuStationCamera rich[5]={{.35f,-.30f,4.0f,0,0},{1.45f,-.18f,4.35f,-7,2},{2.75f,-.40f,3.9f,8,-1},{4.25f,-.24f,4.25f,-5,1},{6.6331853f,-.30f,4.0f,0,0}};
 static const MenuStationCamera mega[5]={{.15f,-.27f,1.85f,0,1},{1.25f,-.13f,2.75f,-18,4},{2.65f,-.38f,2.15f,12,-2},{4.15f,-.20f,2.9f,-14,3},{6.4331853f,-.27f,1.85f,0,1}};
 const MenuStationCamera *a=kind==STATION_MEGA?mega:rich;float q=fmodf(phase,6.283185307f)*(4.f/6.283185307f);int shot=(int)q;if(shot<0)shot=0;if(shot>3)shot=3;float t=menu_station_ease(q-shot),u=1.f-t;
 return (MenuStationCamera){a[shot].yaw*u+a[shot+1].yaw*t,a[shot].tilt*u+a[shot+1].tilt*t,a[shot].zoom*u+a[shot+1].zoom*t,a[shot].dx*u+a[shot+1].dx*t,a[shot].dy*u+a[shot+1].dy*t};
}
static Vec3 menu_station_point(Vec3 p,Vec3 centre,float yaw,float tilt,float scale,float dx,float dy){return add((Vec3){dx*320.f/240.f,dy*320.f/240.f,320},mul(menu_hull_rotate(sub(p,centre),yaw,tilt,0),scale));}
/* Continue the lens push once dolly travel reaches its safe distance. Apply
 * only after visibility/lighting: this changes the lens, not hull normals. */
static Vec3 menu_station_lens(Vec3 p,float lens){p.x*=lens;p.y*=lens;return p;}
/* Use the same authored per-panel atlas as flight, with explicit local UVs.
 * Preview cameras must not reconstruct world UVs from the player's live pose. */
typedef struct {float u[3],v[3],light;short block,face;} MenuStationTexture;
static MenuStationTexture menu_station_texture[4096];
static void menu_station_texture_queue(Vec3 a,Vec3 b,Vec3 c,Vec3 la,Vec3 lb,Vec3 lc,int id,int side,unsigned ink,float light){
 int first=drawcount;queue_triangle(a,b,c,ink);if(drawcount!=first+1||first>=4096)return;
 MenuStationTexture *m=&menu_station_texture[first];const MegaBlock *block=&mc_draw_city->b[id];Vec3 p[3]={sub(la,block->c),sub(lb,block->c),sub(lc,block->c)};
 for(int i=0;i<3;i++){m->u[i]=side<4?p[i].x:p[i].z;m->v[i]=side==2||side==3?p[i].z:p[i].y;}
 m->block=id;m->face=side;m->light=light;drawlist[first].material=30000+first;
}
static void menu_station_textured_triangle(DrawTri *t){
 MenuStationTexture *m=&menu_station_texture[t->material-30000];const MegaBlock*b=&mc_draw_city->b[m->block];
 station_art_face_prepare(b,m->face);Point *p=t->p;float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1-1:view_bot();
 int y0=fmaxf(top,floorf(fminf(p[0].y,fminf(p[1].y,p[2].y)))),y1=fminf(bot,ceilf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))));
 float iz[3],u[3],v[3];for(int i=0;i<3;i++){iz[i]=1/(area*p[i].z);u[i]=m->u[i]*iz[i];v[i]=m->v[i]*iz[i];}
 float dx[3]={p[2].y-p[1].y,p[0].y-p[2].y,p[1].y-p[0].y};
 float di=dx[0]*iz[0]+dx[1]*iz[1]+dx[2]*iz[2],du=dx[0]*u[0]+dx[1]*u[1]+dx[2]*u[2],dv=dx[0]*v[0]+dx[1]*v[1]+dx[2]*v[2];
 int light=(int)(m->light*256);unsigned tint=game.bodies[0].color;
 for(int y=y0;y<=y1;y++){
  float yy=y+.5f,left=1e9f,right=-1e9f;
  for(int k=0;k<3;k++){Point a=p[k],b=p[(k+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*(b.x-a.x)/(b.y-a.y);left=fminf(left,x);right=fmaxf(right,x);}}
  if(left>right)continue;int x0=fmaxf(clipx0,ceilf(left-.5f)),x1=fminf(clipx1-1,floorf(right-.5f));
  float aa=edge(p[1],p[2],x0+.5f,yy),bb=edge(p[2],p[0],x0+.5f,yy),cc=edge(p[0],p[1],x0+.5f,yy);
  float inv=aa*iz[0]+bb*iz[1]+cc*iz[2],uq=aa*u[0]+bb*u[1]+cc*u[2],vq=aa*v[0]+bb*v[1]+cc*v[2];
  station_art_footprint=0;
  for(int x=x0;x<=x1;x++,inv+=di,uq+=du,vq+=dv){if(inv<=0)continue;float z=1/inv;unsigned c=t->color;
   if(station_art_face_texel(uq*z,vq*z,320,&c)){
    int r=(c&255)*light/256,g=((c>>8)&255)*light/256,blue=((c>>16)&255)*light/256;
    if(menu_station_arrival_camera){r+=(tint&255)*(light-96)/2304;g+=((tint>>8)&255)*(light-96)/2304;blue+=((tint>>16)&255)*(light-96)/2304;}
    c=RGB(r>255?255:r,g>255?255:g,blue>255?255:blue);
   }fb[y*STRIDE+x]=c;
  }
 }
}
static void menu_station_generated(int x,int y,int w,int h,float phase){
 const MegaCity *m=station_architecture_for(&game);StationProfile profile=station_profile_for(&game,0);
 const MegaCity *saved_city=mc_draw_city;StationProfile saved_profile=mc_profile;mc_draw_city=m;mc_profile=profile;dg_station_prepare(norm((Vec3){-.31f,.72f,-.62f}));station_art_prepare();station_art_last=-1;
 Vec3 lo=m->b[0].c,hi=lo;for(int i=0;i<m->n;i++){const MegaBlock *b=&m->b[i];lo.x=fminf(lo.x,b->c.x-b->e.x);lo.y=fminf(lo.y,b->c.y-b->e.y);lo.z=fminf(lo.z,b->c.z-b->e.z);hi.x=fmaxf(hi.x,b->c.x+b->e.x);hi.y=fmaxf(hi.y,b->c.y+b->e.y);hi.z=fmaxf(hi.z,b->c.z+b->e.z);}
 Vec3 centre=mul(add(lo,hi),.5f);float radius=1;for(int i=0;i<m->n;i++){const MegaBlock *b=&m->b[i];radius=fmaxf(radius,length(sub(add(b->c,b->e),centre)));radius=fmaxf(radius,length(sub(sub(b->c,b->e),centre)));}
 float screen_radius=fminf(w*.43f,(h-14)*.48f),scale=screen_radius*320.f/(radius*(240.f+screen_radius));
 /* Decorative antennae and remote blocks widen the architecture bounds.  Give
    ordinary hubs a closer beauty shot, while keeping megacities far enough
    back for their larger silhouette to remain readable in the PSP viewport. */
 MenuStationCamera camera=profile.station_class==STATION_POOR?(MenuStationCamera){phase+.55f,-.30f,4.f,0,0}:menu_station_camera(profile.station_class,phase);scale*=camera.zoom;
 float lens=1;
 if(menu_station_arrival_camera){scale/=camera.zoom;camera=menu_station_arrival;float requested=scale*camera.zoom;float bound=fmaxf(radius,length(sub(hi,lo))*.5f);scale=fminf(requested,245.f/bound);lens=requested/scale;}
 float yaw=camera.yaw,tilt=camera.tilt;
 for(int k=0;k<m->n;k++){
  const MegaBlock *b=&m->b[k];const MegaGeometry *g=mega_geometry_for(b->shape);Vec3 v[MC_VERTEX_MAX],local[MC_VERTEX_MAX];
  for(int i=0;i<g->vertices;i++){local[i]=(Vec3){b->c.x+g->v[i].x*b->e.x,b->c.y+g->v[i].y*b->e.y,b->c.z+g->v[i].z*b->e.z};v[i]=menu_station_point(local[i],centre,yaw,tilt,scale,camera.dx,camera.dy);}
  for(int i=0;i<g->faces;i++){
   const MegaFace *f=&g->f[i];Vec3 n=menu_hull_rotate((Vec3){f->normal.x/b->e.x,f->normal.y/b->e.y,f->normal.z/b->e.z},yaw,tilt,0);Vec3 mid=mul(add(add(v[f->v[0]],v[f->v[1]]),v[f->v[2]]),1.f/3);
   if(dot(n,mul(mid,-1))<=0)continue;float diffuse=fmaxf(0,dot(norm(n),menu_station_arrival_camera?(Vec3){.32f,.25f,.92f}:(Vec3){-.31f,.72f,-.62f}));float light=.38f+.62f*diffuse;unsigned base=b->kind==4?profile.trim:profile.hull;
   if(menu_station_arrival_camera)base=mix_rgb(base,mix_rgb(game.bodies[0].color,RGB(255,221,150),.35f),diffuse*.45f);
   if(b->kind==3)base=profile.light;else if(b->district&1)base=livery_tint(base,8);unsigned ink=RGB((int)((base&255)*light),(int)(((base>>8)&255)*light),(int)(((base>>16)&255)*light));
   menu_station_texture_queue(menu_station_lens(v[f->v[0]],lens),menu_station_lens(v[f->v[1]],lens),menu_station_lens(v[f->v[2]],lens),local[f->v[0]],local[f->v[1]],local[f->v[2]],k,f->side,ink,light);if(f->count==4)menu_station_texture_queue(menu_station_lens(v[f->v[0]],lens),menu_station_lens(v[f->v[2]],lens),menu_station_lens(v[f->v[3]],lens),local[f->v[0]],local[f->v[2]],local[f->v[3]],k,f->side,ink,light);
  }
 }
 flush_meshes();mc_draw_city=saved_city;mc_profile=saved_profile;station_art_last=-1;
}
static void menu_station_relay(int w,int h,float phase){
 StationProfile profile=station_profile_for(&game,game.station_variant);int id=mesh_id("CORIOLIS");const Mesh *m=&meshes[id];Vec3 centre;float scale;menu_hull_layout(id,h,&centre,&scale);scale*=.92f;
 float yaw=phase+game.station_variant*1.7f,tilt=-.30f,stretch=profile.half/profile.radius,lens=1;
 if(menu_station_arrival_camera){float radius=1;for(int i=0;i<m->vertices;i++){Vec3 v=sub(m->v[i],centre);v.z*=stretch;radius=fmaxf(radius,length(v));}float requested=(h*.35f)*320.f/(radius*(240.f+h*.35f))*menu_station_arrival.zoom;scale=fminf(requested,245.f/radius);lens=requested/scale;yaw=menu_station_arrival.yaw;tilt=menu_station_arrival.tilt;}
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];Vec3 p[3];int at[3]={t->a,t->b,t->c};for(int q=0;q<3;q++){Vec3 v=m->v[at[q]];v.z*=stretch;p[q]=menu_station_point(v,centre,yaw,tilt,scale,0,0);}Vec3 n=menu_hull_rotate(t->normal,yaw,tilt,0);if(dot(n,mul(add(add(p[0],p[1]),p[2]),-1.f/3))<=0)continue;float light=.35f+.65f*fmaxf(0,dot(norm(n),(Vec3){-.31f,.72f,-.62f}));unsigned ink=RGB((int)((profile.hull&255)*light),(int)(((profile.hull>>8)&255)*light),(int)(((profile.hull>>16)&255)*light));queue_triangle(menu_station_lens(p[0],lens),menu_station_lens(p[1],lens),menu_station_lens(p[2],lens),ink);}flush_meshes();(void)w;
}
static void menu_station_sun(int x,int y,int w,int h,float phase,int kind,unsigned sky){
 if(kind==STATION_POOR)return;float travel=.5f+.5f*sinf(phase*.62f+(kind==STATION_MEGA?.8f:0));int sx=x+12+(int)(travel*(w-35)),sy=y+10+(int)(5*sinf(phase*.43f));unsigned sun=mix_rgb(game.bodies[0].color,RGB(255,224,160),.42f);
 for(int r=12;r>=3;r-=3){unsigned glow=mix_rgb(sky,sun,(13-r)*.035f);for(int yy=-r;yy<=r;yy+=2)for(int xx=-r;xx<=r;xx+=2)if(xx*xx+yy*yy<=r*r)pixel(sx+xx,sy+yy,glow);}
 for(int yy=-3;yy<=3;yy++)for(int xx=-3;xx<=3;xx++)if(xx*xx+yy*yy<=9)pixel(sx+xx,sy+yy,sun);
 int cy=y+h/2;for(int k=1;k<=3;k++){int fx=sx+(x+w/2-sx)*k/4,fy=sy+(cy-sy)*k/4;unsigned c=mix_rgb(sky,k&1?RGB(104,142,147):RGB(196,126,72),.24f);for(int yy=-1;yy<=1;yy++)for(int xx=-2;xx<=2;xx++)if(xx*xx+yy*yy<=4)pixel(fx+xx,fy+yy,c);}
}
static void menu_station_view(int x,int y,int w,int h){
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cx1=clipx1,cy0=clipy0,cy1=clipy1;float phase=fmodf(fmaxf(0,preview_time),96.f)*(6.283185307f/96.f);unsigned sky=RGB(7,12,22);rect(x,y,w,h,sky);preview_clip(x+w/2,y+h/2,x+1,y+1,x+w-1,y+h-1);
 for(int i=0;i<25;i++){unsigned s=sun_hash(game.bodies[0].seed+(unsigned)i*6151u);pixel(x+3+(s%(unsigned)(w-6)),y+3+((s>>9)%(unsigned)(h-6)),i%5?RGB(54,68,84):RGB(151,163,176));}
 menu_station_sun(x,y,w,h,phase,station_profile_for(&game,game.station_variant).station_class,sky);
 if(game.station_variant==0)menu_station_generated(x,y,w,h,phase);else menu_station_relay(w,h,phase);
 preview_clip(ox,oy,cx0,cy0,cx1,cy1);rect(x,y,w,1,UI_EDGE);rect(x,y+h-1,w,1,UI_RAISED);rect(x,y,1,h,UI_EDGE);rect(x+w-1,y,1,h,UI_RAISED);
 rect(x+4,y+h-14,w-8,11,mix_rgb(sky,UI_PANEL,.72f));text_px(x+8,y+h-12,UI_TEXT,"%.24s",station_name(&game));
}
static void menu_space_view(int x,int y,int w,int h){
 if(w<32||h<32)return;
 if(game.docked){menu_station_view(x,y,w,h);return;}
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
 int detail_panels=0;
 for(int i=0;i<m->triangles;i++){
  const MeshTri *t=&m->t[i];
  Vec3 va=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[t->a],centre),yaw,tilt,bank),scale));
  Vec3 vb=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[t->b],centre),yaw,tilt,bank),scale));
  Vec3 vc=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[t->c],centre),yaw,tilt,bank),scale));
  Vec3 n=menu_hull_rotate(t->normal,yaw,tilt,bank);
  if(dot(n,mul(add(add(va,vb),vc),-1.f/3))<=0)continue;
  float light=.3f+.7f*fmaxf(0,dot(n,(Vec3){-.309426f,.721995f,-.618853f}));
  unsigned paint=ship_paint[ship];
  unsigned ink=RGB((int)((paint&255)*light),(int)(((paint>>8)&255)*light),(int)(((paint>>16)&255)*light));
  if(n.y>.42f)ink=livery_tint(ink,22);else if(n.y<-.35f)ink=livery_tint(ink,-20);else if((i&7)==0)ink=livery_tint(ink,12);
  if(detail_panels<4&&hull_detail_candidate(id,i,t)){
   queue_hull_panel(va,vb,vc,(Vec3){0,0,320},ink,t->normal.z>.2f&&detail_panels==0);detail_panels++;
  }else queue_triangle(va,vb,vc,ink);
 }
 flush_meshes();
 /* Restore caller render state, not generic full-screen defaults. */
 preview_clip(ox,oy,cx0,cy0,cx1,cy1);
 rect(x,y,w,1,RGB(193,139,77));rect(x,y+h-1,w,1,RGB(41,54,70));
 rect(x,y,1,h,RGB(90,96,76));rect(x+w-1,y,1,h,RGB(41,54,70));
}
