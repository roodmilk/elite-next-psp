static void guild_dialogue(void){
 char title[96];snprintf(title,sizeof(title),"GUILD %d/4 / %s",game.guild_chapter>=4?4:game.guild_chapter+1,guild_title(&game));
 dialogue_begin(title);
 const char *a=guild_line(&game,0),*sep=strchr(a,':');char speaker[24]="GUILD";
 if(sep){int n=(int)(sep-a);if(n>23)n=23;memcpy(speaker,a,n);speaker[n]=0;a=sep+1;while(*a==' ')a++;}
 dialogue_named(speaker,EXPLORERS,a,guild_line(&game,1));
 dialogue_context("CURRENT OBJECTIVE",guild_objective(&game));
}
static void guild_screen(void){
 guild_dialogue();
 dialogue_reply(0,narrative_label(narrative_action(GUILD)));
 dialogue_reply(1,"Show me the optional flight guide.");
 narrative_footer();
}
