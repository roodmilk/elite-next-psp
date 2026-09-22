/* Player-facing reliability and progression helpers. */
int route_next_hop(const Game *g,int destination,int *jumps){
 int previous[256],queue[256],head=0,tail=0;*jumps=0;
 if(destination<0||destination>255||destination==g->system)return -1;
 for(int i=0;i<256;i++)previous[i]=-1;
 previous[g->system]=g->system;queue[tail++]=g->system;
 while(head<tail&&previous[destination]<0){int from=queue[head++];
  float range=(from==g->system?g->fuel:player_ships[g->ship].range)*.1f;
  for(int i=0;i<256;i++)if(previous[i]<0&&distance_ly(g,from,i)<=range+.001f){previous[i]=from;queue[tail++]=i;}
 }
 if(previous[destination]<0)return -1;
 int hop=destination;*jumps=1;
 while(previous[hop]!=g->system){hop=previous[hop];(*jumps)++;}
 return hop;
}
int mission_cargo_reserved(const Game *g,int item){
 int n=0;
 for(int i=0;i<g->job_n;i++)if((item==0&&g->jobs[i].type==MISSION_DELIVERY)||
                              (item==6&&g->jobs[i].type==MISSION_SMUGGLING))n++;
 if(!g->job_n&&g->contract>=0&&((item==0&&g->mission_type==MISSION_DELIVERY)||
                              (item==6&&g->mission_type==MISSION_SMUGGLING)))n++;
 return n;
}
int mission_target_id(const Game *g,int slot){
 if(slot<0||slot>=g->job_n||g->jobs[slot].dest!=g->system)return -1;
 const Job *j=&g->jobs[slot];
 if(j->type==MISSION_DELIVERY||j->type==MISSION_SMUGGLING||
    (j->type==MISSION_RESCUE&&j->stage))return 0;
 if(j->type==MISSION_EXPLORATION)return j->item+1;
 if(j->target>=0&&j->target<NPC_COUNT&&g->npc[j->target].alive)return BODY_COUNT+1+j->target;
 return -1;
}
int guild_claim(Game *g){
 if(!g->docked){message(g,"Dock to report to the Guild.");return 0;}
 if(g->guild_chapter>=4){message(g,"Opening assignments complete. Thank you, Commander.");return 0;}
 if(!guild_ready(g)){message(g,guild_objective(g));return 0;}
 const int rewards[]={1000,1500,2000,3000};int reward=rewards[g->guild_chapter];
 g->credits+=reward;g->guild_chapter++;g->guild_flags=0;g->cue=SFX_SELECT;
 char note[96];snprintf(note,sizeof(note),"Guild payment: %.1f units. Next assignment ready.",reward*.1f);message(g,note);
 return 1;
}
int emergency_rescue(Game *g){
 if(g->docked){message(g,"Already safe at a station.");return 0;}
 if(g->dead||g->jump>0||g->dock_stage||g->police_stop||g->planet>=0){message(g,"Recovery unavailable during this operation.");return 0;}
 int fee=g->credits<500?g->credits:500;g->credits-=fee;
 g->boost=0;g->approach=-1;g->incoming_missile=0;g->missile_time=0;g->attacked=0;
 docking_complete(g);g->fuel=(float)player_ships[g->ship].range;
 char note[96];snprintf(note,sizeof(note),"Recovered to hub. Fuel restored. Fee %.1f units.",fee*.1f);message(g,note);
 speak(g,VOICE_VENN,"You're safe. We'll charge only what you can afford.");return 1;
}
