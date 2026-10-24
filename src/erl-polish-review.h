/* Included in native review only. Real inputs, contacts, saves and rendering. */
{
 Game *baseline=malloc(sizeof(Game)),*copy=malloc(sizeof(Game));NCHK(baseline&&copy,"polish review obtains isolated commander fixtures");
 if(baseline&&copy){
  *baseline=game;for(int i=0;i<RA_PICKUPS;i++)r->arcade.pickup[i].cooldown=fminf(5,r->arcade.pickup[i].cooldown);unsigned traffic=race_polish_traffic_review(&game);
  NCHK(traffic&1,"closing rival prepares a clear passing lane");
  NCHK(traffic&2,"blocked rival aborts the pass and follows within the leader's speed");
  NCHK(traffic&4,"alongside rival holds its line and leaves room");
  NCHK(traffic&8,"predicted merge rejects an occupied corridor");
  NCHK(traffic&16,"vertical clearance avoids false occupancy from a different helix level");
  NCHK(traffic&32,"contact cooldown keeps physical hull separation without repeated speed penalties");
  NCHK(erl_trailing_gain(-2)==0&&erl_trailing_gain(.30f)==0&&erl_trailing_gain(2)>.049f&&erl_trailing_gain(.7f)<.015f,"trailing-only catch-up fades before overtaking range");
  NCHK(erl_drive_target(8000,.7f,1,1,0)==11840&&erl_drive_target(8000,.7f,0,1,0)==10640&&erl_drive_target(8000,.7f,1,1,1)==0,"player and rivals share boost ceilings and brake priority");
  neon_review_pose(ordinary,0,100*scale);r->reserve=40;r->boost_time=.65f;r->manual_input=0;game.boost=0;
  erl_input(0,PSP_CTRL_CROSS,.1f,0,0);
  NCHK(fabsf(r->reserve-40.8f)<.01f&&r->boost_time>.54f,"released R recharges reserve during automatic boost");
  game.speed=r->top_speed*ERL_AUTO_SURGE;r->reserve=40;r->boost_lock=0;erl_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.1f,0,0);
  NCHK(game.speed>r->top_speed*ERL_AUTO_SURGE&&game.speed<=r->top_speed*ERL_MANUAL_SURGE&&fabsf(r->reserve-37.5f)<.01f,"manual boost adds a capped surge while spending the normal reserve");
  r->boost_time=.65f;r->arcade.actor[0].rocket=.65f;float fuel=r->reserve;
  erl_input(0,PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER,.1f,0,0);
  NCHK(r->boost_time>.54f&&r->arcade.actor[0].rocket>.54f&&r->reserve==fuel,"braking preserves boost timers and held R cannot recharge");
  float slow=game.speed;erl_input(0,PSP_CTRL_CROSS,.1f,0,0);NCHK(game.speed>slow&&r->reserve>fuel,"brake release restores the remaining surge smoothly");
  neon_review_pose(ordinary,0,100*scale);r->boost_time=0;r->reserve=40;RaceItemActor *a=&r->arcade.actor[0];a->item=RA_MISSILE;a->ammo=0;a->shuffle=a->protection=a->spin=0;a->previous_x=a->require_release=0;erl_vertical_speed=0;
  RaceTrackFrame f=race_track_nearest(r,r->gate,game.pos);float height=dot(sub(game.pos,f.centre),f.up);
  for(int i=0;i<8;i++)erl_input(0,PSP_CTRL_DOWN,1.f/60,0,0);f=race_track_nearest(r,r->gate,game.pos);
  NCHK(r->rear_input&&fabsf(dot(sub(game.pos,f.centre),f.up)-height)<1,"holding Down before a rear rocket does not lower the craft");
  memset(r->arcade.shot,0,sizeof(r->arcade.shot));Vec3 heading=forward(&game);erl_input(PSP_CTRL_CIRCLE,PSP_CTRL_DOWN|PSP_CTRL_CIRCLE,1.f/60,0,0);
  RaceProjectile *shot=0;for(int i=0;i<RA_PROJECTILES;i++)if(r->arcade.shot[i].live){shot=&r->arcade.shot[i];break;}
  NCHK(shot&&shot->kind==RA_MISSILE&&shot->target==-1&&shot->generation==UINT32_MAX&&dot(shot->dir,heading)<-.999f&&shot->life>1.70f&&shot->life<1.75f,"Down+Circle launches a straight short rear rocket without a lock");
  NCHK(save_game(&game,"polish-rear.sav")&&load_game(copy,"polish-rear.sav")&&copy->race.arcade.shot[0].generation==UINT32_MAX,"rear projectile direction survives save and reload");
  erl_input(0,0,1.f/60,0,0);erl_vertical_speed=0;height=dot(sub(game.pos,f.centre),f.up);
  for(int i=0;i<8;i++)erl_input(0,PSP_CTRL_DOWN,1.f/60,0,0);f=race_track_nearest(r,r->gate,game.pos);
  NCHK(!r->rear_input&&dot(sub(game.pos,f.centre),f.up)<height-4,"ordinary descent remains available after releasing rear-fire Down");
  neon_review_pose(0,0,0);f=race_track_frame(r,0,.08f);float edge=190*2.5f*scale;
  game.yaw=atan2f(f.ahead.x,f.ahead.z);game.pitch=asinf(f.ahead.y);game.roll=0;game.pos=add(f.centre,add(mul(f.right,edge*ERL_SERVICE_LANE),mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35)));
  a=&r->arcade.actor[0];a->health=1200;r->health=50;r->reserve=40;r->boost_time=0;game.speed=r->top_speed*.7f;
  race_tick(&game,sub(game.pos,mul(f.ahead,200)),.1f);
  NCHK(a->health==1236&&r->reserve==40&&r->gate==0,"moving repair restores15 percent HULL per second without reserve/checkpoint reward");
  int hull=a->health;race_tick(&game,game.pos,.1f);race_tick(&game,add(game.pos,mul(f.ahead,200)),.1f);
  NCHK(a->health==hull,"stationary and reverse repair cannot farm HULL");neon_review_compact_capture("neon59-repair.bmp");
  game.pos=add(game.pos,mul(f.right,edge*.4f));race_tick(&game,sub(game.pos,mul(f.ahead,200)),.1f);NCHK(a->health==hull,"leaving the service footprint stops healing immediately");
  r->phase=RACE_WON;race_tick(&game,sub(game.pos,mul(f.ahead,200)),.1f);NCHK(a->health==hull,"finished races cannot continue repairing");
  neon_review_pose(15,0,0);f=race_track_frame(r,15,1);game.pos=add(f.centre,add(mul(f.right,edge*ERL_SERVICE_LANE),mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35)));game.pos=add(game.pos,mul(f.ahead,80));Vec3 lap_from=sub(game.pos,mul(f.ahead,160));r->arcade.actor[0].health=1200;
  race_tick(&game,lap_from,.016f);NCHK(r->gate==16,"mint repair route crosses the same legal lap plane as the main road");race_tick(&game,lap_from,.016f);NCHK(r->gate==16,"repeated service-plane contact cannot grant a second checkpoint");
  neon_review_pose(ordinary,0,0);r->boost_time=0;r->protection=1;r->arcade.actor[0].protection=1;r->arcade.actor[0].ammo=0;r->arcade.actor[0].item=RA_EMPTY;
  memset(r->arcade.branch_boost,0,sizeof(r->arcade.branch_boost));memset(r->arcade.branch_repair,0,sizeof(r->arcade.branch_repair));
  float ring_arc=neon_sky_rings[1][0]*(RACE_GATES/r->laps);int ring_k=(int)ring_arc;RaceTrackFrame ring=race_track_frame(r,ring_k,ring_arc-ring_k);Vec3 ring_c=add(ring.centre,mul(ring.up,RACE_TRACK_FLOOR+neon_sky_rings[1][1]*scale));r->gate=ring_k;game.pos=add(ring_c,mul(ring.ahead,80));race_tick(&game,sub(ring_c,mul(ring.ahead,80)),.016f);
  int ring_save=r->boost_time>.6f&&r->arcade.branch_repair[0]!=0&&save_game(&game,"polish-ring.sav")&&load_game(copy,"polish-ring.sav");
  if(ring_save){copy->race.gate=ring_k;copy->race.boost_time=0;copy->pos=add(ring_c,mul(ring.ahead,80));copy->erl.serial++;race_tick(copy,sub(ring_c,mul(ring.ahead,80)),.016f);ring_save=copy->race.boost_time==0;}
  NCHK(ring_save,"consumed ring remains consumed after save/reload and presentation-history reset");
  neon_review_pose(0,0,0);r->rivals=2;r->arcade.rules.mode=RA_COMBAT;r->arcade.rules.course=RA_CORRIDOR;r->repair=r->crash_time=r->crash_cooldown=0;r->crash_awards=0;r->takedowns=0;r->protection=0;r->reserve=40;
  f=race_track_frame(r,0,.45f);game.yaw=atan2f(f.ahead.x,f.ahead.z);game.pitch=asinf(f.ahead.y);game.roll=0;Vec3 impact=add(f.centre,mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35));
  for(int i=0;i<2;i++){RaceRival *p=&r->pilot[i];memset(p,0,sizeof(*p));p->dir=f.ahead;p->pos=add(impact,add(mul(f.ahead,i?3500:0),mul(f.right,i?900:0)));p->gate=0;p->radius=240;p->speed=2000;p->pace=r->top_speed*.68f;p->health=100;p->mesh=mesh_id("KRAIT");ra_actor_reset(&r->arcade.actor[i+1]);}
  a=&r->arcade.actor[0];a->health=RA_HEALTH;a->protection=0;memset(r->arcade.shot,0,sizeof(r->arcade.shot));erl_replay.serial=0;
  for(int i=0;i<21;i++){r->elapsed=2+i*.1f;game.pos=add(impact,mul(f.ahead,-2000+i*90));erl_replay_capture(.1f);}
  game.pos=sub(impact,mul(f.ahead,50));game.speed=12000;game.boost=1;r->manual_input=1;race_tick(&game,sub(impact,mul(f.ahead,900)),.001f);erl_replay_capture(.016f);
  NCHK(r->pilot[0].disabled&&r->takedowns==1&&r->crash_time==3&&r->crash_awards==1&&r->reserve==40,"qualified ram records one3-second event and one pending fuel receipt");
  NCHK(erl_replay.crash_pinned&&erl_replay.crash_count==21,"crash pins two seconds of existing history within the unchanged65 samples");
  NCHK(save_game(&game,"polish-award.sav")&&load_game(copy,"polish-award.sav")&&copy->race.crash_awards==0&&fabsf(copy->race.reserve-65)<.01f,"v39 reload settles one pending25-point receipt when optional film is discarded");
  NCHK(save_game(copy,"polish-award-settled.sav")&&load_game(copy,"polish-award-settled.sav")&&copy->race.crash_awards==0&&fabsf(copy->race.reserve-65)<.01f,"settled takedown fuel cannot duplicate after another reload");
  const float cuts[]={0,.82f,1.8f,2.9f};int isolated=1;
  for(int n=0;n<4;n++){r->crash_time=3-cuts[n];*copy=game;char name[48];snprintf(name,sizeof(name),"neon59-crash-cut-%d.bmp",n);neon_review_compact_capture(name);isolated&=memcmp(&game,copy,sizeof(Game))==0;}
  NCHK(isolated,"recorded crash rendering restores every byte of authoritative state");
  r->crash_time=3;r->crash_cooldown=.8f;race_crash_present_tick(&game,.79f);NCHK(r->crash_awards==1&&r->reserve==40,"earned fuel waits for the displayed explosion beat");
  race_crash_present_tick(&game,.02f);float awarded=r->reserve;race_crash_present_tick(&game,.01f);NCHK(r->crash_awards==0&&fabsf(awarded-65)<.01f&&r->reserve==awarded,"explosion settles exactly25 points once");
  r->crash_time=3;r->crash_manual_boost=1;r->manual_input=1;r->protection=3;game.boost=1;game.speed=r->top_speed;float elapsed=r->elapsed;Vec3 other=r->pilot[1].pos;
  for(int i=0;i<31;i++)erl_input(0,PSP_CTRL_RTRIGGER,.1f,0,0);
  NCHK(r->crash_time==0&&r->handover==0&&r->elapsed>elapsed+3&&length(sub(r->pilot[1].pos,other))>1000,"live field continues through3-second cutaway with no added handover autopilot");
  NCHK(r->reserve<=65&&r->reserve>62.4f,"retained manual surge spends no reserve during film and normal spending resumes at handback");
  ErlReplayPilot packed=erl_replay_pilot_pack(&r->pilot[1]);RaceRival unpacked={0};erl_replay_pilot_unpack(&unpacked,&packed);
  NCHK(ERL_REPLAY_SAMPLES==65&&!memcmp(&unpacked.pos,&r->pilot[1].pos,sizeof(Vec3))&&unpacked.mesh==r->pilot[1].mesh&&unpacked.radius==r->pilot[1].radius&&unpacked.health==r->pilot[1].health,"compact history retains all65 samples and exact visual transforms");
  /* A fresh authoritative second victim drives a complete native90-frame film.
   * Fixed simulation steps + GPU readback: motion evidence, not FPS evidence. */
  f=race_track_nearest(r,r->gate,game.pos);RaceRival *film=&r->pilot[1];film->pos=add(game.pos,mul(f.ahead,50));film->dir=forward(&game);film->gate=r->gate;film->speed=2000;film->disabled=0;film->protection=film->bump=0;ra_actor_reset(&r->arcade.actor[2]);r->protection=0;r->arcade.actor[0].protection=0;r->crash_time=r->crash_cooldown=0;r->reserve=40;game.speed=12000;game.boost=1;
  race_tick(&game,sub(game.pos,mul(forward(&game),900)),.001f);erl_replay_capture(.016f);
  NCHK(film->disabled&&r->crash_awards==1&&r->takedowns==2,"second distinct generation records one fresh cinematic without replay-generated credit");
  for(int i=0;i<90;i++){if(i)erl_input(0,PSP_CTRL_RTRIGGER,1.f/30,0,0);char name[64];snprintf(name,sizeof(name),"neon59-takedown-%02d.bmp",i);neon_review_compact_capture(name);}
  for(int i=0;i<3&&r->crash_time>0;i++)erl_input(0,PSP_CTRL_RTRIGGER,1.f/30,0,0);
  NCHK(r->crash_time==0&&r->crash_awards==0&&r->takedowns==2,"native90-frame cinematic returns without duplicate takedown or fuel credit");
  NCHK(load_game(copy,"legacy-v38.sav")&&copy->race.rivals==14&&copy->race.crash_awards==0,"v38 fifteen-racer commander imports with an empty new receipt ledger");
  /* Gradual high capture and contact grace are physical, not render effects. */
  f=neon_review_pose(magnetic,0,800);r->boost_time=0;game.speed=0;race_tick(&game,game.pos,1.f/30);f=race_track_nearest(r,r->gate,game.pos);
  float capture_error=dot(sub(game.pos,f.centre),f.up)-RACE_TRACK_FLOOR-race_floor_support(&game,0,f)-35;
  NCHK(capture_error>400&&capture_error<800,"magnetic capture begins gradually instead of snapping a high craft onto the road");
  for(int i=0;i<30;i++)race_tick(&game,game.pos,1.f/30);f=race_track_nearest(r,r->gate,game.pos);
  NCHK(fabsf(dot(sub(game.pos,f.centre),f.up)-RACE_TRACK_FLOOR-race_floor_support(&game,0,f)-35)<2,"magnetic capture settles into firm attachment without changing lateral position");
  f=neon_review_pose(magnetic,0,0);r->protection=0;r->arcade.actor[0].protection=0;game.speed=r->top_speed*.7f;
  float wall_edge=RACE_NEON_EDGE*2.5f*scale,body=race_neon_body_radius(&game);int short_hull=r->arcade.actor[0].health;
  for(int i=0;i<6;i++){f=race_track_nearest(r,r->gate,game.pos);game.pos=add(f.centre,add(mul(f.right,wall_edge-body+10),mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35)));race_tick(&game,game.pos,1.f/30);}
  NCHK(r->arcade.actor[0].health==short_hull,"a brief phase-wall brush is forgiven");
  for(int i=0;i<30;i++){f=race_track_nearest(r,r->gate,game.pos);game.pos=add(f.centre,add(mul(f.right,wall_edge-body+10),mul(f.up,RACE_TRACK_FLOOR+race_floor_support(&game,0,f)+35)));race_tick(&game,game.pos,1.f/30);}
  NCHK(r->arcade.actor[0].health<short_hull-100&&r->arcade.actor[0].health>short_hull-170&&r->repair==0,"sustained wall grinding drains HULL slowly without an arbitrary crash");
  /* Fatal weapon hit gives own3-second recovery; no skip or forward checkpoint. */
  f=neon_review_pose(ordinary,0,0);r->rivals=1;r->arcade.rules.mode=RA_COMBAT;r->protection=0;r->arcade.actor[0].protection=0;r->arcade.actor[0].health=600;r->reserve=70;
  RaceRival *enemy=&r->pilot[0];memset(enemy,0,sizeof(*enemy));enemy->pos=add(game.pos,mul(f.ahead,5000));enemy->dir=f.ahead;enemy->radius=240;enemy->pace=4000;enemy->health=100;enemy->mesh=mesh_id("KRAIT");ra_actor_reset(&r->arcade.actor[1]);
  memset(r->arcade.shot,0,sizeof(r->arcade.shot));RaceProjectile *incoming=&r->arcade.shot[0];incoming->live=1;incoming->owner=1;incoming->kind=RA_MISSILE;incoming->target=-1;incoming->life=1;incoming->dir=mul(f.ahead,-1);incoming->pos=add(game.pos,mul(f.ahead,400));
  int own_gate=r->gate;race_tick(&game,game.pos,.05f);erl_replay_capture(.05f);
  NCHK(r->repair==3&&r->crash_time==3&&r->arcade.actor[0].health==0&&!game.dead,"fatal race hit starts own3-second recorded recovery without killing the commander");
  neon_review_compact_capture("neon59-own-crash.bmp");
  for(int i=0;i<31&&r->repair>0;i++)erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.1f,0,0);
  fprintf(out,"RECOVERY repair=%.6f film=%.6f hull=%d reserve=%.6f gate=%d expected_gate=%d\n",r->repair,r->crash_time,r->arcade.actor[0].health,r->reserve,r->gate,own_gate);
  NCHK(r->repair==0&&r->crash_time==0&&r->arcade.actor[0].health==RA_HEALTH&&r->reserve>=25&&r->reserve<=25.801f&&r->gate==own_gate,"unskippable own recovery rejoins with full HULL and25reserve plus at most100ms of ordinary recharge at the last legal checkpoint");
  game=*baseline;game.erl.serial++;race_restart(&game);r->phase=RACE_RUNNING;r->countdown=0;r->arcade.rules.objective=RA_FINISH;r->arcade.rules.seconds=900;r->arcade.rules.pool=RA_POOL_CLEAN;r->arcade.actor[0].eliminated=1;game.pos=add(r->start,(Vec3){1000000,1000000,1000000});
  FILE *trace=fopen("polish-ai-route.csv","w");if(trace)fprintf(trace,"time,id,gate,x,y,z,speed,hull,disabled\n");int all=0;
  for(int tick=0;tick<7200&&!all;tick++){race_tick(&game,game.pos,1.f/30);all=1;for(int id=0;id<r->rivals;id++){RaceRival *p=&r->pilot[id];if(p->gate<RACE_GATES)all=0;if(trace&&tick%30==0)fprintf(trace,"%.3f,%d,%d,%.3f,%.3f,%.3f,%.3f,%d,%d\n",r->elapsed,id,p->gate,p->pos.x,p->pos.y,p->pos.z,p->speed,r->arcade.actor[id+1].health,p->disabled);}if(trace&&tick%300==0)fflush(trace);}
  if(trace)fclose(trace);NCHK(r->rivals==14&&all,"all14 rivals physically race both laps through magnetic helix and leap");
  fprintf(out,"AI_ROUTE seconds=%.3f",r->elapsed);for(int id=0;id<r->rivals;id++)fprintf(out," id%d_gate%d",id,r->pilot[id].gate);fprintf(out,"\n");
  game=*baseline;
 }
 free(baseline);free(copy);
}
