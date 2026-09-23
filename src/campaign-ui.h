static void campaign_screen(void){
 header("TRACKED MISSION");panel(8,32,464,190);
 if(station_tour_active()){
  text(2,5,GOLD,"%.31s",station_tour_title());
  draw_portrait(16,48,48,48,game.system*17+TRADERS,TRADERS);
  speaker_name_tag(10,6,"STATION GUIDE",faction_colors[TRADERS]);
  text(10,8,CYAN,"NEXT STEP / REORTE HUB BAR");
  text(10,10,WHITE,"A short welcome tour of the authored station interior.");
  panel(16,120,448,56);text(3,16,GOLD,"CURRENT OBJECTIVE");
  text_wrap(3,18,54,2,WHITE,station_tour_objective(),0);
  text(3,21,CYAN,"Reorte Hub  •  THE SECOND SHIFT  •  Lysa Kest");
  footer("X FOLLOW NEXT STEP   SELECT MISSION LOG   O BACK");return;
 }
 if(tracked_mission==1){
  text(2,5,GOLD,"EXPLORERS GUILD ASSIGNMENTS");
  draw_portrait(16,48,48,48,game.system*17+EXPLORERS,EXPLORERS);
  speaker_name_tag(10,6,"GUILD",faction_colors[EXPLORERS]);
  text(10,8,CYAN,"NEXT STEP / %s",guild_title(&game));
  text(10,10,WHITE,"Independent survey work for the Guild.");
  text(10,12,DIM,"Assignment %d of 4",game.guild_chapter>=4?4:game.guild_chapter+1);
  panel(16,120,448,56);text(3,16,GOLD,"CURRENT OBJECTIVE");text_wrap(3,18,54,1,WHITE,guild_objective(&game),0);text(3,20,CYAN,"PROGRESS");text(3,22,DIM,"Launch %s   Dock %s   Report %s",(game.guild_flags&GUILD_LAUNCH)?"DONE":"--",(game.guild_flags&GUILD_DOCK)?"DONE":"--",guild_ready(&game)?"READY":"--");
  footer(narrative_action(GUILD)==NA_REWARD?"X COLLECT REWARD   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");return;
 }
 if(tracked_mission>=2){int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){tracked_mission=0;campaign_screen();return;}Job *j=&game.jobs[ji];text(2,5,GOLD,"CONTRACT / %s",mission_name(j->type));draw_icon(18,63,12+j->type,1);text(6,8,CYAN,"DESTINATION: %.18s",game.systems[j->dest].name);text(6,10,DIM,"TIME %.0fs   REWARD %.1f U",j->time,j->reward*.1f);rect(16,118,448,54,RGB(13,36,43));text(3,15,GOLD,"CURRENT OBJECTIVE");text_wrap(3,17,54,2,WHITE,mission_objective_at(&game,ji),0);text(3,20,DIM,"CHOOSE AN ACTION");narrative_choice(0,21,"Navigate to objective");narrative_choice(1,23,"Return to mission log");narrative_footer();return;}
 if(game.campaign_stage>=6){
  if(game.saga_chapter>=SAGA_COUNT){text(2,5,GOLD,"THE OPEN CHANNEL / COMPLETE");kei_speech_bubble(54,"Berth six is warm. The channel stays open.",saga_epilogue_line(&game),2);text(3,16,CYAN,"TRUST  P%d G%d L%d I%d",game.saga_trust[0],game.saga_trust[1],game.saga_trust[2],game.saga_trust[3]);text_wrap(3,18,54,1,DIM,saga_trust_helper(&game),0);text(3,20,CYAN,"FREE FLIGHT CONTINUES");footer("SELECT MISSION LOG   O BACK");return;}
  /* Act I locked coda — page script after chapter complete, before the next brief. */
  if(saga_coda_pending>=0){
   int ch=saga_coda_pending;const SagaBeat *cb=&saga_beats[ch];unsigned ink=saga_speaker_color(cb);
   text(2,5,GOLD,"CHAPTER %02d / CODA",ch+2);
   saga_speaker_face(16,56,48,cb);
   rect(76,54,388,68,RGB(14,29,39));rect(76,54,388,2,ink);rect(76,120,388,2,RGB(30,78,86));rect(462,54,2,68,ink);
   line(76,74,64,82,ink);line(64,82,76,90,ink);rect(73,76,4,13,RGB(14,29,39));
   speaker_name_tag(11,7,saga_coda_speaker(ch),ink);
   text_wrap(11,9,46,3,WHITE,saga_coda_line1(ch),0);
   panel(16,124,448,45);text(3,16,GOLD,"AFTERMATH");
   text_wrap(3,18,54,2,WHITE,saga_coda_line2(ch),0);
   narrative_reply_choice(0,22,"Continue");
   footer("X CONTINUE   (O/SELECT LOCKED)");return;
  }
  /* Note: speech bubble wrap allows three lines so chapter dialogue can breathe. */
  const SagaBeat *b=&saga_beats[game.saga_chapter];unsigned ink=saga_speaker_color(b);
  saga_brief_reset(game.saga_chapter);
  text(2,5,GOLD,"CHAPTER %02d / %.31s",game.saga_chapter+2,b->title);
  if(!game.saga_step){
   int beat=saga_brief_beat;if(beat<0)beat=0;if(beat>=SAGA_BRIEF_BEATS)beat=SAGA_BRIEF_BEATS-1;
   if(saga_brief_echo){
    player_speech_bubble(54,saga_brief_reply(b,beat));
    panel(16,124,448,45);text(3,16,GOLD,"WAITING");
    text_wrap(3,18,54,1,WHITE,"X — hear their answer.",0);
    text(3,20,AMBER,"%d / %d",beat+1,SAGA_BRIEF_BEATS);
    narrative_reply_choice(0,22,"Hear the answer");
   }else{
    saga_speaker_face(16,56,48,b);
    rect(76,54,388,68,RGB(14,29,39));rect(76,54,388,2,ink);rect(76,120,388,2,RGB(30,78,86));rect(462,54,2,68,ink);
    line(76,74,64,82,ink);line(64,82,76,90,ink);rect(73,76,4,13,RGB(14,29,39));
    speaker_name_tag(11,7,b->speaker,ink);
    text_wrap(11,9,46,3,WHITE,saga_brief_line(b,beat),0);
    panel(16,130,448,39);text(3,17,GOLD,beat<SAGA_BRIEF_BEATS-1?"YOUR REPLY":"ACCEPT");
    text_wrap(3,19,54,1,WHITE,beat<SAGA_BRIEF_BEATS-1?"Ask, then hear the answer.":b->objective,0);
    text(3,20,AMBER,"%d / %d",beat+1,SAGA_BRIEF_BEATS);
    narrative_reply_choice(0,22,saga_brief_reply(b,beat));
   }
   footer("X CONTINUE   (O/SELECT LOCKED)");return;
  }
  saga_speaker_face(16,56,48,b);
  rect(76,54,388,68,RGB(14,29,39));rect(76,54,388,2,ink);rect(76,120,388,2,RGB(30,78,86));rect(462,54,2,68,ink);
  line(76,74,64,82,ink);line(64,82,76,90,ink);rect(73,76,4,13,RGB(14,29,39));
  speaker_name_tag(11,7,b->speaker,ink);
  /* After accept: reinforce the next step only — no old dialogue branches. */
  text_wrap(11,9,46,3,WHITE,b->talk8,0);
  panel(16,124,448,45);text(3,16,GOLD,"CURRENT OBJECTIVE");text_wrap(3,18,54,2,WHITE,b->objective,0);
  if(game.saga_step&&b->kind!=SAGA_CHOICE){int jumps=0,hop=saga_next_hop(&game,&jumps);if(game.system==game.saga_dest)text(3,20,CYAN,"YOU ARE IN %.24s",game.systems[game.saga_dest].name);else if(hop>=0&&hop!=game.saga_dest)text(3,20,DIM,"NEXT: %.12s   FINAL: %.12s",game.systems[hop].name,game.systems[game.saga_dest].name);else text(3,20,DIM,"DESTINATION: %.24s",game.systems[game.saga_dest].name);}
  if(b->kind==SAGA_CHOICE){
   text(3,20,AMBER,"YOUR DECISION");
   text_wrap(3,21,54,1,DIM,saga_choice_blurb(game.saga_chapter,row<0||row>2?0:row),0);
   narrative_reply_choice(0,22,saga_choice_label(game.saga_chapter,0));
   narrative_reply_choice(1,24,saga_choice_label(game.saga_chapter,1));
   narrative_reply_choice(2,26,saga_choice_label(game.saga_chapter,2));
  }
  else narrative_reply_choice(0,23,saga_ready(&game)?"Complete chapter":"Set course for objective");
  narrative_footer();return;
 }
 text(2,5,GOLD,"CHAPTER 1 / %s",game.campaign_stage==6?"COMPLETE":game.campaign_stage==5?"REPORT":"FIRST FLIGHT");
 if(prologue_brief_locked()){
  int beat=prologue_brief_beat;if(beat<0)beat=0;if(beat>=PROLOGUE_BRIEF_BEATS)beat=PROLOGUE_BRIEF_BEATS-1;
  if(prologue_brief_echo){
   /* Commander ask is on screen; Kei's answer waits for the next Cross. */
   player_speech_bubble(54,prologue_brief_reply(beat));
   rect(16,130,448,27,RGB(13,36,43));text(3,17,CYAN,"WAITING");
   text_wrap(3,19,54,1,WHITE,"X — hear Kei's answer.",0);
   text(3,20,AMBER,"%d / %d",beat+1,PROLOGUE_BRIEF_BEATS);
   narrative_reply_choice(0,22,"Hear Kei's answer");
  }else{
   kei_speech_bubble(48,prologue_brief_line1(beat),prologue_brief_line2(beat),0);
   rect(16,130,448,27,RGB(13,36,43));text(3,17,CYAN,beat<PROLOGUE_BRIEF_BEATS-1?"YOUR REPLY":"ACCEPT");
   text_wrap(3,19,54,1,WHITE,beat<PROLOGUE_BRIEF_BEATS-1?"Ask Kei, then hear the answer.":"Accept first flight, then launch.",0);
   text(3,20,AMBER,"%d / %d",beat+1,PROLOGUE_BRIEF_BEATS);
   narrative_reply_choice(0,22,prologue_brief_reply(beat));
  }
  footer("X CONTINUE   (O/SELECT LOCKED UNTIL ACCEPT)");return;
 }
 const char *a="Return safely to Lave Hub.";
 const char *b="Docking guidance can bring you inside if the approach turns into a dare.";
 if(game.campaign_stage==0){a="Ryn is missing. Help me find her before the silence becomes ordinary.";b=game.system!=7?"Get to Lave Hub before we begin — I will not brief this over an empty channel.":"Dock at Lave Hub and finish this conversation with me.";}
 if(game.campaign_stage>0&&game.campaign_stage<5){a="Return safely to Lave Hub.";b="Docking guidance can bring you inside if you need a boring approach.";}
 if(game.campaign_stage==5){a="You made it home. Thank you for treating the return like the point.";b="Collect your 100-unit harbour badge below — a receipt, not a medal.";}
 if(game.campaign_stage==6){a="Your badge and reward are yours.";b="Open Channel briefings continue from here whenever you are ready.";}
 kei_speech_bubble(48,a,b,game.campaign_stage>=5?2:0);
 rect(16,130,448,27,RGB(13,36,43));text(3,17,CYAN,"CURRENT OBJECTIVE");text_wrap(3,19,54,1,WHITE,narrative_label(narrative_action(CAMPAIGN)),0);
 if(game.campaign_stage==0){narrative_reply_choice(0,22,narrative_label(narrative_action(CAMPAIGN)));footer("UP/DOWN   X SELECT   O BACK");}
 else footer(narrative_action(CAMPAIGN)==NA_REWARD?"X COLLECT REWARD   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");
}
