/* Opt-in, exact EBOOT proof. Runs only in a disposable review directory. */
static int intro_film_review(void){
 FILE *flag=fopen("intro-film-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("intro-film-review.txt","w");if(!report)return 1;
 unsigned *saved=fb,*allocation=calloc(STRIDE*H+32,sizeof(unsigned));if(!allocation){fclose(report);return 1;}
 fb=allocation+16;for(int i=0;i<16;i++)allocation[i]=allocation[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0;
 #define IFCHECK(ok,label) do{int good=(ok);fprintf(report,"%s %s\n",good?"PASS":"FAIL",label);failures+=!good;fflush(report);}while(0)
 int cold_black=1;for(int b=0;b<2;b++){unsigned *plane=(unsigned*)(0x44000000u+(unsigned)b*STRIDE*H*4);for(int i=0;i<STRIDE*H;i++)cold_black&=plane[i]==RGB(0,0,0);}
 IFCHECK(cold_black,"cold startup clears every pixel and padding column in both display planes to black");
 void *displayed=NULL;int width=0,format=-1;sceDisplayGetFrameBuf(&displayed,&width,&format,PSP_DISPLAY_SETBUF_IMMEDIATE);
 IFCHECK(((unsigned)displayed&0x0fffffffu)==0x04000000u&&width==STRIDE&&format==PSP_DISPLAY_PIXEL_FORMAT_8888,"first published display is the initialized black plane in the correct pixel format");
 IFCHECK(buffer==1,"first cinematic is prepared in the hidden back plane rather than flashing over the displayed black frame");
 unsigned tile_background=RGB(11,17,25);int title_width=(int)strlen(INTRO_PRESENT_TITLE)*8,title_x=(W-title_width)/2;
 for(int i=0;i<STRIDE*H;i++)fb[i]=tile_background;
 intro_present(0);
 IFCHECK(fb[109*STRIDE+title_x]==tile_background,"presentation title and tiles start fully transparent");
 intro_present(.5f);
 IFCHECK(fb[109*STRIDE+title_x]==intro_mix(tile_background,RGB(195,41,51),128)&&fb[109*STRIDE+title_x+16]==intro_mix(tile_background,RGB(42,77,184),128),"red Cu and blue La tiles blend at half opacity");
 for(int i=0;i<STRIDE*H;i++)fb[i]=tile_background;
 intro_present(1);
 int title_bounds=1;
 for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(fb[y*STRIDE+x]!=tile_background)title_bounds&=(x>=title_x&&x<title_x+title_width&&y>=109&&y<125);
 IFCHECK(title_bounds&&fb[109*STRIDE+title_x]==RGB(195,41,51)&&fb[109*STRIDE+title_x+31]==RGB(42,77,184)&&fb[109*STRIDE+title_x+32]==tile_background,"two 16-pixel squares sit only behind Cu and La with the full title centred");
 int title_glyphs=1;const char *title=INTRO_PRESENT_TITLE;
 for(int n=0;title[n];n++)for(int y=0;y<8;y++)for(int x=0;x<8;x++)if(font8[(unsigned char)title[n]-32][y]&(1u<<(7-x)))title_glyphs&=fb[(113+y)*STRIDE+title_x+n*8+x]==(n<4?WHITE:GOLD);
 IFCHECK(title_glyphs,"exact CuLa Games presents... lettering uses the native font without clipping");
 IFCHECK(intro_present_opacity(0)==0&&intro_present_opacity(.5f)==.5f&&intro_present_opacity(1)==1&&intro_present_opacity(2)==1&&intro_present_opacity(2.8f)>.49f&&intro_present_opacity(2.8f)<.51f&&intro_present_opacity(3.2f)==0,"title fades in for one full second and fades out before the following credit");
 int gradual=1;float previous_opacity=-1;for(int step=0;step<=60;step++){float opacity=intro_present_opacity(step/60.f);gradual&=opacity>=previous_opacity&&(step==60||opacity<1);previous_opacity=opacity;}
 IFCHECK(gradual,"all sixty title fade steps increase smoothly without premature full brightness");
 FILE *audio_flag=fopen("intro-film-audio.flag","r");int with_audio=audio_flag!=NULL,audio_samples=-1;
 if(audio_flag){fclose(audio_flag);radio_station=4;radio_off=1;comms_quiet=1;radio_save_settings("radio.cfg");audio_init();}
 game_init(&game);deck_reset();intro_boot_pending=1;page=HOME;change_page(INTRO);intro_profile_ready=1;
 profile_info[0].state=profile_info[1].state=profile_info[2].state=0;
 IFCHECK(intro_time==0&&intro_menu_age<0,"normal first entry starts the continuous film with controls hidden");
 IFCHECK(game.cue==SFX_NONE&&game.voice_time==0&&game.message_time==0,"hidden setup greeting cannot emit a startup comms blip");
 unsigned original=intro_movie_fnv(&game,sizeof(game));intro_screen();
 IFCHECK(intro_movie_pixels&&intro_movie_error==0&&intro_movie_frame==0,"embedded DATA.PSAR film opens and hardware JPEG decoder produces first frame");
 if(with_audio){
  for(int wait=0;wait<120&&(audio_intro_seek_pending||audio_intro_skip_remaining||mp3_handle<0);wait++)sceKernelDelayThread(50000);
  sceKernelDelayThread(250000);audio_samples=mp3_handle>=0?sceMp3GetSumDecodedSample(mp3_handle):-1;
  IFCHECK(audio_run&&music_format==1&&radio_file_error==0&&audio_samples>0,"real supplied MP3 starts alongside the first JPEG frame");
  /* Existing settings encode OFF as their station sentinel and reload it as
   * station zero. Verify the serialized preference, not the pre-save value. */
  fprintf(report,"INFO intro source %d; saved station %d off %d quiet %d; skipped %d / %d samples\n",radio_file_station,radio_station,radio_off,comms_quiet,audio_intro_skipped_samples,audio_intro_source_rate*AUDIO_INTRO_SKIP_SECONDS);
  IFCHECK(radio_file_station==AUDIO_INTRO_STATION&&radio_station==0&&radio_off==1&&comms_quiet==1&&audio_intro_source_rate>0&&audio_intro_skipped_samples==audio_intro_source_rate*AUDIO_INTRO_SKIP_SECONDS&&!audio_intro_skip_remaining,"startup discards exactly twenty-three seconds of decoded music before playback");
  IFCHECK(audio_intro_fade_frame>0&&audio_intro_fade_frame<=AUDIO_INTRO_FADE_FRAMES&&audio_intro_gain(0)==0&&audio_intro_gain(22050)==500&&audio_intro_gain(44100)==1000,"startup music uses the one-second output fade while retaining the saved volume");
 }
 IFCHECK(intro_movie_count==1440&&intro_movie_loop==1200,"native 30fps film contains a 40-second journey and animated vista loop");
 IFCHECK(intro_movie_memory>W*H*4&&intro_movie_memory<655360,"movie uses less than 640 KiB of bounded frame memory");
 const float title_times[]={0,.25f,.5f,.75f,1,2,2.8f,3.2f};
 for(unsigned i=0;i<sizeof(title_times)/sizeof(*title_times);i++){
  intro_time=title_times[i];intro_menu_age=-1;intro_screen();char name[48];snprintf(name,sizeof(name),"intro-title-%02u.bmp",i);dump_native_bmp(name);
 }
 const float times[]={0,2,8,13,17,19,23,26.6f,27.5f,30,37,41,47.966667f,48,48.1f};
 int decoded=1;unsigned long long total=0,worst=0;
 for(unsigned i=0;i<sizeof(times)/sizeof(*times);i++){
  intro_time=times[i];intro_menu_age=-1;unsigned long long a,b;sceRtcGetCurrentTick(&a);intro_screen();sceRtcGetCurrentTick(&b);total+=b-a;if(b-a>worst)worst=b-a;
  decoded&=intro_movie_pixels&&intro_movie_error==0&&intro_movie_frame==(int)intro_movie_at(times[i]);
  char name[48];snprintf(name,sizeof(name),"intro-film-%02u.bmp",i);dump_native_bmp(name);if(with_audio)sceKernelDelayThread(100000);
 }
 IFCHECK(decoded,"all journey, flare-apex, late-vista and loop-boundary frames decode at native resolution");
 if(with_audio)IFCHECK(audio_run&&music_format==1&&radio_file_error==0&&mp3_handle>=0&&sceMp3GetSumDecodedSample(mp3_handle)>audio_samples,"MP3 samples continue advancing during JPEG journey and loop decoding");
 IFCHECK(original==intro_movie_fnv(&game,sizeof(game)),"film drawing never advances simulation, missions, inventory or commander data");
 unsigned index=intro_movie_frame;int before=intro_movie_decodes;intro_screen();
 IFCHECK(intro_movie_decodes==before&&intro_movie_frame==(int)index,"60Hz redraw reuses the current decoded 30Hz frame");
 intro_time=29.5f;intro_menu_age=1;intro_screen();unsigned hash=intro_movie_fnv(fb,STRIDE*H*4);
 intro_suspend();IFCHECK(!intro_movie_file&&!intro_movie_pixels&&!intro_movie_memory,"suspend closes the stream and frees frame buffers");
 intro_screen();IFCHECK(intro_movie_frame==885&&hash==intro_movie_fnv(fb,STRIDE*H*4),"resume reopens at the same frame and preserves the menu");
 uint32_t h[]={1,W,H,30,1440,1200,24222,0},bad[8];
 IFCHECK(intro_movie_header(h,20000000),"valid native film dimensions and limits are accepted");
 int rejected=1;for(int i=0;i<7;i++){memcpy(bad,h,sizeof(h));bad[i]=i==5?1440:0;rejected&=!intro_movie_header(bad,20000000);}
 memcpy(bad,h,sizeof(h));bad[6]=INTRO_FILM_JPEG_LIMIT+1;rejected&=!intro_movie_header(bad,20000000);
 IFCHECK(rejected,"malformed versions, dimensions, timing, loop indices and oversized frames are rejected");
 const float skip_times[]={.1f,5.f,15.f,26.f,28.f,35.f};int skip_logos=1,skip_clocks=1;
 for(int i=0;i<6;i++){
  page=INTRO;intro_time=skip_times[i];intro_menu_age=-1;intro_gate(PSP_CTRL_CROSS,PSP_CTRL_CROSS);intro_tick(.8f);intro_screen();
  skip_clocks&=page==INTRO&&fabsf(intro_time-(skip_times[i]+.8f))<.001f;
  for(int y=0;y<53;y++)for(int x=0;x<236;x++){
   unsigned p=next_logo[(y*72/53)*320+x*320/236];if(!(p&0x8000))continue;
   int r=(p>>10)&31,g=(p>>5)&31,b=p&31;unsigned ink=RGB((r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2));
   skip_logos&=fb[(y+15)*STRIDE+x+122]==ink;
  }
  if(i==0||i==2){char name[48];snprintf(name,sizeof(name),"intro-skip-logo-%d.bmp",i);dump_native_bmp(name);}
 }
 IFCHECK(skip_logos,"X at six early/middle/late film times reveals every opaque logo pixel with the menu");
 IFCHECK(skip_clocks,"revealing logo and menu preserves continuous film time and stays on title screen");
 intro_boot_pending=1;intro_enter();intro_choice=1;page=INTRO;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 IFCHECK(page==INTRO&&intro_menu_age==0,"first X reveals the menu over the moving film without starting a game");
 float clock=intro_time;
 for(int i=0;i<3;i++){input(0,0,.1f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.1f,0,0);}
 IFCHECK(page==INTRO&&intro_time>clock&&intro_menu_age>.5f,"rapid X presses cannot activate during the 800ms fade while film keeps moving");
 input(0,0,.25f,0,0);input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);
 IFCHECK(intro_choice==0,"Up selects the adjacent visible menu choice");
 input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);IFCHECK(intro_choice==1,"Down selects New Commander predictably");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 IFCHECK(page==STATUS&&game.opening.stage==OP_CREATE&&!intro_movie_pixels,"fresh X starts the existing character creator and releases film resources");
 if(with_audio){int samples=mp3_handle>=0?sceMp3GetSumDecodedSample(mp3_handle):-1;sceKernelDelayThread(150000);
  IFCHECK(audio_run&&music_format==1&&radio_file_error==0&&mp3_handle>=0&&sceMp3GetSumDecodedSample(mp3_handle)>samples,"releasing JPEG resources leaves the shared MP3 decoder playing");}
 change_page(INTRO);input(0,0,.016f,0,0);intro_choice=2;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 IFCHECK(page==STATUS&&profile_from_intro,"Load Commander still opens the real three-slot profile loader");
 change_page(INTRO);input(0,0,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);
 IFCHECK(page==MULTIPLAYER&&online_return_page==INTRO,"Square retains the existing Multiplayer screen and title return destination");
 change_page(INTRO);intro_menu_age=-1;intro_time=36.33f;intro_tick(.02f);
 IFCHECK(intro_menu_age>=0,"menu appears automatically once the camera settles at the vista");
 intro_menu_age=1;intro_time=40;intro_profile_ready=1;profile_info[0].state=1;profile_info[0].ship=21;profile_info[0].system=255;profile_info[0].credits=9000000;snprintf(profile_info[0].name,25,"ALEXANDRA STARFARER");intro_screen();dump_native_bmp("intro-long-commander.bmp");
 int guards=1;for(int i=0;i<16;i++)guards&=allocation[i]==0xa55ac33cu&&allocation[STRIDE*H+16+i]==0xa55ac33cu;
 IFCHECK(guards,"native framebuffer guard words remain intact with long commander names");
 fprintf(report,"INFO %d hardware-decode frames; %.3f ms average / %.3f ms worst sampled draw\n",intro_movie_decodes,total*1000.f/sceRtcGetTickResolution()/15,worst*1000.f/sceRtcGetTickResolution());
 if(with_audio){
  int discarded=audio_intro_skipped_samples;audio_stop();audio_init();sceKernelDelayThread(350000);
  IFCHECK(audio_run&&!audio_intro_seek_pending&&!audio_intro_skip_remaining&&audio_intro_skipped_samples==discarded,"audio restart does not repeat the twenty-three-second startup skip");
  audio_stop();
 }
 game.cue=SFX_COMM;game.voice_time=6;game.message_time=4;opening_restart_intro();
 IFCHECK(page==INTRO&&intro_time==0&&game.cue==SFX_NONE&&game.voice_time==0&&game.message_time==0,"Select replay resets the film without replaying the hidden greeting or blip");
 audio_replay_intro=0;
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);intro_leave();fb=saved;free(allocation);return 1;
 #undef IFCHECK
}
