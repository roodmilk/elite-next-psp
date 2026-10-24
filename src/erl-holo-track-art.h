/* World-space translucent pink ribbon. No heap/texture/asset loads. Fixed
 * section/tessellation/raster budgets; opaque meshes always occlude the floor. */
static uint64_t race_pad_us;static unsigned race_pad_frames;
static int race_floor_cells,race_floor_samples;
typedef struct {Vec3 a,b;unsigned ink;} RaceFloorLine;
static RaceFloorLine race_floor_lines[768];static int race_floor_line_count;
static RaceFloorLine race_floor_grid[512];static int race_floor_grid_count;
static void race_floor_gridline(Vec3 a,Vec3 b,unsigned ink){if(race_floor_grid_count<512)race_floor_grid[race_floor_grid_count++]=(RaceFloorLine){a,b,ink};}
static void race_floor_line(Vec3 a,Vec3 b,unsigned ink){if(race_floor_line_count<768)race_floor_lines[race_floor_line_count++]=(RaceFloorLine){a,b,ink};}
static uint64_t race_floor_us;static unsigned race_floor_frames;
static int race_round_fast,race_round_horizon=112;
static uint64_t race_floor_stage_us[4];
/* Rendering only reads these axes; ahead/section/t remain in public frames.
 * Seven slots retain the same 33 samples without redundant cached fields. */
typedef struct {Vec3 centre,right,up;} RaceFloorFrame;
_Static_assert(sizeof(RaceFloorFrame)==36,"Render floor cache must contain only three vectors");
static RaceFloorFrame race_floor_frame[7][33];static int race_floor_owner[7];
static Vec3 race_floor_nodes[RACE_GATES+1];static int race_floor_cached,race_floor_laps,race_floor_layout;
/* Authored pad geometry is immutable for a course. Cache its true curved
 * surface rather than evaluating hundreds of spline frames every view. */
typedef struct {Vec3 bed[21][2],arrow[5][2][6];} RacePadArt;
static RacePadArt race_pad_mesh[7];
static Vec3 race_pad_surface(const SpaceRace *r,int k,float t,float lane,float lift){RaceTrackFrame f=race_track_frame(r,k,t);return add(f.centre,add(mul(f.right,(r->layout==4?0:2500)+lane),mul(f.up,RACE_TRACK_FLOOR+4+lift)));}
static void race_pad_prepare(const SpaceRace *r,int k){
 RacePadArt *p=&race_pad_mesh[k%7];
 for(int i=0;i<=20;i++)for(int side=0;side<2;side++)p->bed[i][side]=race_pad_surface(r,k,.32f+i*.01875f,side?1100:-1100,0);
 for(int j=0;j<5;j++)for(int side=0;side<2;side++){
  float u=.32f+j*.075f,lane=side?500:-500;
  const float t[6]={.012f,.063f,.012f,.012f,.048f,.012f},x[6]={-350,0,350,220,0,-220};
  for(int v=0;v<6;v++)p->arrow[j][side][v]=race_pad_surface(r,k,u+t[v],lane+x[v],16);
 }
}
static void race_floor_prepare(const SpaceRace *r){
 int changed=!race_floor_cached||race_floor_laps!=r->laps||race_floor_layout!=r->layout||memcmp(&race_floor_nodes[0],&r->start,sizeof(Vec3));
 for(int k=0;k<RACE_GATES&&!changed;k++)changed=memcmp(&race_floor_nodes[k+1],&r->gates[k].pos,sizeof(Vec3));
 if(changed){race_floor_nodes[0]=r->start;for(int k=0;k<RACE_GATES;k++){race_floor_nodes[k+1]=r->gates[k].pos;}for(int i=0;i<7;i++)race_floor_owner[i]=-1;race_floor_laps=r->laps;race_floor_layout=r->layout;race_floor_cached=1;}
 for(int k=r->gate>2?r->gate-2:0;k<RACE_GATES&&k<=r->gate+4;k++)if(race_floor_owner[k%7]!=k){for(int i=0;i<=32;i++){RaceTrackFrame f=race_track_frame(r,k,i/32.f);race_floor_frame[k%7][i]=(RaceFloorFrame){f.centre,f.right,f.up};}if(r->layout!=5)race_pad_prepare(r,k);race_floor_owner[k%7]=k;}
}
static Vec3 race_floor_point(RaceTrackFrame f,float lane){return add(f.centre,add(mul(f.right,lane),mul(f.up,RACE_TRACK_FLOOR)));}
static Vec3 race_floor_cached_point(RaceFloorFrame f,float lane){return add(f.centre,add(mul(f.right,lane),mul(f.up,RACE_TRACK_FLOOR)));}
/* A road-attached camera always has solid deck immediately beneath it.  Lay
 * that near field before ships/scenery, then project authored rails and
 * panels over it.  This removes near-plane holes without paying to depth
 * rasterise a screen-sized polygon after every actor. */
