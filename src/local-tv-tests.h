/* Opt-in native tests: fake only wall clock, never renderer/input. */
static void local_tv_tests(void){
 FILE *f=fopen("local-tv-check.txt","w");if(!f)return;int failures=0;
#define TV_CHECK(c,n) do{int ok=(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",n);if(!ok)failures++;}while(0)
 unsigned *old=fb,*pixels=malloc((STRIDE*H+32)*sizeof(unsigned)),*previous=malloc(STRIDE*H*sizeof(unsigned));
 TV_CHECK(pixels&&previous,"native framebuffer allocation");
 if(pixels&&previous){
  fb=pixels;for(int i=STRIDE*H;i<STRIDE*H+32;i++)pixels[i]=0xdeadbeef;
  game_init(&game);deck_reset();story_complete(&game);page=HOME;row=26;paused=0;ps_open=ps_release=0;quiet_comms=0;
  local_tv_test_time=8*3600+1;game_input(PSP_CTRL_CROSS,0,.016f,0,0);
  TV_CHECK(page==LOCALTV&&local_tv_program==0&&local_tv_reveal==28,"Discover joins current live programme, not beginning");
  int units=game.credits,system=game.system;float hull=game.hull;
  char label[16];local_tv_time_label(0,label,sizeof(label));TV_CHECK(!strcmp(label,"08:00"),"top-right local clock");
  local_tv_time_label(1,label,sizeof(label));TV_CHECK(!strcmp(label,"08:05"),"NEXT start time");
  local_tv_time_label(2,label,sizeof(label));TV_CHECK(!strcmp(label,"08:10"),"LATER start time");
  game_input(PSP_CTRL_LEFT|PSP_CTRL_RIGHT|PSP_CTRL_CROSS,0,.016f,0,0);
  TV_CHECK(local_tv_program==0&&local_tv_reveal==28,"buttons cannot tune or restart live broadcast");
  local_tv_test_time=8*3600+299.999;local_tv_tick(.016f);TV_CHECK(local_tv_program==0,"programme remains until exact boundary");
  local_tv_test_time=8*3600+300;local_tv_tick(.016f);TV_CHECK(local_tv_program==1&&local_tv_beat==0&&local_tv_reveal==0,"automatic five-minute programme switch");
  local_tv_test_time=8*3600+600;local_tv_tick(.016f);TV_CHECK(local_tv_program==2,"third scheduled programme");
  local_tv_test_time=8*3600+900;local_tv_tick(.016f);TV_CHECK(local_tv_program==0,"schedule repeats after fifteen minutes");
  local_tv_test_time=86399;local_tv_tick(.016f);local_tv_time_label(1,label,sizeof(label));TV_CHECK(local_tv_program==2&&!strcmp(label,"00:00"),"NEXT midnight wrap");
  local_tv_time_label(2,label,sizeof(label));TV_CHECK(!strcmp(label,"00:05"),"LATER midnight wrap");
  local_tv_test_time=0;local_tv_tick(.016f);TV_CHECK(local_tv_program==0&&local_tv_reveal==0,"midnight changes broadcast cleanly");
  local_tv_test_time=8*3600+301;local_tv_open();TV_CHECK(local_tv_program==1&&local_tv_reveal==28,"reopen/resume/clock change seeks current broadcast");
  for(int fps=30;fps<=120;fps*=2){for(int i=0;i<fps;i++){local_tv_test_time=8*3600+(i+1.0)/fps;local_tv_tick(1.f/fps);}TV_CHECK(local_tv_reveal==28,"typewriter agrees at 30/60/120 fps");}
  for(int c=0;c<3;c++){local_tv_program=c;for(int b=0;b<4;b++){local_tv_beat=b;TV_CHECK(local_tv_caption(191,0),"caption fits native rows");}
   local_tv_test_time=8*3600+c*300+5;local_tv_tick(.016f);local_tv_screen();char name[64];snprintf(name,sizeof(name),"tv-programme-%d.bmp",c);dump_native_bmp(name);
  }
  local_tv_test_time=86399;local_tv_tick(.016f);local_tv_screen();dump_native_bmp("tv-midnight.bmp");
  local_tv_test_time=8*3600+.01;local_tv_tick(.016f);local_tv_clock=0;local_tv_screen();memcpy(previous,pixels,STRIDE*H*sizeof(unsigned));
  local_tv_clock=.3f;local_tv_screen();int face=0,window=0,escaped=0;
  for(int y=24;y<188;y++)for(int x=8;x<366;x++)if(pixels[y*STRIDE+x]!=previous[y*STRIDE+x]){if(x>=128&&x<=142&&y>=85&&y<=95)face++;else if((x>=166&&x<198&&y>=41&&y<98)||(x>=199&&x<=311&&y>=40&&y<=143))window++;else escaped++;}
  TV_CHECK(face>0,"Mira mouth moves at original lips");TV_CHECK(window>0,"window animates");TV_CHECK(escaped==0,"animations stay inside studio masks");
  local_tv_clock=0;local_tv_screen();memcpy(previous,pixels,STRIDE*H*sizeof(unsigned));local_tv_clock=10;local_tv_screen();int ships=0;
  for(int y=110;y<132;y++)for(int x=199;x<312;x++)ships+=pixels[y*STRIDE+x]!=previous[y*STRIDE+x];TV_CHECK(ships>10,"ships visibly travel through lower window");
  local_tv_audio_update();TV_CHECK(audio_tv_on&&audio_tv_code,"speaking publishes TV syllable to mixer");
  quiet_comms=1;local_tv_audio_update();TV_CHECK(audio_tv_on&&!audio_tv_code,"quiet chatter mutes voice");quiet_comms=0;
  local_tv_reveal=(int)strlen(local_tv_copy());local_tv_audio_update();TV_CHECK(!audio_tv_code&&!local_tv_speaking(),"caption hold rests mouth and silences chatter");
  local_tv_screen();memcpy(previous,pixels,STRIDE*H*sizeof(unsigned));local_tv_clock+=.3f;local_tv_screen();face=0;for(int y=85;y<=95;y++)for(int x=128;x<=142;x++)face+=pixels[y*STRIDE+x]!=previous[y*STRIDE+x];TV_CHECK(face==0,"completed caption mouth rests");
  LocalTVVoice voice={0};int peak=0,jump=0,last=0,heard=0;FILE *pcm=fopen("tv-chatter.pcm","wb");
  for(int i=0;i<44100*6;i++){local_tv_test_time=8*3600+(double)i/44100;local_tv_tick(0);local_tv_audio_update();int s=local_tv_voice_sample(&voice,audio_tv_code);if(abs(s)>peak)peak=abs(s);if(abs(s-last)>jump)jump=abs(s-last);last=s;heard+=s!=0;short sample=(short)s;if(pcm)fwrite(&sample,2,1,pcm);}
  if(pcm)fclose(pcm);TV_CHECK(heard>1000&&peak<=1024,"babble generates bounded non-silent PCM");TV_CHECK(jump<256,"voice waveform has no full-scale edges");
  for(int i=0;i<4410;i++)last=local_tv_voice_sample(&voice,0);TV_CHECK(last==0,"voice fades to exact silence");
  local_tv_clock=0;for(int i=0;i<40;i++){local_tv_test_time=8*3600+i*.16;local_tv_tick(.16f);local_tv_screen();char name[64];snprintf(name,sizeof(name),"tv-motion-%02d.bmp",i);dump_native_bmp(name);}
  game.message_time=5;snprintf(game.message,sizeof(game.message),"Must not cover TV");local_tv_screen();memcpy(previous,pixels,STRIDE*H*sizeof(unsigned));menu_notice();TV_CHECK(!memcmp(previous,pixels,STRIDE*H*sizeof(unsigned)),"old notices cannot cover broadcast");
  TV_CHECK(game.credits==units&&game.system==system&&game.hull==hull,"TV never changes gameplay state");
  int guards=1;for(int i=STRIDE*H;i<STRIDE*H+32;i++)if(pixels[i]!=0xdeadbeef)guards=0;TV_CHECK(guards,"framebuffer bounds intact");
  game_input(PSP_CTRL_CIRCLE|PSP_CTRL_RIGHT,0,.016f,0,0);TV_CHECK(page==HOME&&row==26&&!audio_tv_on&&!audio_tv_code,"Circle returns and stops TV audio");
  local_tv_test_time=-1;local_tv_tick(0);TV_CHECK(local_tv_clock_valid,"PSP real local clock is available");
 }
 local_tv_test_time=-1;audio_tv_on=audio_tv_code=0;fb=old;free(pixels);free(previous);fprintf(f,"RESULT %d failures\n",failures);fclose(f);
#undef TV_CHECK
}
