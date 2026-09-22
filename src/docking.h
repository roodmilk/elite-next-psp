/* Shared station geometry, collision and arrival state machine. */
float station_angle(const Game *g){return fmodf(g->time*.025f,6.2831853f);}
const char *station_name(const Game *g){static char name[40];const char *kind;if(g->station_variant==1)kind="Outer Relay";else if(g->station_variant==2)kind="Frontier Outpost";else kind=g->system==7?"System Hub":prosperity(g,g->system)>=5?"Trade Citadel":prosperity(g,g->system)>=4?"Orbital Exchange":danger_rating(g,g->system)>=4?"Frontier Bastion":"System Port";snprintf(name,sizeof(name),"%s %s",g->systems[g->system].name,kind);return name;}
static Vec3 station_local(const Game *g,Vec3 p){p.z-=STATION_Z;float a=station_angle(g),c=cosf(a),s=sinf(a);return (Vec3){p.x*c+p.y*s,-p.x*s+p.y*c,p.z};}
static int station_collision(Game *g,Vec3 previous);
static int station_intersection(const Game *g,Vec3 from,Vec3 to,Vec3 *impact){
 Vec3 a=station_local(g,from),b=station_local(g,to),d=sub(b,a);float enter=0,leave=1;
 for(int i=0;i<14;i++){Vec3 n;float limit;if(i<6){n=(Vec3){0,0,0};float sign=i%2?1:-1;if(i/2==0)n.x=sign;else if(i/2==1)n.y=sign;else n.z=sign;limit=160;}else {int k=i-6;n=(Vec3){k&1?1:-1,k&2?1:-1,k&4?1:-1};limit=320;}
  float q=limit-dot(n,a),v=dot(n,d);if(fabsf(v)<.00001f){if(q<0)return 0;}else if(v>0)leave=fminf(leave,q/v);else enter=fmaxf(enter,q/v);if(enter>leave)return 0;
 }
 *impact=add(a,mul(d,enter));return 1;
}
static void docking_complete(Game *g){guild_event(g,GUILD_DOCK);g->dock_stage=0;g->docked=1;g->speed=0;g->energy=100;g->heat=0;g->pos=(Vec3){0,0,3500};if(g->passenger_dest==g->system){g->credits+=g->passenger_pay>0?g->passenger_pay:1200;g->passenger_dest=-1;g->passenger_kind=0;g->passenger_pay=0;message(g,"Passenger delivered. Fare paid.");speak(g,VOICE_CONTACT,"This is my stop. Thanks for the ride.");}jobs_from_legacy(g);int done=0,need_rescue=0,need_cargo=0;for(int i=0;i<g->job_n;){Job *j=&g->jobs[i];if(j->dest==g->system&&(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING||(j->type==MISSION_RESCUE&&j->stage))){if(j->type==MISSION_DELIVERY&&g->cargo[0]<1){need_cargo=1;i++;continue;}if(j->type==MISSION_SMUGGLING&&g->cargo[6]<1){need_cargo=1;i++;continue;}if(j->type==MISSION_DELIVERY)g->cargo[0]--;else if(j->type==MISSION_SMUGGLING)g->cargo[6]--;g->job_sel=i;mission_finish_slot(g,i,"Mission complete. Payment received.");done++;}else {if(j->dest==g->system&&j->type==MISSION_RESCUE&&!j->stage)need_rescue=1;i++;}}if(done&&need_rescue)message(g,"Paid. Rescue is still out in this system.");else if(done&&need_cargo)message(g,"Paid. A crate is still in your hold.");else if(!done){message(g,need_rescue?"Rescue target is not yet aboard.":need_cargo?"Delivery is still in your hold. Don't sell it.":"Docked. Station services are open.");speak(g,VOICE_DOCK,need_rescue?"Rescue is not aboard. Go back out.":need_cargo?"Unload the crate, not the empty ship.":"Berth locked. Don't scratch the paint.");}saga_dock_event(g);g->cue=SFX_DOCK;story_event(g,STORY_EV_DOCK);}
static void docking_leg(Game *g){float side=g->dock_from.x<0?-1:1;
 if(g->dock_phase==0)g->dock_to=(Vec3){side*900,g->pos.y,g->pos.z};
 else if(g->dock_phase==1)g->dock_to=(Vec3){side*900,0,2700};
 else if(g->dock_phase==2)g->dock_to=(Vec3){0,0,2700};
 else g->dock_to=(Vec3){0,0,STATION_ENTRY_Z+2};
 g->dock_from=g->pos;g->dock_duration=fmaxf(.6f,length(sub(g->dock_to,g->pos))/(g->dock_phase==3?180:850));g->dock_timer=0;
}
int dock(Game *g){if(g->docked)return 1;if(g->planet>=0){message(g,"Return to orbit before docking.");return 0;}if(g->dead||g->jump>0||g->police_stop||g->dock_stage)return 0;
 int hub=nearest_hub(g);Vec3 hc=hub_position(g,hub);float range=(g->upgrades&1)?8000:2500;if(length(sub(g->pos,hc))>range){message(g,(g->upgrades&1)?"Nearest hub out of enhanced comms range.":"Nearest hub out of range. Move within 2,500 m.");return 0;}if(hub>0){g->station_variant=hub;g->docked=1;g->speed=0;g->boost=0;g->pos=hc;g->energy=100;g->heat=0;saga_dock_event(g);g->cue=SFX_DOCK;message(g,"Outpost berth secured. Station services open.");speak(g,VOICE_DOCK,"Welcome to the local relay. Berth is yours.");return 1;}
 Vec3 inside;if(station_intersection(g,g->pos,g->pos,&inside)){message(g,"Inside station hull. Guidance unavailable.");return 0;}
 g->dock_stage=1;g->dock_phase=g->pos.z<3200?2:0;g->dock_from=g->pos;g->speed=0;g->boost=0;g->approach=-1;docking_leg(g);message(g,"Docking cleared. Guidance has control.");campaign_event(g,CP_GUIDANCE);return 1;
}
static void docking_tick(Game *g,float dt){g->dock_timer+=dt;g->speed=0;g->boost=0;
 if(g->dock_stage==1){Vec3 previous=g->pos;float t=fminf(1,g->dock_timer/g->dock_duration);t=t*t*(3-2*t);g->pos=add(g->dock_from,mul(sub(g->dock_to,g->dock_from),t));Vec3 aim=norm(sub((Vec3){0,0,STATION_Z},g->pos));g->yaw=atan2f(aim.x,aim.z);g->pitch=asinf(fmaxf(-1,fminf(1,aim.y)));g->roll=station_angle(g);
  /* Guidance must cross the same aperture as a manually flown ship. */
  if(station_collision(g,previous)){if(g->dead)g->dock_stage=0;return;}
  if(g->dock_timer>=g->dock_duration){if(++g->dock_phase>3){g->dock_stage=0;message(g,"Guidance stopped. Request docking again.");}else docking_leg(g);}}
 else if(g->dock_stage==2&&g->dock_timer>=3){g->dock_stage=3;g->dock_timer=0;}
 else if(g->dock_stage==3&&g->dock_timer>=1.4f){docking_complete(g);campaign_event(g,CP_RETURN);}
}
static int station_collision(Game *g,Vec3 previous){Vec3 hit;if(!station_intersection(g,previous,g->pos,&hit))return 0;
 float roll=fabsf(sinf(g->roll-station_angle(g)));
 Vec3 start=station_local(g,previous);
 if(start.z < -STATION_HALF&&g->pos.z>previous.z&&hit.z<=-STATION_HALF+.01f&&fabsf(hit.x)<=STATION_PORT_HALF_W-STATION_SHIP_HALF_W&&fabsf(hit.y)<=STATION_PORT_HALF_H-STATION_SHIP_HALF_H&&forward(g).z>.9f&&roll<.30f&&g->speed<=200&&!g->boost){g->dock_stage=2;g->dock_timer=0;g->pos=(Vec3){0,0,STATION_ENTRY_Z};g->speed=0;g->boost=0;message(g,"Entry confirmed. Docking in progress.");return 1;}
 float a=station_angle(g);g->pos=(Vec3){hit.x*cosf(a)-hit.y*sinf(a),hit.x*sinf(a)+hit.y*cosf(a),hit.z+3500};g->pos=add(g->pos,mul(norm(sub(previous,g->pos)),5));g->dead=1;g->energy=0;g->explosion=0;g->jump=0;g->speed=0;g->boost=0;g->cue=SFX_DEATH;snprintf(g->collide,sizeof(g->collide),"%s",station_name(g));message(g,"Station hull impact. Ship destroyed.");return 1;
}
