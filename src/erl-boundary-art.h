/* Nearby violet energy wall, sharing the real elliptical collision section.
 * Fixed 24-cell grid, clipped translucent spans and a world-space impact ripple. */
static int race_wall_cells,race_wall_samples;
static unsigned race_glass_ink;
/* Resolve the nearest translucent road once. Adjacent curves and further
 * laps must not brighten each other through repeated alpha blending. */
#define race_deck_quad render_scratch.deck
static int race_grid_attempts,race_grid_writes;
#ifdef ELITE_ENABLE_REVIEW
#define RACE_GRID_TRY() do{if(race_deck_lines)race_grid_attempts++;}while(0)
#define RACE_GRID_WRITE() do{if(race_deck_lines)race_grid_writes++;}while(0)
#else
#define RACE_GRID_TRY() ((void)0)
#define RACE_GRID_WRITE() ((void)0)
#endif
static int race_deck_collect,race_deck_lines,race_deck_far;static float race_deck_lanes[3];
#include "erl-apex-ge.h"
static void race_deck_begin(void){
 int top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max:clipy1-1;if(drawcount)flush_solid_meshes();
 race_ge_begin();if(race_deck_gpu){race_deck_collect=1;return;}
 for(int y=top/2;y<=bottom/2;y++)memset(&race_deck_quad[y*(W/2)+clipx0/2],255,((clipx1+1)/2-clipx0/2)*sizeof(unsigned));race_deck_collect=1;
}
static void race_deck_end(void){
 if(race_deck_gpu){race_ge_end();race_deck_collect=0;return;}
 int top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max:clipy1-1;
 const unsigned deck=RGB(64,32,72);
 for(int y=top&~1;y<=bottom;y+=2)for(int x=clipx0&~1;x<clipx1;x+=2){unsigned packed=race_deck_quad[(y/2)*(W/2)+x/2];if(packed==~0u)continue;
  unsigned z=packed>>8,coverage=(packed>>2)&15,alpha=packed&3;int shift=alpha==2?1:2;unsigned mask=alpha==2?0xfefefe:0xfcfcfc,tint=alpha==1?deck-((deck&mask)>>shift):(deck&mask)>>shift;
  int at=y*W+x,out=y*STRIDE+x;
  if((coverage&1)&&z<=surface_depth[at]){unsigned old=fb[out];fb[out]=(alpha==3?old-((old&mask)>>shift):((old&mask)>>shift))+tint;surface_depth[at]=z;}
  if((coverage&2)&&z<=surface_depth[at+1]){unsigned old=fb[out+1];fb[out+1]=(alpha==3?old-((old&mask)>>shift):((old&mask)>>shift))+tint;surface_depth[at+1]=z;}
  if((coverage&4)&&z<=surface_depth[at+W]){unsigned old=fb[out+STRIDE];fb[out+STRIDE]=(alpha==3?old-((old&mask)>>shift):((old&mask)>>shift))+tint;surface_depth[at+W]=z;}
  if((coverage&8)&&z<=surface_depth[at+W+1]){unsigned old=fb[out+STRIDE+1];fb[out+STRIDE+1]=(alpha==3?old-((old&mask)>>shift):((old&mask)>>shift))+tint;surface_depth[at+W+1]=z;}
 }race_deck_collect=0;
}
static unsigned race_deck_at(int x,int y){return race_deck_quad[(y/2)*(W/2)+x/2]>>8;}
static void race_boundary_line(Vec3 from,Vec3 to,unsigned ink){
 Vec3 a=camera(&game,from),b=camera(&game,to);if(a.z<15&&b.z<15)return;
 if(a.z<15)a=add(a,mul(sub(b,a),(15-a.z)/(b.z-a.z)));if(b.z<15)b=add(b,mul(sub(a,b),(15-b.z)/(a.z-b.z)));
 Point p=project(a),q=project(b);float dx=q.x-p.x,dy=q.y-p.y,lo=0,hi=1;
 int left=clipx0,right=clipx1-1,top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max:clipy1-1;
 float ds[4]={-dx,dx,-dy,dy},es[4]={p.x-left,right-p.x,p.y-top,bottom-p.y};
 for(int k=0;k<4;k++){if(fabsf(ds[k])<.0001f){if(es[k]<0)return;continue;}float t=es[k]/ds[k];if(ds[k]<0)lo=fmaxf(lo,t);else hi=fminf(hi,t);if(lo>hi)return;}
 int steps=(int)ceilf(fmaxf(fabsf(dx),fabsf(dy))*(hi-lo));if(steps<1)steps=1;if(steps>750)steps=750;
 float step=(hi-lo)/steps,fx=p.x+dx*lo,fy=p.y+dy*lo,iz0=1.f/a.z,iz1=1.f/b.z,inv=iz0+(iz1-iz0)*lo;
 float sx=dx*step,sy=dy*step,sz=(iz1-iz0)*step;
 /* Interpolate encoded depth over short line spans. Re-evaluate endpoints
  * every eight pixels, rather than taking a PSP reciprocal at every pixel. */
 for(int i=0;i<=steps;){int count=steps-i+1;if(count>8)count=8;float z=surface_encode_depth(inv),end=surface_encode_depth(inv+sz*count),dz=(end-z)*(count==8?.125f:1.f/count);
  for(int j=0;j<count;j++,i++,fx+=sx,fy+=sy,inv+=sz,z+=dz){int x=fx,y=fy;unsigned encoded=z;RACE_GRID_TRY();if(x>=left&&x<=right&&y>=top&&y<=bottom&&encoded<=surface_depth[y*W+x]+(race_deck_lines?128+(encoded>>8):8)&&(!race_deck_lines||race_deck_lines==2||race_deck_gpu||encoded<=race_deck_at(x,y)+128+(encoded>>5))){fb[y*STRIDE+x]=ink;RACE_GRID_WRITE();}}
 }
}
static void race_wall_triangle(Vec3 a,Vec3 b,Vec3 c,int amount){
 int holo=race_glass_ink==RGB(232,46,163),step=holo&&clipx1-clipx0>200?4:2;
 int budget=holo?(step==4?49152:8192):32768;int rowstep=holo?2:step;if(race_wall_samples>=budget)return;
 int shift=amount<=10?5:amount<=24?4:amount<=40?3:2;unsigned mask=shift==5?0xe0e0e0:shift==4?0xf0f0f0:0xf8f8f8;
 unsigned tint=(race_glass_ink&mask)>>shift;
 Vec3 input[3]={camera(&game,a),camera(&game,b),camera(&game,c)},v[4];float lane[4];int n=0;
 for(int i=0;i<3;i++){int j=(i+1)%3;Vec3 p=input[i],q=input[j];if(p.z>=15){v[n]=p;lane[n++]=race_deck_lanes[i];}if((p.z>=15)!=(q.z>=15)){float t=(15-p.z)/(q.z-p.z);v[n]=add(p,mul(sub(q,p),t));lane[n++]=race_deck_lanes[i]+(race_deck_lanes[j]-race_deck_lanes[i])*t;}}
 for(int tri=1;tri+1<n;tri++){
  Vec3 u[3]={v[0],v[tri],v[tri+1]};Point p[3]={project(u[0]),project(u[1]),project(u[2])};
  float area=(float)(p[1].x-p[0].x)*(p[2].y-p[0].y)-(float)(p[2].x-p[0].x)*(p[1].y-p[0].y);if(fabsf(area)<1)continue;
  int x0=fmaxf(clipx0,fminf(p[0].x,fminf(p[1].x,p[2].x))),x1=fminf(clipx1-1,fmaxf(p[0].x,fmaxf(p[1].x,p[2].x)));
  int y0=fmaxf(clipy0<0?surface_y_min:clipy0,fminf(p[0].y,fminf(p[1].y,p[2].y))),y1=fminf(clipy1<0?surface_y_max:clipy1-1,fmaxf(p[0].y,fmaxf(p[1].y,p[2].y)));
  if(x0>x1||y0>y1)continue;
  /* Increment edge/depth planes across spans. PSP division is expensive;
   * do the reciprocals once per triangle, never per raster sample. */
  if(holo)y0&=~1;
  float invarea=1.f/area,iz0=1.f/u[0].z,iz1=1.f/u[1].z,iz2=1.f/u[2].z;
  float dax=(p[1].y-p[2].y)*invarea,dbx=(p[2].y-p[0].y)*invarea;
  float day=(p[2].x-p[1].x)*invarea,dby=(p[0].x-p[2].x)*invarea;
  float ar=((float)(p[1].x-x0)*(p[2].y-y0)-(float)(p[2].x-x0)*(p[1].y-y0))*invarea;
  float br=((float)(p[2].x-x0)*(p[0].y-y0)-(float)(p[0].x-x0)*(p[2].y-y0))*invarea;
  float slope[3];for(int e=0;e<3;e++){int next=(e+1)%3;slope[e]=p[next].y==p[e].y?0:(float)(p[next].x-p[e].x)/(p[next].y-p[e].y);}
  for(int y=y0;y<=y1;y+=rowstep,ar+=day*rowstep,br+=dby*rowstep){
   int left=x0,right=x1;
   if(holo){
    int l[2]={x0,x0},rr[2]={x0-1,x0-1};
    /* Exact single-pixel edge masks, shared depth over a 2x2 interior quad.
     * This matches the opaque renderer's depth granularity without making
     * staircase block edges or repeatedly shading the same translucent road. */
    for(int row=0;row<2&&y+row<=y1;row++){if(y+row<(clipy0<0?surface_y_min:clipy0))continue;float lo=1.e30f,hi=-1.e30f;int hits=0;
     for(int e=0;e<3;e++){int next=(e+1)%3;if((p[e].y<=y+row&&p[next].y>y+row)||(p[next].y<=y+row&&p[e].y>y+row)){float cut=p[e].x+(y+row-p[e].y)*slope[e];if(cut<lo)lo=cut;if(cut>hi)hi=cut;hits++;}}
     if(hits<2)continue;int a=(int)lo,b=(int)hi;if(a<lo)a++;if(b<hi)b++;b--;l[row]=a<x0?x0:a;rr[row]=b>x1?x1:b;
    }
    if(rr[0]<l[0]&&rr[1]<l[1])continue;
    left=(rr[0]<l[0]?l[1]:rr[1]<l[1]?l[0]:l[0]<l[1]?l[0]:l[1])&~1;right=rr[0]>rr[1]?rr[0]:rr[1];
    float dz=dax*(iz0-iz2)+dbx*(iz1-iz2),inv=iz2+(ar+dax*(left-x0))*(iz0-iz2)+(br+dbx*(left-x0))*(iz1-iz2);
    float q0=lane[0]*iz0,q1=lane[tri]*iz1,q2=lane[tri+1]*iz2,dqx=dax*(q0-q2)+dbx*(q1-q2),q=q2+(ar+dax*(left-x0))*(q0-q2)+(br+dbx*(left-x0))*(q1-q2);
    unsigned *quad=&race_deck_quad[(y/2)*(W/2)+left/2];
    for(int x=left;x<=right;){int count=right-x+1;if(count>8)count=8;float world=1.f/inv,endworld=1.f/(inv+dz*count),span=(count==8?.125f:1.f/count);
     float depth=mega_encode_depth(world),depthstep=((float)mega_encode_depth(endworld)-depth)*span;
     float across=q*world,acrossstep=((q+dqx*count)*endworld-across)*span;
     int fullspan=x>=l[0]&&x+count-1<=rr[0]&&x>=l[1]&&x+count-1<=rr[1];
     for(int pair=0;pair<count;pair+=2,x+=2,quad++,inv+=dz*2,depth+=depthstep*2,across+=acrossstep*2,q+=dqx*2){
      unsigned coverage=fullspan?15:((x>=l[0]&&x<=rr[0])?1:0)|((x+1>=l[0]&&x+1<=rr[0])?2:0)|((x>=l[1]&&x<=rr[1])?4:0)|((x+1>=l[1]&&x+1<=rr[1])?8:0);if(!coverage)continue;
      unsigned encoded=(unsigned)depth,old=*quad,oldz=old>>8;if(encoded>oldz+64)continue;
      if(++race_wall_samples>budget)return;float lateral=fabsf(across);int shift=lateral<5300?3:lateral<7100?4:5;if(race_deck_far&&shift<5)shift++;
      if(encoded+64<oldz)*quad=(encoded<<8)|(coverage<<2)|(shift-2);
      else if(encoded<=oldz+64){unsigned near=encoded<oldz?encoded:oldz,alpha=shift-2;if((old&3)<alpha)alpha=old&3;*quad=(near<<8)|((((old>>2)&15)|coverage)<<2)|alpha;}
     }
    }continue;
   }
   float wa=ar+dax*(left-x0),wb=br+dbx*(left-x0);for(int x=left;x<=right;x+=step,wa+=dax*step,wb+=dbx*step){
   float wc=1-wa-wb;if(wa<-.00001f||wb<-.00001f||wc<-.00001f)continue;
   if(++race_wall_samples>budget)return;
   unsigned z=surface_encode_depth(wa*iz0+wb*iz1+wc*iz2);
   for(int oy=0;oy<rowstep&&y+oy<=y1;oy++)for(int ox=0;ox<step&&x+ox<=right;ox++)if(z<=surface_depth[(y+oy)*W+x+ox]){unsigned *dest=&fb[(y+oy)*STRIDE+x+ox],old=*dest;*dest=holo?old-((old&mask)>>shift)+tint:planet_rgb_mix(race_glass_ink,old,255-amount);}
  }}
 }
}
static void race_boundary_glass(void){
 SpaceRace *r=&game.race;race_glass_ink=RGB(91,42,151);race_wall_cells=race_wall_samples=0;if(r->arcade.rules.course!=RA_CORRIDOR)return;
 RaceTrackFrame nearest=race_track_nearest(r,r->gate,camera_origin(&game));int best=nearest.section;
 Vec3 eye=camera_origin(&game),offset=sub(eye,nearest.centre);float angle=atan2f(dot(offset,nearest.up)/6400,dot(offset,nearest.right)/8900);
 Vec3 hit=add(nearest.centre,add(mul(nearest.right,cosf(angle)*8900),mul(nearest.up,sinf(angle)*6400)));
 float distance=length(sub(hit,eye));if(distance>2600)return;RaceGate *q=&r->gates[best];
 Vec3 from=best?r->gates[best-1].pos:r->start;float leg=length(sub(q->pos,from)),along=nearest.t*leg,lo=fmaxf(0,along-5000),hi=fminf(leg,along+5000);int alpha=(int)(48*(1-distance/3000));if(alpha<6)alpha=6;
 for(int i=0;i<6;i++)for(int j=0;j<4;j++){
  float a=angle-.48f+i*.16f,b=a+.16f,z0=lo+(hi-lo)*j/4,z1=lo+(hi-lo)*(j+1)/4;
  RaceTrackFrame f0=race_track_frame(r,best,z0/fmaxf(1,leg)),f1=race_track_frame(r,best,z1/fmaxf(1,leg));
  Vec3 p[4]={add(f0.centre,add(mul(f0.right,cosf(a)*8900),mul(f0.up,sinf(a)*6400))),add(f0.centre,add(mul(f0.right,cosf(b)*8900),mul(f0.up,sinf(b)*6400))),add(f1.centre,add(mul(f1.right,cosf(b)*8900),mul(f1.up,sinf(b)*6400))),add(f1.centre,add(mul(f1.right,cosf(a)*8900),mul(f1.up,sinf(a)*6400)))};
  race_wall_triangle(p[0],p[1],p[2],alpha);race_wall_triangle(p[0],p[2],p[3],alpha);race_wall_cells++;
  unsigned ink=RGB(121,82,165);race_boundary_line(p[0],p[1],ink);race_boundary_line(p[0],p[3],ink);if(i==5)race_boundary_line(p[1],p[2],ink);if(j==3)race_boundary_line(p[2],p[3],ink);
 }
 if(r->arcade.wall_ripple>0){float age=.4f-r->arcade.wall_ripple;Vec3 tangent=norm(add(mul(nearest.right,-sinf(angle)*8900),mul(nearest.up,cosf(angle)*6400)));Vec3 impact=r->arcade.wall_impact;
  for(int ring=0;ring<3;ring++){float radius=100+age*1900+ring*170;for(int i=0;i<24;i++){float a=i*.2617994f,b=a+.2617994f;Vec3 p=add(impact,add(mul(tangent,cosf(a)*radius),mul(q->normal,sinf(a)*radius))),v=add(impact,add(mul(tangent,cosf(b)*radius),mul(q->normal,sinf(b)*radius)));race_boundary_line(p,v,ring?RGB(155,109,207):RGB(215,164,255));}}
 }
}
/* Pixel-authored filled arrow: same long stem and outlined violet head as the
 * approved reference. Rotate geometry, never a blurred scaled bitmap. */
