static void campaign_screen(void){
 char title[96],detail[256];
 if(station_tour_active()){
  dialogue_begin(station_tour_title());
  dialogue_named("STATION GUIDE",TRADERS,"A short welcome tour of the authored station interior.","Meet Lysa Kest at The Second Shift, Reorte Hub.");
  dialogue_context("CURRENT OBJECTIVE",station_tour_objective());
  dialogue_reply(0,"Follow next step");narrative_footer();return;
 }
 if(tracked_mission==1){
  guild_dialogue();
  if(narrative_action(GUILD)==NA_REWARD)dialogue_reply(0,"Collect reward");
  footer(narrative_action(GUILD)==NA_REWARD?"X CHOOSE   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");return;
 }
 if(tracked_mission>=2){
  int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){tracked_mission=0;campaign_screen();return;}
  Job *j=&game.jobs[ji];snprintf(title,sizeof(title),"CONTRACT / %s",mission_name(j->type));dialogue_begin(title);
  snprintf(detail,sizeof(detail),"Destination: %s. Time remaining: %.0f seconds. Reward: %.1f U.",game.systems[j->dest].name,j->time,j->reward*.1f);
  dialogue_named("MISSION DESK",TRADERS,detail,0);
  dialogue_context("CURRENT OBJECTIVE",mission_objective_at(&game,ji));
  dialogue_reply(0,"Navigate to objective");dialogue_reply(1,"Return to mission log");narrative_footer();return;
 }
 if(game.campaign_stage>=6){
  if(game.saga_chapter>=SAGA_COUNT){
   dialogue_begin("THE OPEN CHANNEL / COMPLETE");
   dialogue_named("KEI",EXPLORERS,"Berth six is warm. The channel stays open.",saga_epilogue_line(&game));
   snprintf(title,sizeof(title),"FREE FLIGHT / TRUST P%d G%d L%d I%d",game.saga_trust[0],game.saga_trust[1],game.saga_trust[2],game.saga_trust[3]);
   dialogue_context(title,saga_trust_helper(&game));footer("SELECT MISSION LOG   O BACK");return;
  }
  if(saga_coda_pending>=0&&saga_coda_pending<SAGA_COUNT){
   int ch=saga_coda_pending;const SagaBeat *b=&saga_beats[ch];
   snprintf(title,sizeof(title),"CHAPTER %02d / CODA",ch+2);dialogue_begin(title);
   dialogue_named(saga_coda_speaker(ch),saga_speaker_role(b),saga_coda_line1(ch),saga_coda_line2(ch));
   dialogue_context("AFTERMATH","Continue when you are ready.");dialogue_reply(0,"Continue");narrative_footer();return;
  }
  const SagaBeat *b=&saga_beats[game.saga_chapter];saga_brief_reset(game.saga_chapter);
  snprintf(title,sizeof(title),"CHAPTER %02d / %.40s",game.saga_chapter+2,b->title);dialogue_begin(title);
  if(!game.saga_step){
   int beat=saga_brief_beat;if(beat<0)beat=0;if(beat>=SAGA_BRIEF_BEATS)beat=SAGA_BRIEF_BEATS-1;
   dialogue_named(saga_brief_echo?"COMMANDER":b->speaker,saga_speaker_role(b),saga_brief_echo?saga_brief_reply(b,beat):saga_brief_line(b,beat),0);
   snprintf(title,sizeof(title),"%s / %d OF %d",saga_brief_echo?"WAITING":beat<SAGA_BRIEF_BEATS-1?"YOUR REPLY":"ACCEPT",beat+1,SAGA_BRIEF_BEATS);
   dialogue_context(title,saga_brief_echo?"Hear their answer when you are ready.":beat<SAGA_BRIEF_BEATS-1?"Ask, then hear the answer.":b->objective);
   dialogue_reply(0,saga_brief_echo?"Hear the answer":saga_brief_reply(b,beat));narrative_footer();return;
  }
  dialogue_named(b->speaker,saga_speaker_role(b),b->talk8,0);
  if(b->kind==SAGA_CHOICE){
   dialogue_context("YOUR DECISION",saga_choice_blurb(game.saga_chapter,row<0||row>2?0:row));
   for(int i=0;i<3;i++)dialogue_reply(i,saga_choice_label(game.saga_chapter,i));
  }else{
   int jumps=0,hop=saga_next_hop(&game,&jumps);
   if(game.system==game.saga_dest)snprintf(title,sizeof(title),"OBJECTIVE / YOU ARE IN %.24s",game.systems[game.saga_dest].name);
   else if(hop>=0&&hop!=game.saga_dest)snprintf(title,sizeof(title),"NEXT %.12s / FINAL %.12s",game.systems[hop].name,game.systems[game.saga_dest].name);
   else snprintf(title,sizeof(title),"OBJECTIVE / DESTINATION %.24s",game.systems[game.saga_dest].name);
   dialogue_context(title,b->objective);dialogue_reply(0,saga_ready(&game)?"Complete chapter":"Set course for objective");
  }
  narrative_footer();return;
 }
 snprintf(title,sizeof(title),"CHAPTER 1 / %s",game.campaign_stage==5?"REPORT":"FIRST FLIGHT");dialogue_begin(title);
 if(prologue_brief_locked()){
  int beat=prologue_brief_beat;if(beat<0)beat=0;if(beat>=PROLOGUE_BRIEF_BEATS)beat=PROLOGUE_BRIEF_BEATS-1;
  dialogue_named("KEI",EXPLORERS,prologue_brief_line1(beat),prologue_brief_line2(beat));
  snprintf(title,sizeof(title),"YOUR REPLY / %d OF %d",beat+1,PROLOGUE_BRIEF_BEATS);
  dialogue_context(title,beat<PROLOGUE_BRIEF_BEATS-1?"Ask Kei about your first flight.":"Accept first flight when you are ready.");
  dialogue_reply(0,prologue_brief_reply(beat));narrative_footer();return;
 }
 const char *a="Return safely to Lave Hub.",*b="Docking guidance can bring you inside if you need a boring approach.";
 if(game.campaign_stage==0){a="Ryn is missing. Help me find her before the silence becomes ordinary.";b=game.system!=7?"Get to Lave Hub before we begin - I will not brief this over an empty channel.":"Dock at Lave Hub and finish this conversation with me.";}
 if(game.campaign_stage==5){a="You made it home. Thank you for treating the return like the point.";b="Collect your 100-unit harbour badge below - a receipt, not a medal.";}
 dialogue_named("KEI",EXPLORERS,a,b);dialogue_context("CURRENT OBJECTIVE",narrative_label(narrative_action(CAMPAIGN)));
 if(game.campaign_stage==0||narrative_action(CAMPAIGN)==NA_REWARD){dialogue_reply(0,narrative_label(narrative_action(CAMPAIGN)));narrative_footer();}
 else footer("SELECT MISSION LOG   O BACK");
}
