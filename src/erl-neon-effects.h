/* Layout 5 wake: live engine roots/history, layered native strokes, a soft
 * warm glow and depth-tested hot cores. Fixed work, no triangle bounding-box
 * blending, extra persistent buffers or simulation RNG. */
static void __attribute__((noinline,noclone,optimize("O3","fast-math"))) race_neon_flare_line(float x,float y,float dx,float dy,float inv,int width,unsigned ink,int alpha){
 NeonStrokeSegment ray={{x-dx,y-dy,inv},{x+dx,y+dy,inv}};neon_stroke_draw(&ray,width,ink,alpha);
}
static void __attribute__((noinline,noclone,optimize("O3","fast-math"))) race_neon_nozzle(Vec3 world,int boosting,int engine){
 Vec3 v=neon_camera(world);if(v.z<64)return;float inv=1/v.z;
 float x=proj_ox+v.x*proj_focal*inv,y=proj_oy-v.y*proj_focal*inv;
 float pulse=.82f+.18f*sinf(game.race.elapsed*31+engine*2.3f);
 int radius=boosting?7:5;
 /* Build the bloom from tapered circular chords. The former single thick
  * stroke had a rectangular footprint at native resolution. These few soft
  * rows make a round falloff while retaining the depth test against the hull. */
 for(int oy=-radius;oy<=radius;oy+=2){
  float chord=sqrtf((float)(radius*radius-oy*oy));
  int alpha=(int)((boosting?72:48)*pulse*(1.f-fabsf((float)oy)/(radius+2)));
  race_neon_flare_line(x,y+oy,chord,0,inv,2,boosting?RGB(255,82,25):RGB(232,72,35),alpha);
 }
 /* A boosted outlet gets one restrained, slowly breathing anamorphic ray;
  * the round falloff keeps it organic instead of reading as a UI cross. */
 if(boosting)race_neon_flare_line(x,y,12,0,inv,1,RGB(255,126,47),(int)(84*pulse));
 /* Compact peach and white cores preserve each real engine outlet without
  * hiding the surrounding ship material. */
 race_neon_flare_line(x,y,3,0,inv,3,RGB(255,151,70),210);
 race_neon_flare_line(x,y,1,0,inv,2,RGB(255,247,218),255);
 race_neon_flare_line(x,y,0,1,inv,1,WHITE,255);
}
static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_shadow(int id,ApexPose pose,int mesh){
 SpaceRace *r=&game.race;int gate=id?r->pilot[id-1].gate:r->gate;
 if(gate>=RACE_GATES)return;
 RaceTrackFrame f=race_track_nearest(r,gate,pose.pos);
 if(race_track_surface(r,f.section,f.t)==2)return;
 float height=dot(sub(pose.pos,f.centre),f.up)-RACE_TRACK_FLOOR;
 if(height<0||height>650||fabsf(dot(sub(pose.pos,f.centre),f.right))>175*2.5f*race_neon_scale(r))return;
 float size=190,wide=size*.65f,longitudinal=size*.8f;
 Vec3 centre=sub(pose.pos,mul(f.up,height-12));
 Vec3 nose=add(centre,mul(f.ahead,longitudinal)),right=add(centre,mul(f.right,wide));
 Vec3 tail=sub(centre,mul(f.ahead,longitudinal*.65f)),left=sub(centre,mul(f.right,wide));
 unsigned ink=race_glass_ink;int samples=race_wall_samples;
 race_glass_ink=RGB(8,5,20);race_wall_samples=0;
 /* A small four-corner footprint remains attached to the actual magnetic
  * floor. It avoids projecting every overlapping hull face onto one plane. */
 int alpha=(int)(110*(1-height/650));
 race_wall_triangle(nose,right,tail,alpha);race_wall_triangle(nose,tail,left,alpha);
 race_glass_ink=ink;race_wall_samples=samples;
}
static void __attribute__((noinline,optimize("O3","fast-math"))) race_neon_effects(void){
 SpaceRace *r=&game.race;race_apex_tick();if(r->phase!=RACE_RUNNING)return;
 uint64_t begin=sceKernelGetSystemTimeWide();
 int old_encoding=mega_depth_encoding,old_depth=surface_depth_on,old_material=surface_material;
 mega_depth_encoding=surface_depth_on=1;surface_material=0;
 for(int id=0;id<=r->rivals;id++){
  if((!id&&r->arcade.actor[0].health<=0)||r->arcade.actor[id].eliminated||(id&&r->pilot[id-1].disabled)||(!id&&!third_person))continue;
  ApexPose pose=race_apex_pose(id);float distance=length(sub(pose.pos,camera_origin(&game)));
  if(distance>15000||(id&&!neon_visible(pose.pos,900)))continue;
  int mesh=id?r->pilot[id-1].mesh:mesh_id(player_ships[game.ship].name);
  race_neon_shadow(id,pose,mesh);ApexEngines engines=race_apex_engines(id);
  float scale=id?RACE_SHIP_SCALE:1.2f;
  int boosting=id?r->pilot[id-1].boosting||r->arcade.actor[id].rocket>0:
   (game.boost&&r->reserve>0)||r->boost_time>0||r->arcade.actor[0].rocket>0;
  int samples=id?(boosting?8:4):(boosting?24:11);
  if(samples>=apex_fx.count)samples=apex_fx.count-1;
  if(samples<1)samples=1;
  int step=distance>6000?3:2;
  for(int engine=0;engine<engines.count;engine++){
   Vec3 root=race_apex_local(pose,engines.root[engine],scale),previous=root;
   NeonStrokeSegment strokes[9];unsigned warm_ink[9],core_ink[9];unsigned char widths[9],glows[9],opacity[9];int count=0;
   for(int j=1;j<=samples;j+=step){
    int at=(apex_fx.head-j+APEX_HISTORY)%APEX_HISTORY;
    Vec3 tail=race_apex_local(apex_fx.history[id][at],engines.root[engine],scale);
    if(apex_fx.count<=1)tail=add(root,mul(pose.ahead,-40));
    Vec3 rawtail=tail,ca=neon_camera(previous),cb=neon_camera(tail);
    if(length(sub(previous,tail))>2400||(ca.z<18&&cb.z<18)){previous=rawtail;continue;}
    if(ca.z<18){float t=(18-ca.z)/(cb.z-ca.z);previous=add(previous,mul(sub(tail,previous),t));ca=neon_camera(previous);}
    if(cb.z<18){float t=(ca.z-18)/(ca.z-cb.z);tail=add(previous,mul(sub(tail,previous),t));cb=neon_camera(tail);}
    float fade=1-(float)j/(samples+2),pulse=.82f+.18f*sinf(r->elapsed*29+engine*2.3f+j*.7f);
    /* Resolve the approved orange/pale wake at native pixel widths. Distant
     * rivals stay thin. All layers reuse the live, curved engine history. */
    if(count<9&&neon_stroke_camera(ca,cb,&strokes[count])){
     int width=id?(distance>6000?1:2):(boosting?6:5);
     widths[count]=width;glows[count]=id?(distance>6000?4:5):(boosting?13:9);
     opacity[count]=(int)((boosting?82:57)*fade*pulse);
     warm_ink[count]=RGB((int)(255*fade),(int)((boosting?151:122)*fade*pulse),(int)(72*fade));
     core_ink[count]=RGB((int)(255*fade),(int)(229*fade),(int)(174*fade));count++;
    }
    previous=rawtail;
   }
   /* Draw complete layers so the next segment's glow cannot erase a hot
    * join. Soft light blends only where the shared scene depth permits it. */
   for(int line=0;line<count;line++)neon_stroke_draw(&strokes[line],glows[line],RGB(255,88,32),opacity[line]);
   for(int line=0;line<count;line++)neon_stroke_draw(&strokes[line],widths[line],warm_ink[line],255);
   for(int line=0;line<count;line++)neon_stroke_draw(&strokes[line],id?1:2,core_ink[line],255);
   if(!id)race_neon_nozzle(root,boosting,engine);else race_apex_glint(root,boosting?1:.25f);
   if(apex_fx.count>5){int at=(apex_fx.head-samples+APEX_HISTORY)%APEX_HISTORY;
    Vec3 tail=race_apex_local(apex_fx.history[id][at],engines.root[engine],scale);
    tail=add(tail,mul(pose.up,sinf(r->elapsed*19+engine+id)*8));race_apex_glint(tail,.1f);}
  }
 }
 for(int i=0;i<APEX_SPARKS;i++){ApexSpark *p=&apex_fx.spark[i];if(p->live)
  neon_line(p->pos,add(p->pos,mul(p->vel,-.008f)),p->life>.25f?RGB(255,229,225):RGB(211,81,146));}
 mega_depth_encoding=old_encoding;surface_depth_on=old_depth;surface_material=old_material;
 race_fx_us+=sceKernelGetSystemTimeWide()-begin;race_fx_frames++;
}

