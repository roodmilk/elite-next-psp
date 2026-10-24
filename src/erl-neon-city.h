/* Approved Neon Tokyo architecture, scaled with the shared authored route.
 * Global ground and upright towers stay independent of magnetic road banking.
 * No heap/texture allocations; at most 24 city slabs and 320 visible marks.
 * Regenerate compact positions with tools/generate-neon-city.py. */
/* BEGIN GENERATED CITY STATIONS */
/* xyz in approved design units, horizontal right x/z, normalized arc. */
static const short neon_city_stations[150][6]={
 {0,0,-7200,6416,-32133,0},
 {401,1,-7121,6297,-32156,185},
 {817,3,-7039,6361,-32144,376},
 {1246,6,-6953,6590,-32098,574},
 {1684,9,-6860,6977,-32016,776},
 {2126,12,-6760,7522,-31892,981},
 {2568,16,-6651,8229,-31717,1187},
 {3008,19,-6531,9108,-31476,1393},
 {3441,22,-6398,10173,-31148,1598},
 {3862,25,-6251,11444,-30704,1799},
 {4270,27,-6088,12940,-30104,1998},
 {4659,29,-5907,14680,-29295,2192},
 {5026,30,-5707,16678,-28205,2380},
 {5367,30,-5487,18917,-26755,2564},
 {5682,29,-5243,21260,-24934,2744},
 {5972,26,-4971,23535,-22799,2924},
 {6240,23,-4666,25590,-20466,3107},
 {6488,18,-4325,27323,-18086,3298},
 {6718,12,-3943,28703,-15805,3500},
 {6934,5,-3515,29751,-13731,3716},
 {7136,-3,-3038,30522,-11920,3950},
 {7328,-13,-2507,31076,-10390,4205},
 {7511,-24,-1918,31470,-9129,4484},
 {7688,-36,-1268,31732,-8171,4789},
 {7854,-49,-600,31838,-7748,5100},
 {8000,-60,0,31815,-7842,5379},
 {8116,-68,462,31792,-7935,5594},
 {8195,-72,798,32074,-6702,5750},
 {8232,-75,1038,32682,-2354,5860},
 {8222,-75,1215,31965,7207,5941},
 {8160,-75,1360,27498,17819,6012},
 {8046,-74,1500,23824,22496,6094},
 {7903,-73,1644,23299,23040,6186},
 {7762,-70,1799,25437,20655,6280},
 {7651,-66,1969,29569,14118,6372},
 {7600,-60,2160,32718,1800,6462},
 {7629,-52,2374,31486,-9073,6561},
 {7719,-41,2601,29400,-14469,6671},
 {7843,-29,2824,28007,-17009,6786},
 {7973,-15,3028,27526,-17776,6896},
 {8080,0,3200,28347,-16435,6988},
 {8144,17,3333,31006,-10599,7055},
 {8166,34,3456,32742,-1291,7112},
 {8157,52,3609,32528,3952,7182},
 {8125,71,3831,32411,4820,7284},
 {8080,90,4160,32520,4013,7434},
 {7991,109,4584,30303,12466,7631},
 {7736,128,4933,21771,24489,7828},
 {7361,147,5149,10139,31159,8025},
 {6931,166,5194,-3444,32586,8222},
 {6520,185,5061,-16394,28371,8419},
 {6199,204,4771,-26514,19253,8616},
 {6023,223,4376,-32052,6810,8813},
 {6023,242,3944,-32052,-6810,9010},
 {6199,261,3549,-26514,-19253,9207},
 {6520,280,3259,-16396,-28370,9404},
 {6931,299,3126,-3429,-32587,9601},
 {7362,318,3171,10068,-31182,9798},
 {7735,337,3386,21948,-24330,9995},
 {7992,356,3739,30262,-12566,10193},
 {8080,375,4160,32182,-6166,10388},
 {8138,367,4933,32463,4450,10741},
 {7624,334,6145,28060,16921,11340},
 {6779,297,7396,26411,19394,12023},
 {5991,267,8384,24616,21627,12594},
 {5269,243,9120,21872,24398,13060},
 {4607,224,9631,17664,27598,13439},
 {3995,210,9942,11507,30680,13749},
 {3427,200,10081,3603,32568,14014},
 {2894,193,10073,-4579,32446,14256},
 {2388,189,9943,-11257,30773,14492},
 {1903,187,9720,-15756,28730,14734},
 {1429,185,9427,-18344,27151,14986},
 {959,184,9092,-19461,26362,15247},
 {485,183,8741,-19374,26426,15513},
 {0,180,8400,-18121,27300,15781},
 {-503,175,8089,-16381,28378,16048},
 {-1019,169,7809,-14943,29161,16314},
 {-1545,161,7552,-13839,29701,16578},
 {-2075,152,7314,-13091,30039,16841},
 {-2603,142,7088,-12713,30200,17100},
 {-3125,131,6869,-12722,30196,17356},
 {-3635,118,6651,-13140,30017,17607},
 {-4128,106,6427,-13990,29630,17852},
 {-4600,92,6193,-15301,28975,18090},
 {-5043,79,5941,-17087,27959,18321},
 {-5455,65,5667,-19336,26454,18544},
 {-5829,52,5364,-21968,24312,18762},
 {-6160,38,5027,-24719,21509,18976},
 {-6448,26,4655,-26932,18664,19188},
 {-6697,14,4255,-28578,16030,19401},
 {-6912,3,3833,-29765,13702,19616},
 {-7096,-8,3394,-30598,11724,19831},
 {-7255,-17,2944,-31170,10105,20046},
 {-7392,-24,2490,-31552,8840,20261},
 {-7511,-30,2037,-31795,7923,20473},
 {-7618,-34,1591,-31930,7357,20680},
 {-7715,-37,1158,-31975,7159,20880},
 {-7809,-37,745,-31927,7370,21072},
 {-7902,-35,357,-31759,8066,21252},
 {-8000,-30,0,-31397,9375,21419},
 {-8106,-23,-322,-30836,11082,21573},
 {-8221,-13,-618,-30184,12752,21716},
 {-8347,-2,-896,-29523,14216,21854},
 {-8484,10,-1167,-28960,15330,21992},
 {-8634,24,-1441,-28581,16025,22133},
 {-8797,38,-1727,-28424,16302,22282},
 {-8974,52,-2036,-28469,16222,22443},
 {-9166,66,-2378,-28667,15871,22620},
 {-9375,78,-2763,-28957,15336,22818},
 {-9600,90,-3200,-29288,14693,23040},
 {-9841,100,-3697,-29673,13899,23290},
 {-10088,109,-4248,-30135,12867,23563},
 {-10328,118,-4844,-30654,11577,23853},
 {-10550,128,-5479,-31207,9989,24157},
 {-10740,138,-6142,-31764,8046,24469},
 {-10888,151,-6826,-32271,5677,24785},
 {-10980,167,-7522,-32647,2802,25103},
 {-11004,187,-8222,-32761,-646,25420},
 {-10948,211,-8917,-32430,-4689,25735},
 {-10800,240,-9600,-31434,-9250,26051},
 {-10551,274,-10246,-29365,-14538,26365},
 {-10206,312,-10773,-24351,-21925,26651},
 {-9774,348,-11085,-12456,-30307,26894},
 {-9287,377,-11191,-2538,-32669,27121},
 {-8800,390,-11200,96,-32767,27341},
 {-8354,382,-11204,-468,-32764,27543},
 {-7938,361,-11206,444,-32764,27731},
 {-7527,340,-11188,2131,-32698,27917},
 {-7105,323,-11168,239,-32766,28108},
 {-6667,310,-11200,-5448,-32311,28307},
 {-6216,298,-11295,-5899,-32232,28516},
 {-5789,281,-11297,8857,-31547,28710},
 {-5432,256,-11012,27132,-18373,28922},
 {-5153,219,-10406,31002,-10609,29224},
 {-4926,177,-9604,31818,-7828,29602},
 {-4723,134,-8736,31898,-7495,30005},
 {-4514,93,-7935,31358,-9506,30379},
 {-4275,59,-7327,28819,-15592,30675},
 {-3993,35,-6956,21697,-24554,30887},
 {-3676,18,-6781,8762,-31574,31052},
 {-3332,8,-6757,-3383,-32592,31209},
 {-2969,2,-6840,-10174,-31148,31378},
 {-2594,0,-6984,-12797,-30165,31560},
 {-2215,-1,-7146,-12440,-30314,31746},
 {-1840,0,-7280,-8960,-31518,31926},
 {-1474,0,-7352,-3618,-32567,32095},
 {-1113,0,-7365,970,-32753,32258},
 {-752,0,-7335,4226,-32493,32422},
 {-383,-1,-7275,6006,-32212,32591},
};
static const short neon_city_bays[2][6]={
 {8136,-75,1395,26272,19582,6031},
 {7615,-54,2319,32011,-6998,6535},
};
/* END GENERATED CITY STATIONS */

