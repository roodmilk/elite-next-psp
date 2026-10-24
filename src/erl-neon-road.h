/* Authored Neon road: dark floor and pink mechanics share every row/seam.
 * No shoulder overlays, lifted edge strips, heap allocations or GPU coupling.
 * Fine 28-unit cells retain editable geometry, while visual joints are grouped
 * into broad four-cell panels and fade by their measured screen footprint.
 * Beyond 560/1288 design units use 2x/4x shared-row blocks.
 * Surface transitions always retain fine cells, including the launch gap. */
typedef struct {Vec3 floor,right,up;} NeonRoadRow;
typedef struct {int cell,kinds,stride;NeonRoadRow a,b,c;} NeonRoadVisible;
static uint64_t neon_road_us[3];static unsigned neon_road_frames;

static int neon_road_row(float q,NeonRoadRow *row){
 int section=surface_floor(q);float t=q-section;if(section==RACE_GATES&&t==0){section--;t=1;}
 if(section<0||section>=RACE_GATES||race_floor_owner[section%7]!=section)return 0;
 float sample=t*32;int n=(int)sample;if(n>31)n=31;float mix=sample-n;
 RaceFloorFrame a=race_floor_frame[section%7][n],b=race_floor_frame[section%7][n+1];
 Vec3 af=race_floor_cached_point(a,0),bf=race_floor_cached_point(b,0);row->floor=add(af,mul(sub(bf,af),mix));
 row->right=add(a.right,mul(sub(b.right,a.right),mix));row->up=add(a.up,mul(sub(b.up,a.up),mix));return 1;
}
static Vec3 neon_road_point(const NeonRoadRow *row,float lane,float lift){
 /* Floor and lateral axes are both affine interpolations of the same cache
  * endpoints. This is exactly the per-lane endpoint interpolation, without
  * renormalising one band or lifting it away from its neighbouring material. */
 return add(row->floor,add(mul(row->right,lane),mul(row->up,lift)));
}
static unsigned neon_road_ink(unsigned ink,float fade){
 return RGB((int)((ink&255)*fade),(int)(((ink>>8)&255)*fade),(int)(((ink>>16)&255)*fade));
}
static int neon_road_kind(float q){int k=surface_floor(q);if(k<0||k>=RACE_GATES)return 2;return race_track_surface(&game.race,k,q-k);}
static int neon_road_lod(int cell,int wanted,float step){
 /* Global alignment keeps distant chords/material averages stable as the
  * camera advances. Halving at a boundary leaves adjacent endpoints shared. */
 while(wanted>1&&cell%wanted)wanted/=2;if(wanted==1)return 1;
 int kind=-1;
 for(int n=0;n<wanted;n++){
  int a=neon_road_kind((cell+n+10.f/28.f)*step),b=neon_road_kind((cell+n+24.f/28.f)*step);
  if(kind<0)kind=a;if(a!=kind||b!=kind)return 1;
 }
 /* Preserve bends/loops when grouping distant cells. Measure the actual
  * midpoint against its chord at both shoulders and the centre: no merging
  * when the difference exceeds two thirds of one native screen pixel. */
 while(wanted>1){
  NeonRoadRow a,m,b;float q=cell*step;int acceptable=neon_road_row(q,&a)&&neon_road_row(q+wanted*step*.5f,&m)&&neon_road_row(q+wanted*step,&b);
  float edge=(kind==1?110:190)*2.5f*race_neon_scale(&game.race);
  for(int side=-1;side<=1&&acceptable;side++){
   Vec3 exact=neon_camera(neon_road_point(&m,side*edge,0));
   Vec3 chord=neon_camera(mul(add(neon_road_point(&a,side*edge,0),neon_road_point(&b,side*edge,0)),.5f));
   if(exact.z<64||chord.z<64){acceptable=0;break;}
   Point p=project(exact),r=project(chord);acceptable=fabsf(p.x-r.x)<=.65f&&fabsf(p.y-r.y)<=.65f;
  }
  if(acceptable)break;wanted/=2;
 }
 return wanted;
}
static void neon_road_strip(const NeonRoadRow *a,const NeonRoadRow *b,float lo,float hi,float lift,unsigned ink){
 neon_quad(neon_road_point(a,lo,lift),neon_road_point(a,hi,lift),neon_road_point(b,hi,lift),neon_road_point(b,lo,lift),ink);
}
/* Smooth vertex colours have sharp polygon edges; the light itself fades
 * continuously across the asphalt instead of filling whole cells. */
