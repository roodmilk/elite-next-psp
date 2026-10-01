/* Pulp-era architecture: large connected masses, not antenna satellites.
 * Same bounded faceted solids feed the renderer and swept collision. The
 * forward berth stays unobstructed; wheel holes are genuinely empty space. */
#ifndef ELITE_STATION_ARCHITECTURE_H
#define ELITE_STATION_ARCHITECTURE_H
static MegaCity station_arch_cache={.system=-1};
static inline Vec3 station_arch_rotate(Vec3 p,float angle){float c=cosf(angle),s=sinf(angle);return (Vec3){p.x*c-p.y*s,p.x*s+p.y*c,p.z};}
static inline void station_ring(MegaCity *m,float r,float z,float half,int district){
 for(int i=0;i<12;i++){
  float a=i*6.2831853f/12;
  mega_shape(m,(Vec3){cosf(a)*r,sinf(a)*r,z},(Vec3){r,r,half},district,6,MC_RING0+i);
 }
 /* Broad structural spokes meet the ring, but do not seal the quadrants. */
 mega_shape(m,(Vec3){0,0,z},(Vec3){r,half*.40f,half*.55f},district,4,MC_BEVEL);
 mega_shape(m,(Vec3){0,0,z},(Vec3){half*.40f,r,half*.55f},district,4,MC_BEVEL);
}
#include "rich-station-layout.h"
static inline const MegaCity *station_architecture_for(const Game *g){
 if(station_class(g)==STATION_MEGA)return mega_city_for(g);
 MegaCity *m=&station_arch_cache;StationProfile p=station_profile_for(g,0);
 if(m->system==g->system&&m->seed==p.seed)return m;
 m->system=g->system;m->seed=p.seed;m->n=0;
 float r=p.radius,h=p.half,s=1.f+((p.seed>>18)&7)*.025f;
 /* Readable front airlock block and opaque pressure bulkhead. */
 mega_shape(m,(Vec3){0,0,0},(Vec3){r,r,h},0,0,MC_DOCK);
 if(p.station_class==STATION_RICH){rich_station_layout(m,p);return m;}
 switch(p.family){
 case 0: /* Bonestell-like single wheel, with deep aft drive drum. */
  station_ring(m,r*2.4f*s,h*.9f,h*.42f,0);
  mega_shape(m,(Vec3){0,0,h*1.75f},(Vec3){r*.72f,r*.72f,h},0,6,MC_POD);break;
 case 1: /* Paired flattened saucers, linked by a thick central gallery. */
  for(int i=-1;i<=1;i+=2){
   mega_shape(m,(Vec3){i*r*1.15f,0,h*.9f},(Vec3){r*1.7f*s,r*.48f,h*1.45f},1,6,MC_SPHERE);
   mega_shape(m,(Vec3){i*r*1.15f,r*.48f,h*.9f},(Vec3){r*.55f,r*.48f,h*.65f},1,6,MC_DOME);
  }break;
 case 2: /* Glass globe citadel: one huge habitation sphere on a pedestal. */
  mega_shape(m,(Vec3){0,r*.85f,h},(Vec3){r*.7f,r*1.05f,h*.8f},2,4,MC_BEVEL);
  mega_shape(m,(Vec3){0,r*2.2f,h*1.1f},(Vec3){r*1.7f*s,r*1.7f*s,h*1.55f},2,6,MC_SPHERE);
  for(int i=-1;i<=1;i+=2)mega_shape(m,(Vec3){i*r*1.05f,-r*.65f,h*1.4f},(Vec3){r*.6f,r*.55f,h*1.3f},2,6,MC_POD);
  break;
 case 3: /* Rocket cathedral: tall tapered tower with stepped shoulders. */
  mega_shape(m,(Vec3){0,r*1.3f,h*.9f},(Vec3){r*1.2f,r*1.6f*s,h*1.25f},3,6,MC_BEVEL);
  mega_shape(m,(Vec3){0,r*3.15f,h*.9f},(Vec3){r*.9f,r*.9f,h},3,6,MC_PYRAMID);
  for(int i=-1;i<=1;i+=2)mega_shape(m,(Vec3){i*r*1.5f,r*.3f,h*.85f},(Vec3){r*.7f,r*1.15f,h*1.1f},3,6,MC_OCTAGON);
  break;
 case 4: /* Asymmetric orbital liner, a long hull and round observation wing. */
  mega_shape(m,(Vec3){-r*1.4f,r*.45f,h*1.4f},(Vec3){r*1.45f*s,r*.8f,h*2.f},4,6,MC_POD);
  mega_shape(m,(Vec3){r*.8f,-r*.25f,h*.75f},(Vec3){r*1.5f,r*.35f,h*.7f},4,4,MC_BEVEL);
  mega_shape(m,(Vec3){r*2.f,-r*.25f,h*.9f},(Vec3){r*.9f,r*.9f,h},4,6,MC_SPHERE);
  mega_shape(m,(Vec3){-r*1.4f,r*1.2f,h*1.1f},(Vec3){r*.7f,r*.6f,h*.9f},4,6,MC_DOME);break;
 case 5: /* Two full habitat wheels, visibly separated along the spin axis. */
  station_ring(m,r*1.95f*s,h*.7f,h*.35f,5);
  station_ring(m,r*2.3f*s,h*2.5f,h*.35f,5);
  mega_shape(m,(Vec3){0,0,h*1.6f},(Vec3){r*.55f,r*.55f,h*1.65f},5,4,MC_POD);break;
 case 6: /* Crystal palace: staggered faceted towers, not symmetric wings. */
  for(int i=-1;i<=1;i++){
   float y=r*(.6f+(i+1)*.5f),ht=r*(1.2f+(i+1)*.35f);
   mega_shape(m,(Vec3){i*r*1.25f,y,h},(Vec3){r*.8f,ht,h*1.2f},6,6,MC_BEVEL);
   mega_shape(m,(Vec3){i*r*1.25f,y+ht+r*.3f,h},(Vec3){r*.8f,r*.6f,h*1.2f},6,6,MC_PYRAMID);
  }break;
 default: /* Orbital palace: stacked onion globes with a wide lower bowl. */
  mega_shape(m,(Vec3){0,r*1.1f,h*.8f},(Vec3){r*2.15f*s,r*.85f,h*1.45f},7,6,MC_SPHERE);
  mega_shape(m,(Vec3){0,r*2.2f,h*.8f},(Vec3){r*1.2f,r*1.2f,h*1.15f},7,6,MC_SPHERE);
  mega_shape(m,(Vec3){0,r*3.45f,h*.8f},(Vec3){r*.7f,r*.8f,h*.8f},7,6,MC_DOME);
  mega_shape(m,(Vec3){0,r*4.05f,h*.8f},(Vec3){r*.18f,r*.5f,h*.18f},7,4,MC_PYRAMID);break;
 }
 return m;
}
static inline int station_architecture_hit(const Game *g,Vec3 a,Vec3 b,float pad,float *hit){
 const MegaCity *m=station_architecture_for(g);float best=2;
 for(int i=0;i<m->n;i++){float t;if(mega_box_hit(a,b,&m->b[i],pad,&t)&&t<best)best=t;}
 if(hit)*hit=best;return best<=1;
}
static inline float station_architecture_clearance(const Game *g){
 const MegaCity *m=station_architecture_for(g);float r=0;
 for(int i=0;i<m->n;i++){
  float x=fabsf(m->b[i].c.x)+m->b[i].e.x,y=fabsf(m->b[i].c.y)+m->b[i].e.y;
  r=fmaxf(r,sqrtf(x*x+y*y));
 }
 return r+350.f;
}
#endif
