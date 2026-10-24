#include "mega-hull-geometry.h"
static unsigned mega_art_tint(unsigned base,int shade){
 static const int n[4]={100,78,124,145};int k=n[shade&3];
 return RGB((base&255)*k/100,((base>>8)&255)*k/100,((base>>16)&255)*k/100);
}
static Vec3 mega_art_point(Vec3 p,float yaw,float roll,float scale,Vec3 pos){if(player_mesh_transform){Vec3 r,u,a;flight_camera_basis(&r,&u,&a);Vec3 world=add(pos,mul(add(add(mul(r,-p.x),mul(u,p.y)),mul(a,-p.z)),scale));return camera(&game,world);}return add(pos,mul(rotate(p,yaw,roll),scale));}
static void mega_art_block(const MegaHullPart *b,float yaw,float roll,float scale,Vec3 pos,unsigned base){
 /* Taper each authored module toward its bow.  The same cheap eight-vertex
  * solid now reads as an armoured spacecraft section instead of a crate,
  * while preserving the carefully bounded PSP preview silhouettes. */
 Vec3 p[8];for(int i=0;i<8;i++){float nose=(i&4)?.62f:1.f;Vec3 q={b->x+((i&1)?b->hx:-b->hx)*nose,b->y+((i&2)?b->hy:-b->hy)*(.78f+.22f*nose),b->z+((i&4)?b->hz:-b->hz)};p[i]=mega_art_point(q,yaw,roll,scale,pos);}
 static const unsigned char f[12][3]={{0,1,3},{0,3,2},{4,6,7},{4,7,5},{0,4,5},{0,5,1},{2,3,7},{2,7,6},{0,2,6},{0,6,4},{1,5,7},{1,7,3}};
 if(race_is_active(&game)&&page==FLIGHT){for(int i=0;i<12;i++)queue_triangle(p[f[i][0]],p[f[i][1]],p[f[i][2]],mega_art_tint(base,(b->shade+i/2)%4));return;}
 for(int i=0;i<12;i+=2){Vec3 a=p[f[i][0]],q=p[f[i][1]],c=p[f[i][2]],d=p[f[i+1][2]];
  Vec3 n=norm((Vec3){(q.y-a.y)*(c.z-a.z)-(q.z-a.z)*(c.y-a.y),(q.z-a.z)*(c.x-a.x)-(q.x-a.x)*(c.z-a.z),(q.x-a.x)*(c.y-a.y)-(q.y-a.y)*(c.x-a.x)});
  if(dot(n,a)>0)continue;
  Vec3 sun=norm((Vec3){-.4f,.75f,-.55f});if(player_mesh_transform){Vec3 centre=camera(&game,game.pos),s=camera(&game,game.bodies[0].pos);sun=norm(sub(s,centre));}
  unsigned ink=ship_art_light(base,n,sun,norm(add(sun,norm(mul(a,-1)))),i);
  int tile=b->shade==3?(i==2?FLEET_ENGINE:FLEET_RADIATOR):b->shade==2?(i==6||i==0?FLEET_GLASS:FLEET_SERVICE):b->shade==1?FLEET_CARGO:FLEET_ARMOUR;
  fleet_panel_quad(a,q,c,d,ink,tile,1);
 }
}
static void mega_hull_preview(int id,float yaw,float roll,unsigned base){
 if(id<0||id>=12)return;const MegaHullArt *h=&mega_hull_art[id];
 float radius=sqrtf(h->hx*h->hx+h->hy*h->hy+h->hz*h->hz);float scale=(34.f*320.f)/(radius*(240.f+34.f));Vec3 pos={0,0,320};
 for(int i=0;i<h->parts;i++)mega_art_block(&h->part[i],yaw,roll,scale,pos,base);
}
static Vec3 mega_npc_point(const NPC*n,Vec3 local){
 const MegaHullArt*h=&mega_hull_art[n->mega_hull-PLAYER_SHIP_MEGA_FIRST];float scale=n->radius/sqrtf(h->hx*h->hx+h->hy*h->hy+h->hz*h->hz);
 Vec3 side=norm((Vec3){n->dir.z,0,-n->dir.x}),up=norm((Vec3){-n->dir.y*side.z,n->dir.z*side.x-n->dir.x*side.z,n->dir.y*side.x});
 return add(n->pos,mul(add(mul(side,-local.x),add(mul(up,local.y),mul(n->dir,-local.z))),scale));
}
static void mega_npc_model(const NPC*n,unsigned paint){
 if(!player_ship_is_mega(n->mega_hull))return;const MegaHullArt*h=&mega_hull_art[n->mega_hull-PLAYER_SHIP_MEGA_FIRST];
 if(paint!=WHITE)paint=RGB(85+(paint&255)*.28f,98+((paint>>8)&255)*.28f,111+((paint>>16)&255)*.28f);
 int detailed=!high_contrast&&render_pixel_radius(n->pos,n->radius)>14.f;Vec3 sun=norm(sub(game.bodies[0].pos,n->pos)),half=norm(add(sun,norm(sub(camera_origin(&game),n->pos))));
 static const unsigned char faces[6][4]={{0,1,3,2},{4,6,7,5},{0,4,5,1},{2,3,7,6},{0,2,6,4},{1,5,7,3}};
 for(int part=0;part<h->parts;part++){const MegaHullPart*b=&h->part[part];Vec3 v[8];
  for(int k=0;k<8;k++){float nose=k&4?.62f:1.f;Vec3 q={b->x+((k&1)?b->hx:-b->hx)*nose,b->y+((k&2)?b->hy:-b->hy)*(.78f+.22f*nose),b->z+((k&4)?b->hz:-b->hz)};v[k]=mega_npc_point(n,q);}
  for(int f=0;f<6;f++){Vec3 a=v[faces[f][0]],q=v[faces[f][1]],c=v[faces[f][2]],d=v[faces[f][3]],u=sub(q,a),w=sub(c,a);Vec3 normal=norm((Vec3){u.y*w.z-u.z*w.y,u.z*w.x-u.x*w.z,u.x*w.y-u.y*w.x});if(dot(normal,sub(camera_origin(&game),a))<=0)continue;
   unsigned ink=ship_art_light(paint,normal,sun,half,f);int tile=b->shade==3?(f==1?FLEET_ENGINE:FLEET_RADIATOR):b->shade==2?(f==0||f==3?FLEET_GLASS:FLEET_SERVICE):b->shade==1?FLEET_CARGO:FLEET_ARMOUR;
   if(detailed&&drawcount<1600)fleet_panel_quad(camera(&game,a),camera(&game,q),camera(&game,c),camera(&game,d),ink,tile,1);
   else {queue_triangle(camera(&game,a),camera(&game,q),camera(&game,c),ink);queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),ink);}
  }
 }
}
#undef MB
