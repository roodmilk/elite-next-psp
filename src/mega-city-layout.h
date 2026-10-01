#ifndef ELITE_MEGA_CITY_LAYOUT_H
#define ELITE_MEGA_CITY_LAYOUT_H
#include <math.h>
/* One bounded, deterministic solid list shared by rendering and navigation.
 * Coordinates are relative to the main station, NOT an enclosing solid ball.
 * Capitals are non-rotating cities; the docking core retains five front ports. */
#define MEGA_BLOCK_MAX 72
#include "mega-city-geometry.h"
typedef struct { Vec3 c,e; unsigned char district,kind,shape; } MegaBlock;
typedef struct { MegaBlock b[MEGA_BLOCK_MAX]; int n,system; unsigned seed; float time; } MegaCity;
static MegaCity mega_city_cache={.system=-1};
static inline void mega_tenders(MegaCity *m,float time){
 if(m->n<3||m->time==time)return;m->time=time;
 for(int i=0;i<3;i++){float a=time*.008f+i*2.0944f;m->b[m->n-3+i].c=(Vec3){cosf(a)*9300,sinf(a)*6300,-500};}
}
static inline void mega_block(MegaCity *m,Vec3 c,Vec3 e,int district,int kind){
 if(m->n<MEGA_BLOCK_MAX)m->b[m->n++]=(MegaBlock){c,e,district,kind,MC_BOX};
}
static inline void mega_shape(MegaCity *m,Vec3 c,Vec3 e,int district,int kind,int shape){
 if(m->n>=MEGA_BLOCK_MAX)return;
 mega_block(m,c,e,district,kind);m->b[m->n-1].shape=shape;
}
static inline const MegaCity *mega_city_for(const Game *g){
 MegaCity *m=&mega_city_cache;unsigned seed=station_profile_hash(g->system,0);
 if(m->system==g->system&&m->seed==seed){if(m->n)mega_tenders(m,g->time);return m;}
 m->n=0;m->system=g->system;m->seed=seed;
 if(station_class(g)!=STATION_MEGA)return m;
 StationProfile p=station_profile_for(g,0);
 mega_block(m,(Vec3){0,0,0},(Vec3){p.radius,p.radius,p.half},0,0);
 /* Eight neighbourhoods, three differently proportioned stepped towers each.
    1-3 kilometre boulevards separate them; every structure sits behind the
    forward docking plane. Tall blocks, not a scaled-up solid Coriolis. */
 for(int d=0;d<8;d++){
  unsigned h=station_profile_hash(g->system,d+20);
  float x=(d%4-1.5f)*4200.f,y=(d<4?-1:1)*(2700.f+(h%4)*170.f);
  float z=1100.f+((h>>5)%5)*420.f;
  float w=420.f+(h>>9)%240,ht=900.f+(h>>17)%1000;
  int style=(seed>>12)%4,roof=(d+style)%3;
  int tower=((h>>24)+style)%4;
  mega_shape(m,(Vec3){x,y,z},(Vec3){w,ht,650},d,1,tower==0?MC_OCTAGON:tower==1?MC_BEVEL:MC_BOX);
  mega_shape(m,(Vec3){x,y+ht+180,z+100},(Vec3){w*.68f,180,450},d,2,roof==0?MC_DOME:roof==1?MC_PYRAMID:MC_OCTAGON);
  mega_shape(m,(Vec3){x,y+ht+510,z+170},(Vec3){55,150,70},d,3,MC_OCTAGON);
  int habitat=(d+style)%4;
  mega_shape(m,(Vec3){x+1000,y-ht*.42f,z+1350},habitat<2?(Vec3){380,300,ht*.62f}:(Vec3){280,ht*.62f,440},d,1,habitat<2?MC_POD:MC_BEVEL);
  float orb=450.f+((h>>21)%171);
  mega_shape(m,(Vec3){x-900,y+ht*.25f,z+2150},(Vec3){orb,orb,orb},d,1,MC_SPHERE);
  /* Service viaduct at the REAR, leaving front/back and vertical passages. */
  mega_block(m,(Vec3){x,y-ht-120,4700},(Vec3){1600,100,210},d,4);
 }
 mega_block(m,(Vec3){0,0,4800},(Vec3){8300,125,180},0,4);
 mega_block(m,(Vec3){0,0,2300},(Vec3){160,150,2300-p.half},0,4);
 for(int d=0;d<4;d++){
  float x=(d-1.5f)*4200;
  mega_block(m,(Vec3){x,0,4800},(Vec3){125,4900,180},d,4);
 }
 /* A capital's dominant crown changes its WHOLE skyline. Broad saucers,
    twin globes, a rocket palace or an asymmetric liner replace the uniform
    row-of-towers impression. All are aft of the five front entrances. */
 int crown=(seed>>18)&3;
 mega_shape(m,(Vec3){0,2300,5400},(Vec3){300,2300,600},0,4,MC_BEVEL);
 if(crown==0){
  mega_shape(m,(Vec3){0,4300,7900},(Vec3){7500,1500,2800},0,6,MC_SPHERE);
  mega_shape(m,(Vec3){0,5700,7900},(Vec3){2500,1600,1900},0,6,MC_DOME);
 }else if(crown==1){
  for(int s=-1;s<=1;s+=2)mega_shape(m,(Vec3){s*4600.f,3900,7900},(Vec3){3000,3000,3000},1,6,MC_SPHERE);
  mega_shape(m,(Vec3){0,3900,7900},(Vec3){4600,300,400},1,4,MC_BEVEL);
 }else if(crown==2){
  mega_shape(m,(Vec3){0,5900,8000},(Vec3){2300,3200,2200},2,6,MC_BEVEL);
  mega_shape(m,(Vec3){0,10300,8000},(Vec3){2300,1200,2200},2,6,MC_PYRAMID);
 }else{
  mega_shape(m,(Vec3){-1500,4000,8200},(Vec3){8000,1700,3000},3,6,MC_POD);
  mega_shape(m,(Vec3){4500,5600,8200},(Vec3){2400,1800,2200},3,6,MC_DOME);
 }
 for(int i=0;i<3;i++)mega_block(m,(Vec3){0,0,0},(Vec3){550,130,190},i,5);
 m->time=-1;mega_tenders(m,g->time);
 return m;
}
/* Slab intersection catches boost-speed crossings, not just end points. */
static inline int mega_box_hit(Vec3 a,Vec3 b,const MegaBlock *box,float pad,float *hit){
 float av[3]={a.x,a.y,a.z},bv[3]={b.x,b.y,b.z};
 float cv[3]={box->c.x,box->c.y,box->c.z},ev[3]={box->e.x+pad,box->e.y+pad,box->e.z+pad};
 float lo=0,hi=1;
 for(int k=0;k<3;k++){
  float v=bv[k]-av[k],q=av[k]-cv[k];
  if(fabsf(v)<.00001f){if(fabsf(q)>ev[k])return 0;}
  else {float t=(-ev[k]-q)/v,u=(ev[k]-q)/v;if(t>u){float tmp=t;t=u;u=tmp;}
   lo=fmaxf(lo,t);hi=fminf(hi,u);if(lo>hi)return 0;}
 }
 /* Plane clipping shrinks the broad-phase box to the rendered faceted hull.
    Padding offsets planes by true world distance for NPC/docking clearance. */
 if(box->shape){
  const MegaGeometry *m=mega_geometry_for(box->shape);lo=0;hi=1;
  Vec3 qa=sub(a,box->c),qb=sub(b,box->c);
  for(int i=0;i<m->faces;i++){
   const MegaFace *f=&m->f[i];Vec3 n={f->normal.x/box->e.x,f->normal.y/box->e.y,f->normal.z/box->e.z};
   float limit=f->plane+(pad>0?pad*length(n):0),q=limit-dot(n,qa),v=dot(n,sub(qb,qa));
   if(fabsf(v)<.000001f){if(q<0)return 0;}
   else {float t=q/v;if(v>0)hi=fminf(hi,t);else lo=fmaxf(lo,t);if(lo>hi)return 0;}
  }
 }
 if(hit)*hit=lo;return 1;
}
static inline int mega_city_hit(const Game *g,Vec3 a,Vec3 b,float pad,float *hit){
 const MegaCity *m=mega_city_for(g);float best=2;
 for(int i=0;i<m->n;i++){float t;if(mega_box_hit(a,b,&m->b[i],pad,&t)&&t<best)best=t;}
 if(hit)*hit=best;return best<=1;
}
static inline float station_comms_range(const Game *g,int hub){
 return hub==0&&station_class(g)==STATION_MEGA?16000.f:hub==0&&station_class(g)==STATION_RICH?(g->upgrades&1)?12000.f:8000.f:(g->upgrades&1)?8000.f:2500.f;
}
#endif
