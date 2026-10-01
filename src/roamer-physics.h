/* Grounded inertial driving; shared EVA collision queries protect all biomes. */
static float rover_towards_zero(float v,float amount){return v>0?fmaxf(0,v-amount):fminf(0,v+amount);}
static void rover_step(Game *g,float dt,float steer,float look,int throttle){
 g->rover_ticks++;
 g->rover_crack=fmaxf(0,g->rover_crack-dt);g->rover_impact_cd=fmaxf(0,g->rover_impact_cd-dt);
 float speed=sqrtf(g->rover_velocity.x*g->rover_velocity.x+g->rover_velocity.z*g->rover_velocity.z);
 steer=fmaxf(-1,fminf(1,steer));
 /* Heading follows steering; momentum follows with traction, not a snap. */
 float travel=g->rover_velocity.x*sinf(g->yaw)+g->rover_velocity.z*cosf(g->yaw);
 float steer_rate=(.15f+fminf(1,speed/55.f))*1.55f/(1+speed/300.f);
 g->yaw+=steer*steer_rate*dt*(travel< -2?-1:1);
 g->pitch=fmaxf(-.55f,fminf(.55f,g->pitch+look*dt*1.2f));g->roll=0;
 float sy=sinf(g->yaw),cy=cosf(g->yaw);
 float longitudinal=g->rover_velocity.x*sy+g->rover_velocity.z*cy;
 float lateral=g->rover_velocity.x*cy-g->rover_velocity.z*sy;
 int boosted=g->rover_boost&&throttle>0&&!g->rover_brake&&g->rover_charge>0;
 if(boosted)g->rover_charge=fmaxf(0,g->rover_charge-28*dt);
 else if(!g->rover_boost)g->rover_charge=fminf(100,g->rover_charge+16*dt);
 if(g->rover_brake){
  if(longitudinal>2){longitudinal=fmaxf(0,longitudinal-150*dt);g->rover_reverse_wait=0;}
  else if(throttle<=0){
   g->rover_reverse_wait+=dt;
   if(g->rover_reverse_wait>.35f||longitudinal< -2)longitudinal=fmaxf(-55,longitudinal-55*dt);
   else longitudinal=rover_towards_zero(longitudinal,150*dt);
  }
  else longitudinal=rover_towards_zero(longitudinal,150*dt);
 }else if(throttle>0)longitudinal+=dt*(boosted?110:65);
 else if(throttle<0)longitudinal-=55*dt;
 else longitudinal=rover_towards_zero(longitudinal,dt*(10+speed*.035f));
 if(!g->rover_brake)g->rover_reverse_wait=0;
 float limit=boosted?260:fmaxf(190,speed-80*dt);
 longitudinal=fmaxf(-55,fminf(limit,longitudinal));
 if(g->rover_drift)longitudinal*=expf(-.65f*dt);
 lateral*=expf(-(g->rover_drift?.85f:7.f)*dt);
 g->rover_velocity=(Vec3){sy*longitudinal+cy*lateral,0,cy*longitudinal-sy*lateral};
 speed=length(g->rover_velocity);if(speed>280)g->rover_velocity=mul(g->rover_velocity,280/speed);
 Vec3 before=g->pos;float impact=0;
 int steps=(int)ceilf(fmaxf(1,length(g->rover_velocity)*dt/3.f));
 for(int i=0;i<steps;i++){
  float dx=g->rover_velocity.x*dt/steps,dz=g->rover_velocity.z*dt/steps;
  float nx=g->pos.x+dx,nz=g->pos.z+dz;
  if(eva_position_allowed(g,nx,nz)){g->pos.x=nx;g->pos.z=nz;continue;}
  int clearx=eva_position_allowed(g,nx,g->pos.z),clearz=eva_position_allowed(g,g->pos.x,nz);
  /* Two individually safe axes may still enter the same diagonal corner. */
  if(clearx&&clearz){if(fabsf(dx)>fabsf(dz))clearz=0;else clearx=0;}
  if(clearx)g->pos.x=nx;else {impact=fmaxf(impact,fabsf(g->rover_velocity.x));g->rover_velocity.x=0;}
  if(clearz)g->pos.z=nz;else {impact=fmaxf(impact,fabsf(g->rover_velocity.z));g->rover_velocity.z=0;}
 }
 if(impact>35&&g->rover_impact_cd<=0){
  g->rover_crack=fminf(2.4f,1.2f+impact*.005f);g->rover_impact_cd=.6f;g->cue=SFX_HIT;
 }
 g->pos.y=terrain_height(g,g->pos.x,g->pos.z)+22;g->rover_pos=g->pos;
 g->jetpack=g->boost=0;g->eva_running=g->eva_run_arm=g->eva_jump_held=0;
 g->hazard=fmaxf(0,g->hazard-dt*22);fauna_tick(g,dt);
 g->speed=sqrtf((g->pos.x-before.x)*(g->pos.x-before.x)+(g->pos.z-before.z)*(g->pos.z-before.z))/dt;
}
