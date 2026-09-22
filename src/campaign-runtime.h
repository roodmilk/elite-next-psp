int campaign_accept(Game *g){
 if(g->campaign_stage)return 0;
 if(!g->docked||g->system!=7){message(g,"Meet Kei at Lave System Hub to begin.");return 0;}
 g->campaign_stage=1;g->campaign_flags=0;g->campaign_distance=0;
 message(g,"First flight accepted. No deadline. No weapon needed.");return 1;
}
void campaign_event(Game *g,int event){
 if(g->system!=7||!campaign_training(g))return;
 if(event==CP_LAUNCH){g->campaign_stage=1;g->campaign_flags=0;g->campaign_distance=0;g->campaign_fuel=g->fuel;}
 if(event==CP_LOCK&&g->campaign_stage>=2&&!g->docked)g->campaign_flags|=CP_LOCKED;
 if(event==CP_GUIDANCE&&g->campaign_stage>=2)g->campaign_flags|=CP_GUIDED;
 for(unsigned i=0;i<sizeof(campaign_transitions)/sizeof(campaign_transitions[0]);i++){
  const CampaignTransition *t=&campaign_transitions[i];
  if(g->campaign_stage==t->stage&&event==t->event){g->campaign_stage=t->next;break;}
 }
 if(event==CP_LAUNCH)speak(g,VOICE_VENN,"Lave traffic. New commander leaving berth six — soft launch, and keep the collar clean.");
 if(event==CP_GUIDANCE)speak(g,VOICE_VENN,"Clearance granted. I will show you the way in; prefer boring over clever.");
 if(event==CP_RETURN&&g->campaign_stage==5){
  speak(g,VOICE_KEI,"Good. Now I know where to find you when the next call goes quiet.");
  message(g,"First flight complete. Open Campaign to report to Kei.");
 }
}
static void campaign_flight(Game *g,Vec3 previous){
 if(!campaign_training(g)||g->campaign_stage<2||g->system!=7||g->dead||g->jump>0)return;
 g->campaign_distance=fminf(600,g->campaign_distance+length(sub(g->pos,previous)));
 if(g->campaign_distance>=600){g->campaign_flags|=CP_FLEW;if(g->campaign_flags&CP_LOCKED)g->campaign_stage=4;}
}
int campaign_claim(Game *g){
 if(g->campaign_stage!=5||!g->docked||g->system!=7){message(g,campaign_task(g));return 0;}
 if(g->credits>99999000){message(g,"Credit limit reached. Spend 100 units first.");return 0;}
 /* One state transition commits payment and badge together. */
 g->campaign_stage=6;g->credits+=1000;g->cue=SFX_SELECT;
 message(g,"Harbour badge earned. 100 units paid — a receipt that you left and returned.");return 1;
}
int campaign_retry(Game *g){
 if(!g->dead||g->system!=7||!campaign_training(g))return 0;
 /* Training insurance restores the existing commander, never reinitializes it.
  * Cargo, jobs, money and warrants survive. No completion event or payout.
  */
 g->campaign_stage=1;g->campaign_flags=0;g->campaign_distance=0;
 g->dead=0;g->explosion=0;g->docked=1;g->dock_stage=0;g->dock_timer=0;
 g->pos=(Vec3){0,0,STATION_Z};g->yaw=g->pitch=g->roll=g->speed=0;
 g->energy=100;g->heat=0;g->fuel=g->campaign_fuel;
 g->jump=0;g->planet=g->approach=-1;g->surface=0;g->boost=0;
 g->incoming_missile=g->missile_time=g->attacked=0;g->police_stop=0;
 g->cue=SFX_DOCK;speak(g,VOICE_VENN,"Training cover paid the repairs. Try again when your hands are steady.");
 message(g,"Back at Lave Hub. Cargo and credits kept. No reward yet.");return 1;
}
