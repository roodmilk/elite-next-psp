/* Camera frustum is invariant throughout one ERL render, including details. */
/* Keep the shared dispatch once instead of duplicating it in every new5 pass. */
static void __attribute__((noinline)) neon_flush(void){flush_solid_meshes();}
static int neon_strict_seams=1;
static float neon_planes[4],neon_norms[4];static Vec3 neon_eye,neon_right,neon_up,neon_ahead;
static Vec3 neon_camera(Vec3 world){Vec3 d=sub(world,neon_eye);return (Vec3){dot(d,neon_right),dot(d,neon_up),dot(d,neon_ahead)};}
static void neon_frame_begin(void){neon_eye=camera_origin(&game);camera_axes(&game,&neon_right,&neon_up,&neon_ahead);neon_planes[0]=-proj_ox/proj_focal;neon_planes[1]=(neon_render_width-proj_ox)/proj_focal;neon_planes[2]=proj_oy/proj_focal;neon_planes[3]=(proj_oy-neon_render_height)/proj_focal;for(int i=0;i<4;i++)neon_norms[i]=sqrtf(1+neon_planes[i]*neon_planes[i]);}
static int neon_visible(Vec3 world,float radius){Vec3 v=neon_camera(world);return v.z+radius>=4&&v.x-neon_planes[0]*v.z>=-radius*neon_norms[0]&&neon_planes[1]*v.z-v.x>=-radius*neon_norms[1]&&neon_planes[2]*v.z-v.y>=-radius*neon_norms[2]&&v.y-neon_planes[3]*v.z>=-radius*neon_norms[3];}
/* Neon Interchange production slice. Bounded geometry, shared spline/frame,
 * near-clipped opaque panels, procedural city; no textures or heap allocations. */
