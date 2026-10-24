/* World anchored items and combat boundaries. Shared by main and rear cameras. */
static void race_world_line(Vec3 a,Vec3 b,unsigned ink);
static void race_projectile_art(void);
static void race_pickup_art(void){
 SpaceRace *r=&game.race;
 for(int k=0;k<RA_PICKUPS;k++){RacePickup *p=&r->arcade.pickup[k];
  float pickup_limit=r->layout==4?6000.f:23000.f;if((p->cooldown>0&&p->cooldown<4.8f)||length(sub(p->pos,camera_origin(&game)))>pickup_limit||!render_sphere_visible(p->pos,260))continue;
  Vec3 centre=p->pos;float size=1;if(p->cooldown>0){float age=(5-p->cooldown)/.2f,near=1000;Vec3 collector=centre;for(int id=0;id<=r->rivals;id++){Vec3 at=id?r->pilot[id-1].pos:game.pos;float d=length(sub(at,centre));if(d<near){near=d;collector=at;}}centre=add(centre,mul(sub(collector,centre),age));size=1-age;}
  RaceGate *q=&r->gates[p->section];float spin=r->elapsed*.6f+k*.15f;
  for(int i=0;i<6;i++){float a=spin+i*1.04719755f,b=a+1.04719755f;Vec3 u=race_ring_vertex(centre,q->right,q->up,q->normal,a,240*size,0),v=race_ring_vertex(centre,q->right,q->up,q->normal,b,240*size,0);race_world_line(u,v,RGB(108,255,229));}
  Vec3 v[4];for(int i=0;i<4;i++)v[i]=race_ring_vertex(centre,q->right,q->up,q->normal,spin+i*1.5707963f,115*size,0);
  queue_triangle(camera(&game,v[0]),camera(&game,v[1]),camera(&game,v[2]),RGB(255,239,159));queue_triangle(camera(&game,v[0]),camera(&game,v[2]),camera(&game,v[3]),RGB(255,239,159));
 }
}
/* The centre stays clear. The nearby physical boundary is composited after
 * world meshes, so its translucent grid never paints over foreground hulls. */
