static void campaign_screen(void){
 header("TRACKED MISSION");panel(8,32,464,190);
 if(tracked_mission==1){
  text(2,5,GOLD,"EXPLORERS GUILD ASSIGNMENTS");
  draw_portrait(16,48,48,48,game.system*17+EXPLORERS,EXPLORERS);
  speaker_name_tag(10,6,"GUILD",faction_colors[EXPLORERS]);
  text(10,8,CYAN,"NEXT STEP / %s",guild_title(&game));
  text(10,10,WHITE,"Independent survey work for the Guild.");
  text(10,12,DIM,"Assignment %d of 4",game.guild_chapter>=4?4:game.guild_chapter+1);
  panel(16,120,448,56);text(3,16,GOLD,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.50s",guild_objective(&game));text(3,20,CYAN,"PROGRESS");text(3,22,DIM,"Launch %s   Dock %s   Report %s",(game.guild_flags&GUILD_LAUNCH)?"DONE":"--",(game.guild_flags&GUILD_DOCK)?"DONE":"--",guild_ready(&game)?"READY":"--");
  footer(narrative_action(GUILD)==NA_REWARD?"X COLLECT REWARD   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");return;
 }
 if(tracked_mission>=2){int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){tracked_mission=0;campaign_screen();return;}Job *j=&game.jobs[ji];text(2,5,GOLD,"CONTRACT / %s",mission_name(j->type));draw_icon(18,63,12+j->type,1);text(6,8,CYAN,"DESTINATION: %.18s",game.systems[j->dest].name);text(6,10,DIM,"TIME %.0fs   REWARD %.1f U",j->time,j->reward*.1f);rect(16,118,448,54,RGB(13,36,43));text(3,15,GOLD,"CURRENT OBJECTIVE");text(3,17,WHITE,"%.50s",mission_objective_at(&game,ji));text(3,20,DIM,"CHOOSE AN ACTION");narrative_choice(0,21,"Navigate to objective");narrative_choice(1,23,"Return to mission log");narrative_footer();return;}
 if(game.campaign_stage>=6){
  if(game.saga_chapter>=SAGA_COUNT){text(2,5,GOLD,"THE OPEN CHANNEL / COMPLETE");kei_speech_bubble(54,"The channel is open, Commander.",saga_epilogue_line(&game),2);text(3,16,CYAN,"TRUST  P%d G%d L%d I%d",game.saga_trust[0],game.saga_trust[1],game.saga_trust[2],game.saga_trust[3]);text(3,18,DIM,"%.52s",saga_trust_helper(&game));text(3,20,CYAN,"FREE FLIGHT CONTINUES");footer("SELECT MISSION LOG   O BACK");return;}
  const SagaBeat *b=&saga_beats[game.saga_chapter];unsigned ink=saga_speaker_color(b);
  saga_brief_reset(game.saga_chapter);
  text(2,5,GOLD,"CHAPTER %02d / %.31s",game.saga_chapter+2,b->title);
  saga_speaker_face(16,56,48,b);
  rect(76,54,388,62,RGB(14,29,39));rect(76,54,388,2,ink);rect(76,114,388,2,RGB(30,78,86));rect(462,54,2,62,ink);
  line(76,74,64,82,ink);line(64,82,76,90,ink);rect(73,76,4,13,RGB(14,29,39));
  speaker_name_tag(11,7,b->speaker,ink);
  if(!game.saga_step){
   int beat=saga_brief_beat;if(beat<0)beat=0;if(beat>=SAGA_BRIEF_BEATS)beat=SAGA_BRIEF_BEATS-1;
   text(11,9,WHITE,"%.43s",saga_brief_line(b,beat));
   panel(16,124,448,45);text(3,16,GOLD,"CONVERSATION");
   text(3,18,WHITE,"%.52s",beat<SAGA_BRIEF_BEATS-1?"Listen, then answer.":b->objective);
   text(3,20,AMBER,"Beat %d / %d — finish before leaving",beat+1,SAGA_BRIEF_BEATS);
   narrative_reply_choice(0,22,saga_brief_reply(beat));
   footer("X CONTINUE   (O/SELECT LOCKED)");return;
  }
  /* After accept: reinforce the next step only — no old dialogue branches. */
  text(11,9,WHITE,"%.43s",b->talk6);
  panel(16,124,448,45);text(3,16,GOLD,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.52s",b->objective);
  if(game.saga_step&&b->kind!=SAGA_CHOICE){int jumps=0,hop=saga_next_hop(&game,&jumps);if(game.system==game.saga_dest)text(3,20,CYAN,"YOU ARE IN %.24s",game.systems[game.saga_dest].name);else if(hop>=0&&hop!=game.saga_dest)text(3,20,DIM,"NEXT: %.12s   FINAL: %.12s",game.systems[hop].name,game.systems[game.saga_dest].name);else text(3,20,DIM,"DESTINATION: %.24s",game.systems[game.saga_dest].name);}
  if(b->kind==SAGA_CHOICE){text(3,21,AMBER,"YOUR DECISION");narrative_reply_choice(0,22,saga_choice_label(game.saga_chapter,0));narrative_reply_choice(1,24,saga_choice_label(game.saga_chapter,1));narrative_reply_choice(2,26,saga_choice_label(game.saga_chapter,2));}
  else narrative_reply_choice(0,23,saga_ready(&game)?"Complete chapter":"Set course for objective");
  narrative_footer();return;
 }
 text(2,5,GOLD,"CHAPTER 1 / %s",game.campaign_stage==6?"COMPLETE":game.campaign_stage==5?"REPORT":"FIRST FLIGHT");
 if(prologue_brief_locked()){
  int beat=prologue_brief_beat;if(beat<0)beat=0;if(beat>=PROLOGUE_BRIEF_BEATS)beat=PROLOGUE_BRIEF_BEATS-1;
  kei_speech_bubble(54,prologue_brief_line1(beat),prologue_brief_line2(beat),0);
  rect(16,126,448,27,RGB(13,36,43));text(3,16,CYAN,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.48s",beat<PROLOGUE_BRIEF_BEATS-1?"Finish this conversation with Kei.":"Accept first flight, then launch.");
  text(3,19,AMBER,"YOUR REPLY — Beat %d / %d",beat+1,PROLOGUE_BRIEF_BEATS);
  narrative_reply_choice(0,21,prologue_brief_reply(beat));
  footer("X CONTINUE   (O/SELECT LOCKED UNTIL ACCEPT)");return;
 }
 const char *a="Return safely to Lave Hub.";
 const char *b="Docking guidance can bring you inside.";
 if(game.campaign_stage==0){a="Ryn is missing. Help me find her.";b=game.system!=7?"Get to Lave Hub before we begin.":"Dock at Lave Hub to begin training.";}
 if(game.campaign_stage>0&&game.campaign_stage<5){a="Return safely to Lave Hub.";b="Docking guidance can bring you inside.";}
 if(game.campaign_stage==5){a="You made it home. Thank you.";b="Collect your 100-unit reward below.";}
 if(game.campaign_stage==6){a="Your badge and reward are yours.";b="This is the end of the current chapter.";}
 kei_speech_bubble(54,a,b,game.campaign_stage>=5?2:0);
 rect(16,126,448,27,RGB(13,36,43));text(3,16,CYAN,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.48s",narrative_label(narrative_action(CAMPAIGN)));
 if(game.campaign_stage==0){narrative_reply_choice(0,21,narrative_label(narrative_action(CAMPAIGN)));footer("UP/DOWN   X SELECT   O BACK");}
 else footer(narrative_action(CAMPAIGN)==NA_REWARD?"X COLLECT REWARD   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");
}
