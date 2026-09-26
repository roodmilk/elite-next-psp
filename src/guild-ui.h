static void guild_dialogue(void){
 char title[96];snprintf(title,sizeof(title),"GUILD %d/4 / %s",game.guild_chapter>=4?4:game.guild_chapter+1,guild_title(&game));
 dialogue_begin(title);
 const char *a=guild_line(&game,0),*sep=strchr(a,':');char speaker[24]="GUILD";
 if(sep){int n=(int)(sep-a);if(n>23)n=23;memcpy(speaker,a,n);speaker[n]=0;a=sep+1;while(*a==' ')a++;}
 const char *b=guild_line(&game,1);
 if(guild_ready(&game)){
  a="You have completed this assignment. The Guild has recorded your work and your reward is ready.";
  b=game.docked?"Choose Collect reward below. Once it is paid, this page will explain your next Guild assignment.":"Return to a station and press Circle within docking range. Open this mission again after you dock to collect your reward.";
 }else if(game.guild_chapter==0&&(game.guild_flags&GUILD_LAUNCH)){
  a="Your launch has been recorded. All that remains for this assignment is a safe return to a station.";
  b="Approach the hub and press Circle within 2,500 metres to request docking. Come back to this mission after the arrival sequence finishes.";
 }else if((game.guild_chapter==1||game.guild_chapter==3)&&assignment_job()>=0){
  a="You have already accepted the contract we need. Follow its objective rather than taking a second job.";
  b="Choose Track your active contract below to highlight it in Mission Log. Press Select there for that contract's next-step details.";
 }
 dialogue_named(speaker,EXPLORERS,a,b);
 dialogue_context("CURRENT OBJECTIVE",guild_objective(&game));
}
static void guild_screen(void){
 guild_dialogue();
 dialogue_reply(0,narrative_label(narrative_action(GUILD)));

 narrative_footer();
}