static void neon_road_lit_strip(const NeonRoadRow *a,const NeonRoadRow *b,float lo,float hi,unsigned ca,unsigned cb,unsigned cc,unsigned cd){
 Vec3 v[4]={neon_camera(neon_road_point(a,lo,0)),neon_camera(neon_road_point(a,hi,0)),neon_camera(neon_road_point(b,hi,0)),neon_camera(neon_road_point(b,lo,0))};
 if(v[0].z<64||v[1].z<64||v[2].z<64||v[3].z<64){neon_road_strip(a,b,lo,hi,0,planet_rgb_mix(ca,cc,128));return;}
 Point p[4];for(int n=0;n<4;n++)p[n]=project(v[n]);unsigned colors[4]={ca,cb,cc,cd};
 for(int n=1;n<3&&drawcount<2048;n++){DrawTri *t=&drawlist[drawcount++];render_submitted++;t->p[0]=p[0];t->p[1]=p[n];t->p[2]=p[n+1];t->color=colors[0];t->material=0x40000000|(colors[n]&0xffffff);t->order=colors[n+1]&0xffffff;t->depth=(v[0].z+v[n].z+v[n+1].z)/3;}
}
static unsigned neon_road_light(float amber,float cyan,float lane,float edge,float fade){

 float side=fabsf(lane/edge);side*=side;
 return neon_road_ink(RGB((int)(38+amber*19+side*51),(int)(25+cyan*12+side*(cyan*29+amber*12)),(int)(54+cyan*14+side*27)),fade);
}
static void neon_road_part(const NeonRoadRow *a,const NeonRoadRow *b,int kind,int panel,unsigned floor,unsigned pink,float unit,float scale,float fade,float seam){
 if(kind==2)return;float edge=(kind?110:190)*unit,inside=edge-15*unit;
 float lanes[5]={-inside,-inside*.65f,0,inside*.65f,inside};
 Vec3 la=mul(sub(a->floor,game.race.start),1/scale),lb=mul(sub(b->floor,game.race.start),1/scale);float qa=(dot(la,game.race.dir)+dot(la,game.race.right)*.35f)*.004f,qb=(dot(lb,game.race.dir)+dot(lb,game.race.right)*.35f)*.004f,aa=fmaxf(0,cosf(qa)),ac=fmaxf(0,sinf(qa)),ba=fmaxf(0,cosf(qb)),bc=fmaxf(0,sinf(qb));unsigned ca[5],cb[5];for(int i=0;i<5;i++){ca[i]=neon_road_light(aa,ac,lanes[i],edge,fade);cb[i]=neon_road_light(ba,bc,lanes[i],edge,fade);}
 Vec3 light_distance=sub(a->floor,neon_eye);int light_step=dot(light_distance,light_distance)>36000000?2:1;
 for(int i=0;i<4;i+=light_step)neon_road_lit_strip(a,b,lanes[i],lanes[i+light_step],ca[i],ca[i+light_step],cb[i+light_step],cb[i]);
 unsigned rim=neon_road_ink(RGB(170,31,127),fade),inner=neon_road_ink(RGB(68,26,79),fade);
 neon_road_lit_strip(a,b,-edge,-inside,rim,inner,inner,rim);neon_road_lit_strip(a,b,inside,edge,inner,rim,rim,inner);
}
/* A restrained coverage fringe softens only the stair-step pixel boundary;
 * the one-pixel hot core remains sharp, preserving the low-poly silhouette. */
