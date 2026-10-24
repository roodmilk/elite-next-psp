/* Stage calls share the completion transmission's native pixel language. */
static int expedition_call=-1,expedition_answer,expedition_history,expedition_history_stage;
static unsigned expedition_call_key;
static float expedition_call_age;
static int expedition_call_released,expedition_call_exit;
static void expedition_call_sync(void){
 if(page==INTRO){expedition_call=-1;expedition_history=0;return;}
 if(expedition_call_exit)return;
 if(expedition_history)return;
 int slot=expedition_card_slot(&game);
 if(slot<0){expedition_call=-1;return;}
 if(load_arrival_time>=0||game.dead||game.police_stop||game.dock_stage||game.jump>0||game.attacked>0||race_is_active(&game)||game.mission_receipt_n)return;
 ExpeditionState *e=&game.expeditions[slot];unsigned key=e->seed^((e->stage+1)*7919u);
 if(expedition_call!=slot||expedition_call_key!=key){expedition_call=slot;expedition_call_key=key;expedition_call_age=0;expedition_call_released=0;expedition_answer=0;game.cue=SFX_COMM;game.message_time=game.voice_time=0;}
}
static int expedition_call_open(void){return expedition_call>=0&&expedition_call<game.job_n&&expedition_stage(&game,expedition_call)&&page!=INTRO&&!game.dead&&!game.police_stop&&!game.dock_stage&&game.jump<=0&&(expedition_history||game.attacked<=0&&!race_is_active(&game))&&!game.mission_receipt_n;}
static const char *expedition_answer_label(int arg,int answer){
 if(arg==1)return answer?"Finish the contract here.":"Follow the lead for 300 more units.";
 if(arg==2)return answer?"Return now for 50 units.":"Take the reroute for 250 more units.";
 if(arg==3)return answer?"Keep the original route.":"Take the paid detour for 250 units.";
 if(arg==4)return answer?"Withdraw from the championship.":"Continue the championship series.";
 return answer?"Submit both accounts for review.":"Clear the accused pilot.";
}
static const char *expedition_ack_label(const ExpeditionStage *s){
 if(!s)return "Close the transmission.";
 switch(s->kind){
  case XP_HAIL:return "Understood. I will contact them.";
  case XP_DOCK:return "Understood. I am returning to the station.";
  case XP_COMBAT:return "I am moving in. Keep your channel open.";
  case XP_ESCORT:return "Stay close. I will cover your ships.";
  case XP_RECOVER:return "I will recover it and report back.";
  case XP_MINE:return "I will extract the required ore.";
  case XP_REFINE:return "I will process the recovered material.";
  case XP_BUY:return "I will obtain the requested cargo.";
  case XP_RACE:return "Send the start coordinates.";
  case XP_CHASE:return "I will pursue the contact.";
  case XP_PATROL:return "I will check each marked ship.";
  case XP_WAYPOINT:return "I will search those coordinates.";
  case XP_MAP:return "I will chart the marked route.";
  case XP_SITE:return "I will investigate the surface site.";
  case XP_FLORA:return "I will survey the local flora.";
  case XP_SAMPLE:return "I will collect the requested samples.";
  case XP_BEHAVIOUR:return "I will observe without disturbing it.";
  case XP_ANOMALY:return "I will scan the anomaly.";
  case XP_DISCOVERY:return "I will inspect the signal source.";
  case XP_WHALE:return "I will approach the pod carefully.";
  case XP_OBSERVE:return "I will keep my distance and observe.";
  case XP_COMPETE:return "Understood. I will recover them first.";
  default:return "Understood. I will follow the objective.";
 }
}
static int expedition_call_input(unsigned pressed,unsigned held,float dt){
 if(expedition_call_exit){if(!held)expedition_call_exit=0;return 1;}
 if(!expedition_call_open()){
  if(page==CAMPAIGN&&tracked_mission>=2&&tracked_mission-2<game.job_n&&expedition_stage(&game,tracked_mission-2)&&(pressed&PSP_CTRL_SQUARE)){expedition_call=tracked_mission-2;expedition_history=1;expedition_history_stage=game.expeditions[expedition_call].stage;expedition_call_age=0;expedition_call_released=0;return 1;}
  return 0;
 }
 expedition_call_age+=dt;if(!(held&PSP_CTRL_CROSS))expedition_call_released=1;
 const ExpeditionStage *s=expedition_stage(&game,expedition_call);
 if(pressed&PSP_CTRL_LTRIGGER&&dialogue_page>0)dialogue_page--;
 if(pressed&PSP_CTRL_RTRIGGER&&dialogue_page+1<dialogue_pages)dialogue_page++;
 if(expedition_history){
  if(pressed&PSP_CTRL_LEFT&&expedition_history_stage>0)expedition_history_stage--;
  if(pressed&PSP_CTRL_RIGHT&&expedition_history_stage<(int)game.expeditions[expedition_call].stage)expedition_history_stage++;
  if(pressed&PSP_CTRL_CIRCLE){expedition_history=0;expedition_call=-1;expedition_call_exit=1;}return 1;
 }
 if(s->kind==XP_CHOICE)expedition_answer=dialogue_move(expedition_answer,2,pressed);
 if(expedition_call_age>.5f&&expedition_call_released&&(pressed&PSP_CTRL_CROSS)){
  int slot=expedition_call;expedition_ack(&game,slot);if(s->kind==XP_CHOICE)expedition_choice(&game,slot,expedition_answer);
  expedition_call=-1;expedition_call_exit=1;fire_blocked=1;tools_block_until_release();game.cue=SFX_SELECT;
 }return 1;
}
static void expedition_call_draw(void){
 if(!expedition_call_open())return;int slot=expedition_call;const ExpeditionDef *d=expedition_def(game.jobs[slot].type);ExpeditionState *e=&game.expeditions[slot];unsigned stage=expedition_history?expedition_history_stage:e->stage;const ExpeditionStage *s=&d->stages[stage];
 char title[96],sender[96];snprintf(title,sizeof(title),"%s / %s",expedition_history?"RECORD":"MISSION",d->name);
 snprintf(sender,sizeof(sender),"%s / %u OF %d %s",d->sender,stage+1,d->count,stage<e->stage?"COMPLETE":"CURRENT");
 int saved=row;row=expedition_answer;dialogue_begin(title);dialogue_named(d->sender,d->role,s->speech,0);dialogue_objective(s->objective);
 /* Stage status belongs in the sender display, without changing identity. */
 snprintf(dialogue.speaker,sizeof(dialogue.speaker),"%s",sender);
 if(expedition_history)footer("LEFT/RIGHT STAGES   L/R READ   O RETURN");
 else if(s->kind==XP_CHOICE){dialogue_reply(0,expedition_answer_label(s->arg,0));dialogue_reply(1,expedition_answer_label(s->arg,1));footer("LEFT/RIGHT   X CONFIRM");}
 else {row=0;dialogue_reply(0,expedition_ack_label(s));footer("X ACKNOWLEDGE");}
 row=saved;
 float t=fminf(1,expedition_call_age/.48f);int ox=(int)(480*(1-t)*(1-t)*(1-t));
 if(ox>0){for(int y=0;y<H;y++){memmove(fb+y*STRIDE+ox,fb+y*STRIDE,(W-ox)*sizeof(unsigned));rect(0,y,ox,1,RGB(5,10,18));}}
}
static void expedition_flight_sync(void){
 if(!expedition_ready(&game)||!game.expedition_runtime.built)return;const ExpeditionStage *s=expedition_stage(&game,game.job_sel);if(s->kind==XP_DISCOVERY&&discovery_count()>0)expedition_external(&game,DISCOVERY_ID_MIN,discovery_position(0));else if(s->kind==XP_WHALE||s->kind==XP_OBSERVE)expedition_external(&game,WHALE_TARGET_ID,whale_position(&game,0));
}
static int expedition_header_info(char*out,int cap){
 if(!expedition_ready(&game)||game.docked||game.dock_stage||race_is_active(&game))return 0;
 const ExpeditionStage*s=expedition_stage(&game,game.job_sel);ExpeditionState*e=&game.expeditions[game.job_sel];
 if(s->kind==XP_DOCK||s->kind==XP_BUY||s->kind==XP_CHOICE||s->kind==XP_RACE)return 0;
 if(s->loc>=3&&(game.planet!=expedition_world(&game,game.job_sel)||s->kind!=XP_SITE&&s->kind!=XP_BEHAVIOUR))return 0;
 snprintf(out,cap,"STAGE %u/%d %u/%d %.1fKM",e->stage+1,expedition_def(game.jobs[game.job_sel].type)->count,e->progress,s->need,length(sub(expedition_waypoint(&game),game.pos))*.001f);return 1;
}
static void expedition_flight_draw(void){
 if(!expedition_ready(&game)||game.docked||game.dock_stage||race_is_active(&game))return;
 const ExpeditionStage *s=expedition_stage(&game,game.job_sel);ExpeditionState *e=&game.expeditions[game.job_sel];
 if(s->kind==XP_DOCK||s->kind==XP_BUY||s->kind==XP_CHOICE||s->kind==XP_RACE)return;
 if(s->loc>=3&&(game.planet!=expedition_world(&game,game.job_sel)||s->kind!=XP_SITE&&s->kind!=XP_BEHAVIOUR))return;
 Vec3 v=camera(&game,expedition_waypoint(&game));Point p=project(v);unsigned ink=UI_SIGNAL;
 if(v.z>20&&p.x>18&&p.x<462&&p.y>48&&p.y<190){float fx=p.x,fy=p.y;hc_indicator_anchor(&fx,&fy);int x=fx,y=fy;line(x-8,y-6,x-8,y-2,ink);line(x-8,y-6,x-4,y-6,ink);line(x+8,y+6,x+8,y+2,ink);line(x+8,y+6,x+4,y+6,ink);}
 else {float dx=v.x,dy=-v.y;if(v.z<0){dx=-dx;dy=-dy;}float m=fmaxf(1,sqrtf(dx*dx+dy*dy));float fx=240+dx/m*185,fy=120+dy/m*55;hc_indicator_anchor(&fx,&fy);int x=fx,y=fy;line(x,y,x-(int)(dx/m*8-dy/m*5),y-(int)(dy/m*8+dx/m*5),ink);line(x,y,x-(int)(dx/m*8+dy/m*5),y-(int)(dy/m*8-dx/m*5),ink);}
 /* Progress is presented in the full cockpit's header without covering tools. */
 if(hc_full_console_visible())return;
 /* Reserved small strip above the lower cockpit, never over radio or radar. */
 rect(8,185,226,14,RGB(7,17,25));text_px(12,188,ink,"STAGE %u/%d  %u/%d  %.1fKM",e->stage+1,expedition_def(game.jobs[game.job_sel].type)->count,e->progress,s->need,length(sub(expedition_waypoint(&game),game.pos))*.001f);
}
