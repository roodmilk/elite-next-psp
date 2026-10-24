/* G / Twin Stack: closed receivers and stacked sleeves built in ship space.
 * The existing hull hardpoints are attachments, never screen-coordinate mounts.
 * No equipment capacity, aiming, discharge timing or damage is changed here. */
typedef struct {Vec3 anchor,right,up,ahead;float size,unit,kick;int side,visible;} TwinMount;
static TwinMount twin_mounts[2];
static float twin_age=-1,twin_window=.1f;
static int twin_visible_count,twin_has_depth;
static Vec3 twin_attachment[23][2];static unsigned char twin_attachment_ready[23];
static unsigned twin_ink;static unsigned short*twin_raster_depth=surface_depth;static unsigned char*twin_raster_owner;static int twin_drawing_side;
static unsigned twin_weapon_ink(int w){
 switch(w){case 2:case 63:return RGB(65,235,235);case 20:case 28:case 62:case 65:return RGB(255,164,52);case 26:return RGB(255,220,95);case 27:return RGB(255,244,205);case 29:case 59:return RGB(76,160,255);case 30:case 66:return RGB(255,92,205);case 31:case 67:return RGB(176,255,72);case 58:return RGB(92,255,112);case 60:return RGB(194,92,255);case 61:return game.shots%3==0?RGB(255,214,76):game.shots%3==1?RGB(255,92,206):RGB(70,230,255);case 64:return RGB(218,236,255);case 68:return RGB(194,92,255);default:return RGB(255,68,58);}
}
static Vec3 twin_point(const TwinMount*m,float u,float t,float z){
 /* Short, substantial receivers sit at the lower canopy corners. The
  * compact sleeves point inward; emission follows their actual far tips. */
 Vec3 p=add(m->anchor,mul(m->ahead,m->size*(-3.2f+1.6f*t)));
 p=add(p,mul(m->up,-m->size*(.65f-.05f*t)+(z-m->kick)*m->unit));
 p=add(p,mul(m->right,m->side*m->size*(1.2f-.35f*t)));
 return add(p,mul(m->right,(m->side<0?u:-u)*m->unit*(1-.55f*t)));
}
static int twin_depth_visible(int x,int y,float z){
 return x>=0&&x<W&&y>=view_top()&&y<=view_bot()&&(!(mega_depth_valid||twin_has_depth)||mega_encode_depth(z)<=surface_depth[y*W+x]+2u);
}
static Vec3 twin_muzzle(const TwinMount*m,float sleeve){
 Vec3 tip=twin_point(m,0,1,sleeve),axis=norm(sub(tip,twin_point(m,0,.99f,sleeve)));
 return add(tip,mul(axis,m->unit*2));
}
static void twin_triangle(Vec3 wa,Vec3 wb,Vec3 wc,unsigned ink){
 Vec3 poly[5]={camera(&game,wa),camera(&game,wb),camera(&game,wc)},out[5];int n=3,kout=0;
 /* Exact near-plane clipping, including a camera grazing a receiver. */
 for(int k=0;k<n;k++){Vec3 a=poly[k],b=poly[(k+1)%n];if(a.z>=5)out[kout++]=a;if((a.z>=5)!=(b.z>=5))out[kout++]=add(a,mul(sub(b,a),(5-a.z)/(b.z-a.z)));}
 for(int tri=1;tri+1<kout;tri++){
  Point p[3]={project(out[0]),project(out[tri]),project(out[tri+1])};float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)continue;
  float iz[3]={1/(p[0].z*area),1/(p[1].z*area),1/(p[2].z*area)};
  int top=(int)fmaxf(view_top(),ceilf(fminf(p[0].y,fminf(p[1].y,p[2].y))-.5f)),bottom=(int)fminf(view_bot(),floorf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))-.5f));
  float step=(p[2].y-p[1].y)*iz[0]+(p[0].y-p[2].y)*iz[1]+(p[1].y-p[0].y)*iz[2];
  for(int y=top;y<=bottom;y++){float yy=y+.5f,left=1e9f,right=-1e9f;for(int k=0;k<3;k++){Point a=p[k],b=p[(k+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*(b.x-a.x)/(b.y-a.y);left=fminf(left,x);right=fmaxf(right,x);}}
   int x0=(int)fmaxf(0,ceilf(left-.5f)),x1=(int)fminf(W-1,floorf(right-.5f));float inv=edge(p[1],p[2],x0+.5f,yy)*iz[0]+edge(p[2],p[0],x0+.5f,yy)*iz[1]+edge(p[0],p[1],x0+.5f,yy)*iz[2];
   for(int x=x0;x<=x1;x++,inv+=step){if(inv<=0)continue;unsigned depth=mega_encode_depth(1/inv);int at=y*W+x;if(depth<=twin_raster_depth[at]){twin_raster_depth[at]=depth;if(twin_raster_owner)twin_raster_owner[at]=twin_drawing_side;fb[y*STRIDE+x]=ink;}}
  }
 }
}
static void twin_quad(const TwinMount*m,const float p[4][3],unsigned ink){Vec3 v[4];for(int k=0;k<4;k++)v[k]=twin_point(m,p[k][0],p[k][1],p[k][2]);twin_triangle(v[0],v[1],v[2],ink);twin_triangle(v[0],v[2],v[3],ink);}
static void twin_detail_line(const TwinMount*m,float u,float t,float z,float uu,float tt,float zz,unsigned ink){
 Vec3 a=camera(&game,twin_point(m,u,t,z)),b=camera(&game,twin_point(m,uu,tt,zz));if(a.z<5&&b.z<5)return;
 if(a.z<5)a=add(a,mul(sub(b,a),(5-a.z)/(b.z-a.z)));if(b.z<5)b=add(b,mul(sub(a,b),(5-b.z)/(a.z-b.z)));
 Point p=project(a),q=project(b);float dx=q.x-p.x,dy=q.y-p.y,lo=0,hi=1,ds[4]={-dx,dx,-dy,dy},es[4]={p.x,W-1-p.x,p.y-view_top(),view_bot()-p.y};
 for(int k=0;k<4;k++){if(fabsf(ds[k])<.00001f){if(es[k]<0)return;continue;}float t=es[k]/ds[k];if(ds[k]<0)lo=fmaxf(lo,t);else hi=fminf(hi,t);if(lo>hi)return;}
 int n=(int)fminf(600,ceilf(fmaxf(fabsf(dx),fabsf(dy))*(hi-lo)));if(n<1)n=1;
 for(int i=0;i<=n;i++){float t=lo+(hi-lo)*i/n;int x=(int)(p.x+dx*t),y=(int)(p.y+dy*t);if(x<0||x>=W||y<view_top()||y>view_bot())continue;unsigned depth=mega_encode_depth(1.f/((1-t)/a.z+t/b.z));int at=y*W+x;if(depth<=twin_raster_depth[at]+2u){if(depth<twin_raster_depth[at])twin_raster_depth[at]=depth;if(twin_raster_owner)twin_raster_owner[at]=twin_drawing_side;fb[y*STRIDE+x]=ink;}}
}
static void twin_mounts_prepare(void){
 twin_age=-1;twin_visible_count=0;PrimaryVisualEvent*e=&primary_visual_event;
 if(e->owner==&game&&e->serial==game.shots&&game.shots>0){float age=game.time-e->time;twin_window=fminf(.18f,fmaxf(.07f,e->cycle*.65f));if(age>=0&&age<twin_window)twin_age=age;}
 twin_ink=twin_weapon_ink(twin_age>=0?e->weapon:fit_weapon_item(&game));
 Vec3 right,up,ahead;flight_camera_basis(&right,&up,&ahead);
 for(int k=0;k<2;k++){TwinMount*m=&twin_mounts[k];memset(m,0,sizeof(*m));m->side=k?1:-1;
  if(game.ship>=0&&game.ship<23&&(twin_attachment_ready[game.ship]&(1u<<k))){Vec3 p=twin_attachment[game.ship][k];m->anchor=add(game.pos,add(mul(right,p.x),add(mul(up,p.y),mul(ahead,p.z))));}
  else {if(!player_hardpoint(m->side,1,&m->anchor))continue;if(game.ship>=0&&game.ship<23){Vec3 d=sub(m->anchor,game.pos);twin_attachment[game.ship][k]=(Vec3){dot(d,right),dot(d,up),dot(d,ahead)};twin_attachment_ready[game.ship]|=1u<<k;}}
  /* Preserve the approved receiver/sleeve proportions at the actual hull
   * attachment depth. A 12m ceiling shrank large-ship guns into floating
   * specks and lifted their rear receivers away from the cockpit sill. */
  float z=dot(sub(m->anchor,game.pos),ahead);if(z<=5)continue;m->right=right;m->up=up;m->ahead=ahead;m->size=fmaxf(2,z*.17f);m->unit=m->size*.0402f;m->kick=twin_age>=0&&twin_age<.035f?1:0;
  Vec3 tip=camera(&game,twin_point(m,0,1,0)),rear=camera(&game,twin_point(m,0,0,0));if(tip.z<5&&rear.z<5)continue;
  Point p=project(tip.z>=5?tip:rear);m->visible=p.x>-64&&p.x<W+64&&p.y>view_top()-32&&p.y<view_bot()+32;twin_visible_count+=m->visible;
 }
}
/* Closed volumes, rather than broad receiver sheets. Each barrel has an
 * eight-sided cross-section, both end caps, sleeve collars and a separate
 * solid breech. All faces share the existing world/depth/cached renderer. */
