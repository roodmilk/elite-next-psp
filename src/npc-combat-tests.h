static NPC *npc_combat_fixture(Game *g){
 game_init(g);story_complete(g);g->opening.stage=0;launch(g);
 g->dock_stage=0;g->speed=0;g->pos=(Vec3){0,250000,0};g->yaw=g->pitch=g->roll=0;
 g->freight_next=100000;g->encounter=100000;g->legal=0;g->police_grace=100000;
 for(int i=0;i<NPC_COUNT;i++){g->npc[i].alive=0;g->npc[i].cooldown=100000;}
 NPC *n=&g->npc[BOUNTY_NPC_FIRST];n->alive=1;n->freighter=0;n->role=PIRATES;n->bounty_slot=0;
 n->pos=add(g->pos,(Vec3){0,0,1000});n->dir=(Vec3){0,0,1};n->radius=30;n->cruise=650;
 n->health=npc_max_health(g,n);n->shield=npc_max_shield(g,n);n->cooldown=0;
 n->target=-1;n->player_tag=n->escape_time=n->route_wait=0;n->route_task=TRAFFIC_CRUISE;
 return n;
}
int npc_combat_tests(const char *path){
 FILE *f=fopen(path,"w");if(!f)return 1;int failures=0;
 Game *g=malloc(sizeof(*g));if(!g){fprintf(f,"RESULT 1 failures\n");fclose(f);return 1;}
#define NCHECK(ok,label) do{int pass=!!(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 NPC *n=npc_combat_fixture(g);
 game_tick(g,.016f,0,0,0,0);
 NCHECK(n->target==-1&&n->player_tag==0,"untouched Lave poster raider does not attack the commander");
 n=npc_combat_fixture(g);g->fit[FIT_WPN]=1;fit_rebuild(g);n->route_wait=10;
 game_tick(g,.016f,0,0,0,1);
 NCHECK(n->player_tag>0&&n->target==-2&&n->route_wait==0,"real laser hit interrupts route and provokes wanted raider even in Lave");
 int fired=0,broke=0,returned=0,finite=1;
 for(int k=0;k<600;k++){
  game_tick(g,1.f/60,0,0,0,0);
  fired|=g->fire_bearing_time[BOUNTY_NPC_FIRST]>0;
  if(n->route_task==TRAFFIC_BREAK)broke=1;
  if(broke&&n->route_task==TRAFFIC_ATTACK)returned=1;
  finite&=isfinite(n->pos.x)&&isfinite(n->pos.y)&&isfinite(n->pos.z)&&fabsf(length(n->dir)-1)<.01f;
 }
 NCHECK(fired,"raider initially facing away turns around and returns actual weapons fire");
 NCHECK(broke&&returned,"close attack pass breaks away and returns for another run");
 NCHECK(finite&&n->target==-2,"staggered pursuit keeps finite normalized steering and its attacker selected");
 n=npc_combat_fixture(g);hit(g,BOUNTY_NPC_FIRST,1,1);n->pos=add(g->pos,(Vec3){0,0,6500});
 NPC *trader=&g->npc[0];*trader=*n;trader->role=TRADERS;trader->bounty_slot=-1;trader->pos=add(n->pos,(Vec3){900,0,0});trader->player_tag=0;trader->cooldown=100000;
 game_tick(g,.016f,0,0,0,0);
 NCHECK(n->target==-2,"engaged raider pursues beyond old 2.4km detection and ignores easier nearby trader");
 n=npc_combat_fixture(g);hit(g,BOUNTY_NPC_FIRST,1,1);
 NPC *law=&g->npc[0];*law=*n;law->role=LAW;law->bounty_slot=-1;law->player_tag=0;law->cooldown=100000;law->pos=add(n->pos,(Vec3){1200,0,0});
 game_tick(g,.016f,0,0,0,0);
 NCHECK(n->target==-2&&n->escape_time==0,"one distant patrol does not erase healthy raider retaliation");
 g->npc[1]=*law;g->npc[1].pos=add(n->pos,(Vec3){-1200,0,0});g->ai_phase=-1;
 game_tick(g,.016f,0,0,0,0);
 NCHECK(n->target==-1&&n->route_task==TRAFFIC_ESCAPE&&traffic_speed(n)>n->cruise*1.5f,"outnumbered raider deliberately boosts away from patrols");
 n=npc_combat_fixture(g);hit(g,BOUNTY_NPC_FIRST,1,1);n->health=20;n->shield=0;n->dir=(Vec3){0,0,1};
 float before=length(sub(n->pos,g->pos));int cash=g->credits;
 for(int k=0;k<90;k++)game_tick(g,1.f/60,0,0,0,0);
 NCHECK(n->alive&&n->target==-1&&n->route_task==TRAFFIC_ESCAPE&&length(sub(n->pos,g->pos))>before+500,"crippled raider flees from attacker at escape speed without despawning");
 NCHECK(!bounty_target_taken(g,0)&&g->credits==cash,"escape preserves wanted-poster identity and does not pay unearned bounty");
 n=npc_combat_fixture(g);hit(g,BOUNTY_NPC_FIRST,1,1);n->pos=add(g->pos,(Vec3){0,0,15000});g->ai_phase=-1;
 game_tick(g,.016f,0,0,0,0);
 NCHECK(n->target==-1,"raider cannot chase the player forever beyond combat sensor leash");
 n=npc_combat_fixture(g);hit(g,BOUNTY_NPC_FIRST,100000,0);
 NCHECK(n->alive&&!bounty_target_taken(g,0),"unengaged police cannot erase a poster target");
 hit(g,BOUNTY_NPC_FIRST,1,1);cash=g->credits;hit(g,BOUNTY_NPC_FIRST,100000,0);
 NCHECK(!n->alive&&bounty_target_taken(g,0)&&g->credits>cash,"police assistance still awards a recently engaged poster target");
 n=npc_combat_fixture(g);n->role=EXPLORERS;n->bounty_slot=-1;hit(g,BOUNTY_NPC_FIRST,1,1);g->ai_phase=-1;
 game_tick(g,.016f,0,0,0,0);
 NCHECK(n->target==-2&&g->legal>0,"attacked peaceful ship still defends itself and reports assault");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);free(g);
#undef NCHECK
 return failures;
}