static void race_round_near_deck(void){
 SpaceRace *r=&game.race;if(r->layout!=4)return;
 RaceTrackFrame nearest=race_track_nearest(r,r->gate,game.pos);int surface=race_track_surface(r,nearest.section,nearest.t),per=RACE_GATES/r->laps;
 if(surface==2){
  /* Across the deliberate gap, paint the real landing centreline back
   * into view.  The old fast path showed only two remote edge pixels,
   * making the jump look like empty space.  A few projected scanlines give
   * a solid, readable destination for a fraction of a mesh raster pass. */
  int lap=(r->gate/per)*per;Point previous_l={0},previous_r={0};int have=0;
  for(int j=0;j<34;j++){float u=.453125f+j*.0026f,q=u*per;int k=lap+(int)q;if(k>=RACE_GATES)break;RaceTrackFrame f=race_track_frame(r,k,q-(int)q);float half=u<.5f?3900:8500;Vec3 l=camera(&game,race_floor_point(f,-half)),rr=camera(&game,race_floor_point(f,half));if(l.z<15||rr.z<15)continue;Point pl=project(l),pr=project(rr);unsigned ink=u<.5f?RGB(45,46,66):RGB(55,28,69);line(pl.x,pl.y,pr.x,pr.y,ink);line(pl.x,pl.y+1,pr.x,pr.y+1,ink);if(have){line(previous_l.x,previous_l.y,pl.x,pl.y,RGB(255,96,205));line(previous_r.x,previous_r.y,pr.x,pr.y,RGB(255,96,205));}previous_l=pl;previous_r=pr;have=1;Point mid={(pl.x+pr.x)/2,(pl.y+pr.y)/2,1};pixel(mid.x,mid.y,RGB(255,154,221));}
  return;
 }
 float lap_u=((nearest.section%per)+nearest.t)/per;
 /* Join the scanline deck to the immediately following authored section.
  * Looking three sections ahead made its vanishing point disagree with the
  * projected road and exposed a floating horizontal seam on bends. */
 int section=r->gate+1;if(section>=RACE_GATES)section=RACE_GATES-1;Vec3 far=camera(&game,race_track_frame(r,section,.5f).centre);int horizon=112;
 Point aim={W/2,horizon,1};if(far.z>15){aim=project(far);horizon=(int)aim.y;}horizon=horizon<88?88:horizon>154?154:horizon;float farx=fmaxf(150,fminf(330,aim.x));
 race_round_horizon=horizon;
 int bottom=surface_y_max<0?H-1:surface_y_max;for(int y=horizon;y<=bottom;y++){float t=(y-horizon)/(float)fmaxf(1,bottom-horizon),curve=t*t,center=farx+(W*.5f-farx)*t,half=surface==1?42+curve*118:64+curve*205;int left=fmaxf(0,center-half),right=fminf(W-1,center+half);if(left>=right)continue;
  int width=right-left,shoulder=fmaxf(3,width*.17f),core_l=left+shoulder,core_r=right-shoulder,depth=y-horizon;
  if(surface==1){
   /* The magnetic quarter is a machine, not a tinted hologram: quiet
    * charcoal running plates surrounded by broad illuminated shoulder
    * blocks.  Sparse scanline spans are much cheaper than a second mesh
    * pass and remain perfectly stable at PSP resolution. */
   unsigned core=((depth/13)&1)?RGB(38,40,58):RGB(43,44,64);rect(core_l,y,core_r-core_l+1,1,core);
   int plate=(depth/8)&3;unsigned edge=plate==0?RGB(229,91,190):plate==1?RGB(117,48,112):plate==2?RGB(190,67,157):RGB(67,38,79);
   rect(left,y,shoulder,1,edge);rect(core_r+1,y,right-core_r,1,edge);
   if(!(depth%13))line(core_l,y,core_r,y,RGB(78,79,101));
   if(!(depth%24)){rect(left,y,shoulder,2,RGB(255,171,225));rect(core_r+1,y,right-core_r,2,RGB(255,171,225));}
   int lane=width*.16f;pixel(center-lane,y,RGB(83,88,111));pixel(center+lane,y,RGB(83,88,111));
   pixel(core_l,y,RGB(255,184,231));pixel(core_r,y,RGB(255,184,231));
  }else{
   /* Free flight remains a translucent, uncluttered ERL ribbon. */
   rect(left,y,width+1,1,((depth/24)&1)?RGB(51,26,64):RGB(57,28,70));
   int lane=width*.25f;pixel(center-lane,y,RGB(88,43,91));pixel(center+lane,y,RGB(88,43,91));
   if(!(depth%24))line(left,y,right,y,RGB(79,38,82));
  }
  int mid=center;pixel(mid,y,(y&1)?RGB(255,151,221):RGB(181,69,160));
  pixel(left,y,surface?RGB(255,108,208):RGB(195,73,171));pixel(right,y,surface?RGB(255,108,208):RGB(195,73,171));
 }
 /* Three bold Ion Cyan floor chevrons mark the booster lip.  They are
  * anchored to the road projection and remain behind ships. */
 if(lap_u>=.375f&&lap_u<.421875f){for(int c=0;c<3;c++){int y=bottom-29-c*31;if(y<=horizon+5)continue;float t=(y-horizon)/(float)fmaxf(1,bottom-horizon),curve=t*t,center=farx+(W*.5f-farx)*t,half=42+curve*118,reach=fmaxf(11,half*.33f);for(int thick=0;thick<3;thick++){line(center-reach,y+thick,center,y+9+thick,RGB(99,255,250));line(center,y+9+thick,center+reach,y+thick,RGB(99,255,250));}}}
}
/* Bounded first-course road: sparse readable grid, solid magnetic panels,
 * and an actual open launch gap. Both camera views share this geometry. */
