/* World-attached facade art, evaluated only for visible, depth-tested pixels.
 * 48 KiB palette-indexed animated screens; no textures loaded during flight. */
static unsigned char mc_ad[8][96][64];static int mc_ad_tick=-1,mc_ad_ready;
static int mc_depth_active,mc_rich;
static int mc_old_min,mc_old_max;static float mc_old_light;static int mc_legacy_review;
static unsigned mc_edge[MEGA_BLOCK_MAX][6],mc_dim[MEGA_BLOCK_MAX][6];
static Vec3 mc_axis[3];
static Vec3 mc_eye;
static const MegaCity *mc_draw_city;
static unsigned mc_palette[8][5];
static void mc_ad_text(int d,int x,int y,const char *s,int color){
 for(int k=0;s[k];k++)for(int j=0;j<8;j++)for(int i=0;i<8;i++){
  int xx=x+k*8+i,yy=y+j;unsigned char c=s[k];
  if(xx>=0&&xx<64&&yy>=0&&yy<96&&c>=32&&c<=126&&(font8[c-32][j]&(128>>i)))mc_ad[d][yy][xx]=color;
 }
}
static void mc_ads_prepare(void){
 int tick=(int)(game.time*30);if(tick==mc_ad_tick)return;mc_ad_tick=tick;
 static const char *brands[]={"NOVA","KOYO","ORBIT","SLEEP","DRIFT","VOX","AZURE","KOBRA"};
 static const char *lines[]={"CASINO","NOODLES","OFFWORLD","HOTEL","ENGINES","RADIO","OXYGEN","SHIPYARD"};
 for(int d=0;d<8;d++){
  if((mc_ad_ready&(1<<d))&&d!=(tick&7))continue;
  mc_ad_ready|=1<<d;
  unsigned accent=d%3==0?RGB(255,79,151):d%3==1?RGB(52,216,230):RGB(250,176,70);
  mc_palette[d][0]=RGB(9,15,31);mc_palette[d][1]=accent;mc_palette[d][2]=RGB(228,237,225);mc_palette[d][3]=livery_tint(accent,-90);mc_palette[d][4]=RGB(44,37,80);
  memset(mc_ad[d],0,sizeof(mc_ad[d]));
  for(int y=0;y<96;y++)for(int x=0;x<64;x++){
   int c=0;if(x<2||x>61||y<2||y>93)c=3;
   int xx=x-32,yy=y-44;
   if(d%4==0){ /* slowly moving globe and orbital band */
    int rr=xx*xx+yy*yy;if(rr<390){c=3;if(((x+tick/2)/5+(y/7))%5==0)c=1;if(xx<-8)c=2;}
    if(abs(yy*3+xx)<5&&abs(xx)<29)c=1;
   }else if(d%4==1){ /* enormous synth-person / visor advertisement */
    if(abs(xx)<15&&yy>-21&&yy<18)c=3;
    if(abs(xx)<18&&yy>-15&&yy<-6)c=1;
    if(abs(xx)<12&&yy==-2)c=2;
    if(abs(xx)<5&&yy>8&&yy<11)c=1;
    if(yy>20&&yy<31&&abs(xx)<24)c=3;
   }else if(d%4==2){ /* launch silhouette with flowing exhaust */
    if(yy>-22&&yy<18&&abs(xx)<(yy+24)/2)c=2;
    if(yy>17&&yy<34&&abs(xx)<5&&((y+tick)%7)<5)c=1;
    if(abs(xx)>19&&abs(xx)<22&&((y+tick)/6)%3==0)c=3;
   }else { /* neon eclipse */
    int rr=xx*xx+yy*yy;if(rr>220&&rr<390)c=1;
    if((xx+7)*(xx+7)+(yy-6)*(yy-6)<290)c=0;
    if(yy>21&&yy<28&&abs(xx)<25)c=3;
   }
   if(c==3&&(y+tick/2+d*7)%29==0)c=1;
   mc_ad[d][y][x]=c;
  }
  mc_ad_text(d,32-(int)strlen(brands[d])*4,7,brands[d],2);
  mc_ad_text(d,32-(int)strlen(lines[d])*4,80,lines[d],1);
  for(int x=6;x<58;x++)mc_ad[d][91][x]=(x<(tick+d*9)%52+6)?1:4;
 }
}
static void mega_city_depth_begin(void){
 mc_depth_active=!mc_legacy_review&&clipy0<0&&game.planet<0;
 if(!mc_depth_active)return;
 mc_old_min=surface_y_min;mc_old_max=surface_y_max;mc_old_light=surface_light;
 surface_y_min=view_top();surface_y_max=view_bot();surface_light=1;surface_material=0;
 memset(surface_depth+surface_y_min*W,255,(surface_y_max-surface_y_min+1)*W*sizeof(*surface_depth));surface_depth_on=1;mega_depth_encoding=1;
 float angle=station_angle(&game);
 Vec3 ax=camera(&game,add(game.pos,rotate((Vec3){1,0,0},0,angle))),ay=camera(&game,add(game.pos,rotate((Vec3){0,1,0},0,angle))),az=camera(&game,add(game.pos,(Vec3){0,0,1}));
 mc_axis[0]=ax;mc_axis[1]=ay;mc_axis[2]=az;
 mc_ads_prepare();
}
static void mega_city_depth_end(void){
 if(!mc_depth_active)return;surface_depth_on=0;surface_material=0;surface_y_min=mc_old_min;surface_y_max=mc_old_max;surface_light=mc_old_light;mc_depth_active=0;mega_depth_encoding=0;mega_depth_valid=1;
}
static void mega_city_capture_depth(void){
 if(!mc_depth_active)return;flush_meshes();
 for(int y=0;y<136;y++)for(int x=0;x<240;x++)mega_occlusion[y*240+x]=(y*2>=surface_y_min&&y*2<=surface_y_max)?surface_depth[y*2*W+x*2]:65535;
 mega_depth_valid=1;
}
static inline unsigned mc_texel(const MegaBlock *b,int face,float u,float v,float z,unsigned base,unsigned edge_ink,unsigned dim){
 float eu=face<4?b->e.x:b->e.z,ev=face==2||face==3?b->e.z:b->e.y;
 if(!b->shape&&(b->kind==1||b->kind==5)&&face<2&&fabsf(u)<eu*.79f&&fabsf(v)<ev*.80f&&z<18000){
  int tx=(int)((u/(eu*.8f)+1)*32),ty=(int)((1-v/(ev*.81f))*48);
  if(tx>=0&&tx<64&&ty>=0&&ty<96){int ci=mc_ad[b->district][ty][tx];return mc_palette[b->district][ci];}
 }
 /* Distance-aware floor ribbons replace high-frequency windows far away. */
 int vv=(int)(v+ev+16384)-16384,uu=(int)(u+eu+16384)-16384;
 int iy=vv>>6,ix=uu>>6,fy=vv&63,fx=uu&63;
 if(b->kind==7){ /* Deep blue segmented photovoltaic/radiator plates. */
  return fx<5||fy<5?RGB(66,110,156):RGB(13,34,68);
 }
 if(mc_rich&&b->shape!=MC_DOME&&b->kind!=4&&b->kind!=3){
  int deck=vv&255;
  if(deck<26)return RGB(23,36,49);
  if(deck<42&&z<14000)return fx<40?RGB(213,171,100):RGB(34,64,80);
  if((uu&255)<6)return RGB(52,67,78);
 }
 if(mc_rich&&b->shape==MC_DOME&&z<10000){
  if(fx<4||fy<4)return RGB(79,169,178);
  if(v<ev*.5f&&((ix*11+iy*17+b->district)&7)<5)return RGB(39+fx/4,83+fy/3,53);
 }
 if(face!=2&&face!=3&&b->kind!=3){
  if(fy<12&&fx>9&&fx<35&&z<12500&&((ix*13+iy*7+b->district*5)&7)<5)return ((ix+iy)%5)?RGB(187,151,98):RGB(53,147,177);
  if(fy<10&&z>=12500&&iy%5==0)return edge_ink;
 }
 if(b->shape){
  if(b->shape==MC_DOME&&fy<5)return RGB(72,161,174);
  if(b->shape==MC_POD&&fx<7)return edge_ink;
  if(fabsf(v)<12&&b->shape==MC_SPHERE)return RGB(63,190,210);
  return base;
 }
 if(fabsf(u)>eu-12||fabsf(v)>ev-12)return edge_ink;
 if((uu&511)<170)return dim;
 return base;
}
/* Separate compact raster loop: interpolate U/Z and V/Z once per span.
 * World-attached pixels stay perspective-correct without reconstructing a
 * complete 3D point or running the planetary terrain shader for each sample. */
