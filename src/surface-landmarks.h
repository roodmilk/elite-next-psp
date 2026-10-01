/* Bounded low-poly landmark kit using the existing FieldBuilding reservations.
 * Near details are geometric, depth tested, and never screen-space labels. */
static int landmark_detail;
static const float lm_circle12[13][2]={{1,0},{.8660254f,.5f},{.5f,.8660254f},{0,1},{-.5f,.8660254f},{-.8660254f,.5f},{-1,0},{-.8660254f,-.5f},{-.5f,-.8660254f},{0,-1},{.5f,-.8660254f},{.8660254f,-.5f},{1,0}};
static const float lm_circle8[9][2]={{1,0},{.7071068f,.7071068f},{0,1},{-.7071068f,.7071068f},{-1,0},{-.7071068f,-.7071068f},{0,-1},{.7071068f,-.7071068f},{1,0}};
static int lm_front(Vec3 a,Vec3 b,Vec3 c){Vec3 u=sub(b,a),v=sub(c,a),n={u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x};return dot(n,sub(game.pos,a))>0;}
static void lm_quad(Vec3 a,Vec3 b,Vec3 c,Vec3 d,unsigned ink){if(drawcount>1750)flush_meshes();planet_quad(a,b,c,d,ink);}
static void lm_beam(Vec3 a,Vec3 b,float r,unsigned ink){
 Vec3 axis=norm(sub(b,a)),side={axis.z,0,-axis.x};if(length(side)<.01f)side=(Vec3){1,0,0};side=mul(norm(side),r);
 Vec3 up={axis.y*side.z-axis.z*side.y,axis.z*side.x-axis.x*side.z,axis.x*side.y-axis.y*side.x};
 Vec3 aa[4]={add(add(a,side),up),add(sub(a,side),up),sub(sub(a,side),up),sub(add(a,side),up)},bb[4];
 for(int i=0;i<4;i++)bb[i]=add(aa[i],sub(b,a));
 for(int i=0;i<4;i++)if(lm_front(aa[i],aa[(i+1)%4],bb[(i+1)%4]))lm_quad(aa[i],aa[(i+1)%4],bb[(i+1)%4],bb[i],mix_rgb(ink,RGB(19,29,35),i*.07f));
 if(dot(axis,sub(game.pos,b))>0)lm_quad(bb[0],bb[1],bb[2],bb[3],ink);
 if(dot(axis,sub(game.pos,a))<0)lm_quad(aa[3],aa[2],aa[1],aa[0],ink);
}
static void lm_round(Vec3 p,float rx,float rz,float h,float taper,unsigned ink){
 int count=landmark_detail?12:8;const float (*circle)[2]=landmark_detail?lm_circle12:lm_circle8;
 for(int i=0;i<count;i++){
  float ca=circle[i][0],sa=circle[i][1],cb=circle[i+1][0],sb=circle[i+1][1];
  Vec3 A={p.x+ca*rx,p.y,p.z+sa*rz},B={p.x+cb*rx,p.y,p.z+sb*rz};
  Vec3 C={p.x+cb*rx*taper,p.y+h,p.z+sb*rz*taper},D={p.x+ca*rx*taper,p.y+h,p.z+sa*rz*taper};
  unsigned c=mix_rgb(ink,RGB(23,42,51),.15f+.14f*sa);if(lm_front(B,A,D))lm_quad(A,B,C,D,c);
  if(taper>0&&game.pos.y>p.y+h)lm_quad((Vec3){p.x,p.y+h,p.z},D,C,(Vec3){p.x,p.y+h,p.z},mix_rgb(ink,WHITE,.1f));
 }
}
static void lm_dome(Vec3 p,float rx,float rz,float h,unsigned ink,int glass){
 int rows=landmark_detail?4:3,segs=landmark_detail?12:8;
 const float (*circle)[2]=landmark_detail?lm_circle12:lm_circle8;
 static const float near_rings[5][2]={{1,0},{.9238795f,.3826834f},{.7071068f,.7071068f},{.3826834f,.9238795f},{0,1}},far_rings[4][2]={{1,0},{.8660254f,.5f},{.5f,.8660254f},{0,1}};
 const float (*rings)[2]=landmark_detail?near_rings:far_rings;
 for(int row=0;row<rows;row++)for(int i=0;i<segs;i++){
  float ca=circle[i][0],sa=circle[i][1],cb=circle[i+1][0],sb=circle[i+1][1],cu=rings[row][0],su=rings[row][1],cv=rings[row+1][0],sv=rings[row+1][1];
  Vec3 A={p.x+ca*cu*rx,p.y+su*h,p.z+sa*cu*rz},B={p.x+cb*cu*rx,p.y+su*h,p.z+sb*cu*rz};
  Vec3 C={p.x+cb*cv*rx,p.y+sv*h,p.z+sb*cv*rz},D={p.x+ca*cv*rx,p.y+sv*h,p.z+sa*cv*rz};
  unsigned c=mix_rgb(ink,RGB(33,76,98),.25f+.24f*sa);if(!glass&&i==segs/4)c=RGB(45,85,109);
  if(lm_front(B,A,D))lm_quad(A,B,C,D,c);
  if(landmark_detail&&glass&&i%2==0)lm_beam(A,D,1.2f,RGB(179,201,169));
 }
}
static void lm_roof(Vec3 p,float w,float d,float h,unsigned ink){
 Vec3 a={p.x-w,p.y,p.z-d},b={p.x+w,p.y,p.z-d},c={p.x+w,p.y,p.z+d},e={p.x-w,p.y,p.z+d},u={p.x,p.y+h,p.z-d},v={p.x,p.y+h,p.z+d};
 lm_quad(a,u,v,e,ink);lm_quad(u,b,c,v,mix_rgb(ink,WHITE,.20f));lm_quad(a,b,u,u,mix_rgb(ink,RGB(29,40,45),.3f));lm_quad(e,v,c,c,ink);
}
static void lm_console(Vec3 p,unsigned ink){
 surface_box(p,5,4,10,ink);surface_box((Vec3){p.x,p.y+6,p.z-4.2f},3,.5f,3,RGB(84,198,202));
}
static void lm_windows(Vec3 p,float w,float d,float y,float height,unsigned glass){
 if(!landmark_detail)return;
 for(int i=-2;i<=2;i++)for(int side=-1;side<=1;side+=2){
  surface_box((Vec3){p.x+i*w*.32f,p.y+y,p.z+side*(d+.25f)},w*.10f,.3f,height,glass);
 }
}
static int lm_natural_site(int kind){
 return kind==FIELD_RUINS||kind==FIELD_FOSSIL||kind==FIELD_WRECK||kind==FIELD_CRYSTAL;
}
static unsigned lm_ground_ink(int kind,unsigned seed){
 Body *body=&game.bodies[game.planet];int biome=planet_biome(body);
 unsigned earth=RGB(91,78,53);
 if(biome==BIOME_OCEAN)earth=mix_rgb(body->color,RGB(76,106,61),.62f);
 else if(biome==BIOME_DESERT)earth=mix_rgb(body->color,RGB(149,105,62),.58f);
 else if(biome==BIOME_ICE)earth=mix_rgb(body->color,RGB(151,170,174),.60f);
 else if(biome==BIOME_VOLCANIC)earth=mix_rgb(body->color,RGB(72,51,47),.62f);
 else if(biome==BIOME_FOREST)earth=mix_rgb(body->color,RGB(65,91,48),.62f);
 if(kind==FIELD_GARDEN)earth=mix_rgb(earth,RGB(69,53,34),.32f);
 if(kind==FIELD_FOSSIL)earth=mix_rgb(earth,RGB(105,73,46),.28f);
 if(kind==FIELD_CRYSTAL)earth=mix_rgb(earth,RGB(73,62,91),.18f);
 return mix_rgb(earth,RGB(31,36,34),((seed>>9)&3)*.035f);
}
/* Grade each landmark into its actual terrain. The former full-height grey box
 * made every POI look placed on the same display stand. This shallow earth
 * berm follows four real ground samples, with only a narrow inset footing for
 * engineered sites. Collision and interaction keep their existing footprint. */
