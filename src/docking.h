/* Shared station geometry, collision and arrival state machine. */
float station_angle(const Game *g){return fmodf(g->time*station_profile_for(g,0).spin,6.2831853f);}
const char *station_name_for(const Game *g,int hub){static char name[40];const char *kind;if(hub==1)kind="Outer Relay";else if(hub==2)kind="Frontier Outpost";else kind=station_class(g)==STATION_MEGA?"Mega Capital":station_class(g)==STATION_POOR?"Free Port":"Orbital Citadel";snprintf(name,sizeof(name),"%s %s",g->systems[g->system].name,kind);return name;}
const char *station_name(const Game *g){return station_name_for(g,g->station_variant);}
static Vec3 station_local(const Game *g,Vec3 p){p.z-=STATION_Z;float a=station_angle(g),c=cosf(a),s=sinf(a);return (Vec3){p.x*c+p.y*s,-p.x*s+p.y*c,p.z};}
static Vec3 station_port_world(const Game *g,int port,float local_z){Vec3 o=station_port_offset_for(g,0,port);float a=station_angle(g),c=cosf(a),s=sinf(a);return (Vec3){o.x*c-o.y*s,o.x*s+o.y*c,STATION_Z+local_z};}
int station_nearest_port(const Game *g,Vec3 world){if(station_port_count_for(g,0)==1)return 0;Vec3 p=station_local(g,world);if(fabsf(p.x)>fabsf(p.y))return p.x<0?1:2;return p.y<0?3:4;}
static int station_collision(Game *g,Vec3 previous);
static int station_intersection(const Game *g,Vec3 from,Vec3 to,Vec3 *impact){
 {Vec3 a=station_local(g,from),b=station_local(g,to);float t;
  if(!station_architecture_hit(g,a,b,0,&t))return 0;*impact=add(a,mul(sub(b,a),t));return 1;}
}
static void docking_complete(Game *g){social_emit(g,SB_DOCK);guild_event(g,GUILD_DOCK);g->dock_stage=0;g->docked=1;g->speed=0;g->energy=100;g->heat=0;g->pos=(Vec3){0,0,3500};if(g->passenger_dest==g->system){g->credits+=g->passenger_pay>0?g->passenger_pay:1200;g->passenger_dest=-1;g->passenger_kind=0;g->passenger_pay=0;message(g,"Passenger delivered. Fare paid.");speak(g,VOICE_CONTACT,"This is my stop. Thanks for the ride.");}jobs_from_legacy(g);int done=0,need_rescue=0,need_cargo=0;for(int i=0;i<g->job_n;){Job *j=&g->jobs[i];if(j->dest==g->system&&(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING||(j->type==MISSION_RESCUE&&j->stage))){if(j->type==MISSION_DELIVERY&&g->cargo[0]<1){need_cargo=1;i++;continue;}if(j->type==MISSION_SMUGGLING&&g->cargo[6]<1){need_cargo=1;i++;continue;}if(j->type==MISSION_DELIVERY)g->cargo[0]--;else if(j->type==MISSION_SMUGGLING)g->cargo[6]--;g->job_sel=i;mission_finish_slot(g,i,"Mission complete. Payment received.");done++;}else {if(j->dest==g->system&&j->type==MISSION_RESCUE&&!j->stage)need_rescue=1;i++;}}if(done&&need_rescue)message(g,"Paid. Rescue is still out in this system.");else if(done&&need_cargo)message(g,"Paid. A crate is still in your hold.");else if(!done){message(g,need_rescue?"Rescue target is not yet aboard.":need_cargo?"Delivery is still in your hold. Don't sell it.":"Docked. Station services are open.");speak(g,VOICE_DOCK,need_rescue?"Rescue is not aboard. Go back out.":need_cargo?"Unload the crate, not the empty ship.":"Berth locked. Don't scratch the paint.");}saga_dock_event(g);g->cue=SFX_DOCK;story_event(g,STORY_EV_DOCK);}
#include "mega-city-guidance.h"
static void docking_leg(Game *g){StationProfile profile=station_profile_for(g,0);float clear=station_architecture_clearance(g),side=g->dock_from.x<0?-1:1;int port=station_nearest_port(g,g->dock_from);Vec3 entry=station_port_world(g,port,-profile.half+2),approach=station_port_world(g,port,-profile.half-650.f);
 if(g->dock_phase==0)g->dock_to=(Vec3){side*clear,g->pos.y,g->pos.z};
 else if(g->dock_phase==1)g->dock_to=(Vec3){side*clear,approach.y,approach.z};
 else if(g->dock_phase==2)g->dock_to=approach;
 else g->dock_to=entry;
 g->dock_from=g->pos;g->dock_duration=fmaxf(.6f,length(sub(g->dock_to,g->pos))/(g->dock_phase==3?180:850));g->dock_timer=0;
}
int dock_hub(Game *g,int hub){if(hub<0||hub>=HUB_COUNT)return 0;if(g->docked)return 1;if(g->planet>=0){message(g,"Return to orbit before docking.");return 0;}if(g->dead||g->jump>0||g->police_stop||g->dock_stage)return 0;
Vec3 hc=hub_position(g,hub);float range=station_comms_range(g,hub);if(length(sub(g->pos,hc))>range){char range_note[80];snprintf(range_note,sizeof(range_note),"Selected station out of range. Move within %d m.",(int)range);message(g,range_note);return 0;}if(g->legal>0){police_begin(g,0);message(g,"LOCAL LAW INTERCEPT: settle your warrant before docking.");speak(g,VOICE_LAW,"Hold position. Your warrant must be settled before this station will accept you.");return 1;}if(hub>0){g->station_variant=hub;g->dock_stage=2;g->dock_timer=0;g->speed=0;g->boost=0;g->pos=hc;g->energy=100;g->heat=0;message(g,"Approaching relay berth. Guidance has control.");speak(g,VOICE_DOCK,"Relay approach confirmed. Bringing you in safely.");return 1;}
 Vec3 inside;if(station_intersection(g,g->pos,g->pos,&inside)){message(g,"Inside station hull. Guidance unavailable.");return 0;}
 return mega_guidance_start(g);
 g->dock_stage=1;g->dock_phase=g->pos.z<station_entry_z_for(g,0)-300?2:0;g->dock_from=g->pos;g->speed=0;g->boost=0;g->approach=-1;docking_leg(g);message(g,station_class(g)==STATION_MEGA?"Mega Capital guidance selected the nearest entrance.":"Docking cleared. Guidance has control.");campaign_event(g,CP_GUIDANCE);return 1;
}
int dock(Game *g){return dock_hub(g,nearest_hub(g));}
static void docking_tick(Game *g,float dt){g->dock_timer+=dt;g->speed=0;g->boost=0;
 if(g->dock_stage==1&&g->dock_phase==10){mega_guidance_tick(g,dt);return;}
 if(g->dock_stage==1){Vec3 previous=g->pos;float t=fminf(1,g->dock_timer/g->dock_duration);t=t*t*(3-2*t);g->pos=add(g->dock_from,mul(sub(g->dock_to,g->dock_from),t));Vec3 aim=norm(sub((Vec3){0,0,STATION_Z},g->pos));g->yaw=atan2f(aim.x,aim.z);g->pitch=asinf(fmaxf(-1,fminf(1,aim.y)));g->roll=station_angle(g);
  /* Guidance must cross the same aperture as a manually flown ship. */
  if(station_collision(g,previous)){
   /* Guidance owns the ship: a rotating-frame seam must not cancel the
    * Circle request or skip the third-person docking sequence. */
   if(g->dead){g->dead=0;g->energy=100;g->explosion=0;g->jump=0;g->pos=(Vec3){0,0,station_entry_z_for(g,0)};g->speed=0;g->boost=0;g->dock_stage=2;g->dock_timer=0;message(g,"Entry confirmed. Docking in progress.");}
   return;
  }
  if(g->dock_timer>=g->dock_duration){if(++g->dock_phase>3){g->dock_stage=0;message(g,"Guidance stopped. Request docking again.");}else docking_leg(g);}}
 else if(g->dock_stage==2&&g->dock_timer>=3){g->dock_stage=3;g->dock_timer=0;}
 else if(g->dock_stage==3&&g->dock_timer>=1.4f){docking_complete(g);campaign_event(g,CP_RETURN);}
}
static int station_collision(Game *g,Vec3 previous){Vec3 hit;if(!station_intersection(g,previous,g->pos,&hit))return 0;
 float roll=fabsf(sinf(g->roll-station_angle(g)));
 Vec3 start=station_local(g,previous);float half=station_half_for(g,0);
 int port=-1;for(int i=0;i<station_port_count_for(g,0);i++){Vec3 o=station_port_offset_for(g,0,i);if(fabsf(hit.x-o.x)<=STATION_PORT_HALF_W-STATION_SHIP_HALF_W&&fabsf(hit.y-o.y)<=STATION_PORT_HALF_H-STATION_SHIP_HALF_H){port=i;break;}}
 if(start.z < -half&&g->pos.z>previous.z&&hit.z<=-half+.01f&&port>=0&&forward(g).z>.9f&&roll<.30f&&g->speed<=200&&!g->boost){g->dock_stage=2;g->dock_timer=0;g->pos=station_port_world(g,port,-half);g->speed=0;g->boost=0;message(g,"Entry confirmed. Docking in progress.");return 1;}
 float a=station_angle(g);g->pos=(Vec3){hit.x*cosf(a)-hit.y*sinf(a),hit.x*sinf(a)+hit.y*cosf(a),hit.z+3500};g->pos=add(g->pos,mul(norm(sub(previous,g->pos)),5));g->dead=1;g->energy=0;g->explosion=0;g->jump=0;g->speed=0;g->boost=0;g->cue=SFX_DEATH;snprintf(g->collide,sizeof(g->collide),"%s",station_name(g));message(g,"Station hull impact. Ship destroyed.");return 1;
}
