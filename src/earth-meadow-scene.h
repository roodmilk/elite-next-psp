/* A single connected lattice: 40 m cells under every accessible position,
 * progressively wider cells beyond it. Adjacent tiles share exact vertices;
 * there are no overlapping near/far sheets, cut-outs or T-junctions. */
#define MEADOW_OUTER 13
#define MEADOW_CENTRE (MEADOW_OUTER+16)
#define MEADOW_GRID (MEADOW_OUTER*2+33)
static Vec3 meadow_ground_cache[MEADOW_GRID][MEADOW_GRID];
static Vec3 meadow_view_cache[MEADOW_GRID][MEADOW_GRID];
static unsigned meadow_ground_ink[MEADOW_GRID-1][MEADOW_GRID-1];
static float meadow_cache_x,meadow_cache_z;static int meadow_cache_ready;
static int meadow_visible_tiles;
static float meadow_grid_axis(int i){
 static const float outer[MEADOW_OUTER]={800,1000,1240,1520,1840,2200,2640,3200,4000,5200,7200,12000,60000};
 if(i<MEADOW_OUTER)return -outer[MEADOW_OUTER-1-i];
 if(i>MEADOW_CENTRE+16)return outer[i-MEADOW_CENTRE-17];
 return (i-MEADOW_CENTRE)*40.f;
}
static Vec3 meadow_ground_point(Vec3 pad,float x,float z,float lift){Vec3 p={pad.x+x,0,pad.z+z};p.y=terrain_height(&game,p.x,p.z)+lift;return p;}
static unsigned meadow_sky_ink(float ray_y){
 /* Inverse camera pitch: a world elevation always receives the same colour.
  * No source-image edge can enter the view when looking up or down. */
 float up=(surface_sp+surface_cp*ray_y)/sqrtf(1+ray_y*ray_y);
 return mix_rgb(RGB(195,216,218),RGB(53,125,184),meadow_smooth(up*1.35f));
}
static void earth_meadow_sky(int top,int bottom){
 for(int y=top;y<bottom;y++){
  unsigned ink=meadow_sky_ink((proj_oy-y)/240.f);
  for(int x=0;x<W;x++)fb[y*STRIDE+x]=ink;
 }
}
static void earth_meadow_horizon(int top,int bottom){
 /* Beyond the mesh lies hazy land, never the sky colour underneath a finite
  * terrain edge. Its horizon follows world pitch, not a fixed screen row.
  * Leave depth untouched so coverage tests can still detect real mesh holes. */
 for(int y=top;y<bottom;y++){
  float up=surface_sp+surface_cp*(proj_oy-y)/240.f;
  if(up>=0)continue;
  unsigned ink=mix_rgb(RGB(176,196,190),RGB(132,165,145),meadow_smooth(-up*12));
  for(int x=0;x<W;x++)fb[y*STRIDE+x]=ink;
 }
}
static void meadow_prepare_ground(Vec3 pad){
 float ox=floorf(pad.x/40)*40,oz=floorf(pad.z/40)*40;
 if(meadow_cache_ready&&ox==meadow_cache_x&&oz==meadow_cache_z)return;
 meadow_cache_ready=1;meadow_cache_x=ox;meadow_cache_z=oz;
 for(int z=0;z<MEADOW_GRID;z++)for(int x=0;x<MEADOW_GRID;x++){
  float wx=ox+meadow_grid_axis(x),wz=oz+meadow_grid_axis(z);
  meadow_ground_cache[z][x]=(Vec3){wx,terrain_mesh_height(&game,wx,wz),wz};
 }
 for(int z=0;z<MEADOW_GRID-1;z++)for(int x=0;x<MEADOW_GRID-1;x++){
  Vec3 a=meadow_ground_cache[z][x],b=meadow_ground_cache[z][x+1],c=meadow_ground_cache[z+1][x+1],d=meadow_ground_cache[z+1][x];
  float mx=(a.x+c.x)*.5f-pad.x,mz=(a.z+c.z)*.5f-pad.z;
  float sx=(b.y-a.y+c.y-d.y)/(2*(b.x-a.x)),sz=(d.y-a.y+c.y-b.y)/(2*(d.z-a.z));
  float slope=hypotf(sx,sz),tone=.45f+.1f*sinf(mx*.012f)+.08f*cosf(mz*.015f)-sx*.22f+sz*.18f;
  unsigned ink=mix_rgb(RGB(75,112,62),RGB(117,148,77),tone);
  float radius=hypotf(mx,mz);
  /* Broad fields follow the valley, with restrained grass/hay variation.
   * These are colours on the one solid mesh, not floating overlay sheets. */
  if(radius>550){
   float field=.5f+.25f*sinf(mx*.0035f+mz*.0017f)+.25f*cosf(mz*.0041f-mx*.0012f);
   unsigned farmland=mix_rgb(RGB(69,112,68),RGB(155,167,91),field);
   ink=mix_rgb(ink,farmland,meadow_smooth((radius-550)/550)*.78f);
  }
  /* The enclosing banks are grassy, not a ring of concrete-coloured walls.
   * Exposed stone belongs to the valley cliff, peaks and separate outcrops. */
  if(slope>.32f&&(radius>750||(mz>meadow_cliff(mx)&&fabsf(mx)<430)))ink=mix_rgb(ink,RGB(114,120,113),fminf(1,(slope-.32f)*1.6f));
  if(radius<600&&slope>.15f)ink=mix_rgb(ink,RGB(65,97,57),fminf(.3f,slope*.2f));
  float height=(a.y+b.y+c.y+d.y)*.25f;
  if(height>440)ink=mix_rgb(ink,RGB(214,223,215),meadow_smooth((height-440)/210));
  float fog=meadow_smooth((radius-600)/8500)*.90f;
  meadow_ground_ink[z][x]=mix_rgb(ink,RGB(176,196,190),fog);
 }
}
/* Distant props use the exact displayed triangle height. Sampling the finer
 * collision lattice here would leave far trees floating above coarse fields. */
