static void panel(int x,int y,int w,int h);
static void wifi_ship(void){
 if(!wifi_contact_ready()||game.planet>=0||!wifi_contact_local()||wifi.remote.planet>=0||wifi.remote.jumping||wifi.remote.docked||occluded(wifi.display.pos))return;
 WifiPilot *p=&wifi.display;Vec3 v=camera(&game,p->pos);if(v.z<10)return;
 int id=mesh_id(player_ships[p->ship].name);
 /* Full pitch on the remote hull, unlike yaw-only ambient NPC geometry. */
 const Mesh *m=&meshes[id];Vec3 vertices[128];if(m->vertices>128)return;
 float cp=cosf(p->pitch),sp=sinf(p->pitch);
 for(int i=0;i<m->vertices;i++){Vec3 q=m->v[i];float y=q.y*cp+q.z*sp,z=q.z*cp-q.y*sp;q.y=y;q.z=z;vertices[i]=camera(&game,add(p->pos,rotate(q,p->yaw,p->roll)));}
 for(int i=0;i<m->triangles;i++){const MeshTri *t=&m->t[i];queue_triangle(vertices[t->a],vertices[t->b],vertices[t->c],livery_tint(p->paint,(i&1)?18:-18));}
}
static void wifi_overlay(void){} /* Normal HUD only; friend lives in Target Computer. */
static void wifi_screen(void){multiplayer_draw();}
/* Six-part block suit. Low, bounded geometry uses the surface depth buffer,
 * so other pilots are occluded by hills/buildings just like parked hulls. */
static void wifi_block(Vec3 origin,Vec3 offset,float yaw,float w,float d,float h,unsigned ink){
 Vec3 centre=add(origin,rotate(offset,yaw,0));
 Vec3 vertices[8];for(int i=0;i<8;i++)vertices[i]=camera(&game,add(centre,rotate((Vec3){(i&1)?w:-w,(i&4)?h:0,(i&2)?d:-d},yaw,0)));
 static const unsigned char face[12][3]={{0,2,3},{0,3,1},{4,5,7},{4,7,6},{0,1,5},{0,5,4},{2,6,7},{2,7,3},{0,4,6},{0,6,2},{1,3,7},{1,7,5}};
 for(int i=0;i<12;i++)queue_triangle(vertices[face[i][0]],vertices[face[i][1]],vertices[face[i][2]],livery_tint(ink,(i/2%3)*12-12));
}
static void wifi_surface_one(void){
 if(!wifi_contact_ready()||game.planet<1||!wifi_contact_local()||wifi.remote.planet!=game.planet||!wifi.remote.surface)return;
 WifiPilot *p=&wifi.display;
 Vec3 car=p->car;car.y=terrain_height(&game,car.x,car.z)+1;
 wifi_block(car,(Vec3){0,5,0},p->car_yaw,10,16,8,p->paint);
 wifi_block(car,(Vec3){0,13,-3},p->car_yaw,7,8,10,RGB(56,115,136));
 for(int i=0;i<4;i++)wifi_block(car,(Vec3){i&1?11:-11,0,i&2?10:-10},p->car_yaw,3,4,9,RGB(28,33,37));
 if(p->surface&&!p->sequence){Vec3 park=p->parked;float low=0;const Mesh *hull=&meshes[mesh_id(player_ships[p->ship].name)];for(int i=0;i<hull->vertices;i++)low=fminf(low,hull->v[i].y*.72f);park.y=terrain_height(&game,park.x,park.z)+2-low;shipmesh_stretched(mesh_id(player_ships[p->ship].name),park,0,0,.72f,1,p->paint,0);}
 if(p->surface==2&&!p->rover&&!p->sequence){
  Vec3 foot=p->pos;foot.y-=22;float sway=sinf(game.time*9)*fminf(3,p->speed*.07f);unsigned boots=RGB(35,43,55);
  float floor=terrain_height(&game,foot.x,foot.z)+.7f;Vec3 a={foot.x-8,floor,foot.z-6},b={foot.x+8,floor,foot.z-6},c={foot.x+8,floor,foot.z+6},d={foot.x-8,floor,foot.z+6};planet_quad(a,b,c,d,RGB(36,47,48));
  wifi_block(foot,(Vec3){-3,0,sway},p->yaw,2.5f,3,10,boots);wifi_block(foot,(Vec3){3,0,-sway},p->yaw,2.5f,3,10,boots);
  wifi_block(foot,(Vec3){0,10,0},p->yaw,6,3.5f,12,p->paint);
  wifi_block(foot,(Vec3){-8,10,-sway},p->yaw,2,2.5f,11,p->paint);wifi_block(foot,(Vec3){8,10,sway},p->yaw,2,2.5f,11,p->paint);
  wifi_block(foot,(Vec3){0,22,0},p->yaw,4.5f,4.5f,8,RGB(190,208,213));wifi_block(foot,(Vec3){0,24,4.6f},p->yaw,3.6f,.25f,4,RGB(34,174,201));
 }
 flush_meshes();
}
