/* Four approved full-lap sky rings. Gameplay contact uses the same generated
 * data in erl-arcade-course.h. Arc positions are the approved clock markers
 * inverted through the existing route LUT; native FPS still needs A/B proof.
 * These strokes only read depth, preserving the opaque 2x2 ownership rule. */
#include "erl-neon-rings-data.h"
static int neon_rings_enabled=1;
typedef struct {float x,y,inv;} NeonStrokePoint;
typedef struct {NeonStrokePoint a,b;} NeonStrokeSegment;

/* Projected inverse depth is linear along each screen segment. Clip once
 * with enough margin for the widest supported stroke, then reuse all layers. */
static int __attribute__((noinline,noclone,optimize("O3","fast-math"))) neon_stroke_clip(NeonStrokePoint a,NeonStrokePoint b,NeonStrokeSegment *out){
 float dx=b.x-a.x,dy=b.y-a.y,lo=0,hi=1;
 int left=clipx0-9,right=clipx1+8,top=(clipy0<0?surface_y_min:clipy0)-9,bottom=(clipy1<0?surface_y_max:clipy1-1)+9;
 if((a.x<left&&b.x<left)||(a.x>right&&b.x>right)||(a.y<top&&b.y<top)||(a.y>bottom&&b.y>bottom))return 0;
 float ds[4]={-dx,dx,-dy,dy},es[4]={a.x-left,right-a.x,a.y-top,bottom-a.y};
 for(int i=0;i<4;i++){
  if(ds[i]>-.00001f&&ds[i]<.00001f){if(es[i]<0)return 0;continue;}
  float t=es[i]/ds[i];if(ds[i]<0){if(t>lo)lo=t;}else if(t<hi)hi=t;if(lo>hi)return 0;
 }
 float di=b.inv-a.inv;
 out->a=(NeonStrokePoint){a.x+dx*lo,a.y+dy*lo,a.inv+di*lo};
 out->b=(NeonStrokePoint){a.x+dx*hi,a.y+dy*hi,a.inv+di*hi};return 1;
}

/* Reusable trail API: inputs are camera-space endpoints, not world points.
 * A prepared segment can be drawn repeatedly with different widths/colours. */
static int __attribute__((noinline,noclone,optimize("O3","fast-math"))) neon_stroke_camera(Vec3 a,Vec3 b,NeonStrokeSegment *out){
 if(a.z<64&&b.z<64)return 0;
 if(a.z<64)a=add(a,mul(sub(b,a),(64-a.z)/(b.z-a.z)));
 if(b.z<64)b=add(b,mul(sub(a,b),(64-b.z)/(a.z-b.z)));
 float ia=1/a.z,ib=1/b.z;
 NeonStrokePoint p={proj_ox+a.x*proj_focal*ia,proj_oy-a.y*proj_focal*ia,ia};
 NeonStrokePoint q={proj_ox+b.x*proj_focal*ib,proj_oy-b.y*proj_focal*ib,ib};return neon_stroke_clip(p,q,out);
}

static void __attribute__((noinline,noclone,optimize("O3","fast-math"))) neon_stroke_draw(const NeonStrokeSegment *s,int width,unsigned ink,int alpha){
 if(width<1||alpha<1)return;if(width>12)width=12;if(alpha>255)alpha=255;
 int left=clipx0<0?0:clipx0,right=clipx1>W?W-1:clipx1-1;
 int top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max:clipy1-1;
 if(top<0)top=0;if(bottom>=H)bottom=H-1;
 float dx=s->b.x-s->a.x,dy=s->b.y-s->a.y,ax=fabsf(dx),ay=fabsf(dy),extent=ax>ay?ax:ay;
 int horizontal=ax>=ay,steps=(int)extent;if(steps<extent)steps++;if(steps<1)steps=1;if(steps>750)return;
 float fraction=1.f/steps,fx=s->a.x,fy=s->a.y,inv=s->a.inv;
 float sx=dx*fraction,sy=dy*fraction,si=(s->b.inv-s->a.inv)*fraction;
 float ratio=(horizontal?dy:dx)/(extent>.0001f?extent:1),half=width*.5f*sqrtf(1+ratio*ratio);
 unsigned ink_rb=(ink&0xff00ffu)*alpha,ink_g=(ink&0xff00u)*alpha;
 for(int i=0;i<=steps;i++,fx+=sx,fy+=sy,inv+=si){
  unsigned depth=neon_encode_depth(inv);int major=(int)((horizontal?fx:fy)+.5f);
  if(major<(horizontal?left:top)||major>(horizontal?right:bottom))continue;
  float minor=horizontal?fy:fx,low=minor-half,high=minor+half;
  int minimum=horizontal?top:left,maximum=horizontal?bottom:right;
  int first=low<minimum?minimum:(int)(low+.9999f),last=high>maximum+1?maximum:(int)(high-.0001f);
  int x=horizontal?major:first,y=horizontal?first:major;
  unsigned *pixel=fb+y*STRIDE+x;const unsigned short *z=surface_depth+y*W+x;
  int pixel_step=horizontal?STRIDE:1,depth_step=horizontal?W:1;
  for(int j=first;j<=last;j++,pixel+=pixel_step,z+=depth_step){
   if(depth>*z+2u)continue;
   if(alpha==255)*pixel=ink;
   else {unsigned old=*pixel,rb=(((old&0xff00ffu)*(256-alpha)+ink_rb)>>8)&0xff00ffu;
    unsigned green=(((old&0xff00u)*(256-alpha)+ink_g)>>8)&0xff00u;*pixel=0xff000000u|rb|green;}
  }
 }
}

