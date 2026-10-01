/* Small deterministic state machines, not perpetual sine-wave translation. */
static unsigned fauna_random(Lifeform *l){l->behaviour_rng=field_hash(l->behaviour_rng+0x9e3779b9u);return l->behaviour_rng;}
static int fauna_family(const Game *g,int slot){return (field_species_seed(g->system,g->planet,slot)>>8)%8;}
static int fauna_flier(int family){return family==0||family==3||family==5;}
static float fauna_distance(Vec3 a,Vec3 b){float x=a.x-b.x,z=a.z-b.z;return sqrtf(x*x+z*z);}
static void fauna_state(Lifeform *l,int state,float duration){l->behaviour=state;l->state_time=duration;}
static int fauna_clear(const Game *g,float x,float z){return eva_position_allowed(g,x,z);}
static void fauna_init(Game *g,int slot){
 Lifeform *l=&g->life[slot];l->behaviour_rng=field_species_seed(g->system,g->planet,slot);l->behaviour_ready=1;
 Vec3 pad=surface_site(g,1);int safe=fauna_clear(g,l->pos.x,l->pos.z);
 for(int k=0;k<32&&!safe;k++){unsigned r=fauna_random(l);float a=(r%6283)*.001f,d=g->system==7&&g->planet==3?160+(r>>12)%70:290+(r>>12)%210;l->pos.x=pad.x+sinf(a)*d;l->pos.z=pad.z+cosf(a)*d;safe=fauna_clear(g,l->pos.x,l->pos.z);}
 l->home=l->goal=l->pos;l->heading=(fauna_random(l)%6283)*.001f;l->speed=l->phase=l->calm_time=l->lift=0;
 fauna_state(l,FAUNA_IDLE,1+(fauna_random(l)%400)*.01f);l->pos.y=terrain_height(g,l->pos.x,l->pos.z)+16;
}
static int fauna_goal(Game *g,Lifeform *l,int flee){
 for(int attempt=0;attempt<12;attempt++){
  unsigned r=fauna_random(l);float a=(r%6283)*.001f,d=35+(r>>12)%120;
  if(flee){a=atan2f(l->pos.x-g->pos.x,l->pos.z-g->pos.z)+((int)(r%101)-50)*.015f;d=140+(r>>12)%70;}
  Vec3 p={l->pos.x+sinf(a)*d,0,l->pos.z+cosf(a)*d};
  if(fauna_distance(p,l->home)>360){if(flee)continue;p=l->home;}
  int clear=fauna_clear(g,p.x,p.z);
  /* A clear destination behind a tree is not a clear route. Bounded short rays
     keep grazing and fleeing from repeatedly walking into the same obstacle. */
  int steps=(int)(fauna_distance(p,l->pos)/12)+1;
  for(int n=1;n<steps&&clear;n++){float t=n/(float)steps;clear=fauna_clear(g,l->pos.x+(p.x-l->pos.x)*t,l->pos.z+(p.z-l->pos.z)*t);}
  if(clear){l->goal=p;return 1;}
 }
 return 0;
}
static void fauna_tick(Game *g,float dt){
 if(dt<=0||g->planet<1)return;
 dt=fminf(dt,.10f);
 for(int slot=0;slot<LIFE_COUNT;slot++){
  Lifeform *l=&g->life[slot];if(!l->alive||l->kind!=LIFE_FAUNA)continue;
  if(!l->behaviour_ready)fauna_init(g,slot);
  int family=fauna_family(g,slot),flying=fauna_flier(family);float distance=fauna_distance(l->pos,g->pos);
  float scare=g->rover_driving?105:g->eva_running?75:36;
  l->state_time-=dt;l->calm_time=fmaxf(0,l->calm_time-dt);
  if(distance<scare&&l->behaviour!=FAUNA_FLEE&&l->behaviour!=FAUNA_ALERT&&l->calm_time<=0){fauna_state(l,FAUNA_ALERT,.65f);l->speed=0;}
  if(l->behaviour==FAUNA_ALERT){
   l->heading=atan2f(g->pos.x-l->pos.x,g->pos.z-l->pos.z);
   if(l->state_time<=0){if(fauna_goal(g,l,1))fauna_state(l,FAUNA_FLEE,3.5f);else {fauna_state(l,FAUNA_IDLE,2);l->calm_time=3;}}
  }else if(l->state_time<=0){
   if(l->behaviour==FAUNA_FLEE){l->calm_time=6;fauna_state(l,FAUNA_IDLE,2);}
   else if(l->behaviour==FAUNA_WANDER||l->behaviour==FAUNA_RETURN)fauna_state(l,FAUNA_FEED,3+(fauna_random(l)%300)*.01f);
   else if(fauna_distance(l->pos,l->home)>180&&fauna_clear(g,l->home.x,l->home.z)){l->goal=l->home;fauna_state(l,FAUNA_RETURN,12);}
   else if(fauna_goal(g,l,0))fauna_state(l,FAUNA_WANDER,7+(fauna_random(l)%400)*.01f);
   else fauna_state(l,FAUNA_IDLE,2);
  }
  int moving=l->behaviour==FAUNA_WANDER||l->behaviour==FAUNA_FLEE||l->behaviour==FAUNA_RETURN;
  float target_speed=0;
  if(moving){
   float dx=l->goal.x-l->pos.x,dz=l->goal.z-l->pos.z,remaining=sqrtf(dx*dx+dz*dz);
   if(remaining<7){fauna_state(l,FAUNA_FEED,3);moving=0;}
   else {
    float desired=atan2f(dx,dz),delta=atan2f(sinf(desired-l->heading),cosf(desired-l->heading));l->heading+=fmaxf(-dt*3.5f,fminf(dt*3.5f,delta));
    static const float pace[]={16,21,18,19,10,23,14,22};target_speed=pace[family]*(l->behaviour==FAUNA_FLEE?2.1f:1);
    if(fabsf(delta)>1.3f)target_speed=0; /* Turn before stepping towards the threat. */
   }
  }
  l->speed+=(target_speed-l->speed)*fminf(1,dt*7);if(!moving)l->speed=0;
  Vec3 before=l->pos;
  if(l->speed>.1f){
   float step=l->speed*dt,nx=l->pos.x+sinf(l->heading)*step,nz=l->pos.z+cosf(l->heading)*step;
   int clear=fauna_clear(g,nx,nz);
   for(int j=0;j<LIFE_COUNT&&clear;j++)if(j!=slot&&g->life[j].alive&&g->life[j].kind==LIFE_FAUNA&&fauna_distance((Vec3){nx,0,nz},g->life[j].pos)<13)clear=0;
   if(clear){l->pos.x=nx;l->pos.z=nz;}
   else {l->speed=0;if(!fauna_goal(g,l,l->behaviour==FAUNA_FLEE))fauna_state(l,FAUNA_IDLE,1.5f);}
  }
  float travelled=fauna_distance(before,l->pos);
  if(flying)l->phase=fmodf(l->phase+dt*(family==0?4.f:1.7f),1.f);
  else if(travelled>.001f)l->phase=fmodf(l->phase+travelled/(family==4?12.f:24.f),1.f);
  float lift=flying&&moving?12.f+2*sinf(l->phase*6.2831853f):family==3?5.f+1.5f*sinf(l->phase*6.2831853f):0;
  if((family==2||family==7)&&travelled>.001f)lift=5*fmaxf(0,sinf(l->phase*6.2831853f));
  l->lift+=(lift-l->lift)*fminf(1,dt*10);
  l->pos.y=terrain_height(g,l->pos.x,l->pos.z)+16+l->lift;
 }
}