static void race_arrow_polygon(const int points[7][2],float dx,float dy,unsigned ink){
 int vx[7],vy[7],low=272,high=0;for(int i=0;i<7;i++){vx[i]=244+points[i][0]*dx-points[i][1]*dy;vy[i]=84+points[i][0]*dy+points[i][1]*dx;low=low<vy[i]?low:vy[i];high=high>vy[i]?high:vy[i];}
 for(int y=low;y<=high;y++){int intersections[7],n=0;for(int i=0;i<7;i++){int j=(i+1)%7;if((vy[i]<=y&&vy[j]>y)||(vy[j]<=y&&vy[i]>y))intersections[n++]=vx[i]+(y-vy[i])*(vx[j]-vx[i])/(vy[j]-vy[i]);}for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(intersections[j]<intersections[i]){int t=intersections[i];intersections[i]=intersections[j];intersections[j]=t;}for(int i=0;i+1<n;i+=2)rect(intersections[i],y,intersections[i+1]-intersections[i]+1,1,ink);}
 for(int i=0;i<7;i++)line(vx[i],vy[i],vx[(i+1)%7],vy[(i+1)%7],ink);
}
static void race_course_arrow(Vec3 waypoint){
 float dx=waypoint.x,dy=-waypoint.y;if(waypoint.z<0){if(fabsf(dx)<200)dx=0;dy=fmaxf(300,fabsf(dy));}
 if(fabsf(dx)+fabsf(dy)<200){dx=0;dy=-1;}float length=sqrtf(dx*dx+dy*dy);dx/=length;dy/=length;
 static const int outer[7][2]={{-36,-7},{11,-7},{11,-15},{37,0},{11,15},{11,7},{-36,7}},rim[7][2]={{-35,-6},{12,-6},{12,-13},{35,0},{12,13},{12,6},{-35,6}},inner[7][2]={{-33,-4},{14,-4},{14,-10},{31,0},{14,10},{14,4},{-33,4}};
 race_arrow_polygon(outer,dx,dy,RGB(11,10,27));race_arrow_polygon(rim,dx,dy,RGB(216,171,255));race_arrow_polygon(inner,dx,dy,RGB(178,109,238));
}
