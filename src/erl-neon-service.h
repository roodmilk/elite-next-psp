/* Mint repair bed follows the same opaque road rows and finish plane.
 * No additional render target, texture, particle pool or permanent overlay. */
static void race_neon_service_draw(void){
 SpaceRace *r=&game.race;int per=RACE_GATES/r->laps;
 float scale=race_neon_scale(r),edge=190*2.5f*scale,step=.025f;
 int first=r->gate>0?r->gate-1:0,last=r->gate+1;if(last>=RACE_GATES)last=RACE_GATES-1;
 for(int k=first;k<=last;k++){if(k%per!=0&&k%per!=per-1)continue;for(int n=0;n<40;n++){
  float q=k+n*step,clock=race_neon_clock(((k%per)+(n+.5f)*step)/per);if(!erl_service_clock(clock))continue;
  NeonRoadRow a,b;if(!neon_road_row(q,&a)||!neon_road_row(q+step,&b))continue;
  Vec3 mid=neon_road_point(&a,edge*ERL_SERVICE_LANE,0);if(!neon_visible(mid,edge*.15f)||dot(sub(mid,neon_eye),sub(mid,neon_eye))>10000.f*10000.f)continue;
  float lo=edge*(ERL_SERVICE_LANE-.12f),hi=edge*(ERL_SERVICE_LANE+.12f);
  neon_road_strip(&a,&b,lo,hi,scale,RGB(38,68,64));
  neon_road_strip(&a,&b,lo,lo+7*scale,2*scale,RGB(148,255,198));
  neon_road_strip(&a,&b,hi-7*scale,hi,2*scale,RGB(148,255,198));
  if(n%4==0)neon_road_strip(&a,&b,lo+18*scale,hi-18*scale,2*scale,RGB(84,153,123));
 }}
 neon_flush();
}