static void neon_line(Vec3 from,Vec3 to,unsigned ink){
 Vec3 a=neon_camera(from),b=neon_camera(to);if(a.z<15&&b.z<15)return;
 if(a.z<15)a=add(a,mul(sub(b,a),(15-a.z)/(b.z-a.z)));if(b.z<15)b=add(b,mul(sub(a,b),(15-b.z)/(a.z-b.z)));
 float ia=1/a.z,ib=1/b.z;Point p={proj_ox+a.x*proj_focal*ia,proj_oy-a.y*proj_focal*ia,a.z},q={proj_ox+b.x*proj_focal*ib,proj_oy-b.y*proj_focal*ib,b.z};float dx=q.x-p.x,dy=q.y-p.y,lo=0,hi=1;
 int left=clipx0,right=clipx1-1,top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max:clipy1-1,strict=race_deck_lines<0;
 if((p.x<left&&q.x<left)||(p.x>right&&q.x>right)||(p.y<top&&q.y<top)||(p.y>bottom&&q.y>bottom))return;
 float ds[4]={-dx,dx,-dy,dy},es[4]={p.x-left,right-p.x,p.y-top,bottom-p.y};
 for(int i=0;i<4;i++){if(fabsf(ds[i])<.0001f){if(es[i]<0)return;continue;}float t=es[i]/ds[i];if(ds[i]<0){if(t>lo)lo=t;}else if(t<hi)hi=t;if(lo>hi)return;}
 float extent=surface_max(fabsf(dx),fabsf(dy))*(hi-lo);int steps=surface_ceil(extent);if(steps<1)steps=1;if(steps>750)steps=750;
 float step=(hi-lo)/steps,fx=p.x+dx*lo,fy=p.y+dy*lo,inv=ia+(ib-ia)*lo,sx=dx*step,sy=dy*step,sz=(ib-ia)*step;
 for(int i=0;i<=steps;){int count=steps-i+1;if(count>8)count=8;float z=neon_encode_depth(inv),end=neon_encode_depth(inv+sz*count),dz=count==8?(end-z)*.125f:(end-z)/count;
  for(int j=0;j<count;j++,i++,fx+=sx,fy+=sy,inv+=sz,z+=dz){int x=fx,y=fy;unsigned encoded=z;if(x>=left&&x<=right&&y>=top&&y<=bottom){unsigned limit=surface_depth[y*W+x];if(neon_depth_mode&&race_deck_lines>0){int qx=x&~1,qy=y&~1;unsigned base=limit,nearby;if(qx+2<W){nearby=surface_depth[qy*W+qx+2];if(nearby>limit&&nearby<65535&&nearby-base<=8)limit=nearby;}if(qy+2<H){nearby=surface_depth[(qy+2)*W+qx];if(nearby>limit&&nearby<65535&&nearby-base<=8)limit=nearby;}}unsigned tolerance=strict?0:neon_depth_mode?2:(race_deck_lines?128+(encoded>>8):8);if((!strict||limit!=65535)&&encoded<=limit+tolerance)fb[y*STRIDE+x]=ink;}}
 }
}
static void neon_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){
 a=neon_camera(a);b=neon_camera(b);c=neon_camera(c);d=neon_camera(d);
 if(!neon_depth_mode||!(a.z>=64&&b.z>=64&&c.z>=64&&d.z>=64)){queue_triangle(a,b,c,ink);queue_triangle(a,c,d,ink);return;}
 /* Quads in front of the near plane need no clipping. Reuse shared projected
  * corners, retaining the original triangle order, rejects and arithmetic. */
 if(drawcount>=2048)return;if(surface_depth_on)ink=surface_shade(ink);
 Point p[4]={project(a),project(b),project(c),project(d)};
 int top=clipy0>=0?clipy0:view_top(),bot=clipy1>=0?clipy1:view_bot()+1;
 for(int i=1;i<3&&drawcount<2048;i++){
  Point q=p[i],r=p[i+1];
  if(!render_reference&&((p[0].x<clipx0&&q.x<clipx0&&r.x<clipx0)||(p[0].x>=clipx1&&q.x>=clipx1&&r.x>=clipx1)||(p[0].y<top&&q.y<top&&r.y<top)||(p[0].y>=bot&&q.y>=bot&&r.y>=bot)))continue;
  DrawTri *t=&drawlist[drawcount];t->order=drawcount++;render_submitted++;t->p[0]=p[0];t->p[1]=q;t->p[2]=r;t->color=ink;t->depth=(p[0].z+q.z+r.z)/3;t->material=surface_depth_on?surface_material:0;
 }
}
static void race_neon_sky(void){
 for(int y=0;y<H;y++){int shade=y*22/H;rect(0,y,W,1,RGB(7+shade/4,12+shade/2,26+shade));}
 /* Quiet distant cloud bands; fixed work and no full-frame alpha passes. */
 for(int i=0;i<18;i++){int y=91+i*5;for(int x=0;x<W;x+=20){int v=(x*7+i*29)%17;rect(x,y+v/4,22,2,RGB(25+i,33+i,50+i));}}
}
static Vec3 neon_edge_sample(int k,float t,float lane){float q=t*32;int n=(int)q;if(n>31)n=31;float mix=q-n;Vec3 a=race_floor_cached_point(race_floor_frame[k%7][n],lane),b=race_floor_cached_point(race_floor_frame[k%7][n+1],lane);return add(add(a,mul(sub(b,a),mix)),mul(race_floor_frame[k%7][n].up,2*race_neon_scale(&game.race)));}
static void race_neon_edge_details(void){
 SpaceRace *r=&game.race;RaceTrackFrame nearest=race_track_nearest(r,r->gate,game.pos);float scale=race_neon_scale(r),step=28.f*scale*(RACE_GATES/r->laps)/(r->top_speed*46.f);
 int first_cell=(int)floorf((nearest.section+nearest.t)/step);
 /* Reference: short 20/28 mechanical shoulder cells, with paired longitudinal
  * lilac lights. Only the nearby 1.8 km design window carries this fine mesh. */
 for(int cell=82;cell>=-4;cell--){float q=(first_cell+cell)*step;int k=(int)floorf(q);float t=q-k;if(k<0||k>=RACE_GATES||k>r->gate+4)continue;int kind=race_track_surface(r,k,t);if(kind!=1)continue;
  float end=q+step*(20.f/28.f);int ek=(int)end;float et=end-ek;if(ek>=RACE_GATES||ek>r->gate+4)continue;
  unsigned light=(first_cell+cell)&1?RGB(188,73,153):RGB(112,43,104);
  for(int side=-1;side<=1;side+=2){Vec3 a=neon_edge_sample(k,t,side*82*2.5f*scale),b=neon_edge_sample(k,t,side*109*2.5f*scale),cc=neon_edge_sample(ek,et,side*109*2.5f*scale),d=neon_edge_sample(ek,et,side*82*2.5f*scale);neon_quad(a,b,cc,d,light);}
 }
 neon_flush();
 for(int cell=0;cell<82;cell+=2){float q=(first_cell+cell)*step;int k=(int)q;float t=q-k;if(k>=RACE_GATES||k>r->gate+4||race_track_surface(r,k,t)!=1)continue;float end=q+step*(19.f/28.f);int ek=(int)end;if(ek>=RACE_GATES||ek>r->gate+4)break;
  neon_line(neon_edge_sample(k,t,-110*2.5f*scale),neon_edge_sample(k,t,110*2.5f*scale),RGB(88,84,108));
  for(int side=-1;side<=1;side+=2)neon_line(neon_edge_sample(k,t,side*100*2.5f*scale),neon_edge_sample(ek,end-ek,side*100*2.5f*scale),RGB(224,185,255));
 }
}
#include "erl-neon-rings.h"
#include "erl-neon-projectiles.h"
static void neon_glow_line(Vec3 from,Vec3 to,int width,unsigned halo,unsigned core,int alpha){
 NeonStrokeSegment stroke;if(neon_stroke_camera(neon_camera(from),neon_camera(to),&stroke))neon_stroke_draw(&stroke,width,halo,alpha);
 neon_line(from,to,core);
}
#include "erl-neon-road.h"
#include "erl-neon-pads.h"
#include "erl-neon-service.h"
static void race_neon_track_draw(void){race_neon_authored_road();race_neon_pads();race_neon_service_draw();}
#include "erl-neon-city.h"
#include "erl-neon-items.h"
/* Solid portals are tied to absolute course cells, never the moving camera.
 * Their rectangular opening clears the entire cylindrical phase envelope. */