static void __attribute__((noinline,noclone,optimize("O3","fast-math"))) neon_ring_draw(RaceTrackFrame f,Vec3 centre,float radius){
 Vec3 camera_points[40],origin=neon_camera(centre);
 Vec3 right={dot(f.right,neon_right)*radius,dot(f.right,neon_up)*radius,dot(f.right,neon_ahead)*radius};
 Vec3 up={dot(f.up,neon_right)*radius,dot(f.up,neon_up)*radius,dot(f.up,neon_ahead)*radius};
 NeonStrokePoint points[40];NeonStrokeSegment segments[40];int count=0;float cs=1,sn=0;
 for(int i=0;i<40;i++){
  Vec3 p=add(origin,add(mul(right,cs),mul(up,sn)));camera_points[i]=p;
  if(p.z>=64){float inv=1/p.z;points[i]=(NeonStrokePoint){proj_ox+p.x*proj_focal*inv,proj_oy-p.y*proj_focal*inv,inv};}
  float next=cs*.9876883406f-sn*.1564344650f;sn=sn*.9876883406f+cs*.1564344650f;cs=next;
 }
 for(int i=0;i<40;i++){
  int next=(i+1)%40,visible;
  if(camera_points[i].z>=64&&camera_points[next].z>=64)visible=neon_stroke_clip(points[i],points[next],&segments[count]);
  else visible=neon_stroke_camera(camera_points[i],camera_points[next],&segments[count]);
  count+=visible;
 }
 /* Whole-ring layers keep neighbouring segment joins free of dark seams. */
 static const unsigned inks[3]={RGB(26,191,255),RGB(84,213,237),RGB(216,255,255)};
 /* Thickness follows perspective too, instead of a distant six-pixel halo
  * making the approaching ring look comparatively thin and small. */
 float projected=radius*proj_focal/fmaxf(64,origin.z);
 int width=(int)(projected*.12f);if(width<4)width=4;if(width>12)width=12;
 int widths[3]={width,width>3?width/2:1,1};
 for(int layer=0;layer<3;layer++)for(int i=0;i<count;i++)neon_stroke_draw(&segments[i],widths[layer],inks[layer],layer==0?90:255);
 int left=clipx0<0?0:clipx0,right_edge=clipx1>W?W-1:clipx1-1;
 int top=clipy0<0?surface_y_min:clipy0,bottom=clipy1<0?surface_y_max:clipy1-1;
 if(top<0)top=0;if(bottom>=H)bottom=H-1;
 for(int i=0;i<40;i+=5){
  if(camera_points[i].z<64)continue;int x=(int)(points[i].x+.5f),y=(int)(points[i].y+.5f);
  if(x<left-1||x>right_edge+1||y<top-1||y>bottom+1)continue;unsigned depth=neon_encode_depth(points[i].inv);
  for(int dy=-1;dy<=1;dy++)for(int dx=-1;dx<=1;dx++){int xx=x+dx,yy=y+dy;
   if(xx>=left&&xx<=right_edge&&yy>=top&&yy<=bottom&&depth<=surface_depth[yy*W+xx]+2u)fb[yy*STRIDE+xx]=RGB(229,255,255);}
 }
}

static void __attribute__((noinline,noclone,optimize("O3","fast-math"))) race_neon_rings(void){
 SpaceRace *r=&game.race;if(!neon_rings_enabled||r->layout!=5||r->laps<=0)return;int per=RACE_GATES/r->laps;if(per<1)return;
 typedef struct {RaceTrackFrame frame;Vec3 centre;float distance;} VisibleRing;
 VisibleRing visible[2];int count=0,first=r->gate-2,last=r->gate+2;
 if(first<0)first=0;if(last>=RACE_GATES)last=RACE_GATES-1;float scale=race_neon_scale(r),radius=ERL_BOOST_RING_RADIUS*scale;
 for(int lap=first/per;lap<=last/per;lap++)for(int i=0;i<NEON_RING_COUNT;i++){
  float q=neon_sky_rings[i][0]*per;int section=lap*per+(int)q;if(section<first||section>last)continue;
  RaceTrackFrame f=race_track_frame(r,section,q-(int)q);Vec3 centre=add(f.centre,mul(f.up,RACE_TRACK_FLOOR+neon_sky_rings[i][1]*scale)),delta=sub(centre,neon_eye);
  float distance=dot(delta,delta);if(distance>14000.f*14000.f||!neon_visible(centre,radius+40*scale))continue;
  int slot=count;if(count<2)count++;else {slot=visible[0].distance>visible[1].distance?0:1;if(distance>=visible[slot].distance)continue;}
  visible[slot]=(VisibleRing){f,centre,distance};
 }
 if(count==2&&visible[0].distance<visible[1].distance){VisibleRing swap=visible[0];visible[0]=visible[1];visible[1]=swap;}
 for(int i=0;i<count;i++)neon_ring_draw(visible[i].frame,visible[i].centre,radius);
}
