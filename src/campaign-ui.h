static void campaign_screen(void){
 header("TRACKED MISSION");panel(8,32,464,190);
 if(tracked_mission==1){text(2,5,GOLD,"EXPLORERS GUILD ASSIGNMENTS");text(2,7,CYAN,"NEXT STEP / %s",guild_title(&game));draw_icon(18,72,15,1);text(7,9,WHITE,"Independent survey work for the Guild.");text(7,11,DIM,"Assignment %d of 4",game.guild_chapter>=4?4:game.guild_chapter+1);panel(16,120,448,56);text(3,16,GOLD,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.50s",guild_objective(&game));text(3,20,CYAN,"PROGRESS");text(3,22,DIM,"Launch %s   Dock %s   Report %s",(game.guild_flags&GUILD_LAUNCH)?"DONE":"--",(game.guild_flags&GUILD_DOCK)?"DONE":"--",guild_ready(&game)?"READY":"--");footer(narrative_action(GUILD)==NA_REWARD?"X COLLECT REWARD   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");return;}
 if(tracked_mission>=2){int ji=tracked_mission-2;if(ji<0||ji>=game.job_n){tracked_mission=0;campaign_screen();return;}Job *j=&game.jobs[ji];text(2,5,GOLD,"CONTRACT / %s",mission_name(j->type));draw_icon(18,63,12+j->type,1);text(6,8,CYAN,"DESTINATION: %.18s",game.systems[j->dest].name);text(6,10,DIM,"TIME %.0fs   REWARD %.1f U",j->time,j->reward*.1f);rect(16,118,448,54,RGB(13,36,43));text(3,15,GOLD,"CURRENT OBJECTIVE");text(3,17,WHITE,"%.50s",mission_objective_at(&game,ji));text(3,20,DIM,"CHOOSE AN ACTION");narrative_choice(0,21,"Navigate to objective");narrative_choice(1,23,"Return to mission log");narrative_footer();return;}
 if(game.campaign_stage>=6){
  if(game.saga_chapter>=SAGA_COUNT){text(2,5,GOLD,"THE OPEN CHANNEL / COMPLETE");kei_speech_bubble(54,"The channel is open, Commander.","There will always be another horizon.",2);text(3,18,CYAN,"FREE FLIGHT CONTINUES");footer("SELECT MISSION LOG   O BACK");return;}
  const SagaBeat *b=&saga_beats[game.saga_chapter];text(2,5,GOLD,"CHAPTER %02d / %.31s",game.saga_chapter+2,b->title);
  panel(16,58,448,58);text(3,8,CYAN,"%s SAYS",b->speaker);text(3,10,WHITE,"%.52s",b->line);
  panel(16,124,448,45);text(3,16,GOLD,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.52s",b->objective);
  if(game.saga_step&&b->kind!=SAGA_CHOICE){int jumps=0,hop=saga_next_hop(&game,&jumps);if(game.system==game.saga_dest)text(3,20,CYAN,"YOU ARE IN %.24s",game.systems[game.saga_dest].name);else if(hop>=0&&hop!=game.saga_dest)text(3,20,DIM,"NEXT: %.12s   FINAL: %.12s",game.systems[hop].name,game.systems[game.saga_dest].name);else text(3,20,DIM,"DESTINATION: %.24s",game.systems[game.saga_dest].name);}
  if(!game.saga_step)narrative_reply_choice(0,22,"Begin chapter");
  else if(b->kind==SAGA_CHOICE){text(3,21,AMBER,"YOUR DECISION");narrative_reply_choice(0,22,"Public and transparent");narrative_reply_choice(1,24,"Explorers Guild custody");narrative_reply_choice(2,26,"Lawful independent archive");}
  else narrative_reply_choice(0,23,saga_ready(&game)?"Complete chapter":"Set course for objective");
  narrative_footer();return;
 }
 text(2,5,GOLD,"CHAPTER 1 / %s",game.campaign_stage==6?"COMPLETE":game.campaign_stage==5?"REPORT":"FIRST FLIGHT");
 const char *a="Ryn is missing. Help me find her.";
 const char *b="First, take a short flight and return.";
 if(game.campaign_stage==0&&game.campaign_choice==1){a="The catch? Come back safely.";b="I need a pilot I can trust.";}
 if(game.campaign_stage==0&&game.campaign_choice==2){a="Ryn flew a ship like this one.";b="This one is yours to borrow.";}
 if(game.campaign_stage>0&&game.campaign_stage<5){a="Return safely to Lave Hub.";b="Docking guidance can bring you inside.";}
 if(game.campaign_stage==5){a="You made it home. Thank you.";b="Collect your 100-unit reward below.";}
 if(game.campaign_stage==6){a="Your badge and reward are yours.";b="This is the end of the current chapter.";}
 kei_speech_bubble(54,a,b,game.campaign_stage>=5?2:0);
 rect(16,126,448,27,RGB(13,36,43));text(3,16,CYAN,"CURRENT OBJECTIVE");text(3,18,WHITE,"%.48s",narrative_label(narrative_action(CAMPAIGN)));
 if(game.campaign_stage==0){text(3,19,AMBER,"YOUR REPLY");narrative_reply_choice(0,20,narrative_label(narrative_action(CAMPAIGN)));narrative_reply_choice(1,22,"What is the catch?");narrative_reply_choice(2,24,"Tell me about Ryn's ship.");narrative_reply_choice(3,26,"Show me the flight controls.");narrative_footer();}
 else footer(narrative_action(CAMPAIGN)==NA_REWARD?"X COLLECT REWARD   SELECT MISSION LOG   O BACK":"SELECT MISSION LOG   O BACK");
}
