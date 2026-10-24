/* Fleet materials share the station's guarded hardware pass. Expanded swizzled pixel tiles stay
 * resident; only sunlit vertex colours change per face; no per-face texture rebakes. */
#define FLEET_TILE_COUNT (sizeof(fleet_tiles)/sizeof(*fleet_tiles))
static unsigned fleet_ge_tiles[FLEET_TILE_COUNT][1408] __attribute__((aligned(16)));
static unsigned fleet_ge_adder[52][1024] __attribute__((aligned(16)));
static DrawTri fleet_ge_prior[6144];static int fleet_ge_prior_count,fleet_ge_prior_overflow;
typedef struct {unsigned color;float x,y,z;} FleetFlatVertex;
static int fleet_ge_ready;
static int fleet_ge_bound=-1,fleet_ge_level=-1;
static unsigned fleet_ge_calls,fleet_ge_triangles,fleet_ge_flat;
static void fleet_ge_assets_prepare(void){
 if(fleet_ge_ready||!ship_art_pixels)return;unsigned palette[16];for(int i=0;i<16;i++)palette[i]=0xff000000u|ship_art_ink(WHITE,i);
 for(unsigned tile=0;tile<FLEET_TILE_COUNT;tile++)for(int level=0;level<3;level++){
  int size=32>>level,rows=size<8?8:size,stride=size<16?16:size,source=level==0?0:level==1?512:640,dest=level==0?0:level==1?1024:1280;
  for(int y=0;y<rows;y++)for(int x=0;x<stride;x++){
   int xx=x<size?x:size-1,yy=y<size?y:size-1,index=yy*size+xx,at=(y>>3)*stride*8+(x>>2)*32+(y&7)*4+(x&3);
   unsigned packed=fleet_tiles[tile][source+(index>>1)];fleet_ge_tiles[tile][dest+at]=palette[(packed>>((index&1)*4))&15];
  }
 }
 for(int tile=0;tile<52;tile++)for(int y=0;y<32;y++)for(int x=0;x<32;x++){int index=y*32+x,packed=ship_art_pixels[tile][index>>1],at=(y>>3)*256+(x>>2)*32+(y&7)*4+(x&3);fleet_ge_adder[tile][at]=palette[(packed>>((index&1)*4))&15];}
 sceKernelDcacheWritebackRange(fleet_ge_tiles,sizeof(fleet_ge_tiles));sceKernelDcacheWritebackRange(fleet_ge_adder,sizeof(fleet_ge_adder));fleet_ge_ready=1;
}
static int fleet_ge_material(int material){return material>=8192&&material<8192+(int)(sizeof(fleet_faces)/sizeof(*fleet_faces))||material>=FLEET_QUAD_BASE&&material<FLEET_QUAD_BASE+2*(int)FLEET_TILE_COUNT||material>=4096&&material<4240;}
static int fleet_ge_view_ready(void){
 if(!flight_cached_frame||!ion_gu_ready||!fleet_ge_ready||page!=FLIGHT||clipy0>=0||game.docked||game.dead||game.planet>=0||game.approach>=0||game.dock_stage>=2||game.police_stop||game.rift_report||thargoid_active||race_ready(&game)||opening_dream(&game)||planet_entry_body||planet_orbit_veil>0||((hud_mode!=0||hud_hidden)&&!hc_full_console_visible())||high_contrast)return 0;
 for(int i=0;i<NPC_COUNT;i++)if(game.npc[i].alive){float r=npc_visual_radius(&game.npc[i]);if(render_sphere_visible(game.npc[i].pos,r)&&render_pixel_radius(game.npc[i].pos,r)>12&&!occluded(game.npc[i].pos))return 1;}
 for(int i=0;i<discovery_count();i++){Vec3 p=discovery_position(i);if(render_sphere_visible(p,3500)&&length(sub(p,game.pos))<65000&&!occluded(p))return 1;}
 for(int hub=1;hub<HUB_COUNT;hub++){Vec3 p=hub_position(&game,hub);if(length(sub(p,game.pos))<18000&&render_sphere_visible(p,station_profile_for(&game,hub).radius*2)&&!occluded(p))return 1;}
 return third_person!=0;
}
static void fleet_ge_record(void){
 if(!fleet_ge_active_frame||!surface_depth_on||!mc_depth_active)return;
 int count=drawcount;if(count>6144-fleet_ge_prior_count){count=6144-fleet_ge_prior_count;fleet_ge_prior_overflow=1;}
 memcpy(fleet_ge_prior+fleet_ge_prior_count,drawlist,count*sizeof(*drawlist));fleet_ge_prior_count+=count;
}
static void fleet_ge_previous_depth(void){
 /* Re-submit prior opaque geometry with colour writes masked. This preserves
  * foreground visibility across flushes without depth-buffer image uploads
  * or a costly full-viewport depth conversion on the CPU. */
 sceGuDisable(GU_TEXTURE_2D);sceGuPixelMask(0xffffffff);
 if(fleet_ge_prior_count){FleetFlatVertex*v=sceGuGetMemory(fleet_ge_prior_count*3*sizeof(*v));
  for(int i=0;i<fleet_ge_prior_count;i++)for(int k=0;k<3;k++){Point p=fleet_ge_prior[i].p[k];v[i*3+k]=(FleetFlatVertex){0xffffffff,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}
  sceGuDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,fleet_ge_prior_count*3,0,v);
 }
 sceGuPixelMask(0);sceGuEnable(GU_TEXTURE_2D);
}
static void fleet_ge_draw(DrawTri*t){
 const unsigned char*uv=0;int tile=0,adder=t->material>=4096&&t->material<4240;
 static const unsigned char quv[2][6]={{0,0,31,0,31,31},{0,0,31,31,0,31}};
 if(adder){int face=t->material-4096;uv=ship_art_uv[face];tile=ship_art_panel[face];}
 else if(t->material>=8192&&t->material<FLEET_QUAD_BASE){const FleetFace*f=&fleet_faces[t->material-8192];uv=f->uv;tile=f->tile;}
 else {tile=(t->material-FLEET_QUAD_BASE)/2;uv=quv[(t->material-FLEET_QUAD_BASE)&1];}
 sceGuEnable(GU_TEXTURE_2D);int key=tile+(adder?FLEET_TILE_COUNT:0);
 if(key!=fleet_ge_bound){sceGuTexMode(GU_PSM_8888,adder?0:2,0,1);sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);unsigned*data=adder?fleet_ge_adder[tile]:fleet_ge_tiles[tile];sceGuTexImage(0,32,32,32,data);
  if(!adder){sceGuTexImage(1,16,16,16,data+1024);sceGuTexImage(2,8,8,16,data+1280);}sceGuTexFilter(GU_NEAREST_MIPMAP_NEAREST,GU_NEAREST);sceGuTexFlush();fleet_ge_bound=key;fleet_ge_level=-1;
 }
 Point a=t->p[0],b=t->p[1],c=t->p[2];float span=fmaxf(fabsf(a.x-b.x)+fabsf(a.y-b.y),fmaxf(fabsf(b.x-c.x)+fabsf(b.y-c.y),fabsf(c.x-a.x)+fabsf(c.y-a.y)));
 int level=adder?0:span<12?2:span<25?1:0;if(level!=fleet_ge_level){sceGuTexLevelMode(GU_TEXTURE_CONST,level);fleet_ge_level=level;}
 typedef struct {float u,v;unsigned color;float x,y,z;} FleetGeVertex;
 FleetGeVertex*v=sceGuGetMemory(3*sizeof(*v));for(int k=0;k<3;k++){Point p=t->p[k];v[k]=(FleetGeVertex){(uv[k*2]+.5f)/32.f,(uv[k*2+1]+.5f)/32.f,0xff000000u|t->color,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}
 sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,v);fleet_ge_triangles++;
}
static void fleet_ge_draw_flat(DrawTri*t){
  sceGuDisable(GU_TEXTURE_2D);FleetFlatVertex*v=sceGuGetMemory(3*sizeof(*v));for(int k=0;k<3;k++){Point p=t->p[k];v[k]=(FleetFlatVertex){0xff000000u|t->color,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}
 sceGuDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,3,0,v);fleet_ge_flat++;
}
static void fleet_ge_depth_triangle(DrawTri*t){
 Point*p=t->p;float area=edge(p[0],p[1],p[2].x,p[2].y);if(fabsf(area)<.05f)return;
 float iz[3]={1.f/(p[0].z*area),1.f/(p[1].z*area),1.f/(p[2].z*area)},step=(p[2].y-p[1].y)*iz[0]+(p[0].y-p[2].y)*iz[1]+(p[1].y-p[0].y)*iz[2];
 int first=(int)fmaxf(surface_y_min,ceilf(fminf(p[0].y,fminf(p[1].y,p[2].y))-.5f)),last=(int)fminf(surface_y_max,floorf(fmaxf(p[0].y,fmaxf(p[1].y,p[2].y))-.5f));
 for(int y=first;y<=last;y++){float yy=y+.5f,left=1e9f,right=-1e9f;for(int k=0;k<3;k++){Point a=p[k],b=p[(k+1)%3];if((a.y<=yy&&b.y>yy)||(b.y<=yy&&a.y>yy)){float x=a.x+(yy-a.y)*(b.x-a.x)/(b.y-a.y);left=fminf(left,x);right=fmaxf(right,x);}}
  int x0=(int)fmaxf(0,ceilf(left-.5f)),x1=(int)fminf(W-1,floorf(right-.5f));float inv=edge(p[1],p[2],x0+.5f,yy)*iz[0]+edge(p[2],p[0],x0+.5f,yy)*iz[1]+edge(p[0],p[1],x0+.5f,yy)*iz[2];
  for(int x=x0;x<=x1;x++,inv+=step){if(inv<=0)continue;unsigned depth=mega_encode_depth(1.f/inv);if(depth<surface_depth[y*W+x])surface_depth[y*W+x]=depth;}
 }
}