static Vec3 meadow_landscape_point(Vec3 pad,float x,float z){
 meadow_prepare_ground(pad);float wx=pad.x+x,wz=pad.z+z;int ix=0,iz=0;
 while(ix<MEADOW_GRID-2&&wx>meadow_ground_cache[0][ix+1].x)ix++;
 while(iz<MEADOW_GRID-2&&wz>meadow_ground_cache[iz+1][0].z)iz++;
 Vec3 a=meadow_ground_cache[iz][ix],b=meadow_ground_cache[iz][ix+1],c=meadow_ground_cache[iz+1][ix+1],d=meadow_ground_cache[iz+1][ix];
 float u=(wx-a.x)/(b.x-a.x),v=(wz-a.z)/(d.z-a.z);
 float y=u>=v?a.y+(b.y-a.y)*u+(c.y-b.y)*v:a.y+(c.y-d.y)*u+(d.y-a.y)*v;
 return (Vec3){wx,y,wz};
}
static int meadow_quad_visible(Vec3 a,Vec3 b,Vec3 c,Vec3 d){
 /* Reject only if ALL corners are beyond one frustum plane. A midpoint
  * behind the camera does not mean the tile beneath our feet is invisible. */
 if(a.z<4&&b.z<4&&c.z<4&&d.z<4)return 0;
 if(a.x< -a.z-3&&b.x< -b.z-3&&c.x< -c.z-3&&d.x< -d.z-3)return 0;
 if(a.x>a.z+3&&b.x>b.z+3&&c.x>c.z+3&&d.x>d.z+3)return 0;
 float upper=(proj_oy-surface_y_min+2)/240.f,lower=(surface_y_max+2-proj_oy)/240.f;
 if(a.y>a.z*upper+3&&b.y>b.z*upper+3&&c.y>c.z*upper+3&&d.y>d.z*upper+3)return 0;
 if(a.y< -a.z*lower-3&&b.y< -b.z*lower-3&&c.y< -c.z*lower-3&&d.y< -d.z*lower-3)return 0;
 return 1;
}
static void earth_meadow_ground(Vec3 pad){
 meadow_prepare_ground(pad);surface_material=0;meadow_visible_tiles=0;
 for(int z=0;z<MEADOW_GRID;z++)for(int x=0;x<MEADOW_GRID;x++){
  meadow_view_cache[z][x]=camera(&game,meadow_ground_cache[z][x]);
 }
 for(int z=0;z<MEADOW_GRID-1;z++)for(int x=0;x<MEADOW_GRID-1;x++){
  Vec3 a=meadow_view_cache[z][x],b=meadow_view_cache[z][x+1],c=meadow_view_cache[z+1][x+1],d=meadow_view_cache[z+1][x];
  if(!meadow_quad_visible(a,b,c,d))continue;
  if(drawcount>1750)flush_meshes();
  meadow_visible_tiles++;
  unsigned ink=meadow_ground_ink[z][x];
  queue_triangle(a,b,c,ink);queue_triangle(a,c,d,ink);
 }flush_meshes();
}
static Vec3 meadow_water_point(Vec3 pad,float x,float z){return (Vec3){pad.x+x,EARTH_MEADOW_WATER_Y,pad.z+z};}
static void earth_meadow_water_draw(Vec3 pad){
 surface_material=0;Vec3 centre=meadow_water_point(pad,-174,132);
 /* Flat lake extends into the banks; the depth buffer cuts its actual shore
  * against the solid basin. No independently triangulated warped water. */
 for(int i=0;i<32;i++){
  float a=i*.19634954f,b=(i+1)*.19634954f;
  Vec3 p=meadow_water_point(pad,-174+104*cosf(a),132+78*sinf(a)),q=meadow_water_point(pad,-174+104*cosf(b),132+78*sinf(b));
  queue_triangle(camera(&game,centre),camera(&game,p),camera(&game,q),mix_rgb(RGB(43,102,117),RGB(83,147,156),.5f+.15f*sinf(a)));
 }
 for(int i=0;i<33;i++){
  float z=-410+i*16,z1=z+16,x=-174+17*sinf(z*.018f),x1=-174+17*sinf(z1*.018f);
  planet_quad(meadow_water_point(pad,x-25,z),meadow_water_point(pad,x+25,z),meadow_water_point(pad,x1+25,z1),meadow_water_point(pad,x1-25,z1),RGB(65,132,143));
 }flush_meshes();
 /* Reflections/ripples stay on the horizontal water plane and disappear
  * behind banks and trunks. Short strokes, never a screen-space overlay. */
 for(int i=0;i<44;i++){
  float a=i*2.39996323f,r=8+(i%9)*8;
  Vec3 p=meadow_water_point(pad,-174+cosf(a)*r,132+sinf(a)*r*.68f);
  if(i<14){p.z=pad.z+85-i*33;p.x=pad.x-174+17*sinf((p.z-pad.z)*.018f)+sinf(i*3.f)*9;}
  p.y+=.3f;Vec3 v=camera(&game,p);if(v.z<6)continue;Point q=project(v);
  surface_sprite_z=v.z-surface_sprite_tan*v.y;
  unsigned ink=mix_rgb(RGB(84,154,161),RGB(183,211,201),.3f+.3f*sinf(game.time*.8f+i));
  planet_cliprect(q.x,q.y,(int)fmaxf(2,fminf(16,720/v.z)),1,ink);
 }surface_sprite_z=0;
}
static void earth_meadow_outcrops(Vec3 pad){
 surface_material=0;
 for(int i=0;i<EARTH_MEADOW_ROCKS;i++){
  float x,z,rx,rz,h;earth_meadow_rock(i,&x,&z,&rx,&rz,&h);
  Vec3 p=meadow_ground_point(pad,x,z,-2),v=camera(&game,add(p,(Vec3){0,h*.5f,0}));
  if(v.z< -h||fabsf(v.x)>v.z+rx*2)continue;
  Vec3 peak=add(p,(Vec3){rx*.12f,h,-rz*.15f});
  for(int j=0;j<7;j++){
   float a=j*.8975979f,b=(j+1)*.8975979f;
   Vec3 u=meadow_ground_point(pad,x+cosf(a)*rx,z+sinf(a)*rz,-3),w=meadow_ground_point(pad,x+cosf(b)*rx,z+sinf(b)*rz,-3);
   Vec3 ut=add(p,(Vec3){cosf(a)*rx*.68f,h*(.60f+.10f*sinf(j+i)),sinf(a)*rz*.68f});
   Vec3 wt=add(p,(Vec3){cosf(b)*rx*.68f,h*(.60f+.10f*sinf(j+1+i)),sinf(b)*rz*.68f});
   unsigned ink=mix_rgb(RGB(82,96,88),RGB(148,151,124),.48f+.26f*cosf(a-.8f));
   planet_quad(u,w,wt,ut,ink);queue_triangle(camera(&game,ut),camera(&game,wt),camera(&game,peak),mix_rgb(ink,RGB(118,140,84),.22f));
  }
  if(drawcount>1600)flush_meshes();
 }flush_meshes();
}