static void twin_box(const TwinMount*m,float l,float r,float rear,float fore,float lo,float hi,unsigned top,unsigned side,unsigned edge_ink){
 const float p[6][4][3]={
  {{l,rear,hi},{r,rear,hi},{r,fore,hi},{l,fore,hi}},
  {{l,rear,lo},{r,rear,lo},{r,fore,lo},{l,fore,lo}},
  {{l,rear,lo},{l,rear,hi},{l,fore,hi},{l,fore,lo}},
  {{r,rear,lo},{r,rear,hi},{r,fore,hi},{r,fore,lo}},
  {{l,rear,lo},{r,rear,lo},{r,rear,hi},{l,rear,hi}},
  {{l,fore,lo},{r,fore,lo},{r,fore,hi},{l,fore,hi}}};
 for(int f=0;f<6;f++)twin_quad(m,p[f],f==0?top:f==1?RGB(15,27,39):side);
 twin_detail_line(m,l,rear,hi,r,rear,hi,edge_ink);twin_detail_line(m,l,rear,lo,l,rear,hi,edge_ink);twin_detail_line(m,r,rear,lo,r,rear,hi,edge_ink);
 twin_detail_line(m,l,rear,hi,l,fore,hi,edge_ink);twin_detail_line(m,r,rear,hi,r,fore,hi,edge_ink);
}
static void twin_octagon(const TwinMount*m,float rear,float fore,float z,float ru,float rz,unsigned steel,unsigned edge_ink,unsigned shade){
 static const float ring[8][2]={{0,1},{.7071f,.7071f},{1,0},{.7071f,-.7071f},{0,-1},{-.7071f,-.7071f},{-1,0},{-.7071f,.7071f}};
 Vec3 back[8],front[8];for(int k=0;k<8;k++){back[k]=twin_point(m,ring[k][0]*ru,rear,z+ring[k][1]*rz);front[k]=twin_point(m,ring[k][0]*ru,fore,z+ring[k][1]*rz);}
 for(int k=0;k<8;k++){int j=(k+1)&7;unsigned ink=k==0||k==7?edge_ink:k==1||k==6?steel:k==2||k==5?shade:RGB(15,27,39);
  twin_triangle(back[k],back[j],front[j],ink);twin_triangle(back[k],front[j],front[k],ink);
  twin_triangle(twin_point(m,0,rear,z),back[j],back[k],shade);twin_triangle(twin_point(m,0,fore,z),front[k],front[j],steel);
 }
}
static void twin_draw_geometry(void){
 for(int k=0;k<2;k++){const TwinMount*m=&twin_mounts[k];if(!m->visible)continue;twin_drawing_side=k;
  float light=twin_age>=0?(1-twin_age/twin_window)*(.35f+.25f*sinf(game.shots*1.7f+k*2.1f)):0;
  unsigned steel=mix_rgb(RGB(75,94,107),twin_ink,light*.4f),edge_ink=mix_rgb(RGB(131,151,160),twin_ink,light*.7f),shade=RGB(27,42,57),accent=RGB(122,182,211);
  twin_box(m,-5.5f,5.5f,-.10f,.14f,-4,-2.8f,shade,RGB(15,27,39),edge_ink);
  twin_box(m,-1.2f,1.2f,-.12f,-.02f,-3,9,shade,RGB(15,27,39),edge_ink);
  for(int sleeve=0;sleeve<2;sleeve++){float z=sleeve*6.f;
   twin_octagon(m,.12f,1,z,2.8f,2.05f,steel,edge_ink,shade);
   twin_octagon(m,.20f,.28f,z,3.2f,2.4f,steel,edge_ink,shade);
   twin_octagon(m,.57f,.63f,z,3.05f,2.25f,shade,steel,RGB(15,27,39));
   twin_octagon(m,.86f,1,z,3.1f,2.3f,steel,edge_ink,shade);
   twin_box(m,-4.5f,4.5f,-.04f,.18f,z-2.6f,z+2.6f,steel,shade,edge_ink);
   twin_detail_line(m,0,.30f,z+2.07f,0,.84f,z+2.07f,accent);
   twin_detail_line(m,-2.7f,.02f,z+2.62f,2.7f,.02f,z+2.62f,accent);
   for(int vent=0;vent<3;vent++){float t=.035f+vent*.04f;twin_detail_line(m,4.51f,t,z-1.1f,4.51f,t,z+1.1f,RGB(15,27,39));twin_detail_line(m,-4.51f,t,z-1.1f,-4.51f,t,z+1.1f,RGB(15,27,39));}
   for(int side=-1;side<=1;side+=2){float u=side*3.f;twin_detail_line(m,u-.35f,-.041f,z-1.5f,u+.35f,-.041f,z-1.5f,edge_ink);twin_detail_line(m,u,-.041f,z-1.85f,u,-.041f,z-1.15f,edge_ink);}
  }
 }
}
/* First-person geometry is rigid relative to its camera. Bake its exact
 * native colour AND depth once, preserving planes, bolts, braces and markings.
 * A real moving/chase camera uses the live 3D path instead of a false overlay. */