static Vec3 neon_city_world(const SpaceRace *r,float x,float y,float z,float scale){
 return add(r->start,add(mul(r->right,x*scale),add(mul(r->up,y*scale),mul(r->dir,(z+7200)*scale))));
}
static unsigned neon_city_fog(unsigned ink,float fog){
 /* Palette haze separates depth without filtering or softening an edge. */
 return RGB((int)((ink&255)*(1-fog)+180*fog),(int)(((ink>>8)&255)*(1-fog)+119*fog),(int)(((ink>>16)&255)*(1-fog)+157*fog));
}
typedef struct {Vec3 root,right,ahead;float width,height,depth,fog;int index,side,detail;} NeonCityBuilding;
static void neon_city_axes(const SpaceRace *r,const short *station,Vec3 *right,Vec3 *ahead){
 *right=add(mul(r->right,station[3]/32767.f),mul(r->dir,station[4]/32767.f));
 *ahead=add(mul(r->right,-station[4]/32767.f),mul(r->dir,station[3]/32767.f));
}
/* Near-clip the fog opacity together with geometry, so flying above/beside
 * towers cannot switch the fade to an opaque near-plane fallback. */
static void __attribute__((noinline,optimize("O3","fast-math"))) neon_city_fade_face(Vec3 *v,unsigned ink){
 Vec3 clipped[6];float alpha[6];int count=0;float a[4]={0,0,255,255};
 for(int i=0;i<4;i++){int j=(i+1)&3;int inside=v[i].z>=64,next=v[j].z>=64;
  if(inside){clipped[count]=v[i];alpha[count++]=a[i];}
  if(inside!=next){float t=(64-v[i].z)/(v[j].z-v[i].z);clipped[count]=add(v[i],mul(sub(v[j],v[i]),t));clipped[count].z=64;alpha[count++]=a[i]+(a[j]-a[i])*t;}}
 for(int i=1;i+1<count&&drawcount<2048;i++){DrawTri *t=&drawlist[drawcount++];render_submitted++;t->p[0]=project(clipped[0]);t->p[1]=project(clipped[i]);t->p[2]=project(clipped[i+1]);t->color=ink;t->material=0x20000000|((int)alpha[0])|((int)alpha[i]<<8)|((int)alpha[i+1]<<16);t->order=0;t->depth=(clipped[0].z+clipped[i].z+clipped[i+1].z)/3;}
}
static void neon_city_building(const NeonCityBuilding *b){
 Vec3 up=game.race.up,v[8];float scale=race_neon_scale(&game.race);
 float root_y=dot(sub(b->root,game.race.start),up)/scale;
 /* Original roots remain below the cloud sea; the hidden continuation is
  * omitted only after opacity has reached zero, never as a visible flat cut. */
 float lower=surface_max(0,(-220-root_y)*scale),seam=surface_max(0,(-30-root_y)*scale);
 if(lower>=b->height)return;if(seam>b->height)seam=b->height;
 for(int i=0;i<8;i++){float h=i&4?b->height:seam,taper=1-.06f*h/b->height;
  v[i]=neon_camera(add(b->root,add(mul(b->right,(i&1?1:-1)*b->width*taper),add(mul(up,h),mul(b->ahead,(i&2?1:-1)*b->depth*taper)))));}
 static const unsigned char faces[5][4]={{0,1,5,4},{1,3,7,5},{3,2,6,7},{2,0,4,6},{4,5,7,6}};
 for(int face=0;face<5;face++){const unsigned char *q=faces[face];Vec3 a=v[q[0]],c=v[q[1]],d=v[q[2]],e=v[q[3]];if(dot(race_ui_cross(sub(c,a),sub(d,a)),a)<=0)continue;
  unsigned ink=neon_city_fog(face==4?RGB(101,77,112):(face&1)?RGB(35,31,60):RGB(65,48,82),b->fog);
  queue_triangle(a,c,d,ink);queue_triangle(a,d,e,ink);
  if(face!=4&&seam>lower){Vec3 band[4];
   for(int j=0;j<2;j++){int corner=q[j];float taper=1-.06f*lower/b->height;
    band[j]=neon_camera(add(b->root,add(mul(b->right,(corner&1?1:-1)*b->width*taper),add(mul(up,lower),mul(b->ahead,(corner&2?1:-1)*b->depth*taper)))));}
   band[2]=c;band[3]=a;neon_city_fade_face(band,ink);
  }
 }
}
/* Perspective-sized window clusters with a soft one-pixel fringe. Bounded
 * footprints and depth tests keep light on the correct visible facade. */
