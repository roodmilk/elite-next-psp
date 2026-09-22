static void guild_screen(void){
 header("OPTIONAL ASSIGNMENTS");panel(8,32,464,190);
 text(2,5,GOLD,"OPTIONAL %d/4 / %s",game.guild_chapter>=4?4:game.guild_chapter+1,guild_title(&game));
 kei_speech_bubble(54,guild_line(&game,0),guild_line(&game,1),0);
 rect(16,126,448,27,RGB(13,36,43));text(3,16,GOLD,"CURRENT OBJECTIVE");text_wrap(3,18,54,1,WHITE,guild_objective(&game),0);
 text(3,20,DIM,"CHOOSE AN ACTION");
 narrative_reply_choice(0,21,narrative_label(narrative_action(GUILD)));
 narrative_reply_choice(1,24,"Show me the optional flight guide.");
 narrative_footer();
}