typedef struct{unsigned offset,ink;unsigned short depth;unsigned char side,gloss;} TwinPixel;
static TwinPixel twin_cache[2][16384];static int twin_cache_count[2],twin_cache_ship=-1;static float twin_cache_focal;static int twin_cache_ox,twin_cache_oy,twin_cache_top,twin_cache_bottom;
static int twin_camera_is_rigid(void){Vec3 r,u,a,cr,cu,ca;flight_camera_basis(&r,&u,&a);camera_axes(&game,&cr,&cu,&ca);return !third_person&&length(sub(camera_origin(&game),game.pos))<.0001f&&dot(r,cr)>.999999f&&dot(u,cu)>.999999f&&dot(a,ca)>.999999f;}
static int twin_bake(void){
 unsigned*pixels=malloc(STRIDE*H*sizeof(*pixels));unsigned short*depth=malloc(W*H*sizeof(*depth));unsigned char*owner=malloc(W*H);if(!pixels||!depth||!owner){free(pixels);free(depth);free(owner);return 0;}
 unsigned*previous=fb;unsigned short*previous_depth=twin_raster_depth;unsigned char*previous_owner=twin_raster_owner;float age=twin_age,kicks[2]={twin_mounts[0].kick,twin_mounts[1].kick};int ok=1;twin_age=-1;fb=pixels;twin_raster_depth=depth;twin_raster_owner=owner;
 for(int pose=0;pose<2;pose++){memset(pixels,0,STRIDE*H*sizeof(*pixels));memset(depth,255,W*H*sizeof(*depth));memset(owner,0,W*H);twin_mounts[0].kick=twin_mounts[1].kick=pose;twin_draw_geometry();int n=0;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++){unsigned ink=pixels[y*STRIDE+x];if(!(ink>>24))continue;if(n==16384){ok=0;break;}int side=owner[y*W+x];int gloss=ink==RGB(75,94,107)?1:ink==RGB(131,151,160)?2:0;twin_cache[pose][n++]=(TwinPixel){y*STRIDE+x,ink,depth[y*W+x],side,gloss};}
  twin_cache_count[pose]=n;
 }
 fb=previous;twin_raster_depth=previous_depth;twin_raster_owner=previous_owner;twin_age=age;twin_mounts[0].kick=kicks[0];twin_mounts[1].kick=kicks[1];free(pixels);free(depth);free(owner);
 if(ok){twin_cache_ship=game.ship;twin_cache_focal=proj_focal;twin_cache_ox=proj_ox;twin_cache_oy=proj_oy;twin_cache_top=view_top();twin_cache_bottom=view_bot();}return ok;
}
static void twin_housings(void){
 twin_has_depth=0;twin_age=-1;twin_visible_count=0;if(!hc_presentation_context()||!fit_is_active_weapon(fit_weapon_item(&game)))return;twin_mounts_prepare();twin_raster_depth=surface_depth;
 int cached=twin_camera_is_rigid();if(cached&&(twin_cache_ship!=game.ship||twin_cache_focal!=proj_focal||twin_cache_ox!=proj_ox||twin_cache_oy!=proj_oy||twin_cache_top!=view_top()||twin_cache_bottom!=view_bot()))cached=twin_bake();
 if(!mega_depth_valid)memset(surface_depth+view_top()*W,255,(view_bot()-view_top()+1)*W*sizeof(*surface_depth));
 if(cached){int pose=twin_mounts[0].kick>0;float light[2]={0};if(twin_age>=0)for(int k=0;k<2;k++)light[k]=(1-twin_age/twin_window)*(.35f+.25f*sinf(game.shots*1.7f+k*2.1f));
  for(int k=0;k<twin_cache_count[pose];k++){const TwinPixel*p=&twin_cache[pose][k];unsigned y=p->offset/STRIDE,x=p->offset%STRIDE,at=y*W+x;if(p->depth>surface_depth[at])continue;surface_depth[at]=p->depth;unsigned ink=p->ink;if(p->gloss&&light[p->side]>0)ink=mix_rgb(ink,twin_ink,light[p->side]*(p->gloss==1?.4f:.7f));fb[p->offset]=ink;}
 }else twin_draw_geometry();twin_has_depth=1;
}
/* Small prebaked additive kernel: no blur pass or full-screen light wash. */
static unsigned char twin_glow_kernel[17*17];static int twin_glow_ready;
static void twin_glow(Vec3 world,unsigned ink,float gain){
 if(high_contrast||gain<=0)return;Vec3 c=camera(&game,world);if(c.z<5)return;Point p=project(c);int cx=(int)p.x,cy=(int)p.y;
 if(!twin_glow_ready){for(int y=-8;y<=8;y++)for(int x=-8;x<=8;x++){int r=65-x*x-y*y;twin_glow_kernel[(y+8)*17+x+8]=r>0?(r*r*255)/(65*65):0;}twin_glow_ready=1;}
 unsigned depth=mega_encode_depth(c.z);int scale=(int)(gain*256);if(scale>384)scale=384;
 for(int yy=0;yy<17;yy++){int y=cy+yy-8;if(y<view_top()||y>view_bot())continue;for(int xx=0;xx<17;xx++){int x=cx+xx-8,w=twin_glow_kernel[yy*17+xx]*scale>>8;if(!w||x<0||x>=W)continue;if((mega_depth_valid||twin_has_depth)&&depth>surface_depth[y*W+x]+2u)continue;sfx_add(x,y,dim_rgb(ink,w,256),view_top(),view_bot());}}
}
static Vec3 twin_path(Vec3 start,Vec3 end,float t){return add(start,mul(sub(end,start),t));}
/* Perspective-correct sampling retains a readable travelling packet instead
 * of collapsing an entire long-range shot into its last screen pixel. */