static void race_round_track_draw(void){
 SpaceRace *r=&game.race;race_floor_prepare(r);int first=r->gate>1?r->gate-1:0,last=r->gate+4;if(last>=RACE_GATES)last=RACE_GATES-1;
 if(race_round_fast)race_floor_line_count=0;
 for(int k=first;k<=last;k++)for(int i=0;i<8;i++){
  float t=i/8.f;RaceFloorFrame a=race_floor_frame[k%7][i*4],b=race_floor_frame[k%7][i*4+4];int kind=race_track_surface(r,k,t+.0625f);if(kind==2)continue;
  Vec3 middle=mul(add(a.centre,b.centre),.5f);if(!render_sphere_visible(middle,10000)||length(sub(middle,camera_origin(&game)))>80000)continue;
  float width=kind?(race_round_fast?3900:5200):(race_round_fast?8500:8500),painted=kind?(race_round_fast?3500:650):width,core=kind?(race_round_fast?2450:500):width;Vec3 v[4]={race_floor_cached_point(a,-core),race_floor_cached_point(a,core),race_floor_cached_point(b,core),race_floor_cached_point(b,-core)};
  unsigned ink=kind?RGB(42,29,60):RGB(54,29,66);
  if(!race_round_fast){queue_triangle(camera(&game,v[0]),camera(&game,v[1]),camera(&game,v[2]),ink);queue_triangle(camera(&game,v[0]),camera(&game,v[2]),camera(&game,v[3]),ink);}
  if(kind)for(int side=-1;side<=1;side+=2){Vec3 q[4]={race_floor_cached_point(a,side*core),race_floor_cached_point(a,side*painted),race_floor_cached_point(b,side*painted),race_floor_cached_point(b,side*core)};unsigned pink=(i&1)?RGB(97,38,94):RGB(210,77,174);if(!race_round_fast){queue_triangle(camera(&game,q[0]),camera(&game,q[1]),camera(&game,q[2]),pink);queue_triangle(camera(&game,q[0]),camera(&game,q[2]),camera(&game,q[3]),pink);}}
  if(race_round_fast){
   /* The scanline deck already supplies near panels and lanes.  Only retain
    * the few perspective cues which describe the authored curve at range;
    * duplicating every lane here costs pixels and turns the horizon into a
    * knot of lines. */
   race_floor_line(race_floor_cached_point(a,0),race_floor_cached_point(b,0),RGB(237,144,214));
   for(int side=-1;side<=1;side+=2)race_floor_line(race_floor_cached_point(a,side*width),race_floor_cached_point(b,side*width),RGB(164,65,145));
   if(!(i&3))race_floor_line(v[0],v[1],RGB(91,45,91));
   int per=RACE_GATES/r->laps;float u=((k%per)+t)/per;if(u>=.402f&&u<.422f)race_floor_line(race_floor_cached_point(a,-500),race_floor_cached_point(a,500),RGB(109,255,249));
   continue;
  }
  race_floor_line(race_floor_cached_point(a,0),race_floor_cached_point(b,0),RGB(237,144,214));
  for(int lane=kind?-1:-2;lane<=(kind?1:2);lane++)if(lane)race_floor_line(race_floor_cached_point(a,lane*(kind?2800.f:2800.f)),race_floor_cached_point(b,lane*(kind?2800.f:2800.f)),kind?RGB(70,58,86):RGB(88,48,89));
  if(kind)for(int side=-1;side<=1;side+=2)race_floor_line(race_floor_cached_point(a,side*6500),race_floor_cached_point(b,side*6500),RGB(244,105,202));
  if(!kind&&length(sub(middle,camera_origin(&game)))<14000)for(int lane=-5;lane<=5;lane++)if(lane)race_floor_line(race_floor_cached_point(a,lane*160.f),race_floor_cached_point(b,lane*160.f),RGB(91,47,88));
  if(!(i&1))race_floor_line(v[0],v[1],kind?RGB(90,96,118):RGB(88,44,84));if(kind&&!(i&3))race_floor_line(race_floor_cached_point(a,-6500),race_floor_cached_point(a,6500),RGB(151,69,135));
  for(int side=-1;side<=1;side+=2){race_floor_line(race_floor_cached_point(a,side*width),race_floor_cached_point(b,side*width),kind?RGB(105,60,112):RGB(114,51,104));if(kind){race_floor_line(race_floor_cached_point(a,side*painted),race_floor_cached_point(b,side*painted),RGB(255,177,231));if(!(i&3))race_floor_line(race_floor_cached_point(a,side*(core+180)),race_floor_cached_point(a,side*(painted-180)),WHITE);}}
  int per=RACE_GATES/r->laps;float u=((k%per)+t)/per;
  if(u>=.402f&&u<.422f)race_floor_line(race_floor_cached_point(a,-500),race_floor_cached_point(a,500),RGB(109,255,249));
 }
 if(!race_round_fast){flush_solid_meshes();race_deck_lines=2;for(int i=0;i<race_floor_line_count;i++)race_boundary_line(race_floor_lines[i].a,race_floor_lines[i].b,race_floor_lines[i].ink);race_deck_lines=0;}
}
static void race_round_track_lines(void){if(game.race.layout!=4)return;race_deck_lines=2;for(int i=0;i<race_floor_line_count;i++){Vec3 a=camera(&game,race_floor_lines[i].a),b=camera(&game,race_floor_lines[i].b);if(a.z>1000&&b.z>1000){Point pa=project(a),pb=project(b);if(pa.y>=race_round_horizon-1&&pb.y>=race_round_horizon-1)race_boundary_line(race_floor_lines[i].a,race_floor_lines[i].b,race_floor_lines[i].ink);}}race_deck_lines=0;}
static void race_holo_track_draw(void){
 race_floor_cells=race_floor_samples=race_floor_line_count=race_floor_grid_count=0;if(!race_is_active(&game))return;
 if(game.race.layout==4){uint64_t begin=sceKernelGetSystemTimeWide();race_round_track_draw();race_floor_us+=sceKernelGetSystemTimeWide()-begin;race_floor_frames++;return;}
 uint64_t begin=sceKernelGetSystemTimeWide();SpaceRace *r=&game.race;race_floor_prepare(r);Vec3 eye=camera_origin(&game);int old_samples=race_wall_samples;unsigned old_ink=race_glass_ink;
 uint64_t prepared=sceKernelGetSystemTimeWide();race_floor_stage_us[0]+=prepared-begin;
 race_wall_samples=0;race_glass_ink=RGB(232,46,163);race_deck_begin();
 int first=r->gate>2?r->gate-2:0,last=r->gate+4;if(last>=RACE_GATES)last=RACE_GATES-1;
 /* Spend the fixed geometry budgets on the current road first. Behind
  * sections must never consume the near-grid or floor budget before it. */
 static const int order[7]={0,1,-1,2,-2,3,4};
 for(int visit=0;visit<7;visit++){int k=r->gate+order[visit];if(k<first||k>last)continue;
  float distance=fminf(length(sub(race_floor_frame[k%7][16].centre,eye)),fminf(length(sub(race_floor_frame[k%7][0].centre,eye)),length(sub(race_floor_frame[k%7][32].centre,eye))));if(distance>75000)continue;
  int segments=distance<18000?(clipx1-clipx0>200?32:16):distance<35000?16:8;race_deck_far=distance>30000;
  float grid_distance=0;
  for(int i=0;i<segments;i++){
   RaceFloorFrame a=race_floor_frame[k%7][i*32/segments],b=race_floor_frame[k%7][(i+1)*32/segments];
   if(!render_sphere_visible(mul(add(a.centre,b.centre),.5f),11000))continue;
   Vec3 p[4]={race_floor_cached_point(a,-8500),race_floor_cached_point(a,8500),race_floor_cached_point(b,8500),race_floor_cached_point(b,-8500)};
   if(race_deck_gpu){const float lane[4]={-8500,-5300,5300,8500};unsigned opacity[4]={0,race_deck_far?128:191,race_deck_far?128:191,0};
    for(int band=0;band<3;band++){Vec3 v[4]={race_floor_cached_point(a,lane[band]),race_floor_cached_point(a,lane[band+1]),race_floor_cached_point(b,lane[band+1]),race_floor_cached_point(b,lane[band])};race_ge_triangle(v[0],v[1],v[2],opacity[band],opacity[band+1],opacity[band+1]);race_ge_triangle(v[0],v[2],v[3],opacity[band],opacity[band+1],opacity[band]);}
   }else{
   race_deck_lanes[0]=-8500;race_deck_lanes[1]=race_deck_lanes[2]=8500;race_wall_triangle(p[0],p[1],p[2],40);
   race_deck_lanes[0]=race_deck_lanes[2]=-8500;race_deck_lanes[1]=8500;race_wall_triangle(p[0],p[2],p[3],40);
   }
   race_floor_cells++;
   float view_depth=(camera(&game,p[0]).z+camera(&game,p[2]).z)*.5f;
   unsigned dim=view_depth<10000?RGB(95,40,93):RGB(44,24,49),pink=view_depth<10000?RGB(255,153,226):view_depth<22000?RGB(145,63,126):RGB(67,31,67);
   race_floor_line(race_floor_cached_point(a,-7100),race_floor_cached_point(b,-7100),dim);race_floor_line(race_floor_cached_point(a,7100),race_floor_cached_point(b,7100),dim);
   race_floor_line(race_floor_cached_point(a,0),race_floor_cached_point(b,0),pink);
   if(distance<22000){race_floor_line(race_floor_cached_point(a,-24),race_floor_cached_point(b,-24),RGB(174,62,143));race_floor_line(race_floor_cached_point(a,24),race_floor_cached_point(b,24),RGB(174,62,143));}
   /* Limit the fine lattice to the nearby visible road. At low ship-height
    * camera framing, kilometre-wide cells disappear into the horizon. */
   Vec3 ca=camera(&game,a.centre),cb=camera(&game,b.centre);
   float lo=0,hi=1;
   if(fabsf(cb.z-ca.z)>.01f){float near=(40-ca.z)/(cb.z-ca.z),far=(3200-ca.z)/(cb.z-ca.z);lo=fmaxf(0,fminf(near,far));hi=fminf(1,fmaxf(near,far));}
   else if(ca.z<40||ca.z>3200)hi=-1;
   float leg_length=length(sub(b.centre,a.centre));
   if(lo<hi){
    float grid_depth=fmaxf(ca.z,cb.z);unsigned grid=grid_depth<600?RGB(105,54,100):grid_depth<1600?RGB(80,40,80):RGB(53,27,57);
    for(int lane=-10;lane<=10;lane++)if(lane){Vec3 pa=race_floor_cached_point(a,lane*80.f),pb=race_floor_cached_point(b,lane*80.f);race_floor_gridline(add(pa,mul(sub(pb,pa),lo)),add(pa,mul(sub(pb,pa),hi)),grid);}
    float next=(floorf((grid_distance+leg_length*lo)/240.f)+1)*240.f;
    for(;next<=grid_distance+leg_length*hi;next+=240.f){float t=(next-grid_distance)/fmaxf(1,leg_length);Vec3 left=race_floor_cached_point(a,-5300),right=race_floor_cached_point(a,5300);left=add(left,mul(sub(race_floor_cached_point(b,-5300),left),t));right=add(right,mul(sub(race_floor_cached_point(b,5300),right),t));race_floor_gridline(left,right,grid);}
   }
   grid_distance+=leg_length;
   if(distance<35000){
    for(int side=-1;side<=1;side+=2){race_floor_line(race_floor_cached_point(a,side*2800),race_floor_cached_point(b,side*2800),view_depth<10000?RGB(117,60,111):RGB(49,26,53));
     if(r->boost_time>0&&distance<22000){float kick=fminf(1,r->boost_time/.3f);for(int echo=1;echo<=2;echo++){float offset=echo*35*side*kick;race_floor_line(race_floor_cached_point(a,side*2800+offset),race_floor_cached_point(b,side*2800+offset),echo==1?RGB(176,76,151):RGB(93,39,87));}}}
   }
  }
  /* A travelling ribbon highlight belongs to the section, not each of its
   * tessellation cells. Drawing it once also removes repeated bright seams. */
  if(distance<35000){float t=fmodf(r->elapsed*.22f+k*.37f,1);RaceTrackFrame c=race_track_frame(r,k,t),d=race_track_frame(r,k,fminf(1,t+.065f));
   for(int side=-1;side<=1;side+=2){race_floor_line(race_floor_point(c,side*2800),race_floor_point(d,side*2800),RGB(247,190,231));if(r->boost_time>0&&distance<22000)race_floor_line(race_floor_point(c,side*2800-24),race_floor_point(d,side*2800-24),RGB(255,227,247));}
  }
 }
 uint64_t collected=sceKernelGetSystemTimeWide();race_floor_stage_us[1]+=collected-prepared;
 race_deck_end();uint64_t resolved=sceKernelGetSystemTimeWide();race_floor_stage_us[2]+=resolved-collected;
 race_deck_lines=1;for(int i=0;i<race_floor_grid_count;i++)race_boundary_line(race_floor_grid[i].a,race_floor_grid[i].b,race_floor_grid[i].ink);for(int i=0;i<race_floor_line_count;i++)race_boundary_line(race_floor_lines[i].a,race_floor_lines[i].b,race_floor_lines[i].ink);race_deck_lines=0;race_floor_stage_us[3]+=sceKernelGetSystemTimeWide()-resolved;
 race_floor_samples=race_wall_samples;race_wall_samples=old_samples;race_glass_ink=old_ink;race_floor_us+=sceKernelGetSystemTimeWide()-begin;race_floor_frames++;
}
/* Ion Cyan uses the same five curved cells and surface as swept detection. */
static Vec3 race_pad_ink(const SpaceRace *r,int k,float t,float lane){RaceTrackFrame f=race_track_frame(r,k,t);return add(race_pad_point(r,k,t,lane),mul(f.up,12));}
static void race_pad_art(void){
 uint64_t begin=sceKernelGetSystemTimeWide();
 SpaceRace *r=&game.race;race_floor_prepare(r);int first=r->gate>2?r->gate-2:0;
 for(int k=first;k<RACE_GATES&&k<=r->gate+3;k++){
  Vec3 centre=race_pad_point(r,k,.5f,0);float pad_limit=r->layout==4?5000.f:45000.f;if(length(sub(centre,camera_origin(&game)))>pad_limit||!render_sphere_visible(centre,6000))continue;
  if(r->layout==4){int per=RACE_GATES/r->laps;if(k%per!=per/8&&k%per!=per*3/4)continue;}
  int used=(r->arcade.branch_boost[0]|r->arcade.branch_repair[0])&(1u<<k);unsigned cyan=used?RGB(39,104,120):RGB(100,255,250);
  RacePadArt *p=&race_pad_mesh[k%7];
  for(int j=0;j<20;j++){Vec3 a=p->bed[j][0],b=p->bed[j][1],c=p->bed[j+1][1],d=p->bed[j+1][0];
   queue_triangle(camera(&game,a),camera(&game,b),camera(&game,c),RGB(10,56,78));queue_triangle(camera(&game,a),camera(&game,c),camera(&game,d),RGB(10,56,78));
  }
  /* Opaque bed first, then the raised filled double chevrons. All queues
   * flush in the current viewport so mirror ink cannot leak forward. */
  flush_solid_meshes();
  static const unsigned char faces[4][3]={{0,1,4},{0,4,5},{1,2,3},{1,3,4}};
  for(int j=0;j<5;j++)for(int side=0;side<2;side++)for(int face=0;face<4;face++){
   Vec3 *v=p->arrow[j][side];queue_triangle(camera(&game,v[faces[face][0]]),camera(&game,v[faces[face][1]]),camera(&game,v[faces[face][2]]),cyan);
  }flush_solid_meshes();
  /* The round course already draws its cyan pad silhouette in the scanline
   * deck. Re-projecting the bed perimeter on a curved distant section made
   * that perimeter escape the road and span the horizon. Keep the filled
   * chevrons, but reserve these extra outlines for the other layouts. */
  if(r->layout!=4)for(int side=0;side<2;side++){
   for(int j=0;j<20;j++)race_boundary_line(p->bed[j][side],p->bed[j+1][side],cyan);
   for(int end=0;end<2;end++){int j=end?20:0;race_boundary_line(p->bed[j][side],add(p->bed[j][side],mul(race_floor_frame[k%7][end?22:10].right,side?-250:250)),WHITE);}
  }
 } race_pad_us+=sceKernelGetSystemTimeWide()-begin;race_pad_frames++;
}
