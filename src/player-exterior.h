/* Native mesh surface anchors, shared by engines and weapon emitters. */
static Vec3 player_local_world(Vec3 v){
 Vec3 r,u,a;flight_camera_basis(&r,&u,&a);
 return add(game.pos,mul(add(add(mul(r,v.x),mul(u,v.y)),mul(a,v.z)),1.2f));
}
static float player_exterior_radius(void){
 if(race_is_active(&game)||game.erl.result)return 200;
 int id=mega_hull_art_id(player_ships[game.ship].name);if(id<0)return render_mesh_radius(mesh_id(player_ships[game.ship].name))*1.2f;
 const MegaHullArt *h=&mega_hull_art[id];return sqrtf(h->hx*h->hx+h->hy*h->hy+h->hz*h->hz)*1.2f;
}
static int player_hardpoint(int side,int weapon,Vec3 *out){
 if(race_is_active(&game)||game.erl.result){*out=player_local_world(weapon?(Vec3){side*12,0,150}:erl_racer_engine(erl_racer_id(&game,0),side));return 1;}
 if(!strcmp(player_ships[game.ship].name,"NEEDLEFISH")){
  if(!weapon&&side<0)return 0; /* One authored offset engine, not a mirrored phantom. */
  *out=player_local_world(weapon?(Vec3){-12+side*2.f,0,120.6f}:!race_is_active(&game)&&fleet_hulls[34].mesh?fleet_hulls[34].outlet[0]:(Vec3){40.5f,0,-66.6f});return 1;
 }
 int art=mega_hull_art_id(player_ships[game.ship].name);
 int visual=mesh_id(player_ships[game.ship].name);
 if(!weapon&&!race_is_active(&game)&&art<0&&visual>=0&&visual<35){
  if(visual==4){*out=player_local_world((Vec3){side*17.f,0,-40.6f});return 1;}
  if(fleet_hulls[visual].mesh){const FleetHull*h=&fleet_hulls[visual];if(h->engines==1&&side<0)return 0;*out=player_local_world(h->outlet[side<0?0:h->engines-1]);return 1;}
 }
 if(art>=0){const MegaHullArt *h=&mega_hull_art[art];int chosen=-1;float best=-1e9f;
  for(int i=0;i<h->parts;i++){const MegaHullPart *b=&h->part[i];if(!weapon&&b->shade!=3)continue;float score=(weapon?-b->z+b->hz:b->z+b->hz)+side*b->x*.5f;if(score>best){best=score;chosen=i;}}
  if(chosen<0)chosen=0;const MegaHullPart *b=&h->part[chosen];float x=b->x+side*b->hx*.3f,z=b->z+(weapon?-b->hz-.6f:b->hz+.6f);
  *out=player_local_world((Vec3){-x,b->y,-z});return 1;
 }
 const Mesh *m=&meshes[mesh_id(player_ships[game.ship].name)];float width=0,ymin=1e9f,ymax=-1e9f;
 for(int i=0;i<m->vertices;i++){width=fmaxf(width,fabsf(m->v[i].x));ymin=fminf(ymin,m->v[i].y);ymax=fmaxf(ymax,m->v[i].y);}
 /* Intersect a fore/aft ray with real hull faces, rather than putting emitters
  * inside a bounding box. Narrow noses and asymmetric hulls get a fallback. */
 for(int attempt=0;attempt<3;attempt++){
  float x=side*width*(attempt==0?.32f:attempt==1?.14f:0),y=(ymin+ymax)*.5f,z=weapon?-1e9f:1e9f;int found=0;
  for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];Vec3 a=m->v[t->a],b=m->v[t->b],c=m->v[t->c];float d=(b.y-c.y)*(a.x-c.x)+(c.x-b.x)*(a.y-c.y);if(fabsf(d)<.0001f)continue;
   float u=((b.y-c.y)*(x-c.x)+(c.x-b.x)*(y-c.y))/d,v=((c.y-a.y)*(x-c.x)+(a.x-c.x)*(y-c.y))/d;
   if(u<-.0001f||v<-.0001f||u+v>1.0001f)continue;float hit=u*a.z+v*b.z+(1-u-v)*c.z;
   z=weapon?fmaxf(z,hit):fminf(z,hit);found=1;
  }
  if(found){*out=player_local_world((Vec3){x,y,z+(weapon?.6f:-.6f)});return 1;}
 }
 return 0;
}
/* Perspective-correct depth on every effect sample prevents beams and flame
 * painting over the hull or a foreground station. Near clipping is 3D. */
static void exterior_ray(Vec3 start,Vec3 end,unsigned ink,int radius){
 Vec3 a=camera(&game,start),b=camera(&game,end);if(a.z<5&&b.z<5)return;
 if(a.z<5)a=add(a,mul(sub(b,a),(5-a.z)/(b.z-a.z)));
 if(b.z<5)b=add(b,mul(sub(a,b),(5-b.z)/(a.z-b.z)));
 Point p=project(a),q=project(b);float dx=q.x-p.x,dy=q.y-p.y,lo=0,hi=1;
 /* Clip in screen space before stepping, so huge offscreen effects stay bounded. */
 float ds[4]={-dx,dx,-dy,dy},es[4]={p.x,W-1-p.x,p.y-view_top(),view_bot()-p.y};
 for(int k=0;k<4;k++){if(fabsf(ds[k])<.00001f){if(es[k]<0)return;continue;}float t=es[k]/ds[k];if(ds[k]<0)lo=fmaxf(lo,t);else hi=fminf(hi,t);if(lo>hi)return;}
 int n=(int)fminf(600,ceilf(fmaxf(fabsf(dx),fabsf(dy))*(hi-lo)));if(n<1)n=1;
 for(int i=0;i<=n;i++){float t=lo+(hi-lo)*i/n,inv=(1-t)/a.z+t/b.z;unsigned depth=surface_encode_depth(inv);int x=(int)(p.x+dx*t),y=(int)(p.y+dy*t);
  for(int oy=-radius+1;oy<radius;oy++)for(int ox=-radius+1;ox<radius;ox++)surface_depth_pixel(x+ox,y+oy,depth,ink);
 }
}