static Vec3 twin_screen_path(Vec3 start,Vec3 end,float t){Vec3 a=camera(&game,start),b=camera(&game,end);if(a.z>0&&b.z>0){float d=b.z*(1-t)+a.z*t;if(d>.001f)t=t*a.z/d;}return twin_path(start,end,t);}
static void twin_orb(Vec3 world,unsigned ink,float phase,int dark){
 Vec3 c=camera(&game,world);if(c.z<5)return;Point p=project(c);int cx=(int)p.x,cy=(int)p.y,r=2+(int)((dark?8:6)*(1-phase));
 for(int y=-r;y<=r;y++)for(int x=-r;x<=r;x++){int d=x*x+y*y;if(d>r*r||!twin_depth_visible(cx+x,cy+y,c.z))continue;unsigned color=dark?RGB(12,8,22):d<r*r/5?RGB(255,231,247):dim_rgb(ink,3,4);if(d>(r-2)*(r-2))color=dim_rgb(ink,2,3);pixel(cx+x,cy+y,color);}
 for(int arc=0;arc<3;arc++)for(int j=0;j<6;j++){float a=arc*2.0943951f+phase*4+j*.12f;int x=cx+(int)(cosf(a)*(r+2)),y=cy+(int)(sinf(a)*r);if(twin_depth_visible(x,y,c.z))pixel(x,y,ink);}
}
static void twin_packet(Vec3 start,Vec3 end,float t,float length,unsigned ink,int width){
 if(t<0||t>1)return;Vec3 a=twin_screen_path(start,end,fmaxf(0,t-length*(1-t)-.02f)),b=twin_screen_path(start,end,t);width=1+(int)((width-1)*(1-t)+.5f);exterior_ray(a,b,dim_rgb(ink,1,3),width+1);exterior_ray(a,b,ink,width);exterior_ray(a,b,WHITE,1);twin_glow(b,ink,(1-t)*.5f);
}
/* Resolve the visible surface of a confirmed NPC hit from the current world
 * depth. The game's cone test records the contact centre for ordinary hulls.
 * A substantially nearer object still blocks the contact and its light. */