static int neon_city_camera_light(Vec3 v,unsigned ink,int wide){
 if(v.z<64)return 0;Point p=project(v);int x=p.x,y=p.y,lit=0;
 int h=wide?(int)(28*race_neon_scale(&game.race)*proj_focal/v.z):1;if(h<1)h=1;if(h>7)h=7;int w=h>2?2:1;
 unsigned depth=mega_encode_depth(v.z);
 for(int dy=-1;dy<=h;dy++)for(int dx=-1;dx<=w;dx++){int xx=x+dx,yy=y+dy;if(xx<0||xx>=W||yy<0||yy>=H||depth>surface_depth[yy*W+xx]+24)continue;
  int core=dx>=0&&dx<w&&dy>=0&&dy<h;unsigned *out=&fb[yy*STRIDE+xx];*out=core?ink:planet_rgb_mix(*out,ink,38);lit=1;
 }return lit;
}
static int neon_city_light(Vec3 position,unsigned ink,int wide){return neon_city_camera_light(neon_camera(position),ink,wide);}
static int __attribute__((noinline,noclone,optimize("O3","fast-math"))) neon_city_row_visible(Vec3 a,Vec3 b){
 if(a.z<15||b.z<15)return a.z>=15||b.z>=15;
 float margin=2.f/proj_focal;
 for(int side=0;side<4;side++){int positive=side==1||side==2;float plane=neon_planes[side]+(positive?margin:-margin);
  float da=(side<2?a.x:a.y)-plane*a.z,db=(side<2?b.x:b.y)-plane*b.z;
  if(positive?(da>0&&db>0):(da<0&&db<0))return 0;
 }return 1;
}
static void __attribute__((noinline,noclone,optimize("O3","fast-math"))) neon_city_spire_line(Vec3 a,Vec3 b,int width,unsigned ink){
 NeonStrokeSegment segment;if(neon_stroke_camera(a,b,&segment))neon_stroke_draw(&segment,width,ink,255);
}
static void __attribute__((noinline,optimize("O3","fast-math"))) neon_city_spire(const NeonCityBuilding *b,Vec3 roof,float scale){
 NeonCityBuilding crown=*b;crown.root=roof;crown.width*=.45f;crown.depth*=.45f;crown.height=90*scale;neon_city_building(&crown);neon_flush();
 Vec3 tip=add(roof,mul(game.race.up,180*scale));neon_line(add(roof,mul(game.race.up,90*scale)),tip,RGB(76,83,112));
 Vec3 v=neon_camera(tip);NeonStrokeSegment spot;if(neon_stroke_camera(v,add(v,(Vec3){.01f,0,0}),&spot)){neon_stroke_draw(&spot,7,RGB(253,41,122),42);neon_stroke_draw(&spot,2,RGB(255,152,166),255);}
}
static void __attribute__((noinline,optimize("O3","fast-math"))) neon_city_details(const NeonCityBuilding *b,int *window_budget,int *attempt_budget,int allow_spire){
 Vec3 up=game.race.up;float scale=race_neon_scale(&game.race),height=b->height/scale;int index=b->index;
 /* Choose the facade with the largest projected horizontal area. The old
  * always-negative-ahead choice hid all windows when viewed from the side. */
 Vec3 eye=sub(camera_origin(&game),b->root),across=b->right,normal=b->ahead;float width=b->width,depth=b->depth,right_eye=dot(eye,b->right),ahead_eye=dot(eye,b->ahead);
 if(fabsf(right_eye)*b->depth>fabsf(ahead_eye)*b->width){across=b->ahead;normal=mul(b->right,right_eye<0?-1:1);width=b->depth;depth=b->width;}else normal=mul(b->ahead,ahead_eye<0?-1:1);
 unsigned warm=neon_city_fog(RGB(255,203,128),b->fog),cyan=neon_city_fog(RGB(244,166,169),b->fog);
 Vec3 camera_across={dot(across,neon_right),dot(across,neon_up),dot(across,neon_ahead)};
 int mark_limit=b->detail?24:4,attempt_floor=b->detail?160:0;
 /* Reference window marks stay one or two native pixels, never huge strips. */
 float visible_base=surface_max(0,(-30-dot(sub(b->root,game.race.start),up)/scale)*scale);
 int floor_step=b->detail?50+(index%4)*7:104+(index%3)*16;
 for(int floor=54+(index%3)*13;floor<height-40&&*window_budget>0&&mark_limit>0&&*attempt_budget>attempt_floor;floor+=floor_step){if(floor*scale<visible_base)continue;float taper=1-.06f*floor/height;
  Vec3 centre=neon_camera(add(b->root,add(mul(up,floor*scale),mul(normal,depth*taper+8*scale)))),step=mul(camera_across,width*.22f*taper),p=add(centre,mul(step,-3));
  /* Rejected rows spend no window attempts; near-crossing rows stay intact. */
  if(!neon_city_row_visible(p,add(p,mul(step,6))))continue;
  /* The facade row is affine in camera space; share its transform and advance
   * all seven authored columns, including gaps in the window cadence. */
  for(int col=-3;col<=3&&*window_budget>0&&mark_limit>0&&*attempt_budget>attempt_floor;col++,p=add(p,step)){int pattern=(floor/floor_step*(11+index%5)+col*(7+index%3)+index*13+b->side+10000)%7;if(pattern<(index%3)+1)continue;
   unsigned ink=(col+index+30)%3?warm:cyan;ink=planet_rgb_mix(ink,RGB(183,133,178),(int)(150*(1-floor/height)));(*attempt_budget)--;if(neon_city_camera_light(p,ink,b->detail)){(*window_budget)--;mark_limit--;}
  }
 }
 unsigned edge=neon_city_fog(RGB(52,65,96),b->fog);Vec3 low=add(add(b->root,mul(up,visible_base)),add(mul(across,-width),mul(normal,depth))),top=add(b->root,add(mul(across,-width*.94f),add(mul(up,b->height),mul(normal,depth*.94f))));
 neon_line(low,top,edge);
 /* The skyline shares this facade outline; close buildings add the far edge. */
 Vec3 opposite_top=add(b->root,add(mul(across,width*.94f),add(mul(up,b->height),mul(normal,depth*.94f))));neon_line(top,opposite_top,edge);
 if(!b->detail)return;
 Vec3 opposite_low=add(add(b->root,mul(up,visible_base)),add(mul(across,width),mul(normal,depth)));neon_line(opposite_low,opposite_top,edge);
 /* One reflected strip grounds close silhouettes in the road's cyan/orange
  * palette. The fixed line replaces any per-pixel dynamic light. */
 {Vec3 glow=add(b->root,add(mul(up,visible_base+(34+(index%3)*17)*scale),mul(normal,depth+2*scale)));float reach=width*(.42f+(index%3)*.12f);unsigned ink=neon_city_fog(index&1?RGB(70,229,242):RGB(255,146,66),b->fog);
  neon_line(add(glow,mul(across,-reach)),add(glow,mul(across,reach)),ink);}
 if(index%3&&b->height*.48f>=visible_base){Vec3 sign=add(b->root,add(mul(across,-width*.65f),add(mul(up,b->height*.48f),mul(normal,depth*(1-.06f*.48f)+4*scale))));unsigned ink=neon_city_fog(index%3==1?RGB(110,198,219):RGB(233,170,118),b->fog);
  for(int stripe=0;stripe<5;stripe++){Vec3 p=add(sign,mul(up,stripe*23*scale));neon_line(add(p,mul(across,-18*scale)),add(p,mul(across,18*scale)),ink);}
  neon_line(sign,add(sign,mul(up,115*scale)),ink);
 }
 Vec3 roof=add(b->root,mul(up,b->height));
 if(!allow_spire){neon_line(roof,add(roof,mul(up,(80+(index%3)*25)*scale)),neon_city_fog(RGB(129,149,172),b->fog));
  if((index+b->side+30)%3==0)for(int h=35;h<=75;h+=20){Vec3 p=add(roof,mul(up,h*scale));neon_line(add(p,mul(b->right,-30*scale)),add(p,mul(b->right,30*scale)),edge);}
 }
}
static void __attribute__((noinline,optimize("O3","fast-math"))) neon_city_landmarks(const SpaceRace *r,int station,float scale){
 /* Ringed tower is the approved visual anchor inside the banked helix. */
 if(station>=35&&station<=67){Vec3 root=neon_city_world(r,7040,-330,4160,scale),centre=add(root,mul(r->up,1025*scale));
  if(neon_visible(centre,2200*scale)){
   NeonCityBuilding b={root,r->right,r->dir,330*scale,2050*scale,330*scale,.12f,0,0,0};neon_city_building(&b);neon_flush();
   for(int h=400;h<=1600;h+=300){Vec3 previous=add(root,add(mul(r->right,360*scale),mul(r->up,h*scale)));
    for(int j=1;j<=16;j++){float a=j*.392699082f;Vec3 p=add(root,add(mul(r->up,h*scale),add(mul(r->right,cosf(a)*360*scale),mul(r->dir,sinf(a)*360*scale))));neon_line(previous,p,RGB(188,115,158));previous=p;}
   }
   Vec3 roof=add(root,mul(r->up,2050*scale)),tip=add(roof,mul(r->up,420*scale));for(int side=-1;side<=1;side+=2){Vec3 a=add(roof,mul(r->right,side*100*scale));neon_line(a,tip,RGB(146,168,201));}
   for(int h=80;h<400;h+=80){float w=(100-h*.23f)*scale;Vec3 p=add(roof,mul(r->up,h*scale));neon_line(add(p,mul(r->right,-w)),add(p,mul(r->right,w)),RGB(181,84,103));}neon_city_light(tip,RGB(255,131,170),1);
  }
 }
 /* Structural bays follow the authored floor, with their uprights entirely
  * outside the 313.5-unit legal corridor. Header clears its full 500 height. */
 for(int i=0;i<2;i++){const short *s=neon_city_bays[i];Vec3 right,ahead;neon_city_axes(r,s,&right,&ahead);Vec3 floor=neon_city_world(r,s[0],s[1],s[2],scale),centre=add(floor,mul(r->up,20*scale));if(!neon_visible(centre,1600*scale)||length(sub(centre,camera_origin(&game)))>7000*scale)continue;
  for(int side=-1;side<=1;side+=2){Vec3 root=add(floor,add(mul(right,side*740*scale),mul(r->up,-650*scale)));NeonCityBuilding b={root,right,ahead,170*scale,1320*scale,230*scale,.1f,0,side,0};neon_city_building(&b);}
  NeonCityBuilding roof={add(floor,mul(r->up,650*scale)),right,ahead,910*scale,130*scale,230*scale,.1f,0,0,0};neon_city_building(&roof);neon_flush();
  for(int side=-1;side<=1;side+=2){Vec3 a=add(floor,add(mul(ahead,-231*scale),add(mul(right,side*565*scale),mul(r->up,20*scale))));neon_line(a,add(a,mul(r->up,630*scale)),RGB(230,179,140));}
  Vec3 p=add(floor,add(mul(r->up,650*scale),mul(ahead,-231*scale)));neon_line(add(p,mul(right,-565*scale)),add(p,mul(right,565*scale)),RGB(243,196,164));
 }
}
/* Two distant low-poly airships; anchored to the world, no collision or AI.
 * Their solid hulls share the existing depth-tested opaque pass. */
