/* Bounded architectural kit: every silhouette stays inside its shared solid
 * footprint; no scenery-only pillars are placed in the driving lanes. */
static void surface_starport_building(int id,Vec3 p,FieldBuilding s,unsigned tint){
 Vec3 v=camera(&game,add(p,(Vec3){0,s.height*.5f,0}));
 float radius=sqrtf(s.w*s.w+s.d*s.d+s.height*s.height*.25f);
 if(v.z+radius<4||v.z-radius>2200||fabsf(v.x)>v.z+radius*1.6f)return;
 landmark_detail=v.z<650;
 unsigned hull=mix_rgb(RGB(116,144,159),tint,.13f),dark=RGB(32,49,65),glass=RGB(37,112,139),glow=RGB(90,217,230),gold=RGB(221,169,75);
 surface_box(p,s.w,s.d,id>=2&&id<=4?2.f:8.f,RGB(63,80,94));
 if(id==1){
  /* Faceted control spire, broad glazed observation crown, antenna fin. */
  lm_round((Vec3){p.x,p.y+8,p.z},s.w*.72f,s.d*.72f,s.height*.65f,.55f,hull);
  lm_round((Vec3){p.x,p.y+s.height*.60f,p.z},s.w*.45f,s.d*.45f,28,2.1f,dark);
  lm_round((Vec3){p.x,p.y+s.height*.69f,p.z},s.w*.96f,s.d*.96f,32,.92f,glass);
  lm_round((Vec3){p.x,p.y+s.height*.69f+32,p.z},s.w*.90f,s.d*.90f,5,1.04f,glow);
  lm_round((Vec3){p.x,p.y+s.height*.83f,p.z},s.w*.70f,s.d*.70f,12,.35f,hull);
  surface_box((Vec3){p.x,p.y+s.height*.85f,p.z},3,3,s.height*.15f,gold);
  if(landmark_detail)for(int side=-1;side<=1;side+=2)lm_beam((Vec3){p.x+side*s.w*.55f,p.y+12,p.z},(Vec3){p.x+side*s.w*.26f,p.y+s.height*.57f,p.z},3,glow);
 }else if(id==0){
  /* Terminal: low stepped concourse, broad glass ribbons and cantilever roof. */
  surface_box((Vec3){p.x,p.y+8,p.z},s.w*.96f,s.d*.96f,64,hull);
  for(int side=-1;side<=1;side+=2){
   if(side*(game.pos.z-p.z)<0)continue;
   surface_box((Vec3){p.x,p.y+28,p.z+side*s.d*.965f},s.w*.88f,.8f,22,glass);
   surface_box((Vec3){p.x,p.y+51,p.z+side*s.d*.97f},s.w*.9f,1,2,glow);
   if(landmark_detail)for(int i=-4;i<=4;i++)surface_box((Vec3){p.x+i*s.w*.18f,p.y+27,p.z+side*s.d*.98f},2,1,26,dark);
  }
  surface_box((Vec3){p.x,p.y+72,p.z},s.w,s.d,6,dark);
  surface_box((Vec3){p.x,p.y+79,p.z},s.w*.7f,s.d*.72f,9,hull);
  lm_dome((Vec3){p.x,p.y+88,p.z},s.w*.32f,s.d*.5f,12,glass,0);
  surface_box((Vec3){p.x,p.y+8,p.z+s.d*.97f},22,1,25,dark);
 }else if(id>=2&&id<=4){
  /* Low open docking terraces, no vaults or walls across the bay. */
  lm_round(p,s.w,s.d,2.5f,.98f,dark);
  lm_round((Vec3){p.x,p.y+2.6f,p.z},s.w*.86f,s.d*.86f,.3f,1,hull);
  for(int side=-1;side<=1;side+=2){
   surface_box((Vec3){p.x+side*s.w*.9f,p.y+3,p.z},1.3f,s.d*.72f,.5f,glow);
   surface_box((Vec3){p.x,p.y+3,p.z+side*s.d*.9f},s.w*.72f,1.3f,.5f,id==2?gold:glow);
  }
  if(landmark_detail)for(int i=-2;i<=2;i++){
   surface_box((Vec3){p.x+i*s.w*.28f,p.y+3,p.z-s.d*.65f},4,10,.4f,WHITE);
   surface_box((Vec3){p.x+i*s.w*.28f,p.y+3,p.z+s.d*.65f},4,10,.4f,WHITE);
  }
 }else{
  surface_box((Vec3){p.x,p.y+8,p.z},s.w,s.d,s.height*.58f,dark);
  for(int i=-1;i<=1;i++)lm_round((Vec3){p.x+i*s.w*.58f,p.y+s.height*.55f,p.z},s.w*.24f,s.d*.78f,s.height*.35f,.85f,hull);
  surface_box((Vec3){p.x,p.y+s.height*.55f,p.z},s.w,s.d,3,gold);
 }
 flush_meshes();
 if(v.z<600){
  static const char *names[]={"STARPORT TERMINAL","FLIGHT CONTROL","LANDING PLATFORM 02","FREIGHT PLATFORM","LANDING PLATFORM 03","FUEL SERVICES","CARGO HANDLING"};
  int side=game.pos.z>p.z?1:-1;
  surface_building_sign((Vec3){p.x,p.y+(id==1?s.height*.74f:id==0?61:id>=2&&id<=4?4:26),p.z+side*(s.d+2)},(Vec3){side>0?-1:1,0,0},s.w*1.35f,names[id]);
 }
}
static void surface_heavy_berth(Vec3 p,int lane){
 float r=FIELD_TRAFFIC_PAD;
 surface_box(p,r,r,.5f,RGB(50,67,81));
 for(int side=-1;side<=1;side+=2){
  surface_box((Vec3){p.x+side*(r-3),p.y+.6f,p.z},2,r-4,.2f,RGB(165,190,193));
  surface_box((Vec3){p.x,p.y+.6f,p.z+side*(r-3)},r-4,2,.2f,RGB(165,190,193));
 }
 for(int i=0;i<4;i++)surface_box((Vec3){p.x+(i&1?r:-r),p.y,p.z+(i&2?r:-r)},3,3,3,lane?RGB(234,178,83):RGB(80,212,221));
}
