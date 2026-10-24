/* These exercise the gameplay handlers, not expedition_advance or fabricated
 * objective counters. Locations are arranged as fixtures; movement/collection,
 * purchases, damage, site interactions and races use production functions. */
static void xp_test_fixture(Game *g){
 game_init(g);story_complete(g);g->opening.stage=0;g->credits=1000000;g->guild_chapter=4;
 memset(g->cargo,0,sizeof(g->cargo));g->fit[FIT_WPN]=1;fit_rebuild(g);g->fuel=player_ships[g->ship].range;
}
static void xp_test_location(Game *g){
 expedition_ack(g,0);
 int sys=expedition_system(g,0);if(g->system!=sys){g->system=sys;game_spawn(g);}
 g->docked=0;g->dock_stage=0;g->planet=-1;g->surface=0;g->jump=0;g->police_stop=0;g->speed=0;g->attacked=0;g->dead=0;g->encounter=999;
 const ExpeditionStage *s=expedition_stage(g,0);
 if(s->loc>=3){g->approach=expedition_world(g,0);enter_planet(g);g->surface=2;}
 expedition_build(g);expedition_tick(g,.01f);
}
static int xp_test_action(Game *g){
 const ExpeditionStage *s=expedition_stage(g,0);if(!s)return 0;int stage=g->expeditions[0].stage,kind=s->kind;
 ExpeditionRuntime *r=&g->expedition_runtime;int arg=s->arg,need=s->need;
 switch(kind){
 case XP_HAIL:case XP_PATROL:
  for(int k=0;k<(kind==XP_PATROL?need:1);k++){int a=kind==XP_PATROL?k:arg==1?1:0,id=r->npc[a];if(id<0)return 0;g->pos=g->npc[id].pos;if(!mission_interact(g,NPC_ID_MIN+id))return 0;}break;
 case XP_COMBAT:
  for(int k=0;k<need;k++){int id=r->npc[k];if(id<0)return 0;hit(g,id,10000,1);}break;
 case XP_RECOVER:case XP_COMPETE:case XP_MINE:case XP_REFINE:
  for(int k=0;k<need;k++){int pod=r->pod[k];if(pod<0)return 0;g->pos=g->debris[pod].pos;
   if(kind==XP_MINE||kind==XP_REFINE){if(kind==XP_REFINE)g->upgrades|=131072;for(int shot=0;shot<100&&g->debris[pod].rock;shot++)mine_rock(g,DEBRIS_ID_MIN+pod);}
   if(!salvage_collect(g,DEBRIS_ID_MIN+pod))return 0;
  }if(kind==XP_COMPETE)expedition_tick(g,.01f);break;
 case XP_BUY:
  g->docked=1;g->stock[arg]=20;for(int k=0;k<need;k++)if(!trade(g,arg,1))return 0;break;
 case XP_DOCK:g->station_variant=0;docking_complete(g);break;
 case XP_ESCORT:
  for(int t=0;t<400&&g->job_n&&g->expeditions[0].stage==(unsigned)stage;t++){
   for(int k=0;k<3;k++)if(r->npc[k]>=0)expedition_npc_control(g,r->npc[k],.5f);
   int id=r->npc[0];if(id<0)return 0;g->pos=add(g->npc[id].pos,(Vec3){0,200,0});expedition_tick(g,.5f);
  }break;
 case XP_WAYPOINT:case XP_CHASE:case XP_MAP:case XP_OBSERVE:
  for(int t=0;t<need;t++){expedition_tick(g,.01f);g->pos=expedition_waypoint(g);g->speed=0;for(int k=0;k<400&&g->job_n&&g->expeditions[0].stage==(unsigned)stage&&g->expeditions[0].progress==(unsigned)t;k++)expedition_tick(g,.05f);}break;
 case XP_CHOICE:if(!expedition_choice(g,0,0))return 0;break;
 case XP_FLORA:case XP_SAMPLE:
  for(int k=0;k<LIFE_COUNT&&g->job_n&&g->expeditions[0].stage==(unsigned)stage;k++)if(g->life[k].kind==(kind==XP_FLORA?LIFE_FLORA:LIFE_MINERAL)){g->pos=g->life[k].pos;if(!survey_scan_target(g,k))return 0;}break;
 case XP_BEHAVIOUR:{int id=-1;for(int k=0;k<LIFE_COUNT;k++)if(g->life[k].kind==LIFE_FAUNA){id=k;break;}if(id<0)return 0;g->pos=g->life[id].pos;g->life[id].behaviour=FAUNA_FEED;expedition_tick(g,.05f);g->life[id].behaviour=FAUNA_ALERT;expedition_tick(g,.05f);break;}
 case XP_SITE:{int id=expedition_site_id(g,0);if(id<0)return 0;Vec3 p=surface_poi(g,id);FieldBuilding b=field_site_building(g,id);int found=0;for(int a=0;a<8;a++){float angle=a*.785398f;g->pos=add(p,(Vec3){cosf(angle)*(b.w+15),0,sinf(angle)*(b.d+15)});if(id==0)g->pos=p;if(surface_nearest_site(g,55)==id){found=1;break;}}if(!found)return 0;
  /* Fixture prerequisite records represent earlier optional exploration. */
  for(int k=0;k<2;k++)g->surface_progress[g->system][g->planet]|=1u<<(8+k);g->surface_progress[g->system][g->planet]|=8;
  for(int k=0;k<LIFE_COUNT;k++)if(g->life[k].kind==LIFE_FAUNA)g->surface_progress[g->system][g->planet]|=1u<<(8+k);
  if(arg==FIELD_DISH&&!(g->surface_progress[g->system][g->planet]&field_site_bit(id))){g->surface_progress[g->system][g->planet]|=OBS_CLUES;if(!observatory_resolve(g,id,2))return 0;}
  else surface_interact(g);break;}
 case XP_DISCOVERY:expedition_external(g,DISCOVERY_ID_MIN,(Vec3){0,0,10000});expedition_event(g,XP_DISCOVERY,DISCOVERY_ID_MIN,0,1);break;
 case XP_WHALE:expedition_event(g,XP_WHALE,WHALE_TARGET_ID,0,1);break;
 case XP_ANOMALY:g->pos=g->anomaly[arg].pos;if(!analysis_scan(g,ANOMALY_ID_MIN+arg))return 0;break;
 case XP_RACE:
  race_restart(g);g->race.phase=RACE_RUNNING;g->race.countdown=0;
  for(int k=0;k<RACE_GATES;k++){RaceGate q=g->race.gates[k];g->pos=add(q.pos,mul(q.normal,300));race_tick(g,add(q.pos,mul(q.normal,-300)),.05f);}break;
 default:return 0;
 }
 return !g->job_n||g->expeditions[0].stage>(unsigned)stage;
}
int expedition_tests(const char *path){
 FILE *out=fopen(path,"w");if(!out)return 1;Game *g=malloc(sizeof(Game)),*loaded=malloc(sizeof(Game));int failures=0,stages=0;
 if(!g||!loaded){free(g);free(loaded);fclose(out);return 1;}
 #define XPCHECK(c,label) do{int good=!!(c);fprintf(out,"%s %s\n",good?"PASS":"FAIL",label);failures+=!good;fflush(out);}while(0)
 for(int index=0;index<EXPEDITION_COUNT;index++){
  xp_test_fixture(g);int type=EXPEDITION_FIRST+index,destination=-1;
  for(int origin=0;origin<256&&destination<0;origin++){g->system=origin;destination=expedition_destination(g,type);}
  char label[160];snprintf(label,sizeof(label),"%02d %s: canonical destination and actual prerequisites",index+1,mission_name(type));
  if(index==18)g->upgrades|=131072;XPCHECK(destination>=0&&expedition_start(g,type,destination,12000),label);if(!g->job_n)continue;
  const ExpeditionDef *d=expedition_def(type);int actual=1,persist=1,paid=0;
  for(int n=0;n<d->count&&g->job_n;n++){
   int stage=g->expeditions[0].stage;g->docked=1;g->planet=-1;ExpeditionState before=g->expeditions[0];
   persist&=save_game(g,"expedition-test.sav")&&load_game(loaded,"expedition-test.sav")&&loaded->job_n==1&&loaded->jobs[0].type==type&&!memcmp(&before,&loaded->expeditions[0],sizeof(before));
   if(persist)memcpy(g,loaded,sizeof(*g));xp_test_location(g);int ok=xp_test_action(g);stages+=ok;
   fprintf(out,"%s %02d / stage %d / goal %d / %s\n",ok?"PASS":"FAIL",index+1,stage+1,d->stages[stage].kind,d->stages[stage].objective);fflush(out);failures+=!ok;actual&=ok;if(!ok)break;
  }
  snprintf(label,sizeof(label),"%02d all %d stages preserve progress, cargo provenance and contact history through saves",index+1,d->count);XPCHECK(persist,label);
  paid=g->job_n==0&&g->mission_receipt_n==1&&g->mission_receipts[0].type==type&&!strcmp(g->mission_receipts[0].sender,d->sender);
  snprintf(label,sizeof(label),"%02d pays once after its actual final objective with the correct issuer",index+1);XPCHECK(actual&&paid,label);
  int cash=g->credits;docking_complete(g);XPCHECK(g->credits==cash,"repeat docking cannot award the contract prize twice");
 }
 fprintf(out,"INFO completed gameplay stages %d / 155\n",stages);
 /* Negative and branch cases catch the old hail-to-win shortcuts. */
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+8,g->system,12000);xp_test_location(g);int civilian=g->expedition_runtime.npc[0];g->pos=g->npc[civilian].pos;mission_interact(g,NPC_ID_MIN+civilian);XPCHECK(g->expeditions[0].stage==0,"hailing an innocent cannot identify or complete the wanted suspect stage");
 int suspect=g->expedition_runtime.npc[1];uint32_t identity=g->npc[suspect].vessel_seed;g->pos=g->npc[suspect].pos;mission_interact(g,NPC_ID_MIN+suspect);expedition_tick(g,.01f);XPCHECK(g->expeditions[0].stage==1&&g->npc[g->expedition_runtime.npc[0]].vessel_seed==identity,"the confirmed suspect keeps the same physical identity into combat");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+17,g->system,12000);xp_test_location(g);g->docked=1;g->stock[12]=10;trade(g,12,1);XPCHECK(g->expeditions[0].stage==0&&g->expeditions[0].progress==0,"purchasing minerals cannot satisfy an extraction order");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+3,g->system,12000);xp_test_location(g);xp_test_action(g);xp_test_location(g);int pod=g->expedition_runtime.pod[0];g->pos=g->debris[pod].pos;salvage_collect(g,DEBRIS_ID_MIN+pod);g->station_variant=0;docking_complete(g);XPCHECK(g->job_n==0&&g->mission_receipt_n==1&&g->mission_receipts[0].amount==4000,"one of three survivors can be returned for a genuine partial rescue and proportional payment");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+21,g->system,12000);int cash=g->credits;abandon_mission(g,0);XPCHECK(g->credits==cash-2000+100,"abandoning a budget contract returns its unspent advance rather than minting credits");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+1,g->system,12000);xp_test_location(g);xp_test_action(g);xp_test_location(g);int freighter=g->expedition_runtime.npc[3];hit(g,freighter,10000,0);XPCHECK(g->job_n==0&&g->mission_receipt_n==0,"destruction of the guarded freighter fails its protection contract without paying");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+17,g->system,12000);xp_test_location(g);int count=g->expeditions[0].progress;g->cargo[0]=cargo_capacity(g);pod=g->expedition_runtime.pod[0];g->pos=g->debris[pod].pos;for(int shot=0;shot<40&&g->debris[pod].rock;shot++)mine_rock(g,DEBRIS_ID_MIN+pod);salvage_collect(g,DEBRIS_ID_MIN+pod);XPCHECK(g->expeditions[0].progress==(unsigned)count,"auto-selling ore from a full hold does not claim delivery provenance");
 XPCHECK(g->debris[pod].alive,"a full hold leaves the marked mission canister recoverable instead of auto-selling it");
 for(int branch=0;branch<4;branch++){
  int index=branch==0?14:branch==1?23:branch==2?37:39;
  xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+index,g->system,12000);
  for(int n=0;n<8&&g->job_n&&expedition_stage(g,0)->kind!=XP_CHOICE;n++){xp_test_location(g);if(!xp_test_action(g))break;}
  int valid=g->job_n&&expedition_stage(g,0)->kind==XP_CHOICE;
  if(valid)valid=expedition_choice(g,0,1);
  for(int n=0;n<8&&g->job_n&&branch!=3;n++){xp_test_location(g);if(!xp_test_action(g))break;}
  XPCHECK(valid&&g->job_n==0&&(branch==3?!g->mission_receipt_n:g->mission_receipt_n==1),"decline, diversion return, original passenger trip and championship withdrawal have real distinct outcomes");
 }
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+24,expedition_destination(g,EXPEDITION_FIRST+24),12000);xp_test_location(g);
 int rescan=-1;for(int k=0;k<LIFE_COUNT;k++)if(g->life[k].kind==LIFE_FLORA){rescan=k;break;}int rescan_ok=0;
 if(rescan>=0){g->life[rescan].scanned=1;g->pos=g->life[rescan].pos;int oldcash=g->credits;rescan_ok=survey_scan_target(g,rescan)&&g->expeditions[0].progress==1&&g->credits==oldcash;survey_scan_target(g,rescan);rescan_ok&=g->expeditions[0].progress==1;}XPCHECK(rescan_ok,"previously logged flora can be re-observed once for this contract without duplicate research payouts");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+38,g->system,12000);xp_test_location(g);xp_test_action(g);xp_test_location(g);xp_test_action(g);g->expeditions[0].elapsed=181;docking_complete(g);XPCHECK(g->job_n==0&&!g->mission_receipt_n,"missing the real timed delivery deadline fails the courier trial without payment");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST+14,g->system,12000);xp_test_location(g);for(int n=0;n<4;n++){xp_test_action(g);if(g->job_n)xp_test_location(g);}XPCHECK(g->job_n==1&&expedition_stage(g,0)->kind==XP_CHOICE&&g->mission_receipt_n==0,"the supplier investigation cannot finish merely by docking before its decision");
 xp_test_fixture(g);uint64_t newtypes=0;uint32_t oldtypes=0;int boards=1;
 for(int system=0;system<256;system++){g->system=system;for(int hub=0;hub<HUB_COUNT;hub++){g->station_variant=hub;int offered[5];for(int offer=0;offer<5;offer++){int type=mission_type_at(g,system,hub,offer);offered[offer]=type;if(type>=EXPEDITION_FIRST)newtypes|=1ull<<(type-EXPEDITION_FIRST);else oldtypes|=1u<<type;for(int k=0;k<offer;k++)boards&=offered[k]!=type;}}}
 XPCHECK(newtypes==((1ull<<40)-1)&&oldtypes==((1u<<MISSION_RACE)-1),"actual station boards offer all forty new contracts and preserve all twenty-six existing families");
 XPCHECK(boards,"each station board retains five distinct contract families plus its local race");
 xp_test_fixture(g);int v32=save_game(g,"expedition-test.sav");
 if(v32){FILE *source=fopen("expedition-test.sav","rb");unsigned char *bytes=malloc(131072);size_t size=source&&bytes?fread(bytes,1,131072,source):0;if(source)fclose(source);v32=size>EXPEDITION_SAVE_BYTES+4;if(v32){bytes[4]=32;size_t module_end=size-4-(ERL_SAVE_BYTES+DB_SAVE_BYTES)-EXPEDITION_SAVE_BYTES-NEWSPAPER_REWARD_SAVE_BYTES-RUMOUR_SAVE_BYTES;memmove(bytes+module_end-2,bytes+module_end,size-module_end);size_t legacy_size=size-EXPEDITION_SAVE_BYTES-(ERL_SAVE_BYTES+DB_SAVE_BYTES)-6;FILE *old=fopen("expedition-v32.sav","wb");v32=old&&fwrite(bytes,1,legacy_size,old)==legacy_size;if(old)fclose(old);if(v32)v32=save_seal("expedition-v32.sav")&&load_game(loaded,"expedition-v32.sav")&&loaded->credits==g->credits&&loaded->job_n==0;}free(bytes);}
 XPCHECK(v32,"a correctly sealed V32 commander imports unchanged without invented staged missions");remove("expedition-v32.sav");
 xp_test_fixture(g);expedition_start(g,EXPEDITION_FIRST,g->system,12000);expedition_ack(g,0);g->docked=0;g->pos=(Vec3){0,0,0};
 for(int k=0;k<NPC_COUNT;k++){g->npc[k].alive=1;g->npc[k].role=k<36?TRADERS:LAW;g->npc[k].bounty_slot=g->npc[k].traveller=-1;g->npc[k].pos=g->pos;}
 expedition_tick(g,.01f);int wait_ok=!g->expedition_runtime.built&&g->job_n==1;g->pos=(Vec3){100000,0,0};expedition_tick(g,.01f);
 int law_safe=1;for(int k=36;k<NPC_COUNT;k++)law_safe&=g->npc[k].alive&&g->npc[k].role==LAW;
 XPCHECK(wait_ok&&g->expedition_runtime.built&&g->expedition_runtime.npc[0]>=0&&law_safe,"saturated traffic waits safely, then constructs a real contact without taking Law's reserved ships");
 remove("expedition-test.sav");remove("expedition-test.sav.bak");remove("expedition-test.sav.tmp");
 free(g);free(loaded);fprintf(out,"RESULT %d failures\n",failures);fclose(out);
 #undef XPCHECK
 return failures;
}
