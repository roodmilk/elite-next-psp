/* Eight-second station launch; retain the authored five-second first ascent. Continuous speed and its analytic integral
 * avoid frame-rate dependent travel. Stay axial inside the hull; bend only after clearance. */
void launch_departure(Game *g){
 if(!g->docked)return;
 social_emit(g,SB_LAUNCH);int hub=g->station_variant;Vec3 center=hub_position(g,hub);
 launch(g);g->station_variant=hub;g->dock_from=center;
 g->dock_to=hub?norm(sub(g->bodies[0].pos,center)):(Vec3){0,0,-1};
 g->pos=sub(center,mul(g->dock_to,110));
 g->yaw=atan2f(g->dock_to.x,g->dock_to.z);g->pitch=asinf(g->dock_to.y);g->roll=0;
 g->departure_glow=7;g->dock_stage=4;g->dock_phase=0;g->dock_timer=0;g->dock_duration=g->opening.stage==OP_LAUNCH?5:8;
 g->speed=40;g->boost=0;g->voice_time=0;g->message_time=0;g->cue=SFX_DOCK;
}
static void departure_tick(Game *g,float dt){
 float duration=g->dock_duration>=5?g->dock_duration:8,previous=g->dock_timer;g->dock_timer=fminf(duration,g->dock_timer+dt);
 /* Slow the interesting port section, preserving the original axial endpoint
    and 40/100 m/s endpoint velocities. This is a time change, not a teleport. */
 float u=g->dock_timer/duration,t=5*u+(duration-5)/6.283185307f*sinf(6.283185307f*u),rate=5/duration+(1-5/duration)*cosf(6.283185307f*u),distance,velocity;
 if(previous<(duration<6?2:3)&&g->dock_timer>=(duration<6?2:3))g->cue=SFX_BOOST;
 if(t<2){velocity=40+65*t*t;distance=40*t+(65.f/3)*t*t*t;}
 else if(t<3){float v=t-2;velocity=300+600*v;distance=253.333333f+300*v+300*v*v;}
 else {float v=(t-3)*.5f;velocity=900-800*v*v*(3-2*v);distance=853.333333f+1800*v-1600*(v*v*v-.5f*v*v*v*v);}
 float travelled=distance-110,half=station_half_for(g,g->station_variant),span=fmaxf(200,1743.333333f-half-100),bend=fmaxf(0,fminf(1,(travelled-half-100)/span));
 float amplitude=(duration<6?0:g->station_variant?30:station_class(g)==STATION_MEGA?100:70)*(g->system&1?1:-1),offset=amplitude*bend*bend*(3-2*bend),slope=amplitude*6*bend*(1-bend)/span;
 Vec3 right=norm((Vec3){g->dock_to.z,0,-g->dock_to.x}),direction=norm(add(g->dock_to,mul(right,slope)));
 g->pos=add(add(g->dock_from,mul(g->dock_to,travelled)),mul(right,offset));g->yaw=atan2f(direction.x,direction.z);g->pitch=asinf(fmaxf(-1,fminf(1,direction.y)));g->roll=-.035f*sinf(3.141592654f*bend)*(amplitude>0?1:-1);
 g->speed=velocity*rate*sqrtf(1+slope*slope);
 g->boost=0;
 if(g->dock_timer>=duration){int jump=g->dock_phase;g->dock_stage=g->dock_phase=0;g->speed=100;g->police_grace=fmaxf(g->police_grace,4);message(g,"Departure complete. You have control.");if(jump)jump_start(g);}
}
