/* Short factual captions, no recorded speech claimed and no stale queue. */
static struct {unsigned seed;int valid,phase,place,takedowns,final_sprint,eliminated;float last,age,cooldown;const char *copy;} race_broadcast;
static void race_broadcast_tick(void){
 SpaceRace *r=&game.race;if(!race_ready(&game)||r->phase==RACE_GRID){race_broadcast.valid=0;return;}
 if(!race_broadcast.valid||race_broadcast.seed!=r->seed||r->elapsed<race_broadcast.last){race_broadcast.seed=r->seed;race_broadcast.valid=1;race_broadcast.phase=r->phase;race_broadcast.place=r->place;race_broadcast.takedowns=r->takedowns;race_broadcast.final_sprint=race_broadcast.eliminated=0;race_broadcast.age=race_broadcast.cooldown=0;race_broadcast.copy=0;}
 float dt=fmaxf(0,fminf(.1f,r->elapsed-race_broadcast.last));race_broadcast.last=r->elapsed;race_broadcast.age+=dt;race_broadcast.cooldown=fmaxf(0,race_broadcast.cooldown-dt);
 const char *copy=0;
 int eliminated=0;for(int i=0;i<=r->rivals;i++)eliminated+=r->arcade.actor[i].eliminated;
 int final_sprint=r->arcade.rules.objective==RA_ELIMINATOR&&r->arcade.rules.required>0;
 if(r->phase==RACE_RUNNING&&race_broadcast.phase==RACE_COUNTDOWN)copy="LIGHTS OUT! LET'S RACE!";
 else if((r->phase==RACE_WON||r->phase==RACE_LOST)&&race_broadcast.phase==RACE_RUNNING)copy=r->place==1?"FIRST ACROSS THE LINE!":"CHEQUERED FLAG. WHAT A RACE!";
 else if(r->phase==RACE_RUNNING&&final_sprint&&!race_broadcast.final_sprint)copy="TWO REMAIN! FINAL SPRINT TO THE FLAG!";
 else if(r->phase==RACE_RUNNING&&eliminated>race_broadcast.eliminated)copy="LAST RACER ELIMINATED! STAY AHEAD!";
 else if(r->phase==RACE_RUNNING&&!race_broadcast.cooldown){if(r->takedowns>race_broadcast.takedowns)copy="SPECTACULAR TAKEDOWN!";else if(r->place==1&&race_broadcast.place>1&&r->arcade.rules.mode!=RA_TIME_TRIAL)copy="YOUR COMMANDER TAKES THE LEAD!";}
 if(copy){race_broadcast.copy=copy;race_broadcast.age=0;race_broadcast.cooldown=8;}
 race_broadcast.phase=r->phase;race_broadcast.place=r->place;race_broadcast.takedowns=r->takedowns;
 race_broadcast.final_sprint=final_sprint;race_broadcast.eliminated=eliminated;
}
static void race_broadcast_draw(void){
 if(!race_broadcast.copy||race_broadcast.age>6||game.race.arcade.warning>=0)return;
 int x0=clipx0,x1=clipx1,y0=clipy0,y1=clipy1;rect(348,39,124,17,RGB(7,22,32));line(348,39,471,39,RGB(91,224,210));preview_clip(proj_ox,proj_oy,351,41,469,55);
 int scroll=(int)(fmaxf(0,race_broadcast.age-.6f)*40);text_px(352-scroll,44,RGB(243,232,199),"%s   ",race_broadcast.copy);clipx0=x0;clipx1=x1;clipy0=y0;clipy1=y1;
}