/* Deterministic hot shards: same current hull breakup, no extra particle pool.
 * Slow motion belongs to the displayed event age, never the live race clock. */
static void race_neon_crash_effects(void){
 if(!erl_crash_visual_active||erl_crash_visual_age<.8f)return;
 SpaceRace *r=&game.race;float age=(erl_crash_visual_age-.8f)*.65f;
 RaceTrackFrame f=race_track_nearest(r,r->gate,r->crash_pos);
 for(int n=0;n<24;n++){
  float angle=n*2.399963f,life=1.25f+(n%5)*.13f;if(age>life)continue;
  Vec3 velocity=add(mul(f.right,cosf(angle)*(190+n%4*70)),add(mul(f.up,120+sinf(angle)*180),mul(r->crash_dir,(n%7-3)*95.f)));
  Vec3 p=add(r->crash_pos,add(mul(velocity,age),mul(f.up,-230*age*age)));
  unsigned ink=age<.20f?RGB(255,245,209):age<.65f?RGB(255,162,54):RGB(195,71,52);
  neon_glow_line(p,sub(p,mul(velocity,.035f)),age<.25f?5:3,RGB(255,83,24),ink,(int)(110*(1-age/life)));
 }
 if(age<.65f){
  float radius=90+age*560,alpha=145*(1-age/.65f);Vec3 centre=add(r->crash_pos,mul(f.up,65));
  for(int n=0;n<16;n++){float a=n*.392699f,b=(n+1)*.392699f;
   Vec3 p=add(centre,add(mul(f.right,cosf(a)*radius),mul(f.up,sinf(a)*radius))),q=add(centre,add(mul(f.right,cosf(b)*radius),mul(f.up,sinf(b)*radius)));
   neon_glow_line(p,q,5,RGB(255,86,22),RGB(255,211,116),(int)alpha);
  }
 }
 if(age<.30f){Vec3 centre=add(r->crash_pos,mul(f.up,65)),toward=norm(sub(neon_eye,centre));Vec3 v=neon_camera(add(centre,mul(toward,140)));if(v.z>=64){
  float inv=1/v.z,x=proj_ox+v.x*proj_focal*inv,y=proj_oy-v.y*proj_focal*inv,power=1-age/.30f;int radius=12+(int)(age*35);
  for(int dy=-radius;dy<=radius;dy+=2){float chord=sqrtf(radius*radius-dy*dy);race_neon_flare_line(x,y+dy,chord,0,inv,2,RGB(255,120,36),(int)(110*power*(1-fabsf((float)dy)/(radius+1))));}
  race_neon_flare_line(x,y,4,0,inv,5,RGB(255,248,205),(int)(230*power));
 }}
 if(age<.25f){Vec3 v=neon_camera(r->crash_pos);if(v.z>=64){float inv=1/v.z,x=proj_ox+v.x*proj_focal*inv,y=proj_oy-v.y*proj_focal*inv;race_neon_flare_line(x,y,28+age*120,0,inv,3,RGB(255,199,98),(int)(180*(1-age/.25f)));}}
}