static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_infrastructure(void){
 SpaceRace *r=&game.race;RaceTrackFrame near=race_track_nearest(r,r->gate,game.pos);float scale=race_neon_scale(r),unit=2.5f*scale;
 float step=28*scale*(RACE_GATES/r->laps)/(r->top_speed*46.f);int first=surface_floor((near.section+near.t)/step);
 for(int cell=(first/32)*32;cell<first+100;cell+=32){float q=cell*step;int kind=neon_road_kind(q);NeonRoadRow row;if(kind==2||!neon_road_row(q,&row))continue;
  float edge=(kind?RACE_NEON_EDGE:190.f)*unit,outer=edge+65*scale,high=edge+130*scale;
  if(!neon_visible(add(row.floor,mul(row.up,high*.5f)),outer*1.5f))continue;
  RaceGate gate={.right=norm(row.right),.up=norm(row.up),.normal=norm(race_ui_cross(row.right,row.up))};
  for(int side=-1;side<=1;side+=2)race_block(&gate,neon_road_point(&row,side*outer,high*.5f-120*scale),32*scale,high*.5f+120*scale,30*scale,RGB(55,57,73));
  race_block(&gate,neon_road_point(&row,0,high),outer+32*scale,34*scale,45*scale,RGB(73,61,63));neon_flush();
  Vec3 face=mul(gate.normal,-46*scale);for(int side=-1;side<=1;side+=2){Vec3 a=add(neon_road_point(&row,side*(outer-33*scale),0),face),b=add(neon_road_point(&row,side*(outer-33*scale),high-35*scale),face);neon_glow_line(a,b,9,RGB(255,111,20),RGB(255,191,81),72);}
  Vec3 a=add(neon_road_point(&row,-outer,high-35*scale),face),b=add(neon_road_point(&row,outer,high-35*scale),face);neon_glow_line(a,b,11,RGB(255,101,18),RGB(255,205,102),88);
 }
}
static uint64_t neon_world_us[4];static unsigned neon_world_frames;
static void race_neon_world_draw(void){
 uint64_t tick=sceKernelGetSystemTimeWide(),now;
 race_neon_city_draw();now=sceKernelGetSystemTimeWide();neon_world_us[0]+=now-tick;tick=now;
 race_neon_infrastructure();now=sceKernelGetSystemTimeWide();neon_world_us[1]+=now-tick;tick=now;
 race_neon_pickups();now=sceKernelGetSystemTimeWide();neon_world_us[2]+=now-tick;tick=now;
 for(int i=0;i<game.race.rivals;i++)race_ship_draw(&game.race.pilot[i],i);neon_flush();neon_world_us[3]+=sceKernelGetSystemTimeWide()-tick;neon_world_frames++;
}
#include "erl-neon-phase.h"
static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_boundary(void){
 SpaceRace *r=&game.race;RaceTrackFrame nearest=race_track_nearest(r,r->gate,game.pos);
 float scale=race_neon_scale(r),nearest_edge=(race_track_surface(r,nearest.section,nearest.t)==1?RACE_NEON_EDGE:190.f)*2.5f*scale;
 float step=84*scale*(RACE_GATES/r->laps)/(r->top_speed*46.f);int first=(int)floorf((nearest.section+nearest.t)/step)-1;
 float signed_across=dot(sub(game.pos,nearest.centre),nearest.right),across_player=fabsf(signed_across),height_player=dot(sub(game.pos,nearest.centre),nearest.up)-RACE_TRACK_FLOOR;
 float radial=sqrtf(across_player*across_player+fmaxf(0,height_player)*fmaxf(0,height_player));
 float proximity=nearest_edge-race_neon_body_radius(&game)-radial,fade=fmaxf(0,fminf(1,(100*scale-proximity)/(100*scale)));
 unsigned old_ink=race_glass_ink;int old_samples=race_wall_samples;race_glass_ink=RGB(167,47,130);race_wall_samples=0;if(fade<=.01f&&r->arcade.wall_ripple<=0){race_glass_ink=old_ink;race_wall_samples=old_samples;return;}fade=fade*fade*(3-2*fade);neon_phase_begin();
 for(int n=0;n<28;n++){
  float q=(first+n)*step,end=q+step;if(q<0||end>=RACE_GATES)continue;
  int ak=neon_road_kind(q),bk=neon_road_kind(end);if(ak==2&&bk==2)continue;
  /* Keep the real jump open, but enclose ordinary and magnetic road. */
  if(ak==2){float lo=q,hi=end;for(int cut=0;cut<7;cut++){float mid=(lo+hi)*.5f;if(neon_road_kind(mid)!=2)hi=mid;else lo=mid;}q=hi;ak=neon_road_kind(q);}
  if(bk==2){float lo=q,hi=end;for(int cut=0;cut<7;cut++){float mid=(lo+hi)*.5f;if(neon_road_kind(mid)!=2)lo=mid;else hi=mid;}end=lo;bk=neon_road_kind(end);}
  int k=(int)floorf(q);if(k>r->gate+3)continue;
  NeonRoadRow a,b;if(!neon_road_row(q,&a)||!neon_road_row(end,&b))continue;
  float edge_a=(ak==1?RACE_NEON_EDGE:190.f)*2.5f*scale,edge_b=(bk==1?RACE_NEON_EDGE:190.f)*2.5f*scale;
  if(!neon_visible(mul(add(a.floor,b.floor),.5f),fmaxf(edge_a,edge_b)*1.45f))continue;
  float local=fade*fmaxf(0,1-fmaxf(0,q-nearest.section-nearest.t)/(step*6));Vec3 prior=neon_road_point(&a,-edge_a,0),endprior=neon_road_point(&b,-edge_b,0);
  /* The phase field materialises locally as the craft approaches it. A
   * centred racer sees the road and skyline instead of a permanent cage. */
  if(local<=.01f)continue;
  if(signed_across<0&&local>.05f)neon_glow_line(prior,endprior,5,RGB(214,36,160),neon_road_ink(RGB(255,145,225),.25f+local*.75f),(int)(local*90));
  /* Ten deliberate facets form one semicircular tunnel. Their hard joints
   * preserve the low-poly look while the translucent field reads as round. */
  for(int arcseg=1;arcseg<=10;arcseg++){float angle=3.14159265f*(1-arcseg/10.f),x=cosf(angle),h=sinf(angle);Vec3 p=neon_road_point(&a,x*edge_a,h*edge_a),ep=neon_road_point(&b,x*edge_b,h*edge_b);
   /* Only the nearby sector materialises; approaching one side must not
    * light a cage around the entire forward view. Collision stays unchanged. */
   float facing=fmaxf(0,(x*signed_across+h*fmaxf(0,height_player))/fmaxf(20*scale,radial)-.25f)*(1.f/.75f),sector=local*facing*facing;int alpha=(int)(sector*150);
   if(sector>.035f){neon_phase_triangle(prior,endprior,ep,alpha);neon_phase_triangle(prior,ep,p,alpha);}
   if(sector>.05f)neon_glow_line(p,ep,4,RGB(205,32,150),neon_road_ink(RGB(255,117,214),.25f+sector*.75f),(int)(sector*90));
   if(sector>.16f&&!(n%4))neon_line(prior,p,neon_road_ink(RGB(211,78,174),sector));prior=p;endprior=ep;
  }
 }
 neon_phase_end();
 /* A phase strike throws short deterministic sparks from the real contact. */
 if(r->arcade.wall_ripple>0){float age=.3f-r->arcade.wall_ripple,life=r->arcade.wall_ripple/.3f;RaceTrackFrame hit=race_track_nearest(r,r->gate,r->arcade.wall_impact);
  for(int i=0;i<14;i++){float angle=i*2.399963f+r->elapsed*11;float reach=(70+i*17)*(1+age*8);Vec3 ray=add(mul(hit.right,cosf(angle)*reach),add(mul(hit.up,sinf(angle)*reach),mul(hit.ahead,(i%3-1)*reach*.45f)));neon_line(r->arcade.wall_impact,add(r->arcade.wall_impact,ray),life>.45f?RGB(255,225,244):RGB(214,72,165));}
 }
 race_glass_ink=old_ink;race_wall_samples=old_samples;
}