static void mega_surface_triangle(DrawTri *t){
 int id=(-t->material-1)/6,face=(-t->material-1)%6;const MegaBlock *block=&mc_draw_city->b[id];
 Point *p=t->p;float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 int ua=face<4?0:2,va=face==2||face==3?2:1;
 float offset[3]={mc_eye.x-block->c.x,mc_eye.y-block->c.y,mc_eye.z-block->c.z};
 float inv[3],uq[3],vq[3];
 for(int i=0;i<3;i++){
  float rx=(p[i].x-proj_ox)/240.f,ry=(proj_oy-p[i].y)/240.f;
  inv[i]=1.f/p[i].z;
  uq[i]=(offset[ua]*inv[i]+mc_axis[ua].x*rx+mc_axis[ua].y*ry+mc_axis[ua].z)/area;
  vq[i]=(offset[va]*inv[i]+mc_axis[va].x*rx+mc_axis[va].y*ry+mc_axis[va].z)/area;
  inv[i]/=area;
 }
 float e0=p[2].y-p[1].y,e1=p[0].y-p[2].y,e2=p[1].y-p[0].y;
 float di=2*(e0*inv[0]+e1*inv[1]+e2*inv[2]),du=2*(e0*uq[0]+e1*uq[1]+e2*uq[2]),dv=2*(e0*vq[0]+e1*vq[1]+e2*vq[2]);
 int first=(int)fmaxf(surface_y_min,floorf(fminf(p[0].y,fminf(p[1].y,p[2].y)))),last=(int)fminf(surface_y_max,ceilf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))));first=(first+1)&~1;
 unsigned edge_ink=mc_edge[id][face],dim=mc_dim[id][face];
 for(int y=first;y<=last;y+=2){
  float yy=y+1,left=1e9f,right=-1e9f;
  for(int k=0;k<3;k++){Point a=p[k],b=p[(k+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*(b.x-a.x)/(b.y-a.y);if(x<left)left=x;if(x>right)right=x;}}
  if(left>right)continue;int x0=(int)fmaxf(0,ceilf(left-1)),x1=(int)fminf(W-1,floorf(right-1));x0=(x0+1)&~1;
  float a=edge(p[1],p[2],x0+1,yy),b=edge(p[2],p[0],x0+1,yy),c=edge(p[0],p[1],x0+1,yy);
  float iz=a*inv[0]+b*inv[1]+c*inv[2],u=a*uq[0]+b*uq[1]+c*uq[2],v=a*vq[0]+b*vq[1]+c*vq[2];
  for(int x=x0;x<=x1;x+=2,iz+=di,u+=du,v+=dv){
   if(iz<=0)continue;float z=1.f/iz;unsigned depth=mega_encode_depth(z);int at=y*W+x,out=y*STRIDE+x;
   if(depth>surface_depth[at]&&depth>surface_depth[at+1]&&(y+1>surface_y_max||(depth>surface_depth[at+W]&&depth>surface_depth[at+W+1])))continue;
   unsigned color=mc_texel(block,face,u*z,v*z,z,t->color,edge_ink,dim);
   for(int dy=0;dy<2&&y+dy<=surface_y_max;dy++){int p0=at+dy*W,o=out+dy*STRIDE;
    if(depth<=surface_depth[p0]){surface_depth[p0]=depth;fb[o]=color;}
    if(depth<=surface_depth[p0+1]){surface_depth[p0+1]=depth;fb[o+1]=color;}
   }
  }
 }
}
static int mc_visible(Vec3 c,Vec3 e){
 Vec3 v=camera(&game,station_vertex_at(c,(Vec3){0,0,STATION_Z},station_angle(&game)));float r=fmaxf(e.x,fmaxf(e.y,e.z))*1.74f;
 return v.z+r>15&&fabsf(v.x)<v.z*1.10f+r*2&&fabsf(v.y)<v.z*.72f+r*2;
}
static void mc_shaped_model(const MegaBlock *b,int id,Vec3 eye){
 const MegaGeometry *m=mega_geometry_for(b->shape);Vec3 view[MC_VERTEX_MAX];
 for(int i=0;i<m->vertices;i++){
  Vec3 v=m->v[i];view[i]=camera(&game,station_vertex_at((Vec3){b->c.x+v.x*b->e.x,b->c.y+v.y*b->e.y,b->c.z+v.z*b->e.z},(Vec3){0,0,STATION_Z},station_angle(&game)));
 }
 Vec3 q=sub(eye,b->c);
 for(int i=0;i<m->faces;i++){
  const MegaFace *f=&m->f[i];Vec3 n={f->normal.x/b->e.x,f->normal.y/b->e.y,f->normal.z/b->e.z};
  if(dot(n,q)<=f->plane)continue;
  n=norm(n);
  unsigned base=b->shape==MC_DOME?RGB(39,95,114):b->shape==MC_SPHERE?RGB(73,107,127):b->shape==MC_POD?RGB(69,58,106):RGB(48+b->district*2,63+b->district*2,82+b->district*2);
  if(station_class(&game)!=STATION_MEGA){StationProfile p=station_profile_for(&game,0);base=b->shape==MC_DOME?livery_tint(p.trim,-35):livery_tint(p.hull,-25);}
  unsigned ink=livery_tint(base,(int)(dot(n,(Vec3){-.4f,.7f,-.5f})*65));
  int side=f->side;mc_edge[id][side]=livery_tint(base,35);mc_dim[id][side]=livery_tint(base,-8);
  surface_material=mc_depth_active?-(id*6+side+1):0;
  queue_triangle(view[f->v[0]],view[f->v[1]],view[f->v[2]],ink);
  if(f->count==4)queue_triangle(view[f->v[0]],view[f->v[2]],view[f->v[3]],ink);
 }
}
static void mega_city_model(void){
 mc_rich=station_class(&game)==STATION_RICH;
 const MegaCity *m=station_architecture_for(&game);mc_draw_city=m;
 Vec3 q={game.pos.x,game.pos.y,game.pos.z-STATION_Z};Vec3 eye=rotate(q,0,-station_angle(&game));mc_eye=eye;
 const int f[6][4]={{0,1,3,2},{4,6,7,5},{0,4,5,1},{2,3,7,6},{0,2,6,4},{1,5,7,3}};
 for(int k=0;k<m->n;k++){
  const MegaBlock *b=&m->b[k];if(!mc_visible(b->c,b->e))continue;
  if(b->kind==3&&length(sub(b->c,eye))>16000)continue;
  if(b->shape){mc_shaped_model(b,k,eye);continue;}
  Vec3 v[8];for(int i=0;i<8;i++)v[i]=(Vec3){b->c.x+(i&1?b->e.x:-b->e.x),b->c.y+(i&2?b->e.y:-b->e.y),b->c.z+(i&4?b->e.z:-b->e.z)};
  int show[6]={eye.z<b->c.z-b->e.z,eye.z>b->c.z+b->e.z,eye.y<b->c.y-b->e.y,eye.y>b->c.y+b->e.y,eye.x<b->c.x-b->e.x,eye.x>b->c.x+b->e.x};
  for(int i=0;i<6;i++)if(show[i]){
   unsigned ink=b->kind==4?RGB(25,40,56):RGB(38+b->district*2,48+b->district*2,67+b->district*2);
   if(station_class(&game)!=STATION_MEGA){StationProfile p=station_profile_for(&game,0);ink=livery_tint(b->kind==4?p.trim:p.hull,b->kind==4?-40:-18);}
   if(b->kind==3)ink=mc_palette[b->district][((int)(game.time*1.5f)+b->district)&1?1:3];
   if(i==3)ink=livery_tint(ink,21);else if(i==4)ink=livery_tint(ink,-8);
   mc_edge[k][i]=b->kind==4?RGB(33,94,125):livery_tint(ink,25);mc_dim[k][i]=livery_tint(ink,-6);
   surface_material=mc_depth_active?-(k*6+i+1):0;
   station_quad(v[f[i][0]],v[f[i][1]],v[f[i][2]],v[f[i][3]],ink);
  }
 }
 surface_material=0;
 /* Apertures are a few small emissive quads, not five rectangle tests for
    every shaded pixel on the entire docking-core facade. */
 float front=-station_half_for(&game,0);
 if(eye.z<front)for(int k=0;k<station_port_count_for(&game,0);k++){
  Vec3 o=station_port_offset_for(&game,0,k);
  for(int rim=0;rim<2;rim++){float w=rim?70:80,h=rim?32:42,z=front-2-rim;
   station_quad((Vec3){o.x-w,o.y-h,z},(Vec3){o.x+w,o.y-h,z},(Vec3){o.x+w,o.y+h,z},(Vec3){o.x-w,o.y+h,z},rim?RGB(2,5,9):RGB(77,241,202));}
 }
 /* Advertising tenders share the solid list. No combat/NPC slots or RNG. */
}