static void race_corridor_art(void){}
#include "erl-boundary-art.h"
#include "erl-holo-track-art.h"
static void race_service_art(void){
 SpaceRace *r=&game.race;if(r->layout==4)return;int first=r->gate>0?r->gate-1:0;
 for(int k=first;k<RACE_GATES&&k<first+3;k++)if(k%4==2){
  RaceTrackFrame service=race_repair_frame(r,k);RaceGate aligned=r->gates[k];aligned.normal=service.ahead;aligned.right=service.right;aligned.up=service.up;RaceGate *q=&aligned;
  Vec3 centre=service.centre;
  if(!render_sphere_visible(centre,2400))continue;
  float service_distance=length(sub(centre,camera_origin(&game)));if(service_distance>24000)continue;
  /* Authored rectangular service tunnel: solid steel ribs and footings,
   * continuous pink energy rails, inset grid and bright corner emitters. */
  int frames=service_distance>12000?4:8;
  for(int frame=0;frame<frames;frame++){
   Vec3 c=add(centre,mul(q->normal,-RA_REPAIR_HALF_LENGTH+frame*(RA_REPAIR_HALF_LENGTH*2/(frames-1))));
   for(int side=-1;side<=1;side+=2){
    race_block(q,add(c,mul(q->right,side*1080)),28,1090,30,RGB(82,101,117));
    race_block(q,add(c,mul(q->up,side*1080)),1110,28,30,RGB(114,135,148));
    race_world_line(add(c,add(mul(q->right,side*1000),mul(q->up,-1000))),add(c,add(mul(q->right,side*1000),mul(q->up,1000))),RGB(255,96,211));
    race_world_line(add(c,add(mul(q->up,side*1000),mul(q->right,-1000))),add(c,add(mul(q->up,side*1000),mul(q->right,1000))),RGB(255,96,211));
    for(int other=-1;other<=1;other+=2){Vec3 corner=add(c,add(mul(q->right,side*1020),mul(q->up,other*1020)));race_block(q,corner,30,30,48,RGB(255,176,239));if(!frame)race_block(q,add(corner,mul(q->up,-160)),140,45,90,RGB(111,130,145));}
   }
   if(frame==frames-1)for(int grid=-3;grid<=3;grid++){
    race_world_line(add(c,add(mul(q->right,grid*250),mul(q->up,-975))),add(c,add(mul(q->right,grid*250),mul(q->up,975))),RGB(132,48,117));
    race_world_line(add(c,add(mul(q->up,grid*250),mul(q->right,-975))),add(c,add(mul(q->up,grid*250),mul(q->right,975))),RGB(132,48,117));
   }
  }
  for(int corner=0;corner<4;corner++){Vec3 offset=add(mul(q->right,corner&1?1000:-1000),mul(q->up,corner&2?1000:-1000));race_world_line(add(centre,add(offset,mul(q->normal,-RA_REPAIR_HALF_LENGTH))),add(centre,add(offset,mul(q->normal,RA_REPAIR_HALF_LENGTH))),RGB(213,76,180));}
  {float z=-RA_REPAIR_HALF_LENGTH+fmodf(r->elapsed*.65f+k*.17f,1)*RA_REPAIR_HALF_LENGTH*2;Vec3 sweep=add(centre,mul(q->normal,z));for(int grid=-2;grid<=2;grid++){race_world_line(add(sweep,add(mul(q->right,grid*400),mul(q->up,-990))),add(sweep,add(mul(q->right,grid*400),mul(q->up,990))),grid?RGB(211,72,174):RGB(255,171,230));}}
  Vec3 label=camera(&game,add(centre,add(mul(q->up,1180),mul(q->normal,-RA_REPAIR_HALF_LENGTH))));if(label.z>100){Point p=project(label);if(p.x>40&&p.x<W-40&&p.y>46&&p.y<205){rect(p.x-38,p.y-6,76,13,RGB(39,20,48));text_px(p.x-30,p.y-3,RGB(255,148,223),"REPAIR");}}
 }
}
/* Racer protection tint is drawn on the actual roster geometry. */
static void race_ghost_art(void){}
static void race_rival_instruments(void){
 SpaceRace *r=&game.race;
 for(int i=0;i<r->rivals;i++){RaceRival *p=&r->pilot[i];RaceItemActor *a=&r->arcade.actor[i+1];
  if(p->disabled||a->eliminated||p->gate>=RACE_GATES)continue;
  Vec3 v=camera(&game,p->pos);float distance=length(v);if(v.z<50||distance>8500)continue;Point point=project(v);
  if(point.x<10||point.x>=W-10||point.y<28||point.y>205)continue;
  unsigned d=(unsigned)fminf(65534,distance);int at=point.y*W+point.x;
  /* World depth uses reciprocal encoding; a hidden hull gets no label. */
  unsigned encoded=mega_depth_encoding?mega_encode_depth(v.z):d;
  if(surface_depth[at]+80<encoded)continue;
  int y=point.y-(int)fminf(40,p->radius*proj_focal/v.z)-6;
  /* Quiet damage cue, not a bright floating UI strip on every full hull. */
  int engaged=r->arcade.lock==i+1;
  if(a->health<RA_HEALTH&&(distance<6000||engaged)){
   int fill=(int)(20.f*fmaxf(0,a->health)/RA_HEALTH);
   line(point.x-10,y,point.x+9,y,RGB(29,45,51));
   if(fill>0)line(point.x-10,y,point.x-11+fill,y,engaged?RGB(128,176,157):RGB(68,100,96));
  }
  if(a->shield>0){unsigned ink=RGB(79,157,255);int radius=8+(int)(p->radius*proj_focal/v.z);
   for(int s=0;s<12;s++){float t=s*.5235988f,b=t+.5235988f;line(point.x+cosf(t)*radius,point.y+sinf(t)*radius,point.x+cosf(b)*radius,point.y+sinf(b)*radius,ink);}
  }
 }
}