/* Opaque faces can be grouped by material: GU depth resolves visibility.
 * One draw for an entire material group instead of one draw per triangle. */
static void fleet_ge_batch(unsigned char*handled){
 enum {FLAT_GROUP=(FLEET_TILE_COUNT+52)*3,GROUPS=FLAT_GROUP+1};int counts[GROUPS]={0},start[GROUPS],cursor[GROUPS],active[GROUPS],active_count=0;unsigned used[(GROUPS+31)/32]={0};short group[2048];unsigned short order[2048];const unsigned char*uvs[2048];
 static const unsigned char quv[2][6]={{0,0,31,0,31,31},{0,0,31,31,0,31}};
 for(int i=0;i<drawcount;i++){
  DrawTri*t=&drawlist[i];group[i]=-1;uvs[i]=0;if(t->material==0){group[i]=FLAT_GROUP;counts[FLAT_GROUP]++;used[FLAT_GROUP>>5]|=1u<<(FLAT_GROUP&31);continue;}if(!fleet_ge_material(t->material))continue;
  int tile,adder=t->material>=4096&&t->material<4240;
  if(adder){int f=t->material-4096;tile=ship_art_panel[f]+FLEET_TILE_COUNT;uvs[i]=ship_art_uv[f];}
  else if(t->material>=8192&&t->material<FLEET_QUAD_BASE){const FleetFace*f=&fleet_faces[t->material-8192];tile=f->tile;uvs[i]=f->uv;}
  else {tile=(t->material-FLEET_QUAD_BASE)/2;uvs[i]=quv[(t->material-FLEET_QUAD_BASE)&1];}
  Point a=t->p[0],b=t->p[1],c=t->p[2];float span=fmaxf(fabsf(a.x-b.x)+fabsf(a.y-b.y),fmaxf(fabsf(b.x-c.x)+fabsf(b.y-c.y),fabsf(c.x-a.x)+fabsf(c.y-a.y)));
  int level=adder?0:span<12?2:span<25?1:0;if(tile<0||tile>=(int)(FLEET_TILE_COUNT+52))continue;group[i]=tile*3+level;counts[group[i]]++;used[group[i]>>5]|=1u<<(group[i]&31);
 }
 /* Preserve ascending material order while visiting only materials actually
  * submitted this frame. Larger authored atlases no longer add empty scans. */
 int total=0;for(int word=0;word<(GROUPS+31)/32;word++){unsigned bits=used[word];while(bits){int g=word*32+__builtin_ctz(bits);bits&=bits-1;active[active_count++]=g;start[g]=cursor[g]=total;total+=counts[g];}}
 for(int i=0;i<drawcount;i++)if(group[i]>=0)order[cursor[group[i]]++]=i;
 typedef struct {float u,v;unsigned color;float x,y,z;} FleetBatchVertex;
 for(int ag=0;ag<active_count;ag++){int g=active[ag];
  int flat=g==FLAT_GROUP,n=counts[g],key=g/3,level=g%3;
  if(sceGuCheckList()+n*(flat?48:72)+512>480000){for(int k=0;k<n;k++)handled[order[start[g]+k]]=1;continue;}
  if(flat){sceGuDisable(GU_TEXTURE_2D);FleetFlatVertex*v=sceGuGetMemory(n*3*sizeof(*v));
   for(int k=0;k<n;k++){int id=order[start[g]+k];DrawTri*t=&drawlist[id];handled[id]=2;for(int j=0;j<3;j++){Point p=t->p[j];v[k*3+j]=(FleetFlatVertex){0xff000000u|t->color,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}}
   sceGuDrawArray(GU_TRIANGLES,GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,n*3,0,v);fleet_ge_flat+=n;continue;
  }
  int adder=key>=FLEET_TILE_COUNT,tile=adder?key-FLEET_TILE_COUNT:key;unsigned*data=adder?fleet_ge_adder[tile]:fleet_ge_tiles[tile];sceGuEnable(GU_TEXTURE_2D);sceGuTexMode(GU_PSM_8888,adder?0:2,0,1);sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);sceGuTexImage(0,32,32,32,data);
  if(!adder){sceGuTexImage(1,16,16,16,data+1024);sceGuTexImage(2,8,8,16,data+1280);}sceGuTexFilter(GU_NEAREST_MIPMAP_NEAREST,GU_NEAREST);sceGuTexLevelMode(GU_TEXTURE_CONST,level);sceGuTexFlush();
  FleetBatchVertex*v=sceGuGetMemory(n*3*sizeof(*v));for(int k=0;k<n;k++){int id=order[start[g]+k];DrawTri*t=&drawlist[id];const unsigned char*uv=uvs[id];handled[id]=2;for(int j=0;j<3;j++){Point p=t->p[j];v[k*3+j]=(FleetBatchVertex){(uv[j*2]+.5f)/32.f,(uv[j*2+1]+.5f)/32.f,0xff000000u|t->color,(p.x-proj_ox)*p.z/proj_focal,(proj_oy-p.y)*p.z/proj_focal,p.z};}}
  sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,n*3,0,v);fleet_ge_triangles+=n;
 }
}
