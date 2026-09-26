/* Native-resolution captures, plus bounded text checks for the whole script. */
{
 int fits=1;
 for(int i=0;i<TUTORIAL_COUNT;i++){
  const char *s=tutorial_beats[i].copy;int col=0,lines=1;
  for(int n=0;s[n];){int word=0;while(s[n+word]&&s[n+word]!=' ')word++;
   if(col&&col+1+word>53){lines++;col=0;}col+=(col?1:0)+word;n+=word;if(s[n]==' ')n++;
  }
  if(lines>9||strlen(tutorial_beats[i].title)>53)fits=0;
 }
 INPUT_CHECK(fits,"tutorial: every script fits the native PSP briefing panel");
 tutorial_start();tutorial_prepare();tutorial_draw();dump_native_bmp("tutorial-briefing.bmp");
 game.tutorial_seen=1;rect(0,0,W,H,BG);home();tutorial_draw();dump_native_bmp("tutorial-fly-only.bmp");
 game.tutorial_step=24;game.tutorial_seen=0;tutorial_prepare();tutorial_draw();dump_native_bmp("tutorial-spacebook-brief.bmp");
 game.tutorial_seen=24;page=GALNET;galnet_tab=3;row=0;galnet_screen();dump_native_bmp("tutorial-spacebook-evidence.bmp");
 game.tutorial_step=53;game.tutorial_seen=0;tutorial_prepare();tutorial_draw();dump_native_bmp("tutorial-surface-brief.bmp");
 game.tutorial_step=0;page=INTRO;intro_time=6;intro_choice=0;intro_screen();dump_native_bmp("tutorial-title.bmp");
 TEST_INIT();page=HOME;tutorial_reset_frontend();
}
