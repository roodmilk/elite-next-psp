/* Review-only native captures. Read real renderer and drive real input. */
static int erl_arcade_visual_review(void){
 FILE *flag=fopen("erl-arcade-visual.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("erl-arcade-visual.txt","w");if(!out)return 1;
 unsigned *old=fb,*memory=calloc(STRIDE*H+32,sizeof(unsigned));if(!memory){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}
 fb=memory+16;for(int i=0;i<16;i++)memory[i]=memory[16+STRIDE*H+i]=0x19abcdef;
 int failures=0;
 #define AV_CHECK(c,s) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;fflush(out);}while(0)
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;
 AV_CHECK(erl_begin(&game,ERL_TRIAL,0,ERL_CIRCUIT,0),"real clean event entry");game.docked=0;race_restart(&game);page=FLIGHT;
 game.race.countdown=5.5f;Vec3 grid_pos=game.pos;unsigned grid_rng=game.rng;erl_review_capture("arcade-pregrid-flyby.bmp");AV_CHECK(length(sub(game.pos,grid_pos))<.001f&&game.rng==grid_rng&&game.race.elapsed==0,"featured rival introduction never mutates the real grid or clock");
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);AV_CHECK(game.race.countdown==4&&game.race.phase==RACE_COUNTDOWN&&game.race.elapsed==0,"X skips only flyby and preserves physical countdown");
 game.race.countdown=.5f;erl_review_capture("arcade-gantry-red.bmp");game.race.phase=RACE_RUNNING;game.race.countdown=0;game.race.elapsed=.1f;erl_review_capture("arcade-gantry-go.bmp");
 SpaceRace *r=&game.race;RaceGate *q=&r->gates[2];r->gate=2;r->elapsed=43.62f;r->reserve=72;game.pos=add(q->pos,mul(q->normal,-9500));game.yaw=atan2f(q->normal.x,q->normal.z);game.pitch=asinf(q->normal.y);game.roll=0;game.speed=3000;
 for(int i=0;i<r->rivals;i++){r->pilot[i].gate=2;r->pilot[i].pos=add(game.pos,add(mul(q->normal,i<3?2200+i*700:-1300-(i-3)*500),mul(q->right,(i%3-1)*600.f)));r->pilot[i].dir=q->normal;}
 r->arcade.actor[0].item=RA_BOOST;r->arcade.actor[0].health=1800;
 Vec3 pos=game.pos;float yaw=game.yaw,pitch=game.pitch,roll=game.roll;uint32_t rng=game.rng;
 erl_review_capture("arcade-clean-hud.bmp");AV_CHECK(game.pos.x==pos.x&&game.pos.y==pos.y&&game.pos.z==pos.z&&game.yaw==yaw&&game.pitch==pitch&&game.roll==roll&&game.rng==rng,"unobstructed race HUD is read-only to authoritative position, orientation and RNG");
 unsigned before=0,after=0;for(int y=52;y<68;y++)for(int x=185;x<300;x++)before=before*33+fb[y*STRIDE+x];r->arcade.warning=0;r->arcade.warning_side=-1;erl_review_capture("arcade-rear-missile-alert.bmp");for(int y=52;y<68;y++)for(int x=185;x<300;x++)after=after*33+fb[y*STRIDE+x];AV_CHECK(before!=after,"real rear missile state draws the minimal directional alert");r->arcade.warning=-1;
 r->arcade.actor[0].item=RA_SHIELD;r->arcade.actor[0].previous_x=r->arcade.actor[0].require_release=0;game.race.handover=game.race.crash_time=game.race.repair=0;
 erl_input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,1.f/60,0,0);AV_CHECK(r->arcade.actor[0].shield>2.9f&&r->arcade.actor[0].item==RA_EMPTY,"ERL X actually activates held item through its live control loop");
 erl_review_capture("arcade-shield.bmp");
 r->arcade.rules=ra_rules_make(RA_COMBAT,RA_FINISH,RA_POOL_ALL,r->seed);r->arcade.actor[0].shield=0;r->arcade.actor[0].item=RA_MISSILE;r->arcade.lock=1;r->arcade.lock_age=1;
 RaceProjectile *missile=&r->arcade.shot[0];missile->live=1;missile->kind=RA_MISSILE;missile->pos=add(game.pos,mul(q->normal,-1200));missile->dir=q->normal;missile->target=0;r->arcade.warning=0;r->arcade.warning_side=-1;
 erl_review_capture("arcade-combat-warning.bmp");
 /* Same physical ellipse as collision: open centre, nearby glass and ripple. */
 RaceTrackFrame boundary=race_track_frame(r,2,.5f);Vec3 centre=boundary.centre;
 game.pos=centre;game.roll=0;game.yaw=atan2f(q->normal.x,q->normal.z);game.pitch=asinf(q->normal.y);r->arcade.warning=-1;
 for(int i=0;i<r->rivals;i++){r->pilot[i].pos=add(centre,add(mul(q->normal,4500+i*1200),mul(q->right,(i%3-1)*750.f)));r->pilot[i].dir=q->normal;}
 erl_review_capture("arcade-corridor-centre.bmp");AV_CHECK(race_wall_cells==0,"centre of broad combat course has no surrounding violet cage");
 game.pos=add(centre,mul(q->right,-7300));r->arcade.wall_impact=add(centre,add(mul(q->right,-8900),mul(q->normal,3000)));r->arcade.wall_ripple=.25f;
 erl_review_capture("arcade-corridor-impact.bmp");AV_CHECK(race_wall_cells==24&&race_wall_samples<=32768+48,"nearby violet boundary uses a bounded 24-cell grid");
 r->arcade.wall_ripple=0;erl_review_capture("arcade-corridor-near.bmp");
 /* Explicit occlusion check: near opaque depth must conceal boundary glass. */
 unsigned depth_hash=0,pixel_hash=0;for(int y=80;y<180;y++)for(int x=100;x<380;x++){surface_depth[y*W+x]=1;fb[y*STRIDE+x]=RGB(32,70,90);depth_hash=depth_hash*33+surface_depth[y*W+x];pixel_hash=pixel_hash*33+fb[y*STRIDE+x];}
 race_boundary_glass();unsigned dh=0,ph=0;for(int y=80;y<180;y++)for(int x=100;x<380;x++){dh=dh*33+surface_depth[y*W+x];ph=ph*33+fb[y*STRIDE+x];}AV_CHECK(dh==depth_hash&&ph==pixel_hash,"transparent wall preserves depth and never paints over foreground hulls");
 RaceTrackFrame service=race_repair_frame(r,r->gate);game.pos=add(service.centre,mul(service.ahead,-7000));Vec3 service_look=norm(add(service.ahead,mul(service.up,.1f)));game.yaw=atan2f(service_look.x,service_look.z);game.pitch=asinf(service_look.y);r->arcade.actor[0].health=600;r->arcade.actor[0].item=RA_BOOST;game.speed=304;r->place=1;
 erl_review_capture("arcade-rectangular-repair.bmp");
 race_broadcast.valid=0;race_broadcast_tick();r->place=2;r->elapsed+=1;race_broadcast_tick();r->place=1;r->elapsed+=1;race_broadcast_tick();erl_review_capture("arcade-commentary-ticker.bmp");AV_CHECK(race_broadcast.copy&&strstr(race_broadcast.copy,"LEAD"),"commentary ticker reports an actual observed lead change");
 r->arcade.warning=0;unsigned no_ticker=fb[48*STRIDE+337];rect(337,48,135,17,RGB(1,2,3));race_broadcast_draw();AV_CHECK(fb[48*STRIDE+337]==RGB(1,2,3),"missile warning suppresses commentary ticker");r->arcade.warning=-1;(void)no_ticker;
 r->arcade.actor[0].item=RA_MINIGUN;r->arcade.actor[0].ammo=17;r->arcade.warning=-1;r->arcade.actor[0].health=400;game.roll=.5f;erl_review_capture("arcade-minigun-damage-roll.bmp");
 {const int ammunition[]={48,1,0};for(int i=0;i<3;i++){r->arcade.actor[0].ammo=ammunition[i];r->arcade.actor[0].item=ammunition[i]?RA_MINIGUN:RA_EMPTY;char shot[48];snprintf(shot,sizeof(shot),"arcade-minigun-%02d.bmp",ammunition[i]);erl_review_capture(shot);}}
 r->arcade.actor[0].item=RA_MISSILE;r->arcade.lock=-1;erl_review_capture("arcade-missile-no-lock.bmp");r->arcade.lock=1;r->arcade.lock_age=.15f;erl_review_capture("arcade-missile-lock.bmp");
 r->arcade.actor[0].rocket=2.5f;erl_review_capture("arcade-active-boost.bmp");r->arcade.actor[0].rocket=0;r->arcade.actor[0].shield_break=.2f;erl_review_capture("arcade-shield-break.bmp");r->arcade.actor[0].shield_break=0;
 r->rivals=10;r->place=10;erl_review_capture("arcade-rank-10-of-11.bmp");r->rivals=6;r->place=1;
 r->arcade.rules=ra_rules_make(RA_TIME_TRIAL,RA_MEDALS,1u<<RA_BOOST,r->seed);r->rivals=0;r->arcade.actor[0].health=RA_HEALTH;r->arcade.actor[0].item=RA_BOOST;erl_review_capture("arcade-solo-delta.bmp");
 /* Genuine bounded recording and render-only replay, including all camera cuts. */
 r->rivals=6;r->phase=RACE_RUNNING;for(int i=0;i<140;i++){game.pos=add(game.pos,mul(q->normal,450));game.time+=.1f;r->elapsed+=.1f;erl_replay_capture(.1f);}
 AV_CHECK(erl_replay.count==ERL_REPLAY_SAMPLES,"recent-action buffer stays bounded after more than its twelve-second window");
 erl_finish(&game,1,0);unsigned paid=game.erl.paid;int cash=game.credits,points=game.erl.points[0];pos=game.pos;yaw=game.yaw;pitch=game.pitch;roll=game.roll;rng=game.rng;float elapsed=r->elapsed;int gate=r->gate,phase=r->phase;
 AV_CHECK(erl_replay_tick(0,.016f)&&erl_replay.active,"actual recorded action starts before the existing result reveal");
 uint32_t branch_before=game.race.arcade.branch_boost[0],repair_before=game.race.arcade.branch_repair[0];int isolated=1;const float cuts[]={1,4.5f,8.2f};for(int i=0;i<3;i++){erl_replay.age=cuts[i];erl_replay_draw();char filename[40];snprintf(filename,sizeof(filename),"arcade-replay-cut-%d.bmp",i);dump_native_bmp(filename);isolated&=game.pos.x==pos.x&&game.pos.y==pos.y&&game.pos.z==pos.z&&game.yaw==yaw&&game.pitch==pitch&&game.roll==roll&&game.rng==rng&&r->elapsed==elapsed&&r->gate==gate&&r->phase==phase&&game.credits==cash&&game.erl.paid==paid&&game.erl.points[0]==points&&game.race.arcade.branch_boost[0]==branch_before&&game.race.arcade.branch_repair[0]==repair_before;}
 AV_CHECK(isolated,"all replay cameras preserve authoritative pose, RNG, checkpoints, results and payment");
 AV_CHECK(erl_replay_tick(PSP_CTRL_CROSS,.016f)&&!erl_replay.active&&!erl_replay_tick(0,.016f)&&game.credits==cash,"X skips once and consumes its input without replaying rewards or selecting results");
 /* LocalTV uses the same bounded actual recording after the event is cleared. */
 game.erl.event_kind=game.erl.result=game.erl.resume=0;memset(&game.race,0,sizeof(game.race));page=LOCALTV;local_tv_program=0;local_tv_beat=2;local_tv_reveal=1000;local_tv_clock=3.2f;
 AV_CHECK(erl_tv_has_highlights(),"LocalTV offers highlights only for its matching recorded paid result");
 Game *tv_state=malloc(sizeof(Game));if(tv_state)*tv_state=game;unsigned tv_paint=ship_paint[game.ship];
 local_tv_screen();dump_native_bmp("arcade-local-tv-highlights.bmp");
 AV_CHECK(tv_state&&!memcmp(tv_state,&game,sizeof(Game))&&ship_paint[game.ship]==tv_paint,"LocalTV replay preserves all commander, race, career, payment and ship-paint state");free(tv_state);
 unsigned recorded=erl_replay.serial;erl_replay.serial++;AV_CHECK(!erl_tv_has_highlights(),"a stale or missing recording cannot invent LocalTV highlights");erl_replay.serial=recorded;
 int bounds=1;for(int i=0;i<16;i++)bounds&=memory[i]==0x19abcdef&&memory[16+STRIDE*H+i]==0x19abcdef;AV_CHECK(bounds,"all native race states preserve framebuffer guards");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(memory);return 1;
 #undef AV_CHECK
}