static void neon_road_aa_line(Vec3 a,Vec3 b,unsigned ink,float distance){
 if(distance<900){NeonStrokeSegment stroke;float fade=fmaxf(0,fminf(1,(900-distance)/520));if(neon_stroke_camera(neon_camera(a),neon_camera(b),&stroke))neon_stroke_draw(&stroke,5,ink,(int)(40+fade*48));}
 neon_line(a,b,ink);
}
static void neon_road_marks(const NeonRoadRow *a,const NeonRoadRow *b,int kind,float unit,float scale,float forward_distance){
 if(kind==2)return;
 float distant=fmaxf(0,forward_distance),fade=fmaxf(.22f,fminf(1,(2100-distant)/1300));
 for(int side=-1;side<=1;side+=2){
  neon_road_aa_line(neon_road_point(a,side*(kind?110:190)*unit,scale),neon_road_point(b,side*(kind?110:190)*unit,scale),neon_road_ink(RGB(246,88,207),fade),distant);
  neon_line(neon_road_point(a,side*70*unit,scale),neon_road_point(b,side*70*unit,scale),neon_road_ink(RGB(91,50,82),fade));
 }
 /* The central guide keeps a crisp core but occupies two native pixels in
  * the playable field. It converges to one stable line before becoming
  * sub-pixel, avoiding the distant shimmer of a permanently wide stripe. */
 neon_road_aa_line(neon_road_point(a,0,scale*1.15f),neon_road_point(b,0,scale*1.15f),neon_road_ink(RGB(255,184,237),fade),distant);

}
static void __attribute__((noinline,optimize("O2"))) race_neon_authored_road(void){
 uint64_t geometry_started=sceKernelGetSystemTimeWide();
 SpaceRace *r=&game.race;race_floor_prepare(r);RaceTrackFrame nearest=race_track_nearest(r,r->gate,game.pos);
 float scale=race_neon_scale(r),unit=2.5f*scale,step=28.f*scale*(RACE_GATES/r->laps)/(r->top_speed*46.f),nearest_q=nearest.section+nearest.t;
 int first=surface_floor(nearest_q/step),block_count=0;NeonRoadVisible visible[108];
 for(int offset=-8;offset<=99;){int cell=first+offset,wanted=offset<12?1:offset<32?2:offset<64?4:8,stride=neon_road_lod(cell,wanted,step);
  /* Stop each band on its exact boundary before increasing the LOD stride. */
  if(offset<12&&offset+stride>12)stride=12-offset;else if(offset<32&&offset+stride>32)stride=32-offset;else if(offset<64&&offset+stride>64)stride=64-offset;
  visible[block_count++]=(NeonRoadVisible){cell,-1,stride};offset+=stride;
 }
 /* Submit every opaque part far to near in one bounded queue. No fine pink
  * strip is laid over a differently tessellated floor or flushed separately. */
 for(int i=block_count-1;i>=0;i--){int cell=visible[i].cell,stride=visible[i].stride;float q=cell*step,mid=(cell+stride*(20.f/28.f))*step,end=(cell+stride)*step,forward_distance=(q-nearest_q)/step*28;
  if(forward_distance>2600)continue;int first_kind=neon_road_kind((q+mid)*.5f),last_kind=neon_road_kind((mid+end)*.5f);if(first_kind==2&&last_kind==2)continue;
  NeonRoadRow a,b,c;if(!neon_road_row(q,&a)||!neon_road_row(mid,&b)||!neon_road_row(end,&c))continue;
  Vec3 centre=mul(add(a.floor,c.floor),.5f);float radius=200*unit+28*scale*stride;if(!neon_visible(centre,radius))continue;
  float fade=surface_max(.18f,1-surface_max(0,forward_distance)/2900.f);
  unsigned magnetic=neon_road_ink(RGB(15,24,43),fade),flight=neon_road_ink(RGB(31,13,43),fade),pink=neon_road_ink(RGB(201,48,163),fade);
  /* One joint per four fine cells supplies scale without a high-frequency
   * comb. Fade it by its projected native-screen thickness: fully gone below
   * one pixel, smooth through two pixels, and stable under bank/boost motion. */
  float seam=0;if(stride==1&&!(cell&3)){Point pa=project(neon_camera(a.floor)),pb=project(neon_camera(b.floor));float span=fmaxf(fabsf((float)(pb.x-pa.x)),fabsf((float)(pb.y-pa.y)));float pixels=span*(first_kind==1?.075f:.06f);seam=fmaxf(0,fminf(1,(pixels-.75f)/1.35f));}
  if(stride>1&&first_kind==0)flight=planet_rgb_mix(flight,neon_road_ink(RGB(100,63,105),fade),12);
  if(first_kind==last_kind)neon_road_part(&a,&c,first_kind,1,first_kind?magnetic:flight,pink,unit,scale,fade,seam);
  else {neon_road_part(&a,&b,first_kind,1,first_kind?magnetic:flight,pink,unit,scale,fade,seam);
   neon_road_part(&b,&c,last_kind,0,last_kind?magnetic:flight,pink,unit,scale,fade,0);}
  visible[i].kinds=first_kind|(last_kind<<2);visible[i].a=a;visible[i].b=b;visible[i].c=c;
 }
 uint64_t raster_started=sceKernelGetSystemTimeWide();neon_road_us[0]+=raster_started-geometry_started;neon_flush();uint64_t raster_finished=sceKernelGetSystemTimeWide();neon_road_us[1]+=raster_finished-raster_started;
 race_neon_cloud_sky();uint64_t marks_started=sceKernelGetSystemTimeWide();
 int old_lines=race_deck_lines;race_deck_lines=2;
 /* Marks use the same row positions, with a tiny physical lift instead of a
  * broad depth bias that could reveal lines through trackside architecture. */
 for(int i=0;i<block_count;i++){if(visible[i].kinds<0)continue;int cell=visible[i].cell,stride=visible[i].stride,first_kind=visible[i].kinds&3,last_kind=(visible[i].kinds>>2)&3;float q=cell*step,mid=(cell+stride*(20.f/28.f))*step,end=(cell+stride)*step,forward_distance=(q-nearest_q)/step*28;
  NeonRoadRow a=visible[i].a,b=visible[i].b,c=visible[i].c;
  /* Distant mechanical repeats occupy only a fraction of a pixel. Resolve
   * their coverage toward the floor, instead of bright full-pixel strands.
   * Only near decals may borrow a bounded adjacent floor depth sample. */
  int normal_lines=stride==1?2:0;race_deck_lines=neon_strict_seams?-1:normal_lines;
  float coverage=1.f/stride,joint_lift=neon_strict_seams?0:scale;
  (void)coverage;(void)joint_lift;
  race_deck_lines=normal_lines;
  if(first_kind==last_kind)neon_road_marks(&a,&c,first_kind,unit,scale,forward_distance);else{neon_road_marks(&a,&b,first_kind,unit,scale,forward_distance);neon_road_marks(&b,&c,last_kind,unit,scale,forward_distance);}
  if(stride==1&&forward_distance<950&&!(cell%8)&&first_kind!=2){float rim=(first_kind?103:183)*unit;
   for(int side=-1;side<=1;side+=2){Vec3 start=neon_road_point(&a,side*rim,scale*2),endp=neon_road_point(&b,side*rim,scale*2);
    neon_glow_line(start,endp,8,RGB(25,180,210),RGB(122,255,255),75);
   }
  }
 }
 race_deck_lines=old_lines;neon_road_us[2]+=sceKernelGetSystemTimeWide()-marks_started;neon_road_frames++;
}