static float lm_ground_foundation(Vec3 p,float w,float d,int kind,unsigned ink,unsigned seed){
 int natural=lm_natural_site(kind);float spread=natural?1.16f:1.10f,ow=w*spread,od=d*spread;
 Vec3 outer[4]={{p.x-ow,0,p.z-od},{p.x+ow,0,p.z-od},{p.x+ow,0,p.z+od},{p.x-ow,0,p.z+od}};
 float high=terrain_height(&game,p.x,p.z);
 for(int i=0;i<4;i++){outer[i].y=terrain_height(&game,outer[i].x,outer[i].z)+.15f;high=fmaxf(high,outer[i].y);}
 float grade=high+.45f;
 Vec3 inner[4]={{p.x-w,grade,p.z-d},{p.x+w,grade,p.z-d},{p.x+w,grade,p.z+d},{p.x-w,grade,p.z+d}};
 unsigned soil=lm_ground_ink(kind,seed),top=mix_rgb(soil,ink,natural?.04f:.13f);
 lm_quad(inner[0],inner[1],inner[2],inner[3],top);
 for(int side=0;side<4;side++){
  int next=(side+1)&3;unsigned bank=mix_rgb(soil,RGB(26,31,29),.06f+side*.035f);
  lm_quad(outer[side],outer[next],inner[next],inner[side],bank);
 }
 if(!natural){
  unsigned footing=mix_rgb(top,RGB(72,91,94),.23f);
  surface_box((Vec3){p.x,grade,p.z},w*.91f,d*.91f,1.2f,footing);
  return grade+1.2f;
 }
 return grade+.15f;
}
static int surface_landmark_v2(Vec3 p,FieldBuilding s,int kind,unsigned ink,unsigned seed){
 if(kind<0||kind>FIELD_WEATHER)return 0;
 Vec3 cv=camera(&game,p);landmark_detail=cv.z<650;
 float w=s.w,d=s.d,h=s.height;
 unsigned stone=mix_rgb(RGB(184,179,147),ink,.15f),steel=RGB(65,89,100),edge=RGB(109,137,138),glass=RGB(65,135,151),gold=RGB(215,161,75),moss=RGB(64,96,47);
 p.y=lm_ground_foundation(p,w,d,kind,ink,seed);
 switch(kind){
 case FIELD_CACHE:
  for(int i=0;i<6;i++){
   Vec3 q={p.x+(i%3-1)*w*.52f,p.y,p.z+(i/3?1:-1)*d*.42f};float hh=15+(seed>>(i*3)&7);
   surface_box(q,w*.18f,d*.22f,hh,gold);surface_box((Vec3){q.x,q.y+hh,q.z},w*.19f,d*.23f,2,stone);
   if(landmark_detail){surface_box((Vec3){q.x,q.y+3,q.z-d*.225f},w*.12f,.5f,hh-5,steel);surface_box((Vec3){q.x,q.y+hh*.45f,q.z-d*.24f},w*.08f,.5f,2,stone);}
  }
  for(int side=-1;side<=1;side+=2)lm_beam((Vec3){p.x+side*w*.86f,p.y,p.z},(Vec3){p.x+side*w*.86f,p.y+h*.68f,p.z},3,steel);
  lm_roof((Vec3){p.x,p.y+h*.68f,p.z},w*.96f,d*.87f,h*.15f,steel);
  lm_console((Vec3){p.x+w*.8f,p.y,p.z-d*.7f},steel);break;
 case FIELD_RUINS:
  for(int i=0;i<5;i++){
   float x=p.x+(i-2)*w*.36f,hh=h*(i==1?.91f:i==3?.47f:.72f);Vec3 q={x,p.y,p.z+d*.25f};
   lm_round(q,w*.10f,d*.14f,hh,.8f,mix_rgb(stone,moss,(seed>>(i*3)&3)*.12f));
   surface_box((Vec3){x,p.y+hh,p.z+d*.25f},w*.13f,d*.17f,5,stone);
   if(landmark_detail)for(int band=1;band<4;band++)surface_box((Vec3){x,p.y+hh*band/4,p.z+d*.25f},w*.105f,d*.145f,2,edge);
  }
  for(int i=0;i<5;i++){float a=3.1415927f-i*3.1415927f/5,b=3.1415927f-(i+1)*3.1415927f/5;
   lm_beam((Vec3){p.x+cosf(a)*w*.7f,p.y+h*.46f+sinf(a)*h*.30f,p.z-d*.45f},(Vec3){p.x+cosf(b)*w*.7f,p.y+h*.46f+sinf(b)*h*.30f,p.z-d*.45f},7,stone);}
  for(int side=-1;side<=1;side+=2)surface_box((Vec3){p.x+side*w*.70f,p.y,p.z-d*.45f},8,9,h*.47f,stone);
  if(landmark_detail)for(int i=0;i<7;i++){unsigned r=field_hash(seed+i);Vec3 q={p.x+((int)(r%160)-80)*w*.01f,p.y,p.z+((int)((r>>8)%150)-75)*d*.01f};lm_round(q,5+(r&7),8,5+(r>>8&7),.6f,i&1?moss:stone);}
  break;
 case FIELD_DISH:
  surface_box(p,w*.9f,d*.85f,32,stone);lm_windows(p,w*.9f,d*.85f,17,8,glass);
  surface_box((Vec3){p.x,p.y+31,p.z},w*.94f,d*.90f,5,edge);
  lm_dome((Vec3){p.x-w*.08f,p.y+36,p.z},w*.68f,d*.77f,fminf(w*.7f,h*.5f),RGB(220,211,173),0);
  lm_beam((Vec3){p.x+w*.85f,p.y+32,p.z},(Vec3){p.x+w*.85f,p.y+h*.96f,p.z},2,stone);
  surface_box((Vec3){p.x+w*.85f,p.y+h*.86f,p.z},5,4,3,gold);
  surface_box((Vec3){p.x,p.y,p.z-d*.86f},9,1,18,steel);break;
 case FIELD_RESCUE:
  surface_box((Vec3){p.x-w*.23f,p.y,p.z},w*.55f,d*.60f,24,RGB(176,120,72));
  lm_roof((Vec3){p.x-w*.23f,p.y+24,p.z},w*.60f,d*.65f,19,RGB(203,164,112));
  surface_box((Vec3){p.x-w*.23f,p.y+8,p.z-d*.61f},4,.6f,14,RGB(216,223,193));surface_box((Vec3){p.x-w*.23f,p.y+13,p.z-d*.62f},9,.6f,4,RGB(216,223,193));
  lm_beam((Vec3){p.x+w*.75f,p.y,p.z+d*.2f},(Vec3){p.x+w*.75f,p.y+h*.87f,p.z+d*.2f},3,steel);
  lm_round((Vec3){p.x+w*.75f,p.y+h*.85f,p.z+d*.2f},9,9,6,.7f,gold);lm_console((Vec3){p.x+w*.56f,p.y,p.z-d*.60f},steel);break;
 case FIELD_GARDEN:
  lm_round(p,w*.96f,d*.96f,12,1,steel);
  for(int i=0;i<5;i++){
   Vec3 q={p.x+(i-2)*w*.30f,p.y+12,p.z-d*.35f};surface_box(q,w*.10f,d*.22f,4,RGB(87,67,43));
   lm_round((Vec3){q.x,q.y+4,q.z},w*.08f,d*.15f,19+(seed>>i&7),.45f,i&1?RGB(127,164,70):RGB(164,127,147));
  }
  lm_dome((Vec3){p.x,p.y+35,p.z},w*.96f,d*.96f,h*.60f,RGB(123,189,153),1);
  for(int i=0;i<8;i++){float a=i*.785398f;lm_beam((Vec3){p.x+cosf(a)*w*.93f,p.y+12,p.z+sinf(a)*d*.93f},(Vec3){p.x+cosf(a)*w*.93f,p.y+35,p.z+sinf(a)*d*.93f},2,stone);}
  surface_box((Vec3){p.x,p.y+12,p.z-d*.94f},w*.17f,2,21,glass);break;
 case FIELD_FOSSIL:
  for(int tier=0;tier<3;tier++){float ww=w*(1-tier*.16f),dd=d*(1-tier*.16f);unsigned soil=mix_rgb(RGB(153,119,77),RGB(66,57,45),tier*.24f);surface_box((Vec3){p.x,p.y,p.z+dd},ww,3,7+tier*3,soil);surface_box((Vec3){p.x-ww,p.y,p.z},3,dd,7+tier*3,soil);}
  lm_beam((Vec3){p.x-w*.6f,p.y+6,p.z},(Vec3){p.x+w*.5f,p.y+6,p.z},3,stone);
  for(int i=0;i<7;i++){float x=p.x-w*.47f+i*w*.14f;for(int side=-1;side<=1;side+=2)lm_beam((Vec3){x,p.y+6,p.z},(Vec3){x-4,p.y+10,p.z+side*d*(.20f+.025f*i)},2,RGB(222,208,166));}
  lm_round((Vec3){p.x+w*.6f,p.y+4,p.z},12,8,8,.65f,stone);lm_console((Vec3){p.x+w*.75f,p.y,p.z-d*.7f},steel);break;
 case FIELD_VENT:
  for(int i=0;i<3;i++){
   float x=p.x+(i-1)*w*.52f,hh=h*(i==1?.86f:.61f);Vec3 q={x,p.y,p.z};
   lm_round(q,12,12,hh,.85f,steel);lm_round((Vec3){x,p.y+hh,p.z},16,16,5,1,gold);
   if(landmark_detail)for(int band=1;band<4;band++)lm_round((Vec3){x,p.y+hh*band/4,p.z},13,13,2,1,edge);
   lm_beam((Vec3){x,p.y+10,p.z},(Vec3){x,p.y+10,p.z+d*.7f},4,edge);
  }
  surface_box((Vec3){p.x,p.y,p.z+d*.7f},w*.65f,8,16,steel);lm_console((Vec3){p.x+w*.8f,p.y,p.z-d*.7f},steel);break;
 case FIELD_WRECK:
  lm_round((Vec3){p.x-w*.1f,p.y+4,p.z},w*.45f,d*.24f,h*.45f,.55f,edge);
  for(int side=-1;side<=1;side+=2){Vec3 a={p.x-w*.35f,p.y+8,p.z},b={p.x+w*.38f,p.y+10,p.z+side*d*.85f},c={p.x-w*.67f,p.y+3,p.z+side*d*.72f};lm_quad(a,b,c,c,steel);lm_beam(a,b,2,gold);}
  surface_box((Vec3){p.x,p.y+h*.32f,p.z-d*.22f},12,2,9,glass);
  lm_round((Vec3){p.x+w*.64f,p.y+2,p.z-d*.54f},10,13,12,.65f,RGB(54,57,61));lm_console((Vec3){p.x-w*.75f,p.y,p.z-d*.70f},steel);break;
 case FIELD_MIGRATION:
  for(int side=-1;side<=1;side+=2){
   for(int z=-1;z<=1;z+=2)lm_beam((Vec3){p.x+side*w*.7f,p.y,p.z+z*d*.65f},(Vec3){p.x+side*w*.48f,p.y+h*.62f,p.z+z*d*.48f},3,steel);
   if(landmark_detail)lm_beam((Vec3){p.x+side*w*.65f,p.y+10,p.z-d*.60f},(Vec3){p.x+side*w*.50f,p.y+h*.55f,p.z+d*.49f},2,edge);
  }
  lm_round((Vec3){p.x,p.y+h*.59f,p.z},w*.78f,d*.78f,6,1,stone);
  lm_round((Vec3){p.x,p.y+h*.63f,p.z},w*.50f,d*.50f,h*.19f,1,glass);
  lm_round((Vec3){p.x,p.y+h*.82f,p.z},w*.60f,d*.60f,h*.12f,.45f,steel);
  lm_beam((Vec3){p.x,p.y+h*.90f,p.z},(Vec3){p.x,p.y+h,p.z},2,gold);break;
 case FIELD_CRYSTAL:
  for(int i=0;i<7;i++){
   unsigned r=field_hash(seed+i*313);float a=i*.8975979f,rad=i?.53f:0,hh=h*(.35f+(r&31)*.015f);Vec3 q={p.x+cosf(a)*w*rad,p.y,p.z+sinf(a)*d*rad};
   unsigned crystal=mix_rgb(RGB(108,123,189),RGB(146,91,171),(r>>8&7)*.09f);
   lm_round(q,8+(r&7),8+(r&7),hh*.7f,.95f,crystal);q.y+=hh*.7f;lm_round(q,8+(r&7),8+(r&7),hh*.3f,0,RGB(185,180,223));
  }break;
 case FIELD_ARCHIVE:
  lm_round(p,w*.96f,d*.96f,h*.52f,1,stone);lm_round((Vec3){p.x,p.y+h*.52f,p.z},w*.98f,d*.98f,h*.23f,.60f,steel);
  surface_box((Vec3){p.x,p.y,p.z-d*.93f},w*.28f,2,h*.44f,steel);
  for(int i=-1;i<=1;i+=2)surface_box((Vec3){p.x+i*w*.22f,p.y+5,p.z-d*.96f},2,1,h*.31f,gold);
  surface_box((Vec3){p.x,p.y+h*.37f,p.z-d*.96f},w*.22f,1,3,gold);
  lm_console((Vec3){p.x+w*.52f,p.y,p.z-d*.75f},steel);
  lm_beam((Vec3){p.x+w*.55f,p.y+h*.5f,p.z},(Vec3){p.x+w*.55f,p.y+h*.95f,p.z},2,edge);break;
 case FIELD_WEATHER:
  for(int t=-1;t<=1;t++){
   float x=p.x+t*w*.59f,hh=h*(t?.65f:.94f);
   for(int side=-1;side<=1;side+=2)lm_beam((Vec3){x+side*11,p.y,p.z},(Vec3){x+side*5,p.y+hh,p.z},2,steel);
   for(int level=0;level<4;level++){float yy=p.y+hh*level/4;lm_beam((Vec3){x-10,yy,p.z},(Vec3){x+10,yy+hh/4,p.z},1.5f,edge);}
   lm_beam((Vec3){x-19,p.y+hh,p.z},(Vec3){x+19,p.y+hh,p.z},2,stone);
   lm_round((Vec3){x,p.y+hh,p.z},6,6,5,.6f,gold);
  }
  for(int side=-1;side<=1;side+=2){Vec3 q={p.x+side*w*.43f,p.y+7,p.z-d*.66f};surface_box(q,w*.24f,d*.18f,2,glass);if(landmark_detail)for(int i=-1;i<=1;i++)surface_box((Vec3){q.x+i*w*.14f,q.y+2,q.z},.5f,d*.18f,.4f,stone);}
  lm_console((Vec3){p.x,p.y,p.z-d*.68f},steel);break;
 }
 /* A real mounted sign, independent of the landmark's irregular silhouette. */
 if(landmark_detail){
  for(int side=-1;side<=1;side+=2)surface_box((Vec3){p.x+side*w*.30f,p.y,p.z-d*.95f},1,1,18,steel);
  surface_box((Vec3){p.x,p.y+10,p.z-d*.95f},w*.44f,.5f,10,steel);
 }
 return 1;
}
static void surface_port_detail(Vec3 p,FieldBuilding s){
 Vec3 v=camera(&game,p);if(v.z+s.w<0||v.z>950||fabsf(v.x)>v.z+s.w*2)return;landmark_detail=v.z<650;
 lm_windows(p,s.w,s.d,s.height*.53f,9,RGB(66,138,159));
 lm_roof((Vec3){p.x,p.y+s.height+4,p.z},s.w,s.d,9,RGB(94,121,135));
 surface_box((Vec3){p.x,p.y,p.z-s.d-.25f},s.w*.27f,.5f,s.height*.38f,RGB(34,56,66));
 if(landmark_detail)for(int side=-1;side<=1;side+=2)surface_box((Vec3){p.x+side*s.w*.31f,p.y+4,p.z-s.d-.8f},2,.5f,s.height*.32f,RGB(216,167,85));
 if(s.height>140){
  surface_box((Vec3){p.x,p.y+s.height-27,p.z},s.w+12,s.d+9,20,RGB(45,102,124));
  lm_windows((Vec3){p.x,p.y+s.height-27,p.z},s.w+12,s.d+9,5,9,RGB(137,220,219));
  surface_box((Vec3){p.x,p.y+s.height+12,p.z},2,2,38,RGB(189,170,128));
 }else if(landmark_detail){
  for(int i=-1;i<=1;i++){float x=p.x+i*s.w*.53f;surface_box((Vec3){x,p.y+3,p.z-s.d-.6f},s.w*.20f,.4f,s.height*.43f,RGB(24,42,54));
   for(int stripe=1;stripe<4;stripe++)surface_box((Vec3){x,p.y+stripe*s.height*.1f,p.z-s.d-1},s.w*.19f,.3f,.45f,RGB(83,107,119));}
 }
}
