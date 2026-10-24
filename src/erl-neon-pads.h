/* One continuous 230-design-unit side-lane bed, eight 20/28 chevrons and the
 * same lane, compact swept contact stations and depletion state as gameplay. */
static void __attribute__((noinline,optimize("O2"))) race_neon_pads(void){
 SpaceRace *r=&game.race;if(r->layout!=5)return;race_floor_prepare(r);
 float scale=race_neon_scale(r),unit=2.5f*scale,section_span=r->top_speed*46.f/(RACE_GATES/r->laps);
 int first=r->gate>0?r->gate-1:0,last=r->gate+3,arrows=0;if(last>=RACE_GATES)last=RACE_GATES-1;
 for(int section=first;section<=last&&arrows<20;section++){
  float pad_t,lane;if(!race_neon_pad(r,section,&pad_t,&lane))continue;int live=1; /* Shared course opportunity; per-racer anti-farming is gameplay-only. */
  unsigned cyan=live?RGB(91,247,240):RGB(30,72,79),edge=live?RGB(153,255,243):RGB(38,84,88),bed=live?RGB(58,41,53):RGB(31,29,39);
  if(race_track_surface(r,section,pad_t)==2)continue;
  NeonRoadRow middle,rows[9];if(!neon_road_row(section+pad_t,&middle))continue;
  Vec3 centre=neon_road_point(&middle,lane,0),delta=sub(centre,neon_eye);
  if(dot(delta,delta)>14000.f*14000.f||!neon_visible(centre,130*scale))continue;
  if(arrows+8>20)break;int valid=1;
  for(int row=0;row<=8;row++)if(!neon_road_row(section+pad_t+(-115+row*28.75f)*scale/section_span,&rows[row])){valid=0;break;}
  if(!valid)continue;
  /* Resolve the bed before its coplanar arrows; the opaque queue reverses
   * submission order. Shared rows retain the actual road's local curve. */
  for(int row=0;row<8;row++)neon_quad(neon_road_point(&rows[row],lane-ERL_PAD_HALF_WIDTH*unit,scale),neon_road_point(&rows[row],lane+ERL_PAD_HALF_WIDTH*unit,scale),
   neon_road_point(&rows[row+1],lane+ERL_PAD_HALF_WIDTH*unit,scale),neon_road_point(&rows[row+1],lane-ERL_PAD_HALF_WIDTH*unit,scale),bed);
  neon_flush();float lift=2*scale;
  for(int arrow=0;arrow<8;arrow++){
   float centre_offset=-98+arrow*28,q=section+pad_t+(centre_offset+10)*scale/section_span;
   NeonRoadRow head_row,tail_row,notch_row;
   if(!neon_road_row(q,&head_row)||!neon_road_row(q-20*scale/section_span,&tail_row)||!neon_road_row(q-6*scale/section_span,&notch_row))continue;
   Vec3 tip=neon_road_point(&head_row,lane,lift),inside=neon_road_point(&notch_row,lane,lift);
   neon_quad(neon_road_point(&tail_row,lane-22*unit,lift),tip,inside,neon_road_point(&tail_row,lane-12*unit,lift),cyan);
   neon_quad(tip,neon_road_point(&tail_row,lane+22*unit,lift),neon_road_point(&tail_row,lane+12*unit,lift),inside,cyan);arrows++;
  }
  neon_flush();
  for(int row=0;row<8;row++)for(int side=-1;side<=1;side+=2)
   neon_line(neon_road_point(&rows[row],lane+side*(ERL_PAD_HALF_WIDTH-3)*unit,lift),neon_road_point(&rows[row+1],lane+side*(ERL_PAD_HALF_WIDTH-3)*unit,lift),edge);
 }
}