/* Local expanding cyan shock ring and radial streaks, only on an actual
 * player pad/ring contact. No screen wash, blur or new particle pool. */
static void __attribute__((noinline,optimize("Os"))) race_neon_boost_flash(void){
 const RaceBoostVisual *v=race_boost_visual(&game);if(!v||game.race.phase!=RACE_RUNNING)return;
 float life=(v->until-game.race.elapsed)/.30f,scale=race_neon_scale(&game.race),radius=(v->ring?32:75)*scale*(1+(1-life)*.65f);
 Vec3 right=v->right,up=v->ring?v->up:norm(race_ui_cross(right,v->up));
 for(int i=0;i<16;i++){float a=i*.392699082f,b=(i+1)*.392699082f;
  Vec3 radial=add(mul(right,cosf(a)),mul(up,sinf(a))),next=add(mul(right,cosf(b)),mul(up,sinf(b)));
  Vec3 p=add(v->pos,mul(radial,radius)),q=add(v->pos,mul(next,radius));NeonStrokeSegment stroke;
  if(neon_stroke_camera(neon_camera(p),neon_camera(q),&stroke)){neon_stroke_draw(&stroke,8,RGB(26,191,255),(int)(110*life));neon_stroke_draw(&stroke,2,RGB(183,255,255),(int)(255*life));}
  if(!(i&1)&&neon_stroke_camera(neon_camera(p),neon_camera(add(p,mul(radial,45*scale*life))),&stroke))neon_stroke_draw(&stroke,2,RGB(126,249,255),(int)(190*life));
 }
}