static Vec3 twin_contact_surface(const PrimaryVisualEvent*e){
 if(!IS_NPC_ID(e->hit_id)||!mega_depth_valid)return e->impact;Vec3 c=camera(&game,e->impact);if(c.z<5)return e->impact;Point p=project(c);int x=(int)p.x,y=(int)p.y;if(x<0||x>=W||y<view_top()||y>view_bot())return e->impact;
 unsigned d=surface_depth[y*W+x];if(d==65535)return e->impact;float z=d<32768?d*.25f:8192+(d-32768)*2.f;NPC*n=&game.npc[e->hit_id-NPC_ID_MIN];float margin=n->freighter?8.f:npc_visual_radius(n)*2;
 if(z>=c.z||c.z-z>margin)return e->impact;c=mul(c,fmaxf(5,z-.75f)/c.z);Vec3 r,u,a;camera_axes(&game,&r,&u,&a);return add(camera_origin(&game),add(mul(r,c.x),add(mul(u,c.y),mul(a,c.z))));
}
static void twin_contact_effect(const PrimaryVisualEvent*e,float age){
 if(e->hit_id<0)return;Vec3 contact=twin_contact_surface(e);Vec3 c=camera(&game,contact);if(c.z<5)return;Point p=project(c);int x=(int)p.x,y=(int)p.y;if(!twin_depth_visible(x,y,c.z))return;
 float life=1-age;unsigned ink=e->shield_hit?twin_ink:e->weapon==30||e->weapon==66?twin_ink:RGB(255,185,83);twin_glow(contact,ink,life*.8f);
 if(e->weapon==20)for(int j=0;j<5;j++){float a=j*1.2566371f+age*4;int xx=x+(int)(cosf(a)*10),yy=y+(int)(sinf(a)*7);if(twin_depth_visible(xx,yy,c.z))line(xx,yy,xx+(int)(cosf(a)*4),yy+(int)(sinf(a)*3),RGB(255,205,82));}
 if(e->weapon==31)for(int j=-2;j<=2;j++){int yy=y+j*4;for(int k=-12;k<=12-abs(j)*3;k++)if((k&3)!=0&&twin_depth_visible(x+k,yy,c.z))pixel(x+k,yy,twin_ink);}
 if(e->weapon==59||e->weapon==64)for(int j=0;j<32;j++){float a=j*.19634954f;int r=4+(int)(age*13),xx=x+(int)(cosf(a)*r),yy=y+(int)(sinf(a)*r*.6f);if(twin_depth_visible(xx,yy,c.z))pixel(xx,yy,dim_rgb(twin_ink,(int)(life*240),256));}
 if(e->shield_hit)for(int arc=0;arc<3;arc++)for(int j=0;j<8;j++){float a=arc*2.0943951f+j*.10f+age*1.5f,b=a+.10f;int r=13+(int)(age*10);int xx=x+(int)(cosf(a)*r),yy=y+(int)(sinf(a)*r*.55f),xxx=x+(int)(cosf(b)*r),yyy=y+(int)(sinf(b)*r*.55f);if(twin_depth_visible(xx,yy,c.z)&&twin_depth_visible(xxx,yyy,c.z))line(xx,yy,xxx,yyy,dim_rgb(ink,(int)(life*220),256));}
 for(int j=0;j<13;j++){float a=j*2.399963f+e->serial*.73f,r=3+age*(10+(j%5)*4);int xx=x+(int)(cosf(a)*r),yy=y+(int)(sinf(a)*r*.6f);if(!twin_depth_visible(xx,yy,c.z))continue;for(int q=0;q<3;q++){int sx=xx+(int)(cosf(a)*q),sy=yy+(int)(sinf(a)*q);if(twin_depth_visible(sx,sy,c.z))pixel(sx,sy,q?dim_rgb(ink,(int)(life*220),256):WHITE);}}
}
static void twin_fire_effects(void){
 if(twin_age<0)return;PrimaryVisualEvent*e=&primary_visual_event;float age=twin_age/twin_window,power=(1-age)*(1-age);int w=e->weapon;
 for(int side=0;side<2;side++){const TwinMount*m=&twin_mounts[side];if(m->size<=0)continue;
  Vec3 start=twin_muzzle(m,(e->serial&1)?6:0),end=e->impact;
  /* The real game resolves aim and hits. These paths only present that event. */
  if(w==1||w==25||w==26||w==58||w==62||w==69){float t=4*age/(1+3*age);int count=w==26?6:w==58?5:w==62?2:1;for(int j=0;j<count;j++)twin_packet(start,end,t-j*.18f,w==25?.15f:w==69?.06f:.10f,twin_ink,w==25?3:2);}
  else if(w==28||w==65||w==67){int spread=w==65?4:w==67?5:3;for(int j=-spread;j<=spread;j++){Vec3 target=add(end,add(mul(m->right,j*(w==67?18.f:26.f)),mul(m->up,-abs(j)*8.f)));twin_packet(start,target,age,.13f,twin_ink,2);}}
  else if(w==29||w==31||w==60){Vec3 last=start;for(int j=1;j<=8;j++){float t=j/8.f;Vec3 p=twin_screen_path(start,end,t);if(j<8)p=add(p,mul(m->right,sinf(j*2.31f+age*18+side)*camera(&game,p).z/proj_focal*(w==60?7:4)*(1-t)));exterior_ray(last,p,dim_rgb(twin_ink,1,2),2);exterior_ray(last,p,twin_ink,1);last=p;}}
  else if(w==30||w==66||w==68){float t=4*age/(1+3*age);Vec3 orb=twin_screen_path(start,end,t);if(w!=66||((side+e->serial)&1)==0){if(w!=68)twin_packet(start,end,t,.05f,twin_ink,3);twin_orb(orb,twin_ink,t,w==68);twin_glow(orb,twin_ink,w==68?.55f:1.3f);}}
  else if(w==61){unsigned colors[3]={RGB(255,214,76),RGB(255,92,206),RGB(70,230,255)};for(int j=0;j<3;j++)twin_packet(start,add(end,mul(m->right,(j-1)*18)),fminf(1,age+j*.08f),.16f,colors[(j+e->serial)%3],2);}
  else if(w==63){Vec3 last=start;for(int j=1;j<=18;j++){float t=j/18.f;Vec3 p=twin_screen_path(start,end,t);p=add(p,mul(m->up,sinf(t*25-age*16+side*3.14f)*camera(&game,p).z/proj_focal*7*(1-t)));exterior_ray(last,p,twin_ink,1);last=p;}}
  else {exterior_ray(start,end,dim_rgb(twin_ink,1,3),3);exterior_ray(start,end,twin_ink,w==25?3:2);exterior_ray(start,end,WHITE,1);}
  twin_glow(start,twin_ink,power*(.75f+.2f*sinf(e->serial+side*2.1f)));
 }
 twin_contact_effect(e,age);
}
static void twin_cockpit_reflections(void){
 if(twin_age<0||!hc_presentation_context()||hud_mode!=0||hud_hidden||high_contrast)return;
 float age=twin_age/twin_window,power=(1-age)*(1-age);for(int side=0;side<2;side++){
  float variation=.4f+.35f*(.5f+.5f*sinf(primary_visual_event.serial*1.37f+side*2.1f+age*4));int cy=99+(int)(sinf(primary_visual_event.serial*.7f+side)*12);
  for(int dy=-18;dy<=18;dy++)for(int dx=-5;dx<=5;dx++){int x=side?472+dx:7+dx,y=cy+dy;if((hc_browser&&x>=8&&x<160&&y>=40)||(tools_open&&side&&((y>=41&&y<64)||(y>=96&&y<142&&x<474))))continue;int radial=324-dy*dy-dx*dx*13;if(radial<=0)continue;int level=(int)(radial*power*variation*.65f);sfx_add(x,y,dim_rgb(twin_ink,level,256),35,163);}
 }
}
