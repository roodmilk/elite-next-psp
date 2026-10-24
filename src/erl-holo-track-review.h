/* Real game geometry/physics/native rendering, never a design mockup. */
static int erl_holo_track_review(void){
 FILE *flag=fopen("erl-holo-track-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("erl-holo-track-review.txt","w");if(!out)return 1;
 int failures=0,checks=0;
 #define HT_CHECK(c,s) do{int ok=!!(c);checks++;failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);fflush(out);}while(0)
 unsigned *old=fb,*memory=calloc(STRIDE*H+32,sizeof(unsigned));Game *saved=malloc(sizeof(Game));if(!memory||!saved){fprintf(out,"RESULT 1 failures\n");fclose(out);free(memory);free(saved);return 1;}
 fb=memory+16;for(int i=0;i<16;i++)memory[i]=memory[16+STRIDE*H+i]=0x1234abcd;
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl_local.selection=1;
 HT_CHECK(erl_begin(&game,ERL_TRIAL,0,ERL_RUSH,0),"combat trial prepares actual course");
 SpaceRace *r=&game.race;int seams=1,frames=1;
 for(int k=0;k<RACE_GATES;k++){
  RaceTrackFrame a=race_track_frame(r,k,0),b=race_track_frame(r,k,1);Vec3 start=k?r->gates[k-1].pos:r->start;
  seams&=length(sub(a.centre,start))<.1f&&length(sub(b.centre,r->gates[k].pos))<.1f;
  if(k+1<RACE_GATES){RaceTrackFrame n=race_track_frame(r,k+1,0);seams&=length(sub(b.centre,n.centre))<.1f&&dot(b.ahead,n.ahead)>.9999f&&dot(b.up,n.up)>.9999f&&dot(b.right,n.right)>.9999f;}
  for(int t=0;t<=6;t++){RaceTrackFrame f=race_track_frame(r,k,t/6.f);frames&=fabsf(length(f.up)-1)<.001f&&fabsf(length(f.right)-1)<.001f&&fabsf(dot(f.up,f.ahead))<.001f&&dot(f.up,(Vec3){0,1,0})>.7f;}
 }
 HT_CHECK(seams,"ribbon joins every real ordered course node with continuous tangents");HT_CHECK(frames,"climbs and bends retain stable orthonormal track frames");
 for(int i=0;i<STRIDE*H;i++)fb[i]=RGB(1,2,3);race_holo_track_draw();int pending=1;for(int i=0;i<STRIDE*H;i++)pending&=fb[i]==RGB(1,2,3);
 HT_CHECK(pending,"accepted pre-start event does not draw racing floor over ordinary flight");
 game.docked=0;race_restart(&game);page=FLIGHT;r->phase=RACE_RUNNING;r->countdown=0;r->elapsed=43.62f;r->gate=2;game.speed=4200;r->arcade.actor[0].item=RA_BOOST;
 RaceTrackFrame frame=race_track_frame(r,2,.35f);game.pos=add(frame.centre,mul(frame.up,200));game.yaw=atan2f(frame.ahead.x,frame.ahead.z);game.pitch=asinf(frame.ahead.y);game.roll=0;
 for(int i=0;i<r->rivals;i++){r->pilot[i].gate=2;r->pilot[i].pos=add(frame.centre,add(mul(frame.ahead,(i-2)*900.f),mul(frame.right,(i%3-1)*650.f)));r->pilot[i].dir=frame.ahead;}
 *saved=game;erl_review_capture("holo-track-combat-bend.bmp");
 int floor_ok=race_floor_line_count>=0&&race_floor_line_count<=768&&race_floor_grid_count>=0&&race_floor_grid_count<=512&&race_floor_samples<=33000;HT_CHECK(floor_ok,"real race view keeps ribbon within fixed geometry/raster budgets");
 int purple=0;for(int y=65;y<105;y++)for(int x=200;x<288;x++)purple+=fb[y*STRIDE+x]==RGB(178,109,238);
 HT_CHECK(!purple,"large filled top guidance arrow is absent from racing HUD");
 r->arcade.warning=0;r->arcade.warning_side=-1;unsigned pose=sw_review_hash(&game,sizeof(game));race_hud();HT_CHECK(pose==sw_review_hash(&game,sizeof(game)),"missile-direction HUD preserves all authoritative gameplay state");r->arcade.warning=-1;
 game.roll=.35f;erl_review_capture("holo-track-banked-view.bmp");game.roll=0;game.pos=add(frame.centre,mul(frame.up,3500));erl_review_capture("holo-track-high-route.bmp");
 for(int mode=RA_CLEAN;mode<=RA_TIME_TRIAL;mode++){
  game=*saved;r=&game.race;r->arcade.rules.mode=mode;r->arcade.rules.course=mode==RA_COMBAT?RA_CORRIDOR:RA_RINGS;r->rivals=0;
  r->arcade.actor[0].protection=r->protection=3;r->arcade.actor[0].shield=2;r->arcade.actor[0].rocket=3;game.boost=1;game.speed=r->top_speed*2.5f;
  game.pos=add(frame.centre,mul(frame.up,RACE_TRACK_FLOOR-1100));int health=r->arcade.actor[0].health;game_tick(&game,.016f,0,0,0,0);
  RaceTrackFrame near=race_track_nearest(r,r->gate,game.pos);float height=dot(sub(game.pos,near.centre),near.up);
  HT_CHECK(height>=RACE_TRACK_FLOOR+race_floor_support(&game,0,near)-.1f&&r->arcade.actor[0].health==health&&r->arcade.actor[0].shield>1.9f,"high-speed floor contact contains ship without damage or consuming shield");
  game=*saved;r=&game.race;r->arcade.rules.mode=mode;r->arcade.rules.course=mode==RA_COMBAT?RA_CORRIDOR:RA_RINGS;r->rivals=0;r->arcade.actor[0].protection=r->protection=3;
  game.pos=add(frame.centre,mul(frame.up,2800));Vec3 initial=game.pos;game_tick(&game,.016f,0,0,0,0);
  HT_CHECK(length(sub(game.pos,initial))<150,"flying above floor retains vertical freedom without centreline snapping");
  erl_review_capture(mode==RA_CLEAN?"holo-track-clean-rings.bmp":mode==RA_TIME_TRIAL?"holo-track-solo.bmp":"holo-track-combat-high.bmp");
 }
 game=*saved;r=&game.race;HT_CHECK(save_game(&game,"holo-track.sav")&&load_game(saved,"holo-track.sav")&&saved->race.gate==r->gate&&saved->race.seed==r->seed,"curved track requires no new save fields and preserves accepted course on reload");
 int guards=1;for(int i=0;i<16;i++)guards&=memory[i]==0x1234abcd&&memory[16+STRIDE*H+i]==0x1234abcd;
 HT_CHECK(guards,"all native track views preserve framebuffer guard words");
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,failures);fclose(out);fb=old;free(memory);free(saved);return 1;
 #undef HT_CHECK
}
