/* Opt-in native tests; no save or random-gameplay state is touched. */
int fauna_tests(const char *path){
 FILE *f=fopen(path,"w");if(!f)return 1;int failures=0;
 #define FCHECK(ok,name) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",name);failures+=!pass;}while(0)
 Game *g=calloc(1,sizeof(Game)),*copy=calloc(1,sizeof(Game));
 if(!g||!copy){free(g);free(copy);fclose(f);return 1;}
 game_init(g);g->system=7;system_bodies(g);g->docked=0;g->approach=1;enter_planet(g);g->surface=2;
 int slot=-1;for(int i=0;i<LIFE_COUNT;i++)if(g->life[i].kind==LIFE_FAUNA&&!fauna_flier(fauna_family(g,i)))slot=i;
 FCHECK(slot>=0,"Lave I contains a grounded animal");
 if(slot>=0){
  Lifeform *l=&g->life[slot];for(int i=0;i<LIFE_COUNT;i++)if(i!=slot)g->life[i].alive=0;
  Vec3 start=l->pos;g->pos=add(start,(Vec3){500,22,0});l->phase=.2f;fauna_state(l,FAUNA_FEED,5);
  unsigned rng=g->rng;for(int i=0;i<30;i++)fauna_tick(g,.05f);
  FCHECK(fauna_distance(start,l->pos)<.001f&&fabsf(l->phase-.2f)<.001f,"feeding stops translation and grounded gait");
  int goal=fauna_goal(g,l,0);fauna_state(l,FAUNA_WANDER,10);l->heading=atan2f(l->goal.x-l->pos.x,l->goal.z-l->pos.z);
  *copy=*g;for(int i=0;i<50;i++){fauna_tick(g,.05f);fauna_tick(copy,.05f);}
  FCHECK(goal&&fauna_distance(start,l->pos)>5&&fabsf(l->phase-.2f)>.001f,"walking advances position and actual-distance gait");
  FCHECK(memcmp(g->life,copy->life,sizeof(g->life))==0&&g->rng==rng,"deterministic behaviour leaves gameplay RNG untouched");
  g->pos=add(l->pos,(Vec3){10,22,0});fauna_tick(g,.05f);FCHECK(l->behaviour==FAUNA_ALERT&&l->speed==0,"nearby walker makes animal stop and notice");
  for(int i=0;i<15;i++)fauna_tick(g,.05f);FCHECK(l->behaviour==FAUNA_FLEE,"alert changes into a flee decision");
  float before=fauna_distance(l->pos,g->pos);for(int i=0;i<30;i++)fauna_tick(g,.05f);
  fprintf(f,"Flee distance %.2f -> %.2f, state %d speed %.2f\n",before,fauna_distance(l->pos,g->pos),l->behaviour,l->speed);
  FCHECK(fauna_distance(l->pos,g->pos)>before+5,"startled animal moves away from player");
  g->pos=add(l->pos,(Vec3){500,22,0});for(int i=0;i<140;i++)fauna_tick(g,.05f);
  FCHECK(l->behaviour!=FAUNA_FLEE&&l->behaviour!=FAUNA_ALERT,"animal settles after player leaves");
  fauna_state(l,FAUNA_IDLE,5);l->calm_time=0;g->pos=add(l->pos,(Vec3){60,22,0});g->eva_running=0;fauna_tick(g,.05f);
  FCHECK(l->behaviour==FAUNA_IDLE,"walking at 60m does not startle wildlife");g->eva_running=1;fauna_tick(g,.05f);
  FCHECK(l->behaviour==FAUNA_ALERT,"running alerts wildlife farther away");
  fauna_state(l,FAUNA_IDLE,5);g->eva_running=0;g->rover_driving=1;g->pos=add(l->pos,(Vec3){95,22,0});fauna_tick(g,.05f);
  FCHECK(l->behaviour==FAUNA_ALERT,"rover alerts wildlife at 95m");g->rover_driving=0;
  g->pos=add(l->pos,(Vec3){500,22,0});int invalid=0;for(int i=0;i<2400;i++){fauna_tick(g,.05f);if(!fauna_clear(g,l->pos.x,l->pos.z)||!isfinite(l->pos.y))invalid++;}
  FCHECK(invalid==0,"two simulated minutes stay on safe finite terrain");
  FCHECK(fauna_distance(l->pos,l->home)<365,"wandering stays near its home range");
  /* Occupied next step must stop its legs as well as its position. */
  int other=(slot+1)%LIFE_COUNT;g->life[other]=*l;g->life[other].alive=1;g->life[other].kind=LIFE_FAUNA;
  l->goal=add(l->pos,(Vec3){0,0,80});l->heading=0;l->speed=14;l->phase=.3f;fauna_state(l,FAUNA_WANDER,5);start=l->pos;
  fauna_tick(g,.05f);FCHECK(fauna_distance(start,l->pos)<.001f&&l->speed==0&&fabsf(l->phase-.3f)<.001f,"blocked grounded animal stops moving and animating its gait");
 }
 int bad=0,count=0;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  g->system=sys;system_bodies(g);g->planet=-1;g->docked=g->dead=g->dock_stage=g->police_stop=0;g->jump=0;g->approach=body;
  if(!enter_planet(g)){bad++;continue;}
  for(int i=0;i<LIFE_COUNT;i++)if(g->life[i].kind==LIFE_FAUNA){count++;Lifeform *l=&g->life[i];if(!l->behaviour_ready||!fauna_clear(g,l->pos.x,l->pos.z)||!isfinite(l->pos.y))bad++;}
 }
 fprintf(f,"Spawn catalogue: %d animals, %d unsafe\n",count,bad);FCHECK(count>0&&bad==0,"safe animal spawns across every current system and planet");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);free(copy);free(g);return failures;
 #undef FCHECK
}
