/* Sharp native polygons with shared UV-mapped pixel paint; no heap.
 * The same geometry draws the player, opponents and selection previews. */
typedef struct {Vec3 pos,right,up,ahead;float scale,burst;int part,paint;} RacerView;
/* Transform the three hull axes once, rather than world-to-view per vertex. */
static RacerView racer_view(Vec3 pos,Vec3 right,Vec3 up,Vec3 ahead,float scale,float burst){
 Vec3 cr,cu,ca;camera_axes(&game,&cr,&cu,&ca);
 RacerView v={camera(&game,pos),mul((Vec3){dot(right,cr),dot(right,cu),dot(right,ca)},scale),mul((Vec3){dot(up,cr),dot(up,cu),dot(up,ca)},scale),mul((Vec3){dot(ahead,cr),dot(ahead,cu),dot(ahead,ca)},scale),scale,burst,0,0};return v;
}
static Vec3 racer_point(const RacerView *v,Vec3 p){return add(v->pos,add(mul(v->right,p.x),add(mul(v->up,p.y),mul(v->ahead,p.z))));}
static void __attribute__((noinline,optimize("O3","fast-math"))) racer_pod(RacerView *v,float x,float y,float front,float rear,float fw,float rw,float rise,unsigned ink){
 Vec3 p[8];float scatter=v->burst*(v->part++%2?1:-1);x+=scatter*65;y+=fabsf(scatter)*25;front+=scatter*37;rear+=scatter*37;
 for(int i=0;i<8;i++){int back=i&4;float width=back?rw:fw;Vec3 q={x+(i&1?width:-width),y+(i&2?rise*(back?1:.45f):-6),back?rear:front};p[i]=racer_point(v,q);}
 static const unsigned char face[6][4]={{0,1,3,2},{4,6,7,5},{0,4,5,1},{2,3,7,6},{0,2,6,4},{1,5,7,3}};
 for(int f=0;f<6;f++){Vec3 a=p[face[f][0]],b=p[face[f][1]],c=p[face[f][2]],d=p[face[f][3]];
  Vec3 u=sub(b,a),w=sub(c,a),n={u.y*w.z-u.z*w.y,u.z*w.x-u.x*w.z,u.x*w.y-u.y*w.x};if(dot(n,a)<=0)continue;
  unsigned col=livery_tint(ink,f==3?20+(int)(35*fabsf(n.z)/fmaxf(1,length(n))):f==2?-48:f==4?-22:f==5?-7:-30);int start=drawcount;queue_triangle(a,b,c,col);queue_triangle(a,c,d,col);
  if(v->paint&&f!=2&&a.z>64&&b.z>64&&c.z>64&&d.z>64&&drawcount==start+2){drawlist[start].material=7800+(v->paint-1)*2;drawlist[start+1].material=7801+(v->paint-1)*2;}
 }
}
static void __attribute__((noinline,optimize("O3","fast-math"))) erl_racer_draw(int id,Vec3 pos,Vec3 right,Vec3 up,Vec3 ahead,float scale,float burst,int ghost){
 const ErlRacer *s=&erl_racers[id];RacerView v=racer_view(pos,right,up,ahead,scale,burst);unsigned hull=s->hull,trim=s->trim;
 if(ghost){hull=planet_rgb_mix(hull,RGB(133,222,239),100);trim=RGB(198,237,244);}
 int detailed=length(sub(pos,camera_origin(&game)))<1800*proj_focal/285.f;
 int fork=s->style==3||s->style==9||s->style==11;
 v.paint=detailed?1+(id&1):0;
 racer_pod(&v,0,0,fork?55:s->nose,-s->tail*.83f,fork?16:3,s->width,11,hull);
 /* Separate outriggers give the split, swept and asymmetric silhouettes. */
 for(int side=-1;side<=1;side+=2){float fore=fork?s->nose-12:s->style==4?92:s->style==1?(side<0?100:30):s->style==7?68:24;
  v.paint=detailed?1+(id&1):0;racer_pod(&v,side*s->pod,-2,fore,-s->tail,3,s->style==7?17:10,9,hull);
  v.paint=0;if(detailed)racer_pod(&v,side*(s->width+8),-2,12,-s->tail*.74f,14,18,3,trim);
  if(detailed)racer_pod(&v,side*(s->pod+5),4,-s->tail*.25f,-s->tail*.91f,2,3,s->fin,hull);
  /* Dark nacelle and a bright core occupy the actual exhaust anchor. */
  if(detailed)racer_pod(&v,side*s->pod,-1,-s->tail+4,-s->tail-1,7,7,4,RGB(19,28,42));
  if(detailed)racer_pod(&v,side*s->pod,0,-s->tail-1,-s->tail-2,3,3,2,RGB(255,191,94));
 }
 if(!detailed)return;
 v.paint=0;
 /* Cockpit rises behind the needle nose, with a narrow sunlit frame. */
 racer_pod(&v,0,10,48,-24,5,14,15,trim);
 racer_pod(&v,0,12,43,-19,3,11,15,RGB(28,71,91));
 if(detailed)racer_pod(&v,0,28,4,-15,2,8,1,RGB(116,214,225));
 if(detailed&&!fork){racer_pod(&v,0,7,s->nose-8,55,1,7,1,trim);}
 else if(detailed)for(int side=-1;side<=1;side+=2)racer_pod(&v,side*s->pod,5,s->nose-21,65,1,3,1,trim);
 /* Broad baked lacquer highlights carry the surface finish without small
  * separate vent geometry, noisy decals or real-time reflection passes. */
}
