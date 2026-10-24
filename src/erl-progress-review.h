/* Review-only captures of production screens and factual integrations. */
static void erl_progress_capture(const char *name){
 camera_override_clear();proj_ox=W/2;proj_oy=H/2;proj_focal=240;clipx0=clipy0=0;clipx1=W;clipy1=H;drawcount=0;
 if(page==LOCALTV){rect(0,0,W,H,EB);local_tv_screen();dump_native_bmp(name);}else erl_review_capture(name);
}
static int erl_progress_review(void){
 FILE *flag=fopen("erl-progress-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("erl-progress-review.txt","w");if(!out)return 1;
 int failures=0;unsigned *old=fb,*memory=calloc(STRIDE*H+32,sizeof(unsigned));if(!memory){fclose(out);return 1;}fb=memory+16;
 for(int i=0;i<16;i++)memory[i]=memory[16+STRIDE*H+i]=0x19abcdef;
 #define PR_CHECK(c,s) do{int ok=!!(c);failures+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);fflush(out);}while(0)
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl_local.selection=1;
 PR_CHECK(erl_begin(&game,ERL_TRIAL,0,ERL_CIRCUIT,0),"real exhibition creates review course");
 game.race.gate=32;game.race.elapsed=140;game.race.arcade.rules.sponsor=1;game.race.arcade.rules.seed=4;game.race.arcade.sponsor_done=1;erl_finish(&game,1,0);
 ErlProgress *p=&game.erl_local.progress;unsigned records=p->records;int units=game.credits;
 game.erl.event_kind=game.erl.result=game.erl.resume=0;game.race.phase=RACE_NONE;third_person=0;
 page=ERL_PAGE;erl_screen=ERL_DRIVER;erl_progress_capture("career-driver.bmp");
 PR_CHECK(p->records==records&&game.credits==units,"driver rendering preserves recorded result and exact reward");
 char author[64],body[256];social_post_text(SB_ERL_WIN|(7u<<9),0,author,sizeof(author),body,sizeof(body));
 PR_CHECK(strstr(body,"first ERL win")&&strstr(body,"Jameson"),"public milestone uses authored ERL copy and commander identity");
 page=LOCALTV;local_tv_program=0;local_tv_beat=2;local_tv_reveal=1000;erl_progress_capture("career-local-tv-result.bmp");
 PR_CHECK(strstr(local_tv_copy(),"JAMESON")&&strstr(local_tv_copy(),"140.00")&&strstr(local_tv_copy(),"1600"),"LocalTV reports exact stored commander, time and awarded units");
 local_tv_beat=3;erl_progress_capture("career-local-tv-sponsor.bmp");PR_CHECK(strstr(local_tv_copy(),"sponsor challenge"),"LocalTV sponsor segment requires a real completed challenge");
 PR_CHECK(audio_race_key_cue(SFX_MISSILE)&&audio_race_key_cue(SFX_BOOST)&&!audio_race_key_cue(SFX_LASER),"race music dip is limited to meaningful cues, never every minigun bullet");
 game.erl.event_kind=0;game.erl.result=0;game.job_n=1;game.jobs[0]=(Job){.type=MISSION_RACE,.dest=game.system,.origin=game.system,.item=1,.target=40,.reward=6000,.time=300};
 page=ERL_PAGE;erl_screen=ERL_BRIEF;erl_board_brief=1;
 for(int i=0;i<9;i++){game.jobs[0].target=40+(i<8?i:7);game.jobs[0].origin=i==8?1:0;game.race.phase=RACE_NONE;race_prepare(&game);char file[40];snprintf(file,sizeof(file),"board-contract-%d.bmp",i);erl_progress_capture(file);unsigned goal=i<4?i==2?RA_MEDALS:RA_FINISH:i==4?RA_TAKEDOWNS:i==5?RA_ELIMINATOR:i==6?RA_HUNTER:i==7?RA_ESCAPE:RA_ENDURANCE;PR_CHECK(game.race.arcade.rules.objective==goal&&ra_rules_valid(&game.race.arcade.rules,game.erl.licence),"board brief and actual event use the advertised contract format");}
 erl_board_brief=0;game.job_n=0;game.erl.event_layout=1;game.erl.event_kind=ERL_TRIAL;game.erl_local.event_rules=ra_rules_make(RA_COMBAT,RA_FINISH,RA_POOL_ALL,19);game.race.phase=RACE_NONE;race_prepare(&game);game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.elapsed=43.62f;page=FLIGHT;
 for(int i=0;i<4;i++){int section=3+i*8;RaceGate *q=&game.race.gates[section];Vec3 c,h,after;PR_CHECK(race_hazard(&game.race,section,game.race.elapsed,&c,&h),"signature venue supplies real moving hazard geometry");race_hazard(&game.race,section,game.race.elapsed+2,&after,&h);PR_CHECK(length(sub(after,c))>100,"hazard moves with authoritative race time");game.race.gate=section;game.pos=add(c,mul(q->normal,-7000));game.yaw=atan2f(q->normal.x,q->normal.z);game.pitch=asinf(q->normal.y);game.roll=0;char file[40];snprintf(file,sizeof(file),"moving-hazard-%d.bmp",i);erl_progress_capture(file);}
 {RaceTrackFrame service=race_repair_frame(&game.race,2);RaceGate aligned=game.race.gates[2];aligned.normal=service.ahead;aligned.right=service.right;aligned.up=service.up;RaceGate *q=&aligned;Vec3 centre=service.centre;game.race.gate=2;game.pos=add(centre,mul(q->normal,-9500));game.yaw=atan2f(q->normal.x,q->normal.z);game.pitch=asinf(q->normal.y);game.roll=0;erl_progress_capture("repair-sweep-a.bmp");game.race.elapsed+=1.1f;erl_progress_capture("repair-sweep-b.bmp");
  RaceRival *rival=&game.race.pilot[0];rival->mesh=mesh_id("COBRA MK 3");rival->radius=200;rival->gate=2;rival->pos=add(game.pos,mul(q->normal,600));rival->dir=norm(add(q->normal,mul(q->up,.4f)));rival->protection=3;game.race.arcade.actor[1].protection=3;erl_progress_capture("respawn-protection.bmp");rival->protection=game.race.arcade.actor[1].protection=0;game.race.arcade.actor[1].health=600;erl_progress_capture("damaged-rival.bmp");game.race.arcade.actor[0].shield_break=.23f;erl_progress_capture("shield-break-shards.bmp");}
 erl_review_fixture(1);game.erl.referral=1;game.erl.licence=2;page=ERL_PAGE;erl_screen=ERL_SEASON;erl_season_offer(&game);erl_progress_capture("season-venues-before-entry.bmp");
 int home=game.system;PR_CHECK(erl_season_start(&game),"native season entry registers home championship");game.erl.round=2;game.erl_local.season[home].round=2;
 PR_CHECK(erl_round_venue(&game,2)==home,"every championship round remains in its owning home system");
 PR_CHECK(erl_begin(&game,ERL_OFFICIAL,2,ERL_RUSH,0)&&game.system==home&&game.erl.event_system==home,"native round briefing and race preserve the home system");erl_screen=ERL_BRIEF;erl_requested_kind=ERL_OFFICIAL;erl_progress_capture("home-round-brief.bmp");fprintf(out,"INFO native sponsor room=%d pending=%u champ=%d prize=%u\n",erl_sponsor_room(&game),game.erl_local.pending_count,game.erl.championship,game.erl_local.season[home].prize_state);
 for(int i=0;i<31;i++){float remaining=i*.01f;PR_CHECK(ra_shuffle_icon(1u<<RA_MINIGUN,remaining)==RA_MINIGUN,"fixed-item roulette never advertises a disallowed award");}
 erl_review_fixture(1);game.erl.referral=game.erl.licence=1;game.erl_local.selection=0;PR_CHECK(erl_begin(&game,ERL_TRIAL,0,ERL_CIRCUIT,0),"trackside artwork uses a real local event rather than a stale away selection");game.docked=0;race_restart(&game);game.race.phase=RACE_RUNNING;game.race.elapsed=45;game.race.gate=1;page=FLIGHT;
 {RaceGate *q=&game.race.gates[1];Vec3 middle=mul(add(game.race.gates[0].pos,q->pos),.5f),advert=add(middle,add(mul(q->right,-10500),mul(q->up,2400)));game.pos=add(advert,mul(q->normal,-6500));game.yaw=atan2f(q->normal.x,q->normal.z);game.pitch=asinf(q->normal.y);game.roll=0;erl_progress_capture("trackside-advert.bmp");}
 game.race.arcade.last_hit=5;game.race.arcade.cue_age=.2f;erl_progress_capture("near-miss-streaks.bmp");
 int guards=1;for(int i=0;i<16;i++)guards&=memory[i]==0x19abcdef&&memory[16+STRIDE*H+i]==0x19abcdef;PR_CHECK(guards,"all added native screens preserve framebuffer guards");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(memory);return 1;
 #undef PR_CHECK
}
