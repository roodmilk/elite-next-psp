/* Sixteen middle-tier retro-future habitats. All structural coordinates are
 * aft of the common pressure bulkhead; exact solids feed draw/collision.
 * Decorations use independent hashes, never gameplay RNG or save fields. */
#ifndef ELITE_RICH_STATION_LAYOUT_H
#define ELITE_RICH_STATION_LAYOUT_H
static inline void rich_part(MegaCity *m,float x,float y,float z,float ex,float ey,float ez,int d,int shape){
 mega_shape(m,(Vec3){x,y,z},(Vec3){ex,ey,ez},d&7,6,shape);
}
static inline void rich_station_layout(MegaCity *m,StationProfile p){
 float r=p.radius,h=p.half,s=1.f+((p.seed>>18)&7)*.02f;
#define RP(x,y,z,a,b,c,d,shape) rich_part(m,(x)*r,(y)*r,(z)*h,(a)*r,(b)*r,(c)*h,d,shape)
 switch(p.family){
 case 0: /* Globe enclosed by one wheel: spacious, genuinely open quadrants. */
  station_ring(m,r*3.1f*s,h*1.7f,h*.32f,0);
  RP(0,0,2.4f,1.4f,1.4f,1.3f,0,MC_SPHERE);break;
 case 1: /* Botanical saucer: broad lower decks and a blue-green garden dome. */
  RP(0,.8f,2.4f,3.6f,.7f,1.5f,1,MC_SPHERE);
  RP(0,1.5f,2.4f,2.5f,1.5f,1.25f,1,MC_DOME);
  RP(0,-.6f,2.4f,1.05f,1.6f,1.1f,1,MC_OCTAGON);break;
 case 2: /* Two orbital wheels on a long axial passenger spine. */
  station_ring(m,r*2.4f*s,h*1.3f,h*.27f,2);
  station_ring(m,r*3.15f*s,h*4.2f,h*.30f,2);
  RP(0,0,2.7f,.7f,.7f,2.5f,2,MC_POD);break;
 case 3: /* Linked globes on a wide service gallery. */
  RP(0,.8f,2.5f,3.5f,.22f,.38f,3,MC_BEVEL);
  for(int i=-1;i<=1;i++)RP(i*2.25f,.8f,2.5f,i?1.1f:1.4f,i?1.1f:1.4f,1.35f,3+i,MC_SPHERE);
  RP(0,2.5f,2.5f,.42f,.7f,.45f,3,MC_DOME);break;
 case 4: /* Stepped rocket cathedral flanked by two habitation saucers. */
  RP(0,1.4f,2.5f,1.2f,2.0f,1.6f,4,MC_BEVEL);
  RP(0,3.9f,2.5f,1.2f,.8f,1.6f,4,MC_PYRAMID);
  for(int i=-1;i<=1;i+=2){RP(i*2.3f,.3f,2.8f,1.35f,.45f,1.25f,4+i,MC_SPHERE);RP(i*1.4f,.3f,2.8f,1.4f,.2f,.3f,4,MC_BEVEL);}break;
 case 5: /* Asymmetric liner with a large round observation wing. */
  RP(-1.1f,.7f,3.1f,2.1f,1.0f,2.4f,5,MC_POD);
  RP(1.1f,.1f,2.8f,2.1f,.25f,.6f,5,MC_BEVEL);
  RP(2.8f,.1f,2.8f,1.15f,1.15f,1.25f,5,MC_SPHERE);
  RP(-1.1f,1.7f,3.0f,1.3f,.8f,1.2f,5,MC_DOME);break;
 case 6: /* Three unequal art-deco towers with peaked crowns. */
  RP(0,.1f,2.8f,3.2f,.35f,.6f,6,MC_BEVEL);
  for(int i=-1;i<=1;i++){float ht=1.2f+(i+1)*.4f;RP(i*1.9f,ht*.6f,2.8f,.7f,ht,1.1f,6+i,MC_OCTAGON);RP(i*1.9f,ht*1.6f+.4f,2.8f,.7f,.4f,1.1f,6+i,MC_PYRAMID);}break;
 case 7: /* Stacked pleasure-palace saucers, tapered upper globe. */
  for(int i=0;i<3;i++)RP(0,.3f+i*1.0f,2.8f,3.4f-i*.7f,.5f,1.5f-i*.18f,i,MC_SPHERE);
  RP(0,3.0f,2.8f,1.15f,.9f,1.05f,7,MC_DOME);break;
 case 8: /* Crescent wheel with satellite globes and a full aft axle. */
  station_ring(m,r*3.0f*s,h*1.5f,h*.38f,0);
  for(int i=-1;i<=1;i+=2)RP(i*2.6f,1.5f,3.5f,1.0f,1.0f,1.0f,1+i,MC_SPHERE);
  RP(0,0,3.0f,.65f,.65f,2.0f,0,MC_POD);break;
 case 9: /* Three bridged bowl habitats, inspired by painted transit cities. */
  RP(0,.6f,3.0f,3.6f,.22f,.42f,1,MC_BEVEL);
  for(int i=-1;i<=1;i++){RP(i*2.4f,.6f,3.0f,1.2f,.45f,1.2f,2+i,MC_SPHERE);RP(i*2.4f,1.1f,3.0f,.85f,.85f,.9f,2+i,MC_DOME);RP(i*2.4f,-.3f,3.0f,.42f,.7f,.6f,2+i,MC_OCTAGON);}break;
 case 10: /* Axial cylinder with two differently sized rotating districts. */
  RP(0,0,3.4f,1.0f,1.0f,3.0f,2,MC_POD);
  station_ring(m,r*3.0f*s,h*1.6f,h*.35f,2);
  RP(0,0,5.0f,2.0f,2.0f,1.1f,2,MC_SPHERE);break;
 case 11: /* Pyramidal orbital campus and companion garden bubbles. */
  RP(0,.4f,3.0f,3.0f,.5f,1.6f,3,MC_BEVEL);
  RP(0,2.2f,3.0f,1.7f,1.6f,1.4f,3,MC_PYRAMID);
  for(int i=-1;i<=1;i+=2)RP(i*2.0f,1.0f,3.0f,.9f,.9f,1.0f,3+i,MC_DOME);break;
 case 12: /* Offset wheel and palace joined at the central airlock. */
  station_ring(m,r*2.5f*s,h*1.4f,h*.32f,4);
  RP(0,2.1f,3.8f,1.2f,1.7f,1.3f,4,MC_BEVEL);
  RP(0,4.0f,3.8f,1.25f,.5f,1.35f,4,MC_DOME);break;
 case 13: /* Two garden saucers with a hanging cargo drum. */
  RP(0,.5f,2.5f,3.6f,.3f,.45f,5,MC_BEVEL);
  for(int i=-1;i<=1;i+=2){RP(i*2.0f,.5f,2.5f,1.5f,.45f,1.4f,5+i,MC_SPHERE);RP(i*2.0f,1.0f,2.5f,1.1f,.9f,1.1f,5+i,MC_DOME);}
  RP(0,-1.3f,3.1f,.8f,1.3f,1.2f,5,MC_OCTAGON);break;
 case 14: /* Vertical tiered metropolis with broad habitation shoulders. */
  RP(0,1.4f,2.9f,1.3f,2.1f,1.5f,6,MC_OCTAGON);
  for(int i=0;i<3;i++)RP(0,i*1.25f,2.9f,3.3f-i*.6f,.3f,1.6f-i*.2f,6,MC_SPHERE);
  RP(0,3.9f,2.9f,.85f,.65f,1.0f,6,MC_DOME);break;
 default: /* Wide industrial cruise-port: heavy pods, not antenna wings. */
  RP(0,.1f,3.2f,3.4f,.3f,.55f,7,MC_BEVEL);
  for(int i=-1;i<=1;i++)RP(i*2.1f,.5f,3.2f,.85f,1.15f,2.0f,i+2,MC_POD);
  RP(0,1.8f,3.2f,1.1f,.7f,1.2f,7,MC_DOME);break;
 }
 /* Rear service backbone joins attachments to the inhabited central hull.
  * Modules alternate shoulders, varying depth, scale and function by seed. */
 RP(0,0,2.7f,.45f,.45f,2.0f,0,MC_POD);
 int pods=3+(int)((p.seed>>10)%3);
 for(int i=0;i<pods;i++){
  unsigned hsh=station_profile_hash(m->system,40+i);
  float side=(i&1)?-1.f:1.f,x=side*(1.6f+(hsh%7)*.1f),y=-.9f+((hsh>>4)%9)*.18f,z=2.2f+((hsh>>9)%7)*.35f;
  RP(x*.5f,y,z,fabsf(x)*.55f,.14f,.22f,i,MC_BEVEL);
  float sz=.38f+((hsh>>16)%5)*.06f;
  RP(x,y,z,sz,sz*1.3f,sz*1.5f,i,(hsh>>21)&1?MC_SPHERE:MC_POD);
  /* Stout antenna mast and solar/radiator plate, both physically present. */
  if(i<2){RP(x,y+sz+.32f,z,.065f,.32f,.065f,i,MC_OCTAGON);RP(x+side*.5f,y-.4f,z,.6f,.07f,.45f,i,MC_BEVEL);m->b[m->n-1].kind=7;}
 }
 /* Two actual facade billboards, mounted to the aft service frame. */
 for(int i=-1;i<=1;i+=2){
  RP(i*1.25f,.2f,2.0f,1.25f,.10f,.18f,i+2,MC_BEVEL);
  mega_block(m,(Vec3){i*r*2.3f,.2f*r,2*h},(Vec3){r*.48f,r*.72f,h*.14f},(p.seed+i+8)&7,1);
 }
#undef RP
}
#endif
