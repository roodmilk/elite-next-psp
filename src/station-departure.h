/* Five-second first-person launch. Continuous speed and its analytic integral
 * avoid frame-rate dependent travel and keep the ship on the clear port axis. */
void launch_departure(Game *g){
 if(!g->docked)return;
 social_emit(g,SB_LAUNCH);int hub=g->station_variant;Vec3 center=hub_position(g,hub);
 launch(g);g->station_variant=hub;g->dock_from=center;
 g->dock_to=hub?norm(sub(g->bodies[0].pos,center)):(Vec3){0,0,-1};
 g->pos=sub(center,mul(g->dock_to,110));
 g->yaw=atan2f(g->dock_to.x,g->dock_to.z);g->pitch=asinf(g->dock_to.y);g->roll=0;
 g->departure_glow=7;g->dock_stage=4;g->dock_phase=0;g->dock_timer=0;g->dock_duration=5;
 g->speed=40;g->boost=0;g->voice_time=0;g->message_time=0;g->cue=SFX_DOCK;
}
static void departure_tick(Game *g,float dt){
 float previous=g->dock_timer;g->dock_timer=fminf(5,g->dock_timer+dt);
 if(previous<2&&g->dock_timer>=2)g->cue=SFX_BOOST;float t=g->dock_timer,distance;
 if(t<2){g->speed=40+65*t*t;distance=40*t+(65.f/3)*t*t*t;}
 else if(t<3){float u=t-2;g->speed=300+600*u;distance=253.333333f+300*u+300*u*u;}
 else {float u=(t-3)*.5f;g->speed=900-800*u*u*(3-2*u);distance=853.333333f+1800*u-1600*(u*u*u-.5f*u*u*u*u);}
 g->pos=add(g->dock_from,mul(g->dock_to,distance-110));
 g->boost=0;
 if(t>=5){int jump=g->dock_phase;g->dock_stage=g->dock_phase=0;g->speed=100;g->police_grace=fmaxf(g->police_grace,4);message(g,"Departure complete. You have control.");if(jump)jump_start(g);}
}
