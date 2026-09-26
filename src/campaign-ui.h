static void campaign_screen(void){
 char title[96],detail[256];
 if(tracked_mission==TRACK_STATION_TOUR&&station_tour_active()){
  dialogue_begin(station_tour_title());
  dialogue_named("STATION GUIDE",TRADERS,"Reorte Hub has a place where visiting pilots can meet the local crew.","Meet Lysa Kest at The Second Shift, Reorte Hub.");
  dialogue_context("CURRENT OBJECTIVE",station_tour_objective());
  dialogue_reply(0,"Follow next step");narrative_footer();return;
 }
 if(tracked_mission==1){
  guild_dialogue();
  dialogue_reply(0,narrative_label(narrative_action(GUILD)));narrative_footer();return;
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
  const char *next=b->objective;
  if(saga_ready(&game))next="The objective is complete. Choose Complete chapter below to report your result.";
  else if(game.system!=game.saga_dest)next="Choose Set course below. Follow the highlighted jumps on the Galaxy Map to the destination shown here.";
  else if(game.saga_chapter==1&&(game.saga_flags&SAGA_OBSERVATION_RESET))next="The observation was interrupted. Let the ship cool, jump out and return to this system before scanning the signal again.";
  else if(b->kind==SAGA_SCAN)next=game.saga_chapter==1?"Select the first anomaly in the target list, approach it and press Circle to scan. Keep the engines cool and do not fire.":"Select an unscanned anomaly in the target list, approach it and press Circle to scan. Return here once the discovery is recorded.";
  else if(b->kind==SAGA_HUNT)next="Launch if you are docked. Find a hostile ship in this system and defeat it, then return here to report.";
  else if(b->kind!=SAGA_CHOICE||game.saga_chapter==3)next="Approach the local hub and press Circle within 2,500 metres to dock. If you were already docked when accepting, launch and return to register the arrival.";
  if(game.saga_chapter==0&&!saga_ready(&game)&&game.system==game.saga_dest)next=(game.saga_flags&SAGA_CASE_HELD)?"The sealed receiver is aboard. Dock at Lave Hub, then return here to complete the delivery.":"Dock at this system's hub to collect Mara's sealed receiver. The destination will then update to Lave for the return trip.";
  dialogue_named(b->speaker,saga_speaker_role(b),b->talk8,next);
  if(b->kind==SAGA_CHOICE&&(game.saga_chapter!=3||(game.saga_flags&SAGA_TIMESTAMP_FOUND))){
   dialogue_context("YOUR DECISION",saga_choice_blurb(game.saga_chapter,row<0||row>2?0:row));
   for(int i=0;i<3;i++)dialogue_reply(i,saga_choice_label(game.saga_chapter,i));
  }else{
   int jumps=0,hop=saga_next_hop(&game,&jumps);
   if(game.system==game.saga_dest)snprintf(title,sizeof(title),"OBJECTIVE / YOU ARE IN %.24s",game.systems[game.saga_dest].name);
   else if(hop>=0&&hop!=game.saga_dest)snprintf(title,sizeof(title),"NEXT %.12s / FINAL %.12s",game.systems[hop].name,game.systems[game.saga_dest].name);
   else snprintf(title,sizeof(title),"OBJECTIVE / DESTINATION %.24s",game.systems[game.saga_dest].name);
   dialogue_context(title,b->objective);dialogue_reply(0,saga_ready(&game)?"Complete chapter":game.system!=game.saga_dest?"Set course for objective":game.docked?"Launch ship":"Return to flight");
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
 const char *a,*b;
 if(game.system!=7){
  a="I am waiting at Lave Hub. Your first flight starts and ends here, so come back to Lave before continuing.";
  b="Choose the route option below to open the Galaxy Map. Follow the selected jumps, then approach Lave Hub and press Circle to dock.";
 }else if(game.campaign_stage==0){
  a="We should discuss the flight while you are safely docked. Approach Lave Hub and press Circle when you are within 2,500 metres.";
  b="The station will bring you to a berth. Open Tracked Mission again once you are inside, and I will explain the assignment.";
 }else if(game.campaign_stage==1){
  a="Your ship is ready at the berth. Choose Launch ship below, then hold R to build speed and steer clear of the station.";
  b="There is no deadline. Press Select whenever you want to pause and read your next step here.";
 }else if(game.campaign_stage==2&&!(game.campaign_flags&CP_LOCKED)){
  a="You are clear of the berth. Keep Lave Hub on your scanner so you can find your way back.";
  b="Hold Square to open the target list. Use Left or Right to find Stations, Up or Down to select the hub, and R to lock it. Release Square to fly.";
 }else if(!(game.campaign_flags&CP_FLEW)&&game.campaign_stage<5){
  a="Keep practising your turns and speed until the flight recorder shows 600 metres travelled. You can check your progress here.";
  b="Hold R to accelerate and L to slow down. There is no need to boost or leave the Lave system.";
 }else if(game.campaign_stage<5){
  a="That is enough flight practice. Return to Lave Hub, slow down as you approach, and press Circle within 2,500 metres to request docking.";
  b="Let the arrival sequence finish. When you are inside, open Tracked Mission again to report to me and collect your reward.";
 }else{
  a="You made it back to Lave safely. Your harbour badge confirms that you can take this ship out and bring it home.";
  b="Choose Collect reward below for your 100 units. Then I will tell you about the sealed receiver Ryn left with Mara.";
 }
 dialogue_named("KEI",EXPLORERS,a,b);
 snprintf(title,sizeof(title),"NEXT STEP / FLIGHT RECORDER %.0f OF 600 M",game.campaign_distance);
 dialogue_context(game.campaign_stage>=2&&game.campaign_stage<5?title:"NEXT STEP",campaign_task(&game));
 dialogue_reply(0,narrative_label(narrative_action(CAMPAIGN)));narrative_footer();
}
