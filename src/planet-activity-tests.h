/* Snapshot actual generation for baseline/candidate semantic comparison. */
{
 FILE *pool=fopen("planet-activity-pools.txt","w");int world_count=0,empty_count=0,pools_valid=1;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  game_init(&g);g.system=sys;launch(&g);g.contract=-1;g.job_n=0;
  if(g.bodies[body].type==GAS||g.bodies[body].type==SUN)continue;
  g.pos=add(g.bodies[body].pos,(Vec3){0,0,-g.bodies[body].radius-500});g.yaw=g.pitch=0;
  int entered=approach_planet(&g,body)&&enter_planet(&g),minerals=0,alive=0;
  pools_valid &= entered&&g.planet==body&&g.surface==0;
  for(int i=0;i<LIFE_COUNT;i++){
   Lifeform *l=&g.life[i];minerals+=l->alive&&l->kind==LIFE_MINERAL;alive+=l->alive!=0;
   pools_valid &= !l->scanned;
   if(pool)fprintf(pool,"%d %d %u %d %d %d %d %.9g %.9g %.9g\n",sys,body,g.bodies[body].seed,i,l->kind,l->alive,l->scanned,l->pos.x,l->pos.y,l->pos.z);
  }
  world_count++;empty_count+=!minerals;pools_valid &= alive==LIFE_COUNT;
 }
 if(pool){fprintf(pool,"SUMMARY worlds=%d empty=%d valid=%d\n",world_count,empty_count,pools_valid);fclose(pool);}
 CHECK(pool&&world_count==768&&pools_valid,"planet activity: all generated solid-world pools contain eight unscanned objects");
 CHECK(empty_count==0,"planet activity: every generated solid world offers at least one mineral");
 game_init(&g);launch(&g);
}

/* Real fallback deposit, existing scan/economy/save path; no target relocation. */
{
 int economy_ok=1,return_ok=1,save_ok=1,cue_ok=1;
 for(int refinery=0;refinery<2;refinery++)for(int full=0;full<2;full++){
  game_init(&g);g.system=0;launch(&g);story_complete(&g);g.contract=-1;g.job_n=0;
  g.approach=3;int entered=enter_planet(&g);Vec3 deposit=g.life[0].pos;
  economy_ok &= entered&&g.life[0].kind==LIFE_MINERAL;
  g.pos=surface_site(&g,1);g.speed=8;return_ok &= land_planet(&g)&&eva_toggle(&g);
  /* Isolate the deposit for arithmetic; generation snapshot tests untouched pools. */
  for(int i=1;i<LIFE_COUNT;i++)g.life[i].alive=0;
  if(refinery){g.fit[FIT_UTIL]=21;fit_rebuild(&g);}
  memset(g.cargo,0,sizeof(g.cargo));g.cargo[0]=cargo_capacity(&g)-(full?0:1);
  int cash=g.credits,disc=g.discoveries,ore=g.scanned_minerals,commodity=refinery?9:12;
  g.cue=SFX_NONE;int scanned=survey_scan(&g);
  economy_ok &= scanned&&g.credits==cash+(full?160:120)&&g.discoveries==disc+1&&g.scanned_minerals==ore+1&&g.cargo[commodity]==!full&&cargo_used(&g)==cargo_capacity(&g);
  economy_ok &= g.life[0].pos.x==deposit.x&&g.life[0].pos.y==deposit.y&&g.life[0].pos.z==deposit.z;
  cue_ok &= g.cue==SFX_SCAN;g.cue=SFX_NONE;cash=g.credits;
  economy_ok &= !survey_scan(&g)&&g.credits==cash&&g.discoveries==disc+1;cue_ok &= g.cue==SFX_NONE;
  return_ok &= eva_toggle(&g)&&takeoff_planet(&g);leave_planet(&g);return_ok &= g.planet<0&&g.credits==cash;
  g.pos=(Vec3){0,0,2800};g.speed=0;
  for(int i=0;i<NPC_COUNT;i++)g.npc[i].alive=0;
  return_ok &= dock(&g);
  for(int i=0;i<1200&&!g.docked;i++)game_tick(&g,1.f/60,0,0,0,0);
  return_ok &= g.docked&&!g.dead&&g.cargo[commodity]==!full&&g.credits==cash;
  Game persisted;int keep=g.credits;
  int saved=save_game(&g,"test-activity-return.sav")&&load_game(&persisted,"test-activity-return.sav");
  save_ok &= saved&&persisted.credits==keep&&persisted.discoveries==g.discoveries&&persisted.scanned_minerals==ore+1&&persisted.cargo[commodity]==!full;
  if(saved&&!full){int price=persisted.price[commodity];save_ok &= trade(&persisted,commodity,0)&&persisted.cargo[commodity]==0&&persisted.credits==keep+price;}
  remove("test-activity-return.sav");remove("test-activity-return.sav.bak");remove("test-activity-return.sav.tmp");
 }
 CHECK(economy_ok,"planet activity: fallback mineral preserves position and existing normal/refinery/full-hold/same-object reward rules");
 CHECK(cue_ok,"planet activity: fallback scan emits existing success cue only on a real award");
 CHECK(return_ok,"planet activity: fallback cargo or full-hold payment survives boarding, orbit and guided hub return");
 CHECK(save_ok,"planet activity: fallback earnings survive V13 save/load and earned ore/alloy sells at actual hub price");
 game_init(&g);launch(&g);
}