static void __attribute__((noinline,optimize("O3","fast-math"))) neon_city_blimps(const SpaceRace *r,float scale){
 for(int i=0;i<2;i++){
  Vec3 centre=neon_city_world(r,(i?4200:-1800)+sinf(r->elapsed*.018f+i)*140,650+i*180,i?3600:-3000,scale);
  if(!neon_visible(centre,400*scale)||length(sub(centre,neon_eye))>16000*scale)continue;
  Vec3 nose=add(centre,mul(r->right,340*scale)),tail=add(centre,mul(r->right,-310*scale)),rim[8];
  for(int n=0;n<8;n++){float angle=n*.785398163f;rim[n]=add(centre,add(mul(r->up,cosf(angle)*85*scale),mul(r->dir,sinf(angle)*105*scale)));}
  for(int n=0;n<8;n++){unsigned ink=neon_city_fog(n<4?RGB(88,64,96):RGB(48,41,72),.25f);queue_triangle(neon_camera(nose),neon_camera(rim[n]),neon_camera(rim[(n+1)&7]),ink);queue_triangle(neon_camera(tail),neon_camera(rim[(n+1)&7]),neon_camera(rim[n]),ink);}
  neon_flush();neon_line(add(centre,mul(r->up,-90*scale)),add(add(centre,mul(r->up,-90*scale)),mul(r->right,80*scale)),RGB(238,171,130));
  neon_city_light(nose,RGB(255,182,169),0);neon_city_light(tail,RGB(242,114,159),0);
 }
}
static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_city_draw(void){
 SpaceRace *r=&game.race;float scale=race_neon_scale(r);RaceTrackFrame nearest=race_track_nearest(r,r->gate,game.pos);int per=RACE_GATES/r->laps,target=(int)((nearest.section%per+nearest.t)*32767/per),station=0;
 while(station<149&&neon_city_stations[station][5]<target)station++;
 NeonCityBuilding visible[36];int count=0,details=0,spire_id[2]={-1,-1};float spire_distance[2]={1.e30f,1.e30f};
 /* Prioritise close architecture before the bounded sparse distant layer.
  * Shared depth and the mesh queue resolve overlap independently of order. */
 for(int offset=-2;offset<=37&&count<36;offset+=offset<12?1:2){int index=(station+offset+150)%150;const short *s=neon_city_stations[index];Vec3 right,ahead;neon_city_axes(r,s,&right,&ahead);
  for(int side=-1;side<=1&&count<36;side+=2){int pattern=index%30;float distance=920+((pattern*7+side+4)%4)*190,height=430+((pattern*13+side*3+6)%6)*105;
   Vec3 root=add(neon_city_world(r,s[0],-330,s[2],scale),mul(right,side*distance*scale)),centre=add(root,mul(r->up,height*.5f*scale));float d=length(sub(centre,camera_origin(&game)));
   if(d>10000*scale||!neon_visible(centre,(height*.5f+350)*scale))continue;
   float depth=d/scale,fog=surface_max(0,surface_min(.78f,(depth-900)/6500.f));int detail=offset<12&&details<8;
   float width=(60+((pattern*3+side+4)%5)*22)*scale,building_depth=(65+pattern%4*25)*scale;
   NeonCityBuilding b={root,right,ahead,width,height*scale,building_depth,fog,pattern,side,detail};visible[count++]=b;details+=detail;
  }
 }
 /* Rooftops compete independently of the window-detail layer. A nearby
  * building may be visible while its entire spire is above the camera. */
 for(int i=0;i<count;i++){NeonCityBuilding *b=&visible[i];if((b->index+b->side+30)%3)continue;
  float height=(180+(b->index%3)*55)*scale*1.13f,radius=b->width*.28f+5*scale;
  Vec3 roof=add(b->root,mul(r->up,b->height)),tip=add(roof,mul(r->up,height)),centre=mul(add(roof,tip),.5f),camera_centre=neon_camera(centre);
  radius=sqrtf(height*height*.25f+2*radius*radius);
  if(camera_centre.z+radius<64||!neon_visible(centre,radius))continue;
  Vec3 low=neon_camera(roof),high=neon_camera(tip);
  if(low.z>=64&&high.z>=64){Point p=project(low),q=project(high);if(fabsf(q.x-p.x)<3&&fabsf(q.y-p.y)<3)continue;}
  Vec3 delta=sub(centre,neon_eye);float distance=dot(delta,delta);int slot=distance<spire_distance[0]?0:distance<spire_distance[1]?1:-1;
  if(slot>=0){if(!slot){spire_id[1]=spire_id[0];spire_distance[1]=spire_distance[0];}spire_id[slot]=i;spire_distance[slot]=distance;}
 }
 for(int i=count-1;i>=0;i--)neon_city_building(&visible[i]);neon_flush();int near_windows=200,far_windows=144,attempts=720;for(int i=0;i<count;i++)neon_city_details(&visible[i],visible[i].detail?&near_windows:&far_windows,&attempts,i==spire_id[0]||i==spire_id[1]);
 for(int i=0;i<2;i++)if(spire_id[i]>=0){NeonCityBuilding *b=&visible[spire_id[i]];neon_city_spire(b,add(b->root,mul(r->up,b->height)),scale);}
 neon_city_landmarks(r,station,scale);neon_city_blimps(r,scale);
}
